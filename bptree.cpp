/*
 * bptree.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#include "bptree.h"
#include <stdexcept>
#include <cstring>
#include <iostream>

using namespace std;

namespace BPlusTree{

bp_tree::bp_tree(const char *name, bt_key* itr) : fName(name), file(NULL), fhelp(NULL), key_itr(itr) {
	file = fopen(name, "r+b");
	if (file == NULL) {
		// Creating new btree file
		file = fopen(name, "w+b");
		if (file == NULL)
				throw new std::runtime_error("Can not open/create B+ tree file");

		// Add initial directory to file
		fhelp = new file_helper(file);
		fhelp->write_page(0, dir.page_block());
	} else {
		fhelp = new file_helper(file);
		// Read existing directory page out
		fhelp->read_page(0, dir.page_block());
	}
}

bp_tree::~bp_tree() {
	if (file != NULL) fclose(file);
	if (fhelp != NULL) delete fhelp;
}

int bp_tree::delete_entry(bt_key *key, RID rid) {
	int ret = -1;
	if (dir.root() < 1){
		return -1;
	}
	cout <<"root is page: "<<dir.root()<<endl;
	page_node pg(dir.root());
	fhelp->read_page(dir[pg.page_id()], pg.page_block());
	cout<<"test"<<endl;

	//while(!pg.is_leaf_node()){
		int entrypg = pg.findEntry(key,key_itr);
		pg.set_id(entrypg);
		fhelp->read_page(dir[pg.page_id()],pg.page_block());
		cout<<"page "<<pg.page_id()<<" is leaf? "<<pg.is_leaf_node()<<endl;

		entrypg = pg.findEntry(key,key_itr);
		pg.set_id(entrypg);
		fhelp->read_page(dir[pg.page_id()],pg.page_block());
		cout<<"page "<<pg.page_id()<<" is leaf? "<<pg.is_leaf_node()<<endl;
	/*
	}
	cout<<"page before delete:"<<endl;
	pg.print_leaf(key_itr);
	ret = pg.delete_leaf(key,rid,key_itr);
	pg.print_leaf(key_itr);
	return ret;
	*/
}

void bp_tree::insert_entry(bt_key *key, RID rid) {

	if (dir.root() < 1) {
		cout << "Init root node" << endl;
		// empty btree
		page_node root(Leaf, 1, 0, 0);
		dir[1] = 1;
		fhelp->write_page(dir[root.page_id()], root.page_block());
		dir.update_root(root.page_id());
		fhelp->write_page(0, dir.page_block());
	}


	page_node root(dir.root());
	fhelp->read_page(dir[root.page_id()], root.page_block());
	key = insert_to_page(root, key, rid);
	if (key != NULL){
		cout << "Grow Up Root" << endl;
		uint16_t splitpage = 0;
		for (uint16_t i=1;i<PAGE_SIZE/sizeof(uint16_t);i++){
			if(dir[i]==0){
				splitpage = i;
				break;
			}
		}
		page_node newroot(Index, splitpage, 0, 0);
		dir.update_root(newroot.page_id());
		cout<<"root is now: "<<newroot.page_id()<<endl;
		dir[splitpage] = splitpage;
		uint16_t id = root.page_id();
		memcpy(newroot.content_block(), &id, sizeof(int16_t));
		memcpy(newroot.content_block() + sizeof(root.page_id()), key->data(), key->length());
		memcpy(newroot.content_block() + sizeof(int16_t) + key->length(), &root.right_id(), sizeof(int16_t));
		newroot.end_offset() = key->length() + 2 * sizeof(int16_t);
		fhelp->write_page(dir[newroot.page_id()],newroot.page_block());
	}
	//insert_to_page
}

bt_key* bp_tree::insert_to_page(page_node& pg, bt_key* key, RID rid) {
	//cout << "In page " << pg.page_id() << "Is Leaf? " << pg.is_leaf_node() << endl;
	if (pg.is_leaf_node()) {
		if (key->length() + sizeof(rid) <= PAGE_SIZE - pg.end_offset() - sizeof(int16_t) * 4){
			//cout << "Insert " << key->to_string() << " to Page " << pg.page_id() << endl;
			pg.insert(key,rid,key_itr);
			fhelp -> write_page(pg.page_id(),pg.page_block());
			//cout << "Write to Page " << pg.page_id() << endl;
			return NULL;
		}else{
			uint16_t splitpage = 0;
			for (uint16_t i=1;i<PAGE_SIZE/sizeof(uint16_t);i++){
				if(dir[i]==0){
					splitpage = i;
					break;
				}
			}
			dir[splitpage] = splitpage;
			page_node splitpg(Leaf, splitpage, pg.page_id(), pg.right_id());
			pg.right_id() = splitpage;
			int flag = 1,splitpos = 0;
			splitpos = pg.findHalf(key,rid,flag,key_itr);
			memcpy(splitpg.content_block(), pg.content_block() + splitpos, pg.end_offset() - splitpos);

			splitpg.end_offset() = pg.end_offset() - splitpos; // Update new page end_off
			pg.end_offset() = splitpos;	// Update old page end_off


			if (flag == 0)
				pg.insert(key,rid,key_itr);
			else splitpg.insert(key,rid,key_itr);

			bt_key* newkey = key;
			newkey -> load(splitpg.content_block());
			fhelp -> write_page(0,dir.page_block());
			fhelp -> write_page(pg.page_id(),pg.page_block());
			fhelp -> write_page(splitpg.page_id(),splitpg.page_block());

			std::cout << "Split happen(leaf):\n Left: " << std::endl;
			pg.print_leaf(key_itr);
			std::cout << "Right:" << std::endl;
			splitpg.print_leaf(key_itr);
			return newkey;
		}
		// insert into leaf node
		// if splitting happen return a key ptr
		// else return NULL ptr
	} else {
		// Page is Index Node, find next step
		int test = pg.findEntry(key, key_itr);
		//cout<< "entry: "<< test<<endl;
		page_node child_pg(test);
		fhelp->read_page(child_pg.page_id(),child_pg.page_block());
		key = insert_to_page(child_pg,key,rid);
		if (key != NULL){
			cout << "Put key up to the index" << endl;
			if(key->length() + sizeof(int16_t) <= PAGE_SIZE - pg.end_offset() - sizeof(int16_t) * 4){
				//cout << "Enough room in current index node" << endl;
				rid.pageNum = child_pg.right_id();
				pg.insert(key,rid,key_itr);
				fhelp -> write_page(pg.page_id(), pg.page_block());
				return NULL;
			}
			else{
				uint16_t splitpage = 0;
				for (uint16_t i=1;i<PAGE_SIZE/sizeof(uint16_t);i++){
					if(dir[i]==0){
						splitpage = i;
						break;
					}
				}
				dir[splitpage] = splitpage;
				page_node splitpg(Index,splitpage,pg.page_id(),pg.right_id());
				pg.right_id() = splitpage;
				int flag = 1,splitpos = 0;
				splitpos = pg.findHalf(key,rid,flag,key_itr);
				if(flag == 0){
					key_itr->load(pg.content_block() + splitpos);
					memcpy(splitpg.content_block(),pg.content_block() + splitpos + key_itr->length() ,pg.end_offset() - splitpos - key_itr->length());
					splitpg.end_offset() = pg.end_offset() - splitpos -  key_itr->length(); // update new end of right leaf;
					pg.right_id() = splitpg.page_id(); // update right id of original leaf
					pg.end_offset() = splitpos; // update new end offset of orginal leaf
					void *tempkey = malloc(key_itr->length());
					memcpy(tempkey,key_itr->data(),key_itr->length());
					rid.pageNum = child_pg.right_id();
					pg.insert(key,rid,key_itr);
					key->load(tempkey);
					free(tempkey);
					cout<<"Split happen at index(left) flag = 0"<<endl;
					pg.print_index(key_itr);
					cout<<"right"<<endl;
					splitpg.print_index(key_itr);
					return key;
				}
				else {
					key_itr-> load(pg.content_block() + splitpos);
					if(*key < *key_itr){
						page_node childpg(*(int16_t*)(pg.content_block() + splitpos - sizeof(int16_t)));
						fhelp->read_page(childpg.page_id(), childpg.page_block());
						memcpy(splitpg.content_block(), &childpg.right_id(), sizeof(int16_t));
						memcpy(splitpg.content_block() + sizeof(int16_t), pg.content_block() + splitpos, pg.end_offset() - splitpos);
						splitpg.end_offset() = pg.end_offset() - splitpos + sizeof(int16_t);
						pg.end_offset() = splitpos;
						pg.right_id() = splitpg.page_id();
						cout<<"Split happen at index(left)"<<endl;
						pg.print_index(key_itr);
						cout<<"right"<<endl;
						splitpg.print_index(key_itr);
						return key;
					}else{
						key_itr->load(pg.content_block() + splitpos);
						memcpy(splitpg.content_block(),pg.content_block() + splitpos + key_itr->length(),pg.end_offset() - splitpos - key_itr->length());
						cout<<"right length"<<pg.end_offset() - splitpos - key_itr->length()<<endl;
						splitpg.end_offset() = pg.end_offset() - splitpos - key_itr->length();
						pg.right_id() = splitpg.page_id();
						pg.end_offset() = splitpos;
						rid.pageNum = child_pg.right_id();
						splitpg.insert(key,rid,key_itr);
						key->load(pg.content_block() + splitpos);
						cout<<"Split happen at index(left)"<<endl;
						pg.print_index(key_itr);
						cout<<"right"<<endl;
						splitpg.print_index(key_itr);
						return key;
					}
				}
				fhelp ->write_page(0,dir.page_block());
				fhelp ->write_page(pg.page_id(),pg.page_block());
				fhelp ->write_page(splitpg.page_id(),splitpg.page_block());
			}
		}else return NULL;
		// find next page, read it out
		// insert_to_page(child_pg, key, rid);
		// if splitting happen return a key ptr
		// else return NULL ptr
	}
	return NULL;
}

}

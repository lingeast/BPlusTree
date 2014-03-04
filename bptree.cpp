/*
 * bptree.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#include "bptree.h"
#include <stdexcept>
#include <cstring>

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

void bp_tree::insert_entry(bt_key *key, RID rid) {

	//insert_to_page
}

bt_key* bp_tree::insert_to_page(page_node& pg, bt_key* key, RID rid) {
	if (pg.is_leaf_node()) {
		if (key->length() + sizeof(rid) <= PAGE_SIZE - pg.end_offset()){
			pg.insert(key,rid,key_itr);
			fhelp -> write_page(pg.page_id(),pg.page_block());
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
			pg.end_offset() = splitpos;
			if (flag == 0)
				pg.insert(key,rid,key_itr);
			else splitpg.insert(key,rid,key_itr);

			key -> load(splitpg.content_block());
			fhelp -> write_page(0,dir.page_block());
			fhelp -> write_page(pg.page_id(),pg.page_block());
			fhelp -> write_page(splitpg.page_id(),splitpg.page_block());
			return key;
		}
		// insert into leaf node
		// if splitting happen return a key ptr
		// else return NULL ptr
	} else {
		page_node child_pg(pg.findEntry(key, key_itr));
		fhelp->read_page(child_pg.page_id(),child_pg.page_block());
		key = insert_to_page(child_pg,key,rid);
		if (key != NULL){
			if(key->length() + sizeof(int16_t) <= PAGE_SIZE - pg.end_offset()){
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
				int flag = 0,splitpos = 0;
				splitpos = pg.findHalf(key,rid,flag,key_itr);
				if(flag == 0){
					memcpy(splitpg.content_block(),pg.content_block() + splitpos ,pg.end_offset() - splitpos);
					pg.insert(key,rid,key_itr);
					key->load(pg.content_block() + splitpos);
					return key;
				}
				else {
					key_itr-> load(pg.content_block() + splitpos);
					if(key < key_itr){
						memcpy(splitpg.content_block(),pg.content_block() + splitpos,pg.end_offset() - splitpos);
						return key;
					}else{
						key_itr->load(pg.content_block() + splitpos);
						memcpy(splitpg.content_block(),pg.content_block() + splitpos + key_itr->length(),pg.end_offset() - splitpos - key_itr->length());
						splitpg.insert(key,rid,key_itr);
						key->load(pg.content_block() + splitpos);
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

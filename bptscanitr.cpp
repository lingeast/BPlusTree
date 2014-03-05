/*
 * bptscanitr.cpp
 *
 *  Created on: Mar 4, 2014
 *      Author: root
 */

#include "bptscanitr.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

#define SCAN_EOF -1
namespace BPlusTree {

bpt_scan_itr::bpt_scan_itr(std::string fname, bt_key* lok, bt_key* hik, bool lo_in, bool hi_in):
		file(fopen(fname.c_str(),"r")), fhelp(file),
		lo_inc(lo_in), hi_inc(hi_in), offset(0), key_itr(lok->clone()), hi_key(hik->clone()) {
	// TODO Auto-generated constructor stub
	fhelp.read_page(0, this->dir.page_block()); // read in dir page

	cur_leaf.set_id(dir.root()); // read in root page
	fhelp.read_page(dir[cur_leaf.page_id()], cur_leaf.page_block());

	while(!cur_leaf.is_leaf_node()) { //keep moving until reach leaf node
		int entry = cur_leaf.findEntry(lok, key_itr);
		cur_leaf.set_id(entry);
		fhelp.read_page(dir[cur_leaf.page_id()], cur_leaf.page_block());
	}

	this->offset = this->begin_offset(cur_leaf, lok, lo_inc); // set up offset
}

bpt_scan_itr::~bpt_scan_itr() {
	// TODO Auto-generated destructor stub
	close();
}

int bpt_scan_itr::begin_offset(const page_node& pg, bt_key *lok, bool lo_in) {
	if (!pg.is_leaf_node()) {
		throw new std::logic_error("Not a leaf node!");
	}
	int offset = 0;
	while(true) {
		if(offset >= pg.end_offset()) break;
		key_itr->load(pg.content_block() + offset);
		if (lo_in) {
			if (!(*key_itr < *lok)) break;
		} else {
			if (!(*key_itr < *lok) && !(*key_itr == *lok)) break;
		}
		offset += key_itr->length() + sizeof(RID);
	}
	return offset;
}

int bpt_scan_itr::get_next(RID& rid, void* key) {

	if (this->file == NULL || this->key_itr == NULL || this->hi_key == NULL) {
		// *this has been already closed
		return SCAN_EOF;
	}
	std::cout << "In page " << cur_leaf.page_id() << " , offset = " << offset << std::endl;
	while (offset >= cur_leaf.end_offset()) {
		if (cur_leaf.right_id() == 0) {
			std::cout << "No right children. Exit" << std::endl;
			return SCAN_EOF; // reach end of right most node
		}
		else { // read next leaf node to cur_leaf, reset offset
			cur_leaf.set_id(cur_leaf.right_id());
			fhelp.read_page(dir[cur_leaf.page_id()], cur_leaf.page_block());
			offset = 0;
		}
	}

	key_itr->load(cur_leaf.content_block() + offset);
	if (hi_inc) {
		if (!(*key_itr < *hi_key)  && !(*key_itr == *hi_key)) return SCAN_EOF;
	} else {
		if (!(*key_itr < *hi_key)) return SCAN_EOF;
	}

	memcpy(key, key_itr->data(), key_itr->length());
	memcpy(&rid, cur_leaf.content_block() + offset + key_itr->length(), sizeof(rid));
	// incre key length()
	offset += key_itr->length() + sizeof(rid);
	return 0;

}

} /* namespace BPlusTree */

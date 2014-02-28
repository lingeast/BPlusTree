/*
 * bptree.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#include "bptree.h"
#include <stdexcept>

namespace BPlusTree{

bp_tree::bp_tree(const char *name) : fName(name), file(NULL), fhelp(NULL) {
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

void bp_tree::insert_entry(bt_key *key, serializable *rid) {


}

bt_key* bp_tree::insert_to_page(page_node& pg, bt_key* key, serializable* rid) {
	if (pg.is_leaf_node()) {
		// insert into leaf node
		// if splitting happen return a key ptr
		// else return NULL ptr
	} else {
		// going through index node
		// if splitting happen return a key ptr
		// else return NULL ptr
	}
	return NULL;
}

}

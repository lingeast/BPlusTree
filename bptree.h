/*
 * bptree.h
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#ifndef BPTREE_H_
#define BPTREE_H_

#include <string>
#include <cstdio>
#include "key/btkey.h"
#include "page/page.h"
#include "file/file_helper.h"

using std::string;

namespace BPlusTree{

class bp_tree {
private:
	string fName;
	FILE* file;
	file_helper* fhelp;
	dir_page dir;
	bt_key* insert_to_page(page_node& pg, bt_key* key, serializable* rid);
public:
	bp_tree(const char *name);
	~bp_tree();

	void insert_entry(bt_key* key, serializable* rid);

	void delete_entry(bt_key* key, serializable* rid);
};


}
//(FileHandle &fileHandle, const Attribute &attribute, const void *key, const RID &rid)
//{

#endif /* BPTREE_H_ */

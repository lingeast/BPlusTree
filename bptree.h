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

/*
typedef struct
{
  unsigned pageNum;
  unsigned slotNum;
} bpRID;
*/

class bp_tree {
private:
	string fName;
	FILE* file;
	file_helper* fhelp;
	bt_key* key_itr;
	dir_page dir;
	bt_key* insert_to_page(page_node& pg, bt_key* key, RID rid);
public:
	bp_tree(const char *name, bt_key* itr);
	~bp_tree();

	void insert_entry(bt_key*, RID);

	int delete_entry(bt_key*, RID);
};


}
//(FileHandle &fileHandle, const Attribute &attribute, const void *key, const RID &rid)
//{

#endif /* BPTREE_H_ */

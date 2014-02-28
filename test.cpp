/*
 * main.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#include <iostream>
#include "bptree.h"
#include "key/varcharkey.h"
using std::cout;
using std::endl;

using namespace BPlusTree;

void testKey(bt_key* key) {
	cout << key->to_string() << endl;
}
int main() {
	//BPlusTree::bp_tree indexTree("my_name");
	cout << "Hello B+ Tree" << endl;
	varchar_key* key = new varchar_key();
	testKey(key);
	delete key; key = NULL;
	page_node (Leaf, 0, 0, 0);
}



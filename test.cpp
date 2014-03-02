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
using std::string;

using namespace BPlusTree;

void testKey(bt_key* key) {
	cout << key->to_string() << endl;
}
int main() {
	//BPlusTree::bp_tree indexTree("my_name");
	cout << "Hello B+ Tree" << endl;
	string s1("Yidong_Ling");
	char* raw = new char[sizeof(uint32_t) + s1.size()];
	*(uint32_t*)raw = s1.size();
	memcpy(raw + sizeof(uint32_t), s1.c_str(), s1.size());
	varchar_key* key = new varchar_key();
	key->load((void*) raw);
	testKey(key);
	memcpy(raw,key->data(), key->length());
	key->load((void*) raw);
	testKey(key);
	delete key; key = NULL;
}



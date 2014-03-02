/*
 * main.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#include <iostream>
#include "bptree.h"
#include "key/varcharkey.h"
#include "key/intkey.h"
#include "key/floatkey.h"
#include <cstring>

using std::cout;
using std::endl;
using std::string;

using namespace BPlusTree;

void testKey(bt_key* key) {
	cout << key->to_string() << endl;
}

void testCompKey(bt_key* k1, bt_key* k2) {
	if (*k1 < *k2) {
		cout << k1->to_string() << " smaller than " << k2->to_string() << endl;
	} else {
		cout << k2->to_string() << " (equal or) smaller than " << k1->to_string() << endl;
	}
}

int main() {
	//BPlusTree::bp_tree indexTree("my_name");
	cout << "Hello B+ Tree" << endl;
	string s1("Yidong");

	char* raw1 = new char[sizeof(uint32_t) + s1.size()];
	*(uint32_t*)raw1 = s1.size();
	memcpy(raw1 + sizeof(uint32_t), s1.c_str(), s1.size());
	varchar_key* key1 = new varchar_key();
	key1->load((void*) raw1);
	testKey(key1);

	string s2("Google");
	char* raw2 = new char[sizeof(uint32_t) + s2.size()];
	*(uint32_t*)raw2 = s2.size();
	memcpy(raw2 + sizeof(uint32_t), s2.c_str(), s2.size());
	varchar_key* key2 = new varchar_key();
	key2->load((void*) raw2);
	testKey(key2);

	memcpy(raw1, key2->data(), key2->length());
	key1->load(raw1);
	testCompKey(key1, key2);
	delete key1;
	delete key2;


	int32_t val1 = 99999;
	int32_t val2 = -1;
	int_key* ik1 = new int_key();
	int_key* ik2 = new int_key();
	ik1->load(&val1);
	ik2->load(&val2);
	testCompKey(ik2, ik1);
	memcpy(&val1, ik2->data(), ik2->length());
	cout << "After Swap: val1 = " << val1 << endl;

	float f1 = 19.442;
	float f2 = 2839.8;

	float_key* fk1 = new float_key();
	float_key* fk2 = new float_key();
	fk1->load(&f1);
	fk2->load(&f2);
	testCompKey(fk2, fk1);
}



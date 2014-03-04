/*
 * main.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "bptree.h"
#include "key/varcharkey.h"
#include "key/intkey.h"
#include "key/floatkey.h"


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

void test1() {
	cout << "########Test(1) Key Begin: " << endl;

	string s1("Yidong");

	char* raw1 = new char[sizeof(uint32_t) + s1.size()];
	*(uint32_t*)raw1 = s1.size();
	memcpy(raw1 + sizeof(uint32_t), s1.c_str(), s1.size());
	varchar_key* key1 = new varchar_key();
	key1->load((void*) raw1);
	//testKey(key1);

		string s2("Google");
		char* raw2 = new char[sizeof(uint32_t) + s2.size()];
		*(uint32_t*)raw2 = s2.size();
		memcpy(raw2 + sizeof(uint32_t), s2.c_str(), s2.size());
		varchar_key* key2 = new varchar_key();
		key2->load((void*) raw2);
		//testKey(key2);

		memcpy(raw1, key2->data(), key2->length());
		key1->load(raw1);
		testCompKey(key1, key2);
		delete key1;
		delete key2;

		delete raw1;
		delete raw2;


		int32_t val1 = 21379;
		int32_t val2 = -13218;
		int_key* ik1 = new int_key();
		int_key* ik2 = new int_key();
		ik1->load(&val1);
		ik2->load(&val2);
		testCompKey(ik2, ik1);
		//memcpy(&val1, ik2->data(), ik2->length());
		//cout << "After Swap: val1 = " << val1 << endl;
		delete ik1;
		delete ik2;

		float f1 = 19.442;
		float f2 = 2839.8;

		float_key* fk1 = new float_key();
		float_key* fk2 = new float_key();
		fk1->load(&f1);
		fk2->load(&f2);
		//fk1->load(fk2->data());
		testCompKey(fk2, fk1);

		delete fk1;
		delete fk2;

		cout << "########Test(1) Key End.\n\n" << endl;
}

void test2() {
	cout << "########Test(2) insertion begin" << endl;
	string bname("IntTestFile.bpt");

	int_key* ikey = new int_key();
	int_key* iitr = new int_key();

	bp_tree test_bpt(bname.c_str(), iitr);
	RID rid;
	rid.slotNum = 0; rid.pageNum = 0;

	for (int i = 0; i < 10000; i++) {
		int32_t num = rand() % 1000;
		ikey->load(&num);
		test_bpt.insert_entry(ikey, rid);
	}
	cout << "########Test(2) insertion end. \n\n" << endl;
}
int main() {
	//BPlusTree::bp_tree indexTree("my_name");
	cout << "Hello B+ Tree" << endl;
	test1();
	test2();

}



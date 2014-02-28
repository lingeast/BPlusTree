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

using BPlusTree::bp_tree;

int main() {
	BPlusTree::bp_tree indexTree("my_name");
	cout << "Hello B+ Tree" << endl;
}



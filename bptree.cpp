/*
 * bptree.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#include "bptree.h"

namespace BPlusTree{

bp_tree::bp_tree(const char *name) : fName(name), file(NULL) {
	file = fopen(name, "r");
	if (file == NULL) {
		// Creating new btree file, Do some Initialization here
	}
}

bp_tree::~bp_tree() {
	fclose(file);
}

}

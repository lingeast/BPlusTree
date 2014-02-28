/*
 * varcharkey.cpp
 *
 *  Created on: Feb 27, 2014
 *      Author: yidong
 */

#include "varcharkey.h"

size_t varchar_key::len(){
	return 1;
}

void* varchar_key::data() const {
	return NULL;
}

	// load from memory, serialzable_ptr->load(ptr_to_position);
void varchar_key::load(const void *) {
	return;
}

	// Used for debug
std::string varchar_key::to_string() const {
	return std::string("polymorphism is hard in C++");
}

bool varchar_key::operator<(const comparable &) {
	return false;
}

bool varchar_key::operator==(const comparable &) {
	return false;
}



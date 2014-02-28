/*
 * varcharkey.cpp
 *
 *  Created on: Feb 27, 2014
 *      Author: yidong
 */

#include "varcharkey.h"

varchar_key::varchar_key() : raw_data(NULL), len(0) {}

varchar_key::~varchar_key() {
	if (raw_data != NULL)
		delete raw_data;
}
size_t varchar_key::length(){
	return 1;
}

void* varchar_key::data() const {
	return NULL;
}

	// load from memory, serialzable_ptr->load(ptr_to_position);
void varchar_key::load(const void *data) {
	return;
}

	// Used for debug
std::string varchar_key::to_string() const {
	return std::string("polymorphism is hard in C++");
}

bool varchar_key::operator<(const comparable & that) {
	return false;
}

bool varchar_key::operator==(const comparable & that) {
	return false;
}



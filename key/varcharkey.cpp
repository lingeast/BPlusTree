/*
 * varcharkey.cpp
 *
 *  Created on: Feb 27, 2014
 *      Author: yidong
 */

#include "varcharkey.h"
#include <iostream>
#include <cstring>
#include <stdint.h>

varchar_key::varchar_key() : raw_data(NULL), len(0), str() {}

varchar_key::varchar_key(const varchar_key &that) :
		raw_data(NULL), len(that.len), str(that.str) {
	this->raw_data = new char[this->len];
	memcpy(this->raw_data, that.raw_data, this->len);
}

varchar_key::~varchar_key() {
	if (raw_data != NULL)
		delete raw_data;
}
size_t varchar_key::length() const {
	return len;
}

const void* varchar_key::data() const {
	return (void*) raw_data;
}

	// load from memory, serialzable_ptr->load(ptr_to_position);
void varchar_key::load(const void *out_data) {
	if (raw_data != NULL) delete raw_data;
	int strlen = *(uint32_t*)out_data;
	this->len = strlen + sizeof(uint32_t);
	raw_data = new char[this->len];
	memcpy(raw_data, out_data, strlen + sizeof(uint32_t));

	string str1(raw_data + sizeof(uint32_t), strlen);
	//string str1("made");
	//::cout << str1 << std::endl;
	str = str1;
}

	// Used for debug
std::string varchar_key::to_string() const {
	return str;
}


bool varchar_key::operator<(const varchar_key & that) {
	return this->str < that.str;
}

bool varchar_key::operator==(const varchar_key & that) {
	return this->str == that.str;
}

bool varchar_key::operator<(const comparable &rhs) {
    const varchar_key *pRhs = dynamic_cast<const varchar_key *>(&rhs);
    if (pRhs == NULL) { // rhs is of another type
         return false;
     } else {
          // Do work to compare two Derived objects
    	 return (str.compare(pRhs->str) < 0);
     }
}

bool varchar_key::operator==(const comparable &rhs)  {
    const varchar_key *pRhs = dynamic_cast<const varchar_key *>(&rhs);
    if (pRhs == NULL) { // rhs is of another type
         return false;
     } else {
          // Do work to compare two Derived objects
    	 return this->str == pRhs->str;
     }
}




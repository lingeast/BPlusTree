/*
 * floatkey.cpp
 *
 *  Created on: Mar 1, 2014
 *      Author: yidong
 */

#include "floatkey.h"
#include <cstring>
#include <stdio.h>
#include <iostream>

float_key::float_key() : len(0), val(0) {
	// TODO Auto-generated constructor stub

}

float_key::float_key(const float_key & that):
		len(that.len), val(that.val) {}

float_key::~float_key() {
	// TODO Auto-generated destructor stub
}

void float_key::load(const void* data) {
	memcpy(&val, data, sizeof(float_key_t));
	len = sizeof(float_key_t);
	//float val = *(float*)data;
	//std::cout << "Read in " << val << std::endl;
}

std::string float_key::to_string() const {
	char buffer [33];
	float tmp;
	memcpy(&tmp, &(this->val), sizeof(this->val));
	sprintf(buffer,"%f",tmp);
	return std::string(buffer);
}

bool float_key::operator<(const float_key &that) {
	//std::cout << "In < op" << std::endl;
	if (sizeof(float) == sizeof(val)) {
		float lhs, rhs;
		memcpy(&lhs, &val, sizeof(lhs));
		memcpy(&rhs, &(that.val), sizeof(rhs));
		//std::cout << "Compare " << lhs <<" and " << rhs << std::endl;
		return lhs < rhs;
	} else if (sizeof(double) == sizeof(val)) {
		double lhs, rhs;
		memcpy(&lhs, &val, sizeof(lhs));
		memcpy(&rhs, &(that.val), sizeof(rhs));
		return lhs < rhs;
	} else {
		return false; // fail to compare
	}

}
bool float_key::operator==(const float_key &that) {
	if (sizeof(float) == sizeof(val)) {
		float lhs, rhs;
		memcpy(&lhs, &val, sizeof(lhs));
		memcpy(&rhs, &(that.val), sizeof(rhs));
		return lhs == rhs;
	} else if (sizeof(double) == sizeof(val)) {
		double lhs, rhs;
		memcpy(&lhs, &val, sizeof(lhs));
		memcpy(&rhs, &(that.val), sizeof(rhs));
		return lhs == rhs;
	} else {
		return false; // fail to compare
	}
}

bool float_key::operator<(const comparable &rhs) {
    const float_key *pRhs = dynamic_cast<const float_key *>(&rhs);
    if (pRhs == NULL) { // rhs is of another type
    	return false;
    } else {
    	return (*this < *pRhs);
    }
}
bool float_key::operator==(const comparable &rhs) {
    const float_key *pRhs = dynamic_cast<const float_key *>(&rhs);
    if (pRhs == NULL) { // rhs is of another type
    	return false;
    } else {
    	return (*this == *pRhs);
    }
}

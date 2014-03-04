/*
 * intkey.cpp
 *
 *  Created on: Mar 1, 2014
 *      Author: yidong
 */

#include "intkey.h"
#include <stdio.h>
int_key::int_key(): val(0), len(0) {
	// TODO Auto-generated constructor stub
}

int_key::int_key(const int_key& that) : val(that.val) , len(that.len){
}

int_key::~int_key() {
	// TODO Auto-generated destructor stub
}

void int_key::load(const void* data) {
	this->val = *(int_key_t*)data;
	len = sizeof(int_key_t);
}

std::string int_key::to_string() const {
	char buffer [33];
	sprintf(buffer,"%d",this->val);
	return std::string(buffer);
}

bool int_key::operator <(const comparable &rhs) {
    const int_key *pRhs = dynamic_cast<const int_key *>(&rhs);
    if (pRhs == NULL) { // rhs is of another type
    	return false;
    } else {
    	return (this->val < pRhs->val);
    }
}

bool int_key::operator ==(const comparable & rhs) {
    const int_key *pRhs = dynamic_cast<const int_key *>(&rhs);
    if (pRhs == NULL) { // rhs is of another type
    	return false;
    } else {
    	return (this->val == pRhs->val);
    }
}

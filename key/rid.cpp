/*
 * rid.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: root
 */

#include "rid.h"
#include <stdlib.h>
#include <cstring>
#include <cstdio>

namespace BPlusTree {

rid::rid() {
	// TODO Auto-generated constructor stub
	id = 0;
}

rid::~rid() {
	// TODO Auto-generated destructor stub
}

void rid::load(const void* data) {
	memcpy(&id, data, sizeof(id));
}

std::string rid::to_string() const {
	char buffer [33];
	sprintf(buffer,"%d",id);
	return std::string(buffer);
}

} /* namespace BPlusTree */

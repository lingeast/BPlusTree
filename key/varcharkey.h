/*
 * varcharkey.h
 *
 *  Created on: Feb 27, 2014
 *      Author: yidong
 */

#ifndef VARCHARKEY_H_
#define VARCHARKEY_H_

#include "btkey.h"

using namespace BPlusTree;

class varchar_key : public bt_key {
public:
	size_t len();
	void* data() const ;

	// load from memory, serialzable_ptr->load(ptr_to_position);
	void load(const void *);
	// Used for debug
	std::string to_string() const;


	bool operator<(const comparable &) = 0;
	bool operator==(const comparable &) = 0;

	~varchar_key(){}
};

#endif /* VARCHARKEY_H_ */

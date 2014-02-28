/*
 * varcharkey.h
 *
 *  Created on: Feb 27, 2014
 *      Author: yidong
 */

#ifndef VARCHARKEY_H_
#define VARCHARKEY_H_

#include "btkey.h"
#include <string>

using std::string;
using namespace BPlusTree;

class varchar_key : public bt_key {
private:
	char* raw_data;
	int len;
	string str;
public:
	size_t length();
	void* data() const ;

	// load from memory, serialzable_ptr->load(ptr_to_position);
	void load(const void *);
	// Used for debug
	std::string to_string() const;

	varchar_key();
	bool operator<(const comparable &);
	bool operator==(const comparable &);

	~varchar_key();
};

#endif /* VARCHARKEY_H_ */

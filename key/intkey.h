/*
 * intkey.h
 *
 *  Created on: Mar 1, 2014
 *      Author: yidong
 */

#ifndef INTKEY_H_
#define INTKEY_H_

#include "btkey.h"
#include <stdint.h>

typedef int32_t int_key_t;

class int_key: public BPlusTree::bt_key {
private:
	int_key_t val;
	size_t len;
public:
	int_key();
	int_key(const int_key &);
	virtual ~int_key();

	size_t length() const {return len;}
	const void * data() const {return &val;}

	// load from memory, serialzable_ptr->load(ptr_to_position);
	void load(const void *);
	// Used for debug
	std::string to_string() const;

	//bool operator<(const int_key &that) {return val < that.val;}
	//bool operator==(const int_key &that) {return val == that.val;}

	bool operator<(const comparable &);
	bool operator==(const comparable &);

	bt_key* clone() const {
		return new int_key(*this);
	}

};

#endif /* INTKEY_H_ */

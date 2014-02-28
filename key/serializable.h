/*
 * writable.h
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#ifndef WRITABLE_H_
#define WRITABLE_H_

#include <string>

namespace BPlusTree{

class serializable {
public:
	/*	len() and data() together are used to make it memcpyable
	 * memcpyable : memcpy(dst, serialzable_ptr->data(), serialzable_ptr->len());
	 *
	 *	Empty serializable obj is determined by len() == 0 && data() == NULL
	 */
	virtual size_t length() = 0;
	virtual void* data() const = 0;

	// load from memory, serialzable_ptr->load(ptr_to_position);
	virtual void load(const void *) = 0;

	virtual ~serializable(){}; // do nothing

	// Used for debug
	virtual std::string to_string() const = 0;

};


}


#endif /* WRITABLE_H_ */

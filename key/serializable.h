/*
 * writable.h
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#ifndef WRITABLE_H_
#define WRITABLE_H_

namespace BPlusTree{

class serializable {
public:
	// memcpyable : memcpy(dst, serialzable_ptr->data(), serialzable_ptr->len());
	virtual size_t len() = 0;
	virtual void* data() const = 0;

	// load from memory, serialzable_ptr->load(ptr_to_position);
	virtual void load(const void *) = 0;

	virtual ~serializable() = 0;

};


}


#endif /* WRITABLE_H_ */

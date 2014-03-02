/*
 * rid.h
 *
 *  Created on: Feb 28, 2014
 *      Author: root
 */

#ifndef RID_H_
#define RID_H_

#include "serializable.h"
#include <stdint.h>

namespace BPlusTree {

class rid: public BPlusTree::serializable {
private:
	int id;
public:
	size_t length(){return sizeof(id);}
	void* data() const {return (void *)&id;}

	// load from memory, serialzable_ptr->load(ptr_to_position);
	void load(const void *) = 0;


	// Used for debug
	std::string to_string() const = 0;
	rid();
	~rid();
};

} /* namespace BPlusTree */

#endif /* RID_H_ */

/*
 * btkey.h
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#ifndef BTKEY_H_
#define BTKEY_H_

#include "serializable.h"
#include "comparable.h"

namespace BPlusTree{

class bt_key : public serializable , public comparable{
	/*
	 * Other requirement on bt_key:
	 * 1. It should have deep-copy copy constructor
	 */
public:
	virtual ~bt_key(){}
	//virtual bt_key* clone() const = 0;
};

}


#endif /* BTKEY_H_ */

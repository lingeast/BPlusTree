/*
 * comparable.h
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#ifndef COMPARABLE_H_
#define COMPARABLE_H_

namespace BPlusTree{

class comparable {
public:
	virtual bool operator<(const comparable &) = 0;
	virtual bool operator==(const comparable &) = 0;
	virtual ~comparable() = 0;
};

}

#endif /* COMPARABLE_H_ */

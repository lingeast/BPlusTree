/*
 * page_size.h
 *
 *  Created on: Feb 27, 2014
 *      Author: yidong
 */

#ifndef PAGE_SIZE_H_
#define PAGE_SIZE_H_

#include <stdint.h>

namespace BPlusTree{

// page size = 4k byte
static unsigned int const PAGE_SIZE = 4096;

// use 16bit to represent page offset in a file
// support up to 2048 node
typedef uint16_t p_offset;
typedef uint16_t p_id;

}

#endif /* PAGE_SIZE_H_ */

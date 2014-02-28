/*
 * file_helper.h
 *
 *  Created on: Feb 26, 2014
 *      Author: yidong
 */

#ifndef FILE_HELPER_H_
#define FILE_HELPER_H_

#include <cstdio>
#include <cstdlib>
#include "../page/page_size.h"

namespace BPlusTree{
/*
 * class file_helper handles read/write page with a give page number
 * it DOES NOT take care of the open and close of a FILE pointer
 */
class file_helper {
private:
	FILE* file;
	//dir_page dir;
	void move_cursor(size_t offset);
	void read_page_block(size_t offset, void * data);
	void write_page_block(size_t offset, const void *data);

public:
	file_helper(): file(NULL) {}
	file_helper(FILE* f);	// keeps a FILE* ptr
	~file_helper() {;} // do nothing
	void read_page(unsigned int slot, void * data);
	void write_page(unsigned int slot, const void * data);
};
}



#endif /* FILE_HELPER_H_ */

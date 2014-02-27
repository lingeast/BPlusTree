/*
 * file_helper.h
 *
 *  Created on: Feb 26, 2014
 *      Author: yidong
 */

#ifndef FILE_HELPER_H_
#define FILE_HELPER_H_

#include <cstdio>
#include "page/page.h"
namespace BPlusTree{
/*
 * class file_helper handles read/write page with a give page number
 * it DOES NOT take care of the open and close of a FILE pointer
 */
class file_helper {
private:
	FILE* file;
	dir_page dir;
	void file_helper::move_cursor(size_t offset);
	void file_helper::read_page_block(size_t offset, void* data);
	void file_helper::write_page_block(size_t offset, const void *data);

public:
	file_helper(FILE* f);	// keeps a FILE* ptr
	~file_helper() {;} // do nothing
};
}



#endif /* FILE_HELPER_H_ */

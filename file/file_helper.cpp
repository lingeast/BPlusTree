/*
 * file_helper.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: yidong
 */

#include <cassert>
#include <cstdio>
#include "file_helper.h"
#include <stdexcept>

namespace BPlusTree{

file_helper::file_helper(FILE* f) : file(f) {
	if (f == NULL)
		throw std::runtime_error("file_helper: constructor receive null file ptr.");

	move_cursor(0);
	//read_page_block(0, dir.page_block());	// Init dir_page
}

void file_helper::read_page(unsigned int slot, void * data) {
	if ( slot >= PAGE_SIZE / sizeof(p_offset)) {
		throw std::out_of_range("file_helper: Receive slot num out of boundary");
	}
	read_page_block(slot * PAGE_SIZE, data);
}

void file_helper::write_page(unsigned int slot, const void* data) {
	if ( slot >= PAGE_SIZE / sizeof(p_offset)) {
		throw std::out_of_range("file_helper: Receive slot num out of boundary");
	}
	write_page_block(slot * PAGE_SIZE, data);
}

void file_helper::move_cursor(size_t offset) {	// Calling fseek to move file pointer
	assert (file != NULL);
	if (fseek(file, offset, SEEK_SET) != 0)
		throw std::runtime_error("file_helper: Move cursor failed.");
}

void file_helper::read_page_block(size_t offset, void* data) {
	assert(file != NULL);
	move_cursor(offset);
	if (1 != fread(data, PAGE_SIZE, 1, file))
		throw std::runtime_error("file_helper: Read page block failed.");
}

void file_helper::write_page_block(size_t offset, const void *data) {
	assert(file != NULL);
	move_cursor(offset);
	if(1 != fwrite(data, PAGE_SIZE, 1, file))
		throw std::runtime_error("file_helper: Write page block failed.");
}

}

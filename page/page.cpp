/*
 * page.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#include "page.h"
#include <stdexcept>

namespace BPlusTree{
page_node::page_node() : pageID(-1), is_leaf(NULL),
						left(NULL), right(NULL), parent(NULL),
						num(NULL), content(NULL){}






p_offset& dir_page::operator [](int slot) const {
	if (slot < 0 || slot >= PAGE_SIZE / sizeof(p_offset)) {
		throw new std::out_of_range("dir_page::operator[]");
	}
	return *((p_offset*) this->page + slot);
}

}



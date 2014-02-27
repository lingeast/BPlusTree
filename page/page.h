/*
 * page.h
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#ifndef PAGE_H_
#define PAGE_H_

#include <stdint.h>
#include <cstdlib>

namespace BPlusTree{

static int const PAGE_SIZE = 4096;

typedef uint32_t p_offset; // use 32bit to represent page offset in a file

class page_node {
private:
	char page[PAGE_SIZE];
	int pageID;
	uint8_t* is_leaf;	// is_left == 1 => is leaf page, == 0 => is index page
	uint16_t* left;		// page id of left sibling
	uint16_t* right;	// page id of right sibling
	uint16_t* parent;	// page id of parent
	uint16_t* num;		// number of keys on page
	const char* content;
public:
	page_node();
	void* content_block() const {return (void*) content;}
	void* page_block() const {return (void*) page;}
	bool is_leaf_node() const {return *is_leaf == 0;}
	// Pass reference to outside
	uint16_t& left_id() {return *left;}
	uint16_t& right_id() {return *right;}
	uint16_t& parent_id() {return *parent;}
	uint16_t& size() {return *num;}
};



class dir_page {
private:
	char page[PAGE_SIZE];
public:
	dir_page() {} // do nothing
	void* page_block() const {return (void*) page;} // expose the page block to outside
	p_offset& operator[](int slot) const;
	int addr(int slot) {return operator[](slot);}
};

}
#endif /* PAGE_H_ */

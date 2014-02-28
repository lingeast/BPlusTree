/*
 * page.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#include "page.h"
#include <stdexcept>
#include <cstring>

namespace BPlusTree{

void page_node::insert_to_index(bt_key* key, serializable* rid, bt_key* itr) {
	;
}

void page_node::insert_to_leaf(bt_key* key, serializable* rid, bt_key* itr) {
	// always insert to end, no order preserved
	memcpy(content + *end, key->data(), key->length());
	*end += key->length();
	memcpy(content + *end, rid->data(), rid->length());
	*end += rid->length();
}
page_node::page_node(int id) : pageID(id), is_leaf((uint16_t*)page),
						left(is_leaf + 1), right(left + 1),
						end(right + 1), content((char*)(end + 1)){}

page_node::page_node(NodeType nt, int id, int left_id, int right_id) : pageID(id), is_leaf((uint16_t*)page),
						left(is_leaf + 1), right(left + 1),
						end(right + 1), content((char*)(end + 1)){
	*is_leaf = (nt == Leaf) ? 1 : 0;
	*left = left_id;
	*right = right_id;
	*end = 0;
}

void page_node::insert(bt_key* key, serializable* rid, bt_key* itr) {
	if (is_leaf_node()) insert_to_leaf(key, rid, itr);
	else insert_to_index(key, rid, itr);
}

dir_page::dir_page() {
	memset(&page, 0, sizeof(page));
}

p_offset& dir_page::operator [](int slot) const {
	if (slot < 0 || slot >= PAGE_SIZE / sizeof(p_offset)) {
		throw new std::out_of_range("dir_page::operator[]");
	}
	return *((p_offset*) this->page + slot);
}

}



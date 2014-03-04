/*
 * page.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#include "page.h"
#include <stdexcept>
#include <cstring>
#include <iostream>

namespace BPlusTree{

void page_node::print_index(bt_key *itr) const {
	int offset = sizeof(uint16_t);
	int length = *(this->end);
	while (offset < length) {
		itr->load(content + offset);
		std::cout << itr->to_string() << ", ";
		offset += itr->length() + sizeof(uint16_t);
	}
	std::cout << std::endl;
}

void page_node::print_leaf(bt_key* itr) const {
	int offset = 0;
	int length = *(this->end);
	while (offset < length) {
		// use itr to load
		itr->load(this->content + offset);
		std::cout << itr->to_string() << ", ";
		// incre offset
		offset += itr->length() + sizeof(RID);
	}
	std::cout << std::endl;
}

void page_node::insert_to_index(bt_key* key, RID rid, bt_key* itr) {
	int offset = sizeof(uint16_t);
	while(true){
		if(offset >= *end) break;
		itr->load(content + offset);
		if (!(*itr < *key)) break;
		offset += itr->length() + sizeof(int16_t);
	}
	memmove(content + offset + key->length() + sizeof(uint16_t), content + offset, *end - offset);
	memcpy(content + offset, key->data(),key->length());
	memcpy(content + offset + key->length(), &rid.pageNum, sizeof(uint16_t));
	//std::cout<<"insert "
	*end += key->length() + sizeof(uint16_t);
}

void page_node::insert_to_leaf(bt_key* key, RID rid, bt_key* itr) {
	int offset = 0;
	while(true) {
		if(offset >= *end) break;
		itr->load(content + offset);
		if (!(*itr < *key)) break;
		offset += itr->length() + sizeof(rid);
	}
	memmove(content + offset + key->length() + sizeof(rid), content + offset, *end - offset);
	memcpy(content + offset, key->data(),key->length());
	memcpy(content + offset + key->length(), &rid, sizeof(rid));
	*end += key->length() + sizeof(rid);

}

page_node::page_node() : pageID(-1), is_leaf((uint16_t*)page),
		left(is_leaf + 1), right(left + 1),
		end(right + 1), content((char*)(end + 1)){}

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

void page_node::insert(bt_key* key, RID rid, bt_key* itr) {
	if (is_leaf_node()) insert_to_leaf(key, rid, itr);
	else insert_to_index(key, rid, itr);
}

int page_node::findEntry(bt_key* key, bt_key *itr){
	int offset = sizeof(uint16_t);
	while (offset < *end){
		itr->load((this->content) + offset);
		if (*key < *itr){
			uint16_t entry = *(uint16_t*)(this->content + offset - sizeof(uint16_t));
			return entry;
		}else{
			offset += sizeof(uint16_t) + itr->length();
		}
	}
	{

		std::cout<<"entry is "<<*(uint16_t*)(this->content)<<std::endl;
		return *(uint16_t*)(this->content + *end - sizeof(uint16_t));
	}
}

int page_node::findHalf(bt_key* key, RID rid,int &flag, bt_key *itr){
	if (is_leaf_node()) return find_leaf_Half(key,rid,flag, itr);
	else return find_index_Half(key, flag, itr);
}

int page_node::find_leaf_Half(bt_key* key, RID rid,int &flag,bt_key* itr) {
	int offset = 0;
	while(offset + (key->length() + sizeof(rid))* (1 - flag) < *end/2){
		itr -> load(content + offset);
		if (!(*itr < *key)){
			flag = 0;
			if(offset + key->length() +sizeof(rid)>= *end/2){
				return offset;
			}
		}
		offset += itr->length() + sizeof(rid);
	}
	return offset;
}

int page_node::find_index_Half(bt_key* key,int &flag, bt_key* itr) {
	int offset = sizeof(uint16_t);
	while(offset + (key->length() + sizeof(uint16_t)) * (1- flag) < *end/2){
		itr -> load(content + offset);
		if (!(*itr < *key)){
			flag = 0;
			if(offset + key->length() >= *end/2){
				return offset;
			}
		}
		offset += itr->length() + sizeof(uint16_t);
	}
	return offset;
}

int page_node::delete_leaf(bt_key *key, RID rid, bt_key *itr){
	int offset = 0;
	while (offset < *end){
		itr->load(content + offset);
		if(*itr == *key){
			int RID_pg = *(int32_t*)(content + offset + itr->length());
			int RID_sl = *(int32_t*)(content + offset + itr->length() + sizeof(int32_t));
			if (rid.pageNum == RID_pg && rid.slotNum == RID_sl)
				break;
			else {
				offset += itr->length() + sizeof(RID);
			}
		}else{
			offset += itr->length() + sizeof(RID);
		}
	}
	if (offset >= *end) return -1;
	else{
		memmove(content + offset, content + offset + itr->length() + sizeof(RID), *end - offset - itr->length() - sizeof(RID));
		*end -= key->length() + sizeof(RID);
		return 0;
	}
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

int dir_page::find_empty() const {
	for (int slot = 0; slot < PAGE_SIZE / sizeof(p_offset); slot++) {
		if (operator[](slot) == 0) {
			return slot;
		}
	}
	throw new std::logic_error("Dir not large enough");
	// Never reach here
	return -1;
}

}



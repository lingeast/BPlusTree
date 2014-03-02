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

void page_node::insert_to_index(bt_key* key, bpRID* rid, bt_key* itr) {
	int offset = sizeof(uint16_t);
	while (offset< *end){
		itr->load(content + offset);
		if (!(itr < key)){
			memmove(content + offset + key->length() + sizeof(uint16_t), content + offset, *end - offset);
			memcpy(content + offset, key->data(),key->length());
			page_node child(*(uint16_t*)(content + offset - sizeof(uint16_t)));
			memcpy(content + offset + key->length(), &child.right_id(), sizeof(uint16_t));
			*end += key->length() + sizeof(uint16_t);
		}
	}
}

void page_node::insert_to_leaf(bt_key* key, bpRID* rid, bt_key* itr) {
	int offset = 0;
	while(offset < *end){
		itr->load(content + offset);
		if (!(itr < key)){
			memmove(content + offset + key->length() + sizeof(rid), content + offset, *end - offset);
			memcpy(content + offset, key->data(),key->length());
			memcpy(content + offset + key->length(), rid, sizeof(rid));
			*end += key->length() + sizeof(rid);
		}
	}
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

void page_node::insert(bt_key* key, bpRID* rid, bt_key* itr) {
	if (is_leaf_node()) insert_to_leaf(key, rid, itr);
	else insert_to_index(key, rid, itr);
}

int page_node::findEntry(bt_key* key, bt_key *itr){
	int offset = sizeof(uint16_t);
	while (offset < *end){
		itr->load((this->content) + offset);
		if (key < itr){
			uint16_t entry = *(uint16_t*)(this->content + offset - sizeof(uint16_t));
			return entry;
		}else{
			offset += sizeof(uint16_t) + itr->length();
		}
	}
	return *(uint16_t*)(this->content + *end - sizeof(uint16_t));
}

int page_node::findHalf(bt_key* key,bpRID * rid,int &flag, bt_key *itr){
	if (is_leaf_node()) return find_leaf_Half(key,rid,flag, itr);
	else return find_index_Half(key, flag, itr);
}

int page_node::find_leaf_Half(bt_key* key,bpRID *rid,int &flag,bt_key* itr) {
	int offset = 0;
	while(offset + (key->length() + sizeof(rid))* (1 - flag) < *end/2){
		itr -> load(content + offset);
		if (!(itr < key)){
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
		if (!(itr < key)){
			flag = 0;
			if(offset + key->length() >= *end/2){
				return offset;
			}
		}
		offset += itr->length() + sizeof(uint16_t);
	}
	return offset;
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



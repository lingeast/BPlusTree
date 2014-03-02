/*
 * page.h
 *
 *  Created on: Feb 25, 2014
 *      Author: yidong
 */

#ifndef PAGE_H_
#define PAGE_H_


#include <cstdlib>
#include "page_size.h"
#include "../key/btkey.h"

namespace BPlusTree{

enum NodeType {
	Index = 0,
	Leaf = 1
};


class page_node {

private:
	char page[PAGE_SIZE];
	int pageID;
	uint16_t * is_leaf;	// is_left == 1 => is leaf page, == 0 => is index page
	uint16_t * left;		// page id of left sibling
	uint16_t * right;	// page id of right sibling
	//uint16_t* parent;	// page id of parent
	uint16_t * end;		// the end offset of content to the start of content
	char * content;
	void insert_to_index(bt_key* key, serializable* rid, bt_key* itr);
	void insert_to_leaf(bt_key* key, serializable* rid, bt_key* itr);
public:
	page_node(int id);	// to be used to load existing page node from file
	page_node(NodeType, int id, int left, int right);	// to create new page node
	void* content_block() const {return (void*) content;}
	void* page_block() const {return (void*) page;}
	bool is_leaf_node() const {return *is_leaf == 0;}
	// Pass reference to outside
	uint16_t& left_id() const {return *left;}
	uint16_t& right_id() const {return *right;}
	uint16_t& end_offset() const { return *end;}
	//uint16_t& parent_id() {return *parent;}

	int page_id() const {return pageID;}

	/*
	 * Insert <key, rid> pair to page
	 * Only called when key is definitely to be stored in the page (right page, enough room)
	 */
	void insert(bt_key* key, serializable* rid, bt_key* itr);
	int findEntry(bt_key* key);
	int findHalf(bt_key* key,int &flag);
};

/*
 * Directory Page Organization:
 *
 * dir_page[0] = root pageID
 * for pageID > 1, dir_page[pageID] = corresponding pageNum
 * dir_page[pageID] = 0 -> no pageID no corresponding position for this pageID,
 * because dir_page occupies the postion 0 in the btree file
 */


class dir_page {
private:
	char page[PAGE_SIZE];
public:
	dir_page(); // memset page to all 0
	void* page_block() const {return (void*) page;} // expose the page block to outside
	p_offset& operator[](int slot) const;
	int addr(int slot) {return operator[](slot);}
	int root() const { return operator[](0);}
	void update_root(int newroot) {operator[](0) = newroot;}
};

}
#endif /* PAGE_H_ */

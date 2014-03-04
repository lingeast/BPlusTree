/*
 * bptscanitr.cpp
 *
 *  Created on: Mar 4, 2014
 *      Author: root
 */

#include "bptscanitr.h"

namespace BPlusTree {

bpt_scan_itr::bpt_scan_itr(std::string fname, bt_key* lok, bt_key* hik, bool lo_in, bool hi_in):
		file(fopen(fname.c_str(),"r")), fhelp(file), lo_inc(lo_in), hi_inc(hi_in), offset(0) {
	// TODO Auto-generated constructor stub
	fhelp.write_page(0, this->dir.page_block()); // read in dir page
	cur_leaf.set_id(dir.root());
	fhelp.write_page(dir[cur_leaf.page_id()], cur_leaf.page_block()); // read in root page
	while(!cur_leaf.is_leaf_node()) {
		int entry = cur_leaf.findEntry(lok, hik);
		break;
		// TODO keep moving until reach leaf node
	}
}

bpt_scan_itr::~bpt_scan_itr() {
	// TODO Auto-generated destructor stub
	close();
}

} /* namespace BPlusTree */

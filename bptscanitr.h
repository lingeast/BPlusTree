/*
 * bptscanitr.h
 *
 *  Created on: Mar 4, 2014
 *      Author: root
 */

#ifndef BPTSCANITR_H_
#define BPTSCANITR_H_

#include "file/file_helper.h"
#include "key/btkey.h"
#include "rid/rid.h"
#include "page/page.h"

namespace BPlusTree {

class bpt_scan_itr {
private:
	FILE* file;
	file_helper fhelp;
	bool lo_inc;
	bool hi_inc;
	int offset;
	dir_page dir;
	page_node cur_leaf;
public:
	bpt_scan_itr(std::string fname, bt_key* lok, bt_key* hik, bool lo_in, bool hi_in);
	virtual ~bpt_scan_itr();

	// read key/rid pair out
	int get_next(RID &rid, void* data);

	// release occupied resources
	void close() {
		if (file != NULL) fclose(file);
	}
};

} /* namespace BPlusTree */

#endif /* BPTSCANITR_H_ */

/*
 * Copyright (c) 2015 Cray Inc. All rights reserved.
 * Copyright (c) 2018 Intel Corp, Inc.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Copied from http://oopweb.com/Algorithms/Documents/Sman/VolumeFrames.html?/Algorithms/Documents/Sman/Volume/RedBlackTrees_files/s_rbt.htm
 *
 * Disclosure from the author's main page:
 * (http://oopweb.com/Algorithms/Documents/Sman/VolumeFrames.html?/Algorithms/Documents/Sman/Volume/RedBlackTrees_files/s_rbt.htm)
 *
 *     Source code when part of a software project may be used freely
 *     without reference to the author.
 *
 */

#ifndef _OFI_TREE_H_
#define _OFI_TREE_H_

#include <stdlib.h>


enum ofi_node_color {
	BLACK,
	RED
};

struct ofi_rbnode {
	struct ofi_rbnode	*left;
	struct ofi_rbnode	*right;
	struct ofi_rbnode	*parent;
	enum ofi_node_color	color;
	void			*data;
};

struct ofi_rbmap {
	struct ofi_rbnode	*root;
	struct ofi_rbnode	sentinel;
	struct ofi_rbnode	*free_list;

	/* compare()
	 *	= 0: a == b
	 *	< 0: a < b
	 *	> 0: a > b
	 */
	int			(*compare)(struct ofi_rbmap *map,
					   void *key, void *data);
};

struct ofi_rbmap *
ofi_rbmap_create(int (*compare)(struct ofi_rbmap *map, void *key, void *data));
void ofi_rbmap_destroy(struct ofi_rbmap *map);
void ofi_rbmap_init(struct ofi_rbmap *map,
		int (*compare)(struct ofi_rbmap *map, void *key, void *data));
void ofi_rbmap_cleanup(struct ofi_rbmap *map);

struct ofi_rbnode *ofi_rbmap_find(struct ofi_rbmap *map, void *key);
struct ofi_rbnode *ofi_rbmap_search(struct ofi_rbmap *map, void *key,
		int (*compare)(struct ofi_rbmap *map, void *key, void *data));
int ofi_rbmap_insert(struct ofi_rbmap *map, void *key, void *data,
		struct ofi_rbnode **node);
void ofi_rbmap_delete(struct ofi_rbmap *map, struct ofi_rbnode *node);
int ofi_rbmap_find_delete(struct ofi_rbmap *map, void *key);
int ofi_rbmap_empty(struct ofi_rbmap *map);


#endif /* OFI_TREE_H_ */

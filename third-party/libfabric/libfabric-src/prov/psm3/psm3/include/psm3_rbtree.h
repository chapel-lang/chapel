/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2016 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef __RBTREE_H__

#define __RBTREE_H__

#include <stdint.h>
#ifdef RBTREE_ITERATOR
#include "utils_queue.h"
#endif

#ifndef RBTREE_MAP_PL
#error "You must define RBTREE_MAP_PL before including rbtree.h"
#endif

#ifndef RBTREE_MI_PL
#error "You must define RBTREE_MI_PL before including rbtree.h"
#endif

// RBTREE_ITERATOR allows first, successor and predecessor to
// perform in O(1) time at the cost of 16 bytes per cl_map_item
// When not enabled, they will perform in O(log N) time
//
// RBTREE_AUGMENT provides an augmented rbtree which can ips_cl_qmap_search()
// for an interval even if the tree contains overlapping items.
// If the tree will not store any overlapping items, this capability is
// unnecessary.  For simplicty in updating max_endp1, nil_item->max_endp1==0

/*
 * Red-Black tid cache definition.
 */
typedef struct _cl_map_item {
	struct _cl_map_item	*p_left;	/* left pointer */
	struct _cl_map_item	*p_right;	/* right pointer */
	struct _cl_map_item	*p_up;		/* up pointer */
#ifdef RBTREE_AUGMENT
	unsigned long max_endp1;		/* largest endp1 (RIGHTMOST) in subtree */
#endif
	uint16_t		color;		/* red-black color */
#ifdef RBTREE_ITERATOR
	TAILQ_ENTRY(_cl_map_item) list_entry;	/* for list of items in qmap */
#endif

	RBTREE_MI_PL            payload;
} cl_map_item_t;

typedef struct _cl_qmap {
	cl_map_item_t		*root;		/* root node pointer */
	cl_map_item_t		*nil_item;	/* terminator node pointer */
#ifdef RBTREE_ITERATOR
	TAILQ_HEAD(list, _cl_map_item) list;	/* sorted list of items in qmap */
#endif
#ifdef RBTREE_VALIDATE
	uint32_t			depth;		// current depth
	uint32_t			max_depth;	// max observed depth
#endif

	RBTREE_MAP_PL            payload;
} cl_qmap_t;

#define CL_MAP_RED   0
#define CL_MAP_BLACK 1

#endif

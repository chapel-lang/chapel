/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

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

  Copyright(c) 2015 Intel Corporation.

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

/* Copyright (c) 2003-2015 Intel Corporation. All rights reserved. */

/*
 * Abstract:
 *	Implementation of quick map, a binary tree where the caller always provides
 *	all necessary storage.
 *
 * Environment:
 *	All
 *
 * $Revision$
 */


/*****************************************************************************
*
* Map
*
* Map is an associative array.  By providing a key, the caller can retrieve
* an object from the map.  All objects in the map have an associated key,
* as specified by the caller when the object was inserted into the map.
* In addition to random access, the caller can traverse the map much like
* a linked list, either forwards from the first object or backwards from
* the last object.  The objects in the map are always traversed in
* order since the nodes are stored sorted.
*
* This implementation of Map uses a red black tree verified against
* Cormen-Leiserson-Rivest text, McGraw-Hill Edition, fourteenth
* printing, 1994.
*
*****************************************************************************/

// TBD algorithm matches IbAccess, but the tree is not well balanced, review
// public algorithm descriptions

#include <string.h> /* for memset declaration */
#include <limits.h> /* for UINT_MAX */
#ifdef RBTREE_ITERATOR
#include "utils_queue.h"
#endif

// RBTREE_CMP should be a comparator, i.e. RBTREE_CMP(a, b) should evaluate to
// -1, 0, or 1 depending on if a < b, a == b, or a > b, respectively.
#if (! defined(RBTREE_CMP) && \
		(!defined ( RBTREE_GET_LEFTMOST )|| \
				! defined ( RBTREE_GET_RIGHTMOST ))) || \
	! defined ( RBTREE_MAP_COUNT )     || \
	! defined ( RBTREE_ASSERT )
#error "You must define (RBTREE_CMP or (RBTREE_GET_LEFTMOST and RBTREE_GET_RIGHTMOST)) and \
		RBTREE_MAP_COUNT and RBTREE_ASSERT before including rbtree.c"
#endif

// All callers specify RIGHTMOST as start+len, so it is actually "end+1".
// This is assumed in all range overlap comparisons for ips_cl_map_search().
// ips_cl_map_search() endp1 argument is also assumed to be end+1
// (aka RIGHTMOST). So for readability, RBTREE_PRINTs in this file adjust
// rightmost and "endp1" by -1 to show the true "last" inclusive value in the
// range.
#if defined(RBTREE_AUGMENT) && \
			(! defined(RBTREE_GET_RIGHTMOST) || ! defined(RBTREE_GET_LEFTMOST))
#error "You must define (RBTREE_GET_LEFTMOST and RBTREE_GET_RIGHTMOST) before \
		including rbtree.c when using RBTREE_AUGMENT"
#endif
#if defined(RBTREE_EMIT_IPS_CL_QMAP_DUMP) &&  \
			(! defined(RBTREE_GET_RIGHTMOST) || ! defined(RBTREE_GET_LEFTMOST) \
			 || ! defined(RBTREE_DUMP_PRINT) || ! defined(RBTREE_FMT))
#error "You must define (RBTREE_GET_LEFTMOST and RBTREE_GET_RIGHTMOST and \
		RBTREE_DUMP_PRINT and RBTREE_FMT) before including rbtree.c  \
		when using RBTREE_EMIT_IPS_CL_QMAP_DUMP"
#endif
#if defined(RBTREE_VALIDATE) &&  \
			(! defined(RBTREE_GET_RIGHTMOST) || ! defined(RBTREE_GET_LEFTMOST) \
			 || ! defined(RBTREE_VALIDATE_PRINT) || ! defined(RBTREE_FMT)  \
			 || ! defined(RBTREE_VALIDATE_ENABLE))
#error "You must define (RBTREE_GET_LEFTMOST and RBTREE_GET_RIGHTMOST and \
		RBTREE_VALIDATE_PRINT and RBTREE_VALIDATE_ENABLE) before including \
		rbtree.c when using RBTREE_VALIDATE"
#endif

// for debug, includer can define RBTREE_PRINT and RBTREE_FMT
// RBTREE_FMT used to format LEFTMOST and RIGHTMOST return values
// for example:
//    #define RBTREE_PRINT _HFI_INFO
//    #define RBTREE_FMT "0x%"PRIx64
#ifndef RBTREE_PRINT
#define RBTREE_PRINT(fmt, ...)
#else
#ifndef RBTREE_FMT
#error "must define RBTREE_FMT when define RBTREE_PRINT"
#endif
#endif

// for when DUMP included for VALIDATE
#ifndef RBTREE_DUMP_PRINT
#define RBTREE_DUMP_PRINT(fmt, ...)
#endif

#define IN /* nothing */

// these macros reduce some clutter when comparing or walking tree
#ifdef RBTREE_GET_RIGHTMOST
#define RBTREE_ENDP1(p_item) RBTREE_GET_RIGHTMOST((&(p_item)->payload))
#endif
#ifdef RBTREE_GET_LEFTMOST
#define RBTREE_START(p_item) RBTREE_GET_LEFTMOST((&(p_item)->payload))
#endif

// these macros make it easier to print an item for various debug and validation
// sample use:
//      RBTREE_PRINT("item: "RBTREE_ITEM_FMT"\n", RBTREE_ITEM(p_item));
// depending on options defined, output can consist of
//     leftmost:righmost-1:max_endp1-1 or leftmost:rightmost-1 or leftmost
//     or simply p_pitem ptr (when RBTREE_FMT not defined)
//     rightmost and max_endp1 adjusted by -1 to show true end of range
#if defined(RBTREE_FMT)
#ifdef RBTREE_AUGMENT
#define RBTREE_ITEM_FMT RBTREE_FMT":"RBTREE_FMT":"RBTREE_FMT
#define RBTREE_ITEM(p_item)  RBTREE_START(p_item), RBTREE_ENDP1(p_item)-1, \
								((p_item)->max_endp1-1)
#else /* RBTREE_AUGMENT */
#ifdef RBTREE_ENDP1
#define RBTREE_ITEM_FMT RBTREE_FMT":"RBTREE_FMT
#define RBTREE_ITEM(p_item)  RBTREE_START(p_item), (RBTREE_ENDP1(p_item)-1)
#else
#define RBTREE_ITEM_FMT RBTREE_FMT
#define RBTREE_ITEM(p_item)  RBTREE_START(p_item)
#endif /* RBTREE_ENDP1 */
#endif /* RBTREE_AUGMENT */
#else /* RBTREE_FMT */
#define RBTREE_ITEM_FMT "%p"
#define RBTREE_ITEM(p_item) p_item
#endif /* RBTREE_FMT */

#ifdef RBTREE_VALIDATE
static int ips_cl_qmap_validate(cl_qmap_t *p_map, const char *msg,
								int check_rules, int do_assert);
#else
static inline int ips_cl_qmap_validate(cl_qmap_t *p_map, const char *msg,
								int check_rules, int do_assert)
{
	return 0;
}
#endif

/******************************************************************************
*******************************************************************************
**************                                                     ************
**************			 IMPLEMENTATION OF QUICK MAP       ************
**************                                                     ************
*******************************************************************************
******************************************************************************/

/* Forward declarations: */
static void ips_cl_qmap_init(
				IN	cl_qmap_t		*p_map,
				IN	cl_map_item_t* const	root,
				IN	cl_map_item_t* const	nil);
static void ips_cl_qmap_insert_item(
				IN	cl_qmap_t* const	p_map,
				IN	cl_map_item_t* const	p_item);
static void ips_cl_qmap_remove_item(
				IN	cl_qmap_t* const	p_map,
				IN	cl_map_item_t* const	p_item);
static cl_map_item_t* ips_cl_qmap_successor(
				IN	cl_qmap_t* const	p_map,
				IN	const cl_map_item_t*	p_item);


#ifndef RBTREE_NO_EMIT_IPS_CL_QMAP_PREDECESSOR
static cl_map_item_t* ips_cl_qmap_predecessor(
				IN	cl_qmap_t* const	p_map,
				IN	const cl_map_item_t*	p_item);
#endif

#if defined(RBTREE_START) && ! defined(RBTREE_NO_EMIT_IPS_CL_QMAP_SEARCH)
static cl_map_item_t* ips_cl_qmap_search(
				IN	cl_qmap_t* const	p_map,
				IN	unsigned long		start,
				IN	unsigned long		endp1);
#endif

#ifdef RBTREE_CMP
static cl_map_item_t* ips_cl_qmap_searchv(
				cl_qmap_t* const	p_map,
				const RBTREE_MI_PL *key);
#endif

/*
 * Get the root.
 */
static inline cl_map_item_t*
__cl_map_root(
	IN	const cl_qmap_t* const	p_map )
{
	RBTREE_ASSERT( p_map );
	return( p_map->root->p_left );
}


/*
 * Returns whether a given item is on the left of its parent.
 */
static int
__cl_map_is_left_child(
	IN	const cl_map_item_t* const	p_item )
{
	RBTREE_ASSERT( p_item );
	RBTREE_ASSERT( p_item->p_up );
	RBTREE_ASSERT( p_item->p_up != p_item );

	return( p_item->p_up->p_left == p_item );
}


/*
 * Retrieve the pointer to the parent's pointer to an item.
 */
static cl_map_item_t**
__cl_map_get_parent_ptr_to_item(
	IN	cl_map_item_t* const	p_item )
{
	RBTREE_ASSERT( p_item );
	RBTREE_ASSERT( p_item->p_up );
	RBTREE_ASSERT( p_item->p_up != p_item );

	if( __cl_map_is_left_child( p_item ) )
		return( &p_item->p_up->p_left );

	RBTREE_ASSERT( p_item->p_up->p_right == p_item );
	return( &p_item->p_up->p_right );
}


/*
 * Rotate a node to the left.  This rotation affects the least number of links
 * between nodes and brings the level of C up by one while increasing the depth
 * of A one.  Note that the links to/from W, X, Y, and Z are not affected.
 *
 *        R                      R
 *        |                      |
 *        A                      C
 *      /   \                  /   \
 *    W       C              A       Z
 *           / \            / \
 *          B   Z          W   B
 *         / \                / \
 *        X   Y              X   Y
 *
 * For RBTREE_AUGMENT:
 *    A.max_endp1 = max(W.max_endp1, A.RIGHTMOST, B.max_endp1)
 *    C.max_endp1 = max(A.max_endp1, C.max_endp1)
 */
static void
__cl_map_rot_left(
	IN	cl_qmap_t* const		p_map,
	IN	cl_map_item_t* const	p_item )
{
	cl_map_item_t	**pp_root;

	RBTREE_ASSERT( p_map );
	RBTREE_ASSERT( p_item );
	RBTREE_ASSERT( p_item->p_right != p_map->nil_item );

	pp_root = __cl_map_get_parent_ptr_to_item( p_item );

	/* Point R to C instead of A. */
	*pp_root = p_item->p_right;
	/* Set C's parent to R. */
	(*pp_root)->p_up = p_item->p_up;

	/* Set A's right to B */
	p_item->p_right = (*pp_root)->p_left;
	/*
	 * Set B's parent to A.  We trap for B being NIL since the
	 * caller may depend on NIL not changing.
	 */
	if( (*pp_root)->p_left != p_map->nil_item )
		(*pp_root)->p_left->p_up = p_item;

	/* Set C's left to A. */
	(*pp_root)->p_left = p_item;
	/* Set A's parent to C. */
	p_item->p_up = *pp_root;
#ifdef RBTREE_AUGMENT
	/* A.max_endp1 = max(W.max_endp1, A.RIGHTMOST, B.max_endp1) */
	p_item->max_endp1 = max(p_item->p_left->max_endp1,
						 max(RBTREE_ENDP1(p_item), p_item->p_right->max_endp1));
	/* C.max_endp1 = max(A.max_endp1, C.max_endp1) */
	if (p_item->max_endp1 > (*pp_root)->max_endp1)
		(*pp_root)->max_endp1 = p_item->max_endp1;
#endif
	ips_cl_qmap_validate(p_map, "rot_left", 0, 1);
}


/*
 * Rotate a node to the right.  This rotation affects the least number of links
 * between nodes and brings the level of A up by one while increasing the depth
 * of C one.  Note that the links to/from W, X, Y, and Z are not affected.
 *
 *            R                     R
 *            |                     |
 *            C                     A
 *          /   \                 /   \
 *        A       Z             W       C
 *       / \                           / \
 *      W   B                         B   Z
 *         / \                       / \
 *        X   Y                     X   Y
 *
 * For RBTREE_AUGMENT:
 *    C.max_endp1 = max(B.max_endp1, C.RIGHTMOST, Z.max_endp1)
 *    A.max_endp1 = max(A.max_endp1, C.max_endp1)
 */
static void
__cl_map_rot_right(
	IN	cl_qmap_t* const		p_map,
	IN	cl_map_item_t* const	p_item )
{
	cl_map_item_t	**pp_root;

	RBTREE_ASSERT( p_map );
	RBTREE_ASSERT( p_item );
	RBTREE_ASSERT( p_item->p_left != p_map->nil_item );

	/* Point R to A instead of C. */
	pp_root = __cl_map_get_parent_ptr_to_item( p_item );
	(*pp_root) = p_item->p_left;
	/* Set A's parent to R. */
	(*pp_root)->p_up = p_item->p_up;

	/* Set C's left to B */
	p_item->p_left = (*pp_root)->p_right;
	/*
	 * Set B's parent to C.  We trap for B being NIL since the
	 * caller may depend on NIL not changing.
	 */
	if( (*pp_root)->p_right != p_map->nil_item )
		(*pp_root)->p_right->p_up = p_item;

	/* Set A's right to C. */
	(*pp_root)->p_right = p_item;
	/* Set C's parent to A. */
	p_item->p_up = *pp_root;
#ifdef RBTREE_AUGMENT
	/* C.max_endp1 = max(B.max_endp1, C.RIGHTMOST, Z.max_endp1) */
	p_item->max_endp1 = max(p_item->p_left->max_endp1,
						 max(RBTREE_ENDP1(p_item), p_item->p_right->max_endp1));
	/* A.max_endp1 = max(A.max_endp1, C.max_endp1) */
	if (p_item->max_endp1 > (*pp_root)->max_endp1)
		(*pp_root)->max_endp1 = p_item->max_endp1;
#endif
	ips_cl_qmap_validate(p_map, "rot_right", 0, 1);
}

/*
 * Balance a tree starting at a given item back to the root.
 */
static void
__cl_map_ins_bal(
	IN	cl_qmap_t* const	p_map,
	IN	cl_map_item_t*		p_item )
{
	cl_map_item_t*		p_grand_uncle;

	RBTREE_ASSERT( p_map );
	RBTREE_ASSERT( p_item );
	RBTREE_ASSERT( p_item != p_map->root );

	while( p_item->p_up->color == CL_MAP_RED )
	{
		if( __cl_map_is_left_child( p_item->p_up ) )
		{
			p_grand_uncle = p_item->p_up->p_up->p_right;
			RBTREE_ASSERT( p_grand_uncle );
			if( p_grand_uncle->color == CL_MAP_RED )
			{
				p_grand_uncle->color = CL_MAP_BLACK;
				p_item->p_up->color = CL_MAP_BLACK;
				p_item->p_up->p_up->color = CL_MAP_RED;
				p_item = p_item->p_up->p_up;
				continue;
			}

			if( !__cl_map_is_left_child( p_item ) )
			{
				p_item = p_item->p_up;
				__cl_map_rot_left( p_map, p_item );
			}
			p_item->p_up->color = CL_MAP_BLACK;
			p_item->p_up->p_up->color = CL_MAP_RED;
			__cl_map_rot_right( p_map, p_item->p_up->p_up );
		}
		else
		{
			p_grand_uncle = p_item->p_up->p_up->p_left;
			RBTREE_ASSERT( p_grand_uncle );
			if( p_grand_uncle->color == CL_MAP_RED )
			{
				p_grand_uncle->color = CL_MAP_BLACK;
				p_item->p_up->color = CL_MAP_BLACK;
				p_item->p_up->p_up->color = CL_MAP_RED;
				p_item = p_item->p_up->p_up;
				continue;
			}

			if( __cl_map_is_left_child( p_item ) )
			{
				p_item = p_item->p_up;
				__cl_map_rot_right( p_map, p_item );
			}
			p_item->p_up->color = CL_MAP_BLACK;
			p_item->p_up->p_up->color = CL_MAP_RED;
			__cl_map_rot_left( p_map, p_item->p_up->p_up );
		}
	}
}

static void ips_cl_qmap_init(
				IN	cl_qmap_t		*p_map,
				IN	cl_map_item_t* const	root,
				IN	cl_map_item_t* const	nil_item)
{
	RBTREE_ASSERT( p_map );
	RBTREE_ASSERT( root );
	RBTREE_ASSERT( nil_item );

	memset(p_map,0,sizeof(cl_qmap_t));

	p_map->root = root;

	/* setup the RB tree map */
	p_map->nil_item = nil_item;

	p_map->root->p_up = p_map->root;
	p_map->root->p_left = p_map->nil_item;
	p_map->root->p_right = p_map->nil_item;
	p_map->root->color = CL_MAP_BLACK;
#ifdef RBTREE_AUGMENT
	p_map->root->max_endp1 = 0;	/* never used, be safe */
#endif

	p_map->nil_item->p_up = p_map->nil_item;
	p_map->nil_item->p_left = p_map->nil_item;
	p_map->nil_item->p_right = p_map->nil_item;
	p_map->nil_item->color = CL_MAP_BLACK;
#ifdef RBTREE_AUGMENT
	p_map->nil_item->max_endp1 = 0;	/* simplifies compute of new max_endp1 */
#endif
#ifdef RBTREE_ITERATOR
	TAILQ_INIT(&p_map->list);
#endif
}

static void
ips_cl_qmap_insert_item(
	IN	cl_qmap_t* const		p_map,
	IN	cl_map_item_t* const	p_item )
{
	cl_map_item_t	*p_insert_at, *p_comp_item;
	int compare_res = 0;

	RBTREE_ASSERT( p_map );
	RBTREE_ASSERT( p_item );
	RBTREE_ASSERT( p_map->root->p_up == p_map->root );
	RBTREE_ASSERT( p_map->root->color != CL_MAP_RED );
	RBTREE_ASSERT( p_map->nil_item->color != CL_MAP_RED );

	/* Find the insertion location. */
	p_insert_at = p_map->root;
	p_comp_item = __cl_map_root( p_map );

	while( p_comp_item != p_map->nil_item )
	{
		p_insert_at = p_comp_item;

		/* Traverse the tree until the correct insertion point is found. */
#ifdef RBTREE_CMP
		if(RBTREE_CMP(&p_item->payload, &p_insert_at->payload) < 0)
#else
		if( RBTREE_START(p_item) < RBTREE_START(p_insert_at) )
#endif
		{
			p_comp_item = p_insert_at->p_left;
			compare_res = 1;
		} else {
			p_comp_item = p_insert_at->p_right;
			compare_res = -1;
		}
	}

	RBTREE_ASSERT( p_insert_at != p_map->nil_item );
	RBTREE_ASSERT( p_comp_item == p_map->nil_item );

	/* Insert the item. */
	p_item->p_left = p_map->nil_item;
	p_item->p_right = p_map->nil_item;
	p_item->color = CL_MAP_RED;
	if( p_insert_at == p_map->root )
	{
		/* first item in tree */
		p_insert_at->p_left = p_item;
#ifdef RBTREE_ITERATOR
		// insert at very start
		TAILQ_INSERT_HEAD(&p_map->list, p_item, list_entry);
#endif
	}
	else if( compare_res > 0 ) /* key < p_insert_at->key */
	{
		p_insert_at->p_left = p_item;
#ifdef RBTREE_ITERATOR
		// insert before p_insert_at
		TAILQ_INSERT_BEFORE(p_insert_at, p_item, list_entry);
#endif
	}
	else
	{
		p_insert_at->p_right = p_item;
#ifdef RBTREE_ITERATOR
		// insert after p_insert_at
		TAILQ_INSERT_AFTER(&p_map->list, p_insert_at, p_item, list_entry);
#endif
	}
	/* Increase the count. */
	RBTREE_MAP_COUNT(&p_map->payload)++;

	p_item->p_up = p_insert_at;
#ifdef RBTREE_AUGMENT
	p_item->max_endp1 = max(p_item->p_left->max_endp1,
						 max(RBTREE_ENDP1(p_item), p_item->p_right->max_endp1));
	unsigned long max_endp1 = p_item->max_endp1;
	/* update max_endp1 for all parents of new item.
	 * Once find a parent whose max_endp1 is already large enough can stop
	 * since all above will also be large enough
	 */
	while (p_insert_at != p_map->root && max_endp1 > p_insert_at->max_endp1) {
		p_insert_at->max_endp1 = max_endp1;
		p_insert_at = p_insert_at->p_up;
	}
#endif
	ips_cl_qmap_validate(p_map, "insert", 0, 1);

	/*
	 * We have added depth to this section of the tree.
	 * Rebalance as necessary as we retrace our path through the tree
	 * and update colors.
	 */
	__cl_map_ins_bal( p_map, p_item );

	__cl_map_root( p_map )->color = CL_MAP_BLACK;

	/*
	 * Note that it is not necessary to re-color the nil node black because all
	 * red color assignments are made via the p_up pointer, and nil is never
	 * set as the value of a p_up pointer.
	 */
	ips_cl_qmap_validate(p_map, "done insert", 1, 1);
}

static void
__cl_map_del_bal(
	IN	cl_qmap_t* const	p_map,
	IN	cl_map_item_t*		p_item )
{
	cl_map_item_t		*p_uncle;

	while( (p_item->color != CL_MAP_RED) && (p_item->p_up != p_map->root) )
	{
		if( __cl_map_is_left_child( p_item ) )
		{
			p_uncle = p_item->p_up->p_right;

			if( p_uncle->color == CL_MAP_RED )
			{
				p_uncle->color = CL_MAP_BLACK;
				p_item->p_up->color = CL_MAP_RED;
				__cl_map_rot_left( p_map, p_item->p_up );
				p_uncle = p_item->p_up->p_right;
			}

			if( p_uncle->p_right->color != CL_MAP_RED )
			{
				if( p_uncle->p_left->color != CL_MAP_RED )
				{
					p_uncle->color = CL_MAP_RED;
					p_item = p_item->p_up;
					continue;
				}

				p_uncle->p_left->color = CL_MAP_BLACK;
				p_uncle->color = CL_MAP_RED;
				__cl_map_rot_right( p_map, p_uncle );
				p_uncle = p_item->p_up->p_right;
			}
			p_uncle->color = p_item->p_up->color;
			p_item->p_up->color = CL_MAP_BLACK;
			p_uncle->p_right->color = CL_MAP_BLACK;
			__cl_map_rot_left( p_map, p_item->p_up );
			break;
		}
		else
		{
			p_uncle = p_item->p_up->p_left;

			if( p_uncle->color == CL_MAP_RED )
			{
				p_uncle->color = CL_MAP_BLACK;
				p_item->p_up->color = CL_MAP_RED;
				__cl_map_rot_right( p_map, p_item->p_up );
				p_uncle = p_item->p_up->p_left;
			}

			if( p_uncle->p_left->color != CL_MAP_RED )
			{
				if( p_uncle->p_right->color != CL_MAP_RED )
				{
					p_uncle->color = CL_MAP_RED;
					p_item = p_item->p_up;
					continue;
				}

				p_uncle->p_right->color = CL_MAP_BLACK;
				p_uncle->color = CL_MAP_RED;
				__cl_map_rot_left( p_map, p_uncle );
				p_uncle = p_item->p_up->p_left;
			}
			p_uncle->color = p_item->p_up->color;
			p_item->p_up->color = CL_MAP_BLACK;
			p_uncle->p_left->color = CL_MAP_BLACK;
			__cl_map_rot_right( p_map, p_item->p_up );
			break;
		}
	}
	p_item->color = CL_MAP_BLACK;
}

/* Removing A, a few cases:
 * If A (p_item) only has left or right, pull up it's only child B (p_child)
 * p_del_item == p_item (A)
 *            R                     R
 *            |                     |
 *            C                     C
 *          /   \                 /   \
 *        A       Z              B      Z
 *         \                    / \
 *          B                  X   Y
 *         / \
 *        X   Y
 * For RBTREE_AUGMENT, recompute max_endp1 for all parents of p_child (B),
 *     so start at C.
 *     Not sure who was max, so must consider left,item,right at each parent
 *     Can stop once hit parent whose max_endp1 was unchanged by recompute
 *
 * If A (p_item) has no children, pull up nothing (p_child==nil_item)
 * p_del_item == p_item (A)
 *            R                     R
 *            |                     |
 *            C                     C
 *          /   \                     \
 *        A       Z                     Z
 * For RBTREE_AUGMENT, recompute max_endp1 for all original parents of
 *     p_del_item (A), so start at C.
 *     Not sure who was max, so must consider left,item,right at each parent
 *     Can stop once hit parent whose max_endp1 was unchanged by recompute
 *
 * If A (p_item) has both left and right, replace A with its successor
 * X (p_del_item).  As a successor of A, X will be the furthest down and left
 * in the B subtree (right of A) and hence have no left children. So p_child
 * will be X's right child (K below) which could be nil (next figure).
 * p_del_item (X) != p_item (A)
 *            R                     R
 *            |                     |
 *            C                     C
 *          /   \                 /   \
 *        A       Z              X      Z
 *       / \                    / \
 *      D   B                  D   B
 *         / \                    / \
 *        X   Y                  K   Y
 *         \
 *          K
 * For RBTREE_AUGMENT, recompute max_endp1 for all new parents of p_child (K),
 *     so start at B.
 *     Not sure who was max, so must consider left,item,right at each parent
 *     In B subtree, can stop once hit parent whose max_endp1 was unchanged by
 *     recompute, but if stop before visiting X, must resume at X and continue
 *     until hit a parent of X whose max_endp1 was unchanged by recompute.
 *
 * As above, if A (p_item) has both left and right and successor is
 * X (p_del_item), but has no children, p_child is nil.
 * p_del_item (X) != p_item (A)
 *            R                     R
 *            |                     |
 *            C                     C
 *          /   \                 /   \
 *        A       Z              X      Z
 *       / \                    / \
 *      D   B                  D   B
 *         / \                      \
 *        X   Y                      Y
 * For RBTREE_AUGMENT, recompute max_endp1 for all original parents of
 *     p_del_item (X), so start at B.
 *     Not sure who was max, so must consider left,item,right at each parent
 *     In B subtree, can stop once hit parent whose max_endp1 was unchanged by
 *     recompute, but if stop before visiting X, must resume at X and continue
 *     until hit a parent of X whose max_endp1 was unchanged by recompute.
 *
 * As above, if A (p_item) has both left and right and successor is
 * X (p_del_item), but X has no children, p_child is nil.
 * p_del_item (X) != p_item (A)
 *            R                     R
 *            |                     |
 *            C                     C
 *          /   \                 /   \
 *        A       Z              X      Z
 *       / \                    /
 *      D   X                  D
 * For RBTREE_AUGMENT, recompute max_endp1 for p_del_item (X) and all it's
 *     new parents.  In this case p_del_item->p_up == p_item.
 *     Not sure who was max, so must consider left,item,right at each parent
 *     Can stop once hit parent whose max_endp1 was unchanged by recompute.
 *
 * As above, if A (p_item) has both left and right and successor is
 * X (p_del_item), but X has a child (must be on right), p_child is K.
 * p_del_item (X) != p_item (A)
 *            R                     R
 *            |                     |
 *            C                     C
 *          /   \                 /   \
 *        A       Z              X      Z
 *       / \                    / \
 *      D   X                  D   K
 *           \
 *            K
 * For RBTREE_AUGMENT, recompute max_endp1 for p_del_item (X) and all it's
 *     new parents.  In this case p_del_item->p_up == p_item.
 *     Not sure who was max, so must consider left,item,right at each parent
 *     Can stop once hit parent whose max_endp1 was unchanged by recompute.
 */
static void
ips_cl_qmap_remove_item(
	IN	cl_qmap_t* const		p_map,
	IN	cl_map_item_t* const	p_item )
{
	cl_map_item_t	*p_child, *p_del_item;

	RBTREE_ASSERT( p_map );
	RBTREE_ASSERT( p_item );

	if( p_item == p_map->nil_item )
		return;

	if( (p_item->p_right == p_map->nil_item) || (p_item->p_left == p_map->nil_item ) )
	{
		/* The item being removed has children on at most one side. */
		p_del_item = p_item;
	}
	else
	{
		/*
		 * The item being removed has children on both side.
		 * We select the item that will replace it.
		 */
		p_del_item = ips_cl_qmap_successor(p_map, p_item);
		RBTREE_ASSERT( p_del_item != p_map->nil_item );
	}

#ifdef RBTREE_ITERATOR
	TAILQ_REMOVE(&p_map->list, p_item, list_entry);
#endif
	RBTREE_MAP_COUNT(&p_map->payload)--;

	/* Get the pointer to the new root's child, if any. */
	if( p_del_item->p_left != p_map->nil_item )
		p_child = p_del_item->p_left;
	else
		p_child = p_del_item->p_right;

	/* move p_child to replace p_del_item
	 * This assignment may modify the parent pointer of the nil node.
	 * This is inconsequential.
	 */
	p_child->p_up = p_del_item->p_up;
	(*__cl_map_get_parent_ptr_to_item( p_del_item )) = p_child;
	/* now have desired tree topology, but when p_item != p_del_item exchange
	 * is not yet done and p_item is still sitting where p_del_item goes
	 */

#ifdef RBTREE_AUGMENT
	/* will fix max_endp1 for all parents of lowest deleted or exchanged item */
	cl_map_item_t *p_tmp = p_del_item->p_up;
	if (p_tmp == p_item)
		p_tmp = p_del_item;
#ifdef RBTREE_VALIDATE
	// for diagnostic message if validate fails
	cl_map_item_t *p_update = p_tmp;
#endif
#endif

	uint16_t color = p_del_item->color;	// before completing exchange
	if( p_del_item != p_item )
	{
		/*
		 * p_item had 2 children, finalize the removal of p_item by exchanging
		 * it with the substitute (p_del_item) which we removed from tree above.
		 */
		p_del_item->p_up = p_item->p_up;
		p_del_item->p_left = p_item->p_left;
		p_del_item->p_right = p_item->p_right;
		(*__cl_map_get_parent_ptr_to_item( p_item )) = p_del_item;
		p_item->p_right->p_up = p_del_item;
		p_item->p_left->p_up = p_del_item;
		p_del_item->color = p_item->color;
#ifdef RBTREE_AUGMENT
		/* now tree is complete, we can recompute all affected max_endp1 */
		int visited = 0;
		for ( ; p_tmp != p_map->root; p_tmp = p_tmp->p_up) {
			visited |= (p_tmp == p_del_item->p_up);
			unsigned long old_max_endp1 = p_tmp->max_endp1;
			p_tmp->max_endp1 = max(p_tmp->p_left->max_endp1,
						   max(RBTREE_ENDP1(p_tmp), p_tmp->p_right->max_endp1));
			if (old_max_endp1 == p_tmp->max_endp1) {
				// make sure we visited the moved item and it's immediate parent
				if (visited)
					break; // all parents will be ok
				// continue at p_del_item and it's parents
				p_tmp = p_del_item;
				p_tmp->max_endp1 = max(p_tmp->p_left->max_endp1,
						   max(RBTREE_ENDP1(p_tmp), p_tmp->p_right->max_endp1));
			}
		}
	} else {
		/* now tree is complete, we can recompute all affected max_endp1 */
		for ( ; p_tmp != p_map->root; p_tmp = p_tmp->p_up) {
			unsigned long old_max_endp1 = p_tmp->max_endp1;
			p_tmp->max_endp1 = max(p_tmp->p_left->max_endp1,
						   max(RBTREE_ENDP1(p_tmp), p_tmp->p_right->max_endp1));
			if (old_max_endp1 == p_tmp->max_endp1)
				break; // all parents will be ok
		}
#endif
	} 
#if defined(RBTREE_AUGMENT) && defined(RBTREE_VALIDATE)
	// debug of remove can be tricky, show some more information so
	// can figure out which scenario occurred
	if (ips_cl_qmap_validate(p_map, "remove", 0, 0)) {
		RBTREE_VALIDATE_PRINT("p_item: %p "RBTREE_ITEM_FMT
							" p_del_item: %p "RBTREE_ITEM_FMT
							" p_child: %p "RBTREE_ITEM_FMT
							" p_update: %p "RBTREE_ITEM_FMT
							" p_nil=%p\n",
							p_item, RBTREE_ITEM(p_item),
							p_del_item, RBTREE_ITEM(p_del_item),
							p_child, RBTREE_ITEM(p_child),
							p_update, RBTREE_ITEM(p_update),
							p_map->nil_item);
		RBTREE_ASSERT(0);
	}
#else
	ips_cl_qmap_validate(p_map, "remove", 0, 1);
#endif /* defined(RBTREE_AUGMENT) && defined(RBTREE_VALIDATE) */

	/* rebalance as needed */
// TBD - p_child could be nil
	if( color != CL_MAP_RED )
		__cl_map_del_bal( p_map, p_child );

	RBTREE_ASSERT( p_map->nil_item->color != CL_MAP_RED );

	ips_cl_qmap_validate(p_map, "done remove", 1, 1);
}

#if defined(RBTREE_EMIT_IPS_CL_QMAP_FIRST) || defined(RBTREE_VALIDATE)
#ifdef RBTREE_ITERATOR
static cl_map_item_t *ips_cl_qmap_first(cl_qmap_t *p_map)
{
	cl_map_item_t *p_tmp = TAILQ_FIRST(&p_map->list);
	if (! p_tmp)
		return p_map->nil_item;
	else
		return p_tmp;
}
#else /* RBTREE_ITERATOR */
static cl_map_item_t *ips_cl_qmap_first(cl_qmap_t *p_map)
{
	cl_map_item_t *p_tmp = __cl_map_root(p_map);
	if (p_tmp != p_map->nil_item) {
		while (p_tmp->p_left != p_map->nil_item)
			p_tmp = p_tmp->p_left;
	}
	RBTREE_ASSERT(p_tmp == p_map->nil_item || p_tmp->p_left == p_map->nil_item);
	RBTREE_ASSERT((p_tmp == p_map->nil_item)
					== (0 == RBTREE_MAP_COUNT(&p_map->payload)));
	return p_tmp;
}
#endif /* RBTREE_ITERATOR */
#endif /* defined(RBTREE_EMIT_IPS_CL_QMAP_FIRST) || defined(RBTREE_VALIDATE) */

#ifdef RBTREE_ITERATOR
static cl_map_item_t *
ips_cl_qmap_successor(
	IN	cl_qmap_t* const		p_map,
	IN	const cl_map_item_t*		p_item )
{
	cl_map_item_t *p_tmp = TAILQ_NEXT(p_item, list_entry);
	if (! p_tmp)
		return p_map->nil_item;
	else
		return p_tmp;
}
#else
static cl_map_item_t *
ips_cl_qmap_successor(
	IN	cl_qmap_t* const		p_map,
	IN	const cl_map_item_t*		p_item )
{
	cl_map_item_t	*p_tmp;

	p_tmp = p_item->p_right;
	if (p_tmp != p_map->nil_item) {
		while (p_tmp->p_left != p_map->nil_item)
			p_tmp = p_tmp->p_left;
		return p_tmp;
	} else {
		for (p_tmp= p_item->p_up; p_tmp->p_right == p_item; p_tmp = p_tmp->p_up)
			p_item = p_tmp;
		if (p_tmp == p_map->root)
			return p_map->nil_item;
		return p_tmp;
	}
}
#endif

// When includer does not define RBTREE_GET_LEFTMOST, ips_cl_qmap_search() is
// not emitted.
// When this happens, ips_cl_qmap_predecessor() is not called by psm3_rbtree.c.
// When combined with -Werror -Wunused-function, psm fails to build.
// So provide macro to control emitting this function since some includers
// will still need it for their own use of ips_cl_qmap_predecessor()
#ifndef RBTREE_NO_EMIT_IPS_CL_QMAP_PREDECESSOR
#ifdef RBTREE_ITERATOR
static cl_map_item_t *
ips_cl_qmap_predecessor(
	IN	cl_qmap_t* const		p_map,
	IN	const cl_map_item_t*		p_item )
{
	cl_map_item_t *p_tmp = TAILQ_PREV(p_item, list, list_entry);
	if (! p_tmp)
		return p_map->nil_item;
	else
		return p_tmp;
}
#else /* RBTREE_ITERATOR */
static cl_map_item_t *
ips_cl_qmap_predecessor(
	IN	cl_qmap_t* const		p_map,
	IN	const cl_map_item_t*		p_item )
{
	cl_map_item_t	*p_tmp;

	p_tmp = p_item->p_left;
	if (p_tmp != p_map->nil_item) {
		while (p_tmp->p_right != p_map->nil_item)
			p_tmp = p_tmp->p_right;
		return p_tmp;
	} else {
		for (p_tmp = p_item->p_up; p_tmp->p_left == p_item; p_tmp = p_tmp->p_up)
			p_item = p_tmp;
		if (p_tmp == p_map->root)
			return p_map->nil_item;
		return p_tmp;
	}
}
#endif /* RBTREE_ITERATOR */
#endif /* RBTREE_NO_EMIT_IPS_CL_QMAP_PREDECESSOR */

#if defined(RBTREE_START) && ! defined(RBTREE_NO_EMIT_IPS_CL_QMAP_SEARCH)
/* endp1, max_endp1 and RIGHTMOST (ENDP1) are really last_addr + 1
 * leftmost (START) is start
 * rightmost (ENDP1) is start+length (also endp1 or max_endp1).
 * rightmost<=start means has no overlap
 * leftmost>=endp1 means has no overlap
 */
static inline int ips_cl_qmap_overlaps(cl_map_item_t *p_item,
								unsigned long start, unsigned long endp1)
{
	return (start < RBTREE_ENDP1(p_item) && endp1 > RBTREE_START(p_item));
}

#ifdef RBTREE_AUGMENT
/* return the "first" node in tree rooted at p_item with buffer overlapping
 * or nil_item.
 * qmap is augmented binary tree sorted by START (p_left, item, p_right).
 * If two entries have same START, one with lower END is earlier.
 * No two entries can have exact same START and END.
 * Items in qmap can overlap eachother, and start/end can overlap
 * more than 1 item.
 * "first" is defined as the overlapping node with lowest key (RBTREE_CMP
 * or if !defined(RBTREE_CMP) RBTREE_GET_LEFTMOST).
 */
static cl_map_item_t *
ips_cl_qmap_search_tree(cl_qmap_t * const p_map, cl_map_item_t *p_item,
		unsigned long start, unsigned long endp1)
{
	RBTREE_ASSERT( p_map );
	RBTREE_ASSERT( p_item != p_map->nil_item );
	RBTREE_PRINT("search tree "RBTREE_FMT":"RBTREE_FMT"\n", start, endp1-1);
	while(1) {
		// this is 1st step in check for a node, but it's
		// implicit in tests below which happened on the prior loop
		// so no need to be redundant.  Doing check here would just
		// optimize 1st loop for a no-match situation, but in at least
		// the User MR use case, we shouldn't get callbacks which don't
		// match any MRs, so no value to optimize 1st loop either.
		//	if start >= p_item->max_endp1
		//		no match

		// search left children
		cl_map_item_t *p_tmp = p_item->p_left;
		if (p_tmp != p_map->nil_item) {
			if (start < p_tmp->max_endp1) {
				// at least 1 item in left tree has start < ENDP1
				// because max_endp1 matches ENDP1 in at least 1 item.
				// Now we need to find an item with endp1 > START.
				// If one is not found in the left tree, START(p_item)
				// and all the START in the right tree are the same or
				// larger and will also mismatch.
				// So we can limit out search to the left tree since we
				// only want to find the "smallest" item matching start/end
				p_item = p_tmp;
				continue;
			}
			// nothing in left tree is relevant, all on left have
			// start >= ENDP1
		}

		// check this node
		if (endp1 > RBTREE_START(p_item)) {
			if (start < RBTREE_ENDP1(p_item)) {
				// p_item is smallest match
				break;
			}
			p_tmp = p_item->p_right;
			if (p_tmp != p_map->nil_item) {
				if (start < p_tmp->max_endp1) {
					// something in right tree may be relevant
					p_item = p_tmp;
					continue;
				}
				// nothing in right tree is relevant, all on right
				// have start >= ENDP1
			}
		}
		// no match
		p_item = p_map->nil_item;
		break;
	}
	return p_item;
}

/* return the "first" node in map with buffer overlapping or nil_item.
 * qmap is augmented binary tree sorted by START (p_left, item, p_right).
 * If two entries have same START, one with lower END is earlier.
 * No two entries can have exact same START and END.
 * Items in qmap can overlap eachother, and start/end can overlap
 * more than 1 item.
 * "first" is defined as the overlapping node with lowest key (RBTREE_CMP
 * or if !defined(RBTREE_CMP) RBTREE_GET_LEFTMOST).
 */
static cl_map_item_t *
ips_cl_qmap_search(cl_qmap_t * const p_map,
		unsigned long start, unsigned long endp1)
{
	RBTREE_ASSERT( p_map );
	cl_map_item_t *p_item = __cl_map_root(p_map);
	ips_cl_qmap_validate(p_map, "search", 1, 1);
	RBTREE_PRINT("search "RBTREE_FMT":"RBTREE_FMT"\n", start, endp1-1);
	if (p_item != p_map->nil_item)
		p_item = ips_cl_qmap_search_tree(p_map, p_item, start, endp1);
#ifdef RBTREE_PRINT
	if (p_item != p_map->nil_item) {
		RBTREE_PRINT("Selected "RBTREE_ITEM_FMT"\n",
						RBTREE_ITEM(p_item));
		RBTREE_ASSERT(ips_cl_qmap_overlaps(p_item, start, endp1));
	} else {
		RBTREE_PRINT("Not Found\n");
	}
#endif
	return p_item;
}

#if defined(RBTREE_EMIT_IPS_CL_QMAP_SEARCH_SUCCESSOR)
/* return the "first" successor to p_item node in map with buffer overlapping
 * or nil_item.
 * qmap is augmented binary tree sorted by START (p_left, item, p_right).
 * If two entries have same START, one with lower END is earlier.
 * No two entries can have exact same START and END.
 * Items in qmap can overlap eachother, and start/end can overlap
 * more than 1 item.
 * "first" is defined as the overlapping node with lowest key (RBTREE_CMP
 * or if !defined(RBTREE_CMP) RBTREE_GET_LEFTMOST).
 *
 * This algorithm has a worst case of 2*(log N) since p_item starting point
 * could be on p_left of root, but at the bottom p_right leg of that side of
 * tree and need to traverse all the way up to root, then search down the
 * p_right of root all the way to bottom of that leg of tree.
 * However RBTREE_ITERATOR helps mitgate that by using O(1) TAILQ_NEXT to get
 * to successors and avoid the O(log N) upward traverse in some of the cases.
 *
 * Realistically, matching entries will be proximate, so unlikely to need to
 * do a 2*(log N) walk of tree and will typically be log N or much better
 * even without RBTREE_ITERATOR optimization. Hence in typical use cases,
 * such as removing all matching items, this can provide benefits over simply
 * iterating on ips_cl_qmap_search().
 */
static cl_map_item_t *
ips_cl_qmap_search_successor(cl_qmap_t * const p_map, cl_map_item_t *p_item,
		unsigned long start, unsigned long endp1)
{
	RBTREE_ASSERT( p_map );
	ips_cl_qmap_validate(p_map, "search", 1, 1);
	RBTREE_PRINT("search succ "RBTREE_FMT":"RBTREE_FMT" item "RBTREE_ITEM_FMT"\n",
				start, endp1-1, RBTREE_ITEM(p_item));
	while (p_item != p_map->nil_item) {
		// ignore left tree, nothing there is a successor of p_item
		// search right children
		cl_map_item_t *p_tmp = p_item->p_right;
		if (p_tmp != p_map->nil_item) {
			if (start < p_tmp->max_endp1) {
				// at least 1 item in right tree has start < ENDP1
				// because max_endp1 matches ENDP1 in at least 1 item.
				// Now we need to find an item with endp1 > START.
				// If one is not found in the right tree, START in all
				// successors up the tree are the same or
				// larger and will also mismatch.
				// So we can limit out search to the right tree since we only
				// want to find the "smallest" successor matching start/end
				p_item = ips_cl_qmap_search_tree(p_map, p_tmp, start, endp1);
				break;
			}
			RBTREE_PRINT("Elim Right of "RBTREE_ITEM_FMT"\n",
							 RBTREE_ITEM(p_item));
#ifdef RBTREE_ITERATOR
			// find the successor up the tree to p_item
			// we ruled out p_item->p_right above, so just go up
			// this is a copy of the no right part of successor() above
			// we want to find a node where p_item was the p_left of it.
			// note root has no p_right, so if p_tmp is root, loop ends
			for (p_tmp = p_item->p_up; p_tmp->p_right == p_item;
							p_tmp = p_tmp->p_up)
				p_item = p_tmp;
			if (p_tmp == p_map->root)
				return p_map->nil_item;
			p_item = p_tmp;	// new candidate
		} else {
			// we have no p_right, so use list to quickly find our successor
			p_item = TAILQ_NEXT(p_item, list_entry);
			if (! p_item) {
				p_item = p_map->nil_item;
				break;
			}
		}
#else
		}
		// find the successor up the tree to p_item
		// we ruled out p_item->p_right above, so just go up
		// this is a copy of the no right part of successor() above
		// we want to find a node where p_item was the p_left of it.
		// note root has no p_right, so if p_tmp is root, loop ends
		for (p_tmp= p_item->p_up; p_tmp->p_right == p_item; p_tmp = p_tmp->p_up)
			p_item = p_tmp;
		if (p_tmp == p_map->root)
			return p_map->nil_item;	// no successor up tree to p_item
		p_item = p_tmp;	// new candidate
#endif
		RBTREE_PRINT("Checking "RBTREE_ITEM_FMT"\n", RBTREE_ITEM(p_item));
		if (endp1 > RBTREE_START(p_item)) {
				if (start < RBTREE_ENDP1(p_item)) {
					// p_item is smallest match
					break;
				}
				// go check successors of p_item
				continue;
		}
		// no match
		p_item = p_map->nil_item;
		break;
	}
#ifdef RBTREE_PRINT
	if (p_item != p_map->nil_item) {
		RBTREE_PRINT("Selected "RBTREE_ITEM_FMT"\n",
						RBTREE_ITEM(p_item));
		RBTREE_ASSERT(ips_cl_qmap_overlaps(p_item, start, endp1));
	} else {
		RBTREE_PRINT("Not Found\n");
	}
#endif
	return p_item;
}
#endif /* defined(RBTREE_EMIT_IPS_CL_QMAP_SEARCH_SUCCESSOR) */

#else /* RBTREE_AUGMENT */

/* p_item overlaps start/end, find 1st item in its subtree which overlaps.
 * This could be p_item or something to its left
 */
static cl_map_item_t *
ips_cl_qmap_select_earliest(cl_qmap_t * const p_map,
		cl_map_item_t *p_item, unsigned long start, unsigned long endp1)
{
	cl_map_item_t *p_matched;

	RBTREE_ASSERT(ips_cl_qmap_overlaps(p_item, start, endp1));
	// Right of p_item not interesting, p_item < everything to its right
	// so looking at p_item and things to it's left
	while (1) {
		RBTREE_PRINT("Candidate "RBTREE_ITEM_FMT"\n", RBTREE_ITEM(p_item));
		cl_map_item_t *p_tmp = p_item->p_left;
		if (p_tmp == p_map->nil_item)
			return p_item;
		if (ips_cl_qmap_overlaps(p_tmp, start, endp1)) {
			// p_tmp overlaps keep checking left
			p_item = p_tmp;
			RBTREE_PRINT("Left Candidate "RBTREE_ITEM_FMT"\n",
					RBTREE_ITEM(p_item));
			continue;
		}
		// p_tmp does not overlap
		p_matched = p_item;	// our best candidate

		// answer is somewhere between p_tmp (left of p_matched) and
		// the things to right of p_tmp or could be p_matched,
		// but answer is not p_tmp and its not anything to the left of p_tmo
		while (1) {
			p_tmp = p_tmp->p_right;
			if (p_tmp == p_map->nil_item) {
				return p_matched;
			}
			if (ips_cl_qmap_overlaps(p_tmp, start, endp1)) {
				p_item = p_tmp;
				RBTREE_PRINT("Right Candidate "RBTREE_ITEM_FMT"\n",
						RBTREE_ITEM(p_item));
				break; // new candidate, look to it's left
			}
			// p_tmp no overlap
			// answer is somewhere between p_matched and p_tmp->p_right
			RBTREE_PRINT("Narrow Right "RBTREE_ITEM_FMT"\n",
						RBTREE_ITEM(p_item));
		}
	}
}
/* return the "first" node in map with buffer overlapping or nil_item.
 * qmap is binary tree sorted by START (p_left, item, p_right).
 * If two entries have same START, one with lower END is earlier.
 * No two entries can have exact same START and END.
 * None of items in qmap can overlap eachother, but start/end can overlap
 * more than 1 item.
 * "first" is defined as the overlapping node with lowest key (RBTREE_CMP
 * or if !defined(RBTREE_CMP) RBTREE_GET_LEFTMOST).
 */
static cl_map_item_t *
ips_cl_qmap_search(cl_qmap_t * const p_map,
		unsigned long start, unsigned long endp1)
{
	RBTREE_ASSERT( p_map );
	cl_map_item_t *p_item = __cl_map_root(p_map);
	ips_cl_qmap_validate(p_map, "search", 1, 1);
	RBTREE_PRINT("search "RBTREE_FMT":"RBTREE_FMT"\n", start, endp1-1);
	while (p_item != p_map->nil_item) {
		if (endp1 <= RBTREE_START(p_item)) {
			RBTREE_PRINT("Going Left of "RBTREE_ITEM_FMT"\n",
							RBTREE_ITEM(p_item));
			p_item = p_item->p_left;
		} else if (start >= RBTREE_ENDP1(p_item)) {
			RBTREE_PRINT("Going Right of "RBTREE_ITEM_FMT"\n",
							RBTREE_ITEM(p_item));
			p_item = p_item->p_right;
		} else {
			// p_item overlaps start/end, find 1st item which overlaps.
			p_item = ips_cl_qmap_select_earliest(p_map, p_item, start, endp1);
			break;
		}
	}
#ifdef RBTREE_PRINT
	if (p_item != p_map->nil_item) {
		RBTREE_PRINT("Selected "RBTREE_ITEM_FMT"\n", RBTREE_ITEM(p_item));
		RBTREE_ASSERT(ips_cl_qmap_overlaps(p_item, start, endp1));
	} else {
		RBTREE_PRINT("Not Found\n");
	}
#endif
	return p_item;
}
#endif /* RBTREE_AUGMENT */
#endif /*defined(RBTREE_START) && ! defined(RBTREE_NO_EMIT_IPS_CL_QMAP_SEARCH)*/

#ifdef RBTREE_CMP
/* return the node in map with exact match (RBTREE_CMP==0) or nil_item.
 * qmap is binary tree sorted by RBTREE_CMP (p_left, item, p_right).
 * No two entries in map can match eachother.
 */
static cl_map_item_t *
ips_cl_qmap_searchv(cl_qmap_t * const p_map, const RBTREE_MI_PL *key)
{
	RBTREE_ASSERT( p_map );
	cl_map_item_t *p_item = __cl_map_root(p_map);
	ips_cl_qmap_validate(p_map, "searchv", 1, 1);

	while (p_item != p_map->nil_item) {
		int cmp = RBTREE_CMP(key, &p_item->payload);
		if (cmp > 0) {
			p_item = p_item->p_right;
		} else if (cmp < 0) {
			p_item = p_item->p_left;
		} else {
			break;
		}
	}

	return p_item;
}
#endif /* defined(RBTREE_CMP) */

#if defined(RBTREE_EMIT_IPS_CL_QMAP_DUMP) || defined(RBTREE_VALIDATE)
static void ips_cl_qmap_traverse_dump(cl_qmap_t *p_map, cl_map_item_t *p_item,
								unsigned cur_depth, unsigned *p_max_depth,
								unsigned *p_min_depth)
{
	if (cur_depth > *p_max_depth)
		*p_max_depth = cur_depth;

	if (p_item->p_left == p_map->nil_item && p_item->p_right == p_map->nil_item
		&& cur_depth < *p_min_depth)
		*p_min_depth = cur_depth;

	if (p_item->p_left != p_map->nil_item) {
		ips_cl_qmap_traverse_dump(p_map, p_item->p_left,
									cur_depth+1, p_max_depth, p_min_depth);
	}
	RBTREE_DUMP_PRINT("item: Depth %u color %u "RBTREE_ITEM_FMT"\n",
							cur_depth, p_item->color, RBTREE_ITEM(p_item));

	if (p_item->p_right != p_map->nil_item) {
		ips_cl_qmap_traverse_dump(p_map, p_item->p_right,
									cur_depth+1, p_max_depth, p_min_depth);
	}
}

static void ips_cl_qmap_dump(cl_qmap_t *p_map)
{
	unsigned max_depth = 0;
	unsigned min_depth = UINT_MAX;

	RBTREE_DUMP_PRINT("Count: %lu\n",
			(unsigned long)RBTREE_MAP_COUNT(&p_map->payload));
	if (__cl_map_root(p_map) != p_map->nil_item)
		ips_cl_qmap_traverse_dump(p_map, __cl_map_root(p_map), 1, &max_depth,
									&min_depth);
	RBTREE_DUMP_PRINT("Max Depth: %u Min Depth: %u\n", max_depth, min_depth);
}
#endif /* defined(RBTREE_EMIT_IPS_CL_QMAP_DUMP) || defined(RBTREE_VALIDATE) */

#ifdef RBTREE_VALIDATE
// validate a < b
// (for tests of just leftmost allow a <= b)
// count as 0 or 1 failure, even though 3 order checks
static int ips_cl_qmap_validate_order(cl_map_item_t *a, cl_map_item_t *b,
							unsigned cur_depth,  const char *msg)
{
	int err_cnt = 0;
#ifdef RBTREE_CMP
	if (RBTREE_CMP(&a->payload, &b->payload) >= 0) {
		RBTREE_VALIDATE_PRINT("bad cmp order %s: Depth %u"RBTREE_ITEM_FMT" >= "RBTREE_ITEM_FMT"\n",
					msg, cur_depth, RBTREE_ITEM(a), RBTREE_ITEM(b));
		err_cnt=1;
	}
#endif
	if (RBTREE_START(a) > RBTREE_START(b)) {
		RBTREE_VALIDATE_PRINT("bad leftmost order %s: Depth %u "RBTREE_ITEM_FMT" > "RBTREE_ITEM_FMT"\n",
					msg, cur_depth, RBTREE_ITEM(a), RBTREE_ITEM(b));
		err_cnt=1;
	}
#ifdef RBTREE_ENDP1
	if (RBTREE_START(a) == RBTREE_START(b)
		&& RBTREE_ENDP1(a) > RBTREE_ENDP1(b)) {
		RBTREE_VALIDATE_PRINT("bad rightmost order %s: Depth %u "RBTREE_ITEM_FMT" >= "RBTREE_ITEM_FMT"\n",
					msg, cur_depth, RBTREE_ITEM(a), RBTREE_ITEM(b));
		err_cnt=1;
	}
#endif
#if ! defined(RBTREE_AUGMENT) && ! defined(RBTREE_NO_EMIT_IPS_CL_QMAP_SEARCH)
	// items cannot overlap eachother
	if (RBTREE_START(a) < RBTREE_ENDP1(b) && RBTREE_ENDP1(a) > RBTREE_START(b)) {
		RBTREE_VALIDATE_PRINT("invalid overlap %s: Depth %u "RBTREE_ITEM_FMT" overlaps "RBTREE_ITEM_FMT"\n",
					msg, cur_depth, RBTREE_ITEM(a), RBTREE_ITEM(b));
		err_cnt=1;
	}
#endif
	return err_cnt;
}

// p_max_endp1 only used for RBTREE_AUGMENT, but less clutter if always supply
static int ips_cl_qmap_traverse_validate(cl_qmap_t *p_map,
								cl_map_item_t *p_item, unsigned cur_depth,
								int check_rules, unsigned *p_max_depth,
								unsigned *p_min_depth, unsigned *p_black_cnt,
								unsigned long *visited,
								unsigned long *p_max_endp1)

{
	unsigned long max_left = 0;
	unsigned long max_right = 0;
	unsigned black_left = 0;
	unsigned black_right = 0;
	int err_cnt = 0;

	(*visited)++;
	if (cur_depth > *p_max_depth)
		*p_max_depth = cur_depth;

	if (p_item->p_left == p_map->nil_item && p_item->p_right == p_map->nil_item
		&& cur_depth < *p_min_depth)
		*p_min_depth = cur_depth;

	if (check_rules && p_item->color == CL_MAP_RED) {
		// red node cannot have red children (NIL is black)
		if (p_item->p_left->color == CL_MAP_RED) {
			RBTREE_VALIDATE_PRINT("Red node has red left child: Depth %u "RBTREE_ITEM_FMT" child "RBTREE_ITEM_FMT"\n",
					cur_depth, RBTREE_ITEM(p_item), RBTREE_ITEM(p_item->p_left));
			err_cnt++;
		}
		if (p_item->p_right->color == CL_MAP_RED) {
			RBTREE_VALIDATE_PRINT("Red node has red right child: Depth %u "RBTREE_ITEM_FMT" child "RBTREE_ITEM_FMT"\n",
					cur_depth, RBTREE_ITEM(p_item), RBTREE_ITEM(p_item->p_right));
			err_cnt++;
		}
	}

	if (p_item->p_left != p_map->nil_item) {
		err_cnt += ips_cl_qmap_validate_order(p_item->p_left, p_item,
											cur_depth, "left child");
		if (p_item->p_left->p_up != p_item) {
			RBTREE_VALIDATE_PRINT("bad left child up: Depth %u "RBTREE_ITEM_FMT" child "RBTREE_ITEM_FMT"\n",
					cur_depth, RBTREE_ITEM(p_item), RBTREE_ITEM(p_item->p_left));
			err_cnt++;
		}
		err_cnt += ips_cl_qmap_traverse_validate(p_map, p_item->p_left,
										cur_depth+1, check_rules, p_max_depth,
										p_min_depth, &black_left,
										visited, &max_left);
	} else {
		black_left = 1;	// count our nil child
	}

	if (p_item->p_right != p_map->nil_item) {
		err_cnt += ips_cl_qmap_validate_order(p_item, p_item->p_right,
											cur_depth, "right child");
		if (p_item->p_right->p_up != p_item) {
			RBTREE_VALIDATE_PRINT("bad right child up: Depth %u "RBTREE_ITEM_FMT" child "RBTREE_ITEM_FMT"\n",
					cur_depth, RBTREE_ITEM(p_item), RBTREE_ITEM(p_item->p_right));
			err_cnt++;
		}
		err_cnt += ips_cl_qmap_traverse_validate(p_map, p_item->p_right,
										cur_depth+1, check_rules, p_max_depth,
										p_min_depth, &black_right,
										visited, &max_right);
	} else {
		black_right = 1;	// count our nil child
	}
	if (check_rules && black_right != black_left) {
		RBTREE_VALIDATE_PRINT("black depth inconsistent: Depth %u "RBTREE_ITEM_FMT" left %u right %u\n",
					cur_depth, RBTREE_ITEM(p_item), black_left, black_right);
		err_cnt++;
	}
	// use max() in case black_left != black_right
	if (p_item->color == CL_MAP_BLACK)
		*p_black_cnt = max(black_left, black_right)+1;
	else
		*p_black_cnt = max(black_left, black_right);

#ifdef RBTREE_AUGMENT
	*p_max_endp1 = max(max_left, max(max_right,
					 RBTREE_ENDP1(p_item)));
	if (*p_max_endp1 != p_item->max_endp1) {
		RBTREE_VALIDATE_PRINT("bad max_end: Depth %u "RBTREE_ITEM_FMT" expected: "RBTREE_FMT"\n",
						cur_depth, RBTREE_ITEM(p_item), *p_max_endp1-1);
		err_cnt++;
	}
#endif

	return err_cnt;
}

// check_rules verifies the definitional rules of a red-black tree
// - nil_item is black
// - a red node does not have any red children
// - every path from a node to the bottom of the tree traverses an equal number
//        of black nodes for left and right paths to bottom
// - path from root to bottom of deepest part of tree tree is < 2x path
//        of shallowest part of tree
static int ips_cl_qmap_validate(cl_qmap_t *p_map, const char *msg,
								int check_rules, int do_assert)
{
	int err_cnt = 0;
	unsigned long max_endp1 = 0;
	unsigned long visited = 0;
	unsigned max_depth = 0;
	unsigned min_depth = UINT_MAX;
	unsigned black_cnt = 0;

	if (! RBTREE_VALIDATE_ENABLE)
		return 0;

	// traverse tree
	cl_map_item_t *p_item = __cl_map_root(p_map);
	if (p_item != p_map->nil_item) {
		if (p_item->p_up != p_map->root) {
			RBTREE_VALIDATE_PRINT("bad root up: "RBTREE_ITEM_FMT"\n",
					RBTREE_ITEM(p_item));
			err_cnt++;
		}
		err_cnt += ips_cl_qmap_traverse_validate(p_map, p_item, 1, check_rules,
								&max_depth, &min_depth, &black_cnt,
								&visited, &max_endp1);
	}
	if (check_rules && p_map->nil_item->color != CL_MAP_BLACK) {
		RBTREE_VALIDATE_PRINT("NIL Item color %u incorrect, should be %u\n",
				p_map->nil_item->color, CL_MAP_BLACK);
		err_cnt++;
	}
	// +1 for a fudge to be a little tolerant
	if (check_rules && max_depth > (2*min_depth)+1) {
		RBTREE_VALIDATE_PRINT("Max_Depth (%u) > 2*Min_Depth (%u)\n",
				max_depth, min_depth);
		err_cnt++;
	}
	p_map->depth = max_depth;
	if (max_depth > p_map->max_depth)
		p_map->max_depth = max_depth;
	if (visited != (unsigned long)RBTREE_MAP_COUNT(&p_map->payload)) {
		RBTREE_VALIDATE_PRINT("Count inconsistent with tree: Count: %lu Visited: %lu\n",
				(unsigned long)RBTREE_MAP_COUNT(&p_map->payload), visited);
		err_cnt++;
	}

	// now walk iterator
	p_item = ips_cl_qmap_first(p_map);
	cl_map_item_t *p_prev = p_map->nil_item;
	visited = 0;
	for ( ; p_item != p_map->nil_item;
			p_prev = p_item, p_item = ips_cl_qmap_successor(p_map, p_item)) {
		visited++;
		if (p_prev != p_map->nil_item) {
			err_cnt += ips_cl_qmap_validate_order(p_prev, p_item, 0,
											"predecessor");
#ifndef RBTREE_NO_EMIT_IPS_CL_QMAP_PREDECESSOR
			cl_map_item_t *p_tmp = ips_cl_qmap_predecessor(p_map, p_item);
			if (p_tmp != p_prev) {
				RBTREE_VALIDATE_PRINT("bad predecessor: "RBTREE_ITEM_FMT
						" got "RBTREE_ITEM_FMT" expected "RBTREE_ITEM_FMT"\n",
						RBTREE_ITEM(p_item), RBTREE_ITEM(p_tmp),
						RBTREE_ITEM(p_prev));
			}
#endif
		}
	}
	if (visited != (unsigned long)RBTREE_MAP_COUNT(&p_map->payload)) {
		RBTREE_VALIDATE_PRINT("Count inconsistent with iterator: Count: %lu Visited: %lu\n",
				(unsigned long)RBTREE_MAP_COUNT(&p_map->payload), visited);
		err_cnt++;
	}

	if (err_cnt) {
		RBTREE_VALIDATE_PRINT("%d Validate errors in %s\n", err_cnt, msg);
		ips_cl_qmap_dump(p_map);
		if (do_assert)
			RBTREE_ASSERT(0);
	}
	return err_cnt;
}
#endif /* RBTREE_VALIDATE */

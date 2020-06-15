/*
 * Copyright (c) 2015-2016 Cray Inc.  All rights reserved.
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
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

#ifndef _GNIX_FREELIST_H_
#define _GNIX_FREELIST_H_

#include <ofi.h>
#include <ofi_list.h>
#include "include/gnix_util.h"

/* Number of elements to seed the freelist with */
#define GNIX_FL_INIT_SIZE 100
/* Refill growth factor */
#define GNIX_FL_GROWTH_FACTOR 2

/** Free list implementation
 *
 * @var freelist           The free list itself
 * @var chunks             Memory chunks (must be saved for freeing)
 * @var refill_size        Number of elements for the next refill
 * @var growth_factor      Factor for increasing refill size
 * @var max_refill_size;   Max refill size
 * @var elem_size          Size of element (in bytes)
 * @var offset             Offset of dlist_entry field (in bytes)
 */
struct gnix_freelist {
	struct dlist_entry freelist;
	struct slist chunks;
	int refill_size;
	int growth_factor;
	int max_refill_size;
	int elem_size;
	int offset;
	int ts;
	fastlock_t lock;
};

/** Initializes a gnix_freelist
 *
 * @param elem_size         Size of element
 * @param offset            Offset of dlist_entry field
 * @param init_size         Initial freelist size
 * @param refill_size       Number of elements for next refill
 * @param growth_factor     Factor for increasing refill size
 * @param max_refill_size   Max refill size
 * @param fl                gnix_freelist
 * @return                  FI_SUCCESS on success, -FI_ENOMEM on failure
 * @note - If the refill_size is zero, then the freelist is not growable.
 */
int _gnix_fl_init(int elem_size, int offset, int init_size,
		   int refill_size, int growth_factor,
		   int max_refill_size, struct gnix_freelist *fl);

/** Initializes a thread safe gnix_freelist
 *
 * @param elem_size         Size of element
 * @param offset            Offset of dlist_entry field
 * @param init_size         Initial freelist size
 * @param refill_size       Number of elements for next refill
 * @param growth_factor     Factor for increasing refill size
 * @param max_refill_size   Max refill size
 * @param fl                gnix_freelist
 * @return                  FI_SUCCESS on success, -FI_ENOMEM on failure
 * @note - If the refill_size is zero, then the freelist is not growable.
 */
int _gnix_fl_init_ts(int elem_size, int offset, int init_size,
		      int refill_size, int growth_factor,
		      int max_refill_size, struct gnix_freelist *fl);

/** Clean up a gnix_freelist, including deleting memory chunks
 *
 * @param fl    Freelist
 */
void _gnix_fl_destroy(struct gnix_freelist *fl);

extern int __gnix_fl_refill(struct gnix_freelist *fl, int n);

/** Return an item from the freelist
 *
 * @param e     item
 * @param fl    gnix_freelist
 * @return      FI_SUCCESS on success, -FI_ENOMEM or -FI_EAGAIN on failure,
 *              or -FI_ECANCELED if the refill size is zero.
 */
__attribute__((unused))
static inline int _gnix_fl_alloc(struct dlist_entry **e, struct gnix_freelist *fl)
{
    int ret = FI_SUCCESS;
    struct dlist_entry *de = NULL;

    assert(fl);

    if (fl->ts)
	    fastlock_acquire(&fl->lock);

    if (dlist_empty(&fl->freelist)) {

        if (fl->refill_size == 0) {
                ret = -FI_ECANCELED;

                GNIX_DEBUG(FI_LOG_DEBUG, "Freelist not growable (refill "
                                   "size is 0\n");

                goto err;
        }

        ret = __gnix_fl_refill(fl, fl->refill_size);
        if (ret != FI_SUCCESS)
            goto err;
        if (fl->refill_size < fl->max_refill_size) {
            int ns = fl->refill_size *= fl->growth_factor;

            fl->refill_size = (ns >= fl->max_refill_size ?
                            fl->max_refill_size : ns);
        }

        if (dlist_empty(&fl->freelist)) {
            /* Can't happen unless multithreaded */
            ret = -FI_EAGAIN;
            goto err;
        }
    }

    de = fl->freelist.next;
    dlist_remove_init(de);

    *e = de;
err:
    if (fl->ts)
        fastlock_release(&fl->lock);
    return ret;
}

/** Return an item to the free list
 *
 * @param e     item
 * @param fl    gnix_freelist
 */
__attribute__((unused))
static inline void _gnix_fl_free(struct dlist_entry *e, struct gnix_freelist *fl)
{
    assert(e);
    assert(fl);

    e->next = NULL;  /* keep slist implementation happy */

    if (fl->ts)
        fastlock_acquire(&fl->lock);
    dlist_init(e);
    dlist_insert_head(e, &fl->freelist);
    if (fl->ts)
        fastlock_release(&fl->lock);
}


/** Is freelist empty (primarily used for testing
 *
 * @param fl    gnix_freelist
 * @return      True if list is currently empty, false otherwise
 */
static inline int _gnix_fl_empty(struct gnix_freelist *fl)
{
	return dlist_empty(&fl->freelist);
}

#endif /* _GNIX_FREELIST_H_ */

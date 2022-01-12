/*
* Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
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

#ifndef _FI_NETDIR_BUF_H_
#define _FI_NETDIR_BUF_H_

#include <windows.h>

#include "rdma/fabric.h"

#include "ofi.h"
#include "ofi_osd.h"

#include "netdir.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Implementation of lock-free thread safe allocator of fixed size data */
/*
 * Lock free allocator is implemented using atomics add/compare-and-swap/sub.
 * There is three levels of data:
 * footer - high level structure, holds links to first chunk, first non-used
 * |     element, number of allocated elelements and number of used elements.
 * |
 * +-chunk - one-directional linked list where stored items
 * |
 * +-item - one directional linked list where stored user's data
 *
 * chunk elements are linked to be freed only, nothing else used linked nature of
 * chunk.
 * item elemens may have two states: used/not-used. All not-used (free) elements
 * are linked into one-directional linked list, pointer to first non-used
 * element is located at footer.
 *
 * when user allocates new element function buf_alloc looks for number of used
 * elements and compares it with allocated number of elements: used atomic
 * API ofi_atomic_inc32 to increment counter by 1 and check if there
 * is non-used items. In case if no more available elements exist
 * (used > counter) - allocate new chunk.
 *
 * All pointers are updated using atomic API compare-and-swap which guarantee
 * thread safe of access to allocator. Only init/fini calls are not thread safe.
 *
 * Call of ofi_nd_buf_init_*** API is not mandatory - default initialization
 * by NULL is enough
 *
 * Note: underscored (__ofi_nd_buf_****) functions are used to implement footer
 * inside of another objects
 */

#define OFI_ND_BUF_DEFCOUNT 1024
#define ICEP InterlockedCompareExchangePointer
#define countof(x) _countof(x) /*(sizeof(x) / sizeof(*(x)))*/

#define OFI_ND_NB_BUF_IMP(name) struct nd_buf_footer_##name nd_footer_##name = {0};

#define OFI_ND_NB_BUF_IMP_ALLOC(name, a, f) \
	struct nd_buf_footer_##name nd_footer_##name = {.alloc_chunk = a, .free_chunk = f};

#define OFI_ND_NB_BUF(name) OFI_ND_NB_BUF_TYPED(name, name)

#define OFI_ND_NB_BUF_TYPED(name, type)								\
	struct nd_buf_item_##name {								\
		struct nd_buf_item_##name *next; /* next free element */			\
		type data;			 /* user's data */				\
	};											\
												\
	struct nd_buf_chunk_##name {	/* cache of elements for fast alloc */			\
		struct nd_buf_chunk_##name *next;						\
		struct nd_buf_item_##name item[OFI_ND_BUF_DEFCOUNT];				\
	};											\
												\
	struct nd_buf_footer_##name {								\
		LONG count; /* number of elements */						\
		LONG used; /* count of used elements */						\
		struct nd_buf_chunk_##name*(*alloc_chunk)(					\
			struct nd_buf_footer_##name *footer, size_t *count);			\
		void(*free_chunk)(struct nd_buf_chunk_##name *chunk);				\
		volatile struct nd_buf_chunk_##name *chunk_head;				\
		volatile struct nd_buf_item_##name  *item_free;					\
	};											\
												\
	extern struct nd_buf_footer_##name nd_footer_##name;					\
												\
	static inline int __ofi_nd_buf_init_##name(struct nd_buf_footer_##name *footer)		\
	{											\
		assert(footer);									\
		memset(footer, 0, sizeof(*footer));						\
		return FI_SUCCESS;								\
	}											\
												\
	static inline int ofi_nd_buf_init_##name(void)						\
	{											\
		return __ofi_nd_buf_init_##name(&nd_footer_##name);				\
	}											\
												\
	static inline void __ofi_nd_buf_fini_##name(struct nd_buf_footer_##name *footer)	\
	{											\
		typedef struct nd_buf_chunk_##name nd_buf_chunk_##name;				\
		assert(footer);									\
		assert(!footer->used);								\
												\
		nd_buf_chunk_##name *next = (nd_buf_chunk_##name*)footer->chunk_head;		\
		while (next) {									\
			nd_buf_chunk_##name *chunk = next;					\
			next = chunk->next;							\
			if(!footer->free_chunk)							\
				free(chunk);							\
			else									\
				footer->free_chunk(chunk);					\
		}										\
		memset(footer, 0, sizeof(*footer));						\
	}											\
												\
	static inline void ofi_nd_buf_fini_##name(void)						\
	{											\
		__ofi_nd_buf_fini_##name(&nd_footer_##name);					\
	}											\
												\
	/* Do NOT clean item after allocation because there may be user's data */		\
	static inline type *__ofi_nd_buf_alloc_##name(struct nd_buf_footer_##name *footer)	\
	{											\
		typedef struct nd_buf_chunk_##name nd_buf_chunk_##name;				\
		typedef struct nd_buf_item_##name nd_buf_item_##name;				\
												\
		assert(footer);									\
												\
		if (InterlockedIncrement(&footer->used) > footer->count) {			\
			/* allocate new chunk of data */					\
			size_t i;								\
			nd_buf_chunk_##name *data;						\
			nd_buf_chunk_##name *next;						\
			nd_buf_item_##name *next_free;						\
			size_t count;								\
												\
			if(!footer->alloc_chunk) {						\
				data = (nd_buf_chunk_##name*)malloc(sizeof(*data));		\
				memset(data, 0, sizeof(*data));					\
				count = countof(data->item);					\
			}									\
			else {									\
				data = footer->alloc_chunk(footer, &count);			\
			}									\
			if (!data || !count)							\
				return 0;							\
			for (i = 0; i < count - 1; i++) {					\
				data->item[i].next = &data->item[i + 1];			\
			}									\
			/* insert new created free elements into linked list */			\
			/* data->item[0] will be returned to user as used element */		\
			/* items data->item[1..N] are linked to non-used list elements */	\
			do {									\
				next_free = (nd_buf_item_##name*)footer->item_free;		\
				data->item[count - 1].next = next_free;				\
			} while (ICEP((volatile PVOID*)&footer->item_free,			\
				      (void*)&data->item[1], (void*)next_free) != next_free);	\
												\
			InterlockedAdd(&footer->count, (LONG)count);				\
												\
			/* add new chunk into footer */						\
			do {									\
				next = (nd_buf_chunk_##name*)footer->chunk_head;		\
				data->next = next;						\
			} while (ICEP((volatile PVOID*)&footer->chunk_head,			\
				      (void*)data, (void*)next) != next);			\
			return &data->item[0].data;						\
		}										\
												\
		nd_buf_item_##name *top_free;							\
		nd_buf_item_##name *next_free;							\
		do {										\
			assert(footer->item_free);						\
			top_free = (nd_buf_item_##name *)footer->item_free;			\
			next_free = (nd_buf_item_##name *)footer->item_free->next;		\
		} while (ICEP((volatile PVOID*)&footer->item_free,				\
			(void*)next_free, (void*)top_free) != top_free);			\
												\
		return &top_free->data;								\
	}											\
												\
	static inline type *ofi_nd_buf_alloc_##name(void)					\
	{											\
		return __ofi_nd_buf_alloc_##name(&nd_footer_##name);				\
	}											\
												\
	static inline 										\
	void __ofi_nd_buf_free_##name(type *data, struct nd_buf_footer_##name *footer)		\
	{											\
		typedef struct nd_buf_item_##name nd_buf_item_##name;				\
												\
		assert(footer);									\
		assert(data);									\
												\
		nd_buf_item_##name *item = container_of(data, struct nd_buf_item_##name, data);	\
												\
		do {										\
			item->next = (nd_buf_item_##name*)footer->item_free;			\
		} while (ICEP((volatile PVOID*)&footer->item_free,				\
			 (void*)item, (void*)item->next) != item->next);			\
												\
		LONG dec = InterlockedDecrement(&footer->used);					\
		assert(dec >= 0);								\
	}											\
												\
	static inline void ofi_nd_buf_free_##name(type *data)					\
	{											\
		__ofi_nd_buf_free_##name(data, &nd_footer_##name);				\
	}

#define ND_BUF_ALLOC(name) ofi_nd_buf_alloc_##name()
#define ND_BUF_FREEPTR(name) ofi_nd_buf_free_##name
#define ND_BUF_FREE(name, ptr) ND_BUF_FREEPTR(name)(ptr)
#define ND_BUF_CHUNK(name) struct nd_buf_chunk_##name
#define ND_BUF_FOOTER(name) struct nd_buf_footer_##name
#define ND_BUF_FINIPTR(name) ofi_nd_buf_fini_##name
#define ND_BUF_FINI(name) ND_BUF_FINIPTR(name)()


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FI_NETDIR_BUF_H_ */

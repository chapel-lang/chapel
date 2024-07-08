/*
 * Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2018 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#ifndef _OFI_MEM_H_
#define _OFI_MEM_H_

#include <config.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ofi.h>
#include <ofi_list.h>
#include <ofi_osd.h>

#ifdef INCLUDE_VALGRIND
#   include <valgrind/memcheck.h>
#   ifndef VALGRIND_MAKE_MEM_DEFINED
#      warning "Valgrind requested, but VALGRIND_MAKE_MEM_DEFINED undefined"
#   endif
#endif

#ifndef VALGRIND_MAKE_MEM_DEFINED
#   define VALGRIND_MAKE_MEM_DEFINED(addr, len)
#endif


void ofi_mem_init(void);
void ofi_mem_fini(void);

enum {
	OFI_PAGE_SIZE,
	OFI_DEF_HUGEPAGE_SIZE,
};

extern size_t *page_sizes;
extern size_t num_page_sizes;

static inline long ofi_get_page_size(void)
{
	return ofi_sysconf(_SC_PAGESIZE);
}
ssize_t ofi_get_hugepage_size(void);

size_t ofi_get_mem_size(void);


/* We implement memdup to avoid external library dependency */
static inline void *mem_dup(const void *src, size_t size)
{
	void *dest = malloc(size);

	if (dest)
		memcpy(dest, src, size);
	return dest;
}

static inline int ofi_str_dup(const char *src, char **dst)
{
	if (src) {
		*dst = strdup(src);
		if (!*dst)
			return -FI_ENOMEM;
	} else {
		*dst = NULL;
	}
	return 0;
}

/* Dynamic array -- see ofi_indexer.h */

/*
 * Buffer pool (free stack) template
 */
#define OFI_FREESTACK_EMPTY	NULL

#define ofi_freestack_get_next(user_buf) ((char *)user_buf - sizeof(void *))
#define ofi_freestack_get_user_buf(entry) ((char *)entry + sizeof(void *))

#if ENABLE_DEBUG
#define ofi_freestack_init_next(entry)	*((void **)entry) = NULL
#define ofi_freestack_check_next(entry)	assert(*((void **)entry) == NULL)
#else
#define ofi_freestack_init_next(entry)
#define ofi_freestack_check_next(entry)
#endif

#define OFI_FREESTACK_HEADER 					\
	size_t		size;					\
	void		*next;					\

#define ofi_freestack_isempty(fs) ((fs)->next == OFI_FREESTACK_EMPTY)
#define ofi_freestack_push(fs, p)				\
do {								\
	ofi_freestack_check_next(ofi_freestack_get_next(p));	\
	*(void **) (ofi_freestack_get_next(p)) = (fs)->next;	\
	(fs)->next = (ofi_freestack_get_next(p));		\
} while (0)
#define ofi_freestack_pop(fs) ofi_freestack_pop_impl(fs, (fs)->next)

static inline void* ofi_freestack_pop_impl(void *fs, void *fs_next)
{
	struct _freestack {
		OFI_FREESTACK_HEADER
	} *freestack = (struct _freestack *)fs;
	assert(!ofi_freestack_isempty(freestack));
	freestack->next = *((void **)fs_next);
	ofi_freestack_init_next(fs_next);
	return ofi_freestack_get_user_buf(fs_next);
}

#define OFI_DECLARE_FREESTACK(entrytype, name)			\
struct name ## _entry {						\
	void		*next;					\
	entrytype	buf;					\
};								\
struct name {							\
	OFI_FREESTACK_HEADER					\
	struct name ## _entry	entry[];			\
};								\
								\
typedef void (*name ## _entry_init_func)(entrytype *buf,	\
					 void *arg);		\
								\
typedef void (*name ## _entry_destroy_func)(entrytype *buf,	\
					 void *arg);		\
								\
static inline void						\
name ## _init(struct name *fs, size_t size,			\
	      name ## _entry_init_func init, void *arg)		\
{								\
	ssize_t i;						\
	assert(size == roundup_power_of_two(size));		\
	assert(sizeof(fs->entry[0].buf) >= sizeof(void *));	\
	fs->size = size;					\
	fs->next = OFI_FREESTACK_EMPTY;				\
	for (i = size - 1; i >= 0; i--) {			\
		if (init)					\
			init(&fs->entry[i].buf, arg);		\
		ofi_freestack_push(fs, &fs->entry[i].buf);	\
	}							\
}								\
								\
static inline struct name *					\
name ## _create(size_t size, name ## _entry_init_func init,	\
		void *arg)					\
{								\
	struct name *fs;					\
	fs = (struct name*) calloc(1, sizeof(*fs) +		\
		       sizeof(struct name ## _entry) *		\
		       (roundup_power_of_two(size)));		\
	if (fs)							\
		name ##_init(fs, roundup_power_of_two(size),	\
			     init, arg);			\
	return fs;						\
}								\
								\
static inline int name ## _index(struct name *fs,		\
				 entrytype *entry)		\
{								\
	return (int)((struct name ## _entry *)			\
			(ofi_freestack_get_next(entry))		\
			- (struct name ## _entry *)fs->entry);	\
}								\
								\
static inline void name ## _free(struct name *fs)		\
{								\
	free(fs);						\
}								\
static inline void name ## _destroy(struct name *fs,		\
	size_t size, name ## _entry_destroy_func destroy,	\
	void *arg)						\
{								\
	ssize_t i;						\
	for (i = size - 1; i >= 0; i--) {			\
		if (destroy)					\
			destroy(&fs->entry[i].buf, arg);	\
	}							\
	free(fs);						\
}								\
void dummy ## name (void) /* work-around global ; scope */

/*
 * Buffer pool (free stack) template for shared memory regions
 */
#define SMR_ALIGN_BOUNDARY	4096
#define SMR_FREESTACK_EMPTY	(-1)

struct smr_freestack {
	uint64_t		entry_base_offset;
	size_t			object_size;
	size_t			size;
	int16_t			free;
	int16_t			top;
	int16_t 		entry_next[];
};

#define smr_freestack_isempty(fs)	((fs)->top == SMR_FREESTACK_EMPTY)
#define smr_freestack_isfull(fs)	((fs)->free == (fs)->size)

static inline void* smr_freestack_get_entry_from_index(struct smr_freestack *fs,
		int16_t index)
{
	return (void*) (((char *) fs) + fs->entry_base_offset +
			(fs->object_size * index));
}

static inline long freestack_size(int elem_size, int num_elements)
{
	return (sizeof(struct smr_freestack) + sizeof(int16_t) * num_elements +
			elem_size * num_elements + SMR_ALIGN_BOUNDARY);
}

/* Push by entry_index */
static inline void smr_freestack_push_by_index(struct smr_freestack *fs,
		int16_t entry_index)
{
	fs->entry_next[entry_index] = fs->top;
	fs->top = entry_index;
	fs->free++;
}

/* Push by entry_offset */
static inline void smr_freestack_push_by_offset(struct smr_freestack *fs,
		uint64_t entry_offset)
{
        smr_freestack_push_by_index(fs,
                        (entry_offset - fs->entry_base_offset) /
                        fs->object_size);
}

/* Push by object */
static inline void smr_freestack_push(struct smr_freestack *fs, void *local_p)
{
        smr_freestack_push_by_offset(fs,
                ((char *) local_p - (char*) fs));
}

static inline void smr_freestack_init(struct smr_freestack *fs, size_t elem_count,
		size_t fs_object_size)
{
	ssize_t i, next_aligned_addr;
	assert(elem_count == roundup_power_of_two(elem_count));
	fs->size = elem_count;
	fs->free = 0;
	fs->object_size = fs_object_size;
	fs->top = SMR_FREESTACK_EMPTY;
	fs->entry_base_offset =
		((char*) &fs->entry_next[0] - (char*) fs) +
		fs->size * sizeof(fs->top);
	next_aligned_addr = ofi_get_aligned_size((( (uint64_t) fs) +
			fs->entry_base_offset), SMR_ALIGN_BOUNDARY);
	fs->entry_base_offset = next_aligned_addr - ((uint64_t) fs);
	for (i = elem_count - 1; i >= 0; i--)
		smr_freestack_push_by_index(fs, i);
}

static inline struct smr_freestack* smr_freestack_create(size_t elem_count,
		size_t fs_object_size)
{
	struct smr_freestack *fs;
	fs = (struct smr_freestack*) calloc(1, freestack_size(fs_object_size,
				roundup_power_of_two(elem_count)));
	if (fs)
		smr_freestack_init(fs, elem_count, fs_object_size);
	return fs;
}

static inline int smr_freestack_pop_by_index(struct smr_freestack *fs)
{
	int entry_index;

	entry_index = fs->top;
	fs->top = fs->entry_next[entry_index];
	fs->entry_next[entry_index] = -1;
	fs->free--;

	return entry_index;
}

static inline size_t smr_freestack_pop_by_offset(struct smr_freestack *fs)
{
	return (size_t) (fs->entry_base_offset +
		smr_freestack_pop_by_index(fs) * fs->object_size);
}

static inline void* smr_freestack_pop(struct smr_freestack *fs)
{
	return (void *) ( ((char*)fs) + smr_freestack_pop_by_offset(fs) );
}
/*
 * Buffer Pool
 */

enum {
	OFI_BUFPOOL_INDEXED		= 1 << 1,
	OFI_BUFPOOL_NO_TRACK		= 1 << 2,
	OFI_BUFPOOL_HUGEPAGES		= 1 << 3,
	OFI_BUFPOOL_NONSHARED		= 1 << 4,
};

struct ofi_bufpool_region;

struct ofi_bufpool_attr {
	size_t 		size;
	size_t 		alignment;
	size_t	 	max_cnt;
	size_t 		chunk_cnt;
	int		(*alloc_fn)(struct ofi_bufpool_region *region);
	void		(*free_fn)(struct ofi_bufpool_region *region);
	void		(*init_fn)(struct ofi_bufpool_region *region, void *buf);
	void 		*context;
	int		flags;
};

struct ofi_bufpool {
	union {
		struct slist		entries;
		struct dlist_entry	regions;
	} free_list;

	size_t 				entry_size;
	size_t 				entry_cnt;

	struct ofi_bufpool_region	**region_table;
	size_t				region_cnt;
	size_t				alloc_size;
	size_t				region_size;
	struct ofi_bufpool_attr		attr;
};

struct ofi_bufpool_region {
	struct dlist_entry		entry;
	struct dlist_entry 		free_list;
	char				*alloc_region;
	char 				*mem_region;
	size_t				index;
	void 				*context;
	struct ofi_bufpool 		*pool;
	int				flags;
	OFI_DBG_VAR(ofi_atomic32_t,	use_cnt)
};

struct ofi_bufpool_ftr {
	size_t				magic;
};

struct ofi_bufpool_hdr {
	union {
		struct slist_entry	slist;
		struct dlist_entry	dlist;
	} entry;
	struct ofi_bufpool_region	*region;
	size_t 				index;

	OFI_DBG_VAR(bool, allocated)
	OFI_DBG_VAR(struct ofi_bufpool_ftr *, ftr)
	OFI_DBG_VAR(size_t,		magic)
};

int ofi_bufpool_create_attr(struct ofi_bufpool_attr *attr,
			    struct ofi_bufpool **buf_pool);

static inline int
ofi_bufpool_create(struct ofi_bufpool **buf_pool,
		   size_t size, size_t alignment,
		   size_t max_cnt, size_t chunk_cnt, int flags)
{
	struct ofi_bufpool_attr attr = {
		.size		= size,
		.alignment 	= alignment,
		.max_cnt	= max_cnt,
		.chunk_cnt	= chunk_cnt,
		.flags		= flags,
	};
	return ofi_bufpool_create_attr(&attr, buf_pool);
}

void ofi_bufpool_destroy(struct ofi_bufpool *pool);

int ofi_bufpool_grow(struct ofi_bufpool *pool);

static inline struct ofi_bufpool_hdr *ofi_buf_hdr(void *buf)
{
	return (struct ofi_bufpool_hdr *)
		((char *) buf - sizeof(struct ofi_bufpool_hdr));
}

static inline void *ofi_buf_data(struct ofi_bufpool_hdr *buf_hdr)
{
	return buf_hdr + 1;
}

static inline struct ofi_bufpool_region *ofi_buf_region(void *buf)
{
	assert(ofi_buf_hdr(buf)->region);
	return ofi_buf_hdr(buf)->region;
}

static inline struct ofi_bufpool *ofi_buf_pool(void *buf)
{
	assert(ofi_buf_region(buf)->pool);
	return ofi_buf_region(buf)->pool;
}

static inline void ofi_buf_free(void *buf)
{
	assert(ofi_atomic_dec32(&ofi_buf_region(buf)->use_cnt) >= 0);
	assert(!(ofi_buf_pool(buf)->attr.flags & OFI_BUFPOOL_INDEXED));
	assert(ofi_buf_hdr(buf)->magic == OFI_MAGIC_SIZE_T);
	assert(ofi_buf_hdr(buf)->ftr->magic == OFI_MAGIC_SIZE_T);
	assert(ofi_buf_hdr(buf)->allocated == true);

	OFI_DBG_SET(ofi_buf_hdr(buf)->allocated, false);

	slist_insert_head(&ofi_buf_hdr(buf)->entry.slist,
			  &ofi_buf_pool(buf)->free_list.entries);
}

int ofi_ibuf_is_lower(struct dlist_entry *item, const void *arg);
int ofi_ibufpool_region_is_lower(struct dlist_entry *item, const void *arg);

static inline void ofi_ibuf_free(void *buf)
{
	struct ofi_bufpool_hdr *buf_hdr;

	buf_hdr = ofi_buf_hdr(buf);

	assert(ofi_atomic_dec32(&ofi_buf_region(buf)->use_cnt) >= 0);
	assert(ofi_buf_pool(buf)->attr.flags & OFI_BUFPOOL_INDEXED);
	assert(buf_hdr->magic == OFI_MAGIC_SIZE_T);
	assert(buf_hdr->ftr->magic == OFI_MAGIC_SIZE_T);
	assert(buf_hdr->allocated == true);

	OFI_DBG_SET(buf_hdr->allocated, false);

	dlist_insert_order(&buf_hdr->region->free_list,
			   ofi_ibuf_is_lower, &buf_hdr->entry.dlist);
	if (dlist_empty(&buf_hdr->region->entry)) {
		dlist_insert_order(&buf_hdr->region->pool->free_list.regions,
				   ofi_ibufpool_region_is_lower,
				   &buf_hdr->region->entry);
	}
}

static inline size_t ofi_buf_index(void *buf)
{
	return ofi_buf_hdr(buf)->index;
}

static inline void *ofi_bufpool_get_ibuf(struct ofi_bufpool *pool, size_t index)
{
	void *buf;
	size_t region_index = index / pool->attr.chunk_cnt;

	assert(region_index < pool->region_cnt);

	buf = pool->region_table[region_index]->mem_region +
		(index % pool->attr.chunk_cnt) * pool->entry_size;

	assert(ofi_buf_hdr(buf)->allocated);

	return buf;
}

static inline int ofi_bufpool_empty(struct ofi_bufpool *pool)
{
	return slist_empty(&pool->free_list.entries);
}

static inline int ofi_ibufpool_empty(struct ofi_bufpool *pool)
{
	return dlist_empty(&pool->free_list.regions);
}

static inline void *ofi_buf_alloc(struct ofi_bufpool *pool)
{
	struct ofi_bufpool_hdr *buf_hdr;

	assert(!(pool->attr.flags & OFI_BUFPOOL_INDEXED));
	if (ofi_bufpool_empty(pool)) {
		if (ofi_bufpool_grow(pool))
			return NULL;
	}

	slist_remove_head_container(&pool->free_list.entries,
				struct ofi_bufpool_hdr, buf_hdr, entry.slist);
	assert(ofi_atomic_inc32(&buf_hdr->region->use_cnt));
	assert(buf_hdr->allocated == false);

	OFI_DBG_SET(buf_hdr->allocated, true);

	return ofi_buf_data(buf_hdr);
}

static inline void *ofi_buf_alloc_ex(struct ofi_bufpool *pool,
				     void **context)
{
	void *buf = ofi_buf_alloc(pool);

	assert(context);
	if (!buf)
		return NULL;

	*context = ofi_buf_region(buf)->context;
	return buf;
}

static inline void *ofi_ibuf_alloc(struct ofi_bufpool *pool)
{
	struct ofi_bufpool_hdr *buf_hdr;
	struct ofi_bufpool_region *buf_region;

	assert(pool->attr.flags & OFI_BUFPOOL_INDEXED);
	if (ofi_ibufpool_empty(pool)) {
		if (ofi_bufpool_grow(pool))
			return NULL;
	}

	buf_region = container_of(pool->free_list.regions.next,
				  struct ofi_bufpool_region, entry);
	dlist_pop_front(&buf_region->free_list, struct ofi_bufpool_hdr,
			buf_hdr, entry.dlist);
	assert(ofi_atomic_inc32(&buf_hdr->region->use_cnt));
	assert(buf_hdr->allocated == false);

	OFI_DBG_SET(buf_hdr->allocated, true);

	if (dlist_empty(&buf_region->free_list))
		dlist_remove_init(&buf_region->entry);
	return ofi_buf_data(buf_hdr);
}


/*
 * Persistent memory support
 */
void ofi_pmem_init(void);

extern uint64_t OFI_RMA_PMEM;
extern void (*ofi_pmem_commit)(const void *addr, size_t len);


#endif /* _OFI_MEM_H_ */

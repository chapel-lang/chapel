/*
 * Copyright (c) 2011 Intel Corporation.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc .  All rights reserved.
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
 *
 */

#ifndef _OFI_INDEXER_H_
#define _OFI_INDEXER_H_

#include "config.h"

#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

#include <ofi_osd.h>


/*
 * Indexer:
 * The indexer is used to associate a pointer with an integer value.
 * This allows passing the integer to other users, including remote
 * peers or processes.  The integer can be used by the owner of the
 * indexer to retrieve the stored pointer.  The integer value is selected
 * by the indexer by selecting the first available unused value.
 *
 * The pointers are stored using a double-lookup array, which grows
 * dynamically.  This helps conserve memory when only a few objects are
 * stored in the indexer.
 *
 * Synchronization must be provided by the caller.  Caller must
 * initialize the indexer by setting free_list and size to 0.
 */

struct ofi_idx_entry {
	void *item;
	int   next;
};

/* User's index is a bit field of format: [chunk_id:offset] */
#define OFI_IDX_CHUNK_BITS 10
#define OFI_IDX_OFFSET_BITS 10

#define OFI_IDX_CHUNK_SIZE (1 << OFI_IDX_OFFSET_BITS)
#define OFI_IDX_MAX_CHUNKS (1 << OFI_IDX_CHUNK_BITS)

#define OFI_IDX_MAX_INDEX  ((OFI_IDX_MAX_CHUNKS * OFI_IDX_CHUNK_SIZE) - 1)

struct indexer
{
	struct ofi_idx_entry 	*chunk[OFI_IDX_MAX_CHUNKS];
	int		 	free_list;
	/* Array size (used): [0, OFI_IDX_MAX_CHUNKS) */
	int		 	size;
};

#define ofi_idx_chunk_id(index) (index >> OFI_IDX_OFFSET_BITS)
#define ofi_idx_offset(index) (index & (OFI_IDX_CHUNK_SIZE - 1))

int ofi_idx_insert(struct indexer *idx, void *item);
void *ofi_idx_remove(struct indexer *idx, int index);
void *ofi_idx_remove_ordered(struct indexer *idx, int index);
void ofi_idx_replace(struct indexer *idx, int index, void *item);
void ofi_idx_reset(struct indexer *idx);

static inline int ofi_idx_is_valid(struct indexer *idx, int index)
{
	return (index > 0) && (index < idx->size * OFI_IDX_CHUNK_SIZE);
}

static inline struct ofi_idx_entry *
ofi_idx_chunk(struct indexer *idx, int index)
{
	assert(ofi_idx_is_valid(idx, index));
	return idx->chunk[ofi_idx_chunk_id(index)];
}

static inline void *ofi_idx_at(struct indexer *idx, int index)
{
	return (ofi_idx_chunk(idx, index) + ofi_idx_offset(index))->item;
}

static inline void *ofi_idx_lookup(struct indexer *idx, int index)
{
	return ofi_idx_is_valid(idx, index) ? ofi_idx_at(idx, index) : NULL;
}

static inline bool ofi_idx_free_list_empty(struct indexer *idx)
{
	return (idx->free_list == 0);
}


/*
 * Byte Indexer Map:
 * Indexer & map with an index that fits into a byte.  Index 0 is invalid.
 *
 * Synchronization must be provided by the caller.  Caller must
 * initialize structure to 0.
 */

enum ofi_byte_idx_type {
	OFI_BYTE_IDX_UNKNOWN,
	OFI_BYTE_IDX_INDEX,
	OFI_BYTE_IDX_MAP,
};

struct ofi_byte_idx
{
	struct ofi_idx_entry *data;
	uint8_t free_list;
	OFI_DBG_VAR(enum ofi_byte_idx_type, type)
};

bool ofi_byte_idx_grow(struct ofi_byte_idx *idx);

static inline uint8_t ofi_byte_idx_insert(struct ofi_byte_idx *idx, void *item)
{
	uint8_t index;

	assert(idx->type == OFI_BYTE_IDX_UNKNOWN ||
	       idx->type == OFI_BYTE_IDX_INDEX);
	OFI_DBG_SET(idx->type, OFI_BYTE_IDX_INDEX);

	index = idx->free_list;
	if (index == 0) {
		if (!ofi_byte_idx_grow(idx))
			return 0;
		index = idx->free_list;
	}

	idx->free_list = (uint8_t) idx->data[index].next;
	idx->data[index].item = item;
	idx->data[index].next = -1;
	return index;
}

static inline void *ofi_byte_idx_remove(struct ofi_byte_idx *idx, uint8_t index)
{
	void *item;

	assert(idx->type == OFI_BYTE_IDX_INDEX);

	if (idx->data[index].next != -1)
		return NULL;

	item = idx->data[index].item;
	idx->data[index].item = NULL;
	idx->data[index].next = idx->free_list;
	idx->free_list = index;
	return item;
}

static inline void *ofi_byte_idx_at(struct ofi_byte_idx *idx, uint8_t index)
{
	return idx->data[index].item;
}

static inline void *ofi_byte_idx_lookup(struct ofi_byte_idx *idx, uint8_t index)
{
	return (idx->data && idx->data[index].next == -1) ?
		idx->data[index].item : NULL;
}

static inline uint8_t
ofi_byte_idx_set(struct ofi_byte_idx *idx, uint8_t index, void *item)
{
	assert(idx->type == OFI_BYTE_IDX_UNKNOWN ||
	       idx->type == OFI_BYTE_IDX_MAP);
	OFI_DBG_SET(idx->type, OFI_BYTE_IDX_MAP);

	if (!idx->data && !ofi_byte_idx_grow(idx))
		return 0;

	assert(idx->data[index].next != -1);
	idx->data[index].item = item;
	idx->data[index].next = -1;
	return index;
}

static inline void *
ofi_byte_idx_clear(struct ofi_byte_idx *idx, uint8_t index)
{
	void *item;

	assert(idx->type == OFI_BYTE_IDX_MAP);
	assert(idx->data);
	assert(idx->data[index].next == -1);

	item = idx->data[index].item;
	idx->data[index].item = NULL;
	idx->data[index].next = 0;
	return item;
}


/*
 * Index map:
 * The index map is similar in concept to the indexer.  It allows the user
 * to associate an integer with a pointer.  The difference between the index
 * map and indexer, is that the user of the index map selects the index.  This
 * results in the index map behaving the same as a standard array.
 *
 * The index map stores pointers using a double-lookup table.  This minimizes
 * the memory footprint relative to using a standard array when the selected
 * integer values are sparse.
 *
 * Synchronization must be provided by the caller.  Caller must initialize
 * the index map by setting it to 0.
 */

struct index_map
{
	void **chunk[OFI_IDX_MAX_CHUNKS];
	int count[OFI_IDX_MAX_CHUNKS];
};

int ofi_idm_set(struct index_map *idm, int index, void *item);
void *ofi_idm_clear(struct index_map *idm, int index);
void ofi_idm_reset(struct index_map *idm, void (*callback)(void *item));

static inline void **ofi_idm_chunk(struct index_map *idm, int index)
{
	assert(idm->chunk);
	return idm->chunk[ofi_idx_chunk_id(index)];
}

static inline void *ofi_idm_at(struct index_map *idm, int index)
{
	void **chunk;
	chunk = ofi_idm_chunk(idm, index);
	assert(chunk && idm->count[ofi_idx_chunk_id(index)]);
	return chunk[ofi_idx_offset(index)];
}

static inline void *ofi_idm_lookup(struct index_map *idm, int index)
{
	return ((index <= OFI_IDX_MAX_INDEX) && ofi_idm_chunk(idm, index)) ?
		ofi_idm_at(idm, index) : NULL;
}


struct ofi_dyn_arr
{
	char *chunk[OFI_IDX_MAX_CHUNKS];
	size_t item_size;
	void (*init)(struct ofi_dyn_arr *arr, void *item);
};

static inline void
ofi_array_init(struct ofi_dyn_arr *arr, size_t item_size,
	       void (*init)(struct ofi_dyn_arr *arr, void *item))
{
	memset(arr, 0, sizeof(*arr));
	arr->item_size = item_size;
	arr->init = init;
}

int ofi_array_grow(struct ofi_dyn_arr *arr, int index);
/* Returning non-zero from callback breaks iteration */
int ofi_array_iter(struct ofi_dyn_arr *arr, void *context,
		   int (*callback)(struct ofi_dyn_arr *arr, void *item,
				   void *context));
void ofi_array_destroy(struct ofi_dyn_arr *arr);

static inline char *ofi_array_chunk(struct ofi_dyn_arr *arr, int index)
{
	assert(arr->chunk);
	return arr->chunk[ofi_idx_chunk_id(index)];
}

static inline void *
ofi_array_item(struct ofi_dyn_arr *arr, char *chunk, int offset)
{
	return chunk + arr->item_size * offset;
}

static inline void *ofi_array_at(struct ofi_dyn_arr *arr, int index)
{
	assert(index <= OFI_IDX_MAX_INDEX);

	if (!ofi_array_chunk(arr, index)) {
		if (ofi_array_grow(arr, index) < 0)
			return NULL;
	}

	return ofi_array_item(arr, ofi_array_chunk(arr, index),
			      ofi_idx_offset(index));
}

#endif /* _OFI_INDEXER_H_ */

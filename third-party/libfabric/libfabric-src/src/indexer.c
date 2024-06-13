/*
 * Copyright (c) 2011 Intel Corporation.  All rights reserved.
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

#include "config.h"

#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <ofi_indexer.h>

/*
 * Indexer - to find a structure given an index
 *
 * We store pointers using a double lookup and return an index to the
 * user which is then used to retrieve the pointer.  The upper bits of
 * the index are itself an index into an array of memory allocations.
 * The lower bits specify the offset into the allocated memory where
 * the pointer is stored.
 *
 * This allows us to adjust the number of pointers stored by the index
 * list without taking a lock during data lookups.
 */

static int ofi_idx_grow(struct indexer *idx)
{
	struct ofi_idx_entry *chunk;
	int i, start_index;

	if (idx->size >= OFI_IDX_MAX_CHUNKS)
		goto nomem;

	idx->chunk[idx->size] = calloc(OFI_IDX_CHUNK_SIZE, sizeof(struct ofi_idx_entry));
	if (!idx->chunk[idx->size])
		goto nomem;

	chunk = idx->chunk[idx->size];
	start_index = idx->size * OFI_IDX_CHUNK_SIZE;
	chunk[OFI_IDX_CHUNK_SIZE - 1].next = idx->free_list;

	for (i = OFI_IDX_CHUNK_SIZE - 2; i >= 0; i--)
		chunk[i].next = start_index + i + 1;

	/* Index 0 is reserved */
	if (start_index == 0)
		start_index++;
	idx->free_list = start_index;
	idx->size++;
	return start_index;

nomem:
	errno = ENOMEM;
	return -1;
}

int ofi_idx_insert(struct indexer *idx, void *item)
{
	struct ofi_idx_entry *chunk;
	int index;

	if ((index = idx->free_list) == 0) {
		if ((index = ofi_idx_grow(idx)) <= 0)
			return index;
	}

	chunk = ofi_idx_chunk(idx, index);
	idx->free_list = chunk[ofi_idx_offset(index)].next;
	chunk[ofi_idx_offset(index)].item = item;
	return index;
}

void *ofi_idx_remove(struct indexer *idx, int index)
{
	struct ofi_idx_entry *chunk;
	void *item;
	int offset = ofi_idx_offset(index);

	chunk = ofi_idx_chunk(idx, index);
	item = chunk[offset].item;
	chunk[offset].item = NULL;
	chunk[offset].next = idx->free_list;
	idx->free_list = index;
	return item;
}

void *ofi_idx_remove_ordered(struct indexer *idx, int index)
{
	struct ofi_idx_entry *chunk;
	void *item;
	int temp_index;
	int offset = ofi_idx_offset(index);

	chunk = ofi_idx_chunk(idx, index);
	item = chunk[offset].item;
	chunk[offset].item = NULL;
	if (ofi_idx_free_list_empty(idx) || index < idx->free_list) {
		chunk[offset].next = idx->free_list;
		idx->free_list = index;
		return item;
	}
	temp_index = idx->free_list;
	while (chunk[ofi_idx_offset(temp_index)].next < index) {
		temp_index = chunk[ofi_idx_offset(temp_index)].next;
	}
	chunk[offset].next = chunk[ofi_idx_offset(temp_index)].next;
	chunk[ofi_idx_offset(temp_index)].next = index;

	return item;
}

void ofi_idx_replace(struct indexer *idx, int index, void *item)
{
	struct ofi_idx_entry *chunk;

	chunk = ofi_idx_chunk(idx, index);
	chunk[ofi_idx_offset(index)].item = item;
}

void ofi_idx_reset(struct indexer *idx)
{
	while (idx->size) {
		free(idx->chunk[idx->size - 1]);
		idx->chunk[idx->size - 1] = NULL;
		idx->size--;
	}
	idx->free_list = 0;
}


bool ofi_byte_idx_grow(struct ofi_byte_idx *idx)
{
	uint8_t i;

	if (idx->data)
		return false;

	idx->data = calloc(UINT8_MAX + 1, sizeof(*idx->data));
	if (!idx->data)
		return false;

	/* index 0 is reserved/invalid, 0 marks end of list */
	for (i = 1; i < UINT8_MAX - 1; i++)
		idx->data[i].next = i + 1;
	idx->free_list = 1;
	return true;
}


static int ofi_idm_grow(struct index_map *idm, int index)
{
	assert(!ofi_idm_chunk(idm, index));
	idm->chunk[ofi_idx_chunk_id(index)] = calloc(OFI_IDX_CHUNK_SIZE,
						     sizeof(void *));
	if (!idm->chunk[ofi_idx_chunk_id(index)])
		goto nomem;

	return index;

nomem:
	errno = ENOMEM;
	return -1;
}

int ofi_idm_set(struct index_map *idm, int index, void *item)
{
	void **chunk;

	if (index > OFI_IDX_MAX_INDEX) {
		errno = ENOMEM;
		return -1;
	}

	if (!ofi_idm_chunk(idm, index)) {
		if (ofi_idm_grow(idm, index) < 0)
			return -1;
	}

	chunk = ofi_idm_chunk(idm, index);
	chunk[ofi_idx_offset(index)] = item;
	idm->count[ofi_idx_chunk_id(index)]++;
	return index;
}

void *ofi_idm_clear(struct index_map *idm, int index)
{
	void **chunk;
	void *item;

	chunk = ofi_idm_chunk(idm, index);
	item = chunk[ofi_idx_offset(index)];
	chunk[ofi_idx_offset(index)] = NULL;
	if (--idm->count[ofi_idx_chunk_id(index)] == 0) {
		free(chunk);
		idm->chunk[ofi_idx_chunk_id(index)] = NULL;
	}
	return item;
}

void ofi_idm_reset(struct index_map *idm, void (*callback)(void *item))
{
	void **chunk;
	void *item;
	int c, i;

	for (c = 0; c < OFI_IDX_MAX_CHUNKS; c++) {
		if (!idm->chunk[c]) {
			assert(idm->count[c] == 0);
			continue;
		}

		chunk = idm->chunk[c];
		for (i = 0; idm->count[c] && i < OFI_IDX_CHUNK_SIZE; i++) {
			item = chunk[i];
			if (item) {
				if (callback)
					callback(item);
				idm->count[c]--;
			}
		}
		free(chunk);
		idm->chunk[c] = NULL;
	}
}


int ofi_array_grow(struct ofi_dyn_arr *arr, int index)
{
	int c, i;

	assert(arr->item_size);
	assert(!ofi_array_chunk(arr, index));

	c = ofi_idx_chunk_id(index);
	arr->chunk[c] = calloc(OFI_IDX_CHUNK_SIZE, arr->item_size);
	if (!arr->chunk[c])
		goto nomem;

	if (arr->init) {
		for (i = 0; i < OFI_IDX_CHUNK_SIZE; i++)
			arr->init(arr, ofi_array_item(arr, arr->chunk[c], i));
	}

	return index;

nomem:
	errno = ENOMEM;
	return -1;
}

int ofi_array_iter(struct ofi_dyn_arr *arr, void *context,
		    int (*callback)(struct ofi_dyn_arr *arr, void *item,
				    void *context))
{
	int ret, c, i;

	for (c = 0; c < OFI_IDX_MAX_CHUNKS; c++) {
		if (!arr->chunk[c])
			continue;

		for (i = 0; i < OFI_IDX_CHUNK_SIZE; i++) {
			ret = callback(arr, ofi_array_item(arr, arr->chunk[c], i),
				       context);
			if (ret)
				return ret;
		}
	}
	return 0;
}

void ofi_array_destroy(struct ofi_dyn_arr *arr)
{
	int c;

	for (c = 0; c < OFI_IDX_MAX_CHUNKS; c++) {
		if (!arr->chunk[c])
			continue;

		free(arr->chunk[c]);
		arr->chunk[c] = NULL;
	}
}

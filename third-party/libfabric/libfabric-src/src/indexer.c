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
	struct ofi_idx_entry *entry;
	int i, start_index;

	if (idx->size >= OFI_IDX_ARRAY_SIZE)
		goto nomem;

	idx->array[idx->size] = calloc(OFI_IDX_ENTRY_SIZE, sizeof(struct ofi_idx_entry));
	if (!idx->array[idx->size])
		goto nomem;

	entry = idx->array[idx->size];
	start_index = idx->size << OFI_IDX_ENTRY_BITS;
	entry[OFI_IDX_ENTRY_SIZE - 1].next = idx->free_list;

	for (i = OFI_IDX_ENTRY_SIZE - 2; i >= 0; i--)
		entry[i].next = start_index + i + 1;

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
	struct ofi_idx_entry *entry;
	int index;

	if ((index = idx->free_list) == 0) {
		if ((index = ofi_idx_grow(idx)) <= 0)
			return index;
	}

	entry = idx->array[ofi_idx_array_index(index)];
	idx->free_list = entry[ofi_idx_entry_index(index)].next;
	entry[ofi_idx_entry_index(index)].item = item;
	return index;
}

void *ofi_idx_remove(struct indexer *idx, int index)
{
	struct ofi_idx_entry *entry;
	void *item;
	int entry_index = ofi_idx_entry_index(index);

	entry = idx->array[ofi_idx_array_index(index)];
	item = entry[entry_index].item;
	entry[entry_index].item = NULL;
	entry[entry_index].next = idx->free_list;
	idx->free_list = index;
	return item;
}

void ofi_idx_replace(struct indexer *idx, int index, void *item)
{
	struct ofi_idx_entry *entry;

	entry = idx->array[ofi_idx_array_index(index)];
	entry[ofi_idx_entry_index(index)].item = item;
}

void ofi_idx_reset(struct indexer *idx)
{
	while (idx->size) {
		free(idx->array[idx->size - 1]);
		idx->array[idx->size - 1] = NULL;
		idx->size--;
	}
	idx->free_list = 0;
}

static int ofi_idm_grow(struct index_map *idm, int index)
{
	idm->array[ofi_idx_array_index(index)] = calloc(OFI_IDX_ENTRY_SIZE, sizeof(void *));
	if (!idm->array[ofi_idx_array_index(index)])
		goto nomem;

	return index;

nomem:
	errno = ENOMEM;
	return -1;
}

int ofi_idm_set(struct index_map *idm, int index, void *item)
{
	void **entry;

	if (index > OFI_IDX_MAX_INDEX) {
		errno = ENOMEM;
		return -1;
	}

	if (!idm->array[ofi_idx_array_index(index)]) {
		if (ofi_idm_grow(idm, index) < 0)
			return -1;
	}

	entry = idm->array[ofi_idx_array_index(index)];
	entry[ofi_idx_entry_index(index)] = item;
	idm->count[ofi_idx_array_index(index)]++;
	return index;
}

void *ofi_idm_clear(struct index_map *idm, int index)
{
	void **entry;
	void *item;

	entry = idm->array[ofi_idx_array_index(index)];
	item = entry[ofi_idx_entry_index(index)];
	entry[ofi_idx_entry_index(index)] = NULL;
	if (--idm->count[ofi_idx_array_index(index)] == 0) {
		free(idm->array[ofi_idx_array_index(index)]);
		idm->array[ofi_idx_array_index(index)] = NULL;
	}
	return item;
}

void ofi_idm_reset(struct index_map *idm)
{
	int i;

	for (i=0; i<OFI_IDX_ARRAY_SIZE; i++) {
		if (idm->array[i]) {
			free(idm->array[i]);
			idm->array[i] = NULL;
			idm->count[i] = 0;
		}
	}
}


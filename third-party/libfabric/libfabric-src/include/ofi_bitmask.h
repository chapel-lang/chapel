/*
 * Copyright (c) 2019-2019 Intel Corporation. All rights reserved.
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

#ifndef _OFI_BITMASK_H_
#define _OFI_BITMASK_H_

#include <rdma/fi_errno.h>

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct bitmask {
	size_t size;
	uint8_t *bytes;
};

static inline int ofi_bitmask_create(struct bitmask *mask, size_t size)
{
	size_t byte_size = size / 8;
	if (byte_size % 8)
		byte_size++;

	mask->bytes = calloc(byte_size, 1);
	if (!mask->bytes)
		return -FI_ENOMEM;

	mask->size = size;

	return FI_SUCCESS;
};

static inline void ofi_bitmask_free(struct bitmask *mask)
{
	free(mask->bytes);
	mask->bytes = NULL;
};

static inline size_t ofi_bitmask_bytesize(struct bitmask *mask)
{
	return (mask->size % 8) ? (mask->size / 8 + 1) : (mask->size / 8);
};

static inline void ofi_bitmask_unset(struct bitmask *mask, size_t idx)
{
	assert(idx <= mask->size);
	mask->bytes[idx / 8] &= ~(0x01 << (idx % 8));
};

static inline void ofi_bitmask_set(struct bitmask *mask, size_t idx)
{
	assert(idx <= mask->size);
	mask->bytes[idx / 8] |= (0x01 << (idx % 8));
};

static inline void ofi_bitmask_set_all(struct bitmask *mask)
{
	memset(mask->bytes, 0xff, ofi_bitmask_bytesize(mask));
};

static inline size_t ofi_bitmask_get_lsbset(struct bitmask mask)
{
	int cur;
	uint8_t tmp;
	size_t ret = 0;

	for (cur = 0; cur < (mask.size/8); cur++) {
		if (mask.bytes[cur]) {
			tmp = mask.bytes[cur];
			while (!(tmp & 0x1)) {
				tmp >>= 1;
				ret++;
			}
			break;
		} else {
			ret += 8;
		}
	}

	assert(ret <= (mask.size));
	return ret;
};

#endif

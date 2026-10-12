/*
 * Copyright (C) 2025 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_HFISVC_KEYSET_H_
#define _FI_PROV_OPX_HFISVC_KEYSET_H_

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include "rdma/opx/opx_hfisvc.h"
#include "rdma/opx/fi_opx_compiler.h"
#include "rdma/opx/fi_opx_debug_counters.h"

typedef uintptr_t opx_hfisvc_keyset_t;
typedef uint32_t  opx_hfisvc_key_t;

#define OPX_HFISVC_KEYSET_SHIFT		  (10)
#define OPX_HFISVC_KEYSET_SIZE		  (1 << OPX_HFISVC_KEYSET_SHIFT)
#define OPX_HFISVC_KEYSET_BITMAP_SIZE_QWS (OPX_HFISVC_KEYSET_SIZE >> 6)
#define OPX_HFISVC_KEY_INDEX_MASK	  ((1 << OPX_HFISVC_KEYSET_SHIFT) - 1)

/**
 * Key set used for vending keys to use with HFI service requests.
 * This uses a 128-byte bitmap to vend keys with values ranging from
 * 0-1023.
 */
struct opx_hfisvc_keyset {
	struct opx_hfisvc_keyset *next;
	uint32_t		  base_val;
	int32_t			  num_free;
	uint64_t		  bitmap[OPX_HFISVC_KEYSET_BITMAP_SIZE_QWS];
} __attribute__((aligned(32)));

/**
 * Initialize the keyset
 */
__OPX_FORCE_INLINE__
int opx_hfisvc_keyset_init(uint32_t base_val, opx_hfisvc_keyset_t *keyset)
{
	struct opx_hfisvc_keyset *new_keyset;

	if (posix_memalign((void **) &new_keyset, 32, sizeof(struct opx_hfisvc_keyset))) {
		return -ENOMEM;
	}

	new_keyset->next     = NULL;
	new_keyset->base_val = base_val;
	new_keyset->num_free = OPX_HFISVC_KEYSET_SIZE;

	for (int i = 0; i < OPX_HFISVC_KEYSET_BITMAP_SIZE_QWS; ++i) {
		new_keyset->bitmap[i] = 0ul;
	}

	*keyset = (opx_hfisvc_keyset_t) new_keyset;

	return 0;
}

/**
 * Allocate the next available key from the keyset.
 *
 * @return 0 if the key was allocated successfully, or -ENOSPC if no keys are available.
 *
 */
__OPX_FORCE_INLINE__
int opx_hfisvc_keyset_alloc_key(opx_hfisvc_keyset_t keyset, opx_hfisvc_key_t *key,
				struct fi_opx_debug_counters *counters)
{
	struct opx_hfisvc_keyset *_keyset = (struct opx_hfisvc_keyset *) keyset;
	assert(_keyset);
	assert(key);

	if (_keyset->num_free < 1) {
		goto alloc_end;
	}

	for (int i = 0; i < OPX_HFISVC_KEYSET_BITMAP_SIZE_QWS; i++) {
		uint64_t inv = ~_keyset->bitmap[i];
		if (inv) {
			uint64_t bit_index = __builtin_ctzl(inv);
			*key		   = (_keyset->base_val << OPX_HFISVC_KEYSET_SHIFT) | (i * 64ul + bit_index);
			_keyset->bitmap[i] |= (1ul << bit_index);
			_keyset->num_free--;

			FI_OPX_DEBUG_COUNTERS_INC(counters->hfisvc.access_key.alloc);

			return 0;
		}
	}

	// TODO: Add FI_WARN about num_free being non-zero when zero keys are available.

alloc_end:
	FI_OPX_DEBUG_COUNTERS_INC(counters->hfisvc.access_key.alloc_enospc);
	return -ENOSPC;
}

/**
 * Free/return a previously allocated key to the keyset so it can be reused.
 */
__OPX_FORCE_INLINE__
void opx_hfisvc_keyset_free_key(opx_hfisvc_keyset_t keyset, opx_hfisvc_key_t key,
				struct fi_opx_debug_counters *counters)
{
	FI_OPX_DEBUG_COUNTERS_INC(counters->hfisvc.access_key.free);

	struct opx_hfisvc_keyset *_keyset = (struct opx_hfisvc_keyset *) keyset;
	assert(_keyset);

	// Assert the key belongs to this keyset
	assert(((key >> OPX_HFISVC_KEYSET_SHIFT) & UINT32_MAX) == _keyset->base_val);

	uint64_t key_index = key & OPX_HFISVC_KEY_INDEX_MASK;

	OPX_HFISVC_DEBUG_LOG("Freeing key %u, key_index=%016lX, _keyset->bitmap[%lX]=%016lX\n", key, key_index,
			     key_index >> 6, _keyset->bitmap[key_index >> 6]);

	// Assert that the key being freed is currently marked as being used.
	assert(_keyset->bitmap[key_index >> 6] & (1ul << (key_index & 0x3Ful)));

	_keyset->bitmap[key_index >> 6] &= ~(1ul << (key_index & 0x3Ful));
	_keyset->num_free++;
}

/**
 * Free a keyset.
 */
__OPX_FORCE_INLINE__
void opx_hfisvc_keyset_free(opx_hfisvc_keyset_t keyset)
{
	struct opx_hfisvc_keyset *_keyset = (struct opx_hfisvc_keyset *) keyset;
	assert(_keyset);
	assert(_keyset->num_free == OPX_HFISVC_KEYSET_SIZE);
	free(_keyset);
}

#endif

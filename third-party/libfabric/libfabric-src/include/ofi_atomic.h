/*
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
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

#ifndef _OFI_ATOMIC_H_
#define _OFI_ATOMIC_H_

#include "config.h"

#include "ofi.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t ofi_datatype_size(enum fi_datatype datatype);

/* The START value is included, LAST is exclusive, which matches the public
 * header file use of LAST.  CNT is the number of valid values.
 */
#define OFI_WRITE_OP_START	FI_MIN
#define OFI_WRITE_OP_LAST	(FI_ATOMIC_WRITE + 1)
#define OFI_WRITE_OP_CNT	(OFI_WRITE_OP_LAST - OFI_WRITE_OP_START)
#define OFI_READWRITE_OP_START	FI_MIN
#define OFI_READWRITE_OP_LAST	(FI_ATOMIC_WRITE + 1)
#define OFI_READWRITE_OP_CNT	(OFI_READWRITE_OP_LAST - OFI_READWRITE_OP_START)
#define OFI_SWAP_OP_START	FI_CSWAP
#define OFI_SWAP_OP_LAST	(FI_MSWAP + 1)
#define OFI_SWAP_OP_CNT		(OFI_SWAP_OP_LAST - OFI_SWAP_OP_START)

#define ofi_atomic_iswrite_op(op) \
	(op >= OFI_WRITE_OP_START && op < OFI_WRITE_OP_LAST && op != FI_ATOMIC_READ)
#define ofi_atomic_isreadwrite_op(op) \
	(op >= OFI_READWRITE_OP_START && op < OFI_READWRITE_OP_LAST)
#define ofi_atomic_isswap_op(op) \
	(op >= OFI_SWAP_OP_START && op < OFI_SWAP_OP_LAST)

#define OFI_DATATYPE_CNT	(FI_UINT128 + 1)

#ifdef HAVE___INT128
typedef __int128 ofi_int128_t;
typedef unsigned __int128 ofi_uint128_t;
#endif

extern void (*ofi_atomic_write_handlers[OFI_WRITE_OP_CNT][OFI_DATATYPE_CNT])
			(void *dst, const void *src, size_t cnt);
extern void (*ofi_atomic_readwrite_handlers[OFI_READWRITE_OP_CNT][OFI_DATATYPE_CNT])
			(void *dst, const void *src, void *res, size_t cnt);
extern void (*ofi_atomic_swap_handlers[OFI_SWAP_OP_CNT][OFI_DATATYPE_CNT])
			(void *dst, const void *src, const void *cmp,
			 void *res, size_t cnt);

#define ofi_atomic_write_handler(op, datatype, dst, src, cnt) \
	ofi_atomic_write_handlers[op][datatype](dst, src, cnt)
#define ofi_atomic_readwrite_handler(op, datatype, dst, src, res, cnt) \
	ofi_atomic_readwrite_handlers[op][datatype](dst, src, res, cnt)
#define ofi_atomic_swap_handler(op, datatype, dst, src, cmp, res, cnt) \
	ofi_atomic_swap_handlers[op - OFI_SWAP_OP_START][datatype](dst, src, \
								cmp, res, cnt)

int ofi_atomic_valid(const struct fi_provider *prov,
		     enum fi_datatype datatype, enum fi_op op, uint64_t flags);


#ifdef __cplusplus
}
#endif

#endif /* _OFI_ATOMIC_H_ */

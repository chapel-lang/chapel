/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021 Cornelis Networks.
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
#ifndef _FI_OPX_DIRECT_ATOMIC_DEF_H_
#define _FI_OPX_DIRECT_ATOMIC_DEF_H_

#ifdef FABRIC_DIRECT
#define FABRIC_DIRECT_ATOMIC_DEF 1

enum fi_datatype {
	FI_INT8,			/*  0 */
	FI_UINT8,			/*  1 */
	FI_INT16,			/*  2 */
	FI_UINT16,			/*  3 */
	FI_INT32,			/*  4 */
	FI_UINT32,			/*  5 */
	FI_INT64,			/*  7 */
	FI_UINT64,			/*  8 */
	FI_FLOAT,			/*  6 */
	FI_DOUBLE,			/*  9 */
	FI_FLOAT_COMPLEX,		/* 10 */
	FI_DOUBLE_COMPLEX,		/* 11 */
	FI_LONG_DOUBLE,			/* 12 */
	FI_LONG_DOUBLE_COMPLEX,		/* 13 */
	FI_DATATYPE_LAST		/* 14 */
};
enum fi_op {
	FI_MIN,
	FI_MAX,
	FI_SUM,
	FI_PROD,
	FI_LOR,
	FI_LAND,
	FI_BOR,
	FI_BAND,
	FI_LXOR,
	FI_BXOR,
	FI_ATOMIC_READ,
	FI_ATOMIC_WRITE,
	FI_CSWAP,
	FI_CSWAP_NE,
	FI_CSWAP_LE,
	FI_CSWAP_LT,
	FI_CSWAP_GE,
	FI_CSWAP_GT,
	FI_MSWAP,
	FI_ATOMIC_OP_LAST
};
#endif


#endif /* _FI_OPX_DIRECT_ATOMIC_DEF_H_ */

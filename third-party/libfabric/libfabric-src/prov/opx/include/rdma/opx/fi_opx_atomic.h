/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2023 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_ATOMIC_H_
#define _FI_PROV_OPX_ATOMIC_H_

#include "rdma/opx/fi_opx.h"

#include "rdma/opx/fi_opx_compiler.h"
#include "rdma/opx/fi_opx_hfi1_packet.h"
#include "rdma/opx/fi_opx_reliability.h"
/* Macro indirection in order to support other macros as arguments
 * C requires another indirection for expanding macros since
 * operands of the token pasting operator are not expanded */

#define FI_OPX_ATOMIC_SPECIALIZED_FUNC(LOCK, AV, CAPS, RELIABILITY)                                \
	FI_OPX_ATOMIC_SPECIALIZED_FUNC_(LOCK, AV, CAPS, RELIABILITY)

#define FI_OPX_ATOMIC_SPECIALIZED_FUNC_(LOCK, AV, CAPS, RELIABILITY)                               \
	static inline ssize_t fi_opx_atomic_##LOCK##_##AV##_##CAPS##_##RELIABILITY(                \
		struct fid_ep *ep, const void *buf, size_t count, void *desc, fi_addr_t dst_addr,  \
		uint64_t addr, uint64_t key, enum fi_datatype datatype, enum fi_op op,             \
		void *context)                                                                     \
	{                                                                                          \
		return fi_opx_atomic_generic(ep, buf, count, dst_addr, addr, key, datatype, op,    \
					     context, LOCK, AV, CAPS, RELIABILITY);                \
	}                                                                                          \
	static inline ssize_t fi_opx_inject_atomic_##LOCK##_##AV##_##CAPS##_##RELIABILITY(         \
		struct fid_ep *ep, const void *buf, size_t count, fi_addr_t dst_addr,              \
		uint64_t addr, uint64_t key, enum fi_datatype datatype, enum fi_op op)             \
	{                                                                                          \
		return fi_opx_inject_atomic_generic(ep, buf, count, dst_addr, addr, key, datatype, \
						    op, LOCK, AV, CAPS, RELIABILITY);              \
	}                                                                                          \
	static inline ssize_t fi_opx_fetch_atomic_##LOCK##_##AV##_##CAPS##_##RELIABILITY(          \
		struct fid_ep *ep, const void *buf, size_t count, void *desc, void *result,        \
		void *result_desc, fi_addr_t dest_addr, uint64_t addr, uint64_t key,               \
		enum fi_datatype datatype, enum fi_op op, void *context)                           \
	{                                                                                          \
		return fi_opx_fetch_atomic_generic(ep, buf, count, desc, result, result_desc,      \
						   dest_addr, addr, key, datatype, op, context,    \
						   LOCK, AV, CAPS, RELIABILITY);                   \
	}                                                                                          \
	static inline ssize_t fi_opx_compare_atomic_##LOCK##_##AV##_##CAPS##_##RELIABILITY(        \
		struct fid_ep *ep, const void *buf, size_t count, void *desc, const void *compare, \
		void *compare_desc, void *result, void *result_desc, fi_addr_t dest_addr,          \
		uint64_t addr, uint64_t key, enum fi_datatype datatype, enum fi_op op,             \
		void *context)                                                                     \
	{                                                                                          \
		return fi_opx_compare_atomic_generic(ep, buf, count, desc, compare, compare_desc,  \
						     result, result_desc, dest_addr, addr, key,    \
						     datatype, op, context, LOCK, AV, CAPS,        \
						     RELIABILITY);                                 \
	}

#define FI_OPX_ATOMIC_SPECIALIZED_FUNC_NAME(TYPE, LOCK, AV, CAPS, RELIABILITY)                     \
	FI_OPX_ATOMIC_SPECIALIZED_FUNC_NAME_(TYPE, LOCK, AV, CAPS, RELIABILITY)

#define FI_OPX_ATOMIC_SPECIALIZED_FUNC_NAME_(TYPE, LOCK, AV, CAPS, RELIABILITY)                    \
	fi_opx_##TYPE##_##LOCK##_##AV##_##CAPS##_##RELIABILITY

#ifdef __cplusplus
extern "C" {
#endif

#include <complex.h>

static inline size_t sizeofdt(const enum fi_datatype datatype)
{
	static const size_t sizeofdt[FI_DATATYPE_LAST] = {
		sizeof(int8_t), /* FI_INT8 */
		sizeof(uint8_t), /* FI_UINT8 */
		sizeof(int16_t), /* FI_INT16 */
		sizeof(uint16_t), /* FI_UINT16 */
		sizeof(int32_t), /* FI_INT32 */
		sizeof(uint32_t), /* FI_UINT32 */
		sizeof(int64_t), /* FI_INT64 */
		sizeof(uint64_t), /* FI_UINT64 */
		sizeof(float), /* FI_FLOAT */
		sizeof(double), /* FI_DOUBLE */
		sizeof(complex float), /* FI_FLOAT_COMPLEX */
		sizeof(complex double), /* FI_DOUBLE_COMPLEX */
		sizeof(long double), /* FI_LONG_DOUBLE */
		sizeof(complex long double) /* FI_LONG_DOUBLE_COMPLEX */
	};

	return sizeofdt[datatype];
}

ssize_t fi_opx_fetch_atomic_generic(struct fid_ep *ep, const void *buf, size_t count, void *desc,
				    void *result, void *result_desc, fi_addr_t dest_addr,
				    uint64_t addr, uint64_t key, enum fi_datatype datatype,
				    enum fi_op op, void *context, const int lock_required,
				    const enum fi_av_type av_type, const uint64_t caps,
				    const enum ofi_reliability_kind reliability);

ssize_t fi_opx_compare_atomic_generic(struct fid_ep *ep, const void *buf, size_t count, void *desc,
				      const void *compare, void *compare_desc, void *result,
				      void *result_desc, fi_addr_t dest_addr, uint64_t addr,
				      uint64_t key, enum fi_datatype datatype, enum fi_op op,
				      void *context, int lock_required,
				      const enum fi_av_type av_type, const uint64_t caps,
				      const enum ofi_reliability_kind reliability);

ssize_t fi_opx_inject_atomic_generic(struct fid_ep *ep, const void *buf, size_t count,
				     fi_addr_t dest_addr, uint64_t addr, uint64_t key,
				     enum fi_datatype datatype, enum fi_op op, int lock_required,
				     const enum fi_av_type av_type, const uint64_t caps,
				     const enum ofi_reliability_kind reliability);

#ifdef __cplusplus
}
#endif

#endif /* _FI_PROV_OPX_ATOMIC_H_ */

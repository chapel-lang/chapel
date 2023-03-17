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

#ifndef _FI_PROV_OPX_RMA_OPS_H_
#define _FI_PROV_OPX_RMA_OPS_H_

/* Macro indirection in order to support other macros as arguments
 * C requires another indirection for expanding macros since
 * operands of the token pasting operator are not expanded */

#define FI_OPX_RMA_SPECIALIZED_FUNC(LOCK, AV, CAPS, RELIABILITY)                                   \
	FI_OPX_RMA_SPECIALIZED_FUNC_(LOCK, AV, CAPS, RELIABILITY)

#define FI_OPX_RMA_SPECIALIZED_FUNC_(LOCK, AV, CAPS, RELIABILITY)                                  \
	static inline ssize_t fi_opx_writemsg_##LOCK##_##AV##_##CAPS##_##RELIABILITY(              \
		struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)                   \
	{                                                                                          \
		return fi_opx_writemsg_generic(ep, msg, flags, LOCK, AV, CAPS, RELIABILITY);       \
	}                                                                                          \
	static inline ssize_t fi_opx_writev_##LOCK##_##AV##_##CAPS##_##RELIABILITY(                \
		struct fid_ep *ep, const struct iovec *iov, void **desc, size_t count,             \
		fi_addr_t dest_addr, uint64_t addr_offset, uint64_t key, void *context)            \
	{                                                                                          \
		return fi_opx_writev_generic(ep, iov, desc, count, dest_addr, addr_offset, key,    \
					     context, LOCK, AV, CAPS, RELIABILITY);                \
	}                                                                                          \
	static inline ssize_t fi_opx_write_##LOCK##_##AV##_##CAPS##_##RELIABILITY(                 \
		struct fid_ep *ep, const void *buf, size_t len, void *desc, fi_addr_t dst_addr,    \
		uint64_t addr_offset, uint64_t key, void *context)                                 \
	{                                                                                          \
		return fi_opx_write_generic(ep, buf, len, desc, dst_addr, addr_offset, key,        \
					    context, LOCK, AV, CAPS, RELIABILITY);                 \
	}                                                                                          \
	static inline ssize_t fi_opx_inject_write_##LOCK##_##AV##_##CAPS##_##RELIABILITY(          \
		struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dst_addr,                \
		uint64_t addr_offset, uint64_t key)                                                \
	{                                                                                          \
		return fi_opx_inject_write_generic(ep, buf, len, dst_addr, addr_offset, key, LOCK, \
						   AV, CAPS, RELIABILITY);                         \
	}                                                                                          \
	static inline ssize_t fi_opx_readmsg_##LOCK##_##AV##_##CAPS##_##RELIABILITY(               \
		struct fid_ep *ep, const struct fi_msg_rma *msg, uint64_t flags)                   \
	{                                                                                          \
		return fi_opx_readmsg_generic(ep, msg, flags, LOCK, AV, CAPS, RELIABILITY);        \
	}                                                                                          \
	static inline ssize_t fi_opx_readv_##LOCK##_##AV##_##CAPS##_##RELIABILITY(                 \
		struct fid_ep *ep, const struct iovec *iov, void **desc, size_t count,             \
		fi_addr_t src_addr, uint64_t addr_offset, uint64_t key, void *context)             \
	{                                                                                          \
		return fi_opx_writev_generic(ep, iov, desc, count, src_addr, addr_offset, key,     \
					     context, LOCK, AV, CAPS, RELIABILITY);                \
	}                                                                                          \
	static inline ssize_t fi_opx_read_##LOCK##_##AV##_##CAPS##_##RELIABILITY(                  \
		struct fid_ep *ep, void *buf, size_t len, void *desc, fi_addr_t src_addr,          \
		uint64_t addr_offset, uint64_t key, void *context)                                 \
	{                                                                                          \
		return fi_opx_read_generic(ep, buf, len, desc, src_addr, addr_offset, key,         \
					   context, LOCK, AV, CAPS, RELIABILITY);                  \
	}

#define FI_OPX_RMA_SPECIALIZED_FUNC_NAME(TYPE, LOCK, AV, CAPS, RELIABILITY)                        \
	FI_OPX_RMA_SPECIALIZED_FUNC_NAME_(TYPE, LOCK, AV, CAPS, RELIABILITY)

#define FI_OPX_RMA_SPECIALIZED_FUNC_NAME_(TYPE, LOCK, AV, CAPS, RELIABILITY)                       \
	fi_opx_##TYPE##_##LOCK##_##AV##_##CAPS##_##RELIABILITY


/*
 * --------------------------- begin: rx atomics ------------------------------
 */
#define FI_OPX_RX_ATOMIC_SPECIALIZED_MACRO_NAME(OP) FI_OPX_RX_ATOMIC_SPECIALIZED_MACRO_NAME_(OP)

#define FI_OPX_RX_ATOMIC_SPECIALIZED_MACRO_NAME_(OP) FI_OPX_RX_ATOMIC_DO_##OP

#define FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(OP, DT, CTYPE)                                           \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_(OP, DT, CTYPE)

#define FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_(OP, DT, CTYPE)                                          \
	static inline void fi_opx_rx_atomic_##OP##_##DT(const void *buf, void *addr, size_t nbytes)                    \
	{                                                                                          \
		FI_OPX_RX_ATOMIC_SPECIALIZED_MACRO_NAME(OP)(buf, addr, CTYPE)                      \
	}

#define FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(OP, DT)                                             \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME_(OP, DT)

#define FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME_(OP, DT) fi_opx_rx_atomic_##OP##_##DT

#define FI_OPX_RX_ATOMIC_DO_MIN(buf_, addr_, ctype)                                                \
	ctype *buf__ = (ctype *)buf_;                                                              \
	ctype *addr__ = (ctype *)addr_;                                                            \
	const size_t count = nbytes / sizeof(ctype);                                               \
	unsigned i;                                                                                \
	for (i = 0; i < count; ++i)                                                                \
		if (buf__[i] < addr__[i])                                                          \
			addr__[i] = buf__[i];

#define FI_OPX_RX_ATOMIC_DO_MAX(buf_, addr_, ctype)                                                \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			if (buf__[i] > addr__[i])                                                  \
				addr__[i] = buf__[i];                                              \
	}

#define FI_OPX_RX_ATOMIC_DO_SUM(buf_, addr_, ctype)                                                \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			addr__[i] += buf__[i];                                                     \
	}

#define FI_OPX_RX_ATOMIC_DO_PROD(buf_, addr_, ctype)                                               \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			addr__[i] = addr__[i] * buf__[i];                                          \
	}

#define FI_OPX_RX_ATOMIC_DO_LOR(buf_, addr_, ctype)                                                \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			addr__[i] = (addr__[i] || buf__[i]);                                       \
	}

#define FI_OPX_RX_ATOMIC_DO_LAND(buf_, addr_, ctype)                                               \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			addr__[i] = (addr__[i] && buf__[i]);                                       \
	}

#define FI_OPX_RX_ATOMIC_DO_BOR_(buf_, addr_, ctype)                                               \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			addr__[i] |= buf__[i];                                                     \
	}

#define FI_OPX_RX_ATOMIC_DO_BOR(buf_, addr_, ctype)                                                \
	{                                                                                          \
		if (sizeof(uint8_t) == sizeof(ctype)) {                                            \
			FI_OPX_RX_ATOMIC_DO_BOR_(buf_, addr_, uint8_t);                            \
		} else if (sizeof(uint16_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_BOR_(buf_, addr_, uint16_t);                           \
		} else if (sizeof(uint32_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_BOR_(buf_, addr_, uint32_t);                           \
		} else if (sizeof(uint64_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_BOR_(buf_, addr_, uint64_t);                           \
		} else if (16 == sizeof(ctype)) {                                                  \
			FI_OPX_RX_ATOMIC_DO_BOR_(&(((uint64_t *)buf_)[0]),                         \
						 &(((uint64_t *)addr_)[0]), uint64_t);             \
			FI_OPX_RX_ATOMIC_DO_BOR_(&(((uint64_t *)buf_)[1]),                         \
						 &(((uint64_t *)addr_)[1]), uint64_t);             \
		} else if (32 == sizeof(ctype)) {                                                  \
			FI_OPX_RX_ATOMIC_DO_BOR_(&(((uint64_t *)buf_)[0]),                         \
						 &(((uint64_t *)addr_)[0]), uint64_t);             \
			FI_OPX_RX_ATOMIC_DO_BOR_(&(((uint64_t *)buf_)[1]),                         \
						 &(((uint64_t *)addr_)[1]), uint64_t);             \
			FI_OPX_RX_ATOMIC_DO_BOR_(&(((uint64_t *)buf_)[2]),                         \
						 &(((uint64_t *)addr_)[2]), uint64_t);             \
			FI_OPX_RX_ATOMIC_DO_BOR_(&(((uint64_t *)buf_)[3]),                         \
						 &(((uint64_t *)addr_)[3]), uint64_t);             \
		} else {                                                                           \
			assert(0);                                                                 \
		}                                                                                  \
	}

#define FI_OPX_RX_ATOMIC_DO_BAND_(buf_, addr_, ctype)                                              \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			addr__[i] &= buf__[i];                                                     \
	}

#define FI_OPX_RX_ATOMIC_DO_BAND(buf_, addr_, ctype)                                               \
	{                                                                                          \
		if (sizeof(uint8_t) == sizeof(ctype)) {                                            \
			FI_OPX_RX_ATOMIC_DO_BAND_(buf_, addr_, uint8_t);                           \
		} else if (sizeof(uint16_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_BAND_(buf_, addr_, uint16_t);                          \
		} else if (sizeof(uint32_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_BAND_(buf_, addr_, uint32_t);                          \
		} else if (sizeof(uint64_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_BAND_(buf_, addr_, uint64_t);                          \
		} else if (16 == sizeof(ctype)) {                                                  \
			FI_OPX_RX_ATOMIC_DO_BAND_(&(((uint64_t *)buf_)[0]),                        \
						  &(((uint64_t *)addr_)[0]), uint64_t);            \
			FI_OPX_RX_ATOMIC_DO_BAND_(&(((uint64_t *)buf_)[1]),                        \
						  &(((uint64_t *)addr_)[1]), uint64_t);            \
		} else if (32 == sizeof(ctype)) {                                                  \
			FI_OPX_RX_ATOMIC_DO_BAND_(&(((uint64_t *)buf_)[0]),                        \
						  &(((uint64_t *)addr_)[0]), uint64_t);            \
			FI_OPX_RX_ATOMIC_DO_BAND_(&(((uint64_t *)buf_)[1]),                        \
						  &(((uint64_t *)addr_)[1]), uint64_t);            \
			FI_OPX_RX_ATOMIC_DO_BAND_(&(((uint64_t *)buf_)[2]),                        \
						  &(((uint64_t *)addr_)[2]), uint64_t);            \
			FI_OPX_RX_ATOMIC_DO_BAND_(&(((uint64_t *)buf_)[3]),                        \
						  &(((uint64_t *)addr_)[3]), uint64_t);            \
		} else {                                                                           \
			assert(0);                                                                 \
		}                                                                                  \
	}

#define FI_OPX_RX_ATOMIC_DO_LXOR(buf_, addr_, ctype)                                               \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			addr__[i] = ((addr__[i] && !buf__[i]) || (!addr__[i] && buf__[i]));        \
	}

#define FI_OPX_RX_ATOMIC_DO_BXOR_(buf_, addr_, ctype)                                              \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			addr__[i] = addr__[i] ^ buf__[i];                                          \
	}

#define FI_OPX_RX_ATOMIC_DO_BXOR(buf_, addr_, ctype)                                               \
	{                                                                                          \
		if (sizeof(uint8_t) == sizeof(ctype)) {                                            \
			FI_OPX_RX_ATOMIC_DO_BXOR_(buf_, addr_, uint8_t);                           \
		} else if (sizeof(uint16_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_BXOR_(buf_, addr_, uint16_t);                          \
		} else if (sizeof(uint32_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_BXOR_(buf_, addr_, uint32_t);                          \
		} else if (sizeof(uint64_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_BXOR_(buf_, addr_, uint64_t);                          \
		} else if (16 == sizeof(ctype)) {                                                  \
			FI_OPX_RX_ATOMIC_DO_BXOR_(&(((uint64_t *)buf_)[0]),                        \
						  &(((uint64_t *)addr_)[0]), uint64_t);            \
			FI_OPX_RX_ATOMIC_DO_BXOR_(&(((uint64_t *)buf_)[1]),                        \
						  &(((uint64_t *)addr_)[1]), uint64_t);            \
		} else if (32 == sizeof(ctype)) {                                                  \
			FI_OPX_RX_ATOMIC_DO_BXOR_(&(((uint64_t *)buf_)[0]),                        \
						  &(((uint64_t *)addr_)[0]), uint64_t);            \
			FI_OPX_RX_ATOMIC_DO_BXOR_(&(((uint64_t *)buf_)[1]),                        \
						  &(((uint64_t *)addr_)[1]), uint64_t);            \
			FI_OPX_RX_ATOMIC_DO_BXOR_(&(((uint64_t *)buf_)[2]),                        \
						  &(((uint64_t *)addr_)[2]), uint64_t);            \
			FI_OPX_RX_ATOMIC_DO_BXOR_(&(((uint64_t *)buf_)[3]),                        \
						  &(((uint64_t *)addr_)[3]), uint64_t);            \
		} else {                                                                           \
			assert(0);                                                                 \
		}                                                                                  \
	}

#define FI_OPX_RX_ATOMIC_DO_ATOMIC_READ(buf_, addr_, ctype)                                        \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			buf__[i] = addr__[i];                                                      \
	}

#define FI_OPX_RX_ATOMIC_DO_ATOMIC_WRITE(buf_, addr_, ctype)                                       \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			addr__[i] = buf__[i];                                                      \
	}

#define FI_OPX_RX_ATOMIC_DO_CSWAP(buf_, addr_, ctype)                                              \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		const ctype *compare__ = &buf__[count];                                            \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			if (compare__[i] == addr__[i])                                             \
				addr__[i] = buf__[i];                                              \
	}

#define FI_OPX_RX_ATOMIC_DO_CSWAP_NE(buf_, addr_, ctype)                                           \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		const ctype *compare__ = &buf__[count];                                            \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			if (compare__[i] != addr__[i])                                             \
				addr__[i] = buf__[i];                                              \
	}

#define FI_OPX_RX_ATOMIC_DO_CSWAP_LE(buf_, addr_, ctype)                                           \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		const ctype *compare__ = &buf__[count];                                            \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			if (compare__[i] <= addr__[i])                                             \
				addr__[i] = buf__[i];                                              \
	}

#define FI_OPX_RX_ATOMIC_DO_CSWAP_LT(buf_, addr_, ctype)                                           \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		const ctype *compare__ = &buf__[count];                                            \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			if (compare__[i] < addr__[i])                                              \
				addr__[i] = buf__[i];                                              \
	}

#define FI_OPX_RX_ATOMIC_DO_CSWAP_GE(buf_, addr_, ctype)                                           \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		const ctype *compare__ = &buf__[count];                                            \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			if (compare__[i] >= addr__[i])                                             \
				addr__[i] = buf__[i];                                              \
	}

#define FI_OPX_RX_ATOMIC_DO_CSWAP_GT(buf_, addr_, ctype)                                           \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		const ctype *compare__ = &buf__[count];                                            \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			if (compare__[i] > addr__[i])                                              \
				addr__[i] = buf__[i];                                              \
	}

#define FI_OPX_RX_ATOMIC_DO_MSWAP_(buf_, addr_, ctype)                                             \
	{                                                                                          \
		ctype *buf__ = (ctype *)buf_;                                                      \
		ctype *addr__ = (ctype *)addr_;                                                    \
		const size_t count = nbytes / sizeof(ctype);                                       \
		const ctype *compare__ = &buf__[count];                                            \
		unsigned i;                                                                        \
		for (i = 0; i < count; ++i)                                                        \
			addr__[i] = (buf__[i] & compare__[i]) | (addr__[i] & ~compare__[i]);       \
	}

#define FI_OPX_RX_ATOMIC_DO_MSWAP(buf_, addr_, ctype)                                              \
	{                                                                                          \
		if (sizeof(uint8_t) == sizeof(ctype)) {                                            \
			FI_OPX_RX_ATOMIC_DO_MSWAP_(buf_, addr_, uint8_t);                          \
		} else if (sizeof(uint16_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_MSWAP_(buf_, addr_, uint16_t);                         \
		} else if (sizeof(uint32_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_MSWAP_(buf_, addr_, uint32_t);                         \
		} else if (sizeof(uint64_t) == sizeof(ctype)) {                                    \
			FI_OPX_RX_ATOMIC_DO_MSWAP_(buf_, addr_, uint64_t);                         \
		} else if (16 == sizeof(ctype)) {                                                  \
			FI_OPX_RX_ATOMIC_DO_MSWAP_(&(((uint64_t *)buf_)[0]),                       \
						   &(((uint64_t *)addr_)[0]), uint64_t);           \
			FI_OPX_RX_ATOMIC_DO_MSWAP_(&(((uint64_t *)buf_)[1]),                       \
						   &(((uint64_t *)addr_)[1]), uint64_t);           \
		} else if (32 == sizeof(ctype)) {                                                  \
			FI_OPX_RX_ATOMIC_DO_MSWAP_(&(((uint64_t *)buf_)[0]),                       \
						   &(((uint64_t *)addr_)[0]), uint64_t);           \
			FI_OPX_RX_ATOMIC_DO_MSWAP_(&(((uint64_t *)buf_)[1]),                       \
						   &(((uint64_t *)addr_)[1]), uint64_t);           \
			FI_OPX_RX_ATOMIC_DO_MSWAP_(&(((uint64_t *)buf_)[2]),                       \
						   &(((uint64_t *)addr_)[2]), uint64_t);           \
			FI_OPX_RX_ATOMIC_DO_MSWAP_(&(((uint64_t *)buf_)[3]),                       \
						   &(((uint64_t *)addr_)[3]), uint64_t);           \
		} else {                                                                           \
			assert(0);                                                                 \
		}                                                                                  \
	}

#define FI_OPX_RX_ATOMIC_DO_NOOP(buf_, addr_, ctype)                                               \
	{                                                                                          \
	}

#define FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(DT, CTYPE)                                              \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(MIN, DT, CTYPE);                                         \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(MAX, DT, CTYPE);                                         \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(SUM, DT, CTYPE);                                         \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(PROD, DT, CTYPE);                                        \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(LOR, DT, CTYPE);                                         \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(LAND, DT, CTYPE);                                        \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(BOR, DT, CTYPE);                                         \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(BAND, DT, CTYPE);                                        \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(LXOR, DT, CTYPE);                                        \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(BXOR, DT, CTYPE);                                        \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(ATOMIC_READ, DT, CTYPE);                                 \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(ATOMIC_WRITE, DT, CTYPE);                                \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP, DT, CTYPE);                                       \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP_NE, DT, CTYPE);                                    \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP_LE, DT, CTYPE);                                    \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP_LT, DT, CTYPE);                                    \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP_GE, DT, CTYPE);                                    \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP_GT, DT, CTYPE);                                    \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(MSWAP, DT, CTYPE);

#define FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS_COMPLEX(DT, CTYPE)                                      \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(SUM, DT, CTYPE);                                         \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(PROD, DT, CTYPE);                                        \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(LOR, DT, CTYPE);                                         \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(BAND, DT, CTYPE);					                       \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(BXOR, DT, CTYPE);					                       \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(LAND, DT, CTYPE);					                       \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(BOR, DT, CTYPE);                                         \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(LXOR, DT, CTYPE);                                        \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(ATOMIC_READ, DT, CTYPE);                                 \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(ATOMIC_WRITE, DT, CTYPE);			                       \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(MSWAP, DT, CTYPE);			                           \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP, DT, CTYPE);

static inline void fi_opx_rx_atomic_NOOP(const void *addr, void *buf, size_t nbytes)
{
}

FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(INT8, int8_t)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(UINT8, uint8_t)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(INT16, int16_t)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(UINT16, uint16_t)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(INT32, int32_t)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(UINT32, uint32_t)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(INT64, int64_t)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(UINT64, uint64_t)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(FLOAT, float)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(DOUBLE, double)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS_COMPLEX(FLOAT_COMPLEX, complex float)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS_COMPLEX(DOUBLE_COMPLEX, complex double)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS(LONG_DOUBLE, long double)
FI_OPX_RX_ATOMIC_SPECIALIZED_FUNCS_COMPLEX(LONG_DOUBLE_COMPLEX, complex long double)

#define FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(DT)                                                   \
	FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(MIN, DT),                                           \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(MAX, DT),                                   \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(SUM, DT),                                   \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(PROD, DT),                                  \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LOR, DT),                                   \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LAND, DT),                                  \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BOR, DT),                                   \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BAND, DT),                                  \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LXOR, DT),                                  \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BXOR, DT),                                  \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(ATOMIC_READ, DT),                           \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(ATOMIC_WRITE, DT),                          \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP, DT),                                 \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP_NE, DT),                              \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP_LE, DT),                              \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP_LT, DT),                              \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP_GE, DT),                              \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP_GT, DT),                              \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(MSWAP, DT)

#define FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES_COMPLEX(DT)                                           \
	fi_opx_rx_atomic_NOOP, fi_opx_rx_atomic_NOOP,                                              \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(SUM, DT),                                   \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(PROD, DT),                                  \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LOR, DT),                                   \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LAND, DT),                                  \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BOR, DT),                                   \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BAND, DT),                                  \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LXOR, DT),                                  \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BXOR, DT),                                  \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(ATOMIC_READ, DT),                           \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(ATOMIC_WRITE, DT),                          \
		FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP, DT), fi_opx_rx_atomic_NOOP,          \
		fi_opx_rx_atomic_NOOP, fi_opx_rx_atomic_NOOP, fi_opx_rx_atomic_NOOP,               \
		fi_opx_rx_atomic_NOOP, FI_OPX_RX_ATOMIC_SPECIALIZED_FUNC_NAME(MSWAP, DT)

static inline void fi_opx_rx_atomic_dispatch(const void *buf, void *addr, size_t nbytes, enum fi_datatype dt,
			       enum fi_op op)
{
	static void (*fi_opx_rx_atomic_dispatch_table[FI_DATATYPE_LAST][FI_ATOMIC_OP_LAST])(
		const void *, void *,
		size_t) = { { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(INT8) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(UINT8) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(INT16) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(UINT16) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(INT32) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(UINT32) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(INT64) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(UINT64) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(FLOAT) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(DOUBLE) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES_COMPLEX(FLOAT_COMPLEX) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES_COMPLEX(DOUBLE_COMPLEX) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES(LONG_DOUBLE) },
			    { FI_OPX_RX_ATOMIC_DISPATCH_FUNC_NAMES_COMPLEX(LONG_DOUBLE_COMPLEX) } };

	fi_opx_rx_atomic_dispatch_table[dt][op](buf, addr, nbytes);
}
/*
* --------------------------- end: rx atomics --------------------------------
*/






#endif // _FI_PROV_OPX_RMA_OPS_H_

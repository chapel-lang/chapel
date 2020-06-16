/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#include "rdma/bgq/fi_bgq.h"
#include <ofi_enosys.h>

#include <complex.h>

/*
 * --------------------------- begin: rx atomics ------------------------------
 */
#define FI_BGQ_RX_ATOMIC_SPECIALIZED_MACRO_NAME(OP)				\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_MACRO_NAME_(OP)

#define FI_BGQ_RX_ATOMIC_SPECIALIZED_MACRO_NAME_(OP)				\
	FI_BGQ_RX_ATOMIC_DO_ ## OP

#define FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(OP, DT, CTYPE)			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_(OP, DT, CTYPE)

#define FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_(OP, DT, CTYPE)			\
	void fi_bgq_rx_atomic_ ## OP ## _ ## DT					\
		(void * buf, void * addr, size_t nbytes)			\
	{									\
		FI_BGQ_RX_ATOMIC_SPECIALIZED_MACRO_NAME(OP)(buf, addr, CTYPE)	\
	}

#define FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(OP, DT)				\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME_(OP, DT)

#define FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME_(OP, DT)				\
	fi_bgq_rx_atomic_ ## OP ## _ ## DT


#define FI_BGQ_RX_ATOMIC_DO_MIN(buf_, addr_, ctype)				\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		if (buf__[i] < addr__[i])					\
			addr__[i] = buf__[i];					\

#define FI_BGQ_RX_ATOMIC_DO_MAX(buf_, addr_, ctype)				\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		if (buf__[i] > addr__[i])					\
			addr__[i] = buf__[i];					\
}

#define FI_BGQ_RX_ATOMIC_DO_SUM(buf_, addr_, ctype)				\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		addr__[i] += buf__[i];						\
}

#define FI_BGQ_RX_ATOMIC_DO_PROD(buf_, addr_, ctype)				\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		addr__[i] = addr__[i] * buf__[i];				\
}

#define FI_BGQ_RX_ATOMIC_DO_LOR(buf_, addr_, ctype)				\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		addr__[i] = (addr__[i] || buf__[i]);				\
}

#define FI_BGQ_RX_ATOMIC_DO_LAND(buf_, addr_, ctype)				\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		addr__[i] = (addr__[i] && buf__[i]);				\
}

#define FI_BGQ_RX_ATOMIC_DO_BOR_(buf_, addr_, ctype)				\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		addr__[i] |= buf__[i];						\
}

#define FI_BGQ_RX_ATOMIC_DO_BOR(buf_, addr_, ctype)				\
{										\
	if (sizeof(uint8_t) == sizeof(ctype)) {					\
		FI_BGQ_RX_ATOMIC_DO_BOR_(buf_, addr_, uint8_t);			\
	} else if (sizeof(uint16_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_BOR_(buf_, addr_, uint16_t);		\
	} else if (sizeof(uint32_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_BOR_(buf_, addr_, uint32_t);		\
	} else if (sizeof(uint64_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_BOR_(buf_, addr_, uint64_t);		\
	} else if (16 == sizeof(ctype)) {					\
		FI_BGQ_RX_ATOMIC_DO_BOR_(&(((uint64_t*)buf_)[0]), &(((uint64_t*)addr_)[0]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_BOR_(&(((uint64_t*)buf_)[1]), &(((uint64_t*)addr_)[1]), uint64_t);          \
	} else if (32 == sizeof(ctype)) {					\
		FI_BGQ_RX_ATOMIC_DO_BOR_(&(((uint64_t*)buf_)[0]), &(((uint64_t*)addr_)[0]), uint64_t);		\
		FI_BGQ_RX_ATOMIC_DO_BOR_(&(((uint64_t*)buf_)[1]), &(((uint64_t*)addr_)[1]), uint64_t);		\
		FI_BGQ_RX_ATOMIC_DO_BOR_(&(((uint64_t*)buf_)[2]), &(((uint64_t*)addr_)[2]), uint64_t);		\
		FI_BGQ_RX_ATOMIC_DO_BOR_(&(((uint64_t*)buf_)[3]), &(((uint64_t*)addr_)[3]), uint64_t);		\
	} else {								\
		assert(0);							\
	}									\
}

#define FI_BGQ_RX_ATOMIC_DO_BAND_(buf_, addr_, ctype)				\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		addr__[i] &= buf__[i];						\
}

#define FI_BGQ_RX_ATOMIC_DO_BAND(buf_, addr_, ctype)				\
{										\
	if (sizeof(uint8_t) == sizeof(ctype)) {					\
		FI_BGQ_RX_ATOMIC_DO_BAND_(buf_, addr_, uint8_t);		\
	} else if (sizeof(uint16_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_BAND_(buf_, addr_, uint16_t);		\
	} else if (sizeof(uint32_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_BAND_(buf_, addr_, uint32_t);		\
	} else if (sizeof(uint64_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_BAND_(buf_, addr_, uint64_t);		\
	} else if (16 == sizeof(ctype)) {			\
		FI_BGQ_RX_ATOMIC_DO_BAND_(&(((uint64_t*)buf_)[0]), &(((uint64_t*)addr_)[0]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_BAND_(&(((uint64_t*)buf_)[1]), &(((uint64_t*)addr_)[1]), uint64_t);          \
	} else if (32 == sizeof(ctype)) {					\
		FI_BGQ_RX_ATOMIC_DO_BAND_(&(((uint64_t*)buf_)[0]), &(((uint64_t*)addr_)[0]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_BAND_(&(((uint64_t*)buf_)[1]), &(((uint64_t*)addr_)[1]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_BAND_(&(((uint64_t*)buf_)[2]), &(((uint64_t*)addr_)[2]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_BAND_(&(((uint64_t*)buf_)[3]), &(((uint64_t*)addr_)[3]), uint64_t);          \
	} else {								\
		assert(0);							\
	}									\
}

#define FI_BGQ_RX_ATOMIC_DO_LXOR(buf_, addr_, ctype)				\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		addr__[i] = ((addr__[i] && !buf__[i]) ||			\
			(!addr__[i] && buf__[i]));				\
}

#define FI_BGQ_RX_ATOMIC_DO_BXOR_(buf_, addr_, ctype)				\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		addr__[i] = addr__[i] ^ buf__[i];				\
}

#define FI_BGQ_RX_ATOMIC_DO_BXOR(buf_, addr_, ctype)				\
{										\
	if (sizeof(uint8_t) == sizeof(ctype)) {					\
		FI_BGQ_RX_ATOMIC_DO_BXOR_(buf_, addr_, uint8_t);		\
	} else if (sizeof(uint16_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_BXOR_(buf_, addr_, uint16_t);		\
	} else if (sizeof(uint32_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_BXOR_(buf_, addr_, uint32_t);		\
	} else if (sizeof(uint64_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_BXOR_(buf_, addr_, uint64_t);		\
	} else if (16 == sizeof(ctype)) {			\
		FI_BGQ_RX_ATOMIC_DO_BXOR_(&(((uint64_t*)buf_)[0]), &(((uint64_t*)addr_)[0]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_BXOR_(&(((uint64_t*)buf_)[1]), &(((uint64_t*)addr_)[1]), uint64_t);          \
	} else if (32 == sizeof(ctype)) {					\
		FI_BGQ_RX_ATOMIC_DO_BXOR_(&(((uint64_t*)buf_)[0]), &(((uint64_t*)addr_)[0]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_BXOR_(&(((uint64_t*)buf_)[1]), &(((uint64_t*)addr_)[1]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_BXOR_(&(((uint64_t*)buf_)[2]), &(((uint64_t*)addr_)[2]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_BXOR_(&(((uint64_t*)buf_)[3]), &(((uint64_t*)addr_)[3]), uint64_t);          \
	} else {								\
		assert(0);							\
	}									\
}

#define FI_BGQ_RX_ATOMIC_DO_ATOMIC_READ(buf_, addr_, ctype)			\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		buf__[i] = addr__[i];						\
}

#define FI_BGQ_RX_ATOMIC_DO_ATOMIC_WRITE(buf_, addr_, ctype)			\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		addr__[i] = buf__[i];						\
}

#define FI_BGQ_RX_ATOMIC_DO_CSWAP(buf_, addr_, ctype)				\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	const ctype * compare__ = &buf__[count];				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		if (compare__[i] == addr__[i])					\
			addr__[i] = buf__[i];					\
}

#define FI_BGQ_RX_ATOMIC_DO_CSWAP_NE(buf_, addr_, ctype)			\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	const ctype * compare__ = &buf__[count];				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		if (compare__[i] != addr__[i])					\
			addr__[i] = buf__[i];					\
}

#define FI_BGQ_RX_ATOMIC_DO_CSWAP_LE(buf_, addr_, ctype)			\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	const ctype * compare__ = &buf__[count];				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		if (compare__[i] <= addr__[i])					\
			addr__[i] = buf__[i];					\
}

#define FI_BGQ_RX_ATOMIC_DO_CSWAP_LT(buf_, addr_, ctype)			\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	const ctype * compare__ = &buf__[count];				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		if (compare__[i] < addr__[i])					\
			addr__[i] = buf__[i];					\
}

#define FI_BGQ_RX_ATOMIC_DO_CSWAP_GE(buf_, addr_, ctype)			\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	const ctype * compare__ = &buf__[count];				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		if (compare__[i] >= addr__[i])					\
			addr__[i] = buf__[i];					\
}

#define FI_BGQ_RX_ATOMIC_DO_CSWAP_GT(buf_, addr_, ctype)			\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	const ctype * compare__ = &buf__[count];				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		if (compare__[i] > addr__[i])					\
			addr__[i] = buf__[i];					\
}

#define FI_BGQ_RX_ATOMIC_DO_MSWAP_(buf_, addr_, ctype)				\
{										\
	ctype * buf__ = (ctype *)buf_;						\
	ctype * addr__ = (ctype *)addr_;					\
	const size_t count = nbytes / sizeof(ctype);				\
	const ctype * compare__ = &buf__[count];				\
	unsigned i;								\
	for (i=0; i<count; ++i)							\
		addr__[i] =							\
			(buf__[i] & compare__[i]) |				\
			(addr__[i] & ~compare__[i]);				\
}

#define FI_BGQ_RX_ATOMIC_DO_MSWAP(buf_, addr_, ctype)				\
{										\
	if (sizeof(uint8_t) == sizeof(ctype)) {					\
		FI_BGQ_RX_ATOMIC_DO_MSWAP_(buf_, addr_, uint8_t);		\
	} else if (sizeof(uint16_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_MSWAP_(buf_, addr_, uint16_t);		\
	} else if (sizeof(uint32_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_MSWAP_(buf_, addr_, uint32_t);		\
	} else if (sizeof(uint64_t) == sizeof(ctype)) {				\
		FI_BGQ_RX_ATOMIC_DO_MSWAP_(buf_, addr_, uint64_t);		\
	} else if (16 == sizeof(ctype)) {			\
		FI_BGQ_RX_ATOMIC_DO_MSWAP_(&(((uint64_t*)buf_)[0]), &(((uint64_t*)addr_)[0]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_MSWAP_(&(((uint64_t*)buf_)[1]), &(((uint64_t*)addr_)[1]), uint64_t);          \
	} else if (32 == sizeof(ctype)) {					\
		FI_BGQ_RX_ATOMIC_DO_MSWAP_(&(((uint64_t*)buf_)[0]), &(((uint64_t*)addr_)[0]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_MSWAP_(&(((uint64_t*)buf_)[1]), &(((uint64_t*)addr_)[1]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_MSWAP_(&(((uint64_t*)buf_)[2]), &(((uint64_t*)addr_)[2]), uint64_t);          \
		FI_BGQ_RX_ATOMIC_DO_MSWAP_(&(((uint64_t*)buf_)[3]), &(((uint64_t*)addr_)[3]), uint64_t);          \
	} else {								\
		assert(0);							\
	}									\
}

#define FI_BGQ_RX_ATOMIC_DO_NOOP(buf_, addr_, ctype) {}

#define FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(DT, CTYPE)				\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(MIN, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(MAX, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(SUM, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(PROD, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(LOR, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(LAND, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(BOR, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(BAND, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(LXOR, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(BXOR, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(ATOMIC_READ, DT, CTYPE);		\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(ATOMIC_WRITE, DT, CTYPE);		\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP_NE, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP_LE, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP_LT, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP_GE, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP_GT, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(MSWAP, DT, CTYPE);

#define FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS_COMPLEX(DT, CTYPE)			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(SUM, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(PROD, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(LOR, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(LAND, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(BOR, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(LXOR, DT, CTYPE);			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(ATOMIC_READ, DT, CTYPE);		\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(ATOMIC_WRITE, DT, CTYPE);		\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC(CSWAP, DT, CTYPE);

void fi_bgq_rx_atomic_NOOP (void * addr, void * buf, size_t nbytes) {}

FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(INT8, int8_t)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(UINT8, uint8_t)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(INT16, int16_t)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(UINT16, uint16_t)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(INT32, int32_t)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(UINT32, uint32_t)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(INT64, int64_t)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(UINT64, uint64_t)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(FLOAT, float)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(DOUBLE, double)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS_COMPLEX(FLOAT_COMPLEX, complex float)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS_COMPLEX(DOUBLE_COMPLEX, complex double)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS(LONG_DOUBLE, long double)
FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNCS_COMPLEX(LONG_DOUBLE_COMPLEX, complex long double)

#define FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(DT)				\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(MIN, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(MAX, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(SUM, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(PROD, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LOR, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LAND, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BOR, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BAND, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LXOR, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BXOR, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(ATOMIC_READ, DT),		\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(ATOMIC_WRITE, DT),		\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP_NE, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP_LE, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP_LT, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP_GE, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP_GT, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(MSWAP, DT)

#define FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES_COMPLEX(DT)			\
	fi_bgq_rx_atomic_NOOP,							\
	fi_bgq_rx_atomic_NOOP,							\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(SUM, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(PROD, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LOR, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LAND, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BOR, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BAND, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(LXOR, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(BXOR, DT),			\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(ATOMIC_READ, DT),		\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(ATOMIC_WRITE, DT),		\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(CSWAP, DT),			\
	fi_bgq_rx_atomic_NOOP,							\
	fi_bgq_rx_atomic_NOOP,							\
	fi_bgq_rx_atomic_NOOP,							\
	fi_bgq_rx_atomic_NOOP,							\
	fi_bgq_rx_atomic_NOOP,							\
	FI_BGQ_RX_ATOMIC_SPECIALIZED_FUNC_NAME(MSWAP, DT)

void
fi_bgq_rx_atomic_dispatch (void * buf, void * addr, size_t nbytes,
	enum fi_datatype dt, enum fi_op op)
{
	static void (*fi_bgq_rx_atomic_dispatch_table[FI_DATATYPE_LAST][FI_ATOMIC_OP_LAST])(void*, void*, size_t) =
	{
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(INT8) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(UINT8) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(INT16) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(UINT16) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(INT32) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(UINT32) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(INT64) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(UINT64) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(FLOAT) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(DOUBLE) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES_COMPLEX(FLOAT) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES_COMPLEX(DOUBLE) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES(LONG_DOUBLE) },
		{ FI_BGQ_RX_ATOMIC_DISPATCH_FUNC_NAMES_COMPLEX(LONG_DOUBLE) }
	};

	fi_bgq_rx_atomic_dispatch_table[dt][op](buf, addr, nbytes);
}
/*
 * --------------------------- end: rx atomics --------------------------------
 */

ssize_t fi_bgq_atomic(struct fid_ep *ep,
		const void *buf, size_t count, void *desc,
		fi_addr_t dst_addr, uint64_t addr,
		uint64_t key, enum fi_datatype datatype,
		enum fi_op op, void* context)
{
	int lock_required = 0;

	struct fi_bgq_ep * bgq_ep;
	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

        switch (bgq_ep->threading) {
        case FI_THREAD_ENDPOINT:
        case FI_THREAD_DOMAIN:
        case FI_THREAD_COMPLETION:
                lock_required = 0;
                break;
        case FI_THREAD_FID:
        case FI_THREAD_UNSPEC:
        case FI_THREAD_SAFE:
                lock_required = 1;
                break;
        default:
                return -FI_EINVAL;
        }

	return fi_bgq_atomic_generic(ep, buf, count, dst_addr,
			addr, key, datatype, op, context,
			lock_required);

}

ssize_t fi_bgq_fetch_atomic(struct fid_ep *ep,
		const void *buf, size_t count,
		void *desc,
		void *result, void *result_desc,
		fi_addr_t dest_addr, uint64_t addr,
		uint64_t key, enum fi_datatype datatype,
		enum fi_op op, void *context)
{
	int lock_required = 0;

	struct fi_bgq_ep * bgq_ep;
	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

        switch (bgq_ep->threading) {
        case FI_THREAD_ENDPOINT:
        case FI_THREAD_DOMAIN:
        case FI_THREAD_COMPLETION:
                lock_required = 0;
                break;
        case FI_THREAD_FID:
        case FI_THREAD_UNSPEC:
        case FI_THREAD_SAFE:
                lock_required = 1;
                break;
        default:
                return -FI_EINVAL;
	}

	return fi_bgq_fetch_atomic_generic(ep,
			buf, count, desc,
			result, result_desc, dest_addr, addr,
			key, datatype, op, context,
			lock_required);
}

ssize_t fi_bgq_compare_atomic(struct fid_ep *ep,
		const void *buf, size_t count, void *desc,
		const void *compare, void *compare_desc,
		void  *result, void *result_desc,
		fi_addr_t dest_addr, uint64_t addr,
		uint64_t key, enum fi_datatype datatype,
		enum fi_op op, void *context)
{
	int lock_required = 0;

	struct fi_bgq_ep * bgq_ep;
	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

        switch (bgq_ep->threading) {
        case FI_THREAD_ENDPOINT:
        case FI_THREAD_DOMAIN:
        case FI_THREAD_COMPLETION:
                lock_required = 0;
                break;
        case FI_THREAD_FID:
        case FI_THREAD_UNSPEC:
        case FI_THREAD_SAFE:
                lock_required = 1;
                break;
        default:
                return -FI_EINVAL;
	}

	return fi_bgq_compare_atomic_generic(ep,
			buf, count, desc,
			compare, compare_desc,
			result, result_desc, dest_addr, addr,
			key, datatype, op, context,
			lock_required);
}

ssize_t fi_bgq_inject_atomic(struct fid_ep *ep,
                const void *buf, size_t count,
                fi_addr_t dest_addr, uint64_t addr, uint64_t key,
                enum fi_datatype datatype, enum fi_op op)
{
	int lock_required = 0;

	struct fi_bgq_ep * bgq_ep;
	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

        switch (bgq_ep->threading) {
        case FI_THREAD_ENDPOINT:
        case FI_THREAD_DOMAIN:
        case FI_THREAD_COMPLETION:
                lock_required = 0;
                break;
        case FI_THREAD_FID:
        case FI_THREAD_UNSPEC:
        case FI_THREAD_SAFE:
                lock_required = 1;
                break;
        default:
                return -FI_EINVAL;
	}

	return fi_bgq_inject_atomic_generic(ep,
			buf, count,
			dest_addr, addr,
			key, datatype, op,
			lock_required);
}

ssize_t	fi_bgq_atomicv(struct fid_ep *ep,
			const struct fi_ioc *iov, void **desc, size_t count,
			uint64_t addr, uint64_t key,
			enum fi_datatype datatype, enum fi_op op, void *context)
{
	errno = FI_ENOSYS;
	return -errno;
}

ssize_t fi_bgq_atomic_writemsg(struct fid_ep *ep,
	const struct fi_msg_atomic *msg, uint64_t flags)
{
	struct fi_bgq_ep * bgq_ep;
	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

        switch (bgq_ep->threading) {
        case FI_THREAD_ENDPOINT:
        case FI_THREAD_DOMAIN:
        case FI_THREAD_COMPLETION:
		return fi_bgq_atomic_writemsg_generic(ep, msg, flags,
				0);
        case FI_THREAD_FID:
        case FI_THREAD_UNSPEC:
        case FI_THREAD_SAFE:
		return fi_bgq_atomic_writemsg_generic(ep, msg, flags,
				1);
	}

	errno = FI_EINVAL;
	return -errno;
}

ssize_t	fi_bgq_atomic_readwritemsg(struct fid_ep *ep,
		const struct fi_msg_atomic *msg,
		struct fi_ioc *resultv,
	       	void **result_desc, size_t result_count,
		uint64_t flags)
{
	struct fi_bgq_ep * bgq_ep;
	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

        switch (bgq_ep->threading) {
        case FI_THREAD_ENDPOINT:
        case FI_THREAD_DOMAIN:
        case FI_THREAD_COMPLETION:
		return fi_bgq_atomic_readwritemsg_generic(ep, msg,
				resultv, result_count, flags,
				0);
        case FI_THREAD_FID:
        case FI_THREAD_UNSPEC:
        case FI_THREAD_SAFE:
		return fi_bgq_atomic_readwritemsg_generic(ep, msg,
				resultv, result_count, flags,
				1);
	}

	errno = FI_EINVAL;
	return -errno;
}

ssize_t	fi_bgq_atomic_compwritemsg(struct fid_ep *ep,
		const struct fi_msg_atomic *msg,
		const struct fi_ioc *comparev,
	       	void **compare_desc, size_t compare_count,
		struct fi_ioc *resultv, void **result_desc,
	       	size_t result_count,
		uint64_t flags)
{
	struct fi_bgq_ep * bgq_ep;
	bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);

        switch (bgq_ep->threading) {
        case FI_THREAD_ENDPOINT:
        case FI_THREAD_DOMAIN:
        case FI_THREAD_COMPLETION:
		return fi_bgq_atomic_compwritemsg_generic(ep, msg,
				comparev, compare_count,
				resultv, result_count,
				flags, 0);
        case FI_THREAD_FID:
        case FI_THREAD_UNSPEC:
        case FI_THREAD_SAFE:
		return fi_bgq_atomic_compwritemsg_generic(ep, msg,
				comparev, compare_count,
				resultv, result_count,
				flags, 1);
	}

	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_atomic_writevalid(struct fid_ep *ep, enum fi_datatype datatype,
		enum fi_op op, size_t *count)
{
	static size_t sizeofdt[FI_DATATYPE_LAST] = {
		sizeof(int8_t),			/* FI_INT8 */
		sizeof(uint8_t),		/* FI_UINT8 */
		sizeof(int16_t),		/* FI_INT16 */
		sizeof(uint16_t),		/* FI_UINT16 */
		sizeof(int32_t),		/* FI_INT32 */
		sizeof(uint32_t),		/* FI_UINT32 */
		sizeof(int64_t),		/* FI_INT64 */
		sizeof(uint64_t),		/* FI_UINT64 */
		sizeof(float),			/* FI_FLOAT */
		sizeof(double),			/* FI_DOUBLE */
		sizeof(complex float),		/* FI_FLOAT_COMPLEX */
		sizeof(complex double),		/* FI_DOUBLE_COMPLEX */
		sizeof(long double),		/* FI_LONG_DOUBLE */
		sizeof(complex long double)	/* FI_LONG_DOUBLE_COMPLEX */
	};

	if ((op > FI_ATOMIC_WRITE) || (datatype >= FI_DATATYPE_LAST)) {
		*count = 0;
		errno = FI_EOPNOTSUPP;
		return -errno;
	}

	*count = sizeof(union fi_bgq_mu_packet_payload) / sizeofdt[datatype];
	return 0;
}

int fi_bgq_atomic_readwritevalid(struct fid_ep *ep,
	       	enum fi_datatype datatype,
		enum fi_op op, size_t *count)
{
	static size_t sizeofdt[FI_DATATYPE_LAST] = {
		sizeof(int8_t),			/* FI_INT8 */
		sizeof(uint8_t),		/* FI_UINT8 */
		sizeof(int16_t),		/* FI_INT16 */
		sizeof(uint16_t),		/* FI_UINT16 */
		sizeof(int32_t),		/* FI_INT32 */
		sizeof(uint32_t),		/* FI_UINT32 */
		sizeof(int64_t),		/* FI_INT64 */
		sizeof(uint64_t),		/* FI_UINT64 */
		sizeof(float),			/* FI_FLOAT */
		sizeof(double),			/* FI_DOUBLE */
		sizeof(complex float),		/* FI_FLOAT_COMPLEX */
		sizeof(complex double),		/* FI_DOUBLE_COMPLEX */
		sizeof(long double),		/* FI_LONG_DOUBLE */
		sizeof(complex long double)	/* FI_LONG_DOUBLE_COMPLEX */
	};

	if ((op > FI_ATOMIC_WRITE) || (datatype >= FI_DATATYPE_LAST)) {
		*count = 0;
		errno = FI_EOPNOTSUPP;
		return -errno;
	}

	*count = (sizeof(union fi_bgq_mu_packet_payload) - sizeof(struct fi_bgq_mu_fetch_metadata)) / sizeofdt[datatype];
	return 0;
}

int fi_bgq_atomic_compwritevalid(struct fid_ep *ep,
	       	enum fi_datatype datatype,
		enum fi_op op, size_t *count)
{
	static size_t sizeofdt[FI_DATATYPE_LAST] = {
		sizeof(int8_t),			/* FI_INT8 */
		sizeof(uint8_t),		/* FI_UINT8 */
		sizeof(int16_t),		/* FI_INT16 */
		sizeof(uint16_t),		/* FI_UINT16 */
		sizeof(int32_t),		/* FI_INT32 */
		sizeof(uint32_t),		/* FI_UINT32 */
		sizeof(int64_t),		/* FI_INT64 */
		sizeof(uint64_t),		/* FI_UINT64 */
		sizeof(float),			/* FI_FLOAT */
		sizeof(double),			/* FI_DOUBLE */
		sizeof(complex float),		/* FI_FLOAT_COMPLEX */
		sizeof(complex double),		/* FI_DOUBLE_COMPLEX */
		sizeof(long double),		/* FI_LONG_DOUBLE */
		sizeof(complex long double)	/* FI_LONG_DOUBLE_COMPLEX */
	};

	if ((op < FI_CSWAP) || (op >= FI_ATOMIC_OP_LAST) || (datatype >= FI_DATATYPE_LAST)) {
		*count = 0;
		errno = FI_EOPNOTSUPP;
		return -errno;
	}

	*count = (sizeof(union fi_bgq_mu_packet_payload) / 2) / sizeofdt[datatype];
	return 0;
}

static struct fi_ops_atomic fi_bgq_ops_atomic = {
	.size		= sizeof(struct fi_ops_atomic),
	.write		= fi_no_atomic_write,
	.writev		= fi_no_atomic_writev,
	.writemsg	= fi_bgq_atomic_writemsg,
	.inject		= fi_no_atomic_inject,
	.readwrite      = fi_no_atomic_readwrite,
	.readwritev     = fi_no_atomic_readwritev,
	.readwritemsg	= fi_bgq_atomic_readwritemsg,
	.compwrite	= fi_no_atomic_compwrite,
	.compwritev	= fi_no_atomic_compwritev,
	.compwritemsg	= fi_bgq_atomic_compwritemsg,
	.writevalid	= fi_bgq_atomic_writevalid,
	.readwritevalid	= fi_bgq_atomic_readwritevalid,
	.compwritevalid = fi_bgq_atomic_compwritevalid
};


int fi_bgq_init_atomic_ops(struct fi_bgq_ep *bgq_ep, struct fi_info *info)
{
	if (!info || !bgq_ep)
		goto err;

	if (info->caps & FI_ATOMICS ||
			(info->tx_attr &&
			 (info->tx_attr->caps & FI_ATOMICS))) {
		bgq_ep->ep_fid.atomic = &fi_bgq_ops_atomic;
	}
	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_enable_atomic_ops(struct fi_bgq_ep *bgq_ep)
{
	if (!bgq_ep || !bgq_ep->domain)
		goto err;

	if (!bgq_ep->ep_fid.atomic) {
		/* atomic ops not enabled on this endpoint */
		return 0;
	}
	/* fill in atomic formats */

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}


int fi_bgq_finalize_atomic_ops(struct fi_bgq_ep *bgq_ep)
{
	return 0;
}

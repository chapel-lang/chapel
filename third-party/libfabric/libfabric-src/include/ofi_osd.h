/*
 * Copyright (c) 2013-2014 Intel Corporation. All rights reserved.
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

#ifndef _OFI_OSD_H_
#define _OFI_OSD_H_

#include <stdint.h>

/* We use macros to create atomic and complex function definitions,
 * and we can't have spaces in function names */
typedef long double long_double;


/* Complex data type support:
 *
 * Complex data types are operating system.  For portability, each osd.h file
 * must define the following datatypes and operations:
 *
 * Datatype:
 * ofi_complex_XXX
 *
 * Operations:
 * ofi_complex_eq_XXX, ofi_complex_sum_XXX, ofi_complex_prod_XXX,
 * ofi_complex_land_XXX, ofi_complex_lor_XXX, ofi_complex_lxor_XXX
 *
 * Where XXX = float, double, or long_double
 */


#ifdef __APPLE__
#include <osx/osd.h>
#include <unix/osd.h>
#elif defined __FreeBSD__
#include <freebsd/osd.h>
#include <unix/osd.h>
#elif defined _WIN32
#include <windows/osd.h>
#else
#include <linux/osd.h>
#include <unix/osd.h>
#endif

#ifdef __GNUC__
#define OFI_LIKELY(x)	__builtin_expect((x), 1)
#define OFI_UNLIKELY(x)	__builtin_expect((x), 0)
#else
#define OFI_LIKELY(x)	(x)
#define OFI_UNLIKELY(x)	(x)
#endif

enum {
	OFI_ENDIAN_UNKNOWN,
	OFI_ENDIAN_BIG,
	OFI_ENDIAN_LITTLE,
	OFI_ENDIAN_BIG_WORD,	/* Middle-endian, Honeywell 316 style */
	OFI_ENDIAN_LITTLE_WORD,	/* Middle-endian, PDP-11 style */
};

static inline uint8_t ofi_detect_endianness(void)
{
	union {
		uint8_t data[sizeof(uint32_t)];
		uint32_t value;
	} checker = {
		.data = { 0x00, 0x01, 0x02, 0x03 },
	};
	switch (checker.value) {
	case 0x00010203UL:
		return OFI_ENDIAN_BIG;
	case 0x03020100UL:
		return OFI_ENDIAN_LITTLE;
	case 0x02030001UL:
		return OFI_ENDIAN_BIG_WORD;
	case 0x01000302UL:
		return OFI_ENDIAN_LITTLE_WORD;
	default:
		return OFI_ENDIAN_UNKNOWN;
	}
}

#define OFI_MAGIC_64 (0x0F1C0DE0F1C0DE64)
#define OFI_MAGIC_PTR ((void *) (uintptr_t) OFI_MAGIC_64)
#define OFI_MAGIC_SIZE_T ((size_t) OFI_MAGIC_64)

#ifndef NDEBUG
#define OFI_DBG_VAR(type, name) type name;
#define OFI_DBG_SET(name, val) name = val
#define OFI_DBG_ADD(name, val) name += val
#define OFI_DBG_CALL(func) func
#else
#define OFI_DBG_VAR(type, name)
#define OFI_DBG_SET(name, val) do {} while (0)
#define OFI_DBG_ADD(name, val) do {} while (0)
#define OFI_DBG_CALL(func) do {} while (0)
#endif

#endif /* _OFI_OSD_H_ */

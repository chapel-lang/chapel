/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#ifndef UTILS_BYTEORDER_H
#define UTILS_BYTEORDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <linux/types.h>
#include <sys/param.h>
#include <endian.h>

#ifndef __BYTE_ORDER
#	error "BYTE_ORDER undefined"
#endif

typedef __u16 __le16;
typedef __u16 __be16;
typedef __u32 __le32;
typedef __u32 __be32;
typedef __u64 __le64;
typedef __u64 __be64;

static __inline__ __u16 __hfi_fswab16(__u16)
    __attribute__ ((always_inline));
static __inline__ __u32 __hfi_fswab32(__u32)
    __attribute__ ((always_inline));
static __inline__ __u64 __hfi_fswab64(__u64)
    __attribute__ ((always_inline));

static __inline__ __u16 __hfi_fswab16(__u16 x) {
	return ((x & (__u16) 0x00ffU) << 8)
	    | ((x & (__u16) 0xff00U) >> 8);
} static __inline__ __u32 __hfi_fswab32(__u32 x) {
	return ((x & (__u32) 0x000000ffUL) << 24)
	    | ((x & (__u32) 0x0000ff00UL) << 8)
	    | ((x & (__u32) 0x00ff0000UL) >> 8)
	    | ((x & (__u32) 0xff000000UL) >> 24);
}

static __inline__ __u64 __hfi_fswab64(__u64 x) {
	return ((x & (__u64) 0x00000000000000ffULL) << 56)
	    | ((x & (__u64) 0x000000000000ff00ULL) << 40)
	    | ((x & (__u64) 0x0000000000ff0000ULL) << 24)
	    | ((x & (__u64) 0x00000000ff000000ULL) << 8)
	    | ((x & (__u64) 0x000000ff00000000ULL) >> 8)
	    | ((x & (__u64) 0x0000ff0000000000ULL) >> 24)
	    | ((x & (__u64) 0x00ff000000000000ULL) >> 40)
	    | ((x & (__u64) 0xff00000000000000ULL) >> 56);
}

static __inline__ __u16 __cpu_to_le16(__le16)
    __attribute__ ((always_inline));
static __inline__ __u32 __cpu_to_le32(__le32)
    __attribute__ ((always_inline));
static __inline__ __u64 __cpu_to_le64(__le64)
    __attribute__ ((always_inline));

static __inline__ __u16 __le16_to_cpu(__le16)
    __attribute__ ((always_inline));
static __inline__ __u32 __le32_to_cpu(__le32)
    __attribute__ ((always_inline));
static __inline__ __u64 __le64_to_cpu(__le64)
    __attribute__ ((always_inline));

static __inline__ __u16 __cpu_to_be16(__be16)
    __attribute__ ((always_inline));
static __inline__ __u32 __cpu_to_be32(__be32)
    __attribute__ ((always_inline));
static __inline__ __u64 __cpu_to_be64(__be64)
    __attribute__ ((always_inline));

static __inline__ __u16 __be16_to_cpu(__be16)
    __attribute__ ((always_inline));
static __inline__ __u32 __be32_to_cpu(__be32)
    __attribute__ ((always_inline));
static __inline__ __u64 __be64_to_cpu(__be64)
    __attribute__ ((always_inline));

#if __BYTE_ORDER == __LITTLE_ENDIAN

/*
 * __cpu_to_le* routines
 */
static __inline__ __le16 __cpu_to_le16(__u16 x) {
	return x;
}

static __inline__ __le32 __cpu_to_le32(__u32 x) {
	return x;
}

static __inline__ __le64 __cpu_to_le64(__u64 x) {
	return x;
}

/*
 * __le*_to_cpu routines
 */
static __inline__ __u16 __le16_to_cpu(__le16 x) {
	return x;
}

static __inline__ __u32 __le32_to_cpu(__le32 x) {
	return x;
}

static __inline__ __u64 __le64_to_cpu(__le64 x) {
	return x;
}

/*
 * __cpu_to_be* routines
 */
static __inline__ __be16 __cpu_to_be16(__u16 x) {
	return __hfi_fswab16(x);
}

static __inline__ __be32 __cpu_to_be32(__u32 x) {
	return __hfi_fswab32(x);
}

static __inline__ __be64 __cpu_to_be64(__u64 x) {
	return __hfi_fswab64(x);
}

/*
 * __be*_to_cpu routines
 */
static __inline__ __u16 __be16_to_cpu(__be16 x) {
	return __hfi_fswab16(x);
}

static __inline__ __u32 __be32_to_cpu(__be32 x) {
	return __hfi_fswab32(x);
}

static __inline__ __u64 __be64_to_cpu(__be64 x) {
	return __hfi_fswab64(x);
}

#elif __BYTE_ORDER == __BIG_ENDIAN

/*
 * __cpu_to_le* routines
 */
static __inline__ __le16 __cpu_to_le16(__u16 x) {
	return __hfi_fswab16(x);
}

static __inline__ __le32 __cpu_to_le32(__u32 x) {
	return __hfi_fswab32(x);
}

static __inline__ __le64 __cpu_to_le64(__u64 x) {
	return __hfi_fswab64(x);
}

/*
 * __le*_to_cpu routines
 */
static __inline__ __u16 __le16_to_cpu(__le16 x) {
	return __hfi_fswab16(x);
}

static __inline__ __u32 __le32_to_cpu(__le32 x) {
	return __hfi_fswab32(x);
}

static __inline__ __u64 __le64_to_cpu(__le64 x) {
	return __hfi_fswab64(x);
}

/*
 * __cpu_to_be* routines
 */
static __inline__ __be16 __cpu_to_be16(__u16 x) {
	return x;
}

static __inline__ __be32 __cpu_to_be32(__u32 x) {
	return x;
}

static __inline__ __be64 __cpu_to_be64(__u64 x) {
	return x;
}

/*
 * __be*_to_cpu routines
 */
static __inline__ __u16 __be16_to_cpu(__be16 x) {
	return x;
}

static __inline__ __u32 __be32_to_cpu(__be32 x) {
	return x;
}

static __inline__ __u64 __be64_to_cpu(__be64 x) {
	return x;
}

#else
#	error "unsupported BYTE_ORDER: " #BYTE_ORDER
#endif

#ifdef __cplusplus
}				/* extern "C" */
#endif
#endif /* UTILS_BYTEORDER_H */

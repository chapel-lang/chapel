/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2022 Intel Corporation.

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

  Copyright(c) 2022 Intel Corporation.

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

#ifdef PSM_ONEAPI

#ifndef _AM_ONEAPI_MEMHANDLE_H
#define _AM_ONEAPI_MEMHANDLE_H

#include "psm_user.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ONEAPI_MEMHANDLE_CACHE_SIZE 64

struct am_ze_memhandle_cache;	// opaque since contains rbtree fields
typedef struct am_ze_memhandle_cache *am_ze_memhandle_cache_t;

struct am_oneapi_ze_ipc_info {
	uint32_t handle;  /* GEM handle or file descriptor */
	uint8_t alloc_type; /* allocation type */
};
typedef struct am_oneapi_ze_ipc_info *am_oneapi_ze_ipc_info_t;

psm2_error_t am_ze_memhandle_cache_alloc(am_ze_memhandle_cache_t *cachep,
										uint32_t memcache_size,
 										psm2_mq_stats_t *stats);

void *
am_ze_memhandle_acquire(am_ze_memhandle_cache_t cache,
			uintptr_t sbuf, uint32_t handle,
			psm2_epaddr_t epaddr, int device_index,
			uint64_t alloc_id, uint8_t alloc_type);
void
am_ze_memhandle_release(am_ze_memhandle_cache_t cache, void *buf_ptr);

void am_ze_memhandle_cache_free(am_ze_memhandle_cache_t cache);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _AM_ONEAPI_MEMHANDLE_H */

#endif /* PSM_ONEAPI */

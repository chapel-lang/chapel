/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

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

  Copyright(c) 2016 Intel Corporation.

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

#ifdef PSM_CUDA

#ifndef _AM_CUDA_MEMHANDLE_CACHE_H
#define _AM_CUDA_MEMHANDLE_CACHE_H

#include "psm_user.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CUDA_MEMHANDLE_CACHE_SIZE 64

struct am_cuda_memhandle_cache;	// opaque since contains rbtree fields
typedef struct am_cuda_memhandle_cache *am_cuda_memhandle_cache_t;

psm2_error_t am_cuda_memhandle_cache_alloc(am_cuda_memhandle_cache_t *cachep,
										uint32_t memcache_size,
										psm2_mq_stats_t *stats);

CUdeviceptr
am_cuda_memhandle_acquire(am_cuda_memhandle_cache_t cache,
				uintptr_t sbuf, CUipcMemHandle* handle,
				psm2_epid_t epid);
void
am_cuda_memhandle_release(am_cuda_memhandle_cache_t cache,
				CUdeviceptr cuda_ipc_dev_ptr);

void am_cuda_memhandle_cache_free(am_cuda_memhandle_cache_t cache);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _AM_CUDA_MEMHANDLE_CACHE_H */

#endif /* PSM_CUDA */

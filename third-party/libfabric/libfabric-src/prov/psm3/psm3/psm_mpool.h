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

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#ifndef _PSMI_IN_USER_H
#error psm_mpool.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef PSM_MPOOL_H
#define PSM_MPOOL_H

/* mpool flags */
#define PSMI_MPOOL_ALIGN_CACHE	0x1
#define PSMI_MPOOL_ALIGN_PAGE   0x2
#define PSMI_MPOOL_NOGENERATION 0x4

/* Backwards compatibility */
#define PSMI_MPOOL_ALIGN	PSMI_MPOOL_ALIGN_CACHE

typedef struct mpool *mpool_t;
typedef void (*non_empty_callback_fn_t) (void *context);
typedef void (*alloc_dealloc_callback_fn_t) (int is_alloc, void *context,
					     void *chunk);

mpool_t
MOCKABLE(psm3_mpool_create)(size_t obj_size, uint32_t num_obj_per_chunk,
			  uint32_t num_obj_max_total, int flags,
			  psmi_memtype_t statstype,
			  non_empty_callback_fn_t cb, void *context);
MOCK_DCL_EPILOGUE(psm3_mpool_create);

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
mpool_t psm3_mpool_create_for_gpu(size_t obj_size, uint32_t num_obj_per_chunk,
				  uint32_t num_obj_max_total, int flags,
				  psmi_memtype_t statstype,
				  non_empty_callback_fn_t cb, void *context,
				  alloc_dealloc_callback_fn_t ad_cb,
				  void *ad_context);
#endif

void psm3_mpool_destroy(mpool_t mp);

void
MOCKABLE(psm3_mpool_get_obj_info)(mpool_t mp, uint32_t *num_obj_per_chunk,
			     uint32_t *num_obj_max_total);
MOCK_DCL_EPILOGUE(psm3_mpool_get_obj_info);

void *psm3_mpool_get(mpool_t mp);
void psm3_mpool_put(void *obj);

int psm3_mpool_get_obj_index(void *obj);
uint32_t psm3_mpool_get_obj_gen_count(void *obj);
int psm3_mpool_get_obj_index_gen_count(void *obj,
				       uint32_t *index, uint32_t *gen_count);

void *psm3_mpool_find_obj_by_index(mpool_t mp, int index);

#endif

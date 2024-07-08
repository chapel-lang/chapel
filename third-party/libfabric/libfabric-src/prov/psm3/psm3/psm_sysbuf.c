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

#include "psm_user.h"
#include "psm_mq_internal.h"

/*
 *
 * System buffer (unexpected message) allocator
 *
 */

#define MM_FLAG_NONE  0
#define MM_FLAG_TRANSIENT  0x1

struct psmi_mem_block_ctrl {
	union {
		psmi_mem_ctrl_t *mem_handler;
		struct psmi_mem_block_ctrl *next;
	};
};


/* Per MQ allocators */
void psm3_mq_sysbuf_init(psm2_mq_t mq)
{
    int i;
    // sysbuf is used for unexpected eager messages in nic, shm and self
    // for self, unexpected is a courtesy to bad apps, app should always post
    //    recv before send when sendint to self.
    // for nic, eager is only messages below rendezvous threshold.
    //    In TCP and CPU jobs threshold can be larger.  TCP allows up to 256K.
    //    Typical verbs rendezvous threshold is 8000-64K bytes, with GPU
    //    tending to use a lower threshold as GPU copies are expensive.
    // for shm, GPU messages use rendezvous anytime GPU supports Scale-Up
    //    GPU to GPU comms, such as xeLink or nvLink.
    // A message which exceeds largest block_size[], will have a temporary
    // sysbuf allocated and freed. For CPU this is ok as malloc is not
    // terribly expensive.  However for GPU, the subsequent copy will pay
    // a GPU DMA registration cost in Cuda or Level Zero, so it is best to
    // avoid temporary buffers.  Fortunately GPU apps tend to have fewer
    // processes per node and hence more available CPU memory to hold the
    // buffers.
    //
    // So for GPU jobs, we allow a few larger block sizes just in case
    // rendezvous threshold is set high or TCP is being used with a large
    // eager message size (aka PSM3_MTU).
    // replenishing_rate is how many we add to pool at a time, there is
    // no upper bound to the pool.
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
    uint32_t gpu_block_sizes[] = {256, 512, 1024, 2048, 4096, 8192, 65536, 262144, (uint32_t)-1};
    uint32_t gpu_replenishing_rate[] = {128, 64, 32, 16, 8, 4, 2, 2, 0};
    uint32_t block_sizes[] = {256, 512, 1024, 2048, 4096, 8192, (uint32_t)-1, (uint32_t)-1,  (uint32_t)-1};
    uint32_t replenishing_rate[] = {128, 64, 32, 16, 8, 4, 0, 0, 0};
#else
    uint32_t block_sizes[] = {256, 512, 1024, 2048, 4096, 8192, (uint32_t)-1};
    uint32_t replenishing_rate[] = {128, 64, 32, 16, 8, 4, 0};
#endif

    if (mq->mem_ctrl_is_init)
        return;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
    if (PSMI_IS_GPU_ENABLED) {
        memcpy(block_sizes, gpu_block_sizes, sizeof(block_sizes));
        memcpy(replenishing_rate, gpu_replenishing_rate, sizeof(replenishing_rate));
    }
#endif
    mq->mem_ctrl_is_init = 1;

    for (i=0; i < MM_NUM_OF_POOLS; i++) {
        mq->handler_index[i].block_size = block_sizes[i];
        mq->handler_index[i].current_available = 0;
        mq->handler_index[i].free_list = NULL;
        mq->handler_index[i].total_alloc = 0;
        mq->handler_index[i].replenishing_rate = replenishing_rate[i];

        if (block_sizes[i] == -1) {
            psmi_assert_always(replenishing_rate[i] == 0);
            mq->handler_index[i].flags = MM_FLAG_TRANSIENT;
        }
        else {
            psmi_assert_always(replenishing_rate[i] > 0);
            mq->handler_index[i].flags = MM_FLAG_NONE;
        }
    }

    /* Hit once on each block size so we have a pool that's allocated */
    for (i=0; i < MM_NUM_OF_POOLS; i++) {
        void *ptr;
        if (block_sizes[i] == -1)
            continue;
        ptr = psm3_mq_sysbuf_alloc(mq, block_sizes[i]);
        psm3_mq_sysbuf_free(mq, ptr);
    }
    // undo counters from psm3_mq_sysbuf_alloc during init
    mq->stats.rx_sysbuf_num = 0;
    mq->stats.rx_sysbuf_bytes  = 0;
}

void psm3_mq_sysbuf_fini(psm2_mq_t mq)  // free all buffers that is currently not used
{
    struct psmi_mem_block_ctrl *block;
    int i;

    if (mq->mem_ctrl_is_init == 0)
        return;

    for (i=0; i < MM_NUM_OF_POOLS; i++) {
        while ((block = mq->handler_index[i].free_list) != NULL) {
            mq->handler_index[i].free_list = block->next;
#if defined(PSM_CUDA) && !defined(PSM_NO_CUDA_REGISTER)
            if (PSMI_IS_GPU_ENABLED && cu_ctxt) {
                /* ignore NOT_REGISTERED in case cuda initialized late */
                /* ignore other errors as context could be destroyed before this */
                CUresult cudaerr;
                //PSMI_CUDA_CALL_EXCEPT(CUDA_ERROR_HOST_MEMORY_NOT_REGISTERED,
                //               cuMemHostUnregister, block);
                psmi_count_cuMemHostUnregister++;
                cudaerr = psmi_cuMemHostUnregister(block);
                if (cudaerr) {
                    const char *pStr = NULL;
                    psmi_count_cuGetErrorString++;
                    psmi_cuGetErrorString(cudaerr, &pStr);
                    _HFI_DBG("CUDA failure: cuMemHostUnregister returned %d: %s\n",
                            cudaerr, pStr?pStr:"Unknown");
                }
            }
#endif
#if defined(PSM_ONEAPI) && !defined(PSM3_NO_ONEAPI_IMPORT)
            if (PSMI_IS_GPU_ENABLED) {
                ze_result_t result;
                //PSMI_ONEAPI_ZE_CALL(zexDriverReleaseImportedPointer, ze_driver, block);
                psmi_count_zexDriverReleaseImportedPointer++;
                result = psmi_zexDriverReleaseImportedPointer(ze_driver,
                        block);
                if (result != ZE_RESULT_SUCCESS) {
                    _HFI_DBG("OneAPI Level Zero failure: zexDriverReleaseImportedPointer returned %d: %s\n", result, psmi_oneapi_ze_result_to_string(result));
                }
            }
#endif
            psmi_free(block);
        }
    }
    mq->mem_ctrl_is_init = 0;
}

void psm3_mq_sysbuf_getinfo(psm2_mq_t mq, char *buf, size_t len)
{
    snprintf(buf, len-1, "Sysbuf consumption: %"PRIu64" bytes\n",
             mq->mem_ctrl_total_bytes);
    buf[len-1] = '\0';
    return;
}

void *psm3_mq_sysbuf_alloc(psm2_mq_t mq, uint32_t alloc_size)
{
    psmi_mem_ctrl_t *mm_handler = mq->handler_index;
    struct psmi_mem_block_ctrl *new_block;
    int replenishing;

    /* There is a timing race with ips initialization, fix later.
 *      * XXX */
    if (!mq->mem_ctrl_is_init)
        psm3_mq_sysbuf_init(mq);

    mq->stats.rx_sysbuf_num++;
    mq->stats.rx_sysbuf_bytes += alloc_size;

    while (mm_handler->block_size < alloc_size)
        mm_handler++;

    replenishing = mm_handler->replenishing_rate;

    if (mm_handler->current_available == 0) { // allocate more buffers
        if (mm_handler->flags & MM_FLAG_TRANSIENT) {
            uint32_t newsz = alloc_size + sizeof(struct psmi_mem_block_ctrl);
            new_block = psmi_malloc(mq->ep, UNEXPECTED_BUFFERS, newsz);

            if (new_block) {
#if defined(PSM_CUDA) && !defined(PSM_NO_CUDA_REGISTER)
                // for transient buffers, no use Importing, adds cost for
                // CPU copy, just pay GPU cost on the copy, we use once & free
                //if (PSMI_IS_GPU_ENABLED && check_have_cuda_ctxt())
                //    PSMI_CUDA_CALL(cuMemHostRegister, new_block, newsz,
                //                   CU_MEMHOSTALLOC_PORTABLE);
#endif
#if defined(PSM_ONEAPI) && !defined(PSM3_NO_ONEAPI_IMPORT)
                // for transient buffers, no use Importing, adds cost for
                // CPU copy, just pay GPU cost on the copy, we use once & free
                //if (PSMI_IS_GPU_ENABLED)
                //    PSMI_ONEAPI_ZE_CALL(zexDriverImportExternalPointer, ze_driver,
                //                    new_block, newsz);
#endif
                new_block->mem_handler = mm_handler;
                new_block++;
                mm_handler->total_alloc++;
                mq->mem_ctrl_total_bytes += newsz;
            }
            return new_block;
        }

        do {
            uint32_t newsz = mm_handler->block_size + sizeof(struct psmi_mem_block_ctrl);

            new_block = psmi_malloc(mq->ep, UNEXPECTED_BUFFERS, newsz);

            if (new_block) {
#if defined(PSM_CUDA) && !defined(PSM_NO_CUDA_REGISTER)
                // By registering memory with Cuds, we make
                // cuMemcpy* run faster for copies between
                // GPU and this sysbuf
                if (PSMI_IS_GPU_ENABLED && check_have_cuda_ctxt())
                    PSMI_CUDA_CALL(cuMemHostRegister, new_block, newsz,
                                   CU_MEMHOSTALLOC_PORTABLE);
#endif
#if defined(PSM_ONEAPI) && !defined(PSM3_NO_ONEAPI_IMPORT)
                // By registering memory with Level Zero, we make
                // zeCommandListAppendMemoryCopy run faster for copies between
                // GPU and this sysbuf
                if (PSMI_IS_GPU_ENABLED)
                    PSMI_ONEAPI_ZE_CALL(zexDriverImportExternalPointer, ze_driver,
                                    new_block, newsz);
#endif
                mm_handler->current_available++;
                mm_handler->total_alloc++;
                mq->mem_ctrl_total_bytes += newsz;

                new_block->next = mm_handler->free_list;
                mm_handler->free_list = new_block;
            }

        } while (--replenishing && new_block);
    }

    if (mm_handler->current_available) {
        mm_handler->current_available--;

        new_block = mm_handler->free_list;
        mm_handler->free_list = new_block->next;

        new_block->mem_handler = mm_handler;
        new_block++;

        return new_block;
    }
    return NULL;
}

void psm3_mq_sysbuf_free(psm2_mq_t mq, void * mem_to_free)
{
    struct psmi_mem_block_ctrl * block_to_free;
    psmi_mem_ctrl_t *mm_handler;

    psmi_assert_always(mq->mem_ctrl_is_init);

    block_to_free = (struct psmi_mem_block_ctrl *)mem_to_free - 1;
    mm_handler = block_to_free->mem_handler;

    if (mm_handler->flags & MM_FLAG_TRANSIENT) {
#if defined(PSM_CUDA) && !defined(PSM_NO_CUDA_REGISTER)
        // for transient buffers, no use Importing, adds cost for
        // CPU copy, just pay GPU cost on the copy, we use once & free
        //if (PSMI_IS_GPU_ENABLED && cu_ctxt) {
        //        /* ignore NOT_REGISTERED in case cuda initialized late */
        //        CUresult cudaerr;
        //        PSMI_CUDA_CALL_EXCEPT(CUDA_ERROR_HOST_MEMORY_NOT_REGISTERED,
        //                        cuMemHostUnregister, block_to_free);
        //}
#endif
#if defined(PSM_ONEAPI) && !defined(PSM3_NO_ONEAPI_IMPORT)
        // for transient buffers, no use Importing, adds cost for
        // CPU copy, just pay GPU cost on the copy, we use once & free
        //if (PSMI_IS_GPU_ENABLED)
        //    PSMI_ONEAPI_ZE_CALL(zexDriverReleaseImportedPointer, ze_driver, block_to_free);
#endif
        psmi_free(block_to_free);
    } else {
        block_to_free->next = mm_handler->free_list;
        mm_handler->free_list = block_to_free;
        mm_handler->current_available++;
    }

    return;
}

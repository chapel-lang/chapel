/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2017 Intel Corporation.

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

  Copyright(c) 2017 Intel Corporation.

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

/* Copyright (c) 2003-2017 Intel Corporation. All rights reserved. */

#ifndef IPS_SPIO_H
#define IPS_SPIO_H

#include "psm_user.h"

#define IPS_CTXT_RESET_MAX	1000	/* max send context reset */
struct ips_spio;
struct ptl;
struct ips_proto;
struct ips_flow;

/* 64B move instruction support */
#define AVX512F_BIT		16	/* level 07h, ebx */
/* 32B move instruction support */
#define AVX2_BIT		 5	/* level 07h, ebx */
/* 16B move instruction support */
#define SSE2_BIT		26	/* level 01h, edx */

typedef
void (*ips_spio_blockcpy_fn_t)(volatile uint64_t *dest,
				const uint64_t *src, uint32_t nblock);
#ifdef PSM_AVX512
void hfi_pio_blockcpy_512(volatile uint64_t *dest,
				const uint64_t *src, uint32_t nblock);
#endif
void hfi_pio_blockcpy_256(volatile uint64_t *dest,
				const uint64_t *src, uint32_t nblock);
void hfi_pio_blockcpy_128(volatile uint64_t *dest,
				const uint64_t *src, uint32_t nblock);
void hfi_pio_blockcpy_64(volatile uint64_t *dest,
				const uint64_t *src, uint32_t nblock);



static inline psm2_error_t ips_spio_transfer_frame(struct ips_proto *proto,
				struct ips_flow *flow, struct ips_scb *scb,
				uint32_t *payload, uint32_t length,
				uint32_t isCtrlMsg, uint32_t cksum_valid,
				uint32_t cksum
#ifdef PSM_CUDA
				, uint32_t is_cuda_payload
#endif
);

static psm2_error_t ips_spio_process_events(const struct ptl *ptl);

#define SPIO_CREDITS_Counter(value)       (((value) >> 0) & 0x7FF)
#define SPIO_CREDITS_Status(value)        (((value) >> 11) & 0x1)
#define SPIO_CREDITS_DueToPbc(value)      (((value) >> 12) & 0x1)
#define SPIO_CREDITS_DueToTheshold(value) (((value) >> 13) & 0x1)
#define SPIO_CREDITS_DueToErr(value)      (((value) >> 14) & 0x1)
#define SPIO_CREDITS_DueToForce(value)    (((value) >> 15) & 0x1)
struct ips_spio_credits {
/* don't use bit operation for performance reason,
 * using above macro instead.
	uint16_t	Counter:11;
	uint16_t	Status:1;
	uint16_t	CreditReturnDueToPbc:1;
	uint16_t	CreditReturnDueToThreshold:1;
	uint16_t	CreditReturnDueToErr:1;
	uint16_t	CreditReturnDueToForce:1;
*/
	union {
		struct {
			uint16_t value;
			uint16_t pad0;
			uint32_t pad1;
		};
		uint64_t credit_return;
	};
};

struct ips_spio_ctrl {
	/* credit return lock for context sharing */
	pthread_spinlock_t spio_ctrl_lock;

	/* PIO write in progress for context sharing */
	volatile uint16_t spio_write_in_progress;
	/* send context reset count */
	volatile uint16_t spio_reset_count;
	/* HFI frozen count, shared copy */
	volatile uint16_t spio_frozen_count;

	volatile uint16_t spio_available_blocks;
	volatile uint16_t spio_block_index;
	volatile uint16_t spio_fill_counter;
	volatile struct ips_spio_credits spio_credits;
} __attribute__ ((aligned(64)));

struct ips_spio {
	const psmi_context_t *context;
	struct ptl *ptl;
	uint16_t unit_id;
	uint16_t portnum;

	pthread_spinlock_t spio_lock;	/* thread lock */
	volatile __le64 *spio_credits_addr __attribute__ ((aligned(64)));
	volatile uint64_t *spio_bufbase_sop;
	volatile uint64_t *spio_bufbase;
	volatile struct ips_spio_ctrl *spio_ctrl;

	uint16_t spio_frozen_count;	/* local copy */
	uint16_t spio_total_blocks;
	uint16_t spio_block_index;

	uint32_t spio_consecutive_failures;
	uint64_t spio_num_stall;
	uint64_t spio_num_stall_total;
	uint64_t spio_next_stall_warning;
	uint64_t spio_last_stall_cyc;
	uint64_t spio_init_cyc;

	psm2_error_t (*spio_reset_hfi)(struct ips_spio *ctrl);
	psm2_error_t (*spio_credit_return_update)(struct ips_spio *ctrl);

	/* copying routines based on block size */
	ips_spio_blockcpy_fn_t spio_blockcpy_med;
	ips_spio_blockcpy_fn_t spio_blockcpy_large;

};

#endif /* IPS_SPIO_H */

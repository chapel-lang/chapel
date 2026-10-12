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

#ifndef _IPS_TIDFLOW_H
#define _IPS_TIDFLOW_H

#include "psm_user.h"

#define TF_ADD 1	// enable code for tidflow statistics

struct ips_tf;
struct ips_protoexp;

typedef void (*ips_tf_avail_cb_fn_t) (struct ips_tf *, void *context);
typedef enum {
	TF_STATE_INVALID = 0,
	TF_STATE_ALLOCATED = 1,
	TF_STATE_DEALLOCATED = 2
} tf_state_t;

struct ips_tf_entry {
	tf_state_t state;
	uint32_t tf_idx;
	uint32_t next_gen;
	uint32_t next_free;
};

struct ips_tf_ctrl {
	uint32_t tf_num_max;
	uint32_t tf_num_avail;
	uint32_t tf_head;
	struct ips_tf_entry tf[HFI_TF_NFLOWS];
} __attribute__ ((aligned(64)));

struct ips_tf {
	ips_tf_avail_cb_fn_t tf_avail_cb;
	void *tf_avail_context;
	struct ips_tf_ctrl *tf_ctrl;

	uint64_t tf_num_total;
	uint32_t tf_num_inuse;
	uint32_t tf_gen_mask;

	/* Pointer to array of size HFI_TF_NFLOWS */
	struct ips_tid_recv_desc *tidrecvc;
};

PSMI_ALWAYS_INLINE(int ips_tf_available(struct ips_tf *tf))
{
	if (tf->tf_ctrl->tf_num_avail == 0) {
		if (tf->tf_ctrl->tf_num_max == tf->tf_num_inuse)
			return -1;
		else
			return 0;
	}

	return tf->tf_ctrl->tf_num_avail;
}

psm2_error_t psm3_ips_tf_init(struct ips_protoexp *protoexp,
			struct ips_tf *tfc,
			ips_tf_avail_cb_fn_t cb);
psm2_error_t psm3_ips_tf_fini(struct ips_tf *tfc);

/* Allocate a tidflow */
psm2_error_t psm3_ips_tf_allocate(struct ips_tf *tfc,
			struct ips_tid_recv_desc **tidrecvc);

/* Deallocate a tidflow */
psm2_error_t psm3_ips_tf_deallocate(struct ips_tf *tfc, uint32_t tf_idx, int used);

#endif

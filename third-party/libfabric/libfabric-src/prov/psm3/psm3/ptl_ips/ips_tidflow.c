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

#include "psm_user.h"
#include "psm2_hal.h"
#include "ips_proto.h"
#include "ips_expected_proto.h"
#include "ips_tidflow.h"

// TBD - this is only needed for UD w/RNDV
// could omit from build for UDP.
// Once that is done, could #ifdef PSMI_STATSTYPE_RDMA declaration
//
// TBD - move this into HAL and have init, fini, alloc, dealloc
// as new HAL APIs or maybe once finish working on expected.c whole
// tf ends up internal to HAL in which case init and fini calls
// in proto.c could be moved into the related HAL functions too

psm2_error_t psm3_ips_tf_init(struct ips_protoexp *protoexp,
			struct ips_tf *tfc,
			ips_tf_avail_cb_fn_t cb)
{
	int tf_idx;
	psm2_ep_t ep = protoexp->proto->ep;

#if TF_ADD
	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECL("tidflow_update_count",
				"total tid flow allocate calls",
				MPSPAWN_STATS_REDUCTION_ALL,
				NULL, &tfc->tf_num_total),
	};
#endif

	tfc->tf_num_total = 0;
	tfc->tf_num_inuse = 0;
	tfc->tf_avail_cb = cb;
	tfc->tf_avail_context = (void *)protoexp;
	tfc->tf_gen_mask = 0xFFFFF;

	/* Allocate and Initialize tidrecvc array. */
	tfc->tidrecvc = (struct ips_tid_recv_desc *)
		psmi_calloc(ep, UNDEFINED, 1,
			sizeof(struct ips_tid_recv_desc)*HFI_TF_NFLOWS);
	if (tfc->tidrecvc == NULL)
		return PSM2_NO_MEMORY;

	for (tf_idx = 0; tf_idx < HFI_TF_NFLOWS; tf_idx++) {
		tfc->tidrecvc[tf_idx].protoexp = protoexp;
		tfc->tidrecvc[tf_idx].rdescid._desc_idx = tf_idx;
		tfc->tidrecvc[tf_idx].rdescid._desc_genc = tf_idx;
	}

	tfc->tf_ctrl = (struct ips_tf_ctrl *)psmi_calloc(ep, UNDEFINED, 1,
			sizeof(struct ips_tf_ctrl));
	if (tfc->tf_ctrl == NULL)
		return PSM2_NO_MEMORY;

	/*
	 * Only the master process can initialize.
	 */
	tfc->tf_ctrl->tf_num_max = HFI_TF_NFLOWS;
	tfc->tf_ctrl->tf_num_avail = HFI_TF_NFLOWS;

	for (tf_idx = 0; tf_idx < HFI_TF_NFLOWS; tf_idx++) {
// USE_RC TBD this is bizzare.
// For UD/UDP mode it crashes at next_free assignment below on some systems
// but adding this print or moving next_free assignment to separate
// loop works fine.  Really odd if this is a compiler issue, but
// I don't see any other reason.  We should be single threaded here
// enabling the empty call to tidflow_reset doesn't help
// Can't explain crash nor workaround
		/* Update flow state */
		tfc->tf_ctrl->tf[tf_idx].state = TF_STATE_DEALLOCATED;
		tfc->tf_ctrl->tf[tf_idx].tf_idx = tf_idx;
		tfc->tf_ctrl->tf[tf_idx].next_gen = 0;
#if 0
		tfc->tf_ctrl->tf[tf_idx].next_free = tf_idx + 1;
#endif

	}
#if 1
	for (tf_idx = 0; tf_idx < HFI_TF_NFLOWS; tf_idx++) {
		tfc->tf_ctrl->tf[tf_idx].next_free = tf_idx + 1;
	}
#endif
	tfc->tf_ctrl->tf_head = 0;

#if TF_ADD
	/* TF_ADD: Add a new stats type for tid flows in psm_stats.h */
	return psm3_stats_register_type("TID_Flow_Statistics",
		"RDMA Transaction Flow statistics for an endpoint in the "
		"process.\n"
		"For each inbound RDMA chunk a Transaction ID Flow is "
		"allocated to identify the transfer and to ensure the "
		"receiver has sufficient resources to handle the RDMA.",
					PSMI_STATSTYPE_RDMA,
					entries,
					PSMI_HOWMANY(entries),
					psm3_epid_fmt_internal(ep->epid, 0), tfc,
					ep->dev_name);
#else
	return PSM2_OK;
#endif
}

psm2_error_t psm3_ips_tf_fini(struct ips_tf *tfc)
{
	psm3_stats_deregister_type(PSMI_STATSTYPE_RDMA, tfc);
		psmi_free(tfc->tf_ctrl);
	psmi_free(tfc->tidrecvc);
	return PSM2_OK;
}

/* Allocate a tidflow */
psm2_error_t psm3_ips_tf_allocate(struct ips_tf *tfc,
		struct ips_tid_recv_desc **tidrecvc)
{
	struct ips_tf_ctrl *ctrl = tfc->tf_ctrl;
	struct ips_tf_entry *entry;

	if (!ctrl->tf_num_avail) {
		psmi_assert(ctrl->tf_head == HFI_TF_NFLOWS);
		*tidrecvc = NULL;
		return PSM2_EP_NO_RESOURCES;
	}

	entry = &ctrl->tf[ctrl->tf_head];
	ctrl->tf_head = entry->next_free;
	ctrl->tf_num_avail--;

	tfc->tf_num_total++;
	tfc->tf_num_inuse++;

	psmi_assert(entry->state == TF_STATE_DEALLOCATED);
	entry->state = TF_STATE_ALLOCATED;

	*tidrecvc = &(tfc->tidrecvc[entry->tf_idx]);
	/* initial tidflow generation */
	(*tidrecvc)->tidflow_active_gen = entry->next_gen;

	psmi_assert((*tidrecvc)->rdescid._desc_idx == entry->tf_idx);
	psmi_assert_always(entry->next_gen < tfc->tf_gen_mask);

	return PSM2_OK;
}

/* Deallocate a tidflow */
psm2_error_t psm3_ips_tf_deallocate(struct ips_tf *tfc, uint32_t tf_idx, int used)
{
	struct ips_tf_ctrl *ctrl = tfc->tf_ctrl;
	struct ips_tf_entry *entry;

	psmi_assert(tf_idx < HFI_TF_NFLOWS);

	entry = &ctrl->tf[tf_idx];
	psmi_assert(entry->state == TF_STATE_ALLOCATED);
	entry->state = TF_STATE_DEALLOCATED;

	if (used) {
		entry->next_gen++;
		if (entry->next_gen == tfc->tf_gen_mask)
			entry->next_gen = 0;
		/* we track all 32 bits to improve ability for err_chk_rdma
		 * to identify completed requests vs rdescid reuse
		 * however only low 16 bits are used in RDMA immediate data
		 */
		tfc->tidrecvc[tf_idx].rdescid.u32w1++;
	}

	entry->next_free = ctrl->tf_head;
	ctrl->tf_head = tf_idx;
	ctrl->tf_num_avail++;

	tfc->tf_num_inuse--;
	/* If an available callback is registered invoke it */
	if (((tfc->tf_num_inuse + 1) == ctrl->tf_num_max) && tfc->tf_avail_cb)
		tfc->tf_avail_cb(tfc, tfc->tf_avail_context);

	return PSM2_OK;
}

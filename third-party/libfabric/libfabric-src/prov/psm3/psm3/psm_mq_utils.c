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

/* Copyright (c) 2003-2015 Intel Corporation. All rights reserved. */

#include "psm_user.h"
#include "psm_mq_internal.h"

/*
 *
 * MQ request allocator
 *
 */

psm2_mq_req_t MOCKABLE(psm3_mq_req_alloc)(psm2_mq_t mq, uint32_t type)
{
	psm2_mq_req_t req;

	psmi_assert(type == MQE_TYPE_RECV || type == MQE_TYPE_SEND);

	if (type == MQE_TYPE_SEND)
		req = psm3_mpool_get(mq->sreq_pool);
	else
		req = psm3_mpool_get(mq->rreq_pool);

	if_pt(req != NULL) {
		memset(req, 0, sizeof(struct psm2_mq_req));
		req->type = type;
		req->state = MQ_STATE_FREE;
		req->mq = mq;

		return req;
	} else {	/* we're out of reqs */
		int issend = (type == MQE_TYPE_SEND);
		uint32_t reqmax;
		psm3_mpool_get_obj_info(issend ? mq->sreq_pool : mq->rreq_pool,
					NULL, &reqmax);

		psm3_handle_error(PSMI_EP_NORETURN, PSM2_PARAM_ERR,
				  "Exhausted %d MQ %s request descriptors, which usually indicates "
				  "a user program error or insufficient request descriptors (%s=%d)",
				  reqmax, issend ? "isend" : "irecv",
				  issend ? "PSM3_MQ_SENDREQS_MAX" :
				  "PSM3_MQ_RECVREQS_MAX", reqmax);
		return NULL;
	}
}
MOCK_DEF_EPILOGUE(psm3_mq_req_alloc);

psm2_error_t psm3_mq_req_init(psm2_mq_t mq)
{
	psm2_mq_req_t warmup_req;
	psm2_error_t err = PSM2_OK;

	_HFI_VDBG("mq element sizes are %d bytes\n",
		  (int)sizeof(struct psm2_mq_req));

	/*
	 * Send MQ requests
	 */
	{
		struct psmi_rlimit_mpool rlim = MQ_SENDREQ_LIMITS;
		uint32_t maxsz, chunksz;

		if ((err =
		     psm3_parse_mpool_env(mq, 0, &rlim, &maxsz, &chunksz)))
			goto fail;

		if ((mq->sreq_pool =
		     psm3_mpool_create(sizeof(struct psm2_mq_req), chunksz,
				       maxsz, 0, DESCRIPTORS, NULL,
				       NULL)) == NULL) {
			err = PSM2_NO_MEMORY;
			goto fail;
		}
	}

	/*
	 * Receive MQ requests
	 */
	{
		struct psmi_rlimit_mpool rlim = MQ_RECVREQ_LIMITS;
		uint32_t maxsz, chunksz;

		if ((err =
		     psm3_parse_mpool_env(mq, 0, &rlim, &maxsz, &chunksz)))
			goto fail;
		if ((mq->rreq_pool =
			psm3_mpool_create(sizeof(struct psm2_mq_req), chunksz,
				       maxsz, 0, DESCRIPTORS, NULL,
				       NULL)) == NULL) {
			err = PSM2_NO_MEMORY;
			goto fail;
		}
	}

	/* Warm up the allocators */
	warmup_req = psm3_mq_req_alloc(mq, MQE_TYPE_RECV);
	psmi_assert_always(warmup_req != NULL);
	psm3_mq_req_free_internal(warmup_req);

	warmup_req = psm3_mq_req_alloc(mq, MQE_TYPE_SEND);
	psmi_assert_always(warmup_req != NULL);
	psm3_mq_req_free_internal(warmup_req);

fail:
	return err;
}

psm2_error_t psm3_mq_req_fini(psm2_mq_t mq)
{
	psm3_mpool_destroy(mq->rreq_pool);
	psm3_mpool_destroy(mq->sreq_pool);
	return PSM2_OK;
}


#if 0 // unused code, specific to QLogic MPI
/*
 * Hooks to plug into QLogic MPI stats
 */

static
void psm3_mq_stats_callback(struct mpspawn_stats_req_args *args)
{
	uint64_t *entry = args->stats;
	psm2_mq_t mq = (psm2_mq_t) args->context;
	psm2_mq_stats_t mqstats;

	psm3_mq_get_stats(sizeof(mqstats), mq, &mqstats);

	if (args->num < 8)
		return;

	entry[0] = mqstats.tx_eager_num;
	entry[1] = mqstats.tx_eager_bytes;
	entry[2] = mqstats.tx_rndv_num;
	entry[3] = mqstats.tx_rndv_bytes;

	entry[4] = mqstats.rx_user_num;
	entry[5] = mqstats.rx_user_bytes;
	entry[6] = mqstats.rx_sys_num;
	entry[7] = mqstats.rx_sys_bytes;
}

void psm3_mq_stats_register(psm2_mq_t mq, mpspawn_stats_add_fn add_fn)
{
	char *desc[8];
	uint16_t flags[8];
	int i;
	struct mpspawn_stats_add_args mp_add;
	/*
	 * Hardcode flags until we correctly move mpspawn to its own repo.
	 * flags[i] = MPSPAWN_REDUCTION_MAX | MPSPAWN_REDUCTION_MIN;
	 */
	for (i = 0; i < 8; i++)
		flags[i] = MPSPAWN_STATS_REDUCTION_ALL;

	desc[0] = "Eager count sent";
	desc[1] = "Eager bytes sent";
	desc[2] = "Rendezvous count sent";
	desc[3] = "Rendezvous bytes sent";
	desc[4] = "Expected count received";
	desc[5] = "Expected bytes received";
	desc[6] = "Unexpect count received";
	desc[7] = "Unexpect bytes received";

	mp_add.version = MPSPAWN_STATS_VERSION;
	mp_add.num = 8;
	mp_add.header = "MPI Statistics Summary (max,min @ rank)";
	mp_add.req_fn = psm3_mq_stats_callback;
	mp_add.desc = desc;
	mp_add.flags = flags;
	mp_add.context = mq;

	add_fn(&mp_add);
}
#endif

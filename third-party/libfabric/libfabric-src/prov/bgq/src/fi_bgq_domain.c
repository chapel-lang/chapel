/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#include "rdma/bgq/fi_bgq.h"
#include <ofi_enosys.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

#include "rdma/bgq/fi_bgq_spi.h"

uint64_t fi_bgq_global_reception_counter __attribute__((__aligned__(L2_CACHE_LINE_SIZE)));

static int fi_bgq_close_domain(fid_t fid)
{
	int ret;
	struct fi_bgq_domain *bgq_domain =
		container_of(fid, struct fi_bgq_domain, domain_fid);

	ret = fi_bgq_fid_check(fid, FI_CLASS_DOMAIN, "domain");
	if (ret)
		return ret;


	/* close/finalize/deallocate the MU hardware? */

	ret = fi_bgq_finalize_mr_ops(bgq_domain);
	if (ret)
		return ret;

	unsigned i;
	for (i=0; i<bgq_domain->progress.max_threads; ++i) {
		ret = fi_bgq_progress_disable(bgq_domain, i);
		if (ret)
			return ret;
	}

	ret = fi_bgq_progress_fini(bgq_domain);
	if (ret)
		return ret;

	ret = fi_bgq_ref_finalize(&bgq_domain->ref_cnt, "domain");
	if (ret)
		return ret;

	ret = fi_bgq_ref_dec(&bgq_domain->fabric->ref_cnt, "fabric");
	if (ret)
		return ret;


	free(bgq_domain);

	return 0;
}

static struct fi_ops fi_bgq_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_bgq_close_domain,
	.bind		= fi_no_bind,
	.control	= fi_no_control,
	.ops_open	= fi_no_ops_open
};

static struct fi_ops_domain fi_bgq_domain_ops = {
	.size		= sizeof(struct fi_ops_domain),
	.av_open	= fi_bgq_av_open,
	.cq_open	= fi_bgq_cq_open,
	.endpoint	= fi_bgq_endpoint,
	.scalable_ep	= fi_bgq_scalable_ep,
	.cntr_open	= fi_bgq_cntr_open,
	.poll_open	= fi_no_poll_open,
	.stx_ctx	= fi_bgq_stx_context,
	.srx_ctx	= fi_no_srx_context
};

static int fi_bgq_mu_init(struct fi_bgq_domain *bgq_domain,
		struct fi_info *info)
{
	int rc;
	rc = fi_bgq_node_mu_lock_init(&bgq_domain->fabric->node, &bgq_domain->mu.lock);
	if (rc) {
		goto err;
	}
	l2atomic_lock_acquire(&bgq_domain->mu.lock);

	const uint32_t ppn = Kernel_ProcessCount();
	const uint32_t tcoord = Kernel_MyTcoord();
	const uint32_t subgroup_total = BGQ_MU_NUM_REC_FIFO_SUBGROUPS*(BGQ_MU_NUM_REC_FIFO_GROUPS-1);	/* do not consider 17th core subgroups */
	const uint32_t subgroups_per_process = subgroup_total / ppn;
	const uint32_t subgroup_offset = subgroups_per_process * tcoord;
	const uint32_t recfifo_total = BGQ_MU_NUM_REC_FIFOS_PER_GROUP*(BGQ_MU_NUM_REC_FIFO_GROUPS-1);	/* do not mess with 17th core group recfifos */

	/*
	 * Create four mu reception fifos in each of the subgroups "owned" by
	 * this process.
	 */
	uint8_t * memptr;
	size_t nbytes = FI_BGQ_MU_RECFIFO_BYTES * BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP * subgroups_per_process;
	rc = posix_memalign((void**)&memptr, 32, nbytes);
	if (rc) goto err;

	Kernel_MemoryRegion_t mregion;
	rc = Kernel_CreateMemoryRegion(&mregion, (void*)memptr, nbytes);
	if (rc) goto err;

	bgq_domain->rfifo_mem = (void*)memptr;

	bgq_domain->rx.max = 0;
	bgq_domain->rx.count = 0;

	uint32_t n;
	for (n = 0; n < recfifo_total; ++n) {
		bgq_domain->rx.rfifo[n] = NULL;
	}

	const uint32_t subgroups_to_allocate_per_process = ppn == 64 ? 1 : ppn == 32 ? 2 : ppn == 16 ? 4 : ppn == 8 ? 8 : ppn == 4 ? 16 : ppn == 2 ? 32 : 64;
	for (n = 0; n < subgroups_to_allocate_per_process; ++n) {

		const uint32_t requested_subgroup = subgroup_offset + n;

		uint32_t free_fifo_num;
		uint32_t free_fifo_ids[BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP];
		rc = Kernel_QueryRecFifos(requested_subgroup, &free_fifo_num, free_fifo_ids);
		if (rc) goto err;
		if (free_fifo_num < 4) goto err;
		if (free_fifo_ids[0] != 0) goto err;
		if (free_fifo_ids[1] != 1) goto err;
		if (free_fifo_ids[2] != 2) goto err;
		if (free_fifo_ids[3] != 3) goto err;

		Kernel_RecFifoAttributes_t rfifo_attrs[4];
		memset((void*)&rfifo_attrs[0], 0, sizeof(Kernel_RecFifoAttributes_t)*4);
		rc = Kernel_AllocateRecFifos(requested_subgroup,
			&bgq_domain->rfifo_subgroup[requested_subgroup],
			4, free_fifo_ids, rfifo_attrs);
		if (rc) goto err;

		uint32_t i;
		for (i = 0; i < BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP; ++i) {
			rc = Kernel_RecFifoInit(&bgq_domain->rfifo_subgroup[requested_subgroup],
				i,
				&mregion,
				((uint64_t)memptr) - (uint64_t)mregion.BaseVa,
				FI_BGQ_MU_RECFIFO_BYTES - 1);
			if (rc) goto err;

			memptr += FI_BGQ_MU_RECFIFO_BYTES;
		}

		uint64_t shift = (BGQ_MU_NUM_REC_FIFOS_PER_GROUP-1) -
			((requested_subgroup&3)*BGQ_MU_NUM_FIFO_SUBGROUPS);
		rc = Kernel_RecFifoEnable(requested_subgroup>>2, 0x01ULL << shift);
		if (rc) goto err;
		rc = Kernel_RecFifoEnable(requested_subgroup>>2, 0x01ULL << (shift-1));
		if (rc) goto err;
		rc = Kernel_RecFifoEnable(requested_subgroup>>2, 0x01ULL << (shift-2));
		if (rc) goto err;
		rc = Kernel_RecFifoEnable(requested_subgroup>>2, 0x01ULL << (shift-3));
		if (rc) goto err;

		for (i = 0; i< BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP; ++i) {
			bgq_domain->rx.rfifo[requested_subgroup*BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP+i] = 
				 &bgq_domain->rfifo_subgroup[requested_subgroup]._recfifos[i];
		}

		bgq_domain->rx.max += 4;	/* initialized 4 mu reception fifos, 1 mu reception fifo is used in each fi rx ctx */
	}

	bgq_domain->tx.count = 0;

	/* initialize the mu gi barrier */
	bgq_domain->gi.leader_tcoord = bgq_domain->fabric->node.leader_tcoord;
	bgq_domain->gi.is_leader = bgq_domain->fabric->node.is_leader;
	if (bgq_domain->gi.is_leader) {
		rc = MUSPI_GIBarrierInit(&bgq_domain->gi.barrier, 0);
		assert(rc==0);
	}

	bgq_domain->subgroups_per_process = 64 / Kernel_ProcessCount();

	l2atomic_lock_release(&bgq_domain->mu.lock);


	/* global barrier after mu initialization is complete */
	l2atomic_barrier_enter(&bgq_domain->fabric->node.barrier);
	if (bgq_domain->gi.is_leader) {
		rc = MUSPI_GIBarrierEnterAndWait(&bgq_domain->gi.barrier);
		assert(rc==0);
	}
	l2atomic_barrier_enter(&bgq_domain->fabric->node.barrier);

	return 0;
err:
	if (l2atomic_lock_isbusy(&bgq_domain->mu.lock)) {
		l2atomic_lock_release(&bgq_domain->mu.lock);
	}
	return -1;
}

int fi_bgq_alloc_default_domain_attr(struct fi_domain_attr **domain_attr)
{
	struct fi_domain_attr *attr;

	attr = calloc(1, sizeof(*attr));
	if (!attr)
		goto err;

	uint32_t ppn = Kernel_ProcessCount();

	/*
	 * See: fi_bgq_stx_init() for the number of mu injection fifos
	 * allocated for each tx context. Each rx context uses one
	 * mu injection fifo and one mu reception fifo.
	 */
	const unsigned tx_ctx_cnt = (((BGQ_MU_NUM_INJ_FIFO_GROUPS-1) * BGQ_MU_NUM_INJ_FIFOS_PER_GROUP) / 3) / ppn;

	/*
	 * The number of rx contexts on a node is the minimum of:
	 * 1. number of mu injection fifos on the node not used by tx contexts
	 * 2. total number mu reception fifos on the node
	 */
	const unsigned rx_ctx_cnt = MIN((((BGQ_MU_NUM_INJ_FIFO_GROUPS-1) * BGQ_MU_NUM_INJ_FIFOS_PER_GROUP) - (tx_ctx_cnt * ppn)), ((BGQ_MU_NUM_REC_FIFO_GROUPS-1) * BGQ_MU_NUM_REC_FIFOS_PER_GROUP)) / ppn;

	attr->domain		= NULL;
	attr->name		= NULL;

	attr->threading		= FI_THREAD_ENDPOINT;
	attr->control_progress 	= FI_PROGRESS_MANUAL;
	attr->data_progress	= FI_BGQ_FABRIC_DIRECT_PROGRESS;
	attr->resource_mgmt	= FI_RM_DISABLED;
	attr->av_type		= FI_AV_MAP;
	attr->mr_mode		= FI_BGQ_FABRIC_DIRECT_MR;
	attr->mr_key_size 	= 2;			/* 2^16 keys */
	attr->cq_data_size 	= FI_BGQ_REMOTE_CQ_DATA_SIZE;
	attr->cq_cnt		= 128 / ppn;
	attr->ep_cnt		= 1;			/* TODO - what about endpoints that only use a shared receive context and a shared transmit context? */
	attr->tx_ctx_cnt	= tx_ctx_cnt;
	attr->rx_ctx_cnt	= rx_ctx_cnt;

	attr->max_ep_tx_ctx	= ((BGQ_MU_NUM_INJ_FIFO_GROUPS-1) * BGQ_MU_NUM_INJ_FIFOS_PER_GROUP) / ppn / 2;
	attr->max_ep_rx_ctx	= ((BGQ_MU_NUM_REC_FIFO_GROUPS-1) * BGQ_MU_NUM_REC_FIFOS_PER_GROUP) / ppn;

	attr->max_ep_stx_ctx	= attr->max_ep_tx_ctx;
	attr->max_ep_srx_ctx	= 0;	/* TODO - reserve some mu reception fifos for use as shared receive context ... how to address? */
	attr->mr_iov_limit	= 1;
	attr->mr_cnt		= 1 << (attr->mr_key_size << 3);

	*domain_attr = attr;

	return 0;
err:
	*domain_attr = NULL;
	errno = FI_ENOMEM;
	return -1;
}

int fi_bgq_choose_domain(uint64_t caps, struct fi_domain_attr *domain_attr,
			 const struct fi_domain_attr *hints)
{
	if (!domain_attr) {
		goto err;
	}

	*domain_attr = *fi_bgq_global.default_domain_attr;
	/* Set the data progress mode to the option used in the configure.
 	 * Ignore any setting by the application.
 	 */
	domain_attr->data_progress = FI_BGQ_FABRIC_DIRECT_PROGRESS;

	/* Set the mr_mode to the option used in the configure.
 	 * Ignore any setting by the application - the checkinfo should have verified
 	 * it was set to the same setting.
 	 */
	domain_attr->mr_mode = FI_BGQ_FABRIC_DIRECT_MR;
 
	if (hints) {
		if (hints->domain) {
			struct fi_bgq_domain *bgq_domain = bgq_domain = container_of(hints->domain, struct fi_bgq_domain, domain_fid);

			domain_attr->threading		= bgq_domain->threading;
			domain_attr->resource_mgmt	= bgq_domain->resource_mgmt;
			domain_attr->tx_ctx_cnt		= fi_bgq_domain_get_tx_max(bgq_domain);
			domain_attr->rx_ctx_cnt		= fi_bgq_domain_get_rx_max(bgq_domain);
			domain_attr->max_ep_tx_ctx	= fi_bgq_domain_get_tx_max(bgq_domain);
			domain_attr->max_ep_rx_ctx	= fi_bgq_domain_get_rx_max(bgq_domain);
			domain_attr->max_ep_stx_ctx	= fi_bgq_domain_get_tx_max(bgq_domain);

		} else {

			if (hints->threading)		domain_attr->threading = hints->threading;
			if (hints->control_progress)	domain_attr->control_progress = hints->control_progress;
			if (hints->resource_mgmt)	domain_attr->resource_mgmt = hints->resource_mgmt;
			if (hints->av_type)		domain_attr->av_type = hints->av_type;
			if (hints->mr_key_size)		domain_attr->mr_key_size = hints->mr_key_size;
			if (hints->cq_data_size)	domain_attr->cq_data_size = hints->cq_data_size;
			if (hints->cq_cnt)		domain_attr->cq_cnt = hints->cq_cnt;
			if (hints->ep_cnt)		domain_attr->ep_cnt = hints->ep_cnt;
			if (hints->tx_ctx_cnt)		domain_attr->tx_ctx_cnt = hints->tx_ctx_cnt;
			if (hints->rx_ctx_cnt)		domain_attr->rx_ctx_cnt = hints->rx_ctx_cnt;
			if (hints->max_ep_tx_ctx)	domain_attr->max_ep_tx_ctx = hints->max_ep_tx_ctx;
			if (hints->max_ep_rx_ctx)	domain_attr->max_ep_rx_ctx = hints->max_ep_rx_ctx;
			if (hints->max_ep_stx_ctx)	domain_attr->max_ep_stx_ctx = hints->max_ep_stx_ctx;
			if (hints->max_ep_srx_ctx)	domain_attr->max_ep_srx_ctx = hints->max_ep_srx_ctx;
			if (hints->mr_iov_limit)	domain_attr->mr_iov_limit = hints->mr_iov_limit;
		}
	}

	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_AUTO)
		if (Kernel_ProcessCount() > 16) {
			fprintf(stderr,"BGQ Provider configure in FI_PROGRESS_AUTO mode and cannot be run higher than 16 ppn due to need for progress thread\n");
			assert(0);
			exit(1);
		}


	domain_attr->name = strdup(FI_BGQ_PROVIDER_NAME);
	if (!domain_attr->name) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"no memory\n");
		errno = FI_ENOMEM;
		return -errno;
	}

	domain_attr->cq_data_size = FI_BGQ_REMOTE_CQ_DATA_SIZE;

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_check_domain_attr(const struct fi_domain_attr *attr)
{
	switch(attr->threading) {
	case FI_THREAD_UNSPEC:
	case FI_THREAD_SAFE:
	case FI_THREAD_FID:
	case FI_THREAD_ENDPOINT:
	case FI_THREAD_COMPLETION:
	case FI_THREAD_DOMAIN:
		break;
	default:
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"incorrect threading level\n");
		goto err;
	}
	if (attr->control_progress &&
			attr->control_progress != FI_PROGRESS_MANUAL) {
		fprintf(stderr,"BGQ Provider only supports control_progress of FI_PROGRESS_MANUAL\n");
		assert(0);
		exit(1);
	}
	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_AUTO) {
		if (attr->data_progress &&
				attr->data_progress == FI_PROGRESS_MANUAL) {
			fprintf(stderr,"BGQ Provider configured with data progress mode of FI_PROGRESS_AUTO but application specified FI_PROGRESS_MANUAL\n");
			fflush(stderr);
			assert(0);
			exit(1);
		}
	}
	else if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {
		if (attr->data_progress &&
				attr->data_progress == FI_PROGRESS_AUTO) {
			fprintf(stderr,"BGQ Provider configured with data progress mode of FI_PROGRESS_MANUAL but application specified FI_PROGRESS_AUTO\n");
			fflush(stderr);
			assert(0);
			exit(1);
		}
	}
	else {
		fprintf(stderr,"BGQ Provider progress mode not properly configured.\n");
		fflush(stderr);
		assert(0);
		exit(1);
	}

	if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_SCALABLE) {
		if (attr->mr_mode != FI_MR_SCALABLE) {
			fprintf(stderr,"BGQ Provider configured with mr mode of FI_MR_SCALABLE but application specified something else.\n");
			fflush(stderr);
			assert(0);
			exit(1);
		}
	}
	else if (FI_BGQ_FABRIC_DIRECT_MR == FI_MR_BASIC) {
		if (attr->mr_mode != FI_MR_BASIC) {
			fprintf(stderr,"BGQ Provider configured with mr mode of FI_MR_BASIC but application specified something else.\n");
			fflush(stderr);
			assert(0);
			exit(1);
		}
	}
	else {
		fprintf(stderr,"BGQ Provider mr mode not properly configured.\n");
		fflush(stderr);
		assert(0);
		exit(1);
	}
	if (attr->mr_key_size) {
		if (attr->mr_key_size > FI_BGQ_MR_KEY_SIZE) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
					"memory key size too large\n");
			goto err;
		}
	}
	if (attr->cq_data_size) {
		if (attr->cq_data_size > FI_BGQ_REMOTE_CQ_DATA_SIZE) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
					"max cq data supported is %d\n",
					FI_BGQ_REMOTE_CQ_DATA_SIZE);
			goto err;
		}
	}

	return 0;

err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_domain(struct fid_fabric *fabric,
		struct fi_info *info,
		struct fid_domain **dom, void *context)
{
	int ret;
	struct fi_bgq_domain 	*bgq_domain = NULL;
	struct fi_bgq_fabric 	*bgq_fabric =
		container_of(fabric, struct fi_bgq_fabric, fabric_fid);

	if (!info) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"no info supplied\n");
		errno = FI_EINVAL;
		return -errno;
	}

	ret = fi_bgq_fid_check(&fabric->fid, FI_CLASS_FABRIC, "fabric");
	if (ret)
		return ret;

	bgq_domain = calloc(1, sizeof(struct fi_bgq_domain));
	if (!bgq_domain) {
		errno = FI_ENOMEM;
		goto err;
	}

	/* fill in default domain attributes */
	bgq_domain->threading		= fi_bgq_global.default_domain_attr->threading;
	bgq_domain->resource_mgmt	= fi_bgq_global.default_domain_attr->resource_mgmt;

	if (info->domain_attr) {
		if (info->domain_attr->domain) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
					"domain cannot be supplied\n");
			goto err;
		}
		ret = fi_bgq_check_domain_attr(info->domain_attr);
		if (ret)
			goto err;
		bgq_domain->threading = info->domain_attr->threading;
		bgq_domain->resource_mgmt = info->domain_attr->resource_mgmt;
	}

        /* Set the data progress mode to the option used in the configure.
	 * Ignore any setting by the application.
 	 */
        bgq_domain->data_progress = FI_BGQ_FABRIC_DIRECT_PROGRESS;


	uint32_t ppn = Kernel_ProcessCount();
	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_AUTO) {
		uint32_t ppn = Kernel_ProcessCount();
 		if (ppn > 16) {
			fprintf(stderr,"BGQ Provider configure in FI_PROGRESS_AUTO mode and cannot be run higher than 16 ppn due to need for progress thread\n");
			assert(0);
			exit(1);
		}
	}

	bgq_domain->fabric = bgq_fabric;


	Personality_t personality;
	if (Kernel_GetPersonality(&personality, sizeof(Personality_t))) goto err;
	bgq_domain->my_coords.a = personality.Network_Config.Acoord;
	bgq_domain->my_coords.b = personality.Network_Config.Bcoord;
	bgq_domain->my_coords.c = personality.Network_Config.Ccoord;
	bgq_domain->my_coords.d = personality.Network_Config.Dcoord;
	bgq_domain->my_coords.e = personality.Network_Config.Ecoord;
	bgq_domain->my_coords.t = Kernel_MyTcoord();

	bgq_domain->zero.value = 0;
	fi_bgq_cnk_vaddr2paddr((const void *)&bgq_domain->zero.value,
		sizeof(uint64_t), &bgq_domain->zero.paddr);

	bgq_domain->one.value = 1;
	fi_bgq_cnk_vaddr2paddr((const void *)&bgq_domain->one.value,
		sizeof(uint64_t), &bgq_domain->one.paddr);

	ret = fi_bgq_mu_init(bgq_domain, info);
	if (ret) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
				"error initializing the MU\n");
		errno = FI_EOTHER;
		goto err;
	}

	fi_bgq_ref_init(&bgq_fabric->node, &bgq_domain->ref_cnt, "domain");

	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_AUTO) {
		uint32_t ppn = Kernel_ProcessCount();
		fi_bgq_progress_init(bgq_domain, 64/ppn - 1);	/* TODO - what should the "max threads" be? */
		if (0 != fi_bgq_progress_enable(bgq_domain, 0)) {

			/* Unable to start progress threads! */
			fprintf(stderr,"BGQ Provider unable to start progress thread for FI_PROGRESS_AUTO mode\n");
			assert(0);
			exit(1);
		}
	} else {
		fi_bgq_progress_init(bgq_domain, 0);
	}


	bgq_domain->domain_fid.fid.fclass  = FI_CLASS_DOMAIN;
	bgq_domain->domain_fid.fid.context = context;
	bgq_domain->domain_fid.fid.ops     = &fi_bgq_fi_ops;
	bgq_domain->domain_fid.ops	   = &fi_bgq_domain_ops;

	ret = fi_bgq_init_mr_ops(bgq_domain, info);
	if (ret)
		goto err;

	unsigned i = 0;
	for (i = 0; i < FI_BGQ_DOMAIN_MAX_RX_CTX; ++i) {
		bgq_domain->rx.ctx[i] = NULL;
	}

	if (fi_bgq_node_lock_allocate(&bgq_fabric->node, &bgq_domain->lock)) goto err;

	fi_bgq_ref_inc(&bgq_domain->fabric->ref_cnt, "fabric");

	*dom = &bgq_domain->domain_fid;

	return 0;

err:
	fi_bgq_finalize_mr_ops(bgq_domain);
	if (bgq_domain)
		free(bgq_domain);
	return -errno;
}

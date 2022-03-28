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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sched.h>		/* cpu_set */
#include <ctype.h>		/* isalpha */
#include <netdb.h>
//#include <infiniband/verbs.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "psm_user.h"
#include "psm_mq_internal.h"
#include "psm_am_internal.h"
#ifdef RNDV_MOD
#include "psm_rndv_mod.h"
#endif
#include "opa_byteorder.h"
#include "ips_proto_params.h"
#include "psm2_hal.h"
#ifdef PSM_FI
#include "ips_config.h"
#endif


#ifdef min
#undef min
#endif
#define min(a, b) ((a) < (b) ? (a) : (b))

#ifdef max
#undef max
#endif
#define max(a, b) ((a) > (b) ? (a) : (b))

// macros taken fron IbAccess imath.h
/* round up value to align, align must be a power of 2 */
#ifndef ROUNDUPP2
#define ROUNDUPP2(val, align)   \
    (((uint32_t)(val) + (uint32_t)(align) - 1) & (~((uint32_t)(align)-1)))
#endif
/* force to use 64 bits in 32bit box */
#ifndef ROUNDUP64P2
#define ROUNDUP64P2(val, align)   \
        (((uint64_t)(val) + (uint64_t)(align) - 1) & (~((uint64_t)(align)-1)))
#endif

/* round up value to align, align can be any value, less efficient than ROUNDUPP2 */
#ifndef ROUNDUP
#define ROUNDUP(val, align) \
    ((( ((uint32_t)(val)) + (uint32_t)(align) -1) / (align) ) * (align))
#endif

/* round down value to align, align must be a power of 2 */
#ifndef ROUNDDOWNP2
#define ROUNDDOWNP2(val, align) \
    (((uint32_t)(val)) & (~((uint32_t)(align)-1)))
#endif

/* round down value to align, align can be any value, less efficient than ROUNDDOWNP2 */
#ifndef ROUNDDOWN
#define ROUNDDOWN(val, align)   \
    ((( ((uint32_t)(val))) / (align) ) * (align))
#endif



// convert MTU enums to bytes
// TBD - is there a way to specify MTU > 4K, such as 9000 byte jumbo
#define MTU_FIX          (7) // mtu_ind of 1 (256) => 2^(7+1)
#define MTU_SIZE(mtu_ind) (((uint64_t)1 << (MTU_FIX + mtu_ind)))

static psm2_error_t verbs_open_dev(psm2_ep_t ep, int unit, int port, psm2_uuid_t const job_key);
static psm2_error_t
check_port_state(psm2_ep_t ep);
static struct ibv_qp* ud_qp_create(psm2_ep_t ep);
static psm2_error_t modify_ud_qp_to_init(psm2_ep_t ep, struct ibv_qp *qp);
static psm2_error_t modify_ud_qp_to_rtr(psm2_ep_t ep, struct ibv_qp *qp);
static psm2_error_t modify_ud_qp_to_rts(psm2_ep_t ep, struct ibv_qp *qp);
static const char *link_layer_str(int8_t link_layer);
static enum psm_ibv_rate verbs_get_rate(uint8_t width, uint8_t speed);

void __psm2_ep_free_verbs(psm2_ep_t ep);
#ifdef RNDV_MOD
static void deregister_rv_conn_stats(psm2_ep_t ep);
static void deregister_rv_event_stats(psm2_ep_t ep);
#endif

// initialize the ep->verbs_ep portion of the ep
psm2_error_t
__psm2_ep_open_verbs(psm2_ep_t ep, int unit, int port, psm2_uuid_t const job_key)
{
	int flags;

	// make sure all fields are empty.
	memset(&ep->verbs_ep,0,sizeof(ep->verbs_ep));

	ep->verbs_ep.qkey = *(uint32_t*)job_key;	// use 1st 32 bits of job_key

	if (_HFI_PRDBG_ON) {
		char uuid_str[64];
		memset(&uuid_str, 0, sizeof(uuid_str));
		uuid_unparse(job_key, uuid_str);
		_HFI_PRDBG("job key %s qkey=0x%x\n", uuid_str, ep->verbs_ep.qkey);
	}

	if (PSM2_OK != verbs_open_dev(ep, unit, port, job_key)) {
		// verbs_open_dev already posted error.
		goto fail;
	}

	// compute an appropriate PSM payload size based on the UD MTU
	// and save result into ep->mtu
	if (PSM2_OK != check_port_state(ep)) {
		goto fail;
	}

	// we'll poll, so no need to allocate an event channel
	// 		eg. ibv_create_comp_channel

	ep->verbs_ep.pd = ibv_alloc_pd(ep->verbs_ep.context);
	if (! ep->verbs_ep.pd) {
		_HFI_ERROR( "Unable to alloc PD on %s: %s\n",
						ep->dev_name, strerror(errno));
		goto fail;
	}

	// planned QP sizes, also influences CQ sizes
	// PSM3_NUM_SEND_WQES, PSM3_NUM_RECV_WQES

	// we use ep as the cq_context (would be in callbacks if any)
	// we don't setup a completion channel nor completion vector since we will
	// poll
	// we will never have more than hfi_num_send_wqes +  hfi_num_send_rdma
	// so CQ only needs a little headroom to be safe (1000)
	ep->verbs_ep.send_cq = ibv_create_cq(ep->verbs_ep.context, ep->hfi_num_send_wqes+ep->hfi_num_send_rdma + 1000, (void*)ep, NULL, 0);
	if (! ep->verbs_ep.send_cq) {
		_HFI_ERROR( "Unable to create send CQ of size %u on %s: %s\n",
						ep->hfi_num_send_wqes+1000, ep->dev_name,
						strerror(errno));
		goto fail;
	}

	ep->verbs_ep.recv_comp_channel = ibv_create_comp_channel(ep->verbs_ep.context);
	if (! ep->verbs_ep.recv_comp_channel) {
		_HFI_ERROR( "Unable to create recv CQ completion channel on %s: %s\n",
						ep->dev_name, strerror(errno));
		goto fail;
	}
	// change completion channel to non-blocking
	flags = fcntl( ep->verbs_ep.recv_comp_channel->fd, F_GETFL);
	if (0 > fcntl( ep->verbs_ep.recv_comp_channel->fd, F_SETFL, flags | O_NONBLOCK)) {
		_HFI_ERROR( "Unable to change file descriptor of completion event channel for %s: %s\n",
					ep->dev_name, strerror(errno));
		goto fail;
	}
	// this gets done by __psm2_ep_poll_type
	//if (ibv_req_notify_cq(ep->verbs_ep.recv_cq, 0)) {
	//	_HFI_ERROR("Can't request RQ events from %s: %s\n",
	//					ep->dev_name, strerror(errno));
	//	goto fail;
	//}

	// TBD - should we pick an EQ number
	// we use ep as the cq_context (would be in callbacks if any)
	// we will never have more than hfi_num_recv_wqes+HFI_TF_NFLOWS
	// inflight WQEs
	// so CQ only needs a little headroom to be safe (1000)
	// HFI_TF_NFLOWS (32) limits receiver side concurrent tidflows (aka inbound
	// RDMA w/immed).
	// For USER RC Eager we can have num_recv_wqes/FRACTION per QP
	// in which case theoretical need could be huge.  We add 4000 as a
	// swag to cover most cases and user can always tune higher as needed
	if (! ep->hfi_num_recv_cqes) {
		ep->hfi_num_recv_cqes = ep->hfi_num_recv_wqes+HFI_TF_NFLOWS+1000;
		if ((ep->rdmamode&IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC)
			ep->hfi_num_recv_cqes += 4000;
	}
	ep->verbs_ep.recv_cq = ibv_create_cq(ep->verbs_ep.context,
						 ep->hfi_num_recv_cqes,
						 (void*)ep,  ep->verbs_ep.recv_comp_channel, 0);
	if (! ep->verbs_ep.recv_cq) {
		_HFI_ERROR( "Unable to create recv CQ of size %u on %s: %s\n",
					ep->hfi_num_recv_cqes, ep->dev_name,
					strerror(errno));
		goto fail;
	}

	ep->verbs_ep.qp = ud_qp_create(ep);
	if (! ep->verbs_ep.qp) {
		_HFI_ERROR( "Unable to create UD QP on %s\n", ep->dev_name);
		goto fail;
	}

	// rest of resources initialized by __psm2_ep_initialize_queues after we
	// have processed PSM3_MTU configuration
	return PSM2_OK;

fail:
	__psm2_ep_free_verbs(ep);
	return PSM2_INTERNAL_ERR;
}

// ep->mtu is now max PSM payload, not including headers and perhaps decreased
// via PSM3_MTU
// initialize the buffer pools and move the UD QP to RTS
psm2_error_t
__psm2_ep_initialize_queues(psm2_ep_t ep)
{

	if (PSM2_OK != psm_verbs_alloc_send_pool(ep, ep->verbs_ep.pd, &ep->verbs_ep.send_pool, 
				// save 1 send WQE just to be paranoid (should be unnecessary)
				min(ep->hfi_num_send_wqes, ep->verbs_ep.qp_cap.max_send_wr-1),
				// want to end up with multiple of cache line (64)
				// ep->mtu+MAX_PSM_HEADERS will be power of 2 verbs MTU
				// be conservative (+BUFFER_HEADROOM)
				ep->mtu + MAX_PSM_HEADER + BUFFER_HEADROOM
		)) {
		_HFI_ERROR( "Unable to allocate UD send buffer pool\n");
		goto fail;
	}
	if (PSM2_OK != psm_verbs_init_send_allocator(&ep->verbs_ep.send_allocator, 
					&ep->verbs_ep.send_pool)) {
		_HFI_ERROR( "Unable to init UD send buffer allocator\n");
		goto fail;
	}

	ep->verbs_ep.send_reap_thresh = min(ep->hfi_send_reap_thresh, ep->verbs_ep.send_pool.send_total/2);
	_HFI_PRDBG("reaping when %u posted.\n", ep->verbs_ep.send_reap_thresh);

	if (PSM2_OK != psm_verbs_alloc_recv_pool(ep, ep->verbs_ep.qp, &ep->verbs_ep.recv_pool, 
				min(ep->hfi_num_recv_wqes, ep->verbs_ep.qp_cap.max_recv_wr),
				// want to end up with multiple of cache line (64)
				// ep->mtu+MAX_PSM_HEADERS will be power of 2 verbs MTU
				// be conservative (+BUFFER_HEADROOM)
				ep->mtu + MAX_PSM_HEADER + BUFFER_HEADROOM
		)) {
		_HFI_ERROR( "Unable to allocate UD recv buffer pool\n");
		goto fail;
	}

	if (PSM2_OK != modify_ud_qp_to_init(ep, ep->verbs_ep.qp)) {
		goto fail;
	}

	if (PSM2_OK != __psm2_ep_verbs_prepost_recv(&ep->verbs_ep.recv_pool)) {
		_HFI_ERROR( "Unable to prepost recv buffers on QP for %s port %u\n", ep->dev_name, ep->portnum);
		goto fail;
	}

	// move QP to RTR and RTS
	if(PSM2_OK != modify_ud_qp_to_rtr(ep, ep->verbs_ep.qp)) {
		goto fail;
	}
	if(PSM2_OK != modify_ud_qp_to_rts(ep, ep->verbs_ep.qp)) {
		goto fail;
	}
	_HFI_PRDBG("created QP %p (%u)\n", ep->verbs_ep.qp, ep->verbs_ep.qp->qp_num);
	return PSM2_OK;

fail:
	psm_verbs_free_send_pool(&ep->verbs_ep.send_pool);
	psm_verbs_free_recv_pool(&ep->verbs_ep.recv_pool);
	return PSM2_INTERNAL_ERR;
}

int __psm2_ep_poll_type(int poll_type, psm2_ep_t ep)
{
	//if (poll_type == PSMI_HAL_POLL_TYPE_URGENT) {
	if (poll_type) {
		// set for event on solicted recv
		_HFI_PRDBG("enable solicited event\n");
		if (0 != ibv_req_notify_cq(ep->verbs_ep.recv_cq, 1)) {
			_HFI_ERROR("Can't request solicitied RQ events on %s: %s\n",
							ep->dev_name, strerror(errno));
			return -1;
		}
#if 0
	} else if (poll_type = PSMI_HAL_POLL_TYPE_ANYRCV) {
		// set for event on all recv completions
		psmi_assert_always(0);	// not used by PSM
		if (0 != ibv_req_notify_cq(ep->verbs_ep.recv_cq, 0)) {
			_HFI_ERROR("Can't request all RQ events on %s: %s\n",
							ep->dev_name, strerror(errno));
			return -1;
		}
#endif
	} else {
		// no events for solicted and unsolictited recv
		_HFI_PRDBG("disable solicited event - noop\n");
		// this is only done once during PSM shutdown of rcvthread.
		// Verbs events are one-shots.  No way to disable.  However once
		// PSM stops rcvthread shortly after this call, no one will be
		// polling for these events so worst case only 1 additional event
		// occurs and does not get reenabled.
	}
	return 0;
}

// free reources in ep->verbs_ep portion of the ep
void __psm2_ep_free_verbs(psm2_ep_t ep)
{
	if (ep->verbs_ep.qp) {
		ibv_destroy_qp(ep->verbs_ep.qp);
		ep->verbs_ep.qp = NULL;
	}
	psm_verbs_free_send_pool(&ep->verbs_ep.send_pool);
	psm_verbs_free_recv_pool(&ep->verbs_ep.recv_pool);
	if (ep->verbs_ep.recv_cq) {
		ibv_destroy_cq(ep->verbs_ep.recv_cq);
		ep->verbs_ep.recv_cq = NULL;
	}
	if (ep->verbs_ep.recv_comp_channel) {
		ibv_destroy_comp_channel(ep->verbs_ep.recv_comp_channel);
		ep->verbs_ep.recv_comp_channel = NULL;
	}

	if (ep->verbs_ep.send_cq) {
		ibv_destroy_cq(ep->verbs_ep.send_cq);
		ep->verbs_ep.send_cq = NULL;
	}
	if (ep->verbs_ep.pd) {
		ibv_dealloc_pd(ep->verbs_ep.pd);
		ep->verbs_ep.pd = NULL;
	}
#ifdef RNDV_MOD
	if (ep->verbs_ep.rv) {
		if (IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)) {
			deregister_rv_conn_stats(ep);
			deregister_rv_event_stats(ep);
		}
		__psm2_rv_close(ep->verbs_ep.rv);
		ep->verbs_ep.rv = NULL;
	}
#endif
	if (ep->verbs_ep.context) {
		ibv_close_device(ep->verbs_ep.context);
		ep->verbs_ep.context = NULL;
	}
	if (ep->dev_name) {
		psmi_free((char*)ep->dev_name);
		ep->dev_name = NULL;
	}
}

// ep argument is only for calloc to associate memory statistics with ep
// do NOT use ep->verbs_ep.*_pool in this function, instead of pool
// to access buffering fields.  This function will be called for other pools
// which are tracked in other structures but still part of the ep's memory stats
psm2_error_t psm_verbs_alloc_send_pool(psm2_ep_t ep, struct ibv_pd *pd,
			psm2_verbs_send_pool_t pool,
			uint32_t send_total, uint32_t send_buffer_size)
{
	memset(pool,0,sizeof(*pool));

	// use what we got, make sure it's a multiple of coallesce
	// don't grow beyond requested, otherwise we could exceed CQ sizes
	pool->send_total = ROUNDDOWN(send_total, VERBS_SEND_CQ_COALLESCE);

	if (send_total && send_buffer_size) {
		// allocate send buffers
		int i;
		pool->send_buffer_size = send_buffer_size;
		pool->send_num_free = pool->send_total;
		pool->send_buffers = (uint8_t *)psmi_memalign(ep, NETWORK_BUFFERS, CPU_PAGE_ALIGN,
													 pool->send_total*pool->send_buffer_size);
		if (! pool->send_buffers) {
			_HFI_ERROR( "can't alloc send buffers");
			goto fail;
		}

		_HFI_PRDBG("send pool: buffers: %p size %u\n",  pool->send_buffers, pool->send_buffer_size);
		pool->send_bufs = (struct verbs_sbuf *)psmi_calloc(ep, NETWORK_BUFFERS,
							 pool->send_total*sizeof(struct verbs_sbuf), 1);
		if (! pool->send_bufs) {
			_HFI_ERROR("can't alloc send buffers ctrl");
			goto fail;
		}
		// prepare free list, put lower numbered buffers at head of free list
		for (i=pool->send_total-1; i >= 0; i--) {
			pool->send_bufs[i].buffer = &(pool->send_buffers[send_buffer_start(pool, i)]);
			pool->send_bufs[i].next = pool->send_free;
			pool->send_free = &(pool->send_bufs[i]);
		}
		_HFI_PRDBG("%u Send Buffers of %u bytes each allocated at %p.\n", pool->send_total, pool->send_buffer_size,
			pool->send_buffers);

		// UD doesn't support RDMA, so we just need local NIC to be able to
		// access our buffers with kernel bypass (IBV_ACCESS_LOCAL_WRITE)
		// technically we probably don't need LOCAL_WRITE for send buffers
		pool->send_buffer_mr = ibv_reg_mr(
						pd, pool->send_buffers,
						pool->send_total*pool->send_buffer_size,
						IBV_ACCESS_LOCAL_WRITE);
		if (! pool->send_buffer_mr) {
			_HFI_ERROR( "Unable to alloc send buffer MR on %s: %s\n",
							ep->dev_name, strerror(errno));
			goto fail;
		}
	}
	return PSM2_OK;

fail:
	psm_verbs_free_send_pool(pool);
	return PSM2_INTERNAL_ERR;
}

extern psm2_error_t psm_verbs_init_send_allocator(
			psm2_verbs_send_allocator_t allocator,
			psm2_verbs_send_pool_t pool)
{

	memset(allocator,0,sizeof(*allocator));
	allocator->pool = pool;
	allocator->send_num_til_coallesce = VERBS_SEND_CQ_COALLESCE;
	return PSM2_OK;
}


// ep argument is only for calloc to associate memory statistics with ep
// do NOT use ep->verbs_ep.*_pool in this function, instead of pool
// to access buffering fields.  This function will be called for other pools
// which are tracked in other structures but still part of the ep's memory stats
// For RC QPs receiving only RDMA Write with immediate, no buffer space is
// needed.  Caller will specify recv_buffer_size==0 with a recv_total.
psm2_error_t psm_verbs_alloc_recv_pool(psm2_ep_t ep, struct ibv_qp *qp,
			psm2_verbs_recv_pool_t pool,
			uint32_t recv_total, uint32_t recv_buffer_size)
{
	memset(pool,0,sizeof(*pool));

	pool->qp = qp;	// save a reference
	pool->ep = ep;
	pool->recv_total = recv_total;

	if (recv_total ) {
		int i;
		if (recv_buffer_size) {
			// allocate recv buffers
			pool->recv_buffer_size = recv_buffer_size;
			// beginning of UD QP Recv Buf always consumed with space for IB GRH
			if (qp->qp_type == IBV_QPT_UD) {
				// round up UD_ADDITION (40) to multiple of 64 for better
				// cache alignment of buffers
				pool->recv_buffer_size += ROUNDUP(UD_ADDITION, 64);
				pool->addition = UD_ADDITION;
			}
			pool->recv_buffers = (uint8_t *)psmi_calloc(ep, NETWORK_BUFFERS,
							 pool->recv_total*pool->recv_buffer_size, 1);
			if (! pool->recv_buffers) {
				_HFI_ERROR( "can't alloc recv buffers");
				goto fail;
			}
			//printf("recv pool: buffers: %p size %u\n",  pool->recv_buffers, pool->recv_buffer_size);
			pool->recv_bufs = (struct verbs_rbuf *)psmi_calloc(ep, NETWORK_BUFFERS,
								 pool->recv_total*sizeof(struct verbs_rbuf), 1);
			if (! pool->recv_bufs) {
				_HFI_ERROR("can't alloc recv buffers ctrl");
				goto fail;
			}
			// prepare rbuf handles for use as wr_id
			for (i=0; i<pool->recv_total; i++) {
				pool->recv_bufs[i].buffer = &(pool->recv_buffers[recv_buffer_start(pool, i)]);
				pool->recv_bufs[i].pool = pool;
			}
			_HFI_PRDBG("%u Recv Buffers of %u bytes each allocated at %p.\n", pool->recv_total, pool->recv_buffer_size,
				pool->recv_buffers);

			// UD doesn't support RDMA, so we just need local NIC to be able to
			// access our buffers with kernel bypass (IBV_ACCESS_LOCAL_WRITE)
			pool->recv_buffer_mr = ibv_reg_mr(
							qp->pd, pool->recv_buffers,
							pool->recv_total*pool->recv_buffer_size,
							IBV_ACCESS_LOCAL_WRITE);
			if (! pool->recv_buffer_mr) {
				_HFI_ERROR( "Unable to alloc recv buffer MR on %s: %s\n",
								ep->dev_name, strerror(errno));
				goto fail;
			}
		} else {
			// we want a pool for RDMA Write w/immediate recv.  No buffers
			psmi_assert(qp->qp_type != IBV_QPT_UD);
			// we use exactly 1 rbuf so wr_id can lead us to pool and qp
			pool->recv_bufs = (struct verbs_rbuf *)psmi_calloc(ep, NETWORK_BUFFERS,
							 sizeof(struct verbs_rbuf), 1);
			if (! pool->recv_bufs) {
				_HFI_ERROR("can't alloc recv buffers ctrl");
				goto fail;
			}
			// prepare rbuf handle for use as wr_id
			pool->recv_bufs->pool = pool;
			_HFI_PRDBG("%u Recv Buffers of %u bytes each allocated.\n", pool->recv_total, pool->recv_buffer_size);
		}
#if VERBS_RECV_QP_COALLESCE > 1
		// prebuild as much as we can
		for (i=0; i < VERBS_RECV_QP_COALLESCE; i++ ) {
			struct ibv_recv_wr *wr = &(pool->recv_wr_list[i]);
			wr->next = &(pool->recv_wr_list[i+1]);
			if (recv_buffer_size) {
				struct ibv_sge *list = &(pool->recv_sge_list[i]);
				wr->sg_list = list;
				list->length = pool->recv_buffer_size;
				list->lkey = pool->recv_buffer_mr->lkey;
				wr->num_sge = 1;	// size of sg_list
			} else {
				wr->sg_list = NULL;
				wr->num_sge = 0;	// size of sg_list
			}
		}
		// fixup end of list
		pool->recv_wr_list[VERBS_RECV_QP_COALLESCE-1].next = NULL;
		pool->next_recv_wqe = 0;
#endif
	}
	return PSM2_OK;

fail:
	psm_verbs_free_recv_pool(pool);
	return PSM2_INTERNAL_ERR;
}

void psm_verbs_free_send_pool(psm2_verbs_send_pool_t pool)
{
	if (pool->send_buffer_mr) {
		ibv_dereg_mr(pool->send_buffer_mr);
		pool->send_buffer_mr = NULL;
	}
	if (pool->send_bufs) {
		psmi_free(pool->send_bufs);
		pool->send_bufs = NULL;
	}
	if (pool->send_buffers) {
		psmi_free(pool->send_buffers);
		pool->send_buffers = NULL;
	}
	memset(pool,0,sizeof(*pool));	// in case anyone looks at other integers
}

// this is not allowed to access pool->qp, it may already be destroyed
void psm_verbs_free_recv_pool(psm2_verbs_recv_pool_t pool)
{
	if (pool->recv_buffer_mr) {
		ibv_dereg_mr(pool->recv_buffer_mr);
		pool->recv_buffer_mr = NULL;
	}
	if (pool->recv_bufs) {
		psmi_free(pool->recv_bufs);
		pool->recv_bufs = NULL;
	}
	if (pool->recv_buffers) {
		psmi_free(pool->recv_buffers);
		pool->recv_buffers = NULL;
	}
	memset(pool,0,sizeof(*pool));	// in case anyone looks at other integers
}

// the allocator tries to reallocate recently freed send buffers
// so we can tend to allocate a small set of buffers
// to improve CPU, MMU and NIC MMU hit rates
sbuf_t __psm2_ep_verbs_alloc_sbuf(psm2_verbs_send_allocator_t allocator)
{
	psm2_verbs_send_pool_t pool = allocator->pool;
	sbuf_t sbuf = pool->send_free;
	if_pt (sbuf) {
		// take off head of free list
		pool->send_free = sbuf->next;
		pool->send_num_free--;
		sbuf->next = NULL;
		// keep a list of allocated buffers in order at alloc_head
		// and put this one at the alloc_end of the list
		if_pf (! allocator->send_alloc_head)	// unlikely when more than 1 posted
			allocator->send_alloc_head = sbuf;
		if_pt (allocator->send_alloc_end)	// likely when more than 1 posted
			allocator->send_alloc_end->next = sbuf;
		allocator->send_alloc_end = sbuf;
		sbuf->allocator = allocator;
	}
	return sbuf;
}

// buffers must be freed in order, the fact the SQ reports completions in
// same order as send WQEs ensures this
// this will free count buffers with buf being the last freed
void __psm2_ep_verbs_free_sbuf(
			sbuf_t buf, uint32_t count)
{
	psm2_verbs_send_allocator_t allocator = buf->allocator;
	psm2_verbs_send_pool_t pool = allocator->pool;
	sbuf_t b;
	do {
		// take 1st off allocated list
		b = allocator->send_alloc_head;
		allocator->send_alloc_head = b->next;
		if_pf (allocator->send_alloc_end == b)	// unlikely last outstanding
			allocator->send_alloc_end = NULL;
		// put at head of free list
		b->next =  pool->send_free;
		pool->send_free = b;
		pool->send_num_free++;
#ifdef UD_DEBUG
		printf("freed: %u num free: %u\n", 
			(uint32_t)send_buffer_index(pool, b->buffer),
			pool->send_num_free);
#endif
	} while (--count && b != buf);
	// normally we will find buf just as we exhaust count (coallesce amount).
	// however when send error CQEs occur (such as flush) we may find less
	// than count inflight ahead of buf
	//psmi_assert_always(b == buf && count == 0);
	psmi_assert_always(b == buf);
}

psm2_error_t __psm2_ep_verbs_post_recv(
				rbuf_t buf)
{
	psm2_verbs_recv_pool_t pool = buf->pool;
#if VERBS_RECV_QP_COALLESCE > 1
	struct ibv_recv_wr *wr;
#else
	struct ibv_recv_wr wr;
	struct ibv_sge list;
#endif
	struct ibv_recv_wr *bad_wr;

	// only RC QPs doing just RDMA Write can have a zero buffer size
	if (pool->recv_buffer_size) {
		uint32_t index = recv_buffer_index(pool, rbuf_to_buffer(buf));
		// make sure its a buffer in our pool
		psmi_assert_always(index < pool->recv_total);
		// assert on index covers these 2 asserts
		//psmi_assert_always(rbuf_to_buffer(buf) >= pool->recv_buffers);
		//psmi_assert_always(rbuf_to_buffer(buf) <= pool->recv_buffers +
		//						 pool->recv_total)*pool->recv_buffer_size);
		// make sure buf is exactly at the start of a buffer in our pool
		psmi_assert_always(rbuf_to_buffer(buf) == &(pool->recv_buffers[recv_buffer_start(pool, index)]));

#if VERBS_RECV_QP_COALLESCE > 1
		// put buf in wr at end of list
		wr = &(pool->recv_wr_list[pool->next_recv_wqe]);
		psmi_assert(wr->sg_list == &(pool->recv_sge_list[pool->next_recv_wqe]));
		wr->sg_list->addr = (uintptr_t)rbuf_to_buffer(buf);
		wr->wr_id = (uintptr_t)buf;	// we'll get this back in completion
#ifdef PSM_FI
		if_pf(PSMI_FAULTINJ_ENABLED_EP(pool->ep)) {
			PSMI_FAULTINJ_STATIC_DECL(fi_rq_lkey, "rq_lkey",
					"post UD "
					"or RC "
					"RQ WQE with bad lkey",
					0, IPS_FAULTINJ_RQ_LKEY);
			if_pf(PSMI_FAULTINJ_IS_FAULT(fi_rq_lkey, " QP %u", pool->qp->qp_num))
				wr->sg_list->lkey = 55;
		} else
			wr->sg_list->lkey = pool->recv_buffer_mr->lkey;
#endif // PSM_FI
		if_pf (++pool->next_recv_wqe >= VERBS_RECV_QP_COALLESCE) {
			// we have a batch ready to post
			if_pf (ibv_post_recv(pool->qp, pool->recv_wr_list, &bad_wr)) {
				_HFI_ERROR("failed to post RQ on %s port %u: %s", pool->ep->dev_name, pool->ep->portnum, strerror(errno));
				return PSM2_INTERNAL_ERR;
			}
			//_HFI_VDBG("posted RQ, including buffer %u\n", index);
			pool->next_recv_wqe = 0;
		} else {
			//_HFI_VDBG("preped RQE, buffer %u\n", index);
		}
#else
		list.addr = (uintptr_t)rbuf_to_buffer(buf);
		list.length = pool->recv_buffer_size;
		list.lkey = pool->recv_buffer_mr->lkey;
#ifdef PSM_FI
		if_pf(PSMI_FAULTINJ_ENABLED_EP(pool->ep)) {
			PSMI_FAULTINJ_STATIC_DECL(fi_rq_lkey, "rq_lkey",
					"post UD "
					"or RC "
					"RQ WQE with bad lkey",
					0, IPS_FAULTINJ_RQ_LKEY);
			if_pf(PSMI_FAULTINJ_IS_FAULT(fi_rq_lkey, " QP %u", pool->qp->qp_num))
				list.lkey = 55;
		}
#endif // PSM_FI
		wr.next = NULL;	// just post 1
		wr.wr_id = (uintptr_t)buf;	// we'll get this back in completion
		wr.sg_list = &list;
		wr.num_sge = 1;	// size of sg_list

		if_pf (ibv_post_recv(pool->qp, &wr, &bad_wr)) {
			_HFI_ERROR("failed to post RQ on %s port %u: %s", pool->ep->dev_name, pool->ep->portnum, strerror(errno));
			return PSM2_INTERNAL_ERR;
		}
		//_HFI_VDBG("posted RQ, buffer %u\n", index);
#endif
	} else {
#if VERBS_RECV_QP_COALLESCE > 1
		// put buf in wr at end of list
		wr = &(pool->recv_wr_list[pool->next_recv_wqe]);
		psmi_assert(wr->sg_list == NULL);
		wr->wr_id = (uintptr_t)buf;	// we'll get this back in completion
		if_pf (++pool->next_recv_wqe >= VERBS_RECV_QP_COALLESCE) {
			// we have a batch ready to post
			if_pf (ibv_post_recv(pool->qp, pool->recv_wr_list, &bad_wr)) {
				_HFI_ERROR("failed to post RQ on %s on port %u: %s", pool->ep->dev_name, pool->ep->portnum, strerror(errno));
				return PSM2_INTERNAL_ERR;
			}
			//_HFI_VDBG("posted RQ\n");
			pool->next_recv_wqe = 0;
		} else {
			//_HFI_VDBG("preped RQE\n");
		}
#else
		wr.next = NULL;	// just post 1
		wr.wr_id = (uintptr_t)buf;	// we'll get this back in completion
		wr.sg_list = NULL;
		wr.num_sge = 0;	// size of sg_list

		if_pf (ibv_post_recv(pool->qp, &wr, &bad_wr)) {
			_HFI_ERROR("failed to post RQ on %s on port %u: %s", pool->ep->dev_name, pool->ep->portnum, strerror(errno));
			return PSM2_INTERNAL_ERR;
		}
		//_HFI_VDBG("posted RQ\n");
#endif
	}
	return PSM2_OK;
}

psm2_error_t __psm2_ep_verbs_prepost_recv(
						psm2_verbs_recv_pool_t pool)
{
	int i;

	if (! pool->recv_total)
		return PSM2_INTERNAL_ERR;
	// prepare RQ
	for (i=0; i< pool->recv_total; i++) {
		rbuf_t buf = &(pool->recv_bufs[i]);
		if (pool->recv_buffer_size)
			buf = &(pool->recv_bufs[i]);
		else
			buf = pool->recv_bufs;	// only 1, just to find pool and qp
		if (PSM2_OK != __psm2_ep_verbs_post_recv(
							buf)) {
			_HFI_ERROR( "Unable to post RQ on %s port %u\n", pool->ep->dev_name, pool->ep->portnum);
			return PSM2_INTERNAL_ERR;
		}
	}
	return PSM2_OK;
}

// only used when PSM3_RDMA enabled
psm2_error_t psm2_verbs_post_rdma_write_immed(psm2_ep_t ep, struct ibv_qp *qp,
				void *loc_buf, struct psm2_verbs_mr *loc_mr,
				uint64_t rem_buf, uint32_t rkey,
				size_t len, uint32_t immed, uint64_t wr_id)
{
	struct ibv_send_wr wr;
	struct ibv_send_wr *bad_wr;
	struct ibv_sge list;
	psm2_error_t ret = PSM2_OK;

	//printf("XXXX %s 0x%p %ld 0x%x\n", __FUNCTION__, loc_buf, len, loc_mr->lkey);
	psmi_assert(IPS_PROTOEXP_FLAG_USER_RC_QP(ep->rdmamode));

	list.addr = (uintptr_t)loc_buf;
	list.length = len;
	list.lkey = loc_mr->lkey;
#ifdef PSM_FI
	if_pf(PSMI_FAULTINJ_ENABLED_EP(ep)) {
		PSMI_FAULTINJ_STATIC_DECL(fi_rc_rdma_lkey, "rc_rdma_lkey",
				"post RC RDMA Write WQE with bad lkey",
				0, IPS_FAULTINJ_RC_RDMA_LKEY);
		if_pf(PSMI_FAULTINJ_IS_FAULT(fi_rc_rdma_lkey, " QP %u", qp->qp_num))
			list.lkey = 55;
	}
#endif // PSM_FI
	wr.next = NULL; // just post 1
	psmi_assert(! (wr_id & VERBS_SQ_WR_ID_MASK));
	wr.wr_id = wr_id | VERBS_SQ_WR_ID_RDMA_WRITE;
	wr.sg_list = &list;
	wr.num_sge = 1; // size of sg_list
	wr.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
	wr.imm_data = immed;
	wr.wr.rdma.remote_addr = rem_buf;
	wr.wr.rdma.rkey = rkey;
#ifdef PSM_FI
	if_pf(PSMI_FAULTINJ_ENABLED_EP(ep)) {
		PSMI_FAULTINJ_STATIC_DECL(fi_rc_rdma_rkey, "rc_rdma_rkey",
				"post RC RDMA Write WQE with bad rkey",
				0, IPS_FAULTINJ_RC_RDMA_RKEY);
		if_pf(PSMI_FAULTINJ_IS_FAULT(fi_rc_rdma_rkey, " QP %u", qp->qp_num))
			wr.wr.rdma.rkey = 55;
	}
#endif // PSM_FI
	// RDMA Writes will tend to be larger and we want the completion
	// to reflect the RDMA for a given CTS is completed
	wr.send_flags = IBV_SEND_SIGNALED;  // get a completion
	// no need for wr.send_flags |= IBV_SEND_SOLICITED
	// these will be bigger sends, no need for inline
	ep->verbs_ep.send_rdma_outstanding++;
	if_pf (ibv_post_send(qp, &wr, &bad_wr)) {
		if (errno != EBUSY && errno != EAGAIN && errno != ENOMEM)
			_HFI_ERROR("failed to post RC SQ on %s port %u: %s",
					ep->dev_name, ep->portnum, strerror(errno));
		// caller will try again later when next send buffer freed
		// or timer expires
		ret = PSM2_TIMEOUT;
		ep->verbs_ep.send_rdma_outstanding--;
		goto done;
	}
	_HFI_VDBG("posted RDMA Write: from 0x%"PRIx64" to 0x%"PRIx64" len %u rkey 0x%x\n",
		list.addr,  wr.wr.rdma.remote_addr, list.length,  wr.wr.rdma.rkey /* TBD rem QPN */ );
#if 0
	// we will not have many in flight at a time so
	// normal progress calls should be sufficient
	// no need to reap completions here
	err = psm2_verbs_completion_update(ep);
	if_pf (err != PSM2_OK)
		return err;
#endif
done:
	//printf("XXXX %s ret:%d\n", __FUNCTION__, ret);
	return ret;
}

#ifdef RNDV_MOD
psm2_error_t psm2_verbs_post_rv_rdma_write_immed(psm2_ep_t ep,
				psm2_rv_conn_t conn,
				void *loc_buf, struct psm2_verbs_mr *loc_mr,
				uint64_t rem_buf, uint32_t rkey,
				size_t len, uint32_t immed, uint64_t wr_id,
				uint8_t *sconn_index, uint32_t *conn_count)
{
	psm2_error_t ret = PSM2_OK;

	//printf("XXXX %s 0x%p %ld 0x%x\n", __FUNCTION__, loc_buf, len, loc_mr->lkey);
	psmi_assert(IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode));

	ep->verbs_ep.send_rdma_outstanding++;
#ifdef PSM_FI
	if_pf(PSMI_FAULTINJ_ENABLED_EP(ep)) {
		PSMI_FAULTINJ_STATIC_DECL(fi_rv_rdma_len, "rv_rdma_len",
				"post RV RDMA Write with bad len (may want RV build with RNDV_LOCAL_ERR_TEST)",
				0, IPS_FAULTINJ_RV_RDMA_LEN);
		if_pf(PSMI_FAULTINJ_IS_FAULT(fi_rv_rdma_len, ""))
			len += 1000000000;
	}
	if_pf(PSMI_FAULTINJ_ENABLED_EP(ep)) {
		PSMI_FAULTINJ_STATIC_DECL(fi_rv_rdma_rkey, "rv_rdma_rkey",
				"post RV RDMA Write with bad rkey",
				1, IPS_FAULTINJ_RV_RDMA_RKEY);
		if_pf(PSMI_FAULTINJ_IS_FAULT(fi_rv_rdma_rkey, ""))
			rkey = 55;
	}
#endif // PSM_FI
	if (__psm2_rv_post_rdma_write_immed(ep->verbs_ep.rv, conn,
                loc_buf, loc_mr->mr.rv_mr,
                rem_buf, rkey,
                len, immed, wr_id, sconn_index, conn_count)) {
		switch (errno) {
		case EIO:
			// lost or failed connection
			ret = PSM2_EPID_RV_CONNECT_ERROR;
			break;
		case EAGAIN:
			// lost connection and are recoverying it
			ret = PSM2_EPID_RV_CONNECT_RECOVERING;
			break;
		case ENOMEM:
		case EBUSY:
			// caller will try again later when next send buffer freed
			// or timer expires
			ret = PSM2_TIMEOUT;
			break;
		default:
			ret = PSM2_INTERNAL_ERR;
			break;
		}
		if (errno != EBUSY && errno != EAGAIN && errno != ENOMEM) {
			_HFI_ERROR("failed to post RV RC SQ on %s port %u: %s",
					ep->dev_name, ep->portnum, strerror(errno));
			psmi_assert_always(errno != EINVAL);
		}
		ep->verbs_ep.send_rdma_outstanding--;
		goto done;
	}
	_HFI_VDBG("posted RV RDMA Write: from 0x%"PRIx64" to 0x%"PRIx64" len %u rkey 0x%x\n",
		(uint64_t)loc_buf,  rem_buf, (unsigned)len,  rkey /* TBD rem QPN */ );
done:
	//printf("XXXX %s ret:%d\n", __FUNCTION__, ret);
	return ret;
}
#endif // RNDV_MOD

extern int ips_protoexp_rdma_write_completion( uint64_t wr_id);

// we structure this similar to ips_proto_dma_completion_update
// this is non-blocking.  We reap what's available and then return
psm2_error_t
psm2_verbs_completion_update(psm2_ep_t ep)
{
	#define CQE_BATCH 10	// reap a few at a time, hopefully faster this way
	//#define CQE_BATCH 8 or 18	// reap a few at a time, hopefully faster this way
							// 18*COALLESE > default reap threshold so we
							// should get away with one poll_q
							// not sure if doing the exact math here would
							// add clocks and hurt a bit more than approx math
							// int batch = (send_reap_thresh/COALLESCE) + 2
							// alloca(sizeof(ibv_wc) & batch)
	struct ibv_wc wc[CQE_BATCH];
	int ne;

	PSMI_LOCK_ASSERT(ep->mq->progress_lock);
	// TBD - when coallescing completions we'll tend to fall through to poll_cq
	// this only called when out of buffers or immediately after posting a send
	// reduce the frequency of poll_cq by only checking once we have at least
	// send_reap_thresh sends in flight
	// for USE_RC this is imperfect, we can have a handful of unsignaled
	// send WQEs on multiple RC QPs, in which case we may exceed the
	// reap_thresh but not find any CQEs until we post more sends and
	// hit the coalsce threshold.
	if_pt ((! ep->verbs_ep.send_rdma_outstanding
				 || IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode))
		   && ep->verbs_ep.send_pool.send_num_free > ep->verbs_ep.send_pool.send_total - ep->verbs_ep.send_reap_thresh  )
		return PSM2_OK;	// not ready to reap, return quickly

	//if ( 0 != (ne = ibv_poll_cq(ep->verbs_ep.send_cq, CQE_BATCH, wc)))
	while ( 0 != (ne = ibv_poll_cq(ep->verbs_ep.send_cq, CQE_BATCH, wc)))
	{
		unsigned i;
		for (i=0; i<ne; i++) {
			psmi_assert_always(wc[i].wr_id);
			if_pf (wc[i].status) {
				if (wc[i].status != IBV_WC_WR_FLUSH_ERR)
					_HFI_ERROR("failed %s on %s port %u status: '%s' (%d) QP %u\n",
						VERBS_SQ_WR_OP_STR(wc[i].wr_id),
						ep->dev_name, ep->portnum,
						ibv_wc_status_str(wc[i].status), (int)wc[i].status,
						wc[i].qp_num);
				// For user space RC QP, the QP is now in QPS_ERROR and we
				// need to reset (or replace) and reconnect it.
				// Upcoming async event will cause us to stop.
				// User's wanting reliability for RDMA should use RV.
				if (VERBS_SQ_WR_OP(wc[i].wr_id) == VERBS_SQ_WR_ID_SEND)
					__psm2_ep_verbs_free_sbuf(
								(sbuf_t)(wc[i].wr_id & ~VERBS_SQ_WR_ID_MASK),
								VERBS_SEND_CQ_COALLESCE);
				continue;
			}
			switch (wc[i].opcode) {
			case IBV_WC_SEND:
				// UD sends just mean it got onto the wire and can reuse our buf
				// no guarantees it made it to the remote side
				// buffer address is in wc.wr_id
				_HFI_VDBG("send done (%u bytes) sbuf index %lu\n", wc[i].byte_len,
					send_buffer_index(&ep->verbs_ep.send_pool, sbuf_to_buffer((sbuf_t)(wc[i].wr_id))));
				__psm2_ep_verbs_free_sbuf(
							(sbuf_t)(wc[i].wr_id & ~VERBS_SQ_WR_ID_MASK),
							VERBS_SEND_CQ_COALLESCE);
				break;
			case IBV_WC_RDMA_WRITE:
				ep->verbs_ep.send_rdma_outstanding--;
				ips_protoexp_rdma_write_completion(
							 wc[i].wr_id & ~VERBS_SQ_WR_ID_MASK);
				break;
			default:
				_HFI_ERROR("unexpected send completion on %s port %u opcode %d QP %u\n",
							ep->dev_name, ep->portnum,
							wc[i].opcode, wc[i].qp_num);
				break;
			}
		}
#if 0
		// this is optional, especially if use "if" above instead of while
		if (ne <CQE_BATCH)
			break;	// we got less than we asked, we are fast enought that
					// there probably aren't any more on CQE, so just let our
					// next pass reap any that appear while we were processing
#endif
	}
	return PSM2_OK;
}

int verbs_get_port_index2pkey(psm2_ep_t ep, int port, int index)
{
	__be16 pkey;

	psmi_assert_always(ep->verbs_ep.context);
	if (0 != ibv_query_pkey(ep->verbs_ep.context, port, index, &pkey)) {
		_HFI_ERROR( "Can't query pkey index %d on %s port %u: %s\n", index,
				ep->dev_name, port, strerror(errno));
		return -1;
	}
	_HFI_PRDBG("got pkey 0x%x on %s port %u\n", __be16_to_cpu(pkey), ep->dev_name, port);
	return __be16_to_cpu(pkey);
}

#ifdef RNDV_MOD
// accessor functions for cm statistics
#define EP_STAT_FUNC(func, stat) \
	static uint64_t func(void *context) \
	{ \
		psm2_ep_t ep = (psm2_ep_t)context; \
		return ep->stat; \
	}

EP_STAT_FUNC(rv_q_depth, rv_q_depth)
EP_STAT_FUNC(rv_reconnect_timeout, rv_reconnect_timeout)
EP_STAT_FUNC(rv_hb_interval, rv_hb_interval)
#undef EP_STAT_FUNC

static uint64_t rv_index(void *context)
{
	struct psm2_verbs_ep *vep = &((psm2_ep_t)context)->verbs_ep;
	return vep->rv_index;
}

static uint64_t rv_conn_flags(void *context)
{
	struct psm2_verbs_ep *vep = &((psm2_ep_t)context)->verbs_ep;
	if (vep->rv) {
		// this is a little sly, we know the stats processing routines will
		// call the accessors in the order from the entries list
		// so we use the 1st of the rv statistics accessors to get
		// the statistics from rv into the cache structure so other accessors
		// can simply return the relevant value
			// we get aggregated values instead of per conn
		(void)__psm2_rv_get_conn_stats(vep->rv, NULL, 0, &vep->rv_conn_stats);
	}
	return vep->rv_conn_stats.flags;
}

#define RV_CM_STAT_FUNC(func, stat) \
	static uint64_t func(void *context) \
	{ \
		struct psm2_verbs_ep *vep = &((psm2_ep_t)context)->verbs_ep; \
		return vep->rv_conn_stats.stat; \
	}

RV_CM_STAT_FUNC(rv_conn_num_conn, num_conn)
RV_CM_STAT_FUNC(rv_conn_req_error, req_error)
RV_CM_STAT_FUNC(rv_conn_req_recv, req_recv)
RV_CM_STAT_FUNC(rv_conn_rep_error, rep_error)
RV_CM_STAT_FUNC(rv_conn_rep_recv, rep_recv)
RV_CM_STAT_FUNC(rv_conn_rtu_recv, rtu_recv)
RV_CM_STAT_FUNC(rv_conn_established, established)
RV_CM_STAT_FUNC(rv_conn_dreq_error, dreq_error)
RV_CM_STAT_FUNC(rv_conn_dreq_recv, dreq_recv)
RV_CM_STAT_FUNC(rv_conn_drep_recv, drep_recv)
RV_CM_STAT_FUNC(rv_conn_timewait, timewait)
RV_CM_STAT_FUNC(rv_conn_mra_recv, mra_recv)
RV_CM_STAT_FUNC(rv_conn_rej_recv, rej_recv)
RV_CM_STAT_FUNC(rv_conn_lap_error, lap_error)
RV_CM_STAT_FUNC(rv_conn_lap_recv, lap_recv)
RV_CM_STAT_FUNC(rv_conn_apr_recv, apr_recv)
RV_CM_STAT_FUNC(rv_conn_unexp_event, unexp_event)
RV_CM_STAT_FUNC(rv_conn_req_sent, req_sent)
RV_CM_STAT_FUNC(rv_conn_rep_sent, rep_sent)
RV_CM_STAT_FUNC(rv_conn_rtu_sent, rtu_sent)
RV_CM_STAT_FUNC(rv_conn_rej_sent, rej_sent)
RV_CM_STAT_FUNC(rv_conn_dreq_sent, dreq_sent)
RV_CM_STAT_FUNC(rv_conn_drep_sent, drep_sent)
//RV_CM_STAT_FUNC(rv_conn_wait_time, wait_time)
//RV_CM_STAT_FUNC(rv_conn_resolve_time, resolve_time)
//RV_CM_STAT_FUNC(rv_conn_connect_time, connect_time)
//RV_CM_STAT_FUNC(rv_conn_connected_time, connected_time)
RV_CM_STAT_FUNC(rv_conn_resolve, resolve)
RV_CM_STAT_FUNC(rv_conn_resolve_fail, resolve_fail)
RV_CM_STAT_FUNC(rv_conn_conn_recovery, conn_recovery)
//RV_CM_STAT_FUNC(rv_conn_rewait_time, rewait_time)
//RV_CM_STAT_FUNC(rv_conn_reresolve_time, reresolve_time)
//RV_CM_STAT_FUNC(rv_conn_reconnect_time, reconnect_time)
//RV_CM_STAT_FUNC(rv_conn_max_rewait_time, max_rewait_time)
//RV_CM_STAT_FUNC(rv_conn_max_reresolve_time, max_reresolve_time)
//RV_CM_STAT_FUNC(rv_conn_max_reconnect_time, max_reconnect_time)
RV_CM_STAT_FUNC(rv_conn_reresolve, reresolve)
RV_CM_STAT_FUNC(rv_conn_reresolve_fail, reresolve_fail)
//RV_CM_STAT_FUNC(rv_conn_post_write, post_write)
//RV_CM_STAT_FUNC(rv_conn_post_write_fail, post_write_fail)
//RV_CM_STAT_FUNC(rv_conn_post_write_bytes, post_write_bytes)
RV_CM_STAT_FUNC(rv_conn_outstand_send_write, outstand_send_write)
//RV_CM_STAT_FUNC(rv_conn_send_write_cqe, send_write_cqe)
//RV_CM_STAT_FUNC(rv_conn_send_write_cqe_fail, send_write_cqe_fail)
//RV_CM_STAT_FUNC(rv_conn_recv_write_cqe, recv_write_cqe)
//RV_CM_STAT_FUNC(rv_conn_recv_write_bytes, recv_write_bytes)
//RV_CM_STAT_FUNC(rv_conn_recv_cqe_fail, recv_cqe_fail)
//RV_CM_STAT_FUNC(rv_conn_post_hb, post_hb)
//RV_CM_STAT_FUNC(rv_conn_post_hb_fail, post_hb_fail)
//RV_CM_STAT_FUNC(rv_conn_send_hb_cqe, send_hb_cqe)
//RV_CM_STAT_FUNC(rv_conn_send_hb_cqe_fail, send_hb_cqe_fail)
//RV_CM_STAT_FUNC(rv_conn_recv_hb_cqe, recv_hb_cqe)
#undef RV_CM_STAT_FUNC

static void register_rv_conn_stats(psm2_ep_t ep)
{
	struct psm2_rv_conn_stats *ep_rv_conn_stats = &ep->verbs_ep.rv_conn_stats;

	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECL("rv_q_depth", MPSPAWN_STATS_REDUCTION_ALL,
				rv_q_depth, NULL),
		PSMI_STATS_DECL("rv_reconnect_timeout", MPSPAWN_STATS_REDUCTION_ALL,
				rv_reconnect_timeout, NULL),
		PSMI_STATS_DECL("rv_hb_interval", MPSPAWN_STATS_REDUCTION_ALL,
				rv_hb_interval, NULL),
		PSMI_STATS_DECL("rv_index", MPSPAWN_STATS_REDUCTION_ALL,
				rv_index, NULL),

		PSMI_STATS_DECL("rv_conn_flags", MPSPAWN_STATS_REDUCTION_ALL,
				rv_conn_flags, NULL),

		PSMI_STATS_DECL_FUNC("num_conn", rv_conn_num_conn),
		PSMI_STATS_DECL_FUNC("req_error", rv_conn_req_error),
		PSMI_STATS_DECL_FUNC("req_recv", rv_conn_req_recv),
		PSMI_STATS_DECL_FUNC("rep_error", rv_conn_rep_error),
		PSMI_STATS_DECL_FUNC("rep_recv", rv_conn_rep_recv),
		PSMI_STATS_DECL_FUNC("rtu_recv", rv_conn_rtu_recv),
		PSMI_STATS_DECL_FUNC("established", rv_conn_established),
		PSMI_STATS_DECL_FUNC("dreq_error", rv_conn_dreq_error),
		PSMI_STATS_DECL_FUNC("dreq_recv", rv_conn_dreq_recv),
		PSMI_STATS_DECL_FUNC("drep_recv", rv_conn_drep_recv),
		PSMI_STATS_DECL_FUNC("timewait", rv_conn_timewait),
		PSMI_STATS_DECL_FUNC("mra_recv", rv_conn_mra_recv),
		PSMI_STATS_DECL_FUNC("rej_recv", rv_conn_rej_recv),
		PSMI_STATS_DECL_FUNC("lap_error", rv_conn_lap_error),
		PSMI_STATS_DECL_FUNC("lap_recv", rv_conn_lap_recv),
		PSMI_STATS_DECL_FUNC("apr_recv", rv_conn_apr_recv),
		PSMI_STATS_DECL_FUNC("unexp_event", rv_conn_unexp_event),
		PSMI_STATS_DECL_FUNC("req_sent", rv_conn_req_sent),
		PSMI_STATS_DECL_FUNC("rep_sent", rv_conn_rep_sent),
		PSMI_STATS_DECL_FUNC("rtu_sent", rv_conn_rtu_sent),
		PSMI_STATS_DECL_FUNC("rej_sent", rv_conn_rej_sent),
		PSMI_STATS_DECL_FUNC("dreq_sent", rv_conn_dreq_sent),
		PSMI_STATS_DECL_FUNC("drep_sent", rv_conn_drep_sent),
		PSMI_STATS_DECLU64("wait_time", (uint64_t*)&ep_rv_conn_stats->wait_time),
		PSMI_STATS_DECLU64("resolve_time", (uint64_t*)&ep_rv_conn_stats->resolve_time),
		PSMI_STATS_DECLU64("connect_time", (uint64_t*)&ep_rv_conn_stats->connect_time),
		PSMI_STATS_DECLU64("connected_time", (uint64_t*)&ep_rv_conn_stats->connected_time),
		PSMI_STATS_DECL_FUNC("resolve", rv_conn_resolve),
		PSMI_STATS_DECL_FUNC("resolve_fail", rv_conn_resolve_fail),
		PSMI_STATS_DECL_FUNC("conn_recovery", rv_conn_conn_recovery),
		PSMI_STATS_DECLU64("rewait_time", (uint64_t*)&ep_rv_conn_stats->rewait_time),
		PSMI_STATS_DECLU64("reresolve_time", (uint64_t*)&ep_rv_conn_stats->reresolve_time),
		PSMI_STATS_DECLU64("reconnect_time", (uint64_t*)&ep_rv_conn_stats->reconnect_time),
		PSMI_STATS_DECLU64("max_rewait_time", (uint64_t*)&ep_rv_conn_stats->max_rewait_time),
		PSMI_STATS_DECLU64("max_reresolve_time", (uint64_t*)&ep_rv_conn_stats->max_reresolve_time),
		PSMI_STATS_DECLU64("max_reconnect_time", (uint64_t*)&ep_rv_conn_stats->max_reconnect_time),
		PSMI_STATS_DECL_FUNC("reresolve", rv_conn_reresolve),
		PSMI_STATS_DECL_FUNC("reresolve_fail", rv_conn_reresolve_fail),
		PSMI_STATS_DECLU64("post_write", (uint64_t*)&ep_rv_conn_stats->post_write),
		PSMI_STATS_DECLU64("post_write_fail", (uint64_t*)&ep_rv_conn_stats->post_write_fail),
		PSMI_STATS_DECLU64("post_write_bytes", (uint64_t*)&ep_rv_conn_stats->post_write_bytes),
		PSMI_STATS_DECL_FUNC("send_write_out", rv_conn_outstand_send_write),
		PSMI_STATS_DECLU64("send_write_cqe", (uint64_t*)&ep_rv_conn_stats->send_write_cqe),
		PSMI_STATS_DECLU64("send_write_cqe_fail", (uint64_t*)&ep_rv_conn_stats->send_write_cqe_fail),

		PSMI_STATS_DECLU64("recv_write_cqe", (uint64_t*)&ep_rv_conn_stats->recv_write_cqe),
		PSMI_STATS_DECLU64("recv_write_bytes", (uint64_t*)&ep_rv_conn_stats->recv_write_bytes),
		PSMI_STATS_DECLU64("recv_cqe_fail", (uint64_t*)&ep_rv_conn_stats->recv_cqe_fail),

		PSMI_STATS_DECLU64("post_hb", (uint64_t*)&ep_rv_conn_stats->post_hb),
		PSMI_STATS_DECLU64("post_hb_fail", (uint64_t*)&ep_rv_conn_stats->post_hb_fail),
		PSMI_STATS_DECLU64("send_hb_cqe", (uint64_t*)&ep_rv_conn_stats->send_hb_cqe),
		PSMI_STATS_DECLU64("send_hb_cqe_fail", (uint64_t*)&ep_rv_conn_stats->send_hb_cqe_fail),
		PSMI_STATS_DECLU64("recv_hb_cqe", (uint64_t*)&ep_rv_conn_stats->recv_hb_cqe),
	};

	psmi_stats_register_type("RV_Shared_Conn_RDMA_Statistics",
					PSMI_STATSTYPE_RV_RDMA,
					entries,
					PSMI_STATS_HOWMANY(entries),
					ep->epid, ep, ep->dev_name);
}

static void deregister_rv_conn_stats(psm2_ep_t ep)
{
	psmi_stats_deregister_type(PSMI_STATSTYPE_RV_RDMA, ep);
}

// accessor functions for event statistics
static uint64_t rv_send_write_cqe(void *context)
{
	struct psm2_verbs_ep *vep = &((psm2_ep_t)context)->verbs_ep;
	if (vep->rv) {
		// this is a little sly, we know the stats processing routines will
		// call the accessors in the order from the entries list
		// so we use the 1st of the rv statistics accessors to get
		// the statistics from rv into the cache structure so other accessors
		// can simply return the relevant value
			// we get aggregated values instead of per conn
		(void)__psm2_rv_get_event_stats(vep->rv, &vep->rv_event_stats);
	}
	return vep->rv_event_stats.send_write_cqe;
}

#define RV_EVENT_STAT_FUNC(func, stat) \
	static uint64_t func(void *context) \
	{ \
		struct psm2_verbs_ep *vep = &((psm2_ep_t)context)->verbs_ep; \
		return vep->rv_event_stats.stat; \
	}

//RV_EVENT_STAT_FUNC(rv_send_write_cqe_fail, send_write_cqe_fail)
//RV_EVENT_STAT_FUNC(rv_send_write_bytes, send_write_bytes)

//RV_EVENT_STAT_FUNC(rv_recv_write_cqe, recv_write_cqe)
//RV_EVENT_STAT_FUNC(rv_recv_write_cqe_fail, recv_write_cqe_fail)
//RV_EVENT_STAT_FUNC(rv_recv_write_bytes, recv_write_bytes)
#undef RV_EVENT_STAT_FUNC

static void register_rv_event_stats(psm2_ep_t ep)
{
	struct psm2_rv_event_stats *ep_rv_event_stats = &ep->verbs_ep.rv_event_stats;

	struct psmi_stats_entry entries[] = {
		PSMI_STATS_DECL_FUNC("send_write_cqe", rv_send_write_cqe),
		PSMI_STATS_DECLU64("send_write_cqe_fail", (uint64_t*)&ep_rv_event_stats->send_write_cqe_fail),
		PSMI_STATS_DECLU64("send_write_bytes", (uint64_t*)&ep_rv_event_stats->send_write_bytes),

		PSMI_STATS_DECLU64("recv_write_cqe", (uint64_t*)&ep_rv_event_stats->recv_write_cqe),
		PSMI_STATS_DECLU64("recv_write_cqe_fail", (uint64_t*)&ep_rv_event_stats->recv_write_cqe_fail),
		PSMI_STATS_DECLU64("recv_write_bytes", (uint64_t*)&ep_rv_event_stats->recv_write_bytes),
	};

	psmi_stats_register_type("RV_User_Event_Statistics",
					PSMI_STATSTYPE_RV_EVENT,
					entries,
					PSMI_STATS_HOWMANY(entries),
					ep->epid, ep, ep->dev_name);
}

static void deregister_rv_event_stats(psm2_ep_t ep)
{
	psmi_stats_deregister_type(PSMI_STATSTYPE_RV_EVENT, ep);
}

static psm2_error_t open_rv(psm2_ep_t ep, psm2_uuid_t const job_key)
{
	struct local_info loc_info = { 0 };

	// we always fill in everything we might need in loc_info
	// in some modes, some of the fields are not used by RV
	loc_info.mr_cache_size = ep->rv_mr_cache_size;
#ifdef PSM_CUDA
	/* gpu_cache_size ignored unless RV_RDMA_MODE_GPU */
	loc_info.gpu_cache_size = ep->rv_gpu_cache_size;
#endif
	loc_info.rdma_mode = IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)?
					RV_RDMA_MODE_KERNEL: RV_RDMA_MODE_USER;
#ifdef PSM_CUDA
	if (PSMI_IS_CUDA_ENABLED) {
		// when Cuda is enabled we will have larger window_sz and
		// need to upsize the caches we will use for priority MRs
		if (ep->rdmamode & IPS_PROTOEXP_FLAG_ENABLED) {
			// priority window_sz reg_mr for CPU
			loc_info.rdma_mode |= RV_RDMA_MODE_UPSIZE_CPU;
		}
 		if (psmi_parse_gpudirect()) {
			// When GPU Direct is enabled we need a GPU Cache
			loc_info.rdma_mode |= RV_RDMA_MODE_GPU;
			if ((ep->rdmamode & IPS_PROTOEXP_FLAG_ENABLED)
				&& (psmi_parse_gpudirect_send_limit()
				|| psmi_parse_gpudirect_recv_limit())) {
				// priority window_sz reg_mr for GPU memory
				loc_info.rdma_mode |= RV_RDMA_MODE_UPSIZE_GPU;
			}
		}
	}
#endif

	// need portnum for rdma_mode KERNEL or USER|GPU
	loc_info.port_num = ep->portnum;
	// the rest of loc_info is really only needed for RV_RDMA_MODE_KERNEL
	loc_info.num_conn = ep->rv_num_conn;
	// caller computes our local EPID, but loc_addr must == PSMI_EPID_GET_LID
	// for what will be established as our local epid by psmi_context_open
	// later rem_addr will be compared to this and is based on PSMI_EPID_GET_LID
	// for a remote epid
	if (ep->verbs_ep.port_attr.link_layer == IBV_LINK_LAYER_ETHERNET) {
		// use IPv4 addr in lgid as local address
		loc_info.loc_addr = ep->verbs_ep.ip_addr;
	} else {
		loc_info.loc_addr = ep->verbs_ep.port_attr.lid;
	}
	loc_info.index_bits = RV_INDEX_BITS;
	loc_info.loc_gid_index = ep->verbs_ep.lgid_index;
	loc_info.loc_gid = ep->verbs_ep.lgid;
	// TBD qos_class_sl
	loc_info.job_key_len = min(RV_MAX_JOB_KEY_LEN, sizeof(psm2_uuid_t));
	loc_info.job_key = (uint8_t*)job_key;
	loc_info.service_id = ep->service_id;
	loc_info.context = ep;
	if (IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)) {
		// HFI_TF_NFLOWS (32) limits recv side concurrent tidflows (aka inbound
		// for send we never have more than hfi_num_send_rdma RDMA outstanding
		loc_info.cq_entries = ep->hfi_num_send_rdma + HFI_TF_NFLOWS + 32;
	}
	loc_info.q_depth = ep->rv_q_depth;
	loc_info.reconnect_timeout = ep->rv_reconnect_timeout;
	loc_info.hb_interval = ep->rv_hb_interval;

	ep->verbs_ep.rv =__psm2_rv_open(ep->dev_name, &loc_info);
	if (! ep->verbs_ep.rv) {
		return PSM2_INTERNAL_ERR;
	}
	// parallel psm_hal_gen1/psm_hal_inline_i.h handling HFI1_CAP_GPUDIRECT_OT
	// for OPA psm_context.c, treats CUDA driver w/non-CUDA PSM as fatal
#ifndef RV_CAP_GPU_DIRECT
#ifdef PSM_CUDA
#error "Inconsistent build.  RV_CAP_GPU_DIRECT must be defined for CUDA builds."
#else
// lifted from rv_user_ioctls.h
#define RV_CAP_GPU_DIRECT (1UL << 63)
#endif
#endif
	if (psmi_parse_identify()) {
		if (loc_info.capability & RV_CAP_GPU_DIRECT)
#ifdef PSM_CUDA
			printf("%s %s run-time rv interface v%d.%d%s gpu v%d.%d cuda\n",
			       hfi_get_mylabel(), hfi_ident_tag,
			       loc_info.major_rev,
			       loc_info.minor_rev,
			       (loc_info.capability & RV_CAP_USER_MR)?" mr":"",
			       loc_info.gpu_major_rev,
			       loc_info.gpu_minor_rev);
#else
			printf("%s %s run-time rv interface v%d.%d%s cuda\n",
			       hfi_get_mylabel(), hfi_ident_tag,
			       loc_info.major_rev,
			       loc_info.minor_rev,
			       (loc_info.capability & RV_CAP_USER_MR)?" mr":"");
#endif
		else
			printf("%s %s run-time rv interface v%d.%d%s\n",
			       hfi_get_mylabel(), hfi_ident_tag,
			       loc_info.major_rev,
			       loc_info.minor_rev,
			       (loc_info.capability & RV_CAP_USER_MR)?" mr":"");
	}
	if (loc_info.capability & RV_CAP_USER_MR)
		psmi_hal_add_cap(PSM_HAL_CAP_USER_MR);
	if (loc_info.capability & RV_CAP_EVICT)
		psmi_hal_add_cap(PSM_HAL_CAP_EVICT);
	if (loc_info.capability & RV_CAP_GPU_DIRECT)
		psmi_hal_add_cap(PSM_HAL_CAP_GPUDIRECT_OT);
	ep->verbs_ep.rv_index = loc_info.rv_index;
	ep->rv_mr_cache_size = loc_info.mr_cache_size;
#ifdef PSM_CUDA
	ep->rv_gpu_cache_size = loc_info.gpu_cache_size;
#endif
	ep->rv_q_depth = loc_info.q_depth;
	ep->rv_reconnect_timeout = loc_info.reconnect_timeout;

	return PSM2_OK;
}
#endif // RNDV_MOD

// initialize verbs specific statistics
void
__psm2_ep_initstats_verbs(psm2_ep_t ep)
{
#ifdef RNDV_MOD
	if (ep->verbs_ep.rv && IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)) {
		// only one set of conn stats per job_dev, so
		// no use gathering for any extra QPs we open
		if (ep->mctxt_master == ep)
			register_rv_conn_stats(ep);
		register_rv_event_stats(ep);
	}
#endif

}

static psm2_error_t verbs_open_dev(psm2_ep_t ep, int unit, int port, psm2_uuid_t const job_key)
{
	// similar to code in ifs-all/Topology, enumerates devices and picks one
	int i, num_of_devices;
	struct ibv_device **dev_list = NULL;
	struct ibv_device *ib_dev = NULL;
	int err = PSM2_OK;
	const char *unitpath = sysfs_unit_path(unit);
	uint64_t hi, lo;
	int flags;

	// callers tend not to set port, 0 means any
	if (PSM3_NIC_PORT_ANY == port)
		port = VERBS_PORT;
	ep->portnum = port;
	if (! unitpath) {
		_HFI_ERROR( "NULL sysfs unitpath for unit %d\n", unit);
		return PSM2_INTERNAL_ERR;
	}

	char *dev_name = strrchr(unitpath, '/');
	if (dev_name == NULL) {
		_HFI_ERROR( "invalid sysfs unitpath for unit %d\n", unit);
		return PSM2_INTERNAL_ERR;
	}
	dev_name++; // Inc past last '/'

	ep->dev_name = psmi_strdup(ep, dev_name);
	if (! ep->dev_name) {
		_HFI_ERROR( "can't alloc devname");
		return PSM2_INTERNAL_ERR;
	}

	dev_list = ibv_get_device_list(&num_of_devices);
	if (num_of_devices <= 0) {
		_HFI_ERROR(" Did not detect any RDMA devices \n");
		_HFI_ERROR(" If device exists, check if driver is up\n");
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}
	if (!dev_list) {
		_HFI_ERROR(" Internal error, exiting.\n");
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}

	for (i = 0; i < num_of_devices; i++) {
		if (!strcmp(ibv_get_device_name(dev_list[i]), ep->dev_name))
			break;
	}
	if (i >= num_of_devices) {
		_HFI_ERROR("Unit Id [%d] name %s not found, number of devices is %d\n",
				   unit, ep->dev_name, num_of_devices);
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}
	ep->unit_id = unit;
	_HFI_PRDBG("Using unit_id[%d] %s.\n", ep->unit_id, ep->dev_name);

	ib_dev = dev_list[i];	// device list order may differ from unit order
	ep->verbs_ep.context = ibv_open_device(ib_dev);
	if (! ep->verbs_ep.context) {
		_HFI_ERROR( "Unable to open %s: %s\n", ep->dev_name,
						strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	} else {
		_HFI_PRDBG("Opened %s.\n",ep->dev_name);
	}
	// change async events to non-blocking
	flags = fcntl( ep->verbs_ep.context->async_fd, F_GETFL);
	if (0 > fcntl( ep->verbs_ep.context->async_fd, F_SETFL, flags | O_NONBLOCK)) {
		_HFI_ERROR( "Unable to change file descriptor of async events for %s: %s\n",
					ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	}

	if (ibv_query_port(ep->verbs_ep.context, ep->portnum, &ep->verbs_ep.port_attr)) {
		_HFI_ERROR( "Unable to query port %u of %s: %s\n", ep->portnum,
						ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	} else {
		_HFI_PRDBG("Queried %s.\n",ep->dev_name);
	}

	if (0 != psmi_hal_get_port_subnet(ep->unit_id, ep->portnum,
			&ep->gid_hi, &ep->gid_lo,	// effective subnet and addr in subnet
			&ep->verbs_ep.ip_addr, &ep->verbs_ep.ip_netmask,	// if eth
			&ep->verbs_ep.lgid_index, &hi, &lo)) {
		_HFI_ERROR( "Unable to get subnet for port %u of %s: %s\n", ep->portnum,
						ep->dev_name, strerror(errno));
		err = PSM2_INTERNAL_ERR;
		goto fail;
	} else {
		ep->verbs_ep.lgid.global.subnet_prefix = __cpu_to_be64(hi);
		ep->verbs_ep.lgid.global.interface_id = __cpu_to_be64(lo);
		_HFI_PRDBG("Subnet for port %u of %s: 0x%"PRIx64" addr 0x%"PRIx64" gid 0x%"PRIx64":0x%"PRIx64"\n",
					ep->portnum, ep->dev_name,
					ep->gid_hi, ep->gid_lo, hi, lo);
	}

#ifdef RNDV_MOD
	if (IPS_PROTOEXP_FLAG_KERNEL_QP(ep->rdmamode)
		|| ep->mr_cache_mode == MR_CACHE_MODE_KERNEL ) {
		// open rendezvous module for the same port as our verbs device
		err = open_rv(ep, job_key);
		if (err != PSM2_OK) {
			_HFI_ERROR( "Unable to open rendezvous module for port %u of %s.\n",
				ep->portnum, ep->dev_name);
			// TBD - could ignore error and proceed with UD mode
			//err = PSM2_OK;
			err = PSM2_INTERNAL_ERR;
			goto fail;
		}
		if (ep->mr_cache_mode == MR_CACHE_MODE_KERNEL
			&& ! psmi_hal_has_cap(PSM_HAL_CAP_USER_MR)) {
			_HFI_ERROR( "Rendezvous module lacks enable_user_mr capability.\n");
			// TBD - could ignore error and proceed with UD mode
			//err = PSM2_OK;
			err = PSM2_INTERNAL_ERR;
			goto fail;
		}
	}
#endif

done:
	if (dev_list)
		ibv_free_device_list(dev_list);
	return err;

fail:
	if (ep->verbs_ep.context) {
		ibv_close_device(ep->verbs_ep.context);
		ep->verbs_ep.context = NULL;
	}
	if (ep->dev_name) {
		psmi_free((char*)ep->dev_name);
		ep->dev_name = NULL;
	}
	goto done;
}

static psm2_error_t
check_port_state(psm2_ep_t ep)
{
	uint32_t active_mtu;

	active_mtu = MTU_SIZE(ep->verbs_ep.port_attr.active_mtu);
	if (ep->verbs_ep.port_attr.link_layer == IBV_LINK_LAYER_ETHERNET) {
		_HFI_PRDBG("running on ethernet at %d MTU\n", active_mtu);
	} else {
		_HFI_PRDBG( "running on %s at %d MTU\n", link_layer_str(ep->verbs_ep.port_attr.link_layer), active_mtu);
	}
	if (strcmp("Unknown", link_layer_str(ep->verbs_ep.port_attr.link_layer)) == 0) {
		_HFI_ERROR( "Link layer on port %d of %s is Unknown\n", ep->portnum,
						ep->dev_name);
		return PSM2_INTERNAL_ERR;
	}
	ep->verbs_ep.link_layer = ep->verbs_ep.port_attr.link_layer;

	if (ep->verbs_ep.port_attr.state != IBV_PORT_ACTIVE) {
		_HFI_ERROR( " Port state is not active for %s port %d: %d\n",
						ep->dev_name, ep->portnum,
						ep->verbs_ep.port_attr.state);
		//_HFI_ERROR( " Port number %d on %s state is %s\n",
				//params->ib_port, ep->dev_name,
				//portStates[ep->verbs_ep.port_attr.state]);
		return PSM2_INTERNAL_ERR;
	}

	// compute MTU.
	// ep->mtu is the PSM payload size.  For OPA native mode, this did not
	// include headers as OPA allowed up to an additional 128 bytes of headers.
	// However all UD QP payloads (including PSM headers) are
	// counted toward MTU in UD verbs.  So need to discount by PSM header size
	ep->mtu = active_mtu - MAX_PSM_HEADER;
	_HFI_PRDBG("Max PSM payload (aka MTU): %u\n", ep->mtu);
	// TBD - *act_mtu = defined constant, we can use an eager RC message size
	// for PSM which is larger than packet MTU
	ep->verbs_ep.active_rate = verbs_get_rate(
									ep->verbs_ep.port_attr.active_width,
									ep->verbs_ep.port_attr.active_speed);
	return PSM2_OK;
}

static struct ibv_qp* ud_qp_create(psm2_ep_t ep)
{
	struct ibv_qp* qp = NULL;

	struct ibv_qp_init_attr attr = { 0 };

	attr.qp_context = ep;	// our own pointer
	attr.send_cq = ep->verbs_ep.send_cq;
	attr.recv_cq = ep->verbs_ep.recv_cq;
	// one extra WQE to be safe in case verbs needs a spare WQE
	attr.cap.max_send_wr  = ep->hfi_num_send_wqes+1;
	attr.cap.max_send_sge = 2;
	attr.cap.max_inline_data = ep->hfi_imm_size;

	attr.srq = NULL;
	attr.cap.max_recv_wr  = ep->hfi_num_recv_wqes;
	attr.cap.max_recv_sge = 1;

	attr.qp_type = IBV_QPT_UD;

	qp = ibv_create_qp(ep->verbs_ep.pd, &attr);
	if (qp == NULL && errno == ENOMEM) {
		_HFI_ERROR( "Unable to create UD QP on %s: %s\n",
					ep->dev_name, strerror(errno));
		_HFI_ERROR( "Requested QP size might be too big. Try reducing TX depth and/or inline size.\n");
		_HFI_ERROR( "Requested TX depth was %u and RX depth was %u .\n",
					ep->hfi_num_send_wqes+1, ep->hfi_num_recv_wqes);
	}

	// attr reports what we got, double check and react in case
	ep->verbs_ep.qp_cap = attr.cap;

	// QP adjusted values due to HW limits
	if (ep->hfi_imm_size > attr.cap.max_inline_data) {
		_HFI_PRDBG( "Limited to inline size of %d, requested %u\n",
			attr.cap.max_inline_data, ep->hfi_imm_size);
	} else {
		_HFI_PRDBG("Inline Size: %u\n", attr.cap.max_inline_data);
	}
	if (ep->hfi_num_send_wqes+1 > attr.cap.max_send_wr) {
		_HFI_PRDBG( "Limited to %d SQ WQEs, requested %u\n",
			attr.cap.max_send_wr, ep->hfi_num_send_wqes+1);
	} else {
		_HFI_PRDBG("SQ WQEs: %u\n", attr.cap.max_send_wr);
	}
	if (2 > attr.cap.max_send_sge) {
		_HFI_PRDBG( "Limited to %d SQ SGEs\n",
			attr.cap.max_send_sge);
	}
	if (ep->hfi_num_recv_wqes > attr.cap.max_recv_wr) {
		_HFI_PRDBG( "Limited to %d RQ WQEs, requested %u\n",
			attr.cap.max_recv_wr, ep->hfi_num_recv_wqes);
	} else {
		_HFI_PRDBG("RQ WQEs: %u\n", attr.cap.max_recv_wr);
	}
	if (1 > attr.cap.max_recv_sge) {
		_HFI_PRDBG( "Limited to %d RQ SGEs\n",
			attr.cap.max_recv_sge);
	}

	return qp;
}

static psm2_error_t modify_ud_qp_to_init(psm2_ep_t ep, struct ibv_qp *qp)
{
	struct ibv_qp_attr attr = { 0 };
	int flags = IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT | IBV_QP_QKEY;

	attr.qp_state = IBV_QPS_INIT;
	attr.pkey_index = ep->network_pkey_index;
	attr.port_num =  ep->portnum;
	attr.qkey = ep->verbs_ep.qkey;
	//attr.qp_access_flags N/A for UD
	//flags |= IBV_QP_ACCESS_FLAGS;

	if (ibv_modify_qp(qp, &attr,flags)) {
		_HFI_ERROR( "Failed to modify UD QP to INIT on %s: %s\n",
					ep->dev_name, strerror(errno));
		return PSM2_INTERNAL_ERR;
	}
	return PSM2_OK;
}

static psm2_error_t modify_ud_qp_to_rtr(psm2_ep_t ep,struct ibv_qp *qp)
{
	struct ibv_qp_attr attr = { 0 };
	int flags = IBV_QP_STATE;

	attr.qp_state = IBV_QPS_RTR;

	if (ibv_modify_qp(qp, &attr, flags)) {
		_HFI_ERROR( "Failed to modify UD QP to RTR on %s: %s\n",
					ep->dev_name, strerror(errno));
		return PSM2_INTERNAL_ERR;
	}
	return PSM2_OK;
}

static psm2_error_t modify_ud_qp_to_rts(psm2_ep_t ep, struct ibv_qp *qp)
{
	struct ibv_qp_attr attr = { 0 };
	int flags = IBV_QP_STATE | IBV_QP_SQ_PSN;

	attr.qp_state = IBV_QPS_RTS;
	attr.sq_psn = 0x1234;	// doesn't really matter for UD

	if (ibv_modify_qp(qp, &attr, flags)) {
		_HFI_ERROR( "Failed to modify UD QP to RTS on %s: %s\n",
					ep->dev_name, strerror(errno));
		return PSM2_INTERNAL_ERR;
	}
	return PSM2_OK;
}

struct ibv_qp* rc_qp_create(psm2_ep_t ep, void *context, struct ibv_qp_cap *cap)
{
	struct ibv_qp* qp = NULL;

	struct ibv_qp_init_attr attr;
	memset(&attr, 0, sizeof(struct ibv_qp_init_attr));

	attr.qp_context = context;
	attr.send_cq = ep->verbs_ep.send_cq;
	attr.recv_cq = ep->verbs_ep.recv_cq;
	attr.srq = NULL;
	// one extra WQE to be safe in case verbs needs a spare WQE
	if ((ep->rdmamode&IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC) {
		// need to be prepared in case all sends posted to same RC QP, so
		// match the number of send buffers we plan to allocate
		attr.cap.max_send_wr  = ep->hfi_num_send_wqes+ep->hfi_num_send_rdma+1;
		attr.cap.max_send_sge = 2;
		// inline data helps latency and message rate for small sends
		// Later we may explore use of
		// send SGEs pointing to application buffers, somewhat like WFR send DMA
		attr.cap.max_inline_data = ep->hfi_imm_size;
		attr.cap.max_recv_wr  = ep->hfi_num_recv_wqes/VERBS_RECV_QP_FRACTION;// TBD
		attr.cap.max_recv_sge = 1;
	} else {
		// only RDMA Write w/immediate
		attr.cap.max_send_wr  = ep->hfi_num_send_rdma+1;
		attr.cap.max_send_sge = 1;
		attr.cap.max_inline_data = 0;
		// incoming Write w/immediate consumes a RQ WQE but no buffer needed
		attr.cap.max_recv_wr  = HFI_TF_NFLOWS+1;
		attr.cap.max_recv_sge = 0;
	}

	attr.qp_type = IBV_QPT_RC;

	qp = ibv_create_qp(ep->verbs_ep.pd, &attr);
	if (qp == NULL) {
		_HFI_ERROR( "Unable to create RC QP on %s: %s\n",
					ep->dev_name, strerror(errno));
		_HFI_ERROR( "Requested QP size might be too big. Try reducing TX depth and/or inline size.\n");
		_HFI_ERROR( "Requested TX depth was %u and RX depth was %u .\n",
					ep->hfi_num_send_wqes+1,
					ep->hfi_num_recv_wqes/VERBS_RECV_QP_FRACTION);
		return NULL;
	}

// TBD - getting too small resources should be fatal or adjust limits to be smaller
	if ((ep->rdmamode&IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC) {
		// QP adjusted values due to HW limits
		if (ep->hfi_imm_size > attr.cap.max_inline_data) {
			_HFI_PRDBG( "Limited to inline size of %d, requested %u\n",
				attr.cap.max_inline_data, ep->hfi_imm_size);
		} else {
			_HFI_PRDBG("Inline Size: %u\n", attr.cap.max_inline_data);
		}
		if (ep->hfi_num_send_wqes+ep->hfi_num_send_rdma+1 > attr.cap.max_send_wr) {
			_HFI_PRDBG( "Limited to %d SQ WQEs, requested %u\n",
				attr.cap.max_send_wr, ep->hfi_num_send_wqes+ep->hfi_num_send_rdma+1);
		} else {
			_HFI_PRDBG("SQ WQEs: %u\n", attr.cap.max_send_wr);
		}
		if (2 > attr.cap.max_send_sge) {
			_HFI_PRDBG( "Limited to %d SQ SGEs\n",
				attr.cap.max_send_sge);
		}
		if (ep->hfi_num_recv_wqes/VERBS_RECV_QP_FRACTION > attr.cap.max_recv_wr) {
			_HFI_PRDBG( "Limited to %d RQ WQEs, requested %u\n",
				attr.cap.max_recv_wr, ep->hfi_num_recv_wqes/VERBS_RECV_QP_FRACTION);
		} else {
			_HFI_PRDBG("RQ WQEs: %u\n", attr.cap.max_recv_wr);
		}
		if (1 > attr.cap.max_recv_sge) {
			_HFI_PRDBG( "Limited to %d RQ SGEs\n",
				attr.cap.max_recv_sge);
		}
	} else {
		// QP adjusted values due to HW limits
		if (ep->hfi_num_send_rdma+1 > attr.cap.max_send_wr) {
			_HFI_PRDBG( "Limited to %d SQ WQEs, requested %u\n",
				attr.cap.max_send_wr, ep->hfi_num_send_rdma+1);
		} else {
			_HFI_PRDBG("SQ WQEs: %u\n", attr.cap.max_send_wr);
		}
		if (1 > attr.cap.max_send_sge) {
			_HFI_PRDBG( "Limited to %d SQ SGEs\n",
				attr.cap.max_send_sge);
		}
		if (HFI_TF_NFLOWS+1 > attr.cap.max_recv_wr) {
			_HFI_PRDBG( "Limited to %d RQ WQEs, requested %u\n",
				attr.cap.max_recv_wr, HFI_TF_NFLOWS+1);
		} else {
			_HFI_PRDBG("RQ WQEs: %u\n", attr.cap.max_recv_wr);
		}
	}

	if (cap)
		*cap = attr.cap;
	_HFI_MMDBG("created RC QP %d\n", qp->qp_num);
	return qp;
}

void rc_qp_destroy(struct ibv_qp* qp)
{
	ibv_destroy_qp(qp);
}

psm2_error_t modify_rc_qp_to_init(psm2_ep_t ep, struct ibv_qp *qp)
{
	struct ibv_qp_attr attr = { 0 };
	int flags = IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT;

	attr.qp_state        = IBV_QPS_INIT;
	attr.pkey_index = ep->network_pkey_index;
	attr.port_num =  ep->portnum;

	//attr.qkey = ep->verbs_ep.qkey;
	//flags |= IBV_QP_QKEY;	// only allowed for UD
	attr.qp_access_flags = 0;
	attr.qp_access_flags |= IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_LOCAL_WRITE;
	//attr.qp_access_flags |= IBV_ACCESS_REMOTE_ATOMIC;
	flags |= IBV_QP_ACCESS_FLAGS;

	if (ibv_modify_qp(qp, &attr, flags)) {
		_HFI_ERROR( "Failed to modify RC QP to INIT on %s: %s\n",
					ep->dev_name, strerror(errno));
		return PSM2_INTERNAL_ERR;
	}
	_HFI_MMDBG("moved %d to INIT\n", qp->qp_num);
	return PSM2_OK;
}

// initpsn is from packet we received
// req_attr is from REQ or REP from other side
psm2_error_t modify_rc_qp_to_rtr(psm2_ep_t ep, struct ibv_qp *qp,
				const struct psm_rc_qp_attr *req_attr,
				const ips_path_rec_t *path_rec, uint32_t initpsn)
{
	int flags = IBV_QP_STATE;
	struct ibv_qp_attr attr = { 0 };

	attr.qp_state = IBV_QPS_RTR;

	ips_path_rec_to_ah_attr(ep, path_rec, &attr.ah_attr);
	flags |= IBV_QP_AV;

	// TBD - we already factored in req vs pr to update pr no need
	// for modify_cq_qp_to_rtr to repeat it
	// pr_mtu is max PSM paylod in bytes and req_attr_mtu is IB enum
	attr.path_mtu = MIN(opa_mtu_int_to_enum(path_rec->pr_mtu), req_attr->mtu);
	attr.dest_qp_num = req_attr->qpn;
	attr.rq_psn = initpsn;
	flags |= (IBV_QP_PATH_MTU | IBV_QP_DEST_QPN | IBV_QP_RQ_PSN);

	_HFI_PRDBG("set max_dest_rd_atomic to %u\n", attr.max_dest_rd_atomic);
	attr.min_rnr_timer = 12;	// TBD well known
	flags |= (IBV_QP_MIN_RNR_TIMER | IBV_QP_MAX_DEST_RD_ATOMIC);

	if (ibv_modify_qp(qp, &attr, flags)) {
		_HFI_ERROR( "Failed to modify RC QP to RTR on %s: %s\n",
					ep->dev_name, strerror(errno));
		return PSM2_INTERNAL_ERR;
	}
	_HFI_MMDBG("moved %d to RTR\n", qp->qp_num);

	return PSM2_OK;
}

// initpsn is value we sent in our req and rep
// req_attr is from REP we received from other side
psm2_error_t modify_rc_qp_to_rts(psm2_ep_t ep, struct ibv_qp *qp,
				const struct psm_rc_qp_attr *req_attr, uint32_t initpsn)
{
	int flags = IBV_QP_STATE;
	struct ibv_qp_attr attr = { 0 };

	attr.qp_state = IBV_QPS_RTS;

	attr.sq_psn = initpsn;	// value we told other side
	flags |= IBV_QP_SQ_PSN;

	_HFI_PRDBG("set max_rd_atomic to %u\n", attr.max_rd_atomic);
	flags |=  IBV_QP_MAX_QP_RD_ATOMIC;

	attr.retry_cnt = ep->hfi_qp_retry;
	attr.rnr_retry = ep->hfi_qp_retry;	// only for eager RC QP rdmamode
	attr.timeout = ep->hfi_qp_timeout;
	flags |= IBV_QP_RETRY_CNT | IBV_QP_RNR_RETRY | IBV_QP_TIMEOUT;

	_HFI_MMDBG("moving %d to RTS\n", qp->qp_num);
	if (ibv_modify_qp(qp, &attr, flags)) {
		_HFI_ERROR( "Failed to modify RC QP to RTS on %s: %s\n",
						ep->dev_name, strerror(errno));
		return PSM2_INTERNAL_ERR;
	}
	//__psm2_dump_verbs_qp(qp);
	return PSM2_OK;
}

/******************************************************************************
 *  * Try to map verbs' link layer types to a descriptive string or "Unknown"
 *   ******************************************************************************/
static const char *link_layer_str(int8_t link_layer)
{
    switch (link_layer) {

        case IBV_LINK_LAYER_UNSPECIFIED:
        case IBV_LINK_LAYER_INFINIBAND:
            return "IB";
        case IBV_LINK_LAYER_ETHERNET:
            return "Ethernet";
        default:
            return "Unknown";
    }
}

int __psm2_nonzero_gid(const union ibv_gid *gid)
{
	static union ibv_gid zero_gid = { { 0 } };

	return memcmp(gid, &zero_gid, sizeof(*gid)) != 0;
}

char *
__psm2_dump_gid(union ibv_gid *gid, char *buf, size_t bufsize)
{
	snprintf(buf, bufsize, "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:"
	                       "%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
		gid->raw[0], gid->raw[1], gid->raw[2], gid->raw[3],
		gid->raw[4], gid->raw[5], gid->raw[6], gid->raw[7],
		gid->raw[8], gid->raw[9], gid->raw[10], gid->raw[11],
		gid->raw[12], gid->raw[13], gid->raw[14], gid->raw[15]);

	return buf;
}

void
__psm2_dump_verbs_ep(psm2_ep_t ep, unsigned igid)
{
	struct psm2_verbs_ep *vep = &(ep->verbs_ep);
	union ibv_gid gid;

	printf("ib_devname = %s\n", ep->dev_name);
	printf("qp_num     = %u\n", vep->qp->qp_num);
	printf("GID        = ");
	if (0 == ibv_query_gid(vep->context, ep->portnum, igid, &gid)) {
		char buf[80];
		printf("%s\n", __psm2_dump_gid(&gid, buf, sizeof(buf)));
	} else {
		printf("unavailable.\n");
	}
}

void
__psm2_dump_verbs_qp(struct ibv_qp *qp)
{
	struct ibv_qp_attr attr;
	struct ibv_qp_init_attr init_attr;
	int mask = IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT | IBV_QP_CAP
			/*| IBV_QP_RATE_LIMIT*/ ;
	if (qp->qp_type == IBV_QPT_RC) {
		mask |= IBV_QP_ACCESS_FLAGS | IBV_QP_AV | IBV_QP_PATH_MTU
				| IBV_QP_TIMEOUT | IBV_QP_RETRY_CNT | IBV_QP_RNR_RETRY
				| IBV_QP_RQ_PSN | IBV_QP_MAX_QP_RD_ATOMIC
				// | IBV_QP_ALT_PATH
				| IBV_QP_MIN_RNR_TIMER | IBV_QP_SQ_PSN
				| IBV_QP_MAX_DEST_RD_ATOMIC | IBV_QP_PATH_MIG_STATE
				| IBV_QP_DEST_QPN;
	} else {
		mask |= IBV_QP_QKEY;
	}
	if (ibv_query_qp(qp, &attr, mask, &init_attr)) {
			printf("unable to query QP\n");
			return;
	}
	// rate_limit field not available in some versions of verbs.h
	//printf("QP %p (%u), type %u state %u PkeyIndx %u Port %u rate %u draining %u\n",
	//		qp, qp->qp_num, qp->qp_type, attr.qp_state, attr.pkey_index,
	//		attr.port_num, attr.rate_limit, attr.sq_draining);
	printf("QP %p (%u), type %u state %u PkeyIndx %u Port %u draining %u\n",
			qp, qp->qp_num, qp->qp_type, attr.qp_state, attr.pkey_index,
			attr.port_num, attr.sq_draining);
	printf("  send: wr %u sge %u inline %u recv: wr %u sqe %u\n",
			attr.cap.max_send_wr, attr.cap.max_send_sge, attr.cap.max_inline_data,
			attr.cap.max_recv_wr, attr.cap.max_recv_sge);
	printf("  context %p send_cq %p recv_cq %p srq %p sg_sig_all %u\n",
			init_attr.qp_context, init_attr.send_cq, init_attr.recv_cq,
			init_attr.srq, init_attr.sq_sig_all);
	if (qp->qp_type == IBV_QPT_RC) {
		char buf[80];
		printf("  mtu %u mig %u rq_psn %u sq_psn %u dest_qp %u access %u\n",
			attr.path_mtu, attr.path_mig_state, attr.rq_psn, attr.sq_psn,
			attr.dest_qp_num, attr.qp_access_flags);
		printf("  max_rd_atomic %u max_dest_rd_atomic %u\n",
			attr.max_rd_atomic, attr.max_dest_rd_atomic);
		printf("  min_rnr_timer %u timeout %u retry_cnt %u rnr_retry %u\n",
			attr.min_rnr_timer, attr.timeout, attr.retry_cnt, attr.rnr_retry);
		printf("  ah_attr:  port %u dlid %u sl %u src_path_bits %u rate %u global %u\n",
			attr.ah_attr.port_num, attr.ah_attr.dlid,
			attr.ah_attr.sl,
			attr.ah_attr.src_path_bits, attr.ah_attr.static_rate,
			attr.ah_attr.is_global);
		if (attr.ah_attr.is_global) {
			printf("           dgid: %s\n",
				__psm2_dump_gid(&attr.ah_attr.grh.dgid, buf, sizeof(buf)));
			printf("           flow %u sgid_idx %u hop %u tc %u\n",
				attr.ah_attr.grh.flow_label, attr.ah_attr.grh.sgid_index,
				attr.ah_attr.grh.hop_limit, attr.ah_attr.grh.traffic_class);
		}
		printf("  alt_ah_attr:  port %u dlid %u sl %u src_path_bits %u rate %u global %u\n",
			attr.alt_ah_attr.port_num, attr.alt_ah_attr.dlid,
			attr.alt_ah_attr.sl,
			attr.alt_ah_attr.src_path_bits, attr.alt_ah_attr.static_rate,
			attr.alt_ah_attr.is_global);
		if (attr.alt_ah_attr.is_global) {
			printf("              dgid: %s\n",
				__psm2_dump_gid(&attr.alt_ah_attr.grh.dgid, buf, sizeof(buf)));
			printf("              flow %u sgid_idx %u hop %u tc %u\n",
				attr.alt_ah_attr.grh.flow_label, attr.alt_ah_attr.grh.sgid_index,
				attr.alt_ah_attr.grh.hop_limit, attr.alt_ah_attr.grh.traffic_class);
		}
		printf("  alt pkey idx %u alt port %u alt timeout %u\n",
			attr.alt_pkey_index, attr.alt_port_num, attr.alt_timeout);
	} else {
			printf("qkey: 0x%x\n", attr.qkey);
	}
	return;
}

static enum psm_ibv_rate verbs_get_rate(uint8_t width, uint8_t speed)
{
	switch (width) {
	case 1: /* 1x */
		switch (speed) {
		case 1: return PSM_IBV_RATE_2_5_GBPS;
		case 2: return PSM_IBV_RATE_5_GBPS;
		case 4: /* fall through */
		case 8: return PSM_IBV_RATE_10_GBPS;
		case 16: return PSM_IBV_RATE_14_GBPS;
		case 32: return PSM_IBV_RATE_25_GBPS;
		case 64: return PSM_IBV_RATE_50_GBPS;
		default:
				_HFI_ERROR( "unknown link speed 0x%x\n", speed);
				return PSM_IBV_RATE_100_GBPS;
		}
	case 2: /* 4x */
		switch (speed) {
		case 1: return PSM_IBV_RATE_10_GBPS;
		case 2: return PSM_IBV_RATE_20_GBPS;
		case 4: /* fall through */
		case 8: return PSM_IBV_RATE_40_GBPS;
		case 16: return PSM_IBV_RATE_56_GBPS;
		case 32: return PSM_IBV_RATE_100_GBPS;
		case 64: return PSM_IBV_RATE_200_GBPS;
		default:
				_HFI_ERROR( "unknown link speed 0x%x\n", speed);
				return PSM_IBV_RATE_100_GBPS;
		}
	case 4: /* 8x */
		switch (speed) {
		case 1: return PSM_IBV_RATE_20_GBPS;
		case 2: return PSM_IBV_RATE_40_GBPS;
		case 4: /* fall through */
		case 8: return PSM_IBV_RATE_80_GBPS;
		case 16: return PSM_IBV_RATE_112_GBPS;
		case 32: return PSM_IBV_RATE_200_GBPS;
		case 64: return PSM_IBV_RATE_400_GBPS;
		default:
				_HFI_ERROR( "unknown link speed 0x%x\n", speed);
				return PSM_IBV_RATE_100_GBPS;
		}
	case 8: /* 12x */
		switch (speed) {
		case 1: return PSM_IBV_RATE_30_GBPS;
		case 2: return PSM_IBV_RATE_60_GBPS;
		case 4: /* fall through */
		case 8: return PSM_IBV_RATE_120_GBPS;
		case 16: return PSM_IBV_RATE_168_GBPS;
		case 32: return PSM_IBV_RATE_300_GBPS;
		case 64: return PSM_IBV_RATE_600_GBPS;
		default:
				_HFI_ERROR( "unknown link speed 0x%x\n", speed);
				return PSM_IBV_RATE_100_GBPS;
		}
	case 16: /* 2x */
		switch (speed) {
		case 1: return PSM_IBV_RATE_5_GBPS;
		case 2: return PSM_IBV_RATE_10_GBPS;
		case 4: /* fall through */
		case 8: return PSM_IBV_RATE_20_GBPS;
		case 16: return PSM_IBV_RATE_28_GBPS;
		case 32: return PSM_IBV_RATE_50_GBPS;
		case 64: return PSM_IBV_RATE_100_GBPS;
		default:
				_HFI_ERROR( "unknown link speed 0x%x\n", speed);
				return PSM_IBV_RATE_100_GBPS;
		}
	default:
		_HFI_ERROR( "unknown link width 0x%x\n", width);
		return PSM_IBV_RATE_100_GBPS;
	}
}

// unfortunately ibv_rate_to_mult and mult_to_ibv_rate have a bug as they
// omit 100g rate and some others, so we create our own
static int my_ibv_rate_to_mult(enum psm_ibv_rate rate)
{
	switch (rate) {
	case PSM_IBV_RATE_2_5_GBPS: return  1;
	case PSM_IBV_RATE_5_GBPS:   return  2;
	case PSM_IBV_RATE_10_GBPS:  return  4;
	case PSM_IBV_RATE_20_GBPS:  return  8;
	case PSM_IBV_RATE_30_GBPS:  return 12;
	case PSM_IBV_RATE_40_GBPS:  return 16;
	case PSM_IBV_RATE_60_GBPS:  return 24;
	case PSM_IBV_RATE_80_GBPS:  return 32;
	case PSM_IBV_RATE_120_GBPS: return 48;
	case PSM_IBV_RATE_14_GBPS:	return 5;
	case PSM_IBV_RATE_56_GBPS:	return 22;
	case PSM_IBV_RATE_112_GBPS:	return 44;
	case PSM_IBV_RATE_168_GBPS:	return 67;
	case PSM_IBV_RATE_25_GBPS:	return 10;
	case PSM_IBV_RATE_100_GBPS:	return 40;
	case PSM_IBV_RATE_200_GBPS:	return 80;
	case PSM_IBV_RATE_300_GBPS:	return 120;
	case PSM_IBV_RATE_28_GBPS:  return 11;
	case PSM_IBV_RATE_50_GBPS:  return 20;
	case PSM_IBV_RATE_400_GBPS: return 160;
	case PSM_IBV_RATE_600_GBPS: return 240;
	default:           return 40;
	}
}

static enum psm_ibv_rate my_mult_to_ibv_rate(int mult)
{
	switch (mult) {
	case 1:  return PSM_IBV_RATE_2_5_GBPS;
	case 2:  return PSM_IBV_RATE_5_GBPS;
	case 4:  return PSM_IBV_RATE_10_GBPS;
	case 8:  return PSM_IBV_RATE_20_GBPS;
	case 12: return PSM_IBV_RATE_30_GBPS;
	case 16: return PSM_IBV_RATE_40_GBPS;
	case 24: return PSM_IBV_RATE_60_GBPS;
	case 32: return PSM_IBV_RATE_80_GBPS;
	case 48: return PSM_IBV_RATE_120_GBPS;
	case 5:  return PSM_IBV_RATE_14_GBPS;
	case 22: return PSM_IBV_RATE_56_GBPS;
	case 44: return PSM_IBV_RATE_112_GBPS;
	case 67: return PSM_IBV_RATE_168_GBPS;
	case 10: return PSM_IBV_RATE_25_GBPS;
	case 40: return PSM_IBV_RATE_100_GBPS;
	case 80: return PSM_IBV_RATE_200_GBPS;
	case 120: return PSM_IBV_RATE_300_GBPS;
	case 11: return PSM_IBV_RATE_28_GBPS;
	case 20: return PSM_IBV_RATE_50_GBPS;
	case 160: return PSM_IBV_RATE_400_GBPS;
	case 240: return PSM_IBV_RATE_600_GBPS;
	default: return PSM_IBV_RATE_100_GBPS;
    }
}


enum psm_ibv_rate min_rate(enum psm_ibv_rate a, enum psm_ibv_rate b)
{
	// unfortunately the ibv_rate enum is not sorted by link rate
	// so we must convert to "mult" to compare then convert back
	return my_mult_to_ibv_rate(min(my_ibv_rate_to_mult(a),
                                 my_ibv_rate_to_mult(b)));
}


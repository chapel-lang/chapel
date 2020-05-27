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

#include <ofi.h>
#include <ofi_enosys.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

#include "rdma/bgq/fi_bgq_spi.h"
#include "rdma/bgq/fi_bgq_rx.h"

static int fi_bgq_close_stx_nofree(struct fi_bgq_stx *bgq_stx)
{
	int ret;

	ret = fi_bgq_ref_finalize(&bgq_stx->ref_cnt, "shared context");
	if (ret)
		return ret;

	ret = fi_bgq_ref_dec(&bgq_stx->domain->ref_cnt, "domain");
	if (ret)
		return ret;

	return 0;
}

static int fi_bgq_close_stx(fid_t fid)
{
	int ret;
	ret = fi_bgq_fid_check(fid, FI_CLASS_STX_CTX, "shared context");
	if (ret)
		return ret;

	struct fi_bgq_stx *bgq_stx =
		container_of(fid, struct fi_bgq_stx, stx_fid);

	ret = fi_bgq_close_stx_nofree(bgq_stx);
	if (ret)
		return ret;

	free(bgq_stx);
	return 0;
}

int fi_bgq_bind_ep_stx(struct fi_bgq_ep *bgq_ep,
		struct fi_bgq_stx *bgq_stx, uint64_t flags)
{
	if (!bgq_ep || !bgq_stx)
		goto err;

	bgq_ep->tx.stx = bgq_stx;

	fi_bgq_ref_inc(&bgq_stx->ref_cnt, "shared context");

	return 0;

err:
	return -errno;
}

static struct fi_ops fi_bgq_stx_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_bgq_close_stx,
	.bind		= fi_no_bind,
	.control	= fi_no_control,
	.ops_open	= fi_no_ops_open
};

static struct fi_ops_ep fi_bgq_stx_ep_ops = {
	.size		= sizeof(struct fi_ops_ep),
	.cancel		= fi_no_cancel,
	.getopt		= fi_no_getopt,
	.setopt		= fi_no_setopt,
	.tx_ctx		= fi_no_tx_ctx,
	.rx_ctx		= fi_no_rx_ctx
};

int fi_bgq_stx_init (struct fi_bgq_domain *bgq_domain, struct fi_tx_attr *attr,
		struct fi_bgq_stx *bgq_stx, void *context) {

	bgq_stx->stx_fid.fid.fclass = FI_CLASS_STX_CTX;
	bgq_stx->stx_fid.fid.context= context;
	bgq_stx->stx_fid.fid.ops    = &fi_bgq_stx_ops;
	bgq_stx->stx_fid.ops 	    = &fi_bgq_stx_ep_ops;

	bgq_stx->domain = bgq_domain;

	bgq_stx->attr = attr ? *attr : *fi_bgq_global.default_tx_attr;

	l2atomic_lock_acquire(&bgq_domain->mu.lock);

	/*
	 * Three options for number of rget fifos to allocate for each tx context:
	 *
	 * 1. 1 rget fifo
	 *    - same for all ppn (1..64)
	 *    - maximizes the number of tx contexts that can be created
	 *    - simplest implementation
	 *    - lowest off-node bandwidth
	 *    - can support multiple domains at all ppn
	 *
	 * 2. 6 rget fifos (a,b,c,d,e,local)
	 *    - same for all ppn (1..64)
	 *    - one subgroup for each tx context (including a single tx injfifo)
	 *    - best for 64 ppn, half off-node bandwidth for all other ppn
	 *    - can only support one domain at 64 ppn
	 *    - requires rget fifo hash algorithm
	 *
	 * 3. 11 rget fifos (+a,-a,+b,-b,+c,-c,+d,-d,+e,-e,local)
	 *    - must use 6 rget fifos (option 1) for 64 ppn
	 *    - complex implementation
	 *    - optimial off-node bandwidth
	 *    - can only support one domain at 64 ppn and 32 ppn
	 *    - requires rget fifo hash algorithm
	 *
	 * Allocating per-tx rget injection fifos could eliminate potential
	 * "rget injection fifo full" MU errors if the number of outstanding
	 * rget injection payload descriptors can be determined. This will
	 * limit the maximum number of tx contexts, and each individual rget
	 * injection fifo will not be saturated - only 11 rget injection fifos
	 * FOR EACH NODE are needed to maximize off-node aggregate bandwidth.
	 *
	 * For now, use option 1.
	 */
	uint32_t rget_fifos_to_allocate = 1;

	/*
	 * initialize the rget injection fifo(s) used for rendezvous; begin at
	 * fifo 0 of subgroup 0 and iterate *up* until an unallocated rget fifo
	 * is found
	 */
	if (rget_fifos_to_allocate !=
		fi_bgq_spi_injfifo_init(&bgq_stx->rgetfifo,
			&bgq_stx->rgetfifo_subgroup,
			rget_fifos_to_allocate,
			FI_BGQ_TX_SIZE,
			0,	/* immediate_payload_sizeof */
			1,	/* is_remote_get */
			0))	/* is_top_down */
	{
		goto err;
	}

	/*
	 * Three options for number of injection fifos to allocate for each tx context:
	 *
	 * 1. 1 injection fifo
	 *    - same for all ppn (1..64)
	 *    - maximizes the number of tx contexts that can be created
	 *    - simplest implementation
	 *    - lowest off-node bandwidth
	 *    - can support multiple domains at all ppn
	 *    - may be sufficient because only single-packet messages are injected
	 *
	 * 2. 11 injection fifos (+a,-a,+b,-b,+c,-c,+d,-d,+e,-e,local)
	 *    - must use 6 rget fifos (option 1) for 64 ppn
	 *    - complex implementation
	 *    - optimial off-node bandwidth
	 *    - can only support one domain at 64 ppn and 32 ppn
	 *    - requires injection fifo pinning algorithm
	 *
	 * For now, use option 1.
	 */
	uint32_t inj_fifos_to_allocate = 1;

	/*
	 * initialize the transmit injection fifo; begin at fifo 7 of subgroup 15
	 * and iterate *down* until an unallocated fifo is found
	 */
	if (inj_fifos_to_allocate !=
		fi_bgq_spi_injfifo_init(&bgq_stx->injfifo,
			&bgq_stx->injfifo_subgroup,
			inj_fifos_to_allocate,
			FI_BGQ_TX_SIZE,
			sizeof(union fi_bgq_mu_packet_payload),
			0,	/* is_remote_get */
			1))	/* is_top_down */
	{
		goto err;
	}

	l2atomic_lock_release(&bgq_domain->mu.lock);

	fi_bgq_ref_init(&bgq_domain->fabric->node, &bgq_stx->ref_cnt, "shared context");
	fi_bgq_ref_inc(&bgq_domain->ref_cnt, "domain");

	return 0;
err:
	if (l2atomic_lock_isbusy(&bgq_domain->mu.lock)) {
		l2atomic_lock_release(&bgq_domain->mu.lock);
	}
	/* TODO  - other cleanup */
	return -1;
}

int fi_bgq_stx_context(struct fid_domain *domain, struct fi_tx_attr *attr,
	       struct fid_stx **stx, void *context)
{
	int ret;
	errno = 0;
	struct fi_bgq_stx *bgq_stx = NULL;
	struct fi_bgq_domain *bgq_domain =
		container_of(domain, struct fi_bgq_domain, domain_fid);

	if (!domain || !stx) {
		errno = FI_EINVAL;
		return -errno;
	}

	ret = fi_bgq_fid_check(&domain->fid, FI_CLASS_DOMAIN, "domain");
	if (ret)
		return ret;

	bgq_stx = calloc(1, sizeof *bgq_stx);
	if (!bgq_stx) {
		errno = FI_ENOMEM;
		goto err;
	}

	if (fi_bgq_stx_init(bgq_domain, attr, bgq_stx, context)) {
		errno = FI_EOTHER;
		goto err;
	}

	*stx = &bgq_stx->stx_fid;
	return FI_SUCCESS;

err:
	if (bgq_stx) {
		free(bgq_stx);
		bgq_stx = NULL;
	}
	return -errno;
}

static int fi_bgq_close_ep(fid_t fid)
{
	if (!fid) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"NULL ep object");
		errno = FI_EINVAL;
		return -errno;
	}

	if (fid->fclass != FI_CLASS_EP &&
			fid->fclass != FI_CLASS_TX_CTX &&
			fid->fclass != FI_CLASS_RX_CTX) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
			"wrong type of object. expected (FI_CLASS_EP), got (%d)\n",
			fid->fclass);
		errno = FI_EINVAL;
		return -errno;
	}

	int ret;
	struct fi_bgq_ep *bgq_ep = container_of(fid, struct fi_bgq_ep, ep_fid);

	/* disable async progress threads */
	fi_bgq_progress_ep_disable(bgq_ep);

	ret = fi_bgq_ref_dec(&bgq_ep->domain->ref_cnt, "domain");
	if (ret)
		return ret;

	/* av is only valid/required if tx capability is enabled */
	if (bgq_ep->av) {
		ret = fi_bgq_ref_dec(&bgq_ep->av->ref_cnt, "address vector");
		if (ret) return ret;
	}

	if (bgq_ep->sep) {
		ret = fi_bgq_ref_dec(&bgq_ep->sep->ref_cnt, "scalable endpoint");
		if (ret) return ret;
	}

	if (bgq_ep->tx.stx) {
		ret = fi_bgq_ref_dec(&bgq_ep->tx.stx->ref_cnt, "shared tx context");
		if (ret) return ret;

		if (bgq_ep->tx.stx == &bgq_ep->tx.exclusive_stx) {
			ret = fi_bgq_close_stx_nofree(bgq_ep->tx.stx);
			if (ret) return ret;
		}
		bgq_ep->tx.stx = NULL;
	}

	if (bgq_ep->send_cq) {
		ret = fi_bgq_ref_dec(&bgq_ep->send_cq->ref_cnt, "completion queue");
		if (ret) return ret;
	}
	if (bgq_ep->recv_cq) {
		ret = fi_bgq_ref_dec(&bgq_ep->recv_cq->ref_cnt, "completion queue");
		if (ret) return ret;
	}

	if (ofi_recv_allowed(bgq_ep->rx.caps) ||
		ofi_rma_target_allowed(bgq_ep->rx.caps)) {
	}

	fi_bgq_finalize_cm_ops(bgq_ep);
	fi_bgq_finalize_msg_ops(bgq_ep);
	fi_bgq_finalize_rma_ops(bgq_ep);
	fi_bgq_finalize_tagged_ops(bgq_ep);
	fi_bgq_finalize_atomic_ops(bgq_ep);

	void *mem = bgq_ep->mem;
	free(mem);

	return 0;
}

static int fi_bgq_bind_ep(struct fid *fid, struct fid *bfid,
		uint64_t flags)
{
if (!bfid) return 0;
	int ret = 0;
	struct fi_bgq_ep *bgq_ep = container_of(fid, struct fi_bgq_ep, ep_fid);

	switch (bfid->fclass) {
	case FI_CLASS_CNTR:
		ret = fi_bgq_bind_ep_cntr(bgq_ep,
				container_of(bfid, struct fi_bgq_cntr, cntr_fid), flags);
		if (ret)
			goto err;
		break;
	case FI_CLASS_CQ:
		ret = fi_bgq_bind_ep_cq(bgq_ep,
				container_of(bfid, struct fi_bgq_cq, cq_fid), flags);
		if (ret)
			goto err;
		break;
	case FI_CLASS_AV:
		ret = fi_bgq_bind_ep_av(bgq_ep,
				container_of(bfid, struct fi_bgq_av, av_fid), flags);
		if (ret)
			goto err;
		break;
	case FI_CLASS_MR:
		ret = fi_bgq_bind_ep_mr(bgq_ep,
				container_of(bfid, struct fi_bgq_mr, mr_fid), flags);
		if (ret)
			goto err;
		break;
	case FI_CLASS_STX_CTX:
		ret = fi_bgq_bind_ep_stx(bgq_ep,
				container_of(bfid, struct fi_bgq_stx, stx_fid),
				flags);
		if (ret)
			goto err;
		break;
	default:
		errno = FI_ENOSYS;
		goto err;
	}
	return ret;
err:
	return -errno;
}

static int fi_bgq_check_ep(struct fi_bgq_ep *bgq_ep)
{


	switch (bgq_ep->ep_fid.fid.fclass) {
	case FI_CLASS_EP:
		if (!bgq_ep->av) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
					"no AV supplied");
			goto err;
		}
		break;
	case FI_CLASS_RX_CTX:
	case FI_CLASS_TX_CTX:
		if (!bgq_ep->sep) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
					"no Scalable Endpoint supplied");
			goto err;
		}
		bgq_ep->av = bgq_ep->sep->av;
		if (!bgq_ep->sep->av) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"no AV supplied on Scalable Endpoint");
			goto err;
		}
		break;
	default:
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"Invalid EP class %lu",
				bgq_ep->ep_fid.fid.fclass);
		goto err;
	}

	if (!bgq_ep->domain) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"no domain supplied\n");
		goto err;
	}

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

static int fi_bgq_ep_tx_init (struct fi_bgq_ep *bgq_ep,
		struct fi_bgq_domain *bgq_domain)
{
	assert(bgq_ep);
	assert(bgq_domain);
	assert(bgq_ep->tx.state == FI_BGQ_EP_UNINITIALIZED);

	if (bgq_ep->tx.stx) {
#ifdef FI_BGQ_TRACE
		fprintf(stderr,"fi_bgq_ep_tx_init - using tx shared on node not picking new fifos\n");
		fflush(stderr);
#endif
		assert(bgq_domain == bgq_ep->tx.stx->domain);

	} else {
		/*
		 * A shared transmit context was not provided; create an
		 * "exclusive" shared transmit context for use by only this
		 * endpoint transmit context
		 */
#ifdef FI_BGQ_TRACE
		fprintf(stderr,"fi_bgq_ep_tx_init - picking new fifos for new tx\n");
		fflush(stderr);
#endif
		if (fi_bgq_stx_init(bgq_domain, 0, &bgq_ep->tx.exclusive_stx, NULL)) {
			return -1;
		}
		bgq_ep->tx.stx = &bgq_ep->tx.exclusive_stx;
		fi_bgq_ref_inc(&bgq_ep->tx.stx->ref_cnt, "exclusive shared context");
	}

	bgq_ep->threading = (uint32_t) bgq_domain->threading;
	bgq_ep->av_type = (uint32_t) bgq_ep->av->type;
	bgq_ep->mr_mode = (uint32_t) bgq_domain->mr_mode;

	/* copy the 'shared tx' resources and information */
	fi_bgq_spi_injfifo_clone(&bgq_ep->tx.injfifo, &bgq_ep->tx.stx->injfifo);

	BG_CoordinateMapping_t my_coords = bgq_domain->my_coords;

	const uint32_t fifo_map =
		fi_bgq_mu_calculate_fifo_map_single(my_coords, my_coords);

	const MUHWI_Destination_t destination =
		fi_bgq_spi_coordinates_to_destination(my_coords);

	const uint32_t base_rx =
		fi_bgq_addr_calculate_base_rx(my_coords.t, Kernel_ProcessCount());

	const union fi_bgq_addr self = {.fi=fi_bgq_addr_create(destination, fifo_map, base_rx)};

#ifdef FI_BGQ_TRACE
	fprintf(stderr,"fi_bgq_ep_tx_init created addr:\n");
	FI_BGQ_ADDR_DUMP((fi_addr_t *)&self.fi);
#endif
	/*
	 *  fi_[t]send*() descriptor models
	 */
	{	/* send model */
		MUHWI_Descriptor_t * desc = &bgq_ep->tx.send.send_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_FIFO;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;

		union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		fi_bgq_mu_packet_type_set(hdr, FI_BGQ_MU_PACKET_TYPE_TAG|FI_BGQ_MU_PACKET_TYPE_EAGER);

		hdr->pt2pt.uid.fi = self.uid.fi;
		hdr->pt2pt.immediate_data = 0;
		hdr->pt2pt.ofi_tag = (uint64_t)-1;

		/* specified at injection time */
		desc->Pa_Payload = 0;
		desc->Message_Length = 0;
		desc->Torus_FIFO_Map = 0;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = -1;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id = -1;


		/* send rendezvous models */
		desc = &bgq_ep->tx.send.rzv_model[0];	/* "internode" */	/* TODO - use an enum */
		*desc = bgq_ep->tx.send.send_model;

		desc->Message_Length = sizeof(struct fi_bgq_mu_iov) + offsetof(union fi_bgq_mu_packet_payload, rendezvous.mu_iov);

		hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		fi_bgq_mu_packet_type_set(hdr, FI_BGQ_MU_PACKET_TYPE_TAG|FI_BGQ_MU_PACKET_TYPE_RENDEZVOUS);
		hdr->pt2pt.rendezvous.is_local = 0;
		hdr->pt2pt.rendezvous.niov_minus_1 = 0;
		hdr->pt2pt.rendezvous.rget_inj_fifo_id = bgq_ep->tx.stx->rgetfifo.node_scoped_fifo_id;

		desc = &bgq_ep->tx.send.rzv_model[1];	/* "intranode" */	/* TODO - use an enum */
		*desc = bgq_ep->tx.send.rzv_model[0];
		hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		hdr->pt2pt.rendezvous.is_local = 1;

#ifdef FI_BGQ_REMOTE_COMPLETION
		/* remote completion model - used for FI_DELIVERY_COMPLETE */
		desc = &bgq_ep->tx.send.remote_completion_model;
		*desc = bgq_ep->tx.send.send_model;

		hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		fi_bgq_mu_packet_type_set(hdr, FI_BGQ_MU_PACKET_TYPE_EAGER|FI_BGQ_MU_PACKET_TYPE_ACK);
		hdr->completion.origin = fi_bgq_uid_get_destination(self.uid.fi);

		/* specified at injection time */
		hdr->completion.is_local = 0;
		hdr->completion.cntr_paddr_rsh3b = 0;
#endif
	}

	/*
	 * fi_write*() descriptor models
	 */
	{	/*
		 * "direct" model(s) for FI_MR_BASIC
		 */

		/* direct-put model */
		MUHWI_Descriptor_t * desc = &bgq_ep->tx.write.direct.dput_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_GLOBAL;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_COUNTER;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;

		/* specified at injection time */
		desc->Pa_Payload = 0;
		desc->Message_Length = 0;
		desc->Torus_FIFO_Map = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_MSB = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_LSB = 0;

		/*
		 *  "emulation" model(s) for FI_MR_SCALABLE
		 */

		/* memory-fifo model */
		desc = &bgq_ep->tx.write.emulation.mfifo_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->Pa_Payload = 0;
		desc->Message_Length = 0;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_FIFO;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;

		/* specified at injection time */
		desc->Torus_FIFO_Map = -1;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = -1;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id = -1;

		union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		fi_bgq_mu_packet_type_set(hdr, FI_BGQ_MU_PACKET_TYPE_RMA);

		/* remote-get model */
		desc = &bgq_ep->tx.write.emulation.rget_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_GET;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination =
			fi_bgq_uid_get_destination(self.uid.fi);
		desc->PacketHeader.messageUnitHeader.Packet_Types.Remote_Get.Rget_Inj_FIFO_Id =
			bgq_ep->tx.stx->rgetfifo.node_scoped_fifo_id;

		/* specified at injection time */
		desc->Pa_Payload = 0;
		desc->Message_Length = 0;
		desc->Torus_FIFO_Map = -1;

		/* direct-put model */
		desc = &bgq_ep->tx.write.emulation.dput_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_GLOBAL;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_COUNTER;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;

		union fi_bgq_mu_descriptor * fi_desc = (union fi_bgq_mu_descriptor *)desc;
		fi_desc->rma.update_type = FI_BGQ_MU_DESCRIPTOR_UPDATE_BAT_TYPE_DST;

		/* specified at injection time */
		desc->Pa_Payload = 0;
		desc->Message_Length = 0;
		desc->Torus_FIFO_Map = -1;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = -1;

		/* TODO counter update model */

	}

	/*
	 * fi_read*() descriptor models
	 */
	{
		bgq_ep->tx.read.global_one_paddr =
			fi_bgq_node_bat_read(&bgq_domain->fabric->node,
			FI_BGQ_MU_BAT_ID_ONE);

		bgq_ep->tx.read.global_zero_paddr =
			fi_bgq_node_bat_read(&bgq_domain->fabric->node,
			FI_BGQ_MU_BAT_ID_ZERO);

		MUHWI_Descriptor_t * desc = NULL;
		union fi_bgq_mu_packet_hdr * hdr = NULL;

		/* memory-fifo model */
		desc = &bgq_ep->tx.read.emulation.mfifo_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination =
			fi_bgq_uid_get_destination(self.uid.fi);
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_FIFO;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;

		hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		fi_bgq_mu_packet_type_set(hdr, FI_BGQ_MU_PACKET_TYPE_RMA);
		hdr->rma.nbytes = 0;		/* no immediate bytes to 'put' for a read operation */
		hdr->rma.key = (uint64_t)-1;	/* not used when nbytes == 0 */
		hdr->rma.offset = 0;		/* not used when nbytes == 0 */

		/* ==== specified at injection time ==== */
		desc->Torus_FIFO_Map = -1;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = -1;
		desc->Pa_Payload = 0;
		desc->Message_Length = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id = -1;
		/* ==== specified at injection time ==== */

		/* direct-put model */
		desc = &bgq_ep->tx.read.emulation.dput_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination =
			fi_bgq_uid_get_destination(self.uid.fi);
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_GLOBAL;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_COUNTER;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;

		union fi_bgq_mu_descriptor * fi_desc = (union fi_bgq_mu_descriptor *)desc;
		fi_desc->rma.update_type = FI_BGQ_MU_DESCRIPTOR_UPDATE_BAT_TYPE_SRC;

		/* ==== specified at injection time ==== */
		desc->Torus_FIFO_Map = -1;
		desc->Message_Length = 0;
		desc->Pa_Payload = 0;
		fi_desc->rma.key_msb = 0;	/* TODO - change this when key size > 48b */
		fi_desc->rma.key_lsb = -1;
		/* ==== specified at injection time ==== */

		/* "counter" completion direct-put model */
		desc = &bgq_ep->tx.read.cntr_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination =
			fi_bgq_uid_get_destination(self.uid.fi);
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_COUNTER;
		desc->Pa_Payload = bgq_ep->tx.read.global_one_paddr;
		desc->Message_Length = 8;

		/* ==== specified at injection time ==== */
		desc->Torus_FIFO_Map = -1;
		MUSPI_SetRecPayloadBaseAddressInfo(desc, FI_BGQ_MU_BAT_ID_GLOBAL, 0); /* offset will add atomic-ness at runtime */
		/* ==== specified at injection time ==== */

		/* "cq" completion direct-put model */
		desc = &bgq_ep->tx.read.cq_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination =
			fi_bgq_uid_get_destination(self.uid.fi);
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_COUNTER;
		desc->Pa_Payload = bgq_ep->tx.read.global_zero_paddr;
		desc->Message_Length = 8;

		/* ==== specified at injection time ==== */
		desc->Torus_FIFO_Map = -1;
		MUSPI_SetRecPayloadBaseAddressInfo(desc, FI_BGQ_MU_BAT_ID_GLOBAL, 0);
		/* ==== specified at injection time ==== */
	}

	/*
	 * fi_atomic*() descriptor models
	 */
	{
		MUHWI_Descriptor_t * desc = NULL;
		union fi_bgq_mu_packet_hdr * hdr = NULL;

		/*
		 * emulation memory-fifo model
		 */
		desc = &bgq_ep->tx.atomic.emulation.mfifo_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination =
			fi_bgq_uid_get_destination(self.uid.fi);
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_FIFO;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;

		desc->Message_Length = sizeof(MUHWI_Descriptor_t);

		hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		fi_bgq_mu_packet_type_set(hdr, FI_BGQ_MU_PACKET_TYPE_ATOMIC);
		hdr->atomic.origin = fi_bgq_uid_get_destination(self.uid.fi);

		/* ==== specified at injection time ==== */
		desc->Torus_FIFO_Map = -1;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = -1;
		desc->Pa_Payload = 0;

		/* emulation memory-fifo fence model */
		desc = &bgq_ep->tx.atomic.emulation.fence.mfifo_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination =
			fi_bgq_uid_get_destination(self.uid.fi);
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_FIFO;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;

		desc->Message_Length = sizeof(MUHWI_Descriptor_t);

		hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		fi_bgq_mu_packet_type_set(hdr, FI_BGQ_MU_PACKET_TYPE_RMA);
		hdr->rma.nbytes = 0;		/* no immediate bytes to 'put' for a fence operation */
		hdr->rma.key = (uint64_t)-1;	/* not used when nbytes == 0 */
		hdr->rma.offset = 0;		/* not used when nbytes == 0 */
		hdr->rma.ndesc = 1;

		/* ==== specified at injection time ==== */
		desc->Torus_FIFO_Map = -1;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = -1;
		desc->Pa_Payload = 0;

		/* emulation direct-put fi_cntr increment model */
		desc = &bgq_ep->tx.atomic.emulation.fence.cntr_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination =
			fi_bgq_uid_get_destination(self.uid.fi);
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_COUNTER;
		desc->Pa_Payload = bgq_ep->tx.read.global_one_paddr;
		desc->Message_Length = 8;

		/* ==== specified at fi_cntr bind time ==== */
//		MUSPI_SetRecPayloadBaseAddressInfo(desc, write_cntr->std.batid,
//			MUSPI_GetAtomicAddress(0, MUHWI_ATOMIC_OPCODE_STORE_ADD));

		/* ==== specified at injection time ==== */
		desc->Torus_FIFO_Map = -1;
		MUSPI_SetRecPayloadBaseAddressInfo(desc, FI_BGQ_MU_BAT_ID_GLOBAL, 0);

		/* emulation direct-put cq byte counter clear model */
		desc = &bgq_ep->tx.atomic.emulation.fence.cq_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination =
			fi_bgq_uid_get_destination(self.uid.fi);
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_COUNTER;
		desc->Pa_Payload = bgq_ep->tx.read.global_zero_paddr;
		desc->Message_Length = 8;

		/* ==== specified at injection time ==== */
		desc->Torus_FIFO_Map = -1;
		MUSPI_SetRecPayloadBaseAddressInfo(desc, FI_BGQ_MU_BAT_ID_GLOBAL, 0);
	}

	bgq_ep->tx.state = FI_BGQ_EP_INITITALIZED_ENABLED;
	return 0;
}



static int fi_bgq_ep_rx_init(struct fi_bgq_ep *bgq_ep)
{
	assert(FI_SHARED_CONTEXT != bgq_ep->rx.index);

	struct fi_bgq_domain * bgq_domain = bgq_ep->domain;

	BG_CoordinateMapping_t my_coords = bgq_domain->my_coords;

	const uint32_t fifo_map =
		fi_bgq_mu_calculate_fifo_map_single(my_coords, my_coords);

	const MUHWI_Destination_t destination =
		fi_bgq_spi_coordinates_to_destination(my_coords);

	const uint32_t rx =
		fi_bgq_addr_calculate_base_rx(my_coords.t, Kernel_ProcessCount()) + bgq_ep->rx.index;

	bgq_ep->rx.self.fi = fi_bgq_addr_create(destination, fifo_map, rx);

	/* assign the mu reception fifos - all potential
	 * reception fifos were allocated at domain initialization */
	if (NULL == bgq_domain->rx.rfifo[fi_bgq_uid_get_rx(bgq_ep->rx.self.uid.fi)]) {
		assert(0);
		goto err;
	}

	if (NULL != bgq_ep->rx.poll.muspi_recfifo) {
		assert(0);
		goto err;
	}

	bgq_ep->rx.poll.muspi_recfifo = bgq_domain->rx.rfifo[fi_bgq_uid_get_rx(bgq_ep->rx.self.uid.fi)];
#ifdef FI_BGQ_TRACE
	fprintf(stderr,"fi_bgq_ep_rx_init recfifo set to %u created addr:\n",fi_bgq_uid_get_rx(bgq_ep->rx.self.uid.fi));
	FI_BGQ_ADDR_DUMP(&bgq_ep->rx.self.fi);
#endif

	bgq_ep->rx.poll.bat = bgq_domain->bat;

	/* **** acquire the mu lock (node scoped) **** */
	l2atomic_lock_acquire(&bgq_domain->mu.lock);

	/* create an injection fifo for rendezvous and ack messages */

	const int num_fifos_to_allocate = 1;
	if (num_fifos_to_allocate !=
		fi_bgq_spi_injfifo_init(&bgq_ep->rx.poll.injfifo,
			&bgq_ep->rx.poll.injfifo_subgroup,
			num_fifos_to_allocate,
			FI_BGQ_RX_SIZE,
			sizeof(union fi_bgq_mu_packet_payload),
			0 /* is_remote_get */,
			1 /* is_top_down */)) {
		assert(0);
		goto err;
	}

	/*
	 * fi_atomic*() descriptor models
	 */
	{
		MUHWI_Descriptor_t * desc = NULL;

		/*
		 * fi_atomic*() direct-put fetch response model
		 */
		desc = &bgq_ep->rx.poll.atomic_dput_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_GLOBAL;

		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset =
			fi_bgq_node_bat_read(&bgq_domain->fabric->node,
				FI_BGQ_MU_BAT_ID_COUNTER);

		/* specified at injection time */
		desc->Torus_FIFO_Map = -1;
		desc->Pa_Payload = 0;
		desc->Message_Length = 0;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = -1;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id = -1;
		MUSPI_SetRecPayloadBaseAddressInfo(desc, FI_BGQ_MU_BAT_ID_GLOBAL, 0);

		/*
		 * fi_atomic*() direct-put fi_cntr completion model
		 */
		desc = &bgq_ep->rx.poll.atomic_cntr_update_model[0];	/* intranode .. TODO - use an enum */
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_COUNTER;

		desc->Pa_Payload = bgq_ep->tx.read.global_one_paddr;
		desc->Message_Length = 8;
		desc->Torus_FIFO_Map =
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP;

		MUSPI_SetRecPayloadBaseAddressInfo(desc,
			FI_BGQ_MU_BAT_ID_GLOBAL,	/* the bat id will be updated at injection time */
			MUSPI_GetAtomicAddress(0,
				MUHWI_ATOMIC_OPCODE_STORE_ADD));

		/* specified at injection time */
		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = -1;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id = -1;

		/* initialize the "intranode" version of the descriptor model */
		bgq_ep->rx.poll.atomic_cntr_update_model[1] = *desc;		/* internode .. TODO - use an enum */
		bgq_ep->rx.poll.atomic_cntr_update_model[1].Torus_FIFO_Map =
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL0 |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL1;
	}

	/*
	 * initialize the remote-get descriptor models - used for
	 * the "rendezvous" protocol
	 */
	{
		/* initialize the "internode" version of the descriptor model */
		MUHWI_Descriptor_t * desc = &bgq_ep->rx.poll.rzv.rget_model[0];	/* TODO - use an enum */
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->Torus_FIFO_Map =
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_GET;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;

		/* specified at injection time */
		desc->Pa_Payload = 0;
		desc->Message_Length = sizeof(MUHWI_Descriptor_t);
		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = -1;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Remote_Get.Rget_Inj_FIFO_Id = -1;

		/* initialize the "intranode" version of the descriptor model */
		bgq_ep->rx.poll.rzv.rget_model[1] = *desc;		/* TODO - use an enum */
		bgq_ep->rx.poll.rzv.rget_model[1].Torus_FIFO_Map =
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL0 |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL1;
	}

	/*
	 * initialize the direct-put descriptor models - used
	 * to transfer the application data in the "rendezvous"
	 * protocol
	 */
	{
		/* initialize the "internode" version of the descriptor model */
		MUHWI_Descriptor_t * desc = &bgq_ep->rx.poll.rzv.dput_model[0];	/* TODO - use an enum */
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->Torus_FIFO_Map =
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination =
			fi_bgq_uid_get_destination(bgq_ep->rx.self.uid.fi);
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_GLOBAL;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;

		/* specified at injection time */
		desc->Pa_Payload = 0;
		desc->Message_Length = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_MSB = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_LSB = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id = -1;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;

		/* initialize the "intranode" version of the descriptor model */
		bgq_ep->rx.poll.rzv.dput_model[1] = *desc;		/* TODO - use an enum */
		bgq_ep->rx.poll.rzv.dput_model[1].Torus_FIFO_Map =
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL0 |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL1;
	}

	/*
	 * initialize the 'local completion' direct-put
	 * descriptor model - used to zero the byte counter
	 * of the send operation on the origin for the
	 * "rendezvous" protocol
	 *
	 * see also -> fi_bgq_cq::local_completion_model
	 */
	{
		MUHWI_Descriptor_t * desc = &bgq_ep->rx.poll.rzv.dput_completion_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only = MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt = MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->Message_Length = sizeof(uint64_t);
		desc->Torus_FIFO_Map =
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL0 |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL1;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_COUNTER;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_COUNTER;

		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = 0;

		/* specified at injection time */
		desc->Pa_Payload = 0;
	}

	{
		MUHWI_Descriptor_t * desc = &bgq_ep->rx.poll.rzv.multi_recv_ack_model;
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->Message_Length = 0;
		desc->Pa_Payload = 0;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_FIFO;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination =
			fi_bgq_uid_get_destination(bgq_ep->rx.self.uid.fi);

		desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id =
			fi_bgq_uid_get_rx(bgq_ep->rx.self.uid.fi);

		union fi_bgq_mu_packet_hdr * hdr = (union fi_bgq_mu_packet_hdr *) &desc->PacketHeader;
		fi_bgq_mu_packet_type_set(hdr, FI_BGQ_MU_PACKET_TYPE_ACK);

		/* specified at injection time */
		desc->Torus_FIFO_Map = 0;
		hdr->ack.context = 0;
	}

	/*
	 * initialize the direct-put descriptor models used to zero an arbitrary
	 * 8 byte variable - used to implement FI_DELIVERY_COMPLETE
	 */
	{
		/* initialize the "internode" version of the descriptor model */
		MUHWI_Descriptor_t * desc = &bgq_ep->rx.poll.ack_model[0];	/* TODO - use an enum */
		MUSPI_DescriptorZeroOut(desc);

		desc->Half_Word0.Prefetch_Only =
			MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
		desc->Half_Word1.Interrupt =
			MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
		desc->Torus_FIFO_Map =
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP;
		desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type =
			MUHWI_PT2PT_DATA_PACKET_TYPE;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 =
			MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 =
			MUHWI_PACKET_TYPE_PUT;
		desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing =
			MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
		desc->Pa_Payload = bgq_domain->zero.paddr;
		desc->Message_Length = sizeof(uint64_t);
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id =
			FI_BGQ_MU_BAT_ID_COUNTER;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;

		/* specified at injection time - not used for local transfers */
		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = -1;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_MSB = 0;
		desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_LSB = 0;
		desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Destination = -1;

		/* initialize the "intranode" version of the descriptor model */
		bgq_ep->rx.poll.ack_model[1] = *desc;		/* TODO - use an enum */
		bgq_ep->rx.poll.ack_model[1].Torus_FIFO_Map =
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL0 |
			MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL1;
	}


	/* allocate the l2atomic fifos for match information and control information */
	{
		struct l2atomic_fifo_data * memptr = NULL;
		size_t bytes = (sizeof(struct l2atomic_fifo_data) + sizeof(uint64_t) * bgq_ep->recv_cq->size) * 2;
		bytes += sizeof(struct l2atomic_fifo_data) + sizeof(uint64_t) * FI_BGQ_L2FIFO_CTL_SIZE;

		if (posix_memalign((void **)&memptr, 32, bytes)) {
			errno = FI_ENOMEM;
			goto err;
		}
		memset((void*)memptr, 0, bytes);
		bgq_ep->rx.l2atomic_memptr = (void*)memptr;

		l2atomic_fifo_initialize(&bgq_ep->rx.poll.rfifo[IS_TAG].match,
			&bgq_ep->rx.post.match[IS_TAG],
			memptr, bgq_ep->recv_cq->size);

		memptr = (struct l2atomic_fifo_data *)((uintptr_t)memptr + sizeof(struct l2atomic_fifo_data) + sizeof(uint64_t) * bgq_ep->recv_cq->size);
		l2atomic_fifo_initialize(&bgq_ep->rx.poll.rfifo[IS_MSG].match,
			&bgq_ep->rx.post.match[IS_MSG],
			memptr, bgq_ep->recv_cq->size);

		memptr = (struct l2atomic_fifo_data *)((uintptr_t)memptr + sizeof(struct l2atomic_fifo_data) + sizeof(uint64_t) * bgq_ep->recv_cq->size);
		l2atomic_fifo_initialize(&bgq_ep->rx.poll.control,
			&bgq_ep->rx.post.control,
			memptr, FI_BGQ_L2FIFO_CTL_SIZE);
	}

	/* **** release the mu lock (node scoped) **** */
	l2atomic_lock_release(&bgq_domain->mu.lock);

	bgq_ep->rx.state = FI_BGQ_EP_INITITALIZED_ENABLED;
	return 0;
err:
	return 1;
}

static int fi_bgq_open_command_queues(struct fi_bgq_ep *bgq_ep)
{
	struct fi_bgq_domain *bgq_domain;

	if (!bgq_ep) {
		errno = FI_EINVAL;
		return -errno;
	}

	bgq_domain = bgq_ep->domain;

#ifdef FI_BGQ_TRACE
	fprintf(stderr,"fi_bgq_open_command_queues ofi_send_allowed(bgq_ep->tx.caps) is %016lx ofi_rma_initiate_allowed(bgq_ep->tx.caps) is %016lx ofi_recv_allowed(bgq_ep->rx.caps) is %016lx ofi_rma_target_allowed(bgq_ep->rx.caps) is %016lx\n",ofi_send_allowed(bgq_ep->tx.caps),ofi_rma_initiate_allowed(bgq_ep->tx.caps),ofi_recv_allowed(bgq_ep->rx.caps),ofi_rma_target_allowed(bgq_ep->rx.caps));
	fflush(stderr);
#endif

	if (ofi_send_allowed(bgq_ep->tx.caps) || ofi_rma_initiate_allowed(bgq_ep->tx.caps)) {

		/* verify there is a completion queue associated with the tx context */
		if (!bgq_ep->send_cq) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"No completion queue bound to send context");
			goto err;
		}

		/* verify there is a shared tx context associated with the endpoint - if so configured */
		if (FI_SHARED_CONTEXT == bgq_ep->tx.index && !bgq_ep->tx.stx) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"No shared tx context bound to endpoint as configured");
			goto err;
		}


		if (fi_bgq_ep_tx_init(bgq_ep, bgq_domain)) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"Too many tx contexts");
			goto err;
		}
	}

	if (ofi_recv_allowed(bgq_ep->rx.caps) || ofi_rma_target_allowed(bgq_ep->rx.caps)) {

		/* verify there is a completion queue associated with the rx context */
		if (!bgq_ep->recv_cq) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"No completion queue bound to receive context");
			goto err;
		}

		if (FI_SHARED_CONTEXT == bgq_ep->rx.index) {
			/* verify there is a shared rx context associated with the endpoint */
			if (!bgq_ep->rx.srx) {
				FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
					"No shared rx context bound to endpoint as configured");
				goto err;
			}

		} else if (bgq_ep->rx.index >= bgq_domain->rx.max) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"Invalid rx context index (exceeds maximum)");
			goto err;

		} else if (bgq_ep->rx.index < 0) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"Invalid rx context index (exceeds minimum)");
			goto err;

		} else if (NULL != bgq_domain->rx.ctx[bgq_ep->rx.index]) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"Invalid rx context index (existing allocation)");
			goto err;
		}

		if (0 != fi_bgq_ep_rx_init(bgq_ep)) {
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"Error during rx context initialization");
			goto err;
		}

		bgq_domain->rx.ctx[bgq_ep->rx.index] = bgq_ep;
	}

	return 0;
err:
	return -1;
}

static int fi_bgq_enable_ep(struct fid_ep *ep)
{
	int ret;
	struct fi_bgq_ep *bgq_ep = container_of(ep, struct fi_bgq_ep, ep_fid);
	ret = fi_bgq_check_ep(bgq_ep);
	if (ret) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"ep enable failed\n");
		return -errno;
	}

	ret = fi_bgq_open_command_queues(bgq_ep);
	if (ret) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"failed to assign command queues\n");
		return -errno;
	}

	ret = fi_bgq_enable_msg_ops(bgq_ep);
	if (ret) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
			"failed to enable msg ops\n");
		return -errno;
	}

	ret = fi_bgq_enable_rma_ops(bgq_ep);
	if (ret) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
			"failed to enable rma ops\n");
		return -errno;
	}

	ret = fi_bgq_enable_atomic_ops(bgq_ep);
	if (ret) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
			"failed to enable rma ops\n");
		return -errno;
	}

	ret = fi_bgq_enable_tagged_ops(bgq_ep);
	if (ret) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
			"failed to enable rma ops\n");
		return -errno;
	}

	bgq_ep->state = FI_BGQ_EP_INITITALIZED_ENABLED;

	/* create an async progress thread for the receive context for FI_PROGRESS_AUTO mode*/
	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_AUTO)
	if (ofi_recv_allowed(bgq_ep->rx.caps)) {

		if (bgq_ep->domain->rx.count == 1) {

			/*
			 * This is the first endpoint to be assigned to a
			 * progress thread. The first progress thread is
			 * started at domain initialization time
			 */

			ret = fi_bgq_progress_ep_enable(&bgq_ep->domain->progress.thread[0], bgq_ep);
			if (ret) {
				FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
					"failed to enable async progress on endpoint\n");
				return -errno;
			}

		} else if (bgq_ep->domain->progress.num_threads_active < bgq_ep->domain->progress.max_threads) {

			const unsigned n = bgq_ep->domain->progress.num_threads_active;
			ret = fi_bgq_progress_enable(bgq_ep->domain, n);
			if (ret) {
				FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
					"failed to enable progress thread\n");
				return -errno;
			}

			ret = fi_bgq_progress_ep_enable(&bgq_ep->domain->progress.thread[n], bgq_ep);
			if (ret) {
				FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
					"failed to enable async progress on endpoint\n");
				return -errno;
			}

		} else {

			/*
			 * Assign endpoint to progress thread in a round-robin fashion.
			 *
			 * TODO - better assignment algorithm
			 */

			const unsigned t = bgq_ep->domain->rx.count % bgq_ep->domain->progress.max_threads;
			ret = fi_bgq_progress_ep_enable(&bgq_ep->domain->progress.thread[t], bgq_ep);
			if (ret) {
				FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
					"failed to enable async progress on endpoint\n");
				return -errno;
			}

		}
	}

	return 0;
}

static int fi_bgq_control_ep(fid_t fid, int command, void *arg)
{
	struct fid_ep *ep;
	ep = container_of(fid, struct fid_ep, fid);

	switch (command) {
	case FI_ENABLE:
		return fi_bgq_enable_ep(ep);
	default:
		return -FI_ENOSYS;
	}

	return 0;
}

static int fi_bgq_getopt_ep(fid_t fid, int level, int optname,
			void *optval, size_t *optlen)
{
	struct fi_bgq_ep *bgq_ep = container_of(fid, struct fi_bgq_ep, ep_fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		*(size_t *)optval = bgq_ep->rx.min_multi_recv;
		*optlen = sizeof(size_t);
		break;
	case FI_OPT_CM_DATA_SIZE:
		*(size_t *)optval = 0;
		*optlen = sizeof(size_t);
		break;
	default:
		return -FI_ENOPROTOOPT;
	}

	return 0;
}

static int fi_bgq_setopt_ep(fid_t fid, int level, int optname,
			const void *optval, size_t optlen)
{
	struct fi_bgq_ep *bgq_ep = container_of(fid, struct fi_bgq_ep, ep_fid);

	if (level != FI_OPT_ENDPOINT)
		return -FI_ENOPROTOOPT;

	switch (optname) {
	case FI_OPT_MIN_MULTI_RECV:
		bgq_ep->rx.min_multi_recv = *(size_t *)optval;
		bgq_ep->rx.poll.min_multi_recv = bgq_ep->rx.min_multi_recv;
		break;

	default:
		return -FI_ENOPROTOOPT;
	}

	return 0;
}

static
ssize_t fi_bgq_cancel(fid_t fid, void *context)
{
	struct fi_bgq_ep *bgq_ep = container_of(fid, struct fi_bgq_ep, ep_fid);

	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {	/* TODO - FI_PROGRESS_AUTO + 64 ppn */
		const enum fi_threading threading = bgq_ep->domain->threading;
		const int lock_required =
			(threading == FI_THREAD_FID) ||
			(threading == FI_THREAD_UNSPEC) ||
			(threading == FI_THREAD_SAFE);

		int ret;
		ret = fi_bgq_lock_if_required(&bgq_ep->lock, lock_required);
		if (ret) return ret;

		fi_bgq_ep_progress_manual_cancel(bgq_ep, (const uint64_t)context);

		ret = fi_bgq_unlock_if_required(&bgq_ep->lock, lock_required);
		if (ret) return ret;

	} else {

		/* context must be 8 byte aligned */
		assert(((uint64_t)context & 0x07ull) == 0);
		uint64_t value = (uint64_t)context >> 3;

		struct l2atomic_fifo_producer * fifo = &bgq_ep->rx.post.control;
		while (0 != l2atomic_fifo_produce(fifo, value));
	}

	return 0;
}

static struct fi_ops fi_bgq_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_bgq_close_ep,
	.bind		= fi_bgq_bind_ep,
	.control	= fi_bgq_control_ep,
	.ops_open	= fi_no_ops_open
};

static struct fi_ops_ep fi_bgq_ep_ops = {
	.size		= sizeof(struct fi_ops_ep),
	.cancel		= fi_bgq_cancel,
	.getopt		= fi_bgq_getopt_ep,
	.setopt		= fi_bgq_setopt_ep,
	.tx_ctx		= fi_no_tx_ctx,
	.rx_ctx		= fi_no_rx_ctx,
	.rx_size_left   = fi_no_rx_size_left,
	.tx_size_left   = fi_no_tx_size_left
};

int fi_bgq_alloc_default_rx_attr(struct fi_rx_attr **rx_attr)
{
	struct fi_rx_attr *attr;

	attr = calloc(1, sizeof(*attr));
	if (!attr)
		goto err;

	attr->caps 	= FI_MSG | FI_RMA | FI_TAGGED | FI_ATOMIC | FI_RECV | FI_REMOTE_READ | FI_REMOTE_WRITE | FI_NAMED_RX_CTX | FI_DIRECTED_RECV | FI_MULTI_RECV | FI_SOURCE;
	attr->mode 	= FI_ASYNC_IOV;
	attr->op_flags 	= 0;
	attr->msg_order = FI_BGQ_DEFAULT_MSG_ORDER;
	attr->comp_order = FI_ORDER_NONE;
	attr->total_buffered_recv = FI_BGQ_TOTAL_BUFFERED_RECV;
	attr->size 	= FI_BGQ_RX_SIZE;
	attr->iov_limit = SIZE_MAX;

	*rx_attr = attr;

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_check_rx_attr(const struct fi_rx_attr *attr)
{
	/* TODO: more error checking of rx_attr */
#ifdef TODO
	if (attr->total_buffered_recv > FI_BGQ_TOTAL_BUFFERED_RECV) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"unavailable [bad total_buffered_recv (%lu)]",
				attr->total_buffered_recv);
		goto err;
	}
#endif
	if (attr->comp_order && attr->comp_order == FI_ORDER_STRICT) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"unavailable [bad rx comp_order (%lx)] ",
				attr->comp_order);
		goto err;
       }


	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_alloc_default_tx_attr(struct fi_tx_attr **tx_attr)
{
	struct fi_tx_attr *attr;

	attr = calloc(1, sizeof(*attr));
	if (!attr)
		goto err;

	attr->caps	= FI_MSG | FI_RMA | FI_TAGGED | FI_ATOMIC | FI_SEND | FI_READ | FI_WRITE;
	attr->mode	= FI_CONTEXT | FI_ASYNC_IOV;
	attr->op_flags	= FI_TRANSMIT_COMPLETE;
	attr->msg_order	= FI_BGQ_DEFAULT_MSG_ORDER;
	attr->comp_order = FI_ORDER_NONE;
	attr->inject_size = FI_BGQ_INJECT_SIZE;
	attr->size	= FI_BGQ_TX_SIZE;
	attr->iov_limit = SIZE_MAX;
	attr->rma_iov_limit = 1;

	*tx_attr = attr;

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_check_tx_attr(const struct fi_tx_attr *attr)
{
	if (attr->inject_size > FI_BGQ_INJECT_SIZE) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"unavailable [bad inject_size (%lu)]",
				attr->inject_size);
		goto err;
	}
	/* TODO: more error checking of tx_attr */

	if (attr->comp_order && attr->comp_order == FI_ORDER_STRICT) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"unavailable [bad tx comp_order (%lx)] ",
				attr->comp_order);
		goto err;
       }

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_alloc_default_ep_attr(struct fi_ep_attr **ep_attr)
{
	struct fi_ep_attr *attr;

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

	attr->type		= FI_EP_RDM;
	attr->protocol		= FI_BGQ_PROTOCOL;
	attr->protocol_version	= FI_BGQ_PROTOCOL_VERSION;
	attr->max_msg_size	= FI_BGQ_MAX_MSG_SIZE;
	attr->msg_prefix_size 	= 0;
	attr->max_order_raw_size= FI_BGQ_MAX_ORDER_RAW_SIZE;
	attr->max_order_war_size= FI_BGQ_MAX_ORDER_WAR_SIZE;
	attr->max_order_waw_size= FI_BGQ_MAX_ORDER_WAW_SIZE;
	attr->mem_tag_format 	= FI_BGQ_MEM_TAG_FORMAT;
	attr->tx_ctx_cnt	= tx_ctx_cnt;
	attr->rx_ctx_cnt	= rx_ctx_cnt;

	*ep_attr = attr;

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_check_ep_attr(const struct fi_ep_attr *attr)
{
	switch(attr->protocol) {
		case FI_PROTO_UNSPEC:
		case FI_BGQ_PROTOCOL:
			break;
		default:
			FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
					"unavailable [bad protocol (%u)]",
					attr->protocol);
			goto err;
	}
	if (attr->max_msg_size > FI_BGQ_MAX_MSG_SIZE) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"unavailable [bad max_msg_size (%lu)]",
				attr->max_msg_size);
		goto err;
	}
	if (attr->max_order_raw_size > FI_BGQ_MAX_ORDER_RAW_SIZE) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"unavailable [bad max_order_raw_size (%lu)",
				attr->max_order_raw_size);
		goto err;
	}
	if (attr->max_order_war_size > FI_BGQ_MAX_ORDER_WAR_SIZE) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"unavailable [bad max_order_war_size (%lu)",
				attr->max_order_war_size);
		goto err;
	}
	if (attr->max_order_waw_size > FI_BGQ_MAX_ORDER_WAW_SIZE) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"unavailable [bad max_order_waw_size (%lu)",
				attr->max_order_waw_size);
		goto err;
	}
	if (attr->mem_tag_format &&
			attr->mem_tag_format & ~FI_BGQ_MEM_TAG_FORMAT) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"unavailable [bad mem_tag_format (%lx)",
				attr->mem_tag_format);
		goto err;
	}
	/* TODO: what msg orders do we not support? */

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_bgq_endpoint_rx_tx (struct fid_domain *dom, struct fi_info *info,
		struct fid_ep **ep, void *context, const ssize_t rx_index, const ssize_t tx_index)
{
#ifdef FI_BGQ_TRACE
	fprintf(stderr,"fi_bgq_endpoint_rx_tx called with rx_index %ld tx_index %ld\n",rx_index,tx_index);
	fflush(stderr);
#endif

	int ret;
	struct fi_bgq_ep *bgq_ep = NULL;
	struct fi_bgq_domain *bgq_domain = NULL;

	if (!info || !dom) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"no info/domain supplied\n");
		errno = FI_EINVAL;
		goto err;
	}

	ret = fi_bgq_fid_check(&dom->fid, FI_CLASS_DOMAIN, "domain");
	if (ret) return ret;

	ret = fi_bgq_check_info(info);
	if (ret)
		return ret;

	void *mem = NULL;
	mem = malloc(sizeof(struct fi_bgq_ep) + FI_BGQ_CACHE_LINE_SIZE);
	if (!mem) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_EP_DATA,
				"no memory for endpoint");
		errno = FI_ENOMEM;
		goto err;
	}
	bgq_ep = (struct fi_bgq_ep *)(((uintptr_t)mem + FI_BGQ_CACHE_LINE_SIZE) & ~(FI_BGQ_CACHE_LINE_SIZE - 1));
	memset(bgq_ep, 0, sizeof(struct fi_bgq_ep));
	bgq_ep->mem = mem;

	bgq_ep->ep_fid.fid.fclass  = FI_CLASS_EP;
	bgq_ep->ep_fid.fid.context = context;
	bgq_ep->ep_fid.fid.ops     = &fi_bgq_fi_ops;
	bgq_ep->ep_fid.ops 	   = &fi_bgq_ep_ops;

	ret = fi_bgq_init_cm_ops((struct fid_ep *)&(bgq_ep->ep_fid), info);
	if (ret)
		goto err;

	ret = fi_bgq_init_msg_ops(bgq_ep, info);
	if (ret)
		goto err;

	ret = fi_bgq_init_rma_ops(bgq_ep, info);
	if (ret)
		goto err;

	ret = fi_bgq_init_tagged_ops(bgq_ep, info);
	if (ret)
		goto err;

	ret = fi_bgq_init_atomic_ops(bgq_ep, info);
	if (ret)
		goto err;

	bgq_ep->rx.index = rx_index;
	bgq_ep->tx.index = tx_index;

	if (rx_index >= 0) {
		bgq_ep->rx.caps = info->rx_attr ? info->rx_attr->caps : info->caps;
		bgq_ep->rx.caps |= FI_RECV;
		bgq_ep->rx.mode = info->rx_attr ? info->rx_attr->mode : 0;
		bgq_ep->rx.op_flags = info->rx_attr ? info->rx_attr->op_flags : 0;
		bgq_ep->rx.total_buffered_recv = info->rx_attr ?
			info->rx_attr->total_buffered_recv : 0;
	}
	else {
		bgq_ep->rx.caps = 0;
		bgq_ep->rx.mode = 0;
		bgq_ep->rx.op_flags = 0;
		bgq_ep->rx.total_buffered_recv = 0;
	}

	if (tx_index >= 0) {
		bgq_ep->tx.caps = info->tx_attr ? info->tx_attr->caps : info->caps;
		bgq_ep->tx.mode = info->tx_attr ? info->tx_attr->mode : 0;
		bgq_ep->tx.op_flags = info->tx_attr ? info->tx_attr->op_flags : 0;
	}
	else {
		bgq_ep->tx.caps = 0;
		bgq_ep->tx.mode = 0;
		bgq_ep->tx.op_flags = 0;
	}


	bgq_domain = container_of(dom, struct fi_bgq_domain, domain_fid);
	bgq_ep->domain = bgq_domain;
	fi_bgq_ref_inc(&bgq_domain->ref_cnt, "domain");

	*ep = &bgq_ep->ep_fid;

	return 0;
err:
	fi_bgq_finalize_cm_ops(bgq_ep);
	fi_bgq_finalize_msg_ops(bgq_ep);
	fi_bgq_finalize_rma_ops(bgq_ep);
	fi_bgq_finalize_tagged_ops(bgq_ep);
	fi_bgq_finalize_atomic_ops(bgq_ep);
	if (bgq_domain)
		fi_bgq_ref_dec(&bgq_domain->ref_cnt, "domain");
	if (bgq_ep)
		free(bgq_ep->mem);
	return -errno;
}

int fi_bgq_endpoint (struct fid_domain *dom, struct fi_info *info,
		struct fid_ep **ep, void *context)
{
	ssize_t rx_index = 0;
	ssize_t tx_index = 0;

	if (info && info->ep_attr) {

		if (FI_SHARED_CONTEXT == info->ep_attr->tx_ctx_cnt)
			tx_index = FI_SHARED_CONTEXT;

		if (FI_SHARED_CONTEXT == info->ep_attr->rx_ctx_cnt)
			rx_index = FI_SHARED_CONTEXT;
	}

	return fi_bgq_endpoint_rx_tx(dom, info, ep, context, rx_index, tx_index);
}

/* ************************************************************************* */
/* These functions are only be used in FI_PROGRESS_MANUAL mode               */
/* ************************************************************************* */

void fi_bgq_ep_progress_manual_cancel (struct fi_bgq_ep * bgq_ep, const uint64_t cancel_context) {

	if (bgq_ep->rx.caps & FI_MSG) {
		cancel_match_queue(bgq_ep, 1, cancel_context);
	}

	if (bgq_ep->rx.caps & FI_TAGGED) {
		cancel_match_queue(bgq_ep, 0, cancel_context);
	}
}


int fi_bgq_ep_progress_manual_recv (struct fi_bgq_ep *bgq_ep,
		const uint64_t is_msg,
		union fi_bgq_context * context,
		const uint64_t rx_op_flags,
		const uint64_t is_context_ext) {

	assert(bgq_ep->rx.poll.injfifo.muspi_injfifo);
	return process_mfifo_context(bgq_ep, is_msg, 0, context, rx_op_flags, is_context_ext, 1);
}


int fi_bgq_ep_progress_manual_recv_fast (struct fi_bgq_ep *bgq_ep,
		const uint64_t is_msg,
		union fi_bgq_context * context) {

	return process_mfifo_context(bgq_ep, is_msg, 0, context, 0, 0, 1);
}


int fi_bgq_ep_progress_manual (struct fi_bgq_ep *bgq_ep) {


	poll_rfifo(bgq_ep, 1);

	return 0;
}

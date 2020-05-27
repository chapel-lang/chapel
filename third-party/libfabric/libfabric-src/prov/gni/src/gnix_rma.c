/*
 * Copyright (c) 2015-2019 Cray Inc. All rights reserved.
 * Copyright (c) 2015-2018 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2019 Triad National Security, LLC. All rights reserved.
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

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gnix.h"
#include "gnix_nic.h"
#include "gnix_vc.h"
#include "gnix_ep.h"
#include "gnix_mr.h"
#include "gnix_cm_nic.h"
#include "gnix_mbox_allocator.h"
#include "gnix_cntr.h"

#include <gni_pub.h>

/* Threshold to switch from indirect transfer to chained transfer to move
 * unaligned read data. */
#define GNIX_RMA_UREAD_CHAINED_THRESH		60
#define slist_entry_foreach(head, item)\
	for (item = head; item; item = item->next)

static int __gnix_rma_send_err(struct gnix_fid_ep *ep,
			       struct gnix_fab_req *req,
			       int error)
{
	struct gnix_fid_cntr *cntr = NULL;
	int rc = FI_SUCCESS;
	uint64_t flags = req->flags & GNIX_RMA_COMPLETION_FLAGS;

	if (ep->send_cq) {
		rc = _gnix_cq_add_error(ep->send_cq, req->user_context,
					flags, 0, 0, 0, 0, 0, error,
					gnixu_to_fi_errno(GNI_RC_TRANSACTION_ERROR),
					NULL, 0);
		if (rc) {
			GNIX_WARN(FI_LOG_EP_DATA,
				  "_gnix_cq_add_error() failed: %d\n", rc);
		}
	}

	if ((req->type == GNIX_FAB_RQ_RDMA_WRITE) &&
	    ep->write_cntr)
		cntr = ep->write_cntr;

	if ((req->type == GNIX_FAB_RQ_RDMA_READ) &&
	    ep->read_cntr)
		cntr = ep->read_cntr;

	if (cntr) {
		rc = _gnix_cntr_inc_err(cntr);
		if (rc)
			GNIX_WARN(FI_LOG_EP_DATA,
				  "_gnix_cntr_inc_err() failed: %d\n", rc);
	}

	return rc;
}

static int __gnix_rma_send_completion(struct gnix_fid_ep *ep,
				      struct gnix_fab_req *req)
{
	struct gnix_fid_cntr *cntr = NULL;
	int rc;
	uint64_t flags = req->flags & GNIX_RMA_COMPLETION_FLAGS;

	if ((req->flags & FI_COMPLETION) && ep->send_cq) {
		rc = _gnix_cq_add_event(ep->send_cq, ep, req->user_context,
					flags, 0, 0, 0, 0, FI_ADDR_NOTAVAIL);
		if (rc) {
			GNIX_WARN(FI_LOG_EP_DATA,
				  "_gnix_cq_add_event() failed: %d\n", rc);
		}
	}

	if ((req->type == GNIX_FAB_RQ_RDMA_WRITE) &&
	    ep->write_cntr) {
		cntr = ep->write_cntr;
	}

	if ((req->type == GNIX_FAB_RQ_RDMA_READ) &&
	    ep->read_cntr) {
		cntr = ep->read_cntr;
	}

	if (cntr) {
		rc = _gnix_cntr_inc(cntr);
		if (rc)
			GNIX_WARN(FI_LOG_EP_DATA,
				  "_gnix_cntr_inc() failed: %d\n", rc);
	}

	return FI_SUCCESS;
}

static inline void __gnix_rma_copy_indirect_get_data(struct gnix_fab_req *req)
{
	int head_off = req->rma.rem_addr & GNI_READ_ALIGN_MASK;
	memcpy((void *)req->rma.loc_addr,
	       (void *) ((uint8_t *) req->int_tx_buf + head_off),
	       req->rma.len);
}

static void __gnix_rma_copy_chained_get_data(struct gnix_fab_req *req)
{
	int head_off, head_len, tail_len;
	void *addr;

	head_off = req->rma.rem_addr & GNI_READ_ALIGN_MASK;
	head_len = GNI_READ_ALIGN - head_off;
	tail_len = (req->rma.rem_addr + req->rma.len) & GNI_READ_ALIGN_MASK;

	if (head_off) {
		GNIX_INFO(FI_LOG_EP_DATA, "writing %d bytes to %p\n",
			  head_len, req->rma.loc_addr);
		memcpy((void *)req->rma.loc_addr,
		       (void *) ((uint8_t *) req->int_tx_buf + head_off),
		       head_len);
	}

	if (tail_len) {
		addr = (void *) ((uint8_t *) req->rma.loc_addr +
				 req->rma.len -
				 tail_len);

		GNIX_INFO(FI_LOG_EP_DATA, "writing %d bytes to %p\n",
			  tail_len, addr);
		memcpy((void *)addr,
			   (void *) ((uint8_t *) req->int_tx_buf + GNI_READ_ALIGN),
			   tail_len);
	}
}

static void __gnix_rma_more_fr_complete(struct gnix_fab_req *req)
{
	int rc;

	if (req->flags & FI_LOCAL_MR) {
		GNIX_INFO(FI_LOG_EP_DATA, "freeing auto-reg MR: %p\n",
			  req->rma.loc_md);
		rc = fi_close(&req->rma.loc_md->mr_fid.fid);
		if (rc != FI_SUCCESS)
			GNIX_FATAL(FI_LOG_DOMAIN,
				"failed to close auto-registration, "
				"rc=%d\n", rc);

		req->flags &= ~FI_LOCAL_MR;
	}

	/* Schedule VC TX queue in case the VC is 'fenced'. */
	_gnix_vc_tx_schedule(req->vc);

	_gnix_fr_free(req->vc->ep, req);
}

static void __gnix_rma_fr_complete(struct gnix_fab_req *req)
{
	int rc;

	if (req->flags & FI_LOCAL_MR) {
		GNIX_INFO(FI_LOG_EP_DATA, "freeing auto-reg MR: %p\n",
			  req->rma.loc_md);
		rc = fi_close(&req->rma.loc_md->mr_fid.fid);
		if (rc != FI_SUCCESS)
			GNIX_FATAL(FI_LOG_DOMAIN,
				"failed to close auto-registration, "
				"rc=%d\n", rc);

		req->flags &= ~FI_LOCAL_MR;
	}

	ofi_atomic_dec32(&req->vc->outstanding_tx_reqs);

	/* Schedule VC TX queue in case the VC is 'fenced'. */
	_gnix_vc_tx_schedule(req->vc);

	_gnix_fr_free(req->vc->ep, req);
}

static int __gnix_rma_post_err_no_retrans(struct gnix_fab_req *req, int error)
{
	int rc;

	rc = __gnix_rma_send_err(req->vc->ep, req, error);
	if (rc != FI_SUCCESS)
		GNIX_WARN(FI_LOG_EP_DATA,
			  "__gnix_rma_send_err() failed: %d\n",
			  rc);

	__gnix_rma_fr_complete(req);
	return FI_SUCCESS;
}

static int __gnix_rma_post_err(struct gnix_fab_req *req, int error)
{
	if (GNIX_EP_RDM(req->gnix_ep->type) &&
		_gnix_req_replayable(req)) {
		req->tx_failures++;
		GNIX_INFO(FI_LOG_EP_DATA,
			  "Requeueing failed request: %p\n", req);
		return _gnix_vc_requeue_work_req(req);
	}

	GNIX_WARN(FI_LOG_EP_DATA, "Failed %u transmits: %p error: %d\n",
		  req->tx_failures, req, error);

	__gnix_rma_post_err_no_retrans(req, error);

	return FI_SUCCESS;
}

/*
 * completer for a GNI_PostCqWrite, for now ignore error status
 */

static int __gnix_rma_post_irq_complete(void *arg, gni_return_t tx_status)
{
	struct gnix_tx_descriptor *txd = (struct gnix_tx_descriptor *)arg;
	struct gnix_vc *vc;

	vc = (struct gnix_vc *)txd->req;
	_gnix_nic_tx_free(vc->ep->nic, txd);

	return FI_SUCCESS;
}

/* SMSG callback for RMA data control message. */
int __smsg_rma_data(void *data, void *msg)
{
	int ret = FI_SUCCESS;
	struct gnix_vc *vc = (struct gnix_vc *)data;
	struct gnix_smsg_rma_data_hdr *hdr =
			(struct gnix_smsg_rma_data_hdr *)msg;
	struct gnix_fid_ep *ep = vc->ep;
	gni_return_t status;

	if (GNIX_ALLOW_FI_REMOTE_CQ_DATA(hdr->flags, ep->caps) && ep->recv_cq) {
		ret = _gnix_cq_add_event(ep->recv_cq, ep, NULL, hdr->user_flags,
					 0, 0, hdr->user_data, 0,
					 FI_ADDR_NOTAVAIL);
		if (ret != FI_SUCCESS)  {
			GNIX_WARN(FI_LOG_EP_DATA,
				  "_gnix_cq_add_event returned %d\n",
				  ret);
		}
	}

	if (hdr->flags & FI_REMOTE_WRITE && ep->rwrite_cntr) {
		ret = _gnix_cntr_inc(ep->rwrite_cntr);
		if (ret != FI_SUCCESS)
			GNIX_WARN(FI_LOG_EP_DATA,
				  "_gnix_cntr_inc() failed: %d\n",
				  ret);
	}

	if (hdr->flags & FI_REMOTE_READ && ep->rread_cntr) {
		ret = _gnix_cntr_inc(ep->rread_cntr);
		if (ret != FI_SUCCESS)
			GNIX_WARN(FI_LOG_EP_DATA,
				  "_gnix_cntr_inc() failed: %d\n",
				  ret);
	}

	status = GNI_SmsgRelease(vc->gni_ep);
	if (OFI_UNLIKELY(status != GNI_RC_SUCCESS)) {
		GNIX_WARN(FI_LOG_EP_DATA,
			  "GNI_SmsgRelease returned %s\n",
			  gni_err_str[status]);
		ret = gnixu_to_fi_errno(status);
	}

	return ret;
}

/* __gnix_rma_txd_data_complete() should match __gnix_rma_txd_complete() except
 * for checking whether to send immediate data. */
static int __gnix_rma_txd_data_complete(void *arg, gni_return_t tx_status)
{
	struct gnix_tx_descriptor *txd = (struct gnix_tx_descriptor *)arg;
	struct gnix_fab_req *req = txd->req;
	int rc;

	_gnix_nic_tx_free(req->gnix_ep->nic, txd);

	if (tx_status != GNI_RC_SUCCESS)
		return __gnix_rma_post_err(req, FI_ECANCELED);

	/* Successful data delivery.  Generate local completion. */
	rc = __gnix_rma_send_completion(req->gnix_ep, req);
	if (rc != FI_SUCCESS)
		GNIX_WARN(FI_LOG_EP_DATA,
			  "__gnix_rma_send_completion() failed: %d\n",
			  rc);

	__gnix_rma_fr_complete(req);

	return FI_SUCCESS;
}

static int __gnix_rma_send_data_req(void *arg)
{
	struct gnix_fab_req *req = (struct gnix_fab_req *)arg;
	struct gnix_fid_ep *ep = req->gnix_ep;
	struct gnix_nic *nic = ep->nic;
	struct gnix_tx_descriptor *txd;
	gni_return_t status;
	int rc;
	int inject_err = _gnix_req_inject_err(req);

	rc = _gnix_nic_tx_alloc(nic, &txd);
	if (rc) {
		GNIX_INFO(FI_LOG_EP_DATA,
				"_gnix_nic_tx_alloc() failed: %d\n",
				rc);
		return -FI_ENOSPC;
	}

	txd->req = req;
	txd->completer_fn = __gnix_rma_txd_data_complete;
	txd->rma_data_hdr.flags = 0;

	if (GNIX_ALLOW_FI_REMOTE_CQ_DATA(req->flags, ep->caps)) {
		txd->rma_data_hdr.flags |= FI_REMOTE_CQ_DATA;
		txd->rma_data_hdr.user_flags = FI_RMA | FI_REMOTE_CQ_DATA;
		if (req->type == GNIX_FAB_RQ_RDMA_WRITE) {
			txd->rma_data_hdr.user_flags |= FI_REMOTE_WRITE;
		} else {
			txd->rma_data_hdr.user_flags |= FI_REMOTE_READ;
		}
		txd->rma_data_hdr.user_data = req->rma.imm;
	}

	if (req->vc->peer_caps & FI_RMA_EVENT) {
		if (req->type == GNIX_FAB_RQ_RDMA_WRITE) {
			txd->rma_data_hdr.flags |= FI_REMOTE_WRITE;
		} else {
			txd->rma_data_hdr.flags |= FI_REMOTE_READ;
		}
	}

	COND_ACQUIRE(nic->requires_lock, &nic->lock);
	if (inject_err) {
		_gnix_nic_txd_err_inject(nic, txd);
		status = GNI_RC_SUCCESS;
	} else {
		status = GNI_SmsgSendWTag(req->vc->gni_ep,
					  &txd->rma_data_hdr,
					  sizeof(txd->rma_data_hdr),
					  NULL, 0, txd->id,
					  GNIX_SMSG_T_RMA_DATA);
	}
	COND_RELEASE(nic->requires_lock, &nic->lock);

	if (status == GNI_RC_NOT_DONE) {
		_gnix_nic_tx_free(nic, txd);
		GNIX_INFO(FI_LOG_EP_DATA,
			  "GNI_SmsgSendWTag returned %s\n",
			  gni_err_str[status]);
	} else if (status != GNI_RC_SUCCESS) {
		_gnix_nic_tx_free(nic, txd);
		GNIX_WARN(FI_LOG_EP_DATA,
			  "GNI_SmsgSendWTag returned %s\n",
			  gni_err_str[status]);
	} else {
		GNIX_INFO(FI_LOG_EP_DATA, "Sent RMA CQ data, req: %p\n", req);
	}

	return gnixu_to_fi_errno(status);
}

static int __gnix_rma_more_txd_complete(void *arg, gni_return_t tx_status)
{
	struct gnix_tx_descriptor *txd = (struct gnix_tx_descriptor *)arg;
	struct gnix_fab_req *req = txd->req;
	struct gnix_fab_req *more_req;
	struct slist_entry *item;
	int rc = FI_SUCCESS;

	if (tx_status != GNI_RC_SUCCESS) {
		return __gnix_rma_post_err(req, FI_ECANCELED);
	}

	free(txd->gni_more_ct_descs);
	_gnix_nic_tx_free(req->gnix_ep->nic, txd);

	/* Successful Delivery. Copy any unaligned data if read req. */
	if ((req->flags & FI_MORE) && (req->type == GNIX_FAB_RQ_RDMA_READ)) {
		if (req->flags & GNIX_RMA_INDIRECT) {
			GNIX_DEBUG(FI_LOG_EP_DATA,
				"Top TXD is Indirect. Copying\n");
			__gnix_rma_copy_indirect_get_data(req);
		} else {
			__gnix_rma_copy_chained_get_data(req);
		}
		slist_entry_foreach(req->rma.sle.next, item) {
			more_req = container_of(item, struct gnix_fab_req,
						rma.sle);
			if (more_req->flags & GNIX_RMA_INDIRECT) {
				GNIX_DEBUG(FI_LOG_EP_DATA,
				"Chain Element is Indirect. Copying\n");
				__gnix_rma_copy_indirect_get_data(more_req);
			} else {
				__gnix_rma_copy_chained_get_data(more_req);
			}
		}
	}
	/* Completion event for top level request */
	rc = __gnix_rma_send_completion(req->vc->ep, req);
	if (rc != FI_SUCCESS)
		GNIX_WARN(FI_LOG_EP_DATA,
			  "__gnix_rma_send_completion() failed: %d\n",
			  rc);
	__gnix_rma_fr_complete(req);

	/* Create completion events for each fab request */
	slist_entry_foreach(req->rma.sle.next, item) {
		more_req = container_of(item, struct gnix_fab_req, rma.sle);

		rc = __gnix_rma_send_completion(more_req->vc->ep, more_req);
		if (rc != FI_SUCCESS)
		GNIX_WARN(FI_LOG_EP_DATA,
			  "__gnix_rma_send_completion() failed: %d\n",
			  rc);
		__gnix_rma_more_fr_complete(more_req);
	}
	return FI_SUCCESS;
}

static int __gnix_rma_txd_complete(void *arg, gni_return_t tx_status)
{
	struct gnix_tx_descriptor *txd = (struct gnix_tx_descriptor *)arg;
	struct gnix_fab_req *req = txd->req;
	int rc = FI_SUCCESS;

	/* Wait for both TXDs before processing RDMA chained requests. */
	if (req->flags & GNIX_RMA_CHAINED && req->flags & GNIX_RMA_RDMA) {
		/* There are two TXDs involved with this request, an RDMA
		 * transfer to move the middle block and an FMA transfer to
		 * move unaligned head and/or tail.  If this is the FMA TXD,
		 * copy the unaligned data to the user buffer. */
		if (txd->gni_desc.type == GNI_POST_FMA_GET)
			__gnix_rma_copy_chained_get_data(req);

		/* Remember any failure.  Retransmit both TXDs once both are
		 * complete. */
		req->rma.status |= tx_status;

		if (ofi_atomic_dec32(&req->rma.outstanding_txds) == 1) {
			_gnix_nic_tx_free(req->gnix_ep->nic, txd);
			GNIX_INFO(FI_LOG_EP_DATA,
				  "Received first RDMA chain TXD, req: %p\n",
				  req);
			return FI_SUCCESS;
		}

		tx_status = req->rma.status;
	}

	if (tx_status != GNI_RC_SUCCESS) {
		_gnix_nic_tx_free(req->gnix_ep->nic, txd);
		return __gnix_rma_post_err(req, FI_ECANCELED);
	}

	/* Successful delivery.  Progress request. */
	if (req->flags & GNIX_RMA_INDIRECT) {
		__gnix_rma_copy_indirect_get_data(req);
	} else if (req->flags & GNIX_RMA_CHAINED &&
		   !(req->flags & GNIX_RMA_RDMA)) {
		__gnix_rma_copy_chained_get_data(req);
	}

	_gnix_nic_tx_free(req->gnix_ep->nic, txd);

	if (GNIX_ALLOW_FI_REMOTE_CQ_DATA(req->flags, req->gnix_ep->caps) ||
	    req->vc->peer_caps & FI_RMA_EVENT) {
		/* control message needed for imm. data or a counter event. */
		req->tx_failures = 0;
		req->work_fn = __gnix_rma_send_data_req;
		_gnix_vc_requeue_work_req(req);
	} else {
		/* complete request */
		rc = __gnix_rma_send_completion(req->vc->ep, req);
		if (rc != FI_SUCCESS)
			GNIX_WARN(FI_LOG_EP_DATA,
				  "__gnix_rma_send_completion() failed: %d\n",
				  rc);

		__gnix_rma_fr_complete(req);
	}

	return FI_SUCCESS;
}

static gni_post_type_t __gnix_fr_post_type(int fr_type, int rdma)
{
	switch (fr_type) {
	case GNIX_FAB_RQ_RDMA_WRITE:
		return rdma ? GNI_POST_RDMA_PUT : GNI_POST_FMA_PUT;
	case GNIX_FAB_RQ_RDMA_READ:
		return rdma ? GNI_POST_RDMA_GET : GNI_POST_FMA_GET;
	default:
		break;
	}

	GNIX_FATAL(FI_LOG_EP_DATA, "Unsupported post type: %d", fr_type);
	return -FI_ENOSYS;
}

static void __gnix_rma_fill_pd_chained_get(struct gnix_fab_req *req,
					   struct gnix_tx_descriptor *txd,
					   gni_mem_handle_t *rem_mdh)
{
	int head_off, head_len, tail_len, desc_idx = 0;
	struct gnix_fid_mem_desc *loc_md;
	struct gnix_fid_ep *ep = req->gnix_ep;

	if (req->int_tx_buf_e == NULL) {
		req->int_tx_buf_e = _gnix_ep_get_int_tx_buf(ep);
		if (req->int_tx_buf_e == NULL) {
			GNIX_FATAL(FI_LOG_EP_DATA, "RAN OUT OF INT_TX_BUFS");
			/* TODO return error */
		}
	}

	req->int_tx_buf = ((struct gnix_int_tx_buf *) req->int_tx_buf_e)->buf;
	req->int_tx_mdh = _gnix_ep_get_int_tx_mdh(req->int_tx_buf_e);

	/* Copy head and tail through intermediate buffer.  Copy
	 * aligned data directly to user buffer. */
	head_off = req->rma.rem_addr & GNI_READ_ALIGN_MASK;
	head_len = head_off ? GNI_READ_ALIGN - head_off : 0;
	tail_len = (req->rma.rem_addr + req->rma.len) & GNI_READ_ALIGN_MASK;

	/* Use full post descriptor for aligned data */
	loc_md = (struct gnix_fid_mem_desc *)req->rma.loc_md;
	txd->gni_desc.local_mem_hndl = loc_md->mem_hndl;
	txd->gni_desc.local_addr = (uint64_t)req->rma.loc_addr + head_len;
	txd->gni_desc.remote_addr = (uint64_t)req->rma.rem_addr + head_len;
	txd->gni_desc.length = req->rma.len - head_len - tail_len;
	assert(txd->gni_desc.length);
	txd->gni_desc.next_descr = &txd->gni_ct_descs[0];

	if (head_off) {
		txd->gni_ct_descs[0].ep_hndl = req->vc->gni_ep;
		txd->gni_ct_descs[0].length = GNI_READ_ALIGN;
		txd->gni_ct_descs[0].remote_addr =
				req->rma.rem_addr & ~GNI_READ_ALIGN_MASK;
		txd->gni_ct_descs[0].remote_mem_hndl = *rem_mdh;
		txd->gni_ct_descs[0].local_addr = (uint64_t)req->int_tx_buf;
		txd->gni_ct_descs[0].local_mem_hndl = req->int_tx_mdh;

		if (tail_len)
			txd->gni_ct_descs[0].next_descr =
					&txd->gni_ct_descs[1];
		else
			txd->gni_ct_descs[0].next_descr = NULL;

		desc_idx++;
	}

	if (tail_len) {
		txd->gni_ct_descs[desc_idx].ep_hndl = req->vc->gni_ep;
		txd->gni_ct_descs[desc_idx].length = GNI_READ_ALIGN;
		txd->gni_ct_descs[desc_idx].remote_addr =
				(req->rma.rem_addr +
				 req->rma.len) & ~GNI_READ_ALIGN_MASK;
		txd->gni_ct_descs[desc_idx].remote_mem_hndl = *rem_mdh;
		txd->gni_ct_descs[desc_idx].local_addr =
				(uint64_t)req->int_tx_buf + GNI_READ_ALIGN;
		txd->gni_ct_descs[desc_idx].local_mem_hndl = req->int_tx_mdh;
		txd->gni_ct_descs[desc_idx].next_descr = NULL;
	}

	GNIX_INFO(FI_LOG_EP_DATA,
		  "ct_rem_addr[0] = %p %p, ct_rem_addr[1] = %p %p\n",
		  txd->gni_ct_descs[0].remote_addr,
		  txd->gni_ct_descs[0].local_addr,
		  txd->gni_ct_descs[1].remote_addr,
		  txd->gni_ct_descs[1].local_addr);
}

static void __gnix_rma_fill_pd_indirect_get(struct gnix_fab_req *req,
					    struct gnix_tx_descriptor *txd)
{
	int head_off = req->rma.rem_addr & GNI_READ_ALIGN_MASK;
	struct gnix_fid_ep *ep = req->gnix_ep;

	if (req->int_tx_buf_e == NULL) {
		req->int_tx_buf_e = _gnix_ep_get_int_tx_buf(ep);
		if (req->int_tx_buf_e == NULL) {
			GNIX_FATAL(FI_LOG_EP_DATA, "RAN OUT OF INT_TX_BUFS");
			/* TODO return error */
		}
	}

	req->int_tx_buf = ((struct gnix_int_tx_buf *) req->int_tx_buf_e)->buf;
	req->int_tx_mdh = _gnix_ep_get_int_tx_mdh(req->int_tx_buf_e);

	/* Copy all data through an intermediate buffer. */
	txd->gni_desc.local_addr = (uint64_t)req->int_tx_buf;
	txd->gni_desc.local_mem_hndl = req->int_tx_mdh;
	txd->gni_desc.length = CEILING(req->rma.len + head_off, GNI_READ_ALIGN);
	txd->gni_desc.remote_addr =
			(uint64_t)req->rma.rem_addr & ~GNI_READ_ALIGN_MASK;
}

static void __gnix_rma_more_fill_pd_indirect_get(struct gnix_fab_req *req,
						 gni_ct_get_post_descriptor_t
						 *more_get, int idx)
{
	int head_off = req->rma.rem_addr & GNI_READ_ALIGN_MASK;
	struct gnix_fid_ep *ep = req->gnix_ep;

	GNIX_DEBUG(FI_LOG_EP_DATA, "FI_MORE: filling indirect get\n");
	if (req->int_tx_buf_e == NULL) {
		req->int_tx_buf_e = _gnix_ep_get_int_tx_buf(ep);
		if (req->int_tx_buf_e == NULL) {
			GNIX_FATAL(FI_LOG_EP_DATA, "RAN OUT OF INT_TX_BUFS");
			/* TODO return error */
		}
	}

	req->int_tx_buf = ((struct gnix_int_tx_buf *) req->int_tx_buf_e)->buf;
	req->int_tx_mdh = _gnix_ep_get_int_tx_mdh(req->int_tx_buf_e);

	/* Copy all data through an intermediate buffer. */
	more_get[idx].local_addr = (uint64_t)req->int_tx_buf;
	more_get[idx].local_mem_hndl = req->int_tx_mdh;
	more_get[idx].length = CEILING(req->rma.len + head_off, GNI_READ_ALIGN);
	more_get[idx].remote_addr =
			(uint64_t)req->rma.rem_addr & ~GNI_READ_ALIGN_MASK;
}

int _gnix_rma_post_irq(struct gnix_vc *vc)
{
	int rc = FI_SUCCESS;
	struct gnix_fid_ep *ep;
	struct gnix_nic *nic;
	struct gnix_tx_descriptor *txd;
	gni_return_t status;

#if 1
	if (vc->conn_state != GNIX_VC_CONNECTED)
		return -FI_EINVAL;

	ep = vc->ep;
	assert(ep != NULL);

	nic = ep->nic;
	assert(nic != NULL);

	rc = _gnix_nic_tx_alloc(nic, &txd);
	if (rc != FI_SUCCESS)
		return rc;

	txd->completer_fn = __gnix_rma_post_irq_complete;
	txd->req = (void *)vc;
	txd->gni_desc.type = GNI_POST_CQWRITE;
	txd->gni_desc.cq_mode = GNI_CQMODE_GLOBAL_EVENT;
	/*
 	 * try to send the cq write request through the network
 	 * on the slow path
 	 */
	txd->gni_desc.dlvr_mode = GNI_DLVMODE_IN_ORDER;
	txd->gni_desc.remote_mem_hndl = vc->peer_irq_mem_hndl;
	txd->gni_desc.cqwrite_value = vc->peer_id;
	txd->gni_desc.rdma_mode = 0;
	txd->gni_desc.src_cq_hndl = nic->tx_cq; /* check flags */

	status = GNI_PostCqWrite(vc->gni_ep,
				 &txd->gni_desc);
	if (OFI_UNLIKELY(status != GNI_RC_SUCCESS)) {
		rc = gnixu_to_fi_errno(status);
		_gnix_nic_tx_free(nic, txd);
	}
#endif

	return rc;
}

int _gnix_rma_post_rdma_chain_req(void *data)
{
	struct gnix_fab_req *req = (struct gnix_fab_req *)data;
	struct gnix_fid_ep *ep = req->gnix_ep;
	struct gnix_nic *nic = ep->nic;
	struct gnix_tx_descriptor *bte_txd, *ct_txd;
	gni_mem_handle_t mdh;
	gni_return_t status;
	int rc;
	int inject_err = _gnix_req_inject_err(req);
	int head_off, head_len, tail_len;
	int fma_chain = 0;

	if (req->int_tx_buf_e == NULL) {
		req->int_tx_buf_e = _gnix_ep_get_int_tx_buf(ep);
		if (req->int_tx_buf_e == NULL) {
			GNIX_WARN(FI_LOG_EP_DATA, "RAN OUT OF INT_TX_BUFS");
			return -FI_ENOSPC;
		}
	}

	req->int_tx_buf = ((struct gnix_int_tx_buf *) req->int_tx_buf_e)->buf;
	req->int_tx_mdh = _gnix_ep_get_int_tx_mdh(req->int_tx_buf_e);

	if (!gnix_ops_allowed(ep, req->vc->peer_caps, req->flags)) {
		rc = __gnix_rma_post_err_no_retrans(req, FI_EOPNOTSUPP);
		if (rc != FI_SUCCESS)
			GNIX_WARN(FI_LOG_EP_DATA,
				  "__gnix_rma_post_err_no_retrans() failed: %d\n",
				  rc);
		return -FI_ECANCELED;
	}

	rc = _gnix_nic_tx_alloc(nic, &bte_txd);
	if (rc) {
		GNIX_INFO(FI_LOG_EP_DATA,
			  "BTE _gnix_nic_tx_alloc() failed: %d\n",
			  rc);
		return -FI_ENOSPC;
	}

	rc = _gnix_nic_tx_alloc(nic, &ct_txd);
	if (rc) {
		_gnix_nic_tx_free(nic, bte_txd);
		GNIX_INFO(FI_LOG_EP_DATA,
			  "CT _gnix_nic_tx_alloc() failed: %d\n",
			  rc);
		return -FI_ENOSPC;
	}

	_GNIX_CONVERT_MR_KEY(ep->auth_key->using_vmdh,
		req->vc->peer_key_offset,
		_gnix_convert_key_to_mhdl,
		&req->rma.rem_mr_key, &mdh);

	/* BTE TXD */
	bte_txd->completer_fn = __gnix_rma_txd_complete;
	bte_txd->req = req;
	bte_txd->gni_desc.type = GNI_POST_RDMA_GET;
	bte_txd->gni_desc.cq_mode = GNI_CQMODE_GLOBAL_EVENT; /* check flags */
	bte_txd->gni_desc.dlvr_mode = GNI_DLVMODE_PERFORMANCE; /* check flags */

	head_off = req->rma.rem_addr & GNI_READ_ALIGN_MASK;
	head_len = head_off ? GNI_READ_ALIGN - head_off : 0;
	tail_len = (req->rma.rem_addr + req->rma.len) & GNI_READ_ALIGN_MASK;

	bte_txd->gni_desc.local_addr = (uint64_t)req->rma.loc_addr + head_len;
	bte_txd->gni_desc.remote_addr = (uint64_t)req->rma.rem_addr + head_len;
	bte_txd->gni_desc.length = req->rma.len - head_len - tail_len;

	bte_txd->gni_desc.remote_mem_hndl = mdh;
	bte_txd->gni_desc.rdma_mode = 0; /* check flags */
	bte_txd->gni_desc.src_cq_hndl = nic->tx_cq; /* check flags */
	bte_txd->gni_desc.local_mem_hndl = req->rma.loc_md->mem_hndl;

	GNIX_LOG_DUMP_TXD(bte_txd);

	/* FMA TXD */
	ct_txd->completer_fn = __gnix_rma_txd_complete;
	ct_txd->req = req;
	ct_txd->gni_desc.type = GNI_POST_FMA_GET;
	ct_txd->gni_desc.cq_mode = GNI_CQMODE_GLOBAL_EVENT; /* check flags */
	ct_txd->gni_desc.dlvr_mode = GNI_DLVMODE_PERFORMANCE; /* check flags */

	ct_txd->gni_desc.remote_mem_hndl = mdh;
	ct_txd->gni_desc.rdma_mode = 0; /* check flags */
	ct_txd->gni_desc.src_cq_hndl = nic->tx_cq; /* check flags */
	ct_txd->gni_desc.local_mem_hndl = req->int_tx_mdh;
	ct_txd->gni_desc.length = GNI_READ_ALIGN;

	if (head_off) {
		ct_txd->gni_desc.remote_addr =
				req->rma.rem_addr & ~GNI_READ_ALIGN_MASK;
		ct_txd->gni_desc.local_addr = (uint64_t)req->int_tx_buf;

		if (tail_len) {
			ct_txd->gni_desc.next_descr = &ct_txd->gni_ct_descs[0];
			ct_txd->gni_ct_descs[0].ep_hndl = req->vc->gni_ep;
			ct_txd->gni_ct_descs[0].length = GNI_READ_ALIGN;
			ct_txd->gni_ct_descs[0].remote_addr =
					(req->rma.rem_addr +
					 req->rma.len) & ~GNI_READ_ALIGN_MASK;
			ct_txd->gni_ct_descs[0].remote_mem_hndl = mdh;
			ct_txd->gni_ct_descs[0].local_addr =
					(uint64_t)req->int_tx_buf +
					GNI_READ_ALIGN;
			ct_txd->gni_ct_descs[0].local_mem_hndl =
					req->int_tx_mdh;
			ct_txd->gni_ct_descs[0].next_descr = NULL;
			fma_chain = 1;
		}
	} else {
		ct_txd->gni_desc.remote_addr =
				(req->rma.rem_addr +
				 req->rma.len) & ~GNI_READ_ALIGN_MASK;
		ct_txd->gni_desc.local_addr =
				(uint64_t)req->int_tx_buf + GNI_READ_ALIGN;
	}

	GNIX_LOG_DUMP_TXD(ct_txd);

	COND_ACQUIRE(nic->requires_lock, &nic->lock);

	/*
	 * TODO: need work here too!
	 */

	if (OFI_UNLIKELY(inject_err)) {
		_gnix_nic_txd_err_inject(nic, bte_txd);
		status = GNI_RC_SUCCESS;
	} else {
		status = GNI_PostRdma(req->vc->gni_ep,
				      &bte_txd->gni_desc);
	}

	if (status != GNI_RC_SUCCESS) {
		COND_RELEASE(nic->requires_lock, &nic->lock);
		_gnix_nic_tx_free(nic, ct_txd);
		_gnix_nic_tx_free(nic, bte_txd);

		GNIX_INFO(FI_LOG_EP_DATA, "GNI_Post*() failed: %s\n",
			  gni_err_str[status]);
		return gnixu_to_fi_errno(status);
	}

	if (OFI_UNLIKELY(inject_err)) {
		_gnix_nic_txd_err_inject(nic, ct_txd);
		status = GNI_RC_SUCCESS;
	} else if (fma_chain) {
		status = GNI_CtPostFma(req->vc->gni_ep,
				       &ct_txd->gni_desc);
	} else {
		status = GNI_PostFma(req->vc->gni_ep,
				     &ct_txd->gni_desc);
	}

	if (status != GNI_RC_SUCCESS) {
		COND_RELEASE(nic->requires_lock, &nic->lock);
		_gnix_nic_tx_free(nic, ct_txd);

		/* Wait for the first TX to complete, then retransmit the
		 * entire thing. */
		ofi_atomic_set32(&req->rma.outstanding_txds, 1);
		req->rma.status = GNI_RC_TRANSACTION_ERROR;

		GNIX_INFO(FI_LOG_EP_DATA, "GNI_Post*() failed: %s\n",
			  gni_err_str[status]);
		return FI_SUCCESS;
	}

	COND_RELEASE(nic->requires_lock, &nic->lock);

	/* Wait for both TXs to complete, then process the request. */
	ofi_atomic_set32(&req->rma.outstanding_txds, 2);
	req->rma.status = 0;

	return FI_SUCCESS;
}

/* Fill head and tail descriptors for chained gets */
static void __gnix_rma_more_fill_sub_htd(struct gnix_fab_req *req,
					gni_ct_get_post_descriptor_t *more_get,
					gni_mem_handle_t *rem_mdh,
					int *idx, int *entries)
{
	int head_off, tail_len;
	struct gnix_fid_ep *ep = req->gnix_ep;
	int indirect = !!(req->flags & GNIX_RMA_INDIRECT);

	head_off = req->rma.rem_addr & GNI_READ_ALIGN_MASK;
	tail_len = (req->rma.rem_addr + req->rma.len)
		& GNI_READ_ALIGN_MASK;

	/* Get int_tx_buf if unaligned and not indirect size. */
	if ((req->int_tx_buf_e == NULL) &&
			(head_off || tail_len) && !(indirect)) {
		req->int_tx_buf_e = _gnix_ep_get_int_tx_buf(ep);
		if (req->int_tx_buf_e == NULL) {
			GNIX_FATAL(FI_LOG_EP_DATA, "RAN OUT OF INT_TX_BUFS");
			/* TODO return error */
		}
	} else {
		return;
	}

	req->int_tx_buf = ((struct gnix_int_tx_buf *)
				req->int_tx_buf_e)->buf;
	req->int_tx_mdh = _gnix_ep_get_int_tx_mdh(req->int_tx_buf_e);

	if (head_off) {
		assert(*idx < *entries);
		GNIX_DEBUG(FI_LOG_EP_DATA, "FI_MORE: Chain Head Off\n");
		more_get[*idx].ep_hndl = req->vc->gni_ep;
		more_get[*idx].length = GNI_READ_ALIGN;
		more_get[*idx].remote_addr =
				req->rma.rem_addr & ~GNI_READ_ALIGN_MASK;
		more_get[*idx].remote_mem_hndl = *rem_mdh;
		more_get[*idx].local_addr = (uint64_t)req->int_tx_buf;
		more_get[*idx].local_mem_hndl = req->int_tx_mdh;

		if (*idx < (*entries) - 1)
			more_get[*idx].next_descr = &more_get[(*idx) + 1];
		else
			more_get[*idx].next_descr = NULL;

		(*idx)++;
	}
	if (tail_len) {
		assert(*idx < *entries);
		GNIX_DEBUG(FI_LOG_EP_DATA, "FI_MORE: Chain Tail Off\n");
		more_get[*idx].ep_hndl = req->vc->gni_ep;
		more_get[*idx].length = GNI_READ_ALIGN;
		more_get[*idx].remote_addr =
				(req->rma.rem_addr +
				 req->rma.len) & ~GNI_READ_ALIGN_MASK;
		more_get[*idx].remote_mem_hndl = *rem_mdh;
		more_get[*idx].local_addr =
				(uint64_t)req->int_tx_buf + GNI_READ_ALIGN;
		more_get[*idx].local_mem_hndl = req->int_tx_mdh;
		more_get[*idx].next_descr = NULL;

		if (*idx < (*entries) - 1)
				more_get[*idx].next_descr = &more_get[(*idx) + 1];
			else
				more_get[*idx].next_descr = NULL;
		(*idx)++;
	}
}

static void __gnix_rma_more_fill_pd(struct gnix_fab_req *req,
				    struct gnix_tx_descriptor *txd)
{
	gni_ct_put_post_descriptor_t *more_put = NULL;
	gni_ct_get_post_descriptor_t *more_get = NULL;
	gni_mem_handle_t mdh;
	struct gnix_fab_req *more_req;
	struct gnix_fid_mem_desc *loc_md;
	struct slist_entry *item;
	int head_off, head_len = 0, tail_len = 0, entries = 0, idx = 0;
	int indirect = !!(req->flags & GNIX_RMA_INDIRECT);
	int sub_indirect;

	GNIX_DEBUG(FI_LOG_EP_DATA, "FI_MORE: Filling PD\n");
	/* If top level fab_req is unaligned, increment entries. */
	if (req->type == GNIX_FAB_RQ_RDMA_READ) {
		head_off = req->rma.rem_addr & GNI_READ_ALIGN_MASK;
		head_len = head_off ? GNI_READ_ALIGN - head_off : 0;
		tail_len = (req->rma.rem_addr + req->rma.len)
			& GNI_READ_ALIGN_MASK;
		if (!indirect) {
			if (head_off) {
				entries++;
				GNIX_DEBUG(FI_LOG_EP_DATA,
				"FI_MORE: Txd Head Off\n");
			}
			if (tail_len) {
				entries++;
				GNIX_DEBUG(FI_LOG_EP_DATA,
				"FI_MORE: Txd Tail Off\n");
			}
		}
		/* Populate txd */
		if (indirect) {
			GNIX_DEBUG(FI_LOG_EP_DATA, "FI_MORE: Txd Indirect\n");
			__gnix_rma_fill_pd_indirect_get(req, txd);
		} else {
			txd->gni_desc.local_addr = (uint64_t)req->rma.loc_addr + head_len;
			txd->gni_desc.remote_addr = (uint64_t)req->rma.rem_addr + head_len;
			txd->gni_desc.length = req->rma.len - head_len - tail_len;
			assert(txd->gni_desc.length);
		}
	}

	/* Count number of sub post-descriptors to be chained. */
	slist_entry_foreach(req->rma.sle.next, item) {
		entries++; /* Increment regardless of type */
		/* Get fab_req pointer */
		if (req->type == GNIX_FAB_RQ_RDMA_READ) {
			more_req = container_of(item, struct gnix_fab_req,
						rma.sle);
			sub_indirect = !!(more_req->flags & GNIX_RMA_INDIRECT);
			head_off = more_req->rma.rem_addr & GNI_READ_ALIGN_MASK;
			head_len = head_off ? GNI_READ_ALIGN - head_off : 0;
			tail_len = (more_req->rma.rem_addr + more_req->rma.len)
				& GNI_READ_ALIGN_MASK;

			/* Additional increments if unaligned */
			if (!sub_indirect) {
				if (head_off) {
					entries++;
					GNIX_DEBUG(FI_LOG_EP_DATA,
					"FI_MORE: Chain Req Head off++\n");
				}
				if (tail_len) {
					entries++;
					GNIX_DEBUG(FI_LOG_EP_DATA,
					"FI_MORE: Chaing Req Tail off++\n");
				}
			}
		}
	}

	GNIX_INFO(FI_LOG_EP_DATA, "FI_MORE: %d sub descs to be populated\n",
		  entries);

	/* Allocate space for sub descriptors */
	if (entries > 0) {
		if (req->type == GNIX_FAB_RQ_RDMA_WRITE) {
			txd->gni_more_ct_descs = malloc(entries *
					 sizeof(gni_ct_put_post_descriptor_t));
			more_put = (gni_ct_put_post_descriptor_t *)
				   txd->gni_more_ct_descs;
		} else {
			txd->gni_more_ct_descs = malloc(entries *
				sizeof(gni_ct_get_post_descriptor_t));
			more_get = (gni_ct_get_post_descriptor_t *)
				   txd->gni_more_ct_descs;
			/* Populate Head/Tail of TOP Req if req is unaligned */
			 __gnix_rma_more_fill_sub_htd(req, more_get,
				&txd->gni_desc.remote_mem_hndl, &idx, &entries);
		}
	}

	/* Populate sub descriptors */
	slist_entry_foreach(req->rma.sle.next, item) {
		/* Get fab_req pointer */
		more_req = container_of(item, struct gnix_fab_req, rma.sle);
		sub_indirect = !!(more_req->flags & GNIX_RMA_INDIRECT);

		/* Populate  based on type */
		if (req->type == GNIX_FAB_RQ_RDMA_WRITE) {
			assert(more_put);
			more_put[idx].ep_hndl = more_req->vc->gni_ep;
			more_put[idx].length = more_req->rma.len;
			more_put[idx].remote_addr = more_req->rma.rem_addr;
			more_put[idx].local_addr = (uint64_t)more_req->
								rma.loc_addr;

			_GNIX_CONVERT_MR_KEY(more_req->vc->ep->auth_key->using_vmdh,
				more_req->vc->peer_key_offset,
				_gnix_convert_key_to_mhdl_no_crc,
				&more_req->rma.rem_mr_key, &mdh);
			more_put[idx].remote_mem_hndl = mdh;

			if (idx < entries - 1)
				more_put[idx].next_descr = &more_put[idx + 1];
			else
				more_put[idx].next_descr = NULL;
			idx++;
		} else {
			assert(more_get);
			_GNIX_CONVERT_MR_KEY(more_req->vc->ep->auth_key->using_vmdh,
				more_req->vc->peer_key_offset,
				_gnix_convert_key_to_mhdl_no_crc,
				&more_req->rma.rem_mr_key, &mdh);
			more_get[idx].remote_mem_hndl = mdh;
			more_get[idx].ep_hndl = more_req->vc->gni_ep;

			if (sub_indirect) {
				__gnix_rma_more_fill_pd_indirect_get(more_req, more_get, idx);
				if (idx < entries - 1) {
					more_get[idx].next_descr = &more_get[idx + 1];
				} else {
					more_get[idx].next_descr = NULL;
				}
				idx++;

			} else {
				/* get main aligned data in first transaction */
				loc_md = (struct gnix_fid_mem_desc *)more_req->rma.loc_md;
				more_get[idx].local_mem_hndl = loc_md->mem_hndl;
				more_get[idx].local_addr = (uint64_t)more_req->rma.loc_addr + head_len;
				more_get[idx].remote_addr = (uint64_t)more_req->rma.rem_addr + head_len;
				more_get[idx].length = more_req->rma.len - head_len - tail_len;
				GNIX_DEBUG(FI_LOG_EP_DATA, "rma.len: %d - head_len:%d - tail_len:%d\n",
					more_req->rma.len, head_len, tail_len);
				assert(more_get[idx].length);

				if (idx < entries - 1) {
					more_get[idx].next_descr = &more_get[idx + 1];
				} else {
					more_get[idx].next_descr = NULL;
				}
				idx++;
				/* head/tail function */
				__gnix_rma_more_fill_sub_htd(more_req, more_get, &mdh,
						&idx, &entries);
			}
		}
	}
	if (entries > 0) {
		if (req->type == GNIX_FAB_RQ_RDMA_WRITE) {
			txd->gni_desc.next_descr = &more_put[0];
		} else {
			txd->gni_desc.next_descr = &more_get[0];
		}
	}
}

int _gnix_rma_more_post_req(void *data)
{
	struct gnix_fab_req *fab_req = (struct gnix_fab_req *)data;
	struct gnix_fid_ep *ep = fab_req->gnix_ep;
	struct gnix_nic *nic = ep->nic;
	struct gnix_fid_mem_desc *loc_md;
	struct gnix_tx_descriptor *txd;
	gni_mem_handle_t mdh;
	gni_return_t status;
	int rc;

	if (!gnix_ops_allowed(ep, fab_req->vc->peer_caps, fab_req->flags)) {
		rc = __gnix_rma_post_err_no_retrans(fab_req, FI_EOPNOTSUPP);
		if (rc != FI_SUCCESS)
			GNIX_WARN(FI_LOG_EP_DATA,
				  "__gnix_rma_post_err_no_retrans() failed: %d\n",
				  rc);
		return -FI_ECANCELED;
	}

	rc = _gnix_nic_tx_alloc(nic, &txd);
	if (rc) {
		GNIX_WARN(FI_LOG_EP_DATA,
				"_gnix_nic_tx_alloc() failed: %d\n",
				rc);
		return -FI_ENOSPC;
	}

	txd->completer_fn = __gnix_rma_more_txd_complete;
	txd->req = fab_req;

	_GNIX_CONVERT_MR_KEY(ep->auth_key->using_vmdh,
		fab_req->vc->peer_key_offset,
		_gnix_convert_key_to_mhdl_no_crc,
		&fab_req->rma.rem_mr_key, &mdh);

	txd->gni_desc.type = __gnix_fr_post_type(fab_req->type, 0);
	txd->gni_desc.cq_mode = GNI_CQMODE_GLOBAL_EVENT; /* check flags */
	txd->gni_desc.dlvr_mode = GNI_DLVMODE_PERFORMANCE; /* check flags */

	if (fab_req->type == GNIX_FAB_RQ_RDMA_WRITE) {
		txd->gni_desc.local_addr = (uint64_t)fab_req->rma.loc_addr;
		txd->gni_desc.length = fab_req->rma.len;
		assert(txd->gni_desc.length);
		txd->gni_desc.remote_addr = (uint64_t)fab_req->rma.rem_addr;
	}

	loc_md = (struct gnix_fid_mem_desc *)fab_req->rma.loc_md;
	txd->gni_desc.local_mem_hndl = loc_md->mem_hndl; /* assert? */
	txd->gni_desc.remote_mem_hndl = mdh;
	txd->gni_desc.rdma_mode = 0; /* check flags */
	txd->gni_desc.src_cq_hndl = nic->tx_cq; /* check flags */
	txd->gni_desc.next_descr = NULL; /*Assume no sub descs */
	txd->gni_desc.prev_descr = NULL;

	__gnix_rma_more_fill_pd(fab_req, txd);

	GNIX_LOG_DUMP_TXD(txd);

	COND_ACQUIRE(nic->requires_lock, &nic->lock);
	status = GNI_CtPostFma(fab_req->vc->gni_ep, &txd->gni_desc);
	GNIX_DEBUG(FI_LOG_EP_DATA, "FI_MORE: Returned from GNI_CtPostFma\n");

	COND_RELEASE(nic->requires_lock, &nic->lock);

	if (status != GNI_RC_SUCCESS) {
		free(txd->gni_more_ct_descs);
		_gnix_nic_tx_free(nic, txd);
		GNIX_WARN(FI_LOG_EP_DATA, "GNI_Post*() failed: %s\n",
			  gni_err_str[status]);
	}

	return gnixu_to_fi_errno(status);
}
int _gnix_rma_post_req(void *data)
{
	struct gnix_fab_req *fab_req = (struct gnix_fab_req *)data;
	struct gnix_fid_ep *ep = fab_req->gnix_ep;
	struct gnix_nic *nic = ep->nic;
	struct gnix_fid_mem_desc *loc_md;
	struct gnix_tx_descriptor *txd;
	gni_mem_handle_t mdh;
	gni_return_t status;
	int rc;
	int rdma = !!(fab_req->flags & GNIX_RMA_RDMA);
	int indirect = !!(fab_req->flags & GNIX_RMA_INDIRECT);
	int chained = !!(fab_req->flags & GNIX_RMA_CHAINED);
	int inject_err = _gnix_req_inject_err(fab_req);

	if (!gnix_ops_allowed(ep, fab_req->vc->peer_caps, fab_req->flags)) {
		rc = __gnix_rma_post_err_no_retrans(fab_req, FI_EOPNOTSUPP);
		if (rc != FI_SUCCESS)
			GNIX_WARN(FI_LOG_EP_DATA,
				  "__gnix_rma_post_err_no_retrans() failed: %d\n",
				  rc);
		return -FI_ECANCELED;
	}

	rc = _gnix_nic_tx_alloc(nic, &txd);
	if (rc) {
		GNIX_INFO(FI_LOG_EP_DATA,
				"_gnix_nic_tx_alloc() failed: %d\n",
				rc);
		return -FI_ENOSPC;
	}

	txd->completer_fn = __gnix_rma_txd_complete;
	txd->req = fab_req;

	if (rdma) {
		_GNIX_CONVERT_MR_KEY(ep->auth_key,
			fab_req->vc->peer_key_offset,
			_gnix_convert_key_to_mhdl,
			&fab_req->rma.rem_mr_key, &mdh);
	} else {
		/* Mem handle CRC is not validated during FMA operations.  Skip
		 * this costly calculation. */
		_GNIX_CONVERT_MR_KEY(ep->auth_key,
			fab_req->vc->peer_key_offset,
			_gnix_convert_key_to_mhdl_no_crc,
			&fab_req->rma.rem_mr_key, &mdh);
	}

	txd->gni_desc.type = __gnix_fr_post_type(fab_req->type, rdma);
	txd->gni_desc.cq_mode = GNI_CQMODE_GLOBAL_EVENT; /* check flags */
	txd->gni_desc.dlvr_mode = GNI_DLVMODE_PERFORMANCE; /* check flags */

	if (OFI_UNLIKELY(indirect)) {
		__gnix_rma_fill_pd_indirect_get(fab_req, txd);
	} else if (OFI_UNLIKELY(chained)) {
		__gnix_rma_fill_pd_chained_get(fab_req, txd, &mdh);
	} else {
		txd->gni_desc.local_addr = (uint64_t)fab_req->rma.loc_addr;
		txd->gni_desc.length = fab_req->rma.len;
		txd->gni_desc.remote_addr = (uint64_t)fab_req->rma.rem_addr;

		loc_md = (struct gnix_fid_mem_desc *)fab_req->rma.loc_md;
		if (loc_md) {
			txd->gni_desc.local_mem_hndl = loc_md->mem_hndl;
		}
	}

	txd->gni_desc.remote_mem_hndl = mdh;
	txd->gni_desc.rdma_mode = 0; /* check flags */
	txd->gni_desc.src_cq_hndl = nic->tx_cq; /* check flags */

	GNIX_LOG_DUMP_TXD(txd);

	COND_ACQUIRE(nic->requires_lock, &nic->lock);

	if (OFI_UNLIKELY(inject_err)) {
		_gnix_nic_txd_err_inject(nic, txd);
		status = GNI_RC_SUCCESS;
	} else if (chained) {
		status = GNI_CtPostFma(fab_req->vc->gni_ep, &txd->gni_desc);
	} else if (rdma) {
		status = GNI_PostRdma(fab_req->vc->gni_ep, &txd->gni_desc);
	} else {
		status = GNI_PostFma(fab_req->vc->gni_ep, &txd->gni_desc);
	}

	COND_RELEASE(nic->requires_lock, &nic->lock);

	if (status != GNI_RC_SUCCESS) {
		_gnix_nic_tx_free(nic, txd);
		GNIX_WARN(FI_LOG_EP_DATA, "GNI_Post*() failed: %s\n",
			  gni_err_str[status]);
	}

	return gnixu_to_fi_errno(status);
}

/**
 * @brief Create an RMA request.
 *
 * Creates a new RMA request.  Reads and writes are supported.  GNI supports
 * writing to local and remote addresses with any alignment and length.  GNI
 * requires reads to use four byte aligned remote address and length.  For
 * reads smaller than one cacheline, aligned data is read into an intermediate
 * buffer, then partially copied to the user buffer (the code terms this an
 * 'INDIRECT' transfer).  For larger unaligned reads, the interior, aligned
 * portion of remote data is pulled directly into the user provided buffer.
 * The four bytes at the head and tail of an unaliged read are pulled into an
 * intermediate buffer, then partially copied into the user buffer.  This
 * method is termed a 'CHAINED' transfer in the code.  Unaligned reads smaller
 * than the RDMA threshold can perform these 3 distinct transactions (head,
 * middle, tail) in a single GNI chained FMA operation (resulting in a single
 * GNI CQE).  For unaligned reads larger than the RDMA threshold, two GNI posts
 * are used, one RDMA TX to transfer the bulk of the data, another FMA TX to
 * transfer the head and/or tail data.
 *
 * @param ep The endpiont to use for the RMA request.
 * @param fr_type RMA request type.
 * @param loc_addr Local address for the RMA request.
 * @param len Length of the RMA request.
 * @param mdesc Local memory descriptor for the RMA request.
 * @param dest_addr Remote endpiont address for the RMA request.
 * @param rem_addr Remote address for the RMA request.
 * @param mkey Remote memory key for the RMA request.
 * @param context Event context for the RMA request.
 * @param flags Flags for the RMA request
 * @param data Remote event data for the RMA request.
 *
 * @return FI_SUCCESS on success.  FI_EINVAL for invalid parameter.  -FI_ENOSPC
 *         for low memory.
 */
ssize_t _gnix_rma(struct gnix_fid_ep *ep, enum gnix_fab_req_type fr_type,
		  uint64_t loc_addr, size_t len, void *mdesc,
		  uint64_t dest_addr, uint64_t rem_addr, uint64_t mkey,
		  void *context, uint64_t flags, uint64_t data)
{
	struct gnix_vc *vc;
	struct gnix_fab_req *req;
	struct gnix_fid_mem_desc *md = NULL;
	int rc;
	int rdma;
	struct fid_mr *auto_mr = NULL;
	struct gnix_fab_req *more_req;
	struct slist_entry *sle;
	int connected;
	struct gnix_auth_key *info;

	if (!(flags & FI_INJECT) && !ep->send_cq &&
	    (((fr_type == GNIX_FAB_RQ_RDMA_WRITE) && !ep->write_cntr) ||
	     ((fr_type == GNIX_FAB_RQ_RDMA_READ) && !ep->read_cntr))) {
		return -FI_ENOCQ;
	}

	if (flags & FI_TRIGGER) {
		struct fi_triggered_context *trigger_context =
				(struct fi_triggered_context *)context;
		if ((trigger_context->event_type != FI_TRIGGER_THRESHOLD) ||
		    (flags & FI_INJECT)) {
			return -FI_EINVAL;
		}
	}

	if ((flags & FI_INJECT) && (len > GNIX_INJECT_SIZE)) {
		GNIX_INFO(FI_LOG_EP_DATA,
			  "RMA length %d exceeds inject max size: %d\n",
			  len, GNIX_INJECT_SIZE);
		return -FI_EINVAL;
	}

	/* setup fabric request */
	req = _gnix_fr_alloc(ep);
	if (!req) {
		GNIX_INFO(FI_LOG_EP_DATA, "_gnix_fr_alloc() failed\n");
		return -FI_ENOSPC;
	}

	rdma = len >= ep->domain->params.rma_rdma_thresh;

	req->type = fr_type;
	req->gnix_ep = ep;
	req->user_context = context;
	req->work_fn = _gnix_rma_post_req;
	req->rma.sle.next = NULL;
	ofi_atomic_initialize32(&req->rma.outstanding_txds, 0);

	if (fr_type == GNIX_FAB_RQ_RDMA_READ &&
	    (rem_addr & GNI_READ_ALIGN_MASK || len & GNI_READ_ALIGN_MASK)) {
		if (len >= GNIX_RMA_UREAD_CHAINED_THRESH) {
			GNIX_INFO(FI_LOG_EP_DATA,
				  "Using CT for unaligned GET, req: %p\n",
				  req);
			flags |= GNIX_RMA_CHAINED;
		} else {
			GNIX_INFO(FI_LOG_EP_DATA,
				  "Using tmp buf for unaligned GET, req: %p\n",
				  req);
			flags |= GNIX_RMA_INDIRECT;
		}

		if (rdma)
			req->work_fn = _gnix_rma_post_rdma_chain_req;
	}

	if (!(flags & (GNIX_RMA_INDIRECT | FI_INJECT)) && !mdesc &&
	    (rdma || fr_type == GNIX_FAB_RQ_RDMA_READ)) {
		uint64_t requested_key;

		info = ep->auth_key;
		assert(info);

		if (info->using_vmdh)
			requested_key = _gnix_get_next_reserved_key(info);
		else
			requested_key = 0;

		/* We need to auto-register the source buffer. */
		rc = _gnix_mr_reg(&ep->domain->domain_fid.fid, (void *)loc_addr,
				 len, FI_READ | FI_WRITE, 0, requested_key,
				 0, &auto_mr, NULL, ep->auth_key,
				 GNIX_PROV_REG);
		if (rc != FI_SUCCESS) {
			GNIX_INFO(FI_LOG_EP_DATA,
				  "Failed to auto-register local buffer: %d\n",
				  rc);
			goto err_auto_reg;
		}
		flags |= FI_LOCAL_MR;
		mdesc = (void *)auto_mr;
		GNIX_INFO(FI_LOG_EP_DATA, "auto-reg MR: %p\n", auto_mr);
	}

	if (mdesc)
		md = container_of(mdesc, struct gnix_fid_mem_desc, mr_fid);
	req->rma.loc_md = (void *)md;

	req->rma.rem_addr = rem_addr;
	req->rma.rem_mr_key = mkey;
	req->rma.len = len;
	req->rma.imm = data;
	req->flags = flags;

	if (req->flags & FI_INJECT) {
		memcpy(req->inject_buf, (void *)loc_addr, len);
		req->rma.loc_addr = (uint64_t)req->inject_buf;
	} else {
		req->rma.loc_addr = loc_addr;
	}

	/* Inject interfaces always suppress completions.  If
	 * SELECTIVE_COMPLETION is set, honor any setting.  Otherwise, always
	 * deliver a completion. */
	if ((flags & GNIX_SUPPRESS_COMPLETION) ||
	    (ep->send_selective_completion && !(flags & FI_COMPLETION))) {
		req->flags &= ~FI_COMPLETION;
	} else {
		req->flags |= FI_COMPLETION;
	}

	if (rdma) {
		req->flags |= GNIX_RMA_RDMA;
	}

	COND_ACQUIRE(ep->requires_lock, &ep->vc_lock);

	/* find VC for target */
	rc = _gnix_vc_ep_get_vc(ep, dest_addr, &vc);
	if (rc) {
		GNIX_INFO(FI_LOG_EP_DATA,
			  "_gnix_vc_ep_get_vc() failed, addr: %lx, rc:%d\n",
			  dest_addr, rc);
		goto err_get_vc;
	}

	req->vc = vc;
	connected = (vc->conn_state == GNIX_VC_CONNECTED);

	/* Adding FI_FENCE to an FI_MORE list will break the FI_MORE Chain.
	 * Current FI_MORE implementation does not create remote CQ events.
	 * Remove FI_MORE flag when FI_FENCE or REMOTE EP requirements are
	 * present. We will also only allow FI_MORE if a prior connection has
	 * been established, so that the peer capabilities can be determined.*/
	if ((flags & FI_FENCE) || (flags & FI_REMOTE_CQ_DATA) ||
	    !connected || (req->vc->peer_caps & FI_RMA_EVENT)) {
		flags &= ~FI_MORE;
	}

	/* Add reads/writes to slist when FI_MORE is present, Or
	 * if this is the first message in the chain without FI_MORE.
	 * When FI_MORE is not present, if the slists are not empty
	 * it is the first message without FI_MORE.
	 * Do not add reqs with FI_FENCE or REMOTE EP requirements requirements
	 * to the fi_more list. */
	if ((flags & FI_MORE) ||
	    (!(flags & FI_MORE) && connected &&
	    (!slist_empty(&ep->more_write) || !slist_empty(&ep->more_read)) &&
	     !(flags & FI_FENCE || flags & FI_REMOTE_CQ_DATA ||
	       req->vc->peer_caps & FI_RMA_EVENT))) {
		if (fr_type == GNIX_FAB_RQ_RDMA_WRITE) {
			slist_insert_tail(&req->rma.sle, &ep->more_write);
			req->work_fn = _gnix_rma_more_post_req;
		} else if (fr_type == GNIX_FAB_RQ_RDMA_READ) {
			slist_insert_tail(&req->rma.sle, &ep->more_read);
			req->work_fn = _gnix_rma_more_post_req;
		}

		if (flags & FI_MORE) {
			COND_RELEASE(ep->requires_lock, &ep->vc_lock);
			return FI_SUCCESS;
		}
	}

	/* Initiate read/write chains on first message without FI_MORE. */
	if (!(flags & FI_MORE) &&
	    (!(slist_empty(&ep->more_write)) ||
	     !(slist_empty(&ep->more_read)))) {
		if (!(slist_empty(&ep->more_write))) {
			sle = ep->more_write.head;
			more_req = container_of(sle, struct gnix_fab_req,
						rma.sle);
			GNIX_DEBUG(FI_LOG_EP_DATA,
				   "FI_MORE: got fab_request from more_write. Queuing Request\n");
			_gnix_vc_queue_tx_req(more_req);
			slist_init(&ep->more_write); /* For future reqs */
		}
		if (!(slist_empty(&ep->more_read))) {
			sle = ep->more_read.head;
			more_req = container_of(sle, struct gnix_fab_req,
						rma.sle);
			GNIX_DEBUG(FI_LOG_EP_DATA,
				   "FI_MORE: got fab_request from more_read. Queuing Request\n");
			_gnix_vc_queue_tx_req(more_req);
			slist_init(&ep->more_read);
		}

		/* Requests with FI_FENCE or REMOTE EP requirements are not
		 * added to the FI_MORE List. They must be queued separately. */
		if ((flags & FI_FENCE) || (flags & FI_REMOTE_CQ_DATA) ||
		    (req->vc->peer_caps & FI_RMA_EVENT)) {
			rc = _gnix_vc_queue_tx_req(req);
			COND_RELEASE(ep->requires_lock, &ep->vc_lock);
			return rc;
		}
		COND_RELEASE(ep->requires_lock, &ep->vc_lock);
		return FI_SUCCESS;
	}

	GNIX_DEBUG(FI_LOG_EP_DATA, "Queuing (%p %p %d)\n",
			(void *)loc_addr, (void *)rem_addr, len);

	rc = _gnix_vc_queue_tx_req(req);
	connected = (vc->conn_state == GNIX_VC_CONNECTED);

	COND_RELEASE(ep->requires_lock, &ep->vc_lock);

	/*
	 * If a new VC was allocated, progress CM before returning.
	 * If the VC is connected and there's a backlog, poke
	 * the nic progress engine befure returning.
	 */
	if (!connected) {
		_gnix_cm_nic_progress(ep->cm_nic);
	} else if (!dlist_empty(&vc->tx_queue)) {
		_gnix_nic_progress(vc->ep->nic);
	}

	return rc;

err_get_vc:
	COND_RELEASE(ep->requires_lock, &ep->vc_lock);
	if (flags & FI_LOCAL_MR) {
		fi_close(&auto_mr->fid);
		flags &= ~FI_LOCAL_MR;
	}
err_auto_reg:
	_gnix_fr_free(req->vc->ep, req);
	return rc;
}


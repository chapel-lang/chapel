/*
 * Copyright (c) 2022 Canon Medical Research USA, Inc.  All rights reserved.
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

#include <assert.h>
#include "ndspi.h"
#include "verbs_nd.h"

static enum ibv_wc_opcode vrb_ndrequest2opcode(enum ND2_REQUEST_TYPE rq)
{
	static enum ibv_wc_opcode opcodes[] = {
		[Nd2RequestTypeReceive] = IBV_WC_RECV,
		[Nd2RequestTypeSend] = IBV_WC_SEND,
		[Nd2RequestTypeRead] = IBV_WC_RDMA_READ,
		[Nd2RequestTypeWrite] = IBV_WC_RDMA_WRITE
	};
	return (rq < ARRAY_SIZE(opcodes)) ? opcodes[rq] : IBV_WC_SEND;
}

struct ibv_device **ibv_get_device_list(int *num_devices)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (num_devices) {
		assert(nd_infra.adapters.count <= INT_MAX);
		*num_devices = (int)nd_infra.adapters.count;
	}
	return nd_infra.adapters.device_list;
}

void ibv_free_device_list(struct ibv_device **list)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	// Since we are keeping state statically, there is nothing to free.
}

struct ibv_context *ibv_open_device(struct ibv_device *device)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	return nd_get_context_by_device(device);
}

int ibv_close_device(struct ibv_context *context)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	// Since we are keeping state statically, there is nothing to close.
	return 0;
}

// The IND2Adapter::Query method doesn't provide data for a number of fields
// in the ibv_device_attr structure. The following definitions were inferred
// from what ibv_query_device returned on a Linux implementation.
#define ND_MAX_CQ_CNT (1 << 24)
#define ND_MAX_QP_CNT (1 << 18)
#define ND_MAX_SRQ_CNT (1 << 23)
#define ND_MAX_MR_CNT (1 << 24)

int ibv_query_device(struct ibv_context *context,
		     struct ibv_device_attr *device_attr)
{
	struct nd_adapter *adapter;
	ND2_ADAPTER_INFO *nd_attr;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!context) || (!device_attr)) {
		errno = EINVAL;
		return errno;
	}

	adapter = nd_get_adapter_by_context(context);
	if (!adapter) {
		errno = ENODEV;
		return errno;
	}

	nd_attr = &adapter->info;
	memset(device_attr, 0, sizeof(struct ibv_device_attr));
	strcpy(device_attr->fw_ver, "(null)");
	device_attr->node_guid = nd_attr->AdapterId;
	device_attr->sys_image_guid = nd_attr->AdapterId;
	device_attr->max_mr_size = nd_attr->MaxRegistrationSize;
	device_attr->vendor_id = nd_attr->VendorId;
	device_attr->vendor_part_id = nd_attr->DeviceId;
	device_attr->max_qp = ND_MAX_QP_CNT;
	device_attr->max_qp_wr = nd_attr->MaxInitiatorQueueDepth;
	device_attr->max_sge = nd_attr->MaxInitiatorSge;
	device_attr->max_sge_rd = nd_attr->MaxReadSge;
	device_attr->max_cq = ND_MAX_CQ_CNT;
	device_attr->max_cqe = nd_attr->MaxCompletionQueueDepth;
	device_attr->max_mr = ND_MAX_MR_CNT;
	device_attr->max_qp_rd_atom = nd_attr->MaxInboundReadLimit;
	device_attr->max_qp_init_rd_atom = nd_attr->MaxOutboundReadLimit;
	device_attr->max_srq =
		nd_attr->MaxSharedReceiveQueueDepth ? ND_MAX_SRQ_CNT : 0;
	device_attr->max_srq_wr = nd_attr->MaxSharedReceiveQueueDepth;
	device_attr->phys_port_cnt = 1;

	return 0;
}

const char *ibv_get_device_name(struct ibv_device *device)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!device)
		return NULL;

	return device->name;
}

/*
 * async event methods are implemented for compatibility
 * but are not currently supported
*/

int ibv_get_async_event(struct ibv_context *context,
			struct ibv_async_event *event)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	/*
	* Normally we use ENOSYS for unsupported features
	* but ibv_get_async_event returns only -1 for any error.
	*/
	return -1;
}

void ibv_ack_async_event(struct ibv_async_event *event)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
}

const char *ibv_event_type_str(enum ibv_event_type event)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	return NULL;
}

// infiniband/verbs.h defines ibv_query_port to be ___ibv_query_port
int ___ibv_query_port(struct ibv_context *context, uint8_t port_num,
		      struct ibv_port_attr *port_attr)
{
	struct nd_adapter *adapter;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!context) || (!port_attr)) {
		errno = EINVAL;
		return errno;
	}

	adapter = nd_get_adapter_by_context(context);
	if (!adapter) {
		errno = ENODEV;
		return errno;
	}

	memset(port_attr, 0, sizeof(struct ibv_port_attr));
	port_attr->state = IBV_PORT_ACTIVE;
	port_attr->max_mtu = IBV_MTU_4096;
	port_attr->active_mtu = IBV_MTU_4096;
	port_attr->gid_tbl_len = 0;
	port_attr->max_msg_sz = adapter->info.MaxTransferLength;
	port_attr->pkey_tbl_len = 0;
	port_attr->lid = 0;
	port_attr->sm_sl = 0;
	port_attr->active_width = 2; // 4x
	port_attr->active_speed = 32; // 25 Gbps
	port_attr->phys_state =
		5; // IB_PORT_PHYS_STATE_LINKUP from "infiniband/iba/ib_types.h"
	port_attr->link_layer = IBV_LINK_LAYER_ETHERNET;

	return 0;
}

int ibv_query_gid(struct ibv_context *context, uint8_t port_num, int index,
		  union ibv_gid *gid)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");

	// little endian representation of link-local address prefix.
	gid->global.subnet_prefix = 0x80fe;
	gid->global.interface_id = 0;

	return 0;
}

int ibv_query_pkey(struct ibv_context *context, uint8_t port_num, int index,
		   __be16 *pkey)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");

	*pkey = 0xffff;

	return 0;
}

struct ibv_pd *ibv_alloc_pd(struct ibv_context *context)
{
	struct ibv_pd *pd;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!context) {
		errno = EINVAL;
		return NULL;
	}

	pd = calloc(1, sizeof(*pd));
	if (!pd) {
		errno = ENOMEM;
		return NULL;
	}

	pd->context = context;

	return pd;
}

int ibv_dealloc_pd(struct ibv_pd *pd)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!pd) {
		errno = EINVAL;
		return errno;
	}

	free(pd);
	return 0;
}

struct ibv_comp_channel *ibv_create_comp_channel(struct ibv_context *context)
{
	struct nd_comp_channel *channel_nd;
	int ret;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!context) {
		errno = EINVAL;
		return NULL;
	}

	channel_nd = calloc(1, sizeof(*channel_nd));
	if (!channel_nd) {
		errno = ENOMEM;
		return NULL;
	}

	ret = dlistfd_head_init(&channel_nd->q);
	if (ret) {
		free(channel_nd);
		errno = ret;
		return NULL;
	}
	ofi_mutex_init(&channel_nd->q_lock);

	channel_nd->channel.context = context;
	channel_nd->channel.fd = fd_signal_get(&channel_nd->q.signal);

	return &channel_nd->channel;
}

int ibv_destroy_comp_channel(struct ibv_comp_channel *channel)
{
	struct nd_comp_channel *channel_nd;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!channel) {
		errno = EINVAL;
		return errno;
	}

	if (channel->refcnt) {
		errno = EBUSY;
		return -1;
	}

	channel_nd = container_of(channel, struct nd_comp_channel, channel);

	dlistfd_head_free(&channel_nd->q);
	ofi_mutex_destroy(&channel_nd->q_lock);
	free(channel_nd);
	return 0;
}

struct ibv_cq *ibv_create_cq(struct ibv_context *context, int cqe,
			     void *cq_context, struct ibv_comp_channel *channel,
			     int comp_vector)
{
	struct nd_adapter *adapter;
	struct nd_cq *cq_nd;
	struct ibv_cq *cq;
	HRESULT hr;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!context) {
		errno = EINVAL;
		return NULL;
	}

	adapter = nd_get_adapter_by_context(context);
	if (!adapter) {
		errno = ENODEV;
		return NULL;
	}

	cq_nd = calloc(1, sizeof(*cq_nd));
	if (!cq_nd) {
		errno = ENOMEM;
		return NULL;
	}

	if (channel)
		++channel->refcnt;

	ofi_mutex_init(&cq_nd->notification.lock);
	pthread_cond_init(&cq_nd->notification.cond, 0);
	cq_nd->notification.event_cb = nd_cq_notify_event;
	cq_nd->notification.error_cb = nd_cq_notify_error;

	cq = &cq_nd->cq;
	cq->context = context;
	cq->channel = channel;
	cq->cq_context = cq_context;
	cq->cqe = cqe;

	hr = adapter->adapter->lpVtbl->CreateOverlappedFile(adapter->adapter,
							    &cq_nd->ov_file);
	VRB_DBG(FI_LOG_CQ, "IND2Adapter::CreateOverlappedFile: hr=0x%08lx\n",
		hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		goto err1;
	}

	assert(cq_nd->ov_file && cq_nd->ov_file != INVALID_HANDLE_VALUE);

	BindIoCompletionCallback(cq_nd->ov_file, nd_io_cb, 0);

	hr = adapter->adapter->lpVtbl->CreateCompletionQueue(
		adapter->adapter, &IID_IND2CompletionQueue, cq_nd->ov_file, cqe,
		0, 0, &cq_nd->nd2cq);
	VRB_DBG(FI_LOG_CQ, "IND2Adapter::CreateCompletionQueue: hr=0x%08lx\n",
		hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		goto err2;
	}
	assert(cq_nd->nd2cq);

	return cq;
err2:
	CloseHandle(cq_nd->ov_file);
err1:
	free(cq_nd);
	return NULL;
}

int ibv_destroy_cq(struct ibv_cq *cq)
{
	struct nd_cq *cq_nd;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!cq) {
		errno = EINVAL;
		return errno;
	}

	cq_nd = container_of(cq, struct nd_cq, cq);
	if (nd_cancel_pending(&cq_nd->notification,
			      (IND2Overlapped *)cq_nd->nd2cq)) {
		return errno;
	}

	if (!CloseHandle(cq_nd->ov_file)) {
		errno = GetLastError();
		return errno;
	}

	// There appears to be a bug related to the reference counting for
	// ComplettionQueue objects in the Mellanox ND driver. CompletionQueue
	// reference counts are incremented when they are used in the creation
	// of a QueuePair, but are not always decremented when the QueuePair is
	// Released.  Ideally, we'd assert that that the reference count is zero
	// here, but we cannot because of that bug.
	cq_nd->nd2cq->lpVtbl->Release(cq_nd->nd2cq);

	if (cq_nd->cq.channel)
		--cq_nd->cq.channel->refcnt;

	pthread_cond_destroy(&cq_nd->notification.cond);
	ofi_mutex_destroy(&cq_nd->notification.lock);
	free(cq_nd);

	return 0;
}

int ibv_poll_cq(struct ibv_cq *cq, int num_entries, struct ibv_wc *wc)
{
	struct nd_cq *cq_nd;
	int num_results;
	ND2_RESULT result;
	ULONG nResults;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!cq || !wc) {
		errno = EINVAL;
		return -1;
	}

	cq_nd = container_of(cq, struct nd_cq, cq);
	for (num_results = 0; num_results < num_entries; ++num_results) {
		nResults = cq_nd->nd2cq->lpVtbl->GetResults(cq_nd->nd2cq,
							    &result, 1);

		if (nResults == 0)
			break;

		memset(&wc[num_results], 0, sizeof(wc[num_results]));
		wc[num_results].wr_id = (uint64_t)result.RequestContext;
		wc[num_results].byte_len = result.BytesTransferred;
		wc[num_results].status = result.Status;
		wc[num_results].opcode = vrb_ndrequest2opcode(result.RequestType);
		FI_LOG(&vrb_prov, result.Status ? FI_LOG_WARN : FI_LOG_DEBUG,
		       FI_LOG_CQ,
		       "ibv_poll_cq: context=0x%016llx, numBytes=%d, "
		       "status=0x%08lx, request=0x%08lx\n",
		       (uint64_t)result.RequestContext, result.BytesTransferred,
		       result.Status, result.RequestType);
	}

	return num_results;
}

int ibv_req_notify_cq(struct ibv_cq *cq, int solicited_only)
{
	struct nd_cq *cq_nd;
	HRESULT hr;
	int ret = 0;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!cq) {
		errno = EINVAL;
		return errno;
	}

	cq_nd = container_of(cq, struct nd_cq, cq);

	ofi_mutex_lock(&cq_nd->notification.lock);
	if (!cq_nd->notification.cb_pending) {
		cq_nd->notification.cb_pending = 1;
		hr = cq_nd->nd2cq->lpVtbl->Notify(
			cq_nd->nd2cq,
			solicited_only ? ND_CQ_NOTIFY_SOLICITED :
					       ND_CQ_NOTIFY_ANY,
			&cq_nd->notification.ov);
		VRB_DBG(FI_LOG_CQ,
			"IND2CompletionQueue::Notify: hr=0x%08lx; ov=%p\n", hr,
			&cq_nd->notification.ov);
		if (FAILED(hr)) {
			cq_nd->notification.cb_pending = 0;
			errno = hresult2fi(hr);
			ret = errno;
		}
	}
	ofi_mutex_unlock(&cq_nd->notification.lock);

	return ret;
}

int ibv_get_cq_event(struct ibv_comp_channel *channel, struct ibv_cq **cq,
		     void **cq_context)
{
	int ret = 0;
	struct nd_comp_channel *channel_nd;
	struct nd_cq *cq_nd = NULL;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!channel) || (!cq) || (!cq_context)) {
		errno = EINVAL;
		return -1;
	}

	channel_nd = container_of(channel, struct nd_comp_channel, channel);

	ofi_mutex_lock(&channel_nd->q_lock);
	if (dlistfd_empty(&channel_nd->q)) {
		ret = -1;
	} else {
		cq_nd = container_of(channel_nd->q.list.next, struct nd_cq,
				     entry);
		*cq = &cq_nd->cq;
		*cq_context = cq_nd->cq.cq_context;
		dlistfd_remove(channel_nd->q.list.next, &channel_nd->q);
	}
	ofi_mutex_unlock(&channel_nd->q_lock);

	return ret;
}

void ibv_ack_cq_events(struct ibv_cq *cq, unsigned int nevents)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	// N/A
}

struct ibv_qp *ibv_create_qp(struct ibv_pd *pd,
			     struct ibv_qp_init_attr *qp_init_attr)
{
	static uint32_t unique_qp_num = 2; // QPN 0 & 1 are reserved by IB spec.
	struct nd_adapter *adapter;
	struct nd_qp *qp_nd;
	struct ibv_qp *qp;
	HRESULT hr;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!pd) || (!qp_init_attr)) {
		errno = EINVAL;
		return NULL;
	}

	adapter = nd_get_adapter_by_context(pd->context);
	if (qp_init_attr->cap.max_inline_data >
	    adapter->info.MaxInlineDataSize) {
		errno = ENODEV;
		return NULL;
	}

	qp_nd = calloc(1, sizeof(*qp_nd));
	if (!qp_nd) {
		errno = ENOMEM;
		return NULL;
	}

	qp = &qp_nd->qp;
	qp->context = pd->context;
	qp->qp_context = qp_init_attr->qp_context;
	qp->pd = pd;
	qp->send_cq = qp_init_attr->send_cq;
	qp->recv_cq = qp_init_attr->recv_cq;
	qp->srq = qp_init_attr->srq;
	qp->qp_num = unique_qp_num++;
	qp->state = IBV_QPS_RESET;
	qp->qp_type = qp_init_attr->qp_type;
	qp_nd->max_inline_data = qp_init_attr->cap.max_inline_data;

	hr = adapter->adapter->lpVtbl->CreateQueuePair(
		adapter->adapter, &IID_IND2QueuePair,
		(IUnknown *)((struct nd_cq *)qp_init_attr->recv_cq)->nd2cq,
		(IUnknown *)((struct nd_cq *)qp_init_attr->send_cq)->nd2cq,
		qp_init_attr->qp_context, qp_init_attr->cap.max_recv_wr,
		qp_init_attr->cap.max_send_wr, qp_init_attr->cap.max_recv_sge,
		qp_init_attr->cap.max_send_sge,
		qp_init_attr->cap.max_inline_data, (void **)&qp_nd->nd2qp);
	VRB_DBG(FI_LOG_EP_CTRL, "IND2Adapter::CreateQueuePair: hr=0x%08lx\n",
		hr);
	if (FAILED(hr)) {
		free(qp_nd);
		errno = hresult2fi(hr);
		return NULL;
	}

	return qp;
}

int ibv_destroy_qp(struct ibv_qp *qp)
{
	struct nd_qp *qp_nd;
	ULONG refcnt;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!qp) {
		errno = EINVAL;
		return errno;
	}

	qp_nd = container_of(qp, struct nd_qp, qp);
	refcnt = qp_nd->nd2qp->lpVtbl->Release(qp_nd->nd2qp);
	assert(!refcnt);
	free(qp_nd);
	return 0;
}

int ibv_modify_qp(struct ibv_qp *qp, struct ibv_qp_attr *attr, int attr_mask)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	return 0;
}

int ibv_query_qp(struct ibv_qp *qp, struct ibv_qp_attr *attr, int attr_mask,
		 struct ibv_qp_init_attr *init_attr)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	return 0;
}

// infiniband/verbs.h defines ibv_reg_mr as a macro to call ibv_reg_mr_iova2.
struct ibv_mr *ibv_reg_mr_iova2(struct ibv_pd *pd, void *addr, size_t length,
				uint64_t iova, unsigned int access)
{
	struct nd_adapter *adapter;
	struct nd_mr *mr_nd;
	struct ibv_mr *mr;
	OVERLAPPED ov = { 0 };
	HRESULT hr;
	int ret;
	int flags = 0;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!pd) || (!addr)) {
		errno = EINVAL;
		return NULL;
	}

	adapter = nd_get_adapter_by_context(pd->context);
	if (!adapter) {
		errno = ENODEV;
		return NULL;
	}

	mr_nd = calloc(1, sizeof(*mr_nd));
	if (!mr_nd) {
		errno = ENOMEM;
		goto err1;
	}

	hr = adapter->adapter->lpVtbl->CreateOverlappedFile(adapter->adapter,
							    &mr_nd->ov_file);
	VRB_DBG(FI_LOG_MR, "IND2Adapter::CreateOverlappedFile: hr=0x%08lx\n",
		hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		goto err2;
	}

	assert(mr_nd->ov_file && mr_nd->ov_file != INVALID_HANDLE_VALUE);

	hr = adapter->adapter->lpVtbl->CreateMemoryRegion(adapter->adapter,
							  &IID_IND2MemoryRegion,
							  mr_nd->ov_file,
							  &mr_nd->region);
	VRB_DBG(FI_LOG_MR, "IND2Adapter::CreateMemoryRegion: hr=0x%08lx\n", hr);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		goto err3;
	}

	mr = &mr_nd->mr;
	mr->context = pd->context;
	mr->pd = pd;
	mr->addr = addr;
	mr->length = length;

	if (access & IBV_ACCESS_LOCAL_WRITE)
		flags |= ND_MR_FLAG_ALLOW_LOCAL_WRITE;
	if (access & IBV_ACCESS_REMOTE_READ)
		flags |= ND_MR_FLAG_ALLOW_REMOTE_READ;
	if (access & IBV_ACCESS_REMOTE_WRITE)
		flags |= ND_MR_FLAG_ALLOW_REMOTE_WRITE;

	hr = mr_nd->region->lpVtbl->Register(mr_nd->region, addr, length, flags,
					     &ov);
	VRB_DBG(FI_LOG_MR,
		"IND2MemoryRegion::Register: hr=0x%08lx; ov=%p, "
		"addr=0x%016llx, len=0x%016llx\n",
		hr, &ov, addr, length);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		goto err4;
	}
	hr = mr_nd->region->lpVtbl->GetOverlappedResult(mr_nd->region, &ov,
							TRUE);
	VRB_DBG(FI_LOG_MR,
		"IND2MemoryRegion::GetOverlappedResult: hr=0x%08lx; ov=%p\n",
		hr, &ov);
	if (FAILED(hr)) {
		errno = hresult2fi(hr);
		goto err5;
	}

	mr->lkey = mr_nd->region->lpVtbl->GetLocalToken(mr_nd->region);
	mr->rkey = mr_nd->region->lpVtbl->GetRemoteToken(mr_nd->region);

	return mr;
err5:
	mr_nd->region->lpVtbl->CancelOverlappedRequests(mr_nd->region);
err4:
	ret = mr_nd->region->lpVtbl->Release(mr_nd->region);
	assert(!ret);
err3:
	ret = CloseHandle(mr_nd->ov_file);
	assert(ret);
err2:
	free(mr_nd);
err1:
	return NULL;
}

int ibv_dereg_mr(struct ibv_mr *mr)
{
	OVERLAPPED ov = { 0 };
	int ret = 0;
	struct nd_mr *mr_nd;
	HRESULT hr;
	int tmp;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if (!mr) {
		errno = EINVAL;
		return errno;
	}

	mr_nd = container_of(mr, struct nd_mr, mr);
	hr = mr_nd->region->lpVtbl->Deregister(mr_nd->region, &ov);
	VRB_DBG(FI_LOG_MR, "IND2MemoryRegion::Deregister: hr=0x%08lx; ov=%p\n",
		hr, &ov);
	if (FAILED(hr)) {
		ret = hresult2fi(hr);
		goto err1;
	}
	hr = mr_nd->region->lpVtbl->GetOverlappedResult(mr_nd->region, &ov,
							TRUE);
	VRB_DBG(FI_LOG_MR,
		"IND2MemoryRegion::GetOverlappedResult: hr=0x%08lx; ov=%p\n",
		hr, &ov);
	if (FAILED(hr)) {
		ret = hresult2fi(hr);
		mr_nd->region->lpVtbl->CancelOverlappedRequests(mr_nd->region);
	}
err1:
	tmp = CloseHandle(mr_nd->ov_file);
	assert(tmp);
	tmp = mr_nd->region->lpVtbl->Release(mr_nd->region);
	assert(!tmp);
	free(mr_nd);
	if (ret)
		errno = ret;
	return ret;
}

int ibv_post_recv(struct ibv_qp *qp, struct ibv_recv_wr *wr,
		  struct ibv_recv_wr **bad_wr)
{
	struct nd_qp *qp_nd;
	HRESULT hr;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!qp) || (!wr) || (!bad_wr)) {
		errno = EINVAL;
		return errno;
	}

	*bad_wr = NULL;

	qp_nd = container_of(qp, struct nd_qp, qp);
	while (wr) {
		hr = qp_nd->nd2qp->lpVtbl->Receive(qp_nd->nd2qp,
						   (void *)wr->wr_id,
						   (ND2_SGE *)wr->sg_list,
						   wr->num_sge);
		VRB_DBG(FI_LOG_EP_DATA,
			"IND2QueuePair::Receive: hr=0x%08lx, "
			"context=0x%016llx\n",
			hr, wr->wr_id);
		if (FAILED(hr)) {
			errno = hresult2fi(hr);
			*bad_wr = wr;
			return errno;
		}

		wr = wr->next;
	}

	return 0;
}

int ibv_post_send(struct ibv_qp *qp, struct ibv_send_wr *wr,
		  struct ibv_send_wr **bad_wr)
{
	struct nd_qp *qp_nd;
	uint32_t flags = 0;
	HRESULT hr;

	VRB_TRACE(FI_LOG_FABRIC, "\n");

	if ((!qp) || (!wr) || (!bad_wr)) {
		errno = EINVAL;
		return errno;
	}

	*bad_wr = NULL;

	qp_nd = container_of(qp, struct nd_qp, qp);
	while (wr) {
		switch (wr->opcode) {
		case IBV_WR_SEND:
			flags = (wr->send_flags & IBV_SEND_INLINE) ?
					      ND_OP_FLAG_INLINE :
					      0;
			hr = qp_nd->nd2qp->lpVtbl->Send(qp_nd->nd2qp,
							(void *)wr->wr_id,
							(ND2_SGE *)wr->sg_list,
							wr->num_sge, flags);
			VRB_DBG(FI_LOG_EP_DATA,
				"IND2QueuePair::Send: hr=0x%08lx, nsge=%d, "
				"addr=0x%016llx, length=%d, lkey=0x%08lx\n",
				hr, wr->num_sge, wr->sg_list[0].addr,
				wr->sg_list[0].length, wr->sg_list[0].lkey);
			break;

		case IBV_WR_RDMA_READ:
			hr = qp_nd->nd2qp->lpVtbl->Read(
				qp_nd->nd2qp, (void *)wr->wr_id,
				(ND2_SGE *)wr->sg_list, wr->num_sge,
				wr->wr.rdma.remote_addr, wr->wr.rdma.rkey,
				flags);
			VRB_DBG(FI_LOG_EP_DATA,
				"IND2QueuePair::Read: hr=0x%08lx, nsge=%d, "
				"addr=0x%016llx, raddr=0x%016llx, "
				"rkey=0x%08lx, lkey=0x%08lx\n",
				hr, wr->num_sge, wr->sg_list[0].addr,
				wr->wr.rdma.remote_addr, wr->wr.rdma.rkey,
				wr->sg_list[0].lkey);
			break;

		case IBV_WR_RDMA_WRITE:
			flags = (wr->send_flags & IBV_SEND_INLINE) ?
					      ND_OP_FLAG_INLINE :
					      0;
			hr = qp_nd->nd2qp->lpVtbl->Write(
				qp_nd->nd2qp, (void *)wr->wr_id,
				(ND2_SGE *)wr->sg_list, wr->num_sge,
				wr->wr.rdma.remote_addr, wr->wr.rdma.rkey,
				flags);
			VRB_DBG(FI_LOG_EP_DATA,
				"IND2QueuePair::Write: hr=0x%08lx, nsge=%d, "
				"addr=0x%016llx, raddr=0x%016llx, "
				"rkey=0x%08lx, lkey=0x%08lx\n",
				hr, wr->num_sge, wr->sg_list[0].addr,
				wr->wr.rdma.remote_addr, wr->wr.rdma.rkey,
				wr->sg_list[0].lkey);
			break;

		default:
			// TODO: Add support for other transfer methods.
			VRB_WARN(FI_LOG_EP_DATA, "Unsupported opcode: %d\n",
				 wr->opcode);
			assert(false);
			errno = EINVAL;
			return errno;
		}

		if (FAILED(hr)) {
			errno = hresult2fi(hr);
			*bad_wr = wr;
			return errno;
		}

		wr = wr->next;
	}

	return 0;
}

const char *ibv_wc_status_str(enum ibv_wc_status status)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");

	static const char *const wc_status_str[] = {
		[IBV_WC_SUCCESS] = "success",
		[IBV_WC_LOC_LEN_ERR] = "local length error",
		[IBV_WC_LOC_QP_OP_ERR] = "local QP operation error",
		[IBV_WC_LOC_EEC_OP_ERR] = "local EE context operation error",
		[IBV_WC_LOC_PROT_ERR] = "local protection error",
		[IBV_WC_WR_FLUSH_ERR] = "Work Request Flushed Error",
		[IBV_WC_MW_BIND_ERR] = "memory management operation error",
		[IBV_WC_BAD_RESP_ERR] = "bad response error",
		[IBV_WC_LOC_ACCESS_ERR] = "local access error",
		[IBV_WC_REM_INV_REQ_ERR] = "remote invalid request error",
		[IBV_WC_REM_ACCESS_ERR] = "remote access error",
		[IBV_WC_REM_OP_ERR] = "remote operation error",
		[IBV_WC_RETRY_EXC_ERR] = "transport retry counter exceeded",
		[IBV_WC_RNR_RETRY_EXC_ERR] = "RNR retry counter exceeded",
		[IBV_WC_LOC_RDD_VIOL_ERR] = "local RDD violation error",
		[IBV_WC_REM_INV_RD_REQ_ERR] = "remote invalid RD request",
		[IBV_WC_REM_ABORT_ERR] = "aborted error",
		[IBV_WC_INV_EECN_ERR] = "invalid EE context number",
		[IBV_WC_INV_EEC_STATE_ERR] = "invalid EE context state",
		[IBV_WC_FATAL_ERR] = "fatal error",
		[IBV_WC_RESP_TIMEOUT_ERR] = "response timeout error",
		[IBV_WC_GENERAL_ERR] = "general error",
		[IBV_WC_TM_ERR] = "TM error",
		[IBV_WC_TM_RNDV_INCOMPLETE] = "TM software rendezvous",
	};

	if (status < IBV_WC_SUCCESS || status > IBV_WC_TM_RNDV_INCOMPLETE)
		return "unknown";

	return wc_status_str[status];
}

struct ibv_srq *ibv_create_srq(struct ibv_pd *pd,
			       struct ibv_srq_init_attr *srq_init_attr)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	errno = ENOSYS;
	return NULL;
}

int ibv_destroy_srq(struct ibv_srq *srq)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	return ENOSYS;
}

struct ibv_ah *ibv_create_ah(struct ibv_pd *pd, struct ibv_ah_attr *attr)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	errno = ENOSYS;
	return NULL;
}

int ibv_destroy_ah(struct ibv_ah *ah)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	return ENOSYS;
}

int ibv_post_srq_recv(struct ibv_srq *srq, struct ibv_recv_wr *recv_wr,
		      struct ibv_recv_wr **bad_recv_wr)
{
	VRB_TRACE(FI_LOG_FABRIC, "\n");
	return ENOSYS;
}

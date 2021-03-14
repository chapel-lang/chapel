/*
 * Copyright (c) 2016 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates. All rights reserved.
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
#include <poll.h>

#include <ofi.h>
#include <ofi_util.h>
#include "rxm.h"

static struct rxm_cmap_handle *rxm_conn_alloc(struct rxm_cmap *cmap);
static int rxm_conn_connect(struct rxm_ep *ep,
			    struct rxm_cmap_handle *handle, const void *addr);
static int rxm_conn_signal(struct rxm_ep *ep, void *context,
			   enum rxm_cmap_signal signal);
static void rxm_conn_av_updated_handler(struct rxm_cmap_handle *handle);
static void *rxm_conn_progress(void *arg);
static void *rxm_conn_atomic_progress(void *arg);
static int rxm_conn_handle_event(struct rxm_ep *rxm_ep,
				 struct rxm_msg_eq_entry *entry);


/*
 * Connection map
 */

char *rxm_cm_state_str[] = {
	RXM_CM_STATES(OFI_STR)
};

static inline ssize_t rxm_eq_readerr(struct rxm_ep *rxm_ep,
				     struct rxm_msg_eq_entry *entry)
{
	ssize_t ret;

	/* reset previous err data info */
	entry->err_entry.err_data_size = 0;

	ret = fi_eq_readerr(rxm_ep->msg_eq, &entry->err_entry, 0);
	if (ret != sizeof(entry->err_entry)) {
		if (ret != -FI_EAGAIN)
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"unable to fi_eq_readerr: %zd\n", ret);
		return ret < 0 ? ret : -FI_EINVAL;
	}

	if (entry->err_entry.err == ECONNREFUSED) {
		entry->context = entry->err_entry.fid->context;
		return -FI_ECONNREFUSED;
	}

	OFI_EQ_STRERROR(&rxm_prov, FI_LOG_WARN, FI_LOG_EP_CTRL,
			rxm_ep->msg_eq, &entry->err_entry);
	return -entry->err_entry.err;
}

static ssize_t rxm_eq_read(struct rxm_ep *ep, size_t len,
			   struct rxm_msg_eq_entry *entry)
{
	ssize_t ret;

	ret = fi_eq_read(ep->msg_eq, &entry->event, &entry->cm_entry, len, 0);
	if (ret == -FI_EAVAIL)
		ret = rxm_eq_readerr(ep, entry);

	return ret;
}

static void rxm_cmap_set_key(struct rxm_cmap_handle *handle)
{
	handle->key = ofi_idx2key(&handle->cmap->key_idx,
		ofi_idx_insert(&handle->cmap->handles_idx, handle));
}

static void rxm_cmap_clear_key(struct rxm_cmap_handle *handle)
{
	int index = ofi_key2idx(&handle->cmap->key_idx, handle->key);

	if (!ofi_idx_is_valid(&handle->cmap->handles_idx, index))
		FI_WARN(handle->cmap->av->prov, FI_LOG_AV, "Invalid key!\n");
	else
		ofi_idx_remove(&handle->cmap->handles_idx, index);
}

struct rxm_cmap_handle *rxm_cmap_key2handle(struct rxm_cmap *cmap, uint64_t key)
{
	struct rxm_cmap_handle *handle;

	if (!(handle = ofi_idx_lookup(&cmap->handles_idx,
				      ofi_key2idx(&cmap->key_idx, key)))) {
		FI_WARN(cmap->av->prov, FI_LOG_AV, "Invalid key!\n");
	} else {
		if (handle->key != key) {
			FI_WARN(cmap->av->prov, FI_LOG_AV,
				"handle->key not matching given key\n");
			handle = NULL;
		}
	}
	return handle;
}

static void rxm_cmap_init_handle(struct rxm_cmap_handle *handle,
				  struct rxm_cmap *cmap,
				  enum rxm_cmap_state state,
				  fi_addr_t fi_addr,
				  struct rxm_cmap_peer *peer)
{
	handle->cmap = cmap;
	RXM_CM_UPDATE_STATE(handle, state);
	rxm_cmap_set_key(handle);
	handle->fi_addr = fi_addr;
	handle->peer = peer;
}

static int rxm_cmap_match_peer(struct dlist_entry *entry, const void *addr)
{
	struct rxm_cmap_peer *peer;

	peer = container_of(entry, struct rxm_cmap_peer, entry);
	return !memcmp(peer->addr, addr, peer->handle->cmap->av->addrlen);
}

static int rxm_cmap_del_handle(struct rxm_cmap_handle *handle)
{
	struct rxm_cmap *cmap = handle->cmap;
	int ret;

	FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
	       "marking connection handle: %p for deletion\n", handle);
	rxm_cmap_clear_key(handle);

	RXM_CM_UPDATE_STATE(handle, RXM_CMAP_SHUTDOWN);

	/* Signal CM thread to delete the handle. This is required
	 * so that the CM thread handles any pending events for this
	 * ep correctly. Handle would be freed finally after processing the
	 * events */
	ret = rxm_conn_signal(cmap->ep, handle, RXM_CMAP_FREE);
	if (ret) {
		FI_WARN(cmap->av->prov, FI_LOG_EP_CTRL,
			"Unable to signal CM thread\n");
		return ret;
	}
	return 0;
}

static inline int
rxm_cmap_check_and_realloc_handles_table(struct rxm_cmap *cmap,
					 fi_addr_t fi_addr)
{
	void *new_handles;
	size_t grow_size;

	if (OFI_LIKELY(fi_addr < cmap->num_allocated))
		return 0;

	grow_size = MAX(cmap->av->count, fi_addr - cmap->num_allocated + 1);

	new_handles = realloc(cmap->handles_av,
			      (grow_size + cmap->num_allocated) *
			      sizeof(*cmap->handles_av));
	if (OFI_LIKELY(!new_handles))
		return -FI_ENOMEM;

	cmap->handles_av = new_handles;
	memset(&cmap->handles_av[cmap->num_allocated], 0,
	       sizeof(*cmap->handles_av) * grow_size);
	cmap->num_allocated += grow_size;
	return 0;
}

static struct rxm_pkt *
rxm_conn_inject_pkt_alloc(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn,
			  uint8_t op, uint64_t flags)
{
	struct rxm_pkt *inject_pkt;
	int ret = ofi_memalign((void **) &inject_pkt, 16,
			       rxm_ep->inject_limit + sizeof(*inject_pkt));

	if (ret)
		return NULL;

	memset(inject_pkt, 0, rxm_ep->inject_limit + sizeof(*inject_pkt));
	inject_pkt->ctrl_hdr.version = RXM_CTRL_VERSION;
	inject_pkt->ctrl_hdr.type = rxm_ctrl_eager;
	inject_pkt->hdr.version = OFI_OP_VERSION;
	inject_pkt->hdr.op = op;
	inject_pkt->hdr.flags = flags;

	return inject_pkt;
}

static void rxm_conn_res_free(struct rxm_conn *rxm_conn)
{
	ofi_freealign(rxm_conn->inject_pkt);
	rxm_conn->inject_pkt = NULL;
	ofi_freealign(rxm_conn->inject_data_pkt);
	rxm_conn->inject_data_pkt = NULL;
	ofi_freealign(rxm_conn->tinject_pkt);
	rxm_conn->tinject_pkt = NULL;
	ofi_freealign(rxm_conn->tinject_data_pkt);
	rxm_conn->tinject_data_pkt = NULL;
}

static int rxm_conn_res_alloc(struct rxm_ep *rxm_ep, struct rxm_conn *rxm_conn)
{
	dlist_init(&rxm_conn->deferred_conn_entry);
	dlist_init(&rxm_conn->deferred_tx_queue);
	dlist_init(&rxm_conn->sar_rx_msg_list);
	dlist_init(&rxm_conn->sar_deferred_rx_msg_list);

	if (rxm_ep->util_ep.domain->threading != FI_THREAD_SAFE) {
		rxm_conn->inject_pkt =
			rxm_conn_inject_pkt_alloc(rxm_ep, rxm_conn,
						  ofi_op_msg, 0);
		rxm_conn->inject_data_pkt =
			rxm_conn_inject_pkt_alloc(rxm_ep, rxm_conn,
						  ofi_op_msg, FI_REMOTE_CQ_DATA);
		rxm_conn->tinject_pkt =
			rxm_conn_inject_pkt_alloc(rxm_ep, rxm_conn,
						  ofi_op_tagged, 0);
		rxm_conn->tinject_data_pkt =
			rxm_conn_inject_pkt_alloc(rxm_ep, rxm_conn,
						  ofi_op_tagged, FI_REMOTE_CQ_DATA);

		if (!rxm_conn->inject_pkt || !rxm_conn->inject_data_pkt ||
		    !rxm_conn->tinject_pkt || !rxm_conn->tinject_data_pkt) {
			rxm_conn_res_free(rxm_conn);
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "unable to allocate "
				"inject pkt for connection\n");
			return -FI_ENOMEM;
		}
	}
	return 0;
}

static void rxm_conn_close(struct rxm_cmap_handle *handle)
{
	struct rxm_conn *rxm_conn = container_of(handle, struct rxm_conn, handle);

	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "closing msg ep\n");
	if (!rxm_conn->msg_ep)
		return;

	if (fi_close(&rxm_conn->msg_ep->fid))
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "unable to close msg_ep\n");

	rxm_conn->msg_ep = NULL;
}

static void rxm_conn_free(struct rxm_cmap_handle *handle)
{
	struct rxm_conn *rxm_conn = container_of(handle, struct rxm_conn, handle);

	rxm_conn_close(handle);
	rxm_conn_res_free(rxm_conn);
	free(rxm_conn);
}

static int rxm_cmap_alloc_handle(struct rxm_cmap *cmap, fi_addr_t fi_addr,
				 enum rxm_cmap_state state,
				 struct rxm_cmap_handle **handle)
{
	int ret;

	*handle = rxm_conn_alloc(cmap);
	if (OFI_UNLIKELY(!*handle))
		return -FI_ENOMEM;
	FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
	       "Allocated handle: %p for fi_addr: %" PRIu64 "\n",
	       *handle, fi_addr);
	ret = rxm_cmap_check_and_realloc_handles_table(cmap, fi_addr);
	if (OFI_UNLIKELY(ret)) {
		rxm_conn_free(*handle);
		return ret;
	}
	rxm_cmap_init_handle(*handle, cmap, state, fi_addr, NULL);
	cmap->handles_av[fi_addr] = *handle;
	return 0;
}

static int rxm_cmap_alloc_handle_peer(struct rxm_cmap *cmap, void *addr,
				       enum rxm_cmap_state state,
				       struct rxm_cmap_handle **handle)
{
	struct rxm_cmap_peer *peer;

	peer = calloc(1, sizeof(*peer) + cmap->av->addrlen);
	if (!peer)
		return -FI_ENOMEM;
	*handle = rxm_conn_alloc(cmap);
	if (!*handle) {
		free(peer);
		return -FI_ENOMEM;
	}
	ofi_straddr_dbg(cmap->av->prov, FI_LOG_AV, "Allocated handle for addr",
			addr);
	FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL, "handle: %p\n", *handle);
	rxm_cmap_init_handle(*handle, cmap, state, FI_ADDR_NOTAVAIL, peer);
	FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL, "Adding handle to peer list\n");
	peer->handle = *handle;
	memcpy(peer->addr, addr, cmap->av->addrlen);
	dlist_insert_tail(&peer->entry, &cmap->peer_list);
	return 0;
}

static struct rxm_cmap_handle *
rxm_cmap_get_handle_peer(struct rxm_cmap *cmap, const void *addr)
{
	struct rxm_cmap_peer *peer;
	struct dlist_entry *entry;

	entry = dlist_find_first_match(&cmap->peer_list, rxm_cmap_match_peer,
				       addr);
	if (!entry)
		return NULL;
	ofi_straddr_dbg(cmap->av->prov, FI_LOG_AV,
			"handle found in peer list for addr", addr);
	peer = container_of(entry, struct rxm_cmap_peer, entry);
	return peer->handle;
}

int rxm_cmap_remove(struct rxm_cmap *cmap, int index)
{
	struct rxm_cmap_handle *handle;
	int ret = -FI_ENOENT;

	handle = cmap->handles_av[index];
	if (!handle) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "cmap entry not found\n");
		return ret;
	}

	handle->peer = calloc(1, sizeof(*handle->peer) + cmap->av->addrlen);
	if (!handle->peer) {
		ret = -FI_ENOMEM;
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "unable to allocate memory "
			"for moving handle to peer list, deleting it instead\n");
		rxm_cmap_del_handle(handle);
		return ret;
	}
	handle->fi_addr = FI_ADDR_NOTAVAIL;
	cmap->handles_av[index] = NULL;
	handle->peer->handle = handle;
	memcpy(handle->peer->addr, ofi_av_get_addr(cmap->av, index),
	       cmap->av->addrlen);
	dlist_insert_tail(&handle->peer->entry, &cmap->peer_list);
	return 0;
}

static int rxm_cmap_move_handle(struct rxm_cmap_handle *handle,
				fi_addr_t fi_addr)
{
	int ret;

	dlist_remove(&handle->peer->entry);
	free(handle->peer);
	handle->peer = NULL;
	handle->fi_addr = fi_addr;
	ret = rxm_cmap_check_and_realloc_handles_table(handle->cmap, fi_addr);
	if (OFI_UNLIKELY(ret))
		return ret;
	handle->cmap->handles_av[fi_addr] = handle;
	return 0;
}

int rxm_cmap_update(struct rxm_cmap *cmap, const void *addr, fi_addr_t fi_addr)
{
	struct rxm_cmap_handle *handle;
	int ret;

	/* Check whether we have already allocated a handle for this `fi_addr`. */
	/* We rely on the fact that `ofi_ip_av_insert`/`ofi_av_insert_addr` returns
	 * the same `fi_addr` for the equal addresses */
	if (fi_addr < cmap->num_allocated) {
		handle = rxm_cmap_acquire_handle(cmap, fi_addr);
		if (handle)
			return 0;
	}

	handle = rxm_cmap_get_handle_peer(cmap, addr);
	if (!handle) {
		ret = rxm_cmap_alloc_handle(cmap, fi_addr,
					    RXM_CMAP_IDLE, &handle);
		return ret;
	}
	ret = rxm_cmap_move_handle(handle, fi_addr);
	if (ret)
		return ret;

	rxm_conn_av_updated_handler(handle);
	return 0;
}

void rxm_cmap_process_shutdown(struct rxm_cmap *cmap,
			       struct rxm_cmap_handle *handle)
{
	FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
		"Processing shutdown for handle: %p\n", handle);
	if (handle->state > RXM_CMAP_SHUTDOWN) {
		FI_WARN(cmap->av->prov, FI_LOG_EP_CTRL,
			"Invalid handle on shutdown event\n");
	} else if (handle->state != RXM_CMAP_SHUTDOWN) {
		FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL, "Got remote shutdown\n");
		rxm_cmap_del_handle(handle);
	} else {
		FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL, "Got local shutdown\n");
	}
}

void rxm_cmap_process_connect(struct rxm_cmap *cmap,
			      struct rxm_cmap_handle *handle,
			      union rxm_cm_data *cm_data)
{
	struct rxm_conn *rxm_conn = container_of(handle, struct rxm_conn, handle);

	FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
	       "processing FI_CONNECTED event for handle: %p\n", handle);
	if (cm_data) {
		assert(handle->state == RXM_CMAP_CONNREQ_SENT);
		handle->remote_key = cm_data->accept.server_conn_id;
		rxm_conn->rndv_tx_credits = cm_data->accept.rx_size;
	} else {
		assert(handle->state == RXM_CMAP_CONNREQ_RECV);
	}
	RXM_CM_UPDATE_STATE(handle, RXM_CMAP_CONNECTED);

	/* Set the remote key to the inject packets */
	if (cmap->ep->util_ep.domain->threading != FI_THREAD_SAFE) {
		rxm_conn->inject_pkt->ctrl_hdr.conn_id = rxm_conn->handle.remote_key;
		rxm_conn->inject_data_pkt->ctrl_hdr.conn_id = rxm_conn->handle.remote_key;
		rxm_conn->tinject_pkt->ctrl_hdr.conn_id = rxm_conn->handle.remote_key;
		rxm_conn->tinject_data_pkt->ctrl_hdr.conn_id = rxm_conn->handle.remote_key;
	}
}

void rxm_cmap_process_reject(struct rxm_cmap *cmap,
			     struct rxm_cmap_handle *handle,
			     enum rxm_cmap_reject_reason reject_reason)
{
	FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
		"Processing reject for handle: %p\n", handle);
	switch (handle->state) {
	case RXM_CMAP_CONNREQ_RECV:
	case RXM_CMAP_CONNECTED:
		/* Handle is being re-used for incoming connection request */
		break;
	case RXM_CMAP_CONNREQ_SENT:
		if (reject_reason == RXM_CMAP_REJECT_GENUINE) {
			FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
			       "Deleting connection handle\n");
			rxm_cmap_del_handle(handle);
		} else {
			FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
			       "Connection handle is being re-used. Close the connection\n");
			rxm_conn_close(handle);
		}
		break;
	case RXM_CMAP_SHUTDOWN:
		FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
			"Connection handle already being deleted\n");
		break;
	default:
		FI_WARN(cmap->av->prov, FI_LOG_EP_CTRL, "Invalid cmap state: "
			"%d when receiving connection reject\n", handle->state);
		assert(0);
	}
}

int rxm_cmap_process_connreq(struct rxm_cmap *cmap, void *addr,
			     struct rxm_cmap_handle **handle_ret,
			     uint8_t *reject_reason)
{
	struct rxm_cmap_handle *handle;
	int ret = 0, cmp;
	fi_addr_t fi_addr = ofi_ip_av_get_fi_addr(cmap->av, addr);

	ofi_straddr_dbg(cmap->av->prov, FI_LOG_EP_CTRL,
			"Processing connreq from remote pep", addr);

	if (fi_addr == FI_ADDR_NOTAVAIL)
		handle = rxm_cmap_get_handle_peer(cmap, addr);
	else
		handle = rxm_cmap_acquire_handle(cmap, fi_addr);

	if (!handle) {
		if (fi_addr == FI_ADDR_NOTAVAIL)
			ret = rxm_cmap_alloc_handle_peer(cmap, addr,
							 RXM_CMAP_CONNREQ_RECV,
							 &handle);
		else
			ret = rxm_cmap_alloc_handle(cmap, fi_addr,
						    RXM_CMAP_CONNREQ_RECV,
						    &handle);
		if (ret)
			goto unlock;
	}

	switch (handle->state) {
	case RXM_CMAP_CONNECTED:
		FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
			"Connection already present.\n");
		ret = -FI_EALREADY;
		break;
	case RXM_CMAP_CONNREQ_SENT:
		ofi_straddr_dbg(cmap->av->prov, FI_LOG_EP_CTRL, "local_name",
				cmap->attr.name);
		ofi_straddr_dbg(cmap->av->prov, FI_LOG_EP_CTRL, "remote_name",
				addr);

		cmp = ofi_addr_cmp(cmap->av->prov, addr, cmap->attr.name);

		if (cmp < 0) {
			FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
				"Remote name lower than local name.\n");
			*reject_reason = RXM_CMAP_REJECT_SIMULT_CONN;
			ret = -FI_EALREADY;
			break;
		} else if (cmp > 0) {
			FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
				"Re-using handle: %p to accept remote "
				"connection\n", handle);
			*reject_reason = RXM_CMAP_REJECT_GENUINE;
			rxm_conn_close(handle);
		} else {
			FI_DBG(cmap->av->prov, FI_LOG_EP_CTRL,
				"Endpoint connects to itself\n");
			ret = rxm_cmap_alloc_handle_peer(cmap, addr,
							  RXM_CMAP_CONNREQ_RECV,
							  &handle);
			if (ret)
				goto unlock;

			assert(fi_addr != FI_ADDR_NOTAVAIL);
			handle->fi_addr = fi_addr;
		}
		/* Fall through */
	case RXM_CMAP_IDLE:
		RXM_CM_UPDATE_STATE(handle, RXM_CMAP_CONNREQ_RECV);
		/* Fall through */
	case RXM_CMAP_CONNREQ_RECV:
		*handle_ret = handle;
		break;
	case RXM_CMAP_SHUTDOWN:
		FI_WARN(cmap->av->prov, FI_LOG_EP_CTRL, "handle :%p marked for "
			"deletion / shutdown, reject connection\n", handle);
		*reject_reason = RXM_CMAP_REJECT_GENUINE;
		ret = -FI_EOPBADSTATE;
		break;
	default:
		FI_WARN(cmap->av->prov, FI_LOG_EP_CTRL,
		       "invalid handle state: %d\n", handle->state);
		assert(0);
		ret = -FI_EOPBADSTATE;
	}
unlock:
	return ret;
}

int rxm_msg_eq_progress(struct rxm_ep *rxm_ep)
{
	struct rxm_msg_eq_entry *entry;
	int ret;

	entry = alloca(RXM_MSG_EQ_ENTRY_SZ);
	if (!entry) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"unable to allocate memory!\n");
		return -FI_ENOMEM;
	}

	while (1) {
		entry->rd = rxm_eq_read(rxm_ep, RXM_MSG_EQ_ENTRY_SZ, entry);
		if (entry->rd < 0 && entry->rd != -FI_ECONNREFUSED) {
			ret = (int) entry->rd;
			break;
		}
		ret = rxm_conn_handle_event(rxm_ep, entry);
		if (ret) {
			FI_DBG(&rxm_prov, FI_LOG_EP_CTRL,
			       "invalid connection handle event: %d\n", ret);
			break;
		}
	}
	return ret;
}

int rxm_cmap_connect(struct rxm_ep *rxm_ep, fi_addr_t fi_addr,
		     struct rxm_cmap_handle *handle)
{
	int ret = FI_SUCCESS;

	switch (handle->state) {
	case RXM_CMAP_IDLE:
		FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "initiating MSG_EP connect "
		       "for fi_addr: %" PRIu64 "\n", fi_addr);
		ret = rxm_conn_connect(rxm_ep, handle,
				       ofi_av_get_addr(rxm_ep->cmap->av, fi_addr));
		if (ret) {
			rxm_cmap_del_handle(handle);
		} else {
			RXM_CM_UPDATE_STATE(handle, RXM_CMAP_CONNREQ_SENT);
			ret = -FI_EAGAIN;
		}
		break;
	case RXM_CMAP_CONNREQ_SENT:
	case RXM_CMAP_CONNREQ_RECV:
	case RXM_CMAP_SHUTDOWN:
		ret = -FI_EAGAIN;
		break;
	default:
		FI_WARN(rxm_ep->cmap->av->prov, FI_LOG_EP_CTRL,
			"Invalid cmap handle state\n");
		assert(0);
		ret = -FI_EOPBADSTATE;
	}
	if (ret == -FI_EAGAIN)
		rxm_msg_eq_progress(rxm_ep);

	return ret;
}

static int rxm_cmap_cm_thread_close(struct rxm_cmap *cmap)
{
	int ret;

	FI_INFO(&rxm_prov, FI_LOG_EP_CTRL, "stopping CM thread\n");
	if (!cmap->cm_thread)
		return 0;

	cmap->ep->do_progress = false;
	ret = rxm_conn_signal(cmap->ep, NULL, RXM_CMAP_EXIT);
	if (ret) {
		FI_WARN(cmap->av->prov, FI_LOG_EP_CTRL,
			"Unable to signal CM thread\n");
		return ret;
	}
	ret = pthread_join(cmap->cm_thread, NULL);
	if (ret) {
		FI_WARN(cmap->av->prov, FI_LOG_EP_CTRL,
			"Unable to join CM thread\n");
		return ret;
	}
	return 0;
}

void rxm_cmap_free(struct rxm_cmap *cmap)
{
	struct rxm_cmap_peer *peer;
	struct dlist_entry *entry;
	size_t i;

	FI_INFO(cmap->av->prov, FI_LOG_EP_CTRL, "Closing cmap\n");
	rxm_cmap_cm_thread_close(cmap);

	for (i = 0; i < cmap->num_allocated; i++) {
		if (cmap->handles_av[i]) {
			rxm_cmap_clear_key(cmap->handles_av[i]);
			rxm_conn_free(cmap->handles_av[i]);
			cmap->handles_av[i] = 0;
		}
	}

	while(!dlist_empty(&cmap->peer_list)) {
		entry = cmap->peer_list.next;
		peer = container_of(entry, struct rxm_cmap_peer, entry);
		dlist_remove(&peer->entry);
		rxm_cmap_clear_key(peer->handle);
		rxm_conn_free(peer->handle);
		free(peer);
	}

	free(cmap->handles_av);
	free(cmap->attr.name);
	ofi_idx_reset(&cmap->handles_idx);
	free(cmap);
}

static int
rxm_cmap_update_addr(struct util_av *av, void *addr,
		     fi_addr_t fi_addr, void *arg)
{
	return rxm_cmap_update((struct rxm_cmap *)arg, addr, fi_addr);
}

int rxm_cmap_bind_to_av(struct rxm_cmap *cmap, struct util_av *av)
{
	cmap->av = av;
	return ofi_av_elements_iter(av, rxm_cmap_update_addr, (void *)cmap);
}

int rxm_cmap_alloc(struct rxm_ep *rxm_ep, struct rxm_cmap_attr *attr)
{
	struct rxm_cmap *cmap;
	struct util_ep *ep = &rxm_ep->util_ep;
	int ret;

	cmap = calloc(1, sizeof *cmap);
	if (!cmap)
		return -FI_ENOMEM;

	cmap->ep = rxm_ep;
	cmap->av = ep->av;

	cmap->handles_av = calloc(cmap->av->count, sizeof(*cmap->handles_av));
	if (!cmap->handles_av) {
		ret = -FI_ENOMEM;
		goto err1;
	}
	cmap->num_allocated = ep->av->count;

	cmap->attr = *attr;
	cmap->attr.name = mem_dup(attr->name, ep->av->addrlen);
	if (!cmap->attr.name) {
		ret = -FI_ENOMEM;
		goto err2;
	}

	memset(&cmap->handles_idx, 0, sizeof(cmap->handles_idx));
	ofi_key_idx_init(&cmap->key_idx, RXM_CMAP_IDX_BITS);

	dlist_init(&cmap->peer_list);

	rxm_ep->cmap = cmap;

	if (ep->domain->data_progress == FI_PROGRESS_AUTO || force_auto_progress) {

		assert(ep->domain->threading == FI_THREAD_SAFE);
		rxm_ep->do_progress = true;
		if (pthread_create(&cmap->cm_thread, 0,
				   rxm_ep->rxm_info->caps & FI_ATOMIC ?
				   rxm_conn_atomic_progress :
				   rxm_conn_progress, ep)) {
			FI_WARN(ep->av->prov, FI_LOG_EP_CTRL,
				"unable to create cmap thread\n");
			ret = -ofi_syserr();
			goto err3;
		}
	}

	assert(ep->av);
	ret = rxm_cmap_bind_to_av(cmap, ep->av);
	if (ret)
		goto err4;

	return FI_SUCCESS;
err4:
	rxm_cmap_cm_thread_close(cmap);
err3:
	rxm_ep->cmap = NULL;
	free(cmap->attr.name);
err2:
	free(cmap->handles_av);
err1:
	free(cmap);
	return ret;
}

static int rxm_msg_ep_open(struct rxm_ep *rxm_ep, struct fi_info *msg_info,
			   struct rxm_conn *rxm_conn, void *context)
{
	struct rxm_domain *rxm_domain;
	struct fid_ep *msg_ep;
	int ret;

	rxm_domain = container_of(rxm_ep->util_ep.domain, struct rxm_domain,
			util_domain);
	ret = fi_endpoint(rxm_domain->msg_domain, msg_info, &msg_ep, context);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"unable to create msg_ep: %d\n", ret);
		return ret;
	}

	ret = fi_ep_bind(msg_ep, &rxm_ep->msg_eq->fid, 0);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"unable to bind msg EP to EQ: %d\n", ret);
		goto err;
	}

	if (rxm_ep->srx_ctx) {
		ret = fi_ep_bind(msg_ep, &rxm_ep->srx_ctx->fid, 0);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "unable to bind msg "
				"EP to shared RX ctx: %d\n", ret);
			goto err;
		}
	}

	// TODO add other completion flags
	ret = fi_ep_bind(msg_ep, &rxm_ep->msg_cq->fid, FI_TRANSMIT | FI_RECV);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"unable to bind msg_ep to msg_cq: %d\n", ret);
		goto err;
	}

	ret = fi_enable(msg_ep);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"unable to enable msg_ep: %d\n", ret);
		goto err;
	}

	if (!rxm_ep->srx_ctx) {
		ret = rxm_msg_ep_prepost_recv(rxm_ep, msg_ep);
		if (ret)
			goto err;
	}

	rxm_conn->msg_ep = msg_ep;
	return 0;
err:
	fi_close(&msg_ep->fid);
	return ret;
}

static int rxm_conn_reprocess_directed_recvs(struct rxm_recv_queue *recv_queue)
{
	struct rxm_rx_buf *rx_buf;
	struct dlist_entry *entry, *tmp_entry;
	struct rxm_recv_match_attr match_attr;
	struct fi_cq_err_entry err_entry = {0};
	int ret, count = 0;

	dlist_foreach_container_safe(&recv_queue->unexp_msg_list,
				     struct rxm_rx_buf, rx_buf,
				     unexp_msg.entry, tmp_entry) {
		if (rx_buf->unexp_msg.addr == rx_buf->conn->handle.fi_addr)
			continue;

		assert(rx_buf->unexp_msg.addr == FI_ADDR_NOTAVAIL);

		rx_buf->unexp_msg.addr = rx_buf->conn->handle.fi_addr;
		match_attr.addr = rx_buf->unexp_msg.addr;
		match_attr.tag = rx_buf->unexp_msg.tag;

		entry = dlist_remove_first_match(&recv_queue->recv_list,
						 recv_queue->match_recv,
						 &match_attr);
		if (!entry)
			continue;

		dlist_remove(&rx_buf->unexp_msg.entry);
		rx_buf->recv_entry = container_of(entry, struct rxm_recv_entry,
						  entry);

		ret = rxm_cq_handle_rx_buf(rx_buf);
		if (ret) {
			err_entry.op_context = rx_buf;
			err_entry.flags = rx_buf->recv_entry->comp_flags;
			err_entry.len = rx_buf->pkt.hdr.size;
			err_entry.data = rx_buf->pkt.hdr.data;
			err_entry.tag = rx_buf->pkt.hdr.tag;
			err_entry.err = ret;
			err_entry.prov_errno = ret;
			ofi_cq_write_error(recv_queue->rxm_ep->util_ep.rx_cq,
					   &err_entry);
			if (rx_buf->ep->util_ep.flags & OFI_CNTR_ENABLED)
				rxm_cntr_incerr(rx_buf->ep->util_ep.rx_cntr);

			rxm_rx_buf_free(rx_buf);

			if (!(rx_buf->recv_entry->flags & FI_MULTI_RECV))
				rxm_recv_entry_release(recv_queue,
						       rx_buf->recv_entry);
		}
		count++;
	}
	return count;
}

static void
rxm_conn_av_updated_handler(struct rxm_cmap_handle *handle)
{
	struct rxm_ep *ep = handle->cmap->ep;
	int count = 0;

	if (ep->rxm_info->caps & FI_DIRECTED_RECV) {
		count += rxm_conn_reprocess_directed_recvs(&ep->recv_queue);
		count += rxm_conn_reprocess_directed_recvs(&ep->trecv_queue);

		FI_DBG(&rxm_prov, FI_LOG_EP_CTRL,
		       "Reprocessed directed recvs - %d\n", count);
	}
}

static struct rxm_cmap_handle *rxm_conn_alloc(struct rxm_cmap *cmap)
{
	struct rxm_conn *rxm_conn = calloc(1, sizeof(*rxm_conn));

	if (OFI_UNLIKELY(!rxm_conn))
		return NULL;

	if (rxm_conn_res_alloc(cmap->ep, rxm_conn)) {
		free(rxm_conn);
		return NULL;
	}

	return &rxm_conn->handle;
}

static inline int
rxm_conn_verify_cm_data(union rxm_cm_data *remote_cm_data,
			union rxm_cm_data *local_cm_data)
{
	/* This should stay at top as it helps to avoid endian conversion
	 * for other fields in rxm_cm_data */
	if (remote_cm_data->connect.version != local_cm_data->connect.version) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "cm data version mismatch "
			"(local: %" PRIu8 ", remote:  %" PRIu8 ")\n",
			local_cm_data->connect.version,
			remote_cm_data->connect.version);
		goto err;
	}
	if (remote_cm_data->connect.endianness != local_cm_data->connect.endianness) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "cm data endianness mismatch "
			"(local: %" PRIu8 ", remote:  %" PRIu8 ")\n",
			local_cm_data->connect.endianness,
			remote_cm_data->connect.endianness);
		goto err;
	}
	if (remote_cm_data->connect.ctrl_version != local_cm_data->connect.ctrl_version) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "cm data ctrl_version mismatch "
			"(local: %" PRIu8 ", remote:  %" PRIu8 ")\n",
			local_cm_data->connect.ctrl_version,
			remote_cm_data->connect.ctrl_version);
		goto err;
	}
	if (remote_cm_data->connect.op_version != local_cm_data->connect.op_version) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "cm data op_version mismatch "
			"(local: %" PRIu8 ", remote:  %" PRIu8 ")\n",
			local_cm_data->connect.op_version,
			remote_cm_data->connect.op_version);
		goto err;
	}
	if (remote_cm_data->connect.eager_size != local_cm_data->connect.eager_size) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "cm data eager_size mismatch "
			"(local: %" PRIu32 ", remote:  %" PRIu32 ")\n",
			local_cm_data->connect.eager_size,
			remote_cm_data->connect.eager_size);
		goto err;
	}
	return FI_SUCCESS;
err:
	return -FI_EINVAL;
}

static size_t rxm_conn_get_rx_size(struct rxm_ep *rxm_ep,
				   struct fi_info *msg_info)
{
	if (msg_info->ep_attr->rx_ctx_cnt == FI_SHARED_CONTEXT)
		return MAX(MIN(16, msg_info->rx_attr->size),
			   (msg_info->rx_attr->size /
			    rxm_ep->util_ep.av->count));
	else
		return msg_info->rx_attr->size;
}

static int
rxm_msg_process_connreq(struct rxm_ep *rxm_ep, struct fi_info *msg_info,
			union rxm_cm_data *remote_cm_data)
{
	struct rxm_conn *rxm_conn;
	union rxm_cm_data cm_data = {
		.connect = {
			.version = RXM_CM_DATA_VERSION,
			.endianness = ofi_detect_endianness(),
			.ctrl_version = RXM_CTRL_VERSION,
			.op_version = RXM_OP_VERSION,
			.eager_size = rxm_ep->rxm_info->tx_attr->inject_size,
		},
	};
	union rxm_cm_data reject_cm_data = {
		.reject = {
			.version = RXM_CM_DATA_VERSION,
			.reason = RXM_CMAP_REJECT_GENUINE,
		}
	};
	struct rxm_cmap_handle *handle;
	struct sockaddr_storage remote_pep_addr;
	int ret;

	assert(sizeof(uint32_t) == sizeof(cm_data.accept.rx_size));
	assert(msg_info->rx_attr->size <= (uint32_t)-1);

	if (rxm_conn_verify_cm_data(remote_cm_data, &cm_data)) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"CM data mismatch was detected\n");
		ret = -FI_EINVAL;
		goto err1;
	}

	memcpy(&remote_pep_addr, msg_info->dest_addr, msg_info->dest_addrlen);
	ofi_addr_set_port((struct sockaddr *)&remote_pep_addr,
			  remote_cm_data->connect.port);

	ret = rxm_cmap_process_connreq(rxm_ep->cmap, &remote_pep_addr,
				       &handle, &reject_cm_data.reject.reason);
	if (ret)
		goto err1;

	rxm_conn = container_of(handle, struct rxm_conn, handle);

	rxm_conn->handle.remote_key = remote_cm_data->connect.client_conn_id;
	rxm_conn->rndv_tx_credits = remote_cm_data->connect.rx_size;
	assert(rxm_conn->rndv_tx_credits);

	ret = rxm_msg_ep_open(rxm_ep, msg_info, rxm_conn, handle);
	if (ret)
		goto err2;

	cm_data.accept.server_conn_id = rxm_conn->handle.key;
	cm_data.accept.rx_size = rxm_conn_get_rx_size(rxm_ep, msg_info);

	ret = fi_accept(rxm_conn->msg_ep, &cm_data.accept.server_conn_id,
			sizeof(cm_data.accept));
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unable to accept incoming connection\n");
		goto err2;
	}

	return ret;
err2:
	rxm_cmap_del_handle(&rxm_conn->handle);
err1:
	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL,
	       "rejecting incoming connection request (reject reason: %d)\n",
	       (enum rxm_cmap_reject_reason)reject_cm_data.reject.reason);
	fi_reject(rxm_ep->msg_pep, msg_info->handle,
		  &reject_cm_data.reject, sizeof(reject_cm_data.reject));
	return ret;
}

static void rxm_flush_msg_cq(struct rxm_ep *rxm_ep)
{
	struct fi_cq_data_entry comp;
	int ret;
	do {
		ret = fi_cq_read(rxm_ep->msg_cq, &comp, 1);
		if (ret > 0) {
			ret = rxm_cq_handle_comp(rxm_ep, &comp);
			if (OFI_UNLIKELY(ret)) {
				rxm_cq_write_error_all(rxm_ep, ret);
			} else {
				ret = 1;
			}
		} else if (ret == -FI_EAVAIL) {
			rxm_cq_read_write_error(rxm_ep);
			ret = 1;
		} else if (ret < 0 && ret != -FI_EAGAIN) {
			rxm_cq_write_error_all(rxm_ep, ret);
		}
	} while (ret > 0);
}

static int rxm_conn_handle_notify(struct fi_eq_entry *eq_entry)
{
	struct rxm_cmap *cmap;
	struct rxm_cmap_handle *handle;

	FI_INFO(&rxm_prov, FI_LOG_EP_CTRL, "notify event %" PRIu64 "\n",
		eq_entry->data);

	if ((enum rxm_cmap_signal) eq_entry->data != RXM_CMAP_FREE)
		return -FI_EOTHER;

	handle = eq_entry->context;
	assert(handle->state == RXM_CMAP_SHUTDOWN);
	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "freeing handle: %p\n", handle);
	cmap = handle->cmap;

	rxm_conn_close(handle);

	// after closing the connection, we need to flush any dangling references to the
	// handle from msg_cq entries that have not been cleaned up yet, otherwise we
	// could run into problems during CQ cleanup.  these entries will be errored so
	// keep reading through EAVAIL.
	rxm_flush_msg_cq(cmap->ep);

	if (handle->peer) {
		dlist_remove(&handle->peer->entry);
		free(handle->peer);
		handle->peer = NULL;
	} else {
		cmap->handles_av[handle->fi_addr] = 0;
	}
	rxm_conn_free(handle);
	return 0;
}

static void rxm_conn_wake_up_wait_obj(struct rxm_ep *rxm_ep)
{
	if (rxm_ep->util_ep.tx_cq && rxm_ep->util_ep.tx_cq->wait)
		util_cq_signal(rxm_ep->util_ep.tx_cq);
	if (rxm_ep->util_ep.tx_cntr && rxm_ep->util_ep.tx_cntr->wait)
		util_cntr_signal(rxm_ep->util_ep.tx_cntr);
}

static int
rxm_conn_handle_reject(struct rxm_ep *rxm_ep, struct rxm_msg_eq_entry *entry)
{
	union rxm_cm_data *cm_data = entry->err_entry.err_data;

	if (!cm_data || entry->err_entry.err_data_size != sizeof(cm_data->reject)) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "connection reject: "
			"no reject error data (cm_data) was found "
			"(data length expected: %zu found: %zu)\n",
			sizeof(cm_data->reject),
			entry->err_entry.err_data_size);
		return -FI_EOTHER;
	}

	if (cm_data->reject.version != RXM_CM_DATA_VERSION) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "connection reject: "
			"cm data version mismatch (local: %" PRIu8
			", remote:  %" PRIu8 ")\n",
			(uint8_t) RXM_CM_DATA_VERSION,
			cm_data->reject.version);
		return -FI_EOTHER;
	}

	if (cm_data->reject.reason == RXM_CMAP_REJECT_GENUINE) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "connection reject: "
		       "remote peer didn't accept the connection\n");
		FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "connection reject: "
		       "(reason: RXM_CMAP_REJECT_GENUINE)\n");
		OFI_EQ_STRERROR(&rxm_prov, FI_LOG_WARN, FI_LOG_EP_CTRL,
				rxm_ep->msg_eq, &entry->err_entry);
	} else if (cm_data->reject.reason == RXM_CMAP_REJECT_SIMULT_CONN) {
		FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "connection reject: "
		       "(reason: RXM_CMAP_REJECT_SIMULT_CONN)\n");
	} else {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "connection reject: "
		        "received unknown reject reason: %d\n",
			cm_data->reject.reason);
	}
	rxm_cmap_process_reject(rxm_ep->cmap, entry->context,
				cm_data->reject.reason);
	return 0;
}

static int
rxm_conn_handle_event(struct rxm_ep *rxm_ep, struct rxm_msg_eq_entry *entry)
{
	if (entry->rd == -FI_ECONNREFUSED)
		return rxm_conn_handle_reject(rxm_ep, entry);

	switch (entry->event) {
	case FI_NOTIFY:
		return rxm_conn_handle_notify((struct fi_eq_entry *)
					      &entry->cm_entry);
	case FI_CONNREQ:
		FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "Got new connection\n");
		if ((size_t)entry->rd != RXM_CM_ENTRY_SZ) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"Received a connection request with no CM data. "
				"Is sender running FI_PROTO_RXM?\n");
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "Received CM entry "
				"size (%zd) not matching expected (%zu)\n",
				entry->rd, RXM_CM_ENTRY_SZ);
			return -FI_EOTHER;
		}
		rxm_msg_process_connreq(rxm_ep, entry->cm_entry.info,
					(union rxm_cm_data *) entry->cm_entry.data);
		fi_freeinfo(entry->cm_entry.info);
		break;
	case FI_CONNECTED:
		assert(entry->cm_entry.fid->context);
		FI_DBG(&rxm_prov, FI_LOG_EP_CTRL,
		       "connection successful\n");
		rxm_cmap_process_connect(rxm_ep->cmap,
			entry->cm_entry.fid->context,
			entry->rd - sizeof(entry->cm_entry) > 0 ?
			(union rxm_cm_data *) entry->cm_entry.data : NULL);
		rxm_conn_wake_up_wait_obj(rxm_ep);
		break;
	case FI_SHUTDOWN:
		FI_DBG(&rxm_prov, FI_LOG_EP_CTRL,
		       "Received connection shutdown\n");
		rxm_cmap_process_shutdown(rxm_ep->cmap,
					  entry->cm_entry.fid->context);
		break;
	default:
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unknown event: %u\n", entry->event);
		return -FI_EOTHER;
	}
	return 0;
}

static ssize_t rxm_eq_sread(struct rxm_ep *rxm_ep, size_t len,
			    struct rxm_msg_eq_entry *entry)
{
	ssize_t rd;
	int once = 1;

	do {
		/* TODO convert this to poll + fi_eq_read so that we can grab
		 * rxm_ep lock before reading the EQ. This is needed to avoid
		 * processing events / error entries from closed MSG EPs. This
		 * can be done only for non-Windows OSes as Windows doesn't
		 * have poll for a generic file descriptor. */
		rd = fi_eq_sread(rxm_ep->msg_eq, &entry->event, &entry->cm_entry,
				 len, -1, 0);
		if (rd >= 0)
			return rd;
		if (rd == -FI_EINTR && once) {
			FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "Ignoring EINTR\n");
			once = 0;
		}
	} while (rd == -FI_EINTR);

	if (rd != -FI_EAVAIL) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"unable to fi_eq_sread: %s (%zd)\n",
			fi_strerror(-rd), -rd);
		return rd;
	}

	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	rd = rxm_eq_readerr(rxm_ep, entry);
	ofi_ep_lock_release(&rxm_ep->util_ep);
	return rd;
}

static inline int rxm_conn_eq_event(struct rxm_ep *rxm_ep,
				    struct rxm_msg_eq_entry *entry)
{
	int ret;

	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	ret = rxm_conn_handle_event(rxm_ep, entry) ? -1 : 0;
	ofi_ep_lock_release(&rxm_ep->util_ep);

	return ret;
}

static void *rxm_conn_progress(void *arg)
{
	struct rxm_ep *ep = container_of(arg, struct rxm_ep, util_ep);
	struct rxm_msg_eq_entry *entry;

	entry = alloca(RXM_MSG_EQ_ENTRY_SZ);
	if (!entry)
		return NULL;

	FI_INFO(&rxm_prov, FI_LOG_EP_CTRL, "Starting auto-progress thread\n");

	while (ep->do_progress) {
		memset(entry, 0, RXM_MSG_EQ_ENTRY_SZ);
		entry->rd = rxm_eq_sread(ep, RXM_CM_ENTRY_SZ, entry);
		if (entry->rd < 0 && entry->rd != -FI_ECONNREFUSED)
			continue;

		rxm_conn_eq_event(ep, entry);
	}

	FI_INFO(&rxm_prov, FI_LOG_EP_CTRL, "Stopping auto-progress thread\n");
	return NULL;
}

static inline int
rxm_conn_auto_progress_eq(struct rxm_ep *rxm_ep, struct rxm_msg_eq_entry *entry)
{
	memset(entry, 0, RXM_MSG_EQ_ENTRY_SZ);

	ofi_ep_lock_acquire(&rxm_ep->util_ep);
	entry->rd = rxm_eq_read(rxm_ep, RXM_CM_ENTRY_SZ, entry);
	ofi_ep_lock_release(&rxm_ep->util_ep);

	if (!entry->rd || entry->rd == -FI_EAGAIN)
		return FI_SUCCESS;
	if (entry->rd < 0 && entry->rd != -FI_ECONNREFUSED)
		return entry->rd;

	return rxm_conn_eq_event(rxm_ep, entry);
}

static void *rxm_conn_atomic_progress(void *arg)
{
	struct rxm_ep *ep = container_of(arg, struct rxm_ep, util_ep);
	struct rxm_msg_eq_entry *entry;
	struct rxm_fabric *fabric;
	struct fid *fids[2] = {
		&ep->msg_eq->fid,
		&ep->msg_cq->fid,
	};
	struct pollfd fds[2] = {
		{.events = POLLIN},
		{.events = POLLIN},
	};
	int ret;

	entry = alloca(RXM_MSG_EQ_ENTRY_SZ);
	if (!entry)
		return NULL;

	fabric = container_of(ep->util_ep.domain->fabric,
			      struct rxm_fabric, util_fabric);

	ret = fi_control(&ep->msg_eq->fid, FI_GETWAIT, &fds[0].fd);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"unable to get msg EQ fd: %s\n", fi_strerror(ret));
		return NULL;
	}

	ret = fi_control(&ep->msg_cq->fid, FI_GETWAIT, &fds[1].fd);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"unable to get msg CQ fd: %s\n", fi_strerror(ret));
		return NULL;
	}

	FI_INFO(&rxm_prov, FI_LOG_EP_CTRL, "Starting auto-progress thread\n");
	while (ep->do_progress) {
		ret = fi_trywait(fabric->msg_fabric, fids, 2);

		if (!ret) {
			fds[0].revents = 0;
			fds[1].revents = 0;

			ret = poll(fds, 2, -1);
			if (ret == -1 && errno != EINTR) {
				FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
					"Select error %s, closing CM thread\n",
					strerror(errno));
				break;
			}
		}
		rxm_conn_auto_progress_eq(ep, entry);
		ep->util_ep.progress(&ep->util_ep);
	}

	FI_INFO(&rxm_prov, FI_LOG_EP_CTRL, "Stopping auto progress thread\n");
	return NULL;
}

static int rxm_prepare_cm_data(struct fid_pep *pep, struct rxm_cmap_handle *handle,
		union rxm_cm_data *cm_data)
{
	struct sockaddr_storage name;
	size_t cm_data_size = 0;
	size_t name_size = sizeof(name);
	size_t opt_size = sizeof(cm_data_size);
	int ret;

	ret = fi_getopt(&pep->fid, FI_OPT_ENDPOINT, FI_OPT_CM_DATA_SIZE,
			&cm_data_size, &opt_size);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "fi_getopt failed\n");
		return ret;
	}

	if (cm_data_size < sizeof(*cm_data)) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "MSG EP CM data size too small\n");
		return -FI_EOTHER;
	}

	ret = fi_getname(&pep->fid, &name, &name_size);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "Unable to get msg pep name\n");
		return ret;
	}

	cm_data->connect.port = ofi_addr_get_port((struct sockaddr *)&name);
	cm_data->connect.client_conn_id = handle->key;
	return 0;
}

static int
rxm_conn_connect(struct rxm_ep *ep, struct rxm_cmap_handle *handle,
		 const void *addr)
{
	int ret;
	struct rxm_conn *rxm_conn = container_of(handle, struct rxm_conn, handle);
	union rxm_cm_data cm_data = {
		.connect = {
			.version = RXM_CM_DATA_VERSION,
			.ctrl_version = RXM_CTRL_VERSION,
			.op_version = RXM_OP_VERSION,
			.endianness = ofi_detect_endianness(),
			.eager_size = ep->rxm_info->tx_attr->inject_size,
		},
	};

	assert(sizeof(uint32_t) == sizeof(cm_data.connect.eager_size));
	assert(sizeof(uint32_t) == sizeof(cm_data.connect.rx_size));
	assert(ep->rxm_info->tx_attr->inject_size <= (uint32_t) -1);
	assert(ep->msg_info->rx_attr->size <= (uint32_t) -1);

	free(ep->msg_info->dest_addr);
	ep->msg_info->dest_addrlen = ep->msg_info->src_addrlen;

	ep->msg_info->dest_addr = mem_dup(addr, ep->msg_info->dest_addrlen);
	if (!ep->msg_info->dest_addr) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "mem_dup failed, len %zu\n",
			ep->msg_info->dest_addrlen);
		return -FI_ENOMEM;
	}

	ret = rxm_msg_ep_open(ep, ep->msg_info, rxm_conn, &rxm_conn->handle);
	if (ret)
		return ret;

	/* We have to send passive endpoint's address to the server since the
	 * address from which connection request would be sent would have a
	 * different port. */
	ret = rxm_prepare_cm_data(ep->msg_pep, &rxm_conn->handle, &cm_data);
	if (ret)
		goto err;

	cm_data.connect.rx_size = rxm_conn_get_rx_size(ep, ep->msg_info);

	ret = fi_connect(rxm_conn->msg_ep, ep->msg_info->dest_addr,
			 &cm_data, sizeof(cm_data));
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "unable to connect msg_ep\n");
		goto err;
	}
	return 0;

err:
	fi_close(&rxm_conn->msg_ep->fid);
	rxm_conn->msg_ep = NULL;
	return ret;
}

static int rxm_conn_signal(struct rxm_ep *ep, void *context,
			   enum rxm_cmap_signal signal)
{
	struct fi_eq_entry entry = {0};
	ssize_t rd;

	entry.context = context;
	entry.data = (uint64_t) signal;

	rd = fi_eq_write(ep->msg_eq, FI_NOTIFY, &entry, sizeof(entry), 0);
	if (rd != sizeof(entry)) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "Unable to signal\n");
		return (int)rd;
	}
	return 0;
}

int rxm_conn_cmap_alloc(struct rxm_ep *rxm_ep)
{
	struct rxm_cmap_attr attr;
	int ret;
	size_t len = rxm_ep->util_ep.av->addrlen;
	void *name = calloc(1, len);
	if (!name) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unable to allocate memory for EP name\n");
		return -FI_ENOMEM;
	}

	/* Passive endpoint should already have fi_setname or fi_listen
	 * called on it for this to work */
	ret = fi_getname(&rxm_ep->msg_pep->fid, name, &len);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unable to fi_getname on msg_ep\n");
		goto fn;
	}
	ofi_straddr_dbg(&rxm_prov, FI_LOG_EP_CTRL, "local_name", name);

	attr.name		= name;

	ret = rxm_cmap_alloc(rxm_ep, &attr);
	if (ret)
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unable to allocate CMAP\n");
fn:
	free(name);
	return ret;
}

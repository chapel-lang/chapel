/*
 * Copyright (c) 2016-2021 Intel Corporation, Inc.  All rights reserved.
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


static void *rxm_cm_progress(void *arg);
static void *rxm_cm_atomic_progress(void *arg);
static void rxm_flush_msg_cq(struct rxm_ep *rxm_ep);


/* castable to fi_eq_cm_entry - we can't use fi_eq_cm_entry directly
 * here because of a compiler error with a 0-sized array
 */
struct rxm_eq_cm_entry {
	fid_t fid;
	struct fi_info *info;
	union rxm_cm_data data;
};


static void rxm_close_conn(struct rxm_conn *conn)
{
	struct rxm_deferred_tx_entry *tx_entry;
	struct rxm_recv_entry *rx_entry;
	struct rxm_rx_buf *buf;

	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "closing conn %p\n", conn);

	assert(ofi_ep_lock_held(&conn->ep->util_ep));
	/* All deferred transfers are internally generated */
	while (!dlist_empty(&conn->deferred_tx_queue)) {
		tx_entry = container_of(conn->deferred_tx_queue.next,
				     struct rxm_deferred_tx_entry, entry);
		rxm_dequeue_deferred_tx(tx_entry);
		free(tx_entry);
	}

	while (!dlist_empty(&conn->deferred_sar_segments)) {
		buf = container_of(conn->deferred_sar_segments.next,
				   struct rxm_rx_buf, unexp_msg.entry);
		dlist_remove(&buf->unexp_msg.entry);
		rxm_rx_buf_free(buf);
	}

	while (!dlist_empty(&conn->deferred_sar_msgs)) {
		rx_entry = container_of(conn->deferred_sar_msgs.next,
					struct rxm_recv_entry, sar.entry);
		dlist_remove(&rx_entry->entry);
		rxm_recv_entry_release(rx_entry);
	}
	fi_close(&conn->msg_ep->fid);
	rxm_flush_msg_cq(conn->ep);
	dlist_remove_init(&conn->loopback_entry);
	conn->msg_ep = NULL;
	conn->state = RXM_CM_IDLE;
}

static int rxm_open_conn(struct rxm_conn *conn, struct fi_info *msg_info)
{
	struct rxm_domain *domain;
	struct rxm_ep *ep;
	struct fid_ep *msg_ep;
	int ret;

	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "open msg ep %p\n", conn);

	assert(ofi_ep_lock_held(&conn->ep->util_ep));
	ep = conn->ep;
	domain = container_of(ep->util_ep.domain, struct rxm_domain,
			      util_domain);
	ret = fi_endpoint(domain->msg_domain, msg_info, &msg_ep, conn);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"unable to create msg_ep: %d\n", ret);
		return ret;
	}

	ret = fi_ep_bind(msg_ep, &ep->msg_eq->fid, 0);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"unable to bind msg EP to EQ: %d\n", ret);
		goto err;
	}

	if (ep->srx_ctx) {
		ret = fi_ep_bind(msg_ep, &ep->srx_ctx->fid, 0);
		if (ret) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "unable to bind msg "
				"EP to shared RX ctx: %d\n", ret);
			goto err;
		}
	}

	ret = fi_ep_bind(msg_ep, &ep->msg_cq->fid, FI_TRANSMIT | FI_RECV);
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

	ret = domain->flow_ctrl_ops->enable(msg_ep);
	if (!ret) {
		domain->flow_ctrl_ops->set_threshold(msg_ep,
					ep->msg_info->rx_attr->size / 2);
	}

	if (!ep->srx_ctx) {
		ret = rxm_prepost_recv(ep, msg_ep);
		if (ret)
			goto err;
	}

	conn->msg_ep = msg_ep;
	return 0;
err:
	fi_close(&msg_ep->fid);
	return ret;
}

/* We send passive endpoint's port to the server as connection request
 * would be from a different one.
 */
static int rxm_init_connect_data(struct rxm_conn *conn,
				 union rxm_cm_data *cm_data)
{
	size_t cm_data_size = 0;
	size_t opt_size = sizeof(cm_data_size);
	int ret;

	memset(cm_data, 0, sizeof(*cm_data));
	cm_data->connect.version = RXM_CM_DATA_VERSION;
	cm_data->connect.ctrl_version = RXM_CTRL_VERSION;
	cm_data->connect.op_version = RXM_OP_VERSION;
	cm_data->connect.endianness = ofi_detect_endianness();
	cm_data->connect.eager_limit = conn->ep->eager_limit;
	cm_data->connect.rx_size = conn->ep->msg_info->rx_attr->size;

	ret = fi_getopt(&conn->ep->msg_pep->fid, FI_OPT_ENDPOINT,
			FI_OPT_CM_DATA_SIZE, &cm_data_size, &opt_size);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "fi_getopt failed\n");
		return ret;
	}

	if (cm_data_size < sizeof(*cm_data)) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "cm data too small\n");
		return -FI_EOTHER;
	}

	cm_data->connect.port = ofi_addr_get_port(&conn->ep->addr.sa);
	cm_data->connect.client_conn_id = conn->peer->index;
	return 0;
}

static int rxm_send_connect(struct rxm_conn *conn)
{
	union rxm_cm_data cm_data;
	struct fi_info *info;
	int ret;

	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "connecting %p\n", conn);
	assert(ofi_ep_lock_held(&conn->ep->util_ep));

	info = conn->ep->msg_info;
	info->dest_addrlen = conn->ep->msg_info->src_addrlen;

	free(info->dest_addr);
	info->dest_addr = mem_dup(&conn->peer->addr, info->dest_addrlen);
	if (!info->dest_addr)
		return -FI_ENOMEM;

	ret = rxm_open_conn(conn, info);
	if (ret)
		return ret;

	ret = rxm_init_connect_data(conn, &cm_data);
	if (ret)
		goto err;

	ret = fi_connect(conn->msg_ep, info->dest_addr, &cm_data,
			 sizeof(cm_data));
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "unable to connect msg_ep\n");
		goto err;
	}
	conn->state = RXM_CM_CONNECTING;
	return 0;

err:
	fi_close(&conn->msg_ep->fid);
	conn->msg_ep = NULL;
	return ret;
}

static int rxm_connect(struct rxm_conn *conn)
{
	int ret;

	assert(ofi_ep_lock_held(&conn->ep->util_ep));

	switch (conn->state) {
	case RXM_CM_IDLE:
		ret = rxm_send_connect(conn);
		if (ret)
			return ret;
		break;
	case RXM_CM_CONNECTING:
	case RXM_CM_ACCEPTING:
		break;
	case RXM_CM_CONNECTED:
		return 0;
	default:
		assert(0);
		conn->state = RXM_CM_IDLE;
		break;
	}

	return -FI_EAGAIN;
}

static void rxm_free_conn(struct rxm_conn *conn)
{
	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "free conn %p\n", conn);
	assert(ofi_ep_lock_held(&conn->ep->util_ep));

	if (conn->flags & RXM_CONN_INDEXED)
		ofi_idm_clear(&conn->ep->conn_idx_map, conn->peer->index);

	rxm_put_peer(conn->peer);
	rxm_av_free_conn(conn);
}

void rxm_freeall_conns(struct rxm_ep *ep)
{
	struct rxm_conn *conn;
	struct dlist_entry *tmp;
	struct rxm_av *av;
	int i, cnt;

	av = container_of(ep->util_ep.av, struct rxm_av, util_av);
	ofi_ep_lock_acquire(&ep->util_ep);

	/* We can't have more connections than the current number of
	 * possible peers.
	 */
	cnt = (int) rxm_av_max_peers(av);
	for (i = 0; i < cnt; i++) {
		conn = ofi_idm_lookup(&ep->conn_idx_map, i);
		if (!conn)
			continue;

		if (conn->state != RXM_CM_IDLE)
			rxm_close_conn(conn);
		rxm_free_conn(conn);
	}

	dlist_foreach_container_safe(&ep->loopback_list, struct rxm_conn,
				     conn, loopback_entry, tmp) {
		rxm_close_conn(conn);
		rxm_free_conn(conn);
	}

	ofi_ep_lock_release(&ep->util_ep);
}

static struct rxm_conn *
rxm_alloc_conn(struct rxm_ep *ep, struct rxm_peer_addr *peer)
{
	struct rxm_conn *conn;
	struct rxm_av *av;

	assert(ofi_ep_lock_held(&ep->util_ep));
	av = container_of(ep->util_ep.av, struct rxm_av, util_av);
	conn = rxm_av_alloc_conn(av);
	if (!conn)
		return NULL;

	conn->ep = ep;
	conn->state = RXM_CM_IDLE;
	conn->remote_index = -1;
	conn->flags = 0;
	dlist_init(&conn->deferred_entry);
	dlist_init(&conn->deferred_tx_queue);
	dlist_init(&conn->deferred_sar_msgs);
	dlist_init(&conn->deferred_sar_segments);
	dlist_init(&conn->loopback_entry);

	conn->peer = peer;
	rxm_ref_peer(peer);

	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "allocated conn %p\n", conn);
	return conn;
}

static struct rxm_conn *
rxm_add_conn(struct rxm_ep *ep, struct rxm_peer_addr *peer)
{
	struct rxm_conn *conn;

	assert(ofi_ep_lock_held(&ep->util_ep));
	conn = ofi_idm_lookup(&ep->conn_idx_map, peer->index);
	if (conn)
		return conn;

	conn = rxm_alloc_conn(ep, peer);
	if (!conn)
		return NULL;

	if (ofi_idm_set(&ep->conn_idx_map, peer->index, conn) < 0) {
		rxm_free_conn(conn);
		return NULL;
	}

	conn->flags |= RXM_CONN_INDEXED;
	return conn;
}

/* The returned conn is only valid if the function returns success. */
ssize_t rxm_get_conn(struct rxm_ep *ep, fi_addr_t addr, struct rxm_conn **conn)
{
	struct rxm_peer_addr **peer;
	ssize_t ret;

	assert(ofi_ep_lock_held(&ep->util_ep));
	peer = ofi_av_addr_context(ep->util_ep.av, addr);
	*conn = rxm_add_conn(ep, *peer);
	if (!*conn)
		return -FI_ENOMEM;

	if ((*conn)->state == RXM_CM_CONNECTED) {
		if (!dlist_empty(&(*conn)->deferred_tx_queue)) {
			rxm_ep_do_progress(&ep->util_ep);
			if (!dlist_empty(&(*conn)->deferred_tx_queue))
				return -FI_EAGAIN;
		}
		return 0;
	}

	ret = rxm_connect(*conn);

	/* If the progress function encounters an error trying to establish
	 * the connection, it may free the connection object.  This resets
	 * the connection process to restart from the beginning.
	 */
	if (ret == -FI_EAGAIN)
		rxm_conn_progress(ep);
	return ret;
}

void rxm_process_connect(struct rxm_eq_cm_entry *cm_entry)
{
	struct rxm_conn *conn;

	conn = cm_entry->fid->context;
	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL,
	       "processing connected for handle: %p\n", conn);

	assert(ofi_ep_lock_held(&conn->ep->util_ep));
	if (conn->state == RXM_CM_CONNECTING)
		conn->remote_index = cm_entry->data.accept.server_conn_id;

	conn->state = RXM_CM_CONNECTED;
}

/* For simultaneous connection requests, if the peer won the coin
 * flip (reject EALREADY), our connection request is discarded.
 */
static void
rxm_process_reject(struct rxm_conn *conn, struct fi_eq_err_entry *entry)
{
	union rxm_cm_data *cm_data;
	uint8_t reason;

	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL,
	       "Processing reject for handle: %p\n", conn);
	assert(ofi_ep_lock_held(&conn->ep->util_ep));

	if (entry->err_data_size >= sizeof(cm_data->reject)) {
		cm_data = entry->err_data;
		if (cm_data->reject.version != RXM_CM_DATA_VERSION) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "invalid reject version\n");
			reason = RXM_REJECT_ECONNREFUSED;
		} else {
			reason = cm_data->reject.reason;
		}
	} else {
		reason = RXM_REJECT_ECONNREFUSED;
	}

	switch (conn->state) {
	case RXM_CM_IDLE:
		/* Unlikely, but can occur if our request was rejected, and
		 * there was a failure trying to accept the peer's.
		 */
		break;
	case RXM_CM_CONNECTING:
		rxm_close_conn(conn);
		if (reason != RXM_REJECT_EALREADY)
			rxm_free_conn(conn);
		break;
	case RXM_CM_ACCEPTING:
	case RXM_CM_CONNECTED:
		/* Our request was rejected, but we accepted the peer's. */
		break;
	default:
		assert(0);
		break;
	}
}

static int
rxm_verify_connreq(struct rxm_ep *ep, union rxm_cm_data *cm_data)
{
	if (cm_data->connect.version != RXM_CM_DATA_VERSION) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "cm version mismatch");
		return -FI_EINVAL;
	}

	if (cm_data->connect.endianness != ofi_detect_endianness()) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "endianness mismatch");
		return -FI_EINVAL;
	}

	if (cm_data->connect.ctrl_version != RXM_CTRL_VERSION) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "cm ctrl_version mismatch");
		return -FI_EINVAL;
	}

	if (cm_data->connect.op_version != RXM_OP_VERSION) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "cm op_version mismatch");
		return -FI_EINVAL;
	}

	if (cm_data->connect.eager_limit != ep->eager_limit) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "eager_limit mismatch");
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

static void
rxm_reject_connreq(struct rxm_ep *ep, struct rxm_eq_cm_entry *cm_entry,
		   uint8_t reason)
{
	union rxm_cm_data cm_data;

	cm_data.reject.version = RXM_CM_DATA_VERSION;
	cm_data.reject.reason = reason;

	fi_reject(ep->msg_pep, cm_entry->info->handle,
		  &cm_data.reject, sizeof(cm_data.reject));
}

static int
rxm_accept_connreq(struct rxm_conn *conn, struct rxm_eq_cm_entry *cm_entry)
{
	union rxm_cm_data cm_data;

	cm_data.accept.server_conn_id = conn->peer->index;
	cm_data.accept.rx_size = cm_entry->info->rx_attr->size;

	return fi_accept(conn->msg_ep, &cm_data.accept, sizeof(cm_data.accept));
}

static void
rxm_process_connreq(struct rxm_ep *ep, struct rxm_eq_cm_entry *cm_entry)
{
	union ofi_sock_ip peer_addr;
	struct rxm_peer_addr *peer;
	struct rxm_conn *conn;
	struct rxm_av *av;
	ssize_t ret;
	int cmp;

	assert(ofi_ep_lock_held(&ep->util_ep));
	if (rxm_verify_connreq(ep, &cm_entry->data))
		goto reject;

	memcpy(&peer_addr, cm_entry->info->dest_addr,
	       cm_entry->info->dest_addrlen);
	ofi_addr_set_port(&peer_addr.sa, cm_entry->data.connect.port);

	av = container_of(ep->util_ep.av, struct rxm_av, util_av);
	peer = rxm_get_peer(av, &peer_addr);
	if (!peer)
		goto reject;

	conn = rxm_add_conn(ep, peer);
	if (!conn)
		goto remove;

	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "connreq for %p\n", conn);
	switch (conn->state) {
	case RXM_CM_IDLE:
		break;
	case RXM_CM_CONNECTING:
		/* simultaneous connections */
		cmp = ofi_addr_cmp(&rxm_prov, &peer_addr.sa, &ep->addr.sa);
		if (cmp < 0) {
			/* let our request finish */
			rxm_reject_connreq(ep, cm_entry,
					   RXM_REJECT_ECONNREFUSED);
			goto put;
		} else if (cmp > 0) {
			/* accept peer's request */
			rxm_close_conn(conn);
		} else {
			/* connecting to ourself, create loopback conn */
			conn = rxm_alloc_conn(ep, peer);
			if (!conn)
				goto remove;

			dlist_insert_tail(&conn->loopback_entry, &ep->loopback_list);
			break;
		}
		break;
	case RXM_CM_ACCEPTING:
	case RXM_CM_CONNECTED:
		goto put;
	default:
		assert(0);
		break;
	}

	conn->remote_index = cm_entry->data.connect.client_conn_id;
	ret = rxm_open_conn(conn, cm_entry->info);
	if (ret)
		goto free;

	ret = rxm_accept_connreq(conn, cm_entry);
	if (ret)
		goto close;

	conn->state = RXM_CM_ACCEPTING;
put:
	rxm_put_peer(peer);
	fi_freeinfo(cm_entry->info);
	return;

close:
	rxm_close_conn(conn);
free:
	rxm_free_conn(conn);
remove:
	rxm_put_peer(peer);
reject:
	rxm_reject_connreq(ep, cm_entry, RXM_REJECT_ECONNREFUSED);
	fi_freeinfo(cm_entry->info);
}

void rxm_process_shutdown(struct rxm_conn *conn)
{
	FI_DBG(&rxm_prov, FI_LOG_EP_CTRL, "shutdown conn %p\n", conn);
	assert(ofi_ep_lock_held(&conn->ep->util_ep));

	switch (conn->state) {
	case RXM_CM_IDLE:
		break;
	case RXM_CM_CONNECTING:
	case RXM_CM_ACCEPTING:
	case RXM_CM_CONNECTED:
		rxm_close_conn(conn);
		rxm_free_conn(conn);
		break;
	default:
		break;
	}
}

static void rxm_handle_error(struct rxm_ep *ep)
{
	struct fi_eq_err_entry entry = {0};
	ssize_t ret;

	assert(ofi_ep_lock_held(&ep->util_ep));
	ret = fi_eq_readerr(ep->msg_eq, &entry, 0);
	if (ret != sizeof(entry)) {
		if (ret != -FI_EAGAIN)
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"unable to fi_eq_readerr: %zd\n", ret);
		return;
	}

	if (entry.err == ECONNREFUSED) {
		rxm_process_reject(entry.fid->context, &entry);
	} else {
		OFI_EQ_STRERROR(&rxm_prov, FI_LOG_WARN, FI_LOG_EP_CTRL,
				ep->msg_eq, &entry);
	}
}

static void
rxm_handle_event(struct rxm_ep *ep, uint32_t event,
		 struct rxm_eq_cm_entry *cm_entry, size_t len)
{
	assert(ofi_ep_lock_held(&ep->util_ep));
	switch (event) {
	case FI_NOTIFY:
		break;
	case FI_CONNREQ:
		rxm_process_connreq(ep, cm_entry);
		break;
	case FI_CONNECTED:
		rxm_process_connect(cm_entry);
		break;
	case FI_SHUTDOWN:
		rxm_process_shutdown(cm_entry->fid->context);
		break;
	default:
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unknown event: %u\n", event);
		break;
	}
}

void rxm_conn_progress(struct rxm_ep *ep)
{
	struct rxm_eq_cm_entry cm_entry;
	uint32_t event;
	int ret;

	assert(ofi_ep_lock_held(&ep->util_ep));
	do {
		ret = fi_eq_read(ep->msg_eq, &event, &cm_entry,
				 sizeof(cm_entry), 0);
		if (ret > 0) {
			rxm_handle_event(ep, event, &cm_entry, ret);
		} else if (ret == -FI_EAVAIL) {
			rxm_handle_error(ep);
			ret = 1;
		}
	} while (ret > 0);
}

void rxm_stop_listen(struct rxm_ep *ep)
{
	struct fi_eq_entry entry = {0};
	int ret;

	FI_INFO(&rxm_prov, FI_LOG_EP_CTRL, "stopping CM thread\n");
	if (!ep->cm_thread)
		return;

	ofi_ep_lock_acquire(&ep->util_ep);
	ep->do_progress = false;
	ofi_ep_lock_release(&ep->util_ep);

	ret = fi_eq_write(ep->msg_eq, FI_NOTIFY, &entry, sizeof(entry), 0);
	if (ret != sizeof(entry)) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL, "Unable to signal\n");
		return;
	}

	ret = pthread_join(ep->cm_thread, NULL);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unable to join CM thread\n");
	}
}

static void rxm_flush_msg_cq(struct rxm_ep *ep)
{
	struct fi_cq_data_entry comp;
	int ret;

	assert(ofi_ep_lock_held(&ep->util_ep));
	do {
		ret = fi_cq_read(ep->msg_cq, &comp, 1);
		if (ret > 0) {
			ret = rxm_handle_comp(ep, &comp);
			if (ret) {
				rxm_cq_write_error_all(ep, ret);
			} else {
				ret = 1;
			}
		} else if (ret == -FI_EAVAIL) {
			rxm_handle_comp_error(ep);
			ret = 1;
		} else if (ret < 0 && ret != -FI_EAGAIN) {
			rxm_cq_write_error_all(ep, ret);
		}
	} while (ret > 0);
}

static void *rxm_cm_progress(void *arg)
{
	struct rxm_ep *ep = container_of(arg, struct rxm_ep, util_ep);
	struct rxm_eq_cm_entry cm_entry;
	uint32_t event;
	ssize_t ret;

	FI_INFO(&rxm_prov, FI_LOG_EP_CTRL, "Starting auto-progress thread\n");

	ofi_ep_lock_acquire(&ep->util_ep);
	while (ep->do_progress) {
		ofi_ep_lock_release(&ep->util_ep);

		ret = fi_eq_sread(ep->msg_eq, &event, &cm_entry,
				  sizeof(cm_entry), -1, 0);

		ofi_ep_lock_acquire(&ep->util_ep);
		if (ret > 0) {
			rxm_handle_event(ep, event, &cm_entry, ret);
		} else if (ret == -FI_EAVAIL) {
			rxm_handle_error(ep);
		} else {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"Fatal error reading from msg eq");
			break;
		}
	}
	ofi_ep_lock_release(&ep->util_ep);

	FI_INFO(&rxm_prov, FI_LOG_EP_CTRL, "Stopping auto-progress thread\n");
	return NULL;
}

static void *rxm_cm_atomic_progress(void *arg)
{
	struct rxm_ep *ep = container_of(arg, struct rxm_ep, util_ep);
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
	ofi_ep_lock_acquire(&ep->util_ep);
	while (ep->do_progress) {
		ofi_ep_lock_release(&ep->util_ep);
		ret = fi_trywait(fabric->msg_fabric, fids, 2);

		if (!ret) {
			ret = poll(fds, 2, -1);
			if (ret == -1) {
				FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
					"Select error %s\n", strerror(errno));
			}
		}
		ep->util_ep.progress(&ep->util_ep);
		ofi_ep_lock_acquire(&ep->util_ep);
		rxm_conn_progress(ep);
	}
	ofi_ep_lock_release(&ep->util_ep);

	FI_INFO(&rxm_prov, FI_LOG_EP_CTRL, "Stopping auto progress thread\n");
	return NULL;
}

int rxm_start_listen(struct rxm_ep *ep)
{
	size_t addr_len;
	int ret;

	ret = fi_listen(ep->msg_pep);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"unable to set msg PEP to listen state\n");
		return ret;
	}

	addr_len = sizeof(ep->addr);
	ret = fi_getname(&ep->msg_pep->fid, &ep->addr, &addr_len);
	if (ret) {
		FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
			"Unable to get msg pep name\n");
		return ret;
	}

	/* Update src_addr that will be used for active endpoints.
	 * Zero out the port to avoid address conflicts, as we will
	 * create multiple msg ep's for a single rdm ep.
	 */
	if (ep->msg_info->src_addr) {
		free(ep->msg_info->src_addr);
		ep->msg_info->src_addr = NULL;
		ep->msg_info->src_addrlen = 0;
	}

	ep->msg_info->src_addr = mem_dup(&ep->addr, addr_len);
	if (!ep->msg_info->src_addr)
		return -FI_ENOMEM;

	ep->msg_info->src_addrlen = addr_len;
	ofi_addr_set_port(ep->msg_info->src_addr, 0);

	if (ep->util_ep.domain->data_progress == FI_PROGRESS_AUTO ||
	    force_auto_progress) {

		assert(ep->util_ep.domain->threading == FI_THREAD_SAFE);
		ep->do_progress = true;
		if (pthread_create(&ep->cm_thread, 0,
				   ep->rxm_info->caps & FI_ATOMIC ?
				   rxm_cm_atomic_progress :
				   rxm_cm_progress, ep)) {
			FI_WARN(&rxm_prov, FI_LOG_EP_CTRL,
				"unable to create cm thread\n");
			return -ofi_syserr();
		}
	}
	return 0;
}

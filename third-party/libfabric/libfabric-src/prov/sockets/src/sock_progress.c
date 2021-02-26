/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
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

#include "config.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include <complex.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <ofi_mem.h>
#include "sock.h"
#include "sock_util.h"

#define SOCK_LOG_DBG(...) _SOCK_LOG_DBG(FI_LOG_EP_DATA, __VA_ARGS__)
#define SOCK_LOG_ERROR(...) _SOCK_LOG_ERROR(FI_LOG_EP_DATA, __VA_ARGS__)

#define PE_INDEX(_pe, _e) (_e - &_pe->pe_table[0])
#define SOCK_GET_RX_ID(_addr, _bits) (((_bits) == 0) ? 0 : \
		(((uint64_t)_addr) >> (64 - _bits)))


static int sock_pe_progress_buffered_rx(struct sock_rx_ctx *rx_ctx);


static inline int sock_pe_is_data_msg(int msg_id)
{
	switch (msg_id) {
	case SOCK_OP_SEND:
	case SOCK_OP_TSEND:
	case SOCK_OP_WRITE:
	case SOCK_OP_READ:
	case SOCK_OP_ATOMIC:
		return 1;
	default:
		return 0;
	}
}

static inline ssize_t sock_pe_send_field(struct sock_pe_entry *pe_entry,
					 void *field, size_t field_len,
					 size_t start_offset)
{
	int ret;
	size_t offset, data_len;

	if (pe_entry->done_len >= start_offset + field_len)
		return 0;

	offset = pe_entry->done_len - start_offset;
	data_len = field_len - offset;
	ret = sock_comm_send(pe_entry, (char *) field + offset, data_len);

	if (ret <= 0)
		return -1;

	pe_entry->done_len += ret;
	return (ret == data_len) ? 0 : -1;
}

static inline ssize_t sock_pe_recv_field(struct sock_pe_entry *pe_entry,
					 void *field, size_t field_len,
					 size_t start_offset)
{
	int ret;
	size_t offset, data_len;

	if (pe_entry->done_len >= start_offset + field_len)
		return 0;

	offset = pe_entry->done_len - start_offset;
	data_len = field_len - offset;
	ret = sock_comm_recv(pe_entry, (char *) field + offset, data_len);
	if (ret <= 0)
		return -1;

	pe_entry->done_len += ret;
	return (ret == data_len) ? 0 : -1;
}

static inline void sock_pe_discard_field(struct sock_pe_entry *pe_entry)
{
	size_t ret;
	if (!pe_entry->rem)
		goto out;

	SOCK_LOG_DBG("Remaining for %p: %" PRId64 "\n", pe_entry, pe_entry->rem);
	ret = sock_comm_discard(pe_entry, pe_entry->rem);
	SOCK_LOG_DBG("Discarded %ld\n", ret);

	pe_entry->rem -= ret;
	if (pe_entry->rem == 0)
		pe_entry->conn->rx_pe_entry = NULL;

 out:
	if (pe_entry->done_len == pe_entry->total_len && !pe_entry->rem) {
		SOCK_LOG_DBG("Discard complete for %p\n", pe_entry);
		pe_entry->is_complete = 1;
	}
}

static void sock_pe_release_entry(struct sock_pe *pe,
				  struct sock_pe_entry *pe_entry)
{
	assert((pe_entry->type != SOCK_PE_RX) ||
		ofi_rbempty(&pe_entry->comm_buf));
	dlist_remove(&pe_entry->ctx_entry);

	if (pe_entry->conn->tx_pe_entry == pe_entry)
		pe_entry->conn->tx_pe_entry = NULL;
	if (pe_entry->conn->rx_pe_entry == pe_entry)
		pe_entry->conn->rx_pe_entry = NULL;

	if (pe_entry->type == SOCK_PE_RX && pe_entry->pe.rx.atomic_cmp) {
		ofi_buf_free(pe_entry->pe.rx.atomic_cmp);
		ofi_buf_free(pe_entry->pe.rx.atomic_src);
	}

	if (pe_entry->is_pool_entry) {
		ofi_rbfree(&pe_entry->comm_buf);
		dlist_remove(&pe_entry->entry);
		ofi_buf_free(pe_entry);
		return;
	}

	if (pe_entry->type == SOCK_PE_TX)
		ofi_rbreset(&pe_entry->comm_buf);

	pe->num_free_entries++;
	pe_entry->conn = NULL;

	memset(&pe_entry->pe.rx, 0, sizeof(pe_entry->pe.rx));
	memset(&pe_entry->pe.tx, 0, sizeof(pe_entry->pe.tx));
	memset(&pe_entry->msg_hdr, 0, sizeof(pe_entry->msg_hdr));
	memset(&pe_entry->response, 0, sizeof(pe_entry->response));

	pe_entry->type = 0;
	pe_entry->is_complete = 0;
	pe_entry->is_error = 0;
	pe_entry->done_len = 0;
	pe_entry->total_len = 0;
	pe_entry->data_len = 0;
	pe_entry->buf = 0;
	pe_entry->flags = 0;
	pe_entry->context = 0L;
	pe_entry->mr_checked = 0;
	pe_entry->completion_reported = 0;

	dlist_remove(&pe_entry->entry);
	dlist_insert_head(&pe_entry->entry, &pe->free_list);
	SOCK_LOG_DBG("progress entry %p released\n", pe_entry);
}

static struct sock_pe_entry *sock_pe_acquire_entry(struct sock_pe *pe)
{
	struct dlist_entry *entry;
	struct sock_pe_entry *pe_entry;

	if (dlist_empty(&pe->free_list)) {
		pe_entry = ofi_buf_alloc(pe->pe_rx_pool);
		SOCK_LOG_DBG("Getting rx pool entry\n");
		if (pe_entry) {
			memset(pe_entry, 0, sizeof(*pe_entry));
			pe_entry->is_pool_entry = 1;
			if (ofi_rbinit(&pe_entry->comm_buf, SOCK_PE_OVERFLOW_COMM_BUFF_SZ))
				SOCK_LOG_ERROR("failed to init comm-cache\n");
			pe_entry->cache_sz = SOCK_PE_OVERFLOW_COMM_BUFF_SZ;
			dlist_insert_tail(&pe_entry->entry, &pe->pool_list);
		}
	} else {
		pe->num_free_entries--;
		entry = pe->free_list.next;
		pe_entry = container_of(entry, struct sock_pe_entry, entry);

		assert(ofi_rbempty(&pe_entry->comm_buf));
		dlist_remove(&pe_entry->entry);
		dlist_insert_tail(&pe_entry->entry, &pe->busy_list);
		SOCK_LOG_DBG("progress entry %p acquired : %lu\n", pe_entry,
			     PE_INDEX(pe, pe_entry));
	}
	return pe_entry;
}

static void sock_pe_report_send_cq_completion(struct sock_pe_entry *pe_entry)
{
	int ret = 0;
	if (!(pe_entry->flags & SOCK_NO_COMPLETION)) {
		if (pe_entry->comp->send_cq &&
		    (!pe_entry->comp->send_cq_event ||
		     (pe_entry->msg_hdr.flags & FI_COMPLETION)))
			ret = pe_entry->comp->send_cq->report_completion(
				pe_entry->comp->send_cq, pe_entry->addr, pe_entry);
	}

	if (ret < 0) {
		SOCK_LOG_ERROR("Failed to report completion %p\n",
			       pe_entry);
		if (pe_entry->comp->eq) {
			sock_eq_report_error(
				pe_entry->comp->eq,
				&pe_entry->comp->send_cq->cq_fid.fid,
				pe_entry->comp->send_cq->cq_fid.fid.context,
				0, FI_ENOSPC, -FI_ENOSPC, NULL, 0);
		}
	}
}

static void sock_pe_report_send_completion(struct sock_pe_entry *pe_entry)
{
	struct sock_triggered_context *trigger_context;

	if (pe_entry->completion_reported)
		return;

	if (!(pe_entry->flags & SOCK_TRIGGERED_OP)) {
		sock_pe_report_send_cq_completion(pe_entry);
		if (pe_entry->comp->send_cntr)
			sock_cntr_inc(pe_entry->comp->send_cntr);
	} else {
		trigger_context = (void *) (uintptr_t) pe_entry->context;
		fi_cntr_add(trigger_context->trigger.work.completion_cntr, 1);
	}
	pe_entry->completion_reported = 1;
}

static void sock_pe_report_recv_cq_completion(struct sock_pe_entry *pe_entry)
{
	int ret = 0;
	if (pe_entry->comp->recv_cq &&
	    (!pe_entry->comp->recv_cq_event ||
	     (pe_entry->flags & FI_COMPLETION)))
		ret = pe_entry->comp->recv_cq->report_completion(
			pe_entry->comp->recv_cq, pe_entry->addr,
			pe_entry);

	if (ret < 0) {
		SOCK_LOG_ERROR("Failed to report completion %p\n", pe_entry);
		if (pe_entry->comp->eq) {
			sock_eq_report_error(
				pe_entry->comp->eq,
				&pe_entry->comp->recv_cq->cq_fid.fid,
				pe_entry->comp->recv_cq->cq_fid.fid.context,
				0, FI_ENOSPC, -FI_ENOSPC, NULL, 0);
		}
	}
}

static void sock_pe_report_recv_completion(struct sock_pe_entry *pe_entry)
{
	struct sock_triggered_context *trigger_context;

	if (pe_entry->completion_reported)
		return;

	if (!(pe_entry->flags & SOCK_TRIGGERED_OP)) {
		sock_pe_report_recv_cq_completion(pe_entry);
		if (pe_entry->comp->recv_cntr)
			sock_cntr_inc(pe_entry->comp->recv_cntr);
	} else {
		trigger_context = (void *) (uintptr_t) pe_entry->context;
		fi_cntr_add(trigger_context->trigger.work.completion_cntr, 1);
	}
	pe_entry->completion_reported = 1;
}

static void sock_pe_report_mr_completion(struct sock_domain *domain,
				  struct sock_pe_entry *pe_entry)
{
	int i;
	struct sock_mr *mr;

	for (i = 0; i < pe_entry->msg_hdr.dest_iov_len; i++) {
		fastlock_acquire(&domain->lock);
		mr = ofi_mr_map_get(&domain->mr_map,
				    pe_entry->pe.rx.rx_iov[i].iov.key);
		fastlock_release(&domain->lock);
		if (!mr || (!mr->cq && !mr->cntr))
			continue;

		pe_entry->buf = pe_entry->pe.rx.rx_iov[i].iov.addr;
		pe_entry->data_len = pe_entry->pe.rx.rx_iov[i].iov.len;

		if (mr->cntr)
			sock_cntr_inc(mr->cntr);
	}
}

static void sock_pe_report_remote_write(struct sock_rx_ctx *rx_ctx,
				 struct sock_pe_entry *pe_entry)
{
	pe_entry->buf = pe_entry->pe.rx.rx_iov[0].iov.addr;
	pe_entry->data_len = pe_entry->pe.rx.rx_iov[0].iov.len;

	if (pe_entry->flags & FI_REMOTE_CQ_DATA) {
		sock_pe_report_recv_cq_completion(pe_entry);
	}

	if ((!pe_entry->comp->rem_write_cntr &&
	     !(pe_entry->msg_hdr.flags & FI_REMOTE_WRITE)))
		return;

	if (pe_entry->comp->rem_write_cntr)
		sock_cntr_inc(pe_entry->comp->rem_write_cntr);
}

static void sock_pe_report_write_completion(struct sock_pe_entry *pe_entry)
{
	struct sock_triggered_context *trigger_context;

	if (pe_entry->completion_reported)
		return;

	if (!(pe_entry->flags & SOCK_NO_COMPLETION))
		sock_pe_report_send_cq_completion(pe_entry);

	if (!(pe_entry->flags & SOCK_TRIGGERED_OP)) {
		if (pe_entry->comp->write_cntr)
			sock_cntr_inc(pe_entry->comp->write_cntr);
	} else {
		trigger_context = (void *) (uintptr_t) pe_entry->context;
		fi_cntr_add(trigger_context->trigger.work.completion_cntr, 1);
	}
	pe_entry->completion_reported = 1;
}

static void sock_pe_report_remote_read(struct sock_rx_ctx *rx_ctx,
				struct sock_pe_entry *pe_entry)
{
	pe_entry->buf = pe_entry->pe.rx.rx_iov[0].iov.addr;
	pe_entry->data_len = pe_entry->pe.rx.rx_iov[0].iov.len;

	if ((!pe_entry->comp->rem_read_cntr &&
	     !(pe_entry->msg_hdr.flags & FI_REMOTE_READ)))
		return;

	if (pe_entry->comp->rem_read_cntr)
		sock_cntr_inc(pe_entry->comp->rem_read_cntr);
}

static void sock_pe_report_read_completion(struct sock_pe_entry *pe_entry)
{
	struct sock_triggered_context *trigger_context;

	if (pe_entry->completion_reported)
		return;

	if (!(pe_entry->flags & SOCK_NO_COMPLETION))
		sock_pe_report_send_cq_completion(pe_entry);

	if (!(pe_entry->flags & SOCK_TRIGGERED_OP)) {
		if (pe_entry->comp->read_cntr)
			sock_cntr_inc(pe_entry->comp->read_cntr);
	} else {
		trigger_context = (void *) (uintptr_t) pe_entry->context;
		fi_cntr_add(trigger_context->trigger.work.completion_cntr, 1);
	}
	pe_entry->completion_reported = 1;
}

static void sock_pe_report_rx_error(struct sock_pe_entry *pe_entry, int rem, int err)
{
	if (pe_entry->completion_reported)
		return;

	if (pe_entry->comp->recv_cntr)
		fi_cntr_adderr(&pe_entry->comp->recv_cntr->cntr_fid, 1);
	if (pe_entry->comp->recv_cq)
		sock_cq_report_error(pe_entry->comp->recv_cq, pe_entry, rem,
				     err, -err, NULL, 0);
	pe_entry->completion_reported = 1;
}

static void sock_pe_report_tx_error(struct sock_pe_entry *pe_entry, int rem, int err)
{
	if (pe_entry->completion_reported)
		return;

	if (pe_entry->comp->send_cntr)
		fi_cntr_adderr(&pe_entry->comp->send_cntr->cntr_fid, 1);
	if (pe_entry->comp->send_cq)
		sock_cq_report_error(pe_entry->comp->send_cq, pe_entry, rem,
				     err, -err, NULL, 0);
	pe_entry->completion_reported = 1;
}

static void sock_pe_report_tx_rma_read_err(struct sock_pe_entry *pe_entry,
						int err)
{
	if (pe_entry->completion_reported)
		return;

	if (pe_entry->comp->read_cntr)
		fi_cntr_adderr(&pe_entry->comp->read_cntr->cntr_fid, 1);
	if (pe_entry->comp->send_cq)
		sock_cq_report_error(pe_entry->comp->send_cq, pe_entry, 0,
				     err, -err, NULL, 0);
	pe_entry->completion_reported = 1;
}

static void sock_pe_report_tx_rma_write_err(struct sock_pe_entry *pe_entry,
						int err)
{
	if (pe_entry->completion_reported)
		return;

	if (pe_entry->comp->write_cntr)
		fi_cntr_adderr(&pe_entry->comp->write_cntr->cntr_fid, 1);
	if (pe_entry->comp->send_cq)
		sock_cq_report_error(pe_entry->comp->send_cq, pe_entry, 0,
			 	     err, -err, NULL, 0);
	pe_entry->completion_reported = 1;
}

static void sock_pe_progress_pending_ack(struct sock_pe *pe,
					 struct sock_pe_entry *pe_entry)
{
	int len, data_len, i;
	struct sock_conn *conn = pe_entry->conn;

	if (!conn || pe_entry->rem)
		return;

	if (conn->tx_pe_entry != NULL && conn->tx_pe_entry != pe_entry) {
		SOCK_LOG_DBG("Cannot progress %p as conn %p is being used by %p\n",
			      pe_entry, conn, conn->tx_pe_entry);
		return;
	}

	if (conn->tx_pe_entry == NULL) {
		SOCK_LOG_DBG("Connection %p grabbed by %p\n", conn, pe_entry);
		conn->tx_pe_entry = pe_entry;
	}

	if (sock_pe_send_field(pe_entry, &pe_entry->response,
			       sizeof(pe_entry->response), 0))
		return;
	len = sizeof(struct sock_msg_response);

	switch (pe_entry->response.msg_hdr.op_type) {
	case SOCK_OP_READ_COMPLETE:
		for (i = 0; i < pe_entry->msg_hdr.dest_iov_len; i++) {
			if (sock_pe_send_field(
				    pe_entry,
				    (char *) (uintptr_t) pe_entry->pe.rx.rx_iov[i].iov.addr,
				    pe_entry->pe.rx.rx_iov[i].iov.len, len))
				return;
			len += pe_entry->pe.rx.rx_iov[i].iov.len;
		}
		break;

	case SOCK_OP_ATOMIC_COMPLETE:
		data_len = pe_entry->total_len - len;
		if (data_len) {
			if (sock_pe_send_field(pe_entry,
					       pe_entry->pe.rx.atomic_cmp,
					       data_len, len))
				return;
			len += data_len;
		}
		break;

	default:
		break;
	}

	if (pe_entry->total_len == pe_entry->done_len && !pe_entry->rem) {
		sock_comm_flush(pe_entry);
		if (!sock_comm_tx_done(pe_entry))
			return;
		pe_entry->is_complete = 1;
		pe_entry->pe.rx.pending_send = 0;
		pe_entry->conn->tx_pe_entry = NULL;
	}
}

static void sock_pe_send_response(struct sock_pe *pe,
				  struct sock_rx_ctx *rx_ctx,
				  struct sock_pe_entry *pe_entry,
				  size_t data_len, uint8_t op_type, int err)
{
	struct sock_msg_response *response = &pe_entry->response;
	memset(response, 0, sizeof(struct sock_msg_response));

	response->pe_entry_id = htons(pe_entry->msg_hdr.pe_entry_id);
	response->err = htonl(err);
	response->msg_hdr.dest_iov_len = 0;
	response->msg_hdr.flags = 0;
	response->msg_hdr.msg_len = sizeof(*response) + data_len;
	response->msg_hdr.version = SOCK_WIRE_PROTO_VERSION;
	response->msg_hdr.op_type = op_type;
	response->msg_hdr.msg_len = htonll(response->msg_hdr.msg_len);
	response->msg_hdr.rx_id = pe_entry->msg_hdr.rx_id;

	pe->pe_atomic = NULL;
	pe_entry->done_len = 0;
	pe_entry->pe.rx.pending_send = 1;
	if (pe_entry->rem == 0)
		pe_entry->conn->rx_pe_entry = NULL;
	pe_entry->total_len = sizeof(*response) + data_len;

	sock_pe_progress_pending_ack(pe, pe_entry);
}

static inline int sock_pe_read_response(struct sock_pe_entry *pe_entry)
{
	int ret, len, data_len;

	if (pe_entry->done_len >= sizeof(struct sock_msg_response))
		return 0;

	len = sizeof(struct sock_msg_hdr);
	data_len = sizeof(struct sock_msg_response) - len;
	ret = sock_pe_recv_field(pe_entry, &pe_entry->response.pe_entry_id,
					data_len, len);
	if (ret)
		return ret;
	pe_entry->response.pe_entry_id = ntohs(pe_entry->response.pe_entry_id);
	pe_entry->response.err = ntohl(pe_entry->response.err);
	return 0;
}

static int sock_pe_handle_ack(struct sock_pe *pe,
			struct sock_pe_entry *pe_entry)
{
	struct sock_pe_entry *waiting_entry;
	struct sock_msg_response *response;

	if (sock_pe_read_response(pe_entry))
		return 0;

	response = &pe_entry->response;
	assert(response->pe_entry_id <= SOCK_PE_MAX_ENTRIES);
	waiting_entry = &pe->pe_table[response->pe_entry_id];
	SOCK_LOG_DBG("Received ack for PE entry %p (index: %d)\n",
		      waiting_entry, response->pe_entry_id);

	assert(waiting_entry->type == SOCK_PE_TX);
	sock_pe_report_send_completion(waiting_entry);
	waiting_entry->is_complete = 1;
	pe_entry->is_complete = 1;
	return 0;
}

static int sock_pe_handle_error(struct sock_pe *pe,
				struct sock_pe_entry *pe_entry)
{
	struct sock_pe_entry *waiting_entry;
	struct sock_msg_response *response;

	if (sock_pe_read_response(pe_entry))
		return 0;

	response = &pe_entry->response;
	assert(response->pe_entry_id <= SOCK_PE_MAX_ENTRIES);
	waiting_entry = &pe->pe_table[response->pe_entry_id];
	SOCK_LOG_ERROR("Received error for PE entry %p (index: %d)\n",
		      waiting_entry, response->pe_entry_id);

	assert(waiting_entry->type == SOCK_PE_TX);

	switch (pe_entry->msg_hdr.op_type) {
	case SOCK_OP_READ_ERROR:
		sock_pe_report_tx_rma_read_err(waiting_entry,
					       pe_entry->response.err);
		break;
	case SOCK_OP_WRITE_ERROR:
	case SOCK_OP_ATOMIC_ERROR:
		sock_pe_report_tx_rma_write_err(waiting_entry,
						pe_entry->response.err);
		break;
	default:
		SOCK_LOG_ERROR("Invalid op type\n");
	}
	waiting_entry->is_complete = 1;
	pe_entry->is_complete = 1;
	return 0;
}

static int sock_pe_handle_read_complete(struct sock_pe *pe,
					struct sock_pe_entry *pe_entry)
{
	struct sock_pe_entry *waiting_entry;
	struct sock_msg_response *response;
	int len, i;

	if (sock_pe_read_response(pe_entry))
		return 0;

	response = &pe_entry->response;
	assert(response->pe_entry_id <= SOCK_PE_MAX_ENTRIES);
	waiting_entry = &pe->pe_table[response->pe_entry_id];
	SOCK_LOG_DBG("Received read complete for PE entry %p (index: %d)\n",
		      waiting_entry, response->pe_entry_id);

	waiting_entry = &pe->pe_table[response->pe_entry_id];
	assert(waiting_entry->type == SOCK_PE_TX);

	len = sizeof(struct sock_msg_response);
	for (i = 0; i < waiting_entry->pe.tx.tx_op.dest_iov_len; i++) {
		if (sock_pe_recv_field(
			    pe_entry,
			    (char *) (uintptr_t) waiting_entry->pe.tx.tx_iov[i].dst.iov.addr,
			    waiting_entry->pe.tx.tx_iov[i].dst.iov.len, len))
			return 0;
		len += waiting_entry->pe.tx.tx_iov[i].dst.iov.len;
	}

	sock_pe_report_read_completion(waiting_entry);
	waiting_entry->is_complete = 1;
	pe_entry->is_complete = 1;
	return 0;
}

static int sock_pe_handle_write_complete(struct sock_pe *pe,
					struct sock_pe_entry *pe_entry)
{
	struct sock_pe_entry *waiting_entry;
	struct sock_msg_response *response;

	if (sock_pe_read_response(pe_entry))
		return 0;

	response = &pe_entry->response;
	assert(response->pe_entry_id <= SOCK_PE_MAX_ENTRIES);
	waiting_entry = &pe->pe_table[response->pe_entry_id];
	SOCK_LOG_DBG("Received ack for PE entry %p (index: %d)\n",
		      waiting_entry, response->pe_entry_id);

	assert(waiting_entry->type == SOCK_PE_TX);
	sock_pe_report_write_completion(waiting_entry);
	waiting_entry->is_complete = 1;
	pe_entry->is_complete = 1;
	return 0;
}

static int sock_pe_handle_atomic_complete(struct sock_pe *pe,
					  struct sock_pe_entry *pe_entry)
{
	size_t datatype_sz;
	struct sock_pe_entry *waiting_entry;
	struct sock_msg_response *response;
	int len, i;

	if (sock_pe_read_response(pe_entry))
		return 0;

	response = &pe_entry->response;
	assert(response->pe_entry_id <= SOCK_PE_MAX_ENTRIES);
	waiting_entry = &pe->pe_table[response->pe_entry_id];
	SOCK_LOG_DBG("Received atomic complete for PE entry %p (index: %d)\n",
		      waiting_entry, response->pe_entry_id);

	waiting_entry = &pe->pe_table[response->pe_entry_id];
	assert(waiting_entry->type == SOCK_PE_TX);

	len = sizeof(struct sock_msg_response);
	datatype_sz = ofi_datatype_size(waiting_entry->pe.tx.tx_op.atomic.datatype);
	for (i = 0; i < waiting_entry->pe.tx.tx_op.atomic.res_iov_len; i++) {
		if (sock_pe_recv_field(
			    pe_entry,
			    (char *) (uintptr_t) waiting_entry->pe.tx.tx_iov[i].res.ioc.addr,
			    waiting_entry->pe.tx.tx_iov[i].res.ioc.count * datatype_sz,
			    len))
			return 0;
		len += (waiting_entry->pe.tx.tx_iov[i].res.ioc.count * datatype_sz);
	}

	if (waiting_entry->pe.rx.rx_op.atomic.res_iov_len)
		sock_pe_report_read_completion(waiting_entry);
	else
		sock_pe_report_write_completion(waiting_entry);

	waiting_entry->is_complete = 1;
	pe_entry->is_complete = 1;
	return 0;
}

static int sock_pe_process_rx_read(struct sock_pe *pe,
					struct sock_rx_ctx *rx_ctx,
					struct sock_pe_entry *pe_entry)
{
	int i;
	struct sock_mr *mr;
	uint64_t len, entry_len, data_len;

	len = sizeof(struct sock_msg_hdr);
	entry_len = sizeof(union sock_iov) * pe_entry->msg_hdr.dest_iov_len;
	if (sock_pe_recv_field(pe_entry, &pe_entry->pe.rx.rx_iov[0],
			       entry_len, len))
		return 0;
	len += entry_len;

	/* verify mr */
	data_len = 0;
	for (i = 0; i < pe_entry->msg_hdr.dest_iov_len && !pe_entry->mr_checked; i++) {

		mr = sock_mr_verify_key(rx_ctx->domain,
					pe_entry->pe.rx.rx_iov[i].iov.key,
					(uintptr_t *) &pe_entry->pe.rx.rx_iov[i].iov.addr,
					pe_entry->pe.rx.rx_iov[i].iov.len,
					FI_REMOTE_READ);
		if (!mr) {
			SOCK_LOG_ERROR("Remote memory access error: %p, %zu, %" PRIu64 "\n",
				       (void *) (uintptr_t) pe_entry->pe.rx.rx_iov[i].iov.addr,
				       pe_entry->pe.rx.rx_iov[i].iov.len,
				       pe_entry->pe.rx.rx_iov[i].iov.key);
			pe_entry->is_error = 1;
			pe_entry->rem = pe_entry->total_len - pe_entry->done_len;
			sock_pe_send_response(pe, rx_ctx, pe_entry, 0,
					      SOCK_OP_READ_ERROR, FI_EACCES);
			return 0;
		}

		data_len += pe_entry->pe.rx.rx_iov[i].iov.len;
	}
	pe_entry->mr_checked = 1;
	pe_entry->buf = pe_entry->pe.rx.rx_iov[0].iov.addr;
	pe_entry->data_len = data_len;
	pe_entry->flags |= (FI_RMA | FI_REMOTE_READ);
	if (!pe_entry->completion_reported) {
		sock_pe_report_remote_read(rx_ctx, pe_entry);
		pe_entry->completion_reported = 1;
	}
	sock_pe_send_response(pe, rx_ctx, pe_entry, data_len,
			      SOCK_OP_READ_COMPLETE, 0);
	return 0;
}

static int sock_pe_process_rx_write(struct sock_pe *pe,
					struct sock_rx_ctx *rx_ctx,
					struct sock_pe_entry *pe_entry)
{
	int i, ret = 0;
	struct sock_mr *mr;
	uint64_t rem, len, entry_len;

	len = sizeof(struct sock_msg_hdr);
	if (pe_entry->msg_hdr.flags & FI_REMOTE_CQ_DATA) {
		if (sock_pe_recv_field(pe_entry, &pe_entry->data,
				       SOCK_CQ_DATA_SIZE, len))
			return 0;
		len += SOCK_CQ_DATA_SIZE;
	}

	entry_len = sizeof(union sock_iov) * pe_entry->msg_hdr.dest_iov_len;
	if (sock_pe_recv_field(pe_entry, &pe_entry->pe.rx.rx_iov[0], entry_len, len))
		return 0;
	len += entry_len;

	for (i = 0; i < pe_entry->msg_hdr.dest_iov_len && !pe_entry->mr_checked; i++) {
		mr = sock_mr_verify_key(rx_ctx->domain,
					pe_entry->pe.rx.rx_iov[i].iov.key,
					(uintptr_t *) &pe_entry->pe.rx.rx_iov[i].iov.addr,
					pe_entry->pe.rx.rx_iov[i].iov.len,
					FI_REMOTE_WRITE);
		if (!mr) {
			SOCK_LOG_ERROR("Remote memory access error: %p, %zu, %" PRIu64 "\n",
				       (void *) (uintptr_t) pe_entry->pe.rx.rx_iov[i].iov.addr,
				       pe_entry->pe.rx.rx_iov[i].iov.len,
				       pe_entry->pe.rx.rx_iov[i].iov.key);
			pe_entry->is_error = 1;
			pe_entry->rem = pe_entry->total_len - pe_entry->done_len;
			sock_pe_send_response(pe, rx_ctx, pe_entry, 0,
					      SOCK_OP_WRITE_ERROR, FI_EACCES);
			return 0;
		}
	}
	pe_entry->mr_checked = 1;

	rem = pe_entry->msg_hdr.msg_len - len;
	for (i = 0; rem > 0 && i < pe_entry->msg_hdr.dest_iov_len; i++) {
		if (sock_pe_recv_field(pe_entry,
				       (void *) (uintptr_t) pe_entry->pe.rx.rx_iov[i].iov.addr,
				       pe_entry->pe.rx.rx_iov[i].iov.len, len))
			return 0;
		len += pe_entry->pe.rx.rx_iov[i].iov.len;
		rem -= pe_entry->pe.rx.rx_iov[i].iov.len;
	}
	pe_entry->buf = pe_entry->pe.rx.rx_iov[0].iov.addr;
	pe_entry->data_len = 0;
	for (i = 0; i < pe_entry->msg_hdr.dest_iov_len; i++) {
		pe_entry->data_len += pe_entry->pe.rx.rx_iov[i].iov.len;
		if ((pe_entry->msg_hdr.flags & FI_COMMIT_COMPLETE) &&
		    ofi_pmem_commit) {
			(*ofi_pmem_commit)((const void *) (uintptr_t)
					   pe_entry->pe.rx.rx_iov[i].iov.addr,
					   pe_entry->pe.rx.rx_iov[i].iov.len);
		}
	}

	/* report error, if any */
	if (rem) {
		sock_pe_report_rx_error(pe_entry, rem, FI_ETRUNC);
		goto out;
	}

out:
	pe_entry->flags |= (FI_RMA | FI_REMOTE_WRITE);
	if (!pe_entry->completion_reported) {
		sock_pe_report_remote_write(rx_ctx, pe_entry);
		sock_pe_report_mr_completion(rx_ctx->domain, pe_entry);
		pe_entry->completion_reported = 1;
	}
	sock_pe_send_response(pe, rx_ctx, pe_entry, 0,
			      SOCK_OP_WRITE_COMPLETE, 0);
	return ret;
}

/*
 * Provider re-uses compare buffer to return result.  This can be optimized
 * in the future to have a separate buffer.
 */
static void sock_pe_do_atomic(void *cmp, void *dst, void *src,
			      enum fi_datatype datatype, enum fi_op op,
			      size_t cnt, int fetch)
{
	char tmp_result[SOCK_EP_MAX_ATOMIC_SZ];

	if (op >= OFI_SWAP_OP_START) {
		ofi_atomic_swap_handlers[op - OFI_SWAP_OP_START][datatype](dst,
			src, cmp, tmp_result, cnt);
                if (cmp != NULL)
			memcpy(cmp, tmp_result, ofi_datatype_size(datatype) * cnt);
	} else if (fetch) {
		ofi_atomic_readwrite_handlers[op][datatype](dst, src,
			cmp /*results*/, cnt);
	} else {
		ofi_atomic_write_handlers[op][datatype](dst, src, cnt);
	}
}

static int sock_pe_recv_atomic_hdrs(struct sock_pe *pe,
				    struct sock_pe_entry *pe_entry,
				    size_t *datatype_sz, uint64_t *entry_len)
{
	uint64_t len;
	int i;

	if (!pe_entry->pe.rx.atomic_cmp) {
		pe_entry->pe.rx.atomic_cmp = ofi_buf_alloc(pe->atomic_rx_pool);
		pe_entry->pe.rx.atomic_src = ofi_buf_alloc(pe->atomic_rx_pool);
		if (!pe_entry->pe.rx.atomic_cmp || !pe_entry->pe.rx.atomic_src)
			return -FI_ENOMEM;
	}

	len = sizeof(struct sock_msg_hdr);
	if (sock_pe_recv_field(pe_entry, &pe_entry->pe.rx.rx_op,
			       sizeof(struct sock_op), len))
		return -FI_EAGAIN;
	len += sizeof(struct sock_op);

	if (pe_entry->msg_hdr.flags & FI_REMOTE_CQ_DATA) {
		if (sock_pe_recv_field(pe_entry, &pe_entry->data,
				       SOCK_CQ_DATA_SIZE, len))
			return -FI_EAGAIN;
		len += SOCK_CQ_DATA_SIZE;
	}

	/* dst iocs */
	*entry_len = sizeof(union sock_iov) * pe_entry->pe.rx.rx_op.dest_iov_len;
	if (sock_pe_recv_field(pe_entry, &pe_entry->pe.rx.rx_iov[0],
			       *entry_len, len))
		return -FI_EAGAIN;
	len += *entry_len;

	*entry_len = 0;
	*datatype_sz = ofi_datatype_size(pe_entry->pe.rx.rx_op.atomic.datatype);
	for (i = 0; i < pe_entry->pe.rx.rx_op.dest_iov_len; i++) {
		*entry_len += pe_entry->pe.rx.rx_iov[i].ioc.count;
	}
	*entry_len *= *datatype_sz;

	/* cmp data */
	if (pe_entry->pe.rx.rx_op.atomic.cmp_iov_len) {
		if (sock_pe_recv_field(pe_entry, pe_entry->pe.rx.atomic_cmp,
				       *entry_len, len))
			return -FI_EAGAIN;
		len += *entry_len;
	}

	/* src data */
	if (pe_entry->pe.rx.rx_op.atomic.op != FI_ATOMIC_READ &&
	    pe_entry->pe.rx.rx_op.src_iov_len) {
		if (sock_pe_recv_field(pe_entry, pe_entry->pe.rx.atomic_src,
				       *entry_len, len))
			return -FI_EAGAIN;
		len += *entry_len;
	}

	return 0;
}

static int sock_pe_process_rx_atomic(struct sock_pe *pe,
				struct sock_rx_ctx *rx_ctx,
				struct sock_pe_entry *pe_entry)
{
	int i, ret = 0;
	size_t datatype_sz;
	struct sock_mr *mr;
	uint64_t offset, entry_len;

	ret = sock_pe_recv_atomic_hdrs(pe, pe_entry, &datatype_sz, &entry_len);
	if (ret)
		return ret == -FI_EAGAIN ? 0 : ret;

	for (i = 0; i < pe_entry->pe.rx.rx_op.dest_iov_len && !pe_entry->mr_checked; i++) {
		mr = sock_mr_verify_key(rx_ctx->domain,
					pe_entry->pe.rx.rx_iov[i].ioc.key,
					(uintptr_t *) &pe_entry->pe.rx.rx_iov[i].ioc.addr,
					pe_entry->pe.rx.rx_iov[i].ioc.count * datatype_sz,
					FI_REMOTE_WRITE);
		if (!mr) {
			SOCK_LOG_ERROR("Remote memory access error: %p, %zu, %" PRIu64 "\n",
				       (void *) (uintptr_t) pe_entry->pe.rx.rx_iov[i].ioc.addr,
				       pe_entry->pe.rx.rx_iov[i].ioc.count * datatype_sz,
				       pe_entry->pe.rx.rx_iov[i].ioc.key);
			pe_entry->is_error = 1;
			pe_entry->rem = pe_entry->total_len - pe_entry->done_len;
			sock_pe_send_response(pe, rx_ctx, pe_entry, 0,
					      SOCK_OP_ATOMIC_ERROR, FI_EACCES);
			return 0;
		}
	}
	pe_entry->mr_checked = 1;

	if (pe->pe_atomic) {
		if (pe->pe_atomic != pe_entry)
			return 0;
	} else {
		pe->pe_atomic = pe_entry;
	}

	offset = 0;
	for (i = 0; i < pe_entry->pe.rx.rx_op.dest_iov_len; i++) {
		sock_pe_do_atomic(pe_entry->pe.rx.atomic_cmp + offset,
			(char *) (uintptr_t) pe_entry->pe.rx.rx_iov[i].ioc.addr,
			pe_entry->pe.rx.atomic_src + offset,
			pe_entry->pe.rx.rx_op.atomic.datatype,
			pe_entry->pe.rx.rx_op.atomic.op,
			pe_entry->pe.rx.rx_iov[i].ioc.count,
			pe_entry->pe.rx.rx_op.atomic.res_iov_len);
		offset += datatype_sz * pe_entry->pe.rx.rx_iov[i].ioc.count;
	}

	pe_entry->buf = pe_entry->pe.rx.rx_iov[0].iov.addr;
	pe_entry->data_len = offset;

	pe_entry->flags |= FI_ATOMIC;
	if (pe_entry->pe.rx.rx_op.atomic.op == FI_ATOMIC_READ)
		pe_entry->flags |= FI_REMOTE_READ;
	else
		pe_entry->flags |= FI_REMOTE_WRITE;

	if (!pe_entry->completion_reported) {
		sock_pe_report_remote_write(rx_ctx, pe_entry);
		sock_pe_report_mr_completion(rx_ctx->domain, pe_entry);
		pe_entry->completion_reported = 1;
	}
	sock_pe_send_response(pe, rx_ctx, pe_entry,
			      pe_entry->pe.rx.rx_op.atomic.res_iov_len ?
			      entry_len : 0, SOCK_OP_ATOMIC_COMPLETE, 0);
	return ret;
}

/*
 * For simplicity, we treat all tagged atomics as buffered.  This would need
 * to change if we wanted to report back possible error data or handle
 * atomic fetch operations.
 */
static int
sock_pe_process_rx_tatomic(struct sock_pe *pe, struct sock_rx_ctx *rx_ctx,
			   struct sock_pe_entry *pe_entry)
{
	int ret = 0;
	size_t datatype_sz;
	uint64_t entry_len;
	struct sock_rx_entry *rx_entry;

	ret = sock_pe_recv_atomic_hdrs(pe, pe_entry, &datatype_sz, &entry_len);
	if (ret)
		return ret == -FI_EAGAIN ? 0 : ret;

	assert(pe_entry->pe.rx.rx_iov[0].ioc.addr == 0);
	assert(pe_entry->pe.rx.rx_op.dest_iov_len == 1);
	assert(pe_entry->pe.rx.rx_op.atomic.cmp_iov_len == 0);

	pe_entry->tag = pe_entry->pe.rx.rx_iov[0].ioc.key;
	pe_entry->data_len = entry_len;

	fastlock_acquire(&rx_ctx->lock);
	rx_entry = sock_rx_new_buffered_entry(rx_ctx, entry_len);
	if (!rx_entry) {
		fastlock_release(&rx_ctx->lock);
		return -FI_ENOMEM;
	}

	rx_entry->rx_op = pe_entry->pe.rx.rx_op;
	memcpy((void *) (uintptr_t) rx_entry->iov[0].ioc.addr,
		pe_entry->pe.rx.atomic_src, entry_len);
	rx_entry->addr = pe_entry->addr;
	rx_entry->tag = pe_entry->tag;
	rx_entry->data = pe_entry->data;
	rx_entry->ignore = 0;
	rx_entry->comp = pe_entry->comp;
	rx_entry->is_complete = 1;

	if (pe_entry->msg_hdr.flags & FI_REMOTE_CQ_DATA)
		rx_entry->flags |= FI_REMOTE_CQ_DATA;
	rx_entry->flags |= FI_TAGGED | FI_ATOMIC;
	rx_entry->is_tagged = 1;

	pe_entry->pe.rx.rx_entry = rx_entry;

	sock_pe_progress_buffered_rx(rx_ctx);
	fastlock_release(&rx_ctx->lock);

	pe_entry->is_complete = 1;

	sock_pe_send_response(pe, rx_ctx, pe_entry, 0,
			      SOCK_OP_ATOMIC_COMPLETE, 0);
	return ret;
}

ssize_t sock_rx_peek_recv(struct sock_rx_ctx *rx_ctx, fi_addr_t addr,
			  uint64_t tag, uint64_t ignore, void *context,
			  uint64_t flags, uint8_t is_tagged)
{
	struct sock_rx_entry *rx_buffered;
	struct sock_pe_entry pe_entry;

	fastlock_acquire(&rx_ctx->lock);
	rx_buffered = sock_rx_get_buffered_entry(rx_ctx,
					(rx_ctx->attr.caps & FI_DIRECTED_RECV) ?
						 addr : FI_ADDR_UNSPEC,
						 tag, ignore, is_tagged);

	memset(&pe_entry, 0, sizeof(pe_entry));
	pe_entry.comp = &rx_ctx->comp;
	pe_entry.context = (uintptr_t)context;
	pe_entry.flags = (flags | FI_MSG | FI_RECV);
	if (is_tagged)
		pe_entry.flags |= FI_TAGGED;

	if (rx_buffered) {
		pe_entry.data_len = rx_buffered->total_len;
		pe_entry.tag = rx_buffered->tag;
		pe_entry.data = rx_buffered->data;
		rx_buffered->context = (uintptr_t)context;
		if (flags & FI_CLAIM)
			rx_buffered->is_claimed = 1;

		if (flags & FI_DISCARD) {
			dlist_remove(&rx_buffered->entry);
			sock_rx_release_entry(rx_buffered);
		}
		sock_pe_report_recv_completion(&pe_entry);
	} else {
		sock_cq_report_error(rx_ctx->comp.recv_cq, &pe_entry, 0,
				     FI_ENOMSG, -FI_ENOMSG, NULL, 0);
	}
	fastlock_release(&rx_ctx->lock);
	return 0;
}

ssize_t sock_rx_claim_recv(struct sock_rx_ctx *rx_ctx, void *context,
			uint64_t flags, uint64_t tag, uint64_t ignore,
			uint8_t is_tagged, const struct iovec *msg_iov,
			size_t iov_count)
{
	ssize_t ret = 0;
	size_t rem = 0, i, offset, len;
	struct dlist_entry *entry;
	struct sock_pe_entry pe_entry;
	struct sock_rx_entry *rx_buffered = NULL;

	fastlock_acquire(&rx_ctx->lock);
	for (entry = rx_ctx->rx_buffered_list.next;
	     entry != &rx_ctx->rx_buffered_list; entry = entry->next) {
		rx_buffered = container_of(entry, struct sock_rx_entry, entry);
		if (rx_buffered->is_claimed &&
		    (uintptr_t)rx_buffered->context == (uintptr_t)context &&
		    is_tagged == rx_buffered->is_tagged &&
		    (tag & ~ignore) == (rx_buffered->tag & ~ignore))
			break;
		else
			rx_buffered = NULL;
	}

	if (rx_buffered) {
		memset(&pe_entry, 0, sizeof(pe_entry));
		pe_entry.comp = &rx_ctx->comp;
		pe_entry.data_len = rx_buffered->total_len;
		pe_entry.tag = rx_buffered->tag;
		pe_entry.data = rx_buffered->data;
		pe_entry.context = rx_buffered->context;
		pe_entry.flags = (flags | FI_MSG | FI_RECV);
		pe_entry.addr = rx_buffered->addr;
		if (is_tagged)
			pe_entry.flags |= FI_TAGGED;

		if (!(flags & FI_DISCARD)) {
			pe_entry.buf = (uintptr_t)msg_iov[0].iov_base;
			offset = 0;
			rem = rx_buffered->total_len;
			for (i = 0; i < iov_count && rem > 0; i++) {
				len = MIN(msg_iov[i].iov_len, rem);
				memcpy(msg_iov[i].iov_base,
				       (char *) (uintptr_t)
				       rx_buffered->iov[0].iov.addr + offset, len);
				rem -= len;
				offset += len;
			}
		}

		if (rem) {
			SOCK_LOG_DBG("Not enough space in posted recv buffer\n");
			sock_pe_report_rx_error(&pe_entry, rem, FI_ETRUNC);
		} else {
			sock_pe_report_recv_completion(&pe_entry);
		}

		dlist_remove(&rx_buffered->entry);
		sock_rx_release_entry(rx_buffered);
	} else {
		ret = -FI_ENOMSG;
	}

	fastlock_release(&rx_ctx->lock);
	return ret;
}

static int sock_pe_progress_buffered_rx(struct sock_rx_ctx *rx_ctx)
{
	struct dlist_entry *entry;
	struct sock_pe_entry pe_entry;
	struct sock_rx_entry *rx_buffered, *rx_posted;
	size_t i, rem = 0, offset, len, used_len, dst_offset, datatype_sz;
	char *src, *dst;

	if (dlist_empty(&rx_ctx->rx_entry_list) ||
	    dlist_empty(&rx_ctx->rx_buffered_list))
		return 0;

	for (entry = rx_ctx->rx_buffered_list.next;
	     entry != &rx_ctx->rx_buffered_list;) {

		rx_buffered = container_of(entry, struct sock_rx_entry, entry);
		entry = entry->next;

		if (!rx_buffered->is_complete || rx_buffered->is_claimed)
			continue;

		rx_posted = sock_rx_get_entry(rx_ctx, rx_buffered->addr,
						rx_buffered->tag,
						rx_buffered->is_tagged);
		if (!rx_posted)
			continue;

		SOCK_LOG_DBG("Consuming buffered entry: %p, ctx: %p\n",
			      rx_buffered, rx_ctx);
		SOCK_LOG_DBG("Consuming posted entry: %p, ctx: %p\n",
			      rx_posted, rx_ctx);

		datatype_sz = (rx_buffered->flags & FI_ATOMIC) ?
			ofi_datatype_size(rx_buffered->rx_op.atomic.datatype) : 0;
		offset = 0;
		rem = rx_buffered->iov[0].iov.len;
		rx_ctx->buffered_len -= rem;
		used_len = rx_posted->used;
		memset(&pe_entry, 0, sizeof(pe_entry));
		for (i = 0; i < rx_posted->rx_op.dest_iov_len && rem > 0; i++) {
			/* Try to find the first iovec entry where the data
			 * has not been consumed. In the common case, there
			 * is only one iovec, i.e. a single buffer */
			if (used_len >= rx_posted->iov[i].iov.len) {
				used_len -= rx_posted->iov[i].iov.len;
				continue;
			}

			dst_offset = used_len;
			len = MIN(rx_posted->iov[i].iov.len, rem);
			pe_entry.buf = rx_posted->iov[i].iov.addr + dst_offset;

			src = (char *) (uintptr_t)
			      rx_buffered->iov[0].iov.addr + offset;
			dst = (char *) (uintptr_t)
			      rx_posted->iov[i].iov.addr + dst_offset;

			if (datatype_sz) {
				int cnt = len / datatype_sz;

				sock_pe_do_atomic(NULL, dst, src,
					rx_buffered->rx_op.atomic.datatype,
					rx_buffered->rx_op.atomic.op, cnt, 0);
			} else {
				memcpy(dst, src, len);
			}
			offset += len;
			rem -= len;
			dst_offset = used_len = 0;
			rx_posted->used += len;
			pe_entry.data_len = rx_buffered->used;
		}

		pe_entry.done_len = offset;
		pe_entry.data = rx_buffered->data;
		pe_entry.tag = rx_buffered->tag;
		pe_entry.context = (uint64_t)rx_posted->context;
		pe_entry.pe.rx.rx_iov[0].iov.addr = rx_posted->iov[0].iov.addr;
		pe_entry.type = SOCK_PE_RX;
		pe_entry.comp = rx_buffered->comp;
		pe_entry.flags = rx_posted->flags;
		pe_entry.flags |= (FI_MSG | FI_RECV);
		pe_entry.addr = rx_buffered->addr;
		if (rx_buffered->is_tagged)
			pe_entry.flags |= FI_TAGGED;
		pe_entry.flags &= ~FI_MULTI_RECV;

		if (rx_posted->flags & FI_MULTI_RECV) {
			if (sock_rx_avail_len(rx_posted) < rx_ctx->min_multi_recv) {
				pe_entry.flags |= FI_MULTI_RECV;
				dlist_remove(&rx_posted->entry);
			}
		} else {
			dlist_remove(&rx_posted->entry);
		}

		if (rem) {
			SOCK_LOG_DBG("Not enough space in posted recv buffer\n");
			sock_pe_report_rx_error(&pe_entry, rem, FI_ETRUNC);
		} else {
			sock_pe_report_recv_completion(&pe_entry);
		}

		/* Mark that we are done processing the posted recv buff.
		 * This allows another thread to grab it when calling
		 * sock_rx_get_entry() */
		rx_posted->is_busy = 0;

		dlist_remove(&rx_buffered->entry);
		sock_rx_release_entry(rx_buffered);

		if ((!(rx_posted->flags & FI_MULTI_RECV) ||
		     (pe_entry.flags & FI_MULTI_RECV))) {
			sock_rx_release_entry(rx_posted);
			rx_ctx->num_left++;
		}
	}
	return 0;
}

static int sock_pe_process_rx_send(struct sock_pe *pe,
				struct sock_rx_ctx *rx_ctx,
				struct sock_pe_entry *pe_entry)
{
	ssize_t i, ret = 0;
	struct sock_rx_entry *rx_entry;
	uint64_t len, rem, offset, data_len, done_data, used;

	offset = 0;
	len = sizeof(struct sock_msg_hdr);

	if (pe_entry->msg_hdr.op_type == SOCK_OP_TSEND) {
		if (sock_pe_recv_field(pe_entry, &pe_entry->tag,
				       SOCK_TAG_SIZE, len))
			return 0;
		len += SOCK_TAG_SIZE;
	}

	if (pe_entry->msg_hdr.flags & FI_REMOTE_CQ_DATA) {
		if (sock_pe_recv_field(pe_entry, &pe_entry->data,
				       SOCK_CQ_DATA_SIZE, len))
			return 0;
		len += SOCK_CQ_DATA_SIZE;
	}

	data_len = pe_entry->msg_hdr.msg_len - len;
	if (pe_entry->done_len == len && !pe_entry->pe.rx.rx_entry) {
		fastlock_acquire(&rx_ctx->lock);
		sock_pe_progress_buffered_rx(rx_ctx);

		rx_entry = sock_rx_get_entry(rx_ctx, pe_entry->addr, pe_entry->tag,
					     pe_entry->msg_hdr.op_type == SOCK_OP_TSEND ? 1 : 0);
		SOCK_LOG_DBG("Consuming posted entry: %p\n", rx_entry);

		if (!rx_entry) {
			SOCK_LOG_DBG("%p: No matching recv, buffering recv (len = %llu)\n",
				      pe_entry, (long long unsigned int)data_len);

			rx_entry = sock_rx_new_buffered_entry(rx_ctx, data_len);
			if (!rx_entry) {
				fastlock_release(&rx_ctx->lock);
				return -FI_ENOMEM;
			}

			rx_entry->addr = pe_entry->addr;
			rx_entry->tag = pe_entry->tag;
			rx_entry->data = pe_entry->data;
			rx_entry->ignore = 0;
			rx_entry->comp = pe_entry->comp;

			if (pe_entry->msg_hdr.flags & FI_REMOTE_CQ_DATA)
				rx_entry->flags |= FI_REMOTE_CQ_DATA;

			if (pe_entry->msg_hdr.op_type == SOCK_OP_TSEND)
				rx_entry->is_tagged = 1;
		}
		fastlock_release(&rx_ctx->lock);
		pe_entry->context = rx_entry->context;
		pe_entry->pe.rx.rx_entry = rx_entry;
	}

	rx_entry = pe_entry->pe.rx.rx_entry;
	done_data = pe_entry->done_len - len;
	pe_entry->data_len = data_len;
	rem = pe_entry->data_len - done_data;
	used = rx_entry->used;

	for (i = 0; rem > 0 && i < rx_entry->rx_op.dest_iov_len; i++) {

		/* skip used contents in rx_entry */
		if (used >= rx_entry->iov[i].iov.len) {
			used -= rx_entry->iov[i].iov.len;
			continue;
		}

		offset = used;
		data_len = MIN(rx_entry->iov[i].iov.len - used, rem);
		ret = sock_comm_recv(pe_entry,
				     (char *) (uintptr_t) rx_entry->iov[i].iov.addr + offset,
				     data_len);
		if (ret <= 0)
			return ret;

		if (!pe_entry->buf)
			pe_entry->buf = rx_entry->iov[i].iov.addr + offset;
		rem -= ret;
		used = 0;
		pe_entry->done_len += ret;
		rx_entry->used += ret;
		if (ret != data_len)
			return 0;
	}

	pe_entry->is_complete = 1;
	rx_entry->is_complete = 1;

	pe_entry->flags = rx_entry->flags;
	if (pe_entry->msg_hdr.op_type == SOCK_OP_TSEND)
		pe_entry->flags |= FI_TAGGED;
	pe_entry->flags |= (FI_MSG | FI_RECV);

	if (pe_entry->msg_hdr.flags & FI_REMOTE_CQ_DATA)
		pe_entry->flags |= FI_REMOTE_CQ_DATA;
	pe_entry->flags &= ~FI_MULTI_RECV;

	fastlock_acquire(&rx_ctx->lock);
	if (rx_entry->flags & FI_MULTI_RECV) {
		if (sock_rx_avail_len(rx_entry) < rx_ctx->min_multi_recv) {
			pe_entry->flags |= FI_MULTI_RECV;
			dlist_remove(&rx_entry->entry);
		}
	} else {
		if (!rx_entry->is_buffered)
			dlist_remove(&rx_entry->entry);
	}
	rx_entry->is_busy = 0;
	fastlock_release(&rx_ctx->lock);

	/* report error, if any */
	if (rem) {
		SOCK_LOG_ERROR("Not enough space in posted recv buffer\n");
		sock_pe_report_rx_error(pe_entry, rem, FI_ETRUNC);
		pe_entry->is_error = 1;
		pe_entry->rem = pe_entry->total_len - pe_entry->done_len;
		goto out;
	} else {
		if (!rx_entry->is_buffered)
			sock_pe_report_recv_completion(pe_entry);
	}

out:
	if (pe_entry->msg_hdr.flags & FI_TRANSMIT_COMPLETE) {
		sock_pe_send_response(pe, rx_ctx, pe_entry, 0,
				      SOCK_OP_SEND_COMPLETE, 0);
	}

	if (!rx_entry->is_buffered &&
	    (!(rx_entry->flags & FI_MULTI_RECV) ||
	     (pe_entry->flags & FI_MULTI_RECV))) {
		fastlock_acquire(&rx_ctx->lock);
		sock_rx_release_entry(rx_entry);
		rx_ctx->num_left++;
		fastlock_release(&rx_ctx->lock);
	}
	return ret;
}

static int sock_pe_process_rx_conn_msg(struct sock_pe *pe,
					struct sock_rx_ctx *rx_ctx,
					struct sock_pe_entry *pe_entry)
{
	uint64_t len, data_len;
	struct sock_ep_attr *ep_attr;
	struct sock_conn_map *map;
	union ofi_sock_ip *addr;
	struct sock_conn *conn;
	uint64_t index;

	if (!pe_entry->comm_addr) {
		pe_entry->comm_addr = calloc(1, sizeof(union ofi_sock_ip));
		if (!pe_entry->comm_addr)
			return -FI_ENOMEM;
	}

	len = sizeof(struct sock_msg_hdr);
	data_len = sizeof(union ofi_sock_ip);
	if (sock_pe_recv_field(pe_entry, pe_entry->comm_addr, data_len, len)) {
		return 0;
	}

	ep_attr = pe_entry->conn->ep_attr;
	map = &ep_attr->cmap;
	addr = pe_entry->comm_addr;
	pe_entry->conn->addr = *addr;

	index = (ep_attr->ep_type == FI_EP_MSG) ? 0 : sock_av_get_addr_index(ep_attr->av, addr);
	if (index != -1) {
		fastlock_acquire(&map->lock);
		conn = sock_ep_lookup_conn(ep_attr, index, addr);
		if (conn == NULL || conn == SOCK_CM_CONN_IN_PROGRESS) {
			if (ofi_idm_set(&ep_attr->av_idm, index, pe_entry->conn) < 0)
				SOCK_LOG_ERROR("ofi_idm_set failed\n");
		}
		fastlock_release(&map->lock);
	}
	pe_entry->conn->av_index = (ep_attr->ep_type == FI_EP_MSG || index == -1) ?
		FI_ADDR_NOTAVAIL : index;

	pe_entry->is_complete = 1;
	pe_entry->pe.rx.pending_send = 0;
	free(pe_entry->comm_addr);
	pe_entry->comm_addr = NULL;
	return 0;
}

static int sock_pe_process_recv(struct sock_pe *pe, struct sock_rx_ctx *rx_ctx,
				struct sock_pe_entry *pe_entry)
{
	int ret;
	struct sock_msg_hdr *msg_hdr;

	msg_hdr = &pe_entry->msg_hdr;
	if (msg_hdr->version != SOCK_WIRE_PROTO_VERSION) {
		SOCK_LOG_ERROR("Invalid wire protocol\n");
		ret = -FI_EINVAL;
		goto out;
	}

	switch (pe_entry->msg_hdr.op_type) {
	case SOCK_OP_SEND:
	case SOCK_OP_TSEND:
		ret = sock_pe_process_rx_send(pe, rx_ctx, pe_entry);
		break;
	case SOCK_OP_WRITE:
		ret = sock_pe_process_rx_write(pe, rx_ctx, pe_entry);
		break;
	case SOCK_OP_READ:
		ret = sock_pe_process_rx_read(pe, rx_ctx, pe_entry);
		break;
	case SOCK_OP_ATOMIC:
		if (msg_hdr->flags & FI_TAGGED)
			ret = sock_pe_process_rx_tatomic(pe, rx_ctx, pe_entry);
		else
			ret = sock_pe_process_rx_atomic(pe, rx_ctx, pe_entry);
		break;
	case SOCK_OP_SEND_COMPLETE:
		ret = sock_pe_handle_ack(pe, pe_entry);
		break;
	case SOCK_OP_WRITE_COMPLETE:
		ret = sock_pe_handle_write_complete(pe, pe_entry);
		break;
	case SOCK_OP_READ_COMPLETE:
		ret = sock_pe_handle_read_complete(pe, pe_entry);
		break;
	case SOCK_OP_ATOMIC_COMPLETE:
		ret = sock_pe_handle_atomic_complete(pe, pe_entry);
		break;
	case SOCK_OP_WRITE_ERROR:
	case SOCK_OP_READ_ERROR:
	case SOCK_OP_ATOMIC_ERROR:
		ret = sock_pe_handle_error(pe, pe_entry);
		break;
	case SOCK_OP_CONN_MSG:
		ret = sock_pe_process_rx_conn_msg(pe, rx_ctx, pe_entry);
		break;
	default:
		ret = -FI_ENOSYS;
		SOCK_LOG_ERROR("Operation not supported\n");
		break;
	}

out:
	return ret;
}

static int sock_pe_peek_hdr(struct sock_pe *pe,
			     struct sock_pe_entry *pe_entry)
{
	int len;
	struct sock_msg_hdr *msg_hdr;
	struct sock_conn *conn = pe_entry->conn;

	if (conn->rx_pe_entry != NULL && conn->rx_pe_entry != pe_entry)
		return -1;

	if (conn->rx_pe_entry == NULL) {
		conn->rx_pe_entry = pe_entry;
	}

	len = sizeof(struct sock_msg_hdr);
	msg_hdr = &pe_entry->msg_hdr;
	if (sock_comm_peek(pe_entry->conn, (void *) msg_hdr, len) != len)
		return -1;

	msg_hdr->msg_len = ntohll(msg_hdr->msg_len);
	msg_hdr->flags = ntohll(msg_hdr->flags);
	msg_hdr->pe_entry_id = ntohs(msg_hdr->pe_entry_id);
	pe_entry->total_len = msg_hdr->msg_len;

	SOCK_LOG_DBG("PE RX (Hdr peek): MsgLen:  %" PRIu64 ", TX-ID: %d, Type: %d\n",
		      msg_hdr->msg_len, msg_hdr->rx_id, msg_hdr->op_type);
	return 0;
}

static int sock_pe_read_hdr(struct sock_pe *pe, struct sock_rx_ctx *rx_ctx,
			     struct sock_pe_entry *pe_entry)
{
	struct sock_msg_hdr *msg_hdr;
	struct sock_conn *conn = pe_entry->conn;

	if (conn->rx_pe_entry != NULL && conn->rx_pe_entry != pe_entry)
		return 0;

	if (conn->rx_pe_entry == NULL)
		conn->rx_pe_entry = pe_entry;

	msg_hdr = &pe_entry->msg_hdr;
	if (sock_pe_peek_hdr(pe, pe_entry))
		return -1;

	if (rx_ctx->is_ctrl_ctx && sock_pe_is_data_msg(msg_hdr->op_type))
		return -1;

	if (sock_pe_is_data_msg(msg_hdr->op_type) &&
	    msg_hdr->rx_id != rx_ctx->rx_id)
		return -1;

	if (sock_pe_recv_field(pe_entry, (void *) msg_hdr,
			       sizeof(struct sock_msg_hdr), 0)) {
		SOCK_LOG_ERROR("Failed to recv header\n");
		return -1;
	}

	msg_hdr->msg_len = ntohll(msg_hdr->msg_len);
	msg_hdr->flags = ntohll(msg_hdr->flags);
	msg_hdr->pe_entry_id = ntohs(msg_hdr->pe_entry_id);
	pe_entry->pe.rx.header_read = 1;
	pe_entry->flags = msg_hdr->flags;
	pe_entry->total_len = msg_hdr->msg_len;

	SOCK_LOG_DBG("PE RX (Hdr read): MsgLen:  %" PRIu64 ", TX-ID: %d, Type: %d\n",
		      msg_hdr->msg_len, msg_hdr->rx_id, msg_hdr->op_type);
	return 0;
}

static int sock_pe_progress_tx_atomic(struct sock_pe *pe,
				      struct sock_pe_entry *pe_entry,
				      struct sock_conn *conn)
{
	int datatype_sz;
	union sock_iov iov[SOCK_EP_MAX_IOV_LIMIT];
	ssize_t len, i, entry_len;

	if (pe_entry->pe.tx.send_done)
		return 0;

	len = sizeof(struct sock_msg_hdr);
	entry_len = sizeof(struct sock_atomic_req) - sizeof(struct sock_msg_hdr);
	if (sock_pe_send_field(pe_entry, &pe_entry->pe.tx.tx_op, entry_len, len))
		return 0;
	len += entry_len;

	if (pe_entry->flags & FI_REMOTE_CQ_DATA) {
		if (sock_pe_send_field(pe_entry, &pe_entry->data,
				       SOCK_CQ_DATA_SIZE, len))
			return 0;
		len += SOCK_CQ_DATA_SIZE;
	}

	/* dest iocs */
	entry_len = sizeof(union sock_iov) * pe_entry->pe.tx.tx_op.dest_iov_len;
	for (i = 0; i < pe_entry->pe.tx.tx_op.dest_iov_len; i++) {
		iov[i].ioc.addr = pe_entry->pe.tx.tx_iov[i].dst.ioc.addr;
		iov[i].ioc.count = pe_entry->pe.tx.tx_iov[i].dst.ioc.count;
		iov[i].ioc.key = pe_entry->pe.tx.tx_iov[i].dst.ioc.key;
	}

	if (sock_pe_send_field(pe_entry, &iov[0], entry_len, len))
		return 0;
	len += entry_len;

	datatype_sz = ofi_datatype_size(pe_entry->pe.tx.tx_op.atomic.datatype);
	if (pe_entry->flags & FI_INJECT) {
		/* cmp data */
		if (sock_pe_send_field(pe_entry,
					&pe_entry->pe.tx.inject[0] + pe_entry->pe.tx.tx_op.src_iov_len,
					pe_entry->pe.tx.tx_op.atomic.cmp_iov_len, len))
			return 0;
		len += pe_entry->pe.tx.tx_op.atomic.cmp_iov_len;
		/* data */
		if (sock_pe_send_field(pe_entry,
				       &pe_entry->pe.tx.inject[0],
				       pe_entry->pe.tx.tx_op.src_iov_len, len))
			return 0;
		len += pe_entry->pe.tx.tx_op.src_iov_len;
	} else {
		/* cmp data */
		for (i = 0; i < pe_entry->pe.tx.tx_op.atomic.cmp_iov_len; i++) {
			if (sock_pe_send_field(pe_entry,
						(void *) (uintptr_t) pe_entry->pe.tx.tx_iov[i].cmp.ioc.addr,
						pe_entry->pe.tx.tx_iov[i].cmp.ioc.count *
						datatype_sz, len))
			return 0;
			len += (pe_entry->pe.tx.tx_iov[i].cmp.ioc.count * datatype_sz);
		}
		/* data */
		for (i = 0; i < pe_entry->pe.tx.tx_op.src_iov_len; i++) {
			if (pe_entry->pe.tx.tx_op.atomic.op != FI_ATOMIC_READ) {
				if (sock_pe_send_field(pe_entry,
				    (void *) (uintptr_t) pe_entry->pe.tx.tx_iov[i].src.ioc.addr,
				    pe_entry->pe.tx.tx_iov[i].src.ioc.count *
				    datatype_sz, len))
					return 0;
				len += (pe_entry->pe.tx.tx_iov[i].src.ioc.count * datatype_sz);
			}
		}
	}

	sock_comm_flush(pe_entry);
	if (!sock_comm_tx_done(pe_entry))
		return 0;

	if (pe_entry->done_len == pe_entry->total_len) {
		pe_entry->pe.tx.send_done = 1;
		pe_entry->conn->tx_pe_entry = NULL;
		SOCK_LOG_DBG("Send complete\n");
	}

	pe_entry->flags |= FI_ATOMIC;
	if (pe_entry->pe.tx.tx_op.atomic.op == FI_ATOMIC_READ)
		pe_entry->flags |= FI_READ;
	else
		pe_entry->flags |= FI_WRITE;
	pe_entry->msg_hdr.flags = pe_entry->flags;
	return 0;
}

static int sock_pe_progress_tx_write(struct sock_pe *pe,
				     struct sock_pe_entry *pe_entry,
				     struct sock_conn *conn)
{
	union sock_iov dest_iov[SOCK_EP_MAX_IOV_LIMIT];
	ssize_t len, i, dest_iov_len;

	if (pe_entry->pe.tx.send_done)
		return 0;

	len = sizeof(struct sock_msg_hdr);
	if (pe_entry->flags & FI_REMOTE_CQ_DATA) {
		if (sock_pe_send_field(pe_entry, &pe_entry->data,
				       SOCK_CQ_DATA_SIZE, len))
			return 0;
		len += SOCK_CQ_DATA_SIZE;
	}

	/* dest iovs */
	dest_iov_len = sizeof(union sock_iov) * pe_entry->pe.tx.tx_op.dest_iov_len;
	for (i = 0; i < pe_entry->pe.tx.tx_op.dest_iov_len; i++) {
		dest_iov[i].iov.addr = pe_entry->pe.tx.tx_iov[i].dst.iov.addr;
		dest_iov[i].iov.len = pe_entry->pe.tx.tx_iov[i].dst.iov.len;
		dest_iov[i].iov.key = pe_entry->pe.tx.tx_iov[i].dst.iov.key;
	}
	if (sock_pe_send_field(pe_entry, &dest_iov[0], dest_iov_len, len))
		return 0;
	len += dest_iov_len;

	/* data */
	if (pe_entry->flags & FI_INJECT) {
		if (sock_pe_send_field(pe_entry, &pe_entry->pe.tx.inject[0],
				       pe_entry->pe.tx.tx_op.src_iov_len, len))
			return 0;
		len += pe_entry->pe.tx.tx_op.src_iov_len;
		pe_entry->data_len = pe_entry->pe.tx.tx_op.src_iov_len;
	} else {
		pe_entry->data_len = 0;
		for (i = 0; i < pe_entry->pe.tx.tx_op.src_iov_len; i++) {
			if (sock_pe_send_field(
				    pe_entry,
				    (void *) (uintptr_t) pe_entry->pe.tx.tx_iov[i].src.iov.addr,
				    pe_entry->pe.tx.tx_iov[i].src.iov.len, len))
				return 0;
			len += pe_entry->pe.tx.tx_iov[i].src.iov.len;
			pe_entry->data_len += pe_entry->pe.tx.tx_iov[i].src.iov.len;
		}
	}

	sock_comm_flush(pe_entry);
	if (!sock_comm_tx_done(pe_entry))
		return 0;

	if (pe_entry->done_len == pe_entry->total_len) {
		pe_entry->pe.tx.send_done = 1;
		pe_entry->conn->tx_pe_entry = NULL;
		SOCK_LOG_DBG("Send complete\n");
	}
	pe_entry->flags |= (FI_RMA | FI_WRITE);
	pe_entry->msg_hdr.flags = pe_entry->flags;
	return 0;
}

static int sock_pe_progress_tx_read(struct sock_pe *pe,
				    struct sock_pe_entry *pe_entry,
				    struct sock_conn *conn)
{
	union sock_iov src_iov[SOCK_EP_MAX_IOV_LIMIT];
	ssize_t len, i, src_iov_len;

	if (pe_entry->pe.tx.send_done)
		return 0;

	len = sizeof(struct sock_msg_hdr);

	/* src iovs */
	src_iov_len = sizeof(union sock_iov) * pe_entry->pe.tx.tx_op.src_iov_len;
	pe_entry->data_len = 0;
	for (i = 0; i < pe_entry->pe.tx.tx_op.src_iov_len; i++) {
		src_iov[i].iov.addr = pe_entry->pe.tx.tx_iov[i].src.iov.addr;
		src_iov[i].iov.len = pe_entry->pe.tx.tx_iov[i].src.iov.len;
		src_iov[i].iov.key = pe_entry->pe.tx.tx_iov[i].src.iov.key;
		pe_entry->data_len += pe_entry->pe.tx.tx_iov[i].src.iov.len;
	}

	if (sock_pe_send_field(pe_entry, &src_iov[0], src_iov_len, len))
		return 0;
	len += src_iov_len;

	sock_comm_flush(pe_entry);
	if (!sock_comm_tx_done(pe_entry))
		return 0;

	if (pe_entry->done_len == pe_entry->total_len) {
		pe_entry->pe.tx.send_done = 1;
		pe_entry->conn->tx_pe_entry = NULL;
		SOCK_LOG_DBG("Send complete\n");
	}
	pe_entry->flags |= (FI_RMA | FI_READ);
	pe_entry->msg_hdr.flags = pe_entry->flags;
	return 0;
}


static int sock_pe_progress_tx_send(struct sock_pe *pe,
				    struct sock_pe_entry *pe_entry,
				    struct sock_conn *conn)
{
	size_t len, i;
	if (pe_entry->pe.tx.send_done)
		return 0;

	len = sizeof(struct sock_msg_hdr);
	if (pe_entry->pe.tx.tx_op.op == SOCK_OP_TSEND) {
		if (sock_pe_send_field(pe_entry, &pe_entry->tag,
				       SOCK_TAG_SIZE, len))
			return 0;
		len += SOCK_TAG_SIZE;
	}

	if (pe_entry->flags & FI_REMOTE_CQ_DATA) {
		if (sock_pe_send_field(pe_entry, &pe_entry->data,
				       SOCK_CQ_DATA_SIZE, len))
			return 0;
		len += SOCK_CQ_DATA_SIZE;
	}

	if (pe_entry->flags & FI_INJECT) {
		if (sock_pe_send_field(pe_entry, pe_entry->pe.tx.inject,
				       pe_entry->pe.tx.tx_op.src_iov_len, len))
			return 0;
		len += pe_entry->pe.tx.tx_op.src_iov_len;
		pe_entry->data_len = pe_entry->pe.tx.tx_op.src_iov_len;
	} else {
		pe_entry->data_len = 0;
		for (i = 0; i < pe_entry->pe.tx.tx_op.src_iov_len; i++) {
			if (sock_pe_send_field(pe_entry,
				    (void *) (uintptr_t) pe_entry->pe.tx.tx_iov[i].src.iov.addr,
				    pe_entry->pe.tx.tx_iov[i].src.iov.len, len))
				return 0;
			len += pe_entry->pe.tx.tx_iov[i].src.iov.len;
			pe_entry->data_len += pe_entry->pe.tx.tx_iov[i].src.iov.len;
		}
	}

	sock_comm_flush(pe_entry);
	if (!sock_comm_tx_done(pe_entry))
		return 0;

	pe_entry->tag = 0;
	if (pe_entry->pe.tx.tx_op.op == SOCK_OP_TSEND)
		pe_entry->flags |= FI_TAGGED;
	pe_entry->flags |= (FI_MSG | FI_SEND);

	pe_entry->msg_hdr.flags = pe_entry->flags;
	if (pe_entry->done_len == pe_entry->total_len) {
		pe_entry->pe.tx.send_done = 1;
		pe_entry->conn->tx_pe_entry = NULL;
		SOCK_LOG_DBG("Send complete\n");

		if (pe_entry->flags & FI_INJECT_COMPLETE) {
			sock_pe_report_send_completion(pe_entry);
			pe_entry->is_complete = 1;
		}
	}

	return 0;
}

static int sock_pe_progress_tx_conn_msg(struct sock_pe *pe,
					struct sock_pe_entry *pe_entry,
					struct sock_conn *conn)
{
	size_t len;
	if (pe_entry->pe.tx.send_done)
		return 0;

	len = sizeof(struct sock_msg_hdr);

	if (sock_pe_send_field(pe_entry, pe_entry->pe.tx.inject,
                               pe_entry->pe.tx.tx_op.src_iov_len, len))
		return 0;
	len += pe_entry->pe.tx.tx_op.src_iov_len;
	pe_entry->data_len = pe_entry->pe.tx.tx_op.src_iov_len;

	sock_comm_flush(pe_entry);
	if (!sock_comm_tx_done(pe_entry))
		return 0;

	if (pe_entry->done_len == pe_entry->total_len) {
		pe_entry->pe.tx.send_done = 1;
		pe_entry->conn->tx_pe_entry = NULL;
		SOCK_LOG_DBG("Send complete\n");
		pe_entry->is_complete = 1;
	}
	return 0;
}

static int sock_pe_progress_tx_entry(struct sock_pe *pe,
				     struct sock_tx_ctx *tx_ctx,
				     struct sock_pe_entry *pe_entry)
{
	int ret = 0;
	struct sock_conn *conn = pe_entry->conn;

	if (pe_entry->is_complete || !conn)
		goto out;

	if (sock_comm_is_disconnected(pe_entry)) {
		SOCK_LOG_DBG("conn disconnected: removing fd from pollset\n");
		if (pe_entry->ep_attr->cmap.used > 0 &&
		     pe_entry->conn->sock_fd != -1) {
			fastlock_acquire(&pe_entry->ep_attr->cmap.lock);
			sock_ep_remove_conn(pe_entry->ep_attr, pe_entry->conn);
			fastlock_release(&pe_entry->ep_attr->cmap.lock);
		}

		sock_pe_report_tx_error(pe_entry, 0, FI_EIO);
		pe_entry->is_complete = 1;

		goto out;
	}

	if (pe_entry->pe.tx.send_done)
		goto out;

	if (conn->tx_pe_entry != NULL && conn->tx_pe_entry != pe_entry) {
		SOCK_LOG_DBG("Cannot progress %p as conn %p is being used by %p\n",
			      pe_entry, conn, conn->tx_pe_entry);
		goto out;
	}

	if (conn->tx_pe_entry == NULL) {
		SOCK_LOG_DBG("Connection %p grabbed by %p\n", conn, pe_entry);
		conn->tx_pe_entry = pe_entry;
	}

	if ((pe_entry->flags & FI_FENCE) &&
	    (tx_ctx->pe_entry_list.next != &pe_entry->ctx_entry)) {
		SOCK_LOG_DBG("Waiting for FI_FENCE\n");
		goto out;
	}

	if (!pe_entry->pe.tx.header_sent) {
		if (sock_pe_send_field(pe_entry, &pe_entry->msg_hdr,
				       sizeof(struct sock_msg_hdr), 0))
			goto out;
		pe_entry->pe.tx.header_sent = 1;
	}

	switch (pe_entry->msg_hdr.op_type) {
	case SOCK_OP_SEND:
	case SOCK_OP_TSEND:
		ret = sock_pe_progress_tx_send(pe, pe_entry, conn);
		break;
	case SOCK_OP_WRITE:
		ret = sock_pe_progress_tx_write(pe, pe_entry, conn);
		break;
	case SOCK_OP_READ:
		ret = sock_pe_progress_tx_read(pe, pe_entry, conn);
		break;
	case SOCK_OP_ATOMIC:
		ret = sock_pe_progress_tx_atomic(pe, pe_entry, conn);
		break;
	case SOCK_OP_CONN_MSG:
		ret = sock_pe_progress_tx_conn_msg(pe, pe_entry, conn);
		break;
	default:
		ret = -FI_ENOSYS;
		SOCK_LOG_ERROR("Operation not supported\n");
		break;
	}

out:
	if (pe_entry->is_complete) {
		sock_pe_release_entry(pe, pe_entry);
		SOCK_LOG_DBG("[%p] TX done\n", pe_entry);
	}
	return ret;
}

static int sock_pe_progress_rx_pe_entry(struct sock_pe *pe,
					struct sock_pe_entry *pe_entry,
					struct sock_rx_ctx *rx_ctx)
{
	int ret;

	if (sock_comm_is_disconnected(pe_entry)) {
		SOCK_LOG_DBG("conn disconnected: removing fd from pollset\n");
		if (pe_entry->ep_attr->cmap.used > 0 &&
		     pe_entry->conn->sock_fd != -1) {
			fastlock_acquire(&pe_entry->ep_attr->cmap.lock);
			sock_ep_remove_conn(pe_entry->ep_attr, pe_entry->conn);
			fastlock_release(&pe_entry->ep_attr->cmap.lock);
		}

		if (pe_entry->pe.rx.header_read)
			sock_pe_report_rx_error(pe_entry, 0, FI_EIO);

		sock_pe_release_entry(pe, pe_entry);
		return 0;
	}

	if (pe_entry->pe.rx.pending_send) {
		sock_pe_progress_pending_ack(pe, pe_entry);
		goto out;
	}

	if (pe_entry->is_error)
		goto out;

	if (!pe_entry->pe.rx.header_read) {
		if (sock_pe_read_hdr(pe, rx_ctx, pe_entry) == -1) {
			sock_pe_release_entry(pe, pe_entry);
			return 0;
		}
	}

	if (pe_entry->pe.rx.header_read) {
		ret = sock_pe_process_recv(pe, rx_ctx, pe_entry);
		if (ret < 0)
			return ret;
	}

out:
	if (pe_entry->is_error)
		sock_pe_discard_field(pe_entry);

	if (pe_entry->is_complete && !pe_entry->pe.rx.pending_send) {
		sock_pe_release_entry(pe, pe_entry);
		SOCK_LOG_DBG("[%p] RX done\n", pe_entry);
	}
	return 0;
}

static void sock_pe_new_rx_entry(struct sock_pe *pe, struct sock_rx_ctx *rx_ctx,
				struct sock_ep_attr *ep_attr, struct sock_conn *conn)
{
	struct sock_pe_entry *pe_entry;

	pe_entry = sock_pe_acquire_entry(pe);
	if (!pe_entry)
		return;
	memset(&pe_entry->pe.rx, 0, sizeof(pe_entry->pe.rx));

	pe_entry->conn = conn;
	pe_entry->type = SOCK_PE_RX;
	pe_entry->ep_attr = ep_attr;
	pe_entry->is_complete = 0;
	pe_entry->done_len = 0;
	pe_entry->completion_reported = 0;

	if (ep_attr->ep_type == FI_EP_MSG || !ep_attr->av)
		pe_entry->addr = FI_ADDR_NOTAVAIL;
	else
		pe_entry->addr = conn->av_index;

	if (rx_ctx->ctx.fid.fclass == FI_CLASS_SRX_CTX)
		pe_entry->comp = &ep_attr->rx_ctx->comp;
	else
		pe_entry->comp = &rx_ctx->comp;

	SOCK_LOG_DBG("New RX on PE entry %p (%ld)\n",
		      pe_entry, PE_INDEX(pe, pe_entry));

	SOCK_LOG_DBG("Inserting rx_entry to PE entry %p, conn: %p\n",
		      pe_entry, pe_entry->conn);

	dlist_insert_tail(&pe_entry->ctx_entry, &rx_ctx->pe_entry_list);
}

static int sock_pe_new_tx_entry(struct sock_pe *pe, struct sock_tx_ctx *tx_ctx)
{
	int i, datatype_sz;
	struct sock_msg_hdr *msg_hdr;
	struct sock_pe_entry *pe_entry;
	struct sock_ep_attr *ep_attr;

	pe_entry = sock_pe_acquire_entry(pe);
	memset(&pe_entry->pe.tx, 0, sizeof(pe_entry->pe.tx));
	memset(&pe_entry->msg_hdr, 0, sizeof(pe_entry->msg_hdr));

	pe_entry->type = SOCK_PE_TX;
	pe_entry->is_complete = 0;
	pe_entry->done_len = 0;
	pe_entry->conn = NULL;
	pe_entry->ep_attr = tx_ctx->ep_attr;
	pe_entry->pe.tx.tx_ctx = tx_ctx;
	pe_entry->completion_reported = 0;

	dlist_insert_tail(&pe_entry->ctx_entry, &tx_ctx->pe_entry_list);

	/* fill in PE tx entry */
	msg_hdr = &pe_entry->msg_hdr;
	msg_hdr->msg_len = sizeof(*msg_hdr);

	msg_hdr->pe_entry_id = PE_INDEX(pe, pe_entry);
	SOCK_LOG_DBG("New TX on PE entry %p (%d)\n",
		      pe_entry, msg_hdr->pe_entry_id);

	sock_tx_ctx_read_op_send(tx_ctx, &pe_entry->pe.tx.tx_op,
			&pe_entry->flags, &pe_entry->context, &pe_entry->addr,
			&pe_entry->buf, &ep_attr, &pe_entry->conn);

	if (pe_entry->pe.tx.tx_op.op == SOCK_OP_TSEND) {
		ofi_rbread(&tx_ctx->rb, &pe_entry->tag, sizeof(pe_entry->tag));
		msg_hdr->msg_len += sizeof(pe_entry->tag);
	}

	if (ep_attr && tx_ctx->fclass == FI_CLASS_STX_CTX)
		pe_entry->comp = &ep_attr->tx_ctx->comp;
	else
		pe_entry->comp = &tx_ctx->comp;

	if (pe_entry->flags & FI_REMOTE_CQ_DATA) {
		ofi_rbread(&tx_ctx->rb, &pe_entry->data, sizeof(pe_entry->data));
		msg_hdr->msg_len += sizeof(pe_entry->data);
	}

	msg_hdr->op_type = pe_entry->pe.tx.tx_op.op;
	switch (pe_entry->pe.tx.tx_op.op) {
	case SOCK_OP_SEND:
	case SOCK_OP_TSEND:
		if (pe_entry->flags & FI_INJECT) {
			ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.inject[0],
				 pe_entry->pe.tx.tx_op.src_iov_len);
			msg_hdr->msg_len += pe_entry->pe.tx.tx_op.src_iov_len;
		} else {
			for (i = 0; i < pe_entry->pe.tx.tx_op.src_iov_len; i++) {
				ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.tx_iov[i].src,
					 sizeof(pe_entry->pe.tx.tx_iov[i].src));
				msg_hdr->msg_len += pe_entry->pe.tx.tx_iov[i].src.iov.len;
			}
		}
		msg_hdr->dest_iov_len = pe_entry->pe.tx.tx_op.dest_iov_len;
		if (pe_entry->flags & SOCK_NO_COMPLETION)
			pe_entry->flags |= FI_INJECT_COMPLETE;
		break;
	case SOCK_OP_WRITE:
		if (pe_entry->flags & FI_INJECT) {
			ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.inject[0],
				 pe_entry->pe.tx.tx_op.src_iov_len);
			msg_hdr->msg_len += pe_entry->pe.tx.tx_op.src_iov_len;
		} else {
			for (i = 0; i < pe_entry->pe.tx.tx_op.src_iov_len; i++) {
				ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.tx_iov[i].src,
					 sizeof(pe_entry->pe.tx.tx_iov[i].src));
				msg_hdr->msg_len += pe_entry->pe.tx.tx_iov[i].src.iov.len;
			}
		}

		for (i = 0; i < pe_entry->pe.tx.tx_op.dest_iov_len; i++) {
			ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.tx_iov[i].dst,
				 sizeof(pe_entry->pe.tx.tx_iov[i].dst));
		}
		msg_hdr->msg_len += sizeof(union sock_iov) * i;
		msg_hdr->dest_iov_len = pe_entry->pe.tx.tx_op.dest_iov_len;
		break;
	case SOCK_OP_READ:
		for (i = 0; i < pe_entry->pe.tx.tx_op.src_iov_len; i++) {
			ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.tx_iov[i].src,
				 sizeof(pe_entry->pe.tx.tx_iov[i].src));
		}
		msg_hdr->msg_len += sizeof(union sock_iov) * i;

		for (i = 0;  i < pe_entry->pe.tx.tx_op.dest_iov_len; i++) {
			ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.tx_iov[i].dst,
				 sizeof(pe_entry->pe.tx.tx_iov[i].dst));
		}
		msg_hdr->dest_iov_len = pe_entry->pe.tx.tx_op.src_iov_len;
		break;
	case SOCK_OP_ATOMIC:
		msg_hdr->msg_len += sizeof(struct sock_op);
		datatype_sz = ofi_datatype_size(pe_entry->pe.tx.tx_op.atomic.datatype);
		if (pe_entry->flags & FI_INJECT) {
			ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.inject[0],
				 pe_entry->pe.tx.tx_op.src_iov_len);
			ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.inject[0] +
				pe_entry->pe.tx.tx_op.src_iov_len,
				pe_entry->pe.tx.tx_op.atomic.cmp_iov_len);
			msg_hdr->msg_len += pe_entry->pe.tx.tx_op.src_iov_len +
						pe_entry->pe.tx.tx_op.atomic.cmp_iov_len;
		} else {
			for (i = 0; i < pe_entry->pe.tx.tx_op.src_iov_len; i++) {
				ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.tx_iov[i].src,
					 sizeof(pe_entry->pe.tx.tx_iov[i].src));

				if (pe_entry->pe.tx.tx_op.atomic.op != FI_ATOMIC_READ)
					msg_hdr->msg_len += datatype_sz *
						pe_entry->pe.tx.tx_iov[i].src.ioc.count;
			}
			for (i = 0; i < pe_entry->pe.tx.tx_op.atomic.cmp_iov_len; i++) {
				ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.tx_iov[i].cmp,
				 	sizeof(pe_entry->pe.tx.tx_iov[i].cmp));
				msg_hdr->msg_len += datatype_sz *
					pe_entry->pe.tx.tx_iov[i].cmp.ioc.count;
			}
		}

		for (i = 0; i < pe_entry->pe.tx.tx_op.dest_iov_len; i++) {
			ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.tx_iov[i].dst,
				 sizeof(pe_entry->pe.tx.tx_iov[i].dst));
		}
		msg_hdr->msg_len += sizeof(union sock_iov) * i;

		for (i = 0; i < pe_entry->pe.tx.tx_op.atomic.res_iov_len; i++) {
			ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.tx_iov[i].res,
				 sizeof(pe_entry->pe.tx.tx_iov[i].res));
		}

		msg_hdr->dest_iov_len = pe_entry->pe.tx.tx_op.dest_iov_len;
		break;
	case SOCK_OP_CONN_MSG:
		ofi_rbread(&tx_ctx->rb, &pe_entry->pe.tx.inject[0],
			pe_entry->pe.tx.tx_op.src_iov_len);
		msg_hdr->msg_len += pe_entry->pe.tx.tx_op.src_iov_len;
		break;
	default:
		SOCK_LOG_ERROR("Invalid operation type\n");
		return -FI_EINVAL;
	}
	SOCK_LOG_DBG("Inserting TX-entry to PE entry %p, conn: %p\n",
		      pe_entry, pe_entry->conn);

	/* prepare message header */
	msg_hdr->version = SOCK_WIRE_PROTO_VERSION;

	if (tx_ctx->av)
		msg_hdr->rx_id = (uint16_t) SOCK_GET_RX_ID(pe_entry->addr,
							   tx_ctx->av->rx_ctx_bits);
	else
		msg_hdr->rx_id = 0;

	if (pe_entry->flags & FI_INJECT_COMPLETE)
		pe_entry->flags &= ~FI_TRANSMIT_COMPLETE;

	msg_hdr->flags = htonll(pe_entry->flags);
	pe_entry->total_len = msg_hdr->msg_len;
	msg_hdr->msg_len = htonll(msg_hdr->msg_len);
	msg_hdr->pe_entry_id = htons(msg_hdr->pe_entry_id);

	return sock_pe_progress_tx_entry(pe, tx_ctx, pe_entry);
}

void sock_pe_signal(struct sock_pe *pe)
{
	char c = 0;
	if (pe->domain->progress_mode != FI_PROGRESS_AUTO)
		return;

	fastlock_acquire(&pe->signal_lock);
	if (pe->wcnt == pe->rcnt) {
		if (ofi_write_socket(pe->signal_fds[SOCK_SIGNAL_WR_FD], &c, 1) != 1)
			SOCK_LOG_ERROR("Failed to signal\n");
		else
			pe->wcnt++;
	}
	fastlock_release(&pe->signal_lock);
}

void sock_pe_poll_add(struct sock_pe *pe, int fd)
{
        fastlock_acquire(&pe->signal_lock);
        if (ofi_epoll_add(pe->epoll_set, fd, OFI_EPOLL_IN, NULL))
			SOCK_LOG_ERROR("failed to add to epoll set: %d\n", fd);
        fastlock_release(&pe->signal_lock);
}

void sock_pe_poll_del(struct sock_pe *pe, int fd)
{
        fastlock_acquire(&pe->signal_lock);
        if (ofi_epoll_del(pe->epoll_set, fd))
			SOCK_LOG_DBG("failed to del from epoll set: %d\n", fd);
        fastlock_release(&pe->signal_lock);
}

void sock_pe_add_tx_ctx(struct sock_pe *pe, struct sock_tx_ctx *ctx)
{
	struct dlist_entry *entry;
	struct sock_tx_ctx *curr_ctx;
	pthread_mutex_lock(&pe->list_lock);
	for (entry = pe->tx_list.next; entry != &pe->tx_list;
	     entry = entry->next) {
		curr_ctx = container_of(entry, struct sock_tx_ctx, pe_entry);
		if (curr_ctx == ctx)
			goto out;
	}

	dlist_insert_tail(&ctx->pe_entry, &pe->tx_list);
	sock_pe_signal(pe);
out:
	pthread_mutex_unlock(&pe->list_lock);
	SOCK_LOG_DBG("TX ctx added to PE\n");
}

void sock_pe_add_rx_ctx(struct sock_pe *pe, struct sock_rx_ctx *ctx)
{
	struct dlist_entry *entry;
	struct sock_rx_ctx *curr_ctx;
	pthread_mutex_lock(&pe->list_lock);
	for (entry = pe->rx_list.next; entry != &pe->rx_list;
	     entry = entry->next) {
		curr_ctx = container_of(entry, struct sock_rx_ctx, pe_entry);
		if (curr_ctx == ctx)
			goto out;
	}
	dlist_insert_tail(&ctx->pe_entry, &pe->rx_list);
	sock_pe_signal(pe);
out:
	pthread_mutex_unlock(&pe->list_lock);
	SOCK_LOG_DBG("RX ctx added to PE\n");
}

void sock_pe_remove_tx_ctx(struct sock_tx_ctx *tx_ctx)
{
	pthread_mutex_lock(&tx_ctx->domain->pe->list_lock);
	dlist_remove(&tx_ctx->pe_entry);
	pthread_mutex_unlock(&tx_ctx->domain->pe->list_lock);
}

void sock_pe_remove_rx_ctx(struct sock_rx_ctx *rx_ctx)
{
	pthread_mutex_lock(&rx_ctx->domain->pe->list_lock);
	dlist_remove(&rx_ctx->pe_entry);
	pthread_mutex_unlock(&rx_ctx->domain->pe->list_lock);
}

static int sock_pe_progress_rx_ep(struct sock_pe *pe,
				  struct sock_ep_attr *ep_attr,
				  struct sock_rx_ctx *rx_ctx)
{
	int i, num_fds;
	struct sock_conn *conn;
	struct sock_conn_map *map;

	map = &ep_attr->cmap;

	if (!map->used)
		return 0;

	if (map->epoll_ctxs_sz < map->used) {
		uint64_t new_size = map->used * 2;
		void *ctxs;

		ctxs = realloc(map->epoll_ctxs,
			       sizeof(*map->epoll_ctxs) * new_size);
		if (ctxs) {
			map->epoll_ctxs = ctxs;
			map->epoll_ctxs_sz = new_size;
		}
	}

	num_fds = ofi_epoll_wait(map->epoll_set, map->epoll_ctxs,
	                        MIN(map->used, map->epoll_ctxs_sz), 0);
	if (num_fds < 0 || num_fds == 0) {
		if (num_fds < 0)
			SOCK_LOG_ERROR("epoll failed: %d\n", num_fds);
		return num_fds;
	}

	fastlock_acquire(&map->lock);
	for (i = 0; i < num_fds; i++) {
		conn = map->epoll_ctxs[i];
		if (!conn)
			SOCK_LOG_ERROR("ofi_idm_lookup failed\n");

		if (!conn || conn->rx_pe_entry)
			continue;

		sock_pe_new_rx_entry(pe, rx_ctx, ep_attr, conn);
	}
	fastlock_release(&map->lock);

	return 0;
}

int sock_pe_progress_rx_ctx(struct sock_pe *pe, struct sock_rx_ctx *rx_ctx)
{
	int ret = 0;
	struct sock_ep_attr *ep_attr;
	struct dlist_entry *entry;
	struct sock_pe_entry *pe_entry;

	fastlock_acquire(&pe->lock);

	fastlock_acquire(&rx_ctx->lock);
	sock_pe_progress_buffered_rx(rx_ctx);
	fastlock_release(&rx_ctx->lock);

	/* check for incoming data */
	if (rx_ctx->ctx.fid.fclass == FI_CLASS_SRX_CTX) {
		for (entry = rx_ctx->ep_list.next;
		     entry != &rx_ctx->ep_list;) {
			ep_attr = container_of(entry, struct sock_ep_attr, rx_ctx_entry);
			entry = entry->next;
			ret = sock_pe_progress_rx_ep(pe, ep_attr, rx_ctx);
			if (ret < 0)
				goto out;
		}
	} else {
		ep_attr = rx_ctx->ep_attr;
		ret = sock_pe_progress_rx_ep(pe, ep_attr, rx_ctx);
		if (ret < 0)
			goto out;
	}

	for (entry = rx_ctx->pe_entry_list.next;
	     entry != &rx_ctx->pe_entry_list;) {
		pe_entry = container_of(entry, struct sock_pe_entry, ctx_entry);
		entry = entry->next;
		ret = sock_pe_progress_rx_pe_entry(pe, pe_entry, rx_ctx);
		if (ret < 0)
			goto out;
	}
out:
	if (ret < 0)
		SOCK_LOG_ERROR("failed to progress RX ctx\n");
	fastlock_release(&pe->lock);
	return ret;
}

int sock_pe_progress_ep_rx(struct sock_pe *pe, struct sock_ep_attr *ep_attr)
{
	struct sock_rx_ctx *rx_ctx;
	int ret, i;

	for (i = 0; i < ep_attr->ep_attr.rx_ctx_cnt; i++) {
		rx_ctx = ep_attr->rx_array[i];
		if (!rx_ctx)
			continue;

		ret = sock_pe_progress_rx_ctx(pe, rx_ctx);
		if (ret < 0)
			return ret;
	}
	return 0;
}

int sock_pe_progress_ep_tx(struct sock_pe *pe, struct sock_ep_attr *ep_attr)
{
	struct sock_tx_ctx *tx_ctx;
	int ret, i;

	for (i = 0; i < ep_attr->ep_attr.tx_ctx_cnt; i++) {
		tx_ctx = ep_attr->tx_array[i];
		if (!tx_ctx)
			continue;

		ret = sock_pe_progress_tx_ctx(pe, tx_ctx);
		if (ret < 0)
			return ret;
	}
	return 0;
}

void sock_pe_progress_rx_ctrl_ctx(struct sock_pe *pe, struct sock_rx_ctx *rx_ctx,
				  struct sock_tx_ctx *tx_ctx)
{
	struct sock_ep_attr *ep_attr;
	struct dlist_entry *entry;
	struct sock_pe_entry *pe_entry;

	/* check for incoming data */
	if (tx_ctx->fclass == FI_CLASS_STX_CTX) {
		for (entry = tx_ctx->ep_list.next; entry != &tx_ctx->ep_list;) {
			ep_attr = container_of(entry, struct sock_ep_attr, tx_ctx_entry);
			entry = entry->next;
			sock_pe_progress_rx_ep(pe, ep_attr, tx_ctx->rx_ctrl_ctx);
		}
	} else {
		sock_pe_progress_rx_ep(pe, tx_ctx->ep_attr, tx_ctx->rx_ctrl_ctx);
	}

	for (entry = rx_ctx->pe_entry_list.next;
	     entry != &rx_ctx->pe_entry_list;) {
		pe_entry = container_of(entry, struct sock_pe_entry, ctx_entry);
		entry = entry->next;
		sock_pe_progress_rx_pe_entry(pe, pe_entry, rx_ctx);
	}
}

int sock_pe_progress_tx_ctx(struct sock_pe *pe, struct sock_tx_ctx *tx_ctx)
{
	int ret = 0;
	struct dlist_entry *entry;
	struct sock_pe_entry *pe_entry;

	fastlock_acquire(&pe->lock);

	/* progress tx_ctx in PE table */
	for (entry = tx_ctx->pe_entry_list.next;
	     entry != &tx_ctx->pe_entry_list;) {
		pe_entry = container_of(entry, struct sock_pe_entry, ctx_entry);
		entry = entry->next;

		ret = sock_pe_progress_tx_entry(pe, tx_ctx, pe_entry);
		if (ret < 0) {
			SOCK_LOG_ERROR("Error in progressing %p\n", pe_entry);
			goto out;
		}
	}

	fastlock_acquire(&tx_ctx->rb_lock);
	if (!ofi_rbempty(&tx_ctx->rb) && !dlist_empty(&pe->free_list)) {
		ret = sock_pe_new_tx_entry(pe, tx_ctx);
	}
	fastlock_release(&tx_ctx->rb_lock);
	if (ret < 0)
		goto out;

	sock_pe_progress_rx_ctrl_ctx(pe, tx_ctx->rx_ctrl_ctx, tx_ctx);
out:
	if (ret < 0)
		SOCK_LOG_ERROR("failed to progress TX ctx\n");
	fastlock_release(&pe->lock);
	return ret;
}

static int sock_pe_wait_ok(struct sock_pe *pe)
{
	struct dlist_entry *entry;
	struct sock_tx_ctx *tx_ctx;
	struct sock_rx_ctx *rx_ctx;

	if (pe->waittime && ((ofi_gettime_ms() - pe->waittime) < (uint64_t)sock_pe_waittime))
		return 0;

	if (dlist_empty(&pe->tx_list) && dlist_empty(&pe->rx_list))
		return 1;

	if (!dlist_empty(&pe->tx_list)) {
		for (entry = pe->tx_list.next;
		     entry != &pe->tx_list; entry = entry->next) {
			tx_ctx = container_of(entry, struct sock_tx_ctx,
						pe_entry);
			if (!ofi_rbempty(&tx_ctx->rb) ||
			    !dlist_empty(&tx_ctx->pe_entry_list)) {
				return 0;
			}
		}
	}

	if (!dlist_empty(&pe->rx_list)) {
		for (entry = pe->rx_list.next;
		     entry != &pe->rx_list; entry = entry->next) {
			rx_ctx = container_of(entry, struct sock_rx_ctx,
						pe_entry);
			if (!dlist_empty(&rx_ctx->rx_buffered_list) ||
			    !dlist_empty(&rx_ctx->pe_entry_list)) {
				return 0;
			}
		}
	}

	return 1;
}

static void sock_pe_wait(struct sock_pe *pe)
{
	char tmp;
	int ret;
	void *ep_contexts[1];

	ret = ofi_epoll_wait(pe->epoll_set, ep_contexts, 1, -1);
	if (ret < 0)
		SOCK_LOG_ERROR("poll failed : %s\n", strerror(ofi_sockerr()));

	fastlock_acquire(&pe->signal_lock);
	if (pe->rcnt != pe->wcnt) {
		if (ofi_read_socket(pe->signal_fds[SOCK_SIGNAL_RD_FD], &tmp, 1) == 1)
			pe->rcnt++;
		else
			SOCK_LOG_ERROR("Invalid signal\n");
	}
	fastlock_release(&pe->signal_lock);
	pe->waittime = ofi_gettime_ms();
}

static void sock_pe_set_affinity(void)
{
	char *sock_pe_affinity_str;
	if (fi_param_get_str(&sock_prov, "pe_affinity", &sock_pe_affinity_str) != FI_SUCCESS)
		return;

	if (sock_pe_affinity_str == NULL)
		return;

	if (ofi_set_thread_affinity(sock_pe_affinity_str) == -FI_ENOSYS)
		SOCK_LOG_ERROR("FI_SOCKETS_PE_AFFINITY is not supported on OS X and Windows\n");
}

static void *sock_pe_progress_thread(void *data)
{
	int ret;
	struct dlist_entry *entry;
	struct sock_tx_ctx *tx_ctx;
	struct sock_rx_ctx *rx_ctx;
	struct sock_pe *pe = (struct sock_pe *)data;

	SOCK_LOG_DBG("Progress thread started\n");
	sock_pe_set_affinity();
	while (*((volatile int *)&pe->do_progress)) {
		pthread_mutex_lock(&pe->list_lock);
		if (pe->domain->progress_mode == FI_PROGRESS_AUTO &&
		    sock_pe_wait_ok(pe)) {
			pthread_mutex_unlock(&pe->list_lock);
			sock_pe_wait(pe);
			pthread_mutex_lock(&pe->list_lock);
		}

		if (!dlist_empty(&pe->tx_list)) {
			for (entry = pe->tx_list.next;
			     entry != &pe->tx_list; entry = entry->next) {
				tx_ctx = container_of(entry, struct sock_tx_ctx,
						      pe_entry);
				ret = sock_pe_progress_tx_ctx(pe, tx_ctx);
				if (ret < 0) {
					SOCK_LOG_ERROR("failed to progress TX\n");
					pthread_mutex_unlock(&pe->list_lock);
					return NULL;
				}
			}
		}

		if (!dlist_empty(&pe->rx_list)) {
			for (entry = pe->rx_list.next;
			     entry != &pe->rx_list; entry = entry->next) {
				rx_ctx = container_of(entry, struct sock_rx_ctx,
						      pe_entry);
				ret = sock_pe_progress_rx_ctx(pe, rx_ctx);
				if (ret < 0) {
					SOCK_LOG_ERROR("failed to progress RX\n");
					pthread_mutex_unlock(&pe->list_lock);
					return NULL;
				}
			}
		}
		pthread_mutex_unlock(&pe->list_lock);
	}

	SOCK_LOG_DBG("Progress thread terminated\n");
	return NULL;
}

static void sock_pe_init_table(struct sock_pe *pe)
{
	int i;

	memset(&pe->pe_table, 0,
	       sizeof(struct sock_pe_entry) * SOCK_PE_MAX_ENTRIES);

	dlist_init(&pe->free_list);
	dlist_init(&pe->busy_list);
	dlist_init(&pe->pool_list);

	for (i = 0; i < SOCK_PE_MAX_ENTRIES; i++) {
		dlist_insert_head(&pe->pe_table[i].entry, &pe->free_list);
		pe->pe_table[i].cache_sz = SOCK_PE_COMM_BUFF_SZ;
		if (ofi_rbinit(&pe->pe_table[i].comm_buf, SOCK_PE_COMM_BUFF_SZ))
			SOCK_LOG_ERROR("failed to init comm-cache\n");
	}

	pe->num_free_entries = SOCK_PE_MAX_ENTRIES;
	SOCK_LOG_DBG("PE table init: OK\n");
}

struct sock_pe *sock_pe_init(struct sock_domain *domain)
{
	struct sock_pe *pe;
	int ret;

	pe = calloc(1, sizeof(*pe));
	if (!pe)
		return NULL;

	sock_pe_init_table(pe);
	dlist_init(&pe->tx_list);
	dlist_init(&pe->rx_list);
	fastlock_init(&pe->lock);
	fastlock_init(&pe->signal_lock);
	pthread_mutex_init(&pe->list_lock, NULL);
	pe->domain = domain;

	
	ret = ofi_bufpool_create(&pe->pe_rx_pool,
				 sizeof(struct sock_pe_entry), 16, 0, 1024, 0);
	if (ret) {
		SOCK_LOG_ERROR("failed to create buffer pool\n");
		goto err1;
	}

	ret = ofi_bufpool_create(&pe->atomic_rx_pool,
				 SOCK_EP_MAX_ATOMIC_SZ, 16, 0, 32, 0);
	if (ret) {
		SOCK_LOG_ERROR("failed to create atomic rx buffer pool\n");
		goto err2;
	}

	if (ofi_epoll_create(&pe->epoll_set) < 0) {
                SOCK_LOG_ERROR("failed to create epoll set\n");
                goto err3;
	}

	if (domain->progress_mode == FI_PROGRESS_AUTO) {
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, pe->signal_fds) < 0)
			goto err4;

		if (fd_set_nonblock(pe->signal_fds[SOCK_SIGNAL_RD_FD]) ||
		    ofi_epoll_add(pe->epoll_set,
				 pe->signal_fds[SOCK_SIGNAL_RD_FD],
				 OFI_EPOLL_IN, NULL))
			goto err5;

		pe->do_progress = 1;
		if (pthread_create(&pe->progress_thread, NULL,
				   sock_pe_progress_thread, (void *)pe)) {
			SOCK_LOG_ERROR("Couldn't create progress thread\n");
			goto err5;
		}
	}
	SOCK_LOG_DBG("PE init: OK\n");
	return pe;

err5:
	ofi_close_socket(pe->signal_fds[0]);
	ofi_close_socket(pe->signal_fds[1]);
err4:
	ofi_epoll_close(pe->epoll_set);
err3:
	ofi_bufpool_destroy(pe->atomic_rx_pool);
err2:
	ofi_bufpool_destroy(pe->pe_rx_pool);
err1:
	fastlock_destroy(&pe->lock);
	free(pe);
	return NULL;
}

static void sock_pe_free_util_pool(struct sock_pe *pe)
{
	struct dlist_entry *entry;
	struct sock_pe_entry *pe_entry;

	while (!dlist_empty(&pe->pool_list)) {
		entry = pe->pool_list.next;
		pe_entry = container_of(entry, struct sock_pe_entry, entry);
		ofi_rbfree(&pe_entry->comm_buf);
		dlist_remove(&pe_entry->entry);
		ofi_buf_free(pe_entry);
	}

	ofi_bufpool_destroy(pe->pe_rx_pool);
	ofi_bufpool_destroy(pe->atomic_rx_pool);
}

void sock_pe_finalize(struct sock_pe *pe)
{
	int i;
	if (pe->domain->progress_mode == FI_PROGRESS_AUTO) {
		pe->do_progress = 0;
		sock_pe_signal(pe);
		pthread_join(pe->progress_thread, NULL);
		ofi_close_socket(pe->signal_fds[0]);
		ofi_close_socket(pe->signal_fds[1]);
	}

	for (i = 0; i < SOCK_PE_MAX_ENTRIES; i++) {
		ofi_rbfree(&pe->pe_table[i].comm_buf);
	}

	sock_pe_free_util_pool(pe);
	fastlock_destroy(&pe->lock);
	fastlock_destroy(&pe->signal_lock);
	pthread_mutex_destroy(&pe->list_lock);
	ofi_epoll_close(pe->epoll_set);
	free(pe);
	SOCK_LOG_DBG("Progress engine finalize: OK\n");
}

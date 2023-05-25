/*
 * Copyright (c) 2019-2020 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifndef _RXR_RDMA_H_
#define _RXR_RDMA_H_

/*
 * read can used in 2 scenarios:
 *
 * 1. application posted a read request.
 *
 * 2. read message protocol is being used, receiver is going
 *    to post a read requst.
 *
 * 3. a packet entry with data has been received, and the
 *    receiving buffer is on GPU memroy. A read request is
 *    being posted to copy data to receiving buffer.
 *
 * To distinguish them, we use a pointer as context.
 *
 * For 1, the tx_entry is used as context
 * For 2, the rx_entry is used as context
 * For 3, the pkt_entry is used as context
 *
 * We also store rxr_read_context_type in read_entry to specify
 * context type.
 */
enum rxr_read_context_type {
	RXR_READ_CONTEXT_TX_ENTRY,
	RXR_READ_CONTEXT_RX_ENTRY,
	RXR_READ_CONTEXT_PKT_ENTRY,
};

enum rxr_read_entry_state {
	RXR_RDMA_ENTRY_FREE = 0,
	RXR_RDMA_ENTRY_CREATED,
	RXR_RDMA_ENTRY_PENDING,
	RXR_RDMA_ENTRY_SUBMITTED,
};

/*
 * rxr_read_entry contains the information of a read request
 */
struct rxr_read_entry {
	enum rxr_x_entry_type type;
	int read_id;
	enum rxr_lower_ep_type lower_ep_type;

	void *context;
	enum rxr_read_context_type context_type;

	enum rxr_read_entry_state state;

	fi_addr_t addr;

	struct iovec iov[RXR_IOV_LIMIT];
	size_t iov_count;
	struct fid_mr *mr[RXR_IOV_LIMIT];
	void *mr_desc[RXR_IOV_LIMIT];

	struct fi_rma_iov rma_iov[RXR_IOV_LIMIT];
	size_t rma_iov_count;

	size_t total_len;
	size_t bytes_submitted; /* bytes fi_read() succeeded */
	size_t bytes_finished; /* bytes received completion */

	struct dlist_entry pending_entry;
};

struct rxr_read_entry *rxr_read_alloc_entry(struct rxr_ep *ep, struct rxr_op_entry *x_entry,
					    enum rxr_lower_ep_type lower_ep_type);

void rxr_read_release_entry(struct rxr_ep *ep, struct rxr_read_entry *read_entry);

/* used by read message protocol and read write protocol */
int rxr_locate_iov_pos(struct iovec *iov, int iov_count, size_t offset,
		       int *iov_idx, size_t *iov_offset);

int rxr_read_init_iov(struct rxr_ep *ep,
		      struct rxr_op_entry *tx_entry,
		      struct fi_rma_iov *read_iov);

int rxr_read_post(struct rxr_ep *ep, struct rxr_read_entry *read_entry);

int rxr_read_post_remote_read_or_queue(struct rxr_ep *ep, struct rxr_op_entry *op_entry);

int rxr_read_post_local_read_or_queue(struct rxr_ep *ep,
				      struct rxr_op_entry *rx_entry,
				      size_t data_offset,
				      struct rxr_pkt_entry *pkt_entry,
				      char *data, size_t data_size);

void rxr_read_handle_read_completion(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry);

void rxr_read_write_error(struct rxr_ep *ep, struct rxr_read_entry *read_entry, int err, int prov_errno);

#endif


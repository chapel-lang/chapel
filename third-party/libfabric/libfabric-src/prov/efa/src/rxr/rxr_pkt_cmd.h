/*
 * Copyright (c) 2019-2022 Amazon.com, Inc. or its affiliates.
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

#ifndef _RXR_PKT_CMD_H
#define _RXR_PKT_CMD_H

#include "rxr.h"

ssize_t rxr_pkt_post(struct rxr_ep *ep, struct rxr_op_entry *op_entry,
		     int pkt_type, bool inject, uint64_t flags);

ssize_t rxr_pkt_post_or_queue(struct rxr_ep *ep, struct rxr_op_entry *op_entry,
			      int req_type, bool inject);

ssize_t rxr_pkt_post_req(struct rxr_ep *ep, struct rxr_op_entry *tx_entry,
			 int req_type, bool inject, uint64_t flags);

fi_addr_t rxr_pkt_determine_addr(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_data_copied(struct rxr_ep *ep,
				struct rxr_pkt_entry *pkt_entry,
				size_t data_size);

void rxr_pkt_handle_send_error(struct rxr_ep *ep,
			       struct rxr_pkt_entry *pkt_entry,
			       int err, int prov_errno);

void rxr_pkt_handle_send_completion(struct rxr_ep *ep,
				    struct rxr_pkt_entry *pkt_entry);

void rxr_pkt_handle_recv_error(struct rxr_ep *ep,
			       struct rxr_pkt_entry *pkt_entry,
			       int err, int prov_errno);

void rxr_pkt_handle_recv_completion(struct rxr_ep *ep,
				    struct rxr_pkt_entry *pkt_entry,
				    enum rxr_lower_ep_type lower_ep_type);

ssize_t rxr_pkt_wait_handshake(struct rxr_ep *ep, fi_addr_t addr, struct rdm_peer *peer);

ssize_t rxr_pkt_trigger_handshake(struct rxr_ep *ep,
				  fi_addr_t addr, struct rdm_peer *peer);

#if ENABLE_DEBUG
void rxr_pkt_print(char *prefix,
		   struct rxr_ep *ep,
		   struct rxr_pkt_entry *pkt_entry);
#endif

#endif


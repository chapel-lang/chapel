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

#ifndef _RXR_PKT_CMD_H
#define _RXR_PKT_CMD_H

#include "rxr.h"

ssize_t rxr_pkt_post_data(struct rxr_ep *rxr_ep, struct rxr_tx_entry *tx_entry);

ssize_t rxr_pkt_post_ctrl(struct rxr_ep *ep, int entry_type, void *x_entry,
			  int ctrl_type, bool inject);

ssize_t rxr_pkt_post_ctrl_or_queue(struct rxr_ep *ep, int entry_type, void *x_entry,
				   int ctrl_type, bool inject);

size_t rxr_pkt_data_size(struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_copy_to_rx(struct rxr_ep *ep,
			   struct rxr_rx_entry *rx_entry,
			   size_t data_offset,
			   struct rxr_pkt_entry *pkt_entry,
			   char *data, size_t data_size);

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
				    struct rxr_pkt_entry *pkt_entry);

ssize_t rxr_pkt_wait_handshake(struct rxr_ep *ep, fi_addr_t addr, struct rdm_peer *peer);

ssize_t rxr_pkt_trigger_handshake(struct rxr_ep *ep,
				  fi_addr_t addr, struct rdm_peer *peer);

#if ENABLE_DEBUG
void rxr_pkt_print(char *prefix,
		   struct rxr_ep *ep,
		   struct rxr_base_hdr *hdr);
#endif

#endif


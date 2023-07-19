/*
 * Copyright (c) 2021 Amazon.com, Inc. or its affiliates.
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

#ifndef _RXR_PKT_TYPE_BASE_H
#define _RXR_PKT_TYPE_BASE_H

#include "rxr.h"

uint32_t *rxr_pkt_connid_ptr(struct rxr_pkt_entry *pkt_entry);

int rxr_pkt_init_data_from_op_entry(struct rxr_ep *ep,
				    struct rxr_pkt_entry *pkt_entry,
				    size_t pkt_data_offset,
				    struct rxr_op_entry *op_entry,
				    size_t op_data_offset,
				    size_t data_size);

ssize_t rxr_pkt_copy_data_to_op_entry(struct rxr_ep *ep,
				      struct rxr_op_entry *rx_entry,
				      size_t data_offset,
				      struct rxr_pkt_entry *pkt_entry,
				      char *data, size_t data_size);

size_t rxr_pkt_data_size(struct rxr_pkt_entry *pkt_entry);

#endif

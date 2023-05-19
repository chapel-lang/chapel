/*
 * Copyright (c) 2017-2022 Intel Corporation, Inc.  All rights reserved.
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
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <sys/types.h>

#include <rdma/fabric.h>

#include <ofi.h>
#include <ofi_proto.h>

#ifndef _XNET_PROTO_H_
#define _XNET_PROTO_H_


/*
 * Wire protocol structures and definitions
 */

#define XNET_CTRL_HDR_VERSION	3

enum {
	XNET_MAX_CM_DATA_SIZE = (1 << 8)
};

struct xnet_cm_msg {
	struct ofi_ctrl_hdr hdr;
	char data[XNET_MAX_CM_DATA_SIZE];
};

#define XNET_HDR_VERSION	3

enum {
	XNET_IOV_LIMIT = 4
};

/* base_hdr::op_data */
enum {
	/* backward compatible value */
	XNET_OP_ACK = 2, /* indicates ack message - should be a flag */
};

/* Flags */
#define XNET_REMOTE_CQ_DATA	(1 << 0)
/* not used XNET_TRANSMIT_COMPLETE	(1 << 1) */
#define XNET_DELIVERY_COMPLETE	(1 << 2)
#define XNET_COMMIT_COMPLETE	(1 << 3)
#define XNET_TAGGED		(1 << 7)

struct xnet_base_hdr {
	uint8_t			version;
	uint8_t			op;
	uint16_t		flags;
	uint8_t			op_data;
	uint8_t			rma_iov_cnt;
	uint8_t			hdr_size;
	union {
		uint8_t		rsvd;
		uint8_t		id; /* debug */
	};
	uint64_t		size;
};

struct xnet_tag_hdr {
	struct xnet_base_hdr	base_hdr;
	uint64_t		tag;
};

struct xnet_cq_data_hdr {
	struct xnet_base_hdr 	base_hdr;
	uint64_t		cq_data;
};

struct xnet_tag_data_hdr {
	struct xnet_cq_data_hdr	cq_data_hdr;
	uint64_t		tag;
};

/* Maximum header is scatter RMA with CQ data */
#define XNET_MAX_HDR (sizeof(struct xnet_cq_data_hdr) + \
		     sizeof(struct ofi_rma_iov) * XNET_IOV_LIMIT)


#endif //_XNET_PROTO_H_

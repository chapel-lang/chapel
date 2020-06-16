/*
 * Copyright (c) 2016-2017 Intel Corporation. All rights reserved.
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

#ifndef _OFI_PROTO_H_
#define _OFI_PROTO_H_

#include "config.h"

#include <stdint.h>
#include <stddef.h>

#include <rdma/fi_rma.h>


#ifdef __cplusplus
extern "C" {
#endif


#define OFI_CTRL_VERSION	2

/* ofi_ctrl_hdr::type */
enum {
	ofi_ctrl_connreq,
	ofi_ctrl_connresp,
	ofi_ctrl_start_data,
	ofi_ctrl_data,
	ofi_ctrl_large_data,
	ofi_ctrl_ack,
	ofi_ctrl_nack,
	ofi_ctrl_discard,
	ofi_ctrl_seg_data,
	ofi_ctrl_atomic,
	ofi_ctrl_atomic_resp,
};

/*
 * Control message header.  For segmentation and reassembly, reliability,
 * rendezvous protocol, acks, and communication setup.
 *
 * version: OFI_CTRL_VERSION
 * type
 * conn_id: Communication identifier.  Conn_id values are exchanged between
 *     peer endpoints as part of communication setup.  This field is valid
 *     as part of the first message in any data transfer.
 * msg_id: This is the sender's identifier for a message.
 *     Unique number identifying all segments of a message
 *     Message id can be formed using an equation similar to:
 *     (seq_no++ << tx size) | tx_key
 * seg_size:
 *     Data packets - size of current message, in bytes.
 *     Large data packets - size of current message, 2 ^ seg_size, in bytes
 *     Ctrl packets - number of segments in window allowed past seg_no.
 * seg_no:
 *     Data packets - position 0..(n-1) of segment in current message.
 *     Ctrl packets - last segment ack'ed.
 * conn_data: Connection specific data.  This may be set to the index
 *     of the transmit endpoint's address in its local AV, which may
 *     be used as a hint at the Rx side to locate the Tx EP address in
 *     its AV.  The assumption is that all addresses were inserted into
 *     all AVs across the fabric using a copied array.  (This is an
 *     optimization hint only; the peer validates the actual entry.)
 * rx_key: This is the receiver's identifier for a message (receive side
 *     equivalent of msg_id).  Key returned by the Rx side, that the
 *     Tx side includes in subsequent packets.  This field is used for
 *     rendezvous protocol.
 *     The rx_key may be formed similar to message_id.
 * ctrl_data: This is provider specific data for remote side
 */
struct ofi_ctrl_hdr {
	uint8_t				version;
	uint8_t				type;
	uint16_t			seg_size;
	uint32_t			seg_no;
	uint64_t			conn_id;
	uint64_t			msg_id;
	union {
		uint64_t		conn_data;
		uint64_t		rx_key;
		uint64_t		ctrl_data;
	};
};


#define OFI_OP_VERSION	2

/*
 * Basic command opcode. ofi_op_hdr::op
 * Intent is that RX can use opcode + control as indices into a function
 * pointer array for message processing (after validating values).
 */
enum {
	ofi_op_msg,
	ofi_op_tagged,
	ofi_op_read_req,
	ofi_op_read_rsp,
	ofi_op_write,
	ofi_op_write_async,
	ofi_op_atomic,
	ofi_op_atomic_fetch,
	ofi_op_atomic_compare,
	ofi_op_read_async,
	ofi_op_max,
};

#define OFI_REMOTE_CQ_DATA	(1 << 0)
#define OFI_TRANSMIT_COMPLETE	(1 << 1)
#define OFI_DELIVERY_COMPLETE	(1 << 2)
#define OFI_COMMIT_COMPLETE	(1 << 3)

/*
 * Common command header
 *
 * version: OFI_OP_VERSION
 * rxid: RX index for scalable endpoints
 * op:
 * op_data: implementation specific
 * tx_key: Tx request identifier for command
 * flags: Command flags
 * size: Size of data transfer
 * data: Remote CQ data, if available
 * tag: Message tag, used for tagged operations only
 * iov_count: Count of destination iov, used for RMA operations
 * atomic: Control fields for atomic operations
 * remote_idx: Tx request identifier of remote side
 * resv: Reserved, used for msg operations
 */
struct ofi_op_hdr {
	uint8_t			version;
	uint8_t			rx_index;
	uint8_t			op;
	uint8_t			op_data;
	uint32_t		flags;

	uint64_t		size;
	uint64_t		data;
	union {
		uint64_t	tag;
		uint8_t		iov_count;
		struct {
			uint8_t	datatype;
			uint8_t	op;
			uint8_t ioc_count;
		} atomic;
		uint64_t	remote_idx;
		uint64_t	resv;
	};
};

struct ofi_iov {
	uint64_t		addr;
	uint64_t		len;
};

struct ofi_rma_iov {
	uint64_t		addr;
	uint64_t		len;
	uint64_t		key;
};

struct ofi_rma_ioc {
	uint64_t		addr;
	uint64_t		count;
	uint64_t		key;
};

#define OFI_CMD_SIZE		64	/* to align with 64-byte cache line */
#define OFI_CMD_DATA_LEN	(OFI_CMD_SIZE - sizeof(struct ofi_ctrl_hdr))


#ifdef __cplusplus
}
#endif

#endif /* _OFI_PROTO_H_ */

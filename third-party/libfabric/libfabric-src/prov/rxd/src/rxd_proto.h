/*
 * Copyright (c) 2015-2018 Intel Corporation, Inc.  All rights reserved.
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

#include <ofi.h>
#include <ofi_proto.h>

#ifndef _RXD_PROTO_H_
#define _RXD_PROTO_H_

#define RXD_IOV_LIMIT		4
#define RXD_NAME_LENGTH		64

/* Values below are part of the wire protocol
   Reserved values are unused but defined for compatibility */
#define RXD_FOREACH_TYPE(FUNC)		\
	FUNC(RXD_MSG),			\
	FUNC(RXD_TAGGED),		\
	FUNC(RXD_READ_REQ),		\
	FUNC(RXD_RESV_1),		\
	FUNC(RXD_WRITE),		\
	FUNC(RXD_RESV_2),		\
	FUNC(RXD_ATOMIC),		\
	FUNC(RXD_ATOMIC_FETCH),		\
	FUNC(RXD_ATOMIC_COMPARE),	\
	FUNC(RXD_RTS),			\
	FUNC(RXD_CTS),			\
	FUNC(RXD_ACK),			\
	FUNC(RXD_DATA),			\
	FUNC(RXD_DATA_READ),		\
	FUNC(RXD_NO_OP)

enum rxd_pkt_type {
	RXD_FOREACH_TYPE(OFI_ENUM_VAL)
};

extern char *rxd_pkt_type_str[];

/* Base header: all packets must start with base_hdr
 * 	- version: RXD version the app is using
 * 	- type: type of message (see above definitions)
 * 	- flags: any neccesary flags including hdr flags indicating which headers
 * 		 are included in the packet
 * 	- peer: RX side peer address (exchanged during RTS-CTS process)
 * 	- seq_no: sequence number (per peer)
 */
struct rxd_base_hdr {
	uint8_t		version;
	uint8_t		type;
	uint16_t	flags;
	uint32_t	peer;
	uint64_t	seq_no;
};

/*
 * Extended header: used for large transfers and ACKs
 * 	- tx_id/rx_id:
 * 		- for large messages: the tx/rx index for the messages
 * 		- for ACKs: the tx/rx index which the ACK corresponds to
 */
struct rxd_ext_hdr {
	uint32_t	tx_id;
	uint32_t	rx_id;
	uint64_t	seg_no;
};

/*
 * Ready to send: initialize peer communication and exchange addressing info
 * 	- rts_addr: local address for peer sending RTS
 * 	- source: name of transmitting endpoint for peer to add to AV
 */
struct rxd_rts_pkt {
	struct rxd_base_hdr	base_hdr;
	uint64_t		rts_addr;
	uint8_t			source[RXD_NAME_LENGTH];
};

/*
 * Clear to send: response to RTS request
 * 	- rts_addr: peer address packet is responding to
 * 	- cts_addr: local address for peer
 */
struct rxd_cts_pkt {
	struct	rxd_base_hdr	base_hdr;
	uint64_t		rts_addr;
	uint64_t		cts_addr;
};

/*
 * ACK: to signal received packets and send tx/rx id info
 */
struct rxd_ack_pkt {
	struct rxd_base_hdr	base_hdr;
	struct rxd_ext_hdr	ext_hdr;
	//TODO fill in more fields? Selective ack?
};

/*
 * Data: send larger block of data using known tx/rx ids for matching
 */
struct rxd_data_pkt {
	struct rxd_base_hdr	base_hdr;
	struct rxd_ext_hdr	ext_hdr;

	char			msg[];
};

/*
 * The below five headers are used for op pkts and can be used in combination.
 * The presence of each header is determined by either op type or flags (in base_hr).
 * The op header order is as follows:
 * base_hdr (present for all packets)
 *
 * tag_hdr: for all tagged messages
 * 	- signaled by base_hdr->flags & RXD_TAG_HDR
 * data_hdr: for messages carrying remote CQ data
 * 	- signaled by base_hdr->flags & RXD_REMOTE_CQ_DATA
 * sar_hdr: for all messages requiring more than one packet
 * 	- lack of the sar_hdr is signaled by base_hdr->flags & RXD_INLINE
 * rma_hdr: for FI_RMA and FI_ATOMIC operations
 * 	- signaled by base_hdr->type = RXD_READ_REQ, RXD_WRITE, RXD_ATOMIC,
 * 	  RXD_ATOMIC_FETCH, and RXD_ATOMIC_COMPARE
 * atom_hdr: for FI_ATOMIC operations
 * 	- signaled by base_hdr->type = RXD_ATOMIC, RXD_ATOMIC_FETCH,
 * 	  RXD_ATOMIC_COMPARE
 * 
 * Any data in the packet following these headers is part of the incoming packet message
 */

struct rxd_sar_hdr {
	uint64_t		size;
	uint64_t		num_segs;
	uint32_t		tx_id;
	uint8_t			iov_count;
	uint8_t			resv[3];
};

struct rxd_tag_hdr {
	uint64_t	tag;
};

struct rxd_data_hdr {
	uint64_t	cq_data;
};

struct rxd_rma_hdr {
	struct ofi_rma_iov	rma[RXD_IOV_LIMIT];
};

struct rxd_atom_hdr {
	uint32_t	datatype;
	uint32_t	atomic_op;
};

#endif

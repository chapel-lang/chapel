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

#ifndef _RXR_PROTO_V4_H
#define _RXR_PROTO_V4_H

/*
 * This header file contains constants, flags and data structures
 * that are defined in EFA RDM protocol v4. Any change to this
 * header file can potentially break backward compatibility, thus
 * need to be reviewed with extra care.
 *
 * The section number in this file refers to the sections
 * in EFA RDM protocol version 4.
 */

#define RXR_PROTOCOL_VERSION	(4)

/* raw address format. (section 1.4) */
#define EFA_GID_LEN	16

struct efa_ep_addr {
	uint8_t			raw[EFA_GID_LEN];
	uint16_t		qpn;
	uint16_t		pad;
	uint32_t		qkey;
	struct efa_ep_addr	*next;
};

#define EFA_EP_ADDR_LEN sizeof(struct efa_ep_addr)

/*
 * Extra Feature/Request Flags (section 2.1)
 */
#define RXR_EXTRA_FEATURE_RDMA_READ			BIT_ULL(0)
#define RXR_EXTRA_FEATURE_DELIVERY_COMPLETE 		BIT_ULL(1)
#define RXR_EXTRA_REQUEST_CONSTANT_HEADER_LENGTH	BIT_ULL(2)
#define RXR_EXTRA_REQUEST_CONNID_HEADER			BIT_ULL(3)
#define RXR_EXTRA_FEATURE_RUNT				BIT_ULL(4)
#define RXR_NUM_EXTRA_FEATURE_OR_REQUEST		5
#define RXR_MAX_NUM_EXINFO	(256)

/*
 * Packet type ID of each packet type (section 1.3)
 *
 * Changing packet type ID would break backward compatiblity thus is strictly
 * prohibited.
 *
 * New packet types can be added with introduction of an extra feature
 * (section 2.1)
 */
#define RXR_RETIRED_RTS_PKT		1
#define RXR_RETIRED_CONNACK_PKT		2
#define RXR_CTS_PKT			3
#define RXR_DATA_PKT			4
#define RXR_READRSP_PKT			5
#define RXR_RMA_CONTEXT_PKT		6
#define RXR_EOR_PKT			7
#define RXR_ATOMRSP_PKT 	        8
#define RXR_HANDSHAKE_PKT		9
#define RXR_RECEIPT_PKT 		10

#define RXR_REQ_PKT_BEGIN		64
#define RXR_BASELINE_REQ_PKT_BEGIN	64
#define RXR_EAGER_MSGRTM_PKT		64
#define RXR_EAGER_TAGRTM_PKT		65
#define RXR_MEDIUM_MSGRTM_PKT		66
#define RXR_MEDIUM_TAGRTM_PKT		67
#define RXR_LONGCTS_MSGRTM_PKT		68
#define RXR_LONGCTS_TAGRTM_PKT		69
#define RXR_EAGER_RTW_PKT		70
#define RXR_LONGCTS_RTW_PKT		71
#define RXR_SHORT_RTR_PKT		72
#define RXR_LONGCTS_RTR_PKT		73
#define RXR_WRITE_RTA_PKT		74
#define RXR_FETCH_RTA_PKT		75
#define RXR_COMPARE_RTA_PKT		76
#define RXR_BASELINE_REQ_PKT_END	77

#define RXR_EXTRA_REQ_PKT_BEGIN		128
#define RXR_LONGREAD_MSGRTM_PKT		128
#define RXR_LONGREAD_TAGRTM_PKT		129
#define RXR_LONGREAD_RTW_PKT		130
#define RXR_READ_RTR_PKT		131

#define RXR_DC_REQ_PKT_BEGIN		132
#define RXR_DC_EAGER_MSGRTM_PKT 	133
#define RXR_DC_EAGER_TAGRTM_PKT 	134
#define RXR_DC_MEDIUM_MSGRTM_PKT 	135
#define RXR_DC_MEDIUM_TAGRTM_PKT 	136
#define RXR_DC_LONGCTS_MSGRTM_PKT  	137
#define RXR_DC_LONGCTS_TAGRTM_PKT  	138
#define RXR_DC_EAGER_RTW_PKT    	139
#define RXR_DC_LONGCTS_RTW_PKT     	140
#define RXR_DC_WRITE_RTA_PKT    	141
#define RXR_DC_REQ_PKT_END		142

#define RXR_RUNT_PKT_BEGIN		142
#define RXR_RUNTCTS_MSGRTM_PKT		142
#define RXR_RUNTCTS_TAGRTM_PKT		143
#define RXR_RUNTCTS_RTW_PKT		144
#define RXR_RUNTREAD_MSGRTM_PKT		145
#define RXR_RUNTREAD_TAGRTM_PKT		146
#define RXR_RUNTREAD_RTW_PKT		147
#define RXR_RUNT_PKT_END		148
#define RXR_EXTRA_REQ_PKT_END   	148

/*
 *  Packet fields common to all rxr packets. The other packet headers below must
 *  be changed if this is updated.
 */
struct rxr_base_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
};

#if defined(static_assert) && defined(__x86_64__)
static_assert(sizeof(struct rxr_base_hdr) == 4, "rxr_base_hdr check");
#endif

/* Universal flags that can be applied on "rxr_base_hdr.flags".
 *
 * Universal flags start from the last bit and goes backwards.
 * Because "rxr_base_hdr.flags" is a 16-bits integer, the
 * last bit is the 15th bit.
 * Other than universal flags, each packet type defines its
 * own set of flags, which generally starts from the 0th bit
 * in "rxr_base_hdr.flags".
 */

/* indicate this packet has the sender connid */
#define RXR_PKT_CONNID_HDR		BIT_ULL(15)

struct efa_rma_iov {
	uint64_t		addr;
	size_t			len;
	uint64_t		key;
};

/*
 * @breif header format of CTS packet (Packet Type ID 3)
 *
 * CTS is used in long-CTS sub-protocols for flow control.
 *
 * It is sent from receiver to sender, and contains number of bytes
 * receiver is ready to receive.
 *
 * long-CTS is used not only by two-sided communication but also
 * by emulated write and emulated read protocols.
 *
 * In emulated write, requester is sender, and responder is receiver.
 *
 * In emulated read, requester is receiver, and responder is sender.
 */
struct rxr_cts_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	union {
		uint32_t connid; /* sender connection ID, set when RXR_PKT_CONNID_HDR is on */
		uint32_t padding; /* otherwise, a padding space to 8 bytes */
	};
	uint32_t send_id; /* ID of the send opertaion on sender side */
	uint32_t recv_id; /* ID of the receive operatin on receive side */
	uint64_t recv_length; /* number of bytes receiver is ready to receive */
};

#if defined(static_assert) && defined(__x86_64__)
static_assert(sizeof(struct rxr_cts_hdr) == 24, "rxr_cts_hdr check");
#endif

/* this flag is to indicated the CTS is the response of a RTR packet */
#define RXR_CTS_READ_REQ		BIT_ULL(7)


/*
 * @brief optional connid header for DATA packet
 */
struct rxr_data_opt_connid_hdr {
	uint32_t connid;
	uint32_t padding;
};

/*
 * @brief header format of DATA packet header (Packet Type ID 4)
 *
 * DATA is used in long-CTS sub-protocols.
 *
 * It is sent from sender to receiver, and contains a segment
 * of application data.
 *
 * long-CTS is used not only by two-sided communication but also
 * by emulated write and emulated read protocols.
 *
 * In emulated write, requester is sender, and responder is receiver.
 *
 * In emulated read, requester is receiver, and responder is sender.
 */
struct rxr_data_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t recv_id; /* ID of the receive operation on receiver */
	uint64_t seg_length;
	uint64_t seg_offset;
	/* optional connid header, present when RXR_PKT_CONNID_HDR is on */
	struct rxr_data_opt_connid_hdr connid_hdr[0];
};

#if defined(static_assert) && defined(__x86_64__)
static_assert(sizeof(struct rxr_data_hdr) == 24, "rxr_data_hdr check");
#endif

/*
 *  @brief READRSP packet header (Packet Type ID 5)
 *
 *  READRSP is sent from read responder to read requester, and it contains
 *  application data.
 */
struct rxr_readrsp_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	union {
		uint32_t connid; /* sender connection ID, set when RXR_PKT_CONNID_HDR is on */
		uint32_t padding; /* otherwise, a padding space to 8 bytes boundary */
	};
	uint32_t recv_id; /* ID of the receive operation on the read requester, from rtr packet */
	uint32_t send_id; /* ID of the send operation on the read responder, will be included in CTS packet */
	uint64_t seg_length;
};

#if defined(static_assert) && defined(__x86_64__)
static_assert(sizeof(struct rxr_readrsp_hdr) == sizeof(struct rxr_data_hdr), "rxr_readrsp_hdr check");
#endif

struct rxr_readrsp_pkt {
	struct rxr_readrsp_hdr hdr;
	char data[];
};

/*
 * RMA Context pkt (Packe Type ID 6) is a special type
 * of packet. It is used as the context of an RMA
 * operatation, thus is not sent over wire. Therefore
 * its header format is not part of protocol. In doc,
 * the packet type ID 6 is marked as reserved
 */

/*
 * @brief format of the EOR packet. (Packet Type ID 7)
 *
 * EOR packet is used in long-read sub-protocols, which is
 * part of the extra request: RDMA read based data transfer.
 *
 * It is sent from receiver to sender, to notify
 * the finish of data transfer.
 *
 * long-read is used not only by two-sided communication but also
 * by emulated write.
 *
 * In emulated write, requester is sender, and responder is receiver.
 */
struct rxr_eor_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t send_id; /* ID of the send operation on sender */
	uint32_t recv_id; /* ID of the receive operation on receiver */
	union {
		uint32_t connid; /* sender connection ID, optional, set whne RXR_PKT_CONNID_HDR is on */
		uint32_t padding; /* otherwise, a padding space to 8 bytes boundary */
	};
};

#if defined(static_assert) && defined(__x86_64__)
static_assert(sizeof(struct rxr_eor_hdr) == 16, "rxr_eor_hdr check");
#endif

/**
 * @brief header format of ATOMRSP packet. (Packet Type ID 8)
 * ATOMRSP packet is used in emulated fetch/compare atomic sub-protocol.
 * 
 * It is sent from responder to requester, which contains the response
 * to a fetch/compare atomic request
 */
struct rxr_atomrsp_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	union {
		uint32_t connid; /* sender connid. set when RXR_PKT_CONNID_HDR is on in flags */
		uint32_t padding; /* otherwise, a padding space to 8 bytes boundary */
	};
	uint32_t reserved;
	uint32_t recv_id;
	uint64_t seg_length;
};

#if defined(static_assert) && defined(__x86_64__)
static_assert(sizeof(struct rxr_atomrsp_hdr) == 24, "rxr_atomrsp_hdr check");
#endif

struct rxr_atomrsp_pkt {
	struct rxr_atomrsp_hdr hdr;
	char data[];
};

/**
 * @breif header format of a HANDSHAKE packet
 *
 * HANDSHAKE packet is used in the handshake sub-protocol.
 *
 * Upon receiving 1st packet from a peer, an endpoint will
 * send a HANDSHAKE packet back, which contains its capablity bits
 */
struct rxr_handshake_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	/* nextra_p3 is number of members in extra_info plus 3.
	 * The "p3" part was introduced for backward compatibility.
	 * See protocol v4 document section 2.1 for detail.
	 */
	uint32_t nextra_p3;
	uint64_t extra_info[0];
};

#if defined(static_assert) && defined(__x86_64__)
static_assert(sizeof(struct rxr_handshake_hdr) == 8, "rxr_handshake_hdr check");
#endif

struct rxr_handshake_opt_connid_hdr {
	uint32_t connid;
	uint32_t padding; /* padding to 8 bytes boundary */
};

#if defined(static_assert) && defined(__x86_64__)
static_assert(sizeof(struct rxr_handshake_opt_connid_hdr) == 8, "rxr_handshake_opt_connid_hdr check");
#endif

/* @brief header format of RECEIPT packet */
struct rxr_receipt_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t tx_id;
	uint32_t msg_id;
	union {
		uint32_t connid; /* sender connection ID, set when RXR_PKT_CONNID_HDR is on */
		uint32_t padding; /* otherwise, a padding space to 8 bytes */
	};
};

/*
 * The following are REQ packets related constants, flags
 * and data structures.
 *
 * REQ packets can be classifed into 4 categories (section 3.1):
 *    RTM (Request To Message) is used by message
 *    RTW (Request To Write) is used by RMA write
 *    RTR (Request To Read) is used by RMA read
 *    RTA (Request To Atomic) is used by Atomic
 */


/*
 * REQ Packets common Header Flags (section 3.1)
 */
#define RXR_REQ_OPT_RAW_ADDR_HDR	BIT_ULL(0)
#define RXR_REQ_OPT_CQ_DATA_HDR		BIT_ULL(1)
#define RXR_REQ_MSG			BIT_ULL(2)
#define RXR_REQ_TAGGED			BIT_ULL(3)
#define RXR_REQ_RMA			BIT_ULL(4)
#define RXR_REQ_ATOMIC			BIT_ULL(5)

/*
 * optional headers for REQ packets
 */
struct rxr_req_opt_raw_addr_hdr {
	uint32_t addr_len;
	char raw_addr[0];
};

struct rxr_req_opt_cq_data_hdr {
	int64_t cq_data;
};

struct rxr_req_opt_connid_hdr {
	uint32_t connid; /* sender's connection ID */
};

#define RXR_REQ_OPT_HDR_ALIGNMENT 8
#define RXR_REQ_OPT_RAW_ADDR_HDR_SIZE (((sizeof(struct rxr_req_opt_raw_addr_hdr) + EFA_EP_ADDR_LEN - 1)/RXR_REQ_OPT_HDR_ALIGNMENT + 1) * RXR_REQ_OPT_HDR_ALIGNMENT)

/*
 * Base header for all RTM packets
 */
struct rxr_rtm_base_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	uint32_t msg_id;
};

/**
 * @brief header format of EAGER_MSGRTM packet (Packet Type ID 64)
 */
struct rxr_eager_msgrtm_hdr {
	struct rxr_rtm_base_hdr hdr;
};


/**
 * @brief header format of EAGER_TAGRTM packet (Packet Type ID 65)
 */
struct rxr_eager_tagrtm_hdr {
	struct rxr_rtm_base_hdr hdr;
	uint64_t tag;
};

struct rxr_medium_rtm_base_hdr {
	struct rxr_rtm_base_hdr hdr;
	uint64_t msg_length;
	uint64_t seg_offset;
};

/**
 * @brief header format of MEDIUM_MSGRTM packet (Packet Type ID 66)
 */
struct rxr_medium_msgrtm_hdr {
	struct rxr_medium_rtm_base_hdr hdr;
};

/**
 * @brief header format of MEDIUM_TAGRTM packet (Packet Type ID 67)
 */
struct rxr_medium_tagrtm_hdr {
	struct rxr_medium_rtm_base_hdr hdr;
	uint64_t tag;
};

struct rxr_longcts_rtm_base_hdr {
	struct rxr_rtm_base_hdr hdr;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t credit_request;
};

/**
 * @brief header format of LONGCTS_MSGRTM packet (Packet Type ID 68)
 */
struct rxr_longcts_msgrtm_hdr {
	struct rxr_longcts_rtm_base_hdr hdr;
};

/**
 * @brief header format of LONGCTS_TAGRTM packet (Packet Type ID 69)
 */
struct rxr_longcts_tagrtm_hdr {
	struct rxr_longcts_rtm_base_hdr hdr;
	uint64_t tag;
};

struct rxr_rtw_base_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
};

/**
 * @brief header format of EAGER_RTW packet (Packet Type ID 70)
 */
struct rxr_eager_rtw_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	struct efa_rma_iov rma_iov[0];
};

/**
 * @brief header format of LONGCTS_RTW packet (Packet Type ID 71)
 */
struct rxr_longcts_rtw_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t credit_request;
	struct efa_rma_iov rma_iov[0];
};

/*
 * rxr_rtr_hdr is used by both SHORT_RTR (Packet Type ID 72)
 * and LONGCTS_RTR (Packet Type ID 73)
 */
struct rxr_rtr_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t recv_id; /* ID of the receive operation of the read requester, will be included in DATA/READRSP header */
	uint32_t recv_length; /* number of bytes that the read requester is ready to receive */
	struct efa_rma_iov rma_iov[0];
};

/* @brief rxr_rta_hdr are shared by 4 types of RTA:
 *    WRITE_RTA (Packet Type ID 74),
 *    FETCH_RTA (Packet Type ID 75),
 *    COMPARE_RTA (Packet Type ID 76) and
 *    DC_WRTIE_RTA (Packe Type ID 141)
 */
struct rxr_rta_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	uint32_t msg_id;
	/* end of rtm_base_hdr, atomic packet need msg_id for reordering */
	uint32_t rma_iov_count;
	uint32_t atomic_datatype;
	uint32_t atomic_op;
	union {
		/* padding is used by WRITE_RTA, align to 8 bytes */
		uint32_t padding;
		/* recv_id is used by FETCH_RTA and COMPARE_RTA. It is the ID of the receive operation on atomic requester,
		 * it will be included in ATOMRSP packet header.
		 */
		uint32_t recv_id;
		/* send_id is used by DC_WRITE_RTA. It is ID of the send operation on the atomic requester.
		 * It will be included in RECEIPT packet header.
		 */
		uint32_t send_id;
	};

	struct efa_rma_iov rma_iov[0];
};

/*
 * Extra request: RDMA read based data transfer (section 4.1)
 */
struct rxr_longread_rtm_base_hdr {
	struct rxr_rtm_base_hdr hdr;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t read_iov_count;
};

/**
 * @brief header format of LONGREAD_MSGRTM (Packet Type ID 128)
 */
struct rxr_longread_msgrtm_hdr {
	struct rxr_longread_rtm_base_hdr hdr;
};

/**
 * @brief header format of LONGREAD_MSGRTM (Packet Type ID 129)
 */
struct rxr_longread_tagrtm_hdr {
	struct rxr_longread_rtm_base_hdr hdr;
	uint64_t tag;
};

/**
 * @brief header format of LONGREAD_MSGRTM (Packet Type ID 130)
 */
struct rxr_longread_rtw_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t read_iov_count;
	struct efa_rma_iov rma_iov[0];
};

/*
 * Extra requester: delivery complete (section 4.2)
 */

struct rxr_dc_eager_rtm_base_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	uint32_t msg_id;
	uint32_t send_id;
	uint32_t padding;
};

/**
 * @brief header format of a DC_EAGER_MSGRTM packet
 */
struct rxr_dc_eager_msgrtm_hdr {
	struct rxr_dc_eager_rtm_base_hdr hdr;
};

/**
 * @brief header format of a DC_EAGER_TAGRTM packet
 */
struct rxr_dc_eager_tagrtm_hdr {
	struct rxr_dc_eager_rtm_base_hdr hdr;
	uint64_t tag;
};

struct rxr_dc_medium_rtm_base_hdr {
	struct rxr_rtm_base_hdr hdr;
	uint32_t send_id;
	uint32_t padding;
	uint64_t msg_length;
	uint64_t seg_offset;
};

/**
 * @brief header format of a DC_MEDIUM_MSGRTM packet
 */
struct rxr_dc_medium_msgrtm_hdr {
	struct rxr_dc_medium_rtm_base_hdr hdr;
};

/**
 * @brief header format of a DC_MEDIUM_TAGRTM packet
 */
struct rxr_dc_medium_tagrtm_hdr {
	struct rxr_dc_medium_rtm_base_hdr hdr;
	uint64_t tag;
};

/**
 * @brief header format of a DC_LONGCTS_MSGRTM packet
 */
struct rxr_dc_longcts_msgrtm_hdr {
	struct rxr_longcts_rtm_base_hdr hdr;
};

/**
 * @brief header format of a DC_LONGCTS_TAGRTM packet
 */
struct rxr_dc_longcts_tagrtm_hdr {
	struct rxr_longcts_rtm_base_hdr hdr;
	uint64_t tag;
};

/**
 * @brief header format of a DC_EAGER_RTW packet
 */
struct rxr_dc_eager_rtw_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	/* end of rxr_rtw_base_hdr */
	uint32_t send_id;
	uint32_t padding;
	struct efa_rma_iov rma_iov[0];
};

/**
 * @brief header format of a DC_LONGCTS_RTW packet
 */
struct rxr_dc_longcts_rtw_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t credit_request;
	struct efa_rma_iov rma_iov[0];
};

/* DC_WRITE_RTA header format is merged into rxr_rta_hdr */

/* Extra feature runting protocols related headers */

struct rxr_runtcts_rtm_base_hdr {
	struct rxr_rtm_base_hdr hdr;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t credit_request;
	uint64_t seg_offset;
	uint64_t runt_length;
};

/**
 * @brief header format of RUNTCTS_MSGRTM (Packet Type ID 142)
 */
struct rxr_runtcts_msgrtm_hdr {
	struct rxr_runtcts_rtm_base_hdr hdr;
};

/**
 * @brief header format of RUNTCTS_TAGRTM (Packet Type ID 143)
 */
struct rxr_runtcts_tagrtm_hdr {
	struct rxr_runtcts_rtm_base_hdr hdr;
	uint64_t tag;
};

/**
 * @brief header format of RUNTCTS_RTW (Packet Type ID 144)
 */
struct rxr_runtcts_rtw_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t credit_request;
	uint64_t seg_offset;
	uint64_t runt_length;
	struct efa_rma_iov rma_iov[0];
};

struct rxr_runtread_rtm_base_hdr {
	struct rxr_rtm_base_hdr hdr;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t read_iov_count;
	uint64_t seg_offset;
	uint64_t runt_length;
};

/**
 * @brief header format of RUNTREAD_MSGRTM (Packet Type ID 145)
 */
struct rxr_runtread_msgrtm_hdr {
	struct rxr_runtread_rtm_base_hdr hdr;
};

/**
 * @brief header format of RUNTREAD_MSGRTM (Packet Type ID 146)
 */
struct rxr_runtread_tagrtm_hdr {
	struct rxr_runtread_rtm_base_hdr hdr;
	uint64_t tag;
};

/**
 * @brief header format of RUNTREAD_RTW (Packet Type ID 147)
 */
struct rxr_runtread_rtw_hdr {
	uint8_t type;
	uint8_t version;
	uint16_t flags;
	/* end of rxr_base_hdr */
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t read_iov_count;
	uint64_t seg_offset;
	uint64_t runt_length;
	struct efa_rma_iov rma_iov[0];
};

#endif

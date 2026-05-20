/* Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */
/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */

#ifndef _EFA_RDM_PROTO_V4_H
#define _EFA_RDM_PROTO_V4_H

/*
 * This header file contains constants, flags and data structures
 * that are defined in EFA RDM protocol v4. Any change to this
 * header file can potentially break backward compatibility, thus
 * need to be reviewed with extra care.
 *
 * The section number in this file refers to the sections
 * in EFA RDM protocol version 4.
 */

#define EFA_RDM_PROTOCOL_VERSION	(4)



/*
 * Extra Feature/Request Flags (section 2.1)
 */
#define EFA_RDM_EXTRA_FEATURE_RDMA_READ			BIT_ULL(0)
#define EFA_RDM_EXTRA_FEATURE_DELIVERY_COMPLETE 	BIT_ULL(1)
#define EFA_RDM_EXTRA_REQUEST_CONSTANT_HEADER_LENGTH	BIT_ULL(2)
#define EFA_RDM_EXTRA_REQUEST_CONNID_HEADER		BIT_ULL(3)
#define EFA_RDM_EXTRA_FEATURE_RUNT			BIT_ULL(4)
#define EFA_RDM_EXTRA_FEATURE_RDMA_WRITE		BIT_ULL(5)
#define EFA_RDM_EXTRA_FEATURE_READ_NACK		BIT_ULL(6)
#define EFA_RDM_EXTRA_FEATURE_REQUEST_USER_RECV_QP	BIT_ULL(7)
#define EFA_RDM_EXTRA_FEATURE_UNSOLICITED_WRITE_RECV	BIT_ULL(8)
#define EFA_RDM_NUM_EXTRA_FEATURE_OR_REQUEST		9
/*
 * The length of 64-bit extra_info array used in efa_rdm_ep
 * and efa_rdm_peer
 * 4 means 64*4=256 bits of extra features or requests
 */
#define EFA_RDM_MAX_NUM_EXINFO				(4)

#define EFA_RDM_HEADERLESS_PKT 0 /**< Sentinel value for headerless packets */

/*
 * Packet type ID of each packet type (section 1.3)
 *
 * Changing packet type ID would break backward compatiblity thus is strictly
 * prohibited.
 *
 * New packet types can be added with introduction of an extra feature
 * (section 2.1)
 */
#define EFA_RDM_RETIRED_RTS_PKT		1
#define EFA_RDM_RETIRED_CONNACK_PKT	2
#define EFA_RDM_CTS_PKT			3
#define EFA_RDM_CTSDATA_PKT		4
#define EFA_RDM_READRSP_PKT		5
#define EFA_RDM_RMA_CONTEXT_PKT		6
#define EFA_RDM_EOR_PKT			7
#define EFA_RDM_ATOMRSP_PKT 	        8
#define EFA_RDM_HANDSHAKE_PKT		9
#define EFA_RDM_RECEIPT_PKT 		10
#define EFA_RDM_READ_NACK_PKT		11

#define EFA_RDM_REQ_PKT_BEGIN		64
#define EFA_RDM_BASELINE_REQ_PKT_BEGIN	64
#define EFA_RDM_EAGER_MSGRTM_PKT	64
#define EFA_RDM_EAGER_TAGRTM_PKT	65
#define EFA_RDM_MEDIUM_MSGRTM_PKT	66
#define EFA_RDM_MEDIUM_TAGRTM_PKT	67
#define EFA_RDM_LONGCTS_MSGRTM_PKT	68
#define EFA_RDM_LONGCTS_TAGRTM_PKT	69
#define EFA_RDM_EAGER_RTW_PKT		70
#define EFA_RDM_LONGCTS_RTW_PKT		71
#define EFA_RDM_SHORT_RTR_PKT		72
#define EFA_RDM_LONGCTS_RTR_PKT		73
#define EFA_RDM_WRITE_RTA_PKT		74
#define EFA_RDM_FETCH_RTA_PKT		75
#define EFA_RDM_COMPARE_RTA_PKT		76
#define EFA_RDM_BASELINE_REQ_PKT_END	77

#define EFA_RDM_EXTRA_REQ_PKT_BEGIN	128
#define EFA_RDM_LONGREAD_MSGRTM_PKT	128
#define EFA_RDM_LONGREAD_TAGRTM_PKT	129
#define EFA_RDM_LONGREAD_RTW_PKT	130
#define EFA_RDM_READ_RTR_PKT		131

#define EFA_RDM_DC_REQ_PKT_BEGIN	132
#define EFA_RDM_DC_EAGER_MSGRTM_PKT 	133
#define EFA_RDM_DC_EAGER_TAGRTM_PKT 	134
#define EFA_RDM_DC_MEDIUM_MSGRTM_PKT 	135
#define EFA_RDM_DC_MEDIUM_TAGRTM_PKT 	136
#define EFA_RDM_DC_LONGCTS_MSGRTM_PKT  	137
#define EFA_RDM_DC_LONGCTS_TAGRTM_PKT  	138
#define EFA_RDM_DC_EAGER_RTW_PKT    	139
#define EFA_RDM_DC_LONGCTS_RTW_PKT     	140
#define EFA_RDM_DC_WRITE_RTA_PKT    	141
#define EFA_RDM_DC_REQ_PKT_END		142

#define EFA_RDM_RUNT_PKT_BEGIN		142
#define EFA_RDM_RUNTCTS_MSGRTM_PKT	142
#define EFA_RDM_RUNTCTS_TAGRTM_PKT	143
#define EFA_RDM_RUNTCTS_RTW_PKT		144
#define EFA_RDM_RUNTREAD_MSGRTM_PKT	145
#define EFA_RDM_RUNTREAD_TAGRTM_PKT	146
#define EFA_RDM_RUNTREAD_RTW_PKT	147
#define EFA_RDM_RUNT_PKT_END		148
#define EFA_RDM_EXTRA_REQ_PKT_END   	148

#if defined(static_assert)
#define EFA_RDM_ENSURE_HEADER_SIZE(hdr, size)	\
	static_assert(sizeof (struct hdr) == (size), #hdr " size check")
#else
#define EFA_RDM_ENSURE_HEADER_SIZE(hdr, size)
#endif

/*
 * Fields common to all packets.
 */
#define EFA_RDM_BASE_HEADER()			\
	struct {				\
		uint8_t		type;		\
		uint8_t		version;	\
		uint16_t	flags;		\
	}

struct efa_rdm_base_hdr {
	EFA_RDM_BASE_HEADER();
};

EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_base_hdr, 4);

/* Universal flags that can be applied on "efa_rdm_base_hdr.flags".
 *
 * Universal flags start from the last bit and goes backwards.
 * Because "efa_rdm_base_hdr.flags" is a 16-bits integer, the
 * last bit is the 15th bit.
 * Other than universal flags, each packet type defines its
 * own set of flags, which generally starts from the 0th bit
 * in "efa_rdm_base_hdr.flags".
 */

/* indicate this packet has the sender connid */
#define EFA_RDM_PKT_CONNID_HDR		BIT_ULL(15)

struct efa_rma_iov {
	uint64_t		addr;
	size_t			len;
	uint64_t		key;
};

/*
 * @brief header format of CTS packet (Packet Type ID 3)
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
struct efa_rdm_cts_hdr {
	EFA_RDM_BASE_HEADER();
	union {
		uint32_t connid; /* sender connection ID, set when EFA_RDM_PKT_CONNID_HDR is on */
		uint32_t padding; /* otherwise, a padding space to 8 bytes */
	};
	uint32_t send_id; /* ID of the send opertaion on sender side */
	uint32_t recv_id; /* ID of the receive operatin on receive side */
	uint64_t recv_length; /* number of bytes receiver is ready to receive */
};

EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_cts_hdr, 24);

/* this flag is to indicated the CTS is the response of a RTR packet */
#define EFA_RDM_CTS_READ_REQ		BIT_ULL(7)


/*
 * @brief optional connid header for DATA packet
 */
struct efa_rdm_ctsdata_opt_connid_hdr {
	uint32_t connid;
	uint32_t padding;
};

/*
 * @brief header format of CTSDATA packet header (Packet Type ID 4)
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
struct efa_rdm_ctsdata_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t recv_id; /* ID of the receive operation on receiver */
	uint64_t seg_length;
	uint64_t seg_offset;
	/* optional connid header, present when EFA_RDM_PKT_CONNID_HDR is on */
	struct efa_rdm_ctsdata_opt_connid_hdr connid_hdr[0];
};

EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_ctsdata_hdr, 24);

/*
 *  @brief READRSP packet header (Packet Type ID 5)
 *
 *  READRSP is sent from read responder to read requester, and it contains
 *  application data.
 */
struct efa_rdm_readrsp_hdr {
	EFA_RDM_BASE_HEADER();
	union {
		uint32_t connid; /* sender connection ID, set when EFA_RDM_PKT_CONNID_HDR is on */
		uint32_t padding; /* otherwise, a padding space to 8 bytes boundary */
	};
	uint32_t recv_id; /* ID of the receive operation on the read requester, from rtr packet */
	uint32_t send_id; /* ID of the send operation on the read responder, will be included in CTS packet */
	uint64_t seg_length;
};

EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_readrsp_hdr, 24);

struct efa_rdm_readrsp_pkt {
	struct efa_rdm_readrsp_hdr hdr;
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
struct efa_rdm_eor_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t send_id; /* ID of the send operation on sender */
	uint32_t recv_id; /* ID of the receive operation on receiver */
	union {
		uint32_t connid; /* sender connection ID, optional, set whne EFA_RDM_PKT_CONNID_HDR is on */
		uint32_t padding; /* otherwise, a padding space to 8 bytes boundary */
	};
};

EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_eor_hdr, 16);

/*
 * @brief format of the read NACK packet. (Packet Type ID 11)
 *
 */
struct efa_rdm_read_nack_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t send_id; /* ID of the send operation on sender */
	uint32_t recv_id; /* ID of the receive operation on receiver */
	union {
		uint32_t connid; /* sender connection ID, optional, set whne EFA_RDM_PKT_CONNID_HDR is on */
		uint32_t padding; /* otherwise, a padding space to 8 bytes boundary */
	};
};

EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_read_nack_hdr, 16);

/**
 * @brief header format of ATOMRSP packet. (Packet Type ID 8)
 * ATOMRSP packet is used in emulated fetch/compare atomic sub-protocol.
 * 
 * It is sent from responder to requester, which contains the response
 * to a fetch/compare atomic request
 */
struct efa_rdm_atomrsp_hdr {
	EFA_RDM_BASE_HEADER();
	union {
		uint32_t connid; /* sender connid. set when EFA_RDM_PKT_CONNID_HDR is on in flags */
		uint32_t padding; /* otherwise, a padding space to 8 bytes boundary */
	};
	uint32_t reserved;
	uint32_t recv_id;
	uint64_t seg_length;
};

EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_atomrsp_hdr, 24);

struct efa_rdm_atomrsp_pkt {
	struct efa_rdm_atomrsp_hdr hdr;
	char data[];
};

/**
 * @brief header format of a HANDSHAKE packet
 *
 * HANDSHAKE packet is used in the handshake sub-protocol.
 *
 * Upon receiving 1st packet from a peer, an endpoint will
 * send a HANDSHAKE packet back, which contains its capablity bits
 */
struct efa_rdm_handshake_hdr {
	EFA_RDM_BASE_HEADER();
	/* nextra_p3 is number of members in extra_info plus 3.
	 * The "p3" part was introduced for backward compatibility.
	 * See protocol v4 document section 2.1 for detail.
	 */
	uint32_t nextra_p3;
	uint64_t extra_info[0];
};

/* indicate this package has the sender host id */
#define EFA_RDM_HANDSHAKE_HOST_ID_HDR		BIT_ULL(0)
#define EFA_RDM_HANDSHAKE_DEVICE_VERSION_HDR	BIT_ULL(1)
#define EFA_RDM_HANDSHAKE_USER_RECV_QP_HDR	BIT_ULL(2)

EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_handshake_hdr, 8);

struct efa_rdm_handshake_opt_connid_hdr {
	union {
		uint32_t connid;
		uint64_t padding;
	};
};

struct efa_rdm_handshake_opt_host_id_hdr {
	uint64_t host_id;
};

struct efa_rdm_handshake_opt_device_version_hdr {
	union {
		uint32_t device_version;
		uint64_t reserved;
	};
};

struct efa_rdm_handshake_opt_user_recv_qp_hdr {
	uint32_t qpn;
	uint32_t qkey;
};

EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_handshake_opt_connid_hdr, 8);
EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_handshake_opt_host_id_hdr, 8);
EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_handshake_opt_device_version_hdr, 8);
EFA_RDM_ENSURE_HEADER_SIZE(efa_rdm_handshake_opt_user_recv_qp_hdr, 8);

/* @brief header format of RECEIPT packet */
struct efa_rdm_receipt_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t tx_id;
	uint32_t msg_id;
	union {
		uint32_t connid; /* sender connection ID, set when EFA_RDM_PKT_CONNID_HDR is on */
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
#define EFA_RDM_REQ_OPT_RAW_ADDR_HDR	BIT_ULL(0)
#define EFA_RDM_REQ_OPT_CQ_DATA_HDR	BIT_ULL(1)
#define EFA_RDM_REQ_MSG			BIT_ULL(2)
#define EFA_RDM_REQ_TAGGED		BIT_ULL(3)
#define EFA_RDM_REQ_RMA			BIT_ULL(4)
#define EFA_RDM_REQ_ATOMIC		BIT_ULL(5)

/* Same as EFA_RDM_OPE_READ_NACK in efa_rdm_ope.h */
#define EFA_RDM_REQ_READ_NACK	BIT_ULL(13)

/*
 * optional headers for REQ packets
 */
struct efa_rdm_req_opt_raw_addr_hdr {
	uint32_t addr_len;
	char raw_addr[0];
};

struct efa_rdm_req_opt_cq_data_hdr {
	uint64_t cq_data;
};

struct efa_rdm_req_opt_connid_hdr {
	uint32_t connid; /* sender's connection ID */
};

#define EFA_RDM_REQ_OPT_HDR_ALIGNMENT 8
#define EFA_RDM_REQ_OPT_RAW_ADDR_HDR_SIZE (((sizeof(struct efa_rdm_req_opt_raw_addr_hdr) + EFA_EP_ADDR_LEN - 1)/EFA_RDM_REQ_OPT_HDR_ALIGNMENT + 1) * EFA_RDM_REQ_OPT_HDR_ALIGNMENT)

/*
 * Base header for all RTM packets
 */
struct efa_rdm_rtm_base_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t msg_id;
};

/**
 * @brief header format of EAGER_MSGRTM packet (Packet Type ID 64)
 */
struct efa_rdm_eager_msgrtm_hdr {
	struct efa_rdm_rtm_base_hdr hdr;
};


/**
 * @brief header format of EAGER_TAGRTM packet (Packet Type ID 65)
 */
struct efa_rdm_eager_tagrtm_hdr {
	struct efa_rdm_rtm_base_hdr hdr;
	uint64_t tag;
};

struct efa_rdm_medium_rtm_base_hdr {
	struct efa_rdm_rtm_base_hdr hdr;
	uint64_t msg_length;
	uint64_t seg_offset;
};

/**
 * @brief header format of MEDIUM_MSGRTM packet (Packet Type ID 66)
 */
struct efa_rdm_medium_msgrtm_hdr {
	struct efa_rdm_medium_rtm_base_hdr hdr;
};

/**
 * @brief header format of MEDIUM_TAGRTM packet (Packet Type ID 67)
 */
struct efa_rdm_medium_tagrtm_hdr {
	struct efa_rdm_medium_rtm_base_hdr hdr;
	uint64_t tag;
};

struct efa_rdm_longcts_rtm_base_hdr {
	struct efa_rdm_rtm_base_hdr hdr;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t credit_request;
};

/**
 * @brief header format of LONGCTS_MSGRTM packet (Packet Type ID 68)
 */
struct efa_rdm_longcts_msgrtm_hdr {
	struct efa_rdm_longcts_rtm_base_hdr hdr;
};

/**
 * @brief header format of LONGCTS_TAGRTM packet (Packet Type ID 69)
 */
struct efa_rdm_longcts_tagrtm_hdr {
	struct efa_rdm_longcts_rtm_base_hdr hdr;
	uint64_t tag;
};

struct efa_rdm_rtw_base_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t rma_iov_count;
};

/**
 * @brief header format of EAGER_RTW packet (Packet Type ID 70)
 */
struct efa_rdm_eager_rtw_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t rma_iov_count;
	struct efa_rma_iov rma_iov[0];
};

/**
 * @brief header format of LONGCTS_RTW packet (Packet Type ID 71)
 */
struct efa_rdm_longcts_rtw_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t credit_request;
	struct efa_rma_iov rma_iov[0];
};

/*
 * efa_rdm_rtr_hdr is used by both SHORT_RTR (Packet Type ID 72)
 * and LONGCTS_RTR (Packet Type ID 73)
 */
struct efa_rdm_rtr_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t recv_id; /* ID of the receive operation of the read requester, will be included in DATA/READRSP header */
	uint32_t recv_length; /* number of bytes that the read requester is ready to receive */
	struct efa_rma_iov rma_iov[0];
};

/* @brief efa_rdm_rta_hdr are shared by 4 types of RTA:
 *    WRITE_RTA (Packet Type ID 74),
 *    FETCH_RTA (Packet Type ID 75),
 *    COMPARE_RTA (Packet Type ID 76) and
 *    DC_WRTIE_RTA (Packe Type ID 141)
 */
struct efa_rdm_rta_hdr {
	EFA_RDM_BASE_HEADER();
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
struct efa_rdm_longread_rtm_base_hdr {
	struct efa_rdm_rtm_base_hdr hdr;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t read_iov_count;
};

/**
 * @brief header format of LONGREAD_MSGRTM (Packet Type ID 128)
 */
struct efa_rdm_longread_msgrtm_hdr {
	struct efa_rdm_longread_rtm_base_hdr hdr;
};

/**
 * @brief header format of LONGREAD_MSGRTM (Packet Type ID 129)
 */
struct efa_rdm_longread_tagrtm_hdr {
	struct efa_rdm_longread_rtm_base_hdr hdr;
	uint64_t tag;
};

/**
 * @brief header format of LONGREAD_MSGRTM (Packet Type ID 130)
 */
struct efa_rdm_longread_rtw_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t read_iov_count;
	struct efa_rma_iov rma_iov[0];
};

/*
 * Extra requester: delivery complete (section 4.2)
 */

struct efa_rdm_dc_eager_rtm_base_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t msg_id;
	uint32_t send_id;
	uint32_t padding;
};

/**
 * @brief header format of a DC_EAGER_MSGRTM packet
 */
struct efa_rdm_dc_eager_msgrtm_hdr {
	struct efa_rdm_dc_eager_rtm_base_hdr hdr;
};

/**
 * @brief header format of a DC_EAGER_TAGRTM packet
 */
struct efa_rdm_dc_eager_tagrtm_hdr {
	struct efa_rdm_dc_eager_rtm_base_hdr hdr;
	uint64_t tag;
};

struct efa_rdm_dc_medium_rtm_base_hdr {
	struct efa_rdm_rtm_base_hdr hdr;
	uint32_t send_id;
	uint32_t padding;
	uint64_t msg_length;
	uint64_t seg_offset;
};

/**
 * @brief header format of a DC_MEDIUM_MSGRTM packet
 */
struct efa_rdm_dc_medium_msgrtm_hdr {
	struct efa_rdm_dc_medium_rtm_base_hdr hdr;
};

/**
 * @brief header format of a DC_MEDIUM_TAGRTM packet
 */
struct efa_rdm_dc_medium_tagrtm_hdr {
	struct efa_rdm_dc_medium_rtm_base_hdr hdr;
	uint64_t tag;
};

/**
 * @brief header format of a DC_LONGCTS_MSGRTM packet
 */
struct efa_rdm_dc_longcts_msgrtm_hdr {
	struct efa_rdm_longcts_rtm_base_hdr hdr;
};

/**
 * @brief header format of a DC_LONGCTS_TAGRTM packet
 */
struct efa_rdm_dc_longcts_tagrtm_hdr {
	struct efa_rdm_longcts_rtm_base_hdr hdr;
	uint64_t tag;
};

/**
 * @brief header format of a DC_EAGER_RTW packet
 */
struct efa_rdm_dc_eager_rtw_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t rma_iov_count;
	/* end of efa_rdm_rtw_base_hdr */
	uint32_t send_id;
	uint32_t padding;
	struct efa_rma_iov rma_iov[0];
};

/**
 * @brief header format of a DC_LONGCTS_RTW packet
 */
struct efa_rdm_dc_longcts_rtw_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t credit_request;
	struct efa_rma_iov rma_iov[0];
};

/* DC_WRITE_RTA header format is merged into efa_rdm_rta_hdr */

/* Extra feature runting protocols related headers */

struct efa_rdm_runtcts_rtm_base_hdr {
	struct efa_rdm_rtm_base_hdr hdr;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t credit_request;
	uint64_t seg_offset;
	uint64_t runt_length;
};

/**
 * @brief header format of RUNTCTS_MSGRTM (Packet Type ID 142)
 */
struct efa_rdm_runtcts_msgrtm_hdr {
	struct efa_rdm_runtcts_rtm_base_hdr hdr;
};

/**
 * @brief header format of RUNTCTS_TAGRTM (Packet Type ID 143)
 */
struct efa_rdm_runtcts_tagrtm_hdr {
	struct efa_rdm_runtcts_rtm_base_hdr hdr;
	uint64_t tag;
};

/**
 * @brief header format of RUNTCTS_RTW (Packet Type ID 144)
 */
struct efa_rdm_runtcts_rtw_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t credit_request;
	uint64_t seg_offset;
	uint64_t runt_length;
	struct efa_rma_iov rma_iov[0];
};

struct efa_rdm_runtread_rtm_base_hdr {
	struct efa_rdm_rtm_base_hdr hdr;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t read_iov_count;
	uint64_t seg_offset;
	uint64_t runt_length;
};

/**
 * @brief header format of RUNTREAD_MSGRTM (Packet Type ID 145)
 */
struct efa_rdm_runtread_msgrtm_hdr {
	struct efa_rdm_runtread_rtm_base_hdr hdr;
};

/**
 * @brief header format of RUNTREAD_MSGRTM (Packet Type ID 146)
 */
struct efa_rdm_runtread_tagrtm_hdr {
	struct efa_rdm_runtread_rtm_base_hdr hdr;
	uint64_t tag;
};

/**
 * @brief header format of RUNTREAD_RTW (Packet Type ID 147)
 */
struct efa_rdm_runtread_rtw_hdr {
	EFA_RDM_BASE_HEADER();
	uint32_t rma_iov_count;
	uint64_t msg_length;
	uint32_t send_id;
	uint32_t read_iov_count;
	uint64_t seg_offset;
	uint64_t runt_length;
	struct efa_rma_iov rma_iov[0];
};

#endif

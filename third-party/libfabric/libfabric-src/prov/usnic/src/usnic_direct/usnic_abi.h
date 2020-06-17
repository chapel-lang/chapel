/*
 * Copyright (c) 2013, Cisco Systems, Inc. All rights reserved.
 *
 * LICENSE_BEGIN
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * LICENSE_END
 *
 *
 */


#ifndef USNIC_ABI_H
#define USNIC_ABI_H

/* ABI between userspace and kernel */
#define USNIC_UVERBS_ABI_VERSION	4

#define USNIC_QP_GRP_MAX_WQS		8
#define USNIC_QP_GRP_MAX_RQS		8
#define USNIC_QP_GRP_MAX_CQS		16

#define USNIC_DECODE_PGOFF_VFID(pgoff)	((pgoff) & ((1ULL << 32) - 1))
#define USNIC_DECODE_PGOFF_TYPE(pgoff)	((pgoff) >> 48)
#define USNIC_DECODE_PGOFF_RES(pgoff)	(((pgoff) >> 32) & ((1ULL << 16) - 1))
#define USNIC_DECODE_PGOFF_BAR(pgoff)	(((pgoff) >> 32) & ((1ULL << 16) - 1))

#define USNIC_ENCODE_PGOFF(vfid, map_type, res_type_bar_id) \
	(((((uint64_t)map_type & 0xffff) << 48) | \
	  (((uint64_t)res_type_bar_id & 0xffff) << 32) | \
	  ((uint64_t)vfid & ((1ULL << 32) - 1))) * sysconf(_SC_PAGE_SIZE))

/*
 * The kernel module eventually issues proxy the devcmd through enic and the
 * maximum number of devcmd arguments supported for a vnic is VNIC_DEVCMD_NARGS
 * (= 15).  Among them, 2 arguments are consumed by proxy command for
 * proxy_index and proxy devcmd. Hence, only a maximum of 13 arguments are
 * supported on input in practice, even though the ABI between user space and
 * the kernel has space for 15.
 *
 * Keep _NARGS as 15 for backwards compatibility (newer user space with older
 * kernel), otherwise usnic_ucmd_devcmd() on the older kernel will fail with
 * -EINVAL.
 */
#define USNIC_UDEVCMD_NARGS 		15
#define USNIC_UDEVCMD_MAX_IN_ARGS	(USNIC_UDEVCMD_NARGS - 2)

enum usnic_mmap_type {
	USNIC_MMAP_BAR			= 0,
	USNIC_MMAP_RES			= 1,
	USNIC_MMAP_BARHEAD		= 2,
	USNIC_MMAP_GRPVECT		= 3,
};

enum usnic_transport_type {
	USNIC_TRANSPORT_UNKNOWN		= 0,
	USNIC_TRANSPORT_ROCE_CUSTOM	= 1,
	USNIC_TRANSPORT_IPV4_UDP	= 2,
	USNIC_TRANSPORT_IPV4_TCP_3T	= 3,
	USNIC_TRANSPORT_ROCEV2	  	= 4,
	USNIC_TRANSPORT_MAX		= 5,
};

#define ROCEV2_PORT 4791

enum usnic_ucmd_type {
	USNIC_USER_CMD_DEVCMD,
	USNIC_USER_CMD_MAX,
};

struct usnic_user_cmd {
	u32			ucmd;
	u32			pad_to_8byte;
	u64			inbuf;
	u64			outbuf;
	u32			inlen;
	u32			outlen;
};

struct usnic_udevcmd_cmd {
	u32			vnic_idx;
	u32			devcmd;
	u32			wait;
	u32			num_args;
	u64			args[USNIC_UDEVCMD_NARGS];
};

struct usnic_udevcmd_resp {
	u32			num_args;
	u64			args[USNIC_UDEVCMD_NARGS];
};

/*
 * This is the version of the transport_spec structure that is used
 * in CREATE_QP versions 0..2
 */
struct usnic_transport_spec_v2 {
	enum usnic_transport_type	trans_type;
	union {
		struct {
			uint16_t	port_num;
		} usnic_roce;
		struct {
			uint32_t	sock_fd;
		} ip;
	};
};

/*
 * This is the version of the transport_spec structure that is used
 * in CREATE_QP versions 3..
 */
struct usnic_transport_spec {
	enum usnic_transport_type	trans_type;
	union {
		struct {
			uint16_t	port_num;
		} usnic_roce;
		struct {
			uint32_t	sock_fd;
		} ip;
		struct {
			uint32_t	qpn;
			uint32_t	ipaddr_be;
		} rocev2;
		u_int8_t pad[256];
	};
};

#define USNIC_IB_ALLOC_PD_VERSION 1

struct usnic_ib_alloc_pd_cmd {
	u32	resp_version;		/* response version requested */
	u32	pad_to_8byte;
};

struct usnic_ib_alloc_pd_resp {
	u32	resp_version;
	u32	pad_to_8byte;
	union {
		struct {
			u32	vfid;
			u32	grp_vect_buf_len;
		} cur;  /* v1 */
	};
};

#define USNIC_IB_CREATE_QP_VERSION 3

struct usnic_ib_create_qp_cmd_v0 {
	struct usnic_transport_spec_v2	spec_v2;
};

struct usnic_ib_create_qp_cmd_v2 {
	struct usnic_transport_spec_v2 spec_v2;
	u32				cmd_version;
	union {
		struct {
			/* length in bytes of resources array */
			u32		resources_len;

			/* ptr to array of struct usnic_vnic_barres_info */
			u64		resources;
		} cur; /* v1 and v2 cmd */
	} u;
};

struct usnic_ib_create_qp_cmd {
	/*
	 * This is the old transport spec struct that must stay as the
	 * first member of this struct for backwards compatibility/ABI
	 * reasons..  It is "v2" because it is used with CREATE_QP
	 * versions 0, 1, and 2.
	 */
	struct usnic_transport_spec_v2 spec_v2;
	u32				cmd_version;
	union {
		struct {
			/* length in bytes of resources array */
			u32		resources_len;

			/* ptr to array of struct usnic_vnic_barres_info */
			u64		resources;
		} cur; /* v1 and v2 cmd */
	} u;
	/*
	 * This is the current version of the transport spec struct.
	 */
	struct usnic_transport_spec	spec;
};


/*
 * infomation of vnic bar resource
 */
struct usnic_vnic_barres_info {
	int32_t	 	type;
	uint32_t	padding;
	uint64_t	bus_addr;
	uint64_t	len;
};

/*
 * All create_qp responses must start with this for backwards compatability
 */
#define USNIC_IB_CREATE_QP_RESP_V0_FIELDS                               \
	u32				vfid;                           \
	u32				qp_grp_id;                      \
	u64				bar_bus_addr;                   \
	u32				bar_len;                        \
	u32				wq_cnt;                         \
	u32				rq_cnt;                         \
	u32				cq_cnt;                         \
	u32				wq_idx[USNIC_QP_GRP_MAX_WQS];   \
	u32				rq_idx[USNIC_QP_GRP_MAX_RQS];   \
	u32				cq_idx[USNIC_QP_GRP_MAX_CQS];   \
	u32				transport;

struct usnic_ib_create_qp_resp_v0 {
	USNIC_IB_CREATE_QP_RESP_V0_FIELDS
	u32				reserved[9];
};

struct usnic_ib_create_qp_resp {
	USNIC_IB_CREATE_QP_RESP_V0_FIELDS
	/* the above fields end on 4-byte alignment boundary */
	u32 cmd_version;
	union {
		struct {
			u32 num_barres;
			u32 pad_to_8byte;
		} v1;
		struct {
			u32 num_barres;
			u32 wq_err_intr_offset;
			u32 rq_err_intr_offset;
			u32 wcq_intr_offset;
			u32 rcq_intr_offset;
			u32 barhead_len;
		} cur; /* v2 */
	} u;

	/* v0 had a "reserved[9]" field, must not shrink the response or we can
	 * corrupt newer clients running on older kernels */
	u32				reserved[2];
};

#define USNIC_CTX_RESP_VERSION 2

/*
 * Make this structure packed in order to make sure v1.num_caps not aligned
 * at 8 byte boundary, hence still being able to support user libary
 * requesting version 1 response.
 */
struct __attribute__((__packed__)) usnic_ib_get_context_cmd {
	u32 resp_version;	/* response version requested */
	union {
		struct {
			u32 num_caps;	/* number of capabilities requested */
		} v1;
		struct {
			u32 encap_subcmd;	/* whether encapsulate subcmd */
			union {
				u32 num_caps;
				struct usnic_user_cmd usnic_ucmd;
			};
		} v2;
	};
};

/*
 * Note that this enum must never have members removed or re-ordered in order
 * to retain backwards compatability
 */
enum usnic_capability {
	USNIC_CAP_CQ_SHARING,	/* CQ sharing version */
	USNIC_CAP_MAP_PER_RES,	/* Map individual RES */
	USNIC_CAP_PIO,		/* PIO send */
	USNIC_CAP_CQ_INTR,  /* CQ interrupts (via comp channels) */
	USNIC_CAP_GRP_INTR, /* Group interrupt */
	USNIC_CAP_DPKT,  	/* Direct Packet Interface */
	USNIC_CAP_CNT
};

/*
 * If and when there become multiple versions of this struct, it will
 * become a union for cross-version compatability to make sure there is always
 * space for older and larger versions of the contents.
 */
struct usnic_ib_get_context_resp {
	u32 resp_version;	/* response version returned */
	u32 num_caps;		/* number of capabilities returned */
	u32 cap_info[USNIC_CAP_CNT];
};

#define USNIC_IB_CREATE_CQ_VERSION 2

struct usnic_ib_create_cq_v0 {
        u64 reserved;
};

#define USNIC_CQ_COMP_SIGNAL_VERBS	0x1	/* whether to signal cq
						 * completion event via verbs
						 */

struct usnic_ib_create_cq {
	u32 resp_version;	/* response version requested */
	union {
		struct {
			u32 intr_arm_mode;
		} v1;
		struct {
			u32 flags;
			__s64 comp_event_fd;	/* wait fd for cq completion */
			u64 affinity_mask_ptr;	/* process affinity mask ptr*/
			u64 affinity_mask_len;
		} cur;	/* v2 */
	};
};

struct usnic_ib_create_cq_resp_v0 {
	u64 reserved;
};

struct usnic_ib_create_cq_resp {
	u32 resp_version;	/* response version returned */
	u32 pad_to_8byte;
};

#endif /* USNIC_ABI_H */

/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2020-2022 Hewlett Packard Enterprise Development LP
 */

#ifndef _FI_CXI_EXT_H_
#define _FI_CXI_EXT_H_

/* CXI provider specific NIC attributes. This information is returned in
 * fi_info::nid::prov_attr.

 * Users can optionally modify some fields. Depending on the field adjusted,
 * this can impact fi_domain() or other endpoint allocation behavior.
 */

#define FI_CXI_NIC_ATTR_VER 1U

struct cxip_nic_attr {
	/* Version of NIC attr. Must remain at the top of this struct. */
	uint32_t version;

	/* NIC address. Should never be modified. */
	const unsigned int addr;

	/* On output from fi_getinfo(), rgroup_id will be set in the following
	 * order:
	 * 1. Resource group ID returned from SLINGSHOT_SVC_ID environment
	 * variable
	 * 2. First resource group ID with matching UID
	 * 3. First resource group ID with matching GID
	 * 4. First resource group ID with open permissions
	 */
	const unsigned int default_rgroup_id;

	/* Default VNI used with the rgroup ID. */
	const unsigned int default_vni;
};

/*
 * TODO: The following should be integrated into the include/rdma/fi_ext.h
 * and are use for provider specific fi_control() operations.
 */
#define FI_PROV_SPECIFIC_CXI	(0xccc << 16)

enum {
	FI_OPT_CXI_SET_TCLASS = -FI_PROV_SPECIFIC_CXI,	/* uint32_t */
	FI_OPT_CXI_SET_MSG_ORDER,			/* uint64_t */

	/* fid_nic control operation to refresh NIC attributes. */
	FI_OPT_CXI_NIC_REFRESH_ATTR,

	FI_OPT_CXI_SET_MR_MATCH_EVENTS,			/* bool */
	FI_OPT_CXI_GET_MR_MATCH_EVENTS,			/* bool */
	FI_OPT_CXI_SET_OPTIMIZED_MRS,			/* bool */
	FI_OPT_CXI_GET_OPTIMIZED_MRS,			/* bool */
	FI_OPT_CXI_SET_PROV_KEY_CACHE,			/* bool */
	FI_OPT_CXI_GET_PROV_KEY_CACHE,			/* bool */
	FI_OPT_CXI_SET_RNR_MAX_RETRY_TIME,		/* uint64_t */
};

/*
 * Defines for compatibility between main branch features and release
 * branches back porting of features required for use. Any value
 * included here should map exactly to the value established in the
 * main branch (enum or define) and this CXI equivalent will exist forever.
 */
#define FI_CXI_CNTR_EVENTS_BYTES	1	/* FI_CNTR_EVENTS_BYTES */

/*
 * TODO: Set this to the upstream value prior to releasing software.
 * This flag returned in a completion and indicates that the message was
 * truncated and that the length indicates the truncated message length.
 */
#define FI_CXI_TRUNC		(1ULL << 56)

/*
 * Execute a given libfabric atomic memory operation as a PCIe operation as
 * compared to a NIC operation.
 *
 * Note: Ordering between PCIe atomic operations and NIC atomic/RMA operations
 * is undefined.
 *
 * Note: This flag overloads the bit used for FI_SOURCE. But, since FI_SOURCE
 * is invalid for AMO operations, overloading this bit is not an issue.
 */
#define FI_CXI_PCIE_AMO (1ULL << 57)

/*
 * Flag an accelerated collective as pre-reduced.
 *
 * This can be passed to the accelerated collectives operations to indicate
 * that the supplied data is a pre-reduced cxip_coll_accumulator structure.
 *
 * Note: This flag overloads FI_CXI_PCIE_AMO. Accelerated collectives do not
 * use FI_CXI_PCIE_AMO or FI_SOURCE.
 */
#define	FI_CXI_PRE_REDUCED (1ULL << 57)

/*
 * Use CXI High Rate Puts (HRP). Increases message rate performance. Applies to
 * RMA and unreliable, non-fetching AMO operations.
 */
#define FI_CXI_HRP (1ULL << 60)

/*
 * Disable AMO reliability. Increases message rate performance. Applies to
 * non-fetching AMOs. Required for HRP AMOs.
 */
#define FI_CXI_UNRELIABLE (1ULL << 61)

/*
 * Request a provider specific weak FENCE operation to facilitate an
 * EP alias ordering point, when the original EP utilizes PCIe RO=1.
 */
#define FI_CXI_WEAK_FENCE (1ULL << 63)

/*
 * Used in conjunction with the deferred work queue API. If a deferred work
 * queue operation has this flag set, the CXI provider will ensure a counter
 * writeback occurs once the deferred work queue operation completes.
 * Note: Addition hardware resources will be used to ensure a counter writeback
 * occurs at the completion of the deferred work queue operation.
 */
#define FI_CXI_CNTR_WB (1ULL << 62)
#define FI_CXI_COUNTER_OPS "cxi_counter_ops"

struct fi_cxi_cntr_ops {
	/* Set the counter writeback address to a client provided address. */
	int (*set_wb_buffer)(struct fid *fid, void *buf, size_t len);

	/* Get the counter MMIO region. */
	int (*get_mmio_addr)(struct fid *fid, void **addr, size_t *len);
};

/* Success values cannot exceed FI_CXI_CNTR_SUCCESS_MAX */
#define FI_CXI_CNTR_SUCCESS_MAX ((1ULL << 48) - 1)

/* Failure values cannot exceed FI_CXI_CNTR_FAILURE_MAX */
#define FI_CXI_CNTR_FAILURE_MAX ((1ULL << 7) - 1)

/* fi_cntr_read() equivalent but for the writeback buffer. */
static inline uint64_t fi_cxi_cntr_wb_read(const void *wb_buf)
{
	return (*(uint64_t *)wb_buf) & FI_CXI_CNTR_SUCCESS_MAX;
};

/* fi_cntr_reader() equivalent but for the writeback buffer. */
static inline uint64_t fi_cxi_cntr_wb_readerr(const void *wb_buf)
{
	return ((*(uint64_t *)wb_buf) >> 48) & FI_CXI_CNTR_FAILURE_MAX;
};

/* Generate a counter success value which can be polled on. */
static inline int fi_cxi_gen_cntr_success(uint64_t value, uint64_t *cxi_value)
{
	if (value > FI_CXI_CNTR_SUCCESS_MAX)
		return -FI_EINVAL;

	*cxi_value = (1ULL << 63) | value;
	return FI_SUCCESS;
};

/* fi_cntr_add() equivalent but for the MMIO region. */
static inline int fi_cxi_cntr_add(void *cntr_mmio, uint64_t value)
{
	/* Success counter is only 48 bits wide. */
	if (value > FI_CXI_CNTR_SUCCESS_MAX)
		return -FI_EINVAL;

	*((uint64_t *)cntr_mmio) = value;
	return FI_SUCCESS;
}

/* fi_cntr_adderr() equivalent but for the MMIO region. */
static inline int fi_cxi_cntr_adderr(void *cntr_mmio, uint64_t value)
{
	/* Error counter is only 7 bits wide. */
	if (value > FI_CXI_CNTR_FAILURE_MAX)
		return -FI_EINVAL;

	*((uint64_t *)cntr_mmio + 8) = value;
	return FI_SUCCESS;
}

/* fi_cntr_set() equivalent but for the MMIO region. */
static inline int fi_cxi_cntr_set(void *cntr_mmio, uint64_t value)
{
	/* Only set of zero is supported through MMIO region. */
	if (value > 0)
		return -FI_EINVAL;

	*((uint64_t *)cntr_mmio + 16) = 0;
	return FI_SUCCESS;
}

/* fi_cntr_seterr() equivalent but for MMIO region. */
static inline int fi_cxi_cntr_seterr(void *cntr_mmio, uint64_t value)
{
	/* Only set of zero is supported through MMIO region. */
	if (value > 0)
		return -FI_EINVAL;

	*((uint64_t *)cntr_mmio + 24) = 0;
	return FI_SUCCESS;
}

/* fi_cntr_add() equivalent but for the MMIO region. */
static inline void *fi_cxi_get_cntr_add_addr(void *cntr_mmio)
{
	return cntr_mmio;
}

/* fi_cntr_adderr() equivalent but for the MMIO region. */
static inline void *fi_cxi_get_cntr_adderr_addr(void *cntr_mmio)
{
	return (void *)((uint64_t *)cntr_mmio + 8);
}

/* fi_cntr_set() equivalent but for the MMIO region reset.
 * NOTE: CXI does not support set to counter MMIO region. Only reset.
 */
static inline void *fi_cxi_get_cntr_reset_addr(void *cntr_mmio)
{
	return (void *)((uint64_t *)cntr_mmio + 16);
}

/* fi_cntr_seterr() equivalent but for MMIO region reset.
 * NOTE: CXI does not support set to counter MMIO region. Only reset.
 */
static inline void *fi_cxi_get_cntr_reseterr_addr(void *cntr_mmio)
{
	return (void *)((uint64_t *)cntr_mmio + 24);
}

#define FI_CXI_DOM_OPS_1 "dom_ops_v1"
#define FI_CXI_DOM_OPS_2 "dom_ops_v2"
#define FI_CXI_DOM_OPS_3 "dom_ops_v3"
#define FI_CXI_DOM_OPS_4 "dom_ops_v4"
#define FI_CXI_DOM_OPS_5 "dom_ops_v5"
#define FI_CXI_DOM_OPS_6 "dom_ops_v6"

/* v1 to v6 can use the same struct since they only appended a routine */
struct fi_cxi_dom_ops {
	int (*cntr_read)(struct fid *fid, unsigned int cntr, uint64_t *value,
		      struct timespec *ts);
	int (*topology)(struct fid *fid, unsigned int *group_id,
			unsigned int *switch_id, unsigned int *port_id);

	/* Enable hybrid MR desc mode. Hybrid MR desc allows for libfabric users
	 * to optionally pass in a valid MR desc for local communication
	 * operations.
	 *
	 * When enabled, if the MR desc is NULL, the provider will
	 * perform internal memory registration. Else, the provider will assume
	 * the MR desc field is valid and skip internal memory registration.
	 *
	 * When disabled, the provider will ignore the MR desc field and always
	 * perform internal memory registration. This is the default behavior.
	 *
	 * All child endpoints will inherit the current domain status of hybrid
	 * MR desc only during endpoint creation. Dynamically changing the
	 * domain hybrid MR desc status with endpoint allocate may not propagate
	 * to child endpoints. Thus, it is recommended to set hybrid MR desc
	 * status prior to allocating endpoints.
	 */
	int (*enable_hybrid_mr_desc)(struct fid *fid, bool enable);

	/* Get unexpected message information.
	 *
	 * Obtain a list of unexpected messages associated with the endpoint.
	 * The list is returned as an array of CQ tagged entries. The following
	 * is how the fields in fi_cq_tagged_entry are used.
	 *
	 * op_context: NULL since this message has not matched a posted receive
	 *	flags: A combination of FI_MSG, FI_TAGGED, FI_RECV,
	 *	and/or FI_REMOTE_CQ_DATA
	 *	len: Unexpected message request length
	 *	data: Completion queue data (only valid if FI_REMOTE_CQ_DATA
	 *	is set)
	 *	tag: Unexpected message tag (only valid if FI_TAGGED is set)
	 *
	 * @ep: Endpoint FID to have unexpected messages returned to user.
	 * @entry: Tagged entry array to be filled in by the provider. If the
	 * entry is NULL, only ux_count will be set.
	 * @count: Number of entries in entry and src_addr array. If count is
	 * zero,then only the ux_count will be set on return.
	 * @src_addr: Source address array to be filled in by the provider. If
	 * the entry is NULL, only ux_count will be set.
	 * @ux_count: Output variable used to return the number of unexpected
	 * messages queued on the given endpoint.
	 *
	 * Return: On success, number of entries copied into the users entry
	 * and src_addr arrays. On error, -FI_ERRNO.
	 */
	size_t (*ep_get_unexp_msgs)(struct fid_ep *fid_ep,
				    struct fi_cq_tagged_entry *entry,
				    size_t count, fi_addr_t *src_addr,
				    size_t *ux_count);

	/* Get the depth of the deferred work queue. The depth is the number of
	 * of triggered operation commands which can be queued to hardware. The
	 * depth is not per fi_domain. The depth is across all processes using
	 * the same CXI service which usually maps to a job-step.
	 */
	int (*get_dwq_depth)(struct fid *fid, size_t *depth);

	/* The following two functions have been deprecated in favor of
	 * using the fi_control() standardized interface. They will be
	 * removed in a future software release, but are left here initially
	 * to allow early users to adjust their usage.
	 */
	int (*enable_mr_match_events)(struct fid *fid, bool enable);
	int (*enable_optimized_mrs)(struct fid *fid, bool enable);
};

/*
 * CXI Authorization Key
 */
struct cxi_auth_key {
	/* The CXI service assigned to the Domain and Endpoints. A CXI service
	 * is associated with a set of local resource limits, VNIs, and Traffic
	 * Classes.
	 *
	 * The svc_id used by an OFI Domain must match all Endpoints belonging
	 * to the Domain.
	 */
	uint32_t svc_id;

	/* The Virtual Network ID (VNI) assigned to the Endpoint. Two Endpoints
	 * must use the same VNI in order to communicate.
	 *
	 * Note that while the CXI service may define one or more VNIs which a
	 * process can access, an Endpoint is assigned to only one.
	 */
	uint16_t vni;
};

/*
 * CXI Collectives
 */

/*
 * AV Set communication key.
 *
 * For production:
 * - Set cxip_comm_key.keytype = COMM_KEY_NONE.
 * - Initialize cxip_comm_key structure to zeros.
 * - Create one av_set on each node.
 * - Initialize each av_set to contain the NIC addresses of all endpoints.
 * - Call fi_join_collective() once on each endpoint.
 * - dest_addr is a multicast address created by the join.
 * - hwroot_nic is assigned by the join.
 * - The PTE will receive at the multicast ID value, index extension of zero.
 * - Sending to the multicast ID will cause delivery to nodes according to the
 *   tree topology.
 *
 * For testing with externally established multicast address:
 * - NOT IMPLEMENTED.
 *
 * For testing on a multinode system without multicast:
 * - Set cxip_comm_key.keytype = COMM_KEY_UNICAST.
 * - Set cxip_comm_key.ucast.hwroot_idx to the desired hw_root index.
 * - Create one av_set on each node.
 * - Initialize each av_set to contain the NIC addresses of all endpoints.
 * - Call fi_join_collective() once one each endpoint.
 * - hwroot_nic is the NIC address of the node that serves as the emulated
 *   hardware root of the tree.
 * - The PTE will use the EP source NIC address and process PID, with a
 *   PID_IDX of CXIP_PTL_IDX_COLL.
 * - Sending to any (valid) node address with CXIP_PTL_IDX_COLL will target the
 *   collectives PTE on that node.
 * - The root/leaf send routines will distribute one or more packets to all
 *   fi_addr_t in the av_set as appropriate.
 *
 * For testing under NETSIM on a single node:
 * - Set cxip_comm_key.keytype = COMM_KEY_RANK.
 * - Set cxip_comm_key.rank.hwroot_idx to the desired hw_root index.
 * - Set cxip_comm_key.rank.rank to the simulated rank.
 * - Create N av_set objects, one for each simulated rank.
 * - Call fi_join_collective() once for each simulated endpoint.
 * - dest_addr is the MC object index.
 * - hwroot_nic is the MC object index for the MC object to serve as the
 *   simulated hardware root.
 * - The PTE will use the EP source NIC address and process PID, with a PID_IDX
 *   of 16 + dest_addr (MC object index).
 * - Sending to the node's own address with a PID_IDX of 16 + MC index will
 *   target the appropriate MC object.
 * - Simulation is limited to 32 simulated endpoints.
 */
enum cxip_comm_key_type {
	COMM_KEY_NONE = 0,
	COMM_KEY_MULTICAST,
	COMM_KEY_UNICAST,
	COMM_KEY_RANK,
	COMM_KEY_MAX
};

typedef unsigned int cxip_coll_op_t;	// CXI collective opcode

struct cxip_coll_mcast_key {
	uint32_t hwroot_idx;		// index of hwroot in av_set list
	uint32_t mcast_addr;		// 13-bit multicast address id
};

struct cxip_coll_unicast_key {
	uint32_t hwroot_idx;		// index of hwroot in av_set list
	uint32_t mcast_addr;		// 13-bit simulated multcast address
};

struct cxip_coll_rank_key {
	uint32_t hwroot_idx;		// index of hwroot in av_set list
	uint32_t rank;			// rank of this object
	bool rx_discard;		// clear to report RX events
};

struct cxip_comm_key {
	enum cxip_comm_key_type keytype;
	union {
		struct cxip_coll_mcast_key mcast;
		struct cxip_coll_unicast_key ucast;
		struct cxip_coll_rank_key rank;
	};
};

/* Extended reduction opcodes.
 *
 * Only the following standard FI_ATOMIC operations are supported:
 * - FI_MIN	: INT or FLT
 * - FI_MAX	: INT or FLT
 * - FI_SUM	: INT or FLT
 * - FI_BOR	: INT
 * - FI_BAND	: INT
 * - FI_BXOR	: INT
 *
 * The codes below extend this standard FI_ATOMIC set to explicitly take
 * advantage of extended hardware operations. These can be used as opcodes for
 * any of the collective operations, just like FI_MIN or FI_SUM.
 *
 * Note that the current FI_ATOMIC set ends at opcode == 19. We start this one
 * at 32, to accommodate possible expansion of the FI_ATOMIC set, and check for
 * overlap during initialization.
 */
enum cxip_coll_op {
	FI_CXI_MINMAXLOC = 32,	// FLT or INT
	FI_CXI_REPSUM,		// FLT only
	FI_CXI_OP_LAST
};

/* Extended accelerated reduction structures.
 */
struct cxip_coll_intminmax {
	int64_t minval;
	uint64_t minidx;
	int64_t maxval;
	uint64_t maxidx;
};

struct cxip_coll_fltminmax {
	double minval;
	uint64_t minidx;
	double maxval;
	uint64_t maxidx;
};

/* opaque export of struct cxip_coll_data */
struct cxip_coll_accumulator {
	uint8_t accum[64];
};

#endif /* _FI_CXI_EXT_H_ */

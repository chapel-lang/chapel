/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2014 Intel Corporation, Inc. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2017 DataDirect Networks, Inc. All rights reserved.
 * Copyright (c) 2018-2023 Hewlett Packard Enterprise Development LP
 */

#ifndef _CXIP_PROV_H_
#define _CXIP_PROV_H_

#include <netinet/ether.h>
#include "config.h"

#include <pthread.h>
#include <json-c/json.h>

#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_eq.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_trigger.h>
#include <semaphore.h>

#include <ofi.h>
#include <ofi_atom.h>
#include <ofi_atomic.h>
#include <ofi_mr.h>
#include <ofi_enosys.h>
#include <ofi_indexer.h>
#include <ofi_rbuf.h>
#include <ofi_lock.h>
#include <ofi_list.h>
#include <ofi_file.h>
#include <ofi_osd.h>
#include <ofi_util.h>
#include <ofi_mem.h>
#include <ofi_hmem.h>
#include <unistd.h>

#include "libcxi/libcxi.h"
#include "cxip_faults.h"
#include "fi_cxi_ext.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef FLOOR
#define FLOOR(a, b) ((long long)(a) - (((long long)(a)) % (b)))
#endif

#ifndef CEILING
#define CEILING(a, b) ((long long)(a) <= 0LL ? 0 : (FLOOR((a)-1, b) + (b)))
#endif

#define CXIP_ALIGN_MASK(x, mask) (((x) + (mask)) & ~(mask))
#define CXIP_ALIGN(x, a) CXIP_ALIGN_MASK(x, (typeof(x))(a) - 1)
#define CXIP_ALIGN_DOWN(x, a) CXIP_ALIGN((x) - ((a) - 1), (a))

#define CXIP_REQ_CLEANUP_TO		3000

#define CXIP_BUFFER_ID_MAX		(1 << 16)

/* Scalable EP not supported */
#define CXIP_EP_MAX_CTX_BITS		0
#define CXIP_EP_MAX_TX_CNT		(1 << CXIP_EP_MAX_CTX_BITS)
#define CXIP_EP_MAX_RX_CNT		(1 << CXIP_EP_MAX_CTX_BITS)
#define CXIP_EP_MAX_MSG_SZ		((1ULL << 32) - 1)
#define CXIP_EP_MIN_MULTI_RECV		64
#define CXIP_EP_MAX_MULTI_RECV		((1 << 24) - 1)

#define CXIP_TX_COMP_MODES		(FI_INJECT_COMPLETE | \
					 FI_TRANSMIT_COMPLETE | \
					 FI_DELIVERY_COMPLETE | \
					 FI_MATCH_COMPLETE)
#define CXIP_TX_OP_FLAGS		(FI_INJECT | \
					 FI_COMPLETION | \
					 CXIP_TX_COMP_MODES | \
					 FI_REMOTE_CQ_DATA | \
					 FI_MORE | \
					 FI_FENCE)
#define CXIP_RX_OP_FLAGS		(FI_COMPLETION | \
					 FI_MULTI_RECV | \
					 FI_MORE)
/* Invalid OP flags for RX that can be silently ignored */
#define CXIP_RX_IGNORE_OP_FLAGS		(FI_REMOTE_CQ_DATA | \
					 FI_INJECT)
#define CXIP_WRITEMSG_ALLOWED_FLAGS	(FI_INJECT | \
					 FI_COMPLETION | \
					 FI_MORE | \
					 FI_FENCE | \
					 CXIP_TX_COMP_MODES)
#define CXIP_READMSG_ALLOWED_FLAGS	(FI_COMPLETION | \
					 FI_MORE | \
					 FI_FENCE | \
					 CXIP_TX_COMP_MODES)

#define CXIP_AMO_MAX_IOV		1
#define CXIP_EQ_DEF_SZ			(1 << 8)
#define CXIP_CQ_DEF_SZ			1024U
#define CXIP_REMOTE_CQ_DATA_SZ		8

#define CXIP_PTE_IGNORE_DROPS		((1 << 24) - 1)
#define CXIP_RDZV_THRESHOLD		2048
#define CXIP_OFLOW_BUF_SIZE		(2*1024*1024)
#define CXIP_OFLOW_BUF_MIN_POSTED	3
#define CXIP_OFLOW_BUF_MAX_CACHED	(CXIP_OFLOW_BUF_MIN_POSTED * 3)
#define CXIP_REQ_BUF_SIZE		(2*1024*1024)
#define CXIP_REQ_BUF_MIN_POSTED		4
#define CXIP_REQ_BUF_MAX_CACHED		0
#define CXIP_UX_BUFFER_SIZE		(CXIP_OFLOW_BUF_MIN_POSTED * \
					 CXIP_OFLOW_BUF_SIZE)

/* When device memory is safe to access via load/store then the
 * CPU will be used to move data below this threshold.
 */
#define CXIP_SAFE_DEVMEM_COPY_THRESH	4096

#define CXIP_EP_PRI_CAPS \
	(FI_RMA | FI_ATOMICS | FI_TAGGED | FI_RECV | FI_SEND | \
	 FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE | \
	 FI_DIRECTED_RECV | FI_MSG | FI_NAMED_RX_CTX | \
	 FI_COLLECTIVE | FI_HMEM)
#define CXIP_EP_SEC_CAPS \
	(FI_SOURCE | FI_SOURCE_ERR | FI_LOCAL_COMM | \
	 FI_REMOTE_COMM | FI_RMA_EVENT | FI_MULTI_RECV | FI_FENCE | FI_TRIGGER)
#define CXIP_EP_CAPS (CXIP_EP_PRI_CAPS | CXIP_EP_SEC_CAPS)
#define CXIP_DOM_CAPS (FI_LOCAL_COMM | FI_REMOTE_COMM | FI_AV_USER_ID)
#define CXIP_CAPS (CXIP_DOM_CAPS | CXIP_EP_CAPS)
#define CXIP_MSG_ORDER			(FI_ORDER_SAS | \
					 FI_ORDER_WAW | \
					 FI_ORDER_RMA_WAW | \
					 FI_ORDER_ATOMIC_WAW | \
					 FI_ORDER_ATOMIC_WAR | \
					 FI_ORDER_ATOMIC_RAW | \
					 FI_ORDER_ATOMIC_RAR)

#define CXIP_EP_CQ_FLAGS \
	(FI_SEND | FI_TRANSMIT | FI_RECV | FI_SELECTIVE_COMPLETION | \
	 FI_COLLECTIVE)
#define CXIP_EP_CNTR_FLAGS \
	(FI_SEND | FI_RECV | FI_READ | FI_WRITE | FI_REMOTE_READ | \
	 FI_REMOTE_WRITE)

#define CXIP_INJECT_SIZE		C_MAX_IDC_PAYLOAD_UNR

/* Max TX size of 16,384 translate to a 4MiB command queue buffer. */
#define CXIP_MAX_TX_SIZE		16384U
#define CXIP_DEFAULT_TX_SIZE		512U

/* Some LEs need to be used for internally by CXI provider. The user facing
 * RX size should be updated to reflect this.
 *
 * Note: This value is an estimation and may be too high.
 */
#define CXI_PROV_LE_PER_EP		1024U

/* Maximum number of LEs per endpoint. */
#define LES_PER_EP_MAX			16384U

#define CXIP_MAX_RX_SIZE		(LES_PER_EP_MAX - CXI_PROV_LE_PER_EP)
#define CXIP_DEFAULT_RX_SIZE		512U

#define CXIP_MAJOR_VERSION		0
#define CXIP_MINOR_VERSION		1
#define CXIP_PROV_VERSION		FI_VERSION(CXIP_MAJOR_VERSION, \
						   CXIP_MINOR_VERSION)
#define CXIP_FI_VERSION			FI_VERSION(1, 21)
#define CXIP_WIRE_PROTO_VERSION		1

#define	CXIP_COLL_MAX_CONCUR		8
#define	CXIP_COLL_MIN_RX_BUFS		8
#define	CXIP_COLL_MIN_RX_SIZE		4096
#define	CXIP_COLL_MIN_MULTI_RECV	64
#define	CXIP_COLL_MAX_DATA_SIZE		32
#define	CXIP_COLL_MAX_SEQNO		(1 << 10)
// TODO adjust based on performance testing
#define	CXIP_COLL_MIN_RETRY_USEC	1
#define	CXIP_COLL_MAX_RETRY_USEC	32000
#define	CXIP_COLL_MIN_TIMEOUT_USEC	1
#define	CXIP_COLL_MAX_TIMEOUT_USEC	32000

#define CXIP_REQ_BUF_HEADER_MAX_SIZE (sizeof(struct c_port_fab_hdr) + \
	sizeof(struct c_port_unrestricted_hdr))
#define CXIP_REQ_BUF_HEADER_MIN_SIZE (sizeof(struct c_port_fab_hdr) + \
	sizeof(struct c_port_small_msg_hdr))

extern int s_page_size;
extern char cxip_prov_name[];
extern struct fi_provider cxip_prov;
extern struct util_prov cxip_util_prov;

extern int cxip_cq_def_sz;
extern int cxip_eq_def_sz;

extern struct slist cxip_if_list;

extern struct fi_fabric_attr cxip_fabric_attr;
extern struct fi_domain_attr cxip_domain_attr;
extern struct fi_ep_attr cxip_ep_attr;
extern struct fi_tx_attr cxip_tx_attr;
extern struct fi_rx_attr cxip_rx_attr;

enum cxip_ats_mlock_mode {
	CXIP_ATS_MLOCK_OFF,
	CXIP_ATS_MLOCK_CACHE,
	CXIP_ATS_MLOCK_ALL,
};

enum cxip_llring_mode {
	CXIP_LLRING_NEVER,
	CXIP_LLRING_IDLE,
	CXIP_LLRING_ALWAYS,
};

enum cxip_ep_ptle_mode {
	CXIP_PTLTE_HARDWARE_MODE,
	CXIP_PTLTE_DEFAULT_MODE = CXIP_PTLTE_HARDWARE_MODE,
	CXIP_PTLTE_SOFTWARE_MODE,
	CXIP_PTLTE_HYBRID_MODE,
};

enum cxip_rdzv_proto {
	CXIP_RDZV_PROTO_DEFAULT,	/* unrestricted gets */
	CXIP_RDZV_PROTO_ALT_READ,	/* restricted gets */
	CXIP_RDZV_PROTO_ALT_WRITE,	/* restricted puts */
};

const char *cxip_rdzv_proto_to_str(enum cxip_rdzv_proto proto);

struct cxip_environment {
	/* Translation */
	int odp;
	int force_odp;
	int ats;
	int iotlb;
	int disable_dmabuf_cuda;
	int disable_dmabuf_rocr;
	enum cxip_ats_mlock_mode ats_mlock_mode;

	/* Messaging */
	int fork_safe_requested;
	enum cxip_ep_ptle_mode rx_match_mode;
	int msg_offload;
	int trunc_ok;
	int hybrid_preemptive;
	int hybrid_recv_preemptive;
	size_t rdzv_threshold;
	size_t rdzv_get_min;
	size_t rdzv_eager_size;
	int rdzv_aligned_sw_rget;
	int rnr_max_timeout_us;
	int disable_non_inject_msg_idc;
	int disable_host_register;
	size_t oflow_buf_size;
	size_t oflow_buf_min_posted;
	size_t oflow_buf_max_cached;
	size_t safe_devmem_copy_threshold;
	size_t req_buf_size;
	size_t req_buf_min_posted;
	size_t req_buf_max_cached;
	int sw_rx_tx_init_max;
	int msg_lossless;
	size_t default_cq_size;
	size_t default_tx_size;
	size_t default_rx_size;
	int optimized_mrs;
	int prov_key_cache;
	int mr_match_events;
	int disable_eq_hugetlb;
	int zbcoll_radix;

	enum cxip_llring_mode llring_mode;

	int cq_policy;

	size_t default_vni;

	size_t eq_ack_batch_size;
	int fc_retry_usec_delay;
	size_t ctrl_rx_eq_max_size;
	char *device_name;
	size_t cq_fill_percent;
	int enable_unrestricted_end_ro;
	int rget_tc;
	int cacheline_size;

	char *coll_job_id;
	char *coll_job_step_id;
	size_t coll_retry_usec;
	size_t coll_timeout_usec;
	char *coll_fabric_mgr_url;
	char *coll_mcast_token;
	size_t hwcoll_addrs_per_job;
	size_t hwcoll_min_nodes;
	int coll_use_dma_put;

	char hostname[255];
	char *telemetry;
	int telemetry_rgid;
	int disable_hmem_dev_register;
	int ze_hmem_supported;
	enum cxip_rdzv_proto  rdzv_proto;
	int enable_trig_op_limit;
	int hybrid_posted_recv_preemptive;
	int hybrid_unexpected_msg_preemptive;
};

extern struct cxip_environment cxip_env;

static inline bool cxip_software_pte_allowed(void)
{
	return cxip_env.rx_match_mode != CXIP_PTLTE_HARDWARE_MODE;
}

/*
 * The CXI Provider Address format.
 *
 * A Cassini NIC Address and PID identify a libfabric Endpoint.  Cassini
 * borrows the name 'PID' from Portals. In CXI, a process can allocate several
 * PID values.
 *
 * The PID value C_PID_ANY is reserved. When used, the library auto-assigns
 * a free PID value. A PID value is assigned when network resources are
 * allocated. Libfabric clients can achieve this by not specifying a 'service'
 * in a call to fi_getinfo() or by not setting src_addr in the fi_info
 * structure used to allocate an Endpoint.
 */
struct cxip_addr {
	uint32_t pid		: C_DFA_PID_BITS_MAX;
	uint32_t nic		: C_DFA_NIC_BITS;
	uint16_t vni;
};

#define CXIP_ADDR_EQUAL(a, b) ((a).nic == (b).nic && (a).pid == (b).pid)
#define CXIP_ADDR_VNI_EQUAL(a, b) (CXIP_ADDR_EQUAL(a, b) && (a).vni == (b).vni)

/*
 * A PID contains "pid_granule" logical endpoints. The PID granule is set per
 * device and can be found in libCXI devinfo. The default pid_granule is 256.
 * These endpoints are partitioned by the provider for the following use:
 *
 * 0       RX Queue PtlTE
 * 16      Collective PtlTE entry
 * 17-116  Optimized write MR PtlTEs 0-99
 *         For Client specified keys:
 *           17-116 Non-cached optimized write MR PtlTEs 0-99
 *         For Provider specified keys:
 *           17-24 Cached optimized write MR PtlTEs 0-7
 *           25-116 Non-cached optimized write MR PtlTEs 8-99
 * 117     Standard client/provider cached/non-cached write MR
 *         PtlTE / Control messaging
 * 127     Rendezvous destination write PtlTE
 * 128-227 Optimized read MR PtlTEs 0-99
 *         For Client specified keys:
 *           128-227 Non-cached optimized read MR PtlTEs 0-99
 *         For Provider specified keys:
 *           128-135 Cached optimized read MR PtlTEs 0-7
 *           136-227 Non-cached optimized read MR PtlTEs 8-99
 * 228     Standard client or provider cached/non-cached read MR
 *         PtlTE
 * 229-237 Rendezvous restricted read PtlTE (TODO consider merge with MR)
 * 255     Rendezvous source PtlTE
 *
 * Note: Any logical endpoint within a PID granule that issues unrestricted Puts
 * MUST be within the logical endpoint range 0 - 127 and unrestricted Gets MUST
 * be within the logical endpoint range 128 - 255.
 */
#define CXIP_PTL_IDX_RXQ				0
#define CXIP_PTL_IDX_RNR_RXQ				1
#define CXIP_PTL_IDX_WRITE_MR_OPT_BASE			17
#define CXIP_PTL_IDX_READ_MR_OPT_BASE			128
#define CXIP_PTL_IDX_MR_OPT_CNT				100
#define CXIP_PTL_IDX_PROV_NUM_CACHE_IDX			8
#define CXIP_PTL_IDX_PROV_MR_OPT_CNT				\
	(CXIP_PTL_IDX_MR_OPT_CNT - CXIP_PTL_IDX_PROV_NUM_CACHE_IDX)

/* Map non-cached optimized MR keys (client or FI_MR_PROV_KEY)
 * to appropriate PTL index.
 */
#define CXIP_MR_PROV_KEY_MASK ((1ULL << 61) - 1)
#define CXIP_MR_PROV_KEY_ID_MASK ((1ULL << 16) - 1)
#define CXIP_MR_UNCACHED_KEY_TO_IDX(key) ((key) & CXIP_MR_PROV_KEY_ID_MASK)
#define CXIP_PTL_IDX_WRITE_MR_OPT(key)		\
	(CXIP_PTL_IDX_WRITE_MR_OPT_BASE +	\
	 CXIP_MR_UNCACHED_KEY_TO_IDX(key))
#define CXIP_PTL_IDX_READ_MR_OPT(key)		\
	(CXIP_PTL_IDX_READ_MR_OPT_BASE +	\
	 CXIP_MR_UNCACHED_KEY_TO_IDX(key))

/* Map cached FI_MR_PROV_KEY optimized MR LAC to Index */
#define CXIP_PTL_IDX_WRITE_PROV_CACHE_MR_OPT(lac)		\
	(CXIP_PTL_IDX_WRITE_MR_OPT_BASE + (lac))
#define CXIP_PTL_IDX_READ_PROV_CACHE_MR_OPT(lac)		\
	(CXIP_PTL_IDX_READ_MR_OPT_BASE + (lac))

#define CXIP_PTL_IDX_WRITE_MR_STD		117
#define CXIP_PTL_IDX_RDZV_DEST			127
#define CXIP_PTL_IDX_COLL			6
#define CXIP_PTL_IDX_CTRL			CXIP_PTL_IDX_WRITE_MR_STD
#define CXIP_PTL_IDX_READ_MR_STD		228
#define CXIP_PTL_IDX_RDZV_RESTRICTED_BASE	229
#define CXIP_PTL_IDX_RDZV_RESTRICTED(lac)			\
	(CXIP_PTL_IDX_RDZV_RESTRICTED_BASE + (lac))

#define CXIP_PTL_IDX_RDZV_SRC			255

/* The CXI provider supports both provider specified MR keys
 * (FI_MR_PROV_KEY MR mode) and client specified keys on a per-domain
 * basis.
 *
 * User specified keys:
 * Hardware resources limit the number of active keys to 16 bits.
 * Key size is 32-bit so there are only 64K unique keys.
 *
 * Provider specified keys:
 * The key size is 64-bits and is separated from the MR hardware
 * resources such that the associated MR can be cached if the
 * following criteria are met:
 *
 *     - The associated memory region is non-zero in length
 *     - The associated memory region mapping is cached
 *     - The MR is not bound to a counter
 *
 * Optimized caching is preferred by default.
 * TODO: Fallback to standard optimized if PTE can not be allocated.
 *
 * FI_MR_PROV_KEY MR are associated with a unique domain wide
 * 16-bit buffer ID, reducing the overhead of maintaining keys.
 * Provider keys should always be preferred over client keys
 * unless well known keys are not exchanged between peers.
 */
#define CXIP_MR_KEY_SIZE sizeof(uint32_t)
#define CXIP_MR_KEY_MASK ((1ULL << (8 * CXIP_MR_KEY_SIZE)) - 1)
#define CXIP_MR_VALID_OFFSET_MASK ((1ULL << 56) - 1)

/* For provider defined keys we define a 64 bit MR key that maps
 * to provider required information.
 */
struct cxip_mr_key {
	union {
		/* Provider generated standard cached */
		struct {
			uint64_t lac	: 3;
			uint64_t lac_off: 58;
			uint64_t opt	: 1;
			uint64_t cached	: 1;
			uint64_t unused1: 1;
			/* shares CXIP_CTRL_LE_TYPE_MR */
		};
		/* Client or Provider non-cached */
		struct {
			uint64_t key	: 61;
			uint64_t unused2: 3;
			/* Provider shares opt */
			/* Provider shares cached == 0 */
			/* Provider shares CXIP_CTRL_LE_TYPE_MR */
		};
		/* Provider Key Only */
		struct {
			/* Non-cached key consists of unique MR ID and sequence
			 * number. The same MR ID can be used with sequence
			 * number to create 2^44 unique keys. That is, a
			 * single standard MR repeatedly created and destroyed
			 * every micro-second, would take months before
			 * it repeated.
			 */
			uint64_t id     : 16;  /* Unique - 64K MR */
			uint64_t seqnum : 44;  /* Sequence with random seed */
			uint64_t events : 1;   /* Requires event generation */
			uint64_t unused3: 2;
			uint64_t is_prov: 1;
			/* Overloads CXIP_CTRL_LE_TYPE_MR and must be cleared
			 * before appending MR LE or TX using in match bits.
			 */
		};
		uint64_t raw;
	};
};

#define CXIP_MR_PROV_KEY_SIZE sizeof(struct cxip_mr_key)
#define CXIP_NUM_CACHED_KEY_LE 8

struct cxip_domain;
struct cxip_mr_domain;
struct cxip_mr;

/* CXI provider MR operations that are specific for the MR
 * based on MR key type and caching.
 */
struct cxip_mr_util_ops {
	bool is_cached;
	int (*init_key)(struct cxip_mr *mr, uint64_t req_key);
	int (*enable_opt)(struct cxip_mr *mr);
	int (*disable_opt)(struct cxip_mr *mr);
	int (*enable_std)(struct cxip_mr *mr);
	int (*disable_std)(struct cxip_mr *mr);
};

struct cxip_ep_obj;

/*
 * cxip_ctrl_mr_cache_flush() - Flush LE associated with remote MR cache.
 */
void cxip_ctrl_mr_cache_flush(struct cxip_ep_obj *ep_obj);

/*
 * cxip_adjust_remote_offset() - Update address with the appropriate offset
 * for key.
 */
static inline
uint64_t cxip_adjust_remote_offset(uint64_t *addr, uint64_t key)
{
	struct cxip_mr_key cxip_key = {
		.raw = key,
	};

	if (cxip_key.cached) {
		*addr += cxip_key.lac_off;
		if (*addr & ~CXIP_MR_VALID_OFFSET_MASK)
			return -FI_EINVAL;
	}
	return FI_SUCCESS;
}

int cxip_generic_mr_key_to_ptl_idx(struct cxip_domain *dom,
				   uint64_t key, bool write);
bool cxip_generic_is_mr_key_opt(uint64_t key);
bool cxip_generic_is_mr_key_events(uint64_t caps, uint64_t key);
bool cxip_generic_is_valid_mr_key(uint64_t key);

/* Messaging Match Bit layout */
#define CXIP_TX_ID_WIDTH	11
#define CXIP_TAG_WIDTH		48
#define CXIP_RDZV_ID_CMD_WIDTH	8
#define CXIP_RDZV_ID_HIGH_WIDTH 7
#define CXIP_TOTAL_RDZV_ID_WIDTH (CXIP_RDZV_ID_CMD_WIDTH +	\
				  CXIP_RDZV_ID_HIGH_WIDTH)
#define CXIP_TAG_MASK		((1UL << CXIP_TAG_WIDTH) - 1)

#define CXIP_CS_TAG_WIDTH	40
#define CXIP_VNI_WIDTH		16
#define CXIP_CS_TAG_MASK	((1UL << CXIP_CS_TAG_WIDTH) - 1)

/* Define several types of LEs */
enum cxip_le_type {
	CXIP_LE_TYPE_RX = 0,	/* RX data LE */
	CXIP_LE_TYPE_ZBP,	/* Zero-byte Put control message LE. Used to
				 * exchange data in the EQ header_data and
				 * match_bits fields. Unexpected headers are
				 * disabled.
				 */
};

enum cxip_ctrl_le_type {
	CXIP_CTRL_LE_TYPE_MR = 0,	/* Memory Region LE */
	CXIP_CTRL_LE_TYPE_CTRL_MSG,	/* Control Message LE */
};

enum cxip_ctrl_msg_type {
	CXIP_CTRL_MSG_FC_NOTIFY = 0,
	CXIP_CTRL_MSG_FC_RESUME,
	CXIP_CTRL_MSG_ZB_DATA,
};

union cxip_match_bits {
	struct {
		uint64_t tag        : CXIP_TAG_WIDTH; /* User tag value */
		uint64_t tx_id      : CXIP_TX_ID_WIDTH; /* Prov. tracked ID */
		uint64_t cq_data    : 1;  /* Header data is valid */
		uint64_t tagged     : 1;  /* Tagged API */
		uint64_t match_comp : 1;  /* Notify initiator on match */
		uint64_t rdzv_done  : 1;  /* Notify initiator when rdzv done */
		uint64_t le_type    : 1;
	};
	/* Rendezvous protocol request, overloads match_comp and rdzv_done
	 * to specify requested protocol.
	 */
	struct {
		uint64_t pad0       : 61;
		uint64_t rdzv_proto : 2;
		uint64_t pad1       : 1;
	};
	/* Split TX ID for rendezvous operations. */
	struct {
		uint64_t pad2       : CXIP_TAG_WIDTH; /* User tag value */
		uint64_t rdzv_id_hi : CXIP_RDZV_ID_HIGH_WIDTH;
		uint64_t rdzv_lac   : 4;  /* Rendezvous Get LAC */
	};
	struct {
		uint64_t rdzv_id_lo : CXIP_RDZV_ID_CMD_WIDTH;
	};
	/* Client/Server messaging match bits */
	struct {
		uint64_t rnr_tag     : CXIP_CS_TAG_WIDTH; /* User tag value */
		uint64_t rnr_rsvd    : 6;		 /* Unused, set to 0 */
		uint64_t rnr_cq_data : 1;		 /* Header data valid */
		uint64_t rnr_tagged  : 1;		 /* Tagged API */
		uint64_t rnr_vni     : CXIP_VNI_WIDTH;	 /* Source VNI */
	};
	/* Control LE match bit format for notify/resume */
	struct {
		uint64_t txc_id       : 8;
		uint64_t rxc_id       : 8;
		uint64_t drops        : 16;
		uint64_t pad3         : 29;
		uint64_t ctrl_msg_type: 2;
		uint64_t ctrl_le_type : 1;
	};
	/* Control LE match bit format for zbcollectives */
	struct {
		uint64_t zb_data       :61;
		uint64_t zb_pad        : 3;
		/* shares ctrl_le_type == CXIP_CTRL_LE_TYPE_CTRL_MSG
		 * shares ctrl_msg_type == CXIP_CTRL_MSG_ZB_BCAST
		 */
	};
	/* Control LE match bit format for cached MR */
	struct {
		uint64_t mr_lac		: 3;
		uint64_t mr_lac_off	: 58;
		uint64_t mr_opt		: 1;
		uint64_t mr_cached	: 1;
		uint64_t mr_unused	: 1;
		/* shares ctrl_le_type == CXIP_CTRL_LE_TYPE_MR */
	};
	struct {
		uint64_t mr_key		: 61;
		uint64_t mr_pad		: 3;
		/* shares mr_opt
		 * shares mr_cached == 0
		 * shares ctrl_le_type == CXIP_CTRL_LE_TYPE_MR
		 */
	};
	struct {
		uint64_t unused2	: 63;
		uint64_t is_prov	: 1;
		/* Indicates provider generated key and shares ctrl_le_type ==
		 * CXIP_CTRL_LE_TYPE_MR so it must be cleared before matching.
		 */
	};
	uint64_t raw;
};
#define CXIP_IS_PROV_MR_KEY_BIT (1ULL << 63)
#define CXIP_KEY_MATCH_BITS(key) ((key) & ~CXIP_IS_PROV_MR_KEY_BIT)

/* libcxi Wrapper Structures */

#define CXI_PLATFORM_ASIC 0
#define CXI_PLATFORM_NETSIM 1
#define CXI_PLATFORM_Z1 2
#define CXI_PLATFORM_FPGA 3

/*
 * CXI Device wrapper
 *
 * There will be one of these for every local Cassini device on the node.
 */
struct cxip_if {
	struct slist_entry if_entry;

	/* Device description */
	struct cxil_devinfo *info;
	int speed;
	int link;

	struct cxil_dev *dev;

	/* PtlTEs (searched during state change events) */
	struct dlist_entry ptes;

	ofi_atomic32_t ref;
	ofi_spin_t lock;
};

/*
 * CXI communication profile wrapper.
 *
 * The wrapper is used to remap user requested traffic class to a communication
 * profile which actually can be allocated.
 */
struct cxip_remap_cp {
	struct dlist_entry remap_entry;
	struct cxi_cp remap_cp;
	struct cxi_cp *hw_cp;
};

/*
 * CXI Logical Network Interface (LNI) wrapper
 *
 * An LNI is a container used allocate resources from a NIC.
 */
struct cxip_lni {
	struct cxip_if *iface;
	struct cxil_lni *lni;

	/* Hardware communication profiles */
	struct cxi_cp *hw_cps[16];
	int n_cps;

	/* Software remapped communication profiles. */
	struct dlist_entry remap_cps;

	ofi_spin_t lock;
};

/* A portals table define a network endpoint address. The endpoint address is
 * a {NIC + PID} and this can be configured against multiple VNIs
 */
struct cxip_portals_table {
	struct cxip_lni *lni;
	uint32_t pid;
	struct cxil_domain **doms;
	size_t doms_count;
};

int cxip_portals_table_alloc(struct cxip_lni *lni, uint16_t *vni,
			     size_t vni_count, uint32_t pid,
			     struct cxip_portals_table **ptable);
void cxip_portals_table_free(struct cxip_portals_table *ptable);

struct cxip_pte_map_entry {
        struct dlist_entry entry;
        struct cxil_pte_map *map;
};

/*
 * CXI Portal Table Entry (PtlTE) wrapper
 *
 * Represents PtlTE mapped in a CXI domain.
 */
struct cxip_pte {
	struct dlist_entry pte_entry;
	struct cxip_portals_table *ptable;
	struct cxil_pte *pte;
	enum c_ptlte_state state;
	struct dlist_entry map_list;

	void (*state_change_cb)(struct cxip_pte *pte,
				const union c_event *event);
	void *ctx;
};

/*
 * CXI Command Queue wrapper
 */
struct cxip_cmdq {
	struct cxi_cq *dev_cmdq;
	struct c_cstate_cmd c_state;
	enum cxip_llring_mode llring_mode;

	struct cxi_cp *cur_cp;
	struct cxip_lni *lni;
};

int cxip_cmdq_emit_idc_put(struct cxip_cmdq *cmdq,
			   const struct c_cstate_cmd *c_state,
			   const struct c_idc_put_cmd *put, const void *buf,
			   size_t len, uint64_t flags);
int cxip_cmdq_emit_dma(struct cxip_cmdq *cmdq, struct c_full_dma_cmd *dma,
		       uint64_t flags);
int cxip_cmdq_emic_idc_amo(struct cxip_cmdq *cmdq,
			   const struct c_cstate_cmd *c_state,
			   const struct c_idc_amo_cmd *amo, uint64_t flags,
			   bool fetching, bool flush);
int cxip_cmdq_emit_dma_amo(struct cxip_cmdq *cmdq, struct c_dma_amo_cmd *amo,
			   uint64_t flags, bool fetching, bool flush);
int cxip_cmdq_emit_idc_msg(struct cxip_cmdq *cmdq,
			   const struct c_cstate_cmd *c_state,
			   const struct c_idc_msg_hdr *msg, const void *buf,
			   size_t len, uint64_t flags);

/* OFI Provider Structures */

/*
 * CXI Provider Fabric object
 */
struct cxip_fabric {
	struct util_fabric util_fabric;
	ofi_atomic32_t ref;
};

/*
 * CXI Provider Memory Descriptor
 */
struct cxip_md {
	struct cxip_domain *dom;
	struct cxi_md *md;
	struct ofi_mr_info info;
	uint64_t handle;
	bool handle_valid;
	bool cached;
};

#define CXIP_MR_DOMAIN_HT_BUCKETS 16

struct cxip_mr_domain {
	struct dlist_entry buckets[CXIP_MR_DOMAIN_HT_BUCKETS];
	ofi_spin_t lock;
};

void cxip_mr_domain_init(struct cxip_mr_domain *mr_domain);
void cxip_mr_domain_fini(struct cxip_mr_domain *mr_domain);

struct cxip_telemetry {
	struct cxip_domain *dom;

	/* List of telemetry entries to being monitored. */
	struct dlist_entry telemetry_list;
};

void cxip_telemetry_dump_delta(struct cxip_telemetry *telemetry);
void cxip_telemetry_free(struct cxip_telemetry *telemetry);
int cxip_telemetry_alloc(struct cxip_domain *dom,
			 struct cxip_telemetry **telemetry);

#define TELEMETRY_ENTRY_NAME_SIZE 64U

struct cxip_telemetry_entry {
	struct cxip_telemetry *telemetry;
	struct dlist_entry telemetry_entry;

	/* Telemetry name. */
	char name[TELEMETRY_ENTRY_NAME_SIZE];

	/* Telemetry value. */
	unsigned long value;
};

struct cxip_domain_cmdq {
	struct dlist_entry entry;
	struct cxip_cmdq *cmdq;
};

/*
 * CXI Provider Domain object
 */
struct cxip_domain {
	struct util_domain util_domain;
	struct cxip_fabric *fab;
	ofi_spin_t lock;
	ofi_atomic32_t ref;

	uint32_t tclass;

	struct cxip_eq *eq; //unused
	struct cxip_eq *mr_eq; //unused

	/* Assigned NIC address */
	uint32_t nic_addr;

	/* Device info */
	struct cxip_if *iface;

	/* Device partition */
	struct cxip_lni *lni;

	/* Trigger and CT support */
	struct cxip_cmdq *trig_cmdq;
	struct ofi_genlock trig_cmdq_lock;
	bool cntr_init;

	/* Provider generated RKEYs, else client */
	bool is_prov_key;

	/* Can disable caching of provider generated RKEYs */
	bool prov_key_cache;

	/* Provider generated RKEYs optimized MR disablement/enablement */
	bool optimized_mrs;

	/* Enable MR match event counting enables a more robust
	 * MR when using FI_MR_PROV_KEY. It disables hardware cached
	 * MR keys and ensures memory backing a MR cannot be
	 * remotely accessed even if that memory remains in the
	 * libfabric MR cache.
	 */
	bool mr_match_events;

	/* Domain wide MR resources.
	 *   Req IDs are control buffer IDs to map MR or MR cache to an LE.
	 *   MR IDs are used by non-cached provider key MR to decouple the
	 *   MR and Req ID, and do not map directly to the MR LE.
	 */
	ofi_spin_t ctrl_id_lock;
	struct indexer req_ids;
	struct indexer mr_ids;

	/* If FI_MR_PROV_KEY is not cached, keys include a sequence number
	 * to reduce the likelyhood of a stale key being used to access
	 * a recycled MR key.
	 */
	uint32_t prov_key_seqnum;

	/* Translation cache */
	struct ofi_mr_cache iomm;
	bool odp;
	bool ats;
	bool hmem;

	/* ATS translation support */
	struct cxip_md scalable_md;
	bool scalable_iomm;
	bool rocr_dev_mem_only;

	/* Domain state */
	bool enabled;

	/* List of allocated resources used for deferred work queue processing.
	 */
	struct dlist_entry txc_list;
	struct dlist_entry cntr_list;
	struct dlist_entry cq_list;

	struct fi_hmem_override_ops hmem_ops;
	bool hybrid_mr_desc;

	/* Container of in-use MRs against this domain. */
	struct cxip_mr_domain mr_domain;

	/* Counters collected for the duration of the domain existence. */
	struct cxip_telemetry *telemetry;

	/* NIC AMO operation which is remapped to a PCIe operation. */
	int amo_remap_to_pcie_fadd;

	/* Maximum number of triggered operations configured for the service
	 * ID.
	 */
	int max_trig_op_in_use;
	sem_t *trig_op_lock;

	/* Domain has been configured with FI_AV_AUTH_KEY. */
	bool av_auth_key;

	/* This is only valid if FI_AV_AUTH_KEY is false. */
	struct cxi_auth_key auth_key;

	/* Maximum number of auth keys requested by user. */
	size_t auth_key_entry_max;

	/* Domain has been configured with FI_AV_USER_ID. */
	bool av_user_id;

	/* Domain level TX command queues used when number of authorization
	 * keys exceeds LCID limit.
	 */
	struct dlist_entry cmdq_list;
	unsigned int cmdq_cnt;
	struct ofi_genlock cmdq_lock;
	size_t tx_size;
};

int cxip_domain_emit_idc_put(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     const struct c_cstate_cmd *c_state,
			     const struct c_idc_put_cmd *put, const void *buf,
			     size_t len, uint64_t flags);
int cxip_domain_emit_dma(struct cxip_domain *dom, uint16_t vni,
			 enum cxi_traffic_class tc, struct c_full_dma_cmd *dma,
			 uint64_t flags);
int cxip_domain_emit_idc_amo(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     const struct c_cstate_cmd *c_state,
			     const struct c_idc_amo_cmd *amo, uint64_t flags,
			     bool fetching, bool flush);
int cxip_domain_emit_dma_amo(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     struct c_dma_amo_cmd *amo, uint64_t flags,
			     bool fetching, bool flush);
int cxip_domain_emit_idc_msg(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     const struct c_cstate_cmd *c_state,
			     const struct c_idc_msg_hdr *msg, const void *buf,
			     size_t len, uint64_t flags);

static inline bool cxip_domain_mr_cache_enabled(struct cxip_domain *dom)
{
	return dom->iomm.domain == &dom->util_domain;
}

static inline bool cxip_domain_mr_cache_iface_enabled(struct cxip_domain *dom,
						      enum fi_hmem_iface iface)
{
	return cxip_domain_mr_cache_enabled(dom) && dom->iomm.monitors[iface];
}

int cxip_domain_valid_vni(struct cxip_domain *dom, unsigned int vni);

/* This structure implies knowledge about the breakdown of the NIC address,
 * which is taken from the AMA, that the provider does not know in a flexible
 * way. However, the domain fi_open_ops() API includes a topology function
 * that requires knowledge of the address breakdown into topology components.
 * TODO: Research a less restricted way to get this information.
 */
#define CXIP_ADDR_PORT_BITS 6
#define CXIP_ADDR_SWITCH_BITS 5
#define CXIP_ADDR_GROUP_BITS 9
#define CXIP_ADDR_FATTREE_PORT_BITS 6
#define CXIP_ADDR_FATTREE_SWITCH_BITS 14

struct cxip_topo_addr {
	union {
		uint32_t addr;
		struct {
			uint32_t port_num:CXIP_ADDR_PORT_BITS;
			uint32_t switch_num:CXIP_ADDR_SWITCH_BITS;
			uint32_t group_num:CXIP_ADDR_GROUP_BITS;
		} dragonfly;
		struct {
			uint32_t port_num:CXIP_ADDR_FATTREE_PORT_BITS;
			uint32_t switch_num:CXIP_ADDR_FATTREE_SWITCH_BITS;
		} fat_tree;
	};
};

static inline ssize_t
cxip_copy_to_hmem_iov(struct cxip_domain *domain, enum fi_hmem_iface hmem_iface,
		      uint64_t device, const struct iovec *hmem_iov,
		      size_t hmem_iov_count, uint64_t hmem_iov_offset,
		      const void *src, size_t size)
{
	return domain->hmem_ops.copy_to_hmem_iov(hmem_iface, device, hmem_iov,
						 hmem_iov_count,
						 hmem_iov_offset, src, size);
}

/*
 *  Event Queue
 *
 *  libfabric fi_eq implementation.
 *
 *  Created in cxip_eq_open().
 */
struct cxip_eq {
	struct util_eq util_eq;
	struct fi_eq_attr attr;
	struct dlist_entry ep_list;
	ofi_mutex_t list_lock;
};

#define CXIP_EQ_MAP_FLAGS \
	(CXI_MAP_WRITE | CXI_MAP_PIN | CXI_MAP_IOVA_ALLOC)

/*
 * RMA request
 *
 * Support structures, accumulated in a union.
 */
struct cxip_req_rma {
	struct cxip_txc *txc;
	struct cxip_md *local_md;	// RMA target buffer
	void *ibuf;
};

struct cxip_req_amo {
	struct cxip_txc *txc;
	struct cxip_md *result_md;
	struct cxip_md *oper1_md;
	char result[16];
	char oper1[16];
	bool tmp_result;
	bool tmp_oper1;
	void *ibuf;
	bool fetching_amo_flush;
	uint8_t fetching_amo_flush_event_count;
	unsigned int fetching_amo_flush_event_rc;
	struct cxip_cntr *fetching_amo_flush_cntr;
};

/* Used with receive request to maintain state associated
 * with MQD support for dumping unexpected messages.
 */
struct cxip_ux_dump_state {
	bool done;

	size_t max_count;	/* Number entries/src_addr provided */
	size_t ret_count;	/* Number of UX entries returned */
	size_t ux_count;	/* Total UX entries available */

	struct fi_cq_tagged_entry *entry;
	fi_addr_t *src_addr;
};

struct cxip_req_recv {
	/* Receive parameters */
	struct dlist_entry rxc_entry;
	union {
		struct cxip_rxc *rxc;
		struct cxip_rxc_hpc *rxc_hpc;
		struct cxip_rxc_rnr *rxc_rnr;
	};

	struct cxip_cntr *cntr;
	void *recv_buf;			// local receive buffer
	struct cxip_md *recv_md;	// local receive MD
	bool hybrid_md;			// True if MD was provided
	bool success_disable;
	uint32_t ulen;			// User buffer length
	bool tagged;
	uint64_t tag;
	uint64_t ignore;
	uint32_t match_id;
	uint64_t flags;

	/* FI_CLAIM work around to hold UX remote offsets for duration of
	 * H/W UX entry matching and deletion. Array of 8-byte unexpected
	 * headers remote offsets, and current remote offset used when
	 * processing search results to match remote offsets.
	 */
	uint64_t *ule_offsets;
	uint64_t ule_offset;
	unsigned int num_ule_offsets;
	unsigned int cur_ule_offsets;
	bool offset_found;

	/* UX list dump state */
	struct cxip_ux_dump_state *ux_dump;

	/* Control info */
	int rc;				// DMA return code
	uint32_t rlen;			// Send length
	uint64_t oflow_start;		// Overflow buffer address
	uint16_t vni;			// VNI operation came in on
	uint32_t initiator;		// DMA initiator address
	uint32_t rdzv_id;		// DMA initiator rendezvous ID
	uint8_t rdzv_lac;		// Rendezvous source LAC
	bool done_notify;		// Must send done notification
	enum cxip_rdzv_proto rdzv_proto;
	int rdzv_events;		// Processed rdzv event count
	enum c_event_type rdzv_event_types[4];
	uint32_t rdzv_initiator;	// Rendezvous initiator used for mrecvs
	uint32_t rget_nic;
	uint32_t rget_pid;
	bool software_list;		// Appended to HW or SW
	bool canceled;			// Request canceled?
	bool unlinked;
	bool multi_recv;
	bool tgt_event;
	uint64_t start_offset;
	uint64_t mrecv_bytes;
	uint64_t mrecv_unlink_bytes;
	bool auto_unlinked;
	bool hw_offloaded;
	struct cxip_req *parent;
	struct dlist_entry children;
	uint64_t src_offset;
	uint16_t rdzv_mlen;
};

struct cxip_req_send {
	/* Send parameters */
	union {
		struct cxip_txc *txc;
		struct cxip_txc_hpc *txc_hpc;
		struct cxip_txc_rnr *txc_rnr;
	};
	struct cxip_cntr *cntr;
	const void *buf;		// local send buffer
	size_t len;			// request length
	struct cxip_md *send_md;	// send buffer memory descriptor
	struct cxip_addr caddr;
	fi_addr_t dest_addr;
	bool tagged;
	bool hybrid_md;
	bool success_disable;
	uint32_t tclass;
	uint64_t tag;
	uint64_t data;
	uint64_t flags;
	void *ibuf;

	/* Control info */
	struct dlist_entry txc_entry;
	struct cxip_fc_peer *fc_peer;
	union {
		int rdzv_id;		// SW RDZV ID for long messages
		int tx_id;
	};
	int rc;				// DMA return code
	int rdzv_send_events;		// Processed event count
	uint64_t max_rnr_time;
	uint64_t retry_rnr_time;
	struct dlist_entry rnr_entry;
	int retries;
	bool canceled;
};

struct cxip_req_rdzv_src {
	struct dlist_entry list;
	struct cxip_txc *txc;
	uint32_t lac;
	int rc;
};

struct cxip_req_search {
	struct cxip_rxc_hpc *rxc;
	bool complete;
	int puts_pending;
};

struct cxip_req_coll {
	struct cxip_coll_pte *coll_pte;
	struct cxip_coll_buf *coll_buf;
	uint32_t mrecv_space;
	size_t hw_req_len;
	bool isred;
	enum c_return_code cxi_rc;
};

enum cxip_req_type {
	CXIP_REQ_RMA,
	CXIP_REQ_AMO,
	CXIP_REQ_OFLOW,
	CXIP_REQ_RECV,
	CXIP_REQ_SEND,
	CXIP_REQ_RDZV_SRC,
	CXIP_REQ_SEARCH,
	CXIP_REQ_COLL,
	CXIP_REQ_RBUF,
};

/*
 * Async Request
 *
 * Support structure.
 *
 * Created in cxip_cq_req_alloc().
 *
 * This implements an async-request/callback mechanism. It uses the libfabric
 * utility pool, which provides a pool of reusable memory objects that supports
 * a fast lookup through the req_id index value, and can be bound to a CQ.
 *
 * The request is allocated and bound to the CQ, and then the command is
 * issued. When the completion queue signals completion, this request is found,
 * and the callback function is called.
 */
struct cxip_req {
	/* Control info */
	struct dlist_entry evtq_entry;
	void *req_ctx;
	struct cxip_cq *cq;		// request CQ
	struct cxip_evtq *evtq;		// request event queue
	int req_id;			// fast lookup in index table
	int (*cb)(struct cxip_req *req, const union c_event *evt);
					// completion event callback
	bool discard;

	/* Triggered related fields. */
	bool triggered;
	uint64_t trig_thresh;
	struct cxip_cntr *trig_cntr;

	/* CQ event fields, set according to fi_cq.3
	 *   - set by provider
	 *   - returned to user in completion event
	 */
	uint64_t context;
	uint64_t flags;
	uint64_t data_len;
	uint64_t buf;
	uint64_t data;
	uint64_t tag;
	fi_addr_t addr;

	/* Request parameters */
	enum cxip_req_type type;
	union {
		struct cxip_req_rma rma;
		struct cxip_req_amo amo;
		struct cxip_req_recv recv;
		struct cxip_req_send send;
		struct cxip_req_rdzv_src rdzv_src;
		struct cxip_req_search search;
		struct cxip_req_coll coll;
	};
};

static inline bool cxip_is_trig_req(struct cxip_req *req)
{
	return req->trig_cntr != NULL;
}

struct cxip_ctrl_req_mr {
	struct cxip_mr *mr;
};

struct cxip_ctrl_send {
	uint32_t nic_addr;
	uint32_t pid;
	uint16_t vni;
	union cxip_match_bits mb;
};

struct cxip_ctrl_req {
	struct dlist_entry ep_entry;
	struct cxip_ep_obj *ep_obj;
	int req_id;
	int (*cb)(struct cxip_ctrl_req *req, const union c_event *evt);

	union {
		struct cxip_ctrl_req_mr mr;
		struct cxip_ctrl_send send;
	};
};

struct cxip_mr_lac_cache {
	/* MR referencing the associated MR cache LE, can only
	 * be flushed if reference count is 0.
	 */
	ofi_atomic32_t ref;
	union cxip_match_bits mb;
	struct cxip_ctrl_req *ctrl_req;
};

struct cxip_fc_peer {
	struct dlist_entry txc_entry;
	struct cxip_txc_hpc *txc;
	struct cxip_ctrl_req req;
	struct cxip_addr caddr;
	struct dlist_entry msg_queue;
	uint16_t pending;
	uint16_t dropped;
	uint16_t pending_acks;
	bool replayed;
	unsigned int retry_count;
};

struct cxip_fc_drops {
	struct dlist_entry rxc_entry;
	struct cxip_rxc_hpc *rxc;
	struct cxip_ctrl_req req;
	uint32_t nic_addr;
	uint32_t pid;
	uint16_t vni;
	uint16_t drops;
	unsigned int retry_count;
};

/* Completion queue specific wrapper around CXI event queue. */
struct cxip_cq_eq {
	struct cxi_eq *eq;
	void *buf;
	size_t len;
	struct cxi_md *md;
	bool mmap;
	unsigned int unacked_events;
	struct c_eq_status prev_eq_status;
	bool eq_saturated;
};

struct cxip_evtq {
	struct cxi_eq *eq;
	void *buf;
	size_t len;
	struct cxi_md *md;
	bool mmap;
	unsigned int unacked_events;
	unsigned int ack_batch_size;
	struct c_eq_status prev_eq_status;
	bool eq_saturated;

	/* Point back to CQ */
	struct cxip_cq *cq;

	/* Protected with ep_ob->lock */
	struct ofi_bufpool *req_pool;
	struct indexer req_table;
	struct dlist_entry req_list;
};

/*
 * CXI Libfbric software completion queue
 */
struct cxip_cq {
	struct util_cq util_cq;
	struct fi_cq_attr attr;

	/* Implement our own CQ ep_list_lock since common code util_cq
	 * implementation is a mutex and can not be optimized. This lock
	 * is always taken walking the CQ EP, but can be optimized to no-op.
	 */
	struct ofi_genlock ep_list_lock;

	/* Internal CXI wait object allocated only if required. */
	struct cxil_wait_obj *priv_wait;

	/* CXI specific fields. */
	struct cxip_domain *domain;
	unsigned int ack_batch_size;
	struct dlist_entry dom_entry;
};

static inline uint16_t cxip_evtq_eqn(struct cxip_evtq *evtq)
{
	return evtq->eq->eqn;
}

/*
 * CXI libfabric completion counter
 */
struct cxip_cntr {
	struct fid_cntr cntr_fid;
	struct cxip_domain *domain;	// parent domain
	ofi_atomic32_t ref;
	struct fi_cntr_attr attr;	// copy of user or default attributes
	struct fid_wait *wait;
	/* Contexts to which counter is bound */
	struct dlist_entry ctx_list;

	ofi_mutex_t lock;

	struct cxi_ct *ct;
	struct c_ct_writeback *wb;
	uint64_t wb_device;
	enum fi_hmem_iface wb_iface;
	uint64_t wb_handle;
	bool wb_handle_valid;
	struct c_ct_writeback lwb;

	struct dlist_entry dom_entry;
};

struct cxip_ux_send {
	struct dlist_entry rxc_entry;
	struct cxip_req *req;
	union c_event put_ev;
	bool claimed;			/* Reserved with FI_PEEK | FI_CLAIM */
};

/* Key used to associate PUT and PUT_OVERFLOW events */
union cxip_def_event_key {
	struct {
		uint64_t initiator	: 32;
		uint64_t rdzv_id	: 15;
		uint64_t pad0		: 16;
		uint64_t rdzv		: 1;
	};
	struct {
		uint64_t start_addr	: 57;
		uint64_t pad1		: 7;
	};
	uint64_t raw;
};

struct cxip_deferred_event {
	struct dlist_entry rxc_entry;
	union cxip_def_event_key key;
	struct cxip_req *req;
	union c_event ev;
	uint64_t mrecv_start;
	uint32_t mrecv_len;

	struct cxip_ux_send *ux_send;
};

/* A very specific (non-generic) hash table is used to map
 * deferred CXI events to associate PUT and PUT_OVERFLOW events.
 * Hash entries are added and removed at a high rate and the
 * overhead of generic implementations is insufficient.
 */
#define CXIP_DEF_EVENT_HT_BUCKETS	256

struct def_event_ht {
	struct dlist_entry bh[CXIP_DEF_EVENT_HT_BUCKETS];
};

/*
 * Zero-buffer collectives.
 */
#define	ZB_NOSIM	-1
#define	ZB_ALLSIM	-2

struct cxip_zbcoll_obj;
typedef void (*zbcomplete_t)(struct cxip_zbcoll_obj *zb, void *usrptr);

struct cxip_zbcoll_cb_obj {
	zbcomplete_t usrfunc;		// callback function
	void *usrptr;			// callback data
};

/* Used to track state for one or more zbcoll endpoints */
struct cxip_zbcoll_state {
	struct cxip_zbcoll_obj *zb;	// backpointer to zbcoll_obj
	uint64_t *dataptr;		// user-supplied target
	uint64_t dataval;		// collective data
	int num_relatives;		// number of nearest relatives
	int *relatives;			// nearest relative indices
	int contribs;			// contribution count
	int grp_rank;			// local rank within group
};

/* Used to track concurrent zbcoll operations */
struct cxip_zbcoll_obj {
	struct dlist_entry ready_link;	// link to zb_coll ready_list
	struct cxip_ep_obj *ep_obj;	// backpointer to endpoint
	struct cxip_zbcoll_state *state;// state array
	struct cxip_addr *caddrs;	// cxip addresses in collective
	int num_caddrs;			// number of cxip addresses
	zbcomplete_t userfunc;		// completion callback function
	void *userptr;			// completion callback data
	uint64_t *grpmskp;		// pointer to global group mask
	uint32_t *shuffle;		// TEST shuffle array
	int simcount;			// TEST count of states
	int simrank;			// TEST simulated rank
	int simref;			// TEST zb0 reference count
	int busy;			// serialize collectives in zb
	int grpid;			// zb collective grpid
	int error;			// error code
	int reduce;			// set to report reduction data
};

/* zbcoll extension to struct cxip_ep_obj */
struct cxip_ep_zbcoll_obj {
	struct dlist_entry ready_list;	// zbcoll ops ready to advance
	struct cxip_zbcoll_obj **grptbl;// group lookup table
	uint64_t grpmsk;		// mask of used grptbl entries
	int refcnt;			// grptbl reference count
	bool disable;			// low level tests
	ofi_spin_t lock;		// group ID negotiation lock
	ofi_atomic32_t dsc_count;	// cumulative RCV discard count
	ofi_atomic32_t err_count;	// cumulative ACK error count
	ofi_atomic32_t ack_count;	// cumulative ACK success count
	ofi_atomic32_t rcv_count;	// cumulative RCV success count
};

/*
 * Collectives context.
 *
 * Extension to cxip_ep_obj for collectives.
 *
 * Initialized in cxip_coll_init() during EP creation.
 */
struct cxip_ep_coll_obj {
	struct index_map mcast_map;	// mc address -> object
	struct dlist_entry mc_list;	// list of mcast addresses
	struct cxip_coll_pte *coll_pte;	// PTE extensions
	struct dlist_ts sched_list;	// scheduled actions
	struct cxip_cmdq *rx_cmdq;	// shared with STD EP
	struct cxip_cmdq *tx_cmdq;	// shared with STD EP
	struct cxip_cntr *rx_cntr;	// shared with STD EP
	struct cxip_cntr *tx_cntr;	// shared with STD EP
	struct cxip_evtq *rx_evtq;	// shared with STD EP
	struct cxip_evtq *tx_evtq;	// shared with STD EP
	struct cxip_eq *eq;		// shared with STD EP
	ofi_atomic32_t num_mc;		// count of MC objects
	ofi_atomic32_t join_cnt;	// advanced on every join
	size_t min_multi_recv;		// trigger value to rotate bufs
	size_t buffer_size;		// size of receive buffers
	size_t buffer_count;		// count of receive buffers
	bool join_busy;			// serialize joins on a node
	bool is_hwroot;			// set if ep is hw_root
	bool enabled;			// enabled
};

/* Receive context state machine.
 * TODO: Handle unexpected RMA.
 */
enum cxip_rxc_state {
	/* Initial state of an RXC. All user posted receives are rejected until
	 * the RXC has been enabled.
	 *
	 * Note that an RXC can be transitioned from any state into
	 * RXC_DISABLED.
	 *
	 * Validate state changes:
	 * RXC_ENABLED: User has successfully enabled the RXC.
	 * RXC_ENABLED_SOFTWARE: User has successfully initialized the RXC
	 * in a software only RX matching mode.
	 */
	RXC_DISABLED = 0,

	/* User posted receives are matched against the software unexpected
	 * list before being offloaded to hardware. Hardware matches against
	 * the corresponding PtlTE priority and overflow list.
	 *
	 * Validate state changes:
	 * RXC_ONLOAD_FLOW_CONTROL: Several scenarios can initiate this state
	 * change.
	 *    1. Hardware fails to allocate an LE for an unexpected message
	 *    or a priority list LE append fails, and hybrid mode is not
	 *    enabled. Hardware transitions the PtlTE from enabled to disabled.
	 *    2. Hardware fails to allocate an LE during an overflow list
	 *    append. The PtlTE remains in the enabled state but appends to
	 *    the overflow list are disabled. Software manually disables
	 *    the PtlTE.
	 *    3. Hardware fails to successfully match on the overflow list.
	 *    Hardware automatically transitions the PtlTE from enabled to
	 *    disabled.
	 * RXC_ONLOAD_FLOW_CONTROL_REENABLE: Several scenarios can initiate
	 * it this state change:
	 *    1. The hardware EQ is full, hardware transitions the PtlTE from
	 *    enabled/software managed to disabled to recover drops, but it
	 *    can re-enable if an LE resource is not recovered.
	 *    2. Running "hardware" RX match mode and matching failed because
	 *    the overflow list buffers were full. Hardware transitions the
	 *    PtlTE from enabled to disabled. The overflow list must be
	 *    replenished and processing can continue if an LE resource is not
	 *    recovered.
	 *    3. Running "hybrid" or "software" RX match mode and a message
	 *    is received, but there is not a buffer available on the request
	 *    list. Hardware transitions the PtlTE from software managed to
	 *    disabled. The request list must be replenished and processing
	 *    can continue if an LE resource is not recovered.
	 * RXC_PENDING_PTLTE_SOFTWARE_MANAGED: When the provider is configured
	 * to run in "hybrid" RX match mode and hardware fails to allocate an
	 * LE for an unexpected message match or an priority list append fails.
	 * Hardware will automatically transition the PtlTE from enabled to
	 * software managed and onload of UX messages will be initiated.
	 */
	RXC_ENABLED,

	/* The NIC has initiated a transition to software managed EP matching.
	 *
	 * Software must onload/reonload the hardware unexpected list while
	 * creating a pending unexpected list from entries received on the PtlTE
	 * request list. Any in flight appends will fail and be added to
	 * a receive replay list, further attempts to post receive operations
	 * will return -FI_EAGAIN. When onloading completes, the pending
	 * UX list is appended to the onloaded UX list and then failed appends
	 * are replayed prior to enabling the posting of receive operations.
	 *
	 * Validate state changes:
	 * RXC_ENABLED_SOFTWARE: The HW to SW transition onloading has
	 * completed and the onloaded and pending request UX list have been
	 * combined.
	 */
	RXC_PENDING_PTLTE_SOFTWARE_MANAGED,

	/* Executing as a software managed PtlTE either due to hybrid
	 * transition from hardware or initial startup in software
	 * RX matching mode.
	 *
	 * Validate state changes:
	 * RXC_PENDING_PTLTE_HARDWARE: TODO: When able, software may
	 * initiate a transition from software managed mode back to
	 * fully offloaded operation.
	 * RXC_ONLODAD_FLOW_CONTROL_REENABLE: Hardware was unable to match
	 * on the request list or the EQ is full. Hardware has disabled the
	 * PtlTE initiating flow control. Operation can continue if LE
	 * resources are not recovered as long as request buffers can be
	 * replenished.
	 */
	RXC_ENABLED_SOFTWARE,

	/* TODO: Hybrid RX match mode PtlTE is transitioning from software
	 * managed operation back to fully offloaded operation.
	 *
	 * Validate state changes:
	 * RXC_ENABLED: Hybrid software managed PtlTE successfully
	 * transitions back to fully offloaded operation.
	 * RXC_ENABLED_SOFTWARE: Hybrid software managed PtlTE was
	 * not able to transition to fully offloaded operation.
	 */
	RXC_PENDING_PTLTE_HARDWARE,

	/* Software has encountered a condition which requires manual transition
	 * of the PtlTE into disable. This state change occurs when a posted
	 * receive could not be appended due to LE exhaustion and software
	 * managed EP PtlTE operation has been disabled or is not possible.
	 *
	 * Validate state changes:
	 * RXC_ONLOAD_FLOW_CONTROL: PtlTE disabled event has successfully been
	 * received and onloading can begin.
	 */
	RXC_PENDING_PTLTE_DISABLE,

	/* Flow control has occurred and the PtlTE is disabled. Software is
	 * in the process of onloading the hardware unexpected headers to free
	 * up LEs. User posted receives are matched against the software
	 * unexpected list. If a match is not found on the software unexpected
	 * list, -FI_EAGAIN is returned to the user. Hardware matching is
	 * disabled.
	 *
	 * Validate state changes:
	 * RXC_ONLOAD_FLOW_CONTROL_REENABLE: An unexpected list entry matched
	 * a user posted receive, the search and delete command free a
	 * unexpected list entry, or a transition to software managed EP is
	 * occuring.
	 */
	RXC_ONLOAD_FLOW_CONTROL,

	/* PtlTE is in the same state as RXC_ONLOAD_FLOW_CONTROL, but the RXC
	 * should attempt to be re-enabled.
	 *
	 * Validate state changes:
	 * RXC_FLOW_CONTROL: Onloading of the unexpected headers has completed.
	 */
	RXC_ONLOAD_FLOW_CONTROL_REENABLE,

	/* Software is performing sideband communication to recover the dropped
	 * messages. User posted receives are matched against the software
	 * unexpected list. If a match is not found on the software unexpected
	 * list, -FI_EAGAIN is returned to the user. Hardware matching is
	 * disabled.
	 *
	 * If an append fails due to RC_NO_SPACE while in the RXC_FLOW_CONTROL
	 * state, hardware LEs are exhausted and no more LEs can be freed by
	 * onloading unexpected headers into software. This is a fatal event
	 * which requires software endpoint mode to workaround.
	 *
	 * Validate state changes:
	 * RXC_ENABLED: Sideband communication is complete and PtlTE is
	 * successfully re-enabled.
	 * RXC_SOFTWARE_MANAGED: When executing in "hybrid" or "software"
	 * RX match mode and processing has requested to re-enable as a
	 * software managed EP.
	 */
	RXC_FLOW_CONTROL,
};

#define CXIP_COUNTER_BUCKETS 31U
#define CXIP_BUCKET_MAX (CXIP_COUNTER_BUCKETS - 1)
#define CXIP_LIST_COUNTS 3U

struct cxip_msg_counters {
	/* Histogram counting the number of messages based on priority, buffer
	 * type (HMEM), and message size.
	 */
	ofi_atomic32_t msg_count[CXIP_LIST_COUNTS][OFI_HMEM_MAX][CXIP_COUNTER_BUCKETS];
};

/* Returns the most significant bit set (indexed from 1 - the LSB) */
static inline int fls64(uint64_t x)
{
	if (!x)
		return 0;

	return (sizeof(x) * 8) - __builtin_clzl(x);
}

static inline void cxip_msg_counters_init(struct cxip_msg_counters *cntrs)
{
	int i;
	int j;
	int k;

	for (i = 0; i < CXIP_LIST_COUNTS; i++) {
		for (j = 0; j < OFI_HMEM_MAX; j++) {
			for (k = 0; k < CXIP_COUNTER_BUCKETS; k++)
				ofi_atomic_initialize32(&cntrs->msg_count[i][j][k], 0);
		}
	}
}

static inline void
cxip_msg_counters_msg_record(struct cxip_msg_counters *cntrs,
			     enum c_ptl_list list, enum fi_hmem_iface buf_type,
			     size_t msg_size)
{
	unsigned int bucket;

	/* Buckets to bytes
	 * Bucket 0: 0 bytes
	 * Bucket 1: 1 byte
	 * Bucket 2: 2 bytes
	 * Bucket 3: 4 bytes
	 * ...
	 * Bucket CXIP_BUCKET_MAX: (1 << (CXIP_BUCKET_MAX - 1))
	 */

	/* Round size up to the nearest power of 2. */
	bucket = fls64(msg_size);
	if ((1ULL << bucket) < msg_size)
		bucket++;

	bucket = MIN(CXIP_BUCKET_MAX, bucket);

	ofi_atomic_add32(&cntrs->msg_count[list][buf_type][bucket], 1);
}

/*
 * The default for the number of SW initiated TX operation that may
 * be initiated by RX processing and be outstanding. This has no
 * impact on hardware initiated rendezvous gets. This value can be
 * adjusted if necessary with FI_CXI_SW_RX_TX_INIT_MAX=#.
 */
#define CXIP_SW_RX_TX_INIT_MAX_DEFAULT	1024
#define CXIP_SW_RX_TX_INIT_MIN		64

/* If a restricted rendezvous protocol notify done message
 * cannot be delivered due to EQ full, delay before retrying.
 */
#define CXIP_DONE_NOTIFY_RETRY_DELAY_US 100

#define RXC_RESERVED_FC_SLOTS 1

/* RXC specialization API support */
struct cxip_rxc_ops {
	ssize_t (*recv_common)(struct cxip_rxc *rxc, void *buf, size_t len,
			       void *desc, fi_addr_t src_add, uint64_t tag,
			       uint64_t ignore, void *context, uint64_t flags,
			       bool tagged, struct cxip_cntr *comp_cntr);
	void (*progress)(struct cxip_rxc *rxc);
	void (*recv_req_tgt_event)(struct cxip_req *req,
				   const union c_event *event);
	int (*cancel_msg_recv)(struct cxip_req *req);
	int (*ctrl_msg_cb)(struct cxip_ctrl_req *req,
			   const union c_event *event);
	void (*init_struct)(struct cxip_rxc *rxc, struct cxip_ep_obj *ep_obj);
	void (*fini_struct)(struct cxip_rxc *rxc);
	void (*cleanup)(struct cxip_rxc *rxc);
	int (*msg_init)(struct cxip_rxc *rxc);
	int (*msg_fini)(struct cxip_rxc *rxc);
};

/*
 * Receive context base object
 */
struct cxip_rxc {
	void *context;
	uint32_t protocol;

	struct fi_rx_attr attr;
	bool selective_completion;
	bool hmem;
	bool trunc_ok;
	bool sw_ep_only;
	bool msg_offload;
	uint8_t pid_bits;		// Zero without SEP
	uint8_t recv_ptl_idx;

	enum cxip_rxc_state state;

	/* Reverse link to EP object that owns this context */
	struct cxip_ep_obj *ep_obj;

	struct cxip_cq *recv_cq;
	struct cxip_cntr *recv_cntr;

	struct cxip_rxc_ops ops;

	struct cxip_domain *domain;

	/* RXC receive portal table, event queue and hardware
	 * command queue.
	 */
	struct cxip_evtq rx_evtq;
	struct cxip_pte *rx_pte;
	struct cxip_cmdq *rx_cmdq;
	ofi_atomic32_t orx_reqs;

	/* If FI_MULTI_RECV is supported, minimum receive size required
	 * for buffers posted.
	 */
	size_t min_multi_recv;

	/* If TX events are required by specialization, the maximum
	 * credits that can be used.
	 */
	int32_t max_tx;
	unsigned int recv_appends;

	struct cxip_msg_counters cntrs;
};

/* Receive context specialization for supporting HPC messaging
 * that requires SAS implemented in a Portals environment.
 */
struct cxip_rxc_hpc {
	/* Must be first */
	struct cxip_rxc base;

	int max_eager_size;
	uint64_t rget_align_mask;

	/* Window when FI_CLAIM mutual exclusive access is required */
	bool hw_claim_in_progress;

	int sw_ux_list_len;
	int sw_pending_ux_list_len;

	/* Number of unexpected list entries in HW. */
	ofi_atomic32_t orx_hw_ule_cnt;

	/* RX context transmit queue, required for rendezvous
	 * gets.
	 */
	struct cxip_cmdq *tx_cmdq;
	ofi_atomic32_t orx_tx_reqs;

	/* Software receive queue. User posted requests are queued here instead
	 * of on hardware if the RXC is in software endpoint mode.
	 */
	struct dlist_entry sw_recv_queue;

	/* Defer events to wait for both put and put overflow */
	struct def_event_ht deferred_events;

	/* Unexpected message handling */
	struct cxip_ptelist_bufpool *req_list_bufpool;
	struct cxip_ptelist_bufpool *oflow_list_bufpool;

	enum cxip_rxc_state prev_state;
	enum cxip_rxc_state new_state;
	enum c_sc_reason fc_reason;

	/* RXC drop count used for FC accounting. */
	int drop_count;

	/* Array of 8-byte of unexpected headers remote offsets. */
	uint64_t *ule_offsets;
	unsigned int num_ule_offsets;

	/* Current remote offset to be processed. Incremented after processing
	 * a search and delete put event.
	 */
	unsigned int cur_ule_offsets;

	struct dlist_entry fc_drops;
	struct dlist_entry replay_queue;
	struct dlist_entry sw_ux_list;
	struct dlist_entry sw_pending_ux_list;

	/* Flow control/software state change metrics */
	int num_fc_eq_full;
	int num_fc_no_match;
	int num_fc_unexp;
	int num_fc_append_fail;
	int num_fc_req_full;
	int num_sc_nic_hw2sw_append_fail;
	int num_sc_nic_hw2sw_unexp;
};

/* Receive context specialization for supporting client/server
 * messaging.
 */
struct cxip_rxc_rnr {
	/* Must be first */
	struct cxip_rxc base;

	bool hybrid_mr_desc;
	/* Used when success events are not required */
	struct cxip_req *req_selective_comp_msg;
	struct cxip_req *req_selective_comp_tag;
};

static inline void cxip_copy_to_md(struct cxip_md *md, void *dest,
				   const void *src, size_t size)
{
	ssize_t ret __attribute__((unused));
	struct iovec iov;

	/* Favor CPU store access instead of relying on HMEM copy functions. */
	if (md->handle_valid && size <= cxip_env.safe_devmem_copy_threshold) {
		ret = ofi_hmem_dev_reg_copy_to_hmem(md->info.iface, md->handle,
						    dest, src, size);
		assert(ret == FI_SUCCESS);
	} else {
		iov.iov_base = dest;
		iov.iov_len = size;

		ret = md->dom->hmem_ops.copy_to_hmem_iov(md->info.iface,
							 md->info.device, &iov,
							 1, 0, src, size);
		assert(ret == size);
	}
}

static inline void cxip_copy_from_md(struct cxip_md *md, void *dest,
				     const void *src, size_t size)
{
	ssize_t ret __attribute__((unused));
	struct iovec iov;

	/* Favor CPU store access instead of relying on HMEM copy functions. */
	if (md->handle_valid && size <= cxip_env.safe_devmem_copy_threshold) {
		ret = ofi_hmem_dev_reg_copy_from_hmem(md->info.iface,
						      md->handle,
						      dest, src, size);
		assert(ret == FI_SUCCESS);
	} else {
		iov.iov_base = (void *)src;
		iov.iov_len = size;


		ret = md->dom->hmem_ops.copy_from_hmem_iov(dest, size,
							   md->info.iface,
							   md->info.device,
							   &iov, 1, 0);
		assert(ret == size);
	}
}

/* PtlTE buffer pool - Common PtlTE request/overflow list buffer
 * management.
 *
 * Only C_PTL_LIST_REQUEST and C_PTL_LIST_OVERFLOW are supported.
 */
struct cxip_ptelist_bufpool_attr {
	enum c_ptl_list list_type;

	/* Callback to handle PtlTE link error/unlink events */
	int (*ptelist_cb)(struct cxip_req *req, const union c_event *event);
	size_t buf_size;
	size_t min_space_avail;
	size_t min_posted;
	size_t max_posted;
	size_t max_cached;
};

struct cxip_ptelist_bufpool {
	struct cxip_ptelist_bufpool_attr attr;
	struct cxip_rxc_hpc *rxc;
	size_t buf_alignment;

	/* Ordered list of buffers emitted to hardware */
	struct dlist_entry active_bufs;

	/* List of consumed buffers which cannot be reposted yet
	 * since unexpected entries have not been matched.
	 */
	struct dlist_entry consumed_bufs;

	/* List of available buffers that may be appended to the list.
	 * These could be from a previous append failure or be cached
	 * from previous message processing to avoid map/unmap of
	 * list buffer.
	 */
	struct dlist_entry free_bufs;

	ofi_atomic32_t bufs_linked;
	ofi_atomic32_t bufs_allocated;
	ofi_atomic32_t bufs_free;
};

struct cxip_ptelist_req {
	/* Pending list of unexpected header entries which could not be placed
	 * on the RX context unexpected header list due to put events being
	 * received out-of-order.
	 */
	struct dlist_entry pending_ux_list;
};

struct cxip_ptelist_buf {
	struct cxip_ptelist_bufpool *pool;

	/* RX context the request buffer is posted on. */
	struct cxip_rxc_hpc *rxc;
	enum cxip_le_type le_type;
	struct dlist_entry buf_entry;
	struct cxip_req *req;

	/* Memory mapping of req_buf field. */
	struct cxip_md *md;

	/* The number of bytes consume by hardware when the request buffer was
	 * unlinked.
	 */
	size_t unlink_length;

	/* Current offset into the buffer where packets/data are landing. When
	 * the cur_offset is equal to unlink_length, software has completed
	 * event processing for the buffer.
	 */
	size_t cur_offset;

	/* Request list specific control information */
	struct cxip_ptelist_req request;

	/* The number of unexpected headers posted placed on the RX context
	 * unexpected header list which have not been matched.
	 */
	ofi_atomic32_t refcount;

	/* Buffer used to land packets. */
	char *data;
};

int cxip_ptelist_bufpool_init(struct cxip_rxc_hpc *rxc,
			      struct cxip_ptelist_bufpool **pool,
			      struct cxip_ptelist_bufpool_attr *attr);
void cxip_ptelist_bufpool_fini(struct cxip_ptelist_bufpool *pool);
int cxip_ptelist_buf_replenish(struct cxip_ptelist_bufpool *pool,
			       bool seq_restart);
void cxip_ptelist_buf_link_err(struct cxip_ptelist_buf *buf,
			       int rc_link_error);
void cxip_ptelist_buf_unlink(struct cxip_ptelist_buf *buf);
void cxip_ptelist_buf_put(struct cxip_ptelist_buf *buf, bool repost);
void cxip_ptelist_buf_get(struct cxip_ptelist_buf *buf);
void cxip_ptelist_buf_consumed(struct cxip_ptelist_buf *buf);

/*
 * cxip_req_bufpool_init() - Initialize PtlTE request list buffer management
 * object.
 */
int cxip_req_bufpool_init(struct cxip_rxc_hpc *rxc);
void cxip_req_bufpool_fini(struct cxip_rxc_hpc *rxc);

/*
 * cxip_oflow_bufpool_init() - Initialize PtlTE overflow list buffer management
 * object.
 */
int cxip_oflow_bufpool_init(struct cxip_rxc_hpc *rxc);
void cxip_oflow_bufpool_fini(struct cxip_rxc_hpc *rxc);

void _cxip_req_buf_ux_free(struct cxip_ux_send *ux, bool repost);
void cxip_req_buf_ux_free(struct cxip_ux_send *ux);

#define CXIP_RDZV_IDS	(1 << CXIP_TOTAL_RDZV_ID_WIDTH)
#define CXIP_RDZV_IDS_MULTI_RECV (1 << CXIP_RDZV_ID_CMD_WIDTH)
#define CXIP_TX_IDS	(1 << CXIP_TX_ID_WIDTH)

/* One per LAC */
#define RDZV_SRC_LES 8U
#define RDZV_NO_MATCH_PTES 8U

/* Base rendezvous PtlTE object */
struct cxip_rdzv_pte {
	struct cxip_txc_hpc *txc;
	struct cxip_pte *pte;

	/* Count of the number of buffers successfully linked on this PtlTE. */
	ofi_atomic32_t le_linked_success_count;

	/* Count of the number of buffers failed to link on this PtlTE. */
	ofi_atomic32_t le_linked_failure_count;
};

/* Matching PtlTE for user generated unrestricted get DMA */
struct cxip_rdzv_match_pte {
	struct cxip_rdzv_pte base_pte;

	/* Request structure used to handle zero byte puts used for match
	 * complete.
	 */
	struct cxip_req *zbp_req;

	/* Request structures used to handle rendezvous source/data transfers.
	 * There is one request structure (and LE) for each LAC.
	 */
	struct cxip_req *src_reqs[RDZV_SRC_LES];
};

/* Matching PtlTE for user generated restricted get DMA. One PtlTE
 * per LAC used.
 */
struct cxip_rdzv_nomatch_pte {
	struct cxip_rdzv_pte base_pte;
	struct cxip_req *le_req;
};

#if ENABLE_DEBUG
/* Defines to force hard to test TXC error path failures;
 * only valid for debug unit testing. See txc->force_err.
 */
#define	CXIP_TXC_FORCE_ERR_ALT_READ_PROTO_ALLOC (1 << 0)
#endif

/* TXC specialization API support */
struct cxip_txc_ops {
	ssize_t (*send_common)(struct cxip_txc *txc, uint32_t tclass,
			       const void *buf, size_t len, void *desc,
			       uint64_t data, fi_addr_t dest_addr, uint64_t tag,
			       void *context, uint64_t flags, bool tagged,
			       bool triggered, uint64_t trig_thresh,
			       struct cxip_cntr *trig_cntr,
			       struct cxip_cntr *comp_cntr);
	void (*progress)(struct cxip_txc *txc);
	int (*cancel_msg_send)(struct cxip_req *req);
	void (*init_struct)(struct cxip_txc *txc, struct cxip_ep_obj *ep_obj);
	void (*fini_struct)(struct cxip_txc *txc);
	void (*cleanup)(struct cxip_txc *txc);
	int (*msg_init)(struct cxip_txc *txc);
	int (*msg_fini)(struct cxip_txc *txc);
};

/*
 * Endpoint object transmit context
 */
struct cxip_txc {
	void *context;

	uint32_t protocol;
	bool enabled;
	bool hrp_war_req;		// Non-fetching 32-bit HRP
	bool hmem;
	bool trunc_ok;

	struct cxip_cq *send_cq;
	struct cxip_cntr *send_cntr;
	struct cxip_cntr *read_cntr;
	struct cxip_cntr *write_cntr;

	struct cxip_txc_ops ops;

	struct cxip_ep_obj *ep_obj;	// parent EP object
	struct cxip_domain *domain;	// parent domain
	uint8_t pid_bits;
	uint8_t recv_ptl_idx;

	struct fi_tx_attr attr;		// attributes
	bool selective_completion;
	uint32_t tclass;

	/* TX H/W Event Queue */
	struct cxip_evtq tx_evtq;

	/* Inject buffers for EP, protected by ep_obj->lock */
	struct ofi_bufpool *ibuf_pool;

	struct cxip_cmdq *tx_cmdq;	// added during cxip_txc_enable()
	ofi_atomic32_t otx_reqs;	// outstanding transmit requests

	/* Queue of TX messages in flight for the context */
	struct dlist_entry msg_queue;

	struct cxip_req *rma_write_selective_completion_req;
	struct cxip_req *rma_read_selective_completion_req;
	struct cxip_req *amo_selective_completion_req;
	struct cxip_req *amo_fetch_selective_completion_req;

	struct dlist_entry dom_entry;
};

/* Default HPC SAS TXC specialization */
struct cxip_txc_hpc {
	/* Must remain first */
	struct cxip_txc base;

	int max_eager_size;
	int rdzv_eager_size;

	/* Rendezvous messaging support */
	struct cxip_rdzv_match_pte *rdzv_pte;
	struct cxip_rdzv_nomatch_pte *rdzv_nomatch_pte[RDZV_NO_MATCH_PTES];
	struct indexer rdzv_ids;
	struct indexer msg_rdzv_ids;
	enum cxip_rdzv_proto rdzv_proto;

	struct cxip_cmdq *rx_cmdq;	// Target cmdq for Rendezvous buffers

#if ENABLE_DEBUG
	uint64_t force_err;
#endif
	/* Flow Control recovery */
	struct dlist_entry fc_peers;

	/* Match complete IDs */
	struct indexer tx_ids;

};

/* Client/server derived TXC, does not support SAS ordering
 * or remotely buffered unexpected messages.
 */
#define CXIP_RNR_TIMEOUT_US	500000
#define CXIP_NUM_RNR_WAIT_QUEUE	5

struct cxip_txc_rnr {
	/* Must remain first */
	struct cxip_txc base;

	uint64_t max_retry_wait_us;	/* Maximum time to retry any request */
	ofi_atomic32_t time_wait_reqs;	/* Number of RNR time wait reqs */
	uint64_t next_retry_wait_us;	/* Time of next retry in all queues */
	uint64_t total_retries;
	uint64_t total_rnr_nacks;
	bool hybrid_mr_desc;

	/* Used when success events are not required */
	struct cxip_req *req_selective_comp_msg;
	struct cxip_req *req_selective_comp_tag;

	/* There are CXIP_NUM_RNR_WAIT_QUEUE queues where each queue has
	 * a specified time wait value and where the last queue is has the
	 * maximum time wait value before retrying (and is used for all
	 * subsequent retries). This implementation allows each queue to
	 * be maintained in retry order with a simple append of the request.
	 */
	struct dlist_entry time_wait_queue[CXIP_NUM_RNR_WAIT_QUEUE];
};

int cxip_txc_emit_idc_put(struct cxip_txc *txc, uint16_t vni,
			  enum cxi_traffic_class tc,
			  enum cxi_traffic_class_type tc_type,
			  const struct c_cstate_cmd *c_state,
			  const struct c_idc_put_cmd *put, const void *buf,
			  size_t len, uint64_t flags);
int cxip_txc_emit_dma(struct cxip_txc *txc, uint16_t vni,
		      enum cxi_traffic_class tc,
		      enum cxi_traffic_class_type tc_type,
		      struct cxip_cntr *trig_cntr, size_t trig_thresh,
		      struct c_full_dma_cmd *dma, uint64_t flags);
int cxip_txc_emit_idc_amo(struct cxip_txc *txc, uint16_t vni,
			  enum cxi_traffic_class tc,
			  enum cxi_traffic_class_type tc_type,
			  const struct c_cstate_cmd *c_state,
			  const struct c_idc_amo_cmd *amo, uint64_t flags,
			  bool fetching, bool flush);
int cxip_txc_emit_dma_amo(struct cxip_txc *txc, uint16_t vni,
			  enum cxi_traffic_class tc,
			  enum cxi_traffic_class_type tc_type,
			  struct cxip_cntr *trig_cntr, size_t trig_thresh,
			  struct c_dma_amo_cmd *amo, uint64_t flags,
			  bool fetching, bool flush);
int cxip_txc_emit_idc_msg(struct cxip_txc *txc, uint16_t vni,
			  enum cxi_traffic_class tc,
			  enum cxi_traffic_class_type tc_type,
			  const struct c_cstate_cmd *c_state,
			  const struct c_idc_msg_hdr *msg, const void *buf,
			  size_t len, uint64_t flags);

void cxip_txc_flush_msg_trig_reqs(struct cxip_txc *txc);

/*
 * Endpoint Control Object
 *
 * Groups control MR and messaging structures that can be exclusively used
 * for a standard EP or globally shared in a SEP by all RX/TX context.
 */
struct cxip_ctrl {
	/* wait object is required to wake up CQ waiters
	 * when control progress is required.
	 */
	struct cxil_wait_obj *wait;

	struct cxi_eq *tgt_evtq;
	struct cxi_eq *tx_evtq;

	/* TX command queue is used to initiate side-band messaging
	 * and is TX credit based.
	 */
	struct cxip_cmdq *txq;
	unsigned int tx_credits;

	/* Target command queue is used for appending RX side-band
	 * messaging control LE and managing standard MR LE.
	 */
	struct cxip_cmdq *tgq;
	struct cxip_pte *pte;
	struct cxip_ctrl_req msg_req;

	/* FI_MR_PROV_KEY caching, protected with ep_obj->lock */
	struct cxip_mr_lac_cache std_mr_cache[CXIP_NUM_CACHED_KEY_LE];
	struct cxip_mr_lac_cache opt_mr_cache[CXIP_NUM_CACHED_KEY_LE];

	struct dlist_entry mr_list;

	/* Event queue buffers */
	void *tgt_evtq_buf;
	struct cxi_md *tgt_evtq_buf_md;
	void *tx_evtq_buf;
	struct cxi_md *tx_evtq_buf_md;
};

/*
 * Base Endpoint Object
 *
 * Support structure, libfabric fi_endpoint implementation.
 *
 * This is the meat of the endpoint object. It has been separated from cxip_ep
 * to support aliasing.
 */
struct cxip_ep_obj {
	/* Allow lock to be optimized out with FI_THREAD_DOMAIN */
	struct ofi_genlock lock;
	struct cxip_domain *domain;
	struct cxip_av *av;

	/* Domain has been configured with FI_AV_AUTH_KEY. */
	bool av_auth_key;

	/* This is only valid if FI_AV_AUTH_KEY is false. */
	struct cxi_auth_key auth_key;

	/* Array of VNIs if FI_AV_AUTH_KEY is true. */
	uint16_t *vnis;
	size_t vni_count;

	struct cxip_addr src_addr;
	fi_addr_t fi_addr;

	bool enabled;

	/* Endpoint protocol implementations.
	 * FI_PROTO_CXI - Portals SAS protocol
	 */
	uint32_t protocol;
	struct cxip_txc *txc;
	struct cxip_rxc *rxc;

	/* ASIC version associated with EP/Domain */
	enum cassini_version asic_ver;

	/* Information that might be owned by an EP (or a SEP
	 * when implemented). Should ultimately be a pointer
	 * to a base/specialization.
	 */
	struct cxip_ctrl ctrl;

	/* Command queues. Each EP has 1 transmit and 1 target
	 * command queue that can be shared. An optional 2nd transmit
	 * command queue may be created for RX initiated rgets.
	 */
	struct cxip_cmdq *txq;
	ofi_atomic32_t txq_ref;
	struct cxip_cmdq *tgq;
	ofi_atomic32_t tgq_ref;
	struct cxip_cmdq *rx_txq;

	/* Libfabric software EQ resource */
	struct cxip_eq *eq;
	struct dlist_entry eq_link;

	/* Values at base EP creation */
	uint64_t caps;
	struct fi_ep_attr ep_attr;
	struct fi_tx_attr tx_attr;
	struct fi_rx_attr rx_attr;

	/* Collectives support */
	struct cxip_ep_coll_obj coll;
	struct cxip_ep_zbcoll_obj zbcoll;

	size_t txq_size;
	size_t tgq_size;
	ofi_atomic32_t ref;
	struct cxip_portals_table *ptable;
};

/*
 * CXI endpoint implementations to support FI_CLASS_EP.
 */
struct cxip_ep {
	struct fid_ep ep;
	struct fi_tx_attr tx_attr;
	struct fi_rx_attr rx_attr;
	struct cxip_ep_obj *ep_obj;
	int is_alias;
};

size_t cxip_ep_get_unexp_msgs(struct fid_ep *fid_ep,
			      struct fi_cq_tagged_entry *entry, size_t count,
			      fi_addr_t *src_addr, size_t *ux_count);
int cxip_build_ux_entry_info(struct cxip_ep *ep,
			     struct fi_cq_tagged_entry *entry, size_t count,
			     fi_addr_t *src_addr, size_t *ux_count);

enum cxip_mr_state {
	CXIP_MR_DISABLED = 1,
	CXIP_MR_ENABLED,
	CXIP_MR_LINKED,
	CXIP_MR_UNLINKED,
	CXIP_MR_LINK_ERR,
};

/*
 * Memory Region
 *
 * libfabric fi_mr implementation.
 *
 * Created in cxip_regattr().
 */
struct cxip_mr {
	struct fid_mr mr_fid;
	struct cxip_domain *domain;	// parent domain
	struct cxip_ep *ep;		// endpoint for remote memory
	uint64_t key;			// memory key
	uint64_t flags;			// special flags
	struct fi_mr_attr attr;		// attributes
	struct cxip_cntr *cntr;		// if bound to cntr

	/* Indicates if FI_RMA_EVENT was specified at creation and
	 * will be used to enable fi_writedata() and fi_inject_writedata()
	 * support for this MR (TODO).
	 */
	bool rma_events;

	/* If requested then count MR events to determine if RMA are in
	 * progress. At close if no RMA are in progress bypass the invalidate
	 * of the PTLTE LE. This improves non-cached key close performance,
	 * enabling their use so that after closing the MR the associated
	 * memory cannot be remotely accessed, even if it remains in the
	 * libfabric MR cache.
	 */
	bool count_events;
	ofi_atomic32_t  match_events;
	ofi_atomic32_t  access_events;

	ofi_spin_t lock;

	struct cxip_mr_util_ops *mr_util;
	bool enabled;
	struct cxip_pte *pte;
	enum cxip_mr_state mr_state;
	int64_t mr_id;			// Non-cached provider key uniqueness
	struct cxip_ctrl_req req;
	bool optimized;

	void *buf;			// memory buffer VA
	uint64_t len;			// memory length
	struct cxip_md *md;		// buffer IO descriptor
	struct dlist_entry ep_entry;

	struct dlist_entry mr_domain_entry;
};

struct cxip_av_auth_key_entry {
	ofi_atomic32_t use_cnt;
	ofi_atomic32_t ref_cnt;
	UT_hash_handle hh;
	struct dlist_entry entry;
	struct cxi_auth_key key;
	fi_addr_t fi_addr;
};

struct cxip_av_entry {
	ofi_atomic32_t use_cnt;
	UT_hash_handle hh;
	struct cxip_addr addr;
	fi_addr_t fi_addr;
	struct cxip_av_auth_key_entry *auth_key;
};

struct cxip_av {
	struct fid_av av_fid;
	struct cxip_domain *domain;

	/* List of endpoints bound to this AV. Each bind takes a reference
	 * as well.
	 */
	struct dlist_entry ep_list;
	ofi_atomic32_t ref;

	/* Memory used to implement lookups. Two data structures are used.
	 * 1. ibuf pool for O(1) lookup on the data path
	 * 2. hash table for O(1) on the receive path
	 */
	struct cxip_av_entry *av_entry_hash;
	struct ofi_bufpool *av_entry_pool;
	ofi_atomic32_t av_entry_cnt;

	/* Memory used to support AV authorization key. Three data structures
	 * are needed.
	 * 1. ibuf pool for memory allocation and lookup O(1) access.
	 * 2. hash table for O(1) reverse lookup
	 * 3. List for iterating
	 */
	struct cxip_av_auth_key_entry *auth_key_entry_hash;
	struct ofi_bufpool *auth_key_entry_pool;
	struct dlist_entry auth_key_entry_list;
	ofi_atomic32_t auth_key_entry_cnt;
	size_t auth_key_entry_max;

	/* Single lock is used to protect entire AV. With domain level
	 * threading, this lock is not used.
	 */
	bool lockless;
	pthread_rwlock_t lock;

	/* AV is configured as symmetric. This is an optimization which enables
	 * endpoints to use logical address.
	 */
	bool symmetric;

	/* Address vector type. */
	enum fi_av_type type;

	/* Whether or not the AV is operating in FI_AV_AUTH_KEY mode. */
	bool av_auth_key;

	/* Whether or not the AV was opened with FI_AV_USER_ID. */
	bool av_user_id;
};

int cxip_av_auth_key_get_vnis(struct cxip_av *av, uint16_t **vni,
			      size_t *vni_count);
void cxip_av_auth_key_put_vnis(struct cxip_av *av, uint16_t *vni,
			       size_t vni_count);
extern struct cxip_addr *(*cxip_av_addr_in)(const void *addr);
extern void (*cxip_av_addr_out)(struct cxip_addr *addr_out,
				struct cxip_addr *addr);
int cxip_av_lookup_addr(struct cxip_av *av, fi_addr_t fi_addr,
			struct cxip_addr *addr);
fi_addr_t cxip_av_lookup_fi_addr(struct cxip_av *av,
				 const struct cxip_addr *addr);
fi_addr_t cxip_av_lookup_auth_key_fi_addr(struct cxip_av *av, unsigned int vni);
int cxip_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		 struct fid_av **av, void *context);
int cxip_av_bind_ep(struct cxip_av *av, struct cxip_ep *ep);
void cxip_av_unbind_ep(struct cxip_av *av, struct cxip_ep *ep);
static inline int cxip_av_entry_count(struct cxip_av *av)
{
	return ofi_atomic_get32(&av->av_entry_cnt);
}

/*
 * AV Set
 *
 * libfabric fi_av_set implementation.
 *
 * Created in cxip_av_set().
 */
struct cxip_av_set {
	struct fid_av_set av_set_fid;
	struct cxip_av *cxi_av;		// associated AV
	struct cxip_coll_mc *mc_obj;	// reference MC
	fi_addr_t *fi_addr_ary;		// addresses in set
	size_t fi_addr_cnt;		// count of addresses
	struct cxip_comm_key comm_key;	// communication key
	uint64_t flags;
};

/* Needed for math functions */
union cxip_dbl_bits {
	struct {
		uint64_t mantissa:52;
		uint64_t exponent:11;
		uint64_t sign:1;
	} __attribute__((__packed__));
	double dval;
	uint64_t ival;
};

static inline uint64_t _dbl2bits(double d)
{
#if (BYTE_ORDER == LITTLE_ENDIAN)
	union cxip_dbl_bits x = {.dval = d};
	return x.ival;
#else
#error "Unsupported processor byte ordering"
#endif
}

static inline double _bits2dbl(uint64_t i)
{
#if (BYTE_ORDER == LITTLE_ENDIAN)
	union cxip_dbl_bits x = {.ival = i};
	return x.dval;
#else
#error "Unsupported processor byte ordering"
#endif
}

static inline void _decompose_dbl(double d, int *sgn, int *exp,
				  unsigned long *man)
{
#if (BYTE_ORDER == LITTLE_ENDIAN)
	union cxip_dbl_bits x = {.dval = d};
	*sgn = (x.sign) ? -1 : 1;
	*exp = x.exponent;
	*man = x.mantissa;
#else
#error "Unsupported processor byte ordering"
#endif
}

/* data structures for reduction support */
enum cxip_coll_redtype {
	REDTYPE_BYT,
	REDTYPE_INT,
	REDTYPE_FLT,
	REDTYPE_IMINMAX,
	REDTYPE_FMINMAX,
	REDTYPE_REPSUM
};

/* int AND, OR, XOR, MIN, MAX, SUM */
struct cxip_intval {
	int64_t ival[4];
};

/* flt MIN, MAX, SUM */
struct cxip_fltval {
	double fval[4];
};

/* int MINMAXLOC */
struct cxip_iminmax {
	int64_t iminval;
	uint64_t iminidx;
	int64_t imaxval;
	uint64_t imaxidx;
};

/* flt MINMAXLOC */
struct cxip_fltminmax {
	double fminval;
	uint64_t fminidx;
	double fmaxval;
	uint64_t fmaxidx;
};

/* repsum SUM */
struct cxip_repsum {
	int64_t T[4];
	int32_t M;
	int8_t overflow_id;
	bool inexact;
	bool overflow;
	bool invalid;
};

/* Collective operation states */
enum cxip_coll_state {
	CXIP_COLL_STATE_NONE,
	CXIP_COLL_STATE_READY,
	CXIP_COLL_STATE_FAULT,
};

/* Similar to C_RC_* provider errors, but pure libfabric */
/* These should be in priority order, from lowest to highest */
enum cxip_coll_prov_errno {
	CXIP_PROV_ERRNO_OK = -1,		// good
	CXIP_PROV_ERRNO_PTE = -2,		// PTE setup failure
	CXIP_PROV_ERRNO_MCAST_INUSE = -3,	// multicast in-use
	CXIP_PROV_ERRNO_HWROOT_INUSE = -4,	// hwroot in-use
	CXIP_PROV_ERRNO_MCAST_INVALID = -5,	// multicast invalid
	CXIP_PROV_ERRNO_HWROOT_INVALID = -6,	// hwroot invalid
	CXIP_PROV_ERRNO_CURL = -7,		// CURL failure
	CXIP_PROV_ERRNO_LAST = -8,		// last error code (unused)
};

/* Rosetta reduction engine error codes */
typedef enum cxip_coll_rc {
	CXIP_COLL_RC_SUCCESS = 0,		// good
	CXIP_COLL_RC_FLT_INEXACT = 1,		// result was rounded
	CXIP_COLL_RC_FLT_OVERFLOW = 3,		// result too large to represent
	CXIP_COLL_RC_FLT_INVALID = 4,           // operand was signalling NaN,
						//   or infinities subtracted
	CXIP_COLL_RC_REP_INEXACT = 5,		// reproducible sum was rounded
	CXIP_COLL_RC_INT_OVERFLOW = 6,		// reproducible sum overflow
	CXIP_COLL_RC_CONTR_OVERFLOW = 7,	// too many contributions seen
	CXIP_COLL_RC_OP_MISMATCH = 8,		// conflicting opcodes
	CXIP_COLL_RC_TX_FAILURE = 9,		// internal send error
	CXIP_COLL_RC_MAX = 10
} cxip_coll_rc_t;

struct cxip_coll_buf {
	struct dlist_entry buf_entry;		// linked list of buffers
	struct cxip_req *req;			// associated LINK request
	struct cxip_md *cxi_md;			// buffer memory descriptor
	size_t bufsiz;				// buffer size in bytes
	uint8_t buffer[];			// buffer space itself
};

struct cxip_coll_pte {
	struct cxip_pte *pte;			// Collectives PTE
	struct cxip_ep_obj *ep_obj;		// Associated endpoint
	struct cxip_coll_mc *mc_obj;		// Associated multicast object
	struct dlist_entry buf_list;		// PTE receive buffers
	ofi_atomic32_t buf_cnt;			// count of linked buffers
	ofi_atomic32_t buf_swap_cnt;		// for diagnostics
	ofi_atomic32_t recv_cnt;		// for diagnostics
	int buf_low_water;			// for diagnostics
	bool enabled;				// enabled
};

/* REQUIRED:
 * sizeof(struct cxip_coll_accumulator) >= sizeof(struct cxip_coll_data)
 * (opaque) struct cxip_coll_accumulator exported in fi_cxi_ext.h
 */
struct cxip_coll_data {
	union {
		uint8_t databuf[32];		// raw data buffer
		struct cxip_intval intval;	// 4 integer values + flags
		struct cxip_fltval fltval;	// 4 double values + flags
		struct cxip_iminmax intminmax;	// 1 intminmax structure + flags
		struct cxip_fltminmax fltminmax;// 1 fltminmax structure + flags
		struct cxip_repsum repsum;	// 1 repsum structure + flags
	};
	cxip_coll_op_t red_op;			// reduction opcode
	cxip_coll_rc_t red_rc;			// reduction return code
	int red_cnt;				// reduction contrib count
	bool initialized;
};

struct cxip_coll_reduction {
	struct cxip_coll_mc *mc_obj;		// parent mc_obj
	uint32_t red_id;			// reduction id
	uint16_t seqno;				// reduction sequence number
	uint16_t resno;				// reduction result number
	struct cxip_req *op_inject_req;		// active operation request
	enum cxip_coll_state coll_state;	// reduction state on node
	struct cxip_coll_data accum;		// reduction accumulator
	void *op_rslt_data;			// user recv buffer (or NULL)
	int op_data_bytcnt;			// bytes in send/recv buffers
	void *op_context;			// caller's context
	bool in_use;				// reduction is in-use
	bool pktsent;				// reduction packet sent
	bool completed;				// reduction is completed
	bool drop_send;				// drop the next send operation
	bool drop_recv;				// drop the next recv operation
	enum cxip_coll_rc red_rc;		// set by first error
	struct timespec tv_expires;		// reduction expiration time
	uint8_t tx_msg[64];			// static packet memory
};

struct cxip_coll_mc {
	struct fid_mc mc_fid;
	struct dlist_entry entry;		// Link to mc object list
	struct cxip_ep_obj *ep_obj;		// Associated endpoint
	struct cxip_av_set *av_set_obj;		// associated AV set
	struct cxip_zbcoll_obj *zb;		// zb object for zbcol
	struct cxip_coll_pte *coll_pte;		// collective PTE
	struct timespec timeout;		// state machine timeout
	fi_addr_t mynode_fiaddr;		// fi_addr of this node
	int mynode_idx;				// av_set index of this node
	uint32_t hwroot_idx;			// av_set index of hwroot node
	uint32_t mcast_addr;			// multicast target address
	int tail_red_id;			// tail active red_id
	int next_red_id;			// next available red_id
	int max_red_id;				// limit total concurrency
	int seqno;				// rolling seqno for packets
	bool is_multicast;			// true if multicast address
	bool arm_disable;			// arm-disable for testing
	bool retry_disable;			// retry-disable for testing
	bool is_joined;				// true if joined
	bool rx_discard;			// true to discard RX events
	enum cxi_traffic_class tc;		// traffic class
	enum cxi_traffic_class_type tc_type;	// traffic class type
	ofi_atomic32_t send_cnt;		// for diagnostics
	ofi_atomic32_t recv_cnt;		// for diagnostics
	ofi_atomic32_t pkt_cnt;			// for diagnostics
	ofi_atomic32_t seq_err_cnt;		// for diagnostics
	ofi_atomic32_t tmout_cnt;		// for diagnostics
	ofi_spin_t lock;

	struct cxi_md *reduction_md;		// memory descriptor for DMA
	struct cxip_coll_reduction reduction[CXIP_COLL_MAX_CONCUR];
};

struct cxip_curl_handle;

typedef void (*curlcomplete_t)(struct cxip_curl_handle *);

struct cxip_curl_handle {
	long status;		// HTTP status, 0 for no server, -1 busy
	const char *endpoint;	// HTTP server endpoint address
	const char *request;	// HTTP request data
	const char *response;	// HTTP response data, NULL until complete
	curlcomplete_t usrfunc;	// user completion function
	void *usrptr;		// user function argument
	void *recv;		// opaque
	void *headers;		// opaque
};

/* Low-level CURL POST/DELETE async wrappers */
enum curl_ops {
	CURL_GET,
	CURL_PUT,
	CURL_POST,
	CURL_PATCH,
	CURL_DELETE,
	CURL_MAX
};
int cxip_curl_init(void);
void cxip_curl_fini(void);
const char *cxip_curl_opname(enum curl_ops op);
int cxip_curl_perform(const char *endpoint, const char *request,
		      const char *sessionToken, size_t rsp_init_size,
		      enum curl_ops op, bool verbose,
		      curlcomplete_t usrfunc, void *usrptr);
int cxip_curl_progress(struct cxip_curl_handle **handleptr);
void cxip_curl_free(struct cxip_curl_handle *handle);

static inline void single_to_double_quote(char *str)
{
	do {if (*str == '\'') *str = '"';} while (*(++str));
}
enum json_type cxip_json_obj(const char *desc, struct json_object *jobj,
			     struct json_object **jval);
int cxip_json_bool(const char *desc, struct json_object *jobj, bool *val);
int cxip_json_int(const char *desc, struct json_object *jobj, int *val);
int cxip_json_int64(const char *desc, struct json_object *jobj, int64_t *val);
int cxip_json_double(const char *desc, struct json_object *jobj, double *val);
int cxip_json_string(const char *desc, struct json_object *jobj,
		     const char **val);

/* Perform zero-buffer collectives */
void cxip_tree_rowcol(int radix, int nodeidx, int *row, int *col, int *siz);
void cxip_tree_nodeidx(int radix, int row, int col, int *nodeidx);
int cxip_tree_relatives(int radix, int nodeidx, int maxnodes, int *rels);

int cxip_zbcoll_recv_cb(struct cxip_ep_obj *ep_obj, uint32_t init_nic,
			uint32_t init_pid, uint64_t mbv);
void cxip_zbcoll_send(struct cxip_zbcoll_obj *zb, int srcidx, int dstidx,
		      uint64_t payload);
void cxip_zbcoll_free(struct cxip_zbcoll_obj *zb);
int cxip_zbcoll_alloc(struct cxip_ep_obj *ep_obj, int num_addrs,
		      fi_addr_t *fiaddrs, int simrank,
		      struct cxip_zbcoll_obj **zbp);
int cxip_zbcoll_simlink(struct cxip_zbcoll_obj *zb0,
			struct cxip_zbcoll_obj *zb);
void cxip_zbcoll_set_user_cb(struct cxip_zbcoll_obj *zb,
			     zbcomplete_t userfunc, void *userptr);

int cxip_zbcoll_max_grps(bool sim);
int cxip_zbcoll_getgroup(struct cxip_zbcoll_obj *zb);
void cxip_zbcoll_rlsgroup(struct cxip_zbcoll_obj *zb);
int cxip_zbcoll_broadcast(struct cxip_zbcoll_obj *zb, uint64_t *dataptr);
int cxip_zbcoll_reduce(struct cxip_zbcoll_obj *zb, uint64_t *dataptr);
int cxip_zbcoll_barrier(struct cxip_zbcoll_obj *zb);
void cxip_ep_zbcoll_progress(struct cxip_ep_obj *ep_obj);

void cxip_zbcoll_reset_counters(struct cxip_ep_obj *ep_obj);
void cxip_zbcoll_get_counters(struct cxip_ep_obj *ep_obj, uint32_t *dsc,
			      uint32_t *err, uint32_t *ack, uint32_t *rcv);
void cxip_zbcoll_fini(struct cxip_ep_obj *ep_obj);
int cxip_zbcoll_init(struct cxip_ep_obj *ep_obj);

/*
 * CNTR/CQ wait object file list element
 *
 * Support structure.
 *
 * Created in cxip_cntr_open(), cxip_cq_open().
 */
struct cxip_fid_list {
	struct dlist_entry entry;
	struct fid *fid;
};

int cxip_rdzv_match_pte_alloc(struct cxip_txc_hpc *txc,
			      struct cxip_rdzv_match_pte **rdzv_pte);
int cxip_rdzv_nomatch_pte_alloc(struct cxip_txc_hpc *txc, int lac,
				struct cxip_rdzv_nomatch_pte **rdzv_pte);
int cxip_rdzv_pte_src_req_alloc(struct cxip_rdzv_match_pte *pte, int lac);
void cxip_rdzv_match_pte_free(struct cxip_rdzv_match_pte *pte);
void cxip_rdzv_nomatch_pte_free(struct cxip_rdzv_nomatch_pte *pte);
int cxip_rdzv_pte_zbp_cb(struct cxip_req *req, const union c_event *event);
int cxip_rdzv_pte_src_cb(struct cxip_req *req, const union c_event *event);

struct cxip_if *cxip_if_lookup_addr(uint32_t nic_addr);
struct cxip_if *cxip_if_lookup_name(const char *name);
int cxip_get_if(uint32_t nic_addr, struct cxip_if **dev_if);
void cxip_put_if(struct cxip_if *dev_if);
int cxip_if_valid_rgroup_vni(struct cxip_if *iface, unsigned int rgroup_id,
			     unsigned int vni);
int cxip_alloc_lni(struct cxip_if *iface, uint32_t svc_id,
		   struct cxip_lni **if_lni);
void cxip_free_lni(struct cxip_lni *lni);
const char *cxi_tc_str(enum cxi_traffic_class tc);
enum cxi_traffic_class cxip_ofi_to_cxi_tc(uint32_t ofi_tclass);
int cxip_cmdq_cp_set(struct cxip_cmdq *cmdq, uint16_t vni,
		     enum cxi_traffic_class tc,
		     enum cxi_traffic_class_type tc_type);
void cxip_if_init(void);
void cxip_if_fini(void);

int cxip_pte_set_state(struct cxip_pte *pte, struct cxip_cmdq *cmdq,
		       enum c_ptlte_state new_state, uint32_t drop_count);
int cxip_pte_set_state_wait(struct cxip_pte *pte, struct cxip_cmdq *cmdq,
			    struct cxip_evtq *evtq,
			    enum c_ptlte_state new_state, uint32_t drop_count);
int cxip_pte_append(struct cxip_pte *pte, uint64_t iova, size_t len,
		    unsigned int lac, enum c_ptl_list list,
		    uint32_t buffer_id, uint64_t match_bits,
		    uint64_t ignore_bits, uint32_t match_id,
		    uint64_t min_free, uint32_t flags,
		    struct cxip_cntr *cntr, struct cxip_cmdq *cmdq,
		    bool ring);
int cxip_pte_unlink(struct cxip_pte *pte, enum c_ptl_list list,
		    int buffer_id, struct cxip_cmdq *cmdq);
int cxip_pte_map(struct cxip_pte *pte, uint64_t pid_idx, bool is_multicast);
int cxip_pte_alloc_nomap(struct cxip_portals_table *ptable, struct cxi_eq *evtq,
			 struct cxi_pt_alloc_opts *opts,
			 void (*state_change_cb)(struct cxip_pte *pte,
						 const union c_event *event),
			 void *ctx, struct cxip_pte **pte);
int cxip_pte_alloc(struct cxip_portals_table *ptable, struct cxi_eq *evtq,
		   uint64_t pid_idx, bool is_multicast,
		   struct cxi_pt_alloc_opts *opts,
		   void (*state_change_cb)(struct cxip_pte *pte,
					   const union c_event *event),
		   void *ctx, struct cxip_pte **pte);
void cxip_pte_free(struct cxip_pte *pte);
int cxip_pte_state_change(struct cxip_if *dev_if, const union c_event *event);

int cxip_cmdq_alloc(struct cxip_lni *lni, struct cxi_eq *evtq,
		    struct cxi_cq_alloc_opts *cq_opts, uint16_t vni,
		    enum cxi_traffic_class tc,
		    enum cxi_traffic_class_type tc_type,
		    struct cxip_cmdq **cmdq);
void cxip_cmdq_free(struct cxip_cmdq *cmdq);
int cxip_cmdq_emit_c_state(struct cxip_cmdq *cmdq,
			   const struct c_cstate_cmd *cmd);

static inline bool cxip_cmdq_empty(struct cxip_cmdq *cmdq)
{
	return cxi_cq_empty(cmdq->dev_cmdq);
}

static inline bool cxip_cmdq_match(struct cxip_cmdq *cmdq, uint16_t vni,
				   enum cxi_traffic_class tc,
				   enum cxi_traffic_class_type tc_type)
{
	return (cmdq->cur_cp->vni == vni) && (cmdq->cur_cp->tc == tc) &&
		(cmdq->cur_cp->tc_type == tc_type);
}

int cxip_evtq_init(struct cxip_evtq *evtq, struct cxip_cq *cq,
		   size_t num_events, size_t num_fc_events);
void cxip_evtq_fini(struct cxip_evtq *eq);

int cxip_domain(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **dom, void *context);

int cxip_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric,
		void *context);

int cxip_endpoint(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep, void *context);

int cxip_tx_id_alloc(struct cxip_txc_hpc *txc, void *ctx);
int cxip_tx_id_free(struct cxip_txc_hpc *txc, int id);
void *cxip_tx_id_lookup(struct cxip_txc_hpc *txc, int id);
int cxip_rdzv_id_alloc(struct cxip_txc_hpc *txc, struct cxip_req *req);
int cxip_rdzv_id_free(struct cxip_txc_hpc *txc, int id);
void *cxip_rdzv_id_lookup(struct cxip_txc_hpc *txc, int id);
int cxip_ep_cmdq(struct cxip_ep_obj *ep_obj, bool transmit, uint32_t tclass,
		 struct cxi_eq *evtq, struct cxip_cmdq **cmdq);
void cxip_ep_cmdq_put(struct cxip_ep_obj *ep_obj, bool transmit);

int cxip_recv_ux_sw_matcher(struct cxip_ux_send *ux);
int cxip_recv_req_sw_matcher(struct cxip_req *req);
int cxip_recv_cancel(struct cxip_req *req);
int cxip_fc_process_drops(struct cxip_ep_obj *ep_obj, uint32_t nic_addr,
			  uint32_t pid, uint16_t vni, uint16_t drops);
void cxip_recv_pte_cb(struct cxip_pte *pte, const union c_event *event);
void cxip_rxc_req_fini(struct cxip_rxc *rxc);
int cxip_rxc_oflow_init(struct cxip_rxc *rxc);
void cxip_rxc_oflow_fini(struct cxip_rxc *rxc);
int cxip_fc_resume(struct cxip_ep_obj *ep_obj, uint32_t nic_addr, uint32_t pid,
		   uint16_t vni);

void cxip_txc_struct_init(struct cxip_txc *txc, const struct fi_tx_attr *attr,
			  void *context);
struct cxip_txc *cxip_txc_calloc(struct cxip_ep_obj *ep_obj, void *context);
void cxip_txc_free(struct cxip_txc *txc);
int cxip_txc_enable(struct cxip_txc *txc);
void cxip_txc_disable(struct cxip_txc *txc);
struct cxip_txc *cxip_stx_alloc(const struct fi_tx_attr *attr, void *context);
int cxip_rxc_msg_enable(struct cxip_rxc_hpc *rxc, uint32_t drop_count);

struct cxip_rxc *cxip_rxc_calloc(struct cxip_ep_obj *ep_obj, void *context);
void cxip_rxc_free(struct cxip_rxc *rxc);
int cxip_rxc_enable(struct cxip_rxc *rxc);
void cxip_rxc_disable(struct cxip_rxc *rxc);
void cxip_rxc_struct_init(struct cxip_rxc *rxc, const struct fi_rx_attr *attr,
			  void *context);
void cxip_rxc_recv_req_cleanup(struct cxip_rxc *rxc);

int cxip_rxc_emit_dma(struct cxip_rxc_hpc *rxc, uint16_t vni,
		      enum cxi_traffic_class tc,
		      enum cxi_traffic_class_type tc_type,
		      struct c_full_dma_cmd *dma, uint64_t flags);
int cxip_rxc_emit_idc_msg(struct cxip_rxc_hpc *rxc, uint16_t vni,
			  enum cxi_traffic_class tc,
			  enum cxi_traffic_class_type tc_type,
			  const struct c_cstate_cmd *c_state,
			  const struct c_idc_msg_hdr *msg, const void *buf,
			  size_t len, uint64_t flags);

int cxip_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		 struct fid_eq **eq, void *context);

bool cxip_evtq_saturated(struct cxip_evtq *evtq);
struct cxip_md *cxip_txc_ibuf_md(void *ibuf);
void *cxip_txc_ibuf_alloc(struct cxip_txc *txc);
void cxip_txc_ibuf_free(struct cxip_txc *txc, void *ibuf);
int cxip_ibuf_chunk_init(struct ofi_bufpool_region *region);
void cxip_ibuf_chunk_fini(struct ofi_bufpool_region *region);
int cxip_evtq_req_cancel(struct cxip_evtq *evtq, void *req_ctx,
			 void *op_ctx, bool match);
void cxip_evtq_req_discard(struct cxip_evtq *evtq, void *req_ctx);
void cxip_evtq_flush_trig_reqs(struct cxip_evtq *evtq);
int cxip_cq_req_complete(struct cxip_req *req);
int cxip_cq_req_complete_addr(struct cxip_req *req, fi_addr_t src);
int cxip_cq_req_error(struct cxip_req *req, size_t olen,
		      int err, int prov_errno, void *err_data,
		      size_t err_data_size, fi_addr_t src_addr);
int proverr2errno(int err);
struct cxip_req *cxip_evtq_req_alloc(struct cxip_evtq *evtq,
				     int remap, void *req_ctx);
void cxip_evtq_req_free(struct cxip_req *req);
void cxip_evtq_progress(struct cxip_evtq *evtq);

void cxip_ep_progress(struct fid *fid);
int cxip_ep_peek(struct fid *fid);
void cxip_ep_flush_trig_reqs(struct cxip_ep_obj *ep_obj);

void cxip_cq_progress(struct cxip_cq *cq);
void cxip_util_cq_progress(struct util_cq *util_cq);
int cxip_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq, void *context);
int cxip_evtq_adjust_reserved_fc_event_slots(struct cxip_evtq *evtq, int value);
void cxip_cq_flush_trig_reqs(struct cxip_cq *cq);

void cxip_dom_cntr_disable(struct cxip_domain *dom);
int cxip_cntr_mod(struct cxip_cntr *cxi_cntr, uint64_t value, bool set,
		  bool err);
int cxip_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		   struct fid_cntr **cntr, void *context);

int cxip_iomm_init(struct cxip_domain *dom);
void cxip_iomm_fini(struct cxip_domain *dom);
int cxip_map(struct cxip_domain *dom, const void *buf, unsigned long len,
	     uint64_t flags, struct cxip_md **md);
void cxip_unmap(struct cxip_md *md);

int cxip_ctrl_msg_send(struct cxip_ctrl_req *req);
void cxip_ep_ctrl_progress(struct cxip_ep_obj *ep_obj);
void cxip_ep_ctrl_progress_locked(struct cxip_ep_obj *ep_obj);
void cxip_ep_tx_ctrl_progress(struct cxip_ep_obj *ep_obj);
void cxip_ep_tx_ctrl_progress_locked(struct cxip_ep_obj *ep_obj);
void cxip_ep_tgt_ctrl_progress(struct cxip_ep_obj *ep_obj);
void cxip_ep_tgt_ctrl_progress_locked(struct cxip_ep_obj *ep_obj);
int cxip_ep_ctrl_init(struct cxip_ep_obj *ep_obj);
void cxip_ep_ctrl_fini(struct cxip_ep_obj *ep_obj);
void cxip_ep_ctrl_del_wait(struct cxip_ep_obj *ep_obj);
int cxip_ep_ctrl_trywait(void *arg);

int cxip_av_set(struct fid_av *av, struct fi_av_set_attr *attr,
	        struct fid_av_set **av_set_fid, void * context);

// TODO: naming convention for testing hooks
void cxip_coll_init(struct cxip_ep_obj *ep_obj);
int cxip_coll_enable(struct cxip_ep *ep);
int cxip_coll_disable(struct cxip_ep_obj *ep_obj);
void cxip_coll_close(struct cxip_ep_obj *ep_obj);
void cxip_coll_populate_opcodes(void);
int cxip_coll_send(struct cxip_coll_reduction *reduction,
		   int av_set_idx, const void *buffer, size_t buflen,
		   struct cxi_md *md);
int cxip_coll_send_red_pkt(struct cxip_coll_reduction *reduction,
			   const struct cxip_coll_data *coll_data,
			   bool arm, bool retry);

void cxip_capture_red_id(int *red_id_buf);
ssize_t cxip_barrier(struct fid_ep *ep, fi_addr_t coll_addr, void *context);
ssize_t cxip_broadcast(struct fid_ep *ep, void *buf, size_t count,
		       void *desc, fi_addr_t coll_addr, fi_addr_t root_addr,
		       enum fi_datatype datatype, uint64_t flags,
		       void *context);
ssize_t cxip_reduce(struct fid_ep *ep, const void *buf, size_t count,
		    void *desc, void *result, void *result_desc,
		    fi_addr_t coll_addr, fi_addr_t root_addr,
		    enum fi_datatype datatype, enum fi_op op, uint64_t flags,
		    void *context);
ssize_t cxip_allreduce(struct fid_ep *ep, const void *buf, size_t count,
		       void *desc, void *result, void *result_desc,
		       fi_addr_t coll_addr, enum fi_datatype datatype,
		       enum fi_op op, uint64_t flags, void *context);
int cxip_join_collective(struct fid_ep *ep, fi_addr_t coll_addr,
			 const struct fid_av_set *coll_av_set,
			 uint64_t flags, struct fid_mc **mc, void *context);
void cxip_coll_progress_join(struct cxip_ep_obj *ep_obj);

int cxip_coll_arm_disable(struct fid_mc *mc, bool disable);
void cxip_coll_limit_red_id(struct fid_mc *mc, int max_red_id);
void cxip_coll_drop_send(struct cxip_coll_reduction *reduction);
void cxip_coll_drop_recv(struct cxip_coll_reduction *reduction);

void cxip_coll_reset_mc_ctrs(struct fid_mc *mc);

void cxip_dbl_to_rep(struct cxip_repsum *x, double d);
void cxip_rep_to_dbl(double *d, const struct cxip_repsum *x);
void cxip_rep_add(struct cxip_repsum *x, const struct cxip_repsum *y);
double cxip_rep_add_dbl(double d1, double d2);
double cxip_rep_sum(size_t count, double *values);

int cxip_check_auth_key_info(struct fi_info *info);
int cxip_gen_auth_key(struct fi_info *info, struct cxi_auth_key *key);

#define CXIP_FC_SOFTWARE_INITIATED -1

/* cxip_fc_reason() - Returns the event reason for portal state
 * change (FC reason or SC reason).
 */
static inline int cxip_fc_reason(const union c_event *event)
{
	if (!event->tgt_long.initiator.state_change.sc_nic_auto)
		return CXIP_FC_SOFTWARE_INITIATED;

	return event->tgt_long.initiator.state_change.sc_reason;
}

static inline void cxip_txq_ring(struct cxip_cmdq *cmdq, bool more,
				 int otx_reqs)
{
	if (!more) {
		switch (cmdq->llring_mode) {
		case CXIP_LLRING_IDLE:
			if (!otx_reqs)
				cxi_cq_ll_ring(cmdq->dev_cmdq);
			else
				cxi_cq_ring(cmdq->dev_cmdq);
			break;
		case CXIP_LLRING_ALWAYS:
			cxi_cq_ll_ring(cmdq->dev_cmdq);
			break;
		case CXIP_LLRING_NEVER:
		default:
			cxi_cq_ring(cmdq->dev_cmdq);
			break;
		}
	}
}

ssize_t cxip_rma_common(enum fi_op_type op, struct cxip_txc *txc,
			const void *buf, size_t len, void *desc,
			fi_addr_t tgt_addr, uint64_t addr,
			uint64_t key, uint64_t data, uint64_t flags,
			uint32_t tclass, uint64_t msg_order, void *context,
			bool triggered, uint64_t trig_thresh,
			struct cxip_cntr *trig_cntr,
			struct cxip_cntr *comp_cntr);

/*
 * Request variants:
 *   CXIP_RQ_AMO
 *      Passes one argument (operand1), and applies that to a remote memory
 *      address content.
 *
 *   CXIP_RQ_AMO_FETCH
 *      Passes two arguments (operand1, resultptr), applies operand1 to a
 *      remote memory address content, and returns the prior content of the
 *      remote memory in resultptr.
 *
 *   CXIP_RQ_AMO_SWAP
 *      Passes three arguments (operand1, compare, resultptr). If remote memory
 *      address content satisfies the comparison operation with compare,
 *      replaces the remote memory content with operand1, and returns the prior
 *      content of the remote memory in resultptr.
 *
 *  CXIP_RQ_AMO_PCIE_FETCH
 *      Passes two arguments (operand1, resultptr), applies operand1 to a
 *      remote memory address content, and returns the prior content of the
 *      remote memory in resultptr.
 *
 *      The resulting operation should be a PCIe AMO instead of NIC AMO.
 */
enum cxip_amo_req_type {
	CXIP_RQ_AMO,
	CXIP_RQ_AMO_FETCH,
	CXIP_RQ_AMO_SWAP,
	CXIP_RQ_AMO_PCIE_FETCH,
	CXIP_RQ_AMO_LAST,
};

int cxip_amo_common(enum cxip_amo_req_type req_type, struct cxip_txc *txc,
		    uint32_t tclass, const struct fi_msg_atomic *msg,
		    const struct fi_ioc *comparev, void **comparedesc,
		    size_t compare_count, const struct fi_ioc *resultv,
		    void **resultdesc, size_t result_count, uint64_t flags,
		    bool triggered, uint64_t trig_thresh,
		    struct cxip_cntr *trig_cntr, struct cxip_cntr *comp_cntr);
int _cxip_atomic_opcode(enum cxip_amo_req_type req_type, enum fi_datatype dt,
			enum fi_op op, int amo_remap_to_pcie_fadd,
			enum c_atomic_op *cop, enum c_atomic_type *cdt,
			enum c_cswap_op *copswp, unsigned int *cdtlen);

static inline void
cxip_domain_add_txc(struct cxip_domain *dom, struct cxip_txc *txc)
{
	ofi_spin_lock(&dom->lock);
	dlist_insert_tail(&txc->dom_entry, &dom->txc_list);
	ofi_spin_unlock(&dom->lock);
}

static inline void
cxip_domain_remove_txc(struct cxip_domain *dom, struct cxip_txc *txc)
{
	ofi_spin_lock(&dom->lock);
	dlist_remove(&txc->dom_entry);
	ofi_spin_unlock(&dom->lock);
}

static inline void
cxip_domain_add_cntr(struct cxip_domain *dom, struct cxip_cntr *cntr)
{
	ofi_spin_lock(&dom->lock);
	dlist_insert_tail(&cntr->dom_entry, &dom->cntr_list);
	ofi_atomic_inc32(&dom->ref);
	ofi_spin_unlock(&dom->lock);
}

static inline void
cxip_domain_remove_cntr(struct cxip_domain *dom, struct cxip_cntr *cntr)
{
	ofi_spin_lock(&dom->lock);
	dlist_remove(&cntr->dom_entry);
	ofi_atomic_dec32(&dom->ref);
	ofi_spin_unlock(&dom->lock);
}

static inline void
cxip_domain_add_cq(struct cxip_domain *dom, struct cxip_cq *cq)
{
	ofi_spin_lock(&dom->lock);
	dlist_insert_tail(&cq->dom_entry, &dom->cq_list);
	ofi_atomic_inc32(&dom->ref);
	ofi_spin_unlock(&dom->lock);
}

static inline void
cxip_domain_remove_cq(struct cxip_domain *dom, struct cxip_cq *cq)
{
	ofi_spin_lock(&dom->lock);
	dlist_remove(&cq->dom_entry);
	ofi_atomic_dec32(&dom->ref);
	ofi_spin_unlock(&dom->lock);
}

int cxip_domain_ctrl_id_alloc(struct cxip_domain *dom,
			      struct cxip_ctrl_req *req);
void cxip_domain_ctrl_id_free(struct cxip_domain *dom,
			      struct cxip_ctrl_req *req);
int cxip_domain_prov_mr_id_alloc(struct cxip_domain *dom,
				 struct cxip_mr *mr);
void cxip_domain_prov_mr_id_free(struct cxip_domain *dom,
				 struct cxip_mr *mr);

static inline
struct cxip_ctrl_req *cxip_domain_ctrl_id_at(struct cxip_domain *dom,
					     int buffer_id)
{
	if (ofi_idx_is_valid(&dom->req_ids, buffer_id))
		return ofi_idx_at(&dom->req_ids, buffer_id);
	return NULL;
}

static inline uint32_t cxip_mac_to_nic(struct ether_addr *mac)
{
	return mac->ether_addr_octet[5] |
			(mac->ether_addr_octet[4] << 8) |
			((mac->ether_addr_octet[3] & 0xF) << 16);
}

static inline bool is_netsim(struct cxip_ep_obj *ep_obj)
{
	return (ep_obj->domain->iface->info->device_platform ==
		CXI_PLATFORM_NETSIM);
}

/* debugging TRACE functions */
#define	cxip_coll_trace_attr	__attribute__((format(__printf__, 1, 2)))
extern bool cxip_coll_trace_muted;		// suppress output if true
extern bool cxip_coll_trace_append;		// append open for trace file
extern bool cxip_coll_trace_linebuf;		// set line buffering for trace
extern int cxip_coll_trace_rank;		// tracing rank
extern int cxip_coll_trace_numranks;		// tracing number of ranks
extern FILE *cxip_coll_trace_fid;		// trace output file descriptor

int cxip_coll_trace_attr cxip_coll_trace(const char *fmt, ...);
void cxip_coll_trace_flush(void);
void cxip_coll_trace_close(void);
void cxip_coll_trace_init(void);

/* debugging TRACE filtering control */
enum cxip_coll_trace_module {
	CXIP_TRC_CTRL,
	CXIP_TRC_ZBCOLL,
	CXIP_TRC_COLL_CURL,
	CXIP_TRC_COLL_PKT,
	CXIP_TRC_COLL_JOIN,
	CXIP_TRC_COLL_DEBUG,
	CXIP_TRC_TEST_CODE,
	CXIP_TRC_MAX
};
extern uint64_t cxip_coll_trace_mask;

static inline void cxip_coll_trace_set(int mod)
{
	cxip_coll_trace_mask |= (1L << mod);
}

static inline void cxip_coll_trace_clr(int mod)
{
	cxip_coll_trace_mask &= ~(1L << mod);
}

static inline bool cxip_coll_trace_true(int mod)
{
	return (!cxip_coll_trace_muted) && (cxip_coll_trace_mask & (1L << mod));
}

#if ENABLE_DEBUG
#define CXIP_COLL_TRACE(mod, fmt, ...) \
	do {if (cxip_coll_trace_true(mod)) \
	    cxip_coll_trace(fmt, ##__VA_ARGS__);} while (0)
#else
#define	CXIP_COLL_TRACE(mod, fmt, ...) do {} while (0)
#endif

/* fabric logging implementation functions */
#define _CXIP_DBG(subsys, fmt,  ...) \
	FI_DBG(&cxip_prov, subsys, "%s: " fmt "", cxip_env.hostname, \
	       ##__VA_ARGS__)
#define _CXIP_INFO(subsys, fmt, ...) \
	FI_INFO(&cxip_prov, subsys, "%s: " fmt "", cxip_env.hostname, \
		##__VA_ARGS__)
#define _CXIP_WARN(subsys, fmt, ...) \
	FI_WARN(&cxip_prov, subsys, "%s: " fmt "", cxip_env.hostname, \
		##__VA_ARGS__)
#define _CXIP_WARN_ONCE(subsys, fmt, ...) \
	FI_WARN_ONCE(&cxip_prov, subsys, "%s: " fmt "", cxip_env.hostname, \
		     ##__VA_ARGS__)
#define CXIP_LOG(fmt,  ...) \
	fi_log(&cxip_prov, FI_LOG_WARN, FI_LOG_CORE, \
	       __func__, __LINE__, "%s: " fmt "", cxip_env.hostname, \
	       ##__VA_ARGS__)

#define CXIP_FATAL(fmt, ...)					\
	do {							\
		CXIP_LOG(fmt, ##__VA_ARGS__);			\
		abort();					\
	} while (0)

#define TXC_BASE(txc) ((struct cxip_txc *)(void *)(txc))
#define TXC_DBG(txc, fmt, ...) \
	_CXIP_DBG(FI_LOG_EP_DATA, "TXC (%#x:%u): " fmt "", \
		  TXC_BASE(txc)->ep_obj->src_addr.nic, \
		  TXC_BASE(txc)->ep_obj->src_addr.pid, ##__VA_ARGS__)
#define TXC_INFO(txc, fmt, ...) \
	_CXIP_INFO(FI_LOG_EP_DATA, "TXC (%#x:%u): " fmt "", \
		   TXC_BASE(txc)->ep_obj->src_addr.nic, \
		   TXC_BASE(txc)->ep_obj->src_addr.pid, ##__VA_ARGS__)
#define TXC_WARN(txc, fmt, ...) \
	_CXIP_WARN(FI_LOG_EP_DATA, "TXC (%#x:%u): " fmt "", \
		   TXC_BASE(txc)->ep_obj->src_addr.nic, \
		   TXC_BASE(txc)->ep_obj->src_addr.pid, ##__VA_ARGS__)
#define TXC_WARN_RET(txc, ret, fmt, ...) \
	TXC_WARN(txc, "%d:%s: " fmt "", ret, fi_strerror(-ret), ##__VA_ARGS__)
#define TXC_FATAL(txc, fmt, ...) \
	CXIP_FATAL("TXC (%#x:%u):: " fmt "", \
		   TXC_BASE(txc)->ep_obj->src_addr.nic, \
		   TXC_BASE(txc)->ep_obj->src_addr.pid, ##__VA_ARGS__)

#define RXC_BASE(rxc) ((struct cxip_rxc *)(void *)(rxc))
#define RXC_DBG(rxc, fmt, ...) \
	_CXIP_DBG(FI_LOG_EP_DATA, "RXC (%#x:%u) PtlTE %u: " fmt "", \
		  RXC_BASE(rxc)->ep_obj->src_addr.nic, \
		  RXC_BASE(rxc)->ep_obj->src_addr.pid, \
		  RXC_BASE(rxc)->rx_pte->pte->ptn, ##__VA_ARGS__)
#define RXC_INFO(rxc, fmt, ...) \
	_CXIP_INFO(FI_LOG_EP_DATA, "RXC (%#x:%u) PtlTE %u: " fmt "", \
		   RXC_BASE(rxc)->ep_obj->src_addr.nic, \
		   RXC_BASE(rxc)->ep_obj->src_addr.pid, \
		   RXC_BASE(rxc)->rx_pte->pte->ptn, ##__VA_ARGS__)
#define RXC_WARN(rxc, fmt, ...) \
	_CXIP_WARN(FI_LOG_EP_DATA, "RXC (%#x:%u) PtlTE %u: " fmt "", \
		   RXC_BASE(rxc)->ep_obj->src_addr.nic, \
		   RXC_BASE(rxc)->ep_obj->src_addr.pid, \
		   RXC_BASE(rxc)->rx_pte->pte->ptn, ##__VA_ARGS__)
#define RXC_WARN_ONCE(rxc, fmt, ...) \
	_CXIP_WARN_ONCE(FI_LOG_EP_DATA, "RXC (%#x:%u) PtlTE %u: " fmt "", \
			RXC_BASE(rxc)->ep_obj->src_addr.nic, \
			RXC_BASE(rxc)->ep_obj->src_addr.pid, \
			RXC_BASE(rxc)->rx_pte->pte->ptn, ##__VA_ARGS__)
#define RXC_FATAL(rxc, fmt, ...) \
	CXIP_FATAL("RXC (%#x:%u) PtlTE %u:[Fatal] " fmt "", \
		   RXC_BASE(rxc)->ep_obj->src_addr.nic, \
		   RXC_BASE(rxc)->ep_obj->src_addr.pid, \
		   RXC_BASE(rxc)->rx_pte->pte->ptn, ##__VA_ARGS__)

#define DOM_INFO(dom, fmt, ...) \
	_CXIP_INFO(FI_LOG_DOMAIN, "DOM (cxi%u:%u:%u:%u:%#x): " fmt "", \
		   (dom)->iface->info->dev_id, (dom)->lni->lni->id, \
		   (dom)->auth_key.svc_id, (dom)->auth_key.vni, \
		   (dom)->nic_addr, ##__VA_ARGS__)
#define DOM_WARN(dom, fmt, ...) \
	_CXIP_WARN(FI_LOG_DOMAIN, "DOM (cxi%u:%u:%u:%u:%#x): " fmt "", \
		   (dom)->iface->info->dev_id, (dom)->lni->lni->id, \
		   (dom)->auth_key.svc_id, (dom)->auth_key.vni, \
		   (dom)->nic_addr, ##__VA_ARGS__)

#define CXIP_UNEXPECTED_EVENT_STS "Unexpected event status, %s rc = %s\n"
#define CXIP_UNEXPECTED_EVENT "Unexpected event %s, rc = %s\n"

#define CXIP_DEFAULT_CACHE_LINE_SIZE 64

#define CXIP_SYSFS_CACHE_LINE_SIZE      \
	"/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size"

/* cxip_cacheline_size() - Return the CPU cache-line size, if unable to
 * read then return the assumed cache size.
 */
static inline int cxip_cacheline_size(void)
{
	FILE *f;
	int cache_line_size;
	int ret;

	f = fopen(CXIP_SYSFS_CACHE_LINE_SIZE, "r");
	if (!f) {
		_CXIP_WARN(FI_LOG_CORE,
			   "Error %d determining cacheline size\n",
			   errno);
		cache_line_size = CXIP_DEFAULT_CACHE_LINE_SIZE;
	} else {
		ret = fscanf(f, "%d", &cache_line_size);
		if (ret != 1) {
			_CXIP_WARN(FI_LOG_CORE,
				   "Error reading cacheline size\n");
			cache_line_size = CXIP_DEFAULT_CACHE_LINE_SIZE;
		}

		fclose(f);
	}

	return cache_line_size;
}

static inline int
cxip_txc_copy_from_hmem(struct cxip_txc *txc, struct cxip_md *hmem_md,
			void *dest, const void *hmem_src, size_t size)
{
	enum fi_hmem_iface iface;
	uint64_t device;
	struct iovec hmem_iov;
	struct cxip_domain *domain = txc->domain;
	uint64_t flags;
	bool unmap_hmem_md = false;
	int ret;

	/* Default to memcpy unless FI_HMEM is set. */
	if (!txc->hmem) {
		memcpy(dest, hmem_src, size);
		return FI_SUCCESS;
	}

	/* With HMEM enabled, performing memory registration will also cause
	 * the device buffer to be registered for CPU load/store access. Being
	 * able to perform load/store instead of using the generic HMEM copy
	 * routines and/or HMEM override copy routines can significantly reduce
	 * latency. Thus, this path is favored.
	 *
	 * However, if FORK_SAFE variables are enabled, we avoid this mapping
	 * to keep from designating the entire page in which the buffer
	 * resides as don't copy, and take the performance hit.
	 *
	 * Memory registration can result in additional latency. Expectation is
	 * the MR cache can amortize the additional memory registration latency.
	 */
	if (!cxip_env.fork_safe_requested) {
		if (!hmem_md) {
			ret = cxip_map(domain, hmem_src, size, 0, &hmem_md);
			if (ret) {
				TXC_WARN(txc, "cxip_map failed: %d:%s\n", ret,
					 fi_strerror(-ret));
				return ret;
			}

			unmap_hmem_md = true;
		}

		cxip_copy_from_md(hmem_md, dest, hmem_src, size);
		if (unmap_hmem_md)
			cxip_unmap(hmem_md);

		return FI_SUCCESS;
	}

	/* Slow path HMEM copy path.*/
	iface = ofi_get_hmem_iface(hmem_src, &device, &flags);
	hmem_iov.iov_base = (void *)hmem_src;
	hmem_iov.iov_len = size;

	ret = domain->hmem_ops.copy_from_hmem_iov(dest, size, iface, device,
						  &hmem_iov, 1, 0);
	if (ret != size) {
		if (ret < 0) {
			TXC_WARN(txc, "copy_from_hmem_iov failed: %d:%s\n", ret,
				 fi_strerror(-ret));
			return ret;
		}

		TXC_WARN(txc,
			 "copy_from_hmem_iov short copy: expect=%ld got=%d\n",
			 size, ret);
		return -FI_EIO;
	}

	return FI_SUCCESS;
}

static inline
int cxip_set_recv_match_id(struct cxip_rxc *rxc, fi_addr_t src_addr,
			   bool auth_key, uint32_t *match_id, uint16_t *vni)
{
	struct cxip_addr caddr;
	int ret;

	/* If FI_DIRECTED_RECV and a src_addr is specified, encode the address
	 * in the LE for matching. If application AVs are symmetric, use
	 * logical FI address for matching. Otherwise, use physical address.
	 */
	if (rxc->attr.caps & FI_DIRECTED_RECV &&
	    src_addr != FI_ADDR_UNSPEC) {
		if (rxc->ep_obj->av->symmetric) {
			/* PID is not used for matching */
			*match_id = CXI_MATCH_ID(rxc->pid_bits,
						C_PID_ANY, src_addr);
			*vni = rxc->ep_obj->auth_key.vni;
		} else {
			ret = cxip_av_lookup_addr(rxc->ep_obj->av, src_addr,
						  &caddr);
			if (ret != FI_SUCCESS) {
				RXC_WARN(rxc, "Failed to look up FI addr: %d\n",
					 ret);
				return -FI_EINVAL;
			}

			*match_id = CXI_MATCH_ID(rxc->pid_bits, caddr.pid,
						 caddr.nic);
			if (auth_key)
				*vni = caddr.vni;
			else
				*vni = rxc->ep_obj->auth_key.vni;
		}
	} else {
		*match_id = CXI_MATCH_ID_ANY;
		*vni = 0;
	}

	return FI_SUCCESS;
}

fi_addr_t cxip_recv_req_src_addr(struct cxip_req *req);
int cxip_recv_req_alloc(struct cxip_rxc *rxc, void *buf, size_t len,
			struct cxip_md *md, struct cxip_req **cxip_req,
			int (*recv_cb)(struct cxip_req *req,
				       const union c_event *event));
void cxip_recv_req_free(struct cxip_req *req);
void cxip_recv_req_report(struct cxip_req *req);
void cxip_recv_req_peek_complete(struct cxip_req *req,
				 struct cxip_ux_send *ux_send);
struct cxip_req *cxip_mrecv_req_dup(struct cxip_req *mrecv_req);
int cxip_complete_put(struct cxip_req *req, const union c_event *event);
/* XXXX TODO: Remove */
/* Defines the posted receive interval for checking LE allocation if
 * in hybrid RX match mode and preemptive transitions to software
 * managed EP are requested.
 */
#define CXIP_HYBRID_RECV_CHECK_INTERVAL (64-1)
#define FC_SW_LE_MSG_FATAL "LE exhaustion during flow control, "\
	"FI_CXI_RX_MATCH_MODE=[hybrid|software] is required\n"
int cxip_recv_pending_ptlte_disable(struct cxip_rxc *rxc, bool check_fc);
int cxip_flush_appends(struct cxip_rxc_hpc *rxc,
		       int (*flush_cb)(struct cxip_req *req,
				       const union c_event *event));
int cxip_recv_req_dropped(struct cxip_req *req);
void cxip_rxc_record_req_stat(struct cxip_rxc *rxc, enum c_ptl_list list,
			      size_t rlength, struct cxip_req *req);
bool tag_match(uint64_t init_mb, uint64_t mb, uint64_t ib);
bool init_match(struct cxip_rxc *rxc, uint32_t init, uint32_t match_id);
uint32_t cxip_msg_match_id(struct cxip_txc *txc);
void cxip_report_send_completion(struct cxip_req *req, bool sw_cntr);
bool cxip_send_eager_idc(struct cxip_req *req);
void cxip_send_buf_fini(struct cxip_req *req);
int cxip_send_buf_init(struct cxip_req *req);

size_t cxip_ep_get_unexp_msgs(struct fid_ep *fid_ep,
			      struct fi_cq_tagged_entry *entry, size_t count,
			      fi_addr_t *src_addr, size_t *ux_count);

int cxip_nic_alloc(struct cxip_if *nic_if, struct fid_nic **fid_nic);

int cxip_domain_dwq_emit_dma(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     enum cxi_traffic_class_type tc_type,
			     struct cxip_cntr *trig_cntr, size_t trig_thresh,
			     struct c_full_dma_cmd *dma, uint64_t flags);
int cxip_domain_dwq_emit_amo(struct cxip_domain *dom, uint16_t vni,
			     enum cxi_traffic_class tc,
			     enum cxi_traffic_class_type tc_type,
			     struct cxip_cntr *trig_cntr, size_t trig_thresh,
			     struct c_dma_amo_cmd *amo, uint64_t flags,
			     bool fetching, bool flush);

#endif

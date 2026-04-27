/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2025 by Cornelis Networks.
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
#ifndef __FI_PROV_OPX_H__
#define __FI_PROV_OPX_H__

#include <config.h>

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include <rdma/fabric.h>
#include <rdma/providers/fi_log.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_eq.h>
#include <rdma/fi_errno.h>
#include <ofi_lock.h>
#include <uthash.h>
#include <ofi_list.h>
#include <opa_byteorder.h>

// #define FI_OPX_TRACE 1

#define FI_OPX_IOV_LIMIT (1)

/* --- Will be exposed by fabric.h */
#define FI_OPX_PROTOCOL		0x0008
#define FI_OPX_PROTOCOL_VERSION (1)
/* --- end */

#define FI_OPX_PROVIDER_NAME "opx"
// Optimized builds will crash if the provider version is something other than this
// So it's hard to communicate that the Opx provider is PRE-GA via the version number
// Delete this comment when OPX is GA-ed
#define FI_OPX_PROVIDER_VERSION	    (OFI_VERSION_DEF_PROV)
#define FI_OPX_DEVICE_MAX_PATH_NAME (32)
#define FI_OPX_FABRIC_NAME	    "OPX-100"
#define FI_OPX_DOMAIN_NAME	    "ib0"
#define FI_OPX_DOMAIN_NAME_PREFIX   "ib"

#define FI_OPX_CACHE_LINE_SIZE (64)
#define FI_OPX_CACHE_LINE_QWS  (FI_OPX_CACHE_LINE_SIZE / sizeof(uint64_t))

#define FI_OPX_MAX_STRLEN (64)

#define EXIT_FAILURE 1

#define OPX_CTX_SHARING_OFF 0
#define OPX_CTX_SHARING_ON  1

// TODO: This is needed until we complete the locking model.
#define OPX_LOCK 0

// These defines may change or disappear as we develop more capabilities.
#define OPX_CQ_FORMAT	FI_CQ_FORMAT_TAGGED
#define OPX_RELIABILITY OFI_RELIABILITY_KIND_ONLOAD
#define OPX_MASK	FI_OPX_HDRQ_MASK_RUNTIME
#define OPX_CQ_CAPS	0x0000000000000000ull
#define OPX_EP_CAPS	0x0018000000000000ull
#define OPX_TAGGED_CAPS 0x0018000000000000ull

// Uncomment to enabled Opx flight recorder
// #define FLIGHT_RECORDER_ENABLE		(1)

// Useful for checking that structures are the correct size and other
// compile-time tests. static_assert has existed since C11 so this
// should be safe, but we have an else clause just in case.
#if __STDC_VERSION__ >= 201112L
#define OPX_COMPILE_TIME_ASSERT(cond, msg) static_assert(cond, msg)
#else
#define OPX_COMPILE_TIME_ASSERT(cond, msg) \
	if (0) {                           \
		switch (0) {               \
		case 0:                    \
		case cond:;                \
		}                          \
	}
#endif

extern struct fi_provider fi_opx_provider;
struct fi_opx_daos_hfi_rank_key {
	uint8_t	 hfi_unit_number;
	uint32_t rank;
};

struct fi_opx_daos_hfi_rank {
	struct fi_opx_daos_hfi_rank_key key;
	uint32_t			instance;
	UT_hash_handle			hh; /* makes this structure hashable */
};

typedef uint32_t opx_lid_t; /* only 3 bytes of lid is used */

/* hfi1 type for bit logic */
enum opx_hfi1_type {
	OPX_HFI1_UNDEF	  = 0, // undefined
	OPX_HFI1_MIXED_9B = 1, // CN5000+ built for mixed network. Internal use
	OPX_HFI1_WFR	  = 2, // Omni-path (all generations)
	OPX_HFI1_JKR	  = 4, // CN5000 (initial generation)
	OPX_HFI1_CYR	  = 8  // CN6000 (initial generation)
};
/* Post WFR 16B support - CN5000, CN6000 - unless they need to be differentiated */
#define OPX_HFI1_CNX000 (OPX_HFI1_JKR | OPX_HFI1_CYR)

/* Arbitrary packet "types" that can be differentiated as needed (route control) */
enum opx_hfi1_packet_type {
	OPX_HFI1_INJECT		  = 0, // Inject packets
	OPX_HFI1_EAGER		  = 1, // Eager eager packets
	OPX_HFI1_MP_EAGER	  = 2, // MP Eager eager packets
	OPX_HFI1_DPUT		  = 3, // DPUT  packets
	OPX_HFI1_RZV_CTRL	  = 4, // Rendezvous RTS/CTS control packets
	OPX_HFI1_RZV_DATA	  = 5, // Rendezvoud data packets
	OPX_HFI1_NUM_PACKET_TYPES = 6
};

static const char *const OPX_HFI1_PACKET_STR[] = {
	[OPX_HFI1_INJECT] = "OPX_HFI1_INJECT",	   [OPX_HFI1_EAGER] = "OPX_HFI1_EAGER",
	[OPX_HFI1_MP_EAGER] = "OPX_HFI1_MP_EAGER", [OPX_HFI1_DPUT] = "OPX_HFI1_DPUT",
	[OPX_HFI1_RZV_CTRL] = "OPX_HFI1_RZV_CTRL", [OPX_HFI1_RZV_DATA] = "OPX_HFI1_RZV_DATA"};

/* Will remove after 16B SDMA support is finished */
#define OPX_NO_9B_SUPPORT(_hfi1_type)                                                                             \
	do {                                                                                                      \
		if (!(_hfi1_type & OPX_HFI1_CNX000)) {                                                            \
			fprintf(stderr, "%s NO 9B SUPPORT for %s\n", __func__, OPX_HFI1_TYPE_STRING(_hfi1_type)); \
			if (getenv("OPX_9B_ABORT"))                                                               \
				abort();                                                                          \
		}                                                                                                 \
		assert(_hfi1_type != OPX_HFI1_UNDEF);                                                             \
	} while (0)

#define OPX_NO_16B_SUPPORT(_hfi1_type)                                                                             \
	do {                                                                                                       \
		if (!(_hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B))) {                                          \
			fprintf(stderr, "%s NO 16B SUPPORT for %s\n", __func__, OPX_HFI1_TYPE_STRING(_hfi1_type)); \
			if (getenv("OPX_16B_ABORT"))                                                               \
				abort();                                                                           \
		}                                                                                                  \
		assert(_hfi1_type != OPX_HFI1_UNDEF);                                                              \
	} while (0)

#define OPX_MAX_HFIS (16)
OPX_COMPILE_TIME_ASSERT((OPX_MAX_HFIS & 3) == 0, "OPX_MAX_HFIS must be a multiple of 4!\n");

struct opx_hfi_local_entry {
	uint32_t instance;
	uint8_t	 hfi_unit;
	uint8_t	 unused[3];
} __attribute__((__packed__)) __attribute__((aligned(8)));

struct opx_hfi_local_info {
	/* == CACHE LINE 0 == */
	uint32_t	   local_lids_size;
	enum opx_hfi1_type sw_type;	 // SW defined hfi1 type, including "mixed networks"
	int		   sim_sctxt_fd; // simulator send context BAR resource fd
	int		   sim_rctxt_fd; // simulator recv context BAR resource fd
	uint64_t	   pbc_lid;
	opx_lid_t	   lid;
	uint8_t		   hfi_unit;
	bool		   sriov;
	bool		   multi_vm;  // self lid is used across VMs
	bool		   multi_lid; // job has multiple lids
	int32_t		   min_rctxt;
	int32_t		   max_rctxt;
	enum opx_hfi1_type hw_type; // HW hfi1 type before "mixed_network" changes
	uint32_t	   unused_dws[1];
	uint64_t	   unused_qws[2];

	/* == CACHE LINE 1 == */
	opx_lid_t local_lid_ids[OPX_MAX_HFIS];

	/* == CACHE LINE 2 & 3 == */
	struct opx_hfi_local_entry local_lid_entries[OPX_MAX_HFIS];
} __attribute__((__packed__)) __attribute__((aligned(64)));
OPX_COMPILE_TIME_ASSERT(offsetof(struct opx_hfi_local_info, local_lid_ids) == (FI_OPX_CACHE_LINE_SIZE * 1),
			"Offset of opx_hfi_local_info->local_lid_ids should start at cacheline 1!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct opx_hfi_local_info, local_lid_entries) == (FI_OPX_CACHE_LINE_SIZE * 2),
			"Offset of opx_hfi_local_info->local_lid_entries should start at cacheline 2!");
OPX_COMPILE_TIME_ASSERT(sizeof(struct opx_hfi_local_info) == (FI_OPX_CACHE_LINE_SIZE * 4),
			"Size of opx_hfi_local_info should be 4 cachelines!");

#ifdef OPX_SIM
/* Build L8SIM support */
#define OPX_SIM_ENABLED
#warning OPX_SIM enabled

#else
/* Build only "real" HFI1 support (default) */
#undef OPX_SIM_ENABLED
#endif

// HFI1 type from SW/logical point of view
#define OPX_SW_HFI1_TYPE fi_opx_global.hfi_local_info.sw_type

// HFI1 HW type independent of SW
#define OPX_HW_HFI1_TYPE fi_opx_global.hfi_local_info.hw_type

#define OPX_IS_CTX_SHARING_ENABLED fi_opx_global.ctx_sharing_enabled

// This is constant for all (macro magic) _hfi1_type except OPX_HFI1_MIXED_9B which additionally checks a variable
// OPX_HW_HFI1_TYPE
#define OPX_IS_EXTENDED_RX(_hfi1_type) \
	((_hfi1_type & OPX_HFI1_CYR) || ((_hfi1_type & OPX_HFI1_MIXED_9B) && (OPX_HW_HFI1_TYPE & OPX_HFI1_CYR)))

// Alternative - this only checks the variable OPX_HW_HFI1_TYPE
// define OPX_IS_EXTENDED_RX(_ignored) (OPX_HW_HFI1_TYPE & OPX_HFI1_CYR)

struct fi_opx_global_data {
	/* == CACHE LINE 0 == */
	struct fi_info	      *info;
	struct fi_domain_attr *default_domain_attr;
	struct fi_ep_attr     *default_ep_attr;
	struct fi_tx_attr     *default_tx_attr;
	struct fi_rx_attr     *default_rx_attr;
	struct fi_provider    *prov;
	struct dlist_entry     tid_domain_list;

	/* == CACHE LINE 1 == */
	struct dlist_entry	     hmem_domain_list;
	struct fi_opx_daos_hfi_rank *daos_hfi_rank_hashmap;
	enum fi_progress	     progress;
	bool			     ctx_sharing_enabled;
	uint16_t		     pkt_size;
	uint8_t			     unused[1];
	uint32_t		     rcvhdrq_entry_dws;
	uint32_t		     unused_dw;
	uint64_t		     unused_qw[3];

	/* == CACHE LINE 2+ == */
	struct opx_hfi_local_info hfi_local_info;

	char *opx_hfi1_type_strings[];
} __attribute__((__packed__)) __attribute__((aligned(64)));
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_global_data, hmem_domain_list) == (FI_OPX_CACHE_LINE_SIZE * 1),
			"Offset of fi_opx_global_data->hmem_domain_list should start at cacheline 1!");
OPX_COMPILE_TIME_ASSERT(offsetof(struct fi_opx_global_data, hfi_local_info) == (FI_OPX_CACHE_LINE_SIZE * 2),
			"Offset of fi_opx_global_data->hfi_local_info should start at cacheline 2!");

#define OPX_HFI1_TYPE_STRING(_hfi1_type)                                   \
	({                                                                 \
		assert((_hfi1_type >= 0) && (_hfi1_type <= OPX_HFI1_CYR)); \
		fi_opx_global.opx_hfi1_type_strings[_hfi1_type];           \
	})

extern struct fi_opx_global_data fi_opx_global;

static const uint64_t FI_OPX_MAX_MSG_SIZE	 = ((uint64_t) -1);
static const uint64_t FI_OPX_MAX_PREFIX_SIZE	 = (0ULL);
static const uint64_t FI_OPX_MAX_ORDER_RAW_SIZE	 = SIZE_MAX;
static const uint64_t FI_OPX_MAX_ORDER_WAR_SIZE	 = SIZE_MAX;
static const uint64_t FI_OPX_MAX_ORDER_WAW_SIZE	 = SIZE_MAX;
static const size_t   FI_OPX_REMOTE_CQ_DATA_SIZE = 4;

static const uint64_t FI_OPX_MEM_TAG_FORMAT = (0xFFFFFFFFFFFFFFFFULL);

/*
Users may wish to change the depth of the Rx context ring.
/sys/module/hfi1/parameters/rcvhdrcnt:8192 is a tuning knob that
allows useres to specify the count of entries.

Supported values are 2048 (default), 4096, and 8192
To save on library bloat, 4096 is not implimented as an optimized compile-time value

Remember that fi_opx_hfi1_poll_once works in terms of 4 doublewords
So for example, 2k ring length (0-based) mask is
2047 * 32 = 0xFFE0
*/
static const uint64_t FI_OPX_HDRQ_MASK_RUNTIME = 0ULL;
static const uint64_t FI_OPX_HDRQ_MASK_2048    = 0X000000000000FFE0UL;
static const uint64_t FI_OPX_HDRQ_MASK_8192    = 0X000000000003FFE0UL;

#define FI_OPX_DEFAULT_MSG_ORDER (FI_ORDER_ATOMIC_RAR | FI_ORDER_ATOMIC_RAW | FI_ORDER_ATOMIC_WAR | FI_ORDER_ATOMIC_WAW)

#define FI_OPX_TXONLY_CAPS (FI_SEND | FI_READ | FI_WRITE | FI_REMOTE_WRITE)

#define FI_OPX_RXONLY_CAPS (FI_RECV | FI_DIRECTED_RECV | FI_MULTI_RECV | FI_REMOTE_READ)

#ifdef OPX_HMEM
#define FI_OPX_BASE_CAPS \
	(FI_MSG | FI_TAGGED | FI_LOCAL_COMM | FI_REMOTE_COMM | FI_SOURCE | FI_RMA | FI_ATOMIC | FI_HMEM)
#define FI_OPX_BASE_MR_MODE (OPX_MR | FI_MR_HMEM)
#else
#define FI_OPX_BASE_CAPS    (FI_MSG | FI_TAGGED | FI_LOCAL_COMM | FI_REMOTE_COMM | FI_SOURCE | FI_RMA | FI_ATOMIC)
#define FI_OPX_BASE_MR_MODE (OPX_MR)
#endif

#define FI_OPX_DEFAULT_CAPS (FI_OPX_BASE_CAPS | FI_OPX_TXONLY_CAPS | FI_OPX_RXONLY_CAPS)

#define FI_OPX_DEFAULT_TX_CAPS (FI_OPX_BASE_CAPS | FI_OPX_TXONLY_CAPS)

#define FI_OPX_DEFAULT_RX_CAPS (FI_OPX_BASE_CAPS | FI_OPX_RXONLY_CAPS)

#define FI_OPX_DEFAULT_MODE (FI_ASYNC_IOV)

/* OPX used to only have "default caps". The goal of this additional #define is to differentiate between what we
   "support" and what our "default" is
   TODO - Our default caps will be supplied to the application when they pass OPX no hints. The default caps should be
   our "optimal" configuration */
#define FI_OPX_SUPPORTED_CAPS (FI_OPX_DEFAULT_CAPS)

#if 0
/*
 * Use these when other FI_PROGRESS_* models are implemented.
 */
#ifndef OPX_PROGRESS
#define OPX_PROGRESS FI_PROGRESS_UNSPEC
#endif

#define IS_PROGRESS_MANUAL(domain_ptr)           \
	((OPX_PROGRESS == FI_PROGRESS_MANUAL) || \
	 ((OPX_PROGRESS == FI_PROGRESS_UNSPEC) && ((domain_ptr)->data_progress == FI_PROGRESS_MANUAL)))

#define IS_PROGRESS_AUTO(domain_ptr)           \
	((OPX_PROGRESS == FI_PROGRESS_AUTO) || \
	 ((OPX_PROGRESS == FI_PROGRESS_UNSPEC) && ((domain_ptr)->data_progress == FI_PROGRESS_AUTO)))
#else

#ifndef OPX_PROGRESS
#define OPX_PROGRESS FI_PROGRESS_UNSPEC
#endif

#define IS_PROGRESS_MANUAL(domain_ptr) (1)
#define IS_PROGRESS_AUTO(domain_ptr)   (0)
#endif

static inline void always_assert(bool val, char *msg)
{
	if (!val) {
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, "%s\n", msg);
		exit(EXIT_FAILURE);
	}
}

static inline void fi_opx_ref_init(int64_t *ref, char *name)
{
	*ref = 0;
	FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, "initializing ref count for (%s) to (%d)\n", name, 0);

	return;
}

static inline void fi_opx_ref_inc(int64_t *ref, char *name)
{
	(*ref) += 1;
	FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, "Incrementing ref count for (%s). New value is (%ld)\n", name,
	       (*ref));
	return;
}

static inline int fi_opx_ref_dec(int64_t *ref, char *name)
{
	int64_t value = --(*ref);
	if (value < 0) {
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, "decrement ref for (%s) (ref_cnt %ld < 0)\n", name, value);

		errno = FI_EOTHER;
		return -errno;
	}
	return 0;
}

static inline int fi_opx_ref_finalize(int64_t *ref, char *name)
{
	int64_t value = *ref;
	if (value != 0) {
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, "error ref for (%s) (ref_cnt %ld != 0)\n", name, value);
		errno = FI_EBUSY;
		return -errno;
	}
	return 0;
}

static inline int fi_opx_fid_check(fid_t fid, int fid_class, char *name)
{
	if (!fid) {
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, "NULL %s object", name);
		errno = FI_EINVAL;
		return -errno;
	}
	if (fid->fclass != fid_class) {
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, "wrong type of object (%s) expected (%d), got (%zu)\n", name,
		       fid_class, fid->fclass);
		errno = FI_EINVAL;
		return -errno;
	}
	return 0;
}

int fi_opx_set_default_info(void);

int fi_opx_check_info(const struct fi_info *info);

int fi_opx_fabric(struct fi_fabric_attr *attr, struct fid_fabric **fabric, void *context);

int fi_opx_check_fabric_attr(struct fi_fabric_attr *attr);

int fi_opx_domain(struct fid_fabric *fabric, struct fi_info *info, struct fid_domain **dom, void *context);

int fi_opx_check_domain_attr(struct fi_domain_attr *attr);
int fi_opx_choose_domain(uint64_t caps, struct fi_domain_attr *domain_attr, struct fi_domain_attr *hints,
			 enum fi_progress progress);

int fi_opx_alloc_default_domain_attr(struct fi_domain_attr **domain_attr);

int fi_opx_av_open(struct fid_domain *dom, struct fi_av_attr *attr, struct fid_av **av, void *context);

int fi_opx_cq_open(struct fid_domain *dom, struct fi_cq_attr *attr, struct fid_cq **eq, void *context);

void fi_opx_cq_finalize_ops(struct fid_ep *ep);

int fi_opx_endpoint(struct fid_domain *dom, struct fi_info *info, struct fid_ep **ep, void *context);

int fi_opx_alloc_default_ep_attr(struct fi_ep_attr **ep_attr);

int fi_opx_check_ep_attr(struct fi_ep_attr *attr);

int fi_opx_alloc_default_tx_attr(struct fi_tx_attr **tx_attr);
int fi_opx_check_tx_attr(struct fi_tx_attr *tx_attr, uint64_t hinted_caps);

int fi_opx_alloc_default_rx_attr(struct fi_rx_attr **rx_attr);
int fi_opx_check_rx_attr(struct fi_rx_attr *rx_attr, uint64_t hinted_caps);

int fi_opx_scalable_ep(struct fid_domain *dom, struct fi_info *info, struct fid_ep **ep, void *context);

int fi_opx_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr, struct fid_cntr **cntr, void *context);

int fi_opx_init_mr_ops(struct fid_domain *domain, struct fi_info *info);
int fi_opx_finalize_mr_ops(struct fid_domain *domain);

int fi_opx_init_rma_ops(struct fid_ep *ep, struct fi_info *info);
int fi_opx_enable_rma_ops(struct fid_ep *ep);
int fi_opx_finalize_rma_ops(struct fid_ep *ep);

int fi_opx_init_msg_ops(struct fid_ep *ep, struct fi_info *info);
int fi_opx_enable_msg_ops(struct fid_ep *ep);
int fi_opx_finalize_msg_ops(struct fid_ep *ep);

int fi_opx_init_atomic_ops(struct fid_ep *ep, struct fi_info *info);
int fi_opx_enable_atomic_ops(struct fid_ep *ep);
int fi_opx_finalize_atomic_ops(struct fid_ep *ep);

int fi_opx_init_tagged_ops(struct fid_ep *ep, struct fi_info *info);
int fi_opx_enable_tagged_ops(struct fid_ep *ep);
int fi_opx_finalize_tagged_ops(struct fid_ep *ep);

// int fi_opx_init_cm_ops(struct fi_opx_ep *opx_ep, struct fi_info *info);
int fi_opx_init_cm_ops(fid_t fid, struct fi_info *info);
// int fi_opx_finalize_cm_ops(struct fi_opx_ep *opx_ep);
int fi_opx_finalize_cm_ops(fid_t fid);

int fi_opx_bind_ep_cq(struct fid_ep *ep, struct fid_cq *cq, uint64_t flags);
int fi_opx_bind_ep_cntr(struct fid_ep *ep, struct fid_cntr *cntr, uint64_t flags);
int fi_opx_bind_ep_mr(struct fid_ep *ep, struct fid_mr *mr, uint64_t flags);
int fi_opx_bind_ep_av(struct fid_ep *ep, struct fid_av *av, uint64_t flags);

#endif /* __FI_PROV_OPX_H__ */

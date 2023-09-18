/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2023 by Cornelis Networks.
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

// #define FI_OPX_TRACE 1

#define FI_OPX_IOV_LIMIT (1)

/* --- Will be exposed by fabric.h */
#define FI_OPX_PROTOCOL		0x0008
#define FI_OPX_PROTOCOL_VERSION (1)
/* --- end */

#define FI_OPX_PROVIDER_NAME		"opx"
// Optimized builds will crash if the provider version is something other than this
// So it's hard to communicate that the Opx provider is PRE-GA via the version number
// Delete this comment when OPX is GA-ed
#define FI_OPX_PROVIDER_VERSION	(OFI_VERSION_DEF_PROV)
#define FI_OPX_DEVICE_MAX_PATH_NAME	(32)
#define FI_OPX_FABRIC_NAME		"OPX-100"
#define FI_OPX_DOMAIN_NAME		"ib0"
#define FI_OPX_DOMAIN_NAME_PREFIX	"ib"

#define FI_OPX_CACHE_LINE_SIZE	(64)

#define FI_OPX_MAX_STRLEN		(64)

#define EXIT_FAILURE 1

// TODO: This is needed until we complete the locking model. 
#define OPX_LOCK				0

// These defines may change or disappear as we develop more capabilities.
#define OPX_CQ_FORMAT			FI_CQ_FORMAT_TAGGED
#define OPX_RELIABILITY		OFI_RELIABILITY_KIND_ONLOAD
#define OPX_MASK				FI_OPX_HDRQ_MASK_RUNTIME
#define OPX_CQ_CAPS			0x0000000000000000ull
#define OPX_EP_CAPS			0x0018000000000000ull
#define OPX_TAGGED_CAPS		0x0018000000000000ull

// Uncomment to enabled Opx flight recorder
//#define FLIGHT_RECORDER_ENABLE		(1)

extern struct fi_provider fi_opx_provider;
struct fi_opx_daos_hfi_rank_key {
	uint8_t		hfi_unit_number;
	uint32_t	rank;
};

struct fi_opx_daos_hfi_rank {
	struct fi_opx_daos_hfi_rank_key key;
	uint32_t instance;
	UT_hash_handle 	hh;         /* makes this structure hashable */
};

struct fi_opx_global_data {
	struct fi_info		*info;
	struct fi_domain_attr	*default_domain_attr;
	struct fi_ep_attr	*default_ep_attr;
	struct fi_tx_attr	*default_tx_attr;
	struct fi_rx_attr	*default_rx_attr;
	struct fi_provider 	*prov;
	struct fi_opx_daos_hfi_rank	*daos_hfi_rank_hashmap;
	enum fi_progress	progress;
};

extern struct fi_opx_global_data fi_opx_global;

static const uint64_t FI_OPX_MAX_MSG_SIZE		= ((uint64_t)-1);
static const uint64_t FI_OPX_MAX_PREFIX_SIZE		= (0ULL);
static const uint64_t FI_OPX_MAX_ORDER_RAW_SIZE	= SIZE_MAX;
static const uint64_t FI_OPX_MAX_ORDER_WAR_SIZE	= SIZE_MAX;
static const uint64_t FI_OPX_MAX_ORDER_WAW_SIZE	= SIZE_MAX;
static const size_t   FI_OPX_REMOTE_CQ_DATA_SIZE	= 4;

static const uint64_t FI_OPX_MEM_TAG_FORMAT		= (0xFFFFFFFFFFFFFFFFULL);
static const int      FI_OPX_MAX_HFIS				= 16;

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
static const uint64_t FI_OPX_HDRQ_MASK_RUNTIME	= 0ULL;
static const uint64_t FI_OPX_HDRQ_MASK_2048 		= 0X000000000000FFE0UL;
static const uint64_t FI_OPX_HDRQ_MASK_8192		= 0X000000000003FFE0UL;


#define FI_OPX_DEFAULT_MSG_ORDER										\
		(FI_ORDER_ATOMIC_RAR | FI_ORDER_ATOMIC_RAW | FI_ORDER_ATOMIC_WAR | FI_ORDER_ATOMIC_WAW)

#define FI_OPX_TXONLY_CAPS							\
	( FI_SEND | FI_READ | FI_WRITE | FI_REMOTE_WRITE )

#define FI_OPX_RXONLY_CAPS							\
	( FI_RECV | FI_DIRECTED_RECV | FI_MULTI_RECV | FI_REMOTE_READ )

#define FI_OPX_BASE_CAPS							\
	( FI_MSG | FI_TAGGED | FI_LOCAL_COMM | FI_REMOTE_COMM	\
	| FI_SOURCE | FI_NAMED_RX_CTX | FI_RMA | FI_ATOMIC )

#define FI_OPX_DEFAULT_CAPS							\
	(FI_OPX_BASE_CAPS | FI_OPX_TXONLY_CAPS | FI_OPX_RXONLY_CAPS)

#define FI_OPX_DEFAULT_TX_CAPS						\
	(FI_OPX_BASE_CAPS | FI_OPX_TXONLY_CAPS)

#define FI_OPX_DEFAULT_RX_CAPS						\
	(FI_OPX_BASE_CAPS | FI_OPX_RXONLY_CAPS)

#define FI_OPX_DEFAULT_MODE							\
	(FI_CONTEXT2 | FI_ASYNC_IOV)



#if 0
/*
 * Use these when other FI_PROGRESS_* models are implemented.
 */
#ifndef OPX_PROGRESS
#define OPX_PROGRESS FI_PROGRESS_UNSPEC
#endif

#define IS_PROGRESS_MANUAL(domain_ptr)						\
	((OPX_PROGRESS == FI_PROGRESS_MANUAL) ||			\
	((OPX_PROGRESS == FI_PROGRESS_UNSPEC) &&			\
		((domain_ptr)->data_progress == FI_PROGRESS_MANUAL)))

#define IS_PROGRESS_AUTO(domain_ptr)						\
	((OPX_PROGRESS == FI_PROGRESS_AUTO) ||			\
	((OPX_PROGRESS == FI_PROGRESS_UNSPEC) &&			\
		((domain_ptr)->data_progress == FI_PROGRESS_AUTO)))
#else

#ifndef OPX_PROGRESS
#define OPX_PROGRESS FI_PROGRESS_UNSPEC
#endif

#define IS_PROGRESS_MANUAL(domain_ptr)	(1)
#define IS_PROGRESS_AUTO(domain_ptr)	(0)
#endif

static inline void always_assert (bool val, char *msg)
{
	if (!val) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"%s\n", msg);
		exit(EXIT_FAILURE);
	}
}

// Useful for checking that structures are the correct size and other
// compile-time tests. static_assert has existed since C11 so this
// should be safe, but we have an else clause just in case.
#if defined(static_assert)
#define OPX_COMPILE_TIME_ASSERT(cond, msg) static_assert(cond,msg)
#else
#define OPX_COMPILE_TIME_ASSERT(cond, msg) if(0){switch(0){case 0:case cond:;}}
#endif

static inline void fi_opx_ref_init (int64_t *ref, char *name)
{
	*ref = 0;
	FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
			"initializing ref count for (%s) to (%d)\n",
			name, 0);

	return;
}

static inline void fi_opx_ref_inc (int64_t *ref, char *name)
{
	(*ref) += 1;
	return;
}

static inline int fi_opx_ref_dec (int64_t *ref, char *name)
{
	int64_t value = --(*ref);
	if (value < 0) {

		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC,
			"decrement ref for (%s) (ref_cnt %ld < 0)\n",
			name, value);

		errno = FI_EOTHER;
		return -errno;
	}
	return 0;
}

static inline int fi_opx_ref_finalize (int64_t *ref, char *name)
{
	int64_t value = *ref;
	if (value != 0) {
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC,
			"error ref for (%s) (ref_cnt %ld != 0)\n",
			name, value);
		errno = FI_EBUSY;
		return -errno;
	}
	return 0;
}

static inline int fi_opx_fid_check (fid_t fid, int fid_class, char *name)
{
	if (!fid) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
				"NULL %s object", name);
		errno = FI_EINVAL;
		return -errno;
	}
	if (fid->fclass != fid_class) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_FABRIC,
			"wrong type of object (%s) expected (%d), got (%zu)\n",
			name, fid_class, fid->fclass);
		errno = FI_EINVAL;
		return -errno;
	}
	return 0;
}

int fi_opx_set_default_info(void);

int fi_opx_check_info(const struct fi_info *info);

int fi_opx_fabric(struct fi_fabric_attr *attr,
		struct fid_fabric **fabric, void *context);

int fi_opx_check_fabric_attr(struct fi_fabric_attr *attr);

int fi_opx_domain(struct fid_fabric *fabric,
		struct fi_info *info,
		struct fid_domain **dom, void *context);

int fi_opx_check_domain_attr(struct fi_domain_attr *attr);
int fi_opx_choose_domain(uint64_t caps,
		struct fi_domain_attr *domain_attr,
		struct fi_domain_attr *hints,
		enum fi_progress progress);

int fi_opx_alloc_default_domain_attr(struct fi_domain_attr **domain_attr);

int fi_opx_av_open(struct fid_domain *dom,
		struct fi_av_attr *attr, struct fid_av **av,
		void *context);

int fi_opx_stx_context(struct fid_domain *domain, struct fi_tx_attr *attr,
                         struct fid_stx **stx, void *context);

int fi_opx_cq_open(struct fid_domain *dom,
		struct fi_cq_attr *attr,
		struct fid_cq **eq, void *context);

void fi_opx_cq_finalize_ops(struct fid_ep *ep);

int fi_opx_endpoint(struct fid_domain *dom, struct fi_info *info,
		struct fid_ep **ep, void *context);

int fi_opx_alloc_default_ep_attr(struct fi_ep_attr **ep_attr);

int fi_opx_check_ep_attr(struct fi_ep_attr *attr);

int fi_opx_alloc_default_tx_attr(struct fi_tx_attr **tx_attr);
int fi_opx_check_tx_attr(struct fi_tx_attr *attr);

int fi_opx_alloc_default_rx_attr(struct fi_rx_attr **rx_attr);
int fi_opx_check_rx_attr(struct fi_rx_attr *attr);

int fi_opx_scalable_ep(struct fid_domain *dom, struct fi_info *info,
		struct fid_ep **ep, void *context);

int fi_opx_cntr_open(struct fid_domain *domain,
		struct fi_cntr_attr *attr,
		struct fid_cntr **cntr, void *context);

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

//int fi_opx_init_cm_ops(struct fi_opx_ep *opx_ep, struct fi_info *info);
int fi_opx_init_cm_ops(fid_t fid, struct fi_info *info);
//int fi_opx_finalize_cm_ops(struct fi_opx_ep *opx_ep);
int fi_opx_finalize_cm_ops(fid_t fid);

int fi_opx_bind_ep_cq(struct fid_ep *ep,
		struct fid_cq *cq, uint64_t flags);
int fi_opx_bind_ep_cntr(struct fid_ep *ep,
		struct fid_cntr *cntr, uint64_t flags);
int fi_opx_bind_ep_mr(struct fid_ep *ep,
		struct fid_mr *mr, uint64_t flags);
int fi_opx_bind_ep_av(struct fid_ep *ep,
		struct fid_av *av, uint64_t flags);

#endif /* __FI_PROV_OPX_H__ */

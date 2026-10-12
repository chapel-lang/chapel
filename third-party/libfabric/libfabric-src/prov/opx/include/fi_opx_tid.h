/*
 * Copyright (C) 2022-2024 Cornelis Networks.
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

#ifndef _FI_PROV_OPX_TID_H_
#define _FI_PROV_OPX_TID_H_

#include "opa_user_gen1.h" /* opx_hfi_free_tid */
#include "rdma/opx/fi_opx_compiler.h"
#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/fi_opx_debug_counters.h"

/* TID info and TID pairs as OPX understands it (see HAS)
 *
 * Update tid returns a list of TID<->contiguous pages pinned in
 * TID info.  They have LEN (pages), CTRL(1 or 2) and IDX.
 * TID info is saved for Free tid.
 *
 * Data transfer can support a "pair" of TIDs using the same IDX
 * in a header by combining CTRL 1 and CTRL 2 into CTRL3 in
 * the header.
 *
 * The receiver will build the list of tid "pairs", and save them
 * for efficiency, and send them to the sender on CTS.
 *
 * A logging example:
 *
 *  Call to update tid for 16 pages, returned tidinfo
 *
 *   tidinfo[0]=0X00100001 LEN 1, CTRL 1, IDX 0
 *   tidinfo[1]=0X00200008 LEN 8, CTRL 2, IDX 0
 *   tidinfo[2]=0X00500004 LEN 4, CTRL 1, IDX 1
 *   tidinfo[3]=0X00600002 LEN 2, CTRL 2, IDX 1
 *   tidinfo[4]=0X00900001 LEN 1, CTRL 1, IDX 2
 *
 *  Converted to CTRL 3 tidpairs for CTS and eventual SDMA RZV DATA:
 *
 *   tidpair[0]=0X00300009 LEN 9, CTRL 3, IDX 0
 *   tidpair[1]=0X00700006 LEN 6, CTRL 3, IDX 1
 *   tidpair[2]=0X00900001 LEN 1, CTRL 1, IDX 2
 *
 *   IMPORTANT NOTES:
 *
 *   TID update limits the input number of pages
 *       to currently available TID entries even
 *       though more could be supported.
 *       eg. 10 TID entries are available.
 *       Only 10 pages may be updated.
 *       TID update returns a single TID info for
 *       all 10 pages.  Another call for 9 pages
 *       may not be attempted and likely succeed
 *       maybe returning 1 contiguous TID (CTRL 2).
 *
 *       Unfortunately, even though it would work,
 *       a single 19 page TID update would get EINVAL.
 *
 *       This is referred to as chunking in code
 *       comments.  OPX does not chunk (multiple
 *       calls) but could be enhanced.  OPX would
 *       try 19 pages, get EINVAL, and fallback
 *       to PIO.
 *
 *   TID info's are not always paired.
 *
 *   TID info's that are paired *appear* to always be
 *       returned in order (CTRL1, CTRL2, CTRL1, CTRL1)
 *       from a TID update.
 *
 *   CTRL 1 and CTRL 2 may appear alone in different
 *       TID updates.  CTRL 1 on the first update and
 *       CTRL 2 on a later update. These are not paired.
 *
 *   TID info's that exceed 512 pages may not be paired
 *       even though they use the same IDX.  Header
 *       offsets are limited to < 512 pages.
 *
 */

/* The max for both eager and expected receive is 2048 entries, which can
   be software partitioned between the eager receive and expected receive
   mechanisms on a per-context basis.
   We use this for a TID cache max size (and validate against
   ctx->__hfi_tidexpcnt)  */
#define OPX_MAX_TID_COUNT 2048

#define OPX_TID_PAIR_MAX_PAGES (512)

#define OPX_TID_NPAGES(tid_reuse_cache, npages)                                                                     \
	do {                                                                                                        \
		npages		      = 0;                                                                          \
		const uint32_t *tids  = &tid_reuse_cache->info[0];                                                  \
		const uint32_t	ntids = tid_reuse_cache->ninfo;                                                     \
		for (int i = 0; i < ntids; ++i) {                                                                   \
			npages += (int) FI_OPX_EXP_TID_GET(tids[i], LEN);                                           \
			FI_DBG(fi_opx_global.prov, FI_LOG_MR, "%s[%u]=%#8.8X LEN %u, CTRL %u, IDX %u, npages %u\n", \
			       "OPX_TID_NPAGES", i, tids[i], (int) FI_OPX_EXP_TID_GET(tids[i], LEN),                \
			       (int) FI_OPX_EXP_TID_GET(tids[i], CTRL), (int) FI_OPX_EXP_TID_GET(tids[i], IDX),     \
			       npages);                                                                             \
		}                                                                                                   \
	} while (0)

#ifndef NDEBUG
/* Dump debug tidinfo or tidpairs */
#define OPX_DEBUG_TIDS(string, ntids, in_tids)                                                                      \
	do {                                                                                                        \
		int		npages = 0;                                                                         \
		const uint32_t *tids   = in_tids;                                                                   \
		for (int i = 0; i < ntids; ++i) {                                                                   \
			npages += (int) FI_OPX_EXP_TID_GET(tids[i], LEN);                                           \
			FI_DBG(fi_opx_global.prov, FI_LOG_MR, "%s[%u]=%#8.8X LEN %u, CTRL %u, IDX %u, npages %u\n", \
			       string, i, tids[i], (int) FI_OPX_EXP_TID_GET(tids[i], LEN),                          \
			       (int) FI_OPX_EXP_TID_GET(tids[i], CTRL), (int) FI_OPX_EXP_TID_GET(tids[i], IDX),     \
			       npages);                                                                             \
		}                                                                                                   \
	} while (0)
#else
#define OPX_DEBUG_TIDS(string, ntids, tids)
#endif

#ifdef OPX_TID_CACHE_DEBUG
/* Dump debug tidinfo or tidpairs */
#define OPX_FPRINTF_TIDS(string, ntids, in_tids)                                                                     \
	do {                                                                                                         \
		int		npages = 0;                                                                          \
		const uint32_t *tids   = in_tids;                                                                    \
		for (int i = 0; i < ntids; ++i) {                                                                    \
			npages += (int) FI_OPX_EXP_TID_GET(tids[i], LEN);                                            \
			fprintf(stderr, "## %s[%u]=%#8.8X LEN %u, CTRL %u, IDX %u, npages %u\n", string, i, tids[i], \
				(int) FI_OPX_EXP_TID_GET(tids[i], LEN), (int) FI_OPX_EXP_TID_GET(tids[i], CTRL),     \
				(int) FI_OPX_EXP_TID_GET(tids[i], IDX), npages);                                     \
		}                                                                                                    \
	} while (0)
#else
#define OPX_FPRINTF_TIDS(string, ntids, in_tids)
#endif

#ifdef OPX_TID_CACHE_DEBUG /* NOT dependent on NDEBUG */
static inline void OPX_TID_CACHE_DEBUG_FPRINTF(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	return;
}
#else
static inline void OPX_TID_CACHE_DEBUG_FPRINTF(const char *format, ...)
{
	return;
}
#endif

/* Special debug for expected receive rts ONLY */
#ifdef OPX_TID_CACHE_DEBUG /* NOT dependent on NDEBUG */
/* Ugly but it's debug... only print when something changes (less noisy) */
#define OPX_TID_CACHE_RZV_RTS(tid_reuse_cache, string)                                                                \
	do {                                                                                                          \
		static int	count	    = 0;                                                                      \
		static uint64_t last_vaddr  = 0UL;                                                                    \
		static int32_t	last_length = 0;                                                                      \
		if ((last_vaddr != tid_vaddr) || (last_length != tid_length)) {                                       \
			fprintf(stderr,                                                                               \
				"## %s:%u OPX_TID_CACHE_DEBUG %s TIDs "                                               \
				"input vaddr [%#lx - %#lx] length %lu, "                                              \
				"tid   vaddr [%#lx - %#lx] length %lu, "                                              \
				"last count %u\n",                                                                    \
				__func__, __LINE__, string, tid_vaddr, tid_vaddr + tid_length, tid_length,            \
				tid_reuse_cache->tid_vaddr, tid_reuse_cache->tid_vaddr + tid_reuse_cache->tid_length, \
				tid_reuse_cache->tid_length, count);                                                  \
			last_vaddr  = tid_vaddr;                                                                      \
			last_length = tid_length;                                                                     \
			count	    = 0;                                                                              \
		}                                                                                                     \
		++count;                                                                                              \
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,                                                                 \
		       "OPX_TID_CACHE_DEBUG %s TIDs "                                                                 \
		       "input vaddr [%#lx - %#lx] length %lu, "                                                       \
		       "tid   vaddr [%#lx - %#lx] length %lu\n",                                                      \
		       string, tid_vaddr, tid_vaddr + tid_length, tid_length, tid_reuse_cache->tid_vaddr,             \
		       tid_reuse_cache->tid_vaddr + tid_reuse_cache->tid_length, tid_reuse_cache->tid_length);        \
	} while (0)
#else
/* noisier regular debug logging */
#define OPX_TID_CACHE_RZV_RTS(tid_reuse_cache, string)                                            \
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,                                                     \
	       "OPX_TID_CACHE_DEBUG %s TIDs "                                                     \
	       "input vaddr [%#lx - %#lx] length %lu, "                                           \
	       "tid   vaddr [%#lx - %#lx] length %lu\n",                                          \
	       string, tid_vaddr, tid_vaddr + tid_length, tid_length, tid_reuse_cache->tid_vaddr, \
	       tid_reuse_cache->tid_vaddr + tid_reuse_cache->tid_length, tid_reuse_cache->tid_length);
#endif

/* Special debug for expected receive data ONLY */
#ifdef OPX_TID_CACHE_VERBOSE_DEBUG /* NOT dependent on NDEBUG */
#define OPX_TID_CACHE_RZV_DATA(tid_reuse_cache, string)                                                                \
	do { /* Ugly but it's debug... */                                                                              \
		static int	count	    = 0;                                                                       \
		static uint64_t last_vaddr  = 0UL;                                                                     \
		static int32_t	last_length = 0;                                                                       \
		if ((last_vaddr != tid_reuse_cache->tid_vaddr) || (last_length != tid_reuse_cache->tid_length)) {      \
			fprintf(stderr,                                                                                \
				"## %s:%u OPX_TID_CACHE_VERBOSE_DEBUG %s TIDs "                                        \
				"input vaddr [%#lx - %#lx] length %lu, "                                               \
				"tid   vaddr [%#lx - %#lx] length %u, "                                                \
				"last count %u\n",                                                                     \
				__func__, __LINE__, string, tid_reuse_cache->tid_vaddr,                                \
				tid_reuse_cache->tid_vaddr + tid_reuse_cache->tid_length, tid_reuse_cache->tid_length, \
				last_vaddr, last_vaddr + last_length, last_length, count);                             \
			last_vaddr  = tid_reuse_cache->tid_vaddr;                                                      \
			last_length = tid_reuse_cache->tid_length;                                                     \
			count	    = 0;                                                                               \
		}                                                                                                      \
		++count;                                                                                               \
	} while (0)
#else
#define OPX_TID_CACHE_RZV_DATA(tid_reuse_cache, string)                                                      \
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,                                                                \
	       "OPX_TID_CACHE_VERBOSE_DEBUG %s TIDs "                                                        \
	       "tid   vaddr [%#lx - %#lx] length %lu\n",                                                     \
	       string, tid_reuse_cache->tid_vaddr, tid_reuse_cache->tid_vaddr + tid_reuse_cache->tid_length, \
	       tid_reuse_cache->tid_length)
#endif

/* Structure overview
 *
 * OPX has a cache of memory regions which may be used for RZV
 *
 *      struct ofi_mr_cache *cache
 *
 * The OPX cache is currently attached to a TID domain linked to
 * the endpoint for convenience. (It will later move to endpoints)
 *
 *      cache = opx_ep->tid_domain->tid_cache;
 *
 * Each memory region has an entry in the cache which can be found
 * with the page aligned memory region [virtual address + length]
 *
 *      struct ofi_mr_entry *entry
 *
 *      struct ofi_mr_info info;
 *	info.iov.iov_base = (void *) tid_vaddr;
 *      info.iov.iov_len = tid_length;
 *
 * 	opx_tid_cache_find(opx_ep, &info, &entry, 0);
 *
 * The entry data is the OPX TID memory region (mr)
 *
 *      struct opx_tid_mr *opx_mr = (struct opx_tid_mr *)entry->data;
 *
 * The TID memory region (mr) has TID info for that mr that is
 * registered/ioctl(update) and deregistered/ioctl(free)
 *
 *      struct opx_mr_tid_info * tid_info = &opx_mr->tid_info;
 *
 * A RZV buffer may be composed of multiple entries/mr's/tid_info's that
 * are reference counted (use_cnt) and registered/deregistered separately.
 *
 */
struct opx_mr_tid_info {
	uint64_t tid_vaddr;
	uint64_t tid_length;
	uint32_t ninfo;
	uint32_t npairs;

	/* tidinfo is used on TID update/free ioctl */
	uint32_t info[FI_OPX_MAX_DPUT_TIDPAIRS];

	/* tidpairs combine CTRL 1 & 2 into CTRL 3 tidpairs for SDMA use */
	uint32_t pairs[FI_OPX_MAX_DPUT_TIDPAIRS];
};

struct opx_tid_mr {
	struct opx_tid_domain *domain;
	struct fi_opx_ep      *opx_ep;
	struct opx_mr_tid_info tid_info;
};

struct opx_tid_addr_block {
	struct iovec target_iov;
	uint32_t     offset;
	uint32_t     npairs;
	uint32_t     pairs[FI_OPX_MAX_DPUT_TIDPAIRS];
};

/*
 * Minimum page sizes to use for different memory types.
 * The array is indexed by the values defined in
 * enum fi_hmem_iface. Some values are not supported.
 */
static const uint64_t OPX_TID_PAGE_SIZE[4] = {
	PAGE_SIZE, /* FI_HMEM_SYSTEM */
	64 * 1024, /* FI_HMEM_CUDA   */
	PAGE_SIZE, /* FI_HMEM_ROCR   */
	PAGE_SIZE  /* FI_HMEM_ZE     */
};

__OPX_FORCE_INLINE__
void opx_tid_range(const struct fi_opx_hmem_iov *in_range, struct fi_opx_hmem_iov *out_range)
{
	assert(in_range->len > 0);

	// TID Start is the buf address rounded down to the previous page boundary
	uintptr_t tid_start = in_range->buf & -((int64_t) OPX_TID_PAGE_SIZE[in_range->iface]);
	assert(tid_start <= in_range->buf);

	// TID End is the buf address rounded up to the next page boundary
	uintptr_t tid_end = (in_range->buf + in_range->len + (OPX_TID_PAGE_SIZE[in_range->iface] - 1)) &
			    -((int64_t) OPX_TID_PAGE_SIZE[in_range->iface]);
	assert(tid_end >= (in_range->buf + in_range->len));

	out_range->buf	  = tid_start;
	out_range->len	  = tid_end - tid_start;
	out_range->iface  = in_range->iface;
	out_range->device = in_range->device;
}

__OPX_FORCE_INLINE__
void opx_tid_inc_bucket_counts(struct fi_opx_debug_counters *counters, const uint32_t tid_pair, const size_t len_pages)
{
#ifdef OPX_DEBUG_COUNTERS_EXPECTED_RECEIVE
	FI_OPX_DEBUG_COUNTERS_INC_COND((len_pages == 1), counters->expected_receive.tid_buckets[0]);
	FI_OPX_DEBUG_COUNTERS_INC_COND((len_pages == 2), counters->expected_receive.tid_buckets[1]);
	FI_OPX_DEBUG_COUNTERS_INC_COND((len_pages > 2 && len_pages < 128), counters->expected_receive.tid_buckets[2]);
	FI_OPX_DEBUG_COUNTERS_INC_COND((len_pages >= 128), counters->expected_receive.tid_buckets[3]);
#endif
}

__OPX_FORCE_INLINE__
void opx_tid_regen_pairs(const uint32_t tid_length, const uint32_t npairs_in, const uint32_t *const pairs_in,
			 uint32_t *npairs_out, uint32_t *pairs_out, struct fi_opx_debug_counters *counters)
{
	int32_t tid_idx	 = 1;
	int32_t pair_idx = 0;

	OPX_DEBUG_TIDS("Input tidinfo", npairs_in, pairs_in);

	uint64_t prev_ctrl	 = FI_OPX_EXP_TID_GET(pairs_in[0], CTRL);
	size_t	 tid_pages	 = FI_OPX_EXP_TID_GET(pairs_in[0], LEN);
	size_t	 accumulated_len = tid_pages * OPX_HFI1_TID_PAGESIZE;
	size_t	 prev_len	 = tid_pages;

	opx_tid_inc_bucket_counts(counters, pairs_in[0], tid_pages);
	pairs_out[0] = pairs_in[0];

	/* Combine ctrl 1/2 tids into single ctrl 3 tid pair */
	while ((tid_idx < npairs_in) && (accumulated_len < tid_length)) {
		opx_tid_inc_bucket_counts(counters, pairs_in[tid_idx], tid_pages);

		size_t	 tid_pages     = FI_OPX_EXP_TID_GET(pairs_in[tid_idx], LEN);
		size_t	 tid_pages_len = tid_pages * OPX_HFI1_TID_PAGESIZE;
		uint64_t tid_ctrl      = FI_OPX_EXP_TID_GET(pairs_in[tid_idx], CTRL);
		accumulated_len += tid_pages_len;

		/* Can't combine into CTRL 3 if :
			- not the same IDX or
			- previous was not CTRL 1 or
			- combined LEN > 512

		   Offset field (OFFSET): For expected receive packets this
		   offset is added to the address field associated with the
		   specified TID to determine a physical address. This physical
		   address is then used to DMA the data portion of the received
		   packet to system memory. If OM is 0 the 15-bit OFFSET can
		   address a 128KB mapping in DW multiples. If OM is 1 the
		   15-bit OFFSET can address a 2MB mapping in 64B multiples.

		   512 pages is 2MB.  So even if a "tid pair" *seems* to be
		   available, it won't work over 512 pages so keep ctrl 1 tid
		   and ctrl 2 tid separate, do not optimize into ctrl 3 tidpair
		*/
		if (tid_ctrl == 2 && prev_ctrl == 1 && (prev_len + tid_pages) <= OPX_TID_PAIR_MAX_PAGES &&
		    (FI_OPX_EXP_TID_GET(pairs_in[tid_idx - 1], IDX) == FI_OPX_EXP_TID_GET(pairs_in[tid_idx], IDX))) {
			FI_OPX_EXP_TID_RESET(pairs_out[pair_idx], CTRL, 0x3);
			FI_OPX_EXP_TID_RESET(pairs_out[pair_idx], LEN, tid_pages + prev_len);
		} else {
			pair_idx++;
			pairs_out[pair_idx] = pairs_in[tid_idx];
		}
		prev_ctrl = tid_ctrl;
		prev_len  = tid_pages;
		tid_idx++;
		FI_DBG(fi_opx_global.prov, FI_LOG_MR, "tid_idx %u, ntidinfo %u, accumulated_len %zu, tid_length %u\n",
		       tid_idx, npairs_in, accumulated_len, tid_length);
	}
	assert(accumulated_len >= tid_length);
#ifdef OPX_DEBUG_COUNTERS_EXPECTED_RECEIVE
	uint32_t first_pair_len = FI_OPX_EXP_TID_GET(pairs_out[0], LEN);
	FI_OPX_DEBUG_COUNTERS_INC_COND_N((counters->expected_receive.first_tidpair_minlen == 0), first_pair_len,
					 counters->expected_receive.first_tidpair_minlen);
	FI_OPX_DEBUG_COUNTERS_MIN_OF(counters->expected_receive.first_tidpair_minlen, first_pair_len);
	FI_OPX_DEBUG_COUNTERS_MAX_OF(counters->expected_receive.first_tidpair_maxlen, first_pair_len);
#endif
	(*npairs_out) = pair_idx + 1;
}

#endif /* _FI_PROV_OPX_TID_H_ */

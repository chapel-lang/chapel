/*
 * Copyright (C) 2022 Cornelis Networks.
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
#include "fi_opx_tid_cache.h"
/*
 * Expected receive (TID) buffer/length alignment
 * ==============================================
 * Goals/notes:
 *
 *  - Pin full pages
 *  - Do not overlap pinning when 2 buffers share a page
 *  - - TID update does not allow double update/pin
 *  - Start SDMA AHG on page boundaries
 *  - - for simplicity and for overlap
 *  - SDMA AHG lengths must be 64b aligned and NOT padded
 *  - - padding, such as SDMA PIO does, would be RDMA'd over non-buffer data
 *  - Send leading (4k) and trailing (64b) data to allow alignment
 *  - SDMA AHG will RDMA the adjusted buffer so...
 *  - assert adjusted buffer boundaries fall in "immediate" ranges
 *
 * Sender sends enough immediate data to allow receiver to align buffers and lengths
 * and so that buffers do not overlap TID update/pin.  Examples:
 *
 * Page boundaries         : |    |    |    |    |    |    |    |    |    |    |    |    |
 * 1st User buffer         :         |uuuuuuuuuuuuuuuuuuuuuuu|
 * Immediate block         :         |uuuu|
 * Immediate trailing bytes:                               |u|
 * Adjusted User buffer    :           |uuuuuuuuuuuuuuuuuuuu|
 *
 * TID Pinned pages        : |    |    |pppp|pppp|pppp|pppp|pppp|    |    |    |    |    |
 *
 * A 2nd buffer will pin non-overlapping AFTER the 1st
 *
 * 2nd User buffer         :                                  |UUUUUUUUUUUUUUUUUUUUUU|
 * Immediate block         :                                  |UUUU|
 * Immediate trailing bytes:                                                       |U|
 * Adjusted User buffer    :                                    |UUUUUUUUUUUUUUUUUUU|
 *
 * TID Pinned pages        : |    |    |pppp|pppp|pppp|pppp|pppp|PPPP|PPPP|PPPP|PPPP|    |
 *
 * A 3rd buffer will pin non-overlapping BEFORE the 1st
 *
 * 3rd User buffer         :xxxxxxxx|
 * Immediate block         : <==not shown to the left
 * Immediate trailing bytes:      |x|
 * Adjusted User buffer    :xxxxxxx|
 *
 * TID Pinned pages        :Q|QQQQ|QQQQ|pppp|pppp|pppp|pppp|pppp|PPPP|PPPP|PPPP|PPPP|    |
 *
 * ONLY APPEND (2nd buffer) UPDATE IS CURRENTLY SUPPORTED.
 * CURRENTLY WE DO NOT ALLOW DISJOINT (not shown) OR PREPENDING (3rd buffer) UPDATES.
 *
 * We could add prepending but disjoint needs a full TID cache (future work).
 */

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

/* SDMA AHG (auto header generation) quirks in the OPX design
 *
 * OPX uses EXPECTED SDMA writes but reliability replay
 * will be EAGER/PIO packets without TID information in the
 * headers.
 *
 *
 * SDMA EAGER will pad writes on the last packet and the
 * receiver uses a header field to memcpy only the data
 * without padding.  SDMA EXPECTED can NOT do this as
 * the padding would be RDMA'd into user memory.
 *
 * enum sdma_req_opcode {
 *      EXPECTED = 0,
 *      EAGER
 *
 * An EXPECTED sdma write uses an additional iovec with the
 * list of TID pairs to use on the transfer.
 *
 * OPX is responsible for creating a valid header for ths
 * first packet.  SDMA AHG will use the TID pairs to build
 * the remaining headers. Each packet is max "fragsize"
 * but may be smaller if the current TID pair LEN is smaller
 * (meaning 1 == 4K page in OPX implementation).
 *
 * As a result, OPX must "walk" the TID pairs and simulate
 * what we expect AHG to generate and setup our PIO
 * replay packets to match and so that we know how
 * many PSN's are used.
 *
 * On the receiver, the data is RDMA'd into memory and
 * OPX still receives the header with a NULL payload. The
 * header is used to count bytes towards completion only.
 *
 * If a expected TID packet is dropped, it will be replayed
 * EAGER/PIO without TID fields and will be memcpy/counted
 * as any EAGER packet.
 *
 * Any EXPECTED packets that arrive out of order, after a
 * dropped packet, will be queued in OPX which sets a
 * non-NULL payload pointer despite there being no payload.
 * This is handled in rzv_data.
 */

/* TID FLows and PSN generation/sequence numbers
 *
 * The TID flow field is 0 in our headers which is a valid
 * TID flow (0-31) but we clear (PSM2 leaves garbage) and
 * do not enable the TID flows so it is effectively
 * ignored on the receiver.
 *
 * However the sender will still wrap sequence numbers
 * every 2048 PSN's during AHG.  AHG will not increment
 * the generation.
 *
 * As a result (OPX only actually writes the first PSN
 * but it calculates how many PSN's are used in the SDMA
 * write) :
 *
 * (wrong)
 * OPX writes packets PSN 2040 - 2050.
 * SDMA AHG will write headers 2040-2047, 0-2.
 * Packets 0-2 will be dropped and replayed EAGER/PIO.
 *
 * (OPX design)
 * OPX writes packets PSN 2040 - 2047
 * OPX writes packets PSN 2048 - 2050
 * SDMA AHG uses the expected PSNs as it appears to
 * be a valid generation increment.
 *
 * This is another reason OPX "walks" the TIDs and
 * simulates what AHG is expected to do.  This results
 * in less than max packets (32) on some SDHMA writes.
 */

/* TID reuse cache (1 entry)
 *
 * OPX does not (yet) support full caching of TID memory
 * regions and tidinfo.
 *
 * This is a simple tool for performance testing of
 * caching/non-caching rendezvous.
 *
 * This is a reuse cache of 1 entry defined by a
 * starting virtual address (vaddr) and length.
 *
 * - vaddr is page aligned.
 * - length is in page increments
 *
 * It is only used with a specific vaddr since TID info
 * doesn't carry memory address information and using
 * any other vaddr in the region would require walking
 * the TID pairs to find the starting TID pair AND
 * starting offset in that TID pair. (Starting offset
 * in CTS is currently assumed to be page aligned).
 * It may only be appended (grown) with a new memory
 * region starting on the same vaddr but longer.
 *
 * It is freed when a different memory region is used.
 *
 * It *could* support prepend/subsets which would require
 * walking the TID's to find the starting address for
 * any vaddr.  Performance measurement will drive whether
 * this changes or if a full MR cache is implemented.
 *
 */

#include "rdma/opx/fi_opx_hfi1.h"


struct fi_opx_tid_reuse_cache {
	uint64_t                                refcount;
	uint64_t                                vaddr;
	uint64_t                                length;
	uint32_t				ninfo;
	uint32_t				npairs;
	uint32_t				invalid; /* mmu notify */
	/* tidinfo is used on TID update/free ioctl */
	uint32_t				info[FI_OPX_MAX_DPUT_TIDPAIRS];
	/* tidpairs combine CTRL 1 & 2 into CTRL 3 tidpairs for SDMA use */
	uint32_t				pairs[FI_OPX_MAX_DPUT_TIDPAIRS];
};

#define OPX_TID_REFCOUNT(tid_reuse_cache) (tid_reuse_cache->refcount)
#define OPX_TID_VADDR(tid_reuse_cache) (tid_reuse_cache->vaddr)
#define OPX_TID_LENGTH(tid_reuse_cache) (tid_reuse_cache->length)
#define OPX_TID_NINFO(tid_reuse_cache) (tid_reuse_cache->ninfo)
#define OPX_TID_INFO(tid_reuse_cache,idx) (tid_reuse_cache->info[idx])
#define OPX_TID_NPAIRS(tid_reuse_cache) (tid_reuse_cache->npairs)
#define OPX_TID_PAIR(tid_reuse_cache,idx) (tid_reuse_cache->pairs[idx])
#define OPX_TID_IS_INVALID(tid_reuse_cache) (tid_reuse_cache->invalid)
#define OPX_TID_INVALID(tid_reuse_cache) (tid_reuse_cache->invalid = 1)
#define OPX_TID_VALID(tid_reuse_cache) (tid_reuse_cache->invalid = 0)

#ifndef NDEBUG
/* Dump debug tidinfo or tidpairs */
#define OPX_DEBUG_TIDS(string,ntids,in_tids)  						              \
do {    											      \
	int npages = 0; 									      \
	uint32_t *tids = in_tids;				                                      \
	for (int i = 0; i < ntids; ++i) {   						              \
		npages += (int)FI_OPX_EXP_TID_GET(tids[i], LEN);    			              \
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,     				      \
			"%s[%u]=%#8.8X LEN %u, CTRL %u, IDX %u, npages %u\n",     	              \
			string, i, tids[i],							      \
			(int)FI_OPX_EXP_TID_GET(tids[i], LEN),      			              \
			(int)FI_OPX_EXP_TID_GET(tids[i], CTRL),     			              \
			(int)FI_OPX_EXP_TID_GET(tids[i], IDX), npages);     		              \
	}       										      \
} while(0)
#else
#define OPX_DEBUG_TIDS(string,ntids,tids)
#endif
/* Special debug for expected receive rts ONLY */
#ifdef OPX_TID_CACHE_DEBUG /* NOT dependent on NDEBUG */
/* Ugly but it's debug... only print when something changes (less noisy) */
#define OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,string)						       \
do {                                                                                                   \
	static int count = 0;   								       \
	static uint64_t last_vaddr = 0UL;       						       \
	static int32_t last_length = 0; 							       \
	if ((last_vaddr != vaddr) || (last_length != length)) { 				       \
		fprintf(stderr, "OPX_TID_CACHE_DEBUG (%ld) %s TIDs "                                   \
				"input vaddr [%#lX - %#lX] length %lu, "                               \
				"tid   vaddr [%#lX - %#lX] length %lu, "                               \
				"last count %u\n",                                                     \
			OPX_TID_REFCOUNT(tid_reuse_cache),string,		                       \
			vaddr, vaddr+length, length,    					       \
			OPX_TID_VADDR(tid_reuse_cache), OPX_TID_VADDR(tid_reuse_cache) +               \
			OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_LENGTH(tid_reuse_cache),              \
			count); 								       \
		last_vaddr = vaddr;     							       \
		last_length = length;   							       \
		count = 0;      								       \
	}       										       \
	++count;										       \
} while(0)
#else
/* noisier regular debug logging */
#define OPX_TID_CACHE_RZV_RTS(tid_reuse_cache, string)   				               \
FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "OPX_TID_CACHE_DEBUG (%ld) %s TIDs " 		       \
	     "input vaddr [%#lX - %#lX] length %lu, "    					       \
	     "tid   vaddr [%#lX - %#lX] length %lu\n",   					       \
             OPX_TID_REFCOUNT(tid_reuse_cache),string,       				               \
	     vaddr, vaddr+length, length,       						       \
             OPX_TID_VADDR(tid_reuse_cache), OPX_TID_VADDR(tid_reuse_cache) +                          \
	     OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_LENGTH(tid_reuse_cache));
#endif

/* Free the tids on the endpoint */
static inline void opx_free_tid(struct fi_opx_ep * opx_ep)
{
        struct _hfi_ctrl *ctx = opx_ep->hfi->ctrl;
	struct fi_opx_tid_reuse_cache *const tid_reuse_cache = opx_ep->tid_reuse_cache;
        uint32_t old_ntidinfo = OPX_TID_NINFO(tid_reuse_cache);
        uint64_t *old_tidlist = (uint64_t *)&OPX_TID_INFO(tid_reuse_cache,0);
        FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,"opx_hfi_free_tid %u tidpairs\n",old_ntidinfo);
        assert(OPX_TID_REFCOUNT(tid_reuse_cache) == 0);
	opx_tid_cache_flush(opx_ep->tid_domain, true);
        opx_hfi_free_tid(ctx, (uint64_t)old_tidlist, old_ntidinfo);
#ifndef NDEBUG
        {
    	    FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "DEBUG INIT tidinfo %u -> %u\n", 0, old_ntidinfo);
    	    for (int i = 0; i < old_ntidinfo; ++i) {
    		    OPX_TID_INFO(tid_reuse_cache,i) = -1U;
    	    }
    	    uint32_t old_ntidpairs = OPX_TID_NPAIRS(tid_reuse_cache);
    	    FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "DEBUG INIT tidpairs %u -> %u\n", 0, old_ntidpairs);
    	    for (int i = 0; i < old_ntidpairs; ++i) {
    		    OPX_TID_PAIR(tid_reuse_cache,i) = -1U;
    	    }
        }
#endif
        OPX_TID_NINFO(tid_reuse_cache) = 0;
        OPX_TID_VADDR(tid_reuse_cache) = 0UL;
        OPX_TID_LENGTH(tid_reuse_cache) =  0UL;
        OPX_TID_NPAIRS(tid_reuse_cache) = 0;
	OPX_TID_VALID(tid_reuse_cache);
}


/* Combine CTRL 1 & 2 into CTRL 3 tidpairs, store in opx_ep */
static inline void opx_regen_tidpairs(struct fi_opx_ep * opx_ep) {
	struct _hfi_ctrl *ctx = opx_ep->hfi->ctrl;
	struct fi_opx_tid_reuse_cache *const tid_reuse_cache = opx_ep->tid_reuse_cache;
	const uint32_t pg_sz = ctx->__hfi_pg_sz;
	assert(pg_sz == FI_OPX_HFI1_TID_SIZE);
	uint32_t *tidinfo = (uint32_t *)&OPX_TID_INFO(tid_reuse_cache,0);
	uint32_t ntidinfo = OPX_TID_NINFO(tid_reuse_cache);
	uint32_t *tidpairs = &OPX_TID_PAIR(tid_reuse_cache,0);
	OPX_TID_NPAIRS(tid_reuse_cache) = 0;
	size_t accumulated_len = 0;
	int32_t tid_idx = 0, pair_idx = -1;
	unsigned int npages=0;
	OPX_DEBUG_TIDS("Input tidinfo", ntidinfo,tidinfo);
	uint32_t tid_length = (OPX_TID_LENGTH(tid_reuse_cache) + (pg_sz - 1)) & -pg_sz;
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,"tid_idx %u, ntidinfo %u, accumulated_len %zu, length_pages %u\n",tid_idx, ntidinfo,accumulated_len,tid_length);
	/* Combine ctrl 1/2 tids into single ctrl 3 tid pair */
	while ((tid_idx < ntidinfo) && (accumulated_len < tid_length)) {
		if (FI_OPX_EXP_TID_GET(tidinfo[tid_idx],CTRL) == 1) {
			npages += (int)FI_OPX_EXP_TID_GET(tidinfo[tid_idx],LEN);
			accumulated_len += FI_OPX_EXP_TID_GET(tidinfo[tid_idx], LEN) * FI_OPX_HFI1_TID_SIZE;
			pair_idx++;
			tidpairs[pair_idx] = tidinfo[tid_idx];
		} else { /* assume CTRL 2 is always after CTRL 1 which set iov_base */
			assert(tid_idx != 0);
			assert(FI_OPX_EXP_TID_GET(tidinfo[tid_idx],CTRL) == 2);
			npages += (int)FI_OPX_EXP_TID_GET(tidinfo[tid_idx],LEN);
			accumulated_len += FI_OPX_EXP_TID_GET(tidinfo[tid_idx], LEN) * FI_OPX_HFI1_TID_SIZE;
			if ((FI_OPX_EXP_TID_GET(tidinfo[tid_idx-1],CTRL) != 1) ||
			    ((FI_OPX_EXP_TID_GET(tidinfo[tid_idx-1],LEN) + FI_OPX_EXP_TID_GET(tidinfo[tid_idx],LEN)) > 512)) {
				/* Can't combine into CTRL 3 if previous was not CTRL 1 or combined LEN > 512

				   Not obvious, from HAS, my thoughts about why I get data corruption if I use a tid LEN > 512.

				   Offset field (OFFSET): For expected receive packets this offset is added to the address field
				   associated with the specified TID to determine a physical address. This physical address is then
				   used to DMA the data portion of the received packet to system memory. If OM is 0 the 15-bit
				   OFFSET can address a 128KB mapping in DW multiples. If OM is 1 the 15-bit OFFSET can address a
				   2MB mapping in 64B multiples.

				   512 pages is 2MB.  So even if a "tid pair" *seems* to be available, it won't work over 512 pages
				   so keep ctrl 1 tid and ctrl 2 tid separate, do not optimize into ctrl 3 tidpair
				 */
				pair_idx++;
				tidpairs[pair_idx] = tidinfo[tid_idx];
			} else {
				FI_OPX_EXP_TID_RESET(tidpairs[pair_idx], CTRL, 0x3);
				int32_t len = FI_OPX_EXP_TID_GET(tidinfo[tid_idx-1],LEN) + FI_OPX_EXP_TID_GET(tidinfo[tid_idx],LEN);
				FI_OPX_EXP_TID_RESET(tidpairs[pair_idx], LEN, len);
			}
		}
		tid_idx++;
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,"tid_idx %u, ntidinfo %u, accumulated_len %zu, tid_length %u\n",tid_idx, ntidinfo,accumulated_len,tid_length);
	}
	OPX_TID_NPAIRS(tid_reuse_cache) = pair_idx + 1;
	OPX_DEBUG_TIDS("Regen tidpairs",OPX_TID_NPAIRS(tid_reuse_cache), &OPX_TID_PAIR(tid_reuse_cache,0));
}

/* TID Update (pin) the pages.  Supports appending (only) to the existing TID cache */
static inline int opx_append_tid(uint64_t vaddr, uint64_t length, struct fi_opx_ep * opx_ep)
{
	struct _hfi_ctrl *ctx = opx_ep->hfi->ctrl;
	struct fi_opx_tid_reuse_cache *const tid_reuse_cache = opx_ep->tid_reuse_cache;
	const uint32_t pg_sz = ctx->__hfi_pg_sz;
	assert(pg_sz == FI_OPX_HFI1_TID_SIZE);
	const uint32_t max_tidcnt = ctx->__hfi_tidexpcnt;
	assert(length < UINT_MAX);
	if(OFI_UNLIKELY(length > (max_tidcnt * pg_sz))) {
		/* This is somewhat arbitrary - if we "chunk" the TID updates we might be able
		 * to do larger buffers using multiple update calls. */
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,"Max length exceeded, %lu\n",length);
		OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"UPDATE LENGTH EXCEEDED");
		return -1;
	}
	uint32_t tidcnt = (uint32_t)((length >> 12) + (length & 0x7FFF ? 1 :0));
	uint32_t starting_ntidpairs = OPX_TID_NINFO(tid_reuse_cache);
	/* Eventually we might need to "chunk" updates, thus the naming here */
	uint32_t tidcnt_chunk = tidcnt;
	uint32_t length_chunk = pg_sz * tidcnt_chunk; /* tid update takes uint32_t, not uint64_t length */
	if (OFI_UNLIKELY((starting_ntidpairs + tidcnt) > max_tidcnt)) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,"Max TIDs exceeded, %u + %u > %u\n",starting_ntidpairs, tidcnt, max_tidcnt);
		OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"UPDATE NTIDS EXCEEDED");
#ifdef OPX_TID_CACHE_DEBUG
		fprintf(stderr, "OPX_TID_CACHE_DEBUG Update number of TIDs (%u) exceeded\n",starting_ntidpairs + tidcnt);
#endif
		return -1;
	}
	/* new (cumulative) vaddr/length of this operation*/
	uint64_t new_vaddr = vaddr;
	uint64_t new_length = length_chunk; /* page aligned length */
	if (starting_ntidpairs) {
		assert(OPX_TID_LENGTH(tid_reuse_cache) && OPX_TID_VADDR(tid_reuse_cache));
		assert(OPX_TID_INFO(tid_reuse_cache,(starting_ntidpairs-1)) != -1U);
		/* Append starts at current vaddr and extends by page aligned new length */
		new_vaddr = OPX_TID_VADDR(tid_reuse_cache);
		new_length += OPX_TID_LENGTH(tid_reuse_cache);
	} else {
		assert((OPX_TID_LENGTH(tid_reuse_cache) == 0) && (OPX_TID_VADDR(tid_reuse_cache) == 0) && (OPX_TID_REFCOUNT(tid_reuse_cache) == 0));
	}
	uint64_t *tidlist = (uint64_t *)&OPX_TID_INFO(tid_reuse_cache, starting_ntidpairs);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "starting ntidpairs %u, starting TID range [%#lX - %#lX] length %lu, append buffer range [%lX - %#lX] length %lu, new range [%#lX - %#lX] length %lu\n",
		     starting_ntidpairs,
		     OPX_TID_VADDR(tid_reuse_cache), OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_LENGTH(tid_reuse_cache),
		     vaddr, vaddr + length, length,
		     new_vaddr, new_vaddr + new_length, new_length);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "update tid length %#X, pages (tidcnt) %u\n",
		length_chunk, tidcnt);
	assert(vaddr + length <= vaddr + (tidcnt * pg_sz));
#ifndef NDEBUG
        {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "DEBUG INIT tidpairs %u -> %u\n", starting_ntidpairs, starting_ntidpairs + tidcnt_chunk);
		for (int i = starting_ntidpairs ; i < starting_ntidpairs + tidcnt_chunk; ++i) {
			OPX_TID_INFO(tid_reuse_cache,(i+starting_ntidpairs))= -1U;
		}
        }
#endif
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,"opx_hfi_update_tid vaddr %#lX, length %u\n",vaddr,length_chunk);
	/* return code is ignored in favor of length checks for failure (length == 0 if rc == -1 */
	opx_hfi_update_tid(ctx,
			   vaddr, /* input */
			   &length_chunk, /* input/output*/
			   (uint64_t)tidlist, /* input/output ptr cast as uint64_t */
			   &tidcnt_chunk, /* output */
			   0);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "opx_hfi_update_tid return length %u, tidcnt %u\n", length_chunk, tidcnt_chunk);
	if(OFI_UNLIKELY((uint64_t)length_chunk < length)) { /* update failed, soft (partial update) or hard (-1 ioctl & 0 length) */
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,"opx_hfi_update_tid failed on vaddr %#lX, length %u\n",vaddr,length_chunk);
		if(starting_ntidpairs) {/* This was APPEND */
			OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"UPDATE/APPEND FAILED");
#ifdef OPX_TID_CACHE_DEBUG
			fprintf(stderr, "OPX_TID_CACHE_DEBUG (UPDATE/APPEND) opx_hfi_update_tid failed on vaddr %#lX, length %u\n",vaddr,length_chunk);
#endif
			/* Cleanup (free) or risk endlessly trying to append/update */
			if (length_chunk) {
				/* We did partial update, free it or it's a TID resource leak */
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,"opx_hfi_free_tid %u tidpairs\n",tidcnt_chunk);
				opx_hfi_free_tid(ctx,(uint64_t)tidlist, tidcnt_chunk);
			}
			opx_free_tid(opx_ep);
			/* Free the original TIDs */
			vaddr = new_vaddr;
			length_chunk = new_length;
			/* Now retry the full buffer, might succeed now */
			starting_ntidpairs = 0; /* not appending now */
			OPX_TID_NINFO(tid_reuse_cache) = 0; /* not appending now */
			tidlist = (uint64_t *)&OPX_TID_INFO(tid_reuse_cache,starting_ntidpairs);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,"opx_hfi_update_tid vaddr %#lX, length %u\n",vaddr,length_chunk);
			/* return code is ignored in favor of length checks for failure (length == 0 if rc == -1 */
			opx_hfi_update_tid(ctx,
					   vaddr, /* input */
					   &length_chunk, /* input/output*/
					   (uint64_t)tidlist, /* input/output ptr cast as uint64_t */
					   &tidcnt_chunk, /* output */
					   0);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "opx_hfi_update_tid return length %u, tidcnt %u\n", length_chunk, tidcnt_chunk);
			if ((uint64_t)length_chunk < (length + OPX_TID_LENGTH(tid_reuse_cache))) {
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,"opx_hfi_update_tid failed on vaddr %#lX, length %u\n",vaddr,length_chunk);
				OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"UPDATE/RETRY FAILED");
#ifdef OPX_TID_CACHE_DEBUG
				fprintf(stderr, "OPX_TID_CACHE_DEBUG (UPDATE/RETRY) opx_hfi_update_tid failed on vaddr %#lX, length %u\n",vaddr,length_chunk);
#endif
				return -1;
			}

		} else {
			OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"UPDATE/NEW FAILED");
#ifdef OPX_TID_CACHE_DEBUG
			fprintf(stderr, "OPX_TID_CACHE_DEBUG (UPDATE/NEW) opx_hfi_update_tid failed on vaddr %#lX, length %u\n",vaddr,length_chunk);
#endif
			/* Not sure why this would fail. Might consider disabling expected receive */
			return -1;
		}
	}
	assert(tidcnt_chunk <= (FI_OPX_MAX_DPUT_TIDPAIRS - starting_ntidpairs));
	OPX_DEBUG_TIDS("Previous tidinfo",starting_ntidpairs,&OPX_TID_INFO(tid_reuse_cache,0));
	OPX_DEBUG_TIDS("Appended tidinfo",tidcnt_chunk,(&(OPX_TID_INFO(tid_reuse_cache,starting_ntidpairs))));
	OPX_TID_VADDR(tid_reuse_cache) = new_vaddr;
	OPX_TID_LENGTH(tid_reuse_cache) = new_length;
	OPX_TID_NINFO(tid_reuse_cache) += tidcnt_chunk; /* appended or replaced */
	++OPX_TID_REFCOUNT(tid_reuse_cache);
	OPX_TID_VALID(tid_reuse_cache);

	if(starting_ntidpairs) {
		OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"UPDATE/APPEND");
	} else {
		OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"UPDATE/NEW");
	}
	/* open (maybe create/flush) and close a cache entry */
	uint64_t key = new_vaddr;
	opx_tid_cache_open_region(opx_ep->tid_domain,  (void*)new_vaddr, new_length, key, opx_ep, &opx_ep->tid_mr);

	opx_tid_cache_close_region(opx_ep->tid_mr);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%lu UPDATED TIDs vaddr [%#lX - %#lX] length %lu, tid vaddr [%#lX - %#lX] , tid length %lu, number of TIDs %u\n",OPX_TID_REFCOUNT(tid_reuse_cache),
		     vaddr, vaddr+length, length, OPX_TID_VADDR(tid_reuse_cache), OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_NINFO(tid_reuse_cache));
	opx_regen_tidpairs(opx_ep);
	return 0;
}



/* Special debug for expected receive data ONLY */
#ifdef OPX_TID_CACHE_DEBUG /* NOT dependent on NDEBUG */
#define OPX_TID_CACHE_RZV_DATA(tid_reuse_cache,string)					               \
do { /* Ugly but it's debug... */  								       \
	static int count = 0;   								       \
	static uint64_t last_vaddr = 0UL;       						       \
	static int32_t last_length = 0; 							       \
	if ((last_vaddr != OPX_TID_VADDR(tid_reuse_cache)) ||                                          \
	    (last_length != OPX_TID_LENGTH(tid_reuse_cache))) { 	                               \
		fprintf(stderr, "OPX_TID_CACHE_DEBUG (%ld) %s TIDs "                                   \
				"input vaddr [%#lX - %#lX] length %lu, "                               \
				"tid   vaddr [%#lX - %#lX] length %u, "                                \
				"last count %u\n",                                                     \
			OPX_TID_REFCOUNT(tid_reuse_cache),string,       			       \
			OPX_TID_VADDR(tid_reuse_cache),                                                \
			OPX_TID_VADDR(tid_reuse_cache)+OPX_TID_LENGTH(tid_reuse_cache),                \
			OPX_TID_LENGTH(tid_reuse_cache),                                               \
			last_vaddr, last_vaddr + last_length, last_length,                             \
			count); 								       \
		last_vaddr = OPX_TID_VADDR(tid_reuse_cache);  				               \
		last_length = OPX_TID_LENGTH(tid_reuse_cache);				               \
		count = 0;      								       \
	}       										       \
	++count;										       \
} while(0)
#else
#define OPX_TID_CACHE_RZV_DATA(tid_reuse_cache,string) 						       \
FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,      						       \
       "OPX_TID_CACHE_DEBUG (%ld) %s TIDs "   							       \
       "tid   vaddr [%#lX - %#lX] length %lu\n", 						       \
       OPX_TID_REFCOUNT(tid_reuse_cache),string,						       \
       OPX_TID_VADDR(tid_reuse_cache),                                                                 \
       OPX_TID_VADDR(tid_reuse_cache)+OPX_TID_LENGTH(tid_reuse_cache),                                 \
       OPX_TID_LENGTH(tid_reuse_cache))
#endif

static inline void opx_buffer_range_debug(uint64_t immediate_data, uint64_t immediate_end_block_count,
					  uint64_t vaddr, uint64_t length, uint64_t alignment_adjustment, uint64_t alignment_mask,
					  uintptr_t dst_vaddr,
					  struct iovec* src_iovec,
					  uintptr_t origin_byte_counter_vaddr, uintptr_t target_byte_counter_vaddr)
{
	if (alignment_adjustment != (alignment_adjustment - (length - (length & -64)))) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "alignment_adjustment %lu/%#lX != (alignment_adjustment - (length -length & -64))  %lu/%#lX\n",alignment_adjustment,alignment_adjustment,alignment_adjustment - (length - (length & -64)),alignment_adjustment - (length - (length & -64)));
		assert(!(alignment_adjustment != (alignment_adjustment - (length - (length & -64)))));
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "total length %#lX, src len %#lX, immediate length %#lX, length adjustment %#lX, alignment_mask %#lX, immediate_end_block_count %lu\n",
		     length, src_iovec->iov_len, immediate_data, alignment_adjustment, alignment_mask, immediate_end_block_count);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "original source range [%p - %p] (%#lX), adjusted source range [%p - %p] (%#lX)\n",
		     src_iovec->iov_base, (char*)src_iovec->iov_base + src_iovec->iov_len,src_iovec->iov_len,
		     (void*)(((uint64_t)src_iovec->iov_base) - alignment_adjustment), (void*)((((uint64_t)src_iovec->iov_base) - alignment_adjustment) + length),length);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "original source range [%p - %p] (%#lX), truncated source range [%p - %p] (%#lX)\n",
		     src_iovec->iov_base, (char*)src_iovec->iov_base + src_iovec->iov_len,src_iovec->iov_len,
		     (void*)(((uint64_t)src_iovec->iov_base) - alignment_adjustment), (void*)((((uint64_t)src_iovec->iov_base) - alignment_adjustment) + length - (immediate_end_block_count << 6)),(uint64_t)length - (immediate_end_block_count << 6));
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "final source range [%p - %p] (%#lX)\n",
		     (void*)(((uint64_t)src_iovec->iov_base) - alignment_adjustment), (char*)(((uint64_t)src_iovec->iov_base - alignment_adjustment) + length), length);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "original immediate dest range [%p - %p], adjusted dest range [%p - %p]\n",
		     (void*)dst_vaddr, (void*)((uint64_t)dst_vaddr + src_iovec->iov_len),
		     (void*)vaddr, (void*)(vaddr + length));
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Original counters %p/%p\n",
		     (uint64_t*)origin_byte_counter_vaddr,
		     (uint64_t*)target_byte_counter_vaddr);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Target counter %#lX, adjusted %#lX\n",
		     *(uint64_t*)target_byte_counter_vaddr,
		     *(uint64_t*)target_byte_counter_vaddr + alignment_adjustment);
	if(!(((char*)src_iovec->iov_base + src_iovec->iov_len - (immediate_end_block_count << 6)) <= ((char*)((((uint64_t)src_iovec->iov_base) - alignment_adjustment) + length)))) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "total length %#lX, src len %#lX, immediate length %#lX, length adjustment %#lX, alignment_mask %#lX, immediate_end_block_count %lu\n",
			     length, src_iovec->iov_len, immediate_data, alignment_adjustment, alignment_mask, immediate_end_block_count);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "original source range [%p - %p] (%#lX), adjusted source range [%p - %p] (%#lX)\n",
			     src_iovec->iov_base, (char*)src_iovec->iov_base + src_iovec->iov_len,src_iovec->iov_len,
			     (void*)(((uint64_t)src_iovec->iov_base) - alignment_adjustment), (void*)((((uint64_t)src_iovec->iov_base) - alignment_adjustment) + length),length);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "original source range [%p - %p] (%#lX), truncated source range [%p - %p] (%#lX)\n",
			     src_iovec->iov_base, (char*)src_iovec->iov_base + src_iovec->iov_len,src_iovec->iov_len,
			     (void*)(((uint64_t)src_iovec->iov_base) - alignment_adjustment), (void*)((((uint64_t)src_iovec->iov_base) - alignment_adjustment) + length - (immediate_end_block_count << 6)),(uint64_t)length - (immediate_end_block_count << 6));
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "final source range [%p - %p] (%#lX)\n",
			     (void*)(((uint64_t)src_iovec->iov_base) - alignment_adjustment), (char*)(((uint64_t)src_iovec->iov_base - alignment_adjustment) + length), length);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "original immediate dest range [%p - %p], adjusted dest range [%p - %p]\n",
			     (void*)dst_vaddr, (void*)((uint64_t)dst_vaddr + src_iovec->iov_len),
			     (void*)vaddr, (void*)(vaddr + length));
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Original counters %p/%p\n",
			     (uint64_t*)origin_byte_counter_vaddr,
			     (uint64_t*)target_byte_counter_vaddr);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Target counter %#lX, adjusted %#lX\n",
			     *(uint64_t*)target_byte_counter_vaddr,
			     *(uint64_t*)target_byte_counter_vaddr + alignment_adjustment);
	}
	if(!(((char*)src_iovec->iov_base + src_iovec->iov_len) >= ((char*)((((uint64_t)src_iovec->iov_base) - alignment_adjustment) + length)))) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "total length %#lX, src len %#lX, immediate length %#lX, length adjustment %#lX, alignment_mask %#lX, immediate_end_block_count %lu\n",
			     length, src_iovec->iov_len, immediate_data, alignment_adjustment, alignment_mask, immediate_end_block_count);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "original source range [%p - %p] (%#lX), adjusted source range [%p - %p] (%#lX)\n",
				src_iovec->iov_base, (char*)src_iovec->iov_base + src_iovec->iov_len,src_iovec->iov_len,
			     (void*)(((uint64_t)src_iovec->iov_base) - alignment_adjustment), (void*)((((uint64_t)src_iovec->iov_base) - alignment_adjustment) + length),length);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "original source range [%p - %p] (%#lX), truncated source range [%p - %p] (%#lX)\n",
			     src_iovec->iov_base, (char*)src_iovec->iov_base + src_iovec->iov_len,src_iovec->iov_len,
			     (void*)(((uint64_t)src_iovec->iov_base) - alignment_adjustment), (void*)((((uint64_t)src_iovec->iov_base) - alignment_adjustment) + length - (immediate_end_block_count << 6)),(uint64_t)length - (immediate_end_block_count << 6));
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "final source range [%p - %p] (%#lX)\n",
			     (void*)(((uint64_t)src_iovec->iov_base) - alignment_adjustment), (char*)(((uint64_t)src_iovec->iov_base - alignment_adjustment) + length), length);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "original immediate dest range [%p - %p], adjusted dest range [%p - %p]\n",
			     (void*)dst_vaddr, (void*)((uint64_t)dst_vaddr + src_iovec->iov_len),
			     (void*)vaddr, (void*)(vaddr + length));
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Original counters %p/%p\n",
			     (uint64_t*)origin_byte_counter_vaddr,
			     (uint64_t*)target_byte_counter_vaddr);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "Target counter %#lX, adjusted %#lX\n",
			     *(uint64_t*)target_byte_counter_vaddr,
			     *(uint64_t*)target_byte_counter_vaddr + alignment_adjustment);
	}
}
#ifndef NDEBUG
        #define OPX_BUFFER_RANGE_DEBUG(immediate_data, immediate_end_block_count, vaddr, length, alignment_adjustment, alignment_mask, dst_vaddr, src_iovec, origin_byte_counter_vaddr,target_byte_counter_vaddr) \
	        opx_buffer_range_debug(immediate_data, immediate_end_block_count, vaddr, length, alignment_adjustment, alignment_mask, dst_vaddr, src_iovec, origin_byte_counter_vaddr,target_byte_counter_vaddr)
#else
        #define OPX_BUFFER_RANGE_DEBUG(immediate_data, immediate_end_block_count, vaddr, length, alignment_adjustment, alignment_mask, dst_vaddr, src_iovec, origin_byte_counter_vaddr,target_byte_counter_vaddr)
#endif

#endif /* _FI_PROV_OPX_TID_H_ */

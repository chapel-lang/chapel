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
#include "fi_opx_tid_cache.h"

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

#include "rdma/opx/fi_opx_hfi1.h"

/* The max for both eager and expected receive is 2048 entries, which can
   be software partitioned between the eager receive and expected receive
   mechanisms on a per-context basis.
   We use this for a TID cache max size (and validate against
   ctx->__hfi_tidexpcnt)  */
#define OPX_MAX_TID_COUNT 2048

#define OPX_TID_NPAGES(tid_reuse_cache, npages)                                      \
	do {                                                                         \
		npages = 0;                                                          \
		const uint32_t *tids = &tid_reuse_cache->info[0];                    \
		const uint32_t ntids = tid_reuse_cache->ninfo;                       \
		for (int i = 0; i < ntids; ++i) {                                    \
			npages += (int)FI_OPX_EXP_TID_GET(tids[i], LEN);             \
			FI_DBG(fi_opx_global.prov, FI_LOG_MR,                        \
			       "%s[%u]=%#8.8X LEN %u, CTRL %u, IDX %u, npages %u\n", \
			       "OPX_TID_NPAGES", i, tids[i],                         \
			       (int)FI_OPX_EXP_TID_GET(tids[i], LEN),                \
			       (int)FI_OPX_EXP_TID_GET(tids[i], CTRL),               \
			       (int)FI_OPX_EXP_TID_GET(tids[i], IDX), npages);       \
		}                                                                    \
	} while (0)

#ifndef NDEBUG
/* Dump debug tidinfo or tidpairs */
#define OPX_DEBUG_TIDS(string, ntids, in_tids)                                       \
	do {                                                                         \
		int npages = 0;                                                      \
		const uint32_t *tids = in_tids;                                      \
		for (int i = 0; i < ntids; ++i) {                                    \
			npages += (int)FI_OPX_EXP_TID_GET(tids[i], LEN);             \
			FI_DBG(fi_opx_global.prov, FI_LOG_MR,                        \
			       "%s[%u]=%#8.8X LEN %u, CTRL %u, IDX %u, npages %u\n", \
			       string, i, tids[i],                                   \
			       (int)FI_OPX_EXP_TID_GET(tids[i], LEN),                \
			       (int)FI_OPX_EXP_TID_GET(tids[i], CTRL),               \
			       (int)FI_OPX_EXP_TID_GET(tids[i], IDX), npages);       \
		}                                                                    \
	} while (0)
#else
#define OPX_DEBUG_TIDS(string, ntids, tids)
#endif

#ifdef OPX_TID_CACHE_DEBUG
/* Dump debug tidinfo or tidpairs */
#define OPX_FPRINTF_TIDS(string, ntids, in_tids)                                         \
	do {                                                                             \
		int npages = 0;                                                          \
		const uint32_t *tids = in_tids;                                          \
		for (int i = 0; i < ntids; ++i) {                                        \
			npages += (int)FI_OPX_EXP_TID_GET(tids[i], LEN);                 \
			fprintf(stderr,                                                  \
				"## %s[%u]=%#8.8X LEN %u, CTRL %u, IDX %u, npages %u\n", \
				string, i,tids[i],                                       \
				(int)FI_OPX_EXP_TID_GET(tids[i], LEN),                   \
				(int)FI_OPX_EXP_TID_GET(tids[i], CTRL),                  \
				(int)FI_OPX_EXP_TID_GET(tids[i], IDX),                   \
				npages);                                                 \
		}                                                                        \
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
#define OPX_TID_CACHE_RZV_RTS(tid_reuse_cache, string)                         \
	do {                                                                   \
		static int count = 0;                                          \
		static uint64_t last_vaddr = 0UL;                              \
		static int32_t last_length = 0;                                \
		if ((last_vaddr != tid_vaddr) ||                               \
		    (last_length != tid_length)) {                             \
			fprintf(stderr,                                        \
				"## %s:%u OPX_TID_CACHE_DEBUG %s TIDs "        \
				"input vaddr [%#lx - %#lx] length %lu, "       \
				"tid   vaddr [%#lx - %#lx] length %lu, "       \
				"last count %u\n",                             \
				__func__, __LINE__,                            \
				string,                                        \
				tid_vaddr, tid_vaddr + tid_length, tid_length, \
				tid_reuse_cache->tid_vaddr,                    \
				tid_reuse_cache->tid_vaddr +                   \
					tid_reuse_cache->tid_length,           \
				tid_reuse_cache->tid_length, count);           \
			last_vaddr = tid_vaddr;                                \
			last_length = tid_length;                              \
			count = 0;                                             \
		}                                                              \
		++count;                                                       \
		FI_DBG(fi_opx_global.prov, FI_LOG_MR,                          \
		       "OPX_TID_CACHE_DEBUG %s TIDs "                          \
		       "input vaddr [%#lx - %#lx] length %lu, "                \
		       "tid   vaddr [%#lx - %#lx] length %lu\n",               \
		       string, tid_vaddr,                                      \
		       tid_vaddr + tid_length, tid_length,                     \
		       tid_reuse_cache->tid_vaddr,                             \
		       tid_reuse_cache->tid_vaddr +                            \
			       tid_reuse_cache->tid_length,                    \
		       tid_reuse_cache->tid_length);                           \
	} while (0)
#else
/* noisier regular debug logging */
#define OPX_TID_CACHE_RZV_RTS(tid_reuse_cache, string)               \
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,                        \
	       "OPX_TID_CACHE_DEBUG %s TIDs "                        \
	       "input vaddr [%#lx - %#lx] length %lu, "              \
	       "tid   vaddr [%#lx - %#lx] length %lu\n",             \
	       string, tid_vaddr,                                    \
	       tid_vaddr + tid_length, tid_length,                   \
	       tid_reuse_cache->tid_vaddr,                           \
	       tid_reuse_cache->tid_vaddr +                          \
		       tid_reuse_cache->tid_length,                  \
	       tid_reuse_cache->tid_length);
#endif

/* Special debug for expected receive data ONLY */
#ifdef OPX_TID_CACHE_VERBOSE_DEBUG /* NOT dependent on NDEBUG */
#define OPX_TID_CACHE_RZV_DATA(tid_reuse_cache, string)                               \
	do { /* Ugly but it's debug... */                                             \
		static int count = 0;                                                 \
		static uint64_t last_vaddr = 0UL;                                     \
		static int32_t last_length = 0;                                       \
		if ((last_vaddr != tid_reuse_cache->tid_vaddr) ||                     \
		    (last_length != tid_reuse_cache->tid_length)) {                   \
			fprintf(stderr,                                               \
				"## %s:%u OPX_TID_CACHE_VERBOSE_DEBUG %s TIDs "       \
				"input vaddr [%#lx - %#lx] length %lu, "              \
				"tid   vaddr [%#lx - %#lx] length %u, "               \
				"last count %u\n",                                    \
				__func__, __LINE__,                                   \
				string,                                               \
				tid_reuse_cache->tid_vaddr,                           \
				tid_reuse_cache->tid_vaddr +                          \
					tid_reuse_cache->tid_length,                  \
				tid_reuse_cache->tid_length, last_vaddr,              \
				last_vaddr + last_length, last_length, count);        \
			last_vaddr = tid_reuse_cache->tid_vaddr;                      \
			last_length = tid_reuse_cache->tid_length;                    \
			count = 0;                                                    \
		}                                                                     \
		++count;                                                              \
	} while (0)
#else
#define OPX_TID_CACHE_RZV_DATA(tid_reuse_cache, string)     \
	FI_DBG(fi_opx_global.prov, FI_LOG_MR,               \
	       "OPX_TID_CACHE_VERBOSE_DEBUG %s TIDs "       \
	       "tid   vaddr [%#lx - %#lx] length %lu\n",    \
	       string,                                      \
	       tid_reuse_cache->tid_vaddr,                  \
	       tid_reuse_cache->tid_vaddr +                 \
		       tid_reuse_cache->tid_length,         \
	       tid_reuse_cache->tid_length)
#endif

#endif /* _FI_PROV_OPX_TID_H_ */

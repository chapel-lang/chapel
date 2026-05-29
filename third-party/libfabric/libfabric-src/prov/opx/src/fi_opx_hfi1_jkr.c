/*
 * Copyright (C) 2024-2025 Cornelis Networks.
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

#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_hfi1_version.h"
#include "rdma/opx/opx_hfi1_cn5000.h"

void opx_jkr_rhe_debug(struct fi_opx_ep *opx_ep, volatile uint64_t *rhe_ptr, volatile uint32_t *rhf_ptr,
		       const uint32_t rhf_msb, const uint32_t rhf_lsb, const uint64_t rhf_seq,
		       const uint64_t hdrq_offset, const uint64_t rhf_rcvd, const union opx_hfi1_packet_hdr *const hdr,
		       const enum opx_hfi1_type hfi1_type, uint16_t last_egrbfr_index)
{
	uint32_t	   rhe_index = hdrq_offset >> FI_OPX_HFI1_HDRQ_INDEX_SHIFT;
	volatile uint64_t *rhe	     = rhe_ptr + rhe_index; /* 8 byte entries */
#ifdef OPX_VERBOSE_TRIGGER				    // verbose output
	fprintf(stderr,
#else
	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
#endif
		"ERROR %s RHF(%#16.16lX) RHE(%p)[%u]=%p RHE %#16.16lX is ERRORED %u, UseEgrBuf %u, EgrIndex %#X/%#X, EgrOffset %#X, %s%s%s %s %#16.16lX  %s%s%s%s%s%s%s%s%s%s%s \n",
		OPX_HFI1_TYPE_STRING(OPX_SW_HFI1_TYPE), rhf_rcvd, rhe_ptr, rhe_index, rhe, *rhe,
		OPX_IS_ERRORED_RHF(rhf_rcvd, hfi1_type) != 0UL, OPX_RHF_IS_USE_EGR_BUF(rhf_rcvd, hfi1_type),
		(uint32_t) OPX_RHF_EGR_INDEX(rhf_rcvd, hfi1_type), last_egrbfr_index,
		(uint32_t) OPX_RHF_EGR_OFFSET(rhf_rcvd, hfi1_type),
		OPX_RHF_RCV_TYPE_EXPECTED_RCV(rhf_rcvd, hfi1_type) ? "EXPECTED_RCV" : "",
		OPX_RHF_RCV_TYPE_EAGER_RCV(rhf_rcvd, hfi1_type) ? "EAGER_RCV" : "",
		OPX_RHF_RCV_TYPE_OTHER(rhf_rcvd, hfi1_type) ? "OTHER RCV" : "",
		((*rhe) & OPX_JKR_RHE_TAIL) ? "OPX_JKR_RHE_TAIL        " : "", ((*rhe) & OPX_JKR_RHE_TAIL),
		((*rhe) & OPX_JKR_RHE_ICRCERR) ? "OPX_JKR_RHE_ICRCERR     " : "",
		((*rhe) & OPX_JKR_RHE_TIDBYPASSERR) ? "OPX_JKR_RHE_TIDBYPASSERR" : "",
		((*rhe) & OPX_JKR_RHE_ECCERR) ? "OPX_JKR_RHE_ECCERR      " : "",
		((*rhe) & OPX_JKR_RHE_LENERR) ? "OPX_JKR_RHE_LENERR      " : "",
		((*rhe) & OPX_JKR_RHE_TIDERR) ? "OPX_JKR_RHE_TIDERR      " : "",
		((*rhe) & OPX_JKR_RHE_RCVTYPEERR) ? "OPX_JKR_RHE_RCVTYPEERR  " : "",
		((*rhe) & OPX_JKR_RHE_CRKERR) ? "OPX_JKR_RHE_CRKERR      " : "",
		((*rhe) & OPX_JKR_RHE_CRKUNCERR) ? "OPX_JKR_RHE_CRKUNCERR   " : "",
		((*rhe) & OPX_JKR_RHE_KHDRLENERR) ? "OPX_JKR_RHE_KHDRLENERR  " : "",
		((*rhe) & OPX_JKR_RHE_FLOWGENERR) ? "OPX_JKR_RHE_FLOWGENERR  " : "",
		((*rhe) & OPX_JKR_RHE_FLOWSEQERR) ? "OPX_JKR_RHE_FLOWSEQERR  " : "");

	FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
		"%s HEADER ERROR RHF(%#16.16lX) RHE(%#16.16lX) %s%s%s  %s%s%s%s%s%s%s%s%s%s%s \n",
		OPX_HFI1_TYPE_STRING(hfi1_type), rhf_rcvd, *rhe,
		OPX_RHF_RCV_TYPE_EXPECTED_RCV(rhf_rcvd, hfi1_type) ? "EXPECTED_RCV" : "",
		OPX_RHF_RCV_TYPE_EAGER_RCV(rhf_rcvd, hfi1_type) ? "EAGER_RCV" : "",
		OPX_RHF_RCV_TYPE_OTHER(rhf_rcvd, hfi1_type) ? "OTHER RCV" : "",
		((*rhe) & OPX_JKR_RHE_ICRCERR) ? "OPX_JKR_RHE_ICRCERR     " : "",
		((*rhe) & OPX_JKR_RHE_TIDBYPASSERR) ? "OPX_JKR_RHE_TIDBYPASSERR" : "",
		((*rhe) & OPX_JKR_RHE_ECCERR) ? "OPX_JKR_RHE_ECCERR      " : "",
		((*rhe) & OPX_JKR_RHE_LENERR) ? "OPX_JKR_RHE_LENERR      " : "",
		((*rhe) & OPX_JKR_RHE_TIDERR) ? "OPX_JKR_RHE_TIDERR      " : "",
		((*rhe) & OPX_JKR_RHE_RCVTYPEERR) ? "OPX_JKR_RHE_RCVTYPEERR  " : "",
		((*rhe) & OPX_JKR_RHE_CRKERR) ? "OPX_JKR_RHE_CRKERR      " : "",
		((*rhe) & OPX_JKR_RHE_CRKUNCERR) ? "OPX_JKR_RHE_CRKUNCERR   " : "",
		((*rhe) & OPX_JKR_RHE_KHDRLENERR) ? "OPX_JKR_RHE_KHDRLENERR  " : "",
		((*rhe) & OPX_JKR_RHE_FLOWGENERR) ? "OPX_JKR_RHE_FLOWGENERR  " : "",
		((*rhe) & OPX_JKR_RHE_FLOWSEQERR) ? "OPX_JKR_RHE_FLOWSEQERR  " : "");

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.rhf.error);
	FI_OPX_DEBUG_COUNTERS_INC_COND((*rhe) & OPX_JKR_RHE_ICRCERR, opx_ep->debug_counters.rhf.icrcerr);
	FI_OPX_DEBUG_COUNTERS_INC_COND((*rhe) & OPX_JKR_RHE_TIDBYPASSERR, opx_ep->debug_counters.rhf.tidbypasserr);
	FI_OPX_DEBUG_COUNTERS_INC_COND((*rhe) & OPX_JKR_RHE_ECCERR, opx_ep->debug_counters.rhf.eccerr);
	FI_OPX_DEBUG_COUNTERS_INC_COND((*rhe) & OPX_JKR_RHE_LENERR, opx_ep->debug_counters.rhf.lenerr);
	FI_OPX_DEBUG_COUNTERS_INC_COND((*rhe) & OPX_JKR_RHE_TIDERR, opx_ep->debug_counters.rhf.tiderr);
	FI_OPX_DEBUG_COUNTERS_INC_COND((*rhe) & OPX_JKR_RHE_CRKERR, opx_ep->debug_counters.rhf.crkerr);
	FI_OPX_DEBUG_COUNTERS_INC_COND((*rhe) & OPX_JKR_RHE_CRKUNCERR, opx_ep->debug_counters.rhf.crkuncerr);
	FI_OPX_DEBUG_COUNTERS_INC_COND((*rhe) & OPX_JKR_RHE_KHDRLENERR, opx_ep->debug_counters.rhf.khdrlenerr);
	FI_OPX_DEBUG_COUNTERS_INC_COND((*rhe) & OPX_JKR_RHE_FLOWGENERR, opx_ep->debug_counters.rhf.flowgenerr);
	FI_OPX_DEBUG_COUNTERS_INC_COND((*rhe) & OPX_JKR_RHE_FLOWSEQERR, opx_ep->debug_counters.rhf.flowseqerr);
	FI_OPX_DEBUG_COUNTERS_INC_COND((*rhe) & OPX_JKR_RHE_RCVTYPEERR, opx_ep->debug_counters.rhf.rcvtypeerr);
	/* Count the packet type that had an error */
	FI_OPX_DEBUG_COUNTERS_INC_COND((OPX_RHF_RCV_TYPE_EXPECTED_RCV(rhf_rcvd, hfi1_type)),
				       opx_ep->debug_counters.rhf.rcvtypeexp);
	FI_OPX_DEBUG_COUNTERS_INC_COND((OPX_RHF_RCV_TYPE_EAGER_RCV(rhf_rcvd, hfi1_type)),
				       opx_ep->debug_counters.rhf.rcvtypeegr);
	FI_OPX_DEBUG_COUNTERS_INC_COND((OPX_RHF_RCV_TYPE_OTHER(rhf_rcvd, hfi1_type)),
				       opx_ep->debug_counters.rhf.rcvtypeoth);

#ifdef OPX_VERBOSE_TRIGGER // verbose output
	fi_opx_hfi1_dump_packet_hdr(hdr, hfi1_type, "OPX_IS_ERRORED_RHF", __LINE__);
#endif

	/* trigger on unexpected errors ) ignoring TIDERR */
	if (rhe && !((*rhe) & OPX_JKR_RHE_TIDERR)) {
		opx_sw_trigger();
	}

	return;
}

int opx_rhf_missing_payload_error_handler(const uint64_t rhf_rcvd, const union opx_hfi1_packet_hdr *const hdr,
					  const enum opx_hfi1_type hfi1_type)
{
	const uint8_t opcode = hdr->bth.opcode;
#ifdef OPX_VERBOSE_TRIGGER // verbose output
	fprintf(stderr,
#else
	FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
#endif
		"MISSING PAYLOAD opcode %#X, UseEgrBuf %u, pktlen %#X, type: %s%s%s\n", opcode,
		OPX_RHF_IS_USE_EGR_BUF(rhf_rcvd, hfi1_type),
		hfi1_type & OPX_HFI1_WFR ? (hdr->lrh_16B.pktlen > 0x9) : ntohs(hdr->lrh_9B.pktlen),
		OPX_RHF_RCV_TYPE_EXPECTED_RCV(rhf_rcvd, hfi1_type) ? "EXPECTED_RCV" : "",
		OPX_RHF_RCV_TYPE_EAGER_RCV(rhf_rcvd, hfi1_type) ? "EAGER_RCV" : "",
		OPX_RHF_RCV_TYPE_OTHER(rhf_rcvd, hfi1_type) ? "OTHER RCV" : "");
#ifdef OPX_VERBOSE_TRIGGER // verbose ouput
	fi_opx_hfi1_dump_packet_hdr(hdr, hfi1_type, "MISSING PAYLOAD", __LINE__);
#endif
	opx_sw_trigger();
	return 1;
}

void opx_print_16B_pbc(uint64_t pbc1, const char *func)
{
	__attribute__((__unused__)) union opx_jkr_pbc pbc;
	pbc.raw64b = pbc1;
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc = %#16.16lX\n", func, pbc1);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.LengthDWs = %#x %zu\n", func, pbc.LengthDWs,
		     pbc.LengthDWs * sizeof(uint32_t));
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.Vl = %#x\n", func, pbc.Vl);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.PortIdx = %#x %s\n", func, pbc.PortIdx,
		     pbc.PortIdx & OPX_PBC_JKR_PORT_LOOPBACK_MASK ? "Loopback" : "");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.Reserved_2 = %#x\n", func, pbc.Reserved_2);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.L2Compressed = %#x\n", func, pbc.L2Compressed);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.L2Type = %#x\n", func, pbc.L2Type);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.Fecnd = %#x\n", func, pbc.Fecnd);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.TestBadLcrc = %#x\n", func, pbc.TestBadLcrc);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.InsertNon9bIcrc = %#x\n", func, pbc.InsertNon9bIcrc);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.CreditReturn = %#x\n", func, pbc.CreditReturn);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.InsertHcrc = %#x\n", func, pbc.InsertHcrc);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.Reserved_1 = %#x\n", func, pbc.Reserved_1);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.TestEbp = %#x\n", func, pbc.TestEbp);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.Sc4 = %#x\n", func, pbc.Sc4);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.Intr = %#x\n", func, pbc.Intr);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.Dlid = %#x %u\n", func, pbc.Dlid, pbc.Dlid);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: Pbc.SendCtxt = %#x\n", func, pbc.SendCtxt);
}

void opx_print_9B_pbc(uint64_t pbc1, const enum opx_hfi1_type hfi1_type, const char *func)
{
	/* WFR 9B is different */
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s %s %s\n", func, OPX_HFI1_TYPE_STRING(hfi1_type),
		     OPX_HFI1_TYPE_STRING(OPX_SW_HFI1_TYPE));
	if (hfi1_type & OPX_HFI1_WFR) {
		return opx_wfr_print_9B_pbc(pbc1, func);
	}
	/* JKR 9B is same as JKR 16B */
	return opx_print_16B_pbc(pbc1, func);
}

void opx_jkr_print_16B_lrh(uint64_t lrh1, uint64_t lrh2, const char *func)
{
	__attribute__((__unused__)) union opx_hfi1_packet_hdr hdr;
	hdr.lrh_16B.qw[0] = lrh1;
	hdr.lrh_16B.qw[1] = lrh2;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH    = %#16.16lX\n", func, lrh1);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH    = %#16.16lX\n", func, lrh2);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.slid = %#x\n", func, hdr.lrh_16B.slid);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.pktlen = %#x %zu\n", func, hdr.lrh_16B.pktlen,
		     hdr.lrh_16B.pktlen * sizeof(uint64_t));
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.b = %#x\n", func, hdr.lrh_16B.b);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.dlid = %#x\n", func, hdr.lrh_16B.dlid);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.sc = %#x\n", func, hdr.lrh_16B.sc);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.rc = %#x\n", func, hdr.lrh_16B.rc);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.f = %#x\n", func, hdr.lrh_16B.f);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.l2 = %#x\n", func, hdr.lrh_16B.l2);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.lt = %#x\n", func, hdr.lrh_16B.lt);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.l4 = %#x\n", func, hdr.lrh_16B.l4);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.slid20 = %#x\n", func, hdr.lrh_16B.slid20);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.dlid20 = %#x\n", func, hdr.lrh_16B.dlid20);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.pkey = %#x\n", func, hdr.lrh_16B.pkey);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.entropy = %#x\n", func, hdr.lrh_16B.entropy);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.age = %#x\n", func, hdr.lrh_16B.age);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.cspec = %#x\n", func, hdr.lrh_16B.cspec);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.r = %#x\n", func, hdr.lrh_16B.r);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.SLID(full) = %#6.6x  (BE format = %#6.6x)\n", func,
		     hdr.lrh_16B.slid20 << 20 | hdr.lrh_16B.slid,
		     htons(((hdr.lrh_16B.slid20 << 20) | hdr.lrh_16B.slid)));
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: LRH.DLID(full) = %#6.6x  (BE format = %#6.6x)\n", func,
		     hdr.lrh_16B.dlid20 << 20 | hdr.lrh_16B.dlid,
		     htons(((hdr.lrh_16B.dlid20 << 20) | hdr.lrh_16B.dlid)));
}

void opx_jkr_print_16B_bth(uint64_t bth1, uint64_t bth2, const char *func)
{
	__attribute__((__unused__)) union opx_hfi1_packet_hdr hdr;
	hdr.qw_16B[2] = bth1;
	hdr.qw_16B[3] = bth2;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: BTH    = %#16.16lX\n", func, bth1);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: BTH    = %#16.16lX\n", func, bth2);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: BTH.opcode = %#x\n", func, hdr.bth.opcode);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: BTH.bth_1 = %#x\n", func, hdr.bth.bth_1);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: BTH.pkey = %#x\n", func, hdr.bth.pkey);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: BTH.ecn = %#x, RC2 %d\n", func, hdr.bth.ecn,
		     (hdr.bth.ecn & OPX_RC2_MASK) >> 2);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: BTH.qp = %#x\n", func, hdr.bth.qp);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: BTH.subctxt_rx  = %#x\n", func, hdr.bth.subctxt_rx);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%s: BTH.psn = %#x\n", func, hdr.bth.psn);
}

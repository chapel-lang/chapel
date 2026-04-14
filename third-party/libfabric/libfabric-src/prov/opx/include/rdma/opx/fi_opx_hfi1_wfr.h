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
#ifndef _OPX_HFI1_WFR_H_
#define _OPX_HFI1_WFR_H_

/* WFR unused define (documentation) */
#define OPX_PBC_WFR_STATICRCC_SHIFT 0	   /* HFI_PBC_STATICRCC_SHIFT   */
#define OPX_PBC_WFR_STATICRCC_MASK  0xffff /* HFI_PBC_STATICRCC_MASK    */

/* Fields that unused on WFR (zero will be OR'd) */
#define OPX_PBC_WFR_UNUSED 0UL

#define OPX_PBC_WFR_DLID(_dlid)	     OPX_PBC_WFR_UNUSED
#define OPX_PBC_WFR_GET_DLID(_dlid)  OPX_PBC_WFR_UNUSED
#define OPX_PBC_WFR_SCTXT(_ctx)	     OPX_PBC_WFR_UNUSED
#define OPX_PBC_WFR_L2COMPRESSED(_c) OPX_PBC_WFR_UNUSED
#define OPX_PBC_WFR_PORTIDX(_pidx)   OPX_PBC_WFR_UNUSED

#ifndef NDEBUG
/* Debug only JKR definition for this sanity check */
#define _OPX_PBC_JKR_L2TYPE_9B_ 0b11

__OPX_FORCE_INLINE__
uint32_t opx_pbc_wfr_l2type(unsigned _type)
{
	return OPX_PBC_WFR_UNUSED;
}
#define OPX_PBC_WFR_L2TYPE(_type) opx_pbc_wfr_l2type(_type)
#else
#define OPX_PBC_WFR_L2TYPE(_type) OPX_PBC_WFR_UNUSED
#endif

#define OPX_PBC_WFR_RUNTIME(_dlid, _pidx) OPX_PBC_WFR_UNUSED

/* Unused WFR field - always initialized with PBC to 0.
  #define OPX_PBC_STATICRCC(srcc) (((unsigned long long)(dlid & OPX_PBC_WFR_STATICRCC_MASK) <<
  OPX_PBC_WFR_STATICRCC_SHIFT) << OPX_MSB_SHIFT)
 */

/* WFR
 *
 * The RHF.RcvSeq field is located in LSB bits [31:28] and values are in
 * the range of (1..13) inclusive. A new packet is available when the
 * expected sequence number in the next header queue element matches
 * the RHF.RcvSeq field.
 *
 * Instead of shifting and masking the RHF bits to retrieve the
 * sequence number in the range of 1..13 (or, 0x1..0xD) use only a bit
 * mask to obtain the RHF sequence in the range of 0x10000000..0xD0000000.
 * In this scheme the expected sequence number is incremented by
 * 0x10000000 instead of 0x1.
 */
#define OPX_WFR_RHF_SEQ_UPDATE(_seq, _rhf)	      (_seq | (_rhf & ~0x00000000F0000000ul))
#define OPX_WFR_RHF_SEQ_NOT_MATCH(_seq, _rhf)	      (_seq != (_rhf & 0xF0000000ul))
#define OPX_WFR_RHF_SEQ_INCREMENT(_seq)		      ((_seq < 0xD0000000ul) * _seq + 0x10000000ul)
#define OPX_WFR_IS_ERRORED_RHF(_rhf, _hfi1_type)      (_rhf & 0xBFE0000000000000ul)
#define OPX_WFR_RHF_SEQ_MATCH(_seq, _rhf, _hfi1_type) (_seq == (_rhf & 0xF0000000ul))
#define OPX_WFR_RHF_SEQ_INIT_VAL		      (0x10000000ul)
#define OPX_WFR_RHF_IS_USE_EGR_BUF(_rhf)	      ((_rhf & 0x00008000ul) == 0x00008000ul)
#define OPX_WFR_RHF_EGRBFR_INDEX_MASK		      (0xFFF)
#define OPX_WFR_RHF_EGRBFR_INDEX_SHIFT		      (16)
#define OPX_WFR_RHF_EGRBFR_OFFSET_SHIFT		      (32)
#define OPX_WFR_RHF_EGRBFR_OFFSET_MASK		      (0x0000000000000FFFul)
#define OPX_WFR_RHF_EGR_INDEX(_rhf)		      ((_rhf >> OPX_WFR_RHF_EGRBFR_INDEX_SHIFT) & OPX_WFR_RHF_EGRBFR_INDEX_MASK)
#define OPX_WFR_RHF_EGR_INDEX_UPDATE(_rhf, index)                                      \
	(((index & OPX_WFR_RHF_EGRBFR_INDEX_MASK) << OPX_WFR_RHF_EGRBFR_INDEX_SHIFT) | \
	 (_rhf & ~(OPX_WFR_RHF_EGRBFR_INDEX_MASK << OPX_WFR_RHF_EGRBFR_INDEX_SHIFT)))
#define OPX_WFR_RHF_EGR_OFFSET_UPDATE(_rhf, offset)                                       \
	(((offset & OPX_WFR_RHF_EGRBFR_OFFSET_MASK) << OPX_WFR_RHF_EGRBFR_OFFSET_SHIFT) | \
	 (_rhf & ~(OPX_WFR_RHF_EGRBFR_OFFSET_MASK << OPX_WFR_RHF_EGRBFR_OFFSET_SHIFT)))
#define OPX_WFR_RHF_EGR_OFFSET(_rhf)  ((_rhf >> OPX_WFR_RHF_EGRBFR_OFFSET_SHIFT) & OPX_WFR_RHF_EGRBFR_OFFSET_MASK)
#define OPX_WFR_RHF_HDRQ_OFFSET(_rhf) ((_rhf >> (32 + 12)) & 0x01FFul)

#define OPX_WFR_RHF_ICRCERR    (0x80000000u)
#define OPX_WFR_RHF_ECCERR     (0x20000000u)
#define OPX_WFR_RHF_LENERR     (0x10000000u)
#define OPX_WFR_RHF_TIDERR     (0x08000000u)
#define OPX_WFR_RHF_RCVTYPEERR (0x07000000u)
#define OPX_WFR_RHF_DCERR      (0x00800000u)
#define OPX_WFR_RHF_DCUNCERR   (0x00400000u)
#define OPX_WFR_RHF_KHDRLENERR (0x00200000u)

struct fi_opx_ep;

void opx_wfr_rhe_debug(struct fi_opx_ep *opx_ep, volatile uint64_t *rhe_ptr, volatile uint32_t *rhf_ptr,
		       const uint32_t rhf_msb, const uint32_t rhf_lsb, const uint64_t rhf_seq,
		       const uint64_t hdrq_offset, const uint64_t rhf_rcvd, const union opx_hfi1_packet_hdr *const hdr,
		       const enum opx_hfi1_type hfi1_type, uint16_t last_egrbfr_index);

#define OPX_WFR_RHE_DEBUG(_opx_ep, _rhe_ptr, _rhf_ptr, _rhf_msb, _rhf_lsb, _rhf_seq, _hdrq_offset, _rhf_rcvd, _hdr, \
			  _hfi1_type, last_egrbfr_index)                                                            \
	opx_wfr_rhe_debug(_opx_ep, _rhe_ptr, _rhf_ptr, _rhf_msb, _rhf_lsb, _rhf_seq, _hdrq_offset, _rhf_rcvd, _hdr, \
			  _hfi1_type, last_egrbfr_index)

// Common to both JKR/WFR

#define OPX_WFR_RHF_RCV_TYPE_EXPECTED_RCV(_rhf) ((_rhf & 0x00007000ul) == 0x00000000ul)
#define OPX_WFR_RHF_RCV_TYPE_EAGER_RCV(_rhf)	((_rhf & 0x00001000ul) == 0x00001000ul)
#define OPX_WFR_RHF_RCV_TYPE_OTHER(_rhf)	((_rhf & 0x00006000ul) != 0x00000000ul)

/* Common handler for WFR/JKR missing eager payload */
int opx_rhf_missing_payload_error_handler(const uint64_t rhf_rcvd, const union opx_hfi1_packet_hdr *const hdr,
					  const enum opx_hfi1_type hfi1_type);

__OPX_FORCE_INLINE__ int opx_wfr_rhf_check_header(const uint64_t rhf_rcvd, const union opx_hfi1_packet_hdr *const hdr,
						  const enum opx_hfi1_type hfi1_type)
{
	/* RHF error */
	if (OFI_UNLIKELY(OPX_WFR_IS_ERRORED_RHF(rhf_rcvd, hfi1_type))) {
		/* Warn later */
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "HEADER ERROR %s %#lX\n",
			     OPX_HFI1_TYPE_STRING(hfi1_type), OPX_WFR_IS_ERRORED_RHF(rhf_rcvd, hfi1_type));
		return 1; /* error */
	}

	/* Bad packet header */
	if (OFI_UNLIKELY((!OPX_WFR_RHF_IS_USE_EGR_BUF(rhf_rcvd)) && (ntohs(hdr->lrh_9B.pktlen) > 0x15) &&
			 !(OPX_WFR_RHF_RCV_TYPE_EXPECTED_RCV(rhf_rcvd)))) {
		/* Warn later */
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "HEADER ERROR MISSING PAYLOAD %s %#lX\n",
			     OPX_HFI1_TYPE_STRING(hfi1_type), OPX_WFR_IS_ERRORED_RHF(rhf_rcvd, hfi1_type));
		return opx_rhf_missing_payload_error_handler(rhf_rcvd, hdr, hfi1_type); /* error */
	} else {
		return 0; /* no error*/
	}
}

#define OPX_WFR_RHF_CHECK_HEADER(_rhf_rcvd, _hdr, _hfi1_type) opx_wfr_rhf_check_header(_rhf_rcvd, _hdr, _hfi1_type)

union opx_wfr_pbc {
	uint64_t raw64b;
	uint32_t raw32b[2];

	__le64 qw;
	__le32 dw[2];
	__le16 w[4];

	struct {
		__le64 LengthDWs : 12;
		__le64 Vl : 4;
		__le64 Reserved_2 : 6;
		__le64 Fecn : 1;
		__le64 TestBadLcrc : 1;
		__le64 InsertNon9bIcrc : 1;
		__le64 CreditReturn : 1;
		__le64 InsertHcrc : 2;
		__le64 PacketBypass : 1;
		__le64 TestEbp : 1;
		__le64 Sc4 : 1;
		__le64 Intr : 1;
		__le64 StaticRateControl : 16;
		__le64 Reserved_1 : 16;
	};
};

void opx_wfr_print_9B_pbc(uint64_t pbc1, const char *func);

#endif

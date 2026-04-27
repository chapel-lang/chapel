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
#ifndef _OPX_HFI1_VERSION_H_
#define _OPX_HFI1_VERSION_H_

#include "rdma/opx/fi_opx_hfi1_wfr.h"
#include "rdma/opx/fi_opx_hfi1_jkr.h"

/*******************************************/
/* These are the same defines both WFR/JKR */
/*******************************************/
// RHF changes
// Common to both JKR/WFR

#define OPX_RHF_RCV_TYPE_EXPECTED_RCV(_rhf, _noop) ((_rhf & 0x00007000ul) == 0x00000000ul)
#define OPX_RHF_RCV_TYPE_EAGER_RCV(_rhf, _noop)	   ((_rhf & 0x00001000ul) == 0x00001000ul)
#define OPX_RHF_RCV_TYPE_OTHER(_rhf, _noop)	   ((_rhf & 0x00006000ul) != 0x00000000ul)

#define OPX_PBC_CR(_cr, _noop)	 ((_cr & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT)
#define OPX_PBC_LEN(_len, _noop) (_len)
#define OPX_PBC_VL(_vl, _noop)	 ((_vl & FI_OPX_HFI1_PBC_VL_MASK) << FI_OPX_HFI1_PBC_VL_SHIFT)

/* Note: double check JKR sc bits */
#define OPX_PBC_SC(_sc, _noop) \
	(((_sc >> FI_OPX_HFI1_PBC_SC4_SHIFT) & FI_OPX_HFI1_PBC_SC4_MASK) << FI_OPX_HFI1_PBC_DCINFO_SHIFT)

/* PBC most significant bits shift (32 bits) defines */
#define OPX_MSB_SHIFT 32

/***************************************************************/
/* Both JKR and WFR runtime is now supported (no longer doing  */
/* build-time constants)                                       */
/*                                                             */
/* Runtime support relies on a local variable "hfi1_type",     */
/*  which is likely passed down through macro and function     */
/*  constants which are selected/optimized inline with         */
/*  function tables.                                           */
/***************************************************************/

#define OPX_PBC_DLID(_lid, _hfi1_type) ((_hfi1_type & OPX_HFI1_WFR) ? OPX_PBC_WFR_DLID(_lid) : OPX_PBC_JKR_DLID(_lid))
#define OPX_PBC_GET_DLID(_lid, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_PBC_WFR_GET_DLID(_lid) : OPX_PBC_JKR_GET_DLID(_lid))

#define OPX_PBC_SCTXT(_ctx, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_PBC_WFR_SCTXT(_ctx) : OPX_PBC_JKR_SCTXT(_ctx))

#define OPX_PBC_L2COMPRESSED(_c, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_PBC_WFR_L2COMPRESSED(_c) : OPX_PBC_JKR_L2COMPRESSED(_c))

#ifndef NDEBUG
#define OPX_PBC_LOOPBACK(_pbc_lid, _hfi1_type)                                                                         \
	({                                                                                                             \
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,                                                             \
		       "OPX_PBC_LOOPBACK %s:%u:%s pbc_lid %#llX/%#llX == slid %#llX LOOPBACK MASK %#llX\n", __func__,  \
		       fi_opx_global.hfi_local_info.sriov, OPX_HFI1_TYPE_STRING(_hfi1_type), (long long int) _pbc_lid, \
		       (long long int) fi_opx_global.hfi_local_info.pbc_lid,                                           \
		       (long long int) fi_opx_global.hfi_local_info.lid,                                               \
		       (long long int) ((_hfi1_type & OPX_HFI1_WFR) ? OPX_PBC_WFR_PORTIDX(_hfi1_type) :                \
								      OPX_PBC_JKR_LOOPBACK_PORTIDX(_pbc_lid)));        \
		assert((_pbc_lid != fi_opx_global.hfi_local_info.pbc_lid) || fi_opx_global.hfi_local_info.sriov);      \
		((_hfi1_type & OPX_HFI1_WFR) ? OPX_PBC_WFR_PORTIDX(_hfi1_type) :                                       \
					       OPX_PBC_JKR_LOOPBACK_PORTIDX(_pbc_lid));                                \
	})
#else
#define OPX_PBC_LOOPBACK(_pbc_lid, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_PBC_WFR_PORTIDX(_hfi1_type) : OPX_PBC_JKR_LOOPBACK_PORTIDX(_pbc_lid))
#endif

#define OPX_PBC_PORTIDX(_pidx, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_PBC_WFR_PORTIDX(_pidx) : OPX_PBC_JKR_PORTIDX(_pidx))

#define OPX_PBC_L2TYPE(_type, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_PBC_WFR_L2TYPE(_type) : OPX_PBC_JKR_L2TYPE(_type))

/* One runtime check for mutiple fields - DLID, PORT, L2TYPE */
#define OPX_PBC_RUNTIME(_dlid, _pidx, _hfi1_type)                                               \
	((_hfi1_type & OPX_HFI1_WFR) ? (OPX_PBC_WFR_DLID(_dlid) | OPX_PBC_WFR_PORTIDX(_pidx)) : \
				       (OPX_PBC_JKR_DLID(_dlid) | OPX_PBC_JKR_PORTIDX(_pidx)))

/* Common BTH defines */

#define OPX_BTH_UNUSED 0 // Default unsupported values to 0

#define OPX_BTH_CSPEC(_cspec, _hfi1_type) ((_hfi1_type & OPX_HFI1_CNX000) ? OPX_BTH_JKR_CSPEC(_cspec) : OPX_BTH_UNUSED)
#define OPX_BTH_CSPEC_DEFAULT		  OPX_BTH_UNUSED // Cspec is not used in 9B header

/* "RC[2]" is MSB bit of the 3 bit RC value. It is in the bth */
#define OPX_BTH_RC2_VAL(_hfi1_type, _pkt_type) (OPX_ROUTE_CONTROL_VALUE(_hfi1_type, _pkt_type) & OPX_RC2_MASK)

#define OPX_BTH_SUBCTXT_RX_SHIFT 48
#define OPX_BTH_SUBCTXT_RX_MASK	 0xFF07

#define OPX_BTH_INJECT_MSG_LENGTH(_val)	   ((_val >> 3) & 0x1F)
#define OPX_BTH_SEND_XFER_BYTES_TAIL(_val) ((_val >> 3) & 0x1F)

/* Common RHF defines */

#define OPX_RHF_SEQ_NOT_MATCH(_seq, _rhf, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_SEQ_NOT_MATCH(_seq, _rhf) : OPX_JKR_RHF_SEQ_NOT_MATCH(_seq, _rhf))

#define OPX_RHF_SEQ_INCREMENT(_seq, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_SEQ_INCREMENT(_seq) : OPX_JKR_RHF_SEQ_INCREMENT(_seq))

#define OPX_IS_ERRORED_RHF(_rhf, _hfi1_type)                                      \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_IS_ERRORED_RHF(_rhf, _hfi1_type) : \
				       OPX_JKR_IS_ERRORED_RHF(_rhf, _hfi1_type))

#define OPX_RHF_SEQ_UPDATE(_rhf, _seq, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_SEQ_UPDATE(_seq, _rhf) : OPX_JKR_RHF_SEQ_UPDATE(_seq, _rhf))

#define OPX_RHF_SEQ_MATCH(_seq, _rhf, _hfi1_type)                                      \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_SEQ_MATCH(_seq, _rhf, _hfi1_type) : \
				       OPX_JKR_RHF_SEQ_MATCH(_seq, _rhf, _hfi1_type))

/* Init-time, let it use the variable - not optimized */
#define OPX_RHF_SEQ_INIT_VAL(_hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_SEQ_INIT_VAL : OPX_JKR_RHF_SEQ_INIT_VAL)

#define OPX_RHF_IS_USE_EGR_BUF(_rhf, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_IS_USE_EGR_BUF(_rhf) : OPX_JKR_RHF_IS_USE_EGR_BUF(_rhf))

#define OPX_RHF_EGR_INDEX(_rhf, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_EGR_INDEX(_rhf) : OPX_JKR_RHF_EGR_INDEX(_rhf))

#define OPX_RHF_EGR_INDEX_UPDATE(_rhf, index, _hfi1_type)                          \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_EGR_INDEX_UPDATE(_rhf, index) : \
				       OPX_JKR_RHF_EGR_INDEX_UPDATE(_rhf, index))

#define OPX_RHF_EGR_OFFSET_UPDATE(_rhf, offset, _hfi1_type)                          \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_EGR_OFFSET_UPDATE(_rhf, offset) : \
				       OPX_JKR_RHF_EGR_OFFSET_UPDATE(_rhf, offset))

#define OPX_RHF_EGR_OFFSET(_rhf, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_EGR_OFFSET(_rhf) : OPX_JKR_RHF_EGR_OFFSET(_rhf))

#define OPX_RHF_HDRQ_OFFSET(_rhf, _hfi1_type) \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_HDRQ_OFFSET(_rhf) : OPX_JKR_RHF_HDRQ_OFFSET(_rhf))

#define OPX_RHE_DEBUG(_opx_ep, _rhe_ptr, _rhf_ptr, _rhf_msb, _rhf_lsb, _rhf_seq, _hdrq_offset, _rhf_rcvd, _hdr,        \
		      _hfi1_type, last_egrbfr_index)                                                                   \
	((_hfi1_type & OPX_HFI1_WFR) ?                                                                                 \
		 OPX_WFR_RHE_DEBUG(_opx_ep, _rhe_ptr, _rhf_ptr, _rhf_msb, _rhf_lsb, _rhf_seq, _hdrq_offset, _rhf_rcvd, \
				   _hdr, _hfi1_type, last_egrbfr_index) :                                              \
		 OPX_JKR_RHE_DEBUG(_opx_ep, _rhe_ptr, _rhf_ptr, _rhf_msb, _rhf_lsb, _rhf_seq, _hdrq_offset, _rhf_rcvd, \
				   _hdr, _hfi1_type, last_egrbfr_index))

#define OPX_RHF_CHECK_HEADER(_rhf_rcvd, _pktlen, _hfi1_type)                                      \
	((_hfi1_type & OPX_HFI1_WFR) ? OPX_WFR_RHF_CHECK_HEADER(_rhf_rcvd, _pktlen, _hfi1_type) : \
				       OPX_JKR_RHF_CHECK_HEADER(_rhf_rcvd, _pktlen, _hfi1_type))

#define OPX_HEADER_SIZE (8 * 8) // doesn't include PBC. For 9B it includes the unused_pad qw.

#endif

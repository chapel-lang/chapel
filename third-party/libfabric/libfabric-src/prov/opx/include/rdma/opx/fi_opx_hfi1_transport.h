/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2025 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_HFI1_TRANSPORT_H_
#define _FI_PROV_OPX_HFI1_TRANSPORT_H_

#ifndef FI_OPX_FABRIC_HFI1
#error "fabric selection #define error"
#endif

#include "rdma/opx/fi_opx_hfi1.h"
#include <ofi_list.h>
#include <rdma/hfi/hfi1_user.h>
#include "rdma/opx/fi_opx_hmem.h"

#include "rdma/opx/opx_tracer.h"

union opx_hmem_event;

/*
 * ==== NOTE_COMPLETION_TYPES ====
 *
 * FI_INJECT_COMPLETE generates the completion entry when the
 * source buffer can be reused. This can be immedately done if
 * the entire source buffer is copied into the reliability
 * replay buffer(s). Otherwise the completion should not be
 * generated until the reliability protocol completes.
 *
 * FI_TRANSMIT_COMPLETE completion entry should only be generated
 * when "the operation is no longer dependent on local resources".
 * Does this means that it should be delivered only when the
 * reliability protocol has completed? If so, then the completion
 * may be delayed significantly, or the reliability protocol
 * needs to be enhanced for the target to do an 'immediate ack'
 * when this packet is received. Regardless, MPICH only uses
 * this completion type when performing a 'dynamic process
 * disconnect' so it is not critical for this completion type
 * to have good performance at this time.
 *
 * FI_DELIVERY_COMPLETE is not supposed to generate a completion
 * event until the send has been "processed by the destination
 * endpoint(s)". The reliability protocol has nothing to do with
 * that acknowledgement.
 *
 * OPX supports FI_DELIVERY_COMPLETE when required but will use
 * FI_INJECT_COMPLETE when possible.
 */

// Function for performing FI_INJECT_COMPLETIONs.
__OPX_FORCE_INLINE__
ssize_t fi_opx_ep_tx_cq_inject_completion(struct fid_ep *ep, void *user_context, const size_t len,
					  const int lock_required, const uint64_t tag, const uint64_t caps)
{
	/*
	 * This is the default completion type, used for all
	 * sends that do not require delivery completion.
	 *
	 * See NOTE_COMPLETION_TYPES for more information.
	 */

	/* initialize the completion entry */
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	assert((caps & (FI_TAGGED | FI_MSG)) != (FI_TAGGED | FI_MSG));

	struct opx_context *context = (struct opx_context *) ofi_buf_alloc(opx_ep->rx->ctx_pool);
	if (OFI_UNLIKELY(context == NULL)) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
		return -FI_ENOMEM;
	}

	context->err_entry.err	      = 0;
	context->err_entry.op_context = user_context;
	context->flags		      = FI_SEND | (caps & (FI_TAGGED | FI_MSG));
	context->len		      = len;
	context->buf		      = NULL; /* receive data buffer */
	context->byte_counter	      = 0;
	context->tag		      = tag;
	context->next		      = NULL;

	if (lock_required) {
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "=================== TX CQ COMPLETION QUEUED\n");
	slist_insert_tail((struct slist_entry *) context, opx_ep->tx->cq_completed_ptr);

	return FI_SUCCESS;
}

/*
 * The following family of opx-cacheline* functions can be used to copy or move
 * data to a single cacheline.
 *
 * The naming convention used is as follows:
 *
 *  - 'opx_cacheline'   : The prefix of all these functions
 *  - 'store' or 'copy' : 'store' functions write the data to the target cacheline only.
 *                        'copy' functions also write the data to local storage, like a replay buffer
 *  - 'block' or 'qw'   : 'block' functions take a single uint64_t * that points to an array of 8 uint64_ts
 *                        'qw' functions take 8 individual uint64_t parms
 *  - 'vol'             : Suffix indicating the target cacheline being written to is volatile (e.g. an SCB)
 *
 */
__OPX_FORCE_INLINE__
void opx_cacheline_store_block(uint64_t dest[restrict 8], const uint64_t source[restrict 8])
{
	dest[0] = source[0];
	dest[1] = source[1];
	dest[2] = source[2];
	dest[3] = source[3];
	dest[4] = source[4];
	dest[5] = source[5];
	dest[6] = source[6];
	dest[7] = source[7];
}

__OPX_FORCE_INLINE__
void opx_cacheline_store_block_vol(volatile uint64_t dest[restrict 8], const uint64_t source[restrict 8])
{
	OPX_HFI1_BAR_PIO_STORE(&dest[0], source[0]);
	OPX_HFI1_BAR_PIO_STORE(&dest[1], source[1]);
	OPX_HFI1_BAR_PIO_STORE(&dest[2], source[2]);
	OPX_HFI1_BAR_PIO_STORE(&dest[3], source[3]);
	OPX_HFI1_BAR_PIO_STORE(&dest[4], source[4]);
	OPX_HFI1_BAR_PIO_STORE(&dest[5], source[5]);
	OPX_HFI1_BAR_PIO_STORE(&dest[6], source[6]);
	OPX_HFI1_BAR_PIO_STORE(&dest[7], source[7]);
}

__OPX_FORCE_INLINE__
void opx_cacheline_store_qw_vol(volatile uint64_t dest[8], const uint64_t d0, const uint64_t d1, const uint64_t d2,
				const uint64_t d3, const uint64_t d4, const uint64_t d5, const uint64_t d6,
				const uint64_t d7)
{
	OPX_HFI1_BAR_PIO_STORE(&dest[0], d0);
	OPX_HFI1_BAR_PIO_STORE(&dest[1], d1);
	OPX_HFI1_BAR_PIO_STORE(&dest[2], d2);
	OPX_HFI1_BAR_PIO_STORE(&dest[3], d3);
	OPX_HFI1_BAR_PIO_STORE(&dest[4], d4);
	OPX_HFI1_BAR_PIO_STORE(&dest[5], d5);
	OPX_HFI1_BAR_PIO_STORE(&dest[6], d6);
	OPX_HFI1_BAR_PIO_STORE(&dest[7], d7);
}

__OPX_FORCE_INLINE__
void opx_cacheline_copy_block_vol(volatile uint64_t dest[restrict 8], uint64_t local[restrict 8],
				  const uint64_t source[restrict 8])
{
	OPX_HFI1_BAR_PIO_STORE(&dest[0], source[0]);
	OPX_HFI1_BAR_PIO_STORE(&dest[1], source[1]);
	OPX_HFI1_BAR_PIO_STORE(&dest[2], source[2]);
	OPX_HFI1_BAR_PIO_STORE(&dest[3], source[3]);
	OPX_HFI1_BAR_PIO_STORE(&dest[4], source[4]);
	OPX_HFI1_BAR_PIO_STORE(&dest[5], source[5]);
	OPX_HFI1_BAR_PIO_STORE(&dest[6], source[6]);
	OPX_HFI1_BAR_PIO_STORE(&dest[7], source[7]);
	local[0] = source[0];
	local[1] = source[1];
	local[2] = source[2];
	local[3] = source[3];
	local[4] = source[4];
	local[5] = source[5];
	local[6] = source[6];
	local[7] = source[7];
}

__OPX_FORCE_INLINE__
void opx_cacheline_copy_qw_vol(volatile uint64_t dest[restrict 8], uint64_t local[restrict 8], const uint64_t d0,
			       const uint64_t d1, const uint64_t d2, const uint64_t d3, const uint64_t d4,
			       const uint64_t d5, const uint64_t d6, const uint64_t d7)
{
	OPX_HFI1_BAR_PIO_STORE(&dest[0], d0);
	OPX_HFI1_BAR_PIO_STORE(&dest[1], d1);
	OPX_HFI1_BAR_PIO_STORE(&dest[2], d2);
	OPX_HFI1_BAR_PIO_STORE(&dest[3], d3);
	OPX_HFI1_BAR_PIO_STORE(&dest[4], d4);
	OPX_HFI1_BAR_PIO_STORE(&dest[5], d5);
	OPX_HFI1_BAR_PIO_STORE(&dest[6], d6);
	OPX_HFI1_BAR_PIO_STORE(&dest[7], d7);
	local[0] = d0;
	local[1] = d1;
	local[2] = d2;
	local[3] = d3;
	local[4] = d4;
	local[5] = d5;
	local[6] = d6;
	local[7] = d7;
}

// fi_opx_duff_copy --A function to handle a fast memcpy of non-trival byte length (like 8 or 16) in the scb copy
// crtical path
//  Pre: Only called by fi_opx_set_scb_special()
//  Pre: NEVER call with length 0, 8, or 16 (Use long = long for 8 and 16)
//  Post: len number of bytes are touched in both buffers.
//  Post: arg0 buffer will be equal to arg1 buffer for arg2 bytes
__OPX_FORCE_INLINE__
void fi_opx_duff_copy(uint8_t *restrict to, const uint8_t *restrict from, int64_t len)
{
	assert(len > 0);
	assert(len < 16);
	assert(len != 8);
	switch (len & 15) { // (len % 16) power of 2
			    // Does not handle 0, save some code gen
	case 15:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 14:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 13:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 12:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 11:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 10:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 9:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 8:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 7:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 6:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 5:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 4:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 3:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 2:
		*to++ = *from++;
		__attribute__((fallthrough));
	case 1:
		*to++ = *from++;
	}
}

__OPX_FORCE_INLINE__
void opx_copy_double_qw(uint64_t *restrict target, const uint8_t *restrict buf, const size_t len)
{
	assert(len <= 16);

	switch (len) {
	case 0:
		target[0] = 0;
		target[1] = 0;
		break;
	case 8:
		target[0] = *((uint64_t *) buf);
		target[1] = 0;
		break;
	case 16:
		target[0] = *((uint64_t *) buf);
		target[1] = *((uint64_t *) buf + 1);
		break;
	default:
		target[0] = 0;
		target[1] = 0;
		fi_opx_duff_copy((uint8_t *) target, buf, len);
	};
}

void fi_opx_hfi1_rx_rzv_rts_etrunc(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr,
				   const uint16_t origin_rx, uintptr_t origin_byte_counter_vaddr, const unsigned is_shm,
				   const enum ofi_reliability_kind reliability, const uint32_t u32_extended_rx,
				   const enum opx_hfi1_type hfi1_type);

void fi_opx_hfi1_rx_rzv_rts(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr,
			    const void *const payload, const uint16_t origin_rx, const uint64_t niov,
			    uintptr_t origin_byte_counter_vaddr, struct opx_context *const target_context,
			    const uintptr_t dst_vaddr, const enum fi_hmem_iface dst_iface, const uint64_t dst_device,
			    const uint64_t immediate_data, const uint64_t immediate_end_bytes,
			    const struct fi_opx_hmem_iov *src_iovs, uint8_t opcode, const unsigned is_shm,
			    const enum ofi_reliability_kind reliability, const uint32_t u32_extended_rx,
			    const enum opx_hfi1_type hfi1_type);

#ifdef OPX_HMEM
void opx_hfi1_rx_ipc_rts(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr,
			 const union fi_opx_hfi1_packet_payload *const payload, const uint16_t origin_rx,
			 const uint64_t niov, const uint64_t is_hmem, struct opx_context *const context,
			 const uint64_t xfer_len, const uint32_t u32_extended_rx, const enum opx_hfi1_type hfi1_type);
#endif

union fi_opx_hfi1_deferred_work *
fi_opx_hfi1_rx_rzv_cts(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr, const void *const payload,
		       size_t payload_bytes_to_copy, const uint16_t origin_rx, const uint32_t niov,
		       const union opx_hfi1_dput_iov *const dput_iov, uint8_t *src_base_addr, const uint8_t op,
		       const uint8_t dt, const uintptr_t rma_request_vaddr, const uintptr_t target_byte_counter_vaddr,
		       uint64_t *origin_byte_counter, uint32_t						      op_kind,
		       void (*completion_action)(union fi_opx_hfi1_deferred_work *work_state), const unsigned is_shm,
		       const enum ofi_reliability_kind reliability, const uint32_t u32_extended_rx,
		       const enum opx_hfi1_type hfi1_type);

union fi_opx_hfi1_deferred_work;
struct fi_opx_work_elem {
	struct slist_entry slist_entry;
	int (*work_fn)(union fi_opx_hfi1_deferred_work *work_state);
	void (*completion_action)(union fi_opx_hfi1_deferred_work *work_state);
	enum opx_work_type		  work_type;
	uint8_t				  unused[3];
	bool				  complete;
	union fi_opx_hfi1_packet_payload *payload_copy;
} __attribute__((__aligned__(8))) __attribute__((__packed__));

struct fi_opx_hfi1_dput_params {
	struct fi_opx_work_elem		    work_elem;
	struct fi_opx_ep		   *opx_ep;
	uint8_t				   *src_base_addr;
	uint64_t			    lrh_dlid;
	uint64_t			    pbc_dlid;
	union opx_hfi1_dput_iov		   *dput_iov;
	void				   *fetch_vaddr;
	void				   *compare_vaddr;
	struct fi_opx_completion_counter   *cc;
	struct fi_opx_completion_counter   *user_cc;
	struct fi_opx_hfi1_sdma_work_entry *sdma_we;
	struct slist			    sdma_reqs;
	struct iovec			    tid_iov;
	union {
		uintptr_t target_byte_counter_vaddr;
		uintptr_t target_rma_request_vaddr;
	};
	uintptr_t		  rma_request_vaddr;
	uint64_t		 *origin_byte_counter;
	uint64_t		  key;
	uint64_t		  bytes_sent;
	uint64_t		  origin_bytes_sent;
	opx_lid_t		  slid;
	uint32_t		  niov;
	uint32_t		  cur_iov;
	uint32_t		  opcode;
	uint32_t		  payload_bytes_for_iovec;
	uint32_t		  ntidpairs;
	uint32_t		  tidoffset;
	uint32_t		  tididx;
	uint32_t		  tidbytes_consumed;
	uint32_t		  tidbytes_remaining;
	uint32_t		  u32_extended_rx;
	uint32_t		  unused;
	enum ofi_reliability_kind reliability;
	uint16_t		  sdma_reqs_used;
	uint16_t		  origin_rx; // rx of sender of CTS ( Where packets should be sent to during dput)
	uint16_t		  padding;

	bool	is_shm;
	bool	sdma_no_bounce_buf;
	bool	use_expected_opcode;
	uint8_t dt;
	uint8_t op;
	uint8_t target_hfi_unit;

	struct fi_opx_hmem_iov compare_iov;
	uint8_t		       inject_data[FI_OPX_HFI1_PACKET_IMM];
	uint32_t	       unused_padding[3];
	/* Either FI_OPX_MAX_DPUT_IOV iov's or
	   1 iov and FI_OPX_MAX_DPUT_TIDPAIRS tidpairs */
	union {
		union opx_hfi1_dput_iov iov[FI_OPX_MAX_DPUT_IOV];
		struct {
			union opx_hfi1_dput_iov reserved; /* skip 1 iov */
			uint32_t		tidpairs[FI_OPX_MAX_DPUT_TIDPAIRS];
		};
	};
} __attribute__((__aligned__(L2_CACHE_LINE_SIZE))) __attribute__((__packed__));
OPX_COMPILE_TIME_ASSERT((offsetof(struct fi_opx_hfi1_dput_params, compare_iov) & 7) == 0,
			"compare_iov not 8-byte aligned!");
OPX_COMPILE_TIME_ASSERT((offsetof(struct fi_opx_hfi1_dput_params, iov) & 63) == 0, "iov not 64-byte aligned!");

struct fi_opx_hfi1_rx_rzv_rts_params {
	/* == CACHE LINE 0 == */
	struct fi_opx_work_elem	      work_elem; // 40 bytes
	struct fi_opx_ep	     *opx_ep;
	struct fi_opx_rzv_completion *rzv_comp;
	uint64_t		      niov;

	/* == CACHE LINE 1 == */
	uint64_t  lrh_dlid;
	uint64_t  pbc_dlid;
	uintptr_t origin_byte_counter_vaddr;
	uintptr_t dst_vaddr; /* bumped past immediate data */

	opx_lid_t		  slid;
	uint32_t		  cur_iov;
	uint32_t		  u32_extended_rx;
	unsigned		  is_shm;
	enum ofi_reliability_kind reliability;
	uint32_t		  unused;

	uint16_t origin_rx; // subctxt_rx of sender of RTS
	uint8_t	 opcode;    // DPUT opcode and not BTH opcode
	uint8_t	 target_hfi_unit;
	uint8_t	 unused2[3];

	/* === Data above here WILL be copied when this struct     === */
	/* === is cloned for sending multiple CTS packets          === */
	uint8_t multi_cts_copy_boundary;
	/* === Data below here will NOT be copied when this struct === */
	/* === is cloned for sending multiple CTS packets          === */

	/* == CACHE LINE 2 == */
	struct {
		struct fi_opx_hmem_iov cur_addr_range;
		uint32_t	       npairs;
		uint32_t	       offset;
		int32_t		       origin_byte_counter_adj;
	} tid_info;
	uint64_t unused3[3];

	/* == CACHE LINE 3 == */
	union opx_hfi1_dput_iov elided_head;
	/* == CACHE LINE 4 == */
	union opx_hfi1_dput_iov elided_tail;

	/* Either FI_OPX_MAX_DPUT_IOV iov's or
	   1 iov and FI_OPX_MAX_DPUT_TIDPAIRS tidpairs */
	/* == CACHE LINE 5 == */
	union {
		union opx_hfi1_dput_iov dput_iov[FI_OPX_MAX_DPUT_IOV];
		struct {
			union opx_hfi1_dput_iov reserved; /* skip 1 iov */
			uint32_t		tidpairs[FI_OPX_MAX_DPUT_TIDPAIRS];
		};
	};
} __attribute__((__aligned__(L2_CACHE_LINE_SIZE))) __attribute__((__packed__));
OPX_COMPILE_TIME_ASSERT(sizeof(((struct fi_opx_hfi1_rx_rzv_rts_params *) 0)->dput_iov) < OPX_HFI1_MAX_PKT_SIZE,
			"sizeof(fi_opx_hfi1_rx_rzv_rts_params->dput_iov) should be < MAX PACKET MTU!");
OPX_COMPILE_TIME_ASSERT(
	sizeof(((struct fi_opx_hfi1_rx_rzv_rts_params *) 0)->tidpairs) <
		(OPX_HFI1_MAX_PKT_SIZE - sizeof(union opx_hfi1_dput_iov)),
	"sizeof(fi_opx_hfi1_rx_rzv_rts_params->tidpairs) should be < (MAX PACKET MTU - sizeof(dput iov)!");
OPX_COMPILE_TIME_ASSERT((offsetof(struct fi_opx_hfi1_rx_rzv_rts_params, tidpairs) & 0xF) == 0,
			"offsetof(fi_opx_hfi1_rx_rzv_rts_params->tidpairs) should be 16-byte aligned!");

struct opx_hfi1_rma_rts_params {
	/* == CACHE LINE 0 == */
	struct fi_opx_work_elem work_elem; // 40 bytes
	struct fi_opx_ep       *opx_ep;
	uint64_t		lrh_dlid;
	uint64_t		slid;

	/* == CACHE LINE 1 == */
	uint64_t		   pbc_dlid;
	uint64_t		   niov;
	struct fi_opx_rma_request *rma_req;
	struct fi_opx_rma_request *origin_rma_req;
	uint64_t		   key;
	uint32_t		   data;
	uint32_t		   u32_extended_rx;
	enum ofi_reliability_kind  reliability;

	union {
		uint16_t origin_rx;
		uint16_t dest_rx;
	};
	bool	is_shm;
	uint8_t opcode;
	uint8_t dt;
	uint8_t op;
	uint8_t target_hfi_unit;
	uint8_t unused[5];

	/* == CACHE LINE 2 == */
	union opx_hfi1_dput_iov dput_iov[FI_OPX_MAX_DPUT_IOV];
} __attribute__((__aligned__(L2_CACHE_LINE_SIZE))) __attribute__((__packed__));
OPX_COMPILE_TIME_ASSERT(offsetof(struct opx_hfi1_rma_rts_params, dput_iov) == FI_OPX_CACHE_LINE_SIZE * 2,
			"opx_hfi1_rma_rts_params->dput_iov should start on cacheline 2!");
OPX_COMPILE_TIME_ASSERT(sizeof(((struct opx_hfi1_rma_rts_params *) 0)->dput_iov) < OPX_HFI1_MAX_PKT_SIZE,
			"sizeof(opx_hfi1_rma_rts_params->dput_iov) should be < MAX PACKET MTU!");

struct opx_hfi1_rx_ipc_rts_params {
	/* == CACHE LINE 0 == */
	struct fi_opx_work_elem work_elem; // 40 bytes
	struct fi_opx_ep       *opx_ep;
	uintptr_t		origin_byte_counter_vaddr;
	uint64_t		niov;

	/* == CACHE LINE 1 == */
	uint64_t	      lrh_dlid;
	uint32_t	      u32_extended_rx;
	uint16_t	      origin_rx;
	uint8_t		      target_hfi_unit;
	uint8_t		      unused[1];
	struct opx_context   *context;
	struct ofi_mr_entry  *cache_entry;
	union opx_hmem_event *hmem_event;

} __attribute__((__aligned__(L2_CACHE_LINE_SIZE))) __attribute__((__packed__));
OPX_COMPILE_TIME_ASSERT(offsetof(struct opx_hfi1_rx_ipc_rts_params, lrh_dlid) == FI_OPX_CACHE_LINE_SIZE * 1,
			"opx_hfi1_rx_ipc_rts_params->lrh_dlid should start on cacheline 1!");

struct fi_opx_hfi1_rx_dput_fence_params {
	struct fi_opx_work_elem		  work_elem;
	struct fi_opx_ep		 *opx_ep;
	struct fi_opx_completion_counter *cc;
	uint64_t			  lrh_dlid;
	uint64_t			  bth_subctxt_rx;
	uint64_t			  bytes_to_fence;
	uint32_t			  u32_extended_rx;
	uint16_t			  origin_rx;
	uint8_t				  target_hfi_unit;
} __attribute__((__aligned__(L2_CACHE_LINE_SIZE))) __attribute__((__packed__));

struct fi_opx_hfi1_rx_readv_params {
	struct fi_opx_work_elem		  work_elem;
	struct fi_opx_ep		 *opx_ep;
	struct fi_opx_rma_request	 *rma_request;
	size_t				  niov;
	union opx_hfi1_dput_iov		  dput_iov;
	union fi_opx_addr		  opx_target_addr;
	struct fi_opx_completion_counter *cc;
	uint64_t			  key;
	uint64_t			  dest_subctxt_rx;
	uint32_t			  u32_extended_rx;
	uint32_t			  opcode;
	uint64_t			  lrh_dlid;
	uint64_t			  bth_subctxt_rx;
	uint64_t			  pbc_dws;
	uint64_t			  lrh_dws;
	uint64_t			  op;
	uint64_t			  dt;
	uint64_t			  pbc_dlid;
	enum ofi_reliability_kind	  reliability;
	bool				  is_shm;
} __attribute__((__aligned__(L2_CACHE_LINE_SIZE))) __attribute__((__packed__));

union fi_opx_hfi1_deferred_work {
	struct fi_opx_work_elem			work_elem;
	struct fi_opx_hfi1_dput_params		dput;
	struct fi_opx_hfi1_rx_rzv_rts_params	rx_rzv_rts;
	struct fi_opx_hfi1_rx_dput_fence_params fence;
	struct fi_opx_hfi1_rx_readv_params	readv;
	struct opx_hfi1_rma_rts_params		rma_rts;
	struct opx_hfi1_rx_ipc_rts_params	rx_ipc_rts;
} __attribute__((__aligned__(L2_CACHE_LINE_SIZE))) __attribute__((__packed__));

int  opx_hfi1_do_dput_fence(union fi_opx_hfi1_deferred_work *work);
void opx_hfi1_dput_fence(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr, const uint16_t origin_rx,
			 const uint32_t u32_extended_rx, const enum opx_hfi1_type hfi1_type);

int fi_opx_hfi1_do_dput_wfr(union fi_opx_hfi1_deferred_work *work);
int fi_opx_hfi1_do_dput_jkr(union fi_opx_hfi1_deferred_work *work);
int fi_opx_hfi1_do_dput_cyr(union fi_opx_hfi1_deferred_work *work);
int fi_opx_hfi1_do_dput_jkr_9B(union fi_opx_hfi1_deferred_work *work);

int fi_opx_hfi1_do_dput_sdma_wfr(union fi_opx_hfi1_deferred_work *work);
int fi_opx_hfi1_do_dput_sdma_jkr(union fi_opx_hfi1_deferred_work *work);
int fi_opx_hfi1_do_dput_sdma_cyr(union fi_opx_hfi1_deferred_work *work);
int fi_opx_hfi1_do_dput_sdma_jkr_9B(union fi_opx_hfi1_deferred_work *work);

int fi_opx_hfi1_do_dput_sdma_tid_wfr(union fi_opx_hfi1_deferred_work *work);
int fi_opx_hfi1_do_dput_sdma_tid_jkr(union fi_opx_hfi1_deferred_work *work);
int fi_opx_hfi1_do_dput_sdma_tid_cyr(union fi_opx_hfi1_deferred_work *work);
int fi_opx_hfi1_do_dput_sdma_tid_jkr_9B(union fi_opx_hfi1_deferred_work *work);
__OPX_FORCE_INLINE__
void fi_opx_hfi1_memcpy8(void *restrict dest, const void *restrict src, size_t n)
{
	const size_t	   qw_to_copy = n >> 3;
	const size_t	   remain     = n & 0x07ul;
	ssize_t		   idx;
	volatile uint64_t *d = dest;
	const uint64_t	  *s = src;

	for (idx = 0; idx < qw_to_copy; idx++) {
		*d++ = *s++;
	}
	if (remain == 0) {
		return;
	}

	union tmp_t {
		uint8_t		  byte[8];
		volatile uint64_t qw;
	} temp, *s8;
	assert(sizeof(temp.byte) == sizeof(temp.qw));
	assert(sizeof(temp.qw) == sizeof(temp));
	assert(sizeof(temp.byte) == sizeof(temp));
	temp.qw = 0ULL;
	s8	= (union tmp_t *) s;
	for (idx = 0; idx < remain; idx++) {
		temp.byte[idx] = s8->byte[idx];
	}
	*d = temp.qw;
}

__OPX_FORCE_INLINE__
uint64_t fi_opx_hfi1_tx_is_shm(struct fi_opx_ep *opx_ep, const union fi_opx_addr addr, const uint64_t caps)
{
	/* If (exclusively FI_LOCAL_COMM) OR (FI_LOCAL_COMM is on AND
	   the destination lid selected SHM) */
	return (((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == FI_LOCAL_COMM) ||
		(((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == (FI_LOCAL_COMM | FI_REMOTE_COMM)) &&
		 (opx_lid_is_shm(addr.lid))));
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_inject(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
			      const uint32_t data, int lock_required, const uint64_t tx_op_flags, const uint64_t caps,
			      const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type,
			      const bool ctx_sharing)
{
	struct fi_opx_ep       *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr   = {.fi = dest_addr};

	const uint64_t bth_subctxt_rx = ((uint64_t) addr.hfi1_subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	const uint64_t lrh_dlid_9B    = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(addr.lid);
	const uint32_t lrh_dlid_16B   = addr.lid;

	if (fi_opx_hfi1_tx_is_shm(opx_ep, addr, caps)) {
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			 "===================================== INJECT, SHM (begin)\n");
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-INJECT-SHM");
		uint64_t			 pos;
		ssize_t				 rc;
		union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
			&opx_ep->tx->shm, addr.hfi1_unit, addr.hfi1_subctxt_rx, &pos,
			opx_ep->daos_info.hfi_rank_enabled, opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);

		if (!hdr) {
			return rc;
		}

#ifdef OPX_HMEM
		if (buf && len) {
			uint64_t	   hmem_device;
			uint64_t	   is_unified __attribute__((__unused__));
			enum fi_hmem_iface iface = opx_hmem_get_ptr_iface(buf, &hmem_device, &is_unified);

			if (iface != FI_HMEM_SYSTEM) {
				opx_copy_from_hmem(iface, hmem_device, OPX_HMEM_NO_HANDLE, opx_ep->hmem_copy_buf, buf,
						   len, OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET);
				buf = opx_ep->hmem_copy_buf;
				FI_OPX_DEBUG_COUNTERS_INC(
					opx_ep->debug_counters.hmem.intranode
						.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
						.send.inject);
			}
		}
#endif

		if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
			hdr->qw_9B[0] = opx_ep->tx->inject_9B.hdr.qw_9B[0] | lrh_dlid_9B;

			hdr->qw_9B[1] = opx_ep->tx->inject_9B.hdr.qw_9B[1] | bth_subctxt_rx | (len << 51) |
					((caps & FI_MSG) ? /* compile-time constant expression */
						 ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							  (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ :
							  (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_INJECT) :
						 ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							  (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ :
							  (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_INJECT));
			hdr->qw_9B[2] = opx_ep->tx->inject_9B.hdr.qw_9B[2];

			hdr->qw_9B[3] = opx_ep->tx->inject_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32);

			hdr->qw_9B[4] = 0;
			hdr->qw_9B[5] = 0;
			fi_opx_hfi1_memcpy8((void *) &hdr->qw_9B[4], buf, len);
			hdr->qw_9B[6] = tag;
		} else {
			hdr->qw_16B[0] = opx_ep->tx->inject_16B.hdr.qw_16B[0] |
					 ((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B)
					  << OPX_LRH_JKR_16B_DLID_SHIFT_16B);
			hdr->qw_16B[1] = opx_ep->tx->inject_16B.hdr.qw_16B[1] |
					 (((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
					   OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
					 (uint64_t) (bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
			hdr->qw_16B[2] = opx_ep->tx->inject_16B.hdr.qw_16B[2] | bth_subctxt_rx | (len << 51) |
					 ((caps & FI_MSG) ? /* compile-time constant expression */
						  ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							   (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ :
							   (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_INJECT) :
						  ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							   (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ :
							   (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_INJECT));

			hdr->qw_16B[3] = opx_ep->tx->inject_16B.hdr.qw_16B[3];
			hdr->qw_16B[4] = opx_ep->tx->inject_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32),
			hdr->qw_16B[5] = 0;
			hdr->qw_16B[6] = 0;
			fi_opx_hfi1_memcpy8((void *) &hdr->qw_16B[5], buf, len);
			hdr->qw_16B[7] = tag;
		}

		opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-INJECT-SHM");
		FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			 "===================================== INJECT, SHM (end)\n");
		return FI_SUCCESS;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== INJECT, HFI (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-INJECT-HFI");

	/* first check for sufficient credits to inject the entire packet */
	OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	const uint16_t credits_needed = (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) ? 1 : 2;
	if (OFI_UNLIKELY(FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, credits_needed) <
			 credits_needed)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);

		if (FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, credits_needed) <
		    credits_needed) {
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(ep, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx, &psn_ptr, &replay,
					    reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-INJECT-HFI");
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		return -FI_EAGAIN;
	}

	if (lock_required) {
		fprintf(stderr, "%s:%s():%d\n", __FILE__, __func__, __LINE__);
		abort();
	}

#ifdef OPX_HMEM
	if (buf && len) {
		uint64_t	   hmem_device;
		uint64_t	   is_unified __attribute__((__unused__));
		enum fi_hmem_iface iface = opx_hmem_get_ptr_iface(buf, &hmem_device, &is_unified);

		if (iface != FI_HMEM_SYSTEM) {
			opx_copy_from_hmem(iface, hmem_device, OPX_HMEM_NO_HANDLE, opx_ep->hmem_copy_buf, buf, len,
					   OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET);
			buf = opx_ep->hmem_copy_buf;
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hmem.hfi
							  .kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
							  .send.inject);
		}
	}
#endif

	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	uint64_t local_temp[2];
	opx_copy_double_qw(local_temp, (const uint8_t *) buf, len);
	const uint64_t pbc_dlid = OPX_PBC_DLID(addr.lid, hfi1_type);
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		opx_cacheline_copy_qw_vol(scb, replay->scb.qws,
					  opx_ep->tx->inject_9B.qw0 |
						  OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
						  OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
					  opx_ep->tx->inject_9B.hdr.qw_9B[0] | lrh_dlid_9B,
					  opx_ep->tx->inject_9B.hdr.qw_9B[1] | bth_subctxt_rx | (len << 51) |
						  ((caps & FI_MSG) ? /* compile-time constant expression */
							   ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
								    (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ :
								    (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_INJECT) :
							   ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
								    (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ :
								    (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_INJECT)),

					  opx_ep->tx->inject_9B.hdr.qw_9B[2] | psn,
					  opx_ep->tx->inject_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32), local_temp[0],
					  local_temp[1], tag);

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
	} else {
		// 1st cacheline
		const uint64_t pbc_dlid = OPX_PBC_DLID(addr.lid, hfi1_type);
		opx_cacheline_copy_qw_vol(
			scb, replay->scb.qws,
			opx_ep->tx->inject_16B.qw0 | OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
				OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
			opx_ep->tx->inject_16B.hdr.qw_16B[0] |
				((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B)
				 << OPX_LRH_JKR_16B_DLID_SHIFT_16B),
			opx_ep->tx->inject_16B.hdr.qw_16B[1] |
				(((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
				  OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
				(uint64_t) (bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B),
			opx_ep->tx->inject_16B.hdr.qw_16B[2] | bth_subctxt_rx | (len << 51) |
				((caps & FI_MSG) ? /* compile-time constant expression */
					 ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
						  (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_INJECT_CQ :
						  (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_INJECT) :
					 ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
						  (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_INJECT_CQ :
						  (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_INJECT)),

			opx_ep->tx->inject_16B.hdr.qw_16B[3] | psn,
			opx_ep->tx->inject_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32), local_temp[0], local_temp[1]);

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

		// 2nd cacheline
		volatile uint64_t *const scb2 = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

		opx_cacheline_store_qw_vol(scb2, tag, OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD,
					   OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD,
					   OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD);

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
		replay->scb.qws[8] = tag;
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	/* save the updated txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, reliability,
							     hfi1_type);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-INJECT-HFI");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "===================================== INJECT, HFI (end)\n");

	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
bool fi_opx_hfi1_fill_from_iov8(const struct iovec  *iov,     /* In:  iovec array */
				size_t		     niov,    /* In:  total iovecs */
				volatile const void *buf,     /* In:  target buffer to fill */
				ssize_t		    *len,     /* In/Out:  buffer length to fill */
				ssize_t		    *iov_idx, /* In/Out:  start index, returns end */
				ssize_t		    *iov_base_offset)     /* In/Out:  start offset, returns offset */
{
	ssize_t idx		= *iov_idx;
	ssize_t iov_offset	= *iov_base_offset;
	ssize_t dst_len		= *len;
	ssize_t dst_buff_offset = 0;

	for (; idx < niov; idx++) {
		const uint8_t *src_buf = (uint8_t *) iov[idx].iov_base + iov_offset;
		ssize_t	       src_len = iov[idx].iov_len - iov_offset;
		assert(src_len > 0);
		ssize_t to_copy = MIN(src_len, dst_len);
		assert(to_copy > 0);
		uint8_t *dst_buf = (uint8_t *) buf + dst_buff_offset;

		fi_opx_hfi1_memcpy8(dst_buf, src_buf, to_copy);

		dst_buff_offset += to_copy;
		dst_len -= to_copy;
		iov_offset += to_copy;

		// Terminates when dest buffer is filled
		if (dst_len == 0) {
			*len	 = dst_len;
			*iov_idx = idx;
			if (src_len == 0) {
				*iov_base_offset = 0;
				(*iov_idx)++;
			} else {
				*iov_base_offset = iov_offset;
			}
			return true;
		}

		// reset the iovec offset when starting a new iovec
		assert((src_len -= to_copy) == 0);
		iov_offset = 0;
	}
	// update state variables
	*len		 = dst_len;
	*iov_idx	 = idx;
	*iov_base_offset = iov_offset;
	if (idx == niov) {
		// returns true when iovec array is done
		// TODO:  do we want to assert this should never happen
		// We should never have any dst_len left, instead of taking
		// this branch, make the user contractually pass us the
		// appropriate length?
		// assert(dst_len == 0); ????
		return true;
	}
	return false;
}

static inline void fi_opx_shm_poll_many(struct fid_ep *ep, const int lock_required, const enum opx_hfi1_type hfi1_type);

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_check_credits(struct fi_opx_ep *opx_ep, union fi_opx_hfi1_pio_state *pio_state,
				     uint16_t credits_needed)
{
	union fi_opx_hfi1_pio_state pio_state_val = *pio_state;
	uint16_t		    total_credits_available =
		FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state_val, &opx_ep->tx->force_credit_return, credits_needed);

	if (OFI_UNLIKELY(total_credits_available < credits_needed)) {
		fi_opx_compiler_msync_writes();
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state_val, opx_ep->tx->pio_credits_addr);
		total_credits_available =
			FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state_val, &opx_ep->tx->force_credit_return, credits_needed);

		if (total_credits_available < credits_needed) {
			opx_ep->tx->pio_state->qw0 = pio_state_val.qw0;
			return -FI_ENOBUFS;
		}
	}
	pio_state->qw0 = pio_state_val.qw0;

	return (ssize_t) total_credits_available;
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_sendv_egr_shm(struct fid_ep *ep, const struct iovec *iov, size_t niov, const uint16_t lrh_dws,
				  const uint64_t lrh_dlid_9B, size_t total_len, const size_t payload_qws_total,
				  const size_t xfer_bytes_tail, const union fi_opx_addr *addr, uint64_t tag,
				  void *context, const uint32_t data, int lock_required, const uint64_t tx_op_flags,
				  const uint64_t caps, const uint64_t do_cq_completion, const enum fi_hmem_iface iface,
				  const uint64_t hmem_device, const uint64_t hmem_handle,
				  const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	struct iovec  *iov_ptr	      = (struct iovec *) iov;
	size_t	      *niov_ptr	      = &niov;
	const uint64_t bth_subctxt_rx = ((uint64_t) addr->hfi1_subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		 "===================================== SENDV, SHM -- EAGER (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SENDV-EAGER-SHM");
	uint64_t			 pos;
	ssize_t				 rc;
	union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, addr->hfi1_unit, addr->hfi1_subctxt_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);

	if (!hdr) {
		return rc;
	}

#ifdef OPX_HMEM
	/* Note: This code is duplicated in the internode and intranode
		   paths at points in the code where we know we'll be able to
		   proceed with the send, so that we don't waste cycles doing
		   this, only to EAGAIN because we couldn't get a SHM packet
		   or credits/replay/psn */
	size_t	     hmem_niov = 1;
	struct iovec hmem_iov;

	/* If the IOVs are GPU-resident, copy all their data to the HMEM
		   bounce buffer, and then proceed as if we only have a single IOV
		   that points to the bounce buffer. */
	if (iface != FI_HMEM_SYSTEM) {
		unsigned iov_total_len = 0;
		for (int i = 0; i < niov; ++i) {
			opx_copy_from_hmem(iface, hmem_device, hmem_handle, &opx_ep->hmem_copy_buf[iov_total_len],
					   iov[i].iov_base, iov[i].iov_len, OPX_HMEM_DEV_REG_SEND_THRESHOLD);
			iov_total_len += iov[i].iov_len;
		}

		hmem_iov.iov_base = opx_ep->hmem_copy_buf;
		hmem_iov.iov_len  = iov_total_len;
		iov_ptr		  = &hmem_iov;
		niov_ptr	  = &hmem_niov;
		FI_OPX_DEBUG_COUNTERS_INC(
			opx_ep->debug_counters.hmem.intranode.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
				.send.eager_noncontig);
	}
#endif
	hdr->qw_9B[0] = opx_ep->tx->send_9B.hdr.qw_9B[0] | lrh_dlid_9B | ((uint64_t) lrh_dws << 32);
	hdr->qw_9B[1] =
		opx_ep->tx->send_9B.hdr.qw_9B[1] | bth_subctxt_rx | (xfer_bytes_tail << 51) |
		((caps & FI_MSG) ? ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ :
									(uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER) :
				   ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ :
									(uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER));
	hdr->qw_9B[2] = opx_ep->tx->send_9B.hdr.qw_9B[2];
	hdr->qw_9B[3] = opx_ep->tx->send_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32);
	hdr->qw_9B[4] = opx_ep->tx->send_9B.hdr.qw_9B[4] | (payload_qws_total << 48);
	/* Fill QW 5 from the iovec */
	uint8_t *buf	= (uint8_t *) &hdr->qw_9B[5];
	ssize_t	 remain = total_len, iov_idx = 0, iov_base_offset = 0;

	if (xfer_bytes_tail) {
		ssize_t tail_len = xfer_bytes_tail;
		remain		 = total_len - tail_len;
		while (false ==
		       fi_opx_hfi1_fill_from_iov8(iov_ptr,	       /* In:  iovec array */
						  *niov_ptr,	       /* In:  total iovecs */
						  buf,		       /* In:  target buffer to fill */
						  &tail_len,	       /* In/Out:  buffer length to fill */
						  &iov_idx,	       /* In/Out:  start index, returns end */
						  &iov_base_offset)) { /* In/Out:  start offset, returns offset */
		} // copy until done
		assert(tail_len == 0);
	}
	hdr->qw_9B[6] = tag;

	union fi_opx_hfi1_packet_payload *const payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);

	buf = payload->byte;
	while (false == fi_opx_hfi1_fill_from_iov8(iov_ptr,		/* In:  iovec array */
						   *niov_ptr,		/* In:  total iovecs */
						   buf,			/* In:  target buffer to fill */
						   &remain,		/* In/Out:  buffer length to fill */
						   &iov_idx,		/* In/Out:  start index, returns end */
						   &iov_base_offset)) { /* In/Out:  start offset, returns offset */
	} // copy until done
	assert(remain == 0);
	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);
	fi_opx_shm_poll_many(&opx_ep->ep_fid, 0, hfi1_type);

	if (OFI_LIKELY(do_cq_completion)) {
		rc = fi_opx_ep_tx_cq_inject_completion(ep, context, total_len, lock_required, tag, caps);
	} else {
		rc = FI_SUCCESS;
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SENDV-EAGER-SHM");
	FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		 "===================================== SENDV, SHM -- EAGER (end)\n");
	return rc;
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_sendv_egr(struct fid_ep *ep, const struct iovec *iov, size_t niov, size_t total_len,
			      fi_addr_t dest_addr, uint64_t tag, void *context, const uint32_t data, int lock_required,
			      const unsigned override_flags, const uint64_t tx_op_flags, const uint64_t caps,
			      const enum ofi_reliability_kind reliability, const uint64_t do_cq_completion,
			      const enum fi_hmem_iface iface, const uint64_t hmem_device, const uint64_t hmem_handle,
			      const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	assert(lock_required == 0);
	struct fi_opx_ep       *opx_ep		  = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr		  = {.fi = dest_addr};
	const size_t		xfer_bytes_tail	  = total_len & 0x07ul;
	const size_t		payload_qws_total = total_len >> 3;
	const size_t		payload_qws_tail  = payload_qws_total & 0x07ul;

	const uint64_t lrh_dlid_9B		 = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(addr.lid);
	uint16_t       full_block_credits_needed = (total_len >> 6);
	uint16_t       total_credits_needed	 = 1 +		   /* packet header */
					full_block_credits_needed; /* full blocks */

	if (payload_qws_tail || xfer_bytes_tail) {
		total_credits_needed += 1;
	}

	const uint64_t pbc_dws = 2 + /* pbc */
				 2 + /* lhr */
				 3 + /* bth */
				 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				 ((total_credits_needed - 1) << 4);

	const uint16_t lrh_dws = __cpu_to_be16(
		pbc_dws - 2 + 1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */

	struct iovec *iov_ptr  = (struct iovec *) iov;
	size_t	     *niov_ptr = &niov;

	if (fi_opx_hfi1_tx_is_shm(opx_ep, addr, caps)) {
		return opx_hfi1_tx_sendv_egr_shm(ep, iov, niov, lrh_dws, lrh_dlid_9B, total_len, payload_qws_total,
						 xfer_bytes_tail, &addr, tag, context, data, lock_required, tx_op_flags,
						 caps, do_cq_completion, iface, hmem_device, hmem_handle, hfi1_type);
	}

	const uint64_t bth_subctxt_rx = ((uint64_t) addr.hfi1_subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SENDV, HFI -- EAGER (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SENDV-EAGER-HFI");

	// Even though we're using the reliability service to pack this buffer
	// we still want to make sure it will have enough credits available to send
	// and allow the user to poll and quiesce the fabric some
	OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	ssize_t total_credits_available = fi_opx_hfi1_tx_check_credits(opx_ep, &pio_state, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < 0)) {
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		return -FI_ENOBUFS;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(ep, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx, &psn_ptr, &replay,
					    reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		return -FI_EAGAIN;
	}

#ifdef OPX_HMEM
	size_t	     hmem_niov = 1;
	struct iovec hmem_iov;

	/* If the IOVs are GPU-resident, copy all their data to the HMEM
	   bounce buffer, and then proceed as if we only have a single IOV
	   that points to the bounce buffer. */
	if (iface != FI_HMEM_SYSTEM) {
		unsigned iov_total_len = 0;
		for (int i = 0; i < niov; ++i) {
			opx_copy_from_hmem(iface, hmem_device, hmem_handle, &opx_ep->hmem_copy_buf[iov_total_len],
					   iov[i].iov_base, iov[i].iov_len, OPX_HMEM_DEV_REG_SEND_THRESHOLD);
			iov_total_len += iov[i].iov_len;
		}

		hmem_iov.iov_base = opx_ep->hmem_copy_buf;
		hmem_iov.iov_len  = iov_total_len;
		iov_ptr		  = &hmem_iov;
		niov_ptr	  = &hmem_niov;
		FI_OPX_DEBUG_COUNTERS_INC(
			opx_ep->debug_counters.hmem.hfi.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
				.send.eager_noncontig);
	}
#endif
	ssize_t remain = total_len, iov_idx = 0, iov_base_offset = 0;

	OPX_NO_16B_SUPPORT(hfi1_type);
	const uint64_t pbc_dlid = OPX_PBC_DLID(addr.lid, hfi1_type);
	replay->scb.scb_9B.qw0	= opx_ep->tx->send_9B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
				 OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
				 OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type);
	replay->scb.scb_9B.hdr.qw_9B[0] = opx_ep->tx->send_9B.hdr.qw_9B[0] | lrh_dlid_9B | ((uint64_t) lrh_dws << 32);
	replay->scb.scb_9B.hdr.qw_9B[1] =
		opx_ep->tx->send_9B.hdr.qw_9B[1] | bth_subctxt_rx | (xfer_bytes_tail << 51) |
		((caps & FI_MSG) ? ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ :
									(uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER) :
				   ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ :
									(uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER));
	replay->scb.scb_9B.hdr.qw_9B[2] = opx_ep->tx->send_9B.hdr.qw_9B[2] | psn;
	replay->scb.scb_9B.hdr.qw_9B[3] = opx_ep->tx->send_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32);
	replay->scb.scb_9B.hdr.qw_9B[4] = opx_ep->tx->send_9B.hdr.qw_9B[4] | (payload_qws_total << 48);
	if (xfer_bytes_tail) {
		ssize_t tail_len = xfer_bytes_tail;
		remain		 = total_len - tail_len;
		while (false ==
		       fi_opx_hfi1_fill_from_iov8(iov_ptr,			    /* In:  iovec array */
						  *niov_ptr,			    /* In:  total iovecs */
						  &replay->scb.scb_9B.hdr.qw_9B[5], /* In:  target buffer to fill */
						  &tail_len,			    /* In/Out:  buffer length to fill */
						  &iov_idx,	       /* In/Out:  start index, returns end */
						  &iov_base_offset)) { /* In/Out:  start offset, returns offset */
		} // copy until done
		assert(tail_len == 0);
	}
	replay->scb.scb_9B.hdr.qw_9B[6] = tag;

	remain		  = total_len - xfer_bytes_tail;
	uint64_t *payload = replay->payload;
	while (false == fi_opx_hfi1_fill_from_iov8(iov_ptr,		/* In:  iovec array */
						   *niov_ptr,		/* In:  total iovecs */
						   payload,		/* In:  target buffer to fill */
						   &remain,		/* In/Out:  buffer length to fill */
						   &iov_idx,		/* In/Out:  start index, returns end */
						   &iov_base_offset)) { /* In/Out:  start offset, returns offset */
	} // copy until done

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, reliability,
							     hfi1_type);

	fi_opx_reliability_service_do_replay(opx_ep, opx_ep->reli_service, replay);

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);

	ssize_t rc;
	if (OFI_LIKELY(do_cq_completion)) {
		rc = fi_opx_ep_tx_cq_inject_completion(ep, context, total_len, lock_required, tag, caps);
	} else {
		rc = FI_SUCCESS;
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SENDV-EAGER-HFI");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SENDV, HFI -- EAGER (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_sendv_egr_shm_16B(struct fid_ep *ep, const struct iovec *iov, size_t niov, const uint16_t lrh_qws,
				      const uint64_t lrh_dlid_16B, size_t total_len, const size_t payload_qws_total,
				      const size_t xfer_bytes_tail, const union fi_opx_addr *addr, uint64_t tag,
				      void *context, const uint32_t data, int lock_required, const uint64_t tx_op_flags,
				      const uint64_t caps, const uint64_t do_cq_completion,
				      const enum fi_hmem_iface iface, const uint64_t hmem_device,
				      const uint64_t hmem_handle, const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);

	struct iovec  *iov_ptr	      = (struct iovec *) iov;
	size_t	      *niov_ptr	      = &niov;
	const uint64_t bth_subctxt_rx = ((uint64_t) addr->hfi1_subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		 "===================================== SENDV 16B, SHM -- EAGER (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SENDV-EAGER-SHM");
	uint64_t			 pos;
	ssize_t				 rc;
	union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, addr->hfi1_unit, addr->hfi1_subctxt_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);

	if (!hdr) {
		return rc;
	}

#ifdef OPX_HMEM
	/* Note: This code is duplicated in the internode and intranode
		   paths at points in the code where we know we'll be able to
		   proceed with the send, so that we don't waste cycles doing
		   this, only to EAGAIN because we couldn't get a SHM packet
		   or credits/replay/psn */
	size_t	     hmem_niov = 1;
	struct iovec hmem_iov;

	/* If the IOVs are GPU-resident, copy all their data to the HMEM
		   bounce buffer, and then proceed as if we only have a single IOV
		   that points to the bounce buffer. */
	if (iface != FI_HMEM_SYSTEM) {
		unsigned iov_total_len = 0;
		for (int i = 0; i < niov; ++i) {
			opx_copy_from_hmem(iface, hmem_device, hmem_handle, &opx_ep->hmem_copy_buf[iov_total_len],
					   iov[i].iov_base, iov[i].iov_len, OPX_HMEM_DEV_REG_SEND_THRESHOLD);
			iov_total_len += iov[i].iov_len;
		}

		hmem_iov.iov_base = opx_ep->hmem_copy_buf;
		hmem_iov.iov_len  = iov_total_len;
		iov_ptr		  = &hmem_iov;
		niov_ptr	  = &hmem_niov;
		FI_OPX_DEBUG_COUNTERS_INC(
			opx_ep->debug_counters.hmem.intranode.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
				.send.eager_noncontig);
	}
#endif
	hdr->qw_16B[0] = opx_ep->tx->send_16B.hdr.qw_16B[0] |
			 ((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
			 ((uint64_t) lrh_qws << 20);
	hdr->qw_16B[1] =
		opx_ep->tx->send_16B.hdr.qw_16B[1] |
		((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >> OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
		(uint64_t) (bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
	hdr->qw_16B[2] = opx_ep->tx->send_16B.hdr.qw_16B[2] | bth_subctxt_rx | (xfer_bytes_tail << 51) |
			 ((caps & FI_MSG) ? /* compile-time constant expression */
				  ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ :
								       (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER) :
				  ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ :
								       (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER));
	hdr->qw_16B[3] = opx_ep->tx->send_16B.hdr.qw_16B[3];
	hdr->qw_16B[4] = opx_ep->tx->send_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32);
	hdr->qw_16B[5] = opx_ep->tx->send_16B.hdr.qw_16B[5] | (payload_qws_total << 48);

	/* Fill QW 6 from the iovec */
	uint8_t *buf	= (uint8_t *) &hdr->qw_16B[6];
	ssize_t	 remain = total_len, iov_idx = 0, iov_base_offset = 0;

	if (xfer_bytes_tail) {
		ssize_t tail_len = xfer_bytes_tail;
		remain		 = total_len - tail_len;
		while (false ==
		       fi_opx_hfi1_fill_from_iov8(iov_ptr,	       /* In:  iovec array */
						  *niov_ptr,	       /* In:  total iovecs */
						  buf,		       /* In:  target buffer to fill */
						  &tail_len,	       /* In/Out:  buffer length to fill */
						  &iov_idx,	       /* In/Out:  start index, returns end */
						  &iov_base_offset)) { /* In/Out:  start offset, returns offset */
		} // copy until done
		assert(tail_len == 0);
	}
	hdr->qw_16B[7] = tag;

	union fi_opx_hfi1_packet_payload *const payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);

	buf = payload->byte;
	while (false == fi_opx_hfi1_fill_from_iov8(iov_ptr,		/* In:  iovec array */
						   *niov_ptr,		/* In:  total iovecs */
						   buf,			/* In:  target buffer to fill */
						   &remain,		/* In/Out:  buffer length to fill */
						   &iov_idx,		/* In/Out:  start index, returns end */
						   &iov_base_offset)) { /* In/Out:  start offset, returns offset */
	} // copy until done
	assert(remain == 0);
	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);
	fi_opx_shm_poll_many(&opx_ep->ep_fid, 0, hfi1_type);

	if (OFI_LIKELY(do_cq_completion)) {
		rc = fi_opx_ep_tx_cq_inject_completion(ep, context, total_len, lock_required, tag, caps);
	} else {
		rc = FI_SUCCESS;
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SENDV-EAGER-SHM");
	FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		 "===================================== SENDV 16B, SHM -- EAGER (end)\n");
	return rc;
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_sendv_egr_16B(struct fid_ep *ep, const struct iovec *iov, size_t niov, size_t total_len,
				  fi_addr_t dest_addr, uint64_t tag, void *context, const uint32_t data,
				  int lock_required, const unsigned override_flags, const uint64_t tx_op_flags,
				  const uint64_t caps, const enum ofi_reliability_kind reliability,
				  const uint64_t do_cq_completion, const enum fi_hmem_iface iface,
				  const uint64_t hmem_device, const uint64_t hmem_handle,
				  const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	assert(lock_required == 0);
	struct fi_opx_ep       *opx_ep		  = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr		  = {.fi = dest_addr};
	const size_t		xfer_bytes_tail	  = total_len & 0x07ul;
	const size_t		payload_qws_total = total_len >> 3;

	const uint32_t lrh_dlid_16B = addr.lid;
	const uint64_t pbc_dlid	    = OPX_PBC_DLID(addr.lid, hfi1_type);
	/* 16B PBC is dws */
	const uint64_t pbc_dws =
		/* PIO SOP is 16 DWS/8 QWS*/
		2 + /* pbc */
		4 + /* lrh uncompressed */
		3 + /* bth */
		3 + /* kdeth */
		4 + /* software kdeth */

		/* PIO is everything else */
		2 +			   /* kdeth9 remaining 2 dws */
					   //--------------------- header split point KDETH 9 DWS
		(payload_qws_total << 1) + /* one packet payload */
		2;			   /* ICRC/tail 1 qws/2 dws   */

	/* Descriptive code above, but for reference most code just has: */
	/*              9 +  kdeth; from "RcvHdrSize[i].HdrSize" CSR */
	/*              2;   ICRC/tail */

	const uint16_t total_credits_needed = (pbc_dws + 15) >> 4; /* round up to full blocks */

	/* 16B LRH is qws */
	const uint16_t lrh_qws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */

	struct iovec *iov_ptr  = (struct iovec *) iov;
	size_t	     *niov_ptr = &niov;

	if (fi_opx_hfi1_tx_is_shm(opx_ep, addr, caps)) {
		return opx_hfi1_tx_sendv_egr_shm_16B(ep, iov, niov, lrh_qws, lrh_dlid_16B, total_len, payload_qws_total,
						     xfer_bytes_tail, &addr, tag, context, data, lock_required,
						     tx_op_flags, caps, do_cq_completion, iface, hmem_device,
						     hmem_handle, hfi1_type);
	}

	const uint64_t bth_subctxt_rx = ((uint64_t) addr.hfi1_subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SENDV 16B, HFI -- EAGER (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SENDV-EAGER-HFI");

	OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);

	// Even though we're using the reliability service to pack this buffer
	// we still want to make sure it will have enough credits available to send
	// and allow the user to poll and quiesce the fabric some
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	ssize_t total_credits_available = fi_opx_hfi1_tx_check_credits(opx_ep, &pio_state, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < 0)) {
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		return -FI_ENOBUFS;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(ep, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx, &psn_ptr, &replay,
					    reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		return -FI_EAGAIN;
	}

#ifdef OPX_HMEM
	size_t	     hmem_niov = 1;
	struct iovec hmem_iov;

	/* If the IOVs are GPU-resident, copy all their data to the HMEM
	   bounce buffer, and then proceed as if we only have a single IOV
	   that points to the bounce buffer. */
	if (iface != FI_HMEM_SYSTEM) {
		unsigned iov_total_len = 0;
		for (int i = 0; i < niov; ++i) {
			opx_copy_from_hmem(iface, hmem_device, hmem_handle, &opx_ep->hmem_copy_buf[iov_total_len],
					   iov[i].iov_base, iov[i].iov_len, OPX_HMEM_DEV_REG_SEND_THRESHOLD);
			iov_total_len += iov[i].iov_len;
		}

		hmem_iov.iov_base = opx_ep->hmem_copy_buf;
		hmem_iov.iov_len  = iov_total_len;
		iov_ptr		  = &hmem_iov;
		niov_ptr	  = &hmem_niov;
		FI_OPX_DEBUG_COUNTERS_INC(
			opx_ep->debug_counters.hmem.hfi.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
				.send.eager_noncontig);
	}
#endif
	ssize_t remain = total_len, iov_idx = 0, iov_base_offset = 0;

	OPX_NO_9B_SUPPORT(hfi1_type);

	replay->scb.scb_16B.qw0 = opx_ep->tx->send_16B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
				  OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
				  OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type);
	replay->scb.scb_16B.hdr.qw_16B[0] =
		opx_ep->tx->send_16B.hdr.qw_16B[0] |
		((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
		((uint64_t) lrh_qws << 20);
	replay->scb.scb_16B.hdr.qw_16B[1] =
		opx_ep->tx->send_16B.hdr.qw_16B[1] |
		((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >> OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
		(uint64_t) (bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
	replay->scb.scb_16B.hdr.qw_16B[2] =
		opx_ep->tx->send_16B.hdr.qw_16B[2] | bth_subctxt_rx | (xfer_bytes_tail << 51) |
		((caps & FI_MSG) ? ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ :
									(uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER) :
				   ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ :
									(uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER));
	replay->scb.scb_16B.hdr.qw_16B[3] = opx_ep->tx->send_16B.hdr.qw_16B[3] | psn;
	replay->scb.scb_16B.hdr.qw_16B[4] = opx_ep->tx->send_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32);
	replay->scb.scb_16B.hdr.qw_16B[5] = opx_ep->tx->send_16B.hdr.qw_16B[5] | (payload_qws_total << 48);
	if (xfer_bytes_tail) {
		ssize_t tail_len = xfer_bytes_tail;
		remain		 = total_len - tail_len;
		while (false ==
		       fi_opx_hfi1_fill_from_iov8(iov_ptr,			      /* In:  iovec array */
						  *niov_ptr,			      /* In:  total iovecs */
						  &replay->scb.scb_16B.hdr.qw_16B[6], /* In:  target buffer to fill */
						  &tail_len,	       /* In/Out:  buffer length to fill */
						  &iov_idx,	       /* In/Out:  start index, returns end */
						  &iov_base_offset)) { /* In/Out:  start offset, returns offset */
		} // copy until done
		assert(tail_len == 0);
	}
	replay->scb.scb_16B.hdr.qw_16B[7] = tag;

	remain		  = total_len - xfer_bytes_tail;
	uint64_t *payload = replay->payload;
	while (false == fi_opx_hfi1_fill_from_iov8(iov_ptr,		/* In:  iovec array */
						   *niov_ptr,		/* In:  total iovecs */
						   payload,		/* In:  target buffer to fill */
						   &remain,		/* In/Out:  buffer length to fill */
						   &iov_idx,		/* In/Out:  start index, returns end */
						   &iov_base_offset)) { /* In/Out:  start offset, returns offset */
	} // copy until done

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, reliability,
							     hfi1_type);

	fi_opx_reliability_service_do_replay(opx_ep, opx_ep->reli_service, replay);

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);

	ssize_t rc;
	if (OFI_LIKELY(do_cq_completion)) {
		rc = fi_opx_ep_tx_cq_inject_completion(ep, context, total_len, lock_required, tag, caps);
	} else {
		rc = FI_SUCCESS;
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SENDV-EAGER-HFI");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SENDV 16B, HFI -- EAGER (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_sendv_egr_select(struct fid_ep *ep, const struct iovec *iov, size_t niov, size_t total_len,
				     fi_addr_t dest_addr, uint64_t tag, void *context, const uint32_t data,
				     int lock_required, const unsigned override_flags, const uint64_t tx_op_flags,
				     const uint64_t caps, const enum ofi_reliability_kind reliability,
				     const uint64_t do_cq_completion, const enum fi_hmem_iface iface,
				     const uint64_t hmem_device, const uint64_t hmem_handle,
				     const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	if (hfi1_type & OPX_HFI1_WFR) {
		return opx_hfi1_tx_sendv_egr(ep, iov, niov, total_len, dest_addr, tag, context, data, lock_required,
					     override_flags, tx_op_flags, caps, reliability, do_cq_completion, iface,
					     hmem_device, hmem_handle, OPX_HFI1_WFR, ctx_sharing);
	} else if (hfi1_type & OPX_HFI1_JKR) {
		return opx_hfi1_tx_sendv_egr_16B(ep, iov, niov, total_len, dest_addr, tag, context, data, lock_required,
						 override_flags, tx_op_flags, caps, reliability, do_cq_completion,
						 iface, hmem_device, hmem_handle, OPX_HFI1_JKR, ctx_sharing);
	} else if (hfi1_type & OPX_HFI1_CYR) {
		return opx_hfi1_tx_sendv_egr_16B(ep, iov, niov, total_len, dest_addr, tag, context, data, lock_required,
						 override_flags, tx_op_flags, caps, reliability, do_cq_completion,
						 iface, hmem_device, hmem_handle, OPX_HFI1_CYR, ctx_sharing);
	} else if (hfi1_type & OPX_HFI1_JKR_9B) {
		return opx_hfi1_tx_sendv_egr(ep, iov, niov, total_len, dest_addr, tag, context, data, lock_required,
					     override_flags, tx_op_flags, caps, reliability, do_cq_completion, iface,
					     hmem_device, hmem_handle, OPX_HFI1_JKR_9B, ctx_sharing);
	}
	abort();
	return (ssize_t) -1L;
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_send_egr_shm(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
				 void *context, const uint32_t data, int lock_required, const uint64_t tx_op_flags,
				 const uint64_t caps, const uint64_t do_cq_completion, const enum fi_hmem_iface iface,
				 const uint64_t hmem_device, const uint64_t hmem_handle)
{
	struct fi_opx_ep       *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr   = {.fi = dest_addr};

	const uint64_t bth_subctxt_rx = ((uint64_t) addr.hfi1_subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	const uint64_t lrh_dlid_9B    = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(addr.lid);

	const size_t xfer_bytes_tail   = len & 0x07ul;
	const size_t payload_qws_total = len >> 3;

	const uint64_t pbc_dws = 2 + /* pbc */
				 2 + /* lhr */
				 3 + /* bth */
				 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				 (payload_qws_total << 1);

	const uint16_t lrh_dws = __cpu_to_be16(
		pbc_dws - 2 + 1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, SHM -- EAGER (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-EAGER-SHM");
	uint64_t			 pos;
	ssize_t				 rc;
	union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, addr.hfi1_unit, addr.hfi1_subctxt_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);

	if (!hdr) {
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-EAGER-SHM");
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== SEND, SHM -- EAGER (end) - No packet available.\n");
		return rc;
	}

#ifdef OPX_HMEM
	if (iface != FI_HMEM_SYSTEM) {
		opx_copy_from_hmem(iface, hmem_device, hmem_handle, opx_ep->hmem_copy_buf, buf, len,
				   OPX_HMEM_DEV_REG_SEND_THRESHOLD);
		buf = opx_ep->hmem_copy_buf;
		FI_OPX_DEBUG_COUNTERS_INC(
			opx_ep->debug_counters.hmem.intranode.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
				.send.eager);
	}
#endif

	hdr->qw_9B[0] = opx_ep->tx->send_9B.hdr.qw_9B[0] | lrh_dlid_9B | ((uint64_t) lrh_dws << 32);
	hdr->qw_9B[1] = opx_ep->tx->send_9B.hdr.qw_9B[1] | bth_subctxt_rx | (xfer_bytes_tail << 51) |
			((caps & FI_MSG) ? /* compile-time constant expression */
				 ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ :
								      (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER) :
				 ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ :
								      (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER));
	hdr->qw_9B[2] = opx_ep->tx->send_9B.hdr.qw_9B[2];
	hdr->qw_9B[3] = opx_ep->tx->send_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32);
	hdr->qw_9B[4] = opx_ep->tx->send_9B.hdr.qw_9B[4] | (payload_qws_total << 48);

	/* only if is_contiguous */
	if (OFI_LIKELY(len > 7)) {
		/* safe to blindly qw-copy the first portion of the source buffer */
		hdr->qw_9B[5] = *((uint64_t *) buf);
	} else {
		hdr->qw_9B[5] = 0;
		memcpy((void *) &hdr->qw_9B[5], buf, xfer_bytes_tail);
	}

	hdr->qw_9B[6] = tag;

	union fi_opx_hfi1_packet_payload *const payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);

	memcpy((void *) payload->byte, (const void *) ((uintptr_t) buf + xfer_bytes_tail),
	       payload_qws_total * sizeof(uint64_t));

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

	if (do_cq_completion) {
		rc = fi_opx_ep_tx_cq_inject_completion(ep, context, len, lock_required, tag, caps);
	} else {
		rc = FI_SUCCESS;
	}
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-EAGER-SHM");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, SHM -- EAGER (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_send_egr_shm_16B(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
				     void *context, const uint32_t data, int lock_required, const uint64_t tx_op_flags,
				     const uint64_t caps, const uint64_t do_cq_completion,
				     const enum fi_hmem_iface iface, const uint64_t hmem_device,
				     const uint64_t hmem_handle)
{
	struct fi_opx_ep       *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr   = {.fi = dest_addr};

	const uint64_t bth_subctxt_rx = ((uint64_t) addr.hfi1_subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	const uint64_t lrh_dlid_16B   = addr.lid;

	const size_t xfer_bytes_tail   = len & 0x07ul;
	const size_t payload_qws_total = len >> 3;

	const uint64_t pbc_dws = 2 +				 /* pbc */
				 4 +				 /* lrh uncompressed */
				 3 +				 /* bth */
				 9 +				 /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				 ((payload_qws_total) << 1) + 2; /* ICRC/tail */

	const uint16_t lrh_qws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND 16B, SHM -- EAGER (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-EAGER-SHM");
	uint64_t			 pos;
	ssize_t				 rc;
	union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, addr.hfi1_unit, addr.hfi1_subctxt_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);

	if (!hdr) {
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-EAGER-SHM");
		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SEND 16B, SHM -- EAGER (end) - No packet available.\n");
		return rc;
	}

#ifdef OPX_HMEM
	if (iface != FI_HMEM_SYSTEM) {
		opx_copy_from_hmem(iface, hmem_device, hmem_handle, opx_ep->hmem_copy_buf, buf, len,
				   OPX_HMEM_DEV_REG_SEND_THRESHOLD);
		buf = opx_ep->hmem_copy_buf;
		FI_OPX_DEBUG_COUNTERS_INC(
			opx_ep->debug_counters.hmem.intranode.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
				.send.eager);
	}
#endif

	hdr->qw_16B[0] = opx_ep->tx->send_16B.hdr.qw_16B[0] |
			 ((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
			 ((uint64_t) lrh_qws << 20);
	hdr->qw_16B[1] =
		opx_ep->tx->send_16B.hdr.qw_16B[1] |
		((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >> OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
		(uint64_t) (bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
	hdr->qw_16B[2] = opx_ep->tx->send_16B.hdr.qw_16B[2] | bth_subctxt_rx | (xfer_bytes_tail << 51) |
			 ((caps & FI_MSG) ? /* compile-time constant expression */
				  ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ :
								       (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER) :
				  ((tx_op_flags & FI_REMOTE_CQ_DATA) ? (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ :
								       (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER));
	hdr->qw_16B[3] = opx_ep->tx->send_16B.hdr.qw_16B[3];
	hdr->qw_16B[4] = opx_ep->tx->send_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32);
	hdr->qw_16B[5] = opx_ep->tx->send_16B.hdr.qw_16B[5] | (payload_qws_total << 48);

	/* only if is_contiguous */
	if (OFI_LIKELY(len > 7)) {
		/* safe to blindly qw-copy the first portion of the source buffer */
		hdr->qw_16B[6] = *((uint64_t *) buf);
	} else {
		hdr->qw_16B[6] = 0;
		memcpy((void *) &hdr->qw_16B[6], buf, xfer_bytes_tail);
	}

	hdr->qw_16B[7] = tag;

	union fi_opx_hfi1_packet_payload *const payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);

	memcpy((void *) payload->byte, (const void *) ((uintptr_t) buf + xfer_bytes_tail),
	       payload_qws_total * sizeof(uint64_t));

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

	if (do_cq_completion) {
		rc = fi_opx_ep_tx_cq_inject_completion(ep, context, len, lock_required, tag, caps);
	} else {
		rc = FI_SUCCESS;
	}
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-EAGER-SHM");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND 16B, SHM -- EAGER (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_egr_write_packet_header(
	struct fi_opx_ep *opx_ep, union fi_opx_hfi1_pio_state *pio_state, uint64_t *local_storage, const void *buf,
	const uint64_t bth_subctxt_rx, const uint64_t lrh_dlid,
	const uint16_t lrh_packet_length, /* 9B dws, 16B qws and little/big-endian as required */
	const uint64_t pbc_dlid, const uint64_t pbc_dws, const ssize_t len, const ssize_t xfer_bytes_tail,
	const size_t payload_qws_total, const uint32_t psn, const uint32_t data, const uint64_t tag,
	const uint64_t tx_op_flags, const uint64_t caps, const enum opx_hfi1_type hfi1_type)
{
	/*
	 * Write the 'start of packet' (hw+sw header) 'send control block'
	 * which will consume a single pio credit.
	 */

	uint64_t tail_bytes;
	if (xfer_bytes_tail) {
		fi_opx_duff_copy((uint8_t *) &tail_bytes, (uint8_t *) buf, xfer_bytes_tail);
	} else {
		tail_bytes = 0;
	}
	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, *pio_state);

	/* only if is_contiguous */
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		/* safe to blindly qw-copy the first portion of the source buffer */
		opx_cacheline_copy_qw_vol(
			scb, local_storage,
			opx_ep->tx->send_9B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
				OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
				OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
			opx_ep->tx->send_9B.hdr.qw_9B[0] | lrh_dlid | ((uint64_t) lrh_packet_length << 32),

			opx_ep->tx->send_9B.hdr.qw_9B[1] | bth_subctxt_rx | (xfer_bytes_tail << 51) |
				((caps & FI_MSG) ? ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ :
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER) :
						   ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ :
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER)),
			opx_ep->tx->send_9B.hdr.qw_9B[2] | psn,
			opx_ep->tx->send_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32),
			opx_ep->tx->send_9B.hdr.qw_9B[4] | (payload_qws_total << 48), tail_bytes, tag);

	} else {
		opx_cacheline_copy_qw_vol(scb, local_storage,
					  opx_ep->tx->send_16B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
						  OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
						  OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
					  opx_ep->tx->send_16B.hdr.qw_16B[0] |
						  ((uint64_t) (lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B)
						   << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
						  ((uint64_t) lrh_packet_length << 20),
					  opx_ep->tx->send_16B.hdr.qw_16B[1] |
						  ((uint64_t) ((lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
							       OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
						  (uint64_t) (bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B),

					  opx_ep->tx->send_16B.hdr.qw_16B[2] | bth_subctxt_rx |
						  (xfer_bytes_tail << 51) |
						  ((caps & FI_MSG) ? /* compile-time constant expression */
							   ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
								    (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER_CQ :
								    (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_EAGER) :
							   ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
								    (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER_CQ :
								    (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_EAGER)),

					  opx_ep->tx->send_16B.hdr.qw_16B[3] | psn,
					  opx_ep->tx->send_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32),
					  opx_ep->tx->send_16B.hdr.qw_16B[5] | (payload_qws_total << 48), tail_bytes);
	}

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);
	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	return 1; // Consumed 1 credit
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_store_16B_hdr_extension(struct fi_opx_ep *opx_ep, union fi_opx_hfi1_pio_state *pio_state,
					    uint64_t *local_storage, const uint64_t first_qw,
					    const size_t hdr_and_payload_qws, uint64_t *buf_qws)
{
	assert(pio_state->credits_total - pio_state->scb_head_index);
	assert(hdr_and_payload_qws <= OPX_JKR_16B_PAYLOAD_AFTER_HDR_QWS);

	union fi_opx_hfi1_pio_state pio_local	= *pio_state;
	volatile uint64_t	   *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_local);

	// spill from 1st cacheline (SOP)
	OPX_HFI1_BAR_PIO_STORE(&scb_payload[0], first_qw); // header
	local_storage[8] = first_qw;			   /* todo: pretty sure it's already there */

	int i;

	for (i = 1; i <= hdr_and_payload_qws; ++i) {
		OPX_HFI1_BAR_PIO_STORE(&scb_payload[i], buf_qws[i - 1]);
		local_storage[8 + i] = buf_qws[i - 1];
	}

	for (; i <= OPX_JKR_16B_PAYLOAD_AFTER_HDR_QWS; ++i) {
		OPX_HFI1_BAR_PIO_STORE(&scb_payload[i], OPX_JKR_16B_PAD_QWORD);
		local_storage[8 + i] = OPX_JKR_16B_PAD_QWORD;
	}

	FI_OPX_HFI1_CONSUME_CREDITS(pio_local, 1);
	pio_state->qw0 = pio_local.qw0;

	return 1;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_egr_store_full_payload_blocks(struct fi_opx_ep *opx_ep, union fi_opx_hfi1_pio_state *pio_state,
						     uint64_t *buf_qws, uint16_t full_block_credits_needed,
						     const ssize_t total_credits_available)
{
	/*
	 * write the payload "send control block(s)"
	 */
	const uint16_t contiguous_credits_until_wrap = pio_state->credits_total - pio_state->scb_head_index;

	const uint16_t contiguous_credits_available = MIN(total_credits_available, contiguous_credits_until_wrap);

	union fi_opx_hfi1_pio_state pio_local	= *pio_state;
	volatile uint64_t	   *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_local);

	const uint16_t contiguous_full_blocks_to_write = MIN(full_block_credits_needed, contiguous_credits_available);

	uint16_t i;
	for (i = 0; i < contiguous_full_blocks_to_write; ++i) {
		opx_cacheline_store_block_vol(scb_payload, buf_qws);

		scb_payload += FI_OPX_CACHE_LINE_QWS;
		buf_qws += FI_OPX_CACHE_LINE_QWS;
	}

	FI_OPX_HFI1_CONSUME_CREDITS(pio_local, contiguous_full_blocks_to_write);
	ssize_t credits_consumed = contiguous_full_blocks_to_write;

	full_block_credits_needed -= contiguous_full_blocks_to_write;

	if (OFI_UNLIKELY(full_block_credits_needed > 0)) {
		/*
		 * handle wrap condition
		 */

		scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_local);

		uint16_t i;
		for (i = 0; i < full_block_credits_needed; ++i) {
			opx_cacheline_store_block_vol(scb_payload, buf_qws);

			scb_payload += FI_OPX_CACHE_LINE_QWS;
			buf_qws += FI_OPX_CACHE_LINE_QWS;
		}

		FI_OPX_HFI1_CONSUME_CREDITS(pio_local, full_block_credits_needed);
		credits_consumed += full_block_credits_needed;
	}

	pio_state->qw0 = pio_local.qw0;

	return credits_consumed;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_egr_store_payload_tail(struct fi_opx_ep *opx_ep, union fi_opx_hfi1_pio_state *pio_state,
					      uint64_t *buf_qws, const size_t payload_qws_tail)
{
	volatile uint64_t *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, *pio_state);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "buf_qws %p, payload_qws_tail %zu\n", buf_qws,
		     payload_qws_tail);

	unsigned i = 0;
	for (; i < payload_qws_tail; ++i) {
		OPX_HFI1_BAR_PIO_STORE(&scb_payload[i], buf_qws[i]);
	}

	for (; i < FI_OPX_CACHE_LINE_QWS; ++i) {
		OPX_HFI1_BAR_PIO_STORE(&scb_payload[i], OPX_JKR_16B_PAD_QWORD);
	}
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);

	return 1; /* Consumed 1 credit */
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_tx_send_egr_write_replay_data(struct fi_opx_ep *opx_ep, const union fi_opx_addr addr,
					       struct fi_opx_reliability_tx_replay *replay,
					       union fi_opx_reliability_tx_psn *psn_ptr, const ssize_t xfer_bytes_tail,
					       const void *buf, const size_t payload_qws_total,
					       const enum ofi_reliability_kind reliability,
					       const enum opx_hfi1_type	       hfi1_type)
{
	uint64_t *buf_qws	 = (uint64_t *) ((uintptr_t) buf + xfer_bytes_tail);
	uint64_t *payload_qws	 = replay->payload;
	uint64_t  payload_blocks = payload_qws_total >> 3;

	for (int i = 0; i < payload_blocks; i++) {
		opx_cacheline_store_block(payload_qws, buf_qws);
		buf_qws += 8;
		payload_qws += 8;
	}

	uint64_t payload_last_block_qws = payload_qws_total & 0x7UL;
	for (int i = 0; i < payload_last_block_qws; i++) {
		payload_qws[i] = buf_qws[i];
	}

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, reliability,
							     hfi1_type);
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_send_egr(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
			     void *context, const uint32_t data, int lock_required, const unsigned override_flags,
			     const uint64_t tx_op_flags, const uint64_t caps,
			     const enum ofi_reliability_kind reliability, const uint64_t do_cq_completion,
			     const enum fi_hmem_iface iface, const uint64_t hmem_device, const uint64_t hmem_handle,
			     const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	struct fi_opx_ep       *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr   = {.fi = dest_addr};

	OPX_NO_16B_SUPPORT(hfi1_type);

	if (fi_opx_hfi1_tx_is_shm(opx_ep, addr, caps)) {
		return opx_hfi1_tx_send_egr_shm(ep, buf, len, dest_addr, tag, context, data, lock_required, tx_op_flags,
						caps, do_cq_completion, iface, hmem_device, hmem_handle);
	}

	const uint64_t bth_subctxt_rx	 = ((uint64_t) addr.hfi1_subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	const size_t   xfer_bytes_tail	 = len & 0x07ul;
	const size_t   payload_qws_total = len >> 3;

	const size_t payload_qws_tail = payload_qws_total & 0x07ul;

	uint16_t full_block_credits_needed = (uint16_t) (payload_qws_total >> 3);

	const uint64_t lrh_dlid_9B = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(addr.lid);
	const uint64_t pbc_dlid	   = OPX_PBC_DLID(addr.lid, hfi1_type);

	assert(!(hfi1_type & OPX_HFI1_CNX000));
	/* 9B PBC is dws */
	const uint64_t pbc_dws =
		/* PIO SOP is 16 DWS/8 QWS*/
		2 + /* pbc */
		2 + /* lhr */
		3 + /* bth */
		9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */

		/* PIO is everything else */
		(payload_qws_total << 1); /* one packet payload */

	/* 9B LRH is dws */
	const uint16_t lrh_dws = __cpu_to_be16(
		pbc_dws - 2 + 1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */

	assert(lock_required == 0);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- EAGER (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-EAGER-HFI");

	/* first check for sufficient credits to inject the entire packet */
	OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	uint16_t total_credits_needed = 1 +			    /* PIO SOP -- 1 credit */
					full_block_credits_needed + /* PIO full blocks -- payload */
					(payload_qws_tail > 0);	    /* PIO partial block -- 1 credit */

	ssize_t total_credits_available = fi_opx_hfi1_tx_check_credits(opx_ep, &pio_state, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < 0)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "SEND, HFI -- EAGER FI_ENOBUFS (end)\n");
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		return -FI_ENOBUFS;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int32_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx,
					    &psn_ptr, &replay, reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "SEND, HFI -- EAGER FI_EAGAIN (end)\n");
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		return -FI_EAGAIN;
	}

#ifdef OPX_HMEM
	if (iface != FI_HMEM_SYSTEM) {
		opx_copy_from_hmem(iface, hmem_device, hmem_handle, opx_ep->hmem_copy_buf, buf, len,
				   OPX_HMEM_DEV_REG_SEND_THRESHOLD);
		buf = opx_ep->hmem_copy_buf;
		FI_OPX_DEBUG_COUNTERS_INC(
			opx_ep->debug_counters.hmem.hfi.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
				.send.eager);
	}
#endif

#ifndef NDEBUG
	unsigned credits_consumed =
#endif
		fi_opx_hfi1_tx_egr_write_packet_header(opx_ep, &pio_state, replay->scb.qws, buf, bth_subctxt_rx,
						       lrh_dlid_9B, lrh_dws, pbc_dlid, pbc_dws, len, xfer_bytes_tail,
						       payload_qws_total, psn, data, tag, tx_op_flags, caps, hfi1_type);

	uint64_t *buf_qws = (uint64_t *) ((uintptr_t) buf + xfer_bytes_tail);

	if (OFI_LIKELY(full_block_credits_needed)) {
#ifndef NDEBUG
		credits_consumed +=
#endif
			fi_opx_hfi1_tx_egr_store_full_payload_blocks(
				opx_ep, &pio_state, buf_qws, full_block_credits_needed, total_credits_available - 1);
	}

	if (OFI_LIKELY(payload_qws_tail)) {
#ifndef NDEBUG
		credits_consumed +=
#endif
			fi_opx_hfi1_tx_egr_store_payload_tail(
				opx_ep, &pio_state, buf_qws + (full_block_credits_needed << 3), payload_qws_tail);
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

#ifndef NDEBUG
	assert(credits_consumed == total_credits_needed);
#endif

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;
	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);

	fi_opx_hfi1_tx_send_egr_write_replay_data(opx_ep, addr, replay, psn_ptr, xfer_bytes_tail, buf,
						  payload_qws_total, reliability, hfi1_type);

	ssize_t rc;
	if (OFI_LIKELY(do_cq_completion)) {
		rc = fi_opx_ep_tx_cq_inject_completion(ep, context, len, lock_required, tag, caps);
	} else {
		rc = FI_SUCCESS;
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-EAGER-HFI");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- EAGER (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_send_egr_16B(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
				 void *context, const uint32_t data, int lock_required, const unsigned override_flags,
				 const uint64_t tx_op_flags, const uint64_t caps,
				 const enum ofi_reliability_kind reliability, const uint64_t do_cq_completion,
				 const enum fi_hmem_iface iface, const uint64_t hmem_device, const uint64_t hmem_handle,
				 const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	struct fi_opx_ep       *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr   = {.fi = dest_addr};

	OPX_NO_9B_SUPPORT(hfi1_type);

	if (fi_opx_hfi1_tx_is_shm(opx_ep, addr, caps)) {
		return opx_hfi1_tx_send_egr_shm_16B(ep, buf, len, dest_addr, tag, context, data, lock_required,
						    tx_op_flags, caps, do_cq_completion, iface, hmem_device,
						    hmem_handle);
	}

	const size_t   xfer_bytes_tail	 = len & 0x07ul;
	const size_t   payload_qws_total = len >> 3;
	const uint64_t bth_subctxt_rx	 = ((uint64_t) addr.hfi1_subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	/* 16B (RcvPktCtrl=9) has 1 QW of KDETH and 1 QW of tail in PIO (non-SOP) */
	const size_t kdeth9_qws_total = 1;
	const size_t tail_qws_total   = 1;

	/* Full 64 byte/8 qword blocks -- 1 credit per block */
	uint16_t full_block_credits_needed = (uint16_t) ((kdeth9_qws_total + payload_qws_total + tail_qws_total) >> 3);
	/* Remaining tail qwords (< 8) after full blocks */
	size_t tail_partial_block_qws = (kdeth9_qws_total + payload_qws_total + tail_qws_total) & 0x07ul;

	const uint64_t lrh_dlid_16B = addr.lid;
	const uint64_t pbc_dlid	    = OPX_PBC_DLID(addr.lid, hfi1_type);

	assert(hfi1_type & OPX_HFI1_CNX000);
	/* 16B PBC is dws */
	const uint64_t pbc_dws =
		/* PIO SOP is 16 DWS/8 QWS*/
		2 + /* pbc */
		4 + /* lrh uncompressed */
		3 + /* bth */
		3 + /* kdeth */
		4 + /* software kdeth */

		/* PIO is everything else */
		(kdeth9_qws_total << 1) +  /* kdeth9 remaining 2 dws */
					   //--------------------- header split point KDETH 9 DWS
		(payload_qws_total << 1) + /* one packet payload */
		(tail_qws_total << 1);	   /* tail 1 qws/2 dws   */

	/* 16B LRH is qws */
	const uint16_t lrh_qws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */

	assert(lock_required == 0);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND 16B, HFI -- EAGER (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-EAGER-HFI");

	OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);

	/* first check for sufficient credits to inject the entire packet */
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	uint16_t total_credits_needed = 1 +			      /* PIO SOP -- 1 credit */
					full_block_credits_needed +   /* PIO full blocks -- kdeth9/payload/tail */
					(tail_partial_block_qws > 0); /* PIO partial block -- 1 credit */

	ssize_t total_credits_available = fi_opx_hfi1_tx_check_credits(opx_ep, &pio_state, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < 0)) {
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		return -FI_ENOBUFS;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int32_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx,
					    &psn_ptr, &replay, reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		return -FI_EAGAIN;
	}

#ifdef OPX_HMEM
	if (iface != FI_HMEM_SYSTEM) {
		opx_copy_from_hmem(iface, hmem_device, hmem_handle, opx_ep->hmem_copy_buf, buf, len,
				   OPX_HMEM_DEV_REG_SEND_THRESHOLD);
		buf = opx_ep->hmem_copy_buf;
		FI_OPX_DEBUG_COUNTERS_INC(
			opx_ep->debug_counters.hmem.hfi.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
				.send.eager);
	}
#endif

#ifndef NDEBUG
	unsigned credits_consumed =
#endif
		fi_opx_hfi1_tx_egr_write_packet_header(opx_ep, &pio_state, replay->scb.qws, buf, bth_subctxt_rx,
						       lrh_dlid_16B, lrh_qws, pbc_dlid, pbc_dws, len, xfer_bytes_tail,
						       payload_qws_total, psn, data, tag, tx_op_flags, caps, hfi1_type);

	uint64_t *buf_qws = (uint64_t *) ((uintptr_t) buf + xfer_bytes_tail);

	assert(hfi1_type & OPX_HFI1_CNX000);

	/* write one block of PIO non-SOP, either one full block (8 qws) or the partial qws/block */
	const size_t first_block_qws =
		full_block_credits_needed ? OPX_JKR_16B_PAYLOAD_AFTER_HDR_QWS : payload_qws_total;

#ifndef NDEBUG
	credits_consumed +=
#endif
		opx_hfi1_tx_store_16B_hdr_extension(opx_ep, &pio_state, replay->scb.qws, tag, first_block_qws, buf_qws);

	buf_qws = buf_qws + first_block_qws;
	/* adjust full or partial for what we just consumed */
	if (full_block_credits_needed) {
		full_block_credits_needed--;
	}
	/* we wrote 7 qw, counts as partial tail*/
	else {
		tail_partial_block_qws = 0;
	}

	if (OFI_LIKELY(full_block_credits_needed)) {
#ifndef NDEBUG
		credits_consumed +=
#endif
			fi_opx_hfi1_tx_egr_store_full_payload_blocks(
				opx_ep, &pio_state, buf_qws, full_block_credits_needed, total_credits_available - 2);
	}

	if (OFI_LIKELY(tail_partial_block_qws)) {
#ifndef NDEBUG
		credits_consumed +=
#endif
			fi_opx_hfi1_tx_egr_store_payload_tail(
				opx_ep, &pio_state, buf_qws + (full_block_credits_needed << 3),
				tail_partial_block_qws - 1); // (tail_partial_block_qws-1) data + 1 QW ICRC
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

#ifndef NDEBUG
	assert(credits_consumed == total_credits_needed);
#endif

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);

	fi_opx_hfi1_tx_send_egr_write_replay_data(opx_ep, addr, replay, psn_ptr, xfer_bytes_tail, buf,
						  payload_qws_total, reliability, hfi1_type);

	ssize_t rc;
	if (OFI_LIKELY(do_cq_completion)) {
		rc = fi_opx_ep_tx_cq_inject_completion(ep, context, len, lock_required, tag, caps);
	} else {
		rc = FI_SUCCESS;
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-EAGER-HFI");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND 16B, HFI -- EAGER (end)\n");

	return rc;
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_send_egr_select(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
				    void *context, const uint32_t data, int lock_required,
				    const unsigned override_flags, const uint64_t tx_op_flags, const uint64_t caps,
				    const enum ofi_reliability_kind reliability, const uint64_t do_cq_completion,
				    const enum fi_hmem_iface iface, const uint64_t hmem_device,
				    const uint64_t hmem_handle, const enum opx_hfi1_type hfi1_type,
				    const bool ctx_sharing)
{
	if (hfi1_type & OPX_HFI1_WFR) {
		return opx_hfi1_tx_send_egr(ep, buf, len, dest_addr, tag, context, data, lock_required, override_flags,
					    tx_op_flags, caps, reliability, do_cq_completion, iface, hmem_device,
					    hmem_handle, OPX_HFI1_WFR, ctx_sharing);
	} else if (hfi1_type & OPX_HFI1_JKR_9B) {
		return opx_hfi1_tx_send_egr(ep, buf, len, dest_addr, tag, context, data, lock_required, override_flags,
					    tx_op_flags, caps, reliability, do_cq_completion, iface, hmem_device,
					    hmem_handle, OPX_HFI1_JKR_9B, ctx_sharing);
	} else if (hfi1_type & OPX_HFI1_JKR) {
		return opx_hfi1_tx_send_egr_16B(ep, buf, len, dest_addr, tag, context, data, lock_required,
						override_flags, tx_op_flags, caps, reliability, do_cq_completion, iface,
						hmem_device, hmem_handle, OPX_HFI1_JKR, ctx_sharing);
	}

	assert(hfi1_type & OPX_HFI1_CYR);
	return opx_hfi1_tx_send_egr_16B(ep, buf, len, dest_addr, tag, context, data, lock_required, override_flags,
					tx_op_flags, caps, reliability, do_cq_completion, iface, hmem_device,
					hmem_handle, OPX_HFI1_CYR, ctx_sharing);
}

/*
 * Write the initial packet header of a multi-packet eager send. This will include the size of
 * the entire multi-packet eager payload.
 */
__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_mp_egr_write_initial_packet_header(
	struct fi_opx_ep *opx_ep, union fi_opx_hfi1_pio_state *pio_state, uint64_t *local_storage, const void *buf,
	const uint64_t bth_subctxt_rx, const uint64_t lrh_dlid, const uint16_t lrh_dws, const uint64_t pbc_dlid,
	const uint64_t pbc_dws, const uint64_t payload_bytes_total, const uint32_t psn, const uint32_t data,
	const uint64_t tag, const uint64_t tx_op_flags, const uint64_t caps, const enum opx_hfi1_type hfi1_type)
{
	/*
	 * Write the 'start of packet' (hw+sw header) 'send control block'
	 * which will consume a single pio credit.
	 */

	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, *pio_state);

	/* For a multi-packet eager, the *first* packet's payload length should always be > 15 bytes,
	   so we should be safe to blindly copy 2 qws out of buf */
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		opx_cacheline_copy_qw_vol(
			scb, local_storage,
			opx_ep->tx->send_mp_9B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
				OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
				OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
			opx_ep->tx->send_mp_9B.hdr.qw_9B[0] | lrh_dlid | ((uint64_t) lrh_dws << 32),
			opx_ep->tx->send_mp_9B.hdr.qw_9B[1] | bth_subctxt_rx |
				((caps & FI_MSG) ? ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST_CQ :
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST) :
						   ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST_CQ :
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST)),
			opx_ep->tx->send_mp_9B.hdr.qw_9B[2] | psn,
			opx_ep->tx->send_mp_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32), payload_bytes_total, 0UL, tag);
	} else {
		opx_cacheline_copy_qw_vol(
			scb, local_storage,
			opx_ep->tx->send_mp_16B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
				OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
				OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
			opx_ep->tx->send_mp_16B.hdr.qw_16B[0] |
				((uint64_t) (lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B)
				 << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
				((uint64_t) lrh_dws << 20),
			opx_ep->tx->send_mp_16B.hdr.qw_16B[1] |
				((uint64_t) ((lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
					     OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
				(uint64_t) (bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B),
			opx_ep->tx->send_mp_16B.hdr.qw_16B[2] | bth_subctxt_rx |
				((caps & FI_MSG) ? /* compile-time constant expression */
					 ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
						  (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST_CQ :
						  (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_MP_EAGER_FIRST) :
					 ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
						  (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST_CQ :
						  (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_MP_EAGER_FIRST)),
			opx_ep->tx->send_mp_16B.hdr.qw_16B[3] | psn,
			opx_ep->tx->send_mp_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32), payload_bytes_total, 0UL);
	}

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);
	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	return 1; /* Consumed 1 credit */
}

/*
 * Write the nth packet header of a multi-packet eager send where the remaining payload data is
 * more than 16 bytes. This means we'll use all 16 bytes of tail space in the packet header, and
 * there will be at least some payload data.
 */
__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_mp_egr_write_nth_packet_header(
	struct fi_opx_ep *opx_ep, union fi_opx_hfi1_pio_state *pio_state, uint64_t *local_storage, const void *buf,
	const uint64_t bth_subctxt_rx, const uint64_t lrh_dlid, const uint16_t lrh_dws, const uint64_t pbc_dlid,
	const uint64_t pbc_dws, const ssize_t xfer_bytes_tail, const uint64_t tail_bytes, const uint32_t payload_offset,
	const uint32_t psn, const uint32_t mp_egr_uid, const enum opx_hfi1_type hfi1_type)
{
	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, *pio_state);

	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		opx_cacheline_copy_qw_vol(
			scb, local_storage,
			opx_ep->tx->send_mp_9B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
				OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
				OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
			opx_ep->tx->send_mp_9B.hdr.qw_9B[0] | lrh_dlid | ((uint64_t) lrh_dws << 32),
			opx_ep->tx->send_mp_9B.hdr.qw_9B[1] | bth_subctxt_rx | (xfer_bytes_tail << 51) |
				(uint64_t) FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH,
			opx_ep->tx->send_mp_9B.hdr.qw_9B[2] | psn, opx_ep->tx->send_mp_9B.hdr.qw_9B[3], tail_bytes, 0UL,
			(((uint64_t) mp_egr_uid) << 32) | payload_offset);
	} else {
		opx_cacheline_copy_qw_vol(scb, local_storage,
					  opx_ep->tx->send_mp_16B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
						  OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
						  OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
					  opx_ep->tx->send_mp_16B.hdr.qw_16B[0] |
						  ((uint64_t) (lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B)
						   << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
						  ((uint64_t) lrh_dws << 20),
					  opx_ep->tx->send_mp_16B.hdr.qw_16B[1] |
						  ((uint64_t) ((lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
							       OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
						  (uint64_t) (bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B),
					  opx_ep->tx->send_mp_16B.hdr.qw_16B[2] | bth_subctxt_rx |
						  (xfer_bytes_tail << 51) |
						  (uint64_t) FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH,
					  opx_ep->tx->send_mp_16B.hdr.qw_16B[3] | psn,
					  opx_ep->tx->send_mp_16B.hdr.qw_16B[4], tail_bytes, 0UL);
	}

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);
	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	return 1; /* Consumed 1 credit */
}

/*
 * Write the nth packet header of a multi-packet eager send where the remaining payload data is <= 16 bytes.
 * This means we won't need to write a payload packet as the entire payload fits in the packet header.
 */
__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_mp_egr_write_nth_packet_header_no_payload(
	struct fi_opx_ep *opx_ep, union fi_opx_hfi1_pio_state *pio_state, uint64_t *local_storage, const void *buf,
	const uint64_t bth_subctxt_rx, const uint64_t lrh_dlid, const uint16_t lrh_dws, const uint64_t pbc_dlid,
	const uint64_t pbc_dws, const ssize_t xfer_bytes_tail, const uint64_t tail_bytes, const uint32_t payload_offset,
	const uint32_t psn, const uint32_t mp_egr_uid, const enum opx_hfi1_type hfi1_type)
{
	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, *pio_state);

	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		opx_cacheline_copy_qw_vol(
			scb, local_storage,
			opx_ep->tx->send_mp_9B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
				OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
				OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
			opx_ep->tx->send_mp_9B.hdr.qw_9B[0] | lrh_dlid | ((uint64_t) lrh_dws << 32),
			opx_ep->tx->send_mp_9B.hdr.qw_9B[1] | bth_subctxt_rx | (xfer_bytes_tail << 51) |
				(uint64_t) FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH,
			opx_ep->tx->send_mp_9B.hdr.qw_9B[2] | psn, opx_ep->tx->send_mp_9B.hdr.qw_9B[3], tail_bytes, 0UL,
			(((uint64_t) mp_egr_uid) << 32) | payload_offset);
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);
		FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

		return 1; /* Consumed 1 credit */
	} else {
		// 1st cacheline
		opx_cacheline_copy_qw_vol(scb, local_storage,
					  opx_ep->tx->send_mp_16B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
						  OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
						  OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
					  opx_ep->tx->send_mp_16B.hdr.qw_16B[0] |
						  ((uint64_t) (lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B)
						   << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
						  ((uint64_t) lrh_dws << 20),
					  opx_ep->tx->send_mp_16B.hdr.qw_16B[1] |
						  ((uint64_t) ((lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
							       OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
						  (uint64_t) (bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B),
					  opx_ep->tx->send_mp_16B.hdr.qw_16B[2] | bth_subctxt_rx |
						  (xfer_bytes_tail << 51) |
						  (uint64_t) FI_OPX_HFI_BTH_OPCODE_MP_EAGER_NTH,
					  opx_ep->tx->send_mp_16B.hdr.qw_16B[3] | psn,
					  opx_ep->tx->send_mp_16B.hdr.qw_16B[4], tail_bytes, 0UL);

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);

		// 2nd cacheline
		volatile uint64_t *const scb2 = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, *pio_state);

		local_storage[8] = (((uint64_t) mp_egr_uid) << 32) | payload_offset;
		opx_cacheline_store_qw_vol(scb2, local_storage[8], OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD,
					   OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD,
					   OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD);

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(*pio_state);

		opx_ep->tx->pio_state->qw0 = pio_state->qw0;

		FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);
		return 2; /* Consumed 2 credit */
	}
}

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_send_mp_egr_first_common(struct fi_opx_ep *opx_ep, void **buf, const uint64_t payload_bytes_total,
					     uint8_t *hmem_bounce_buf, const uint64_t pbc_dlid,
					     const uint64_t bth_subctxt_rx, const uint64_t lrh_dlid,
					     const union fi_opx_addr addr, uint64_t tag, const uint32_t data,
					     int lock_required, const uint64_t tx_op_flags, const uint64_t caps,
					     const enum ofi_reliability_kind reliability, uint32_t *psn_out,
					     size_t *payload_bytes_sent, const enum fi_hmem_iface iface,
					     const uint64_t hmem_device, const uint64_t hmem_handle,
					     const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	assert(lock_required == 0);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- MULTI-PACKET EAGER FIRST (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-MP-EAGER-FIRST-HFI");

	const uint16_t chunk_credits  = opx_ep->tx->mp_eager_chunk_size >> 6;
	const uint16_t credits_needed = chunk_credits + ((hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) ? 1 : 2);

	OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	ssize_t total_credits_available = fi_opx_hfi1_tx_check_credits(opx_ep, &pio_state, credits_needed);
	if (OFI_UNLIKELY(total_credits_available < credits_needed)) {
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		OPX_TRACER_TRACE(OPX_TRACER_END_ENOBUFS, "SEND-MP-EAGER-FIRST-HFI");
		return -FI_ENOBUFS;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int32_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx,
					    &psn_ptr, &replay, reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-MP-EAGER-FIRST-HFI");
		return -FI_EAGAIN;
	}

	*psn_out = psn; /* This will be the UID used in the remaining packets */

#ifdef OPX_HMEM
	/* If the source buf resides in GPU memory, copy the entire payload to
	   the HMEM bounce buf. This HMEM bounce buf will be used as the source
	   buffer for this first MP Eager packet as well as all subsequent
	   MP Eager Nth packets. */
	if (iface != FI_HMEM_SYSTEM) {
		opx_copy_from_hmem(iface, hmem_device, hmem_handle, hmem_bounce_buf, *buf, payload_bytes_total,
				   OPX_HMEM_DEV_REG_SEND_THRESHOLD);
		*buf = hmem_bounce_buf;
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hmem.hfi.kind[FI_OPX_KIND_TAG].send.mp_eager);
	}
#endif
	void *buf_ptr = *buf;

	const uint64_t payload_qws_total = opx_ep->tx->mp_eager_chunk_size >> 3;
	uint64_t       pbc_dws;
	uint16_t       lrh_dws;
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		pbc_dws = 16 + /* pbc + packet header */
			  (payload_qws_total << 1);
		lrh_dws = __cpu_to_be16(pbc_dws - 1);
	} else {
		const size_t kdeth9_qws_total = 1;
		const size_t tail_qws_total   = 1;
		pbc_dws			      = /* PIO SOP is 16 DWS/8 QWS*/
			2 +			/* pbc */
			4 +			/* lrh uncompressed */
			3 +			/* bth */
			3 +			/* kdeth */
			4 +			/* software kdeth */
			/* PIO is everything else */
			(kdeth9_qws_total << 1) +  /* kdeth9 remaining 2 dws */
						   //--------------------- header split point KDETH 9 DWS
			(payload_qws_total << 1) + /* one packet payload */
			(tail_qws_total << 1);	   /* tail 1 qws/2 dws   */

		lrh_dws = (pbc_dws - 2) >> 1;
	}
#ifndef NDEBUG
	unsigned credits_consumed =
#endif
		fi_opx_hfi1_tx_mp_egr_write_initial_packet_header(
			opx_ep, &pio_state, replay->scb.qws, buf_ptr, bth_subctxt_rx, lrh_dlid, lrh_dws, pbc_dlid,
			pbc_dws, payload_bytes_total, psn, data, tag, tx_op_flags, caps, hfi1_type);

	uint64_t *buf_qws = (uint64_t *) buf_ptr;

	if (hfi1_type & OPX_HFI1_CNX000) {
		/* write header and payload */

#ifndef NDEBUG
		credits_consumed +=
#endif
			opx_hfi1_tx_store_16B_hdr_extension(opx_ep, &pio_state, replay->scb.qws, tag,
							    OPX_JKR_16B_PAYLOAD_AFTER_HDR_QWS, buf_qws);

		buf_qws += OPX_JKR_16B_PAYLOAD_AFTER_HDR_QWS;

		uint32_t full_block_credits_needed = chunk_credits - 1; // the last block needs to include icrc,
#ifndef NDEBUG
		credits_consumed +=
#endif
			fi_opx_hfi1_tx_egr_store_full_payload_blocks(
				opx_ep, &pio_state, buf_qws, full_block_credits_needed, total_credits_available - 2);

		buf_qws = buf_qws + (full_block_credits_needed << 3);

#ifndef NDEBUG
		credits_consumed +=
#endif
			fi_opx_hfi1_tx_egr_store_payload_tail(opx_ep, &pio_state, buf_qws,
							      7); // 7 QW data + 1 QW ICRC
	} else {
#ifndef NDEBUG
		credits_consumed +=
#endif
			fi_opx_hfi1_tx_egr_store_full_payload_blocks(opx_ep, &pio_state, buf_qws, chunk_credits,
								     total_credits_available - 1);
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);
#ifndef NDEBUG
	assert(credits_consumed == credits_needed);
#endif

	/* Since this is the first packet of a multi-packet eager send,
	   there will be no partial-payload blocks at the end to send */

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;
	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);

	fi_opx_hfi1_tx_send_egr_write_replay_data(opx_ep, addr, replay, psn_ptr, 0, buf_ptr, payload_qws_total,
						  reliability, hfi1_type);
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-MP-EAGER-FIRST-HFI");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- MULTI-PACKET EAGER FIRST (end)\n");

	(*payload_bytes_sent) = opx_ep->tx->mp_eager_chunk_size;

	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_send_mp_egr_nth(struct fi_opx_ep *opx_ep, const void *buf, const uint32_t payload_offset,
				       const ssize_t len, const uint32_t mp_egr_uid, const uint64_t pbc_dlid,
				       const uint64_t bth_subctxt_rx, const uint64_t lrh_dlid_9B,
				       const union fi_opx_addr addr, int lock_required,
				       const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type,
				       const bool ctx_sharing)
{
	assert(lock_required == 0);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- MULTI-PACKET EAGER NTH (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-MP-EAGER-NTH");

	size_t xfer_bytes_tail = len & 0x07UL;

	const size_t payload_qws_total = (len - xfer_bytes_tail) >> 3;
	const size_t payload_qws_tail  = payload_qws_total & 0x07ul;

	uint16_t full_block_credits_needed = (uint16_t) (payload_qws_total >> 3);
	uint16_t total_credits_needed	   = full_block_credits_needed + 1 + /* pbc/packet header */
					(payload_qws_tail ? 1 : 0);

	const uint64_t pbc_dws = 16 + /* pbc + packet header */
				 (payload_qws_total << 1);

	const uint16_t lrh_dws = __cpu_to_be16(
		pbc_dws - 2 + 1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */

	OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	uint16_t total_credits_available =
		FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		OPX_TRACER_TRACE(OPX_TRACER_END_ENOBUFS, "SEND-MP-EAGER-NTH");
		return -FI_ENOBUFS;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int32_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx,
					    &psn_ptr, &replay, reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-MP-EAGER-NTH");
		return -FI_EAGAIN;
	}

	uint64_t tail_bytes;

	if (xfer_bytes_tail) {
		fi_opx_duff_copy((uint8_t *) &tail_bytes, ((const uint8_t *) buf) + (len & -8UL), xfer_bytes_tail);
	} else {
		tail_bytes = 0UL;
	}

#ifndef NDEBUG
	unsigned credits_consumed;
#endif

	if (OFI_UNLIKELY(len < 8)) {
#ifndef NDEBUG
		credits_consumed =
#endif
			fi_opx_hfi1_tx_mp_egr_write_nth_packet_header_no_payload(
				opx_ep, &pio_state, replay->scb.qws, buf, bth_subctxt_rx, lrh_dlid_9B, lrh_dws,
				pbc_dlid, pbc_dws, xfer_bytes_tail, tail_bytes, payload_offset, psn, mp_egr_uid,
				hfi1_type);

	} else {
#ifndef NDEBUG
		credits_consumed =
#endif
			fi_opx_hfi1_tx_mp_egr_write_nth_packet_header(opx_ep, &pio_state, replay->scb.qws, buf,
								      bth_subctxt_rx, lrh_dlid_9B, lrh_dws, pbc_dlid,
								      pbc_dws, xfer_bytes_tail, tail_bytes,
								      payload_offset, psn, mp_egr_uid, hfi1_type);

		uint64_t *buf_qws = (uint64_t *) buf;

		if (OFI_LIKELY(full_block_credits_needed)) {
#ifndef NDEBUG
			credits_consumed +=
#endif
				fi_opx_hfi1_tx_egr_store_full_payload_blocks(opx_ep, &pio_state, buf_qws,
									     full_block_credits_needed,
									     total_credits_available - 1);
		}

		if (OFI_LIKELY(payload_qws_tail)) {
#ifndef NDEBUG
			credits_consumed +=
#endif
				fi_opx_hfi1_tx_egr_store_payload_tail(opx_ep, &pio_state,
								      buf_qws + (full_block_credits_needed << 3),
								      payload_qws_tail);
		}

		uint64_t *replay_payload_qws = replay->payload;
		for (int i = 0; i < full_block_credits_needed; i++) {
			opx_cacheline_store_block(replay_payload_qws, buf_qws);
			buf_qws += 8;
			replay_payload_qws += 8;
		}

		for (int i = 0; i < payload_qws_tail; i++) {
			replay_payload_qws[i] = buf_qws[i];
		}
	}

#ifndef NDEBUG
	assert(credits_consumed == total_credits_needed);
#endif

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;
	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, reliability,
							     hfi1_type);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-MP-EAGER-NTH");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- MULTI-PACKET EAGER NTH (end)\n");

	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
ssize_t fi_opx_hfi1_tx_send_mp_egr_nth_16B(struct fi_opx_ep *opx_ep, const void *buf, const uint32_t payload_offset,
					   const ssize_t len, const uint32_t mp_egr_uid, const uint64_t pbc_dlid,
					   const uint64_t bth_subctxt_rx, const uint64_t lrh_dlid_16B,
					   const union fi_opx_addr addr, int lock_required,
					   const enum ofi_reliability_kind reliability,
					   const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	assert(lock_required == 0);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND 16B, HFI -- MULTI-PACKET EAGER NTH (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-MP-EAGER-NTH");

	size_t xfer_bytes_tail = len & 0x07UL;

	const size_t payload_qws_total = (len - xfer_bytes_tail) >> 3;
	/* 16B (RcvPktCtrl=9) has 1 QW of KDETH and 1 QW of tail in PIO (non-SOP) */
	const size_t kdeth9_qws_total = 1;
	const size_t tail_qws_total   = 1;

	/* Full 64 byte/8 qword blocks -- 1 credit per block */
	uint16_t full_block_credits_needed = (uint16_t) ((kdeth9_qws_total + payload_qws_total + tail_qws_total) >> 3);
	/* Remaining tail qwords (< 8) after full blocks */
	size_t tail_partial_block_qws = (kdeth9_qws_total + payload_qws_total + tail_qws_total) & 0x07ul;

	const uint64_t pbc_dws =
		/* PIO SOP is 16 DWS/8 QWS*/
		2 + /* pbc */
		4 + /* lrh uncompressed */
		3 + /* bth */
		3 + /* kdeth */
		4 + /* software kdeth */
		/* PIO is everything else */
		(kdeth9_qws_total << 1) +  /* kdeth9 remaining 2 dws */
					   //--------------------- header split point KDETH 9 DWS
		(payload_qws_total << 1) + /* one packet payload */
		(tail_qws_total << 1);	   /* tail 1 qws/2 dws   */

	const uint16_t lrh_qws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */

	uint16_t total_credits_needed = 1 +			      /* PIO SOP -- 1 credit */
					full_block_credits_needed +   /* PIO full blocks -- kdeth9/payload/tail */
					(tail_partial_block_qws > 0); /* PIO partial block -- 1 credit */

	OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	uint16_t total_credits_available =
		FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		OPX_TRACER_TRACE(OPX_TRACER_END_ENOBUFS, "SEND-MP-EAGER-NTH");
		return -FI_ENOBUFS;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int32_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx,
					    &psn_ptr, &replay, reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-MP-EAGER-NTH");
		return -FI_EAGAIN;
	}

	uint64_t tail_bytes;

	if (xfer_bytes_tail) {
		fi_opx_duff_copy((uint8_t *) &tail_bytes, ((const uint8_t *) buf) + (len & -8UL), xfer_bytes_tail);
	} else {
		tail_bytes = 0UL;
	}

#ifndef NDEBUG
	unsigned credits_consumed;
#endif

	if (OFI_UNLIKELY(len < 8)) {
#ifndef NDEBUG
		credits_consumed =
#endif
			fi_opx_hfi1_tx_mp_egr_write_nth_packet_header_no_payload(
				opx_ep, &pio_state, replay->scb.qws, buf, bth_subctxt_rx, lrh_dlid_16B, lrh_qws,
				pbc_dlid, pbc_dws, xfer_bytes_tail, tail_bytes, payload_offset, psn, mp_egr_uid,
				hfi1_type);
	} else {
#ifndef NDEBUG
		credits_consumed =
#endif
			fi_opx_hfi1_tx_mp_egr_write_nth_packet_header(opx_ep, &pio_state, replay->scb.qws, buf,
								      bth_subctxt_rx, lrh_dlid_16B, lrh_qws, pbc_dlid,
								      pbc_dws, xfer_bytes_tail, tail_bytes,
								      payload_offset, psn, mp_egr_uid, hfi1_type);

		uint64_t *buf_qws = (uint64_t *) buf;

		/* write 7 qwords of payload data or the partial tail qws/block minus hdr/kdeth minus tail (not in
		 * buffer) */
		const size_t payload_after_hdr_qws = full_block_credits_needed ?
							     OPX_JKR_16B_PAYLOAD_AFTER_HDR_QWS :
							     tail_partial_block_qws - kdeth9_qws_total - tail_qws_total;

		/* header and payload */
#ifndef NDEBUG
		credits_consumed +=
#endif
			opx_hfi1_tx_store_16B_hdr_extension(opx_ep, &pio_state, replay->scb.qws,
							    (((uint64_t) mp_egr_uid) << 32) | payload_offset,
							    payload_after_hdr_qws, buf_qws);

		buf_qws += payload_after_hdr_qws /* qws of payload, not the kdeth qword */;

		/* adjust full or partial for what we just consumed */
		if (full_block_credits_needed) {
			full_block_credits_needed--;
		}
		/* we wrote 7 qw, counts as partial tail*/
		else {
			tail_partial_block_qws = 0;
		}

		if (OFI_LIKELY(full_block_credits_needed)) {
#ifndef NDEBUG
			credits_consumed +=
#endif
				fi_opx_hfi1_tx_egr_store_full_payload_blocks(opx_ep, &pio_state, buf_qws,
									     full_block_credits_needed,
									     total_credits_available - 2);
		}

		if (OFI_LIKELY(tail_partial_block_qws)) {
#ifndef NDEBUG
			credits_consumed +=
#endif
				fi_opx_hfi1_tx_egr_store_payload_tail(
					opx_ep, &pio_state, buf_qws + (full_block_credits_needed << 3),
					tail_partial_block_qws - 1); // (tail_partial_block_qws-1) data + 1 QW ICRC
		}

		uint64_t *replay_payload_qws = replay->payload;
		for (int i = 0; i < full_block_credits_needed; i++) {
			opx_cacheline_store_block(replay_payload_qws, buf_qws);
			buf_qws += 8;
			replay_payload_qws += 8;
		}

		for (int i = 0; i < tail_partial_block_qws; i++) {
			replay_payload_qws[i] = buf_qws[i];
		}
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

#ifndef NDEBUG
	assert(credits_consumed == total_credits_needed);
#endif

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, reliability,
							     hfi1_type);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-MP-EAGER-NTH");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- MULTI-PACKET EAGER NTH (end)\n");

	return FI_SUCCESS;
}

static inline void fi_opx_shm_write_fence(struct fi_opx_ep *opx_ep, const uint8_t dest_hfi_unit, const uint64_t dest_rx,
					  const uint64_t lrh_dlid, struct fi_opx_completion_counter *cc,
					  const uint64_t bytes_to_sync, const uint32_t dest_extended_rx,
					  enum opx_hfi1_type hfi1_type)
{
	const uint64_t bth_rx = dest_rx << OPX_BTH_SUBCTXT_RX_SHIFT;
	uint64_t       pos;
	ssize_t	       rc;
	/* DAOS support - rank_inst field has been depricated and will be phased out.
	 * The value is always zero. */
	union opx_hfi1_packet_hdr *hdr = opx_shm_tx_next(&opx_ep->tx->shm, dest_hfi_unit, dest_rx, &pos,
							 opx_ep->daos_info.hfi_rank_enabled, dest_extended_rx, 0, &rc);
	/* Potential infinite loop, unable to return result to application */
	while (OFI_UNLIKELY(hdr == NULL)) { // TODO: Verify that all callers of this function can tolderate a NULL rc
		fi_opx_shm_poll_many(&opx_ep->ep_fid, FI_OPX_LOCK_NOT_REQUIRED, OPX_HFI1_TYPE);
		hdr = opx_shm_tx_next(&opx_ep->tx->shm, dest_hfi_unit, dest_rx, &pos,
				      opx_ep->daos_info.hfi_rank_enabled, dest_extended_rx, 0, &rc);
	}

	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_JKR_9B)) {
		const uint64_t pbc_dws = 2 + /* pbc */
					 2 + /* lrh */
					 3 + /* bth */
					 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
					 (0 << 4);
		const uint16_t lrh_dws = __cpu_to_be16(
			pbc_dws - 2 +
			1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */
		hdr->qw_9B[0] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[0] | lrh_dlid | ((uint64_t) lrh_dws << 32);
		hdr->qw_9B[1] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[1] | bth_rx;
		hdr->qw_9B[2] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[2];
		hdr->qw_9B[3] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[3];
		hdr->qw_9B[4] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[4] | FI_OPX_HFI_DPUT_OPCODE_FENCE | (0ULL << 32);
		hdr->qw_9B[5] = (uintptr_t) cc;
		hdr->qw_9B[6] = bytes_to_sync;
	} else {
		const uint64_t pbc_dws = 2 +		     /* pbc */
					 4 +		     /* lrh uncompressed */
					 3 +		     /* bth */
					 9 +		     /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
					 2;		     /* ICRC/tail */
		const uint16_t lrh_dws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
		hdr->qw_16B[0] =
			opx_ep->rx->tx.cts_16B.hdr.qw_16B[0] |
			((uint64_t) (lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
			((uint64_t) lrh_dws << 20);
		hdr->qw_16B[1] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[1] |
				 ((uint64_t) ((lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
					      OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
				 (uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
		hdr->qw_16B[2] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[2] | bth_rx;
		hdr->qw_16B[3] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[3];
		hdr->qw_16B[4] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[4];
		hdr->qw_16B[5] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[5] | FI_OPX_HFI_DPUT_OPCODE_FENCE | (0ULL << 32);
		hdr->qw_16B[6] = (uintptr_t) cc;
		hdr->qw_16B[7] = bytes_to_sync;
	}
	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);
}

ssize_t opx_hfi1_tx_sendv_rzv(struct fid_ep *ep, const struct iovec *iov, size_t niov, size_t total_len,
			      fi_addr_t dest_addr, uint64_t tag, void *user_context, const uint32_t data,
			      int lock_required, const unsigned override_flags, const uint64_t tx_op_flags,
			      const uint64_t dest_rx, const uint64_t caps, const enum ofi_reliability_kind reliability,
			      const uint64_t do_cq_completion, const enum fi_hmem_iface hmem_iface,
			      const uint64_t hmem_device, const uint64_t hmem_handle,
			      const enum opx_hfi1_type hfi1_type, const bool ctx_sharing);

ssize_t opx_hfi1_tx_send_rzv(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
			     void *user_context, const uint32_t data, int lock_required, const unsigned override_flags,
			     const uint64_t tx_op_flags, const uint64_t dest_rx, const uint64_t caps,
			     const enum ofi_reliability_kind reliability, const uint64_t do_cq_completion,
			     const enum fi_hmem_iface hmem_iface, const uint64_t hmem_handle,
			     const uint64_t hmem_device, const enum opx_hfi1_type hfi1_type, const bool ctx_sharing);

ssize_t opx_hfi1_tx_send_rzv_16B(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
				 void *user_context, const uint32_t data, int lock_required,
				 const unsigned override_flags, const uint64_t tx_op_flags, const uint64_t dest_rx,
				 const uint64_t caps, const enum ofi_reliability_kind reliability,
				 const uint64_t do_cq_completion, const enum fi_hmem_iface hmem_iface,
				 const uint64_t hmem_handle, const uint64_t hmem_device,
				 const enum opx_hfi1_type hfi1_type, const bool ctx_sharing);

__OPX_FORCE_INLINE__
ssize_t opx_hfi1_tx_send_rzv_select(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
				    void *context, const uint32_t data, int lock_required,
				    const unsigned override_flags, const uint64_t tx_op_flags, const uint64_t dest_rx,
				    const uint64_t caps, const enum ofi_reliability_kind reliability,
				    const uint64_t do_cq_completion, const enum fi_hmem_iface hmem_iface,
				    const uint64_t hmem_device, const uint64_t hmem_handle,
				    const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	if (hfi1_type & OPX_HFI1_WFR) {
		return opx_hfi1_tx_send_rzv(ep, buf, len, dest_addr, tag, context, data, lock_required, override_flags,
					    tx_op_flags, dest_rx, caps, reliability, do_cq_completion, hmem_iface,
					    hmem_device, hmem_handle, OPX_HFI1_WFR, ctx_sharing);
	} else if (hfi1_type & OPX_HFI1_JKR) {
		return opx_hfi1_tx_send_rzv_16B(ep, buf, len, dest_addr, tag, context, data, lock_required,
						override_flags, tx_op_flags, dest_rx, caps, reliability,
						do_cq_completion, hmem_iface, hmem_device, hmem_handle, OPX_HFI1_JKR,
						ctx_sharing);
	} else if (hfi1_type & OPX_HFI1_CYR) {
		return opx_hfi1_tx_send_rzv_16B(ep, buf, len, dest_addr, tag, context, data, lock_required,
						override_flags, tx_op_flags, dest_rx, caps, reliability,
						do_cq_completion, hmem_iface, hmem_device, hmem_handle, OPX_HFI1_CYR,
						ctx_sharing);
	} else if (hfi1_type & OPX_HFI1_JKR_9B) {
		return opx_hfi1_tx_send_rzv(ep, buf, len, dest_addr, tag, context, data, lock_required, override_flags,
					    tx_op_flags, dest_rx, caps, reliability, do_cq_completion, hmem_iface,
					    hmem_device, hmem_handle, OPX_HFI1_JKR_9B, ctx_sharing);
	}
	abort();
	return (ssize_t) -1L;
}

void opx_hfi1_rx_rma_rts(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr,
			 const void *const payload, const uint64_t niov, uintptr_t origin_rma_req,
			 struct opx_context *const target_context, const uintptr_t dst_vaddr,
			 const enum fi_hmem_iface dst_iface, const uint64_t dst_device, const uint64_t dst_handle,
			 const union opx_hfi1_dput_iov *src_iovs, const unsigned is_shm,
			 const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type);

#endif /* _FI_PROV_OPX_HFI1_TRANSPORT_H_ */

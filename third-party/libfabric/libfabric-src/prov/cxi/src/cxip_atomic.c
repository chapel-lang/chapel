/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2014 Intel Corporation, Inc. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2018,2021-2023 Hewlett Packard Enterprise Development LP
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include <ofi_list.h>
#include <ofi.h>

#include "cxip.h"

#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EP_CTRL, __VA_ARGS__)

/* Cassini supports ONLY 1-element vectors, and this code presumes that the
 * value is 1.
 */
_Static_assert(CXIP_AMO_MAX_IOV == 1, "Unexpected max IOV #");

/* Cassini supports ONLY 1-element packed IOVs.
 */
#define CXIP_AMO_MAX_PACKED_IOV (1)

/**
 * Data type codes for all of the supported fi_datatype values.
 */
static enum c_atomic_type _cxip_amo_type_code[FI_DATATYPE_LAST] = {
	[FI_INT8]	  = C_AMO_TYPE_INT8_T,
	[FI_UINT8]	  = C_AMO_TYPE_UINT8_T,
	[FI_INT16]	  = C_AMO_TYPE_INT16_T,
	[FI_UINT16]	  = C_AMO_TYPE_UINT16_T,
	[FI_INT32]	  = C_AMO_TYPE_INT32_T,
	[FI_UINT32]	  = C_AMO_TYPE_UINT32_T,
	[FI_INT64]	  = C_AMO_TYPE_INT64_T,
	[FI_UINT64]	  = C_AMO_TYPE_UINT64_T,
	[FI_FLOAT]	  = C_AMO_TYPE_FLOAT_T,
	[FI_DOUBLE]	  = C_AMO_TYPE_DOUBLE_T,
	[FI_FLOAT_COMPLEX]	  = C_AMO_TYPE_FLOAT_COMPLEX_T,
	[FI_DOUBLE_COMPLEX]	  = C_AMO_TYPE_DOUBLE_COMPLEX_T,
};
//TODO: C_AMO_TYPE_UINT128_T

/**
 * AMO operation codes for all of the fi_op values.
 */
static enum c_atomic_op _cxip_amo_op_code[FI_ATOMIC_OP_LAST] = {
	[FI_MIN]	  = C_AMO_OP_MIN,
	[FI_MAX]	  = C_AMO_OP_MAX,
	[FI_SUM]	  = C_AMO_OP_SUM,
	[FI_LOR]	  = C_AMO_OP_LOR,
	[FI_LAND]	  = C_AMO_OP_LAND,
	[FI_BOR]	  = C_AMO_OP_BOR,
	[FI_BAND]	  = C_AMO_OP_BAND,
	[FI_LXOR]	  = C_AMO_OP_LXOR,
	[FI_BXOR]	  = C_AMO_OP_BXOR,
	[FI_ATOMIC_READ]  = C_AMO_OP_SUM,

	/* ATOMIC_WRITE is implemented as a CSWAP NE instead of SWAP. This
	 * allows for SWAP to be remapped to PCIe fadd.
	 */
	[FI_ATOMIC_WRITE] = C_AMO_OP_CSWAP,
	[FI_CSWAP]	  = C_AMO_OP_CSWAP,
	[FI_CSWAP_NE]	  = C_AMO_OP_CSWAP,
	[FI_CSWAP_LE]	  = C_AMO_OP_CSWAP,
	[FI_CSWAP_LT]	  = C_AMO_OP_CSWAP,
	[FI_CSWAP_GE]	  = C_AMO_OP_CSWAP,
	[FI_CSWAP_GT]	  = C_AMO_OP_CSWAP,
	[FI_MSWAP]	  = C_AMO_OP_AXOR,	/* special handling */
};

/**
 * AMO swap operation codes for the CSWAP comparison conditions.
 */
static enum c_cswap_op _cxip_amo_swpcode[FI_ATOMIC_OP_LAST] = {
	[FI_CSWAP]	  = C_AMO_OP_CSWAP_EQ,
	[FI_CSWAP_NE]	  = C_AMO_OP_CSWAP_NE,
	[FI_CSWAP_LE]	  = C_AMO_OP_CSWAP_LE,
	[FI_CSWAP_LT]	  = C_AMO_OP_CSWAP_LT,
	[FI_CSWAP_GE]	  = C_AMO_OP_CSWAP_GE,
	[FI_CSWAP_GT]	  = C_AMO_OP_CSWAP_GT,
};

/**
 * Multi-dimensional array defining supported/unsupported operations. Bits
 * correspond to the 14 possible fi_datatype values. The OP_VALID() macro will
 * return a 1 if the (request,op,dt) triple is supported by Cassini.
 */
static uint16_t _cxip_amo_valid[CXIP_RQ_AMO_LAST][FI_ATOMIC_OP_LAST] = {

	[CXIP_RQ_AMO] = {
		[FI_MIN]	  = 0x03ff,
		[FI_MAX]	  = 0x03ff,
		[FI_SUM]	  = 0x0fff,
		[FI_LOR]	  = 0x00ff,
		[FI_LAND]	  = 0x00ff,
		[FI_LXOR]	  = 0x00ff,
		[FI_BOR]	  = 0x00ff,
		[FI_BAND]	  = 0x00ff,
		[FI_BXOR]	  = 0x00ff,
		[FI_ATOMIC_WRITE] = 0x0fff,
	},

	[CXIP_RQ_AMO_FETCH] = {
		[FI_MIN]	  = 0x03ff,
		[FI_MAX]	  = 0x03ff,
		[FI_SUM]	  = 0x0fff,
		[FI_LOR]	  = 0x00ff,
		[FI_LAND]	  = 0x00ff,
		[FI_LXOR]	  = 0x00ff,
		[FI_BOR]	  = 0x00ff,
		[FI_BAND]	  = 0x00ff,
		[FI_BXOR]	  = 0x00ff,
		[FI_ATOMIC_WRITE] = 0x0fff,
		[FI_ATOMIC_READ]  = 0x0fff,
	},

	[CXIP_RQ_AMO_SWAP] = {
		[FI_CSWAP]	  = 0x0fff,
		[FI_CSWAP_NE]	  = 0x0fff,
		[FI_CSWAP_LE]	  = 0x03ff,
		[FI_CSWAP_LT]	  = 0x03ff,
		[FI_CSWAP_GE]	  = 0x03ff,
		[FI_CSWAP_GT]	  = 0x03ff,
		[FI_MSWAP]	  = 0x00ff,
	},

	[CXIP_RQ_AMO_PCIE_FETCH] = {
		[FI_MIN]	  = 0x0,
		[FI_MAX]	  = 0x0,
		[FI_SUM]	  = 0xf0,
		[FI_LOR]	  = 0x0,
		[FI_LAND]	  = 0x0,
		[FI_LXOR]	  = 0x0,
		[FI_BOR]	  = 0x0,
		[FI_BAND]	  = 0x0,
		[FI_BXOR]	  = 0x0,
		[FI_ATOMIC_WRITE] = 0x0,
		[FI_ATOMIC_READ]  = 0x0,
	},

};
#define	OP_VALID(rq, op, dt)	(_cxip_amo_valid[rq][op] & (1 << dt))

/**
 * Supply opcodes for a request, and determine if the operation is supported.
 *
 * @param req_type basic, fetch, or swap
 * @param dt data type for operation
 * @param op operation
 * @param amo_remap_to_pcie_fadd NIC AMO operation which is remapped as PCIe
 * fetch add
 * @param cop Cassini code for operation
 * @param cdt Cassini code for data type
 * @param copswp Cassini code for cswap operation
 * @param cdtlen Length of datatype in bytes
 *
 * @return int 0 on success, -FI_EOPNOTSUPP if operation is not supported
 */
int _cxip_atomic_opcode(enum cxip_amo_req_type req_type, enum fi_datatype dt,
			enum fi_op op, int amo_remap_to_pcie_fadd,
			enum c_atomic_op *cop, enum c_atomic_type *cdt,
			enum c_cswap_op *copswp, unsigned int *cdtlen)
{
	int opcode;
	int dtcode;

	if (dt < 0 || dt >= FI_DATATYPE_LAST ||
	    op < 0 || op >= FI_ATOMIC_OP_LAST)
		return -FI_EINVAL;

	if (!OP_VALID(req_type, op, dt))
		return -FI_EOPNOTSUPP;

	/* If the request is a PCIe fetching AMO, then the remap opcode is
	 * used.
	 *
	 * Note: Only fetching FI_SUM is supported as a PCIe AMO.
	 */
	if (req_type == CXIP_RQ_AMO_PCIE_FETCH) {
		if (amo_remap_to_pcie_fadd >= 0)
			opcode = amo_remap_to_pcie_fadd;
		else
			return -FI_EOPNOTSUPP;
	} else {
		opcode = _cxip_amo_op_code[op];
		if (opcode == amo_remap_to_pcie_fadd)
			return -FI_EOPNOTSUPP;
	}

	/* For fetching FI_SUMs done as a PCIe AMO, force signed data types to
	 * unsigned. This is required by the NIC to allow libfabric to support
	 * signed PCIe fetching FI_SUMs.
	 */
	dtcode = _cxip_amo_type_code[dt];
	if (req_type == CXIP_RQ_AMO_PCIE_FETCH) {
		if (dtcode == C_AMO_TYPE_INT32_T)
			dtcode = C_AMO_TYPE_UINT32_T;
		else if (dtcode == C_AMO_TYPE_INT64_T)
			dtcode = C_AMO_TYPE_UINT64_T;
	}

	if (cop)
		*cop = opcode;
	if (cdt)
		*cdt = dtcode;
	if (cdtlen)
		*cdtlen = ofi_datatype_size(dt);
	if (copswp) {
		if (op == FI_ATOMIC_WRITE)
			*copswp = C_AMO_OP_CSWAP_NE;
		else
			*copswp = _cxip_amo_swpcode[op];
	}

	return 0;
}

/**
 * Implementation of the provider *_atomic_valid() functions.
 *
 * The returned count is the maximum number of atomic objects on which a single
 * atomic call can operate. For Cassini, this is 1.
 *
 * @param ep endpoint
 * @param req_type request type
 * @param datatype datatype
 * @param op operation
 * @param count returns count of operations supported
 *
 * @return int 0 on success, -FI_EOPNOTSUPP if operation not supported
 */
static inline int _cxip_ep_valid(struct fid_ep *fid_ep,
				 enum cxip_amo_req_type req_type,
				 enum fi_datatype datatype,
				 enum fi_op op,
				 size_t *count)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	int ret;

	/* Check for a valid opcode */
	ret = _cxip_atomic_opcode(req_type, datatype, op,
				  ep->ep_obj->domain->amo_remap_to_pcie_fadd,
				  NULL, NULL, NULL, NULL);
	if (ret < 0)
		return ret;

	/* "Cassini implements single element atomics. There is no hardware
	 *  support for packed atomics or IOVECs." -- CSDG
	 */
	if (count)
		*count = CXIP_AMO_MAX_IOV;

	return 0;
}

/*
 * cxip_amo_inject_cb() - AMO inject event callback.
 */
static int cxip_amo_inject_cb(struct cxip_req *req, const union c_event *event)
{
	/* When errors happen, send events can occur before the put/get event.
	 * These events should just be dropped.
	 */
	if (event->hdr.event_type == C_EVENT_SEND) {
		CXIP_WARN(CXIP_UNEXPECTED_EVENT,
			  cxi_event_to_str(event),
			  cxi_rc_to_str(cxi_event_rc(event)));
		return FI_SUCCESS;
	}

	int event_rc;

	event_rc = cxi_init_event_rc(event);
	int ret_err;

	ret_err = proverr2errno(event_rc);
	return cxip_cq_req_error(req, 0, ret_err,
				 cxi_event_rc(event), NULL, 0,
				 FI_ADDR_UNSPEC);
}

/*
 * cxip_amo_selective_completion_req() - Return request state associated with
 * all AMO inject transactions on the transmit context.
 *
 * The request is freed when the TXC send CQ is closed.
 */
static struct cxip_req *cxip_amo_selective_completion_req(struct cxip_txc *txc)
{
	if (!txc->amo_selective_completion_req) {
		struct cxip_req *req;
		bool free_request = false;

		req = cxip_evtq_req_alloc(&txc->tx_evtq, 0, txc);
		if (!req)
			return NULL;

		req->cb = cxip_amo_inject_cb;
		req->context = (uint64_t)txc->context;
		req->flags = FI_ATOMIC | FI_WRITE;
		req->addr = FI_ADDR_UNSPEC;

		if (!txc->amo_selective_completion_req)
			txc->amo_selective_completion_req = req;
		else
			free_request = true;

		if (free_request)
			cxip_evtq_req_free(req);
	}

	return txc->amo_selective_completion_req;
}

/*
 * cxip_amo_fetching_selective_completion_req() - Return request state
 * associated with all fetching AMO inject transactions on the transmit context.
 *
 * The request is freed when the TXC send CQ is closed.
 */
static struct cxip_req *
cxip_amo_fetching_selective_completion_req(struct cxip_txc *txc)
{
	if (!txc->amo_fetch_selective_completion_req) {
		struct cxip_req *req;
		bool free_request = false;

		req = cxip_evtq_req_alloc(&txc->tx_evtq, 0, txc);
		if (!req)
			return NULL;

		req->cb = cxip_amo_inject_cb;
		req->context = (uint64_t)txc->context;
		req->flags = FI_ATOMIC | FI_READ;
		req->addr = FI_ADDR_UNSPEC;

		if (!txc->amo_fetch_selective_completion_req)
			txc->amo_fetch_selective_completion_req = req;
		else
			free_request = true;

		if (free_request)
			cxip_evtq_req_free(req);
	}

	return txc->amo_fetch_selective_completion_req;
}

/**
 * Callback for non-fetching AMO operations.
 *
 * @param req AMO request structure
 * @param event resulting event
 */
static int _cxip_amo_cb(struct cxip_req *req, const union c_event *event)
{
	int ret;
	int event_rc;
	int ret_err;
	int success_event = (req->flags & FI_COMPLETION);
	struct cxip_txc *txc = req->amo.txc;

	/* When errors happen, send events can occur before the put/get event.
	 * These events should just be dropped.
	 */
	if (event->hdr.event_type == C_EVENT_SEND) {
		TXC_WARN(txc, CXIP_UNEXPECTED_EVENT,
			 cxi_event_to_str(event),
			 cxi_rc_to_str(cxi_event_rc(event)));
		return FI_SUCCESS;
	}

	/* Fetching AMO with flush requires two events. Only once two events are
	 * processed can the user-generated completion queue event be
	 * generated. In addition, since multiple initiator events are
	 * generated and zero assumptions can be made about the event order,
	 * counters cannot be incremented until both events are processed.
	 * This means that software must modify the counter (i/e it cannot be
	 * offloaded to hardware).
	 */
	if (req->amo.fetching_amo_flush) {
		req->amo.fetching_amo_flush_event_count++;

		if (event->hdr.event_type == C_EVENT_REPLY)
			req->amo.fetching_amo_flush_event_rc =
				cxi_init_event_rc(event);

		if (req->amo.fetching_amo_flush_event_count != 2)
			return FI_SUCCESS;

		event_rc = req->amo.fetching_amo_flush_event_rc;

		if (req->amo.fetching_amo_flush_cntr) {
			if (event_rc == C_RC_OK)
				ret = cxip_cntr_mod(req->amo.fetching_amo_flush_cntr,
						    1, false, false);
			else
				ret = cxip_cntr_mod(req->amo.fetching_amo_flush_cntr,
						    1, false, true);

			if (ret != FI_SUCCESS) {
				req->amo.fetching_amo_flush_event_count--;
				return ret;
			}
		}
	} else {
		event_rc = cxi_init_event_rc(event);
	}

	if (req->amo.result_md)
		cxip_unmap(req->amo.result_md);

	if (req->amo.oper1_md)
		cxip_unmap(req->amo.oper1_md);

	if (req->amo.ibuf)
		cxip_txc_ibuf_free(txc, req->amo.ibuf);

	req->flags &= (FI_ATOMIC | FI_READ | FI_WRITE);

	if (event_rc == C_RC_OK) {
		if (success_event) {
			ret = cxip_cq_req_complete(req);
			if (ret != FI_SUCCESS)
				TXC_WARN_RET(txc, ret,
					     "Failed to report completion\n");
		}
	} else {
		ret_err = proverr2errno(event_rc);

		ret = cxip_cq_req_error(req, 0, ret_err,
					event_rc, NULL, 0,
					FI_ADDR_UNSPEC);

		if (ret != FI_SUCCESS)
			TXC_WARN_RET(txc, ret, "Failed to report error\n");
	}

	ofi_atomic_dec32(&req->amo.txc->otx_reqs);
	cxip_evtq_req_free(req);

	return FI_SUCCESS;
}

/**
 * Return true if vector specification is valid.
 *
 * vn must be > 0 and <= 1 (CXIP_AMO_MAX_IOV). Formally, we could do this test,
 * but formally we would have to loop (once) over the vectors, and test each
 * count for being > 0 and <= 1 (CXIP_AMO_MAX_PACKED_IOV). Instead, we just test
 * to ensure that each is 1.
 *
 * @param vn vector element count
 * @param v vector pointer
 *
 * @return bool true if vector is valid, false otherwise
 */
static inline bool _vector_valid(size_t vn, const struct fi_ioc *v)
{
	return (vn == CXIP_AMO_MAX_IOV && v &&
		v[0].count == CXIP_AMO_MAX_PACKED_IOV &&
		v[0].addr);
}

/**
 * Return true if RMA vector specification is valid. Note that the address is
 * treated as an offset into an RMA MR window, so a value of zero is valid.
 *
 * @param vn vector element count
 * @param v vector pointer
 *
 * @return bool true if RMA vector is valid, false otherwise
 */
static inline bool _rma_vector_valid(size_t vn, const struct fi_rma_ioc *v)
{
	return (vn == CXIP_AMO_MAX_IOV && v &&
		v[0].count == CXIP_AMO_MAX_PACKED_IOV);
}

static bool cxip_amo_emit_idc_req_needed(uint64_t flags, void *result,
					 struct cxip_mr *result_mr,
					 bool fetching_amo_flush)
{
	/* User completion events always require a tracking structure. */
	if (flags & FI_COMPLETION)
		return true;

	/* If a fetching operation (i.e. result buffer is valid) and the user
	 * did not provide an MR for the result arg, internal memory
	 * registration needs to occur. This requires tracking.
	 */
	if (result && !result_mr)
		return true;

	/* Fetching AMO with flush always requires a request struct since two
	 * operations are required to implement it.
	 */
	if (fetching_amo_flush)
		return true;

	return false;
}

/* TODO: Update HMEM buf type for 128-bit AMOs. */
static int cxip_amo_emit_idc(struct cxip_txc *txc,
			     enum cxip_amo_req_type req_type,
			     const struct fi_msg_atomic *msg, void *buf,
			     void *compare, void *result,
			     struct cxip_mr *result_mr, uint64_t key,
			     uint64_t remote_offset, union c_fab_addr *dfa,
			     uint8_t *idx_ext, uint16_t vni,
			     enum c_atomic_op atomic_op,
			     enum c_cswap_op cswap_op,
			     enum c_atomic_type atomic_type,
			     unsigned int atomic_type_len, uint64_t flags,
			     uint32_t tclass)
{
	struct cxip_domain *dom = txc->domain;
	struct cxip_md *result_md = NULL;
	struct c_cstate_cmd cstate_cmd = {};
	struct c_idc_amo_cmd idc_amo_cmd = {};
	struct cxip_req *req = NULL;
	bool flush = !!(flags & (FI_DELIVERY_COMPLETE | FI_MATCH_COMPLETE));
	bool fetching = result != NULL;
	bool fetching_amo_flush = fetching && flush;
	bool restricted = !!(flags & FI_CXI_UNRELIABLE);
	int ret;
	void *selective_completion_req;
	enum cxi_traffic_class_type tc_type;
	uint64_t hmem_buf;
	uint64_t hmem_compare;
	bool tgt_events = cxip_generic_is_mr_key_events(txc->ep_obj->caps,
							key);

	/* MR desc cannot be value unless hybrid MR desc is enabled. */
	if (!dom->hybrid_mr_desc)
		result_mr = NULL;

	/* Restricted AMOs must target optimized MRs without target events */
	if (restricted && tgt_events) {
		TXC_WARN(txc,
			 "Restricted AMOs with FI_RMA_EVENT not supported\n");
		return -FI_EINVAL;
	}

	/* Usage of the FI_CXI_HRP requires FI_CXI_UNRELIABLE. */
	if (flags & FI_CXI_HRP && !(flags & FI_CXI_UNRELIABLE)) {
		TXC_WARN(txc, "FI_CXI_HRP requires FI_CXI_UNRELIABLE\n");
		return -FI_EINVAL;
	}

	/* Since fetching AMO with flush results in two commands, if
	 * FI_RMA_EVENT is enabled, this would results in two remote MR counter
	 * increments. Thus, this functionality cannot be supported.
	 */
	if (fetching_amo_flush && tgt_events) {
		TXC_WARN(txc,
			 "Fetching AMO with FI_DELIVERY_COMPLETE not supported with FI_RMA_EVENT\n");
		return -FI_EINVAL;
	}

	/* Work around for silent drops at the target for non-fetching
	 * FI_UNIT32 atomic operations when using FI_CXI_HRP. Force
	 * switching out of HRP if necessary.
	 */
	if (txc->hrp_war_req && (flags & FI_CXI_HRP) &&
	    req_type == CXIP_RQ_AMO && msg->datatype == FI_UINT32)
		flags &= ~FI_CXI_HRP;

	ofi_genlock_lock(&txc->ep_obj->lock);
	if (cxip_amo_emit_idc_req_needed(flags, result, result_mr,
	    fetching_amo_flush)) {
		/* if (result && !result_mr) we end up in this branch */
		req = cxip_evtq_req_alloc(&txc->tx_evtq, 0, txc);
		if (!req) {
			TXC_WARN(txc, "Failed to allocate request\n");
			ret = -FI_EAGAIN;
			goto err;
		}

		/* Values set here are passed back to the user through the CQ */
		if (flags & FI_COMPLETION)
			req->context = (uint64_t)msg->context;
		else
			req->context = (uint64_t)txc->context;
		req->flags = FI_ATOMIC;
		req->flags |= (req_type == CXIP_RQ_AMO ? FI_WRITE : FI_READ);
		req->flags |= (flags & FI_COMPLETION);
		req->cb = _cxip_amo_cb;
		req->amo.txc = txc;
		req->amo.fetching_amo_flush = fetching_amo_flush;
		req->type = CXIP_REQ_AMO;

		/* For fetching AMOs, the result buffer (i.e. fetch buffer) must
		 * always be registered.
		 */
		if (result) {
			if (result_mr) {
				result_md = result_mr->md;
			} else {
				ret = cxip_map(dom, result, atomic_type_len, 0,
					       &req->amo.result_md);
				if (ret) {
					TXC_WARN_RET(txc, ret,
						     "Failed to map result buffer\n");
					goto err_free_req;
				}

				result_md = req->amo.result_md;
			}
		}
	} else if (result_mr) {
		result_md = result_mr->md;
	}
	/* else {result == false} */

	/* Identify the correct traffic class sub-type. */
	if (flags & FI_CXI_HRP)
		tc_type = CXI_TC_TYPE_HRP;
	else if (flags & FI_CXI_UNRELIABLE)
		tc_type = CXI_TC_TYPE_RESTRICTED;
	else
		tc_type = CXI_TC_TYPE_DEFAULT;

	/* Prepare the c-state command for the AMO IDC operation. */
	if (result)
		cstate_cmd.write_lac = result_md->md->lac;

	cstate_cmd.event_send_disable = 1;
	cstate_cmd.index_ext = *idx_ext;
	cstate_cmd.eq = cxip_evtq_eqn(&txc->tx_evtq);
	cstate_cmd.restricted = restricted;

	/* If a request structure is not allocated, success events will be
	 * disabled. But, if for some reason the operation completes with an
	 * error, an event will occur. For this case, a TXC inject request is
	 * allocated. This request enables the reporting of failed operation to
	 * the completion queue. This request is freed when the TXC is closed.
	 */
	if (req) {
		cstate_cmd.user_ptr = (uint64_t)req;
	} else {
		if (req_type == CXIP_RQ_AMO)
			selective_completion_req =
				cxip_amo_selective_completion_req(txc);
		else
			selective_completion_req =
				cxip_amo_fetching_selective_completion_req(txc);

		if (!selective_completion_req) {
			ret = -FI_EAGAIN;
			TXC_WARN(txc,
				 "Failed to allocate selective completion request\n");
			goto err_unmap_result_buf;
		}

		cstate_cmd.user_ptr = (uint64_t)selective_completion_req;
		cstate_cmd.event_success_disable = 1;
	}

	/* Fetching AMO with flushes requires a trailing zero-byte put with
	 * flush. Normal AMOs can use the operation flush functionality.
	 */
	if (!fetching_amo_flush) {
		if (flush)
			cstate_cmd.flush = 1;

		if (req_type == CXIP_RQ_AMO) {
			if (txc->write_cntr) {
				cstate_cmd.event_ct_ack = 1;
				cstate_cmd.ct = txc->write_cntr->ct->ctn;
			}
		} else {
			if (txc->read_cntr) {
				cstate_cmd.event_ct_reply = 1;
				cstate_cmd.ct = txc->read_cntr->ct->ctn;
			}
		}
	}

	/* Prepare the IDC AMO command. */
	idc_amo_cmd.idc_header.dfa = *dfa;
	idc_amo_cmd.idc_header.remote_offset = remote_offset;
	idc_amo_cmd.atomic_op = atomic_op;
	idc_amo_cmd.atomic_type = atomic_type;
	idc_amo_cmd.cswap_op = cswap_op;

	/* if (result) {result_md is set} */
	if (result)
		idc_amo_cmd.local_addr = CXI_VA_TO_IOVA(result_md->md, result);

	switch (msg->op) {
	case FI_MSWAP:
		ret = cxip_txc_copy_from_hmem(txc, NULL, &hmem_buf, buf,
					      atomic_type_len);
		if (ret) {
			TXC_WARN(txc, "cxip_txc_copy_from_hmem failed: %d:%s\n",
				 ret, fi_strerror(-ret));
			goto err_unmap_result_buf;
		}

		ret = cxip_txc_copy_from_hmem(txc, NULL, &hmem_compare, compare,
					      atomic_type_len);
		if (ret) {
			TXC_WARN(txc, "cxip_txc_copy_from_hmem failed: %d:%s\n",
				 ret, fi_strerror(-ret));
			goto err_unmap_result_buf;
		}

		hmem_buf &= hmem_compare;

		/* Note: 16-byte value will overflow into op2_word2 */
		memcpy(&idc_amo_cmd.op1_word1, &hmem_buf, atomic_type_len);
		break;

	/* FI_ATOMIC_READ is implemented as a sum of zero. Thus, only copy over
	 * the buffer contents for non-FI_ATOMIC_READ operations.
	 */
	case FI_ATOMIC_READ:
		break;

	/* FI_ATOMIC_WRITE is implemented as a CSWAP NE operation. For this to
	 * work, the compare buffer (i.e. operand 2) needs to have the same
	 * contents as the write payload (i.e. operand 1).
	 */
	case FI_ATOMIC_WRITE:
		assert(compare == NULL);

		ret = cxip_txc_copy_from_hmem(txc, NULL, &idc_amo_cmd.op2_word1,
					      buf, atomic_type_len);
		if (ret) {
			TXC_WARN(txc, "cxip_txc_copy_from_hmem failed: %d:%s\n",
				 ret, fi_strerror(-ret));
			goto err_unmap_result_buf;
		}

		/* Fall through. */
	default:
		ret = cxip_txc_copy_from_hmem(txc, NULL, &idc_amo_cmd.op1_word1,
					      buf, atomic_type_len);
		if (ret) {
			TXC_WARN(txc, "cxip_txc_copy_from_hmem failed: %d:%s\n",
				 ret, fi_strerror(-ret));
			goto err_unmap_result_buf;
		}
	}

	if (compare) {
		/* Note: 16-byte value will overflow into op2_word2 */
		ret = cxip_txc_copy_from_hmem(txc, NULL, &idc_amo_cmd.op2_word1,
					      compare, atomic_type_len);
		if (ret) {
			TXC_WARN(txc, "cxip_txc_copy_from_hmem failed: %d:%s\n",
				 ret, fi_strerror(-ret));
			goto err_unmap_result_buf;
		}
	}

	/* Optionally configure the flushing command used for fetching AMOs. */
	if (fetching_amo_flush) {
		assert(req != NULL);
		if (req_type == CXIP_RQ_AMO)
			req->amo.fetching_amo_flush_cntr = txc->write_cntr;
		else
			req->amo.fetching_amo_flush_cntr = txc->read_cntr;
	}

	ret = cxip_txc_emit_idc_amo(txc, vni, cxip_ofi_to_cxi_tc(tclass),
				    tc_type, &cstate_cmd, &idc_amo_cmd, flags,
				    fetching, flush);
	if (ret) {
		TXC_WARN_RET(txc, ret, "Failed to emit IDC amo\n");
		goto err_unmap_result_buf;
	}

	ofi_genlock_unlock(&txc->ep_obj->lock);

	return FI_SUCCESS;

err_unmap_result_buf:
	if (req && req->amo.result_md)
		cxip_unmap(req->amo.result_md);
err_free_req:
	if (req)
		cxip_evtq_req_free(req);
err:
	ofi_genlock_unlock(&txc->ep_obj->lock);

	TXC_WARN_RET(txc, ret,
		     "%s IDC %s failed: atomic_op=%u cswap_op=%u atomic_type=%u buf=%p compare=%p result=%p len=%u roffset=%#lx nid=%#x ep=%u idx_ext=%u\n",
		     restricted ? "Restricted" : "Unrestricted",
		     fetching ? "FAMO" : "AMO", atomic_op, cswap_op,
		     atomic_type, buf, compare, result, atomic_type_len,
		     remote_offset, dfa->unicast.nid,
		     dfa->unicast.endpoint_defined, *idx_ext);

	return ret;
}

static bool cxip_amo_emit_dma_req_needed(const struct fi_msg_atomic *msg,
					 uint64_t flags, void *result,
					 struct cxip_mr *buf_mr,
					 struct cxip_mr *result_mr,
					 bool fetching_amo_flush)
{
	/* To support FI_INJECt + DMA operations, an internal bounce buffer is
	 * needed. This buffer is tracked in the request structure.
	 */
	if (flags & FI_INJECT)
		return true;

	/* User completion events always require a tracking structure. */
	if (flags & FI_COMPLETION)
		return true;

	/* If the user did not provide an MR for the buffer arg, internal memory
	 * registration needs to occur. This requires tracking.
	 */
	if (!buf_mr)
		return true;

	/* If a fetching operation (i.e. result buffer is valid) and the user
	 * did not provide an MR for the result arg, internal memory
	 * registration needs to occur. This requires tracking.
	 */
	if (result && !result_mr)
		return true;

	/* FI_ATOMIC_READ and FI_MSWAP are require the use of an internal bounce
	 * buffer. This requires tracking.
	 */
	if (msg->op == FI_ATOMIC_READ || msg->op == FI_MSWAP)
		return true;

	/* Fetching AMO with flush always requires a request struct since two
	 * operations are required to implement it.
	 */
	if (fetching_amo_flush)
		return true;

	return false;
}

/* TODO: Update HMEM buf type for 128-bit AMOs. */
static int cxip_amo_emit_dma(struct cxip_txc *txc,
			     enum cxip_amo_req_type req_type,
			     const struct fi_msg_atomic *msg, void *buf,
			     void *compare, void *result,
			     struct cxip_mr *buf_mr, struct cxip_mr *result_mr,
			     uint64_t key, uint64_t remote_offset,
			     union c_fab_addr *dfa, uint8_t *idx_ext,
			     uint16_t vni, enum c_atomic_op atomic_op,
			     enum c_cswap_op cswap_op,
			     enum c_atomic_type atomic_type,
			     unsigned int atomic_type_len, uint64_t flags,
			     uint32_t tclass, bool triggered,
			     uint64_t trig_thresh, struct cxip_cntr *trig_cntr,
			     struct cxip_cntr *comp_cntr)
{
	struct cxip_domain *dom = txc->domain;
	struct c_dma_amo_cmd dma_amo_cmd = {};
	bool flush = !!(flags & (FI_DELIVERY_COMPLETE | FI_MATCH_COMPLETE));
	bool fetching = result != NULL;
	bool fetching_amo_flush = fetching && flush;
	struct cxip_req *req;
	struct cxip_cntr *cntr;
	int ret;
	uint64_t hmem_buf;
	uint64_t hmem_compare;
	struct cxip_md *buf_md;
	struct cxip_md *result_md = NULL;
	void *selective_completion_req;

	/* MR desc cannot be value unless hybrid MR desc is enabled. */
	if (!dom->hybrid_mr_desc) {
		buf_mr = NULL;
		result_mr = NULL;
	}

	/* Since fetching AMO with flush results in two commands, if the
	 * target MR needs events, this would results in two remote MR counter
	 * increments. Thus, this functionality cannot be supported.
	 */
	if (fetching_amo_flush &&
	    cxip_generic_is_mr_key_events(txc->ep_obj->caps, key)) {
		TXC_WARN(txc,
			 "Fetching AMO with FI_DELIVERY_COMPLETE not supported with FI_RMA_EVENT\n");
		return -FI_EINVAL;
	}

	ofi_genlock_lock(&txc->ep_obj->lock);
	if (cxip_amo_emit_dma_req_needed(msg, flags, result, buf_mr, result_mr,
					 fetching_amo_flush)) {
		/* if (result && !result_mr) we end up in this branch */
		req = cxip_evtq_req_alloc(&txc->tx_evtq, 0, txc);
		if (!req) {
			ret = -FI_EAGAIN;
			TXC_WARN_RET(txc, ret, "Failed to allocate request\n");
			goto err;
		}

		/* Values set here are passed back to the user through the CQ */
		if (flags & FI_COMPLETION)
			req->context = (uint64_t)msg->context;
		else
			req->context = (uint64_t)txc->context;
		req->flags = FI_ATOMIC;
		req->flags |= (req_type == CXIP_RQ_AMO ? FI_WRITE : FI_READ);
		req->flags |= (flags & FI_COMPLETION);
		req->cb = _cxip_amo_cb;
		req->amo.txc = txc;
		req->amo.fetching_amo_flush = fetching_amo_flush;
		req->type = CXIP_REQ_AMO;
		req->trig_cntr = trig_cntr;

		/* Optionally register result MR. */
		if (result) {
			if (!result_mr) {
				ret = cxip_map(dom, result, atomic_type_len, 0,
					       &req->amo.result_md);
				if (ret) {
					TXC_WARN(txc,
						 "Failed to map result buffer: %d:%s\n",
						 ret, fi_strerror(-ret));
					goto err_free_req;
				}

				result_md = req->amo.result_md;
			} else {
				result_md = result_mr->md;
			}
		}

		if ((flags & FI_INJECT) || msg->op == FI_ATOMIC_READ ||
		    msg->op == FI_MSWAP) {
			/* To support FI_INJECT ot FI_ATOMIC_READ with matching
			 * AMO commands, an internal buffer is needed to store
			 * the payload.
			 */
			req->amo.ibuf = cxip_txc_ibuf_alloc(txc);
			if (!req->amo.ibuf) {
				ret = -FI_EAGAIN;
				TXC_WARN(txc,
					 "Failed to allocate ibuf: %d:%s\n",
					 ret, fi_strerror(-ret));
				goto err_unmap_result_buf;
			}

			switch (msg->op) {
			/* FI_ATOMIC_READ is implemented as a sum of zero. Thus,
			 * zero internal buffer which is used for the sum
			 * operand.
			 */
			case FI_ATOMIC_READ:
				memset(req->amo.ibuf, 0, atomic_type_len);
				break;

			case FI_MSWAP:
				ret = cxip_txc_copy_from_hmem(txc, NULL,
							      &hmem_buf, buf,
							      atomic_type_len);
				if (ret) {
					TXC_WARN(txc,
						 "cxip_txc_copy_from_hmem failed: %d:%s\n",
						 ret, fi_strerror(-ret));
					goto err_unmap_operand_buf;
				}

				ret = cxip_txc_copy_from_hmem(txc, NULL,
							      &hmem_compare,
							      compare,
							      atomic_type_len);
				if (ret) {
					TXC_WARN(txc,
						 "cxip_txc_copy_from_hmem failed: %d:%s\n",
						 ret, fi_strerror(-ret));
					goto err_unmap_operand_buf;
				}

				hmem_buf &= hmem_compare;

				memcpy(req->amo.ibuf, &hmem_buf,
				       atomic_type_len);
				break;

			/* Copy over user payload for FI_INJECT operation. */
			default:
				ret = cxip_txc_copy_from_hmem(txc, NULL,
							      req->amo.ibuf,
							      buf,
							      atomic_type_len);
				if (ret) {
					TXC_WARN(txc,
						 "cxip_txc_copy_from_hmem failed: %d:%s\n",
						 ret, fi_strerror(-ret));
					goto err_unmap_operand_buf;
				}
			}

			buf = req->amo.ibuf;
			buf_md = cxip_txc_ibuf_md(req->amo.ibuf);
		} else if (buf_mr) {
			buf_md = buf_mr->md;
		} else {
			/* Map user operand buffer for DMA command. */
			ret = cxip_map(dom, buf, atomic_type_len, 0,
				       &req->amo.oper1_md);
			if (ret) {
				TXC_WARN(txc,
					 "Failed to map operand buffer: %d:%s\n",
					 ret, fi_strerror(-ret));
				goto err_unmap_result_buf;
			}

			buf_md = req->amo.oper1_md;
		}
	} else {
		req = NULL;

		if (result)
			result_md = result_mr->md;

		buf_md = buf_mr->md;
	}

	/* Build up the matching AMO command. */
	dma_amo_cmd.dfa = *dfa;
	dma_amo_cmd.index_ext = *idx_ext;
	dma_amo_cmd.event_send_disable = 1;
	dma_amo_cmd.remote_offset = remote_offset;
	dma_amo_cmd.request_len = atomic_type_len;
	dma_amo_cmd.eq = cxip_evtq_eqn(&txc->tx_evtq);
	dma_amo_cmd.match_bits = CXIP_KEY_MATCH_BITS(key);
	dma_amo_cmd.atomic_op = atomic_op;
	dma_amo_cmd.atomic_type = atomic_type;
	dma_amo_cmd.cswap_op = cswap_op;
	dma_amo_cmd.local_read_addr = CXI_VA_TO_IOVA(buf_md->md, buf);
	dma_amo_cmd.lac = buf_md->md->lac;

	if (req) {
		dma_amo_cmd.user_ptr = (uint64_t)req;
	} else {
		if (req_type == CXIP_RQ_AMO)
			selective_completion_req =
				cxip_amo_selective_completion_req(txc);
		else
			selective_completion_req =
				cxip_amo_fetching_selective_completion_req(txc);

		if (!selective_completion_req) {
			ret = -FI_EAGAIN;
			TXC_WARN(txc,
				 "Failed to allocate selective completion request\n");
			goto err_unmap_operand_buf;
		}

		dma_amo_cmd.user_ptr = (uint64_t)selective_completion_req;
		dma_amo_cmd.event_success_disable = 1;
	}

	/* FI_ATOMIC_WRITE is implemented as a CSWAP NE operation. For this to
	 * work, the compare buffer (i.e. operand 2) needs to have the same
	 * contents as the write payload (i.e. operand 1).
	 */
	if (msg->op == FI_ATOMIC_WRITE) {
		assert(compare == NULL);

		ret = cxip_txc_copy_from_hmem(txc, NULL, &dma_amo_cmd.op2_word1,
					      buf, atomic_type_len);
		if (ret) {
			TXC_WARN(txc,
				 "cxip_txc_copy_from_hmem failed: %d:%s\n", ret,
				 fi_strerror(-ret));
			goto err_unmap_operand_buf;
		}
	} else if (compare) {
		/* Note: 16-byte value will overflow into op2_word2 */
		ret = cxip_txc_copy_from_hmem(txc, NULL, &dma_amo_cmd.op2_word1,
					      compare, atomic_type_len);
		if (ret) {
			TXC_WARN(txc,
				 "cxip_txc_copy_from_hmem failed: %d:%s\n", ret,
				 fi_strerror(-ret));
			goto err_unmap_operand_buf;
		}
	}

	/* if (result) {result_md is set} */
	if (result) {
		dma_amo_cmd.local_write_addr =
			CXI_VA_TO_IOVA(result_md->md, result);
		dma_amo_cmd.write_lac = result_md->md->lac;
	}

	/* Fetching AMO with flushes requires a trailing zero-byte put with
	 * Normal AMOs can use the operation flush functionality.
	 */
	if (!fetching_amo_flush) {
		dma_amo_cmd.flush = flush;

		if (req_type == CXIP_RQ_AMO) {
			cntr = triggered ? comp_cntr : txc->write_cntr;

			if (cntr) {
				dma_amo_cmd.event_ct_ack = 1;
				dma_amo_cmd.ct = cntr->ct->ctn;
			}
		} else {
			cntr = triggered ? comp_cntr : txc->read_cntr;

			if (cntr) {
				dma_amo_cmd.event_ct_reply = 1;
				dma_amo_cmd.ct = cntr->ct->ctn;
			}
		}
	}

	/* Optionally configure the flushing command used for fetching AMOs. */
	if (fetching_amo_flush) {
		assert(req != NULL);

		if (req_type == CXIP_RQ_AMO)
			req->amo.fetching_amo_flush_cntr = txc->write_cntr;
		else
			req->amo.fetching_amo_flush_cntr = txc->read_cntr;
	}

	ret = cxip_txc_emit_dma_amo(txc, vni, cxip_ofi_to_cxi_tc(tclass),
				    CXI_TC_TYPE_DEFAULT, trig_cntr, trig_thresh,
				    &dma_amo_cmd, flags, fetching, flush);
	if (ret) {
		TXC_WARN_RET(txc, ret, "Failed to emit AMO\n");
		goto err_unmap_operand_buf;
	}

	ofi_genlock_unlock(&txc->ep_obj->lock);

	return FI_SUCCESS;

err_unmap_operand_buf:
	if (req) {
		if (req->amo.ibuf)
			cxip_txc_ibuf_free(txc, req->amo.ibuf);
		else
			cxip_unmap(req->amo.oper1_md);
	}
err_unmap_result_buf:
	if (req && req->amo.result_md)
		cxip_unmap(req->amo.result_md);
err_free_req:
	if (req)
		cxip_evtq_req_free(req);
err:
	ofi_genlock_unlock(&txc->ep_obj->lock);

	TXC_WARN_RET(txc, ret,
		     "%s %s failed: atomic_op=%u cswap_op=%u atomic_type=%u buf=%p compare=%p result=%p len=%u rkey=%#lx roffset=%#lx nid=%#x ep=%u idx_ext=%u\n",
		     triggered ? "Triggered" : "DMA", fetching ? "FAMO" : "AMO",
		     atomic_op, cswap_op, atomic_type, buf, compare, result,
		     atomic_type_len, key, remote_offset, dfa->unicast.nid,
		     dfa->unicast.endpoint_defined, *idx_ext);

	return ret;
}

static bool cxip_amo_is_idc(struct cxip_txc *txc, uint64_t key, bool triggered)
{
	/* Triggered AMOs can never be IDCs. */
	if (triggered)
		return false;

	/* Only optimized MR can be used for IDCs. */
	return cxip_generic_is_mr_key_opt(key);
}

int cxip_amo_common(enum cxip_amo_req_type req_type, struct cxip_txc *txc,
		    uint32_t tclass, const struct fi_msg_atomic *msg,
		    const struct fi_ioc *comparev, void **comparedesc,
		    size_t compare_count, const struct fi_ioc *resultv,
		    void **resultdesc, size_t result_count, uint64_t flags,
		    bool triggered, uint64_t trig_thresh,
		    struct cxip_cntr *trig_cntr, struct cxip_cntr *comp_cntr)
{
	void *buf;
	void *compare = NULL;
	void *result = NULL;
	uint64_t remote_offset;
	uint64_t key;
	bool idc;
	enum c_atomic_op atomic_op;
	enum c_cswap_op cswap_op;
	enum c_atomic_type atomic_type;
	unsigned int atomic_type_len;
	struct cxip_addr caddr;
	int ret;
	union c_fab_addr dfa;
	uint8_t idx_ext;
	uint32_t pid_idx;
	struct cxip_mr *buf_mr = NULL;
	struct cxip_mr *result_mr = NULL;
	uint16_t vni;

	if (!msg) {
		TXC_WARN(txc, "NULL fi_msg_atomic");
		return -FI_EINVAL;
	}

	switch (req_type) {
	case CXIP_RQ_AMO_SWAP:
		/* Must have a valid compare address */
		if (!_vector_valid(compare_count, comparev)) {
			TXC_WARN(txc, "compare IOV invalid\n");
			return -FI_EINVAL;
		}

		compare = comparev[0].addr;

		/* FALLTHRU */
	case CXIP_RQ_AMO_FETCH:
	case CXIP_RQ_AMO_PCIE_FETCH:
		/* Must have a valid result address */
		if (!_vector_valid(result_count, resultv)) {
			TXC_WARN(txc, "result IOV invalid\n");
			return -FI_EINVAL;
		}

		result = resultv[0].addr;
		if (resultdesc && resultdesc[0])
			result_mr = resultdesc[0];

		/* FALLTHRU */
	case CXIP_RQ_AMO:
		if (msg->op != FI_ATOMIC_READ) {
			if (!_vector_valid(msg->iov_count, msg->msg_iov)) {
				TXC_WARN(txc, "msg IOV invalid\n");
				return -FI_EINVAL;
			}
			buf = msg->msg_iov[0].addr;
			if (msg->desc && msg->desc[0])
				buf_mr = msg->desc[0];
		} else {
			buf = NULL;
			buf_mr = NULL;
		}

		/* The supplied RMA address is actually an offset into a
		 * registered MR. A value of 0 is valid.
		 */
		if (!_rma_vector_valid(msg->rma_iov_count, msg->rma_iov)) {
			TXC_WARN(txc, "RMA IOV invalid\n");
			return -FI_EINVAL;
		}

		remote_offset = msg->rma_iov[0].addr;
		key = msg->rma_iov[0].key;

		ret = cxip_adjust_remote_offset(&remote_offset, key);
		if (ret) {
			TXC_WARN(txc, "RMA IOV address overflow\n");
			return -FI_EINVAL;
		}
		break;

	default:
		TXC_WARN(txc, "Invalid AMO request type: %d\n", req_type);
		return -FI_EINVAL;
	}

	if (!cxip_generic_is_valid_mr_key(key)) {
		TXC_WARN(txc, "Invalid remote key: 0x%lx\n", key);
		return -FI_EKEYREJECTED;
	}

	idc = cxip_amo_is_idc(txc, key, triggered);

	/* Convert FI to CXI codes, fail if operation not supported */
	ret = _cxip_atomic_opcode(req_type, msg->datatype, msg->op,
				  txc->domain->amo_remap_to_pcie_fadd,
				  &atomic_op, &atomic_type, &cswap_op,
				  &atomic_type_len);
	if (ret < 0) {
		TXC_WARN_RET(txc, ret, "Failed to generate CXI AMO opcodes\n");
		return ret;
	}

	/* Look up target CXI address */
	ret = cxip_av_lookup_addr(txc->ep_obj->av, msg->addr, &caddr);
	if (ret != FI_SUCCESS) {
		TXC_WARN_RET(txc, ret, "Failed to look up dst FI addr\n");
		return ret;
	}

	if (txc->ep_obj->av_auth_key)
		vni = caddr.vni;
	else
		vni = txc->ep_obj->auth_key.vni;

	pid_idx = cxip_generic_mr_key_to_ptl_idx(txc->domain, key, !result);
	cxi_build_dfa(caddr.nic, caddr.pid, txc->pid_bits, pid_idx, &dfa,
		      &idx_ext);
	if (idc)
		ret = cxip_amo_emit_idc(txc, req_type, msg, buf, compare,
					result, result_mr, key, remote_offset,
					&dfa, &idx_ext, vni, atomic_op,
					cswap_op, atomic_type, atomic_type_len,
					flags, tclass);
	else
		ret = cxip_amo_emit_dma(txc, req_type, msg, buf, compare,
					result, buf_mr, result_mr, key,
					remote_offset, &dfa, &idx_ext, vni,
					atomic_op, cswap_op, atomic_type,
					atomic_type_len, flags, tclass,
					triggered, trig_thresh, trig_cntr,
					comp_cntr);
	if (ret)
		TXC_WARN_RET(txc, ret,
			     "%s AMO failed: op=%u buf=%p compare=%p result=%p len=%u rkey=%#lx roffset=%#lx nic=%#x pid=%u pid_idx=%u triggered=%u",
			     idc ? "IDC" : "DMA", msg->op, buf, compare, result,
			     atomic_type_len, key, remote_offset, caddr.nic,
			     caddr.pid, pid_idx, triggered);
	else
		TXC_DBG(txc,
			"%s AMO emitted: op=%u buf=%p compare=%p result=%p len=%u rkey=%#lx roffset=%#lx nic=%#x pid=%u pid_idx=%u triggered=%u",
			idc ? "IDC" : "DMA", msg->op, buf, compare, result,
			atomic_type_len, key, remote_offset, caddr.nic,
			caddr.pid, pid_idx, triggered);

	return ret;
}

/*
 * Libfabric APIs
 */
static ssize_t cxip_ep_atomic_write(struct fid_ep *fid_ep, const void *buf,
				    size_t count, void *desc,
				    fi_addr_t dest_addr, uint64_t addr,
				    uint64_t key, enum fi_datatype datatype,
				    enum fi_op op, void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct fi_ioc oper1 = {
		.addr = (void *)buf,
		.count = count
	};
	struct fi_rma_ioc rma = {
		.addr = addr,
		.count = 1,
		.key = key
	};
	struct fi_msg_atomic msg = {
		.msg_iov = &oper1,
		.desc = &desc,
		.iov_count = 1,
		.addr = dest_addr,
		.rma_iov = &rma,
		.rma_iov_count = 1,
		.datatype = datatype,
		.op = op,
		.context = context
	};

	return cxip_amo_common(CXIP_RQ_AMO, ep->ep_obj->txc,
			       ep->tx_attr.tclass, &msg, NULL, NULL, 0,
			       NULL, NULL, 0, ep->tx_attr.op_flags, false,
			       0, NULL, NULL);
}

static ssize_t cxip_ep_atomic_writev(struct fid_ep *fid_ep,
				     const struct fi_ioc *iov, void **desc,
				     size_t count, fi_addr_t dest_addr,
				     uint64_t addr, uint64_t key,
				     enum fi_datatype datatype, enum fi_op op,
				     void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct fi_rma_ioc rma = {
		.addr = addr,
		.count = 1,
		.key = key
	};
	struct fi_msg_atomic msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.addr = dest_addr,
		.rma_iov = &rma,
		.rma_iov_count = 1,
		.datatype = datatype,
		.op = op,
		.context = context
	};

	return cxip_amo_common(CXIP_RQ_AMO, ep->ep_obj->txc,
			       ep->tx_attr.tclass, &msg, NULL, NULL, 0, NULL,
			       NULL, 0, ep->tx_attr.op_flags, false, 0,
			       NULL, NULL);
}

static ssize_t cxip_ep_atomic_writemsg(struct fid_ep *fid_ep,
				       const struct fi_msg_atomic *msg,
				       uint64_t flags)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;

	if (flags & ~(CXIP_WRITEMSG_ALLOWED_FLAGS |
		      FI_CXI_UNRELIABLE |
		      FI_CXI_HRP | FI_CXI_WEAK_FENCE))
		return -FI_EBADFLAGS;

	if (flags & FI_FENCE && !(txc->attr.caps & FI_FENCE))
		return -FI_EINVAL;

	/* If selective completion is not requested, always generate
	 * completions.
	 */
	if (!txc->selective_completion)
		flags |= FI_COMPLETION;

	return cxip_amo_common(CXIP_RQ_AMO, txc, ep->tx_attr.tclass, msg,
			       NULL, NULL, 0, NULL, NULL, 0, flags, false, 0,
			       NULL, NULL);
}

static ssize_t cxip_ep_atomic_inject(struct fid_ep *fid_ep, const void *buf,
				     size_t count, fi_addr_t dest_addr,
				     uint64_t addr, uint64_t key,
				     enum fi_datatype datatype, enum fi_op op)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct fi_ioc oper1 = {
		.addr = (void *)buf,
		.count = count
	};
	struct fi_rma_ioc rma = {
		.addr = addr,
		.count = 1,
		.key = key
	};
	struct fi_msg_atomic msg = {
		.msg_iov = &oper1,
		.desc = NULL,
		.iov_count = 1,
		.addr = dest_addr,
		.rma_iov = &rma,
		.rma_iov_count = 1,
		.datatype = datatype,
		.op = op,
		.context = NULL
	};

	return cxip_amo_common(CXIP_RQ_AMO, ep->ep_obj->txc,
			       ep->tx_attr.tclass, &msg, NULL, NULL, 0, NULL,
			       NULL, 0, FI_INJECT, false, 0, NULL, NULL);
}

static ssize_t cxip_ep_atomic_readwrite(struct fid_ep *fid_ep, const void *buf,
					size_t count, void *desc, void *result,
					void *result_desc, fi_addr_t dest_addr,
					uint64_t addr, uint64_t key,
					enum fi_datatype datatype,
					enum fi_op op, void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct fi_ioc oper1 = {
		.addr = (void *)buf,
		.count = count
	};
	struct fi_ioc resultv = {
		.addr = result,
		.count = count
	};
	struct fi_rma_ioc rma = {
		.addr = addr,
		.count = 1,
		.key = key
	};
	struct fi_msg_atomic msg = {
		.msg_iov = &oper1,
		.desc = &desc,
		.iov_count = 1,
		.addr = dest_addr,
		.rma_iov = &rma,
		.rma_iov_count = 1,
		.datatype = datatype,
		.op = op,
		.context = context
	};

	return cxip_amo_common(CXIP_RQ_AMO_FETCH, ep->ep_obj->txc,
			       ep->tx_attr.tclass, &msg, NULL, NULL, 0,
			       &resultv, &result_desc, 1, ep->tx_attr.op_flags,
			       false, 0, NULL, NULL);
}

static ssize_t cxip_ep_atomic_readwritev(struct fid_ep *fid_ep,
					 const struct fi_ioc *iov,
					 void **desc, size_t count,
					 struct fi_ioc *resultv,
					 void **result_desc,
					 size_t result_count,
					 fi_addr_t dest_addr, uint64_t addr,
					 uint64_t key,
					 enum fi_datatype datatype,
					 enum fi_op op, void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct fi_rma_ioc rma = {
		.addr = addr,
		.count = 1,
		.key = key
	};
	struct fi_msg_atomic msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.addr = dest_addr,
		.rma_iov = &rma,
		.rma_iov_count = 1,
		.datatype = datatype,
		.op = op,
		.context = context
	};

	return cxip_amo_common(CXIP_RQ_AMO_FETCH, ep->ep_obj->txc,
			       ep->tx_attr.tclass, &msg, NULL, NULL, 0, resultv,
			       result_desc, result_count, ep->tx_attr.op_flags,
			       false, 0, NULL, NULL);
}

static ssize_t cxip_ep_atomic_readwritemsg(struct fid_ep *fid_ep,
					   const struct fi_msg_atomic *msg,
					   struct fi_ioc *resultv,
					   void **result_desc,
					   size_t result_count, uint64_t flags)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;
	enum cxip_amo_req_type req_type;

	if (flags & ~(CXIP_WRITEMSG_ALLOWED_FLAGS |
		      FI_CXI_UNRELIABLE | FI_CXI_WEAK_FENCE |
		      FI_CXI_PCIE_AMO))
		return -FI_EBADFLAGS;

	if (flags & FI_FENCE && !(txc->attr.caps & FI_FENCE))
		return -FI_EINVAL;

	/* If selective completion is not requested, always generate
	 * completions.
	 */
	if (!txc->selective_completion)
		flags |= FI_COMPLETION;

	if (flags & FI_CXI_PCIE_AMO)
		req_type = CXIP_RQ_AMO_PCIE_FETCH;
	else
		req_type = CXIP_RQ_AMO_FETCH;

	return cxip_amo_common(req_type, txc, ep->tx_attr.tclass, msg, NULL,
			       NULL, 0, resultv, result_desc, result_count,
			       flags, false, 0, NULL, NULL);
}

static ssize_t cxip_ep_atomic_compwrite(struct fid_ep *fid_ep, const void *buf,
					size_t count, void *desc,
					const void *compare, void *compare_desc,
					void *result, void *result_desc,
					fi_addr_t dest_addr, uint64_t addr,
					uint64_t key, enum fi_datatype datatype,
					enum fi_op op, void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct fi_ioc oper1 = {
		.addr = (void *)buf,
		.count = count
	};
	struct fi_ioc comparev = {
		.addr = (void *)compare,
		.count = count
	};
	struct fi_ioc resultv = {
		.addr = result,
		.count = count
	};
	struct fi_rma_ioc rma = {
		.addr = addr,
		.count = 1,
		.key = key
	};
	struct fi_msg_atomic msg = {
		.msg_iov = &oper1,
		.desc = &desc,
		.iov_count = 1,
		.addr = dest_addr,
		.rma_iov = &rma,
		.rma_iov_count = 1,
		.datatype = datatype,
		.op = op,
		.context = context
	};

	return cxip_amo_common(CXIP_RQ_AMO_SWAP, ep->ep_obj->txc,
			       ep->tx_attr.tclass, &msg, &comparev,
			       &result_desc, 1, &resultv, &result_desc, 1,
			       ep->tx_attr.op_flags, false, 0, NULL, NULL);
}

static ssize_t cxip_ep_atomic_compwritev(struct fid_ep *fid_ep,
					 const struct fi_ioc *iov, void **desc,
					 size_t count,
					 const struct fi_ioc *comparev,
					 void **compare_desc,
					 size_t compare_count,
					 struct fi_ioc *resultv,
					 void **result_desc,
					 size_t result_count,
					 fi_addr_t dest_addr, uint64_t addr,
					 uint64_t key,
					 enum fi_datatype datatype,
					 enum fi_op op, void *context)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct fi_rma_ioc rma = {
		.addr = addr,
		.count = 1,
		.key = key
	};
	struct fi_msg_atomic msg = {
		.msg_iov = iov,
		.desc = desc,
		.iov_count = count,
		.addr = dest_addr,
		.rma_iov = &rma,
		.rma_iov_count = 1,
		.datatype = datatype,
		.op = op,
		.context = context
	};

	return cxip_amo_common(CXIP_RQ_AMO_SWAP, ep->ep_obj->txc,
			       ep->tx_attr.tclass, &msg, comparev, compare_desc,
			       compare_count, resultv, result_desc,
			       result_count, ep->tx_attr.op_flags, false, 0,
			       NULL, NULL);
}

static ssize_t
cxip_ep_atomic_compwritemsg(struct fid_ep *fid_ep,
			    const struct fi_msg_atomic *msg,
			    const struct fi_ioc *comparev, void **compare_desc,
			    size_t compare_count, struct fi_ioc *resultv,
			    void **result_desc, size_t result_count,
			    uint64_t flags)
{
	struct cxip_ep *ep = container_of(fid_ep, struct cxip_ep, ep);
	struct cxip_txc *txc = ep->ep_obj->txc;

	if (flags & ~(CXIP_WRITEMSG_ALLOWED_FLAGS |
		      FI_CXI_UNRELIABLE | FI_CXI_WEAK_FENCE))
		return -FI_EBADFLAGS;

	if (flags & FI_FENCE && !(txc->attr.caps & FI_FENCE))
		return -FI_EINVAL;

	/* If selective completion is not requested, always generate
	 * completions.
	 */
	if (!txc->selective_completion)
		flags |= FI_COMPLETION;

	return cxip_amo_common(CXIP_RQ_AMO_SWAP, txc, ep->tx_attr.tclass, msg,
			       comparev, compare_desc, compare_count, resultv,
			       result_desc, result_count, flags, false, 0,
			       NULL, NULL);
}

static int cxip_ep_atomic_valid(struct fid_ep *ep,
				enum fi_datatype datatype,
				enum fi_op op,
				size_t *count)
{
	return _cxip_ep_valid(ep, CXIP_RQ_AMO, datatype, op, count);
}

static int cxip_ep_fetch_atomic_valid(struct fid_ep *ep,
				      enum fi_datatype datatype, enum fi_op op,
				      size_t *count)
{
	return _cxip_ep_valid(ep, CXIP_RQ_AMO_FETCH, datatype, op, count);
}

static int cxip_ep_comp_atomic_valid(struct fid_ep *ep,
				     enum fi_datatype datatype,
				     enum fi_op op, size_t *count)
{
	return _cxip_ep_valid(ep, CXIP_RQ_AMO_SWAP, datatype, op, count);
}

struct fi_ops_atomic cxip_ep_atomic_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = cxip_ep_atomic_write,
	.writev = cxip_ep_atomic_writev,
	.writemsg = cxip_ep_atomic_writemsg,
	.inject = cxip_ep_atomic_inject,
	.readwrite = cxip_ep_atomic_readwrite,
	.readwritev = cxip_ep_atomic_readwritev,
	.readwritemsg = cxip_ep_atomic_readwritemsg,
	.compwrite = cxip_ep_atomic_compwrite,
	.compwritev = cxip_ep_atomic_compwritev,
	.compwritemsg = cxip_ep_atomic_compwritemsg,
	.writevalid = cxip_ep_atomic_valid,
	.readwritevalid = cxip_ep_fetch_atomic_valid,
	.compwritevalid = cxip_ep_comp_atomic_valid,
};

struct fi_ops_atomic cxip_ep_atomic_no_ops = {
	.size = sizeof(struct fi_ops_atomic),
	.write = fi_no_atomic_write,
	.writev = fi_no_atomic_writev,
	.writemsg = fi_no_atomic_writemsg,
	.inject = fi_no_atomic_inject,
	.readwrite = fi_no_atomic_readwrite,
	.readwritev = fi_no_atomic_readwritev,
	.readwritemsg = fi_no_atomic_readwritemsg,
	.compwrite = fi_no_atomic_compwrite,
	.compwritev = fi_no_atomic_compwritev,
	.compwritemsg = fi_no_atomic_compwritemsg,
	.writevalid = fi_no_atomic_writevalid,
	.readwritevalid = fi_no_atomic_readwritevalid,
	.compwritevalid = fi_no_atomic_compwritevalid,
};

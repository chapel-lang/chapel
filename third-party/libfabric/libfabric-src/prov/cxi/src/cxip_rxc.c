/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2014 Intel Corporation. All rights reserved.
 * Copyright (c) 2019,2020-2023 Hewlett Packard Enterprise Development LP
 */

/* CXI RX Context Management */

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_INFO(...) _CXIP_INFO(FI_LOG_EP_CTRL, __VA_ARGS__)

extern struct cxip_rxc_ops hpc_rxc_ops;
extern struct cxip_rxc_ops rnr_rxc_ops;

/*
 * cxip_rxc_msg_enable() - Enable RXC messaging.
 *
 * Change the RXC RX PtlTE to enabled state. Once in enabled state, messages
 * will be accepted by hardware. Prepare all messaging resources before
 * enabling the RX PtlTE.
 *
 * Caller must hold ep_obj->lock.
 */
int cxip_rxc_msg_enable(struct cxip_rxc_hpc *rxc, uint32_t drop_count)
{
	int ret;

	/* If transitioning from disabled to the software managed state a
	 * synchronous call is used which handles drop count mismatches.
	 */
	if (rxc->new_state == RXC_ENABLED_SOFTWARE) {
		ret = cxil_pte_transition_sm(rxc->base.rx_pte->pte, drop_count);
		if (ret)
			RXC_WARN(rxc,
				 "Error transitioning to SW EP %d %s\n",
				 ret, fi_strerror(-ret));
		return ret;
	}

	return cxip_pte_set_state(rxc->base.rx_pte, rxc->base.rx_cmdq,
				  C_PTLTE_ENABLED, drop_count);
}

/*
 * rxc_msg_disable() - Disable RXC messaging.
 *
 * Change the RXC RX PtlTE to disabled state. Once in disabled state, the PtlTE
 * will receive no additional events.
 *
 * Caller must hold rxc->ep_obj->lock.
 */
static int rxc_msg_disable(struct cxip_rxc *rxc)
{
	int ret;

	if (rxc->state != RXC_ENABLED &&
	    rxc->state != RXC_ENABLED_SOFTWARE)
		RXC_FATAL(rxc, "RXC in bad state to be disabled: state=%d\n",
			  rxc->state);

	rxc->state = RXC_DISABLED;

	ret = cxip_pte_set_state_wait(rxc->rx_pte, rxc->rx_cmdq, &rxc->rx_evtq,
				      C_PTLTE_DISABLED, 0);
	if (ret == FI_SUCCESS)
		CXIP_DBG("RXC PtlTE disabled: %p\n", rxc);

	return ret;
}

static size_t cxip_rxc_get_num_events(struct cxip_rxc *rxc)
{
	size_t num_events;

	/* Hardware will ensure incoming RDMA operations have event queue space.
	 * It is the responsibility of software to ensure that any SW initiated
	 * target commands which may generate an event (e.g. append with failure
	 * or search) have enough space in the EQ. This can be done in two ways.
	 *
	 * 1. Continually increase EQ buffer size until EQ overflows go away.
	 * This option is not ideal since many application variables are in play
	 * which impact number of events needed.
	 *
	 * 2. Use hybrid endpoint mode to preemptively transition to software
	 * endpoint when event queue space may be under pressure. When in
	 * software endpoint mode, software should not be issuing commands, like
	 * append and search/search & delete, which could result in events being
	 * generated.
	 *
	 * For both cases, RXC size will be used to size number of events. To
	 * accommodate a stream of unexpected puts and append failures, RXC size
	 * is added again. With correct credit control for hybrid endpoint to
	 * preemptively transition to software endpoint, 2* RXC size should be
	 * enough to prevent EQ overflow. For all other cases, EQ size needs to
	 * be increased.
	 */

	num_events = rxc->attr.size * 2;

	/* Add 1 more event for software initiated state change. */
	num_events++;

	return num_events;
}

/*
 * rxc_msg_init() - Initialize an RX context for messaging.
 *
 * Allocates and initializes hardware resources used for receiving expected and
 * unexpected message data.
 *
 * Caller must hold ep_obj->lock.
 */
static int rxc_msg_init(struct cxip_rxc *rxc)
{
	size_t num_events;
	int ret;

	/* Base message initialization */
	num_events = cxip_rxc_get_num_events(rxc);
	ret = cxip_evtq_init(&rxc->rx_evtq, rxc->recv_cq, num_events, 1);
	if (ret) {
		CXIP_WARN("Failed to initialize RXC event queue: %d, %s\n",
			  ret, fi_strerror(-ret));
		return ret;
	}

	ret = cxip_ep_cmdq(rxc->ep_obj, false, FI_TC_UNSPEC, rxc->rx_evtq.eq,
			   &rxc->rx_cmdq);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Unable to allocate RX CMDQ, ret: %d\n", ret);
		goto free_evtq;
	}

	/* Derived messaging initialization/overrides */
	ret = rxc->ops.msg_init(rxc);
	if (ret) {
		CXIP_WARN("RXC derived initialization failed %d\n", ret);
		goto put_rx_cmdq;
	}

	return FI_SUCCESS;

put_rx_cmdq:
	cxip_ep_cmdq_put(rxc->ep_obj, false);
free_evtq:
	cxip_evtq_fini(&rxc->rx_evtq);

	return ret;
}

/*
 * rxc_msg_fini() - Finalize RX context messaging.
 *
 * Free hardware resources allocated when the RX context was initialized for
 * messaging.
 *
 * Caller must hold ep_obj->lock.
 */
static int rxc_msg_fini(struct cxip_rxc *rxc)
{
	int ret;

	ret = rxc->ops.msg_fini(rxc);
	if (ret)
		return ret;

	cxip_pte_free(rxc->rx_pte);
	cxip_ep_cmdq_put(rxc->ep_obj, false);
	cxip_evtq_fini(&rxc->rx_evtq);

	return FI_SUCCESS;
}

/*
 * cxip_rxc_enable() - Enable an RX context for use.
 *
 * Called via fi_enable(). The context could be used in a standard endpoint or
 * a scalable endpoint.
 */
int cxip_rxc_enable(struct cxip_rxc *rxc)
{
	int ret;

	if (rxc->state != RXC_DISABLED)
		return FI_SUCCESS;

	if (!ofi_recv_allowed(rxc->attr.caps)) {
		rxc->state = RXC_ENABLED;
		return FI_SUCCESS;
	}

	if (!rxc->recv_cq) {
		CXIP_WARN("Undefined recv CQ\n");
		return -FI_ENOCQ;
	}

	ret = rxc_msg_init(rxc);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("rxc_msg_init returned: %d\n", ret);
		return -FI_EDOMAIN;
	}

	return FI_SUCCESS;
}

/*
 * cxip_rxc_recv_req_cleanup() - Attempt to free outstanding requests.
 *
 * Outstanding commands may be dropped when the RX Command Queue is freed.
 * This leads to missing events. Attempt to gather all events before freeing
 * the RX CQ. If events go missing, resources will be leaked until the
 * Completion Queue is freed.
 */
void cxip_rxc_recv_req_cleanup(struct cxip_rxc *rxc)
{
	int ret;
	uint64_t start;
	int canceled = 0;

	if (!ofi_atomic_get32(&rxc->orx_reqs))
		return;

	cxip_evtq_req_discard(&rxc->rx_evtq, rxc);

	do {
		ret = cxip_evtq_req_cancel(&rxc->rx_evtq, rxc, 0, false);
		if (ret == FI_SUCCESS)
			canceled++;
	} while (ret == FI_SUCCESS);

	if (canceled)
		CXIP_DBG("Canceled %d Receives: %p\n", canceled, rxc);

	start = ofi_gettime_ms();
	while (ofi_atomic_get32(&rxc->orx_reqs)) {
		sched_yield();
		cxip_evtq_progress(&rxc->rx_evtq);

		if (ofi_gettime_ms() - start > CXIP_REQ_CLEANUP_TO) {
			CXIP_WARN("Timeout waiting for outstanding requests.\n");
			break;
		}
	}
}

static void cxip_rxc_dump_counters(struct cxip_rxc *rxc)
{
	int i;
	int j;
	int k;
	size_t msg_size;
	bool print_header;
	int count;

	for (i = 0; i < CXIP_LIST_COUNTS; i++) {
		for (j = 0; j < OFI_HMEM_MAX; j++) {

			print_header = true;

			for (k = 0; k < CXIP_COUNTER_BUCKETS; k++) {
				if (k == 0)
					msg_size = 0;
				else
					msg_size = (1ULL << (k - 1));

				count = ofi_atomic_get32(&rxc->cntrs.msg_count[i][j][k]);
				if (count) {
					if (print_header) {
						RXC_INFO(rxc, "Recv Message Size %s - %s Histogram\n",
							 c_ptl_list_strs[i],
							 fi_tostr(&j, FI_TYPE_HMEM_IFACE));
						RXC_INFO(rxc, "%-14s Count\n", "Size");
						print_header = false;
					}

					RXC_INFO(rxc, "%-14lu %u\n", msg_size,
						 count);
				}
			}
		}

	}
}

/*
 * cxip_rxc_disable() - Disable the RX context of an base endpoint object.
 *
 * Free hardware resources allocated when the context was enabled. Called via
 * fi_close().
 */
void cxip_rxc_disable(struct cxip_rxc *rxc)
{
	int ret;

	cxip_rxc_dump_counters(rxc);

	if (rxc->state == RXC_DISABLED)
		return;

	if (ofi_recv_allowed(rxc->attr.caps)) {
		/* Stop accepting Puts. */
		ret = rxc_msg_disable(rxc);
		if (ret != FI_SUCCESS)
			CXIP_WARN("rxc_msg_disable returned: %d\n", ret);

		/* Protocol cleanup must call cxip_rxc_recv_req_cleanup() */
		rxc->ops.cleanup(rxc);

		/* Free hardware resources. */
		ret = rxc_msg_fini(rxc);
		if (ret != FI_SUCCESS)
			CXIP_WARN("rxc_msg_fini returned: %d\n", ret);
	}
}

int cxip_rxc_emit_dma(struct cxip_rxc_hpc *rxc, uint16_t vni,
		      enum cxi_traffic_class tc,
		      enum cxi_traffic_class_type tc_type,
		      struct c_full_dma_cmd *dma, uint64_t flags)
{
	int ret;

	if (rxc->base.ep_obj->av_auth_key) {
		ret = cxip_domain_emit_dma(rxc->base.domain, vni, tc,
					   dma, flags);
		if (ret)
			RXC_WARN(rxc, "Failed to emit domain dma command: %d\n",
				 ret);
		return ret;
	}

	/* Ensure correct traffic class is used. */
	ret = cxip_cmdq_cp_set(rxc->tx_cmdq, vni, tc, tc_type);
	if (ret) {
		RXC_WARN(rxc, "Failed to set traffic class: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	ret = cxip_cmdq_emit_dma(rxc->tx_cmdq, dma, flags);
	if (ret) {
		RXC_WARN(rxc, "Failed to emit dma command: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	cxip_txq_ring(rxc->tx_cmdq, 0, 1);

	return FI_SUCCESS;
}

int cxip_rxc_emit_idc_msg(struct cxip_rxc_hpc *rxc, uint16_t vni,
			  enum cxi_traffic_class tc,
			  enum cxi_traffic_class_type tc_type,
			  const struct c_cstate_cmd *c_state,
			  const struct c_idc_msg_hdr *msg, const void *buf,
			  size_t len, uint64_t flags)
{
	int ret;

	if (rxc->base.ep_obj->av_auth_key) {
		ret = cxip_domain_emit_idc_msg(rxc->base.domain, vni, tc,
					       c_state, msg, buf, len, flags);
		if (ret)
			RXC_WARN(rxc, "Failed to emit domain idc msg: %d\n",
				 ret);
		return ret;
	}

	/* Ensure correct traffic class is used. */
	ret = cxip_cmdq_cp_set(rxc->tx_cmdq, vni, tc, tc_type);
	if (ret) {
		RXC_WARN(rxc, "Failed to set traffic class: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	ret = cxip_cmdq_emit_idc_msg(rxc->tx_cmdq, c_state, msg, buf, len,
				     flags);
	if (ret) {
		RXC_WARN(rxc, "Failed to emit idc_msg command: %d:%s\n", ret,
			 fi_strerror(-ret));
		return ret;
	}

	cxip_txq_ring(rxc->tx_cmdq, 0, 1);

	return FI_SUCCESS;
}

struct cxip_rxc *cxip_rxc_calloc(struct cxip_ep_obj *ep_obj, void *context)
{
	struct cxip_rxc *rxc = NULL;

	switch (ep_obj->protocol) {
	case FI_PROTO_CXI:
		rxc = calloc(1, sizeof(struct cxip_rxc_hpc));
		if (rxc)
			rxc->ops = hpc_rxc_ops;
		break;
	case FI_PROTO_CXI_RNR:
		rxc = calloc(1, sizeof(struct cxip_rxc_rnr));
		if (rxc)
			rxc->ops = rnr_rxc_ops;
		break;
	default:
		CXIP_WARN("Unsupported EP protocol requested %d\n",
			  ep_obj->protocol);
		return NULL;
	}

	if (!rxc) {
		CXIP_WARN("Memory allocation failure\n");
		return NULL;
	}

	/* Base initialization */
	rxc->protocol = ep_obj->protocol;
	rxc->context = context;
	rxc->ep_obj = ep_obj;
	rxc->domain = ep_obj->domain;
	rxc->min_multi_recv = CXIP_EP_MIN_MULTI_RECV;
	rxc->state = RXC_DISABLED;
	rxc->msg_offload = cxip_env.msg_offload;
	rxc->max_tx = cxip_env.sw_rx_tx_init_max;
	rxc->attr = ep_obj->rx_attr;
	rxc->hmem = !!(rxc->attr.caps & FI_HMEM);
	rxc->pid_bits = ep_obj->domain->iface->dev->info.pid_bits;
	ofi_atomic_initialize32(&rxc->orx_reqs, 0);

	rxc->sw_ep_only = cxip_env.rx_match_mode ==
					CXIP_PTLTE_SOFTWARE_MODE;
	cxip_msg_counters_init(&rxc->cntrs);

	/* Derived initialization/overrides */
	rxc->ops.init_struct(rxc, ep_obj);

	return rxc;
}

void cxip_rxc_free(struct cxip_rxc *rxc)
{
	if (!rxc)
		return;

	/* Derived structure free */
	rxc->ops.fini_struct(rxc);

	/* Any base stuff */

	free(rxc);
}

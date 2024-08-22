/*
 * Copyright (c) 2023 Hewlett Packard Enterprise Development LP
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 */
#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_DOMAIN, __VA_ARGS__)
#define CXIP_INFO(...) _CXIP_INFO(FI_LOG_DOMAIN, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_DOMAIN, __VA_ARGS__)

enum cxi_traffic_class cxip_ofi_to_cxi_tc(uint32_t ofi_tclass)
{
	switch (ofi_tclass) {
	case FI_TC_BULK_DATA:
		return CXI_TC_BULK_DATA;
	case FI_TC_DEDICATED_ACCESS:
		return CXI_TC_DEDICATED_ACCESS;
	case FI_TC_LOW_LATENCY:
		return CXI_TC_LOW_LATENCY;
	case FI_TC_BEST_EFFORT:
	case FI_TC_NETWORK_CTRL:
	case FI_TC_SCAVENGER:
	default:
		return CXI_TC_BEST_EFFORT;
	}
}

static int cxip_cp_get(struct cxip_lni *lni, uint16_t vni,
		       enum cxi_traffic_class tc,
		       enum cxi_traffic_class_type tc_type,
		       struct cxi_cp **cp)
{
	int ret;
	int i;
	struct cxip_remap_cp *sw_cp;
	static const enum cxi_traffic_class remap_tc = CXI_TC_BEST_EFFORT;

	ofi_spin_lock(&lni->lock);

	/* Always prefer SW remapped CPs over allocating HW CP. */
	dlist_foreach_container(&lni->remap_cps, struct cxip_remap_cp, sw_cp,
				remap_entry) {
		if (sw_cp->remap_cp.vni == vni && sw_cp->remap_cp.tc == tc &&
		    sw_cp->remap_cp.tc_type == tc_type) {
			CXIP_DBG("Reusing SW CP: %u VNI: %u TC: %s TYPE: %s\n",
				 sw_cp->remap_cp.lcid, sw_cp->remap_cp.vni,
				 cxi_tc_to_str(sw_cp->remap_cp.tc),
				 cxi_tc_type_to_str(sw_cp->remap_cp.tc_type));
			*cp = &sw_cp->remap_cp;
			goto success_unlock;
		}
	}

	/* Allocate a new SW remapped CP entry and attempt to allocate the
	 * user requested HW CP.
	 */
	sw_cp = calloc(1, sizeof(*sw_cp));
	if (!sw_cp) {
		ret = -FI_ENOMEM;
		goto err_unlock;
	}

	ret = cxil_alloc_cp(lni->lni, vni, tc, tc_type,
			    &lni->hw_cps[lni->n_cps]);
	if (ret) {
		/* Attempt to fall back to remap traffic class with the same
		 * traffic class type and allocate HW CP if necessary.
		 */
		CXIP_WARN("Failed to allocate CP, ret: %d VNI: %u TC: %s TYPE: %s\n",
			  ret, vni, cxi_tc_to_str(tc),
			  cxi_tc_type_to_str(tc_type));
		CXIP_WARN("Remapping original TC from %s to %s\n",
			  cxi_tc_to_str(tc), cxi_tc_to_str(remap_tc));

		/* Check to see if a matching HW CP has already been allocated.
		 * If so, reuse the entry.
		 */
		for (i = 0; i < lni->n_cps; i++) {
			if (lni->hw_cps[i]->vni == vni &&
			    lni->hw_cps[i]->tc == remap_tc &&
			    lni->hw_cps[i]->tc_type == tc_type) {
				sw_cp->hw_cp = lni->hw_cps[i];
				goto found_hw_cp;
			}
		}

		/* Attempt to allocated a remapped HW CP. */
		ret = cxil_alloc_cp(lni->lni, vni, remap_tc, tc_type,
				    &lni->hw_cps[lni->n_cps]);
		if (ret) {
			CXIP_WARN("Failed to allocate CP, ret: %d VNI: %u TC: %s TYPE: %s\n",
				  ret, vni, cxi_tc_to_str(remap_tc),
				  cxi_tc_type_to_str(tc_type));
			ret = -FI_EINVAL;
			goto err_free_sw_cp;
		}
	}

	CXIP_DBG("Allocated CP: %u VNI: %u TC: %s TYPE: %s\n",
		 lni->hw_cps[lni->n_cps]->lcid, vni,
		 cxi_tc_to_str(lni->hw_cps[lni->n_cps]->tc),
		 cxi_tc_type_to_str(lni->hw_cps[lni->n_cps]->tc_type));

	sw_cp->hw_cp = lni->hw_cps[lni->n_cps++];

found_hw_cp:
	sw_cp->remap_cp.vni = vni;
	sw_cp->remap_cp.tc = tc;
	sw_cp->remap_cp.tc_type = tc_type;
	sw_cp->remap_cp.lcid = sw_cp->hw_cp->lcid;
	dlist_insert_tail(&sw_cp->remap_entry, &lni->remap_cps);

	*cp = &sw_cp->remap_cp;

success_unlock:
	ofi_spin_unlock(&lni->lock);

	return FI_SUCCESS;

err_free_sw_cp:
	free(sw_cp);
err_unlock:
	ofi_spin_unlock(&lni->lock);

	return ret;
}

int cxip_cmdq_cp_set(struct cxip_cmdq *cmdq, uint16_t vni,
		     enum cxi_traffic_class tc,
		     enum cxi_traffic_class_type tc_type)
{
	struct cxi_cp *cp;
	int ret;

	if (cxip_cmdq_match(cmdq, vni, tc, tc_type))
		return FI_SUCCESS;

	ret = cxip_cp_get(cmdq->lni, vni, tc, tc_type, &cp);
	if (ret != FI_SUCCESS) {
		CXIP_DBG("Failed to get CP: %d\n", ret);
		return -FI_EOTHER;
	}

	ret = cxi_cq_emit_cq_lcid(cmdq->dev_cmdq, cp->lcid);
	if (ret) {
		CXIP_DBG("Failed to update CMDQ(%p) CP: %d\n", cmdq, ret);
		ret = -FI_EAGAIN;
	} else {
		ret = FI_SUCCESS;
		cmdq->cur_cp = cp;

		CXIP_DBG("Updated CMDQ(%p) CP: %d VNI: %u TC: %s TYPE: %s\n",
			 cmdq, cp->lcid, cp->vni, cxi_tc_to_str(cp->tc),
			 cxi_tc_type_to_str(cp->tc_type));
	}

	return ret;
}

/*
 * cxip_cmdq_alloc() - Allocate a command queue.
 */
int cxip_cmdq_alloc(struct cxip_lni *lni, struct cxi_eq *evtq,
		    struct cxi_cq_alloc_opts *cq_opts, uint16_t vni,
		    enum cxi_traffic_class tc,
		    enum cxi_traffic_class_type tc_type,
		    struct cxip_cmdq **cmdq)
{
	int ret;
	struct cxi_cq *dev_cmdq;
	struct cxip_cmdq *new_cmdq;
	struct cxi_cp *cp = NULL;

	new_cmdq = calloc(1, sizeof(*new_cmdq));
	if (!new_cmdq) {
		CXIP_WARN("Unable to allocate CMDQ structure\n");
		return -FI_ENOMEM;
	}

	if (cq_opts->flags & CXI_CQ_IS_TX) {
		ret = cxip_cp_get(lni, vni, tc, tc_type, &cp);
		if (ret != FI_SUCCESS) {
			CXIP_WARN("Failed to allocate CP: %d\n", ret);
			return ret;
		}
		cq_opts->lcid = cp->lcid;

		new_cmdq->cur_cp = cp;

		/* Trig command queue can never use LL ring. */
		if (cq_opts->flags & CXI_CQ_TX_WITH_TRIG_CMDS ||
		    lni->iface->info->device_platform == CXI_PLATFORM_NETSIM)
			new_cmdq->llring_mode = CXIP_LLRING_NEVER;
		else
			new_cmdq->llring_mode = cxip_env.llring_mode;
	} else {
		new_cmdq->llring_mode = CXIP_LLRING_NEVER;
	}

	ret = cxil_alloc_cmdq(lni->lni, evtq, cq_opts, &dev_cmdq);
	if (ret) {
		CXIP_WARN("Failed to allocate %s, ret: %d\n",
			  cq_opts->flags & CXI_CQ_IS_TX ? "TXQ" : "TGQ", ret);
		ret = -FI_ENOSPC;
		goto free_cmdq;
	}

	new_cmdq->dev_cmdq = dev_cmdq;
	new_cmdq->lni = lni;
	*cmdq = new_cmdq;

	return FI_SUCCESS;

free_cmdq:
	free(new_cmdq);

	return ret;
}

/*
 * cxip_cmdq_free() - Free a command queue.
 */
void cxip_cmdq_free(struct cxip_cmdq *cmdq)
{
	int ret;

	ret = cxil_destroy_cmdq(cmdq->dev_cmdq);
	if (ret)
		CXIP_WARN("cxil_destroy_cmdq failed, ret: %d\n", ret);

	free(cmdq);
}

/* Must hold cmdq->lock. */
int cxip_cmdq_emit_c_state(struct cxip_cmdq *cmdq,
			   const struct c_cstate_cmd *c_state)
{
	int ret;

	if (memcmp(&cmdq->c_state, c_state, sizeof(*c_state))) {
		ret = cxi_cq_emit_c_state(cmdq->dev_cmdq, c_state);
		if (ret) {
			CXIP_DBG("Failed to issue C_STATE command: %d\n", ret);
			return -FI_EAGAIN;
		}

		cmdq->c_state = *c_state;
	}

	return FI_SUCCESS;
}

int cxip_cmdq_emit_idc_put(struct cxip_cmdq *cmdq,
			   const struct c_cstate_cmd *c_state,
			   const struct c_idc_put_cmd *put, const void *buf,
			   size_t len, uint64_t flags)
{
	int ret;

	if (flags & (FI_FENCE | FI_CXI_WEAK_FENCE)) {
		ret = cxi_cq_emit_cq_cmd(cmdq->dev_cmdq, C_CMD_CQ_FENCE);
		if (ret) {
			CXIP_WARN("Failed to issue fence command: %d:%s\n", ret,
				  fi_strerror(-ret));
			return -FI_EAGAIN;
		}
	}

	ret = cxip_cmdq_emit_c_state(cmdq, c_state);
	if (ret) {
		CXIP_WARN("Failed to emit c_state command: %d:%s\n", ret,
			  fi_strerror(-ret));
		return ret;
	}

	ret = cxi_cq_emit_idc_put(cmdq->dev_cmdq, put, buf, len);
	if (ret) {
		CXIP_WARN("Failed to emit idc_put command: %d:%s\n", ret,
			  fi_strerror(-ret));
		return -FI_EAGAIN;
	}

	return FI_SUCCESS;
}

int cxip_cmdq_emit_dma(struct cxip_cmdq *cmdq, struct c_full_dma_cmd *dma,
		       uint64_t flags)
{
	int ret;

	if (flags & (FI_FENCE | FI_CXI_WEAK_FENCE)) {
		ret = cxi_cq_emit_cq_cmd(cmdq->dev_cmdq, C_CMD_CQ_FENCE);
		if (ret) {
			CXIP_WARN("Failed to issue fence command: %d:%s\n", ret,
				  fi_strerror(-ret));
			return -FI_EAGAIN;
		}
	}

	ret = cxi_cq_emit_dma(cmdq->dev_cmdq, dma);
	if (ret) {
		CXIP_WARN("Failed to emit dma command: %d:%s\n", ret,
			  fi_strerror(-ret));
		return -FI_EAGAIN;
	}

	return FI_SUCCESS;
}

int cxip_cmdq_emic_idc_amo(struct cxip_cmdq *cmdq,
			   const struct c_cstate_cmd *c_state,
			   const struct c_idc_amo_cmd *amo, uint64_t flags,
			   bool fetching, bool flush)
{
	struct c_full_dma_cmd flush_cmd;
	bool fetching_flush = fetching && flush;
	int ret;

	if (fetching_flush) {
		memset(&flush_cmd, 0, sizeof(flush_cmd));
		flush_cmd.command.opcode = C_CMD_PUT;
		flush_cmd.index_ext = c_state->index_ext;
		flush_cmd.event_send_disable = 1;
		flush_cmd.dfa = amo->idc_header.dfa;
		flush_cmd.remote_offset = amo->idc_header.remote_offset;
		flush_cmd.eq = c_state->eq;
		flush_cmd.user_ptr = c_state->user_ptr;
		flush_cmd.flush = 1;
	}

	if (flags & (FI_FENCE | FI_CXI_WEAK_FENCE)) {
		ret = cxi_cq_emit_cq_cmd(cmdq->dev_cmdq, C_CMD_CQ_FENCE);
		if (ret) {
			CXIP_WARN("Failed to issue fence command: %d:%s\n", ret,
				  fi_strerror(-ret));
			return -FI_EAGAIN;
		}
	}

	ret = cxip_cmdq_emit_c_state(cmdq, c_state);
	if (ret) {
		CXIP_WARN("Failed to emit c_state command: %d:%s\n", ret,
			  fi_strerror(-ret));
		return ret;
	}

	/* Fetching AMO with flush requires two commands. Ensure there is enough
	 * space. At worse at least 16x 32-byte slots are needed.
	 */
	if (fetching_flush && __cxi_cq_free_slots(cmdq->dev_cmdq) < 16) {
		CXIP_WARN("No space for FAMO with FI_DELIVERY_COMPLETE\n");
		return -FI_EAGAIN;
	}

	ret = cxi_cq_emit_idc_amo(cmdq->dev_cmdq, amo, fetching);
	if (ret) {
		CXIP_WARN("Failed to emit IDC amo\n");
		return -FI_EAGAIN;
	}

	if (fetching_flush) {
		/* CQ space check already occurred. Thus, return code can be
		 * ignored.
		 */
		ret = cxi_cq_emit_dma(cmdq->dev_cmdq, &flush_cmd);
		assert(ret == 0);
	}

	return FI_SUCCESS;
}

int cxip_cmdq_emit_dma_amo(struct cxip_cmdq *cmdq, struct c_dma_amo_cmd *amo,
			   uint64_t flags, bool fetching, bool flush)
{
	struct c_full_dma_cmd flush_cmd;
	bool fetching_flush = fetching && flush;
	int ret;

	if (fetching_flush) {
		memset(&flush_cmd, 0, sizeof(flush_cmd));
		flush_cmd.command.opcode = C_CMD_PUT;
		flush_cmd.index_ext = amo->index_ext;
		flush_cmd.event_send_disable = 1;
		flush_cmd.dfa = amo->dfa;
		flush_cmd.remote_offset = amo->remote_offset;
		flush_cmd.eq = amo->eq;
		flush_cmd.user_ptr = amo->user_ptr;
		flush_cmd.flush = 1;
		flush_cmd.match_bits = amo->match_bits;
	}

	if (flags & (FI_FENCE | FI_CXI_WEAK_FENCE)) {
		ret = cxi_cq_emit_cq_cmd(cmdq->dev_cmdq, C_CMD_CQ_FENCE);
		if (ret) {
			CXIP_WARN("Failed to issue fence command: %d:%s\n", ret,
				  fi_strerror(-ret));
			return -FI_EAGAIN;
		}
	}

	/* Fetching AMO with flush requires two commands. Ensure there is enough
	 * space. At worse at least 16x 32-byte slots are needed.
	 */
	if (fetching_flush && __cxi_cq_free_slots(cmdq->dev_cmdq) < 16) {
		CXIP_WARN("No space for FAMO with FI_DELIVERY_COMPLETE\n");
		return -FI_EAGAIN;
	}

	ret = cxi_cq_emit_dma_amo(cmdq->dev_cmdq, amo, fetching);
	if (ret) {
		CXIP_WARN("Failed to emit DMA amo\n");
		return -FI_EAGAIN;
	}

	if (fetching_flush) {
		/* CQ space check already occurred. Thus, return code can be
		 * ignored.
		 */
		ret = cxi_cq_emit_dma(cmdq->dev_cmdq, &flush_cmd);
		assert(ret == 0);
	}

	return FI_SUCCESS;
}

int cxip_cmdq_emit_idc_msg(struct cxip_cmdq *cmdq,
			   const struct c_cstate_cmd *c_state,
			   const struct c_idc_msg_hdr *msg, const void *buf,
			   size_t len, uint64_t flags)
{
	int ret;

	if (flags & (FI_FENCE | FI_CXI_WEAK_FENCE)) {
		ret = cxi_cq_emit_cq_cmd(cmdq->dev_cmdq, C_CMD_CQ_FENCE);
		if (ret) {
			CXIP_WARN("Failed to issue fence command: %d:%s\n", ret,
				  fi_strerror(-ret));
			return -FI_EAGAIN;
		}
	}

	ret = cxip_cmdq_emit_c_state(cmdq, c_state);
	if (ret) {
		CXIP_WARN("Failed to emit c_state command: %d:%s\n", ret,
			  fi_strerror(-ret));
		return ret;
	}

	ret = cxi_cq_emit_idc_msg(cmdq->dev_cmdq, msg, buf, len);
	if (ret) {
		CXIP_WARN("Failed to emit idc_msg command: %d:%s\n", ret,
			  fi_strerror(-ret));
		return -FI_EAGAIN;
	}

	return FI_SUCCESS;
}

/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2014 Intel Corporation, Inc. All rights reserved.
 * Copyright (c) 2018 Hewlett Packard Enterprise Development LP
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "cxip.h"

#include <ofi_util.h>

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_EP_DATA, __VA_ARGS__)
#define CXIP_INFO(...) _CXIP_WARN(FI_LOG_EP_DATA, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EP_DATA, __VA_ARGS__)

static int cxip_cntr_copy_ct_writeback(struct cxip_cntr *cntr,
				       struct c_ct_writeback *wb_copy)
{
	struct cxip_domain *dom = cntr->domain;
	ssize_t ret __attribute__((unused));
	struct iovec hmem_iov;

	if (cntr->wb_iface == FI_HMEM_SYSTEM) {
		memcpy(wb_copy, cntr->wb, sizeof(*cntr->wb));
		return FI_SUCCESS;
	}

	if (cntr->wb_handle_valid) {
		ret = ofi_hmem_dev_reg_copy_from_hmem(cntr->wb_iface,
						      cntr->wb_handle, wb_copy,
						      cntr->wb,
						      sizeof(*cntr->wb));
		assert(ret == FI_SUCCESS);
		return FI_SUCCESS;
	}

	hmem_iov.iov_base = cntr->wb;
	hmem_iov.iov_len = sizeof(*cntr->wb);

	ret = dom->hmem_ops.copy_from_hmem_iov(wb_copy, sizeof(*cntr->wb),
					       cntr->wb_iface, cntr->wb_device,
					       &hmem_iov, 1, 0);
	assert(ret == sizeof(*wb_copy));
	return FI_SUCCESS;
}

static int cxip_cntr_get_ct_error(struct cxip_cntr *cntr, uint64_t *error)
{
	struct c_ct_writeback wb_copy;
	int ret;

	/* Only can reference the ct_failure field directly if dealing with
	 * system memory. Device memory requires a memcpy of the contents into
	 * system memory.
	 */
	if (cntr->wb_iface == FI_HMEM_SYSTEM) {
		*error = cntr->wb->ct_failure;
		return FI_SUCCESS;
	}

	ret = cxip_cntr_copy_ct_writeback(cntr, &wb_copy);
	if (ret)
		return ret;

	*error = wb_copy.ct_failure;
	return FI_SUCCESS;
}

static int cxip_cntr_get_ct_success(struct cxip_cntr *cntr, uint64_t *success)
{
	struct c_ct_writeback wb_copy;
	int ret;

	/* Only can reference the ct_success field directly if dealing with
	 * system memory. Device memory requires a memcpy of the contents into
	 * system memory.
	 */
	if (cntr->wb_iface == FI_HMEM_SYSTEM) {
		*success = cntr->wb->ct_success;
		return FI_SUCCESS;
	}

	ret = cxip_cntr_copy_ct_writeback(cntr, &wb_copy);
	if (ret)
		return ret;

	*success = wb_copy.ct_success;
	return FI_SUCCESS;
}

#define CT_WRITEBACK_OFFSET 7U

static int cxip_cntr_clear_ct_writeback(struct cxip_cntr *cntr)
{
	struct iovec hmem_iov;
	ssize_t ret __attribute__((unused));
	uint8_t ct_writeback;

	/* Only can reference the ct_success field directly if dealing with
	 * system memory. Device memory requires a memcpy of the contents into
	 * device memory.
	 */
	if (cntr->wb_iface == FI_HMEM_SYSTEM) {
		cntr->wb->ct_writeback = 0;
		return FI_SUCCESS;
	}

	/* Only write to ct_writeback byte. */
	ct_writeback = 0;
	hmem_iov.iov_base = (char *)cntr->wb + CT_WRITEBACK_OFFSET;
	hmem_iov.iov_len = 1;

	ret = cntr->domain->hmem_ops.copy_to_hmem_iov(cntr->wb_iface, 0,
						      &hmem_iov, 1, 0,
						      &ct_writeback, 1);
	assert(ret == 1);

	return FI_SUCCESS;
}

static int cxip_cntr_get_ct_writeback(struct cxip_cntr *cntr)
{
	struct c_ct_writeback wb_copy;
	int ret;

	/* Only can reference the ct_writeback field directly if dealing with
	 * system memory. Device memory requires a memcpy of the contents into
	 * system memory.
	 */
	if (cntr->wb_iface == FI_HMEM_SYSTEM)
		return cntr->wb->ct_writeback;

	ret = cxip_cntr_copy_ct_writeback(cntr, &wb_copy);
	if (ret)
		return ret;

	return wb_copy.ct_writeback;
}

#define TRIG_OP_LOCK_NAME_FMT "/.uuid%d_cxi%d_vni%d_svcid%d"
#define TRIG_OP_LOCK_NAME_SIZE 256U

static int cxip_dom_cntr_enable(struct cxip_domain *dom)
{
	char trig_op_lock_name[TRIG_OP_LOCK_NAME_SIZE];
	struct cxi_cq_alloc_opts cq_opts = {
		.policy = CXI_CQ_UPDATE_ALWAYS,
	};
	int ret;

	ofi_spin_lock(&dom->lock);

	if (dom->cntr_init) {
		ofi_spin_unlock(&dom->lock);
		return FI_SUCCESS;
	}

	assert(dom->enabled);

	ret = snprintf(trig_op_lock_name, TRIG_OP_LOCK_NAME_SIZE,
		       TRIG_OP_LOCK_NAME_FMT, getuid(),
		       dom->iface->dev->info.dev_id, dom->auth_key.vni,
		       dom->auth_key.svc_id);
	if (ret >= TRIG_OP_LOCK_NAME_SIZE) {
		CXIP_WARN("snprintf buffer too small\n");
		ret = -FI_ENOSPC;
		goto err_unlock;
	} else if (ret < 0) {
		CXIP_WARN("snprintf failed: %d\n", ret);
		goto err_unlock;
	}

	dom->trig_op_lock = sem_open(trig_op_lock_name, O_CREAT,
				     S_IRUSR | S_IWUSR, 1);
	if (dom->trig_op_lock == SEM_FAILED) {
		ret = -errno;
		CXIP_WARN("sem_open failed: %d\n", ret);
		goto err_unlock;
	}

	cq_opts.count = MAX(dom->max_trig_op_in_use, 64);
	cq_opts.flags = CXI_CQ_IS_TX | CXI_CQ_TX_WITH_TRIG_CMDS;
	cq_opts.policy = CXI_CQ_UPDATE_ALWAYS;

	ret = cxip_cmdq_alloc(dom->lni, NULL, &cq_opts,
			      dom->auth_key.vni,
			      cxip_ofi_to_cxi_tc(dom->tclass),
			      CXI_TC_TYPE_DEFAULT,
			      &dom->trig_cmdq);
	if (ret != FI_SUCCESS) {
		CXIP_WARN("Failed to allocate trig_cmdq: %d\n", ret);
		goto err_close_sem;
	}

	if (dom->util_domain.threading == FI_THREAD_DOMAIN)
		ofi_genlock_init(&dom->trig_cmdq_lock, OFI_LOCK_NONE);
	else
		ofi_genlock_init(&dom->trig_cmdq_lock, OFI_LOCK_SPINLOCK);

	dom->cntr_init = true;

	CXIP_DBG("Domain counters enabled: %p\n", dom);

	ofi_spin_unlock(&dom->lock);

	return FI_SUCCESS;

err_close_sem:
	sem_close(dom->trig_op_lock);
err_unlock:
	ofi_spin_unlock(&dom->lock);

	return ret;
}

void cxip_dom_cntr_disable(struct cxip_domain *dom)
{
	char trig_op_lock_name[TRIG_OP_LOCK_NAME_SIZE];
	int ret;

	if (dom->cntr_init) {
		ofi_genlock_destroy(&dom->trig_cmdq_lock);

		sem_close(dom->trig_op_lock);

		ret = snprintf(trig_op_lock_name, TRIG_OP_LOCK_NAME_SIZE,
			TRIG_OP_LOCK_NAME_FMT, getuid(),
			dom->iface->dev->info.dev_id, dom->auth_key.vni,
			dom->auth_key.svc_id);
		if (ret >= TRIG_OP_LOCK_NAME_SIZE)
			CXIP_WARN("snprintf buffer too small\n");
		else if (ret < 0)
			CXIP_WARN("snprintf failed: %d\n", ret);
		else
			sem_unlink(trig_op_lock_name);

		cxip_cmdq_free(dom->trig_cmdq);
	}
}

const struct fi_cntr_attr cxip_cntr_attr = {
	.events = FI_CNTR_EVENTS_COMP,
	.wait_obj = FI_WAIT_YIELD,
	.wait_set = NULL,
	.flags = 0,
};

/*
 * cxip_cntr_mod() - Modify counter value.
 *
 * Set or increment the success or failure value of a counter by 'value'.
 */
int cxip_cntr_mod(struct cxip_cntr *cxi_cntr, uint64_t value, bool set,
		  bool err)
{
	struct c_ct_cmd cmd;
	struct cxip_cmdq *cmdq;
	int ret;

	if (!set) {
		/* Doorbell supports counter increment */
		if (err)
			cxi_ct_inc_failure(cxi_cntr->ct, value);
		else
			cxi_ct_inc_success(cxi_cntr->ct, value);
	} else {
		/* Doorbell supports counter reset */
		if (!value) {
			if (err)
				cxi_ct_reset_failure(cxi_cntr->ct);
			else
				cxi_ct_reset_success(cxi_cntr->ct);
		} else {
			memset(&cmd, 0, sizeof(cmd));
			cmdq = cxi_cntr->domain->trig_cmdq;

			/* Use CQ to set a specific counter value */
			cmd.ct = cxi_cntr->ct->ctn;
			if (err) {
				cmd.set_ct_failure = 1;
				cmd.ct_failure = value;
			} else {
				cmd.set_ct_success = 1;
				cmd.ct_success = value;
			}
			ofi_genlock_lock(&cxi_cntr->domain->trig_cmdq_lock);

			ret = cxi_cq_emit_ct(cmdq->dev_cmdq, C_CMD_CT_SET,
					     &cmd);
			if (ret) {
				ofi_genlock_unlock(&cxi_cntr->domain->trig_cmdq_lock);
				return -FI_EAGAIN;
			}
			cxi_cq_ring(cmdq->dev_cmdq);
			ofi_genlock_unlock(&cxi_cntr->domain->trig_cmdq_lock);
		}
	}

	return FI_SUCCESS;
}

static int cxip_cntr_issue_ct_get(struct cxip_cntr *cntr, bool *issue_ct_get)
{
	int ret;

	/* The calling thread which changes CT writeback bit from 1 to 0 must
	 * issue a CT get command.
	 */
	ofi_mutex_lock(&cntr->lock);

	ret = cxip_cntr_get_ct_writeback(cntr);
	if (ret < 0) {
		CXIP_WARN("Failed to read counter writeback: rc=%d\n", ret);
		goto err_unlock;
	}

	if (ret) {
		ret = cxip_cntr_clear_ct_writeback(cntr);
		if (ret) {
			CXIP_WARN("Failed to clear counter writeback bit: rc=%d\n",
				  ret);
			goto err_unlock;
		}

		*issue_ct_get = true;
	} else {
		*issue_ct_get = false;
	}

	ofi_mutex_unlock(&cntr->lock);

	return FI_SUCCESS;

err_unlock:
	ofi_mutex_unlock(&cntr->lock);

	*issue_ct_get = false;
	return ret;
}

/*
 * cxip_cntr_get() - Schedule a counter write-back.
 *
 * Schedule hardware to write the value of a counter to memory. Avoid
 * scheduling multiple write-backs at once. The counter value will appear in
 * memory a small amount of time later.
 */
static int cxip_cntr_get(struct cxip_cntr *cxi_cntr, bool force)
{
	struct c_ct_cmd cmd;
	struct cxip_cmdq *cmdq;
	int ret;
	bool issue_ct_get;

	if (!force) {
		ret = cxip_cntr_issue_ct_get(cxi_cntr, &issue_ct_get);
		if (ret) {
			CXIP_WARN("cxip_cntr_issue_ct_get() error: rc=%d\n",
				  ret);
			return ret;
		}

		if (!issue_ct_get)
			return FI_SUCCESS;
	}

	memset(&cmd, 0, sizeof(cmd));
	cmdq = cxi_cntr->domain->trig_cmdq;

	/* Request a write-back */
	cmd.ct = cxi_cntr->ct->ctn;

	ofi_genlock_lock(&cxi_cntr->domain->trig_cmdq_lock);
	ret = cxi_cq_emit_ct(cmdq->dev_cmdq, C_CMD_CT_GET, &cmd);
	if (ret) {
		ofi_genlock_unlock(&cxi_cntr->domain->trig_cmdq_lock);
		return -FI_EAGAIN;
	}
	cxi_cq_ring(cmdq->dev_cmdq);
	ofi_genlock_unlock(&cxi_cntr->domain->trig_cmdq_lock);

	return FI_SUCCESS;
}

/*
 * cxip_cntr_progress() - Make CQ progress on bound endpoint.
 */
static void cxip_cntr_progress(struct cxip_cntr *cntr)
{
	struct fid_list_entry *fid_entry;
	struct dlist_entry *item;

	/* Lock is used to protect bound context list. Note that
	 * CQ processing updates counters via doorbells, use of
	 * cntr->lock is not required by CQ processing.
	 */
	ofi_mutex_lock(&cntr->lock);

	dlist_foreach(&cntr->ctx_list, item) {
		fid_entry = container_of(item, struct fid_list_entry, entry);
		cxip_ep_progress(fid_entry->fid);
	}
	ofi_mutex_unlock(&cntr->lock);
}

/*
 * cxip_cntr_read() - fi_cntr_read() implementation.
 */
static uint64_t cxip_cntr_read(struct fid_cntr *fid_cntr)
{
	struct cxip_cntr *cxi_cntr;
	uint64_t success = 0;
	int ret;

	cxi_cntr = container_of(fid_cntr, struct cxip_cntr, cntr_fid);

	cxip_cntr_progress(cxi_cntr);
	cxip_cntr_get(cxi_cntr, false);

	/* TODO: Fall back to reading register on error? */
	ret = cxip_cntr_get_ct_success(cxi_cntr, &success);
	if (ret != FI_SUCCESS)
		CXIP_WARN("Failed to read counter success: rc=%d\n", ret);

	return success;
}

/*
 * cxip_cntr_readerr() - fi_cntr_readerr() implementation.
 */
static uint64_t cxip_cntr_readerr(struct fid_cntr *fid_cntr)
{
	struct cxip_cntr *cxi_cntr;
	uint64_t error = 0;
	int ret;

	cxi_cntr = container_of(fid_cntr, struct cxip_cntr, cntr_fid);

	cxip_cntr_progress(cxi_cntr);
	cxip_cntr_get(cxi_cntr, false);

	/* TODO: Fall back to reading register on error? */
	ret = cxip_cntr_get_ct_error(cxi_cntr, &error);
	if (ret != FI_SUCCESS)
		CXIP_WARN("Failed to read counter error: rc=%d\n", ret);

	return error;
}

/*
 * cxip_cntr_add() - fi_cntr_add() implementation.
 */
static int cxip_cntr_add(struct fid_cntr *fid_cntr, uint64_t value)
{
	struct cxip_cntr *cxi_cntr;

	if (value > FI_CXI_CNTR_SUCCESS_MAX)
		return -FI_EINVAL;

	cxi_cntr = container_of(fid_cntr, struct cxip_cntr, cntr_fid);

	return cxip_cntr_mod(cxi_cntr, value, false, false);
}

/*
 * cxip_cntr_set() - fi_cntr_set() implementation.
 */
static int cxip_cntr_set(struct fid_cntr *fid_cntr, uint64_t value)
{
	struct cxip_cntr *cxi_cntr;

	if (value > FI_CXI_CNTR_SUCCESS_MAX)
		return -FI_EINVAL;

	cxi_cntr = container_of(fid_cntr, struct cxip_cntr, cntr_fid);

	return cxip_cntr_mod(cxi_cntr, value, true, false);
}

/*
 * cxip_cntr_adderr() - fi_cntr_adderr() implementation.
 */
static int cxip_cntr_adderr(struct fid_cntr *fid_cntr, uint64_t value)
{
	struct cxip_cntr *cxi_cntr;

	if (value > FI_CXI_CNTR_FAILURE_MAX)
		return -FI_EINVAL;

	cxi_cntr = container_of(fid_cntr, struct cxip_cntr, cntr_fid);

	return cxip_cntr_mod(cxi_cntr, value, false, true);
}

/*
 * cxip_cntr_seterr() - fi_cntr_seterr() implementation.
 */
static int cxip_cntr_seterr(struct fid_cntr *fid_cntr, uint64_t value)
{
	struct cxip_cntr *cxi_cntr;

	if (value > FI_CXI_CNTR_FAILURE_MAX)
		return -FI_EINVAL;

	cxi_cntr = container_of(fid_cntr, struct cxip_cntr, cntr_fid);

	return cxip_cntr_mod(cxi_cntr, value, true, true);
}

static int cxip_cntr_emit_trig_event_cmd(struct cxip_cntr *cntr,
					 uint64_t threshold)
{
	struct c_ct_cmd cmd = {
		.trig_ct = cntr->ct->ctn,
		.threshold = threshold,
		.eq = C_EQ_NONE,
	};
	struct cxip_cmdq *cmdq = cntr->domain->trig_cmdq;
	int ret;

	/* TODO: Need to handle TLE exhaustion. */
	ofi_genlock_lock(&cntr->domain->trig_cmdq_lock);
	ret = cxi_cq_emit_ct(cmdq->dev_cmdq, C_CMD_CT_TRIG_EVENT, &cmd);
	if (!ret)
		cxi_cq_ring(cmdq->dev_cmdq);
	ofi_genlock_unlock(&cntr->domain->trig_cmdq_lock);

	if (ret)
		return -FI_EAGAIN;
	return FI_SUCCESS;
}

/*
 * cxip_cntr_wait() - fi_cntr_wait() implementation.
 */
static int cxip_cntr_wait(struct fid_cntr *fid_cntr, uint64_t threshold,
			  int timeout)
{
	struct cxip_cntr *cntr =
		container_of(fid_cntr, struct cxip_cntr, cntr_fid);
	uint64_t success = 0;
	int ret;
	uint64_t endtime;
	uint64_t start_error = 0;
	uint64_t error = 0;

	if (cntr->attr.wait_obj == FI_WAIT_NONE ||
	    threshold > FI_CXI_CNTR_SUCCESS_MAX)
		return -FI_EINVAL;

	/* Determine existing value of error count, if it increments
	 * the function should return before threshold has been met.
	 */
	ret = cxip_cntr_get_ct_error(cntr, &start_error);
	if (ret) {
		CXIP_WARN("Failed to read counter error: %d\n", ret);
		return ret;
	}

	endtime = ofi_timeout_time(timeout);

	/* Use a triggered list entry setup to fire at the user's threshold.
	 * This will cause a success/error writeback to occur at the desired
	 * threshold.
	 */
	ret = cxip_cntr_emit_trig_event_cmd(cntr, threshold);
	if (ret) {
		CXIP_INFO("Failed to emit trig cmd: %d\n", ret);
		return ret;
	}

	/* Spin until the trigger list entry fires which updates the CT success
	 * field.
	 */
	do {
		ret = cxip_cntr_get_ct_success(cntr, &success);
		if (ret) {
			CXIP_WARN("Failed to read counter success: %d\n", ret);
			return ret;
		}

		if (success >= threshold)
			return FI_SUCCESS;

		ret = cxip_cntr_get_ct_error(cntr, &error);
		if (ret) {
			CXIP_WARN("Failed to read counter error: %d\n", ret);
			return ret;
		}
		if (error != start_error)
			return -FI_EAVAIL;

		if (ofi_adjust_timeout(endtime, &timeout))
			return -FI_ETIMEDOUT;

		/* Only FI_WAIT_YIELD is supported. */
		sched_yield();

		cxip_cntr_progress(cntr);

	} while (1);

	/* TODO: Triggered operation may get leaked on timeout and threshold
	 * never met.
	 */
}

/*
 * cxip_cntr_control() - fi_control() implementation for counter objects.
 */
static int cxip_cntr_control(struct fid *fid, int command, void *arg)
{
	int ret = FI_SUCCESS;
	struct cxip_cntr *cntr;

	cntr = container_of(fid, struct cxip_cntr, cntr_fid);

	switch (command) {
	case FI_GETWAIT:
		if (cntr->wait)
			ret = fi_control(&cntr->wait->fid,
					 FI_GETWAIT, arg);
		else
			ret = -FI_EINVAL;
		break;

	case FI_GETOPSFLAG:
		memcpy(arg, &cntr->attr.flags, sizeof(uint64_t));
		break;

	case FI_SETOPSFLAG:
		memcpy(&cntr->attr.flags, arg, sizeof(uint64_t));
		break;

	default:
		ret = -FI_EINVAL;
		break;
	}

	return ret;
}

/*
 * cxip_cntr_enable() - Assign hardware resources to the Counter.
 */
static int cxip_cntr_enable(struct cxip_cntr *cxi_cntr)
{
	int ret;

	ret = cxip_dom_cntr_enable(cxi_cntr->domain);
	if (ret != FI_SUCCESS)
		return ret;

	cxi_cntr->wb = &cxi_cntr->lwb;
	cxi_cntr->wb_iface = FI_HMEM_SYSTEM;
	cxi_cntr->wb_handle_valid = false;

	ret = cxil_alloc_ct(cxi_cntr->domain->lni->lni,
			    cxi_cntr->wb, &cxi_cntr->ct);
	if (ret) {
		CXIP_WARN("Failed to allocate CT, ret: %d\n", ret);
		return -FI_EDOMAIN;
	}

	/* Zero the success and failure values. In addition, this will force a
	 * writeback into the writeback buffer.
	 */
	cxi_ct_reset_failure(cxi_cntr->ct);
	cxi_ct_reset_success(cxi_cntr->ct);

	CXIP_DBG("Counter enabled: %p (CT: %d)\n", cxi_cntr, cxi_cntr->ct->ctn);

	return FI_SUCCESS;
}

/*
 * cxip_cntr_close() - fi_close() implementation for counter objects.
 */
static int cxip_cntr_close(struct fid *fid)
{
	struct cxip_cntr *cntr;
	int ret;

	cntr = container_of(fid, struct cxip_cntr, cntr_fid.fid);
	if (ofi_atomic_get32(&cntr->ref))
		return -FI_EBUSY;

	assert(dlist_empty(&cntr->ctx_list));

	if (cntr->wb_iface != FI_HMEM_SYSTEM &&
	    cntr->wb_handle_valid)
		ofi_hmem_dev_unregister(cntr->wb_iface, cntr->wb_handle);

	ret = cxil_destroy_ct(cntr->ct);
	if (ret)
		CXIP_WARN("Failed to free CT, ret: %d\n", ret);
	else
		CXIP_DBG("Counter disabled: %p\n", cntr);

	ofi_mutex_destroy(&cntr->lock);

	cxip_domain_remove_cntr(cntr->domain, cntr);

	free(cntr);
	return 0;
}

/* Set the counter writeback address to a client provided address. */
int cxip_set_wb_buffer(struct fid *fid, void *buf, size_t len)
{
	int ret;
	struct cxip_cntr *cntr;
	uint64_t flags;

	if (!buf)
		return -FI_EINVAL;

	if (len < sizeof(struct c_ct_writeback))
		return -FI_EINVAL;

	cntr = container_of(fid, struct cxip_cntr, cntr_fid.fid);

	ret = cxil_ct_wb_update(cntr->ct, buf);
	if (ret)
		return ret;

	if (cntr->wb_iface != FI_HMEM_SYSTEM &&
	    cntr->wb_handle_valid)
		ofi_hmem_dev_unregister(cntr->wb_iface, cntr->wb_handle);

	cntr->wb = buf;
	cntr->wb_iface = ofi_get_hmem_iface(buf, &cntr->wb_device, &flags);

	if (cntr->wb_iface != FI_HMEM_SYSTEM) {
		ret = ofi_hmem_dev_register(cntr->wb_iface, cntr->wb,
					    sizeof(*cntr->wb),
					    &cntr->wb_handle);
		cntr->wb_handle_valid = (ret == FI_SUCCESS);
	}

	/* Force a counter writeback into the user's provider buffer. */
	do {
		ret = cxip_cntr_get(cntr, true);
	} while (ret == -FI_EAGAIN);

	return ret;
}

/* Get the counter MMIO region. */
int cxip_get_mmio_addr(struct fid *fid, void **addr, size_t *len)
{
	struct cxip_cntr *cntr;

	cntr = container_of(fid, struct cxip_cntr, cntr_fid.fid);

	if (!cntr || !cntr->ct)
		return -FI_EINVAL;

	*addr = cntr->ct->doorbell;
	*len = sizeof(cntr->ct->doorbell);

	return FI_SUCCESS;
}

static struct fi_cxi_cntr_ops cxip_cntr_ext_ops = {
	.set_wb_buffer = cxip_set_wb_buffer,
	.get_mmio_addr = cxip_get_mmio_addr,
};

static int cxip_cntr_ops_open(struct fid *fid, const char *ops_name,
			      uint64_t flags, void **ops, void *context)
{
	if (!strcmp(ops_name, FI_CXI_COUNTER_OPS)) {
		*ops = &cxip_cntr_ext_ops;
		return FI_SUCCESS;
	}

	return -FI_EINVAL;
}

static struct fi_ops_cntr cxip_cntr_ops = {
	.size = sizeof(struct fi_ops_cntr),
	.readerr = cxip_cntr_readerr,
	.read = cxip_cntr_read,
	.add = cxip_cntr_add,
	.set = cxip_cntr_set,
	.wait = cxip_cntr_wait,
	.adderr = cxip_cntr_adderr,
	.seterr = cxip_cntr_seterr,
};

static struct fi_ops cxip_cntr_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = cxip_cntr_close,
	.bind = fi_no_bind,
	.control = cxip_cntr_control,
	.ops_open = cxip_cntr_ops_open,
};

/*
 * cxip_cntr_verify_attr() - Verify counter creation attributes.
 */
static int cxip_cntr_verify_attr(struct fi_cntr_attr *attr)
{
	if (!attr)
		return FI_SUCCESS;

	if (attr->events != FI_CNTR_EVENTS_COMP &&
	    attr->events != FI_CXI_CNTR_EVENTS_BYTES)
		return -FI_ENOSYS;

	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
	case FI_WAIT_UNSPEC:
	case FI_WAIT_YIELD:
		break;
	default:
		return -FI_ENOSYS;
	}

	if (attr->flags)
		return -FI_ENOSYS;

	return FI_SUCCESS;
}

/*
 * cxip_cntr_open() - fi_cntr_open() implementation.
 */
int cxip_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		   struct fid_cntr **cntr, void *context)
{
	int ret;
	struct cxip_domain *dom;
	struct cxip_cntr *_cntr;

	dom = container_of(domain, struct cxip_domain, util_domain.domain_fid);

	ret = cxip_cntr_verify_attr(attr);
	if (ret != FI_SUCCESS)
		return ret;

	_cntr = calloc(1, sizeof(*_cntr));
	if (!_cntr)
		return -FI_ENOMEM;

	if (!attr)
		memcpy(&_cntr->attr, &cxip_cntr_attr, sizeof(cxip_cntr_attr));
	else
		memcpy(&_cntr->attr, attr, sizeof(cxip_cntr_attr));

	ofi_atomic_initialize32(&_cntr->ref, 0);
	dlist_init(&_cntr->ctx_list);

	ofi_mutex_init(&_cntr->lock);

	_cntr->cntr_fid.fid.fclass = FI_CLASS_CNTR;
	_cntr->cntr_fid.fid.context = context;
	_cntr->cntr_fid.fid.ops = &cxip_cntr_fi_ops;
	_cntr->cntr_fid.ops = &cxip_cntr_ops;
	_cntr->domain = dom;

	ret = cxip_cntr_enable(_cntr);
	if (ret)
		goto err_free_cntr;

	cxip_domain_add_cntr(dom, _cntr);

	*cntr = &_cntr->cntr_fid;

	return FI_SUCCESS;

err_free_cntr:
	free(_cntr);

	return ret;
}

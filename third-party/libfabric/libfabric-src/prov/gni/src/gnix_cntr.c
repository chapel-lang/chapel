/*
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
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

/*
 * CNTR common code
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gnix.h"
#include "gnix_cntr.h"
#include "gnix_nic.h"
#include "gnix_trigger.h"

/*******************************************************************************
 * Forward declarations for filling functions.
 ******************************************************************************/

/*******************************************************************************
 * Forward declarations for ops structures.
 ******************************************************************************/
static struct fi_ops gnix_cntr_fi_ops;
static struct fi_ops_cntr gnix_cntr_ops;

/*******************************************************************************
 * Internal helper functions
 ******************************************************************************/

static int __verify_cntr_attr(struct fi_cntr_attr *attr)
{
	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_CQ, "\n");

	if (!attr)
		return -FI_EINVAL;

	if (attr->events != FI_CNTR_EVENTS_COMP) {
		GNIX_WARN(FI_LOG_CQ, "cntr event type: %d unsupported.\n",
			  attr->events);
		return -FI_EINVAL;
	}

	switch (attr->wait_obj) {
	case FI_WAIT_UNSPEC:
	case FI_WAIT_NONE:
	case FI_WAIT_SET:
		break;
	case FI_WAIT_FD:
	case FI_WAIT_MUTEX_COND:
	default:
		GNIX_WARN(FI_LOG_CQ, "wait type: %d unsupported.\n",
			  attr->wait_obj);
		return -FI_EINVAL;
	}

	return ret;
}

static int gnix_cntr_set_wait(struct gnix_fid_cntr *cntr)
{
	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_EQ, "\n");

	struct fi_wait_attr requested = {
		.wait_obj = cntr->attr.wait_obj,
		.flags = 0
	};

	switch (cntr->attr.wait_obj) {
	case FI_WAIT_UNSPEC:
		ret = gnix_wait_open(&cntr->domain->fabric->fab_fid,
				&requested, &cntr->wait);
		break;
	case FI_WAIT_SET:
		ret = _gnix_wait_set_add(cntr->attr.wait_set,
					 &cntr->cntr_fid.fid);

		if (!ret)
			cntr->wait = cntr->attr.wait_set;
		break;
	default:
		break;
	}

	return ret;
}

static int __gnix_cntr_progress(struct gnix_fid_cntr *cntr)
{
	return _gnix_prog_progress(&cntr->pset);
}

/*******************************************************************************
 * Exposed helper functions
 ******************************************************************************/

int _gnix_cntr_inc(struct gnix_fid_cntr *cntr)
{
	if (cntr == NULL)
		return -FI_EINVAL;

	ofi_atomic_inc32(&cntr->cnt);

	if (cntr->wait)
		_gnix_signal_wait_obj(cntr->wait);

	if (_gnix_trigger_pending(cntr))
		_gnix_trigger_check_cntr(cntr);

	return FI_SUCCESS;
}

int _gnix_cntr_inc_err(struct gnix_fid_cntr *cntr)
{
	if (cntr == NULL)
		return -FI_EINVAL;

	ofi_atomic_inc32(&cntr->cnt_err);

	if (cntr->wait)
		_gnix_signal_wait_obj(cntr->wait);

	return FI_SUCCESS;
}

int _gnix_cntr_poll_obj_add(struct gnix_fid_cntr *cntr, void *obj,
			    int (*prog_fn)(void *data))
{
	return _gnix_prog_obj_add(&cntr->pset, obj, prog_fn);
}

int _gnix_cntr_poll_obj_rem(struct gnix_fid_cntr *cntr, void *obj,
			    int (*prog_fn)(void *data))
{
	return _gnix_prog_obj_rem(&cntr->pset, obj, prog_fn);
}

/*******************************************************************************
 * API functions.
 ******************************************************************************/

static int gnix_cntr_wait_sleep(struct gnix_fid_cntr *cntr_priv,
				uint64_t threshold, int timeout)
{
	int ret = FI_SUCCESS;
	struct timespec ts0, ts;
	int msec_passed = 0;

	clock_gettime(CLOCK_REALTIME, &ts0);
	while (ofi_atomic_get32(&cntr_priv->cnt) < threshold &&
	       ofi_atomic_get32(&cntr_priv->cnt_err) == 0) {

		ret = gnix_wait_wait((struct fid_wait *)cntr_priv->wait,
					timeout - msec_passed);
		if (ret == -FI_ETIMEDOUT)
			break;

		if (ret) {
			GNIX_WARN(FI_LOG_CQ,
				" fi_wait returned %d.\n",
				  ret);
			break;
		}

		if (ofi_atomic_get32(&cntr_priv->cnt) >= threshold)
			break;

		if (timeout < 0)
			continue;

		clock_gettime(CLOCK_REALTIME, &ts);
		msec_passed = (ts.tv_sec - ts0.tv_sec) * 1000 +
			      (ts.tv_nsec - ts0.tv_nsec) / 100000;

		if (msec_passed >= timeout) {
			ret = -FI_ETIMEDOUT;
			break;
		}
	}

	return (ofi_atomic_get32(&cntr_priv->cnt_err)) ? -FI_EAVAIL : ret;
}


DIRECT_FN STATIC int gnix_cntr_wait(struct fid_cntr *cntr, uint64_t threshold,
				    int timeout)
{
	struct gnix_fid_cntr *cntr_priv;

	cntr_priv = container_of(cntr, struct gnix_fid_cntr, cntr_fid);
	if (!cntr_priv->wait)
		return -FI_EINVAL;

	if (cntr_priv->attr.wait_obj == FI_WAIT_SET ||
	    cntr_priv->attr.wait_obj == FI_WAIT_NONE)
		return -FI_EINVAL;

	return gnix_cntr_wait_sleep(cntr_priv, threshold, timeout);
}

DIRECT_FN STATIC int gnix_cntr_adderr(struct fid_cntr *cntr, uint64_t value)
{
	struct gnix_fid_cntr *cntr_priv;

	cntr_priv = container_of(cntr, struct gnix_fid_cntr, cntr_fid);
	if (FI_VERSION_LT(cntr_priv->domain->fabric->fab_fid.api_version, FI_VERSION(1, 5)))
		return -FI_EOPNOTSUPP;

	ofi_atomic_add32(&cntr_priv->cnt_err, (int)value);

	if (cntr_priv->wait)
		_gnix_signal_wait_obj(cntr_priv->wait);

	return FI_SUCCESS;
}

DIRECT_FN STATIC int gnix_cntr_seterr(struct fid_cntr *cntr, uint64_t value)
{
	struct gnix_fid_cntr *cntr_priv;

	cntr_priv = container_of(cntr, struct gnix_fid_cntr, cntr_fid);

	if (FI_VERSION_LT(cntr_priv->domain->fabric->fab_fid.api_version, FI_VERSION(1, 5)))
		return -FI_EOPNOTSUPP;

	ofi_atomic_set32(&cntr_priv->cnt_err, (int)value);

	if (cntr_priv->wait)
		_gnix_signal_wait_obj(cntr_priv->wait);

	return FI_SUCCESS;
}

static void __cntr_destruct(void *obj)
{
	struct gnix_fid_cntr *cntr = (struct gnix_fid_cntr *) obj;

	_gnix_ref_put(cntr->domain);

	switch (cntr->attr.wait_obj) {
	case FI_WAIT_NONE:
		break;
	case FI_WAIT_SET:
		_gnix_wait_set_remove(cntr->wait, &cntr->cntr_fid.fid);
		break;
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
	case FI_WAIT_MUTEX_COND:
		assert(cntr->wait);
		gnix_wait_close(&cntr->wait->fid);
		break;
	default:
		GNIX_WARN(FI_LOG_CQ, "format: %d unsupported.\n",
			  cntr->attr.wait_obj);
		break;
	}

	_gnix_prog_fini(&cntr->pset);

	free(cntr);
}

static int gnix_cntr_close(fid_t fid)
{
	struct gnix_fid_cntr *cntr;
	int references_held;

	GNIX_TRACE(FI_LOG_CQ, "\n");

	cntr = container_of(fid, struct gnix_fid_cntr, cntr_fid.fid);

	/* applications should never call close more than once. */
	references_held = _gnix_ref_put(cntr);
	if (references_held) {
		GNIX_INFO(FI_LOG_CQ, "failed to fully close cntr due to lingering "
			  "references. references=%i cntr=%p\n",
			  references_held, cntr);
	}

	return FI_SUCCESS;
}

DIRECT_FN STATIC uint64_t gnix_cntr_readerr(struct fid_cntr *cntr)
{
	int v, ret;
	struct gnix_fid_cntr *cntr_priv;

	if (cntr == NULL)
		return -FI_EINVAL;

	cntr_priv = container_of(cntr, struct gnix_fid_cntr, cntr_fid);
	v = ofi_atomic_get32(&cntr_priv->cnt_err);

	ret = __gnix_cntr_progress(cntr_priv);
	if (ret != FI_SUCCESS)
		GNIX_WARN(FI_LOG_CQ, " __gnix_cntr_progress returned %d.\n",
			  ret);

	return (uint64_t)v;
}

DIRECT_FN STATIC uint64_t gnix_cntr_read(struct fid_cntr *cntr)
{
	int v, ret;
	struct gnix_fid_cntr *cntr_priv;

	if (cntr == NULL)
		return -FI_EINVAL;

	cntr_priv = container_of(cntr, struct gnix_fid_cntr, cntr_fid);

	if (cntr_priv->wait)
		gnix_wait_wait((struct fid_wait *)cntr_priv->wait, 0);

	ret = __gnix_cntr_progress(cntr_priv);
	if (ret != FI_SUCCESS)
		GNIX_WARN(FI_LOG_CQ, " __gnix_cntr_progress returned %d.\n",
			  ret);

	v = ofi_atomic_get32(&cntr_priv->cnt);

	return (uint64_t)v;
}

DIRECT_FN STATIC int gnix_cntr_add(struct fid_cntr *cntr, uint64_t value)
{
	struct gnix_fid_cntr *cntr_priv;

	if (cntr == NULL)
		return -FI_EINVAL;

	cntr_priv = container_of(cntr, struct gnix_fid_cntr, cntr_fid);
	ofi_atomic_add32(&cntr_priv->cnt, (int)value);

	if (cntr_priv->wait)
		_gnix_signal_wait_obj(cntr_priv->wait);

	_gnix_trigger_check_cntr(cntr_priv);

	return FI_SUCCESS;
}

DIRECT_FN STATIC int gnix_cntr_set(struct fid_cntr *cntr, uint64_t value)
{
	struct gnix_fid_cntr *cntr_priv;

	if (cntr == NULL)
		return -FI_EINVAL;

	cntr_priv = container_of(cntr, struct gnix_fid_cntr, cntr_fid);
	ofi_atomic_set32(&cntr_priv->cnt, (int)value);

	if (cntr_priv->wait)
		_gnix_signal_wait_obj(cntr_priv->wait);

	_gnix_trigger_check_cntr(cntr_priv);

	return FI_SUCCESS;
}

static int gnix_cntr_control(struct fid *cntr, int command, void *arg)
{
	struct gnix_fid_cntr *cntr_priv;

	if (cntr == NULL)
		return -FI_EINVAL;

	cntr_priv = container_of(cntr, struct gnix_fid_cntr, cntr_fid);

	switch (command) {
	case FI_SETOPSFLAG:
		cntr_priv->attr.flags = *(uint64_t *)arg;
		break;
	case FI_GETOPSFLAG:
		if (!arg)
			return -FI_EINVAL;
		*(uint64_t *)arg = cntr_priv->attr.flags;
		break;
	case FI_GETWAIT:
		/* return _gnix_get_wait_obj(cntr_priv->wait, arg); */
		return -FI_ENOSYS;
	default:
		return -FI_EINVAL;
	}

	return FI_SUCCESS;

}


DIRECT_FN int gnix_cntr_open(struct fid_domain *domain,
			     struct fi_cntr_attr *attr,
			     struct fid_cntr **cntr, void *context)
{
	int ret = FI_SUCCESS;
	struct gnix_fid_domain *domain_priv;
	struct gnix_fid_cntr *cntr_priv;

	GNIX_TRACE(FI_LOG_CQ, "\n");

	ret = __verify_cntr_attr(attr);
	if (ret)
		goto err;

	domain_priv = container_of(domain, struct gnix_fid_domain, domain_fid);
	if (!domain_priv) {
		ret = -FI_EINVAL;
		goto err;
	}

	cntr_priv = calloc(1, sizeof(*cntr_priv));
	if (!cntr_priv) {
		ret = -FI_ENOMEM;
		goto err;
	}

	cntr_priv->requires_lock = (domain_priv->thread_model !=
			FI_THREAD_COMPLETION);

	cntr_priv->domain = domain_priv;
	cntr_priv->attr = *attr;
	/* ref count is initialized to one to show that the counter exists */
	_gnix_ref_init(&cntr_priv->ref_cnt, 1, __cntr_destruct);

	/* initialize atomics */
	ofi_atomic_initialize32(&cntr_priv->cnt, 0);
	ofi_atomic_initialize32(&cntr_priv->cnt_err, 0);

	_gnix_ref_get(cntr_priv->domain);

	_gnix_prog_init(&cntr_priv->pset);

	dlist_init(&cntr_priv->trigger_list);
	fastlock_init(&cntr_priv->trigger_lock);

	ret = gnix_cntr_set_wait(cntr_priv);
	if (ret)
		goto err_wait;

	cntr_priv->cntr_fid.fid.fclass = FI_CLASS_CNTR;
	cntr_priv->cntr_fid.fid.context = context;
	cntr_priv->cntr_fid.fid.ops = &gnix_cntr_fi_ops;
	cntr_priv->cntr_fid.ops = &gnix_cntr_ops;

	*cntr = &cntr_priv->cntr_fid;
	return ret;

err_wait:
	_gnix_ref_put(cntr_priv->domain);
	free(cntr_priv);
err:
	return ret;
}


/*******************************************************************************
 * FI_OPS_* data structures.
 ******************************************************************************/
static struct fi_ops gnix_cntr_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = gnix_cntr_close,
	.bind = fi_no_bind,
	.control = gnix_cntr_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_cntr gnix_cntr_ops = {
	.size = sizeof(struct fi_ops_cntr),
	.readerr = gnix_cntr_readerr,
	.read = gnix_cntr_read,
	.add = gnix_cntr_add,
	.set = gnix_cntr_set,
	.wait = gnix_cntr_wait,
	.adderr = gnix_cntr_adderr,
	.seterr = gnix_cntr_seterr
};

/*
 * Copyright (c) 2013-2019 Intel Corporation. All rights reserved.
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

#include "psmx3.h"
#include "psmx3_trigger.h"

void psmx3_cntr_check_trigger(struct psmx3_fid_cntr *cntr)
{
	struct psmx3_trigger *trigger;
	struct psmx3_trx_ctxt *trx_ctxt;
	struct psmx3_fid_ep *ep;

	if (!cntr->trigger)
		return;

	cntr->domain->trigger_lock_fn(&cntr->trigger_lock, 2);

	trigger = cntr->trigger;
	while (trigger) {
		if (ofi_atomic_get64(&cntr->counter) < trigger->threshold)
			break;

		cntr->trigger = trigger->next;

		/* 'ep' is the first field of the union regardless of the op type */
		ep = container_of(trigger->send.ep, struct psmx3_fid_ep, ep);

		switch (trigger->op) {
		case PSMX3_TRIGGERED_RECV:
		case PSMX3_TRIGGERED_TRECV:
			trx_ctxt = ep->rx;
			break;
		default:
			trx_ctxt = ep->tx;
			break;
		}

		if (trx_ctxt->am_initialized) {
			cntr->domain->trigger_queue_lock_fn(&trx_ctxt->trigger_queue.lock, 2);
			slist_insert_tail(&trigger->list_entry,
					  &trx_ctxt->trigger_queue.list);
			cntr->domain->trigger_queue_unlock_fn(&trx_ctxt->trigger_queue.lock, 2);
		} else {
			psmx3_process_trigger(trx_ctxt, trigger);
		}

		trigger = cntr->trigger;
	}

	cntr->domain->trigger_unlock_fn(&cntr->trigger_lock, 2);
}

void psmx3_cntr_add_trigger(struct psmx3_fid_cntr *cntr,
			    struct psmx3_trigger *trigger)
{
	struct psmx3_trigger *p, *q;

	cntr->domain->trigger_lock_fn(&cntr->trigger_lock, 2);

	q = NULL;
	p = cntr->trigger;
	while (p && p->threshold <= trigger->threshold) {
		q = p;
		p = p->next;
	}
	if (q)
		q->next = trigger;
	else
		cntr->trigger = trigger;
	trigger->next = p;

	cntr->domain->trigger_unlock_fn(&cntr->trigger_lock, 2);

	psmx3_cntr_check_trigger(cntr);
}

DIRECT_FN
STATIC uint64_t psmx3_cntr_read(struct fid_cntr *cntr)
{
	struct psmx3_fid_cntr *cntr_priv;
	struct psmx3_poll_ctxt *poll_ctxt;
	struct slist_entry *item, *prev;

	cntr_priv = container_of(cntr, struct psmx3_fid_cntr, cntr);

	if (cntr_priv->poll_all) {
		psmx3_progress_all(cntr_priv->domain);
	} else {
		slist_foreach(&cntr_priv->poll_list, item, prev) {
			poll_ctxt = container_of(item,
						 struct psmx3_poll_ctxt,
						 list_entry);
			psmx3_progress(poll_ctxt->trx_ctxt);
			(void) prev; /* suppress compiler warning */
		}
	}

	return ofi_atomic_get64(&cntr_priv->counter);
}

DIRECT_FN
STATIC uint64_t psmx3_cntr_readerr(struct fid_cntr *cntr)
{
	struct psmx3_fid_cntr *cntr_priv;

	cntr_priv = container_of(cntr, struct psmx3_fid_cntr, cntr);
	cntr_priv->error_avail = 0;

	return ofi_atomic_get64(&cntr_priv->error_counter);
}

DIRECT_FN
STATIC int psmx3_cntr_add(struct fid_cntr *cntr, uint64_t value)
{
	struct psmx3_fid_cntr *cntr_priv;

	cntr_priv = container_of(cntr, struct psmx3_fid_cntr, cntr);
	ofi_atomic_add64(&cntr_priv->counter, value);

	psmx3_cntr_check_trigger(cntr_priv);

	if (cntr_priv->wait)
		cntr_priv->wait->signal(cntr_priv->wait);

	return 0;
}

DIRECT_FN
STATIC int psmx3_cntr_set(struct fid_cntr *cntr, uint64_t value)
{
	struct psmx3_fid_cntr *cntr_priv;

	cntr_priv = container_of(cntr, struct psmx3_fid_cntr, cntr);
	ofi_atomic_set64(&cntr_priv->counter, value);

	psmx3_cntr_check_trigger(cntr_priv);

	if (cntr_priv->wait)
		cntr_priv->wait->signal(cntr_priv->wait);

	return 0;
}

DIRECT_FN
STATIC int psmx3_cntr_adderr(struct fid_cntr *cntr, uint64_t value)
{
	struct psmx3_fid_cntr *cntr_priv;

	cntr_priv = container_of(cntr, struct psmx3_fid_cntr, cntr);
	ofi_atomic_add64(&cntr_priv->error_counter, value);
	cntr_priv->error_avail = 1;

	psmx3_cntr_check_trigger(cntr_priv);

	if (cntr_priv->wait)
		cntr_priv->wait->signal(cntr_priv->wait);

	return 0;
}

DIRECT_FN
STATIC int psmx3_cntr_seterr(struct fid_cntr *cntr, uint64_t value)
{
	struct psmx3_fid_cntr *cntr_priv;

	cntr_priv = container_of(cntr, struct psmx3_fid_cntr, cntr);
	ofi_atomic_set64(&cntr_priv->error_counter, value);
	cntr_priv->error_avail = 1;

	psmx3_cntr_check_trigger(cntr_priv);

	if (cntr_priv->wait)
		cntr_priv->wait->signal(cntr_priv->wait);

	return 0;
}

DIRECT_FN
STATIC int psmx3_cntr_wait(struct fid_cntr *cntr, uint64_t threshold, int timeout)
{
	struct psmx3_fid_cntr *cntr_priv;
	struct psmx3_poll_ctxt *poll_ctxt;
	struct slist_entry *item, *prev;
	struct timespec ts0, ts;
	int msec_passed = 0;
	int ret = 0;

	cntr_priv = container_of(cntr, struct psmx3_fid_cntr, cntr);

	clock_gettime(CLOCK_REALTIME, &ts0);

	while (ofi_atomic_get64(&cntr_priv->counter) < threshold) {
		if (cntr_priv->error_avail) {
			ret = -FI_EAVAIL;
			break;
		}

		if (cntr_priv->wait) {
			ret = fi_wait((struct fid_wait *)cntr_priv->wait,
				      timeout - msec_passed);
			if (ret == -FI_ETIMEDOUT)
				break;
		} else if (cntr_priv->poll_all) {
			psmx3_progress_all(cntr_priv->domain);
		} else {
			slist_foreach(&cntr_priv->poll_list, item, prev) {
				poll_ctxt = container_of(item,
							 struct psmx3_poll_ctxt,
							 list_entry);
				psmx3_progress(poll_ctxt->trx_ctxt);
				(void) prev; /* suppress compiler warning */
			}
		}

		if (cntr_priv->error_avail) {
			ret = -FI_EAVAIL;
			break;
		}

		if (ofi_atomic_get64(&cntr_priv->counter) >= threshold)
			break;

		if (timeout < 0)
			continue;

		clock_gettime(CLOCK_REALTIME, &ts);
		msec_passed = (ts.tv_sec - ts0.tv_sec) * 1000 +
			      (ts.tv_nsec - ts0.tv_nsec) / 1000000;

		if (msec_passed >= timeout) {
			ret = -FI_ETIMEDOUT;
			break;
		}
	}

	return ret;
}

static int psmx3_cntr_close(fid_t fid)
{
	struct psmx3_fid_cntr *cntr;
	struct psmx3_poll_ctxt *item;
	struct slist_entry *entry;

	cntr = container_of(fid, struct psmx3_fid_cntr, cntr.fid);

	while (!slist_empty(&cntr->poll_list)) {
		entry = slist_remove_head(&cntr->poll_list);
		item = container_of(entry, struct psmx3_poll_ctxt, list_entry);
		if (!ofi_atomic_dec32(&item->trx_ctxt->poll_refcnt))
			free(item->trx_ctxt);
		free(item);
	}

	if (cntr->wait) {
		fi_poll_del(&cntr->wait->pollset->poll_fid, &cntr->cntr.fid, 0);
		if (cntr->wait_is_local)
			fi_close((fid_t)cntr->wait);
	}

	ofi_spin_destroy(&cntr->trigger_lock);
	psmx3_domain_release(cntr->domain);
	free(cntr);

	return 0;
}

static int psmx3_cntr_control(fid_t fid, int command, void *arg)
{
	struct psmx3_fid_cntr *cntr;
	int ret = 0;

	cntr = container_of(fid, struct psmx3_fid_cntr, cntr.fid);

	switch (command) {
	case FI_SETOPSFLAG:
		cntr->flags = *(uint64_t *)arg;
		break;

	case FI_GETOPSFLAG:
		if (!arg)
			return -FI_EINVAL;
		*(uint64_t *)arg = cntr->flags;
		break;

	case FI_GETWAIT:
		if (cntr->wait)
			ret = fi_control(&cntr->wait->wait_fid.fid, FI_GETWAIT, arg);
		else
			return -FI_EINVAL;
		break;
	default:
		return -FI_ENOSYS;
	}

	return ret;
}

static struct fi_ops psmx3_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = psmx3_cntr_close,
	.bind = fi_no_bind,
	.control = psmx3_cntr_control,
	.ops_open = fi_no_ops_open,
};

static struct fi_ops_cntr psmx3_cntr_ops = {
	.size = sizeof(struct fi_ops_cntr),
	.read = psmx3_cntr_read,
	.readerr = psmx3_cntr_readerr,
	.add = psmx3_cntr_add,
	.set = psmx3_cntr_set,
	.wait = psmx3_cntr_wait,
	.adderr = psmx3_cntr_adderr,
	.seterr = psmx3_cntr_seterr,
};

DIRECT_FN
int psmx3_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
			struct fid_cntr **cntr, void *context)
{
	struct psmx3_fid_domain *domain_priv;
	struct psmx3_fid_cntr *cntr_priv;
	struct fid_wait *wait = NULL;
	struct fi_wait_attr wait_attr;
	int wait_is_local = 0;
	int events;
	uint64_t flags;
	int err;

	flags = 0;
	domain_priv = container_of(domain, struct psmx3_fid_domain,
				   util_domain.domain_fid);

	switch (attr->events) {
	case FI_CNTR_EVENTS_COMP:
		events = attr->events;
		break;

	default:
		PSMX3_INFO(&psmx3_prov, FI_LOG_CQ,
			"attr->events=%d, supported=%d\n",
			attr->events, FI_CNTR_EVENTS_COMP);
		return -FI_EINVAL;
	}

	if (psmx3_env.yield_mode && attr->wait_obj != FI_WAIT_NONE) {
		PSMX3_INFO(&psmx3_prov, FI_LOG_CQ,
			"waitset %d not allowed when FI_PSM3_YIELD_MODE enabled\n", attr->wait_obj);
		return -FI_EINVAL;
	}
	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
	case FI_WAIT_UNSPEC:
		break;

	case FI_WAIT_SET:
		if (!attr->wait_set) {
			PSMX3_INFO(&psmx3_prov, FI_LOG_CQ,
				"FI_WAIT_SET is specified but attr->wait_set is NULL\n");
			return -FI_EINVAL;
		}
		wait = attr->wait_set;
		break;

	case FI_WAIT_FD:
	case FI_WAIT_MUTEX_COND:
		wait_attr.wait_obj = attr->wait_obj;
		wait_attr.flags = 0;
		err = fi_wait_open(&domain_priv->fabric->util_fabric.fabric_fid,
				      &wait_attr, (struct fid_wait **)&wait);
		if (err)
			return err;
		wait_is_local = 1;
		break;

	default:
		PSMX3_INFO(&psmx3_prov, FI_LOG_CQ,
			"attr->wait_obj=%d, supported=%d...%d\n",
			attr->wait_obj, FI_WAIT_NONE, FI_WAIT_MUTEX_COND);
		return -FI_EINVAL;
	}

	cntr_priv = (struct psmx3_fid_cntr *) calloc(1, sizeof *cntr_priv);
	if (!cntr_priv) {
		err = -FI_ENOMEM;
		goto fail;
	}


	cntr_priv->domain = domain_priv;
	cntr_priv->events = events;
	if (wait)
		cntr_priv->wait = container_of(wait, struct util_wait, wait_fid);
	cntr_priv->wait_is_local = wait_is_local;
	cntr_priv->flags = flags;
	cntr_priv->cntr.fid.fclass = FI_CLASS_CNTR;
	cntr_priv->cntr.fid.context = context;
	cntr_priv->cntr.fid.ops = &psmx3_fi_ops;
	cntr_priv->cntr.ops = &psmx3_cntr_ops;
	ofi_atomic_initialize64(&cntr_priv->counter, 0);
	ofi_atomic_initialize64(&cntr_priv->error_counter, 0);

	slist_init(&cntr_priv->poll_list);
	ofi_spin_init(&cntr_priv->trigger_lock);

	if (wait)
		fi_poll_add(&cntr_priv->wait->pollset->poll_fid,
			    &cntr_priv->cntr.fid, 0);

	psmx3_domain_acquire(domain_priv);
	*cntr = &cntr_priv->cntr;
	return 0;
fail:
	if (wait && wait_is_local)
		fi_close(&wait->fid);
	return err;
}


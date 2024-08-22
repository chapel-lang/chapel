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

/* It is necessary to have a separate thread making progress in order
 * for the wait functions to succeed. This thread is only created when
 * wait functions are called and. In order to minimize performance
 * impact, it only goes active during te time when wait calls are
 * blocked.
 */
static pthread_t	psmx3_wait_thread;
static pthread_mutex_t	psmx3_wait_mutex;
static pthread_cond_t	psmx3_wait_cond;
static volatile int	psmx3_wait_thread_ready = 0;
static volatile int	psmx3_wait_thread_enabled = 0;
static volatile int	psmx3_wait_thread_busy = 0;

// When fi_wait is called, this thread is activated (psmx3_wait_thread_enabled)
// While activiated it loops checking each domain for progress.
// When progress is made, CQs/CNTRs with wait_sets will get fd events and
// be woken from the fi_wait call below which will then deactivate this thread.
// In future, once psm3_wait supports multi-EP and multi-rail/QP/addr
// could use psm3_wait to wait for a potentially interesting event and then
// use psmx3_progress_all to check for CQ and CNTR events which will wake
// FDs in the waitset and hence wake the fi_wait.  However, psm3_wait is not
// supported by all modes or HALs so need to use the existing polling approach
// if the HAL does not support psm3_wait, which could be determined by an
// error return from  psm3_wait (PSM2_PARAM_ERR or PSM2_INTERNAL_ERR).
static void *psmx3_wait_progress(void *args)
{
	struct psmx3_fid_fabric *fabric = args;
	struct psmx3_fid_domain *domain;
	struct dlist_entry *item;

	psmx3_wait_thread_ready = 1;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	while (1) {
		pthread_mutex_lock(&psmx3_wait_mutex);
		if (!psmx3_wait_thread_enabled)
			pthread_cond_wait(&psmx3_wait_cond, &psmx3_wait_mutex);
		pthread_mutex_unlock(&psmx3_wait_mutex);
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		psmx3_wait_thread_busy = 1;
		while (psmx3_wait_thread_enabled) {
			psmx3_lock(&fabric->domain_lock, 1);
			dlist_foreach(&fabric->domain_list, item) {
				domain = container_of(item, struct psmx3_fid_domain, entry);
				if (domain->progress_thread_enabled &&
				    domain->progress_thread != pthread_self())
					continue;

				psmx3_progress_all(domain);

				if (!psmx3_wait_thread_enabled)
					break;
			}
			psmx3_unlock(&fabric->domain_lock, 1);
		}

		psmx3_wait_thread_busy = 0;

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	}

	return NULL;
}

static void psmx3_wait_start_progress(struct psmx3_fid_fabric *fabric)
{
	struct dlist_entry *item;
	struct psmx3_fid_domain *domain;
	int run_wait_thread = 0;
	pthread_attr_t attr;
	int err;

	if (!fabric)
		return;

	psmx3_lock(&fabric->domain_lock, 1);
	dlist_foreach(&fabric->domain_list, item) {
		domain = container_of(item, struct psmx3_fid_domain, entry);
		if (!domain->progress_thread_enabled ||
		    domain->progress_thread == pthread_self())
			run_wait_thread = 1;
	}
	psmx3_unlock(&fabric->domain_lock, 1);

	if (!run_wait_thread)
		return;

	if (!psmx3_wait_thread) {
		pthread_mutex_init(&psmx3_wait_mutex, NULL);
		pthread_cond_init(&psmx3_wait_cond, NULL);
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		err = pthread_create(&psmx3_wait_thread, &attr,
				     psmx3_wait_progress, (void *)fabric);
		if (err)
			PSMX3_WARN(&psmx3_prov, FI_LOG_EQ,
				"cannot create wait progress thread\n");
		pthread_attr_destroy(&attr);
		while (!psmx3_wait_thread_ready)
			;
	}

	psmx3_wait_thread_enabled = 1;
	pthread_cond_signal(&psmx3_wait_cond);
}

static void psmx3_wait_stop_progress(void)
{
	psmx3_wait_thread_enabled = 0;

	while (psmx3_wait_thread_busy)
		;
}

static struct fi_ops_wait *psmx3_wait_ops_save;
static struct fi_ops_wait psmx3_wait_ops;

static int psmx3_wait_wait_wait(struct fid_wait *wait_fid, int timeout)
{
	struct ofi_epollfds_event event;
	struct util_wait_fd *wait;
	uint64_t endtime;
	int ret;

	wait = container_of(wait_fid, struct util_wait_fd, util_wait.wait_fid);
	endtime = ofi_timeout_time(timeout);

	while (1) {
		ret = wait->util_wait.wait_try(&wait->util_wait);
		if (ret) {
			return ret == -FI_EAGAIN ? 0 : ret;
		}

		if (ofi_adjust_timeout(endtime, &timeout))
			return -FI_ETIMEDOUT;

		ret = (wait->util_wait.wait_obj == FI_WAIT_FD) ?
		      ofi_epoll_wait(wait->epoll_fd, &event, 1, 100) :
		      ofi_pollfds_wait(wait->pollfds, &event, 1, 100);
		if (ret > 0)
			return FI_SUCCESS;

		if (ret < 0) {
#if ENABLE_DEBUG
			/* ignore interrupts in order to enable debugging */
			if (ret == -FI_EINTR)
				continue;
#endif
			PSMX3_WARN(wait->util_wait.prov, FI_LOG_FABRIC,
				"poll failed\n");
			return ret;
		}
	}
}

DIRECT_FN
STATIC int psmx3_wait_wait(struct fid_wait *wait, int timeout)
{
	struct util_wait *wait_priv;
	struct psmx3_fid_fabric *fabric;
	int err;
	
	wait_priv = container_of(wait, struct util_wait, wait_fid);
	fabric = container_of(wait_priv->fabric, struct psmx3_fid_fabric, util_fabric);

	// special fi_wait support for Intel MPI when FI_PSM3_YIELD_MODE=1
	// When used, fi_wait always delays until next PSM3 internal progress event
	// regardless of how the waitset has been constructed.
	// After fi_wait returns, caller must check for OFI CQ and CNTR events of
	// interest and if none, call fi_wait again.  Those OFI CQ and CNTR checks
	// will do progress_all as needed to complete OFI completion delivery.
	// TBD, in future could perhaps build waitset a specific way, such as
	// FI_WAIT_YIELD entries and then use waitset to figure out which EPs
	// are of interest to the given fi_wait call.  Doing so would need more
	// code in this file as the util_fd_wait code used and the fd_pollset
	// seem opaque and hence don't reveal which their waitset contains.  They
	// don't seem extensible and they have no way to determine if they are empty
	// so we depend on FI_PSM3_YIELD_MODE=1 to disable normal waitset handling
	// and allow this simplified use to meet Intel MPI needs.
	//if (wait_priv->pollset is empty && wait_priv->wait_obj == FI_WAIT_YIELD)
	//	psm3_wait();
	if (psmx3_env.yield_mode) {
		switch (psm3_wait(timeout)) {
		case PSM2_OK:
			return FI_SUCCESS;
		case PSM2_TIMEOUT:
			return -FI_ETIMEDOUT;
		default:
			return -FI_EINVAL;
		}
	}

	psmx3_wait_start_progress(fabric);

	err = psmx3_wait_wait_wait(wait, timeout);

	psmx3_wait_stop_progress();

	return err;
}

DIRECT_FN
int psmx3_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
		   struct fid_wait **waitset)
{
	struct fid_wait *wait;
	int err;
	if (psmx3_env.yield_mode && attr->wait_obj == FI_WAIT_YIELD) {
		// CQ and CNTR won't be allowed to be added to waitset, so
		// we simply create an UNSPEC fd waitset for simplicity here
		// It should not actually be used
		struct fi_wait_attr tmp = *attr;
		tmp.wait_obj = FI_WAIT_UNSPEC;
		err = ofi_wait_fd_open(fabric, &tmp, &wait);
	} else {
		err = ofi_wait_fd_open(fabric, attr, &wait);
	}
	if (err)
		return err;

	psmx3_wait_ops_save = wait->ops;
	psmx3_wait_ops = *psmx3_wait_ops_save;
	psmx3_wait_ops.wait = psmx3_wait_wait;
	wait->ops = &psmx3_wait_ops;

	*waitset = wait;
	return 0;
}

DIRECT_FN
int psmx3_wait_trywait(struct fid_fabric *fabric, struct fid **fids, int count)
{
	struct psmx3_fid_cq *cq_priv;
	struct util_eq *eq;
	struct util_wait *wait;
	int i, ret;

	for (i = 0; i < count; i++) {
		switch (fids[i]->fclass) {
			case FI_CLASS_CQ:
				cq_priv = container_of(fids[i], struct psmx3_fid_cq, cq);
				wait = cq_priv->wait;
				break;
			case FI_CLASS_EQ:
				eq = container_of(fids[i], struct util_eq, eq_fid.fid);
				wait = eq->wait;
				break;
			case FI_CLASS_CNTR:
				return -FI_ENOSYS;
			case FI_CLASS_WAIT:
				wait = container_of(fids[i], struct util_wait, wait_fid.fid);
				break;
			default:
				return -FI_EINVAL;
		}

		ret = wait->wait_try(wait);
		if (ret)
			return ret;
	}
	return 0;
}


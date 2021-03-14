/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
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

#include "psmx.h"

/* It is necessary to have a separate thread making progress in order
 * for the wait functions to succeed. This thread is only created when
 * wait functions are called and. In order to minimize performance
 * impact, it only goes active during te time when wait calls are
 * blocked.
 */
static pthread_t	psmx_wait_thread;
static pthread_mutex_t	psmx_wait_mutex;
static pthread_cond_t	psmx_wait_cond;
static volatile int	psmx_wait_thread_ready = 0;
static volatile int	psmx_wait_thread_enabled = 0;
static volatile int	psmx_wait_thread_busy = 0;

static void *psmx_wait_progress(void *args)
{
	struct psmx_fid_domain *domain = args;

	psmx_wait_thread_ready = 1;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	while (1) {
		pthread_mutex_lock(&psmx_wait_mutex);
		if (!psmx_wait_thread_enabled)
			pthread_cond_wait(&psmx_wait_cond, &psmx_wait_mutex);
		pthread_mutex_unlock(&psmx_wait_mutex);
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

		psmx_wait_thread_busy = 1;
		while (psmx_wait_thread_enabled)
			psmx_progress(domain);

		psmx_wait_thread_busy = 0;

		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	}

	return NULL;
}

static void psmx_wait_start_progress(struct psmx_fid_domain *domain)
{
	pthread_attr_t attr;
	int err;

	if (!domain)
		return;

	if (domain->progress_thread_enabled && domain->progress_thread != pthread_self())
		return;

	if (!psmx_wait_thread) {
		pthread_mutex_init(&psmx_wait_mutex, NULL);
		pthread_cond_init(&psmx_wait_cond, NULL);
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		err = pthread_create(&psmx_wait_thread, &attr, psmx_wait_progress, (void *)domain);
		if (err)
			FI_WARN(&psmx_prov, FI_LOG_EQ,
				"cannot create wait progress thread\n");
		pthread_attr_destroy(&attr);
		while (!psmx_wait_thread_ready)
			;
	}

	psmx_wait_thread_enabled = 1;
	pthread_cond_signal(&psmx_wait_cond);
}

static void psmx_wait_stop_progress(void)
{
	psmx_wait_thread_enabled = 0;

	while (psmx_wait_thread_busy)
		;
}

static struct fi_ops_wait *psmx_wait_ops_save;
static struct fi_ops_wait psmx_wait_ops;

static int psmx_wait_wait(struct fid_wait *wait, int timeout)
{
	struct util_wait *wait_priv;
	struct psmx_fid_fabric *fabric;
	int err;
	
	wait_priv = container_of(wait, struct util_wait, wait_fid);
	fabric = container_of(wait_priv->fabric, struct psmx_fid_fabric,
			      util_fabric);

	psmx_wait_start_progress(fabric->active_domain);

	err = psmx_wait_ops_save->wait(wait, timeout);

	psmx_wait_stop_progress();

	return err;
}

int psmx_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
		   struct fid_wait **waitset)
{
	struct fid_wait *wait;
	int err;

	err = ofi_wait_fd_open(fabric, attr, &wait);
	if (err)
		return err;

	psmx_wait_ops_save = wait->ops;
	psmx_wait_ops = *psmx_wait_ops_save;
	psmx_wait_ops.wait = psmx_wait_wait;
	wait->ops = &psmx_wait_ops;

	*waitset = wait;
	return 0;
}

int psmx_wait_trywait(struct fid_fabric *fabric, struct fid **fids, int count)
{
	struct psmx_fid_cq *cq_priv;
	struct util_eq *eq;
	struct util_wait *wait;
	int i, ret;

	for (i = 0; i < count; i++) {
		switch (fids[i]->fclass) {
			case FI_CLASS_CQ:
				cq_priv = container_of(fids[i], struct psmx_fid_cq, cq);
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


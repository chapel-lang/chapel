/*
 * Copyright (c) 2015-2018 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
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
#include <stdlib.h>
#include <signal.h>
#include "gnix.h"
#include "gnix_wait.h"
#include "gnix_nic.h"
#include "gnix_cm_nic.h"
#include "gnix_eq.h"
/*
 * Gnix wait progress thread declarations for making sure nic progress
 * occurs when inside a gnix_wait call
 */

static pthread_t        gnix_wait_thread;
static pthread_mutex_t  gnix_wait_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t   gnix_wait_cond;
/* This is protected by the wait mutex and is only operated on under the
 * mutex, the mutex protects us from losing wake_ups, from the conditional.
 * This could be changed to an atomic but the variable would still need to
 * be protected under the mutex.
 */
static int              gnix_wait_thread_enabled;
static ofi_atomic32_t   gnix_wait_refcnt;

uint32_t         gnix_wait_thread_sleep_time = 20;

/*
 * It is necessary to have a separate thread making progress in order for the
 * wait functions to succeed. This version of that thread is designed
 * to always make progress so we don't hard stall while sitting on fi_wait.
 */
static void *__gnix_wait_nic_prog_thread_fn(void *the_arg)
{
	int ret = FI_SUCCESS, prev_state;
	struct gnix_nic *nic1, *nic2;
	struct gnix_fid_eq *eq1, *eq2;
	struct gnix_cm_nic *cm_nic1, *cm_nic2;
	sigset_t  sigmask;
	DLIST_HEAD(gnix_nic_prog_list);
	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	/*
	 * temporarily disable cancelability while we set up
	 * some stuff
	 */

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &prev_state);

	/*
	 * help out Cray core-spec, say we're not an app thread
	 * and can be run on core-spec cpus.
	 */
	ret = _gnix_task_is_not_app();
	if (ret)
		GNIX_WARN(FI_LOG_EP_CTRL,
			"_gnix_task_is_not_app call returned %d\n",
			ret);

	/*
	 * block all signals, don't want this thread to catch
	 * signals that may be for app threads
	 */

	memset(&sigmask, 0, sizeof(sigset_t));
	ret = sigfillset(&sigmask);
	if (ret) {
		GNIX_WARN(FI_LOG_EP_CTRL,
		"sigfillset call returned %d\n", ret);
	} else {

		ret = pthread_sigmask(SIG_SETMASK,
					&sigmask, NULL);
		if (ret)
			GNIX_WARN(FI_LOG_EP_CTRL,
			"pthread_sigmask call returned %d\n", ret);
	}

	/*
	 * okay now we're ready to be cancelable.
	 */

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &prev_state);

	while (1) {
		/* Check if we're tearing down. */
		pthread_testcancel();

		/* Wait until we're signaled to poll. */
		pthread_mutex_lock(&gnix_wait_mutex);
		pthread_cleanup_push((void (*)(void *))pthread_mutex_unlock,
					(void *)&gnix_wait_mutex);
		if (!gnix_wait_thread_enabled) {
			pthread_cond_wait(&gnix_wait_cond, &gnix_wait_mutex);
		}

		pthread_cleanup_pop(1);

		/* Progress all EQs. */
		pthread_mutex_lock(&gnix_eq_list_lock);

		dlist_for_each_safe(&gnix_eq_list, eq1, eq2, gnix_fid_eq_list) {
			_gnix_eq_progress(eq1);
		}

		pthread_mutex_unlock(&gnix_eq_list_lock);

		/* Progress all NICs. */
		pthread_mutex_lock(&gnix_nic_list_lock);

		dlist_for_each_safe(&gnix_nic_list, nic1, nic2, gnix_nic_list) {
			dlist_insert_tail(&nic1->gnix_nic_prog_list, &gnix_nic_prog_list);
			_gnix_ref_get(nic1);
		}

		pthread_mutex_unlock(&gnix_nic_list_lock);

		dlist_for_each_safe(&gnix_nic_prog_list, nic1, nic2, gnix_nic_prog_list) {
			_gnix_nic_progress(nic1);
			dlist_remove_init(&nic1->gnix_nic_prog_list);
			_gnix_ref_put(nic1);
		}

		/* Progress all CM NICs. */
		pthread_mutex_lock(&gnix_cm_nic_list_lock);

		dlist_for_each_safe(&gnix_cm_nic_list, cm_nic1, cm_nic2,
				    cm_nic_list) {
			_gnix_cm_nic_progress((void *)cm_nic1);
		}

		pthread_mutex_unlock(&gnix_cm_nic_list_lock);

		usleep(gnix_wait_thread_sleep_time);
	}

	return NULL;
}

static void __gnix_wait_start_progress(void)
{
	int ret;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	pthread_mutex_lock(&gnix_wait_mutex);
	if (!gnix_wait_thread) {
		GNIX_TRACE(FI_LOG_EP_CTRL, "\n");
		pthread_cond_init(&gnix_wait_cond, NULL);
		ofi_atomic_initialize32(&gnix_wait_refcnt, 0);
		ret = _gnix_job_disable_affinity_apply();
		if (ret != 0)
			GNIX_WARN(WAIT_SUB,
				  "_gnix_job_disable call returned %d\n", ret);

		ret = pthread_create(&gnix_wait_thread, NULL,
				     __gnix_wait_nic_prog_thread_fn, NULL);
		if (ret)
			GNIX_WARN(WAIT_SUB,
				  "pthread_create call returned %d\n", ret);
	}
	ofi_atomic_inc32(&gnix_wait_refcnt);
	pthread_mutex_unlock(&gnix_wait_mutex);
}

static void __gnix_wait_stop_progress(void)
{
	int ret;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	pthread_mutex_lock(&gnix_wait_mutex);
	if (gnix_wait_thread) {
		if (ofi_atomic_dec32(&gnix_wait_refcnt) == 0) {
			ret = pthread_cancel(gnix_wait_thread);
			if (ret)
				GNIX_WARN(WAIT_SUB,
					  "pthread_cancel call returned %d\n",
					  ret);

			gnix_wait_thread_enabled++;
			pthread_cond_signal(&gnix_wait_cond);
			pthread_mutex_unlock(&gnix_wait_mutex);
			ret = pthread_join(gnix_wait_thread, NULL);
			if (ret)
				GNIX_WARN(WAIT_SUB,
					  "pthread_join call returned %d\n",
					  ret);

			gnix_wait_thread = 0;
		} else {
			pthread_mutex_unlock(&gnix_wait_mutex);
		}
	} else
		pthread_mutex_unlock(&gnix_wait_mutex);

	return;

}

/*******************************************************************************
 * Forward declarations for FI_OPS_* structures.
 ******************************************************************************/
static struct fi_ops gnix_fi_ops;
static struct fi_ops_wait gnix_wait_ops;

/*******************************************************************************
 * List match functions.
 ******************************************************************************/
static int gnix_match_fid(struct slist_entry *item, const void *fid)
{
	struct gnix_wait_entry *entry;

	entry = container_of(item, struct gnix_wait_entry, entry);

	return (entry->wait_obj == (struct fid *) fid);
}

/*******************************************************************************
 * Exposed helper functions.
 ******************************************************************************/
int _gnix_wait_set_add(struct fid_wait *wait, struct fid *wait_obj)
{
	struct gnix_fid_wait *wait_priv;
	struct gnix_wait_entry *wait_entry;

	GNIX_TRACE(WAIT_SUB, "\n");

	wait_entry = calloc(1, sizeof(*wait_entry));
	if (!wait_entry) {
		GNIX_WARN(WAIT_SUB,
			  "failed to allocate memory for wait entry.\n");
		return -FI_ENOMEM;
	}

	wait_priv = container_of(wait, struct gnix_fid_wait, wait.fid);

	wait_entry->wait_obj = wait_obj;

	gnix_slist_insert_tail(&wait_entry->entry, &wait_priv->set);

	return FI_SUCCESS;
}

int _gnix_wait_set_remove(struct fid_wait *wait, struct fid *wait_obj)
{
	struct gnix_fid_wait *wait_priv;
	struct gnix_wait_entry *wait_entry;
	struct slist_entry *found;

	GNIX_TRACE(WAIT_SUB, "\n");

	wait_priv = container_of(wait, struct gnix_fid_wait, wait.fid);

	found = slist_remove_first_match(&wait_priv->set, gnix_match_fid,
					 wait_obj);

	if (found) {
		wait_entry = container_of(found, struct gnix_wait_entry,
					  entry);
		free(wait_entry);

		return FI_SUCCESS;
	}

	return -FI_EINVAL;
}

int _gnix_get_wait_obj(struct fid_wait *wait, void *arg)
{
	struct fi_mutex_cond mutex_cond;
	struct gnix_fid_wait *wait_priv;
	size_t copy_size;
	const void *src;

	GNIX_TRACE(WAIT_SUB, "\n");

	if (!wait || !arg)
		return -FI_EINVAL;

	wait_priv = container_of(wait, struct gnix_fid_wait, wait);

	switch (wait_priv->type) {
	case FI_WAIT_FD:
		copy_size = sizeof(wait_priv->fd[WAIT_READ]);
		src = &wait_priv->fd[WAIT_READ];
		break;
	case FI_WAIT_MUTEX_COND:
		mutex_cond.mutex = &wait_priv->mutex;
		mutex_cond.cond = &wait_priv->cond;

		copy_size = sizeof(mutex_cond);
		src = &mutex_cond;
		break;
	default:
		GNIX_WARN(WAIT_SUB, "wait type: %d not supported.\n",
			  wait_priv->type);
		return -FI_EINVAL;
	}

	memcpy(arg, src, copy_size);

	return FI_SUCCESS;
}

void _gnix_signal_wait_obj(struct fid_wait *wait)
{
	static char msg = 'g';
	size_t len = sizeof(msg);
	struct gnix_fid_wait *wait_priv;

	wait_priv = container_of(wait, struct gnix_fid_wait, wait);

	switch (wait_priv->type) {
	case FI_WAIT_UNSPEC:
		GNIX_TRACE(WAIT_SUB,
			   "The Read FD is %d Write is %d\n",
			   wait_priv->fd[WAIT_READ],
			   wait_priv->fd[WAIT_WRITE]);
		/* This is a non-blocking write as the fd could become full */
		write(wait_priv->fd[WAIT_WRITE], &msg, len);
		break;
	default:
		GNIX_WARN(WAIT_SUB,
			 "error signaling wait object: type: %d not supported.\n",
			 wait_priv->type);
		return;
	}
}

/*******************************************************************************
 * Internal helper functions.
 ******************************************************************************/
static int gnix_verify_wait_attr(struct fi_wait_attr *attr)
{
	GNIX_TRACE(WAIT_SUB, "\n");

	if (!attr || attr->flags)
		return -FI_EINVAL;

	switch (attr->wait_obj) {
	case FI_WAIT_UNSPEC:
		attr->wait_obj = FI_WAIT_UNSPEC;
		break;
	default:
		GNIX_WARN(WAIT_SUB, "wait type: %d not supported.\n",
			  attr->wait_obj);
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

static int gnix_init_wait_obj(struct gnix_fid_wait *wait, enum fi_wait_obj type)
{
	GNIX_TRACE(WAIT_SUB, "\n");

	wait->type = type;

	switch (type) {
	case FI_WAIT_UNSPEC:
		if (socketpair(AF_LOCAL, SOCK_STREAM, 0, wait->fd))
			goto err;

		if (fi_fd_nonblock(wait->fd[WAIT_READ]))
			goto cleanup;

		if (fi_fd_nonblock(wait->fd[WAIT_WRITE]))
			goto cleanup;

		break;
	default:
		GNIX_WARN(WAIT_SUB, "Invalid wait type: %d\n",
			 type);
		return -FI_EINVAL;
	}

	return FI_SUCCESS;

cleanup:
	close(wait->fd[WAIT_READ]);
	close(wait->fd[WAIT_WRITE]);
err:
	GNIX_WARN(WAIT_SUB, "%s\n", strerror(errno));
	return -FI_EOTHER;
}

/*******************************************************************************
 * API Functionality.
 ******************************************************************************/
static int gnix_wait_control(struct fid *wait, int command, void *arg)
{
/*
	struct fid_wait *wait_fid_priv;

	GNIX_TRACE(WAIT_SUB, "\n");

	wait_fid_priv = container_of(wait, struct fid_wait, fid);
*/

	switch (command) {
	case FI_GETWAIT:
		return -FI_ENOSYS;
	default:
		return -FI_EINVAL;
	}
}

/**
 * Waits on a wait set until one or more of it's underlying objects is signaled.
 *
 * @param[in] wait	the wait object set
 * @param[in] timeout	time to wait for a signal, in milliseconds
 *
 * @return FI_SUCCESS	upon successfully waiting
 * @return -FI_ERRNO	upon failure
 * @return -FI_ENOSYS	if this operation is not supported
 */
DIRECT_FN int gnix_wait_wait(struct fid_wait *wait, int timeout)
{
	int err = 0, ret;
	char c;
	struct gnix_fid_wait *wait_priv;

	GNIX_TRACE(WAIT_SUB, "\n");

	wait_priv = container_of(wait, struct gnix_fid_wait, wait.fid);
	switch (wait_priv->type) {
	case FI_WAIT_UNSPEC:
		pthread_mutex_lock(&gnix_wait_mutex);
		gnix_wait_thread_enabled++;
		pthread_cond_signal(&gnix_wait_cond);
		pthread_mutex_unlock(&gnix_wait_mutex);
		GNIX_DEBUG(WAIT_SUB,
			   "Calling fi_poll_fd %d timeout %d\n",
			   wait_priv->fd[WAIT_READ],
			   timeout);
		err = fi_poll_fd(wait_priv->fd[WAIT_READ], timeout);
		GNIX_DEBUG(WAIT_SUB, "Return code from poll was %d\n", err);
		if (err == 0) {
			err = -FI_ETIMEDOUT;
		} else {
			while (err > 0) {
				ret = ofi_read_socket(wait_priv->fd[WAIT_READ],
						      &c,
						      1);
				GNIX_DEBUG(WAIT_SUB, "ret is %d C is %c\n",
					  ret,
					  c);
				if (ret != 1) {
					GNIX_ERR(WAIT_SUB,
						 "failed to read wait_fd\n");
					err = 0;
					break;
				}
				err--;
			}
		}
		break;
	default:
		GNIX_WARN(WAIT_SUB, "Invalid wait object type\n");
		return -FI_EINVAL;
	}
	pthread_mutex_lock(&gnix_wait_mutex);
	gnix_wait_thread_enabled--;
	pthread_mutex_unlock(&gnix_wait_mutex);
	return err;
}

int gnix_wait_close(struct fid *wait)
{
	struct gnix_fid_wait *wait_priv;

	GNIX_TRACE(WAIT_SUB, "\n");

	wait_priv = container_of(wait, struct gnix_fid_wait, wait.fid);

	if (!slist_empty(&wait_priv->set)) {
		GNIX_WARN(WAIT_SUB,
			  "resources still connected to wait set.\n");
		return -FI_EBUSY;
	}

	if (wait_priv->type == FI_WAIT_FD) {
		close(wait_priv->fd[WAIT_READ]);
		close(wait_priv->fd[WAIT_WRITE]);
	}

	_gnix_ref_put(wait_priv->fabric);

	free(wait_priv);

	__gnix_wait_stop_progress();
	return FI_SUCCESS;
}

DIRECT_FN int gnix_wait_open(struct fid_fabric *fabric,
			     struct fi_wait_attr *attr,
			     struct fid_wait **waitset)
{
	struct gnix_fid_fabric *fab_priv;
	struct gnix_fid_wait *wait_priv;
	int ret = FI_SUCCESS;

	GNIX_TRACE(WAIT_SUB, "\n");

	ret = gnix_verify_wait_attr(attr);
	if (ret)
		goto err;

	fab_priv = container_of(fabric, struct gnix_fid_fabric, fab_fid);

	wait_priv = calloc(1, sizeof(*wait_priv));
	if (!wait_priv) {
		GNIX_WARN(WAIT_SUB,
			 "failed to allocate memory for wait set.\n");
		ret = -FI_ENOMEM;
		goto err;
	}

	ret = gnix_init_wait_obj(wait_priv, attr->wait_obj);
	if (ret)
		goto cleanup;

	slist_init(&wait_priv->set);

	wait_priv->wait.fid.fclass = FI_CLASS_WAIT;
	wait_priv->wait.fid.ops = &gnix_fi_ops;
	wait_priv->wait.ops = &gnix_wait_ops;

	wait_priv->fabric = fab_priv;

	_gnix_ref_get(fab_priv);
	*waitset = &wait_priv->wait;

	__gnix_wait_start_progress();
	return ret;

cleanup:
	free(wait_priv);
err:
	return ret;
}

/*******************************************************************************
 * FI_OPS_* data structures.
 ******************************************************************************/
static struct fi_ops gnix_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = gnix_wait_close,
	.bind = fi_no_bind,
	.control = gnix_wait_control,
	.ops_open = fi_no_ops_open
};

static struct fi_ops_wait gnix_wait_ops = {
	.size = sizeof(struct fi_ops_wait),
	.wait = gnix_wait_wait
};

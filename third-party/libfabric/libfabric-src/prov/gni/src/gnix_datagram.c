/*
 * Copyright (c) 2015-2016 Cray Inc.  All rights reserved.
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
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

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <signal.h>

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_errno.h>

#include <rdma/providers/fi_prov.h>

#include "gnix.h"
#include "gnix_datagram.h"
#include "gnix_util.h"
#include "gnix_cm_nic.h"
#include "gnix_nic.h"


/*******************************************************************************
 * Helper functions.
 ******************************************************************************/

/*
 * this function is intended to be invoked as an argument to pthread_create,
 */
static void *_gnix_dgram_prog_thread_fn(void *the_arg)
{
	int ret = FI_SUCCESS, prev_state;
	struct gnix_dgram_hndl *the_hndl = (struct gnix_dgram_hndl *)the_arg;
	sigset_t  sigmask;

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
		"_gnix_task_is_not_app call returned %d\n", ret);

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

	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

retry:
	ret = _gnix_dgram_poll(the_hndl, GNIX_DGRAM_BLOCK);
	if ((ret == -FI_ETIMEDOUT) || (ret == FI_SUCCESS))
		goto retry;

	GNIX_WARN(FI_LOG_EP_CTRL,
		"_gnix_dgram_poll returned %s\n", fi_strerror(-ret));

	/*
	 * TODO: need to be able to enqueue events on to the
	 * ep associated with the cm_nic.
	 */
	return NULL;
}

/*******************************************************************************
 * API function implementations.
 ******************************************************************************/

/*
 * function to pack data into datagram in/out buffers.
 * On success, returns number of bytes packed in to the buffer,
 * otherwise -FI errno.
 */
ssize_t _gnix_dgram_pack_buf(struct gnix_datagram *d, enum gnix_dgram_buf buf,
			 void *data, uint32_t nbytes)
{
	char *dptr;
	uint32_t index;

	assert(d != NULL);
	if (buf == GNIX_DGRAM_IN_BUF) {
		index = d->w_index_in_buf;
		dptr = &d->dgram_in_buf[index];
	} else {
		index = d->w_index_out_buf;
		dptr = &d->dgram_out_buf[index];
	}

	/*
	 * make sure there's room
	 */
	if ((index + nbytes) > GNI_DATAGRAM_MAXSIZE)
		return -FI_ENOSPC;

	memcpy(dptr, data, nbytes);

	if (buf == GNIX_DGRAM_IN_BUF)
		d->w_index_in_buf += nbytes;
	else
		d->w_index_out_buf += nbytes;

	return nbytes;
}


/*
 * function to unpack data from datagram in/out buffers.
 * On success, returns number of bytes unpacked,
 * otherwise -FI errno.
 */
ssize_t _gnix_dgram_unpack_buf(struct gnix_datagram *d, enum gnix_dgram_buf buf,
			   void *data, uint32_t nbytes)
{
	char *dptr;
	uint32_t index, bytes_left;

	assert(d != NULL);
	if (buf == GNIX_DGRAM_IN_BUF) {
		index = d->r_index_in_buf;
		dptr = &d->dgram_in_buf[index];
	} else {
		index = d->r_index_out_buf;
		dptr = &d->dgram_out_buf[index];
	}

	/*
	 * only copy out up to GNI_DATAGRAM_MAXSIZE
	 */

	bytes_left = GNI_DATAGRAM_MAXSIZE - index;

	nbytes = (nbytes > bytes_left) ? bytes_left : nbytes;

	memcpy(data, dptr, nbytes);

	if (buf == GNIX_DGRAM_IN_BUF)
		d->r_index_in_buf += nbytes;
	else
		d->r_index_out_buf += nbytes;

	return nbytes;
}

/*
 * function to rewind the internal pointers to
 * datagram in/out buffers.
 */
int _gnix_dgram_rewind_buf(struct gnix_datagram *d, enum gnix_dgram_buf buf)
{
	assert(d != NULL);
	if (buf == GNIX_DGRAM_IN_BUF) {
		d->r_index_in_buf = 0;
		d->w_index_in_buf = 0;
	} else {
		d->r_index_out_buf = 0;
		d->w_index_out_buf = 0;
	}
	return FI_SUCCESS;
}

int _gnix_dgram_alloc(struct gnix_dgram_hndl *hndl, enum gnix_dgram_type type,
			struct gnix_datagram **d_ptr)
{
	int ret = -FI_EAGAIN;
	struct gnix_datagram *d = NULL;
	struct dlist_entry *the_free_list;
	struct dlist_entry *the_active_list;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	fastlock_acquire(&hndl->lock);

	if (type == GNIX_DGRAM_WC) {
		the_free_list = &hndl->wc_dgram_free_list;
		the_active_list = &hndl->wc_dgram_active_list;
	} else {
		the_free_list = &hndl->bnd_dgram_free_list;
		the_active_list = &hndl->bnd_dgram_active_list;
	}

	if (!dlist_empty(the_free_list)) {
		d = dlist_first_entry(the_free_list, struct gnix_datagram,
				      list);
		if (d != NULL) {
			dlist_remove_init(&d->list);
			dlist_insert_head(&d->list, the_active_list);
			d->type = type;
			ret = FI_SUCCESS;
		}

	}

	fastlock_release(&hndl->lock);

	if (d != NULL) {
		d->r_index_in_buf = 0;
		d->w_index_in_buf = 0;
		d->w_index_in_buf = 0;
		d->w_index_out_buf = 0;
	}

	*d_ptr = d;
	return ret;
}

int _gnix_dgram_free(struct gnix_datagram *d)
{
	int ret = FI_SUCCESS;
	gni_return_t status;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (d->type == GNIX_DGRAM_BND) {
		status = GNI_EpUnbind(d->gni_ep);
		if (status != GNI_RC_SUCCESS) {
			/* TODO: have to handle this */
			GNIX_FATAL(FI_LOG_EP_CTRL,
				   "GNI_EpUnbind returned %s (ep=%p)\n",
				   gni_err_str[status], d->gni_ep);
		}
	}

	fastlock_acquire(&d->d_hndl->lock);
	dlist_remove_init(&d->list);
	d->state = GNIX_DGRAM_STATE_FREE;
	dlist_insert_head(&d->list, d->free_list_head);
	fastlock_release(&d->d_hndl->lock);
	return ret;
}

int _gnix_dgram_wc_post(struct gnix_datagram *d)
{
	int ret = FI_SUCCESS;
	gni_return_t status;
	struct gnix_nic *nic = d->cm_nic->nic;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	COND_ACQUIRE(nic->requires_lock, &nic->lock);
	status = GNI_EpPostDataWId(d->gni_ep,
				   d->dgram_in_buf,
				   GNI_DATAGRAM_MAXSIZE,
				   d->dgram_out_buf,
				   GNI_DATAGRAM_MAXSIZE,
				   (uint64_t)d);
	if (status != GNI_RC_SUCCESS) {
		ret = gnixu_to_fi_errno(status);
	} else {
		/*
		 * datagram is active now, listening
		 */
		d->state = GNIX_DGRAM_STATE_ACTIVE;
	}
	COND_RELEASE(nic->requires_lock, &nic->lock);

	return ret;
}

int _gnix_dgram_bnd_post(struct gnix_datagram *d)
{
	gni_return_t status = GNI_RC_SUCCESS;
	int ret = FI_SUCCESS;
	struct gnix_nic *nic = d->cm_nic->nic;
	int post = 1;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	/*
	 * bind the datagram ep
	 */

	status = GNI_EpBind(d->gni_ep,
			    d->target_addr.device_addr,
			    d->target_addr.cdm_id);
	if (status != GNI_RC_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			"GNI_EpBind returned %s\n", gni_err_str[status]);
		ret = gnixu_to_fi_errno(status);
		goto err;
	}

	COND_ACQUIRE(nic->requires_lock, &nic->lock);
	if (d->pre_post_clbk_fn != NULL) {
		ret = d->pre_post_clbk_fn(d, &post);
		if (ret != FI_SUCCESS)
			GNIX_WARN(FI_LOG_EP_CTRL,
				"pre_post_callback_fn: %d\n",
				ret);
	}

	if (post) {
		/*
		 * if we get GNI_RC_ERROR_RESOURCE status return from
		 * GNI_EpPostDataWId  that means that either a previously posted
		 * wildcard datagram has matched up with an incoming
		 * bound datagram or we have a previously posted bound
		 * datagram whose transfer to the target node has
		 * not yet completed.  Don't treat this case as an error.
		 */
		status = GNI_EpPostDataWId(d->gni_ep,
					   d->dgram_in_buf,
					   GNI_DATAGRAM_MAXSIZE,
					   d->dgram_out_buf,
					   GNI_DATAGRAM_MAXSIZE,
					   (uint64_t)d);
		if (d->post_post_clbk_fn != NULL) {
			ret = d->post_post_clbk_fn(d, status);
			if (ret != FI_SUCCESS)
				GNIX_WARN(FI_LOG_EP_CTRL,
				"post_post_callback_fn: %d\n",
				ret);
		}
	}

	COND_RELEASE(nic->requires_lock, &nic->lock);

	if (post) {
		if ((status != GNI_RC_SUCCESS) &&
			(status != GNI_RC_ERROR_RESOURCE)) {
				GNIX_WARN(FI_LOG_EP_CTRL,
				    "GNI_EpPostDataWId returned %s\n",
				     gni_err_str[status]);
				ret = gnixu_to_fi_errno(status);
				goto err;
		}

		if (status == GNI_RC_SUCCESS) {
			/*
			 * datagram is active now, connecting
			 */
			d->state = GNIX_DGRAM_STATE_ACTIVE;
		} else {
			ret = -FI_EBUSY;
		}
	}

err:
	return ret;
}

int  _gnix_dgram_poll(struct gnix_dgram_hndl *hndl,
			enum gnix_dgram_poll_type type)
{
	int ret = FI_SUCCESS;
	gni_return_t status;
	gni_post_state_t post_state = GNI_POST_PENDING;
	uint32_t responding_remote_id;
	uint32_t timeout = -1;
	unsigned int responding_remote_addr;
	struct gnix_datagram *dg_ptr;
	uint64_t datagram_id = 0UL;
	struct gnix_cm_nic *cm_nic = NULL;
	struct gnix_nic *nic = NULL;
	struct gnix_address responding_addr;

	cm_nic = hndl->cm_nic;
	assert(cm_nic != NULL);
	nic = cm_nic->nic;
	assert(nic != NULL);

	if (type == GNIX_DGRAM_BLOCK) {
		if (hndl->timeout_needed &&
			(hndl->timeout_needed(hndl->timeout_data) == true))
				timeout = hndl->timeout;

		status = GNI_PostdataProbeWaitById(nic->gni_nic_hndl,
						   timeout,
						   &datagram_id);
		if ((status != GNI_RC_SUCCESS) &&
			(status  != GNI_RC_TIMEOUT)) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				"GNI_PostdataProbeWaitById returned %s\n",
					gni_err_str[status]);
			ret = gnixu_to_fi_errno(status);
			goto err;
		}
	} else {
		status = GNI_PostDataProbeById(nic->gni_nic_hndl,
						   &datagram_id);
		if ((status != GNI_RC_SUCCESS) &&
			(status  != GNI_RC_NO_MATCH)) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				"GNI_PostdataProbeById returned %s\n",
					gni_err_str[status]);
			ret = gnixu_to_fi_errno(status);
			goto err;
		}
	}

	switch (status) {
	case GNI_RC_SUCCESS:
		dg_ptr = (struct gnix_datagram *)datagram_id;
		assert(dg_ptr != NULL);

		/*
		 * do need to take lock here
		 */
		COND_ACQUIRE(nic->requires_lock, &nic->lock);

		status = GNI_EpPostDataTestById(dg_ptr->gni_ep,
						datagram_id,
						&post_state,
						&responding_remote_addr,
						&responding_remote_id);
		if ((status != GNI_RC_SUCCESS) &&
			(status !=GNI_RC_NO_MATCH)) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				"GNI_EpPostDataTestById:  %s\n",
					gni_err_str[status]);
			ret = gnixu_to_fi_errno(status);
			COND_RELEASE(nic->requires_lock, &nic->lock);
			goto err;
		} else {
			if ((status == GNI_RC_SUCCESS) &&
			     (dg_ptr->state != GNIX_DGRAM_STATE_ACTIVE)) {
				GNIX_DEBUG(FI_LOG_EP_CTRL,
					"GNI_EpPostDataTestById ",
					"returned success but dgram not active\n");
			}
		}

		COND_RELEASE(nic->requires_lock, &nic->lock);

		/*
		 * no match is okay, it means another thread
		 * won the race to get this datagram
		 */

		if (status == GNI_RC_NO_MATCH) {
			ret = FI_SUCCESS;
			goto err;
		}

		/*
		 * pass COMPLETED and error post state cases to
		 * callback function if present.  If a callback funciton
		 * is not present, the error states set ret to -FI_EIO.
		 *
		 * TODO should we also pass pending,remote_data states to
		 * the callback?  maybe useful for debugging weird
		 * datagram problems?
		 */
		switch (post_state) {
		case GNI_POST_TIMEOUT:
		case GNI_POST_TERMINATED:
		case GNI_POST_ERROR:
			ret = -FI_EIO;
			break;
		case GNI_POST_COMPLETED:
			if (dg_ptr->callback_fn != NULL) {
				responding_addr.device_addr =
					responding_remote_addr;
				responding_addr.cdm_id =
					responding_remote_id;
				ret = dg_ptr->callback_fn((void *)datagram_id,
							responding_addr,
							post_state);
			}
			break;
		case GNI_POST_PENDING:
		case GNI_POST_REMOTE_DATA:
			break;
		default:
			GNIX_FATAL(FI_LOG_EP_CTRL, "Invalid post_state: %d\n",
				   post_state);
			break;
		}
		break;
	case GNI_RC_TIMEOUT:
		/* call progress function */
		if (hndl->timeout_progress)
			hndl->timeout_progress(hndl->timeout_data);
		break;
	case GNI_RC_NO_MATCH:
		break;
	default:
		/* an error */
		break;
	}

err:
	return ret;
}

int _gnix_dgram_hndl_alloc(struct gnix_cm_nic *cm_nic,
			   enum fi_progress progress,
			   const struct gnix_dgram_hndl_attr *attr,
			   struct gnix_dgram_hndl **hndl_ptr)
{
	int i, ret = FI_SUCCESS;
	int n_dgrams_tot;
	struct gnix_datagram *dgram_base = NULL, *dg_ptr;
	struct gnix_dgram_hndl *the_hndl = NULL;
	struct gnix_fid_domain *dom = cm_nic->domain;
	struct gnix_fid_fabric *fabric = NULL;
	struct gnix_nic *nic;
	gni_return_t status;
	uint32_t num_corespec_cpus = 0;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	nic = cm_nic->nic;

	if (dom == NULL)
		return -FI_EINVAL;

	fabric = dom->fabric;

	the_hndl = calloc(1, sizeof(struct gnix_dgram_hndl));
	if (the_hndl == NULL) {
		ret = -FI_ENOMEM;
		goto err;
	}

	the_hndl->cm_nic = cm_nic;

	dlist_init(&the_hndl->bnd_dgram_free_list);
	dlist_init(&the_hndl->bnd_dgram_active_list);

	dlist_init(&the_hndl->wc_dgram_free_list);
	dlist_init(&the_hndl->wc_dgram_active_list);

	the_hndl->timeout = -1;

	/*
	 * inherit some stuff from the fabric object being
	 * used to open the domain which will use this cm nic.
	 */

	the_hndl->n_dgrams = fabric->n_bnd_dgrams;
	the_hndl->n_wc_dgrams = fabric->n_wc_dgrams;
	fastlock_init(&the_hndl->lock);

	n_dgrams_tot = the_hndl->n_dgrams + the_hndl->n_wc_dgrams;

	/*
	 * set up the free lists for datagrams
	 */

	dgram_base = calloc(n_dgrams_tot,
			    sizeof(struct gnix_datagram));
	if (dgram_base == NULL) {
		ret = -FI_ENOMEM;
		goto err;
	}

	dg_ptr = dgram_base;

	/*
	 * first build up the list for connection requests
	 */

	for (i = 0; i < fabric->n_bnd_dgrams; i++, dg_ptr++) {
		dg_ptr->d_hndl = the_hndl;
		dg_ptr->cm_nic = cm_nic;
		status = GNI_EpCreate(nic->gni_nic_hndl,
					NULL,
					&dg_ptr->gni_ep);
		if (status != GNI_RC_SUCCESS) {
			ret = gnixu_to_fi_errno(status);
			goto err;
		}
		dlist_node_init(&dg_ptr->list);
		dlist_insert_head(&dg_ptr->list,
				  &the_hndl->bnd_dgram_free_list);
		dg_ptr->free_list_head = &the_hndl->bnd_dgram_free_list;
	}

	/*
	 * now the wild card (WC) dgrams
	 */

	for (i = 0; i < fabric->n_wc_dgrams; i++, dg_ptr++) {
		dg_ptr->d_hndl = the_hndl;
		dg_ptr->cm_nic = cm_nic;
		status = GNI_EpCreate(nic->gni_nic_hndl,
					NULL,
					&dg_ptr->gni_ep);
		if (status != GNI_RC_SUCCESS) {
			ret = gnixu_to_fi_errno(status);
			goto err;
		}
		dlist_node_init(&dg_ptr->list);
		dlist_insert_head(&dg_ptr->list, &the_hndl->wc_dgram_free_list);
		dg_ptr->free_list_head = &the_hndl->wc_dgram_free_list;
	}

	/*
	 * check the progress model, if FI_PROGRESS_AUTO, fire off
	 * a progress thread
	 */

	if (progress == FI_PROGRESS_AUTO) {

		if (attr != NULL) {
			the_hndl->timeout_needed = attr->timeout_needed;
			the_hndl->timeout_progress = attr->timeout_progress;
			the_hndl->timeout_data = attr->timeout_data;
			the_hndl->timeout = attr->timeout;
		}

		/*
		 * tell CLE job container that next thread should be
		 * runnable anywhere in the cpuset, don't treat as
		 * an error if one is returned, may have perf issues
		 * though...
		 */

		ret = _gnix_get_num_corespec_cpus(&num_corespec_cpus);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "failed to get num corespec cpus\n");
		}

		if (num_corespec_cpus > 0) {
			ret = _gnix_job_disable_affinity_apply();
		} else {
			ret = _gnix_job_enable_unassigned_cpus();
		}
		if (ret != 0)
			GNIX_WARN(FI_LOG_EP_CTRL,
			"disable_affinity/unassigned_cpus call returned %d\n",
			ret);

		ret = pthread_create(&the_hndl->progress_thread,
				     NULL,
				     _gnix_dgram_prog_thread_fn,
				     (void *)the_hndl);
		if (ret) {
			GNIX_WARN(FI_LOG_EP_CTRL,
			"pthread_ceate  call returned %d\n", ret);
			goto err1;
		}
	}

	the_hndl->dgram_base = dgram_base;

	*hndl_ptr = the_hndl;

	return ret;

err1:

err:
	dg_ptr = dgram_base;
	if (dg_ptr) {

		for (i = 0; i < n_dgrams_tot; i++, dg_ptr++) {
			if (dg_ptr->gni_ep != NULL)
				GNI_EpDestroy(dg_ptr->gni_ep);
		}
		free(dgram_base);
	}
	if (the_hndl)
		free(the_hndl);
	return ret;
}

int _gnix_dgram_hndl_free(struct gnix_dgram_hndl *the_hndl)
{
	int i;
	int n_dgrams;
	int ret = FI_SUCCESS;
	struct gnix_datagram *p, *next, *dg_ptr;
	gni_return_t status;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (the_hndl->dgram_base == NULL) {
		ret = -FI_EINVAL;
		goto err;
	}

	/*
	 * cancel any active datagrams - GNI_RC_NO_MATCH is okay.
	 */
	dlist_for_each_safe(&the_hndl->bnd_dgram_active_list, p, next, list) {
		dg_ptr = p;
		if (dg_ptr->state != GNIX_DGRAM_STATE_FREE) {
			status = GNI_EpPostDataCancel(dg_ptr->gni_ep);
			if ((status != GNI_RC_SUCCESS) &&
					(status != GNI_RC_NO_MATCH)) {
				ret = gnixu_to_fi_errno(status);
				goto err;
			}
		}
		dlist_remove_init(&dg_ptr->list);
	}

	dlist_for_each_safe(&the_hndl->wc_dgram_active_list, p, next, list) {
		dg_ptr = p;
		if (dg_ptr->state == GNIX_DGRAM_STATE_FREE) {
			status = GNI_EpPostDataCancel(dg_ptr->gni_ep);
			if ((status != GNI_RC_SUCCESS) &&
					(status != GNI_RC_NO_MATCH)) {
				ret = gnixu_to_fi_errno(status);
				goto err;
			}
		}
		dlist_remove_init(&dg_ptr->list);
	}

	/*
	 * destroy all the endpoints
	 */

	n_dgrams = the_hndl->n_dgrams + the_hndl->n_wc_dgrams;
	dg_ptr = the_hndl->dgram_base;

	for (i = 0; i < n_dgrams; i++, dg_ptr++) {
		if (dg_ptr->gni_ep != NULL)
			GNI_EpDestroy(dg_ptr->gni_ep);
	}

	/*
	 * cancel the progress thread, if any
	 */

	if (the_hndl->progress_thread) {

		ret = pthread_cancel(the_hndl->progress_thread);
		if ((ret != 0) && (ret != ESRCH)) {
			GNIX_WARN(FI_LOG_EP_CTRL,
			"pthread_cancel returned %d\n", ret);
			goto err;
		}

		ret = pthread_join(the_hndl->progress_thread,
				   NULL);
		if ((ret != 0) && (ret != ESRCH)) {
			GNIX_WARN(FI_LOG_EP_CTRL,
			"pthread_join returned %d\n", ret);
			goto err;
		}

		GNIX_INFO(FI_LOG_EP_CTRL, "pthread_join returned %d\n", ret);
	}
err:
	if (ret != FI_SUCCESS)
		GNIX_INFO(FI_LOG_EP_CTRL, "returning error %d\n", ret);
	free(the_hndl->dgram_base);
	free(the_hndl);

	return ret;
}

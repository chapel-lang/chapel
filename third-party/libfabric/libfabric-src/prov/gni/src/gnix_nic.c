/*
 * Copyright (c) 2015-2018 Cray Inc. All rights reserved.
 * Copyright (c) 2015-2018 Los Alamos National Security, LLC.
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

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/mman.h>
#include <signal.h>

#include "gnix.h"
#include "gnix_nic.h"
#include "gnix_cm_nic.h"
#include "gnix_vc.h"
#include "gnix_mbox_allocator.h"
#include "gnix_util.h"
#include "fi_ext_gni.h"

/*
 * TODO: make this a domain parameter
 */
#define GNIX_VC_FL_MIN_SIZE 128
#define GNIX_VC_FL_INIT_REFILL_SIZE 10

static int gnix_nics_per_ptag[GNI_PTAG_MAX];
struct dlist_entry gnix_nic_list_ptag[GNI_PTAG_MAX];
DLIST_HEAD(gnix_nic_list);
pthread_mutex_t gnix_nic_list_lock = PTHREAD_MUTEX_INITIALIZER;

/*
 * globals
 */

uint32_t gnix_max_nics_per_ptag = GNIX_DEF_MAX_NICS_PER_PTAG;

/*
 * local variables
 */

static struct gnix_nic_attr default_attr = {
		.gni_cdm_hndl        = NULL,
		.gni_nic_hndl        = NULL
};

/*******************************************************************************
 * Helper functions.
 ******************************************************************************/

/*
 * this function is intended to be invoked as an argument to pthread_create,
 */
static void *__gnix_nic_prog_thread_fn(void *the_arg)
{
	int ret = FI_SUCCESS, prev_state;
	int retry = 0;
	uint32_t which;
	struct gnix_nic *nic = (struct gnix_nic *)the_arg;
	sigset_t  sigmask;
	gni_cq_handle_t cqv[2];
	gni_return_t status;
	gni_cq_entry_t cqe;

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

	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	cqv[0] = nic->tx_cq_blk;
	cqv[1] = nic->rx_cq_blk;

try_again:
	status = GNI_CqVectorMonitor(cqv,
				     2,
				     -1,
				     &which);

	switch (status) {
	case GNI_RC_SUCCESS:

		/*
		 * first dequeue RX CQEs
		 */
		if (nic->rx_cq_blk != nic->rx_cq && which == 1) {
			do {
				status = GNI_CqGetEvent(nic->rx_cq_blk,
							&cqe);
			} while (status == GNI_RC_SUCCESS);
		}
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &prev_state);
		_gnix_nic_progress(nic);
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &prev_state);
		retry = 1;
		break;
	case GNI_RC_TIMEOUT:
	case GNI_RC_NOT_DONE:
        /* Invalid state indicates call interrupted by signal using various tools */
	case GNI_RC_INVALID_STATE:
		retry = 1;
		break;
	case GNI_RC_INVALID_PARAM:
	case GNI_RC_ERROR_RESOURCE:
	case GNI_RC_ERROR_NOMEM:
		retry = 0;
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "GNI_CqGetEvent returned %s\n",
			  gni_err_str[status]);
		break;
	default:
		retry = 0;
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "GNI_CqGetEvent returned unexpected code %s\n",
			  gni_err_str[status]);
		break;
	}

	if (retry)
		goto try_again;

	return NULL;
}

/*
 * setup memory registration for remote GNI_PostCqWrite's to target
 */

static int __nic_setup_irq_cq(struct gnix_nic *nic)
{
	int ret = FI_SUCCESS;
	size_t len;
	gni_return_t status;
	int fd = -1;
	void *mmap_addr;
	int vmdh_index = -1;
	int flags = GNI_MEM_READWRITE;
	struct gnix_auth_key *info;
	struct fi_gni_auth_key key;

	len = (size_t)sysconf(_SC_PAGESIZE);

	mmap_addr = mmap(NULL, len, PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANON, fd, 0);
	if (mmap_addr == MAP_FAILED) {
		GNIX_WARN(FI_LOG_EP_CTRL, "mmap failed - %s\n",
			strerror(errno));
		ret = -errno;
		goto err;
	}

	nic->irq_mmap_addr = mmap_addr;
	nic->irq_mmap_len = len;

	/* On some systems, the page may not be zero'd from first use.
		 Memset it here */
	memset(mmap_addr, 0x0, len);

	if (nic->using_vmdh) {
		key.type = GNIX_AKT_RAW;
		key.raw.protection_key = nic->cookie;

		info = _gnix_auth_key_lookup((uint8_t *) &key, sizeof(key));
		assert(info);

		if (!nic->mdd_resources_set) {
			/* check to see if the ptag registration limit was set
			   yet or not -- becomes read-only after success */
			ret = _gnix_auth_key_enable(info);
			if (ret != FI_SUCCESS && ret != -FI_EBUSY) {
				GNIX_WARN(FI_LOG_DOMAIN,
					"failed to enable authorization key, "
					"unexpected error rc=%d\n", ret);
			}

			status = GNI_SetMddResources(nic->gni_nic_hndl,
					(info->attr.prov_key_limit +
					info->attr.user_key_limit));
			if (status != GNI_RC_SUCCESS) {
				GNIX_FATAL(FI_LOG_DOMAIN,
					"failed to set MDD resources, rc=%d\n",
					status);
			}

			nic->mdd_resources_set = 1;
		}
		vmdh_index = _gnix_get_next_reserved_key(info);
		if (vmdh_index <= 0) {
			GNIX_FATAL(FI_LOG_DOMAIN,
				"failed to get next reserved key, "
				"rc=%d\n", vmdh_index);
		}

		flags |= GNI_MEM_USE_VMDH;
	}

	status = GNI_MemRegister(nic->gni_nic_hndl,
				(uint64_t) nic->irq_mmap_addr,
				len,
				nic->rx_cq_blk,
				flags,
				vmdh_index,
				 &nic->irq_mem_hndl);
	if (status != GNI_RC_SUCCESS) {
		ret = gnixu_to_fi_errno(status);
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "GNI_MemRegister returned %s\n",
			  gni_err_str[status]);
		goto err_w_mmap;
	}

#if 0
	fprintf(stderr,"registered ireq memhndl 0x%016lx 0x%016lx\n",
		nic->irq_mem_hndl.qword1,
		nic->irq_mem_hndl.qword2);
#endif


	return ret;

err_w_mmap:
	munmap(mmap_addr, len);
err:
	return ret;
}

/*
 * release resources previously set up for remote
 * GNI_PostCqWrite's to target
 */
static int __nic_teardown_irq_cq(struct gnix_nic *nic)
{
	int ret = FI_SUCCESS;
	gni_return_t status;

	if (nic == NULL)
		return ret;

	if (nic->irq_mmap_addr == NULL)
		return ret;

	if ((nic->irq_mem_hndl.qword1) ||
		(nic->irq_mem_hndl.qword2)) {
		status = GNI_MemDeregister(nic->gni_nic_hndl,
					  &nic->irq_mem_hndl);
		if (status != GNI_RC_SUCCESS) {
			ret = gnixu_to_fi_errno(status);
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "GNI_MemDeregister returned %s\n",
				  gni_err_str[status]);
		}
	}

	munmap(nic->irq_mmap_addr,
		nic->irq_mmap_len);
	return ret;
}


/*
 * place holder for better attributes checker
 */
static int __gnix_nic_check_attr_sanity(struct gnix_nic_attr *attr)
{
	return FI_SUCCESS;
}

static inline struct gnix_tx_descriptor *
__desc_lkup_by_id(struct gnix_nic *nic, int desc_id)
{
	struct gnix_tx_descriptor *tx_desc;

	assert((desc_id >= 0) && (desc_id <= nic->max_tx_desc_id));
	tx_desc = &nic->tx_desc_base[desc_id];
	return tx_desc;
}

static int __nic_rx_overrun(struct gnix_nic *nic)
{
	int i, max_id, ret;
	struct gnix_vc *vc;
	gni_return_t status;
	gni_cq_entry_t cqe;

	GNIX_WARN(FI_LOG_EP_DATA, "\n");

	/* clear out the CQ */
	/*
	 * TODO:  really need to process CQEs better for error reporting,
	 * etc.
	 */
	while ((status = GNI_CqGetEvent(nic->rx_cq, &cqe)) == GNI_RC_SUCCESS);
	assert(status == GNI_RC_NOT_DONE);

	COND_ACQUIRE(nic->requires_lock, &nic->vc_id_lock);
	max_id = nic->vc_id_table_count;
	COND_RELEASE(nic->requires_lock, &nic->vc_id_lock);
	/*
	 * TODO: optimization would
	 * be to keep track of last time
	 * this happened and where smsg msgs.
	 * were found.
	 */
	for (i = 0; i < max_id; i++) {
		ret = _gnix_test_bit(&nic->vc_id_bitmap, i);
		if (ret) {
			vc = __gnix_nic_elem_by_rem_id(nic, i);
			ret = _gnix_vc_rx_schedule(vc);
			assert(ret == FI_SUCCESS);
		}
	}

	return FI_SUCCESS;
}

static int __process_rx_cqe(struct gnix_nic *nic, gni_cq_entry_t cqe)
{
	int ret = FI_SUCCESS, vc_id = 0;
	struct gnix_vc *vc;

	vc_id =  GNI_CQ_GET_INST_ID(cqe);

	/*
	 * its possible this vc has been destroyed, so may get NULL
	 * back.
	 */

	vc = __gnix_nic_elem_by_rem_id(nic, vc_id);
	if (vc != NULL) {
		switch (vc->conn_state) {
		case GNIX_VC_CONNECTING:
			GNIX_DEBUG(FI_LOG_EP_DATA,
				  "Scheduling VC for RX processing (%p)\n",
				  vc);
			ret = _gnix_vc_rx_schedule(vc);
			assert(ret == FI_SUCCESS);
			break;
		case GNIX_VC_CONNECTED:
			GNIX_DEBUG(FI_LOG_EP_DATA,
				  "Processing VC RX (%p)\n",
				  vc);
			ret = _gnix_vc_rx_schedule(vc);
			assert(ret == FI_SUCCESS);
			break;
		default:
			break;  /* VC not in a state for scheduling or
				   SMSG processing */
		}
	}

	return ret;
}

static int __nic_rx_progress(struct gnix_nic *nic)
{
	int ret = FI_SUCCESS;
	gni_return_t status = GNI_RC_NOT_DONE;
	gni_cq_entry_t cqe;

	status = GNI_CqTestEvent(nic->rx_cq);
	if (status == GNI_RC_NOT_DONE)
		return FI_SUCCESS;

	COND_ACQUIRE(nic->requires_lock, &nic->lock);

	do {
		status = GNI_CqGetEvent(nic->rx_cq, &cqe);
		if (OFI_UNLIKELY(status == GNI_RC_NOT_DONE)) {
			ret = FI_SUCCESS;
			break;
		}

		if (OFI_LIKELY(status == GNI_RC_SUCCESS)) {
			/* Find and schedule the associated VC. */
			ret = __process_rx_cqe(nic, cqe);
			if (ret != FI_SUCCESS) {
				GNIX_WARN(FI_LOG_EP_DATA,
					  "process_rx_cqe() failed: %d\n",
					  ret);
			}
		} else if (status == GNI_RC_ERROR_RESOURCE) {
			/* The remote CQ was overrun.  Events related to any VC
			 * could have been missed.  Schedule each VC to be sure
			 * all messages are processed. */
			assert(GNI_CQ_OVERRUN(cqe));
			__nic_rx_overrun(nic);
		} else {
			GNIX_WARN(FI_LOG_EP_DATA,
				  "GNI_CqGetEvent returned %s\n",
				  gni_err_str[status]);
			ret = gnixu_to_fi_errno(status);
			break;
		}
	} while (1);

	COND_RELEASE(nic->requires_lock, &nic->lock);

	return ret;
}

void _gnix_nic_txd_err_inject(struct gnix_nic *nic,
			      struct gnix_tx_descriptor *txd)
{
	slist_insert_tail(&txd->err_list, &nic->err_txds);
}

static int __gnix_nic_txd_err_get(struct gnix_nic *nic,
				  struct gnix_tx_descriptor **txd)
{
	struct slist_entry *list_entry;
	struct gnix_tx_descriptor *txd_p;

	list_entry = slist_remove_head(&nic->err_txds);
	if (list_entry) {
		txd_p = container_of(list_entry,
				     struct gnix_tx_descriptor,
				     err_list);
		*txd = txd_p;
		return 1;
	}

	return 0;
}

static void __nic_get_completed_txd(struct gnix_nic *nic,
				   gni_cq_handle_t hw_cq,
				   struct gnix_tx_descriptor **txd,
				   gni_return_t *tx_status)
{
	gni_post_descriptor_t *gni_desc;
	struct gnix_tx_descriptor *txd_p = NULL;
	struct gnix_fab_req *req;
	gni_return_t status;
	int msg_id;
	gni_cq_entry_t cqe;
	uint32_t recov = 1;

	if (__gnix_nic_txd_err_get(nic, &txd_p)) {
		*txd = txd_p;
		*tx_status = GNI_RC_TRANSACTION_ERROR;
		return;
	}

	status = GNI_CqGetEvent(hw_cq, &cqe);
	if (status == GNI_RC_NOT_DONE) {
		*txd = NULL;
		*tx_status = GNI_RC_NOT_DONE;
		return;
	}

	assert(status == GNI_RC_SUCCESS ||
	       status == GNI_RC_TRANSACTION_ERROR);

	if (OFI_UNLIKELY(status == GNI_RC_TRANSACTION_ERROR)) {
		status = GNI_CqErrorRecoverable(cqe, &recov);
		if (status == GNI_RC_SUCCESS) {
			if (!recov) {
				char ebuf[512];

				GNI_CqErrorStr(cqe, ebuf, sizeof(ebuf));
				GNIX_WARN(FI_LOG_EP_DATA,
					  "CQ error status: %s\n",
					   ebuf);
			}
		} else {
			GNIX_WARN(FI_LOG_EP_DATA,
				  "GNI_CqErrorRecover returned: %s\n",
				   gni_err_str[status]);
			recov = 0;  /* assume something bad has happened */
		}
	}

	if (GNI_CQ_GET_TYPE(cqe) == GNI_CQ_EVENT_TYPE_POST) {
		status = GNI_GetCompleted(hw_cq, cqe, &gni_desc);

		assert(status == GNI_RC_SUCCESS ||
		       status == GNI_RC_TRANSACTION_ERROR);

		txd_p = container_of(gni_desc,
				   struct gnix_tx_descriptor,
				   gni_desc);
	} else if (GNI_CQ_GET_TYPE(cqe) == GNI_CQ_EVENT_TYPE_SMSG) {
		msg_id = GNI_CQ_GET_MSG_ID(cqe);
		txd_p = __desc_lkup_by_id(nic, msg_id);
	}

	if (OFI_UNLIKELY(txd_p == NULL))
		GNIX_FATAL(FI_LOG_EP_DATA, "Unexpected CQE: 0x%lx", cqe);

	/*
	 * set retry count on the request to max to force
	 * delivering error'd CQ event to application
	 */
	if (!recov) {
		status = GNI_RC_TRANSACTION_ERROR;
		req = txd_p->req;
		if (req)
			req->tx_failures = UINT_MAX;
	}

	*tx_status = status;
	*txd = txd_p;

}

static int __nic_tx_progress(struct gnix_nic *nic, gni_cq_handle_t cq)
{
	int ret = FI_SUCCESS;
	gni_return_t tx_status;
	struct gnix_tx_descriptor *txd;

	do {
		txd = NULL;

		COND_ACQUIRE(nic->requires_lock, &nic->lock);
		__nic_get_completed_txd(nic, cq, &txd,
					&tx_status);
		COND_RELEASE(nic->requires_lock, &nic->lock);

		if (txd && txd->completer_fn) {
			ret = txd->completer_fn(txd, tx_status);
			if (ret != FI_SUCCESS) {
				/*
				 * TODO: need to post error to CQ
				 */
				GNIX_WARN(FI_LOG_EP_DATA,
					  "TXD completer failed: %d", ret);
			}
		}

		if ((txd == NULL) || ret != FI_SUCCESS)
			break;
	} while (1);

	return ret;
}

int _gnix_nic_progress(void *arg)
{
	struct gnix_nic *nic = (struct gnix_nic *)arg;
	int ret = FI_SUCCESS;

	ret =  __nic_tx_progress(nic, nic->tx_cq);
	if (OFI_UNLIKELY(ret != FI_SUCCESS))
		return ret;

	if (nic->tx_cq_blk && nic->tx_cq_blk != nic->tx_cq) {
		ret =  __nic_tx_progress(nic, nic->tx_cq_blk);
		if (OFI_UNLIKELY(ret != FI_SUCCESS))
			return ret;
	}

	ret = __nic_rx_progress(nic);
	if (ret != FI_SUCCESS)
		return ret;

	ret = _gnix_vc_nic_progress(nic);
	if (ret != FI_SUCCESS)
		return ret;

	return ret;
}

int _gnix_nic_free_rem_id(struct gnix_nic *nic, int remote_id)
{
	assert(nic);

	if ((remote_id < 0) || (remote_id > nic->vc_id_table_count))
		return -FI_EINVAL;

	_gnix_clear_bit(&nic->vc_id_bitmap, remote_id);

	return FI_SUCCESS;
}

/*
 * this function is needed to allow for quick lookup of a vc based on
 * the contents of the GNI CQE coming off of the GNI RX CQ associated
 * with GNI nic being used by this VC.  Using a bitmap to expedite
 * scanning vc's in the case of a GNI CQ overrun.
 */

int _gnix_nic_get_rem_id(struct gnix_nic *nic, int *remote_id, void *entry)
{
	int ret = FI_SUCCESS;
	void **table_base;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	/*
	 * TODO:  really need to search bitmap for clear
	 * bit before resizing the table
	 */

	COND_ACQUIRE(nic->requires_lock, &nic->vc_id_lock);
	if (nic->vc_id_table_capacity == nic->vc_id_table_count) {
		table_base = realloc(nic->vc_id_table,
				     2 * nic->vc_id_table_capacity *
				     sizeof(void *));
		if (table_base == NULL) {
			ret =  -FI_ENOMEM;
			goto err;
		}
		nic->vc_id_table_capacity *= 2;
		nic->vc_id_table = table_base;

		ret = _gnix_realloc_bitmap(&nic->vc_id_bitmap,
					   nic->vc_id_table_capacity);
		if (ret != FI_SUCCESS) {
			assert(ret == -FI_ENOMEM);
			goto err;
		}
	}

	nic->vc_id_table[nic->vc_id_table_count] = entry;
	*remote_id = nic->vc_id_table_count;

	/*
	 * set bit in the bitmap
	 */

	_gnix_set_bit(&nic->vc_id_bitmap, nic->vc_id_table_count);

	++(nic->vc_id_table_count);
err:
	COND_RELEASE(nic->requires_lock, &nic->vc_id_lock);
	return ret;
}

/*
 * allocate a free list of tx descs for a gnix_nic struct.
 */

static int __gnix_nic_tx_freelist_init(struct gnix_nic *nic, int n_descs)
{
	int i, ret = FI_SUCCESS;
	struct gnix_tx_descriptor *desc_base, *desc_ptr;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	/*
	 * set up free list of tx descriptors.
	 */

	desc_base = calloc(n_descs, sizeof(struct gnix_tx_descriptor));
	if (desc_base == NULL) {
		ret = -FI_ENOMEM;
		goto err;
	}

	dlist_init(&nic->tx_desc_free_list);
	dlist_init(&nic->tx_desc_active_list);

	for (i = 0, desc_ptr = desc_base; i < n_descs; i++, desc_ptr++) {
		desc_ptr->id = i;
		dlist_insert_tail(&desc_ptr->list,
				  &nic->tx_desc_free_list);
	}

	nic->max_tx_desc_id = n_descs - 1;
	nic->tx_desc_base = desc_base;

	fastlock_init(&nic->tx_desc_lock);

	return ret;

err:
	return ret;

}

/*
 * clean up the tx descs free list
 */
static void __gnix_nic_tx_freelist_destroy(struct gnix_nic *nic)
{
	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	free(nic->tx_desc_base);
	fastlock_destroy(&nic->tx_desc_lock);
}

/*
 * free a gnix nic and associated resources if refcnt drops to 0
 */

static void __nic_destruct(void *obj)
{
	int ret = FI_SUCCESS;
	gni_return_t status = GNI_RC_SUCCESS;
	struct gnix_nic *nic = (struct gnix_nic *) obj;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	/* Get us out of the progression tables we are destroying the nic
	 * and we don't want the wait progression thread to progress us
	 * after our structures are destroyed.
	 */
	pthread_mutex_lock(&gnix_nic_list_lock);

	dlist_remove(&nic->gnix_nic_list);
	--gnix_nics_per_ptag[nic->ptag];
	dlist_remove(&nic->ptag_nic_list);

	pthread_mutex_unlock(&gnix_nic_list_lock);
	__gnix_nic_tx_freelist_destroy(nic);

	/*
	 *free irq cq related resources
	 */

	ret = __nic_teardown_irq_cq(nic);
	if (ret != FI_SUCCESS)
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "__nic_teardown_irq_cq returned %s\n",
			  fi_strerror(-ret));

	/*
	 * kill off progress thread, if any
	 */

	if (nic->progress_thread) {

		ret = pthread_cancel(nic->progress_thread);
		if ((ret != 0) && (ret != ESRCH)) {
			GNIX_WARN(FI_LOG_EP_CTRL,
			"pthread_cancel returned %d\n", ret);
			goto err;
		}

		ret = pthread_join(nic->progress_thread,
				   NULL);
		if ((ret != 0) && (ret != ESRCH)) {
			GNIX_WARN(FI_LOG_EP_CTRL,
			"pthread_join returned %d\n", ret);
			goto err;
		}

		GNIX_INFO(FI_LOG_EP_CTRL, "pthread_join returned %d\n", ret);
		nic->progress_thread = 0;
	}

	/* Must free mboxes first, because the MR has a pointer to the
	 * nic handles below */
	ret = _gnix_mbox_allocator_destroy(nic->mbox_hndl);
	if (ret != FI_SUCCESS)
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "_gnix_mbox_allocator_destroy returned %s\n",
			  fi_strerror(-ret));

	/*
	 * see comments in the nic constructor about why
	 * the following code section is currently stubbed out.
	 */
#if 0
	ret = _gnix_mbox_allocator_destroy(nic->s_rdma_buf_hndl);
	if (ret != FI_SUCCESS)
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "_gnix_mbox_allocator_destroy returned %s\n",
			  fi_strerror(-ret));

	ret = _gnix_mbox_allocator_destroy(nic->r_rdma_buf_hndl);
	if (ret != FI_SUCCESS)
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "_gnix_mbox_allocator_destroy returned %s\n",
			  fi_strerror(-ret));
#endif

	if (!nic->gni_cdm_hndl) {
		GNIX_WARN(FI_LOG_EP_CTRL, "No CDM attached to nic, nic=%p");
	}

	assert(nic->gni_cdm_hndl != NULL);

	if (nic->rx_cq != NULL && nic->rx_cq != nic->rx_cq_blk) {
		status = GNI_CqDestroy(nic->rx_cq);
		if (status != GNI_RC_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "GNI_CqDestroy returned %s\n",
				 gni_err_str[status]);
			ret = gnixu_to_fi_errno(status);
			goto err;
		}
	}

	if (nic->rx_cq_blk != NULL) {
		status = GNI_CqDestroy(nic->rx_cq_blk);
		if (status != GNI_RC_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "GNI_CqDestroy returned %s\n",
				 gni_err_str[status]);
			ret = gnixu_to_fi_errno(status);
			goto err;
		}
	}

	if (nic->tx_cq != NULL && nic->tx_cq != nic->tx_cq_blk) {
		status = GNI_CqDestroy(nic->tx_cq);
		if (status != GNI_RC_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "GNI_CqDestroy returned %s\n",
				 gni_err_str[status]);
			ret = gnixu_to_fi_errno(status);
			goto err;
		}
	}

	if (nic->tx_cq_blk != NULL) {
		status = GNI_CqDestroy(nic->tx_cq_blk);
		if (status != GNI_RC_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "GNI_CqDestroy returned %s\n",
				 gni_err_str[status]);
			ret = gnixu_to_fi_errno(status);
			goto err;
		}
	}

	if (nic->allocd_gni_res & GNIX_NIC_CDM_ALLOCD) {
		status = GNI_CdmDestroy(nic->gni_cdm_hndl);
		if (status != GNI_RC_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "GNI_CdmDestroy returned %s\n",
				  gni_err_str[status]);
			ret = gnixu_to_fi_errno(status);
			goto err;
		}
	}

	if (nic->vc_id_table != NULL) {
		free(nic->vc_id_table);
	} else {
		GNIX_WARN(FI_LOG_EP_CTRL, "vc_id_table was NULL\n");
	}

	/*
	 * destroy VC free list associated with this nic
	 */

	_gnix_fl_destroy(&nic->vc_freelist);

	/*
	 * remove the nic from the linked lists
	 * for the domain and the global nic list
	 */

err:
	_gnix_free_bitmap(&nic->vc_id_bitmap);

	free(nic);
}

int _gnix_nic_free(struct gnix_nic *nic)
{
	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (nic == NULL)
		return -FI_EINVAL;

	_gnix_ref_put(nic);

	return FI_SUCCESS;
}

/*
 * allocate a gnix_nic struct using attributes of the domain
 */

int gnix_nic_alloc(struct gnix_fid_domain *domain,
		   struct gnix_nic_attr *attr,
		   struct gnix_nic **nic_ptr)
{
	int ret = FI_SUCCESS;
	struct gnix_nic *nic = NULL;
	uint32_t device_addr;
	gni_return_t status;
	uint32_t fake_cdm_id = GNIX_CREATE_CDM_ID;
	gni_smsg_attr_t smsg_mbox_attr;
	struct gnix_nic_attr *nic_attr = &default_attr;
	uint32_t num_corespec_cpus = 0;
	bool must_alloc_nic = false;
	bool free_list_inited = false;
	struct gnix_auth_key *auth_key;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	*nic_ptr = NULL;
	nic_attr->gni_cdm_modes = gnix_cdm_modes;

	if (attr) {
		ret = __gnix_nic_check_attr_sanity(attr);
		if (ret != FI_SUCCESS)
			return ret;
		nic_attr = attr;
		must_alloc_nic = nic_attr->must_alloc;
	}

	auth_key = nic_attr->auth_key;

	/*
	 * If we've maxed out the number of nics for this domain/ptag,
	 * search the list of existing nics.  Take the gnix_nic_list_lock
	 * here since the gnix_nic_list will be manipulated whether or
	 * not we attach to an existing nic or create a new one.
	 *
	 * Should not matter much that this is a pretty fat critical section
	 * since endpoint setup for RDM type will typically occur near
	 * app startup, likely in a single threaded region, and for the
	 * case of MSG, where there will likely be many 100s of EPs, after
	 * a few initial slow times through this section when nics are created,
	 * max nic count for the ptag will be reached and only the first part
	 * of the critical section - iteration over existing nics - will be
	 * happening.
	 */

	pthread_mutex_lock(&gnix_nic_list_lock);

	/*
	 * we can reuse previously allocated nics as long as a
	 * must_alloc is not specified in the nic_attr arg.
	 */

	if ((must_alloc_nic == false) &&
	    (gnix_nics_per_ptag[auth_key->ptag] >= gnix_max_nics_per_ptag)) {
		assert(!dlist_empty(&gnix_nic_list_ptag[auth_key->ptag]));

		nic = dlist_first_entry(&gnix_nic_list_ptag[auth_key->ptag],
					struct gnix_nic, ptag_nic_list);
		dlist_remove(&nic->ptag_nic_list);
		dlist_insert_tail(&nic->ptag_nic_list,
				  &gnix_nic_list_ptag[auth_key->ptag]);
		_gnix_ref_get(nic);

		GNIX_INFO(FI_LOG_EP_CTRL, "Reusing NIC:%p\n", nic);
	}

	/*
	 * no nic found create a cdm and attach
	 */

	if (!nic) {

		nic = calloc(1, sizeof(struct gnix_nic));
		if (nic == NULL) {
			ret = -FI_ENOMEM;
			goto err;
		}

		nic->using_vmdh = domain->using_vmdh;

		if (nic_attr->use_cdm_id == false) {
			ret = _gnix_cm_nic_create_cdm_id(domain, &fake_cdm_id);
			if (ret != FI_SUCCESS) {
				GNIX_WARN(FI_LOG_EP_CTRL,
					  "_gnix_cm_nic_create_cdm_id returned %s\n",
					  fi_strerror(-ret));
				goto err;
			}
		} else
			fake_cdm_id = nic_attr->cdm_id;

		if (nic_attr->gni_cdm_hndl == NULL) {
			status = GNI_CdmCreate(fake_cdm_id,
						auth_key->ptag,
						auth_key->cookie,
						gnix_cdm_modes,
						&nic->gni_cdm_hndl);
			if (status != GNI_RC_SUCCESS) {
				GNIX_WARN(FI_LOG_EP_CTRL, "GNI_CdmCreate returned %s\n",
					 gni_err_str[status]);
				ret = gnixu_to_fi_errno(status);
				goto err1;
			}
			nic->allocd_gni_res |= GNIX_NIC_CDM_ALLOCD;
		} else {
			nic->gni_cdm_hndl = nic_attr->gni_cdm_hndl;
		}

		/*
		 * Okay, now go for the attach
		*/

		if (nic_attr->gni_nic_hndl == NULL) {
			status = GNI_CdmAttach(nic->gni_cdm_hndl,
						0,
						&device_addr,
						&nic->gni_nic_hndl);
			if (status != GNI_RC_SUCCESS) {
				GNIX_WARN(FI_LOG_EP_CTRL, "GNI_CdmAttach returned %s\n",
					 gni_err_str[status]);
				_gnix_dump_gni_res(auth_key->ptag);
				ret = gnixu_to_fi_errno(status);
				goto err1;
			}
		} else
			nic->gni_nic_hndl = nic_attr->gni_nic_hndl;

		/*
		 * create TX CQs - first polling, then blocking
		 */

		status = GNI_CqCreate(nic->gni_nic_hndl,
					domain->params.tx_cq_size,
					0,                  /* no delay count */
					GNI_CQ_BLOCKING |
						domain->gni_cq_modes,
					NULL,              /* useless handler */
					NULL,               /* useless handler
								context */
					&nic->tx_cq_blk);
		if (status != GNI_RC_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "GNI_CqCreate returned %s\n",
				  gni_err_str[status]);
			_gnix_dump_gni_res(auth_key->ptag);
			ret = gnixu_to_fi_errno(status);
			goto err1;
		}

		/* Use blocking CQs for all operations if eager_auto_progress
		 * is used.  */
		if (domain->params.eager_auto_progress) {
			nic->tx_cq = nic->tx_cq_blk;
		} else {
			status = GNI_CqCreate(nic->gni_nic_hndl,
						domain->params.tx_cq_size,
						0, /* no delay count */
						domain->gni_cq_modes,
						NULL, /* useless handler */
						NULL, /* useless handler ctx */
						&nic->tx_cq);
			if (status != GNI_RC_SUCCESS) {
				GNIX_WARN(FI_LOG_EP_CTRL,
					  "GNI_CqCreate returned %s\n",
					  gni_err_str[status]);
				_gnix_dump_gni_res(auth_key->ptag);
				ret = gnixu_to_fi_errno(status);
				goto err1;
			}
		}


		/*
		 * create RX CQs - first polling, then blocking
		 */

		status = GNI_CqCreate(nic->gni_nic_hndl,
					domain->params.rx_cq_size,
					0,
					GNI_CQ_BLOCKING |
						domain->gni_cq_modes,
					NULL,
					NULL,
					&nic->rx_cq_blk);
		if (status != GNI_RC_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "GNI_CqCreate returned %s\n",
				  gni_err_str[status]);
			_gnix_dump_gni_res(auth_key->ptag);
			ret = gnixu_to_fi_errno(status);
			goto err1;
		}

		/* Use blocking CQs for all operations if eager_auto_progress
		 * is used.  */
		if (domain->params.eager_auto_progress) {
			nic->rx_cq = nic->rx_cq_blk;
		} else {
			status = GNI_CqCreate(nic->gni_nic_hndl,
						domain->params.rx_cq_size,
						0,
						domain->gni_cq_modes,
						NULL,
						NULL,
						&nic->rx_cq);
			if (status != GNI_RC_SUCCESS) {
				GNIX_WARN(FI_LOG_EP_CTRL,
					  "GNI_CqCreate returned %s\n",
					  gni_err_str[status]);
				_gnix_dump_gni_res(auth_key->ptag);
				ret = gnixu_to_fi_errno(status);
				goto err1;
			}
		}

		nic->device_addr = device_addr;
		nic->ptag = auth_key->ptag;
		nic->cookie = auth_key->cookie;

		nic->vc_id_table_capacity = domain->params.vc_id_table_capacity;
		nic->vc_id_table = malloc(sizeof(void *) *
					       nic->vc_id_table_capacity);
		if (nic->vc_id_table == NULL) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "malloc of vc_id_table failed\n");
			ret = -FI_ENOMEM;
			goto err1;
		}

		ret = _gnix_alloc_bitmap(&nic->vc_id_bitmap,
					 nic->vc_id_table_capacity, NULL);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "alloc_bitmap returned %d\n", ret);
			goto err1;
		}
		fastlock_init(&nic->vc_id_lock);

		/*
		 * initialize free list for VC's
		 * In addition to hopefully allowing for a more compact
		 * allocation of VC structs, the free list is also import
		 * because there is a window of time when using auto progress
		 * that a thread may be going through the progress engine
		 * while one of the application threads is actively tearing
		 * down an endpoint (and hence its associated VCs) before the
		 * rem_id for the vc is removed from the vector.
		 * As a consequence, it is important that
		 * the memory allocated within the freelist allocator not be
		 * returned to the system prior to the freelist being destroyed
		 * as part of the nic destructor procedure.  The freelist is
		 * destroyed in that procedure after the progress thread
		 * has been joined.
		 */

		ret = _gnix_fl_init_ts(sizeof(struct gnix_vc),
				       offsetof(struct gnix_vc, fr_list),
				       GNIX_VC_FL_MIN_SIZE,
				       GNIX_VC_FL_INIT_REFILL_SIZE,
				       0,
				       0,
				       &nic->vc_freelist);
		if (ret == FI_SUCCESS) {
			free_list_inited = true;
		} else {
			GNIX_DEBUG(FI_LOG_EP_DATA, "_gnix_fl_init returned: %s\n",
				   fi_strerror(-ret));
			goto err1;
		}

		fastlock_init(&nic->lock);

		ret = __gnix_nic_tx_freelist_init(nic,
						  domain->params.tx_cq_size);
		if (ret != FI_SUCCESS)
			goto err1;

		fastlock_init(&nic->prog_vcs_lock);
		dlist_init(&nic->prog_vcs);

		_gnix_ref_init(&nic->ref_cnt, 1, __nic_destruct);

		smsg_mbox_attr.msg_type = GNI_SMSG_TYPE_MBOX_AUTO_RETRANSMIT;
		smsg_mbox_attr.mbox_maxcredit = domain->params.mbox_maxcredit;
		smsg_mbox_attr.msg_maxsize =  domain->params.mbox_msg_maxsize;

		status = GNI_SmsgBufferSizeNeeded(&smsg_mbox_attr,
						  &nic->mem_per_mbox);
		if (status != GNI_RC_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "GNI_SmsgBufferSizeNeeded returned %s\n",
				  gni_err_str[status]);
			ret = gnixu_to_fi_errno(status);
			goto err1;
		}

		/*
		 * set up mailbox allocator for SMSG mailboxes
		 */

		ret = _gnix_mbox_allocator_create(nic,
					  nic->rx_cq,
					  domain->params.mbox_page_size,
					  (size_t)nic->mem_per_mbox,
					  domain->params.mbox_num_per_slab,
					  &nic->mbox_hndl);

		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "_gnix_mbox_alloc returned %s\n",
				  fi_strerror(-ret));
			goto err1;
		}

		/*
		 * use the mailbox allocator system to set up an
		 * pre-pinned RDMA bounce buffers for longer eager
		 * messages and other cases where zero-copy
		 * can't be safely used.
		 *
		 * One set of blocks is used for the send side.
		 * A second set of blocks is used for the receive
		 * side.  Both sets of blocks are registered against
		 * the blocking RX CQ for this nic.
		 *
		 * TODO: hardwired constants, uff
		 * TODO: better to use a buddy allocator or some other
		 * allocator
		 * Disable these for now as we're not using and they
		 * chew up a lot of IOMMU space per nic.
		 */

#if 0
		ret = _gnix_mbox_allocator_create(nic,
						  NULL,
						  GNIX_PAGE_2MB,
						  65536,
						  512,
						  &nic->s_rdma_buf_hndl);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "_gnix_mbox_alloc returned %s\n",
				  fi_strerror(-ret));
			_gnix_dump_gni_res(domain->ptag);
			goto err1;
		}

		ret = _gnix_mbox_allocator_create(nic,
						  NULL,
						  GNIX_PAGE_2MB,
						  65536,
						  512,
						  &nic->r_rdma_buf_hndl);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "_gnix_mbox_alloc returned %s\n",
				  fi_strerror(-ret));
			_gnix_dump_gni_res(domain->ptag);
			goto err1;
		}
#endif

		ret =  __nic_setup_irq_cq(nic);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "__nic_setup_irq_cq returned %s\n",
				  fi_strerror(-ret));
			_gnix_dump_gni_res(auth_key->ptag);
			goto err1;
		}

		/*
 		 * if the domain is using PROGRESS_AUTO for data, set up
 		 * a progress thread.
 		 */

		if (domain->data_progress == FI_PROGRESS_AUTO) {

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
				"job_disable/unassigned cpus returned %d\n",
					 ret);

			ret = pthread_create(&nic->progress_thread,
					     NULL,
					     __gnix_nic_prog_thread_fn,
					     (void *)nic);
			if (ret)
				GNIX_WARN(FI_LOG_EP_CTRL,
				"pthread_create call returned %d\n", ret);
		}

		dlist_insert_tail(&nic->gnix_nic_list, &gnix_nic_list);
		dlist_insert_tail(&nic->ptag_nic_list,
				  &gnix_nic_list_ptag[auth_key->ptag]);

		nic->smsg_callbacks = gnix_ep_smsg_callbacks;

		++gnix_nics_per_ptag[auth_key->ptag];

		GNIX_INFO(FI_LOG_EP_CTRL, "Allocated NIC:%p\n", nic);
	}

	if (nic) {
		nic->requires_lock = domain->thread_model != FI_THREAD_COMPLETION;
		nic->using_vmdh = domain->using_vmdh;
	}

	*nic_ptr = nic;
	goto out;

err1:
	ofi_atomic_dec32(&gnix_id_counter);
err:
	if (nic != NULL) {
		__nic_teardown_irq_cq(nic);
		if (nic->r_rdma_buf_hndl != NULL)
			_gnix_mbox_allocator_destroy(nic->r_rdma_buf_hndl);
		if (nic->s_rdma_buf_hndl != NULL)
			_gnix_mbox_allocator_destroy(nic->s_rdma_buf_hndl);
		if (nic->mbox_hndl != NULL)
			_gnix_mbox_allocator_destroy(nic->mbox_hndl);
		if (nic->rx_cq != NULL && nic->rx_cq != nic->rx_cq_blk)
			GNI_CqDestroy(nic->rx_cq);
		if (nic->rx_cq_blk != NULL)
			GNI_CqDestroy(nic->rx_cq_blk);
		if (nic->tx_cq != NULL && nic->tx_cq != nic->tx_cq_blk)
			GNI_CqDestroy(nic->tx_cq);
		if (nic->tx_cq_blk != NULL)
			GNI_CqDestroy(nic->tx_cq_blk);
		if ((nic->gni_cdm_hndl != NULL) && (nic->allocd_gni_res &
		    GNIX_NIC_CDM_ALLOCD))
			GNI_CdmDestroy(nic->gni_cdm_hndl);
		if (free_list_inited == true)
			_gnix_fl_destroy(&nic->vc_freelist);
		free(nic);
	}

out:
	pthread_mutex_unlock(&gnix_nic_list_lock);
	return ret;
}

void _gnix_nic_init(void)
{
	int i, rc;

	for (i = 0; i < GNI_PTAG_MAX; i++) {
		dlist_init(&gnix_nic_list_ptag[i]);
	}

	rc = _gnix_nics_per_rank(&gnix_max_nics_per_ptag);
	if (rc == FI_SUCCESS) {
		GNIX_DEBUG(FI_LOG_FABRIC, "gnix_max_nics_per_ptag: %u\n",
			   gnix_max_nics_per_ptag);
	} else {
		GNIX_WARN(FI_LOG_FABRIC, "_gnix_nics_per_rank failed: %d\n",
			  rc);
	}

	if (getenv("GNIX_MAX_NICS") != NULL)
		gnix_max_nics_per_ptag = atoi(getenv("GNIX_MAX_NICS"));

	/*
	 * Well if we didn't get 1 nic, that means we must really be doing
	 * FMA sharing.
	 */

	if (gnix_max_nics_per_ptag == 0) {
		gnix_max_nics_per_ptag = 1;
		GNIX_WARN(FI_LOG_FABRIC, "Using inter-procss FMA sharing\n");
	}
}


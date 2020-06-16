/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
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

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <assert.h>

#include "gnix.h"
#include "gnix_datagram.h"
#include "gnix_cm_nic.h"
#include "gnix_cm.h"
#include "gnix_nic.h"
#include "gnix_hashtable.h"


#define GNIX_CM_NIC_BND_TAG (100)
#define GNIX_CM_NIC_WC_TAG (99)

DLIST_HEAD(gnix_cm_nic_list);
pthread_mutex_t gnix_cm_nic_list_lock = PTHREAD_MUTEX_INITIALIZER;

/*******************************************************************************
 * Helper functions
 ******************************************************************************/

static void __dgram_set_tag(struct gnix_datagram *d, uint8_t tag)
{

	_gnix_dgram_pack_buf(d, GNIX_DGRAM_IN_BUF,
				    &tag, sizeof(uint8_t));
}

/*
 * we unpack the out tag instead of getting it
 * since we need to pass the partially advanced
 * out buf to the receive callback function
 * associated with the cm_nic instance.
 */
static void __dgram_unpack_out_tag(struct gnix_datagram *d, uint8_t *tag)
{

	_gnix_dgram_rewind_buf(d, GNIX_DGRAM_OUT_BUF);
	_gnix_dgram_unpack_buf(d, GNIX_DGRAM_OUT_BUF,
				      tag, sizeof(uint8_t));
}

static void __dgram_get_in_tag(struct gnix_datagram *d, uint8_t *tag)
{

	_gnix_dgram_rewind_buf(d, GNIX_DGRAM_IN_BUF);
	_gnix_dgram_unpack_buf(d, GNIX_DGRAM_IN_BUF,
				      tag, sizeof(uint8_t));
	_gnix_dgram_rewind_buf(d, GNIX_DGRAM_IN_BUF);

}

static int __process_dgram_w_error(struct gnix_cm_nic *cm_nic,
				   struct gnix_datagram *dgram,
				   struct gnix_address peer_address,
				   gni_post_state_t state)
{
	return -FI_ENOSYS;
}

static int __process_datagram(struct gnix_datagram *dgram,
				 struct gnix_address peer_address,
				 gni_post_state_t state)
{
	int ret = FI_SUCCESS;
	struct gnix_cm_nic *cm_nic = NULL;
	uint8_t in_tag = 0, out_tag = 0;
	char rcv_buf[GNIX_CM_NIC_MAX_MSG_SIZE];

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	cm_nic = (struct gnix_cm_nic *)dgram->cache;
	if (cm_nic == NULL) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			"process_datagram, null cache\n");
		goto err;
	}

	if (state != GNI_POST_COMPLETED) {
		ret = __process_dgram_w_error(cm_nic,
					      dgram,
					      peer_address,
					      state);
		GNIX_WARN(FI_LOG_EP_CTRL,
			"process_datagram bad post state %d\n", state);
		goto err;
	}

	__dgram_get_in_tag(dgram, &in_tag);
	if ((in_tag != GNIX_CM_NIC_BND_TAG) &&
		(in_tag != GNIX_CM_NIC_WC_TAG)) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			"datagram with unknown in tag %d\n", in_tag);
		goto err;
	}

	 __dgram_unpack_out_tag(dgram, &out_tag);
	if ((out_tag != GNIX_CM_NIC_BND_TAG) &&
		(out_tag != GNIX_CM_NIC_WC_TAG)) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			"datagram with unknown out tag %d\n", out_tag);
		goto err;
	}

	/*
	 * if out buf actually has data, call consumer's
	 * receive callback
	 */

	if (out_tag == GNIX_CM_NIC_BND_TAG) {
		_gnix_dgram_unpack_buf(dgram,
					GNIX_DGRAM_OUT_BUF,
					rcv_buf,
					GNIX_CM_NIC_MAX_MSG_SIZE);
		ret = cm_nic->rcv_cb_fn(cm_nic,
					rcv_buf,
					peer_address);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				"cm_nic->rcv_cb_fn returned %s\n",
				fi_strerror(-ret));
			goto err;
		}

		ret = _gnix_cm_nic_progress(cm_nic);
		if (ret != FI_SUCCESS)
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "_gnix_cm_nic_progress returned %s\n",
				  fi_strerror(-ret));
	}

	/*
	 * if we are processing a WC datagram, repost, otherwise
	 * just put back on the freelist.
	 */
	if (in_tag == GNIX_CM_NIC_WC_TAG) {
		dgram->callback_fn = __process_datagram;
		dgram->cache = cm_nic;
		 __dgram_set_tag(dgram, in_tag);
		ret = _gnix_dgram_wc_post(dgram);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				"_gnix_dgram_wc_post returned %s\n",
				fi_strerror(-ret));
			goto err;
		}
	} else {
		ret  = _gnix_dgram_free(dgram);
		if (ret != FI_SUCCESS)
			GNIX_WARN(FI_LOG_EP_CTRL,
				"_gnix_dgram_free returned %s\n",
				fi_strerror(-ret));
	}

	return ret;

err:
	if (in_tag == GNIX_CM_NIC_BND_TAG)
		_gnix_dgram_free(dgram);
	return ret;
}

static bool __gnix_cm_nic_timeout_needed(void *data)
{
	struct gnix_cm_nic *cm_nic = (struct gnix_cm_nic *)data;
	return _gnix_cm_nic_need_progress(cm_nic);
}

static void __gnix_cm_nic_timeout_progress(void *data)
{
	int ret;
	struct gnix_cm_nic *cm_nic = (struct gnix_cm_nic *)data;
	ret = _gnix_cm_nic_progress(cm_nic);
	if (ret != FI_SUCCESS)
		GNIX_WARN(FI_LOG_EP_CTRL,
			"_gnix_cm_nic_progress returned %s\n",
			fi_strerror(-ret));
}


/*******************************************************************************
 * Internal API functions
 ******************************************************************************/

int _gnix_cm_nic_create_cdm_id(struct gnix_fid_domain *domain, uint32_t *id)
{
	uint32_t cdm_id;
	int v;

	if (*id != GNIX_CREATE_CDM_ID) {
		return FI_SUCCESS;
	}

	/*
	 * generate a cdm_id, use the 16 LSB of base_id from domain
	 * with 16 MSBs being obtained from atomic increment of
	 * a local variable.
	 */

	v = ofi_atomic_inc32(&gnix_id_counter);

	cdm_id = ((domain->cdm_id_seed & 0xFFF) << 12) | v;
	*id = cdm_id;
	return FI_SUCCESS;
}

/**
 * This function will return a block of id's starting at id through nids
 *
 * @param domain  gnix domain
 * @param nids    number of id's
 * @param id      if -1 return an id based on the counter and seed
 */
int _gnix_get_new_cdm_id_set(struct gnix_fid_domain *domain, int nids,
			     uint32_t *id)
{
	uint32_t cdm_id;
	int v;

	if (*id == -1) {
		v = ofi_atomic_add32(&gnix_id_counter, nids);
		cdm_id = ((domain->cdm_id_seed & 0xFFF) << 12) | v;
		*id = cdm_id;
	} else {
		/*
		 * asking for a block starting at a chosen base
		 * TODO: sanity check that requested base is reasonable
		 */
		if (*id <= ofi_atomic_get32(&gnix_id_counter))
			return -FI_ENOSPC;
		ofi_atomic_set32(&gnix_id_counter, (*(int *)id + nids));
	}
	return FI_SUCCESS;
}

int _gnix_cm_nic_progress(void *arg)
{
	struct gnix_cm_nic *cm_nic = (struct gnix_cm_nic *)arg;
	int ret = FI_SUCCESS;
	int complete;
	struct gnix_work_req *p = NULL;

	/*
	 * if we're doing FI_PROGRESS_MANUAL,
	 * see what's going on inside kgni's datagram
	 * box...
	 */

	if (cm_nic->ctrl_progress == FI_PROGRESS_MANUAL) {
		++cm_nic->poll_cnt;
		if (((cm_nic->poll_cnt % 512) == 0)  ||
			!dlist_empty(&cm_nic->cm_nic_wq)) {
			ret = _gnix_dgram_poll(cm_nic->dgram_hndl,
						  GNIX_DGRAM_NOBLOCK);
			if (ret != FI_SUCCESS) {
				GNIX_WARN(FI_LOG_EP_CTRL,
					"_gnix_dgram_poll returned %s\n",
					  fi_strerror(-ret));
					goto err;
			}
		}
	}

	/*
	 * do a quick check if queue doesn't have anything yet,
	 * don't need this to be atomic
	 */

check_again:
	if (dlist_empty(&cm_nic->cm_nic_wq))
		return ret;

	/*
	 * okay, stuff to do, lock work queue,
	 * dequeue head, unlock, process work element,
	 * if it doesn't compete, put back at the tail
	 * of the queue.
	 */

	fastlock_acquire(&cm_nic->wq_lock);
	p = dlist_first_entry(&cm_nic->cm_nic_wq, struct gnix_work_req,
			      list);
	if (p == NULL) {
		fastlock_release(&cm_nic->wq_lock);
		return ret;
	}

	dlist_remove_init(&p->list);
	fastlock_release(&cm_nic->wq_lock);

	assert(p->progress_fn);

	ret = p->progress_fn(p->data, &complete);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "dgram prog fn returned %s\n",
			  fi_strerror(-ret));
	}

	if (complete == 1) {
		if (p->completer_fn) {
			ret = p->completer_fn(p->completer_data);
			free(p);
			if (ret != FI_SUCCESS) {
				GNIX_WARN(FI_LOG_EP_CTRL,
					  "dgram completer fn returned %s\n",
					  fi_strerror(-ret));
				goto err;
			}
		} else {
			free(p);
		}
		goto check_again;
	} else {
		fastlock_acquire(&cm_nic->wq_lock);
		dlist_insert_before(&p->list, &cm_nic->cm_nic_wq);
		fastlock_release(&cm_nic->wq_lock);
	}

err:
	return ret;
}

static void  __cm_nic_destruct(void *obj)
{
	int ret;
	struct gnix_cm_nic *cm_nic = (struct gnix_cm_nic *)obj;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	pthread_mutex_lock(&gnix_cm_nic_list_lock);
	dlist_remove(&cm_nic->cm_nic_list);
	pthread_mutex_unlock(&gnix_cm_nic_list_lock);

	if (cm_nic->dgram_hndl != NULL) {
		ret = _gnix_dgram_hndl_free(cm_nic->dgram_hndl);
		if (ret != FI_SUCCESS)
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "gnix_dgram_hndl_free returned %d\n",
				  ret);
	}

	if (cm_nic->addr_to_ep_ht != NULL) {
		ret = _gnix_ht_destroy(cm_nic->addr_to_ep_ht);
		if (ret != FI_SUCCESS)
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "gnix_ht_destroy returned %d\n",
				  ret);
		free(cm_nic->addr_to_ep_ht);
		cm_nic->addr_to_ep_ht = NULL;
	}

	if (cm_nic->nic != NULL) {
		_gnix_ref_put(cm_nic->nic);
		cm_nic->nic = NULL;
	}

	cm_nic->domain->cm_nic = NULL;
	free(cm_nic);
}

static int __gnix_cm_nic_intra_progress_fn(void *data, int *complete_ptr)
{
	struct gnix_datagram *dgram;
	struct gnix_cm_nic *cm_nic;
	int ret;

	GNIX_INFO(FI_LOG_EP_CTRL, "\n");

	dgram = (struct gnix_datagram *)data;
	cm_nic = (struct gnix_cm_nic *)dgram->cache;
	ret = __process_datagram(dgram,
				 cm_nic->my_name.gnix_addr,
				 GNI_POST_COMPLETED);
	if (ret == FI_SUCCESS) {
		GNIX_INFO(FI_LOG_EP_CTRL, "Intra-CM NIC dgram completed\n");
		*complete_ptr = 1;
	}

	return FI_SUCCESS;
}

int _gnix_cm_nic_send(struct gnix_cm_nic *cm_nic,
		      char *sbuf, size_t len,
		      struct gnix_address target_addr)
{
	int ret = FI_SUCCESS;
	struct gnix_datagram *dgram = NULL;
	ssize_t  __attribute__((unused)) plen;
	uint8_t tag;
	struct gnix_work_req *work_req;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if ((cm_nic == NULL) || (sbuf == NULL))
		return -FI_EINVAL;

	if (len > GNI_DATAGRAM_MAXSIZE)
		return -FI_ENOSPC;

	ret = _gnix_dgram_alloc(cm_nic->dgram_hndl,
				GNIX_DGRAM_BND,
				&dgram);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "_gnix_dgram_alloc returned %s\n",
			  fi_strerror(-ret));
		goto exit;
	}

	dgram->target_addr = target_addr;
	dgram->callback_fn = __process_datagram;
	dgram->cache = cm_nic;

	tag = GNIX_CM_NIC_BND_TAG;
	 __dgram_set_tag(dgram, tag);

	plen = _gnix_dgram_pack_buf(dgram, GNIX_DGRAM_IN_BUF,
				   sbuf, len);
	assert (plen == len);

	/* If connecting with the same CM NIC, skip datagram exchange.  The
	 * caller could be holding an endpoint lock, so schedule connection
	 * completion for later. */
	if (GNIX_ADDR_EQUAL(target_addr, cm_nic->my_name.gnix_addr)) {
		char tmp_buf[GNIX_CM_NIC_MAX_MSG_SIZE];

		/* Pack output buffer with input data. */
		_gnix_dgram_unpack_buf(dgram, GNIX_DGRAM_IN_BUF, tmp_buf,
				       GNIX_CM_NIC_MAX_MSG_SIZE);
		_gnix_dgram_pack_buf(dgram, GNIX_DGRAM_OUT_BUF, tmp_buf,
				       GNIX_CM_NIC_MAX_MSG_SIZE);

		work_req = calloc(1, sizeof(*work_req));
		if (work_req == NULL) {
			_gnix_dgram_free(dgram);
			return -FI_ENOMEM;
		}

		work_req->progress_fn = __gnix_cm_nic_intra_progress_fn;
		work_req->data = dgram;
		work_req->completer_fn = NULL;

		fastlock_acquire(&cm_nic->wq_lock);
		dlist_insert_before(&work_req->list, &cm_nic->cm_nic_wq);
		fastlock_release(&cm_nic->wq_lock);

		GNIX_INFO(FI_LOG_EP_CTRL, "Initiated intra-CM NIC connect\n");
	} else {
		ret = _gnix_dgram_bnd_post(dgram);
		if (ret == -FI_EBUSY) {
			ret = -FI_EAGAIN;
			_gnix_dgram_free(dgram);
		}
	}

exit:
	return ret;
}

int _gnix_cm_nic_reg_recv_fn(struct gnix_cm_nic *cm_nic,
			     gnix_cm_nic_rcv_cb_func *recv_fn,
			     gnix_cm_nic_rcv_cb_func **prev_fn)
{
	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (cm_nic == NULL)
		return -FI_EINVAL;

	*prev_fn = cm_nic->rcv_cb_fn;
	cm_nic->rcv_cb_fn = recv_fn;

	return ret;
}

int _gnix_cm_nic_enable(struct gnix_cm_nic *cm_nic)
{
	int i, ret = FI_SUCCESS;
	struct gnix_fid_fabric *fabric;
	struct gnix_datagram *dg_ptr;
	uint8_t tag = GNIX_CM_NIC_WC_TAG;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (cm_nic == NULL)
		return -FI_EINVAL;

	if (cm_nic->domain == NULL) {
		GNIX_FATAL(FI_LOG_EP_CTRL, "domain is NULL\n");
	}

	if (cm_nic->domain->fabric == NULL) {
		GNIX_FATAL(FI_LOG_EP_CTRL, "fabric is NULL\n");
	}

	fabric = cm_nic->domain->fabric;

	assert(cm_nic->dgram_hndl != NULL);

	for (i = 0; i < fabric->n_wc_dgrams; i++) {
		ret = _gnix_dgram_alloc(cm_nic->dgram_hndl, GNIX_DGRAM_WC,
					&dg_ptr);

		/*
 		 * wildcards may already be posted to the cm_nic,
 		 * so just break if -FI_EAGAIN is returned by
 		 * _gnix_dgram_alloc
 		 */

		if (ret == -FI_EAGAIN) {
			ret = FI_SUCCESS;
			break;
		}

		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
			     "_gnix_dgram_alloc call returned %d\n", ret);
				goto err;
		}

		dg_ptr->callback_fn = __process_datagram;
		dg_ptr->cache = cm_nic;
		 __dgram_set_tag(dg_ptr, tag);

		ret = _gnix_dgram_wc_post(dg_ptr);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				"_gnix_dgram_wc_post returned %d\n", ret);
			_gnix_dgram_free(dg_ptr);
			goto err;
		}
	}

	/*
	 * TODO: better cleanup in error case
	 */
err:
	return ret;
}

int _gnix_cm_nic_free(struct gnix_cm_nic *cm_nic)
{

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (cm_nic == NULL)
		return -FI_EINVAL;

	_gnix_ref_put(cm_nic);

	return FI_SUCCESS;
}

int _gnix_cm_nic_alloc(struct gnix_fid_domain *domain,
		       struct fi_info *info,
		       uint32_t cdm_id,
			   struct gnix_auth_key *auth_key,
		       struct gnix_cm_nic **cm_nic_ptr)
{
	int ret = FI_SUCCESS;
	struct gnix_cm_nic *cm_nic = NULL;
	gnix_hashtable_attr_t gnix_ht_attr = {0};
	uint32_t name_type = GNIX_EPN_TYPE_UNBOUND;
	struct gnix_nic_attr nic_attr = {0};
	struct gnix_ep_name ep_name;
	struct gnix_dgram_hndl_attr dgram_hndl_attr = {0};
	struct gnix_dgram_hndl_attr *dgram_hndl_attr_ptr = NULL;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	*cm_nic_ptr = NULL;

	/*
	 * if app has specified a src_addr in the info
	 * argument and length matches that for gnix_ep_name
	 * we must allocate a cm_nic, otherwise we first
	 * check to see if there is a cm_nic already for this domain
	 * and just use it.
	 */

	if (info->src_addr) {
		/*TODO (optimization): strchr to name_type and strtol */
		_gnix_get_ep_name(info->src_addr, 0, &ep_name, domain);
		name_type = ep_name.name_type;
	}

	GNIX_INFO(FI_LOG_EP_CTRL, "creating cm_nic for %u/0x%x/%u\n",
			auth_key->ptag, auth_key->cookie, cdm_id);

	cm_nic = (struct gnix_cm_nic *)calloc(1, sizeof(*cm_nic));
	if (cm_nic == NULL) {
		ret = -FI_ENOMEM;
		goto err;
	}

	/*
	 * we have to force allocation of a new nic since we want
	 * an a particular cdm id
	 */
	nic_attr.must_alloc = true;
	nic_attr.use_cdm_id = true;
	nic_attr.cdm_id = cdm_id;
	nic_attr.auth_key = auth_key;

	ret = gnix_nic_alloc(domain, &nic_attr, &cm_nic->nic);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "gnix_nic_alloc returned %s\n",
			  fi_strerror(-ret));
		goto err;
	}

	cm_nic->my_name.gnix_addr.cdm_id = cdm_id;
	cm_nic->ptag = auth_key->ptag;
	cm_nic->my_name.cookie = auth_key->cookie;
	cm_nic->my_name.gnix_addr.device_addr = cm_nic->nic->device_addr;
	cm_nic->domain = domain;
	cm_nic->ctrl_progress = domain->control_progress;
	cm_nic->my_name.name_type = name_type;
	cm_nic->poll_cnt = 0;
	fastlock_init(&cm_nic->wq_lock);
	dlist_init(&cm_nic->cm_nic_wq);

	/*
	 * prep the cm nic's dgram component
	 */
	if (domain->control_progress == FI_PROGRESS_AUTO) {
		dgram_hndl_attr.timeout_needed = __gnix_cm_nic_timeout_needed;
		dgram_hndl_attr.timeout_progress = __gnix_cm_nic_timeout_progress;
		dgram_hndl_attr.timeout_data = (void *)cm_nic;
		dgram_hndl_attr.timeout = domain->params.dgram_progress_timeout;
		dgram_hndl_attr_ptr = &dgram_hndl_attr;
	};

	ret = _gnix_dgram_hndl_alloc(cm_nic,
				     domain->control_progress,
				     dgram_hndl_attr_ptr,
				     &cm_nic->dgram_hndl);
	if (ret != FI_SUCCESS)
		goto err;

	/*
	 * allocate hash table for translating ep addresses
	 * to ep's.
	 * This table will not be large - how many FI_EP_RDM ep's
	 * will an app create using one domain?, nor in the critical path
	 * so just use defaults.
	 */
	cm_nic->addr_to_ep_ht = calloc(1, sizeof(struct gnix_hashtable));
	if (cm_nic->addr_to_ep_ht == NULL)
		goto err;

	gnix_ht_attr.ht_initial_size = 64;
	gnix_ht_attr.ht_maximum_size = 1024;
	gnix_ht_attr.ht_increase_step = 2;
	gnix_ht_attr.ht_increase_type = GNIX_HT_INCREASE_MULT;
	gnix_ht_attr.ht_collision_thresh = 500;
	gnix_ht_attr.ht_hash_seed = 0xdeadbeefbeefdead;
	gnix_ht_attr.ht_internal_locking = 1;
	gnix_ht_attr.destructor = NULL;

	ret = _gnix_ht_init(cm_nic->addr_to_ep_ht, &gnix_ht_attr);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "gnix_ht_init returned %s\n",
			  fi_strerror(-ret));
		goto err;
	}

	_gnix_ref_init(&cm_nic->ref_cnt, 1, __cm_nic_destruct);

	*cm_nic_ptr = cm_nic;

	pthread_mutex_lock(&gnix_cm_nic_list_lock);
	dlist_insert_tail(&cm_nic->cm_nic_list, &gnix_cm_nic_list);
	pthread_mutex_unlock(&gnix_cm_nic_list_lock);

	return ret;

err:
	if (cm_nic->dgram_hndl)
		_gnix_dgram_hndl_free(cm_nic->dgram_hndl);

	if (cm_nic->nic)
		_gnix_nic_free(cm_nic->nic);

	if (cm_nic->addr_to_ep_ht) {
		_gnix_ht_destroy(cm_nic->addr_to_ep_ht);
		free(cm_nic->addr_to_ep_ht);
	}

	if (cm_nic != NULL)
		free(cm_nic);

	return ret;
}

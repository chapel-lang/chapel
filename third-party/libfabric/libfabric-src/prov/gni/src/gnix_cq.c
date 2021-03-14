/*
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2019 Triad National Security, LLC. All rights reserved.
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
 * CQ common code
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gnix.h"
#include "gnix_cq.h"
#include "gnix_nic.h"
#include "gnix_cm_nic.h"

/*******************************************************************************
 * Function pointer for filling specific entry format type.
 ******************************************************************************/
typedef void (*fill_entry)(void *cq_entry, void *op_context, uint64_t flags,
			   size_t len, void *buf, uint64_t data, uint64_t tag);

/*******************************************************************************
 * Forward declarations for filling functions.
 ******************************************************************************/
static void fill_cq_entry(void *cq_entry, void *op_context, uint64_t flags,
			  size_t len, void *buf, uint64_t data, uint64_t tag);
static void fill_cq_msg(void *cq_entry, void *op_context, uint64_t flags,
			size_t len, void *buf, uint64_t data, uint64_t tag);
static void fill_cq_data(void *cq_entry, void *op_context, uint64_t flags,
			size_t len, void *buf, uint64_t data, uint64_t tag);
static void fill_cq_tagged(void *cq_entry, void *op_context, uint64_t flags,
			size_t len, void *buf, uint64_t data, uint64_t tag);

/*******************************************************************************
 * Forward declarations for ops structures.
 ******************************************************************************/
static const struct fi_ops gnix_cq_fi_ops;
static const struct fi_ops_cq gnix_cq_ops;

/*******************************************************************************
 * Size array corresponding format type to format size.
 ******************************************************************************/
static const size_t format_sizes[] = {
	[FI_CQ_FORMAT_UNSPEC]  = sizeof(GNIX_CQ_DEFAULT_FORMAT),
	[FI_CQ_FORMAT_CONTEXT] = sizeof(struct fi_cq_entry),
	[FI_CQ_FORMAT_MSG]     = sizeof(struct fi_cq_msg_entry),
	[FI_CQ_FORMAT_DATA]    = sizeof(struct fi_cq_data_entry),
	[FI_CQ_FORMAT_TAGGED]  = sizeof(struct fi_cq_tagged_entry)
};

static const fill_entry fill_function[] = {
	[FI_CQ_FORMAT_UNSPEC]  = fill_cq_entry,
	[FI_CQ_FORMAT_CONTEXT] = fill_cq_entry,
	[FI_CQ_FORMAT_MSG]     = fill_cq_msg,
	[FI_CQ_FORMAT_DATA]    = fill_cq_data,
	[FI_CQ_FORMAT_TAGGED]  = fill_cq_tagged
};

/*******************************************************************************
 * Internal helper functions
 ******************************************************************************/
static void fill_cq_entry(void *cq_entry, void *op_context, uint64_t flags,
			  size_t len, void *buf, uint64_t data, uint64_t tag)
{
	struct fi_cq_entry *entry = cq_entry;

	entry->op_context = op_context;
}

static void fill_cq_msg(void *cq_entry, void *op_context, uint64_t flags,
			size_t len, void *buf, uint64_t data, uint64_t tag)
{
	struct fi_cq_msg_entry *entry = cq_entry;

	entry->op_context = op_context;
	entry->flags = flags;
	entry->len = len;
}

static void fill_cq_data(void *cq_entry, void *op_context, uint64_t flags,
			 size_t len, void *buf, uint64_t data, uint64_t tag)
{
	struct fi_cq_data_entry *entry = cq_entry;

	entry->op_context = op_context;
	entry->flags = flags;
	entry->len = len;
	entry->buf = buf;
	entry->data = data;
}

static void fill_cq_tagged(void *cq_entry, void *op_context, uint64_t flags,
			   size_t len, void *buf, uint64_t data, uint64_t tag)
{
	struct fi_cq_tagged_entry *entry = cq_entry;

	entry->op_context = op_context;
	entry->flags = flags;
	entry->buf = buf;
	entry->data = data;
	entry->tag = tag;
	entry->len = len;
}

static int verify_cq_attr(struct fi_cq_attr *attr, struct fi_ops_cq *ops,
			  struct fi_ops *fi_cq_ops)
{
	GNIX_TRACE(FI_LOG_CQ, "\n");

	if (!attr || !ops || !fi_cq_ops)
		return -FI_EINVAL;

	if (!attr->size)
		attr->size = GNIX_CQ_DEFAULT_SIZE;

	switch (attr->format) {
	case FI_CQ_FORMAT_UNSPEC:
		attr->format = FI_CQ_FORMAT_CONTEXT;
	case FI_CQ_FORMAT_CONTEXT:
	case FI_CQ_FORMAT_MSG:
	case FI_CQ_FORMAT_DATA:
	case FI_CQ_FORMAT_TAGGED:
		break;
	default:
		GNIX_WARN(FI_LOG_CQ, "format: %d unsupported.\n",
			  attr->format);
		return -FI_EINVAL;
	}

	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
		ops->sread = fi_no_cq_sread;
		ops->signal = fi_no_cq_signal;
		ops->sreadfrom = fi_no_cq_sreadfrom;
		fi_cq_ops->control = fi_no_control;
		break;
	case FI_WAIT_SET:
		if (!attr->wait_set) {
			GNIX_WARN(FI_LOG_CQ,
				  "FI_WAIT_SET is set, but wait_set field doesn't reference a wait object.\n");
			return -FI_EINVAL;
		}
		break;
	case FI_WAIT_UNSPEC:
		break;
	case FI_WAIT_FD:
	case FI_WAIT_MUTEX_COND:
	default:
		GNIX_WARN(FI_LOG_CQ, "wait type: %d unsupported.\n",
			  attr->wait_obj);
		return -FI_EINVAL;
	}

	return FI_SUCCESS;
}

static int gnix_cq_set_wait(struct gnix_fid_cq *cq)
{
	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_CQ, "\n");

	struct fi_wait_attr requested = {
		.wait_obj = cq->attr.wait_obj,
		.flags = 0
	};

	switch (cq->attr.wait_obj) {
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
	case FI_WAIT_MUTEX_COND:
		ret = gnix_wait_open(&cq->domain->fabric->fab_fid,
				&requested, &cq->wait);
		break;
	case FI_WAIT_SET:
		ret = _gnix_wait_set_add(cq->attr.wait_set, &cq->cq_fid.fid);
		if (!ret)
			cq->wait = cq->attr.wait_set;

		break;
	default:
		break;
	}

	return ret;
}

static void free_cq_entry(struct slist_entry *item)
{
	struct gnix_cq_entry *entry;

	entry = container_of(item, struct gnix_cq_entry, item);

	free(entry->the_entry);
	free(entry);
}

static struct slist_entry *alloc_cq_entry(size_t size)
{
	struct gnix_cq_entry *entry = malloc(sizeof(*entry));

	if (!entry) {
		GNIX_DEBUG(FI_LOG_CQ, "out of memory\n");
		goto err;
	}

	entry->the_entry = malloc(size);
	if (!entry->the_entry) {
		GNIX_DEBUG(FI_LOG_CQ, "out of memory\n");
		goto cleanup;
	}

	return &entry->item;

cleanup:
	free(entry);
err:
	return NULL;
}

static int __gnix_cq_progress(struct gnix_fid_cq *cq)
{
	return _gnix_prog_progress(&cq->pset);
}

/*******************************************************************************
 * Exposed helper functions
 ******************************************************************************/
ssize_t _gnix_cq_add_event(struct gnix_fid_cq *cq, struct gnix_fid_ep *ep,
			   void *op_context, uint64_t flags, size_t len,
			   void *buf, uint64_t data, uint64_t tag,
			   fi_addr_t src_addr)
{
	struct gnix_cq_entry *event;
	struct slist_entry *item;
	uint64_t mask;
	ssize_t ret = FI_SUCCESS;

	if (ep) {
		if (ep->info && ep->info->mode & FI_NOTIFY_FLAGS_ONLY) {
			mask = (FI_REMOTE_CQ_DATA | FI_MULTI_RECV);

			if (flags & FI_RMA_EVENT) {
				mask |= (FI_REMOTE_READ | FI_REMOTE_WRITE |
					 FI_RMA);
			}

			flags &= mask;
		}
	}

	COND_ACQUIRE(cq->requires_lock, &cq->lock);

	item = _gnix_queue_get_free(cq->events);
	if (!item) {
		GNIX_DEBUG(FI_LOG_CQ, "error creating cq_entry\n");
		ret = -FI_ENOMEM;
		goto err;
	}

	event = container_of(item, struct gnix_cq_entry, item);

	assert(event->the_entry);

	fill_function[cq->attr.format](event->the_entry, op_context, flags,
			len, buf, data, tag);
	event->src_addr = src_addr;

	_gnix_queue_enqueue(cq->events, &event->item);
	GNIX_DEBUG(FI_LOG_CQ, "Added event: %lx\n", op_context);

	if (cq->wait)
		_gnix_signal_wait_obj(cq->wait);

err:
	COND_RELEASE(cq->requires_lock, &cq->lock);

	return ret;
}

ssize_t _gnix_cq_add_error(struct gnix_fid_cq *cq, void *op_context,
			   uint64_t flags, size_t len, void *buf,
			   uint64_t data, uint64_t tag, size_t olen,
			   int err, int prov_errno, void *err_data,
			   size_t err_data_size)
{
	struct fi_cq_err_entry *error;
	struct gnix_cq_entry *event;
	struct slist_entry *item;

	ssize_t ret = FI_SUCCESS;

	GNIX_INFO(FI_LOG_CQ, "creating error event entry\n");


	COND_ACQUIRE(cq->requires_lock, &cq->lock);

	item = _gnix_queue_get_free(cq->errors);
	if (!item) {
		GNIX_WARN(FI_LOG_CQ, "error creating error entry\n");
		ret = -FI_ENOMEM;
		goto err;
	}

	event = container_of(item, struct gnix_cq_entry, item);

	error = event->the_entry;

	error->op_context = op_context;
	error->flags = flags;
	error->len = len;
	error->buf = buf;
	error->data = data;
	error->tag = tag;
	error->olen = olen;
	error->err = err;
	error->prov_errno = prov_errno;
	error->err_data = err_data;
	error->err_data_size = err_data_size;

	_gnix_queue_enqueue(cq->errors, &event->item);

	if (cq->wait)
		_gnix_signal_wait_obj(cq->wait);

err:
	COND_RELEASE(cq->requires_lock, &cq->lock);

	return ret;
}

int _gnix_cq_poll_obj_add(struct gnix_fid_cq *cq, void *obj,
			  int (*prog_fn)(void *data))
{
	return _gnix_prog_obj_add(&cq->pset, obj, prog_fn);
}

int _gnix_cq_poll_obj_rem(struct gnix_fid_cq *cq, void *obj,
			  int (*prog_fn)(void *data))
{
	return _gnix_prog_obj_rem(&cq->pset, obj, prog_fn);
}

static void __cq_destruct(void *obj)
{
	struct gnix_fid_cq *cq = (struct gnix_fid_cq *) obj;

	_gnix_ref_put(cq->domain);

	switch (cq->attr.wait_obj) {
	case FI_WAIT_NONE:
		break;
	case FI_WAIT_SET:
		_gnix_wait_set_remove(cq->wait, &cq->cq_fid.fid);
		break;
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
	case FI_WAIT_MUTEX_COND:
		assert(cq->wait);
		gnix_wait_close(&cq->wait->fid);
		break;
	default:
		GNIX_WARN(FI_LOG_CQ, "format: %d unsupported.\n",
			  cq->attr.wait_obj);
		break;
	}

	_gnix_prog_fini(&cq->pset);

	_gnix_queue_destroy(cq->events);
	_gnix_queue_destroy(cq->errors);

	fastlock_destroy(&cq->lock);
	free(cq->cq_fid.ops);
	free(cq->cq_fid.fid.ops);
	free(cq);
}

/*******************************************************************************
 * API functions.
 ******************************************************************************/
static int gnix_cq_close(fid_t fid)
{
	struct gnix_fid_cq *cq;
	int references_held;

	GNIX_TRACE(FI_LOG_CQ, "\n");

	cq = container_of(fid, struct gnix_fid_cq, cq_fid);

	references_held = _gnix_ref_put(cq);

	if (references_held) {
		GNIX_INFO(FI_LOG_CQ, "failed to fully close cq due to lingering "
				"references. references=%i cq=%p\n",
				references_held, cq);
	}

	return FI_SUCCESS;
}

static ssize_t __gnix_cq_readfrom(struct fid_cq *cq, void *buf,
					  size_t count, fi_addr_t *src_addr)
{
	struct gnix_fid_cq *cq_priv;
	struct gnix_cq_entry *event;
	struct slist_entry *temp;

	ssize_t read_count = 0;

	if (!cq || !buf || !count)
		return -FI_EINVAL;

	cq_priv = container_of(cq, struct gnix_fid_cq, cq_fid);

	__gnix_cq_progress(cq_priv);

	if (_gnix_queue_peek(cq_priv->errors))
		return -FI_EAVAIL;

	COND_ACQUIRE(cq_priv->requires_lock, &cq_priv->lock);

	while (_gnix_queue_peek(cq_priv->events) && count--) {
		temp = _gnix_queue_dequeue(cq_priv->events);
		event = container_of(temp, struct gnix_cq_entry, item);

		assert(event->the_entry);
		memcpy(buf, event->the_entry, cq_priv->entry_size);
		if (src_addr)
			memcpy(&src_addr[read_count], &event->src_addr, sizeof(fi_addr_t));

		_gnix_queue_enqueue_free(cq_priv->events, &event->item);

		buf = (void *) ((uint8_t *) buf + cq_priv->entry_size);

		read_count++;
	}

	COND_RELEASE(cq_priv->requires_lock, &cq_priv->lock);

	return read_count ?: -FI_EAGAIN;
}

static ssize_t __gnix_cq_sreadfrom(int blocking, struct fid_cq *cq, void *buf,
				     size_t count, fi_addr_t *src_addr,
				     const void *cond, int timeout)
{
	struct gnix_fid_cq *cq_priv;

	cq_priv = container_of(cq, struct gnix_fid_cq, cq_fid);
	if ((blocking && !cq_priv->wait) ||
	    (blocking && cq_priv->attr.wait_obj == FI_WAIT_SET))
		return -FI_EINVAL;

	if (_gnix_queue_peek(cq_priv->errors))
		return -FI_EAVAIL;

	if (cq_priv->wait)
		gnix_wait_wait((struct fid_wait *)cq_priv->wait, timeout);


	return __gnix_cq_readfrom(cq, buf, count, src_addr);

}

DIRECT_FN STATIC ssize_t gnix_cq_sreadfrom(struct fid_cq *cq, void *buf,
					   size_t count, fi_addr_t *src_addr,
					   const void *cond, int timeout)
{
	return __gnix_cq_sreadfrom(1, cq, buf, count, src_addr, cond, timeout);
}

DIRECT_FN STATIC ssize_t gnix_cq_read(struct fid_cq *cq,
				      void *buf,
				      size_t count)
{
	return __gnix_cq_sreadfrom(0, cq, buf, count, NULL, NULL, 0);
}

DIRECT_FN STATIC ssize_t gnix_cq_sread(struct fid_cq *cq, void *buf,
				       size_t count, const void *cond,
				       int timeout)
{
	return __gnix_cq_sreadfrom(1, cq, buf, count, NULL, cond, timeout);
}

DIRECT_FN STATIC ssize_t gnix_cq_readfrom(struct fid_cq *cq, void *buf,
					  size_t count, fi_addr_t *src_addr)
{
	return __gnix_cq_sreadfrom(0, cq, buf, count, src_addr, NULL, 0);
}

DIRECT_FN STATIC ssize_t gnix_cq_readerr(struct fid_cq *cq,
					 struct fi_cq_err_entry *buf,
					 uint64_t flags)
{
	struct gnix_fid_cq *cq_priv;
	struct gnix_cq_entry *event;
	struct slist_entry *entry;
	size_t err_data_cpylen;
	struct fi_cq_err_entry *gnix_cq_err;

	ssize_t read_count = 0;

	if (!cq || !buf)
		return -FI_EINVAL;

	cq_priv = container_of(cq, struct gnix_fid_cq, cq_fid);

	/*
	 * we need to progress cq.  some apps may be only using
	 * cq to check for errors.
	 */

	_gnix_prog_progress(&cq_priv->pset);

	COND_ACQUIRE(cq_priv->requires_lock, &cq_priv->lock);

	entry = _gnix_queue_dequeue(cq_priv->errors);
	if (!entry) {
		read_count = -FI_EAGAIN;
		goto err;
	}

	event = container_of(entry, struct gnix_cq_entry, item);
	gnix_cq_err = event->the_entry;

	buf->op_context = gnix_cq_err->op_context;
	buf->flags = gnix_cq_err->flags;
	buf->len = gnix_cq_err->len;
	buf->buf = gnix_cq_err->buf;
	buf->data = gnix_cq_err->data;
	buf->tag = gnix_cq_err->tag;
	buf->olen = gnix_cq_err->olen;
	buf->err = gnix_cq_err->err;
	buf->prov_errno = gnix_cq_err->prov_errno;

	if (gnix_cq_err->err_data != NULL) {
		/*
		 * Note: If the api version is >= 1.5 then copy err_data into
		 * buf->err_data and copy at most buf->err_data_size.
		 * If buf->err_data_size is zero or the api version is < 1.5,
		 * use the old method of allocating space in provider.
		 */
		if (FI_VERSION_LT(cq_priv->domain->fabric->fab_fid.api_version,
		    FI_VERSION(1, 5)) || buf->err_data_size == 0) {
			err_data_cpylen = sizeof(cq_priv->err_data);

			memcpy(cq_priv->err_data, gnix_cq_err->err_data,
				err_data_cpylen);

			buf->err_data = cq_priv->err_data;
		} else {
			if (buf->err_data == NULL)
				return -FI_EINVAL;

			err_data_cpylen = MIN(buf->err_data_size,
						gnix_cq_err->err_data_size);
			memcpy(buf->err_data, gnix_cq_err->err_data, err_data_cpylen);
			buf->err_data_size = err_data_cpylen;
		}
		free(gnix_cq_err->err_data);
		gnix_cq_err->err_data = NULL;
	} else {
		if (FI_VERSION_LT(cq_priv->domain->fabric->fab_fid.api_version,
		    FI_VERSION(1, 5))) {
			buf->err_data = NULL;
		} else {
			buf->err_data_size = 0;
		}
	}

	_gnix_queue_enqueue_free(cq_priv->errors, &event->item);

	read_count++;

err:
	COND_RELEASE(cq_priv->requires_lock, &cq_priv->lock);

	return read_count;
}

DIRECT_FN STATIC const char *gnix_cq_strerror(struct fid_cq *cq, int prov_errno,
					      const void *prov_data, char *buf,
					      size_t len)
{
	return NULL;
}

DIRECT_FN STATIC int gnix_cq_signal(struct fid_cq *cq)
{
	struct gnix_fid_cq *cq_priv;

	cq_priv = container_of(cq, struct gnix_fid_cq, cq_fid);

	if (cq_priv->wait)
		_gnix_signal_wait_obj(cq_priv->wait);

	return FI_SUCCESS;
}

static int gnix_cq_control(struct fid *cq, int command, void *arg)
{

	switch (command) {
	case FI_GETWAIT:
		return -FI_ENOSYS;
	default:
		return -FI_EINVAL;
	}
}


DIRECT_FN int gnix_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
			   struct fid_cq **cq, void *context)
{
	struct gnix_fid_domain *domain_priv;
	struct gnix_fid_cq *cq_priv;
	struct fi_ops_cq *cq_ops;
	struct fi_ops *fi_cq_ops;

	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_CQ, "\n");

	cq_ops = calloc(1, sizeof(*cq_ops));
	if (!cq_ops) {
		return -FI_ENOMEM;
	}

	fi_cq_ops = calloc(1, sizeof(*fi_cq_ops));
	if (!fi_cq_ops) {
		ret = -FI_ENOMEM;
		goto free_cq_ops;
	}

	*cq_ops = gnix_cq_ops;
	*fi_cq_ops = gnix_cq_fi_ops;

	ret = verify_cq_attr(attr, cq_ops, fi_cq_ops);
	if (ret)
		goto free_fi_cq_ops;

	domain_priv = container_of(domain, struct gnix_fid_domain, domain_fid);
	if (!domain_priv) {
		ret = -FI_EINVAL;
		goto free_fi_cq_ops;
	}

	cq_priv = calloc(1, sizeof(*cq_priv));
	if (!cq_priv) {
		ret = -FI_ENOMEM;
		goto free_fi_cq_ops;
	}

	cq_priv->requires_lock = (domain_priv->thread_model !=
			FI_THREAD_COMPLETION);

	cq_priv->domain = domain_priv;
	cq_priv->attr = *attr;

	_gnix_ref_init(&cq_priv->ref_cnt, 1, __cq_destruct);
	_gnix_ref_get(cq_priv->domain);

	_gnix_prog_init(&cq_priv->pset);

	cq_priv->cq_fid.fid.fclass = FI_CLASS_CQ;
	cq_priv->cq_fid.fid.context = context;
	cq_priv->cq_fid.fid.ops = fi_cq_ops;
	cq_priv->cq_fid.ops = cq_ops;

	/*
	 * Although we don't need to store entry_size since we're already
	 * storing the format, this might provide a performance benefit
	 * when allocating storage.
	 */
	cq_priv->entry_size = format_sizes[cq_priv->attr.format];

	fastlock_init(&cq_priv->lock);
	ret = gnix_cq_set_wait(cq_priv);
	if (ret)
		goto free_cq_priv;

	ret = _gnix_queue_create(&cq_priv->events, alloc_cq_entry,
				 free_cq_entry, cq_priv->entry_size,
				 cq_priv->attr.size);
	if (ret)
		goto free_cq_priv;

	ret = _gnix_queue_create(&cq_priv->errors, alloc_cq_entry,
				 free_cq_entry, sizeof(struct fi_cq_err_entry),
				 0);
	if (ret)
		goto free_gnix_queue;

	*cq = &cq_priv->cq_fid;
	return ret;

free_gnix_queue:
	_gnix_queue_destroy(cq_priv->events);
free_cq_priv:
	_gnix_ref_put(cq_priv->domain);
	fastlock_destroy(&cq_priv->lock);
	free(cq_priv);
free_fi_cq_ops:
	free(fi_cq_ops);
free_cq_ops:
	free(cq_ops);

	return ret;
}


/*******************************************************************************
 * FI_OPS_* data structures.
 ******************************************************************************/
static const struct fi_ops gnix_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = gnix_cq_close,
	.bind = fi_no_bind,
	.control = gnix_cq_control,
	.ops_open = fi_no_ops_open
};

static const struct fi_ops_cq gnix_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = gnix_cq_read,
	.readfrom = gnix_cq_readfrom,
	.readerr = gnix_cq_readerr,
	.sread = gnix_cq_sread,
	.sreadfrom = gnix_cq_sreadfrom,
	.signal = gnix_cq_signal,
	.strerror = gnix_cq_strerror
};

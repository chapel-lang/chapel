/*
 * Copyright (c) 2015-2016 Cray Inc.  All rights reserved.
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
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

#include <assert.h>

#include <stdlib.h>

#include "gnix.h"
#include "gnix_eq.h"
#include "gnix_util.h"
#include "gnix_cm.h"

/*******************************************************************************
 * Global declarations
 ******************************************************************************/
DLIST_HEAD(gnix_eq_list);
pthread_mutex_t gnix_eq_list_lock = PTHREAD_MUTEX_INITIALIZER;

/*******************************************************************************
 * Forward declaration for ops structures.
 ******************************************************************************/
static struct fi_ops_eq gnix_eq_ops;
static struct fi_ops gnix_fi_eq_ops;


/*******************************************************************************
 * Helper functions.
 ******************************************************************************/

static void gnix_eq_cleanup_err_bufs(struct gnix_fid_eq *eq, int free_all)
{
	struct gnix_eq_err_buf *ebuf, *tmp;

	dlist_for_each_safe(&eq->err_bufs, ebuf, tmp, dlist) {
		if (free_all || ebuf->do_free) {
			dlist_remove(&ebuf->dlist);
			free(ebuf);
		}
	}
}

static int gnix_eq_set_wait(struct gnix_fid_eq *eq)
{
	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_EQ, "\n");

	struct fi_wait_attr requested = {
		.wait_obj = eq->attr.wait_obj,
		.flags = 0
	};

	switch (eq->attr.wait_obj) {
	case FI_WAIT_UNSPEC:
		ret = gnix_wait_open(&eq->fabric->fab_fid, &requested,
				     &eq->wait);
		break;
	case FI_WAIT_SET:
		ret = _gnix_wait_set_add(eq->attr.wait_set, &eq->eq_fid.fid);
		if (!ret)
			eq->wait = eq->attr.wait_set;
		break;
	default:
		break;
	}

	return ret;
}

static int gnix_verify_eq_attr(struct fi_eq_attr *attr)
{

	GNIX_TRACE(FI_LOG_EQ, "\n");

	if (!attr)
		return -FI_EINVAL;

	if (!attr->size)
		attr->size = GNIX_EQ_DEFAULT_SIZE;

	/*
	 * We only support FI_WAIT_SET and FI_WAIT_UNSPEC
	 */
	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
		break;
	case FI_WAIT_SET:
		if (!attr->wait_set) {
			GNIX_WARN(FI_LOG_EQ,
				  "FI_WAIT_SET is set, but wait_set field doesn't reference a wait object.\n");
			return -FI_EINVAL;
		}
		break;
	case FI_WAIT_UNSPEC:
		break;
	case FI_WAIT_FD:
	case FI_WAIT_MUTEX_COND:
	default:
		GNIX_WARN(FI_LOG_EQ, "wait type: %d unsupported.\n",
			  attr->wait_obj);
		return -FI_ENOSYS;
	}

	return FI_SUCCESS;
}

static void free_eq_entry(struct slist_entry *item)
{
	struct gnix_eq_entry *entry;

	entry = container_of(item, struct gnix_eq_entry, item);

	free(entry->the_entry);
	free(entry);
}

static struct slist_entry *alloc_eq_entry(size_t size)
{
	struct gnix_eq_entry *entry = calloc(1, sizeof(*entry));

	if (!entry) {
		GNIX_WARN(FI_LOG_EQ, "out of memory\n");
		goto err;
	}

	if (size) {
		entry->the_entry = malloc(size);
		if (!entry->the_entry) {
			GNIX_WARN(FI_LOG_EQ, "out of memory\n");
			goto cleanup;
		}
	}

	return &entry->item;

cleanup:
	free(entry);
err:
	return NULL;
}

ssize_t _gnix_eq_write_error(struct gnix_fid_eq *eq, fid_t fid,
			     void *context, uint64_t index, int err,
			     int prov_errno, void *err_data,
			     size_t err_size)
{
	struct fi_eq_err_entry *error;
	struct gnix_eq_entry *event;
	struct slist_entry *item;
	struct gnix_eq_err_buf *err_buf;

	ssize_t ret = FI_SUCCESS;

	if (!eq)
		return -FI_EINVAL;

	fastlock_acquire(&eq->lock);

	item = _gnix_queue_get_free(eq->errors);
	if (!item) {
		GNIX_WARN(FI_LOG_EQ, "error creating error entry\n");
		ret = -FI_ENOMEM;
		goto err;
	}

	event = container_of(item, struct gnix_eq_entry, item);

	error = event->the_entry;

	error->fid = fid;
	error->context = context;
	error->data = index;
	error->err = err;
	error->prov_errno = prov_errno;

	if (err_size) {
		err_buf = malloc(sizeof(struct gnix_eq_err_buf) + err_size);
		if (!err_buf) {
			_gnix_queue_enqueue_free(eq->errors, &event->item);
			ret = -FI_ENOMEM;
			goto err;
		}
		err_buf->do_free = 0;

		memcpy(err_buf->buf, err_data, err_size);
		error->err_data = err_buf->buf;
		error->err_data_size = err_size;

		dlist_insert_tail(&err_buf->dlist, &eq->err_bufs);
	} else {
		error->err_data = NULL;
		error->err_data_size = 0;
	}

	_gnix_queue_enqueue(eq->errors, &event->item);

	if (eq->wait)
		_gnix_signal_wait_obj(eq->wait);

err:
	fastlock_release(&eq->lock);

	return ret;
}

static void __eq_destruct(void *obj)
{
	struct gnix_fid_eq *eq = (struct gnix_fid_eq *) obj;
	pthread_mutex_lock(&gnix_eq_list_lock);
	dlist_remove(&eq->gnix_fid_eq_list);
	pthread_mutex_unlock(&gnix_eq_list_lock);

	_gnix_ref_put(eq->fabric);

	fastlock_destroy(&eq->lock);

	switch (eq->attr.wait_obj) {
	case FI_WAIT_NONE:
		break;
	case FI_WAIT_SET:
		_gnix_wait_set_remove(eq->wait, &eq->eq_fid.fid);
		break;
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
	case FI_WAIT_MUTEX_COND:
		assert(eq->wait);
		gnix_wait_close(&eq->wait->fid);
		break;
	default:
		GNIX_WARN(FI_LOG_EQ, "format: %d unsupported\n.",
			  eq->attr.wait_obj);
		break;
	}

	_gnix_queue_destroy(eq->events);
	_gnix_queue_destroy(eq->errors);

	gnix_eq_cleanup_err_bufs(eq, 1);

	free(eq);
}

int _gnix_eq_poll_obj_add(struct gnix_fid_eq *eq, struct fid *obj_fid)
{
	struct gnix_eq_poll_obj *pobj;

	COND_WRITE_ACQUIRE(eq->requires_lock, &eq->poll_obj_lock);

	pobj = malloc(sizeof(struct gnix_eq_poll_obj));
	if (!pobj) {
		GNIX_WARN(FI_LOG_EQ, "Failed to add object to EQ poll list.\n");
		COND_RW_RELEASE(eq->requires_lock, &eq->poll_obj_lock);
		return -FI_ENOMEM;
	}

	pobj->obj_fid = obj_fid;
	dlist_init(&pobj->list);
	dlist_insert_tail(&pobj->list, &eq->poll_objs);

	COND_RW_RELEASE(eq->requires_lock, &eq->poll_obj_lock);

	GNIX_INFO(FI_LOG_EQ, "Added object(%d, %p) to EQ(%p) poll list\n",
		  obj_fid->fclass, obj_fid, eq);

	return FI_SUCCESS;
}

int _gnix_eq_poll_obj_rem(struct gnix_fid_eq *eq, struct fid *obj_fid)
{
	struct gnix_eq_poll_obj *pobj, *tmp;

	COND_WRITE_ACQUIRE(eq->requires_lock, &eq->poll_obj_lock);

	dlist_for_each_safe(&eq->poll_objs, pobj, tmp, list) {
		if (pobj->obj_fid == obj_fid) {
			dlist_remove(&pobj->list);
			free(pobj);
			GNIX_INFO(FI_LOG_EQ,
				  "Removed object(%d, %p) from EQ(%p) poll list\n",
				  pobj->obj_fid->fclass, pobj, eq);
			COND_RW_RELEASE(eq->requires_lock, &eq->poll_obj_lock);
			return FI_SUCCESS;
		}
	}

	COND_RW_RELEASE(eq->requires_lock, &eq->poll_obj_lock);

	GNIX_WARN(FI_LOG_EQ, "object not found on EQ poll list.\n");
	return -FI_EINVAL;
}

int _gnix_eq_progress(struct gnix_fid_eq *eq)
{
	struct gnix_eq_poll_obj *pobj, *tmp;
	int rc;
	struct gnix_fid_pep *pep;
	struct gnix_fid_ep *ep;

	COND_READ_ACQUIRE(eq->requires_lock, &eq->poll_obj_lock);

	dlist_for_each_safe(&eq->poll_objs, pobj, tmp, list) {
		switch (pobj->obj_fid->fclass) {
		case FI_CLASS_PEP:
			pep = container_of(pobj->obj_fid, struct gnix_fid_pep,
					   pep_fid.fid);
			rc = _gnix_pep_progress(pep);
			if (rc) {
				GNIX_WARN(FI_LOG_EQ,
					  "_gnix_pep_progress failed: %d\n",
					  rc);
			}
			break;
		case FI_CLASS_EP:
			ep = container_of(pobj->obj_fid, struct gnix_fid_ep,
					  ep_fid.fid);
			rc = _gnix_ep_progress(ep);
			if (rc) {
				GNIX_WARN(FI_LOG_EP_CTRL,
					  "_gnix_ep_progress failed: %d\n",
					  rc);
			}
			break;
		default:
			GNIX_WARN(FI_LOG_EQ,
				  "invalid poll object: %d %p\n",
				  pobj->obj_fid->fclass, pobj);
			break;
		}
	}

	COND_RW_RELEASE(eq->requires_lock, &eq->poll_obj_lock);

	return FI_SUCCESS;
}

/*******************************************************************************
 * API function implementations.
 ******************************************************************************/
/*
 * - Handle FI_WRITE flag. When not included, replace write function with
 *   fi_no_eq_write.
 */
DIRECT_FN int gnix_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
			   struct fid_eq **eq, void *context)
{
	struct gnix_fid_eq *eq_priv;

	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_EQ, "\n");

	if (!fabric)
		return -FI_EINVAL;

	eq_priv = calloc(1, sizeof(*eq_priv));
	if (!eq_priv)
		return -FI_ENOMEM;

	ret = gnix_verify_eq_attr(attr);
	if (ret)
		goto err;

	eq_priv->fabric = container_of(fabric, struct gnix_fid_fabric,
					  fab_fid);

	_gnix_ref_init(&eq_priv->ref_cnt, 1, __eq_destruct);

	_gnix_ref_get(eq_priv->fabric);

	eq_priv->eq_fid.fid.fclass = FI_CLASS_EQ;
	eq_priv->eq_fid.fid.context = context;
	eq_priv->eq_fid.fid.ops = &gnix_fi_eq_ops;
	eq_priv->eq_fid.ops = &gnix_eq_ops;
	eq_priv->requires_lock = 1;
	eq_priv->attr = *attr;

	fastlock_init(&eq_priv->lock);

	rwlock_init(&eq_priv->poll_obj_lock);
	dlist_init(&eq_priv->poll_objs);

	dlist_init(&eq_priv->err_bufs);

	ret = gnix_eq_set_wait(eq_priv);
	if (ret)
		goto err1;

	ret = _gnix_queue_create(&eq_priv->events, alloc_eq_entry,
				 free_eq_entry, 0, eq_priv->attr.size);
	if (ret)
		goto err1;

	ret = _gnix_queue_create(&eq_priv->errors, alloc_eq_entry,
				 free_eq_entry, sizeof(struct fi_eq_err_entry),
				 0);
	if (ret)
		goto err2;

	*eq = &eq_priv->eq_fid;

	pthread_mutex_lock(&gnix_eq_list_lock);
	dlist_insert_tail(&eq_priv->gnix_fid_eq_list, &gnix_eq_list);
	pthread_mutex_unlock(&gnix_eq_list_lock);

	return ret;

err2:
	_gnix_queue_destroy(eq_priv->events);
err1:
	_gnix_ref_put(eq_priv->fabric);
	fastlock_destroy(&eq_priv->lock);
err:
	free(eq_priv);
	return ret;
}

DIRECT_FN STATIC int gnix_eq_close(struct fid *fid)
{
	struct gnix_fid_eq *eq;
	int references_held;

	GNIX_TRACE(FI_LOG_EQ, "\n");

	if (!fid)
		return -FI_EINVAL;

	eq = container_of(fid, struct gnix_fid_eq, eq_fid);

	references_held = _gnix_ref_put(eq);
	if (references_held) {
		GNIX_INFO(FI_LOG_EQ, "failed to fully close eq due "
				"to lingering references. references=%i eq=%p\n",
				references_held, eq);
	}

	return FI_SUCCESS;
}

static ssize_t __gnix_eq_sread(int blocking, struct fid_eq *eq,
			       uint32_t *event, void *buf, size_t len,
			       uint64_t flags, int timeout)
{
	struct gnix_fid_eq *eq_priv;
	struct gnix_eq_entry *entry;
	struct slist_entry *item;
	ssize_t read_size;

	if (!eq || !event || (len && !buf))
		return -FI_EINVAL;

	eq_priv = container_of(eq, struct gnix_fid_eq, eq_fid);

	if ((blocking && !eq_priv->wait) ||
	    (blocking && eq_priv->attr.wait_obj == FI_WAIT_SET)) {
		GNIX_WARN(FI_LOG_EQ, "Invalid wait type\n");
		return -FI_EINVAL;
	}

	gnix_eq_cleanup_err_bufs(eq_priv, 0);

	_gnix_eq_progress(eq_priv);

	if (_gnix_queue_peek(eq_priv->errors))
		return -FI_EAVAIL;

	if (eq_priv->wait)
		gnix_wait_wait((struct fid_wait *) eq_priv->wait, timeout);

	fastlock_acquire(&eq_priv->lock);

	if (_gnix_queue_peek(eq_priv->errors)) {
		read_size = -FI_EAVAIL;
		goto err;
	}

	item = _gnix_queue_peek(eq_priv->events);

	if (!item) {
		read_size = -FI_EAGAIN;
		goto err;
	}

	entry = container_of(item, struct gnix_eq_entry, item);

	if (len < entry->len) {
		read_size = -FI_ETOOSMALL;
		goto err;
	}

	*event = entry->type;

	read_size = entry->len;
	memcpy(buf, entry->the_entry, read_size);

	if (!(flags & FI_PEEK)) {
		item = _gnix_queue_dequeue(eq_priv->events);

		free(entry->the_entry);
		entry->the_entry = NULL;

		_gnix_queue_enqueue_free(eq_priv->events, &entry->item);
	}

err:
	fastlock_release(&eq_priv->lock);

	return read_size;
}

DIRECT_FN STATIC ssize_t gnix_eq_read(struct fid_eq *eq, uint32_t *event,
				      void *buf, size_t len, uint64_t flags)
{
	return __gnix_eq_sread(0, eq, event, buf, len, flags, 0);
}

DIRECT_FN STATIC ssize_t gnix_eq_sread(struct fid_eq *eq, uint32_t *event,
				       void *buf, size_t len, int timeout,
				       uint64_t flags)
{
	return __gnix_eq_sread(1, eq, event, buf, len, flags, timeout);
}

DIRECT_FN STATIC int gnix_eq_control(struct fid *eq, int command, void *arg)
{
	/* disabled until new trywait interface is implemented
	struct gnix_fid_eq *eq_priv;

	eq_priv = container_of(eq, struct gnix_fid_eq, eq_fid);
	*/
	switch (command) {
	case FI_GETWAIT:
		/* return _gnix_get_wait_obj(eq_priv->wait, arg); */
		return -FI_ENOSYS;
	default:
		return -FI_EINVAL;
	}
}

DIRECT_FN STATIC ssize_t gnix_eq_readerr(struct fid_eq *eq,
					 struct fi_eq_err_entry *buf,
					 uint64_t flags)
{
	struct gnix_fid_eq *eq_priv;
	struct gnix_eq_entry *entry;
	struct slist_entry *item;
	struct gnix_eq_err_buf *err_buf;
	struct fi_eq_err_entry *fi_err;

	ssize_t read_size = sizeof(*buf);

	eq_priv = container_of(eq, struct gnix_fid_eq, eq_fid);

	fastlock_acquire(&eq_priv->lock);

	if (flags & FI_PEEK)
		item = _gnix_queue_peek(eq_priv->errors);
	else
		item = _gnix_queue_dequeue(eq_priv->errors);

	if (!item) {
		read_size = -FI_EAGAIN;
		goto err;
	}

	entry = container_of(item, struct gnix_eq_entry, item);
	fi_err = (struct fi_eq_err_entry *)entry->the_entry;

	memcpy(buf, entry->the_entry, read_size);

	/* If removing an event with err_data, mark err buf to be freed during
	 * the next EQ read. */
	if (!(flags & FI_PEEK) && fi_err->err_data) {
		err_buf = container_of(fi_err->err_data,
				       struct gnix_eq_err_buf, buf);
		err_buf->do_free = 1;
	}

	_gnix_queue_enqueue_free(eq_priv->errors, &entry->item);

err:
	fastlock_release(&eq_priv->lock);

	return read_size;
}

DIRECT_FN STATIC ssize_t gnix_eq_write(struct fid_eq *eq, uint32_t event,
				       const void *buf, size_t len,
				       uint64_t flags)
{
	struct gnix_fid_eq *eq_priv;
	struct slist_entry *item;
	struct gnix_eq_entry *entry;

	ssize_t ret = len;

	eq_priv = container_of(eq, struct gnix_fid_eq, eq_fid);

	fastlock_acquire(&eq_priv->lock);

	item = _gnix_queue_get_free(eq_priv->events);
	if (!item) {
		GNIX_WARN(FI_LOG_EQ, "error creating eq_entry\n");
		ret = -FI_ENOMEM;
		goto err;
	}

	entry = container_of(item, struct gnix_eq_entry, item);

	entry->the_entry = calloc(1, len);
	if (!entry->the_entry) {
		_gnix_queue_enqueue_free(eq_priv->events, &entry->item);
		GNIX_WARN(FI_LOG_EQ, "error allocating buffer\n");
		ret = -FI_ENOMEM;
		goto err;
	}

	memcpy(entry->the_entry, buf, len);

	entry->len = len;
	entry->type = event;
	entry->flags = flags;

	_gnix_queue_enqueue(eq_priv->events, &entry->item);

	if (eq_priv->wait)
		_gnix_signal_wait_obj(eq_priv->wait);

err:
	fastlock_release(&eq_priv->lock);

	return ret;
}

/**
 * Converts provider specific error information into a printable string.
 *
 * @param[in] eq		the event queue
 * @param[in] prov_errno	the provider specific error number
 * @param[in/out] buf		optional buffer to print error information
 * @param[in] len		the length of buf
 *
 * @return the printable string
 * @return NULL upon error or if the operation is not supported yet
 */
DIRECT_FN STATIC const char *gnix_eq_strerror(struct fid_eq *eq, int prov_errno,
					      const void *err_data, char *buf,
					      size_t len)
{
	return NULL;
}

/*******************************************************************************
 * FI_OPS_* data structures.
 ******************************************************************************/
static struct fi_ops_eq gnix_eq_ops = {
	.size = sizeof(struct fi_ops_eq),
	.read = gnix_eq_read,
	.readerr = gnix_eq_readerr,
	.write = gnix_eq_write,
	.sread = gnix_eq_sread,
	.strerror = gnix_eq_strerror
};

static struct fi_ops gnix_fi_eq_ops = {
	.size = sizeof(struct fi_ops),
	.close = gnix_eq_close,
	.bind = fi_no_bind,
	.control = gnix_eq_control,
	.ops_open = fi_no_ops_open
};

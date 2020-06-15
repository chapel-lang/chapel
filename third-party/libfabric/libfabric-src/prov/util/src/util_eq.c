/*
 * Copyright (c) 2013-2016 Intel Corporation. All rights reserved.
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

#include <ofi_enosys.h>
#include <ofi_util.h>

void ofi_eq_handle_err_entry(uint32_t api_version, uint64_t flags,
			     struct fi_eq_err_entry *err_entry,
			     struct fi_eq_err_entry *user_err_entry)
{
	if ((FI_VERSION_GE(api_version, FI_VERSION(1, 5)))
	    && user_err_entry->err_data && user_err_entry->err_data_size) {
		void *err_data = user_err_entry->err_data;
		size_t err_data_size = MIN(err_entry->err_data_size,
					   user_err_entry->err_data_size);

		memcpy(err_data, err_entry->err_data, err_data_size);

		*user_err_entry = *err_entry;
		user_err_entry->err_data = err_data;
		user_err_entry->err_data_size = err_data_size;

		if (!(flags & FI_PEEK)) {
			free(err_entry->err_data);
			err_entry->err_data = NULL;
			err_entry->err_data_size = 0;
		}
	} else {
		*user_err_entry = *err_entry;
	}

	if (!(flags & FI_PEEK)) {
		err_entry->err = 0;
		err_entry->prov_errno = 0;
	}
}

ssize_t ofi_eq_read(struct fid_eq *eq_fid, uint32_t *event,
		    void *buf, size_t len, uint64_t flags)
{
	struct util_eq *eq;
	struct util_event *entry;
	struct fi_eq_err_entry *err_entry;
	ssize_t ret;

	eq = container_of(eq_fid, struct util_eq, eq_fid);

	fastlock_acquire(&eq->lock);
	if (slist_empty(&eq->list)) {
		ret = -FI_EAGAIN;
		goto out;
	}

	entry = container_of(eq->list.head, struct util_event, entry);
	if (entry->err && !(flags & UTIL_FLAG_ERROR)) {
		ret = -FI_EAVAIL;
		goto out;
	} else if (!entry->err && (flags & UTIL_FLAG_ERROR)) {
		ret = -FI_EAGAIN;
		goto out;
	}

	if (event)
		*event = entry->event;
	if (buf) {
		if (flags & UTIL_FLAG_ERROR) {
			free(eq->saved_err_data);
			eq->saved_err_data = NULL;

			assert((size_t) entry->size == sizeof(*err_entry));
			err_entry = (struct fi_eq_err_entry *) entry->data;

			ofi_eq_handle_err_entry(eq->fabric->fabric_fid.api_version,
						flags, err_entry, buf);
			ret = (ssize_t) entry->size;

			if (!(flags & FI_PEEK))
				eq->saved_err_data = err_entry->err_data;
		} else {
			ret = MIN(len, (size_t)entry->size);
			memcpy(buf, entry->data, ret);
		}
	}  else {
		ret = 0;
	}

	if (!(flags & FI_PEEK)) {
		slist_remove_head(&eq->list);
		free(entry);
	}
out:
	fastlock_release(&eq->lock);
	return ret;
}

ssize_t ofi_eq_readerr(struct fid_eq *eq_fid, struct fi_eq_err_entry *buf,
		       uint64_t flags)
{
	return fi_eq_read(eq_fid, NULL, buf, sizeof(*buf),
			  flags | UTIL_FLAG_ERROR);
}

ssize_t ofi_eq_write(struct fid_eq *eq_fid, uint32_t event,
		     const void *buf, size_t len, uint64_t flags)
{
	struct util_eq *eq;
	struct util_event *entry;

	eq = container_of(eq_fid, struct util_eq, eq_fid);
	entry = calloc(1, sizeof(*entry) + len);
	if (!entry)
		return -FI_ENOMEM;

	entry->size = (int) len;
	entry->event = event;
	entry->err = !!(flags & UTIL_FLAG_ERROR);
	memcpy(entry->data, buf, len);

	fastlock_acquire(&eq->lock);
	slist_insert_tail(&entry->entry, &eq->list);
	fastlock_release(&eq->lock);

	if (eq->wait)
		eq->wait->signal(eq->wait);

	return len;
}

ssize_t ofi_eq_sread(struct fid_eq *eq_fid, uint32_t *event, void *buf,
		     size_t len, int timeout, uint64_t flags)
{
	struct util_eq *eq;
	uint64_t endtime;
	ssize_t ret;

	eq = container_of(eq_fid, struct util_eq, eq_fid);
	if (!eq->internal_wait) {
		FI_WARN(eq->prov, FI_LOG_EQ, "EQ not configured for sread\n");
		return -FI_ENOSYS;
	}

	endtime = ofi_timeout_time(timeout);
	do {
		ret = fi_eq_read(eq_fid, event, buf, len, flags);
		if (ret != -FI_EAGAIN)
			break;

		if (ofi_adjust_timeout(endtime, &timeout))
			return -FI_EAGAIN;

		ret = fi_wait(&eq->wait->wait_fid, timeout);
	} while (!ret);

	return ret == -FI_ETIMEDOUT ? -FI_EAGAIN : ret;
}

const char *ofi_eq_strerror(struct fid_eq *eq_fid, int prov_errno,
			    const void *err_data, char *buf, size_t len)
{
	return (buf && len) ? strncpy(buf, fi_strerror(prov_errno), len) :
			      fi_strerror(prov_errno);
}

int ofi_eq_control(struct fid *fid, int command, void *arg)
{
	struct util_eq *eq;
	int ret;

	eq = container_of(fid, struct util_eq, eq_fid.fid);

	switch (command) {
	case FI_GETWAIT:
	case FI_GETWAITOBJ:
		ret = fi_control(&eq->wait->wait_fid.fid, command, arg);
		break;
	default:
		ret = -FI_ENOSYS;
		break;
	}

	return ret;
}

int ofi_eq_cleanup(struct fid *fid)
{
	struct util_eq *eq;
	struct slist_entry *entry;
	struct util_event *event;

	eq = container_of(fid, struct util_eq, eq_fid.fid);
	if (ofi_atomic_get32(&eq->ref))
		return -FI_EBUSY;

	while (!slist_empty(&eq->list)) {
		entry = slist_remove_head(&eq->list);
		event = container_of(entry, struct util_event, entry);
		free(event);
	}

	if (eq->wait) {
		fi_poll_del(&eq->wait->pollset->poll_fid,
			    &eq->eq_fid.fid, 0);
		if (eq->internal_wait)
			fi_close(&eq->wait->wait_fid.fid);
	}

	free(eq->saved_err_data);
	fastlock_destroy(&eq->lock);
	ofi_atomic_dec32(&eq->fabric->ref);
	return 0;
}

static int util_eq_close(struct fid *fid)
{
	struct util_eq *eq;
	int ret;

	ret = ofi_eq_cleanup(fid);
	if (ret)
		return ret;

	eq = container_of(fid, struct util_eq,
			  eq_fid.fid);
	free(eq);
	return 0;
}

static struct fi_ops_eq util_eq_ops = {
	.size = sizeof(struct fi_ops_eq),
	.read = ofi_eq_read,
	.readerr = ofi_eq_readerr,
	.sread = ofi_eq_sread,
	.write = ofi_eq_write,
	.strerror = ofi_eq_strerror,
};

static struct fi_ops util_eq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = util_eq_close,
	.bind = fi_no_bind,
	.control = ofi_eq_control,
	.ops_open = fi_no_ops_open,
};

static int util_eq_init(struct fid_fabric *fabric, struct util_eq *eq,
			const struct fi_eq_attr *attr)
{
	struct fi_wait_attr wait_attr;
	struct fid_wait *wait;
	int ret;

	ofi_atomic_initialize32(&eq->ref, 0);
	slist_init(&eq->list);
	fastlock_init(&eq->lock);

	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
		break;
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
	case FI_WAIT_POLLFD:
	case FI_WAIT_MUTEX_COND:
	case FI_WAIT_YIELD:
		memset(&wait_attr, 0, sizeof wait_attr);
		wait_attr.wait_obj = attr->wait_obj;
		eq->internal_wait = 1;
		ret = fi_wait_open(fabric, &wait_attr, &wait);
		if (ret)
			return ret;
		eq->wait = container_of(wait, struct util_wait, wait_fid);
		break;
	case FI_WAIT_SET:
		eq->wait = container_of(attr->wait_set, struct util_wait,
					wait_fid);
		break;
	default:
		assert(0);
		return -FI_EINVAL;
	}

	return 0;
}

static int ofi_eq_match_fid_event(struct slist_entry *entry, const void *arg)
{
	fid_t fid = (fid_t) arg;
	struct util_event *event;
	struct fi_eq_cm_entry *cm_entry;
	struct fi_eq_entry *cq_entry;

	event = container_of(entry, struct util_event, entry);
	cm_entry = (struct fi_eq_cm_entry *) event->data;

	if (event->event == FI_CONNREQ &&
	    fid == cm_entry->info->handle)
		return 1;

	cq_entry = (struct fi_eq_entry *)event->data;
	return (fid == cq_entry->fid);
}

void ofi_eq_remove_fid_events(struct util_eq *eq, fid_t fid)
{
	struct fi_eq_err_entry *err_entry;
	struct slist_entry *entry;
	struct util_event *event;
	struct fi_eq_cm_entry *cm_entry;

	fastlock_acquire(&eq->lock);
	while((entry =
	      slist_remove_first_match(&eq->list, ofi_eq_match_fid_event,
				       fid))) {
		event = container_of(entry, struct util_event, entry);
		if (event->err) {
			err_entry = (struct fi_eq_err_entry *) event->data;
			if (err_entry->err_data)
				free(err_entry->err_data);

		} else if (event->event == FI_CONNREQ) {
			cm_entry = (struct fi_eq_cm_entry *) event->data;
			assert(cm_entry->info);
			fi_freeinfo(cm_entry->info);
		}
		free(event);
	}
	fastlock_release(&eq->lock);
}

static int util_verify_eq_attr(const struct fi_provider *prov,
			       const struct fi_eq_attr *attr)
{
	switch (attr->wait_obj) {
	case FI_WAIT_NONE:
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
	case FI_WAIT_POLLFD:
	case FI_WAIT_MUTEX_COND:
	case FI_WAIT_YIELD:
		break;
	case FI_WAIT_SET:
		if (!attr->wait_set) {
			FI_WARN(prov, FI_LOG_EQ, "invalid wait set\n");
			return -FI_EINVAL;
		}
		break;
	default:
		FI_WARN(prov, FI_LOG_EQ, "invalid wait object type\n");
		return -FI_EINVAL;
	}

	if (attr->flags & ~(FI_WRITE)) {
		FI_WARN(prov, FI_LOG_EQ, "invalid flags\n");
		return -FI_EINVAL;
	}

	if (attr->flags & ~(FI_AFFINITY | FI_WRITE)) {
		FI_WARN(prov, FI_LOG_EQ, "invalid flags\n");
		return -FI_EINVAL;
	}

	if (attr->flags & FI_AFFINITY) {
		FI_WARN(prov, FI_LOG_EQ, "signaling vector ignored\n");
	}

	return 0;
}

int ofi_eq_init(struct fid_fabric *fabric_fid, struct fi_eq_attr *attr,
		struct fid_eq *eq_fid, void *context)
{
	struct util_fabric *fabric;
	struct util_eq *eq;
	int ret;

	fabric = container_of(fabric_fid, struct util_fabric, fabric_fid);
	ret = util_verify_eq_attr(fabric->prov, attr);
	if (ret)
		return ret;

	eq = container_of(eq_fid, struct util_eq, eq_fid);
	eq->fabric = fabric;
	eq->prov = fabric->prov;
	ret = util_eq_init(fabric_fid, eq, attr);
	if (ret) {
		return ret;
	}

	eq->eq_fid.fid.fclass = FI_CLASS_EQ;
	eq->eq_fid.fid.context = context;
	eq->eq_fid.fid.ops = &util_eq_fi_ops;
	eq->eq_fid.ops = &util_eq_ops;

	ofi_atomic_inc32(&fabric->ref);

	/* EQ must be fully operational before adding to wait set */
	if (eq->wait) {
		ret = fi_poll_add(&eq->wait->pollset->poll_fid,
				  &eq->eq_fid.fid, 0);
		if (ret) {
			ofi_eq_cleanup(&eq->eq_fid.fid);
			return ret;
		}
	}

	return 0;
}

int ofi_eq_create(struct fid_fabric *fabric_fid, struct fi_eq_attr *attr,
		  struct fid_eq **eq_fid, void *context)
{
	struct util_eq *eq;
	int ret;

	eq = calloc(1, sizeof(*eq));
	if (!eq)
		return -FI_ENOMEM;

	ret = ofi_eq_init(fabric_fid, attr, &eq->eq_fid, context);
	if (ret) {
		free(eq);
		return ret;
	}
	*eq_fid = &eq->eq_fid;
	return 0;
}

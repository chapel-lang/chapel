/*
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
 * Copyright (c) 2015-2017 Cray Inc.  All rights reserved.
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

#include "gnix.h"
#include "gnix_wait.h"
#include <ofi_list.h>
#include <string.h>

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fi_info *hints;
static struct fi_info *fi;
static struct gnix_fid_wait *wait_priv;
static struct fi_wait_attr wait_attr;
static struct fid_wait *wait_set;

void wait_setup(void)
{
	int ret = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->mode = mode_bits;
	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");
}

void wait_teardown(void)
{
	int ret = 0;

	ret = fi_close(&wait_set->fid);
	cr_assert(!ret, "failure in closing wait set.");

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

void setup_wait_type(enum fi_wait_obj wait_obj)
{
	int ret;

	wait_setup();
	wait_attr.wait_obj = wait_obj;

	ret = fi_wait_open(fab, &wait_attr, &wait_set);
	cr_assert(!ret, "fi_wait_open");

	wait_priv = container_of(wait_set, struct gnix_fid_wait, wait);
}

void unspec_setup(void)
{
	setup_wait_type(FI_WAIT_UNSPEC);
}

void fd_setup(void)
{
	setup_wait_type(FI_WAIT_FD);
}

void mutex_cond_setup(void)
{
	setup_wait_type(FI_WAIT_MUTEX_COND);
}

Test(wait_creation, unspec, .init = unspec_setup, .fini = wait_teardown)
{
	cr_expect_eq(wait_priv->type, FI_WAIT_UNSPEC);
	cr_expect_eq(wait_priv->type, wait_attr.wait_obj);
	cr_expect_eq(&wait_priv->fabric->fab_fid, fab);
	cr_expect_eq(wait_priv->cond_type, FI_CQ_COND_NONE);
}

Test(wait_creation, fd, .init = fd_setup, .fini = wait_teardown,
	.disabled = true)
{
	cr_expect_eq(wait_priv->type, FI_WAIT_FD);
	cr_expect_eq(wait_priv->type, wait_attr.wait_obj);
	cr_expect_eq(&wait_priv->fabric->fab_fid, fab);
	cr_expect_eq(wait_priv->cond_type, FI_CQ_COND_NONE);
}

Test(wait_creation, mutex_cond, .init = mutex_cond_setup, .fini = wait_teardown,
	.disabled = true)
{
	cr_expect_eq(wait_priv->type, FI_WAIT_MUTEX_COND);
	cr_expect_eq(wait_priv->type, wait_attr.wait_obj);
	cr_expect_eq(&wait_priv->fabric->fab_fid, fab);
	cr_expect_eq(wait_priv->cond_type, FI_CQ_COND_NONE);
}

Test(wait_control, unspec, .init = unspec_setup, .fini = wait_teardown,
     .disabled = true)
{
	int fd;
	int ret;

	ret = fi_control(&wait_priv->wait.fid, FI_GETWAIT, &fd);
	cr_expect_eq(FI_SUCCESS, ret, "fi_control failed.");

	cr_expect_eq(wait_priv->fd[WAIT_READ], fd);
}

Test(wait_control, fd, .init = fd_setup, .fini = wait_teardown,
     .disabled = true)
{
	int fd;
	int ret;

	ret = fi_control(&wait_priv->wait.fid, FI_GETWAIT, &fd);
	cr_expect_eq(FI_SUCCESS, ret, "fi_control failed.");

	cr_expect_eq(wait_priv->fd[WAIT_READ], fd);
}

Test(wait_control, mutex_cond, .init = mutex_cond_setup, .fini = wait_teardown,
     .disabled = true)
{
	int ret;
	struct fi_mutex_cond mutex_cond;

	ret = fi_control(&wait_priv->wait.fid, FI_GETWAIT, &mutex_cond);
	cr_expect_eq(FI_SUCCESS, ret, "fi_control failed.");

	ret = memcmp(&wait_priv->mutex, mutex_cond.mutex,
		     sizeof(*mutex_cond.mutex));
	cr_expect_eq(0, ret, "mutex compare failed.");

	ret = memcmp(&wait_priv->cond, mutex_cond.cond,
		     sizeof(*mutex_cond.cond));
	cr_expect_eq(0, ret, "cond compare failed.");
}

Test(wait_set, signal_multi, .init = unspec_setup)
{
	int ret;
	struct gnix_wait_entry *entry;

	struct fid temp_wait = {
		.fclass = FI_CLASS_CNTR
	};

	struct fid temp_wait2 = {
		.fclass = FI_CLASS_CQ
	};

	cr_expect(slist_empty(&wait_priv->set),
		  "wait set is not initially empty.");
	ret = _gnix_wait_set_add(&wait_priv->wait, &temp_wait);

	cr_expect_eq(FI_SUCCESS, ret, "gnix_wait_set_add failed.");

	ret = _gnix_wait_set_add(&wait_priv->wait, &temp_wait2);
	cr_expect_eq(FI_SUCCESS, ret, "gnix_wait_set_add failed.");

	cr_expect(!slist_empty(&wait_priv->set),
		  "wait set is empty after add.");

	entry = container_of(wait_priv->set.head, struct gnix_wait_entry,
			     entry);

	ret = memcmp(entry->wait_obj, &temp_wait, sizeof(temp_wait));
	cr_expect_eq(0, ret, "wait objects are not equal.");

	ret = fi_close(&wait_set->fid);
	cr_expect_eq(-FI_EBUSY, ret);

	ret = _gnix_wait_set_remove(&wait_priv->wait, &temp_wait);

	cr_expect_eq(FI_SUCCESS, ret, "gnix_wait_set_remove failed.");

	ret = _gnix_wait_set_remove(&wait_priv->wait, &temp_wait2);

	cr_expect_eq(FI_SUCCESS, ret, "gnix_wait_set_remove failed.");

	ret = fi_close(&wait_set->fid);
	cr_expect_eq(FI_SUCCESS, ret, "fi_close on wait set failed.");

	ret = fi_close(&fab->fid);
	cr_expect_eq(FI_SUCCESS, ret, "failure in closing fabric.");

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

Test(wait_set, add, .init = unspec_setup)
{
	int ret;
	struct gnix_wait_entry *entry;

	struct fid temp_wait = {
		.fclass = FI_CLASS_CQ
	};

	cr_expect(slist_empty(&wait_priv->set),
		  "wait set is not initially empty.");
	ret = _gnix_wait_set_add(&wait_priv->wait, &temp_wait);

	cr_expect_eq(FI_SUCCESS, ret, "gnix_wait_set_add failed.");

	cr_expect(!slist_empty(&wait_priv->set),
		  "wait set is empty after add.");

	entry = container_of(wait_priv->set.head, struct gnix_wait_entry,
			     entry);

	ret = memcmp(entry->wait_obj, &temp_wait, sizeof(temp_wait));
	cr_expect_eq(0, ret, "wait objects are not equal.");

	ret = fi_close(&wait_set->fid);
	cr_expect_eq(-FI_EBUSY, ret);

	ret = _gnix_wait_set_remove(&wait_priv->wait, &temp_wait);

	cr_expect_eq(FI_SUCCESS, ret, "gnix_wait_set_remove failed.");

	ret = fi_close(&wait_set->fid);
	cr_expect_eq(FI_SUCCESS, ret, "fi_close on wait set failed.");

	ret = fi_close(&fab->fid);
	cr_expect_eq(FI_SUCCESS, ret, "failure in closing fabric.");

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

Test(wait_set, empty_remove, .init = unspec_setup)
{
	int ret;

	struct fid temp_wait = {
		.fclass = FI_CLASS_CQ
	};

	cr_expect(slist_empty(&wait_priv->set));
	ret = _gnix_wait_set_remove(&wait_priv->wait, &temp_wait);
	cr_expect_eq(-FI_EINVAL, ret);
	cr_expect(slist_empty(&wait_priv->set));

	ret = fi_close(&wait_set->fid);
	cr_expect_eq(FI_SUCCESS, ret, "fi_close on wait set failed.");

	ret = fi_close(&fab->fid);
	cr_expect_eq(FI_SUCCESS, ret, "fi_close on fabric failed.");

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

Test(wait_verify, invalid_type, .init = wait_setup)
{
	int ret;

	wait_attr.wait_obj = FI_WAIT_SET;

	ret = fi_wait_open(fab, &wait_attr, &wait_set);
	cr_expect_eq(-FI_EINVAL, ret,
		     "Requesting incorrect type FI_WAIT_SET succeeded.");

	ret = fi_wait_open(fab, NULL, &wait_set);
	cr_expect_eq(-FI_EINVAL, ret,
		     "Requesting verification with NULL attr succeeded.");

	wait_attr.flags = 1;
	ret = fi_wait_open(fab, &wait_attr, &wait_set);
	cr_expect_eq(-FI_EINVAL, ret,
		     "Requesting verifications with flags set succeeded.");
}

Test(wait_signal, has_data, .init = unspec_setup)
{
	int ret;

	struct fid temp_wait = {
		.fclass = FI_CLASS_CQ
	};

	cr_expect(slist_empty(&wait_priv->set),
		"error");
	ret = _gnix_wait_set_add(&wait_priv->wait, &temp_wait);

	cr_expect_eq(FI_SUCCESS, ret, "gnix_wait_set_add failed.");

	cr_expect(!slist_empty(&wait_priv->set),
		"wait set is empty after add.");

	_gnix_signal_wait_obj(&wait_priv->wait);

	ret = fi_wait(&wait_priv->wait, 60);
	cr_expect_eq(FI_SUCCESS, ret, "fi_wait test failed. %d", ret);

	ret = _gnix_wait_set_remove(&wait_priv->wait, &temp_wait);

	ret = fi_close(&wait_set->fid);
	cr_expect_eq(FI_SUCCESS, ret, "fi_close on wait set failed.");

	ret = fi_close(&fab->fid);
	cr_expect_eq(FI_SUCCESS, ret, "fi_close on fabric failed.");

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

Test(wait_spin_adjust, set_val, .init = unspec_setup)
{
	int ret;
	int op = GNI_WAIT_THREAD_SLEEP;
	struct fi_gni_ops_fab *gni_fabric_ops;
	int32_t get_val, val;

	ret = fi_open_ops(&fab->fid, FI_GNI_FAB_OPS_1,
			  0, (void **) &gni_fabric_ops, NULL);

	cr_assert(ret == FI_SUCCESS, "fi_open_ops");

	ret = gni_fabric_ops->get_val(&fab->fid, op, &get_val);
	cr_assert(ret == FI_SUCCESS, "get_val");

	cr_expect_eq(20, get_val, "Value returned does not match default");

	val = 300;
	ret = gni_fabric_ops->set_val(&fab->fid, op, &val);

	cr_assert(ret == FI_SUCCESS, "set val");

	ret = gni_fabric_ops->get_val(&fab->fid, op, &get_val);
	cr_assert(val == get_val, "get val");


}

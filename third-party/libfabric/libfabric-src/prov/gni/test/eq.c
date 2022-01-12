/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC. All rights reserved.
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

#include <stdlib.h>

#include "gnix_eq.h"
#include "gnix.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fi_info *hints;
static struct fi_info *fi;

static struct gnix_fid_eq *eq_priv;
static struct fi_eq_attr eq_attr;
static struct fid_eq *eq;

static struct gnix_fid_wait *wait_priv;
static struct fi_wait_attr wait_attr;
static struct fid_wait *wait_set;

void _setup(void)
{
	int ret = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo failed.");

	hints->mode = mode_bits;
	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi);
	cr_assert_eq(ret, FI_SUCCESS, "fi_getinfo failed.");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert_eq(ret, FI_SUCCESS, "fi_fabric failed.");
}

void _teardown(void)
{
	int ret = 0;

	ret = fi_close(&fab->fid);
	cr_assert_eq(ret, FI_SUCCESS, "failure in closing fabric.");

	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

void eq_teardown(void)
{
	cr_assert_eq(FI_SUCCESS, fi_close(&eq->fid), "failure in closing eq.");
	_teardown();
}

void eq_create(enum fi_wait_obj wait_obj, size_t size)
{
	int ret = 0;

	eq_attr.size = size;
	eq_attr.wait_obj = wait_obj;

	ret = fi_eq_open(fab, &eq_attr, &eq, NULL);
	cr_assert_eq(ret, FI_SUCCESS, "fi_eq_open failed.");

	eq_priv = container_of(eq, struct gnix_fid_eq, eq_fid);

	if (eq_priv->wait) {
		wait_priv = container_of(eq_priv->wait, struct gnix_fid_wait,
					 wait);
	}
}

void eq_wait_none_setup(void)
{
	_setup();
	eq_create(FI_WAIT_NONE, 8);
}

void eq_wait_fd_setup(void)
{
	_setup();
	eq_create(FI_WAIT_FD, 8);
}

void eq_wait_unspec_setup(void)
{
	_setup();
	eq_create(FI_WAIT_UNSPEC, 8);
}

void eq_wait_mutex_cond_setup(void)
{
	_setup();
	eq_create(FI_WAIT_MUTEX_COND, 8);
}

/******************************************************************************
 * Basic in/out tests
 *****************************************************************************/

TestSuite(eq_basic, .init = eq_wait_unspec_setup, .fini = eq_teardown);

void eq_rw(int blocking)
{
	int ret = 0, i;
	uint32_t in_event = 'a';
	uint32_t out_event = 'a';

	fid_t in_err_fid = (fid_t)0xf1d;
	int64_t in_err_ctx = 0xc0413;
	uint64_t in_err_idx = 10;
	int in_err_err = 255;
	int in_err_perrno = -13;

	char *in_buf = "12345";
	char out_buf[1024];
	const size_t eq_size = 1025;
	struct fi_eq_err_entry err_buf;
	char *err_data_buf = "abcd";
	int err_cnt = 0;

	for (i = 0; i < eq_size; i++) {
		ret = fi_eq_write(eq, in_event + i + eq_size, in_buf,
				  sizeof(in_buf), 0);
		cr_assert_eq(ret, sizeof(in_buf));

#if 0
		fid_t            fid;        /* fid associated with error */
		void            *context;    /* operation context */
		uint64_t         data;       /* completion-specific data */
		int              err;        /* positive error code */
		int              prov_errno; /* provider error code */
		void            *err_data;   /* additional error data */
		size_t           err_data_size; /* size of err_data */
#endif
		ret = _gnix_eq_write_error(eq_priv,
					   (fid_t)(in_err_fid + i),
					   (void *)(in_err_ctx + i),
					   in_err_idx + i,
					   in_err_err + i,
					   in_err_perrno + i,
					   err_data_buf,
					   sizeof(err_data_buf));
		cr_assert_eq(ret, FI_SUCCESS);
		err_cnt++;
	}

	for (i = 0; i < eq_size*2; i++) {
		if (blocking) {
			ret = fi_eq_sread(eq, &out_event, &out_buf, 1024, 1, 0);
		} else {
			ret = fi_eq_read(eq, &out_event, &out_buf, 1024, 0);
		}

		if (ret == -FI_EAVAIL) {
			struct fi_eq_err_entry tmp_err = {
				.fid = (fid_t)(in_err_fid + i),
				.context = (void *)(in_err_ctx + i),
				.data = in_err_idx + i,
				.err = in_err_err + i,
				.prov_errno = in_err_perrno + i,
			};

			ret = fi_eq_readerr(eq, &err_buf, 0);
			cr_assert(!memcmp(&tmp_err, &err_buf,
					  offsetof(struct fi_eq_err_entry,
						   err_data)),
				  "bad error fields");
			cr_assert(err_buf.err_data_size = strlen(err_data_buf),
				  "bad error data size");
			cr_assert(!strncmp(err_buf.err_data, err_data_buf,
					   err_buf.err_data_size),
				  "bad error data");
			err_cnt--;
		} else {
			cr_assert(err_cnt == 0);
			cr_assert_eq(ret, sizeof(in_buf));
			cr_assert_eq(out_event, in_event + i);
		}
	}

	if (blocking) {
		ret = fi_eq_sread(eq, &out_event, &out_buf, 1024, 1, 0);
	} else {
		ret = fi_eq_read(eq, &out_event, &out_buf, 1024, 0);
	}
	cr_assert_eq(ret, -FI_EAGAIN);
}

Test(eq_basic, rw)
{
	eq_rw(0);
}

Test(eq_basic, rw_blocking)
{
	eq_rw(1);
}

/******************************************************************************
 * Wait object tests
 *****************************************************************************/

TestSuite(eq_wait_obj, .fini = eq_teardown);

Test(eq_wait_obj, none, .init = eq_wait_none_setup)
{
	char out_buf[1024];
	uint32_t out_event = 'a';
	int ret;

	ret = fi_eq_sread(eq, &out_event, &out_buf, 1024, 1, 0);
	cr_expect_eq(ret, -FI_EINVAL);
	cr_expect(!wait_priv, "wait_priv should be null.");
}

Test(eq_wait_obj, unspec, .init = eq_wait_unspec_setup)
{
	cr_expect_eq(wait_priv->type, FI_WAIT_UNSPEC);
	cr_expect_eq(wait_priv->type, eq_priv->attr.wait_obj);
	cr_expect_eq(wait_priv->type, eq_attr.wait_obj);
	cr_expect_eq(&wait_priv->fabric->fab_fid, fab);
	cr_expect_eq(wait_priv->cond_type, FI_CQ_COND_NONE);
}

Test(eq_wait_obj, fd, .init = eq_wait_fd_setup,
	.disabled =  true)
{
	cr_expect_eq(wait_priv->type, FI_WAIT_FD);
	cr_expect_eq(wait_priv->type, eq_priv->attr.wait_obj);
	cr_expect_eq(wait_priv->type, eq_attr.wait_obj);
	cr_expect_eq(&wait_priv->fabric->fab_fid, fab);
	cr_expect_eq(wait_priv->cond_type, FI_CQ_COND_NONE);
}

Test(eq_wait_obj, mutex_cond, .init = eq_wait_mutex_cond_setup,
	.disabled = true)
{
	cr_expect_eq(wait_priv->type, FI_WAIT_MUTEX_COND);
	cr_expect_eq(wait_priv->type, eq_priv->attr.wait_obj);
	cr_expect_eq(wait_priv->type, eq_attr.wait_obj);
	cr_expect_eq(&wait_priv->fabric->fab_fid, fab);
	cr_expect_eq(wait_priv->cond_type, FI_CQ_COND_NONE);
}

TestSuite(eq_wait_control, .fini = eq_teardown, .disabled = true);

/*
Test(eq_wait_control, none, .init = eq_wait_none_setup)
{
	int ret;
	int fd;

	ret = fi_control(&eq_priv->eq_fid.fid, FI_GETWAIT, &fd);
	cr_expect_eq(-FI_ENOSYS, ret, "fi_control exists for none.");
}
*/

Test(eq_wait_control, unspec, .init = eq_wait_unspec_setup)
{
	int ret;
	int fd;

	ret = fi_control(&eq_priv->eq_fid.fid, FI_GETWAIT, &fd);
	cr_expect_eq(FI_SUCCESS, ret, "fi_control failed.");

	cr_expect_eq(wait_priv->fd[WAIT_READ], fd);
}

Test(eq_wait_control, fd, .init = eq_wait_fd_setup)
{
	int ret;
	int fd;

	ret = fi_control(&eq_priv->eq_fid.fid, FI_GETWAIT, &fd);
	cr_expect_eq(FI_SUCCESS, ret, "fi_control failed.");

	cr_expect_eq(wait_priv->fd[WAIT_READ], fd);
}

Test(eq_wait_control, mutex_cond, .init = eq_wait_mutex_cond_setup)
{
	int ret;
	struct fi_mutex_cond mutex_cond;

	ret = fi_control(&eq_priv->eq_fid.fid, FI_GETWAIT, &mutex_cond);
	cr_expect_eq(FI_SUCCESS, ret, "fi_control failed.");

	ret = memcmp(&wait_priv->mutex, mutex_cond.mutex,
		     sizeof(*mutex_cond.mutex));
	cr_expect_eq(0, ret, "mutex compare failed.");

	ret = memcmp(&wait_priv->cond, mutex_cond.cond,
		     sizeof(*mutex_cond.cond));
	cr_expect_eq(0, ret, "cond compare failed.");
}

Test(eq_wait_set, fd, .init = _setup, .fini = _teardown, .disabled = true)
{
	int ret;
	int fd;

	wait_attr.wait_obj = FI_WAIT_FD;

	ret = fi_wait_open(fab, &wait_attr, &wait_set);
	cr_expect_eq(FI_SUCCESS, ret, "fi_wait_open failed.");

	wait_priv = container_of(wait_set, struct gnix_fid_wait, wait);

	eq_attr.size = 8;
	eq_attr.wait_obj = FI_WAIT_SET;
	eq_attr.wait_set = wait_set;

	ret = fi_eq_open(fab, &eq_attr, &eq, NULL);
	cr_expect_eq(FI_SUCCESS, ret, "fi_eq_open failed.");

	eq_priv = container_of(eq, struct gnix_fid_eq, eq_fid);

	ret = fi_control(&eq_priv->eq_fid.fid, FI_GETWAIT, &fd);
	cr_expect_eq(FI_SUCCESS, ret, "fi_control failed.");

	cr_expect_eq(wait_priv->fd[WAIT_READ], fd);

	ret = fi_close(&eq->fid);
	cr_expect_eq(FI_SUCCESS, ret, "failure in closing cq.");

	ret = fi_close(&wait_set->fid);
	cr_expect_eq(FI_SUCCESS, ret, "failure in closing waitset.");
}

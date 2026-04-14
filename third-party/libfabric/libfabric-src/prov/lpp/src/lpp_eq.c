/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include "lpp.h"

static const struct fi_ops lpp_fi_eq_ops = {
	.size              = sizeof(struct fi_ops),
	.close             = lpp_fi_eq_close,
	.bind              = fi_no_bind,
	.control           = fi_no_control,
	.ops_open          = fi_no_ops_open,
};

static const struct fi_ops_eq lpp_eq_ops = {
	.size              = sizeof(struct fi_ops_eq),
	.read              = lpp_fi_eq_read,
	.readerr           = lpp_fi_eq_readerr,
	.write             = fi_no_eq_write,
	.sread             = lpp_fi_eq_sread,
	.strerror          = lpp_fi_eq_strerror,
};

static const struct fi_eq_attr lpp_default_eq_attrs = {
	.size              = 0,
	.flags             = 0,
	.wait_obj          = FI_WAIT_NONE,
	.signaling_vector  = 0,
	.wait_set          = NULL,
};

static int lpp_eq_verify_attr(struct fi_eq_attr *attr)
{

	if (attr == NULL)
		return 0;

	if ((attr->wait_obj != FI_WAIT_NONE) && (attr->wait_obj != FI_WAIT_UNSPEC))
		return -FI_ENOSYS;

	return 0;
}


int lpp_fi_eq_open(struct fid_fabric *fid, struct fi_eq_attr *attr, struct fid_eq **eq, void *context)
{
	int			status = 0;
	struct fi_eq_attr	eq_attrs = lpp_default_eq_attrs;
	struct lpp_eq		*lpp_eqp = NULL;
	struct lpp_fabric	*lpp_fabricp = container_of(fid, struct lpp_fabric, fabric_fid);

	// Verify that the attributes make sense for us.
	if (attr != NULL)
		eq_attrs = *attr;

	if (eq_attrs.size == 0)
		eq_attrs.size = 1;

	if (status = lpp_eq_verify_attr(&eq_attrs), status != 0)
		return status;

	// Allocate the EQ.
	if (lpp_eqp = calloc(1, sizeof(struct lpp_eq)), lpp_eqp == NULL)
		return -FI_ENOMEM;

	lpp_eqp->fabric = lpp_fabricp;
	lpp_eqp->attr = eq_attrs;

	// Populate the EQ.
	lpp_eqp->eq_fid.fid.fclass = FI_CLASS_EQ;
	lpp_eqp->eq_fid.fid.context = context;
	lpp_eqp->eq_fid.fid.ops = (struct fi_ops *)&lpp_fi_eq_ops;
	lpp_eqp->eq_fid.ops = (struct fi_ops_eq *)&lpp_eq_ops;
	*eq = &lpp_eqp->eq_fid;
	return 0;
}

int lpp_fi_eq_close(struct fid *eq)
{
	struct lpp_eq *lpp_eqp = container_of(eq, struct lpp_eq, eq_fid);

	free(lpp_eqp);
	return 0;
}

ssize_t lpp_fi_eq_read(struct fid_eq *eq, uint32_t *event, void *buf, size_t len, uint64_t flags)
{
	return -FI_EAGAIN;
}

ssize_t lpp_fi_eq_readerr(struct fid_eq *eq, struct fi_eq_err_entry *buf, uint64_t flags)
{
	return -FI_EAGAIN;
}

ssize_t lpp_fi_eq_sread(struct fid_eq *eq, uint32_t *event, void *buf, size_t len, int timeout, uint64_t flags)
{
	return -FI_EAGAIN;
}

const char *lpp_fi_eq_strerror(struct fid_eq *eq, int prov_errno, const void *err_data, char *buf, size_t len)
{
	return "Not implemented";
}

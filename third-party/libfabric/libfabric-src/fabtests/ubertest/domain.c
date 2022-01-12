/*
 * Copyright (c) 2013-2017 Intel Corporation.  All rights reserved.
 *
 * This software is available to you under the BSD license below:
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

#include <stdio.h>
#include <string.h>

#include "fabtest.h"


static int ft_open_fabric(void)
{
	int ret;

	if (fabric) {
		if (fabric_info->fabric_attr->fabric &&
		    fabric_info->fabric_attr->fabric != fabric) {
			printf("Opened fabric / fabric_attr mismatch\n");
			return -FI_EOTHER;
		}
		return 0;
	}

	ret = fi_fabric(fabric_info->fabric_attr, &fabric, NULL);
	if (ret)
		FT_PRINTERR("fi_fabric", ret);

	return ret;
}

static int ft_open_eq(void)
{
	struct fi_eq_attr attr;
	int ret;

	if (eq)
		return 0;

	memset(&attr, 0, sizeof attr);
	attr.wait_obj = test_info.eq_wait_obj;
	ret = fi_eq_open(fabric, &attr, &eq, NULL);
	if (ret)
		FT_PRINTERR("fi_eq_open", ret);

	return ret;
}

int ft_eq_readerr(void)
{
	struct fi_eq_err_entry err;
	ssize_t ret;

	memset(&err, 0, sizeof(err));
	ret = fi_eq_readerr(eq, &err, 0);
	if (ret != sizeof(err)) {
		FT_PRINTERR("fi_eq_readerr", ret);
		return ret;
	} else {
		fprintf(stderr, "Error event %d %s\n",
			err.err, fi_strerror(err.err));
		return err.err;
	}
}

ssize_t ft_get_event(uint32_t *event, void *buf, size_t len,
		     uint32_t event_check, size_t len_check)
{
	ssize_t ret;

	switch(test_info.eq_wait_obj) {
	case FI_WAIT_NONE:
		do {
			ret = fi_eq_read(eq, event, buf, len, 0);
			if (ret == -FI_EAVAIL) {
				return ft_eq_readerr();
			} else if (ret < 0 && ret != -FI_EAGAIN) {
				FT_PRINTERR("fi_eq_read", ret);
				return ret;
			}
		} while (ret == -FI_EAGAIN);
		break;
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
	case FI_WAIT_MUTEX_COND:
		ret = fi_eq_sread(eq, event, buf, len, FT_SREAD_TO, 0);
		if (ret == -FI_EAVAIL) {
			return ft_eq_readerr();
		} else if (ret < 0) {
			FT_PRINTERR("fi_eq_sread", ret);
			return ret;
		}
		break;
	case FI_WAIT_SET:
		FT_ERR("fi_ubertest: Unsupported eq wait object");
		return -1;
	default:
		FT_ERR("Unknown eq wait object");
		return -1;
	}

	if (event_check && event_check != *event) {
		fprintf(stderr, "Unexpected event %d, wanted %d\n",
			*event, event_check);
		return -FI_ENOMSG;

	}

	if (ret < len_check) {
		fprintf(stderr, "Reported event too small\n");
		return -FI_ETOOSMALL;
	}

	return ret;
}

static int ft_open_domain(void)
{
	int ret;

	if (domain) {
		if (fabric_info->domain_attr->domain &&
		    fabric_info->domain_attr->domain != domain) {
			fprintf(stderr, "Opened domain / domain_attr mismatch\n");
			return -FI_EDOMAIN;
		}
		return 0;
	}

	ret = fi_domain(fabric, fabric_info, &domain, NULL);
	if (ret)
		FT_PRINTERR("fi_domain", ret);

	return ret;
}

static int ft_open_av(void)
{
	struct fi_av_attr attr;
	int ret;

	if (av)
		return 0;

	memset(&attr, 0, sizeof attr);
	attr.type = test_info.av_type;
	attr.count = 2;
	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret) {
		FT_PRINTERR("fi_av_open", ret);
		return ret;
	}

	return ret;
}

static int ft_setup_xcontrol_bufs(struct ft_xcontrol *ctrl)
{
	size_t size;
	int i, ret;

	size = ft_ctrl.size_array[ft_ctrl.size_cnt - 1];
	if (!ctrl->buf) {
		ctrl->buf = calloc(1, size);
		ctrl->cpy_buf = calloc(1, size);
		if (!ctrl->buf || !ctrl->cpy_buf)
			return -FI_ENOMEM;
	} else {
		memset(ctrl->buf, 0, size);
	}

	if ((fabric_info->domain_attr->mr_mode & FI_MR_LOCAL) && !ctrl->mr) {
		ret = fi_mr_reg(domain, ctrl->buf, size,
				ft_info_to_mr_access(fabric_info),
				0, 0, 0, &ctrl->mr, NULL);
		if (ret) {
			FT_PRINTERR("fi_mr_reg", ret);
			return ret;
		}
		ctrl->memdesc = fi_mr_desc(ctrl->mr);
	}

	for (i = 0; i < ft_ctrl.iov_cnt; i++)
		ctrl->iov_desc[i] = ctrl->memdesc;

	return 0;
}

static int ft_setup_atomic_control(struct ft_atomic_control *ctrl)
{
	size_t size;
	int ret = 0;
	uint64_t access;

	size = ft_ctrl.size_array[ft_ctrl.size_cnt - 1];
	if (!ctrl->res_buf) {
		ctrl->res_buf = calloc(1, size);
		if (!ctrl->res_buf)
			return -FI_ENOMEM;
	} else {
		memset(ctrl->res_buf, 0, size);
	}

	if (!ctrl->comp_buf) {
		ctrl->comp_buf = calloc(1, size);
		if (!ctrl->comp_buf)
			return -FI_ENOMEM;
	} else {
		memset(ctrl->comp_buf, 0, size);
	}

	if (!ctrl->orig_buf) {
		ctrl->orig_buf = calloc(1, size);
		if (!ctrl->orig_buf)
			return -FI_ENOMEM;
	} else {
		memset(ctrl->orig_buf, 0, size);
	}

	if (fabric_info->domain_attr->mr_mode & FI_MR_LOCAL) {
		access = FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE;
		if (!ctrl->res_mr) {
			ret = fi_mr_reg(domain, ctrl->res_buf, size, access,
				0, 0, 0, &ctrl->res_mr, NULL);
			if (ret) {
				FT_PRINTERR("fi_mr_reg", ret);
				return ret;
			}
			ctrl->res_memdesc = fi_mr_desc(ctrl->res_mr);
		}

		if (!ctrl->comp_mr) {
			ret = fi_mr_reg(domain, ctrl->comp_buf, size, access,
				0, 0, 0, &ctrl->comp_mr, NULL);
			if (ret) {
				FT_PRINTERR("fi_mr_reg", ret);
				return ret;
			}
			ctrl->comp_memdesc = fi_mr_desc(ctrl->comp_mr);
		}
	}
	ft_atom_ctrl.op = test_info.op;
	ft_atom_ctrl.datatype = test_info.datatype;

	return ret;
}

static int ft_setup_mr_control(struct ft_mr_control *ctrl)
{
	size_t size;
	int ret;
	uint64_t access;

	if (!(fabric_info->caps & (FI_RMA | FI_ATOMIC)))
		return 0;

	size = ft_ctrl.size_array[ft_ctrl.size_cnt - 1];
	if (!ctrl->buf) {
		ctrl->buf = calloc(1, size);
		if (!ctrl->buf)
			return -FI_ENOMEM;
	} else {
		memset(ctrl->buf, 0, size);
	}

	if (!ctrl->mr) {
		access = FI_READ | FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE;
		ret = fi_mr_reg(domain, ctrl->buf, size, access, 0,
				FT_MR_KEY, 0, &ctrl->mr, NULL);
		if (ret) {
			FT_PRINTERR("fi_mr_reg", ret);
			return ret;
		}
		ctrl->memdesc = fi_mr_desc(ctrl->mr);
		ctrl->peer_mr_key = ctrl->mr_key = fi_mr_key(ctrl->mr);
	}
	
	return 0;
}

static int ft_setup_bufs(void)
{
	int ret;

	ret = ft_setup_xcontrol_bufs(&ft_rx_ctrl);
	if (ret)
		return ret;

	ret = ft_setup_xcontrol_bufs(&ft_tx_ctrl);
	if (ret)
		return ret;

	ret = ft_setup_mr_control(&ft_mr_ctrl);
	if (ret)
		return ret;

	ret = ft_setup_atomic_control(&ft_atom_ctrl);
	if (ret)
		return ret;

	return 0;
}

int ft_open_control(void)
{
	int ret;

	ret = ft_open_fabric();
	if (ret)
		return ret;

	ret = ft_open_eq();
	if (ret)
		return ret;

	ret = ft_open_domain();
	if (ret)
		return ret;

	ret = ft_setup_bufs();
	if (ret)
		return ret;

	if (test_info.ep_type != FI_EP_MSG) {
		ret = ft_open_av();
		if (ret)
			return ret;
	}

	return 0;
}

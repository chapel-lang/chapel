/*
 * Copyright (c) 2004, 2005 Topspin Communications.  All rights reserved.
 * Copyright (c) 2006, 2007 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2017-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
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

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <alloca.h>
#include <errno.h>

#include <rdma/fi_errno.h>

#include "efa.h"

static struct efa_context **ctx_list;
static int dev_cnt;

static struct efa_context *efa_device_open(struct ibv_device *device)
{
	struct efa_context *ctx;

	ctx = calloc(1, sizeof(struct efa_context));
	if (!ctx) {
		errno = ENOMEM;
		return NULL;
	}

	ctx->ibv_ctx = ibv_open_device(device);
	if (!ctx->ibv_ctx)
		goto err_free_ctx;

	return ctx;

err_free_ctx:
	free(ctx);
	return NULL;
}

static int efa_device_close(struct efa_context *ctx)
{
	ibv_close_device(ctx->ibv_ctx);
	free(ctx);

	return 0;
}

int efa_device_init(void)
{
	struct ibv_device **device_list;
	int ctx_idx;
	int ret;

	device_list = ibv_get_device_list(&dev_cnt);
	if (dev_cnt <= 0)
		return -ENODEV;

	ctx_list = calloc(dev_cnt, sizeof(*ctx_list));
	if (!ctx_list) {
		ret = -ENOMEM;
		goto err_free_dev_list;
	}

	for (ctx_idx = 0; ctx_idx < dev_cnt; ctx_idx++) {
		ctx_list[ctx_idx] = efa_device_open(device_list[ctx_idx]);
		if (!ctx_list[ctx_idx]) {
			ret = -ENODEV;
			goto err_close_devs;
		}
	}

	ibv_free_device_list(device_list);

	return 0;

err_close_devs:
	for (ctx_idx--; ctx_idx >= 0; ctx_idx--)
		efa_device_close(ctx_list[ctx_idx]);
	free(ctx_list);
err_free_dev_list:
	ibv_free_device_list(device_list);
	dev_cnt = 0;
	return ret;
}

void efa_device_free(void)
{
	int i;

	for (i = 0; i < dev_cnt; i++)
		efa_device_close(ctx_list[i]);

	free(ctx_list);
	dev_cnt = 0;
}

struct efa_context **efa_device_get_context_list(int *num_ctx)
{
	struct efa_context **devs = NULL;
	int i;

	devs = calloc(dev_cnt, sizeof(*devs));
	if (!devs)
		goto out;

	for (i = 0; i < dev_cnt; i++)
		devs[i] = ctx_list[i];
out:
	*num_ctx = devs ? dev_cnt : 0;
	return devs;
}

void efa_device_free_context_list(struct efa_context **list)
{
	free(list);
}


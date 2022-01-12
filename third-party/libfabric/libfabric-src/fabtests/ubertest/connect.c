/*
 * Copyright (c) 2013-2015 Intel Corporation.  All rights reserved.
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

static int ft_accept(void)
{
	struct fi_eq_cm_entry entry;
	uint32_t event;
	ssize_t rd;
	int ret;

	rd = ft_get_event(&event, &entry, sizeof entry,
			  FI_CONNREQ, sizeof entry);
	if (rd < 0)
		return (int) rd;

	fabric_info = entry.info;
	ret = ft_open_active();
	if (ret)
		return ret;

	ret = fi_accept(ep, NULL, 0);
	if (ret) {
		FT_PRINTERR("fi_accept", ret);
		return ret;
	}

	rd = ft_get_event(&event, &entry, sizeof entry,
			  FI_CONNECTED, sizeof entry);
	if (rd < 0)
		return (int) rd;

	return 0;
}

static int ft_connect(void)
{
	struct fi_eq_cm_entry entry;
	uint32_t event;
	ssize_t rd;
	int ret;

	ret = fi_connect(ep, fabric_info->dest_addr, NULL, 0);
	if (ret) {
		FT_PRINTERR("fi_connect", ret);
		return ret;
	}

	rd = ft_get_event(&event, &entry, sizeof entry,
			  FI_CONNECTED, sizeof entry);
	if (rd < 0)
		return (int) rd;

	return 0;
}

static int ft_load_av(void)
{
	struct ft_msg msg;
	size_t len;
	int ret;

	memset(&msg, 0, sizeof(struct ft_msg));
	len = sizeof(msg.data);
	ret = fi_getname(&ep->fid, msg.data, &len);
	if (ret) {
		FT_PRINTERR("fi_getname", ret);
		return ret;
	}

	msg.len = (uint32_t) len;
	ret = ft_sock_send(sock, &msg, sizeof msg);
	if (ret)
		return ret;

	ret = ft_sock_recv(sock, &msg, sizeof msg);
	if (ret)
		return ret;

	ret = ft_av_insert(av, msg.data, 1, &ft_tx_ctrl.addr, 0, NULL);
	if (ret)
		return ret;

	return 0;
}

int ft_enable_comm(void)
{
	if (test_info.ep_type == FI_EP_MSG)
		return pep ? ft_accept() : ft_connect();
	else
		return ft_load_av();
}

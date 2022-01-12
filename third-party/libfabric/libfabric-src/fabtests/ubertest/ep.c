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


//struct fid_stx	 *stx;
//struct fid_sep	 *sep;


int ft_open_passive(void)
{
	int ret;

	if (pep)
		return 0;

	ret = fi_passive_ep(fabric, fabric_info, &pep, NULL);
	if (ret) {
		FT_PRINTERR("fi_passive_ep", ret);
		return ret;
	}

	ret = fi_pep_bind(pep, &eq->fid, 0);
	if (ret) {
		FT_PRINTERR("fi_pep_bind", ret);
		return ret;
	}

	ret = fi_listen(pep);
	if (ret) {
		FT_PRINTERR("fi_listen", ret);
		return ret;
	}

	return 0;
}

int ft_open_active(void)
{
	int ret;

	if (ep)
		return 0;

	ret = ft_open_control();
	if (ret)
		return ret;

	ret = ft_open_comp();
	if (ret)
		return ret;

	ret = fi_endpoint(domain, fabric_info, &ep, NULL);
	if (ret) {
		FT_PRINTERR("fi_endpoint", ret);
		return ret;
	}

	ft_rx_ctrl.ep = ep;
	ft_tx_ctrl.ep = ep;

	if (test_info.ep_type == FI_EP_MSG) {
		ret = fi_ep_bind(ep, &eq->fid, 0);
		if (ret) {
			FT_PRINTERR("fi_ep_bind", ret);
			return ret;
		}
	}

	ret = ft_bind_comp(ep);
	if (ret)
		return ret;

	if (test_info.ep_type != FI_EP_MSG) {
		ret = fi_ep_bind(ep, &av->fid, 0);
		if (ret) {
			FT_PRINTERR("fi_ep_bind", ret);
			return ret;
		}
	}

	ret = fi_enable(ep);
	if (ret) {
		FT_PRINTERR("fi_enable", ret);
		return ret;
	}

	return 0;
}

int ft_reset_ep(void)
{
	int ret;

	ret = ft_comp_rx(0);
	if (ret < 0)
		return ret;

	while (ft_tx_ctrl.credits < ft_tx_ctrl.max_credits) {
		ret = ft_comp_tx(0);
		if (ret < 0)
			return ret;
	}

	memset(ft_tx_ctrl.buf, 0, ft_tx_ctrl.msg_size);
	memset(ft_rx_ctrl.buf, 0, ft_rx_ctrl.msg_size);
	ft_tx_ctrl.seqno = 0;
	ft_rx_ctrl.seqno = 0;

	return 0;
}

/*
 * Copyright (c) 2013-2015 Intel Corporation.  All rights reserved.
 * Copyright (c) 2014-2017 Cisco Systems, Inc.  All rights reserved.
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
 *	- Redistributions of source code must retain the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer.
 *
 *	- Redistributions in binary form must reproduce the above
 *	  copyright notice, this list of conditions and the following
 *	  disclaimer in the documentation and/or other materials
 *	  provided with the distribution.
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
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include <rdma/fi_errno.h>

#include "shared.h"
#include "unit_common.h"

#define TEST_ENTRY_GETINFO(name) TEST_ENTRY(getinfo_ ## name,\
					    getinfo_ ## name ## _desc)

typedef int (*ft_getinfo_init)(struct fi_info *);
typedef int (*ft_getinfo_test)(char *, char *, uint64_t, struct fi_info *,
				struct fi_info **);
typedef int (*ft_getinfo_check)(struct fi_info *);
typedef int (*ft_getinfo_init_val)(struct fi_info *, uint64_t);
typedef int (*ft_getinfo_check_val)(struct fi_info *, uint64_t);

static char err_buf[512];
static char new_prov_var[128];


static int check_addr(void *addr, size_t addrlen, char *str)
{
	if (!addrlen) {
		sprintf(err_buf, "%s addrlen not set", str);
		return EXIT_FAILURE;
	}
	if (!addr) {
		sprintf(err_buf, "%s address not set", str);
		return EXIT_FAILURE;
	}
	return 0;
}

static int check_srcaddr(struct fi_info *info)
{
	return check_addr(info->src_addr, info->src_addrlen, "source");
}

static int check_src_dest_addr(struct fi_info *info)
{
	int ret;

	ret = check_addr(info->src_addr, info->src_addrlen, "source");
	if (ret)
		return ret;

	return check_addr(info->dest_addr, info->dest_addrlen, "destination");
}

static int check_util_prov(struct fi_info *info)
{
	const char *util_name;
	size_t len;

	util_name = ft_util_name(info->fabric_attr->prov_name, &len);
	if (!util_name) {
		sprintf(err_buf, "Util provider name not appended to core "
			"provider name: %s", info->fabric_attr->prov_name);
		return EXIT_FAILURE;
	}
	return 0;
}

static int check_api_version(struct fi_info *info)
{
	return info->fabric_attr->api_version != FT_FIVERSION;
}

static int invalid_dom(struct fi_info *hints)
{
	if (hints->domain_attr->name)
		free(hints->domain_attr->name);
	hints->domain_attr->name = strdup("invalid_domain");
	if (!hints->domain_attr->name)
		return -FI_ENOMEM;
	return 0;
}

static int validate_bit_combos(char *node, char *service, uint64_t flags,
		struct fi_info *hints, struct fi_info **info, uint64_t bits,
		ft_getinfo_init_val init, ft_getinfo_check_val check)
{
	int i, ret;
	uint64_t *combinations;
	int cnt, fail, skipped;

	ret = ft_alloc_bit_combo(0, bits, &combinations, &cnt);
	if (ret) {
		FT_UNIT_STRERR(err_buf, "ft_alloc_bit_combo failed", ret);
		return ret;
	}

	for (i = 0, fail = skipped = 0; i < cnt; i++) {
		init(hints, combinations[i]);
		ret = fi_getinfo(FT_FIVERSION, node, service, flags, hints, info);
		if (ret) {
			if (ret == -FI_ENODATA) {
				skipped++;
				continue;
			}
			FT_UNIT_STRERR(err_buf, "fi_getinfo failed", ret);
			goto out;
		}

		for (fi = *info; fi; fi = fi->next) {
			if (check && check(fi, combinations[i])) {
				FT_DEBUG("%s:failed check for caps [%s]\n",
					 fi->fabric_attr->prov_name,
					 fi_tostr(&combinations[i],
					 FI_TYPE_CAPS));
				ret = -FI_EIO;
			}
		}
		if (ret)
			fail++;

		fi_freeinfo(*info);
		*info = NULL;
	}
	ret = 0;
	printf("(passed)(skipped) (%d)(%d)/%d combinations\n",
		cnt - (fail + skipped), skipped, cnt);
out:
	fi = NULL;
	ft_free_bit_combo(combinations);
	return fail ? -FI_EIO : ret;
}

#define check_has_bits(val, bits)	(((val) & (bits)) != (bits))
#define check_only_has_bits(val, bits)	((val) & ~(bits))

static int init_tx_order(struct fi_info *hints, uint64_t order)
{
	hints->tx_attr->msg_order = order;
	return 0;
}

static int check_tx_order(struct fi_info *info, uint64_t order)
{
	return check_has_bits(info->tx_attr->msg_order, order);
}

static int validate_tx_ordering_bits(char *node, char *service, uint64_t flags,
		struct fi_info *hints, struct fi_info **info)
{
	return validate_bit_combos(node, service, flags, hints, info,
				   FI_ORDER_STRICT | FI_ORDER_DATA,
				   init_tx_order, check_tx_order);
}

static int init_rx_order(struct fi_info *hints, uint64_t order)
{
	hints->rx_attr->msg_order = order;
	return 0;
}

static int check_rx_order(struct fi_info *info, uint64_t order)
{
	return check_has_bits(info->rx_attr->msg_order, order);
}

static int validate_rx_ordering_bits(char *node, char *service, uint64_t flags,
		struct fi_info *hints, struct fi_info **info)
{
	return validate_bit_combos(node, service, flags, hints, info,
				   FI_ORDER_STRICT | FI_ORDER_DATA,
				   init_rx_order, check_rx_order);
}

static int init_caps(struct fi_info *hints, uint64_t bits)
{
	hints->caps = bits;
	return 0;
}

#define PRIMARY_TX_CAPS	(FI_MSG | FI_RMA | FI_TAGGED | FI_ATOMIC | \
			 FI_MULTICAST | FI_NAMED_RX_CTX | FI_HMEM | \
			 FI_COLLECTIVE)
#define PRIMARY_RX_CAPS (FI_MSG | FI_RMA | FI_TAGGED | FI_ATOMIC | \
			 FI_DIRECTED_RECV | FI_VARIABLE_MSG | \
			 FI_HMEM | FI_COLLECTIVE)

#define PRIMARY_CAPS (PRIMARY_TX_CAPS | PRIMARY_RX_CAPS)
#define DOMAIN_CAPS (FI_LOCAL_COMM | FI_REMOTE_COMM | FI_SHARED_AV)
#define SEC_TX_CAPS (FI_TRIGGER | FI_FENCE | FI_RMA_PMEM)
#define SEC_RX_CAPS (FI_RMA_PMEM | FI_SOURCE | FI_SOURCE_ERR | \
		     FI_RMA_EVENT | FI_MULTI_RECV | FI_TRIGGER)
#define MOD_TX_CAPS (FI_SEND | FI_READ | FI_WRITE)
#define MOD_RX_CAPS (FI_RECV | FI_REMOTE_READ | FI_REMOTE_WRITE)
#define OPT_TX_CAPS (MOD_TX_CAPS | SEC_TX_CAPS)
#define OPT_RX_CAPS (MOD_RX_CAPS | SEC_RX_CAPS)
#define OPT_CAPS (DOMAIN_CAPS | OPT_TX_CAPS | OPT_RX_CAPS)

static void print_incorrect_caps(char *prov, char *attr,
				 uint64_t expected, uint64_t actual)
{
	FT_DEBUG("%s: %s->caps has unexpected caps -\n", prov, attr);
	FT_DEBUG("expected\t[%s]\n", fi_tostr(&expected, FI_TYPE_CAPS));
	FT_DEBUG("actual\t[%s]\n", fi_tostr(&actual, FI_TYPE_CAPS));
}

static int check_no_extra_caps(struct fi_info *info, uint64_t caps)
{
	if (caps & check_only_has_bits(info->caps, caps | OPT_CAPS)) {
		print_incorrect_caps(info->fabric_attr->prov_name, "info",
				caps & PRIMARY_CAPS, info->caps & ~OPT_CAPS);
		return 1;
	}
	if (check_only_has_bits(info->tx_attr->caps,
				PRIMARY_TX_CAPS | OPT_TX_CAPS)) {
		print_incorrect_caps(info->fabric_attr->prov_name, "tx_attr",
				     caps & PRIMARY_TX_CAPS,
				     info->tx_attr->caps & ~OPT_TX_CAPS);
		return 1;
	}
	if (check_only_has_bits(info->tx_attr->caps, info->caps)) {
		print_incorrect_caps(info->fabric_attr->prov_name, "tx_attr",
				     info->caps & (PRIMARY_TX_CAPS | OPT_TX_CAPS),
				     info->tx_attr->caps);
	}
	if (check_only_has_bits(info->rx_attr->caps,
				PRIMARY_RX_CAPS | OPT_RX_CAPS)) {
		print_incorrect_caps(info->fabric_attr->prov_name, "rx_attr",
				     caps & PRIMARY_RX_CAPS,
				     info->rx_attr->caps & ~OPT_RX_CAPS);
		return 1;
	}
	if (check_only_has_bits(info->rx_attr->caps, info->caps)) {
		print_incorrect_caps(info->fabric_attr->prov_name, "rx_attr",
				     info->caps & (PRIMARY_RX_CAPS | OPT_RX_CAPS),
				     info->rx_attr->caps);
		return 1;
	}
	return 0;
}

static int check_caps(struct fi_info *info, uint64_t caps)
{
	int ret;

	ret = check_no_extra_caps(info, caps);
	if (!caps)
		return ret;

	if (check_has_bits(info->caps, caps)) {
		print_incorrect_caps(info->fabric_attr->prov_name, "info",
				caps & PRIMARY_CAPS, info->caps & ~OPT_CAPS);
		return 1;
	}
	if (check_has_bits(info->tx_attr->caps, caps & PRIMARY_TX_CAPS)) {
		print_incorrect_caps(info->fabric_attr->prov_name, "tx_attr",
				     caps & PRIMARY_TX_CAPS,
				     info->tx_attr->caps & ~OPT_TX_CAPS);
		return 1;
	}
	if (check_has_bits(info->rx_attr->caps, caps & PRIMARY_RX_CAPS)) {
		print_incorrect_caps(info->fabric_attr->prov_name, "rx_attr",
				     caps & PRIMARY_RX_CAPS,
				     info->rx_attr->caps & ~OPT_RX_CAPS);
		return 1;
	}

	return 0;
}

static int validate_primary_caps(char *node, char *service, uint64_t flags,
		struct fi_info *hints, struct fi_info **info)
{
	return validate_bit_combos(node, service, flags, hints, info,
				   PRIMARY_TX_CAPS | PRIMARY_RX_CAPS,
				   init_caps, check_caps);
}

static int test_null_hints_caps(struct fi_info *info)
{
	return check_no_extra_caps(info, 0);
}

static int init_valid_rma_RAW_ordering_no_set_size(struct fi_info *hints)
{
	hints->caps = FI_RMA;
	hints->tx_attr->msg_order = FI_ORDER_RAW;
	hints->rx_attr->msg_order = FI_ORDER_RAW;
	hints->ep_attr->max_order_raw_size = 0;

	return 0;
}

static int init_valid_rma_RAW_ordering_set_size(struct fi_info *hints)
{
	int ret;
	struct fi_info *fi;

	hints->caps = FI_RMA;
	hints->tx_attr->msg_order = FI_ORDER_RAW;
	hints->rx_attr->msg_order = FI_ORDER_RAW;

	ret = fi_getinfo(FT_FIVERSION, NULL, NULL, 0, hints, &fi);
	if (ret) {
		sprintf(err_buf, "fi_getinfo returned %d - %s",
			-ret, fi_strerror(-ret));
		return ret;
	}
	if (fi->ep_attr->max_order_raw_size > 0) {
		hints->ep_attr->max_order_raw_size =
				fi->ep_attr->max_order_raw_size - 1;
	}

	fi_freeinfo(fi);

	return 0;
}

static int init_valid_rma_WAR_ordering_no_set_size(struct fi_info *hints)
{
	hints->caps = FI_RMA;
	hints->tx_attr->msg_order = FI_ORDER_WAR;
	hints->rx_attr->msg_order = FI_ORDER_WAR;
	hints->ep_attr->max_order_war_size = 0;

	return 0;
}

static int init_valid_rma_WAR_ordering_set_size(struct fi_info *hints)
{
	int ret;
	struct fi_info *fi;

	hints->caps = FI_RMA;
	hints->tx_attr->msg_order = FI_ORDER_WAR;
	hints->rx_attr->msg_order = FI_ORDER_WAR;

	ret = fi_getinfo(FT_FIVERSION, NULL, NULL, 0, hints, &fi);
	if (ret) {
		sprintf(err_buf, "fi_getinfo returned %d - %s",
			-ret, fi_strerror(-ret));
		return ret;
	}
	if (fi->ep_attr->max_order_war_size > 0) {
		hints->ep_attr->max_order_war_size =
				fi->ep_attr->max_order_war_size - 1;
	}

	fi_freeinfo(fi);

	return 0;
}

static int init_valid_rma_WAW_ordering_no_set_size(struct fi_info *hints)
{
	hints->caps = FI_RMA;
	hints->tx_attr->msg_order = FI_ORDER_WAW;
	hints->rx_attr->msg_order = FI_ORDER_WAW;
	hints->ep_attr->max_order_waw_size = 0;

	return 0;
}

static int init_valid_rma_WAW_ordering_set_size(struct fi_info *hints)
{
	int ret;
	struct fi_info *fi;

	hints->caps = FI_RMA;
	hints->tx_attr->msg_order = FI_ORDER_WAW;
	hints->rx_attr->msg_order = FI_ORDER_WAW;
	ret = fi_getinfo(FT_FIVERSION, NULL, NULL, 0, hints, &fi);
	if (ret) {
		sprintf(err_buf, "fi_getinfo returned %d - %s",
			-ret, fi_strerror(-ret));
		return ret;
	}
	if (fi->ep_attr->max_order_waw_size > 0) {
		hints->ep_attr->max_order_waw_size =
				fi->ep_attr->max_order_waw_size - 1;
	}

	fi_freeinfo(fi);

	return 0;
}

static int check_valid_rma_ordering_sizes(struct fi_info *info)
{
	if ((info->tx_attr->msg_order & FI_ORDER_RAW) ||
	    (info->rx_attr->msg_order & FI_ORDER_RAW)) {
		if (info->ep_attr->max_order_raw_size <= 0)
			return EXIT_FAILURE;
		if (hints->ep_attr->max_order_raw_size) {
			if (info->ep_attr->max_order_raw_size <
			    hints->ep_attr->max_order_raw_size)
				return EXIT_FAILURE;
		}
	}
	if ((info->tx_attr->msg_order & FI_ORDER_WAR) ||
	    (info->rx_attr->msg_order & FI_ORDER_WAR)) {
		if (info->ep_attr->max_order_war_size <= 0)
			return EXIT_FAILURE;
		if (hints->ep_attr->max_order_war_size) {
			if (info->ep_attr->max_order_war_size <
			    hints->ep_attr->max_order_war_size)
				return EXIT_FAILURE;
		}
	}
	if ((info->tx_attr->msg_order & FI_ORDER_WAW) ||
	    (info->rx_attr->msg_order & FI_ORDER_WAW)) {
		if (info->ep_attr->max_order_waw_size <= 0)
			return EXIT_FAILURE;
		if (hints->ep_attr->max_order_waw_size) {
			if (info->ep_attr->max_order_waw_size <
			    hints->ep_attr->max_order_waw_size)
				return EXIT_FAILURE;
		}
	}

	return 0;
}

static int init_invalid_rma_RAW_ordering_size(struct fi_info *hints)
{
	int ret;
	struct fi_info *fi;

	hints->caps |= FI_RMA;
	hints->tx_attr->msg_order = FI_ORDER_RAW;
	hints->rx_attr->msg_order = FI_ORDER_RAW;
	hints->ep_attr->max_order_war_size = 0;
	hints->ep_attr->max_order_waw_size = 0;

	ret = fi_getinfo(FT_FIVERSION, NULL, NULL, 0, hints, &fi);
	if (ret) {
		sprintf(err_buf, "fi_getinfo returned %d - %s",
			-ret, fi_strerror(-ret));
		return ret;
	}

	if (fi->ep_attr->max_order_raw_size) {
		hints->ep_attr->max_order_raw_size =
				fi->ep_attr->max_order_raw_size + 1;
	}

	fi_freeinfo(fi);

	return 0;
}

static int init_invalid_rma_WAR_ordering_size(struct fi_info *hints)
{
	int ret;
	struct fi_info *fi;

	hints->caps |= FI_RMA;
	hints->tx_attr->msg_order = FI_ORDER_WAR;
	hints->rx_attr->msg_order = FI_ORDER_WAR;
	hints->ep_attr->max_order_raw_size = 0;
	hints->ep_attr->max_order_waw_size = 0;

	ret = fi_getinfo(FT_FIVERSION, NULL, NULL, 0, hints, &fi);
	if (ret) {
		sprintf(err_buf, "fi_getinfo returned %d - %s",
			-ret, fi_strerror(-ret));
		return ret;
	}

	if (fi->ep_attr->max_order_war_size) {
		hints->ep_attr->max_order_war_size =
				fi->ep_attr->max_order_war_size + 1;
	}

	fi_freeinfo(fi);

	return 0;
}

static int init_invalid_rma_WAW_ordering_size(struct fi_info *hints)
{
	int ret;
	struct fi_info *fi;

	hints->caps |= FI_RMA;
	hints->tx_attr->msg_order = FI_ORDER_WAW;
	hints->rx_attr->msg_order = FI_ORDER_WAW;
	hints->ep_attr->max_order_raw_size = 0;
	hints->ep_attr->max_order_war_size = 0;

	ret = fi_getinfo(FT_FIVERSION, NULL, NULL, 0, hints, &fi);
	if (ret) {
		sprintf(err_buf, "fi_getinfo returned %d - %s",
			-ret, fi_strerror(-ret));
		return ret;
	}

	if (fi->ep_attr->max_order_waw_size) {
		hints->ep_attr->max_order_waw_size =
				fi->ep_attr->max_order_waw_size + 1;
	}

	fi_freeinfo(fi);

	return 0;
}


/*
 * MR mode checks
 */
static int init_mr_basic(struct fi_info *hints)
{
	hints->caps |= FI_RMA;
	hints->domain_attr->mr_mode = FI_MR_BASIC;
	return 0;
}

static int check_mr_basic(struct fi_info *info)
{
	return (info->domain_attr->mr_mode != FI_MR_BASIC) ?
		EXIT_FAILURE : 0;
}

static int init_mr_scalable(struct fi_info *hints)
{
	hints->caps |= FI_RMA;
	hints->domain_attr->mr_mode = FI_MR_SCALABLE;
	return 0;
}

static int check_mr_scalable(struct fi_info *info)
{
	return (info->domain_attr->mr_mode != FI_MR_SCALABLE) ?
		EXIT_FAILURE : 0;
}

static int init_mr_unspec(struct fi_info *hints)
{
	hints->caps |= FI_RMA;
	hints->domain_attr->mr_mode = FI_MR_UNSPEC;
	return 0;
}

static int test_mr_v1_0(char *node, char *service, uint64_t flags,
			struct fi_info *test_hints, struct fi_info **info)
{
	return fi_getinfo(FI_VERSION(1, 0), node, service, flags,
			  test_hints, info);
}

static int check_mr_unspec(struct fi_info *info)
{
	return (info->domain_attr->mr_mode != FI_MR_BASIC &&
		info->domain_attr->mr_mode != FI_MR_SCALABLE) ?
		EXIT_FAILURE : 0;
}

static int init_mr_mode(struct fi_info *hints, uint64_t mode)
{
	hints->domain_attr->mr_mode = (uint32_t) mode;
	return 0;
}

static int check_mr_mode(struct fi_info *info, uint64_t mode)
{
	return check_only_has_bits(info->domain_attr->mr_mode, mode);
}

static int validate_mr_modes(char *node, char *service, uint64_t flags,
		struct fi_info *hints, struct fi_info **info)
{
	uint64_t mode_bits = FI_MR_LOCAL | FI_MR_RAW | FI_MR_VIRT_ADDR |
			FI_MR_ALLOCATED | FI_MR_PROV_KEY | FI_MR_MMU_NOTIFY |
			FI_MR_RMA_EVENT | FI_MR_ENDPOINT;

	return validate_bit_combos(node, service, flags, hints, info, mode_bits,
				   init_mr_mode, check_mr_mode);
}

/*
 * Progress checks
 */
static int init_data_manual(struct fi_info *hints)
{
	hints->domain_attr->data_progress = FI_PROGRESS_MANUAL;
	return 0;
}

static int init_data_auto(struct fi_info *hints)
{
	hints->domain_attr->data_progress = FI_PROGRESS_AUTO;
	return 0;
}

static int init_ctrl_manual(struct fi_info *hints)
{
	hints->domain_attr->control_progress = FI_PROGRESS_MANUAL;
	return 0;
}

static int init_ctrl_auto(struct fi_info *hints)
{
	hints->domain_attr->control_progress = FI_PROGRESS_AUTO;
	return 0;
}

static int check_data_manual(struct fi_info *info)
{
	return (info->domain_attr->data_progress != FI_PROGRESS_MANUAL) ?
		EXIT_FAILURE : 0;
}

static int check_data_auto(struct fi_info *info)
{
	return (info->domain_attr->data_progress != FI_PROGRESS_AUTO) ?
		EXIT_FAILURE : 0;
}

static int check_ctrl_manual(struct fi_info *info)
{
	return (info->domain_attr->control_progress != FI_PROGRESS_MANUAL) ?
		EXIT_FAILURE : 0;
}

static int check_ctrl_auto(struct fi_info *info)
{
	return (info->domain_attr->control_progress != FI_PROGRESS_AUTO) ?
		EXIT_FAILURE : 0;
}


static int init_domain_caps(struct fi_info *hints, uint64_t caps)
{
	hints->domain_attr->caps = caps;
	return 0;
}

static int check_domain_caps(struct fi_info *info, uint64_t caps)
{
	return check_has_bits(info->domain_attr->caps, caps);
}

static int validate_domain_caps(char *node, char *service, uint64_t flags,
		struct fi_info *hints, struct fi_info **info)
{
	return validate_bit_combos(node, service, flags, hints, info,
				   FI_LOCAL_COMM | FI_REMOTE_COMM | FI_SHARED_AV,
				   init_domain_caps, check_domain_caps);
}

/* Some apps (MPI) request all fi_info structures, and use the output to
 * form the hints for a second call.  This usage breaks if the provider
 * adds a new capability bit that also requires setting a mode or mr_mode
 * bit (new or otherwise), which the app does not set.
 * This is really a problem with the app, but avoid a regression
 * by verifying that providers do not add new requirements for apps that
 * inadvertently pick up a new capability bit.
 */
static int test_caps_regression(char *node, char *service, uint64_t flags,
		struct fi_info *hints, struct fi_info **info)
{
	struct fi_info *fi;
	int ret;

	ret = fi_getinfo(FT_FIVERSION, node, service, flags, NULL, info);
	if (ret)
		return ret;

	if (!hints || !hints->fabric_attr || !hints->fabric_attr->prov_name) {
		fi = *info;
	} else {
		for (fi = *info; fi; fi = fi->next) {
			if (!strcasecmp(hints->fabric_attr->prov_name,
					(*info)->fabric_attr->prov_name))
				break;
		}
	}

	if (!fi)
		return 0;

	/* Limit mode bits to common, older options only */
	hints->caps |= fi->caps;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = FI_MR_LOCAL | OFI_MR_BASIC_MAP;

	fi_freeinfo(*info);
	*info = NULL;

	ret = fi_getinfo(FT_FIVERSION, node, service, flags, hints, info);
	if (ret) {
		printf("regression: new mode/mr_mode bits required...");
		return -FI_EINVAL;
	}

	return 0;
}


/*
 * getinfo test
 */
static int getinfo_unit_test(char *node, char *service, uint64_t flags,
		struct fi_info *base_hints, ft_getinfo_init init,
		ft_getinfo_test test, ft_getinfo_check check, int ret_exp)
{
	struct fi_info *info = NULL, *fi, *test_hints = NULL;
	int ret;

	if (base_hints) {
		test_hints = fi_dupinfo(base_hints);
		if (!test_hints)
			return -FI_ENOMEM;
	}

	if (init) {
		ret = init(test_hints);
		if (ret)
			goto out;
	}

	if (test) {
		ret = test(node, service, flags, test_hints, &info);
	} else {
		ret = fi_getinfo(FT_FIVERSION, node, service, flags,
				 test_hints, &info);
	}
	if (ret) {
		if (ret == ret_exp) {
			ret = 0;
			goto out;
		}
		sprintf(err_buf, "fi_getinfo returned %d - %s",
			-ret, fi_strerror(-ret));
		goto out;
	}

	if (!info || !check)
		goto out;

	for (fi = info; fi; fi = fi->next) {
		FT_DEBUG("\nTesting for fabric: %s, domain: %s, endpoint type: %d",
			 fi->fabric_attr->prov_name, fi->domain_attr->name,
			 fi->ep_attr->type);
		ret = check(fi);
		if (ret)
			break;
	}
out:
	fi_freeinfo(test_hints);
	fi_freeinfo(info);
	return ret;
}

#define getinfo_test(name, num, desc, node, service, flags, hints, 	\
		     init, test, check, ret_exp)			\
char *getinfo_ ## name ## num ## _desc = desc;				\
static int getinfo_ ## name ## num(void)				\
{									\
	int ret, testret = FAIL;					\
	ret = getinfo_unit_test(node, service, flags, hints, init,	\
				test, check, ret_exp);			\
	if (ret)							\
		goto fail;						\
	testret = PASS;							\
fail:									\
	return TEST_RET_VAL(ret, testret);				\
}

/*
 * Tests:
 */


/* 1. No hints tests
 * These tests do not receive hints. If a particular provider has been
 * requested, the environment variable FI_PROVIDER will be set to restrict
 * the provider used. Otherwise, test failures may occur for any provider.
 */

/* 1.1 Source address only tests */
getinfo_test(no_hints, 1, "Test with no node, service, flags or hints",
	     NULL, NULL, 0, NULL, NULL, NULL, check_srcaddr, 0)
getinfo_test(no_hints, 2, "Test with node, no service, FI_SOURCE flag, no hints",
	     opts.src_addr ? opts.src_addr : "localhost", NULL, FI_SOURCE,
	     NULL, NULL, NULL, check_srcaddr, 0)
getinfo_test(no_hints, 3, "Test with service, FI_SOURCE flag, no node, no hints",
		 NULL, opts.src_port, FI_SOURCE, NULL, NULL,
		 NULL, check_srcaddr, 0)
getinfo_test(no_hints, 4, "Test with node, service, FI_SOURCE flag, no hints",
	     opts.src_addr ? opts.src_addr : "localhost", opts.src_port,
	     FI_SOURCE, NULL, NULL, NULL, check_srcaddr, 0)

/* 1.2 Source and destination address tests */
getinfo_test(no_hints, 5, "Test with node, service, no hints",
	     opts.dst_addr ? opts.dst_addr : "localhost", opts.dst_port,
	     0, NULL, NULL, NULL, check_src_dest_addr, 0)

/* 2. Tests, most with hints */
/* 2.1 Source address only tests */
getinfo_test(src, 1, "Test with no node, service, or flags",
	     NULL, NULL, 0, hints, NULL, NULL, check_srcaddr, 0)
getinfo_test(src, 2, "Test with node, no service, FI_SOURCE flag",
	     opts.src_addr ? opts.src_addr : "localhost", NULL, FI_SOURCE,
	     hints, NULL, NULL, check_srcaddr, 0)
getinfo_test(src, 3, "Test with service, FI_SOURCE flag, no node",
	     NULL, opts.src_port, FI_SOURCE, hints, NULL,
	     NULL, check_srcaddr, 0)
getinfo_test(src, 4, "Test with node, service, FI_SOURCE flag",
	     opts.src_addr ? opts.src_addr : "localhost", opts.src_port,
	     FI_SOURCE, hints, NULL, NULL, check_srcaddr, 0)

/* 2.2 Source and destination address tests */
getinfo_test(src_dest, 1, "Test with node, service",
	     opts.dst_addr ? opts.dst_addr : "localhost", opts.dst_port,
	     0, hints, NULL, NULL, check_src_dest_addr, 0)

getinfo_test(src_dest, 2, "Test API version",
	     NULL, NULL, 0, hints, NULL, NULL, check_api_version , 0)

/* Negative tests */
getinfo_test(neg, 1, "Test with non-existent domain name",
	     NULL, NULL, 0, hints, invalid_dom, NULL, NULL, -FI_ENODATA)

/* Utility provider tests */
getinfo_test(util, 1, "Test if we get utility provider when requested",
	     NULL, NULL, 0, hints, NULL, NULL, check_util_prov, 0)

/* Message Ordering Tests */
getinfo_test(msg_ordering, 1, "Test tx ordering bits supported are set",
	     NULL, NULL, 0, hints, NULL, validate_tx_ordering_bits, NULL, 0)
getinfo_test(msg_ordering, 2, "Test rx ordering bits supported are set",
	     NULL, NULL, 0, hints, NULL, validate_rx_ordering_bits, NULL, 0)

getinfo_test(raw_ordering, 1, "Test rma RAW ordering size is set",
	     NULL, NULL, 0, hints, init_valid_rma_RAW_ordering_no_set_size,
	     NULL, check_valid_rma_ordering_sizes, 0)
getinfo_test(raw_ordering, 2, "Test rma RAW ordering size is set to hints",
	     NULL, NULL, 0, hints, init_valid_rma_RAW_ordering_set_size,
	     NULL, check_valid_rma_ordering_sizes, 0)
getinfo_test(war_ordering, 1, "Test rma WAR ordering size is set",
	     NULL, NULL, 0, hints, init_valid_rma_WAR_ordering_no_set_size,
	     NULL, check_valid_rma_ordering_sizes, 0)
getinfo_test(war_ordering, 2, "Test rma WAR ordering size is set to hints",
	     NULL, NULL, 0, hints, init_valid_rma_WAR_ordering_set_size,
	     NULL, check_valid_rma_ordering_sizes, 0)
getinfo_test(waw_ordering, 1, "Test rma WAW ordering size is set",
	     NULL, NULL, 0, hints, init_valid_rma_WAW_ordering_no_set_size,
	     NULL, check_valid_rma_ordering_sizes, 0)
getinfo_test(waw_ordering, 2, "Test rma WAW ordering size is set to hints",
	     NULL, NULL, 0, hints, init_valid_rma_WAW_ordering_set_size,
	     NULL, check_valid_rma_ordering_sizes, 0)
getinfo_test(bad_raw_ordering, 1, "Test invalid rma RAW ordering size",
	     NULL, NULL, 0, hints, init_invalid_rma_RAW_ordering_size,
	     NULL, NULL, -FI_ENODATA)
getinfo_test(bad_war_ordering, 1, "Test invalid rma WAR ordering size",
	     NULL, NULL, 0, hints, init_invalid_rma_WAR_ordering_size,
	     NULL, NULL, -FI_ENODATA)
getinfo_test(bad_waw_ordering, 1, "Test invalid rma WAW ordering size",
	     NULL, NULL, 0, hints, init_invalid_rma_WAW_ordering_size,
	     NULL, NULL, -FI_ENODATA)

/* MR mode tests */
getinfo_test(mr_mode, 1, "Test FI_MR_BASIC", NULL, NULL, 0,
	     hints, init_mr_basic, NULL, check_mr_basic, -FI_ENODATA)
getinfo_test(mr_mode, 2, "Test FI_MR_SCALABLE", NULL, NULL, 0,
	     hints, init_mr_scalable, NULL, check_mr_scalable, -FI_ENODATA)
getinfo_test(mr_mode, 3, "Test FI_MR_UNSPEC (v1.0)", NULL, NULL, 0,
	     hints, init_mr_unspec, test_mr_v1_0, check_mr_unspec, -FI_ENODATA)
getinfo_test(mr_mode, 4, "Test FI_MR_BASIC (v1.0)", NULL, NULL, 0,
	     hints, init_mr_basic, test_mr_v1_0, check_mr_basic, -FI_ENODATA)
getinfo_test(mr_mode, 5, "Test FI_MR_SCALABLE (v1.0)", NULL, NULL, 0,
     	     hints, init_mr_scalable, test_mr_v1_0, check_mr_scalable,
	     -FI_ENODATA)
getinfo_test(mr_mode, 6, "Test mr_mode bits", NULL, NULL, 0,
	     hints, NULL, validate_mr_modes, NULL, 0)

/* Progress tests */
getinfo_test(progress, 1, "Test data manual progress", NULL, NULL, 0,
	     hints, init_data_manual, NULL, check_data_manual, 0)
getinfo_test(progress, 2, "Test data auto progress", NULL, NULL, 0,
	     hints, init_data_auto, NULL, check_data_auto, 0)
getinfo_test(progress, 3, "Test ctrl manual progress", NULL, NULL, 0,
	     hints, init_ctrl_manual, NULL, check_ctrl_manual, 0)
getinfo_test(progress, 4, "Test ctrl auto progress", NULL, NULL, 0,
	     hints, init_ctrl_auto, NULL, check_ctrl_auto, 0)

/* Capability test */
getinfo_test(caps, 1, "Test capability bits supported are set",
	     NULL, NULL, 0, hints, NULL, validate_primary_caps, NULL, 0)
getinfo_test(caps, 2, "Test capability with no hints",
	     NULL, NULL, 0, NULL, NULL, NULL, test_null_hints_caps, 0)
getinfo_test(caps, 3, "Test domain capabilities", NULL, NULL, 0,
	     hints, NULL, validate_domain_caps, NULL, 0)
getinfo_test(caps, 4, "Test for capability bit regression",
	     NULL, NULL, 0, hints, NULL, test_caps_regression, NULL, 0)


static void usage(void)
{
	ft_unit_usage("getinfo_test", "Unit tests for fi_getinfo");
	FT_PRINT_OPTS_USAGE("-e <ep_type>",
			    "Endpoint type: msg|rdm|dgram (default:rdm)");
	ft_addr_usage();
}

static int set_prov(char *prov_name)
{
	const char *util_name;
	const char *core_name;
	char *core_name_dup;
	size_t len;

	util_name = ft_util_name(prov_name, &len);
	core_name = ft_core_name(prov_name, &len);

	if (util_name && !core_name)
		return 0;

	core_name_dup = strndup(core_name, len);
	if (!core_name_dup)
		return -FI_ENOMEM;

	snprintf(new_prov_var, sizeof(new_prov_var) - 1, "FI_PROVIDER=%s",
		 core_name_dup);

	putenv(new_prov_var);
	free(core_name_dup);
	return 0;
}

int main(int argc, char **argv)
{
	int failed;
	int op;
	size_t len;
	const char *util_name;

	struct test_entry no_hint_tests[] = {
		TEST_ENTRY_GETINFO(no_hints1),
		TEST_ENTRY_GETINFO(no_hints2),
		TEST_ENTRY_GETINFO(no_hints3),
		TEST_ENTRY_GETINFO(no_hints4),
		TEST_ENTRY_GETINFO(no_hints5),
		{ NULL, "" }
	};

	struct test_entry hint_tests[] = {
		TEST_ENTRY_GETINFO(src1),
		TEST_ENTRY_GETINFO(src2),
		TEST_ENTRY_GETINFO(src3),
		TEST_ENTRY_GETINFO(src4),
		TEST_ENTRY_GETINFO(src_dest1),
		TEST_ENTRY_GETINFO(src_dest2),
		TEST_ENTRY_GETINFO(msg_ordering1),
		TEST_ENTRY_GETINFO(msg_ordering2),
		TEST_ENTRY_GETINFO(raw_ordering1),
		TEST_ENTRY_GETINFO(raw_ordering2),
		TEST_ENTRY_GETINFO(war_ordering1),
		TEST_ENTRY_GETINFO(war_ordering2),
		TEST_ENTRY_GETINFO(waw_ordering1),
		TEST_ENTRY_GETINFO(waw_ordering2),
		TEST_ENTRY_GETINFO(bad_raw_ordering1),
		TEST_ENTRY_GETINFO(bad_war_ordering1),
		TEST_ENTRY_GETINFO(bad_waw_ordering1),
		TEST_ENTRY_GETINFO(neg1),
		TEST_ENTRY_GETINFO(mr_mode1),
		TEST_ENTRY_GETINFO(mr_mode2),
		TEST_ENTRY_GETINFO(mr_mode3),
		TEST_ENTRY_GETINFO(mr_mode4),
		TEST_ENTRY_GETINFO(mr_mode5),
		TEST_ENTRY_GETINFO(mr_mode6),
		TEST_ENTRY_GETINFO(progress1),
		TEST_ENTRY_GETINFO(progress2),
		TEST_ENTRY_GETINFO(progress3),
		TEST_ENTRY_GETINFO(progress4),
		TEST_ENTRY_GETINFO(caps1),
		TEST_ENTRY_GETINFO(caps2),
		TEST_ENTRY_GETINFO(caps3),
		TEST_ENTRY_GETINFO(caps4),
		{ NULL, "" }
	};

	struct test_entry util_prov_tests[] = {
		TEST_ENTRY_GETINFO(util1),
		{ NULL, "" }
	};

	opts = INIT_OPTS;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, ADDR_OPTS INFO_OPTS "h")) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case 'h':
			usage();
			return EXIT_SUCCESS;
		case '?':
			usage();
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];
	if (!opts.dst_port)
		opts.dst_port = "9228";
	if (!opts.src_port)
		opts.src_port = "9228";

	hints->mode = ~0;
	hints->domain_attr->mr_mode = opts.mr_mode;

	if (hints->fabric_attr->prov_name) {
		if (set_prov(hints->fabric_attr->prov_name))
			return EXIT_FAILURE;
	} else {
	       FT_WARN("\nSome tests do not pass in hints, and may not run "
		       "exclusively for a particular provider.\n"
		       "Failures in any of those tests may not be "
		       "attributable to a specific provider.\n");
	}

	failed = run_tests(no_hint_tests, err_buf);

	if (hints->fabric_attr->prov_name) {
		util_name = ft_util_name(hints->fabric_attr->prov_name, &len);
		if (util_name)
			failed += run_tests(util_prov_tests, err_buf);
	}

	failed += run_tests(hint_tests, err_buf);

	if (failed > 0) {
		printf("\nSummary: %d tests failed\n", failed);
	} else {
		printf("\nSummary: all tests passed\n");
	}

	ft_free_res();
	return (failed > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}

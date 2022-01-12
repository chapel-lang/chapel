/*
 * Copyright (c) 2018 Intel Corporation.  All rights reserved.
 *
 * This software is available to you under the BSD license
 * below:
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
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

#include <shared.h>


static struct fi_info *base_hints;


static int av_removal_test(void)
{
	int ret;

	fprintf(stdout, "AV address removal: ");
	hints = fi_dupinfo(base_hints);
	if (!hints)
		return -FI_ENOMEM;

	ret = ft_init_fabric();
	if (ret)
		goto out;

	if (opts.dst_addr) {
		ret = ft_tx(ep, remote_fi_addr, opts.transfer_size, &tx_ctx);
		if (ret) {
			FT_PRINTERR("ft_tx", -ret);
			goto out;
		}

		ret = fi_av_remove(av, &remote_fi_addr, 1, 0);
		if (ret) {
			FT_PRINTERR("fi_av_remove", ret);
			goto out;
		}

		ret = ft_sync();
		if (ret)
			goto out;

		ret = ft_init_av();
		if (ret) {
			FT_PRINTERR("ft_init_av", -ret);
			goto out;
		}

		ret = ft_rx(ep, opts.transfer_size);
		if (ret) {
			FT_PRINTERR("ft_rx", -ret);
			goto out;
		}
	} else {
		ret = ft_rx(ep, opts.transfer_size);
		if (ret) {
			FT_PRINTERR("ft_rx", -ret);
			goto out;
		}

		ret = fi_av_remove(av, &remote_fi_addr, 1, 0);
		if (ret) {
			FT_PRINTERR("fi_av_remove", ret);
			goto out;
		}

		ret = ft_sync();
		if (ret)
			goto out;

		ret = ft_init_av();
		if (ret) {
			FT_PRINTERR("ft_init_av", -ret);
			goto out;
		}

		ret = ft_tx(ep, remote_fi_addr, opts.transfer_size, &tx_ctx);
		if (ret) {
			FT_PRINTERR("ft_tx", -ret);
			goto out;
		}
	}

	(void) ft_sync();
out:
	fprintf(stdout, "%s\n", ret ? "FAIL" : "PASS");
	ft_free_res();
	return ret;
}

static int av_reinsert_test(void)
{
	int ret;

	fprintf(stdout, "AV re-insertion address: ");
	hints = fi_dupinfo(base_hints);
	if (!hints)
		return -FI_ENOMEM;

	ret = ft_init_fabric();
	if (ret)
		goto out;

	if (opts.dst_addr) {
		ret = ft_tx(ep, remote_fi_addr, opts.transfer_size, &tx_ctx);
		if (ret) {
			FT_PRINTERR("ft_tx", -ret);
			goto out;
		}
	} else {
		ret = ft_rx(ep, opts.transfer_size);
		if (ret) {
			FT_PRINTERR("ft_rx", -ret);
			goto out;
		}
	}

	ret = fi_av_remove(av, &remote_fi_addr, 1, 0);
	if (ret) {
		FT_PRINTERR("fi_av_remove", ret);
		goto out;
	}

	ret = ft_sync();
	if (ret)
		goto out;

	ret = ft_init_av();
	if (ret)
		goto out;

	if (opts.dst_addr) {
		ret = ft_tx(ep, remote_fi_addr, opts.transfer_size, &tx_ctx);
		if (ret) {
			FT_PRINTERR("ft_tx", -ret);
			goto out;
		}
	} else {
		ret = ft_rx(ep, opts.transfer_size);
		if (ret) {
			FT_PRINTERR("ft_rx", -ret);
			goto out;
		}
	}

	(void) ft_sync();
out:
	fprintf(stdout, "%s\n", ret ? "FAIL" : "PASS");
	ft_free_res();
	return ret;
}

/*
 Test flow proposal for directed receive
 client (dst_addr):
	recvfrom
 	remove addr
 	insert addr
 	OOB sync
	recvfrom
 server (else):
 	tsend
 	OOB sync
 	tsend
 */

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;
	opts.options |= FT_OPT_SIZE | FT_OPT_OOB_CTRL;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	hints->ep_attr->type = FI_EP_RDM;

	while ((op = getopt(argc, argv, "h" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "AV communication unit test.");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->caps = hints->ep_attr->type == FI_EP_RDM ?
		      FI_TAGGED : FI_MSG;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;
	base_hints = hints;

	ret = av_removal_test();
	if (ret && ret != -FI_ENODATA)
		goto out;

	if (opts.dst_addr)
		sleep(1);
	ret = av_reinsert_test();
	if (ret && ret != -FI_ENODATA)
		goto out;

out:
	return ft_exit_code(ret);
}

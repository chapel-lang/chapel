/*
 * Copyright (c) 2013-2019 Intel Corporation.  All rights reserved.
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
#include <stdlib.h>
#include <getopt.h>
#include <sys/mman.h>

#include <shared.h>


static int map_tx_buf(void)
{
	/* Try to get the mapping at the same virtual address */
	tx_buf = mmap(tx_buf, opts.transfer_size, PROT_READ | PROT_WRITE,
		      MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (!tx_buf) {
		FT_PRINTERR("mmap", -errno);
		return -errno;
	}
	return 0;
}

static void unmap_tx_buf(void)
{
	int ret;

	ret = madvise(tx_buf, opts.transfer_size, MADV_DONTNEED);
	if (ret)
		FT_PRINTERR("madvise", -errno);

	ret = munmap(tx_buf, opts.transfer_size);
	if (ret)
		FT_PRINTERR("munmap", -errno);
}

static int pingpong(void)
{
	int ret, i;

	ret = ft_sync();
	if (ret)
		return ret;

	ft_start();
	if (opts.dst_addr) {
		for (i = 0; i < opts.iterations; i++) {
			ret = map_tx_buf();
			if (ret)
				return ret;

			ret = ft_tx(ep, remote_fi_addr, opts.transfer_size, &tx_ctx);
			if (ret)
				return ret;

			unmap_tx_buf();
			ret = ft_rx(ep, opts.transfer_size);
			if (ret)
				return ret;
		}
	} else {
		for (i = 0; i < opts.iterations; i++) {
			ret = ft_rx(ep, opts.transfer_size);
			if (ret)
				return ret;

			ret = map_tx_buf();
			if (ret)
				return ret;

			ret = ft_tx(ep, remote_fi_addr, opts.transfer_size, &tx_ctx);
			if (ret)
				return ret;
			unmap_tx_buf();
		}
	}
	ft_stop();

	show_perf(NULL, opts.transfer_size, opts.iterations, &start, &end, 2);
	return 0;
}

static int run(void)
{
	void *saved_tx_buf;
	int i, ret;

	ret = (hints->ep_attr->type == FI_EP_MSG) ?
	      ft_init_fabric_cm() : ft_init_fabric();
	if (ret)
		return ret;

	/* We use our own tx buffer for transfers */
	saved_tx_buf = tx_buf;
	tx_buf = NULL;

	if (!(opts.options & FT_OPT_SIZE)) {
		for (i = 0; i < TEST_CNT; i++) {
			if (!ft_use_size(i, opts.sizes_enabled))
				continue;

			opts.transfer_size = test_size[i].size;
			init_test(&opts, test_name, sizeof(test_name));
			ret = pingpong();
			if (ret)
				return ret;
		}
	} else {
		init_test(&opts, test_name, sizeof(test_name));
		ret = pingpong();
		if (ret)
			return ret;
	}

	tx_buf = saved_tx_buf;
	return ft_finalize();
}

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;
	opts.options |= FT_OPT_VERIFY_DATA | FT_OPT_OOB_SYNC;
	opts.mr_mode &= ~FI_MR_LOCAL;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "h" CS_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parseinfo(op, optarg, hints, &opts);
			ft_parsecsopts(op, optarg, &opts);
			break;
		case '?':
		case 'h':
			ft_usage(argv[0],
				"Changes physical address mappings of data buffers.\n");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->mode = FI_CONTEXT;
	hints->caps = FI_MSG;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = run();

	ft_free_res();
	return ft_exit_code(ret);
}

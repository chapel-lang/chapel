/*
 * Copyright (c) 2013-2019 Intel Corporation.  All rights reserved.
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

#include <rdma/fi_errno.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_cm.h>

#include <shared.h>

#define MAX_XFER_SIZE (1 << 20)

static struct fid_mr *mr_multi_recv;
struct fi_context ctx_multi_recv[2];
static int use_recvmsg, comp_per_buf;

static int repost_recv(int iteration)
{
	struct fi_msg msg;
	struct iovec msg_iov;
	void *buf_addr;
	int ret;

	buf_addr = rx_buf + (rx_size / 2) * iteration;
	if (use_recvmsg) {
		msg_iov.iov_base = buf_addr;
		msg_iov.iov_len = rx_size / 2;
		msg.msg_iov = &msg_iov;
		msg.desc = fi_mr_desc(mr_multi_recv);
		msg.iov_count = 1;
		msg.addr = 0;
		msg.data = NO_CQ_DATA;
		msg.context = &ctx_multi_recv[iteration];
		ret = fi_recvmsg(ep, &msg, FI_MULTI_RECV);
		if (ret) {
			FT_PRINTERR("fi_recvmsg", ret);
			return ret;
		}
	} else {
		ret = fi_recv(ep, buf_addr, rx_size / 2,
			      fi_mr_desc(mr_multi_recv), 0,
			      &ctx_multi_recv[iteration]);
		if (ret) {
			FT_PRINTERR("fi_recv", ret);
			return ret;
		}
	}

	return 0;
}


static int wait_for_recv_completion(int num_completions)
{
	int i, ret, per_buf_cnt = 0;
	struct fi_cq_data_entry comp;

	while (num_completions > 0) {
		ret = fi_cq_read(rxcq, &comp, 1);
		if (ret == -FI_EAGAIN)
			continue;

		if (ret < 0) {
			FT_PRINTERR("fi_cq_read", ret);
			return ret;
		}

		if (comp.flags & FI_RECV) {
			if (comp.len != opts.transfer_size) {
				FT_ERR("completion length %lu, expected %lu",
					comp.len, opts.transfer_size);
				return -FI_EIO;
			}
			if (ft_check_opts(FT_OPT_VERIFY_DATA | FT_OPT_ACTIVE) &&
			    ft_check_buf(comp.buf, opts.transfer_size))
				return -FI_EIO;
			per_buf_cnt++;
			num_completions--;
		}

		if (comp.flags & FI_MULTI_RECV) {
			if (per_buf_cnt != comp_per_buf) {
				FT_ERR("Received %d completions per buffer, expected %d",
					per_buf_cnt, comp_per_buf);
				return -FI_EIO;
			}
			per_buf_cnt = 0;
			i = comp.op_context == &ctx_multi_recv[1];

			ret = repost_recv(i);
			if (ret)
				return ret;
		}
	}
	return 0;
}

/*
 * Post buffer as two halves, so that we can repost one half
 * when the other half is full.
 */
static int post_multi_recv_buffer()
{
	int ret, i;

	for (i = 0; i < 2; i++) {
		ret = repost_recv(i);
		if (ret)
			return ret;
	}

	return 0;
}

static int run_test(void)
{
	int ret, i;

	ret = ft_sync();
	if (ret)
		return ret;

	ft_start();
	if (opts.dst_addr) {
		for (i = 0; i < opts.iterations; i++) {
			ret = ft_tx(ep, remote_fi_addr, opts.transfer_size, &tx_ctx);
			if (ret)
				return ret;
		}
	} else {
		ret = wait_for_recv_completion(opts.iterations);
		if (ret)
			return ret;
	}
	ft_stop();

	if (opts.machr)
		show_perf_mr(opts.transfer_size, opts.iterations,
			&start, &end, 1, opts.argc, opts.argv);
	else
		show_perf(NULL, opts.transfer_size, opts.iterations,
			&start, &end, 1);

	return ret;
}

static void free_res(void)
{
	FT_CLOSE_FID(mr_multi_recv);
	if (tx_buf) {
		free(tx_buf);
		tx_buf = NULL;
	}
	if (rx_buf) {
		free(rx_buf);
		rx_buf = NULL;
	}
}

static int alloc_ep_res(struct fi_info *fi)
{
	int ret;

	tx_size = opts.transfer_size;
	if (tx_size > fi->ep_attr->max_msg_size) {
		fprintf(stderr, "transfer size is larger than the maximum size "
				"of the data transfer supported by the provider\n");
		return -1;
	}

	tx_buf = malloc(tx_size);
	if (!tx_buf) {
		fprintf(stderr, "Cannot allocate tx_buf\n");
		return -1;
	}

	ret = fi_mr_reg(domain, tx_buf, tx_size, FI_SEND,
			0, FT_MR_KEY, 0, &mr, NULL);
	if (ret) {
		FT_PRINTERR("fi_mr_reg", ret);
		return ret;
	}

	/* We only ues the common code to send messages, so
	 * set mr_desc to the tx buffer's region.
	 */
	mr_desc = fi_mr_desc(mr);

	//Each multi recv buffer will be able to hold at least 2 and
	//up to 64 messages, allowing proper testing of multi recv
	//completions and reposting
	rx_size = MIN(tx_size * 128, MAX_XFER_SIZE * 4);
	comp_per_buf = rx_size / 2 / opts.transfer_size;
	rx_buf = malloc(rx_size);
	if (!rx_buf) {
		fprintf(stderr, "Cannot allocate rx_buf\n");
		return -1;
	}

	ret = fi_mr_reg(domain, rx_buf, rx_size, FI_RECV, 0, FT_MR_KEY + 1, 0,
			&mr_multi_recv, NULL);
	if (ret) {
		FT_PRINTERR("fi_mr_reg", ret);
		return ret;
	}

	return 0;
}

static int run(void)
{
	int ret = 0;

	ret = hints->ep_attr->type == FI_EP_MSG ?
		ft_init_fabric_cm() : ft_init_fabric();
	if (ret)
		return ret;

	ret = alloc_ep_res(fi);
	if (ret)
		return ret;

	ret = fi_setopt(&ep->fid, FI_OPT_ENDPOINT, FI_OPT_MIN_MULTI_RECV,
			&tx_size, sizeof(tx_size));
	if (ret)
		return ret;

	ret = post_multi_recv_buffer();
	if (ret)
		return ret;

	ret = run_test();

	rx_seq++;
	ft_finalize();

	return ret;
}

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;
	opts.options |= FT_OPT_SIZE | FT_OPT_SKIP_MSG_ALLOC | FT_OPT_OOB_SYNC |
			FT_OPT_OOB_ADDR_EXCH;
	use_recvmsg = 0;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "Mhv" CS_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parseinfo(op, optarg, hints, &opts);
			ft_parsecsopts(op, optarg, &opts);
			break;
		case 'M':
			use_recvmsg = 1;
			break;
		case 'v':
			opts.options |= FT_OPT_VERIFY_DATA;
			break;
		case '?':
		case 'h':
			ft_csusage(argv[0],
				"Streaming RDM client-server using multi recv buffer.");
			FT_PRINT_OPTS_USAGE("-M", "enable testing with fi_recvmsg");
			FT_PRINT_OPTS_USAGE("-v", "Enable data verification");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	if (opts.transfer_size > MAX_XFER_SIZE) {
		FT_ERR("Use smaller transfer size (max %d)", MAX_XFER_SIZE);
		return EIO;
	}

	hints->caps = FI_MSG | FI_MULTI_RECV;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;
	hints->rx_attr->op_flags = FI_MULTI_RECV;

	cq_attr.format = FI_CQ_FORMAT_DATA;

	ret = run();

	free_res();
	ft_free_res();
	return ft_exit_code(ret);
}

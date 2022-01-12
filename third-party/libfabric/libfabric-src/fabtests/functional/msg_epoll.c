/*
 * Copyright (c) 2013-2015 Intel Corporation.  All rights reserved.
 * Copyright (c) 2016, Cisco Systems, Inc. All rights reserved.
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

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <shared.h>

#if HAVE_EPOLL == 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/epoll.h>

#include <rdma/fi_errno.h>
#include <rdma/fi_cm.h>

static int epfd;

static int alloc_epoll_res(void)
{
	struct epoll_event event;
	int ret, fd;

	epfd = epoll_create1(0);
	if (epfd < 0) {
		ret = -errno;
		FT_PRINTERR("epoll_create1", ret);
		return ret;
	}

	memset((void *) &event, 0, sizeof event);
	if (opts.dst_addr) {
		fd = tx_fd;
		event.data.ptr = (void *) &txcq->fid;
	} else {
		fd = rx_fd;
		event.data.ptr = (void *) &rxcq->fid;
	}

	event.events = EPOLLIN;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
	if (ret) {
		ret = -errno;
		FT_PRINTERR("epoll_ctl", ret);
		return ret;
	}

	return 0;
}

static int send_recv()
{
	struct fi_cq_entry comp;
	struct epoll_event event;
	struct fid *fids[1];
	int ret;
	const char *message = "Hello from Client!";
	size_t message_len = strlen(message) + 1;

	if (opts.dst_addr) {
		fprintf(stdout, "Posting a send...\n");
		if (snprintf(tx_buf, tx_size, "%s", message) >= tx_size) {
			fprintf(stderr, "Transmit buffer too small.\n");
			return -FI_ETOOSMALL;
		}
		ret = ft_post_tx(ep, remote_fi_addr, message_len, NO_CQ_DATA, &tx_ctx);
		if (ret)
			return ret;

		memset(&event, 0, sizeof event);
		fids[0] = &txcq->fid;
		do {
			if (fi_trywait(fabric, fids, 1) == FI_SUCCESS) {
				ret = TEMP_FAILURE_RETRY(epoll_wait(epfd, &event, 1, -1));
				if (ret < 0) {
					ret = -errno;
					FT_PRINTERR("epoll_wait", ret);
					return ret;
				}

				if (event.data.ptr != &txcq->fid)
					fprintf(stdout, "unexpected event!\n");
			}

			ret = fi_cq_read(txcq, &comp, 1);
		} while (ret == -FI_EAGAIN);

		if (ret < 0) {
			if (ret == -FI_EAVAIL)
				ret = ft_cq_readerr(txcq);
			return ret;
		}

		fprintf(stdout, "Send completion received\n");
	} else {
		fprintf(stdout, "Waiting for client...\n");

		memset(&event, 0, sizeof event);
		fids[0] = &rxcq->fid;
		do {
			if (fi_trywait(fabric, fids, 1) == FI_SUCCESS) {
				ret = TEMP_FAILURE_RETRY(epoll_wait(epfd, &event, 1, -1));
				if (ret < 0) {
					ret = -errno;
					FT_PRINTERR("epoll_wait", ret);
					return ret;
				}

				if (event.data.ptr != &rxcq->fid) {
					fprintf(stdout, "unexpected event!\n");
				}
			}

			ret = fi_cq_read(rxcq, &comp, 1);
		} while (ret == -FI_EAGAIN);

		if (ret < 0) {
			if (ret == -FI_EAVAIL)
				ret = ft_cq_readerr(rxcq);
			return ret;
		}

		ret = check_recv_msg(message);
		if (ret)
			return ret;

		fprintf(stdout, "Received data from client: %s\n", (char *) rx_buf);
	}

	return 0;
}

static int run(void)
{
	int ret;

	if (!opts.dst_addr) {
		ret = ft_start_server();
		if (ret)
			return ret;
	}

	ret = opts.dst_addr ? ft_client_connect() : ft_server_connect();
	if (ret) {
		return ret;
	}

	ret = alloc_epoll_res();
	if (ret)
		return ret;

	ret = send_recv();

	fi_shutdown(ep, 0);
	return ret;
}

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;
	opts.options |= FT_OPT_SIZE;
	opts.comp_method = FT_COMP_WAIT_FD;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "h" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "A simple MSG client-sever example that "
				"demonstrates one possible usage of the underlying "
				"cq wait objects.");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->ep_attr->type		= FI_EP_MSG;
	hints->caps			= FI_MSG;
	hints->domain_attr->mr_mode 	= opts.mr_mode;

	ret = run();

	ft_free_res();
	close(epfd);
	return ft_exit_code(ret);
}

#else

#include <rdma/fi_errno.h>

int main(int argc, char **argv)
{
	return ft_exit_code(FI_ENODATA);
}
#endif

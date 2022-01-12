/*
 * Copyright (c) 2018 Intel Corporation.  All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "shared.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <rdma/fi_cm.h>

const char *msg = "hello stream!";


int send_stream(struct fid_ep *ep, const char *msg, size_t msg_len)
{
	int offset, ret;

	for (offset = 0; offset < msg_len; ) {
		ret = fi_send(ep, (msg + offset), (msg_len - offset), NULL, 0, NULL);
		if (ret < 0 && ret != -FI_EAGAIN) {
			fprintf(stderr, "%s error %s\n", __func__, fi_strerror(-ret));
			return ret;
		}

		if (ret > 0)
			offset += ret;
	}

	return offset;
}

int recv_stream(struct fid_ep *ep, char *msg, size_t msg_len)
{
	int offset, ret;

	for (offset = 0; offset < msg_len; ) {
		ret = fi_recv(ep, (msg + offset), (msg_len - offset), NULL, 0, NULL);
		if (ret < 0 && ret != -FI_EAGAIN) {
			fprintf(stderr, "%s error %s\n", __func__, fi_strerror(-ret));
			return ret;
		}
		if (ret > 0)
			offset += ret;
	}

	return offset;
}

static int send_greeting(struct fid_ep *ep)
{
	const size_t msg_len = strlen(msg);
	char buffer[msg_len];
	int ret;

	ret = send_stream(ep, msg, msg_len);
	if (ret < 0)
		return ret;

	ret = recv_stream(ep, buffer, msg_len);
	if (ret < 0)
		return ret;

	if (strncmp(buffer, msg, msg_len) != 0) {
		printf("error recv: %s\n", buffer);
		return -FI_EIO;
	}

	return 0;
}

static int recv_greeting(struct fid_ep *ep)
{
	const size_t msg_len = strlen(msg);
	char buffer[msg_len];
	int ret;

	ret = recv_stream(ep, buffer, msg_len);
	if (ret < 0)
		return ret;

	if (strncmp(buffer, msg, msg_len) != 0) {
		printf("error recv: %s\n", buffer);
		return -FI_EIO;
	}

	ret = send_stream(ep, msg, msg_len);
	if (ret < 0)
		return ret;

	return 0;
}

static int send_recv_greeting(struct fid_ep *ep)
{
	return opts.dst_addr ? recv_greeting(ep) : send_greeting(ep);
}

int stream_init_ep()
{
	int ret = fi_endpoint(domain, fi, &ep, NULL);
	if (ret) {
		FT_PRINTERR("fi_endpoint", ret);
		return ret;
	}

	FT_EP_BIND(ep, eq, 0);

	ret = fi_enable(ep);
	if (ret) {
		FT_PRINTERR("fi_enable", ret);
		return ret;
	}
	return 0;
}

void print_address(struct sockaddr_in *addr)
{
	printf(" accepted IPv4: %s port: %u\n", inet_ntoa(addr->sin_addr),
		ntohs(addr->sin_port));
}

int stream_server_connect(void)
{
	int ret;
	struct sockaddr_in peer_addr;
	size_t addrlen = sizeof(struct sockaddr_in);

	ret = ft_retrieve_conn_req(eq, &fi);
	if (ret)
		goto err;

	ret = fi_domain(fabric, fi, &domain, NULL);
	if (ret) {
		FT_PRINTERR("fi_domain", ret);
		goto err;
	}

	ret = stream_init_ep();
	if (ret)
		goto err;

	ret = ft_accept_connection(ep, eq);
	if (ret)
		goto err;

	ret = fi_getpeer(ep, &peer_addr, &addrlen);
	print_address(&peer_addr);
	return 0;

err:
	return ret;
}


int stream_client_connect()
{
	int ret;

	ret = ft_getinfo(hints, &fi);
	if (ret)
		return ret;

	ret = ft_open_fabric_res();
	if (ret)
		return ret;

	ret = stream_init_ep();
	if (ret)
		return ret;

	ret = ft_connect_ep(ep, eq, fi->dest_addr);
	if (ret)
		return ret;

	return 0;
}

void set_stream_hints(void) {
	hints->ep_attr->type		= FI_EP_SOCK_STREAM;
	hints->caps			= FI_MSG;
	hints->domain_attr->mr_mode	= 0;
	hints->addr_format		= FI_SOCKADDR;
	hints->domain_attr->threading = FI_THREAD_SAFE;
	hints->domain_attr->data_progress = FI_PROGRESS_MANUAL;
	hints->domain_attr->control_progress = FI_PROGRESS_AUTO;
	hints->tx_attr->msg_order = FI_ORDER_SAS;
	hints->rx_attr->msg_order = FI_ORDER_SAS;
}
static int stream_run(void)
{
	int ret;

	if (!opts.dst_addr) {
		ret = ft_start_server();
		if (ret)
			return ret;
	}

	ret = opts.dst_addr ? stream_client_connect() : stream_server_connect();
	if (ret) {
		return ret;
	}

	ret = send_recv_greeting(ep);
	if (ret < 0)
		return ret;

	fi_shutdown(ep, 0);
	return ret;
}

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;
	/* remove CQ usage on ep */
	opts.options = FT_OPT_SIZE;

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
			ft_usage(argv[0], "A simple MSG client-sever example.");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	set_stream_hints();

	ret = stream_run();

	ft_free_res();
	return ft_exit_code(ret);
}

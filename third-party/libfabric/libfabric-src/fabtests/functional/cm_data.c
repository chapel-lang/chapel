/*
 * Copyright (c) 2013-2015 Intel Corporation.  All rights reserved.
 * Copyright (c) 2014-2016, Cisco Systems, Inc. All rights reserved.
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
#include <string.h>

#include <rdma/fi_errno.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_cm.h>

#include "shared.h"

static char *cm_data;
static size_t cm_data_size;
static struct fi_eq_cm_entry *entry;
static struct fi_eq_err_entry err_entry;

char *sock_service = "2710";

static int read_shutdown_event()
{
	int ret;
	uint32_t event;

	memset(entry, 0, sizeof(*entry));
	ret = fi_eq_sread(eq, &event, entry, sizeof(*entry), -1, 0);
	if (ret < 0) {
		FT_PROCESS_EQ_ERR(ret, eq, "fi_eq_sread", "shutdown");
		return ret;
	}
	if (event != FI_SHUTDOWN || entry->fid != &ep->fid) {
		FT_ERR("Unexpected CM event %d fid %p (ep %p)", event,
			entry->fid, ep);
		ret = -FI_EOTHER;
		return ret;
	}
	return 0;
}

static int server_setup(void)
{
	size_t opt_size;
	int ret;

	ret = ft_start_server();
	if (ret)
		return ret;

	/* Get the maximum cm_size supported in all domains */
	opt_size = sizeof(cm_data_size);
	return fi_getopt(&pep->fid, FI_OPT_ENDPOINT, FI_OPT_CM_DATA_SIZE,
		&cm_data_size, &opt_size);
}

static int client_setup(void)
{
	size_t opt_size;
	int ret;

	/* Get fabric info */
	ret = fi_getinfo(FT_FIVERSION, opts.dst_addr, opts.dst_port, 0, hints,
			&fi);
	if (ret) {
		FT_PRINTERR("fi_getinfo", ret);
		return ret;
	}

	ret = ft_open_fabric_res();
	if (ret)
		return ret;

	ret = ft_alloc_active_res(fi);
	if (ret)
		return ret;

	ret = ft_enable_ep_recv();
	if (ret)
		return ret;

	/* Get the maximum cm_size for this domain + endpoint combination */
	opt_size = sizeof(opt_size);
	return fi_getopt(&ep->fid, FI_OPT_ENDPOINT, FI_OPT_CM_DATA_SIZE,
		&cm_data_size, &opt_size);
}

static int server_listen(size_t paramlen)
{
	size_t expected;
	uint32_t event;
	int ret;

	expected = paramlen + sizeof(*entry);
	memset(entry, 0, expected);

	ret = fi_eq_sread(eq, &event, entry, expected, -1, 0);
	if (ret != expected) {
		FT_PROCESS_EQ_ERR(ret, eq, "fi_eq_sread", "listen");
		return ret;
	}

	if (event != FI_CONNREQ) {
		FT_ERR("Unexpected CM event %d", event);
		return -FI_EOTHER;
	}

	ret = ft_check_buf(entry->data, paramlen);
	if (ret)
		return ret;

	fi = entry->info;

	return 0;
}

static int server_reject(size_t paramlen)
{
	int ret;

	ret = server_listen(paramlen);
	if (ret)
		return ret;

	/* Data will appear in error event generated on remote end. */
	ret = ft_fill_buf(cm_data, paramlen);
	if (ret)
		return ret;

	ret = fi_reject(pep, fi->handle, cm_data, paramlen);
	if (ret)
		FT_PRINTERR("fi_reject", ret);

	fi_freeinfo(fi);
	fi = NULL;
	return ret;
}

static int server_accept(size_t paramlen)
{
	uint32_t event;
	int ret;

	ret = server_listen(paramlen);
	if (ret)
		return ret;

	ret = fi_domain(fabric, fi, &domain, NULL);
	if (ret) {
		FT_PRINTERR("fi_domain", ret);
		goto err;
	}

	ret = ft_alloc_active_res(fi);
	if (ret) {
		FT_PRINTERR("alloc_active_res", ret);
		goto err;
	}

	ret = ft_enable_ep_recv();
	if (ret) {
		FT_PRINTERR("init_ep", ret);
		goto err;
	}
	/* Data will appear on accept event on remote end. */
	ret = ft_fill_buf(cm_data, paramlen);
	if (ret)
		return ret;

	/* Accept the incoming connection. Also transitions endpoint to active
	 * state.
	 */
	ret = fi_accept(ep, cm_data, paramlen);
	if (ret) {
		FT_PRINTERR("fi_accept", ret);
		goto err;
	}

	/* Local FI_CONNECTED event does not have data associated. */
	memset(entry, 0, sizeof(*entry));
	ret = fi_eq_sread(eq, &event, entry, sizeof(*entry), -1, 0);
	if (ret != sizeof(*entry)) {
		FT_PROCESS_EQ_ERR(ret, eq, "fi_eq_sread", "accept");
		goto err;
	}

	if (event != FI_CONNECTED || entry->fid != &ep->fid) {
		FT_ERR("Unexpected CM event %d fid %p (ep %p)", event,
				entry->fid, ep);
		ret = -FI_EOTHER;
		goto err;
	}

	fi_shutdown(ep, 0);
	ret = read_shutdown_event();
	if (ret)
		goto err;

	FT_CLOSE_FID(ep);
	FT_CLOSE_FID(rxcq);
	FT_CLOSE_FID(txcq);
	FT_CLOSE_FID(rxcntr);
	FT_CLOSE_FID(txcntr);
	FT_CLOSE_FID(av);
	if (mr != &no_mr)
		FT_CLOSE_FID(mr);
	FT_CLOSE_FID(domain);

	free(buf);
	buf = rx_buf = tx_buf = NULL;
	buf_size = rx_size = tx_size = 0;
	fi_freeinfo(fi);
	fi = NULL;

	return 0;

err:
	fi_reject(pep, fi->handle, NULL, 0);
	return ret;
}

static int server(size_t paramlen)
{
	int ret;

	ret = server_reject(paramlen);
	if (ret)
		return ret;

	return server_accept(paramlen);
}

static int client_connect(size_t paramlen)
{
	int ret;

	ret = ft_fill_buf(cm_data, paramlen);
	if (ret)
		return ret;

	/* Connect to server */
	return fi_connect(ep, fi->dest_addr, cm_data, paramlen);
}

static int client_open_new_ep()
{
	size_t opt_size;
	int ret;

	FT_CLOSE_FID(ep);

	ret = fi_endpoint(domain, fi, &ep, NULL);
	if (ret) {
		FT_PRINTERR("fi_endpoint", ret);
		return ret;
	}

	ret = ft_enable_ep_recv();
	if (ret)
		return ret;

	/* Get the maximum cm_size for this domain + endpoint combination */
	opt_size = sizeof(opt_size);
	return fi_getopt(&ep->fid, FI_OPT_ENDPOINT, FI_OPT_CM_DATA_SIZE,
		&cm_data_size, &opt_size);
}

static int client_expect_reject(size_t paramlen)
{
	uint32_t event;
	int ret;

	ret = client_connect(paramlen);
	if (ret) {
		FT_PRINTERR("fi_connect", ret);
		return ret;
	}

	ret = fi_eq_sread(eq, &event, entry, sizeof(*entry), -1, 0);
	if (ret != -FI_EAVAIL) {
		FT_PROCESS_EQ_ERR(ret, eq, "fi_eq_sread", "connect");
		return ret;
	}

	memset(&err_entry, 0, sizeof(err_entry));
	ret = fi_eq_readerr(eq, &err_entry, 0);
	if (ret != sizeof(err_entry)) {
		FT_EQ_ERR(eq, err_entry, NULL, 0);
		return err_entry.err;
	}

	if (err_entry.err != FI_ECONNREFUSED)
		return err_entry.err;

	/* Check data on FI_ECONNREFUSED error event. */
	return ft_check_buf(err_entry.err_data, err_entry.err_data_size);
}

static int client_expect_accept(size_t paramlen)
{
	size_t expected;
	uint32_t event;
	int ret;

	expected = paramlen + sizeof(*entry);

	ret = client_connect(paramlen);
	if (ret) {
		FT_PRINTERR("fi_connect", ret);
		return ret;
	}

	ret = fi_eq_sread(eq, &event, entry, expected, -1, 0);
	if (ret != expected) {
		FT_PROCESS_EQ_ERR(ret, eq, "fi_eq_sread", "connect");
		return ret;
	}

	if (event != FI_CONNECTED || entry->fid != &ep->fid) {
		FT_ERR("Unexpected CM event %d fid %p (ep %p)", event,
				entry->fid, ep);
		return -FI_EOTHER;
	}

	/* Check data on FI_CONNECTED event. */
	ret = ft_check_buf(entry->data, paramlen);
	if (ret)
		return ret;

	fi_shutdown(ep, 0);
	return read_shutdown_event();
}

/*
 * After each reject and accept of a connection request we close the endpoint and
 * open a new one since fi_connect can be called only once in a connected
 * endpoint's lifetime.
 */
static int client(size_t paramlen)
{
	int ret;

	ret = client_expect_reject(paramlen);
	if (ret)
		return ret;

	ret = client_open_new_ep();
	if (ret)
		return ret;

	ret = client_expect_accept(paramlen);
	if (ret)
		return ret;

	return client_open_new_ep();
}

static int run(void)
{
	char *node, *service;
	uint64_t flags;
	int ret;
	size_t i;

	ret = ft_read_addr_opts(&node, &service, hints, &flags, &opts);
	if (ret)
		return ret;

	ret = opts.dst_addr ? client_setup() : server_setup();
	if (ret) {
		fprintf(stderr, "error: %s\n", fi_strerror(-ret));
		return ret;
	}

	/* Leave extra space for invalid size test */
	cm_data = calloc(1, cm_data_size + 1);
	if (!cm_data)
		return -FI_ENOMEM;

	entry = calloc(1, sizeof(*entry) + cm_data_size);
	if (!entry)
		return -FI_ENOMEM;

	if (opts.dst_addr) {
		ret = ft_sock_connect(opts.dst_addr, sock_service);
		if (ret)
			goto err2;
	} else {
		ret = ft_sock_listen(opts.src_addr, sock_service);
		if (ret)
			goto err2;
		ret = ft_sock_accept();
		if (ret)
			goto err1;
	}

	for (i = 1; i <= cm_data_size; i <<= 1) {
		printf("trying with data size: %zu\n", i);

		if (opts.dst_addr)
			ret = client(i);
		else
			ret = server(i);

		if (ret)
			goto err1;

		ret = ft_sock_sync(0);
		if (ret)
			goto err1;
	}

	/* Despite server not being setup to handle this, the client should fail
	 * with -FI_EINVAL since this exceeds its max data size.
	 */
	if (opts.dst_addr) {
		printf("trying with data size exceeding maximum: %zu\n",
				cm_data_size + 1);
		/* Don't call client since it produces an error message. */
		ret = client_connect(cm_data_size + 1);
		if (ret != -FI_EINVAL) {
			FT_ERR("expected -FI_EINVAL, got: [%d]:%s\n", ret,
				fi_strerror(-ret));
		} else {
			ret = FI_SUCCESS;
		}
	}

err1:
	ft_sock_shutdown(sock);
err2:
	free(entry);
	free(cm_data);
	return ret;
}

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;
	opts.options |= FT_OPT_SIZE | FT_OPT_SKIP_REG_MR | FT_OPT_SKIP_MSG_ALLOC;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, "q:h" ADDR_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		case 'q':
			sock_service = optarg;
			break;
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case '?':
		case 'h':
			ft_usage(argv[0],
					"A MSG client-sever example that uses CM data.");
			FT_PRINT_OPTS_USAGE("-q <service_port>", "management port");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->ep_attr->type	= FI_EP_MSG;
	hints->caps		= FI_MSG;
	hints->domain_attr->mr_mode = opts.mr_mode;

	ret = run();

	ft_free_res();
	return ft_exit_code(ret);
}

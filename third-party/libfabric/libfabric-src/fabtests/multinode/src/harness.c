/*
 * Copyright (c) 2019 Intel Corporation. All rights reserved.
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
#include <errno.h>
#include <shared.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <core.h>
struct pm_job_info pm_job;

static int parse_caps(char* caps)
{
	if (strcmp(caps, "msg") == 0) {
		return multi_msg;
	} else if (strcmp(caps, "rma") == 0) {
		return multi_rma;
	} else {
		printf("Warn: Invalid capability, defaulting to msg\n");
		return multi_msg;
	}
}

static inline ssize_t socket_send(int sock, void *buf, size_t len, int flags)
{
	ssize_t ret;
	size_t m = 0;
	uint8_t *ptr = (uint8_t *) buf;

	do {
		ret = send(sock, (void *) &ptr[m], len-m, flags);
		if (ret < 0)
			return ret;

		m += ret;
	} while (m != len);

	return len;
}

static inline int socket_recv(int sock, void *buf, size_t len, int flags)
{
	ssize_t ret;
	size_t m = 0;
	uint8_t *ptr = (uint8_t *) buf;

	do {
		ret = recv(sock, (void *) &ptr[m], len-m, flags);
		if (ret <= 0)
			return -1;

		m += ret;
	} while (m < len);

	return len;
}

int pm_allgather(void *my_item, void *items, int item_size)
{
	int i, ret;
	uint8_t *offset;

	/* client */
	if (!pm_job.clients) {
		ret = socket_send(pm_job.sock, my_item, item_size, 0);
		if (ret < 0)
			return errno == EPIPE ? -FI_ENOTCONN : -errno;

		ret = socket_recv(pm_job.sock, items,
				  pm_job.num_ranks*item_size, 0);
		if (ret <= 0)
			return (ret)? -errno : -FI_ENOTCONN;

		return 0;
	}

	/* server */
	memcpy(items, my_item, item_size);

	for (i = 0; i < pm_job.num_ranks-1; i++) {
		offset = (uint8_t *)items + item_size * (i+1);

		ret = socket_recv(pm_job.clients[i], (void *)offset,
				  item_size, 0);
		if (ret <= 0)
			return ret;
	}

	for (i = 0; i < pm_job.num_ranks-1; i++) {
		ret = socket_send(pm_job.clients[i], items,
				  pm_job.num_ranks*item_size, 0);
		if (ret < 0)
			return ret;
	}
	return 0;
}

void pm_barrier()
{
	char ch;
	char chs[pm_job.num_ranks];

	pm_allgather(&ch, chs, 1);
}

static int pm_init_ranks()
{
	int ret;
	int i;
	size_t send_rank;

	if (pm_job.clients) {
		for(i = 0; i < pm_job.num_ranks-1; i++) {
			send_rank = i + 1;
			ret = socket_send(pm_job.clients[i], &send_rank, sizeof(send_rank), 0);
			if (ret < 0)
				return ret;
		}
	} else {
		ret = socket_recv(pm_job.sock, &(pm_job.my_rank), sizeof(pm_job.my_rank), 0);
	}

	return ret;
}

static int server_connect()
{
	int new_sock;
	int ret, i;

	ret = listen(pm_job.sock, pm_job.num_ranks);
	if (ret)
		return ret;

	pm_job.clients = calloc(pm_job.num_ranks, sizeof(int));
	if (!pm_job.clients)
		return -FI_ENOMEM;

	for (i = 0; i < pm_job.num_ranks-1; i++) {
		new_sock = accept(pm_job.sock, NULL, NULL);
		if (new_sock < 0) {
			FT_ERR("error during server init\n");
			goto err;
		}
		pm_job.clients[i] = new_sock;
		FT_DEBUG("connection established\n");
	}
	close(pm_job.sock);
	return 0;
err:
	while (i--) {
		close(pm_job.clients[i]);
	}
	free(pm_job.clients);
	return new_sock;
}

static int pm_conn_setup()
{
	int sock,  ret;
	int optval = 1;

	sock = socket(pm_job.oob_server_addr.ss_family, SOCK_STREAM, 0);
	if (sock < 0)
		return -1;

	pm_job.sock = sock;

	ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &optval,
			 sizeof(optval));
	if (ret) {
		FT_ERR("error setting socket options\n");
		return ret;
	}

	ret = bind(sock, (struct sockaddr *)&pm_job.oob_server_addr,
		  pm_job.server_addr_len);
	if (ret == 0) {
		ret = server_connect();
	} else {
		opts.dst_addr = opts.src_addr;
		opts.dst_port = opts.src_port;
		opts.src_addr = NULL;
		opts.src_port = 0;
		ret = connect(pm_job.sock, (struct sockaddr *)&pm_job.oob_server_addr,
			      pm_job.server_addr_len);
	}
	if (ret) {
		FT_ERR("OOB conn failed - %s\n", strerror(errno));
		return ret;
	}

	return 0;
}

static void pm_finalize()
{
	int i;

	if (!pm_job.clients) {
		close(pm_job.sock);
		return;
	}

	for (i = 0; i < pm_job.num_ranks-1; i++) {
		close(pm_job.clients[i]);
	}
	free(pm_job.clients);
}

int pm_get_oob_server_addr()
{
	struct addrinfo *res;
	struct sockaddr_in *in;
	struct sockaddr_in6 *in6;
	int ret;

	ret = getaddrinfo(opts.src_addr, NULL, NULL, &res);
	if (ret) {
		FT_ERR( "getaddrinfo failed\n");
		return ret;
	}

	memcpy(&pm_job.oob_server_addr, res->ai_addr, res->ai_addrlen);
	pm_job.server_addr_len = res->ai_addrlen;

	switch (pm_job.oob_server_addr.ss_family) {
	case AF_INET:
		in = (struct sockaddr_in *) &pm_job.oob_server_addr;
		in->sin_port = PM_DEFAULT_OOB_PORT;
		break;
	case AF_INET6:
		in6 = (struct sockaddr_in6 *) &pm_job.oob_server_addr;
		in6->sin6_port = PM_DEFAULT_OOB_PORT;
		break;
	default:
		FT_ERR( "Unsupported Address family\n");
		ret = -1;
		break;
	}

	freeaddrinfo(res);
	return ret;
}

int main(int argc, char **argv)
{
	extern char *optarg;
	int c, ret;

	opts = INIT_OPTS;
	opts.options |= FT_OPT_SIZE;

	pm_job.clients = NULL;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((c = getopt(argc, argv, "n:C:h" CS_OPTS INFO_OPTS)) != -1) {
		switch (c) {
		default:
			ft_parse_addr_opts(c, optarg, &opts);
			ft_parseinfo(c, optarg, hints, &opts);
			ft_parsecsopts(c, optarg, &opts);
			break;
		case 'n':
			pm_job.num_ranks = atoi(optarg);
			break;
		case 'C':
			pm_job.transfer_method = parse_caps(optarg);
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "A simple multinode test");
			return EXIT_FAILURE;
		}
	}

	ret = pm_get_oob_server_addr();
	if (ret)
		goto err1;

	ret = pm_conn_setup();
	if (ret) {
		FT_ERR("connection setup failed\n");
		goto err1;
	}
	
	ret = pm_init_ranks();
	if (ret < 0) {
		FT_ERR("rank initialization failed\n");
		goto err2;
	}

	FT_DEBUG("OOB job setup done\n");

	ret = multinode_run_tests(argc, argv);
	if (ret) {
		FT_ERR( "Tests failed\n");
		goto err2;
	}
	FT_DEBUG("Tests Passed\n");
err2:
	pm_finalize();
err1:
	return ret;
}

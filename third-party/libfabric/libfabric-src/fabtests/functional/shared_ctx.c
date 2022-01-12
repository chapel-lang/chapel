/*
 * Copyright (c) 2013-2015 Intel Corporation.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
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
#include <string.h>
#include <getopt.h>

#include <rdma/fi_errno.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_cm.h>

#include <shared.h>

enum {
	FT_UNSPEC,
	FT_EP_CNT,
};

enum ft_ep_state {
	FT_EP_STATE_INIT,
	FT_EP_CONNECT_RCVD,
	FT_EP_CONNECTING,
	FT_EP_CONNECTED,
};

struct ep_info {
	struct fid_ep *ep;
	struct fi_info *fi;
	enum ft_ep_state state;
};

static struct fi_info *fi_dup;
static int tx_shared_ctx = 1;
static int rx_shared_ctx = 1;
static int ep_cnt = 4;
static struct fid_ep **ep_array, *srx_ctx;
static struct fid_stx *stx_ctx;
static size_t addrlen = 0;
static fi_addr_t *addr_array;

static int get_dupinfo(void)
{
	struct fi_info *hints_dup;
	int ret;

       /* Get a fi_info corresponding to a wild card port. The first endpoint
	* should use default/given port since that is what is known to both
	* client and server. For other endpoints we should use addresses with
	* random ports to avoid collision. fi_getinfo should return a random
	* port if we don't specify it in the service arg or the hints. This
	* is used only for non-MSG endpoints. */

	hints_dup = fi_dupinfo(hints);
	if (!hints_dup)
		return -FI_ENOMEM;

	free(hints_dup->src_addr);
	free(hints_dup->dest_addr);
	hints_dup->src_addr = NULL;
	hints_dup->dest_addr = NULL;
	hints_dup->src_addrlen = 0;
	hints_dup->dest_addrlen = 0;

	if (opts.dst_addr) {
		ret = fi_getinfo(FT_FIVERSION, opts.dst_addr, NULL, 0,
				 hints_dup, &fi_dup);
	} else {
		ret = fi_getinfo(FT_FIVERSION, opts.src_addr, NULL, FI_SOURCE,
				 hints_dup, &fi_dup);
	}
	if (ret)
		FT_PRINTERR("fi_getinfo", ret);
	fi_freeinfo(hints_dup);
	return ret;
}

static int alloc_ep(void)
{
	int i, ret;

	ep_array = calloc(ep_cnt, sizeof(*ep_array));
	if (!ep_array)
		return -FI_ENOMEM;

	ret = fi_endpoint(domain, fi, &ep_array[0], NULL);
	if (ret) {
		FT_PRINTERR("fi_endpoint", ret);
		return ret;
	}

	for (i = 1; i < ep_cnt; i++) {
		if (hints->ep_attr->type == FI_EP_MSG)
			ret = fi_endpoint(domain, fi, &ep_array[i], NULL);
		else
			ret = fi_endpoint(domain, fi_dup, &ep_array[i], NULL);
		if (ret) {
			FT_PRINTERR("fi_endpoint", ret);
			return ret;
		}
	}

	return 0;
}

static int alloc_ep_res(struct fi_info *fi)
{
	int ret;

	ret = ft_alloc_ep_res(fi);
	if (ret)
		return ret;

	if (tx_shared_ctx) {
		ret = fi_stx_context(domain, fi->tx_attr, &stx_ctx, NULL);
		if (ret) {
			FT_PRINTERR("fi_stx_context", ret);
			return ret;
		}
	}

	if (rx_shared_ctx) {
		ret = fi_srx_context(domain, fi->rx_attr, &srx_ctx, NULL);
		if (ret) {
			FT_PRINTERR("fi_srx_context", ret);
			return ret;
		}
	}
	return 0;
}

static int bind_ep_res(struct fid_ep *ep)
{
	int ret;

	if (hints->ep_attr->type == FI_EP_MSG)
		FT_EP_BIND(ep, eq, 0);

	if (tx_shared_ctx)
		FT_EP_BIND(ep, stx_ctx, 0);

	if (rx_shared_ctx)
		FT_EP_BIND(ep, srx_ctx, 0);

	FT_EP_BIND(ep, txcq, FI_SEND);
	FT_EP_BIND(ep, rxcq, FI_RECV);
	FT_EP_BIND(ep, av, 0);

	ret = fi_enable(ep);
	if (ret) {
		FT_PRINTERR("fi_enable", ret);
		return ret;
	}
	return 0;
}

static int bind_ep_array_res(void)
{
	int i, ret;
	for (i = 0; i < ep_cnt; i++) {
		ret = bind_ep_res(ep_array[i]);
		if (ret)
			return ret;
	}
	return 0;
}

static int run_test()
{
	int ret, i;

	/* Post recvs */
	for (i = 0; i < ep_cnt; i++) {
		if (rx_shared_ctx) {
			fprintf(stdout, "Posting recv #%d for shared rx ctx\n", i);
			ret = ft_post_rx(srx_ctx, rx_size, &rx_ctx_arr[i].context);
		 } else {
			fprintf(stdout, "Posting recv for endpoint #%d\n", i);
			ret = ft_post_rx(ep_array[i], rx_size, &rx_ctx_arr[i].context);
		 }
		if (ret)
			return ret;
	}

	if (opts.dst_addr) {
		/* Post sends addressed to remote EPs */
		for (i = 0; i < ep_cnt; i++) {
			if (tx_shared_ctx)
				fprintf(stdout, "Posting send #%d to shared tx ctx\n", i);
			else
				fprintf(stdout, "Posting send to endpoint #%d\n", i);
			ret = ft_tx(ep_array[i], addr_array[i], tx_size, &tx_ctx_arr[i].context);
			if (ret)
				return ret;
		}
	}

	/* Wait for recv completions */
	ret = ft_get_rx_comp(rx_seq - 1);
	if (ret)
		return ret;

	if (!opts.dst_addr) {
		/* Post sends addressed to remote EPs */
		for (i = 0; i < ep_cnt; i++) {
			if (tx_shared_ctx)
				fprintf(stdout, "Posting send #%d to shared tx ctx\n", i);
			else
				fprintf(stdout, "Posting send to endpoint #%d\n", i);
			ret = ft_tx(ep_array[i], addr_array[i], tx_size, &tx_ctx_arr[i].context);
			if (ret)
				return ret;
		}
	}

	return 0;
}

static int init_av(void)
{
	int ret;
	int i;

	if (opts.dst_addr) {
		ret = ft_av_insert(av, fi->dest_addr, 1, &addr_array[0], 0, NULL);
		if (ret)
			return ret;
	}

	for (i = 0; i < ep_cnt; i++) {
		addrlen = tx_size;
		ret = fi_getname(&ep_array[i]->fid, tx_buf + ft_tx_prefix_size(),
				 &addrlen);
		if (ret) {
			FT_PRINTERR("fi_getname", ret);
			return ret;
		}

		if (opts.dst_addr) {
			ret = ft_tx(ep_array[0], addr_array[0], addrlen, &tx_ctx);
			if (ret)
				return ret;

			if (rx_shared_ctx)
				ret = ft_rx(srx_ctx, rx_size);
			else
				ret = ft_rx(ep_array[0], rx_size);
			if (ret)
				return ret;

			/* Skip the first address since we already have it in AV */
			if (i) {
				ret = ft_av_insert(av, rx_buf + ft_rx_prefix_size(), 1,
						   &addr_array[i], 0, NULL);
				if (ret)
					return ret;
			}
		} else {
			if (rx_shared_ctx)
				ret = ft_rx(srx_ctx, rx_size);
			else
				ret = ft_rx(ep_array[0], rx_size);
			if (ret)
				return ret;

			ret = ft_av_insert(av, rx_buf + ft_rx_prefix_size(), 1,
					   &addr_array[i], 0, NULL);
			if (ret)
				return ret;

			ret = ft_tx(ep_array[0], addr_array[0], addrlen, &tx_ctx);
			if (ret)
				return ret;

		}
	}

	/* ACK */
	if (opts.dst_addr) {
		ret = ft_tx(ep_array[0], addr_array[0], 1, &tx_ctx);
	} else {
		if (rx_shared_ctx)
			ret = ft_rx(srx_ctx, rx_size);
		else
			ret = ft_rx(ep_array[0], rx_size);
	}

	return ret;
}

static int init_fabric(void)
{
	int ret;

	ret = ft_getinfo(hints, &fi);
	if (ret)
		return ret;

	ret = get_dupinfo();
	if (ret)
		return ret;

	ret = ft_open_fabric_res();
	if (ret)
		return ret;

	av_attr.count = ep_cnt;

	ret = alloc_ep_res(fi);
	if (ret)
		return ret;

	ret = alloc_ep();
	if (ret)
		return ret;

	ret = bind_ep_array_res();
	if (ret)
		return ret;

	/* Post recv */
	if (rx_shared_ctx)
		ret = ft_post_rx(srx_ctx, MAX(rx_size, FT_MAX_CTRL_MSG), &rx_ctx);
	else
		ret = ft_post_rx(ep_array[0], MAX(rx_size, FT_MAX_CTRL_MSG), &rx_ctx);
	if (ret)
		return ret;

	ret = init_av();
	return ret;
}

static int client_connect(void)
{
	struct fi_eq_cm_entry entry;
	uint32_t event;
	ssize_t rd;
	int i, ret;

	ret = ft_getinfo(hints, &fi);
	if (ret)
		return ret;

	ret = get_dupinfo();
	if (ret)
		return ret;

	ret = ft_open_fabric_res();
	if (ret)
		return ret;

	ret = alloc_ep_res(fi);
	if (ret)
		return ret;

	ret = alloc_ep();
	if (ret)
		return ret;

	ret = bind_ep_array_res();
	if (ret)
		return ret;

	for (i = 0; i < ep_cnt; i++) {
		ret = fi_connect(ep_array[i], fi->dest_addr, NULL, 0);
		if (ret) {
			FT_PRINTERR("fi_connect", ret);
			return ret;
		}

		rd = fi_eq_sread(eq, &event, &entry, sizeof entry, -1, 0);
		if (rd != sizeof entry) {
			FT_PROCESS_EQ_ERR(rd, eq, "fi_eq_sread", "connect");
			ret = (int) rd;
			return ret;
		}

		if (event != FI_CONNECTED || entry.fid != &ep_array[i]->fid) {
			fprintf(stderr, "Unexpected CM event %d fid %p (ep %p)\n",
				event, entry.fid, ep);
			ret = -FI_EOTHER;
			return ret;
		}
	}

	/* Post recv */
	if (rx_shared_ctx)
		ret = ft_post_rx(srx_ctx, MAX(rx_size, FT_MAX_CTRL_MSG), &rx_ctx);
	else
		ret = ft_post_rx(ep_array[0], MAX(rx_size, FT_MAX_CTRL_MSG), &rx_ctx);
	if (ret)
		return ret;

	return 0;
}

static int server_connect(void)
{
	struct fi_eq_cm_entry entry;
	uint32_t event;
	ssize_t rd;
	int ret, k;
	int num_conn_reqs = 0, num_connected = 0;
	struct ep_info *ep_state_array = NULL;

	ep_array = calloc(ep_cnt, sizeof(*ep_array));
	if (!ep_array)
		return -FI_ENOMEM;

	ep_state_array = calloc(ep_cnt, sizeof(*ep_state_array));
	if (!ep_state_array)
		return -FI_ENOMEM;

	while (num_connected != ep_cnt) {
		rd = fi_eq_sread(eq, &event, &entry, sizeof entry, -1, 0);
		if (rd != sizeof entry) {
			FT_PROCESS_EQ_ERR(rd, eq, "fi_eq_sread", "cm-event");
			ret = (int) rd;
			goto err;
		}

		switch(event) {
		case FI_CONNREQ:
			if (num_conn_reqs == ep_cnt) {
				fprintf(stderr, "Unexpected CM event %d\n", event);
				ret = -FI_EOTHER;
				goto err;
			}
			fi = ep_state_array[num_conn_reqs].fi = entry.info;
			ep_state_array[num_conn_reqs].state = FT_EP_CONNECT_RCVD;

			if (num_conn_reqs == 0) {
				ret = fi_domain(fabric, fi, &domain, NULL);
				if (ret) {
					FT_PRINTERR("fi_domain", ret);
					goto err;
				}

				ret = alloc_ep_res(fi);
				if (ret)
					goto err;
			}

			ret = fi_endpoint(domain, fi, &ep_array[num_conn_reqs], NULL);
			if (ret) {
				FT_PRINTERR("fi_endpoint", ret);
				goto err;
			}

			ep_state_array[num_conn_reqs].ep = ep_array[num_conn_reqs];
			ret = bind_ep_res(ep_array[num_conn_reqs]);
			if (ret)
				goto err;

			ret = fi_accept(ep_array[num_conn_reqs], NULL, 0);
			if (ret) {
				FT_PRINTERR("fi_accept", ret);
				goto err;
			}

			ep_state_array[num_conn_reqs].state = FT_EP_CONNECTING;
			num_conn_reqs++;
			break;

		case FI_CONNECTED:
			if (num_conn_reqs <= num_connected) {
				ret = -FI_EOTHER;
				goto err;
			}

			for (k = 0; k < num_conn_reqs; k++) {
				if (ep_state_array[k].state != FT_EP_CONNECTING)
					continue;
				if (&ep_state_array[k].ep->fid == entry.fid) {
					ep_state_array[k].state = FT_EP_CONNECTED;
					num_connected++;
					if (num_connected != ep_cnt)
						fi_freeinfo(ep_state_array[k].fi);
					break;
				}
			}

			if (k == num_conn_reqs) {
				fprintf(stderr, "Unexpected CM event %d fid %p (ep %p)\n",
					event, entry.fid, ep);
				ret = -FI_EOTHER;
				goto err;
			}
			break;

		default:
			ret = -FI_EOTHER;
			goto err;
		}
	}

	/* Post recv */
	if (rx_shared_ctx)
		ret = ft_post_rx(srx_ctx, MAX(rx_size, FT_MAX_CTRL_MSG), &rx_ctx);
	else
		ret = ft_post_rx(ep_array[0], MAX(rx_size, FT_MAX_CTRL_MSG), &rx_ctx);
	if (ret)
		goto err;

	free(ep_state_array);
	return 0;
err:
	for (k = 0; k < ep_cnt; k++) {
		switch(ep_state_array[k].state) {
		case FT_EP_CONNECT_RCVD:
			fi_reject(pep, ep_state_array[k].fi->handle, NULL, 0);
			break;

		case FT_EP_CONNECTING:
		case FT_EP_CONNECTED:
			fi_shutdown(ep_state_array[k].ep, 0);
			break;

		case FT_EP_STATE_INIT:
		default:
			break;
		}
	}

	free(ep_state_array);
	return ret;
}

static int run(void)
{
	int ret = 0;

	addr_array = calloc(ep_cnt, sizeof(*addr_array));
	if (!addr_array) {
		perror("malloc");
		return -FI_ENOMEM;
	}

	if (hints->ep_attr->type == FI_EP_MSG) {
		if (!opts.dst_addr) {
			ret = ft_start_server();
			if (ret)
				return ret;
		}

		ret = opts.dst_addr ? client_connect() : server_connect();
	} else {
		ret = init_fabric();
	}
	if (ret)
		return ret;

	ret = run_test();

	/* TODO: Add a local finalize applicable to shared ctx */
	//ft_finalize(fi, ep_array[0], txcq, rxcq, addr_array[0]);
	return ret;
}

int main(int argc, char **argv)
{
	int op, ret;
	int option_index = 0;

	struct option long_options[] = {
		{"no-tx-shared-ctx", no_argument, &tx_shared_ctx, 0},
		{"no-rx-shared-ctx", no_argument, &rx_shared_ctx, 0},
		{"ep-count", required_argument, 0, FT_EP_CNT},
		{0, 0, 0, 0},
	};

	opts = INIT_OPTS;
	opts.options |= FT_OPT_SIZE;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt_long(argc, argv, "h" ADDR_OPTS INFO_OPTS,
					long_options, &option_index)) != -1) {
		switch (op) {
		case FT_EP_CNT:
			ep_cnt = atoi(optarg);
			if (ep_cnt <= 0) {
				FT_ERR("ep_count needs to be greater than 0\n");
				return EXIT_FAILURE;
			}
			hints->domain_attr->ep_cnt = ep_cnt;
			break;
		default:
			ft_parse_addr_opts(op, optarg, &opts);
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case '?':
		case 'h':
			ft_usage(argv[0], "An RDM client-server example that uses"
				       " shared context.\n");
			FT_PRINT_OPTS_USAGE("--no-tx-shared-ctx",
					"Disable shared context for TX");
			FT_PRINT_OPTS_USAGE("--no-rx-shared-ctx",
					"Disable shared context for RX");
			FT_PRINT_OPTS_USAGE("--ep-count <count> (default: 4)",
					"# of endpoints to be opened");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	hints->caps = FI_MSG;
	hints->mode = FI_CONTEXT;
	hints->domain_attr->mr_mode = opts.mr_mode;

	if (tx_shared_ctx)
		hints->ep_attr->tx_ctx_cnt = FI_SHARED_CONTEXT;
	if (rx_shared_ctx)
		hints->ep_attr->rx_ctx_cnt = FI_SHARED_CONTEXT;

	ret = run();

	FT_CLOSEV_FID(ep_array, ep_cnt);
	if (rx_shared_ctx)
		FT_CLOSE_FID(srx_ctx);
	if (tx_shared_ctx)
		FT_CLOSE_FID(stx_ctx);
	ft_free_res();
	free(addr_array);
	free(ep_array);
	fi_freeinfo(fi_dup);
	return ft_exit_code(ret);
}

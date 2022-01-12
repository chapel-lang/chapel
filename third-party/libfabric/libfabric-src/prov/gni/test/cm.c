/*
 * Copyright (c) 2016-2017 Cray Inc. All rights reserved.
 * Copyright (c) 2019-2020 Triad National Security, LLC.
 *                         All rights reserved.
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
#include <errno.h>
#include <getopt.h>
#include <poll.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>

#include "gnix_vc.h"
#include "gnix_cm_nic.h"
#include "gnix_hashtable.h"
#include "gnix_atomic.h"
#include "gnix_cm.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"

#include "common.h"

#if 1
#define dbg_printf(...)
#else
#define dbg_printf(...)				\
	do {					\
		printf(__VA_ARGS__);		\
		fflush(stdout);			\
	} while (0)
#endif

#define NUMEPS 2

#define DEF_PORT "1973"

#define EQE_SIZE	(sizeof(struct fi_eq_cm_entry) + GNIX_CM_DATA_MAX_SIZE)


static struct fid_fabric *cli_fab;
static struct fid_domain *cli_dom;
static struct fid_ep *cli_ep;
static struct fi_info *cli_hints;
static struct fi_info *cli_fi;
static struct fid_eq *cli_eq;
static struct fid_cq *cli_cq;
static char *cli_cm_in_data = "Hola.  Soy cliente.";

static struct fid_fabric *srv_fab;
static struct fid_domain *srv_dom;
static struct fid_pep *srv_pep;
static struct fid_ep *srv_ep;
static struct fi_info *srv_hints;
static struct fi_info *srv_fi;
static struct fid_eq *srv_eq;
static struct fid_cq *srv_cq;
static char *srv_cm_in_data = "Este es servidor.";

struct fi_eq_attr eq_attr = {
	.wait_obj = FI_WAIT_UNSPEC
};

struct fi_cq_attr cq_attr = {
	.wait_obj = FI_WAIT_NONE
};

int cm_local_ip(struct sockaddr_in *sa)
{
	struct ifaddrs *ifap;
	struct ifaddrs *ifa;
	int ret = -1;

	ofi_getifaddrs(&ifap);

	ifa = ifap;
	while (ifa) {
		dbg_printf("IF: %s, IP ADDR: %s\n",
			   ifa->ifa_name,
			   inet_ntoa(((struct sockaddr_in *)
				      (ifa->ifa_addr))->sin_addr));
		/* Return first non loopback interface. */
		if (ifa->ifa_addr &&
		    ifa->ifa_addr->sa_family == AF_INET &&
		    !ofi_is_loopback_addr(ifa->ifa_addr)) {
			ret = 0;
			break;
		}
		ifa = ifa->ifa_next;
	}

	if (!ret) {
		memcpy((void *)sa, (void *)ifa->ifa_addr,
		       sizeof(struct sockaddr));
	}

	freeifaddrs(ifap);

	return ret;
}

int cm_server_start(void)
{
	int ret;
	struct sockaddr_in loc_sa;

	cm_local_ip(&loc_sa);

	srv_hints = fi_allocinfo();
	srv_hints->fabric_attr->name = strdup("gni");
	srv_hints->ep_attr->type = FI_EP_MSG;
	srv_hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;

	ret = fi_getinfo(fi_version(), inet_ntoa(loc_sa.sin_addr),
			 DEF_PORT, FI_SOURCE, srv_hints, &srv_fi);
	cr_assert(!ret);

	ret = fi_fabric(srv_fi->fabric_attr, &srv_fab, NULL);
	cr_assert(!ret);

	ret = fi_eq_open(srv_fab, &eq_attr, &srv_eq, NULL);
	cr_assert(!ret);

	ret = fi_passive_ep(srv_fab, srv_fi, &srv_pep, NULL);
	cr_assert(!ret);

	ret = fi_pep_bind(srv_pep, &srv_eq->fid, 0);
	cr_assert(!ret);

	ret = fi_listen(srv_pep);
	cr_assert(!ret);

	dbg_printf("Server start complete.\n");

	return 0;
}

void cm_stop_server(void)
{
	int ret;

	ret = fi_close(&srv_cq->fid);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = fi_close(&srv_ep->fid);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = fi_close(&srv_dom->fid);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = fi_close(&srv_pep->fid);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = fi_close(&srv_eq->fid);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = fi_close(&srv_fab->fid);
	cr_assert_eq(ret, FI_SUCCESS);

	fi_freeinfo(srv_fi);
}

int cm_server_accept(void)
{
	uint32_t event;
	ssize_t rd;
	int ret;
	struct fi_eq_cm_entry *entry;
	void *eqe_buf[EQE_SIZE] = {0};

	rd = fi_eq_sread(srv_eq, &event, &eqe_buf, EQE_SIZE, -1, 0);
	cr_assert(rd == (sizeof(*entry) + strlen(cli_cm_in_data)));
	cr_assert(event == FI_CONNREQ);

	entry = (struct fi_eq_cm_entry *)eqe_buf;
	cr_assert(!memcmp(cli_cm_in_data, entry->data,
			  strlen(cli_cm_in_data)));

	ret = fi_domain(srv_fab, entry->info, &srv_dom, NULL);
	cr_assert(!ret);

	ret = fi_endpoint(srv_dom, entry->info, &srv_ep, NULL);
	cr_assert(!ret, "fi_endpoint");

	fi_freeinfo(entry->info);

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	ret = fi_cq_open(srv_dom, &cq_attr, &srv_cq, &srv_cq);
	cr_assert(!ret);

	ret = fi_ep_bind(srv_ep, &srv_eq->fid, 0);
	cr_assert(!ret);

	ret = fi_ep_bind(srv_ep, &srv_cq->fid, FI_SEND | FI_RECV);
	cr_assert(!ret);

	ret = fi_enable(srv_ep);
	cr_assert(!ret);

	ret = fi_accept(srv_ep, srv_cm_in_data, GNIX_CM_DATA_MAX_SIZE+1);
	cr_assert(ret == -FI_EINVAL);

	ret = fi_accept(srv_ep, srv_cm_in_data, strlen(srv_cm_in_data));
	cr_assert(!ret);

	dbg_printf("Server accept complete.\n");

	return 0;
}

int cm_server_finish_connect(void)
{
	uint32_t event;
	struct fi_eq_cm_entry entry;
	ssize_t rd;

	rd = fi_eq_read(srv_eq, &event, &entry, sizeof(entry), 0);
	if (rd > 0) {
		dbg_printf("got event: %d\n", event);
		cr_assert(rd == sizeof(entry));
		cr_assert(event == FI_CONNECTED && entry.fid == &srv_ep->fid);
		return 1;
	}

	return 0;
}

int cm_client_start_connect(void)
{
	int ret;
	struct sockaddr_in loc_sa;

	cm_local_ip(&loc_sa);

	cli_hints = fi_allocinfo();
	cli_hints->fabric_attr->name = strdup("gni");
	cli_hints->caps = GNIX_EP_PRIMARY_CAPS;
	cli_hints->ep_attr->type = FI_EP_MSG;
	cli_hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;

	ret = fi_getinfo(fi_version(), inet_ntoa(loc_sa.sin_addr),
			 DEF_PORT, 0, cli_hints, &cli_fi);
	cr_assert(!ret);

	ret = fi_fabric(cli_fi->fabric_attr, &cli_fab, NULL);
	cr_assert(!ret);

	ret = fi_eq_open(cli_fab, &eq_attr, &cli_eq, NULL);
	cr_assert(!ret);

	ret = fi_domain(cli_fab, cli_fi, &cli_dom, NULL);
	cr_assert(!ret);

	ret = fi_endpoint(cli_dom, cli_fi, &cli_ep, NULL);
	cr_assert(!ret, "fi_endpoint");

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	ret = fi_cq_open(cli_dom, &cq_attr, &cli_cq, &cli_cq);
	cr_assert(!ret);

	ret = fi_ep_bind(cli_ep, &cli_eq->fid, 0);
	cr_assert(!ret);

	ret = fi_ep_bind(cli_ep, &cli_cq->fid, FI_SEND | FI_RECV);
	cr_assert(!ret);

	ret = fi_enable(cli_ep);
	cr_assert(!ret);

	ret = fi_connect(cli_ep, cli_fi->dest_addr, cli_cm_in_data,
			 GNIX_CM_DATA_MAX_SIZE+1);
	cr_assert(ret == -FI_EINVAL);

	ret = fi_connect(cli_ep, cli_fi->dest_addr, cli_cm_in_data,
			 strlen(cli_cm_in_data));
	cr_assert(!ret);

	dbg_printf("Client connect complete.\n");

	return 0;
}

int cm_client_finish_connect(void)
{
	uint32_t event;
	ssize_t rd;
	struct fi_eq_cm_entry *entry;
	void *eqe_buf[EQE_SIZE] = {0};

	rd = fi_eq_read(cli_eq, &event, eqe_buf, EQE_SIZE, 0);
	if (rd > 0) {
		dbg_printf("got event: %d\n", event);
		entry = (struct fi_eq_cm_entry *)eqe_buf;
		cr_assert(rd == (sizeof(*entry) + strlen(srv_cm_in_data)));
		cr_assert(event == FI_CONNECTED && entry->fid == &cli_ep->fid);
		cr_assert(!memcmp(srv_cm_in_data, entry->data,
				  strlen(srv_cm_in_data)));
		return 1;
	}

	return 0;
}

void cm_stop_client(void)
{
	int ret;

	ret = fi_close(&cli_cq->fid);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = fi_close(&cli_ep->fid);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = fi_close(&cli_dom->fid);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = fi_close(&cli_eq->fid);
	cr_assert_eq(ret, FI_SUCCESS);

	ret = fi_close(&cli_fab->fid);
	cr_assert_eq(ret, FI_SUCCESS);

	fi_freeinfo(cli_fi);
}

void cm_basic_send(int trunc)
{
	int ret,i;
	int slen = 8, rlen;
	int source_done = 0, dest_done = 0;
	struct fi_cq_tagged_entry cqe;
	struct fi_cq_err_entry err_cqe = {0};
	ssize_t sz;
	uint64_t source = 0xa4321234a4321234,
		 target = 0xb5678901b5678901,
		 before = target;
	uint8_t *s_ptr,*t_ptr;

	rlen = trunc ? slen/2 : slen;

	sz = fi_send(cli_ep, &source, slen, 0, 0, &target);
	cr_assert_eq(sz, 0);

	sz = fi_recv(srv_ep, &target, rlen, 0, 0, &source);
	cr_assert_eq(sz, 0);

	/* need to progress both CQs simultaneously for rendezvous */
	do {
		ret = fi_cq_read(cli_cq, &cqe, 1);
		if (ret == 1) {
			cr_assert_eq(cqe.op_context, &target);
			source_done = 1;
		}

		ret = fi_cq_read(srv_cq, &cqe, 1);
		if (ret == 1) {
			cr_assert(trunc == 0);
			cr_assert_eq(cqe.op_context, &source);
			cr_assert_eq(cqe.len, rlen);
			dest_done = 1;
		} else {
			if (ret == -FI_EAVAIL) {
				cr_assert(trunc != 0);
				ret = fi_cq_readerr(srv_cq, &err_cqe, 0);
				if (ret == 1) {
					cr_assert(err_cqe.olen == (slen - rlen),
						"Bad error olen");
					cr_assert(err_cqe.err == FI_ETRUNC,
						"Bad error errno");
					cr_assert(err_cqe.prov_errno == FI_ETRUNC,
						"Bad prov errno");
					cr_assert(err_cqe.err_data == NULL,
						"Bad error provider data");
					dest_done = 1;
				}
			}
		}

	} while (!source_done || !dest_done);

	s_ptr = (uint8_t *)&source;
	t_ptr = (uint8_t *)&target;
	for (i = 0; i < rlen; i++)
		cr_assert_eq(s_ptr[i], t_ptr[i]);

	s_ptr = (uint8_t *)&before;
	for (i = rlen; i < slen; i++)
		cr_assert_eq(s_ptr[i], t_ptr[i]);

	dbg_printf("Basic send/recv complete! (0x%lx, 0x%lx)\n",
		   source, target);
}

Test(cm_basic, srv_setup, .disabled = false)
{
	int cli_connected = 0, srv_connected = 0;
	int i;

	/* Start listening PEP. */
	cm_server_start();
	/* Create EP and fi_connect() to server. */
	cm_client_start_connect();
	/* Wait for EQE and fi_accept() new EP. */
	cm_server_accept();

	/* Wait for FI_CONNECTED EQES on client and server EQ. */
	do {
		if (!srv_connected) {
			srv_connected += cm_server_finish_connect();
			if (srv_connected) {
				dbg_printf("Server connect complete!\n");
			}
		}

		if (!cli_connected) {
			cli_connected += cm_client_finish_connect();
			if (cli_connected) {
				dbg_printf("Client connect complete!\n");
			}
		}
	} while (!srv_connected || !cli_connected);

	dbg_printf("testing cm_basic_send without trunc\n");
	for (i = 0; i < 1000; i++) {
		/* Perform basic send/recv. */
		cm_basic_send(0);
	}

	/* Perform basic send/recv with trunc*/
	dbg_printf("testing cm_basic_send with trunc\n");
	cm_basic_send(1);

	cm_stop_server();
	cm_stop_client();
}

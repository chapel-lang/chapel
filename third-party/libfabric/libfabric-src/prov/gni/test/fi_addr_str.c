/*
 * Copyright (c) 2017 Los Alamos National Security, LLC. All rights reserved.
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
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "fi_ext_gni.h"
#include "gnix.h"
#include "common.h"

#if 1
#define dbg_printf(...)
#else
#define dbg_printf(...)                                \
	do {                                        \
		printf(__VA_ARGS__);                \
		fflush(stdout);                        \
	} while (0)
#endif

#define NUMEPS 2
#define NUMCONTEXTS (NUMEPS * 2)

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fid_domain *dom[NUMEPS];

static struct fid_ep *ep[NUMEPS];
static struct fid_pep *pep[NUMEPS];
static struct fid_ep **tx_ep[NUMEPS], **rx_ep[NUMEPS];
static struct fid_cq **tx_cq[NUMEPS];
static struct fid_cq **rx_cq[NUMEPS];
static struct fid_av *av[NUMEPS];
static struct fid_cq *msg_cq[NUMEPS];
static fi_addr_t gni_addr[NUMEPS];
static struct fi_info *fi[NUMEPS];
static struct fi_info *hints;

static struct fi_cq_attr cq_attr;
static struct fi_av_attr attr;

static void *ep_name[NUMEPS];
static size_t ep_name_len[NUMEPS];

#define BUF_SZ (1<<15)
static char *target[NUMEPS];
static char *source[NUMEPS];

static struct fid_cntr *send_cntr[NUMEPS], *recv_cntr[NUMEPS];
static struct fi_cntr_attr cntr_attr = {
	.events = FI_CNTR_EVENTS_COMP,
	.flags = 0
};
static uint64_t sends[NUMEPS] = {0}, recvs[NUMEPS] = {0};


static bool use_str_fmt = true;
static enum ep_type_e {
	EP, PEP, SEP
} ep_type;

static fid_t (*get_fid[3])(int);

static int ctx_cnt;

/******************************************************************************
* Begin help routines
******************************************************************************/
static fid_t get_ep_fid(int i)
{
	return &ep[i]->fid;
}

static fid_t get_pep_fid(int i)
{
	return &pep[i]->fid;
}

static fid_t get_sep_fid(int i)
{
	return get_ep_fid(i);
}

struct fid_ep *get_fid_ep(int i, void **info, void **dest_addr,
			  void **src_addr)
{
	struct gnix_fid_ep *ep = NULL;
	struct gnix_fid_sep *sep = NULL;
	struct gnix_fid_pep *pep = NULL;

	switch (ep_type) {
	case EP:
		ep = container_of(get_fid[ep_type](i),
				  struct gnix_fid_ep,
				  ep_fid.fid);
		if (info)
			*info = (void *) ep->info;
		if (dest_addr)
			*dest_addr = (void *) &ep->dest_addr;
		if (src_addr)
			*src_addr = (void *) &ep->src_addr;

		return &ep->ep_fid;
	case SEP:
		sep = container_of(get_fid[ep_type](i),
				   struct gnix_fid_sep,
				   ep_fid);
		if (info)
			*info = (void *) sep->info;
		if (dest_addr) {
			if (!sep->info->dest_addr) {
				sep->info->dest_addr = malloc
					(sep->info->dest_addrlen);
				cr_assert(sep->info->dest_addr, "malloc "
					"failed");
			}
			*dest_addr = sep->info->dest_addr;
		}
		if (src_addr)
			*src_addr = (void *) &sep->my_name;

		return &sep->ep_fid;
	case PEP:
		pep = container_of(get_fid[ep_type](i),
				   struct gnix_fid_pep,
				   pep_fid.fid);
		if (info)
			*info = (void *) pep->info;
		if (dest_addr)
			*dest_addr = pep->info->dest_addr;
		if (src_addr)
			*src_addr = (void *) &pep->src_addr;
		break;
	default:
		cr_assert_fail("Unknown endpoint type.");
	}

	return NULL;
}

/*
 * FI_ADDR_STR; "gni;NODE;SERVICE;GNIX_AV_STR_ADD_VERSION;device_addr;cdm_id;
 * name_type;cm_nic_cdm_id;cookie;rx_ctx_cnt
 */
int generate_rand_fas(char **node)
{
	char rand_str[GNIX_FI_ADDR_STR_LEN] = {0};
	char byte;
	size_t nbytes;

	if (node) {
		/*gni:3*/
		sprintf(rand_str, "gni;");

		/*node:[0-9]+*/
		byte = (rand() % ('Z' - 'A')) + 'A';
		nbytes = 4;
		memset(rand_str + strlen(rand_str), byte, nbytes);
		rand_str[strlen(rand_str)] = ';';
		dbg_printf(BLUE "rand_str = %s\n" COLOR_RESET, rand_str);

		/*service:[0-9]+*/
		byte = (rand() % ('Z' - 'A')) + 'A';
		nbytes = 4;
		memset(rand_str + strlen(rand_str), byte, nbytes);
		rand_str[strlen(rand_str)] = ';';
		dbg_printf(BLUE "rand_str = %s\n" COLOR_RESET, rand_str);

		/*GNIX_AV_STR_ADDR_VERSION:4*/
		sprintf(rand_str + strlen(rand_str), "%04i",
			GNIX_AV_STR_ADDR_VERSION);
		rand_str[strlen(rand_str)] = ';';
		dbg_printf(BLUE "rand_str = %s\n" COLOR_RESET, rand_str);

		/*device_addr:10*/
		byte = (rand() % 10) + '0';
		nbytes = 8;
		sprintf(rand_str + strlen(rand_str), "0x");
		memset(rand_str + strlen(rand_str), byte, nbytes);
		rand_str[strlen(rand_str)] = ';';
		dbg_printf(BLUE "rand_str = %s\n" COLOR_RESET, rand_str);

		/*cdm_id:10*/
		byte = (rand() % 10) + '0';
		nbytes = 8;
		sprintf(rand_str + strlen(rand_str), "0x");
		memset(rand_str + strlen(rand_str), byte, nbytes);
		rand_str[strlen(rand_str)] = ';';
		dbg_printf(BLUE "rand_str = %s\n" COLOR_RESET, rand_str);

		/*name_type:2*/
		byte = (rand() % ('9' - '0')) + '0';
		nbytes = 2;
		memset(rand_str + strlen(rand_str), byte, nbytes);
		rand_str[strlen(rand_str)] = ';';
		dbg_printf(BLUE "rand_str = %s\n" COLOR_RESET, rand_str);

		/*cm_nic_cdm_id:8*/
		byte = (rand() % 10) + '0';
		nbytes = 6;
		sprintf(rand_str + strlen(rand_str), "0x");
		memset(rand_str + strlen(rand_str), byte, nbytes);
		rand_str[strlen(rand_str)] = ';';
		dbg_printf(BLUE "rand_str = %s\n" COLOR_RESET, rand_str);

		/*cookie:10*/
		byte = (rand() % 10) + '0';
		nbytes = 8;
		sprintf(rand_str + strlen(rand_str), "0x");
		memset(rand_str + strlen(rand_str), byte, nbytes);
		rand_str[strlen(rand_str)] = ';';
		dbg_printf(BLUE "rand_str = %s\n" COLOR_RESET, rand_str);

		/*rx_ctx_cnt:3*/
		byte = (rand() % 10) + '0';
		nbytes = 3;
		memset(rand_str + strlen(rand_str), byte, nbytes);
		dbg_printf(BLUE "rand_str = %s\n" COLOR_RESET, rand_str);

		sprintf(node[0], rand_str);
	}
	return 0;
}
/******************************************************************************
* End help routines
******************************************************************************/

/******************************************************************************
* Begin setup routines
******************************************************************************/
static void fas_setup_common(uint32_t version)
{
	int i, ret;

	srand(time(NULL));

	get_fid[EP]  = get_ep_fid;
	get_fid[PEP] = get_pep_fid;
	get_fid[SEP] = get_sep_fid;

	/* This is sufficient for verifying FI_ADDR_STR with seps */
	ctx_cnt = 1;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = NUMEPS * 2;
	hints->domain_attr->data_progress = FI_PROGRESS_AUTO;
	hints->domain_attr->control_progress = FI_PROGRESS_AUTO;
	hints->mode = mode_bits;
	hints->fabric_attr->prov_name = strdup("gni");
	hints->addr_format = use_str_fmt ? FI_ADDR_STR : FI_ADDR_GNI;
	if (ep_type == SEP) {
		hints->ep_attr->tx_ctx_cnt = NUMCONTEXTS;
		hints->ep_attr->rx_ctx_cnt = NUMCONTEXTS;
	}

	/* Get info about fabric services with the provided hints */
	for (i = 0; i < NUMEPS; i++) {
		ret = fi_getinfo(version, NULL, 0, 0, hints,
				 &fi[i]);
		cr_assert(!ret, "fi_getinfo returned: %s", fi_strerror(-ret));

		tx_cq[i] = calloc(ctx_cnt, sizeof(*tx_cq));
		rx_cq[i] = calloc(ctx_cnt, sizeof(*rx_cq));
		tx_ep[i] = calloc(ctx_cnt, sizeof(*tx_ep));
		rx_ep[i] = calloc(ctx_cnt, sizeof(*rx_ep));
	}

	memset(&attr, 0, sizeof(attr));
	attr.rx_ctx_bits = ctx_cnt;
	attr.count = NUMEPS;

	cq_attr.format = FI_CQ_FORMAT_TAGGED;
	cq_attr.size = 1024;
	cq_attr.wait_obj = 0;

	for (i = 0; i < NUMEPS; i++) {
		target[i] = malloc(BUF_SZ);
		cr_assert(target[i], "malloc returned: %s", strerror(errno));

		source[i] = malloc(BUF_SZ);
		cr_assert(source[i], "malloc returned: %s", strerror(errno));
	}

	ret = fi_fabric(fi[0]->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric returned: %s", fi_strerror(-ret));
}

static void fas_ep_setup(void)
{
	int ret, i, j;
	size_t addrlen = 0;

	fas_setup_common(fi_version());
	ctx_cnt = MIN(ctx_cnt, fi[0]->domain_attr->rx_ctx_cnt);
	ctx_cnt = MIN(ctx_cnt, fi[0]->domain_attr->tx_ctx_cnt);

	for (i = 0; i < NUMEPS; i++) {
		fi[i]->ep_attr->tx_ctx_cnt = ctx_cnt;
		fi[i]->ep_attr->rx_ctx_cnt = ctx_cnt;

		ret = fi_domain(fab, fi[i], dom + i, NULL);
		cr_assert(!ret, "fi_domain returned: %s", fi_strerror(-ret));

		ret = fi_cntr_open(dom[i], &cntr_attr, send_cntr + i, 0);
		cr_assert(!ret, "fi_cntr_open returned: %s", fi_strerror(-ret));

		ret = fi_cntr_open(dom[i], &cntr_attr, recv_cntr + i, 0);
		cr_assert(!ret, "fi_cntr_open returned: %s", fi_strerror(-ret));

		switch (ep_type) {
		case EP:
			ret = fi_endpoint(dom[i], fi[i], ep + i, NULL);
			cr_assert(!ret, "fi_endpoint returned: %s",
				  fi_strerror(-ret));
			break;
		case SEP:
			ret = fi_scalable_ep(dom[i], fi[i], ep + i,
					     NULL);
			cr_assert(!ret, "fi_endpoint returned: %s",
				  fi_strerror(-ret));
			break;
		case PEP:
			ret = fi_passive_ep(fab, fi[i], pep + i,
					    NULL);
			cr_assert(!ret, "fi_endpoint returned: %s",
				  fi_strerror(-ret));
			ret = fi_getname(get_fid[ep_type](i), NULL,
					 &addrlen);
			if (use_str_fmt) {
				cr_assert(addrlen == GNIX_FI_ADDR_STR_LEN,
					  "fi_getname returned: %s",
					  fi_strerror(-ret));
			} else {
				cr_assert(addrlen ==
					  sizeof(struct gnix_ep_name),
					  "fi_getname returned: %s",
					  fi_strerror(-ret));
			}
			ep_name_len[i] = addrlen;
			continue;
		default:
			cr_assert_fail("Unknown endpoint type.");
		}

		ret = fi_av_open(dom[i], &attr, av + i, NULL);
		cr_assert(!ret, "fi_av_open returned: %s", fi_strerror(-ret));

		switch (ep_type) {
		case EP:
		case PEP:
			ret = fi_cq_open(dom[i], &cq_attr, msg_cq + i,
					 0);
			cr_assert(!ret, "fi_cq_open returned: %s",
				  fi_strerror(-ret));

			ret = fi_ep_bind(ep[i], &msg_cq[i]->fid,
					 FI_SEND | FI_RECV);
			cr_assert(!ret, "fi_ep_bind returned: %s",
				  fi_strerror(-ret));
			break;
		case SEP:
			dbg_printf(BLUE
					   "ctx_cnt = %d\n"
					   COLOR_RESET,
				   ctx_cnt);

			for (j = 0; j < ctx_cnt; j++) {
				ret = fi_tx_context(ep[i], j, NULL,
						    &tx_ep[i][j], NULL);
				cr_assert(!ret,
					  "fi_tx_context  returned: %s",
					  fi_strerror(-ret));

				ret = fi_cq_open(dom[i], &cq_attr,
						 &tx_cq[i][j],
						 NULL);
				cr_assert(!ret,
					  "fi_cq_open  returned: %s",
					  fi_strerror(-ret));

				ret = fi_rx_context(ep[i], j, NULL,
						    &rx_ep[i][j], NULL);
				cr_assert(!ret,
					  "fi_rx_context  returned: %s",
					  fi_strerror(-ret));

				ret = fi_cq_open(dom[i], &cq_attr,
						 &rx_cq[i][j],
						 NULL);
				cr_assert(!ret,
					  "fi_cq_open  returned: %s",
					  fi_strerror(-ret));
			}
			break;
		default:
			cr_assert_fail("Unknown endpoint type.");
		}

		ret = fi_getname(get_fid[ep_type](i), NULL, &addrlen);
		if (use_str_fmt) {
			cr_assert(addrlen > sizeof(struct gnix_ep_name),
				  "fi_getname returned: %s",
				  fi_strerror(-ret));
		} else {
			cr_assert(addrlen == sizeof(struct gnix_ep_name),
				  "fi_getname returned: %s",
				  fi_strerror(-ret));
		}

		ep_name[i] = malloc(addrlen);
		ep_name_len[i] = addrlen;

		dbg_printf(BLUE
				   "ep_name_len[%d] = %lu\n"
				   COLOR_RESET, i,
			   ep_name_len[i]);
		cr_assert(ep_name[i] != NULL, "malloc returned: %s",
			  strerror(errno));

		ret = fi_getname(get_fid[ep_type](i), ep_name[i], &addrlen);
		cr_assert(ret == FI_SUCCESS, "fi_getname returned: %s",
			  fi_strerror(-ret));
	}

	/* Just testing setname / getname for passive endpoints */
	if (ep_type == PEP)
		return;

	for (i = 0; i < NUMEPS; i++) {
		/*Insert all gni addresses into each av*/
		for (j = 0; j < NUMEPS; j++) {
			ret = fi_av_insert(av[i], ep_name[j], 1, &gni_addr[j],
					   0, NULL);
			cr_assert(ret == 1, "fi_av_insert returned: %s",
				  fi_strerror(-ret));
		}

		switch (ep_type) {
		case EP:
			ret = fi_ep_bind(ep[i], &av[i]->fid, 0);
			cr_assert(!ret, "fi_ep_bind returned: %s",
				  fi_strerror(-ret));

			ret = fi_ep_bind(ep[i], &send_cntr[i]->fid,
					 FI_SEND);
			cr_assert(!ret, "fi_ep_bind returned: %s",
				  fi_strerror(-ret));

			ret = fi_ep_bind(ep[i], &recv_cntr[i]->fid,
					 FI_RECV);
			cr_assert(!ret, "fi_ep_bind returned: %s",
				  fi_strerror(-ret));
			break;
		case SEP:
			ret = fi_scalable_ep_bind(ep[i], &av[i]->fid,
						  0);
			cr_assert(!ret,
				  "fi_scalable_ep_bind returned: %s",
				  fi_strerror(-ret));
			dbg_printf(BLUE
					   "ctx_cnt = %d\n"
					   COLOR_RESET,
				   ctx_cnt);
			for (j = 0; j < ctx_cnt; j++) {
				ret = fi_ep_bind(tx_ep[i][j],
						 &tx_cq[i][j]->fid,
						 FI_TRANSMIT);
				cr_assert(!ret,
					  "fi_ep_bind  returned: %s",
					  fi_strerror(-ret));

				ret = fi_ep_bind(tx_ep[i][j],
						 &send_cntr[i]->fid,
						 FI_SEND);
				cr_assert(!ret,
					  "fi_ep_bind  returned: %s",
					  fi_strerror(-ret));

				ret = fi_enable(tx_ep[i][j]);
				cr_assert(!ret,
					  "fi_enable  returned: %s",
					  fi_strerror(-ret));

				ret = fi_ep_bind(rx_ep[i][j],
						 &rx_cq[i][j]->fid,
						 FI_RECV);
				cr_assert(!ret,
					  "fi_ep_bind  returned: %s",
					  fi_strerror(-ret));

				ret = fi_ep_bind(rx_ep[i][j],
						 &recv_cntr[i]->fid,
						 FI_RECV);
				cr_assert(!ret,
					  "fi_ep_bind  returned: %s",
					  fi_strerror(-ret));

				ret = fi_enable(rx_ep[i][j]);
				cr_assert(!ret,
					  "fi_enable  returned: %s",
					  fi_strerror(-ret));

			}
			break;
		case PEP:
			break;
		default:
			cr_assert_fail("Unknown endpoint type.");
		}

		ret = fi_enable(ep[i]);
		cr_assert(!ret, "fi_ep_enable returned: %s", fi_strerror(-ret));

		if (ep_type != SEP) {
			ret = fi_enable(ep[i]);
			cr_assert_eq(ret, -FI_EOPBADSTATE,
				     "fi_enable returned: %s",
				     fi_strerror(-ret));
		}
	}
}

static void fas_getinfo_setup(void)
{
	srand(time(NULL));

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = NUMEPS * 2;
	hints->domain_attr->data_progress = FI_PROGRESS_AUTO;
	hints->domain_attr->control_progress = FI_PROGRESS_AUTO;
	hints->mode = ~0;
	hints->fabric_attr->prov_name = strdup("gni");
	hints->addr_format = use_str_fmt ? FI_ADDR_STR : FI_ADDR_GNI;
}

static void fas_getinfo_teardown(void)
{
	fi_freeinfo(hints);
}

static void fas_ep_setup_gni_fmt_av_map(void)
{
	use_str_fmt = false;
	ep_type = EP;
	attr.type = FI_AV_MAP;

	fas_ep_setup();
}

static void fas_ep_setup_str_fmt_av_map(void)
{
	use_str_fmt = true;
	ep_type = EP;
	attr.type = FI_AV_MAP;

	fas_ep_setup();
}

static void fas_sep_setup_gni_fmt_av_map(void)
{
	use_str_fmt = false;
	ep_type = SEP;
	attr.type = FI_AV_MAP;

	fas_ep_setup();
}

static void fas_sep_setup_str_fmt_av_map(void)
{
	use_str_fmt = true;
	ep_type = SEP;
	attr.type = FI_AV_MAP;

	fas_ep_setup();
}

static void fas_pep_setup_gni_fmt_av_map(void)
{
	use_str_fmt = false;
	ep_type = PEP;
	attr.type = FI_AV_MAP;

	fas_ep_setup();
}

static void fas_pep_setup_str_fmt_av_map(void)
{
	use_str_fmt = true;
	ep_type = PEP;
	attr.type = FI_AV_MAP;

	fas_ep_setup();
}

static void fas_ep_setup_gni_fmt_av_tbl(void)
{
	use_str_fmt = false;
	ep_type = EP;
	attr.type = FI_AV_TABLE;

	fas_ep_setup();
}

static void fas_ep_setup_str_fmt_av_tbl(void)
{
	use_str_fmt = true;
	ep_type = EP;
	attr.type = FI_AV_TABLE;

	fas_ep_setup();
}

static void fas_sep_setup_gni_fmt_av_tbl(void)
{
	use_str_fmt = false;
	ep_type = SEP;
	attr.type = FI_AV_TABLE;

	fas_ep_setup();
}

static void fas_sep_setup_str_fmt_av_tbl(void)
{
	use_str_fmt = true;
	ep_type = SEP;
	attr.type = FI_AV_TABLE;

	fas_ep_setup();
}

static void fas_pep_setup_gni_fmt_av_tbl(void)
{
	use_str_fmt = false;
	ep_type = PEP;
	attr.type = FI_AV_TABLE;

	fas_ep_setup();
}

static void fas_pep_setup_str_fmt_av_tbl(void)
{
	use_str_fmt = true;
	ep_type = PEP;
	attr.type = FI_AV_TABLE;

	fas_ep_setup();
}

static void fas_teardown_common(void)
{
	int ret = 0, i = 0, j;

	for (; i < NUMEPS; i++) {
		if (ep_type == SEP || ep_type == EP) {
			ret = fi_close(&recv_cntr[i]->fid);
			cr_assert(!ret, "failure in closing recv cntr.");

			ret = fi_close(&send_cntr[i]->fid);
			cr_assert(!ret, "failure in closing send cntr.");
		}

		switch (ep_type) {
		case EP:
			ret = fi_close(&msg_cq[i]->fid);
			cr_assert(!ret, "failure in closing msg cq.");
			break;
		case SEP:
			for (j = 0; j < ctx_cnt; j++) {
				ret = fi_close(&tx_ep[i][j]->fid);
				cr_assert(!ret,
					  "failure closing tx_ep.");

				ret = fi_close(&rx_ep[i][j]->fid);
				cr_assert(!ret,
					  "failure closing rx_ep.");

				ret = fi_close(&tx_cq[i][j]->fid);
				cr_assert(!ret,
					  "failure closing tx cq.");

				ret = fi_close(&rx_cq[i][j]->fid);
				cr_assert(!ret,
					  "failure closing rx cq.");
			}
			break;
		case PEP:
			ret = fi_close(get_fid[ep_type](i));
			cr_assert(!ret, "failure in closing ep.");
			continue;
			break;
		default:
			cr_assert_fail("Unknown endpoint type.");
			break;
		}

		ret = fi_close(get_fid[ep_type](i));
		cr_assert(!ret, "failure in closing ep.");

		ret = fi_close(&av[i]->fid);
		cr_assert(!ret, "failure in closing av.");

		ret = fi_close(&dom[i]->fid);
		cr_assert(!ret, "failure in closing domain.");

		free(ep_name[i]);
		free(target[i]);
		free(source[i]);
		free(tx_cq[i]);
		free(tx_ep[i]);
		free(rx_cq[i]);
		free(rx_ep[i]);

		fi_freeinfo(fi[i]);
	}

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");

	fi_freeinfo(hints);
}
/******************************************************************************
* End setup and teardown routines
******************************************************************************/

/******************************************************************************
* Begin verification routines
******************************************************************************/
static void fas_to_ep_name(char *ep_name_str, struct gnix_ep_name *rebuilt)
{
	char *buf;

	dbg_printf(BLUE
			   "ep_name_str(%p) = %s.\n"
			   COLOR_RESET, ep_name_str, ep_name_str);

	buf = strtok(ep_name_str, ";");

	cr_assert_not_null(buf, "address family not found");

	dbg_printf(BLUE
			   "buf = %s\nbuf_len = %lu\n"
			   COLOR_RESET, buf,
		   strlen(buf));
	cr_assert(!memcmp(buf, "gni", 3));

	buf = strtok(NULL, ";");
	cr_assert_not_null(buf, "node not found");
	dbg_printf(BLUE
			   "buf = %s\nbuf_len = %lu\n"
			   COLOR_RESET, buf,
		   strlen(buf));

	buf = strtok(NULL, ";");
	cr_assert_not_null(buf, "service not found");
	dbg_printf(BLUE
			   "buf = %s\nbuf_len = %lu\n"
			   COLOR_RESET, buf, strlen(buf));


	buf = strtok(NULL, ";");
	cr_assert_not_null(buf, "zeroth additional field "
		"(GNIX_AV_STR_ADDR_VERSION) not found");
	dbg_printf(BLUE
			   "buf = %s\nbuf_len = %lu\n"
			   COLOR_RESET, buf, strlen(buf));

	buf = strtok(NULL, ";");
	cr_assert_not_null(buf, "first additional field (device address) not "
		"found");
	rebuilt->gnix_addr.device_addr = strtol(buf, NULL, 16);
	dbg_printf(BLUE
			   "buf = %s\nbuf_len = %lu\n"
			   COLOR_RESET, buf, strlen(buf));

	buf = strtok(NULL, ";");
	cr_assert_not_null(buf, "second additional field (cdm id) not found");
	rebuilt->gnix_addr.cdm_id = strtol(buf, NULL, 16);
	dbg_printf(BLUE
			   "buf = %s\nbuf_len = %lu\n"
			   COLOR_RESET, buf, strlen(buf));

	buf = strtok(NULL, ";");
	cr_assert_not_null(buf, "third additional field (name type) not found");
	rebuilt->name_type = strtol(buf, NULL, 10);
	dbg_printf(BLUE
			   "buf = %s\nbuf_len = %lu\n"
			   COLOR_RESET, buf, strlen(buf));

	buf = strtok(NULL, ";");
	cr_assert_not_null(buf, "forth additional field (cm_nic_cdm_id) not "
		"found");
	rebuilt->cm_nic_cdm_id = strtol(buf, NULL, 16);
	dbg_printf(BLUE
			   "buf = %s\nbuf_len = %lu\n"
			   COLOR_RESET, buf, strlen(buf));

	buf = strtok(NULL, ";");
	cr_assert_not_null(buf, "fifth additional field (cookie) not found");
	rebuilt->cookie = strtol(buf, NULL, 16);
	dbg_printf(BLUE
			   "buf = %s\nbuf_len = %lu\n"
			   COLOR_RESET, buf, strlen(buf));

	buf = strtok(NULL, ";");
	cr_assert_not_null(buf, "sixth additional field (rx_ctx_cnt) not "
		"found");
	rebuilt->rx_ctx_cnt = strtol(buf, NULL, 10);
	dbg_printf(BLUE
			   "buf = %s\nbuf_len = %lu\n"
			   COLOR_RESET, buf, strlen(buf));
}

static void check_ep_name(struct gnix_ep_name actual,
			  struct gnix_ep_name expected)
{
	cr_assert_eq(expected.gnix_addr.cdm_id, actual.gnix_addr.cdm_id,
		     "Invalid cdm_id: expected(%x) actual(%x)",
		     actual.gnix_addr.cdm_id, expected.gnix_addr.cdm_id);

	cr_assert_eq(expected.name_type, actual.name_type, "Invalid name_type: "
		"expected(%x) actual(%x)", actual.name_type,
		     expected.name_type);

	cr_assert_eq(expected.cm_nic_cdm_id, actual.cm_nic_cdm_id, "Invalid "
		"cm_nic_cmd_id: expected(%x) actual(%x)", actual.cm_nic_cdm_id,
		     expected.cm_nic_cdm_id);

	cr_assert_eq(expected.cookie, actual.cookie, "Invalid cookie: expected"
		"(%x) actual(%x)", actual.cookie, expected.cookie);

	cr_assert_eq(expected.rx_ctx_cnt, actual.rx_ctx_cnt, "Invalid "
		"rx_ctx_cnt: expected(%x) actual(%x)", actual.rx_ctx_cnt,
		     expected.rx_ctx_cnt);
}

static void check_ep_name_str(struct gnix_ep_name actual, void *expected,
			      size_t ep_name_len)
{
	char *ep_name_str;
	struct gnix_ep_name rebuilt;

	ep_name_str = (char *) mem_dup(expected, ep_name_len);

	fas_to_ep_name(ep_name_str, &rebuilt);

	check_ep_name(actual, rebuilt);

	free(ep_name_str);
}

void init_bufs(void **bufs, int nbufs, int len)
{
	int i;
	char byte = (char) rand();

	for (i = 0; i < nbufs; i++, byte = (char) rand()) {
		memset(bufs[i], byte, len);
	}
}

void check_buf(char *expected, char *buf, int len)
{
	int idx;

	for (idx = 0; idx < len; idx++) {
		cr_assert(expected[idx] == buf[idx], "data mismatch: "
			"expected[%d] = 0x%x, buf[%d] = 0x%x", idx,
			  expected[idx], idx, buf[idx]);
	}
}

void check_tagged_cqe(struct fi_cq_tagged_entry expected_tcqe,
		      struct fi_cq_tagged_entry fi_tcqe)
{
	cr_assert_eq(expected_tcqe.op_context, fi_tcqe.op_context,
		     "Invalid op_context: expected(%p) actual(%p)",
		     expected_tcqe.op_context, fi_tcqe.op_context);

	cr_assert_eq(expected_tcqe.flags, fi_tcqe.flags,
		     "Invalid flags: expected(0x%lx) actual(0x%lx",
		     expected_tcqe.flags, fi_tcqe.flags);

	cr_assert_eq(expected_tcqe.len, fi_tcqe.len,
		     "Invalid len: expected(%lu) actual(%lu)",
		     expected_tcqe.len, fi_tcqe.len);

	cr_assert_eq(expected_tcqe.buf, fi_tcqe.buf,
		     "Invalid buf: expected(%p) actual(%p)",
		     expected_tcqe.buf, fi_tcqe.buf);

	cr_assert_eq(expected_tcqe.data, fi_tcqe.data,
		     "Invalid data: expected(0x%lx) actual(0x%lx)",
		     expected_tcqe.data, fi_tcqe.data);

	cr_assert_eq(expected_tcqe.tag, fi_tcqe.tag,
		     "Invalid tag: expected(0x%lx) actual(0x%lx)",
		     expected_tcqe.tag, fi_tcqe.tag);
}
/******************************************************************************
* End verification routines
******************************************************************************/

/******************************************************************************
* Begin test running routines
******************************************************************************/
void do_getname(void)
{
	int i, ret;
	size_t addrlen;
	void *addr;
	struct gnix_ep_name *src_addr;

	ret = fi_getname(get_fid[ep_type](0), NULL, NULL);
	cr_assert(ret == -FI_EINVAL, "fi_getname returned: %s",
		  fi_strerror(-ret));

	for (i = 0; i < NUMEPS; i++) {
		ret = fi_getname(get_fid[ep_type](i), NULL, &addrlen);
		cr_assert(ret == -FI_ETOOSMALL, "fi_getname returned: %s",
			  fi_strerror(-ret));
		if (use_str_fmt) {
			cr_assert(addrlen == GNIX_FI_ADDR_STR_LEN,
				  "addrlen: %lu does not match size for "
					  "FI_ADDR_STR", addrlen);
		} else {
			cr_assert(addrlen == sizeof(struct gnix_ep_name),
				  "addrlen: %lu does not match the size for"
					  " FI_ADDR_GNI", addrlen);
		}

		addr = malloc(addrlen);
		ret = errno;
		cr_assert_not_null(addr, "malloc returned: %s", strerror(ret));

		ret = fi_getname(get_fid[ep_type](i), addr, &addrlen);
		cr_assert(ret == FI_SUCCESS, "fi_getname returned: %s",
			  fi_strerror(-ret));

		if (use_str_fmt) {
			cr_assert(addrlen == GNIX_FI_ADDR_STR_LEN,
				  "addrlen: %lu does not match size for "
					  "FI_ADDR_STR", addrlen);
		} else {
			cr_assert(addrlen == sizeof(struct gnix_ep_name),
				  "addrlen: %lu does not match the size for "
					  "FI_ADDR_GNI", addrlen);
		}

		get_fid_ep(i, NULL, NULL, (void **) &src_addr);

		dbg_printf(BLUE "ep_name = %p\n" COLOR_RESET, src_addr);

		if (use_str_fmt)
			check_ep_name_str(*src_addr, addr, ep_name_len[i]);
		free(addr);
	}
}

void do_setname(void)
{
	int i, ret;
	void *addr;
	struct gnix_ep_name *src_addr = NULL, rebuilt;
	struct fi_info *info = NULL;

	ret = fi_setname(get_fid[ep_type](0), NULL, 0xbabbbcbd);
	cr_assert(ret == -FI_EINVAL, "fi_setname returned: %s",
		  fi_strerror(-ret));

	for (i = 0; i < NUMEPS; i++) {
		addr = malloc(ep_name_len[i]);
		ret = errno;
		cr_assert_not_null(addr, "malloc returned: %s", strerror(ret));

		if (use_str_fmt)
			generate_rand_fas((char **) &addr);
		else
			init_bufs(&addr, 1, ep_name_len[i]);

		ret = fi_setname(get_fid[ep_type](i), addr, ep_name_len[i]);
		cr_assert(ret == FI_SUCCESS, "fi_setname returned: %s",
			  fi_strerror(-ret));

		get_fid_ep(i, (void **) &info, (void **) NULL, (void **)
			&src_addr);

		/* Ensure that the address was set properly. */
		if (use_str_fmt) {
			fas_to_ep_name(addr, &rebuilt);
			check_ep_name(rebuilt, *src_addr);
		} else {
			check_ep_name(((struct gnix_ep_name *)addr)[0],
				      *src_addr);
		}


		free(addr);
	}
}

void do_getpeer(void)
{
	int i = 0, ret;
	size_t addrlen;
	void *addr;
	struct gnix_ep_name *dest_addr, rebuilt;
	struct fid_ep *ep_fid;

	ret = fi_getpeer(get_fid_ep(0, NULL, NULL, NULL), NULL, NULL);
	cr_assert(ret == -FI_EINVAL, "fi_getpeer returned: %s",
		  fi_strerror(-ret));

	for (i = 0; i < NUMEPS; i++) {
		ep_fid = get_fid_ep(i, NULL, (void **) &dest_addr, NULL);
		addrlen = ep_name_len[i];
		addr = malloc(addrlen);
		ret = errno;
		cr_assert_not_null(addr, "malloc returned: %s", strerror(ret));
		init_bufs(&addr, 1, addrlen);

		addrlen = 0;
		ret = fi_getpeer(ep_fid, addr, &addrlen);
		cr_assert(ret == -FI_ETOOSMALL, "fi_getpeer returned: %s",
			  fi_strerror(-ret));

		ret = fi_getpeer(ep_fid, addr, &addrlen);
		cr_assert(ret == FI_SUCCESS, "fi_getpeer returned: %s",
			  fi_strerror(-ret));

		if (use_str_fmt) {
			dbg_printf(BLUE "strlen(addr) = %lu\n" COLOR_RESET,
				   strlen(addr));

			fas_to_ep_name(addr, &rebuilt);
			check_ep_name(*dest_addr, rebuilt);
		} else {
			check_ep_name(*dest_addr,
				      ((struct gnix_ep_name *) addr)[0]);
		}

		free(addr);
	}
}

void do_getname_enosys(void)
{
	int ret, i;

	for (i = 0; i < NUMEPS; i++) {
		ret = fi_getname(get_fid[ep_type](i), NULL, NULL);
		cr_assert_eq(ret, -FI_ENOSYS, "Invalid return value: %s",
			     fi_strerror(-ret));
	}
}

void do_setname_enosys(void)
{
	int ret, i;

	for (i = 0; i < NUMEPS; i++) {
		ret = fi_setname(get_fid[ep_type](i), NULL, 0);
		cr_assert_eq(ret, -FI_ENOSYS, "Invalid return value: %s",
			     fi_strerror(-ret));
	}
}

void do_getpeer_enosys(void)
{
	int ret = 0, i;
	struct gnix_fid_pep *gnix_pep;
	struct fid_ep *ep_fid = NULL;

	for (i = 0; i < NUMEPS; i++) {
		switch (ep_type) {
		case EP:
			ep_fid = get_fid_ep(i, NULL, NULL, NULL);
			ret = fi_getpeer(ep_fid, NULL, NULL);
			break;
		case SEP:
			ep_fid = get_fid_ep(i, NULL, NULL, NULL);
			ret = fi_getpeer(ep_fid, NULL, NULL);
			break;
		case PEP:
			gnix_pep = container_of(get_fid[ep_type](i),
						struct gnix_fid_pep,
						pep_fid.fid);
			ret = gnix_pep->pep_fid.cm->getpeer(NULL, NULL,
							    NULL);
			break;
		default:
			cr_assert_fail("Unknown endpoint type.");
		}

		cr_assert_eq(ret, -FI_ENOSYS, "Invalid return value: %s",
			     fi_strerror(-ret));
	}
}

void do_ep_send_recv_iter(int len)
{
	ssize_t sz;
	int i = 0;
	uint64_t cntr;
	ssize_t ret, src_done, dest_done;
	struct fi_cq_tagged_entry s_cqe = {(void *) -1, UINT_MAX, UINT_MAX,
					   (void *) -1, UINT_MAX, UINT_MAX};
	struct fi_cq_tagged_entry d_cqe = {(void *) -1, UINT_MAX, UINT_MAX,
					   (void *) -1, UINT_MAX, UINT_MAX};
	struct fi_cq_tagged_entry s_expected_cqe, d_expected_cqe;

	init_bufs((void **) source, NUMEPS, len);
	init_bufs((void **) target, NUMEPS, len);

	for (i = 0; i < NUMEPS; i++) {
		dbg_printf(BLUE
				   "From ep(%d) to ep(%d) of xfer size %d\n"
				   COLOR_RESET, i, NUMEPS - 1 - i, len);

		s_expected_cqe.buf = NULL;
		s_expected_cqe.data = 0;
		s_expected_cqe.flags = (FI_MSG | FI_SEND);
		s_expected_cqe.len = 0;
		s_expected_cqe.op_context = target[NUMEPS - 1 - i];
		s_expected_cqe.tag = 0;

		sz = fi_send(ep[i], source[i], len, NULL,
			     gni_addr[NUMEPS - 1 - i], target[NUMEPS - 1 - i]);

		cr_assert(sz == FI_SUCCESS, "Invalid return value: %s",
			  fi_strerror((int) -sz));

		d_expected_cqe.buf = NULL;
		d_expected_cqe.data = 0;
		d_expected_cqe.flags = (FI_MSG | FI_RECV);
		d_expected_cqe.len = len;
		d_expected_cqe.op_context = source[i];
		d_expected_cqe.tag = 0;

		sz = fi_recv(ep[NUMEPS - 1 - i], target[NUMEPS - 1 - i], len,
			     NULL, gni_addr[i], source[i]);

		cr_assert(sz == FI_SUCCESS, "Invalid return value: %s",
			  fi_strerror((int) -sz));

		src_done = dest_done = 0;
		/* Progress sender and receiver */
		do {
			ret = fi_cq_read(msg_cq[i], &s_cqe, 1);
			if (ret == 1)
				src_done = 1;

			ret = fi_cq_read(msg_cq[NUMEPS - 1 - i],
					 &d_cqe, 1);
			if (ret == 1)
				dest_done = 1;
		} while (src_done != 1 || dest_done != 1);

		cntr = fi_cntr_read(send_cntr[i]);
		cr_assert(cntr == ++sends[i],
			  "Invalid send counter: actual(%lu), expected(%lu)",
			  cntr, sends[i]);

		cntr = fi_cntr_read(recv_cntr[NUMEPS - 1 - i]);
		cr_assert(cntr == ++recvs[NUMEPS - 1 - i],
			  "Invalid recv counter: actual(%lu), expected(%lu)",
			  cntr, recvs[NUMEPS - 1 - i]);

		check_tagged_cqe(s_expected_cqe, s_cqe);
		check_tagged_cqe(d_expected_cqe, d_cqe);

		check_buf(source[i], target[NUMEPS - 1 - i], len);
	}
}

void do_sep_send_recv_iter(int idx, int len)
{
	ssize_t sz;
	int i = 0;
	uint64_t cntr;
	ssize_t ret, src_done, dest_done;
	struct fi_cq_tagged_entry s_cqe = {(void *) -1, UINT_MAX, UINT_MAX,
					   (void *) -1, UINT_MAX, UINT_MAX};
	struct fi_cq_tagged_entry d_cqe = {(void *) -1, UINT_MAX, UINT_MAX,
					   (void *) -1, UINT_MAX, UINT_MAX};
	struct fi_cq_tagged_entry s_expected_cqe, d_expected_cqe;

	init_bufs((void **) source, NUMEPS, len);
	init_bufs((void **) target, NUMEPS, len);

	for (i = 0; i < NUMEPS; i++) {
		dbg_printf(BLUE
				   "From ep(%d) to ep(%d) of xfer size %d\n"
				   COLOR_RESET, i, NUMEPS - 1 - i, len);

		s_expected_cqe.buf = NULL;
		s_expected_cqe.data = 0;
		s_expected_cqe.flags = (FI_MSG | FI_TRANSMIT/*FI_SEND*/);
		s_expected_cqe.len = 0;
		s_expected_cqe.op_context = target[NUMEPS - 1 - i];
		s_expected_cqe.tag = 0;

		sz = fi_send(tx_ep[i][idx], source[i], len, NULL,
			     gni_addr[NUMEPS - 1 - i], target[NUMEPS - 1 - i]);

		cr_assert(sz == FI_SUCCESS, "Invalid return value: %s",
			  fi_strerror((int) -sz));

		d_expected_cqe.buf = NULL;
		d_expected_cqe.data = 0;
		d_expected_cqe.flags = (FI_MSG | FI_RECV);
		d_expected_cqe.len = len;
		d_expected_cqe.op_context = source[i];
		d_expected_cqe.tag = 0;

		sz = fi_recv(rx_ep[NUMEPS - 1 - i][idx],
			     target[NUMEPS - 1 - i], len,
			     NULL, gni_addr[i], source[i]);

		cr_assert(sz == FI_SUCCESS, "Invalid return value: %s",
			  fi_strerror((int) -sz));

		src_done = dest_done = 0;
		/* Progress sender and receiver */
		do {
			ret = fi_cq_read(tx_cq[i][idx], &s_cqe, 1);
			if (ret == 1)
				src_done = 1;

			ret = fi_cq_read(rx_cq[NUMEPS - 1 - i][idx],
					 &d_cqe, 1);
			if (ret == 1)
				dest_done = 1;
		} while (src_done != 1 || dest_done != 1);

		cntr = fi_cntr_read(send_cntr[i]);
		cr_assert(cntr == ++sends[i],
			  "Invalid send counter: actual(%lu), expected(%lu)",
			  cntr, sends[i]);

		cntr = fi_cntr_read(recv_cntr[NUMEPS - 1 - i]);
		cr_assert(cntr == ++recvs[NUMEPS - 1 - i],
			  "Invalid recv counter: actual(%lu), expected(%lu)",
			  cntr, recvs[NUMEPS - 1 - i]);

		check_tagged_cqe(s_expected_cqe, s_cqe);
		check_tagged_cqe(d_expected_cqe, d_cqe);

		check_buf(source[i], target[NUMEPS - 1 - i], len);
	}
}

void do_send_recv(void)
{
	int len, i, j;

	switch (ep_type) {
	case EP:
		for (len = 2; len <= BUF_SZ; len *= 2) {
			do_ep_send_recv_iter(len);
		}
		break;

	case SEP:
		for (j = 0; j < ctx_cnt; j++) {
			for (len = 2; len <= BUF_SZ; len *= 2) {
				do_sep_send_recv_iter(j, len);
			}

			for (i = 0; i < NUMEPS; i++) {
				fi_cntr_set(send_cntr[i], 0);
				fi_cntr_set(recv_cntr[i], 0);
			}
		}
		break;
	case PEP:
		break;

	default:
		cr_assert_fail("Invalid endpoint type.");
	}
}

/*
 * Note: the default addr_format is FI_ADDR_STR unless use_str_fmt is otherwise
 * set to false.
 */
void do_invalid_fi_getinfo(void)
{
	int i, ret;

	for (i = 0; i < NUMEPS; i++) {
		/*
		 * This test is to ensure that gni provider fails to provide
		 * info if the FI_ADDR_STR format is being used and both the
		 * node and service parameters are non-NULL.
		 *
		 * See the fi_getinfo man page DESCRIPTION section.
		 */
		ret = fi_getinfo(FI_VERSION(1, 5), "this is a test", "testing",
				 0, hints, &fi[i]);
		cr_assert(ret == -FI_ENODATA, "fi_getinfo returned: %s",
			  fi_strerror(-ret));

		fi_freeinfo(fi[i]);

		/*
		 * This test is to ensure that the gni provider does not allow
		 * FI_ADDR_STR to be used with api versions <= 1.5.
		 */
		ret = fi_getinfo(FI_VERSION(1, 0), NULL, NULL, 0, hints,
				 &fi[i]);
		cr_assert(ret == -FI_ENODATA, "fi_getinfo returned: %s",
			  fi_strerror(-ret));

		fi_freeinfo(fi[i]);
	}
}

void do_valid_fi_getinfo_with_fas(void)
{
	int i, ret;
	char *fas = calloc(GNIX_FI_ADDR_STR_LEN, sizeof(char));
	struct gnix_ep_name ep_name;

	/*
	 * This test ensures the gni provider can set addresses properly with
	 * FI_ADDR_STR and no flags set.
	 */
	for (i = 0; i < NUMEPS; i++) {
		generate_rand_fas(&fas);

		ret = fi_getinfo(fi_version(), fas, NULL, 0, hints, &fi[i]);
		cr_assert(ret == FI_SUCCESS, "fi_getinfo returned: %s",
			  fi_strerror(-ret));

		dbg_printf(BLUE "fi[%d]->dest_addr = %s\n" COLOR_RESET,
			   i, (char *) fi[i]->dest_addr);

		fas_to_ep_name(fas, &ep_name);
		check_ep_name(((struct gnix_ep_name *) fi[i]->dest_addr)[0],
			      ep_name);

		fi_freeinfo(fi[i]);
	}

	/*
	 * This test ensures the gni provider can set addresses properly with
	 * FI_ADDR_STR and the FI_SOURCE set.
	 */
	for (i = 0; i < NUMEPS; i++) {
		generate_rand_fas(&fas);

		ret = fi_getinfo(fi_version(), fas, NULL, FI_SOURCE, hints,
				 &fi[i]);
		cr_assert(ret == FI_SUCCESS, "fi_getinfo returned: %s",
			  fi_strerror(-ret));

		fas_to_ep_name(fas, &ep_name);
		check_ep_name(((struct gnix_ep_name *) fi[i]->src_addr)[0],
			      ep_name);

		fi_freeinfo(fi[i]);
	}

	free(fas);
}
/******************************************************************************
* End test running routines
******************************************************************************/

/******************************************************************************
* Begin test invocation routines - FI_AV_MAP
******************************************************************************/
TestSuite(fas_ep_str_fmt_av_map, .init = fas_ep_setup_str_fmt_av_map,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_ep_str_fmt_av_map, getname)
{
	do_getname();
}

Test(fas_ep_str_fmt_av_map, setname)
{
	do_setname();
}

Test(fas_ep_str_fmt_av_map, getpeer)
{
	do_getpeer();
}

Test(fas_ep_str_fmt_av_map, send_recv)
{
	do_send_recv();
}

TestSuite(fas_ep_gni_fmt_av_map, .init = fas_ep_setup_gni_fmt_av_map,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_ep_gni_fmt_av_map, getname)
{
	do_getname();
}

Test(fas_ep_gni_fmt_av_map, setname)
{
	do_setname();
}

Test(fas_ep_gni_fmt_av_map, getpeer)
{
	do_getpeer();
}

Test(fas_ep_gni_fmt_av_map, send_recv)
{
	do_send_recv();
}

TestSuite(fas_sep_str_fmt_av_map, .init = fas_sep_setup_str_fmt_av_map,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_sep_str_fmt_av_map, getname)
{
	do_getname();
}

Test(fas_sep_str_fmt_av_map, setname)
{
	do_setname();
}

Test(fas_sep_str_fmt_av_map, getpeer)
{
	do_getpeer();
}

Test(fas_sep_str_fmt_av_map, send_recv)
{
	do_send_recv();
}

TestSuite(fas_sep_gni_fmt_av_map, .init = fas_sep_setup_gni_fmt_av_map,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_sep_gni_fmt_av_map, getname)
{
	do_getname();
}

Test(fas_sep_gni_fmt_av_map, setname)
{
	do_setname();
}

Test(fas_sep_gni_fmt_av_map, getpeer)
{
	do_getpeer();
}

Test(fas_sep_gni_fmt_av_map, send_recv)
{
	do_send_recv();
}

TestSuite(fas_pep_str_fmt_av_map, .init = fas_pep_setup_str_fmt_av_map,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_pep_str_fmt_av_map, getname)
{
	do_getname();
}

Test(fas_pep_str_fmt_av_map, setname)
{
	do_setname();
}

Test(fas_pep_str_fmt_av_map, getpeer)
{
	do_getpeer_enosys();
}

TestSuite(fas_pep_gni_fmt_av_map, .init = fas_pep_setup_gni_fmt_av_map,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_pep_gni_fmt_av_map, getname)
{
	do_getname();
}

Test(fas_pep_gni_fmt_av_map, setname)
{
	do_setname();
}

Test(fas_pep_gni_fmt_av_map, getpeer)
{
	do_getpeer_enosys();
}
/******************************************************************************
 * Begin FI_AV_TABLE
 ******************************************************************************/
TestSuite(fas_ep_str_fmt_av_tbl, .init = fas_ep_setup_str_fmt_av_tbl,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_ep_str_fmt_av_tbl, getname)
{
	do_getname();
}

Test(fas_ep_str_fmt_av_tbl, setname)
{
	do_setname();
}

Test(fas_ep_str_fmt_av_tbl, getpeer)
{
	do_getpeer();
}

Test(fas_ep_str_fmt_av_tbl, send_recv)
{
	do_send_recv();
}

TestSuite(fas_ep_gni_fmt_av_tbl, .init = fas_ep_setup_gni_fmt_av_tbl,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_ep_gni_fmt_av_tbl, getname)
{
	do_getname();
}

Test(fas_ep_gni_fmt_av_tbl, setname)
{
	do_setname();
}

Test(fas_ep_gni_fmt_av_tbl, getpeer)
{
	do_getpeer();
}

Test(fas_ep_gni_fmt_av_tbl, send_recv)
{
	do_send_recv();
}

TestSuite(fas_sep_str_fmt_av_tbl, .init = fas_sep_setup_str_fmt_av_tbl,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_sep_str_fmt_av_tbl, getname)
{
	do_getname();
}

Test(fas_sep_str_fmt_av_tbl, setname)
{
	do_setname();
}

Test(fas_sep_str_fmt_av_tbl, getpeer)
{
	do_getpeer();
}

Test(fas_sep_str_fmt_av_tbl, send_recv)
{
	do_send_recv();
}

TestSuite(fas_sep_gni_fmt_av_tbl, .init = fas_sep_setup_gni_fmt_av_tbl,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_sep_gni_fmt_av_tbl, getname)
{
	do_getname();
}

Test(fas_sep_gni_fmt_av_tbl, setname)
{
	do_setname();
}

Test(fas_sep_gni_fmt_av_tbl, getpeer)
{
	do_getpeer();
}

Test(fas_sep_gni_fmt_av_tbl, send_recv)
{
	do_send_recv();
}

TestSuite(fas_pep_str_fmt_av_tbl, .init = fas_pep_setup_str_fmt_av_tbl,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_pep_str_fmt_av_tbl, getname)
{
	do_getname();
}

Test(fas_pep_str_fmt_av_tbl, setname)
{
	do_setname();
}

Test(fas_pep_str_fmt_av_tbl, getpeer)
{
	do_getpeer_enosys();
}

TestSuite(fas_pep_gni_fmt_av_tbl, .init = fas_pep_setup_gni_fmt_av_tbl,
	  .fini = fas_teardown_common, .disabled = false);

Test(fas_pep_gni_fmt_av_tbl, getname)
{
	do_getname();
}

Test(fas_pep_gni_fmt_av_tbl, setname)
{
	do_setname();
}

Test(fas_pep_gni_fmt_av_tbl, getpeer)
{
	do_getpeer_enosys();
}

TestSuite(fas_getinfo_str_fmt, .init = fas_getinfo_setup, .fini =
	fas_getinfo_teardown, .disabled = false);

/* TODO: uncomment the ifdef below after the 1.5 release */
#if 0
Test(fas_getinfo_str_fmt, getinfo_invalid_param)
{
	do_invalid_fi_getinfo();
}
#endif

Test(fas_getinfo_str_fmt, getinfo_valid_param)
{
	do_valid_fi_getinfo_with_fas();
}
/******************************************************************************
 * End test invocation routines
 ******************************************************************************/

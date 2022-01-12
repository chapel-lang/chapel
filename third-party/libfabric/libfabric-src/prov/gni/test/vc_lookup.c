/*
 * Copyright (c) 2016-2017 Cray Inc. All rights reserved.
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

#include "gnix_ep.h"
#include "gnix_vc.h"
#include "common.h"

#include <criterion/criterion.h>

static struct fi_info *hints;
static struct fi_info *fi;
/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fid_domain *dom;
static struct fid_ep *ep;
static struct gnix_fid_ep *gnix_ep;
static struct gnix_ep_name ep_name;
static size_t ep_name_len;
static struct fid_av *av;
static struct fi_info *hints;
static struct fi_info *fi;

void vc_lookup_setup(int av_type, int av_size)
{
	int ret = 0;
	struct fi_av_attr attr;

	hints = fi_allocinfo();

	hints->mode = mode_bits;
	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->fabric_attr->prov_name = strdup("gni");

	/* Create endpoint */
	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	ret = fi_domain(fab, fi, &dom, NULL);
	cr_assert(!ret, "fi_domain");

	attr.type = av_type;
	attr.count = av_size;

	ret = fi_av_open(dom, &attr, &av, NULL);
	cr_assert(!ret, "fi_av_open");

	ret = fi_endpoint(dom, fi, &ep, NULL);
	cr_assert(!ret, "fi_endpoint");

	gnix_ep = container_of(ep, struct gnix_fid_ep, ep_fid);

	ret = fi_getname(&ep->fid, NULL, &ep_name_len);

	ret = fi_getname(&ep->fid, &ep_name, &ep_name_len);
	cr_assert(ret == FI_SUCCESS);

	ret = fi_ep_bind(ep, &av->fid, 0);
	cr_assert(!ret, "fi_ep_bind");

	ret = fi_enable(ep);
	cr_assert(!ret, "fi_ep_enable");

	fi_freeinfo(hints);
}

void vc_lookup_teardown(void)
{
	int ret = 0;

	ret = fi_close(&ep->fid);
	cr_assert(!ret, "failure in closing ep[0].");

	ret = fi_close(&av->fid);
	cr_assert(!ret, "failure in closing dom[0] av.");

	ret = fi_close(&dom->fid);
	cr_assert(!ret, "failure in closing domain dom[0].");

	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");

	fi_freeinfo(fi);
}

int _do_vc_lookup_perf(int av_type, int niters, int npeers, int naddrs)
{
	struct gnix_ep_name *addrs;
	fi_addr_t *fi_addrs;
	uint32_t i, ret, inc = npeers / naddrs;
	struct timeval s1, s2;
	int sec, usec;
	double usec_p_lookup;
	struct gnix_vc *vc;
	struct gnix_ep_name tmp_name = ep_name;

	addrs = malloc(ep_name_len * npeers);
	cr_assert(addrs, "failed to malloc addresses");

	fi_addrs = (fi_addr_t *)malloc(sizeof(fi_addr_t) * npeers);
	cr_assert(fi_addrs, "failed to malloc FI addresses");

	for (i = 0; i < npeers; i++) {
		/* insert fake addresses into AV */
		tmp_name.gnix_addr.cdm_id++;
		tmp_name.cm_nic_cdm_id++;
		addrs[i] = tmp_name;
	}

	ret = fi_av_insert(av, (void *)addrs, npeers,
			   (void *)fi_addrs, 0, NULL);
	cr_assert(ret == npeers);

	for (i = 0; i < npeers; i++) {
		/* do warump */
		ret = _gnix_vc_ep_get_vc(gnix_ep, fi_addrs[i],
					 &vc);
		cr_assert(ret == FI_SUCCESS);
	}

	gettimeofday(&s1, 0);
	for (i = 0; i < niters; i++) {
		/* do lookups */
		ret = _gnix_vc_ep_get_vc(gnix_ep, fi_addrs[(i * inc)%npeers],
					 &vc);
		/* cr_assert has ridiculous overhead */
		/* cr_assert(ret == FI_SUCCESS); */
	}
	gettimeofday(&s2, 0);

	calculate_time_difference(&s1, &s2, &sec, &usec);
	usec += sec * 1e6;
	usec_p_lookup = (double)usec;
	usec_p_lookup /= niters;

	fprintf(stderr, "type: %s\tnpeers: %7d naddrs: %7d ns: %f\n",
		av_type == 1 ? "MAP" : "TABLE", npeers, naddrs,
		usec_p_lookup*1000);

	free(fi_addrs);
	free(addrs);

	return 0;
}

int do_vc_lookup_perf(int av_type, int niters, int naddrs, int npeers)
{
	vc_lookup_setup(av_type, npeers);
	_do_vc_lookup_perf(av_type, niters, naddrs, npeers);
	vc_lookup_teardown();

	return 0;
}

#define NITERS	10000
#define EXP_INC	4
#define TESTS	5
Test(vc_lookup, perf, .disabled = true)
{
	int i, j;

	for (i = 0; i < TESTS; i++) {
		for (j = 0; j <= i; j++) {
			do_vc_lookup_perf(FI_AV_MAP, NITERS,
					  2<<(i*EXP_INC), 2<<(j*EXP_INC));
		}
	}

	for (i = 0; i < TESTS; i++) {
		for (j = 0; j <= i; j++) {
			do_vc_lookup_perf(FI_AV_TABLE, NITERS,
					  2<<(i*EXP_INC), 2<<(j*EXP_INC));
		}
	}
}


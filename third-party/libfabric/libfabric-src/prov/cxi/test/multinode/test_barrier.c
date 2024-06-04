/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2021-2023 Hewlett Packard Enterprise Development LP
 */

/**
 * Standalone BARRIER test to illustrate how to set up collectives.
 *
 * This uses the multinode_frmwk.c code to prepare a generic multinode
 * environment for libfabric collectives testing, and provides some common
 * tools for:
 *
 * - evaluating SLURM environment variables
 * - configuring a vanilla libfabric, including HMEM overlays
 * - distributing HSN addresses among the nodes
 *
 * The distribution of HSN addresses uses a linux socket-based method to
 * share the HSN addresses among the nodes, and as such, presumes the
 * existence of a standard Ethernet network linking the nodes (which is also
 * presumed by SLURM).
 *
 * This code creates a single av_set consisting of the HSN0 addresses among
 * the full set of nodes (i.e. MPI_COMM_WORLD), and then performs
 * fi_join_collective() to obtain a "multicast" address to be used in the
 * barrier operation. Note that the create_av_set() call checks for the
 * environment variable FI_COLL_FABRIC_MGR_URL. If this is set, the join will
 * attempt to use the specified fabric manager URL to set up a valid
 * multicast address in the fabric. If this environment variable is not set,
 * the join will use a "unicast" model in which all leaf nodes communicate
 * with the root, and the root communicates with the leaves.
 *
 * The unicast model is not intended to be performant; it is intended to be
 * simple, since the primary purpose of the unicast model is debugging and
 * instruction.
 */


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include <malloc.h>
#include <time.h>
#include <ofi.h>
#include <cxip.h>
#include "multinode_frmwk.h"

/**
 * Create av_set.
 *
 * fiaddrs must be in the same order across all nodes.
 *
 * rootidx is the index of the collective root for this group.
 */
int create_av_set(fi_addr_t *fiaddrs, size_t size, int rootidx,
		  struct fid_av_set **avsetp)
{
	struct cxip_comm_key comm_key = {
		.keytype = (cxip_env.coll_fabric_mgr_url) ?
			    COMM_KEY_NONE : COMM_KEY_UNICAST,
		.ucast.hwroot_idx = rootidx
	};
	struct fi_av_set_attr attr = {
		.count = 0,
		.start_addr = FI_ADDR_NOTAVAIL,
		.end_addr = FI_ADDR_NOTAVAIL,
		.stride = 1,
		.comm_key_size = sizeof(comm_key),
		.comm_key = (void *)&comm_key,
		.flags = 0,
	};
	struct fid_av_set *avset;
	int i, ret;

	*avsetp = NULL;

	if (rootidx < 0 || rootidx >= size) {
		printf("%s invalid rootidx value=%d\n", __func__, rootidx);
		return -1;
	}

	// create empty av_set (alloc and initialize to empty)
	ret = fi_av_set(cxit_av, &attr, &avset, NULL);
	if (ret) {
		printf("%s fi_av_set failed %d\n", __func__, ret);
		goto quit;
	}
	// append all addresses (in rank order) to av_set
	for (i = 0; i < size; i++) {
		ret = fi_av_set_insert(avset, fiaddrs[i]);
		if (ret) {
			printf("%s fi_av_set_insert failed %d\n", __func__, ret);
			goto quit;
		}
	}
        *avsetp = avset;
	return 0;

quit:
	printf("%s FAILED %d\n", __func__, ret);
	if (avset)
		fi_close(&avset->fid);
	return ret;
}

/**
 * Poll the cqs once, and fill out a cqd structure.
 *
 * Note that the cqd is the largest supported cqd structure, so it can serve
 * as both the success (smaller) or failure (larger) return structure.
 *
 * The rx_cq has the discard flag set for all operational modes other than
 * the COMM_KEY_RANK simulation, so rx_cq events are not generated in this
 * test code. Should this be expanded to use the COMM_KEY_RANK simulation
 * (e.g. for automated single-node regression testing), the rx_cq should be
 * read and the data discarded.
 */
static ssize_t _poll_cqs(struct fi_cq_err_entry *pcqd)
{
	ssize_t size;

#if 1
	/* read/discard rx_cq -- needed in COMM_KEY_RANK simulation only */
	size = fi_cq_read(cxit_rx_cq, pcqd, 1);
	if (size == -FI_EAVAIL)
		size = fi_cq_readerr(cxit_rx_cq, pcqd, 1);
#endif

	/* tx_cq indicates barrier status */
	size = fi_cq_read(cxit_tx_cq, pcqd, 1);
	if (size == -FI_EAVAIL) {
		size = fi_cq_readerr(cxit_tx_cq, pcqd, 1);
		if (size >= 0)
			size = -FI_EAVAIL;
	}

	return size;
}

static ssize_t _wait_cqs(struct fi_cq_err_entry *pcqd)
{
	ssize_t size;

	do {
		size = _poll_cqs(pcqd);
	} while (size == -FI_EAGAIN);
	return size;
}

/**
 * Poll the endpoint EQ once, and fill out an eqd data structure.
 */
static ssize_t _poll_eq(uint32_t *pevent, struct fi_eq_err_entry *peqd) {
	struct cxip_ep *ep;
	struct fid_eq *eq;
	ssize_t size;

	ep = container_of(cxit_ep, struct cxip_ep, ep);
	eq = &ep->ep_obj->coll.eq->util_eq.eq_fid;
	size = fi_eq_read(eq, pevent, peqd, sizeof(*peqd), 0);
	if (size == -FI_EAVAIL) {
		size = fi_eq_readerr(eq, peqd, 0);
		if (size >= 0)
			size = -FI_EAVAIL;
	}
	return size;
}

static ssize_t _wait_eq(uint32_t *pevent, struct fi_eq_err_entry *peqd) {
	ssize_t size;

	do {
		size = _poll_eq(pevent, peqd);
	} while (size == -FI_EAGAIN);
	return size;
}

/**
 * Join the specified avset to create a multicast reference pointer.
 *
 * This is implemented as a blocking call, for simplicity. In practice,
 * multiple join operations can be initiated, and then the wait can be called
 * until all joins have completed. Note that completion can occur in any
 * order, and the resulting eqd.context value must be checked to see which of
 * the joins completed. We are simply using the avset pointer itself as the
 * context, but in production, this could be any kind of unique pointer or
 * index.
 */
int join_collective(struct fid_av_set *avset, struct fid_mc **mcp)
{
	struct fi_cq_err_entry cqd;
	struct fi_eq_err_entry eqd;
	uint32_t event;
	ssize_t ret;

	do {
		_poll_cqs(&cqd);
		ret = fi_join_collective(cxit_ep, FI_ADDR_NOTAVAIL,
						avset, 0L, mcp, avset);
	} while (ret == -FI_EAGAIN);
	if (ret) {
		frmwk_log("join initiation error = %ld\n", ret);
		return ret;
	}

	ret = _wait_eq(&event, &eqd);
	if (ret < 0) {
		frmwk_log("join wait error = %ld\n", ret);
		return ret;
	}
	if (event != FI_JOIN_COMPLETE) {
		frmwk_log("join event = %d != %d\n", event, FI_JOIN_COMPLETE);
		return -FI_EADDRNOTAVAIL;
	}

	return FI_SUCCESS;
}

/**
 * Perform N barriers in sequence.
 *
 * delay is a limit for a random delay inserted before each barrier is
 * initiated. The srand() function has initialized rand() to different seeds
 * based on the rank, so this serves to ensure that the ranks initiate
 * barriers out-of-sync in different orders, i.e. sometimes the root will go
 * first, sometimes a leaf will go first.
 */
int barrier(struct fid_mc *mc, int N, int delay)
{
	struct timespec t0, t1;
	uint64_t icontext;
	uint64_t wcontext;
	struct fi_cq_err_entry cqd;
	int i, ret;

	srand(100*frmwk_rank);
	icontext = 0x1000;
	clock_gettime(CLOCK_MONOTONIC, &t0);
	for (i = 0; i < N; i++) {
		do {
			if (delay)
				usleep(rand() % delay);
			ret = fi_barrier(cxit_ep, (fi_addr_t)mc,
						(void *)icontext);
		} while (ret == -FI_EAGAIN);
		frmwk_log("fi_barrier(%08lx) = %d\n", icontext, ret);
		if (ret != FI_SUCCESS)
			break;

		ret = _wait_cqs(&cqd);
		wcontext = (ret > 0) ? (uint64_t)cqd.op_context : -1L;
		frmwk_log("wait_cqs(%08lx) = %d\n", wcontext, ret);
		icontext++;
	}
	clock_gettime(CLOCK_MONOTONIC, &t1);
	if (t1.tv_nsec < t0.tv_nsec) {
		t1.tv_nsec += 1000000000;
		t1.tv_sec--;
	}
	t1.tv_nsec -= t0.tv_nsec;
	t1.tv_sec -= t0.tv_sec;
	if (i < N) {
		frmwk_log0("failed after %d barriers\n", i);
		return -1;
	}
	frmwk_log0("%d barriers completed in %ld.%09ld sec\n", N,
		   t1.tv_sec, t1.tv_nsec);
	return FI_SUCCESS;
}

int barrier2(struct fid_mc *mc, int N)
{
	struct timespec t0, t1;
	uint64_t icontext;
	uint64_t wcontext;
	struct fi_cq_err_entry cqd;
	int started, pending, blocked;
	int ret;

	clock_gettime(CLOCK_MONOTONIC, &t0);
	started = 0;
	pending = 0;
	blocked = 0;
	icontext = 0x1000;
	while (started < N) {
		/* start barriers until blocked by -FI_EAGAIN */
		ret = fi_barrier(cxit_ep, (fi_addr_t)mc, (void *)icontext);
		if (ret == FI_SUCCESS) {
			started++;
			pending++;
			blocked = 0;
			frmwk_log("fi_barrier[%08lx] started=%d pending=%d\n",
				  icontext, started, pending);
			icontext++;
			continue;
		}
		if (ret != -FI_EAGAIN) {
			frmwk_log("fi_barrier[%08lx] = %d (failed)\n", icontext, ret);
			break;
		}
		if (!blocked++)
			frmwk_log("fi_barrier[%08lx] blocked\n", icontext);
		/* poll for one barrier */
		ret = _poll_cqs(&cqd);
		wcontext = (ret > 0) ? (uint64_t)cqd.op_context : -1L;
		if (ret > 0) {
			if (ret > 1)
				frmwk_log("poll returned %d unexpected\n", ret);
			pending -= ret;
			blocked = 0;
			frmwk_log("_poll_cqs[%08lx], pending = %d\n",
				  wcontext, pending);
			continue;
		}
		if (ret != -FI_EAGAIN) {
			frmwk_log("_poll_cqs = %d (failed)\n", ret);
			break;
		}
	}
	frmwk_log("started %d of %d, pending %d\n", started, N, pending);
	if (started < N) {
		frmwk_log("failed\n");
		return -1;
	}
	while (pending > 0) {
		ret = _poll_cqs(&cqd);
		wcontext = (ret > 0) ? (uint64_t)cqd.op_context : -1L;
		if (ret > 0) {
			pending -= ret;
			frmwk_log("wait_cqs[%08lx], pending = %d\n",
				  wcontext, pending);
			continue;
		}
		if (ret != -FI_EAGAIN) {
			frmwk_log("_poll_cqs = %d\n", ret);
			break;
		}
	}
	frmwk_log("completed %d\n", started);
	clock_gettime(CLOCK_MONOTONIC, &t1);
	if (t1.tv_nsec < t0.tv_nsec) {
		t1.tv_nsec += 1000000000;
		t1.tv_sec--;
	}
	t1.tv_nsec -= t0.tv_nsec;
	t1.tv_sec -= t0.tv_sec;
	if (started < N) {
		frmwk_log("_wait_cqs() = %d\n", ret);
		frmwk_log("failed after %d barriers\n", started);
		return -1;
	}
	frmwk_log("%d barriers completed in %ld.%09ld sec\n", N,
		   t1.tv_sec, t1.tv_nsec);
	return FI_SUCCESS;
}

const char *helpstr =
	"\n"
	"-N specifies the number of barriers to perform, default=1\n"
	"-R specifies the rank to be used as the root, default=0\n"
	"-D specifies a random max delay in usec, default=0\n"
	"-p parallel barriers\n"
	"\n";

int main(int argc, char **argv)
{
	fi_addr_t *fiaddrs = NULL;
	size_t size = 0;
	int rootidx = 0;
	struct fid_av_set *avset = NULL;
	struct fid_mc *mc = NULL;
	int N = 1;
	bool parallel = false;
	int delay = 0;
	int help = 0;
	int opt, ret;

	while ((opt = getopt(argc, argv, "hpN:R:D:")) != -1) {
		switch (opt) {
		case 'p':
			parallel = true;
			break;
		case 'N':
			N = atoi(optarg);
			break;
		case 'R':
			rootidx = atoi(optarg);
			break;
		case 'D':
			delay = atoi(optarg);
			break;
		case 'h':
		default:
			help = 1;
			ret = (opt == 'h') ? 0 : 1;
			break;
		}
	}

	/* Read environment variables and initialize frmwk memory */
	frmwk_init(help);
	if (help) {
		frmwk_log0("Usage: %s [-h] [-N iterations]\n"
			   "       [-R root_rank] [-D usec] [-p]\n",
			   basename(argv[0]));
		frmwk_log0("%s", helpstr);
		return ret;
	}

	/* Test requires a minimum of two nodes */
	if (frmwk_check_env(2))
		return -1;

	/* Must be done before populting AV */
	ret = frmwk_init_libfabric();
	if (ret)
		goto quit;

	/* Acquire HSN0 addresses and distribute across job */
	ret = frmwk_populate_av(&fiaddrs, &size);
	if (ret)
		goto quit;

	/* Create the MPI_COMM_WORLD group */
	ret = create_av_set(fiaddrs, size, rootidx, &avset);
	if (ret)
		goto quit;

	/* Create the collective multicast identifier */
	ret = join_collective(avset, &mc);
	if (ret)
		goto quit;

	/* Perform N barriers */
	ret = (parallel) ? barrier2(mc, N) : barrier(mc, N, delay);
	if (ret)
		goto quit;

quit:
	if (mc)
		fi_close(&mc->fid);
	if (avset)
		fi_close(&avset->fid);
	free(fiaddrs);
	frmwk_free_libfabric();
	frmwk_term();
	return ret;
}

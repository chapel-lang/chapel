/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2021-2023 Hewlett Packard Enterprise Development LP
 */

/*
 * Validation test for the multinode coll implementation.
 *
 * Launch using: srun -N4 ./test_coll [args]
 * Note that -N4 is the minimum. There is no maximum.
 */

/**
 * Test the coll functions in a real environment.
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

/* If not compiled with DEBUG=1, this is a no-op */
#define	TRACE(fmt, ...)	CXIP_COLL_TRACE(CXIP_TRC_TEST_CODE, fmt, ##__VA_ARGS__)

/* convert delays to nsecs */
#define	nUSEC(n)	(n * 1000L)
#define nMSEC(n)	(n * 1000000L)
#define	nSEC(n)		(n * 1000000000L)

int verbose;
bool create_multicast;

/* Signaling NaN generation, for testing.
 * Linux feature requires GNU_SOURCE.
 * This generates a specific sNaN value.
 */
static inline double cxip_snan64(void)
{
	return _bits2dbl(0x7ff4000000000000);
}

/* initialize nsecs timer structure */
static inline void _init_nsecs(struct timespec *tsp)
{
	clock_gettime(CLOCK_MONOTONIC, tsp);
}

/* return elapsed nsecs since initialized tsp */
static inline long _measure_nsecs(struct timespec *tsp)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	ts.tv_nsec -= tsp->tv_nsec;
	ts.tv_sec -= tsp->tv_sec;
	if (ts.tv_nsec < 0) {
		ts.tv_nsec += 1000000000L;
		ts.tv_sec -= 1;
	}
	return 1000000000L*ts.tv_sec + ts.tv_nsec;
}

static inline void _nsecs_from_now(struct timespec *tsp, long nsecs)
{
	long secs = (nsecs/1000000000L);

	nsecs %= 1000000000L;
	clock_gettime(CLOCK_MONOTONIC, tsp);
	tsp->tv_nsec += nsecs;
	tsp->tv_sec += secs;
	if (tsp->tv_nsec > 1000000000L) {
		tsp->tv_nsec -= 1000000000L;
		tsp->tv_sec += 1;
	}
}

static inline bool _nsecs_expired(const struct timespec *tsp)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	if (ts.tv_sec < tsp->tv_sec)
		return false;
	if (ts.tv_sec == tsp->tv_sec &&
	    ts.tv_nsec < tsp->tv_nsec)
		return false;
	return true;
}

/* poll rx and tx cqs once to drive I/O and return completion context */
static void *_poll_cqs(void)
{
	struct fi_cq_err_entry cqd;
	ssize_t size;

	size = fi_cq_read(cxit_rx_cq, &cqd, 1);
	if (size == -FI_EAVAIL)
		size = fi_cq_readerr(cxit_rx_cq, &cqd, 1);
	if (size > 0) {
		TRACE("rx event seen\n");
		TRACE("  size          %ld\n",size);
		TRACE("  buf           %p\n",cqd.buf);
		TRACE("  data          %016lx\n",cqd.data);
		TRACE("  err           %d\n",cqd.err);
		TRACE("  err_data      %p\n",cqd.err_data);
		TRACE("  err_data_size %ld\n",cqd.err_data_size);
		TRACE("  flags         %016lx\n",cqd.flags);
		TRACE("  len           %ld\n",cqd.len);
		TRACE("  olen          %ld\n",cqd.olen);
		TRACE("  op_context    %p\n",cqd.op_context);
		TRACE("  prov_errno    %d\n",cqd.prov_errno);
		TRACE("  tag           %016lx\n",cqd.tag);
	} else if (size != -FI_EAGAIN)
		TRACE("rx ERROR seen = %ld\n", size);

	size = fi_cq_read(cxit_tx_cq, &cqd, 1);
	if (size == -FI_EAVAIL)
		size = fi_cq_readerr(cxit_tx_cq, &cqd, 1);
	if (size > 0) {
		TRACE("tx event seen\n");
		TRACE("  size          %ld\n",size);
		TRACE("  buf           %p\n",cqd.buf);
		TRACE("  data          %016lx\n",cqd.data);
		TRACE("  err           %d\n",cqd.err);
		TRACE("  err_data      %p\n",cqd.err_data);
		TRACE("  err_data_size %ld\n",cqd.err_data_size);
		TRACE("  flags         %016lx\n",cqd.flags);
		TRACE("  len           %ld\n",cqd.len);
		TRACE("  olen          %ld\n",cqd.olen);
		TRACE("  op_context    %p\n",cqd.op_context);
		TRACE("  prov_errno    %d\n",cqd.prov_errno);
		TRACE("  tag           %016lx\n",cqd.tag);
		return cqd.op_context;
	}
	if (size != -FI_EAGAIN)
		TRACE("tx ERROR seen = %ld\n", size);
	return NULL;
}

/* blocking wait for single collective op completion */
static void _wait_cqs(void *pcontext)
{
	TRACE("Wait for context %p\n", pcontext);
	do {
		if (pcontext == _poll_cqs())
			break;
	} while (true);
}

/**
 * @brief Manage multiple av_sets.
 *
 * The avset_ary is an ordered list of different av_set objects, each of which
 * represents a specific collective group.
 *
 * In this test framework, the multi join operation will create an
 * independent join (and mc object) for EACH av_set in the avset_ary, to be
 * initiated concurrently. Joins will be initiated in the list order, and
 * completed in an arbitrary order.
 *
 * Hint: fid_av_set consists of just a constant self-size value, and a list
 * of function pointers. It is contained inside cxip_av_set, which contains
 * the addresses, address counts, etc. You need to take container_of() on the
 * fid_av_set pointer to get the containing cxip_av_set. Real (non-test)
 * users will not need this extra information.
 */
struct avset_ary {
	struct fid_av_set **avset;
	int avset_cnt;
	int avset_siz;
};

void avset_ary_init(struct avset_ary *setary)
{
	setary->avset = NULL;
	setary->avset_cnt = 0;
	setary->avset_siz = 0;
}

void avset_ary_destroy(struct avset_ary *setary)
{
	int i;

	if (setary->avset) {
		for (i = 0; i < setary->avset_cnt; i++)
			fi_close(&setary->avset[i]->fid);
		free(setary->avset);
	}
	avset_ary_init(setary);
}

/* create a single avset using fiaddrs, size, and append it to the setary.
 * mcast_addr and root_idx apply only to UNICAST model.
 */
int avset_ary_append(fi_addr_t *fiaddrs, size_t size,
		     int mcast_addr, int root_idx,
		     struct avset_ary *setary)
{
	struct cxip_comm_key comm_key = {
		.keytype = (cxip_env.coll_fabric_mgr_url && create_multicast) ?
			    COMM_KEY_NONE : COMM_KEY_UNICAST,
		.ucast.mcast_addr = mcast_addr,
		.ucast.hwroot_idx = root_idx
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
	struct fid_av_set *setp;
	int i, ret;

	// expand accumulator list as necessary
	TRACE("%s cnt=%d siz=%d multicast=%d\n", __func__, setary->avset_cnt,
	       setary->avset_siz, create_multicast);
	if (setary->avset_siz <= setary->avset_cnt) {
		void *ptr;
		int siz;

		TRACE("%s expand setary\n", __func__);
		siz = setary->avset_siz + 4;
		ptr = realloc(setary->avset, siz * sizeof(void *));
		if (!ptr) {
			TRACE("%s realloc failed\n", __func__);
			ret = -FI_ENOMEM;
			goto quit;
		}
		setary->avset_siz = siz;
		setary->avset = ptr;
	}
	// create empty av_set (alloc and initialize to empty)
	ret = fi_av_set(cxit_av, &attr, &setp, NULL);
	if (ret) {
		TRACE("%s fi_av_set failed %d\n", __func__, ret);
		goto quit;
	}
	// append addresses to av_set
	for (i = 0; i < size; i++) {
		ret = fi_av_set_insert(setp, fiaddrs[i]);
		if (ret) {
			TRACE("%s fi_av_set_insert failed %d\n", __func__, ret);
			goto quit;
		}
	}
	// add to expanded list
	setary->avset[setary->avset_cnt++] = setp;
	return 0;

quit:
	TRACE("%s: FAILED %d\n", __func__, ret);
	if (setp) {
		fi_close(&setp->fid);
		free(setp);
	}
	return ret;
}

/**
 * @brief Perform concurrent joins over avset_ary objects.
 *
 * A single multi-join will initiate concurrent join operations over each of
 * the av_set objects in the avset_ary.
 *
 * Each join is represented by a join_item, which contains a pointer to the
 * generating av_set, and the resulting mc object. It also records a
 * completion result and a provider error (if any). The join_items are linked
 * to a dlist called the joinlist.
 *
 * A multi-join can be called multiple times for the same joinlist, and will
 * continue add join_items to the joinlist.
 *
 * If the av_set objects are all disjoint, joins should proceed in parallel.
 * If the av_set objects overlap, the first join will proceed, and subsequent
 * joins will return -FI_EAGAIN until the blocking zbcoll getgroup operation
 * completes, after which they will proceed in parallel. If the maximum
 * zbcoll groupid value is acquired, all join operations will be blocked
 * until at least one join operation completes, freeing a zbcoll groupid.
 *
 * Proper behavior is dependent on initiating all joins in the same relative
 * order on every participating endpoint, which is a general MPI requirement
 * for all collective operations.
 *
 * This returns when all joins specified in the setary have been initiated.
 *
 * Note that fi_join_collective() can be called from an endpoint that is not
 * a valid endpoint in the collective group. These tests, in fact, will call
 * fi_join_collective() for every endpoint in the WLM job, even if the av_set
 * represents some subset of this. The call will return the value
 * -FI_ECONNREFUSED for endpoints that do not belong to the collective
 * group, and this causes the join structure to be discarded without adding
 * it to the result joinlist. This means that when doing a multijoin,
 * different endpoints may have different joinlist lengths.
 *
 * A join failure on an endpoint that is part of the collective group will
 * result in an error propagated to all members of that group through zbcoll,
 * so all endpoints will fail the join operation with the same error code.
 */
struct join_item {
	struct dlist_entry entry;
	struct fid_av_set *avset;
	struct fid_mc *mc;
	int prov_errno;
	int retval;
	int trace_no;
};

/* poll the collective eq once, count of completions (0 or 1) */
static int _poll_eq(void)
{
	struct cxip_ep *ep;
	struct fid_eq *eq;
	struct fi_eq_err_entry eqd = {};
	struct join_item *jctx;
	uint32_t event;
	int ret;

	ep = container_of(cxit_ep, struct cxip_ep, ep);
	eq = &ep->ep_obj->coll.eq->util_eq.eq_fid;

	jctx = NULL;
	ret = fi_eq_read(eq, &event, &eqd, sizeof(eqd), 0);
	if (ret >= 0) {
		TRACE("read EQ = %d\n", ret);
		if (ret < sizeof(struct fi_eq_entry)) {
			TRACE("fi_eq_read()=%d, exp=%ld\n",
				ret, sizeof(struct fi_eq_entry));
			return -FI_EINVAL;
		}
		TRACE("=== EQ SUCCESS\n");
		TRACE("  size    = %d\n", ret);
		TRACE("  event   = %d\n", event);
		TRACE("  fid     = %p\n", eqd.fid);
		TRACE("  context = %p\n", eqd.context);
		TRACE("  data    = %lx\n", eqd.data);
		if (eqd.context && event == FI_JOIN_COMPLETE) {
			jctx = eqd.context;
			jctx->retval = 0;
			jctx->prov_errno = 0;
			return 1;
		}
	}
	if (ret == -FI_EAVAIL) {
		TRACE("read EQ = %d\n", ret);
		ret = fi_eq_readerr(eq, &eqd, 0);
		if (ret < sizeof(struct fi_eq_err_entry)) {
			TRACE("fi_eq_readerr()=%d, exp=%ld\n",
			      ret, sizeof(struct fi_eq_err_entry));
			return -FI_EINVAL;
		}
		TRACE("=== EQ error available\n");
		TRACE("  size    = %d\n", ret);
		TRACE("  event   = %d\n", event);
		TRACE("  fid     = %p\n", eqd.fid);
		TRACE("  context = %p\n", eqd.context);
		TRACE("  data    = %lx\n", eqd.data);
		TRACE("  err     = %s (%d)\n",
			fi_strerror(-eqd.err), eqd.err);
		TRACE("  prov_err= %d\n", eqd.prov_errno);
		TRACE("  err_data= %p\n", eqd.err_data);
		TRACE("  err_size= %ld\n", eqd.err_data_size);
		if (eqd.context) {
			jctx = eqd.context;
			jctx->retval = eqd.err;
			jctx->prov_errno = eqd.prov_errno;
			return 1;
		}
	}
	if (ret != -FI_EAGAIN) {
		TRACE("read EQ = %d\n", ret);
		TRACE("=== EQ other\n");
		TRACE("  size    = %d\n", ret);
		TRACE("  event   = %d\n", event);
	}
	return 0;
}

/* close a list of collectives */
void coll_multi_release(struct dlist_entry *joinlist)
{
	struct join_item *jctx;

	TRACE("coll_multi_release\n");
	while (!dlist_empty(joinlist)) {
		dlist_pop_front(joinlist, struct join_item, jctx, entry);
		TRACE("close mc, empty = %d\n", dlist_empty(joinlist));
		if (jctx->mc)
			fi_close(&jctx->mc->fid);
		TRACE("free jctx\n");
		free(jctx);
	}
	TRACE("return\n");
}

/* initiate join on all sets in setary, and append to joinlist */
int coll_multi_join(struct avset_ary *setary, struct dlist_entry *joinlist)
{
	struct join_item *jctx;
	int i, ret, total, count;

	TRACE("ENTRY %s\n", __func__);

	// perform collective joins from setary
	total = setary->avset_cnt;
	count = 0;
	for (i = 0; i < total; i++) {
		jctx = calloc(1, sizeof(*jctx));
		jctx->trace_no = i;
		jctx->avset = setary->avset[i];
		dlist_init(&jctx->entry);
		TRACE("join %d of %d initiating\n", i, total);
		ret = fi_join_collective(cxit_ep, FI_ADDR_NOTAVAIL,
					 setary->avset[i], 0L, &jctx->mc, jctx);
		/* node is not participating in this join */
		if (ret == -FI_ECONNREFUSED) {
			free(jctx);
			continue;
		}
		TRACE("join %d continuing ret=%d\n", i, ret);
		if (ret != FI_SUCCESS) {
			TRACE("join %d FAILED\n", ret);
			goto fail;
		}
		/* wait for join to complete */
		do {
			_poll_cqs();
			ret = _poll_eq();
		} while (ret == 0);
		dlist_insert_tail(&jctx->entry, joinlist);
		count++;
	}
	TRACE("DONE %s completed %d joins\n", __func__, count);
	return FI_SUCCESS;

fail:
	TRACE("TEST failed\n");
	coll_multi_release(joinlist);
	return ret;
}

/* Perform cleanup on a multijoin */
void coll_join_cleanup(struct avset_ary *setary, struct dlist_entry *joinlist)
{
	coll_multi_release(joinlist);
	avset_ary_destroy(setary);
}

struct join_item *_get_join_jctx(struct dlist_entry *joinlist, int index)
{
	struct join_item *jctx;

	dlist_foreach_container(joinlist, struct join_item, jctx, entry) {
		if (!index--)
			return jctx;
	}
	return NULL;
}

bool _is_hwroot(struct join_item *jctx)
{
	struct cxip_coll_mc *mc_obj;

	mc_obj = (struct cxip_coll_mc *)jctx->mc;
	return (mc_obj->hwroot_idx == mc_obj->mynode_idx);
}

/* Utility function to create a single join with no errors.
 * mcast_addr and root_idx apply only to UNICAST model.
 * Allows join error conditions to be tested.
 */
struct join_item *coll_single_join(fi_addr_t *fiaddrs, size_t size,
				   int mcast_addr, int root_idx,
				   int exp_retval, int exp_prov_errno,
				   struct avset_ary *setary,
				   struct dlist_entry *joinlist,
				   const char *msg)
{
	struct join_item *jctx = NULL;
	int ret;

	avset_ary_init(setary);
	ret = avset_ary_append(fiaddrs, size, mcast_addr, root_idx, setary);
	if (ret) {
		TRACE("%s JOIN avset_ary_append()=%d\n", msg, ret);
		goto quit;
	}

	dlist_init(joinlist);
	ret = coll_multi_join(setary, joinlist);
	if (ret) {
		TRACE("%s JOIN coll_multi_join()=%d\n", msg, ret);
		goto quit;
	}

	jctx = dlist_first_entry_or_null(joinlist, struct join_item, entry);
	if (!jctx) {
		TRACE("%s JOIN produced NULL result\n", msg);
		goto quit;
	}

	if (jctx->retval != exp_retval || jctx->prov_errno != exp_prov_errno) {
		TRACE("%s JOIN ret=%d,exp=%d prov_errno=%d,exp=%d\n", msg,
		      jctx->retval, exp_retval,
		      jctx->prov_errno, exp_prov_errno);
		goto quit;
	}
	TRACE("%s JOIN SUCCESS\n", msg);
	return jctx;

quit:
	TRACE("%s JOIN FAILED\n", msg);
	coll_join_cleanup(setary, joinlist);
	return NULL;
}

#if 0
int _test_multi_barrier(struct avset_ary *setary, struct dlist_entry *joinlist,
			int N, long *nsec_delay, int total_secs)
{
	struct timespec *nsec_times, nsec_start;
	int i, ret;

	nsec_times = calloc(sizeof(struct timespec), N);
	ret = coll_init_multi_join(setary, joinlist);
	if (ret) {
		TRACE("multicast_join init error = %d\n", ret);
		goto quit;
	}
	ret = coll_wait_multi_join(joinlist);
	if (ret) {
		TRACE("multicast_join wait error = %d\n", ret);
		goto quit;
	}

	_nsecs_from_now(&nsec_start, 0L);
	nsec_start.tv_sec += total_secs;

	for (i = 0; i < N; i++)
		_nsecs_from_now(&nsec_times[i], nsec_delay[i]);
	while (!_nsecs_expired(&nsec_start)) {
		for (i = 0; i < N; i++) {
			if (!_nsecs_expired(&nsec_times[i]))
				continue;
			for (j = 0; j < )
		}

	}
quit:
	free(nsec_times);
	coll_multi_releasejoinlist);
	avset_ary_destroy(setary);
	return ret;
}
#endif

int _simple_join(fi_addr_t *fiaddrs, size_t size,
		 struct avset_ary *setary,
		 struct dlist_entry *joinlist)
{
	int ret;

	avset_ary_init(setary);
	ret = avset_ary_append(fiaddrs, size, 0, 1, setary);
	if (ret)
		return ret;

	dlist_init(joinlist);
	ret = coll_multi_join(setary, joinlist);
	if (ret)
		return ret;

	return 0;
}

uint64_t _simple_get_mc(struct dlist_entry *joinlist)
{
	struct join_item *jctx;

	jctx = dlist_first_entry_or_null(joinlist, struct join_item, entry);
	return (uint64_t)jctx->mc;
}

void _simple_join_release(struct avset_ary *setary,
			  struct dlist_entry *joinlist)
{
	coll_multi_release(joinlist);
	avset_ary_destroy(setary);
}

/**
 * @brief Simple test of join/delete returns a count of errors.
 *
 * This creates a single avset_ary from the supplied addresses, with hwroot
 * of zero, and performs a single join, tests errors, and cleans up. Used to
 * probe the basic error conditions.
 */
int _test_join(fi_addr_t *fiaddrs, size_t size)
{
	struct avset_ary setary;
	struct dlist_entry joinlist;
	int ret;

	ret = _simple_join(fiaddrs, size, &setary, &joinlist);
	_simple_join_release(&setary, &joinlist);

	return ret;
}

/* Simple test of barrier, returns a count of errors. */
int _test_barrier(fi_addr_t *fiaddrs, size_t size, int count)
{
	struct avset_ary setary;
	struct dlist_entry joinlist;
	uint64_t context;
	uint64_t mc;
	int i, ret, total;

	TRACE("%s entry, create_mcast=%d\n", __func__, create_multicast);
	ret = _simple_join(fiaddrs, size, &setary, &joinlist);
	if (ret) {
		TRACE("BARRIER JOIN failed\n");
		goto quit;
	}
	TRACE("BARRIER JOIN COMPLETE\n");

	mc = _simple_get_mc(&joinlist);
	if (!mc) {
		TRACE("BARRIER MC invalid\n");
		goto quit;
	}
	for (i = 0; i < count; i++) {
		do {
			usleep(rand() % 100);
			ret = fi_barrier(cxit_ep, mc, &context);
		} while (ret == -FI_EAGAIN);
		TRACE("barrier = %d\n", ret);
		if (ret == FI_SUCCESS) {
			TRACE("spin 1...\n");
			_wait_cqs(&context);
			TRACE("BARRIER COMPLETE #%d\n", i);
			total++;
		} else {
			TRACE("BARRIER FAILED   #%d, ret=%d\n", i, ret);
			goto quit;
		}
	}
	ret = 0;

quit:
	TRACE("BARRIER exit\n");
	frmwk_log0("Barrier total=%d\n", total);
	_simple_join_release(&setary, &joinlist);
	return ret;
}

/* Simple test of broadcast, returns a count of errors. */
int _test_broadcast(fi_addr_t *fiaddrs, size_t size, int rootidx)
{
	struct avset_ary setary;
	struct dlist_entry joinlist;
	uint64_t data[4], rslt[4];
	uint64_t context;
	uint64_t mc;
	int i, ret;

	TRACE("%s entry, create_mcast=%d\n", __func__, create_multicast);

	ret = _simple_join(fiaddrs, size, &setary, &joinlist);
	if (ret) {
		TRACE("join failed\n");
		goto quit;
	}

	mc = _simple_get_mc(&joinlist);
	if (!mc) {
		TRACE("BARRIER MC invalid\n");
		ret = -1;
		goto quit;
	}

	data[0] = 0x12345678;
	data[1] = 0x2468ace0;
	data[2] = 0x13579bdf;
	data[3] = 0x10101010;
	memset(rslt, 0, sizeof(rslt));
	if (frmwk_rank == rootidx)
		memcpy(rslt, data, sizeof(rslt));
	do {
		_poll_cqs();
		ret = fi_broadcast(cxit_ep, rslt, 4, NULL,
				   mc, fiaddrs[rootidx],
				   FI_UINT64, 0L, &context);
	} while (ret == -FI_EAGAIN);
	if (ret)
		goto quit;

	TRACE("spin 1...\n");
	_wait_cqs(&context);
	TRACE("BROADCAST COMPLETE\n");
	if (memcmp(rslt, data, sizeof(rslt))) {
		for (i = 0; i < 4; i++)
			TRACE("[%d] %016lx exp %016lx\n",
				i, rslt[i], data[i]);
		ret = -1;
	}

quit:
	TRACE("BROADCAST exit\n");
	_simple_join_release(&setary, &joinlist);
	return ret;
}

const struct timespec usec1 = {.tv_sec = 0, .tv_nsec = 10000};

/* simple test of allreduce, returns a count of errors. */
int _test_allreduce(fi_addr_t *fiaddrs, size_t size)
{
	struct avset_ary setary;
	struct dlist_entry joinlist;
	int64_t *data, *rslt, *comp;
	uint64_t context;
	uint64_t mc;
	int r, v, ret;

	TRACE("%s entry, create_mcast=%d\n", __func__, create_multicast);

	ret = _simple_join(fiaddrs, size, &setary, &joinlist);
	if (ret) {
		TRACE("join failed\n");
		goto quit;
	}

	mc = _simple_get_mc(&joinlist);
	if (!mc) {
		TRACE("ALLREDUCE MC invalid\n");
		ret = -1;
		goto quit;
	}
	if (_is_hwroot(_get_join_jctx(&joinlist, 0)))
		nanosleep(&usec1, NULL);

	data = calloc(frmwk_numranks*4, sizeof(int64_t));
	comp = calloc(4, sizeof(int64_t));
	rslt = calloc(4, sizeof(int64_t));
	for (v = 0; v < 4; v++)
		for (r = 0; r < frmwk_numranks; r++)
			data[4*r + v] = 4*r  + v;
	for (v = 0; v < 4; v++)
		for (r = 0; r < frmwk_numranks; r++)
			comp[v] += data[4*r + v];
	do {
		_poll_cqs();
		ret = fi_allreduce(cxit_ep, &data[frmwk_rank*4], 4, NULL,
				   rslt, NULL, mc, FI_INT64,
				   FI_SUM, 0L, &context);
	} while (ret == -FI_EAGAIN);
	if (ret)
		goto quit;

	TRACE("spin...\n");
	_wait_cqs(&context);
	TRACE("ALLREDUCE COMPLETE\n");
	for (v = 0; v < 4; v++) {
		if (rslt[v] != comp[v]) {
			TRACE("[%d] %016lx exp %016lx\n",
				v, rslt[v], comp[v]);
			ret = 1;
		}
	}
	free(rslt);
	free(comp);
	free(data);

quit:
	TRACE("ALLREDUCE exit\n");
	_simple_join_release(&setary, &joinlist);
	return ret;
}

/**
 * Main application.
 *
 * ./test_coll -h or srun -Nx ./test_coll -h displays syntax and a list of
 * tests.
 *
 * Each test is bracketed by do {...} while(0) and will be evaluated against
 * the test mask created by the -t argument. If the test isn't in the -t
 * selection, then the test is silently skipped. Tests can be easily
 * rearranged or modified by adding new do {...} while(0) test cases. Each
 * should begin with PREAMBLE(), which manages the argument handling.
 *
 * Each test case should end with frmwk_barrier(), which uses the framework()
 * sockets-based barrier to ensure separation of the test cases.
 */

static uint64_t testmask = 0L;

#define TAG(skip)	(skip ? "SKIP " : "----")
#define	TEST(n)		(1 << n)
#define	STDMSG(ret)	((ret > 0) ? "SKIP" : ((ret) ? "FAIL" : "good"))
#define PREAMBLE(skip, num, nam) \
	ret = 1; \
	testname = nam; \
	if (help) { \
		frmwk_log0("%2d: %s\n", num, testname); break; \
	}; \
	if (!(testmask & TEST(num))) break; \
	frmwk_log0("%4s %2d:%s\n", TAG(skip), num, testname); \
	TRACE("%4s %2d:%s\n", TAG(skip), num, testname); \
	if (skip) break; \
	ret = 0

int main(int argc, char **argv)
{
	fi_addr_t *fiaddrs = NULL;
	fi_addr_t myaddr;
	struct cxip_addr mycaddr;
	size_t mycaddr_len;
	size_t size = 0;
	int errcnt = 0;
	int tstcnt = 0;
	int tstnum = 0;
	int ret = 0;
	int N = 0;
	int S = 1;
	bool help = false;
	bool trace_muted = true;
	struct join_item *jctx;
	struct avset_ary setary;
	struct dlist_entry joinlist;
	const char *testname;
	char opt;
	int i, j;

	/* by default, perform all tests */
	testmask = -1L;
	testname = NULL;

	setvbuf(stdout, NULL, _IONBF, 0);
	while ((opt = getopt(argc, argv, "hvVS:Mt:N:")) != -1) {
		char *str, *s, *p;

		switch (opt) {
		case 't':
			/* perform only selected tests */
			str = optarg;
			i = j = 0;
			testmask = 0L;
			while (*str) {
				while (*str == ' ')
					str++;
				s = str;
				while (*str && *str != ',')
					str++;
				if (*str)
					*str++ = 0;
				p = s;
				while (*p && *p != '-')
					p++;
				if (*p)
					*p++ = 0;
				i = (*s) ? atoi(s) : 0;
				j = (*p) ? atoi(p) : i;
				if (j > 63)
					j = 63;
				while (i <= j) {
					testmask |= (1L << i++);
				}
			}
			break;
		case 'M':
			create_multicast = true;
			break;
		case 'N':
			N = atoi(optarg);
			break;
		case 'S':
			S = atoi(optarg);
			printf("S = %d\n", S);
			break;
		case 'V':
			/* tracing is enabled below */
			trace_muted = false;
			break;
		case 'v':
			verbose = true;
			break;
		case 'h':
			help = true;
			break;
		default:
			help = true;
			frmwk_log0("Syntax error\n");
			break;
		}
	}

	/* initialize framework, silently if running help */
	frmwk_init(help);
	srand(frmwk_rank);

	/* Collect env variable information from WLM */
	do {
		if (help) {
			frmwk_log0(
				"Usage: t est_coll [-hvV] -M -Ncount [-t testno[-testno][,...]]\n"
				"  -h   generate help and quit.\n"
				"  -M   use multicast model (default unicast model)\n"
				"  -N   iterations (default 1)\n"
				"  -t   test list (default all)\n");
			break;
		}

		/* Test requires a minimum of two nodes */
		if (frmwk_check_env(2))
			return -1;

		/* Initialize libfabric on this node */
		ret = frmwk_init_libfabric();
		errcnt += !!ret;
		if (frmwk_errmsg(ret, "frmwk_init_libfabric()\n"))
			goto done;

		/* mute or unmute tracing */
		cxip_coll_trace_muted = trace_muted;

		/* always start with FI_UNIVERSE */
		ret = frmwk_populate_av(&fiaddrs, &size);
		errcnt += !!ret;
		if (frmwk_errmsg(ret, "frmwk_populate_av()\n"))
			goto done;

		myaddr = fiaddrs[frmwk_rank];
		ret = fi_av_lookup(cxit_av, myaddr, &mycaddr, &mycaddr_len);
		errcnt += !!ret;
		if (frmwk_errmsg(ret, "fi_av_lookup(%d)\n", frmwk_rank))
			goto done;

		TRACE("numranks=%2d rank=%2d fiaddr=%ld caddr=%05x\n",
		      frmwk_numranks, frmwk_rank, myaddr, mycaddr.nic);
	} while (0);
	if (errcnt)
		goto done;

	/* TEST CASES*/

	/* Sanity test of framework.
	*/
	do {
		PREAMBLE(0, tstnum, "test framework");
		ret = 0;
		tstcnt += 1;
		errcnt += !!ret;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	/* Sanity test of the avset_ary_append() utility function.
	*/
	do {
		PREAMBLE(0, tstnum, "create av_set list 1");
		// Test multijoins over one array list
		TRACE("======= %s\n", testname);
		avset_ary_init(&setary);
		ret = avset_ary_append(fiaddrs, size, 0, 0, &setary);
		errcnt += !!ret;

		avset_ary_destroy(&setary);
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	/* Exercise the avset_ary_append() utility function.
	*/
	do {
		PREAMBLE(0, tstnum, "create av_set list 10");
		// Exercise creating av_set lists
		avset_ary_init(&setary);
		dlist_init(&joinlist);

		ret = 0;
		for (i = 0; !ret && i < 10; i++)
			ret = avset_ary_append(fiaddrs, size, 0, 0, &setary);
		TRACE("ret=%d cnt=%d siz=%d\n", ret,
		      setary.avset_cnt, setary.avset_siz);
		errcnt += !!ret;
		errcnt += !!(setary.avset_cnt != 10);
		errcnt += !!(setary.avset_siz < 10);

		avset_ary_destroy(&setary);
		errcnt += !!(setary.avset_cnt != 0);
		errcnt += !!(setary.avset_siz != 0);
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	/* Sanity test for _test_join() utility function.
	*/
	do {
		PREAMBLE(0, tstnum, "test join (simple)");
		// Test single join over one array list
		TRACE("======= %s\n", testname);
		TRACE("[%d] starting join\n", frmwk_rank);
		jctx = coll_single_join(fiaddrs, size, 0, 0, 0, 0,
					&setary, &joinlist, "simple");
		TRACE("[%d] jctx = %p\n", frmwk_rank, jctx);
		coll_join_cleanup(&setary, &joinlist);
		errcnt += !!!jctx;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	/* Test join operation with a 1-second delay on the root.
	 */
	do {
		PREAMBLE(0, tstnum, "test join (slow root)");
		// cause slow root rank
		if (frmwk_rank == 0)
			usleep(1000000);
		jctx = coll_single_join(fiaddrs, size, 0, 0, 0, 0,
					&setary, &joinlist, "slow root");
		coll_join_cleanup(&setary, &joinlist);
		errcnt += !!!jctx;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	/* Test join operation with a 1-second delay on a leaf.
	 */
	do {
		PREAMBLE(0, tstnum, "test join (slow leaf)");
		// cause slow leaf rank
		if (frmwk_rank == (frmwk_numranks - 1))
			usleep(1000000);
		jctx = coll_single_join(fiaddrs, size, 0, 0, 0, 0,
					&setary, &joinlist, "slow leaf");
		coll_join_cleanup(&setary, &joinlist);
		errcnt += !!!jctx;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	do {
		PREAMBLE(0, tstnum, "force -FI_EAGAIN on root getgroup");
		// cause zbcoll root (rank 0) to reject getgroup requests once
		cxip_trap_set(0, CXIP_TRAP_GETGRP, -FI_EAGAIN);
		// cause non-root ranks attempt zbcoll getgroup first
		if (frmwk_rank == 0)
			usleep(10000);
		jctx = coll_single_join(fiaddrs, size, 0, 0, 0, 0,
					&setary, &joinlist, "FI_EAGAIN root");
		coll_join_cleanup(&setary, &joinlist);
		errcnt += !!!jctx;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	do {
		PREAMBLE(0, tstnum, "force -FI_EAGAIN on root broadcast");
		// cause zbcoll root (rank 0) to reject broadcast requests once
		cxip_trap_set(0, CXIP_TRAP_BCAST, -FI_EAGAIN);
		jctx = coll_single_join(fiaddrs, size, 0, 0, 0, 0,
					&setary, &joinlist,
					"FI_EAGAIN root bcast");
		coll_join_cleanup(&setary, &joinlist);
		errcnt += !!!jctx;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	do {
		PREAMBLE(0, tstnum, "force -FI_EAGAIN on root reduce");
		// cause zbcoll root (rank 0) to reject join reduce once
		cxip_trap_set(0, CXIP_TRAP_REDUCE, -FI_EAGAIN);
		jctx = coll_single_join(fiaddrs, size, 0, 0, 0, 0,
					&setary, &joinlist,
					"FI_EAGAIN root reduce");
		coll_join_cleanup(&setary, &joinlist);
		errcnt += !!!jctx;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

#if 0
	do {
		PREAMBLE(0, tstnum, "force -FI_EFAULT on PTE alloc");
		// cause zbcoll root (rank 0) to simulate PTE alloc failure
		cxip_trap_set(0, CXIP_TRAP_INITPTE, -FI_EFAULT);
		ret =  _test_join(fiaddrs, size, -FI_EAVAIL,
				  CXIP_PROV_ERRNO_PTE);
		tstcnt += 1;
		errcnt += !!ret;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;
#endif

	do {
		struct cxip_coll_mc *mc_obj;
		struct cxip_coll_reduction *reduction;
		struct cxip_coll_data coll_data;
		int ret;

		PREAMBLE(0, tstnum, "test single packet send");
		// Create multicast and send packet through HWRoot
		TRACE("======= %s\n", testname);
		TRACE("starting join\n");

		/* root is index 0, others are leaves */
		jctx = coll_single_join(fiaddrs, size, 0, 0, 0, 0,
					&setary, &joinlist, "simple");
		TRACE("completed join jctx = %p\n", jctx);
		mc_obj = (struct cxip_coll_mc *)jctx->mc;
		mc_obj->arm_disable = true;
		mc_obj->retry_disable = true;
		TRACE("S=%d rank=%d hwroot=%d\n", S, frmwk_rank,
		      mc_obj->hwroot_idx);
		reduction = &mc_obj->reduction[0];
		coll_data.red_cnt = 1;
		coll_data.intval.ival[0] = 1234;
		coll_data.intval.ival[1] = frmwk_rank;
		memset(&reduction->accum, 0, sizeof(reduction->accum));
		if (frmwk_rank == S) {
			TRACE("test starting send on %d\n", S);
			do {
				ret = cxip_coll_send_red_pkt(
					reduction, &coll_data,
					false, false);
				TRACE("send result = %d\n", ret);
			} while (ret == -FI_EAGAIN);
			TRACE("completed send = %d\n", ret);
		}
		while (1)
			_poll_cqs();

		coll_join_cleanup(&setary, &joinlist);
		errcnt += !!!jctx;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

/*###############################################################*/
	do {
		uint64_t context;

		PREAMBLE(0, tstnum, "test barrier (simple)");
		// Test single join over one array list
		TRACE("======= %s\n", testname);
		TRACE("[%d] starting join\n", frmwk_rank);
		jctx = coll_single_join(fiaddrs, size, 0, 0, 0, 0,
					&setary, &joinlist, "simple");
		TRACE("completed join jctx = %p\n", jctx);
		TRACE("start barrier\n");
		do {
			ret = fi_barrier(cxit_ep, (fi_addr_t )jctx->mc,
					 &context);
			TRACE("barrier = %d\n", ret);
		} while (ret == -FI_EAGAIN);

		if (ret == FI_SUCCESS) {
			TRACE("spin 1...\n");
			_wait_cqs(&context);
			TRACE("BARRIER COMPLETE #%d\n", i);
		} else {
			TRACE("BARRIER FAILED   #%d, ret=%d\n", i, ret);
			errcnt++;
		}
		coll_join_cleanup(&setary, &joinlist);
		errcnt += !!!jctx;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	do {
		PREAMBLE(0, tstnum, "perform barrier");
		TRACE("Starting barrier\n");
		ret = _test_barrier(fiaddrs, size, 1);
		errcnt += !!ret;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	do {
		PREAMBLE(0, tstnum, "perform broadcast");
		for (i = 0; i < frmwk_numranks; i++) {
			ret = _test_broadcast(fiaddrs, size, i);
			errcnt += !!ret;
		}
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	do {
		PREAMBLE(0, tstnum, "perform allreduce sum");
		ret = _test_allreduce(fiaddrs, size);
		TRACE("allreduce ret = %d\n", ret);
		errcnt += !!ret;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	do {
		PREAMBLE(0, tstnum, "perform barrier x N");
		ret = _test_barrier(fiaddrs, size, N);
		errcnt += !!ret;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;

	do {
		PREAMBLE(0, tstnum, "test mcast dup");
		avset_ary_init(&setary);
		TRACE("avset initialized\n");
		ret = avset_ary_append(fiaddrs, size, 0, 0, &setary);
		TRACE("avset append 1 = %d\n", ret);
		ret = avset_ary_append(fiaddrs, size, 0, 1, &setary);
		TRACE("avset append 2 = %d\n", ret);

		dlist_init(&joinlist);
		ret = coll_multi_join(&setary, &joinlist);
		TRACE("join = %d\n", ret);

		jctx = _get_join_jctx(&joinlist, 0);
		TRACE("item 0 mc=%p retval=%d prov_errno=%d\n",
		      jctx->mc, jctx->retval, jctx->prov_errno);
		if (jctx->retval || jctx->prov_errno) {
			TRACE("unexpected result on coll 0\n");
			errcnt++;
		}
		jctx = _get_join_jctx(&joinlist, 1);
		TRACE("item 1 mc=%p retval=%d prov_errno=%d\n",
		      jctx->mc, jctx->retval, jctx->prov_errno);
		if (jctx->retval != -FI_EAVAIL ||
		    jctx->prov_errno != CXIP_PROV_ERRNO_MCAST_INUSE) {
			TRACE("unexpected result on coll 1\n");
			errcnt++;
		}
		tstcnt += 1;

		frmwk_log0("%4s\n", STDMSG(ret));
		coll_multi_release(&joinlist);
		avset_ary_destroy(&setary);
	} while (0);
	tstnum++;

	do {
		PREAMBLE(0, tstnum, "test hwroot dup");
		avset_ary_init(&setary);
		TRACE("avset initialized\n");
		ret = avset_ary_append(fiaddrs, size, 0, 0, &setary);
		TRACE("avset append 1 = %d\n", ret);
		ret = avset_ary_append(fiaddrs, size, 1, 0, &setary);
		TRACE("avset append 2 = %d\n", ret);

		dlist_init(&joinlist);
		ret = coll_multi_join(&setary, &joinlist);
		TRACE("join = %d\n", ret);

		jctx = _get_join_jctx(&joinlist, 0);
		TRACE("item 0 mc=%p retval=%d prov_errno=%d\n",
		      jctx->mc, jctx->retval, jctx->prov_errno);
		if (jctx->retval || jctx->prov_errno) {
			TRACE("unexpected result on coll 0\n");
			errcnt++;
		}
		jctx = _get_join_jctx(&joinlist, 1);
		TRACE("item 1 mc=%p retval=%d prov_errno=%d\n",
		      jctx->mc, jctx->retval, jctx->prov_errno);
		if (jctx->retval != -FI_EAVAIL ||
		    jctx->prov_errno != CXIP_PROV_ERRNO_HWROOT_INUSE) {
			TRACE("unexpected result on coll 1\n");
			errcnt++;
		}
		tstcnt += 1;

		frmwk_log0("%4s\n", STDMSG(ret));
		coll_multi_release(&joinlist);
		avset_ary_destroy(&setary);
	} while (0);
	tstnum++;

	do {
		PREAMBLE(0, tstnum, "test hwroot and mcast dup");
		avset_ary_init(&setary);
		TRACE("avset initialized\n");
		ret = avset_ary_append(fiaddrs, size, 0, 0, &setary);
		TRACE("avset append 1 = %d\n", ret);
		ret = avset_ary_append(fiaddrs, size, 0, 0, &setary);
		TRACE("avset append 2 = %d\n", ret);

		dlist_init(&joinlist);
		ret = coll_multi_join(&setary, &joinlist);
		TRACE("join = %d\n", ret);

		jctx = _get_join_jctx(&joinlist, 0);
		TRACE("item 0 mc=%p retval=%d prov_errno=%d\n",
		      jctx->mc, jctx->retval, jctx->prov_errno);
		if (jctx->retval || jctx->prov_errno) {
			TRACE("unexpected result on coll 0\n");
			errcnt++;
		}
		jctx = _get_join_jctx(&joinlist, 1);
		TRACE("item 1 mc=%p retval=%d prov_errno=%d\n",
		      jctx->mc, jctx->retval, jctx->prov_errno);
		if (jctx->retval != -FI_EAVAIL ||
		    jctx->prov_errno != CXIP_PROV_ERRNO_HWROOT_INUSE) {
			TRACE("unexpected result on coll 1\n");
			errcnt++;
		}
		tstcnt += 1;

		frmwk_log0("%4s\n", STDMSG(ret));
		coll_multi_release(&joinlist);
		avset_ary_destroy(&setary);
	} while (0);
	tstnum++;

	do {
		PREAMBLE(0, tstnum, "test multiple join");
		avset_ary_init(&setary);
		TRACE("avset initialized\n");

		for (i = 0; i < N; i++) {
			ret = avset_ary_append(fiaddrs, size, i, i, &setary);
			TRACE("avset append %d = %d\n", i, ret);
		}

		dlist_init(&joinlist);
		ret = coll_multi_join(&setary, &joinlist);
		TRACE("multijoin = %d\n", ret);

		for (i = 0; i < N; i++) {
			int exp_ret = (i < size) ? 0 : -FI_EAVAIL;
			int exp_errno = (i < size) ? 0 : CXIP_PROV_ERRNO_HWROOT_INUSE;
			int good;

			jctx = _get_join_jctx(&joinlist, i);
			if (!jctx) {
				TRACE("no join item\n");
				continue;
			}
			good = (jctx->retval == exp_ret &&
				jctx->prov_errno == exp_errno);
			errcnt += !good;
			TRACE("item %d mc=%p retval=%d prov_errno=%d %s\n",
			      i, jctx->mc, jctx->retval, jctx->prov_errno,
			      good ? "ok" : "bad");
		}
		tstcnt += 1;

		frmwk_log0("%4s\n", STDMSG(ret));
		coll_multi_release(&joinlist);
		avset_ary_destroy(&setary);
	} while (0);
	tstnum++;


	do {
		PREAMBLE(0, tstnum, "test multiple broadcast");

		uint64_t **datary, *ctxary, *ctxptr;
		int in_progress, tree, root, i, j;

		/* set up maximum number of trees possible */
		avset_ary_init(&setary);
		for (tree = 0; tree < size; tree++) {
			ret = avset_ary_append(fiaddrs, size, tree, tree, &setary);
			TRACE("avset append group %d = %d\n", tree, ret);
		}
		TRACE("avset initialized\n");

		dlist_init(&joinlist);
		ret = coll_multi_join(&setary, &joinlist);
		TRACE("multijoin = %d\n", ret);

		/* context and data for each collective tree */
		ctxary = calloc(size, sizeof(uint64_t));
		datary = calloc(size, sizeof(void *));
		for (tree = 0; tree < size; tree++) {
			datary[tree] = calloc(4, sizeof(uint64_t));
			ctxary[tree] = tree;
		}

		/* repeat the collective N times as requested*/
		for (i = 0; i < N; i++) {
			in_progress = 0;

			/* rotate root every time */
			root = i%size;

			/* start a broadcast on every tree */
			for (tree = 0; tree < size; tree++) {
				uint64_t id = (uint64_t)tree << 32;

				/* prepare the data */
				memset(datary[tree], 0, 4*sizeof(uint64_t));
				if (frmwk_rank == root) {
					for (j = 0; j < 4; j++)
						datary[tree][j] = id|root;
				}
				TRACE("strt=%d tree=%d\n", i, tree);
				for (j = 0; j < 4; j++)
					TRACE("  %016lx\n", datary[tree][j]);

			}
			for (tree = 0; tree < size; tree++) {
				int tree2 = (tree + frmwk_rank)%size;

				usleep(rand() % 100);
				jctx = _get_join_jctx(&joinlist, tree2);
				ret = fi_broadcast(cxit_ep, datary[tree2], 4, NULL,
						   (fi_addr_t )jctx->mc,
						   fiaddrs[root], FI_UINT64,
						   0L, &ctxary[tree2]);
				in_progress++;
				TRACE("in_progress=%d\n", in_progress);
				if ((ctxptr = _poll_cqs())) {
					in_progress--;
					TRACE("ctxptr=%ld in_progress=%d\n",
					      *ctxptr, in_progress);
				}
			}
			while (in_progress > 0) {
				if ((ctxptr = _poll_cqs())) {
					in_progress--;
					TRACE("ctxptr=%ld in_progress=%d\n",
					      *ctxptr, in_progress);
				}
			}
			for (tree = 0; tree < size; tree++) {
				TRACE("rslt=%d tree=%d\n", i, tree);
				for (j = 0; j < 4; j++)
					TRACE("  %016lx\n", datary[tree][j]);

			}
		}
		tstcnt += 1;

		frmwk_log0("%4s\n", STDMSG(ret));
		coll_multi_release(&joinlist);
		avset_ary_destroy(&setary);
	} while (0);
	tstnum++;

#if 0
	do {
		PREAMBLE(0, tstnum, "title of test");
		ret = 0;	// some test
		errcnt += !!ret;
		tstcnt += 1;
		frmwk_log0("%4s\n", STDMSG(ret));
		frmwk_barrier();
	} while (0);
	tstnum++;
#endif

	if (help)
		return (errcnt);

done:
	frmwk_log0("%2d tests run, %d failures\n", tstcnt, errcnt);
	frmwk_log0(!!errcnt ? "ERRORS SEEN\n" : "SUCCESS\n");
	free(fiaddrs);
	frmwk_free_libfabric();
	frmwk_term();
	return !!errcnt;
}

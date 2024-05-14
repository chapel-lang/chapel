/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2021-2023 Hewlett Packard Enterprise Development LP
 */

/*
 * Validation test for the multinode zbcoll implementation.
 *
 * Launch using: srun -N4 ./test_zbcoll [args]
 */

/**
 * Test the zbcoll functions in a real environment.
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

#define	TRACE(fmt, ...)	CXIP_COLL_TRACE(CXIP_TRC_TEST_CODE, fmt, ##__VA_ARGS__)

/* convert delays to nsecs */
#define	nUSEC(n)	(n * 1000L)
#define nMSEC(n)	(n * 1000000L)
#define	nSEC(n)		(n * 1000000000L)

int verbose = false;

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

/* introduce random jitter delay into operations per rank */
void _jitter(int usec)
{
	static unsigned int seed = 0;
	if (!seed)
		seed = rand() + frmwk_rank + 1;
	if (usec) {
		usec = rand_r(&seed) % usec;
		TRACE("_jitter delay = %d usec\n", usec);
		usleep(usec);
	}
}

/* utility to poll and capture trailing errors/completions */
static void _idle_wait(struct cxip_ep_obj *ep_obj, int msec)
{
	uint32_t dsc0, err0, ack0, rcv0;
	uint32_t dsc, err, ack, rcv;
	struct timespec ts;
	long nsecs = 0L;

	cxip_zbcoll_get_counters(ep_obj, &dsc0, &err0, &ack0, &rcv0);
	_init_nsecs(&ts);
	do {
		cxip_ep_zbcoll_progress(ep_obj);
		cxip_zbcoll_get_counters(ep_obj, &dsc, &err, &ack, &rcv);
		nsecs = _measure_nsecs(&ts);
		if (dsc==dsc0 && err==err0 && ack==ack0 && rcv==rcv0)
			continue;
		TRACE("ns=%ld dsc=%d err=%d ack=%d rcv=%d\n",
			nsecs, dsc, err, ack, rcv);
		cxip_coll_trace_flush();
		dsc0 = dsc;
		err0 = err;
		ack0 = ack;
		rcv0 = rcv;
	} while (msec < 0 || nsecs < nMSEC(msec));
}

/* utility to do a primitive wait for send completion based on counters */
static int _send_wait(struct cxip_zbcoll_obj *zb, int sndcnt, int rcvcnt)
{
	struct cxip_ep_obj *ep_obj = zb->ep_obj;
	uint32_t dsc, err, ack, rcv;
	struct timespec ts;
	long nsecs = 0L;

	_init_nsecs(&ts);
	do {
		cxip_ep_zbcoll_progress(ep_obj);
		cxip_zbcoll_get_counters(ep_obj, &dsc, &err, &ack, &rcv);
		if (err || dsc)
			break;
		if (ack >= sndcnt && rcv >= rcvcnt)
			break;
		nsecs = _measure_nsecs(&ts);
	} while (nsecs < nMSEC(100));
	TRACE("ns=%ld dsc=%d err=%d ack=%d rcv=%d rc=%d\n",
		   nsecs, dsc, err, ack, rcv, zb->error);
	if (nsecs >= nMSEC(100)) {
		TRACE("TIMEOUT\n");
		return 1;
	}
	if (err || dsc || ack < sndcnt || rcv < rcvcnt) {
		TRACE("TRANSPORT FAILURE\n");
		return 1;
	}
	if (zb->error) {
		TRACE("STATE FAILURE\n");
		return 1;
	}
	cxip_coll_trace_flush();
	return 0;
}

/* send a single packet from node to node, and wait for completion */
static struct cxip_addr bad_cxip_addr;
static int bad_cxip_index = -1;

int _test_send_to_dest(struct cxip_ep_obj *ep_obj,
		       size_t size, fi_addr_t *fiaddrs,
		       int src, int dst, uint64_t payload)
{
	struct cxip_zbcoll_obj *zb;
	int grp_rank;
	int sndcnt, rcvcnt;
	int i, ret;

	ret = cxip_zbcoll_alloc(ep_obj, size, fiaddrs, ZB_NOSIM, &zb);
	if (frmwk_errmsg(ret, "%s: cxip_zbcoll_alloc()\n", __func__))
		return ret;

	/* replace an address with a different address */
	if (bad_cxip_index >= 0 && bad_cxip_index < size) {
		TRACE("bad id being used\n");
		zb->caddrs[bad_cxip_index] = bad_cxip_addr;
	}

	grp_rank = zb->state[0].grp_rank;

	ep_obj->zbcoll.disable = true;
	zb->grpid = 0;
	cxip_zbcoll_reset_counters(ep_obj);
	if (src < 0 && dst < 0) {
		/* every source to every destination */
		sndcnt = size;
		rcvcnt = size;
		for (i = 0; i < size; i++)
			cxip_zbcoll_send(zb, grp_rank, i, payload);
	} else if (src < 0) {
		/* every source sends to one destination */
		sndcnt = 1;
		rcvcnt = (dst == grp_rank) ? size : 0;
		cxip_zbcoll_send(zb, grp_rank, dst, payload);
	} else if (dst < 0 && src == grp_rank) {
		/* this source sends to every destination */
		sndcnt = size;
		rcvcnt = 1;
		for (i = 0; i < size; i++)
			cxip_zbcoll_send(zb, grp_rank, i, payload);
	} else if (dst < 0) {
		/* some other src to every destination */
		sndcnt = 0;
		rcvcnt = 1;
	} else if (grp_rank == src) {
		/* this source to a destination */
		sndcnt = 1;
		rcvcnt = (grp_rank == dst) ? 1 : 0;
		cxip_zbcoll_send(zb, grp_rank, dst, payload);
	} else if (grp_rank == dst) {
		/* some other source to this destination */
		sndcnt = 0;
		rcvcnt = 1;
	} else {
		/* not participating */
		sndcnt = 0;
		rcvcnt = 0;
	}
	ret = _send_wait(zb, sndcnt, rcvcnt);
	ep_obj->zbcoll.disable = false;
	cxip_zbcoll_free(zb);

	return ret;
}

/* normal utility to wait for collective completion, returns coll error */
static int _coll_wait(struct cxip_zbcoll_obj *zb, long nsec_wait)
{
	uint32_t dsc, err, ack, rcv;
	struct timespec ts;
	long nsecs = 0L;

	if (!zb) {
		TRACE("%s: NULL zb passed\n", __func__);
		return -FI_EINVAL;
	}
	_init_nsecs(&ts);
	do {
		cxip_ep_zbcoll_progress(zb->ep_obj);
		cxip_zbcoll_get_counters(zb->ep_obj, &dsc, &err, &ack, &rcv);
		/* this waits for a software completion */
		if (zb->error || !zb->busy)
			break;
		nsecs = _measure_nsecs(&ts);
	} while (nsecs < nsec_wait);
	TRACE("ns=%ld dsc=%d err=%d ack=%d rcv=%d\n",
		   nsecs, dsc, err, ack, rcv);
	if (nsecs >= nsec_wait) {
		TRACE("TIMEOUT\n");
		return -FI_ETIMEDOUT;
	}
	/* return the software error code -- may be -FI_EAGAIN */
	TRACE("return code = %d\n", zb->error);
	return zb->error;
}

/**
 * @brief Internal workhorse to create zb object and get group id.
 *
 * If the endpoint is not in the group, this will return FI_SUCCESS, delete the
 * zb object (if any), and do nothing.
 *
 * This creates a zb object as necessary.
 *
 * This destroys the zb object on any error.
 *
 * This call blocks for up to 100 msec waiting for completion.
 *
 * @param ep_obj : endpoint
 * @param size   : number of NIDs in group
 * @param fiaddrs: fiaddrs in group
 * @param zbp    : return pointer to zb object (may be non-NULL)
 * @return int   : libfabric error code
 */
int _getgroup(struct cxip_ep_obj *ep_obj,
	      size_t size, fi_addr_t *fiaddrs,
	      struct cxip_zbcoll_obj **zbp)
{
	int ret;

	/* need a zbcoll object for this */
	if (!zbp) {
		TRACE("%s: NULL zbp passed\n", __func__);
		return -FI_EINVAL;
	}
	if (!*zbp) {
		ret = cxip_zbcoll_alloc(ep_obj, size, fiaddrs, ZB_NOSIM, zbp);
		if (ret == -FI_ECONNREFUSED) {
			TRACE("=== COMPLETED SKIP\n");
			return FI_SUCCESS;
		}
		if (frmwk_errmsg(ret, "%s: cxip_zbcoll_alloc()\n", __func__))
			goto out;
	}

	/* getgroup collective */
	do {
		TRACE("microsleep\n");
		usleep(10);
		ret = cxip_zbcoll_getgroup(*zbp);
		if (ret == -FI_EAGAIN)
			continue;
		if (frmwk_errmsg(ret, "%s: cxip_zbcoll_getgroup()\n", __func__))
			break;
		/* Returns a collective completion error */
		ret = _coll_wait(*zbp, nMSEC(100));
		if (ret == -FI_EAGAIN)
			continue;
		break;
	} while (true);

	/* clean up after error */
	if (ret)
		goto out;

	TRACE("=== COMPLETED GETGROUP grpid=%d ret=%s\n", (*zbp)->grpid,
	    fi_strerror(-ret));
	return FI_SUCCESS;

out:
	TRACE("%s: failed\n", __func__);
	cxip_zbcoll_free(*zbp);
	*zbp = NULL;
	return ret;
}

/* detect overt getgroup errors */
int _check_getgroup_errs(struct cxip_zbcoll_obj *zb, int exp_grpid)
{
	return (frmwk_errmsg(!zb, "zb == NULL") ||
		frmwk_errmsg(zb->error, "zb->error == %d\n", zb->error) ||
		frmwk_errmsg(zb->grpid != exp_grpid, "zb->grpid=%d exp=%d\n",
			     zb->grpid, exp_grpid));
}

/* rotate array[size] by rot positions */
void _rotate_array32(uint32_t *array, size_t size, int rot)
{
	uint32_t *copy;
	uint32_t i, j;

	copy = calloc(size, sizeof(uint32_t));
	memcpy(copy, array, size*sizeof(uint32_t));
	for (i = 0; i < size; i++) {
		j = (i + rot) % size;
		array[i] = copy[j];
	}
	free(copy);
}

/* shuffle array[size] randomly */
void _shuffle_array32(uint32_t *array, size_t size)
{
	uint32_t i, j, t;

	for (i = 0; i < size-1; i++) {
		j = i + (rand() / ((RAND_MAX / (size - i)) + 1));
		t = array[j];
		array[j] = array[i];
		array[i] = t;
	}
}

/**
 * @brief Perform multiple concurrent getgroup operations.
 *
 * Parametrized test to thoroughly exercise getgroup edge conditions.
 *
 * This sets up to acquire 'nruns' group IDs.
 *
 * On each run it will only use 'naddrs' of the 'size' endpoints. If the default
 * value of -1 is used, each run will use a random number between 1 and 'size'.
 *
 * Prior to each run, the list of addresses is rotated. If 'rot' is -1, the list
 * is randomly shuffled. The purpose of rotation is to guarantee disjoint sets
 * of NIDs can be created. For instance, if you have 16 addresses (size=16), and
 * you set nruns=naddrs=rot=4, then all of the groups will be disjoint.
 *
 * This imposes a random jitter of up to 'usec' microseconds on each node, to
 * break up synchronous behavior among the nodes, and exaggerate race
 * conditions.
 *
 * This presumes a shared file system across all of the nodes under srun, and
 * writes results to files named using the rank number, overwriting old files
 * from prior runs. The rank 0 node will complete the test by reading back all
 * of the files and processing them to ensure correct behavior.
 *
 * @param ep_obj : endpoint object
 * @param size   : total number of NID addresses
 * @param fiaddrs: all NID addresses
 * @param nruns  : nruns of concurrency
 * @param naddrs : number of NIDs to use (-1 implies random)
 * @param rot    : nid rotations per run (-1 implies shuffle)
 * @param usec   : usec jitter to impose randomly
 * @return int   : 0 on success, or error code
 */
int _multigroup(struct cxip_ep_obj *ep_obj, size_t size, fi_addr_t *fiaddrs,
		int nruns, int naddrs, int rot, int usec)
{
	char fnam[256];
	FILE *fd;
	struct cxip_zbcoll_obj **zb;
	fi_addr_t *addrs;
	uint32_t *index;
	uint32_t **rows;
	uint32_t *length;
	int *grps;
	bool shuffle = false;
	uint32_t dsc, err, ack, rcv;
	int i, j, ret;

	cxip_zbcoll_reset_counters(ep_obj);

	ret = 0;
	if (nruns < 0)
		nruns = size;
	if (nruns > cxip_zbcoll_max_grps(false))
		nruns = cxip_zbcoll_max_grps(false);
	if (naddrs > size)
		naddrs = size;

	addrs = calloc(size, sizeof(fi_addr_t));// indices converted to addrs
	index = calloc(size, sizeof(uint32_t));	// nid indices (easier to read)
	for (j = 0; j < size; j++)
		index[j] = j;

	/* rows   : getgroup requests, list of nids involved
	 * length : number of addrs in each getgroup request, is <= size
	 * grps   : resulting group ID for each getgroup request
	 * zb     : zb_coll object for each getgroup request
	 */
	rows = calloc(nruns, sizeof(void *));
	length = calloc(nruns, sizeof(uint32_t));
	grps = calloc(nruns, sizeof(int));
	zb = calloc(nruns, sizeof(void *));
	for (i = 0; i < nruns; i++) {
		/* -1 means random sizes */
		if (naddrs < 0) {
			length[i] = 1 + (rand() % (size - 1));
		} else {
			length[i] = naddrs;
		}
		/* -1 means shuffle targets */
		if (rot < 0) {
			rot = 1;
			shuffle = true;
		}
		/* copy shuffled indices into row */
		rows[i] = calloc(length[i], sizeof(uint32_t));
		_rotate_array32(index, size, rot);
		if (shuffle)
			_shuffle_array32(index, size);
		memcpy(rows[i], index, length[i]*sizeof(uint32_t));
	}

	/* create zb with grpid, in same group order across nodes */
	for (i = 0; i < nruns; i++) {
		for (j = 0; j < length[i]; j++)
			addrs[j] = fiaddrs[rows[i][j]];
		_jitter(usec);
		ret = _getgroup(ep_obj, length[i], addrs, &zb[i]);
		if (frmwk_errmsg(ret, "FAILURE getgroup %d\n", i)) {
			TRACE("FAILURE getgroup %d\n", i);
			goto done;
		}
		grps[i] = (zb[i]) ? zb[i]->grpid : -1;
	}

	/* need to compare each node result with other, write to file */
	sprintf(fnam, "grpid%d", frmwk_rank);
	fd = fopen(fnam, "w");

	cxip_zbcoll_get_counters(ep_obj, &dsc, &err, &ack, &rcv);
	fprintf(fd, "%d %d %d %d\n", dsc, err, ack, rcv);
	for (i = 0; i < nruns; i++) {
		fprintf(fd, " %2d", grps[i]);
		for (j = 0; j < size; j++)
			fprintf(fd, " %2d", (j < length[i]) ? rows[i][j] : -1);
		fprintf(fd, "\n");
	}
	fclose(fd);


	/* clean up */
done:
	for (i = 0; i < nruns; i++) {
		cxip_zbcoll_free(zb[i]);
		free(rows[i]);
	}
	free(grps);
	free(length);
	free(rows);
	free(index);
	free(addrs);
	return ret;
}

/* display the accumulated data for the full test run */
void _printrun(size_t size, int irun, int ***data)
{
	int irank, inid;

	printf("Test run #%d\n", irun);
	for (irank = 0; irank < frmwk_numranks; irank++) {
		printf("rank %2d: ", irank);
		if (data[irank][irun][0] < 0) {
			printf("SKIP\n");
			continue;
		}
		printf("GRP %2d:", data[irank][irun][0]);
		for (inid = 1; inid < size+1; inid++)
			printf(" %2d", data[irank][irun][inid]);
		printf("\n");
	}
}

/**
 * @brief Check _multigroup results across all nodes.
 *
 * This is run only on the rank 0 process, and verifies the prior test run.
 *
 * @param size  : total number of NID addresses
 * @param nruns : nruns of concurrency in test
 * @return int  : 0 on success, non-zero on failure
 */
int _multicheck(size_t size, int nruns)
{
	char fnam[256];
	FILE *fd;
	uint32_t *dsc, *err, *ack, *rcv;
	int ***data;
	uint64_t bitv, *mask;
	int grp, nid;
	int irank, irank2, irun, inid, ret;

	ret = 0;
	/* data[irank][irun][inid], inid==0 is grpid */
	data = calloc(frmwk_numranks, sizeof(void *));
	for (irank = 0; irank < frmwk_numranks; irank++) {
		data[irank] = calloc(nruns, sizeof(void *));
		for (irun = 0; irun < nruns; irun++) {
			data[irank][irun] = calloc(size + 1, sizeof(int));
		}
	}
	/* one bit for each nid, max is 64 */
	mask = calloc(size, sizeof(uint64_t));
	dsc = calloc(frmwk_numranks, sizeof(uint32_t));
	err = calloc(frmwk_numranks, sizeof(uint32_t));
	ack = calloc(frmwk_numranks, sizeof(uint32_t));
	rcv = calloc(frmwk_numranks, sizeof(uint32_t));

	/* read in the per-rank file data from the last test run */
	for (irank = 0; irank < frmwk_numranks; irank++) {
		/* read file contents into data array */
		sprintf(fnam, "grpid%d", irank);
		fd = fopen(fnam, "r");
		if (! fd) {
			printf("Could not open %s\n", fnam);
			ret = 1;
			goto cleanup;
		}
		if (fscanf(fd, " %d %d %d %d",
			   &dsc[irank],
			   &err[irank],
			   &ack[irank],
			   &rcv[irank]) < 4) {
			printf("bad read (errs)\n");
			ret = 1;
			goto cleanup;
		}
		for (irun = 0; irun < nruns; irun++) {
			for (inid = 0; inid < size + 1; inid++) {
				int *ptr = &data[irank][irun][inid];
				if (fscanf(fd, " %d", ptr) < 1) {
					printf("bad read[%d,%d]\n", irun, inid);
					ret = 1;
					goto cleanup;
				}
			}
		}
		fclose(fd);
	}

	/* All ranks in any test run must use the same grpid, ranks */
	for (irun = 0; irun < nruns; irun++) {
		irank2 = -1;
		for (irank = 1; irank < frmwk_numranks; irank++) {
			/* grpid < 0: rank not involved */
			if (data[irank][irun][0] < 0)
				continue;
			/* remember first involved rank */
			if (irank2 < 0)
				irank2 = irank;
			/* compare entire row with first involved */
			for (inid = 0; inid < size+1; inid++)
				if (data[irank][irun][inid] !=
				    data[irank2][irun][inid])
					break;
			/* miscompare is a failure */
			if (inid < size+1) {
				printf("ERROR in run #%d @ %d\n", irun, inid);
				printf("reductions do not match\n");
				_printrun(size, irun, data);
				ret = 1;
				goto cleanup;
			}
		}
	}
	/* validated that all ranks in each run are identical */

	/* No nid should reuse the same grpid, only check rank 0 */
	irank = 0;
	for (irun = 0; irun < nruns; irun++) {
		/* grpid < 0: rank not involved */
		if (data[irank][irun][0] < 0)
			continue;
		grp = data[irank][irun][0];
		for (inid = 1; inid < size+1; inid++) {
			/* ignore unused fiaddrs */
			if (data[irank][irun][inid] < 0)
				continue;
			nid = data[irank][irun][inid];
			bitv = 1L << grp;
			/* failure if grpid already used */
			if (mask[nid] & bitv) {
				printf("ERROR in run #%d @ %d\n",
					irun, inid);
				printf("reuse of grpid %d by %d\n",
					grp, nid);
				_printrun(size, irun, data);
				goto cleanup;
			}
			mask[nid] |= bitv;
		}
	}

	/* We don't expect discard or ack errors */
	for (irank = 0; irank < frmwk_numranks; irank++)
		if (dsc[irank] || err[irank])
			break;
	if (irank < frmwk_numranks) {
		printf("ERROR transmission errors\n");
		for (irank = 0; irank < frmwk_numranks; irank++) {
			printf("rank %2d: dsc=%d err=%d ack=%d rcv=%d\n",
				irank, dsc[irank], err[irank],
				ack[irank], rcv[irank]);
		}
		goto cleanup;
	}

cleanup:
	if (verbose) {
		printf("==================\n");
		printf("Dump all test runs\n");
		for (irun = 0; irun < nruns; irun++)
			_printrun(size, irun, data);
		printf("getgroup test %s\n", !ret ? "passed" : "FAILED");
	}
	fflush(stdout);

	free(dsc);
	free(err);
	free(ack);
	free(rcv);
	free(mask);
	for (irank = 0; irank < frmwk_numranks; irank++) {
		for (irun = 0; irun < nruns; irun++)
			free(data[irank][irun]);
		free(data[irank]);
	}
	free(data);
	return ret;
}

/* use up all group IDs, then free zb objects and add more */
int _exhaustgroup(struct cxip_ep_obj *ep_obj, size_t size, fi_addr_t *fiaddrs,
		 int nruns, int usec)
{
	struct cxip_zbcoll_obj **zb;
	int maxgrps;
	int i, n, ret = 0;

	maxgrps = cxip_zbcoll_max_grps(false);
	if (nruns < 0)
		nruns = maxgrps + 10;
	zb = calloc(nruns, sizeof(void *));
	n = 1;
	for (i = 0; i < nruns; i++) {
		_jitter(usec);
		ret = _getgroup(ep_obj, size, fiaddrs, &zb[i]);
		if (ret == -FI_EBUSY) {
			/* free an old zb, and try again */
			cxip_zbcoll_free(zb[n]);
			zb[n] = NULL;
			ret = _getgroup(ep_obj, size, fiaddrs, &zb[i]);
			if (frmwk_errmsg(ret, "FAILURE\n")) {
				TRACE("FAILURE\n");
				break;
			}
			if (zb[i]->grpid != n) {
				TRACE("FAILURE\n");
				break;
			}
			n = (n + 3) % maxgrps;
		}
	}
	for (i = 0; i < nruns; i++)
		cxip_zbcoll_free(zb[i]);

	return 0;
}

/* Wait for completion, log errors, free zb object */
int _test_wait_free(struct cxip_zbcoll_obj *zb,
		    uint64_t *result, uint64_t expect)
{
	int ret;

	/* wait for completion */
	ret = _coll_wait(zb, nMSEC(100));
	if (frmwk_errmsg(ret, "reduce wait failed\n"))
		goto done;

	if (!result)
		goto done;

	TRACE("expect=%08lx result=%08lx, ret=%s\n",
	    expect, *result, fi_strerror(-ret));
	if (*result != expect) {
		ret = 1;
		frmwk_errmsg(ret, "expect=%08lx result=%08lx\n",
				expect, *result);
	}
done:
	cxip_zbcoll_free(zb);
	return ret;
}

/* barrier across all NIDs, return zb object */
int _test_barr(struct cxip_ep_obj *ep_obj,
	     size_t size, fi_addr_t *fiaddrs,
	     struct cxip_zbcoll_obj **zbp)
{
	struct cxip_zbcoll_obj *zb = NULL;
	int ret;

	/* need a zbcoll context for this */
	ret = _getgroup(ep_obj, size, fiaddrs, &zb);
	if (ret)
		goto out;

	/* reset counters */
	cxip_zbcoll_reset_counters(ep_obj);

	/* if this fails, do not continue */
	ret = cxip_zbcoll_barrier(zb);
	if (frmwk_errmsg(ret, "barr0 return=%s, exp=%d\n", fi_strerror(-ret), 0))
		goto out;

	/* try this again, should fail with -FI_EAGAIN */
	ret = cxip_zbcoll_barrier(zb);
	if (frmwk_errmsg((ret != -FI_EAGAIN), "barr1 return=%d, exp=%d\n",
		       ret, -FI_EAGAIN))
		goto out;

	*zbp = zb;
	return 0;
out:
	cxip_zbcoll_free(zb);
	return 1;
}

/* broadcast the payload from rank 0 to all other ranks, return zb object */
int _test_bcast(struct cxip_ep_obj *ep_obj,
	       size_t size, fi_addr_t *fiaddrs,
	       uint64_t *result, struct cxip_zbcoll_obj *zb)
{
	int ret;

	TRACE("%s: entry\n", __func__);
	/* reset counters */
	cxip_zbcoll_reset_counters(ep_obj);

	/* if this fails, do not continue */
	TRACE("%s: initiate broadcast\n", __func__);
	ret = cxip_zbcoll_broadcast(zb, result);
	TRACE("bcast payload=%08lx, ret=%s\n", *result, fi_strerror(-ret));
	if (frmwk_errmsg(ret, "bcast0 return=%s, exp=%d\n", fi_strerror(-ret), 0))
		goto out;

	/* try this again, should fail with -FI_EAGAIN */
	ret = cxip_zbcoll_broadcast(zb, result);
	TRACE("bcast payload=%08lx, ret=%s\n", *result, fi_strerror(-ret));
	if (frmwk_errmsg((ret != -FI_EAGAIN), "bcast1 return=%d, exp=%d\n",
		       ret, -FI_EAGAIN))
		goto out;
	return 0;
out:
	TRACE("%s: failed\n", __func__);
	return 1;
}

/* Generate a random number with some constant bits, limited to 53 bits.
 * rand() sequence is deterministic.
 */
static inline uint64_t _reduce_val(void)
{
	uint64_t val = rand();
	val = (val << 32) | rand();
	return (val | 0x10010002) % (1L << 54);
}

int _test_reduce(struct cxip_ep_obj *ep_obj,
	         size_t size, fi_addr_t *fiaddrs,
		 uint64_t *payload, struct cxip_zbcoll_obj *zb)
{
	int ret;

	/* reset counters */
	cxip_zbcoll_reset_counters(ep_obj);

	/* if this fails, do not continue */
	ret = cxip_zbcoll_reduce(zb, payload);
	TRACE("reduce payload=%08lx, ret=%s\n", *payload, fi_strerror(-ret));
	if (frmwk_errmsg(ret, "reduce0 return=%s, exp=%d\n",
		       fi_strerror(-ret), 0))
		goto out;

	/* try this again, should fail with -FI_EAGAIN */
	ret = cxip_zbcoll_reduce(zb, payload);
	TRACE("reduce payload=%08lx, ret=%s\n", *payload, fi_strerror(-ret));
	if (frmwk_errmsg((ret != -FI_EAGAIN), "reduce1 return=%d, exp=%d\n",
		       ret, -FI_EAGAIN))
		goto out;

	return 0;
out:
	TRACE("%s: failed\n", __func__);
	return 1;
}

const char *testnames[] = {
	"test  0: send one packet 0 -> 0",
	"test  1: send one packet 0 -> 1",
	"test  2: send one packet 1 -> 0",
	"test  3: send one packet 0 -> N",
	"test  4: send one packet N -> 0",
	"test  5: send one packet N -> N",
	"test  6: single getgroup",
	"test  7: double getgroup full overlap",
	"test  8: double getgroup partial overlap",
	"test  9: getgroup regression [-NMRD]",
	"test 10: getgroup exahustion [-ND]",
	"test 11: barrier",
	"test 12: broadcast (single)",
	"test 13: broadcast (concurrent)",
	"test 14: reduce (single)",
	"test 15: reduce (concurrent)",
	"test 16: getgroup perf",
	"test 17: barrier perf",
	"test 18: broadcast perf",
	"test 19: reduce perf",
	"test 20: send bad dest [-B required]",
	"test 21: recv bad dest [-B required]",
	NULL
};
const char *testname;

int usage(int ret)
{
	int i;

	frmwk_log0("Usage: test_zbcoll [-hvV] [-s seed]\n"
		"                    [-N nruns] [-M sublen] [-R rotate]\n"
		"                    [-D usec_delay] [-B bad_NIC]\n"
		"                    [-t testno[,testno...]]\n"
		"\n"
		"  -h displays this help\n"
		"  -v provides verbose output\n"
		"  -V provides per-node tracing\n"
		"  -s specifies a random seed for randomized tests\n"
		"  -t specifies tests e.g. (1,2,3) or (1-3) or (1-3,11-12)"
		"\n");
	for (i = 0; testnames[i]; i++)
		frmwk_log0("%s\n", testnames[i]);

	return ret;
}

/* scan for integers in -t option */
static inline char *scanint(char *ptr, int *val)
{
	char *p = ptr;
	while (*ptr >= '0' && *ptr <= '9')
		ptr++;
	*val = atoi(p);
	return ptr;
}

#define	TEST(n)	(1 << n)
static inline bool _istest(uint64_t mask, int test)
{
	return (mask & (1 << test)) && (testname = testnames[test]);
}

int main(int argc, char **argv)
{
	bool trace_muted = true;
	char hostname[256];
	fi_addr_t *fiaddrs = NULL;
	struct cxip_ep *cxip_ep;
	struct cxip_ep_obj *ep_obj;
	struct cxip_zbcoll_obj *zb1 = NULL;
	struct cxip_zbcoll_obj *zb2 = NULL;
	size_t size = 0;
	unsigned int seed;
	uint64_t testmask;
	uint64_t result1, result2;
	uint64_t payload1, payload2;
	uint64_t expect1, expect2;
	int opt, nruns, naddrs, rot, usec, badnic, ret;

	int errcnt = 0;
	int i;

	seed = 123;
	usec = 0;	// as fast as possible
	nruns = -1;	// run maximum number groups
	naddrs = -1;	// random selection of fiaddrs
	rot = -1;	// random shuffle of fiaddrs
	testmask = -1;	// run all tests
	badnic = -1;	// do not use an address override

	while ((opt = getopt(argc, argv, "hvVt:s:N:M:R:D:B:")) != -1) {
		char *str, *s, *p;
		int i, j;

		switch (opt) {
		case 't':
			testmask = 0;
			str = optarg;
			i = j = 0;
			while (*str) {
				s = str;
				while (*str && *str != ',')
					str++;
				if (*str)
					*str++ = 0;
				p = s;
				while (*p && *p != '-')
					p++;
				i = atoi(s);
				j = (*p) ? atoi(++p) : i;
				while (i <= j)
					testmask |= 1 << i++;
			}
			break;
		case 's':
			seed = atoi(optarg);
			break;
		case 'N':
			nruns = atoi(optarg);
			break;
		case 'M':
			naddrs = atoi(optarg);
			break;
		case 'R':
			rot = atoi(optarg);
			break;
		case 'D':
			usec = atoi(optarg);
			break;
		case 'T':
			frmwk_rank = atoi(optarg);
			break;
		case 'B':
			badnic = strtol(optarg, NULL, 16);
			break;
		case 'V':
			trace_muted = false;
			break;
		case 'v':
			verbose = true;
			break;
		case 'h':
			return usage(0);
		default:
			return usage(1);
		}
	}

	frmwk_init(false);
	if (frmwk_check_env(2))
		return -1;

	ret = frmwk_init_libfabric();
	if (frmwk_errmsg(ret, "frmwk_init_libfabric()\n"))
		return ret;

	cxip_coll_trace_rank = frmwk_rank;
	cxip_coll_trace_numranks = frmwk_numranks;
	cxip_coll_trace_muted = trace_muted;
	TRACE("==== tracing enabled offset %d\n", frmwk_rank);

	srand(seed);
	if (naddrs < 0)
		naddrs = frmwk_numranks;
	if (nruns < 0)
		nruns = frmwk_numranks;
	if (nruns > cxip_zbcoll_max_grps(false))
		nruns = cxip_zbcoll_max_grps(false);

	frmwk_log0("Using random seed = %d\n", seed);
	if (verbose) {
		frmwk_log0("verbose = true\n");
		frmwk_log0("nruns    = %d\n", nruns);
		frmwk_log0("naddrs    = %d\n", naddrs);
		frmwk_log0("rotate   = %d\n", rot);
		frmwk_log0("delay    = %d usec\n", usec);
	}

	cxip_ep = container_of(cxit_ep, struct cxip_ep, ep.fid);
	ep_obj = cxip_ep->ep_obj;

	/* always start with FI_UNIVERSE */
	ret = frmwk_populate_av(&fiaddrs, &size);
	if (frmwk_errmsg(ret, "frmwk_populate_av()\n"))
		return 1;
	frmwk_log0("libfabric populated\n");

	gethostname(hostname, sizeof(hostname));
	TRACE("%s NIC=%04x PID=%d\n", hostname, ep_obj->src_addr.nic,
	    ep_obj->ptable->pid);

	if (_istest(testmask, 0)) {
		TRACE("======= %s\n", testname);
		ret = _test_send_to_dest(ep_obj, size, fiaddrs, 0, 0, frmwk_rank);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		TRACE("rank %2d result = %s\n", frmwk_rank, fi_strerror(-ret));
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 1)) {
		TRACE("======= %s\n", testname);
		ret = _test_send_to_dest(ep_obj, size, fiaddrs, 0, 1, frmwk_rank);
		errcnt += !!ret;
		TRACE("rank %2d result = %s\n", frmwk_rank, fi_strerror(-ret));
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 2)) {
		TRACE("======= %s\n", testname);
		ret = _test_send_to_dest(ep_obj, size, fiaddrs, 1, 0, frmwk_rank);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		TRACE("rank %2d result = %s\n", frmwk_rank, fi_strerror(-ret));
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 3)) {
		TRACE("======= %s\n", testname);
		ret = _test_send_to_dest(ep_obj, size, fiaddrs, 0, -1, frmwk_rank);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		TRACE("rank %2d result = %s\n", frmwk_rank, fi_strerror(-ret));
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 4)) {
		TRACE("======= %s\n", testname);
		ret = _test_send_to_dest(ep_obj, size, fiaddrs, -1, 0, frmwk_rank);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		TRACE("rank %2d result = %s\n", frmwk_rank, fi_strerror(-ret));
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 5)) {
		TRACE("======= %s\n", testname);
		ret = _test_send_to_dest(ep_obj, size, fiaddrs, -1, -1, frmwk_rank);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		TRACE("rank %2d result = %s\n", frmwk_rank, fi_strerror(-ret));
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 6)) {
		TRACE("======= %s\n", testname);
		zb1 = NULL;
		ret = 0;
		ret += !!_getgroup(ep_obj, size, fiaddrs, &zb1);
		ret += !!_check_getgroup_errs(zb1, 0);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		cxip_zbcoll_free(zb1);
		frmwk_barrier();
	}

	if (_istest(testmask, 7)) {
		TRACE("======= %s\n", testname);
		zb1 = NULL;
		ret = 0;
		ret += !!_getgroup(ep_obj, size, fiaddrs, &zb1);
		ret += !!_getgroup(ep_obj, size, fiaddrs, &zb2);
		ret += !!_check_getgroup_errs(zb1, 0);
		ret += !!_check_getgroup_errs(zb2, 1);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		cxip_zbcoll_free(zb2);
		cxip_zbcoll_free(zb1);
		frmwk_barrier();
	}

	if (_istest(testmask, 8)) {
		TRACE("======= %s\n", testname);
		zb1 = zb2 = NULL;
		ret = 0;
		TRACE("test one\n");
		if (frmwk_rank != frmwk_numranks-1) {
			ret += !!_getgroup(ep_obj, size-1, &fiaddrs[0], &zb2);
			ret += !!_check_getgroup_errs(zb2, 0);
		} else {
			TRACE("SKIP\n");
		}
		TRACE("test two\n");
		if (frmwk_rank != 0) {
			ret += !!_getgroup(ep_obj, size-1, &fiaddrs[1], &zb1);
			ret += !!_check_getgroup_errs(zb1, 1);
		} else {
			TRACE("SKIP\n");
		}
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		cxip_zbcoll_free(zb2);
		cxip_zbcoll_free(zb1);
		frmwk_barrier();
	}

	if (_istest(testmask, 9)) {
		TRACE("======= %s\n", testname);
		ret = 0;
		ret += !!_multigroup(ep_obj, size, fiaddrs, nruns, naddrs,
				     rot, usec);
		frmwk_barrier();

		if (!ret && frmwk_rank == 0)
			ret += !!_multicheck(size, nruns);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 10)) {
		TRACE("======= %s\n", testname);
		ret = 0;
		ret += !!_exhaustgroup(ep_obj, size, fiaddrs, nruns, usec);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		ret += !!_test_barr(ep_obj, size, fiaddrs, &zb1);
		ret += !!_test_wait_free(zb1, NULL, 0);
		frmwk_barrier();
	}

	if (_istest(testmask, 11)) {
		TRACE("======= %s\n", testname);
		zb1 = NULL;
		ret = 0;
		ret += !!_test_barr(ep_obj, size, fiaddrs, &zb1);
		ret += !!_test_wait_free(zb1, NULL, 0);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 12)) {
		TRACE("======= %s\n", testname);
		zb1 = NULL;
		ret = 0;
		result1 = (frmwk_rank) ? frmwk_rank : 0x123;
		ret += !!_getgroup(ep_obj, size, fiaddrs, &zb1);
		ret += !!_test_bcast(ep_obj, size, fiaddrs, &result1, zb1);
		ret += !!_test_wait_free(zb1, &result1, 0x123);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 13)) {
		TRACE("======= %s\n", testname);
		zb1 = zb2 = NULL;
		ret = 0;
		result1 = (frmwk_rank) ? frmwk_rank : 0x123;
		result2 = (frmwk_rank) ? frmwk_rank : 0x456;
		ret += !!_getgroup(ep_obj, size, fiaddrs, &zb1);
		ret += !!_getgroup(ep_obj, size, fiaddrs, &zb2);
		ret += !!_test_bcast(ep_obj, size, fiaddrs, &result1, zb1);
		ret += !!_test_bcast(ep_obj, size, fiaddrs, &result2, zb2);
		TRACE("waiting for bcast 1\n");
		ret += !!_test_wait_free(zb1, &result1, 0x123);
		TRACE("waiting for bcast 2\n");
		ret += !!_test_wait_free(zb2, &result2, 0x456);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 14)) {

		TRACE("======= %s\n", testname);
		expect1 = -1L % (1L << 54);
		for (i = 0; i < size; i++) {
			uint64_t val = _reduce_val();
			if (i == frmwk_rank)
				payload1 = val;
			expect1 &= val;
		}
		zb1 = NULL;
		ret = 0;
		ret += !!_getgroup(ep_obj, size, fiaddrs, &zb1);
		ret += !!_test_reduce(ep_obj, size, fiaddrs,
				      &payload1, zb1);
		ret += !!_test_wait_free(zb1, &payload1, expect1);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 15)) {
		TRACE("======= %s\n", testname);
		expect1 = -1L % (1L << 54);
		expect2 = -1L % (1L << 54);
		for (i = 0; i < size; i++) {
			uint64_t val = _reduce_val();
			if (i == frmwk_rank)
				payload1 = val;
			expect1 &= val;
		}
		for (i = 0; i < size; i++) {
			uint64_t val = _reduce_val();
			if (i == frmwk_rank)
				payload2 = val;
			expect2 &= val;
		}
		zb1 = zb2 = NULL;
		ret = 0;
		ret += !!_getgroup(ep_obj, size, fiaddrs, &zb1);
		ret += !!_getgroup(ep_obj, size, fiaddrs, &zb2);
		ret += !!_test_reduce(ep_obj, size, fiaddrs,
				      &payload1, zb1);
		ret += !!_test_reduce(ep_obj, size, fiaddrs,
				      &payload2, zb2);
		ret += !!_test_wait_free(zb1, &payload1, expect1);
		ret += !!_test_wait_free(zb2, &payload2, expect2);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		frmwk_barrier();
	}

	if (_istest(testmask, 16)) {
		struct timespec t0;
		long count = 0;
		double time;

		TRACE("======= %s\n", testname);
		cxip_coll_trace_muted = true;
		zb1 = NULL;
		ret = cxip_zbcoll_alloc(ep_obj, size, fiaddrs, ZB_NOSIM, &zb1);
		clock_gettime(CLOCK_MONOTONIC, &t0);
		while (!ret && count < 100000) {
			int ret2;
			do {
				ret += !!cxip_zbcoll_getgroup(zb1);
				ret2 = _coll_wait(zb1, nMSEC(100));
			} while (!ret && ret2 == -FI_EAGAIN);
			ret += !!ret2;
			cxip_zbcoll_rlsgroup(zb1);
			count++;
		}
		time = _measure_nsecs(&t0);
		time /= 1.0*count;
		time /= 1000.0;
		cxip_coll_trace_muted = trace_muted;
		cxip_zbcoll_free(zb1);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s \tcount=%ld time=%1.2fus/op\n",
			 ret ? "FAIL" : "ok", testname, count, time);
		frmwk_barrier();
	}

	if (_istest(testmask, 17)) {
		struct timespec t0;
		long count = 0;
		double time;

		TRACE("======= %s\n", testname);
		cxip_coll_trace_muted = true;
		zb1 = NULL;
		ret = _getgroup(ep_obj, size, fiaddrs, &zb1);
		clock_gettime(CLOCK_MONOTONIC, &t0);
		while (!ret && count < 100000) {
			ret += !!cxip_zbcoll_barrier(zb1);
			ret += !!_coll_wait(zb1, nMSEC(100));
			count++;
		}
		time = _measure_nsecs(&t0);
		time /= 1.0*count;
		time /= 1000.0;
		cxip_coll_trace_muted = trace_muted;
		cxip_zbcoll_free(zb1);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s \tcount=%ld time=%1.2fus/op\n",
			 ret ? "FAIL" : "ok", testname, count, time);
		frmwk_barrier();
	}

	if (_istest(testmask, 18)) {
		struct timespec t0;
		uint64_t result = 0x1234;
		long count = 0;
		double time;

		TRACE("======= %s\n", testname);
		cxip_coll_trace_muted = true;
		zb1 = NULL;
		ret = _getgroup(ep_obj, size, fiaddrs, &zb1);
		clock_gettime(CLOCK_MONOTONIC, &t0);
		while (!ret && count < 100000) {
			ret += !!cxip_zbcoll_broadcast(zb1, &result);
			ret += !!_coll_wait(zb1, nMSEC(100));
			count++;
		}
		time = _measure_nsecs(&t0);
		time /= 1.0*count;
		time /= 1000.0;
		cxip_coll_trace_muted = trace_muted;
		cxip_zbcoll_free(zb1);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s \tcount=%ld time=%1.2fus/op\n",
			 ret ? "FAIL" : "ok", testname, count, time);
		frmwk_barrier();
	}

	if (_istest(testmask, 19)) {
		struct timespec t0;
		uint64_t result = 0x1234;
		long count = 0;
		double time;

		TRACE("======= %s\n", testname);
		cxip_coll_trace_muted = true;
		zb1 = NULL;
		ret = _getgroup(ep_obj, size, fiaddrs, &zb1);
		clock_gettime(CLOCK_MONOTONIC, &t0);
		while (!ret && count < 100000) {
			ret += !!cxip_zbcoll_reduce(zb1, &result);
			ret += !!_coll_wait(zb1, nMSEC(100));
			count++;
		}
		time = _measure_nsecs(&t0);
		time /= 1.0*count;
		time /= 1000.0;
		cxip_coll_trace_muted = trace_muted;
		cxip_zbcoll_free(zb1);
		errcnt += !!ret;
		_idle_wait(ep_obj, 100);
		frmwk_log0("%4s %s \tcount=%ld time=%1.2fus/op\n",
			 ret ? "FAIL" : "ok", testname, count, time);
		frmwk_barrier();
	}

	if (_istest(testmask, 20)) {
		if (badnic >= 0) {
			TRACE("======= %s\n", testname);
			bad_cxip_addr.nic = badnic;
			bad_cxip_addr.pid = 0;
			bad_cxip_index = 1;
			ret = _test_send_to_dest(ep_obj, size, fiaddrs,
						 0, 1, frmwk_rank);
			bad_cxip_index = -1;
			errcnt += !!ret;
			_idle_wait(ep_obj, 100);
			TRACE("rank %2d result = %d\n", frmwk_rank, ret);
			frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		} else {
			frmwk_log0("%4s %s\n", "SKIP", testname);
		}
		frmwk_barrier();
	}

	if (_istest(testmask, 21)) {
		if (badnic >= 0) {
			TRACE("======= %s\n", testname);
			ret = _test_send_to_dest(ep_obj, size, fiaddrs,
						 0, 1, frmwk_rank);
			ret = _test_send_to_dest(ep_obj, size, fiaddrs,
						 1, 0, frmwk_rank);
			//ret = _getgroup(ep_obj, size, fiaddrs, &zb1);
			TRACE("listening forever....\n");
			cxip_coll_trace_flush();
			_idle_wait(ep_obj, -1);
			frmwk_log0("%4s %s\n", ret ? "FAIL" : "ok", testname);
		} else {
			frmwk_log0("%4s %s\n", "SKIP", testname);
		}
		frmwk_barrier();
	}

	TRACE("Finished test run, cleaning up\n");
	free(fiaddrs);
	frmwk_free_libfabric();
	frmwk_log0(!!errcnt ? "ERRORS SEEN\n" : "SUCCESS\n");
	frmwk_term();
	return !!errcnt;
}

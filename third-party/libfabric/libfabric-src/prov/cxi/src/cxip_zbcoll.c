/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2021-2022 Hewlett Packard Enterprise Development LP
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <endian.h>
#include <sys/time.h>
#include <sys/types.h>

#include <ofi_list.h>
#include <ofi.h>
#include <fenv.h>

#include "cxip.h"

/* Distinctions:
 * CXIP_DBG() is generally useless in a multi-node collective. Use TRACE().
 * CXIP_INFO() is generally useless in internal code of this sort.
 * CXIP_WARN() is used to leave a log trace to identify failures.
 *     -FI_ENOMEM is not logged, since where it occurs is irrelevant: all
 *         memory allocation in this module is small, so heap exhaustion
 *         indicates a systemic failure.
 *     -FI_EAGAIN and -FI_EBUSY are not logged, as they are transient
 */
#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_INFO(...) _CXIP_INFO(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EP_CTRL, __VA_ARGS__)

#define	TRACE(fmt, ...)	CXIP_COLL_TRACE(CXIP_TRC_ZBCOLL, fmt, ##__VA_ARGS__)

/* see data packing structures below */
#define	ZB_MAP_BITS	54
#define	ZB_GRPID_BITS	6
#define	ZB_SIM_BITS	5
#define	ZB_SIM_MAX	(1 << (ZB_SIM_BITS))
#define	ZB_NEG_BIT	((ZB_MAP_BITS) - 1)

static int zbdata_send_cb(struct cxip_ctrl_req *req,
			  const union c_event *event);

/****************************************************************************
 * OVERVIEW
 *
 * There are two related components in this file.
 * - An abstract radix tree constructor
 * - A collective implemention built on the Zero-Buffer Put control channel.
 *
 * The basic operational flow is as follows:
 * - cxip_zbcoll_init() prepares the system for zbcoll collectives.
 * - cxip_zbcoll_alloc() allocates and configures a collective structure.
 * - cxip_zbcoll_getgroup() negotiates a collective identifier (one time).
 * - cxip_zbcoll_barrier() performs a barrier (can be repeated).
 * - cxip_zbcoll_broadcast() performs a broadcast (can be repeated).
 * - cxip_zbcoll_progress() progresses getgroup/barrier/broadcast/reduce on ep.
 * - cxip_zbcoll_free() releases the collective structure and identifier.
 * - cxip_zbcoll_fini() releases all collectives and cleans up.
 *
 * Any number of collective structures can be created, spanning the same, or
 * different node-sets.
 *
 * To enable the structure, it must acquire a group identifier using the
 * getgroup operation, which is itself a collective operation. Getgroup
 * negotiates for and acquires one of 53 possible group identifiers (43 in
 * simulation). The group identifier remains with that structure until the
 * structure is deleted, allowing it to be used for multiple collective
 * operations without renegotiating.
 *
 * Collective operations are concurrent for groups with different group
 * identifiers. Collective operations for a single group are serialized,
 * returning -FI_EAGAIN if there is already a collective operation in progress
 * for that group.
 *
 * The getgroup, barrier, and broadcast functions support a callback stack that
 * allows caller-defined callback functions to be stacked for execution upon
 * completion of a collective. The callback can initiate a new collective on the
 * same object.
 *
 * Note that this is NOT a general-purpose collective implementation.
 */

/****************************************************************************
 * ABSTRACT RADIX TREE
 *
 * We lay out all of the node indices (0..maxnodes-1) in layers, as follows:
 *
 * RADIX 1:
 * row: nodeidx
 *   0: 0
 *   1: 1
 *   2: 2
 * ...
 *
 * RADIX 2:
 * row: nodeidx
 *   0: 0
 *   1: 1, 2
 *   2: 3, 4, 5, 6
 *   3: 7, 8, 9, 10, 11, 12, 13, 14
 * ...
 *
 * RADIX 3:
 * row: nodeidx
 *   0: 0
 *   1: 1, 2, 3
 *   2: 4, 5, 6, 7, 8, 9, 10, 11, 12
 *   3: 13, 14, 15, 16, 17, 18, ... 38, 39
 * ...
 *
 * The parent of any node is in the row above it, and the children are in the
 * row below it. The width of any row is (RADIX ^ row), so for every node, there
 * can be up to RADIX children, and one parent, with the exception of the root
 * node (no parent).
 */

/**
 * @brief Compute row and column for a given node index.
 *
 * @param radix   : radix of tree
 * @param nodeidx : node index
 * @param row     : returned row of this node
 * @param col     : returned offset of this node in the row
 * @param siz     : returned size of the row, (0 <= col < siz)
 */
void cxip_tree_rowcol(int radix, int nodeidx, int *row, int *col, int *siz)
{
	int rownum = 0;
	int rowcum = 0;
	int rowsiz = 1;

	*row = 0;
	*col = 0;
	*siz = rowsiz;
	if (radix < 1)
		return;
	while (nodeidx > rowcum) {
		rowsiz *= radix;
		*row = rownum + 1;
		*col = nodeidx - rowcum - 1;
		*siz = rowsiz;
		rowcum += rowsiz;
		rownum += 1;
	}
}

/**
 * @brief Compute the node index for a give row and column.
 *
 * Note that illegal columns can be specified for a row, which results
 * in a return index of -1.
 *
 * @param radix   : radix of tree
 * @param row     : row of node
 * @param col     : column of node
 * @param nodeidx : returned node index, or -1 if illegal
 */
void cxip_tree_nodeidx(int radix, int row, int col, int *nodeidx)
{
	int rownum = 0;
	int rowcum = 0;
	int rowsiz = 1;

	*nodeidx = 0;
	while (radix && rownum < row) {
		rowsiz *= radix;
		*nodeidx = rowcum + col + 1;
		rowcum += rowsiz;
		rownum += 1;
	}
	if (col >= rowsiz)
		*nodeidx = -1;
}

/**
 * @brief Provide the relatives (parent, children) of a node
 *
 * The rels array must be provided, and must have RADIX+1 entries.
 *
 * The parent position [0] will always be populated, but with -1 if the node is
 * the root node.
 *
 * Only valid child positions in [1..RADIX] will be populated.
 *
 * This returns the total number of positions populated.
 *
 * If radix < 1, there can be no relatives, and this returns 0.
 *
 * @param radix    : radix of tree
 * @param nodeidx  : index of node to find relatives for
 * @param maxnodes : maximum valid node indices available
 * @param rels     : relative index array
 * @return int : number of valid relatives found
 */
int cxip_tree_relatives(int radix, int nodeidx, int maxnodes, int *rels)
{
	int row, col, siz, idx, n;

	if (radix < 1 || !maxnodes || !rels)
		return 0;

	cxip_tree_rowcol(radix, nodeidx, &row, &col, &siz);

	idx = 0;
	if (row)
		cxip_tree_nodeidx(radix, row - 1, col / radix, &rels[idx++]);
	else
		rels[idx++] = -1;

	cxip_tree_nodeidx(radix, row+1, col*radix, &nodeidx);
	for (n = 0; n < radix; n++) {
		if ((nodeidx + n) >= maxnodes)
			break;
		rels[idx++] = nodeidx + n;
	}

	return idx;
}

/****************************************************************************
 * @brief Zero-buffer collectives.
 *
 * ZB collectives are intended for implementation of the fi_join_collective()
 * function.
 *
 * The ep_obj has a container structure of type cxip_ep_zbcoll_obj, which
 * maintains endpoint-global state for all zb collectives on that NIC endpoint.
 * We refer to this as the zbcoll object, and it is an extension of the endpoint
 * itself.
 *
 * The zbcoll object contains dynamic zb objects, each representing a collective
 * group.
 *
 * Each zb object contains one or more state structures, which support simulated
 * operations on a single node. Production code will use only one state for the
 * NID.
 *
 * Diagnostic counters are maintained:
 *
 * - ack_count == successful sends
 * - err_count == failed sends
 * - rcv_count == successful receives
 * - dsc_count == discarded receives
 */

static inline void _setbit(uint64_t *mask, int bit)
{
	*mask |= (1ULL << bit);
}

static inline void _clrbit(uint64_t *mask, int bit)
{
	*mask &= ~(1ULL << bit);
}

void cxip_zbcoll_get_counters(struct cxip_ep_obj *ep_obj, uint32_t *dsc,
			      uint32_t *err, uint32_t *ack, uint32_t *rcv)
{
	struct cxip_ep_zbcoll_obj *zbcoll;

	zbcoll = &ep_obj->zbcoll;
	if (dsc)
		*dsc = ofi_atomic_get32(&zbcoll->dsc_count);
	if (err)
		*err = ofi_atomic_get32(&zbcoll->err_count);
	if (ack)
		*ack = ofi_atomic_get32(&zbcoll->ack_count);
	if (rcv)
		*rcv = ofi_atomic_get32(&zbcoll->rcv_count);
}

/**
 * @brief Link a secondary zb object to a primary zb object.
 *
 * This is used with multi-zb object simulation. The basic (single-zb) model is
 * that the zb object has num_caddrs state structures to manage the state of
 * each simulated destination address, each of which has a backpointer to the
 * containing zb object.
 *
 * For the multi-zb simulation, the root zb (simrank == 0) has num_caddrs state
 * structures, but each points back to a different zb object. When packets are
 * received, the are initially received by the root zb, which determines the
 * state structure to use from the simulated 'dst' address embedded in the
 * packet, and that is then re-routed through the state[dst].zb pointer to the
 * correct target zb object and state[dst] on that object.
 *
 * - zb[0]->state[0].zb -> zb[0]
 * - zb[0]->state[1].zb -> zb[1]
 * - ...
 * - zb[0]->state[n].zb -> zb[n]
 *
 * This also modifies each of the other structures to backlink state[0] to the
 * root structure. This allows data from the leaf nodes to be placed in the root
 * structure for sending.
 *
 * - zb[1]->state[0].zb -> zb[0]
 * - ...
 * - zb[n]->state[0].zb -> zb[0]
 *
 * Note that only zb->state[0].zb is a "real" zb pointer. If the pointer
 * reference is needed, use the BASEZB() macro below.
 *
 * @param zb0  : primary (root) zb structure
 * @param zb   : secondary zb structure to link to the root
 * @return int error if conditions aren't met
 */

#define	BASEZB(zb)	zb->state[0].zb

int cxip_zbcoll_simlink(struct cxip_zbcoll_obj *zb0,
			struct cxip_zbcoll_obj *zb1)
{
	int i;

	if (zb0 == zb1)
		return FI_SUCCESS;
	if (!zb0 || !zb1) {
		CXIP_WARN("arguments cannot be NULL\n");
		return -FI_EINVAL;
	}
	if (zb0->num_caddrs != zb1->num_caddrs) {
		CXIP_WARN("address counts do not match\n");
		return -FI_EINVAL;
	}
	for (i = 0; i < zb0->num_caddrs; i++)
		if (!CXIP_ADDR_EQUAL(zb0->caddrs[i], zb1->caddrs[i])) {
			CXIP_WARN("address values do not match caddr[%d]\n", i);
			return -FI_EINVAL;
		}
	/* zb0 must be root */
	if (zb0->simrank != 0) {
		CXIP_WARN("zb0 simrank != 0\n");
		return -FI_EINVAL;
	}
	/* zb1 must be valid simrank */
	if (zb1->simrank <= 0 || zb1->simrank >= zb1->num_caddrs) {
		CXIP_WARN("zb1 simrank %d invalid, max = %d\n",
			  zb1->simrank, zb1->num_caddrs);
		return -FI_EINVAL;
	}
	/* may only link once for a simrank */
	if (zb0->state[zb1->simrank].zb != zb0) {
		CXIP_WARN("zb0 state[%d] cannot be re-linked\n", zb1->simrank);
		return -FI_EINVAL;
	}
	/* may not re-link after linking is done */
	if (zb1->state[0].zb != zb1) {
		CXIP_WARN("zb1 state[0] cannot be re-linked\n");
		return -FI_EINVAL;
	}

	/* link each to the other */
	zb0->simref++;
	zb0->state[zb1->simrank].zb = zb1;
	zb1->state[zb0->simrank].zb = zb0;

	return FI_SUCCESS;
}

/* utility to free a zbcoll object */
static void _free_zbcoll(struct cxip_zbcoll_obj *zb)
{
	int i;

	if (zb->state)
		for (i = 0; i < zb->simcount; i++)
			free(zb->state[i].relatives);
	cxip_zbcoll_rlsgroup(zb);
	free(zb->caddrs);
	free(zb->state);
	free(zb->shuffle);
	free(zb);
}

/**
 * @brief Free zb object.
 *
 * This flushes the callback stack, and releases the group identifier associated
 * with this zb object. It also removes the backreference in the multi-zb
 * simulation.
 *
 * In the multi-zb simulation, it must defer actual deletion until all of the zb
 * objects in the collective have been deleted, since the tree may still be in
 * use until all of have deleted.
 *
 * @param zb : zb object to free
 */
void cxip_zbcoll_free(struct cxip_zbcoll_obj *zb)
{
	int i;

	if (!zb)
		return;

	/* edge case in some tests */
	if (!zb->state) {
		_free_zbcoll(zb);
		return;
	}
	if (zb->simrank >= 0) {
		zb = BASEZB(zb);
		if (--zb->simref)
			return;
		for (i = 1; i < zb->simcount; i++) {
			_free_zbcoll(zb->state[i].zb);
		}
	}
	_free_zbcoll(zb);
}

/* configure the zb object -- error frees zb in caller */
static int _state_config(struct cxip_zbcoll_obj *zb)
{
	struct cxip_zbcoll_state *zbs;
	int radix, n;

	radix = cxip_env.zbcoll_radix;

	zb->state = calloc(zb->simcount, sizeof(*zbs));
	if (!zb->state)
		return -FI_ENOMEM;

	for (n = 0; n < zb->simcount; n++) {
		zbs = &zb->state[n];
		zbs->zb = zb;

		/* do not create relatives if no addrs */
		if (!zb->num_caddrs)
			continue;

		/* simcount == 1, production: user specifies rank
		 * simcount >  1, simulation: each state has its own rank
		 */
		zbs->grp_rank = (zb->simcount == 1) ? zb->simrank : n;

		/* create space for relatives */
		zbs->relatives = calloc(radix + 1, sizeof(*zbs->relatives));
		if (!zbs->relatives)
			return -FI_ENOMEM;

		/* This produces indices in an abstract tree */
		zbs->num_relatives =
			cxip_tree_relatives(radix, zbs->grp_rank,
					    zb->num_caddrs, zbs->relatives);
	}
	return FI_SUCCESS;
}

/* sort out the various configuration cases -- error frees zb in caller */
static int _zbcoll_config(struct cxip_zbcoll_obj *zb, int num_addrs,
			  fi_addr_t *fiaddrs)
{
	int i, ret;

	if (!num_addrs) {
		/* test case: no nics, send-to-self only */
		zb->num_caddrs = 1;
		zb->caddrs = calloc(zb->num_caddrs, sizeof(*zb->caddrs));
		if (!zb->caddrs)
			return -FI_ENOMEM;
		zb->caddrs[0] = zb->ep_obj->src_addr;
		zb->simrank = 0;
		zb->simcount = 1;
	} else if (zb->simrank != ZB_NOSIM) {
		/* test case: regression with simulated addresses */
		if (num_addrs > ZB_SIM_MAX || zb->simrank >= num_addrs) {
			CXIP_WARN("Simulation maximum size = %d\n",
				  MIN(num_addrs, ZB_SIM_MAX));
			return -FI_EINVAL;
		}
		zb->num_caddrs = num_addrs;
		zb->caddrs = calloc(zb->num_caddrs, sizeof(*zb->caddrs));
		if (!zb->caddrs)
			return -FI_ENOMEM;
		for (i = 0; i < num_addrs; i++) {
			zb->caddrs[i].nic = i;
			zb->caddrs[i].pid = zb->ep_obj->src_addr.pid;
		}
		zb->simcount = num_addrs;
	} else {
		/* production case: real addresses supplied */
		zb->num_caddrs = num_addrs;
		zb->caddrs = calloc(zb->num_caddrs, sizeof(*zb->caddrs));
		if (!zb->caddrs)
			return -FI_ENOMEM;
		zb->simrank = -1;
		for (i = 0; i < num_addrs; i++) {
			ret = cxip_av_lookup_addr(zb->ep_obj->av,
						  fiaddrs[i], &zb->caddrs[i]);
			if (ret) {
				CXIP_WARN("Lookup on fiaddr=%ld failed\n",
					  fiaddrs[i]);
				return -FI_ECONNREFUSED;
			}
			if (zb->simrank < 0 &&
			    CXIP_ADDR_EQUAL(zb->caddrs[i],
					    zb->ep_obj->src_addr))
				zb->simrank = i;
		}
		if (zb->simrank < 0) {
			CXIP_WARN("Endpoint addr not in addrs[]\n");
			return -FI_ECONNREFUSED;
		}
		zb->simcount = 1;
	}

	/* find the index of the source address in the address list */
	return _state_config(zb);
}

/**
 * @brief Allocate and configure a zb object.
 *
 * The zb object represents a radix tree through multiple nics that can perform
 * sequential synchronizing collectives. It can be reused.
 *
 * This supports several test modes.
 *
 * If num_nics == 0, the zb object can only be used to test cxip_zbcoll_send(),
 * to exercise a send-to-self using the ctrl channel, and will work with NETSIM.
 *
 * If simrank is ZB_NOSIM, this will be used to perform real collectives over
 * the group specified by the specified nics. The self-address of the node
 * calling this must be a member of this set.
 *
 * If simrank is ZB_ALLSIM, this will be used to perform an internal simulation
 * of all the nics with a single call to a collective operation.
 *
 * If simrank is >= 0, then it represents the rank to be simulated by this zb
 * object. The test will need to create num_nics zb objects, each with a
 * different rank, and the zb collective operation will have to be initiated on
 * each of these to complete the collective.
 *
 * Simulation is limited to (1 << ZB_SIM_BITS) simulated endpoints. Simulation
 * also reduces the number of group identifiers that can be used.
 *
 * nid[0] is defined as the collective root nid.
 *
 * @param ep_obj    : NIC endpoint object
 * @param num_addrs : number of fabric addresses
 * @param fiaddrs   : fabric addresses
 * @param simrank   : simulated rank
 * @param zbp       : returned zb object
 * @return int : FI_SUCCESS or error value
 */
int cxip_zbcoll_alloc(struct cxip_ep_obj *ep_obj,
		      int num_addrs, fi_addr_t *fiaddrs, int simrank,
		      struct cxip_zbcoll_obj **zbp)
{
	struct cxip_zbcoll_obj *zb;
	int ret;

	if (!zbp) {
		CXIP_WARN("zbp is NULL\n");
		return -FI_EINVAL;
	}

	/* allocate the zb object */
	*zbp = NULL;
	zb = calloc(1, sizeof(*zb));
	if (!zb)
		return -FI_ENOMEM;
	dlist_init(&zb->ready_link);
	zb->ep_obj = ep_obj;
	zb->grpmskp = &ep_obj->zbcoll.grpmsk;
	zb->grpid = ZB_NEG_BIT;
	zb->simrank = simrank;
	zb->simref = 1;

	/* configure the zb object */
	ret = _zbcoll_config(zb, num_addrs, fiaddrs);
	if (ret) {
		cxip_zbcoll_free(zb);
		CXIP_WARN("Failed to configure zb object = %s\n",
			  fi_strerror(-ret));
		return ret;
	}

	/* return the zb object */
	*zbp = zb;
	return FI_SUCCESS;
}

/**
 * Data packing structures.
 *
 * This defines the specific bit meanings in the 64-bit zb put packet. Bit
 * mapping could be modified, see considerations below.
 *
 * Considerations for the (production) network field:
 *
 * - dat MUST hold a multicast address and hardware root data
 * - grpid size limits the number of concurrent zbcoll operations
 * - sim requires only one bit and applies only to devel testing
 * - pad is fixed by the control channel implementation
 *
 * Implementation of the negotiation operation requires that dat contain a
 * bitmap. The choice of 54 allows for 54 grpid values (0-53), which will fit
 * into a 6-bit grpid value. This is a large number for concurrencies. The grpid
 * field could be reduced to 5 bits, offering only 32 concurrent operations. The
 * map bits should then be reduced to 32, which would free up 23 bits for other
 * information during negotiation, should extra bits be required.
 *
 * For broadcast, the full dat field is available for multicast information. The
 * multicast address is currently 13 bits. Future revisions of Rosetta may
 * increase this. The remaining bits can be used for a representation of the
 * root node. A full caddr would require 32 bits, while using a 32-bit index
 * into the fi_av_set would allow for a collective spanning up to 4 billion
 * endpoints. This allows the multicast address to expand by another 9 bits, for
 * a total of 22 bits, or 4 million multicast addresses.
 *
 * Considerations for the simulation fields:
 *
 * - src and dst must have the same number of bits
 * - src/dst bits constrain the size of the simulated zbcoll tree
 */
union packer {
	struct {
		uint64_t dat: (ZB_MAP_BITS - 2*ZB_SIM_BITS);
		uint64_t src: ZB_SIM_BITS;
		uint64_t dst: ZB_SIM_BITS;
		uint64_t grpid: ZB_GRPID_BITS;
		uint64_t sim: 1;
		uint64_t pad: 3;
	} sim __attribute__((__packed__));
	struct {
		uint64_t dat: ZB_MAP_BITS;
		uint64_t grpid: ZB_GRPID_BITS;
		uint64_t sim: 1;
		uint64_t pad: 3;
	} net __attribute__((__packed__));
	uint64_t raw;
};


/* pack data */
static inline uint64_t zbpack(int sim, int src, int dst, int grpid,
			      uint64_t dat)
{
	union packer x = {.raw = 0};
	if (sim) {
		x.sim.sim = 1;
		x.sim.src = src;
		x.sim.dst = dst;
		x.sim.grpid = grpid;
		x.sim.dat = dat;
	} else {
		x.sim.sim = 0;
		x.net.grpid = grpid;
		x.net.dat = dat;
	}
	return x.raw;
}

/* unpack data */
static inline int zbunpack(uint64_t data, int *src, int *dst, int *grpid,
			   uint64_t *dat)
{
	union packer x = {.raw = data};
	if (x.sim.sim) {
		*src = x.sim.src;
		*dst = x.sim.dst;
		*grpid = x.sim.grpid;
		*dat = x.sim.dat;
	} else {
		*src = 0;
		*dst = 0;
		*grpid = x.net.grpid;
		*dat = x.net.dat;
	}
	return x.sim.sim;
}

/**
 * zbcoll state machine.
 *
 * The zbcollectives are intended to perform necessary synchronization among all
 * NIDs participating in a fi_join_collective() operation. Every join will have
 * its own set of NIDs, which may overlap with the NIDs used in another
 * concurrently-executing fi_join_collective(). Thus, every NID may be
 * participating simultaneously in a different number of join operations.
 *
 * Every process (NID) in the collective sits somewhere in a radix tree, with
 * one parent as relative[0] (except for the root), and up to RADIX-1 children
 * at relative[1,...].
 *
 * The collective follows a two-stage data flow, first from children toward the
 * root (upstream), then from root toward the children (downstream).
 *
 * Processes (NIDs) must wait for all children to report before forwarding their
 * own contribution toward the root. When the children of the root all report,
 * the root reflects the result back to its children, and completes. As each
 * child receives from its parent, it propagates the result to its children, and
 * completes.
 *
 * Packets are unrestricted, and thus receive confirmation ACK messages from the
 * hardware, or NAK and retry if delivery fails.
 *
 * The leaf (childless) NIDs contribute immediately and send the zb->dataval
 * data upstream. Each parent collects data from its children and bitwise-ANDs
 * the data with its own zb->dataval. When all children have reported to the
 * root, the root sends the root contents of *zb->dataptr downstream, and the
 * children simply propagate the received data to the leaves. This fixed
 * behavior covers all our use-cases.
 *
 * For the barrier operation, zb->dataptr is set to NULL, and zb->dataval is set
 * to zero. Both are effectively ignored.
 *
 * For the broadcast operation, zb->dataptr is a caller-supplied pointer, and
 * zb->dataval is ignored. When all contributions have arrived on the root, the
 * user-supplied value of *zb->dataptr is sent downstream, and propagated to all
 * leaves, overwriting *zb->dataptr on each endpoint.
 *
 * For the reduce operation, zb->dataptr is set to a caller-supplied pointer,
 * and zb->dataval is set to the value contained in this pointer. All of these
 * caller values are sent upstream and reduced using a bitwise-AND reduction.
 * When all contributions have arrived on the root, the value of the root
 * *zb->dataptr is overwritten with the reduced zb->dataval, and then propagated
 * to all leaves.
 *
 * Barrier, broadcast, and reduce must be preceded by a getgroup operation, to
 * obtain a grpid value for the zb object.
 *
 * For the getgroup operation, zb->dataptr points to &zb->dataval, and
 * zb->dataval contains a copy of the endpoint zbcoll grpmsk, which has a bit
 * set to 1 for every grpid that is available for that NID. NIDs may have
 * different grpmsk values. All of these masks are passed upstream through
 * zb->dataval in a bitwise-AND reduction. When it reaches the root, the set
 * bits in zb->dataval are the grpid values still available across all of the
 * NIDs in the group. Because zb->dataptr == &zb-dataval, *zb->dataptr on the
 * root contains the final reduced value, which is then propagated to all the
 * leaves.
 *
 * The negotiated group id is the lowest numbered bit still set, and every NID
 * computes this from the bitmask.
 *
 * It is possible for all group ID values to be exhausted. In this case, the
 * getgroup operation will report -FI_EBUSY, and the caller should retry until a
 * join operation completes, releasing one of the group ID values. If zb
 * collective objects are never released, new operations will be blocked
 * indefinitely.
 *
 * Getgroup operations are always serialized across the entire endpoint.
 * Attempting a second getgroup on any (new) zb object before the first has
 * completed will return -FI_EAGAIN. This is required to prevent race conditions
 * that would issue the same group id to multiple zbcoll objects.
 *
 * We are externally guaranteed that all fi_join_collective() operations will
 * observe proper collective ordering. Specifically, if any two joins share two
 * or more NIDs, those joins will be initiated in the same order on all shared
 * NIDs (possibly interspersed with other joins for unrelated groups). This
 * behavior is necessary to ensure that all NIDs in a group obtain the same
 * grpid value.
 */

/* send a zbcoll packet -- wrapper for cxip_ctrl_msg_send().
 *
 * Caller must hold ep_obj->lock.
 */
static void zbsend(struct cxip_ep_obj *ep_obj, uint32_t dstnic, uint32_t dstpid,
		   uint64_t mbv)
{
	struct cxip_ep_zbcoll_obj *zbcoll;
	struct cxip_ctrl_req *req;
	int ret;

	zbcoll = &ep_obj->zbcoll;

	req = calloc(1, sizeof(*req));
	if (!req) {
		CXIP_WARN("failed request allocation\n");
		ofi_atomic_inc32(&zbcoll->err_count);
		return;
	}

	req->ep_obj = ep_obj;
	req->cb = zbdata_send_cb;
	req->send.nic_addr = dstnic;
	req->send.pid = dstpid;
	req->send.vni = ep_obj->auth_key.vni;
	req->send.mb.raw = mbv;
	req->send.mb.ctrl_le_type = CXIP_CTRL_LE_TYPE_CTRL_MSG;
	req->send.mb.ctrl_msg_type = CXIP_CTRL_MSG_ZB_DATA;

	/* If we can't send, collective cannot complete, just spin */
	do {
		ret =  cxip_ctrl_msg_send(req);
		if (ret == -FI_EAGAIN)
			cxip_ep_ctrl_progress_locked(ep_obj);
	} while (ret == -FI_EAGAIN);
	if (ret) {
		CXIP_WARN("failed CTRL message send\n");
		ofi_atomic_inc32(&zbcoll->err_count);
	}
}

/* send a rejection packet */
static void reject(struct cxip_ep_obj *ep_obj, int dstnic, int dstpid,
		   int sim, int src, int dst, int grpid)
{
	union cxip_match_bits mb;

	mb.raw = zbpack(sim, src, dst, grpid, 0);
	zbsend(ep_obj, dstnic, dstpid, mb.raw);
}

/**
 * @brief Send a zero-buffer collective packet.
 *
 * Creates a request packet that must be freed (or retried) in callback.
 *
 * This can physically send ONLY from the endpoint source address, but the src
 * address can be provided for simulation.
 *
 * Only the lower bits of the 64-bit payload will be delivered, depending on the
 * specific packing model. Upper control bits will be overwritten as necessary.
 *
 * @param zb      : indexed zb structure
 * @param srcidx  : source address index (ignored unless simulating)
 * @param dstidx  : destination address index (required)
 * @param payload : packet value to send
 */
void cxip_zbcoll_send(struct cxip_zbcoll_obj *zb, int srcidx, int dstidx,
		      uint64_t payload)
{
	union cxip_match_bits mb = {.raw = 0};
	struct cxip_addr dstaddr;

	/* resolve NETSIM testcase */
	TRACE("SND %04x->%04x %016lx\n", srcidx, dstidx, payload);
	if (zb->simcount > 1) {
		if (dstidx >= zb->simcount) {
			ofi_atomic_inc32(&zb->ep_obj->zbcoll.err_count);
			return;
		}
		/* alter the data to pass srcaddr/dstaddr */
		mb.zb_data = zbpack(1, srcidx, dstidx, zb->grpid, payload);
		dstaddr = zb->ep_obj->src_addr;
	} else {
		/* srcidx, dstaddr are discarded in zbpack() */
		if (dstidx >= zb->num_caddrs) {
			ofi_atomic_inc32(&zb->ep_obj->zbcoll.err_count);
			return;
		}
		mb.zb_data = zbpack(0, 0, 0, zb->grpid, payload);
		dstaddr = zb->caddrs[dstidx];
	}
	zbsend(zb->ep_obj, dstaddr.nic, dstaddr.pid, mb.raw);
}

/* set the group ID */
static void setgrpid(struct cxip_zbcoll_obj *zb, uint64_t mask)
{
	uint64_t v;
	int grpid;

	TRACE("search for grpid in %016lx\n", mask);
	for (grpid = 0, v = 1; grpid <= ZB_NEG_BIT; grpid++, v<<=1)
		if (v & mask)
			break;
	TRACE("found grpid = %d\n", grpid);

	/* manage a rejection due to a transient race condition */
	if (grpid > ZB_NEG_BIT) {
		/* race condition reported */
		TRACE("cancel: getgroup transient race\n");
		zb->error = -FI_EAGAIN;
		return;
	}

	/* manage failure due to all grpid values in-use */
	if (grpid == ZB_NEG_BIT) {
		/* no group IDs available */
		TRACE("cancel: getgroup no grpid available\n");
		zb->error = -FI_EBUSY;
		return;
	}

	/* we found our group ID */
	TRACE("set grpid = %d\n", grpid);
	zb->grpid = grpid;
	_clrbit(zb->grpmskp, grpid);
}

/* mark a collective operation done */
static inline void zbdone(struct cxip_zbcoll_state *zbs, uint64_t mbv)
{
	struct cxip_zbcoll_obj *zb;
	struct cxip_ep_zbcoll_obj *zbcoll;

	/* getgroup:
	 *   single-zb sim: refcnt=1, busy=N
	 *   multi-zb  sim: refcnt=N, busy=1
	 *   production   : refcnt=1, busy=1
	 * reduction:
	 *   single-zb sim: refcnt=0, busy=N
	 *   multi-zb  sim: refcnt=0, busy=1
	 *   production   : refcnt=0, busy=1
	 */
	zb = zbs->zb;
	zbcoll = &zbs->zb->ep_obj->zbcoll;
	TRACE("%s: zb[%d] contribs=%d\n", __func__, zb->simrank, zbs->contribs);

	ofi_spin_lock(&zbcoll->lock);
	zbs->contribs = 0;
	TRACE("--REFCNT=%d in %s\n", zbcoll->refcnt, __func__);
	TRACE("--BUSY  =%d in %s\n", zb->busy, __func__);
	/* Reduce the refcnt when we are no longer busy */
	if (zb->busy && !--zb->busy) {
		if (zb->grpid == ZB_NEG_BIT)
			setgrpid(zb, mbv);
		/* Complete the negotiation on the last reference */
		if (!zbcoll->refcnt || !--zbcoll->refcnt) {
			if (zbcoll->grptbl[ZB_NEG_BIT] == BASEZB(zb)) {
				TRACE("GETGROUP FINISHED\n");
				zbcoll->grptbl[zb->grpid] = BASEZB(zb);
				zbcoll->grptbl[ZB_NEG_BIT] = NULL;
			}
		}
		TRACE(".. append to zb[%d]\n", zb->simrank);
		dlist_insert_tail(&zb->ready_link, &zbcoll->ready_list);
	}
	ofi_spin_unlock(&zbcoll->lock);
}

/* mark a collective send failure and end the collective */
static void zbsend_fail(struct cxip_zbcoll_state *zbs,
			struct cxip_ctrl_req *req, int ret)
{
	struct cxip_ep_zbcoll_obj *zbcoll;

	/* highly unexpected ret == -FI_EIO */
	zbcoll = &zbs->zb->ep_obj->zbcoll;
	ofi_atomic_inc32(&zbcoll->err_count);
	zbs->zb->error = ret;
	free(req);
}

/* root has no parent */
static inline bool isroot(struct cxip_zbcoll_state *zbs)
{
	return (zbs->relatives[0] < 0);
}

/* receive is complete when all contributors have spoken */
static inline bool rcvcomplete(struct cxip_zbcoll_state *zbs)
{
	return (zbs->contribs == zbs->num_relatives);
}

/* send upstream to the parent */
static void zbsend_up(struct cxip_zbcoll_state *zbs,
		      uint64_t mbv)
{
	TRACE("%04x->%04x: %-10s %-10s %d/%d\n",
		zbs->grp_rank, zbs->relatives[0], "", __func__,
		zbs->contribs, zbs->num_relatives);
	cxip_zbcoll_send(zbs->zb, zbs->grp_rank, zbs->relatives[0], mbv);
 }

/* send downstream to all of the children */
static void zbsend_dn(struct cxip_zbcoll_state *zbs,
		      uint64_t mbv)
{
	int relidx;

	for (relidx = 1; relidx < zbs->num_relatives; relidx++) {
		TRACE("%04x->%04x: %-10s %-10s\n",
			zbs->grp_rank, zbs->relatives[relidx],
			__func__, "");
		cxip_zbcoll_send(zbs->zb, zbs->grp_rank,
				 zbs->relatives[relidx], mbv);
	}
}

/* advance the upstream data flow, reverse direction at root */
static void advance(struct cxip_zbcoll_state *zbs, uint64_t mbv)
{
	union cxip_match_bits mb = {.raw = mbv};

	if (!rcvcomplete(zbs))
		return;

	if (isroot(zbs)) {
		/* Reduction overwrites root data */
		if (zbs->dataptr && zbs->zb->reduce)
			*zbs->dataptr = zbs->dataval;
		/* The root always reflects its data down */
		mb.zb_data = (zbs->dataptr) ? (*zbs->dataptr) : 0;
		zbsend_dn(zbs, mb.raw);
		zbdone(zbs, mbv);
	} else {
		/* completed children send up */
		zbsend_up(zbs, mbv);
	}
}

/* standard message for discarding a packet (should be rare) */
static void discard_msg(uint32_t inic, uint32_t ipid, char *msg)
{
	CXIP_WARN("discard: INI=%04x PID=%d: %s\n", inic, ipid, msg);
	TRACE("discard: INI=%04x PID=%d: %s\n", inic, ipid, msg);
}

/**
 * @brief zbcoll message receive callback.
 *
 * This is called by the cxip_ctrl handler when a ZB collective packet is
 * received. This function is "installed" at ep initialization, so it can begin
 * receiving packets before a zb object has been allocated to receive the data.
 * Races are handled by issuing a rejection packet back to the sender, which
 * results in a retry.
 *
 * All incoming packets pass through this function. The group identifier is part
 * of the packet format, and directs the packet to the zb object in the grptbl[]
 * associated with that grpid, which allows for multiple concurrent collective
 * operations.
 *
 * For the production case, there is only one zb associated with a grpid, with
 * one state entry. The source address is provided to us by the NIC, and the
 * destination is (obviously) this NIC.
 *
 * For the single-zb simulation case, there is only one zb associated with a
 * grpid, with a state entry for each simulated collective endpoint. The
 * simulated source and destination is present in the packet format, and this is
 * used to identify the source, and direct the packet to the correct destination
 * state object. The actual source address (always this NIC) is ignored.
 *
 * In the multi-zb simulation, there are multiple (linked) zb objects associated
 * with the grpid, each with a state entry for each simulated endpoint. The
 * grptbl[] only selects a single zb, which is the root (simrank=0) zb. Each
 * state in this object contains a backpointer that normally points to the
 * containing zb, but the linking operation modifies this to point to the
 * separate zb objects. So a simple redirection through the state backpointer
 * gets us to the correct zb and state within that zb. The linking operation
 * also modifies the state[0] entry in each of the different zb objects to point
 * back to the simrank=0 zb. The other state entries are unused. While this
 * requires an O(N^2) memory where only O(N) is used, we are fundamentally
 * limited to N=32 simulated endpoints by the space available in the packet for
 * addresses, so the waste is negligible.
 *
 * Calling code does not handle error returns gracefully, so handle all errors,
 * and return FI_SUCCESS.
 *
 * @param ep_obj    : endpoint
 * @param init_nic  : received (actual) initiator NIC
 * @param init_pid  : received (actual) initiator PID
 * @param mbv       : received match bits
 * @return int : FI_SUCCESS (formal return)
 */
int cxip_zbcoll_recv_cb(struct cxip_ep_obj *ep_obj, uint32_t init_nic,
			uint32_t init_pid, uint64_t mbv)
{
	struct cxip_ep_zbcoll_obj *zbcoll;
	struct cxip_zbcoll_obj *zb;
	struct cxip_zbcoll_state *zbs;
	int sim, src, dst, grpid;
	uint32_t inic, ipid;
	uint64_t dat;
	union cxip_match_bits mb = {.raw = mbv};
	int relidx;

	zbcoll = &ep_obj->zbcoll;
	/* src, dst always zero for production */
	sim = zbunpack(mbv, &src, &dst, &grpid, &dat);
	/* determine the initiator to use */
	if (sim) {
		inic = src;
		ipid = ep_obj->src_addr.pid;
	} else {
		inic = init_nic;
		ipid = init_pid;
	}
	TRACE("RCV INI=%04x PID=%04x sim=%d %d->%d grp=%d dat=%016lx\n",
	    inic, ipid, sim, src, dst, grpid, dat);

	/* discard if grpid is explicitly invalid (bad packet) */
	if (grpid > ZB_NEG_BIT) {
		discard_msg(inic, ipid, "rejected by target");
		ofi_atomic_inc32(&zbcoll->dsc_count);
		return FI_SUCCESS;
	}
	/* low-level packet test */
	if (zbcoll->disable) {
		/* Attempting a low-level test */
		ofi_atomic_inc32(&zbcoll->rcv_count);
		return FI_SUCCESS;
	}
	/* resolve the zb object */
	zb = zbcoll->grptbl[grpid];
	if (grpid == ZB_NEG_BIT) {
		/* This is a negotiation packet */
		if (!zb) {
			/* mask from downstream node, we aren't ready */
			TRACE("reject: getgroup negotiation conflict %08lx\n",
			      dat);
			reject(ep_obj, inic, ipid, sim, dst, src, grpid);
			return FI_SUCCESS;
		}
		if (!dat) {
			/* negotiation rejection from upstream node */
			zbs = &zb->state[dst];
			zbs->dataval = *zb->grpmskp;
			zbs->dataptr = &zbs->dataval;
			mb.zb_data = zbs->dataval;
			TRACE("rejected: re-send %016lx\n", mb.raw);
			zbsend_up(zbs, mb.zb_data);
			return FI_SUCCESS;
		}
		/* upstream/downstream mask for negotiating zb */
	} else {
		/* This is a collective packet */
		if (!zb) {
			/* Received packet for unknown group */
			discard_msg(inic, ipid, "reject unknown group ID");
			reject(ep_obj, inic, ipid, sim, dst, src, ZB_MAP_BITS);
			ofi_atomic_inc32(&zbcoll->dsc_count);
			return FI_SUCCESS;
		}
		/* upstream/downstream data for collective zb */
	}
	/* discard bad state indices */
	if (src >= zb->simcount || dst >= zb->simcount) {
		TRACE("discard: simsrc=%d simdst=%d\n", src, dst);
		CXIP_WARN("Bad simulation: src=%d dst=%d max=%d\n",
			  src, dst, zb->simcount);
		ofi_atomic_inc32(&zbcoll->dsc_count);
		return FI_SUCCESS;
	}
	/* set the state object */
	zbs = &zb->state[dst];
	/* simulation redirection for multi-zb simulation */
	if (zbs->zb != zb) {
		zb = zbs->zb;
		zbs = &zb->state[dst];
	}
	/* raw send test case, we are done */
	if (!zbs->num_relatives) {
		TRACE("ZBCOLL no relatives: test case\n");
		return FI_SUCCESS;
	}
	/* determine which relative this came from (upstream or downstream) */
	for (relidx = 0; relidx < zbs->num_relatives; relidx++) {
		if (inic == zb->caddrs[zbs->relatives[relidx]].nic &&
		    ipid == zb->caddrs[zbs->relatives[relidx]].pid)
			break;
	}
	if (relidx == zbs->num_relatives) {
		/* not a relative */
		discard_msg(inic, ipid, "reject initiator not in tree");
		reject(ep_obj, inic, ipid, sim, dst, src, grpid);
		ofi_atomic_inc32(&zbcoll->dsc_count);
		return FI_SUCCESS;
	}
	/* data received, increment the counter */
	ofi_atomic_inc32(&zbcoll->rcv_count);

	/* advance the state */
	if (relidx == 0) {
		/* downstream recv from parent */

		/* copy the data to the zbs */
		zbs->dataval = dat;
		if (zbs->dataptr)
			*zbs->dataptr = dat;
		TRACE("%04x<-%04x: %-10s %-10s %d/%d (%016lx)\n",
			zbs->grp_rank, zbs->relatives[0], "dn_recvd", "",
			zbs->contribs, zbs->num_relatives, dat);

		/* send downstream to children */
		zbsend_dn(zbs, mb.raw);
		zbdone(zbs, mb.raw);
	} else {
		/* upstream recv from child */

		/* bitwise-AND the upstream data value */
		zbs->dataval &= mb.raw;
		mb.zb_data = zbs->dataval;
		/* upstream packets contribute */
		zbs->contribs += 1;
		TRACE("%04x<-%04x: %-10s %-10s %d/%d\n",
			zbs->grp_rank, inic, "", "up_recvd", zbs->contribs,
			zbs->num_relatives);

		/* advance the collective */
		advance(zbs, mb.raw);
	}
	return FI_SUCCESS;
}

/**
 * @brief Send callback function to manage source ACK.
 *
 * The request must be retried, or freed.
 *
 * NETSIM will simply drop packets sent to non-existent addresses, which leaks
 * the request packet.
 *
 * Calling code does not handle error returns gracefully. Handle all errors, and
 * return FI_SUCCESS.
 *
 * @param req   : original request
 * @param event : CXI driver event
 * @return int  : FI_SUCCESS (formal return)
 */
static int zbdata_send_cb(struct cxip_ctrl_req *req, const union c_event *event)
{
	struct cxip_ep_zbcoll_obj *zbcoll;
	struct cxip_zbcoll_obj *zb;
	struct cxip_zbcoll_state *zbs;
	int src, dst, grpid;
	int sim __attribute__((unused));
	uint64_t dat;
	int ret;

	sim = zbunpack(req->send.mb.zb_data, &src, &dst, &grpid, &dat);
	TRACE("ACK sim=%d %d->%d grp=%d dat=%016lx\n",
	    sim, src, dst, grpid, dat);

	zbcoll = &req->ep_obj->zbcoll;
	ofi_atomic_inc32(&zbcoll->ack_count);

	if (grpid > ZB_NEG_BIT) {
		/* rejection packet sent */
		TRACE("ACK: rejection sent\n");
		goto done;
	}
	zb = zbcoll->grptbl[grpid];
	if (!zb) {
		/* Low-level testing, or ack is late */
		TRACE("ACK: late arrival\n");
		goto done;
	}
	if (src >= zb->simcount || dst >= zb->simcount) {
		TRACE("ACK: bad simulation\n");
		goto done;
	}
	zbs = &zb->state[dst];

	switch (event->hdr.event_type) {
	case C_EVENT_ACK:
		switch (cxi_event_rc(event)) {
		case C_RC_OK:
			ret = FI_SUCCESS;
			free(req);
			break;
		case C_RC_ENTRY_NOT_FOUND:
			/* likely a target queue is full, retry */
			CXIP_WARN("Target dropped packet, retry\n");
			usleep(cxip_env.fc_retry_usec_delay);
			ret = cxip_ctrl_msg_send(req);
			break;
		case C_RC_PTLTE_NOT_FOUND:
			/* could be a race during setup, retry */
			CXIP_WARN("Target connection failed, retry\n");
			usleep(cxip_env.fc_retry_usec_delay);
			ret = cxip_ctrl_msg_send(req);
			break;
		default:
			CXIP_WARN("ACK return code = %d, failed\n",
				  cxi_event_rc(event));
			ret = -FI_EIO;
			break;
		}
		break;
	default:
		/* fail the send */
		CXIP_WARN(CXIP_UNEXPECTED_EVENT,
			  cxi_event_to_str(event),
			  cxi_rc_to_str(cxi_event_rc(event)));
		ret = -FI_EIO;
		break;
	}
	if (ret != FI_SUCCESS)
		zbsend_fail(zbs, req, ret);

	return FI_SUCCESS;
done:
	free(req);
	return FI_SUCCESS;
}

/**
 * @brief Define the user callback function to execute on completion.
 *
 * @param zb
 * @param userfunc
 * @param userptr
 * @return int
 */
void cxip_zbcoll_set_user_cb(struct cxip_zbcoll_obj *zb,
			     zbcomplete_t userfunc, void *userptr)
{
	zb->userfunc = userfunc;
	zb->userptr = userptr;
}

/**
 * @brief Return the maximum number of groups for concurrent zbcoll operations.
 *
 * Maximum slots are ZB_NEG_BIT+1, with one reserved for negotiation. Using
 * simulation reduces the number of bits available for negotiation.
 *
 * @param sim  : true if nics are simulated
 * @return int maximum group ID value
 */
int cxip_zbcoll_max_grps(bool sim)
{
	return (!sim) ? ZB_NEG_BIT : ZB_NEG_BIT - 2*ZB_SIM_BITS;
}

/* used in each loop over states for each collective operation */
static bool _skip_or_shuffle(struct cxip_zbcoll_obj *zb, int i, int *n)
{
	/* default is that this returns n as value of i */
	*n = i;
	/* production means proceed over loop (of 1) with n = i */
	if (zb->simcount == 1)
		return false;
	/* multi-zb simulation should skip unless simrank == i */
	if (zb->simrank >= 0 && zb->simrank != i)
		return true;
	/* single-zb simulation simulates all values, with shuffling */
	if (zb->shuffle)
		*n = zb->shuffle[i];
	return false;
}

/**
 * @brief Negotiate a group id among participants.
 *
 * We are guaranteed that any two negotiations that take place on any two zb
 * objects will occur in the same order. However, either of those negotiations
 * could be separated by an arbitrary number of other negotiations for other
 * collectives that don't involve both of those zb objects. E.g.
 *
 * - zb1: A1 A2
 * - zb2: A1 B1 A2
 *
 * zb1 is able to start negotiation A2 as soon as A1 completes, but zb2 cannot
 * begin until B1 has completed. To prevent issuing the same grpid to two
 * different groups, or issuing different grpids to a single group, all getgroup
 * collectives are serialized over the NIC endpoint. Thus, attempting to
 * negotiate for A2 on zb2 before B1 has completed will result in -FI_EAGAIN.
 *
 * In production, each zb represents a different process, on a different NIC
 * endpoint, and these typically represent different compute nodes.
 *
 * In the single-zb and multi-zb simulations, the entire simulation is
 * single-threaded in a single process, in a common memory space.
 *
 * In the single-zb simulation, there is only one zb, and each zb->state[]
 * represents the different simulated collective endpoints. Operations across
 * all simulated endpoints are done sequentially, though the ordering is
 * randomized using the shuffle[] array.
 *
 * In the multi-zb simulation, there is a separate zb for each collective
 * endpoint. The same collective operation must be called independently on each
 * zb object, and all zb objects in that group must be called. Ordering is
 * controlled by the ordering of the operations using each zb.
 *
 * In production and the single-zb simulation, this is a simple first-come
 * first-served use of the NIC endpoint zbcoll->grptbl[ZB_NEG_BIT] pointer.
 * Serialization is guaranteed by simply testing whether grptbl[ZB_NEG_BIT] is
 * NULL.
 *
 * In the multi-zb simulation, acquiring zbcoll->grptbl[ZB_NEG_BIT] is a
 * multi-step process that requires multiple calls to getgroup, using different
 * (linked) zb objects. Serialization means that multiple calls must be allowed,
 * provided that they all belong to the same set of linked zb objects, until all
 * endpoints have been called. We use the refcnt value to determine when all
 * calls have been made.
 *
 * @param zb : zbcoll structure
 * @return int : FI_SUCCESS or error value
 */
int cxip_zbcoll_getgroup(struct cxip_zbcoll_obj *zb)
{
	struct cxip_ep_zbcoll_obj *zbcoll;
	struct cxip_zbcoll_state *zbs;
	union cxip_match_bits mb = {.raw = 0};
	int i, n, ret;

	/* function could be called by non-participating nodes */
	if (!zb) {
		TRACE("zb is NULL\n");
		CXIP_WARN("zb is NULL\n");
		return -FI_EINVAL;
	}

	/* if disabled, exit */
	zbcoll = &zb->ep_obj->zbcoll;
	if (zbcoll->disable) {
		TRACE("Disabled zb\n");
		return FI_SUCCESS;
	}

	/* check for already grouped */
	if (zb->grpid != ZB_NEG_BIT) {
		TRACE("grpid already set = %d\n", zb->grpid);
		CXIP_WARN("Cannot acquire a second group id\n");
		return -FI_EINVAL;
	}

	/* getgroup operations must be serialized */
	ret = FI_SUCCESS;
	ofi_spin_lock(&zbcoll->lock);
	if (!zbcoll->grptbl[ZB_NEG_BIT]) {
		/* free to start negotiating */
		zbcoll->grptbl[ZB_NEG_BIT] = BASEZB(zb);
		zbcoll->refcnt++;
	} else if (zbcoll->grptbl[ZB_NEG_BIT] == BASEZB(zb) &&
		   zbcoll->refcnt < zb->simcount &&
		   zb->busy < zb->simcount) {
		/* single-zb sim, refcnt=1, busy=simcount
		 * multi-zb  sim, refcnt=simcount, busy=1
		 */
		zbcoll->refcnt++;
		TRACE("continue grpid negotiation, refcnt=%d\n",
		      zbcoll->refcnt);
	} else {
		/* any other attempt has to wait */
		ret = -FI_EAGAIN;
		TRACE("failed grpid negotiation, retry later\n");
	}
	ofi_spin_unlock(&zbcoll->lock);
	TRACE("++REFCNT=%d ret=%d in %s\n", zbcoll->refcnt, ret, __func__);
	if (ret)
		return ret;

	/* process all states */
	zb->error = FI_SUCCESS;
	zb->reduce = false;
	for (i = 0; i < zb->simcount; i++) {
		if (_skip_or_shuffle(zb, i, &n))
			continue;
		zbs = &zb->state[n];
		zbs->dataval = *zb->grpmskp;
		zbs->dataptr = &zbs->dataval;
		zbs->contribs++;
		zb->busy++;
		TRACE("%s: zb[%d] contribs=%d\n", __func__, i, zbs->contribs);
		/* if terminal leaf node, will send up immediately */
		mb.zb_data = zbs->dataval;
		advance(zbs, mb.raw);
	}
	TRACE("++BUSY  =%d in %s\n", zb->busy, __func__);
	return FI_SUCCESS;
}

/**
 * @brief Release negotiated group id.
 *
 * @param zb : zbcoll structure
 */
void cxip_zbcoll_rlsgroup(struct cxip_zbcoll_obj *zb)
{
	struct cxip_ep_zbcoll_obj *zbcoll;

	if (!zb || zb->grpid > ZB_NEG_BIT)
		return;

	zbcoll = &zb->ep_obj->zbcoll;

	ofi_spin_lock(&zbcoll->lock);
	_setbit(zb->grpmskp, zb->grpid);
	zbcoll->grptbl[zb->grpid] = NULL;
	zb->grpid = ZB_NEG_BIT;
	ofi_spin_unlock(&zbcoll->lock);
}

/* All exported functions are variants of this core function */
static int _zbreduce(struct cxip_zbcoll_obj *zb, uint64_t *dataptr, bool reduce)
{
	struct cxip_ep_zbcoll_obj *zbcoll;
	struct cxip_zbcoll_state *zbs;
	union cxip_match_bits mb = {.raw = 0};
	int i, n;

	/* function could be called on non-participating NIDs */
	if (!zb) {
		TRACE("[-] zb is NULL\n");
		CXIP_WARN("zb is NULL\n");
		return -FI_EINVAL;
	}

	/* low level testing */
	zbcoll = &zb->ep_obj->zbcoll;
	if (zbcoll->disable) {
		TRACE("[%d] Disabled zb\n", zb->simrank);
		return FI_SUCCESS;
	}

	/* operations on a single zb_obj are serialized */
	if (zb->busy) {
		TRACE("[%d] busy\n", zb->simrank);
		return -FI_EAGAIN;
	}

	/* check for not grouped */
	if (zb->grpid >= ZB_NEG_BIT) {
		TRACE("[%d] Requires a group ID\n", zb->simrank);
		CXIP_WARN("Requires group id\n");
		return -FI_EINVAL;
	}
	TRACE("[%d] grpid = %d\n", zb->simrank, zb->grpid);

	/* process all states */
	zb->error = FI_SUCCESS;
	zb->reduce = reduce;
	/* Note that for simulation, dataptr must be an array */
	for (i = 0; i < zb->simcount; i++) {
		if (_skip_or_shuffle(zb, i, &n))
			continue;
		zbs = &zb->state[n];
		zbs->dataval = (dataptr) ? *dataptr : 0;
		zbs->dataptr = (dataptr) ? dataptr++ : NULL;
		zbs->contribs++;
		zb->busy++;
		TRACE("%s: zb[%d] contribs=%d\n", __func__, i, zbs->contribs);
		/* if terminal leaf node, will send up immediately */
		mb.zb_data = zbs->dataval;
		advance(zbs, mb.raw);
		TRACE("%s: zb[%d] contribs=%d\n", __func__, i, zbs->contribs);
	}
	TRACE("%s: busy=%d\n", __func__, zb->busy);

	return FI_SUCCESS;
}

/**
 * @brief Initiate a bitwise-AND reduction.
 *
 * All participants call this.
 *
 * On entry, *dataptr contains the data to be reduced. On return, *dataptr
 * contains the reduced data.
 *
 * NOTE: When testing in simulation, dataptr should reference an array of
 * uint64_t with one item for each endpoint.
 *
 * @param zb      : zbcoll structure
 * @param dataptr : pointer to return data
 * @return int    : FI_SUCCESS or error value
 */
int cxip_zbcoll_reduce(struct cxip_zbcoll_obj *zb, uint64_t *dataptr)
{
	return _zbreduce(zb, dataptr, true);
}

/**
 * @brief Initiate a broadcast from root to leaves.
 *
 * All participants call this.
 *
 * On entry, *dataptr on root contains the data to be broadcast.
 * On return, *dataptr contains the broadcast data from root.
 *
 * NOTE: When testing in simulation, dataptr should reference an array of
 * uint64_t with one item for each endpoint.
 *
 * @param zb      : zbcoll structure
 * @param dataptr : pointer to return data
 * @return int    : FI_SUCCESS or error value
 */
int cxip_zbcoll_broadcast(struct cxip_zbcoll_obj *zb, uint64_t *dataptr)
{
	return _zbreduce(zb, dataptr, false);
}

/**
 * @brief Initiate a no-data barrier.
 *
 * All participants call this.
 *
 * @param zb      : zbcoll structure
 * @return int : FI_SUCCESS or error value
 */
int cxip_zbcoll_barrier(struct cxip_zbcoll_obj *zb)
{
	return _zbreduce(zb, NULL, false);
}

/**
 * @brief Progress completion.
 *
 * This is called from cxip_coll_progress_join(), which is called when reading
 * the endpoint EQ as part of progressing the zb collective operation.
 *
 * The callback function can thus initiate new operations without concerns about
 * recursion.
 *
 * @param ep_obj : endpoint
 *
 * Caller holds eq_obj->lock.
 */
void cxip_ep_zbcoll_progress(struct cxip_ep_obj *ep_obj)
{
	struct cxip_zbcoll_obj *zb;
	struct cxip_ep_zbcoll_obj *zbcoll;

	zbcoll = &ep_obj->zbcoll;
	while (true) {
		/* progress the underlying ctrl transfers */
		cxip_ep_ctrl_progress_locked(ep_obj);

		/* see if there is a zb ready to be advanced */
		zb = NULL;
		ofi_spin_lock(&zbcoll->lock);
		if (!dlist_empty(&zbcoll->ready_list))
			dlist_pop_front(&zbcoll->ready_list,
					struct cxip_zbcoll_obj,
					zb, ready_link);
		ofi_spin_unlock(&zbcoll->lock);
		if (!zb)
			break;
		TRACE("SAW COMPLETION on zb[%d], error=%d!!!\n",
		    zb->simrank, zb->error);
		if (zb->userfunc)
			(zb->userfunc)(zb, zb->userptr);
	}
}

/**
 * @brief Intialize the zbcoll system.
 *
 * @param ep_obj : endpoint
 * @return int : FI_SUCCESS or error value
 */
int cxip_zbcoll_init(struct cxip_ep_obj *ep_obj)
{
	struct cxip_ep_zbcoll_obj *zbcoll;

	zbcoll = &ep_obj->zbcoll;
	memset(zbcoll, 0, sizeof(*zbcoll));
	dlist_init(&zbcoll->ready_list);
	zbcoll->grpmsk = -1ULL;
	zbcoll->grptbl = calloc(ZB_MAP_BITS, sizeof(void *));
	if (!zbcoll->grptbl)
		return -FI_ENOMEM;
	ofi_spin_init(&zbcoll->lock);
	ofi_atomic_initialize32(&zbcoll->dsc_count, 0);
	ofi_atomic_initialize32(&zbcoll->err_count, 0);
	ofi_atomic_initialize32(&zbcoll->ack_count, 0);
	ofi_atomic_initialize32(&zbcoll->rcv_count, 0);

	return FI_SUCCESS;
}

/**
 * @brief Cleanup all operations in progress.
 *
 * @param ep_obj : endpoint
 */
void cxip_zbcoll_fini(struct cxip_ep_obj *ep_obj)
{
	struct cxip_ep_zbcoll_obj *zbcoll;
	int i;

	zbcoll = &ep_obj->zbcoll;
	for (i = 0; i < ZB_MAP_BITS; i++)
		cxip_zbcoll_free(zbcoll->grptbl[i]);
	free(zbcoll->grptbl);
	zbcoll->grptbl = NULL;
}

/**
 * @brief Reset the endpoint counters.
 *
 * @param ep : endpoint
 */
void cxip_zbcoll_reset_counters(struct cxip_ep_obj *ep_obj)
{
	struct cxip_ep_zbcoll_obj *zbcoll;

	zbcoll = &ep_obj->zbcoll;
	ofi_atomic_set32(&zbcoll->dsc_count, 0);
	ofi_atomic_set32(&zbcoll->err_count, 0);
	ofi_atomic_set32(&zbcoll->ack_count, 0);
	ofi_atomic_set32(&zbcoll->rcv_count, 0);
}

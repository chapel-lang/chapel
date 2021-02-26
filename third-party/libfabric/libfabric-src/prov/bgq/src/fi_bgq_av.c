/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#include "rdma/bgq/fi_bgq.h"
#include <ofi_enosys.h>

#include "rdma/bgq/fi_bgq_spi.h"

static int fi_bgq_close_av(fid_t fid)
{
	int ret;
	struct fi_bgq_av *bgq_av =
		container_of(fid, struct fi_bgq_av, av_fid);

	ret = fi_bgq_fid_check(fid, FI_CLASS_AV, "address vector");
	if (ret)
		return ret;

	if (bgq_av->map_addr) free(bgq_av->map_addr);

	ret = fi_bgq_ref_dec(&bgq_av->domain->ref_cnt, "domain");
	if (ret)
		return ret;

	ret = fi_bgq_ref_finalize(&bgq_av->ref_cnt, "address vector");
	if (ret)
		return ret;

	free(bgq_av);
	return 0;
}

/*
 * The 'addr' is a representation of the address - not a string
 *
 * 'flags' is allowed to be ignored
 * 'context' is not used ... what is the purpose?
 */
static int
fi_bgq_av_insert(struct fid_av *av, const void *addr, size_t count,
	     fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct fi_bgq_av *bgq_av =
		container_of(av, struct fi_bgq_av, av_fid);

	if (!bgq_av) {
		errno = FI_EINVAL;
		return -errno;
	}

	switch (bgq_av->type) {
	case FI_AV_TABLE:
		/* The address table is internal and the application uses a
		 * 'monotonically increasing integer' to index the table and
		 * retrieve the actual internal address
		 */
		errno = FI_ENOSYS;
		return -errno;
		break;
	case FI_AV_MAP:
		/* The address map is maintained by the application ('fi_addr') and
		 * the provider must fill in the map with the actual network
		 * address of each .
		 */
		if (!addr) {
			errno = FI_EINVAL;
			return -errno;
		}
		break;
	default:
		errno = FI_EINVAL;
		return -errno;
	}

	BG_CoordinateMapping_t my_coords = bgq_av->domain->my_coords;
	BG_CoordinateMapping_t * your_coords = (BG_CoordinateMapping_t *) addr;
	union fi_bgq_addr * output = (union fi_bgq_addr *) fi_addr;
	uint32_t ppn = Kernel_ProcessCount();

	Personality_t personality;
	int rc;
	rc = Kernel_GetPersonality(&personality, sizeof(Personality_t));
	if (rc) {
		errno = FI_EINVAL;
		return -errno;
	}
	uint64_t dcr_value = DCRReadUser(ND_500_DCR(CTRL_CUTOFFS));


	uint32_t n;
	for (n=0; n<count; ++n) {

		const uint32_t fifo_map =
			fi_bgq_mu_calculate_fifo_map(my_coords, your_coords[n],
					&personality, dcr_value);

		const MUHWI_Destination_t destination =
			fi_bgq_spi_coordinates_to_destination(your_coords[n]);

		const uint32_t base_rx =
			fi_bgq_addr_calculate_base_rx(your_coords[n].t, ppn);

		output[n].fi = fi_bgq_addr_create(destination, fifo_map, base_rx);
	}

	return count;
}

static int
fi_bgq_av_insertsvc(struct fid_av *av, const char *node, const char *service,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct fi_bgq_av *bgq_av =
		container_of(av, struct fi_bgq_av, av_fid);

	if (!bgq_av) {
		errno = FI_EINVAL;
		return -errno;
	}

	switch (bgq_av->type) {
	case FI_AV_TABLE:
		/* The address table is internal and the application uses a
		 * 'monotonically increasing integer' to index the table and
		 * retrieve the actual internal address
		 */
		errno = FI_ENOSYS;
		return -errno;
		break;
	case FI_AV_MAP:
		/* The address map is maintained by the application ('fi_addr') and
		 * the provider must fill in the map with the actual network
		 * address of each .
		 */

		break;
	default:
		errno = FI_EINVAL;
		return -errno;
	}

	/*
	 * convert the string representation of the node ("#.#.#.#.#.#") into
	 * torus coordinates and the 't' coordinate.
	 */
	uint32_t a, b, c, d, e, t;
	const char * node_str = (const char *) node;
	sscanf(node_str, "%u.%u.%u.%u.%u.%u", &a, &b, &c, &d, &e, &t);
	BG_CoordinateMapping_t your_coords;
	your_coords.a = a;
	your_coords.b = b;
	your_coords.c = c;
	your_coords.d = d;
	your_coords.e = e;
	your_coords.t = t;

	BG_CoordinateMapping_t my_coords = bgq_av->domain->my_coords;

	const uint32_t fifo_map =
		fi_bgq_mu_calculate_fifo_map_single(my_coords, your_coords);

	const MUHWI_Destination_t destination =
		fi_bgq_spi_coordinates_to_destination(your_coords);

	const uint32_t base_rx =
		fi_bgq_addr_calculate_base_rx(your_coords.t, Kernel_ProcessCount());

	*fi_addr = fi_bgq_addr_create(destination, fifo_map, base_rx);

	return 0;
}

/*
 * This is similar to "ranks to coords" syscall. The "node" is the string
 * representation of the torus coordinates of a node and the 't' coordinate,
 * such as "0.0.0.0.0.0", and the "service" is the string representation of
 * what could be considered a pami-style "client id". Currently, only a single
 * "service" per "node" is supported - the service parameter is ignored and
 * a svccnt != 1 is considered an error.
 *
 * If the "node" parameter is NULL, then the insert begins at coordinate
 * 0.0.0.0.0.0 and increments according to the default ABCDET map order until
 * "nodecnt" addresses have been inserted. In this respect, "nodecnt" is the
 * same as the job size.
 *
 * The bgq provider does not support rank reorder via mapfiles.
 */
static int
fi_bgq_av_insertsym(struct fid_av *av, const char *node, size_t nodecnt,
		const char *service, size_t svccnt,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct fi_bgq_av *bgq_av =
		container_of(av, struct fi_bgq_av, av_fid);

	if (!bgq_av) {
		errno = FI_EINVAL;
		return -errno;
	}

	if (svccnt != 1) {
		fprintf(stderr, "Error. Only one 'service' per 'node' is supported by the bgq provider\n");
		errno = FI_EINVAL;
		return -errno;
	}

	switch (bgq_av->type) {
	case FI_AV_TABLE:
		/* The address table is internal and the application uses a
		 * 'monotonically increasing integer' to index the table and
		 * retrieve the actual internal address
		 */
		errno = FI_ENOSYS;
		return -errno;
		break;
	case FI_AV_MAP:
		/* The address map is maintained by the application ('fi_addr') and
		 * the provider must fill in the map with the actual network
		 * address of each .
		 */

		break;
	default:
		errno = FI_EINVAL;
		return -errno;
	}

	/*
	 * convert the string representation of the node ("#.#.#.#.#") into
	 * torus coordinates and convert the string representation of the
	 * service, a.k.a. "process", into a t coordinate.
	 */
	uint32_t a, b, c, d, e, t;
	if (node)
		sscanf(node, "%u.%u.%u.%u.%u.%u", &a, &b, &c, &d, &e, &t);
	else
		a = b = c = d = e = t = 0;

	Personality_t personality;
	int rc;
	rc = Kernel_GetPersonality(&personality, sizeof(Personality_t));
	if (rc) {
		errno = FI_EINVAL;	/* is this the correct errno? */
		return -errno;
	}
	uint32_t ppn = Kernel_ProcessCount();
	size_t node_count = personality.Network_Config.Anodes *
		personality.Network_Config.Bnodes *
		personality.Network_Config.Cnodes *
		personality.Network_Config.Dnodes *
		personality.Network_Config.Enodes *
		ppn;

	uint32_t maximum_to_insert = (node_count < nodecnt) ? node_count : nodecnt;

	BG_CoordinateMapping_t my_coords = bgq_av->domain->my_coords;
	BG_CoordinateMapping_t your_coords;
	uint64_t dcr_value = DCRReadUser(ND_500_DCR(CTRL_CUTOFFS));

	int n = 0;
	uint32_t _a, _b, _c, _d, _e, _t;
	union fi_bgq_addr * output = (union fi_bgq_addr *) fi_addr;
	for (_a = a; _a < personality.Network_Config.Anodes; ++_a) {
		your_coords.a = _a;
	for (_b = b; _b < personality.Network_Config.Bnodes; ++_b) {
		your_coords.b = _b;
	for (_c = c; _c < personality.Network_Config.Cnodes; ++_c) {
		your_coords.c = _c;
	for (_d = d; _d < personality.Network_Config.Dnodes; ++_d) {
		your_coords.d = _d;
	for (_e = e; _e < personality.Network_Config.Enodes; ++_e) {
		your_coords.e = _e;
	for (_t = t; _t < ppn; ++_t) {
		your_coords.t = _t;

		if (n == maximum_to_insert) break;

		const uint32_t fifo_map =
			fi_bgq_mu_calculate_fifo_map(my_coords, your_coords,
				&personality, dcr_value);

		const MUHWI_Destination_t destination =
			fi_bgq_spi_coordinates_to_destination(your_coords);

		const uint32_t base_rx =
			fi_bgq_addr_calculate_base_rx(your_coords.t, ppn);

		fi_addr[n++] = fi_bgq_addr_create(destination, fifo_map, base_rx);

	}}}}}}

	return n;
}

static int
fi_bgq_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count, uint64_t flags)
{
	return 0;	/* noop on bgq */
}

static int
fi_bgq_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr, size_t *addrlen)
{
	const union fi_bgq_addr bgq_addr = {.fi=fi_addr};

	BG_CoordinateMapping_t tmp;
	tmp.a = bgq_addr.uid.a;
	tmp.b = bgq_addr.uid.b;
	tmp.c = bgq_addr.uid.c;
	tmp.d = bgq_addr.uid.d;
	tmp.e = bgq_addr.uid.e;

	const uint32_t ppn = Kernel_ProcessCount();
	const uint32_t rx_per_node = ((BGQ_MU_NUM_REC_FIFO_GROUPS-1) * BGQ_MU_NUM_REC_FIFOS_PER_GROUP) / 2;	/* each rx uses two mu reception fifos */
	const uint32_t rx_per_process = rx_per_node / ppn;
	tmp.t = fi_bgq_addr_rec_fifo_id(bgq_addr.fi) / rx_per_process;

	memcpy(addr, (const void *)&tmp, *addrlen);

	*addrlen = sizeof(BG_CoordinateMapping_t);

	return 0;
}

static const char *
fi_bgq_av_straddr(struct fid_av *av, const void *addr,
			char *buf, size_t *len)
{
	BG_CoordinateMapping_t * input = (BG_CoordinateMapping_t *) addr;
	snprintf(buf, *len, "%u.%u.%u.%u.%u.%u", input->a, input->b, input->c,
		input->d, input->e, input->t);

	*len = 16;	/* "aa.bb.cc.dd.e.tt" */
	return buf;
}

static struct fi_ops fi_bgq_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_bgq_close_av,
	.bind		= fi_no_bind,
	.control	= fi_no_control,
	.ops_open	= fi_no_ops_open
};

int fi_bgq_bind_ep_av(struct fi_bgq_ep *bgq_ep,
		struct fi_bgq_av *bgq_av, uint64_t flags)
{
	if (bgq_ep->av) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_DOMAIN,
			"Address vector already bound to TX endpoint\n");
		errno = FI_EINVAL;
		return -errno;
	}

	bgq_ep->av = bgq_av;

	fi_bgq_ref_inc(&bgq_av->ref_cnt, "address vector");

	return 0;
}

static struct fi_ops_av fi_bgq_av_ops = {
	.size		= sizeof(struct fi_ops_av),
	.insert		= fi_bgq_av_insert,
	.insertsvc	= fi_bgq_av_insertsvc,
	.insertsym	= fi_bgq_av_insertsym,
	.remove		= fi_bgq_av_remove,
	.lookup		= fi_bgq_av_lookup,
	.straddr	= fi_bgq_av_straddr
};

int fi_bgq_av_open(struct fid_domain *dom,
		struct fi_av_attr *attr, struct fid_av **av,
		void *context)
{
	int ret;
	struct fi_bgq_av *bgq_av = NULL;

	if (!attr) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_AV,
				"no attr provided\n");
		errno = FI_EINVAL;
		return -errno;
	}

	ret = fi_bgq_fid_check(&dom->fid, FI_CLASS_DOMAIN, "domain");
	if (ret)
		return ret;

	bgq_av = calloc(1, sizeof(*bgq_av));
	if (!bgq_av) {
		errno = FI_ENOMEM;
		goto err;
	}

	bgq_av->av_fid.fid.fclass = FI_CLASS_AV;
	bgq_av->av_fid.fid.context= context;
	bgq_av->av_fid.fid.ops    = &fi_bgq_fi_ops;
	bgq_av->av_fid.ops 	  = &fi_bgq_av_ops;

	bgq_av->domain = (struct fi_bgq_domain *) dom;
	bgq_av->type = attr->type;

	bgq_av->map_addr = NULL;
	if (attr->name != NULL && (attr->flags & FI_READ)) {

		assert(0 == attr->map_addr);


		Personality_t personality;
		int rc;
		rc = Kernel_GetPersonality(&personality, sizeof(Personality_t));
		if (rc) {
			errno = FI_EINVAL;
			return -errno;
		}

		const uint32_t ppn = Kernel_ProcessCount();
		const size_t node_count = personality.Network_Config.Anodes *
			personality.Network_Config.Bnodes *
			personality.Network_Config.Cnodes *
			personality.Network_Config.Dnodes *
			personality.Network_Config.Enodes;

		size_t mapsize = node_count * ppn;
		BG_CoordinateMapping_t map[mapsize];
		uint64_t ep_count;	/* one endpoint per process */
		rc = Kernel_RanksToCoords(sizeof(map), map, &ep_count);

		fi_addr_t *addr = (fi_addr_t *)malloc(sizeof(fi_addr_t)*ep_count);	/* TODO - mmap this into shared memory */

		size_t n = 0;
		int i;

		BG_CoordinateMapping_t my_coords = bgq_av->domain->my_coords;
		uint64_t dcr_value = DCRReadUser(ND_500_DCR(CTRL_CUTOFFS));
		for (i=0;i<ep_count;i++) {

			const uint32_t fifo_map =
				fi_bgq_mu_calculate_fifo_map(my_coords, map[i],
					&personality, dcr_value);

			const MUHWI_Destination_t destination =
				fi_bgq_spi_coordinates_to_destination(map[i]);

			const uint32_t base_rx =
				fi_bgq_addr_calculate_base_rx(map[i].t, ppn);

			addr[n++] = fi_bgq_addr_create(destination, fifo_map, base_rx);
		}

		bgq_av->map_addr = (void *)addr;
		attr->map_addr = (void *)addr;
	}

	*av = &bgq_av->av_fid;

	fi_bgq_ref_init(&bgq_av->domain->fabric->node, &bgq_av->ref_cnt, "address vector");
	fi_bgq_ref_inc(&bgq_av->domain->ref_cnt, "domain");

	return 0;
err:
	if (bgq_av)
		free(bgq_av);
	return -errno;
}

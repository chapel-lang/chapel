/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2023 Cornelis Networks.
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
#include <ofi.h>

#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx.h"

#include <ofi_enosys.h>

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

static int fi_opx_close_av(fid_t fid)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_AV, "close av\n");

	int ret;
	struct fi_opx_av *opx_av =
		container_of(fid, struct fi_opx_av, av_fid);

	ret = fi_opx_fid_check(fid, FI_CLASS_AV, "address vector");
	if (ret)
		return ret;

	if (opx_av->map_addr) { /* not used */
		free(opx_av->map_addr);
		opx_av->map_addr = NULL;
	} 
	if (opx_av->table_addr) {
		free(opx_av->table_addr);
		opx_av->table_addr = NULL;
	}

	ret = fi_opx_ref_dec(&opx_av->domain->ref_cnt, "domain");
	if (ret)
		return ret;

	ret = fi_opx_ref_finalize(&opx_av->ref_cnt, "address vector");
	if (ret)
		return ret;

	free(opx_av);
	opx_av = NULL;
	//opx_av (the object passed in as fid) is now unusable

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_AV, "av closed\n");
	return 0;
}
ssize_t fi_opx_ep_tx_connect (struct fi_opx_ep *opx_ep, size_t count, union fi_opx_addr *peers,
		struct fi_opx_extended_addr *peers_ext);
/*
 * The 'addr' is a representation of the address - not a string
 *
 * 'flags' is allowed to be ignored
 * 'context' is not used ... what is the purpose?
 */
static int
fi_opx_av_insert(struct fid_av *av, const void *addr, size_t count,
	     fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct fi_opx_av *opx_av =
		container_of(av, struct fi_opx_av, av_fid);

	if (!opx_av) {
		FI_WARN(fi_opx_global.prov, FI_LOG_AV, "FI_EINVAL\n");
		errno = FI_EINVAL;
		return -errno;
	}

	uint32_t n, i, t;
	fi_addr_t * input = (fi_addr_t *) addr;
	const unsigned ep_tx_count = opx_av->ep_tx_count;

	switch (opx_av->type) {
	case FI_AV_TABLE:
		/* The address table is internal and the application uses a
		 * 'monotonically increasing integer' to index the table and
		 * retrieve the actual internal address
		 */
		if (!input) {
			FI_WARN(fi_opx_global.prov, FI_LOG_AV, "FI_ENOSYS\n");
			errno = FI_ENOSYS;
			return -errno;
		} else {
			if (opx_av->table_addr == NULL) {
				opx_av->table_count = count;
				if (posix_memalign((void**)&opx_av->table_addr, sizeof(union fi_opx_addr), sizeof(union fi_opx_addr) * opx_av->table_count)) {
					FI_WARN(fi_opx_global.prov, FI_LOG_AV, "FI_ENOMEM\n");
					errno = FI_ENOMEM;
					return -errno;
				}
			}
			if ((count + opx_av->addr_count) > opx_av->table_count) {
				union fi_opx_addr * opx_addr = opx_av->table_addr;
				assert(opx_addr!=NULL); /* realloc - can't be null */
				assert(opx_av->addr_count); /* relloc - can't be zero */
				opx_av->table_count = count + opx_av->table_count;
				if (posix_memalign((void**)&opx_av->table_addr, sizeof(union fi_opx_addr), sizeof(union fi_opx_addr) * opx_av->table_count)) {
					FI_WARN(fi_opx_global.prov, FI_LOG_AV, "FI_ENOMEM\n");
					errno = FI_ENOMEM;
					return -errno;
				}
				memcpy(opx_av->table_addr, opx_addr, sizeof(union fi_opx_addr) * opx_av->addr_count);
				free(opx_addr);
				opx_addr = NULL;
			}
			union fi_opx_addr * opx_addr = opx_av->table_addr;
			/* append <count> to table, starting at <addr_count> */
			for (n=0,t=opx_av->addr_count; n<count; ++n,++t) {
				if (fi_addr != NULL) fi_addr[n] = t;
				opx_addr[t].fi = input[n];
			}
			for (i=0; i<ep_tx_count; ++i) {
				fi_opx_ep_tx_connect(opx_av->ep_tx[i],(opx_av->addr_count+count),opx_addr, NULL);
			}
		}
		break;
	case FI_AV_MAP:
		/* The address map is maintained by the application ('fi_addr') and
		 * the provider must fill in the map with the actual network
		 * address of each .
		 */
		if (!input) {
			FI_WARN(fi_opx_global.prov, FI_LOG_AV, "FI_EINOSYS\n");
			errno = FI_ENOSYS;
			return -errno;
		} else if (opx_av->table_addr != NULL) {
			fprintf(stderr, "%s:%s():%d abort\n", __FILE__, __func__, __LINE__); abort();
		} else {
			union fi_opx_addr * output = (union fi_opx_addr *) fi_addr;
			struct fi_opx_extended_addr * output_ext = NULL;
			ssize_t rc;

			if (opx_av->ep_tx[0] == NULL ||
				!opx_av->ep_tx[0]->daos_info.hfi_rank_enabled) {
				for (n=0; n<count; ++n) {
					output[n].fi = input[n];
				}
			} else {
				struct fi_opx_extended_addr * input =
					(struct fi_opx_extended_addr *) addr;

				if (posix_memalign((void**)&output_ext, 32/*sizeof(struct fi_opx_extended_addr)*/,
						sizeof(struct fi_opx_extended_addr) * count)) {
					FI_WARN(fi_opx_global.prov, FI_LOG_AV, "FI_ENOMEM\n");
					errno = FI_ENOMEM;
					return -errno;
				}

				for (n=0; n<count; ++n) {
					output[n].fi = input[n].addr.fi;
					output_ext[n].rank = input[n].rank;
					output_ext[n].rank_inst = input[n].rank_inst;
				}
			}

			for (i=0; i<ep_tx_count; ++i) {
				rc = fi_opx_ep_tx_connect(opx_av->ep_tx[i], count, output, output_ext);
				if (OFI_UNLIKELY(rc)) {
					FI_WARN(fi_opx_global.prov, FI_LOG_AV, "FI_EAGAIN\n");
					errno = FI_EAGAIN;
					if (output_ext) {
						free(output_ext);
						output_ext = NULL;
					}
					return -errno;
				}
			}

			if (output_ext) {
				free(output_ext);
				output_ext = NULL;
			}
		}
		break;
	default:
		FI_WARN(fi_opx_global.prov, FI_LOG_AV, "FI_EINVAL\n");
		errno = FI_EINVAL;
		return -errno;
	}

	opx_av->addr_count += count;

	return count;
}

static int
fi_opx_av_insertsvc(struct fid_av *av, const char *node, const char *service,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct fi_opx_av *opx_av =
		container_of(av, struct fi_opx_av, av_fid);

	if (!opx_av) {
		errno = FI_EINVAL;
		return -errno;
	}

	switch (opx_av->type) {
	case FI_AV_TABLE:
		/* The address table is internal and the application uses a
		 * 'monotonically increasing integer' to index the table and
		 * retrieve the actual internal address
		 */
		break;
	case FI_AV_MAP:
		/* The address map is maintained by the application ('fi_addr') and
		 * the provider must fill in the map with the actual network
		 * address of each .
		 */
		errno = FI_ENOSYS;
		return -errno;
		break;
	default:
		errno = FI_EINVAL;
		return -errno;
	}

	FI_WARN(fi_opx_global.prov, FI_LOG_AV, "unimplemented\n");
	abort();
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
 * The opx provider does not support rank reorder via mapfiles.
 */
static int
fi_opx_av_insertsym(struct fid_av *av, const char *node, size_t nodecnt,
		const char *service, size_t svccnt,
		fi_addr_t *fi_addr, uint64_t flags, void *context)
{
	struct fi_opx_av *opx_av =
		container_of(av, struct fi_opx_av, av_fid);

	if (!opx_av) {
		errno = FI_EINVAL;
		return -errno;
	}

	if (svccnt != 1) {
		FI_WARN(fi_opx_global.prov, FI_LOG_AV,
			"Error. Only one 'service' per 'node' is supported by the opx provider\n");
		errno = FI_EINVAL;
		return -errno;
	}

	switch (opx_av->type) {
	case FI_AV_TABLE:
		/* The address table is internal and the application uses a
		 * 'monotonically increasing integer' to index the table and
		 * retrieve the actual internal address
		 */
		break;
	case FI_AV_MAP:
		/* The address map is maintained by the application ('fi_addr') and
		 * the provider must fill in the map with the actual network
		 * address of each .
		 */
		errno = FI_ENOSYS;
		return -errno;
		break;
	default:
		errno = FI_EINVAL;
		return -errno;
	}

	FI_WARN(fi_opx_global.prov, FI_LOG_AV, "unimplemented\n");
	abort();
	return 0;
}

static int
fi_opx_av_remove(struct fid_av *av, fi_addr_t *fi_addr, size_t count, uint64_t flags)
{
	return 0;	/* noop on opx */
}

static int
fi_opx_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr, size_t *addrlen)
{
	if (!addr || !addrlen) {
		errno = FI_EINVAL;
		return -errno;
	}

	struct fi_opx_av *opx_av =
		container_of(av, struct fi_opx_av, av_fid);

	if (opx_av->type == FI_AV_MAP) {

		memcpy(addr, (void*)&fi_addr, MIN(sizeof(fi_addr_t), *addrlen));

	} else {

		assert(opx_av->table_addr != NULL);
		memcpy(addr, (void*)&opx_av->table_addr[fi_addr], MIN(sizeof(union fi_opx_addr), *addrlen));
	}

	*addrlen = sizeof(union fi_opx_addr);

	return 0;
}

static const char *
fi_opx_av_straddr(struct fid_av *av, const void *addr,
			char *buf, size_t *len)
{
	if (!addr || !buf || !len) {
		errno = FI_EINVAL;
		return NULL;
	}

	struct fi_opx_av *opx_av =
		container_of(av, struct fi_opx_av, av_fid);
	char tmp[100];
	int n;

	if (opx_av->ep_tx[0] == NULL ||
		!opx_av->ep_tx[0]->daos_info.hfi_rank_enabled) {
		union fi_opx_addr * opx_addr = (union fi_opx_addr *)addr;
		/* Parse address with standard address format */
		n = 1 + snprintf(tmp, sizeof(tmp), "%04x.%04x.%02x.%02x.%02x.%02x",
			opx_addr->uid.lid,
			opx_addr->uid.endpoint_id, opx_addr->rx_index,
			opx_addr->hfi1_rx, opx_addr->hfi1_unit,
			opx_addr->reliability_rx);
	} else {
		struct fi_opx_extended_addr * opx_addr = (struct fi_opx_extended_addr *)addr;
		/* Parse address with extended address format - FI_ADDRESS.inst:rank*/
		n = 1 + snprintf(tmp, sizeof(tmp), "%04x.%04x.%02x.%02x.%02x.%02x.%04x:%d",
			opx_addr->addr.uid.lid,
			opx_addr->addr.uid.endpoint_id, opx_addr->addr.rx_index,
			opx_addr->addr.hfi1_rx, opx_addr->addr.hfi1_unit,
			opx_addr->addr.reliability_rx,
			opx_addr->rank_inst,
			opx_addr->rank);
	}

	memcpy(buf, tmp, MIN(n, *len));
	*len = n;

	return buf;
}

static struct fi_ops fi_opx_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_opx_close_av,
	.bind		= fi_no_bind,
	.control	= fi_no_control,
	.ops_open	= fi_no_ops_open
};

int fi_opx_bind_ep_av(struct fid_ep *ep,
		struct fid_av *av, uint64_t flags)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_AV, "bind av\n");

	struct fi_opx_ep *opx_ep =
		container_of(ep, struct fi_opx_ep, ep_fid);

	struct fi_opx_av *opx_av =
		container_of(av, struct fi_opx_av, av_fid);

	if (opx_ep->av) {
		FI_DBG(fi_opx_global.prov, FI_LOG_AV,
			"Address vector already bound to TX endpoint\n");
		errno = FI_EINVAL;
		return -errno;
	}

	if (opx_ep->ep_fid.fid.fclass != FI_CLASS_EP) {
		FI_DBG(fi_opx_global.prov, FI_LOG_AV,
			"Wrong type of endpoint\n");
		errno = FI_EINVAL;
		return -errno;
	}

	opx_ep->av = opx_av;

	const unsigned ep_tx_max = sizeof(opx_av->ep_tx) / sizeof(struct fi_opx_ep*);
	if (opx_av->ep_tx_count < ep_tx_max) {
		opx_av->ep_tx[opx_av->ep_tx_count++] = opx_ep;
	} else {
		FI_WARN(fi_opx_global.prov, FI_LOG_AV, "Too many ep tx contexts (max = %u)\n", ep_tx_max); abort();
	}

	fi_opx_ref_inc(&opx_av->ref_cnt, "address vector");

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_AV, "av bound to ep\n");
	return 0;
}

static struct fi_ops_av fi_opx_av_ops = {
	.size		= sizeof(struct fi_ops_av),
	.insert		= fi_opx_av_insert,
	.insertsvc	= fi_opx_av_insertsvc,
	.insertsym	= fi_opx_av_insertsym,
	.remove		= fi_opx_av_remove,
	.lookup		= fi_opx_av_lookup,
	.straddr	= fi_opx_av_straddr
};

int fi_opx_av_open(struct fid_domain *dom,
		struct fi_av_attr *attr, struct fid_av **av,
		void *context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_AV, "open av\n");

	int ret;
	struct fi_opx_av *opx_av = NULL;

	if (!attr) {
		errno = FI_EINVAL;
		goto err;
	}

	ret = fi_opx_fid_check(&dom->fid, FI_CLASS_DOMAIN, "domain");
	if (ret) {
		errno = ret;
		goto err;
	}

	if (attr->name != NULL && (attr->flags & FI_READ)) {
		/* named address vector not supported */
		errno = FI_EOPNOTSUPP;
		goto err;
	}
	
	if ((attr->type == FI_AV_TABLE) && (OPX_AV != FI_AV_MAP)) {
		/* allocate the av object now, allocate the address table at av_insert */
		opx_av = calloc(1, sizeof(*opx_av));
		if (!opx_av) {
			errno = FI_ENOMEM;
			goto err;
		}
	} else if ((attr->type == FI_AV_MAP) && (OPX_AV != FI_AV_TABLE)) {

		opx_av = calloc(1, sizeof(*opx_av));
		if (!opx_av) {
			errno = FI_ENOMEM;
			goto err;
		}
	} else if (attr->type == FI_AV_UNSPEC) {
		/* Depending on configure options, chose FI_AV_MAP or FI_AV_TABLE */
		attr->type = (OPX_AV == FI_AV_TABLE) ? FI_AV_TABLE : FI_AV_MAP;
		opx_av = calloc(1, sizeof(*opx_av));
		if (!opx_av) {
			errno = FI_ENOMEM;
			goto err;
		}
	}
	else {
		FI_DBG(fi_opx_global.prov, FI_LOG_AV,
				"Unsupported AV type requested\n");
		errno = FI_EINVAL;
		goto err;
	}

	opx_av->av_fid.fid.fclass = FI_CLASS_AV;
	opx_av->av_fid.fid.context= context;
	opx_av->av_fid.fid.ops    = &fi_opx_fi_ops;
	opx_av->av_fid.ops 	  = &fi_opx_av_ops;

	opx_av->domain = (struct fi_opx_domain *) dom;
	opx_av->type = attr->type;

	opx_av->ep_tx_count = 0;
	unsigned i, ep_tx_max = sizeof(opx_av->ep_tx) / sizeof(struct fi_opx_ep*);
	for (i=0; i<ep_tx_max; ++i)
		opx_av->ep_tx[i] = NULL;

	opx_av->map_addr = NULL;

	opx_av->rx_ctx_bits = attr->rx_ctx_bits;

	opx_av->addr_count = 0;
	opx_av->table_addr = NULL; /* table is separately allocated*/
	opx_av->table_count = 0;   /* count allocated */

	*av = &opx_av->av_fid;

	fi_opx_ref_inc(&opx_av->domain->ref_cnt, "domain");

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_AV, "av opened\n");
	return 0;
err:
	FI_WARN(fi_opx_global.prov, FI_LOG_AV, "errno %u\n", errno);
	if (opx_av) {
		free(opx_av);
		opx_av = NULL;
	}

	return -errno;
}

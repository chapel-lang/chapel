/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021 Cornelis Networks.
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

#include <string.h>

int fi_opx_getname(fid_t fid, void *addr, size_t *addrlen)
{
	if (addrlen == NULL) {
		errno = FI_EINVAL;
		return -errno;
	}

	const size_t len = *addrlen;

	switch(fid->fclass) {
	case FI_CLASS_EP:
		{
			struct fi_opx_ep *opx_ep;
			opx_ep = container_of(fid, struct fi_opx_ep, ep_fid);

			if (!opx_ep->daos_info.hfi_rank_enabled) {
				*addrlen = sizeof(union fi_opx_addr);
				if (len > 0) {
					if (!addr) {
						errno = FI_EINVAL;
						return -errno;
					}

					memcpy(addr, (void*)&opx_ep->rx->self, MIN(len, *addrlen));
				}
			} else {
				*addrlen = sizeof(struct fi_opx_extended_addr);
				if (len > 0) {
					if (!addr) {
						errno = FI_EINVAL;
						return -errno;
					}

					struct fi_opx_extended_addr *ext_addr = (struct fi_opx_extended_addr *)addr;
					memcpy(&ext_addr->addr, (void*)&opx_ep->rx->self, sizeof(union fi_opx_addr));
					ext_addr->rank = opx_ep->hfi->daos_info.rank;
					ext_addr->rank_inst = opx_ep->hfi->daos_info.rank_inst;
				}
			}

			if (len < *addrlen) {
				errno = FI_ETOOSMALL;
				return -errno;
			}
		}
		break;
	case FI_CLASS_SEP:
		*addrlen = sizeof(union fi_opx_addr);
		if (len > 0) {
			if (!addr) {
				errno = FI_EINVAL;
				return -errno;
			}

			struct fi_opx_sep *opx_sep;
			opx_sep = container_of(fid, struct fi_opx_sep, ep_fid);
			unsigned i;
			for (i = 0; (i < FI_OPX_ADDR_SEP_RX_MAX) && (opx_sep->ep[i] == NULL); ++i);
			if (i == FI_OPX_ADDR_SEP_RX_MAX) {
				/* no sep rx ctx were created? */
				errno = FI_EINVAL;
				return -errno;
			}

			union fi_opx_addr tmp;
			tmp.raw64b = 0;
			tmp.rx_index = 0;
			tmp.uid.endpoint_id = opx_sep->ep[i]->hfi->send_ctxt;
			tmp.reliability_rx = opx_sep->ep[i]->hfi->info.rxe.id;
			tmp.uid.lid = htons(opx_sep->ep[i]->hfi->lid);
			tmp.hfi1_rx = opx_sep->ep[i]->rx->self.hfi1_rx;
			tmp.hfi1_unit = opx_sep->ep[i]->rx->self.hfi1_unit;
			memcpy(addr, (void*)&tmp, MIN(len, *addrlen));
		}

		if (len < sizeof(union fi_opx_addr)) {
			errno = FI_ETOOSMALL;
			return -errno;
		}
		break;

	default:
		errno = FI_EINVAL;
		return -errno;
	}
	return 0;
}

static struct fi_ops_cm fi_opx_cm_ops = {
	.size		= sizeof(struct fi_ops_cm),
	.getname 	= fi_opx_getname,
	.getpeer 	= fi_no_getpeer,
	.connect 	= fi_no_connect,
	.listen  	= fi_no_listen,
	.accept  	= fi_no_accept,
	.reject  	= fi_no_reject,
	.shutdown 	= fi_no_shutdown,
};

int fi_opx_init_cm_ops(fid_t fid, struct fi_info *info)
{
	if (!info) goto err;

	struct fi_opx_ep *opx_ep;
	struct fi_opx_sep *opx_sep;

	switch(fid->fclass) {
    case FI_CLASS_RX_CTX:
    case FI_CLASS_TX_CTX:
	case FI_CLASS_EP:

		opx_ep = container_of(fid, struct fi_opx_ep, ep_fid);

		if (!opx_ep) goto err;

		opx_ep->ep_fid.cm = &fi_opx_cm_ops;
		break;
	case FI_CLASS_SEP:

		opx_sep = container_of(fid, struct fi_opx_sep, ep_fid);

		if (!opx_sep) goto err;

		opx_sep->ep_fid.cm = &fi_opx_cm_ops;
		break;

	default:
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, "Abort. Unknown fid class (%zu)\n", fid->fclass);
		abort();
		break;
	}


	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

int fi_opx_finalize_cm_ops(fid_t fid)
{
	return 0;
}

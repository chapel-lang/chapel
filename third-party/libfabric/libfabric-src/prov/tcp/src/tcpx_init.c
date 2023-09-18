/*
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *		- Redistributions of source code must retain the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer.
 *
 *		- Redistributions in binary form must reproduce the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer in the documentation and/or other materials
 *		  provided with the distribution.
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

#include <rdma/fi_errno.h>

#include <ofi_prov.h>
#include "tcpx.h"

#include <sys/types.h>
#include <ofi_util.h>
#include <stdlib.h>

static int tcpx_getinfo(uint32_t version, const char *node, const char *service,
			uint64_t flags, const struct fi_info *hints,
			struct fi_info **info)
{
	return ofi_ip_getinfo(&tcpx_util_prov, version, node, service, flags,
			      hints, info);
}

struct tcpx_port_range port_range = {
	.low  = 0,
	.high = 0,
};

int tcpx_nodelay = -1;

int tcpx_staging_sbuf_size = 9000;
int tcpx_prefetch_rbuf_size = 9000;
size_t tcpx_default_tx_size = 256;
size_t tcpx_default_rx_size = 256;
size_t tcpx_zerocopy_size = SIZE_MAX;


static void tcpx_init_env(void)
{
	size_t tx_size;
	size_t rx_size;

	/* Checked in util code */
	fi_param_define(&tcpx_prov, "iface", FI_PARAM_STRING,
			"Specify interface name");

	fi_param_define(&tcpx_prov,"port_low_range", FI_PARAM_INT,
			"define port low range");
	fi_param_define(&tcpx_prov,"port_high_range", FI_PARAM_INT,
			"define port high range");
	fi_param_get_int(&tcpx_prov, "port_high_range", &port_range.high);
	fi_param_get_int(&tcpx_prov, "port_low_range", &port_range.low);

	if (port_range.high > TCPX_PORT_MAX_RANGE)
		port_range.high = TCPX_PORT_MAX_RANGE;

	if (port_range.low < 0 || port_range.high < 0 ||
	    port_range.low > port_range.high) {
		FI_WARN(&tcpx_prov, FI_LOG_EP_CTRL,"User provided "
			"port range invalid. Ignoring. \n");
		port_range.low  = 0;
		port_range.high = 0;
	}

	fi_param_define(&tcpx_prov,"tx_size", FI_PARAM_SIZE_T,
			"define default tx context size (default: %zu)",
			tcpx_default_tx_size);
	fi_param_define(&tcpx_prov,"rx_size", FI_PARAM_SIZE_T,
			"define default rx context size (default: %zu)",
			tcpx_default_rx_size);
	if (!fi_param_get_size_t(&tcpx_prov, "tx_size", &tx_size)) {
		tcpx_default_tx_size = tx_size;
	}
	if (!fi_param_get_size_t(&tcpx_prov, "rx_size", &rx_size)) {
		tcpx_default_rx_size = rx_size;
	}

	fi_param_define(&tcpx_prov, "nodelay", FI_PARAM_BOOL,
			"overrides default TCP_NODELAY socket setting");
	fi_param_get_bool(&tcpx_prov, "nodelay", &tcpx_nodelay);

	fi_param_define(&tcpx_prov, "staging_sbuf_size", FI_PARAM_INT,
			"size of buffer used to coalesce iovec's or "
			"send requests before posting to the kernel, "
			"set to 0 to disable");
	fi_param_define(&tcpx_prov, "prefetch_rbuf_size", FI_PARAM_INT,
			"size of buffer used to prefetch received data from "
			"the kernel, set to 0 to disable");
	fi_param_define(&tcpx_prov, "zerocopy_size", FI_PARAM_SIZE_T,
			"lower threshold where zero copy transfers will be "
			"used, if supported by the platform, set to -1 to "
			"disable (default: %zu)", tcpx_zerocopy_size);
	fi_param_get_int(&tcpx_prov, "staging_sbuf_size",
			 &tcpx_staging_sbuf_size);
	fi_param_get_int(&tcpx_prov, "prefetch_rbuf_size",
			 &tcpx_prefetch_rbuf_size);
	fi_param_get_size_t(&tcpx_prov, "zerocopy_size", &tcpx_zerocopy_size);
}

static void fi_tcp_fini(void)
{
	/* empty as of now */
}

struct fi_provider tcpx_prov = {
	.name = "tcp",
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = tcpx_getinfo,
	.fabric = tcpx_create_fabric,
	.cleanup = fi_tcp_fini,
};

TCP_INI
{
#if HAVE_TCP_DL
	ofi_pmem_init();
#endif
	tcpx_init_env();
	return &tcpx_prov;
}

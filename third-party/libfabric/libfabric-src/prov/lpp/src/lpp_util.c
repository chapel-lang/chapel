/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include <time.h>

#include "lpp.h"

// LPP tunables (environment variables).
int lpp_disable_osbypass = 0;
int lpp_cq_overcommit = 0;
int lpp_domain_cleanup = 1;
int lpp_disable_dqp = 0;
int lpp_system_memcpy = 0;
int lpp_write_only = 1;
size_t lpp_max_wr_osbypass_size = 256;
size_t lpp_max_rd_osbypass_size = 128;
size_t lpp_max_eager_size = 2048;
size_t lpp_keep_alive_msec = 10000;
size_t lpp_keep_alive_retries = 3;
int lpp_disable_hmem_dma = 0;

int lpp_translate_errno(int prov_errno)
{
	prov_errno = abs(prov_errno);

	// One-to-one mappings.
	switch (prov_errno) {
	case 0:			return FI_SUCCESS;
	case EPERM:		return FI_EPERM;
	case ENOENT:		return FI_ENOENT;
	case EINTR:		return FI_EINTR;
	case EIO:		return FI_EIO;
	case E2BIG:		return FI_E2BIG;
	case EBADF:		return FI_EBADF;
	case EAGAIN:		return FI_EAGAIN;
	case ENOMEM:		return FI_ENOMEM;
	case EACCES:		return FI_EACCES;
	case EFAULT:		return FI_EFAULT;
	case EBUSY:		return FI_EBUSY;
	case ENODEV:		return FI_ENODEV;
	case EINVAL:		return FI_EINVAL;
	case EMFILE:		return FI_EMFILE;
	case ENOSPC:		return FI_ENOSPC;
	case ENOSYS:		return FI_ENOSYS;
	//case EWOULDBLOCK:	return FI_EWOULDBLOCK; // Same as EAGAIN
	case ENOMSG:		return FI_ENOMSG;
	case ENODATA:		return FI_ENODATA;
	case EOVERFLOW:		return FI_EOVERFLOW;
	case EMSGSIZE:		return FI_EMSGSIZE;
	case ENOPROTOOPT:	return FI_ENOPROTOOPT;
	case EOPNOTSUPP:	return FI_EOPNOTSUPP;
	case EADDRINUSE:	return FI_EADDRINUSE;
	case EADDRNOTAVAIL:	return FI_EADDRNOTAVAIL;
	case ENETDOWN:		return FI_ENETDOWN;
	case ENETUNREACH:	return FI_ENETUNREACH;
	case ECONNABORTED:	return FI_ECONNABORTED;
	case ECONNRESET:	return FI_ECONNRESET;
	case ENOBUFS:		return FI_ENOBUFS;
	case EISCONN:		return FI_EISCONN;
	case ENOTCONN:		return FI_ENOTCONN;
	case ESHUTDOWN:		return FI_ESHUTDOWN;
	case ETIMEDOUT:		return FI_ETIMEDOUT;
	case ECONNREFUSED:	return FI_ECONNREFUSED;
	case EHOSTDOWN:		return FI_EHOSTDOWN;
	case EHOSTUNREACH:	return FI_EHOSTUNREACH;
	case EALREADY:		return FI_EALREADY;
	case EINPROGRESS:	return FI_EINPROGRESS;
	case EREMOTEIO:		return FI_EREMOTEIO;
	case ECANCELED:		return FI_ECANCELED;
	case EKEYREJECTED:	return FI_EKEYREJECTED;
	}

	// Remappings
	switch (prov_errno) {
		case ERANGE:	return FI_ETOOSMALL;
	}

	return FI_EOTHER;
}

uint64_t lpp_get_time_ms()
{
	return ofi_gettime_ms();
}

static void lpp_init_params(void)
{
	fi_param_define(&lpp_prov, "disable_osbypass", FI_PARAM_BOOL, "Prevents provider from performing RMA reads/writes directly from userspace.");
	fi_param_get_bool(&lpp_prov, "disable_osbypass", &lpp_disable_osbypass);

	fi_param_define(&lpp_prov, "cq_overcommit", FI_PARAM_BOOL, "Allow IOs to start even if they may overrun the CQ.");
	fi_param_get_bool(&lpp_prov, "cq_overcommit", &lpp_cq_overcommit);

	fi_param_define(&lpp_prov, "domain_cleanup", FI_PARAM_BOOL, "Silently clean up domain resources upon closing.");
	fi_param_get_bool(&lpp_prov, "domain_cleanup", &lpp_domain_cleanup);

	fi_param_define(&lpp_prov, "disable_dqp", FI_PARAM_BOOL, "Do not automatically assign DQPs.");
	fi_param_get_bool(&lpp_prov, "disable_dqp", &lpp_disable_dqp);

	fi_param_define(&lpp_prov, "system_memcpy", FI_PARAM_BOOL, "Use the system libc memcpy.");
	fi_param_get_bool(&lpp_prov, "system_memcpy", &lpp_system_memcpy);

	fi_param_define(&lpp_prov, "write_only", FI_PARAM_BOOL, "Operate in write-only mode");
	fi_param_get_bool(&lpp_prov, "write_only", &lpp_write_only);

	fi_param_define(&lpp_prov, "max_wr_osbypass_size", FI_PARAM_SIZE_T, "The maximum size of OS bypass writes.");
	fi_param_get_size_t(&lpp_prov, "max_wr_osbypass_size", &lpp_max_wr_osbypass_size);

	fi_param_define(&lpp_prov, "max_rd_osbypass_size", FI_PARAM_SIZE_T, "The maximum size of OS bypass reads.");
	fi_param_get_size_t(&lpp_prov, "max_rd_osbypass_size", &lpp_max_rd_osbypass_size);

	fi_param_define(&lpp_prov, "max_eager_size", FI_PARAM_SIZE_T, "The maximum size of an eager msg send.");
	fi_param_get_size_t(&lpp_prov, "max_eager_size", &lpp_max_eager_size);

	fi_param_define(&lpp_prov, "keep_alive_msec", FI_PARAM_SIZE_T, "SRQ idle time before sending keep alive (in milliseconds).");
	fi_param_get_size_t(&lpp_prov, "keep_alive_msec", &lpp_keep_alive_msec);

	fi_param_define(&lpp_prov, "keep_alive_retries", FI_PARAM_SIZE_T, "Number of keep alives to send before declaring path dead.");
	fi_param_get_size_t(&lpp_prov, "keep_alive_retries", &lpp_keep_alive_retries);

	fi_param_define(&lpp_prov, "disable_hmem_dma", FI_PARAM_BOOL, "Copy hmem buf to host before RDZV protocol.");
	fi_param_get_bool(&lpp_prov, "disable_hmem_dma", &lpp_disable_hmem_dma);
}

static void lpp_fi_fini(void)
{
	return;
}

static const char lpp_prov_name[] = "lpp";

struct fi_provider lpp_prov = {
	.name       = lpp_prov_name,
	.version    = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo    = lpp_fi_getinfo,
	.fabric     = lpp_fi_fabric,
	.cleanup    = lpp_fi_fini
};

struct fi_provider *lpp_initialization(void)
{
	#if HAVE_LPP_DL
	ofi_hmem_init();
	ofi_params_init();
	#endif
	lpp_init_params();
	lpp_init_memcpy();

	return &lpp_prov;
}

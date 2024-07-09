/*
 * Copyright (c) 2013-2017 Intel Corporation. All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
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

#ifndef _OFI_PROV_H_
#define _OFI_PROV_H_

#include "config.h"

#include <rdma/providers/fi_prov.h>

/* Provider initialization function signature that built-in providers
 * must specify. */
#define INI_SIG(name) struct fi_provider* name(void)

/* for each provider defines for three scenarios:
 * dl: externally visible ctor with known name (see fi_prov.h)
 * built-in: ctor function def, don't export symbols
 * not built: no-op call for ctor
*/

#if (HAVE_CXI) && (HAVE_CXI_DL)
#  define CXI_INI FI_EXT_INI
#  define CXI_INIT NULL
#elif (HAVE_CXI)
#  define CXI_INI INI_SIG(fi_cxi_ini)
#  define CXI_INIT fi_cxi_ini()
CXI_INI ;
#else
#  define CXI_INIT NULL
#endif

/* If HAVE_EFA is defined on Windows, then the VisualStudio project configures
 * MSBuild to include the efa related files and exclude the verbs related files.
 * With the verbs related files excluded from the build, we need only ensure
 * that VERBS_INIT is NULL so that ofi_register_provider will skip it.
 */
#if defined(_WIN32) && (HAVE_EFA)
#  define VERBS_INIT NULL
#else
#  if (HAVE_VERBS) && (HAVE_VERBS_DL)
#    define VERBS_INI FI_EXT_INI
#    define VERBS_INIT NULL
#  elif (HAVE_VERBS)
#    define VERBS_INI INI_SIG(fi_verbs_ini)
#    define VERBS_INIT fi_verbs_ini()
VERBS_INI ;
#  else
#    define VERBS_INIT NULL
#  endif
#endif

#if (HAVE_EFA) && (HAVE_EFA_DL)
#  define EFA_INI FI_EXT_INI
#  define EFA_INIT NULL
#elif (HAVE_EFA)
#  define EFA_INI INI_SIG(fi_efa_ini)
#  define EFA_INIT fi_efa_ini()
EFA_INI ;
#else
#  define EFA_INIT NULL
#endif

#if (HAVE_PSM2) && (HAVE_PSM2_DL)
#  define PSM2_INI FI_EXT_INI
#  define PSM2_INIT NULL
#elif (HAVE_PSM2)
#  define PSM2_INI INI_SIG(fi_psm2_ini)
#  define PSM2_INIT fi_psm2_ini()
PSM2_INI ;
#else
#  define PSM2_INIT NULL
#endif

#if (HAVE_PSM3) && (HAVE_PSM3_DL)
#  define PSM3_INI FI_EXT_INI
#  define PSM3_INIT NULL
#elif (HAVE_PSM3)
#  define PSM3_INI INI_SIG(fi_psm3_ini)
#  define PSM3_INIT fi_psm3_ini()
PSM3_INI ;
#else
#  define PSM3_INIT NULL
#endif

#if (HAVE_SOCKETS) && (HAVE_SOCKETS_DL)
#  define SOCKETS_INI FI_EXT_INI
#  define SOCKETS_INIT NULL
#elif (HAVE_SOCKETS)
#  define SOCKETS_INI INI_SIG(fi_sockets_ini)
#  define SOCKETS_INIT fi_sockets_ini()
SOCKETS_INI ;
#else
#  define SOCKETS_INIT NULL
#endif

#if (HAVE_USNIC) && (HAVE_USNIC_DL)
#  define USNIC_INI FI_EXT_INI
#  define USNIC_INIT NULL
#elif (HAVE_USNIC)
#  define USNIC_INI INI_SIG(fi_usnic_ini)
#  define USNIC_INIT fi_usnic_ini()
USNIC_INI ;
#else
#  define USNIC_INIT NULL
#endif

#if (HAVE_UDP) && (HAVE_UDP_DL)
#  define UDP_INI FI_EXT_INI
#  define UDP_INIT NULL
#elif (HAVE_UDP)
#  define UDP_INI INI_SIG(fi_udp_ini)
#  define UDP_INIT fi_udp_ini()
UDP_INI ;
#else
#  define UDP_INIT NULL
#endif

#if (HAVE_TCP) && (HAVE_TCP_DL)
#  define TCP_INI FI_EXT_INI
#  define TCP_INIT NULL
#elif (HAVE_TCP)
#  define TCP_INI INI_SIG(fi_tcp_ini)
#  define TCP_INIT fi_tcp_ini()
TCP_INI ;
#else
#  define TCP_INIT NULL
#endif

#if (HAVE_RXM) && (HAVE_RXM_DL)
#  define RXM_INI FI_EXT_INI
#  define RXM_INIT NULL
#elif (HAVE_RXM)
#  define RXM_INI INI_SIG(fi_rxm_ini)
#  define RXM_INIT fi_rxm_ini()
RXM_INI ;
#else
#  define RXM_INIT NULL
#endif

#if (HAVE_RXD) && (HAVE_RXD_DL)
#  define RXD_INI FI_EXT_INI
#  define RXD_INIT NULL
#elif (HAVE_RXD)
#  define RXD_INI INI_SIG(fi_rxd_ini)
#  define RXD_INIT fi_rxd_ini()
RXD_INI ;
#else
#  define RXD_INIT NULL
#endif

#ifdef _WIN32
#if (HAVE_NETDIR) && (HAVE_NETDIR_DL)
#  define NETDIR_INI FI_EXT_INI
#  define NETDIR_INIT NULL
#elif (HAVE_NETDIR)
#  define NETDIR_INI INI_SIG(fi_netdir_ini)
#  define NETDIR_INIT fi_netdir_ini()
NETDIR_INI ;
#else
#  define NETDIR_INIT NULL
#endif
#else /* _WIN32 */
#  define NETDIR_INIT NULL
#endif /* _WIN32 */

#if (HAVE_SHM) && (HAVE_SHM_DL)
#  define SHM_INI FI_EXT_INI
#  define SHM_INIT NULL
#elif (HAVE_SHM)
#  define SHM_INI INI_SIG(fi_shm_ini)
#  define SHM_INIT fi_shm_ini()
SHM_INI ;
#else
#  define SHM_INIT NULL
#endif

#if (HAVE_SM2) && (HAVE_SM2_DL)
#  define SM2_INI FI_EXT_INI
#  define SM2_INIT NULL
#elif (HAVE_SM2)
#  define SM2_INI INI_SIG(fi_sm2_ini)
#  define SM2_INIT fi_sm2_ini()
SM2_INI ;
#else
#  define SM2_INIT NULL
#endif

#if (HAVE_MRAIL) && (HAVE_MRAIL_DL)
#  define MRAIL_INI FI_EXT_INI
#  define MRAIL_INIT NULL
#elif (HAVE_MRAIL)
#  define MRAIL_INI INI_SIG(fi_mrail_ini)
#  define MRAIL_INIT fi_mrail_ini()
MRAIL_INI ;
#else
#  define MRAIL_INIT NULL
#endif

#if (HAVE_PERF) && (HAVE_PERF_DL)
#  define HOOK_PERF_INI FI_EXT_INI
#  define HOOK_PERF_INIT NULL
#elif (HAVE_PERF)
#  define HOOK_PERF_INI INI_SIG(fi_hook_perf_ini)
#  define HOOK_PERF_INIT fi_hook_perf_ini()
HOOK_PERF_INI ;
#else
#  define HOOK_PERF_INIT NULL
#endif

#if (HAVE_TRACE) && (HAVE_TRACE_DL)
#  define HOOK_TRACE_INI FI_EXT_INI
#  define HOOK_TRACE_INIT NULL
#elif (HAVE_TRACE)
#  define HOOK_TRACE_INI INI_SIG(fi_hook_trace_ini)
#  define HOOK_TRACE_INIT fi_hook_trace_ini()
HOOK_TRACE_INI ;
#else
#  define HOOK_TRACE_INIT NULL
#endif

#if (HAVE_PROFILE) && (HAVE_PROFILE_DL)
#  define HOOK_PROFILE_INI FI_EXT_INI
#  define HOOK_PROFILE_INIT NULL
#elif (HAVE_PROFILE)
#  define HOOK_PROFILE_INI INI_SIG(fi_hook_profile_ini)
#  define HOOK_PROFILE_INIT fi_hook_profile_ini()
HOOK_PROFILE_INI ;
#else
#  define HOOK_PROFILE_INIT NULL
#endif


#if (HAVE_HOOK_DEBUG) && (HAVE_HOOK_DEBUG_DL)
#  define HOOK_DEBUG_INI FI_EXT_INI
#  define HOOK_DEBUG_INIT NULL
#elif (HAVE_HOOK_DEBUG)
#  define HOOK_DEBUG_INI INI_SIG(fi_debug_hook_ini)
#  define HOOK_DEBUG_INIT fi_debug_hook_ini()
HOOK_DEBUG_INI ;
#else
#  define HOOK_DEBUG_INIT NULL
#endif

#if (HAVE_HOOK_HMEM) && (HAVE_HOOK_HMEM_DL)
#  define HOOK_HMEM_INI FI_EXT_INI
#  define HOOK_HMEM_INIT NULL
#elif (HAVE_HOOK_HMEM)
#  define HOOK_HMEM_INI INI_SIG(fi_hook_hmem_ini)
#  define HOOK_HMEM_INIT fi_hook_hmem_ini()
HOOK_HMEM_INI ;
#else
#  define HOOK_HMEM_INIT NULL
#endif

#if (HAVE_DMABUF_PEER_MEM) && (HAVE_DMABUF_PEER_MEM_DL)
#  define HOOK_DMABUF_PEER_MEM_INI FI_EXT_INI
#  define HOOK_DMABUF_PEER_MEM_INIT NULL
#elif (HAVE_DMABUF_PEER_MEM)
#  define HOOK_DMABUF_PEER_MEM_INI INI_SIG(fi_dmabuf_peer_mem_hook_ini)
#  define HOOK_DMABUF_PEER_MEM_INIT fi_dmabuf_peer_mem_hook_ini()
HOOK_DMABUF_PEER_MEM_INI ;
#else
#  define HOOK_DMABUF_PEER_MEM_INIT NULL
#endif

#  define HOOK_NOOP_INI INI_SIG(fi_hook_noop_ini)
#  define HOOK_NOOP_INIT fi_hook_noop_ini()
HOOK_NOOP_INI ;

#if (HAVE_OPX) && (HAVE_OPX_DL)
#  define OPX_INI FI_EXT_INI
#  define OPX_INIT NULL
#elif (HAVE_OPX)
#  define OPX_INI INI_SIG(fi_opx_ini)
#  define OPX_INIT fi_opx_ini()
OPX_INI ;
#else
#  define OPX_INIT NULL
#endif

#if (HAVE_UCX) && (HAVE_UCX_DL)
#  define UCX_INI FI_EXT_INI
#  define UCX_INIT NULL
#elif (HAVE_UCX)
#  define UCX_INI INI_SIG(fi_ucx_ini)
#  define UCX_INIT fi_ucx_ini()
UCX_INI ;
#else
#  define UCX_INIT NULL
#endif

/* the utility collective provider is always enabled and built-in */
#define COLL_INI INI_SIG(fi_coll_ini)
#define COLL_INIT fi_coll_ini()
COLL_INI ;

#endif /* _OFI_PROV_H_ */

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

#if (HAVE_GNI) && (HAVE_GNI_DL)
#  define GNI_INI FI_EXT_INI
#  define GNI_INIT NULL
#elif (HAVE_GNI)
#  define GNI_INI INI_SIG(fi_gni_ini)
#  define GNI_INIT fi_gni_ini()
GNI_INI ;
#else
#  define GNI_INIT NULL
#endif

#if (HAVE_VERBS) && (HAVE_VERBS_DL)
#  define VERBS_INI FI_EXT_INI
#  define VERBS_INIT NULL
#elif (HAVE_VERBS)
#  define VERBS_INI INI_SIG(fi_verbs_ini)
#  define VERBS_INIT fi_verbs_ini()
VERBS_INI ;
#else
#  define VERBS_INIT NULL
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

#if (HAVE_PSM) && (HAVE_PSM_DL)
#  define PSM_INI FI_EXT_INI
#  define PSM_INIT NULL
#elif (HAVE_PSM)
#  define PSM_INI INI_SIG(fi_psm_ini)
#  define PSM_INIT fi_psm_ini()
PSM_INI ;
#else
#  define PSM_INIT NULL
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

#if (HAVE_BGQ) && (HAVE_BGQ_DL)
#  define BGQ_INI FI_EXT_INI
#  define BGQ_INIT NULL
#elif (HAVE_BGQ)
#  define BGQ_INI INI_SIG(fi_bgq_ini)
#  define BGQ_INIT fi_bgq_ini()
BGQ_INI ;
#else
#  define BGQ_INIT NULL
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

#if (HAVE_RSTREAM) && (HAVE_RSTREAM_DL)
#  define RSTREAM_INI FI_EXT_INI
#  define RSTREAM_INIT NULL
#elif (HAVE_RSTREAM)
#  define RSTREAM_INI INI_SIG(fi_rstream_ini)
#  define RSTREAM_INIT fi_rstream_ini()
RSTREAM_INI ;
#else
#  define RSTREAM_INIT NULL
#endif

#if(HAVE_PERF)
#  define HOOK_PERF_INI INI_SIG(fi_hook_perf_ini)
#  define HOOK_PERF_INIT fi_hook_perf_ini()
HOOK_PERF_INI ;
#else
#  define HOOK_PERF_INIT NULL
#endif

#if(HAVE_HOOK_DEBUG)
#  define HOOK_DEBUG_INI INI_SIG(fi_debug_hook_ini)
#  define HOOK_DEBUG_INIT fi_debug_hook_ini()
HOOK_DEBUG_INI ;
#else
#  define HOOK_DEBUG_INIT NULL
#endif

#  define HOOK_NOOP_INI INI_SIG(fi_hook_noop_ini)
#  define HOOK_NOOP_INIT fi_hook_noop_ini()
HOOK_NOOP_INI ;

#endif /* _OFI_PROV_H_ */

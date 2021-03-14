/*
 * Copyright (c) 2014, Cisco Systems, Inc. All rights reserved.
 *
 * LICENSE_BEGIN
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * LICENSE_END
 *
 *
 */

#include "usnic_direct.h"

const char *usd_link_state_str(enum usd_link_state state)
{
    switch (state) {
    case USD_LINK_DOWN: return "USD_LINK_DOWN";
    case USD_LINK_UP:   return "USD_LINK_UP";
    default:            return "UNKNOWN";
    }
}

const char *usd_completion_status_str(enum usd_completion_status cstatus)
{
    switch (cstatus) {
    case USD_COMPSTAT_SUCCESS:        return "USD_COMPSTAT_SUCCESS";
    case USD_COMPSTAT_ERROR_CRC:      return "USD_COMPSTAT_ERROR_CRC";
    case USD_COMPSTAT_ERROR_TRUNC:    return "USD_COMPSTAT_ERROR_TRUNC";
    case USD_COMPSTAT_ERROR_TIMEOUT:  return "USD_COMPSTAT_ERROR_TIMEOUT";
    case USD_COMPSTAT_ERROR_INTERNAL: return "USD_COMPSTAT_ERROR_INTERNAL";
    default:                          return "UNKNOWN";
    }
}

const char *usd_completion_type_str(enum usd_completion_type ctype)
{
    switch (ctype) {
    case USD_COMPTYPE_SEND:      return "USD_COMPTYPE_SEND";
    case USD_COMPTYPE_RECV:      return "USD_COMPTYPE_RECV";
    default:                     return "UNKNOWN";
    }
}

const char *usd_filter_type_str(enum usd_filter_type ftype)
{
    switch (ftype) {
    case USD_FTY_UDP:      return "USD_FTY_UDP";
    case USD_FTY_UDP_SOCK: return "USD_FTY_UDP_SOCK";
    case USD_FTY_TCP:      return "USD_FTY_TCP";
    case USD_FTY_MCAST:    return "USD_FTY_MCAST";
    case USD_FTY_8915:     return "USD_FTY_8915";
    default:               return "UNKNOWN";
    }
}

const char *usd_qp_transport_str(enum usd_qp_transport qpt)
{
    switch (qpt) {
    case USD_QTR_RAW: return "USD_QTR_RAW";
    case USD_QTR_UDP: return "USD_QTR_UDP";
    default:          return "UNKNOWN";
    }
}

const char *usd_qp_type_str(enum usd_qp_type qpt)
{
    switch (qpt) {
    case USD_QTY_UD: return "USD_QTY_UD";
    case USD_QTY_UD_PIO:    return "USD_QTY_UD_PIO";
    default:             return "UNKNOWN";
    }
}

const char *usd_qp_event_event_type_str(enum usd_device_event_type det)
{
    switch (det) {
    case USD_EVENT_LINK_UP:   return "USD_EVENT_LINK_UP";
    case USD_EVENT_LINK_DOWN: return "USD_EVENT_LINK_DOWN";
    default:                  return "UNKNOWN";
    }
}

const char *usd_send_flag_sift_str(enum usd_send_flag_shift sfs)
{
    switch (sfs) {
    case USD_SFS_SIGNAL: return "USD_SFS_SIGNAL";
    default:             return "UNKNOWN";
    }
}

const char *usd_capability(enum usd_capability cap)
{
    switch (cap) {
    case USD_CAP_CQ_SHARING: return "USD_CAP_CQ_SHARING";
    case USD_CAP_MAP_PER_RES: return "USD_CAP_MAP_PER_RES";
    case USD_CAP_PIO: return "USD_CAP_PIO";
    case USD_CAP_CQ_INTR: return "USD_CAP_CQ_INTR";
    case USD_CAP_GRP_INTR: return "USD_CAP_GRP_INTR";
    case USD_CAP_MAX:        return "USD_CAP_MAX";
    default:                 return "UNKNOWN";
    }
}

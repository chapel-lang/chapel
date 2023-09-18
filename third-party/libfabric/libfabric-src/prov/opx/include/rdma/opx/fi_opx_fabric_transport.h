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
#ifndef _FI_PROV_OPX_FABRIC_TRANSPORT_H_ 
#define _FI_PROV_OPX_FABRIC_TRANSPORT_H_

#define FI_OPX_FABRIC_HFI1

#ifdef FI_OPX_FABRIC_HFI1
#include "rdma/opx/fi_opx_hfi1_transport.h"

#define FI_OPX_FABRIC_TX_INJECT	fi_opx_hfi1_tx_inject
#define FI_OPX_FABRIC_TX_SEND_EGR	fi_opx_hfi1_tx_send_egr
#define FI_OPX_FABRIC_TX_SENDV_EGR	fi_opx_hfi1_tx_sendv_egr
#define FI_OPX_FABRIC_TX_SEND_RZV	fi_opx_hfi1_tx_send_rzv
#define FI_OPX_FABRIC_TX_SENDV_RZV	fi_opx_hfi1_tx_sendv_rzv
#define FI_OPX_FABRIC_RX_RZV_RTS	fi_opx_hfi1_rx_rzv_rts
#define FI_OPX_FABRIC_RX_RZV_CTS	fi_opx_hfi1_rx_rzv_cts
#define FI_OPX_FABRIC_TX_DO_PUT     fi_opx_hfi1_do_dput

#endif

#endif /* _FI_PROV_OPX_FABRIC_TRANSPORT_H_ */

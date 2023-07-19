/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#include "psm_user.h"
#include "psm2_hal.h"
#include "ips_proto.h"
#include "ips_proto_internal.h"
#include "ips_epstate.h"

/* The indexes are used to map a particular endpoint to a structure at the
 * receiver.  Although we take extra care to validate the identity of endpoints
 * when packets are received, the communication index is at an offset selected
 * by the endpoint that allocates the index.  This narrows the window of two
 * jobs communicated with the same set of indexes from getting crosstalk.
 */

psm2_error_t
psm3_ips_epstate_init(struct ips_epstate *eps, psm2_ep_t ep)
{
	memset(eps, 0, sizeof(*eps));
	eps->ep = ep;
	eps->eps_base_idx = ((ips_epstate_idx)get_cycles()) &
				(IPS_EPSTATE_CONNIDX_MAX-1);
	return PSM2_OK;
}

psm2_error_t psm3_ips_epstate_fini(struct ips_epstate *eps)
{
	if (eps->eps_tab)
		psmi_free(eps->eps_tab);
	memset(eps, 0, sizeof(*eps));
	return PSM2_OK;
}

/*
 * Use this to debug issues involving the epstate table.
 */
void ips_epstate_dump(struct ips_epstate *eps)
{
	if (_HFI_DBG_ON) {
		int i=0; 
		_HFI_DBG_ALWAYS("eps_base_idx = 0x%x, eps_tabsize = %d, "
			"eps_tabsizeused = %d, eps_tab_nextidx = %d\n",
			eps->eps_base_idx, eps->eps_tabsize,
			eps->eps_tabsizeused, eps->eps_tab_nextidx);
		for (i=0; i<eps->eps_tabsize; i++) {
			if (eps->eps_tab[i].ipsaddr)	// skip empty slots
				_HFI_DBG_ALWAYS("%03d: ipsaddr = %p, cstate-o: %u, cstate-i: %u\n", i,
					eps->eps_tab[i].ipsaddr,
					eps->eps_tab[i].ipsaddr->cstate_outgoing,
					eps->eps_tab[i].ipsaddr->cstate_incoming);
		}
	}
}

/*
 * Add ipsaddr with epid to the epstate table, return new index to caller in
 * 'connidx'.
 */
psm2_error_t
psm3_ips_epstate_add(struct ips_epstate *eps, struct ips_epaddr *ipsaddr,
		ips_epstate_idx *connidx_o)
{
	int i, j;
	ips_epstate_idx connidx;

	if (++eps->eps_tabsizeused > eps->eps_tabsize) {	/* realloc */
		struct ips_epstate_entry *newtab;
		eps->eps_tabsize += PTL_EPADDR_ALLOC_CHUNK;
		newtab = (struct ips_epstate_entry *)
		    psmi_calloc(eps->ep, PER_PEER_ENDPOINT,
				eps->eps_tabsize,
				sizeof(struct ips_epstate_entry));
		if (newtab == NULL)
			return PSM2_NO_MEMORY;
		else if (eps->eps_tab) {	/* NOT first alloc */
			for (i = 0;
			     i < eps->eps_tabsize - PTL_EPADDR_ALLOC_CHUNK; i++)
				newtab[i] = eps->eps_tab[i];	/* deep copy */
			psmi_free(eps->eps_tab);
		}
		eps->eps_tab = newtab;
	}
	/* Find the next free hole.  We can afford to do this since connect is not
	 * in the critical path */
	for (i = 0, j = eps->eps_tab_nextidx; i < eps->eps_tabsize; i++, j++) {
		if (j == eps->eps_tabsize)
			j = 0;
		if (eps->eps_tab[j].ipsaddr == NULL) {
			eps->eps_tab_nextidx = j + 1;
			if (eps->eps_tab_nextidx == eps->eps_tabsize)
				eps->eps_tab_nextidx = 0;
			break;
		}
	}
	psmi_assert_always(i != eps->eps_tabsize);
	connidx = (j - eps->eps_base_idx) & (IPS_EPSTATE_CONNIDX_MAX-1);
	_HFI_VDBG("node %s gets connidx=%d (table idx %d)\n",
		  psm3_epaddr_get_name(((psm2_epaddr_t) ipsaddr)->epid, 0), connidx,
		  j);
	eps->eps_tab[j].ipsaddr = ipsaddr;
	if (j >= IPS_EPSTATE_CONNIDX_MAX) {
		return psm3_handle_error(eps->ep,
					 PSM2_TOO_MANY_ENDPOINTS,
					 "Can't connect to more than %d non-local endpoints",
					 IPS_EPSTATE_CONNIDX_MAX);
	}
	*connidx_o = connidx;
	return PSM2_OK;
}

psm2_error_t psm3_ips_epstate_del(struct ips_epstate *eps, ips_epstate_idx connidx)
{
	ips_epstate_idx idx;
	/* actual table index */
	idx = (connidx + eps->eps_base_idx) & (IPS_EPSTATE_CONNIDX_MAX-1);
	psmi_assert_always(idx < eps->eps_tabsize);
	_HFI_VDBG("connidx=%d, table_idx=%d\n", connidx, idx);
	eps->eps_tab[idx].ipsaddr = NULL;
	/* We may eventually want to release memory, but probably not */
	eps->eps_tabsizeused--;
	return PSM2_OK;
}

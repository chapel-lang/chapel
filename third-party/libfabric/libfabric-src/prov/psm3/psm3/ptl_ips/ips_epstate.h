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

#ifndef _IPS_EPSTATE_H
#define _IPS_EPSTATE_H

#include "psm_user.h"

typedef uint32_t ips_epstate_idx;
#define IPS_EPSTATE_CONNIDX_MAX (1<<26)

struct ips_epaddr;

struct ips_epstate_entry {
	struct ips_epaddr *ipsaddr;
};

struct ips_epstate {
	psm2_ep_t ep;
	ips_epstate_idx eps_base_idx;
	int eps_tabsize;
	int eps_tabsizeused;
	int eps_tab_nextidx;

	struct ips_epstate_entry *eps_tab;
};

psm2_error_t psm3_ips_epstate_init(struct ips_epstate *eps, psm2_ep_t ep);
psm2_error_t psm3_ips_epstate_fini(struct ips_epstate *eps);

psm2_error_t psm3_ips_epstate_add(struct ips_epstate *eps,
			    struct ips_epaddr *ipsaddr,
			    ips_epstate_idx *connidx);
psm2_error_t psm3_ips_epstate_del(struct ips_epstate *eps, ips_epstate_idx connidx);

/* Use this to debug EP issues. */
void ips_epstate_dump(struct ips_epstate *eps);

PSMI_INLINE(
struct ips_epstate_entry *
ips_epstate_lookup(const struct ips_epstate *eps, ips_epstate_idx idx))
{
	idx = (idx + eps->eps_base_idx) & (IPS_EPSTATE_CONNIDX_MAX-1);
	if (idx < (ips_epstate_idx)eps->eps_tabsize)
		return &eps->eps_tab[idx];
	else
		return NULL;
}

#endif /* _IPS_EPSTATE_H */

/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2024 Intel Corporation.

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

  Copyright(c) 2024 Intel Corporation.

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

/* Copyright (c) 2003-2024 Intel Corporation. All rights reserved. */

#ifndef _PSMI_IN_USER_H
#error psm_nic_select.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSM_NIC_SELECT_H
#define _PSM_NIC_SELECT_H

struct multirail_config {
    int num_rails;
    uint32_t units[PSMI_MAX_RAILS];
    uint16_t ports[PSMI_MAX_RAILS];
    int addr_indexes[PSMI_MAX_RAILS];
};

// Autoselect one unit for non-multirail operation.
// caller will select 1st active port and an addr_index within unit
// returns the unit number or -1 if unable to find an active unit
int
psm3_autoselect_one(long addr_index, int nunits, psm2_uuid_t const job_key);

// determine if PSM3_MULTIRAIL is enabled, and if so select the rails
// and place the list in multirail_config.  If multirail is not enabled
// multirail_config.num_rails will be set to 0
psm2_error_t
psm3_ep_multirail(struct multirail_config *multirail_config);

// decrement any NIC refcounts which may have been
// incremented by psm3_compute_start_and_end_unit
void psm3_dec_nic_refcount(int unit_id);

// manage hwloc topology discovery.  These will be Noops when ! PSM_USE_HWLOC
void psm3_hwloc_topology_init();
void psm3_hwloc_topology_destroy();

#endif /* PSM_NIC_SELECT_H */

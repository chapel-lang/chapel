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

// PSM3_NIC_SELECTION_ALG choices
/*
 * round robin contexts across HFIs, then
 * ports; this is the default.
 * This option spreads the HFI selection within the local socket.
 * If it is preferred to spread job over over entire set of
 * HFIs within the system, see ALG_ACROSS_ALL below.
 */
#define PSMI_UNIT_SEL_ALG_ACROSS     PSM_HAL_ALG_ACROSS

#define PSMI_UNIT_SEL_ALG_ACROSS_ALL PSM_HAL_ALG_ACROSS_ALL

/*
 * use all contexts on an HFI (round robin
 * active ports within), then next HFI
 */
#define PSMI_UNIT_SEL_ALG_WITHIN     PSM_HAL_ALG_WITHIN

#define PSMI_UNIT_SEL_ALG_CPU_CENTRIC     PSM_HAL_ALG_CPU_CENTRIC
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
#define PSMI_UNIT_SEL_ALG_GPU_CENTRIC     PSM_HAL_ALG_GPU_CENTRIC
#endif

struct multirail_config {
    int num_rails;
    uint32_t units[PSMI_MAX_RAILS];
    uint16_t ports[PSMI_MAX_RAILS];
    int addr_indexes[PSMI_MAX_RAILS];
};

// return set of units to consider and which to start at.
// caller will use 1st active unit which can be opened.
// caller will wrap around so it's valid for start >= end
// Note: When using multiple rails per PSM process, higher level code will
// walk through desired units and unit_param will specify a specific unit
// if unit_param is PSM3_NIC_ANY, this will pick starting point for nic search
psm2_error_t
psm3_compute_start_and_end_unit(long unit_param, long addr_index,
				int nunitsactive,int nunits,
				psm2_uuid_t const job_key,
				long *unit_start,long *unit_end);

psm2_error_t
psm3_ep_multirail(struct multirail_config *multirail_config);

// decrement any NIC refcounts which may have been
// incremented by psm3_compute_start_and_end_unit
void psm3_dec_nic_refcount(int unit_id);

// manage hwloc topology discovery.  These will be Noops when ! PSM_USE_HWLOC
void psm3_hwloc_topology_init();
void psm3_hwloc_topology_destroy();

#endif /* PSM_NIC_SELECT_H */

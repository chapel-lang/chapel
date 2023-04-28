#ifdef PSM_SOCKETS
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

#ifndef PSM_HAL_SOCKETS_SERVICE_H
#define PSM_HAL_SOCKETS_SERVICE_H

/* This file contains all the lowest level routines calling into sysfs */
/* and qib driver. All other calls are based on these routines. */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#endif
#include <sched.h>              /* cpu_set_t and CPU_* MACROs */
#include <libgen.h>

#include "utils_user.h"
#include "psm_netutils.h"

/* HAL specific upper and lower bounds for NIC port numbers */
#define HFI_MIN_PORT 1
#define HFI_MAX_PORT 1
#ifndef HFI_NUM_PORTS_SOCKETS
#define HFI_NUM_PORTS_SOCKETS (HFI_MAX_PORT - HFI_MIN_PORT + 1)
#endif


/* Given a unit number and port number, returns 1 if the unit and port are active.
   returns 0 if the unit and port are not active. returns -1 when an error occurred. */
int psm3_hfp_sockets_get_port_active(int, int);

/* Given the unit number, port and addr_index, */
/* return an error, or the corresponding LID */
/* Returns an int, so -1 indicates a general error.  -2 indicates that the unit/port
   are not active.  0 indicates that the unit is valid, but no LID has been assigned. */
enum sockets_init_max_speed { SIMS_NOFILTER, SIMS_FILTER, SIMS_FINDMAX };
int psm3_sockets_get_port_lid(int, int, int, enum sockets_init_max_speed init_max_speed);

/* Given the unit number, port and addr_index, */
/* return an error, or the corresponding */
/* subnet, addr and gid.  For ethernet uses 1st IPv4 RoCE gid. */
/* For IB/OPA uses 1st valid gid */
/* Returns an int, so -1 indicates an error. */
int psm3_hfp_sockets_get_port_subnet(int unit, int port, int addr_index,
	psmi_subnet128_t *subnet, psmi_naddr128_t *addr,
	int *idx, psmi_gid128_t *gid);

/* Given a unit and port umber, return an error, or the corresponding speed in bps. */
/* Returns an int, so -1 indicates an error. 0 on success */
int psm3_hfp_sockets_get_port_speed(int unit, int port, uint64_t *speed);

/* Get the number of units supported by the driver.  Does not guarantee
   that a working chip has been found for each possible unit #.
   Returns -1 with errno set, or number of units >=0 (0 means none found). */
int psm3_hfp_sockets_get_num_units();

/* Given a unit number, returns 1 if any port on the unit is active.
   returns <=0 if no port on the unit is active. */
int psm3_sockets_get_unit_active(int unit, enum sockets_init_max_speed init_max_speed);

/* Given a number of units, returns 1 if any port on the units is active
   returns <= 0 if no port on any of the units is active. */
int psm3_hfp_sockets_have_active_unit(int num_units);

#endif /* PSM_HAL_SOCKETS_SERVICE_H */
#endif /* PSM_SOCKETS */

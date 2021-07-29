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

#ifndef OPA_SERVICE_GEN1_H
#define OPA_SERVICE_GEN1_H

/* This file contains all the lowest level routines calling into sysfs */
/* and qib driver. All other calls are based on these routines. */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#endif
#include <sched.h>              /* cpu_set_t and CPU_* MACROs */
#include <libgen.h>

#include "opa_intf.h"
#include "opa_common_gen1.h"
#include "opa_udebug.h"
#include "opa_byteorder.h"

/* upper and lower bounds for HFI port numbers */
#define HFI_MIN_PORT 1
#define HFI_MAX_PORT 1
#ifndef HFI_NUM_PORTS_GEN1
#define HFI_NUM_PORTS_GEN1 (HFI_MAX_PORT - HFI_MIN_PORT + 1)
#endif
/* any unit id to match. */
#define PSM3_NIC_ANY ((long)-1)
/* any port num to match. */
#define PSM3_NIC_PORT_ANY ((long)0)


/* Given a unit number and port number, returns 1 if the unit and port are active.
   returns 0 if the unit and port are not active. returns -1 when an error occurred. */
int hfi_get_port_active(int, int);

/* Given the unit number and port, return an error, or the corresponding LID */
/* Returns an int, so -1 indicates a general error.  -2 indicates that the unit/port
   are not active.  0 indicates that the unit is valid, but no LID has been assigned. */
int hfi_get_port_lid(int, int);

/* Given a unit number, return an error, or the corresponding cpuset. */
/* Returns an int, so -1 indicates an error. */
int hfi_get_unit_cpumask(int unit, cpu_set_t *cpuset);

/* Given the unit number and port, return an error, or the corresponding */
/* subnet, addr and gid.  For ethernet uses 1st IPv4 RoCE gid. */
/* For IB/OPA uses 1st valid gid */
/* Returns an int, so -1 indicates an error. */
int hfi_get_port_subnet(int unit, int port, uint64_t *subnet, uint64_t *addr,
		uint32_t *ip_addr, uint32_t *netmask,
		int *idx, uint64_t *hi, uint64_t *lo);


/* Given the unit number, return an error, or the corresponding link rate
   for the port */
/* Returns an int, so -1 indicates an error. */
int hfi_get_port_rate(int unit, int port);


/* Get the number of units supported by the driver.  Does not guarantee
   that a working chip has been found for each possible unit #.
   Returns -1 with errno set, or number of units >=0 (0 means none found). */
int hfi_get_num_units();

/* Given a unit number, returns 1 if any port on the unit is active.
   returns 0 if no port on the unit is active.
   returns -1 when an error occurred. */
int hfi_get_unit_active(int unit);

/* get the number of contexts from the unit id. */
int hfi_get_num_contexts(int unit);


/* We use mmap64() because we compile in both 32 and 64 bit mode,
   and we have to map physical addresses that are > 32 bits long.
   While linux implements mmap64, it doesn't have a man page,
   and isn't declared in any header file, so we declare it here ourselves. */

/* We'd like to just use -D_LARGEFILE64_SOURCE, to make off_t 64 bits and
   redirects mmap to mmap64 for us, but at least through suse10 and fc4,
   it doesn't work when the address being mapped is > 32 bits.  It chips
   off bits 32 and above.   So we stay with mmap64. */
extern void *mmap64(void *, size_t, int, int, int, __off64_t);
void *hfi_mmap64(void *, size_t, int, int, int, __off64_t);

/* Statistics maintained by the driver */
int hfi_get_stats(uint64_t *, int);
int hfi_get_stats_names(char **namep);
/* Counters maintained in the chip, globally, and per-prot */
int hfi_get_ctrs_unit(int unitno, uint64_t *, int);
int hfi_get_ctrs_unit_names(int unitno, char **namep);
int hfi_get_ctrs_port(int unitno, int port, uint64_t *, int);
int hfi_get_ctrs_port_names(int unitno, char **namep);

/* sysfs helper routines (only those currently used are exported;
 * try to avoid using others) */

const char *sysfs_unit_path(int unit_id);

/* read a string value */
int hfi_sysfs_port_read(uint32_t unit, uint32_t port, const char *attr,
			char **datap);

/* read a string value into buff, no more than size bytes.
   returns the number of bytes read */
size_t hfi_sysfs_unit_port_read(uint32_t unit, uint32_t port, const char *attr,
			char *buff, size_t size);

/* open attribute in unit's sysfs directory via open(2) */
int hfi_sysfs_unit_open(uint32_t unit, const char *attr, int flags);
int hfi_sysfs_port_open(uint32_t unit, uint32_t port, const char *attr,
			int flags);

int hfi_sysfs_unit_read(uint32_t unit, const char *attr, char **datap);

/* print to attribute in {unit,port} sysfs directory */
int hfi_sysfs_port_printf(uint32_t unit, uint32_t port, const char *attr,
			  const char *fmt, ...)
			  __attribute__((format(printf, 4, 5)));
int hfi_sysfs_unit_printf(uint32_t unit, const char *attr, const char *fmt, ...)
			  __attribute__((format(printf, 3, 4)));

/* read a signed 64-bit quantity, in some arbitrary base */
int hfi_sysfs_unit_read_s64(uint32_t unit, const char *attr,
			    int64_t *valp, int base);
int hfi_sysfs_port_read_s64(uint32_t unit, uint32_t port, const char *attr,
			    int64_t *valp, int base);
int64_t hfi_sysfs_unit_read_node_s64(uint32_t unit);

#endif /* OPA_SERVICE_GEN1_H */

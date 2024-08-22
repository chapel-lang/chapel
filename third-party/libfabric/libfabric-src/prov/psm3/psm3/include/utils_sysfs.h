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

#ifndef UTILS_SYSFS_H
#define UTILS_SYSFS_H

/* This file contains all the lowest level routines calling into sysfs */
/* and qib driver. All other calls are based on these routines. */

#include <libgen.h>

/* we can only include low level headers here because this is
 * #included by utils_sysfs.c.  Can't pull in HAL headers or heap debug macros
 */
#include "utils_macros.h"
#include "utils_debug.h"
#include "utils_byteorder.h"
#include <sched.h>              /* cpu_set_t and CPU_* MACROs */

/* upper and lower bounds for HFI port numbers */
#define HFI_MIN_PORT 1
#define HFI_MAX_PORT 1

/* any unit id to match. */
#define PSM3_NIC_ANY ((long)-1)
/* any port num to match. */
#define PSM3_NIC_PORT_ANY ((long)0)
/* any addr_index to match. */
#define PSM3_ADDR_INDEX_ANY ((long)-1)
#define PSM3_ADDR_INDEX_ALL ((long)-2)	// only for use inside parser


/* format args: char *unitpath, uint32_t port, char *attr */
typedef enum {
	PSM3_PORT_PATH_TYPE_NO_PORT = 0, /* "%s/%s" */
	PSM3_PORT_PATH_TYPE_IB = 1,      /* "%s/ports/%u/%s" */
} psm3_port_path_type;

/* sysfs helper routines (only those currently used are exported;
 * try to avoid using others) */

/* Initializes the following sysfs helper routines.
   psm3_sysfs_init() returns 0 on success, non-zero on an error: */
int psm3_sysfs_init(const char *nic_class_path, const psm3_port_path_type port_path_fmt);
void psm3_sysfs_fini(void);

const char *psm3_sysfs_unit_path(int unit_id);
const char *psm3_sysfs_unit_dev_name(int unit_id);
int psm3_sysfs_find_unit(const char *name);

void psm3_sysfs_free(char *data);

/* read a string value */
/* caller must use psm3_sysfs_free to free *datap */
int psm3_sysfs_unit_read(uint32_t unit, const char *attr, char **datap);
int psm3_sysfs_port_read(uint32_t unit, uint32_t port, const char *attr,
			char **datap);

#if 0
/* read a string value into buff, no more than size bytes.
   returns the number of bytes read */
size_t psm3_sysfs_unit_port_read(uint32_t unit, uint32_t port, const char *attr,
			char *buff, size_t size);
#endif

/* read a signed 64-bit quantity, in some arbitrary base */
int psm3_sysfs_unit_read_s64(uint32_t unit, const char *attr,
			    int64_t *valp, int base);
int psm3_sysfs_port_read_s64(uint32_t unit, uint32_t port, const char *attr,
			    int64_t *valp, int base);
int64_t psm3_sysfs_unit_read_node_s64(uint32_t unit);


/* Given a unit number, return an error, or the corresponding cpuset. */
/* Returns an int, so -1 indicates an error. */
int psm3_sysfs_get_unit_cpumask(int unit, cpu_set_t *cpuset);

/* Given a unit number, return an error, or the corresponding pci bus values. */
/* Returns an int, so -1 indicates an error, 0 on success */
int psm3_sysfs_get_unit_pci_bus(int unit, uint32_t *domain, uint32_t *bus,
					uint32_t *device, uint32_t *function);

/* Given a unit number, return an error, or the corresponding devce string. */
/* Returns an int, so -1 indicates an error. 0 on success */
int psm3_sysfs_get_unit_device_id(int unit, char *buf, size_t bufsize);
int psm3_sysfs_get_unit_device_version(int unit, char *buf, size_t bufsize);
int psm3_sysfs_get_unit_vendor_id(int unit, char *buf, size_t bufsize);
int psm3_sysfs_get_unit_driver(int unit, char *buf, size_t bufsize);

#endif /* UTILS_SYSFS_H */

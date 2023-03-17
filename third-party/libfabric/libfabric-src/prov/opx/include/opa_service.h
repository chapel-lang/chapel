/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.
  Copyright(c) 2021 Cornelis Networks.

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
  Copyright(c) 2021 Cornelis Networks.

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

#ifndef OPA_SERVICE_H
#define OPA_SERVICE_H

/* This file contains all the lowest level routines calling into sysfs */
/* and qib driver. All other calls are based on these routines. */

#include <libgen.h>

#include "opa_intf.h"
#include "opa_common_gen1.h"
#include "opa_udebug.h"
#include "opa_byteorder.h"

/* upper and lower bounds for HFI port numbers */
#define OPX_MIN_PORT 1
#define OPX_MAX_PORT 1
#ifndef OPX_NUM_PORTS_GEN1
#define OPX_NUM_PORTS_GEN1 (OPX_MAX_PORT - OPX_MIN_PORT + 1)
#endif
/* any unit id to match. */
#define OPX_UNIT_ID_ANY ((long)-1)
/* any port num to match. */
#define OPX_PORT_NUM_ANY ((long)0)

/* base name of path (without unit #) for qib driver */
#ifndef OPX_DEVICE_PATH_GEN1
#define OPX_DEVICE_PATH_GEN1 "/dev/hfi1"
#endif

#ifdef PSM_CUDA
#define GDR_DEVICE_PATH "/dev/hfi1_gdr"
#endif

/* The major and minor versions of driver that support non-DW multiple SDMA */
#define HFI1_USER_SWMAJOR_NON_DW_MUL_MSG_SIZE_ALLOWED 6
#define HFI1_USER_SWMINOR_NON_DW_MUL_MSG_SIZE_ALLOWED 2

/* base name of path (without unit #) for opa driver */
#define OPX_DEVICE_PATH "/dev/hfi1"
#define OPX_CLASS_PATH "/sys/class/infiniband/hfi1"

/* Commands used to communicate with driver. */
enum OPX_HFI_CMD {
    OPX_HFI_CMD_ASSIGN_CTXT = 0,   /* allocate HFI and context */
    OPX_HFI_CMD_CTXT_INFO,         /* find out what resources we got */
    OPX_HFI_CMD_USER_INFO,         /* set up userspace */
    OPX_HFI_CMD_TID_UPDATE,        /* update expected TID entries */
    OPX_HFI_CMD_TID_FREE,          /* free expected TID entries */
    OPX_HFI_CMD_CREDIT_UPD,        /* force an update of PIO credit */
    OPX_HFI_CMD_RECV_CTRL,         /* control receipt of packets */
    OPX_HFI_CMD_POLL_TYPE,         /* set the kind of polling we want */
    OPX_HFI_CMD_ACK_EVENT,         /* ack & clear user status bits */
    OPX_HFI_CMD_SET_PKEY,          /* set context's pkey */
    OPX_HFI_CMD_CTXT_RESET,        /* reset context's HW send context */
    OPX_HFI_CMD_TID_INVAL_READ,    /* read TID cache invalidations */
    OPX_HFI_CMD_GET_VERS,          /* get the version of the user cdev */

#ifdef PSM_CUDA
    OPX_HFI_CMD_TID_UPDATE_V2 = 28,
#endif
    OPX_HFI_CMD_LAST,
};

/* Legacy commands used to communicate with driver using 'write' */
enum LEGACY_HFI1_CMD {
    LEGACY_HFI1_CMD_ASSIGN_CTXT     = 1,     /* allocate HFI and context */
    LEGACY_HFI1_CMD_CTXT_INFO       = 2,     /* find out what resources we got */
    LEGACY_HFI1_CMD_USER_INFO       = 3,     /* set up userspace */
    LEGACY_HFI1_CMD_TID_UPDATE      = 4,     /* update expected TID entries */
    LEGACY_HFI1_CMD_TID_FREE        = 5,     /* free expected TID entries */
    LEGACY_HFI1_CMD_CREDIT_UPD      = 6,     /* force an update of PIO credit */

    LEGACY_HFI1_CMD_RECV_CTRL       = 8,     /* control receipt of packets */
    LEGACY_HFI1_CMD_POLL_TYPE       = 9,     /* set the kind of polling we want */
    LEGACY_HFI1_CMD_ACK_EVENT       = 10,    /* ack & clear user status bits */
    LEGACY_HFI1_CMD_SET_PKEY        = 11,    /* set context's pkey */
    LEGACY_HFI1_CMD_CTXT_RESET      = 12,    /* reset context's HW send context */
    LEGACY_HFI1_CMD_TID_INVAL_READ  = 13,    /* read TID cache invalidations */
    LEGACY_HFI1_CMD_GET_VERS        = 14    /* get the version of the user cdev */
};

/* Given a unit number and port number, returns 1 if the unit and port are active.
   returns 0 if the unit and port are not active. returns -1 when an error occurred. */
int opx_hfi_get_port_active(int, int);

/* Given the unit number and port, return an error, or the corresponding LID */
/* Returns an int, so -1 indicates a general error.  -2 indicates that the unit/port
   are not active.  0 indicates that the unit is valid, but no LID has been assigned. */
int opx_hfi_get_port_lid(int, int);

/* Given the unit number and port, return an error, or the corresponding GID */
/* Returns an int, so -1 indicates an error. */
int opx_hfi_get_port_gid(int, int, uint64_t *hi, uint64_t *lo);

/* Given the unit number, return an error, or the corresponding LMC value
   for the port */
/* Returns an int, so -1 indicates an error.  0 */
int opx_hfi_get_port_lmc(int unit, int port);

/* Given the unit number, return an error, or the corresponding link rate
   for the port */
/* Returns an int, so -1 indicates an error. */
int opx_hfi_get_port_rate(int unit, int port);

/* Given a unit, port and SL, return an error, or the corresponding SC for the
   SL as programmed by the SM */
/* Returns an int, so -1 indicates an error. */
int opx_hfi_get_port_sl2sc(int unit, int port, int sl);

/* Given a unit, port and SC, return an error, or the corresponding VL for the
   SC as programmed by the SM */
/* Returns an int, so -1 indicates an error. */
int opx_hfi_get_port_sc2vl(int unit, int port, int sc);

/* Given a unit, port and VL, return an error, or the corresponding MTU for the
   VL as programmed by the SM */
/* Returns an int, so -1 indicates an error. */
int opx_hfi_get_port_vl2mtu(int unit, int port, int vl);

/* Given a unit, port and index, return an error, or the corresponding pkey for
   the index as programmed by the SM */
/* Returns an int, so -1 indicates an error. */
int opx_hfi_get_port_index2pkey(int unit, int port, int index);

/* Get the number of units supported by the driver.  Does not guarantee
   that a working chip has been found for each possible unit #.
   Returns -1 with errno set, or number of units >=0 (0 means none found). */
int opx_hfi_get_num_units();

/* Given a unit number, returns 1 if any port on the unit is active.
   returns 0 if no port on the unit is active.
   returns -1 when an error occurred. */
int opx_hfi_get_unit_active(int unit);

/* get the number of contexts from the unit id. */
int opx_hfi_get_num_contexts(int unit);

/* Open hfi device file, return -1 on error. */
int opx_hfi_context_open(int unit, int port, uint64_t open_timeout);
int opx_hfi_context_open_ex(int unit, int port, uint64_t open_timeout,
		     char *dev_name,size_t dev_name_len);

uint32_t opx_hfi_check_non_dw_mul_sdma(void);

void opx_hfi_context_close(int fd);

/* opx_hfi_get_user_major_version() returns the major version of the driver
   that should be used for this session of psm. Valid only after
   opx_hfi_context_open has been called. */
uint16_t opx_hfi_get_user_major_version(void);

/* opx_hfi_get_user_minor_version() return the minor version of the driver */
uint16_t opx_hfi_get_user_minor_version(void);

void opx_hfi_set_user_version(uint32_t version);
void opx_hfi_set_user_major_version(uint16_t major_version);

int opx_hfi_cmd_write(int fd, struct hfi1_cmd *, size_t count);

int opx_hfi_cmd_writev(int fd, const struct iovec *iov, int iovcnt);

int opx_hfi_get_cc_table_bin(int unit, int port, uint16_t **cctp);

/* We use mmap64() because we compile in both 32 and 64 bit mode,
   and we have to map physical addresses that are > 32 bits long.
   While linux implements mmap64, it doesn't have a man page,
   and isn't declared in any header file, so we declare it here ourselves. */

/* We'd like to just use -D_LARGEFILE64_SOURCE, to make off_t 64 bits and
   redirects mmap to mmap64 for us, but at least through suse10 and fc4,
   it doesn't work when the address being mapped is > 32 bits.  It chips
   off bits 32 and above.   So we stay with mmap64. */
extern void *mmap64(void *, size_t, int, int, int, __off64_t);
void *opx_hfi_mmap64(void *, size_t, int, int, int, __off64_t);

/* Statistics maintained by the driver */
int opx_hfi_get_stats(uint64_t *, int);
int opx_hfi_get_stats_names(char **namep);
/* Counters maintained in the chip, globally, and per-prot */
int opx_hfi_get_ctrs_unit(int unitno, uint64_t *, int);
int opx_hfi_get_ctrs_unit_names(int unitno, char **namep);
int opx_hfi_get_ctrs_port(int unitno, int port, uint64_t *, int);
int opx_hfi_get_ctrs_port_names(int unitno, char **namep);

/* sysfs helper routines (only those currently used are exported;
 * try to avoid using others) */

const char *hfi_sysfs_path(void);

/* read a string value */
int opx_hfi_sysfs_port_read(uint32_t unit, uint32_t port, const char *attr,
			char **datap);

/* read a string value into buff, no more than size bytes.
   returns the number of bytes read */
size_t opx_hfi_sysfs_unit_port_read(uint32_t unit, uint32_t port, const char *attr,
			char *buff, size_t size);

/* open attribute in unit's sysfs directory via open(2) */
int opx_hfi_sysfs_unit_open(uint32_t unit, const char *attr, int flags);
int opx_hfi_sysfs_port_open(uint32_t unit, uint32_t port, const char *attr,
			int flags);
/* print to attribute in {unit,port} sysfs directory */
int opx_hfi_sysfs_port_printf(uint32_t unit, uint32_t port, const char *attr,
			  const char *fmt, ...)
			  __attribute__((format(printf, 4, 5)));
int opx_hfi_sysfs_unit_printf(uint32_t unit, const char *attr, const char *fmt, ...)
			  __attribute__((format(printf, 3, 4)));

/* read a signed 64-bit quantity, in some arbitrary base */
int opx_hfi_sysfs_unit_read_s64(uint32_t unit, const char *attr,
			    int64_t *valp, int base);
int opx_hfi_sysfs_port_read_s64(uint32_t unit, uint32_t port, const char *attr,
			    int64_t *valp, int base);
int64_t opx_hfi_sysfs_unit_read_node_s64(uint32_t unit);

int opx_hfi_cmd_wait_for_packet(int fd);

/* returns number of hfi1_* devs in /dev */
int opx_hfi_get_hfi1_count();

#endif /* OPA_SERVICE_H */

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

#ifndef OPA_USER_H
#define OPA_USER_H

/* This file contains all of the data structures and routines that are
   publicly visible and usable (to low level infrastructure code; it is
   not expected that any application, or even normal application-level library,
   will ever need to use any of this).

   Additional entry points and data structures that are used by these routines
   may be referenced in this file, but they should not be generally available;
   they are visible here only to allow use in inlined functions.  Any variable,
   data structure, or function that starts with a leading "_" is in this
   category.
*/

/* Include header files we need that are unlikely to otherwise be needed by */
/* programs. */
#include <stddef.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <syslog.h>
#include "opa_intf.h"
#include "opa_byteorder.h"
#include "opa_udebug.h"
#include "opa_service.h"

#ifndef PACK_SUFFIX
/* XXX gcc only */
#define PACK_SUFFIX __attribute__((packed))
#endif

#define HFI_TF_NFLOWS                       32

// The sender uses an RDMA Write with Immediate.  The immediate data
// carries the receiver's desc genc and idx from which the receiver can
// locate the ips_tid_recv_desc
// we have 16 bits of genc and 5 bits of desc_idx (max of HFI_TF_NFLOWS).
// leaving up to 11 bits for dest_rv_idx for RNDV_MOD (we use 9)
// so desc_idx could grow to 7 bits if needed
#define RV_INDEX_BITS 9
#define RDMA_PACK_IMMED(desc_genc, desc_idx, dest_rv_idx) \
		((((uint32_t)(desc_genc))&0xffff) \
		| ((((uint32_t)(desc_idx))&0x7f) << 16) \
		| ((dest_rv_idx) << (32-RV_INDEX_BITS)))
#define RDMA_UNPACK_IMMED_GENC(immed) ((immed) & 0xffff)
#define RDMA_UNPACK_IMMED_IDX(immed) (((immed) >> 16) & 0x7f)
#define RDMA_UNPACK_IMMED_RV_IDX(immed) ((immed) >> (32-RV_INDEX_BITS))
#define RDMA_IMMED_DESC_MASK 0x7fffff // mask for desc genc and desc idx

// source of the immediate callback
#define RDMA_IMMED_USER_RC 0	// from a user space RC QP
#define RDMA_IMMED_RV 1			// from RV module kernel QP

/* IB - LRH header consts */
#define HFI_LRH_BTH 0x0002	/* 1. word of IB LRH - next header: BTH */
#define HFI_LRH_SC_SHIFT 12
#define HFI_LRH_SC_MASK 0xf
#define HFI_LRH_SL_SHIFT 4
#define HFI_LRH_SL_MASK 0xf
#define HFI_LRH_PKTLEN_MASK 0xfff

/* IB - BTH header consts */
// bth[0]
#define HFI_BTH_OPCODE_SHIFT 24
#define HFI_BTH_OPCODE_MASK 0xff
// bth[1]
#define HFI_BTH_FLOWID_SHIFT 11
#define HFI_BTH_FLOWID_MASK 0x1f
// bth[2]
#define HFI_BTH_SEQ_SHIFT 0
#define HFI_BTH_SEQ_MASK 0x7ff
#define HFI_BTH_GEN_SHIFT 11
#define HFI_BTH_GEN_MASK 0xfffff
#define HFI_BTH_ACK_SHIFT 31

/* KDETH header consts */
#define HFI_KHDR_OFFSET_MASK 0x7fff
#define HFI_KHDR_OM_SHIFT 15
#define HFI_KHDR_TID_SHIFT 16
#define HFI_KHDR_TID_MASK 0x3ff
#define HFI_KHDR_TIDCTRL_SHIFT 26
#define HFI_KHDR_TIDCTRL_MASK 0x3
#define HFI_KHDR_INTR_SHIFT 28
#define HFI_KHDR_SH_SHIFT 29
#define HFI_KHDR_KVER_SHIFT 30
#define HFI_KHDR_KVER_MASK 0x3

#define HFI_KHDR_MSGSEQ_MASK 0xffff
#define HFI_KHDR_TINYLEN_MASK 0xf
#define HFI_KHDR_TINYLEN_SHIFT 16

#define GET_HFI_KHDR_TIDCTRL(val) \
	(((val) >> HFI_KHDR_TIDCTRL_SHIFT) & \
	HFI_KHDR_TIDCTRL_MASK)

#ifdef PSM_CUDA
extern int is_driver_gpudirect_enabled;

#define PSMI_IS_DRIVER_GPUDIRECT_ENABLED  likely(is_driver_gpudirect_enabled)
#define PSMI_IS_DRIVER_GPUDIRECT_DISABLED unlikely(!is_driver_gpudirect_enabled)
#endif

/* hfi kdeth header format */
struct hfi_kdeth {
	__u32 kdeth0;

	union {
		struct {
			__u16 job_key;	// unused for UD/UDP
			__u16 hcrc;	// unused for UD/UDP
		};
		__u32 kdeth1;
	};
} PACK_SUFFIX;

/* misc. */
#define HFI_CRC_SIZE_IN_BYTES 4

//#define HFI_DEFAULT_SERVICE_ID 0 /* let rv module decide */
#define HFI_DEFAULT_SERVICE_ID 0x1000125500000001ULL
#define HFI_DEFAULT_P_KEY 0      /* use slot 0 as default */

#if 0
#define HFI_PERMISSIVE_LID 0xFFFF
#define HFI_AETH_CREDIT_SHIFT 24
#define HFI_AETH_CREDIT_MASK 0x1F
#define HFI_AETH_CREDIT_INVAL 0x1F
#define HFI_PSN_MASK 0xFFFFFF
#define HFI_MSN_MASK 0xFFFFFF
#define HFI_QPN_MASK 0xFFFFFF
#define HFI_MULTICAST_LID_BASE 0xC000
#define HFI_MULTICAST_QPN 0xFFFFFF
#endif

/* Receive Header Queue: receive type (from hfi) */
#define RCVHQ_RCV_TYPE_EXPECTED  0
#define RCVHQ_RCV_TYPE_EAGER     1
#define RCVHQ_RCV_TYPE_NON_KD    2
#define RCVHQ_RCV_TYPE_ERROR     3

/* OPA PSM assumes that the message header is always 56 bytes. */
#define HFI_MESSAGE_HDR_SIZE	56

/* interval timing routines */
/* Convert a count of cycles to elapsed nanoseconds */
/* this is only accurate for reasonably large numbers of cycles (at least tens)
*/
static __inline__ uint64_t cycles_to_nanosecs(uint64_t)
					  __attribute__ ((always_inline));
/* convert elapsed nanoseconds to elapsed cycles */
/* this is only accurate for reasonably large numbers of nsecs (at least tens)
*/
static __inline__ uint64_t nanosecs_to_cycles(uint64_t)
					  __attribute__ ((always_inline));

/* Statistics maintained by the driver */
const char *hfi_get_next_name(char **names);
int hfi_get_stats_names_count(void);
/* Counters maintained in the chip, globally, and per-prot */
int hfi_get_ctrs_unit_names_count(int unitno);
int hfi_get_ctrs_port_names_count(int unitno);
/* Convert Timeout value from usec to
 * timeout_mult where usec = 4.096usec * 2^timeout_mult
 */
uint8_t timeout_usec_to_mult(uint64_t timeout_us);

uint64_t hfi_get_single_unitctr(int unit, const char *attr, uint64_t *s);
int hfi_get_single_portctr(int unit, int port, const char *attr, uint64_t *c);
void hfi_release_names(char *namep);

/* Syslog wrapper

   level is one of LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_ERR, LOG_WARNING,
   LOG_NOTICE, LOG_INFO, LOG_DEBUG.

   prefix should be a short string to describe which part of the software stack
   is using syslog, i.e. "PSM", "mpi", "mpirun".
*/
void hfi_syslog(const char *prefix, int to_console, int level,
		const char *format, ...)
		__attribute__((format(printf, 4, 5)));

void hfi_vsyslog(const char *prefix, int to_console, int level,
		 const char *format, va_list ap);

/*
 * Copy routine that may copy a byte multiple times but optimized for througput
 * This is not safe to use for PIO routines where we want a guarantee that a
 * byte is only copied/moved across the bus once.
 */
void hfi_dwordcpy(volatile uint32_t *dest, const uint32_t *src,
		  uint32_t ndwords);
void hfi_qwordcpy(volatile uint64_t *dest, const uint64_t *src,
		  uint32_t nqwords);

extern uint32_t __hfi_pico_per_cycle;	/* only for use in these functions */

/* this is only accurate for reasonably large numbers of cycles (at least tens) */
static __inline__ uint64_t cycles_to_nanosecs(uint64_t cycs)
{
	return (__hfi_pico_per_cycle * cycs) / 1000ULL;
}

/* this is only accurate for reasonably large numbers of nsecs (at least tens) */
static __inline__ uint64_t nanosecs_to_cycles(uint64_t ns)
{
	return (ns * 1000ULL) / __hfi_pico_per_cycle;
}

#endif /* OPA_USER_H */

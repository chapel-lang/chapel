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

#ifndef OPA_USER_GEN1_H
#define OPA_USER_GEN1_H

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
#include <stdbool.h>
#include "opa_intf.h"
#include "opa_common_gen1.h"
#include "opa_byteorder.h"
#include "opa_udebug.h"
#include "opa_service_gen1.h"
#include "opa_user.h"

#define HFI_RHF_USE_EGRBFR_MASK 0x1
#define HFI_RHF_USE_EGRBFR_SHIFT 15
#define HFI_RHF_EGRBFR_INDEX_MASK 0x7FF
#define HFI_RHF_EGRBFR_INDEX_SHIFT 16

#define HFI_RHF_SEQ_MASK 0xF
#define HFI_RHF_SEQ_SHIFT 28
#define HFI_RHF_EGRBFR_OFFSET_MASK 0xFFF
#define HFI_RHF_EGRBFR_OFFSET_SHIFT 0
#define HFI_RHF_HDRQ_OFFSET_MASK 0x1FF
#define HFI_RHF_HDRQ_OFFSET_SHIFT 12
#define HFI_RHF_TIDERR     0x08000000

/* TidFlow related bits */
#define HFI_TF_SEQNUM_SHIFT                 0
#define HFI_TF_SEQNUM_MASK                  0x7ff

#define HFI_TF_GENVAL_SHIFT                 11
#define HFI_TF_GENVAL_MASK                  0xfffff

#define HFI_TF_FLOWVALID_SHIFT              32
#define HFI_TF_FLOWVALID_MASK               0x1

#define HFI_TF_KEEP_AFTER_SEQERR_SHIFT      34
#define HFI_TF_KEEP_AFTER_SEQERR_MASK       0x1
#define HFI_TF_KEEP_ON_GENERR_SHIFT         35
#define HFI_TF_KEEP_ON_GENERR_MASK          0x1
#define HFI_TF_KEEP_PAYLOAD_ON_GENERR_SHIFT 36
#define HFI_TF_KEEP_PAYLOAD_ON_GENERR_MASK  0x1
#define HFI_TF_STATUS_SEQMISMATCH_SHIFT     37
#define HFI_TF_STATUS_SEQMISMATCH_MASK      0x1
#define HFI_TF_STATUS_GENMISMATCH_SHIFT     38
#define HFI_TF_STATUS_GENMISMATCH_MASK      0x1

/* PBC bits */
#define HFI_PBC_STATICRCC_SHIFT         0
#define HFI_PBC_STATICRCC_MASK          0xffff

#define HFI_PBC_SC4_SHIFT               4
#define HFI_PBC_SC4_MASK                0x1

#define HFI_PBC_INTR_SHIFT              31
#define HFI_PBC_DCINFO_SHIFT            30
#define HFI_PBC_TESTEBP_SHIFT           29
#define HFI_PBC_PACKETBYPASS_SHIFT      28
#define HFI_PBC_INSERTHCRC_SHIFT        26
#define HFI_PBC_INSERTHCRC_MASK         0x3
#define HFI_PBC_CREDITRETURN_SHIFT      25
#define HFI_PBC_INSERTBYPASSICRC_SHIFT  24
#define HFI_PBC_TESTBADICRC_SHIFT       23
#define HFI_PBC_FECN_SHIFT              22
#define HFI_PBC_VL_SHIFT                12
#define HFI_PBC_VL_MASK                 0xf
#define HFI_PBC_LENGTHDWS_SHIFT         0
#define HFI_PBC_LENGTHDWS_MASK          0xfff

/* this portion only defines what we currently use */
struct hfi_pbc {
	__u32 pbc0;
	__u16 PbcStaticRateControlCnt;
	__u16 fill1;
};

typedef enum mapsize
{	SC_CREDITS,
	PIO_BUFBASE_SOP,
	PIO_BUFBASE,
	RCVHDR_BUFBASE,
	RCVEGR_BUFBASE,
	SDMA_COMP_BUFBASE,
	USER_REGBASE,
	RCVHDRTAIL_BASE,
	EVENTS_BUFBASE,
	STATUS_BUFBASE,
	SUBCTXT_UREGBASE,
	SUBCTXT_RCVHDRBUF,
	SUBCTXT_RCVEGRBUF,
	MAPSIZE_MAX
} mapsize_t;

/* TODO: consider casting in the ALIGN() macro */
#define ALIGN(x, a)				(((x)+(a)-1)&~((a)-1))
#define ALIGNDOWN_PTR(x, a)			((void*)(((uintptr_t)(x))&~((uintptr_t)((a)-1))))

/* using the same flags for all the mappings */
#define HFI_MMAP_FLAGS				(MAP_SHARED|MAP_LOCKED)
#define HFI_MMAP_PGSIZE				sysconf(_SC_PAGESIZE)
/* cast to uintptr_t as opposed to intptr_t which evaluates to a signed type
 *  * on which one should not perform bitwise operations (undefined behavior)
 *   */
#define HFI_MMAP_PGMASK				(~(uintptr_t)(HFI_MMAP_PGSIZE-1))

/* this is only an auxiliary macro for HFI_MMAP_ERRCHECK()
 * @off expected to be unsigned in order to AND with the page mask and avoid undefined behavior
 */
#define U64_TO_OFF64_PGMASK(off)		((__off64_t)((off) & HFI_MMAP_PGMASK))

#define HFI_MMAP_ALIGNOFF(fd, off, size, prot)	hfi_mmap64(0,(size),(prot),HFI_MMAP_FLAGS,(fd),U64_TO_OFF64_PGMASK((off)))
/* complementary */
#define HFI_MUNMAP(addr, size)			munmap((addr), (size))

/* make sure uintmax_t can hold the result of unsigned int multiplication */
#if UINT_MAX > (UINTMAX_MAX / UINT_MAX)
#error We cannot safely multiply unsigned integers on this platform
#endif

/* @member assumed to be of type u64 and validated to be so */
#define HFI_MMAP_ERRCHECK(fd, binfo, member, size, prot) ({						\
		typeof((binfo)->member) *__tptr = (__u64 *)NULL;					\
		(void)__tptr;										\
		void *__maddr = HFI_MMAP_ALIGNOFF((fd), (binfo)->member, (size), (prot));		\
		do {											\
			if (unlikely(__maddr == MAP_FAILED)) {						\
				uintmax_t outval = (uintmax_t)((binfo)->member);			\
				_HFI_INFO("mmap of " #member " (0x%jx) size %zu failed: %s\n",		\
					outval, size, strerror(errno));					\
				goto err_mmap_##member;							\
			}										\
			(binfo)->member = (__u64)__maddr;						\
			_HFI_VDBG(#member "mmap %jx successful\n", (uintmax_t)((binfo)->member));	\
		} while(0);										\
		__maddr;										\
})

/* assigns 0 to the member after unmapping */
#define HFI_MUNMAP_ERRCHECK(binfo, member, size)						\
		do {	typeof((binfo)->member) *__tptr = (__u64 *)NULL;			\
			(void)__tptr;								\
			void *__addr = ALIGNDOWN_PTR((binfo)->member, HFI_MMAP_PGSIZE);		\
			if (unlikely( __addr == NULL || (munmap(__addr, (size)) == -1))) {	\
				_HFI_INFO("unmap of " #member " (%p) failed: %s\n",		\
					__addr, strerror(errno));				\
			}									\
			else {									\
				_HFI_VDBG("unmap of " #member "(%p) succeeded\n", __addr);	\
				(binfo)->member = 0;						\
			}									\
		} while(0)

#define HFI_PCB_SIZE_IN_BYTES 8

/* Usable bytes in header (hdrsize - lrh - bth) */
#define HFI_MESSAGE_HDR_SIZE_HFI       (HFI_MESSAGE_HDR_SIZE-20)

/*
 * SDMA includes 8B sdma hdr, 8B PBC, and message header.
 * If we are using GPU workloads, we need to set a new
 * "flags" member which takes another 2 bytes in the
 * sdma hdr. We let the driver know of this 2 extra bytes
 * at runtime when we set the length for the iovecs.
 */
#define HFI_SDMA_HDR_SIZE      (8+8+56)

static inline __u32 hfi_hdrget_seq(const __le32 *rbuf)
{
	return (__le32_to_cpu(rbuf[0]) >> HFI_RHF_SEQ_SHIFT)
	    & HFI_RHF_SEQ_MASK;
}

static inline __u32 hfi_hdrget_hdrq_offset(const __le32 *rbuf)
{
	return (__le32_to_cpu(rbuf[1]) >> HFI_RHF_HDRQ_OFFSET_SHIFT)
	    & HFI_RHF_HDRQ_OFFSET_MASK;
}



/* don't inline these; it's all init code, and not inlining makes the */
/* overall code shorter and easier to debug */
void hfi_touch_mmap(void *, size_t) __attribute__ ((noinline));


/*
* Safe version of hfi_[d/q]wordcpy that is guaranteed to only copy each byte once.
*/
#if defined(__x86_64__) && defined(HAVE_PSM3_DWORD_FAST)
void hfi_dwordcpy_safe(volatile uint32_t *dest, const uint32_t *src,
		       uint32_t ndwords);
void hfi_qwordcpy_safe(volatile uint64_t *dest, const uint64_t *src,
		       uint32_t nqwords);
#else
#define hfi_dwordcpy_safe hfi_dwordcpy
#define hfi_qwordcpy_safe hfi_qwordcpy
#endif






















#endif /* OPA_USER_GEN1_H */

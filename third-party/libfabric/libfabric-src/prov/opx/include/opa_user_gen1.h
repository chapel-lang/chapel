/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.
  Copyright(c) 2021-2022 Cornelis Networks.

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
  Copyright(c) 2021-2022 Cornelis Networks.

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
#include "opa_service.h"
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
#define HFI_TF_HDRSUPP_ENABLED_SHIFT        33
#define HFI_TF_HDRSUPP_ENABLED_MASK         0x1

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
struct opx_hfi_pbc {
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

#define HFI_MMAP_ALIGNOFF(fd, off, size, prot)	opx_hfi_mmap64(0,(size),(prot),HFI_MMAP_FLAGS,(fd),U64_TO_OFF64_PGMASK((off)))
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
			if (OFI_UNLIKELY(__maddr == MAP_FAILED)) {						\
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
			if (OFI_UNLIKELY( __addr == NULL || (munmap(__addr, (size)) == -1))) {	\
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

struct _hfi_ctrl {
	int32_t fd;		/* device file descriptor */
	uint32_t __hfi_pg_sz;
	/* tidflow valid */
	uint32_t __hfi_tfvalid;
	/* unit id */
	uint32_t __hfi_unit;
	/* port id */
	uint32_t __hfi_port;

	/* number of eager tid entries */
	uint32_t __hfi_tidegrcnt;
	/* number of expected tid entries */
	uint32_t __hfi_tidexpcnt;

	/* effective mtu size, should be <= base_info.mtu */
	uint32_t __hfi_mtusize;
	/* max PIO size, should be <= effective mtu size */
	uint32_t __hfi_piosize;

	/* two struct output from driver. */
	struct hfi1_ctxt_info ctxt_info;
	struct hfi1_base_info base_info;

	/* some local storages in some condition: */
	/* as storage of __hfi_rcvtidflow in opx_hfi_userinit_internal(). */
	__le64 regs[HFI_TF_NFLOWS];

	/* location to which OPA writes the rcvhdrtail register whenever
	   it changes, so that no chip registers are read in the performance
	   path. */
	volatile __le64 *__hfi_rcvtail;

	/* address where ur_rcvhdrtail is written */
	volatile __le64 *__hfi_rcvhdrtail;
	/* address where ur_rcvhdrhead is written */
	volatile __le64 *__hfi_rcvhdrhead;
	/* address where ur_rcvegrindextail is read */
	volatile __le64 *__hfi_rcvegrtail;
	/* address where ur_rcvegrindexhead is written */
	volatile __le64 *__hfi_rcvegrhead;
	/* address where ur_rcvegroffsettail is read */
	volatile __le64 *__hfi_rcvofftail;
	/* address where ur_rcvtidflow is written */
	volatile __le64 *__hfi_rcvtidflow;
};

/* After the device is opened, opx_hfi_userinit() is called to give the driver the
   parameters the user code wants to use, and to get the implementation values,
   etc. back.  0 is returned on success, a positive value is a standard errno,
   and a negative value is reserved for future use.  The first argument is
   the filedescriptor returned by the device open.

   It is allowed to have multiple devices (and of different types)
   simultaneously opened and initialized, although this won't be fully
   implemented initially.  This routine is used by the low level
   hfi protocol code (and any other code that has similar low level
   functionality).
   This is the only routine that takes a file descriptor, rather than an
   struct _hfi_ctrl *.  The struct _hfi_ctrl * used for everything
   else is returned by this routine.
*/
struct _hfi_ctrl *opx_hfi_userinit(int32_t, struct hfi1_user_info_dep *);

/* don't inline these; it's all init code, and not inlining makes the */
/* overall code shorter and easier to debug */
void opx_hfi_touch_mmap(void *, size_t) __attribute__ ((noinline));

/* set the BTH pkey to check for this process. */
/* This is for receive checks, not for sends.  It isn't necessary
   to set the default key, that's always allowed by the hardware.
   If too many pkeys are in use for the hardware to support, this
   will return EAGAIN, and the caller should then fail and exit
   or use the default key and check the pkey in the received packet
   checking. */
/* set send context pkey to verify, error if driver is not configured with */
/* this pkey in its pkey table. */
int opx_hfi_set_pkey(struct _hfi_ctrl *, uint16_t);

int opx_hfi_wait_for_packet(struct _hfi_ctrl *);

/* New user event mechanism, using spi_sendbuf_status HFI_EVENT_* bits
   obsoletes opx_hfi_disarm_bufs(), and extends it, although old mechanism
   remains for binary compatibility. */
int opx_hfi_event_ack(struct _hfi_ctrl *ctrl, __u64 ackbits);

/* set whether we want an interrupt on all packets, or just urgent ones */
int opx_hfi_poll_type(struct _hfi_ctrl *ctrl, uint16_t poll_type);

/* reset halted send context, error if context is not halted. */
int opx_hfi_reset_context(struct _hfi_ctrl *ctrl);

/*
* Safe version of opx_hfi_[d/q]wordcpy that is guaranteed to only copy each byte once.
*/
#if defined(__x86_64__)
void opx_hfi_dwordcpy_safe(volatile uint32_t *dest, const uint32_t *src,
		       uint32_t ndwords);
void opx_hfi_qwordcpy_safe(volatile uint64_t *dest, const uint64_t *src,
		       uint32_t nqwords);
#else
#define opx_hfi_dwordcpy_safe opx_hfi_dwordcpy
#define opx_hfi_qwordcpy_safe opx_hfi_qwordcpy
#endif

static __inline__ void opx_hfi_tidflow_set_entry(struct _hfi_ctrl *ctrl,
					 uint32_t flowid, uint32_t genval,
					 uint32_t seqnum)
{
/* For proper behavior with RSM interception of FECN packets for CCA,
 * the tidflow entry needs the KeepAfterSequenceError bit set.
 * A packet that is converted from expected to eager by RSM will not
 * trigger an update in the tidflow state.  This will cause the tidflow
 * to incorrectly report a sequence error on any non-FECN packets that
 * arrive after the RSM intercepted packets.  If the KeepAfterSequenceError
 * bit is set, PSM can properly detect this "false SeqErr" condition,
 * and recover without dropping packets.
 * Note that if CCA/RSM are not important, this change will slightly
 * increase the CPU load when packets are dropped.  If this is significant,
 * consider hiding this change behind a CCA/RSM environment variable.
 */

	ctrl->__hfi_rcvtidflow[flowid] = __cpu_to_le64(
		((genval & HFI_TF_GENVAL_MASK) << HFI_TF_GENVAL_SHIFT) |
		((seqnum & HFI_TF_SEQNUM_MASK) << HFI_TF_SEQNUM_SHIFT) |
		((uint64_t)ctrl->__hfi_tfvalid << HFI_TF_FLOWVALID_SHIFT) |
		(1ULL << HFI_TF_HDRSUPP_ENABLED_SHIFT) |
		/* KeepAfterSequenceError = 1 -- previously was 0 */
		(1ULL << HFI_TF_KEEP_AFTER_SEQERR_SHIFT) |
		(1ULL << HFI_TF_KEEP_ON_GENERR_SHIFT) |
		/* KeePayloadOnGenErr = 0 */
		(1ULL << HFI_TF_STATUS_SEQMISMATCH_SHIFT) |
		(1ULL << HFI_TF_STATUS_GENMISMATCH_SHIFT));
}

static __inline__ void opx_hfi_tidflow_reset(struct _hfi_ctrl *ctrl,
					 uint32_t flowid, uint32_t genval,
					 uint32_t seqnum)
{
/*
 * If a tidflow table entry is set to "Invalid", we want to drop
 * header if payload is dropped, we want to get a header if the payload
 * is delivered.
 *
 * We set a tidflow table entry "Invalid" by setting FlowValid=1 and
 * GenVal=0x1FFF/0xFFFFF, this is a special generation number and no
 * packet will use this value. We don't care SeqNum but we set it to
 * 0x7FF. So if GenVal does not match, the payload is dropped because
 * KeepPayloadOnGenErr=0; for packet header, KeepOnGenErr=0 make sure
 * header is not generated. But if a packet happens to have the special
 * generation number, the payload is delivered, HdrSuppEnabled=0 make
 * sure header is generated if SeqNUm matches, if SeqNum does not match,
 * KeepAfterSeqErr=1 makes sure the header is generated.
 */
	ctrl->__hfi_rcvtidflow[flowid] = __cpu_to_le64(
		/* genval = 0x1FFF or 0xFFFFF */
		((genval & HFI_TF_GENVAL_MASK) << HFI_TF_GENVAL_SHIFT) |
		/* seqnum = 0x7FF */
		((seqnum & HFI_TF_SEQNUM_MASK) << HFI_TF_SEQNUM_SHIFT) |
		((uint64_t)ctrl->__hfi_tfvalid << HFI_TF_FLOWVALID_SHIFT) |
		/* HdrSuppEnabled = 0 */
		(1ULL << HFI_TF_KEEP_AFTER_SEQERR_SHIFT) |
		/* KeepOnGenErr = 0 */
		/* KeepPayloadOnGenErr = 0 */
		(1ULL << HFI_TF_STATUS_SEQMISMATCH_SHIFT) |
		(1ULL << HFI_TF_STATUS_GENMISMATCH_SHIFT));
}

/*
 * This should only be used for debugging.
 * Normally, we shouldn't read the chip.
 */
static __inline__ uint64_t opx_hfi_tidflow_get(struct _hfi_ctrl *ctrl,
					   uint32_t flowid)
{
	return __le64_to_cpu(ctrl->__hfi_rcvtidflow[flowid]);
}

static __inline__ uint32_t opx_hfi_tidflow_get_seqnum(uint64_t val)
{
	return (val >> HFI_TF_SEQNUM_SHIFT) & HFI_TF_SEQNUM_MASK;
}

static __inline__ uint32_t opx_hfi_tidflow_get_genval(uint64_t val)
{
	return (val >> HFI_TF_GENVAL_SHIFT) & HFI_TF_GENVAL_MASK;
}

static __inline__ uint32_t opx_hfi_tidflow_get_flowvalid(uint64_t val)
{
	return (val >> HFI_TF_FLOWVALID_SHIFT) & HFI_TF_FLOWVALID_MASK;
}

static __inline__ uint32_t opx_hfi_tidflow_get_enabled(uint64_t val)
{
	return (val >> HFI_TF_HDRSUPP_ENABLED_SHIFT) &
	    HFI_TF_HDRSUPP_ENABLED_MASK;
}

static __inline__ uint32_t opx_hfi_tidflow_get_keep_after_seqerr(uint64_t val)
{
	return (val >> HFI_TF_KEEP_AFTER_SEQERR_SHIFT) &
	    HFI_TF_KEEP_AFTER_SEQERR_MASK;
}

static __inline__ uint32_t opx_hfi_tidflow_get_keep_on_generr(uint64_t val)
{
	return (val >> HFI_TF_KEEP_ON_GENERR_SHIFT) &
	    HFI_TF_KEEP_ON_GENERR_MASK;
}

static __inline__ uint32_t opx_hfi_tidflow_get_keep_payload_on_generr(uint64_t val)
{
	return (val >> HFI_TF_KEEP_PAYLOAD_ON_GENERR_SHIFT) &
	    HFI_TF_KEEP_PAYLOAD_ON_GENERR_MASK;
}

static __inline__ uint32_t opx_hfi_tidflow_get_seqmismatch(uint64_t val)
{
	return (val >> HFI_TF_STATUS_SEQMISMATCH_SHIFT) &
	    HFI_TF_STATUS_SEQMISMATCH_MASK;
}

static __inline__ uint32_t opx_hfi_tidflow_get_genmismatch(uint64_t val)
{
	return (val >> HFI_TF_STATUS_GENMISMATCH_SHIFT) &
	    HFI_TF_STATUS_GENMISMATCH_MASK;
}

/*
 * This should only be used by a process to write the eager index into
 * a subcontext's eager header entry.
 */
static __inline__ void opx_hfi_hdrset_use_egrbfr(__le32 *rbuf, uint32_t val)
{
	rbuf[0] =
	    (rbuf[0] &
	     __cpu_to_le32(~(HFI_RHF_USE_EGRBFR_MASK <<
			     HFI_RHF_USE_EGRBFR_SHIFT))) |
	    __cpu_to_le32((val & HFI_RHF_USE_EGRBFR_MASK) <<
			  HFI_RHF_USE_EGRBFR_SHIFT);
}

static __inline__ void opx_hfi_hdrset_egrbfr_index(__le32 *rbuf, uint32_t val)
{
	rbuf[0] =
	    (rbuf[0] &
	     __cpu_to_le32(~(HFI_RHF_EGRBFR_INDEX_MASK <<
			     HFI_RHF_EGRBFR_INDEX_SHIFT))) |
	    __cpu_to_le32((val & HFI_RHF_EGRBFR_INDEX_MASK) <<
			  HFI_RHF_EGRBFR_INDEX_SHIFT);
}

static __inline__ void opx_hfi_hdrset_egrbfr_offset(__le32 *rbuf, uint32_t val)
{
	rbuf[1] =
	    (rbuf[1] &
	     __cpu_to_le32(~(HFI_RHF_EGRBFR_OFFSET_MASK <<
			     HFI_RHF_EGRBFR_OFFSET_SHIFT))) |
	    __cpu_to_le32((val & HFI_RHF_EGRBFR_OFFSET_MASK) <<
			  HFI_RHF_EGRBFR_OFFSET_SHIFT);
}

/*
 * This should only be used by a process to update the receive header
 * error flags.
 */
static __inline__ void opx_hfi_hdrset_err_flags(__le32 *rbuf, uint32_t val)
{
	rbuf[1] |= __cpu_to_le32(val);
}

/*
 * This should only be used by a process to write the rhf seq number into
 * a subcontext's eager header entry.
 */
static __inline__ void opx_hfi_hdrset_seq(__le32 *rbuf, uint32_t val)
{
	rbuf[0] =
	    (rbuf[0] &
	     __cpu_to_le32(~(HFI_RHF_SEQ_MASK <<
			     HFI_RHF_SEQ_SHIFT))) |
	    __cpu_to_le32((val & HFI_RHF_SEQ_MASK) << HFI_RHF_SEQ_SHIFT);
}

/* Manage TID entries.  It is possible that not all entries
   requested may be allocated.  A matching opx_hfi_free_tid() must be
   done for each opx_hfi_update_tid(), because currently no caching or
   reuse of expected tid entries is allowed, to work around malloc/free
   and mmap/munmap issues.  The driver decides which TID entries to allocate.
   If opx_hfi_free_tid is called to free entries in use by a different
   send by the same process, data corruption will probably occur,
   but only within that process, not for other processes.
*/

/* update tidcnt expected TID entries from the array pointed to by tidinfo. */
/* Returns 0 on success, else an errno.  See full description at declaration */
static __inline__ int32_t opx_hfi_update_tid(struct _hfi_ctrl *ctrl,
					 uint64_t vaddr, uint32_t *length,
					 uint64_t tidlist, uint32_t *tidcnt, uint16_t flags)
{
	struct hfi1_cmd cmd;
	struct hfi1_tid_info tidinfo;
#ifdef PSM_CUDA
	struct hfi1_tid_info_v2 tidinfov2;
#endif
	int err;

	tidinfo.vaddr = vaddr;		/* base address for this send to map */
	tidinfo.length = *length;	/* length of vaddr */

	tidinfo.tidlist = tidlist;	/* driver copies tids back directly */
	tidinfo.tidcnt = 0;		/* clear to zero */

	cmd.type = OPX_HFI_CMD_TID_UPDATE; /* HFI1_IOCTL_TID_UPDATE */
	cmd.len = sizeof(tidinfo);
	cmd.addr = (__u64) &tidinfo;
#ifdef PSM_CUDA
	if (PSMI_IS_DRIVER_GPUDIRECT_ENABLED) {
		/* Copy values to v2 struct */
		tidinfov2.vaddr   = tidinfo.vaddr;
		tidinfov2.length  = tidinfo.length;
		tidinfov2.tidlist = tidinfo.tidlist;
		tidinfov2.tidcnt  = tidinfo.tidcnt;
		tidinfov2.flags   = flags;

		cmd.type = OPX_HFI_CMD_TID_UPDATE_V2;
		cmd.len = sizeof(tidinfov2);
		cmd.addr = (__u64) &tidinfov2;
	}
#endif

	err = opx_hfi_cmd_write(ctrl->fd, &cmd, sizeof(cmd));

	if (err != -1) {
		struct hfi1_tid_info *rettidinfo =
			(struct hfi1_tid_info *)cmd.addr;
		*length = rettidinfo->length;
		*tidcnt = rettidinfo->tidcnt;
	} else {
#if 0 
		perror("HFI1_IOCTL_TID_UPDATE: errno ");
#endif
		FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,"ERR %d \"%s\"\n", err, strerror(errno));
		*length = 0;
		*tidcnt = 0;
	}

	return err;
}

static __inline__ int32_t opx_hfi_free_tid(struct _hfi_ctrl *ctrl,
					 uint64_t tidlist, uint32_t tidcnt)
{
	struct hfi1_cmd cmd;
	struct hfi1_tid_info tidinfo;
	int err;

	tidinfo.tidlist = tidlist;	/* input to driver */
	tidinfo.tidcnt = tidcnt;

	cmd.type = OPX_HFI_CMD_TID_FREE; /* HFI1_IOCTL_TID_FREE */
	cmd.len = sizeof(tidinfo);
	cmd.addr = (__u64) &tidinfo;

	err = opx_hfi_cmd_write(ctrl->fd, &cmd, sizeof(cmd));
	if (err == -1) {
#if 0 
		perror("HFI1_IOCTL_TID_FREE: errno ");
#endif
		FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,"ERR %d \"%s\"\n", err, strerror(errno));
	}

	return err;
}

static __inline__ int32_t opx_hfi_get_invalidation(struct _hfi_ctrl *ctrl,
					 uint64_t tidlist, uint32_t *tidcnt)
{
	struct hfi1_cmd cmd;
	struct hfi1_tid_info tidinfo;
	int err;

	tidinfo.tidlist = tidlist;	/* driver copies tids back directly */
	tidinfo.tidcnt = 0;		/* clear to zero */

	cmd.type = OPX_HFI_CMD_TID_INVAL_READ;
	cmd.len = sizeof(tidinfo);
	cmd.addr = (__u64) &tidinfo;

	err = opx_hfi_cmd_write(ctrl->fd, &cmd, sizeof(cmd));

	if (err != -1)
		*tidcnt = tidinfo.tidcnt;

	return err;
}

/*
 * Data layout in I2C flash (for GUID, etc.)
 * All fields are little-endian binary unless otherwise stated
 */
#define HFI_FLASH_VERSION 2
struct opx_hfi_flash {
	/* flash layout version (HFI_FLASH_VERSION) */
	__u8 if_fversion;
	/* checksum protecting if_length bytes */
	__u8 if_csum;
	/*
	 * valid length (in use, protected by if_csum), including
	 * if_fversion and if_csum themselves)
	 */
	__u8 if_length;
	/* the GUID, in network order */
	__u8 if_guid[8];
	/* number of GUIDs to use, starting from if_guid */
	__u8 if_numguid;
	/* the (last 10 characters of) board serial number, in ASCII */
	char if_serial[12];
	/* board mfg date (YYYYMMDD ASCII) */
	char if_mfgdate[8];
	/* last board rework/test date (YYYYMMDD ASCII) */
	char if_testdate[8];
	/* logging of error counts, TBD */
	__u8 if_errcntp[4];
	/* powered on hours, updated at driver unload */
	__u8 if_powerhour[2];
	/* ASCII free-form comment field */
	char if_comment[32];
	/* Backwards compatible prefix for longer QLogic Serial Numbers */
	char if_sprefix[4];
	/* 82 bytes used, min flash size is 128 bytes */
	__u8 if_future[46];
};
#endif /* OPA_USER_GEN1_H */

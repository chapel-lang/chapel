/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.
  Copyright(C) 2021-2023 Cornelis Networks.

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

/* This file contains the initialization functions used by the low
   level hfi protocol code. */

#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <malloc.h>

#include "opa_user_gen1.h"
#include "opa_udebug.h"

#include <sched.h>

size_t arrsz[MAPSIZE_MAX] = { 0 };

static int map_hfi_mem(int fd, struct _hfi_ctrl *ctrl, size_t subctxt_cnt)
{
#define CREDITS_NUM     64
	struct hfi1_ctxt_info *cinfo = &ctrl->ctxt_info;
	struct hfi1_base_info *binfo = &ctrl->base_info;
	ssize_t sz;
	__u64 off;
	void *maddr;


	/* 1. Map the PIO credits address */
	off = binfo->sc_credits_addr &~ HFI_MMAP_PGMASK;

	sz = HFI_MMAP_PGSIZE;
	if(sz < 0){
		return -1;
	}
	maddr = HFI_MMAP_ERRCHECK(fd, binfo, sc_credits_addr, sz, PROT_READ);
	opx_hfi_touch_mmap(maddr, sz);
	arrsz[SC_CREDITS] = sz;

	binfo->sc_credits_addr |= off;


	/* 2. Map the PIO buffer SOP address
	 * Skipping the cast of cinfo->credits to size_t. This causes the outcome of the multiplication
	 * to be sign-extended in the event of too large input values. This results in a very large product
	 * when treated as unsigned which in turn will make the HFI_MMAP_ERRCHECK() macro fail and give an
	 * adequate error report. TODO: Consider sanitizing the credits value explicitly
	 */
	sz = cinfo->credits * CREDITS_NUM;
	HFI_MMAP_ERRCHECK(fd, binfo, pio_bufbase_sop, sz, PROT_WRITE);
	arrsz[PIO_BUFBASE_SOP] = sz;


	/* 3. Map the PIO buffer address */
	sz = cinfo->credits * CREDITS_NUM;
	HFI_MMAP_ERRCHECK(fd, binfo, pio_bufbase, sz, PROT_WRITE);
	arrsz[PIO_BUFBASE] = sz;


	/* 4. Map the receive header queue
	 * (u16 * u16 -> max value 0xfffe0001)
	 */
	sz = (size_t)cinfo->rcvhdrq_cnt * cinfo->rcvhdrq_entsize;
	maddr = HFI_MMAP_ERRCHECK(fd, binfo, rcvhdr_bufbase, sz, PROT_READ);
	opx_hfi_touch_mmap(maddr, sz);
	arrsz[RCVHDR_BUFBASE] = sz;


	/* 5. Map the receive eager buffer
	 * (u16 * u32. Assuming size_t's precision is 64 bits - no overflow)
	 */
	sz = (size_t)cinfo->egrtids * cinfo->rcvegr_size;
	maddr = HFI_MMAP_ERRCHECK(fd, binfo, rcvegr_bufbase, sz, PROT_READ);
	opx_hfi_touch_mmap(maddr, sz);
	arrsz[RCVEGR_BUFBASE] = sz;


	/* 6. Map the sdma completion queue */
	if (cinfo->runtime_flags & HFI1_CAP_SDMA) {
		sz = cinfo->sdma_ring_size * sizeof(struct hfi1_sdma_comp_entry);
		HFI_MMAP_ERRCHECK(fd, binfo, sdma_comp_bufbase, sz, PROT_READ);
	} else {
		sz = 0;
		binfo->sdma_comp_bufbase = (__u64)0;
	}
	arrsz[SDMA_COMP_BUFBASE] = sz;


	/* 7. Map RXE per-context CSRs */
	sz = HFI_MMAP_PGSIZE;
	HFI_MMAP_ERRCHECK(fd, binfo, user_regbase, sz, PROT_WRITE|PROT_READ);
	arrsz[USER_REGBASE] = sz;
	/* Set up addresses for optimized register writeback routines.
 	 * This is for the real onchip registers, shared context or not
 	 */
	uint64_t *regbasep = (uint64_t *)binfo->user_regbase;
	ctrl->__hfi_rcvhdrtail = (volatile __le64 *)(regbasep + ur_rcvhdrtail);
	ctrl->__hfi_rcvhdrhead = (volatile __le64 *)(regbasep + ur_rcvhdrhead);
	ctrl->__hfi_rcvegrtail = (volatile __le64 *)(regbasep + ur_rcvegrindextail);
	ctrl->__hfi_rcvegrhead = (volatile __le64 *)(regbasep + ur_rcvegrindexhead);
	ctrl->__hfi_rcvofftail = (volatile __le64 *)(regbasep + ur_rcvegroffsettail);

	if (cinfo->runtime_flags & HFI1_CAP_HDRSUPP) {
		ctrl->__hfi_rcvtidflow = (volatile __le64 *)(regbasep + ur_rcvtidflowtable);
		ctrl->__hfi_tfvalid = 1;
	} else {
		ctrl->__hfi_rcvtidflow = ctrl->regs;
		ctrl->__hfi_tfvalid = 0;
	}


	/* 8. Map the rcvhdrq tail register address */
	if (cinfo->runtime_flags & HFI1_CAP_DMA_RTAIL) {
		sz = HFI_MMAP_PGSIZE;
		HFI_MMAP_ERRCHECK(fd, binfo, rcvhdrtail_base, sz, PROT_READ);
	} else {
		/* We don't use receive header queue tail register to detect new packets,
 		 * but here we save the address for false-eager-full recovery
 		 */
		sz = 0;
		/* This points inside the previously established mapping (user_rehbase). Don't munmap()! */
		binfo->rcvhdrtail_base = (uint64_t) (uintptr_t) ctrl->__hfi_rcvhdrtail;
	}
	ctrl->__hfi_rcvtail = (__le64 *)binfo->rcvhdrtail_base;
	arrsz[RCVHDRTAIL_BASE] = sz;


	/* 9. Map the event page */
	off = binfo->events_bufbase &~ HFI_MMAP_PGMASK;

	sz = HFI_MMAP_PGSIZE;
	HFI_MMAP_ERRCHECK(fd, binfo, events_bufbase, sz, PROT_READ);
	arrsz[EVENTS_BUFBASE] = sz;
	/* keep the offset in the address */
	binfo->events_bufbase |= off;


	/* 10. Map the status page */
	sz = HFI_MMAP_PGSIZE;
	HFI_MMAP_ERRCHECK(fd, binfo, status_bufbase, sz, PROT_READ);
	arrsz[STATUS_BUFBASE] = sz;


	if (!subctxt_cnt){
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize SC_CREDITS         %zu \n",arrsz[SC_CREDITS       ]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize PIO_BUFBASE_SOP    %zu \n",arrsz[PIO_BUFBASE_SOP  ]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize PIO_BUFBASE        %zu \n",arrsz[PIO_BUFBASE      ]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize RCVHDR_BUFBASE     %zu \n",arrsz[RCVHDR_BUFBASE   ]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize RCVEGR_BUFBASE     %zu \n",arrsz[RCVEGR_BUFBASE   ]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize SDMA_COMP_BUFBASE  %zu \n",arrsz[SDMA_COMP_BUFBASE]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize USER_REGBASE       %zu \n",arrsz[USER_REGBASE     ]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize RCVHDRTAIL_BASE    %zu \n",arrsz[RCVHDRTAIL_BASE  ]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize EVENTS_BUFBASE     %zu \n",arrsz[EVENTS_BUFBASE   ]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize STATUS_BUFBASE     %zu \n",arrsz[STATUS_BUFBASE   ]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize SUBCTXT_UREGBASE   %zu \n",arrsz[SUBCTXT_UREGBASE ]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize SUBCTXT_RCVHDRBUF  %zu \n",arrsz[SUBCTXT_RCVHDRBUF]);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt mapsize SUBCTXT_RCVEGRBUF  %zu \n",arrsz[SUBCTXT_RCVEGRBUF]);

		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->fd     %u \n",ctrl->fd);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_tfvalid     %u \n",ctrl->__hfi_tfvalid);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_unit     %u \n",ctrl->__hfi_unit);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_port     %u \n",ctrl->__hfi_port);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_tidegrcnt     %u \n",ctrl->__hfi_tidegrcnt);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_tidexpcnt     %u \n",ctrl->__hfi_tidexpcnt);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_mtusize     %u \n",ctrl->__hfi_mtusize);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_piosize     %u \n",ctrl->__hfi_piosize);
		{
			int i;
			for (i=0; i < HFI_TF_NFLOWS; ++i) {
				_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->regs[%u]     %llu \n",i,ctrl->regs[i]);
			}
		}
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_rcvtail     %p \n",ctrl->__hfi_rcvtail);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_rcvhdrtail     %p \n",ctrl->__hfi_rcvhdrtail);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_rcvhdrhead     %p \n",ctrl->__hfi_rcvhdrhead);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_rcvegrtail     %p \n",ctrl->__hfi_rcvegrtail);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_rcvegrhead     %p \n",ctrl->__hfi_rcvegrhead);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_rcvofftail     %p \n",ctrl->__hfi_rcvofftail);
		_HFI_PDBG("CONTEXT INIT !subctxt_cnt ctrl->__hfi_rcvtidflow     %p \n",ctrl->__hfi_rcvtidflow);
		{
			int i;
			/* TID flows aren't cleared between jobs, do it now. */
			for (i=0; i < 32; ++i) {
				_HFI_PDBG("CONTEXT INIT ctrl->__hfi_rcvtidflow[%u]               %#16.16llX \n",i,ctrl->__hfi_rcvtidflow[i]);
				ctrl->__hfi_rcvtidflow[i] = 0UL;
				_HFI_PDBG("CONTEXT INIT ctrl->__hfi_rcvtidflow[%u]               %#16.16llX \n",i,ctrl->__hfi_rcvtidflow[i]);
			}
		}


		return 0;
	}

	/* 11. If subcontext is used, map the buffers */
	const char *errstr = "Incorrect input values for the subcontext";
	size_t factor;

	/* 11a) subctxt_uregbase */
	sz = HFI_MMAP_PGSIZE;
	maddr = HFI_MMAP_ERRCHECK(fd, binfo, subctxt_uregbase, sz, PROT_READ|PROT_WRITE);
	opx_hfi_touch_mmap(maddr, sz);
	arrsz[SUBCTXT_UREGBASE] = sz;


	/* 11b) subctxt_rcvhdrbuf
	 * u16 * u16. Prevent promotion to int through an explicit cast to size_t
	 */
	factor = (size_t)cinfo->rcvhdrq_cnt * cinfo->rcvhdrq_entsize;
	factor = ALIGN(factor, HFI_MMAP_PGSIZE);
	sz = factor * subctxt_cnt;
	maddr = HFI_MMAP_ERRCHECK(fd, binfo, subctxt_rcvhdrbuf, sz, PROT_READ|PROT_WRITE);
	opx_hfi_touch_mmap(maddr, sz);
	arrsz[SUBCTXT_RCVHDRBUF] = sz;


	/* 11c) subctxt_rcvegrbuf
	 * u16 * u32. Assuming size_t's precision to be 64 bits (no overflow)
	 */
	factor = (size_t)cinfo->egrtids * cinfo->rcvegr_size;
	factor = ALIGN(factor, HFI_MMAP_PGSIZE);
	sz = factor * subctxt_cnt;
	if (sz / subctxt_cnt != factor) {
		_HFI_INFO("%s (rcvegrbuf)\n", errstr);
		goto err_int_overflow_subctxt_rcvegrbuf;
	}
	maddr = HFI_MMAP_ERRCHECK(fd, binfo, subctxt_rcvegrbuf, sz, PROT_READ|PROT_WRITE);
	opx_hfi_touch_mmap(maddr, sz);
	arrsz[SUBCTXT_RCVEGRBUF] = sz;

	_HFI_PDBG("CONTEXT INIT mapsize SC_CREDITS         %zu \n",arrsz[SC_CREDITS       ]);
	_HFI_PDBG("CONTEXT INIT mapsize PIO_BUFBASE_SOP    %zu \n",arrsz[PIO_BUFBASE_SOP  ]);
	_HFI_PDBG("CONTEXT INIT mapsize PIO_BUFBASE        %zu \n",arrsz[PIO_BUFBASE      ]);
	_HFI_PDBG("CONTEXT INIT mapsize RCVHDR_BUFBASE     %zu \n",arrsz[RCVHDR_BUFBASE   ]);
	_HFI_PDBG("CONTEXT INIT mapsize RCVEGR_BUFBASE     %zu \n",arrsz[RCVEGR_BUFBASE   ]);
	_HFI_PDBG("CONTEXT INIT mapsize SDMA_COMP_BUFBASE  %zu \n",arrsz[SDMA_COMP_BUFBASE]);
	_HFI_PDBG("CONTEXT INIT mapsize USER_REGBASE       %zu \n",arrsz[USER_REGBASE     ]);
	_HFI_PDBG("CONTEXT INIT mapsize RCVHDRTAIL_BASE    %zu \n",arrsz[RCVHDRTAIL_BASE  ]);
	_HFI_PDBG("CONTEXT INIT mapsize EVENTS_BUFBASE     %zu \n",arrsz[EVENTS_BUFBASE   ]);
	_HFI_PDBG("CONTEXT INIT mapsize STATUS_BUFBASE     %zu \n",arrsz[STATUS_BUFBASE   ]);
	_HFI_PDBG("CONTEXT INIT mapsize SUBCTXT_UREGBASE   %zu \n",arrsz[SUBCTXT_UREGBASE ]);
	_HFI_PDBG("CONTEXT INIT mapsize SUBCTXT_RCVHDRBUF  %zu \n",arrsz[SUBCTXT_RCVHDRBUF]);
	_HFI_PDBG("CONTEXT INIT mapsize SUBCTXT_RCVEGRBUF  %zu \n",arrsz[SUBCTXT_RCVEGRBUF]);

	_HFI_PDBG("CONTEXT INIT ctrl->fd     %u \n",ctrl->fd);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_tfvalid     %u \n",ctrl->__hfi_tfvalid);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_unit     %u \n",ctrl->__hfi_unit);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_port     %u \n",ctrl->__hfi_port);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_tidegrcnt     %u \n",ctrl->__hfi_tidegrcnt);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_tidexpcnt     %u \n",ctrl->__hfi_tidexpcnt);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_mtusize     %u \n",ctrl->__hfi_mtusize);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_piosize     %u \n",ctrl->__hfi_piosize);
	{
		int i;
		for (i=0; i < HFI_TF_NFLOWS; ++i) {
			_HFI_PDBG("CONTEXT INIT ctrl->regs[%u]     %llu \n",i,ctrl->regs[i]);
		}
	}
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_rcvtail     %p \n",ctrl->__hfi_rcvtail);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_rcvhdrtail     %p \n",ctrl->__hfi_rcvhdrtail);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_rcvhdrhead     %p \n",ctrl->__hfi_rcvhdrhead);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_rcvegrtail     %p \n",ctrl->__hfi_rcvegrtail);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_rcvegrhead     %p \n",ctrl->__hfi_rcvegrhead);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_rcvofftail     %p \n",ctrl->__hfi_rcvofftail);
	_HFI_PDBG("CONTEXT INIT ctrl->__hfi_rcvtidflow     %p \n",ctrl->__hfi_rcvtidflow);
	{
		int i;
		/* TID flows aren't cleared between jobs, do it now. */
		for (i=0; i < 32; ++i) {
			_HFI_PDBG("CONTEXT INIT ctrl->__hfi_rcvtidflow[%u]               %#16.16llX \n",i,ctrl->__hfi_rcvtidflow[i]);
			ctrl->__hfi_rcvtidflow[i] = 0UL;
			_HFI_PDBG("CONTEXT INIT ctrl->__hfi_rcvtidflow[%u]               %#16.16llX \n",i,ctrl->__hfi_rcvtidflow[i]);
		}
	}


	return 0;

err_int_overflow_subctxt_rcvegrbuf:
err_mmap_subctxt_rcvegrbuf:
	/* if we got here, subctxt_cnt must be != 0 */
	HFI_MUNMAP_ERRCHECK(binfo, subctxt_rcvhdrbuf, arrsz[SUBCTXT_RCVHDRBUF]);

err_mmap_subctxt_rcvhdrbuf:
	/* if we got it here, subctxt_cnt must be != 0 */
	HFI_MUNMAP_ERRCHECK(binfo, subctxt_uregbase, arrsz[SUBCTXT_UREGBASE]);

err_mmap_subctxt_uregbase:
	HFI_MUNMAP_ERRCHECK(binfo, status_bufbase, arrsz[STATUS_BUFBASE]);

err_mmap_status_bufbase:
	HFI_MUNMAP_ERRCHECK(binfo, events_bufbase, arrsz[EVENTS_BUFBASE]);

err_mmap_events_bufbase:
	if(cinfo->runtime_flags & HFI1_CAP_DMA_RTAIL) {
		HFI_MUNMAP_ERRCHECK(binfo, rcvhdrtail_base, arrsz[RCVHDRTAIL_BASE]);
	}

err_mmap_rcvhdrtail_base:
	HFI_MUNMAP_ERRCHECK(binfo, user_regbase, arrsz[USER_REGBASE]);

err_mmap_user_regbase:
	/* the condition could be: if(cinfo->runtime_flags & HFI1_CAP_SDMA) too */
	if(binfo->sdma_comp_bufbase != 0) {
		HFI_MUNMAP_ERRCHECK(binfo, sdma_comp_bufbase, arrsz[SDMA_COMP_BUFBASE]);
	}

err_mmap_sdma_comp_bufbase:
	HFI_MUNMAP_ERRCHECK(binfo, rcvegr_bufbase, arrsz[RCVEGR_BUFBASE]);

err_mmap_rcvegr_bufbase:
	HFI_MUNMAP_ERRCHECK(binfo, rcvhdr_bufbase, arrsz[RCVHDR_BUFBASE]);

err_mmap_rcvhdr_bufbase:
	HFI_MUNMAP_ERRCHECK(binfo, pio_bufbase, arrsz[PIO_BUFBASE]);

err_mmap_pio_bufbase:
	HFI_MUNMAP_ERRCHECK(binfo, pio_bufbase_sop, arrsz[PIO_BUFBASE_SOP]);

err_mmap_pio_bufbase_sop:
	HFI_MUNMAP_ERRCHECK(binfo, sc_credits_addr, arrsz[SC_CREDITS]);

err_mmap_sc_credits_addr:
	return -1;
}

/* It is allowed to have multiple devices (and of different types)
   simultaneously opened and initialized, although this (still! Oct 07)
   unimplemented.  This routine is used by the low level hfi protocol code (and
   any other code that has similar low level functionality).
   This is the only routine that takes a file descriptor, rather than an
   struct _hfi_ctrl *.  The struct _hfi_ctrl * used for everything
   else is returned as part of hfi1_base_info.
*/
static struct _hfi_ctrl *opx_hfi_userinit_internal(int fd, bool skip_affinity,
		struct hfi1_user_info_dep *uinfo)
{
	struct _hfi_ctrl *spctrl = NULL;
	struct hfi1_ctxt_info *cinfo;
	struct hfi1_base_info *binfo;
	struct hfi1_cmd c;
	int __hfi_pg_sz;
#ifdef PSM2_SUPPORT_IW_CMD_API
	/* for major version 6 of driver, we will use uinfo_new.  See below for details. */
	struct hfi1_user_info uinfo_new = {0};
#endif

	/* First get the page size */
	__hfi_pg_sz = sysconf(_SC_PAGESIZE);
	_HFI_PDBG("CONTEXT INIT __hfi_pg_sz %d\n",__hfi_pg_sz);

	if (!(spctrl = calloc(1, sizeof(struct _hfi_ctrl)))) {
		_HFI_ERROR("Warning: can't allocate memory for hfi_ctrl: %s\n",
			  strerror(errno));
		goto err_calloc_hfi_ctrl;
	}
	cinfo = &spctrl->ctxt_info;
	binfo = &spctrl->base_info;

	_HFI_PDBG("CONTEXT INIT uinfo: ver %x, alg %d, subc_cnt %d, subc_id %d\n",
		  uinfo->userversion, uinfo->hfi1_alg,
		  uinfo->subctxt_cnt, uinfo->subctxt_id);

	/* 1. ask driver to assign context to current process */
	memset(&c, 0, sizeof(struct hfi1_cmd));
	c.type = OPX_HFI_CMD_ASSIGN_CTXT;

#ifdef PSM2_SUPPORT_IW_CMD_API
	/* If psm is communicating with a MAJOR version 6 driver, we need
	   to pass in an actual struct hfi1_user_info not a hfi1_user_info_dep.
	   Else if psm is communicating with a MAJOR version 5 driver, we can
	   just continue to pass a hfi1_user_info_dep as struct hfi1_user_info_dep
	   is identical to the MAJOR version 5 struct hfi1_user_info. */
	if (opx_hfi_get_user_major_version() == IOCTL_CMD_API_MODULE_MAJOR)
	{
		/* If psm is communicating with a MAJOR version 6 driver,
		   we copy uinfo into uinfo_new and pass uinfo_new to the driver. */
		c.len = sizeof(uinfo_new);
		c.addr = (__u64) (&uinfo_new);

		uinfo_new.userversion = uinfo->userversion;
		uinfo_new.pad         = uinfo->pad;
		uinfo_new.subctxt_cnt = uinfo->subctxt_cnt;
		uinfo_new.subctxt_id  = uinfo->subctxt_id;
		memcpy(uinfo_new.uuid,uinfo->uuid,sizeof(uinfo_new.uuid));
	}
	else
	{
		/* If psm is working with an old driver, we continue to use
		   the struct hfi1_user_info_dep version of the struct: */
		c.len = sizeof(*uinfo);
		c.addr = (__u64) uinfo;
	}
#else
	c.len = sizeof(*uinfo);
	c.addr = (__u64) uinfo;
#endif
	if (opx_hfi_cmd_write(fd, &c, sizeof(c)) == -1) {
		if (errno == ENODEV) {
			_HFI_ERROR("Warning: OPX and driver version mismatch\n");
			/* Overwrite errno. One would wish that the driver
			 * didn't return ENODEV for a version mismatch */
			errno = EPROTONOSUPPORT;
		} else {
			_HFI_ERROR("Warning: assign_context command failed: %s\n",
				  strerror(errno));
		}
		goto err_hfi_cmd_assign_ctxt;
	}

#ifdef PSM2_SUPPORT_IW_CMD_API
	if (opx_hfi_get_user_major_version() == IOCTL_CMD_API_MODULE_MAJOR)
	{
		/* for the new driver, we copy the results of the call back to uinfo from
		   uinfo_new. */
		uinfo->userversion = uinfo_new.userversion;
		uinfo->pad         = uinfo_new.pad;
		uinfo->subctxt_cnt = uinfo_new.subctxt_cnt;
		uinfo->subctxt_id  = uinfo_new.subctxt_id;
		memcpy(uinfo->uuid,uinfo_new.uuid,sizeof(uinfo_new.uuid));
	}
#endif

	/* 2. get context info from driver */
	c.type = OPX_HFI_CMD_CTXT_INFO;
	c.len = sizeof(*cinfo);
	c.addr = (__u64) cinfo;

	if (opx_hfi_cmd_write(fd, &c, sizeof(c)) == -1) {
		_HFI_ERROR("CTXT_INFO command failed: %s\n", strerror(errno));
		goto err_hfi_cmd_ctxt_info;
	}

	/* sanity checking... */
	if (cinfo->rcvtids%8) {
		_HFI_ERROR("rcvtids not 8 multiple: %d\n", cinfo->rcvtids);
		goto err_sanity_check;
	}
	if (cinfo->egrtids%8) {
		_HFI_ERROR("egrtids not 8 multiple: %d\n", cinfo->egrtids);
		goto err_sanity_check;
	}
	if (cinfo->rcvtids < cinfo->egrtids) {
		_HFI_ERROR("rcvtids(%d) < egrtids(%d)\n",
				cinfo->rcvtids, cinfo->egrtids);
		goto err_sanity_check;
	}
	if (cinfo->rcvhdrq_cnt%32) {
		_HFI_ERROR("rcvhdrq_cnt not 32 multiple: %d\n",
				cinfo->rcvhdrq_cnt);
		goto err_sanity_check;
	}
	if (cinfo->rcvhdrq_entsize%64) {
		_HFI_ERROR("rcvhdrq_entsize not 64 multiple: %d\n",
				cinfo->rcvhdrq_entsize);
		goto err_sanity_check;
	}
	if (cinfo->rcvegr_size%__hfi_pg_sz) {
		_HFI_ERROR("rcvegr_size not page multiple: %d\n",
				cinfo->rcvegr_size);
		goto err_sanity_check;
	}

	_HFI_PDBG("CONTEXT INIT ctxtinfo: runtime_flags %#016llx, rcvegr_size %d\n",
		  cinfo->runtime_flags, cinfo->rcvegr_size);
	/* The hex hell is for comparison */
	_HFI_PDBG("CONTEXT INIT ctxinfo: %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx), %s(%#016llx)\n",
		  cinfo->runtime_flags &  HFI1_CAP_DMA_RTAIL? "HFI1_CAP_DMA_RTAIL" : "!HFI1_CAP_DMA_RTAIL",(__u64)HFI1_CAP_DMA_RTAIL,
		  cinfo->runtime_flags &  HFI1_CAP_SDMA? "HFI1_CAP_SDMA" : "!HFI1_CAP_SDMA",(__u64)HFI1_CAP_SDMA,
		  cinfo->runtime_flags &  HFI1_CAP_SDMA_AHG? "HFI1_CAP_SDMA_AHG" : "!HFI1_CAP_SDMA_AHG",(__u64)HFI1_CAP_SDMA_AHG,
		  cinfo->runtime_flags &  HFI1_CAP_EXTENDED_PSN? "HFI1_CAP_EXTENDED_PSN" : "!HFI1_CAP_EXTENDED_PSN",(__u64)HFI1_CAP_EXTENDED_PSN,
		  cinfo->runtime_flags &  HFI1_CAP_HDRSUPP? "HFI1_CAP_HDRSUPP" : "!HFI1_CAP_HDRSUPP",(__u64)HFI1_CAP_HDRSUPP,
		  cinfo->runtime_flags &  HFI1_CAP_TID_RDMA? "HFI1_CAP_TID_RDMA" : "!HFI1_CAP_TID_RDMA",(__u64)HFI1_CAP_TID_RDMA,
		  cinfo->runtime_flags &  HFI1_CAP_USE_SDMA_HEAD? "HFI1_CAP_USE_SDMA_HEAD" : "!HFI1_CAP_USE_SDMA_HEAD",(__u64)HFI1_CAP_USE_SDMA_HEAD,
		  cinfo->runtime_flags &  HFI1_CAP_MULTI_PKT_EGR? "HFI1_CAP_MULTI_PKT_EGR" : "!HFI1_CAP_MULTI_PKT_EGR",(__u64)HFI1_CAP_MULTI_PKT_EGR,
		  cinfo->runtime_flags &  HFI1_CAP_NODROP_RHQ_FULL? "HFI1_CAP_NODROP_RHQ_FULL" : "!HFI1_CAP_NODROP_RHQ_FULL",(__u64)HFI1_CAP_NODROP_RHQ_FULL,
		  cinfo->runtime_flags &  HFI1_CAP_NODROP_EGR_FULL? "HFI1_CAP_NODROP_EGR_FULL" : "!HFI1_CAP_NODROP_EGR_FULL",(__u64)HFI1_CAP_NODROP_EGR_FULL,
		  cinfo->runtime_flags &  HFI1_CAP_TID_UNMAP? "HFI1_CAP_TID_UNMAP" : "!HFI1_CAP_TID_UNMAP",(__u64)HFI1_CAP_TID_UNMAP,
		  cinfo->runtime_flags &  HFI1_CAP_PRINT_UNIMPL? "HFI1_CAP_PRINT_UNIMPL" : "!HFI1_CAP_PRINT_UNIMPL",(__u64)HFI1_CAP_PRINT_UNIMPL,
		  cinfo->runtime_flags &  HFI1_CAP_ALLOW_PERM_JKEY? "HFI1_CAP_ALLOW_PERM_JKEY" : "!HFI1_CAP_ALLOW_PERM_JKEY",(__u64)HFI1_CAP_ALLOW_PERM_JKEY,
		  cinfo->runtime_flags &  HFI1_CAP_NO_INTEGRITY? "HFI1_CAP_NO_INTEGRITY" : "!HFI1_CAP_NO_INTEGRITY",(__u64)HFI1_CAP_NO_INTEGRITY,
		  cinfo->runtime_flags &  HFI1_CAP_PKEY_CHECK? "HFI1_CAP_PKEY_CHECK" : "!HFI1_CAP_PKEY_CHECK",(__u64)HFI1_CAP_PKEY_CHECK,
		  cinfo->runtime_flags &  HFI1_CAP_STATIC_RATE_CTRL? "HFI1_CAP_STATIC_RATE_CTRL" : "!HFI1_CAP_STATIC_RATE_CTRL",(__u64)HFI1_CAP_STATIC_RATE_CTRL,
		  cinfo->runtime_flags &  HFI1_CAP_OPFN? "HFI1_CAP_OPFN" : "!HFI1_CAP_OPFN",(__u64)HFI1_CAP_OPFN,
		  cinfo->runtime_flags &  HFI1_CAP_SDMA_HEAD_CHECK? "HFI1_CAP_SDMA_HEAD_CHECK" : "!HFI1_CAP_SDMA_HEAD_CHECK",(__u64)HFI1_CAP_SDMA_HEAD_CHECK,
		  cinfo->runtime_flags &  HFI1_CAP_EARLY_CREDIT_RETURN? "HFI1_CAP_EARLY_CREDIT_RETURN" : "!HFI1_CAP_EARLY_CREDIT_RETURN",(__u64)HFI1_CAP_EARLY_CREDIT_RETURN);
	_HFI_INFO("CONTEXT INIT ctxtinfo: active %d, unit %d, ctxt %d, subctxt %d\n",
		  cinfo->num_active, cinfo->unit, cinfo->ctxt, cinfo->subctxt);
	_HFI_INFO("CONTEXT INIT ctxtinfo: rcvtids %d, credits %d\n",
		  cinfo->rcvtids, cinfo->credits);
	_HFI_INFO("CONTEXT INIT ctxtinfo: numa %d, cpu %x, send_ctxt %d\n",
		  cinfo->numa_node, cinfo->rec_cpu, cinfo->send_ctxt);
	_HFI_INFO("CONTEXT INIT ctxtinfo: rcvhdrq_cnt %d, rcvhdrq_entsize %d\n",
		  cinfo->rcvhdrq_cnt, cinfo->rcvhdrq_entsize);
	_HFI_INFO("CONTEXT INIT ctxtinfo: egrtids %d, sdma_ring_size %d\n",
		  cinfo->egrtids, cinfo->sdma_ring_size);

	/* 4. Get user base info from driver */
	c.type = OPX_HFI_CMD_USER_INFO;
	c.len = sizeof(*binfo);
	c.addr = (__u64) binfo;

	if (opx_hfi_cmd_write(fd, &c, sizeof(c)) == -1) {
		_HFI_ERROR("BASE_INFO command failed: %s\n", strerror(errno));
		goto err_hfi_cmd_user_info;
	}

	opx_hfi_set_user_version(binfo->sw_version);


	_HFI_INFO("CONTEXT INIT baseinfo: hwver %x, swver %x, jkey %d, qp %d\n",
		  binfo->hw_version, binfo->sw_version,
		  binfo->jkey, binfo->bthqp);
	_HFI_INFO("CONTEXT INIT baseinfo: credit_addr %llx, sop %llx, pio %llx\n",
		  binfo->sc_credits_addr, binfo->pio_bufbase_sop,
		  binfo->pio_bufbase);
	_HFI_INFO("CONTEXT INIT baseinfo: hdrbase %llx, egrbase %llx, sdmabase %llx\n",
		  binfo->rcvhdr_bufbase, binfo->rcvegr_bufbase,
		  binfo->sdma_comp_bufbase);
	_HFI_INFO("CONTEXT INIT baseinfo: ureg %llx, eventbase %llx, "
		  "statusbase %llx, tailaddr %llx\n", binfo->user_regbase,
		  binfo->events_bufbase, binfo->status_bufbase,
		  binfo->rcvhdrtail_base);

	/*
	 * Check if driver version matches PSM version,
	 * this is different from PSM API version.
	 */
	if ((binfo->sw_version >> HFI1_SWMAJOR_SHIFT) != opx_hfi_get_user_major_version()) {
		_HFI_ERROR
		    ("User major version 0x%x not same as driver major 0x%x\n",
		     opx_hfi_get_user_major_version(), binfo->sw_version >> HFI1_SWMAJOR_SHIFT);
		if ((binfo->sw_version >> HFI1_SWMAJOR_SHIFT) < opx_hfi_get_user_major_version())
			goto err_version_mismatch;	/* else assume driver knows how to be compatible */
	} else if ((binfo->sw_version & 0xffff) != HFI1_USER_SWMINOR) {
		_HFI_PRDBG
		    ("User minor version 0x%x not same as driver minor 0x%x\n",
		     HFI1_USER_SWMINOR, binfo->sw_version & 0xffff);
	}

	if (map_hfi_mem(fd, spctrl, uinfo->subctxt_cnt) == -1) {
		_HFI_ERROR("Failed to map HFI memory.\n");
		goto err_map_hfi_mem;
	}

	/* Save some info. */
	spctrl->fd = fd;
	spctrl->__hfi_pg_sz = __hfi_pg_sz;
	spctrl->__hfi_unit = cinfo->unit;
	/*
	 * driver should provide the port where the context is opened for, But
	 * OPA driver does not have port interface to psm because there is only
	 * one port. So we hardcode the port to 1 here. When we work on the
	 * version of PSM for the successor to OPA, we should have port returned
	 * from driver and will be set accordingly.
	 */
	/* spctrl->__hfi_port = cinfo->port; */
	spctrl->__hfi_port = 1;
	spctrl->__hfi_tidegrcnt = cinfo->egrtids;
	spctrl->__hfi_tidexpcnt = cinfo->rcvtids - cinfo->egrtids;

	_HFI_PDBG("CONTEXT INIT spctrl->fd     %u \n",spctrl->fd);
	_HFI_PDBG("CONTEXT INIT spctrl->__hfi_unit     %u \n",spctrl->__hfi_unit);
	_HFI_PDBG("CONTEXT INIT spctrl->__hfi_port     %u \n",spctrl->__hfi_port);
	_HFI_PDBG("CONTEXT INIT spctrl->__hfi_tidegrcnt     %u \n",spctrl->__hfi_tidegrcnt);
	_HFI_PDBG("CONTEXT INIT spctrl->__hfi_tidexpcnt     %u \n",spctrl->__hfi_tidexpcnt);


	return spctrl;

err_map_hfi_mem:
err_version_mismatch:
err_hfi_cmd_user_info:
	/* TODO: restore the original CPU affinity? */

err_sanity_check:
err_hfi_cmd_ctxt_info:
	/* TODO: ioctl de-assign context here? */
	// without de-assigning the context, all subsequent opx_hfi_userinit_internal()
	// calls are going to fail
	_HFI_ERROR("An unrecoverable error occurred while communicating with the driver\n");
	abort(); /* TODO: or do we want to include psm_user.h to use psmi_handle_error()? */
	// no recovery here

	/* if we failed to allocate memory or to assign the context, we might still recover from this.
 	 * Returning NULL will cause the function to be reinvoked n times. Do we really want this
 	 * behavior?
	*/
err_hfi_cmd_assign_ctxt:
	free(spctrl);
	spctrl = NULL;

err_calloc_hfi_ctrl:
	return NULL;
}

struct _hfi_ctrl *opx_hfi_userinit(int fd, struct hfi1_user_info_dep *uinfo)
{
	return opx_hfi_userinit_internal(fd, false, uinfo);
}

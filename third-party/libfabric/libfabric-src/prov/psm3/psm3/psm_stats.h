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

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#ifndef _PSMI_IN_USER_H
#error psm_stats.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSM_STATS_H
#define _PSM_STATS_H

#include "mpspawn_stats.h"

#define PSMI_STATSTYPE_MQ	    	0x00001
#ifdef PSM_CUDA
#define PSMI_STATSTYPE_CUDA	    	0x00002 /* count of cuda calls */
#endif
#define PSMI_STATSTYPE_RCVTHREAD    0x00100	/* num_wakups, ratio, etc. */
#define PSMI_STATSTYPE_IPSPROTO	    0x00200	/* acks,naks,err_chks */
#define PSMI_STATSTYPE_TIDS	    	0x00400
#if 0	// unused code, specific to QLogic MPI
#define PSMI_STATSTYPE_P2P	    	0x00800	/* ep-to-ep details */
#endif
#define PSMI_STATSTYPE_MR_CACHE	    0x00800
#define PSMI_STATSTYPE_MEMORY	    0x01000
#ifdef RNDV_MOD
#define PSMI_STATSTYPE_RV_EVENT	    0x02000	/* RV user event */
#define PSMI_STATSTYPE_RV_RDMA	    0x04000	/* RV shared conn RDMA */
#endif
#define PSMI_STATSTYPE_FAULTINJ	    0x08000	/* fault injection - PSM_FI */
#define PSMI_STATSTYPE_ALL	    	0xfffff
#define _PSMI_STATSTYPE_SHOWZERO	0x100000

#if 0	// unused code, specific to QLogic MPI
#define PSMI_STATSTYPE_HFI	    (PSMI_STATSTYPE_RCVTHREAD|	\
				     PSMI_STATSTYPE_IPSPROTO |  \
				     PSMI_STATSTYPE_MEMORY |  \
				     PSMI_STATSTYPE_TIDS)
#endif

/* Used to determine how many stats in static array decl. */
#define PSMI_STATS_HOWMANY(entries)	    \
	    (sizeof(entries)/sizeof(entries[0]))

#define PSMI_STATS_DECL(_desc, _flags, _getfn, _val)   \
	{  .desc  = _desc,			    \
	   .flags = _flags,			    \
	   .getfn = _getfn,			    \
	   .u.val = _val,			    \
	}

#define PSMI_STATS_DECLU64(_desc, _val)					  \
	    PSMI_STATS_DECL(_desc,					  \
		MPSPAWN_STATS_REDUCTION_ALL | MPSPAWN_STATS_SKIP_IF_ZERO, \
		NULL,							  \
		_val)

#define PSMI_STATS_DECL_FUNC(_desc, _getfn)					  \
	    PSMI_STATS_DECL(_desc,					  \
		MPSPAWN_STATS_REDUCTION_ALL | MPSPAWN_STATS_SKIP_IF_ZERO, \
		_getfn,							  \
		NULL)

struct psmi_stats_entry {
	const char *desc;
	uint16_t flags;
	uint64_t(*getfn) (void *context); /* optional fn ptr to get value */
	union {
		uint64_t *val;	/* where value is stored if getfn is NULL */
		//uint64_t off;	/* or offset if that makes more sense */
	} u;
	uint64_t old_value;	/* value fetched from previous report */
};

static inline void
psmi_stats_init_u64(struct psmi_stats_entry *e, const char *desc, uint64_t *val)
{
	e->desc = desc;
	e->flags = MPSPAWN_STATS_REDUCTION_ALL | MPSPAWN_STATS_SKIP_IF_ZERO;
	e->getfn = NULL;
	e->u.val = val;
	e->old_value = 0;
}

/*
 * Copy the array of entries and keep track of the context
 * statstype and context form a unique key to identify the stats for deregister
 */
psm2_error_t
psmi_stats_register_type(const char *heading,
			 uint32_t statstype,
			 const struct psmi_stats_entry *entries,
			 int num_entries, uint64_t id, void *context,
			 const char *info);

/* deregister old copy and register a new one in it's place */
psm2_error_t
psmi_stats_reregister_type(const char *heading,
			 uint32_t statstype,
			 const struct psmi_stats_entry *entries,
			 int num_entries, uint64_t id, void *context,
			 const char *info);

psm2_error_t psmi_stats_deregister_type(uint32_t statstype, void *context);

psm2_error_t  psmi_stats_initialize(void);

void psmi_stats_finalize(void);

void psmi_stats_ep_close(void);	// let stats react to 1st ep close if desired

#endif /* PSM_STATS_H */

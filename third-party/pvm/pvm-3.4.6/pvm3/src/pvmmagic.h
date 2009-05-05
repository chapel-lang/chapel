
/* $Id: pvmmagic.h,v 1.2 1997/06/25 22:11:08 pvmsrc Exp $ */

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */

/*
 * pvmmagic.h
 *
 *	magic numbers for pvm data structures. 
 *
 */


/* Magic #'s  are segmented by use (magic segment)
 * Each segment defines it own use (magic category) 
*/

#ifndef __PVMMAGIC_H__

#define __PVMMAGIC_H__

typedef unsigned int PVMMAGIC;

#define MAGIC_SEGSHIFT 15
#define MAGICMASK	 0xffff	 

#define BADMAGIC(_ref,_test) ((_ref)!=(_test) ? 1 : 0)
									  		
									

#define MAKE_MAGIC(_seg, _cat) ((((_seg) & MAGICMASK) << 15 ) \
								| ((_cat) & MAGICMASK))

/* ------- Define magic segments ---------- */
/* Segment numbers should not change in a release. */
/* They can be added to */

#define MG_DDPRO_SEG		1
#define MG_GLOBAL_SEG		2
#define MG_HOST_SEG			3
#define MG_IMALLOC_SEG		4
#define MG_LPVM_SEG			5
#define MG_LPVMCAT_SEG		6
#define MG_LPVMGEN_SEG		7
#define MG_LPVMGLOB_SEG		8
#define MG_LPVMMIMD_SEG		9
#define MG_LMSG_SEG			10
#define MG_LPVMPACK_SEG		11
#define MG_LPVMSHMEM_SEG	12
#define MG_MSGBOX_SEG		13
#define MG_NMDCLASS_SEG		14
#define MG_PKT_SEG			15
#define MG_PMSG_SEG			16
#define MG_PVMALLOC_SEG		17
#define MG_PVMCRUFT_SEG		18
#define MG_PVMD_SEG			19
#define MG_PVMDABUF_SEG		20
#define MG_PVMDPACK_SEG		21
#define MG_PVMDSHMEM_SEG	22
#define MG_PVMDTEV_SEG		23
#define MG_PVMDUNIX_SEG		24
#define MG_PVMERR_SEG		25
#define MG_PVMFRAG_SEG		26
#define MG_PVMLOG_SEG		27
#define MG_PVMSHMEM_SEG		28
#define MG_SDPRO_SEG		29
#define MG_TASK_SEG			30
#define MG_TDPROC_SEG		31
#define MG_TEV_SEG			32
#define MG_WAITC_SEG		33


/* ----- Magic Categories ------- */
/* -- MG_LMSG_SEG -- */

#define LMSG_MSGID	MAKE_MAGIC(MG_LMSG_SEG,1) 


/* ------ END of Magic Categories ----------- */

#endif /* ifndef __PVMMAGIC__ */


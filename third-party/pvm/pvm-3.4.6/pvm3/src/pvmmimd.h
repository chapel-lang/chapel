
/* $Id: pvmmimd.h,v 1.7 1999/07/08 19:00:24 kohl Exp $ */

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
 *  pvmmimd.h
 *
 * $Log: pvmmimd.h,v $
 * Revision 1.7  1999/07/08 19:00:24  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.6  1997/06/25  22:11:09  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.5  1997/05/05  20:06:15  pvmsrc
 * 	Extend the size of the info array passed to mpp tasks on startup.
 *
 * Revision 1.4  1997/04/25  19:15:42  pvmsrc
 * added BUSYWAIT #def needed by shmem version (via lpvmglob.c)
 *
 * Revision 1.3  1997/03/06  21:12:58  pvmsrc
 * 		- Changed everything :-)
 * 			tid structure changed.
 * 			lots of macro changes
 * 			defines for message types.
 * 			reorganized.
 *
 * Revision 1.2  1997/01/28  19:28:22  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:43:30  pvmsrc
 * Initial revision
 *
 * Revision 1.8  1995/06/16  16:09:30  manchek
 * changed size of PGON host info array to 10
 *
 * Revision 1.7  1995/06/12  18:24:20  manchek
 * SIZEHINFO only affects PGON for now
 *
 * Revision 1.6  1995/06/12  16:01:56  manchek
 * changed SIZEHINFO to 6
 *
 * Revision 1.5  1995/05/30  17:50:23  manchek
 * Added new defines for SP2MPI architecture
 *
 * Revision 1.4  1994/11/08  19:19:23  manchek
 * mpp fixes
 *
 * Revision 1.3  1994/06/03  20:38:25  manchek
 * version 3.3.0
 *
 * Revision 1.2  1993/12/20  15:39:44  manchek
 * patch 6 from wcj
 *
 * Revision 1.1  1993/08/30  23:26:51  manchek
 * Initial revision
 *
 */

#ifndef __PVMMIMD_H__
#define __PVMMIMD_H__
/* ====== SECTION I (STRUCTURE DEFINITIONS) ====== */

/* nodes in use */
struct nodeset {
	struct nodeset *n_link;		/* linked list */
	struct nodeset *n_rlink;
	int n_size;					/* number of nodes */
	int n_first;				/* the first node in the set */
	int n_ptype;				/* (PGON) ptype of tasks on the nodes  */
	int n_alive;				/* number of active nodes */
	int n_ptid;					/* tid of task who spawned us */
#ifdef IMA_I860
#define n_cid	n_ptype			/* the global qm_cubeid */
	char *n_name;				/* cube name */
#endif
};

/* ====== SECTION II (TIDS) ====== */

/* PVM task identifiers are 32 bits. The first 18 bits are assigned
 * by the local pvmd. The following summarizes the pvm tid breakdown:
 *
 *    |31 |30 |29                  18|17                      0|
 *    | s | g |   < host part >      |    <local part>         |
 *
 * MPPs further break down the <local part> as follows:
 *
 *     17  16    14 13             0
 *    | n | <part> |   <phys node>  |
 *
 * bits  0 - 13 represent the physical node # --> 16384 nodes
 * bits 14 - 16 represent partition ids --> 8 simulatenous partitions
 * bit  17 - 17 task is a compute node (bit set) or service node (not set)
*/
	
#define NODEWIDTH		14		/* 14 bits for nodes - 16K nodes */
#define PARTWIDTH		3		/* 4 bits for partitions - 8 partitions */
#define CNODEWIDTH		1		/* 1 bit to indicate a node task */
/*						---
 *						18      <--- must sum to 18, unless tid defn changes 
*/

#define MAXNODES		(1 << NODEWIDTH )   /* 2^NODEWIDTH */
#define NPARTITIONS		(1 << PARTWIDTH )	/* 2^PARTWIDTH */
#define NODESTATES		(1 << CNODEWIDTH)   /* 2^CNODEWIDTH */


/* Masks for the above information */

/* -- First NODEWIDTH bits, mask for the node number */
#define TIDNODE		(MAXNODES - 1) 					

/* -- next PARTWIDTH bits, mask for the partition identifier */
#define TIDPTYPE	((NPARTITIONS -1 ) << NODEWIDTH)

/* -- next NODESTATES bits, mask for service/compute part residency */
#define TIDONNODE	((NODESTATES - 1) << (NODEWIDTH + PARTWIDTH))

/*
#define TIDNODE			0x00003fff     first NODEWIDTH BITS 
#define TIDPTYPE	    0x0001c000     next  PARTWIDTH BITS 
#define TIDONNODE		0x00020000     next  CNODEWIDTH BITS 
*/
/* ====== SECTION III (CONSTANTS, MESSAGE CONSTANTS) ====== */


#define MAXFRAGSIZE	(0x00004000 - 16)	/* max fragment size (16K - 16) */
#define MXPP		256			/* maximum number of pre-posted recv buffers */
#define MPPANY		-1
#define MPPREPOST	1

#define PVMDPTYPE	0			/* process type of pvmd */
#define SIZEHINFO	20			/* size of hostinfo array */

											 /* need by lpvmglob */
#define      BUSYWAIT        1000000         /* max wait cycles before backing off */

/* Packed messages arrive in pre-posted receive buffers with a certain
 * physical tag.  For peer-peer communication, pvm message fragments 
 * arrive in buffers with tags in the range [PMTPBASE, PMTPBASE+MXPP-1].
 * Bodies of inplace messages arrive  
*/

#define PMTCONF	 	(MAXNODES+1)	/* Initial configuration message */
#define PMTHOST		(PMTCONF+1)		/* tag for bcasting myhostpart (I860) */
#define PMTPHYS		(PMTCONF+2)		/* tag for physical node# (I860) */
#define PMTDBASE	(PMTCONF+3)		/* base tag for pre-posted recvs from pvmd*/
#define PMTPBASE	(PMTDBASE+MXPP)	/* base tag for pre-posted recvs from peer*/



/* ====== SECTION IV (MACROS) ====== */

/* determine if task is a node process */
#define TIDISNODE(x)	(!((x)&(TIDGID|TIDPVMD)) && ((x)&TIDONNODE))

/* extract the ptype bits */
#define TIDTOTYPE(x)	((x)&TIDPTYPE)>>(NODEWIDTH)

/* ====== SECTION V (Prototypes) ====== */

struct ttpcb * mpp_ttpcb_find __ProtoGlarp__( (int tid) ); 
struct frag * pvm_readfrompvmd __ProtoGlarp__( () );
struct frag * pvm_readfrompeer __ProtoGlarp__( () );
struct pmsg * pvm_mpp_pmsgs __ProtoGlarp__( () );

#endif

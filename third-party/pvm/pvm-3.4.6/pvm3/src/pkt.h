
/* $Id: pkt.h,v 1.6 1999/07/08 19:00:22 kohl Exp $ */

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
 *	pkt.h
 *
 *	Packet buffers.
 *
 * $Log: pkt.h,v $
 * Revision 1.6  1999/07/08 19:00:22  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.5  1997/06/25  22:09:17  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.4  1997/04/24  20:59:42  pvmsrc
 * added pk_rip structure member (used on mpps)
 *
 * Revision 1.3  1997/03/06  21:09:10  pvmsrc
 * 	- define __PKT_H__ so that multiple includes of pkt.h won't honk.
 *
 * Revision 1.2  1997/01/28  19:28:16  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:43:22  pvmsrc
 * Initial revision
 *
 * Revision 1.3  1995/07/24  18:26:09  manchek
 * added message header fields and cpos, removed PF_STARTED
 *
 * Revision 1.2  1994/06/03  20:38:20  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:49  manchek
 * Initial revision
 *
 */

#ifndef __PKT_H__
#define __PKT_H__
struct pkt {
	struct pkt *pk_link;		/* queue or 0 */
	struct pkt *pk_rlink;
	struct pkt *pk_tlink;		/* scheduling queue or 0 */
	struct pkt *pk_trlink;
	char *pk_buf;				/* buffer or zero if master pkt */
	char *pk_dat;				/* data */
	int pk_max;					/* size of buffer */
	int pk_len;					/* length of data */
	int pk_src;					/* source tid */
	int pk_dst;					/* dest tid */
	int pk_flag;				/* fragment flags */
	int pk_enc;					/* message encoding */
	int pk_tag;					/* message type tag */
	int pk_ctx;					/* message context */
	int pk_wid;					/* message wait context id */
	int pk_crc;					/* message checksum */
	int pk_nrt;					/* retry count */
	int pk_seq;					/* seq num */
	int pk_ack;					/* ack num */
	int pk_rip;					/* received in place - used on mpps */
	struct hostd *pk_hostd;		/* receiving host */
	struct timeval pk_rtv;		/* time to retry */
	struct timeval pk_rta;		/* next-retry accumulator */
	struct timeval pk_rto;		/* total time spent on pkt */
	struct timeval pk_at;		/* time pkt first sent */
	char *pk_cpos;				/* write pointer for local protocol */
};

struct pkt *pk_new __ProtoGlarp__((int n));
void pk_free __ProtoGlarp__((struct pkt *p));
#endif

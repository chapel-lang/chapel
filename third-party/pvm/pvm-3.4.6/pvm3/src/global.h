
/* $Id: global.h,v 1.12 2001/02/07 23:14:03 pvmsrc Exp $ */

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
 *	global.h
 *
 *	Generic includes.
 *
 * $Log: global.h,v $
 * Revision 1.12  2001/02/07 23:14:03  pvmsrc
 * First Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.11  1999/07/08 19:00:20  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.10  1999/01/28  18:54:58  pvmsrc
 * Added alternate WIN32 pvmd path define:
 * 	- WINPVMDPATH = "%PVM_ROOT%\\lib\\WIN32\\pvmd3.exe"
 * 	- allows retry for WIN32 hosts if $PVM_ROOT/lib/pvmd fails
 * 		(PVMDPATH).
 * (Spanker=kohl)
 *
 * Revision 1.9  1998/10/02  15:43:56  pvmsrc
 * Single source code merge of Win32 and Unix code.
 * (Spanker=sscott)
 *
 * Revision 1.8  1997/08/29  13:35:01  pvmsrc
 * OS2 Port Submitted by Bohumir Horeni, horeni@login.cz.
 * (Spanker=kohl)
 *
 * Revision 1.7  1997/06/25  22:08:44  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.6  1997/04/09  14:37:53  pvmsrc
 * PVM patches from the base 3.3.10 to 3.3.11 versions where applicable.
 * Originals by Bob Manchek. Altered by Graham Fagg where required.
 *
 * Revision 1.5  1997/04/08  19:57:50  pvmsrc
 * Promoted mbox static "classes" to public global "pvmmboxclasses".
 * 	- so pvmd can spank mboxes in ddpro.c...  :-Q
 * 	- renamed everywhere, moved decl / extern to global.[ch].
 *
 * Revision 1.4  1997/03/06  20:56:26  pvmsrc
 * 	add include host.h
 *
 * Revision 1.3  1997/01/28  19:27:55  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.2  1996/10/24  20:37:02  pvmsrc
 * Modified for New Tracing Facility:
 * 	- added new PDMTRACE constant for tracer debug tracing...
 * 	- moved in miscellaneous external declarations:
 * 		* PVM Error Globals:  char *pvm_errlist[], int pvm_nerr,
 * 			int pvm_errno.
 * 		* wait list:  struct waitc *waitlist.
 * 		* trace globals:  struct Pvmtevdid pvmtevdidlist[],
 * 			struct Pvmtevinfo pvmtevinfo[].
 *
 * Revision 1.1  1996/09/23  23:43:15  pvmsrc
 * Initial revision
 *
 * Revision 1.21  1996/05/13  20:24:52  manchek/GEF
 * changed DEFBINDIR to $HOME...
 * release patch 11
 *
 * Revision 1.20  1995/11/02  16:00:37  manchek
 * version change
 *
 * Revision 1.19  1995/09/05  19:28:49  manchek
 * version change
 *
 * Revision 1.18  1995/07/24  18:24:19  manchek
 * added TIDISHERE macro
 *
 * Revision 1.17  1995/05/17  16:07:30  manchek
 * version 3.3.8
 *
 * Revision 1.16  1995/02/03  16:46:47  manchek
 * version 3.3.7
 *
 * Revision 1.15  1994/12/21  21:27:34  manchek
 * version 3.3.6
 *
 * Revision 1.14  1994/10/15  19:05:01  manchek
 * version 3.3.5
 *
 * Revision 1.13  1994/09/02  15:20:38  manchek
 * version 3.3.4
 *
 * Revision 1.12  1994/07/18  19:20:00  manchek
 * version to 3.3.3
 *
 * Revision 1.11  1994/07/01  15:08:02  manchek
 * version to 3.3.2
 *
 * Revision 1.10  1994/06/21  18:30:26  manchek
 * to version 3.3.1
 *
 * Revision 1.9  1994/06/03  20:38:55  manchek
 * oops
 *
 * Revision 1.8  1994/06/03  20:38:14  manchek
 * version 3.3.0
 *
 * Revision 1.7  1993/12/20  15:42:26  manchek
 * version change
 *
 * Revision 1.6  1993/11/30  15:49:14  manchek
 * version change
 *
 * Revision 1.5  1993/10/27  17:32:16  manchek
 * version change
 *
 * Revision 1.4  1993/10/12  14:44:31  manchek
 * version change
 *
 * Revision 1.3  1993/09/23  20:53:50  manchek
 * version change
 *
 * Revision 1.2  1993/09/22  18:59:46  manchek
 * version change
 *
 * Revision 1.1  1993/08/30  23:26:47  manchek
 * Initial revision
 *
 */


/* UDPMAXLEN should be set to the largest UDP message length
   your system can handle. */

#ifndef	UDPMAXLEN

#if	defined(IMA_CRAY) || defined(IMA_CRAY2)
#include <sys/tcp_config.h>
#define	UDPMAXLEN	UDPSENDSPACE/4 /* UDPSENDSPACE/2 whacks on some C-90s */
#endif

#endif	/*!UDPMAXLEN*/

/*	16 is the number of bytes taken by align_me, which is used to keep
	reference information to a da_buf.
	By subtracting this number from UDPMAXLEN, we can assure ourselves 
	that da_bufs do not spill over a few bytes onto the next page of
	memory. Should keep us from *always* page faulting on a send 
*/

#define KEEPONPAGE 16

#ifndef	UDPMAXLEN
#define	UDPMAXLEN	(4096 - KEEPONPAGE)	/* generic max fragment length */
#endif


/*DocThis*/
/*
 * TID definitions
 *
 *    3                   2                   1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +---------------+---------------+---------------+---------------+
 * |               |               |               |               |
 * +---------------+---------------+---------------+---------------+
 *  S G \__________ H ________/ \_______________ L _______________/
 */
/*EndDocThis*/

#define	TIDHOST		0x3ffc0000
#define	TIDLOCAL	0x0003ffff
#define	TIDGID		0x40000000
#define	TIDPVMD		0x80000000

#define	TIDISTASK(x)	(!((x)&(TIDGID|TIDPVMD)) && ((x)&TIDLOCAL))
#define	TIDISPVMD(x)	(((x)&(TIDGID|TIDPVMD)) == TIDPVMD && !((x)&TIDLOCAL))
#define	TIDISMCA(x)		(((x)&(TIDGID|TIDPVMD)) == TIDGID)
#define	TIDISHERE(x,me)	(!((x) & TIDHOST) || !(((x) ^ (me)) & TIDHOST))


/*
* Message header
*/

#define	MSGHDRLEN	32

/*DocThis*/
/*
 *    0               1               2               3
 *   +---------------+---------------+---------------+---------------+
 *  0|                        data signature                         |
 *   +---------------------------------------------------------------+
 *  4|                              tag                              |
 *   +---------------------------------------------------------------+
 *  8|                            context                            |
 *   +---------------------------------------------------------------+
 * 12|                        sequence number                        |
 *   +---------------------------------------------------------------+
 * 16|                            wait id                            |
 *   +---------------------------------------------------------------+
 * 20|                           checksum                            |
 *   +---------------------------------------------------------------+
 * 24|                           reserved                            |
 *   +---------------------------------------------------------------+
 * 28|                           reserved                            |
 *   +---------------------------------------------------------------+
 */
/*EndDocThis*/


/* frag and packet flags */

#define	FFSOM		1		/* first frag of msg */
#define	FFEOM		2		/* final frag of msg */
#define FFMCA		8		/* This is a multicast message */
#define FFMCAWH		16		/* Output should write the header of a mcast msg */

#ifdef IMA_OS2
#define DEFBINDIR	"$HOME/pvm3/bin/$PVM_ARCH;$PVM_ROOT/bin/$PVM_ARCH"
#endif
#ifdef WIN32
#define DEFBINDIR	"$PVM_ROOT\\bin\\$PVM_ARCH"
#endif
#ifdef CYGWIN
#define	DEFBINDIR	"$HOME/pvm3/bin/$PVM_ARCH:$PVM_ROOT_U/bin/$PVM_ARCH"
#endif

#ifndef	DEFBINDIR
#define	DEFBINDIR	"$HOME/pvm3/bin/$PVM_ARCH:$PVM_ROOT/bin/$PVM_ARCH"
#endif

#ifndef	DEFDEBUGGER
#define	DEFDEBUGGER	"$PVM_ROOT/lib/debugger"
#endif

#ifdef IMA_OS2
#define PVMDFILE	"lib/pvmd.cmd"
#endif
#ifdef CYGWIN
#define	PVMDFILE	"lib/Win32/pvmd3.exe"
#endif

#ifndef	PVMDFILE
#define	PVMDFILE	"lib/pvmd"
#endif

#ifndef	PVMDPATH
#define	PVMDPATH	"$PVM_ROOT/lib/pvmd"
#endif

#ifndef	WINPVMDPATH
#define	WINPVMDPATH	"%PVM_ROOT%\\lib\\WIN32\\pvmd3.exe"
#endif


/* debug mask for pvmd and libpvm */

#define	PDMPACKET	0x00001		/* packet tracing */
#define	PDMMESSAGE	0x00002		/* message tracing */
#define	PDMTASK		0x00004		/* task state */
#define	PDMSTARTUP	0x00008		/* slave pvmd startup */
#define	PDMHOST		0x00010		/* host table updates */
#define	PDMSELECT	0x00020		/* select loop/fd set stuff */
#define	PDMNET		0x00040		/* IP network stuff */
#define	PDMNODE		0x00080		/* multiprocessor control */
#define	PDMSCHED	0x00100		/* resource manager tasks stuff */
#define	PDMAPPL		0x00200		/* application (scrapped msgs/stuff) */
#define	PDMWAITC	0x00400		/* wait contexts */
#define	PDMMEM		0x00800		/* shared memory */
#define	PDMSEM		0x01000		/* semaphores */
#define	PDMLOCK		0x02000		/* locks */
#define	PDMROUTE	0x04000		/* message routing */
#define	PDMMBOX		0x08000		/* message mailbox */
#define	PDMTRACE	0x10000		/* tracer tracing */
#define PDMMCA		0x20000		/* multicast tracking */


/* General PVM Globals Declarations */

extern	char *pvm_errlist[];		/* error messages for -pvm_errno */

extern	int pvm_nerr;				/* exported num of errors */
extern	int pvm_errno;				/* last err in PVM API funct */

extern	struct waitc *waitlist;		/* wait list */

extern	struct pvmmclass *pvmmboxclasses;		/* the mbox database */


/* General Trace Globals Declarations */

extern	struct Pvmtevdid pvmtevdidlist[];

extern	struct Pvmtevinfo pvmtevinfo[];



static char rcsid[] =
	"$Id: lpvmglob.c,v 1.19 2004/01/14 18:50:57 pvmsrc Exp $";

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
 *	lpvmglob.c
 *
 *	Libpvm Globals.
 *
 * $Log: lpvmglob.c,v $
 * Revision 1.19  2004/01/14 18:50:57  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.18  2001/02/07 23:14:08  pvmsrc
 * First Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.17  2000/06/15 17:51:52  pvmsrc
 * Fixed bug in WIN32 direct routing.
 * 	- stupid #endif in the wrong place, pvm_fd_add() call whacked.
 * 	- turned back on direct routing default and setopt.
 * (Spanker=kohl)
 *
 * Revision 1.16  2000/02/17 23:12:15  pvmsrc
 * *** Changes for new BEOLIN port ***
 * 	- MPP-like, similar to SP2, etc.
 * 	- submitted by Paul Springer <pls@smokeymt.jpl.nasa.gov>.
 * 	- format-checked & cleaned up by Jeembo...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.15  2000/02/16 22:01:00  pvmsrc
 * Added #ifndef NO_NETINET_TCP_H around <netinet/tcp.h> for
 * 	archs that can't handle it...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.14  1999/07/08 18:59:59  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.13  1999/03/12  20:55:37  pvmsrc
 * Don't allow direct routing in WIN32 until we fix it.  Shit.
 * (Spanker=kohl)
 *
 * Revision 1.12  1998/11/20  20:04:07  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (Spanker=sscott)
 *
 * Revision 1.11  1997/09/22  21:13:32  pvmsrc
 * Added new pvmsettaskname() linkage (for shell-spawned tasks only!).
 * 	- call pvmsettaskname() before joining PVM, sends task name
 * 		(stored in new char *pvmmytaskname global) to pvmd in
 * 		TM_CONN2 message.
 * 	- appears in trace events and console ps.
 * (Spanker=kohl)
 *
 * Revision 1.10  1997/08/06  22:43:18  pvmsrc
 * Added new SGI6 and SGIMP6 arches.
 *
 * Revision 1.9  1997/06/27  19:53:56  pvmsrc
 * Fixed polltype/polltime bug in AIX4MP.
 *
 * Revision 1.8  1997/06/27  17:32:25  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.7  1997/05/29  15:13:06  pvmsrc
 * Moved decl (init) of pvmrouteopt inside MPP/SHMEM vs. "normal" defines.
 * 	- for MPP/SHMEM -> PvmDontRoute, "normal" -> PvmAllowDirect.
 *
 * Revision 1.6  1997/05/21  16:01:52  pvmsrc
 * Updated ifdefs to include AIX4MP arch type.
 *
 * Revision 1.5  1997/03/06  21:08:28  pvmsrc
 * 		Added includes:
 * 		<netinet/in.h>, <netinet/tcp.h> "host.h" "pvmmimd.h"
 *
 * Revision 1.4  1997/02/13  23:35:54  pvmsrc
 * Added new PvmNoReset setopt/getopt option.
 * 	- added new pvmnoreset, pvmnoresetindex globals in lpvm.[ch].
 * 	- to set use pvm_putinfo() -> PVMNORESETCLASS first avail w/lock,
 * 		save index in pvmnoresetindex.
 * 	- to unset use pvm_delinfo().
 *
 * Revision 1.3  1997/01/28  19:26:28  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.2  1996/12/18  22:28:55  pvmsrc
 * Added new FILE *logfp decl.
 * 	- only #ifdef LOG.  (previously only in shmem stuff)
 * 	- needed for now shared pvmlogerror() routine.
 *
 * Revision 1.1  1996/10/24  19:43:39  pvmsrc
 * Initial revision
 *
 *
 */


#include <stdio.h>
#include <pvm3.h>

#if defined(WIN32) || defined(CYGWIN)
#include "..\xdr\types.h"
#include "..\xdr\xdr.h"
#else
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

#ifndef WIN32
#include <netinet/in.h>
#ifndef NO_NETINET_TCP_H
#include <netinet/tcp.h>
#endif
#endif

#include "pmsg.h"
#include <pvmtev.h>
#include "tevmac.h"
#include "host.h"
#include "waitc.h"
#include "global.h"



/***************
 **  Globals  **
 **           **
 ***************/

int pvmautoerr = 1;						/* whether auto print err msg */

struct Pvmtracer pvmctrc =				/* child tracer info */
	{ 0, 0, 0, 0, 0, 0, 0, 0, "" };

struct Pvmtracer pvmtrc =				/* task tracer info */
	{ 0, 0, 0, 0, 0, 0, 0, 0, "" };

int pvmmyptid = -1;						/* parent task id */
int pvmmytid = -1;						/* this task id */
int pvmmyupid = -1;						/* process unix pid */
int pvm_useruid = -1;					/* user's unix uid */
int pvmmydsig = 0;						/* native data enc, init XDR */
int pvmschedtid = 0;					/* scheduler task */
int pvmrescode = 0;						/* allow reserved tids, tags */
int pvmshowtaskid = 1;					/* catchout shows task ids */
int pvmtoplvl = 1;						/* funct called outside lib */
int pvmdebmask = 0;						/* which debugging info */
int pvmnoreset = 0;						/* task reset override */
int pvmnoresetindex = -1;				/* task reset overrd mb index */

char *pvmmytaskname = (char *) NULL;	/* task name */

#if defined(IMA_CM5) || defined(IMA_CUBE) || defined(IMA_I860) \
  || defined(IMA_PGON) || defined(IMA_SP2MPI) || defined(IMA_ALPHAMP) \
  || defined(IMA_CSPP) || defined(IMA_HPPAMP) || defined(IMA_RS6KMP) \
  || defined(IMA_SGIMP) || defined(IMA_SGIMP6) || defined(IMA_SGIMP64) \
  || defined(IMA_SUNMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP) \
  || defined(AIX4SP2) || defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
#include "pvmmimd.h"
int pvmrouteopt = PvmDontRoute;			/* task-task routing style */
int pvmfrgsiz = MAXFRAGSIZE;			/* msg frag length (to pack) */
int pvmudpmtu = MAXFRAGSIZE;			/* local UDP MTU */
#else
int pvmrouteopt = PvmAllowDirect;		/* task-task routing style */
int pvmfrgsiz = UDPMAXLEN;				/* msg frag length (to pack) */
int pvmudpmtu = UDPMAXLEN;				/* local UDP MTU */
#endif

int pvmmyctx = 0;						/* current message context */
int pvmnsibs = -1;						/* number in our spawn group */
int *pvmsibtids = 0;					/* tids in our spawn group */

struct pmsg *pvmrxlist = 0;				/* not-recvd msg list */

struct pmsg *pvmrbuf = 0;				/* current recv/unpack msg */
struct pmsg *pvmsbuf = 0;				/* current send/pack msg */


#if defined(IMA_ALPHAMP) || defined(IMA_CSPP) || defined(IMA_HPPAMP) \
	|| defined(IMA_RS6KMP) || defined(IMA_SGIMP) \
	|| defined(IMA_SGIMP6) || defined(IMA_SGIMP64) \
	|| defined(IMA_AIX4MP) || defined(IMA_AIX5MP) \
	|| defined(IMA_SUNMP)
int pvmpolltype = PvmPollSleep;			/* memory polling style */
int pvmpolltime = BUSYWAIT;				/* time value for poll wait */
#endif


#ifdef LOG
FILE *logfp = 0;						/* my own log file */
#endif


/* General Trace Globals */

struct pvmtrcencvec *pvmtrccodef;		/* trace packing vector */

struct timeval pvmtrcztv = { 0, 0 };	/* trace zero time value */

struct pmsg *pvmtrcmp;					/* trace record buffer mesg */

int pvmtrcsbfsave = 0;					/* trace save send buffer */
int pvmtrcsbf = 0;						/* trace record buffer */

int pvmtrcdesc;							/* trace descriptor status */
int pvmtrctmp;							/* trace temporary storage */

int pvmtrcsavekind;						/* trace event kind for fin */



/* $Id: lpvm.h,v 1.14 2004/01/14 18:50:56 pvmsrc Exp $ */

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
 *	lpvm.h
 *
 *	Libpvm Globals Declarations.
 *
 * $Log: lpvm.h,v $
 * Revision 1.14  2004/01/14 18:50:56  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.13  2000/02/16 22:00:59  pvmsrc
 * Added #ifndef NO_NETINET_TCP_H around <netinet/tcp.h> for
 * 	archs that can't handle it...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.12  1999/07/08 19:00:21  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.11  1997/09/22  21:13:28  pvmsrc
 * Added new pvmsettaskname() linkage (for shell-spawned tasks only!).
 * 	- call pvmsettaskname() before joining PVM, sends task name
 * 		(stored in new char *pvmmytaskname global) to pvmd in
 * 		TM_CONN2 message.
 * 	- appears in trace events and console ps.
 * (Spanker=kohl)
 *
 * Revision 1.10  1997/08/06  22:43:13  pvmsrc
 * Added new SGI6 and SGIMP6 arches.
 *
 * Revision 1.9  1997/06/27  19:53:55  pvmsrc
 * Fixed polltype/polltime bug in AIX4MP.
 *
 * Revision 1.8  1997/06/27  17:32:22  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.7  1997/05/07  21:23:28  pvmsrc
 * Added new pvmupkstralloc() for dynamic string unpacking lunacy.
 * 	- with proto in header.
 *
 * Revision 1.6  1997/03/17  20:56:34  pvmsrc
 * include sys/types.h for SUNOS in.h problem.
 *
 * Revision 1.5  1997/03/06  21:06:58  pvmsrc
 * 		define struct ttpcb and TT states
 * 		added includes for this struct defn
 *
 * Revision 1.4  1997/02/13  23:35:42  pvmsrc
 * Added new PvmNoReset setopt/getopt option.
 * 	- added new pvmnoreset, pvmnoresetindex globals in lpvm.[ch].
 * 	- to set use pvm_putinfo() -> PVMNORESETCLASS first avail w/lock,
 * 		save index in pvmnoresetindex.
 * 	- to unset use pvm_delinfo().
 *
 * Revision 1.3  1997/01/28  19:27:58  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.2  1996/12/18  22:28:54  pvmsrc
 * Added new FILE *logfp decl.
 * 	- only #ifdef LOG.  (previously only in shmem stuff)
 * 	- needed for now shared pvmlogerror() routine.
 *
 * Revision 1.1  1996/10/24  19:44:17  pvmsrc
 * Initial revision
 *
 *
 */


#include <sys/types.h>
#ifndef WIN32
#include <netinet/in.h>
#ifndef NO_NETINET_TCP_H
#include <netinet/tcp.h>
#endif
#endif

#ifdef IMA_MPP
#include "mppchunk.h"
#endif
extern	int pvmautoerr;				/* whether to auto print err msgs */

extern	struct Pvmtracer pvmctrc;	/* child tracer info */
extern	struct Pvmtracer pvmtrc;	/* task tracer info */

extern	int pvmmyptid;				/* parent task id */
extern	int pvmmytid;				/* this task id */
extern	int pvmmyupid;				/* process unix pid */
extern	int pvm_useruid;			/* user's unix uid */
extern	int pvmmydsig;				/* native data enc, init XDR */
extern	int pvmschedtid;			/* scheduler task */
extern	int pvmrescode;				/* allow reserved tids, tags */
extern	int pvmshowtaskid;			/* catchout shows task ids */
extern	int pvmtoplvl;				/* funct called from outside lib */
extern	int pvmdebmask;				/* which debugging info */
extern	int pvmrouteopt;			/* task-task routing style */
extern	int pvmnoreset;				/* task reset override */
extern	int pvmnoresetindex;		/* task reset override mb index */

extern	char *pvmmytaskname;		/* task name */

extern	int pvmfrgsiz;				/* message frag length (to pack) */
extern	int pvmudpmtu;				/* local UDP MTU */

extern	int pvmmyctx;				/* current message context */
extern	int pvmnsibs;				/* number in our spawn group */
extern	int *pvmsibtids;			/* tids in our spawn group */

extern	struct pmsg *pvmrxlist;		/* not-recvd msg list */

extern	struct pmsg *pvmrbuf;		/* current recv/unpack message */
extern	struct pmsg *pvmsbuf;		/* current send/pack message */


#if defined(IMA_ALPHAMP) || defined(IMA_CSPP) || defined(IMA_HPPAMP) \
	|| defined(IMA_RS6KMP) || defined(IMA_SGIMP) \
	|| defined(IMA_SGIMP6) || defined(IMA_AIX4MP) \
	|| defined(IMA_AIX5MP) || defined(IMA_SGIMP64) \
	|| defined(IMA_SUNMP)
extern	 int pvmpolltype;			/* memory polling style */
extern	 int pvmpolltime;			/* time value for poll wait */
#endif

#ifdef LOG
extern	FILE *logfp;				/* my own log file */
#endif


/* Trace Globals Declarations */

extern	struct pvmtrcencvec *pvmtrccodef;

extern	struct timeval pvmtrcztv;

extern	struct pmsg *pvmtrcmp;

extern	int pvmtrcsbfsave;
extern	int pvmtrcsbf;

extern	int pvmtrcdesc;
extern	int pvmtrctmp;

extern	int pvmtrcsavekind;


/* Routines */

int	pvm_tc_noop				__ProtoGlarp__(( int ));
int	pvm_tc_settrace			__ProtoGlarp__(( int ));
int	pvm_tc_settrcbuf		__ProtoGlarp__(( int ));
int	pvm_tc_settrcopt		__ProtoGlarp__(( int ));
int	pvm_tc_settmask			__ProtoGlarp__(( int ));
int	pvm_tc_siblings			__ProtoGlarp__(( int ));

int pvmupkstralloc			__ProtoGlarp__(( char ** ));


/*
*   task-task or task-pvmd control block
*/

struct ttpcb {
	struct ttpcb *tt_link;          /* dll of peers */
	struct ttpcb *tt_rlink;
	int tt_tid;                     /* tid of peer or 0 if master of list */
	int tt_state;
	int tt_fd;                      /* fd of connection or -1 */
	struct sockaddr_in tt_sad;      /* address of our socket */
	struct sockaddr_in tt_osad;     /* (authd) address of peer socket */
	struct pmsg *tt_rxfrag;         /* not-assembled incm msg */
	struct frag *tt_rxf;            /* partial incm frag */
#ifndef NOUNIXDOM
	char *tt_spath;                 /* socket path */
#endif
#if defined(IMA_MPP)			/* ordering structure for send/recv msgs */
	MPP_DIRECTI_PTR	mpdirect;
#endif	
};

/* ttpcb states */
#define TTCONWAIT   1           /* requested, waiting for reply to connect */
#define TTGRNWAIT   2           /* granted, waiting for connect */
#define TTOPEN      3           /* connection running */
#define TTDENY      4           /* don't attempt connection */
#define TTDEAD      5           /* task dead; pcb should be cleared */



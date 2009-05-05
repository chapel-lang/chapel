
/* $Id: pvmproto.h,v 1.18 2000/02/10 20:43:54 pvmsrc Exp $ */

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
 *	pvmproto.h
 *
 *	PVM protocol definitions.
 *
 * $Log: pvmproto.h,v $
 * Revision 1.18  2000/02/10 20:43:54  pvmsrc
 * Removed hard-coded /tmp in TDSOCKNAME / TDSOCKNAME_CSPP.
 * 	- use pvmgettmp() routine now to determine PVM temp dir.
 * (Spanker=kohl)
 *
 * Revision 1.17  1999/07/08 18:59:48  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.16  1998/01/12  21:12:01  pvmsrc
 * Added task output collection op defines.
 * 	- TO_NEW, TO_SPAWN, TO_EOF.
 * (Spanker=kohl)
 *
 * Revision 1.15  1997/10/22  22:08:16  pvmsrc
 * Added SYSCTX_DG constant for dynamic group context.
 * (Spanker=kohl)
 *
 * Revision 1.14  1997/10/01  15:34:49  pvmsrc
 * Fixed TM_LAST decl.
 * 	- should be same as last entry, not last + 1...
 * (Spanker=kohl)
 *
 * Revision 1.13  1997/07/09  13:29:37  pvmsrc
 * Fixed Author Header.
 *
 * Revision 1.12  1997/06/10  16:38:55  pvmsrc
 * Minor text cleanup.
 *
 * Revision 1.11  1997/05/29  15:15:05  pvmsrc
 * Fixed decl of SM_FIRST:
 * 	- some compilers too stupid to know it's an (int)0x80040001...
 * 	- D-Oh.
 *
 * Revision 1.10  1997/05/27  14:40:58  pvmsrc
 * Added new TM message:
 * 	TM_SHMCONN. This allows socket and shmem task structs to be merged.
 * 	Also when exchanged sets new TF flag TF_SHMCONN.
 *
 * Revision 1.9  1997/04/09  14:37:17  pvmsrc
 * PVM patches from the base 3.3.10 to 3.3.11 versions where applicable.
 * Originals by Bob Manchek. Altered by Graham Fagg where required.
 *
 * Revision 1.8  1997/04/08  19:18:11  pvmsrc
 * Damn.  Added matching DM_RESETACK constant...  :-Q
 *
 * Revision 1.7  1997/04/08  19:05:51  pvmsrc
 * Added DM_RESET constant.
 * 	- for reset of system, cleanup of persistent message mboxes.
 *
 * Revision 1.6  1997/04/07  18:20:15  pvmsrc
 * Added TMDB_RESET constant.
 * 	- for resetting mbox db on pvmreset()...
 *
 * Revision 1.5  1997/04/04  15:45:03  pvmsrc
 * Typos CTXT -> CTX for consistency. Affects task.c, too
 *
 * Revision 1.4  1997/04/03  19:15:52  pvmsrc
 * Added some system defined contexts for use by TM, TC messages
 *
 * Revision 1.3  1997/04/03  16:12:38  pvmsrc
 * Added correct macro for sequence number wrap-around comparison.
 * This is on SEQLESSTHAN and is useful for 16 bit sequence numbers
 *
 * Revision 1.2  1997/01/28  19:14:22  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/10/25  13:43:38  pvmsrc
 * Initial revision
 *
 *
 */

#ifndef	_PVMPROTO_H_

#define	_PVMPROTO_H_

/* protocol version */

#ifndef	SDPROTOCOL
#define	SDPROTOCOL	4302
#endif

/*
*	t - task
*	d - pvmd
*	R - resource manager
*	H - hoster
*	T - tasker
*/

#define	SM_FIRST		(int)0x80040001	/* first SM_ message */
#define	SM_SPAWN		(SM_FIRST+0)	/* t<>R like TM_SPAWN */
#define	SM_EXEC			(SM_FIRST+1)	/* R->d like DM_EXEC */
#define	SM_EXECACK		(SM_FIRST+2)	/* d->R like DM_EXECACK */
#define	SM_TASK			(SM_FIRST+3)	/* t<>R like TM_TASK */
#define	SM_CONFIG		(SM_FIRST+4)	/* t<>R like TM_CONFIG */
#define	SM_ADDHOST		(SM_FIRST+5)	/* t<>R like TM_ADDHOST */
#define	SM_DELHOST		(SM_FIRST+6)	/* t<>R like TM_DELHOST */
#define	SM_ADD			(SM_FIRST+7)	/* R->d like DM_ADD */
#define	SM_ADDACK		(SM_FIRST+8)	/* d->R like DM_ADDACK */
#define	SM_NOTIFY		(SM_FIRST+9)	/* t->R like TM_NOTIFY */
#define	SM_TASKX		(SM_FIRST+10)	/* d->R notify of task exit */
#define	SM_HOSTX		(SM_FIRST+11)	/* d->R notify sched of host delete */
#define	SM_HANDOFF		(SM_FIRST+12)	/* R->d pvmd to new sched */
#define	SM_SCHED		(SM_FIRST+13)	/* t<>R like TM_SCHED */
#define	SM_STHOST		(SM_FIRST+14)	/* d->H start slave pvmds */
#define	SM_STHOSTACK	(SM_FIRST+15)	/* H->d like DM_STARTACK */
#define	SM_STTASK		(SM_FIRST+16)	/* d->T start task */
#define	SM_LAST			(SM_FIRST+16)	/* last SM_ message */


/*
 *	ddpro.h
 *
 *	Pvmd-pvmd protocol
 *
 * Revision 1.5  1996/05/13  20:24:06  manchek
 * [GEF pvm3.3.10to11 patch updates]
 * added PVMDISTASK
 *
 * Revision 1.4  1995/05/17  16:06:18  manchek
 * cast DM_FIRST to int.
 * added debugging classes
 *
 * Revision 1.3  1994/07/18  19:19:27  manchek
 * added PDMWAITC
 *
 * Revision 1.2  1994/06/03  20:38:13  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:47  manchek
 * Initial revision
 *
 */


/* protocol version */

#ifndef	DDPROTOCOL
#define	DDPROTOCOL	2316
#endif

#define	DDMINRETRIES	10
#define	DDMAXRTT		9		/* seconds */
#define	DDMINTIMEOUT	(DDMINRETRIES*DDMAXRTT*2)	/* seconds */
#define	DDERRRETRY		300000	/* uSec */
#define	DDPINGTIME		60		/* (sec) between keepalive pings */
#define	DDBAILTIME		300		/* (sec) for new slave to bail if no config */

#define	NEXTSEQNUM(x)		(((x)+1)&0xffff)
#define	SEQNUMCOMPARE(x,y)	((x)<(y)||(((x)>0x8000)&&((y)<0x8000)))
#define	SEQLESSTHAN(x,y)( ((x)<(y) && (y)-(x) <  32768) ||\
					((x)>(y) && (x)-(y) >= 32768) )


/*DocThis*/
/*
 * Pvmd-pvmd packet header
 *
 *    0               1               2               3
 *   +---------------------------------------------------------------+
 *  0|                              dst                              |
 *   +---------------------------------------------------------------+
 *  4|                              src                              |
 *   +-------------------------------+-------------------------------+
 *  8|             seq               |              ack              |
 *   +---------------+---------------+-------------------------------+
 * 12|     flags     |                                               |
 *   +---------------+-----------------------------------------------+
 */
/*EndDocThis*/

#define	DDFRAGHDR		16

/* frag flags */

#define	FFDAT			4		/* packet contains data, seq num is valid */
#define	FFFIN			8		/* sender closing connection */
#define	FFACK			16		/* ack num is valid */

/* d-d message flavors */

#define	DM_FIRST		(int)0x80020001	/* first DM_ message */
#define	DM_ADD			(DM_FIRST+0)	/* add hosts to machine */
#define	DM_ADDACK		(DM_FIRST+1)
#define	DM_EXEC			(DM_FIRST+2)	/* start tasks */
#define	DM_EXECACK		(DM_FIRST+3)
#define	DM_SENDSIG		(DM_FIRST+4)	/* signal a task */
#define	DM_HTUPD		(DM_FIRST+5)	/* define new host table */
#define	DM_HTUPDACK		(DM_FIRST+6)
#define	DM_HTCOMMIT		(DM_FIRST+7)	/* switch to new host table */
#define	DM_SLCONF		(DM_FIRST+8)	/* config slave pvmd */
#define	DM_STARTACK		(DM_FIRST+9)	/* pvmd' completed slave startup */
#define	DM_TASK			(DM_FIRST+10)	/* get list of tasks */
#define	DM_TASKACK		(DM_FIRST+11)
#define	DM_DELHOST		(DM_FIRST+12)	/* delete hosts from machine */
#define	DM_DELHOSTACK	(DM_FIRST+13)
#define	DM_NULL			(DM_FIRST+14)	/* null message */
#define	DM_TASKOUT		(DM_FIRST+15)	/* task standard out/err data */
#define	DM_PSTAT		(DM_FIRST+16)	/* query status of task */
#define	DM_PSTATACK		(DM_FIRST+17)
#define	DM_HALT			(DM_FIRST+18)	/* request master pvmd to halt */
#define	DM_MCA			(DM_FIRST+19)	/* define multicast tid */
#define	DM_NOTIFY		(DM_FIRST+20)	/* request notify */
#define	DM_NOTIFYACK	(DM_FIRST+21)
#define	DM_DB			(DM_FIRST+22)	/* message-mailbox request */
#define	DM_DBACK		(DM_FIRST+23)
#define	DM_RESET		(DM_FIRST+24)	/* reset linkage (mbox tidy) */
#define	DM_RESETACK		(DM_FIRST+25)
#define	DM_HTDEL		(DM_FIRST+26)	/* delete hosts from host table */
#define	DM_HOSTSYNC		(DM_FIRST+27)	/* request clock sample */
#define	DM_HOSTSYNCACK	(DM_FIRST+28)
#define	DM_LAST			(DM_FIRST+28)	/* last DM_ message */

/* DM_SLCONF fields - don't use 0 */

#define	DM_SLCONF_EP	1		/* set epath */
#define	DM_SLCONF_BP	2		/* set bpath */
#define	DM_SLCONF_WD	3		/* set wdir */
#define	DM_SLCONF_SCHED	4		/* set scheduler tid */
#define	DM_SLCONF_TRACE	5		/* set tracer info */

/* pvmd run states */

#define	PVMDNORMAL		1		/* normal running state */
#define	PVMDHTUPD		2		/* getting host table update */
#define	PVMDSTARTUP		3		/* slave just started, not fully configured */
#define	PVMDPRIME		4		/* am pvmd' */
#define	PVMDHALTING		5		/* stopping rsn */
#define	PVMDSHUTDOWN	6		/* in timeout to halt, waiting for FIN|ACK */
#define	PVMDISTASK		7		/* between fork and exec to spawn task */

/* used to cache multicast addresses */

struct mca {
	struct mca *mc_link;	/* dll of peers */
	struct mca *mc_rlink;
	int mc_tid;				/* the multicast tid */
	int mc_ndst;			/* number of dsts */
	int *mc_dsts;			/* dst tids (local tasks or foreign pvmds) */
};

struct mca *mca_new __ProtoGlarp__(( void ));
void mca_free __ProtoGlarp__((struct mca *mcap));
struct pmsg *mesg_new __ProtoGlarp__(( int master ));


/*
 *	tdpro.h
 *
 *	Task-pvmd protocol
 *
 * Revision 1.5  1995/06/28  15:32:29  manchek
 * added TC_SHMAT
 *
 * Revision 1.4  1995/05/17  16:56:06  manchek
 * added support for CSPP subcomplexes (/tmp file naming).
 * case TM_FIRST, TC_FIRST to int
 * add TM_GETOPT message
 *
 * Revision 1.3  1994/06/03  20:38:31  manchek
 * version 3.3.0
 *
 * Revision 1.2  1993/10/04  20:30:50  manchek
 * added alternate TDSOCKNAME if SHAREDTMP is defined
 *
 * Revision 1.1  1993/08/30  23:26:52  manchek
 * Initial revision
 *
 */


/* protocol version */

#ifndef	TDPROTOCOL
#define	TDPROTOCOL	1318
#endif

/*DocThis*/
/*
 * Task-task, task-pvmd fragment header
 *
 *    0               1               2               3
 *   +---------------------------------------------------------------+
 *  0|                              dst                              |
 *   +---------------------------------------------------------------+
 *  4|                              src                              |
 *   +---------------------------------------------------------------+
 *  8|                             length                            |
 *   +---------------+-----------------------------------------------+
 * 12|     flags     |                                               |
 *   +---------------+-----------------------------------------------+
 */
/*EndDocThis*/

#define	TDFRAGHDR	16

/*
* Maximum size of all headers in system
*/

#define	MAXHDR		((DDFRAGHDR > TDFRAGHDR ? DDFRAGHDR : TDFRAGHDR) \
					+ MSGHDRLEN)

/* file with task->pvmd sockaddr */

#ifdef	SHAREDTMP
#define	TDSOCKNAME	"%s/pvmd.%d.%s"
#else
#define	TDSOCKNAME	"%s/pvmd.%d"
#endif

#ifdef	IMA_CSPP
#ifdef	SHAREDTMP
#define	TDSOCKNAME_CSPP	"%s/pvmd.%d.%d.%s"
#else
#define	TDSOCKNAME_CSPP	"%s/pvmd.%d.%d"
#endif
#endif	/*IMA_CSPP*/

#define	TM_FIRST	(int)0x80010001	/* first TM_ message */
#define	TM_CONNECT	(TM_FIRST+0)	/* task connects */
#define	TM_CONN2	(TM_FIRST+1)	/* 2nd round of connect */
#define	TM_EXIT		(TM_FIRST+2)	/* task disconnects */
#define	TM_ADDHOST	(TM_FIRST+3)	/* add hosts to machine */
#define	TM_DELHOST	(TM_FIRST+4)	/* delete hosts from machine */
#define	TM_CONFIG	(TM_FIRST+5)	/* get machine config */
#define	TM_MSTAT	(TM_FIRST+6)	/* get host status */
#define	TM_HALT		(TM_FIRST+7)	/* halt machine */
#define	TM_TICKLE	(TM_FIRST+8)	/* tickle pvmd */
#define	TM_SPAWN	(TM_FIRST+9)	/* spawn new tasks */
#define	TM_PSTAT	(TM_FIRST+10)	/* get task status */
#define	TM_SENDSIG	(TM_FIRST+11)	/* signal a task */
#define	TM_TASK		(TM_FIRST+12)	/* list tasks */
#define	TM_MCA		(TM_FIRST+13)	/* request multicast tid */
#define	TM_NOTIFY	(TM_FIRST+14)	/* request notification */
#define	TM_DB		(TM_FIRST+15)	/* message-mailbox request */
#define	TM_SCHED	(TM_FIRST+16)	/* register task as scheduler */
#define	TM_TASKER	(TM_FIRST+17)	/* register task as task starter */
#define	TM_HOSTER	(TM_FIRST+18)	/* register task as pvmd starter */
#define	TM_TRACER	(TM_FIRST+19)	/* register task as task tracer */
#define	TM_HOSTSYNC	(TM_FIRST+20)	/* get clock delta to host */
#define	TM_SETOPT	(TM_FIRST+21)	/* set task parameters */
#define	TM_GETOPT	(TM_FIRST+22)	/* get all task parameters */
#define	TM_CONTEXT	(TM_FIRST+23)	/* allocate or free a context id */
#define	TM_SHMCONN	(TM_FIRST+24)	/* shmem task connect */
#define	TM_LAST		(TM_FIRST+24)	/* last TM_ message */

/*
* for TM_DB
*/

#define	TMDB_PUT	1			/* insert message */
#define	TMDB_REMOVE	2			/* delete entry */
#define	TMDB_GET	3			/* retrieve entry */
#define	TMDB_NAMES	4			/* get list of all names defined */
#define	TMDB_RESET	5			/* wipe out mbox db entries */

/*
* for TM_SETOPT
*/

#define	TS_OUTTID	1				/* stdout dst */
#define	TS_OUTCTX	2				/* stdout message context */
#define	TS_OUTTAG	3				/* stdout message tag */
#define	TS_TRCTID	4				/* trace dst */
#define	TS_TRCCTX	5				/* trace message context */
#define	TS_TRCTAG	6				/* trace message tag */

#define	TC_FIRST		(int)0x80030001	/* first TC_ message */
#define	TC_CONREQ		(TC_FIRST+0)	/* connection request */
#define	TC_CONACK		(TC_FIRST+1)	/* connection request ack */
#define	TC_TASKEXIT		(TC_FIRST+2)	/* task exited/doesn't exist */
#define	TC_NOOP			(TC_FIRST+3)	/* do nothing */
#define	TC_OUTPUT		(TC_FIRST+4)	/* child stdout data */
#define	TC_SETTRACE		(TC_FIRST+5)	/* set task tracing */
#define	TC_SETTMASK		(TC_FIRST+6)	/* set task trace mask */
#define	TC_SETTRCBUF	(TC_FIRST+7)	/* set task trace buffer */
#define	TC_SETTRCOPT	(TC_FIRST+8)	/* set task trace options */
#define	TC_SHMAT		(TC_FIRST+9)	/* shmem connect handshake */
#define	TC_OUTPUTX		(TC_FIRST+10)	/* (not sent) flush out coll */
#define	TC_SIBLINGS		(TC_FIRST+11)	/* parent announce spawn grp */
#define	TC_LAST			(TC_FIRST+11)	/* last TC_ message */

/*
* for Task Output Ops
*/

#define TO_EOF		 0
#define TO_SPAWN	-1
#define TO_NEW		-2


/* Contexts Used by Some System Calls - these are allocated from the
   top of the context space */

#define SYSCTX_FIRST	(int) 0x0007ffff	/* first system context */
#define SYSCTX_TC		(SYSCTX_FIRST-0)	/* ctxt for TC messages */
#define SYSCTX_TM		(SYSCTX_FIRST-1)	/* ctxt for TM messages */
#define SYSCTX_SG		(SYSCTX_FIRST-2) 	/* ctxt for static group formation*/
#define SYSCTX_DG		(SYSCTX_FIRST-3) 	/* ctxt for dynamic groups */
#define SYSCTX_RS4		(SYSCTX_FIRST-4)	/* Reserved system context */
#define SYSCTX_RS5		(SYSCTX_FIRST-5)	/* Reserved system context */
#define SYSCTX_RS6		(SYSCTX_FIRST-6)	/* Reserved system context */
#define SYSCTX_RS7		(SYSCTX_FIRST-7)	/* Reserved system context */
#define SYSCTX_RS8		(SYSCTX_FIRST-8)	/* Reserved system context */
#define SYSCTX_RS9		(SYSCTX_FIRST-9)	/* Reserved system context */
#define SYSCTX_LAST		(SYSCTX_FIRST-9)
#define NUM_SYSCTX		(SYSCTX_FIRST-SYSCTX_LAST+1)			

#define BASECONTEXT		(int) 0

#endif	/*_PVMPROTO_H_*/


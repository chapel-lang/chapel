
static char rcsid[] =
	"$Id: task.c,v 1.25 2002/04/16 15:04:50 pvmsrc Exp $";

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
 *	task.c
 *
 *	Task descriptors.
 *
 * $Log: task.c,v $
 * Revision 1.25  2002/04/16 15:04:50  pvmsrc
 * Fixed bug in task_cleanup():
 * 	- & vs && in scheduler notification code...  :-Q
 * 	- bug report submitted by Yan Xiao <yxiao@BlackstoneComputing.com>.
 * (Spanker=kohl)
 *
 * Revision 1.24  2002/02/21 23:18:07  pvmsrc
 * Added new (not to be documented!) PVM_MAX_TASKS env var support.
 * 	- for Mahir Lokvancic <mahir@math.ufl.edu>.
 * 	- forcefully limits the number of tasks that can attach to a
 * 		pvmd, required on Solaris in rare circumstances when hard
 * 		FD_SETSIZE limit is reached, and all hell breaks loose...
 * 	- check pvm_max_ntasks global when allocating new tasks
 * 		(0 = no limit)
 * 	- in task_new() call, if limit reached return NULL ptr,
 * 		indicating PvmOutOfRes...
 * 	- keep track of private "ntasks" counter in task_new()/task_free(),
 * 		only increment for non-zero tids (avoid counting temp task
 * 		structs...).
 * (Spanker=kohl)
 *
 * Revision 1.23  2001/02/07 23:15:54  pvmsrc
 * 2nd Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.22  2000/09/01 14:39:22  pvmsrc
 * Plugged memory (waitc) leak in task_cleanup():
 * 	- better get rid of any notify wait contexts, if the task is
 * 		dead we probably don't care any more...
 * 	- added wait_delete() for WT_HOSTA, WT_TASKX, WT_ROUTEA, WT_ROUTED.
 * 	- I hope this doesn't break any shit...  :-o
 * (Spanker=kohl)
 *
 * Revision 1.21  2000/02/17 23:12:19  pvmsrc
 * *** Changes for new BEOLIN port ***
 * 	- MPP-like, similar to SP2, etc.
 * 	- submitted by Paul Springer <pls@smokeymt.jpl.nasa.gov>.
 * 	- format-checked & cleaned up by Jeembo...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.20  2000/02/16 22:00:30  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.19  1999/07/08 19:00:15  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.18  1998/11/20  20:06:47  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (spanker=sscott)
 *
 * Revision 1.17  1998/10/02  15:44:10  pvmsrc
 * Single source code merge of Win32 and Unix code.
 * (Spanker=sscott)
 *
 * Revision 1.16  1998/01/12  21:13:30  pvmsrc
 * Replaced inline constants with new task output op defines.
 * 	- TO_NEW == -2.
 * 	- TO_SPAWN == -1.
 * 	- TO_EOF == 0.
 * (Spanker=kohl)
 *
 * Revision 1.15  1997/12/01  22:17:43  pvmsrc
 * Fixed tracer registry problem.
 * 	- in task_cleanup(), if exited task was the tracer, forward a
 * 		DM_SLCONF_TRACE message to all hosts to clear tracer setting.
 * (Spanker=kohl)
 *
 * Revision 1.14  1997/06/27  20:05:27  pvmsrc
 * Integrated WIN32 changes.
 *
 * Revision 1.13  1997/06/27  19:22:08  pvmsrc
 * Task struct updated to hold message state info about its last message
 * 	to the RM.
 * Avoids duplicate messages and helps migrated/new RMs understand task
 * 	state.
 *
 * Revision 1.12  1997/06/02  13:50:05  pvmsrc
 * Added missing #include host.h for waitc.h.
 *
 * Revision 1.11  1997/05/12  20:28:20  pvmsrc
 * Removed duplicate #includes...
 *
 * Revision 1.10  1997/04/30  21:26:29  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.9  1997/04/10  17:52:35  pvmsrc
 * Added WT_RECVINFO case to task_cleanup().
 * 	- if task died while waiting for a reply from pvm_recvinfo(),
 * 		wipe out the wait context.
 *
 * Revision 1.8  1997/04/08  19:42:57  pvmsrc
 * *** Added new system reset protocol / wait linkage.
 * 	- new DM_RESET / dm_reset() & DM_RESETACK / dm_resetack().
 * 	- new WT_RESET wait type.
 * 	- modified dm_db() to include new TMDB_RESET(nnr, noresets) option:
 * 		* clean up mboxes, except for "no-reset" tasks.
 * 		* for persistent mboxes, set up WT_RESET to remove mbox on task
 * 			exit, propagate to task's host via DM_RESET.
 * 		* on task exit, WT_RESET wipes mbox out, DM_RESETACK passes
 * 			word on to master pvmd (if necessary).
 *
 * Revision 1.7  1997/04/04  15:45:39  pvmsrc
 * Take into account the number of system contexts (NUM_SYSCTX)
 * 	that are preallocated.
 *
 * Revision 1.6  1997/02/13  19:05:07  pvmsrc
 * Fixed mbox cleanup problem:
 * 	- in dm_db() for TMDB_PUT case, if successful create master PVMD
 * 		notify on inserting task (if task not local, forward DM_NOTIFY).
 * 	- then on task exit, call mb_tidy() if WT_TASKX notify wait context
 * 		exists (in hostfailentry() and task_cleanup()), or if empty
 * 		notify propagates back to master PVMD via DM_NOTIFYACK.
 *
 * Revision 1.5  1997/02/13  15:10:04  pvmsrc
 * Removed unnecessary extern for struct waitc *waitlist.
 * 	- now in global.h.
 *
 * Revision 1.4  1997/01/28  19:27:32  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/25  13:58:07  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.2  1996/10/24  22:10:04  pvmsrc
 * Moved #include "global.h" below other #include's for typing.
 * Added #include <pvmtev.h> for tracing constants.
 * Added extern struct Pvmtracer pvmtracer for tracer info.
 * Modified checking of trctid / outtid:
 * 	- check for > 0, not non-zero, to handle new case where task
 * 		denies external collection.
 * Replaced inline code for pvmd trace events with new routine calls:
 * 	- tev_send_endtask().
 * On task exit, check for tracer registration -> clear tracer info...
 *
 * Revision 1.1  1996/09/23  23:44:43  pvmsrc
 * Initial revision
 *
 * Revision 1.8  1995/09/05  19:26:36  manchek
 * clear wait WT_HOSTF in task_cleanup
 *
 * Revision 1.7  1995/05/17  16:53:35  manchek
 * don't need mytid anymore
 *
 * Revision 1.6  1995/02/06  18:45:45  manchek
 * added stuff to task_dump
 *
 * Revision 1.5  1994/11/08  15:40:59  manchek
 * shared memory cleanup.
 * check if wa_tid is zero before sending message in task_cleanup
 *
 * Revision 1.4  1994/10/15  19:32:49  manchek
 * added log in task_free()
 *
 * Revision 1.3  1994/06/03  20:38:28  manchek
 * version 3.3.0
 *
 * Revision 1.2  1993/11/30  15:55:25  manchek
 * task_free() deletes auth file if one exists
 *
 * Revision 1.1  1993/08/30  23:26:51  manchek
 * Initial revision
 *
 */

#ifdef IMA_TITN
#include <bsd/sys/types.h>
#else
#include <sys/types.h>
#endif

#ifdef NEEDMENDIAN
#include <machine/endian.h>
#endif
#ifdef NEEDENDIAN
#include <endian.h>
#endif
#ifdef NEEDSENDIAN
#include <sys/endian.h>
#endif

#include <pvm3.h>

#if defined(WIN32) || defined(CYGWIN)
#include "..\xdr\types.h"
#include "..\xdr\xdr.h"
#else
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

#ifdef WIN32
#include "pvmwin.h"
#include <time.h>
#else
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#include <pvmproto.h>
#include "pmsg.h"
#include "pkt.h"
#include "task.h"
#include "host.h"
#include "waitc.h"
#include "pvmalloc.h"
#include "listmac.h"
#include "bfunc.h"
#include <pvmtev.h>
#include "msgbox.h"
#include "global.h"


/***************
 **  Globals  **
 **           **
 ***************/

extern void pvmbailout();

extern int busyadding;				/* from ddpro.c */
extern int pvmdebmask;				/* from pvmd.c */
extern int hostertid;				/* from pvmd.c */
extern struct htab *hosts;			/* from pvmd.c */
extern int myhostpart;				/* from pvmd.c */
extern int taskertid;				/* from pvmd.c */
extern struct Pvmtracer pvmtracer;	/* from pvmd.c */
extern int tidlmask;				/* from pvmd.c */
extern int pvm_max_ntasks;			/* from pvmd.c */

struct task *locltasks = 0;			/* all task structs sorted by tid */

void task_dump();
void tev_send_endtask();

/***************
 **  Private  **
 **           **
 ***************/

static struct ccon *cconlist = 0;

static int ntasks = 0;	/* tot # of tasks registered w/this pvmd */


/*****************
 **  Interface  **
 **             **
 *****************/

/*	task_init()
*
*	Call once before any other task stuff.
*/

void
task_init()
{
	if (!locltasks) {
		locltasks = TALLOC(1, struct task, "task");
		BZERO((char*)locltasks, sizeof(struct task));
		locltasks->t_link = locltasks;
		locltasks->t_rlink = locltasks;
		locltasks->t_plink = locltasks;
		locltasks->t_prlink = locltasks;

		cconlist = TALLOC(1, struct ccon, "ccon");
		BZERO((char*)cconlist, sizeof(struct ccon));
		cconlist->c_link = cconlist->c_rlink = cconlist;
	}
}


/*	ccon_new()
*
*	Allocate a new unique context id to a task.
*/

struct ccon *
ccon_new(tp)
	struct task *tp;
{
	static int lastcid = 0;

	int startcid;
	int cid;
	struct ccon *cp, *cp2;

	if (++lastcid > tidlmask - NUM_SYSCTX)
		lastcid = 1;
	startcid = lastcid;
	cp = cconlist;

	while (1) {
		cid = myhostpart + lastcid;
		while (cp->c_cid < cid)
			if ((cp = cp->c_link) == cconlist)
				break;

		if (cp->c_cid != cid)
			break;

		if (++lastcid > tidlmask) {
			lastcid = 1;
			cp = cconlist;
		}
		if (lastcid == startcid) {
			pvmlogprintf("ccon_new() out of cids?\n");
			return 0;
		}
	}

	if (!(cp2 = TALLOC(1, struct ccon, "ccon"))) {
		pvmlogprintf("ccon_new() can't get memory\n");
		return 0;
	}
	cp2->c_cid = cid;
	cp2->c_tid = tp->t_tid;
	LISTPUTBEFORE(tp->t_ccs, cp2, c_peer, c_rpeer);
	LISTPUTBEFORE(cp, cp2, c_link, c_rlink);
	if (pvmdebmask & PDMTASK) {
		pvmlogprintf("ccon_new() tid=%x cid=%x\n", cp2->c_tid, cp2->c_cid);
	}
	return cp2;
}


/*	tid_new()
*
*	Generates a task id not already in use.
*
*	XXX Be sure to call task_new() with tid before calling
*	tid_new() again.
*/

int
tid_new()
{
	static int lastind = 0;		/* last local-part assigned */

	int startind;				/* to detect when we're out of tids */
	struct task *tp;
	int tid;

	if (++lastind > tidlmask)
		lastind = 1;
	startind = lastind;
	tp = locltasks;

	while (1) {
		tid = myhostpart + lastind;
		while (tp->t_tid < tid)
			if ((tp = tp->t_link) == locltasks)
				break;

		if (tp->t_tid != tid)
			return tid;

		if (++lastind > tidlmask) {
			lastind = 1;
			tp = locltasks;
		}
		if (lastind == startind)
			return -1;
	}
}


#ifdef WIN32
void
task_sethandle(tp,t_handle)
	struct task *tp;
	int t_handle;
{
	tp->t_handle = t_handle;
}
#endif


/*	task_new()
*
*	Make a new task descriptor, add to list of local tasks but not
*	to by-pid list.
*/

struct task *
task_new(tid)
	int tid;
{
	struct task *tp, *tp2;

	if (tid) {
		ntasks++;
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("task_new(%d) ntasks=%d pvm_max_ntasks=%d\n",
				tid, ntasks, pvm_max_ntasks );
		}
		if (pvm_max_ntasks && ntasks > pvm_max_ntasks) {
			pvmlogprintf("task_new() too many tasks?\n");
			ntasks--;  /* this one didn't make it... :-) */
			return( (struct task *) NULL );
		}
	}

	if (!(tp = TALLOC(1, struct task, "task"))) {
		pvmlogprintf("task_new() can't get memory\n");
		pvmbailout(0);
	}
	BZERO((char*)tp, sizeof(struct task));
	tp->t_tid = tid;
	tp->t_txq = pk_new(0);
	tp->t_sock = -1;
	tp->t_out = -1;
	tp->t_authfd = -1;
	tp->t_schedlmsg = 0;
	tp->t_ccs = TALLOC(1, struct ccon, "ccon");
    tp->t_ccs->c_link = tp->t_ccs->c_rlink = 0;
	tp->t_ccs->c_peer = tp->t_ccs->c_rpeer = tp->t_ccs;
    tp->t_ccs->c_cid = 0;
    tp->t_ccs->c_tid = tid;
	FORLIST (tp2, locltasks, t_link)
		if (tp2->t_tid > tid)
			break;
	LISTPUTBEFORE(tp2, tp, t_link, t_rlink);
	return tp;
}


/*	task_free()
*
*	Do low-level cleanup needed when a task exits.
*	Remove task descriptor from lists and destroy it.
*	Close any fds, unlink any files, free mbufs.
*/

void
task_free(tp)
	struct task *tp;
{
	struct timeval now;
	struct pmsg *mp;
	struct ccon *cp;

	if (pvmdebmask & PDMTASK) {
		pvmlogprintf("task_free() t%x\n", tp->t_tid);
	}
#ifdef SHMEM
	/* XXX this is inside out - mpp_free should call task_free.
	   XXX but for now task_free is what's called.
	   XXX this will change in the portable processor interface
	   XXX cleanup. */
	mpp_free(tp->t_tid);
#endif
#ifdef IMA_BEOLIN
	mpp_free(tp);
#endif
	if (tp->t_plink && tp->t_prlink) {
		LISTDELETE(tp, t_plink, t_prlink);
	}
	if (tp->t_link && tp->t_rlink) {
		LISTDELETE(tp, t_link, t_rlink);
	}
	if (tp->t_rxm)
		pmsg_unref(tp->t_rxm);
	if (tp->t_rxp)
		pk_free(tp->t_rxp);
	if (tp->t_txq)
		pk_free(tp->t_txq);
	if (tp->t_wait)
		wait_delete(tp->t_wait);
	if (tp->t_authnam) {
		(void)unlink(tp->t_authnam);
		PVM_FREE(tp->t_authnam);
	}
	if (tp->t_sock != -1) {
		wrk_fds_delete(tp->t_sock, 3);
#ifdef WIN32
		closesocket(tp->t_sock);
#else
		(void)close(tp->t_sock);
#endif
	}
	if (tp->t_out != -1) {
		wrk_fds_delete(tp->t_out, 1);
#ifdef WIN32
		closesocket(tp->t_out);
#else
		(void)close(tp->t_out);
#endif
	}
	if (tp->t_outtid > 0) {
		mp = mesg_new(0);
		mp->m_dst = tp->t_outtid;
		mp->m_ctx = tp->t_outctx;
		mp->m_tag = tp->t_outtag;
		pkint(mp, tp->t_tid);
		pkint(mp, TO_EOF);
		sendmessage(mp);
		tp->t_outtid = 0;
	}
	if (tp->t_trctid > 0) {
		tev_send_endtask( tp->t_trctid, tp->t_trcctx, tp->t_trctag,
			tp->t_tid, tp->t_status,
			tp->t_utime.tv_sec, tp->t_utime.tv_usec,
			tp->t_stime.tv_sec, tp->t_stime.tv_usec );
		tp->t_trctid = 0;
	}
	if (tp->t_authfd != -1)
		(void)close(tp->t_authfd);
	if (tp->t_a_out)
		PVM_FREE(tp->t_a_out);
	if (tp->t_mca)
		mca_free(tp->t_mca);
	if (tp->t_ccs) {
		while (cp = LISTFIRST(tp->t_ccs, c_peer)) {
			LISTDELETE(cp, c_link, c_rlink);
			LISTDELETE(cp, c_peer, c_rpeer);
			PVM_FREE(cp);
		}
		PVM_FREE(tp->t_ccs);
	}

	if (tp->t_tid) {
		ntasks--;
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("task_free(%d) ntasks=%d\n",
					tp->t_tid, ntasks );
		}
	}

	PVM_FREE(tp);
}


/*	task_find()
*
*	Find a task in local tasks list by its tid.
*/

struct task *
task_find(tid)
	int tid;
{
	struct task *tp;

	if (tid) {
		tp = locltasks->t_link;
		while (tp != locltasks && tp->t_tid < tid)
			tp = tp->t_link;
		if (tp->t_tid == tid)
			return tp;
	}
	return (struct task*)0;
}


/*	task_findpid()
*
*	Find a task in local tasks list by its pid.
*/

struct task *
task_findpid(pid)
	int pid;
{
	struct task *tp;

	tp = locltasks->t_plink;
	while (tp != locltasks && tp->t_pid < pid)
		tp = tp->t_plink;
	return (tp->t_pid == pid) ? tp : (struct task*)0;
}


/*	task_setpid()
*
*	Set the pid for a task, insert it into by-pid list.  Move it
*	if it's already in the list.
*/

void
task_setpid(tp, pid)
	struct task *tp;
	int pid;
{
	struct task *tp2;

	if (tp->t_plink) {
		LISTDELETE(tp, t_plink, t_prlink);
	}
	tp->t_pid = pid;
	for (tp2 = locltasks->t_plink; tp2 != locltasks; tp2 = tp2->t_plink)
		if (tp2->t_pid > pid)
			break;
	LISTPUTBEFORE(tp2, tp, t_plink, t_prlink);
}


/*	task_cleanup()
*
*	Do high-level cleanup needed when a task exits.
*	Wake up any entities waiting on task, free multicast context.
*	XXX should flush any partial messages, but that would be hard.  hm.
*/

void
task_cleanup(tp)
	struct task *tp;
{
	struct pmsg *mp;
	struct waitc *wp, *wp2;
	struct task *tp2;
	struct pvmmentry *ep;
	struct hostd *hp;
	struct pmsg *mpd;
	char buf[512];
	int hh;

	/* notify anyone who asked */

	if (tp->t_tid) {
		if (pvmdebmask & PDMTASK)
			pvmlogprintf("task_cleanup() t%x\n", tp->t_tid);

		for (wp = waitlist->wa_link; wp != waitlist; wp = wp->wa_link) {

	/* waits depending on this task */

			if (wp->wa_on == tp->t_tid) {
				switch (wp->wa_kind) {

				case WT_HOSTSTART:
					busyadding = 0;
					free_waitc_add((struct waitc_add *)wp->wa_spec);
					pkint(wp->wa_mesg, PvmDSysErr);
					sendmessage(wp->wa_mesg);
					wp->wa_mesg = 0;
					if (pvmdebmask & (PDMTASK|PDMSTARTUP))
						pvmlogprintf(
							"task_cleanup() hoster t%x takes wid %d with it\n",
							tp->t_tid, wp->wa_wid);
					break;

				case WT_TASKSTART:
					if (wp->wa_tid) {
						if (pvmdebmask & PDMTASK) {
							pvmlogprintf(
								"task_cleanup() tasker t%x takes t%x with it\n",
								tp->t_tid, wp->wa_tid);
						}
						if (tp2 = task_find(wp->wa_tid)) {
							wp->wa_tid = 0;
							task_cleanup(tp2);
							task_free(tp2);
						}
					}
					break;

				case WT_TASKX:
					if (wp->wa_tid && wp->wa_mesg) {
						sendmessage(wp->wa_mesg);
						wp->wa_mesg = 0;
					}
					mb_tidy(tp->t_tid);
					break;

				case WT_RESET:
					if (wp->wa_tid && wp->wa_mesg) {
						sendmessage(wp->wa_mesg);
						wp->wa_mesg = 0;
					}
					mb_tidy_reset(tp->t_tid);
					break;

				case WT_RECVINFO:
					/* clean up pending recvinfo */
					ep = (struct pvmmentry *) wp->wa_spec;
					if ( ep->me_msg )	/* class name (overload :-Q) */
						PVM_FREE( ep->me_msg );
					PVM_FREE( ep );
					break;

				case WT_HOSTA:
					break;

				default:
					pvmlogprintf(
							"task_cleanup() can't deal with wait kind %d\n",
							wp->wa_kind);
					break;
				}
				wp2 = wp;
				wp = wp->wa_rlink;
				wait_delete(wp2);
				continue;
			}

	/* waits this task was waiting on */

			if (wp->wa_tid == tp->t_tid) {
				switch (wp->wa_kind) {

				case WT_HOSTF:
				case WT_HOSTA:
				case WT_TASKX:
				case WT_ROUTEA:
				case WT_ROUTED:
				case WT_TASKSTART:
					wp2 = wp;				/* some kinds we can toss now */
					wp = wp->wa_rlink;
					wait_delete(wp2);
					break;

				default:
					wp->wa_tid = 0;			/* in case tid gets recycled */
					break;
				}
			}
		}

		/* notify the scheduler */

		if ((tp->t_sched)&&(tp->t_schedlmsg!=SM_TASKX)) {
			mp = mesg_new(0);
			mp->m_dst = tp->t_sched;
			mp->m_tag = SM_TASKX;
			tp->t_schedlmsg = SM_TASKX;
			if (pvmdebmask & PDMSCHED) {
				pvmlogprintf("task_cleanup() taskx to t%x status = 0x%x\n",
						tp->t_sched, tp->t_status);
			}
			pkint(mp, tp->t_tid);
			pkint(mp, tp->t_status);
			pkint(mp, (int)tp->t_utime.tv_sec);
			pkint(mp, (int)tp->t_utime.tv_usec);
			pkint(mp, (int)tp->t_stime.tv_sec);
			pkint(mp, (int)tp->t_stime.tv_usec);
			sendmessage(mp);
		}

		/* check if it's the hoster */

		if (tp->t_tid == hostertid) {
			if (pvmdebmask & (PDMTASK|PDMSTARTUP)) {
				pvmlogprintf("task_cleanup() unreg hoster t%x\n", tp->t_tid);
			}
			hostertid = 0;
		}

		/* check if it's the tasker */

		if (tp->t_tid == taskertid) {
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("task_cleanup() unreg tasker t%x\n", tp->t_tid);
			}
			taskertid = 0;
		}

		/* check if it's the tracer */

		if (tp->t_tid == pvmtracer.trctid) {

			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("task_cleanup() unreg tracer t%x\n",
						tp->t_tid);
			}

			pvmtracer.trctid = 0;
			pvmtracer.trcctx = 0;
			pvmtracer.trctag = 0;
			pvmtracer.outtid = 0;
			pvmtracer.outctx = 0;
			pvmtracer.outtag = 0;
			TEV_MASK_INIT(pvmtracer.tmask);
			pvmtracer.trcbuf = 0;
			pvmtracer.trcopt = 0;

			/* tell the other pvmds */

			for (hh = hosts->ht_last; hh > 0; hh--) {
				if (hh != hosts->ht_local
						&& (hp = hosts->ht_hosts[hh])) {
					mpd = mesg_new(0);
					mpd->m_tag = DM_SLCONF;
					mpd->m_dst = hp->hd_hostpart | TIDPVMD;
					pkint(mpd, DM_SLCONF_TRACE);
					sprintf(buf, "%x %d %d %x %d %d %d %d %s",
						pvmtracer.trctid, pvmtracer.trcctx,
							pvmtracer.trctag,
						pvmtracer.outtid, pvmtracer.outctx,
							pvmtracer.outtag,
						pvmtracer.trcbuf, pvmtracer.trcopt,
						pvmtracer.tmask);
					pkstr(mpd, buf);
					sendmessage(mpd);
				}
			}
		}
	}

	/* complete multicast */

	if (tp->t_mca) {
/* XXX should send an EOM frag to all rcpts */
		mca_free(tp->t_mca);
		tp->t_mca = 0;
	}
}


void
task_dump()
{
	struct task *tp;
	struct pkt *pp;
	struct ccon *cp;

	pvmlogprintf("task_dump()\n");
	pvmlogprintf("     tid     ptid flag    pid soc out     wait   outtid   trctid    sched   es\n");
	for (tp = locltasks->t_link; tp != locltasks; tp = tp->t_link) {
		pvmlogprintf("%8x %8x %4x %6d %3d %3d %8d %8x %8x %8x %4x\n",
				tp->t_tid,
				tp->t_ptid,
				tp->t_flag,
				tp->t_pid,
				tp->t_sock,
				tp->t_out,
				(tp->t_wait ? tp->t_wait->wa_wid : 0),
				tp->t_outtid,
				tp->t_trctid,
				tp->t_sched,
				tp->t_status);
		if (LISTFIRST(tp->t_txq, pk_link)) {
			pvmlogprintf(" txq:pkt      src      dst flag    len    ofs\n");
			FORLIST (pp, tp->t_txq, pk_link) {
				pvmlogprintf("%08x %8x %8x %4x %6d %6d\n",
						pp,
						pp->pk_src,
						pp->pk_dst,
						pp->pk_flag,
						pp->pk_len,
						pp->pk_dat - pp->pk_buf);
			}
		}
		if (LISTFIRST(tp->t_ccs, c_peer)) {
			pvmlogprintf("contexts:");
			FORLIST (cp, tp->t_ccs, c_peer)
				pvmlogprintf(" 0x%x", cp->c_cid);
			pvmlogprintf("\n");
		}
	}
}


int
ccon_dumpall()
{
	struct ccon *cp;

	pvmlogprintf("ccon_dumpall()\n");
	FORLIST (cp, cconlist, c_link)
		pvmlogprintf(" tid=%x cid=%x\n", cp->c_tid, cp->c_cid);
	return 0;
}



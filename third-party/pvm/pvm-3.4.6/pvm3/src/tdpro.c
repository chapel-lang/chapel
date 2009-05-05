
static char rcsid[] =
	"$Id: tdpro.c,v 1.37 2009/01/30 21:22:58 pvmsrc Exp $";

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
 *	tdpro.c
 *
 *	Entry points for messages from local tasks.
 *
 * $Log: tdpro.c,v $
 * Revision 1.37  2009/01/30 21:22:58  pvmsrc
 * Fixed up insecure usage of fopen() for creating/writing/appending files.
 * 	- replace with open(...O_EXCL...) and fdopen()...
 * Make sure to always return a failure response to connect-attempting tasks.
 * 	- especially if they fail...  :-}
 * (Spanker=kohl)
 *
 * Revision 1.36  2009/01/23 00:09:34  pvmsrc
 * Updated BEOLIN changes from Paul Springer...
 * (Spanker=kohl)
 *
 * Revision 1.35  2007/07/09 19:59:55  pvmsrc
 * Added missing "char *pvmnametag();" function decl.
 * 	- submitted by David Mathog <mathog@caltech.edu>.
 * (Spanker=kohl)
 *
 * Revision 1.34  2002/02/21 23:18:42  pvmsrc
 * Added new (not to be documented!) PVM_MAX_TASKS env var support.
 * 	- for Mahir Lokvancic <mahir@math.ufl.edu>.
 * 	- forcefully limits the number of tasks that can attach to a
 * 		pvmd, required on Solaris in rare circumstances when hard
 * 		FD_SETSIZE limit is reached, and all hell breaks loose...
 * 	- check return for task_new() call, can now produce NULL ptr,
 * 		indicating PvmOutOfRes...
 * (Spanker=kohl)
 *
 * Revision 1.33  2001/09/25 21:20:20  pvmsrc
 * Minor TMPNAMFUN()/tmpnam() cleanup.
 * 	- moved macro def to pvm3.h, renamed PVMTNPMAN().
 * 	- same for LEN_OF_TMP_NAM -> PVMTMPNAMLEN.
 * 	- mostly a huge waste of time, since *both* tmpnam() & mktemp()
 * 		produce the same "dangerous" warning message in Linux/gcc...
 * 	- damn.
 * (Spanker=kohl)
 *
 * Revision 1.32  2001/04/23 14:16:14  pvmsrc
 * Added new working directory option to pvm_spawn().
 * 	- use "where" argument to cram in working directory,
 * 		e.g. where = "msr.epm.ornl.gov:/home/user/project/bozo"
 * 	- TM_SPAWN strips out working directory & creates PVMSPAWNWD env var
 * 	- exectasks() (called by DM_EXEC or SM_EXEC) checks for env var
 * 		and does a chdir() (even uses getcwd() to reset directory :-).
 * 	- should not introduce any run-time incompatibility with older
 * 		PVM releases.
 * 	- PvmTaskHost or PvmTaskArch flags need not be used, i.e.
 * 		pvm_spawn( "foo", 0, PvmTaskDefault, ":/tmp", 1, &tid ) works.
 * (Spanker=kohl)
 *
 * Revision 1.31  2001/02/07 23:15:58  pvmsrc
 * 2nd Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.30  2000/02/17 23:12:20  pvmsrc
 * *** Changes for new BEOLIN port ***
 * 	- MPP-like, similar to SP2, etc.
 * 	- submitted by Paul Springer <pls@smokeymt.jpl.nasa.gov>.
 * 	- format-checked & cleaned up by Jeembo...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.29  2000/02/16 22:00:30  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.28  1999/07/08 19:00:16  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.27  1998/11/20  20:06:49  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (spanker=sscott)
 *
 * Revision 1.26  1998/01/12  21:13:31  pvmsrc
 * Replaced inline constants with new task output op defines.
 * 	- TO_NEW == -2.
 * 	- TO_SPAWN == -1.
 * 	- TO_EOF == 0.
 * (Spanker=kohl)
 *
 * Revision 1.25  1997/09/22  21:14:02  pvmsrc
 * Modified TM_CONN2/tm_conn2() linkage for shell-spanwed tasks.
 * 	- check for additional task name packed in message, save it in
 * 		task structure, use it for tev_newtask().
 * (Spanker=kohl)
 *
 * Revision 1.24  1997/09/18  14:14:44  pvmsrc
 * Fixed bug in tm_mca():
 * 	- loop to filter out bogus dests could hang, if a tid was not
 * 		local and on a failed host, index i would not get incremented,
 * 		resulting in an infinite loop...
 * 	- added second index to count dsts.
 * 	- submitted by Kamran Kazemi (kamran@cs.uwa.edu.au).
 * (Spanker=kohl)
 *
 * Revision 1.23  1997/08/29  13:35:20  pvmsrc
 * OS2 Port Submitted by Bohumir Horeni, horeni@login.cz.
 * (Spanker=kohl)
 *
 * Revision 1.22  1997/08/26  22:55:28  pvmsrc
 * Make sure wp->wa_mesg is cleared after sendmessage() call.
 * 	- otherwise wait_delete() will re-free the pmsg struct causing
 * 		major bogusness...
 * (Spanker=kohl)
 *
 * Revision 1.21  1997/08/11  14:40:13  pvmsrc
 * Attempt to remove CRAY compiler warning.
 * 	- if ( 1 ) got bail caused unreachables...
 * 	- replaced with "dummy_true" var set to 1, to trick compiler?!  :-)
 *
 * Revision 1.20  1997/07/02  20:27:35  pvmsrc
 * 	Fixed startup race on shmem to that a shmem task can get fully
 * 	configured before getting any messages.
 * 	This involved adding two states
 * 	TF_PRESHMCONN and TF_SHM.  TF_PRESHMCONN indicates that messages
 * 	with MM_PRIO set can be sent to a task, but regular messages are
 * 	queued. This allows shmem tasks to be completely configured
 * 	before any messages flow.  When the daemon changes the state from
 * 	TF_PRESHMCONN to TF_SHMCONN it calls shm_wrt_pkts to write any
 * 	packets that were queued before task state changed to TF_SHMCONN.
 *
 * Revision 1.19  1997/06/27  17:32:56  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.18  1997/06/27  16:10:01  pvmsrc
 * 		Spawning tasks with option PvmNoSpawnParent get the
 * 		parent tid set to PvmParentNotSet in spawned tasks.
 *
 * Revision 1.17  1997/06/24  20:39:22  pvmsrc
 * Eliminated unnecessary global externs.
 *
 * Revision 1.16  1997/06/23  21:26:46  pvmsrc
 * Added checking for master host in tm_hoster().
 * 	- actually check hd_hostpart instead of ht_local/ht_master indices,
 * 		in case freaking host table has copies/duplicates.
 * 		(always wear 2 condoms...  :-)
 * 	- return new PvmHostrNMstr error code.
 *
 * Revision 1.15  1997/06/16  13:41:44  pvmsrc
 * assign_tasks() now passes distribution info in DM_EXEC message.
 *
 * Revision 1.14  1997/06/02  13:20:48  pvmsrc
 * tm_shmconn() marks old task w/ TF_CLOSE
 * 	- so work() doesn't complain about a dead socket.
 *
 * Revision 1.13  1997/05/28  14:04:05  pvmsrc
 * Cleaned up TM_SHMCONN() handler routine.
 * 	- formatted like hell, and didn't compile clean on SGI64.
 *
 * Revision 1.12  1997/05/27  14:45:53  pvmsrc
 * Added new TM message TM_SHMCONN with routine tm_shmconn():
 * 	which merges records and sets TF_SHMCONN flag.
 * Updated entry point in loclentry() as well:
 * 	to accept this type of connection.
 *
 * Revision 1.11  1997/05/12  20:28:22  pvmsrc
 * Removed duplicate #includes...
 *
 * Revision 1.10  1997/05/07  21:22:12  pvmsrc
 * AAAIIIEEEEEEEEE!!!!
 * 	- removed all static-limited string unpacking:
 * 		* replaced with use of:
 * 			upkstralloc() / PVM_FREE() (for pvmd stuff).
 * 			new pvmupkstralloc() / PVM_FREE() (for lpvm stuff).
 * 		* manual allocation of local buffers for sprintf() & packing.
 * 		* alternate static string arrays to replace fixed-length cases.
 * 		* I hope this shit works...  :-Q
 *
 * Revision 1.9  1997/04/30  21:26:31  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.8  1997/03/27  19:57:10  pvmsrc
 * Spanked in PvmNoSpawnParent flag usage.
 * 	- in tm_spawn(), don't set wxp->w_ptid if PvmNoSpawnParent set.
 * 	- in assign_tasks(), use wxp->w_ptid as parent tid instead of
 * 		snarfing it from wp->wa_tid...  (Bob Hack) D-Oh...
 *
 * Revision 1.7  1997/02/13  15:10:05  pvmsrc
 * Removed unnecessary extern for struct waitc *waitlist.
 * 	- now in global.h.
 *
 * Revision 1.6  1997/01/28  19:27:49  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.5  1996/12/30  13:37:49  pvmsrc
 * Fixed closing comment in last log entry...
 *
 * Revision 1.4  1996/12/20  15:02:07  pvmsrc
 * Fixed missing comment end *-/ ...  D-Oh!
 *
 * Revision 1.3  1996/10/25  13:58:08  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.2  1996/10/24  22:11:04  pvmsrc
 * Moved #include "global.h" below other #include's for typing.
 * Added #include <pvmtev.h> for tracing constants.
 * Added extern struct Pvmtracer pvmtracer for tracer info.
 * Added new tm_tracer() routine to handle tracer registration requests
 * 	from pvm_reg_tracer().
 * In tm_conn2(), insert tracer info for tasks spawned from the shell.
 * 	- can't get env vars set here, so rely on mailbox for that info.
 * 	- use new tev_send_newtask() routine.
 * In assign_tasks():
 * 	- replace inline trace event code with new tev_send_spntask().
 * 	- check trctid / outtid with > 0, not non-zero, for new case where
 * 		task denies external collection.
 * Modified change_trace() to use new event routines:
 * 	- tev_send_newtask(), tev_send_spntask() & tev_send_endtask().
 *
 * Revision 1.1  1996/09/23  23:44:45  pvmsrc
 * Initial revision
 *
 * Revision 1.9  1995/07/19  21:27:26  manchek
 * use new function pvmnametag instead of [dts]mname
 *
 * Revision 1.8  1995/07/03  19:11:40  manchek
 * added tickle #100
 *
 * Revision 1.7  1995/05/17  16:55:20  manchek
 * changed global mytid to pvmmytid.
 * added TM_GETOPT message class
 *
 * Revision 1.6  1994/10/15  19:37:46  manchek
 * cast message tags for comparison as integers.
 * check newhosts when deleting host.
 * unset TF_FORKD if task reconnects with a different pid
 *
 * Revision 1.5  1994/06/21  18:30:00  manchek
 * subscript arith in tmname() broke with opt
 *
 * Revision 1.4  1994/06/04  21:45:54  manchek
 * misc
 *
 * Revision 1.3  1994/06/03  20:38:31  manchek
 * version 3.3.0
 *
 * Revision 1.2  1993/11/30  15:56:23  manchek
 * tm_conn() complains if it can't write t-auth file (fs full?)
 *
 * Revision 1.1  1993/08/30  23:26:52  manchek
 * Initial revision
 *
 */

#include <stdio.h>

#ifdef IMA_TITN
#include <bsd/sys/types.h>
#else
#include <sys/types.h>
#endif

#include <sys/stat.h>
#include <fcntl.h>

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
#include <time.h>
#else
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <fcntl.h>
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <errno.h>

#include <pvmproto.h>
#include "pvmalloc.h"
#include "pmsg.h"
#include "task.h"
#include "host.h"
#include "waitc.h"
#include "listmac.h"
#include "bfunc.h"
#include <pvmtev.h>
#include "global.h"
#ifdef IMA_BEOLIN
#include "pvmdmp.h"
#include "pvmmimd.h"
#endif

#ifndef	min
#define	min(a,b)	((a)<(b)?(a):(b))
#endif


/***************
 **  Globals  **
 **           **
 ***************/

extern void task_dump();
char *debug_flags();
char *inadport_hex();
char *pvmnametag();
void i_dump();
void tev_send_newtask();
void tev_send_spntask();
void tev_send_endtask();

extern int pvmdebmask;				/* from pvmd.c */
extern int hostertid;				/* from pvmd.c */
extern struct htab *hosts;			/* from pvmd.c */
extern int pvmmydsig;				/* from pvmd.c */
extern int myhostpart;				/* from pvmd.c */
extern int pvmmytid;				/* from pvmd.c */
extern struct htab *newhosts;		/* from pvmd.c */
extern int nopax;					/* from pvmd.c */
extern int pvmudpmtu;				/* from pvmd.c */
extern int pvmschedtid;				/* from pvmd.c */
extern int taskertid;				/* from pvmd.c */
extern struct Pvmtracer pvmtracer;	/* from pvmd.c */
extern int tidhmask;				/* from pvmd.c */


/***************
 **  Private  **
 **           **
 ***************/

int tm_addhost();
int tm_config();
int tm_conn2();
int tm_connect();
int tm_context();
int tm_db();
int tm_delhost();
int tm_exit();
int tm_getopt();
int tm_halt();
int tm_hoster();
int tm_hostsync();
int tm_mca();
int tm_mstat();
int tm_notify();
int tm_pstat();
int tm_sendsig();
int tm_sched();
int tm_setopt();
int tm_spawn();
int tm_task();
int tm_tasker();
int tm_tickle();
int tm_tracer();
int tm_shmconn();

int (*loclswitch[])() = {
	tm_connect,
	tm_conn2,
	tm_exit,
	tm_addhost,
	tm_delhost,
	tm_config,
	tm_mstat,
	tm_halt,
	tm_tickle,
	tm_spawn,
	tm_pstat,
	tm_sendsig,
	tm_task,
	tm_mca,
	tm_notify,
	tm_db,
	tm_sched,
	tm_tasker,
	tm_hoster,
	tm_tracer,
	tm_hostsync,
	tm_setopt,
	tm_getopt,
	tm_context,
	tm_shmconn,
};


int
loclentry(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int c = mp->m_tag;

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("loclentry() from t%x tag %s\n", mp->m_src,
				pvmnametag(c, (int *)0));
/*
		pvmhdump(mp->m_cfrag->fr_dat, mp->m_cfrag->fr_len, "frag: ");
*/
	}

/*
	if (mp->m_enc != 1) {
		pvmlogprintf("loclentry() message from t%x with bad enc %d\n",
				tp->t_tid, mp->m_enc);
		goto bail;
	}
*/

	if (c < (int)TM_FIRST || c > (int)TM_LAST) {
		pvmlogprintf("loclentry() message from t%x with bogus tag %d\n",
				tp->t_tid, c);
		goto bail;
	}

	if ((!(tp->t_flag & TF_CONN) && c != TM_CONNECT && c != TM_CONN2 
								 && c != TM_SHMCONN)
	|| ((tp->t_flag & TF_AUTH) && c != TM_CONN2)) {
		pvmlogerror("loclentry() non-connect message from anon task\n");
		tp->t_flag |= TF_CLOSE;
		goto bail;
	}

	c -= TM_FIRST;
	(loclswitch[c])(tp, mp);

bail:
	pmsg_unref(mp);
	return 0;
}


/*	replymessage()
*
*	Construct reply message to sender with matching context, tag, wait id
*/

struct pmsg *
replymessage(mp)
	struct pmsg *mp;
{
	struct pmsg *rmp;

	if (rmp = mesg_new(0)) {
		rmp->m_dst = mp->m_src;
		rmp->m_ctx = mp->m_ctx;
		rmp->m_tag = mp->m_tag;
		rmp->m_wid = mp->m_wid;
	}
	return rmp;
}


/*********************************
 **  Task request entry points  **
 **                             **
 *********************************/


/*	tm_connect()
*
*	Task connecting to pvmd phase 1.
*	We assign it a context, write in t-auth file to prove our ident,
*	then make d-auth file for task to prove itself.
*
*	TM_CONNECT
*	call {
*		int tdprotocol		// t-d protocol number compiled into libpvm
*		string authfile		// t-auth file pvmd must write
*	}
*	ret {
*		int tdprotocol		// pvmd's td protocol version
*		int acknack			// 1 if pvmd accepts connection
*		string authfile		// d-auth file task must write
*	}
*/

int
tm_connect(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int ver;						/* task's libpvm t-d proto version */
	char authfn[PVMTMPNAMLEN];	/* t-auth file name */
	int d;
	int cc;

	if (upkint(mp, &ver) || upkstr(mp, authfn, sizeof(authfn))) {
		pvmlogerror("tm_connect() bad msg format\n");
		goto bail;
	}

	/*
	*	if protocols are not compatible, send nack
	*	context will get flushed after reply is sent
	*/

	if (ver != TDPROTOCOL) {
		pvmlogprintf("tm_connect() t-d protocol mismatch (%d/%d)\n",
			ver, TDPROTOCOL);

		mp = replymessage(mp);
		pkint(mp, TDPROTOCOL);
		pkint(mp, 0);
		pkstr(mp, "");
		mp->m_flag |= MM_PRIO;
		mesg_to_task(tp, mp);
		pmsg_unref(mp);
		goto bail;
	}

#ifdef NOPROT
	tp->t_authnam = TALLOC(PVMTMPNAMLEN, char, "auth");
	(void)PVMTMPNAMFUN(tp->t_authnam);
#else

	/*
	*	write in t-auth file, create empty d-auth file that task
	*	must write and we'll check later
	*/

	if ((d = open(authfn, O_WRONLY, 0)) == -1) {
		pvmlogperror("tm_connect() can't open t-auth file");
		goto bail;
	}
	cc = write(d, authfn, 1);
	if (cc != 1) {
		if (cc == -1)
			pvmlogperror(authfn);
		pvmlogerror("tm_connect() can't write t-auth file\n");
	}
	(void)close(d);

	tp->t_authnam = TALLOC(PVMTMPNAMLEN, char, "auth");
	(void)PVMTMPNAMFUN(tp->t_authnam);

#ifndef IMA_OS2
	if ((tp->t_authfd = open(tp->t_authnam, O_RDONLY|O_CREAT|O_EXCL|O_TRUNC, 0600)) == -1)
#else
	if ((tp->t_authfd = open(tp->t_authnam, O_RDWR|O_CREAT|O_EXCL|O_TRUNC, 0600)) == -1)
#endif
	{
		pvmlogperror("tm_connect() can't create d-auth file");
		PVM_FREE(tp->t_authnam);
		tp->t_authnam = 0;
		goto bail;
	}

#endif

	/*
	*	task's turn to auth
	*/

	tp->t_flag |= TF_AUTH;

	mp = replymessage(mp);
	pkint(mp, TDPROTOCOL);
	pkint(mp, 1);
	pkstr(mp, tp->t_authnam);
	mp->m_flag |= MM_PRIO;
	mesg_to_task(tp, mp);
	pmsg_unref(mp);
	return 0;

bail:

	tp->t_flag |= TF_CLOSE;

	/* better send a failure notification to the task... */
	/* (is this a good idea, or better to let task hang, */
	/* i.e. for security purposes - unresponsiveness...?) */
	mp = replymessage(mp);
	pkint(mp, TDPROTOCOL);
	pkint(mp, 0);
	pkstr(mp, "");
	mp->m_flag |= MM_PRIO;
	mesg_to_task(tp, mp);
	pmsg_unref(mp);
	return 0;
}


/*	tm_conn2()
*
*	Task connecting to pvmd phase 2.
*	We check d-auth file and give it config info, attach to real context.
*
*	TM_CONN2
*	call {
*		int unixpid			// real pid
*		int cookie			// magic cookie inherited from pvmd (or 0)
*		(char *taskname)	// (optional) task name specification
*	}
*	ret {
*		int acknack			// 1 if pvmd accepts connection
*		int tid				// task tid
*		int ptid			// parent tid
*		int outtid			// output dst
*		int outctx
*		int outtag
*		int trctid			// trace dst
*		int trcctx
*		int trctag
*		int udpmax
*		int nativecode
*		string inaddr
*		int schedtid		// scheduler tid
*	}
*/

int
tm_conn2(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int tid;
	int pid;						/* real pid of task */
	int cookie;						/* cookie to identify task or 0 */
	struct task *tp2;				/* to search for existing context */
	struct pmsg *mp2;
	int cc;
	char c;
	char *taskname = (char *) NULL;

	if (upkint(mp, &pid) || upkint(mp, &cookie)) {
		pvmlogerror("tm_conn2() bad msg format\n");
		goto bail;
	}
	if (!cookie)
		cookie = pid;

	if (!(tp->t_flag & TF_AUTH)) {
		pvmlogprintf("tm_conn2() message from t%x, TF_AUTH not set\n",
				tp->t_tid);
		return 0;
	}

	/*
	*	check that task could write d-auth file
	*/

#ifndef NOPROT

	if ((cc = read(tp->t_authfd, &c, 1)) == -1) {
		pvmlogperror("tm_conn2() can't read d-auth file");
		return 0;
	}

	if (cc != 1) {
		pvmlogerror("tm_conn2() task didn't validate itself\n");
		goto bail;
	}

	(void)close(tp->t_authfd);
	tp->t_authfd = -1;
	(void)unlink(tp->t_authnam);

#endif

	PVM_FREE(tp->t_authnam);
	tp->t_authnam = 0;

	/*
	*	if task spawned by us, already has a context,
	*	else make it one
	*/

	if ((tp2 = task_findpid(cookie)) && !(tp2->t_flag & (TF_AUTH|TF_CONN))) {
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("tm_conn2() reconnect task t%x\n", tp2->t_tid);
		}
		tp->t_sched = tp2->t_sched;

#ifdef IMA_BEOLIN
	} else if ( (tp2 = mpp_conn(tp))
			&& !(tp2->t_flag & (TF_AUTH|TF_CONN)) ) {
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf(
					"tm_conn2() reconnect task t%x pid=%d name=%s\n",
					tp2->t_tid,tp2->t_pid, tp2->t_a_out );
		}
#endif

	} else {
		if ((tid = tid_new()) < 0) {
			pvmlogerror("tm_conn2() out of tids?\n");
			goto bail;		/* XXX should disconnect nicely */
		}
		if ((tp2 = task_new(tid)) == NULL) {
			pvmlogerror("tm_conn2() too many tasks?\n");
			goto bail;		/* XXX should disconnect nicely */
		}

		if ( !upkstralloc(mp, &taskname) ) {
			tp2->t_a_out = taskname;
		}

		if (pvmschedtid) {
			/* inform the scheduler about the new task */

			tp->t_sched = pvmschedtid;
			mp2 = mesg_new(0);
			pkint(mp2, 1);
			pkint(mp2, tid);
			mp2->m_tag = SM_EXECACK;		/* XXX yecch, overload */
			mp2->m_dst = pvmschedtid;
			sendmessage(mp2);
		}

		/* check for tracer */
		if (pvmtracer.trctid && pvmtracer.trctag) {
			tp2->t_trctid = pvmtracer.trctid;
			tp2->t_trcctx = pvmtracer.trcctx;
			tp2->t_trctag = pvmtracer.trctag;
			/* Note:  can't get the trace mask or trace buffer size
				to task here, but this will be handled using mailbox */

			/* trace start message */
			/* XXX Note:  we don't really know if we should send this
				because this task could have prohibited tracing
				by setting PvmSelfTraceTid to -1... */
			tev_send_newtask(
				pvmtracer.trctid, pvmtracer.trcctx, pvmtracer.trctag,
				tid, -1, -1, tp->t_a_out ? tp->t_a_out : "-" );
		}
		if (pvmtracer.outtid && pvmtracer.outtag) {
			tp2->t_outtid = pvmtracer.outtid;
			tp2->t_outctx = pvmtracer.outctx;
			tp2->t_outtag = pvmtracer.outtag;

			/* output start message */
			/* Note:  same deal here as for TEV_NEWTASK above... */
			mp2 = mesg_new(0);
			mp2->m_dst = pvmtracer.outtid;
			mp2->m_ctx = pvmtracer.outctx;
			mp2->m_tag = pvmtracer.outtag;
			pkint(mp2, tid);
			pkint(mp2, TO_NEW);
			pkint(mp2, -1);
			sendmessage(mp2);
		}

		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("tm_conn2() new task t%x\n", tp2->t_tid);
		}
	}

	/*
	*	brundle-fly the contexts together
	*/

	tp2->t_sock = tp->t_sock;
	tp2->t_sad = tp->t_sad;
	tp2->t_salen = tp->t_salen;

#ifdef NOPROT
	if (!TIDISNODE(tp2->t_tid)) {
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf( "Setting pid of t%x to %d, same as t%x\n",
					tp2->t_tid, tp->t_pid, tp->t_tid );
		}
		task_setpid(tp2, tp->t_pid);
	}
#else
	if (tp2->t_pid != pid)
		task_setpid(tp2, pid);
#endif

	tp2->t_rxp = tp->t_rxp;
	tp2->t_sched = tp->t_sched;
	tp->t_sock = -1;	/* tp will be freed by loclinput() */
	tp->t_rxp = 0;
	tp = tp2;
	if (cookie != pid)
		tp->t_flag &= ~TF_FORKD;

	/*
	*	kick it in the butt; it's ready to go
	*/

	tp->t_flag &= ~TF_AUTH;
	tp->t_flag |= TF_CONN;

	mp = replymessage(mp);
	pkint(mp, 1);
	pkint(mp, tp->t_tid);
	pkint(mp, tp->t_ptid);
	pkint(mp, tp->t_outtid);
	pkint(mp, tp->t_outctx);
	pkint(mp, tp->t_outtag);
	pkint(mp, tp->t_trctid);
	pkint(mp, tp->t_trcctx);
	pkint(mp, tp->t_trctag);
	pkint(mp, pvmudpmtu);
	pkint(mp, pvmmydsig);
#ifdef IMA_BEOLIN
	if (TIDISNODE(tp->t_tid))
		/* on a node, use the address that came from nodeaddr[] */
		pkstr(mp, inadport_hex(&(tp->t_sad)));
	else
		pkstr(mp, inadport_hex(&(hosts->ht_hosts[hosts->ht_local]->hd_sad)));
#else
	pkstr(mp, inadport_hex(&(hosts->ht_hosts[hosts->ht_local]->hd_sad)));
#endif
	pkint(mp, pvmschedtid);
	mp->m_dst = tp->t_tid;
	mp->m_flag |= MM_PRIO;
	sendmessage(mp);
	return 0;

bail:
	tp->t_flag |= TF_CLOSE;
	return 0;
}


/*	tm_exit()
*
*	Last message from a task.  This is the nice way of disconnecting.
*
*	TM_EXIT
*	call { }
*	ret { }
*/

int
tm_exit(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	mp = replymessage(mp);
	tp->t_flag |= TF_CLOSE;
	sendmessage(mp);
	if (!(tp->t_flag & TF_FORKD))
		task_cleanup(tp);
	return 0;
}


/*	tm_pstat()
*
*	Task wants status of another task.
*
*	TM_PSTAT
*	call {
*		int tid
*	}
*	ret {
*		int status
*	}
*/

int
tm_pstat(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int tid;
	struct hostd *hp;
	struct waitc *wp;

	/* unpack and sanity check tid */

	if (upkuint(mp, &tid)) {
		pvmlogerror("tm_pstat() bad msg format\n");
		return 0;
	}
	if (!TIDISTASK(tid)) {
		pvmlogprintf("tm_pstat() bad tid %x\n", tid);
		return 0;
	}

	/* nack if no such host */

	if (!(hp = tidtohost(hosts, tid))) {
		mp = replymessage(mp);
		pkint(mp, PvmNoTask);
		sendmessage(mp);
		return 0;
	}

	/* else make a wait context and send query */

	wp = wait_new(WT_PSTAT);
	wp->wa_tid = tp->t_tid;
	wp->wa_on = hp->hd_hostpart;
	wp->wa_mesg = replymessage(mp);

	mp = mesg_new(0);
	mp->m_dst = hp->hd_hostpart | TIDPVMD;
	mp->m_tag = DM_PSTAT;
	mp->m_wid = wp->wa_wid;
	pkint(mp, tid);
	sendmessage(mp);
	return 0;
}


/*	tm_addhost()
*
*	Task requesting to add hosts to virtual machine.  Exit point is
*	here or dm_addack().
*
*	TM_ADDHOST
*	call {
*		int nhosts
*		string names[nhosts]
*	}
*	ret {
*		int nhosts			// or error code
*		int narches			// if nhosts >= 0
*		{
*			int tid			// or error code
*			string name
*			string arch
*			int speed
*			int dsig
*		} [nhosts]
*	}
*/

int
tm_addhost(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int count;
	char *buf;
	struct waitc *wp;

	/* sanity check the message */

	if (upkint(mp, &count))
		goto bad;
	if (count < 1 || count > (tidhmask >> (ffs(tidhmask) - 1)))
		goto bad;
	while (count-- > 0) {
		if (upkstralloc(mp, &buf))
			goto bad;
		else
			PVM_FREE(buf);
	}

	/* make a wait channel for the task */

	wp = wait_new(WT_ADDHOST);
	wp->wa_tid = tp->t_tid;
	wp->wa_on = hosts->ht_hosts[hosts->ht_master]->hd_hostpart;
	wp->wa_mesg = replymessage(mp);

	/* forward message to master pvmd */

	mp->m_ref++;
	mp->m_src = pvmmytid;
	mp->m_dst = hosts->ht_hosts[hosts->ht_master]->hd_hostpart | TIDPVMD;
	mp->m_tag = DM_ADD;
	mp->m_wid = wp->wa_wid;
	sendmessage(mp);
	return 0;

bad:
	pvmlogprintf("tm_addhost() from t%x bad msg format\n", mp->m_src);
	return 0;
}


int
free_wait_spawn(wxp)
	struct waitc_spawn *wxp;
{
	int i;

	if (wxp->w_file)
		PVM_FREE(wxp->w_file);
	if (wxp->w_argv) {
		for (i = 0; i < wxp->w_argc; i++)
			if (wxp->w_argv[i])
				PVM_FREE(wxp->w_argv[i]);
		PVM_FREE(wxp->w_argv);
	}
	if (wxp->w_env) {
		for (i = 0; i < wxp->w_nenv; i++)
			if (wxp->w_env[i])
				PVM_FREE(wxp->w_env[i]);
		PVM_FREE(wxp->w_env);
	}
	if (wxp->w_ht)
		ht_free(wxp->w_ht);
	if (wxp->w_vec)
		PVM_FREE(wxp->w_vec);
	PVM_FREE(wxp);
	return 0;
}


/*	tm_spawn()
*
*	Task requesting to spawn other tasks.  Exit point for this
*	request is here or dm_execack().
*
*	TM_SPAWN
*	call {
*		string file
*		int flags
*		string where
*		int count
*		int nargs
*		string argv[nargs]
*		int outtid
*		int outctx
*		int outtag
*		int trctid
*		int trcctx
*		int trctag
*		int nenv
*		string env[nenv]
*	}
*	ret {
*		int ntids  // or nack
*		int tids[acknack]
*	}
*/

int
tm_spawn(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	char *where = 0;				/* location from req */
	struct waitc *wp;				/* 'seed' waitc */
	struct waitc_spawn *wxp = 0;	/* new task parameters */
	struct htab *htp;				/* set of usable hosts */
	struct hostd *hp;
	int hh;
	int i;
	char *wd = 0;
	char *wdenv = 0;
	char *ptr;
	int sz;

	/*
	* unpack spawn command from task
	*/

	wxp = TALLOC(1, struct waitc_spawn, "waix");
	BZERO((char*)wxp, sizeof(struct waitc_spawn));

	if (upkstralloc(mp, &wxp->w_file)
	|| upkint(mp, &wxp->w_flags)
	|| upkstralloc(mp, &where)
	|| upkint(mp, &wxp->w_veclen)
	|| upkint(mp, &wxp->w_argc))
		goto bad;

	if (wxp->w_veclen < 1)
		goto bad;

	wxp->w_argv = TALLOC(wxp->w_argc + 1, char*, "argv");
	BZERO((char*)wxp->w_argv, (wxp->w_argc + 1) * sizeof(char*));
	for (i = 0; i < wxp->w_argc; i++)
		if (upkstralloc(mp, &wxp->w_argv[i]))
			goto bad;

	if (upkuint(mp, &wxp->w_outtid)
	|| upkuint(mp, &wxp->w_outctx)
	|| upkuint(mp, &wxp->w_outtag)
	|| upkuint(mp, &wxp->w_trctid)
	|| upkuint(mp, &wxp->w_trcctx)
	|| upkuint(mp, &wxp->w_trctag))
		goto bad;

	/*
	* extract any working directory string from "where"
	*/

	ptr = where;
	while ( *ptr != ':' && *ptr != '\0' )
		ptr++;
	if ( *ptr == ':' ) {
		*ptr++ = '\0';    /* close off actual "where" string */
		wd = ptr;         /* save ptr to working directory */
	}

	/*
	* unpack environment from task
	*/

	if (upkuint(mp, &wxp->w_nenv))
		goto bad;
	sz = wxp->w_nenv + 1 + (wd ? 1 : 0);
	wxp->w_env = TALLOC(sz, char*, "env");
	BZERO((char*)wxp->w_env, sz * sizeof(char*));
	for (i = 0; i < wxp->w_nenv; i++)
		if (upkstralloc(mp, &wxp->w_env[i]))
			goto bad;

	/*
	* add extra env string for working directory
	*/

	if ( wd ) {
		sz = strlen(wd) + strlen("PVMSPAWNWD=") + 1;
		wdenv = TALLOC(sz, char, "wdenv");
		sprintf( wdenv, "PVMSPAWNWD=%s", wd );
		wxp->w_env[ (wxp->w_nenv)++ ] = STRALLOC( wdenv );
	}

	/*
	* make host set containing hosts (matching where option)
	*/

	if ((wxp->w_flags & (PvmTaskHost|PvmTaskArch)) && !where)
		goto bad;

	htp = ht_new(1);

	if (wxp->w_flags & PvmTaskHost) {			/* given host */
		if (hp = nametohost(hosts, where))
			ht_insert(htp, hp);

	} else {
		if (wxp->w_flags & PvmTaskArch) {		/* given arch */
			for (hh = hosts->ht_last; hh > 0; hh--)
				if ((hp = hosts->ht_hosts[hh])
				&& !strcmp(where, hp->hd_arch))
					ht_insert(htp, hp);

#ifdef IMA_BEOLIN
		/* local pvmd for now */
		} else if (wxp->w_flags & PvmMppFront) {
			hp = hosts->ht_hosts[hosts->ht_local];
			ht_insert(htp, hp);
#endif

		} else {						/* anywhere */
			ht_merge(htp, hosts);
		}
	}

	if (wxp->w_flags & PvmHostCompl) {
		for (hh = hosts->ht_last; hh > 0; hh--) {
			if (hh <= htp->ht_last && (hp = htp->ht_hosts[hh]))
				ht_delete(htp, hp);
			else
				if (hp = hosts->ht_hosts[hh])
					ht_insert(htp, hp);
		}
	}

	if (pvmdebmask & PDMTASK) {
		pvmlogerror("tm_spawn() host set:\n");
		ht_dump(htp);
	}

	if ( !(wxp->w_flags & PvmNoSpawnParent) ) {
		wxp->w_ptid = tp->t_tid;
	}
	else
		wxp->w_ptid = PvmParentNotSet;	/* indicate task unset parent */

	/*
	* assign each task to a host
	*/

	wxp->w_ht = htp;
	wxp->w_vec = TALLOC(wxp->w_veclen, int, "vec");
	BZERO((char*)wxp->w_vec, wxp->w_veclen * sizeof(int));
	wxp->w_togo = wxp->w_veclen;

	wp = wait_new(WT_SPAWN);
	wp->wa_tid = tp->t_tid;
	wp->wa_spec = (void*)wxp;
	wxp = 0;
	wp->wa_mesg = replymessage(mp);

	assign_tasks(wp);

	/* if already done, reply to task */

	if (wp->wa_peer == wp) {
		assign_tasks(wp);
	}

	wait_delete(wp);
	goto cleanup;

bad:
	pvmlogprintf("tm_spawn() from t%x bad msg format\n", mp->m_src);

cleanup:
	if (where)
		PVM_FREE(where);
	if (wxp)
		free_wait_spawn(wxp);
	return 0;
}


/*	assign_tasks()
*
*	This is only called when no replies are pending (at the beginning
*	of the operation or when all pvmds have checked back in).
*/

assign_tasks(wp)
	struct waitc *wp;		/* (any) waitc in peer group for this op */
{
	static int lasthh = -1;			/* for assigning hosts */

	struct waitc_spawn *wxp = (struct waitc_spawn*)wp->wa_spec;
	struct htab *htp;				/* set of hosts to use */
	int *vec;						/* result/status vector */
	int veclen;						/* length of vector */
	int count = 0;					/* num of tasks to be assigned */
	int nh;							/* num of hosts to assign tasks */
	int a = 0;						/* accum for finding hosts */
	int na = 0;						/* num tasks assigned to a host */
	struct waitc *wp2;
	struct hostd *hp;
	struct pmsg *mp;
	int t;
	int i;
	int tid;
	struct timeval now;

	if (!wxp)
		return 0;

	htp = wxp->w_ht;
	vec = wxp->w_vec;
	veclen = wxp->w_veclen;

	/*
	* if no hosts left, fill unassigned entries with PvmNoHost
	*/

	if (!htp->ht_cnt)
		for (t = veclen; t-- > 0; )
			if (!vec[t])
				vec[t] = PvmNoHost;

	/*
	* count tasks to be assigned, if none left reply to task
	*/

	for (t = veclen; t-- > 0; )
		if (!vec[t])
			count++;

	if (!count) {
		pkint(wp->wa_mesg, wxp->w_veclen);
		for (t = 0; t < wxp->w_veclen; t++) {
			tid = wxp->w_vec[t];
			pkint(wp->wa_mesg, tid);
			if (TIDISTASK(tid) && wxp->w_trctid > 0) {
				tev_send_spntask(
					wxp->w_trctid, wxp->w_trcctx, wxp->w_trctag,
					tid, wxp->w_ptid );
			}
			if (TIDISTASK(tid) && wxp->w_outtid > 0) {
				mp = mesg_new(0);
				mp->m_dst = wxp->w_outtid;
				mp->m_ctx = wxp->w_outctx;
				mp->m_tag = wxp->w_outtag;
				pkint(mp, tid);
				pkint(mp, TO_SPAWN);
				pkint(mp, wxp->w_ptid);
				sendmessage(mp);
			}
		}
		sendmessage(wp->wa_mesg);
		wp->wa_mesg = 0;

		free_wait_spawn(wxp);
		wp->wa_spec = 0;
		return 0;
	}

	/*
	* assign tasks to hosts
	*/

	nh = min(htp->ht_cnt, count);

	/* find first host to assign */

	if (lasthh == -1)
		lasthh = hosts->ht_local + 1;
	if (lasthh > htp->ht_last)
		lasthh = 0;
	while (!htp->ht_hosts[lasthh])
		if (++lasthh > htp->ht_last)
			lasthh = 1;
	hp = htp->ht_hosts[lasthh];

	for (t = 0; t < veclen && vec[t]; t++);

	while (t < veclen) {
/*
		pvmlogprintf("assign_tasks() %s <- %d\n", hp->hd_name, t);
*/

		vec[t] = hp->hd_hostpart;
		na++;

	/* when enough tasks for this host, move to next */

		if ((a += nh) >= count) {
			a -= count;

			wp2 = wait_new(WT_SPAWN);
			wp2->wa_tid = wp->wa_tid;
			wp2->wa_on = hp->hd_hostpart;
			wp2->wa_spec = wp->wa_spec;
			wp->wa_mesg->m_ref++;
			wp2->wa_mesg = wp->wa_mesg;
			LISTPUTBEFORE(wp, wp2, wa_peer, wa_rpeer);

			mp = mesg_new(0);
			pkint(mp, wxp->w_ptid);
			pkstr(mp, wxp->w_file);
			pkint(mp, wxp->w_flags);
			pkint(mp, na);
			pkint(mp, wxp->w_argc);
			for (i = 0; i < wxp->w_argc; i++)
				pkstr(mp, wxp->w_argv[i]);
			pkint(mp, wxp->w_outtid);
			pkint(mp, wxp->w_outctx);
			pkint(mp, wxp->w_outtag);
			pkint(mp, wxp->w_trctid);
			pkint(mp, wxp->w_trcctx);
			pkint(mp, wxp->w_trctag);
			pkint(mp, wxp->w_nenv);
			for (i = 0; i < wxp->w_nenv; i++)
				pkstr(mp, wxp->w_env[i]);
			pkint(mp, (t+1)-na);	/* start proc location 0..count-1 */
			pkint(mp, count);		/* how many are there in total */
			mp->m_dst = hp->hd_hostpart | TIDPVMD;
			mp->m_tag = DM_EXEC;
			mp->m_wid = wp2->wa_wid;

			do {
				if (++lasthh > htp->ht_last)
					lasthh = 1;
			} while (!htp->ht_hosts[lasthh]);

			sendmessage(mp);

			hp = htp->ht_hosts[lasthh];
			na = 0;
		}
		for (t++ ; t < veclen && vec[t]; t++);
	}
	return 0;
}


/*	tm_sendsig()
*
*	Task sending a signal to another task.
*
*	TM_SENDSIG
*	call {
*		int tid
*		int signum
*	}
*	ret { }
*/

int
tm_sendsig(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int tid;
	struct pmsg *mp2;

	if (upkuint(mp, &tid)) {
		pvmlogerror("tm_sendsig() bad msg format\n");
		return 0;
	}
	if (!TIDISTASK(tid)) {
		pvmlogprintf("tm_sendsig() bad tid %x\n", tid);
		return 0;
	}

	mp2 = replymessage(mp);
	sendmessage(mp2);

	mp->m_ref++;
	mp->m_src = pvmmytid;
	mp->m_dst = (tid & TIDHOST) | TIDPVMD;
	mp->m_tag = DM_SENDSIG;
	mp->m_wid = 0;
	sendmessage(mp);
	return 0;
}


/*	tm_config()
*
*	Task wants machine configuration.
*
*	TM_CONFIG
*	call { }
*	ret {
*		int nhosts
*		int narches
*		{
*			int tid
*			string name
*			string arch
*			int speed
*			int dsig
*		} [nhosts]
*	}
*/

int
tm_config(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int hh;
	struct hostd *hp;

	mp = replymessage(mp);
	pkint(mp, hosts->ht_cnt);
	pkint(mp, hosts->ht_narch);
	for (hh = 1; hh <= hosts->ht_last; hh++) {
		if (hp = hosts->ht_hosts[hh]) {
			pkint(mp, hp->hd_hostpart);
			pkstr(mp, hp->hd_name);
			pkstr(mp, hp->hd_arch ? hp->hd_arch : "");
			pkint(mp, hp->hd_speed);
			pkint(mp, hp->hd_dsig);
		}
	}
	sendmessage(mp);
	return 0;
}


/*	tm_halt()
*
*	Command from task to stop master pvmd.
*
*	TM_HALT
*	call { }
*	ret { }
*/

int
tm_halt(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	mp = mesg_new(0);
	mp->m_tag = DM_HALT;
	mp->m_dst = hosts->ht_hosts[hosts->ht_master]->hd_hostpart | TIDPVMD;
	sendmessage(mp);
	return 0;
}


/*	tm_task()
*
*	Task wants a list of tasks.
*	If where is a host or task tid, give a list of matching tasks.
*	If where is zero, give all running tasks.
*
*	TM_TASK
*	call {
*		int where
*	}
*	ret {
*		int errcode
*		{
*			int tid
*			int ptid
*			int hostpart
*			int flag
*			string a_out
*			int pid
*		} [count]    // implied
*	}
*/

int
tm_task(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int where;
	struct pmsg *mp2;
	struct waitc *wp;
	struct waitc *wp2 = 0;	/* master waitc of peer group */
	int hh;
	struct hostd *hp;

	if (upkuint(mp, &where)) {
		pvmlogerror("tm_task() bad msg format\n");
		return 0;
	}

	mp = replymessage(mp);
	mp->m_dst = tp->t_tid;
	mp->m_tag = TM_TASK;

	if (where) {		/* specific host or task requested */
		if (!(hp = tidtohost(hosts, where))) {
			pkint(mp, PvmNoHost);
			sendmessage(mp);
			return 0;
		}
		pkint(mp, 0);
		wp = wait_new(WT_TASK);
		wp->wa_mesg = mp;
		wp->wa_tid = tp->t_tid;
		wp->wa_on = hp->hd_hostpart;

		mp = mesg_new(0);
		mp->m_tag = DM_TASK;
		mp->m_dst = hp->hd_hostpart | TIDPVMD;
		mp->m_wid = wp->wa_wid;
		pkint(mp, where);
		sendmessage(mp);

	} else {			/* all tasks requested */
		pkint(mp, 0);

		wp2 = wait_new(WT_TASK);
		mp->m_ref++;
		wp2->wa_mesg = mp;
		wp2->wa_tid = tp->t_tid;

		mp2 = mesg_new(0);
		mp2->m_tag = DM_TASK;
		pkint(mp2, 0);

		for (hh = hosts->ht_last; hh > 0; hh--) {
			if (!hosts->ht_hosts[hh])
				continue;

			wp = wait_new(WT_TASK);
			mp->m_ref++;
			wp->wa_mesg = mp;
			wp->wa_tid = tp->t_tid;
			wp->wa_on = hosts->ht_hosts[hh]->hd_hostpart;

			LISTPUTBEFORE(wp2, wp, wa_peer, wa_rpeer);

			mp2->m_dst = hosts->ht_hosts[hh]->hd_hostpart | TIDPVMD;
			mp2->m_wid = wp->wa_wid;
			mp2->m_ref++;
			sendmessage(mp2);
		}
		pmsg_unref(mp2);
		pmsg_unref(mp);

		/* send message if all waiters are in */

		if (wp2->wa_peer == wp2) {
			mp->m_ref++;
			sendmessage(mp);
		}
		wait_delete(wp2);
	}
	return 0;
}


/*	tm_tickle()
*
*	Task wants to poke at pvmd.
*
*	TM_TICKLE
*	call {
*		int nargs
*		int args[nargs]
*	}
*	ret {
*		int nresult
*		int results[nresult]
*	}
*/

int
tm_tickle(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int nar;
	int arg[10];
	int i;
	char *p;
	struct hostd *hp;

	upkint(mp, &nar);
	if (nar < 1 || nar > 10) {
		pvmlogprintf("tm_tickle() bad msg format\n");
		return 0;
	}
	for (i = 0; i < nar; i++)
		upkint(mp, &arg[i]);
	while (i < sizeof(arg)/sizeof(arg[0]))
		arg[i++] = 0;

	pvmlogprintf("tm_tickle() #");
	for (i = 0; i < nar; i++)
		pvmlogprintf(" %d", arg[i]);
	pvmlogprintf("\n");

	mp = replymessage(mp);

	switch (arg[0]) {

	case 0:
		i_dump(1);
		pkint(mp, 0);
		break;

	case 1:
		ht_dump(hosts);
		pkint(mp, 0);
		break;

	case 2:
		task_dump();
		pkint(mp, 0);
		break;

	case 3:
		wait_dumpall();
		pkint(mp, 0);
		break;

	case 4:
		mb_dumpall();
		pkint(mp, 0);
		break;

	case 5:
		pkint(mp, 1);
		pkint(mp, pvmdebmask);
		break;

	case 6:
		pvmdebmask = arg[1];
		pvmlogprintf("tm_tickle() debug mask is %x (%s)\n",
				pvmdebmask, debug_flags(pvmdebmask));
		pkint(mp, 0);
		break;

	case 7:
		if (arg[1] > 0 && arg[1] < 50) {
			nopax = arg[1];
			pvmlogprintf("tm_tickle() nopax is %d\n", nopax);
		} else
			pvmlogprintf("tm_tickle() bogus nopax %d\n", arg[1]);
		pkint(mp, 0);
		break;

	case 8:
		pkint(mp, 1);
		if ((hp = tidtohost(hosts, arg[1]))
		&& hp != hosts->ht_hosts[hosts->ht_local]) {
			pvmlogprintf("tm_tickle() failing %s\n", hp->hd_name);
			hostfailentry(hp);
			ht_delete(hosts, hp);
			if (newhosts)
				ht_delete(newhosts, hp);
			pkint(mp, 1);
		} else {
			if (hp)
				pvmlogprintf("tm_tickle() can't fail %s\n", hp->hd_name);
			else
				pvmlogprintf("tm_tickle() no such host %x\n", arg[1]);
			pkint(mp, 0);
		}
		break;

	case 9:
#ifdef	STATISTICS
		dump_statistics();
		if (arg[1])
			reset_statistics();
#else
		pvmlogerror("tm_tickle() statistics not compiled in\n");
#endif
		pkint(mp, 0);
		break;

	case 100:
#ifdef SHMEM
		peer_dump();
		pidtid_dump();
#else
		pvmlogerror("tm_tickle() not using shared memory\n");
#endif
		pkint(mp, 0);
		break;

	case 20:
		{
			struct pmsg *mp2;
			int i, j;

			for (j = 0; j < arg[1]; j++) {
				mp2 = mesg_new(0);
				for (i = 0; i < 1000; i++)
					pkint(mp2, i);
				mp2->m_dst = arg[2];
				mp2->m_tag = DM_NULL;
				sendmessage(mp2);
			}
			pkint(mp, 0);
		}
		break;

	default:
		pvmlogprintf("tm_tickle() don't know #%d\n", arg[0]);
		pkint(mp, 0);
		break;
	}

	sendmessage(mp);
	return 0;
}


/*	tm_delhost()
*
*	Task requesting to add hosts to virtual machine.  Exit point is
*	here or dm_delhostack().
*
*	TM_DELHOST
*	call {
*		int nhosts
*		string names[nhosts]
*	}
*	ret {
*		int nhosts			// or negative for error
*		int status[nhosts]	// status of each host
*	}
*/

int
tm_delhost(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int count;
	char *buf;
	struct waitc *wp;

	/* sanity check the message */

	if (upkint(mp, &count))
		goto bad;
	if (count < 1 || count > (tidhmask >> (ffs(tidhmask) - 1)))
		goto bad;
	while (count-- > 0)
		if (upkstralloc(mp, &buf))
			goto bad;
		else
			PVM_FREE(buf);

	/* make a wait channel for the task */

	wp = wait_new(WT_DELHOST);
	wp->wa_tid = tp->t_tid;
	wp->wa_on = hosts->ht_hosts[hosts->ht_master]->hd_hostpart;
	wp->wa_mesg = replymessage(mp);

	/* forward message to master pvmd */

	mp->m_ref++;
	mp->m_src = pvmmytid;
	mp->m_dst = hosts->ht_hosts[hosts->ht_master]->hd_hostpart | TIDPVMD;
	mp->m_tag = DM_DELHOST;
	mp->m_wid = wp->wa_wid;
	sendmessage(mp);
	return 0;

bad:
	pvmlogprintf("tm_delhost() from t%x bad msg format\n", mp->m_src);
	return 0;
}


/*	tm_mca()
*
*	Task announces multicast address list for subsequent message.
*
*	TM_MCA
*	call {
*		int count			// number of addresses
*		int tids[count]		// addresses
*	}
*	// No Return
*/

int
tm_mca(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	struct mca *mcap;			/* mca descriptor */
	int ndst;					/* num of dst tids */
	int *dsts;					/* dst tids */
	int tid;
	int i, j;

	/*
	* unpack list of dst tids from message (expect it to be sorted).
	* discard tids to nonexistent foreign hosts.
	*/

	mcap = mca_new();
	mcap->mc_tid = TIDGID | tp->t_tid;
	upkint(mp, &ndst);
	dsts = TALLOC(ndst, int, "dsts");
	for ( i=0, j=0 ; i < ndst ; i++ ) {
		upkuint(mp, &tid);
		if ((tid & tidhmask) == myhostpart || tidtohost(hosts, tid))
			dsts[j++] = tid;
	}
	ndst = j;

	if (ndst < 1)
		goto noguys;

	/*
	* send DM_MCA messages containing tids to destination hosts
	* make list of destination hosts for us
	*/

	mcap->mc_dsts = TALLOC(ndst, int, "mcal");	/* XXX cheat, too much space */
	mcap->mc_ndst = 0;

	for (j = 0; j < ndst; ) {
		i = j;
		tid = dsts[i] & tidhmask;
		while (++j < ndst && tid == (dsts[j] & tidhmask)) ;
		mp = mesg_new(0);
		mp->m_dst = (tid |= TIDPVMD);
		mp->m_tag = DM_MCA;
		pkint(mp, mcap->mc_tid);
		pkint(mp, j - i);
		while (i < j)
			pkint(mp, dsts[i++]);
		sendmessage(mp);
		mcap->mc_dsts[mcap->mc_ndst++] = tid;
	}

noguys:
	PVM_FREE(dsts);

	/*
	* tag task descriptor with mca desc and send mca back to task
	*/

	tp->t_mca = mcap;

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("tm_mca() made mca %x for t%x\n",
				mcap->mc_tid, tp->t_tid);
	}
	return 0;
}


/*	tm_notify()
*
*	Task wants to be notified when an event happens.
*
*	TM_NOTIFY
*	call {
*		int what			// event type and flags
*		int ctx				// message context to use in reply
*		int tag				// message tag to use in reply
*		int count			// number of addresses
*		int tids[count]		// addresses
*	}
*	// No Return
*/

int
tm_notify(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int what, flags, ctx, tag, count, tid;
	struct hostd *hp;
	struct pmsg *mp2;
	struct waitc *wp;
	int i;

	if (upkint(mp, &what)
	|| upkint(mp, &ctx)
	|| upkint(mp, &tag)
	|| upkint(mp, &count)) {
		pvmlogerror("tm_notify() bad msg format\n");
		return 0;
	}

	flags = what;
	what &= 0xff;
	switch (what) {

	/*
	* TaskExit: make a wait context that hangs out until the task is
	* cleaned up.
	*/

	case PvmTaskExit:
		for (i = 0; i < count; i++) {
			if (upkuint(mp, &tid)) {
				pvmlogerror("tm_notify() bad msg format\n");
				return 0;
			}

			if (flags & PvmNotifyCancel) {
				FORLIST (wp, waitlist, wa_link)
					if (wp->wa_kind == WT_TASKX
					&& wp->wa_on == tid
					&& wp->wa_tid == tp->t_tid
					&& wp->wa_mesg->m_ctx == ctx
					&& wp->wa_mesg->m_tag == tag)
						break;
				if (wp != waitlist) {
					sendmessage(wp->wa_mesg);
					wp->wa_mesg = 0;
					if ((hp = tidtohost(hosts, tid))
					&& hp->hd_hostpart != myhostpart)
						wp->wa_tid = 0;
					else
						wait_delete(wp);
				}

			} else {
				/* make reply message */

				mp2 = mesg_new(0);
				mp2->m_dst = tp->t_tid;
				mp2->m_ctx = ctx;
				mp2->m_tag = tag;
				mp2->m_wid = mp->m_wid;
				pkint(mp2, tid);

				/* if task doesn't exist, reply immediately */

				if (!(hp = tidtohost(hosts, tid))
				|| (hp->hd_hostpart == myhostpart && !task_find(tid)))
					sendmessage(mp2);

				else {

					/* otherwise make a wc for it */

					wp = wait_new(WT_TASKX);
					wp->wa_on = tid;
					wp->wa_tid = tp->t_tid;
					wp->wa_dep = mp->m_wid;
					wp->wa_mesg = mp2;

					/* and if not on this host, pass on the request */

					if (hp->hd_hostpart != myhostpart) {
						mp2 = mesg_new(0);
						pkint(mp2, what);
						pkint(mp2, tid);
						mp2->m_dst = hp->hd_hostpart | TIDPVMD;
						mp2->m_tag = DM_NOTIFY;
						mp2->m_wid = wp->wa_wid;
						sendmessage(mp2);
					}
				}
			}
		}
		break;

	case PvmHostDelete:
		for (i = 0; i < count; i++) {
			if (upkuint(mp, &tid)) {
				pvmlogerror("tm_notify() bad msg format\n");
				return 0;
			}
			if (flags & PvmNotifyCancel) {
				if (hp = tidtohost(hosts, tid)) {
					FORLIST (wp, waitlist, wa_link)
						if (wp->wa_kind == WT_HOSTF
						&& wp->wa_on == hp->hd_hostpart
						&& wp->wa_tid == tp->t_tid
						&& wp->wa_mesg->m_ctx == ctx
						&& wp->wa_mesg->m_tag == tag)
							break;
					if (wp != waitlist) {
						sendmessage(wp->wa_mesg);
						wp->wa_mesg = 0;
						wait_delete(wp);
					}
				}

			} else {
				mp2 = mesg_new(0);
				mp2->m_dst = mp->m_src;
				mp2->m_ctx = ctx;
				mp2->m_tag = tag;
				pkint(mp2, tid);
				if (hp = tidtohost(hosts, tid)) {
					wp = wait_new(WT_HOSTF);
					wp->wa_tid = tp->t_tid;
					wp->wa_on = hp->hd_hostpart;
					wp->wa_mesg = mp2;

				} else {
					sendmessage(mp2);
				}
			}
		}
		break;

	case PvmHostAdd:

		FORLIST (wp, waitlist, wa_link)
			if (wp->wa_kind == WT_HOSTA
			&& wp->wa_tid == tp->t_tid
			&& wp->wa_mesg->m_ctx == ctx
			&& wp->wa_mesg->m_tag == tag)
				break;

	/* if cancelling, delete possible existing waitc */

		if (count == 0 || (flags & PvmNotifyCancel)) {
			if (wp != waitlist)
				wait_delete(wp);

	/* otherwise, update existing waitc or create new one */
		} else {
			if (wp == waitlist) {
				wp = wait_new(WT_HOSTA);
				wp->wa_tid = tp->t_tid;
				wp->wa_on = tp->t_tid;
				wp->wa_mesg = mesg_new(0);
				wp->wa_mesg->m_dst = tp->t_tid;
				wp->wa_mesg->m_ctx = ctx;
				wp->wa_mesg->m_tag = tag;
			}
			wp->wa_count = count;
		}
		break;

	default:
		pvmlogprintf("tm_notify() unknown what=%d\n", what);
		break;
	}
	return 0;
}


/*	tm_mstat()
*
*	Task wants status of a host.
*
*	TM_MSTAT
*	call {
*		string hostname
*	}
*	ret {
*		int status
*	}
*/

int
tm_mstat(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	char *name;
	struct hostd *hp;
	struct waitc *wp;

	if (upkstralloc(mp, &name)) {
		pvmlogerror("tm_mstat() bad msg format\n");
		return 0;
	}

	hp = nametohost(hosts, name);

	PVM_FREE(name);

	if (!hp) {
		mp = replymessage(mp);
		pkint(mp, PvmNoHost);
		sendmessage(mp);
		return 0;
	}

	wp = wait_new(WT_MSTAT);
	wp->wa_tid = tp->t_tid;
	wp->wa_on = hp->hd_hostpart;
	wp->wa_mesg = replymessage(mp);

	mp = mesg_new(0);
	mp->m_dst = hp->hd_hostpart | TIDPVMD;
	mp->m_tag = DM_PSTAT;
	mp->m_wid = wp->wa_wid;
	pkint(mp, hp->hd_hostpart | TIDPVMD);
	sendmessage(mp);
	return 0;
}


/*	tm_db()
*
*	Task wants a database op.
*
*	TM_DB
*	call {
*		int opcode		// insert, delete, lookup
*		string name
*		int index
*		int data		// if 'insert'
*	}
*	ret {
*		int index		// and status
*		int data		// if 'lookup'
*	}
*/

int
tm_db(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	struct waitc *wp;

	wp = wait_new(WT_DB);
	wp->wa_tid = tp->t_tid;
	wp->wa_on = hosts->ht_hosts[hosts->ht_master]->hd_hostpart;
	wp->wa_mesg = replymessage(mp);

	mp->m_ref++;
	mp->m_src = pvmmytid;
	mp->m_dst = hosts->ht_hosts[hosts->ht_master]->hd_hostpart | TIDPVMD;
	mp->m_tag = DM_DB;
	mp->m_wid = wp->wa_wid;
	sendmessage(mp);
	return 0;
}


/*	tm_sched()
*
*	Task wants to register as scheduler.
*
*	TM_SCHED
*	call {
*	}
*	ret {
*		int oldschedtid
*		int tid			// of master pvmd
*		string name
*		string arch
*		int speed
*		int dsig
*	}
*/

int
tm_sched(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	struct pmsg *mp2;
	struct hostd *hp;

	mp2 = replymessage(mp);
	pkint(mp2, pvmschedtid);
	hp = hosts->ht_hosts[hosts->ht_master];
	pkint(mp2, hp->hd_hostpart);
	pkstr(mp2, hp->hd_name);
	pkstr(mp2, hp->hd_arch ? hp->hd_arch : "");
	pkint(mp2, hp->hd_speed);
	pkint(mp2, hp->hd_dsig);
	sendmessage(mp2);

	if (pvmdebmask & PDMSCHED) {
		pvmlogprintf("tm_sched() old t%x new t%x\n", pvmschedtid, tp->t_tid);
	}
	pvmschedtid = tp->t_tid;
	tp->t_flag |= TF_ISSCHED;
	return 0;
}


/*	tm_tasker()
*
*	Task registers as local task starter.
*
*	TM_TASKER
*	call {
*		int regme	// true to register, false to unregister
*	}
*	ret {
*		int code	// 0 if okay, else error
*	}
*/

int
tm_tasker(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int regme;
	struct pmsg *mp2;

	if (upkint(mp, &regme)) {
		pvmlogerror("tm_tasker() bad msg format\n");
		return 0;
	}
	mp2 = replymessage(mp);

	if (regme) {
		if (taskertid) {
			pkint(mp2, PvmAlready);

		} else {
			taskertid = tp->t_tid;
			tp->t_flag |= TF_ISTASKER;
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("tm_tasker() register t%x \n", tp->t_tid);
			}
			pkint(mp2, 0);
		}

	} else {
		if (taskertid == tp->t_tid) {
			taskertid = 0;
			tp->t_flag &= ~TF_ISTASKER;
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("tm_tasker() unregister t%x \n", tp->t_tid);
			}
			pkint(mp2, 0);

		} else {
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("tm_tasker() t%x tries to unregister?\n",
						tp->t_tid);
			}
			pkint(mp2, PvmNoTask);
		}
	}
	sendmessage(mp2);
	return 0;
}


/*	tm_hoster()
*
*	Task registers as slave pvmd starter.
*
*	TM_HOSTER
*		int regme	// true to register, false to unregister
*	call {
*	}
*	ret {
*		int code	// 0 if okay, else error
*	}
*/

int
tm_hoster(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int regme;
	struct pmsg *mp2;

	if (upkint(mp, &regme)) {
		pvmlogerror("tm_hoster() bad msg format\n");
		return 0;
	}

	mp2 = replymessage(mp);

	/* always wear 2 condoms, who knows if indices are enough... :D */
	if (hosts->ht_hosts[hosts->ht_local]->hd_hostpart
			!= hosts->ht_hosts[hosts->ht_master]->hd_hostpart) {
		pkint(mp2, PvmHostrNMstr);
		sendmessage(mp2);
		return 0;
	}

	if (regme) {
		if (hostertid) {
			pkint(mp2, PvmAlready);

		} else {
			hostertid = tp->t_tid;
			tp->t_flag |= TF_ISHOSTER;
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("tm_hoster() register t%x \n", tp->t_tid);
			}
			pkint(mp2, 0);
		}

	} else {
		if (hostertid == tp->t_tid) {
			hostertid = 0;
			tp->t_flag &= ~TF_ISHOSTER;
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("tm_hoster() unregister t%x \n", tp->t_tid);
			}
			pkint(mp2, 0);

		} else {
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("tm_hoster() t%x tries to unregister?\n",
						tp->t_tid);
			}
			pkint(mp2, PvmNoTask);
		}
	}

	sendmessage(mp2);
	return 0;
}


/*	tm_tracer()
*
*	Task registers as task trace collector.
*
*	TM_TRACER
*	call {
*		int regme		// true to register, false to unregister
*		int tctx		// if regme true, message ctxt for trace events
*		int ttag		// if regme true, message tag for trace events
*		int octx		// if regme true, message ctxt for output events
*		int otag		// if regme true, message tag for output events
*		Pvmtmask tmask	// if regme true, trace mask for tasks
*		int tbuf		// if regme true, trace buffer size for tasks
*		int topt		// if regme true, trace options for tasks
*	}
*	ret {
*		int code	// 0 if okay, else error
*	}
*/

int
tm_tracer(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int regme;
	int tctx;
	int ttag;
	int octx;
	int otag;
	Pvmtmask tmask;
	int tbuf;
	int topt;
	struct pmsg *mp2;
	struct pmsg *mpd;
	int slconf;
	int hh;
	struct hostd *hp;
	char buf[512];

	if (upkint(mp, &regme)) {
		pvmlogerror("tm_tracer() bad msg format\n");
		return 0;
	}

	mp2 = replymessage(mp);

	slconf = 0;

	if (regme) {
		if (pvmtracer.trctid) {
			pkint(mp2, PvmAlready);

		} else {
			tp->t_flag |= TF_ISTRACER;
			upkint(mp, &tctx);
			upkint(mp, &ttag);
			upkint(mp, &octx);
			upkint(mp, &otag);
			upkstr(mp, tmask, TEV_MASK_LENGTH);
			upkint(mp, &tbuf);
			upkint(mp, &topt);
			pvmtracer.trctid = tp->t_tid;
			pvmtracer.trcctx = tctx;
			pvmtracer.trctag = ttag;
			pvmtracer.outtid = tp->t_tid;
			pvmtracer.outctx = octx;
			pvmtracer.outtag = otag;
			BCOPY(tmask,pvmtracer.tmask,TEV_MASK_LENGTH);
			pvmtracer.trcbuf = tbuf;
			pvmtracer.trcopt = topt;
			slconf++;
			if (pvmdebmask & PDMTRACE) {
				pvmlogprintf("tm_tracer() register t%x \n", tp->t_tid);
			}
			pkint(mp2, 0);
		}

	} else {
		if (pvmtracer.trctid == tp->t_tid) {
			tp->t_flag &= ~TF_ISTRACER;
			pvmtracer.trctid = 0;
			pvmtracer.trcctx = 0;
			pvmtracer.trctag = 0;
			pvmtracer.outtid = 0;
			pvmtracer.outctx = 0;
			pvmtracer.outtag = 0;
			TEV_MASK_INIT(pvmtracer.tmask);
			pvmtracer.trcbuf = 0;
			pvmtracer.trcopt = 0;
			slconf++;
			if (pvmdebmask & PDMTRACE) {
				pvmlogprintf("tm_tracer() unregister t%x \n",
					tp->t_tid);
			}
			pkint(mp2, 0);

		} else {
			if (pvmdebmask & PDMTRACE) {
				pvmlogprintf("tm_tracer() t%x tries to unregister?\n",
						tp->t_tid);
			}
			pkint(mp2, PvmNoTask);
		}
	}

	/* Update slave pvmds */
	if ( slconf ) {
		for (hh = hosts->ht_last; hh > 0; hh--) {
			if (hp = hosts->ht_hosts[hh]) {
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

	/* Send response to requesting task */
	sendmessage(mp2);

	return 0;
}


/*	tm_hostsync()
*
*	Task wants clock sample from host.
*
*	TM_HOSTSYNC
*	call {
*		int where	// pvmd tid
*	}
*	ret {
*		int status	// 0 or error code
*		int sec		// time of day clock
*		int usec
*	}
*/

int
tm_hostsync(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int where;
	struct hostd *hp;
	struct pmsg *mp2;
	struct waitc *wp;

	if (upkuint(mp, &where)) {
		pvmlogerror("tm_hostsync() bad msg format\n");
		return 0;
	}

	mp2 = replymessage(mp);

	if (!(hp = tidtohost(hosts, where))) {
		pkint(mp2, PvmNoHost);
		sendmessage(mp2);
		return 0;
	}

	wp = wait_new(WT_HOSTSYNC);
	wp->wa_tid = mp->m_src;
	wp->wa_on = hp->hd_hostpart;
	wp->wa_mesg = mp2;

	mp2 = mesg_new(0);
	mp2->m_dst = where | TIDPVMD;
	mp2->m_tag = DM_HOSTSYNC;
	mp2->m_wid = wp->wa_wid;

	sendmessage(mp2);
	return 0;
}


/*	tm_setopt()
*
*	Set task parameters
*
*	TM_SETOPT
*	call {
*		{
*			int what		// which option
*			string value	// option value
*		} []				// implied
*	}
*	ret {
*/

int
tm_setopt(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int what;
	char *val;
	int x;

	while (!upkint(mp, &what) && !upkstralloc(mp, &val)) {
		switch (what) {

		case TS_OUTTID:
			x = pvmxtoi(val);
			change_output(tp, x, tp->t_outctx, tp->t_outtag);
			break;

		case TS_OUTCTX:
			x = pvmxtoi(val);
			change_output(tp, tp->t_outtid, x, tp->t_outtag);
			break;

		case TS_OUTTAG:
			x = pvmxtoi(val);
			change_output(tp, tp->t_outtid, tp->t_outctx, x);
			break;

		case TS_TRCTID:
			x = pvmxtoi(val);
			change_trace(tp, x, tp->t_trcctx, tp->t_trctag);
			break;

		case TS_TRCCTX:
			x = pvmxtoi(val);
			change_trace(tp, tp->t_trctid, x, tp->t_trctag);
			break;

		case TS_TRCTAG:
			x = pvmxtoi(val);
			change_trace(tp, tp->t_trctid, tp->t_trcctx, x);
			break;

		default:
			pvmlogprintf("tm_setopt() ? option %d val <%s>\n", what, val);
			break;
		}
		if (val)
			PVM_FREE(val);
	}
	mp = replymessage(mp);
	sendmessage(mp);
	return 0;
}


/*	change_output()
*
*	Task sets its output sink parameters.
*	If new values are different we have to send close and open
*	messages, because the collector won't know what's going on.
*/

int
change_output(tp, outtid, outctx, outtag)
	struct task *tp;
	int outtid, outctx, outtag;
{
	struct pmsg *mp;

	if (tp->t_outtid != outtid
	|| tp->t_outctx != outctx
	|| tp->t_outtag != outtag) {
		if (tp->t_outtid > 0) {
			mp = mesg_new(0);
			mp->m_dst = tp->t_outtid;
			mp->m_ctx = tp->t_outctx;
			mp->m_tag = tp->t_outtag;
			pkint(mp, tp->t_tid);
			pkint(mp, TO_EOF);
			sendmessage(mp);
		}
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("t%x changes output from <t%x %d %d> to <t%x %d %d>\n",
					tp->t_tid,
					tp->t_outtid, tp->t_outctx, tp->t_outtag,
					outtid, outctx, outtag);
		}
		tp->t_outtid = outtid;
		tp->t_outctx = outctx;
		tp->t_outtag = outtag;
		if (tp->t_outtid > 0) {
			mp = mesg_new(0);
			mp->m_dst = tp->t_outtid;
			mp->m_ctx = tp->t_outctx;
			mp->m_tag = tp->t_outtag;
			pkint(mp, tp->t_tid);
			pkint(mp, TO_SPAWN);
			pkint(mp, tp->t_ptid);
			sendmessage(mp);

			mp = mesg_new(0);
			mp->m_dst = tp->t_outtid;
			mp->m_ctx = tp->t_outctx;
			mp->m_tag = tp->t_outtag;
			pkint(mp, tp->t_tid);
			pkint(mp, TO_NEW);
			pkint(mp, tp->t_ptid);
			sendmessage(mp);
		}
	}
	return 0;
}


/*	tm_getopt()
*
*	Connect message for shared memory apps to get trace & output parameters.
*	XXX hack, will be merged into something else in v3.4.
*
*	TM_GETOPT
*	call { }
*	ret {
*		int tid				// task tid
*		int ptid			// parent tid
*		int outtid			// output dst
*		int outctx
*		int outtag
*		int trctid			// trace dst
*		int trcctx
*		int trctag
*		int udpmax
*		int nativecode
*		string inaddr
*		int schedtid		// scheduler tid
*	}
*/

int
tm_getopt(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	mp = replymessage(mp);
	pkint(mp, tp->t_tid);
	pkint(mp, tp->t_ptid);
	pkint(mp, tp->t_outtid);
	pkint(mp, tp->t_outctx);
	pkint(mp, tp->t_outtag);
	pkint(mp, tp->t_trctid);
	pkint(mp, tp->t_trcctx);
	pkint(mp, tp->t_trctag);
	pkint(mp, pvmudpmtu);
	pkint(mp, pvmmydsig);
	pkstr(mp, inadport_hex(&(hosts->ht_hosts[hosts->ht_local]->hd_sad)));
	pkint(mp, pvmschedtid);
	mp->m_flag |= MM_PRIO;  /* set priority handling for this message */
	sendmessage(mp);

#if defined(SHMEM)

	/* now change connection to TF_SHMCONN and try send packets that have
		gathered in this tasks sendq */
	tp->t_flag &= ~TF_PRESHMCONN;  
	tp->t_flag |= TF_SHMCONN;
	
	shm_wrt_pkts(tp); /* write  any waiting packets */
	
#endif
	return 0;
}


/*	change_trace()
*
*	Task sets its trace sink parameters.
*	If new values are different we have to send close and open
*	messages, because the collector won't know what's going on.
*/

int
change_trace(tp, trctid, trcctx, trctag)
	struct task *tp;
	int trctid, trcctx, trctag;
{
	struct pmsg *mp;

	if (tp->t_trctid != trctid
			|| tp->t_trcctx != trcctx
			|| tp->t_trctag != trctag) {
		if (tp->t_trctid > 0) {
			tev_send_endtask(
				tp->t_trctid, tp->t_trcctx, tp->t_trctag,
				tp->t_tid, tp->t_status,
				tp->t_utime.tv_sec, tp->t_utime.tv_usec,
				tp->t_stime.tv_sec, tp->t_stime.tv_usec );
		}
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf( "t%x changes trace from <t%x %d %d> to <t%x %d %d>\n",
					tp->t_tid,
					tp->t_trctid, tp->t_trcctx, tp->t_trctag,
					trctid, trcctx, trctag);
		}
		tp->t_trctid = trctid;
		tp->t_trcctx = trcctx;
		tp->t_trctag = trctag;
		if (tp->t_trctid > 0) {
			tev_send_spntask(
				tp->t_trctid, tp->t_trcctx, tp->t_trctag,
				tp->t_tid, tp->t_ptid );
			tev_send_newtask(
				tp->t_trctid, tp->t_trcctx, tp->t_trctag,
				tp->t_tid, tp->t_ptid, 0 /* XXX lie */,
				tp->t_a_out ? tp->t_a_out : "" );
		}
	}
	return 0;
}


/*	tm_context()
*
*	Assign task a communication context, or free one in use
*
*	TM_CONTEXT
*	call {
*		int cid			// to free, or zero if requesting one
*	}
*	ret {
*		int cid			// new cid or status
*	}
*/

int
tm_context(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	struct pmsg *mp2;
	struct ccon *cp;
	int cid;
	int cc;

	mp2 = replymessage(mp);
	if (upkuint(mp, &cid)) {
		cc = PvmBadParam;

	} else {
		if (cid == 0) {
			if (cp = ccon_new(tp))
				cc = cp->c_cid;
			else
				cc = PvmOutOfRes;

		} else {
			cc = PvmNotFound;
			if (tp->t_ccs) {
				FORLIST (cp, tp->t_ccs, c_peer) {
					if (cp->c_cid == cid) {
						LISTDELETE(cp, c_link, c_rlink);
						LISTDELETE(cp, c_peer, c_rpeer);
						PVM_FREE(cp);
						cc = 0;
						break;
					}
				}
			}
		}
	}
	pkint(mp2, cc);
	sendmessage(mp2);
	return 0;
}


/*	tm_shmconn()
 *
 *	Task connecting to pvmd under the shmem system.
 *	This is used to tie the task to the pvmd socket into the correct
 *	task struct.  This socket is a one-way only socket that is used
 *	by the task to kick the daemon when the task has put a message
 *	for it in its shared memory inbox segment.  The socket is also
 *	used to catch when the task dies....
 *	
 *	There is no d-auth file checking.
 *	Only data passed is the task's pid so we can match it with our
 *	own table.
 *
 *	TM_SHMCONN()
 *	call {
 *		int unixpid			// real pid
 *	}
 *	ret {
 *	}
 */

int
tm_shmconn(tp, mp)
	struct task *tp;
	struct pmsg *mp;	/* remember no return message */
{
	int tid;			
	int pid;						/* real pid of task */
	int ppid;						/* probable pid */
	struct task *tp2;				/* to search for real context */
	struct pmsg *mp2;
	int cc;
	char c;
	int dummy_true = 1;				/* not so harsh "always true"? */

	if ( upkint( mp, &ppid ) ) {
		pvmlogerror(
				"tm_shmconn() bad msg format. Missing Unix pid??\n" );
		goto bail;
	}

#ifndef	SHMEM
	if ( dummy_true ) {
		pvmlogerror( "tm_shmconn() shmem conn on non shmem sys? " );
		pvmlogerror( "Bit bucket...\n" );
		goto bail;
	}
#endif
	
	/*
	*	Task should already have a correct task struct.
	*	Because TM_SHMCONN is passed after the shmem connect phase
	*	in pvmbeatask() in lpvmshmem.c
	*/

	if ( tp2 = task_findpid( ppid ) )
	{
		if ( pvmdebmask & PDMTASK ) {
			pvmlogprintf(
					"tm_shmconn() reconnect task t%x via sockets\n",
					tp2->t_tid);
		}
		/* tp->t_sched = tp2->t_sched; */
	}
	else
	{
		pvmlogerror( "tm_shmconn() conn from non shmem connd task?\n");
		goto bail;		
	}

	/* 
	 * tp2 is now the real task.... 
	 * So we now update it with the correct socket descripter and then
	 * mark the shadow one as dead.
	 *
	 */

	tp2->t_sock = tp->t_sock;

	tp->t_sock = -1;	/* tp will be freed by loclinput() */
	tp->t_rxp = 0;
	tp->t_flag |= TF_CLOSE; /* stop work() complaining - dead socket? */

	/*
	 *	kick it in the butt; it's ready to go
	 */

	/* We are happy to mark it as a shared memory connected process */
	/* i.e. please use shmem (mpp_output) not my socket to talk to me */

	/* the PRESHMCONN flag indicates that messages with MM_PRIO can be
		sent over shared memory, but no others.  tm_getopt will
		set the flag to SHMCONN
	*/
	tp2->t_flag |= TF_PRESHMCONN;

	return 0;

bail:
	/* i.e. kill it! */
	tp->t_sock = -1;	/* tp will be freed by loclinput() */
	tp->t_rxp = 0;
	tp->t_flag |= TF_CLOSE;
	return 0;
}



static char rcsid[] =
	"$Id: pvmdshmem.c,v 1.23 2009/01/30 21:22:46 pvmsrc Exp $";

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
 * pvmdshmem.c
 *
 * Shared-memory MPP interface.
 *
 * $Log: pvmdshmem.c,v $
 * Revision 1.23  2009/01/30 21:22:46  pvmsrc
 * Fixed up insecure usage of fopen() for creating/writing/appending files.
 * 	- replace with open(...O_EXCL...) and fdopen()...
 * (Spanker=kohl)
 *
 * Revision 1.22  2005/08/22 15:13:21  pvmsrc
 * Added #include <pvmtev.h> for #include global.h...  :-Q
 * 	- submitted by Skipper Hartley <charles.l.hartley@bankofamerica.com>
 * (Spanker=kohl)
 *
 * Revision 1.21  2004/01/14 18:51:02  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.20  2002/02/21 23:18:41  pvmsrc
 * Added new (not to be documented!) PVM_MAX_TASKS env var support.
 * 	- for Mahir Lokvancic <mahir@math.ufl.edu>.
 * 	- forcefully limits the number of tasks that can attach to a
 * 		pvmd, required on Solaris in rare circumstances when hard
 * 		FD_SETSIZE limit is reached, and all hell breaks loose...
 * 	- check return for task_new() call, can now produce NULL ptr,
 * 		indicating PvmOutOfRes...
 * (Spanker=kohl)
 *
 * Revision 1.19  2001/09/25 21:21:04  pvmsrc
 * Yanked "char *pvmgettmp();" decl - now in pvm3.h...
 * (Spanker=kohl)
 *
 * Revision 1.18  2000/02/10 20:45:33  pvmsrc
 * Replaced hard-coded /tmp usage.
 * 	- use pvmgettmp() routine now to determine PVM temp dir.
 * (Spanker=kohl)
 *
 * Revision 1.17  1999/07/08 19:00:11  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.16  1998/08/13  18:31:15  pvmsrc
 * Altered SUNMP to use test and set operations with semaphores
 * 		for page locking instead of MUTEX and cond vars.
 * 	Changes are mainly in pvmshmem.h, with lots of #ifdefs changes.
 * 	Makefile altered to use the PLOCKFILE to indicate the Page Locking
 * 		INLINE code used (from SUNMP.conf).
 * 	Some changes effect AIX MP versions which still use conditional
 * 		variables and may change to semaphores soon.
 * (Spanker=fagg)
 *
 * Revision 1.15  1997/11/04  23:21:43  pvmsrc
 * Added SYSVSTR stuff.
 * (Spanker=kohl)
 *
 * Revision 1.14  1997/07/02  20:27:32  pvmsrc
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
 * Revision 1.13  1997/06/27  20:54:41  pvmsrc
 * Allow forked process to be caught by daemon
 * 	when its not ignoring SIGCLD.
 *
 * Revision 1.12  1997/06/25  22:09:34  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.11  1997/06/24  20:39:19  pvmsrc
 * Eliminated unnecessary global externs.
 *
 * Revision 1.10  1997/06/16  13:42:10  pvmsrc
 * Upated forkexec arg list.
 *
 * Revision 1.9  1997/06/02  13:50:03  pvmsrc
 * Added missing #include host.h for waitc.h.
 *
 * Revision 1.8  1997/06/02  13:26:43  pvmsrc
 * Changed mpp_input so that spawned processes can be claimed after
 * 	exiting.  Previously they were claimed as their shadow t0 died
 * 	and set a flag in work() that would get caught leading to
 * 	mpp_dredge() finding them sometime later...
 *
 * Revision 1.7  1997/05/21  16:01:53  pvmsrc
 * Updated ifdefs to include AIX4MP arch type.
 *
 * Revision 1.6  1997/05/19  15:03:01  pvmsrc
 * pvmfrgsiz is now set according to memory page size and msg header
 * 	lengths instead of just the UDP host-host packet size!
 * Mesg headers are set correctly in mpp_input / mpp_output to include
 * 	contexts etc
 *
 * Revision 1.5  1997/04/25  19:17:29  pvmsrc
 * Mashed to make it work within 3.4 new messaging system. Added:
 * 	-	correct includes
 * 	-	changed ph_cod's to ph_tag's and pk_cod -> pk_tag
 * 	-	add duplicate of pvmdunix ppi_kill()
 *
 * Revision 1.4  1997/01/28  19:27:23  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/25  13:58:01  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.2  1996/10/24  21:04:50  pvmsrc
 * Moved #include of "global.h" down below other headers:
 * 	- need to have all of the structures / types declared before
 * 		the globals can be declared...
 *
 * Revision 1.1  1996/09/23  23:44:33  pvmsrc
 * Initial revision
 *
 * Revision 1.20  1995/11/02  16:31:23  manchek
 * skip over stale packets from dead tasks in mpp_input
 *
 * Revision 1.19  1995/09/05  19:24:05  manchek
 * mpp_input copies all pages for now (in case sender task exits)
 *
 * Revision 1.18  1995/07/25  17:35:32  manchek
 * mpp_probe cancels retry to a task if not ready.
 * mpp_output returns success code
 *
 * Revision 1.17  1995/07/24  21:48:25  manchek
 * mpp_output puts pkts in order (at the end) of ovfpkts.
 * mpp_probe consumes ovfpkts in order to retry
 *
 * Revision 1.16  1995/07/24  19:22:33  manchek
 * message, frag headers passed in inbox shmpkhdr instead of databuf
 * changes in mpp_input, mpp_output.
 * removed mpp_mcast - wasn't called
 *
 * Revision 1.15  1995/07/12  01:12:14  manchek
 * do nothing in mpp_free if tid is zero.
 * peer_detach now frees peer struct.
 * mpp_dredge can reclaim pidtid slot immediately if task not registered
 *
 * Revision 1.14  1995/07/05  16:21:40  manchek
 * added ST_FINISH to pidtid_dump
 *
 * Revision 1.13  1995/07/05  16:16:29  manchek
 * added mpp_dredge function to skim pidtid table for zombies and call
 * task_cleanup and task_free.
 * mpp_free (called from task_free) now reclaims pidtid table entry
 *
 * Revision 1.12  1995/07/03  19:55:47  manchek
 * hellish cleanup of comments and formatting.
 * removed POWER4 ifdefs.
 * removed removeshm().
 * added mpp_setstatus() and pidtid_dump()
 *
 * Revision 1.11  1995/06/28  15:50:57  manchek
 * added arg to peer_conn calls
 *
 * Revision 1.10  1995/06/19  17:45:01  manchek
 * inc refcount before signalling in mpp_output
 *
 * Revision 1.9  1995/06/02  16:21:10  manchek
 * fixed references to detached memory segments
 *
 * Revision 1.8  1995/05/22  19:51:37  manchek
 * added ifdefs for RS6KMP
 *
 * Revision 1.7  1995/05/18  17:22:25  manchek
 * need to export pvminbox and myshmbufid
 *
 * Revision 1.6  1995/05/17  16:41:29  manchek
 * changed global mytid to pvmmytid.
 * changed inbox to pvminbox and mybufid to myshmbufid.
 * added support for CSPP shared memory.
 * unset TF_FORKD flag if task doesn't reconnect with expected pid
 *
 * Revision 1.5  1995/02/06  22:42:01  manchek
 * new function mpp_setmtu, called before slave_config
 *
 * Revision 1.4  1995/02/01  21:35:06  manchek
 * added nenv and envp args to mpp_load, which passes them to forkexec
 *
 * Revision 1.3  1994/11/08  15:35:07  manchek
 * shared memory damage control
 *
 */


#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>
#ifdef IMA_SYMM
#include <sys/file.h>		/* XXX for open(); change to fcntl.h in ptx? */
#include <parallel/parallel.h>
#endif
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#include <pvm3.h>
#include <pvmproto.h>
#include "pvmalloc.h"
#include "pvmdabuf.h"
#include "pkt.h"
#include "task.h"
#include "listmac.h"
#include "pvmshmem.h"
#include "bfunc.h"
#include "host.h"
#include "waitc.h"
#include <pvmtev.h>
#include "global.h"

#ifndef max
#define max(a,b)    ((a)>(b)?(a):(b))
#endif

#ifndef min
#define min(a,b)    ((a)<(b)?(a):(b))
#endif

char *getenv();


extern int pvmdebmask;			/* from pvmd.c */
extern int pvm_useruid;			/* from pvmd.c */
extern int pvmmytid;			/* from pvmd.c */
extern int pvmmydsig;			/* from pvmd.c */
extern int pvmudpmtu;			/* from pvmd.c */
extern int pvmmyupid;			/* from pvmd.c */
extern struct peer *peers;		/* from pvmshmem.c */


/***************
 **  Globals  **
 **           **
 ***************/

char *outmsgbuf = 0;			/* outgoing message buffer */
int outbufsz = 0;				/* size of outgoing msg buffer */
int nbufsowned = 0;				/* num shared frags held by us */
int pgsz = 0;					/* page size */
int pvmpgsz = 0;				/* PVM virtual page size */
char *infopage = 0;				/* proto, NDF, pid-tid table */
struct pidtid *pidtids = 0;		/* pid -> tid table */
int maxpidtid = 0;				/* size of pid-tid table */
int shmbufsiz = 0;				/* shared-memory buffer size */
int myshmbufid = -1;			/* shared-memory msg buffer ID */
char *pvminbox = 0;				/* incoming message header buffer */
extern int pvmfrgsiz;			/* From pvmd.c frag size */
								/* included here as we change it in ppi_config*/

/***************
 **  Private  **
 **           **
 ***************/

static char pvmtxt[512];		/* scratch for error log */
static int inboxsz = 0;			/* size of incoming message buffer */
static struct pkt *ovfpkts = 0;	/* packets waiting to be delivered */
static int globid = -1;			/* ID of global shared segment */


/*
 *			Buffer layout
 *		 ____________________
 *		|       inbox        | 1
 *		|____________________|
 *		|   pid->tid table   | 2
 *		|____________________|
 *		|                    | 3
 *		|      outmsgbuf     |
 *      |          .         | .
 *		|          .         | .
 *		|          .         | .
 *
 *
 * Note: tasks don't keep a pid-tid table, so their outgoing message
 * buffer starts at the second page.
 *
 * To send messages to one another, the sender puts the message header,
 * which contains its own task ID and the location of the message body
 * (expressed as an offset from the head of the segment), into the inbox
 * of the intended receiver. When the addressee is ready to accept the
 * message, it reads the message header and locates the message fragments
 * in the sender's outgoing message buffer. Access to buffers are guarded
 * locks. Tasks can read the same data simultaneously, but one must obtain
 * exclusive access to the page before it can modify the data.
 *
 * Messages to tasks on other hosts are routed by pvmd.
 */


int
ppi_config(argc, argv)
	int argc;
	char **argv;
{
	pgsz = sysconf(_SC_PAGESIZE);
	pvmudpmtu = pgsz - PVMPAGEHDR; /* XXX yuck host to host limited */
	pvmfrgsiz = pvmudpmtu;	/* host-task limit.. not same as UDP host to host */
					/* except h-h is not same as it should be, see above */
	return 0;
}


/*	ppi_init()
*
*	Create our shared memory segment, initialize stuff, etc.
*
*	XXX shouldn't just return if something goes wrong
*/

void
ppi_init()
{
	struct pidtidhdr *pvminfo;
	char *pvmtmp = pvmgettmp();
	char *p;
	int key;
	int fd;

#ifdef LOG
	char fname[32];
	FILE *logfp;
#ifdef IMA_CSPP
	int scid = get_scid();
	if (scid > 1)
		sprintf(fname, "%s/pvmt.%d.%d", pvmtmp, pvm_useruid, scid);
	else
#else
	sprintf(fname, "%s/pvmt.%d", pvmtmp, pvm_useruid);
#endif
	if ((fd = open( fname, O_RDWR|O_CREAT|O_EXCL|O_TRUNC, 0600 )) < 0) {
	}
	logfp = fdopen( fd, "w" );
	fclose(logfp);
#endif	/*LOG*/

	pvmpgsz = FRAGPAGE*pgsz;
	inboxsz =
		(INBOXPAGE*pgsz - sizeof(struct msgboxhdr))/sizeof(struct shmpkhdr);
	if (!(p = getenv("PVMBUFSIZE")) || !(shmbufsiz = strtol(p, (char**)0, 0)))
		shmbufsiz = SHMBUFSIZE;

	key = pvmshmkey(0);
	if ((myshmbufid = shmget((key_t)key, shmbufsiz, IPC_CREAT|PERMS)) == -1) {
		pvmlogperror("ppi_init() can't create msg buffer");
		pvmbailout(0);
		return;
	}
#ifdef IMA_CSPP
	if ((pvminbox = (char *)shm_search(myshmbufid)) == (char *)-1L)
#else
	if ((pvminbox = (char *)shmat(myshmbufid, 0, 0)) == (char *)-1L)
#endif
	{
		pvmlogperror("ppi_init() can't attach msg buffer");
		shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0);
		myshmbufid = -1;
		pvmbailout(0);
		return;
	}

	infopage = pvminbox + INBOXPAGE*pgsz;
	outmsgbuf = infopage + pgsz;
	if (!(outbufsz = (shmbufsiz - INBOXPAGE*pgsz - pgsz)/pvmpgsz)) {
		pvmlogerror("ppi_init() SHMBUFSIZE too small!");
		shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0);
		myshmbufid = -1;
		pvmbailout(0);
		return;
	}
	nbufsowned = 0;

	msgbufinit(pvminbox);
#ifndef IMA_KSR1
	PAGEINITLOCK(&((struct shmpghdr *)infopage)->pg_lock);
#endif
	((struct shmpghdr *)infopage)->pg_ref = 0;
	pvminfo = (struct pidtidhdr *)(infopage + PVMPAGEHDR);
	pvminfo->i_proto = TDPROTOCOL;
	pvminfo->i_dsig = pvmmydsig;
	pvminfo->i_next = 0;
	pvminfo->i_bufsiz = shmbufsiz;
	pvminfo->i_dpid = pvmmyupid;
	pidtids = (struct pidtid *)(pvminfo + 1);
	maxpidtid = (pgsz - sizeof(struct pidtidhdr) - PVMPAGEHDR)/sizeof(struct pidtid);
	BZERO((char *)pidtids, sizeof(struct pidtid)*maxpidtid);
	{
		int i;

		for (i = 0; i < maxpidtid; i++)
			pidtids[i].pt_stat = ST_EXIT;
	}

	peer_init();

	ovfpkts = pk_new(0);
	ovfpkts -> pk_link = ovfpkts->pk_rlink = ovfpkts;

/*
#if defined(SUN4SOL2)
	sprintf(pvmtxt, "%ld CPUs online\n", sysconf(_SC_NPROCESSORS_ONLN));
	sprintf(pvmtxt, "%ld CPUs online\n", sysconf(15));
	pvmlogerror(pvmtxt);
#endif
*/
}


/*	mpp_free()
*
*	Remove shared resources for tid.
*	Delete it from peers list (possibly removing segment and semaphore).
*	Delete its pidtid table entry.
*/

void
mpp_free(tid)
	int tid;
{
	struct peer *pp;
	struct task *tp;
	int i;

	if (pvmdebmask & (PDMTASK|PDMNODE)) {
		sprintf(pvmtxt, "mpp_free() t%x\n", tid);
		pvmlogerror(pvmtxt);
	}

	if (!tid)
		return;

	for (pp = peers->p_link; pp != peers; pp = pp->p_link) {
		if (pp->p_tid == tid) {
			peer_detach(pp);
			break;
		}
	}

	PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
	for (i = 0; i < maxpidtid; i++)
		if (pidtids[i].pt_tid == tid) {
			pidtids[i].pt_stat = ST_EXIT;
			break;
		}
	PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
}


/*	mpp_conn()
*
*	Fill in pid->tid->key table for a new task.
*	This isn't really "connection" (done by peer_conn).
*/

int
mpp_conn(tp)
	struct task *tp;
{
	int firstidx, idx;
	struct pidtidhdr *pvminfo = (struct pidtidhdr *)(infopage + PVMPAGEHDR);

	PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
	firstidx = idx = pvminfo->i_next;
	while (pidtids[idx].pt_stat != ST_EXIT) {
		if (++idx == maxpidtid)
			idx = 0;
		if (idx == firstidx) {
			PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
			pvmlogerror("mpp_conn() pidtid table full\n");
			return PvmOutOfRes;
		}
	}
	if ((firstidx = idx + 1) == maxpidtid)
		firstidx = 0;
	pvminfo->i_next = firstidx;

	/* XXX so wtf good is an almost-refcount? */
	if (((struct shmpghdr *)infopage)->pg_ref < maxpidtid)
		((struct shmpghdr *)infopage)->pg_ref++;
	pidtids[idx].pt_tid = tp->t_tid;
	pidtids[idx].pt_ptid = tp->t_ptid;
	pidtids[idx].pt_stat = ST_NOTREADY;
	pidtids[idx].pt_pid = tp->t_pid;
	pidtids[idx].pt_key = 0;
	pidtids[idx].pt_cond = 0;
	PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
	if (pvmdebmask & (PDMTASK|PDMMEM)) {
		sprintf(pvmtxt, "mpp_conn() assigning pidtid[%d] to t%x\n",
				idx, tp->t_tid);
		pvmlogerror(pvmtxt);
	}
	if (pvmdebmask & PDMTASK) {
		sprintf(pvmtxt, "mpp_conn() new task t%x\n", tp->t_tid);
		pvmlogerror(pvmtxt);
	}
	return 0;
}


void
mpp_input()
{
	int next;
	struct pkt *pp;
	struct peer *pe;
	struct task *tp;
	int tid;
	int sdr;
	int src;
	int dst;
	int len;
	char *cp, *buf;
	struct msgboxhdr *inbp;
	struct shmpkhdr *inmsgs;

	inbp = (struct msgboxhdr *)pvminbox;
	inmsgs = (struct shmpkhdr *)(inbp + 1);
	do {
		next = (inbp->mb_read + 1) % inboxsz;
		sdr = inmsgs[next].ph_sdr;
		src = inmsgs[next].ph_src;

		if (inmsgs[next].ph_dat < 0) {		/* new task */
			int ipid;

			if (!(ipid = inmsgs[next].ph_dst))
				ipid = src;
			if (!(tp = task_findpid(ipid))) {
				/* not spawned by us */
				if ((tid = tid_new()) < 0) {
					pvmlogerror("mpp_input() out of tids?\n");
					continue;
				}
				if ((tp = task_new(tid)) == NULL) {
					pvmlogerror("mpp_input() too many tasks?\n");
					continue;
				}
				task_setpid(tp, src);
				mpp_conn(tp);

			} else if (tp->t_pid != src) {
				task_setpid(tp, src);
				tp->t_flag &= ~TF_FORKD;
			}
			tp->t_flag |= TF_CONN | TF_SHM;  
			continue;
		}

		if (!(pe = peer_conn(sdr, (int *)0)) || pe == (struct peer *)-1L) {
			sprintf(pvmtxt, "mpp_input() can't connect to sender t%x\n", sdr);
			pvmlogerror(pvmtxt);
			continue;
		}
		cp = pe->p_buf + INBOXPAGE*pgsz + inmsgs[next].ph_dat;
		buf = cp - (inmsgs[next].ph_dat & (pgsz-1)) + PVMPAGEHDR;
		dst = inmsgs[next].ph_dst;
		len = inmsgs[next].ph_len;

		/*
		* must copy all packets (for now).
		* sender may exit, and we don't check if we have page references
		* into a segment before we detach.
		*/
/*
		if (TIDISHERE(dst, pvmmytid) && TIDISTASK(dst)) {
*/
			pp = pk_new(len + MAXHDR);
			pp->pk_dat += MAXHDR;
			BCOPY(cp, pp->pk_dat, len);
			da_unref(buf);
			cp = pp->pk_dat;

/*
		} else {
			pp = pk_new(0);
			pp->pk_dat = cp;
			pp->pk_buf = buf;
			pp->pk_max = pvmudpmtu;
		}
*/
		pp->pk_len = len;
		pp->pk_src = src;
		pp->pk_dst = dst;
		pp->pk_flag = inmsgs[next].ph_flag;
		pp->pk_tag = inmsgs[next].ph_tag;
		pp->pk_ctx = inmsgs[next].ph_ctx;
		pp->pk_enc = inmsgs[next].ph_enc;
		pp->pk_wid = inmsgs[next].ph_wid;
		pp->pk_crc = inmsgs[next].ph_crc;

		if (tp = task_find(src)) {
			loclinpkt(tp, pp);

			/* CL tasks caught here. forked ones caught by SIGCHLD/SIGCLD */
			if ((tp->t_flag & TF_CLOSE) && !(tp->t_flag & TF_FORKD)) { 
				task_cleanup(tp);
				task_free(tp);
			}

		} else {
			sprintf(pvmtxt, "mpp_input() from unknown task t%x\n", src);
			pvmlogerror(pvmtxt);
			pk_free(pp);
		}

	} while ((inbp->mb_read = next) != inbp->mb_last);
}


/*	mpp_output()
*
*	Send packet to a task if it's connected, otherwise queue it in
*	a list to be retried later.
*
*	Returns 0 if packet sent, else 1.
*/

int
mpp_output(tp, pp)
	struct task *tp;
	struct pkt *pp;
{
	struct peer *pe;
	int dst;
	struct shmpkhdr *dmsgs = 0;
	struct pkt *pp1, *pp2;
	struct msgboxhdr *dboxp;
	char *cp;
	int loc;
	int next;

	dst = pp->pk_dst;

	/*
	* if page is private, copy and replace it with one in shared buf
	*/

	if ((loc = pp->pk_dat - outmsgbuf) > outbufsz * pvmpgsz || loc < 0) {
		if (nbufsowned == outbufsz) {
			static int once = 1;

			if (once) {
				pvmlogerror("mpp_output() Message(s) too long for shared buffer, deadlocked.\n");
				once = 0;
			}
		}

		cp = 0;
		do {
			if (cp)
				da_unref(cp);
			cp = da_new(MAXHDR + pp->pk_len);
		} while ((loc = cp - outmsgbuf) > outbufsz*pvmpgsz || loc < 0);

		BCOPY(pp->pk_dat, cp + MAXHDR, pp->pk_len);
		pp->pk_dat = cp + MAXHDR;
		da_unref(pp->pk_buf);
		pp->pk_buf = cp;
	}

	if ((pe = peer_conn(dst, (int *)0)) && pe != (struct peer *)-1L) {
		dboxp = (struct msgboxhdr *)pe->p_buf;
		dmsgs = (struct shmpkhdr *)(dboxp + 1);
		PAGELOCK(&dboxp->mb_lock);
		if ((next = (dboxp->mb_last + 1) % inboxsz) != dboxp->mb_read) {
			if (pvmdebmask & PDMPACKET) {
				sprintf(pvmtxt,
					"mpp_output() src t%x dst t%x ff %x len %d\n",
					pp->pk_src, pp->pk_dst, pp->pk_flag & (FFSOM|FFEOM),
					pp->pk_len);
				pvmlogerror(pvmtxt);
			}
			dmsgs[next].ph_src = pp->pk_src;
			dmsgs[next].ph_dst = dst;
			dmsgs[next].ph_sdr = pvmmytid;
			dmsgs[next].ph_dat = loc;
			dmsgs[next].ph_len = pp->pk_len;
			dmsgs[next].ph_flag = pp->pk_flag & (FFSOM|FFEOM);
			dmsgs[next].ph_ctx = pp->pk_ctx;
			dmsgs[next].ph_tag = pp->pk_tag;
			dmsgs[next].ph_enc = pp->pk_enc;
			dmsgs[next].ph_wid = pp->pk_wid;
			dmsgs[next].ph_crc = pp->pk_crc;
			da_ref(pp->pk_buf);
			dboxp->mb_last = next;

			if (dboxp->mb_sleep) {

/* #if defined(IMA_SUNMP) || defined(IMA_RS6KMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP) */
#ifdef PVMUSEMUTEX
#ifdef	IMA_SUNMP
				cond_signal(&dboxp->mb_cond);
#endif
#if defined(IMA_RS6KMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP)
				pthread_cond_signal(&dboxp->mb_cond);
#endif
#else
				peer_wake(pe);

#endif	/* PVMUSEMUTEX */
				dboxp->mb_sleep = 0;
			}

			PAGEUNLOCK(&dboxp->mb_lock);
			pk_free(pp);
			return 0;

		} else
			LISTPUTBEFORE(ovfpkts, pp, pk_link, pk_rlink);
		PAGEUNLOCK(&dboxp->mb_lock);

	} else
		LISTPUTBEFORE(ovfpkts, pp, pk_link, pk_rlink);
	return 1;
}


/*	mpp_probe()
*
*	Try to send buffered packets that couldn't be delivered before.
*	Update state of task from NOTREADY to SOCKET if it has socket connection.
*	XXX shouldn't be done here, why not in loclconn.
*
*	Returns 1 if packets ready for receipt, else 0.
*/

int
mpp_probe()
{
	struct pkt *pp, *pp2, *tosend;
	int dst;
	struct task *tp;
	int hasmsg;
	struct msgboxhdr *inbp = (struct msgboxhdr *)pvminbox;
	struct pidtidhdr *pvminfo = (struct pidtidhdr *)(infopage + PVMPAGEHDR);
	int ntids, i;

	tosend = ovfpkts;

	ovfpkts = pk_new(0);
	ovfpkts -> pk_link = ovfpkts -> pk_rlink = ovfpkts;

	while ((pp = tosend->pk_link) != tosend) {
		LISTDELETE(pp, pk_link, pk_rlink);
		dst = pp->pk_dst;
		if (tp = task_find(dst)) {
			if (tp->t_sock < 0) {
				if (mpp_output(tp, pp)) {
					for (pp = tosend->pk_link; pp != tosend; pp = pp2) {
						pp2 = pp->pk_link;
						if (pp->pk_dst == dst) {
							LISTDELETE(pp, pk_link, pk_rlink);
							LISTPUTBEFORE(ovfpkts, pp, pk_link, pk_rlink);
						}
					}
				}

			} else
				pkt_to_task(tp, pp);

		} else {
			sprintf(pvmtxt, "mpp_probe() pkt from t%x to t%x scrapped",
					pp->pk_src, pp->pk_dst);
			pvmlogperror(pvmtxt);
			pk_free(pp);
		}
	}

	pk_free(tosend);

	ntids = min(maxpidtid, ((struct shmpghdr *)infopage)->pg_ref);
	for (i = 0; i < ntids; i++)
		if (pidtids[i].pt_stat == ST_NOTREADY
		&& (tp = task_find(pidtids[i].pt_tid)) && tp->t_sock >= 0)
			pidtids[i].pt_stat = ST_SOCKET;

	hasmsg = (inbp->mb_read != inbp->mb_last);
	return hasmsg;
}


/*	mpp_cleanup()
*
*	We're bailing out.  All hands on deck. Remove our shared segment and 
*   as many segments and semaphores as we can for our tasks.
*
*/

void
mpp_cleanup()
{
	struct peer *pp;
	struct shmid_ds shmds;

/*
	if (pvminbox && shmdt(pvminbox) == -1)
		pvmlogperror("mpp_cleanup() shmdt inbox");
*/

	if (myshmbufid != -1
	&& shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0) == -1)
		pvmlogperror("mpp_cleanup() shmctl IPC_RMID mybuf");

	if (peers) {
		for (pp = peers->p_link; pp != peers; pp = pp->p_link) {
			if (pp->p_buf) {
				shmdt(pp->p_buf);
				pp->p_buf = 0;
			}

			if (pp->p_shmid == -1 && pp->p_key)
				pp->p_shmid = shmget((key_t)pp->p_key, shmbufsiz, 0);
			if (pp->p_shmid != -1 &&
			shmctl(pp->p_shmid, IPC_RMID, (struct shmid_ds *)0) == -1) {
				sprintf(pvmtxt, "shmctl id=0x%x", pp->p_shmid);
				pvmlogperror(pvmtxt);
			}

#ifdef	USERECVSEMAPHORE
			if (pp->p_semid == -1 && pp->p_key)
				pp->p_semid = semget((key_t)pp->p_key, 1, PERMS);
			if (pp->p_semid != -1 && semctl(pp->p_semid, 0, IPC_RMID) == -1) {
				sprintf(pvmtxt, "semctl id=0x%x", pp->p_semid);
				pvmlogperror(pvmtxt);
			}
#endif
			pp->p_key = 0;
		}
	}
}


pidtid_dump()
{
	int i;
	char *s;

	pvmlogerror("pidtid_dump()\n");
	for (i = 0; i < maxpidtid; i++) {
		switch (pidtids[i].pt_stat) {
		case ST_EXIT:
			s = 0;
			break;

		case ST_NOTREADY:
			s = "NOTREADY";
			break;

		case ST_SHMEM:
			s = "SHMEM";
			break;

		case ST_SOCKET:
			s = "SOCKET";
			break;

		case ST_FINISH:
			s = "FINISH";
			break;

		default:
			s = "UNKNOWN";
			break;
		}

		if (s) {
			sprintf(pvmtxt, "%4d pid %d tid %x ptid %x stat %s key 0x%x",
					i,
					pidtids[i].pt_pid,
					pidtids[i].pt_tid,
					pidtids[i].pt_ptid,
					s,
					pidtids[i].pt_key);
#ifdef IMA_CSPP
			sprintf(pvmtxt + strlen(pvmtxt), " node %d\n", pidtids[i].pt_node);
#else
			strcat(pvmtxt, "\n");
#endif
			pvmlogerror(pvmtxt);
		}
	}
	return 0;
}


/*	mpp_setstatus()
*
*	Take snapshot of task conditions and set flags in task records.
*/

int
mpp_setstatus(tid)
	int tid;			/* not used right now */
{
	int i;
	struct task *tp;

	PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
	for (i = 0; i < maxpidtid; i++)
		if (pidtids[i].pt_stat != ST_EXIT)
			if (tp = task_find(pidtids[i].pt_tid)) {
				tp->t_flag &= ~TF_DEADSND;
				if (pidtids[i].pt_cond)
					tp->t_flag |= TF_DEADSND;
			}
	PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
	return 0;
}


/*	mpp_dredge()
*
*	Dredge pidtid table for zombies, with pt_stat == ST_FINISH.
*	Exit the task cleanly if it still exists and recycle the table entry.
*/

int
mpp_dredge()
{
	int i;
	struct task *tp;

	PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
	for (i = 0; i < maxpidtid; i++)
		if (pidtids[i].pt_stat == ST_FINISH) {
			if (tp = task_find(pidtids[i].pt_tid)) {
				PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
				task_cleanup(tp);
				task_free(tp);
				PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
			} else
				pidtids[i].pt_stat = ST_EXIT;
		}
	PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
	return 0;
}


/*	ppi_load()
*
*	Fork and exec new tasks.  Give them pidtid table entries.
*/

int
ppi_load(wxp)
	struct waitc_spawn *wxp;
{
	int i;
	struct task *tp;
	int err = 0;

	for (i = 0; i < wxp->w_veclen; i++) {
		if (err) {
			wxp->w_vec[i] = err;

		} else {
			if (err = forkexec(wxp->w_flags, wxp->w_argv[0], wxp->w_argv,
					wxp->w_nenv, wxp->w_env, 
					(wxp->w_instance+i), wxp->w_hosttotal, wxp->w_outof, &tp)) {
				wxp->w_vec[i] = err;

			} else {
				tp->t_ptid = wxp->w_ptid;
				tp->t_outtid = wxp->w_outtid;
				tp->t_outctx = wxp->w_outctx;
				tp->t_outtag = wxp->w_outtag;
				tp->t_trctid = wxp->w_trctid;
				tp->t_trcctx = wxp->w_trcctx;
				tp->t_trctag = wxp->w_trctag;
				tp->t_sched = wxp->w_sched;
				mpp_conn(tp);	/* XXX this can fail, dunno how to clean up */
				wxp->w_vec[i] = tp->t_tid;
			}
		}
	}
	return 0;
}


/* XXX this is just a basic copy of the pvmdunix code so we can at least compile */
int
ppi_kill(tp, signum)
    struct task *tp;
    int signum;
{
    if (tp->t_pid)
        (void)kill(tp->t_pid, signum);
    else
        pvmlogprintf("ppi_kill() signal for t%x scrapped (pid = 0)\n",
                tp->t_tid);
    return 0;
}


/* write any packets waiting the tp -> t_txq, send queue, these may 
have been stacked up before the task state changed to  TF_SHMCONN
This is currently called by tm_getopt when the state change is
marked from TF_PRESHMCONN to TF_SHMCONN
*/
int
shm_wrt_pkts(tp)
struct task * tp;
{
struct pkt *pp, *pp2, *pptmp;
struct pkt tmplist;

	
	if (tp -> t_txq)
	{
		pptmp = &tmplist;
		pptmp -> pk_link = pptmp->pk_rlink = pptmp;

		/* Take packets off of the t_txq */
		pp = tp -> t_txq -> pk_link;
		while (pp != tp -> t_txq)
		{
			pp2 = pp;
			pp = pp -> pk_link;
			LISTDELETE(pp2, pk_link, pk_rlink);
			LISTPUTBEFORE(pptmp, pp2, pk_link, pk_rlink);
		}

		pp = pptmp -> pk_link;

		while (pp != pptmp)
		{
			pp2 = pp;
			pp = pp -> pk_link;
			LISTDELETE(pp2, pk_link, pk_rlink);
			if (tp -> t_flag & TF_SHMCONN)
			{
				if ( pvmdebmask & PDMPACKET)
					pvmlogprintf("shm_wrt_pkts for %x \n", tp -> t_tid);
				mpp_output(tp, pp2);
			}
			else
				break;
		}	
	}
		
	return 0;
}
	

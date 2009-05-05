
static char rcsid[] =
	"$Id: pvmshmem.c,v 1.12 2005/08/22 15:13:25 pvmsrc Exp $";

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
 *  pvmshmem.c
 *
 *  Shared-memory stuff.
 *
 * $Log: pvmshmem.c,v $
 * Revision 1.12  2005/08/22 15:13:25  pvmsrc
 * Added #include <pvmtev.h> for #include global.h...  :-Q
 * 	- submitted by Skipper Hartley <charles.l.hartley@bankofamerica.com>
 * (Spanker=kohl)
 *
 * Revision 1.11  2004/01/14 18:51:03  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.10  1999/07/08 19:00:14  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.9  1998/08/13  18:31:12  pvmsrc
 * Altered SUNMP to use test and set operations with semaphores
 * 		for page locking instead of MUTEX and cond vars.
 * 	Changes are mainly in pvmshmem.h, with lots of #ifdefs changes.
 * 	Makefile altered to use the PLOCKFILE to indicate the Page Locking
 * 		INLINE code used (from SUNMP.conf).
 * 	Some changes effect AIX MP versions which still use conditional
 * 		variables and may change to semaphores soon.
 * (Spanker=fagg)
 *
 * Revision 1.8  1998/03/05  16:31:31  pvmsrc
 * Hack for CSPP - _touch() wasn't typed before use...  :-Q
 * (Spanker=kohl)
 *
 * Revision 1.7  1997/07/08  20:18:07  pvmsrc
 * Better logging in peer_conn()
 *
 * Revision 1.6  1997/06/25  22:11:10  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.5  1997/05/21  16:01:55  pvmsrc
 * Updated ifdefs to include AIX4MP arch type.
 *
 * Revision 1.4  1997/04/25  19:18:14  pvmsrc
 * Corrected includes.
 *
 * Revision 1.3  1997/01/28  19:27:30  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.2  1996/10/25  14:19:06  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.1  1996/09/23  23:44:40  pvmsrc
 * Initial revision
 *
 * Revision 1.21  1995/11/02  16:33:04  manchek
 * peer_conn returns -1 if peer has disconnected
 *
 * Revision 1.21  1995/11/02  16:33:04  manchek
 * peer_conn returns -1 if peer has disconnected
 *
 * Revision 1.20  1995/09/07  17:38:27  manchek
 * Add __synch() when releasing lock.
 *
 * Revision 1.19  1995/07/28  15:42:45  manchek
 * _touch does all pages, uses PVMTOUCH envar to switch on
 *
 * Revision 1.18  1995/07/28  14:31:42  manchek
 * oops, shm_search had too many args
 *
 * Revision 1.17  1995/07/24  18:32:21  manchek
 * da_new warns of using malloc.
 * had the wrong ST_ state test in peer_conn
 *
 * Revision 1.16  1995/07/19  18:05:14  manchek
 * peer_dump prints peer info even if stat fails
 *
 * Revision 1.15  1995/07/12  01:32:52  manchek
 * peer_conn checks that p_buf is nonzero before returning connection,
 * also that tid and state in pidtid table are correct.
 * peer_detach zeros p_buf before detaching in case interrupt occurs,
 * now unlinks and frees peer struct.
 * peer_cleanup adjusted to this
 *
 * Revision 1.14  1995/07/05  15:33:32  manchek
 * clean up semaphore too in peer_detach
 *
 * Revision 1.13  1995/07/03  21:15:40  manchek
 * declare getenv
 *
 * Revision 1.12  1995/07/03  20:36:14  manchek
 * made convex loopback code the general case in peer_conn()
 *
 * Revision 1.11  1995/07/03  19:35:43  manchek
 * hellish cleanup of comments and formatting.
 * removed POWER4 ifdefs.
 * maintain shared page counter to da_xxx to help detect deadlock.
 * shared segment key now created by task, passed through pt_key.
 * added pvmshmkey(), nextpvmshmkey()
 *
 * Revision 1.10  1995/06/28  16:08:49  manchek
 * peer_conn returns new connection status
 *
 * Revision 1.9  1995/06/02  16:22:10  manchek
 * zero p_buf in peer_detach() so we don't reference segment later
 *
 * Revision 1.8  1995/05/30  14:37:38  manchek
 * touched shared segment after detaching in peer_detach()
 *
 * Revision 1.7  1995/05/24  19:08:55  manchek
 * changed HPPA shared memory name to HPPAMP
 *
 * Revision 1.6  1995/05/22  19:53:00  manchek
 * added ifdefs for RS6KMP
 *
 * Revision 1.5  1995/05/18  17:22:34  manchek
 * need to import pvminbox and myshmbufid
 *
 * Revision 1.4  1995/05/17  16:47:22  manchek
 * bug fixes from SGI and Convex.
 * added CSPP shared memory support
 *
 * Revision 1.3  1994/11/08  15:36:49  manchek
 * shared memory damage control
 *
 * Revision 1.2  1994/06/04  21:44:25  manchek
 * updated header
 *
 * Revision 1.1  1994/06/03  20:38:26  manchek
 * Initial revision
 *
 */

#include <sys/param.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#ifdef IMA_SYMM
#include <parallel/parallel.h>
#endif
#include <errno.h>

#include <pvm3.h>
#include <pvmproto.h>
#include "pvmalloc.h"
#include "listmac.h"
#include "pvmshmem.h"
#include "bfunc.h"

#include <pvmtev.h>
#include "global.h"

#ifndef min
#define min(a,b)	((a)<(b)?(a):(b))
#endif

extern int pvmdebmask;				/* from pvmd.c or lpvmshmem.c */
extern int pvmmytid;				/* from pvmd.c or lpvmshmem.c */
extern int pvmpgsz;					/* from pvmdshmem.c or lpvmshmem.c */
extern int pgsz;					/* from pvmdshmem.c or lpvmshmem.c */
extern char *outmsgbuf;				/* from pvmdshmem.c or lpvmshmem.c */
extern int outbufsz;				/* from pvmdshmem.c or lpvmshmem.c */
extern int nbufsowned;				/* from pvmdshmem.c or lpvmshmem.c */
extern char *infopage;				/* from pvmdshmem.c or lpvmshmem.c */
extern struct pidtid *pidtids;		/* from pvmdshmem.c or lpvmshmem.c */
extern int maxpidtid;				/* from pvmdshmem.c or lpvmshmem.c */
extern int shmbufsiz;				/* from pvmdshmem.c or lpvmshmem.c */
extern int myshmbufid;				/* from pvmdshmem.c or lpvmshmem.c */
extern char *pvminbox;				/* from pvmdshmem.c or lpvmshmem.c */

char *getenv();

/***************
 **  Globals  **
 **           **
 ***************/

struct peer *peers = 0;				/* tasks we're connected to */
int bufpageused = 0;				/* number of dirty pages in msg buf */


/***************
 **  Private  **
 **           **
 ***************/

static char pvmtxt[512];			/* scratch for error log */
static int nxtpage = 0;				/* next free page in outgoing msg buf */

#ifdef IMA_CSPP
static void _touch();
#endif


/*******************************************************
 **  Shared-memory data buffer manipulation routines  **
 **                                                   **
 *******************************************************/

/*	msgbufinit()
*
*	Initialize the shared-memory message buffer.
*/

int
msgbufinit(buf)
	char *buf;
{
	char *p, *end;

	end = buf + shmbufsiz;
#ifndef IMA_KSR1
#if defined(IMA_SUNMP) && defined(PVMUSEMUTEX)
	cond_init(&((struct msgboxhdr *)buf)->mb_cond, USYNC_PROCESS, 0);
#endif
#if defined(IMA_RS6KMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP)
	pthread_cond_init(&((struct msgboxhdr *)buf)->mb_cond, NULL);
#endif
	PAGEINITLOCK(&((struct msgboxhdr *)buf)->mb_lock);
	((struct msgboxhdr *)buf)->mb_sleep = 0;
#endif
	((struct msgboxhdr *)buf)->mb_read = -1;
	((struct msgboxhdr *)buf)->mb_last = -1;

	for (p = outmsgbuf; p < end; p += pvmpgsz) {
#if !defined(TEST_ADD) && !defined(IMA_KSR1)
		PAGEINITLOCK(&((struct shmpghdr *)p)->pg_lock);
#endif
		((struct shmpghdr *)p)->pg_priv = 0;
		((struct shmpghdr *)p)->pg_ref = 0;
	}

	if (pvmdebmask & PDMNODE) {
		sprintf(pvmtxt, "msgbufinit() outmsgbuf=0x%x\n", outmsgbuf);
		pvmlogerror(pvmtxt);
		sprintf(pvmtxt, "msgbufinit() last buffer=0x%x\n", p - pvmpgsz);
		pvmlogerror(pvmtxt);
		sprintf(pvmtxt, "msgbufinit() pvmpgsz=%d\n", pvmpgsz);
		pvmlogerror(pvmtxt);
	}
	return 0;
}


/*	da_new()
*
*	Replacement for functions in pvmdabuf.c
*
*	Try to find a free page. Check page 0 first, and then the current one.
*	If neither is free, then move beyond the current page towards the end
*	of the buffer.
*/

char *
da_new(len)
	int len;
{
	char *p;
	int start;

	if (len > pvmpgsz - PVMPAGEHDR) {
		sprintf(pvmtxt, "da_new() len = %d: frag must fit in a page\n", len);
		pvmlogerror(pvmtxt);
		return 0;
	}

#ifdef IMA_CSPP
	p = outmsgbuf + nxtpage*pvmpgsz;
#else
	p = outmsgbuf;
	if (nxtpage && !((struct shmpghdr *)p)->pg_ref)
		nxtpage = 0;		/* recycle to avoid page fault */
	else
		p += nxtpage*pvmpgsz;
#endif

	start = nxtpage;
	while (1) {
		if (((struct shmpghdr *)p)->pg_ref == 0) {
			nbufsowned++;
			break;
		}
		if (++nxtpage == outbufsz) {
			nxtpage = 0;
			p = outmsgbuf;

		} else
			p += pvmpgsz;

		if (nxtpage == start) {		/* buffer full */
			p = TALLOC(len + PVMPAGEHDR, char, "data");
			if (p) {
				static int once = 1;

				if (once) {
					once = 0;
					pvmlogerror("da_new() Warning: shared buffer full, using malloc\n");
				}
#if !defined(IMA_KSR1) && !defined(TEST_ADD)
				PAGEINITLOCK(&((struct shmpghdr *)p)->pg_lock);
#endif
				((struct shmpghdr *)p)->pg_priv = 1;
			}
			break;
		}
	}

	if (nxtpage >= bufpageused)
		bufpageused = nxtpage + 1;
	if (p) {
		((struct shmpghdr *)p)->pg_ref = 1;
		return (p + PVMPAGEHDR);
	} else
		return 0;
}

void
da_ref(p)
	char *p;
{
	p -= PVMPAGEHDR;
#ifdef TEST_ADD
	TEST_ADD(&((struct shmpghdr *)p)->pg_ref, 1);
#else
	PAGELOCK(&((struct shmpghdr *)p)->pg_lock);
	++((struct shmpghdr *)p)->pg_ref;
	PAGEUNLOCK(&((struct shmpghdr *)p)->pg_lock);
#endif
}


void
da_unref(p)
	char *p;
{
	p -= PVMPAGEHDR;
#ifdef TEST_ADD
	if ((long) TEST_ADD(&((struct shmpghdr *)p)->pg_ref, -1L) < 0)
		pvmlogerror("ref count is negative!\n");
#else
	PAGELOCK(&((struct shmpghdr *)p)->pg_lock);
	if (--((struct shmpghdr *)p)->pg_ref < 0) {
		sprintf(pvmtxt, "ref = %d on page %d\n",
			((struct shmpghdr *)p)->pg_ref, (p - outmsgbuf)/pgsz + 1);
		pvmlogerror(pvmtxt);
	}
	PAGEUNLOCK(&((struct shmpghdr *)p)->pg_lock);
#endif
	if (((struct shmpghdr *)p)->pg_priv) {
		if (((struct shmpghdr *)p)->pg_ref < 1)
			PVM_FREE(p);
	} else {
		if (p >= outmsgbuf && p < outmsgbuf + outbufsz * pvmpgsz)
			nbufsowned--;
	}
}


void
peer_init()
{
	peers = TALLOC(1, struct peer, "peer");
	BZERO((char*)peers, sizeof(struct peer));
	peers->p_link = peers->p_rlink = peers;
}


/*	peer_conn()
*
*	Attach peer's message buffer.
*	Returns pointer to peer struct,
*	Else 0 if peer isn't ready to receive yet
*	Or  -1 if peer can't be contacted via shared memory or has disconnected.
*/

struct peer *
peer_conn(tid, new_connection)
	int tid;
	int *new_connection;
{
	struct peer *pp;
	char *p;

	if (new_connection)
		*new_connection = 0;
	for (pp = peers->p_link; pp != peers; pp = pp->p_link) {
		if (pp->p_tid == tid) {		/* already connected */
			if (pp->p_exited || !pp->p_buf) {
				if (pvmdebmask & PDMMEM) {
					sprintf(pvmtxt, "peer_conn() t%x has exited?\n", tid);
				 	pvmlogerror(pvmtxt);
				 }
				return ((struct peer *)-1L);
				}
			else {
				if (pvmdebmask & PDMMEM) {
					sprintf(pvmtxt, "peer_conn() to t%x got pp addr %lx\n", 
							tid, pp);
				 	pvmlogerror(pvmtxt);
				 }
				return pp;

				}
		}
	/*
	* test p_buf in case we're in the process of destroying this peer struct
	*/

#if 0
	/* XXX disconnect from inactive peers */
		if (pp2->p_tid == -1) {		/* used by pvmd */
			/* peer_detach(pp2); */
			shmdt(pp2->p_buf);
			LISTDELETE(pp2, p_link, p_rlink);
			PVM_FREE(pp2);
		}
#endif
	}

	if (pp == peers && tid == pvmmytid) {
		if (!(pp = TALLOC(1, struct peer, "peer"))) {
			pvmlogerror("peer_conn() can't get memory\n");
			return (struct peer *)0;
		}
		BZERO((char*)pp, sizeof(struct peer));
		pp->p_buf = pvminbox;
		pp->p_shmid = myshmbufid;
		pp->p_tid = pvmmytid;
		pp->p_semid = -1;
		LISTPUTAFTER(peers, pp, p_link, p_rlink);

	} else if (pp == peers && tid) {
		int bufid, ntids, i, key;

		/* PAGELOCK(pvminfo); */
	/* XXX the first few don't have to search the whole empty table. hmm. */
		ntids = min(maxpidtid, ((struct shmpghdr *)infopage)->pg_ref);
		for (i = 0; i < ntids; i++)
			if (pidtids[i].pt_tid == tid && pidtids[i].pt_stat != ST_EXIT) {
				if (pidtids[i].pt_stat == ST_NOTREADY)
					return (struct peer *)0;
				if (pidtids[i].pt_stat == ST_SOCKET)
					return (struct peer *)-1L;
				/* must be ST_SHMEM or ST_FINISH */
				break;
			}
		if (i == ntids) 
			{
			if (pvmdebmask & PDMMEM) {
				sprintf(pvmtxt, "peer_conn() could not find t%x in daemon list\n", tid);
			 pvmlogerror(pvmtxt);
			 }
			return (struct peer *)-1L;
		/* PAGEUNLOCK(pvminfo); */
		}

		key = pidtids[i].pt_key;
		if ((bufid = shmget((key_t)key, shmbufsiz, 0)) == -1) {
			sprintf(pvmtxt, "peer_conn() shmget t%x key 0x%x", tid, key);
			pvmlogperror(pvmtxt);
			return (struct peer *)0;
		}
		while ((p = (char *)shmat(bufid, 0, 0)) == (char *)-1L) {
			sprintf(pvmtxt, "peer_conn() shmat to t%x using bufid %d\n", 
					tid, bufid);
			pvmlogperror(pvmtxt);
			if (errno == EMFILE && (pp = peers->p_rlink) != peers) {
				shmdt(pp->p_buf);
				LISTDELETE(pp, p_link, p_rlink);
				PVM_FREE(pp);
				continue;
			}
			return (struct peer *)0;
		}

#ifdef IMA_CSPP
		_touch(p);
#endif

		if (!(pp = TALLOC(1, struct peer, "peer"))) {
			pvmlogerror("peer_conn() can't get memory\n");
			return (struct peer *)0;
		}
		BZERO((char*)pp, sizeof(struct peer));
		pp->p_key = key;
		pp->p_tid = tid;
		pp->p_buf = p;
		pp->p_shmid = bufid;
		pp->p_semid = -1;
#ifdef IMA_CSPP
		pp->p_node = pidtids[i].pt_node;
#endif
		pp->p_exited = 0;
		LISTPUTAFTER(peers, pp, p_link, p_rlink);
		if (new_connection)
			*new_connection = 1;
	}

	return pp;
}


/*	peer_wake()
*
*	Wake up sleeping task that we just sent message to.
*/

void
peer_wake(pp)
	struct peer *pp;
{
	struct sembuf	sops;

	if (pp->p_semid == -1
	&& (pp->p_semid = semget((key_t)pp->p_key, 1, PERMS)) == -1) {
		sprintf(pvmtxt, "peer_wake() semget t%x key 0x%x\n",
				pp->p_tid, pp->p_key);
		pvmlogperror(pvmtxt);
		return;
	}
	sops.sem_num = 0;
	sops.sem_op = 1;
	sops.sem_flg = 0;
	if (semop(pp->p_semid, &sops, 1) == -1) {
		sprintf(pvmtxt, "peer_wake() semop t%x\n", pp->p_tid);
		pvmlogperror(pvmtxt);

	} else if (pvmdebmask & PDMSEM) {
		sprintf(pvmtxt,"peer_wake() sem %d\n", pp->p_semid);
		pvmlogerror(pvmtxt);
	}
}


/*	peer_detach()
*
*	Detach peer's msg buffer, destroy if no one else is attached to it.
*/

int
peer_detach(pp)
	struct peer *pp;
{
	struct shmid_ds shmds;
	char *p;

	if (pp->p_tid != pvmmytid) {
	/*
	* zero p_buf before destroying it in case we're interrupted right here.
	* yes, it happend.
	*/
		if (p = pp->p_buf) {
			pp->p_buf = 0;
			if (shmdt(p) == -1)
				pvmlogperror("peer_detach() shmdt");
		}

		if (!shmctl(pp->p_shmid, IPC_STAT, &shmds) && shmds.shm_nattch == 0) {
			shmctl(pp->p_shmid, IPC_RMID, (struct shmid_ds *)0);

#ifdef	USERECVSEMAPHORE
			if (pp->p_semid == -1 && pp->p_key)
				pp->p_semid = semget((key_t)pp->p_key, 1, PERMS);
			if (pp->p_semid != -1)
				semctl(pp->p_semid, 0, IPC_RMID);
#endif
		}
	}

	LISTDELETE(pp, p_link, p_rlink);
	PVM_FREE(pp);
	return 0;
}


void
peer_cleanup()
{
	while (peers->p_link != peers)
		peer_detach(peers->p_link);
}


int
peer_dump()
{
	struct peer *pp;
	struct shmid_ds shmds;

	pvmlogerror("peer_dump()\n");
	for (pp = peers->p_link; pp != peers; pp = pp->p_link) {
		if (shmctl(pp->p_shmid, IPC_STAT, &shmds) == -1) {
			sprintf(pvmtxt, "peer_dump() shmctl STAT %d", pp->p_shmid);
			pvmlogperror(pvmtxt);
			sprintf(pvmtxt,
					" t%x key=0x%x shmid=%d semid=%d shmlen=%d\n",
					pp->p_tid, pp->p_key, pp->p_shmid, pp->p_semid, pp->p_dlen);
			pvmlogerror(pvmtxt);

		} else {
			sprintf(pvmtxt,
					" t%x key=0x%x shmid=%d semid=%d shmlen=%d shmna=%d\n",
					pp->p_tid, pp->p_key, pp->p_shmid, pp->p_semid, pp->p_dlen,
					(int)shmds.shm_nattch);
			pvmlogerror(pvmtxt);
		}
	}
	return 0;
}


#ifdef IMA_CSPP
#define	SHMIDPIDPART	0xfff
#endif

#ifndef	SHMIDUIDPART
#define	SHMIDUIDPART	0xffff0000
#endif
#ifndef	SHMIDPIDPART
#define	SHMIDPIDPART	0xffff
#endif

/*	pvmshmkey()
*
*	Get the shared-memory segment key of the pvmd, if pid == 0,
*	else of process pid.
*/

int
pvmshmkey(pid)
	int pid;
{
	static int pvmd_key = 0;

	int k;
	char *p;

	if (!pvmd_key) {
		if (p = getenv("PVMKEY"))
			pvmd_key = atoi(p);
		else
			pvmd_key = getuid();
		pvmd_key = (pvmd_key << 16) & SHMIDUIDPART;
		if (!pvmd_key)
			pvmd_key = SHMIDUIDPART;
#ifdef IMA_CSPP
		pvmd_key |= (get_scid() & 0xf) << 12;
#endif
	}

	if (pid) {
		if (!(pid &= SHMIDPIDPART))
			pid = 1;
		k = pvmd_key | pid;
		if (pvmdebmask & (PDMMEM | PDMSEM)) {
			sprintf(pvmtxt, "pvmshmkey(): shm/sem key is 0x%x\n", k);
			pvmlogerror(pvmtxt);
		}

	} else {
		k = pvmd_key;

		if (pvmdebmask & (PDMMEM | PDMSEM)) {
			sprintf(pvmtxt, "pvmshmkey(): pvmd shm/sem key is 0x%x\n", k);
			pvmlogerror(pvmtxt);
		}
	}

	return k;
}


/*	nextpvmshmkey()
*
*	Get a new shared memory key to try (in case the default failed).
*/

int
nextpvmshmkey(key)
	int key;
{
	int pid;

	pid = (key + 1) & SHMIDPIDPART;
	if (!pid)
		pid = 1;
	pid |= pvmshmkey(0);
	if (pvmdebmask & (PDMMEM | PDMSEM)) {
		sprintf(pvmtxt, "nextpvmshmkey() shm/sem key is 0x%x\n", pid);
		pvmlogerror(pvmtxt);
	}
	return pid;
}


#if defined(IMA_CSPP) || defined(IMA_HPPAMP)

void
init_lock(vp, f, l)
	pvmspplock_t *vp;
	char *f;
	int l;
{
	if (pvmdebmask & PDMLOCK) {
		sprintf(pvmtxt,"init_lock(): init %#x from %s line %d\n", vp,f,l);
		pvmlogerror(pvmtxt);
	}

	*(int *)vp = 1;
}

int
acquire_lock(vp, f, l)
	pvmspplock_t *vp;
	char *f;
	int l;
{
	int * volatile ip = (int *)vp;
	int i=0, j=0;

	if (pvmdebmask & PDMLOCK) {
		sprintf(pvmtxt,"acquire_lock(): polling %#x from %s line %d\n", ip,f,l);
		pvmlogerror(pvmtxt);
	}

	while (1) {
		while (!(*ip)) i++;
		if (__ldcws32(ip))
			break;
		j++;
	}

	if (pvmdebmask & PDMLOCK) {
		if (i)
			sprintf(pvmtxt,"acquire_lock(): acquired %#x after %d/%d polls\n",
				ip, i, j);
		else
			sprintf(pvmtxt,"acquire_lock(): acquired %#x\n", ip);
		pvmlogerror(pvmtxt);
	}
	return 0;
}

int
release_lock(vp,f,l)
	pvmspplock_t *vp;
	char *f;
	int l;
{
	if (pvmdebmask & PDMLOCK) {
		sprintf(pvmtxt,"release_lock(): releasing %#x from %s line %d\n", vp,f,l);
		pvmlogerror(pvmtxt);
	}
	__synch();
	*(int *)vp = 1;
	return 0;
}

#endif	/*defined(IMA_CSPP) || defined(IMA_HPPAMP)*/

#ifdef IMA_CSPP
static int touch_flg = -1;        /* flag to page memory early */
#define DONT_TOUCH   0
#define TOUCH_PAGES  1

/* _touch()
 *
 *  Touch all the pages in this shared memory block
 */

static void
_touch(addr)
char *addr;
{
	switch (touch_flg) {
	case TOUCH_PAGES:
		{
            char *shm_pg;
            int pgsz = sysconf(_SC_PAGESIZE);

			if(pvmdebmask & PDMMEM) {
				sprintf(pvmtxt, "Touching block at addr 0x%x (%d bytes)\n",
					addr, shmbufsiz);
				pvmlogerror(pvmtxt);
			}

            for (shm_pg = addr; shm_pg < (addr + shmbufsiz); shm_pg += pgsz)
                shm_pg[63] = 0;
        }
		break;

	case DONT_TOUCH:
		break;

	default:
		touch_flg = (getenv("PVMTOUCH") ? TOUCH_PAGES : DONT_TOUCH);
		if(pvmdebmask & (PDMTASK|PDMMEM)) {
			sprintf(pvmtxt, "Shared memory touching: %s\n",
				touch_flg ? "ON" : "OFF" );
			pvmlogerror(pvmtxt);
		}
	}
	return;
}

/*
 * ---> On CSPP SYS V memory systems, the VM addr given to the PVM daemon may
 * ---> not work for the PVM clients.  Search for an open spot in the VM space
 * ---> that will work for SOM and ESOM executables.
 */

#define GATEWY_LO_ADDR  0xbff00000
#define GATEWY_HI_ADDR  0xc0010000
#define SHM_HI_ADDR     0xf0000000
#define SHM_LO_ADDR     0x80000000

char *
shm_search(bufid)
int bufid;
{
	unsigned int i, unmapped = 1;
	char *inbox;
	int shm_incr_addr = (shmbufsiz + 4095) & (~4095);	/* round up */

	for(i = GATEWY_LO_ADDR; i > SHM_LO_ADDR && unmapped; i-=shm_incr_addr) {
		inbox = (char*)shmat(bufid, (char*) i, 0);
		unmapped = (inbox == (char*)-1L);
		if ( unmapped && (pvmdebmask & PDMMEM) ) {
			sprintf(pvmtxt, "shm_search(): shmat() failed attach at %#x\n", i);
			pvmlogerror(pvmtxt);
		}
	}

	for(i = GATEWY_HI_ADDR; i < SHM_HI_ADDR && unmapped; i+=shm_incr_addr) {
		inbox = (char*)shmat(bufid, (char*) i, 0);
		unmapped = (inbox == (char*)-1L);
		if ( unmapped && (pvmdebmask & PDMMEM) ) {
			sprintf(pvmtxt, "shm_search(): shmat() failed attach at %#x\n", i);
			pvmlogerror(pvmtxt);
		}
	}

	if ( unmapped ) {
		pvmlogerror("shm_search(): Unable to shmat\n");
		return (char *) -1;
	}

	if (pvmdebmask & PDMMEM) {
		sprintf(pvmtxt, "shm_search(): shmat attached to addr %x\n",
			(unsigned int) inbox);
		pvmlogerror(pvmtxt);
	}

	/* Page in the shared memory */

	_touch(inbox);

	return inbox;
}
#endif	/*IMA_CSPP*/



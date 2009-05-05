
static char rcsid[] =
	"$Id: pvmshmmsg.c,v 1.4 2005/08/22 15:13:25 pvmsrc Exp $";

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
 *
 */
/* Design/philosophy of shared memory message transport:

	1) Each task allocates a shared memory segment.

	2) The shared memory segment is logically split into two parts
		a) the message control page
		b) data buffer pool

	3) Reading/writing access to all of the shared memory is controlled
		by a single semaphore (or lock) on the message control page.

	4) The data buffers are of fixed size (MAXFRAGSIZE).
		Messages that are longer than this fixed size must be
		fragmented.

	5) Message sends/recvs are based on asynchronous semantics.  For a
		send to complete, a matching receive must be posted at the
		destination.  What this means is that the receiver must allocate
		a  buffer from the shared pool before a sender can write.

	6) The number of message copies needed to send a message from user
		space to user space is 3. One copy is the original pvm pack.
		The "send" is a copy into a shared buffer. The third is the pvm
	 	unpack.


	Shared Memory Segment (1 per process)

			|-------------------|
			|                   | 0
			|                   |
			|   control page    |
			|                   |
			|                   | CTRLPAGELEN - 1
			|-------------------|
			|                   | CTRLPAGELEN
			|                   |
			|   data buffer     |
			|                   |
			|                   | CTRLPAGELEN + MAXFRAGSIZE - 1
			|-------------------|
			|                   | CTRLPAGELEN + MAXFRAGSIZE
			|                   |
			|   data buffer     |
			|                   |
			|                   | CTRLPAGELEN + 2*MAXFRAGSIZE - 1
			|-------------------|
			          o
			          o
			          o
			|                   |
			|                   | SHMSEGSIZE - 1
			|-------------------|


-- Mutual exclusion control exclusion.
	- Whenever a task (either a sender or receiver) wants to
		interrogate (determine if a receive has been posted/completed)
		the entire control page is exclusively locked.

		1) Posting a receive - the receiver must pre-allocate a fixed
			size buffer from his shared memory pool.
			Step 1) acquire lock.
			Step 2) Find first free open message msgcntrl.msgs[i].
					Update msgctrl.nmsgs if adding to number of messages
			Step 3) Modify msginfo.msgs[i] to reflect desired src, tag,
					and maxlen. Offset into the shared memory region
					gives the buffer.
					msgcntrl.msgs[i].status = ISRECVMSG.
			Step 4) release lock.
			Step 5) return i to caller for message completion testing.

		2) Receive completion for message i
			Step 1) acquire lock
			Step 2) determine if message has completed  by checking
					msgcntrl.msgs[i].status & COMPLETE
			Step 3) If complete,  mark mscntrl.msgs[i].status = NOBUFFER
			Step 4)  Release lock.
			Step 5) Return pointer to data buffer if complete, else
					return 0;


		3) Posting a send:
			On sender's side, for each receiver, retain a queue of send
			requests.
			Add send to *end* of task send queue.

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>

#include <pvm3.h>
#include <pvmproto.h>
#include <pvmtev.h>
#include "global.h"
#include "pvmalloc.h"
#include "listmac.h"
#include "pvmdabuf.h"
#include "pvmmagic.h"
#include "lmsg.h"
#include "pvmmimd.h"
#include "pvmshmmsg.h"

#include "bfunc.h"

extern int pvmdebmask;
/* GEF */
/* extern int pvmmynode; */
/* not imported from lpvmmind.c yet... */
static int pvmmynode = -1;
/* GEF */

struct segdesc *pvmmyshmseg = (struct segdesc *) NULL;

static struct shm_midlist pvm_shmmids[NSHMMIDS];
static int lastMidAlloced = 1;

char *getenv();

/* ============================================
	 Section I
		 shared memory creation, destruction and attachment.
		 lock (semaphore) creation, destruction. attachment.
   ============================================ */

/* --------- pvm_shmget ------- */
/* Create a shared segment.

	o For the "owner" of the shared segment, It should be called with:
		len = length of segment
		iAmOwner= 1;

	o For client attaching to a shared segment, It should be called
		len= 0;
		iAmOwner= 0;

	o pvm_shmat must be called with id returned in segdesc->segid to
		bind to the shared segment
*/

struct segdesc *
pvm_shmget(key, len, iAmOwner)
key_t key;
int len;
int iAmOwner;
{
struct segdesc *rseg;
struct shmid_ds shminfo;
int flags = 0;

	if ((rseg = (struct segdesc *) TALLOC(1, struct segdesc, "shmget")) == 0)
	{
		pvmlogerror( "pvm_shmget (): No memory for seg descriptor \n");
		return (struct segdesc *) NULL;
	}

	BZERO(rseg, sizeof(struct segdesc));

	 if (iAmOwner)
	 	flags =IPC_CREAT | IPC_EXCL | PERMS;
	 else
		flags = PERMS;

	if ((rseg -> segid = shmget(key, len, flags)) == -1
		 && pvmdebmask & PDMNODE)
	{
		pvmlogperror ("pvm_shmget (): could not create shared segment \n");
		/* GEF */
		/* there was just a free here... wonder if pvm_free will work */
/* 		FREE(rseg); */
		PVM_FREE(rseg);
		return (struct segdesc *) NULL;
	}

	shmctl(rseg -> segid , IPC_STAT, &shminfo);

	rseg -> len= shminfo.shm_segsz;
	rseg -> bound = 0;
	rseg -> segControl = (struct msgctrl *) NULL;
	rseg -> pagelock = rseg -> pageunlock = 0;
	rseg -> lock = 0;
	rseg -> iAmOwner= iAmOwner;

	return rseg;

}

/* -------- pvm_shmat () ------	*/
/* attach to a previously defined shared segment. This is called after
	a successful call to pvm_shmget

*/
char *
pvm_shmat ( sseg )
struct segdesc *sseg;
{
char *buffer;
	if (sseg->bound)
		return(sseg->start);

	if (!sseg->lock)
	{
		pvmlogerror( "pvm_shmat(): need to get lock before attach\n") ;
		return (char *) NULL;
	}

	if ( (buffer = shmat(sseg->segid, (char *) NULL, 0) ) == (char *) NULL)
	{
		pvmlogerror( "pvm_shmat() : could not attach to the share segment\n");
		return (char *) NULL;
	}


	sseg -> start = buffer;
	sseg -> segControl = (struct msgctrl *) buffer;


	/* Initialize and/or Update msgctrl section of this shared segment */

	(*sseg->pagelock) (sseg->lock);

	if (sseg -> iAmOwner)
		pvm_msgctrlInit(sseg -> segControl, sseg->len,
				MAXFRAGSIZE, MAXFRAGSIZE);

	sseg->segControl->nattach ++; /* our idea of # tasks attached */

	(*sseg->pageunlock) (sseg->lock);

	sseg->bound = 1;

	return buffer;
}

/* -------pvm_shmdt() ---------- */
/* detach from a previously attached shared segment
*/

int
pvm_shmdt ( sseg )
struct segdesc *sseg;
{

struct shmid_ds shminfo;

	if (!sseg ||  !sseg -> bound )       /* not bound to the shared segment */
	 	return (0);

	(*sseg -> pagelock)(sseg -> lock);

	sseg -> segControl -> nattach --;

	/* find out how many are attached to this segment */

	shmctl ( sseg -> segid, IPC_STAT, &shminfo);

	if ( shminfo.shm_nattch == 1 ) /* I am the only one left, destroy */
		shmctl( sseg -> segid, IPC_RMID, &shminfo);

	(*sseg -> pageunlock)( sseg -> lock );

	shmdt( sseg -> start); /* the segment will get destroyed after detach */

 	/* reset structure variables */
	sseg -> bound = 0;
	sseg -> segControl = (struct msgctrl *) 0;
	sseg -> segid = 0;
	sseg -> len = 0;
	sseg -> start = (char *) NULL;

	/* iAmOwner is shared between the shm seg and the semaphore lock */
	/* thus we leave this variable to be reset by the pvm_lockdt() call */
	/* unless the lock as already gone.. yes the lock we used above! */
	if (!sseg->lock)
		sseg -> iAmOwner= 0;

	return 0;
}


/* -------pvm_shmfree() ---------- */
/* delete a shared segment ( this is also done by the shmdt.... )
*/

int
pvm_shmfree ( sseg )
struct segdesc *sseg;
{

struct shmid_ds shminfo;

	/* We haven't checked to see if this segment is locked... */
	/* We also haven't checked to see if we are attached/own it etc */

	/* Ok..  just grab it */
	(*sseg -> pagelock)(sseg -> lock);

	/* find out how many are attached to this segment */
	shmctl ( sseg -> segid, IPC_STAT, &shminfo);

	if ( shminfo.shm_nattch == 0 )
		shmctl( sseg -> segid, IPC_RMID, &shminfo);
	else /* what? */
		shmctl( sseg -> segid, IPC_RMID, &shminfo);
		/* yep, wrong... should send message to all so that they can detach */
		/* in a nice way */

	(*sseg -> pageunlock)( sseg -> lock );

 	/* reset structure variables */
	sseg -> bound = 0;
	sseg -> segControl = (struct msgctrl *) 0;
	sseg -> segid = 0;
	sseg -> len = 0;
	sseg -> start = (char *) NULL;
	sseg -> iAmOwner= 0;

	return 0;
}

/* --------- pvm_lockget ------- */
/* Create a lock for a particular shared segment.
	If owner of the lock, initialize the lock.

*/
int
pvm_lockget (key, sseg, iAmOwner )
key_t key;
struct segdesc *sseg;
int iAmOwner;
{
int flags;
int semid;
/* GEF */
/* ops SysV requires that we explicitly declare this if the forth arg is needed
 * in any shmem op.
 */
/* union semun lockarg; */
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
} lockarg;

int lockcmd;

	if ( sseg == (struct segdesc *) NULL)
	{
		pvmlogerror( "pvm_lockget (): invalid segment descriptor \n");
		return PvmBadParam;
	}

	if (iAmOwner)
		flags =IPC_CREAT | IPC_EXCL | PERMS;
	else
		flags = PERMS;

	if ((sseg -> lock = semget(key, 1, flags)) == -1
		 && pvmdebmask& PDMNODE)
	{
		sseg -> lock = 0;
		pvmlogperror( "pvm_lockget (): could not create lock \n");
		return 0;
	}

	sseg -> pagelock =  pvm_shmlock;
	sseg -> pageunlock = pvm_shmunlock;

/* If I am the owner, initialize the lock to 0 (unlocked) */

	lockcmd = IPC_SET;
	lockarg.val = 0;
	semctl( sseg -> lock, 0, lockcmd, lockarg );

	return sseg -> lock;
}

/* --------- pvm_lockdt ------- */
/* detach from a lock that has been gotten by pvm_lockget */
int
pvm_lockdt ( sseg )
struct segdesc *sseg;
{
int lockcmd;
int semid;
/* see above routine GEF */
/* union semun lockarg; */
union semun {
	int val;
	struct semid_ds *buf;
		ushort *array;
} lockarg;



	if (!sseg || !sseg -> lock )
		return 0;

	if ( sseg -> iAmOwner )      /* I own the lock, will free it */
	{

		/* Lock the page -- upon deletion, any tasks waiting on the */
	 	/* 	semaphore will be woken up and returned an error */
		(* sseg -> pagelock ) (sseg->lock);
		lockcmd = IPC_RMID;
		lockarg.val = 0;
		semctl ( sseg -> lock, 0, lockcmd, lockarg );
		/* we can now reset the ownership variable if the shm seg has gone */
		if (!sseg->start)
			sseg -> iAmOwner= 0;
	}

	sseg -> lock = 0;
	sseg -> pagelock = sseg -> pageunlock = 0;

	return 0;
}
/* --------- pvm_shmlock --------- */
int
pvm_shmlock( lock )
PVM_LOCK lock;
{
struct sembuf lockop;

	lockop.sem_num= 0;
	lockop.sem_op= 1;
	lockop.sem_flg= 0;

	if (pvmdebmask & PDMSEM )
	 	pvmlogprintf ("Acquiring lock %d \n" , lock) ;

	while (semop(lock, &lockop, 1) == -1)
	{
		if (errno != EINTR)
		{
			pvmlogprintf(" Fatal Error acquiring lock %d (errno %d)\n",
					lock, errno);
			return PvmSysErr;
		}
		/* GEF */
		pvmsleep (1);	/* don't even ask... */
	}

	if (pvmdebmask & PDMSEM )
		 pvmlogprintf("lock %d acquired \n", lock);

	return 0;
}

/* --------- pvm_shmunlock --------- */
int
pvm_shmunlock(lock)
PVM_LOCK lock;
{
struct sembuf lockop;

	lockop.sem_num = 0;
	lockop.sem_op = -1;
	lockop. sem_flg= 0;

	if (pvmdebmask & PDMSEM )
		pvmlogprintf ("Releasing lock %d \n", lock);

	while (semop(lock, &lockop, 1) == -1)
	{
		if (errno != EINTR )
		{
			pvmlogprintf(" Fatal Error releasing lock %d (errno %d)\n",
					lock, errno);
			return PvmSysErr;
		}
	}

	if (pvmdebmask & PDMSEM)
		pvmlogprintf("lock %d released \n", lock);

	return 0;
}
/* --------- pvm_msgctrlInit --------- */
/* initialize the message control page at the beginning of the
shared memory segment pointed to by cblock of length len.
*/
int
pvm_msgctrlInit(cblock, len, ctrlpagelen, blksize)
struct msgctrl *cblock;
int len;		/* length of the controlled shared segment */
int ctrlpagelen;/* length of the control page */
int blksize;    /* size of blocks that will be allocd out of this shared seg */
{
	BZERO((char *) cblock, ctrlpagelen);

	cblock -> len = len;
	cblock -> maxmsgs= (len- ctrlpagelen) /blksize;

	return 0;
}

/* --------- pvm_shmmsginit () --- */
/* Initialize shared memory messaging, structures  for the local
	task
*/
int
pvm_shmmsginit( key, localseg )
key_t *key;
struct segdesc **localseg;
{
key_t lockkey;
key_t shmkey;

int first;
int lock;
int nretries;
char *tmp_shmseg_ptr;

struct segdesc * myshmseg;

	/* Step 1 - create my local shared memory segment */
	/* Step 2 - create lock for local shared segment */
	/* Step 3 - create the local midlist for posted sends and receives */

	nretries = 0;

	/*  === Step 1: === */
	/* This section really just makes a segment descriptor and a shm key */

	first = shmkey = (key_t) pvm_shmkey(getpid());
	do
	{
		if ( (myshmseg = pvm_shmget(shmkey, SHMSEGLEN, SHM_OWNER)) !=
				(struct segdesc *) NULL )
		{
	        if ( (tmp_shmseg_ptr = shmat(myshmseg->segid, (char *) NULL, 0) )
					== (char *) NULL)
/* GEF */
/* Can't it in this order as we need a lock before pvm_shat() will work */
/* 			if ( pvm_shmat( myshmseg ) == (char *) NULL)	 */
/* we attach as this is the surest way of testing everything's ok */
/* GEF */
			{
				pvmlogprintf("Created segment but could not attach!\n");
				pvm_shmfree(myshmseg);
				myshmseg = (struct segdesc *) NULL;
				return PvmSysErr;
			}
			else
				shmdt (tmp_shmseg_ptr);
				/* we detach here to make sure that the attach count is right */
			break;
		}
		else
		{
			nretries ++;
			shmkey = (key_t) pvm_nextshmkey(shmkey);
		}
	}
	while (shmkey != first);

	if (!myshmseg)
	{
		pvmlogprintf("Could not create shared memory segment after %d\n",
				nretries);
		return PvmSysErr;
	}

	/* ==== Step 2: ===== */

	/* ops Phil... the key used for the shared memory segment may be used */
	/* by another semaphore.. so is should really be a loop like above */
	/* I know... it stinks, but... */
	lockkey = shmkey;

	if ( (lock = pvm_lockget(lockkey, myshmseg, LOCK_OWNER)) == 0)
	{
		pvmlogprintf("Could not create lock \n");
		return PvmSysErr;
	}

	/* ==== Step 2 and a half: ===== */
	/* ok... we can use the PVM_SHMAT routine now as we have a valid lock...  */

			if ( pvm_shmat( myshmseg ) == (char *) NULL)
			{
				pvmlogprintf("Created segment but could not attach!\n");
				pvm_shmfree(myshmseg);
				myshmseg = (struct segdesc *) NULL;
				return PvmSysErr;
			}

	/* ==== Step 3: ===== */
	/* Initialize the send/receive midlist */

	BZERO( pvm_shmmids, NSHMMIDS*sizeof(struct shm_midlist));

	*localseg = myshmseg;
	*key = shmkey;

	return 0;
}

/* --------- pvm_shmkey () --- */
/*	pvm_shmkey()
*
*	Get the shared-memory segment key of the pvmd, if pid == 0,
*	else of process pid.
*/

int
pvm_shmkey(pid)
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
			pvmlogprintf("pvmshmkey(): shm/sem key is 0x%x\n", k);
		}

	} else {
		k = pvmd_key;

		if (pvmdebmask & (PDMMEM | PDMSEM)) {
			pvmlogprintf("pvmshmkey(): pvmd shm/sem key is 0x%x\n", k);
		}
	}

	return k;
}


/* --------- pvm_nextshmkey () --- */
/*	pvm_nextshmkey()
*
*	Get a new shared memory key to try (in case the default failed).
*/

int
pvm_nextshmkey(key)
	int key;
{
	int pid;

	pid = (key + 1) & SHMIDPIDPART;
	if (!pid)
		pid = 1;
    /* GEF */
/* 	pid |= pvmshmkey(0); */
	pid |= pvm_shmkey(0);
	if (pvmdebmask & (PDMMEM | PDMSEM)) {
		pvmlogprintf("nextpvmshmkey() shm/sem key is 0x%x\n", pid);
	}
	return pid;
}

/* ==========================================
	Section II
    Basic shared memory messaging
   ========================================== */

/* --------- pvm_allocShmmid ------ */
int
pvm_allocShmmid(midentry)
struct shm_midlist **midentry;
{
int i;

	for (i = lastMidAlloced; i < NSHMMIDS; i++)
	{
		if (!pvm_shmmids[i].ml_status)  /* this mid is free */
		{
			pvm_shmmids[i].ml_status = ISALLOCED;
			lastMidAlloced = i;
			*midentry = pvm_shmmids + i;
			return i;
		}
	}

	for (i = 1; i < lastMidAlloced; i++)
	{
		if (!pvm_shmmids[i].ml_status)  /* this mid is free */
		{
			pvm_shmmids[i].ml_status = ISALLOCED;
			lastMidAlloced = i;
			*midentry = pvm_shmmids + i;
			return i;
		}
	}
	return PvmSysErr;		/* all the mids were full */

}
/* --------- pvm_freeShmmid ------ */
int
pvm_freeShmmid(mid)
{
		pvm_shmmids[mid].ml_status = 0;
		lastMidAlloced = mid;		/* try to reclaim the mids */
		return 0;
}

/* --------- pvm_irecvshm --------- */
/* return the mid of the posted receive message. The mid is allocated
	from pvm_shmmidlist;
*/
int
pvm_irecvshm(sseg, buf, len, src, tag, info)
struct segdesc *sseg;
char *buf;
int len;
int src;
int tag;
int *info;
{

int freebuf;
int msgnum;
int offset;
struct msgctrl *control;
struct msginfo *thismsginfo;
struct shm_midlist *midentry;
int midListId;

	if (sseg == (struct segdesc *) NULL)
		return PvmBadParam;		/* invalid shared segment */

	if ( (offset = (buf -  sseg -> start)) < 0
			|| (sseg -> start + sseg -> len) < (buf + len) )
		return PvmBadParam;		/* data buffer not completely in shared seg */

	if ( (midListId = pvm_allocShmmid( &midentry)) < 0)
		return midListId;

	/* record the receive info in the midlist */
	midentry -> ml_status = ISRECVMSG;
	midentry -> ml_src = src;
	midentry -> ml_tag = tag;
	midentry -> ml_len = len;
	midentry -> ml_buf = buf;
	midentry -> shm_msgnum = -1;
	midentry -> ml_info = info;
	midentry -> ml_link = midentry -> ml_rlink = (struct shm_midlist *) NULL;


	(*sseg->pagelock)(sseg->lock);	/* acquire lock */

	control = sseg->segControl;

	/* determine which msginfo buffer we can use for this message */
	freebuf = 0;
	for (msgnum = 0; msgnum < control->nmsgs; msgnum++ )
	{
		freebuf = control->msgs[msgnum].mi_status == NOBUFFER;
		if (freebuf)
			break;
	}

	/* If no free msginfo buffers, Space for one more ? */
	if (msgnum == control->nmsgs)
	{
		if (control -> nmsgs >= control -> maxmsgs)  /* too many posted recv's*/
		{
			midentry -> ml_status = NOBUFFER; 	/* free the midentry */
			(*sseg->pageunlock)(sseg->lock);	/* release lock */
			return PvmSysErr;			/* out of mids */
		}
		else
			control-> nmsgs ++;
	}

	/* Set the msginfo buffer parameters */
	thismsginfo = &(control->msgs[msgnum]);
	thismsginfo -> mi_src = src;
	thismsginfo -> mi_tag = tag;
	thismsginfo -> mi_len = len;
	thismsginfo -> mi_offset = offset;
	thismsginfo -> mi_midlistId = midListId;
	thismsginfo -> mi_status = ISRECVMSG;

	(*sseg->pageunlock)(sseg->lock); /* release lock */

	return midListId;	/* Valid message number */
}

/* --------- pvm_isendshm --------- */
/* return the mid of the posted send message. The mid is allocated
	from pvm_shmmidlist;
	if buf = 0, isendshm will not allocate a new mid. It will instead
	try to move any messages on the sendq to the destination.
*/
int
pvm_isendshm(sseg, buf, len, dst, tag)
struct segdesc *sseg;
char *buf;
int len;
int dst;
int tag;
{

int freebuf;
int i;
int msgnum;
int offset;
struct msgctrl *control;
struct msginfo *tstmsg;
struct shm_midlist *midentry;
struct shm_midlist *testentry;
int midListId = 0;
char *sbuf;

	if (sseg == (struct segdesc *) NULL)
		return PvmBadParam;		/* invalid shared segment */

	if (buf) /* we are actually trying to send something */
	{
		if ( (midListId = pvm_allocShmmid( &midentry)) < 0)
		return midListId;

		/* record the receive info in the midlist */
		midentry -> ml_status = ISSENDMSG;
		midentry -> ml_dst = dst;
		midentry -> ml_tag = tag;
		midentry -> ml_len = len;
		midentry -> ml_buf = buf;
		midentry -> shm_msgnum = -1;
		midentry -> ml_info = 0;
		midentry -> ml_link = midentry -> ml_rlink = (struct shm_midlist *) NULL;

		/* Put this on the sendq for this shared segment, this maintains
			point-to-point message order */
		if ( !sseg -> sendq )
			sseg -> sendq = midentry;
		else
			LISTPUTBEFORE (sseg -> sendq, midentry, ml_link, ml_rlink);

	}

	testentry = sseg -> sendq; /* get message to send */

	control = sseg->segControl;

	while ( testentry )
	{
		(*sseg->pagelock)(sseg->lock);	/* acquire lock */

		tstmsg = control -> msgs;

		for (i = 0; i < control -> nmsgs; i++)
		{
			if ( tstmsg -> mi_status & ISRECVMSG
					&& ! ( tstmsg -> mi_status & COMPLETE )
					&& ! ( tstmsg -> mi_status & BUFINFLUX ) /*another writer?*/
					&& (tstmsg -> mi_src == MPPANY ||
							tstmsg -> mi_src == testentry->ml_dst)
					&& (tstmsg -> mi_tag == MPPANY ||
							tstmsg -> mi_tag == testentry->ml_tag)
					&& (tstmsg -> mi_len >= testentry -> ml_len) )

			{
				/* record send message information in the shared buffer */
				tstmsg -> mi_status |=  BUFINFLUX; /* write in progress */
				tstmsg -> mi_tag = testentry -> ml_tag;
				tstmsg -> mi_src = pvmmynode;
				tstmsg -> mi_len = testentry -> ml_len;
				sbuf = sseg -> start + tstmsg -> mi_offset;

				/* unlock the control segment while copying */
				(*sseg -> pageunlock)(sseg -> lock);
				BCOPY (sbuf,  testentry -> ml_buf, testentry -> ml_len);
				/* NOTE: control->nmsgs may have increased while unlocked,
					this means that control->nmsgs can never decrease, or
					errors can occur */

				/* relock  to update message that has been copied */

				(*sseg -> pagelock)(sseg -> lock);

				tstmsg -> mi_status &= ~BUFINFLUX;  /* write complete */
				tstmsg -> mi_status |= COMPLETE; 	/* message complete */

				/* record information in our mid structure entry */
				testentry -> ml_status |= COMPLETE;
				break;
			}

			tstmsg ++; /* look to see if another entry can receive this one */
		}

		/* if we were able to complete this message, try to send  next one */
		if ( testentry -> ml_status & COMPLETE )
		{
			if (testentry -> ml_link)
			{
				sseg -> sendq = testentry -> ml_link;
				LISTDELETE (testentry, ml_link, ml_rlink);
				testentry = sseg -> sendq;	/* get the next send message */
			}
			else
		    {
				sseg -> sendq = testentry = ( struct shm_midlist * ) NULL;
			}
		}
	}

	return midListId;	/* Valid message number */
}

/* --------- pvm_shmmsgdone --------- */
/* test to see if a message has completed. For send messages, if the
	message has not completed, attempt to resend, if a message is
	is done, then release the mid in midListId; */
pvm_shmsgdone (midListId)
int midListId;
{
int rval;
struct shm_midlist *midentry;
struct msginfo *tstmsg;
struct segdesc *sseg;

	if (midListId < 1 || midListId >= NSHMMIDS)
		return PvmBadParam;

	midentry =  &(pvm_shmmids[midListId]);

	if (! midentry -> ml_status ) /* No Buffer defined, SEND or RECV */
		return PvmBadParam;

	if ( midentry -> ml_status & ISRECVMSG )  /* This is a receive message */
	{
		if ( midentry -> shm_msgnum < 0) /* invalid shared message num */
			return PvmBadParam;

		sseg = midentry -> shmseg;

		tstmsg = &(sseg->segControl->msgs[ midentry -> shm_msgnum ]);

		(*sseg -> pagelock)(sseg -> lock ); /* lock page and check status */

		if ( tstmsg -> mi_status & COMPLETE )		/* message completed */
		{
			midentry -> ml_status |= COMPLETE;
			midentry -> ml_info[TAGENTRY] = tstmsg -> mi_tag;
			midentry -> ml_info[SRCENTRY] = tstmsg -> mi_src;
			midentry -> ml_info[LENENTRY] = tstmsg -> mi_len;
			tstmsg -> mi_status = NOBUFFER;		/* Free this message buffer*/
		}

		(*sseg -> pageunlock)(sseg -> lock ); /* unlock page  */

	}
	else if (midentry -> ml_status & ISSENDMSG )
	{
		if (! (midentry -> ml_status & COMPLETE))
		{
			/* message isn't complete, try and send again */
			pvm_isendshm( midentry -> shmseg , (char *) NULL,
					0, 0, 0, (int *) NULL);
		}
	}

	/* Check one more time, if the send/recv operation has completed.
		if it has, mark the midlist entry as free and return that the message
		has completed */

	if ((rval = (midentry -> ml_status & COMPLETE)) != 0)
		pvm_freeShmmid( midListId);

	return rval;
}


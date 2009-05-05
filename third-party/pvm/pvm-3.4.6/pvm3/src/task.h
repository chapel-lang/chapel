
/* $Id: task.h,v 1.10 2000/06/16 16:27:33 pvmsrc Exp $ */

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
 *	task.h
 *
 *	Task descriptors.
 *
 * $Log: task.h,v $
 * Revision 1.10  2000/06/16 16:27:33  pvmsrc
 * DAMN.  The seemingly cool and efficient tp->t_flag / TF_MBNOTIFY
 * solution is not as cool as previously thought.
 * 	- each pvmd only keeps a *local* tasks table...!  D-OH!
 * 	- need to just search the wait context list for an existing
 * 		WT_TASKX mbox notify, as previously feared.
 * 	- already checking wait context list for WT_RECVINFO, so this
 * 		is not too terrible...
 * 	- Damn, though...
 * Removed TF_MBNOTIFY contant - now fricking useless.
 * (Spanker=kohl)
 *
 * Revision 1.9  2000/06/13 22:37:25  pvmsrc
 * Added new task flag (tp->t_flag):
 * 	- TF_MBNOTIFY for marking whether a task has had a mbox notify
 * 		set up or not.
 * 	- to avoid a HUGE number of redundant notifies for multiple
 * 		inserts by the same task.  D-Oh!
 * (Spanker=kohl)
 *
 * Revision 1.8  1999/07/08 19:00:25  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.7  1997/07/02  20:27:34  pvmsrc
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
 * Revision 1.6  1997/06/27  20:05:58  pvmsrc
 * Integrated WIN32 changes.
 *
 * Revision 1.5  1997/06/27  19:22:09  pvmsrc
 * Task struct updated to hold message state info about its last message
 * 	to the RM.
 * Avoids duplicate messages and helps migrated/new RMs understand task
 * 	state.
 *
 * Revision 1.4  1997/05/27  14:44:11  pvmsrc
 * Added TF_SHMCONN for tasks that connect via shared memory instead
 * 	of sockets.
 *
 * Revision 1.3  1997/01/28  19:28:25  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.2  1996/10/24  21:47:19  pvmsrc
 * Added new TF_ISTRACER constant for tracer tasks.
 *
 * Revision 1.1  1996/09/23  23:43:35  pvmsrc
 * Initial revision
 *
 * Revision 1.4  1995/07/03  19:08:36  manchek
 * added TF_DEADSND flag
 *
 * Revision 1.3  1994/10/15  19:33:47  manchek
 * added TF_FORKD flag
 *
 * Revision 1.2  1994/06/03  20:38:28  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:52  manchek
 * Initial revision
 *
 */


/* descriptor for a pvm task */

struct task {
	struct task *t_link;		/* complete list ordered by tid */
	struct task *t_rlink;
	struct task *t_plink;		/* partial list ordered by pid */
	struct task *t_prlink;
	int t_tid;					/* task id */
#ifdef WIN32
	int t_handle;				/* task handle */
#endif
	int t_ptid;					/* parent task id */
	int t_flag;					/* status */
	int t_pid;					/* unix pid */
	int t_sock;					/* t-d tcp socket */
	struct sockaddr_in t_sad;	/* addr of sock */
	int t_salen;				/* addr length */
	struct waitc *t_wait;		/* wait channel */
	struct pkt *t_txq;			/* to task */
	struct pkt *t_rxp;			/* pkt reassembly from task */
	struct pmsg *t_rxm;			/* to-us msg reassembly from task */
	int t_out;					/* pipe from stdout/err */
	char *t_authnam;			/* d-auth file name */
	int t_authfd;				/* d-auth file desc */
	char *t_a_out;				/* a.out name (info only) */
	struct mca *t_mca;			/* mca currently in use by task */
	int t_outtid;				/* dst for stdout data */
	int t_outctx;
	int t_outtag;
	int t_trctid;				/* dst for trace data */
	int t_trcctx;
	int t_trctag;
	int	t_sched;				/* scheduler responsible for task */
	int	t_schedlmsg;			/* Last message to scheduler */
	int t_status;				/* unix exit status and code */
	struct timeval t_utime;		/* user time used */
	struct timeval t_stime;		/* system time used */
	struct ccon *t_ccs;			/* list of context ids owned by task */
};

/* to reserve communication contexts */

struct ccon {
	struct ccon *c_link;		/* complete list ordered by cid */
	struct ccon *c_rlink;
	struct ccon *c_peer;		/* list attached to an owner */
	struct ccon *c_rpeer;
	int c_cid;					/* context id */
	int c_tid;					/* owner */
};

/* t_flag bits */

#define	TF_FORKD		2		/* task process is child of pvmd */
#define	TF_CONN			4		/* task is connected */
#define	TF_AUTH			8		/* task needs to auth */
#define	TF_CLOSE		16		/* should delete ctx after flush msgs */
#define	TF_DEADSND		32		/* task is deadlocked needing shmem */
#define	TF_SHMCONN		64		/* task is connected via shmem */
#define	TF_ISSCHED		0x100	/* task is scheduler */
#define	TF_ISHOSTER		0x200	/* task is hoster */
#define	TF_ISTASKER		0x400	/* task is tasker */
#define	TF_ISTRACER		0x800	/* task is tracer */
#define	TF_PRESHMCONN	0x1000	/* task is pre-connected via shmem, */
								/* can send msgs w/MM_PRIO flag set */
#define	TF_SHM			0x2000

#define	ISLOCLTASK(x)	(((x)&tidhmask)==(hosts->ht_local<<(ffs(tidhmask)-1)))

void task_init __ProtoGlarp__(( void ));
int tid_new __ProtoGlarp__(( void ));
struct task *task_new __ProtoGlarp__(( int tid ));
struct task *task_find __ProtoGlarp__(( int tid ));
struct task *task_findpid __ProtoGlarp__(( int pid ));
void task_setpid __ProtoGlarp__(( struct task *tp, int pid ));
void task_cleanup __ProtoGlarp__(( struct task *tp ));
void task_free __ProtoGlarp__(( struct task *tp ));
struct ccon *ccon_new __ProtoGlarp__(( struct task *tp ));


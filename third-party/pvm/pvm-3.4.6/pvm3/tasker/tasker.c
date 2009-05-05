
static char rcsid[] =
	"$Id: tasker.c,v 1.7 1998/01/28 19:14:29 pvmsrc Exp $";

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
*	tasker.c
*
*	Example tasker task to demonstrate and verify the protocol.
*
*	06 Mar 1994  Robert Manchek
*/


#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
/* #ifdef	IMA_LINUX */
/* #include <linux/time.h> */
/* #endif */
#include <pvm3.h>
#include <sys/wait.h>
#include <sys/resource.h>
#ifdef	IMA_RS6K
#include <sys/select.h>
#endif
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <pvmproto.h>

#if defined(SYSVBFUNC)
#include <memory.h>
#define BZERO(d,n)      memset(d,0,n)
#define BCMP(s,d,n)     memcmp(d,s,n)
#define BCOPY(s,d,n)    memcpy(d,s,n)

#else
#define BZERO(d,n)      bzero(d,n)
#define BCMP(s,d,n)     bcmp(s,d,n)
#define BCOPY(s,d,n)    bcopy(s,d,n)
#endif

#define	LISTPUTAFTER(o,n,f,r) \
	{ (n)->f=(o)->f; (n)->r=o; (o)->f->r=n; (o)->f=n; }
#define	LISTPUTBEFORE(o,n,f,r) \
	{ (n)->r=(o)->r; (n)->f=o; (o)->r->f=n; (o)->r=n; }
#define	LISTDELETE(e,f,r) \
	{ (e)->f->r=(e)->r; (e)->r->f=(e)->f; (e)->r=(e)->f=0; }

#define	TALLOC(n,t,g)	(t*)malloc((n)*sizeof(t))
#define	FREE(p)	free((char *)p)
#define	STRALLOC(s)			strcpy(TALLOC(strlen(s)+1,char,"str"),s)

#ifndef	DEBUG
#define	DEBUG	0
#endif


/*
*	 to collect stats from exited tasks
*/

struct deaddata {
	int dd_pid;				/* process id */
	int dd_es;				/* unix exit status */
	struct timeval dd_ut;	/* user time used */
	struct timeval dd_st;	/* system time used */
};

/*
*	to track live tasks
*/

struct task {
	struct task *t_link, *t_rlink;
	int t_tid;
	int t_pid;
};


int debug = DEBUG;
struct deaddata *deads = 0;		/* circ queue of dead task data */
int ndead = 1000;				/* len of deads */
int rdead = 0;					/* read ptr for deads */
int wdead = 0;					/* write ptr for deads */
struct task *mytasks = 0;


/**************************
*  task management stuff  *
*                         *
**************************/

void
task_init()
{
	mytasks = TALLOC(1, struct task, 0);
	mytasks->t_link = mytasks->t_rlink = mytasks;
	mytasks->t_tid = 0;
	mytasks->t_pid = 0;
}


/*	task_new()
*
*	Make a new task descriptor, add to list.
*/

struct task *
task_new(tid, pid)
	int tid, pid;
{
	struct task *tp;

	if (!(tp = TALLOC(1, struct task, 0))) {
		fprintf(stderr, "task_new() can't get memory\n");
		exit(1);
	}
	tp->t_tid = tid;
	tp->t_pid = pid;
	LISTPUTBEFORE(mytasks, tp, t_link, t_rlink);
	return tp;
}


void
task_free(tp)
	struct task *tp;
{
	LISTDELETE(tp, t_link, t_rlink);
	FREE(tp);
}


/*	task_find()
*
*	Find a task by its tid.
*/

struct task *
task_find(tid)
	int tid;
{
	struct task *tp;

	for (tp = mytasks->t_link; tp != mytasks; tp = tp->t_link)
		if (tp->t_tid == tid)
			return tp;
	return (struct task*)0;
}


/*	task_findpid()
*
*	Find a task by its pid.
*/

struct task *
task_findpid(pid)
	int pid;
{
	struct task *tp;

	for (tp = mytasks->t_link; tp != mytasks; tp = tp->t_link)
		if (tp->t_pid == pid)
			return tp;
	return (struct task*)0;
}


void
task_dump()
{
	struct task *tp;

	for (tp = mytasks->t_link; tp != mytasks; tp = tp->t_link)
		printf("t%x pid %d\n", tp->t_tid, tp->t_pid);
}


/******************
*  general stuff  *
*                 *
******************/

/*	reap()
*
*	Child task has exited; get its stats and put in the fifo.
*/

void
reap(sig)
	int sig;
{
	int pid;
	int es = 0;
#ifndef NOWAIT3
	struct rusage rus;
#endif

	sig = sig;

#ifdef	NOWAIT3
#ifdef	NOWAITPID
	if ((pid = wait(&es)) > 0)
#else
	while ((pid = waitpid(-1, &es, WNOHANG)) > 0)
#endif
#else	/*NOWAIT3*/
	while ((pid = wait3(&es, WNOHANG, &rus)) > 0)
#endif	/*NOWAIT3*/
	{
/*
		deads[wdead] = pid;
*/
#ifdef NOWAIT3
		deads[wdead].dd_ut.tv_sec = 0;
		deads[wdead].dd_ut.tv_usec = 0;
		deads[wdead].dd_st.tv_sec = 0;
		deads[wdead].dd_st.tv_usec = 0;
#else
		deads[wdead].dd_ut = rus.ru_utime;
		deads[wdead].dd_st = rus.ru_stime;
#endif
		deads[wdead].dd_pid = pid;
		deads[wdead].dd_es = es;
		if (++wdead >= ndead)
			wdead = 0;
	}
#ifdef	SYSVSIGNAL
	(void)signal(SIGCLD, reap);
#endif
}


main(argc, argv)
	int argc;
	char **argv;
{
	int otid;			/* tid of request */
	int tag;			/* message tag */
	int *fdlist;		/* to get libpvm fd list */
#ifdef FDSETNOTSTRUCT
	fd_set rfds, fds;
#else
	struct fd_set rfds, fds;
#endif
	int nfds;
	int mid;
	struct task *tp;

	pvm_mytid();						/* become a task */

	pvm_setopt(PvmResvTids, 1);			/* allow reserved messages */

	pvm_reg_tasker();					/* register as tasker */

	pvm_setopt(PvmRoute, PvmDontRoute);	/* no freaks talking to me */

	/* get the pvmd socket fd so we can wait with select() */

	pvm_getfds(&fdlist);
	FD_ZERO(&rfds);
	FD_SET(fdlist[0], &rfds);
	nfds = fdlist[0] + 1;

	/* install child signal handler */

	deads = TALLOC(ndead, struct deaddata, 0);
	BZERO((char*)deads, ndead * sizeof(struct deaddata));

#ifndef	SYSVSIGNAL
	(void)signal(SIGCHLD, reap);
#else
	(void)signal(SIGCLD, reap);
#endif

	task_init();

	while (1) {
/*
		task_dump();
*/
		fds = rfds;
		select(nfds,
#ifdef  FDSETISINT
				(int *)&fds, (int *)0, (int *)0,
#else
				(fd_set *)&fds, (fd_set *)0, (fd_set *)0,
#endif
				(struct timeval *)0);

	/* clean up after any croaked processes */

		while (rdead != wdead) {
			if (tp = task_findpid(deads[rdead].dd_pid)) {
				if (debug)
					printf("Exit t%x pid %d\n", tp->t_tid, tp->t_pid);

				pvm_packf("%+ %d %d %d %d %d %d",
						PvmDataFoo,
						tp->t_tid,
						deads[rdead].dd_es,
						(int)deads[rdead].dd_ut.tv_sec,
						(int)deads[rdead].dd_ut.tv_usec,
						(int)deads[rdead].dd_st.tv_sec,
						(int)deads[rdead].dd_st.tv_usec);
				pvm_send(0x80000000, SM_TASKX);
				task_free(tp);
			}
			if (++rdead >= ndead)
				rdead = 0;
		}

	/* check for task-start messages */

		while ((mid = pvm_nrecv(-1, -1)) > 0) {
			pvm_bufinfo(pvm_getrbuf(), (int *)0, &tag, &otid);
			if (tag == SM_STTASK) {
				startem();

			} else {
				printf("tasker: unknown message tag %d\n", tag);
			}
		}	/* while nrecv >0 */

	/* we need a test here to catch nrecv sys errors */
	if (mid<0) break;

	} /* while the select/nrecv loop */

	pvm_reg_tasker();
	pvm_exit();
	exit(0);
}


/*	startem()
*
*	Unpack task info from message and start it up.
*
*	SM_STTASK() {
*		uint tid
*		int flags
*		string path
*		int argc
*		string argv[argc]
*		int nenv
*		string env[nenv]
*	}
*/

int
startem()
{
	int tid;
	int flags;
	char path[1024];
	char buf[1024];
	int ac;
	char **av;
	int nenv;
	char **envp;
	int i;
	int pid;

	pvm_unpackf("%ud %d %s %d", &tid, &flags, path, &ac);
	av = TALLOC(ac + 1, char *, 0);
	av[ac] = 0;
	for (i = 0; i < ac; i++) {
		pvm_unpackf("%s", buf);
		av[i] = STRALLOC(buf);
	}
	pvm_unpackf("%d", &nenv);
	envp = TALLOC(nenv + 1, char *, 0);
	envp[nenv] = 0;
	for (i = 0; i < nenv; i++) {
		pvm_unpackf("%s", buf);
		envp[i] = STRALLOC(buf);
	}
	if (debug) {
		printf("tid t%x flags %x path \"%s\"\n", tid, flags, path);
		printf("argc %d\n", ac);
		for (i = 0; i < ac; i++)
			printf(" %d \"%s\"\n", i, av[i]);
		printf("nenv %d\n", nenv);
		for (i = 0; i < nenv; i++)
			printf(" %d \"%s\"\n", i, envp[i]);
	}

	if ((pid = dofork(path, ac, av, nenv, envp)) != -1) {
		task_new(tid, pid);
		if (debug)
			printf("Forked %d\n", pid);

	} else {
		pvm_packf("%+ %d %d %d %d %d %d", PvmDataFoo, tid, 0, 0, 0, 0, 0);
		pvm_send(0x80000000, SM_TASKX);
	}
	return pid;
}


/*	dofork()
*
*	Fork and exec task, add changes to environment first.
*/

int
dofork(path, argc, argv, nenv, envp)
	char *path;
	int argc;
	char **argv;
	int nenv;
	char **envp;
{
	int pid;

	if (pid = fork()) {

	} else {
		pvmendtask();
		while (nenv-- > 0)
			pvmputenv(envp[nenv]);
		execv(path, argv);
/*
		execve(path, argv, envp);
*/
		fprintf(stderr, "dofork() aaugh, bit it\n");
		_exit(1);
	}

	return pid;
}



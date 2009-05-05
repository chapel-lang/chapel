
static char rcsid[] =
	"$Id: calc.c,v 1.3 1999/03/16 16:01:43 pvmsrc Exp $";

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
 *	calc.c
 *
 *	Bag of tasks driver for tiled workers.
 *	Manages idle, busy, failed workers and message routing sockets.
 *
 *	Oct 95 Manchek
 */


#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include <stdio.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Form.h>
#include <pvm3.h>
#include "../src/bfunc.h"
#include "../src/listmac.h"
#include "myalloc.h"
#include "hostc.h"
#include "imp.h"


#ifndef	min
#define	min(a,b)	((a)<(b)?(a):(b))
#endif
#ifndef	max
#define	max(a,b)	((a)>(b)?(a):(b))
#endif


#define	JobSend			1			/* send tile to worker */
#define	JobReturn		2			/* get image back from worker */
#define	AddMessage		3			/* hosts were added */
#define	DelMessage		4			/* hosts were deleted */
#define	ExitMessage		5			/* worker exited */
#define	RouteAddTag		6			/* new route socket opened */
#define	RouteDeleteTag	7			/* route socket closed */

#define	TILEHEIGHT	10

struct worker {
	struct worker *link, *rlink;	/* dll of active or idle */
	int tid;						/* worker tid */
	struct job *job;				/* number */
	XtInputId route;				/* X id for route socket */
};

struct job {
	struct job *link, *rlink;
	int tile;						/* tile number */
};


int gotmorehosts();


/***************
 **  Globals  **
 **           **
 ***************/

extern struct canvas imCan;	/* from xep.c */
extern int dobars;			/* from xep.c */
extern char *workerfile;	/* from xep.c */
extern int nworkers;		/* from xep.c */

int mytid;					/* pvm tid */
struct worker *active = 0;	/* active workers */
struct worker *idle = 0;	/* idle workers */
struct job *todo = 0;		/* jobs to be started */
int ntiles = 0;				/* number of tiles in frame */


pvminit()
{
	if ((mytid = pvm_mytid()) < 0)
		exit(1);

	pvm_notify(PvmRouteAdd, RouteAddTag, -1, (int *)0);
	pvm_setopt(PvmRoute, PvmRouteDirect);

	host_init(AddMessage, DelMessage, gotmorehosts, (int (*)())0);

	idle = TALLOC(1, struct worker, "worker");
	idle->link = idle->rlink = idle;
	idle->tid = 0;
	idle->job = 0;
	active = TALLOC(1, struct worker, "worker");
	active->link = active->rlink = active;
	active->tid = 0;
	active->job = 0;

	todo = TALLOC(1, struct job, "job");
	todo->link = todo->rlink = todo;

	return 0;
}


more_workers()
{
	int nh;
	int i;
	struct hostc *hp;
	struct worker *wp;
	int tid;

	hp = 0;
	nh = 0;
	while (hp = host_next(hp))
		nh++;

	nh -= nworkers;
	if (nh <= 0)
		return 0;

	hp = 0;
	while (hp = host_next(hp)) {
		i = 0;
		for (wp = idle->link; wp != idle; wp = wp->link)
			if (pvm_tidtohost(wp->tid) == hp->pvmd_tid) {
				i = 1;
				break;
			}
		if (i)
			continue;
		for (wp = active->link; wp != active; wp = wp->link)
			if (pvm_tidtohost(wp->tid) == hp->pvmd_tid) {
				i = 1;
				break;
			}
		if (i)
			continue;
		if (pvm_spawn(workerfile, (char**)0, PvmTaskHost, hp->name, 1, &tid)
		< 0) {
			pvm_exit();
			exit(1);
		}
		if (tid > 0) {
			wp = TALLOC(1, struct worker, "worker");
			wp->tid = tid;
			wp->job = 0;
			wp->route = 0;
			LISTPUTBEFORE(idle, wp, link, rlink);
			pvm_notify(PvmTaskExit, ExitMessage, 1, &tid);
#ifdef	DEBUG
			fprintf(stderr, "more_workers() new worker 0x%x\n", tid);
#endif
			nworkers++;
		}
	}

	setlabel();
	return 0;
}


stop_workers()
{
	struct worker *wp;

	while ((wp = idle->link) != idle) {
#ifdef	DEBUG
		fprintf(stderr, "stop_workers() killing 0x%x\n", wp->tid);
#endif
		pvm_kill(wp->tid);
		LISTDELETE(wp, link, rlink);
		if (wp->route) {
/*
			fprintf(stderr, "removeaninput() xii %ld\n", wp->route);
*/
			XtRemoveInput(wp->route);
			wp->route = 0;
		}
		MY_FREE(wp);
	}
	while ((wp = active->link) != active) {
#ifdef	DEBUG
		fprintf(stderr, "stop_workers() killing 0x%x\n", wp->tid);
#endif
		pvm_kill(wp->tid);
		LISTDELETE(wp, link, rlink);
		if (wp->route) {
/*
			fprintf(stderr, "removeaninput() xii %ld\n", wp->route);
*/
			XtRemoveInput(wp->route);
			wp->route = 0;
		}
		MY_FREE(wp);
	}
	nworkers = 0;
	setlabel();
	return 0;
}


do_recalc()
{
	struct worker *wp;
	struct job *jp;
	int ht = imCan.cn_ht;
	int i;

	/* toss anything that's already on the todo list */

	while ((jp = todo->link) != todo) {
		LISTDELETE(jp, link, rlink);
		MY_FREE(jp);
	}

	/* scrub in-progress jobs */

	for (wp = active; (wp = wp->link) != active; )
		wp->job->tile = -1;

	/* fill it with new tiles */

	ntiles = ht / TILEHEIGHT + 1;

	for (i = 0; i < ntiles; i += 8) {
		jp = TALLOC(1, struct job, "job");
		jp->tile = i;
		LISTPUTBEFORE(todo, jp, link, rlink);
	}
	for (i = 4; i < ntiles; i += 8) {
		jp = TALLOC(1, struct job, "job");
		jp->tile = i;
		LISTPUTBEFORE(todo, jp, link, rlink);
	}
	for (i = 2; i < ntiles; i += 4) {
		jp = TALLOC(1, struct job, "job");
		jp->tile = i;
		LISTPUTBEFORE(todo, jp, link, rlink);
	}
	for (i = 1; i < ntiles; i += 2) {
		jp = TALLOC(1, struct job, "job");
		jp->tile = i;
		LISTPUTBEFORE(todo, jp, link, rlink);
	}

	assign_work();
	return 0;
}


/*	assign_work()
*
*	Send off tiles to idle workers.
*/

assign_work()
{
	struct job *jp;
	struct worker *wp;
	double im1 = imCan.cn_im1;
	double im2 = imCan.cn_im2 - im1;
	int ht = imCan.cn_ht;
	int wd = imCan.cn_wd;
	double reim[4];					/* tile corner coords */
	int wdht[2];					/* tile wd, ht */
	int y1, y2;

	reim[0] = imCan.cn_re1;
	reim[2] = imCan.cn_re2;
	wdht[0] = wd;

	while (idle->link != idle && todo->link != todo) {
		jp = todo->link;
		LISTDELETE(jp, link, rlink);
		y2 = ((jp->tile + 1) * ht) / ntiles;
		y1 = (jp->tile * ht) / ntiles;
		reim[1] = im1 + (y1 * im2) / ht;
		reim[3] = im1 + (y2 * im2) / ht;
		wdht[1] = y2 - y1;
		wp = idle->link;
		LISTDELETE(wp, link, rlink);
		LISTPUTBEFORE(active, wp, link, rlink);
		wp->job = jp;
#ifdef	DEBUG
		fprintf(stderr, "sent job %d to 0x%x: %dx%d %f,%f/%f,%f\n",
				jp->tile, wp->tid,
				wdht[0], wdht[1], reim[0], reim[1], reim[2], reim[3]);
#endif
		pvm_packf("%+ %2lx %2d", PvmDataDefault, reim, wdht);
		if (pvm_send(wp->tid, JobSend)) {
			pvm_exit();
			exit(1);
		}

		if (dobars)
			label_row(y1, y2, wp->tid);
	}

	/*
	* we must check receive here because messages may have arrived
	* while we were sending, so the socket will not be ready to
	* read to wake us up.  a bit sick.
	*/

	while (pvm_nrecv(-1, -1) > 0)
		claim_message();
	return 0;
}


claim_message()
{
	struct job *jp;
	struct worker *wp;
	int tag;
	int tid;		/* id of sender */
	int wd = imCan.cn_wd;
	int ht = imCan.cn_ht;
	double im1 = imCan.cn_im1;
	double im2 = imCan.cn_im2 - im1;
	double reim[4];					/* tile corner coords */
	int wdht[2];					/* tile wd, ht */
	int y1, y2;						/* tile start, end rows */
	int h;							/* height of tile */

	pvm_bufinfo(pvm_getrbuf(), (int*)0, &tag, &tid);
#ifdef	DEBUG
	fprintf(stderr, "message %d from 0x%x\n", tag, tid);
#endif

	if (tag == AddMessage) {
		host_add();

	} else if (tag == DelMessage) {
		host_delete();

	} else if (tag == ExitMessage) {
		pvm_upkint(&tid, 1, 1);
		for (wp = idle->link; wp != idle; wp = wp->link) {
			if (wp->tid == tid) {
				LISTDELETE(wp, link, rlink);
				if (wp->route) {
/*
					fprintf(stderr, "removeaninput() xii %ld\n", wp->route);
*/
					XtRemoveInput(wp->route);
					wp->route = 0;
				}
				MY_FREE(wp);
				nworkers--;
				setlabel();
				goto tryagain;
			}
		}
		for (wp = active->link; wp != active; wp = wp->link) {
			if (wp->tid == tid) {
				int i;

				LISTDELETE(wp, link, rlink);
				jp = wp->job;
				if (jp->tile >= 0) {
					LISTPUTAFTER(todo, jp, link, rlink);
					y1 = (jp->tile * ht) / ntiles;
					y2 = ((jp->tile + 1) * ht) / ntiles;
					h = y2 - y1;
					for (i = wd * h; i-- > 0; )
						*((char*)imCan.cn_dat + y1 * wd + i) = 32;
/*
					BZERO((char*)imCan.cn_dat + y1 * wd, wd * h);
*/
					repaint_region(&imCan, 0, y1, wd - 1, y2 - 1);
					refresh_region(&imCan, 0, y1, wd - 1, y2 - 1);
				} else {
					MY_FREE(jp);
				}
				if (wp->route) {
/*
					fprintf(stderr, "removeaninput() xii %ld\n", wp->route);
*/
					XtRemoveInput(wp->route);
					wp->route = 0;
				}
				MY_FREE(wp);
				nworkers--;
				setlabel();
/*
				assign_work();
*/
				goto tryagain;
			}
		}

tryagain:
		gotmorehosts();		/* hope we can start another */

	} else if (tag == JobReturn) {
		for (wp = active->link; wp != active; wp = wp->link)
			if (wp->tid == tid)
				break;
		if (wp == active) {
			fprintf(stderr, "bogus message?\n");
			return 0;
		}
		LISTDELETE(wp, link, rlink);
		jp = wp->job;

		if (jp->tile >= 0) {
#ifdef	DEBUG
			fprintf(stderr, "got job %d from 0x%x\n", jp->tile, wp->tid);
#endif
			y1 = (jp->tile * ht) / ntiles;
			y2 = ((jp->tile + 1) * ht) / ntiles;
			h = y2 - y1;
			pvm_unpackf("%*c", wd * h, (char*)imCan.cn_dat + y1 * wd);

			if (dobars)
				label_row(y1, y2, wp->tid);

			repaint_region(&imCan, 0, y1, wd - 1, y2 - 1);
			refresh_region(&imCan, 0, y1, wd - 1, y2 - 1);
		}

		MY_FREE(jp);

		/* put work server back on free list */

		LISTPUTBEFORE(idle, wp, link, rlink);

		/* assign more work if available */

		assign_work();

	} else if (tag == RouteAddTag) {
		int tid, fd;
		XtInputId xii;

		pvm_unpackf("%d %d", &tid, &fd);
		fprintf(stderr, "got route add notify tid 0x%x fd %d\n", tid, fd);
		for (wp = idle->link; wp != idle; wp = wp->link)
			if (wp->tid == tid)
				break;
		if (wp == idle)
			for (wp = active->link; wp != active; wp = wp->link)
				if (wp->tid == tid)
					break;
		if (wp == active) {
			fprintf(stderr, "route add notify for worker not mine tid 0x%x\n",
					tid);

		} else {
			addaninputfile(fd, &wp->route);
			pvm_notify(PvmRouteDelete, RouteDeleteTag, 1, &tid);
		}

	} else if (tag == RouteDeleteTag) {
		int tid, fd;
		XtInputId xii;

		pvm_unpackf("%d %d", &tid, &fd);
		fprintf(stderr, "got route delete notify tid 0x%x fd %d\n", tid, fd);
		for (wp = idle->link; wp != idle; wp = wp->link)
			if (wp->tid == tid)
				break;
		if (wp == idle)
			for (wp = active->link; wp != active; wp = wp->link)
				if (wp->tid == tid)
					break;
		if (wp == active) {
			fprintf(stderr,
					"route delete notify for worker not mine tid 0x%x\n", tid);

		} else if (wp->route) {
/*
			removeaninputfile(wp->route);
*/
/*
			fprintf(stderr, "removeaninput() xii %ld\n", wp->route);
*/
			XtRemoveInput(wp->route);
			wp->route = 0;
		}
	}

	return 0;
}


int
gotmorehosts()
{
#ifdef	DEBUG
	fprintf(stderr, "host added\n");
#endif
	more_workers();
	assign_work();
	return 0;
}



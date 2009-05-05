
/* $Id: waitc.h,v 1.8 1999/07/08 19:00:26 kohl Exp $ */

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
 *	waitc.h
 *
 *	Wait context descriptors.
 *
 * $Log: waitc.h,v $
 * Revision 1.8  1999/07/08 19:00:26  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.7  1997/06/25  22:11:15  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.6  1997/06/16  13:39:11  pvmsrc
 * Updated struct waitc_spawn to include additional start info required
 * 	by the GRM.
 *
 * Revision 1.5  1997/04/10  17:51:39  pvmsrc
 * Added new wait context constant WT_RECVINFO.
 * 	- for blocking calls to pvm_recvinfo().
 *
 * Revision 1.4  1997/04/08  17:27:26  pvmsrc
 * Added WT_RESET constant.
 *
 * Revision 1.3  1997/03/06  21:14:24  pvmsrc
 * 	take out names in prototype.
 *
 * Revision 1.2  1997/01/28  19:28:37  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:43:40  pvmsrc
 * Initial revision
 *
 * Revision 1.2  1994/06/03  20:38:33  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:52  manchek
 * Initial revision
 *
 */


/* Wait context info */

struct waitc {
	struct waitc *wa_link;		/* list of all, ordered by wid */
	struct waitc *wa_rlink;
	int wa_wid;					/* unique identifier, >0 */
	int wa_kind;				/* why waiting */
	int wa_on;					/* entity waiting on */
	int wa_tid;					/* entity waiting */
	int wa_dep;					/* remote wait id that depends on us or 0 */
	struct waitc *wa_peer;		/* dll of peers, not sorted */
	struct waitc *wa_rpeer;
/* wait kind-specific stuff below */
	struct pmsg *wa_mesg;		/* message to be sent on completion */
	int wa_count;				/* number of times to trigger */
	void *wa_spec;				/* for kind-specific data */
};

/* Wait kinds */

#define	WT_ADDHOST		1	/* hosts being added for us */
#define	WT_SPAWN		2	/* tasks being spawned for us */
#define	WT_HOSTSTART	3	/* hoster starting slaves for us */
#define	WT_TASK			4	/* getting task list from other pvmds */
#define	WT_DELHOST		5	/* hosts being deleted for us */
#define	WT_HTUPD		6	/* host table update waiting to commit */
#define	WT_PSTAT		7	/* query status of task */
#define	WT_TASKX		8	/* waiting for task to exit */
#define	WT_MSTAT		9	/* query status of host */
#define	WT_DB			10	/* name-server query */
#define	WT_HOSTF		11	/* waiting for host to fail */
#define	WT_HOSTA		12	/* waiting for hosts to be added */
#define	WT_HOSTSYNC		13	/* get remote time of day clock */
#define	WT_TASKSTART	14	/* tasker starting task */
#define	WT_ROUTEA		15	/* notify if route created */
#define	WT_ROUTED		16	/* notify if route deleted */
#define	WT_RESET		17	/* waiting for reset task to exit */
#define	WT_RECVINFO		18	/* waiting for mbox entry put */

void wait_init __ProtoGlarp__(( int base, int range ));
struct waitc *wait_new __ProtoGlarp__(( int kind ));
struct waitc *wait_find __ProtoGlarp__(( int wid ));
void wait_delete __ProtoGlarp__(( struct waitc *wp ));
void wait_dump __ProtoGlarp__(( struct waitc *wp ));
void wait_dumpall __ProtoGlarp__(( void ));
struct waitc *wait_get __ProtoGlarp__(( struct hostd *, struct pmsg *, int ));

struct waitc_spawn {
	char *w_file;				/* executable name */
	int w_flags;				/* spawn flags */
	int w_argc;					/* length of argv */
	char **w_argv;				/* arg list */
	struct htab *w_ht;			/* set of usable hosts */
	int *w_vec;					/* result/status vector */
	int w_veclen;				/* length of wa_tids */
	int w_togo;					/* num of unresolved in vec */
	int w_outtid;				/* stdout dst */
	int w_outctx;
	int w_outtag;
	int w_trctid;				/* trace dst */
	int w_trcctx;
	int w_trctag;
	int w_nenv;					/* length of env */
	char **w_env;				/* envar list */
	int w_ptid;					/* parent task id */
	int w_sched;				/* task's scheduler */
	int w_instance;				/* we are the n'th task */
	int w_hosttotal;			/* how many on this host alone */
	int w_outof;				/* total to spawn */
};

struct waitc_add {
	int w_num;				/* number of hosts */
	struct hostd **w_hosts;	/* vector of host entries */
};


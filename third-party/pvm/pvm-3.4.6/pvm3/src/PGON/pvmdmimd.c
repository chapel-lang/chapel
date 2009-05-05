
static char rcsid[] =
	"$Id: pvmdmimd.c,v 1.11 2005/08/22 15:13:55 pvmsrc Exp $";

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
 *	pvmdmimd.c
 *
 *  MPP interface.
 *
 *		void mpp_init(int argc, char **argv):	
 *			Initialization. Create a table to keep track of active nodes.
 *			argc, argv: passed from main.
 *
 *		int mpp_load( struct waitc_spawn *wxp ) 
 *
 *			Load executable onto nodes; create new entries in task table,
 *			encode node number and process type into task IDs, etc.
 *
 *				Construction of Task ID:
 *
 *			 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 *			+-+-+-----------------------+-+-----+--------------------------+
 *			|s|g|		host index	    |n| prt |    	node # (16384)	   |
 *			+-+-+-----------------------+-+-----+--------------------------+
 *
 *			The "n" bit is set for node task but clear for host task.
 *
 *			flags:	exec options;
 *			name:	executable to be loaded;
 *			argv:	command line argument for executable
 *			count:	number of tasks to be created;
 *			tids:	array to store new task IDs;
 *			ptid:	parent task ID.
 *
 *			mpp_new(int count, int ptid):		
 *				Allocate a set of nodes. (called by mpp_load())
 *				count: number of nodes;  ptid: parent task ID.
 *
 *		int mpp_output():	
 *			Send all pending packets to nodes via native send. Node number
 *			and process type are extracted from task ID.
 *
 *		int mpp_mcast(int src, struct pkt pp, int tids[], int ntask):	
 *			Global send.
 *			src:	source task ID;
 *			pp:		packet;
 *			tids:	list of destination task IDs;
 *			ntask:	how many.
 *
 *		int mpp_probe():	
 *			Probe for pending packets from nodes (non-blocking). Returns
 *			1 if packets are dectected, otherwise 0.
 *
 *		void mpp_input():	
 *			Receive pending packets (from nodes) via native recv.
 *
 *		struct task *mpp_find(int pid):
 *			Find a task in task table by its Unix pid.
 *
 *		void mpp_free(struct task *tp):
 *			Remove node/process-type from active list.
 *			tp: task pointer.
 *
 */

/*
 *
 * $Log: pvmdmimd.c,v $
 * Revision 1.11  2005/08/22 15:13:55  pvmsrc
 * Added #include <pvmtev.h> for #include global.h...  :-Q
 * 	- submitted by Skipper Hartley <charles.l.hartley@bankofamerica.com>
 * (Spanker=kohl)
 *
 * Revision 1.10  2002/02/21 23:19:28  pvmsrc
 * Added new (not to be documented!) PVM_MAX_TASKS env var support.
 * 	- for Mahir Lokvancic <mahir@math.ufl.edu>.
 * 	- forcefully limits the number of tasks that can attach to a
 * 		pvmd, required on Solaris in rare circumstances when hard
 * 		FD_SETSIZE limit is reached, and all hell breaks loose...
 * 	- check return for task_new() call, can now produce NULL ptr,
 * 		indicating PvmOutOfRes...
 * (Spanker=kohl)
 *
 * Revision 1.9  2000/02/17 21:10:31  pvmsrc
 * Cleaned up comments...  mpp_load() args...
 * (Spanker=kohl)
 *
 * Revision 1.8  1999/07/08 19:00:27  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.7  1997/07/09  13:54:32  pvmsrc
 * Fixed Author Header.
 *
 * Revision 1.6  1997/06/02  13:49:06  pvmsrc
 * Moved #include waitc.h below #include host.h.
 *
 * Revision 1.5  1997/05/05  20:07:48  pvmsrc
 * 	Fix so that output is collected properly. One bug remains:
 * 	tasks can exit before all of the output is collected. Hmm.
 *
 * Revision 1.4  1997/04/29  20:30:27  pvmsrc
 * 	Use new call sequences defined in mppmsg.h
 * 	Use the asynchronous send list management functions.
 *
 * Revision 1.3  1997/03/06  21:19:42  pvmsrc
 * 		Wholesale changes to this code. Tried for simplification.
 * 		still needs work.
 *
 * Revision 1.2  1997/01/28  19:30:31  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:13:45  pvmsrc
 * Initial revision
 *
 * Revision 1.12  1995/11/02  15:59:01  manchek
 * fixed so spawned tasks inherit pvmd environment plus parent task env
 *
 * Revision 1.11  1995/07/28  20:30:38  manchek
 * pvmtxt should have been etext
 *
 * Revision 1.10  1995/07/25  17:40:26  manchek
 * mpp_output returns int
 *
 * Revision 1.9  1995/07/24  20:03:15  manchek
 * message header no longer part of packet data, goes in pkt struct.
 * drivers must strip and reconstitute headers
 *
 * Revision 1.8  1995/06/16  16:16:06  manchek
 * mpp_load passes trace and output sink to task
 *
 * Revision 1.7  1995/06/12  16:03:20  manchek
 * added PGON partition size to pvminfo array
 *
 * Revision 1.6  1995/05/30  17:21:41  manchek
 * Handle absolute path name properly in mpp_load() (changes from forkexec).
 * mpp_free() takes struct task instead of tid.
 * Declare partsize static.
 * Fixed bug in node allocation in mpp_new().
 * mpp_new() opens pipe to collect stdout
 *
 * Revision 1.5  1995/02/01  20:51:05  manchek
 * added nenv and envp args to mpp_load
 *
 * Sat Dec  3 14:54:20 EST 1994
 *		copy new code that handles absolute filename to mpp_load() 
 *		from forkexec()
 *
 * Revision 1.4  1994/11/07  21:30:45  manchek
 * Modify mpp_output() and mpp_mcast() to send a null packet to alert precv().
 * Modify mpp_input() to handle DataInPlace properly.
 * mpp_new() should return PvmOutOfRes when it runs out of nodes.
 * Fix a bug in the way ptype is computed in mpp_mcast()
 *
 * Revision 1.3  1994/06/03  20:54:52  manchek
 * version 3.3.0
 *
 * Revision 1.2  1993/12/20  15:39:49  manchek
 * patch 6 from wcj
 *
 * Revision 1.1  1993/08/30  23:36:09  manchek
 * Initial revision
 *
 * Jul 12 23:57:07 EDT 1993
 *      deleted loclinput(), and merged loclinpkt() into pvmd.c
 */

#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#ifdef  SYSVSTR
#include <string.h>
#define CINDEX(s,c) strchr(s,c)
#else
#include <strings.h>
#define CINDEX(s,c) index(s,c)
#endif

#if defined(IMA_PGON)
#include <nx.h>
#endif

#if defined(IMA_PGONPUMA)
#include <hostlib.h>
#endif
#include <pvm3.h>
#include <pvmproto.h>
#include <pvmtev.h>
#include "global.h"
#include "pvmalloc.h"
#include "host.h"
#include "waitc.h"
#include "pkt.h"
#include "task.h"
#include "listmac.h"
#include "pvmdmp.h"
#include "pvmmimd.h"
#include "bfunc.h"
#include "lmsg.h"
#include "mppchunk.h"
#include "mppmsg.h"

#ifndef min
#define min(a,b)	((a)<(b)?(a):(b))
#endif


#if defined(IMA_PGONPUMA)
#include <hostlib.h>
#endif
/* Global */

extern char **pvmcopyenv();
extern int pvmenvinsert();
extern int pvmfreeenv();

extern char **environ;

extern int pvmdebmask;			/* from pvmd.c */
extern char **epaths;			/* from pvmd.c */
extern int myhostpart;			/* from pvmd.c */
extern struct htab *hosts;		/* from pvmd.c */
extern int tidhmask;			/* from pvmd.c */
extern int ourudpmtu;			/* from pvmd.c */

int tidtmask = TIDPTYPE;		/* mask for ptype field of tids */
int tidnmask = TIDNODE;			/* mask for node field of tids */

#if defined(IMA_PGONPUMA)
static short phystolmap[2048];		/* XXX HACK. match physical to logical node*/
#endif
/* private */

static struct nodeset *busynodes;	/* active nodes; ordered by proc type */
static char etext[512];			/* scratch for error log */
static int ptypemask;			/* mask; we use these bits of ptype in tids */
static long isendmid = -1;		/* msg ID returned by isend() */
static struct pkt *outpkt = 0;	/* packet being sent */
static int partsize;			/* size of partition */
static int taskstdout;			/* stdout/stderr of pvmd and node tasks */
static int myndf = 0;			/* obsolete.  Fix this from pvmd.c */


/* information to handle multiple recv bufs */
static MPP_DIRECTI_PTR mpdirect = (MPP_DIRECTI_PTR) NULL; 

/* the receive buffers, themselves */
static MSG_INFO_PTR	mprecvbufs =  (MSG_INFO_PTR) NULL; 

/* need to keep track of packets that need to be written to tasks */
static struct pkt *mppopq = (struct pkt *) NULL; 

/* array of mids of outstanding messages to be sent to backend */
#define NMPPSBUFMIDS 32
#define MPPMIDFREE -1
#define MPPMIDALLOCED -2
static msgmid_t mppsendmids[NMPPSBUFMIDS];
static struct pkt *mppoutpkts[NMPPSBUFMIDS]; 
static int lastidx = 0;

static int appid = -1;
static int killing_appid = 0;

void
mpp_init(argc, argv)
	int *argc;
	char **argv;
{
	int i;
	int cc;

#if defined(IMA_PGON) 
	if ((partsize = nx_initve((char *)0, 0, "", argc, argv)) < 0) 
	{
		pvmlogperror("mpp_init() nx_initve\n");
		pvmbailout();
	}
	sprintf(etext, "using %d nodes\n", partsize);
	pvmlogerror(etext);
#endif

#if defined(IMA_PGONPUMA)
   	if ( (cc = host_lib_init()) < 0)
	{
		pvmlogperror("mpp_init(): host_lib_int\n"); 	
		pvmbailout();
	}
	for (i=0; i < (sizeof(phystolmap)/sizeof(short)); i++)
		phystolmap[i] = -1;
#endif

	busynodes = TALLOC(1, struct nodeset, "nsets");
	BZERO((char*)busynodes, sizeof(struct nodeset));
	busynodes->n_link = busynodes;
	busynodes->n_rlink = busynodes;

	ptypemask = tidtmask >> (ffs(tidtmask) - 1);

	/* need a list of packets that have been sent */
	mppopq = TALLOC(1, struct pkt , "mppopq");
	mppopq->pk_link = mppopq;
	mppopq->pk_rlink = mppopq;;	

	pvm_init_asynch_list( mppsendmids, (CHUNK_PTR *) mppoutpkts, NMPPSBUFMIDS);
}

/*
 *	allocate a set of nodes; ptype is always 0, so all tasks can exchange
 * 	messages directly, bypassing pvmd
 *
 *	Node sets are allocated in blocks. When count processes are needed,
 *	There must be a contiguous range of free nodes of size count 
*/

struct nodeset *
mpp_new(count, ptid)
	int count;		/* number of nodes requested */
	int ptid;		/* parent's tid */
{

	static int first = 1;
	int i;
	int last = -1;
	int pfd[2];		/* pipe to read stdout */

	long ptype = busynodes->n_ptype;

	struct nodeset *sp, *newp;

	ptype = 0;

#if defined(IMA_PGONPUMA)
	/* XXX Only run a single PUMA partition for now */
	if (appid != -1)
	{
		pvmlogerror("mpp_new() only one partition allowed, sorry\n");
		return (struct nodeset *) 0;
	}
	partsize = count;
#endif
	if (first)	/* take care of task input/output */ 
	{
		if (pipe(pfd) != -1)  	/* pfd[0] is a read, pfd[1] is a write */
		{
			dup2(pfd[1], 1);	/* send standard output write end of pipe */
			dup2(1, 2);			/* and stderr to write end of pipe */
			taskstdout = pfd[0];
			wrk_fds_add(taskstdout, 1);
		} 
		else
		{
			pvmlogperror("mpp_new() pipe");
		}

		first = 0;
	}

	if (!(newp = TALLOC(1, struct nodeset, "nsets"))) 
	{
		pvmlogerror("nodes_new() can't get memory\n");
		pvmbailout(0);
	}
	BZERO((char*)newp, sizeof(struct nodeset));
	
	newp->n_size = count;

	/* this loop is bypassed on the first mpp_new */
	for (sp = busynodes->n_link; sp != busynodes; sp = sp->n_link) 
	{
		if (sp->n_ptype == ptype) 
		{
			if (sp->n_first - last > count)
				goto done;
			else 
			{
				last = sp->n_first + sp->n_size - 1;
				if (sp->n_link == busynodes && partsize - last > count)
					goto done;
			}
		} 
		else if (sp->n_ptype > ptype) 
		{
			if (partsize - last > count)
				goto done;
			else 
			{
				ptype = sp->n_ptype;	/* go to the next layer */
				last = -1;
			}
		}
	} /* end the for loop */

	if (partsize - last <= count) 
	{
		pvmlogerror("mpp_new() not enough nodes in partition\n");
		PVM_FREE(newp);
		return (struct nodeset *) 0;
	}

done:
	if (pvmdebmask & PDMNODE) {
		sprintf(etext, "mpp_new() %d nodes %d ... ptype=%d ptid=%x\n",
			count, last+1, ptype, ptid);
		pvmlogerror(etext);
	}
	newp->n_first = last + 1;
	newp->n_ptype = ptype;
	newp->n_alive = count;
	newp->n_ptid = ptid;

	LISTPUTAFTER(sp, newp, n_link, n_rlink);

	return newp;
}

/* remove nodes/ptype from active list */
void
mpp_free(tp)
	struct task *tp;
{
	struct nodeset *sp;
	int i;
	int node;
	int ptype;
	int tid = tp->t_tid;

	if (!TIDISNODE(tid))
		return;

	node = tid & tidnmask;
	ptype = TIDTOTYPE(tid);
	tp->t_out = -1;

	for (sp = busynodes->n_link; sp != busynodes; sp = sp->n_link) 
	{
		if ((sp->n_ptype & ptypemask) == ptype && node >= sp->n_first
				&& node - sp->n_first < sp->n_size) 
		{

			if (pvmdebmask & PDMNODE) {
				sprintf(etext, "mpp_free() t%x type=%ld alive=%d\n",
					tid, sp->n_ptype, sp->n_alive);
				pvmlogerror(etext);
			}
			if (--sp->n_alive == 0) 
			{
				LISTDELETE(sp, n_link, n_rlink);
				PVM_FREE(sp);

#if defined(IMA_PGONPUMA)
				if (pvmdebmask & PDMNODE) {
					sprintf(etext, "mpp_free() freeing %d recv bufs \n", NRBUFS);
					pvmlogerror(etext);
				}
				for (i=0; i < NRBUFS; i++)
				{
					pk_free(mprecvbufs->rpkt);
					mprecvbufs++;
				}

				mprecvbufs = (MSG_INFO_PTR) NULL;

				if (pvmdebmask & PDMNODE) {
					sprintf(etext, "mpp_free() mprecvbufs is %x \n", mprecvbufs);
					pvmlogerror(etext);
				}

				for (i=0; i < partsize; i++)
				{
					if (mpdirect->ordering)
						PVM_FREE(mpdirect->ordering);
					PVM_FREE(mpdirect);
					mpdirect ++;
				}

				mpdirect = (MPP_DIRECTI_PTR) NULL;

				partsize = -1;

				if (!killing_appid)
					kill_app(appid);

				disc_app(appid);

				appid = -1;

				killing_appid = 0;
#endif

			}
			return;
		}
	}
	sprintf(etext, "mpp_free() t%x not active\n", tid);
	pvmlogerror(etext);
	return;
}

/* load executable onto the given set of nodes */
int
mpp_load( wxp )
struct waitc_spawn *wxp;
{
int flags;              /* exec options */
char *name;             /* executable */
char **argv;            /* arg list (argv[-1] must be there) */
int count;				/* how many */
int *tids;				/* array to store new tids */
int ptid;				/* parent task ID */
int nenv;				/* length of environment */
char **envp;			/* environment strings */
int outtid;				/* output tid */
int outcod;				/* output code */
int trctid;				/* tid for trace messages */
int trccod;				/* code to use on trace messages */

    char **ep, **eplist;
	static char *nullep[] = { "", 0 };
    char path[MAXPATHLEN];
	char **cenv;

	int i, cc;
	int err = 0;
	static int first = 1;
	int j;
	int cnode;
	int cpid;
	int pvminfo[SIZEHINFO];	/* proto, myset, parent tid, frag size, NDF */
	int ptypepart;

	int *pids = 0;			/* array of OSF/1 process IDs */
	int *nodes = 0;
	int nnodes;			/* number of nodes to needed */

	msgmid_t mid;

	info_t minfo[MPPINFOSIZE];

	struct nodeset *sp;

    struct stat sb;

	struct task *tp;

	MPP_DIRECTI_PTR tdirect;

	MSGFUNC_PTR mfunc;

	/* -- initialize some variables from the struct waitc_spawn  struct -- */

	name = wxp->w_argv[0];
	argv = wxp->w_argv;
	count = wxp->w_veclen;
	tids = wxp->w_vec;
	ptid = wxp->w_ptid;
	nenv = wxp->w_nenv;
	envp = wxp->w_env; 
	

	mfunc = pvm_hostmsgfunc();

    eplist = CINDEX(name, '/') ? nullep : epaths;

	/* -- look for the executable name in the executable path (ep) -- */
    for (ep = eplist; *ep; ep++) 
	{
		strcpy(path, *ep);	 /* search for file */

		if (path[0])
			(void)strcat(path, "/");

		strncat(path, name, sizeof(path) - strlen(path) - 1);

        if (stat(path, &sb) == -1
                || ((sb.st_mode & S_IFMT) != S_IFREG)
                || !(sb.st_mode & S_IEXEC)) 
		{
            if (pvmdebmask & PDMTASK) 
			{
                sprintf(etext, "mpp_load() stat failed <%s>\n", path);
                pvmlogerror(etext);
            }
			continue;	/* try next path element */
        }

		/* Here we have found an executable -- try to start it */

		if ( !(pids = TALLOC(count, int, "pids")) 
				||  !(nodes = TALLOC(count, int, "nodes"))) 
		{
			err = PvmNoMem;
			goto done;
		}

		if (!(sp = mpp_new(count, ptid)))  
		{
			err = PvmOutOfRes;
			goto done;
		}

		for (j = 0; j < count; j++)
			nodes[j] = sp->n_first + j;

		/* copy the pvmd's environment, augment with what is passed to us */
		cenv = pvmcopyenv(environ);
		while (nenv > 0)
			pvmenvinsert(&cenv, envp[--nenv]);


		/* Do the load, return how many were actually started */

		err = nx_loadve((long *) nodes, (long) count, (long) sp->n_ptype, (long *)pids, path, argv, cenv);

		pvmfreeenv(cenv); /* free the copied environment */

		if (err < count)  /* loadve yacked ?? */
		{
			sprintf(etext, "mpp_load() loaded only %d <%s>\n", err, path);
			pvmlogerror(etext);
			err = PvmDSysErr;
			goto done;
		}

		if (first)		/* first time through, do some initialization */ 
		{
			if ( _setptype(PVMDPTYPE) < 0)
        		pvmlogperror("mpp_load() setptype to PVMDPTYPE");
			first = 0;

			/* Set up the prealloced buffers      */
			mpdirect = new_vdirectstruct( partsize, NRBUFS, NSBUFS );	

			mprecvbufs = init_recv_list(NRBUFS, PMTDBASE, 
					MAXFRAGSIZE, 0, MPPANY, mfunc);

		}


		/* configure the tasks
		/* alright, send some important information to the tasks that we
			just started */

		ptypepart = ((sp->n_ptype & ptypemask) << (ffs(tidtmask) - 1)) 
					| TIDONNODE;
		pvminfo[0] = TDPROTOCOL;
		pvminfo[1] = myhostpart + ptypepart;
		pvminfo[2] = ptid;
		pvminfo[3] = MAXFRAGSIZE;
		pvminfo[4] = myndf;
		sprintf(etext, "partsize is %d\n",partsize); 
		pvmlogerror(etext);
		pvminfo[5] = partsize;
		pvminfo[6] = wxp->w_outtid;
		pvminfo[7] = wxp->w_outtag;
		pvminfo[8] = wxp->w_outctx;
		pvminfo[9] = wxp->w_trctid;
		pvminfo[10] = wxp->w_trctag;
		pvminfo[11] = wxp->w_trcctx;

		if (pvmdebmask & PDMTASK) 
		{
			sprintf(etext, "mpp_load() %d type=%ld ptid=%x pid%ld... t%x...\n",
					count, sp->n_ptype, ptid, pids[0], 
					myhostpart + ptypepart + sp->n_first);

			pvmlogerror(etext);
		}

		pvmlogerror("Starting configuration Message send Loop\n");
		for (i = 0; i < count; i ++ )
		{
			cnode =  nodes[i];
			if ( (*mfunc->imsgsend)(appid, PMTCONF,  (char *) pvminfo, 
					sizeof(pvminfo), cnode, PVMDPTYPE, &mid) < 0) 
			{
				pvmlogperror("mpp_load() configuration message");
				err = PvmDSysErr;
				goto done;
			}
			while (! ((*mfunc->msgdone)(appid, &mid, minfo)));
		}
		pvmlogerror("Finished Message send Loop\n");


		/* create new task structs */

		for (j = 0; j < count; j++) 
		{
			if (pids[j] > 0) 
			{
				if ((tp = task_new(myhostpart + ptypepart
						+ sp->n_first + j)) == NULL) {
					err = PvmOutOfRes;
					goto done;
				}
				task_setpid(tp, pids[j]);
				tp->t_a_out = STRALLOC(name);
				tp->t_ptid = ptid;
				tp->t_flag |= TF_CONN;		/* no need for the authorization */
				tp->t_out = taskstdout;
				tp->t_outtid = wxp->w_outtid;
				pvmlogprintf("setting output context of %x to %x\n",
								tp->t_tid, tp->t_outtid);
				tp->t_outctx = wxp->w_outctx;
				tp->t_outtag = wxp->w_outtag;
				tp->t_trctid = wxp->w_trctid;
				tp->t_trcctx = wxp->w_trcctx;
				tp->t_trctag = wxp->w_trctag;
				tids[j] = tp->t_tid;

			/* initialize the sequence numbers for this task */
				fill_directstruct (mpdirect + nodes[j], NSBUFS, tp->t_tid, 
						pids[j], PMTDBASE, 0, appid);   
				init_chunkostruct( (mpdirect + nodes[j])->ordering, NRBUFS);

			} else
				tids[j] = PvmDSysErr;
		}

		if (pids)
			PVM_FREE(pids);

		if (nodes)
			PVM_FREE(nodes);

		return 0;
	}


	/* here, only if there is an error in the way things were spawned */
	if (pvmdebmask & PDMTASK) 
	{
		sprintf(etext, "mpp_load() didn't find <%s>\n", name);
		pvmlogerror(etext);
	}

	err = PvmNoFile;

done:
	for (j = 0; j < count; j++)
		tids[j] = err;

	if (pids)
		PVM_FREE(pids);

	return err;
}


/* input from node tasks */
int
mpp_input()
{
	static int cbuf = 0;
	struct pkt *pp = 0;

	int buf; 				/* buffer we are working on */
	int mxbufs = NRBUFS;	/* maximum number of buffers we will proc on read */
	int npkts = 0;
	int tsrc, src;
	int tag;
	int pid = 0;
	int mid;

	MPP_DIRECTI_PTR tcon;

	static CHUNK_PTR readyList = (CHUNK_PTR) NULL;

	struct pkt *hdr; 

	if ( mprecvbufs == (MSG_INFO_PTR) NULL )  
	{
		/* we haven't alloc'ed any buffers. No tasks loaded */
		return npkts;
	}

	for (buf = 0; buf < mxbufs; buf++) /* limit #of packets to read */
	{
		if (pp = (struct pkt *) pvm_chunkReady(mprecvbufs, mxbufs,
				pvm_hostmsgfunc(), mpdirect, partsize, &cbuf,
				&readyList) )
		{
#if defined(IMA_PGONPUMA)
			src = extract_lnid(src);
#endif
			npkts ++;

			/* Step 2) extract header information from the packet */

			pp->pk_dst = pvmget32(pp->pk_dat);
			pp->pk_src = pvmget32(pp->pk_dat + 4);
			pp->pk_flag = pvmget8(pp->pk_dat + 12);
			pp->pk_len -= TDFRAGHDR;
			pp->pk_dat += TDFRAGHDR;
			if (pp->pk_flag & FFSOM) 
			{
				if (pp->pk_len < MSGHDRLEN) 
				{
					sprintf(etext,
							"mpp_input() SOM pkt src t%x dst t%x short\n",
							pp->pk_src, pp->pk_dst);
					pvmlogerror(etext);
					pk_free(pp);
					return;
				}
				pp->pk_enc = pvmget32(pp->pk_dat);
				pp->pk_tag = pvmget32(pp->pk_dat + 4);
				pp->pk_ctx = pvmget32(pp->pk_dat + 8);
				pp->pk_wid = pvmget32(pp->pk_dat + 16);
				pp->pk_crc = pvmget32(pp->pk_dat + 20);
				pp->pk_len -= MSGHDRLEN;
				pp->pk_dat += MSGHDRLEN;
			}
		
			/* Step 3) Deliver packet to the PVM processing input stream */

			loclinpkt((struct task *)0, pp);

		}
		else
			break;	/* no packets to read */

	}

	return npkts;
}

/* output to node tasks */
/* this is supposed to be like pkt_to_task from the pvmd. 
	if pp is NULL then mpp_output should try to write any
	packets that are on the output queue
*/ 
int
mpp_output(tp, pp)
	struct task *tp;
	struct pkt *pp;
{
	char *cp;
	int len;
	long node;				/* node number */
	long ptype;				/* process type */
	struct nodeset *sp;

	int cc;
	int i;
	int idx1;
	int tag;

	struct pkt * delpkt;
	struct pkt * npkt;
	struct pkt * pp2;
	struct pkt * tstpkt;
	struct task *tp2;

	MPP_DIRECTI_PTR tdirect;

	MSGFUNC_PTR	mfunc;

	mfunc = pvm_hostmsgfunc();

	/* check if we have any packets to queue up and send */
	if (tp && pp )
	{
		node = tp->t_tid & tidnmask;

		ptype = (tp->t_tid & tidtmask) >> (ffs(tidtmask) - 1);

#if defined(IMA_PGON)
		/* XXX ptype is not used for PUMA host-node communication */
		for (sp = busynodes->n_link; sp != busynodes; sp = sp->n_link)
		{
			if ((sp->n_ptype & ptypemask) == ptype) 
			{
				ptype = sp->n_ptype;
				break;
			}
		}
		if (sp == busynodes) 
		{
			sprintf(etext, "mpp_output() pkt to t%x scrapped (no ptype %ld)\n",
				tp->t_tid, ptype);
			pvmlogerror(etext);
			pk_free(pp);
			goto done;
		}

#endif
		cp = pp->pk_dat;

		len = pp->pk_len;

		if (pp->pk_flag & FFSOM) 
		{
			cp -= MSGHDRLEN;

			len += MSGHDRLEN;

			if (cp < pp->pk_buf) {
				pvmlogerror("mpp_output() no headroom for message header\n");
				pk_free(pp);
				goto done;
			}
			/* If we are not an MCA'ed pkt OR FFMCAWH (MCA write hdr)
											is set, then write the header */
			if ( !(pp->pk_flag & FFMCA)  ||  pp->pk_flag & FFMCAWH)
			{
				pvmput32(cp, pp->pk_enc);
				pvmput32(cp + 4, pp->pk_tag);
				pvmput32(cp + 8, pp->pk_ctx);
				pvmput32(cp + 16, pp->pk_wid);
				pvmput32(cp + 20, pp->pk_crc);
				if (pvmdebmask & PDMMCA)
				{
					sprintf(etext, "mpp_output wrt TDMSGHDR for %x\n", pp->pk_buf);
					pvmlogerror(etext);
				} 
			}

		}
		if (pvmdebmask & PDMPACKET) {
			sprintf(etext,
				"mpp_output() src t%x dst t%x ff %x len %d ptype %d\n",
				pp->pk_src, pp->pk_dst, pp->pk_flag & (FFSOM|FFEOM),
				len, ptype);
			pvmlogerror(etext);
		}
		cp -= TDFRAGHDR;
		if (cp < pp->pk_buf) {
			pvmlogerror("mpp_output() no headroom for packet header\n");
			pk_free(pp);
			goto done;
		}
		/* If we are not an MCA'ed pkt OR FFMCAWH (MCA write hdr)
									is set, then write the header */
		if ( !(pp->pk_flag & FFMCA)  ||  pp->pk_flag & FFMCAWH)
		{
			pvmput32(cp, pp->pk_dst);
			pvmput32(cp + 4, pp->pk_src);
			pvmput32(cp + 8, len);
			pvmput32(cp + 12, 0);
			pvmput8(cp + 12, pp->pk_flag & (FFSOM|FFEOM|FFMCA));
			if (pvmdebmask & PDMMCA)
			{
				sprintf(etext, "mpp_output wrt TDFRAGHDR for %x\n", pp->pk_buf);
				pvmlogerror(etext);
			} 
		}
		len += TDFRAGHDR;

		pp->pk_dat = cp; 	/* store where the data starts */ 
		pp->pk_len = len;	/* store the length of the buffer */

		/* okay we've done everything to the packet, now put it on the output
			queue */

		LISTPUTBEFORE(mppopq, pp, pk_link, pk_rlink);
	}

	tstpkt = mppopq -> pk_link;
	while (tstpkt != mppopq)
	{
		/* find free index */
		if ( (idx1 = pvm_mpp_find_midx(mppsendmids, (CHUNK_PTR *) mppoutpkts, 
					&lastidx, NMPPSBUFMIDS, mfunc) ) >= 0 ) 
		{
			node = tstpkt->pk_dst & tidnmask;

			tdirect = mpdirect + node;

			ptype = (tstpkt->pk_dst & tidtmask) >> (ffs(tidtmask) - 1);

			tag = PMTDBASE + tdirect->sseq;

			if (++(tdirect->sseq) >= tdirect->nbufs)
				tdirect->sseq = 0;

			/* send the actual packet of information */
			/* Hmm, should check here for long packets  ... ?? */
			/* XXX appid needs to be checked here for PUMA */ 
#if defined(IMA_PGONPUMA)
			ptype = extract_pid(appid, node);
			node = extract_nid(appid, node);
#endif
			
			len = tstpkt->pk_len;

			if (len < 16)
			{
				sprintf(etext,"mpp_output() bogus len %d? (dst %x, ff %d)\n",
					len, tstpkt->pk_dst, tstpkt->pk_flag);
		
				pvmlogperror(etext);
			}
			(*mfunc->imsgsend)(appid, tag, tstpkt->pk_dat, len, 
					node, ptype, &cc); 
			pvm_assign_mid(mppsendmids, cc, idx1);
			if (cc < 0 ) 
			{
				sprintf(etext,"mpp_output() can't send to t%x", tstpkt->pk_dst);
				pvmlogperror(etext);

				/* clear the output queue of packets for this tid in the output
				   queue */	
				npkt = tstpkt->pk_link;
				while (npkt != tstpkt)
				{
					if ( npkt->pk_dst == tstpkt->pk_dst)
					{
						delpkt = npkt;
						npkt = npkt->pk_link;
						LISTDELETE(delpkt,pk_link, pk_rlink);
						pk_free(delpkt);
					}
					else
						npkt = npkt->pk_link;
				} /* mppopq queue is cleared */

				delpkt = tstpkt;

				tstpkt = tstpkt->pk_link;

				LISTDELETE(delpkt, pk_link, pk_rlink);

				tp2 = task_find(delpkt->pk_dst);	

				pk_free(delpkt);

				mpp_free(tp);

				tp2 = tp2->t_rlink;

				task_cleanup(tp2->t_link);

				task_free(tp2->t_link);

			} 
			else  /* isend was ok, remember the pkt, go onto the next one */
			{
				pvm_assign_chunk((CHUNK_PTR *) mppoutpkts, (CHUNK_PTR) tstpkt,
								 idx1);

				if (pvmdebmask & (PDMPACKET | PDMNODE)) 
				{
					sprintf(etext,
						"mpp_output() from mppopq src t%x dst t%x len %d ptype %d flags %s\n",
						tstpkt->pk_src, tstpkt->pk_dst, len, ptype,
						pkt_flags(tstpkt->pk_flag));
					pvmlogerror(etext);
				}

				/* remove the packet from the mppopq */ 
				delpkt = tstpkt;
				tstpkt = tstpkt->pk_link;

				LISTDELETE(delpkt,pk_link,pk_rlink);
			}
		}
		else
		{
			if (pvmdebmask & PDMPACKET) {
				sprintf(etext,
					"mpp_output() no free send message ids.\n");
				pvmlogerror(etext);
			}
			pvmlogerror("(*)\n");
			return 0;
		}
	}
done:
	if (tp && tp->t_flag & TF_CLOSE) {
	    mpp_free(tp);
		tp = tp->t_rlink;
/* XXX tm_exit() also calls task_cleanup(); should only be done once */
		task_cleanup(tp->t_link);
		task_free(tp->t_link);
	}
	return 0;
}


/* probe for messages from node tasks */
int
mpp_probe()
{
	if (busynodes->n_link != busynodes)
	{
		return 1; /* don't probe messages -- can do this in one-shot at recv*/
	}
	else	/* task queue empty */
		return 0;
}

#if defined(IMA_PGONPUMA)
int
extract_lnid(pnid)
int pnid;
{
	if (pnid >=0 && pnid < (sizeof(phystolmap)/sizeof(short)))
		return phystolmap[pnid];
	else
		return -1;
}

void
mpp_kill(tp, signum)
	struct task *tp;
	int signum;
{
	struct nodeset *sp;
	int cid;

	if (TIDISNODE(tp->t_tid)) 
	{
		if (signum == SIGKILL || signum == SIGTERM) 
		{
       	    		mpp_free(tp);
/* XXX tm_exit() also calls task_cleanup(); should only be done once */
			tp = tp->t_rlink;
			task_cleanup(tp->t_link);
			task_free(tp->t_link);
		} else {
			sprintf(etext,"mpp_kill() signal %d to node t%x ignored\n",
				signum, tp->t_tid);
			pvmlogerror(etext);
		}
	} 
	else 	/* Not a node */
		(void)kill(tp->t_pid, signum);
}

#endif

/* ----------- find_direct ---------- */
/* this is a hack to find the correct ordering structure for
	a node. 
*/
MPP_DIRECTI_PTR 
pvm_find_direct (dlist, nstruct, node)
MPP_DIRECTI_PTR dlist;
int nstruct;
int node;
{
	node = node & TIDNODE;  /* make sure this is a node */

	if ( dlist && dlist == mpdirect )
	{
			return dlist + node;
	}	
}

struct msgid *
pvm_mpp_get_precvids()
{
	return (struct msgid *) NULL;
}
		 

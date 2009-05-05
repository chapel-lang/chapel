
static char rcsid[] =
	"$Id: pvmdmimd.c,v 1.6 2009/01/30 21:23:15 pvmsrc Exp $";

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
$Log: pvmdmimd.c,v $
Revision 1.6  2009/01/30 21:23:15  pvmsrc
Fixed up insecure usage of fopen() for creating/writing/appending files.
	- replace with open(...O_EXCL...) and fdopen()...
(Spanker=kohl)

Revision 1.5  2005/08/22 15:13:36  pvmsrc
Added #include <pvmtev.h> for #include global.h...  :-Q
	- submitted by Skipper Hartley <charles.l.hartley@bankofamerica.com>
(Spanker=kohl)

Revision 1.4  2002/02/21 23:19:01  pvmsrc
Added new (not to be documented!) PVM_MAX_TASKS env var support.
	- for Mahir Lokvancic <mahir@math.ufl.edu>.
	- forcefully limits the number of tasks that can attach to a
		pvmd, required on Solaris in rare circumstances when hard
		FD_SETSIZE limit is reached, and all hell breaks loose...
	- check return for task_new() call, can now produce NULL ptr,
		indicating PvmOutOfRes...
(Spanker=kohl)

Revision 1.3  2001/09/25 21:19:00  pvmsrc
Minor TMPNAMFUN()/tmpnam() cleanup.
	- moved macro def to pvm3.h, renamed PVMTNPMAN().
	- same for LEN_OF_TMP_NAM -> PVMTMPNAMLEN.
	- mostly a huge waste of time, since *both* tmpnam() & mktemp()
		produce the same "dangerous" warning message in Linux/gcc...
	- damn.
(Spanker=kohl)

Revision 1.2  2000/02/17 21:10:16  pvmsrc
Cleaned up comments...  mpp_load() args...
(Spanker=kohl)

 * Revision 1.1  1998/02/23  21:31:44  pvmsrc
 * Copied from SP2MPI.
 *
 * Revision 1.10  1997/10/01  15:36:06  pvmsrc
 * Removed unnecessary #include "fromlib.h" header.
 * 	- all consts now included in pvm3.h...
 * 	- header file eliminated.
 * (Spanker=kohl)
 *
 * Revision 1.9  1997/08/27  20:18:58  pvmsrc
 * Added blank args (0,0,0) in mpp_load() to make it call forkexec correctly.
 * Protocol needs to be changed to allow taskers to access IBM poe directly.
 * (Spanker=fagg)
 *
 * Revision 1.8  1997/07/09  13:54:58  pvmsrc
 * Fixed Author Header.
 *
 * Revision 1.7  1997/06/02  13:48:38  pvmsrc
 * Moved #include host.h above #include waitc.h.
 * Removed old mesg.h include...  gone baby.
 *
 * Revision 1.6  1997/05/06  20:14:36  pvmsrc
 * 	Catch stdout/stderr correctly and redirect to outtid,tag,ctx
 *
 * Revision 1.5  1997/05/05  20:08:56  pvmsrc
 * 	Pass outctx and trcctx to mpp tasks.
 *
 * Revision 1.4  1997/05/02  13:52:50  pvmsrc
 * 	Start up MPI jobs correctly and get them configured.
 *
 * Revision 1.3  1997/03/25  15:52:21  pvmsrc
 * PVM patches from the base 3.3.10 to 3.3.11 versions where applicable.
 * Originals by Bob Manchek. Altered by Graham Fagg where required.
 * -IP enabled over switch
 * -RMPOOL env can be used instead of host list
 *
 * Revision 1.2  1997/01/28  19:30:57  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:15:09  pvmsrc
 * Initial revision
 *
 * Revision 1.3  1996/05/14  14:35:59  manchek
 * inc'd changes from chulho@kgn.ibm.com
 *
 * Revision 1.2  1995/07/25  17:41:27  manchek
 * mpp_output returns int
 *
 * Revision 1.1  1995/05/30  17:23:56  manchek
 * Initial revision
 *
 * Revision 1.3  1994/06/03  20:54:24  manchek
 * version 3.3.0
 *
 * Revision 1.2  1993/12/20  15:39:47  manchek
 * patch 6 from wcj
 *
 * Revision 1.1  1993/08/30  23:35:09  manchek
 * Initial revision
 *
 */

#include <stdlib.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#ifdef  SYSVSTR
#include <string.h>
#define CINDEX(s,c) strchr(s,c)
#else
#include <strings.h>
#define CINDEX(s,c) index(s,c)
#endif

#include <pvm3.h>
#include <pvmproto.h>
#include <pvmtev.h>
#include "global.h"
#include "host.h"
#include "waitc.h"
#include "pvmalloc.h"
#include "pkt.h"
#include "task.h"
#include "listmac.h"
#include "pvmdmp.h"
#include "pvmmimd.h"
#include "bfunc.h"

#define MPICOMM		"/usr/bin/poe"
#define MPIOPT1		"-procs"
#define MPIOPT2		"-euilib"
#define MPIOPT3		"-hfile"
#define MPIOPT4		"-rmpool"
#define MPIOPARG2	"us"		/* options: -procs # -euilib us */
#define MPIOPARG3	"ip"		/* Enable IP over switch */
#define MPIARGC		7		/* number of command line arguments */

char *getenv();

/* Global */

extern int pvmdebmask;			/* from pvmd.c */
extern char **epaths;			/* from pvmd.c */
extern int myhostpart;			/* from pvmd.c */
extern int tidhmask;			/* from pvmd.c */
extern int ourudpmtu;			/* from pvmd.c */
extern struct htab *hosts;		/* from pvmd.c */
extern struct task *locltasks;	/* from task.c */

int tidtmask = TIDPTYPE;		/* mask for ptype field of tids */
int tidnmask = TIDNODE;			/* mask for node field of tids */

/* private */

static int myndf = 0;
static struct nodeset *busynodes;	/* active nodes; ordered by proc type */
static char pvmtxt[512];		/* scratch for error log */
static int ptypemask;			/* mask; we use these bits of ptype in tids */
static char nodefile[PVMTMPNAMLEN];	/* tmp node file */
static char **nodelist = 0;		/* default poe node list */
static int partsize = 0;		/* number of nodes allocated */
static int hostfileused = TRUE;         /* Check if MP_HOSTFILE used */
static char defaultpool[64]="1";        /* default MP_POOL if not set */
static char mpiadapter[]={MPIOPARG2};   /* default User-Space */

static int sp2pvminfo[SIZEHINFO];



void
mpp_init(argc, argv)
	int *argc;
	char **argv;
{
	struct stat sb;
	char *hfn;		/* host file name */
	char nname[128];	/* node name */
	FILE *hfp;
	int i;

	if ((hfn = getenv("LOADLBATCH"))) {
		if (strcmp(hfn, "yes") == 0) {
			if ((hfn = getenv("LOADL_PROCESSOR_LIST"))) {
				sprintf(pvmtxt, 
				  "LOADL_PROCESSOR_LIST=%s.\n",hfn);
				pvmlogperror(pvmtxt);
				hostfileused=FALSE;
				for (hfn;*hfn!='\0';hfn++) {
					if (*hfn == ' ')
						partsize++;
				}
			} else {
				sprintf(pvmtxt, 
				  "LOADL_PROCESSOR_LIST=%s - is not set\n",hfn);
				pvmlogperror(pvmtxt);
				exit(PvmHostFail);
			}
		} else {
			sprintf(pvmtxt,"LOADLBATCH=%s - not set to yes\n",hfn);
			pvmlogperror(pvmtxt);
			exit(PvmHostFail);
                }
        } else if ((hfn = getenv("MP_PROCS"))) {
		if ((partsize = atoi(hfn)) < 1) {
			sprintf(pvmtxt,"MP_PROCS=%d must be >= to 1\n",partsize);
			pvmlogperror(pvmtxt);
			exit(PvmHostFail);
		}
		hostfileused=FALSE;
        	if ((hfn = getenv("MP_RMPOOL"))) {
			if (atoi(hfn) < 0) {
				sprintf(pvmtxt,
				   "MP_RMPOOL=%d must be >= to 0\n",defaultpool);
				pvmlogperror(pvmtxt);
				exit(PvmHostFail);
			}
			strcpy(defaultpool,hfn);
		}
        } else if ((hfn = getenv("MP_HOSTFILE")))
	{
		if (!(hfp = fopen(hfn, "r"))) {
			sprintf(pvmtxt, "sp2hostfile() fopen %s\n", hfn);
			pvmlogperror(pvmtxt);
			exit(PvmHostFail);
		}
		hostfileused=TRUE;
		while (fscanf(hfp, "%s", nname) != EOF)
			partsize++;

		nodelist = TALLOC(partsize, char*, "nname");
		rewind(hfp);
		for (i = 0; i < partsize; i++) {
			fscanf(hfp, "%s", nname);
			nodelist[i] = STRALLOC(nname);
		}
		fclose(hfp);

	} else {
		pvmlogerror("mpp_init() no POE host file.\n");
		pvmlogerror("mpp_init() MP_PROCS, MP_RMPOOL or MP_HOSTFILE must be set.\n");
		exit(PvmHostFail);
	}
	if ((hfn = getenv("MP_EUILIB"))) {
		if (strcmp(hfn, "ip") == 0) {
			strcpy(mpiadapter,MPIOPARG3);  /* IP over switch */
		}
	}
	sprintf(pvmtxt, "%d nodes allocated.\n", partsize);
	pvmlogerror(pvmtxt);

	busynodes = TALLOC(1, struct nodeset, "nsets");
	BZERO((char*)busynodes, sizeof(struct nodeset));
	busynodes->n_link = busynodes;
	busynodes->n_rlink = busynodes;

	ptypemask = tidtmask >> (ffs(tidtmask) - 1);
}


/* create tmp poe host file from default */
char *
sp2hostfile(first, count)
	int first;			/* first node in the set */
	int count;			/* number of nodes requested */
{
	int i;
	FILE *tmpfp;
	int fd;

	if (partsize < count) { 
		sprintf(pvmtxt, "sp2hostfile() need at least %d nodes\n", count+1);
		pvmlogperror(pvmtxt);
		return (char *)0;
	}
	(void)PVMTMPNAMFUN(nodefile);
	if ((fd = open( nodefile, O_RDWR|O_CREAT|O_EXCL|O_TRUNC, 0600 )) < 0) {
		sprintf(pvmtxt, "sp2hostfile() open %s", nodefile);
		pvmlogperror(pvmtxt);
		return (char *)0;
	}
	if (!(tmpfp = fdopen( fd, "w" ))) {
		sprintf(pvmtxt, "sp2hostfile() fdopen %s", nodefile);
		pvmlogperror(pvmtxt);
		return (char *)0;
	}
	if (pvmdebmask & PDMNODE) {
		sprintf(pvmtxt, "sp2hostfile() POE host file: %s\n", nodefile);
		pvmlogerror(pvmtxt);
	}
	for (i = first; i < count + first; i++) 
		fprintf(tmpfp, "%s\n", nodelist[i]);
	fclose(tmpfp);
	return nodefile;
}


/*
 * find a set of free nodes from nodelist; assign ptype sequentially,
 * only tasks spawned together get the same ptype
 */
struct nodeset *
mpp_new(count, ptid)
	int count;		/* number of nodes requested */
	int ptid;		/* parent's tid */
{
	struct nodeset *sp, *newp, *sp2;
	int last = -1;
	int ptype = 0;

	if (!(newp = TALLOC(1, struct nodeset, "nsets"))) {
		pvmlogerror("mpp_new() can't get memory\n");
		pvmbailout(0);
	}
	BZERO((char*)newp, sizeof(struct nodeset));

	newp->n_size = count;
	for (sp = busynodes->n_link; sp != busynodes; sp = sp->n_link) {
		if (sp->n_first - last > count)
			break;
		last = sp->n_first + sp->n_size - 1;
/*
		if (sp->n_link == busynodes && partsize - last > count)
			break;
*/
		if (ptype <= sp->n_ptype)
			ptype = sp->n_ptype + 1;
	}
	if (sp == busynodes && partsize - last <= count) {
		pvmlogerror("mpp_new() not enough nodes in partition\n");
		PVM_FREE(newp);
		return (struct nodeset *)0;
	}
	for (sp2 = busynodes->n_link; sp2 != busynodes; sp2 = sp2->n_link)
		if ((sp2->n_ptype & ptypemask) == (ptype & ptypemask))
			break;
	if (sp2 != busynodes || ptype == NPARTITIONS) {
		for (ptype = 0; ptype < NPARTITIONS; ptype++) {
			for (sp2 = busynodes->n_link; sp2 != busynodes; sp2 = sp2->n_link)
				if ((sp2->n_ptype & ptypemask) == (ptype & ptypemask))
					break;
			if (sp2 == busynodes)
				break;
		}
		if (ptype == NPARTITIONS) {
			pvmlogerror("mpp_new() out of ptypes: too many spawns\n");
			return (struct nodeset *)0;
		}
	}

done:
	if (pvmdebmask & PDMNODE) {
		sprintf(pvmtxt, "mpp_new() %d nodes %d ... ptype=%d ptid=%x\n",
			count, last+1, ptype, ptid);
		pvmlogerror(pvmtxt);
	}
	newp->n_first = last + 1;
	if (hostfileused) {
		if (!sp2hostfile(newp->n_first, count)) {
			PVM_FREE(newp);
			return (struct nodeset *)0;
		}
	}
	newp->n_ptype = ptype;
	newp->n_ptid = ptid;
	newp->n_alive = count - 1;
	LISTPUTBEFORE(sp, newp, n_link, n_rlink);

	return newp;
}


/*
 * remove node/ptype from active list; if tid is the last to go, shutdown
 * pvmhost's socket, but do not destroy the node set because pvmhost may 
 * not exit immediately. To avoid a race condition, let mpp_output()
 * do the cleanup.
 */
void
mpp_free(tp)
	struct task *tp;
{
	struct nodeset *sp;
	int ptype;
	struct task *tp2;
	int tid = tp->t_tid;

	if (!TIDISNODE(tid))
		return;

	ptype = TIDTOTYPE(tid);
	tp->t_txq = 0;				/* don't free pvmhost's txq */
	for (sp = busynodes->n_link; sp != busynodes; sp = sp->n_link) {
		if ((sp->n_ptype & ptypemask) == ptype) {

			if (pvmdebmask & PDMNODE) {
				sprintf(pvmtxt, "mpp_free() t%x type=%ld alive=%d\n",
					tid, sp->n_ptype, sp->n_alive);
				pvmlogerror(pvmtxt);
			}
			if (--sp->n_alive == 0) {
				if (tp2 = task_find(sp->n_ptid)) {
					tp2->t_flag |= TF_CLOSE;
					if (tp2->t_sock != -1) {
/*
						wrk_fds_delete(tp2->t_sock, 3);
						(void)close(tp2->t_sock);
						tp2->t_sock = -1;
*/
						shutdown(tp2->t_sock, 1);
						/* close stdout after pvmhost dies */
						tp2->t_out = tp->t_out;
					}
				}
/*
				LISTDELETE(sp, n_link, n_rlink);
				PVM_FREE(sp);
*/
			}
			tp->t_out = -1;		/* don't free shared stdout if alive > 0 */
			return;
		}
	}
	sprintf(pvmtxt, "mpp_free() t%x not active\n", tid);
	pvmlogerror(pvmtxt);
	return;
}


/* load executable onto the given set of nodes */
int
mpp_load( wxp )
struct waitc_spawn *wxp;
{
	int flags = 0;              /* exec options */
	char *name;             /* executable */
	char **argv;            /* arg list (argv[-1] must be there) */
	int count;				/* how many */
	int *tids;				/* array to store new tids */
	int ptid;				/* parent task ID */
	int nenv;				/* length of environment */
	char **envp;			/* environment strings */
	int ptypepart;			/* type field */
	int j;
	struct task *tp;
	struct pkt *hosttxq;	/* out-going queue of pvmhost */
	int err = 0;
	struct nodeset *sp;
	char c[128];			/* buffer to store count, name.host */
	char **ar = 0;
	int nargs;
	char **ep, **eplist;
	char path[MAXPATHLEN];
	struct stat sb;
	char **av;
	int hostout;			/* stdout of pvmhost */
	struct hostd *hp = hosts->ht_hosts[hosts->ht_local];
	int hostpid;			/* Unix pid of pvmhost */
	char htid[128];			/* buffer to store pvmhost tid */
	char *p;

	static char *nullep[] = { "", 0 };

        /* -- initialize some variables from the waitc_spawn struct -- */

        name = wxp->w_argv[0];
        argv = wxp->w_argv;
        count = wxp->w_veclen;
        tids = wxp->w_vec;
        ptid = wxp->w_ptid;
        nenv = wxp->w_nenv;
        envp = wxp->w_env;



	eplist = CINDEX(name, '/') ? nullep : epaths;

	for (ep = eplist; *ep; ep++) {
		/* search for file */
		(void)strcpy(path, *ep);
		if (path[0])
			(void)strcat(path, "/");
		(void)strncat(path, name, sizeof(path) - strlen(path) - 1);

		if (stat(path, &sb) == -1
		|| ((sb.st_mode & S_IFMT) != S_IFREG)
		|| !(sb.st_mode & S_IEXEC)) {
			if (pvmdebmask & PDMTASK) {
				sprintf(pvmtxt, "mpp_load() stat failed <%s>\n", path);
				pvmlogerror(pvmtxt);
			}
			continue;
		}

		if (!(sp = mpp_new(count+1, ptid))) {
			err = PvmOutOfRes;
			goto done;
		}
		ptypepart = (sp->n_ptype << (ffs(tidtmask) - 1)) | TIDONNODE;

		if (argv)
			for (nargs = 0; argv[nargs]; nargs++);
		else
			nargs = 0;
		/* ar[-1], poe, -procs, #, -euilib, us, -hfile fname */
		nargs += MPIARGC + 1;
		av = TALLOC(nargs + 1, char*, "argv");
		av++;				/* reserve room for debugger */
		BZERO((char*)av, nargs * sizeof(char*));
		av[0] = MPICOMM;
		av[1] = path;
		av[--nargs] = 0;
		if (hostfileused) {
			av[--nargs] = nodefile;
			av[--nargs] = MPIOPT3;
		} else {
			av[--nargs] = defaultpool;
			av[--nargs] = MPIOPT4;
		}
		av[--nargs] = mpiadapter;
		av[--nargs] = MPIOPT2;
		sprintf(c, "%d", count+1);
		av[--nargs] = c;
		av[--nargs] = MPIOPT1;
		for (j = 2; j < nargs; j++)
			av[j] = argv[j - 1];	/* poe name argv -procs # -euilib us */
/*
		if ((sock = mksock()) == -1) {
			err = PvmSysErr;
			goto done;
		}
*/
		if (flags & PvmTaskDebug)
			av++;					/* pdbx name -procs # -euilib us */
		/* if (err = forkexec(flags, av[0], av, 0, (char **)0, &tp))
*/
		if (err = forkexec(flags, av[0], av, 0, (char **)0, 0,
0, 0, &tp))
			goto done;
		tp->t_ptid = ptid;
		PVM_FREE(tp->t_a_out);
		sprintf(c, "%s.host", name);
		tp->t_a_out = STRALLOC(c);
		sp->n_ptid = tp->t_tid;		/* pvmhost's tid */
		hosttxq = tp->t_txq;
		hostout = tp->t_out;
		hostpid = tp->t_pid;
		tp->t_out = -1;
		sprintf(htid, "PVMHTID=%d", tp->t_tid);
		p = STRALLOC(htid);
		pvmputenv(p);
		sp2pvminfo[0] = TDPROTOCOL;
		sp2pvminfo[1] = myhostpart + ptypepart;
		sp2pvminfo[2] = ptid;
		sp2pvminfo[3] = MAXFRAGSIZE;
		sp2pvminfo[4] = myndf;
		sp2pvminfo[5] = partsize;
		sp2pvminfo[6] = wxp->w_outtid;
		sp2pvminfo[7] = wxp->w_outtag;
		sp2pvminfo[8] = wxp->w_outctx;
		sp2pvminfo[9] = wxp->w_trctid;
		sp2pvminfo[10] = wxp->w_trctag;
		sp2pvminfo[11] = wxp->w_trcctx;


/*
		if (sockconn(sock, tp, pvminfo) == -1) {
			err = PvmSysErr;
			task_free(tp);
			goto done;
		}
*/
		/* XXX task may not be on same host; can't do auth with tmp file */
		tp->t_flag |= TF_CONN;
		if (pvmdebmask & PDMTASK) {
			sprintf(pvmtxt, "mpp_load() %d type=%d ptid=%x t%x...\n",
				count, sp->n_ptype, ptid, myhostpart + ptypepart);
			pvmlogerror(pvmtxt);
		}

		/* create new task structs */

		for (j = 0; j < count; j++) {
			if ((tp = task_new(myhostpart + ptypepart + j)) == NULL) {
				err = PvmOutOfRes;
				goto done;
			}
			tp->t_a_out = STRALLOC(name);
			tp->t_ptid = ptid;
			tp->t_flag |= TF_CONN;		/* no need for the auth crap */
			tids[j] = tp->t_tid;
			PVM_FREE(tp->t_txq);
			tp->t_txq = hosttxq;		/* node tasks share pvmhost's txq */
			tp->t_out = hostout;        /* and stdout */
			tp->t_pid = hostpid;		/* pvm_kill should kill pvmhost */
			tp->t_outtid = wxp->w_outtid;  /* catch stdout/stderr  */
			tp->t_outtag = wxp->w_outtag; 
			tp->t_outctx = wxp->w_outctx; 
		}
		return 0;
	}
    if (pvmdebmask & PDMTASK) {
        sprintf(pvmtxt, "mpp_load() didn't find <%s>\n", name);
        pvmlogerror(pvmtxt);
    }
    err = PvmNoFile;

done:
	for (j = 0; j < count; j++)
		tids[j] = err;
	return err;
}


#if 0

/* kill poe process */
void
sp2kill(a_out, node)
	char *a_out;
	int node;
{
	FILE *fp;
	int i;
	char nname[128];	/* node name */
	char comm[512];		/* command to issue */
	char *hfn;			/* host file name */
	struct stat sb;
	char *av[8];		/* for rsh args */
	int ac;
	int pid = -1;		/* pid of rsh */

	node++;
	if ((hfn = getenv("MP_HOSTFILE")) || stat(hfn = "host.list", &sb) != -1) {
		if (fp = fopen(hfn, "r")) {
			for (i = 0; i < node; i++)
				fscanf(fp, "%s", nname);
			fclose(hfn);
			if ((pid = fork()) == -1) {
				pvmlogperror("sp2kill() fork");
				return;
			}
			if (!pid) {
				ac = 0;
				av[ac++] = "/usr/bin/rsh";
				av[ac++] = nname;
				av[ac++] = "poekill";
				av[ac++] = a_out;
				av[ac++] = 0;
				for (i = getdtablesize(); --i > 2; )
					(void)close(i);
				execvp(av[0], av);
				_exit(1);
			}
		} else
			pvmlogperror("sp2kill() fopen");
		return;
	}
	pvmlogerror("sp2kill() no host file");
}
			
#endif /*0*/


void
mpp_kill(tp, signum)
	struct task *tp;
	int signum;
{

	if (TIDISNODE(tp->t_tid)) {
		if (signum == SIGTERM || signum == SIGKILL) {
			/* sp2kill(tp->t_a_out, tp->t_tid & tidnmask); */
			(void)kill(tp->t_pid, signum);
			mpp_free(tp);
			task_cleanup(tp);
			task_free(tp);
        } else {
			sprintf(pvmtxt,"mpp_kill() signal %d to node t%x ignored\n",
				signum, tp->t_tid);
			pvmlogerror(pvmtxt);
		}
	} else
		(void)kill(tp->t_pid, signum);
}


/*
 * Add pvmhost's socket to wfds if there are packets waiting to
 * be sent to a related node task. Node tasks have no sockets;
 * they share pvmhost's packet queue (txq). Pvmhost simply
 * forwards any packets it receives to the appropriate node.
 */

int
mpp_output(dummy1, dummy2)
	struct task *dummy1;
	struct pkt *dummy2;
{
	struct nodeset *sp, *sp2;
	struct task *tp;
	int ptype;

	for (sp = busynodes->n_link; sp != busynodes; sp = sp->n_link)
		if ((tp = task_find(sp->n_ptid))) {
			if (tp->t_txq->pk_link->pk_buf && tp->t_sock != -1)
				wrk_fds_add(tp->t_sock, 2);
		} else {
			if (sp->n_alive) {
				sprintf(pvmtxt, "mpp_output() pvmhost %d died!\n", sp->n_ptype);
				pvmlogerror(pvmtxt);
				/* clean up tasks it serves */
				ptype = sp->n_ptype & ptypemask;
				for (tp = locltasks->t_link; tp != locltasks; tp = tp->t_link)
					if (TIDISNODE(tp->t_tid) && TIDTOTYPE(tp->t_tid) == ptype) {
						tp->t_txq = 0;
						tp = tp->t_rlink;
						task_cleanup(tp->t_link);
						task_free(tp->t_link);
					}
			}
			/* pvmhost has died, destroy the node set */
			sp2 = sp;
			sp = sp->n_rlink;
			LISTDELETE(sp2, n_link, n_rlink);
			PVM_FREE(sp2);
		}
	return 0;
}


/* replace tm_connect and tm_conn2 */
int
mpp_conn(tp, tp2)
	struct task *tp;
	struct task *tp2;
{
	struct nodeset *sp;
	int pvminfo[SIZEHINFO];			/* host info */
	int ptypepart;					/* type field */

	if (pvmdebmask & PDMNODE) {
		sprintf(pvmtxt, "mpp_conn() pvmhost %x", tp2->t_tid);
		pvmlogerror(pvmtxt);
	}
	tp2->t_sock = tp->t_sock;
	tp2->t_sad = tp->t_sad;
	tp2->t_salen = tp->t_salen;
	tp2->t_flag |= TF_CONN;
	tp->t_sock = -1;
	for (sp = busynodes->n_link; sp != busynodes; sp = sp->n_link)
		if (sp->n_ptid == tp2->t_tid)
			break;
	if (sp == busynodes) {
		pvmlogerror("mpp_conn() task is not pvmhost\n");
		return -1;
	}
	ptypepart = (sp->n_ptype << (ffs(tidtmask) - 1)) | TIDONNODE;
	if (write(tp2->t_sock, sp2pvminfo, sizeof(sp2pvminfo)) 
	!= sizeof(sp2pvminfo)) {
		pvmlogperror("mpp_conn() write");
		return -1;
	}
	return 0;
}


#if 0

/* 
 * Create socket to talk to pvmhost.
 * Return socket descriptor if successful, -1 otherwise.
 */
int 
mksock()
{	
	struct hostd *hp = hosts->ht_hosts[hosts->ht_local];
	struct sockaddr_in sin;
	char buf[128];
	char *p;
	int cc;
	int sock;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		pvmlogperror("mksock() socket");
		return -1;
	}
	sin = hp->hd_sad;
	if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1) {
		pvmlogperror("mksock() bind");
		(void)close(sock);
		return -1;
	}
	cc = sizeof(sin);
	if (getsockname(sock, (struct sockaddr*)&sin, &cc) == -1) {
		pvmlogperror("mksock() getsockname");
		(void)close(sock);
		return -1;
	}
	if (listen(sock, 1) == -1) {
		pvmlogperror("mksock() listen");
		(void)close(sock);
		return -1;
	}

	p = inadport_hex(&sin);
	sprintf(buf, "PVMSOCK=%s", p);
	p = STRALLOC(buf);
	pvmputenv(p);

	return sock;
}


/* 
 * Wait for connect request from pvmhost and establish connection.
 * Return 0 if successful, -1 otherwise.
 * Close listening socket.
 */
int
sockconn(sock, tp, hinfo)
	int sock;			/* listening post */
	struct task *tp;	/* pvm host */
	int hinfo[];		/* host info to pass along */
{
	int i;

	if ((tp->t_sock = accept(sock, (struct sockaddr*)&tp->t_sad,
	&tp->t_salen)) == -1) {
		pvmlogperror("sockconn() accept");
		return -1;
	} else {
		if (pvmdebmask & (PDMPACKET|PDMTASK)) {
			sprintf(pvmtxt, "sockconn() accept from %s sock %d\n",
				inadport_decimal(&tp->t_sad), tp->t_sock);
			pvmlogerror(pvmtxt);
		}
		close(sock);
#ifndef NOSOCKOPT
		i = 1;
		if (setsockopt(tp->t_sock, IPPROTO_TCP, TCP_NODELAY,
		(char*)&i, sizeof(int)) == -1)
			pvmlogperror("sockconn() setsockopt");
#endif
	}
	if (write(tp->t_sock, hinfo, SIZEHINFO*sizeof(int)) 
	!= SIZEHINFO*sizeof(int)) {
		pvmlogperror("sockconn: write");
		return -1;
	}
	if ((i = fcntl(tp->t_sock, F_GETFL, 0)) == -1)
		pvmlogperror("sockconn: fcntl");
	else {
		i |= O_NDELAY;
		(void)fcntl(tp->t_sock, F_SETFL, i);
	}
	wrk_fds_add(tp->t_sock, 1);

	return 0;
}

#endif /*0*/

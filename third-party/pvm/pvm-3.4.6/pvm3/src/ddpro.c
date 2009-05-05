
static char rcsid[] =
	"$Id: ddpro.c,v 1.48 2004/01/14 18:50:55 pvmsrc Exp $";

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
 *	ddpro.c
 *
 *	Entry points for messages from network.
 *
 * $Log: ddpro.c,v $
 * Revision 1.48  2004/01/14 18:50:55  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.47  2002/04/16 15:06:00  pvmsrc
 * WIN32 Fixes for Multiple Domains.
 * 	- submitted by Jigen Zhou <jigen@icemcfd.com>.
 * (Spanker=kohl)
 *
 * Revision 1.46  2002/02/21 23:18:40  pvmsrc
 * Added new (not to be documented!) PVM_MAX_TASKS env var support.
 * 	- for Mahir Lokvancic <mahir@math.ufl.edu>.
 * 	- forcefully limits the number of tasks that can attach to a
 * 		pvmd, required on Solaris in rare circumstances when hard
 * 		FD_SETSIZE limit is reached, and all hell breaks loose...
 * 	- check return for task_new() call, can now produce NULL ptr,
 * 		indicating PvmOutOfRes...
 * (Spanker=kohl)
 *
 * Revision 1.45  2001/09/26 23:35:19  pvmsrc
 * Added new hd_vmid to hostd struct.
 * 	- use to override local PVM_VMID settings with hostfile "id=" option
 * (Spanker=kohl)
 *
 * Revision 1.44  2001/09/26 21:22:16  pvmsrc
 * Added Handling for Optional Virtual Machine ID.
 * 	- extra vmid comes through with SM_STHOST message (after wincmd).
 * 	- instruct user to type VMID on remote pvmd stdin for manual startup
 * 	- in phase1(), after potential rsh/rexec, write VMID env string
 * 		to remote pvmd's stdin.
 * (Spanker=kohl)
 *
 * Revision 1.43  2001/05/11 17:31:56  pvmsrc
 * Cast getcwd() call, in case header file sucks.
 * 	- eliminates warning message on compile...
 * (Spanker=kohl)
 *
 * Revision 1.42  2001/04/23 14:16:13  pvmsrc
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
 * Revision 1.41  2001/02/07 23:14:02  pvmsrc
 * First Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.40  2000/06/16 16:27:32  pvmsrc
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
 * Revision 1.39  2000/06/13 22:37:56  pvmsrc
 * In dm_db():
 * 	- on successful insert, check whether task already had an mbox
 * 		notify (for mb_tidy()) set up.
 * 	- after first such notify / waitc created, set tp->t_flag
 * 		TF_MBNOTIFY bit.
 * 	- this avoids a HUGE number of redundant notifies for multiple
 * 		inserts by the same task.  D-Oh!
 * (Spanker=kohl)
 *
 * Revision 1.38  2000/02/17 23:12:08  pvmsrc
 * *** Changes for new BEOLIN port ***
 * 	- MPP-like, similar to SP2, etc.
 * 	- submitted by Paul Springer <pls@smokeymt.jpl.nasa.gov>.
 * 	- format-checked & cleaned up by Jeembo...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.37  2000/02/16 21:59:38  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.36  2000/02/10 23:53:35  pvmsrc
 * Added new PvmIPLoopback error code:
 * 	- Master Host IP Address tied to Loopback.
 * 	- check for this case in addhosts(), don't even TRY to add hosts...
 * (Spanker=kohl)
 *
 * Revision 1.35  1999/08/19 15:39:22  pvmsrc
 * Whoa...  New wincmd stuff was whacking addhost protocol!
 * 	- *always* pack in something for wincmd, else the attempt to unpack
 * 		"possible" wincmd will snag start of next host's startup info...
 * 	- damn.
 * (Spanker=kohl)
 *
 * Revision 1.34  1999/07/08 18:59:50  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.33  1999/03/05  17:21:23  pvmsrc
 * improved to work with registry/environment on NT/win95-98
 * and devstudio 5/6
 * (Spanker=sscott)
 *
 * Revision 1.32  1999/02/10  22:38:24  pvmsrc
 * Fixed passing of pvmdebmask to new / slave pvmds...
 * 	- need to do -d0x%x to insure interpreted as hex (not just -d%x).
 * 	- report submitted by Paul Springer <pls@volcanoes.jpl.nasa.gov>.
 * (Spanker=kohl)
 *
 * Revision 1.31  1999/01/28  18:55:30  pvmsrc
 * Modified logic to allow alternate WIN32 pvmd path.
 * 	- use new PVM_WINDPATH env var if set, else new WINPVMDPATH define.
 * 	- append alternate WIN32 command path to end of SM_STHOST message,
 * 		so smart hosters (and pvmd' running hoster()) can unpack it
 * 		and use it if the default Unix pvmd path fails to start a pvmd.
 * (Spanker=kohl)
 *
 * Revision 1.30  1998/10/02  15:43:55  pvmsrc
 * Single source code merge of Win32 and Unix code.
 * (Spanker=sscott)
 *
 * Revision 1.29  1998/02/23  22:51:24  pvmsrc
 * Added AIX4SP2 stuff.
 * (Spanker=kohl)
 *
 * Revision 1.28  1998/01/12  21:13:22  pvmsrc
 * Replaced inline constants with new task output op defines.
 * 	- TO_NEW == -2.
 * 	- TO_SPAWN == -1.
 * 	- TO_EOF == 0.
 * (Spanker=kohl)
 *
 * Revision 1.27  1997/08/29  13:34:59  pvmsrc
 * OS2 Port Submitted by Bohumir Horeni, horeni@login.cz.
 * (Spanker=kohl)
 *
 * Revision 1.26  1997/08/26  22:55:25  pvmsrc
 * Make sure wp->wa_mesg is cleared after sendmessage() call.
 * 	- otherwise wait_delete() will re-free the pmsg struct causing
 * 		major bogusness...
 * (Spanker=kohl)
 *
 * Revision 1.25  1997/08/18  19:08:37  pvmsrc
 * Cleaned up dm_pstatack() routine.
 * 	- had leftover pmsg_dump() calls in it, was cluttering
 * 		up /tmp/pvml.<uid>.
 * (Spanker=kohl)
 *
 * Revision 1.24  1997/06/27  18:03:30  pvmsrc
 * Integrated WIN32 changes.
 *
 * Revision 1.23  1997/06/17  19:04:26  pvmsrc
 * Enhanced WT_SPAWN case in hostfailentry().
 * 	- for PvmTaskDefault, if the host that the pvmd picked has failed,
 * 		go ahead and retry spawning it on another host.
 *
 * Revision 1.22  1997/06/16  13:39:44  pvmsrc
 * Updated exectasks() to pass additional information during spawn ops.
 *
 * Revision 1.21  1997/05/19  21:18:13  pvmsrc
 * Awwww Shit.  I did it.
 * 	- crunchzap() sux, can't free buf in startack() until ac/av used.
 *
 * Revision 1.20  1997/05/12  20:28:17  pvmsrc
 * Removed duplicate #includes...
 *
 * Revision 1.19  1997/05/07  21:22:07  pvmsrc
 * AAAIIIEEEEEEEEE!!!!
 * 	- removed all static-limited string unpacking:
 * 		* replaced with use of:
 * 			upkstralloc() / PVM_FREE() (for pvmd stuff).
 * 			new pvmupkstralloc() / PVM_FREE() (for lpvm stuff).
 * 		* manual allocation of local buffers for sprintf() & packing.
 * 		* alternate static string arrays to replace fixed-length cases.
 * 		* I hope this shit works...  :-Q
 *
 * Revision 1.18  1997/05/02  13:43:43  pvmsrc
 * 	call mpp_load for SP2MPI
 *
 * Revision 1.17  1997/04/30  21:25:54  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.16  1997/04/10  18:59:06  pvmsrc
 * Damn.  Stupid typo (as opposed to a smart one?).
 *
 * Revision 1.15  1997/04/10  18:46:56  pvmsrc
 * Bug fix in dm_db():
 * 	- return value for mb_lookup() call was overwriting req index,
 * 		need to save req in case PvmMboxWaitForInfo...
 *
 * Revision 1.14  1997/04/10  18:32:03  pvmsrc
 * Damn.  Forgot the stupid PvmMboxWaitForInfo flag check...  D-Oh!
 *
 * Revision 1.13  1997/04/10  17:53:19  pvmsrc
 * Added handling of blocking (PvmMboxWaitForInfo) calls to pvm_recvinfo().
 * 	- in dm_db(), on TMDB_GET, if not found create WT_RECVINFO.
 * 	- in dm_db(), on TMDB_PUT, check for matching WT_RECVINFO & release.
 * 	- in hostfailentry(), wipe out any pending WT_RECVINFOs if host
 * 		where task resided is now toast.
 *
 * Revision 1.12  1997/04/09  19:37:56  pvmsrc
 * Added class name and index args to pvmreset() routine:
 * 	- allow systematic elimination of leftover persistents...
 * 	- heh, heh, heh...
 * In dm_db(), check for name (class) and req (index) match
 * 	for TMDB_RESET case (""/-1 for all, resp).
 *
 * Revision 1.11  1997/04/09  14:36:06  pvmsrc
 * PVM patches from the base 3.3.10 to 3.3.11 versions where applicable.
 * Originals by Bob Manchek. Altered by Graham Fagg where required.
 *
 * Revision 1.10  1997/04/08  19:57:47  pvmsrc
 * Promoted mbox static "classes" to public global "pvmmboxclasses".
 * 	- so pvmd can spank mboxes in ddpro.c...  :-Q
 * 	- renamed everywhere, moved decl / extern to global.[ch].
 *
 * Revision 1.9  1997/04/08  19:42:55  pvmsrc
 * *** Added new system reset protocol / wait linkage.
 * 	- new DM_RESET / dm_reset() & DM_RESETACK / dm_resetack().
 * 	- new WT_RESET wait type.
 * 	- modified dm_db() to include new TMDB_RESET(nnr, noresets) option:
 * 		* clean up mboxes, except for "no-reset" tasks.
 * 		* for persistent mboxes, set up WT_RESET to remove mbox on task
 * 			exit, propagate to task's host via DM_RESET.
 * 		* on task exit, WT_RESET wipes mbox out, DM_RESETACK passes
 * 			word on to master pvmd (if necessary).
 *
 * Revision 1.8  1997/03/07  14:05:11  pvmsrc
 * Phixed Phunky Phil #endif.
 * 	- no preceding tab on some archs.
 *
 * Revision 1.7  1997/03/06  20:55:34  pvmsrc
 * 	add call to mpp_load when loading onto compute partition.
 *
 * Revision 1.6  1997/02/13  19:05:05  pvmsrc
 * Fixed mbox cleanup problem:
 * 	- in dm_db() for TMDB_PUT case, if successful create master PVMD
 * 		notify on inserting task (if task not local, forward DM_NOTIFY).
 * 	- then on task exit, call mb_tidy() if WT_TASKX notify wait context
 * 		exists (in hostfailentry() and task_cleanup()), or if empty
 * 		notify propagates back to master PVMD via DM_NOTIFYACK.
 *
 * Revision 1.5  1997/02/13  15:10:00  pvmsrc
 * Removed unnecessary extern for struct waitc *waitlist.
 * 	- now in global.h.
 *
 * Revision 1.4  1997/01/28  19:26:14  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/25  13:57:16  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.2  1996/10/24  21:50:47  pvmsrc
 * Moved #include "global.h" below other #include's for typing.
 * Added #include <pvmtev.h> for TEV_* constants.
 * Added extern struct Pvmtracer pvmtracer for tracer info.
 * Modified exectasks():
 * 	- if tracer present, adjust settings for trace & output collect,
 * 		and munge the environment to replace values for PVMTMASK,
 * 		PVMTRCBUF & PVMTRCOPT.
 * 	- replaced inline code with call to new tev_send_newtask().
 * 	- modified checking of trctid / outtid, check for < 0, not non-zero
 * 		in case task denies external collect.
 * Added handling of new DM_SLCONF_TRACE in dm_slconf():
 * 	- tracer update configuration msg.
 * 	- also added packing up of same in startack().
 *
 * Revision 1.1  1996/09/23  23:44:06  pvmsrc
 * Initial revision
 *
 * Revision 1.15  1996/05/13  20:23:21  manchek
 * a few fixes to manual startup code
 *
 * Revision 1.14  1995/07/24  18:23:14  manchek
 * spelling problem
 *
 * Revision 1.13  1995/07/19  21:27:20  manchek
 * use new function pvmnametag instead of [dts]mname
 *
 * Revision 1.12  1995/07/03  19:03:35  manchek
 * if shared memory, get status from global table in dm_task before replying
 *
 * Revision 1.11  1995/06/16  16:11:34  manchek
 * hack to pass output and trace sink to mpp_load for PGON
 *
 * Revision 1.10  1995/05/30  17:26:30  manchek
 * added ifdefs for SP2MPI architecture
 *
 * Revision 1.9  1995/05/17  16:04:36  manchek
 * added HF_OVERLOAD flag.
 * changed global mytid to pvmmytid
 *
 * Revision 1.8  1995/02/01  21:03:07  manchek
 * addhosts returns PvmDSysErr instead of -1 if something breaks.
 * mpp_load is called with environment so it can be passed to tasks.
 * dm_execack processes received tids in order instead of backwards
 *
 * Revision 1.7  1994/10/15  19:02:36  manchek
 * cast message tags for comparison as integer.
 * send output and trace open messages for dmp and shmem ports.
 * check newhosts when deleting host
 *
 * Revision 1.6  1994/07/18  19:18:10  manchek
 * hostfailentry() no longer matches pvmd' to t0.
 * wa_dep wasn't propogated from hoststart to htupd
 *
 * Revision 1.5  1994/06/30  21:33:46  manchek
 * addhosts() uses PVM_DPATH envar
 *
 * Revision 1.4  1994/06/21  18:29:21  manchek
 * subscript arith in dmname() broke with opt
 *
 * Revision 1.3  1994/06/04  22:33:16  manchek
 * missed resetting busyadding in hostfailentry(WT_HTUPD)
 *
 * Revision 1.2  1994/06/03  20:38:12  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:47  manchek
 * Initial revision
 *
 */


#ifdef IMA_TITN
#include <bsd/sys/types.h>
#else
#include <sys/types.h>
#endif

#ifdef NEEDMENDIAN
#include <machine/endian.h>
#endif
#ifdef NEEDENDIAN
#include <endian.h>
#endif
#ifdef NEEDSENDIAN
#include <sys/endian.h>
#endif

#ifdef WIN32
#include "pvmwin.h"
#endif

#if defined(WIN32) || defined(CYGWIN)
#include "..\xdr\types.h"
#include "..\xdr\xdr.h"
#else
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

#ifdef WIN32
#include <sys/stat.h>
#include <time.h>
#include <direct.h>
#include <errno.h>
#else 
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#include <errno.h>
#include <stdio.h>

#include <pvm3.h>
#include <pvmproto.h>
#include "pvmalloc.h"
#include "host.h"
#include "pmsg.h"
#include "waitc.h"
#include "task.h"
#include "listmac.h"
#if defined(IMA_PGON) || defined(IMA_I860) || defined(IMA_CM5) \
	|| defined(SHMEM) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
	|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
#include "pvmdmp.h"
#endif
#include "bfunc.h"
#include <pvmtev.h>
#include "msgbox.h"
#include "global.h"


void pvmbailout();
char *inadport_hex();
char *getenv();
void hex_inadport __ProtoGlarp__ (( char *, struct sockaddr_in * ));
void mesg_rewind __ProtoGlarp__ (( struct pmsg * ));
void tev_send_newtask();

char **colonsep();
char *varsub();
char *pvmnametag();

extern int pvmdebmask;				/* from pvmd.c */
extern char **epaths;				/* from pvmd.c */
extern char *debugger;				/* from pvmd.c */
extern struct htab *filehosts;		/* from pvmd.c */
extern struct htab *hosts;			/* from pvmd.c */
extern int hostertid;				/* from pvmd.c */
extern struct task *locltasks;		/* from task.c */
extern char *myarchname;			/* from pvmd.c */
extern int myhostpart;				/* from pvmd.c */
extern int pvmmytid;				/* from pvmd.c */
extern struct htab *newhosts;		/* from pvmd.c */
extern int pprime;					/* from pvmd.c */
extern int runstate;				/* from pvmd.c */
extern int pvmschedtid;				/* from pvmd.c */
extern struct Pvmtracer pvmtracer;	/* from pvmd.c */
extern int tidhmask;				/* from pvmd.c */
extern int tidlmask;				/* from pvmd.c */

#ifndef IMA_WIN32_WATCOM
extern char **environ;
#endif

int busyadding = 0;					/* lock for addhost op */
int hosterwid = 0;					/* wid waiting on hoster */


/***************
 **  Private  **
 **           **
 ***************/

int dm_add();
int dm_addack();
int dm_db();
int dm_dback();
int dm_delhost();
int dm_delhostack();
int dm_halt();
int dm_hostsync();
int dm_hostsyncack();
int dm_htcommit();
int dm_htdel();
int dm_htupd();
int dm_htupdack();
int dm_mca();
int dm_notify();
int dm_notifyack();
int dm_null();
int dm_pstat();
int dm_pstatack();
int dm_reset();
int dm_resetack();
int dm_sendsig();
int dm_slconf();
int dm_exec();
int dm_execack();
int dm_startack();
int dm_task();
int dm_taskack();
int dm_taskout();

int (*netswitch[])() = {
	dm_add, dm_addack,
	dm_exec, dm_execack,
	dm_sendsig,
	dm_htupd, dm_htupdack,
	dm_htcommit,
	dm_slconf,
	dm_startack,
	dm_task, dm_taskack,
	dm_delhost, dm_delhostack,
	dm_null,
	dm_taskout,
	dm_pstat, dm_pstatack,
	dm_halt,
	dm_mca,
	dm_notify, dm_notifyack,
	dm_db, dm_dback,
	dm_reset, dm_resetack,
	dm_htdel,
	dm_hostsync, dm_hostsyncack,
};


/*	hostfailentry()
*
*	We've decided a remote pvmd has failed.
*	Wake up any wait context waiting on that host (different action
*	for each kind of wait).
*	Send a HTDEL message to remaining hosts except us.
*/

int
hostfailentry(hp)
	struct hostd *hp;
{
	int hpart = hp->hd_hostpart;
	struct waitc *wp, *wp2;
	struct pmsg *mp;
	struct pvmmentry *ep;

	if (pvmdebmask & PDMHOST) {
		pvmlogprintf("hostfailentry() host %s\n", hp->hd_name);
		hd_dump(hp);
	}

	if (hp == hosts->ht_hosts[hosts->ht_master]) {
		pvmlogerror("hostfailentry() lost master host, we're screwwwed\n");
		pvmbailout(0);
	}

	/*
	* if we're master pvmd, send HT_DEL message to all others
	*/

	if (hp->hd_hostpart && hosts->ht_master == hosts->ht_local) {
		struct hostd *hp2;
		int hh;

		mp = mesg_new(0);
		mp->m_tag = DM_HTDEL;
		pkint(mp, hosts->ht_serial);
		pkint(mp, hp->hd_hostpart);
		for (hh = hosts->ht_last; hh > 0; hh--)
			if (hh != hosts->ht_local
			&& (hp2 = hosts->ht_hosts[hh]) && hp2 != hp) {
				mp->m_ref++;
				mp->m_dst = hp2->hd_hostpart | TIDPVMD;
				sendmessage(mp);
			}
		pmsg_unref(mp);

		/* inform the scheduler too */

		if (pvmschedtid) {
			mp = mesg_new(0);
			mp->m_tag = SM_HOSTX;
			mp->m_dst = pvmschedtid;
			pkint(mp, hp->hd_hostpart | TIDPVMD);
			sendmessage(mp);
		}
	}

	for (wp = waitlist->wa_link; wp != waitlist; wp = wp->wa_link) {
		if (wp->wa_on && (wp->wa_on & TIDHOST) == hpart) {
			switch (wp->wa_kind) {

			case WT_ADDHOST:	/* the master must have died */
			case WT_DELHOST:	/* the master must have died */

				pvmlogprintf("hostfailentry() can't deal with wait kind %d\n",
						wp->wa_kind);
				break;

			case WT_HTUPD:
				if (wp->wa_peer == wp) {
					int hh;

					mp = mesg_new(0);
					mp->m_tag = DM_HTCOMMIT;

					for (hh = hosts->ht_last; hh > 0; hh--)
						if (hh != hosts->ht_local && (hp = hosts->ht_hosts[hh])) {
							mp->m_ref++;
							mp->m_dst = hp->hd_hostpart | TIDPVMD;
							sendmessage(mp);
						}
					pmsg_unref(mp);

					busyadding = 0;
					sendmessage(wp->wa_mesg);
					wp->wa_mesg = 0;
				}
				break;

			case WT_SPAWN:
				{
					struct waitc_spawn *wxp;
					int retry = 0;
					int v;

					wxp = (struct waitc_spawn*)wp->wa_spec;

	/* mark tasks assigned to this host as failed */
	/* (or reset if PvmTaskDefault) */

					for (v = wxp->w_veclen; v-- > 0; )
						if (wxp->w_vec[v] == hp->hd_hostpart)
							if (!(wxp->w_flags
									& (PvmTaskHost|PvmTaskArch)))
							{
								wxp->w_vec[v] = 0;
								retry++;
							}
							else
								wxp->w_vec[v] = PvmHostFail;

					ht_delete(wxp->w_ht, hp);

	/* try assigning task again without failed host */

					if ( retry )
						assign_tasks(wp);

	/* is this was the last wait, reply to task */

					if (wp->wa_peer == wp)
						assign_tasks(wp);
				}
				break;

			case WT_TASK:

	/* send message if we're the last waiter */

				if (wp->wa_peer == wp) {
					mp = wp->wa_mesg;
					mp->m_ref++;
					sendmessage(mp);
				}
				break;

			case WT_HOSTSTART:

	/* reply to waiter */

				busyadding = 0;
				if (wp->wa_spec) {
					free_waitc_add((struct waitc_add *)wp->wa_spec);
					wp->wa_spec = 0;
				}
				pkint(wp->wa_mesg, PvmDSysErr);
				sendmessage(wp->wa_mesg);
				wp->wa_mesg = 0;
				break;

			case WT_TASKX:
				if (wp->wa_tid && wp->wa_mesg) {
					sendmessage(wp->wa_mesg);
					wp->wa_mesg = 0;
				}
				mb_tidy(wp->wa_on);
				break;

			case WT_RESET:
				if (wp->wa_tid && wp->wa_mesg) {
					sendmessage(wp->wa_mesg);
					wp->wa_mesg = 0;
				}
				mb_tidy_reset(wp->wa_on);
				break;

			case WT_RECVINFO:
				/* clean up pending recvinfo */
				ep = (struct pvmmentry *) wp->wa_spec;
				if ( ep->me_msg )	/* class name (overload :-Q) */
					PVM_FREE( ep->me_msg );
				PVM_FREE( ep );
				break;

			case WT_HOSTF:
				sendmessage(wp->wa_mesg);
				wp->wa_mesg = 0;
				break;

			case WT_PSTAT:
			case WT_MSTAT:
			case WT_HOSTSYNC:
				pkint(wp->wa_mesg, PvmHostFail);
				sendmessage(wp->wa_mesg);
				wp->wa_mesg = 0;
				break;

			default:
				pvmlogprintf("hostfailentry() alien wait kind %d\n",
						wp->wa_kind);
				break;
			}

			wp2 = wp->wa_rlink;
			wait_delete(wp);
			wp = wp2;
		}
	}
	return 0;
}


int
netentry(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int c = mp->m_tag;

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf(
				"netentry() from host %s src t%x dst t%x tag %s wid %d\n",
				hp->hd_name, mp->m_src, mp->m_dst, pvmnametag(c, (int *)0),
				mp->m_wid);
/*
		pvmhdump(mp->m_frag->fr_link->fr_dat, mp->m_frag->fr_link->fr_len,
				"netentry() ");
*/
	}

	if (c < (int)DM_FIRST || c > (int)DM_LAST) {
		pvmlogprintf("netentry() message from t%x with bogus code %s\n",
				mp->m_src, pvmnametag(c, (int *)0));
		goto bail;
	}

	c -= DM_FIRST;
	(netswitch[c])(hp, mp);

bail:
	pmsg_unref(mp);
	return 0;
}


/*********************************
 **  Pvmd message entry points  **
 **                             **
 *********************************/

/*	hostids_new()
*
*	Get a list of new host tids (ones not it user in host table).
*	Wrap around to lowest number when we reach the maximum.
*	Return as many as are available.
*/

int
hostids_new(num, tids)
	int *num;			/* count requested, returned */
	int *tids;			/* return tids */
{
	static int lasthh = 1;
	int oldhh;
	int maxhostid = tidhmask >> (ffs(tidhmask) - 1);
	int i;

	oldhh = lasthh;

	/* find next free hostid */

	for (i = 0; i < *num; i++) {
		if (++lasthh > maxhostid)
			lasthh = 1;
		while ((lasthh <= hosts->ht_last && hosts->ht_hosts[lasthh])) {
			if (++lasthh > maxhostid)
				lasthh = 1;
			if (lasthh == oldhh)
				goto done;
		}
		tids[i] = lasthh << (ffs(tidhmask) - 1);
/*
		pvmlogprintf("hostids_new() tid %x\n", tids[i]);
*/
	}

done:
	return *num = i;
}


free_waitc_add(wxp)
	struct waitc_add *wxp;
{
	int i;

	for (i = 0; i < wxp->w_num && wxp->w_hosts[i]; i++)
		hd_unref(wxp->w_hosts[i]);
	PVM_FREE(wxp->w_hosts);
	PVM_FREE(wxp);
	return 0;
}


/*	addhosts()
*
*	Add hosts requested with DM_ADD or SM_ADD.  Call the hoster or
*	start 'em.
*/

int
addhosts(mp, rmp)
	struct pmsg *mp;	/* the request message */
	struct pmsg *rmp;	/* reply message blank */
{
	struct hostd *hp, *hp2;
	struct pmsg *mp2;
	struct waitc *wp = 0;
	struct waitc_add *wxp = 0;
	int i, j;
	int count;
	int ngood;
	int ntid;
	struct hostent *he;
	int maxhostid = (tidhmask >> ffs(tidhmask) - 1);
	int hh;
	int pid;
	int *tids;
	char *winpvmdpath;
	char *pvmdpath;
	char *vmid;
	char *buf;
	int len;

	/*
	* have to lock this for 2 reasons:
	*  1. system can't handle overlapping host table updates,
	*  2. the new host tids aren't reserved
	*/
	if (busyadding) {
/*
		pvmlogerror("addhosts() already adding new hosts\n");
*/
		pkint(rmp, PvmAlready);
		sendmessage(rmp);
		return 0;
	}

	busyadding = 1;

	/* sanity check count */

	if (upkint(mp, &count) || count < 1 || count > maxhostid) {
		pvmlogerror("addhosts() bad msg format\n");
		goto bad;
	}

	/*
	* make wait context, extract host list from message,
	*/

	wp = wait_new(WT_HOSTSTART);
	wp->wa_tid = mp->m_src;
	wp->wa_dep = mp->m_wid;
	wxp = TALLOC(1, struct waitc_add, "waix");
	wxp->w_num = count;
	wxp->w_hosts = TALLOC(count, struct hostd *, "waiv");
	BZERO((char*)wxp->w_hosts, count * sizeof(struct hostd *));
	wp->wa_spec = (void *)wxp;

	for (i = 0; i < count; i++) {
		hp = hd_new(0);
		wxp->w_hosts[i] = hp;
		if (upkstralloc(mp, &buf)) {
			pvmlogerror("addhosts() bad msg format\n");
			goto bad;
		}
		if (parsehost(buf, hp)) {
			hp->hd_err = PvmBadParam;

		} else {

		/* Set unspecified fields from hostfile if available */

			if (filehosts &&
					((hp2 = nametohost(filehosts, hp->hd_name))
					|| (hp2 = filehosts->ht_hosts[0])))
				applydefaults(hp, hp2);
		}
		PVM_FREE(buf);
	}

	/*
	* verify we have a chance to add these babies...
	* check whether our IP is "real" or just loopback
	*/

	hp = hosts->ht_hosts[hosts->ht_local];

	if ( hp->hd_sad.sin_addr.s_addr == htonl(0x7f000001) ) {

		/* damn, we're hosed.  bail on host adds with new */
		/* PvmIPLoopback error code... */

		for (i = 0; i < count; i++) {
			hp = wxp->w_hosts[i];
			if (hp->hd_err)
				continue;
			hp->hd_err = PvmIPLoopback;
		}
	}

	/*
	* lookup IP addresses  XXX we already have some of them
	*/

	ngood = 0;
	for (i = 0; i < count; i++) {
		hp = wxp->w_hosts[i];
		if (hp->hd_err)
			continue;

		if (he = gethostbyname(hp->hd_aname ? hp->hd_aname : hp->hd_name)) {
			BCOPY(he->h_addr_list[0], (char*)&hp->hd_sad.sin_addr,
					sizeof(struct in_addr));

		} else {
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf(
						"start_slaves() can't gethostbyname: %s\n",
						hp->hd_name);
			}
			hp->hd_err = PvmNoHost;
			continue;
		}

	/* make sure it's not already configured */

		if (!(hp->hd_flag & HF_OVERLOAD)) {
			for (hh = hosts->ht_last; hh > 0; hh--)
				if ((hp2 = hosts->ht_hosts[hh])
				&& (hp2->hd_sad.sin_addr.s_addr == hp->hd_sad.sin_addr.s_addr)) {
					hp->hd_err = PvmDupHost;
					break;
				}
			if (hp->hd_err)
				continue;

	/* make sure new ones aren't duplicated */

			for (j = i; j-- > 0; )
				if (hp->hd_sad.sin_addr.s_addr
				== wxp->w_hosts[j]->hd_sad.sin_addr.s_addr) {
					hp->hd_err = PvmDupHost;
					break;
				}
			if (hp->hd_err)
				continue;
		}

		ngood++;
	}

	/*
	* assign tids  XXX these are unreserved until hosts are added...
	*/

	ntid = ngood;
	tids = TALLOC(ngood, int, "xxx");
	hostids_new(&ntid, tids);
	if (ntid < ngood) {
		pvmlogerror("addhosts() out of hostids\n");
		ngood = ntid;
	}
	for (j = i = 0; i < count; i++) {
		hp = wxp->w_hosts[i];
		if (hp->hd_err)
			continue;
		if (j < ntid)
			hp->hd_hostpart = tids[j++];
		else
			hp->hd_err = PvmOutOfRes;
	}
	PVM_FREE(tids);

/*
	if (!ngood) {
		XXX don't really need to send the message
	}
*/

	/* keep stub reply message to caller */

	wp->wa_mesg = rmp;

	/* make request message and send to hoster or pvmd' */

	mp2 = mesg_new(0);
	mp2->m_wid = wp->wa_wid;
	pkint(mp2, ngood);
	if (!(pvmdpath = getenv("PVM_DPATH")))
		pvmdpath = PVMDPATH;
	if (!(winpvmdpath = getenv("PVM_WINDPATH")))
		winpvmdpath = WINPVMDPATH;
	for (i = 0; i < count; i++) {
		hp = wxp->w_hosts[i];
		if (hp->hd_err)
			continue;
		pkint(mp2, hp->hd_hostpart);
		pkstr(mp2, hp->hd_sopts ? hp->hd_sopts : "");

		if (hp->hd_login) {
			len = strlen(hp->hd_login)
				+ strlen((hp->hd_aname ? hp->hd_aname : hp->hd_name))
				+ 2;
			buf = TALLOC( len, char, "hdl" );
			sprintf(buf, "%s@%s", hp->hd_login,
					(hp->hd_aname ? hp->hd_aname : hp->hd_name));
		}
		else
			buf = STRALLOC( (hp->hd_aname
					? hp->hd_aname : hp->hd_name) );
		pkstr(mp2, buf);
		PVM_FREE(buf);

		/* default unix dpath */
		len = strlen( (hp->hd_dpath ? hp->hd_dpath : pvmdpath) ) + 1
			+ strlen( (hp->hd_sopts && !strcmp(hp->hd_sopts, "ms")
					? "-S" : "-s") ) + 1
			+ 2 + 16 + 1
			+ 2 + strlen( hp->hd_name ) + 1
			+ 5 * ( 16 + 1 );
		buf = TALLOC( len, char, "hdall" );
		(void)sprintf(buf, "%s %s -d0x%x -n%s %d %s %d",
				(hp->hd_dpath ? hp->hd_dpath : pvmdpath),
				(hp->hd_sopts && !strcmp(hp->hd_sopts, "ms")
					? "-S" : "-s"),
				pvmdebmask,
				hp->hd_name,
				hosts->ht_master,
				inadport_hex(
					&hosts->ht_hosts[hosts->ht_master]->hd_sad),
				hosts->ht_hosts[hosts->ht_master]->hd_mtu);
		(void)sprintf(buf + strlen(buf), " %d %s",
				((hp->hd_hostpart & tidhmask) >> (ffs(tidhmask) - 1)),
				inadport_hex(&hp->hd_sad));
		pkstr(mp2, buf);
		PVM_FREE(buf);

		/* default WIN32 dpath - only if not set manually (a la dx=) */
		if (!(hp->hd_dpath)){
		len = strlen( winpvmdpath ) + 1
			+ strlen( (hp->hd_sopts && !strcmp(hp->hd_sopts, "ms")
					? "-S" : "-s") ) + 1
			+ 2 + 16 + 1
			+ 2 + strlen( hp->hd_name ) + 1
			+ 5 * ( 16 + 1 );
		buf = TALLOC( len, char, "hdallwin" );
		(void)sprintf(buf, "%s %s -d0x%x -n%s %d %s %d",
				winpvmdpath,
				(hp->hd_sopts && !strcmp(hp->hd_sopts, "ms")
					? "-S" : "-s"),
				pvmdebmask,
				hp->hd_name,
				hosts->ht_master,
				inadport_hex(
					&hosts->ht_hosts[hosts->ht_master]->hd_sad),
				hosts->ht_hosts[hosts->ht_master]->hd_mtu);
		(void)sprintf(buf + strlen(buf), " %d %s",
				((hp->hd_hostpart & tidhmask) >> (ffs(tidhmask) - 1)),
				inadport_hex(&hp->hd_sad));
		pkstr(mp2, buf);
		PVM_FREE(buf);
		}
		/* be sure to pack SOMETHING, dammit */
		else
			pkstr(mp2, "");

		/* Include VMID (If Any) */
		if (hp->hd_vmid)
			pkstr(mp2, hp->hd_vmid);
		else if (vmid = getenv("PVM_VMID"))
			pkstr(mp2, vmid);
		/* be sure to pack SOMETHING, dammit */
		else
			pkstr(mp2, "");
	}
	mp2->m_tag = SM_STHOST;

#if !defined(WIN32) && !defined(IMA_OS2)

	if (hostertid) {
		hosterwid = wp->wa_wid;
		mp2->m_dst = hostertid;
		wp->wa_on = hostertid;
		sendmessage(mp2);

	} else {
		wp->wa_on = TIDPVMD;

		if (pid = fork()) {		/* still us */
			if (pid == -1) {
	/* nack request if can't fork */
				pvmlogperror("addhosts() fork");
				goto bad;

			} else {
				pprime = pid;
				pmsg_unref(mp2);
			}

		} else {				/* pvmd' to do the startup */
			beprime();
			mesg_rewind(mp2);
			hoster(mp2);
		}
	}

#else /* WIN32 */

	if (!hostertid)	{			/* no hoster yet */
		hostertid = tid_new();	/* give him a tid so that we can send
									him the message */

		if (start_hoster(hostertid) == -1) {
			hostertid=0;			/* you did not make it buddy */
			pvmlogperror("addhosts(): could not start hoster \n");
			goto bad;
		}
	}
	hosterwid = wp->wa_wid;
	mp2->m_dst = hostertid;
	wp->wa_on = hostertid;
	sendmessage(mp2);

#endif

	return 0;

bad:
	if (wxp)
		free_waitc_add(wxp);
	if (wp) {
		wp->wa_mesg = 0;	/* shared with rmp */
		wait_delete(wp);
	}
	busyadding = 0;
	pkint(rmp, PvmDSysErr);
	sendmessage(rmp);
	return 0;
}

#ifdef WIN32

/*	start_hoster()
*
*	forkexec the hoster process which
*	will start other pvmd's.
*	acts like pvmd'
*/

int
start_hoster(reserved_tid)
	int reserved_tid;
{
	char hosterpath[128];

	(void) strcpy(hosterpath,(char *) pvmgetroot());
	(void) strcat(hosterpath,"\\bin\\WIN32\\hoster.exe");

	if (hostexectasker(hosterpath,reserved_tid))
		return -1;

	return 0;
}

int
hostexectasker(file, tid)
	char *file;
	int tid;
{
	int tids = 0;			/* tid from hosterforkexec */
	
	struct pmsg *rmp;
	struct task *tp = 0;
	int err = 0;			/* error code from forkexec */

	rmp = mesg_new(0);
	rmp->m_dst = pvmmytid;
	rmp->m_src = pvmmytid;

	rmp->m_tag = DM_EXECACK;

	if (err = hosterforkexec(tid,file, &tp)) {
		tids = err;
	} else {
			tp->t_ptid = 0;
			tp->t_outtid = 0;
			tp->t_trctid = 0;
			tp->t_sched = 0;
			tids = tp->t_tid;
	}

	pkint(rmp, 1);
	pkint(rmp, tids);
	sendmessage(rmp);
	
	return err;
}

extern char **environ;

int
hosterforkexec(tid, name, tpp)
	int tid;				/* tid given by hosterexectask */
	char *name;				/* filename */
	struct task **tpp;		/* return task context */
{
	int pid = -1;				/* task pid */
	char *argv[2];
	struct task *tp;		/* new task context */
	char *expected_pid;
	char buf[32];
	char *myenv[100];
	HANDLE hpid;
	char **p, **q;
	struct stat sb;	
	extern int *ptr_nfp;		/* XXX fix this */
	SECURITY_ATTRIBUTES saPipe;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;  /* for CreateProcess call */

	if (stat(name, &sb) == -1) 
		return PvmNoFile;

	if ((tp = task_new(tid)) == NULL) {
		pvmlogperror("forkexec_hoster() too many tasks?\n");
		return PvmOutOfRes;
	}

	p = myenv;
	q = environ;
	while (*q) {
		*p++ = *q++;   	
	}

	/* copy all the environment for socket stuff and more */

	expected_pid=malloc(20 * sizeof(char));
	sprintf(expected_pid, "PVMEPID=%d", *ptr_nfp);
		
	*p++ = expected_pid;
	*p=0;
	pvmputenv(expected_pid);
	argv[0]=name;
	argv[1]=0;
	saPipe.nLength = sizeof(SECURITY_ATTRIBUTES);
	saPipe.lpSecurityDescriptor = NULL;
	saPipe.bInheritHandle = FALSE;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	if (0) {
		pid = CreateProcess(name,  /* filename */
			NULL,  /* full command line for child */
			NULL,  /* process security descriptor */
			NULL,  /* thread security descriptor */
			FALSE,  /* inherit handles? */
			DETACHED_PROCESS,  /* creation flags */
			NULL,  /* inherited environment address */
			NULL,  /* startup dir; NULL = start in current */
			&si,  /* pointer to startup info (input) */
			&pi);  /* pointer to process info (output) */
			
			/* fprintf(stderr,"yup \n"); */
	}
	else
	pid = _spawnve(_P_NOWAIT,name,argv,myenv); 
								  
	if (pid == -1) {
			pvmlogperror("forkexec_hoster() _spawnve");
			/* task_free(&tp); */
			pvmbailout(0);
			return PvmOutOfRes;
	}
		
	task_setpid(tp,*ptr_nfp);
	*ptr_nfp=*ptr_nfp + 1;

	tp->t_flag |= TF_FORKD;

	tp->t_a_out = STRALLOC(name);
	 
	*tpp = tp;
	
	return 0;
}		

#endif 

/*	dm_add()
*
*	(Master pvmd) gets request to add new hosts.
*DocThis
*	DM_ADD(wid) {
*		int count
*		string name[count]
*	}
*EndDocThis
*/

int
dm_add(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct pmsg *rmp;

	rmp = mesg_new(0);
	rmp->m_dst = mp->m_src;
	rmp->m_ctx = mp->m_ctx;
	rmp->m_tag = DM_ADDACK;
	rmp->m_wid = mp->m_wid;
	addhosts(mp, rmp);
	return 0;
}


/*	dm_addack()
*
*	Reply to DM_ADD operation.
*
*	DM_ADDACK(wid_rtn) {
*		int nhosts			// or error code
*		int narches			// if nhosts >= 0
*		{
*			int tid			// or error code
*			string name
*			string arch
*			int mtu
*			int speed
*		} [nhosts]
*	}
*/

int
dm_addack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;

	if (!(wp = wait_get(hp, mp, WT_ADDHOST)))
		return 0;
	pmsg_packbody(wp->wa_mesg, mp);
	sendmessage(wp->wa_mesg);
	wp->wa_mesg = 0;
	wait_delete(wp);
	return 0;
}


/*	exectasks()
*
*	Start tasks requested with DM_EXEC or SM_EXEC.
*	Call the tasker or start 'em.
*	We try to fork/exec given number of tasks.  On first error, fill
*	remainder of tid array with error code.
*	Return tid array to caller.
*/

int
exectasks(mp, rmp, schtid)
	struct pmsg *mp;		/* the request message */
	struct pmsg *rmp;		/* reply message blank */
	int schtid;				/* scheduler for new tasks */
{
	struct pmsg *mp2;			/* reply message hdl */
	int i;
	struct timeval now;
	struct waitc_spawn *wxp;	/* new task parameters */
	int munge_tenv = 0;
	char tmp[255];
	char *wd = 0;
	char *savewd = 0;

	wxp = TALLOC(1, struct waitc_spawn, "waix");
	BZERO((char*)wxp, sizeof(struct waitc_spawn));

	/* unpack message */

	if (upkuint(mp, &wxp->w_ptid)
	|| upkstralloc(mp, &wxp->w_file)
	|| upkint(mp, &wxp->w_flags)
	|| upkint(mp, &wxp->w_veclen)
	|| upkint(mp, &wxp->w_argc))
		goto bad;

	if (wxp->w_veclen < 1)
		goto bad;
	wxp->w_vec = TALLOC(wxp->w_veclen, int, "tids");

	wxp->w_argc += 2;
	wxp->w_argv = TALLOC(wxp->w_argc + 1, char*, "argv");
	BZERO((char*)wxp->w_argv, (wxp->w_argc + 1) * sizeof(char*));
	wxp->w_argv++;
	wxp->w_argv[0] = wxp->w_file;
	wxp->w_file = 0;
	wxp->w_argv[--wxp->w_argc] = 0;
	for (i = 1; i < wxp->w_argc; i++)
		if (upkstralloc(mp, &wxp->w_argv[i]))
			goto bad;

	if (upkuint(mp, &wxp->w_outtid)
	|| upkuint(mp, &wxp->w_outctx)
	|| upkuint(mp, &wxp->w_outtag)
	|| upkuint(mp, &wxp->w_trctid)
	|| upkuint(mp, &wxp->w_trcctx)
	|| upkuint(mp, &wxp->w_trctag)
	|| upkuint(mp, &wxp->w_nenv))
		goto bad;

	wxp->w_hosttotal = wxp->w_veclen; /* this is the task count! */

	if (pvmtracer.trctid) {
		if (!(wxp->w_trctid) && pvmtracer.trctag) {
			wxp->w_trctid = pvmtracer.trctid;
			wxp->w_trcctx = pvmtracer.trcctx;
			wxp->w_trctag = pvmtracer.trctag;
			munge_tenv++;
		}
	}

	if (pvmtracer.outtid) {
		if (!(wxp->w_outtid) && pvmtracer.outtag) {
			wxp->w_outtid = pvmtracer.outtid;
			wxp->w_outctx = pvmtracer.outctx;
			wxp->w_outtag = pvmtracer.outtag;
		}
	}

	wxp->w_env = TALLOC((wxp->w_nenv + 1), char*, "env");
	BZERO(wxp->w_env, (wxp->w_nenv + 1) * sizeof(char*));

	for (i = 0; i < wxp->w_nenv; i++)
		if (upkstralloc(mp, &wxp->w_env[i]))
			goto bad;

	if ( upkuint(mp, &wxp->w_instance) || upkuint(mp, &wxp->w_outof))
		goto bad;

	/* check for spawn working directory */

	for (i = 0; i < wxp->w_nenv; i++)
		if ( !strncmp( "PVMSPAWNWD=", wxp->w_env[i],
				strlen("PVMSPAWNWD=") ) )
			wd = STRALLOC( wxp->w_env[i] + strlen("PVMSPAWNWD=") );

	/* munge env for tracing stuff */

	if ( munge_tenv ) {
		sprintf( tmp, "PVMTMASK=%s", pvmtracer.tmask );
		pvmenvinsert( &(wxp->w_env), tmp );

		sprintf( tmp, "PVMTRCBUF=%d", pvmtracer.trcbuf );
		pvmenvinsert( &(wxp->w_env), tmp );

		sprintf( tmp, "PVMTRCOPT=%d", pvmtracer.trcopt );
		pvmenvinsert( &(wxp->w_env), tmp );

		for ( wxp->w_nenv = 0 ; wxp->w_env[ wxp->w_nenv ] ;
			(wxp->w_nenv)++ );
	}

	/* call ppi_load to get tasks running */

	wxp->w_sched = schtid;

	/* change to desired working directory (if specified) */
	if (wd) {
		savewd = (char *) getcwd( (char *) NULL, 255 );
		chdir( wd );
	}

#if defined(IMA_PGON) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
	if (!(wxp->w_flags & PvmMppFront))
	{
		mpp_load(wxp);
	}
	else
#endif
	{
		ppi_load(wxp);
	}

	/* go back to original directory (if getcwd() was successful) */
	if (savewd)
		chdir( savewd );

for (i = 0; i < wxp->w_veclen; i++) {
		if (wxp->w_vec[i] > 0) {
			if (wxp->w_trctid > 0) {
				tev_send_newtask(
					wxp->w_trctid, wxp->w_trcctx, wxp->w_trctag,
					wxp->w_vec[i], wxp->w_ptid, wxp->w_flags,
					wxp->w_argv[0] );
			}
			if (wxp->w_outtid > 0) {
				mp2 = mesg_new(0);
				mp2->m_dst = wxp->w_outtid;
				mp2->m_ctx = wxp->w_outctx;
				mp2->m_tag = wxp->w_outtag;
				pkint(mp2, wxp->w_vec[i]);
				pkint(mp2, TO_NEW);
				pkint(mp2, wxp->w_ptid);
				sendmessage(mp2);
			}
		}
	}

	pkint(rmp, wxp->w_veclen);
	for (i = 0; i < wxp->w_veclen; i++)
		pkint(rmp, wxp->w_vec[i]);
	sendmessage(rmp);
	goto cleanup;

bad:
	pvmlogprintf("exectasks() from t%x bad msg format\n", mp->m_src);

cleanup:
	if (wxp->w_argv)
		*--wxp->w_argv = 0;
	free_wait_spawn(wxp);
	if (wd)
		PVM_FREE(wd);
	return 0;
}


/*	dm_exec()
*
*	Request to start task processes.
*
*	DM_EXEC(wid) {
*		int ptid
*		string file
*		int flags
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
*/

int
dm_exec(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct pmsg *rmp;

	hp = hp;

	rmp = mesg_new(0);
	rmp->m_dst = mp->m_src;
	rmp->m_ctx = mp->m_ctx;
	rmp->m_tag = DM_EXECACK;
	rmp->m_wid = mp->m_wid;
	exectasks(mp, rmp, pvmschedtid);
	return 0;
}


/*	dm_execack()
*
*	Reply to DM_EXEC op.
*
*	DM_EXECACK(wid_rtn) {
*		int count
*		int tids[count]
*	}
*/

int
dm_execack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;
	struct waitc_spawn *wxp;
	int rcnt;				/* num of tids+errors returned */
	int tid;
	int v;
	int err = 0;
	int i;

	if (!(wp = wait_get(hp, mp, WT_SPAWN)))
		return 0;

	wxp = (struct waitc_spawn*)wp->wa_spec;
	if (upkint(mp, &rcnt))
		goto bad;
	v = wxp->w_veclen;

	/*
	* unpack tids and place in result vector where hostpart is now
	*/

/*
	pvmlogprintf("dm_execack() hp %x vec len %d, repl len %d\n",
			hp->hd_hostpart, v, rcnt);
*/
	i = 0;
	while (rcnt-- > 0) {
		if (upkint(mp, &tid))
			goto bad;
		if (tid < 0)
			err++;
		while (i < v && wxp->w_vec[i] != hp->hd_hostpart)
			i++;
		if (i == v) {
			pvmlogerror("dm_execack() tids don't fit result vector?\n");
			wait_delete(wp);
			return 0;
		}
		wxp->w_vec[i++] = tid;
	}

	if (err)
		ht_delete(wxp->w_ht, hp);

	/*
	* if everyone has checked in, either restart the failed ones
	* or reply to the caller.
	*/

	if (wp->wa_peer == wp)
		assign_tasks(wp);
	wait_delete(wp);
	return 0;

bad:
	pvmlogprintf("dm_execack() from 0x%x bad msg format\n", mp->m_src);
	wait_delete(wp);
	return 0;
}


/*	dm_sendsig()
*
*	Request to send signal to local task.
*
*	DM_SENDSIG {
*		int tid
*		int signum
*	}
*/

int
dm_sendsig(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int tid;
	int signum;
	struct task *tp;

	hp = hp;

	if (upkuint(mp, &tid) || upkint(mp, &signum)) {
		pvmlogerror("dm_sendsig() bad msg format\n");
		return 0;
	}
	if (tp = task_find(tid)) {
		ppi_kill(tp, signum);

	} else
		if (pvmdebmask & (PDMTASK|PDMAPPL)) {
			pvmlogprintf("dm_sendsig() signal for t%x scrapped\n", tid);
		}
	return 0;
}


/*	dm_htupd()
*
*	Host table update phase 1 - (Non-master) pvmd is notified of new
*	host table.
*
*	DM_HTUPD(wid) {
*		int serial
*		int master
*		int console
*		int count
*		{
*			int index
*			string name
*			string arch
*			string hexipaddr
*			int mtu
*			int speed
*			int dsig
*		} [count]
*	}
*/

int
dm_htupd(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int count;			/* number of hosts in message */
	int hh;
	char buf[16];		/* for converting sockaddr */
	struct pmsg *mp2;

	/* unpack new host table params */

	newhosts = ht_new(1);
	newhosts->ht_local = hosts->ht_local;
	upkint(mp, &newhosts->ht_serial);
	upkint(mp, &newhosts->ht_master);
	upkint(mp, &newhosts->ht_cons);

	/* add current hosts to the table */

	ht_merge(newhosts, hosts);

	/* unpack new hosts and add to table */

	/* XXX if we find a host already in table we should kill it with
	   XXX hostfail and put the new one in its place */

	upkint(mp, &count);
	while (count-- > 0) {
		upkint(mp, &hh);
		hp = hd_new(hh);
		upkstralloc(mp, &hp->hd_name);
		upkstralloc(mp, &hp->hd_arch);
		upkstr(mp, buf, sizeof(buf));
		hex_inadport(buf, &hp->hd_sad);
		upkint(mp, &hp->hd_mtu);
		upkint(mp, &hp->hd_speed);
		upkint(mp, &hp->hd_dsig);
		ht_insert(newhosts, hp);
		hd_unref(hp);
	}

	if (pvmdebmask & PDMHOST) {
		pvmlogerror("dm_htupd() new host table:\n");
		ht_dump(newhosts);
	}
	runstate = PVMDHTUPD;

	/* reply to sender that we have new host table */

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_ctx = mp->m_ctx;
	mp2->m_tag = DM_HTUPDACK;
	mp2->m_wid = mp->m_wid;
	sendmessage(mp2);
	return 0;
}


/*	dm_htupdack()
*
*	Reply to DM_HTUPD op.
*
*	DM_HTUPDACK(wid_rtn) { }
*/

int
dm_htupdack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;

	if (!(wp = wait_get(hp, mp, WT_HTUPD)))
		return 0;

	/* is this is the last host checking in, send ht commit */

	if (wp->wa_peer == wp) {
		int hh;

		mp = mesg_new(0);
		mp->m_tag = DM_HTCOMMIT;

		for (hh = hosts->ht_last; hh > 0; hh--)
			if (hh != hosts->ht_local && (hp = hosts->ht_hosts[hh])) {
				mp->m_ref++;
				mp->m_dst = hp->hd_hostpart | TIDPVMD;
				sendmessage(mp);
			}
		pmsg_unref(mp);

		busyadding = 0;
		sendmessage(wp->wa_mesg);
		wp->wa_mesg = 0;
	}
	wait_delete(wp);
	return 0;
}


/*	ht_diff()
*
*	Generates a host table containing entries in ht2 but not in ht1.
*/

struct htab *
ht_diff(htp2, htp1)
	struct htab *htp2;		/* more */
	struct htab *htp1;		/* less */
{
	struct htab *htp;
	int hh;

	htp = ht_new(1);
	for (hh = htp2->ht_last; hh > 0; hh--)
		if (htp2->ht_hosts[hh] && !htp2->ht_hosts[hh]->hd_err
		&& (hh > htp1->ht_last || !htp1->ht_hosts[hh]))
			ht_insert(htp, htp2->ht_hosts[hh]);
	return htp;
}


/*	gotnewhosts()
*
*	Used to wake up any waitcs of kind WT_HOSTA.
*	Sends a message containing count and list of new d-tids.
*/

gotnewhosts(htp2, htp1)
	struct htab *htp2;		/* new host table */
	struct htab *htp1;		/* old host table */
{
	struct pmsg *mp;
	struct htab *htp;
	struct waitc *wp, *wp2;
	int hh;

	mp = 0;
	for (wp = waitlist->wa_link; wp != waitlist; wp = wp2) {
		wp2 = wp->wa_link;
		if (wp->wa_kind == WT_HOSTA) {
			if (!mp) {
				mp = mesg_new(0);
				htp = ht_diff(htp2, htp1);
				pkint(mp, htp->ht_cnt);
				for (hh = htp->ht_last; hh > 0; hh--)
					if (htp->ht_hosts[hh])
						pkint(mp, htp->ht_hosts[hh]->hd_hostpart);
				ht_free(htp);
			}
			mp->m_ref++;
			mp->m_dst = wp->wa_mesg->m_dst;
			mp->m_ctx = wp->wa_mesg->m_ctx;
			mp->m_tag = wp->wa_mesg->m_tag;
			sendmessage(mp);
			if (wp->wa_count != -1 && --wp->wa_count < 1)
				wait_delete(wp);
		}
	}
	if (mp)
		pmsg_unref(mp);
	return 0;
}


/*	dm_htcommit()
*
*	Host table update phase 2 - commit to new host table.
*
*	DM_HTCOMMIT { }
*/

int
dm_htcommit(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct htab *htp;

	if (hp != hosts->ht_hosts[hosts->ht_master]) {
		pvmlogprintf("dm_htcommit() from t%x (not master)?\n",
				mp->m_src);
	}

	if (newhosts) {
		htp = hosts;
		hosts = newhosts;
		newhosts = 0;
		if (pvmdebmask & PDMHOST) {
			pvmlogprintf(
			"dm_htcommit() committing from host table serial %d to %d\n",
					htp->ht_serial, hosts->ht_serial);
		}

		gotnewhosts(hosts, htp);
		ht_free(htp);
		runstate = PVMDNORMAL;

	} else {
		pvmlogerror("dm_htcommit() no new host table pending?\n");
	}
	return 0;
}


/*	dm_slconf()
*
*	Pvmd gets config info (from master).
*	One of these should arrive before a pvmd is fully up, even
*	before the host table update, to set private params.
*	More may be used later to set random knobs.
*
*	DM_SLCONF {
*		{
*			int fieldtype		// from ddpro.h
*			string value
*		} []					// implied
*	}
*/

int
dm_slconf(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int t;				/* field type */
	char *s, *s2;

	mp = mp;
	hp = hp;

	while (!upkint(mp, &t) && !upkstralloc(mp, &s)) {
		switch (t) {

		case DM_SLCONF_EP:
			if (pvmdebmask & (PDMTASK|PDMSTARTUP)) {
				pvmlogprintf("dm_slconf() ep<%s>\n", s);
			}
			epaths = colonsep(varsub(s));
			PVM_FREE(s);
			break;

		case DM_SLCONF_BP:
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("dm_slconf() bp<%s>\n", s);
			}
			debugger = varsub(s);
			PVM_FREE(s);
			break;

		case DM_SLCONF_WD:
			if (pvmdebmask & (PDMTASK|PDMSTARTUP)) {
				pvmlogprintf("dm_slconf() wd<%s>\n", s);
			}
			s2 = varsub(s);
			if (chdir(s2) == -1)
				pvmlogperror(s2);
			PVM_FREE(s);
			PVM_FREE(s2);
			break;

		case DM_SLCONF_SCHED:
			if (pvmdebmask & (PDMSCHED|PDMSTARTUP)) {
				pvmlogprintf("dm_slconf() sched<t%x>\n", pvmschedtid);
			}
			pvmschedtid = pvmxtoi(s);
			break;

		case DM_SLCONF_TRACE: {
			Pvmtmask tmask;
			int ttid, tctx, ttag, otid, octx, otag, tbuf, topt;
			if (pvmdebmask & (PDMTRACE|PDMSTARTUP)) {
				pvmlogprintf("dm_slconf() tracer<t%x>\n",
					pvmtracer.trctid);
			}
			if (sscanf(s, "%x %d %d %x %d %d %d %d %s",
					&ttid, &tctx, &ttag, &otid, &octx, &otag,
					&tbuf, &topt, tmask) != 9) {
				pvmlogprintf("dm_slconf() bogus string<%s>\n", s);
			}
			else {
				pvmtracer.trctid = ttid;
				pvmtracer.trcctx = tctx;
				pvmtracer.trctag = ttag;
				pvmtracer.outtid = otid;
				pvmtracer.outctx = octx;
				pvmtracer.outtag = otag;
				pvmtracer.trcbuf = tbuf;
				pvmtracer.trcopt = topt;
				BCOPY(tmask,pvmtracer.tmask,TEV_MASK_LENGTH);
			}
			break;
		}

		default:
			pvmlogprintf("dm_slconf() ? type %d val <%s>\n", t, s);
			PVM_FREE(s);
			break;
		}
	}
	return 0;
}


/*	startack()
*
*	Take results from pvmd' or hoster.  Update the machine config
*	if any were successful.  Reply to the DM_ADD or SM_ADD request.
*/

int
startack(wp, mp)
	struct waitc *wp;		/* wait context on hoster */
	struct pmsg *mp;
{
	struct hostd *hp;
	struct pmsg *mp2;
	struct waitc *wp2;		/* seed waitc for htupd peer group */
	struct waitc *wp3;
	int count;				/* num of new hosts */
	int happy;				/* num of happy new hosts */
	struct waitc_add *wxp;
	char *av[16];			/* for reply parsing */
	int ac;
	int ver;
	int i, j;
	int t;
	int hh;
	char *buf;
	char buf2[256];

	/*
	* unpack startup results, update hosts in wait context
	*/

	wxp = (struct waitc_add *)wp->wa_spec;
	count = wxp->w_num;
	upkint(mp, &j);

	while (j-- > 0) {
		if (upkuint(mp, &t) || upkstralloc(mp, &buf)) {
			pvmlogerror("startack() bad message format\n");
			pkint(wp->wa_mesg, PvmDSysErr);
			sendmessage(wp->wa_mesg);
			wp->wa_mesg = 0;
			wait_delete(wp);
			busyadding = 0;
			return 0;
		}
		for (i = count; i-- > 0 && wxp->w_hosts[i]->hd_hostpart != t; ) ;
		if (i < 0) {
			pvmlogprintf("startack() what? some random tid %x\n", t);
			pkint(wp->wa_mesg, PvmDSysErr);
			sendmessage(wp->wa_mesg);
			wp->wa_mesg = 0;
			wait_delete(wp);
			busyadding = 0;
			PVM_FREE(buf);
			return 0;
		}
		hp = wxp->w_hosts[i];
		ac = sizeof(av)/sizeof(av[0]);
		if (crunchzap(buf, &ac, av) || ac != 5) {
			pvmlogprintf("startack() host %s expected version, got \"%s\"\n",
					hp->hd_name, buf);
			if (!(hp->hd_err = errnamecode(buf)))
				hp->hd_err = PvmCantStart;
			PVM_FREE(buf);
			continue;
		}

		ver = atoi(av[0]);
		if (ver != DDPROTOCOL) {
			pvmlogprintf(
					"slave_exec() host %s d-d protocol mismatch (%d/%d)\n",
					hp->hd_name, ver, DDPROTOCOL);
			hp->hd_err = PvmBadVersion;
			continue;
		}

		hp->hd_arch = STRALLOC(av[1]);
		hex_inadport(av[2], &hp->hd_sad);
		hp->hd_mtu = atoi(av[3]);
		hp->hd_dsig = atoi(av[4]);

		PVM_FREE(buf);
	}

	/*
	* update reply message to add-host requestor
	*/

	mp2 = wp->wa_mesg;
	pkint(mp2, count);
	pkint(mp2, 0);	/* XXX narches = 0 for now */
	for (i = 0; i < count; i++) {
		hp = wxp->w_hosts[i];
		if (hp->hd_err) {
			pkint(mp2, hp->hd_err);
			pkstr(mp2, "");
			pkstr(mp2, "");
			pkint(mp2, 0);
			pkint(mp2, 0);

		} else {
			pkint(mp2, hp->hd_hostpart);
			pkstr(mp2, hp->hd_name);
			pkstr(mp2, hp->hd_arch);
			pkint(mp2, hp->hd_speed);
			pkint(mp2, hp->hd_dsig);
		}
	}

	/*
	* delete broken ones, done now if none succeeded,
	* otherwise done when host table update is complete.
	*/

	for (j = i = 0; i < count; i++)
		if (!wxp->w_hosts[i]->hd_err) {
			hp = wxp->w_hosts[i];
			wxp->w_hosts[i] = 0;
			wxp->w_hosts[j++] = hp;

		} else {
			hd_unref(wxp->w_hosts[i]);
			wxp->w_hosts[i] = 0;
		}
	count = j;

	if (count < 1) {
		busyadding = 0;
		sendmessage(wp->wa_mesg);
		wp->wa_mesg = 0;
		free_waitc_add(wxp);
		wait_delete(wp);
		return 0;
	}

	wp2 = wait_new(WT_HTUPD);
	wp2->wa_dep = wp->wa_dep;
	wp2->wa_mesg = wp->wa_mesg;
	wp->wa_mesg = 0;

	/*
	* make next host table
	*/

	newhosts = ht_new(1);
	newhosts->ht_serial = hosts->ht_serial + 1;
	newhosts->ht_master = hosts->ht_master;
	newhosts->ht_cons = hosts->ht_cons;
	newhosts->ht_local = hosts->ht_local;

	for (i = 0; i < count; i++)
		ht_insert(newhosts, wxp->w_hosts[i]);

	free_waitc_add(wxp);
	wait_delete(wp);
	wp = 0;

	runstate = PVMDHTUPD;

	/*
	* send DM_SLCONF message to each new host
	*/

	for (hh = newhosts->ht_last; hh > 0; hh--)
		if (hp = newhosts->ht_hosts[hh]) {
			mp2 = mesg_new(0);
			mp2->m_tag = DM_SLCONF;
			mp2->m_dst = hp->hd_hostpart | TIDPVMD;
			if (hp->hd_epath) {
				pkint(mp2, DM_SLCONF_EP);
				pkstr(mp2, hp->hd_epath);
			}
			if (hp->hd_bpath) {
				pkint(mp2, DM_SLCONF_BP);
				pkstr(mp2, hp->hd_bpath);
			}
			if (hp->hd_wdir) {
				pkint(mp2, DM_SLCONF_WD);
				pkstr(mp2, hp->hd_wdir);
			}
			if (pvmschedtid) {
				sprintf(buf2, "%x", pvmschedtid);
				pkint(mp2, DM_SLCONF_SCHED);
				pkstr(mp2, buf2);
			}
			if (pvmtracer.trctid || pvmtracer.outtid) {
				sprintf(buf2, "%x %d %d %x %d %d %d %d %s",
					pvmtracer.trctid, pvmtracer.trcctx,
						pvmtracer.trctag,
					pvmtracer.outtid, pvmtracer.outctx,
						pvmtracer.outtag,
					pvmtracer.trcbuf, pvmtracer.trcopt,
					pvmtracer.tmask);
				pkint(mp2, DM_SLCONF_TRACE);
				pkstr(mp2, buf2);
			}
			sendmessage(mp2);
		}

	/*
	* create host table update message containing all current hosts
	* plus new ones, send to each new host.
	*/

	mp2 = mesg_new(0);
	mp2->m_tag = DM_HTUPD;
	pkint(mp2, newhosts->ht_serial);
	pkint(mp2, newhosts->ht_master);
	pkint(mp2, newhosts->ht_cons);
	pkint(mp2, hosts->ht_cnt + newhosts->ht_cnt);
	for (hh = hosts->ht_last; hh > 0; hh--)
		if (hp = hosts->ht_hosts[hh]) {
			pkint(mp2, hh);
			pkstr(mp2, hp->hd_name);
			pkstr(mp2, hp->hd_arch);
			pkstr(mp2, inadport_hex(&hp->hd_sad));
			pkint(mp2, hp->hd_mtu);
			pkint(mp2, hp->hd_speed);
			pkint(mp2, hp->hd_dsig);
		}
	for (hh = newhosts->ht_last; hh > 0; hh--)
		if (hp = newhosts->ht_hosts[hh]) {
			pkint(mp2, hh);
			pkstr(mp2, hp->hd_name);
			pkstr(mp2, hp->hd_arch);
			pkstr(mp2, inadport_hex(&hp->hd_sad));
			pkint(mp2, hp->hd_mtu);
			pkint(mp2, hp->hd_speed);
			pkint(mp2, hp->hd_dsig);
		}

	for (hh = newhosts->ht_last; hh > 0; hh--)
		if (hp = newhosts->ht_hosts[hh]) {
			mp2->m_ref++;
			mp2->m_dst = hp->hd_hostpart | TIDPVMD;
			wp3 = wait_new(WT_HTUPD);
			wp3->wa_dep = wp2->wa_dep;
			wp2->wa_mesg->m_ref++;
			wp3->wa_mesg = wp2->wa_mesg;
			wp3->wa_on = hp->hd_hostpart;
			LISTPUTBEFORE(wp2, wp3, wa_peer, wa_rpeer);
			mp2->m_wid = wp3->wa_wid;
			sendmessage(mp2);
		}
	pmsg_unref(mp2);

	/*
	* create host table update message containing happy new hosts,
	* send to each old host.
	*/

	mp2 = mesg_new(0);
	mp2->m_tag = DM_HTUPD;
	pkint(mp2, newhosts->ht_serial);
	pkint(mp2, newhosts->ht_master);
	pkint(mp2, newhosts->ht_cons);
	pkint(mp2, newhosts->ht_cnt);
	for (hh = newhosts->ht_last; hh > 0; hh--)
		if (hp = newhosts->ht_hosts[hh]) {
			pkint(mp2, hh);
			pkstr(mp2, hp->hd_name);
			pkstr(mp2, hp->hd_arch);
			pkstr(mp2, inadport_hex(&hp->hd_sad));
			pkint(mp2, hp->hd_mtu);
			pkint(mp2, hp->hd_speed);
			pkint(mp2, hp->hd_dsig);
		}

	for (hh = hosts->ht_last; hh > 0; hh--)
		if (hh != hosts->ht_local && (hp = hosts->ht_hosts[hh])) {
			mp2->m_ref++;
			mp2->m_dst = hp->hd_hostpart | TIDPVMD;
			wp3 = wait_new(WT_HTUPD);
			wp3->wa_dep = wp2->wa_dep;
			wp2->wa_mesg->m_ref++;
			wp3->wa_mesg = wp2->wa_mesg;
			wp3->wa_on = hp->hd_hostpart;
			LISTPUTBEFORE(wp2, wp3, wa_peer, wa_rpeer);
			mp2->m_wid = wp3->wa_wid;
			sendmessage(mp2);
		}
	pmsg_unref(mp2);

	/*
	* update our host table
	*/

	gotnewhosts(newhosts, hosts);

	/* XXX returning to normal state right here is a hack, we should
	   XXX wait for all the htupdacks to come back but we need the
	   XXX regular message service, hostfail entry, etc. */

	for (hh = newhosts->ht_last; hh > 0; hh--)
		if ((hp = newhosts->ht_hosts[hh]) && !hp->hd_err)
			ht_insert(hosts, hp);
	hosts->ht_serial = newhosts->ht_serial;

	if (pvmdebmask & PDMHOST) {
		pvmlogerror("startack() committing to new host table:\n");
		ht_dump(hosts);
	}
	runstate = PVMDNORMAL;
	ht_free(newhosts);
	newhosts = 0;

	/* if peered waitcs on htupdack already finished, send the reply */

	if (wp2->wa_peer == wp2) {
		busyadding = 0;
		sendmessage(wp2->wa_mesg);
		wp2->wa_mesg = 0;
	}
	wait_delete(wp2);

	return 0;
}


/*	dm_startack()
*
*	The results come back from the pvmd' hoster to pvmd[master].
*
*	DM_STARTACK(wid_rtn) {
*		int count
*		{
*			int tid
*			string result		// line from slave pvmd or error token
*		} [count]
*	}
*/

int
dm_startack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;		/* wait context on pvmd' */

	if (!(wp = wait_get(hp, mp, WT_HOSTSTART)))
		return 0;

	finack_to_host(hp);

	startack(wp, mp);

	return 0;
}


/*	dm_task()
*
*	(Remote or local) pvmd requests information about local task(s).
*
*	DM_TASK(wid) {
*		int where			// 0 for all, hostpart, or full tid
*	}
*/

int
dm_task(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct task *tp;
	struct pmsg *mp2;
	int where;

	hp = hp;

	if (upkuint(mp, &where)) {
		pvmlogerror("dm_task() bad msg format\n");
		return 0;
	}

#ifdef SHMEM
	mpp_setstatus(0);	/* get status from global info table */
#endif

	/* pack list of local tasks and reply to waiter */

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_TASKACK;
	mp2->m_wid = mp->m_wid;
	if (where & tidlmask) {
		if (tp = task_find(where)) {
			pkint(mp2, tp->t_tid);
			pkint(mp2, tp->t_ptid);
			pkint(mp2, myhostpart);
			pkint(mp2, tp->t_flag);
			pkstr(mp2, tp->t_a_out ? tp->t_a_out : "");
			pkint(mp2, tp->t_pid);
		}

	} else {
		for (tp = locltasks->t_link; tp != locltasks; tp = tp->t_link) {
			pkint(mp2, tp->t_tid);
			pkint(mp2, tp->t_ptid);
			pkint(mp2, myhostpart);
			pkint(mp2, tp->t_flag);
			pkstr(mp2, (tp->t_a_out ? tp->t_a_out : ""));
			pkint(mp2, tp->t_pid);
		}
	}
	sendmessage(mp2);
	return 0;
}


/*	dm_taskack()
*
*	Reply to DM_TASK op.
*
*	DM_TASKACK(wid_rtn) {
*		{
*			int tid
*			int ptid
*			int hostpart
*			int flag
*			string a_out
*			int pid
*		} []				// implied
*	}
*/

int
dm_taskack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;
	struct pmsg *mp2;
	int i;
	char *p;

	if (!(wp = wait_get(hp, mp, WT_TASK)))
		return 0;

	/* append data to waiting message */

	mp2 = wp->wa_mesg;
	while (!upkint(mp, &i)) {
		pkint(mp2, i);			/* tid */
		upkint(mp, &i);			/* ptid */
		pkint(mp2, i);
		upkint(mp, &i);			/* host */
		pkint(mp2, i);
		upkint(mp, &i);			/* flag */
		pkint(mp2, i);
		upkstralloc(mp, &p);	/* a.out name */
		pkstr(mp2, p);
		PVM_FREE(p);
		upkint(mp, &i);			/* pid */
		pkint(mp2, i);
	}

	/* send message if we're the last waiter */

	if (wp->wa_peer == wp) {
		mp2->m_ref++;
		sendmessage(mp2);

	}
	wait_delete(wp);
	return 0;
}


/*	dm_delhost()
*
*	(Master pvmd) gets request to delete hosts.
*
*	DM_DELHOST(wid) {
*		int count
*		string names[count]
*	}
*/

int
dm_delhost(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int count;
	char *buf;
	struct pmsg *mp2;		/* DELHOSTACK message */
	struct pmsg *mp3;		/* HTDEL message */
	struct htab *ht_del;	/* hosts to delete */
	struct htab *ht_save;	/* remaining hosts */
	int hh;

/* XXX danger, this doesn't check if already doing a host add/delete */

	/* sanity check count */

	if (upkint(mp, &count)) {
		pvmlogerror("dm_delhost() bad msg format\n");
		return 0;
	}
	if (count < 1 || count > (tidhmask >> (ffs(tidhmask) - 1))) {
		pvmlogerror("dm_delhost() bad count\n");
		return 0;
	}

	/*
	* read host names from message, generate delete and save sets
	* and a DELHOSTACK reply message with result code for each host.
	* set SHUTDOWN flag for each host in delete set.
	*/

	ht_del = ht_new(1);
	ht_save = ht_new(1);
	ht_merge(ht_save, hosts);

	mp2 = mesg_new(0);
	mp2->m_tag = DM_DELHOSTACK;
	mp2->m_wid = mp->m_wid;
	mp2->m_dst = mp->m_src;

	mp3 = mesg_new(0);
	mp3->m_tag = DM_HTDEL;
	pkint(mp3, hosts->ht_serial);

	pkint(mp2, count);
	while (count-- > 0) {
		upkstralloc(mp, &buf);
		if (hp = nametohost(hosts, buf)) {
			if (tidtohost(ht_del, hp->hd_hostpart)) {
				pkint(mp2, PvmDupHost);

			} else {
				if (hp->hd_hostpart == myhostpart)
					pkint(mp2, PvmBadParam);

				else {
					ht_insert(ht_del, hp);
					ht_delete(ht_save, hp);
					pkint(mp3, hp->hd_hostpart);
					fin_to_host(hp);
					pkint(mp2, 0);
				}
			}

		} else
			pkint(mp2, PvmNoHost);
		PVM_FREE(buf);
	}

	/*
	* send HTDEL message to all hosts in ht_save set except us
	*/

	for (hh = ht_save->ht_last; hh > 0; hh--)
		if (hh != hosts->ht_local && (hp = ht_save->ht_hosts[hh])) {
			mp3->m_ref++;
			mp3->m_dst = hp->hd_hostpart | TIDPVMD;
			sendmessage(mp3);
		}
	pmsg_unref(mp3);

	/* reply to host that requested DELHOST operation */

	sendmessage(mp2);

	ht_free(ht_del);
	ht_free(ht_save);
	return 0;
}


/*	dm_delhostack()
*
*	Reply to DM_DELHOST operation.
*
*	DM_DELHOSTACK(wid_rtn) {
*		int count			// or negative for error
*		int status[count]	// status of each host
*	}
*/

int
dm_delhostack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;

	if (!(wp = wait_get(hp, mp, WT_DELHOST)))
		return 0;

	pmsg_packbody(wp->wa_mesg, mp);
	sendmessage(wp->wa_mesg);
	wp->wa_mesg = 0;
	wait_delete(wp);
	return 0;
}


/*	dm_null()
*
*	No-op message.
*
*	DM_NULL { }
*/

int
dm_null(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	hp = hp;
	mp = mp;
/*
	pvmlogprintf("dm_null() from %s\n", hp->hd_name);
*/
	return 0;
}


/*	dm_taskout()
*
*	Stdout data from a task.
*
*	DM_TASKOUT {
*		int tid
*		int length
*		char data[length]
*	}
*/

int
dm_taskout(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int tid;
	int l;
	char *p, *q, c;
	char buf2[32];
	char buf[4100];		 /* XXX a bit bigger than in pvmd.c`loclstout() */

	hp = hp;

	if (upkuint(mp, &tid) || upkint(mp, &l) || l < 1)
		return 0;

	/* unpack data, leaving room at head of buffer */

	p = buf + 32;
	if (l > sizeof(buf) - (p - buf) - 2)
		l = sizeof(buf) - (p - buf) - 2;
	upkbyte(mp, p, l);

	/* ends with "\n\0" */
	if (p[l - 1] != '\n')
		p[l++] = '\n';
	p[l] = 0;

	sprintf(buf2, "[t%x] ", tid);
	l = strlen(buf2);
	while (*p) {
		for (q = p; *q++ != '\n'; ) ;
		c = *q;
		*q = 0;
		BCOPY(buf2, p - l, l);
		pvmlogerror(p - l);
		*q = c;
		p = q;
	}
	return 0;
}


/*	dm_pstat()
*
*	(Remote or local) pvmd requests status of a local task.
*
*	DM_PSTAT(wid) {
*		int tid
*	}
*/

int
dm_pstat(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int tid;
	struct pmsg *mp2;

	hp = hp;
	upkuint(mp, &tid);
	if (tid == pvmmytid || task_find(tid))
		tid = 0;
	else
		tid = PvmNoTask;
	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_PSTATACK;
	mp2->m_wid = mp->m_wid;
	pkint(mp2, tid);
	sendmessage(mp2);
	return 0;
}


/*	dm_pstatack()
*
*	Reply to DM_PSTAT op.
*
*	DM_PSTATACK(wid_rtn) {
*		int status
*	}
*/

int
dm_pstatack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;

	if (!(wp = wait_get(hp, mp, 0)))
		return 0;

	pmsg_packbody(wp->wa_mesg, mp);
	sendmessage(wp->wa_mesg);
	wp->wa_mesg = 0;
	wait_delete(wp);
	return 0;
}


/*	dm_halt()
*
*	Task has requested whole machine to halt.
*
*	DM_HALT { }
*/

int
dm_halt(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int hh;

	pvmlogprintf("dm_halt() from (%s), halting...\n", hp->hd_name);
	for (hh = hosts->ht_last; hh > 0; hh--) {
		if (hh == hosts->ht_local || !(hp = hosts->ht_hosts[hh]))
			continue;
		finack_to_host(hp);
	}
	runstate = PVMDHALTING;
	return 0;
}


/*	dm_mca()
*
*	Remote pvmd defines a new (single-use) multicast address, to be used
*	by a subsequent message.
*
*	DM_MCA {
*		int gtid			// multicast tid
*		int count			// number of addresses
*		int tids[count]		// addresses
*	}
*/

int
dm_mca(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct mca *mcap;
	int i;

	/* unpack struct mca from message */

	mcap = mca_new();
	upkuint(mp, &mcap->mc_tid);
	upkint(mp, &mcap->mc_ndst);
	mcap->mc_dsts = TALLOC(mcap->mc_ndst, int, "mcad");
	for (i = 0; i < mcap->mc_ndst; i++)
		upkuint(mp, &mcap->mc_dsts[i]);

	/* put on list of mcas of src host */

	LISTPUTBEFORE(hp->hd_mcas, mcap, mc_link, mc_rlink);

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("dm_mca() mca %x from %s\n", mcap->mc_tid, hp->hd_name);
	}
	return 0;
}


/*	dm_notify()
*
*	Remote pvmd requests to be notified on an event.
*
*	DM_NOTIFY(wid) {
*		int what			// event type, currenty only PvmTaskExit
*		int tid				// address
*	}
*/

int
dm_notify(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int what, tid;
	struct waitc *wp;
	struct pmsg *mp2;

	hp = hp;

	upkint(mp, &what);
	upkuint(mp, &tid);
	if (what != PvmTaskExit) {
		pvmlogprintf("dm_notify() what = %d?\n", what);
		return 0;
	}

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_NOTIFYACK;
	mp2->m_wid = mp->m_wid;
	pkint(mp2, tid);

	if (task_find(tid)) {
		wp = wait_new(WT_TASKX);
		wp->wa_on = tid;
		wp->wa_tid = mp->m_src;
		wp->wa_dep = mp->m_wid;
		wp->wa_mesg = mp2;

	} else {
		sendmessage(mp2);
	}
	return 0;
}


/*	dm_notifyack()
*
*	Reply to DM_NOTIFY op.
*
*	DM_NOTIFYACK(wid_rtn) { }
*/

int
dm_notifyack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;

	hp = hp;

	if (!(wp = wait_get((struct hostd*)0, mp, 0)))
		return 0;

	if (wp->wa_tid && wp->wa_mesg) {
		sendmessage(wp->wa_mesg);
		wp->wa_mesg = 0;
	}

	if ( wp->wa_tid == pvmmytid )
		mb_tidy(wp->wa_on);

	wait_delete(wp);
	return 0;
}


/*	dm_db()
*
*	(Remote or local) pvmd requests a database op.
*
*	DM_DB(wid) {
*		int opcode		// TMDB_PUT, TMDB_REMOVE, TMDB_GET, TMDB_NAMES
*		int tid			// owner task (XXX we're just taking its word, hahaha)
*		string name
*		int index
*		int flags
*		msg data		// if TMDB_PUT
*	}
*/

int
dm_db(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int opcode;					/* op requested */
	int tid;
	int req;					/* index requested */
	int flags;
	char *name = 0;				/* class name */
	struct pmsg *mp2 = 0;		/* reply */
	struct pmsg *mp3 = 0;		/* data message */

	struct waitc *wp, *wp2;		/* wait ctx ptrs (notify, recvinfo */
	struct pmsg *mp4 = 0;		/* notify forward message */
	struct hostd *hp2;			/* remote notify host */

	struct pvmmclass *np, *np2;	/* reset pointers */
	struct pvmmentry *ep, *ep2;	/* reset pointers */
	int *noresets;				/* noreset tids */
	int nnr;					/* # of noreset tasks */
	int found;
	int cc;
	int i;
	int notified;

	hp = hp;

	if (upkint(mp, &opcode) || upkint(mp, &tid)
	|| upkstralloc(mp, &name) || upkint(mp, &req) || upkint(mp, &flags))
		goto badformat;

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_DBACK;
	mp2->m_wid = mp->m_wid;

	switch (opcode) {

	case TMDB_PUT:
		mp3 = mesg_new(0);
		if (pmsg_unpack(mp, mp3))
			goto badformat;
		if ((req = mb_insert(tid, name, req, flags, mp3)) < 0)
			pmsg_unref(mp3);
		else {

			/* check for any pending requests for this mbox entry */
			notified = 0;
			for (wp = waitlist->wa_link; wp != waitlist; wp = wp2) {
				wp2 = wp->wa_link;
				if (wp->wa_kind == WT_RECVINFO) {
					ep = (struct pvmmentry *) wp->wa_spec;
					if ( !strcmp( (char *) ep->me_msg, name ) ) {
						cc = mb_lookup(ep->me_tid, (char *) ep->me_msg,
							ep->me_ind, ep->me_flags, &mp3);
						if ( cc != PvmNotFound ) {
							pkint(wp->wa_mesg, cc);
							if (mp3) {
								pmsg_pack(wp->wa_mesg, mp3);
								pmsg_unref(mp3);
							}
							sendmessage(wp->wa_mesg);
							wp->wa_mesg = 0;
							PVM_FREE(ep->me_msg);
							PVM_FREE(ep);
							wait_delete(wp);
						}
					}
				}
				/* check if task needs mbox notify for mb_tidy()... */
				else if (wp->wa_kind == WT_TASKX) {
					if ( wp->wa_on == tid && wp->wa_tid == pvmmytid )
						notified++;
				}
			}

			/* create mbox notify for mb_tidy() cleanup... */
			if ( !notified ) {
				/* dummy notify for clean up */
				wp = wait_new(WT_TASKX);
				wp->wa_on = tid;
				wp->wa_tid = pvmmytid;
				wp->wa_dep = 0;
				wp->wa_mesg = (struct pmsg *) NULL;

				/* pass on to non-master host */
				hp2 = tidtohost(hosts, tid);
				if ( hp2 && hp2->hd_hostpart != myhostpart ) {
					mp4 = mesg_new(0);
					pkint(mp4, PvmTaskExit);
					pkint(mp4, tid);
					mp4->m_dst = hp2->hd_hostpart | TIDPVMD;
					mp4->m_tag = DM_NOTIFY;
					mp4->m_wid = wp->wa_wid;
					sendmessage(mp4);
				}
			}

		}
		pkint(mp2, req);
		break;

	case TMDB_REMOVE:
		req = mb_delete(tid, name, req, flags);
		pkint(mp2, req);
		break;

	case TMDB_GET:
		cc = mb_lookup(tid, name, req, flags, &mp3);
		if ( cc == PvmNotFound && (flags & PvmMboxWaitForInfo) ) {
			ep = me_new(req);
			ep->me_tid = tid;
			ep->me_msg = (struct pmsg *) name;  /* XXX ouch, overload */
			ep->me_flags = flags;

			wp = wait_new(WT_RECVINFO);
			wp->wa_on = tid;
			wp->wa_tid = pvmmytid;
			wp->wa_dep = mp->m_wid;
			wp->wa_mesg = mp2;
			wp->wa_spec = (void *) ep;

			return 0;
		} else {
			pkint(mp2, cc);
			if (mp3) {
				pmsg_pack(mp2, mp3);
				pmsg_unref(mp3);
			}
		}
		break;

	case TMDB_NAMES:
		pkint(mp2, 0);
		req = mb_names(tid, name, mp2);
		break;

	case TMDB_RESET:
		if ( upkint(mp, &nnr) )
			goto badformat;
		noresets = TALLOC( nnr, int, "int" );
		for ( i=0 ; i < nnr ; i++ ) {
			if ( upkint(mp, &(noresets[i])) ) {
				PVM_FREE(noresets);
				goto badformat;
			}
		}
		pkint(mp2, 0);
		for (np = pvmmboxclasses->mc_link; np != pvmmboxclasses;
				np = np2) {
			np2 = np->mc_link;

			/* If name passed in, only wipe mboxes in that class */
			if ( *name == '\0' || !strcmp( np->mc_name, name ) ) {

			for (ep = np->mc_ent->me_link; ep != np->mc_ent; ep = ep2) {
				ep2 = ep->me_link;

				/* If index passed in, only wipe that mbox */
				/* -1 == All Entries */
				if ( req < 0 || req == ep->me_ind ) {

				/* Check for Persistency (that thorn in my "side" :-) */
				if ( ep->me_flags & PvmMboxPersistent ) {

					/* Task Already Gone?  Spank It. */
					if (!(ep->me_tid)) {
						me_free(np, ep);
						if (np2->mc_rlink != np)
							break;
					}
				
					/* Check for No-Reset Task */
					/* (Only if actually killing tasks...) */
					else if ( flags ) {  /* killtasks */
						for ( i=0, found=0 ; i < nnr && !found ; i++ )
							if ( noresets[i] == ep->me_tid )
								found++;

						/* Not a No-Reset, It WILL die soon. */
						/* Wait for cleanup. */
						if ( !found ) {
							wp = wait_new(WT_RESET);
							wp->wa_on = ep->me_tid;
							wp->wa_tid = pvmmytid;
							wp->wa_dep = 0;
							wp->wa_mesg = (struct pmsg *) NULL;

							/* pass on to non-master host */
							hp2 = tidtohost(hosts, ep->me_tid);
							if ( hp2 && hp2->hd_hostpart != myhostpart )
							{
								mp4 = mesg_new(0);
								pkint(mp4, ep->me_tid);
								mp4->m_dst = hp2->hd_hostpart | TIDPVMD;
								mp4->m_tag = DM_RESET;
								mp4->m_wid = wp->wa_wid;
								sendmessage(mp4);
							}
						}
					}
				}
				}
			}
			}
		}
		PVM_FREE(noresets);
		break;

	default:
		goto badformat;
	}

	sendmessage(mp2);
	PVM_FREE(name);
	return 0;

badformat:
	pvmlogerror("dm_db() bad msg format\n");

	if (name)
		PVM_FREE(name);
	if (mp2)
		pmsg_unref(mp2);
	return 0;
}


/*	dm_dback()
*
*	Reply to DM_DB op.
*
*	DM_DBACK(wid_rtn) {
*		int status		// and index
*	if TMDB_PUT, TMDB_REMOVE, TMDB_GET
*		msg data		// if TMDB_GET
*	else if TMDB_NAMES
*		string names[]	// list of names, length implied
*	}
*/

int
dm_dback(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;

	if (!(wp = wait_get(hp, mp, WT_DB)))
		return 0;

	pmsg_packbody(wp->wa_mesg, mp);
	sendmessage(wp->wa_mesg);
	wp->wa_mesg = 0;
	wait_delete(wp);
	return 0;
}


/*	dm_reset()
*
*	Remote pvmd requests to be notified when task is reset.
*		- for mbox cleanup...
*
*	DM_RESET(wid) {
*		int tid				// task id of persistent mbox owner
*	}
*/

int
dm_reset(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int tid;
	struct waitc *wp;
	struct pmsg *mp2;

	hp = hp;

	upkuint(mp, &tid);

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_RESETACK;
	mp2->m_wid = mp->m_wid;
	pkint(mp2, tid);

	if (task_find(tid)) {
		wp = wait_new(WT_RESET);
		wp->wa_on = tid;
		wp->wa_tid = mp->m_src;
		wp->wa_dep = mp->m_wid;
		wp->wa_mesg = mp2;

	} else {
		sendmessage(mp2);
	}

	return 0;
}


/*	dm_resetack()
*
*	Reply to DM_RESET op.
*
*	DM_RESETACK(wid_rtn) { }
*/

int
dm_resetack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;

	hp = hp;

	if (!(wp = wait_get((struct hostd*)0, mp, 0)))
		return 0;

	if (wp->wa_tid && wp->wa_mesg) {
		sendmessage(wp->wa_mesg);
		wp->wa_mesg = 0;
	}

	mb_tidy_reset(wp->wa_on);

	wait_delete(wp);
	return 0;
}


int
dm_htdel(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int serial;
	int tid;

	if (hp != hosts->ht_hosts[hosts->ht_master]) {
		pvmlogprintf("dm_htdel() from t%x (not master)?\n", mp->m_src);
		return 0;
	}
	if (upkint(mp, &serial)) {
		pvmlogerror("dm_htdel() bad format\n");
		return 0;
	}
	if (serial != hosts->ht_serial) {
		pvmlogprintf("dm_htdel() for serial %d, current is %d?\n",
				serial, hosts->ht_serial);
		return 0;
	}
	while (!upkuint(mp, &tid)) {
		if (hp = tidtohost(hosts, tid)) {
			if (pvmdebmask & PDMHOST) {
				pvmlogprintf("dm_htdel() host %s\n", hp->hd_name);
			}
			hostfailentry(hp);
			ht_delete(hosts, hp);
			if (newhosts)
				ht_delete(newhosts, hp);
		}
	}
	return 0;
}


/*	dm_hostsync()
*
*	Request time of day clock sample
*
*	DM_HOSTSYNC(wid) { }
*/

int
dm_hostsync(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct pmsg *mp2;
	struct timeval now;

	mp = mp;
	hp = hp;

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_HOSTSYNCACK;
	mp2->m_wid = mp->m_wid;
	gettimeofday(&now, (struct timezone *)0);
	pkint(mp2, (int)now.tv_sec);
	pkint(mp2, (int)now.tv_usec);
	sendmessage(mp2);
	return 0;
}


/*	dm_hostsyncack()
*
*	Clock sample comes back
*
*	DM_HOSTSYNCACK(wid_rtn) {
*		int sec
*		int usec
*	}
*/

int
dm_hostsyncack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;
	int i;

	if (!(wp = wait_get(hp, mp, WT_HOSTSYNC)))
		return 0;

	pkint(wp->wa_mesg, 0);
	upkuint(mp, &i);
	pkint(wp->wa_mesg, i);
	upkuint(mp, &i);
	pkint(wp->wa_mesg, i);
	sendmessage(wp->wa_mesg);
	wp->wa_mesg = 0;
	wait_delete(wp);
	return 0;
}


struct mca *
mca_new()
{
	struct mca *mcap;

	if (mcap = TALLOC(1, struct mca, "mca")) {
		mcap->mc_link = mcap->mc_rlink = mcap;
		mcap->mc_tid = mcap->mc_ndst = 0;
		mcap->mc_dsts = 0;
	}
	return mcap;
}


void
mca_free(mcap)
	struct mca *mcap;
{
	LISTDELETE(mcap, mc_link, mc_rlink);
	if (mcap->mc_dsts)
		PVM_FREE(mcap->mc_dsts);
	PVM_FREE(mcap);
}


struct pmsg *
mesg_new(master)
	int master;
{
	struct pmsg *mp;

	if (mp = pmsg_new(master)) {
		mp->m_src = pvmmytid;
		pmsg_setenc(mp, 0x10000000);	/* PvmDataDefault */
		(mp->m_codef->enc_init)(mp);
	}
	return mp;
}



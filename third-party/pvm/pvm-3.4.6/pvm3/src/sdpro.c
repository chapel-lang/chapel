
static char rcsid[] =
	"$Id: sdpro.c,v 1.12 2007/07/09 19:59:55 pvmsrc Exp $";

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
 *	sdpro.c
 *
 *	Pvmd entry points for messages from scheduler.
 *
 * $Log: sdpro.c,v $
 * Revision 1.12  2007/07/09 19:59:55  pvmsrc
 * Added missing "char *pvmnametag();" function decl.
 * 	- submitted by David Mathog <mathog@caltech.edu>.
 * (Spanker=kohl)
 *
 * Revision 1.11  2005/08/22 15:13:25  pvmsrc
 * Added #include <pvmtev.h> for #include global.h...  :-Q
 * 	- submitted by Skipper Hartley <charles.l.hartley@bankofamerica.com>
 * (Spanker=kohl)
 *
 * Revision 1.10  2002/04/16 15:06:04  pvmsrc
 * WIN32 Fixes for Multiple Domains.
 * 	- submitted by Jigen Zhou <jigen@icemcfd.com>.
 * (Spanker=kohl)
 *
 * Revision 1.9  2001/02/07 23:15:54  pvmsrc
 * 2nd Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.8  2000/02/16 22:00:29  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.7  1999/07/08 19:00:15  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.6  1998/11/20  20:06:45  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (spanker=sscott)
 *
 * Revision 1.5  1997/06/27  17:32:55  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.4  1997/01/28  19:27:31  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/25  13:58:06  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.2  1996/10/24  21:04:53  pvmsrc
 * Moved #include of "global.h" down below other headers:
 * 	- need to have all of the structures / types declared before
 * 		the globals can be declared...
 *
 * Revision 1.1  1996/09/23  23:44:42  pvmsrc
 * Initial revision
 *
 * Revision 1.4  1995/07/19  21:28:16  manchek
 * use new function pvmnametag instead of [dts]mname
 *
 * Revision 1.3  1994/10/15  19:29:02  manchek
 * cast message tags for comparison as integers
 *
 * Revision 1.2  1994/07/18  19:22:56  manchek
 * ALPHA segfaulted on array subscript
 *
 * Revision 1.1  1994/06/03  20:38:27  manchek
 * Initial revision
 *
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
#include <time.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#endif

#include <stdio.h>

#include <pvm3.h>
#include <pvmproto.h>
#include "pvmalloc.h"
#include "host.h"
#include "pmsg.h"
#include "waitc.h"
#include "task.h"
#include "bfunc.h"
#include <pvmtev.h>
#include "global.h"


extern int pvmdebmask;				/* from pvmd.c */
extern int hostertid;				/* from pvmd.c */
extern int hosterwid;				/* from ddpro.c */
extern int pvmschedtid;				/* from pvmd.c */
extern int taskertid;				/* from pvmd.c */


char *pvmnametag();


/***************
 **  Private  **
 **           **
 ***************/

int sm_bogus();
int sm_exec();
int sm_add();
int sm_handoff();
int sm_sthostack();
int sm_taskx();

int (*smswitch[])() = {
	sm_bogus,
	sm_exec,
	sm_bogus,
	sm_bogus,
	sm_bogus,
	sm_bogus,
	sm_bogus,
	sm_add,
	sm_bogus,
	sm_bogus,
	sm_taskx,
	sm_bogus,
	sm_handoff,
	sm_bogus,
	sm_bogus,
	sm_sthostack,
	sm_bogus,
};


int
schentry(mp)
	struct pmsg *mp;
{
	int c = mp->m_tag;

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("schentry() from t%x tag %s\n", mp->m_src,
				pvmnametag(c, (int *)0));
	}

	if (c < (int)SM_FIRST || c > (int)SM_LAST) {
		pvmlogprintf("schentry() message from t%x with bogus tag %d\n",
				mp->m_src, c);
		goto bail;
	}

	c -= SM_FIRST;
	(smswitch[c])(mp);

bail:
	pmsg_unref(mp);
	return 0;
}


int
sm_bogus(mp)
	struct pmsg *mp;
{
	pvmlogprintf("sm_bogus() message from t%x tag %s ?\n",
			mp->m_src, pvmnametag(mp->m_tag, (int *)0));
	return 0;
}


/*	sm_exec(wid)
*
*	Request the pvmd to start task processes.
*	Just like DM_EXEC but different.
*/

int
sm_exec(mp)
	struct pmsg *mp;
{
	struct pmsg *rmp;

	rmp = mesg_new(0);
	rmp->m_dst = mp->m_src;
	rmp->m_ctx = mp->m_ctx;
	rmp->m_tag = SM_EXECACK;
	rmp->m_wid = mp->m_wid;
	exectasks(mp, rmp, mp->m_src);
	return 0;
}


/*	sm_add()
*
*	Scheduler requests the master pvmd to add hosts.
*	Just like DM_ADD but different.
*
*	SM_ADD(wid) {
*		int count
*		string name[count]
*	}
*/

int
sm_add(mp)
	struct pmsg *mp;
{
	struct pmsg *rmp;

	rmp = mesg_new(0);
	rmp->m_dst = mp->m_src;
	rmp->m_ctx = mp->m_ctx;
	rmp->m_tag = SM_ADDACK;
	rmp->m_wid = mp->m_wid;
	addhosts(mp, rmp);
	return 0;
}


/*	sm_taskx()
*
*	Tasker notifies us that process exited.
*
*	SM_TASKX() {
*		int tid		// of toasted process
*		int status	// exit status
*		int u_sec	// user time used
*		int u_usec
*		int s_sec	// system time used
*		int s_usec
*	}
*/

int
sm_taskx(mp)
	struct pmsg *mp;
{
	int tid;
	struct task *tp;
	int i;

	if (mp->m_src != taskertid) {
		pvmlogprintf("sm_taskx() from t%x (not the tasker)\n", mp->m_src);
		return 0;
	}
	upkuint(mp, &tid);
	if (tp = task_find(tid)) {
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("sm_taskx() t%x\n", tid);
		}
		upkuint(mp, &tp->t_status);
		upkuint(mp, &i);
		tp->t_utime.tv_sec = i;
		upkuint(mp, &i);
		tp->t_utime.tv_usec = i;
		upkuint(mp, &i);
		tp->t_stime.tv_sec = i;
		upkuint(mp, &i);
		tp->t_stime.tv_usec = i;
		task_cleanup(tp);
		task_free(tp);

	} else {
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("sm_taskx() t%x (no such)\n", tid);
		}
	}
	return 0;
}


/*	sm_handoff()
*
*	Current scheduler for this pvmd gets reassigned.
*
*	SM_HANDOFF() {
*		uint newsched		// tid
*	}
*/

int
sm_handoff(mp)
	struct pmsg *mp;
{
	int tid;

	upkuint(mp, &tid);
	if (pvmdebmask & PDMSCHED) {
		pvmlogprintf("sm_handoff() old t%x new t%x\n", pvmschedtid, tid);
	}
	pvmschedtid = tid;
	return 0;
}


/*	sm_sthostack()
*
*	Hoster finishes starting slave pvmds.
*
*	SM_STHOSTACK(wid_rtn) {
*		{
*			int tid
*			string result
*		} []				// implied
*	}
*/

int
sm_sthostack(mp)
	struct pmsg *mp;
{
	struct waitc *wp;		/* wait context on pvmd' */

	if (mp->m_src != hostertid) {
		pvmlogprintf("sm_sthostack() from t%x (not the hoster)\n",
				mp->m_src);
		return 0;
	}

	if (mp->m_wid != hosterwid) {
		pvmlogprintf("sm_sthostack() from t%x bad wid %d wanted %d\n",
				mp->m_src, mp->m_wid, hosterwid);
		return 0;
	}

	if (!(wp = wait_find(hosterwid)))
		return 0;

	hosterwid = 0;

	startack(wp, mp);

	return 0;
}



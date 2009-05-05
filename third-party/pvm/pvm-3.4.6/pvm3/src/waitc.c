
static char rcsid[] =
	"$Id: waitc.c,v 1.11 2005/08/22 15:13:26 pvmsrc Exp $";

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
 *	waitc.c
 *
 *	Wait context descriptors.
 *
 * $Log: waitc.c,v $
 * Revision 1.11  2005/08/22 15:13:26  pvmsrc
 * Added #include <pvmtev.h> for #include global.h...  :-Q
 * 	- submitted by Skipper Hartley <charles.l.hartley@bankofamerica.com>
 * (Spanker=kohl)
 *
 * Revision 1.10  2001/02/07 23:16:00  pvmsrc
 * 2nd Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.9  2000/02/16 22:00:32  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.8  1999/07/08 19:00:18  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.7  1998/11/20  20:04:19  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (Spanker=sscott)
 *
 * Revision 1.6  1997/06/27  17:32:59  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.5  1997/03/06  21:50:24  pvmsrc
 * Yanked out #includes for <netinet/in.h> and <netinet/tcp.h>.
 * 	- dups with lpvm.h #includes...
 *
 * Revision 1.4  1997/01/28  19:27:52  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/25  13:58:10  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.2  1996/10/24  21:18:31  pvmsrc
 * Moved #include "global.h" to end of #include's for typing.
 * Added #include of new "lpvm.h" in place of explicitly extern-ed
 * 	globals...
 *
 * Revision 1.1  1996/09/23  23:44:47  pvmsrc
 * Initial revision
 *
 * Revision 1.6  1995/11/02  16:36:44  manchek
 * added NEEDSENDIAN switch
 *
 * Revision 1.5  1995/07/28  16:04:07  manchek
 * switch endian includes on flag, not arch name
 *
 * Revision 1.4  1995/07/19  21:27:33  manchek
 * use new function pvmnametag instead of [dts]mname
 *
 * Revision 1.3  1994/07/18  19:25:18  manchek
 * wait_get() checks hostpart, ignoring TIDPVMD.
 * added create/free logging
 *
 * Revision 1.2  1994/06/03  20:38:32  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:52  manchek
 * Initial revision
 *
 */

#include <stdio.h>

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

#include <pvm3.h>

#if defined(WIN32) || defined(CYGWIN)
#include "..\xdr\types.h"
#include "..\xdr\xdr.h"
#else
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

#ifndef WIN32
#include <sys/socket.h>
#include <sys/time.h>
#endif

#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#include <pvmproto.h>
#include "pvmalloc.h"
#include "pmsg.h"
#include "listmac.h"
#include "bfunc.h"
#include "lpvm.h"
#include "host.h"
#include "waitc.h"
#include <pvmtev.h>
#include "global.h"


/***************
 **  Globals  **
 **           **
 ***************/

extern void pvmbailout();
char *pvmnametag();


/***************
 **  Private  **
 **           **
 ***************/

static int widbase = 0;
static int widrange = ~0;
static char *waitnames[] = {
	"addhost", "spawn", "hoststart", "task",
	"delhost", "htupd", "pstat", "taskx",
	"mstat", "db", "hostf", "hosta", "hostsync",
	"taskstart",
	"routea", "routed"
};


static char *
waitkind(kind)
	int kind;
{
	if (kind < 1 || kind > sizeof(waitnames)/sizeof(waitnames[0]))
		return "???";
	return waitnames[kind - 1];
}


/*****************
 **  Interface  **
 **             **
 *****************/

/*	wait_init()
*
*	Call this boy before any other wait stuff.
*/

void
wait_init(base, range)
	int base;			/* base value for wait context ids */
	int range;			/* mask for id generation */
{
	if (!waitlist) {
		widbase = base;
		widrange = range;
		waitlist = TALLOC(1, struct waitc, "wait");
		BZERO((char*)waitlist, sizeof(struct waitc));
		waitlist->wa_link = waitlist;
		waitlist->wa_rlink = waitlist;
	}
}


/*	wait_new()
*
*	Create new wait context of given kind with no peers.  Add to
*	active list.
*/

struct waitc *
wait_new(kind)
	int kind;
{
	static int lastwid = 0;		/* last wid assigned */

	int startwid;				/* to detect when we've tried all */
	int wid;
	struct waitc *wp, *wp2;

	/*
	* find a unique wid by incrementing lastwid and stepping through
	* waitlist until we find a vacant slot.
	*/

	if (++lastwid > widrange)
		lastwid = 1;
	startwid = lastwid;
	wp = waitlist;

	while (1) {
		wid = widbase + lastwid;
		while (wp->wa_wid < wid)
			if ((wp = wp->wa_link) == waitlist)
				break;

		if (wp->wa_wid != wid)
			break;

		if (++lastwid > widrange) {
			lastwid = 1;
			wp = waitlist;
		}
		if (lastwid == startwid) {
			pvmlogprintf("wait_new() out of wids\n");
			pvmbailout(0);		/* XXX this is kinda harsh */
		}
	}

	/*
	* initialze new w context and add to list
	*/

	if (!(wp2 = TALLOC(1, struct waitc, "wait"))) {
		pvmlogprintf("wait_new() can't get memory\n");
		pvmbailout(0);
	}
	wp2->wa_wid = wid;
	wp2->wa_kind = kind;
	wp2->wa_peer = wp2->wa_rpeer = wp2;

	wp2->wa_on = wp2->wa_tid = wp2->wa_dep = 0;
	wp2->wa_mesg = 0;
	wp2->wa_count = 0;
	wp2->wa_spec = 0;

	LISTPUTBEFORE(wp, wp2, wa_link, wa_rlink);
	if (pvmdebmask & PDMWAITC) {
		pvmlogprintf("wait_new():\n");
		wait_dump(wp2);
	}
	return wp2;
}


/*	wait_find()
*
*	Find a wait context in active list by id.  Returns pointer
*	or 0 if not found.
*/

struct waitc *
wait_find(wid)
	int wid;
{
	struct waitc *wp;

	for (wp = waitlist->wa_link; wp != waitlist; wp = wp->wa_link)
		if (wp->wa_wid == wid)
			return wp;
	return (struct waitc*)0;
}


/*	wait_delete()
*
*	Remove a wait context from the active list, disassociate from
*	any peers and destroy it.
*/

void
wait_delete(wp)
	struct waitc *wp;
{
	if (pvmdebmask & PDMWAITC) {
		pvmlogprintf("wait_delete():\n");
		wait_dump(wp);
	}
	if (wp->wa_mesg)
		pmsg_unref(wp->wa_mesg);

	if (wp->wa_link) {
		LISTDELETE(wp, wa_link, wa_rlink);
	}
	if (wp->wa_peer) {
		LISTDELETE(wp, wa_peer, wa_rpeer);
	}
	PVM_FREE(wp);
}


/*	wait_get()
*
*	Find the wait context in waitlist for a given message.
*	If hp != 0, check that message came from correct host.
*	If kind != 0, check that message is correct kind.
*/

struct waitc *
wait_get(hp, mp, kind)
	struct hostd *hp;
	struct pmsg *mp;
	int kind;
{
	struct waitc *wp;

	if (!mp->m_wid)
		return 0;
	if (!(wp = wait_find(mp->m_wid))) {
		pvmlogprintf("waitc_get() tag %s from t%x wid %d not found\n",
				pvmnametag(mp->m_tag, (int *)0), mp->m_src, mp->m_wid);
		return 0;
	}
	if (hp && hp->hd_hostpart != (wp->wa_on & TIDHOST)) {
		pvmlogprintf("waitc_get() tag %s from t%x wid %d wrong host\n",
				pvmnametag(mp->m_tag, (int *)0), mp->m_src, mp->m_wid);
		return 0;
	}
	if (kind && kind != wp->wa_kind) {
		pvmlogprintf("waitc_get() tag %s from t%x wid %d wrong kind: %s\n",
				pvmnametag(mp->m_tag, (int *)0), mp->m_src, mp->m_wid,
				waitkind(wp->wa_kind));
		return 0;
	}
	return wp;
}


void
wait_dump(wp)
	struct waitc *wp;
{
	struct waitc *wp2;

	pvmlogprintf("wid %d kind %s on 0x%x tid 0x%x dep %d peer {",
			wp->wa_wid,
			waitkind(wp->wa_kind),
			wp->wa_on,
			wp->wa_tid,
			wp->wa_dep);
	for (wp2 = wp->wa_peer; wp2 != wp; wp2 = wp2->wa_peer)
		pvmlogprintf(" %d", wp2->wa_wid);
	pvmlogprintf(" } cnt %d\n", wp->wa_count);
}


void
wait_dumpall()
{
	struct waitc *wp;

	pvmlogprintf("wait_dumpall()\n");
	for (wp = waitlist->wa_link; wp != waitlist; wp = wp->wa_link)
		wait_dump(wp);
}



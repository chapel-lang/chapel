
static char rcsid[] =
	"$Id: hostc.c,v 1.5 1999/07/08 19:00:29 kohl Exp $";

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
 *	hostc.c
 *
 *	Local host table cache functions.
 *
 * $Log: hostc.c,v $
 * Revision 1.5  1999/07/08 19:00:29  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.4  1998/01/28  19:15:14  pvmsrc
 * Commented out ancient #include <linux/time.h>...
 * (Spanker=kohl)
 *
 * Revision 1.3  1997/08/07  21:58:09  pvmsrc
 * Fixed stupid bugs...
 *
 * Revision 1.2  1997/07/09  13:56:50  pvmsrc
 * Fixed Author Header.
 *
 * Revision 1.1  1996/09/23  22:52:48  pvmsrc
 * Initial revision
 *
 */


#include <stdio.h>
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <sys/types.h>
#include <sys/time.h>
/* #ifdef IMA_LINUX */
/* #include <linux/time.h> */
/* #endif */
#include <pvm3.h>
#include <../src/bfunc.h>
#include <../src/listmac.h>
#include "myalloc.h"
#include "hostc.h"


static struct hostc *curhosts = 0;
static int addtag = -1;
static int deltag = -1;
static int (*addcallback)() = 0;
static int (*delcallback)() = 0;


static struct hostc *
hc_new()
{
	struct hostc *hp;

	if (hp = TALLOC(1, struct hostc, "hd"))
		BZERO(hp, sizeof(struct hostc));
	return hp;
}


static int
hc_init()
{
	if (curhosts = hc_new())
		curhosts->link = curhosts->rlink = curhosts;

	else
		fprintf(stderr, "hc_init() out of memory\n");
	return 0;
}


static struct hostc *
hc_add(hip)
	struct pvmhostinfo *hip;
{
	struct hostc *hp, *hp2;

	/*
	* if we have a delete message tag and it's not this host, notify
	*/

	if (deltag != -1 && hip->hi_tid != pvm_tidtohost(pvm_mytid()))
		pvm_notify(PvmHostDelete, deltag, 1, &hip->hi_tid);

	if (hp = hc_new()) {
		hp->pvmd_tid = hip->hi_tid;
		hp->name = STRALLOC(hip->hi_name);
		hp->alias = STRALLOC(hip->hi_name);
		hp->arch = STRALLOC(hip->hi_arch);
		hp->speed = hip->hi_speed;
		pvm_hostsync(hp->pvmd_tid, (struct timeval *)0, &(hp->delta));
	}

	for (hp2 = curhosts->link; hp2 != curhosts; hp2 = hp2->link)
		if (hp->pvmd_tid < hp2->pvmd_tid)
			break;
	LISTPUTBEFORE(hp2, hp, link, rlink);

	return hp;
}


static int
hc_delete(hp)
	struct hostc *hp;
{
	LISTDELETE(hp, link, rlink);
	if (hp->name)
		MY_FREE(hp->name);
	if (hp->arch)
		MY_FREE(hp->arch);
	if (hp->alias)
		MY_FREE(hp->alias);
	MY_FREE(hp);
	return 0;
}


static struct hostc *
hc_find(tid)
{
	struct hostc *hp;

	for (hp = curhosts->link; hp != curhosts; hp = hp->link)
		if (tid <= hp->pvmd_tid)
			break;
	return (tid == hp->pvmd_tid) ? hp : (struct hostc *)0;
}


/*	host_init()
*
*	Initialize host cache.  Called once at beginning of time.
*	Synchronize to current configuration.
*	Specify message tags to be used for HostAdd and HostDelete notify.
*/

host_init(atag, dtag, acb, dcb)
	int atag;			/* message tag to use for HostAdd notify or -1 */
	int dtag;			/* tag to use for HostDelete notify or -1 */
	int (*acb)();		/* callback for each host added */
	int (*dcb)();		/* callback for each host deleted */
{
	struct pvmhostinfo *hip;
	int nh;

	addtag = atag;
	deltag = dtag;
	addcallback = acb;
	delcallback = dcb;

	hc_init();

	if (addtag != -1)
		pvm_notify(PvmHostAdd, addtag, -1, (int *)0);

	if (!pvm_config(&nh, (int *)0, &hip)) {
		while (nh > 0) {
			nh--;
			hc_add(&hip[nh]);
		}
	}
	return 0;
}


/*	host_add()
*
*	Called when a HostAdd notify message has been received.
*	The message is in the current receive buffer.
*/

host_add()
{
	int d = 0;
	int n;
	int *dtids;
	int i, j;
	struct pvmhostinfo *hip;
	int nh;

	pvm_upkint(&n, 1, 1);
	if (n < 1 || n > 4096)
		return 0;
	dtids = TALLOC(n, int, "dtid");
	pvm_upkint(dtids, n, 1);
	pvm_freebuf(pvm_getrbuf());
	if (!pvm_config(&nh, (int *)0, &hip)) {
		for (j = n; j-- > 0; )
			for (i = nh; i-- > 0; ) {
				if (dtids[j] == hip[i].hi_tid) {
					hc_add(&hip[i]);
					if (addcallback)
						addcallback(dtids[j]);
					d++;
					break;
				}
			}
	}
	MY_FREE(dtids);
	return d;
}


/*	host_delete()
*
*	Called when a HostDelete notify message has been received.
*	The message is in the current receive buffer.
*/

host_delete()
{
	int tid;
	struct hostc *hp;
	int d = 0;

	pvm_upkint(&tid, 1, 1);
	if (tid == pvm_tidtohost(tid) && (hp = hc_find(tid))) {
		if (delcallback)
			delcallback(tid);
		hc_delete(hp);
		d++;
	}
	return d;
}


struct hostc *
host_findtid(tid)
	int tid;
{
	return hc_find(tid);
}


struct hostc *
host_findname(name)
	char *name;
{
	struct hostc *hp;

	for (hp = curhosts->link; hp != curhosts; hp = hp->link)
		if (!strcmp(name, hp->name))
			return hp;
	return (struct hostc *)0;
}


struct hostc *
host_next(hp)
	struct hostc *hp;
{
	hp = hp ? hp->link : curhosts->link;
	return (hp == curhosts) ? 0 : hp;
}


int
host_show()
{
	struct hostc *hp;
	int nh = 0;
	int na = 0;
	int a;
	long mask = 0;

	for (hp = 0; hp = host_next(hp); ) {
		nh++;
		a = pvm_archcode(hp->arch);
		if (!(mask & (1 << a))) {
			na++;
			mask |= (1 << a);
		}
	}
	printf("Configuration: %d host%s, %d data format%s\n",
			nh, (nh == 1 ? "" : "s"),
			na, (na == 1 ? "" : "s"));
	printf("     TID       ARCH   SPEED HOSTNAME\n");
	for (hp = 0; hp = host_next(hp); ) {
		printf("%8x %10s %7d %-24s\n",
				hp->pvmd_tid,
				hp->arch,
				hp->speed,
				hp->name);
	}
	return 0;
}



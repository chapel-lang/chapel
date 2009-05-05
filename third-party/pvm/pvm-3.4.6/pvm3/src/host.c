
static char rcsid[] =
	"$Id: host.c,v 1.22 2005/08/22 15:13:17 pvmsrc Exp $";

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
 *	host.c
 *
 *	Host table functions.
 *
 * $Log: host.c,v $
 * Revision 1.22  2005/08/22 15:13:17  pvmsrc
 * Added #include <pvmtev.h> for #include global.h...  :-Q
 * 	- submitted by Skipper Hartley <charles.l.hartley@bankofamerica.com>
 * (Spanker=kohl)
 *
 * Revision 1.21  2004/01/14 18:50:55  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.20  2002/10/07 19:01:47  pvmsrc
 * Bug in host table dumping - vmid can be NULL...
 * 	- fix submitted by Kasidit Chanchio <chanchiok@ornl.gov>.
 * (Spanker=kohl)
 *
 * Revision 1.19  2001/09/26 23:35:18  pvmsrc
 * Added new hd_vmid to hostd struct.
 * 	- use to override local PVM_VMID settings with hostfile "id=" option
 * (Spanker=kohl)
 *
 * Revision 1.18  2001/02/07 23:14:03  pvmsrc
 * First Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.17  2000/07/12 19:24:09  pvmsrc
 * Goofy fix for FreeBSD - variation on ifreq struct.
 * 	- submitted via Frederik Meerwaldt <frederik@freddym.org>.
 * (Spanker=kohl)
 *
 * Revision 1.16  2000/02/16 21:59:39  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.15  1999/07/08 18:59:52  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.14  1998/11/20  20:03:57  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (Spanker=sscott)
 *
 * Revision 1.13  1998/06/26  12:31:07  pvmsrc
 * Added IMA_FREEBSD to the ioctl() list of arches that have both
 * 	SIOCGIFCONF and OSIOCGIFCONF defined...
 * 	- need to force use of SIOCGIFCONF anyway for these bozos...
 * (Spanker=kohl)
 *
 * Revision 1.12  1998/02/23  22:51:27  pvmsrc
 * Added AIX4SP2 stuff.
 * (Spanker=kohl)
 *
 * Revision 1.11  1997/12/05  16:23:37  pvmsrc
 * Fixed setting of host table "ht_narch" field.
 * 	- used by pvm_config() to return # of archs.
 * 	- replaced old pvm_archcode() solution with new data signature
 * 		stuff (hd_dsig field in host descriptor).
 * (Spanker=kohl)
 *
 * Revision 1.10  1997/06/27  17:32:21  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.9  1997/06/24  20:39:16  pvmsrc
 * Eliminated unnecessary global externs.
 *
 * Revision 1.8  1997/04/30  21:25:56  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.7  1997/04/09  14:38:20  pvmsrc
 * PVM patches from the base 3.3.10 to 3.3.11 versions where applicable.
 * Originals by Bob Manchek. Altered by Graham Fagg where required.
 *
 * Revision 1.6  1997/01/28  19:26:17  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.5  1996/10/25  14:04:44  pvmsrc
 * changed ifdef around ioctl for AIX 3.x/4.x to use IMA_arch-class
 *
 * Revision 1.4  1996/10/25  13:57:17  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.3  1996/10/24  21:04:45  pvmsrc
 * Moved #include of "global.h" down below other headers:
 * 	- need to have all of the structures / types declared before
 * 		the globals can be declared...
 *
 * Revision 1.2  1996/10/24  18:14:07  pvmsrc
 * ifdefs to ioctl call parameter for AIX3.x/4.x difference
 *
 * Revision 1.1  1996/09/23  23:44:08  pvmsrc
 * Initial revision
 *
 * Revision 1.10  1996/05/13  20:25:48  manchek/GEF
 * missing pvmlogerror in readhostfile
 *
 * Revision 1.9  1995/11/02  16:01:49  manchek
 * use NET_IF_IN_SYS if include/sys/net/if.h
 *
 * Revision 1.8  1995/07/28  16:51:30  manchek
 * ifdef for UTS2
 *
 * Revision 1.7  1995/05/17  16:09:13  manchek
 * added HF_OVERLOAD and '$' syntax
 *
 * Revision 1.6  1994/10/15  19:05:39  manchek
 * must use PVM_FREE instead of free in iflist()
 *
 * Revision 1.5  1994/06/21  18:30:55  manchek
 * HF_SPEED flag now determines whether hd_speed has been set
 *
 * Revision 1.4  1994/06/03  20:38:14  manchek
 * version 3.3.0
 *
 * Revision 1.3  1993/11/30  19:52:05  manchek
 * readhostfile() puts defaults in ht_hosts[0]
 *
 * Revision 1.2  1993/09/15  15:03:31  manchek
 * defined max() if not already there
 *
 * Revision 1.1  1993/08/30  23:26:47  manchek
 * Initial revision
 *
 */

#ifndef WIN32
#include <sys/param.h>
#endif

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

#ifdef WIN32
#include "pvmwin.h"
#else
#ifndef IMA_TITN
#include <sys/ioctl.h>
#else
#include <sys/43ioctl.h>
#endif

#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#ifdef	NET_IF_IN_SYS
#include <sys/net/if.h>
#else
#include <net/if.h>
#endif
#include <netdb.h>
#endif

#include <stdio.h>

#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <ctype.h>
#include <pvmproto.h>
#include "pvmalloc.h"
#include "host.h"
#include "pmsg.h"
#include "pkt.h"
#include "bfunc.h"
#include <pvmtev.h>
#include "global.h"

#ifndef	max
#define	max(a,b)	((a)>(b)?(a):(b))
#endif

#ifdef SOCKADHASLEN
#define SIZ(p) max(sizeof(p), (p).sa_len)
#else
#define SIZ(p) sizeof(p)
#endif


char *inadport_decimal();


/***************
 **  Globals  **
 **           **
 ***************/

extern int pvmdebmask;				/* from pvmd.c */
extern int tidhmask;				/* from pvmd.c */


/***************
 **  Private  **
 **           **
 ***************/


/*****************
 **  Interface  **
 **             **
 *****************/

/*	hd_new()
*
*	Make a new host descriptor.
*/

struct hostd *
hd_new(hh)
	int hh;		/* index in host table */
{
	struct hostd *hp;

	if (hp = TALLOC(1, struct hostd, "hd")) {
		BZERO((char*)hp, sizeof(struct hostd));
		hp->hd_ref = 1;
		hp->hd_hostpart = hh << (ffs(tidhmask) - 1);
		hp->hd_sad.sin_family = AF_INET;
		hp->hd_txq = pk_new(0);
		hp->hd_opq = pk_new(0);
		hp->hd_rxq = pk_new(0);
		hp->hd_txseq = 1;
		hp->hd_rxseq = 1;
		hp->hd_speed = 1000;
		hp->hd_rtt.tv_sec = 1;	/* XXX const */
		hp->hd_mcas = mca_new();
	}
	return hp;
}


/*	hd_free()
*
*	Free a host descriptor and all associated storage.
*	Probably want to use hd_unref() instead.
*/

void
hd_free(hp)
	struct hostd *hp;
{
	struct mca *mcap;

	if (hp->hd_name)
		PVM_FREE(hp->hd_name);
	if (hp->hd_arch)
		PVM_FREE(hp->hd_arch);
	if (hp->hd_login)
		PVM_FREE(hp->hd_login);
	if (hp->hd_dpath)
		PVM_FREE(hp->hd_dpath);
	if (hp->hd_epath)
		PVM_FREE(hp->hd_epath);
	if (hp->hd_bpath)
		PVM_FREE(hp->hd_bpath);
	if (hp->hd_wdir)
		PVM_FREE(hp->hd_wdir);
	if (hp->hd_sopts)
		PVM_FREE(hp->hd_sopts);
	if (hp->hd_txq)
		pk_free(hp->hd_txq);
	if (hp->hd_opq)
		pk_free(hp->hd_opq);
	if (hp->hd_rxq)
		pk_free(hp->hd_rxq);
	if (hp->hd_rxm)
		pmsg_unref(hp->hd_rxm);
	if (mcap = hp->hd_mcas) {
		while (mcap->mc_link != mcap)
			mca_free(mcap->mc_link);
		mca_free(mcap);
	}
	if (hp->hd_aname)
		PVM_FREE(hp->hd_aname);
	if (hp->hd_vmid)
		PVM_FREE(hp->hd_vmid);
	PVM_FREE(hp);
}


/*	hd_unref()
*
*	Decrement the refcount of a host descriptor.  Free it if count
*	reaches zero.
*/

void
hd_unref(hp)
	struct hostd *hp;
{
	if (--hp->hd_ref < 1)
		hd_free(hp);
}


void
hd_dump(hp)
	struct hostd *hp;
{
	int n;
	struct pkt *pp, *pp2;

	pvmlogprintf(
			" hd_dump() ref %d t 0x%x n \"%s\" a \"%s\" ar \"%s\" dsig 0x%x\n",
			hp->hd_ref,
			hp->hd_hostpart,
			(hp->hd_name ? hp->hd_name : ""),
			(hp->hd_aname ? hp->hd_aname : ""),
			(hp->hd_arch ? hp->hd_arch : ""),
			hp->hd_dsig);

	pvmlogprintf(
			"           lo \"%s\" so \"%s\" dx \"%s\" ep \"%s\" bx \"%s\" wd \"%s\" sp %d\n",
			(hp->hd_login ? hp->hd_login : ""),
			(hp->hd_sopts ? hp->hd_sopts : ""),
			(hp->hd_dpath ? hp->hd_dpath : ""),
			(hp->hd_epath ? hp->hd_epath : ""),
			(hp->hd_bpath ? hp->hd_bpath : ""),
			(hp->hd_wdir ? hp->hd_wdir : ""),
			hp->hd_speed);

	n = 0;
	if (pp2 = hp->hd_txq)
		for (pp = pp2->pk_link; pp != pp2; pp = pp->pk_link)
			n++;
	pvmlogprintf("           sa %s mtu %d f 0x%x e %d txq %d\n",
			inadport_decimal(&hp->hd_sad),
			hp->hd_mtu,
			hp->hd_flag,
			hp->hd_err,
			n);

	pvmlogprintf(
			"           tx %d rx %d rtt %d.%06d id \"%s\"\n",
			hp->hd_txseq, hp->hd_rxseq,
			hp->hd_rtt.tv_sec, hp->hd_rtt.tv_usec,
			(hp->hd_vmid ? hp->hd_vmid : ""));
}


/*	nametohost()
*
*	Get a host descriptor by hostname.
*/

struct hostd *
nametohost(htp, name)
	struct htab *htp;
	char *name;
{
	int hh;
	struct hostd *hp;

	if (!strcmp(name, "."))
		return htp->ht_hosts[htp->ht_local];

	for (hh = htp->ht_last; hh > 0; hh--)
		if ((hp = htp->ht_hosts[hh]) && !strcmp(name, hp->hd_name))
			return hp;
	return (struct hostd*)0;
}


/*	indtohost()
*
*	Get a host descriptor by its host table index.
*/

struct hostd *
indtohost(htp, n)
	struct htab *htp;
	int n;
{
	return (n >= 1 && n <= htp->ht_last) ? htp->ht_hosts[n] : 0;
}


/*	tidtohost()
*
*	Get a host descriptor by its tid.
*/

struct hostd *
tidtohost(htp, tid)
	struct htab *htp;
	int tid;
{
	return indtohost(htp, (tid & tidhmask) >> (ffs(tidhmask) - 1));
}


/*	ht_free()
*
*	Free a host table and unref all hosts in it.
*/

void
ht_free(htp)
	struct htab *htp;
{
	int i;

	for (i = 1; i <= htp->ht_last; i++)
		if (htp->ht_hosts[i])
			hd_unref(htp->ht_hosts[i]);
	PVM_FREE(htp->ht_hosts);
	PVM_FREE(htp);
}


/*	ht_new()
*
*	Make a new (empty) host table.  Length is advisory as ht_hosts
*	is extended as needed.
*/

struct htab *
ht_new(siz)
	int siz;		/* initial length of ht_hosts[] */
{
	struct htab *htp;

	if (siz < 1)
		siz = 1;
	htp = TALLOC(1, struct htab, "ht1");
	BZERO((char*)htp, sizeof(struct htab));
	htp->ht_last = siz;
	htp->ht_hosts = TALLOC(siz + 1, struct hostd*, "ht2");
	BZERO((char*)htp->ht_hosts, (siz + 1) * sizeof(struct hostd*));
	return htp;
}


/*	ht_insert()
*
*	Add a host to a host table.  Extend ht_hosts if necessary.
*	Update ht_narch.
*/

void
ht_insert(htp, hp)
	struct htab *htp;
	struct hostd *hp;
{
	int hh;
	int i;
	unsigned long mask = 0, tmpmask;

	int *dsigs;
	int ndsigs;
	int found;
	int d;

	hh = (hp->hd_hostpart & tidhmask) >> (ffs(tidhmask) - 1);

	/* extend ht_hosts[] if no room */

	if (hh > htp->ht_last) {
		int n = htp->ht_last;

		htp->ht_last = (hh * 3) / 2;
		htp->ht_hosts = TREALLOC(htp->ht_hosts, htp->ht_last + 1, struct hostd*);
		while (++n <= htp->ht_last)
			htp->ht_hosts[n] = 0;
	}

	/* if already have an entry, take this as an update XXX kind of a hack */

	if (htp->ht_hosts[hh]) {	/* already have an entry */

		struct hostd *hp2 = htp->ht_hosts[hh];

		if (hp->hd_name) {
			if (hp2->hd_name)
				PVM_FREE(hp2->hd_name);
			hp2->hd_name = STRALLOC(hp->hd_name);
		}
		if (hp->hd_arch) {
			if (hp2->hd_arch)
				PVM_FREE(hp2->hd_arch);
			hp2->hd_arch = STRALLOC(hp->hd_arch);
		}
		hp2->hd_dsig = hp->hd_dsig;
		hp2->hd_mtu = hp->hd_mtu;
		hp2->hd_sad = hp->hd_sad;
		hp2->hd_speed = hp->hd_speed;

	} else {					/* add new entry */

		htp->ht_hosts[hh] = hp;
		if (hh)
			htp->ht_cnt++;
		hp->hd_ref++;
	}

	/* update number of arches */

	dsigs = TALLOC( htp->ht_cnt, int, "dsigs");
	ndsigs = 0;

	htp->ht_narch = 0;
	for (hh = htp->ht_last; hh > 0; hh--) {
		hp = htp->ht_hosts[hh];
		if (hp && hp->hd_arch) {
			d = htp->ht_hosts[hh]->hd_dsig;
			for ( i=0, found=0 ; i < ndsigs && !found ; i++ ) {
				if ( dsigs[i] == d )
					found++;
			}
			if ( !found ) {
				dsigs[ ndsigs++ ] = d;
				htp->ht_narch++;
			}
		}
	}

	PVM_FREE( dsigs );
}


/*	ht_delete()
*
*	Remove a host from a host table and unreference it.
*/

void
ht_delete(htp, hp)
	struct htab *htp;
	struct hostd *hp;
{
	int hh;
	int i;
	unsigned long mask = 0, tmpmask;

	int *dsigs;
	int ndsigs;
	int found;
	int d;

	hh = (hp->hd_hostpart & tidhmask) >> (ffs(tidhmask) - 1);
	if (hh < 0 || hh > htp->ht_last || htp->ht_hosts[hh] != hp) {
		pvmlogerror("ht_delete() host not in table\n");
		return;
	}
	htp->ht_hosts[hh] = 0;
	if (hh)
		htp->ht_cnt--;

	hd_unref(hp);

	/* update number of arches */

	dsigs = TALLOC( htp->ht_cnt, int, "dsigs");
	ndsigs = 0;

	htp->ht_narch = 0;
	for (hh = htp->ht_last; hh > 0; hh--) {
		hp = htp->ht_hosts[hh];
		if (hp && hp->hd_arch) {
			d = htp->ht_hosts[hh]->hd_dsig;
			for ( i=0, found=0 ; i < ndsigs && !found ; i++ ) {
				if ( dsigs[i] == d )
					found++;
			}
			if ( !found ) {
				dsigs[ ndsigs++ ] = d;
				htp->ht_narch++;
			}
		}
	}

	PVM_FREE( dsigs );
}


void
ht_dump(htp)
	struct htab *htp;
{
	int hh;

	pvmlogprintf(
"ht_dump() ser %d last %d cnt %d master %d cons %d local %d narch %d\n",
			htp->ht_serial, htp->ht_last, htp->ht_cnt, htp->ht_master,
			htp->ht_cons, htp->ht_local, htp->ht_narch);
	for (hh = 0; hh <= htp->ht_last; hh++)
		if (htp->ht_hosts[hh])
			hd_dump(htp->ht_hosts[hh]);
}


/*	ht_merge()
*
*	Add entries in src host table to dst host table.
*/

int
ht_merge(dst, src)
	struct htab *dst, *src;
{
	int hh;
	struct hostd *hp;

	for (hh = src->ht_last; hh > 0; hh--)
		if (hp = src->ht_hosts[hh])
			ht_insert(dst, hp);
	return 0;
}


/*	applydefaults()
*
*	Apply default settings to unspecified fields of hostd.
*/

int
applydefaults(hp, defhp)
	struct hostd *hp;		/* hostd to modify */
	struct hostd *defhp;	/* default settings */
{
	if (!hp->hd_login && defhp->hd_login)
		hp->hd_login = STRALLOC(defhp->hd_login);

	if (!hp->hd_dpath && defhp->hd_dpath)
		hp->hd_dpath = STRALLOC(defhp->hd_dpath);

	if (!hp->hd_epath && defhp->hd_epath)
		hp->hd_epath = STRALLOC(defhp->hd_epath);

	if (!hp->hd_bpath && defhp->hd_bpath)
		hp->hd_bpath = STRALLOC(defhp->hd_bpath);

	if (!hp->hd_wdir && defhp->hd_wdir)
		hp->hd_wdir = STRALLOC(defhp->hd_wdir);

	if (!hp->hd_sopts && defhp->hd_sopts)
		hp->hd_sopts = STRALLOC(defhp->hd_sopts);

	if (!hp->hd_aname && defhp->hd_aname)
		hp->hd_aname = STRALLOC(defhp->hd_aname);

	if (!(hp->hd_flag & HF_SPEED) && (defhp->hd_flag & HF_SPEED))
		hp->hd_speed = defhp->hd_speed;

	if (!hp->hd_vmid && defhp->hd_vmid)
		hp->hd_vmid = STRALLOC(defhp->hd_vmid);

	hp->hd_flag |= defhp->hd_flag;

	return 0;
}


/*	parsehost()
*
*	Parse hostfile line into hostd.
*/

int
parsehost(buf, hp)
	char *buf;
	struct hostd *hp;
{
	char *av[10];		/* parsed words */
	int ac;
	int err = 0;

	ac = sizeof(av)/sizeof(av[0]);
	if (acav(buf, &ac, av)) {
		pvmlogprintf("parsehost(): line too long\n");
		goto bad;
	}
	if (!ac)
		goto bad;

	/* add options to host descriptor */

	while (--ac > 0) {
		if (!strncmp(av[ac], "lo=", 3)) {
			if (hp->hd_login)
				PVM_FREE(hp->hd_login);
			hp->hd_login = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "dx=", 3)) {
			if (hp->hd_dpath)
				PVM_FREE(hp->hd_dpath);
			hp->hd_dpath = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "ep=", 3)) {
			if (hp->hd_epath)
				PVM_FREE(hp->hd_epath);
			hp->hd_epath = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "sp=", 3)) {
			hp->hd_speed = atoi(av[ac] + 3);
			hp->hd_flag |= HF_SPEED;
			continue;
		}
		if (!strncmp(av[ac], "bx=", 3)) {
			if (hp->hd_bpath)
				PVM_FREE(hp->hd_bpath);
			hp->hd_bpath = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "wd=", 3)) {
			if (hp->hd_wdir)
				PVM_FREE(hp->hd_wdir);
			hp->hd_wdir = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "so=", 3)) {
			if (hp->hd_sopts)
				PVM_FREE(hp->hd_sopts);
			hp->hd_sopts = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "ip=", 3)) {
			if (hp->hd_aname)
				PVM_FREE(hp->hd_aname);
			hp->hd_aname = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "id=", 3)) {
			if (hp->hd_vmid)
				PVM_FREE(hp->hd_vmid);
			hp->hd_vmid = STRALLOC(av[ac] + 3);
			continue;
		}
		pvmlogprintf("parsehost(): unknown option \"%s\"\n", av[ac]);
		err++;
	}
	if (err)
		goto bad;

	if (hp->hd_name)
		PVM_FREE(hp->hd_name);
	hp->hd_name = STRALLOC(av[0]);
	return 0;

bad:
	return -1;
}


/*	readhostfile()
*
*	Read a host file and return host table with options and ipaddrs
*	filled in.
*/

struct htab *
readhostfile(fn)
	char *fn;
{
	struct htab *htp;
	struct hostd *hp;
	FILE *ff = 0;
	char buf[512];		/* line buffer */
	int lnum = 0;		/* line counter */
	int err = 0;		/* error count */
	struct hostent *he;
	char *p;
	struct in_addr *my_in_addrs;
	int num_addrs;
	int i;
	int maxhostid = tidhmask >> (ffs(tidhmask) - 1);
	struct hostd *defaults = 0;

	htp = ht_new(1);

	/*
	* get list of this host's interfaces so we can detect ourself in file
	*/

	if (iflist(&my_in_addrs, &num_addrs) == -1 || num_addrs < 1) {
		pvmlogprintf("readhostfile() iflist failed\n");
		goto bail;
	}

	if (!(ff = fopen(fn, "r"))) {
		pvmlogprintf("readhostfile() %s: can't read\n", fn);
		goto bail;
	}

	/* parse each line of host file */

	while (fgets(buf, sizeof(buf), ff)) {
		lnum++;
		for (p = buf; *p && isspace(*p); p++);

		if (!*p || *p == '#')	/* leading '#' is comment */
			continue;

		if (lnum > maxhostid) {
			pvmlogprintf("readhostfile() %s: too many lines\n", fn);
			err++;
			break;
		}
		hp = hd_new(lnum);

		while (*p == '&' || *p == '$') {
			if (*p == '&')		/* leading '&' is no-start */
				hp->hd_flag |= HF_NOSTART;
			if (*p == '$')		/* leading '$' allows overloading host */
				hp->hd_flag |= HF_OVERLOAD;
			p++;
		}

		if (parsehost(p, hp)) {
			pvmlogprintf("readhostfile() %s %d: errors\n", fn, lnum);
			err++;
			goto badline;
		}

	/*
	* if host == "*", change the default options
	* instead of adding a new host to the list
	*/

		if (!strcmp(hp->hd_name, "*")) {
			if (defaults)
				hd_unref(defaults);
			defaults = hp;
			continue;
		}

	/* Set unspecified fields of hp to defaults */

		if (defaults)
			applydefaults(hp, defaults);

	/* look up ip addr */

		if (!(he = gethostbyname(hp->hd_aname ? hp->hd_aname : hp->hd_name))) {
			pvmlogprintf("readhostfile() %s %d: %s: can't gethostbyname\n",
				fn, lnum, hp->hd_name);
			err++;
			goto badline;
		}
		BCOPY(he->h_addr_list[0], (char*)&hp->hd_sad.sin_addr,
			sizeof(struct in_addr));

	/* mark master host to not start */

		if (!(hp->hd_flag & HF_OVERLOAD))
			for (i = num_addrs; i-- > 0; ) {
				if (BCMP((char*)&my_in_addrs[i], (char*)&hp->hd_sad.sin_addr,
						sizeof(struct in_addr)) == 0) {
					hp->hd_flag |= HF_NOSTART;
					break;
				}
			}

		ht_insert(htp, hp);
		hd_unref(hp);
		continue;

badline:
		hd_unref(hp);
	}
	if (err) {
		pvmlogprintf("readhostfile() %s: %d errors in hostfile\n", fn, err);
	}

	if (defaults) {
		defaults->hd_hostpart = 0;
		ht_insert(htp, defaults);
	}
	fclose(ff);
	return htp;

bail:
	if (defaults)
		hd_unref(defaults);
	if (ff)
		fclose(ff);
	ht_free(htp);
	return (struct htab*)0;
}


/*	iflist()
*
*	Return list of addresses for active network interfaces.
*/

#ifdef	SIOCGIFCONF

int
iflist(alp, np)
	struct in_addr **alp;	/* return list of addresses */
	int *np;				/* return len of alp */
{
	int soc = -1;						/* socket */
	static struct in_addr *iplist = 0;	/* list of interface addrs found */
	int nip = 0;						/* length of iplist */
	char buf[4096];						/* return space for SIOCGIOCONF */
	struct ifconf sif;
	struct ifreq *reqp;
	struct ifreq req;
	char *cp;

	if (iplist)
		PVM_FREE(iplist);
	iplist = TALLOC(10, struct in_addr, "ifl");

	if ((soc = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		goto bail;
	}

	sif.ifc_ifcu.ifcu_buf = buf;
	sif.ifc_len = sizeof(buf);
	if (ioctl(soc,
#ifdef OSIOCGIFCONF
#if defined(IMA_RS6K) || defined(IMA_RS6KMP) || defined(IMA_SP2MPI) \
			|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
			|| defined(IMA_FREEBSD)
		SIOCGIFCONF
#else
		OSIOCGIFCONF
#endif
#else
		SIOCGIFCONF
#endif
		, &sif) == -1) {
		perror("ioctl");
		goto bail;
	}

	for (cp = sif.ifc_ifcu.ifcu_buf;
			cp - sif.ifc_ifcu.ifcu_buf < sif.ifc_len;
			cp += sizeof(*reqp) - sizeof(struct sockaddr) + SIZ(reqp->ifr_addr))
	{
		reqp = (struct ifreq*)cp;
		if (reqp->ifr_addr.sa_family != AF_INET)
			continue;
		BCOPY(reqp->ifr_name, req.ifr_name, sizeof(req.ifr_name));
		if (ioctl(soc, SIOCGIFFLAGS, &req) == -1) {
			perror("ioctl");
			goto bail;
		}
		/* On some FreeBSD systems: */
		/* if (IFF_UP & req.ifr_flags) { */
		if (IFF_UP & req.ifr_ifru.ifru_flags) {
			if (nip > 0 && !(nip % 10))
				iplist = TREALLOC(iplist, (nip + 10), struct in_addr);
			iplist[nip++] =
			((struct sockaddr_in*)(&reqp->ifr_ifru.ifru_addr))->sin_addr;
			if (pvmdebmask & PDMNET) {
				long a;

				a = ((struct sockaddr_in*)(&reqp->ifr_ifru.ifru_addr))->sin_addr.s_addr;
				a = ntohl(a);
				pvmlogprintf("iflist() %s %d.%d.%d.%d\n",
						reqp->ifr_name,
						0xff & (a >> 24),
						0xff & (a >> 16),
						0xff & (a >> 8),
						0xff & a);
			}
		}
	}

	*alp = iplist;
	*np = nip;
	return 0;

bail:
	(void)close(soc);
	return -1;
}

#else	/*SIOCGIFCONF*/

int
iflist(alp, np)
	struct in_addr **alp;	/* return list of addresses */
	int *np;				/* return len of alp */
{
	static struct in_addr *iplist = 0;	/* list of interface addrs found */
	int nip = 0;						/* length of iplist */
	char hn[MAXHOSTNAMELEN];
	struct hostent *he;
	char **p;

	if (iplist)
		PVM_FREE(iplist);
	iplist = TALLOC(10, struct in_addr, "ifl");

	if (gethostname(hn, sizeof(hn))) {
		perror("gethostname");
		goto bail;
	}
	if (!(he = gethostbyname(hn))) {
		fprintf(stderr, "can't gethostbyname\n");
		goto bail;
	}
	for (; he->h_addr_list[nip]; nip++) {
		if (nip > 0 && !(nip % 10))
			iplist = TREALLOC(iplist, (nip + 10), struct in_addr);
		iplist[nip].s_addr = ((struct in_addr*)(he->h_addr_list[nip]))->s_addr;
	}

	*alp = iplist;
	*np = nip;
	return 0;

bail:
	return -1;
}

#endif	/*SIOCGIFCONF*/


/*	acav()
*
*	Parse a string into words separated by whitespace.
*	Max number of words is original value of *acp.
*
*	Trashes out the original string.
*	Returns 0 with av[0]..av[*acp - 1] pointing to the words.
*	Returns 1 if too many words.
*/

int
acav(s, acp, av)
	char *s;		/* the string to parse */
	int *acp;		/* max words in, ac out */
	char **av;		/* pointers to args */
{
	register int ac;
	register char *p = s;
	register int n = *acp;

	/* separate out words of command */

	ac = 0;
	while (*p) {
		while (isspace(*p)) p++;
		if (*p) {
			if (ac >= n) {
	/* command too long */
				*acp = ac;
				return 1;
			}
			av[ac++] = p;
			while (*p && !isspace(*p)) p++;
			if (*p) *p++ = 0;
		}
	}
	*acp = ac;
	return 0;
}



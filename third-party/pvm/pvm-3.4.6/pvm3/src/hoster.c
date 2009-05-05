
static char rcsid[] =
	"$Id: hoster.c,v 1.22 2007/04/19 23:14:34 pvmsrc Exp $";

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
 *	hoster.c
 *
 *	Slave pvmd starter.
 *
 * $Log: hoster.c,v $
 * Revision 1.22  2007/04/19 23:14:34  pvmsrc
 * Added modern blanket header files to eliminate warnings on _exit()...
 * 	- #include <unistd.h>
 * 	- #include <stdlib.h>
 * (Spanker=kohl)
 *
 * Revision 1.21  2005/08/22 15:13:18  pvmsrc
 * Added #include <pvmtev.h> for #include global.h...  :-Q
 * 	- submitted by Skipper Hartley <charles.l.hartley@bankofamerica.com>
 * (Spanker=kohl)
 *
 * Revision 1.20  2001/09/27 21:25:09  pvmsrc
 * BEOSCYLD port.
 * 	- submitted by Joe Vitale <vitale@scyld.com>.
 * 	(we renamed it from BEOWULF to BEOSCYLD, but it's his port... :-)
 * (Spanker=kohl)
 *
 * Revision 1.19  2001/09/26 21:22:17  pvmsrc
 * Added Handling for Optional Virtual Machine ID.
 * 	- extra vmid comes through with SM_STHOST message (after wincmd).
 * 	- instruct user to type VMID on remote pvmd stdin for manual startup
 * 	- in phase1(), after potential rsh/rexec, write VMID env string
 * 		to remote pvmd's stdin.
 * (Spanker=kohl)
 *
 * Revision 1.18  2001/02/07 23:14:04  pvmsrc
 * First Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.17  2000/02/16 21:59:40  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.16  2000/02/14 20:31:57  pvmsrc
 * Lose #define-d RSHCOMMAND usage.
 * 	- use new pvmgetrsh() routine, which checks for PVM_RSH or else
 * 		uses old RSHCOMMAND interface.
 * (Spanker=kohl)
 *
 * Revision 1.15  1999/08/19 15:39:24  pvmsrc
 * Whoa...  New wincmd stuff was whacking addhost protocol!
 * 	- *always* pack in something for wincmd, else the attempt to unpack
 * 		"possible" wincmd will snag start of next host's startup info...
 * 	- damn.
 * (Spanker=kohl)
 *
 * Revision 1.14  1999/07/08 18:59:53  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.13  1999/01/28  18:56:21  pvmsrc
 * Added host add retries for alternate WIN32 pvmd command.
 * 	- added char *h_wincmd field to hst struct.
 * 	- in hoster() check for extra command string at end of each
 * 		host unpack (ignore if omitted), stick in h_wincmd ptr.
 * 	- in pl_startup() if default pvmd command fails,
 * 		and alternate WIN32 command is defined, reset h_cmd and
 * 		try phase1() again...
 * 	- in phase1(), if doing manual startup and an alternate WIN32
 * 		command is defined, echo that to the user too, so they can
 * 		try both before typing back the response...
 * (Spanker=kohl)
 *
 * Revision 1.12  1998/11/20  20:03:58  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (Spanker=sscott)
 *
 * Revision 1.11  1997/12/01  19:20:34  pvmsrc
 * Replaced #ifdef IMA_OS2 fd_set declarations:
 * 	- new #ifdef FDSETNOTSTRUCT.
 * 	- choose between "fd_set foo" and "struct fd_set foo"...
 * (Spanker=kohl)
 *
 * Revision 1.10  1997/11/04  23:19:16  pvmsrc
 * Cleaned up fd_set stuff (hopefully).
 * (Spanker=kohl)
 *
 * Revision 1.9  1997/09/10  17:40:54  pvmsrc
 * Oops...  bcopy() -> BCOPY()...
 * (Spanker=kohl)
 *
 * Revision 1.8  1997/08/29  13:35:03  pvmsrc
 * OS2 Port Submitted by Bohumir Horeni, horeni@login.cz.
 * (Spanker=kohl)
 *
 * Revision 1.7  1997/06/25  22:08:46  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.6  1997/04/21  14:58:22  pvmsrc
 * Changed #ifdefs that checked IMA_RS6K,IMA_SP2MPI & IMA_AIX46K
 * 	to see if select.h was needed into single define NEEDSSELECTH.
 * 	New archs need to set this in conf/
 *
 * Revision 1.5  1997/01/28  19:26:19  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.4  1996/10/25  13:57:19  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.3  1996/10/24  21:04:47  pvmsrc
 * Moved #include of "global.h" down below other headers:
 * 	- need to have all of the structures / types declared before
 * 		the globals can be declared...
 *
 * Revision 1.2  1996/09/23  23:48:01  pvmsrc
 * Initial Creation - original hoster.c.
 *
 * Revision 1.1  1996/09/23  23:44:09  pvmsrc
 * Initial revision
 *
 * Revision 1.17  1995/11/02  16:36:14  manchek
 * added NEEDSENDIAN switch
 *
 * Revision 1.16  1995/07/28  16:41:01  manchek
 * wrap HASERRORVARS around errno declarations
 *
 * Revision 1.15  1995/07/28  16:04:06  manchek
 * switch endian includes on flag, not arch name
 *
 * Revision 1.14  1995/07/11  18:55:33  manchek
 * moved printing of PVMSOCK in master_config to main
 *
 * Revision 1.13  1995/07/03  19:06:54  manchek
 * removed POWER4 ifdefs
 *
 * Revision 1.12  1995/05/30  17:53:08  manchek
 * Added ifdefs and small changes for SP2MPI arch
 *
 * Revision 1.11  1995/05/17  16:53:03  manchek
 * use FDSETISINT in select
 *
 * Revision 1.10  1995/02/06  21:35:59  manchek
 * host table mtu set from ourudpmtu instead of UDPMAXLEN
 *
 * Revision 1.9  1994/10/15  19:31:31  manchek
 * fixed typo in log message
 *
 * Revision 1.8  1994/06/04  21:45:24  manchek
 * added unix domain sockets.
 * ripped out old (serial) startup code
 *
 * Revision 1.7  1994/06/03  20:38:27  manchek
 * version 3.3.0
 *
 * Revision 1.6  1993/11/30  19:54:41  manchek
 * check the default entry in filehosts when adding new hosts
 *
 * Revision 1.5  1993/11/30  16:46:10  manchek
 * pass whole remote command as a single arg to rsh
 *
 * Revision 1.4  1993/11/30  15:54:37  manchek
 * master pvmd once again doesn't close fds 0..2 -
 * this broke rexec startup
 *
 * Revision 1.3  1993/10/25  20:53:51  manchek
 * fixed a few typos in error log messages.
 * added code to close all fds and reopen 0..2 as /dev/null
 *
 * Revision 1.2  1993/10/04  20:30:30  manchek
 * mksocks() now uses pvmdsockfile() instead of TDSOCKNAME
 *
 * Revision 1.1  1993/08/30  23:26:51  manchek
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

#include <pvm3.h>

#if defined(WIN32) || defined(CYGWIN)
#include "..\xdr\types.h"
#include "..\xdr\xdr.h"
#else
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

#ifndef WIN32
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#ifdef NEEDSSELECTH
#include <sys/select.h>
#endif

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#ifdef	SYSVSTR
#include <string.h>
#define	CINDEX(s,c)	strchr(s,c)
#else
#include <strings.h>
#define	CINDEX(s,c)	index(s,c)
#endif

#include <pvmproto.h>
#include "pvmalloc.h"
#include "pmsg.h"
#include "host.h"
#include "listmac.h"
#include "tvdefs.h"
#include "bfunc.h"
#include <pvmtev.h>
#include "global.h"

#ifdef IMA_BEOSCYLD
#include <string.h>
#endif

#ifndef	RSHTIMEOUT
#define	RSHTIMEOUT	60
#endif

#ifndef	RSHNPLL
#define	RSHNPLL	5
#endif

#ifndef	max
#define	max(a,b)	((a)>(b)?(a):(b))
#endif

#ifndef	min
#define	min(a,b)	((a)<(b)?(a):(b))
#endif


struct hst {
	int h_tid;
	char *h_name;
	char *h_login;
	char *h_sopts;
	int h_flag;
#define	HST_PASSWORD	1		/* ask for a password */
#define	HST_MANUAL		2		/* do manual startup */
	char *h_cmd;
	char *h_wincmd;				/* alternate WIN32 default pvmd cmd */
	char *h_vmid;				/* optional virtual machine ID */
	char *h_result;
};

struct slot {
	struct slot *s_link, *s_rlink;		/* free/active list */
	struct hst *s_hst;					/* host table entry */
	struct timeval s_bail;				/* timeout time */
	int s_rfd, s_wfd, s_efd;			/* slave stdin/out/err */
	char s_buf[256];					/* stdout from host */
	char s_ebuf[256];					/* stderr from host */
	int s_len;							/* length of s_buf */
	int s_elen;							/* length of s_ebuf */
};


/***************
 **  Globals  **
 **           **
 ***************/

#ifndef HASERRORVARS
extern int errno;
#endif

extern void pvmbailout();

extern int pvmdebmask;				/* from pvmd.c */
extern char *username;				/* from pvmd.c */

char *pvmgetrsh();


/***************
 **  Private  **
 **           **
 ***************/

static struct slot slots[RSHNPLL+2];	/* state var/context for each slot */
static struct slot *slfree = 0;			/* free list of slots */


int
hoster(mp)
	struct pmsg *mp;
{
	struct pmsg *mp2;
	int num;
	int i;
	struct hst **hostlist;
	struct hst *hp;
	char *p;

	/*
	* unpack the startup message
	*/

	upkint(mp, &num);
	if (pvmdebmask & PDMSTARTUP) {
		pvmlogprintf("hoster() %d to start\n", num);
	}
	if (num > 0) {
		hostlist = TALLOC(num, struct hst *, "hsts");
		for (i = 0; i < num; i++) {
			hp = TALLOC(1, struct hst, "hst");
			hostlist[i] = hp;
			hp->h_flag = 0;
			hp->h_result = 0;
			if (upkint(mp, &hp->h_tid)
			|| upkstralloc(mp, &hp->h_sopts)
			|| upkstralloc(mp, &hp->h_login)
			|| upkstralloc(mp, &hp->h_cmd)
			|| upkstralloc(mp, &hp->h_wincmd)
			|| upkstralloc(mp, &hp->h_vmid)) {
				pvmlogerror("hoster() bad message format\n");
				pvmbailout(0);
			}
			/* Check for (possible) alternate WIN32 pvmd cmd */
			if (!strcmp(hp->h_wincmd,"")) {
				PVM_FREE(hp->h_wincmd);
				hp->h_wincmd = 0;
			}
			/* Check for (optional) virtual machine ID */
			if (!strcmp(hp->h_vmid,"")) {
				PVM_FREE(hp->h_vmid);
				hp->h_vmid = 0;
			}
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("%d. t%x %s so=\"%s\"\n", i,
						hp->h_tid,
						hp->h_login,
						hp->h_sopts);
			}
			if (p = CINDEX(hp->h_login, '@')) {
				hp->h_name = STRALLOC(p + 1);
				*p = 0;
				p = STRALLOC(hp->h_login);
				PVM_FREE(hp->h_login);
				hp->h_login = p;

			} else {
				hp->h_name = hp->h_login;
				hp->h_login = 0;
			}
			if (!strcmp(hp->h_sopts, "pw"))
				hp->h_flag |= HST_PASSWORD;
			if (!strcmp(hp->h_sopts, "ms"))
				hp->h_flag |= HST_MANUAL;
		}
	}

	/*
	* do it
	*/

	pl_startup(num, hostlist);

	/*
	* send results back to pvmd
	*/

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_STARTACK;
	mp2->m_wid = mp->m_wid;
	pkint(mp2, num);
	for (i = 0; i < num; i++) {
		pkint(mp2, hostlist[i]->h_tid);
		pkstr(mp2, hostlist[i]->h_result
				? hostlist[i]->h_result : "PvmDSysErr");
	}
	if (pvmdebmask & PDMSTARTUP)
		pvmlogerror("hoster() pvmd' sending back host table\n");
	sendmessage(mp2);
	work();		/* no return */
	return 0;	/* not reached */
}


char *
makevis(dst, src)
	char *dst;
	char *src;
{
	char c;

	while (c = *src++ & 0x7f) {
		if (isprint(c))
			*dst++ = c;

		else {
			*dst++ = '^';
			*dst++ = (c + '@') & 0x7f;
		}
	}
	*dst = 0;
	return dst;
}


close_slot(sp)
	struct slot *sp;
{
	if (sp->s_wfd != -1)
		(void)close(sp->s_wfd);
	if (sp->s_rfd != -1)
		(void)close(sp->s_rfd);
	if (sp->s_efd != -1)
		(void)close(sp->s_efd);
	LISTDELETE(sp, s_link, s_rlink);
	LISTPUTBEFORE(slfree, sp, s_link, s_rlink);
	return 0;
}


pl_startup(num, hostlist)
	int num;
	struct hst **hostlist;
{
	int nxth = 0;						/* next host in list to start */
	struct slot *slact = 0;				/* active list of slots */
	struct hst *hp;
	struct slot *sp, *sp2;
	struct timeval tnow;
	struct timeval tout;
#ifdef FDSETNOTSTRUCT
	fd_set rfds;						/* emx headers */
#else
	struct fd_set rfds;
#endif
	int nfds;
	int i;
	int n;
	char *p;

	/* init slot free list */

	slfree = &slots[RSHNPLL+1];
	slfree->s_link = slfree->s_rlink = slfree;
	slact = &slots[RSHNPLL];
	slact->s_link = slact->s_rlink = slact;
	for (i = RSHNPLL; i-- > 0; ) {
		LISTPUTAFTER(slfree, &slots[i], s_link, s_rlink);
	}

	/*
	* keep at this until all hosts in table are completed
	*/

	for (; ; ) {

		/*
		* if empty slots, start on new hosts
		*/

		for (; ; ) {

			/* find a host for slot */

			if (slfree->s_link != slfree && nxth < num)
				hp = hostlist[nxth++];
			else
				break;

			sp = slfree->s_link;
			LISTDELETE(sp, s_link, s_rlink);
			sp->s_hst = hp;
			sp->s_len = 0;
			sp->s_elen = 0;
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("pl_startup() trying %s\n", hp->h_name);
			}
			phase1(sp);
			if (hp->h_result) {
				/* error or fully started (manual startup) */

				if (pvmdebmask & PDMSTARTUP) {
					pvmlogprintf(
					"pl_startup() got result \"%s\" for %s phase1()\n",
						hp->h_result, hp->h_name);
				}

				LISTPUTBEFORE(slfree, sp, s_link, s_rlink);

			} else {
				/* partially started */

				LISTPUTBEFORE(slact, sp, s_link, s_rlink);
				pvmgetclock(&sp->s_bail);
				tout.tv_sec = RSHTIMEOUT;
				tout.tv_usec = 0;
				TVXADDY(&sp->s_bail, &sp->s_bail, &tout);
			}
		}

		/* if no hosts in progress, we are finished */

		if (slact->s_link == slact)
			break;

		/*
		* until next timeout, get output from any slot
		*/

		FD_ZERO(&rfds);
		nfds = 0;
		TVCLEAR(&tout);
		pvmgetclock(&tnow);
		for (sp = slact->s_link; sp != slact; sp = sp->s_link) {
			if (TVXLTY(&sp->s_bail, &tnow)) {
				pvmlogprintf("pl_startup() giving up on host %s after %d secs\n",
						sp->s_hst->h_name, RSHTIMEOUT);
				sp->s_hst->h_result = STRALLOC("PvmCantStart");
				sp2 = sp->s_rlink;
				close_slot(sp);
				sp = sp2;
				continue;
			}

			if (!TVISSET(&tout) || TVXLTY(&sp->s_bail, &tout))
				tout = sp->s_bail;
			if (sp->s_rfd >= 0)
				FD_SET(sp->s_rfd, &rfds);
			if (sp->s_rfd > nfds)
				nfds = sp->s_rfd;
			if (sp->s_efd >= 0)
				FD_SET(sp->s_efd, &rfds);
			if (sp->s_efd > nfds)
				nfds = sp->s_efd;
		}

		if (slact->s_link == slact)
			break;

		nfds++;

		if (TVXLTY(&tnow, &tout)) {
			TVXSUBY(&tout, &tout, &tnow);
		} else {
			TVCLEAR(&tout);
		}
		if (pvmdebmask & PDMSTARTUP) {
			pvmlogprintf("pl_startup() select timeout is %d.%06d\n",
					tout.tv_sec, tout.tv_usec);
		}
		if ((n = select(nfds,
#ifdef	FDSETISINT
				(int *)&rfds, (int *)0, (int *)0,
#else
				(fd_set *)&rfds, (fd_set *)0, (fd_set *)0,
#endif
				&tout)) == -1) {
			if (errno != EINTR) {
				pvmlogperror("pl_startup() select");
				pvmbailout(0);
			}
		}
		if (pvmdebmask & PDMSTARTUP) {
			pvmlogprintf("pl_startup() select returns %d\n", n);
		}
		if (n < 1) {
			if (n == -1 && errno != EINTR) {
				pvmlogperror("pl_startup() select");
				pvmbailout(0);	/* XXX this is too harsh */
			}
			continue;
		}

		/*
		* check for response on stdout or stderr of any slave.
		*/

		for (sp = slact->s_link; sp != slact; sp = sp->s_link) {

			/*
			* stderr ready.  log output with remote's host name.
			*/
			if (sp->s_efd >= 0 && FD_ISSET(sp->s_efd, &rfds)) {
				n = read(sp->s_efd, sp->s_ebuf + sp->s_elen,
						sizeof(sp->s_ebuf) - sp->s_elen - 1);
				if (n > 0) {
					sp->s_elen += n;
					sp->s_ebuf[sp->s_elen] = 0;
					while (p = CINDEX(sp->s_ebuf, '\n')) {
						*p = 0;
						pvmlogprintf("stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						p++;
						sp->s_elen -= p - sp->s_ebuf;
						if (sp->s_elen > 0) {
							BCOPY(p, sp->s_ebuf, sp->s_elen);
							sp->s_ebuf[sp->s_elen] = 0;
						}
					}
					if (sp->s_elen == sizeof(sp->s_ebuf) - 1) {
						pvmlogprintf("stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						sp->s_elen = 0;
					}

				} else {
					if (sp->s_elen > 0) {
						pvmlogprintf("stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						sp->s_elen = 0;
					}
					(void)close(sp->s_efd);
					sp->s_efd = -1;
				}
			}

			/*
			* stdout ready.
			* look for a complete line starting with "ddpro".
			*/
			if (sp->s_rfd >= 0 && FD_ISSET(sp->s_rfd, &rfds)) {
				n = read(sp->s_rfd, sp->s_buf + sp->s_len,
						sizeof(sp->s_buf) - sp->s_len - 1);
				if (n > 0) {
					sp->s_len += n;
					sp->s_buf[sp->s_len] = 0;
					while (p = CINDEX(sp->s_buf, '\n')) {
						*p = 0;
						if (!strncmp(sp->s_buf, "ddpro", 5)) {
							if (pvmdebmask & PDMSTARTUP) {
								pvmlogprintf("stdout@%s: %s\n",
										sp->s_hst->h_name, sp->s_buf);
							}
							sp->s_hst->h_result = STRALLOC(sp->s_buf);
							break;

						} else {
							pvmlogprintf("stdout@%s: %s\n",
									sp->s_hst->h_name, sp->s_buf);
							p++;
							sp->s_len -= p - sp->s_buf;
							if (sp->s_len > 0) {
								BCOPY(p, sp->s_buf, sp->s_len);
								sp->s_buf[sp->s_len] = 0;
							}
						}
					}
					if (sp->s_len == sizeof(sp->s_buf) - 1) {
						pvmlogprintf("stdout@%s: %s\n",
								sp->s_hst->h_name, sp->s_buf);
						sp->s_len = 0;
					}

				} else {
					if (sp->s_len > 0) {
						pvmlogprintf("stdout@%s: %s\n",
								sp->s_hst->h_name, sp->s_buf);
						sp->s_len = 0;
					}
					if (n) {
						pvmlogprintf("stdout@%s",
								sp->s_hst->h_name);
					} else {
						pvmlogprintf("stdout@%s: EOF\n",
								sp->s_hst->h_name);
					}
					if (sp->s_elen > 0) {
						pvmlogprintf("stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						sp->s_elen = 0;
					}

					/* before failing, check for alternate WIN32 cmd */
					if (sp->s_hst->h_wincmd) {
						if (pvmdebmask & PDMSTARTUP) {
							pvmlogprintf(
								"pl_startup() re-trying %s as WIN32\n",
								hp->h_name);
						}
						/* no need to free h_cmd, we're a pvmd'... */
						sp->s_hst->h_cmd = sp->s_hst->h_wincmd;
						sp->s_hst->h_wincmd = 0;

						phase1(sp);

						if (!(sp->s_hst->h_result)) {
							/* partially started */
							pvmgetclock(&sp->s_bail);
							tout.tv_sec = RSHTIMEOUT;
							tout.tv_usec = 0;
							TVXADDY(&sp->s_bail, &sp->s_bail, &tout);
						}

					} else {
						sp->s_hst->h_result = STRALLOC("PvmCantStart");
					}
				}
				if (sp->s_hst->h_result) {
					sp2 = sp->s_rlink;
					close_slot(sp);
					sp = sp2;
					continue;
				}
			}
		}
	}
	return 0;
}


phase1(sp)
	struct slot *sp;
{
	struct hst *hp;
	char *hn;
	char *av[32];			/* for rsh args */
	int ac;
	char buf[512];
	int pid = -1;			/* pid of rsh */
	char *p;

#ifndef NOREXEC
	struct servent *se;
	static u_short execport = 0;

	if (!execport) {
		if (!(se = getservbyname("exec", "tcp"))) {
			pvmlogprintf("phase1() can't getservbyname(): %s\n", "exec");
			pvmbailout(0);
		}
		execport = se->s_port;
		endservent();
	}
#endif

	hp = sp->s_hst;
	hn = hp->h_name;
	sp->s_rfd = sp->s_wfd = sp->s_efd = -1;

	/*
	* XXX manual startup hack... this is if we can't use rexec or rsh
	*/

	if (hp->h_flag & HST_MANUAL) {
		fprintf(stderr, "*** Manual startup ***\n");
		fprintf(stderr, "Login to \"%s\" and type:\n", hn);

		if (hp->h_wincmd) {
			fprintf(stderr, "%s\n\n", hp->h_cmd);
			fprintf(stderr, "or, if that command fails, " );
			fprintf(stderr, "for WIN32 hosts try:\n");
			fprintf(stderr, "%s\n\n", hp->h_wincmd);
		}
		else
			fprintf(stderr, "%s\n", hp->h_cmd);

	/* get version */

		fprintf(stderr, "Type response: ");
		fflush(stderr);
		if (!(fgets(buf, sizeof(buf), stdin))) {
			pvmlogprintf("host %s read error\n", hn);
			goto oops;
		}
		p = buf + strlen(buf) - 1;
		if (*p == '\n')
			*p = 0;
		hp->h_result = STRALLOC(buf);

	/* send vmid, if set */

		if (hp->h_vmid) {
			fprintf(stderr, "Now Type the Virtual Machine ID on %s:\n",
					hn);
			fprintf(stderr, "%s\n", hp->h_vmid);
		}

	/* done */

		fprintf(stderr, "Thanks\n");
		fflush(stderr);
		return 0;
	}

	/*
	* XXX end manual startup hack
	*/

	if (!(hp->h_flag & HST_PASSWORD)) {		/* use rsh to start */
		int wpfd[2], rpfd[2], epfd[2];
		int i;

		if (pvmdebmask & PDMSTARTUP) {
			pvmlogprintf("phase1() trying rsh to %s\n", hn);
		}

	/* fork an rsh to startup the slave pvmd */

#ifdef	IMA_TITN
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, wpfd) == -1
		|| socketpair(AF_UNIX, SOCK_STREAM, 0, rpfd) == -1
		|| socketpair(AF_UNIX, SOCK_STREAM, 0, epfd) == -1) {
			pvmlogperror("phase1() socketpair");
			goto oops;
		}
#else
		if (pipe(wpfd) == -1 || pipe(rpfd) == -1 || pipe(epfd) == -1) {
			pvmlogperror("phase1() pipe");
			goto oops;
		}
#endif

		if (pvmdebmask & PDMSTARTUP) {
			pvmlogprintf("phase1() pipes: %d %d %d %d %d %d\n",
					wpfd[0], wpfd[1], rpfd[0], rpfd[1], epfd[0], epfd[1]);
		}

		if ((pid = fork()) == -1) {
			pvmlogperror("phase1() fork");
			pvmbailout(0);
		}
		if (!pid) {
			(void)dup2(wpfd[0], 0);
			(void)dup2(rpfd[1], 1);
			(void)dup2(epfd[1], 2);
			for (i = getdtablesize(); --i > 2; )
				(void)close(i);
			ac = 0;
			av[ac++] = pvmgetrsh();
			av[ac++] = hn;
			if (hp->h_login) {
				av[ac++] = "-l";
				av[ac++] = hp->h_login;
			}
#ifdef IMA_BEOSCYLD
			/* Chop up the command line and all its parameters into
			 * individual strings. The underlying assumption here is
			 * that PVM_RSH is defined to specify 'bpsh' instead of
			 * 'rsh/ssh'. This bit of code converts the single string
			 * av[2] from this: "pvmd3 -option1 -option2", into this:
			 * av[2] = "pvmd3", av[3] = "-option1", av[4] = "-option2").
			 * If this is not done 'bpsh' treats the original av[2] as
			 * the file name of the command to be executed, which will
			 * fail to execute.
			 */
			p = hp->h_cmd;
			do {
				av[ac++] = p;
				p = strstr(p," ");
				if (p != NULL)
					*p++ = '\0';
			} while (p != NULL);
#else
			av[ac++] = hp->h_cmd;
#endif
			av[ac++] = 0;
			if (pvmdebmask & PDMSTARTUP) {
				for (ac = 0; av[ac]; ac++)
					fprintf(stderr, "av[%d]=\"%s\" ", ac, av[ac]);
				fputc('\n', stderr);
			}
			execvp(av[0], av);
			fputs("phase1() execvp failed\n", stderr);
			fflush(stderr);
			_exit(1);
		}
		(void)close(wpfd[0]);
		(void)close(rpfd[1]);
		(void)close(epfd[1]);
		sp->s_wfd = wpfd[1];
		sp->s_rfd = rpfd[0];
		sp->s_efd = epfd[0];

	} else {		/* use rexec to start */

#ifdef NOREXEC
		pvmlogprintf("slconfg() PVM was built without rexec support, can't take passwords\n");
		goto oops;
#else
		if (pvmdebmask & PDMSTARTUP) {
			pvmlogprintf("phase1() rexec \"%s\"\n", hp->h_cmd);
		}
		if ((sp->s_wfd = sp->s_rfd = rexec(&hn, execport,
				(hp->h_login ? hp->h_login : username),
				(char*)0, hp->h_cmd, &sp->s_efd))
		== -1) {
			pvmlogprintf("phase1() rexec failed for host %s\n", hn);
			goto oops;
		}
#endif
	}

	/* send vmid, if set */
	if (sp->s_hst->h_vmid && sp->s_wfd >= 0) {
#ifdef WIN32
		win32_write_socket(sp->s_wfd, "PVM_VMID=", 9);
		win32_write_socket(sp->s_wfd,
				sp->s_hst->h_vmid, strlen(sp->s_hst->h_vmid));
		win32_write_socket(sp->s_wfd, "\n", 1);
#else
		write(sp->s_wfd, "PVM_VMID=", 9);
		write(sp->s_wfd, sp->s_hst->h_vmid, strlen(sp->s_hst->h_vmid));
		write(sp->s_wfd, "\n", 1);
#endif
	}

	return 0;

oops:
	hp->h_result = STRALLOC("PvmCantStart");
	if (sp->s_wfd != -1)
		close(sp->s_wfd);
	if (sp->s_rfd != -1)
		close(sp->s_rfd);
	if (sp->s_efd != -1)
		close(sp->s_efd);
	sp->s_wfd = sp->s_rfd = sp->s_efd = -1;
	return 1;
}



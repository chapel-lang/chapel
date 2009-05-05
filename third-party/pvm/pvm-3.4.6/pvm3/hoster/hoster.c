
static char rcsid[] =
	"$Id: hoster.c,v 1.19 2005/05/18 17:55:41 pvmsrc Exp $";

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
 * hoster.c
 *
 * Example hoster task to demonstrate and verify the protocol.
 * This code was cut out of the pvmd default hoster in startup.c,
 * and minimally modified to make it compile here.
 *
 * 24 Jan 1994  Manchek
 */

#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#ifndef WIN32
#include <pwd.h>
#include <netdb.h>
#endif
#ifdef NEEDSSELECTH
#include <sys/select.h>
#endif
#ifdef	SYSVSTR
#include <string.h>
#define	CINDEX(s,c)	strchr(s,c)
#else
#include <strings.h>
#define	CINDEX(s,c)	index(s,c)
#endif
#include <pvm3.h>
#include <pvmproto.h>
#include "bfunc.h"

#ifdef WIN32
#include "pvmwin.h"
#endif

#ifndef	RSHTIMEOUT
#define	RSHTIMEOUT 100
#endif

/* if > 1, uses parallel startup strategy */
#ifndef	RSHNPLL
#define	RSHNPLL	5
#endif

#ifndef	max
#define	max(a,b)	((a)>(b)?(a):(b))
#endif

#ifndef	min
#define	min(a,b)	((a)<(b)?(a):(b))
#endif

#ifdef WIN32
#define close(fd)	_close(fd)
#endif

#define	TALLOC(n,t,g)	(t*)malloc((n)*sizeof(t))
#define	FREE(p)	free((char *)p)
#define	STRALLOC(s)			strcpy(TALLOC(strlen(s)+1,char,"str"),s)

#define	LISTPUTAFTER(o,n,f,r) \
	{ (n)->f=(o)->f; (n)->r=(o); (o)->f->r=(n); (o)->f=(n); }
#define	LISTPUTBEFORE(o,n,f,r) \
	{ (n)->r=(o)->r; (n)->f=(o); (o)->r->f=(n); (o)->r=(n); }
#define	LISTDELETE(e,f,r) \
	{ (e)->f->r=(e)->r; (e)->r->f=(e)->f; (e)->r=(e)->f=0; }

#define	TVCLEAR(tvp)	((tvp)->tv_sec = (tvp)->tv_usec = 0)

#define	TVISSET(tvp)	((tvp)->tv_sec || (tvp)->tv_usec)

#define	TVXLTY(xtv, ytv) \
	((xtv)->tv_sec < (ytv)->tv_sec || \
		((xtv)->tv_sec == (ytv)->tv_sec && (xtv)->tv_usec < (ytv)->tv_usec))

#define	TVXADDY(ztv, xtv, ytv)	\
	if (((ztv)->tv_usec = (xtv)->tv_usec + (ytv)->tv_usec) < 1000000) {	\
		(ztv)->tv_sec = (xtv)->tv_sec + (ytv)->tv_sec;	\
	} else {	\
		(ztv)->tv_usec -= 1000000;	\
		(ztv)->tv_sec = (xtv)->tv_sec + (ytv)->tv_sec + 1;	\
	}

#define	TVXSUBY(ztv, xtv, ytv)	\
	if ((xtv)->tv_usec >= (ytv)->tv_usec) {	\
		(ztv)->tv_sec = (xtv)->tv_sec - (ytv)->tv_sec;	\
		(ztv)->tv_usec = (xtv)->tv_usec - (ytv)->tv_usec;	\
	} else {	\
		(ztv)->tv_sec = (xtv)->tv_sec - (ytv)->tv_sec - 1;	\
		(ztv)->tv_usec = (xtv)->tv_usec + 1000000 - (ytv)->tv_usec;	\
	}

/*
*	for keeping state assoc. with a host
*/

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

/* 0 turns off -- set to 1 to activate error messages in hoster */
int debugmask = 0;

#ifndef WIN32
char *username = 0;
#else
extern char* username;
extern int rsh( char *hostname, char *username, char *command,
	char *retresult );
#endif

char *pvmgetrsh();

/***************
 **  Private  **
 **           **
 ***************/

static struct slot slots[RSHNPLL+2];	/* state var/ctx each slot */
static struct slot *slfree = 0;			/* free list of slots */


int
main(argc, argv)
	int argc;
	char **argv;
{
#ifdef WIN32 
	static int init = 0;
	static int needed = 1;
#else
	struct passwd *pe;
	int uid;

	if ((uid = getuid()) == -1) {
		fprintf(stderr, "main() can't getuid()\n");
		exit(1);
	}
	pvmchkuid( uid );
	if (pe = getpwuid(uid))
		username = STRALLOC(pe->pw_name);
	else
		fprintf(stderr, "main() can't getpwuid\n");
	endpwent();
#endif

	pvm_mytid();
	pvm_reg_hoster();
	pvm_setopt(PvmResvTids, 1);

#ifndef WIN32
	printf("*** PVM pvmd starter ***\n");
#endif

	while (pvm_recv(-1, SM_STHOST) > 0) {

#ifdef WIN32
		AllocConsole();
		
		GetStdHandle(STD_OUTPUT_HANDLE);
		if (init)  
			printf(
				"***need more ?                                 ***\n");
#endif

		hoster();

#ifdef WIN32
		if (!init) {
			printf(
				"*** PVM %s pvmd starter                                     ***\n",
				PVM_VER);
			printf(
				"*** This window will show what your 'add command' is doing. ***\n");
			printf(
				"*** Also, if required, this is where you type in passwords. ***\n");
			printf(
				"*** This window will display each time you add machines.    ***\n");
			Sleep(15000);	
			FreeConsole();
			init=1;
		}
		Sleep(1500);
		FreeConsole();	
#endif 
	}

	pvm_exit();
	exit(0);

#ifdef WIN32
	return(0);
#endif
}
 

/*	hoster()
*
*	Unpack host table from message, attempt to start 'em up,
*	send reply message.
*/

hoster()
{
	int num;
	int i;
	struct hst **hostlist;
	struct hst *hp;
	char *p;
	char sopts[64];
	char lognam[256];
	char cmd[512];
	char wincmd[512];
	char vmid[256];
	int fromtid;
	int wid;
	struct pvmminfo minfo;

	/*
	* unpack the startup message
	*/

	pvm_bufinfo(pvm_getrbuf(), (int *)0, (int *)0, &fromtid);
	pvm_unpackf("%d", &num);
	pvm_getminfo(pvm_getrbuf(), &minfo);
	wid = minfo.wid;
	printf("hoster() %d to start, wait id %d\n", num, wid);
	if (num > 0) {
		hostlist = TALLOC(num, struct hst *, "xxx");
		for (i = 0; i < num; i++) {
			hp = TALLOC(1, struct hst, "xxx");
			hostlist[i] = hp;
			hp->h_flag = 0;
			hp->h_result = 0;
			if (pvm_unpackf("%d %s %s %s %s %s",
					&hp->h_tid, sopts, lognam, cmd, wincmd, vmid)) {
				printf("hoster() bad message format\n");
				pvm_exit();
				exit(1);
			}
			hp->h_sopts = STRALLOC(sopts);
			hp->h_login = STRALLOC(lognam);
			hp->h_cmd = STRALLOC(cmd);

			/* Check for (possible) alternate WIN32 pvmd cmd */
			if (!strcmp(wincmd,""))
				hp->h_wincmd = 0;
			else
				hp->h_wincmd = STRALLOC(wincmd);

			/* Check for (optional) virtual machine ID */
			if (!strcmp(vmid,""))
				hp->h_vmid = 0;
			else
				hp->h_vmid = STRALLOC(vmid);

			printf("%d. t%x %s so=\"%s\"\n", i,
					hp->h_tid,
					hp->h_login,
					hp->h_sopts);
			if (p = CINDEX(hp->h_login, '@')) {
				hp->h_name = STRALLOC(p + 1);
				*p = 0;
				p = STRALLOC(hp->h_login);
				FREE(hp->h_login);
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

	pvm_packf("%+ %d", PvmDataFoo, num);
	for (i = 0; i < num; i++) {
		pvm_packf("%d", hostlist[i]->h_tid);
		pvm_packf("%s", hostlist[i]->h_result
				? hostlist[i]->h_result : "PvmDSysErr");
	}

/*
	printf("hoster() sending back host table\n");
*/
	pvm_getminfo(pvm_getsbuf(), &minfo);
	minfo.wid = wid;
	pvm_setminfo(pvm_getsbuf(), &minfo);
	pvm_send(fromtid, SM_STHOSTACK);

	return 0;
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
	fd_set rfds;
#else
	struct fd_set rfds;
#endif
	int nfds;
	int i;
	int n;
	char *p;
	char c;

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
			if (debugmask) {
				fprintf(stderr, "pl_startup() trying %s\n", hp->h_name);
			}
			phase1(sp);
			if (hp->h_result) {
				/* error or fully started (manual startup) */

				if (debugmask) {
					fprintf(stderr,
					"pl_startup() got result \"%s\" for %s phase1()\n",
						hp->h_result, hp->h_name);
				}

				if (!strcmp(hp->h_result,"PvmCantStart")
						&& hp->h_wincmd) {
					if (debugmask) {
						fprintf(stderr,
							"pl_startup() re-trying %s as WIN32\n",
							hp->h_name);
					}
					hp->h_cmd = sp->s_hst->h_wincmd;
					hp->h_wincmd = 0;
					FREE(hp->h_result);
					hp->h_result = 0;

					phase1(sp);

					if (hp->h_result) {
						if (debugmask) {
							fprintf(stderr,
					"pl_startup() got result \"%s\" for %s phase1()\n",
								hp->h_result, hp->h_name);
						}
					} else {
						/* partially started */

						LISTPUTBEFORE(slact, sp, s_link, s_rlink);
						gettimeofday(&sp->s_bail, (struct timezone*)0);
						tout.tv_sec = RSHTIMEOUT;
						tout.tv_usec = 0;
						TVXADDY(&sp->s_bail, &sp->s_bail, &tout);
					}

				} else {
					LISTPUTBEFORE(slfree, sp, s_link, s_rlink);
				}

			} else {
				/* partially started */

				LISTPUTBEFORE(slact, sp, s_link, s_rlink);
				gettimeofday(&sp->s_bail, (struct timezone*)0);
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
		gettimeofday(&tnow, (struct timezone*)0);
		for (sp = slact->s_link; sp != slact; sp = sp->s_link) {
			if (TVXLTY(&sp->s_bail, &tnow)) {
				fprintf(stderr,
					"pl_startup() giving up on host %s after %d secs\n",
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
		if (debugmask) {
			fprintf(stderr, "pl_startup() select timeout is %d.%06ld\n",
					tout.tv_sec, tout.tv_usec);
		}
		if ((n = select(nfds,
#ifdef  FDSETISINT
				(int *)&rfds, (int *)0, (int *)0,
#else
				(fd_set *)&rfds, (fd_set *)0, (fd_set *)0,
#endif
				&tout)) == -1) {
			if (errno != EINTR) {
#ifdef WIN32
				pvmlogperror("pl_startup() select");
#else
				perror("pl_startup() select");
#endif
				pvmbailout(0);
			}
		}
		if (debugmask) {
			fprintf(stderr, "pl_startup() select returns %d\n", n);
		}
		if (n < 1) {
			if (n == -1 && errno != EINTR) {
#ifdef WIN32
				pvmlogperror("pl_startup() select");
#else
				perror("pl_startup() select");
#endif
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
#ifdef WIN32
				n = win32_read_socket(sp->s_efd,
						sp->s_ebuf + sp->s_elen,
						sizeof(sp->s_ebuf) - sp->s_elen - 1);
#else
				n = read(sp->s_efd, sp->s_ebuf + sp->s_elen,
						sizeof(sp->s_ebuf) - sp->s_elen - 1);
#endif
				if (n > 0) {
					sp->s_elen += n;
					sp->s_ebuf[sp->s_elen] = 0;
#ifdef WIN32

					p = sp->s_ebuf + sp->s_elen - n;
					fprintf(stderr, "stderr@%s: ", sp->s_hst->h_name);
					while (c = *p++ & 0x7f) {
						if (isprint(c))
							fputc(c, stderr);

						else {
							fputc('^', stderr);
							fputc((c + '@') & 0x7f, stderr);
						}
					}
					fputc('\n', stderr);
#else
					while (p = CINDEX(sp->s_ebuf, '\n')) {
						*p = 0;
						fprintf(stderr, "stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						p++;
						sp->s_elen -= p - sp->s_ebuf;
						if (sp->s_elen > 0) {
							BCOPY(p, sp->s_ebuf, sp->s_elen);
							sp->s_ebuf[sp->s_elen] = 0;
						}
					}
					if (sp->s_elen == sizeof(sp->s_ebuf) - 1) {
						fprintf(stderr, "stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						sp->s_elen = 0;
					}
#endif
				} else {
#ifndef WIN32
					if (sp->s_elen > 0) {
						fprintf(stderr, "stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						sp->s_elen = 0;
					}
#endif
					(void)close(sp->s_efd);
					sp->s_efd = -1;
				}
			}

			/*
			* stdout ready.
			* look for a complete line starting with "ddpro".
			*/
			if (sp->s_rfd >= 0 && FD_ISSET(sp->s_rfd, &rfds)) {
#ifdef WIN32
				n = win32_read_socket(sp->s_rfd, sp->s_buf + sp->s_len,
						sizeof(sp->s_buf) - sp->s_len - 1);
#else
				n = read(sp->s_rfd, sp->s_buf + sp->s_len,
						sizeof(sp->s_buf) - sp->s_len - 1);
#endif
				if (n > 0) {
					sp->s_len += n;
#ifdef WIN32
					if (sp->s_len >= sizeof(sp->s_buf)) {
						fprintf(stderr,
								"pl_startup() pvmd@%s: big read\n",
								sp->s_hst->h_name);
						sp->s_hst->h_result = STRALLOC("PvmCantStart");
					}
#endif
					sp->s_buf[sp->s_len] = 0;
#ifdef WIN32
					if (p = CINDEX(sp->s_buf + sp->s_len - n, '\n')) {
						if (debugmask) {
							fprintf(stderr, "pvmd@%s: %s",
									sp->s_hst->h_name, sp->s_buf);
						}
						*p = 0;
						sp->s_hst->h_result = STRALLOC(sp->s_buf);
					}
#else
					while (p = CINDEX(sp->s_buf, '\n')) {
						*p = 0;
						if (!strncmp(sp->s_buf, "ddpro", 5)) {
							if (debugmask) {
								fprintf(stderr, "stdout@%s: %s\n",
										sp->s_hst->h_name, sp->s_buf);
							}
							sp->s_hst->h_result = STRALLOC(sp->s_buf);
							break;

						} else {
							fprintf(stderr, "stdout@%s: %s\n",
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
						fprintf(stderr, "stdout@%s: %s\n",
								sp->s_hst->h_name, sp->s_buf);
						sp->s_len = 0;
					}
#endif
				} else {
#ifndef WIN32
					if (sp->s_len > 0) {
						fprintf(stderr, "stdout@%s: %s\n",
								sp->s_hst->h_name, sp->s_buf);
						sp->s_len = 0;
					}
#endif
					if (n) {
						fprintf(stderr, "pl_startup() stdout@%s\n",
								sp->s_hst->h_name);
					} else {
						fprintf(stderr, "pl_startup() stdout@%s: EOF\n",
								sp->s_hst->h_name);
					}
#ifndef WIN32
					if (sp->s_elen > 0) {
						fprintf(stderr, "stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						sp->s_elen = 0;
					}
#endif

					/* before failing, check for alternate WIN32 cmd */
					if (sp->s_hst->h_wincmd) {
						if (debugmask) {
							fprintf(stderr,
								"pl_startup() re-trying %s as WIN32\n",
								hp->h_name);
						}
						sp->s_hst->h_cmd = sp->s_hst->h_wincmd;
						sp->s_hst->h_wincmd = 0;
						FREE(sp->s_hst->h_result);
						sp->s_hst->h_result = 0;

						phase1(sp);

						if (!(sp->s_hst->h_result)) {
							/* partially started */
							gettimeofday(&sp->s_bail,
								(struct timezone *) 0);
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
	char *av[16];			/* for rsh args */
	int ac;
	char buf[512];
	int pid = -1;			/* pid of rsh */
	char *p;

#ifdef WIN32
	char do_cmd[128];
	char *val=0;
	FILE   *faked_stream;
#endif

#ifndef NOREXEC
	struct servent *se;
	static u_short execport = 0;

	if (!execport) {
		if (!(se = getservbyname("exec", "tcp"))) {
			fprintf(stderr, "phase1() can't getservbyname(): %s\n", "exec");
			pvmbailout(0);
		}
		execport = se->s_port;
#ifndef WIN32
		endservent();
#endif
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
		fprintf(stderr, "%s\n", hp->h_cmd);

		if (hp->h_wincmd) {
			fprintf(stderr, "\nor, if that command fails, " );
			fprintf(stderr, "for WIN32 hosts try:\n");
			fprintf(stderr, "%s\n\n", hp->h_wincmd);
		}

	/* get version */

		fprintf(stderr, "Type response: ");
		fflush(stderr);
		if (!(fgets(buf, sizeof(buf), stdin))) {
			fprintf(stderr, "host %s read error\n", hn);
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

		if (debugmask) {
			fprintf(stderr, "phase1() trying rsh to %s\n", hn);
		}

	/* fork an rsh to startup the slave pvmd */

#ifdef	IMA_TITN
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, wpfd) == -1
		|| socketpair(AF_UNIX, SOCK_STREAM, 0, rpfd) == -1
		|| socketpair(AF_UNIX, SOCK_STREAM, 0, epfd) == -1) {
			perror("phase1() socketpair");
			goto oops;
		}
#else
#ifndef WIN32
		if (pipe(wpfd) == -1 || pipe(rpfd) == -1 || pipe(epfd) == -1) {
			perror("phase1() pipe");
			goto oops;
		}
#endif
#endif
#ifndef WIN32
		if (debugmask) {
			fprintf(stderr, "phase1() pipes: %d %d %d %d %d %d\n",
					wpfd[0], wpfd[1], rpfd[0], rpfd[1], epfd[0], epfd[1]);
		}
		if ((pid = fork()) == -1) {
			perror("phase1() fork");
			pvmbailout(0);
		}
		if (!pid) {
			(void)dup2(wpfd[0], 0);
			(void)dup2(rpfd[1], 1);
			(void)dup2(epfd[1], 2);
			for (i = getdtablesize(); --i > 2; )
				(void)close(i);
#else
		{
#endif
			ac = 0;

			av[ac++] = pvmgetrsh();
			av[ac++] = hn;
			if (hp->h_login) {
				av[ac++] = "-l";
				av[ac++] = hp->h_login;
			}
#ifdef WIN32
			else {
				hp->h_login=username;
				av[ac++] = "-l";
				av[ac++] = hp->h_login;
			}
#endif
			av[ac++] = hp->h_cmd;
			/* av[ac++] = "-u"; WIN32 */
			/* av[ac++] = username; WIN32 */
			av[ac++] = 0;
			if (debugmask) {
				for (ac = 0; av[ac]; ac++)
					fprintf(stderr, "av[%d]=\"%s\" ", ac, av[ac]);
				fputc('\n', stderr);
			}
#ifndef WIN32
			execvp(av[0], av);
			fputs("phase1() execvp failed\n", stderr);
			fflush(stderr);
			_exit(1);
#else
			
			sp->s_hst->h_result = malloc (512 * sizeof(char));

			if (!rsh(hn,(hp->h_login ? hp->h_login : username),
					hp->h_cmd,sp->s_hst->h_result)) {
				fprintf(stderr, "phase1() rsh failed for host %s\n", hn);
				goto oops;
			}
/* old code using popen ... */

/*
			faked_stream=_popen(do_cmd,"r"); 
			
			if (faked_stream != NULL)
			{
				while (1) {
					fgets(buf,100,faked_stream);
					if (strstr(buf,"ddpro")) {
						if (!val)
							val = malloc(sizeof (buf) * sizeof(char));
						strcpy(val,buf);
						sp->s_hst->h_result=val;
						break;
					
					}
					if (strcmp(buf,"PvmDupHost")){
						if (!val)
							val = malloc(sizeof (buf) * sizeof(char));
						strcpy(val,buf);
						sp->s_hst->h_result=val;
						break;
					}
				}
			}		
*/
#endif
		}
#ifndef WIN32
		(void)close(wpfd[0]);
		(void)close(rpfd[1]);
		(void)close(epfd[1]);
		sp->s_wfd = wpfd[1];
		sp->s_rfd = rpfd[0];
		sp->s_efd = epfd[0];
#endif
	} else {		/* use rexec to start */

#ifdef NOREXEC
#ifdef WIN32
		fprintf(stderr, "slconfg() sorry, no rexec()\n");
#else
		fprintf(stderr, "slconfg() hoster was built without rexec support, can't take passwords\n");
#endif
		goto oops;
#else
		if (debugmask) {
			fprintf(stderr, "phase1() rexec \"%s\"\n", hp->h_cmd);
		}
#ifndef WIN32
		if ((sp->s_wfd = sp->s_rfd = rexec(&hn, execport,
				(hp->h_login ? hp->h_login : username),
				(char*)0, hp->h_cmd, &sp->s_efd))
		== -1) {
			fprintf(stderr, "phase1() rexec failed for host %s\n", hn);
			goto oops;
		}
#else
		sp->s_hst->h_result = malloc (512 * sizeof(char));

		if (!rexec(hn,(hp->h_login ? hp->h_login : username),
					hp->h_cmd,sp->s_hst->h_result))
		{
			fprintf(stderr, "phase1() rexec failed for host %s\n", hn);
			goto oops;
		}
#endif
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


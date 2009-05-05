
static char rcsid[] =
	"$Id: pvmcruft.c,v 1.42 2005/08/22 15:13:20 pvmsrc Exp $";

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
 *	cruft.c
 *
 *	Missing links and other wonk.
 *
 * $Log: pvmcruft.c,v $
 * Revision 1.42  2005/08/22 15:13:20  pvmsrc
 * Added #include <pvmtev.h> for #include global.h...  :-Q
 * 	- submitted by Skipper Hartley <charles.l.hartley@bankofamerica.com>
 * (Spanker=kohl)
 *
 * Revision 1.41  2005/05/18 17:55:55  pvmsrc
 * Added new pvmchkuid() routine...
 * 	- make sure PVM isn't run as root!  :-o
 * (Spanker=kohl)
 *
 * Revision 1.40  2004/03/10 17:01:27  pvmsrc
 * Fixed funny pvmtmpnam() bug...  (ha ha ha :)
 * 	- first pvmd call for socket file will name clash after 10000
 * 		task spawns (t-auth/d-auth)...  (go figure... :-)
 * 	- tweaked to start counter at 0, but thereafter roll over back to 1.
 * (Spanker=kohl)
 *
 * Revision 1.39  2002/04/23 16:33:29  pvmsrc
 * Added overflow check in pvmtmpnam() for PVMTMPNAMLEN string length...
 * 	- just a good idea to check, though it's a lazy check-and-bail...
 * (Spanker=kohl)
 *
 * Revision 1.38  2001/09/27 21:25:10  pvmsrc
 * BEOSCYLD port.
 * 	- submitted by Joe Vitale <vitale@scyld.com>.
 * 	(we renamed it from BEOWULF to BEOSCYLD, but it's his port... :-)
 * (Spanker=kohl)
 *
 * Revision 1.37  2001/09/26 22:04:20  pvmsrc
 * Modified temp file prefix in pvmtmpnam().
 * 	- tmp* -> pvmtmp*.
 * (Spanker=kohl)
 *
 * Revision 1.36  2001/09/26 21:22:59  pvmsrc
 * Append VMID string to PVMD socket file name in pvmdsockfile().
 * 	- if set in PVM_VMID env var.
 * 	- not restricted to be integer as in SGI, can be arbitrary
 * 		string...  heh, heh...  :-}
 * (Spanker=kohl)
 *
 * Revision 1.35  2001/09/25 21:21:02  pvmsrc
 * Yanked "char *pvmgettmp();" decl - now in pvm3.h...
 * (Spanker=kohl)
 *
 * Revision 1.34  2001/02/09 15:23:47  pvmsrc
 * Minor tweak to pvmgettmp() for Markus.
 * 	- check %PVM_TMP% & registry before resorting to %TEMP%.
 * (Spanker=kohl)
 *
 * Revision 1.33  2001/02/07 23:15:51  pvmsrc
 * 2nd Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.32  2001/02/02 14:50:26  pvmsrc
 * Win32 fixes & additions.
 * (Spanker=kohl)
 *
 * Revision 1.31  2000/03/29 20:04:12  pvmsrc
 * Added prototype for pvm_readregistry() routine.
 * (Spanker=sscott)
 *
 * Revision 1.30  2000/02/17 23:12:15  pvmsrc
 * *** Changes for new BEOLIN port ***
 * 	- MPP-like, similar to SP2, etc.
 * 	- submitted by Paul Springer <pls@smokeymt.jpl.nasa.gov>.
 * 	- format-checked & cleaned up by Jeembo...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.29  2000/02/16 23:30:24  pvmsrc
 * Added back in read_pvmregistry() calls.
 * 	- for PVM_ROOT and PVM_TMP, if env vars not present on WIN32.
 * 	- couldn't find reference to PVM_ARCH anywhere, Markus?!
 * (Spanker=kohl)
 *
 * Revision 1.28  2000/02/14 20:32:11  pvmsrc
 * Added new pvmgetrsh() routine
 * 	- checks for PVM_RSH or else uses old RSHCOMMAND interface.
 * 	- one-stop shopping for rsh...
 * (Spanker=kohl)
 *
 * Revision 1.27  2000/02/10 20:45:15  pvmsrc
 * Replaced hard-coded /tmp usage.
 * 	- modified pvmgettmp() to try $PVM_TMP env var for Unix, too.
 * 	- use for TDSOCKNAME / TDSOCKNAME_CSPP in pvmdsockfile().
 * Also in pvmdsockfile():
 * 	- fixed WIN32 chdir() problem, cleaned up string manip.
 * (Spanker=kohl)
 *
 * Revision 1.26  1999/12/13 18:13:55  pvmsrc
 * Moved pvmmatchstring() here from msgbox.c.
 * 	- for use by pvmtester stuff...
 * (Spanker=kohl)
 *
 * Revision 1.25  1999/07/08 19:00:06  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.24  1999/03/16  16:29:56  pvmsrc
 * Oops...  Fixed typing problem for WIN32 in pvmsleep()...
 * (Spanker=kohl)
 *
 * Revision 1.23  1999/03/15  19:06:32  pvmsrc
 * Added new pvmsleep() routine.
 * 	- Unix / Win32, C / Fortran compat...
 * 	- invokes Sleep() on WIN32 (with secs * 1000 :-)~
 * (Spanker=kohl)
 *
 * Revision 1.22  1999/03/05  17:21:15  pvmsrc
 * improved to work with registry/environment on NT/win95-98
 * and devstudio 5/6
 * (Spanker=sscott)
 *
 * Revision 1.21  1999/02/09  23:06:53  pvmsrc
 * Cleaned up temp file stuff (mainly for WIN32).
 * 	- extracted new pvmgettmp() routine for returning tmp directory.
 * (Spanker=kohl)
 *
 * Revision 1.20  1999/02/09  20:10:10  pvmsrc
 * Cleaned up pvmgetroot().
 * 	- some arch cases not properly executed.
 * 	- for WIN32, if read_pvmregistry() fails, give the env var a shot.
 * (Spanker=kohl)
 *
 * Revision 1.19  1998/10/02  15:44:04  pvmsrc
 * Single source code merge of Win32 and Unix code.
 * (Spanker=sscott)
 *
 * Revision 1.18  1998/09/23  15:23:42  pvmsrc
 * 	changes to use WIN32 registry as per Markus.
 * 	ifdef in pvmd.c::colonsep() to include WIN32 and OS2
 * (Spanker=phil)
 *
 * Revision 1.17  1997/12/01  19:20:48  pvmsrc
 * Replaced #ifdef IMA_OS2 fd_set declarations:
 * 	- new #ifdef FDSETNOTSTRUCT.
 * 	- choose between "fd_set foo" and "struct fd_set foo"...
 * (Spanker=kohl)
 *
 * Revision 1.16  1997/11/04  23:19:25  pvmsrc
 * Cleaned up fd_set stuff (hopefully).
 * (Spanker=kohl)
 *
 * Revision 1.15  1997/06/27  19:19:34  pvmsrc
 * Integrated WIN32 changes.
 *
 * Revision 1.14  1997/05/27  14:47:04  pvmsrc
 * New message type added: TM_SHMCONN.
 *
 * Revision 1.13  1997/04/30  21:26:10  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.12  1997/04/21  14:58:23  pvmsrc
 * Changed #ifdefs that checked IMA_RS6K,IMA_SP2MPI & IMA_AIX46K
 * 	to see if select.h was needed into single define NEEDSSELECTH.
 * 	New archs need to set this in conf/
 *
 * Revision 1.11  1997/04/08  19:41:41  pvmsrc
 * Added "DM_RESET", "DM_RESETACK" to taglist2...
 *
 * Revision 1.10  1997/04/08  17:23:33  pvmsrc
 * Damn, Damn, Damn...
 * 	- pvmreset() must be in library side only, pvmds use different
 * 		message buffer mgt, packing and sending routines...  :-Q
 *
 * Revision 1.9  1997/04/08  17:08:17  pvmsrc
 * Oops...  need to pass in caller's tid, lest they be reset, too...
 *
 * Revision 1.8  1997/04/07  20:51:18  pvmsrc
 * Added pvmreset() routine.
 * 	- for use by "pvm" console / XPVM, etc.
 * 	- wipe out tasks, and tell master pvmd to clean up mboxes.
 *
 * Revision 1.7  1997/03/06  21:50:23  pvmsrc
 * Yanked out #includes for <netinet/in.h> and <netinet/tcp.h>.
 * 	- dups with lpvm.h #includes...
 *
 * Revision 1.6  1997/03/06  21:09:47  pvmsrc
 * 	use dclock() for pvmgetclock()
 *
 * Revision 1.5  1997/01/28  19:27:00  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.4  1996/10/25  13:57:57  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.3  1996/10/24  21:30:39  pvmsrc
 * Moved #include "global.h" to below other #includes for typing.
 * Added #include of new "lpvm.h" & replaced explicit externs.
 * Added new trace control constants:
 * 	- TM_TRACER, for pvm_reg_tracer() messages.
 * 	- TC_SETTRACE, TC_SETTRCBUF, TC_SETTRCOPT, for on-the-fly setopt
 * 		control message / handlers.
 *
 * Revision 1.2  1996/09/27  13:17:09  pvmsrc
 * Added special #include of <sys/select.h> for old RS6K (AIX 3.2):
 * 	- fd_set declarations not found in <sys/time.h> on those systems,
 * 		or evidently in SP2MPI, need <sys/select.h>.
 * 	- similar fixes already existed in hoster.c, lpvm.c and pvmd.c.
 *
 * Revision 1.1  1996/09/23  23:44:29  pvmsrc
 * Initial revision
 *
 * Revision 1.10  1995/11/02  16:21:34  manchek
 * made pvmhdump write to pvm log.
 * new switch NEEDSFFS for ffs.
 * added pvmcopyenv, pvmenvinsert and pvmfreeenv
 *
 * Revision 1.9  1995/07/19  21:26:27  manchek
 * new function pvmnametag converts message tag to name or decimal string
 *
 * Revision 1.8  1995/07/18  17:01:35  manchek
 * added function pvmcrcappend to generate 32-bit crcs
 *
 * Revision 1.7  1995/05/24  16:58:42  manchek
 * whacked doofy coding style in pvmhdump
 *
 * Revision 1.6  1995/05/17  16:27:52  manchek
 * added support for CSPP subcomplexes (/tmp file naming)
 *
 * Revision 1.5  1994/11/07  22:48:09  manchek
 * added tweaks for SX3, SCO
 *
 * Revision 1.4  1994/07/18  19:21:05  manchek
 * pvmputenv() matched variable name substrings (need to compare = too)
 *
 * Revision 1.3  1994/06/03  20:38:22  manchek
 * version 3.3.0
 *
 * Revision 1.2  1993/10/04  20:28:27  manchek
 * added pvmdsockfile() function
 *
 * Revision 1.1  1993/08/30  23:26:50  manchek
 * Initial revision
 *
 */

#include <stdio.h>
#ifdef IMA_TITN
#include <bsd/sys/types.h>
#else
#include <sys/types.h>
#endif
#ifndef WIN32
#include <sys/time.h>
#include <sys/socket.h>
#else 
#include <time.h>
#endif
#ifdef NEEDSSELECTH
#include <sys/select.h>
#endif
#include <sys/stat.h>
#include <ctype.h>
#ifdef	SYSVSTR
#include <string.h>
#define	CINDEX(s,c)	strchr(s,c)
#else
#include <strings.h>
#define	CINDEX(s,c)	index(s,c)
#endif
#include <pvm3.h>
#include <pvmproto.h>
#include "pvmalloc.h"
#include "lpvm.h"
#include <pvmtev.h>
#include "global.h"

#ifdef WIN32
char *read_pvmregistry	__ProtoGlarp__(( const char * ));
#endif

/***************
 **  Globals  **
 **           **
 ***************/

#ifndef RSHCOMMAND
#define RSHCOMMAND  "/usr/ucb/rsh"
#endif

char *getenv();

#ifdef WIN32
extern char *username;
#endif

#ifdef IMA_BEOSCYLD
#include <sys/bproc.h>
#endif


/***************
 **  Private  **
 **           **
 ***************/


int
pvmput32(p, n)
	char *p;
	int n;
{
	*p++ = n >> 24;
	*p++ = n >> 16;
	*p++ = n >> 8;
	*p = n;
	return 0;
}


int
pvmput16(p, n)
	char *p;
	int n;
{
	*p++ = n >> 8;
	*p = n;
	return 0;
}


int
pvmput8(p, n)
	char *p;
	int n;
{
	*p = n;
	return 0;
}


int
pvmget32(p)
	char *p;
{
	return ((0xff & p[0]) << 24)
	+ ((0xff & p[1]) << 16)
	+ ((0xff & p[2]) << 8)
	+ (0xff & p[3]);
}


int
pvmget16(p)
	char *p;
{
	return ((0xff & p[0]) << 8) + (0xff & p[1]);
}


int
pvmget8(p)
	char *p;
{
	return 0xff & *p;
}


#ifdef NOGETDTBLSIZ

#ifdef IMA_TITN

int
getdtablesize()
{
	return 64;
}

#else	/*IMA_TITN*/

#ifdef IMA_SX3

int
getdtablesize()
{
	return 64*8;
}

#else /*IMA_SX3*/

#ifdef WIN32
int
getdtablesize()
{
	return 64;
}
#else
#include <unistd.h>
int
getdtablesize()
{
	return sysconf(_SC_OPEN_MAX);
}
#endif

#endif /*IMA_SX3*/

#endif	/*IMA_TITN*/

#endif /*NOGETDTBLSIZ*/

#ifdef	NEEDSFFS
int
ffs(x)
	int x;
{
	int n = 1, m = 1;

	if (!x)
		return 0;
	while (!(x & m)) {
		m += m;
		n++;
	}
	return n;
}

#endif

#ifndef IMA_WIN32_WATCOM
extern char **environ;
#endif

/*	pvmputenv()
 *
 *	Replacement for systems that don't have putenv.
 *	Tries to be conservative, i.e. doesn't assume that environ
 *	points to malloc'd space.
 *
 *	String must never be freed...
 */

int
pvmputenv(s)
	char *s;	/* string of form "NAME=Value" */
{
	char *p;					/* gp */
	int l;						/* length of env var name or env */
	static char **myen = 0;		/* last environ that i created */
	char **newen;				/* new env if extending */
	char **ep, **ep2;			/* gp */

	/* sanity check the string and locate the '=' */

	if (!s)
		return -1;
	for (p = s; *p && *p != '='; p++);
	if (p == s || !*p)
		return -1;
	l = p - s + 1;	/* must include '=' */

	/* search current environ for name */

	for (ep = environ; *ep; ep++)
		if (!strncmp(*ep, s, l))
			break;

	if (*ep) {	/* already there, replace it */
		*ep = s;

	} else {	/* not there, must extend environ */

	/* length needed is? */

		for (l = 2, ep = environ; *ep; l++, ep++);

		if (environ == myen) {	/* mine, reallocate */
			if (!(newen = (char**)realloc((char *)environ,
					(unsigned)(l * sizeof(char*)))))
				return -1;

		} else {	/* not mine, alloc new and copy */
			if (!(newen = (char**)malloc((unsigned)(l * sizeof(char*)))))
				return -1;
			for (ep = environ, ep2 = newen; *ep2++ = *ep++; );
		}
		newen[l - 2] = s;
		newen[l - 1] = 0;
		environ = myen = newen;
	}
	return 0;
}


/*	pvmcopyenv()
 *
 *	Copy an environment list (null-terminated vector of strings).
 *	This is just like environ, but we own all the strings.
 *	Can also be used to create an empty list, by passing a null pointer.
 *
 *	Returns pointer to environment or NULL if malloc fails.
 */

char **
pvmcopyenv(anyep)
	char **anyep;		/* old environment to copy, or NULL */
{
	char **newep;
	int i, n;

	if (anyep) {
		for (n = 0; anyep[n]; n++) ;
		if (newep = TALLOC(n + 1, char *, "env")) {
			newep[n] = 0;
			for (i = 0; i < n; i++) {
				if (!(newep[i] = STRALLOC(anyep[i]))) {
					while (i-- > 0)
						PVM_FREE(newep[i]);
					PVM_FREE(newep);
					newep = 0;
					break;
				}
			}
		}

	} else {
		if (newep = TALLOC(1, char *, "env"))
			newep[0] = 0;
	}
	return newep;
}


/*	pvmenvinsert()
 *
 *	Copy a string into an environment list (made by pvmcopyenv).
 *	List must be one of ours, since we malloc and free the strings.
 *
 *	Returns zero or success, or -1 if malloc fails.
 *	May update pointer to environment on return.
 */

int
pvmenvinsert(anep, s)
	char ***anep;		/* environment pointer, may change (NOT environ) */
	char *s;			/* string of form "NAME=Value" */
{
	char **ep;
	char **newep;
	char *p;
	int l;						/* length of env var name or env */

	newep = *anep;

	if (!s || !(p = CINDEX(s, '=')))
		return -1;
	l = p - s + 1;			/* must include the '=' */

	/* search environ for name */

	for (ep = newep; *ep; ep++)
		if (!strncmp(*ep, s, l))
			break;

	if (*ep) {				/* already there, replace it */
		PVM_FREE(*ep);
		*ep = STRALLOC(s);

	} else {				/* not there, must extend environ */

	/* length needed is? */

		for (l = 2, ep = newep; *ep; l++, ep++);

		if (!(newep = TREALLOC((char *)newep, l, char *)))
			return -1;

		newep[l - 2] = STRALLOC(s);
		newep[l - 1] = 0;
		*anep = newep;
	}
	return 0;
}


/*	pvmfreeenv()
 *
 *	Free an environment list (made by pvmcopyenv).
 */

int
pvmfreeenv(anep)
	char **anep;
{
	char **p;

	for (p = anep; *p; p++)
		PVM_FREE(*p);
	PVM_FREE(anep);
	return 0;
}


/*	pvmxtoi()
 *
 *	Yet another version of ascii hex to integer
 */

pvmxtoi(p)
	char *p;
{
	int i = 0;
	char c;

	if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X'))
		p += 2;
	while (isxdigit(c = *p++)) {
		i = (i << 4) + c - (isdigit(c) ? '0' : (isupper(c) ? 'A' : 'a') - 10);
	}
	return i;
}


/*	pvmstrtoi()
 *
 *	Convert string in decimal, hex or octal to integer.
 */

int
pvmstrtoi(p)
	char *p;
{
	int i = 0;
	char c;

	if (*p == '0') {
		p++;
		if (*p == 'x' || *p == 'X')
			while (isxdigit(c = *++p))
				i = (i << 4) + c
						- (isdigit(c) ? '0' : (isupper(c) ? 'A' : 'a') - 10);
		else
			while (c = *p++, c >= '0' && c <= '7')
				i = (i << 3) + c - '0';
		return i;
	} else
		return (int)atoi(p);
}


#ifndef	I860_NODE

/*	hex_inadport()
 *
 *	Takes a string of format 00000000:0000 and returns a sockaddr_in.
 */

void
hex_inadport(s, sad)
	char *s;
	struct sockaddr_in *sad;
{
	sad->sin_addr.s_addr = htonl((unsigned)pvmxtoi(s));
	s = (s = CINDEX(s, ':')) ? s + 1 : "";
	sad->sin_port = htons((unsigned short)pvmxtoi(s));
}


/*	inadport_decimal()
 *
 *	Returns printable string corr. to sockaddr_in with addr in decimal.
 */

char *
inadport_decimal(sad)
	struct sockaddr_in *sad;
{
	static char buf[32];
	int a;

	a = ntohl(0xffffffff & sad->sin_addr.s_addr);
	sprintf(buf, "%d.%d.%d.%d:%d",
			0xff & (a >> 24),
			0xff & (a >> 16),
			0xff & (a >> 8),
			0xff & a,
			0xffff & (int)ntohs(sad->sin_port));
	return buf;
}


/*	inadport_hex()
 *
 *	Returns printable string corr. to sockaddr_in with addr in hex.
 */

char *
inadport_hex(sad)
	struct sockaddr_in *sad;
{
	static char buf[16];
	int a;

	a = ntohl(0xffffffff & sad->sin_addr.s_addr);
	sprintf(buf, "%08x:%04x", a, 0xffff & (int)ntohs(sad->sin_port));
	return buf;
}

#endif	/*I860_NODE*/


#ifdef	NOTMPNAM
char *
pvmtmpnam(buf)
	char *buf;
{
	static int f = 0;
	static int n = 0;
	static char scratch[255];

	char *pvmtmp;

	if (!buf)
		buf = scratch;

	pvmtmp = pvmgettmp();

#ifndef WIN32
	sprintf(buf, "%s/pvmtmp%06d.%d", pvmtmp, getpid(), n);
#else
	sprintf(buf, "%s\\pvmtmp%06d.%d", pvmtmp, getpid(), n);
#endif

	/* Silly Hack to avoid stomping on pvmd's first pvmtmpnam()
		call to get it's socket file... :-) */
	n = 1 + (f++ % 10000);

	/* Better Check for String Overflow... */
	/* (Late Check is Better Than No Check... :-) */

	if ( strlen( buf ) >= PVMTMPNAMLEN ) {
		pvmlogperror("Whoa!  PVMTMPNAMLEN Overflow in pvmtmpnam()...");
		pvmlogprintf("<%s> = %d >= %d", buf, strlen( buf ),
				PVMTMPNAMLEN);
		pvmbailout(0);
		exit(1);		/* the other meaning of bail out */
	}

	return buf;
}
#endif	/*NOTMPNAM*/


#ifdef IMA_BEOSCYLD

/*
 * Implement a version of gethostname() that gives the "right" answer
 * for a Scyld Beowulf cluster; calling bproc_currnode() here is why
 * both the pvm daemon and the pvm library are linked with libbproc
 */

int
gethostname(name, len)
	char * name;
	size_t len;
{
	int retval;

	retval = snprintf(name,len,".%d",bproc_currnode());

	return( (retval == -1) ? retval : 0 );
}

#endif

pvmhdump(cp, n, pad)
	char *cp;		/* bytes */
	int n;			/* length */
	char *pad;		/* tag string */
{
	static char *buf = 0;
	static int bufl = 0;

	int i, l;
	char *r;

	pad = pad ? pad : "";
	l = strlen(pad) + 50;
	if (l > bufl) {
		if (buf)
			PVM_FREE(buf);
		if (!(buf = TALLOC(l, char, "hdump"))) {
			bufl = 0;
			pvmlogerror("pvmhdump() malloc failed\n");
			return 1;
		}
		bufl = l;
	}

	r = buf;
	for (i = 0; n-- > 0; i = (i + 1) & 15) {
		sprintf(r, "%s%02x",
			i ? " " : pad,
			0xff & *cp++);
		r += strlen(r);
		if (!n || i == 15) {
			strcpy(r, "\n");
			pvmlogerror(buf);
			r = buf;
		}
	}

	return 0;
}


/*	pvmgethome()
 *
 *	Return path of user homedir.
 */

char *
pvmgethome()
{
	static char *hd = 0;
#ifndef WIN32 /* which registry should we use otherwise ? */
	if (!hd) {
		if (hd = getenv("HOME")) {
			hd = STRALLOC(hd);

		} else {
			pvmlogerror("warning: can't getenv(HOME), using /\n");
			hd = "/";
		}
	}
#else
	hd="\\";
#endif
	return hd;
}


/*	pvmgetroot()
 *
 *	Return absolute path of PVM top level directory.
 */

char *
pvmgetroot()
{
	static char *rd = 0;

	if (!rd) {

#ifdef IMA_CSPP

		struct stat buf;

		rd = STRALLOC("/usr/convex/pvm");
		if (stat(rd, &buf) == -1) {
			pvmlogperror(
				"Unable to default PVM_ROOT to /usr/convex/pvm");
			pvmbailout(0);
			exit(1);		/* the other meaning of bail out */
		}
		pvmputenv("PVM_ROOT=/usr/convex/pvm");
		pvmlogerror("Defaulting PVM_ROOT to /usr/convex/pvm");

#else

#ifdef CYGWIN
		rd = getenv("PVM_ROOT_U");
#else
		rd = getenv("PVM_ROOT");
#endif

#ifdef WIN32
		if (!rd)
			rd = read_pvmregistry("PVM_ROOT");
#endif

#endif

		if (!rd) {
			pvmlogerror("PVM_ROOT environment variable not set.\n");
			pvmbailout(0);
			exit(1);		/* the other meaning of bail out */
		}
	}

	return rd;
}


/*	pvmgettmp()
 *
 *	Return absolute path of PVM temporary directory.
 */

char *
pvmgettmp()
{
	static char *td = 0;

	if (!td) {
#ifndef WIN32
		if ( !(td=getenv("PVM_TMP")) )
#ifdef IMA_BEOLIN
			td = "/tmps";	/* the shared tmp */
#else
			td = "/tmp";
#endif
#else
		if ( !(td=getenv("PVM_TMP")) ) {
			if ( !(td=read_pvmregistry("PVM_TMP")) ) {
				if ( !(td=getenv("TEMP")) ) {
					td = "C:\\TEMP";
					fprintf(stderr,
							"Could not get %%PVM_TMP%% or %%TEMP%% ");
					fprintf(stderr, "-> using \"%s\".\n", td);
					fprintf(stderr,
							"Please check your PVM installation.\n");
				}
			}
		}
#endif
	}

	return td;
}


/*	pvmgetrsh()
 *
 *	Return absolute path of the rsh executable for the local system.
 */

char *
pvmgetrsh()
{
	static char *rd = 0;

	if (!rd) {
		if ( !(rd=getenv("PVM_RSH")) )
			rd = RSHCOMMAND;
	}

	return rd;
}


/*	pvmgetpvmd()
 *
 *	Return absolute path of pvmd startup script.
 */

char *
pvmgetpvmd()
{
	char *r;
	static char *pp = 0;

	if (!pp) {
		if (PVMDFILE[0] == '/')
			pp = PVMDFILE;
		else {
			r = pvmgetroot();
			pp = TALLOC(strlen(r) + strlen(PVMDFILE) + 2, char, "pvmdpath");
			sprintf(pp, "%s/%s", r, PVMDFILE);
		}
	}
	return pp;
}


/*	pvmdsockfile()
 *
 *	Returns name of file where pvmd advertises its sockaddr.
 *	Assumes global pvm_useruid is set before calling.
 */

char *
pvmdsockfile()
{
	static char buf[255];

	char hna[128];
	char *pvmtmp;
	char *p;

#ifdef	IMA_CSPP
	int scid = get_scid();	/* default (system) subcomplex ID is 1 */
#endif

	pvmtmp = pvmgettmp();

#ifdef	SHAREDTMP

	if (gethostname(hna, sizeof(hna)-1) == -1) {
		pvmlogerror("pvmdsockfile() can't gethostname()\n");
		return (char*)0;
	}
	if (p = CINDEX(hna, '.'))
		*p = 0;

#ifdef	IMA_CSPP
	if (scid > 1)
		(void)sprintf(buf, TDSOCKNAME_CSPP,
				pvmtmp, pvm_useruid, scid, hna);
	else
#endif
		(void)sprintf(buf, TDSOCKNAME, pvmtmp, pvm_useruid, hna);

#else

#ifdef	IMA_CSPP
	if (scid > 1)
		(void)sprintf(buf, TDSOCKNAME_CSPP, pvmtmp, pvm_useruid, scid);
	else
#endif
#ifdef WIN32
		sprintf( buf, "%s\\pvmd.%s", pvmtmp, username );
#else
		(void)sprintf(buf, TDSOCKNAME, pvmtmp, pvm_useruid);
#endif

#endif

	/* Append a Virtual Machine ID, If Set */
	if ( p = getenv("PVM_VMID") ) {
		strcat( buf, "." );
		strcat( buf, p );
	}

	return buf;
}


#ifdef IMA_CSPP
#include <sys/cnx_pattr.h>

/*	get_scid ()
 *
 *	(Convex CSPP only)
 *	Returns the subcomplex identifier for the calling process
 */

int
get_scid ()
{
	static int my_scid = -1;

	int rc;
	struct cnx_pattributes pattrib;

	if (my_scid < 0) {
		my_scid = 0;
		if ((rc = cnx_getpattr(getpid(), CNX_PATTR_SCID, &pattrib)) != 0) {
			pvmlogperror ("get_scid: Unable to get scid");
			return 0;
		}
		my_scid = pattrib.pattr_scid;
	}
	return my_scid;
}
#endif	/*IMA_CSPP*/


#define	PVMCRCPOLY	0xedb88320

unsigned int
pvmcrcappend(buf, len, crc)
	char *buf;			/* buffer to append to crc value */
	int len;			/* length */
	unsigned int crc;	/* initial value */
{
	static unsigned int crctbl[256];
	static int once = 1;

	if (once) {
		int i, j;
		unsigned int v;

		for (i = 0; i < 256; i++) {
			v = i;
			for (j = 8; j--; )
				v = v & 1 ? (v >> 1) ^ PVMCRCPOLY : v >> 1;
			crctbl[i] = v;
		}
		once = 0;
	}

	while (len-- > 0)
		crc = ((crc >> 8) & 0x00ffffff) ^ crctbl[(crc ^ *buf++) & 0xff];
	return crc;
}


static char *taglist1[] = {
	"TM_CONNECT",
	"TM_CONN2",
	"TM_EXIT",
	"TM_ADDHOST",
	"TM_DELHOST",
	"TM_CONFIG",
	"TM_MSTAT",
	"TM_HALT",
	"TM_TICKLE",
	"TM_SPAWN",
	"TM_PSTAT",
	"TM_SENDSIG",
	"TM_TASK",
	"TM_MCA",
	"TM_NOTIFY",
	"TM_DB",
	"TM_SCHED",
	"TM_TASKER",
	"TM_HOSTER",
	"TM_TRACER",
	"TM_HOSTSYNC",
	"TM_SETOPT",
	"TM_GETOPT",
	"TM_CONTEXT",
	"TM_SHMCONN",
};

static char *taglist2[] = {
	"DM_ADD", "DM_ADDACK",
	"DM_EXEC", "DM_EXECACK",
	"DM_SENDSIG",
	"DM_HTUPD", "DM_HTUPDACK", "DM_HTCOMMIT",
	"DM_SLCONF",
	"DM_STARTACK",
	"DM_TASK", "DM_TASKACK",
	"DM_DELHOST", "DM_DELHOSTACK",
	"DM_NULL",
	"DM_TASKOUT",
	"DM_PSTAT", "DM_PSTATACK",
	"DM_HALT",
	"DM_MCA",
	"DM_NOTIFY", "DM_NOTIFYACK",
	"DM_DB", "DM_DBACK",
	"DM_RESET", "DM_RESETACK",
	"DM_HTDEL",
	"DM_HOSTSYNC", "DM_HOSTSYNCACK",
};

static char *taglist3[] = {
	"TC_CONREQ",
	"TC_CONACK",
	"TC_TASKEXIT",
	"TC_NOOP",
	"TC_OUTPUT",
	"TC_SETTRACE",
	"TC_SETTMASK",
	"TC_SETTRCBUF",
	"TC_SETTRCOPT",
	"TC_SHMAT",
	"TC_OUTPUTX",
	"TC_SIBLINGS",
};

static char *taglist4[] = {
	"SM_SPAWN",
	"SM_EXEC", "SM_EXECACK",
	"SM_TASK",
	"SM_CONFIG",
	"SM_ADDHOST",
	"SM_DELHOST",
	"SM_ADD", "SM_ADDACK",
	"SM_NOTIFY",
	"SM_TASKX",
	"SM_HOSTX",
	"SM_HANDOFF",
	"SM_SCHED",
	"SM_STHOST", "SM_STHOSTACK",
	"SM_STTASK",
};

struct nametag {
	int first;
	int last;
	char **names;
};

static struct nametag nametaglist[] = {
	{ TM_CONNECT, TM_SHMCONN, taglist1 },
	{ DM_ADD, DM_HOSTSYNCACK, taglist2 },
	{ TC_CONREQ, TC_SIBLINGS, taglist3 },
	{ SM_SPAWN, SM_STTASK, taglist4 },
};


/*	pvmnametag()
 *
 *	Convert message tag to symbolic name for reserved tags.
 *	Returns name string or decimal tag if not found.
 *	If found is nonnull, return whether found.
 */

char *
pvmnametag(tag, found)
	int tag;
	int *found;
{
	static char buf[32];

	int i, j;

	for (i = sizeof(nametaglist)/sizeof(nametaglist[0]); i-- > 0; )
		if (tag >= nametaglist[i].first && tag <= nametaglist[i].last) {
			j = tag - nametaglist[i].first;
			if (found)
				*found = 1;
			return nametaglist[i].names[j];
		}
	sprintf(buf, "%d", tag);
	if (found)
		*found = 0;
	return buf;
}


/*	debug_flags()
 *
 *	Convert debug mask to flag names
 */

static char *dflgs[] = {
	"pkt",	/* 1 */
	"msg",	/* 2 */
	"tsk",	/* 4 */
	"slv",	/* 8 */
	"hst",	/* 10 */
	"sel",	/* 20 */
	"net",	/* 40 */
	"mpp",	/* 80 */
	"sch",	/* 100 */
	"app",	/* 200 */
	"wai",	/* 400 */
	"mem",	/* 800 */
	"sem",	/* 1000 */
	"lck",	/* 2000 */
	"rtg",	/* 4000 */
	"mbx"	/* 8000 */
};

char *
debug_flags(mask)
	int mask;
{
	static char buf[64];
	int bit, i;

	buf[0] = 0;
	for (bit = 1, i = 0; i < sizeof(dflgs)/sizeof(dflgs[0]); i++, bit *= 2)
		if (mask & bit) {
			if (buf[0])
				strcat(buf, ",");
			strcat(buf, dflgs[i]);
		}
	return buf;
}


/*	print_fdset()
 *
 *	Print an fd_set in human-readable format.
 */

print_fdset(pad, n, f)
	char *pad;			/* label at head */
	int n;				/* max fd + 1 */
#ifdef FDSETNOTSTRUCT
	fd_set *f;			/* fd set */
#else
	struct fd_set *f;	/* fd set */
#endif
{
	int i;
	char *s = "";

	pvmlogprintf("%s", pad);
	for (i = 0; i < n; i++)
		if (FD_ISSET(i, f)) {
			pvmlogprintf("%s%d", s, i);
			s = ",";
		}
	pvmlogprintf("\n");
	return 0;
}


#ifdef I860_NODE
int
gettimeofday(tp, tzp)
	struct timeval *tp;
	struct timezone *tzp;
{
	double t, dclock();

	t = dclock();
	tp->tv_sec = t;
	tp->tv_usec = (t - tp->tv_sec)*1.0E06;
	return 0;
}
#endif


/*	pvmgetclock()
 *
 *	Use this whenever we need a clock value in seconds and microseconds.
 *	Return values shouldn't be compared to those from gettimeofday,
 *	because it doesn't necessarily return the actual time of day.
 */

int
pvmgetclock(tm)
	struct timeval *tm;
{
#if defined(IMA_PGON)
	double dclock();
	double t;

	t = dclock();
	tm->tv_sec = t;
	tm->tv_usec = (t - tm->tv_sec)*1.0E06;
#else
	return gettimeofday(tm, (struct timezone*)0);
#endif
}


/*  pvmsleep()
 *
 *   A consistent way to invoke sleep(), from:
 *     Unix / Windows, C / Fortran
 */

int
pvmsleep( secs )
int secs;
{
	int ret;
#ifdef WIN32
	Sleep( secs * 1000 );
	ret = secs;
#else
	ret = (int) sleep( secs );
#endif
	return( ret );
}


/*	pvmmatchstring()
 *
 *	Replacement for GNU Regex pattern matching.
 */

int
pvmmatchstring( str, pattern )
char *str;
char *pattern;
{
	char *start;
	char *save;
	char *x;
	char *y;

	/* Find Start of Search String (Ignore Preceding '*'s) */

	start = pattern;

	while ( *start == '*' && *start != '\0' )
		start++;

	/* Null Search - Always Matches */

	if ( *start == '\0' )
		return( 1 );

	/* Initialize Pointers */

	x = str;

	y = start;

	/* Search Line for Start of Search String */

	while ( *x != '\0' )
	{
		/* Starting Match Found...  Check it Out */

		if ( *x == *y )
		{
			/* Save Continuation Pointer */

			save = x + 1;

			/* Traverse Line Until No Longer Matches */

			while ( *x != '\0' && *y != '\0' && *x == *y )
			{
				x++;
				y++;

				/* Nested '*' Search Expansion */

				if ( *y == '*' )
				{
					if ( pvmmatchstring( x, y + 1 ) )
						return( 1 );

					else
						break;
				}

				/* Skip Over '\' for Escaped '*'s */

				else if ( *y == '\\' && *(y+1) == '*' )
					y++;
			}

			/* It's a Match! */

			if ( *y == '\0' )
				return( 1 );

			/* Reset and Continue */

			x = save;

			y = start;
		}

		else
			x++;
	}

	return( 0 );
}

/* pvmchkuid()
 *
 * Verify that PVM isn't running as user "root"...!
 * 
 * This often creates all sorts of havoc for first-time PVM
 * users, so give them a big fat warning message and bail out...
 * (If they *really* need to run as root, force them to set
 * the $PVM_ALLOW_ROOT environment variable...  this will
 * "root" out the novice users...  tee-hee... :-)
 */

void
pvmchkuid( uid )
int uid;
{
	char *p;

	/* We are Running as User "root" - Bad Baby, Bad Baby...! :-o */
	if ( !uid )
	{
		/* Let the Brave User Shoot Self in Foot... :-) */
		if ( (p = getenv( "PVM_ALLOW_ROOT" )) != NULL )
			return;

		printf( "\n" );
		printf( "*** Error: PVM is being run as user \"root\" ***!\n" );
		printf( "\n" );
		printf( "This is not a good idea, and often doesn't work!\n" );
		printf( "\n" );
		printf( "The root user cannot typically rsh/ssh to remote\n" );
		printf( "computers over the network, to add them as hosts\n" );
		printf( "to the PVM virtual machine.\n" );
		printf( "\n" );
		printf( "You can install PVM in a system-wide directory,\n" );
		printf( "with the files *owned* by user \"root\",\n" );
		printf( "but you need to *run* PVM as a *regular* user!\n" );
		printf( "\n" );
		printf( "\n" );
		printf( "*If* you know what you're doing, and you are\n" );
		printf( "willing to accept the security risks, then you\n" );
		printf( "can enable root execution of PVM by setting the\n" );
		printf( "%s environment variable in your shell.\n",
				"PVM_ALLOW_ROOT" );
		printf( "\n" );
		printf( "Please send any inquiries regarding this feature\n" );
		printf( "to:  pvm@csm.msr.ornl.gov\n" );
		printf( "\n" );

		pvmbailout(0);
		exit(1);		/* the other meaning of bail out */
	}
}


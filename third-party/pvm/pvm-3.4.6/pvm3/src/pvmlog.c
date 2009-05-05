
static char rcsid[] =
	"$Id: pvmlog.c,v 1.29 2009/01/30 21:22:46 pvmsrc Exp $";

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
 *	pvmlog.c
 *
 *	Error logging fac.
 *
 * $Log: pvmlog.c,v $
 * Revision 1.29  2009/01/30 21:22:46  pvmsrc
 * Fixed up insecure usage of fopen() for creating/writing/appending files.
 * 	- replace with open(...O_EXCL...) and fdopen()...
 * (Spanker=kohl)
 *
 * Revision 1.28  2005/11/16 16:26:23  pvmsrc
 * Yanked old debugging output (with compiler error... :-Q)
 * 	in pvmlogperror()...  D-Oh.
 * (Spanker=kohl)
 *
 * Revision 1.27  2005/06/15 15:06:27  pvmsrc
 * Fixed up formatting for truncated case in vpvmlogprintf()...
 * 	- need to assume a '\n'...  else garbage ensues...
 * (Spanker=kohl)
 *
 * Revision 1.26  2005/06/15 15:02:47  pvmsrc
 * Fixed potential buffer overflow in goofy AMD64 fix to vpvmlogprintf().
 * 	- patch submitted by Lasse Makholm <lasse.makholm@nokia.com>.
 * (Spanker=kohl)
 *
 * Revision 1.25  2004/05/26 15:56:36  pvmsrc
 * Attempt to fix yet-another-x86_64 bug...
 * 	- looks like varargs stuff is one-time-use-only...
 * 		-> replace repeated vfprintf() calls with one vsprintf() call...
 * (Spanker=kohl)
 *
 * Revision 1.24  2004/02/09 17:17:02  pvmsrc
 * Added pvmlogperror debug output for AMD64...
 * 	- the arch that wouldn't die...  er, run without seg faulting...  :)
 * (Spanker=kohl)
 *
 * Revision 1.23  2004/02/03 21:40:57  pvmsrc
 * Added check for NULL strerror() return...
 * 	- to chase down AMD64 problem...
 * (Spanker=kohl)
 *
 * Revision 1.22  2004/01/14 19:12:35  pvmsrc
 * Added (char *) cast to strerror() assignment...
 * (Spanker=kohl)
 *
 * Revision 1.21  2001/09/26 21:23:56  pvmsrc
 * Added Handling for Optional Virtual Machine ID.
 * 	- append virtual machine ID string to pvmd log file, too,
 * 		if set in PVM_VMID env var.
 * 	- not restricted to be integer as in SGI, can be arbitrary
 * 		string...  heh, heh...  :-}
 * (Spanker=kohl)
 *
 * Revision 1.20  2001/09/25 21:21:05  pvmsrc
 * Yanked "char *pvmgettmp();" decl - now in pvm3.h...
 * (Spanker=kohl)
 *
 * Revision 1.19  2001/05/11 18:58:13  pvmsrc
 * Added use of new "USESTRERROR" define.
 * 	- uses strerror() function instead of sys_errlist/sys_nerr
 * 		(which aren't public globals on some new operating systems).
 * (Spanker=kohl)
 *
 * Revision 1.18  2000/03/30 18:01:18  pvmsrc
 * Damn.  Max PVMD Log Size Checking Whacked!
 * 	- everything goes through pvmlogprintf() now, transferred old
 * 		code from pvmlogerror() to truncate logging...
 * (Spanker=kohl)
 *
 * Revision 1.17  2000/02/10 20:46:43  pvmsrc
 * Replaced hard-coded /tmp usage with PVMDLOGFILE.
 * 	- use pvmgettmp() routine now to determine PVM temp dir.
 * 	- also, eliminated goofy pvmtmpspec stuff for WIN32, cleaned up
 * 		string manip...
 * (Spanker=kohl)
 *
 * Revision 1.16  1999/12/10 21:29:40  pvmsrc
 * Added pvmdlogmax = pvmdlogmax for picky SGI compiler warning...  :-Q
 * (Spanker=kohl)
 *
 * Revision 1.15  1999/07/08 19:00:13  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.14  1999/02/09  23:06:54  pvmsrc
 * Cleaned up temp file stuff (mainly for WIN32).
 * 	- extracted new pvmgettmp() routine for returning tmp directory.
 * (Spanker=kohl)
 *
 * Revision 1.13  1998/11/20  20:06:42  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (spanker=sscott)
 *
 * Revision 1.12  1998/10/02  15:44:09  pvmsrc
 * Single source code merge of Win32 and Unix code.
 * (Spanker=sscott)
 *
 * Revision 1.11  1998/09/23  15:23:48  pvmsrc
 * 	changes to use WIN32 registry as per Markus.
 * 	ifdef in pvmd.c::colonsep() to include WIN32 and OS2
 * (Spanker=phil)
 *
 * Revision 1.10  1998/01/28  20:09:00  pvmsrc
 * Added new -DSYSERRISCONST define.
 * 	- for const char *sys_errlist...
 * (Spanker=kohl)
 *
 * Revision 1.9  1998/01/28  19:13:59  pvmsrc
 * Added new IMA_LINUXHPPA to #if cases...
 * (Spanker=kohl)
 *
 * Revision 1.8  1997/12/29  19:40:03  pvmsrc
 * 	Made char *em a const char *em for LINUX machines.
 * 	(Redhat 5.0/4.2 compatible.)
 * (Spanker=phil)
 *
 * Revision 1.7  1997/11/04  23:20:42  pvmsrc
 * Added IMA_LINUXALPHA to const char * error log stuff.
 * (Spanker=kohl)
 *
 * Revision 1.6  1997/08/29  13:35:18  pvmsrc
 * OS2 Port Submitted by Bohumir Horeni, horeni@login.cz.
 * (Spanker=kohl)
 *
 * Revision 1.5  1997/07/11  18:44:52  pvmsrc
 * WIN32 fix for console bug (quit caused halt) - Markus.
 *
 * Revision 1.4  1997/06/27  20:04:54  pvmsrc
 * Integrated WIN32 changes.
 *
 * Revision 1.3  1997/04/30  21:32:48  pvmsrc
 * SGI Compiler Warning Cleanup.
 * 	- fixed up return codes for vpvmlogprintf().
 *
 * Revision 1.2  1997/01/28  19:27:29  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:44:39  pvmsrc
 * Initial revision
 *
 * Revision 1.6  1995/07/28  16:41:00  manchek
 * wrap HASERRORVARS around errno declarations
 *
 * Revision 1.5  1995/05/17  16:44:56  manchek
 * added CSPP subcomplex support (/tmp file names include proc. id).
 * use PVMDLOGMAX envar to set logging limit to /tmp/pvml.uid
 *
 * Revision 1.4  1994/06/03  20:38:25  manchek
 * version 3.3.0
 *
 * Revision 1.3  1993/11/30  15:53:59  manchek
 * added timestamp option
 *
 * Revision 1.2  1993/10/04  20:31:21  manchek
 * renamed useruid to pvm_useruid for compat with libpvm.
 * added alternate logfile name if SHAREDTMP is defined
 *
 * Revision 1.1  1993/08/30  23:26:51  manchek
 * Initial revision
 *
 */

#include <stdio.h>
#include <pvm3.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#ifdef	SYSVSTR
#include <string.h>
#define	CINDEX(s,c)	strchr(s,c)
#else
#include <strings.h>
#define	CINDEX(s,c)	index(s,c)
#endif
#if defined (__STDC__) || defined(IMA_WIN32_WATCOM)
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#ifndef PVMDLOGMAX
#define PVMDLOGMAX 1000000			/* (approx) max chars to log to file */
#endif

/* pvmd log file name */

#ifdef	SHAREDTMP
#define	PVMDLOGFILE	"%s/pvml.%d.%s"
#else
#ifndef WIN32
#define	PVMDLOGFILE	"%s/pvml.%d"
#else
#define PVMDLOGFILE "%s\\pvml.%s"  /* for default */
#endif
#endif

#ifdef	IMA_CSPP
#ifdef	SHAREDTMP
#define	PVMDLOGFILE_CSPP	"%s/pvml.%d.%d.%s"
#else
#define	PVMDLOGFILE_CSPP	"%s/pvml.%d.%d"
#endif

int get_scid();
#endif	/*IMA_CSPP*/


/***************
 **  Globals  **
 **           **
 ***************/

char *getenv();

#ifndef HASERRORVARS
extern int errno;					/* from libc */
#ifndef USESTRERROR
extern int sys_nerr;
extern char *sys_errlist[];
#endif
#endif

#ifdef IMA_OS2
#include <stdlib.h>					/* ERRORVARS */
#endif

extern int pvmmytid;				/* from pvmd.c */
extern int pvmmyupid;				/* from pvmd.c */
extern int pvm_useruid;				/* from pvmd.c */
#ifdef WIN32
extern char *username;				/* from pvmd.c */
#endif

int log_fd = -1;					/* error log file */


/***************
 **  Private  **
 **           **
 ***************/

static int log_how = 1;		/* how much error logging we can do */
FILE *log_ff = 0;
static int log_alrdy = 0;	/* how much already written to file */
static int pvmdlogmax = PVMDLOGMAX;
static char *toomuch = "\n*** logging truncated\n";
static int atnewline = 1;	/* on new log line (XXX ick) */


/*	pvmsetlog()
*
*	Set new error logging level.
*	If how & 1, write to stderr
*	If how & 2, write to log file in /tmp
*/

int
pvmsetlog(how)
	int how;
{
	char buf[255];
	char hna[128];
	char *pvmtmp;
	char *p;

#ifdef	IMA_CSPP
	int scid = get_scid();	/* default (system) subcomplex ID is 1) */
					/* differentiate logfile when not on system s/c */
#endif

	if (2 & how & ~log_how) {
		pvmtmp = pvmgettmp();
#ifdef	SHAREDTMP
		if (gethostname(hna, sizeof(hna)-1) == -1) {
			pvmlogerror("pvmsetlog() can't gethostname()\n");
			return 0;
		}
		if (p = CINDEX(hna, '.'))
			*p = 0;
#ifdef	IMA_CSPP
		if (scid > 1)
			(void)sprintf(buf, PVMDLOGFILE_CSPP,
					pvmtmp, pvm_useruid, scid, hna);
		else
#endif
			(void)sprintf(buf, PVMDLOGFILE, pvmtmp, pvm_useruid, hna);

#else	/*SHAREDTMP*/
#ifdef	IMA_CSPP
		if (scid > 1)
			(void)sprintf(buf, PVMDLOGFILE_CSPP,
					pvmtmp, pvm_useruid, scid);
		else
#endif
#ifndef WIN32
		(void)sprintf(buf, PVMDLOGFILE, pvmtmp, pvm_useruid);
#else
		(void)sprintf(buf, "%s\\pvml.%s", pvmtmp, username);	
#endif		
#endif	/*SHAREDTMP*/

		/* Append a Virtual Machine ID, If Set */
		if ( p = getenv("PVM_VMID") ) {
			strcat( buf, "." );
			strcat( buf, p );
		}

		if ((log_fd = open(buf,
				O_WRONLY|O_TRUNC|O_CREAT|O_EXCL|O_APPEND, 0600)) != -1)
			log_ff = fdopen(log_fd, "a");
		else
			how &= ~2;

		if (p = getenv("PVMDLOGMAX"))
			pvmdlogmax = atoi(p);

		pvmdlogmax = pvmdlogmax;  /* for picky SGI compiler warning */
	}

	if (2 & log_how & ~how) {
		(void)fclose(log_ff);
		log_ff = 0;
		log_fd = -1;
	}

	log_how = how;
	return 0;
}


int
vpvmlogprintf(fmt, ap)
	char *fmt;
	va_list ap;
{
	char vtmp[1024];
	char *truncated;
	int cnt = 0;
	int cc;

#ifdef	TIMESTAMPLOG
	time_t now;
	struct tm *tmp;

	time(&now);
	tmp = localtime(&now);
#endif	/*TIMESTAMPLOG*/

	/* snag var arg string, in case re-use whacks it... */
	/* (thank you very much x86_64...  :-Q */
	/* damn, also need to prevent buffer overflows... */
	/* (thanks to Lasse Makholm <lasse.makholm@nokia.com> */
	/* for this patch...) */
	cc = vsnprintf(vtmp, sizeof(vtmp), fmt, ap);
	truncated = ( cc >= 0 && cc < sizeof(vtmp) )
			? "" : "[...]\n";

	if (log_how & 1) {
		if (atnewline) {
			if (pvmmytid)
				fprintf(stderr, "[t%x] ", pvmmytid);
			else
				fprintf(stderr, "[pvmd pid%d] ", pvmmyupid);

#ifdef	TIMESTAMPLOG
			fprintf(stderr, "%02d/%02d %02d:%02d:%02d ",
						tmp->tm_mon + 1,
						tmp->tm_mday,
						tmp->tm_hour,
						tmp->tm_min,
						tmp->tm_sec);
#endif	/*TIMESTAMPLOG*/
		}
		/* cc = vfprintf(stderr, fmt, ap); */
		cc = fprintf(stderr, "%s%s", vtmp, truncated);
		cnt = ( cc >= 0 ) ? cnt + cc : cc;
		fflush(stderr);
	}

	if (log_how & 2) {
		if (log_alrdy < pvmdlogmax) {
			if (atnewline) {
				if (pvmmytid)
					fprintf(log_ff, "[t%x] ", pvmmytid);
				else
					fprintf(log_ff, "[pvmd pid%d] ", pvmmyupid);

#ifdef	TIMESTAMPLOG
				fprintf(log_ff, "%02d/%02d %02d:%02d:%02d ",
						tmp->tm_mon + 1,
						tmp->tm_mday,
						tmp->tm_hour,
						tmp->tm_min,
						tmp->tm_sec);
#endif	/*TIMESTAMPLOG*/
			}

			/* cc = vfprintf(log_ff, fmt, ap); */
			cc = fprintf(log_ff, "%s%s", vtmp, truncated);
			cnt = ( cnt >= 0 ) ? ( ( cc >= 0 ) ? cnt + cc : cc ) : cnt;
			fflush(log_ff);

			if ((log_alrdy += cnt) >= pvmdlogmax)
				(void)write(log_fd, toomuch, strlen(toomuch));
		}
	}

	atnewline = (fmt[strlen(fmt) - 1] == '\n') ? 1 : 0;

/*
	if ((log_how & 4) && hosts->ht_local != hosts->ht_cons) {
XXX send error-log message to console host
	}
*/

	return(cnt);
}


int
#if defined (__STDC__) || defined(IMA_WIN32_WATCOM)
pvmlogprintf(const char *fmt, ...)
#else
pvmlogprintf(va_alist)
	va_dcl
#endif
{
	va_list ap;
	int cc;

#if defined (__STDC__) || defined(IMA_WIN32_WATCOM)

	va_start(ap, fmt);
#else
	char    *fmt;

	va_start(ap);
	fmt = va_arg(ap, char *);
#endif
	cc = vpvmlogprintf(fmt, ap);
	va_end(ap);

	return cc;
}


int
pvmlogperror(s)
	char *s;	/* text */
{
#ifdef WIN32
	char em[16];
	int error_nr=0;
#else
#ifdef SYSERRISCONST
	const char *em;
#else
	char *em;
#endif
#endif
	int i;

#ifndef WIN32
#ifdef USESTRERROR
	em = (char *) strerror( errno );
#else
	em = ((errno >= 0 && errno < sys_nerr)
		? sys_errlist[errno] : "Unknown Error");
#endif
#else
	error_nr = GetLastError();
	sprintf(em,"%d",error_nr);
#endif

	pvmlogprintf("%s: %s\n", s, em);
	return 0;
}


/*	pvmlogerror()
*
*	Log an error message.  Effect depends on how far we've gotten.
*	If log_how & 1, write to stderr.
*	If log_how & 2, can write to the logfile.
*	If log_how & 4, can send message to console pvmd.
*/

pvmlogerror(s)
	char *s;	/* text */
{
	pvmlogprintf("%s", s);
	atnewline = 1;
	return 0;

#if NOMORELOGERROR
	int len = strlen(s);
	char buf[40];
	int l;
#ifdef	TIMESTAMPLOG
	time_t now;
	struct tm *tmp;

	time(&now);
	tmp = localtime(&now);
#endif	/*TIMESTAMPLOG*/

	if (log_how & 1) {
#ifdef	TIMESTAMPLOG
		if (pvmmytid)
			fprintf(stderr, "[t%x] %02d/%02d %02d:%02d:%02d %s", pvmmytid,
					tmp->tm_mon + 1,
					tmp->tm_mday,
					tmp->tm_hour,
					tmp->tm_min,
					tmp->tm_sec,
					s);
		else
			fprintf(stderr, "[pvmd pid%d] %02d/%02d %02d:%02d:%02d %s", pvmmyupid,
					tmp->tm_mon + 1,
					tmp->tm_mday,
					tmp->tm_hour,
					tmp->tm_min,
					tmp->tm_sec,
					s);

#else	/*TIMESTAMPLOG*/
		if (pvmmytid)
			fprintf(stderr, "[t%x] %s", pvmmytid, s);
		else
			fprintf(stderr, "[pvmd pid%d] %s", pvmmyupid, s);
#endif	/*TIMESTAMPLOG*/
	}

	if (log_how & 2) {
		if (log_alrdy < pvmdlogmax) {
#ifdef	TIMESTAMPLOG
			if (pvmmytid)
				sprintf(buf, "[t%x] %02d/%02d %02d:%02d:%02d ", pvmmytid,
						tmp->tm_mon + 1,
						tmp->tm_mday,
						tmp->tm_hour,
						tmp->tm_min,
						tmp->tm_sec);
			else
				sprintf(buf, "[pvmd pid%d] %02d/%02d %02d:%02d:%02d ", pvmmyupid,
						tmp->tm_mon + 1,
						tmp->tm_mday,
						tmp->tm_hour,
						tmp->tm_min,
						tmp->tm_sec);

#else	/*TIMESTAMPLOG*/
			if (pvmmytid)
				sprintf(buf, "[t%x] ", pvmmytid);
			else
				sprintf(buf, "[pvmd pid%d] ", pvmmyupid);
#endif	/*TIMESTAMPLOG*/

			l = strlen(buf);
			(void)write(log_fd, buf, l);
			(void)write(log_fd, s, len);
			if ((log_alrdy += len + l) >= pvmdlogmax)
				(void)write(log_fd, toomuch, strlen(toomuch));
		}
	}

/*
	if ((log_how & 4) && hosts->ht_local != hosts->ht_cons) {
XXX send error-log message to console host
	}
*/
#endif
}




static char rcsid[] =
	"$Id: pvmdunix.c,v 1.15 2001/02/07 23:15:54 pvmsrc Exp $";

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
 *	pvmdunix.c
 *
 *	Support routines for pvmd in unix environment.
 *
 * $Log: pvmdunix.c,v $
 * Revision 1.15  2001/02/07 23:15:54  pvmsrc
 * 2nd Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.14  2000/02/16 22:00:29  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.13  2000/02/08 16:58:59  pvmsrc
 * Re-arranged #include of header files for CRAY J932se.
 * 	- move rpc/types.h, rpc/xdr.h before sys/time.h, sys/socket.h...
 * 	- patch submitted by "Francois Moyroud" <francois@egi.kth.se>
 * (Spanker=kohl)
 *
 * Revision 1.12  1999/07/08 19:00:11  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.11  1998/11/20  20:10:40  pvmsrc
 * Lets try this again....
 *
 * Changes so that compiles & builds on win32.  Also, single
 * source win32 & unix
 * (Spanker=sscott)
 *
 * Revision 1.10  1997/08/29  13:35:17  pvmsrc
 * OS2 Port Submitted by Bohumir Horeni, horeni@login.cz.
 * (Spanker=kohl)
 *
 * Revision 1.9  1997/07/30  19:01:23  pvmsrc
 * Markus Fixes.
 *
 * Revision 1.8  1997/06/27  19:52:15  pvmsrc
 * Integrated WIN32 changes.
 *
 * Revision 1.7  1997/06/24  20:39:21  pvmsrc
 * Eliminated unnecessary global externs.
 *
 * Revision 1.6  1997/06/16  13:40:35  pvmsrc
 * ppi_load changed to pass extra info into forkexec call to help taskers.
 *
 * Revision 1.5  1997/03/06  21:11:38  pvmsrc
 * 		formatting :-)
 *
 * Revision 1.4  1997/01/28  19:27:25  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/25  13:58:04  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.2  1996/10/24  21:04:52  pvmsrc
 * Moved #include of "global.h" down below other headers:
 * 	- need to have all of the structures / types declared before
 * 		the globals can be declared...
 *
 * Revision 1.1  1996/09/23  23:44:35  pvmsrc
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
#include <time.h>
#else
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#ifdef	SYSVSTR
#include <string.h>
#define	CINDEX(s,c)	strchr(s,c)
#else
#include <strings.h>
#define	CINDEX(s,c)	index(s,c)
#endif
#include <errno.h>
#include <stdio.h>
#include <pvmtev.h>
#include <pvmproto.h>
#include "pvmalloc.h"
#include "host.h"
#include "pmsg.h"
#include "waitc.h"
#include "task.h"
#include "listmac.h"
#include "bfunc.h"
#include "global.h"

#if defined(IMA_CRAY) || defined(IMA_CRAY2) || defined(IMA_I860) || defined(IMA_E88K) || defined(IMA_SCO)
#ifndef	MAXPATHLEN
#define	MAXPATHLEN	CANBSIZ
#endif
#endif


/***************
 **  Globals  **
 **           **
 ***************/


/*	ppi_load()
*
*	Start a set of new tasks running on this host.
*/

int
ppi_load(wxp)
	struct waitc_spawn *wxp;
{
	int i;
	struct task *tp;
	int err = 0;

#ifdef WIN32
	/* even being left untouched the filename gets a further */
	/* extension to %SystemRoot% which causes to return PvmNoFile */
	char filename[64];
	strcpy(filename,wxp->w_argv[0]);
#endif

	for (i = 0; i < wxp->w_veclen; i++) {
		if (err) {
			wxp->w_vec[i] = err;

		} else {
#ifdef WIN32
			strcpy(wxp->w_argv[0],filename);
#endif		
			if ( err = forkexec(wxp->w_flags, wxp->w_argv[0], 
					wxp->w_argv, wxp->w_nenv, wxp->w_env, 
					(wxp->w_instance+i), wxp->w_hosttotal, wxp->w_outof, &tp)) 
		/* inst+i as each process is started inst is really an offset */
			{
				wxp->w_vec[i] = err;

			} else {
				tp->t_ptid = wxp->w_ptid;
				tp->t_outtid = wxp->w_outtid;
				tp->t_outctx = wxp->w_outctx;
				tp->t_outtag = wxp->w_outtag;
				tp->t_trctid = wxp->w_trctid;
				tp->t_trcctx = wxp->w_trcctx;
				tp->t_trctag = wxp->w_trctag;
				tp->t_sched = wxp->w_sched;
				wxp->w_vec[i] = tp->t_tid;
			}
		}
	}
	return 0;
}


int
ppi_kill(tp, signum)
	struct task *tp;
	int signum;
{
	if (tp->t_pid)
#ifndef WIN32
#ifdef IMA_OS2
		(void)os2_kill(tp->t_pid, signum);
#else
		(void)kill(tp->t_pid, signum);
#endif
#else 
		(void)kill(tp->t_pid,tp->t_handle,signum);
#endif
	else
		pvmlogprintf("ppi_kill() signal for t%x scrapped (pid = 0)\n",
				tp->t_tid);
	return 0;
}


int
ppi_config(argc, argv)
	int argc;
	char **argv;
{
	return 0;
}


int
ppi_init()
{
	return 0;
}



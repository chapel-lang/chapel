
static char rcsid[] =
	"$Id: lpvm.c,v 1.73 2009/01/30 21:23:24 pvmsrc Exp $";

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
 *	lpvm.c
 *
 *	Libpvm core for unix environment.
 *
 * $Log: lpvm.c,v $
 * Revision 1.73  2009/01/30 21:23:24  pvmsrc
 * Fixed up insecure usage of fopen() for creating/writing/appending files.
 * 	- replace with open(...O_EXCL...) and fdopen()...
 * Fixed improper return code wrangling in pvmbeatask()...
 * 	- always return PvmSysErr on pvmd communication failure...  :-}
 * (Spanker=kohl)
 *
 * Revision 1.72  2009/01/22 23:18:36  pvmsrc
 * Updated version for BEOLIN from Paul Springer...
 * (Spanker=kohl)
 *
 * Revision 1.71  2007/04/19 23:14:34  pvmsrc
 * Added modern blanket header files to eliminate warnings on _exit()...
 * 	- #include <unistd.h>
 * 	- #include <stdlib.h>
 * (Spanker=kohl)
 *
 * Revision 1.70  2005/05/18 17:56:27  pvmsrc
 * Added calls to pvmchkuid()...
 * 	- keep people from running PVM as root...!  :-o
 * (Spanker=kohl)
 *
 * Revision 1.69  2004/01/14 18:50:55  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.68  2003/02/04 20:03:04  pvmsrc
 * Got rid of Jigen's goofy my_errno...
 * 	- never resolved anywhere...
 * (Spanker=kohl)
 *
 * Revision 1.67  2002/04/16 15:06:01  pvmsrc
 * WIN32 Fixes for Multiple Domains.
 * 	- submitted by Jigen Zhou <jigen@icemcfd.com>.
 * (Spanker=kohl)
 *
 * Revision 1.66  2002/02/18 19:18:33  pvmsrc
 * Added "|| defined(IMA_SUN4SOL2)" to "const void *i, *j" #if
 * 	for int_compare() / qsort()...
 * 	(needed for sure for 64 bit) Solaris)
 * (Spanker=kohl)
 *
 * Revision 1.65  2001/12/07 16:06:09  pvmsrc
 * Oops!  Some new Unix fixes & features break Windows (of course!).
 * 	- #ifdef-ed away fcntl() calls for non-blocking sockets / stdin.
 * 	(one for bug fix in lpvm.c, one for check_ext_input() in pvmd.c)
 * (Spanker=kohl)
 *
 * Revision 1.64  2001/09/25 21:20:17  pvmsrc
 * Minor TMPNAMFUN()/tmpnam() cleanup.
 * 	- moved macro def to pvm3.h, renamed PVMTNPMAN().
 * 	- same for LEN_OF_TMP_NAM -> PVMTMPNAMLEN.
 * 	- mostly a huge waste of time, since *both* tmpnam() & mktemp()
 * 		produce the same "dangerous" warning message in Linux/gcc...
 * 	- damn.
 * (Spanker=kohl)
 *
 * Revision 1.63  2001/09/25 17:28:09  pvmsrc
 * Fixed leftover tmpnam() usage -> use TMPNAMFUN() instead...
 * 	- also got rid of "double" call to tmpnam()...
 * (Spanker=kohl)
 *
 * Revision 1.62  2001/05/11 19:40:50  pvmsrc
 * Fixed direct-routing bug, removed blocking socket deadlock.
 * 	- fix submitted by Kamil Iskra <kamil@wins.uva.nl> (as well as
 * 		<jc@esi.fr>, Dick van Albada <dick@wins.uva.nl>, and
 * 		Zeger Hendrikse <zegerh@wins.uva.nl>).
 * (Spanker=kohl)
 *
 * Revision 1.61  2001/05/11 19:21:29  pvmsrc
 * Added "&& errno != EAGAIN" to select() return code check in mxfer().
 * 	- reported by "Johannes Hennecke" <Johannes.Hennecke@ELSA.de>
 * (Spanker=kohl)
 *
 * Revision 1.60  2001/05/11 17:32:26  pvmsrc
 * Eliminated references to sys_errlist & sys_nerr.
 * 	- unnecessary, and we're whacking that crap anyway.
 * (Spanker=kohl)
 *
 * Revision 1.59  2001/02/07 23:14:04  pvmsrc
 * First Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.58  2000/06/15 17:51:49  pvmsrc
 * Fixed bug in WIN32 direct routing.
 * 	- stupid #endif in the wrong place, pvm_fd_add() call whacked.
 * 	- turned back on direct routing default and setopt.
 * (Spanker=kohl)
 *
 * Revision 1.57  2000/02/17 23:12:10  pvmsrc
 * *** Changes for new BEOLIN port ***
 * 	- MPP-like, similar to SP2, etc.
 * 	- submitted by Paul Springer <pls@smokeymt.jpl.nasa.gov>.
 * 	- format-checked & cleaned up by Jeembo...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.56  2000/02/16 21:59:40  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.55  2000/02/07 22:22:07  pvmsrc
 * Hack to help with select()/fd_sets in WIN32:
 * 	- fd_set is *NOT* a bit field in WIN32, is a circular buffer.
 * 	- must check for !FD_ISSET() before FD_SET() else duplicate
 * 		entries possible, and fd_set buffer overflow/overwrite.
 * 	- similary, FD_ISSET() check before FD_CLR().
 * 	- patch submitted by "Bruce W. Church" <bwc1@cornell.edu>.
 * (Spanker=kohl)
 *
 * Revision 1.54  1999/11/08 17:44:29  pvmsrc
 * SGI compiler cleanup.
 * (Spanker=kohl)
 *
 * Revision 1.53  1999/07/08 18:59:53  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.52  1999/03/15  19:05:58  pvmsrc
 * Replaced sleep() calls with pvmsleep().
 * 	- Unix / Win32, C / Fortran compat...
 * (Spanker=kohl)
 *
 * Revision 1.51  1999/03/04  22:09:33  pvmsrc
 * New SHMD #ifdefs for pvm_psend() & pvm_precv().
 * More direct conn debugging output.
 * (Spanker=kohl)
 *
 * Revision 1.50  1999/02/12  17:43:38  pvmsrc
 * Improved error messages in mksocs() for connect().
 * 	- dump out the socket file being tried, in case some BOZOS (like
 * 		the people at Harris :-) put an echo statement into the
 * 		pvm3/lib/pvmd script...  D-OH!
 * (Spanker=kohl)
 *
 * Revision 1.49  1998/11/20  20:04:00  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (Spanker=sscott)
 *
 * Revision 1.48  1998/10/02  15:43:58  pvmsrc
 * Single source code merge of Win32 and Unix code.
 * (Spanker=sscott)
 *
 * Revision 1.47  1998/09/30  18:08:32  pvmsrc
 * Fixed (I think :-) the direct connection initialization protocol.
 * 	- if two tasks tried to direct connect to each at the same time,
 * 		the CONREQ control messages could cross, causing each task
 * 		to fail on a connect().
 * 	- instead, independent of Unix Domain / TCP (local / remote)
 * 		socket, *always* have greater (numerically) task ID handle
 * 		the direct connection, and have lower task ID just drop the
 * 		CONREQ on the floor.
 * 	- seems to work after extensive testing, both local & remote,
 * 		here's hoping...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.46  1998/09/23  15:25:23  pvmsrc
 * Need to make char *username non-extern here.
 * 	- username storage has to be somewhere for pvmd & libpvm.
 * 	- pvmwin.c can't have the storage, because it wouold clash with
 * 		pvmd.c, which needs to define username for non-WIN32 archs...
 * 	- beat me with a shovel dammit.
 * (Spanker=kohl)
 *
 * Revision 1.45  1998/08/13  18:32:45  pvmsrc
 * Added special case for AIX4SP2 arch with SOCKLENISUINT.
 * 	- on AIX 4.3 systems, I guess unsigned int is really
 * 		unsigned int, and not size_t...  D-Oh!
 * 	- probably need a better constant name (or two) here...
 * (Spanker=kohl)
 *
 * Revision 1.44  1998/07/24  17:11:44  pvmsrc
 * Cleaned up use of SOCKLENISUINT / oslen.
 * 	- use oslen for every socket-related call:
 * 		* bind(), recvfrom(), getsockname() and accept().
 * (Spanker=kohl)
 *
 * Revision 1.43  1998/03/02  19:27:32  pvmsrc
 * Fixed PGON typo seg fault parentheses d-oh.  (Phil P.)
 * (Spanker=kohl)
 *
 * Revision 1.42  1998/02/23  22:51:29  pvmsrc
 * Added AIX4SP2 stuff.
 * (Spanker=kohl)
 *
 * Revision 1.41  1998/01/23  20:28:41  pvmsrc
 * 	Changed arguments to mxinput so that it will return number of frags
 * 	read and the number of messages.
 * 	mxfer will now continue reading frags if called by probe or nrecv and
 * 	mxinput returns frags > 0 but messages == 0.
 * (Spanker=phil)
 *
 * Revision 1.40  1997/12/31  20:50:02  pvmsrc
 * Cleaned Up System Message Handlers.
 * 	- current send / recv buffers now saved before invocation of
 * 		message handler functs.
 * 	- removed manual rbf = setrbuf(mid) saving & resetting in
 * 		handlers...
 * (Spanker=kohl)
 *
 * Revision 1.39  1997/12/18  21:22:53  pvmsrc
 * Applied perf patch for IBMs from Anders Alund / Nils Jonsson (PDC).
 * 	- nissej@pdc.kth.se
 * (Spanker=kohl)
 *
 * Revision 1.38  1997/12/01  19:20:36  pvmsrc
 * Replaced #ifdef IMA_OS2 fd_set declarations:
 * 	- new #ifdef FDSETNOTSTRUCT.
 * 	- choose between "fd_set foo" and "struct fd_set foo"...
 * (Spanker=kohl)
 *
 * Revision 1.37  1997/11/04  23:20:00  pvmsrc
 * Cleaned up fd_set stuff (hopefully).
 * Removed unused CINDEX() define.
 * (Spanker=kohl)
 *
 * Revision 1.36  1997/10/24  15:17:46  pvmsrc
 * Added TEV_DID_RCX to trace events for Receive Message Context.
 * 	- in pvm_recv(), pvm_trecv(), pvm_nrecv(), and pvm_precv().
 * (Spanker=kohl)
 *
 * Revision 1.35  1997/10/24  14:29:23  pvmsrc
 * Added TEV_DID_MCX / pvmmyctx trace event info to:
 * 	- pvm_send(), pvm_mcast(), pvm_recv(), pvm_trecv(), pvm_nrecv().
 * 	- pvm_psend(), pvm_precv().
 * (Spanker=kohl)
 *
 * Revision 1.34  1997/09/22  21:13:25  pvmsrc
 * Added new pvmsettaskname() linkage (for shell-spawned tasks only!).
 * 	- call pvmsettaskname() before joining PVM, sends task name
 * 		(stored in new char *pvmmytaskname global) to pvmd in
 * 		TM_CONN2 message.
 * 	- appears in trace events and console ps.
 * (Spanker=kohl)
 *
 * Revision 1.33  1997/09/10  21:37:43  pvmsrc
 * putting socket startup to pvmbeatask. Markus
 * (Spanker=fischer)
 *
 * Revision 1.32  1997/08/29  13:35:06  pvmsrc
 * OS2 Port Submitted by Bohumir Horeni, horeni@login.cz.
 * (Spanker=kohl)
 *
 * Revision 1.31  1997/08/06  22:43:10  pvmsrc
 * Added new SGI6 and SGIMP6 arches.
 *
 * Revision 1.30  1997/06/27  18:04:17  pvmsrc
 * Integrated WIN32 changes.
 *
 * Revision 1.29  1997/06/12  20:10:37  pvmsrc
 * Made sure all communications for TC_* task control messages
 * 	use the SYSCTX_TC system context.
 * 	- some messages being sent in default context...  D-Oh...
 *
 * Revision 1.28  1997/05/07  21:19:23  pvmsrc
 * swapped logic around SOCKLENISINT so that it is now
 * the default path and SOCKLENISUINT is the compile-time
 * selected path.
 *
 * Now, AIX 4.1 will have to remove SOCKLENISUINT from
 * their AIXxxx.def configure files.
 *
 * Revision 1.27  1997/05/07  18:37:13  pvmsrc
 * AIX 3.2 vs 4.1 vs 4.2 problems resolved (I hope)
 * Define oslen as size_t with compile flag SOCKLENISINT
 * for AIX 4.1 to explicitly set to int.
 * To use flag - set in conf/AIX46K.def.
 * Tested with cc and gcc on all 3 OS versions.
 *
 * Revision 1.26  1997/05/05  15:19:40  pvmsrc
 * 	context was not being set in pvmmcast.
 *
 * Revision 1.25  1997/05/01  14:11:46  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.24  1997/04/30  21:26:00  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.23  1997/04/29  20:29:12  pvmsrc
 * 	Support new calling sequences defined in mppmsg.h
 *
 * Revision 1.22  1997/04/25  19:21:18  pvmsrc
 * Handle inplace bodies of zero length correctly
 *
 * Revision 1.21  1997/04/24  21:06:59  pvmsrc
 * Excised unused variables.
 * Support for pvm_psend/precv constructs
 *
 * Revision 1.20  1997/04/21  14:58:28  pvmsrc
 * Changed #ifdefs that checked IMA_RS6K,IMA_SP2MPI & IMA_AIX46K
 * 	to see if select.h was needed into single define NEEDSSELECTH.
 * 	New archs need to set this in conf/
 *
 * Revision 1.19  1997/04/07  21:09:17  pvmsrc
 * pvm_addmhf() - new paramter interface
 *
 * Revision 1.18  1997/04/03  19:23:13  pvmsrc
 * Added context for TM_xxx messages
 *
 * Revision 1.17  1997/04/01  21:28:11  pvmsrc
 * Damn Damn Damn.
 * 	- pvm_recvinfo() returns a bufid, not an index.  Damn.
 *
 * Revision 1.16  1997/04/01  20:48:17  pvmsrc
 * Fixed tracer mbox usage:
 * 	- pvm_getinfo() -> pvm_recvinfo(), new semantics handled (recvinfo
 * 		sets rbuf implicitly, a la pvm_recv, need to save rbuf).
 *
 * Revision 1.15  1997/03/27  19:55:27  pvmsrc
 * Fixed up pvmbeatask() to go get tracer info if spawned from shell:
 * 	- env var info including trace mask, trace buffer size, trace opts.
 * 	- use PVMTRACERCLASS mbox entry to fill in values, if matches
 * 		on trctid, trcctx, and trctag.
 *
 * Revision 1.14  1997/03/07  15:12:11  pvmsrc
 * Fixed annoying SGI warnings for qsort()'s  int_compare() function...
 *
 * Revision 1.13  1997/03/06  21:50:17  pvmsrc
 * Yanked out #includes for <netinet/in.h> and <netinet/tcp.h>.
 * 	- dups with lpvm.h #includes...
 *
 * Revision 1.12  1997/03/06  21:06:10  pvmsrc
 * - added include for lmsg.h, host.h
 * - for mpp's added pvmmimd.h, mppmsg.h
 * - moved struct ttpcb definition to header file lpvm.h
 * - for mpps added call to mpp_ttpcb_find inside of ttpcb_find
 * - mxinput rewritten to handle mpp input streams when ifdef'ed on
 * - Added a stack of inplace headers instead of just one dummy header.
 *   so we can do asynch header transmission
 * - in mxfer:
 * 	logic written in to handle mpp streams, including bypass of
 * 	reading routes if writing, posting a complete message for
 * 	asynch send before checking for read routes, pass information
 * 	to pvm_node_send (on mpps) about inplace sending so that
 * 	pvm_node_send can be optimized for inplace (not optimized yet :-))
 * - in mroute:
 * 	direct routing socket setup turned off for MPPs
 * - in pvmbeatask:
 * 	call pvm_mpp_beatask for mpps
 * 	don't do socket auth dance for mpps (sockets not used)
 * - added routine ogm_complete to test if an outgoing message is
 *  	complete (when sent asychronously). Returns TRUE for workstations
 * 	since writes are synchronous
 *
 * Revision 1.11  1997/01/28  19:26:21  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.10  1996/12/18  22:27:43  pvmsrc
 * Extracted duplicate versions of routines from lpvm/mimd/shmem.c,
 * 	inserted into shared lpvmgen.c:
 * 	- pvmbailout().
 * 	- pvmlogerror().
 * 	- vpvmlogprintf(), pvmlogprintf().  (hope these work on MPP & shmem)
 * 	- pvmlogperror().
 *
 * Revision 1.9  1996/10/25  13:57:20  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.8  1996/10/24  22:44:31  pvmsrc
 * Modified for New Tracing Facility:
 * 	- moved #include "global.h" below other #include's for typing.
 * 	- removed extra #include <pvm3.h> in lpvm.c...
 * 	- added #include of new "lpvm.h" to replace explicit externs.
 * 	- removed common control message handlers from lpvm.c:
 * 		* extracted to lpvmgen.c for general usage.
 * 		* pvm_tc_noop(), pvm_tc_settmask(), pvm_tc_siblings().
 * 		-> lpvmmimd.c & lpvmshmem.c still need remainder of pvmmctl()
 * 			replaced with control message handlers.
 * 	- arg typing hassles with int_compare() / qsort() exacerbated...
 * 	- modified pvmbeatask():
 * 		* handle new tracing info, unpack tracing and output collection
 * 			parameters into temp storage, and then check for local task
 * 			override before applying.
 * 		* read in new tracing env vars PVMTRCBUF & PVMTRCOPT.
 * 		* install new common message handlers.
 * 		* call new tev_init() routine to set up tracing stuff.
 * 		* use new Pvmtracer structures (pvmtrc & pvmctrc) to store info.
 * 	- removed pvm_getopt() & pvm_setopt() -> moved to common lpvmgen.c.
 * 	- removed old tev_begin(), tev_fin() & tev_do_trace() routines.
 * 	- updated trace event generation for pvm_getfds(), pvm_start_pvmd(),
 * 		pvm_precv(), pvm_psend().
 *
 * Revision 1.7  1996/10/14  19:17:06  pvmsrc
 * Used ARCHFLAG SOCKLENISUINT where socket length is unsigned int
 *
 * Revision 1.6  1996/10/09  21:48:19  pvmsrc
 * Problem:
 * --------
 * The problem was the result of AIX4.2 changing the expected datatype of
 * the XxxxLength parameter in the following system function calls:
 *
 * int bind (Socket, Name, NameLength)
 * int getsockname (Socket, Name, NameLength)
 * int recvfrom (Socket, Buffer, Length, Flags, From, FromLength)
 * int accept (Socket, Address, AddressLength)
 *
 * The compiler warning message generated was:
 *   Function argument assignment between types "unsigned long*" and "int*"
 *   is not allowed.
 *
 * Action:
 * -------
 * In PVM functions where oslen was already declared_
 * 	changed declaration from:	int oslen;
 * 			      to:	unsigned int oslen;
 *
 * In PVM functions where a local "temporary" variable such as i, j, etc.
 * was used_
 * 	added declaration of:		unsigned int oslen;
 * 	and then changed to use oslen where necessary.
 *
 * Revision 1.5  1996/10/04  13:17:23  pvmsrc
 * 		New context was generated on each spawn. This is bad.
 *
 * Revision 1.4  1996/09/24  15:15:46  pvmsrc
 * Test failure...  fixed.
 *
 * Revision 1.3  1996/09/24  15:12:19  kohl
 * Test check in...
 *
 * Revision 1.2  1996/09/23  23:30:53  pvmsrc
 * Initial Creation - original lpvm.c.
 *
 * Revision 1.32  1995/11/02  16:07:10  manchek
 * added NEEDSENDIAN switch.
 * must declare ptr to sys_errlist const in pvmlogperror for FREEBSD.
 * free replies to control messages in mxfer
 *
 * Revision 1.31  1995/09/06  17:37:24  manchek
 * aargh, forgot pvm_precv
 *
 * Revision 1.30  1995/09/06  17:30:32  manchek
 * pvm_psend returns not implemented instead of bad param for string type
 *
 * Revision 1.29  1995/07/28  16:40:58  manchek
 * wrap HASERRORVARS around errno declarations
 *
 * Revision 1.28  1995/07/28  16:04:05  manchek
 * switch endian includes on flag, not arch name
 *
 * Revision 1.27  1995/07/19  21:43:04  manchek
 * better mxfer logging
 *
 * Revision 1.26  1995/07/19  21:27:49  manchek
 * use pvmnametag to give symbolic message tags
 *
 * Revision 1.25  1995/07/18  16:59:03  manchek
 * added code to generate and check crc on each message (MCHECKSUM)
 *
 * Revision 1.24  1995/07/03  19:05:35  manchek
 * removed POWER4 arch ifdefs
 *
 * Revision 1.23  1995/06/28  18:18:07  manchek
 * do-nothing check_for_exit so one can be in lpvmshmem.c
 *
 * Revision 1.22  1995/06/19  17:35:05  manchek
 * addr sometimes wasn't set in pvm_tc_conreq, causing segfault
 *
 * Revision 1.21  1995/06/12  15:56:14  manchek
 * added PGON partition size support
 *
 * Revision 1.20  1995/06/02  17:19:14  manchek
 * added check in mxfer() for when outgoing route closed during input
 *
 * Revision 1.19  1995/06/01  14:43:54  manchek
 * pvm_start_pvmd ignores INT, QUIT, TSTP signals
 *
 * Revision 1.18  1995/05/30  17:28:08  manchek
 * added ifdefs for SP2MPI arch
 *
 * Revision 1.17  1995/05/17  17:06:54  manchek
 * added PVMTASKDEBUG envar
 *
 * Revision 1.16  1995/05/17  16:16:36  manchek
 * use FDSETISINT.
 * use umbuf_get and put instead of ALLOC.
 * in mksocs, read pvmd sockaddr file only once.
 * in pvmbeatask, read altpid from environment each time.
 * add PvmPollTime and Type, NotImplemented.
 * pvm_start_pvmd reads sockaddr from pvmd instead of sleeping on addr file,
 * sets PVMSOCK envar from return.
 * allows better synchronization, master host overloading.
 *
 * Revision 1.15  1995/02/01  21:08:46  manchek
 * error 4 is now PvmOverflow.
 * connect is retried in case of EINTR
 *
 * Revision 1.14  1994/12/20  16:28:19  manchek
 * added PvmShowTids case to setopt.
 * removed EOF message on pvmd socket close
 *
 * Revision 1.13  1994/10/15  19:07:31  manchek
 * don't use fd_sets when select returns -1; will hang reading.
 * cast message tags for comparison as integers
 *
 * Revision 1.12  1994/09/02  15:23:37  manchek
 * fixed segfaults in setopt when task not connected
 *
 * Revision 1.11  1994/07/18  19:20:18  manchek
 * fix to call write() with max 4096 length for RS6K
 *
 * Revision 1.10  1994/06/21  19:36:23  manchek
 * forgot to ifdef tt_spath
 *
 * Revision 1.9  1994/06/21  18:31:31  manchek
 * connect in mksocs() tries several times before giving up.
 * don't setsockopt() at TCP level on Unix task-task sockets
 *
 * Revision 1.8  1994/06/04  21:44:57  manchek
 * added unix domain sockets
 *
 * Revision 1.7  1994/06/03  20:38:15  manchek
 * version 3.3.0
 *
 * Revision 1.6  1993/11/30  23:50:01  manchek
 * set nodelay and snd, rcv buffer sizes on t-t sockets
 *
 * Revision 1.5  1993/11/30  15:51:30  manchek
 * beatask complains if it can't write d-auth file (fs full?)
 *
 * Revision 1.4  1993/10/04  20:29:05  manchek
 * mksocks() and pvm_start_pvmd() now use pvmdsockfile(), not TDSOCKNAME.
 * made pvm_useruid global for pvmcruft.c
 *
 * Revision 1.3  1993/10/04  19:10:22  manchek
 * mctl() conflicts with mctl(2) - declare static for now
 *
 * Revision 1.2  1993/09/16  21:36:20  manchek
 * pvm_start_pvmd() was freeing the return string from pvmgetpvmd()
 *
 * Revision 1.1  1993/08/30  23:26:48  manchek
 * Initial revision
 *
 */

#ifdef WIN32
#include "pvmwin.h"
#include <process.h>
#include <stdlib.h>
#include <search.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
#endif

#ifndef NOUNIXDOM
#include <sys/un.h>
#endif
#ifdef NEEDSSELECTH
#include <sys/select.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <errno.h>
#include <signal.h>

#ifdef IMA_BEOLIN
#include <netdb.h>
#endif

#include <pvmproto.h>
#include "pvmalloc.h"
#include "pvmfrag.h"
#include "pmsg.h"
#include "listmac.h"
#include "tvdefs.h"
#include "bfunc.h"
#include "lpvm.h"
#include <pvmtev.h>
#include "tevmac.h"
#include "host.h"
#include "waitc.h"
#include "global.h"
#include "lmsg.h"

#ifdef IMA_MPP
#include "pvmmimd.h"
#include "mppmsg.h"
#endif 

#ifdef PVM_SHMD
/* need the prototypes for the pvm_shmd send and receive calls */
#include "../shmd/shmdproto.h"
#endif


#ifndef	max
#define	max(a,b)	((a)>(b)?(a):(b))
#endif

#ifndef	min
#define	min(a,b)	((a)<(b)?(a):(b))
#endif

#ifndef	TTSOCKBUF
#define	TTSOCKBUF	0x8000
#endif


/***************
 **  Globals  **
 **           **
 ***************/

char *getenv();
void hex_inadport __ProtoGlarp__ (( char *, struct sockaddr_in * ));

extern char *inadport_decimal();
extern char *inadport_hex();
char *pvmgetpvmd();
char *pvmdsockfile();
char *pvmnametag();
struct pmsg *midtobuf();
struct pmsg *umbuf_new();
char *debug_flags();
struct msgid *pvm_inprecv = (struct msgid *) NULL;

#ifndef HASERRORVARS
extern int errno;						/* from libc */
#endif

#if defined(IMA_PGON)
int pvmpgonpartsize = -1;				/* FE prog, no partition */
#endif


/***************
 **  Private  **
 **           **
 ***************/

static int mxfersingle = 1;				/* mxfer returns after single frag */
static struct sockaddr_in pvmourinet;	/* our host ip addr */
static struct ttpcb *ttlist = 0;		/* dll of connected tasks */
static struct ttpcb *topvmd = 0;		/* default route (to pvmd) */
static int pvmnfds = 0;					/* 1 + highest bit set in fds */
#ifdef FDSETNOTSTRUCT
static fd_set pvmrfds;					/* rd fdset for mxfer() */
#else
static struct fd_set pvmrfds;			/* rd fdset for mxfer() */
#endif
static struct pmsg *txlist[100];		/* queue for when mroute reentered */
static int txrp = 0;					/* txlist read pointer */
static int txwp = 0;					/* txlist write pointer */
#ifdef WIN32
char *username = 0;
int system_loser_win;
#ifndef IMA_WIN32_WATCOM
extern char **environ;
#endif
#ifndef SOCK_DEFINES
WSADATA WSAData;
extern int nAlert;
#define SOCK_DEFINES
#endif
#endif

#ifdef WIN32
int int_compare(const void *i, const void *j);
#endif

/**************************
 **  Internal Functions  **
 **                      **
 **************************/

int
pvm_fd_add(fd, sets)
	int fd;				/* the fd */
	int sets;			/* which sets */
{
#ifdef	SANITY
#ifndef WIN32
	if (fd < 0 || fd >= FD_SETSIZE) {
		pvmlogprintf("pvm_fd_add() bad fd %d\n", fd);
		return 1;
	}
#endif
#endif
	if (sets & 1)
		if ( !FD_ISSET(fd, &pvmrfds) ) {
			FD_SET(fd, &pvmrfds);
#ifdef WIN32
			pvmnfds++;
#endif
		}
/*
	if (sets & 2)
		if ( !FD_ISSET(fd, &pvmwfds) ) {
			FD_SET(fd, &pvmwfds);
#ifdef WIN32
			pvmnwfds++;
#endif
		}
	if (sets & 4)
		if ( !FD_ISSET(fd, &pvmefds) ) {
			FD_SET(fd, &pvmefds);
#ifdef WIN32
			pvmnefds++;
#endif
		}
*/

#ifndef WIN32
	/* if this is new highest, adjust nfds */
	if (fd >= pvmnfds)
		pvmnfds = fd + 1;
#endif

	return 0;
}


pvm_fd_delete(fd, sets)
	int fd;				/* the fd */
	int sets;			/* which sets */
{
#ifdef	SANITY
#ifndef WIN32
	if (fd < 0 || fd >= FD_SETSIZE) {
		pvmlogprintf("pvm_fd_delete() bad fd %d\n", fd);
		return 1;
	}
#endif
#endif
	if (sets & 1)
		if ( FD_ISSET(fd, &pvmrfds) ) {
			FD_CLR(fd, &pvmrfds);
#ifdef WIN32
			pvmnfds--;
#endif
		}
/*
	if (sets & 2)
		if ( FD_ISSET(fd, &pvmwfds) ) {
			FD_CLR(fd, &pvmwfds);
#ifdef WIN32
			pvmnwfds--;
#endif
		}
	if (sets & 4)
		if ( FD_ISSET(fd, &pvmefds) ) {
			FD_CLR(fd, &pvmefds);
#ifdef WIN32
			pvmnefds--;
#endif
		}
*/

#ifndef WIN32
	/* if this was highest, may have to adjust nfds to new highest */
	if (fd + 1 == pvmnfds)
		while (pvmnfds > 0) {
			pvmnfds--;
			if (FD_ISSET(pvmnfds, &pvmrfds)
/*
			|| FD_ISSET(pvmnefds, &pvmefds)
			|| FD_ISSET(pvmnwfds, &pvmwfds)
*/
			) {
				pvmnfds++;
				break;
			}
		}
#endif

	return 0;
}


/*	ttpcb_new()
*
*	Create a new, blank ttpcb.
*/

struct ttpcb *
ttpcb_new()
{
	struct ttpcb *pcbp;

	if (pcbp = TALLOC(1, struct ttpcb, "tpcb")) {
		BZERO((char*)pcbp, sizeof(struct ttpcb));
		pcbp->tt_fd = -1;
		pcbp->tt_rxfrag = pmsg_new(1);
		BZERO((char*)pcbp->tt_rxfrag, sizeof(struct pmsg));
		pcbp->tt_rxfrag->m_link = pcbp->tt_rxfrag->m_rlink = pcbp->tt_rxfrag;
	}
	return pcbp;
}


/*	ttpcb_delete()
*
*	Get rid of a ttpcb.  Delete it from any list, close the socket,
*	free any rx frag heap.
*/

void
ttpcb_delete(pcbp)
	struct ttpcb *pcbp;
{
	struct pmsg *up;

	if (pcbp->tt_link) {
		LISTDELETE(pcbp, tt_link, tt_rlink);
	}
	if (pcbp->tt_fd != -1) {
		pvm_fd_delete(pcbp->tt_fd, 3);
		(void)close(pcbp->tt_fd);
	}
	if (up = pcbp->tt_rxfrag) {
		while (up->m_link != up)
			umbuf_free(up->m_link);
		pmsg_unref(up);
	}
	if (pcbp->tt_rxf)
		fr_unref(pcbp->tt_rxf);
#ifndef NOUNIXDOM
	if (pcbp->tt_spath)
		(void)unlink(pcbp->tt_spath);
#endif

	PVM_FREE(pcbp);
}


/*	ttpcb_creat()
*
*	Create a new task-task entry.  Allocates a ttpcb, inserts it in
*	ttlist.
*/

struct ttpcb *
ttpcb_creat(tid)
	int tid;		/* peer tid */
{
	struct ttpcb *pcbp, *pcbp2;

	if (pcbp = ttpcb_new()) {
		pcbp->tt_tid = tid;

		for (pcbp2 = ttlist->tt_link; pcbp2 != ttlist; pcbp2 = pcbp2->tt_link)
			if (pcbp2->tt_tid > tid)
				break;
		LISTPUTBEFORE(pcbp2, pcbp, tt_link, tt_rlink);
	}
	return pcbp;
}


/*	ttpcb_find()
*
*	Find a task-task entry by tid in ttlist.
*/

struct ttpcb *
ttpcb_find(tid)
	int tid;		/* peer tid */
{
	struct ttpcb *pcbp;

#if defined(IMA_MPP)
	if ( pcbp = mpp_ttpcb_find(tid))
		return pcbp;
#endif
	for (pcbp = ttlist->tt_link; pcbp != ttlist; pcbp = pcbp->tt_link)
		if (pcbp->tt_tid >= tid)
			break;
	return (pcbp->tt_tid == tid) ? pcbp : (struct ttpcb*)0;
}


/*	ttpcb_dead()
*
*	Mark ttpcb dead, close socket, remove it from fd sets.
*/

void
ttpcb_dead(pcbp)
	struct ttpcb *pcbp;
{
	struct pmsg *up;
	int sbf;

	pcbp->tt_state = TTDEAD;
	if (pcbp->tt_fd != -1) {
		pvm_fd_delete(pcbp->tt_fd, 3);
		(void)close(pcbp->tt_fd);

	/* notify routing socket closed */

	/*
	XXX problems with the route notify messages:
	XXX they can be delayed / cause deadlock because gotem isn't incremented
	*/
		check_routedelete(pcbp);
		pcbp->tt_fd = -1;
	}
#ifndef NOUNIXDOM
	if (pcbp->tt_spath) {
		(void)unlink(pcbp->tt_spath);
		pcbp->tt_spath = 0;
	}
#endif
	if (pcbp->tt_rxf) {
		fr_unref(pcbp->tt_rxf);
		pcbp->tt_rxf = 0;
	}
	if (up = pcbp->tt_rxfrag) {
		while (up->m_link != up)
			umbuf_free(up->m_link);
	}
}


int
ttpcb_dump(pcbp)
	struct ttpcb *pcbp;
{
	pvmlogprintf("ttpcb_dump() t%x fd=%d sad=%s",
		pcbp->tt_tid,
		pcbp->tt_fd,
		inadport_decimal(&pcbp->tt_sad));
	pvmlogprintf(" osad=%s state=%d\n",
		inadport_decimal(&pcbp->tt_osad), pcbp->tt_state);
	return 0;
}


int
ttpcb_dumpall()
{
	struct ttpcb *pcbp;

	pvmlogerror("ttpcb_dumpall()\n");
	ttpcb_dump(topvmd);
	for (pcbp = ttlist->tt_link; pcbp != ttlist; pcbp = pcbp->tt_link)
		ttpcb_dump(pcbp);
	return 0;
}


/*	check_routeadd()
*
*	Check if a notify is posted for new route creation, and send us
*	a message if so.
*/

int
check_routeadd(pcbp)
	struct ttpcb *pcbp;
{
	struct waitc *wp, *wp2;
	struct pmsg *up;
	int sbf;

	wp = waitlist->wa_link;
	while (wp != waitlist) {
		wp2 = wp->wa_link;
		if (wp->wa_kind == WT_ROUTEA) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			pvm_pkint(&pcbp->tt_tid, 1, 1);
			pvm_pkint(&pcbp->tt_fd, 1, 1);
			sbf = pvm_setsbuf(sbf);
			up = midtobuf(sbf);
			up->m_ctx = wp->wa_mesg->m_ctx;
			up->m_tag = wp->wa_mesg->m_tag;
			mesg_input(up);
			if (wp->wa_count != -1 && --wp->wa_count < 1)
				wait_delete(wp);
		}
		wp = wp2;
	}
	return 0;
}


/*	check_routedelete()
*
*	Check if a notify is posted for route deletion, and send us
*	a message if so.
*/

int
check_routedelete(pcbp)
	struct ttpcb *pcbp;
{
	struct waitc *wp, *wp2;
	struct pmsg *up;
	int tid = pcbp->tt_tid;

	wp = waitlist->wa_link;
	while (wp != waitlist) {
		wp2 = wp->wa_link;
		if (wp->wa_kind == WT_ROUTED && tid == wp->wa_on) {
			up = wp->wa_mesg;
			wp->wa_mesg = 0;
			mesg_input(up);
			wait_delete(wp);
		}
		wp = wp2;
	}
	return 0;
}


int
post_routedelete(tid, ctx, tag)
	int tid;
	int ctx;
	int tag;
{
	int sbf;
	struct waitc *wp;
	struct pmsg *up;
	int i;

	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
	pvm_pkint(&tid, 1, 1);
	i = -1;
	pvm_pkint(&i, 1, 1);
	sbf = pvm_setsbuf(sbf);
	up = midtobuf(sbf);
	up->m_ctx = ctx;
	up->m_tag = tag;

	if (ttpcb_find(tid)) {
		wp = wait_new(WT_ROUTED);
		wp->wa_tid = pvmmytid;
		wp->wa_on = tid;
		wp->wa_mesg = up;

	} else {
		mesg_input(up);
	}
	return 0;
}


/*	pvm_tc_conreq()
*
*	Another task requests a connection with us.
*	Reply with a TC_CONACK message.
*
*	TC_CONREQ() {
*		int tdprotocol		// t-d protocol revision number
*		string sockaddr		// address of other socket
*	}
*/

static int
pvm_tc_conreq(mid)
	int mid;
{
	int src;					/* sender of request */
	int sbf = 0;				/* reply message mid */
	int ttpro;					/* protocol revision */
	struct ttpcb *pcbp;			/* pcb for connection */
	int ackd;					/* allow connection (0) */
	char *addr = "";			/* socket address */
	int i;
	int ictx;
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	unsigned int oslen;
#else
	size_t oslen;
#endif
#else
	int oslen;
#endif
#ifndef NOUNIXDOM
	struct sockaddr_un uns;
	char spath[PVMTMPNAMLEN];
#endif
	char buf[256];

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);

	pvm_upkint(&ttpro, 1, 1);
	pvm_upkstr(buf);

	if (pcbp = ttpcb_find(src)) {
		if (pvmdebmask & PDMROUTE) {
			pvmlogprintf(
					"pvm_tc_conreq() crossed CONREQ from t%x\n", src);
		}
		if (pcbp->tt_state == TTCONWAIT) {

			/* must handle simultaneous connect from both ends:
			 *  lower tid of the two just ignores the CONREQ.
			 *  higher tid pretends it didn't send a CONREQ,
			 *      sends a CONACK back.
			 * this makes the connection single-sided.
			 */

			if (pvmdebmask & PDMROUTE)
				pvmlogerror( "pvmmctl() handling crossed CONREQ\n");

			if (pvmmytid > src) {
				if (listen(pcbp->tt_fd, 1) == -1)
					pvmlogperror("pvm_tc_conreq() listen");

				else {
					pcbp->tt_state = TTGRNWAIT;
					pvm_fd_add(pcbp->tt_fd, 1);
					ackd = 0;
					if (buf[0] == '/') {
#ifdef NOUNIXDOM
						pvmlogprintf( "%s CONREQ from t%x, ",
								"pvm_tc_conreq()", src );
						pvmlogprintf(
								"Unix domain socket unsupported\n" );
#else
						addr = pcbp->tt_spath;
#endif
					} else {
						hex_inadport(buf, &pcbp->tt_osad);
						addr = inadport_hex(&pcbp->tt_sad);
					}
					check_routeadd(pcbp);

					sbf=pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
					ttpro = TDPROTOCOL;
					pvm_pkint(&ttpro, 1, 1);
					pvm_pkint(&ackd, 1, 1);
					pvm_pkstr(addr);
					i = pvmrescode;
					pvmrescode = 1;
					ictx = pvm_setcontext(SYSCTX_TC);
					pvm_send(src, TC_CONACK);
					pvm_setcontext(ictx);
					pvmrescode = i;
					pvm_freebuf(pvm_setsbuf(sbf));
				}
			}

		} else {
			pvmlogprintf(
					"pvm_tc_conreq() CONREQ from t%x but state=%d ?\n",
					src, pcbp->tt_state);
		}

	} else {
		if (pvmdebmask & PDMROUTE)
			pvmlogprintf("pvm_tc_conreq() CONREQ from t%x\n", src);
		ackd = 1;
		pcbp = ttpcb_creat(src);
		if (pvmrouteopt != PvmDontRoute) {
			if (buf[0] == '/') {

#ifdef NOUNIXDOM

				pvmlogprintf( "%s CONREQ from t%x, ",
						"pvm_tc_conreq()", src );
				pvmlogprintf( "Unix domain socket unsupported\n" );

#else /*NOUNIXDOM*/

				if ((pcbp->tt_fd = socket(AF_UNIX, SOCK_STREAM, 0))
						== -1) {
					pvmlogperror("pvm_tc_conreq() socket");

				} else {
					BZERO((char*)&uns, sizeof(uns));
					uns.sun_family = AF_UNIX;
					spath[0] = 0;
					(void)PVMTMPNAMFUN(spath);
					strcpy(uns.sun_path, spath);

					oslen = sizeof(uns);
					if (bind(pcbp->tt_fd, (struct sockaddr*)&uns, oslen)
							== -1) {
						pvmlogperror("pvm_tc_conreq() bind");

					} else {
						if (listen(pcbp->tt_fd, 1) == -1)
							pvmlogperror("pvm_tc_conreq() listen");

						else {
							pcbp->tt_state = TTGRNWAIT;
							pvm_fd_add(pcbp->tt_fd, 1);
							ackd = 0;
							addr = pcbp->tt_spath = STRALLOC(spath);

							if (pvmdebmask & PDMROUTE)
								pvmlogprintf( "%s: %s (t%x)\n",
									"pvm_tc_conreq()",
									"new route socket listening", src );

							/* new route socket listening, */
							/* will be accepted later */

							check_routeadd(pcbp);
						}
					}
				}

#endif /*NOUNIXDOM*/

			} else {
				if ((pcbp->tt_fd = socket(AF_INET, SOCK_STREAM, 0))
						== -1) {
					pvmlogperror("pvm_tc_conreq() socket");

				} else {
					pcbp->tt_sad = pvmourinet;
					oslen = sizeof(pcbp->tt_sad);
					if (bind(pcbp->tt_fd,
							(struct sockaddr*)&pcbp->tt_sad, oslen)
								== -1) {
						pvmlogperror("pvm_tc_conreq() bind");

					} else {
						if (getsockname(pcbp->tt_fd,
								(struct sockaddr*)&pcbp->tt_sad, &oslen)
									== -1) {
							pvmlogperror("pvm_tc_conreq() getsockname");

						} else {
							if (listen(pcbp->tt_fd, 1) == -1)
								pvmlogperror("pvm_tc_conreq() listen");

							else {
								hex_inadport(buf, &pcbp->tt_osad);
								pcbp->tt_state = TTGRNWAIT;
								pvm_fd_add(pcbp->tt_fd, 1);
								ackd = 0;
								addr = inadport_hex(&pcbp->tt_sad);

								if (pvmdebmask & PDMROUTE)
									pvmlogprintf( "%s: %s (t%x)\n",
										"pvm_tc_conreq()",
										"new route socket listening",
										src );

								/* new route socket listening, */
								/* will be accepted later */

								check_routeadd(pcbp);
							}
						}
					}
				}
			}
		}

		if (pvmdebmask & PDMROUTE)
			pvmlogprintf( "%s: sending CONACK to t%x\n",
				"pvm_tc_conreq()", src );

		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		ttpro = TDPROTOCOL;
		pvm_pkint(&ttpro, 1, 1);
		pvm_pkint(&ackd, 1, 1);
		pvm_pkstr(addr);
		i = pvmrescode;
		pvmrescode = 1;
		ictx = pvm_setcontext(SYSCTX_TC);
		pvm_send(src, TC_CONACK);
		pvm_setcontext(ictx);
		pvmrescode = i;
		pvm_freebuf(pvm_setsbuf(sbf));

		if (ackd)
			ttpcb_delete(pcbp);
	}
	pvm_freebuf(mid);
	return 0;
}


/*	pvm_tc_conack()
*
*	Another task replies to our connection request.
*
*	TC_CONACK() {
*		int tdprotocol		// t-d protocol revision number
*		int ack				// 0 ok, 1 denied
*		string sockaddr		// address of other socket
*	}
*/

static int
pvm_tc_conack(mid)
	int mid;
{
	int src;					/* sender of reply */
	int ttpro;					/* protocol revision */
	int ackd;					/* connection allowed (0) */
	struct ttpcb *pcbp;			/* pcb for connection */
	static int linger[2] = { 1, 60 };   /* XXX arbitrary time */
	int i;
#ifndef NOUNIXDOM
	struct sockaddr_un uns;
#endif
	char buf[256];

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);

	pvm_upkint(&ttpro, 1, 1);
	pvm_upkint(&ackd, 1, 1);
	pvm_upkstr(buf);

	if (pcbp = ttpcb_find(src)) {
		if (pcbp->tt_state == TTCONWAIT) {
			if (pvmdebmask & PDMROUTE) {
				pvmlogprintf(
						"pvm_tc_conack() CONACK from t%x ackd=%d\n",
						src, ackd );
			}
			if (ttpro != TDPROTOCOL) {
				pvmlogprintf("pvm_tc_conack() t-t protocol mismatch with t%x\n",
						pcbp->tt_tid);
				ackd = 1;

			} else {
				if (ackd != 0) {
					if (pvmdebmask & PDMROUTE) {
						pvmlogprintf("pvm_tc_conack() route to t%x denied\n",
								pcbp->tt_tid);
					}

				} else {
					if (buf[0] == '/') {
#ifdef NOUNIXDOM
						pvmlogprintf(
						"pvm_tc_conack() CONREQ from t%x, Unix domain socket unsupported\n",
								src);
						ackd = 1;
#else /*NOUNIXDOM*/
						BZERO((char*)&uns, sizeof(uns));
						uns.sun_family = AF_UNIX;
						strcpy(uns.sun_path, buf);
						while ((i = connect(pcbp->tt_fd, (struct sockaddr*)&uns,
								sizeof(uns))) == -1
								&& errno == EINTR)
							;
						if (i == -1) {
							pvmlogperror("pvm_tc_conack() connect");
							ackd = 1;

						} else {
							pcbp->tt_state = TTOPEN;
#ifndef WIN32	
							if ((i = fcntl(pcbp->tt_fd, F_GETFL, 0)) == -1)
								pvmlogperror("pvm_tc_conack() fcntl");
							else {
#ifdef O_NDELAY
								i |= O_NDELAY;
#else
								i |= FNDELAY;
#endif
								(void)fcntl(pcbp->tt_fd, F_SETFL, i);
							}
#endif
							pvm_fd_add(pcbp->tt_fd, 1);
						}

#endif /*NOUNIXDOM*/

					} else {
						pcbp->tt_osad.sin_family = AF_INET;
						hex_inadport(buf, &pcbp->tt_osad);
						while ((i = connect(pcbp->tt_fd,
								(struct sockaddr*)&pcbp->tt_osad,
								sizeof(pcbp->tt_osad))) == -1
								&& errno == EINTR)
							;
						if (i == -1) {
							pvmlogperror("pvm_tc_conack() connect");
							ackd = 1;

						} else {
							pcbp->tt_state = TTOPEN;
#ifndef NOSOCKOPT
							if (setsockopt(pcbp->tt_fd, SOL_SOCKET, SO_LINGER,
									(char*)linger, sizeof(linger)) == -1)
								pvmlogperror("pvm_tc_conack() setsockopt");
#endif /*NOSOCKOPT*/

#ifndef WIN32
							if ((i = fcntl(pcbp->tt_fd, F_GETFL, 0)) == -1)
								pvmlogperror("pvm_tc_conack() fcntl");
							else {
#ifdef O_NDELAY
								i |= O_NDELAY;
#else
								i |= FNDELAY;
#endif
								(void)fcntl(pcbp->tt_fd, F_SETFL, i);
							}
#endif
							pvm_fd_add(pcbp->tt_fd, 1);
						}
					}
				}
			}

			if (ackd != 0) {
				pcbp->tt_state = TTDENY;
				(void)close(pcbp->tt_fd);
				pcbp->tt_fd = -1;
			}
			else if (pvmdebmask & PDMROUTE) {
				pvmlogprintf( "%s: connection accepted to t%x\n",
					"pvm_tc_conack()", src );
			}

		} else {
			pvmlogprintf("pvm_tc_conack() CONACK from t%x but state=%d\n",
					src, pcbp->tt_state);
		}

	} else {
		pvmlogprintf("pvm_tc_conack() suprious CONACK from t%x\n", src);
	}

	pvm_freebuf(mid);
	return 0;
}


/*	pvm_tc_taskexit()
*
*	We are notified that another task (to which we have a direct route)
*	has exited.
*/

static int
pvm_tc_taskexit(mid)
	int mid;
{
	int tid;					/* exited task id */
	struct ttpcb *pcbp;

	pvm_upkint(&tid, 1, 1);

	if (pvmdebmask & PDMROUTE) {
		pvmlogprintf("pvm_tc_taskexit() TASKEXIT for t%x\n", tid);
	}

	/*
	* v3.3: for now, don't close fully open route as we may lose
	* messages.  rely on socket getting EOF
	*/
	if ((pcbp = ttpcb_find(tid))
	&& pcbp->tt_state != TTOPEN)
		ttpcb_dead(pcbp);

	pvm_freebuf(mid);
	return 0;
}


/*	mxinput()
*
*	Input from a connection.
*	Reassemble packets into messages and receive them.
*	Returns >= 0 the number of messages added to pvmrxlist,
*	or negative on error.
*/

static int
mxinput(pcbp, nfr)
	struct ttpcb *pcbp;
	int *nfr;				/* number of frags read */
{
	int gotem = 0;			/* num msgs added to pvmrxlist */
	struct frag *fp;		/* shadows pcbp->tt_rxf */
	struct frag *fp2;
	int n;					/* bytes received */
	int m;					/* length of fragment */
	struct pmsg *rxup;		/* message containing this frag */
	struct pmsg *hdfrgpile;
	char *cp;				/* gp */
	int src;
	int dst;
	int ff;
	static int fairprobe = 0;

	*nfr = 0;


#if defined(IMA_MPP)

	/* Messages from Tasks and  Messages from the daemon  are probed
	 * fairprobe makes sure that neither interface gets starved.  
	 * fairprobe is toggled when an EOM is found  
	*/

	switch (fairprobe)
	{
		/* pvm_readfrom<xxxx> returns a pointer to a frag, with its
		 *	length properly filled in. cp returns a pointer to the
		 *	the start of the data
		*/
		case 0:
			if ((fp = pvm_readfrompeer()) == (struct frag *) NULL) 
				fp = pvm_readfrompvmd();
			break;
		
		default:	
			if ((fp = pvm_readfrompvmd()) == (struct frag *) NULL) 
				fp = pvm_readfrompeer();
			break;

	}

	if (!fp) 
		return gotem;	/* didn't read anything */

#else
	if (!pcbp->tt_rxf)
		if (!(pcbp->tt_rxf = fr_new(pvmfrgsiz)))
			return PvmNoMem;
	fp = pcbp->tt_rxf;

	/* read fragment header separately from body */

	n = (fp->fr_len < TDFRAGHDR) ? 0 : pvmget32(fp->fr_dat + 8);
	n += TDFRAGHDR - fp->fr_len;
	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("mxinput() pcb t%x fr_len=%d fr_dat=+%d n=%d\n",
				pcbp->tt_tid, fp->fr_len, fp->fr_dat - fp->fr_buf, n);
	}
#ifndef WIN32
	n = read(pcbp->tt_fd, fp->fr_dat + fp->fr_len, n);
#else
	n = win32_read_socket( pcbp->tt_fd,fp->fr_dat+fp->fr_len,n);
#endif
	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("mxinput() read=%d\n", n);
	}

	/* deal with errors and closes */

	if (n == -1 && 
#ifndef WIN32
			errno != EWOULDBLOCK && 
#endif 
			errno != EINTR)
	{
		if (pvmdebmask & PDMPACKET) {
			pvmlogperror("mxinput() read");
			pvmlogprintf("mxinput() t%x\n", pcbp->tt_tid);
		}
		return PvmSysErr;
	}
	if (!n) {
		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf("mxinput() t%x read EOF\n", pcbp->tt_tid);
		}
		return -1;
	}

	if (n < 1)
		return gotem;
	if ((fp->fr_len += n) < TDFRAGHDR)
		return gotem;

	/* realloc buffer if frag won't fit */

	m = TDFRAGHDR + pvmget32(fp->fr_dat + 8);
	if (fp->fr_len == TDFRAGHDR) {
		if (m > fp->fr_max - (fp->fr_dat - fp->fr_buf)) {
			fp2 = fr_new(m);
			BCOPY(fp->fr_dat, fp2->fr_dat, TDFRAGHDR);
			fp2->fr_len = fp->fr_len;
			fr_unref(fp);
			fp = pcbp->tt_rxf = fp2;
			if (pvmdebmask & PDMPACKET) {
				pvmlogprintf("mxinput() realloc frag max=%d\n", m);
			}
		}
	}

#endif /* defined (IMA_MPP) */
	
	/* Now get information from the frag header to determine
	 * src, destination, length, etc.
	*/
#if !defined(IMA_MPP)
	if (fp->fr_len == m) 
#else
	if (fp)  /* got a frag */
#endif
	{
		(*nfr)++;
#if !defined (IMA_MPP)
		pcbp->tt_rxf = 0;
#endif
		cp = fp->fr_dat;
		dst = pvmget32(cp);
		src = pvmget32(cp + 4);
		ff = pvmget8(cp + 12);
		fp->fr_len -= TDFRAGHDR;
		fp->fr_dat += TDFRAGHDR;

		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf("mxinput() pkt src t%x len %d ff %d\n",
					src, fp->fr_len, ff);
	}

#if defined(IMA_MPP)
	if (fp -> fr_rip )	/* frag was received inplace */
	{
		gotem++;
		fr_unref(fp); /* free frag -- mppprecv knows that the msg completed */
		return gotem;
	}

	hdfrgpile = pvm_mpp_pmsgs();
#else
	hdfrgpile = pcbp->tt_rxfrag ;
#endif
	/*
	* if start of message, make new umbuf, add to frag pile
	*/
		if (ff & FFSOM) {
			cp += TDFRAGHDR;
			fp->fr_len -= MSGHDRLEN;
			fp->fr_dat += MSGHDRLEN;
			rxup = umbuf_new();
			rxup->m_enc = pvmget32(cp);
			rxup->m_tag = pvmget32(cp + 4);
			rxup->m_ctx = pvmget32(cp + 8);
			rxup->m_wid = pvmget32(cp + 16);
			rxup->m_crc = pvmget32(cp + 20);
			rxup->m_src = src;
			rxup->m_dst = dst;

			LISTPUTBEFORE(hdfrgpile, rxup, m_link, m_rlink);

		} else {
			/* locate frag's message */

			for (rxup = hdfrgpile->m_link; rxup != hdfrgpile; 
					rxup = rxup->m_link)
				if (rxup->m_src == src)
					break;
		}

		if (rxup == hdfrgpile) {	/* uh oh, no message for it */
			pvmlogerror("mxinput() frag with no message\n");
			fr_unref(fp);

		} else {
			LISTPUTBEFORE(rxup->m_frag, fp, fr_link, fr_rlink);
			rxup->m_len += fp->fr_len;
	/*
	* if end of message, move to pvmrxlist and count it
	*/
			if (ff & FFEOM) {
#if defined(IMA_MPP)
				fairprobe = (fairprobe ? 0 : 1);
#endif
				LISTDELETE(rxup, m_link, m_rlink);
				if (pvmdebmask & PDMMESSAGE) {
					pvmlogprintf(
							"mxinput() src t%x route t%x ctx %d tag %s len %d\n",
							rxup->m_src, (pcbp != 0 ? pcbp->tt_tid: -1), 
							rxup->m_ctx,
							pvmnametag(rxup->m_tag, (int *)0), rxup->m_len);
				}
#ifdef	MCHECKSUM
				if (rxup->m_crc != umbuf_crc(rxup)) {
					pvmlogprintf(
					"mxinput() message src t%x route t%x tag %s bad checksum\n",
							rxup->m_src, pcbp->tt_tid,
							pvmnametag(rxup->m_tag, (int *)0));
					umbuf_free(rxup);

				} else {
#endif
					pmsg_setenc(rxup, rxup->m_enc);
					mesg_input(rxup);
					gotem++;
#ifdef	MCHECKSUM
				}
#endif
			}
		}
	}
	return gotem;
}


/*	mxfer()
*
*	Move message frags between task and pvmd or other tasks.
*	Returns when
*		Outgoing message (if any) fully sent AND
*		(Timed out (tmout) OR
*			At least one message fully received) AND
*		No message from pvmd partially received.
*	Returns >= 0 the number of complete messages downloaded, or
*	negative on error.
*/

/* define a preallocated pool of inplace headers */
#define SHORT_PAYLOAD 128
#define HEADER_SIZE (TDFRAGHDR + MSGHDRLEN)
#define NINPLACE_HDRS (8096/(SHORT_PAYLOAD + HEADER_SIZE)) 
static char inPlaceHdrPool[NINPLACE_HDRS * ( HEADER_SIZE )];
static int hdrPoolStk = 0;

static int
mxfer(txup, tmout)
	struct pmsg *txup;			/* outgoing message or null */
	struct timeval *tmout;		/* time limit to get at least one message */
{
	struct ttpcb *txpcbp = 0;	/* route for ogm */
	struct frag *txfp = 0;		/* cur ogm frag or null */
	struct timeval tin;
	struct timeval tnow;
	struct timeval *tvp;
#ifdef FDSETNOTSTRUCT
	fd_set rfds, wfds;
#else
	struct fd_set rfds, wfds;
#endif
	int wantmore = 1;			/* nothing rxd yet and not timed out */
	int gotem = 0;				/* count of received msgs downloaded */
	int ff;
	int n;
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	unsigned int oslen;
#else
	size_t oslen;
#endif
#else
	int oslen;
#endif
	char *txcp = 0;				/* point to remainder of txfp */
	int txtogo = 0;				/* len of remainder of txfp */
	struct sockaddr_in sad;
	int s;
	struct ttpcb *pcbp;
	char *inPlaceHeader = (char  *) NULL;	/* for inplace data */
	int inPlaceBodyLen = 0;					/* len of inplace body */	
	int bypassRead = FALSE;				/* bypass reading packets before send?*/
	int waitForOgmToComplete = FALSE;
	int probedForIncomingPkts = FALSE;	/* need to probe for incoming pkts */ 
	struct msgid *sendmsg = (struct msgid *) NULL;
	char errtxt[64];

	int nfr = 0, totfr = 0;				/* number frags read my mxinput */
	int probe = 0;						/* called at a probe (nrecv) */

	if (tmout)
	{
		pvmgetclock(&tin);
		probe = ( tmout -> tv_sec == 0 && tmout -> tv_usec == 0);
	}

	if (txup) {	/* transmitting a umsg */
		txfp = txup->m_frag->fr_link;
		if (!txfp->fr_buf) {
			if (!(txfp = fr_new(MAXHDR)))
				return PvmNoMem;
			txfp->fr_dat += MAXHDR;
			LISTPUTBEFORE(txup->m_frag, txfp, fr_link, fr_rlink);
		}
		/* Get connection info for this dest. If none, go to pvmd */
		if (!(txpcbp = ttpcb_find(txup->m_dst)) || txpcbp->tt_state != TTOPEN)
			txpcbp = topvmd;
		txup->m_ref++;
		bypassRead = TRUE;		/* bypass reading the first time through */
	}

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("mxfer() mid %d", (txup ? txup->m_mid : 0));
		if (txup)
			pvmlogprintf(" ctx %d tag %s dst t%x route t%x",
					txup->m_ctx,
					pvmnametag(txup->m_tag, (int *)0),
					txup->m_dst, txpcbp->tt_tid);
		if (tmout)
			pvmlogprintf(" tmout %d.%06d\n",
					tmout->tv_sec, tmout->tv_usec);
		else
			pvmlogprintf(" tmout inf\n");
	}

/* XXX we go an extra time through select because wantmore gets set
   XXX inside loop.  no big deal.  also, could avoid calling gettimeofday
   XXX if tmout is 0.0 */

	/* Do this loop if
	 *		1) Sending a frag
	 * OR	2) Received *part* of an incoming frag from the daemon
	 * OR	3) Wantmore is true
	 * OR 	4) Need the current OGM to complete its asynch send 
	 * OR   5) We haven't probed at least once for incoming packets
	 *
	 * wantmore is initialized to  TRUE  -> loop exec'ed at least once 
	*/


	while (txfp || topvmd->tt_rxf || wantmore || waitForOgmToComplete
			|| !probedForIncomingPkts
	) 
	{

		totfr = 0; 

		/* Either we are sending something, or have a partial recv'd frag */
		if (txfp || topvmd->tt_rxf) 			/* gotta block */
		{
	 	 	/* must block if sending any message, or receiving from pvmd */
			tvp = (struct timeval *) NULL;

		} 
		else 
		{
			if (wantmore) 
			{
				/* Calculate how long to wait inside the "select"
				 *	tval = NULL means wait forever
				 *  tval = (0,0) means probe
				 * 	tval = (x,y) means wait at most x sec + y usec
				*/

				if (tmout) 					/* finite time to block */
				{
					pvmgetclock(&tnow);
					TVXSUBY(&tnow, &tnow, &tin);
					if (TVXLTY(tmout, &tnow)) 	/* time's up */
					{
						TVCLEAR(&tnow);
						wantmore = 0;
					} 
					else 
					{
						TVXSUBY(&tnow, tmout, &tnow);
					}
					tvp = &tnow;
				} 
				else 
				{						/* forever to block */
					tvp = (struct timeval *) NULL;
				}

			} 
			else 
			{							/* return asap */
				TVCLEAR(&tnow);
				tvp = &tnow;
			}
		}

		/* Here we are setting the read and write file descriptors for
		 *	the following select statement. 
		*/


#if !defined(IMA_MPP)
		rfds = pvmrfds;
		FD_ZERO(&wfds);
		if (txfp)
			FD_SET(txpcbp->tt_fd, &wfds);
#endif

		if (pvmdebmask & PDMSELECT) {
			if (tvp)
				pvmlogprintf("mxfer() select timeout %d.%06d\n",
						tvp->tv_sec, tvp->tv_usec);
			else
				pvmlogprintf("mxfer() select timeout inf\n");
#if !defined(IMA_MPP)
			print_fdset("mxfer() rfds=", pvmnfds, &rfds);
			print_fdset("mxfer() wfds=", pvmnfds, &wfds);
#endif
		}

#if !defined(IMA_MPP)
		probedForIncomingPkts = TRUE;
		if ((n = select(pvmnfds,
#ifdef	FDSETISINT
				(int *)&rfds, (int *)&wfds, (int *)0,
#else
				(fd_set *)&rfds, (fd_set *)&wfds, (fd_set*)0,
#endif
				tvp)) == -1
		&& errno != EINTR
#ifdef IMA_LINUX
		&& errno != EAGAIN
#endif
		) {
			pvmlogperror("mxfer() select");
			return PvmSysErr;
		}
		if (pvmdebmask & PDMSELECT) {
			pvmlogprintf("mxfer() select returns %d\n", n);
		}
		if (n == -1)
			continue;
	/*
	* if pvmd conn has data ready, read packets
	*/

		if (FD_ISSET(topvmd->tt_fd, &rfds) 
			&& !(mxfersingle && gotem)) 
		{
			if ((n = mxinput(topvmd, &nfr)) < 0) {
				if (n != -1)
					pvmlogerror("mxfer() mxinput bad return on pvmd sock\n");
				else if (pvmdebmask & (PDMSELECT|PDMMESSAGE|PDMPACKET))
					pvmlogerror("mxfer() EOF on pvmd sock\n");
				return PvmSysErr;
			}

			totfr += nfr;
			if (gotem += n)
				wantmore = 0;
		}

	/*
	* if task conn has data ready, read packets
	*/

	/* This goes through *all* of the pcbs that have been defined to
	 *	check for messages.  This is too much overhead for IMA_MPP
	 *	machines. Instead all local routes must be probed at one time.
	*/ 
		
		for (pcbp = ttlist->tt_link; pcbp != ttlist; pcbp = pcbp->tt_link) {
			if (pcbp->tt_state == TTOPEN && FD_ISSET(pcbp->tt_fd, &rfds)) {
				if ((n = mxinput(pcbp, &nfr)) < 0)
					ttpcb_dead(pcbp);

				else {
					totfr += nfr;
					if (gotem += n)
						wantmore = 0;
				}

			} else /* Try and complete a direct (socket) Connection rqst */ 
				if (pcbp->tt_state == TTGRNWAIT
				&& FD_ISSET(pcbp->tt_fd, &rfds)) {
					oslen = sizeof(sad);
					s = accept(pcbp->tt_fd, (struct sockaddr*)&sad, &oslen);
					if (s == -1) {
						pvmlogperror("mxfer() accept");
						ttpcb_dead(pcbp);

					} else {
#ifndef NOSOCKOPT
#ifndef NOUNIXDOM
						if (!pcbp->tt_spath) {	/* not unix sock */
#endif
							n = 1;
							if (setsockopt(s, IPPROTO_TCP, TCP_NODELAY,
									(char*)&n, sizeof(int)) == -1)
								pvmlogperror("mxfer() setsockopt");
#if (!defined(IMA_RS6K)) \
		&& (!defined(IMA_AIX46K)) && (!defined(IMA_AIX46K64)) \
		&& (!defined(IMA_AIX56K)) && (!defined(IMA_AIX56K64))
							n = TTSOCKBUF;
							if (setsockopt(s, SOL_SOCKET, SO_SNDBUF,
									(char*)&n, sizeof(int)) == -1
							|| setsockopt(s, SOL_SOCKET, SO_RCVBUF,
									(char*)&n, sizeof(n)) == -1)
								pvmlogperror(
									"mxfer() setsockopt SO_SNDBUF");
#endif
#ifndef NOUNIXDOM
						}
#endif
#endif /*NOSOCKOPT*/
						if (pvmdebmask & PDMROUTE) {
							pvmlogprintf("mxfer() accept from t%x = %d\n",
									pcbp->tt_tid, s);
						}
						(void)close(pcbp->tt_fd);
/*
						(void)dup2(s, pcbp->tt_fd);
						(void)close(s);
*/
						pvm_fd_delete(pcbp->tt_fd, 1);

	/* notify route socket closed */

						check_routedelete(pcbp);
						pcbp->tt_fd = s;
						pcbp->tt_state = TTOPEN;

#ifndef WIN32
						{
							int i;

							if ((i = fcntl(pcbp->tt_fd, F_GETFL, 0))
									== -1)
								pvmlogperror("mxfer() fcntl");
							else {
#ifdef O_NDELAY
								i |= O_NDELAY;
#else
								i |= FNDELAY;
#endif
								(void)fcntl(pcbp->tt_fd, F_SETFL, i);
							}
						}
#endif

						pvm_fd_add(s, 1);

	/* new route socket created for communication */

						check_routeadd(pcbp);
					}
				}
		}

		bypassRead = bypassRead; /* sgi compiler */

#else	/* !IMA_MPP */

		/* For Native Messaging mxinput is used to probe pvmd AND tasks when 
		 * given a NULL ttpcb pointer 
		*/

		if (!bypassRead)
		{
	
			probedForIncomingPkts = TRUE;
			if ((n = mxinput((struct ttpcb *) NULL, &nfr)) < 0) {
					pvmlogerror("mxfer() mxinput bad return on probe\n");
					return PvmSysErr;
				}

			totfr += nfr;
			if (gotem += n)
				wantmore = 0;
		}
		

#endif /* !IMA_MPP */
	/*
	* if sending and socket ready to write, send packets
	*/

#if !defined(IMA_MPP)
		/* XXX Native message direct routes are alway open, if they exist */

		if (txfp && txpcbp->tt_state != TTOPEN) 
		{
		/* make sure our route hasn't just been closed */
			txpcbp = topvmd;
			txcp = 0;
			txtogo = 0;
			txfp = txup->m_frag->fr_link;

		} 
		else 
#endif
		{
			if (txfp 
				&& !waitForOgmToComplete
#if !defined(IMA_MPP)
				&& FD_ISSET(txpcbp->tt_fd, &wfds) 
#endif
			){
				inPlaceHeader = (char *) NULL; 		/*sending inplace header ?*/
				inPlaceBodyLen = 0;	   /* length of inplace body, if inplace*/

				if (!txtogo)  /* We haven't sent anything, yet, Build Header */
				{
					if (txfp->fr_u.dab)			/* packed data */
					{
						txcp = txfp->fr_dat;
					}
					else						/* inplace data */
					{
						txcp = inPlaceHeader = inPlaceHdrPool + 
								hdrPoolStk*(HEADER_SIZE+SHORT_PAYLOAD) + HEADER_SIZE; 
									
						if (pvmdebmask & PDMPACKET)
						{
							sprintf(errtxt,"mxfer() hdrPoolStk is %d\n", hdrPoolStk);
							pvmlogerror(errtxt);
						}

						if (hdrPoolStk++ >= NINPLACE_HDRS)
						{
							sprintf(errtxt, "mxfer(): > %d inplace frags \n",
									NINPLACE_HDRS);
							pvmlogerror(errtxt);
							return PvmSysErr;
						}

					}

					txtogo = txfp->fr_len;

					/*
				 	 * if this is first frag, prepend message header
					*/
					ff = 0;
					if (txfp->fr_rlink == txup->m_frag) 
					{
						txcp -= MSGHDRLEN;
						txtogo += MSGHDRLEN;
						pvmput32(txcp,
							(txup->m_enc == 0x20000000 ? pvmmydsig : txup->m_enc));
						pvmput32(txcp + 4, txup->m_tag);
						pvmput32(txcp + 8, txup->m_ctx);
						pvmput32(txcp + 16, txup->m_wid);
#ifdef	MCHECKSUM
						pvmput32(txcp + 20, umbuf_crc(txup));
#else
						pvmput32(txcp + 20, 0);
#endif
						ff = FFSOM;
					}
					if (txfp->fr_link == txup->m_frag)
						ff |= FFEOM;
					/*
					 * prepend frag header
					*/
					txcp -= TDFRAGHDR;
					pvmput32(txcp, txup->m_dst);
					pvmput32(txcp + 4, pvmmytid);
					pvmput32(txcp + 8, txtogo);
					pvmput32(txcp + 12, 0);		/* to keep putrify happy */
					pvmput8(txcp + 12, ff);
					txtogo += TDFRAGHDR;
					if (!txfp->fr_u.dab)			/* inplace data */
					{
						txtogo -= txfp->fr_len;
						inPlaceBodyLen = txfp->fr_len;
					}
				}

				if (pvmdebmask & PDMPACKET) 
				{
					pvmlogprintf("mxfer() dst t%x n=%d\n", txup->m_dst, txtogo);
				}
				
#if defined(IMA_MPP)

				/* node_send will do an async send. Feed it a midlist struct. 
				 * when we get to the end of a message we call ogm_done
				 * to make sure message is really gone, and we can free	
				 * memory 
				 *	
				*/
				n = pvm_node_send(txcp, txtogo, txpcbp, &sendmsg, 
							inPlaceHeader, inPlaceBodyLen);
#else 
				inPlaceBodyLen = inPlaceBodyLen; /* sgi compiler */

#if defined(IMA_RS6K) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
				n = write(txpcbp->tt_fd, txcp, min(txtogo, 4096));
#else
#ifndef WIN32
				n = write(txpcbp->tt_fd, txcp, txtogo);
#else 
				n = win32_write_socket(txpcbp->tt_fd,txcp,txtogo);
#endif
#endif

#endif
				if (pvmdebmask & PDMPACKET) {
					pvmlogprintf("mxfer() wrote %d\n", n);
				}
				if (n == -1 && 
#ifndef WIN32
						errno != EWOULDBLOCK && 
#endif
						errno != EINTR) 
				{
					if (txpcbp == topvmd) 
					{
						pvmlogperror("mxfer() write pvmd sock");
						return PvmSysErr;
					} 
					else 
					{
						pvmlogperror("mxfer() write tt sock");
						/* reset message and route to pvmd */
						ttpcb_dead(txpcbp);
						txpcbp = topvmd;
						txcp = 0;
						txtogo = 0;
						txfp = txup->m_frag->fr_link;
					}
				}

				if (n > 0) 
				{
					txcp += n;
					if ((txtogo -= n) <= 0) 
					{
						if (txcp == inPlaceHeader && txfp->fr_len ) 	
						/* inplace, body length > 0  */
						{
							txcp = txfp->fr_dat;
							txtogo = txfp->fr_len;
							bypassRead = TRUE; /* try to send body without 
													doing another read */ 
						}
						else						/* packed */ 
						{
							txcp = 0;
							txfp = txfp->fr_link;
							if (!txfp->fr_buf) {
								txfp = 0;
								waitForOgmToComplete = TRUE;
								bypassRead = FALSE;
							}
						}
					}
				} 
				else /* we couldn't send, might be a head-head send so ... */ 
				{
					bypassRead = FALSE; /* enable reading if couldn't send */
				}
			}

		}

		/* We are waiting for an outgoing message to complete. We
			can't send the next message until it does complete */

		if (waitForOgmToComplete)
		{
			if (ogm_complete(&sendmsg))
			{
				pmsg_unref(txup);	/* unref the message */
				txup = 0;
				waitForOgmToComplete = FALSE;
				hdrPoolStk = 0;		/* recover all the inplace headers */
			}
		}

	/* if not sending, and not waiting for ogm to complete,
		flush anything waiting in the reentered-send queue */

		if (!txfp && txrp != txwp && !waitForOgmToComplete) {
			txup = txlist[txrp];
			if (++txrp >= sizeof(txlist)/sizeof(txlist[0]))
				txrp = 0;

			txfp = txup->m_frag->fr_link;
			txfp = txfp->fr_buf ? txfp : 0;
			if (!(txpcbp = ttpcb_find(txup->m_dst))
			|| txpcbp->tt_state != TTOPEN)
				txpcbp = topvmd;
		}

		if (pvmdebmask & PDMMESSAGE) {
			pvmlogprintf("mxfer() txfp %d gotem %d tt_rxf %d\n",
					(txfp ? 1 : 0), gotem, (topvmd->tt_rxf ? 1 : 0));
		}

		if (probe && !gotem && totfr)		/* got frags, keep reading */
			wantmore = 1;
	}

	return gotem;
}


/*	mroute()
*
*	Route a message to a destination / Receive incoming messages.
*	Returns when
*		Outgoing message (if any) fully sent AND
*		(Timed out waiting (tmout) OR
*			At least one message fully received)
*	Returns >= 0 the number of complete messages downloaded
*	(although some may have been consumed by message-handlers),
*	or negative on error.
*
*	If mroute() is reentered (mroute -> mxfer -> handler -> pvm_send),
*	it queues the message and returns immediately.
*	The enclosing mxfer() later sends any messages in this queue.
*/

int
mroute(mid, dtid, tag, tmout)
	int mid;				/* message (or 0) */
	int dtid;				/* dest */
	int tag;				/* type tag */
	struct timeval *tmout;	/* get at least one message */
{
	static int alreadyhere = 0;	/* have i called myself? */

	struct ttpcb *pcbp;			/* pcb for foreign task */
	int s;						/* socket */
	int sbf;					/* temp TC_CONREQ message */
	struct pmsg *up, *up2;
	struct sockaddr_in sad;
	int l;
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	unsigned int oslen;
#else
	size_t oslen;
#endif
#else
	int oslen;
#endif
	int cc = 0;
	int gotem = 0;				/* count of received messages */
	static struct timeval ztv = { 0, 0 };
	char *addr = 0;
#ifndef NOUNIXDOM
	struct sockaddr_un uns;
	char spath[PVMTMPNAMLEN];
#endif

	if (up = midtobuf(mid)) {
		up->m_dst = dtid;
		up->m_tag = tag;
	}

	if (alreadyhere) {
		if (up) {
			up->m_ref++;
			txlist[txwp] = up;
			if (++txwp >= sizeof(txlist)/sizeof(txlist[0]))
				txwp = 0;
			if (txwp == txrp)
				abort();
		}

		/* we can't wait around if reentered */

		if (!tmout || tmout->tv_sec || tmout->tv_usec)
			return PvmNotImpl;

		return 0;
	}

	alreadyhere = 1;

	/* if sending a message, decide whether we need to route it. */

	/* Direct Routing is being set up in this section of code  
     * For workstations (not MPP, SHMEM) 
	 *	either Unix domain or TCP sockets are used.
	 *  task exit notifies are set up for route closure
	 *
	 * For MPPs and SHMEM,
 	 *	connections to local tasks are always direct and require no
	 *	handshake negotiation. 
	*/

	if (up) {        /*  We've got something to send */ 

#if !defined(IMA_MPP) /* This is the workstation direct route setup */

		/* Checks made (Workstation):
	 	 *	o route option is direct
		 *	o destination is given 
		 *	o destination is a task
		 *  o destination is not me
		 *  o could not find task process control block 	
		*/

		if (pvmrouteopt == PvmRouteDirect
			&& dtid
			&& TIDISTASK(dtid)
			&& dtid != pvmmytid
			&& !ttpcb_find(dtid))
		{

#if !defined(NOUNIXDOM)
			/* Create a UNIX domain socket to build the direct route */

			if ((pvmmytid & TIDHOST) == (dtid & TIDHOST)) {
				if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
					pvmlogperror("mroute() socket");

				} else {
					BZERO((char*)&uns, sizeof(uns));
					uns.sun_family = AF_UNIX;
					spath[0] = 0;
					(void)PVMTMPNAMFUN(spath);
					strcpy(uns.sun_path, spath);

					oslen = sizeof(uns);
					if (bind(s, (struct sockaddr*)&uns, oslen) == -1) {
						pvmlogperror("mroute() bind");
						(void)close(s);
						s = -1;

					} else {
						pcbp = ttpcb_creat(dtid);
						pcbp->tt_fd = s;
						addr = pcbp->tt_spath = STRALLOC(spath);
						pcbp->tt_state = TTCONWAIT;
					}
				}

			} else {
#endif /* !NOUNIXDOM */
			/* Open a TCP direct Connection here, instead */

				if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
					pvmlogperror("mroute() socket");

				} else {
					sad = pvmourinet;
					oslen = sizeof(sad);
					if (bind(s, (struct sockaddr*)&sad, oslen) == -1) {
						pvmlogperror("mroute() bind");
						(void)close(s);
						s = -1;

					} else {
						if (getsockname(s, (struct sockaddr*)&sad, &oslen) == -1) {
							pvmlogperror("mroute() getsockname");
							(void)close(s);
							s = -1;

						} else {
#ifndef NOSOCKOPT
							l = 1;
							if (setsockopt(s, IPPROTO_TCP, TCP_NODELAY,
									(char*)&l, sizeof(int)) == -1)
								pvmlogperror("mroute() setsockopt");
#if (!defined(IMA_RS6K)) \
		&& (!defined(IMA_AIX46K)) && (!defined(IMA_AIX46K64)) \
		&& (!defined(IMA_AIX56K)) && (!defined(IMA_AIX56K64))
							l = TTSOCKBUF;
							if (setsockopt(s, SOL_SOCKET, SO_SNDBUF,
									(char*)&l, sizeof(l)) == -1
							|| setsockopt(s, SOL_SOCKET, SO_RCVBUF,
									(char*)&l, sizeof(l)) == -1)
								pvmlogperror("mroute() setsockopt SO_SNDBUF");
#endif
#endif
							pcbp = ttpcb_creat(dtid);
							pcbp->tt_fd = s;
							pcbp->tt_sad = sad;
							pcbp->tt_state = TTCONWAIT;
							addr = inadport_hex(&sad);
						}
					}
				}
#if !defined(NOUNIXDOM)
			}
#endif

			/* 1. Request direct connection  (TC_CONREQ)
			 * 2. Request Notify so that we can close dead routes 
		 	*/	 		 
			if (s >= 0) {  /* Socket was opened correctly */ 

		/* Send Connection Request */
				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
				l = TDPROTOCOL;
				pvm_pkint(&l, 1, 1);
				pvm_pkstr(addr);
				sbf = pvm_setsbuf(sbf);
				up2 = midtobuf(sbf);
				up2->m_dst = dtid;
				up2->m_tag = TC_CONREQ;
				up2->m_ctx = SYSCTX_TC;
				cc = mxfer(up2, &ztv);
				pvm_freebuf(sbf);

		/* now send a task exit notify to my pvmd */
				if (cc >= 0) {
					gotem += cc;
					sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
					l = PvmTaskExit;
					pvm_pkint(&l, 1, 1);
					l = SYSCTX_TC;
					pvm_pkint(&l, 1, 1);
					l = TC_TASKEXIT;
					pvm_pkint(&l, 1, 1);
					l = 1;
					pvm_pkint(&l, 1, 1);
					pvm_pkint(&dtid, 1, 1);
					sbf = pvm_setsbuf(sbf);
					up2 = midtobuf(sbf);
					up2->m_dst = TIDPVMD;
					up2->m_tag = TM_NOTIFY;
					up2->m_ctx = SYSCTX_TM;
					cc = mxfer(up2, &ztv);
					pvm_freebuf(sbf);
				}

				if (pvmdebmask & PDMROUTE) {
					pvmlogprintf("mroute() CONREQ to t%x\n", dtid);
				}

	/* wait until the route pcb changes state */

				while (pcbp->tt_state == TTCONWAIT) {
					if ((cc = mxfer((struct pmsg *)0, (struct timeval *)0)) < 0)
						break;
					gotem += cc;
				}
				if (pcbp->tt_state == TTOPEN)
					check_routeadd(pcbp);
			}
		}
#endif /* IMA_MPP */
	}

	/* now send the original message with whatever route we have */

	if (cc >= 0)
		if ((cc = mxfer(up, tmout)) > 0)
			gotem += cc;

	/* clean up any stale route pcbs */

#if !defined(IMA_MPP)
	pcbp = ttlist->tt_link;
	while (pcbp != ttlist)
		if (pcbp->tt_state == TTDEAD) {
			if (pvmdebmask & PDMROUTE) {
				pvmlogprintf("mroute() freeing pcb t%x\n", pcbp->tt_tid);
			}
			pcbp = pcbp->tt_link;
			ttpcb_delete(pcbp->tt_rlink);
		} else
			pcbp = pcbp->tt_link;
#endif /* IMA_MPP */

	alreadyhere = 0;
	return (cc < 0 ? cc : gotem);
}


/*	msendrecv()
*
*	Single op to send a system message (usually to our pvmd) and get
*	the reply.
*	Returns message handle or negative if error.
*/

int
msendrecv(other, tag, context)
	int other;				/* dst, src tid */
	int tag;				/* message tag */
	int context;			/* context to use for the message */
{
	static int nextwaitid = 1;

	int cc;
	struct pmsg *up;

	if (!pvmsbuf)
		return PvmNoBuf;

	/* send message to other */
	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("msendrecv() to t%x tag %s\n", other,
				pvmnametag(tag, (int *)0));
	}

	pvmsbuf->m_wid = nextwaitid++;
	pvmsbuf->m_ctx = context;
/*
	pvmlogprintf("msendrecv() ctx=%d\n", pvmsbuf->m_ctx);
*/
	if ((cc = mroute(pvmsbuf->m_mid, other, tag, (struct timeval *)0)) < 0)
		return cc;

	/* recv matching message from other */
	for (up = pvmrxlist->m_link; 1; up = up->m_link) {
		if (up == pvmrxlist) {
			up = up->m_rlink;
			if ((cc = mroute(0, 0, 0, (struct timeval *)0)) < 0)
				return cc;
			up = up->m_link;
			if (up == pvmrxlist)
				continue;
		}

		if (pvmdebmask & PDMMESSAGE) {
			pvmlogprintf("msendrecv() from t%x tag %s\n",
					up->m_src, pvmnametag(up->m_tag, (int *)0));
		}
		if (up->m_src == other && up->m_tag == tag) {
			if (up->m_ctx != pvmsbuf->m_ctx) {
				pvmlogprintf("msendrecv() tag %s, context doesn't match\n",
						pvmnametag(up->m_tag, (int *)0));
			}
			if (up->m_wid != pvmsbuf->m_wid) {
				pvmlogprintf("msendrecv() tag %s, waitid doesn't match\n",
						pvmnametag(up->m_tag, (int *)0));
			}
			break;
		}
	}
	LISTDELETE(up, m_link, m_rlink);
	if (pvmrbuf)
		umbuf_free(pvmrbuf);
	pvmrbuf = 0;
	if (cc = pvm_setrbuf(up->m_mid))
		return cc;
	return up->m_mid;
}

static int
int_compare(i, j)
#if defined (WIN32) || defined(IMA_SGI) || defined(IMA_SGI5) || defined(IMA_SGI6) || defined(IMA_SGI64) || defined(IMA_SGIMP) || defined(IMA_SGIMP6) || defined(IMA_SGIMP64) || defined(IMA_SUN4SOL2)
	const void *i, *j;
#else
	void *i, *j;
#endif
{
	return *((int *)i) - *((int *)j);
}


/*	pvmmcast()
*
*	Multicast a message to a list of destinations.
*/

int
pvmmcast(mid, tids, count, tag)
	int mid;	
	int *tids;
	int count;
	int tag;
{
	static struct timeval ztv = { 0, 0 };

	int *dst;
	int i, j;
	int cc = 0;
	struct ttpcb *pcbp;
	int sbf;

	/*
	* make sorted list of destinations
	*/
	dst = TALLOC(count, int, "mcal");
	BCOPY(tids, dst, count * sizeof(int));

	qsort(
		(void *)dst, 
		(size_t)count, 
		sizeof(int),
		int_compare);

	/*
	* remove duplicates
	*/
	j = 0;
	for (i = 1; i < count; i++)
		if (dst[i] != dst[j])
			dst[++j] = dst[i];
	count = j + 1;

	/* Need to set the context of the message buffer */
	pvmsbuf->m_ctx = pvmmyctx;

	/*
	* remove self from list
	* send over any direct routes we have
	*
	* XXX we should attempt new routes here if RouteDirect is on.
	*/
	j = 0;
	for (i = 0; i < count; i++) {
		if (dst[i] == pvmmytid)
			continue;
		if ((pcbp = ttpcb_find(dst[i])) && pcbp->tt_state == TTOPEN)
			mroute(pvmsbuf->m_mid, dst[i], tag, &ztv);
		else
			dst[j++] = dst[i];
	}
	count = j;		/* destinations still to go */

	/*
	* send rest of addresses to pvmd for distribution
	*/
	if (count > 0) {
		/* announce multicast address list to pvmd */
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		pvm_pkint(&count, 1, 1);
		pvm_pkint(dst, count, 1);
		sbf = pvm_setsbuf(sbf);
		if ((cc = mroute(sbf, TIDPVMD, TM_MCA, &ztv)) > 0)
			cc = 0;
		pvm_freebuf(sbf);

		/* send message */
		if (cc >= 0)
			if ((cc = mroute(pvmsbuf->m_mid, pvmmytid | TIDGID, tag, &ztv)) > 0)
				cc = 0;
	}

	PVM_FREE(dst);
	return cc;
}


/*	mksocs()
*
*	Make socket for talking to pvmd.  Connect to address advertized
*	in pvmd socket-name file.
*
*	Returns 0 if okay, else error code.
*/

static int
mksocs()
{
	char buf[128];
#ifndef WIN32
	int d;
#else
	HANDLE d;
#endif

#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	unsigned int oslen;
#else
	size_t oslen;
#endif
#else
	int oslen;
#endif
	int n;
	int try;
	char *p;
#ifndef NOUNIXDOM
	struct sockaddr_un uns;
#endif

	if (topvmd)
		return 0;

	/*
	* get addr of pvmd, make socket to talk.
	* first try envar PVMSOCK, then try sockaddr file.
	*/

	if (!(p = getenv("PVMSOCK"))) {
		if (!(p = pvmdsockfile())) {
			pvmlogerror("mksocs() pvmdsockfile() failed\n");
			goto bail;
		}
#ifndef WIN32
		if ((d = open(p, O_RDONLY, 0)) == -1) {
#else 
		d = win32_open_file(p);
		if (d == (HANDLE) -2) {
			system_loser_win=TRUE;
			d = (HANDLE) _open(p,O_RDONLY,0);
		}
		if (d== (HANDLE) -1) {
#endif
			pvmlogperror(p);
			goto bail;
		}
#ifdef WIN32
		if (!system_loser_win) {
			n = win32_read_file(d,buf,sizeof(buf));
			win32_close_file(d);
		}
		else  {
			n = (int)_read((int)d, (void *)buf, (unsigned int)sizeof(buf));
			(void)_close((int)d);
		}
#else
		n = read(d, buf, sizeof(buf));
		(void)close(d);
#endif
		if (n == -1) {
			pvmlogperror("mksocs() read addr file");
			goto bail;
		}
		if (n == 0) {
			pvmlogerror("mksocs() read addr file: wrong length read\n");
			goto bail;
		}
		buf[n] = 0;
		p = buf;
	}

	FD_ZERO(&pvmrfds);
/*
	FD_ZERO(&pvmwfds);
*/
	pvmnfds = 0;

	topvmd = ttpcb_new();
	topvmd->tt_tid = TIDPVMD;

	if (p[0] == '/') {
#ifdef NOUNIXDOM
		pvmlogerror("mksocs() no support for Unix domain socket\n");
		goto bail;

#else /*NOUNIXDOM*/
		if ((topvmd->tt_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
			pvmlogperror("mksocs() socket");
			goto bail;
		}

	/*
	* XXX these 2 loops are a hack -
	* XXX it keeps too many tasks from overflowing the pvmd listen q.
	* XXX maybe the pvmd should make a socket pair for the task instead.
	*/
		try = 5;
		while (1) {
			BZERO((char*)&uns, sizeof(uns));
			uns.sun_family = AF_UNIX;
			strcpy(uns.sun_path, p);
			n = sizeof(uns);
			if (connect(topvmd->tt_fd, (struct sockaddr*)&uns, n) == -1) {
				if (--try <= 0) {
					pvmlogperror("mksocs() connect");
					pvmlogprintf("\tsocket address tried: %s\n",p);
					goto bail;
				}
				pvmsleep(1);	/* XXX hmm again */

			} else
				break;
		}
#endif /*NOUNIXDOM*/

	} else {
		if ((topvmd->tt_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			pvmlogperror("mksocs() socket");
			goto bail;
		}

		try = 5;
		while (1) {	/* goto is at the root of all good programming style */
			hex_inadport(p, &topvmd->tt_osad);
			topvmd->tt_osad.sin_family = AF_INET;
			n = sizeof(topvmd->tt_osad);

			if (connect(topvmd->tt_fd, (struct sockaddr*)&topvmd->tt_osad, n)
			== -1) {
				if (--try <= 0) {
					pvmlogperror("mksocs() connect");
					pvmlogprintf("\tsocket address tried: %s\n",p);
					goto bail;
				}
				pvmsleep(1);	/* XXX hmm again */
			} else
				break;
		}

#ifndef NOSOCKOPT
#ifdef WIN32
		d = (HANDLE)1;
#else
		d = 1;
#endif
		if (setsockopt(topvmd->tt_fd, IPPROTO_TCP, TCP_NODELAY, (char*)&d, sizeof(int))
		== -1) {
			pvmlogperror("mksocs() setsockopt");
			goto bail;
		}
#endif
		oslen = sizeof(topvmd->tt_sad);
		if (getsockname(topvmd->tt_fd, (struct sockaddr*)&topvmd->tt_sad, &oslen) == -1) {
			pvmlogperror("mksocs() getsockname");
			goto bail;
		}
	}

	topvmd->tt_state = TTOPEN;
	pvm_fd_add(topvmd->tt_fd, 1);

	return 0;

bail:
	if (topvmd)
		ttpcb_delete(topvmd);
	topvmd = 0;
	return PvmSysErr;
}


/*	unmksocs()
*
*	Close socket to local pvmd.
*/

static int
unmksocs()
{
	if (!topvmd)
		return 1;
	while (ttlist->tt_link != ttlist)
		ttpcb_delete(ttlist->tt_link);
	ttpcb_delete(topvmd);
	topvmd = 0;
	return 0;
}


/*	pvmbeatask()
*
*	Initialize libpvm, config process as a task.
*	This is called as the first step of each libpvm function so no
*	explicit initialization is required.
*
*	Makes socket to talk to local pvmd, connects to pvmd, trades
*	config information and authentication.
*
*	Returns 0 if okay, else error code.
*/

int
pvmbeatask()
{
	int sbf = 0, rbf = 0;			/* saved rx and tx message handles */
	int prver;						/* protocol version */
	int cookie;						/* cookie assigned by pvmd for ident */
	int cc;
	char **ep=0;
	char authfn[PVMTMPNAMLEN];	/* auth file name */
	int authfd = -1;				/* auth fd to validate pvmd ident */
	int i;
	char buf[16];					/* for converting sockaddr */
	char *p;
	struct pvmminfo minfo;
	int outtid, outctx, outtag;
	int trctid, trcctx, trctag;
	int need_trcinfo = 0;
	int new_tracer = 0;
	char tmask[ 2 * TEV_MASK_LENGTH ];
	int tbuf, topt;
	int mid;
#ifdef IMA_BEOLIN
	struct hostent *hostaddr;
	char namebuf[128];
#endif
	TEV_DECLS
#ifdef WIN32

   /* 	WSAStartup has to be called before any socket command 
   		can be executed. Why ? Ask Bill
   */
	for (ep=environ;*ep;ep++)
		pvmputenv(*ep);   
   if (WSAStartup(0x0101, &WSAData) != 0)
	{
      fprintf(stderr,"\nWSAStartup() failed\n");
      ExitProcess(1);
	}
 	/* setsockopt(INVALID_SOCKET,SOL_SOCKET,SO_OPENTYPE, */
	/*	(char *)&nAlert,sizeof(int)); */

#endif
/*
	pvm_setopt(PvmDebugMask, -1);
*/
	if (pvmmytid != -1)
		return 0;

	pvmmydsig = pvmgetdsig();

	TEV_EXCLUSIVE;

	authfn[0] = 0;

#ifndef WIN32
	pvmmyupid = getpid();
#else
	pvmmyupid = _getpid();     /* .... */
#endif

	/*
	* get expected pid from environment in case we were started by
	* the pvmd and someone forked again
	*/

	if (p = getenv("PVMEPID"))
		cookie = atoi(p);
	else
		cookie = 0;

	/* if ((pvm_useruid = getuid()) == -1) */

#ifndef WIN32
	if ((pvm_useruid = geteuid()) == -1) {
		pvmlogerror("can't getuid()\n");
		cc = PvmSysErr;
		goto bail2;
	}
	pvmchkuid( pvm_useruid );
#else
	if (!username)
		username = MyGetUserName();
#endif

	if (p = getenv("PVMTASKDEBUG")) {
		pvmdebmask = pvmstrtoi(p);
		if (pvmdebmask) {
			pvmlogprintf("task debug mask is 0x%x (%s)\n",
					pvmdebmask, debug_flags(pvmdebmask));
		}
	}

#ifndef IMA_MPP
	if (cc = mksocs())		/* make socket to talk to pvmd */
		goto bail2;
#endif

	/*
	*	initialize received-message list
	*/

	pvmrxlist = pmsg_new(1);
	BZERO((char*)pvmrxlist, sizeof(struct pmsg));
	pvmrxlist->m_link = pvmrxlist->m_rlink = pvmrxlist;

	ttlist = TALLOC(1, struct ttpcb, "tpcb");
	BZERO((char*)ttlist, sizeof(struct ttpcb));
	ttlist->tt_link = ttlist->tt_rlink = ttlist;

	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
	rbf = pvm_setrbuf(0);

	/*
	*	create empty t-auth file that pvmd must write and we'll check later
	*/

#ifndef IMA_MPP
#ifndef NOPROT
	(void)PVMTMPNAMFUN(authfn);
#ifdef IMA_OS2
	if ((authfd = open(authfn, O_RDWR|O_CREAT|O_EXCL|O_TRUNC, 0600)) == -1)
#else
	if ((authfd = open(authfn, O_RDONLY|O_CREAT|O_EXCL|O_TRUNC, 0600)) == -1)
#endif
	{
		pvmlogperror(authfn);
		pvmlogerror("pvmbeatask() can't creat t-auth file\n");
		cc = PvmSysErr;
		goto bail2;
	}
#endif

	/*
	*	send first connect message to pvmd
	*/

	mxfersingle = 1;
	prver = TDPROTOCOL;
	pvm_pkint(&prver, 1, 1);
	pvm_pkstr(authfn);
	if ((cc = msendrecv(TIDPVMD, TM_CONNECT, SYSCTX_TM)) <= 0)
		goto bail;
	pvm_upkint(&prver, 1, 1);
	if (prver != TDPROTOCOL) {
		pvmlogprintf("pvmbeatask() t-d protocol mismatch (%d/%d)\n",
			TDPROTOCOL, prver);
		cc = PvmSysErr;
		goto bail;
	}
	pvm_upkint(&cc, 1, 1);
	if (!cc) {
		pvmlogerror("pvmbeatask() pvmd refuses connection\n");
		cc = PvmSysErr;
		goto bail;
	}

	/*
	*	check our t-auth file; write in pvmd d-auth file
	*/

#ifndef NOPROT

#ifdef IMA_BEOLIN
	sleep(15);  /* because of race condition over NFS--yuck! (PLS) */
#endif

	if ((cc = read(authfd, (char*)&cc, 1)) == -1) {
		pvmlogperror("pvmbeatask() read authfile");
		cc = PvmSysErr;
		goto bail;
	}
	if (cc != 1) {
		pvmlogerror("pvmbeatask() pvmd didn't validate itself\n");
		cc = PvmSysErr;
		goto bail;
	}
	(void)close(authfd);
	(void)unlink(authfn);

	pvm_upkstr(authfn);
	if ((authfd = open(authfn, O_WRONLY, 0)) == -1) {
		pvmlogperror(authfn);
		pvmlogerror("pvmbeatask() failed to open d-auth file\n");
		authfn[0] = 0;
		cc = PvmSysErr;
		goto bail;
	}
	cc = write(authfd, authfn, 1);
	(void)close(authfd);
	if (cc != 1) {
		if (cc == -1)
			pvmlogperror(authfn);
		pvmlogerror("pvmbeatask() can't write d-auth file\n");
		authfn[0] = 0;
		cc = PvmSysErr;
		goto bail;
	}
	authfd = -1;
	authfn[0] = 0;

#endif

	/*
	*	send second connect message to pvmd
	*/

	pvm_initsend(PvmDataFoo);
	pvm_pkint(&pvmmyupid, 1, 1);
	pvm_pkint(&cookie, 1, 1);
	if ( pvmmytaskname != NULL )
		pvm_pkstr(pvmmytaskname);
	if ((cc = msendrecv(TIDPVMD, TM_CONN2, SYSCTX_TM)) <= 0)
		goto bail;
	pvm_upkint(&cc, 1, 1);
	if (!cc) {
		pvmlogerror("pvmbeatask() pvmd refuses connection\n");
		cc = PvmSysErr;
		goto bail;
	}
#endif /* IMA_MPP */

#ifdef IMA_MPP
	pvm_mpp_beatask(&pvmmytid, &pvmmyptid, &outtid, &outctx, 
				&outtag, &trctid, &trcctx,&trctag, &pvmudpmtu,
				&pvmschedtid, &topvmd);
#else
	pvm_upkint(&pvmmytid, 1, 1);
	pvm_upkint(&pvmmyptid, 1, 1);

	pvm_upkint(&outtid, 1, 1);
	pvm_upkint(&outctx, 1, 1);
	pvm_upkint(&outtag, 1, 1);
#endif
	if (!pvmtrc.outtid) {
		pvmtrc.outtid = outtid;
		pvmtrc.outctx = outctx;
		pvmtrc.outtag = outtag;
		pvmctrc.outtid = pvmtrc.outtid;
		pvmctrc.outctx = pvmtrc.outctx;
		pvmctrc.outtag = pvmtrc.outtag;
	}

#ifndef IMA_MPP
	pvm_upkint(&trctid, 1, 1);
	pvm_upkint(&trcctx, 1, 1);
	pvm_upkint(&trctag, 1, 1);
#endif
	if (!pvmtrc.trctid) {
		pvmtrc.trctid = trctid;
		pvmtrc.trcctx = trcctx;
		pvmtrc.trctag = trctag;
		pvmctrc.trctid = pvmtrc.trctid;
		pvmctrc.trcctx = pvmtrc.trcctx;
		pvmctrc.trctag = pvmtrc.trctag;
		new_tracer++;
	}

	if (p = getenv("PVMCTX"))
		pvmmyctx = pvmstrtoi(p);
/* 
	if (pvmmyctx == 0)
		pvmmyctx = pvm_newcontext();
*/

	/* get trace mask from envar or zero it */

	if ( (p = getenv("PVMTMASK")) ) {
		if ( strlen(p) + 1 == TEV_MASK_LENGTH )
			BCOPY(p, pvmtrc.tmask, TEV_MASK_LENGTH);
		else
			TEV_MASK_INIT(pvmtrc.tmask);
	} else {
		TEV_MASK_INIT(pvmtrc.tmask);
		if ( new_tracer ) need_trcinfo++;
	}

	BCOPY(pvmtrc.tmask, pvmctrc.tmask, TEV_MASK_LENGTH);

	/* get trace buffering from envar */

	if ((p = getenv("PVMTRCBUF")))
		pvmtrc.trcbuf = atoi( p );
	else {
		pvmtrc.trcbuf = 0;
		if ( new_tracer ) need_trcinfo++;
	}

	pvmctrc.trcbuf = pvmtrc.trcbuf;

	/* get trace options from envar */

	if ((p = getenv("PVMTRCOPT")))
		pvmtrc.trcopt = atoi( p );
	else {
		pvmtrc.trcopt = 0;
		if ( new_tracer ) need_trcinfo++;
	}

	pvmctrc.trcopt = pvmtrc.trcopt;

#ifndef IMA_MPP
	pvm_upkint(&pvmudpmtu, 1, 1);
#endif
	pvmfrgsiz = pvmudpmtu;

#ifndef IMA_MPP
	pvm_upkint(&i, 1, 1);	/* XXX data signature */

	pvm_upkstr(buf);

#ifdef IMA_BEOLIN
	hex_inadport(buf, &pvmourinet);	/* fixed to pass proper address */
	if (pvmdebmask & PDMROUTE)
		{
		pvmlogprintf( "mroute(): pvmourinet is %s\n", inadport_hex( &pvmourinet ) );
		}
#if 0
	if (gethostname(namebuf, sizeof(namebuf)-1) == -1) {
		pvmlogerror("pvmbeatask() can't gethostname()\n");
		hex_inadport(buf, &pvmourinet);
	} else {
		/* got name, now get addr */
		if (!(hostaddr = gethostbyname( namebuf ))) {
			pvmlogprintf( "pvmbeatask() can't gethostbyname() for %s\n",
					namebuf );
			hex_inadport(buf, &pvmourinet);
		} else {
			/* got addr, now save it */
			BCOPY( hostaddr->h_addr_list[0],
					(char*)&pvmourinet.sin_addr,
					sizeof(struct in_addr));
		}
	}
#endif
#else
	hex_inadport(buf, &pvmourinet);
#endif	/* IMA_BEOLIN */

	pvmourinet.sin_family = AF_INET;
	pvmourinet.sin_port = 0;

	pvm_upkint(&pvmschedtid, 1, 1);
#endif

	wait_init(pvmmytid, TIDLOCAL);

	BZERO(&minfo, sizeof(minfo));
	minfo.src = -1;
	minfo.ctx = SYSCTX_TC;
	minfo.tag = TC_CONREQ;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_conreq);
	minfo.tag = TC_CONACK;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_conack);
	minfo.tag = TC_TASKEXIT;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_taskexit);
	minfo.tag = TC_NOOP;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_noop);
	minfo.tag = TC_SETTRACE;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_settrace);
	minfo.tag = TC_SETTRCBUF;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_settrcbuf);
	minfo.tag = TC_SETTRCOPT;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_settrcopt);
	minfo.tag = TC_SETTMASK;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_settmask);
	minfo.tag = TC_SIBLINGS;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_siblings);

	pvm_freebuf(pvm_setrbuf(rbf));
	pvm_freebuf(pvm_setsbuf(sbf));
	mxfersingle = 0;

	if ( need_trcinfo )
	{
		rbf = pvm_setrbuf( 0 );

		if ( pvm_recvinfo( PVMTRACERCLASS, 0, PvmMboxDefault ) > 0 )
		{
			pvm_upkint(&trctid, 1, 1);

			pvm_upkint(&trcctx, 1, 1);
			pvm_upkint(&trctag, 1, 1);

			pvm_upkint(&outctx, 1, 1);  /* unused here */
			pvm_upkint(&outtag, 1, 1);  /* unused here */

			pvm_upkstr(tmask);

			pvm_upkint(&tbuf, 1, 1);
			pvm_upkint(&topt, 1, 1);

			if ( pvmtrc.trctid == trctid && pvmtrc.trcctx == trcctx
					&& pvmtrc.trctag == trctag )
			{
				if ( strlen(tmask) + 1 == TEV_MASK_LENGTH ) {
					BCOPY(tmask, pvmtrc.tmask, TEV_MASK_LENGTH);
					BCOPY(pvmtrc.tmask, pvmctrc.tmask, TEV_MASK_LENGTH);
				}

				pvmtrc.trcbuf = tbuf;
				pvmctrc.trcbuf = pvmtrc.trcbuf;

				pvmtrc.trcopt = topt;
				pvmctrc.trcopt = pvmtrc.trcopt;
			}

			pvm_freebuf(pvm_setrbuf(rbf));
		}

		else
			pvm_setrbuf(rbf);
	}

	tev_init();

	if (TEV_AMEXCL) {
		TEV_ENDEXCL;
	}
	return 0;

bail:
	if (pvm_getrbuf() > 0)
		pvm_freebuf(pvm_getrbuf());
	if (pvm_getsbuf() > 0)
		pvm_freebuf(pvm_getsbuf());
	pvm_setrbuf(rbf);
	pvm_setsbuf(sbf);

#ifndef IMA_MPP
	if (authfd != -1)
		(void)close(authfd);
	if (authfn[0])
		(void)unlink(authfn);
	unmksocs();
#endif

bail2:
	if (TEV_AMEXCL) {
		TEV_ENDEXCL;
	}
	return cc;
}


int
pvmendtask()
{
	if (pvmmytid != -1) {
#if !defined(IMA_MPP)
		unmksocs();
#endif
		pvmmytid = -1;
		pvmtrc.trctid = 0;
	}

	return 0;
}


void
check_for_exit(src)
	int src;
{
}


/************************
 **  Libpvm Functions  **
 **                    **
 ************************/


int
pvm_getfds(fds)		/* XXX this function kinda sucks */
	int **fds;			/* fd list return */
{
	static int *fdlist = 0;
	static int fdlen = 0;
	int cc;
	int nfds;
	struct ttpcb *pcbp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETFDS,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (!(cc = BEATASK)) {
		nfds = 1;
		for (pcbp = ttlist->tt_link; pcbp != ttlist; pcbp = pcbp->tt_link)
			if (pcbp->tt_state == TTOPEN || pcbp->tt_state == TTGRNWAIT)
				nfds++;

		if (fdlen < nfds) {
			fdlen = (nfds * 3) / 2 + 1;
			if (fdlist)
				fdlist = TREALLOC(fdlist, fdlen, int);
			else
				fdlist = TALLOC(fdlen, int, "fdlist");
		}

		fdlist[0] = topvmd->tt_fd;
		nfds = 1;
		for (pcbp = ttlist->tt_link; pcbp != ttlist; pcbp = pcbp->tt_link)
			if (pcbp->tt_state == TTOPEN || pcbp->tt_state == TTGRNWAIT)
				fdlist[nfds++] = pcbp->tt_fd;
		*fds = fdlist;
		cc = nfds;
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETFDS,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			if (cc > 0) {
				TEV_PACK_INT( TEV_DID_FDS, TEV_DATA_ARRAY,
					(int *)fdlist, nfds, 1 );
			}
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	return (cc < 0 ? lpvmerr("pvm_getfds", cc) : cc);
}


int
pvm_start_pvmd(argc, argv, block)
	int argc;		/* number of args to pass to pvmd (>= 0) */
	char **argv;	/* args for pvmd or null */
	int block;		/* if true, don't return until add hosts are started */
{
	char *sfn;
	struct stat sb;
	int cc;
	char *fn;			/* file to exec */
	char **av;
	int pfd[2];
	int n;
	FILE *ff;
	char buf[128];

#ifdef WIN32
	char *pathending = "\\lib\\win32\\pvmd3.exe";
	SECURITY_ATTRIBUTES saPipe;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;  /* for CreateProcess call */
	HANDLE pvmdid;
	int i = 0;
	char cmd[256];
#endif

	TEV_DECLS;

	if (TEV_EXCLUSIVE) {
		if (pvmmytid != -1
				&& TEV_DO_TRACE(TEV_START_PVMD,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_BF, TEV_DATA_SCALAR, &block, 1, 1 );
			TEV_PACK_STRING( TEV_DID_AS, TEV_DATA_ARRAY,
				argv, argc, 1 );
			TEV_FIN;
		}
	}

	if (argc < 0 || !argv)
		argc = 0;

#ifndef WIN32
	if ((pvm_useruid = getuid()) == -1) {
		pvmlogerror("can't getuid()\n");
		cc = PvmSysErr;
		goto bail;
	}
	pvmchkuid( pvm_useruid );
#else
	if (!username) 
		username = MyGetUserName();
#endif

	if (!(sfn = pvmdsockfile())) {
		pvmlogerror("pvm_start_pvmd() pvmdsockfile() failed\n");
		cc = PvmSysErr;
		goto bail;
	}

	if (stat(sfn, &sb) != -1) {
		cc = PvmDupHost;
		goto bail;
	}

#ifdef WIN32 

	cc = PvmSysErr;

	fn = malloc(128 * sizeof(char));
	
	strcpy(fn,(char *) pvmgetroot());
	strcat(fn,(char *) pathending);
	
	av = TALLOC(argc + 2, char *, "argv");
	if (argc > 0)
		BCOPY((char *)&argv[0], (char *)&av[1], argc * sizeof(char*));
	av[0] = fn;
	if (stat(av[0],&sb) == -1) {
		fprintf(stderr,"Couldn't find daemon executable !\n", 
			"It should be stored in %PVM_ROOT%\\lib\\win32");
		goto bail;
	}
	av[argc + 1] = 0;

	strcpy(cmd,"");
	strcat(cmd, "pvmd3.exe ");
	for (i=1;i <= argc ;i++) {	
		strcat(cmd,av[i]);
		strcat(cmd," ");
	}
	av[argc + 1] = 0;

	/* pvmdid gives us back the process handle 
	    but it is an int anyway 	*/
	
	saPipe.nLength = sizeof(SECURITY_ATTRIBUTES);
	saPipe.lpSecurityDescriptor = NULL;
	saPipe.bInheritHandle = FALSE;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	pvmdid = (HANDLE) CreateProcess(
			av[0],				/* filename */
			cmd,				/* full command line for child */
								/* for example : hostfiles */
			NULL,				/* process security descriptor */
			NULL,				/* thread security descriptor */
			FALSE,				/* inherit handles? */
			DETACHED_PROCESS,	/* creation flags */
			NULL,				/* inherited environment address */
			NULL,				/* startup dir */
								/* NULL = start in current */
			&si,				/* pointer to startup info (input) */
			&pi);				/* pointer to process info (output) */
		
	if (!pvmdid) {
		fprintf(stderr,"CreateProcess() failed \n");
		fprintf(stderr,"enough memory ?\n");
		goto bail;
		return 1;
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
		   
	PVM_FREE(av);

	while (stat(sfn, &sb) == -1)
		Sleep(5000);		/* what a hack */

#else

#ifdef	IMA_TITN
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, pfd) == -1)
#else
	if (pipe(pfd) == -1)
#endif
	{
		cc = PvmSysErr;
		goto bail;
	}

	fn = pvmgetpvmd();

	av = TALLOC(argc + 2, char *, "argv");
	if (argc > 0)
		BCOPY((char *)&argv[0], (char *)&av[1], argc * sizeof(char*));
	av[0] = fn;
	av[argc + 1] = 0;

	if (!fork()) {
		(void)close(pfd[0]);
	/* fork again so the pvmd is not the child - won't have to wait() for it */
		if (!fork()) {
			if (pfd[1] != 1)
				dup2(pfd[1], 1);
			for (n = getdtablesize(); n-- > 0; )
				if (n != 1)
					(void)close(n);
			(void)open("/dev/null", O_RDONLY, 0);	/* should be 0 */
			(void)open("/dev/null", O_WRONLY, 0);	/* should be 2 */
			(void)signal(SIGINT, SIG_IGN);
			(void)signal(SIGQUIT, SIG_IGN);
#ifndef IMA_OS2
			(void)signal(SIGTSTP, SIG_IGN);
#endif
			execvp(av[0], av);
		}
		_exit(0);
	}
	(void)close(pfd[1]);
	(void)wait(0);
	PVM_FREE(av);

	if (!(ff = fdopen(pfd[0], "r"))) {
		cc = PvmSysErr;
		(void)close(pfd[0]);
		goto bail;
	}

	strcpy(buf, "PVMSOCK=");
	n = strlen(buf);
	if (!fgets(buf + n, sizeof(buf) - n - 1, ff)) {
		cc = PvmCantStart;
		fclose(ff);
		goto bail;
	}
	fclose(ff);
	if (strlen(buf + n) < 2) {
		cc = PvmCantStart;
		goto bail;
	}
	n = strlen(buf);
	if (buf[n - 1] == '\n')
		buf[n - 1] = 0;
	pvmputenv(STRALLOC(buf));

	/* fprintf(stderr, "pvm_start_pvmd: %s\n", buf); */

#endif

	if (cc = BEATASK)
		goto bail;

	if (block) {
		struct pvmhostinfo *hip;
		int t = 1;

		pvm_config((int*)0, (int*)0, &hip);
		while ((cc = pvm_addhosts(&hip[0].hi_name, 1, (int*)0)) == PvmAlready) {
			pvmsleep(t);
			if (t < 8)
				t *= 2;
		}
		if (cc != PvmDupHost)
			goto bail;
		cc = BEATASK;
	}

bail:

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_START_PVMD,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	return (cc < 0 ? lpvmerr("pvm_start_pvmd", cc) : 0);
}

#ifndef PVM_SHMD
int
pvm_precv(tid, tag, cp, len, dt, rtid, rtag, rlen)
	int tid;
	int tag;
	void *cp;
	int len;
	int dt;
	int *rtid;
	int *rtag;
	int *rlen;
{
	int nb, mc, src;
	int rbf;
	int cc = 0;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PRECV,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			ad = (long)cp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &len, 1, 1 );
			TEV_PACK_INT( TEV_DID_PDT, TEV_DATA_SCALAR, &dt, 1, 1 );
			TEV_FIN;
		}
	}

	switch (dt) {

	case PVM_BYTE:
		len *= sizeof(char);
		break;

	case PVM_SHORT:
	case PVM_USHORT:
		len *= sizeof(short);
		break;

	case PVM_INT:
	case PVM_UINT:
		len *= sizeof(int);
		break;

	case PVM_LONG:
	case PVM_ULONG:
		len *= sizeof(long);
		break;

	case PVM_FLOAT:
		len *= sizeof(float);
		break;

	case PVM_CPLX:
		len *= sizeof(float) * 2;
		break;

	case PVM_DOUBLE:
		len *= sizeof(double);
		break;

	case PVM_DCPLX:
		len *= sizeof(double) * 2;
		break;

	case PVM_STR:
		cc = PvmNotImpl;
		break;

	default:
		cc = PvmBadParam;
		break;
	}

	if (!cc) {
#if !defined(IMA_MPP)
		rbf = pvm_setrbuf(0);
		cc = pvm_recv(tid, tag);
		if (cc > 0) {
			pvm_bufinfo(cc, &nb, &mc, &src);
			if (rlen)
				*rlen = nb;
			if (nb < len)
				len = nb;
			if (rtag)
				*rtag = mc;
			if (rtid)
				*rtid = src;
			pvm_upkbyte((char *)cp, len, 1);
			pvm_freebuf(cc);
			cc = 0;
		}
		pvm_setrbuf(rbf);
#else
		cc = pvm_mppprecv(tid, tag, cp ,len, dt, rtid, rtag, rlen);
#endif
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PRECV,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			if ( cc < 0 )
				nb = mc = src = -1;
			TEV_PACK_INT( TEV_DID_MNB, TEV_DATA_SCALAR, &nb, 1, 1 );
			TEV_PACK_INT( TEV_DID_MC, TEV_DATA_SCALAR, &mc, 1, 1 );
			TEV_PACK_INT( TEV_DID_MCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			TEV_PACK_INT( TEV_DID_SRC, TEV_DATA_SCALAR, &src, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_precv", cc);
	return cc;
}
#else /* PVM_SHMD */
/* Do the PVM_SHMD version of precv */
int
pvm_precv(tid, tag, cp, len, dt, rtid, rtag, rlen)
	int tid;
	int tag;
	void *cp;
	int len;
	int dt;
	int *rtid;
	int *rtag;
	int *rlen;
{
	return (shmd_pvm_precv (tid, tag, cp, len, dt, rtid, rtag, rlen));
}
#endif /* PVM_SHMD */

#ifndef PVM_SHMD
int
pvm_psend(tid, tag, cp, len, dt)
	int tid;
	int tag;
	void *cp;
	int len;
	int dt;
{
	int sbf;
	int cc = 0;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PSEND,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_DST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_MC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_MCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			ad = (long)cp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &len, 1, 1 );
			TEV_PACK_INT( TEV_DID_PDT, TEV_DATA_SCALAR, &dt, 1, 1 );
			TEV_FIN;
		}
	}

	switch (dt) {

	case PVM_BYTE:
		len *= sizeof(char);
		break;

	case PVM_SHORT:
	case PVM_USHORT:
		len *= sizeof(short);
		break;

	case PVM_INT:
	case PVM_UINT:
		len *= sizeof(int);
		break;

	case PVM_LONG:
	case PVM_ULONG:
		len *= sizeof(long);
		break;

	case PVM_FLOAT:
		len *= sizeof(float);
		break;

	case PVM_CPLX:
		len *= sizeof(float) * 2;
		break;

	case PVM_DOUBLE:
		len *= sizeof(double);
		break;

	case PVM_DCPLX:
		len *= sizeof(double) * 2;
		break;

	case PVM_STR:
		cc = PvmNotImpl;
		break;

	default:
		cc = PvmBadParam;
		break;
	}

#if !defined(IMA_MPP)
	if (!cc) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataInPlace));
		pvm_pkbyte((char *)cp, len, 1);
		if ((cc = pvm_send(tid, tag)) > 0)
			cc = 0;
		pvm_freebuf(pvm_setsbuf(sbf));
	}
#else
	cc = pvm_mpppsend((char *) cp, len, tid, tag);
#endif

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PSEND,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_psend", cc);
	return cc;
}
#else
/* i.e. PVM_SHMD */
int
pvm_psend(tid, tag, cp, len, dt)
	int tid;
	int tag;
	void *cp;
	int len;
	int dt;
{
	/* Go do the shmd thing instead */
	return (shmd_pvm_psend(tid, tag, cp, len, dt));
}
#endif /* PVM_SHMD */

/* ----- ogm_complete ------ */
int 
ogm_complete( ogmlist )
struct msgid **ogmlist; 
{

struct msgid *mp, *oldmp, *head;

#if defined(IMA_MPP)
 	if (!(mp = *ogmlist) )
		return TRUE;		/* there is nothing to check */
	
	/* go through the list message ids. checking if the message has
		complete. If the list is empty at the end, then return that
		message has finished */ 

	head = mp;
	mp = mp->ms_link;
	while (mp != head)
	{
		if ((*mp->mfunc->msgdone)(0, &(mp->id), mp->info))
		{
			mp = mp->ms_link;
			oldmp = mp->ms_rlink;
			LISTDELETE(oldmp,ms_link, ms_rlink)
			msgid_free(oldmp);
		}
		else
			mp = mp->ms_link;
		
	}

	/* mp now points to the head of the list */
	if ((*mp->mfunc->msgdone)(0, &(mp->id), mp->info))
	{
		if (mp->ms_link == mp)	/* the only one left */
		{
			msgid_free(mp);
			*ogmlist = (struct msgid *) NULL;
			return TRUE;
		}
		else
		{
			mp = mp->ms_link;
			oldmp = mp->ms_rlink;
			LISTDELETE(oldmp, ms_link, ms_rlink);
			msgid_free(oldmp);
			*ogmlist = mp;	/* new head of the message id list */
		}
	}	
	return FALSE;
#else
	return TRUE;	/* for workstations, the write is sychronous */
#endif
}

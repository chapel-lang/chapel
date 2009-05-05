
static char rcsid[] =
	"$Id: pvmd.c,v 1.84 2009/01/22 23:51:41 pvmsrc Exp $";

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
 *	pvmd.c
 *
 *	Mr. pvm daemon.
 *
 * $Log: pvmd.c,v $
 * Revision 1.84  2009/01/22 23:51:41  pvmsrc
 * Updated BEOLIN code from Paul Springer...
 * (Spanker=kohl)
 *
 * Revision 1.83  2006/08/15 15:31:02  pvmsrc
 * Fixed WIN32 forkexec() "path" management...
 * 	- sizeof(path) returns pointer size, not allocated size...
 * 	- ugly hack, not a real fix...  :-Q
 * (Spanker=kohl)
 *
 * Revision 1.82  2005/05/18 17:56:29  pvmsrc
 * Added calls to pvmchkuid()...
 * 	- keep people from running PVM as root...!  :-o
 * (Spanker=kohl)
 *
 * Revision 1.81  2004/09/08 19:35:36  pvmsrc
 * Added #ifdef IMA_AIX56K64 to unsigned int oslen fiasco...
 * (Spanker=kohl)
 *
 * Revision 1.80  2004/02/06 20:05:38  pvmsrc
 * Yanked out all the AMD64 test crap.
 * 	- it was just the time.h header all along...!  ;-/~
 * (Spanker=kohl)
 *
 * Revision 1.79  2004/02/06 16:50:36  pvmsrc
 * Yet more ctime() test crap for AMD64...
 * (Spanker=kohl)
 *
 * Revision 1.78  2004/02/04 23:30:14  pvmsrc
 * Yet more temp AMD64 hacks...
 * (Spanker=kohl)
 *
 * Revision 1.77  2004/02/04 21:46:23  pvmsrc
 * Temp hack for AMD64 seg fault...
 * (Spanker=kohl)
 *
 * Revision 1.76  2004/01/14 18:51:02  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.75  2002/10/14 19:23:12  pvmsrc
 * Oops!  Need to htons() to convert the host byte to network byte order!
 * 	- for PVMNETSOCKPORT cruft...
 * (Spanker=kohl)
 *
 * Revision 1.74  2002/10/07 14:30:32  pvmsrc
 * Added new PVMNETSOCKPORT env var for mksocs().
 * 	- allow starting port address for netsock binding, in the hopes
 * 		that PVM will get a port that is allowed access through a
 * 		firewall...!
 * 	(it's about time!)
 * (Spanker=kohl)
 *
 * Revision 1.73  2002/07/03 16:57:54  pvmsrc
 * Fixes for compilation on Linux Alpha.
 * 	- submitted by "David Mathog" <mathog@mendel.bio.caltech.edu>.
 * (Spanker=kohl)
 *
 * Revision 1.72  2002/04/16 15:06:02  pvmsrc
 * WIN32 Fixes for Multiple Domains.
 * 	- submitted by Jigen Zhou <jigen@icemcfd.com>.
 * (Spanker=kohl)
 *
 * Revision 1.71  2002/02/21 23:17:40  pvmsrc
 * Added new (not to be documented!) PVM_MAX_TASKS env var support.
 * 	- for Mahir Lokvancic <mahir@math.ufl.edu>.
 * 	- forcefully limits the number of tasks that can attach to a
 * 		pvmd, required on Solaris in rare circumstances when hard
 * 		FD_SETSIZE limit is reached, and all hell breaks loose...
 * 	- set new pvm_max_ntasks global depending on env var (0 = no limit)
 * 	- check return for task_new() call, can now produce NULL ptr,
 * 		indicating PvmOutOfRes...
 * (Spanker=kohl)
 *
 * Revision 1.70  2001/12/07 16:06:11  pvmsrc
 * Oops!  Some new Unix fixes & features break Windows (of course!).
 * 	- #ifdef-ed away fcntl() calls for non-blocking sockets / stdin.
 * 	(one for bug fix in lpvm.c, one for check_ext_input() in pvmd.c)
 * (Spanker=kohl)
 *
 * Revision 1.69  2001/11/26 17:45:12  pvmsrc
 * Added #define for MAXPATHLEN for goofy arches...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.68  2001/09/28 12:43:18  pvmsrc
 * D-Oh!  Stupid Solaris doesn't recognize FNDELAY, only O_NDELAY...  :-Q
 * (Spanker=kohl)
 *
 * Revision 1.67  2001/09/27 21:25:11  pvmsrc
 * BEOSCYLD port.
 * 	- submitted by Joe Vitale <vitale@scyld.com>.
 * 	(we renamed it from BEOWULF to BEOSCYLD, but it's his port... :-)
 * (Spanker=kohl)
 *
 * Revision 1.66  2001/09/26 21:23:21  pvmsrc
 * Added Handling for Optional Virtual Machine ID.
 * 	- added new check_ext_input() routine to check for extension
 * 		options on stdin before anything else happens...  currently
 * 		only expects env string a la "PVM_VMID=xxxx", but more options
 * 		could be added on same input line, white-space-separated...
 * 	- extra vmid comes through with SM_STHOST message (after wincmd)
 * 		to hoster.
 * 	- user instructed to type VMID (or other options) to pvmd stdin
 * 		during manual startup.
 * (Spanker=kohl)
 *
 * Revision 1.65  2001/09/25 21:20:19  pvmsrc
 * Minor TMPNAMFUN()/tmpnam() cleanup.
 * 	- moved macro def to pvm3.h, renamed PVMTNPMAN().
 * 	- same for LEN_OF_TMP_NAM -> PVMTMPNAMLEN.
 * 	- mostly a huge waste of time, since *both* tmpnam() & mktemp()
 * 		produce the same "dangerous" warning message in Linux/gcc...
 * 	- damn.
 * (Spanker=kohl)
 *
 * Revision 1.64  2001/04/04 15:13:58  pvmsrc
 * Fixed pesky bug buried in pvmd opq...
 * 	- ack packets were being stuck at the end of the output queue,
 * 		where they could get delayed (not lost) long enough to trigger
 * 		a cascade of packet retries from remote pvmds.
 * 	- solution is to stick ack packets after new (zero-rtv) packets,
 * 		but before and locally queued retry packets (which can block
 * 		processing of the opq at a specified time...).
 * Yanked Bogus chdir() from WIN32 forkexec().
 * 	- patch submitted by Peter J. Puchyr, SpaceTime Software
 * (Spanker=kohl)
 *
 * Revision 1.63  2001/02/07 23:15:51  pvmsrc
 * 2nd Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.62  2001/02/02 14:50:26  pvmsrc
 * Win32 fixes & additions.
 * (Spanker=kohl)
 *
 * Revision 1.61  2000/10/11 20:50:24  pvmsrc
 * Yikes...
 * 	- fixed '//' comments in WIN32 forkexec() implementation to
 * 		'/ * * /'.
 * 	- apparently intereferes with SUN4SOL2 preprocessor?  HOW BOGUS.
 * (Spanker=kohl)
 *
 * Revision 1.60  2000/02/18 15:48:43  pvmsrc
 * Commented out ECONNREFUSED check for sendto() in LINUX.
 * 	- apparently results from local networking problems...
 * 	- leave in code as a sign post for future occurrences.
 * (Spanker=kohl)
 *
 * Revision 1.59  2000/02/17 23:12:16  pvmsrc
 * *** Changes for new BEOLIN port ***
 * 	- MPP-like, similar to SP2, etc.
 * 	- submitted by Paul Springer <pls@smokeymt.jpl.nasa.gov>.
 * 	- format-checked & cleaned up by Jeembo...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.58  2000/02/17 16:42:56  pvmsrc
 * Added hack to ignore errno=ECONNREFUSED from sendto() for Linux.
 * 	- apparently occurs intermittently on some setups...
 * 	- patch submitted by Russell Mora <rd.mora@auckland.ac.nz>.
 * (Spanker=kohl)
 *
 * Revision 1.57  2000/02/16 22:01:25  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * Added #ifndef NO_NETINET_TCP_H around <netinet/tcp.h> for
 * 	archs that can't handle it...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.56  2000/02/11 21:04:03  pvmsrc
 * Added new PVMHOSTFILE env var.
 * 	- submitted by Anders Mundt Due <amd@pd-house.dk>.
 * (Spanker=kohl)
 *
 * Revision 1.55  2000/02/10 20:46:12  pvmsrc
 * Yanked char *pvmtmpspec for WIN32.
 * 	- not necessary, fixed string manip in log file name gen.
 * (Spanker=kohl)
 *
 * Revision 1.54  2000/02/07 22:22:10  pvmsrc
 * Hack to help with select()/fd_sets in WIN32:
 * 	- fd_set is *NOT* a bit field in WIN32, is a circular buffer.
 * 	- must check for !FD_ISSET() before FD_SET() else duplicate
 * 		entries possible, and fd_set buffer overflow/overwrite.
 * 	- similary, FD_ISSET() check before FD_CLR().
 * 	- patch submitted by "Bruce W. Church" <bwc1@cornell.edu>.
 * (Spanker=kohl)
 *
 * Revision 1.53  1999/07/08 19:00:07  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.52  1999/03/05  17:21:18  pvmsrc
 * improved to work with registry/environment on NT/win95-98
 * and devstudio 5/6
 * (Spanker=sscott)
 *
 * Revision 1.51  1999/02/09  23:07:18  pvmsrc
 * Don't re-invoke read_pvmregistry("PVM_ROOT")...
 * 	- use pvmgetroot().
 * In varsub(), if read_pvmregistry() fails for substring,
 * 	try getenv() too just in case...
 * (Spanker=kohl)
 *
 * Revision 1.50  1999/02/09  20:11:00  pvmsrc
 * Typo in log message...
 * (Spanker=kohl)
 *
 * Revision 1.49  1998/11/20  20:06:38  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (spanker=sscott)
 *
 * Revision 1.48  1998/10/02  15:44:06  pvmsrc
 * Single source code merge of Win32 and Unix code.
 * (Spanker=sscott)
 *
 * Revision 1.47  1998/09/23  15:31:59  pvmsrc
 * Damn!  A non-#ifdef WIN32-ed read_pvmregistry() call!  Blarghhh...
 * (Spanker=kohl)
 *
 * Revision 1.46  1998/09/23  15:23:45  pvmsrc
 * 	changes to use WIN32 registry as per Markus.
 * 	ifdef in pvmd.c::colonsep() to include WIN32 and OS2
 * (Spanker=phil)
 *
 * Revision 1.45  1998/08/13  18:32:49  pvmsrc
 * Added special case for AIX4SP2 arch with SOCKLENISUINT.
 * 	- on AIX 4.3 systems, I guess unsigned int is really
 * 		unsigned int, and not size_t...  D-Oh!
 * 	- probably need a better constant name (or two) here...
 * (Spanker=kohl)
 *
 * Revision 1.44  1998/07/24  17:31:25  pvmsrc
 * Fixed use of PVM_TIMET stuff:
 * 	- on dorky systems (like LINUXALPHA) timeval->tv_sec could be an
 * 		int, while time_t is a long, so casting the &tnow.tv_sec ptr
 * 		blows chunks.
 * 	- e.g., check this date:  Fri Oct 11 07:59:03 3386993  :-)
 * 	- use a temp var & cast the value before passing the ptr...
 * 	- thanks to Greg Lindahl (lindahl@cs.virginia.edu) for the patch.
 * (Spanker=kohl)
 *
 * Revision 1.43  1998/07/24  17:11:49  pvmsrc
 * Cleaned up use of SOCKLENISUINT / oslen.
 * 	- use oslen for every socket-related call:
 * 		* bind(), recvfrom(), getsockname() and accept().
 * (Spanker=kohl)
 *
 * Revision 1.42  1998/06/03  18:46:37  pvmsrc
 * Enhanced spawning of default pluggable modules...
 * 	- created static env var name list "modulenames[]".
 * 	- replaced simple command name handling with acav() parsing...
 * 	- check for non-zero myhostpart to avoid re-spawning modules
 * 		in pvmd'...
 * (Spanker=kohl)
 *
 * Revision 1.41  1998/06/02  20:49:00  pvmsrc
 * Added new locl_spawn() routine for pvmd to spawn local system tasks.
 * 	- applied for new env vars interface to pluggable modules:
 * 		* $PVM_TASKER - defines tasker to spawn on pvmd startup.
 * 		* $PVM_HOSTER - defines hoster to spawn on pvmd startup.
 * 		* $PVM_RM - defines resource manager to spawn on pvmd startup.
 * (Spanker=kohl)
 *
 * Revision 1.40  1998/02/23  22:51:46  pvmsrc
 * Added AIX4SP2 stuff.
 * (Spanker=kohl)
 *
 * Revision 1.39  1998/01/28  21:27:35  pvmsrc
 * Leftover LINUX fixes:
 * 	- un-#ifdef-ed sendto() error ENOMEM for LINUX, should be O.K. for
 * 		every arch.
 * 	- also commented out #ifndef LINUX socket addressing stuff in
 * 		netinput(), looks a bit antiquated and seems to work just
 * 		fine without it - here's hoping.
 * (Spanker=kohl)
 *
 * Revision 1.38  1998/01/28  19:31:31  pvmsrc
 * Added new -DCTIMEISTIMET flag.
 * 	- the #if defined() list for PVM_TIMET was getting way
 * 		out of hand...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.37  1998/01/28  19:13:57  pvmsrc
 * Added new IMA_LINUXHPPA to #if cases...
 * (Spanker=kohl)
 *
 * Revision 1.36  1998/01/12  21:13:27  pvmsrc
 * Replaced inline constants with new task output op defines.
 * 	- TO_NEW == -2.
 * 	- TO_SPAWN == -1.
 * 	- TO_EOF == 0.
 * (Spanker=kohl)
 *
 * Revision 1.35  1997/12/01  19:20:51  pvmsrc
 * Replaced #ifdef IMA_OS2 fd_set declarations:
 * 	- new #ifdef FDSETNOTSTRUCT.
 * 	- choose between "fd_set foo" and "struct fd_set foo"...
 * (Spanker=kohl)
 *
 * Revision 1.34  1997/11/04  23:22:25  pvmsrc
 * Cleaned up fd_set stuff (hopefully).
 * Added IMA_LINUXALPHA to PVM_TIMET stuff.
 * (Spanker=kohl)
 *
 * Revision 1.33  1997/08/29  13:35:14  pvmsrc
 * OS2 Port Submitted by Bohumir Horeni, horeni@login.cz.
 * (Spanker=kohl)
 *
 * Revision 1.32  1997/08/26  22:54:02  pvmsrc
 * fixed Win32 slave_config
 * (Spanker=fischer)
 *
 * Revision 1.31  1997/07/30  19:01:21  pvmsrc
 * Markus Fixes.
 *
 * Revision 1.30  1997/07/23  14:37:24  pvmsrc
 * Added support for new UXPV arch.
 *
 * Revision 1.29  1997/07/16  21:10:41  pvmsrc
 * WIN32 Fixes - Markus.
 *
 * Revision 1.28  1997/07/02  20:27:29  pvmsrc
 * 	Fixed startup race on shmem to that a shmem task can get fully
 * 	configured before getting any messages.
 * 	This involved adding two states
 * 	TF_PRESHMCONN and TF_SHM.  TF_PRESHMCONN indicates that messages
 * 	with MM_PRIO set can be sent to a task, but regular messages are
 * 	queued. This allows shmem tasks to be completely configured
 * 	before any messages flow.  When the daemon changes the state from
 * 	TF_PRESHMCONN to TF_SHMCONN it calls shm_wrt_pkts to write any
 * 	packets that were queued before task state changed to TF_SHMCONN.
 *
 * Revision 1.27  1997/06/27  21:17:17  pvmsrc
 * Using sigset() to force daemon under SUNMP to catch SIGCLD.
 *
 * Revision 1.26  1997/06/27  19:26:09  pvmsrc
 * Integrated WIN32 changes.
 *
 * Revision 1.25  1997/06/24  20:37:56  pvmsrc
 * Installed new env var features:
 * 	- PVM_PATH overrides default execution path (but not hostfile ep=).
 * 	- PVM_WD overrides default working directory (but not hostfile wd=).
 *
 * Revision 1.24  1997/06/16  13:41:11  pvmsrc
 * forkexec() passes extra info to taskers.
 *
 * Revision 1.23  1997/06/12  20:14:57  pvmsrc
 * Added #define for MAXPATHLEN.
 * 	- fix submitted by Brian Forney (bforney@cray.com)
 * 		of Cray Research, Inc.
 *
 * Revision 1.22  1997/06/02  13:21:22  pvmsrc
 * Added TM_SHMCONN test in pkt_to_task so that remote tasks write to local
 * tasks via their shared memory instead of the t-d socket.
 *
 * Revision 1.21  1997/05/29  15:14:34  pvmsrc
 * Added IMA_SGIMP64 to the infamous PVM_TIMET #define list.  :-)
 *
 * Revision 1.20  1997/05/27  14:49:25  pvmsrc
 * Added test for shmem and TF_SHMCONN to mesg_to_task():
 * 	so that mpp_output is used insted of locloutput when a t-d
 * 	socket exists when tasks are connected via shared memory
 * 	correctly...
 *
 * Revision 1.19  1997/05/13  13:27:01  pvmsrc
 * Fixed up SGI64 crapola - PVM_TIMET -> time_t.
 *
 * Revision 1.18  1997/05/08  21:06:30  pvmsrc
 * 	defined PVM_TIMET to time_t for LINUX and LINUXSPARC
 * 	and PVM_TIMET to long for all others  so
 * 	that calls to ctime don't generate warnings
 *
 * Revision 1.17  1997/05/07  21:19:19  pvmsrc
 * swapped logic around SOCKLENISINT so that it is now
 * the default path and SOCKLENISUINT is the compile-time
 * selected path.
 *
 * Now, AIX 4.1 will have to remove SOCKLENISUINT from
 * their AIXxxx.def configure files.
 *
 * Revision 1.16  1997/05/07  18:37:10  pvmsrc
 * AIX 3.2 vs 4.1 vs 4.2 problems resolved (I hope)
 * Define oslen as size_t with compile flag SOCKLENISINT
 * for AIX 4.1 to explicitly set to int.
 * To use flag - set in conf/AIX46K.def.
 * Tested with cc and gcc on all 3 OS versions.
 *
 * Revision 1.15  1997/05/05  18:51:16  pvmsrc
 *  	Removed Ifdefs so that select timeout was set properly for PGONs.
 *
 * Revision 1.14  1997/05/02  13:50:20  pvmsrc
 * 	Support for SP2MPI. call mpp_init() mpp_output()
 *
 * Revision 1.13  1997/04/30  21:26:24  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.12  1997/04/21  14:58:25  pvmsrc
 * Changed #ifdefs that checked IMA_RS6K,IMA_SP2MPI & IMA_AIX46K
 * 	to see if select.h was needed into single define NEEDSSELECTH.
 * 	New archs need to set this in conf/
 *
 * Revision 1.11  1997/04/03  16:13:55  pvmsrc
 * 1) Fixed sequence number rollover bug (apparent only when nopax > 1)
 * 2) Fixed queueing order of output pkts (opq). (apparent when nopax > 1)
 * 	Packets could  be queued in reverse order causing the remote daemon
 * 	to reorder packets even when there were no transmission errors.
 *
 * Revision 1.10  1997/03/07  14:01:06  pvmsrc
 * 	Support for Mpps.
 *
 * Revision 1.9  1997/02/13  15:10:02  pvmsrc
 * Removed unnecessary extern for struct waitc *waitlist.
 * 	- now in global.h.
 *
 * Revision 1.8  1997/01/28  19:27:02  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.7  1996/11/26  19:18:33  pvmsrc
 * Added code to prevent multiple pvmds for a single user on one machine.
 * Define OVERLOADHOST if want to enable multiple pvmds.
 *
 * Revision 1.6  1996/10/25  13:57:59  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.5  1996/10/24  21:31:36  pvmsrc
 * Moved #include "global.h" to below other #includes for typing.
 * Added #include <pvmtev.h> (to get TEV_* constants).
 * Added decl for storing registered tracer info:
 * 	- struct Pvmtracer pvmtracer.
 * 	- init to { 0, 0, 0, 0, 0, 0, 0, 0, "" }.
 * Changed comparisons on tp->t_outtid:
 * 	- check for > 0, not just non-zero.
 * 	- to handle new case where task sets to -1 to deny external collect.
 *
 * Revision 1.4  1996/10/14  19:17:03  pvmsrc
 * Used ARCHFLAG SOCKLENISUINT where socket length is unsigned int
 *
 * Revision 1.3  1996/10/09  21:48:16  pvmsrc
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
 * Revision 1.2  1996/09/23  23:27:26  pvmsrc
 * Initial Creation - original pvmd.c.
 *
 * Revision 1.40  1995/11/02  16:29:24  manchek
 * added -t flag for test mode.
 * put back save under packet header in netoutput.
 * refragment in pkt_to_host now handles message header correctly
 *
 * Revision 1.39  1995/09/05  19:22:07  manchek
 * forgot ifdef for SP2MPI
 *
 * Revision 1.38  1995/07/28  20:52:01  manchek
 * missed changing src to pk_src in loclinpkt
 *
 * Revision 1.37  1995/07/28  16:40:59  manchek
 * wrap HASERRORVARS around errno declarations
 *
 * Revision 1.36  1995/07/24  19:52:02  manchek
 * message header no longer part of packet data, goes in pkt struct.
 * socket drivers in {locl,net}{in,out}put must strip and reconstitute headers.
 * no longer need to replicate first fragment of message to send,
 * or to save-under.
 * cleaned up line between loclinput and loclinpkt.
 *
 * Revision 1.35  1995/07/19  21:26:57  manchek
 * use new function pvmnametag instead of [dts]mname
 *
 * Revision 1.34  1995/07/18  17:02:03  manchek
 * added code to generate and check crc on each message (MCHECKSUM)
 *
 * Revision 1.33  1995/07/11  18:56:00  manchek
 * main prints PVMSOCK instead of master_config (after mpp_init)
 *
 * Revision 1.32  1995/07/05  16:20:39  manchek
 * work calls mpp_dredge for zombies if task with zero tid closes socket
 * (possibly a shared memory task exiting)
 *
 * Revision 1.31  1995/07/03  19:16:24  manchek
 * removed POWER4 ifdefs and misc. schmutz
 *
 * Revision 1.30  1995/06/28  15:27:33  manchek
 * pvmbailout doesn't set global bailing_out
 *
 * Revision 1.29  1995/06/16  16:28:35  manchek
 * (CSPP) CINDEX macro defined both in pvmd.c and system include file.
 * can undef CINDEX before we define it for us
 *
 * Revision 1.28  1995/06/02  17:51:44  manchek
 * added code to balance spawn (forks) on CSPP
 *
 * Revision 1.27  1995/05/30  17:46:59  manchek
 * Added ifdefs for SP2MPI arch
 *
 * Revision 1.26  1995/05/17  16:31:57  manchek
 * changed global mytid to pvmmytid.
 * pvmbailout sets global bailing_out (used by shared memory code).
 * use PVMDDEBUG envar to set pvmdebmask at startup.
 * added new debug classes.
 * use FDSETISINT.
 * on LINUX systems, check sendto for ENOMEM.
 *
 * Revision 1.25  1995/02/06  22:40:11  manchek
 * shared memory ports call mpp_setmtu before slave_config
 *
 * Revision 1.24  1995/02/06  18:52:24  manchek
 * added debugging prints for when main select in work fails (solaris)
 *
 * Revision 1.23  1995/02/06  05:01:28  manchek
 * hmm
 *
 * Revision 1.22  1995/02/03  16:45:27  manchek
 * touch up reap - define rus as int if we don't think struct rusage exists
 *
 * Revision 1.21  1995/02/01  21:31:23  manchek
 * added clear_opq_of, called when host is deleted from table or pvmd' exits
 *
 * Revision 1.20  1994/12/20  16:40:35  manchek
 * use O_NONBLOCK for RS6K
 *
 * Revision 1.19  1994/11/08  19:05:07  manchek
 * mpp fix?
 *
 * Revision 1.18  1994/11/08  15:30:51  manchek
 * shared memory cleanup
 *
 * Revision 1.17  1994/10/15  19:27:02  manchek
 * make wrk_fds_init(), use instead of FD_ZERO.
 * don't send FIN|ACK to ourself in bailout.
 * don't clean up task until SIGCHLD if TF_FORKD set.
 * check newhosts when deleting host.
 * cast message tags for comparison as integers.
 * in beprime() call task_init instead of trying to clean up
 *
 * Revision 1.16  1994/09/02  15:48:17  manchek
 * added UXPM ifdef to parallel SUN4SOL2
 *
 * Revision 1.15  1994/09/02  15:27:55  manchek
 * forgot to inc refcount of nth fragment in sendmessage
 *
 * Revision 1.14  1994/07/18  19:21:51  manchek
 * added PDMWAITC.
 * fix to call write() with max 4096 length for RS6K
 *
 * Revision 1.13  1994/06/30  21:36:56  manchek
 * don't check remote sockaddr in netinput() on LINUX
 *
 * Revision 1.12  1994/06/04  21:45:10  manchek
 * added unix domain sockets
 *
 * Revision 1.11  1994/06/03  20:38:22  manchek
 * version 3.3.0
 *
 * Revision 1.10  1993/12/20  15:39:28  manchek
 * patch 6 from wcj
 *
 * Revision 1.9  1993/10/25  20:51:11  manchek
 * make sure pvmd doesn't use 0..2 for sockets, etc. - open /dev/null.
 * added code to change process group/disassoc. from tty (TTYDIS).
 * ping other pvmds also when run state is PVMDHTUPD
 *
 * Revision 1.8  1993/10/12  14:18:37  manchek
 * fixed bug in locloutput() - hung if write() returned 0
 *
 * Revision 1.7  1993/10/04  20:27:42  manchek
 * renamed useruid to pvm_useruid for compat with libpvm
 *
 * Revision 1.6  1993/10/04  19:17:45  manchek
 * on Solaris, sendto() can return ECHILD.  Hahahahahaha!!!
 *
 * Revision 1.5  1993/10/04  19:12:25  manchek
 * hd_txseq wasn't wrapped properly with NEXTSEQNUM
 *
 * Revision 1.4  1993/09/23  20:36:19  manchek
 * fixed broken mca lookup
 *
 * Revision 1.3  1993/09/22  19:14:47  manchek
 * added network resend statistic.
 * removed redundant code in netinpkt() where it finds mca
 *
 * Revision 1.2  1993/09/16  21:45:32  manchek
 * replaced reap() - now uses SYSVSIGNAL and NOWAIT3 macros
 *
 * Revision 1.1  1993/08/30  23:26:50  manchek
 * Initial revision
 *
 */

#include <stdio.h>

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
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#ifndef NO_NETINET_TCP_H
#include <netinet/tcp.h>
#endif
#include <netdb.h>
#include <pwd.h>
#endif

#ifdef IMA_BEOSCYLD
#include <sys/bproc.h>
#endif

#ifndef IMA_TITN
#ifndef WIN32
#include <sys/ioctl.h>
#endif
#endif

#ifndef	NOWAIT3
#include <sys/resource.h>
#endif
#ifdef NEEDSSELECTH
#include <sys/select.h>
#endif
#include <sys/stat.h>
#ifndef NOUNIXDOM
#include <sys/un.h>
#endif
#ifdef IMA_LINUXALPHA
#include <unistd.h>
#endif
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
/* Must come before local CINDEX macro definition */
#if defined(IMA_CSPP) && defined(BALANCED_SPAWN)
#include <sys/cnx_types.h>
#include <sys/cnx_sysinfo.h>
#include <sys/cnx_pattr.h>
#undef CINDEX
#endif
#ifdef	SYSVSTR
#include <string.h>
#define	CINDEX(s,c)	strchr(s,c)
#else
#include <strings.h>
#define	CINDEX(s,c)	index(s,c)
#endif

#include <pvmproto.h>
#include "pvmalloc.h"
#include "host.h"
#include "pvmdabuf.h"
#include "pvmfrag.h"
#include "pmsg.h"
#include "pkt.h"
#include "task.h"
#include "waitc.h"
#include "listmac.h"
#include "tvdefs.h"
#if defined(IMA_PGON) || defined(IMA_I860) || defined(IMA_CM5) \
	|| defined(SHMEM) || defined(IMA_SP2MPI) \
	|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
	|| defined(IMA_BEOLIN)
#include "pvmdmp.h"
#endif
#include "bfunc.h"
#include <pvmtev.h>
#include "global.h"

#ifdef IMA_CRAY
#define MAXPATHLEN PATH_MAX+1
#endif

#if defined(IMA_CRAY) || defined(IMA_CRAY2) || defined(IMA_I860) || defined(IMA_E88K) || defined(IMA_SCO)
#ifndef	MAXPATHLEN
#define	MAXPATHLEN	CANBSIZ
#endif
#endif

#ifndef	max
#define	max(a,b)	((a)>(b)?(a):(b))
#endif

#ifndef	min
#define	min(a,b)	((a)<(b)?(a):(b))
#endif

#ifndef	TTYDIS
#define	TTYDIS	0
#endif

#ifndef	SOMAXCONN
#define	SOMAXCONN	5
#endif

#ifdef	STATISTICS
struct statistics {
	int selneg, selzer, selrdy;		/* neg, zero, ready selects */
	int rdneg, rdzer, rdok;			/* neg, zero, positive reads */
	int wrneg, wrzer, wrshr, wrok;	/* neg, zero, short, ok writes */
	int sdneg, sdok;				/* neg, ok sendtos */
	int rfok;						/* ok recvfroms */
	int refrag;						/* refragmented frags */
	int netret;						/* network resends */
};

struct statistics stats;
#endif

struct deaddata {
	int dd_pid;				/* process id */
	int dd_es;				/* unix exit status */
	struct timeval dd_ut;	/* user time used */
	struct timeval dd_st;	/* system time used */
};

void catch();
char *debug_flags();
char *pvmnametag();
void biteme();
void evilsig();
char *getenv();
char *inadport_decimal();
char *inadport_hex();
void pvmbailout();
char *pvmdsockfile();
char *pvmgethome();
char *pvmgetroot();
void reap();
void i_dump();
void hex_inadport __ProtoGlarp__ (( char *, struct sockaddr_in * ));
void mesg_rewind __ProtoGlarp__ (( struct pmsg * ));
void locl_spawn();
/* void make_valid(char *n); sls */
void make_valid();

#ifndef WIN32
void check_ext_input();
#endif

#ifdef CTIMEISTIMET
#define PVM_TIMET time_t
#else
#define PVM_TIMET long
#endif

#if defined(IMA_PGON) && !defined(IMA_MPP)
#define IMA_MPP
#endif

#ifdef WIN32
char *read_pvmregistry __ProtoGlarp__(( const char * ));
#endif

/***************
 **  Globals  **
 **           **
 ***************/

#ifndef HASERRORVARS
extern int errno;
#endif

extern struct task *locltasks;	/* from task.c */
extern int log_fd;				/* from logging.c */

char **epaths = 0;				/* exec search path */
char *debugger = 0;				/* debugger executable */
int pvmdebmask = 0;				/* which debugging info */
struct htab *filehosts = 0;		/* advisory host table from hostfile */
struct htab *hosts = 0;			/* active host table */
int hostertid = 0;				/* slave pvmd starter task */
char *loclsnam = 0;				/* t-d socket or addr file path */
int loclsock = -1;				/* pvmd-task master tcp socket */
#ifndef NOUNIXDOM
char *loclspath = 0;			/* t-d socket path */
#endif
char *myarchname = ARCHCLASS;
int myhostpart = 0;				/* host number shifted to tid host field */
int pvmfrgsiz = UDPMAXLEN;		/* message frag length (to pack) */
int pvmmydsig = 0;				/* host native data enc */
int pvmmytid = 0;				/* this pvmd tid */
int pvmmyupid = -1;				/* pvmd pid */
int netsock = -1;				/* host-host udp socket */
int nopax = 1;					/* number of outstanding pkts on d-d link */
struct htab *newhosts = 0;		/* hosts being added by pvmd' */
struct htab *oldhosts = 0;		/* real host table (for pvmd') */
int pvmudpmtu = UDPMAXLEN;		/* local UDP MTU */
int ppnetsock = -1;				/* pvmd' host-host udp socket */
int pprime = 0;					/* pvmd' pid for when we're forked */
int runstate = 0;				/* pvmd run state */
int pvmschedtid = 0;			/* scheduler task id */
int taskertid = 0;				/* task starter task */
struct Pvmtracer pvmtracer =	/* task tracer info struct */
	{ 0, 0, 0, 0, 0, 0, 0, 0, "" };
int tidhmask = TIDHOST;			/* mask for host field of tids */
int tidlmask = TIDLOCAL;		/* mask for local field of tids */
int pvm_useruid = -1;			/* our uid */
char *username = 0;				/* our loginname */

#ifdef WIN32
/* the userid is not available in WIN 32! */
/* some useless binary sid struct */
int system_loser_win=FALSE;
int nAlert=SO_SYNCHRONOUS_NONALERT;
int nFileHandle;
WSADATA WSAData;
LPOSVERSIONINFO osinfo=0;
BOOL os_is_NT=FALSE;
#endif

/***************
 **  Private  **
 **           **
 ***************/

static int totprobes = 0;
static int altprobe = 100;	/* ratio of mpp_input() to select for pgons */

static struct deaddata *deads = 0;	/* circ queue of dead task data */
static int ndead = 0;			/* len of deads */
static struct pkt *opq = 0;		/* outstanding pkt q to all hosts */
static int rdead = 0;			/* read ptr for deads */
static int slavemode = 0;		/* started by master pvmd */
static struct pmsg *addmesg = 0;	/* message to self to add slaves */
static int wdead = 0;			/* write ptr for deads */
#ifdef FDSETNOTSTRUCT
static fd_set wrk_rfds;			/* fd_sets for select() in work() */
static fd_set wrk_wfds;
/* static fd_set wrk_efds; */
#else
static struct fd_set wrk_rfds;	/* fd_sets for select() in work() */
static struct fd_set wrk_wfds;
/* static struct fd_set wrk_efds; */
#endif
static int wrk_nfds = 0;		/* 1 + highest bit set in fds */

int pvm_max_ntasks = 0;			/* bound on total number of tasks */
								/* allowed to attach to pvmd... */
									/* == 0 for no bound... */

static char *modulenames[] = {
	"PVM_TASKER",
	"PVM_TRACER",
	"PVM_HOSTER",
	"PVM_RM",
	0
};

main(argc, argv)
	int argc;
	char **argv;
{
	int i, j;
	char *name = "";
	struct passwd *pe;
	int testmode = 0;
	struct timeval tnow;
	char buf[128];

#ifndef WIN32
	/* check for extension config options */
	check_ext_input();

	/* make sure 0, 1, 2 are in use */
	(void)open("/dev/null", O_RDONLY, 0);
	(void)open("/dev/null", O_RDONLY, 0);
	(void)open("/dev/null", O_RDONLY, 0);
#else
	/* WSAStartup has to be called before any socket command */
	/* can be executed. Why ? Ask Bill   */

 	if (WSAStartup(0x0101, &WSAData) != 0) {
	      printf("\nWSAStartup() failed\n");
	      ExitProcess(1);
 	}
	setsockopt(INVALID_SOCKET,SOL_SOCKET,SO_OPENTYPE,
			(char *)&nAlert,sizeof(int));
	osinfo=malloc(sizeof(OSVERSIONINFO));
	osinfo->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(osinfo);
	os_is_NT= ( osinfo->dwPlatformId == VER_PLATFORM_WIN32_NT);
#endif

	{
		char *p;
		if (p = getenv("PVMDDEBUG"))
			pvmdebmask = pvmstrtoi(p);
	}

#ifndef WIN32
	if ((pvm_useruid = getuid()) == -1) {
		pvmlogerror("main() can't getuid()\n");
		pvmbailout(0);
	}
	pvmchkuid( pvm_useruid );
#else
	username = MyGetUserName();
#endif

	pvmsetlog(3);

#ifndef WIN32
	if (pe = getpwuid(pvm_useruid))
		username = STRALLOC(pe->pw_name);
	else
		pvmlogerror("main() can't getpwuid\n");
	endpwent();
#endif

#ifdef WIN32
	if ((pvmmyupid = getpid()) == -1) {
		pvmlogerror("main() can't getpid() %d \n",GetLastError());
		pvmbailout(0);
	}
#else
	if ((pvmmyupid = getpid()) == -1) {
		pvmlogerror("main() can't getpid()\n");
		pvmbailout(0);
	}
#endif

	(void)pvmgetroot();		/* fail here if we can't */

	sprintf(buf, "PVM_ARCH=%s", myarchname);
	pvmputenv(STRALLOC(buf));

	pvmmydsig = pvmgetdsig();

	ppi_config(argc, argv);

#if defined(IMA_PGON) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
	mpp_init(&argc, argv);
#endif

	for (i = j = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {

			case 'd':
				pvmdebmask = pvmstrtoi(argv[i] + 2);
				break;

			case 'n':
				name = argv[i] + 2;
				break;

			case 'S':
				argv[j++] = argv[i];
			case 's':
				slavemode = 1;
				break;

			case 't':
				testmode = 1;
				break;

#ifdef WIN32
			case 'u':
				if (os_is_NT==FALSE) {
					/* someone is on win95 ... */
					argv[i]++;argv[i]++;
					strcpy(username,argv[i]);
				}
				else
					if (strcmp(strcat("-u",username),argv[i])) {
						fprintf(stderr,
								"Provided Username(%s) does not match ",
								username);
						fprintf(stderr,"with OS account(%s).\n",
								argv[i]);
						fprintf(stderr,"Wrong rshd ?!\n");
						exit(1);
				}
				break;
#endif

			default:
				argv[j++] = argv[i];
			}

		} else {
			argv[j++] = argv[i];
		}
	}
	argc = j;

	if (pvmdebmask) {
		pvmlogprintf("version %s\n", PVM_VER);
		pvmlogprintf("ddpro %d tdpro %d\n", DDPROTOCOL, TDPROTOCOL);
		pvmlogprintf("main() debug mask is 0x%x (%s)\n",
				pvmdebmask, debug_flags(pvmdebmask));
	}

	if (!*name) {
		if (gethostname(buf, sizeof(buf)-1) == -1) {
			pvmlogerror("main() can't gethostname()\n");
			pvmbailout(0);
		}
		name = buf;
	}
	if (testmode) {
		PVM_TIMET time_temp;
		gettimeofday(&tnow, (struct timezone*)0);
		pvmlogprintf("version %s ddpro %d tdpro %d sdpro %d\n",
				PVM_VER, DDPROTOCOL, TDPROTOCOL, SDPROTOCOL);
		time_temp = (PVM_TIMET) tnow.tv_sec;
		pvmlogprintf(ctime(&time_temp));
		for (i = 0; i < argc; i++)
			pvmlogprintf("argv[%d]=\"%s\"\n", i, argv[i]);
		exit(0);
	}

	{
		char *p;
		if (p = getenv("PVM_MAX_TASKS")) {
			pvm_max_ntasks = atoi(p);
			if (pvm_max_ntasks < 0)
				pvm_max_ntasks = 0;
		}
	}

	if (slavemode)					/* slave pvmd */
		slave_config(name, argc, argv);

	else							/* master pvmd */
		master_config(name, argc, argv);

#if TTYDIS & 8
	setsid();
#endif
#if TTYDIS & 4
	setpgid(0, 0);
#endif
#if TTYDIS & 2
	setpgrp(0, 0);
#endif
#if TTYDIS & 1
	if ((i = open("/dev/tty", O_RDWR, 0)) != -1) {
		(void)ioctl(i, TIOCNOTTY, 0);
		(void)close(i);
	}
#endif

	myhostpart = hosts->ht_local << (ffs(tidhmask) - 1);
	pvmmytid = myhostpart | TIDPVMD;

	ndead = 1000;	/* XXX hum, static limit makes this easy to do */

	/* deads = TALLOC(ndead, int, "pids"); */

	deads = TALLOC(ndead, struct deaddata, "dead");
	BZERO((char*)deads, ndead * sizeof(struct deaddata));

#ifndef WIN32
	/* no signaling in win32 because no parent child relation ... :-( */

#ifndef IMA_I860 /* this signal interferes with getcube() on I860 */
#ifdef	SYSVSIGNAL
#ifndef IMA_BEOSCYLD
	(void)signal(SIGCLD, reap);
#endif
#ifdef IMA_SUNMP
	sigset(SIGCLD, reap); /* yep we go really want to catch our kids */
#endif
#else
	(void)signal(SIGCHLD, reap);
#endif	/* SYSVSIGNAL */
#endif /*IMA_I860*/

#endif

	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
#ifdef WIN32
		(void)signal(SIGINT, (void*) catch);
#else
		(void)signal(SIGINT, catch);
#endif

	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
#ifdef WIN32
	(void)signal(SIGTERM, (void *) catch);
#else
	(void)signal(SIGTERM, catch);
#endif

#ifndef WIN32
	(void)signal(SIGHUP, SIG_IGN);
	(void)signal(SIGPIPE, SIG_IGN);
#endif

#ifndef WIN32
	(void)signal(SIGFPE, evilsig);
	(void)signal(SIGILL, evilsig);
#else
	(void)signal(SIGFPE, (void *)evilsig);
	(void)signal(SIGILL, (void *)evilsig);
#endif

#ifdef	SIGBUS
	(void)signal(SIGBUS, evilsig);
#endif

#ifndef WIN32
	(void)signal(SIGSEGV, evilsig);
#else
	(void)signal(SIGSEGV, (void *)evilsig);
#endif

#ifdef	SIGSYS
	(void)signal(SIGSYS, evilsig);
#endif

#ifdef	SIGDANGER
	(void)signal(SIGDANGER, biteme);
#endif

#ifdef	STATISTICS
	reset_statistics();
#endif

	task_init();
	wait_init(myhostpart, TIDLOCAL);
	mb_init();
	ppi_init();

	opq = pk_new(0);
	opq->pk_tlink = opq->pk_trlink = opq;

	/* print local socket address on stdout in case someone cares */

	if (!slavemode) {
		printf("%s\n", getenv("PVMSOCK"));
		fflush(stdout);
	}

/* XXX hack to start slaves automatically */

	if (!slavemode && filehosts) {
		struct hostd *hp;
		int hh;
		int n = 0;

		for (hh = filehosts->ht_last; hh >= 1; hh--)
			if ((hp = filehosts->ht_hosts[hh]) && !(hp->hd_flag & HF_NOSTART))
				n++;
		if (n) {
			addmesg = mesg_new(0);
			addmesg->m_tag = DM_ADD;
			pkint(addmesg, n);
			for (hh = 1; hh <= filehosts->ht_last; hh++)
				if ((hp = filehosts->ht_hosts[hh]) && !(hp->hd_flag & HF_NOSTART))
					pkstr(addmesg, hp->hd_name);
			addmesg->m_dst = TIDPVMD;
		}
	}

	work();
	pvmbailout(0);		/* not reached */
	exit(0);
}


#ifndef WIN32
void
check_ext_input()
{
	char input[256];

	char *ptr, *ptr2;

	char c;

	int flags;
	int i;

	/* Set stdin Non-Blocking */

	flags = fcntl( 0, F_GETFL, 0 );

	fcntl( 0, F_SETFL, flags | O_NDELAY );

	/* Test for Input */

	i = 0;

	while ( (int)(input[i] = getc( stdin )) != (char) EOF
			&& input[i] != '\n' && i < 255 )
		i++;

	input[i] = '\0';

	/* Parse Extension Options Here */

	if ( i ) {
		ptr = input;
		while ( *ptr != '\0' )
		{
			/* Virtual Machine ID */
			if ( !strncmp( "PVM_VMID=", ptr, 9 ) ) {
				ptr2 = ptr + 9;
				while ( *ptr2 != ' ' && *ptr2 != '\t' && *ptr2 != '\0' )
					ptr2++;
				c = *ptr2;
				*ptr2 = '\0';
				pvmputenv(STRALLOC(ptr));
				*ptr2 = c;
				ptr = ptr2;
			}

			/* Move Past Unknown Options */
			else
				while ( *ptr != ' ' && *ptr != '\t' && *ptr != '\0' )
					ptr++;

			/* Advance to Next Option */
			while ( *ptr == ' ' || *ptr == '\t' )
				ptr++;
		}
	}

	/* Reset stdin to Blocking */

	fcntl( 0, F_SETFL, flags );
}
#endif


static char *ffnames[] = {
	"SOM", "EOM", "DAT", "FIN", "ACK"
};

char *
pkt_flags(ff)
	int ff;
{
	static char buf[64];
	int bit, i;

	buf[0] = 0;
	for (bit = 1, i = 0; i < sizeof(ffnames)/sizeof(ffnames[0]); i++, bit *= 2)
		if (ff & bit) {
			if (buf[0])
				strcat(buf, ",");
			strcat(buf, ffnames[i]);
		}
	if (!buf[0])
		strcpy(buf, "0");
	return buf;
}


void
evilsig(sig)
	int sig;
{
	if (runstate == PVMDISTASK)
		exit(sig);
	(void)signal(SIGILL, SIG_DFL);
	(void)signal(SIGFPE, SIG_DFL);
#ifdef	SIGBUS
	(void)signal(SIGBUS, SIG_DFL);
#endif
	(void)signal(SIGSEGV, SIG_DFL);
#ifdef	SIGSYS
	(void)signal(SIGSYS, SIG_DFL);
#endif
	(void)signal(SIGINT, SIG_DFL);
	(void)signal(SIGTERM, SIG_DFL);
#ifndef WIN32
	(void)signal(SIGHUP, SIG_DFL);
	(void)signal(SIGPIPE, SIG_DFL);
#endif
#ifdef	SYSVSIGNAL
	(void)signal(SIGCLD, SIG_DFL);
#else
#ifndef WIN32
	(void)signal(SIGCHLD, SIG_DFL);
#endif
#endif
	pvmlogprintf("evilsig() caught signal %d\n", sig);
	i_dump(1);
/*
	abort();
*/
	pvmbailout(-sig);
}


void
catch(sig)
	int sig;
{
	if (runstate == PVMDISTASK)
		exit(sig);
	(void)signal(SIGINT, SIG_DFL);
	(void)signal(SIGTERM, SIG_DFL);
	pvmlogprintf("catch() caught signal %d\n", sig);
	pvmbailout(sig);
}


#ifdef	SIGDANGER
void
biteme(sig)
	int sig;
{
	pvmlogprintf("biteme() caught signal %d and spaced it.\n", sig);
	pvmlogerror("the mad fools, when will they learn?\n");
#ifdef	SYSVSIGNAL
	(void)signal(SIGDANGER, biteme);
#endif
}
#endif	/*SIGDANGER*/


/*	reap()
*
*	Child process has exited.  Put its pid in the fifo of tasks
*	to be cleaned up (in the work loop).
*/

void
reap(sig)
	int sig;
{
	int pid;
	int es = 0;

#ifndef WIN32

#ifndef NOWAIT3
#if defined(RUSAGE_SELF)
	struct rusage rus;
#else
	int rus;
#endif
#endif

	sig = sig;

#ifdef	NOWAIT3
#ifdef	NOWAITPID
	if ((pid = wait(&es)) > 0)
#else
#ifdef IMA_BEOSCYLD
	if ((pid = waitpid(-1, &es, WNOHANG)) > 0)
#else
	while ((pid = waitpid(-1, &es, WNOHANG)) > 0)
#endif
#endif
#else	/*NOWAIT3*/
	while ((pid = wait3(&es, WNOHANG, &rus)) > 0)
#endif	/*NOWAIT3*/

	{
#if !defined(NOWAIT3) && defined(RUSAGE_SELF)
		deads[wdead].dd_ut = rus.ru_utime;
		deads[wdead].dd_st = rus.ru_stime;
#else
		deads[wdead].dd_ut.tv_sec = 0;
		deads[wdead].dd_ut.tv_usec = 0;
		deads[wdead].dd_st.tv_sec = 0;
		deads[wdead].dd_st.tv_usec = 0;
#endif
		deads[wdead].dd_pid = pid;
		deads[wdead].dd_es = es;
		if (++wdead >= ndead)
			wdead = 0;
	}

#ifdef	SYSVSIGNAL
#ifndef IMA_BEOSCYLD
	(void)signal(SIGCLD, reap);
#endif
#endif

#endif

}


/*	pvmbailout()
*
*	We're hosed.  Clean up as much as possible and exit.
*/

void
pvmbailout(n)
	int n;
{
	struct task *tp;

	pvmlogprintf("pvmbailout(%d)\n", n);

	/* sockaddr file */

	if (loclsnam)
		(void)unlink(loclsnam);

	/* kill local tasks */

#ifdef SHMEM
	mpp_cleanup();
#endif

	if (locltasks)
		for (tp = locltasks->t_link; tp != locltasks; tp = tp->t_link) {
			if (tp->t_pid)
#ifndef WIN32
#ifdef IMA_OS2
				(void)os2_kill(tp->t_pid, SIGTERM);
#else
				(void)kill(tp->t_pid, SIGTERM);
#endif
#else
				(void)kill(tp->t_pid,tp->t_handle,SIGTERM);
#endif
			if (tp->t_authnam)
				(void)unlink(tp->t_authnam);
		}

	/* shutdown slave pvmds / notify master */

	if (netsock != -1) {
		char dummy[DDFRAGHDR];
		int hh;
		struct hostd *hp;

		if (pvmdebmask)
			pvmlogerror("sending FIN|ACK to all pvmds\n");
		for (hh = hosts->ht_last; hh >= 1; hh--)
			if ((hp = hosts->ht_hosts[hh]) && hp->hd_hostpart != myhostpart) {
				pvmput32(dummy, hp->hd_hostpart | TIDPVMD);
				pvmput32(dummy + 4, myhostpart | TIDPVMD);
				pvmput16(dummy + 8, 0);
				pvmput16(dummy + 10, 0);
				pvmput8(dummy + 12, FFFIN|FFACK);
				sendto(netsock, dummy, DDFRAGHDR, 0,
						(struct sockaddr*)&hp->hd_sad, sizeof(hp->hd_sad));
			}
	}

#ifndef NOUNIXDOM
	if (loclspath)
		(void)unlink(loclspath);
#endif

	if (n < 0)
		abort();
	exit(n);
}


void
wrk_fds_init()
{
	wrk_nfds = 0;
	FD_ZERO(&wrk_rfds);
	FD_ZERO(&wrk_wfds);
/*
	FD_ZERO(&wrk_efds);
*/
}


wrk_fds_add(fd, sets)
	int fd;				/* the fd */
	int sets;			/* which sets */
{
#ifdef	SANITY
#ifndef WIN32
	if (fd < 0 || fd >= FD_SETSIZE) {
		pvmlogprintf("wrk_fds_add() bad fd %d\n", fd);
		return 1;
	}
#endif
#endif
	if (sets & 1)
		if ( !FD_ISSET(fd, &wrk_rfds) ) {
			FD_SET(fd, &wrk_rfds);
#ifdef WIN32
			wrk_nfds++;
#endif
		}
	if (sets & 2)
		if ( !FD_ISSET(fd, &wrk_wfds) ) {
			FD_SET(fd, &wrk_wfds);
#ifdef WIN32
			if ( !(sets & 1) ) wrk_nfds++;
#endif
		}
/*
	if (sets & 4)
		if ( !FD_ISSET(fd, &wrk_efds) ) {
			FD_SET(fd, &wrk_efds);
#ifdef WIN32
			wrk_nefds++;
#endif
		}
*/

#ifndef WIN32
	/* if this is new highest, adjust nfds */
	if (fd >= wrk_nfds)
		wrk_nfds = fd + 1;
#endif

	return 0;
}


wrk_fds_delete(fd, sets)
	int fd;				/* the fd */
	int sets;			/* which sets */
{
#ifdef	SANITY
#ifndef WIN32
	if (fd < 0 || fd >= FD_SETSIZE) {
		pvmlogprintf("wrk_fds_delete() bad fd %d\n", fd);
		return 1;
	}
#endif
#endif
	if (sets & 1)
		if ( FD_ISSET(fd, &wrk_rfds) ) {
			FD_CLR(fd, &wrk_rfds);
#ifdef WIN32
			wrk_nfds--;
#endif
		}
	if (sets & 2)
		if ( FD_ISSET(fd, &wrk_wfds) ) {
			FD_CLR(fd, &wrk_wfds);
#ifdef WIN32
			if ( !(sets & 1) ) wrk_nfds--;
#endif
		}
/*
	if (sets & 4)
		if ( FD_ISSET(fd, &wrk_efds) ) {
			FD_CLR(fd, &wrk_efds);
#ifdef WIN32
			wrk_nefds--;
#endif
		}
*/

#ifndef WIN32
	/* if this was highest, may have to adjust nfds to new highest */
	if (fd + 1 == wrk_nfds)
		while (wrk_nfds > 0) {
			wrk_nfds--;
			if (FD_ISSET(wrk_nfds, &wrk_rfds)
			|| FD_ISSET(wrk_nfds, &wrk_wfds)
/*
			|| FD_ISSET(wrk_nfds, &wrk_efds)
*/
			) {
				wrk_nfds++;
				break;
			}
		}
#endif

	return 0;
}


/*	clear_opq_of()
*
*	Clear packets dst for host in opq but _not_ in host hd_opq.
*/

int
clear_opq_of(tid)
	int tid;			/* host */
{
	struct pkt *pp, *pp2;

	for (pp = opq->pk_tlink; pp != opq; pp = pp->pk_tlink) {
		if (pp->pk_dst == tid && !pp->pk_link) {
			pp2 = pp->pk_trlink;
			LISTDELETE(pp, pk_tlink, pk_trlink);
			pk_free(pp);
			pp = pp2;
		}
	}
	return 0;
}


/*	work()
*
*	The whole sausage
*/

work()
{
	static int lastpinged = 0;	/* host that got last keepalive msg */
#ifdef FDSETNOTSTRUCT
	fd_set rfds, wfds;			/* result of select */
	/* fd_set efds; */
#else
	struct fd_set rfds, wfds;	/* result of select */
	/* struct fd_set efds; */
#endif
	int nrdy;					/* number of fds ready after select */
	struct timeval tbail;		/* time to bail if state = STARTUP */
	struct timeval tping;		/* time to send next keepalive packet */
	struct timeval tnow;
	struct timeval tout;
	struct pmsg *mp;
	struct task *tp;
	struct hostd *hp;
#if defined(IMA_PGON) || defined(IMA_I860) || defined(SHMEM)
	int nodemsg = 0;
#if defined(IMA_PGON)
	struct timeval tpgon;		/* time to spend in paragon select */
	double tbpl;				/* time at beginning of probe loop */
	double toutpl;				/* timeout in the probe loop */
	int timed_out;
	extern double dclock();
#endif
#endif
#ifdef	SHMEM
	int someclosed;
#endif

	gettimeofday(&tnow, (struct timezone*)0);
	if (pvmdebmask || myhostpart) {
		PVM_TIMET time_temp;
		pvmlogprintf("%s (%s) %s %s\n",
				hosts->ht_hosts[hosts->ht_local]->hd_name,
				inadport_decimal(&hosts->ht_hosts[hosts->ht_local]->hd_sad),
				myarchname,
				PVM_VER);
		pvmlogprintf("ready ");
		time_temp = (PVM_TIMET) tnow.tv_sec;
		pvmlogprintf(ctime(&time_temp));
	}

	/*
	* check for default plug-in modules (& start them)
	* (only if pvmd, not for pvmd'...)
	*/

	if ( myhostpart ) {
		char *av[5];
		char *buf;
		char *cmd;
		int ac;
		int i;

		for ( i=0 ; modulenames[i] ; i++ ) {
			cmd = getenv( modulenames[i] );
			if ( cmd != NULL )
			{
				buf = STRALLOC( cmd );
				ac = sizeof(av)/sizeof(av[0]);
				if (!buf || acav( buf, &ac, av )) {
					pvmlogprintf( "$%s: Line Too Long \"%s\".\n",
							modulenames[i], cmd );
				} else {
					av[ ac ] = (char *) NULL;
					locl_spawn( av[0], av + 1 );
				}
				if (buf) PVM_FREE( buf );
			}
		}
	}

	/*
	* remind myself to start those pesky slave pvmds
	*/

	if (addmesg) {
		struct pmsg *mp = addmesg;

		addmesg = 0;
		sendmessage(mp);
	}

	/*
	* init bail (for PVMDSTARTUP) and ping times
	*/

	pvmgetclock(&tnow);
	tout.tv_sec = DDBAILTIME;
	tout.tv_usec = 0;
	TVXADDY(&tbail, &tnow, &tout);

	tout.tv_sec = DDPINGTIME;
	tout.tv_usec = 0;
	TVXADDY(&tping, &tnow, &tout);

	/* init select fd sets */

	wrk_fds_init();

	if (loclsock >= 0)
		wrk_fds_add(loclsock, 1);
	wrk_fds_add(netsock, 1);

	for (; ; ) {

		/*
		*	clean up after any tasks that we got SIGCHLDs for
		*/
#ifdef IMA_BEOSCYLD
		reap(SIGCLD);
#endif
		while (rdead != wdead) {
			if (deads[rdead].dd_pid == pprime) {
				int cc;
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
		|| defined(IMA_AIX56K64) || defined(IMA_LINUXALPHA)
				unsigned int oslen;
#else
				size_t oslen;
#endif
#else
				int oslen;
#endif
				struct sockaddr_in osad;
				struct timeval t;
				char buf[DDFRAGHDR];

				hostfailentry(hosts->ht_hosts[0]);
				clear_opq_of((int)(TIDPVMD | hosts->ht_hosts[0]->hd_hostpart));
				pprime = 0;

				while (1) {
					FD_ZERO(&rfds);
					FD_SET(ppnetsock, &rfds);
					t.tv_sec = 0;
					t.tv_usec = 0;
					cc = select(ppnetsock + 1,
#ifdef	FDSETISINT
							(int *)&rfds, (int *)0, (int *)0,
#else
							(fd_set *)&rfds, (fd_set *)0, (fd_set *)0,
#endif
							&t);
					if (cc == 1) {
						oslen = sizeof(osad);
						recvfrom(ppnetsock, buf, sizeof(buf),
								0, (struct sockaddr*)&osad, &oslen);

					} else if (cc != -1 || errno != EINTR)
						break;
				}

			} else {
				if (tp = task_findpid(deads[rdead].dd_pid)) {

		/* check for output one last time
		   XXX this could be cleaner by going through main select again
		   XXX before flushing the task */

					tp->t_status = deads[rdead].dd_es;
					tp->t_utime = deads[rdead].dd_ut;
					tp->t_stime = deads[rdead].dd_st;
					while (tp->t_out >= 0) {

#ifdef FDSETNOTSTRUCT
						fd_set rfds;
#else
						struct fd_set rfds;
#endif

						FD_ZERO(&rfds);
						FD_SET(tp->t_out, &rfds);
						TVCLEAR(&tout);
						if (select(tp->t_out + 1,
#ifdef	FDSETISINT
								(int *)&rfds, (int *)0, (int *)0,
#else
								(fd_set *)&rfds, (fd_set *)0, (fd_set *)0,
#endif
								&tout) == 1)
							loclstout(tp);

						else
							break;
					}
#if defined(IMA_PGON) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
	|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
					mpp_free(tp);
#endif
					task_cleanup(tp);
					task_free(tp);
				}
			}
			if (++rdead >= ndead)
				rdead = 0;
		}

		netoutput();

		if (runstate == PVMDHALTING) {
			pvmlogerror("work() pvmd halting\n");
			pvmbailout(0);
		}

		/* bail if new slave and haven't been configured for too long */
		pvmgetclock(&tnow);
		if (runstate == PVMDSTARTUP && TVXLTY(&tbail, &tnow)) {
			pvmlogerror("work() run = STARTUP, timed out waiting for master\n");
			pvmbailout(0);
		}

		/*
		* send keepalive message to remote pvmd once in a while
		*/
		if (TVXLTY(&tping, &tnow)) {
			if (pvmdebmask & (PDMPACKET|PDMSELECT))
				pvmlogerror("work() ping timer\n");
			if (runstate == PVMDNORMAL || runstate == PVMDHTUPD) {
				do {
					if (++lastpinged > hosts->ht_last)
						lastpinged = 1;
				} while (!(hp = hosts->ht_hosts[lastpinged]));

				if (hp->hd_hostpart != myhostpart
				&& hp->hd_txq->pk_link == hp->hd_txq) {
					mp = mesg_new(0);
					mp->m_tag = DM_NULL;
					mp->m_dst = hp->hd_hostpart | TIDPVMD;
					sendmessage(mp);
				}
			}
			tout.tv_sec = DDPINGTIME;
			tout.tv_usec = 0;
			TVXADDY(&tping, &tnow, &tout);
		}

		/*
		* figure select timeout
		*/

		if (opq->pk_tlink == opq)
			tout = tping;
		else
			tout = opq->pk_tlink->pk_rtv;

		if (TVXLTY(&tout, &tnow)) {
			TVCLEAR(&tout);

		} else {
			TVXSUBY(&tout, &tout, &tnow);
		}

		if (pvmdebmask & PDMSELECT) {
			pvmlogprintf("work() select tout is %d.%06d\n",
					tout.tv_sec, tout.tv_usec);
		}


#ifdef SHMEM
		if ((nodemsg = mpp_probe()) == 1) {
			mpp_input();
			TVCLEAR(&tout);
		}
#endif

		rfds = wrk_rfds;
		wfds = wrk_wfds;
/*
		efds = wrk_efds;
*/
		if (pvmdebmask & PDMSELECT) {
			pvmlogprintf("work() wrk_nfds=%d\n", wrk_nfds);
			print_fdset("work() rfds=", wrk_nfds, &rfds);
			print_fdset("work() wfds=", wrk_nfds, &wfds);
		}

#if !defined(IMA_PGON) && !defined(IMA_I860)

		if ((nrdy = select(wrk_nfds,
#ifdef	FDSETISINT
				(int *)&rfds, (int *)&wfds, (int *)0,
#else
				(fd_set *)&rfds, (fd_set *)&wfds, (fd_set *)0,
#endif
				&tout)) == -1) {
			if (errno != EINTR) {
				pvmlogperror("work() select");
				pvmlogprintf(" wrk_nfds=%d\n", wrk_nfds);
				print_fdset(" rfds=", wrk_nfds, &wrk_rfds);
				print_fdset(" wfds=", wrk_nfds, &wrk_wfds);
				pvmlogprintf(" netsock=%d, ppnetsock=%d, loclsock=%d\n",
						netsock, ppnetsock, loclsock);
				task_dump();
				pvmbailout(0);
			}
		}

#else /*IMA_PGON/IMA_I860*/

		timed_out = 0;
		toutpl = (double)tout.tv_sec + 1e-6*((double)tout.tv_usec);
		tbpl = dclock();   /* use the pgon hw clock */

		if (pvmdebmask & PDMSELECT) {
			pvmlogprintf(
					"work() probe loop timeout is %f dclock is %f\n",
					toutpl,tbpl);
		}

		do {
			totprobes++;
			if ((nodemsg = mpp_input()) > 1)
			{
				mpp_input();
				TVCLEAR(&tpgon);

			} else {
				tout.tv_sec = 0;
				tout.tv_usec = TIMEOUT;
			}
			rfds = wrk_rfds;
			wfds = wrk_wfds;

			nrdy = 0;

			if (totprobes % altprobe == 0)
			{
				totprobes = 0;

				if ((nrdy = select(wrk_nfds,
#ifdef	FDSETISINT
						(int *)&rfds, (int *)&wfds, (int *)0,
#else
						(fd_set *)&rfds, (fd_set *)&wfds, (fd_set *)0,
#endif
						&tout))
				== -1) {
					if (errno != EINTR) {
						pvmlogperror("work() select");
						pvmbailout(0);
					}
				}

				if ( (dclock() - tbpl ) >= toutpl)
					timed_out = 1;
				else
					timed_out = 0;
				if (timed_out && pvmdebmask & PDMSELECT)
				{
					pvmlogprintf(
							"work() probe loop timeout, dclock %f\n",
							dclock());
				}
			}

			/* Try to send packets that are still on the mpp output q */
			mpp_output( (struct task *) NULL, (struct pkt *) NULL);

		} while(!(nrdy || nodemsg || timed_out));

#endif /*IMA_PGON/IMA_I860*/

#ifdef	STATISTICS
		switch (nrdy) {
		case -1:
			stats.selneg++;
			break;
		case 0:
			stats.selzer++;
			break;
		default:
			stats.selrdy++;
			break;
		}
#endif
		if (pvmdebmask & PDMSELECT) {
			pvmlogprintf("work() SELECT returns %d\n", nrdy);
		}

	/*
	*	check network socket and local master socket for action
	*/

		if (nrdy > 0) {
			if (FD_ISSET(netsock, &rfds)) {
				nrdy--;
				netinput();
			}
			if (loclsock >= 0 && FD_ISSET(loclsock, &rfds)) {
				nrdy--;
				loclconn();
			}
		}

	/*
	*	check tasks for action
	*/

#ifdef	SHMEM
		someclosed = 0;
#endif
		if (loclsock >= 0) {
			for (tp = locltasks->t_link;
					nrdy > 0 && tp != locltasks;
					tp = tp->t_link) {

				if (tp->t_sock >= 0 && FD_ISSET(tp->t_sock, &rfds)) {
					FD_CLR(tp->t_sock, &rfds);
					nrdy--;
					if (loclinput(tp)) {
#ifdef	SHMEM
						if (tp->t_tid == 0)
							someclosed++;
#endif
						if (pvmdebmask & PDMTASK) {
							pvmlogprintf(
									"work() error reading from t%x, marking dead\n",
									tp->t_tid);
						}
						if (!(tp->t_flag & TF_FORKD)) {
							tp = tp->t_rlink;
							task_cleanup(tp->t_link);
							task_free(tp->t_link);

						} else
							wrk_fds_delete(tp->t_sock, 3);
						continue;
					}
				}

				if (tp->t_sock >= 0 && FD_ISSET(tp->t_sock, &wfds)) {
					FD_CLR(tp->t_sock, &wfds);
					nrdy--;
					if (locloutput(tp)) {
#ifdef	SHMEM
						if (tp->t_tid == 0)
							someclosed++;
#endif
						if (!(tp->t_flag & TF_FORKD)) {
							tp = tp->t_rlink;
							task_cleanup(tp->t_link);
							task_free(tp->t_link);

						} else
							wrk_fds_delete(tp->t_sock, 3);
						continue;
					}
				}

				if (tp->t_out >= 0 && FD_ISSET(tp->t_out, &rfds)) {
					FD_CLR(tp->t_out, &rfds);
					nrdy--;
					loclstout(tp);
				}
			}
		}
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
	|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
		mpp_output((struct task *)0, (struct pkt *)0);
#endif
#ifdef	SHMEM
		if (someclosed)
			mpp_dredge();
#endif
	}
}


/*	locl_spawn()
*
*	Shortcut to spawn a task on local host.
*		(a.k.a. taskers, etc, at startup)
*/

void
locl_spawn( file, argv )
char *file;
char **argv;
{
	struct pmsg *spawnmesg;

	char **ep;

	int i;
	int n;

	spawnmesg = mesg_new(0);
	spawnmesg->m_tag = DM_EXEC;
	spawnmesg->m_dst = TIDPVMD;

	pkint(spawnmesg, PvmParentNotSet);
	pkstr(spawnmesg, file);
	pkint(spawnmesg, PvmTaskDefault | PvmMppFront);
	pkint(spawnmesg, 1); /* number of tasks per host */

	if (argv)
		for (n = 0; argv[n]; n++);
	else
		n = 0;
	pkint(spawnmesg, n);
	for ( i=0 ; i < n ; i++ )
		pkstr(spawnmesg, argv[i]);

	pkint(spawnmesg, 0); /* outtid */
	pkint(spawnmesg, 0); /* outctx */
	pkint(spawnmesg, 0); /* outtag */
	pkint(spawnmesg, 0); /* trctid */
	pkint(spawnmesg, 0); /* trcctx */
	pkint(spawnmesg, 0); /* trctag */

	pkint(spawnmesg, 0); /* inherit pvmd env vars as is */

	pkint(spawnmesg, 0); /* start proc location */
	pkint(spawnmesg, 1); /* total tasks to spawn */

	sendmessage(spawnmesg);
}


/*	netoutput()
*
*	Send packets out the wire to remote pvmds.
*/

netoutput()
{
	struct timeval tnow, tx;
	struct pkt *pp, *pp2;
	struct hostd *hp;
	char *cp;
	int len;
	int cc;
	char dummy[DDFRAGHDR];

/*
	len = 0;
	for (pp = opq->pk_tlink; pp != opq; pp = pp->pk_tlink)
		len++;
	pvmlogprintf("netoutput() %d in opq\n", len);
*/
	if (opq->pk_tlink == opq)
		return 0;

	/*
	* send any pkts whose time has come
	*/

	pvmgetclock(&tnow);

	while ((pp = opq->pk_tlink) != opq && TVXLTY(&pp->pk_rtv, &tnow)) {

	/*
	* fail if we've tried too hard
	*/
		hp = pp->pk_hostd;
		if (pp->pk_nrt >= DDMINRETRIES
		&& pp->pk_rto.tv_sec >= DDMINTIMEOUT) {		/* host is toast */
			pvmlogprintf(
					"netoutput() timed out sending to %s after %d, %d.%06d\n",
					hp->hd_name, pp->pk_nrt,
					pp->pk_rto.tv_sec, pp->pk_rto.tv_usec);
			hd_dump(hp);
			hostfailentry(hp);
			clear_opq_of((int)(TIDPVMD | hp->hd_hostpart));
			ht_delete(hosts, hp);
			if (newhosts)
				ht_delete(newhosts, hp);
			continue;
		}

		cp = pp->pk_dat;
		len = pp->pk_len;
		if (pp->pk_flag & FFSOM) {
			cp -= MSGHDRLEN;
			len += MSGHDRLEN;
			if (cp < pp->pk_buf) {
				pvmlogerror("netoutput() no headroom for message header\n");
				return 0;
			}
			pvmput32(cp, pp->pk_enc);
			pvmput32(cp + 4, pp->pk_tag);
			pvmput32(cp + 8, pp->pk_ctx);
			pvmput32(cp + 16, pp->pk_wid);
			pvmput32(cp + 20, pp->pk_crc);
		}
		cp -= DDFRAGHDR;
		len += DDFRAGHDR;

	/*
	* save under packet header, because databuf may be shared.
	* we don't worry about message header, because it's only at the head.
	*/
		BCOPY(cp, dummy, sizeof(dummy));
		if (cp < pp->pk_buf) {
			pvmlogerror("netoutput() no headroom for packet header\n");
			return 0;
		}

		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf(
			"netoutput() pkt to %s src t%x dst t%x f %s len %d seq %d ack %d retry %d\n",
					hp->hd_name, pp->pk_src, pp->pk_dst, pkt_flags(pp->pk_flag),
					pp->pk_len, pp->pk_seq, pp->pk_ack, pp->pk_nrt);
		}
		pvmput32(cp, pp->pk_dst);
		pvmput32(cp + 4, pp->pk_src);
		pvmput16(cp + 8, pp->pk_seq);
		pvmput16(cp + 10, pp->pk_ack);
		pvmput32(cp + 12, 0);			/* to keep purify happy */
		pvmput8(cp + 12, pp->pk_flag);
#if 0
		/* drop (don't send) random packets */
		if (!(random() & 3)) {
			pvmlogerror("netoutput() darn, dropped one\n");
			cc = -1;
		} else
#endif
			if ((cc = sendto(netsock, cp, len, 0,
					(struct sockaddr*)&hp->hd_sad, sizeof(hp->hd_sad))) == -1
			&& errno != EINTR
#ifndef WIN32
			&& errno != ENOBUFS
#endif
#ifdef IMA_LINUX
			/* some Linux systems report this intermittent error */
			/* && errno != ECONNREFUSED */
#endif
			/* hope this works for all archs, not just linux */
			&& errno != ENOMEM
			) {
				pvmlogperror("netoutput() sendto");
#if defined(IMA_SUN4SOL2) || defined(IMA_X86SOL2) || defined(IMA_SUNMP) || defined(IMA_UXPM) || defined(IMA_UXPV)
	/* life, don't talk to me about life... */
				if (errno == ECHILD)
					pvmlogerror("this message brought to you by solaris\n");
				else
#endif
				pvmbailout(0);
			}
#ifdef	STATISTICS
		if (cc == -1)
			stats.sdneg++;
		else
			stats.sdok++;
#endif

		BCOPY(dummy, cp, sizeof(dummy));	/* restore under header */

	/*
	* set timer for next retry
	*/
		if (cc != -1) {
			if ((pp->pk_flag & (FFFIN|FFACK)) == (FFFIN|FFACK)) {
				pk_free(pp);
				if (hp != hosts->ht_hosts[0]) {
					hostfailentry(hp);
					clear_opq_of((int)(TIDPVMD | hp->hd_hostpart));
					ht_delete(hosts, hp);
					if (newhosts)
						ht_delete(newhosts, hp);
				}
				continue;
			}
			if (!((pp->pk_flag & FFDAT)
					|| (pp->pk_flag & (FFFIN|FFACK)) == FFFIN)) {
				pk_free(pp);
				continue;
			}
			if (!TVISSET(&pp->pk_at))
				pp->pk_at = tnow;
			TVXADDY(&pp->pk_rtv, &tnow, &pp->pk_rta);
			TVXADDY(&pp->pk_rto, &pp->pk_rto, &pp->pk_rta);
#ifdef	STATISTICS
			if (pp->pk_nrt)
				stats.netret++;
#endif
			++pp->pk_nrt;
			if (pp->pk_rta.tv_sec < DDMAXRTT) {
				TVXADDY(&pp->pk_rta, &pp->pk_rta, &pp->pk_rta);
			}

		} else {
			tx.tv_sec = DDERRRETRY/1000000;
			tx.tv_usec = DDERRRETRY%1000000;
			TVXADDY(&pp->pk_rtv, &tnow, &tx);
			TVXADDY(&pp->pk_rto, &pp->pk_rto, &tx);
		}

		/* reinsert packet into opq */

		LISTDELETE(pp, pk_tlink, pk_trlink);
		for (pp2 = opq->pk_trlink; pp2 != opq; pp2 = pp2->pk_trlink)
			if (TVXLTY(&pp2->pk_rtv, &pp->pk_rtv))
				break;
		LISTPUTAFTER(pp2, pp, pk_tlink, pk_trlink);
	}
	return 0;
}


/*	netinput()
*
*	Input from a remote pvmd.
*	Accept a packet, do protocol stuff then pass pkt to netinpkt().
*/

int
netinput()
{
	struct sockaddr_in osad;		/* sender's ip addr */
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
		|| defined(IMA_AIX56K64) || defined(IMA_LINUXALPHA)
	unsigned int oslen;
#else
	size_t oslen;
#endif
#else
	int oslen;
#endif						/* sockaddr length */
	struct timeval tnow;
	struct pkt *pp, *pp2;
	struct hostd *hp;
	char *cp;
	int sqn;
	int aqn;
	int ff;
	int dst;
	int src;
	int hh;
	int already;
	struct timeval tdiff;			/* packet rtt */
	int rttusec;

	/*
	* alloc new pkt buffer and read packet
	*/

	pp = pk_new(pvmudpmtu);
	if (TDFRAGHDR > DDFRAGHDR)
		pp->pk_dat += TDFRAGHDR - DDFRAGHDR;

	oslen = sizeof(osad);
	if ((pp->pk_len = recvfrom(netsock, pp->pk_dat,
			pp->pk_max - (pp->pk_dat - pp->pk_buf),
			0, (struct sockaddr*)&osad, &oslen)) == -1) {
		if (errno != EINTR)
			pvmlogperror("netinput() recvfrom(netsock)");
		goto scrap;
	}

#if 0
	/* drop random packets */
	if (!(random() & 3)) {
		pvmlogerror("netinput() oops, dropped one\n");
		goto scrap;
	}
#endif

#ifdef	STATISTICS
	stats.rfok++;
#endif

	cp = pp->pk_dat;
	pp->pk_len -= DDFRAGHDR;
	pp->pk_dat += DDFRAGHDR;
	dst = pp->pk_dst = pvmget32(cp);
	src = pp->pk_src = pvmget32(cp + 4);
	sqn = pp->pk_seq = pvmget16(cp + 8);
	aqn = pvmget16(cp + 10);
	ff = pp->pk_flag = pvmget8(cp + 12);
	if (ff & FFSOM) {
		if (pp->pk_len < MSGHDRLEN) {
			pvmlogprintf("netinput() SOM pkt src t%x dst t%x too short\n",
					src, dst);
			goto scrap;
		}
		cp += DDFRAGHDR;
		pp->pk_enc = pvmget32(cp);
		pp->pk_tag = pvmget32(cp + 4);
		pp->pk_ctx = pvmget32(cp + 8);
		pp->pk_wid = pvmget32(cp + 16);
		pp->pk_crc = pvmget32(cp + 20);
		pp->pk_len -= MSGHDRLEN;
		pp->pk_dat += MSGHDRLEN;
	}

	/*
	* make sure it's from where it claims
	*/

	hh = (src & tidhmask) >> (ffs(tidhmask) - 1);
	if (hh < 0 || hh > hosts->ht_last || !(hp = hosts->ht_hosts[hh])
/* #ifndef IMA_LINUX */
/* appears to be O.K. now in RedHat 5.0... JAK 1/28/98 */
	/*
	* XXX removing these lines is a hack and reduces security between
	* XXX pvmds somewhat, but it's the easiest fix for Linux right now.
	*/
	|| (osad.sin_addr.s_addr != hp->hd_sad.sin_addr.s_addr)
	|| (osad.sin_port != hp->hd_sad.sin_port)
/* #endif */
	) {
		pvmlogprintf("netinput() bogus pkt from %s\n",
				inadport_decimal(&osad));
		goto scrap;
	}

	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf(
		"netinput() pkt from %s src t%x dst t%x f %s len %d seq %d ack %d\n",
				hp->hd_name, src, dst, pkt_flags(ff), pp->pk_len, sqn, aqn);
	}

	if ((ff & (FFFIN|FFACK)) == (FFFIN|FFACK)) {
		if (hh == hosts->ht_master) {
	/*
	* FIN|ACK from master means we should bailout
	*/
			if (runstate == PVMDPRIME) {
				if (pvmdebmask & PDMSTARTUP)
					pvmlogerror("work() PVMDPRIME halting\n");
				exit(0);
			}
			pvmlogprintf("netinput() FIN|ACK from master (%s)\n",
					hp->hd_name);
			runstate = PVMDHALTING;

		} else {
	/*
	* FIN|ACK from slave means it croaked
	*/
			pvmlogprintf("netinput() FIN|ACK from %s\n",
					hp->hd_name);
			hd_dump(hp);
			hostfailentry(hp);
			clear_opq_of((int)(TIDPVMD | hp->hd_hostpart));
			if (hp->hd_hostpart) {
				ht_delete(hosts, hp);
				if (newhosts)
					ht_delete(newhosts, hp);
			}
		}
		goto scrap;
	}

	/*
	* done with outstanding packet covered by this ack
	*/

	if (ff & FFACK) {
		for (pp2 = hp->hd_opq->pk_link; pp2 != hp->hd_opq; pp2 = pp2->pk_link)
			if (pp2->pk_seq == aqn) {
				if (pp2->pk_flag & FFDAT) {
					if (pp2->pk_nrt == 1) {
						pvmgetclock(&tnow);

						TVXSUBY(&tdiff, &tnow, &pp2->pk_at);
						rttusec = tdiff.tv_sec * 1000000 + tdiff.tv_usec;
						if (rttusec < 1)
							rttusec = 1000;	/* XXX const */
						else
							if (rttusec > DDMAXRTT*1000000)
								rttusec = DDMAXRTT*1000000;
						rttusec += 3 * (hp->hd_rtt.tv_sec * 1000000 + hp->hd_rtt.tv_usec);
						rttusec /= 4;
						hp->hd_rtt.tv_sec = rttusec / 1000000;
						hp->hd_rtt.tv_usec = rttusec % 1000000;
					}
				}
				if (pp2->pk_flag & FFFIN) {
					finack_to_host(hp);
				}
				hp->hd_nop--;
				LISTDELETE(pp2, pk_link, pk_rlink);
				pk_free(pp2);
				break;
			}
	}

	/*
	* move another pkt to output q
	*/

/*
	if ((hp->hd_opq->pk_link == hp->hd_opq)
*/
	if (hp->hd_nop < nopax
	&& (hp->hd_txq->pk_link != hp->hd_txq)) {
		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf("netinput() pkt to opq\n");
		}
		pp2 = hp->hd_txq->pk_link;
		LISTDELETE(pp2, pk_link, pk_rlink);
		TVCLEAR(&pp2->pk_rtv);
		TVXADDY(&pp2->pk_rta, &hp->hd_rtt, &hp->hd_rtt);
		TVCLEAR(&pp2->pk_rto);
		TVCLEAR(&pp2->pk_at);
		pp2->pk_nrt = 0;
		pp2->pk_hostd = hp;
		pp2->pk_seq = hp->hd_txseq;
		hp->hd_txseq = NEXTSEQNUM(hp->hd_txseq);
		LISTPUTBEFORE(hp->hd_opq, pp2, pk_link, pk_rlink);
		hp->hd_nop++;
		LISTPUTBEFORE(opq, pp2, pk_tlink, pk_trlink);
	}

	if (!(ff & (FFDAT|FFFIN)))
		goto scrap;

	/*
	* send an ack for the pkt
	*/

	pp2 = pk_new(DDFRAGHDR);	/* XXX could reref a dummy databuf here */
	pp2->pk_dat += DDFRAGHDR;
	pp2->pk_dst = hp->hd_hostpart | TIDPVMD;
	pp2->pk_src = pvmmytid;
	pp2->pk_flag = FFACK;
	TVCLEAR(&pp2->pk_rtv);
	TVCLEAR(&pp2->pk_rta);
	TVCLEAR(&pp2->pk_rto);
	TVCLEAR(&pp2->pk_at);
	pp2->pk_nrt = 0;
	pp2->pk_hostd = hp;
	pp2->pk_seq = 0;
	pp2->pk_ack = sqn;
	/*
	 * Bogus!  Acks can't be put at end of opq, as could be
	 * stuck behind packet retries with non-zero pk_rtv's...
	 * Better place Acks, in order, after other zero-rtv-ed (new)
	 * packets, but *before* any retry packets...
	 */
	/* LISTPUTAFTER(opq, pp2, pk_tlink, pk_trlink); */
	{
		struct pkt *pp3;
		for (pp3 = opq->pk_tlink; pp3 != opq; pp3 = pp3->pk_tlink)
			if (TVXLTY(&pp2->pk_rtv, &pp3->pk_rtv))
				break;
		LISTPUTBEFORE(pp3, pp2, pk_tlink, pk_trlink);
	}

	if (!(ff & FFDAT))
		goto scrap;

	/*
	* if we don't have it already, put it in reordering q
	*/

	pp2 = 0;
	if (SEQLESSTHAN(sqn, hp->hd_rxseq))
		already = 1;
	else {
		already = 0;
		for (pp2 = hp->hd_rxq->pk_link; pp2 != hp->hd_rxq; pp2 = pp2->pk_link)
			if (!SEQLESSTHAN(pp2->pk_seq,sqn)) {
				if (pp2->pk_seq == sqn)
					already = 1;
				break;
			}
	}
	if (already) {
		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf("netinput() pkt resent from %s seq %d\n",
					hp->hd_name, sqn);
		}
		goto scrap;
	}

	LISTPUTBEFORE(pp2, pp, pk_link, pk_rlink);

	/*
	* accept pkts from reordering q
	*/

	while (pp = hp->hd_rxq->pk_link,
			pp != hp->hd_rxq && pp->pk_seq == hp->hd_rxseq) {
		hp->hd_rxseq = NEXTSEQNUM(hp->hd_rxseq);
		LISTDELETE(pp, pk_link, pk_rlink);
		netinpkt(hp, pp);
	}
	return 0;

scrap:
	if (pp)
		pk_free(pp);
	return 0;
}


/*	netinpkt()
*
*	Consume pkt from network.  It's either for the pvmd and needs to
*	be reassembled into a message or it's for a local task and needs
*	to be put on the queue to be sent.
*/

netinpkt(hp, pp)
	struct hostd *hp;
	struct pkt *pp;
{
	struct mca *mcap = 0;
	struct task *tp;
	struct pmsg *mp;
	struct frag *fp;
	struct pkt *pp2;
	int src = pp->pk_src;
	int dst = pp->pk_dst;
	int ff = pp->pk_flag;
	char *cp;
	int i;
	int firstmca = 1;

	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf(
		"netinpkt() pkt from %s src t%x dst t%x f %s len %d\n",
				hp->hd_name, src, dst, pkt_flags(ff), pp->pk_len);
	}

	/* throw out packet if it's not for us */

	if (TIDISMCA(dst)) {
		for (mcap = hp->hd_mcas->mc_link; mcap != hp->hd_mcas;
				mcap = mcap->mc_link)
			if (mcap->mc_tid == dst)
				break;
		if (mcap == hp->hd_mcas)
			mcap = 0;
	}

	if ((dst & tidhmask) != myhostpart && !mcap) {
		if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
			pvmlogprintf(
					"netinpkt() pkt from t%x for t%x scrapped (not us)\n",
					src, dst);
		}
		goto done;
	}

	if (mcap) {

		for (i = mcap->mc_ndst; i-- > 0; ) {
			dst = mcap->mc_dsts[i];
			if (tp = task_find(dst)) {		/* to local task */
				pp2 = pk_new(0);
				pp2->pk_src = src;
				pp2->pk_dst = dst;
				pp2->pk_flag = ff;

#if defined(IMA_MPP)
				if (firstmca)
				{
					pp2->pk_flag |= (FFMCA | FFMCAWH);
					firstmca = 0;
				}
				else
					pp2->pk_flag |= FFMCA;
#endif
				pp2->pk_enc = pp->pk_enc;
				pp2->pk_tag = pp->pk_tag;
				pp2->pk_ctx = pp->pk_ctx;
				pp2->pk_wid = pp->pk_wid;
				pp2->pk_crc = pp->pk_crc;
				pp2->pk_buf = pp->pk_buf;
				pp2->pk_max = pp->pk_max;
				pp2->pk_dat = pp->pk_dat;
				pp2->pk_len = pp->pk_len;
				da_ref(pp->pk_buf);

				pkt_to_task(tp, pp2);

			} else
				if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
					pvmlogprintf(
					"netinpkt() mc pkt from t%x for t%x scrapped (no dst)\n",
							src, dst);
				}
		}

		if (ff & FFEOM) {
			if (pvmdebmask & PDMMESSAGE) {
				pvmlogprintf("netinpkt() freed mca %x from t%x\n",
						mcap->mc_tid, hp->hd_name);
			}
			mca_free(mcap);
		}
		goto done;
	}

	if ((dst & ~tidhmask) == TIDPVMD) {		/* for pvmd */
		if (ff & FFSOM) {			/* start of message */
			if (hp->hd_rxm) {
				pvmlogprintf("netinpkt() repeated start pkt from %s\n",
						hp->hd_name);
				goto done;
			}
			hp->hd_rxm = mesg_new(0);
			hp->hd_rxm->m_enc = pp->pk_enc;
			hp->hd_rxm->m_tag = pp->pk_tag;
			hp->hd_rxm->m_ctx = pp->pk_ctx;
			hp->hd_rxm->m_wid = pp->pk_wid;
			hp->hd_rxm->m_crc = pp->pk_crc;
			hp->hd_rxm->m_dst = dst;
			hp->hd_rxm->m_src = src;

		} else {					/* middle or end of message */
			if (!hp->hd_rxm) {
				pvmlogprintf(
						"netinpkt() spurious pkt (no message) from %s\n",
						hp->hd_name);
				goto done;
			}
		}

		fp = fr_new(0);
		fp->fr_buf = pp->pk_buf;
		fp->fr_dat = pp->pk_dat;
		fp->fr_max = pp->pk_max;
		fp->fr_len = pp->pk_len;
		da_ref(pp->pk_buf);
		LISTPUTBEFORE(hp->hd_rxm->m_frag, fp, fr_link, fr_rlink);
		hp->hd_rxm->m_len += fp->fr_len;

		if (ff & FFEOM) {		/* end of message */
			mp = hp->hd_rxm;
			hp->hd_rxm = 0;
#ifdef	MCHECKSUM
			if (mp->m_crc != mesg_crc(mp)) {
				pvmlogprintf(
						"netinpkt() message from t%x to t%x bad checksum\n",
						src, dst);
	/* XXX must free message? */
				goto done;
			}
#endif
			mesg_rewind(mp);
			if (TIDISTASK(src)) {
				if (src == pvmschedtid) {
					schentry(mp);

				} else
					if (pvmdebmask & (PDMMESSAGE|PDMAPPL)) {
						pvmlogprintf(
						"netinpkt() mesg from t%x to t%x tag %d scrapped\n",
								src, dst, mp->m_tag);
	/* XXX must free message? */
					}

			} else {
				netentry(hp, mp);
			}
		}

	} else {								/* for a task */
		if (tp = task_find(dst)) {

#if defined(IMA_PGON) || defined(IMA_I860)
			if (TIDISNODE(dst))
				mpp_output(tp, pp);
			else
#endif
				pkt_to_task(tp, pp);
			pp = 0;

		} else {
			if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
				pvmlogprintf(
						"netinpkt() pkt from t%x for t%x scrapped (no dst)\n",
						src, dst);
	/* XXX must free message? */
			}
			goto done;
		}
	}

done:
	if (pp)
		pk_free(pp);
	return 0;
}


/*	loclconn()
*
*	Task has attempted to connect.  Accept the new connection and make
*	a blank context for it.
*/

loclconn()
{
	struct task *tp;			/* new task context */
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
		|| defined(IMA_AIX56K64) || defined(IMA_LINUXALPHA)
	unsigned int oslen;
#else
	size_t oslen;
#endif
#else
	int oslen;
#endif
	int i;
#ifndef NOUNIXDOM
	struct sockaddr_un uns;
#endif

/*
#ifdef SHMEM
 	return 0;
	just getma outa here boy!
#endif
*/

	if ((tp = task_new(0)) == NULL) {
		pvmlogprintf("loclconn() too many tasks?\n" );
		return PvmOutOfRes;
	}

#ifdef NOUNIXDOM
	tp->t_salen = sizeof(tp->t_sad);

	oslen = sizeof(tp->t_sad);
	if ((tp->t_sock = accept(loclsock, (struct sockaddr*)&tp->t_sad,
			&oslen)) == -1) {
		pvmlogperror("loclconn() accept");
		task_free(tp);
		tp = 0;

	} else {
		if (pvmdebmask & (PDMPACKET|PDMTASK)) {
			pvmlogprintf("loclconn() accept from %s sock %d\n",
					inadport_decimal(&tp->t_sad), tp->t_sock);
		}
#ifndef NOSOCKOPT
		i = 1;
		if (setsockopt(tp->t_sock, IPPROTO_TCP, TCP_NODELAY,
				(char*)&i, sizeof(int)) == -1) {
			pvmlogperror("loclconn() setsockopt");
		}
#endif
	}

#else /*NOUNIXDOM*/
	oslen = sizeof(uns);
	if ((tp->t_sock = accept(loclsock, (struct sockaddr*)&uns, &oslen)) == -1) {
		pvmlogperror("loclconn() accept");
		task_free(tp);
		tp = 0;

	} else {
		if (pvmdebmask & (PDMPACKET|PDMTASK))
			pvmlogerror("loclconn() accept\n");
	}

#endif /*NOUNIXDOM*/

	if (tp) {
#ifndef WIN32
		if ((i = fcntl(tp->t_sock, F_GETFL, 0)) == -1)
			pvmlogperror("loclconn: fcntl");
		else {
#ifdef	IMA_RS6K
	/* did you ever feel as though your mind had started to erode? */
			i |= O_NONBLOCK;
#else	/*IMA_RS6K*/
#ifdef O_NDELAY
			i |= O_NDELAY;
#else
			i |= FNDELAY;
#endif
#endif	/*IMA_RS6K*/
			(void)fcntl(tp->t_sock, F_SETFL, i);
		}
#endif
		wrk_fds_add(tp->t_sock, 1);
	}

	return 0;
}


/*	locloutput()
*
*	Output to local task.  Sends packets until write() blocks.
*	Deletes task's bit from wrk_wfds if no more data to send.
*
*	Returns 0 if okay, else -1 if unrecoverable error.
*/

locloutput(tp)
	struct task *tp;
{
	struct pkt *pp;
	char *cp;
	int len;
	int n;

	while ((pp = tp->t_txq->pk_link)->pk_buf) {

		if (!pp->pk_cpos || pp->pk_cpos < pp->pk_dat) {
	/*
	* prepend frag [message] headers if we'll be writing them.
	*/
			cp = pp->pk_dat;
			len = pp->pk_len;
			if (pp->pk_flag & FFSOM) {
				cp -= MSGHDRLEN;
				len += MSGHDRLEN;
				if (cp < pp->pk_buf) {
					pvmlogerror("locloutput() no headroom for message header\n");
					return 0;
				}
				pvmput32(cp, pp->pk_enc);
				pvmput32(cp + 4, pp->pk_tag);
				pvmput32(cp + 8, pp->pk_ctx);
				pvmput32(cp + 16, pp->pk_wid);
				pvmput32(cp + 20, pp->pk_crc);
			}
			cp -= TDFRAGHDR;
			if (cp < pp->pk_buf) {
				pvmlogerror("locloutput() no headroom for packet header\n");
				return 0;
			}
			pvmput32(cp, pp->pk_dst);
			pvmput32(cp + 4, pp->pk_src);
			pvmput32(cp + 8, len);
			pvmput32(cp + 12, 0);			/* to keep purify happy */
			pvmput8(cp + 12, pp->pk_flag & (FFSOM|FFEOM));
			len += TDFRAGHDR;
		}

		if (pp->pk_cpos) {
			cp = pp->pk_cpos;
			len = pp->pk_len + (pp->pk_dat - cp);

		} else {
			pp->pk_cpos = cp;
			if (pvmdebmask & PDMPACKET) {
				pvmlogprintf(
					"locloutput() src t%x dst t%x f %s len %d\n",
					pp->pk_src, pp->pk_dst, pkt_flags(pp->pk_flag), len);
			}
		}

	/*
	* send as much as possible; skip to next packet when all sent
	*/

#if defined(IMA_RS6K) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
		n = write(tp->t_sock, cp, min(len, 4096));
#else
#ifndef WIN32
		n = write(tp->t_sock, cp, len);
#else
	 	n = win32_write_socket(tp->t_sock,cp,len);
#endif
#endif

#ifdef	STATISTICS
		if (n == -1)
			stats.wrneg++;
		else
			if (!n)
				stats.wrzer++;
			else
				if (n == len)
					stats.wrok++;
				else
					stats.wrshr++;
#endif
		if (n == -1) {
			if (errno != EINTR
#ifndef WIN32
					&& errno != EWOULDBLOCK
					&& errno != ENOBUFS
#endif
					&& errno != EAGAIN)
			{
#ifdef WIN32
			if (GetLastError() != WSAECONNRESET) {
#endif
				pvmlogperror("locloutput() write");
				pvmlogprintf("locloutput() marking t%x dead\n",
						tp->t_tid);
#ifdef WIN32
			}
#endif
				return -1;
			}
			break;
		}

		if (n > 0) {
			if (pvmdebmask & PDMPACKET) {
				pvmlogprintf(
						"locloutput() src t%x dst t%x wrote %d\n",
						pp->pk_src, pp->pk_dst, n);
			}
			if ((len - n) > 0) {
				pp->pk_cpos += n;

			} else {
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
	|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
				int dst = pp->pk_dst;
#endif
				LISTDELETE(pp, pk_link, pk_rlink);
				pk_free(pp);
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
	|| defined(IMA_AIX5SP2)
				/* Not done for BEOLIN, since tp2 is tp, */
				/* the task on which we are working (PLS) */
				if (TIDISNODE(dst)) {
					struct task *tp2;

					/* Expensive! But what else can we do? */
					if ((tp2 = task_find(dst)) && (tp2->t_flag & TF_CLOSE)) {
						mpp_free(tp2);
						/* XXX task_cleanup(tp2); */
						task_free(tp2);
					}
				}
#endif /*defined(IMA_CM5) || defined(IMA_SP2MPI)*/
			}

		} else
			break;
	}

	if (tp->t_txq->pk_link == tp->t_txq) {
		wrk_fds_delete(tp->t_sock, 2);

	/* flush context if TF_CLOSE set */

		if (tp->t_flag & TF_CLOSE)
			return -1;
	}

	return 0;
}


/*	loclinput()
*
*	Input from a task.
*	Accept a packet and pass pkt to loclinpkt().
*	Returns 0 else -1 if error (work() should clean up the task context).
*/

loclinput(tp)
	struct task *tp;
{
	struct pkt *pp = 0;
	struct pkt *pp2;
	int n, m;

again:
	/*
	* if no current packet, start a new one
	*/

	if (!tp->t_rxp) {
		tp->t_rxp = pk_new(pvmudpmtu);
/*
		tp->t_rxp = pk_new(TDFRAGHDR + 2);
*/
		if (DDFRAGHDR > TDFRAGHDR)
			tp->t_rxp->pk_dat += DDFRAGHDR - TDFRAGHDR;
	}
	pp = tp->t_rxp;

	/*
	* read the fragment header and body separately so we can
	* make a bigger buffer if needed
	*/

	n = (pp->pk_len < TDFRAGHDR) ? 0 : pvmget32(pp->pk_dat + 8);
	n += TDFRAGHDR - pp->pk_len;
	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("loclinput() t%x fr_len=%d fr_dat=+%d n=%d\n",
				tp->t_tid, pp->pk_len, pp->pk_dat - pp->pk_buf, n);
	}
#ifndef WIN32
	n = read(tp->t_sock, pp->pk_dat + pp->pk_len, n);
#else
	n = win32_read_socket(tp->t_sock,pp->pk_dat + pp->pk_len,n);
#endif
	if (pvmdebmask & PDMPACKET) {
		if (n >= 0) {
			pvmlogprintf("loclinput() read=%d\n", n);
		} else
			pvmlogperror("loclinput() read");
	}

#ifdef	STATISTICS
		switch (n) {
		case -1:
			stats.rdneg++;
			break;
		case 0:
			stats.rdzer++;
			break;
		default:
			stats.rdok++;
			break;
		}
#endif
	if (n == -1) {
		if (errno != EINTR
#ifndef WIN32
				&& errno != EWOULDBLOCK
#endif
		) {
			pvmlogperror("loclinput() read");
			pvmlogprintf("loclinput() marking t%x dead\n",
					tp->t_tid);
			return -1;
		}
		return 0;
	}
	if (!n) {
		if (pvmdebmask & (PDMPACKET|PDMMESSAGE|PDMTASK)) {
			pvmlogprintf("loclinput() read EOF from t%x sock %d\n",
					tp->t_tid, tp->t_sock);
		}
		return -1;
	}

	if ((pp->pk_len += n) < TDFRAGHDR)
		return 0;

	/*
	* if we have a complete frag, accept it
	*/

	m = TDFRAGHDR + pvmget32(pp->pk_dat + 8);
	if (pp->pk_len == m) {
		tp->t_rxp = 0;
		pp->pk_dst = pvmget32(pp->pk_dat);
#if defined(IMA_PGON) || defined(IMA_I860) || defined(IMA_CM5) \
		|| defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
		pp->pk_src = pvmget32(pp->pk_dat + 4);
#else
		pp->pk_src = tp->t_tid;
#endif
		pp->pk_flag = pvmget8(pp->pk_dat + 12);
		pp->pk_len -= TDFRAGHDR;
		pp->pk_dat += TDFRAGHDR;
		if (pp->pk_flag & FFSOM) {
			if (pp->pk_len < MSGHDRLEN) {
				pvmlogprintf(
						"loclinput() SOM pkt src t%x dst t%x too short\n",
						pp->pk_src, pp->pk_dst);
				pk_free(pp);
				return 0;
			}
			pp->pk_enc = pvmget32(pp->pk_dat);
			pp->pk_tag = pvmget32(pp->pk_dat + 4);
			pp->pk_ctx = pvmget32(pp->pk_dat + 8);
			pp->pk_wid = pvmget32(pp->pk_dat + 16);
			pp->pk_crc = pvmget32(pp->pk_dat + 20);
			pp->pk_len -= MSGHDRLEN;
			pp->pk_dat += MSGHDRLEN;
		}
		if (loclinpkt(tp, pp))
			return -1;
		return 0;
	}

	/* realloc buffer if frag won't fit */

	if (pp->pk_len == TDFRAGHDR) {
		if (m > pp->pk_max - (pp->pk_dat - pp->pk_buf)) {
			if (!(tp->t_flag & TF_CONN)) {
				pvmlogprintf(
					"loclinput() unconnected task sends frag length %d (ha)\n",
					m);
				return -1;
			}
			if (DDFRAGHDR > TDFRAGHDR) {
				pp2 = pk_new(m + DDFRAGHDR - TDFRAGHDR);
				pp2->pk_dat += DDFRAGHDR - TDFRAGHDR;
			} else
				pp2 = pk_new(m);
			BCOPY(pp->pk_dat, pp2->pk_dat, TDFRAGHDR);
			pp2->pk_len = pp->pk_len;
			pk_free(pp);
			pp = tp->t_rxp = pp2;
			if (pvmdebmask & PDMPACKET) {
				pvmlogprintf("loclinput() realloc frag max=%d\n", m);
			}
		}
		goto again;
	}

	return 0;
}


/*	loclinpkt()
*
*	Consume pkt from task.
*	If it's for the pvmd it needs to be reassembled into a message.
*	If for a local or foreign task it needs to be put on a queue to be sent.
*	If for a remote pvmd, reassemble as for local then fwd whole message.
*	Returns 0 else -1 if error (work() should cleanup the
*	task context).
*/

loclinpkt(tp, pp)
	struct task *tp;
	struct pkt *pp;
{
	int dst;			/* pkt dst */
	int ff;				/* pkt flags */
	struct pkt *pp2;
	struct frag *fp;
	struct pmsg *mp;
	struct hostd *hp;
	struct task *tp2;
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
	struct task *socktp = tp;	/* owner of the socket */
#endif

	dst = pp->pk_dst;
	ff = pp->pk_flag;
	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf(
				"loclinpkt() src t%x dst t%x f %s len %d\n",
				pp->pk_src, dst, pkt_flags(ff), pp->pk_len);
	}

#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	if (pp->pk_src > 0 && !tp->t_tid && (tp2 = task_findpid(pp->pk_src))) {
		/* connect request from pvmhost */
		mpp_conn(tp, tp2);
		pk_free(pp);
		return -1;
	}
#endif
#if defined(IMA_PGON) || defined(IMA_I860) || defined(IMA_CM5) \
		|| defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
	if (TIDISNODE(pp->pk_src))		/* from a node */
		if (!(tp = task_find(pp->pk_src))) {
			pvmlogprintf("loclinpkt() from unknown task t%x\n", pp->pk_src);
			goto done;
		}
#endif	/*defined(IMA_PGON) || defined(IMA_I860) || defined(IMA_CM5) || defined(IMA_SP2MPI)*/

	/*
	* if to multicast addr, replicate pkt in each q
	*/

	if (TIDISMCA(dst) && tp->t_mca && tp->t_mca->mc_tid == dst) {

		struct mca *mcap = tp->t_mca;
		int i;

		for (i = mcap->mc_ndst; i-- > 0; ) {
			dst = mcap->mc_dsts[i];
			if (hp = tidtohost(hosts, dst)) {
				pp2 = pk_new(0);
				pp2->pk_src = pp->pk_src;
				pp2->pk_dst = mcap->mc_tid;
				pp2->pk_flag = ff;
				pp2->pk_enc = pp->pk_enc;
				pp2->pk_tag = pp->pk_tag;
				pp2->pk_ctx = pp->pk_ctx;
				pp2->pk_wid = pp->pk_wid;
				pp2->pk_crc = pp->pk_crc;
				pp2->pk_buf = pp->pk_buf;
				pp2->pk_max = pp->pk_max;
				pp2->pk_dat = pp->pk_dat;
				pp2->pk_len = pp->pk_len;
				da_ref(pp->pk_buf);

				if (hp->hd_hostpart == myhostpart) {
					netinpkt(hp, pp2);

				} else {
					pkt_to_host(hp, pp2);
				}

			} else
				if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
					pvmlogprintf(
					"loclinpkt() pkt src t%x dst t%x scrapped (no such host)\n",
							pp->pk_src, dst);
				}
		}

	/* free mca on last pkt */

		if (ff & FFEOM) {
			if (pvmdebmask & PDMMESSAGE) {
				pvmlogprintf("loclinpkt() freed mca %x for t%x\n",
						mcap->mc_tid, tp->t_tid);
			}
			mca_free(mcap);
			tp->t_mca = 0;
		}
		goto done;
	}

	/*
	* if to a pvmd, always reassemble (forward if not for us)
	*/

	if ((dst & ~tidhmask) == TIDPVMD) {
		if (ff & FFSOM) {			/* start of message */
			if (tp->t_rxm) {
				pvmlogprintf("loclinpkt() repeated start pkt t%x\n",
						tp->t_tid);
				goto done;
			}
			tp->t_rxm = mesg_new(0);
			tp->t_rxm->m_ctx = pp->pk_ctx;
			tp->t_rxm->m_tag = pp->pk_tag;
			tp->t_rxm->m_enc = pp->pk_enc;
			tp->t_rxm->m_wid = pp->pk_wid;
			tp->t_rxm->m_crc = pp->pk_crc;
			tp->t_rxm->m_dst = dst;
			tp->t_rxm->m_src = tp->t_tid;

		} else {					/* middle or end of message */
			if (!tp->t_rxm) {
				pvmlogprintf(
					"loclinpkt() pkt with no message src t%x\n",
					tp->t_tid);
				goto done;
			}
		}

		fp = fr_new(0);
		fp->fr_buf = pp->pk_buf;
		fp->fr_dat = pp->pk_dat;
		fp->fr_max = pp->pk_max;
		fp->fr_len = pp->pk_len;
		da_ref(pp->pk_buf);

		LISTPUTBEFORE(tp->t_rxm->m_frag, fp, fr_link, fr_rlink);
		tp->t_rxm->m_len += fp->fr_len;

		if (ff & FFEOM) {		/* end of message */
			mp = tp->t_rxm;
			tp->t_rxm = 0;
#ifdef	MCHECKSUM
			if (mp->m_crc != mesg_crc(mp)) {
				pvmlogprintf(
						"loclinpkt() message src t%x dst t%x bad checksum\n",
						mp->m_src, dst);
				goto done;
			}
#endif
			if (!(dst & tidhmask) || (dst & tidhmask) == myhostpart) {	/* local */
				mesg_rewind(mp);
				if (mp->m_tag >= (int)SM_FIRST && mp->m_tag <= (int)SM_LAST
				&& (mp->m_src == pvmschedtid || mp->m_src == hostertid || mp->m_src == taskertid))
				{
					schentry(mp);

				} else {
					loclentry(tp, mp);
				}

			} else {		/* remote */
				if (!tp->t_tid) {
					pvmlogprintf("loclinpkt() pkt src null dst t%x\n", dst);
					goto done;
				}
				sendmessage(mp);
			}
	/*
	* if sock is -1, tm_conn2() wants us to throw out this context
	* because it's been merged into another.
	*/
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
			/* node procs have no socket; they use pvmhost's */
			if (socktp->t_sock == -1)
#else
			if (tp->t_sock == -1)
#endif
			{
				pk_free(pp);
				return -1;
			}
		}
		goto done;
	}

	/*
	* if to a task, put in local or remote send queue
	*/

	if (TIDISTASK(dst)) {
		if (!tp->t_tid) {
			pvmlogprintf("loclinpkt() pkt src null dst t%x\n", dst);
			goto done;
		}
		if ((dst & tidhmask) == myhostpart) {	/* local host */
			if (tp2 = task_find(dst)) {

#if defined(IMA_PGON) || defined(IMA_I860)
				if (TIDISNODE(dst))
					mpp_output(tp2, pp);
				else
#endif
					pkt_to_task(tp2, pp);
/*
				LISTPUTBEFORE(tp2->t_txq, pp, pk_link, pk_rlink);
*/
				pp = 0;

			} else
				if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
					pvmlogprintf(
					"loclinpkt() pkt src t%x dst t%x scrapped (no such task)\n",
							pp->pk_src, dst);
				}

		} else {								/* remote host */
			if (hp = tidtohost(hosts, dst)) {
				pkt_to_host(hp, pp);
				pp = 0;

			} else {
				if (pvmdebmask & (PDMPACKET|PDMAPPL)) {
					pvmlogprintf(
					"loclinpkt() pkt src t%x dst t%x scrapped (no such host)\n",
							pp->pk_src, dst);
				}
				goto done;
			}
		}
	}

done:
	if (pp)
		pk_free(pp);

	return 0;
}


/*	loclstout()
*
*	Read stdout/err pipe from a task.
*	Ship it to the output log tid if set, else send it to the master
*	pvmd to scribble in its log file.
*/

loclstout(tp)
	struct task *tp;
{
	int n;
	struct pmsg *mp;

	static char buf[4000];

#ifndef WIN32
	n = read(tp->t_out, buf, sizeof(buf) - 1);
#else
	n = win32_read_socket(tp->t_out,buf, sizeof(buf) -1);
#endif

	if (n < 1) {
		if (n == 0 || (errno != EINTR
#ifndef WIN32
				&& errno != EWOULDBLOCK
#endif
			)) {
			wrk_fds_delete(tp->t_out, 1);
#ifndef WIN32
			(void)close(tp->t_out);
#else
			(void)_close(tp->t_out);
#endif
			tp->t_out = -1;
			if (tp->t_outtid > 0) {
				mp = mesg_new(0);
				mp->m_dst = tp->t_outtid;
				mp->m_ctx = tp->t_outctx;
				mp->m_tag = tp->t_outtag;
				pkint(mp, tp->t_tid);
				pkint(mp, TO_EOF);
				sendmessage(mp);
				tp->t_outtid = 0;
			}
		}

	} else {
		mp = mesg_new(0);
		pkint(mp, tp->t_tid);
		pkint(mp, n);
		pkbyte(mp, buf, n);
		if (tp->t_outtid > 0) {
			mp->m_dst = tp->t_outtid;
			mp->m_ctx = tp->t_outctx;
			mp->m_tag = tp->t_outtag;

		} else {
			mp->m_tag = DM_TASKOUT;
			mp->m_dst = hosts->ht_hosts[hosts->ht_cons]->hd_hostpart | TIDPVMD;
		}
		sendmessage(mp);
	}
	return 0;
}


/*	mesg_to_task()
*
*	Append a message to the send queue for a task.
*
*	N.B. Message must contain at least one frag or this will honk.
*/

int
mesg_to_task(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	struct frag *fp = mp->m_frag->fr_link;
	struct pkt *pp;
	int ff = FFSOM;			/* frag flags */
	int dst = mp->m_dst;

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("mesg_to_task() dst t%x tag %s len %d\n",
				dst, pvmnametag(mp->m_tag, (int *)0), mp->m_len);
	}

	/* if nothing yet in q, add task's sock to wrk_wfds */

	if (tp->t_sock >= 0)
		wrk_fds_add(tp->t_sock, 2);

	do {
		pp = pk_new(0);
		if (ff & FFSOM) {
			pp->pk_enc = mp->m_enc;
			pp->pk_tag = mp->m_tag;
			pp->pk_ctx = mp->m_ctx;
			pp->pk_wid = mp->m_wid;
#ifdef	MCHECKSUM
			pp->pk_crc = mesg_crc(mp);
#else
			pp->pk_crc = 0;
#endif
		}
		pp->pk_buf = fp->fr_buf;
		pp->pk_dat = fp->fr_dat;
		pp->pk_max = fp->fr_max;
		pp->pk_len = fp->fr_len;
		da_ref(pp->pk_buf);
		if (fp->fr_link == mp->m_frag)
			ff |= FFEOM;
		pp->pk_src = TIDPVMD;
		pp->pk_dst = dst;
		pp->pk_flag = ff;
		ff = 0;
#if defined(IMA_PGON) || defined(IMA_I860)
		if (TIDISNODE(dst)) {
			mpp_output(tp, pp);
			continue;
		}
#endif
#ifdef SHMEM
		if ((tp->t_sock < 0) && ( (tp->t_flag & TF_SHMCONN)
			|| ((tp->t_flag & TF_PRESHMCONN) && (mp->m_flag & MM_PRIO)) ) )
		{
			/* If shmem and no d-t socket or fully connected shmem */
			/* use mpp routine instead of socket one */
			mpp_output(tp, pp);
			continue;
		}
#endif
		if (mp->m_flag & MM_PRIO) {
			LISTPUTAFTER(tp->t_txq, pp, pk_link, pk_rlink);
		} else {
			pkt_to_task(tp, pp);
/*
			LISTPUTBEFORE(tp->t_txq, pp, pk_link, pk_rlink);
*/
		}
	} while ((fp = fp->fr_link) != mp->m_frag);

	return 0;
}


/*	sendmessage()
*
*	Send a message.  If it's for a local task or remote host, cut
*	apart the fragments and queue to be sent.  If it's for the local
*	pvmd, just call netentry() with the whole message.
*
*	N.B. MM_PRIO only works for single-frag messages.
*/

int
sendmessage(mp)
	struct pmsg *mp;
{
	struct hostd *hp = 0;
	struct task *tp;
	struct frag *fp;
	struct pkt *pp;
	int ff = FFSOM;
	int dst = mp->m_dst;

	if (!dst) {
		pvmlogerror("sendmessage() what? to t0\n");
	}

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("sendmessage() dst t%x ctx %d tag %s len %d\n",
				dst, mp->m_ctx, pvmnametag(mp->m_tag, (int *)0), mp->m_len);
	}

	/*
	*	add a frag to empty message to simplify handling
	*/

	if ((fp = mp->m_frag->fr_link) == mp->m_frag) {
		fp = fr_new(MAXHDR);
		fp->fr_dat += MAXHDR;
		LISTPUTBEFORE(mp->m_frag, fp, fr_link, fr_rlink);
	}

	/*
	*	route message
	*/

	if (!(dst & tidhmask) || (dst & tidhmask) == myhostpart) {	/* to local */

		if (TIDISTASK(dst)) {				/* to local task */

			if (tp = task_find(dst)) {
				mesg_to_task(tp, mp);

			} else
				if (pvmdebmask & (PDMMESSAGE|PDMAPPL)) {
					pvmlogprintf(
							"sendmessage() scrapped, no such task t%x\n",
							dst);
				}

		} else {				/* to myself */
			mp->m_ref++;
			mesg_rewind(mp);
			netentry(hosts->ht_hosts[hosts->ht_local], mp);
		}

	} else {					/* to remote */

	/* lookup host */

		if (runstate == PVMDHTUPD)
			hp = tidtohost(newhosts, dst);
		if (!hp && !(hp = tidtohost(hosts, dst))) {
			if (pvmdebmask & (PDMMESSAGE|PDMAPPL)) {
				pvmlogprintf("sendmessage() scrapped, no such host t%x\n",
						dst);
			}
			goto bail;
		}

	/* packetize frags */

		do {
			pp = pk_new(0);
			if (ff & FFSOM) {
				pp->pk_enc = mp->m_enc;
				pp->pk_tag = mp->m_tag;
				pp->pk_ctx = mp->m_ctx;
				pp->pk_wid = mp->m_wid;
#ifdef	MCHECKSUM
				pp->pk_crc = mesg_crc(mp);
#else
				pp->pk_crc = 0;
#endif
			}
			pp->pk_buf = fp->fr_buf;
			pp->pk_dat = fp->fr_dat;
			pp->pk_max = fp->fr_max;
			pp->pk_len = fp->fr_len;
			da_ref(pp->pk_buf);
			if (fp->fr_link == mp->m_frag)
				ff |= FFEOM;
			pp->pk_src = mp->m_src;
			pp->pk_dst = dst;
			pp->pk_flag = ff;
			ff = 0;
			if (mp->m_flag & MM_PRIO) {
				if (pvmdebmask & (PDMMESSAGE|PDMAPPL))
					pvmlogerror("sendmessage() PRIO message to host? (scrapped)\n");

			} else {
				pkt_to_host(hp, pp);
			}
		} while ((fp = fp->fr_link) != mp->m_frag);
	}

bail:
	pmsg_unref(mp);
	return 0;
}


/*	forkexec()
*
*	Search directories in epaths for given file.
*	Clean up any files we opened, fork and exec the named process.
*	Leave std{out,err} open so the process can whine if it needs to.
*
*	Returns 0 if ok (and fills in tpp), else returns PvmNoFile or
*	PvmOutOfRes
*
*	N.B. must be able to use argv[-1].
*/

#ifndef WIN32
/* too many ifdefs, WIN 32 gets its own */

#ifdef IMA_OS2
#include <process.h>
static int nextfakepid = 10000000;		/* XXX fix this */
int *ptr_nfp = &nextfakepid;
#endif

int
forkexec(flags, name, argv, nenv, env, inst, hosttotal, outof, tpp)
	int flags;				/* exec options */
	char *name;				/* filename */
	char **argv;			/* arg list (argv[-1] must be there) */
	int nenv;				/* num of envars */
	char **env;				/* envars */
	int inst;				/* this processes instance */
	int hosttotal;			/* how many on this host */
	int outof;				/* how many are being spawned across machine */
	struct task **tpp;		/* return task context */
{
	int tid;				/* task tid */
	int pid;				/* task pid */
	int pfd[2];				/* pipe back from task */
	struct task *tp;		/* new task context */
	char path[MAXPATHLEN];
	struct stat sb;
	char **ep, **eplist;
	int i;
	struct pmsg *mp;		/* message to tasker */
	struct waitc *wp;
	int ac;
	int realrunstate;
	char buf[32];
#ifdef IMA_BEOSCYLD
	int node;
#endif

	static char *nullep[] = { "", 0 };
#ifndef IMA_OS2
	static int nextfakepid = 10000000;		/* XXX fix this */
#endif

	if ((tid = tid_new()) < 0) {
		pvmlogerror("forkexec() out of tids?\n");
		return PvmOutOfRes;
	}
	if ((tp = task_new(tid)) == NULL) {
		pvmlogerror("forkexec() too many tasks?\n");
		return PvmOutOfRes;
	}

	/* search for file */

	eplist = CINDEX(name, '/') ? nullep : epaths;

	for (ep = eplist; *ep; ep++) {
		(void)strcpy(path, *ep);
		if (path[0])
			(void)strcat(path, "/");
		(void)strncat(path, name, sizeof(path) - strlen(path) - 1);
#ifdef IMA_OS2
		(void)strcat(path,".exe");	/* no *.cmd !!! */
#endif
#ifdef IMA_BEOSCYLD
		/* what node are we on? */
		node = bproc_currnode();
		/* only perform the stat check when we're
		 * running on the master; since the slave
		 * nodes typically don't contain binaries,
		 * the stat call is more than likely to fail
		 * anyway; down below we perform some bproc
		 * magic to "find" the binary back on the
		 * master when we're running on a slave
		 */
		if(node == BPROC_NODE_MASTER)
#endif
		if (stat(path, &sb) == -1
				|| ((sb.st_mode & S_IFMT) != S_IFREG)
				|| !(sb.st_mode & S_IEXEC)) {
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("forkexec() stat failed <%s>\n", path);
			}
			continue;
		}

		if (taskertid) {
			mp = mesg_new(0);
			mp->m_tag = SM_STTASK;
			mp->m_dst = taskertid;
			pkint(mp, tid);
			pkint(mp, flags);
			pkstr(mp, path);
			for (ac = 1; argv[ac]; ac++) ;
			pkint(mp, ac);
			pkstr(mp, path);
			for (i = 1; i < ac; i++)
				pkstr(mp, argv[i]);
			pkint(mp, nenv + 1);
			sprintf(buf, "PVMEPID=%d", nextfakepid);
			pkstr(mp, buf);
			task_setpid(tp, nextfakepid);
			if (++nextfakepid > 20000000)
				nextfakepid = 10000000;
			for (i = 0; i < nenv; i++)
				pkstr(mp, env[i]);
			pkint(mp, inst);
			pkint(mp, hosttotal);
			pkint(mp, outof);
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("forkexec() info:: inst %d host %d outof %d\n",
							inst, hosttotal, outof);
			}
			wp = wait_new(WT_TASKSTART);
			wp->wa_tid = tid;
			wp->wa_on = taskertid;
			mp->m_wid = wp->wa_wid;
			sendmessage(mp);
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("forkexec() sent tasker t%x pid %d\n",
						tp->t_tid, tp->t_pid);
			}

		} else {
#ifdef	IMA_TITN
			if (socketpair(AF_UNIX, SOCK_STREAM, 0, pfd) == -1) {
				pvmlogperror("forkexec() socketpair");
				task_free(tp);
				return PvmOutOfRes;
			}
#else
			if (pipe(pfd) == -1) {
				pvmlogperror("forkexec() pipe");
				task_free(tp);
				return PvmOutOfRes;
			}
#endif

	/*
	* switch runstate to is-task before forking to avoid race.
	* if we're killed as a task, we don't want to clean up pvmd stuff.
	*/
			realrunstate = runstate;
			runstate = PVMDISTASK;
#if defined(IMA_CSPP) && defined(BALANCED_SPAWN)
			pid = cnx_sc_fork(CNX_INHERIT_SC, (int) __get_node_id());
#else
#ifndef IMA_OS2
			pid = fork();
#else
			pid = os2_spawn( path, argv, nenv, env,
					( flags & PvmTaskDebug ) ? debugger : 0 );
#endif
#endif
			if (pid)
				runstate = realrunstate;

			if (!pid) {

	/* close any random fds */

				dup2(pfd[1], 1);
				dup2(1, 2);
				for (i = getdtablesize(); --i > 2; )
					(void)close(i);
	/*
	* set envars
	*/
				while (nenv-- > 0) {
					pvmputenv(env[nenv]);
/*
					pvmlogprintf("forkexec() putenv(%s)\n", env[nenv]);
*/
				}
	/*
	* put expected pid in environment for libpvm in case
	* the process we exec forks before connecting back to the pvmd
	*/
				sprintf(buf, "PVMEPID=%d", getpid());
				pvmputenv(buf);
				argv[0] = path;
				if (flags & PvmTaskDebug) {
					char *p;

					argv--;
					if (p = getenv("PVM_DEBUGGER"))
						argv[0] = p;
					else
						argv[0] = debugger;
					execv(argv[0], argv);

				} else {
#ifdef IMA_BEOSCYLD
				  /* if we're running on the master */
				  if(node == BPROC_NODE_MASTER) {
				    /* simply call execv */
				    execv(path,argv);
				  }
				  /* if we're running on a slave node */
				  else {
				    /* migrate process back to the master */
				    if(bproc_move(BPROC_NODE_MASTER) != -1) {
				      /* locate the executable */
				      for(ep = eplist; *ep; ep++) {
					strcpy(path,*ep);
					if(path[0]) strcat(path,"/");
					strncat(path,name,sizeof(path)-strlen(path)-1);
					if((stat(path,&sb) == -1) ||
					   ((sb.st_mode & S_IFMT) != S_IFREG) ||
					   !(sb.st_mode & S_IEXEC)) {
					  /* try the next path */
					  continue;
					}
					else {
					  /* execmove back to the slave */
					  bproc_execmove(node,path,argv,env);
					  /* if we get this far, the call to execmove
					   * failed and we now have a ghost job back
					   * on the slave and this whacked job on the
					   * master; if we exit, both jobs are cleaned
					   * up through the magic of bproc
					   */
					  exit(1);
					}
				      }
				      /* if we fell through the loop, we didn't
				       * find the executable file; in this case
				       * we move back to the slave and return
				       */
				      if(bproc_move(node) == -1) {
					/* if the move back to the slave fails,
					 * we have a ghost job back on the slave
					 * and this whacked job on the master;
					 * if we exit, both jobs are cleaned up
					 * through the magic of bproc
					 */
					exit(1);
				      }
				      else {
					/* since the move back to the slave was
					 * successfully and we were unable to find
					 * the executable back on the master, we
					 * mimic forkexec's normal functionality
					 * of ending the for-loop
					 */
					break;
				      }
				    }
				    else if (pvmdebmask & PDMTASK) {
				      pvmlogprintf(
							"forkexec() bproc move to master failed\n");
				    }
				    /* end-if moving to master */
				  }
				  /* end-if running on a slave */
#else
					execv(path, argv);
#endif
				}
				exit(1);
			}
			if (pid == -1) {
				pvmlogperror("forkexec() fork");
				(void)close(pfd[0]);
				(void)close(pfd[1]);
				task_free(tp);
				return PvmOutOfRes;
			}
			(void)close(pfd[1]);

			task_setpid(tp, pid);
			tp->t_out = pfd[0];
			tp->t_flag |= TF_FORKD;

			wrk_fds_add(tp->t_out, 1);
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("forkexec() new task t%x pid %d pfd=%d\n",
						tp->t_tid, tp->t_pid, tp->t_out);
			}
		}

		tp->t_a_out = STRALLOC(name);
		*tpp = tp;
		return 0;
	}
	if (pvmdebmask & PDMTASK) {
		pvmlogprintf("forkexec() didn't find <%s>\n", name);
	}
	task_free(tp);
	return PvmNoFile;
}

#else
#ifndef IMA_WIN32_WATCOM
extern char **environ;
#endif
static int nextfakepid = 10000000;		/* XXX fix this */
int *ptr_nfp = &nextfakepid;

int
forkexec(flags, name, argv, nenv, env, inst, hosttotal, outof, tpp)
	int flags;				/* exec options */
	char *name;				/* filename */
	char **argv;			/* arg list (argv[-1] must be there) */
	int nenv;				/* num of envars */
	char **env;				/* envars */
	int inst;				/* this processes instance */
	int hosttotal;			/* how many on this host */
	int outof;				/* how many are being spawned elsewhere */
	struct task **tpp;		/* return task context */
{
	int tid;				/* task tid */
	int pid=-1;				/* task pid */

	struct task *tp;		/* new task context */
	char *path;
	struct stat sb;
	char **ep, **eplist;
	int i;
	struct mesg *mp;		/* message to tasker */
	struct waitc *wp;
	int ac;
	char *expected_pid=0;
	char buf[32];
	HANDLE hpid;
	const char *penv=0;
	char filename[128];

	SECURITY_ATTRIBUTES saPipe;
  	int fSuccess;
  
  	HANDLE hChildStdinRd, hChildStdinWr, hChildStdinWrDup, 
     	hChildStdoutRd, hChildStdoutWr, hChildStdoutRdDup, 
     	hInputFile, hSaveStdin, hSaveStdout; 
   
  	SECURITY_DESCRIPTOR SecDescript;
  	SECURITY_ATTRIBUTES saAttr; 
  
  
	PROCESS_INFORMATION pi;
	STARTUPINFO si;  /* for CreateProcess call */
	char fixedargv[256];

	static char *nullep[] = { "", 0 };

	path = (char *) malloc (128 * sizeof(char));

	if ((tid = tid_new()) < 0) {
		pvmlogerror("forkexec() out of tids?\n");
		return PvmOutOfRes;
	}
	if ((tp = task_new(tid)) == NULL) {
		pvmlogerror("forkexec() too many tasks?\n");
		return PvmOutOfRes;
	}
  	/* PIPE SECURITY STUFF */
  
  	   InitializeSecurityDescriptor(&SecDescript,SECURITY_DESCRIPTOR_REVISION);
  	   SetSecurityDescriptorDacl(&SecDescript,TRUE,NULL,FALSE);
  
  	  /* Set the bInheritHandle flag so pipe handles are inherited.  */
   
  	   saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    	   saAttr.bInheritHandle = TRUE; 
  	   saAttr.lpSecurityDescriptor = &SecDescript; 
   
	  /*
  	  The steps for redirecting child process's STDOUT: 
   	       1. Save current STDOUT, to be restored later. 
   	       2. Create anonymous pipe to be STDOUT for child process. 
   	       3. Set STDOUT of the parent process to be write handle of 
  	          the pipe, so it is inherited by the child process. 
  	       4. Create a noninheritable duplicate of the read handle and
  	          close the inheritable read handle. 
	  */
   
  	  /* Save the handle to the current STDOUT.  */
   
  	  hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
   	  if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) 
        		pvmlogprintf("Stdout pipe creation failed\n"); 
   
  	/* Set a write handle to the pipe to be STDOUT.  */
   
  	   if (! SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr)) 
        		pvmlogprintf("Redirecting STDOUT failed"); 
   
  	/* Create noninheritable read handle and close the inheritable */
	/* read handle.  */
  
  	    fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
          	GetCurrentProcess(), &hChildStdoutRdDup , 0,
          	FALSE,
  	        DUPLICATE_SAME_ACCESS);
  	    if( !fSuccess )
          	pvmlogprintf("DuplicateHandle failed");
  	    CloseHandle(hChildStdoutRd);
  
	/*
	The steps for redirecting child process's STDIN: 
	     1.  Save current STDIN, to be restored later. 
	     2.  Create anonymous pipe to be STDIN for child process. 
	     3.  Set STDIN of the parent to be the read handle of the 
	         pipe, so it is inherited by the child process. 
	     4.  Create a noninheritable duplicate of the write handle, 
	         and close the inheritable write handle. 
	*/
   
  	/* Save the handle to the current STDIN.  */
   
  	   hSaveStdin = GetStdHandle(STD_INPUT_HANDLE); 
   
  	/* Create a pipe for the child process's STDIN.  */
   
  	   if (! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) 
        		pvmlogprintf("Stdin pipe creation failed\n"); 
   
  	/* Set a read handle to the pipe to be STDIN.  */
   
  	   if (! SetStdHandle(STD_INPUT_HANDLE, hChildStdinRd)) 
        		pvmlogprintf("Redirecting Stdin failed"); 
   
  	/* Duplicate the write handle to the pipe so it is not inherited. */
   
  	   fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdinWr, 
        		GetCurrentProcess(), &hChildStdinWrDup, 0, 
  	      	FALSE,                  /* not inherited */
        		DUPLICATE_SAME_ACCESS); 
  	   if (! fSuccess) 
        		pvmlogprintf("DuplicateHandle failed"); 
   
  	   CloseHandle(hChildStdinWr); 
   
  
	/* search for file */

	strcpy(filename,name); 		/* store the filename */

	eplist = CINDEX(filename, '/') ? nullep : epaths;
	make_valid(filename);

	for (ep = eplist; *ep; ep++) {

		(void)strcpy(path, *ep);
		if (path[0])
			(void)strcat(path, "/");

		(void)strncat(path, filename, /* Yuk */ 128 - strlen(path) - 1);

		if (stat(path, &sb) == -1
				|| ((sb.st_mode & S_IFMT) != S_IFREG)
				|| !(sb.st_mode & S_IEXEC)) {
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("forkexec() stat failed <%s>\n", path);
			}
			continue;
	    }

	/* have found the path with executable */

		argv[0] = path;

		expected_pid=malloc(64 * sizeof(char));
		sprintf(expected_pid, "PVMEPID=%d", nextfakepid);

		penv=(const char*) expected_pid;
		if (_putenv(penv)) {
			pvmlogerror("putenv failed !\n");
			exit(1);
		}

		/* concatenate the argv together, glue it */

		if (argv[0]) {
			strcpy(fixedargv,argv[0]);
			strcat(fixedargv," ");
			for (i=1; argv[i];i++) {
				strcat(fixedargv,argv[i]);
				strcat(fixedargv," ");
			}
			fixedargv[strlen(fixedargv)-1]=0;
		}

		if (flags & PvmTaskDebug) {
			char *pdebug;

				if (pdebug = getenv("PVM_DEBUGGER"))
					argv[0] = pdebug;

					saPipe.nLength = sizeof(SECURITY_ATTRIBUTES);
					saPipe.lpSecurityDescriptor = NULL;
					saPipe.bInheritHandle = FALSE;

					memset(&si, 0, sizeof(si));
					si.cb = sizeof(si);
					pid = CreateProcess(
							argv[0],	/* filename */
							fixedargv,	/* command line for child */
							NULL,	/* process security descriptor */
							NULL,	/* thread security descriptor */
							FALSE,	/* inherit handles? */
							DEBUG_PROCESS,	/* creation flags */
							NULL,	/* inherited environment address */
							NULL,	/* startup dir */
									/* NULL = start in current */
							&si,	/* ptr to startup info (input) */
							&pi);	/* ptr to process info (output) */
		}
		else {
           saPipe.nLength = sizeof(SECURITY_ATTRIBUTES);
           saPipe.lpSecurityDescriptor = NULL;
           saPipe.bInheritHandle = FALSE;

           memset(&si, 0, sizeof(si));
           si.cb = sizeof(si);
           pid = CreateProcess(
					argv[0],		/* filename */
					fixedargv,		/* command line for child */
					NULL,			/* process security descriptor */
					NULL,			/* thread security descriptor */
					FALSE,			/* inherit handles? */
					DETACHED_PROCESS,	/* creation flags */
					NULL,			/* inherited environment address */
					NULL,			/* startup dir */
									/* NULL = start in current */
					&si,			/* ptr to startup info (input) */
					&pi);			/* ptr to process info (output) */
		}

		if (pid == -1) {
			pvmlogperror("forkexec() _spawnve");
			task_free(tp);
			return PvmOutOfRes;
		}
		CloseHandle(pi.hThread);
		task_sethandle(tp,pi.hProcess);

		task_setpid(tp,nextfakepid);
		nextfakepid++;

		tp->t_flag |= TF_FORKD;

		if (pvmdebmask  & PDMTASK) {
			pvmlogprintf("forkexec() new task t%x pid %d pfd=%d\n",
					tp->t_tid, tp->t_pid, tp->t_out);
		}

	    tp->t_a_out = STRALLOC(filename);
	    *tpp = tp;

    	    return 0;
	}	 /* for */

	if (pvmdebmask  & PDMTASK) {
		pvmlogprintf("forkexec() didn't find <%s>\n", filename);
	}
	task_free(tp);
	return PvmNoFile;
}

/* convenience for pvm programs, just program your executable without */
/* any .exe appendix. added on WIN32 machines automatically. make it */
/* valid */

void make_valid(n)
	char *n;
{
	char *appendix=".exe";
	int lenapp=0;
	char *position=0;
	int i=strlen(n)-1;
	int j;
	lenapp=	strlen(appendix)-1;

	if ((i= i- lenapp) > 0) {

		position=n;
		for (j=0;j<i;j++) position++;

		/* have to append appendix */
		if (strncmp(position,appendix,lenapp))
			strcat(n,appendix);
	} else strcat(n,appendix);
}

#endif


/*	beprime()
*
*	Pvmd[master] becomes pvmd'[master].
*	Set runstate, make ppnetsock the real netsock, close loclsock.
*/

beprime()
{
	struct htab *htp;
	struct task *tp;
	int i;

	runstate = PVMDPRIME;

	if ((pvmmyupid = getpid()) == -1) {
		pvmlogerror("beprime() can't getpid()\n");
		pvmbailout(0);
	}

	myhostpart = 0;
	pvmmytid = TIDPVMD;

	htp = ht_new(hosts->ht_local);
	htp->ht_master = hosts->ht_local;
	htp->ht_local = 0;
	ht_insert(htp, hosts->ht_hosts[hosts->ht_local]);
	ht_insert(htp, hosts->ht_hosts[0]);
	htp->ht_hosts[htp->ht_master]->hd_txseq
			= htp->ht_hosts[0]->hd_rxseq;
	htp->ht_hosts[htp->ht_master]->hd_rxseq
			= htp->ht_hosts[0]->hd_txseq;

	oldhosts = hosts;
	hosts = htp;

#ifndef NOUNIXDOM
	loclspath = 0;
#endif
	(void)close(loclsock);
	loclsock = -1;
	loclsnam = 0;
	(void)close(netsock);
	netsock = ppnetsock;
	ppnetsock = -1;

	locltasks = 0;
	task_init();

	/* close everything but netsock, log_fd and 0, 1, 2 */

	for (i = getdtablesize(); --i > 2; )
		if (i != netsock && i != log_fd)
			(void)close(i);

	wrk_fds_init();
	wrk_fds_add(netsock, 1);

	opq = pk_new(0);
	opq->pk_tlink = opq->pk_trlink = opq;

	wdead = 0;
	rdead = 0;

	return 0;
}


/*	pkt_to_host()
*
*	Add data pkt to send queue (txq) for a host.  Consume the pkt.
*	If data plus header length is greater than host mtu,
*	refragment into >1 pkts.
*
*	We have to pay special attention to the FFSOM packet - make it
*	shorter so there's room to prepend the message header later.
*
*	If send window to host has room, push packet to opq.
*/

int
pkt_to_host(hp, pp)
	struct hostd *hp;
	struct pkt *pp;
{
	int maxl = (hp->hd_mtu < pvmudpmtu ? hp->hd_mtu : pvmudpmtu) - DDFRAGHDR;
	int llim = pp->pk_flag & FFSOM ? maxl - MSGHDRLEN : maxl;

	pp->pk_flag = (pp->pk_flag & (FFSOM|FFEOM)) | FFDAT;
	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("pkt_to_host() pkt src t%x dst t%x f %s len %d\n",
				pp->pk_src, pp->pk_dst, pkt_flags(pp->pk_flag), pp->pk_len);
	}

	if (pp->pk_len <= llim) {
		LISTPUTBEFORE(hp->hd_txq, pp, pk_link, pk_rlink);

	} else {
		struct pkt *pp2;
		char *cp = pp->pk_dat;
		int togo;
		int n;
		int ff = pp->pk_flag & FFSOM;
		int fe = pp->pk_flag & FFEOM;

		for (togo = pp->pk_len; togo > 0; togo -= n) {
			n = min(togo, llim);
			if ((pvmdebmask & PDMPACKET) && togo != pp->pk_len) {
				pvmlogprintf("pkt_to_host() refrag len %d\n", n);
			}
#ifdef	STATISTICS
			stats.refrag++;
#endif
			pp2 = pk_new(0);
			pp2->pk_src = pp->pk_src;
			pp2->pk_dst = pp->pk_dst;
			if (n == togo)
				ff |= fe;
			pp2->pk_flag = ff | FFDAT;
			ff = 0;
			llim = maxl;
			pp2->pk_enc = pp->pk_enc;
			pp2->pk_tag = pp->pk_tag;
			pp2->pk_ctx = pp->pk_ctx;
			pp2->pk_wid = pp->pk_wid;
			pp2->pk_crc = pp->pk_crc;
			pp2->pk_buf = pp->pk_buf;
			pp2->pk_max = pp->pk_max;
			pp2->pk_dat = cp;
			pp2->pk_len = n;
			da_ref(pp->pk_buf);
			cp += n;
			LISTPUTBEFORE(hp->hd_txq, pp2, pk_link, pk_rlink);
		}
		pk_free(pp);
	}

	while (hp->hd_nop < nopax
	&& (hp->hd_txq->pk_link != hp->hd_txq)) {
		if (pvmdebmask & PDMPACKET) {
			pvmlogprintf("pkt_to_host() pkt to opq\n");
		}
		pp = hp->hd_txq->pk_link;
		LISTDELETE(pp, pk_link, pk_rlink);
		TVCLEAR(&pp->pk_rtv);
		TVXADDY(&pp->pk_rta, &hp->hd_rtt, &hp->hd_rtt);
		TVCLEAR(&pp->pk_rto);
		TVCLEAR(&pp->pk_at);
		pp->pk_nrt = 0;
		pp->pk_hostd = hp;
		pp->pk_seq = hp->hd_txseq;
		hp->hd_txseq = NEXTSEQNUM(hp->hd_txseq);
		pp->pk_ack = 0;
		LISTPUTBEFORE(hp->hd_opq, pp, pk_link, pk_rlink);
		hp->hd_nop++;
		LISTPUTBEFORE(opq, pp, pk_tlink, pk_trlink);
	}
	return 0;
}


int
fin_to_host(hp)
	struct hostd *hp;
{
	struct pkt *pp;

	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("fin_to_host() %s\n", hp->hd_name);
	}
	pp = pk_new(DDFRAGHDR);	/* XXX could reref a dummy databuf here */
	pp->pk_dat += DDFRAGHDR;
	pp->pk_dst = hp->hd_hostpart | TIDPVMD;
	pp->pk_src = pvmmytid;
	pp->pk_flag = FFFIN;
	TVCLEAR(&pp->pk_rtv);
	TVXADDY(&pp->pk_rta, &hp->hd_rtt, &hp->hd_rtt);
	TVCLEAR(&pp->pk_rto);
	TVCLEAR(&pp->pk_at);
	pp->pk_nrt = 0;
	pp->pk_hostd = hp;
	pp->pk_seq = hp->hd_txseq;
	hp->hd_txseq = NEXTSEQNUM(hp->hd_txseq);
	pp->pk_ack = 0;
	LISTPUTBEFORE(hp->hd_opq, pp, pk_link, pk_rlink);
	hp->hd_nop++;
	LISTPUTAFTER(opq, pp, pk_tlink, pk_trlink);
	return 0;
}


int
finack_to_host(hp)
	struct hostd *hp;
{
	struct pkt *pp;

	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("finack_to_host() %s\n", hp->hd_name);
	}
	pp = pk_new(DDFRAGHDR);	/* XXX could reref a dummy databuf here */
	pp->pk_dat += DDFRAGHDR;
	pp->pk_dst = hp->hd_hostpart | TIDPVMD;
	pp->pk_src = pvmmytid;
	pp->pk_flag = FFFIN|FFACK;
	TVCLEAR(&pp->pk_rtv);
	TVCLEAR(&pp->pk_rta);
	TVCLEAR(&pp->pk_rto);
	TVCLEAR(&pp->pk_at);
	pp->pk_nrt = 0;
	pp->pk_hostd = hp;
	pp->pk_seq = 0;
	pp->pk_ack = 0;
	LISTPUTAFTER(opq, pp, pk_tlink, pk_trlink);
	return 0;
}


/*	pkt_to_task()
*
*	Add data pkt to send queue (txq) for a task.  Consume the pkt.
*	If data plus header length is greater than task mtu,
*	refragment into >1 pkts.
*/

int
pkt_to_task(tp, pp)
	struct task *tp;
	struct pkt *pp;
{
	if (tp->t_sock >= 0 && (tp->t_flag & TF_CONN)
#ifdef SHMEM
		&& !(tp -> t_flag & TF_SHM) /* don't add socket if a shm task */
#endif
	)
		wrk_fds_add(tp->t_sock, 2);

#if defined(IMA_PGON) || defined(IMA_I860)
	if (TIDISNODE(pp->pk_dst))
		mpp_output(tp, pp);
	else
#endif
#ifdef SHMEM
	if (((tp->t_sock < 0) && (tp->t_flag & TF_SHMCONN))
		|| (tp->t_flag & TF_SHMCONN) )
		mpp_output(tp, pp);
	else
#endif

#ifdef LocalRefragmentTest
	if (pp->pk_len + DDFRAGHDR <= pvmudpmtu) {
		LISTPUTBEFORE(tp->t_txq, pp, pk_link, pk_rlink);

	} else {
		struct pkt *pp2;
		int maxl = pvmudpmtu - DDFRAGHDR;
		char *cp = pp->pk_dat;
		int togo;
		int n;
		int ff = pp->pk_flag & FFSOM;
		int fe = pp->pk_flag & FFEOM;

		for (togo = pp->pk_len; togo > 0; togo -= n) {
			n = min(togo, maxl);
			pvmlogprintf("pkt_to_task() refrag len %d\n", n);
			pp2 = pk_new(0);
			pp2->pk_src = pp->pk_src;
			pp2->pk_dst = pp->pk_dst;
			if (n == togo)
				ff |= fe;
			pp2->pk_flag = ff | FFDAT;
			ff = 0;
			pp2->pk_enc = pp->pk_enc;
			pp2->pk_tag = pp->pk_tag;
			pp2->pk_ctx = pp->pk_ctx;
			pp2->pk_wid = pp->pk_wid;
			pp2->pk_crc = pp->pk_crc;
			pp2->pk_buf = pp->pk_buf;
			pp2->pk_max = pp->pk_max;
			pp2->pk_dat = cp;
			pp2->pk_len = n;
			da_ref(pp->pk_buf);
			cp += n;
			LISTPUTBEFORE(tp->t_txq, pp2, pk_link, pk_rlink);
		}
		pk_free(pp);
	}
#else /*LocalRefragmentTest*/
	{
	if (pvmdebmask & PDMMESSAGE)
		pvmlogprintf("pkt_to_task: queueing %x \n", pp->pk_dst);
	LISTPUTBEFORE(tp->t_txq, pp, pk_link, pk_rlink);
	}
#endif /*LocalRefragmentTest*/

	return 0;
}


#ifdef	STATISTICS
dump_statistics()
{
	pvmlogprintf(" select: rdy %d, zero %d, neg %d\n",
			stats.selrdy, stats.selzer, stats.selneg);
	pvmlogprintf(" sendto: ok %d, neg %d  recvfrom: ok %d\n",
			stats.sdok, stats.sdneg, stats.rfok);
	pvmlogprintf(" read: pos %d, zero %d, neg %d\n",
			stats.rdok, stats.rdzer, stats.rdneg);
	pvmlogprintf(" write: ok %d, short %d, zero %d, neg %d\n",
			stats.wrok, stats.wrshr, stats.wrzer, stats.wrneg);
	pvmlogprintf(" refrags: %d\n", stats.refrag);
	pvmlogprintf(" netwk resends: %d\n", stats.netret);
	return 0;
}


reset_statistics()
{
	BZERO((char*)&stats, sizeof(stats));
	return 0;
}
#endif	/*STATISTICS*/


#if defined(IMA_CSPP) && defined(BALANCED_SPAWN)
static int number_nodes = -1;
static int number_cpus = -1;
static cnx_scid_t scid_num;

static int
__get_node_id()
{
	static int current_node = 0;
	static int current_cpu = 0;

	if (number_nodes == -1) {
		number_nodes = get_number_nodes();
		number_cpus = get_number_cpus(current_node);
		goto done;
	}

	if (number_nodes == 1) {
		goto done;
	}

	if (current_cpu < (number_cpus - 1) ) {
		current_cpu++;

	} else {
		current_cpu = 0;
		if (current_node < (number_nodes - 1)) {
			current_node++;
			number_cpus = get_number_cpus(current_node);
		} else {
			current_node = 0;
			number_cpus = get_number_cpus(current_node);
			if (pvmdebmask & PDMTASK) {
				pvmlogerror (
					"Warning:pvm_spawn restarting process placement on Node 0");
			}
		}
	}
done:
	return current_node;
}


static cnx_is_scnode_basic_info_data_t sc_info[CNX_MAX_NODES];

static int
get_number_nodes()
{
	cnx_is_target_data_t target;
	int ret;
	cnx_pattributes_t pattr;
	int val;
	char errortxt[128];

	cnx_getpattr(getpid(), CNX_PATTR_SCID, &pattr);
	scid_num = pattr.pattr_scid;
	cnx_sysinfo_target_scnode(&target, scid_num, CNX_IS_ALL_NODES);
	ret = cnx_sysinfo(
		CNX_IS_SCNODE_BASIC_INFO,
		(void *) &target,
		sc_info,
		CNX_MAX_NODES,
		CNX_IS_SCNODE_BASIC_INFO_COUNT,
		(unsigned *) &val);

	if (ret == -1) {
		sprintf(errortxt,
				"Error calling cnx_sysinfo in %s:line %d errno: %d \n",
				__FILE__, __LINE__, errno);
		pvmlogerror(errortxt);
		exit (-1);
	}
	return val;
}

static int
get_number_cpus(int current_node)
{
	return sc_info[current_node].num_cpus;
}

#endif /*defined(IMA_CSPP) && defined(BALANCED_SPAWN)*/


/*	mksocs()
*
*	Make UDP sockets netsock and ppnetsock.  Make TCP master socket
*	loclsock.
*
*	Returns 0 if ok,
*	else 2 if pvmd already running,
*	else 1.
*/

int
mksocs()
{
	struct hostd *hp = hosts->ht_hosts[hosts->ht_local];
	struct hostd *hp0 = hosts->ht_hosts[0];
	struct sockaddr_in sin;
	char buf[128];
	char *sfn;
#ifndef WIN32
	int d;
#else
	HANDLE d;
	int e=0;
#endif
#ifndef NOSOCKOPT
	int bsz;
#endif
	char *p;
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
		|| defined(IMA_AIX56K64) || defined(IMA_LINUXALPHA)
	unsigned int oslen;
#else
	size_t oslen;
#endif
#else
	int oslen;
#endif
	int cc;
#ifndef NOUNIXDOM
	char spath[PVMTMPNAMLEN];	/* local socket path */
	struct sockaddr_un uns;
#endif
#ifdef IMA_BEOLIN
	struct hostent *hostaddr;
#endif
	short tmpp;

	/*
	* make pvmd-pvmd socket
	*/

	if ((netsock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		pvmlogperror("mksocs() socket netsock");
		return 1;
	}

	p = getenv("PVMNETSOCKPORT");

	tmpp = (p) ? atoi(p) : 0;

	hp->hd_sad.sin_port = htons(tmpp);

	do
	{
		oslen = sizeof(hp->hd_sad);
		if ((cc=bind(netsock, (struct sockaddr*)&hp->hd_sad, oslen))
				== -1)
		{
			if (p) {
				tmpp++;
				hp->hd_sad.sin_port = htons(tmpp);
			} else {
				pvmlogperror("mksocs() bind netsock");
				return 1;
			}
		}
	}
	while ( cc == -1 && p && hp->hd_sad.sin_port < 65535 );

	if ( cc == -1 ) {
		pvmlogperror("mksocs() bind netsock");
		return 1;
	}

	oslen = sizeof(hp->hd_sad);
	if (getsockname(netsock, (struct sockaddr*)&hp->hd_sad, &oslen) == -1) {
		pvmlogperror("mksocs() getsockname netsock");
		return 1;
	}

	/*
	* make pvmd-pvmd' socket
	*/

	if ((ppnetsock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		pvmlogperror("mksocs() socket ppnetsock");
		return 1;
	}

	hp0->hd_sad.sin_port = 0;
	oslen = sizeof(hp0->hd_sad);
	if (bind(ppnetsock, (struct sockaddr*)&hp0->hd_sad, oslen) == -1) {
		pvmlogperror("mksocs() bind ppnetsock");
		return 1;
	}
	oslen = sizeof(hp0->hd_sad);
	if (getsockname(ppnetsock, (struct sockaddr*)&hp0->hd_sad, &oslen) == -1) {
		pvmlogperror("mksocs() getsockname ppnetsock");
		return 1;
	}

	/*
	* make pvmd-local task socket
	*/

#ifdef NOUNIXDOM
	if ((loclsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		pvmlogperror("mksocs() socket loclsock");
		return 1;
	}

	/*
	* first try localhost address (loopback) then regular address
	* XXX 127.0.0.1 is a hack, we should really gethostbyaddr()
	*/

	BZERO((char*)&sin, sizeof(sin));
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	sin = hp->hd_sad;		/* allow task to connect from a node */
#else

#ifdef IMA_BEOLIN
	/* allow connection from another node */
	sin.sin_family = AF_INET;
#if 0
	if (gethostname(buf, sizeof(buf)-1) == -1) {
		pvmlogerror("mksocs() can't gethostname()\n");
		return 1;
	} else {
		/* got name, now get addr */
		if (!(hostaddr = gethostbyname( buf ))) {
			pvmlogprintf( "mksocs() can't gethostbyname() for %s\n", buf );
			return 1;
		} else {
			/* got addr, now save it */
			BCOPY( hostaddr->h_addr_list[0], (char*)&sin.sin_addr,
					sizeof(struct in_addr));
		}
	}
#else
		/* got name, now get addr */
		if (!(hostaddr = gethostbyname( hp->hd_name ))) {
			pvmlogprintf( "mksocs() can't gethostbyname() for %s\n",
				hp->hd_name );
			return 1;
		} else {
			/* got addr, now save it */
			BCOPY( hostaddr->h_addr_list[0], (char*)&sin.sin_addr,
					sizeof(struct in_addr));
		}
#endif
	sin.sin_port = 0;
#else
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(0x7f000001);
	sin.sin_port = 0;
#endif

#endif

	oslen = sizeof(sin);
	if (bind(loclsock, (struct sockaddr*)&sin, oslen) == -1) {
		sin = hp->hd_sad;
		oslen = sizeof(sin);
		if (bind(loclsock, (struct sockaddr*)&sin, oslen) == -1) {
			pvmlogperror("mksocs() bind loclsock");
			return 1;
		}
	}
	oslen = sizeof(sin);
	if (getsockname(loclsock, (struct sockaddr*)&sin, &oslen) == -1) {
		pvmlogperror("mksocs() getsockname loclsock");
		return 1;
	}

	if (listen(loclsock, SOMAXCONN) == -1) {
		pvmlogperror("mksocs() listen loclsock");
		return 1;
	}

#ifndef NOSOCKOPT
	bsz = pvmudpmtu * 2;
	if (setsockopt(netsock, SOL_SOCKET, SO_SNDBUF,
			(char*)&bsz, sizeof(bsz)) == -1
	|| setsockopt(netsock, SOL_SOCKET, SO_RCVBUF,
			(char*)&bsz, sizeof(bsz)) == -1
	|| setsockopt(ppnetsock, SOL_SOCKET, SO_SNDBUF,
			(char*)&bsz, sizeof(bsz)) == -1
	|| setsockopt(ppnetsock, SOL_SOCKET, SO_RCVBUF,
			(char*)&bsz, sizeof(bsz)) == -1) {
		pvmlogperror("mksocs() setsockopt");
		return 1;
	}
#endif /*NOSOCKOPT*/

	p = inadport_hex(&sin);

#else /*NOUNIXDOM*/
	if ((loclsock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		pvmlogperror("mksocs() socket loclsock");
		return 1;
	}

	BZERO((char*)&uns, sizeof(uns));
	uns.sun_family = AF_UNIX;
	spath[0] = 0;
	(void)PVMTMPNAMFUN(spath);
	strcpy(uns.sun_path, spath);
/*
XXX len?
*/

	oslen = sizeof(uns);
	if (bind(loclsock, (struct sockaddr*)&uns, oslen) == -1) {
		pvmlogperror("mksocs() bind loclsock");
		return 1;
	}

	if (listen(loclsock, SOMAXCONN) == -1) {
		pvmlogperror("mksocs() listen loclsock");
		return 1;
	}

	loclspath = STRALLOC(spath);
	p = spath;

#endif /*NOUNIXDOM*/

	/*
	* make pvmd-local task socket address file
	*/

	if (!(sfn = pvmdsockfile())) {
		pvmlogerror("mksocs() pvmdsockfile() failed\n");
		pvmbailout(0);
	}

#ifndef WIN32
	if ((d = open(sfn, O_CREAT|O_EXCL|O_WRONLY|O_TRUNC, 0600)) == -1) {
		if (errno == EEXIST) {
#else
	d = win32_create_file(sfn, CREATE_NEW);
	if ((int) d == -2){
		/* this code is for WIN95 */
		system_loser_win = TRUE;
		e = _open(sfn,O_CREAT|O_EXCL|O_WRONLY|O_TRUNC, 0600);
	}
	if (((int)d == -1) || (e ==-1)) {
		if (1) {  /* errno == EEXIST */
#endif
#ifndef	OVERLOADHOST
#ifdef WIN32
			if (d == INVALID_HANDLE_VALUE) {
				(void) pvmlogprintf("mksocs() %s failed. You are required to run on NTFS\n", sfn);
				return 1; /* same as write address */
			} else
#endif
			(void)pvmlogprintf(
					"mksocs() %s exists.  pvmd already running?\n", sfn);
			return 2;
#endif

		} else {
			pvmlogperror(sfn);
			pvmlogerror("mksocs() can't write address file\n");
			return 1;
		}
	} else {
#ifndef WIN32
		cc = write(d, p, strlen(p));
#else
		CloseHandle(d);
		if (!system_loser_win) {
			if ((d = (HANDLE) win32_open_file(sfn)) == (HANDLE) -1) {
				pvmlogprintf("could not open file %s: %d \n",
						sfn,GetLastError());
			return 2;
			}
			cc = win32_write_file(d,p,strlen(p));
		} else
			/* the win95 case */
			cc = write(e,p,strlen(p));
#endif
		if (cc != strlen(p)) {
			if (cc == -1) {
				pvmlogperror(sfn);
				pvmlogerror("mksocs() can't write address file\n");

			} else {
				(void)pvmlogprintf(
						"mksocs() aargh, short write on %s: %d\n",
						sfn, cc);
				pvmlogerror("mksocs() is the partition full?\n");
			}
#ifndef WIN32
			(void)close(d);
#else
			if (d)
				CloseHandle(d);
			if (e)
				close(e);
#endif
			(void)unlink(sfn);
			return 1;
		}
		loclsnam = STRALLOC(sfn);
#ifndef WIN32
		(void)close(d);
#else
		if (d)
			win32_close_file(d);
		if (e)
			close(e);
#endif
	}

	/* set PVMSOCK envar */

	sprintf(buf, "PVMSOCK=%s", p);
	p = STRALLOC(buf);
	pvmputenv(p);

	return 0;
}


/*	colonsep()
*
*	Break string into substrings on ':' delimiter.
*	Return null-terminated array of strings, in new malloc'd space.
*	Modifies the original string.
*/

char **
colonsep(s)
	char *s;	/* the string to break up */
{
	char **els;
	int nel = 2;			/* length of els */
	char *p, *q;

#if defined (IMA_OS2) || defined (WIN32)
	for (p = s; p = CINDEX(p, ';'); p++)
#else
	for (p = s; p = CINDEX(p, ':'); p++)
#endif
		nel++;
	els = TALLOC(nel, char*, "path");

	nel = 0;
	for (p = s; p; p = q) {
#if defined (IMA_OS2) || defined (WIN32)
		if (q = CINDEX(p, ';'))
#else
		if (q = CINDEX(p, ':'))
#endif
			*q++ = 0;
		els[nel++] = p;
	}
	els[nel] = 0;
	return els;
}


/*	varsub()
*
*	Substitute environment variables into string.
*	Variables named by $NAME or ${NAME}.
*	Return string in new malloc'd space.
*/

char *
varsub(s)
	char *s;
{
	int rm = 8;		/* length of result string space */
	char *r;		/* result string */
	int rl = 0;
	char *p;
	char *vn, *vv;
	char c;
	int l;

	r = TALLOC(rm, char, "var");
	while (*s) {
		for (p = s; *p && *p != '$'; p++) ;
		if (l = p - s) {
			if (rl + l >= rm) {
				rm = rl + l + 1;
				r = TREALLOC(r, rm, char);
			}
			strncpy(r + rl, s, l);
			rl += l;
		}
		s = p++;
		if (*s == '$') {
			if (*p == '{')
				p++;
			vn = p;
			while (isalnum(*p) || *p == '_')
				p++;
			c = *p;
			*p = 0;

			vv = getenv(vn);

#ifdef WIN32
			if (!vv)
				vv = (char *) read_pvmregistry(vn);
#endif

			*p = c;
			if (*p == '}')
				p++;
			if (vv)
				l = strlen(vv);
			else {
				vv = s;
				l = p - s;
			}
			if (l) {
				if (rl + l >= rm) {
					rm = rl + l + 1;
					r = TREALLOC(r, rm, char);
				}
				strncpy(r + rl, vv, l);
				rl += l;
			}
			s = p;
		}
	}
	r[rl] = 0;
	return r;
}


/*	crunchzap()
*
*	Parse a string into words delimited by <> pairs.
*	Max number of words is original value of *acp.
*
*	Trashes out the original string.
*	Returns 0 with av[0]..av[*acp - 1] pointing to the words.
*	Returns 1 if too many words.
*/

int
crunchzap(s, acp, av)
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
		while (*p && *p++ != '<');
		if (*p) {
			if (ac >= n) {
	/* command too long */
				*acp = ac;
				return 1;
			}
			av[ac++] = p;
			while (*p && *p != '>') p++;
			if (*p) *p++ = 0;
		}
	}
	*acp = ac;
	return 0;
}


/*	master_config()
*
*	Master pvmd.  Config a host table with length 1.
*/

int
master_config(hn, argc, argv)
	char *hn;			/* hostname or null */
	int argc;
	char **argv;
{
	struct hostent *he;
	struct hostd *hp;
	struct hostd *hp2;
	int i;
	char *s;

	if (argc > 2) {
		pvmlogerror("usage: pvmd3 [-ddebugmask] [-nhostname] [hostfile]\n");
		pvmbailout(0);
	}
	if (argc == 2) {
		filehosts = readhostfile(argv[1]);
	}
	else if (s = getenv("PVMHOSTFILE")) {
		filehosts = readhostfile(s);
	}
	if (pvmdebmask & PDMSTARTUP) {
		if (filehosts) {
			pvmlogerror("master_config() host file:\n");
			ht_dump(filehosts);

		} else
			pvmlogerror("master_config() null host file\n");
	}

	hosts = ht_new(1);
	hosts->ht_serial = 1;
	hosts->ht_master = 1;
	hosts->ht_cons = 1;
	hosts->ht_local = 1;

	hp = hd_new(1);
	hp->hd_name = STRALLOC(hn);
	hp->hd_arch = STRALLOC(myarchname);
	hp->hd_mtu = pvmudpmtu;
	hp->hd_dsig = pvmmydsig;
	ht_insert(hosts, hp);
	hd_unref(hp);

	hp = hd_new(0);
	hp->hd_name = STRALLOC("pvmd'");
	hp->hd_arch = STRALLOC(myarchname);
	hp->hd_mtu = pvmudpmtu;
	hp->hd_dsig = pvmmydsig;
	ht_insert(hosts, hp);
	hd_unref(hp);

	/*
	* get attributes from host file if available
	*/

	hp = hosts->ht_hosts[1];
	if (filehosts &&
			((hp2 = nametohost(filehosts, hp->hd_name))
			|| (hp2 = filehosts->ht_hosts[0]))) {
		applydefaults(hp, hp2);
	}

	if (!hp->hd_epath) {
		if ((s = getenv("PVM_PATH")))
			hp->hd_epath = STRALLOC(s);
		else
			hp->hd_epath = STRALLOC(DEFBINDIR);
	}
	epaths = colonsep(varsub(hp->hd_epath));
	if (!hp->hd_bpath)
		hp->hd_bpath = STRALLOC(DEFDEBUGGER);
	debugger = varsub(hp->hd_bpath);
	if (!hp->hd_wdir) {
		if ((s = getenv("PVM_WD")))
			hp->hd_wdir = STRALLOC(s);
		else
			hp->hd_wdir = STRALLOC(pvmgethome());
	}
	s = varsub(hp->hd_wdir);
	if (chdir(s) == -1)
		pvmlogperror(s);
	PVM_FREE(s);

	if (!(he = gethostbyname(hp->hd_aname ? hp->hd_aname : hp->hd_name))) {
		pvmlogprintf("master_config() %s: can't gethostbyname\n", hn);
		pvmbailout(0);
	}

	BCOPY(he->h_addr_list[0], (char*)&hp->hd_sad.sin_addr,
		sizeof(struct in_addr));

	hp = hosts->ht_hosts[0];
	BCOPY(he->h_addr_list[0], (char*)&hp->hd_sad.sin_addr,
		sizeof(struct in_addr));

	if (pvmdebmask & (PDMHOST|PDMSTARTUP)) {
		pvmlogerror("master_config() host table:\n");
		ht_dump(hosts);
	}

	if (mksocs())
		pvmbailout(0);


#ifndef WIN32
	/* close everything but our sockets */
	for (i = getdtablesize(); --i > 2; )
/* XXX don't like this - hard to maintain */
		if (i != netsock && i != ppnetsock && i != loclsock && i != log_fd)
			(void)close(i);

	/* reopen 0, 1, 2*/
	(void)open("/dev/null", O_RDONLY, 0);
	(void)open("/dev/null", O_WRONLY, 0);
	(void)dup2(1, 2);
#endif

	pvmsetlog(2);

	runstate = PVMDNORMAL;
	return 0;
}


/*	slave_config()
*
*	Slave pvmd being started by master.  Trade minimal config info
*	so we can send packets back and forth.
*/

int
slave_config(hn, argc, argv)
	char *hn;
	int argc;
	char **argv;
{
	int lh;			/* local host index */
	int mh;			/* master host index */
	struct hostd *hp;
	int i, j;
	int ac;
	int ms = 0;		/* manual (humanoid) startup */
#ifndef WIN32
	int dof = 1;		/* fork, exit parent (default) */
#else
	int dof = 0;
#endif
	int bad = 0;
	char *p;
	char *s;

	for (i = j = ac = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {

			case 'S':
				ms = 1;
				break;

			case 'f':
				dof = 0;
				break;

			default:
				pvmlogprintf("slave_config() unknown switch: %s\n", argv[i]);
				bad++;
			}

		} else {
			argv[j++] = argv[i];
			ac++;
		}
	}
	argc = ac;

	if (bad || argc != 6) {
		pvmlogerror("slave_config: bad args\n");
		pvmbailout(0);
	}

	mh = atoi(argv[1]);
	lh = atoi(argv[4]);
	hosts = ht_new(1);
	hosts->ht_serial = 1;
	hosts->ht_master = mh;
	hosts->ht_cons = mh;
	hosts->ht_local = lh;

	hp = hd_new(mh);
	hp->hd_name = STRALLOC("?");
	hex_inadport(argv[2], &hp->hd_sad);
	hp->hd_mtu = atoi(argv[3]);
	ht_insert(hosts, hp);
	hd_unref(hp);

	hp = hd_new(0);
	hp->hd_name = STRALLOC("pvmd'");
	hp->hd_arch = STRALLOC(myarchname);
	hp->hd_mtu = pvmudpmtu;
	hp->hd_dsig = pvmmydsig;
	hex_inadport(argv[5], &hp->hd_sad);
	ht_insert(hosts, hp);
	hd_unref(hp);

	hp = hd_new(lh);
	hp->hd_name = STRALLOC(hn);
	hp->hd_arch = STRALLOC(myarchname);
	hp->hd_mtu = pvmudpmtu;
	hp->hd_dsig = pvmmydsig;
	hex_inadport(argv[5], &hp->hd_sad);
	ht_insert(hosts, hp);
	hd_unref(hp);

	if (i = mksocs()) {
		if (i == 2) {
			printf("PvmDupHost\n");
			fflush(stdout);
		}
		pvmbailout(0);
	}

	printf("ddpro<%d> arch<%s> ip<%s> mtu<%d> dsig<%d>\n",
		DDPROTOCOL,
		myarchname,
		inadport_hex(&hp->hd_sad),
		pvmudpmtu,
		pvmmydsig);
	fflush(stdout);

#ifndef WIN32

#if !defined(IMA_OS2) && !defined(CYGWIN)
	if (!ms)
		(void)read(0, (char*)&i, 1);
#endif

	if (dof) {
		if (i = fork()) {
			if (i == -1)
				pvmlogperror("slave_config() fork");
			exit(0);
		}

	/* close everything but our sockets */

		for (i = getdtablesize(); --i >= 0; )
/* XXX don't like this - hard to maintain */
			if (i != netsock && i != loclsock && i != log_fd)
				(void)close(i);
	}

	/* reopen 0, 1, 2*/

	(void)open("/dev/null", O_RDONLY, 0);
	(void)open("/dev/null", O_WRONLY, 0);
	(void)dup2(1, 2);

#endif

	pvmsetlog(2);

	if ((p = getenv("PVM_PATH")))
		s = STRALLOC(p);
	else
		s = STRALLOC(DEFBINDIR);
	epaths = colonsep(varsub(s));
	PVM_FREE(s);

	s = STRALLOC(DEFDEBUGGER);
	debugger = varsub(s);
	PVM_FREE(s);

	if ((s = getenv("PVM_WD")))
		p = STRALLOC(s);
	else
		p = STRALLOC(pvmgethome());
	s = varsub(p);
	if (chdir(s) == -1)
		pvmlogperror(s);
	PVM_FREE(p);
	PVM_FREE(s);

	runstate = PVMDSTARTUP;

	return 0;
}



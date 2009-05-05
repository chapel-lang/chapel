
static char rcsid[] =
	"$Id: lpvmgen.c,v 1.101 2008/12/17 20:40:49 pvmsrc Exp $";

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
 *	lpvmgen.c
 *
 *	Libpvm generic functions.
 *
 * $Log: lpvmgen.c,v $
 * Revision 1.101  2008/12/17 20:40:49  pvmsrc
 * Added some extra debugging for diagnosing any future strerror() problems...
 * 	:-b
 * (Spanker=kohl)
 *
 * Revision 1.100  2007/07/09 19:59:54  pvmsrc
 * Added missing "char *pvmnametag();" function decl.
 * 	- submitted by David Mathog <mathog@caltech.edu>.
 * (Spanker=kohl)
 *
 * Revision 1.99  2004/02/17 18:01:29  pvmsrc
 * False alarm, removed debug output for AMD64.
 * (Spanker=kohl)
 *
 * Revision 1.98  2004/02/09 17:21:13  pvmsrc
 * Oops...  typo...
 * (Spanker=kohl)
 *
 * Revision 1.97  2004/02/09 17:17:01  pvmsrc
 * Added pvmlogperror debug output for AMD64...
 * 	- the arch that wouldn't die...  er, run without seg faulting...  :)
 * (Spanker=kohl)
 *
 * Revision 1.96  2004/02/03 21:40:57  pvmsrc
 * Added check for NULL strerror() return...
 * 	- to chase down AMD64 problem...
 * (Spanker=kohl)
 *
 * Revision 1.95  2004/01/14 19:12:34  pvmsrc
 * Added (char *) cast to strerror() assignment...
 * (Spanker=kohl)
 *
 * Revision 1.94  2004/01/14 18:50:56  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.93  2001/09/26 23:35:43  pvmsrc
 * Removed stuffing of PVM_VMID env var when spawning remote tasks.
 * 	- first, each host can have it's own local vmid (why not?),
 * 		so we shouldn't stomp that on the spawn.
 * 	- second, each local pvmd sets its PVM_VMID env var on startup
 * 		if the option is passed to it, so there's no need to
 * 		additionally set the vmid on the spawn.
 * (Spanker=kohl)
 *
 * Revision 1.92  2001/09/26 21:22:34  pvmsrc
 * Added Handling for Optional Virtual Machine ID.
 * 	- append PVM_VMID env var to spawn env for tasks (if set).
 * (Spanker=kohl)
 *
 * Revision 1.91  2001/06/28 16:45:29  pvmsrc
 * D-Oh!  Better set context around message handler functions.
 * 	- if message comes in with a particular context, reply should
 * 		probably be in same context!  :-]
 * (Spanker=kohl)
 *
 * Revision 1.90  2001/05/11 18:58:11  pvmsrc
 * Added use of new "USESTRERROR" define.
 * 	- uses strerror() function instead of sys_errlist/sys_nerr
 * 		(which aren't public globals on some new operating systems).
 * (Spanker=kohl)
 *
 * Revision 1.89  2001/02/07 23:14:06  pvmsrc
 * First Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.88  2000/06/15 17:51:50  pvmsrc
 * Fixed bug in WIN32 direct routing.
 * 	- stupid #endif in the wrong place, pvm_fd_add() call whacked.
 * 	- turned back on direct routing default and setopt.
 * (Spanker=kohl)
 *
 * Revision 1.87  2000/02/17 23:12:12  pvmsrc
 * *** Changes for new BEOLIN port ***
 * 	- MPP-like, similar to SP2, etc.
 * 	- submitted by Paul Springer <pls@smokeymt.jpl.nasa.gov>.
 * 	- format-checked & cleaned up by Jeembo...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.86  2000/02/16 21:59:43  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.85  1999/11/08 17:44:32  pvmsrc
 * SGI compiler cleanup.
 * (Spanker=kohl)
 *
 * Revision 1.84  1999/10/27 18:49:00  pvmsrc
 * Fixed (hopefully) the function header declaration for pvm_recvf().
 * 	- should work on all platforms now (using __ProtoGlarp__())...
 * (Spanker=kohl)
 *
 * Revision 1.83  1999/07/08 18:59:56  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.82  1999/03/12 20:55:33  pvmsrc
 * Don't allow direct routing in WIN32 until we fix it.  Shit.
 * (Spanker=kohl)
 * 
 * Revision 1.81  1999/01/29  17:02:50  pvmsrc
 * Implemented pvm_archcode() for 3.4 data signature handling.
 * 	- backwards compat.
 * (Spanker=kohl)
 *
 * Revision 1.80  1999/01/14  19:10:25  pvmsrc
 * More mbox fixes:
 * 	- make sure pvm_errno gets set in mbox and old insert/lookup/delete
 * 		calls, even if lpvmerr() isn't called because of an accepted
 * 		error code.
 * 	- modified old 3.3 interface to create persistent mboxes...
 * 	- gutted pvm_lookup() to snag the returned index for -1/firstavail
 * 		queries, as pvm_recvinfo() doesn't ever return the index value.
 * 		D-Oh...  Too late to fix it now...
 * (Spanker=kohl)
 *
 * Revision 1.79  1999/01/13  00:04:46  pvmsrc
 * Filled in backwards compat guts:
 * 	- to pvm_insert(), pvm_lookup() & pvm_delete().
 * 	- use mbox stuff, no more #ifdef PVM33COMPAT...
 * 	- modified mbox pvm_putinfo() stuff to pass in -1 instead of 0
 * 		for the internal index value...
 * (Spanker=kohl)
 *
 * Revision 1.78  1998/11/20  20:04:04  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (Spanker=sscott)
 *
 * Revision 1.77  1998/10/12  21:16:28  pvmsrc
 * Damn.  Typo - should be pvmsbuf->m_mid, not pvmsbuf...  D-Oh!
 * (Spanker=kohl)
 *
 * Revision 1.76  1998/10/12  21:08:57  pvmsrc
 * Fixed pvmtrcsbfsave fiasco in pvm_send/mcast().
 * 	- as used for message number of bytes trace data.
 * 	- if not using PvmTraceFull tracing option, pvmtrcsbfsave will not
 * 		be set / swapped with pvmsbuf...
 * 	- check for non-NULL pvmtrcsbfsave, if not use pvmsbuf instead.
 * (Spanker=kohl)
 *
 * Revision 1.75  1998/10/02  15:44:01  pvmsrc
 * Single source code merge of Win32 and Unix code.
 * (Spanker=sscott)
 *
 * Revision 1.74  1998/09/02  14:07:05  pvmsrc
 * Fixed bug in pvm_siblings().
 * 	- incorrect handling of PvmParentNotSet case...
 * (Spanker=kohl)
 *
 * Revision 1.73  1998/08/27  15:16:53  pvmsrc
 * Plugged memory leak in pvm_recvinfo().
 * 	- pvmrbuf is freed O.K., but the wrapper message from the pvmd
 * 		(the one with the mbox message in it :-) was not being freed.
 * (Spanker=kohl)
 *
 * Revision 1.72  1998/06/26  15:26:28  pvmsrc
 * Fixed pvm_trecv():
 * 	- if timeout is 0, could return without ever probing...
 * 	- make sure mroute() is called at least once before timing
 * 		out in the 0 case.
 * (Spanker=kohl)
 *
 * Revision 1.71  1998/02/23  22:51:33  pvmsrc
 * Added AIX4SP2 stuff.
 * (Spanker=kohl)
 *
 * Revision 1.70  1998/01/28  20:08:56  pvmsrc
 * Added new -DSYSERRISCONST define.
 * 	- for const char *sys_errlist...
 * (Spanker=kohl)
 *
 * Revision 1.69  1998/01/28  19:13:52  pvmsrc
 * Added new IMA_LINUXHPPA to #if cases...
 * (Spanker=kohl)
 *
 * Revision 1.68  1997/12/31  22:14:25  pvmsrc
 * Renamed TEV_REMOVE -> TEV_DELINFO.  D-Oh.
 * (Spanker=kohl)
 *
 * Revision 1.67  1997/12/31  20:51:29  pvmsrc
 * Added saving / resetting of current send / recv buffer around mhf.
 * 	- so each message handler doesn't have to do it.
 * 	- while we're at it, might as well set rbuf to mhf message, too.
 * Removed TEV_DID_MRF trace event element for pvm_addmhf() entry event.
 * 	- seemed like a good idea at the time, but no way to avoid the
 * 		compiler warnings on SGIs when trying to cast funct ptr to
 * 		int ptr for packing into trace message...  :-Q
 * (Spanker=kohl)
 *
 * Revision 1.66  1997/12/29  19:41:21  pvmsrc
 * 	Changed char *em to const char *em.
 * 	For Redhat 4.2/5.0 compatibility under LINUX.
 * (Spanker=phil)
 *
 * Revision 1.65  1997/12/23  20:23:53  pvmsrc
 * Added new TEV_MHF_INVOKE trace event.
 * 	- include message handler src/tag/ctx settings.
 * 	- include actual message bufid, len, and src/tag/ctx.
 * 	- use new TEV_PUSHTOP, TEV_AMPUSHED and TEV_POPTOP macros to
 * 		fake top level tracing for PVM calls in message handler.
 * (Spanker=kohl)
 *
 * Revision 1.64  1997/12/17  22:16:51  pvmsrc
 * Added tracing instrumentation to pvm_addmhf() and pvm_delmhf().
 * 	- rewrote as necessary to capture return code.
 * (Spanker=kohl)
 *
 * Revision 1.63  1997/11/04  23:20:39  pvmsrc
 * Added IMA_LINUXALPHA to const char * error log stuff.
 * (Spanker=kohl)
 *
 * Revision 1.62  1997/10/24  15:17:52  pvmsrc
 * Added TEV_DID_RCX to trace events for Receive Message Context.
 * 	- in pvm_recv(), pvm_trecv(), pvm_nrecv(), and pvm_precv().
 * (Spanker=kohl)
 *
 * Revision 1.61  1997/10/24  14:29:27  pvmsrc
 * Added TEV_DID_MCX / pvmmyctx trace event info to:
 * 	- pvm_send(), pvm_mcast(), pvm_recv(), pvm_trecv(), pvm_nrecv().
 * 	- pvm_psend(), pvm_precv().
 * (Spanker=kohl)
 *
 * Revision 1.60  1997/09/22  21:13:30  pvmsrc
 * Added new pvmsettaskname() linkage (for shell-spawned tasks only!).
 * 	- call pvmsettaskname() before joining PVM, sends task name
 * 		(stored in new char *pvmmytaskname global) to pvmd in
 * 		TM_CONN2 message.
 * 	- appears in trace events and console ps.
 * (Spanker=kohl)
 *
 * Revision 1.59  1997/09/10  21:36:12  pvmsrc
 * changing socket startup to pvmbeatask --> pvm_parent did not
 * enroll correctly. Markus
 * (Spanker=fischer)
 *
 * Revision 1.58  1997/08/29  13:35:10  pvmsrc
 * OS2 Port Submitted by Bohumir Horeni, horeni@login.cz.
 * (Spanker=kohl)
 *
 * Revision 1.57  1997/08/27  20:19:44  pvmsrc
 * Fixed bug in tracing - message number of bytes for pvm_send/mcast().
 * 	- "pvmsbuf" is really "pvmtrcbuf" inside tracing scope...  D-Oh!
 * 	- use saved "pvmtrcsbfsave" buffer id for pvm_bufinfo() instead.
 * (Spanker=kohl)
 *
 * Revision 1.56  1997/08/25  21:46:21  pvmsrc
 * Fixed call to enc_trc_fin() in pvm_send().
 * 	- for user-defined trace events, pvmsbuf arg was missing...
 * (Spanker=kohl)
 *
 * Revision 1.55  1997/08/06  22:43:16  pvmsrc
 * Added new SGI6 and SGIMP6 arches.
 *
 * Revision 1.54  1997/07/21  19:08:31  pvmsrc
 * Well slap SScott around the next time you see him.  :-)
 * 	- fixed pvm_delmhf() to handle boundary case where deleted
 * 		entry is at end of list (can't copy last entry to the
 * 		deleted slot if you're already there... :-).
 * 	- fixed up mesg_input() to check for wildcarded handler tag
 * 		like everything else...
 *
 * Revision 1.53  1997/07/08  15:29:53  pvmsrc
 * 	Don't let routing option be changed on PGON and SP2MPI.
 *
 * Revision 1.52  1997/06/27  21:02:29  pvmsrc
 * removed working debug statments from pvm_delmhf()
 *
 * Revision 1.51  1997/06/27  18:04:58  pvmsrc
 * Integrated WIN32 changes.
 *
 * Revision 1.50  1997/06/26  19:38:05  pvmsrc
 * pvm_siblings now waits for siblings to come from spawning parent
 * 	even when PvmNoParent is set.
 *
 * Also, if a task is run from the command line, pvm_siblings returns
 * 	a sibling set of size 1 and with tid = pvmmytid
 *
 * Revision 1.49  1997/06/26  15:03:03  pvmsrc
 * Awwwwwww Crap.  Beat me with a shovel.  F---ing Typo.
 *
 * Revision 1.48  1997/06/26  14:56:48  pvmsrc
 * Forget something Steve?  D-Oh, D-Oh, D-Oh...
 * 	- how about setting the handles[].f field when you create a
 * 		message handler -- it comes in handy when you actually
 * 		want to HANDLE a message...  :-/~
 *
 * Revision 1.47  1997/06/26  13:41:34  pvmsrc
 * Added default return value to pvm_delmhf().
 * 	- SGI compiler warning (and incorrect (missing) return case,
 * 		PvmOk).
 * 	- text formatting...  (broke up SScott's 200 character long lines :)
 *
 * Revision 1.46  1997/06/25  18:02:58  pvmsrc
 * pvm_addmhf() complete rewrite
 * pvm_delmhf() complete rewrite
 * pvm_exit() reversed removal sequence of message handlers.
 * pvmdisplaymhfinfo() new internal function for testing message handlers
 *
 * Revision 1.45  1997/06/23  18:36:04  pvmsrc
 * Oops...  pvm_reg_rm() should add itself to PVMRMCLASS.
 * 	- not PVMTASKERCLASS...  typo...  D-Oh...
 *
 * Revision 1.44  1997/06/12  20:10:41  pvmsrc
 * Made sure all communications for TC_* task control messages
 * 	use the SYSCTX_TC system context.
 * 	- some messages being sent in default context...  D-Oh...
 *
 * Revision 1.43  1997/05/21  16:01:47  pvmsrc
 * Updated ifdefs to include AIX4MP arch type.
 *
 * Revision 1.42  1997/05/07  21:22:09  pvmsrc
 * AAAIIIEEEEEEEEE!!!!
 * 	- removed all static-limited string unpacking:
 * 		* replaced with use of:
 * 			upkstralloc() / PVM_FREE() (for pvmd stuff).
 * 			new pvmupkstralloc() / PVM_FREE() (for lpvm stuff).
 * 		* manual allocation of local buffers for sprintf() & packing.
 * 		* alternate static string arrays to replace fixed-length cases.
 * 		* I hope this shit works...  :-Q
 *
 * Revision 1.41  1997/05/02  19:48:06  pvmsrc
 * 	Fix precv on SP2MPI -- needed to be added to an ifdef.
 *
 * Revision 1.40  1997/05/02  14:54:16  pvmsrc
 * Implemented guts of pvm_getmboxinfo():
 * 	- user library side sends pattern & receives back / allocs array.
 * 	- at pvmd side mb_names() searches class list for matching pattern,
 * 		count up # of entries, and pack up struct info.
 * 		(Note:  does not yet use GNU regex stuff...)
 *
 * Revision 1.39  1997/05/02  13:44:32  pvmsrc
 * 	Support for SP2MPI. Comment references to mpierrcode.
 *
 * Revision 1.38  1997/05/01  20:17:39  pvmsrc
 * Renamed TEV_MBOXINFO -> TEV_GETMBOXINFO.
 *
 * Revision 1.37  1997/04/30  21:26:02  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.36  1997/04/24  21:05:54  pvmsrc
 * Short circuiting on pvm_precv for mpps
 *
 * Revision 1.35  1997/04/21  14:46:28  pvmsrc
 * pvm_addmhf() ifdefs for SCO
 *
 * Revision 1.34  1997/04/17  12:54:25  pvmsrc
 * rename of pvm_mboxinfo() to include word "get"
 *
 * Revision 1.33  1997/04/14  14:28:42  pvmsrc
 * pvm_exit() now removes all message handlers
 *
 * Revision 1.32  1997/04/10  20:32:46  pvmsrc
 * Typo...
 *
 * Revision 1.31  1997/04/10  20:14:20  pvmsrc
 * (Partially) replaced pvm_getnames() with new pvm_mboxinfo().
 * 	- needs actual implementation details...  ha ha ha...
 *
 * Revision 1.30  1997/04/09  20:10:52  pvmsrc
 * Memory spank - freeing static mem...  D-Oh...
 *
 * Revision 1.29  1997/04/09  19:37:58  pvmsrc
 * Added class name and index args to pvmreset() routine:
 * 	- allow systematic elimination of leftover persistents...
 * 	- heh, heh, heh...
 * In dm_db(), check for name (class) and req (index) match
 * 	for TMDB_RESET case (""/-1 for all, resp).
 *
 * Revision 1.28  1997/04/09  18:31:13  pvmsrc
 * Added "killtasks" arg tp pvmreset():
 * 	- allow for only resetting of mboxes, w/o task kill...
 * 	- otherwise, too painful to clean up mbox db...
 *
 * Revision 1.27  1997/04/08  17:23:31  pvmsrc
 * Damn, Damn, Damn...
 * 	- pvmreset() must be in library side only, pvmds use different
 * 		message buffer mgt, packing and sending routines...  :-Q
 *
 * Revision 1.26  1997/04/07  21:07:57  pvmsrc
 * pvm_addmhf() - new paramter interface
 *
 * pvm_addmhf() and pvm_delmhf() now maintain a "free-list" from which
 * new entries are first filled before allocating new space.
 *
 * Revision 1.25  1997/04/07  19:55:26  pvmsrc
 * Nope, changed my mind.  pvm_getnoresets() saves tidlist in static,
 * 	reuse allocated mem on next call, a la pcm_tasks()...
 *
 * Revision 1.24  1997/04/07  19:39:58  pvmsrc
 * Plugged memory leak in pvm_getnoresets().
 * 	- if no return tids ptr provided, just free tidlist array.
 *
 * Revision 1.23  1997/04/04  15:44:35  pvmsrc
 * New pvm_addmhf() and pvm_delmhf()
 *
 * Revision 1.22  1997/04/03  19:25:02  pvmsrc
 * Added context for TM_xxx messages SM_xxx messages use the
 * PvmBaseContext for compatibility
 *
 * Revision 1.21  1997/04/01  21:28:14  pvmsrc
 * Damn Damn Damn.
 * 	- pvm_recvinfo() returns a bufid, not an index.  Damn.
 *
 * Revision 1.20  1997/03/31  21:39:08  pvmsrc
 * *** Changed syntax of mbox routines.
 * 	- pvm_putinfo() dropped index arg, changed flags semantics.
 * 	- pvm_getinfo() -> pvm_recvinfo(), a la pvm_recv()...
 * 	- added PvmMboxDefault for flags usage.
 * 	- changed routines & calls.
 * 	- extension of sscott submission...
 *
 * Revision 1.19  1997/03/27  19:56:15  pvmsrc
 * In pvm_reg_tracer():
 * 	- added packing of other assundry trace settings in tracer mailbox.
 * 	- for shell-spawned tasks to get tmask, topt, etc...
 *
 * Revision 1.18  1997/03/06  21:07:38  pvmsrc
 * 		- added include to pvmmimd.h
 * 		- test properly on mpps when trying to set pvmfrgsize
 *
 * Revision 1.17  1997/03/04  22:47:25  pvmsrc
 * Added stuffing of message mailboxes for:
 * 	- Resource Managers, Hosters, Taskers and Tracers.
 * 	- done by registered task, for auto-cleanup on exit.
 * 	- delete info on unregister...
 *
 * Revision 1.16  1997/02/17  20:50:58  pvmsrc
 * D-Oh! D-Oh! D-Oh!  A stupid Jim typo:
 * 	* PvmFirstAvail & PvmLock  ->  PvmFirstAvail | PvmLock.
 * 	* D-Oh!
 *
 * Revision 1.15  1997/02/17  19:43:41  pvmsrc
 * Added check in pvm_setopt() / PvmNoReset for pvmmytid > 0.
 *
 * Revision 1.14  1997/02/17  18:47:26  pvmsrc
 * Oops...  must pass ptr to tids array for pvm_getnoresets().
 *
 * Revision 1.13  1997/02/17  16:29:44  pvmsrc
 * Added new pvm_getnoresets() routine.
 *
 * Revision 1.12  1997/02/13  23:35:44  pvmsrc
 * Added new PvmNoReset setopt/getopt option.
 * 	- added new pvmnoreset, pvmnoresetindex globals in lpvm.[ch].
 * 	- to set use pvm_putinfo() -> PVMNORESETCLASS first avail w/lock,
 * 		save index in pvmnoresetindex.
 * 	- to unset use pvm_delinfo().
 *
 * Revision 1.11  1997/02/13  15:10:43  pvmsrc
 * Fixed Bob Bug in pvm_exit():
 * 	- "=" -> "=="...  Ooooo...
 *
 * Revision 1.10  1997/01/28  19:26:26  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.9  1996/12/19  19:59:15  pvmsrc
 * Fixed pvm log messages for pvm_tc_*() routines.
 * 	- stuff no longer in pvmmctl()...
 *
 * Revision 1.8  1996/12/19  15:06:45  pvmsrc
 * Added missing decls for error globals (#ifndef HASERRORVARS).
 * 	- errno, sys_errlist[] & sys_nerr.
 *
 * Revision 1.7  1996/12/18  22:27:46  pvmsrc
 * Extracted duplicate versions of routines from lpvm/mimd/shmem.c,
 * 	inserted into shared lpvmgen.c:
 * 	- pvmbailout().
 * 	- pvmlogerror().
 * 	- vpvmlogprintf(), pvmlogprintf().  (hope these work on MPP & shmem)
 * 	- pvmlogperror().
 *
 * Revision 1.6  1996/10/31  20:44:20  pvmsrc
 * Added a static char version variable in pvm_version() to avoid
 * stack corruption.
 *
 * Revision 1.5  1996/10/25  13:57:24  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.4  1996/10/24  22:26:55  pvmsrc
 * Moved #include "global.h" below other #include's for typing.
 * Added #include of new "lpvm.h" header to replace explicit externs.
 * Extracted common message handler routines from lpvm*.c:
 * 	- pvm_tc_noop(), pvm_tc_settrace(), pvm_tc_settrcbuf(),
 * 		pvm_tc_settrcopt(), pvm_tc_settmask(), and pvm_tc_siblings().
 * Extracted PVM option routines from lpvm*.c:
 * 	- pvm_getopt() & pvm_setopt().
 * 	- merged special mimd / shmem cases into generic routine.
 * 	- added new tracing options PvmTraceBuffer, PvmTraceOptions,
 * 		PvmSelfTraceBuffer & PvmSelfTraceOptions.
 * Modified trace event generation for message / packing routines:
 * 	- replaced old TEV_*[01] constants with new TEV_EVENT_ENTRY/EXIT.
 * 	- replaced standard pvm_pk*() calls with new TEV_PACK_*() macro
 * 		calls and TEV_DID_* identification.
 * 	- in pvm_send(), check for user-defined message destination
 * 		and handle proper finishing and sending of user-defined
 * 		trace event.
 * 	- in pvm_spawn(), use new Pvmtracer structure info in place of
 * 		old globals, pass along new PVMTRCBUF & PVMTRCOPT env vars.
 * Added new pvm_reg_tracer() routine for registering a task as a tracer.
 *
 * Revision 1.3  1996/10/11  17:03:10  pvmsrc
 * Replaced references to pvm_remove() with pvm_delinfo():  renamed.
 *
 * Revision 1.2  1996/10/08  18:32:21  pvmsrc
 * Renamed routines:
 * 	- pvm_put() -> pvm_putinfo().
 * 	- pvm_get() -> pvm_getinfo().
 *
 * Revision 1.1  1996/09/23  23:44:14  pvmsrc
 * Initial revision
 *
 * Revision 1.10  1995/11/02  16:09:32  manchek
 * added NEEDSENDIAN switch.
 * pass environment through spawn on PGON
 *
 * Revision 1.9  1995/07/28  16:04:05  manchek
 * switch endian includes on flag, not arch name
 *
 * Revision 1.8  1995/06/12  15:46:46  manchek
 * pvm_recvf now returns pointer to default matcher instead of 0
 *
 * Revision 1.7  1995/02/01  21:10:33  manchek
 * pvm_hostsync didn't free its message buffers
 *
 * Revision 1.6  1994/11/07  21:36:33  manchek
 * flush messages on pvm_exit.
 * function prototype for SCO
 *
 * Revision 1.5  1994/10/15  19:12:35  manchek
 * check for system error in TM_SPAWN reply (cc != count)
 *
 * Revision 1.4  1994/06/03  20:38:16  manchek
 * version 3.3.0
 *
 * Revision 1.3  1993/11/30  15:52:42  manchek
 * implemented case autoerr == 2 in lpvmerr()
 *
 * Revision 1.2  1993/09/16  21:37:39  manchek
 * pvm_send() missing a return before lpvmerr()
 *
 * Revision 1.1  1993/08/30  23:26:48  manchek
 * Initial revision
 *
 */


#include <stdio.h>
#ifdef NEEDMENDIAN
#include <sys/types.h>
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

#ifdef	SYSVSTR
#include <string.h>
#define	CINDEX(s,c)	strchr(s,c)
#else
#include <strings.h>
#define	CINDEX(s,c)	index(s,c)
#endif

#include <errno.h>
#include <signal.h>

#ifdef	__STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <pvmproto.h>
#include "pvmalloc.h"
#include "pmsg.h"
#include "listmac.h"
#include "bfunc.h"
#include "tvdefs.h"
#include "lpvm.h"
#include <pvmtev.h>
#include "tevmac.h"
#include "host.h"
#include "waitc.h"
#include "global.h"

#if defined(IMA_PGON) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
	|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
#include "pvmmimd.h"
#include "lmsg.h"
extern struct msgid *pvm_inprecv;
#endif

#ifndef HASERRORVARS
extern int errno;						/* from libc */
#ifndef USESTRERROR
extern char *sys_errlist[];
extern int sys_nerr;
#endif
#endif

#ifdef WIN32
#ifndef SOCK_DEFINES
extern WSADATA WSAData;
int nAlert=SO_SYNCHRONOUS_NONALERT;
#define SOCK_DEFINES
#endif
#endif

struct mhandler {
	int mhid;					/* unique message handler id */
	struct pvmminfo header;		/* source, tag, context */
	int (*f)();					/* handler function */
};

struct dhandler {
	int mhid;		/* message handler id - used for free list links */
	int handle;		/* index of associated handles[] entry */
};


/***************
 **  Globals  **
 **           **
 ***************/

char *pvmnametag();
char *getenv();

struct pmsg *midtobuf();


/***************
 **  Private  **
 **           **
 ***************/

static int def_match();
static int (*recv_match)() = def_match;

/* handles[] contains structures with message handler info */
static struct mhandler *handles = 0;

/* element count for handles[] array - all of these are active */
static int nhandles = 0;

/* pointer to the dhandles[] array */
static struct dhandler *dhandles = 0;

/* element count for dhandles[] array - not all active though */
static int ndhandles = 0;

/* free list dhandles; -1 indicates empty list */
static int fl_dhandles = -1;


/**************************
 **  Internal Functions  **
 **                      **
 **************************/


/*
*	Function: pvmdisplaymhfinfo
*
*  function displays message handler data structure information
*
*/
void
pvmdisplaymhfinfo(who, msg, tid)
	char *who;			/* who made this call */
	char *msg;			/* message from caller */
	int tid;			/* tid of caller */
{	char *me = "pvmdisplaymhfinfo";
	int i;
	int head;

	printf("\n%s t%x: Display Message Handler Information: ",
		who, tid );
	printf("ndhandles = %d\tnhandles = %d\tfl_dhandles = %d", 
		ndhandles, nhandles, fl_dhandles);

	printf("\n%s: free list.  head <-", me);
	head = fl_dhandles;
	while (head != -1){
		printf(" %d", head);
		head = dhandles[head].mhid;
	}
	printf(" -< tail");

	printf("\n%s t%x: %s\n", who, tid, msg);
	for (i = 0; i < ndhandles; i++ ) {
		printf("%s t%x: dhandles[%2d].mhid = %2d  .handle = %2d     ",
			who, tid, i, dhandles[i].mhid, dhandles[i].handle);
		printf("handles[%2d].mhid = %2d  .header.src = t%-8x  ",
			i, handles[i].mhid, handles[i].header.src);
		printf(".header.ctx = %8d  .header.tag= %8d\n",
			handles[i].header.ctx, handles[i].header.tag);
	}
	printf("\n\n"); fflush(stdout);
} /* pvmdisplaymhfinfo */


static int
def_match(mid, tid, tag)
	int mid;
	int tid;
	int tag;
{
	struct pmsg *up;

	if (!(up = midtobuf(mid)))
		return PvmNoSuchBuf;
	return ((tid == -1 || tid == up->m_src)
			&& (tag == -1 || tag == up->m_tag)
			&& (pvmmyctx == -1 || pvmmyctx == up->m_ctx)) ? 1 : 0;
}


/*	pvmbailout()
*
*	Called by low-level stuff in f.e. pvmfrag.c.  Don't really want to
*	bail in libpvm.
*/

void
pvmbailout(n)
	int n;
{
	n = n;	/* ayn rand was here */
}


/*	pvmlogerror()
*
*	Log a libpvm error message.  Prepends a string identifying the task.
*/

int
pvmlogerror(s)
	char *s;
{
	if (pvmmytid == -1)
		fprintf(stderr, "libpvm [pid%d]: %s", pvmmyupid, s);
#ifdef LOG
	else if (logfp)
	{
		fprintf(logfp, "libpvm [t%x]: %s", pvmmytid, s);
		fflush(logfp);
	}
#endif
	else
		fprintf(stderr, "libpvm [t%x]: %s", pvmmytid, s);
	return 0;
}


int
vpvmlogprintf(fmt, ap)
	char *fmt;
	va_list ap;
{
	static int newline = 1;
	int cc;

	if (newline) {
		if (pvmmytid == -1)
			fprintf(stderr, "libpvm [pid%d] ", pvmmyupid);
		else
			fprintf(stderr, "libpvm [t%x] ", pvmmytid);
	}
	cc = vfprintf(stderr, fmt, ap);
	newline = (fmt[strlen(fmt) - 1] == '\n') ? 1 : 0;
	fflush(stderr);
	return(cc);
}


/*	pvmlogprintf()
*
*	Log a libpvm error message.  Prepends the task id.  Takes
*	printf-like args.
*/

int
#ifdef __STDC__
pvmlogprintf(const char *fmt, ...)
#else
pvmlogprintf(va_alist)
	va_dcl
#endif
{
	va_list ap;
	int cc;

#ifdef __STDC__
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


/*	pvmlogperror()
*
*	Log a libpvm error message.  Prepends a string identifying the
*	task and appends the system error string for _errno.
*/

int
pvmlogperror(s)
	char *s;
{
#ifdef SYSERRISCONST
	const char *em;
#else
	char *em;
#endif

#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	int l;

	/* if (mpierrcode) {
		MPI_Error_string(mpierrcode, pvmtxt, &l);
		em = pvmtxt;
	} else */
#ifdef USESTRERROR
		em = strerror( errno );
#else
		em = ((errno >= 0 && errno < sys_nerr)
			? sys_errlist[errno] : "Unknown Error");
#endif
#else

#ifdef IMA_CM5
	errno = CMMD_get_errno();
#endif

#ifdef USESTRERROR
	em = (char *) strerror( errno );
#else
	em = ((errno >= 0 && errno < sys_nerr)
		? sys_errlist[errno] : "Unknown Error");
#endif

#endif /*IMA_SP2MPI*/

        if ( s == NULL ) {
           pvmlogprintf("NULL STRING PASSED TO pvmlogperror() errno=%d\n", errno);
           s = "(null string passed to pvmlogperror()...)";
        }

        if ( em == NULL ) {
           pvmlogprintf("STRERROR IS BROKEN - %s errno=%d\n", s, errno);
           return -1;
        }

	pvmlogprintf("%s: %s\n", s, em);

	return 0;
}


/*	lpvmerr()
*
*	Error has occurred in libpvm function.
*	Action determined by pvmautoerr (set by setopt):
*		0	Do nothing
*		1	Print error message
*		2	Print error message, exit program with error code
*		3	Print error message, abort program
*/

int
lpvmerr(f, n)
	char *f;		/* error location */
	int n;			/* error code */
{
	char buf[128];

	pvm_errno = n;
	if (pvmautoerr) {
		buf[0] = 0;
		strncat(buf, f, sizeof(buf)-4);
		strcat(buf, "()");
		pvm_perror(buf);
		fflush(stderr);
		if (pvmautoerr == 3)
			abort();
		if (pvmautoerr == 2) {
			pvmautoerr = 1;
			pvm_exit();
			exit(n);
		}
	}
	return n;
}


/*	mesg_input()
*
*	Receive message into task.  All received messages go through this function.
*	If a handler was specified for this tag, call it.
*	Otherwise, put the message on rxlist.
*/

int
mesg_input(up)
	struct pmsg *up;
{
	int savesbuf;
	int saverbuf;
	int savectx;
	int i;
	TEV_DECLS

	if (pvmdebmask & PDMMESSAGE)
		pvmlogprintf("mesg_input() src t%x ctx %d tag %s len %d\n",
				up->m_src, up->m_ctx,
				pvmnametag(up->m_tag, (int *)0), up->m_len);
	for (i = nhandles; i-- > 0; ) {
		if ( (handles[i].header.tag == -1
				|| handles[i].header.tag == up->m_tag)
			&& (handles[i].header.ctx == -1
				|| handles[i].header.ctx == up->m_ctx)
			&& (handles[i].header.src == -1
				|| handles[i].header.src == up->m_src) )
		{
			if (TEV_DO_TRACE(TEV_MHF_INVOKE,TEV_EVENT_ENTRY)) {

				TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR,
						&(handles[i].header.src), 1, 1 );
				TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR,
						&(handles[i].header.tag), 1, 1 );
				TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR,
						&(handles[i].header.ctx), 1, 1 );

				TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR,
						&(up->m_mid), 1, 1 );
				TEV_PACK_INT( TEV_DID_MNB, TEV_DATA_SCALAR,
						&(up->m_len), 1, 1 );
				TEV_PACK_INT( TEV_DID_MC, TEV_DATA_SCALAR,
						&(up->m_tag), 1, 1 );
				TEV_PACK_INT( TEV_DID_MCX, TEV_DATA_SCALAR,
						&(up->m_ctx), 1, 1 );
				TEV_PACK_INT( TEV_DID_SRC, TEV_DATA_SCALAR,
						&(up->m_src), 1, 1 );

				TEV_FIN;

				TEV_PUSHTOP;
			}

			savesbuf = pvm_setsbuf( 0 );
			saverbuf = pvm_setrbuf( up->m_mid );
			savectx = pvm_setcontext( up->m_ctx );

			(handles[i].f)(up->m_mid);

			pvm_setcontext( savectx );
			pvm_freebuf( pvm_setsbuf( savesbuf ) );
			pvm_freebuf( pvm_setrbuf( saverbuf ) );

			if ( TEV_AMPUSHED )
				TEV_POPTOP;

			return 0;
		}
	}
	LISTPUTBEFORE(pvmrxlist, up, m_link, m_rlink);
	return 0;
}


/************************
 **  Message Handlers  **
 **                    **
 ************************/

/*	pvm_tc_noop()
*
*	Process a NO-OP message.  Har.
*/

int
pvm_tc_noop(mid)
	int mid;
{
	int src;

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);

	if (pvmdebmask) {
		pvmlogprintf("pvm_tc_noop() NOOP from t%x\n", src);
	}
	pvm_freebuf(mid);
	return 0;
}


/*	pvm_tc_settrace()
*
*	Set our tracing on the fly
*
*	TC_SETTRACE() {
*		int trctid			// task id for tracing
*		int trcctx			// message context for trace messages
*		int trctag			// message tag for trace messages
*		int outtid			// task id for stdout output
*		int outctx			// message context for output messages
*		int outtag			// message tag for output messages
*		string mask			// new trace mask
*		int tbuf			// trace buffer size
*		int topt			// trace options
*	}
*/

int
pvm_tc_settrace(mid)
	int mid;
{
	int trctid;
	int trcctx;
	int trctag;
	int outtid;
	int outctx;
	int outtag;
	char buf[256];
	int tbuf;
	int topt;

	pvm_upkint(&trctid, 1, 1);
	pvm_upkint(&trcctx, 1, 1);
	pvm_upkint(&trctag, 1, 1);
	pvm_upkint(&outtid, 1, 1);
	pvm_upkint(&outctx, 1, 1);
	pvm_upkint(&outtag, 1, 1);
	pvm_upkstr(buf);
	pvm_upkint(&tbuf, 1, 1);
	pvm_upkint(&topt, 1, 1);

	if (trctid) {
		/* cheat on trcctx & trctag to avoid race */
		pvmtrc.trcctx = trcctx;
		pvmtrc.trctag = trctag;
		pvm_setopt( PvmSelfTraceTid, trctid );
		if (strlen(buf) + 1 == TEV_MASK_LENGTH)
			BCOPY(buf, pvmtrc.tmask, TEV_MASK_LENGTH);
		else {
			TEV_MASK_INIT(pvmtrc.tmask);
			pvmlogerror("pvm_tc_settrace() bogus trace mask\n");
		}
		BCOPY(pvmtrc.tmask, pvmctrc.tmask, TEV_MASK_LENGTH);
		if (tbuf >= 0)
			pvmtrc.trcbuf = tbuf;
		else {
			pvmtrc.trcbuf = 0;
			pvmlogerror("pvm_tc_settrace() bogus trace buffering\n");
		}
		if (topt >= 0)
			pvmtrc.trcopt = topt;
		else {
			pvmtrc.trcopt = 0;
			pvmlogerror("pvm_tc_settrace() bogus trace options\n");
		}
	}

	if (outtid) {
		/* cheat on outctx & outtag to avoid race */
		pvmtrc.outctx = outctx;
		pvmtrc.outtag = outtag;
		pvm_setopt( PvmSelfOutputTid, outtid );
	}

	pvm_freebuf(mid);
	return 0;
}


/*	pvm_tc_settrcbuf()
*
*	Set our trace buffering on the fly
*
*	TC_SETTRCBUF() {
*		int buf				// new trace buffer size
*	}
*/

int
pvm_tc_settrcbuf(mid)
	int mid;
{
	int tbuf;

	pvm_upkint(&tbuf,1,1);

	if (tbuf >= 0)
		pvmtrc.trcbuf = tbuf;
	else {
		pvmtrc.trcbuf = 0;
		pvmlogerror("pvm_tc_settrcbuf() bogus trace buffering\n");
	}

	pvm_freebuf(mid);
	return 0;
}


/*	pvm_tc_settrcopt()
*
*	Set our trace options on the fly
*
*	TC_SETTRCOPT() {
*		int opt				// new trace options
*	}
*/

int
pvm_tc_settrcopt(mid)
	int mid;
{
	int topt;

	pvm_upkint(&topt,1,1);

	if (topt >= 0)
		pvmtrc.trcopt = topt;
	else {
		pvmtrc.trcopt = 0;
		pvmlogerror("pvm_tc_settrcopt() bogus trace options\n");
	}

	pvm_freebuf(mid);
	return 0;
}


/*	pvm_tc_settmask()
*
*	Set our trace mask on the fly
*
*	TC_SETTMASK() {
*		string mask				// new trace mask
*	}
*/

int
pvm_tc_settmask(mid)
	int mid;
{
	char buf[256];

	pvm_upkstr(buf);
	if (strlen(buf) + 1 == TEV_MASK_LENGTH)
		BCOPY(buf, pvmtrc.tmask, TEV_MASK_LENGTH);
	else
		pvmlogerror("pvm_tc_settmask() bogus trace mask\n");

	pvm_freebuf(mid);
	return 0;
}


/*	pvm_tc_siblings()
*
*	Parent task announces our spawn group.
*
*	TC_SIBLINGS() {
*		int length
*		int tids[length]		// sibling tids
*	}
*/

int
pvm_tc_siblings(mid)
	int mid;
{
	pvm_upkint(&pvmnsibs, 1, 1);
	if (pvmnsibs > 0) {
		pvmsibtids = TALLOC(pvmnsibs, int, "sibs");
		pvm_upkint(pvmsibtids, pvmnsibs, 1);
	}
	pvm_freebuf(mid);
	return 0;
}


/************************
 **  Libpvm Functions  **
 **                    **
 ************************/

int
pvm_getopt(what)
	int what;
{
	int rc = 0;
	int err = 0;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (pvmmytid != -1
				&& TEV_DO_TRACE(TEV_GETOPT,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_OPT, TEV_DATA_SCALAR, &what, 1, 1 );
			TEV_FIN;
		}
	}

	switch (what) {
	case PvmRoute:
		rc = pvmrouteopt;
		break;

	case PvmDebugMask:
		rc = pvmdebmask;
		break;

	case PvmAutoErr:
		rc = pvmautoerr;
		break;

	case PvmOutputTid:
		rc = pvmctrc.outtid;
		break;

	case PvmOutputContext:
		rc = pvmctrc.outctx;
		break;

	case PvmOutputCode:
		rc = pvmctrc.outtag;
		break;

	case PvmTraceTid:
		rc = pvmctrc.trctid;
		break;

	case PvmTraceContext:
		rc = pvmctrc.trcctx;
		break;

	case PvmTraceCode:
		rc = pvmctrc.trctag;
		break;

	case PvmTraceBuffer:
		rc = pvmctrc.trcbuf;
		break;

	case PvmTraceOptions:
		rc = pvmctrc.trcopt;
		break;

	case PvmFragSize:
		rc = pvmfrgsiz;
		break;

	case PvmResvTids:
		rc = pvmrescode;
		break;

	case PvmSelfOutputTid:
		rc = pvmtrc.outtid;
		break;

	case PvmSelfOutputContext:
		rc = pvmtrc.outctx;
		break;

	case PvmSelfOutputCode:
		rc = pvmtrc.outtag;
		break;

	case PvmSelfTraceTid:
		rc = pvmtrc.trctid;
		break;

	case PvmSelfTraceContext:
		rc = pvmtrc.trcctx;
		break;

	case PvmSelfTraceCode:
		rc = pvmtrc.trctag;
		break;

	case PvmSelfTraceBuffer:
		rc = pvmtrc.trcbuf;
		break;

	case PvmSelfTraceOptions:
		rc = pvmtrc.trcopt;
		break;

	case PvmShowTids:
		rc = pvmshowtaskid;
		break;

	case PvmNoReset:
		rc = pvmnoreset;
		break;

#if defined(IMA_AIX4MP) || defined(IMA_AIX5MP) \
	|| defined(IMA_ALPHAMP) || defined(IMA_CSPP) \
	|| defined(IMA_HPPAMP) || defined(IMA_RS6KMP) \
	|| defined(IMA_SGIMP) || defined(IMA_SGIMP6) \
	|| defined(IMA_SGIMP64) || defined(IMA_SUNMP)

	case PvmPollTime:
		rc = pvmpolltime;
		break;
	
	case PvmPollType:
		rc = pvmpolltype;
		break;

#else /* SHMEM */

	case PvmPollTime:
	case PvmPollType:
		rc = PvmNotImpl;
		err = 1;
		break;

#endif /* SHMEM */

	default:
		err = 1;
		break;
	}

	if (TEV_AMEXCL) {
		if (pvmmytid != -1
				&& TEV_DO_TRACE(TEV_GETOPT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_OPV, TEV_DATA_SCALAR, &rc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (err)
		return lpvmerr("pvm_getopt", PvmBadParam);
	return rc;
}


int
pvm_setopt(what, val)
	int what;
	int val;
{
	int rc = 0;
	int err = 0;
	int sbf, rbf;
	char buf[16];
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (pvmmytid != -1
				&& TEV_DO_TRACE(TEV_SETOPT,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_OPT, TEV_DATA_SCALAR, &what, 1, 1 );
			TEV_PACK_INT( TEV_DID_OPV, TEV_DATA_SCALAR, &val, 1, 1 );
			TEV_FIN;
		}
	}

	switch (what) {
	case PvmRoute:
		switch (val) {
		case PvmDontRoute:
		case PvmAllowDirect:
		case PvmRouteDirect:
			rc = pvmrouteopt;
#if (!defined(IMA_PGON)) && (!defined(IMA_SP2MPI)) \
		&& (!defined(IMA_AIX4SP2)) && (!defined(IMA_AIX5SP2))
			pvmrouteopt = val;
#endif
			break;

		default:
			rc = PvmBadParam;
			err = 1;
			break;
		}
		break;

	case PvmDebugMask:
		rc = pvmdebmask;
		pvmdebmask = val;
		break;

	case PvmAutoErr:
		rc = pvmautoerr;
		pvmautoerr = val;
		break;

	case PvmOutputTid:
		if (val && val != pvmmytid
		&& (val != pvmtrc.outtid || pvmctrc.outtag != pvmtrc.outtag)) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.outtid;
			pvmctrc.outtid = val;
		}
		break;

	case PvmOutputContext:
		if (pvmctrc.outtid > 0 && pvmctrc.outtid != pvmmytid
		&& val != pvmtrc.outctx) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.outctx;
			pvmctrc.outctx = val;
		}
		break;

	case PvmOutputCode:
		if (pvmctrc.outtid > 0 && pvmctrc.outtid != pvmmytid
		&& val != pvmtrc.outtag) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.outtag;
			pvmctrc.outtag = val;
		}
		break;

	case PvmTraceTid:
		if (val && val != pvmmytid
		&& (val != pvmtrc.trctid || pvmctrc.trctag != pvmtrc.trctag)) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.trctid;
			pvmctrc.trctid = val;
		}
		break;

	case PvmTraceContext:
		if (pvmctrc.trctid > 0 && pvmctrc.trctid != pvmmytid
		&& val != pvmtrc.trcctx) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.trcctx;
			pvmctrc.trcctx = val;
		}
		break;

	case PvmTraceCode:
		if (pvmctrc.trctid > 0 && pvmctrc.trctid != pvmmytid
		&& val != pvmtrc.trctag) {
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmctrc.trctag;
			pvmctrc.trctag = val;
		}
		break;

	case PvmTraceBuffer:
		rc = pvmctrc.trcbuf;
		pvmctrc.trcbuf = val;
		break;

	case PvmTraceOptions:
		switch (val) {
		case PvmTraceFull:
		case PvmTraceTime:
		case PvmTraceCount:
			rc = pvmctrc.trcopt;
			pvmctrc.trcopt = val;
			break;
		
		default:
			rc = PvmBadParam;
			err = 1;
			break;
		}
		break;

	case PvmFragSize:

#if defined(IMA_AIX4MP) || defined(IMA_AIX5MP) \
	|| defined(IMA_ALPHAMP) || defined(IMA_CSPP) \
	|| defined(IMA_HPPAMP) || defined(IMA_RS6KMP) \
	|| defined(IMA_SGIMP) || defined(IMA_SGIMP6) \
	|| defined(IMA_SGIMP64) || defined(IMA_SUNMP)

		rc = PvmBadParam;
		err = 1;

#else /* SHMEM */

#if defined(IMA_CM5) || defined(IMA_CUBE) || defined(IMA_I860) \
	|| defined(IMA_PGON) || defined(IMA_SP2MPI) \
	|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
	|| defined(IMA_BEOLIN)
		/* if (val < TDFRAGHDR + TTMSGHDR + 4 || val > 1048576) */
		if (val < TDFRAGHDR + MSGHDRLEN + 4 || val > MAXFRAGSIZE)
#else
		if (val < TDFRAGHDR + MSGHDRLEN + 4 || val > 1048576)
#endif

		{
			rc = PvmBadParam;
			err = 1;

		} else {
			rc = pvmfrgsiz;
			pvmfrgsiz = val;
		}

#endif /* SHMEM */

		break;

	case PvmResvTids:
		rc = pvmrescode;
		pvmrescode = val;
		break;

	case PvmSelfOutputTid:
		if (pvmtrc.outtid > 0 || val > 0) {
			if (rc = BEATASK)
				err = 1;
			else {
				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
				rbf = pvm_setrbuf(0);
				what = TS_OUTTID;
				pvm_pkint(&what, 1, 1);
				sprintf(buf, "%x", 0xffffffff & val);
				pvm_pkstr(buf);
				if ((rc = msendrecv(TIDPVMD, TM_SETOPT, SYSCTX_TM)) > 0) {
					pvm_freebuf(pvm_setrbuf(rbf));
					rc = pvmtrc.outtid;
					pvmtrc.outtid = val;
					pvmctrc.outtid = pvmtrc.outtid;
					pvmctrc.outctx = pvmtrc.outctx;
					pvmctrc.outtag = pvmtrc.outtag;

				} else {
					pvm_setrbuf(rbf);
					err = 1;
				}
				pvm_freebuf(pvm_setsbuf(sbf));
			}
		}
		else {
			rc = pvmtrc.outtid;
			pvmtrc.outtid = val;
			pvmctrc.outtid = pvmtrc.outtid;
			pvmctrc.outctx = pvmtrc.outctx;
			pvmctrc.outtag = pvmtrc.outtag;
		}
		break;

	case PvmSelfOutputContext:
		rc = PvmNotImpl;
		err = 1;
		break;

	case PvmSelfOutputCode:
		if (pvmtrc.outtid <= 0 || (rc = BEATASK))
			err = 1;
		else {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			what = TS_OUTTAG;
			pvm_pkint(&what, 1, 1);
			sprintf(buf, "%x", 0xffffffff & val);
			pvm_pkstr(buf);
			if ((rc = msendrecv(TIDPVMD, TM_SETOPT, SYSCTX_TM)) > 0) {
				pvm_freebuf(pvm_setrbuf(rbf));
				rc = pvmtrc.outtag;
				pvmtrc.outtag = val;
				pvmctrc.outtid = pvmtrc.outtid;
				pvmctrc.outctx = pvmtrc.outctx;
				pvmctrc.outtag = pvmtrc.outtag;

			} else {
				pvm_setrbuf(rbf);
				err = 1;
			}
			pvm_freebuf(pvm_setsbuf(sbf));
		}
		break;

	case PvmSelfTraceTid:
		if ( pvmtrc.trctid > 0 || val > 0 ) {
			if (rc = BEATASK)
				err = 1;
			else {
				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
				rbf = pvm_setrbuf(0);
				what = TS_TRCTID;
				pvm_pkint(&what, 1, 1);
				sprintf(buf, "%x", 0xffffffff & val);
				pvm_pkstr(buf);
				if ((rc = msendrecv(TIDPVMD, TM_SETOPT, SYSCTX_TM)) > 0) {
					pvm_freebuf(pvm_setrbuf(rbf));
					rc = pvmtrc.trctid;
					pvmtrc.trctid = val;
					pvmctrc.trctid = pvmtrc.trctid;
					pvmctrc.trcctx = pvmtrc.trcctx;
					pvmctrc.trctag = pvmtrc.trctag;

				} else {
					pvm_setrbuf(rbf);
					err = 1;
				}
				pvm_freebuf(pvm_setsbuf(sbf));
			}
		}
		else {
			rc = pvmtrc.trctid;
			pvmtrc.trctid = val;
			pvmctrc.trctid = pvmtrc.trctid;
			pvmctrc.trcctx = pvmtrc.trcctx;
			pvmctrc.trctag = pvmtrc.trctag;
		}
		break;

	case PvmSelfTraceContext:
		rc = PvmNotImpl;
		err = 1;
		break;

	case PvmSelfTraceCode:
		if (pvmtrc.trctid <= 0 || (rc = BEATASK))
			err = 1;
		else {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			what = TS_TRCTAG;
			pvm_pkint(&what, 1, 1);
			sprintf(buf, "%x", 0xffffffff & val);
			pvm_pkstr(buf);
			if ((rc = msendrecv(TIDPVMD, TM_SETOPT, SYSCTX_TM)) > 0) {
				pvm_freebuf(pvm_setrbuf(rbf));
				rc = pvmtrc.trctag;
				pvmtrc.trctag = val;
				pvmctrc.trctid = pvmtrc.trctid;
				pvmctrc.trcctx = pvmtrc.trcctx;
				pvmctrc.trctag = pvmtrc.trctag;

			} else {
				pvm_setrbuf(rbf);
				err = 1;
			}
			pvm_freebuf(pvm_setsbuf(sbf));
		}
		break;

	case PvmSelfTraceBuffer:
		tev_flush( 1 );
		rc = pvmtrc.trcbuf;
		pvmtrc.trcbuf = val;
		break;

	case PvmSelfTraceOptions:
		tev_flush( 1 );
		switch (val) {
		case PvmTraceFull:
		case PvmTraceTime:
		case PvmTraceCount:
			rc = pvmtrc.trcopt;
			pvmtrc.trcopt = val;
			break;

		default:
			rc = PvmBadParam;
			err = 1;
			break;
		}
		break;

	case PvmShowTids:
		rc = pvmshowtaskid;
		pvmshowtaskid = val;
		break;

	case PvmNoReset:
		if ( pvmmytid < 0 ) {
			rc = PvmBadParam;
			lpvmerr("pvm_setopt: PvmNoReset task not initialized", rc);
			err = 1;
		} else {
			rc = pvmnoreset;
			if ( val && !pvmnoreset ) {
				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataDefault));
				pvm_pkint( &pvmmytid, 1, 1 );
				if ( pvmnoresetindex = pvm_putinfo( PVMNORESETCLASS,
						pvm_getsbuf(), PvmMboxMultiInstance ) < 0 ) {
					rc = PvmSysErr;
					err = 1;
					val = 0;
				}
				pvm_freebuf(pvm_setsbuf(sbf));
			}
			else if ( !val && pvmnoreset && pvmnoresetindex >= 0 ) {
				if ( pvm_delinfo( PVMNORESETCLASS, pvmnoresetindex,
						PvmMboxDefault ) < 0 ) {
					rc = PvmSysErr;
					err = 1;
					val = 0;
				}
				pvmnoresetindex = -1;
			}
			pvmnoreset = val;
		}
		break;

#if defined(IMA_AIX4MP) || defined(IMA_AIX5MP) \
	|| defined(IMA_ALPHAMP) || defined(IMA_CSPP) \
	|| defined(IMA_HPPAMP) || defined(IMA_RS6KMP) \
	|| defined(IMA_SGIMP) || defined(IMA_SGIMP6) \
	|| defined(IMA_SGIMP64) || defined(IMA_SUNMP)

	case PvmPollTime:
		rc = pvmpolltime;
		if (val < 0) {
			rc = PvmBadParam;
			err = 1;
			break;
		}
		pvmpolltime = val;
		break;

	case PvmPollType:
		rc = pvmpolltype;
		if ((val != PvmPollConstant) && (val != PvmPollSleep)) {
			rc = PvmBadParam;
			err = 1;
			break;
		}
		pvmpolltype = val;
		break;

#else /* SHMEM */

	case PvmPollTime:
	case PvmPollType:
		rc = PvmNotImpl;
		err = 1;
		break;

#endif /* SHMEM */

	default:
		rc = PvmBadParam;
		err = 1;
		break;
	}

	if (TEV_AMEXCL) {
		if (pvmmytid != -1
				&& TEV_DO_TRACE(TEV_SETOPT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_OPV, TEV_DATA_SCALAR, &rc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (err)
		return lpvmerr("pvm_setopt", rc);
	return rc;
}


int
pvmsettaskname( name )
	char *name;
{
	if ( name == NULL )
		return lpvmerr( "pvmsettaskname", PvmBadParam );

	if ( pvmmytid != -1 )
		return lpvmerr( "pvmsettaskname", PvmExists );

	if ( pvmmytaskname != NULL )
	{
		PVM_FREE( pvmmytaskname );
		pvmmytaskname = (char *) NULL;
	}

	pvmmytaskname = STRALLOC( name );

	return 0;
}


int
pvm_getnoresets( tids, ntids )
	int **tids;
	int *ntids;
{
	static int *tidlist = (int *) NULL;
	static int num = 0;

	int index;
	int done;
	int save;
	int mid;
	int cnt;

	/* Allocate Initial Tids List */

	if ( !tidlist )
	{
		num = 16;
		tidlist = TALLOC( num, int, "int" );
	}

	/* Save Receive Buffer */

	save = pvm_setrbuf( 0 );

	/* Search for No Resets */

	index = 0;
	done = 0;
	cnt = 0;

	while ( !done )
	{
		if ( pvm_recvinfo( PVMNORESETCLASS, index, PvmMboxFirstAvail )
				<= 0 )
		{
			done++;
		}

		else
		{
			if ( cnt >= num )
			{
				num *= 2;

				tidlist = TREALLOC( tidlist, num, int );
			}

			pvm_upkint( &(tidlist[cnt++]), 1, 1 );

			index++;
		}
	}

	/* Restore Receive Buffer */

	pvm_setrbuf( save );

	/* Set Return Values */

	if ( tids != NULL )
		*tids = tidlist;

	if ( ntids != NULL )
		*ntids = cnt;

	return( PvmOk );
}


int
pvm_addhosts(names, count, svp)
	char **names;	/* host name vector */
	int count;		/* length of names */
	int *svp;		/* status vector return */
{
	int sbf, rbf;
	int cc;
	int i;
	int *sv;		/* status vector */
	char *buf;
	int tmp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_ADDHOSTS,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_HNL, TEV_DATA_ARRAY,
				names, count, 1 );
			TEV_FIN;
		}
	}

	if (count < 1 || count > (TIDHOST >> (ffs(TIDHOST) - 1))) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			pvm_pkint(&count, 1, 1);
			for (i = 0; i < count; i++)
				pvm_pkstr(names[i]);

			if (pvmschedtid)
				cc = msendrecv(pvmschedtid, SM_ADDHOST, PvmBaseContext);
			else
				cc = msendrecv(TIDPVMD, TM_ADDHOST, SYSCTX_TM);
			if (cc > 0) {
				pvm_upkint(&cc, 1, 1);
				if (cc >= 0) {
					if (cc == count) {
						pvm_upkint(&cc, 1, 1);	/* toss narches */
						sv = TALLOC(count, int, "sv1");
						cc = 0;
						for (i = 0; i < count; i++) {
							pvm_upkint(&sv[i], 1, 1);
							/* toss name, arch, speed, ds */
							pvmupkstralloc(&buf);	PVM_FREE(buf);
							pvmupkstralloc(&buf);	PVM_FREE(buf);
							pvm_upkint(&tmp, 1, 1);
							pvm_upkint(&tmp, 1, 1);
							if (sv[i] >= 0)
								cc++;
						}
						if (svp)
							BCOPY((char*)sv, (char*)svp, count * sizeof(int));
						PVM_FREE(sv);

					} else {
						pvmlogprintf("pvm_addhosts() sent count %d received count %d\n",
								count, cc);
						cc = PvmOutOfRes;
					}
				}
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_ADDHOSTS,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_addhosts", cc);
	return cc;
}


int
pvm_config(nhostp, narchp, hostp)
	int *nhostp;
	int *narchp;
	struct pvmhostinfo **hostp;
{
	int sbf, rbf, cc;
	static int nhost = 0;
	static int narch = 0;
	static struct pvmhostinfo *hlist = 0;
	int i;
	/* char buf[256]; XXX static limit, argh (Not Any More! :-) JAK */
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_CONFIG,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (hlist) {
		while (nhost-- > 0) {
			PVM_FREE(hlist[nhost].hi_name);
			PVM_FREE(hlist[nhost].hi_arch);
		}
		PVM_FREE(hlist);
		hlist = 0;
		nhost = 0;
	}
	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		if (pvmschedtid)
			cc = msendrecv(pvmschedtid, SM_CONFIG, PvmBaseContext);
		else
			cc = msendrecv(TIDPVMD, TM_CONFIG, SYSCTX_TM);
		if (cc > 0) {
			pvm_upkint(&nhost, 1, 1);
			pvm_upkint(&narch, 1, 1);
			hlist = TALLOC(nhost, struct pvmhostinfo, "hi");
			for (i = 0; i < nhost; i++) {
				pvm_upkint(&hlist[i].hi_tid, 1, 1);
				pvmupkstralloc(&(hlist[i].hi_name));
				pvmupkstralloc(&(hlist[i].hi_arch));
				pvm_upkint(&hlist[i].hi_speed, 1, 1);
				pvm_upkint(&hlist[i].hi_dsig, 1, 1);
			}
			pvm_freebuf(pvm_setrbuf(rbf));
			if (nhostp)
				*nhostp = nhost;
			if (narchp)
				*narchp = narch;
			if (hostp)
				*hostp = hlist;
			cc = 0;
		}
		pvm_freebuf(pvm_setsbuf(sbf));
		pvm_setrbuf(rbf);
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_CONFIG,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_NH, TEV_DATA_SCALAR, &nhost, 1, 1 );
			TEV_PACK_INT( TEV_DID_NA, TEV_DATA_SCALAR, &narch, 1, 1 );
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_config", cc);
	return cc;
}


int
pvm_delhosts(names, count, svp)
	char **names;
	int count;
	int *svp;		/* status vector return */
{
	int sbf, rbf;
	int cc;
	int i;
	int *sv;		/* return values */
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_DELHOSTS,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_HNL, TEV_DATA_ARRAY,
				names, count, 1 );
			TEV_FIN;
		}
	}

	if (count < 1 || count > (TIDHOST >> (ffs(TIDHOST) - 1))) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			pvm_pkint(&count, 1, 1);
			for (i = 0; i < count; i++)
				pvm_pkstr(names[i]);

			if (pvmschedtid)
				cc = msendrecv(pvmschedtid, SM_DELHOST, PvmBaseContext);
			else
				cc = msendrecv(TIDPVMD, TM_DELHOST, SYSCTX_TM);
			if (cc > 0) {
				pvm_upkint(&cc, 1, 1);
				if (cc >= 0) {
					if (cc == count) {
						sv = TALLOC(count, int, "sv2");
						pvm_upkint(sv, count, 1);
						cc = 0;
						for (i = count; i-- > 0; )
							if (sv[i] >= 0)
								cc++;
						if (svp)
							BCOPY((char*)sv, (char*)svp, count * sizeof(int));
						PVM_FREE(sv);

					} else {
						pvmlogprintf("pvm_delhosts() sent count %d received count %d\n",
								count, cc);
						cc = PvmOutOfRes;
					}
				}
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_DELHOSTS,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_delhosts", cc);
	return cc;
}


int
pvm_exit()
{
	int i, lndh;
	int sbf, rbf;
	int cc = 0;
	struct waitc *wp, *wp2;
	struct pmsg *up;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_EXIT,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (pvmmytid != -1) {
		wp = waitlist->wa_link;
		while (wp != waitlist) {
			wp2 = wp->wa_link;
			if (wp->wa_kind == WT_TASKX) {
				up = wp->wa_mesg;
				wp->wa_mesg = 0;
				mesg_input(up);
				LISTDELETE(wp, wa_link, wa_rlink);
			}
			wp = wp2;
		}

		while (pvm_nrecv(-1, -1) > 0) ;		/* XXX attempt to flush messages */

		fflush(stderr);
		fflush(stdout);

		tev_flush( 1 );

		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		if ((cc = msendrecv(TIDPVMD, TM_EXIT, SYSCTX_TM)) > 0) {
			pvm_freebuf(pvm_setrbuf(rbf));
			cc = 0;

		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));

		/*
		*  remove all task's message handler functions
		*/
		lndh = ndhandles - 1;
		for (i = lndh; i >= 0; i--){
			pvm_delmhf(i);
		}

		pvmendtask();
	}

	if (TEV_AMEXCL) {
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_exit", cc);
	return cc;
}


int
pvm_halt()
{
	int cc, sbf, rbf;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_HALT,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		cc = (msendrecv(TIDPVMD, TM_HALT, SYSCTX_TM) < 0) ? 0 : PvmSysErr;
		pvm_freebuf(pvm_setsbuf(sbf));
		pvm_setrbuf(rbf);
	}

	if (TEV_AMEXCL) {
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_halt", cc);
	return cc;
}


int
pvm_kill(tid)
	int tid;
{
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_KILL,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_TT, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_FIN;
		}
	}

	cc = pvm_sendsig(tid, SIGTERM);

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_KILL,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	if (cc < 0)
		lpvmerr("pvm_kill", cc);
	return cc;
}


int
pvm_mcast(tids, count, tag)
	int *tids;		/* dest tasks */
	int count;		/* number of tids */
	int tag;		/* message type tag */
{
	int cc;			/* for return codes */
	int i;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_MCAST,TEV_EVENT_ENTRY)) {
			int nb = -1;
			pvm_bufinfo(
					( pvmtrcsbfsave ) ? pvmtrcsbfsave : pvmsbuf->m_mid,
					&nb, (int *) NULL, (int *) NULL );
			TEV_PACK_INT( TEV_DID_MNB, TEV_DATA_SCALAR, &nb, 1, 1 );
			TEV_PACK_INT( TEV_DID_MC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_MCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			TEV_PACK_INT( TEV_DID_MDL, TEV_DATA_ARRAY, tids, count, 1 );
			TEV_FIN;
		}
	}

	/* sanity check args and sendable message */

	if (!(cc = BEATASK)) {
		if (!pvmsbuf)
			cc = PvmNoBuf;
		else if (count < 0 || (!pvmrescode && (tag & ~0x7fffffff)))
				cc = PvmBadParam;
		else if (!pvmrescode)
			for (i = count; i-- > 0; )
				if (!TIDISTASK(tids[i])) {
					cc = PvmBadParam;
					break;
				}

	}

	if (!cc && count > 0)
		cc = pvmmcast(pvmsbuf->m_mid, tids, count, tag);

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_MCAST,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_mcast", cc);
	return cc;
}

#ifdef WIN32
#ifndef IMA_WIN32_WATCOM
extern char **environ;
#endif
#endif

int
pvm_mytid()
{
	int cc;
	char **ep=0;

	TEV_DECLS;



	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_MYTID,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (!(cc = BEATASK))
		cc = pvmmytid;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_MYTID,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MT, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_mytid", cc);
	return cc;
}


int
pvm_mstat(host)
	char *host;
{
	int sbf, rbf, cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_MSTAT,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_HN, TEV_DATA_SCALAR,
				host ? host : "", 1, 1 );
			TEV_FIN;
		}
	}
	if (!host || !*host) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			pvm_pkstr(host);
			if ((cc = msendrecv(TIDPVMD, TM_MSTAT, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_MSTAT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_HS, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0 && cc != PvmNoHost && cc != PvmHostFail)
		lpvmerr("pvm_mstat", cc);
	return cc;
}


int
pvm_notify(what, code, count, vals)
	int what;
	int code;
	int count;
	int *vals;
{
	static struct timeval ztv = { 0, 0 };

	int sbf;
	int cc;
	int numtid;
	int dosend = 0;
	struct waitc *wp;
	int flags;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_NOTIFY,TEV_EVENT_ENTRY)) {
			int tevcount;
			TEV_PACK_INT( TEV_DID_NE, TEV_DATA_SCALAR, &what, 1, 1 );
			TEV_PACK_INT( TEV_DID_NMC, TEV_DATA_SCALAR, &code, 1, 1 );
			tevcount = ( what != PvmHostAdd ) ? count : 0;
			TEV_PACK_INT( TEV_DID_NTL, TEV_DATA_ARRAY,
				vals, tevcount, 1 );
			TEV_FIN;
		}
	}

	flags = what;
	what &= ~PvmNotifyCancel;

	if (!(cc = BEATASK)) {
		if (!pvmrescode && (code & ~0x7fffffff)) {
			cc = PvmBadParam;

		} else {
			switch (what) {

			case PvmHostDelete:
				if (count < 1)
					cc = PvmBadParam;
				else {
					numtid = count;
					dosend = 1;
				}
				break;

			case PvmTaskExit:
				if (count < 1)
					cc = PvmBadParam;
				else
					for (numtid = count; numtid-- > 0; )
						if (!TIDISTASK(vals[numtid])) {
							cc = PvmBadParam;
							break;
						}
				if (!cc) {
					for (numtid = count; numtid-- > 0; )
						if (vals[numtid] == pvmmytid) {
							wp = wait_new(WT_TASKX);
							wp->wa_tid = pvmmytid;
							wp->wa_on = pvmmytid;
							sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
							pvm_pkint(&pvmmytid, 1, 1);
							sbf = pvm_setsbuf(sbf);
							wp->wa_mesg = midtobuf(sbf);
							wp->wa_mesg->m_ctx = pvmmyctx;
							wp->wa_mesg->m_tag = code;
						}
					numtid = count;
					dosend = 1;
				}
				break;

			case PvmHostAdd:
				numtid = 0;
				vals = &numtid;
				dosend = 1;
				break;

			case PvmRouteAdd:
				FORLIST (wp, waitlist, wa_link)
					if (wp->wa_kind == WT_ROUTEA
					&& wp->wa_mesg->m_ctx == pvmmyctx
					&& wp->wa_mesg->m_tag == code)
						break;

				if (count == 0 || (flags & PvmNotifyCancel)) {
					if (wp != waitlist)
						wait_delete(wp);

				} else {
					if (wp == waitlist) {
						wp = wait_new(WT_ROUTEA);
						wp->wa_tid = pvmmytid;
						wp->wa_on = pvmmytid;
						sbf = pvm_mkbuf(PvmDataFoo);
						wp->wa_mesg = midtobuf(sbf);
						wp->wa_mesg->m_ctx = pvmmyctx;
						wp->wa_mesg->m_tag = code;
					}
					wp->wa_count = count;
				}
				break;

			case PvmRouteDelete:
				if (count < 1)
					cc = PvmBadParam;
				else
					for (numtid = count; numtid-- > 0; )
						if (!TIDISTASK(vals[numtid])) {
							cc = PvmBadParam;
							break;
						}
				if (!cc) {
					for (numtid = count; numtid-- > 0; )
						post_routedelete(vals[numtid], pvmmyctx, code);
				}
				break;

			default:
				cc = PvmBadParam;
				break;
			}

			if (dosend) {
				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
				pvm_pkint(&flags, 1, 1);
				pvm_pkint(&pvmmyctx, 1, 1);
				pvm_pkint(&code, 1, 1);
				pvm_pkint(&count, 1, 1);
				pvm_pkint(vals, numtid, 1);

				if (pvmschedtid)
					cc = mroute(pvmsbuf->m_mid, pvmschedtid, SM_NOTIFY, &ztv);
				else
					cc = mroute(pvmsbuf->m_mid, TIDPVMD, TM_NOTIFY, &ztv);
				pvm_freebuf(pvm_setsbuf(sbf));
				if (cc > 0)
					cc = 0;
			}
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_NOTIFY,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_notify", cc);
	return cc;
}


int
pvm_nrecv(tid, tag)
	int tid;
	int tag;
{
	struct pmsg *up;
	struct pmsg *bestup;
	int bestcc = 0;
	int cc;
	int alrdy = 0;
	TEV_DECLS
	static struct timeval ztv = { 0, 0 };

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_NRECV,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		if (pvmrbuf)
			umbuf_free(pvmrbuf);
		pvmrbuf = 0;

		for (up = pvmrxlist->m_link; 1; up = up->m_link) {
			if (up == pvmrxlist && bestcc)
				break;
			while (up == pvmrxlist) {
				if (alrdy) {
					cc = 0;
					goto done;
				}
				up = up->m_rlink;
				if ((cc = mroute(0, 0, 0, &ztv)) < 0)
					goto done;
				up = up->m_link;
				alrdy = 1;
			}

			if ((cc = recv_match(up->m_mid, tid, tag)) < 0)
				goto done;
			if (cc == 1) {
				bestup = up;
				break;
			}
			if (cc > bestcc) {
				bestcc = cc;
				bestup = up;
			}
		}

		LISTDELETE(bestup, m_link, m_rlink);
		bestup->m_flag &= ~MM_UPACK;
		if (!(cc = pvm_setrbuf(bestup->m_mid)))
			cc = bestup->m_mid;
	}

done:
	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_NRECV,TEV_EVENT_EXIT)) {
			int nb, mc, src;
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &cc, 1, 1 );
			if (cc > 0)
				pvm_bufinfo(cc, &nb, &mc, &src);
			else
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
		lpvmerr("pvm_nrecv", cc);
	return cc;
}


int
pvm_parent()
{
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PARENT,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (!(cc = BEATASK))
		cc = pvmmyptid ? pvmmyptid : PvmNoParent;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PARENT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_PT, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0 && cc != PvmNoParent && cc != PvmParentNotSet )
		lpvmerr("pvm_parent", cc);
	return cc;
}


int
pvm_probe(tid, tag)
	int tid;
	int tag;
{
	struct pmsg *up;
	struct pmsg *bestup;
	int bestcc = 0;
	int cc;
	int alrdy = 0;
	TEV_DECLS
	static struct timeval ztv = { 0, 0 };

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PROBE,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		for (up = pvmrxlist->m_link; 1; up = up->m_link) {
			if (up == pvmrxlist && bestcc)
				break;
			while (up == pvmrxlist) {
				if (alrdy) {
					cc = 0;
					goto done;
				}
				up = up->m_rlink;
				if ((cc = mroute(0, 0, 0, &ztv)) < 0)
					goto done;
				up = up->m_link;
				alrdy = 1;
			}

			if ((cc = recv_match(up->m_mid, tid, tag)) < 0)
				goto done;
			if (cc == 1) {
				bestup = up;
				break;
			}
			if (cc > bestcc) {
				bestcc = cc;
				bestup = up;
			}
		}
		bestup->m_flag &= ~MM_UPACK;
		cc = bestup->m_mid;
	}

done:
	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PROBE,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_probe", cc);
	return cc;
}


int
pvm_pstat(tid)
	int tid;	/* task */
{
	int sbf, rbf;
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PSTAT,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_TT, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		if (!TIDISTASK(tid))
			cc = PvmBadParam;

		else {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			pvm_pkint(&tid, 1, 1);
			if ((cc = msendrecv(TIDPVMD, TM_PSTAT, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PSTAT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_TST, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0 && cc != PvmNoTask)
		lpvmerr("pvm_pstat", cc);
	return cc;
}


int
pvm_recv(tid, tag)
	int tid;
	int tag;
{
	struct pmsg *up;
	struct pmsg *bestup;
	int bestcc = 0;
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_RECV,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		if (pvmrbuf)
			umbuf_free(pvmrbuf);
		pvmrbuf = 0;

		for (up = pvmrxlist->m_link; 1; up = up->m_link) {
			if (up == pvmrxlist && bestcc)
				break;
			while (up == pvmrxlist) {
				up = up->m_rlink;
				if ((cc = mroute(0, 0, 0, (struct timeval *)0)) < 0)
					goto done;
#if defined(IMA_PGON) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
			/* bypass matching of messages when in a precv that completed */
				if (pvm_inprecv && pvm_inprecv->complete)
					return 0;
#endif
				up = up->m_link;
			}


			if ((cc = recv_match(up->m_mid, tid, tag)) < 0)
				goto done;
			if (cc == 1) {
				bestup = up;
				break;
			}
			if (cc > bestcc) {
				bestcc = cc;
				bestup = up;
			}
		}

		LISTDELETE(bestup, m_link, m_rlink);
		bestup->m_flag &= ~MM_UPACK;
		if (!(cc = pvm_setrbuf(bestup->m_mid)))
			cc = bestup->m_mid;
	}

done:
	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_RECV,TEV_EVENT_EXIT)) {
			int nb, mc, src;
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &cc, 1, 1 );
			if (cc > 0)
				pvm_bufinfo(cc, &nb, &mc, &src);
			else
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
		lpvmerr("pvm_recv", cc);
	return cc;
}


int (*
pvm_recvf(new))()
	int (*new)__ProtoGlarp__((int,int,int));
{
	int (*old)() = recv_match;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_RECVF,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	recv_match = new ? new : def_match;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_RECVF,TEV_EVENT_EXIT))
			TEV_FIN;
		TEV_ENDEXCL;
	}

	return old;
}


int
pvm_send(tid, tag)
	int tid;	/* dest task */
	int tag;	/* type code */
{
	static struct timeval ztv = { 0, 0 };

	int tmp;
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SEND,TEV_EVENT_ENTRY)) {
			int nb = -1;
			pvm_bufinfo(
					( pvmtrcsbfsave ) ? pvmtrcsbfsave : pvmsbuf->m_mid,
					&nb, (int *) NULL, (int *) NULL );
			TEV_PACK_INT( TEV_DID_MNB, TEV_DATA_SCALAR, &nb, 1, 1 );
			TEV_PACK_INT( TEV_DID_DST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_MC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_MCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		if ( tid == PVM_TRACE_TID && tag == PVM_TRACE_CODE ) {
			if ( TEV_MASK_CHECK( pvmtrc.tmask, TEV_USER_DEFINED ) ) {
				/* Schlopp In End of Event Marker */
				enc_trc_fin(pvmsbuf);
				/* Send to Proper Tracer Destination */
				pvmsbuf->m_ctx = pvmtrc.trcctx;
				if ((cc = mroute(pvmsbuf->m_mid, pvmtrc.trctid,
						pvmtrc.trctag, &ztv)) > 0)
					cc = 0;
			}
			else
				cc = 0;
		}
		else if (!pvmrescode
				&& (!TIDISTASK(tid) || (tag & ~0x7fffffff)))
			cc = PvmBadParam;
		else
			if (!pvmsbuf)
				cc = PvmNoBuf;
			else {

/* XXX short-ckt to us should go here.  maybe can inc frag chain
   XXX count and make new message, put on pvmrxlist. */
				pvmsbuf->m_ctx = pvmmyctx;
				if ((cc = mroute(pvmsbuf->m_mid, tid, tag, &ztv)) > 0)
					cc = 0;
			}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SEND,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_send", cc);
	return cc;
}


int
pvm_sendsig(tid, signum)
	int tid;
	int signum;
{
	int cc;
	int sbf, rbf;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SENDSIG,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_TT, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_SN, TEV_DATA_SCALAR, &signum, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		if (!TIDISTASK(tid))
			cc = PvmBadParam;

		else {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);

			pvm_pkint(&tid, 1, 1);
			pvm_pkint(&signum, 1, 1);
			if ((cc = msendrecv(TIDPVMD, TM_SENDSIG, SYSCTX_TM)) > 0) {
				pvm_freebuf(pvm_setrbuf(rbf));
				cc = 0;

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SENDSIG,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_sendsig", cc);
	return cc;
}


/*	bubble()
*
*	Move nonnegative els to head of array, negative ones to end.
*	Returns number of nonnegative els.
*/

static int
bubble(n, a)
	int n;			/* length of a */
	int *a;
{
	int r, w, t;

	for (w = r = 0; r < n; r++) {
		if (a[w] < 0) {
			if (a[r] >= 0) {
				t = a[w];
				a[w] = a[r];
				a[r] = t;
				w++;
			}

		} else {
			w++;
		}
	}
	return w;
}


#if	defined(IMA_I860) || defined(IMA_CM5)
static int
pvmgetenvars(ep)
	char ***ep;
{
	return 0;
}

#else	/*defined(IMA_I860) || defined(IMA_CM5)*/
static int
pvmgetenvars(ep)
	char ***ep;
{
	char **xpl;			/* vars to export */
	int mxpl;			/* cur length of xpl */
	int nxpl;			/* num vars found */
	char buf[200];
	char *p, *q;
	int n;

	if (p = getenv("PVM_EXPORT")) {
		mxpl = 5;
		xpl = TALLOC(mxpl, char *, "env");
		xpl[0] = p - 11;
		nxpl = 1;
		while (1) {
			while (*p == ':')
				p++;
			if (!*p)
				break;
			n = (q = CINDEX(p, ':')) ? q - p : strlen(p);
			strncpy(buf, p, n);
			buf[n] = 0;
			if (q = getenv(buf)) {
				if (nxpl == mxpl) {
					mxpl += mxpl / 2 + 1;
					xpl = TREALLOC(xpl, mxpl, char *);
				}
				xpl[nxpl++] = q - n - 1;
			}
			p += n;
		}
		*ep = xpl;
		return nxpl;

	} else {
		return 0;
	}
}
#endif	/*defined(IMA_I860) || defined(IMA_CM5)*/


int
pvm_spawn(file, argv, flags, where, count, tids)
	char *file;
	char **argv;
	int flags;
	char *where;
	int count;
	int *tids;
{
	int sbf, rbf;	/* temp for current tx, rx msgs */
	int ictx;
	int cc;
	int i, n;
	char **ep;
	int *tidlist = 0;
	char *p;
	char buf[TEV_MASK_LENGTH + 20];
	TEV_DECLS

	if (p = getenv("PVMTASK"))
		i = pvmstrtoi(p) | flags;
	else
		i = flags;

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SPAWN,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_SE, TEV_DATA_SCALAR,
				file ? file : "", 1, 1 );
			TEV_PACK_STRING( TEV_DID_SW, TEV_DATA_SCALAR,
				where ? where : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_SF, TEV_DATA_SCALAR, &i, 1, 1 );
			TEV_PACK_INT( TEV_DID_SC, TEV_DATA_SCALAR, &count, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		if (count < 1)
			cc = PvmBadParam;

		else {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);

			pvm_pkstr(file);
			pvm_pkint(&i, 1, 1);
			pvm_pkstr(where ? where : "");
			pvm_pkint(&count, 1, 1);
			if (argv)
				for (n = 0; argv[n]; n++);
			else
				n = 0;
			pvm_pkint(&n, 1, 1);
			for (i = 0; i < n; i++)
				pvm_pkstr(argv[i]);

			pvm_pkint( &(pvmctrc.outtid), 1, 1 );
			pvm_pkint( &(pvmctrc.outctx), 1, 1 );
			pvm_pkint( &(pvmctrc.outtag), 1, 1 );
			pvm_pkint( &(pvmctrc.trctid), 1, 1 );
			pvm_pkint( &(pvmctrc.trcctx), 1, 1 );
			pvm_pkint( &(pvmctrc.trctag), 1, 1 );

			n = pvmgetenvars(&ep) + 4;
			pvm_pkint(&n, 1, 1);
			n -= 4;
			sprintf(buf, "PVMTMASK=%s", pvmctrc.tmask);
			pvm_pkstr(buf);
			sprintf(buf, "PVMTRCBUF=%d", pvmctrc.trcbuf);
			pvm_pkstr(buf);
			sprintf(buf, "PVMTRCOPT=%d", pvmctrc.trcopt);
			pvm_pkstr(buf);
			sprintf(buf, "PVMCTX=0x%x", pvmmyctx);
			pvm_pkstr(buf);
			if (n > 0) {
				for (i = 0; i < n; i++)
					pvm_pkstr(ep[i]);
				PVM_FREE(ep);
			}

			if (pvmschedtid)
				cc = msendrecv(pvmschedtid, SM_SPAWN, PvmBaseContext);
			else
				cc = msendrecv(TIDPVMD, TM_SPAWN, SYSCTX_TM);
			if (cc > 0) {
				pvm_upkint(&cc, 1, 1);
				if (cc == count) {
					tidlist = tids ? tids : TALLOC(count, int, "xxx");
					pvm_upkint(tidlist, cc, 1);
					cc = bubble(cc, tidlist);
				}
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));

			if (cc > 0) {
				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
				pvm_pkint(&cc, 1, 1);
				pvm_pkint(tidlist, cc, 1);
				ictx = pvm_setcontext(SYSCTX_TC);
				pvmmcast(pvmsbuf->m_mid, tidlist, cc, TC_SIBLINGS);
				pvm_setcontext(ictx);
				pvm_freebuf(pvm_setsbuf(sbf));
			}
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SPAWN,TEV_EVENT_EXIT)) {
			int tevcount;
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			tevcount = ( cc > 0 ) ? cc : 0;
			TEV_PACK_INT( TEV_DID_STL, TEV_DATA_ARRAY,
				tidlist, tevcount, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (tidlist != tids && tidlist != NULL)
		PVM_FREE(tidlist);

	if (cc < 0)
		lpvmerr("pvm_spawn", cc);
	return cc;
}


int
pvm_tasks(where, ntaskp, taskp)
	int where;					/* which host or 0 for all */
	int *ntaskp;
	struct pvmtaskinfo **taskp;
{
	int cc, ec, sbf, rbf, ae;
	static struct pvmtaskinfo *tlist = 0;
	static int ntask = 0;
	int len1 = 5, len2 = 3;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_TASKS,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_TW, TEV_DATA_SCALAR, &where, 1, 1 );
			TEV_FIN;
		}
	}

	if (tlist) {
		while (ntask-- > 0)
			PVM_FREE(tlist[ntask].ti_a_out);
		PVM_FREE(tlist);
		tlist = 0;
		ntask = 0;
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		pvm_pkint(&where, 1, 1);

		if (pvmschedtid)
			cc = msendrecv(pvmschedtid, SM_TASK, PvmBaseContext);
		else
			cc = msendrecv(TIDPVMD, TM_TASK, SYSCTX_TM);
		if (cc > 0) {
			if (!(cc = pvm_upkint(&ec, 1, 1))
			&& (cc = ec) >= 0) {
				tlist = TALLOC(len1, struct pvmtaskinfo, "ti");
				ae = pvm_setopt(PvmAutoErr, 0);
				ntask = 0;
				while (!pvm_upkint(&tlist[ntask].ti_tid, 1, 1)) {
					pvm_upkint(&tlist[ntask].ti_ptid, 1, 1);
					pvm_upkint(&tlist[ntask].ti_host, 1, 1);
					pvm_upkint(&tlist[ntask].ti_flag, 1, 1);
					pvmupkstralloc(&(tlist[ntask].ti_a_out));
					pvm_upkint(&tlist[ntask].ti_pid, 1, 1);
					ntask++;
					if (ntask == len1) {
						len1 += len2;
						len2 = ntask;
						tlist = TREALLOC(tlist, len1, struct pvmtaskinfo);
					}
				}
				pvm_setopt(PvmAutoErr, ae);
				cc = 0;
			}
			pvm_freebuf(pvm_setrbuf(rbf));
			if (ntaskp)
				*ntaskp = ntask;
			if (taskp)
				*taskp = tlist;
		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_TASKS,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_PACK_INT( TEV_DID_TNT, TEV_DATA_SCALAR, &ntask, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_tasks", cc);
	return cc;
}


int
pvm_tickle(narg, argp, nresp, resp)
	int narg;
	int *argp;
	int *nresp;
	int *resp;
{
	int cc;
	int sbf, rbf;
	int nres;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_TICKLE,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_TA, TEV_DATA_ARRAY, argp, narg, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		if (narg < 1 || narg > 10)
			cc = PvmBadParam;

		else {

			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			pvm_pkint(&narg, 1, 1);
			pvm_pkint(argp, narg, 1);
			if ((cc = msendrecv(TIDPVMD, TM_TICKLE, SYSCTX_TM)) > 0) {
				pvm_upkint(&nres, 1, 1);
				if (nresp)
					*nresp = nres;
				if (resp)
					pvm_upkint(resp, nres, 1);
				pvm_freebuf(pvm_setrbuf(rbf));
				cc = 0;

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_TICKLE,TEV_EVENT_EXIT)) {
			int tevcount;
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			tevcount = ( resp ) ? nres : 0;
			TEV_PACK_INT( TEV_DID_TR, TEV_DATA_ARRAY,
				resp, tevcount, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_tickle", cc);
	return cc;
}


int
pvm_tidtohost(tid)
	int tid;
{
	return (tid & TIDHOST);
}


int
pvm_trecv(tid, tag, tmout)
	int tid;				/* source tid to match */
	int tag;				/* message tag to match */
	struct timeval *tmout;	/* time to wait for match */
{
	struct pmsg *up;
	struct pmsg *bestup;
	int bestcc = 0;
	int cc;
	struct timeval tin, tnow;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_TRECV,TEV_EVENT_ENTRY)) {
			int ts, tu;
			TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR,
					&pvmmyctx, 1, 1 );
			if (tmout) {
				ts = tmout->tv_sec;
				tu = tmout->tv_usec;
			}
			else
				ts = tu = -1;
			TEV_PACK_INT( TEV_DID_MTS, TEV_DATA_SCALAR, &ts, 1, 1 );
			TEV_PACK_INT( TEV_DID_MTU, TEV_DATA_SCALAR, &tu, 1, 1 );
			TEV_FIN;
		}
	}

	pvmgetclock(&tin);

	if (!(cc = BEATASK)) {
		if (pvmrbuf)
			umbuf_free(pvmrbuf);
		pvmrbuf = 0;

		for (up = pvmrxlist->m_link; 1; up = up->m_link) {
			if (up == pvmrxlist && bestcc)
				break;
			while (up == pvmrxlist) {
				up = up->m_rlink;
				if (tmout) {
					pvmgetclock(&tnow);
					TVXSUBY(&tnow, &tnow, &tin);
					if (TVXLTY(tmout, &tnow)) {
						if (bestcc)
							goto fnd;
						cc = 0;
						if (!TVISSET(tmout)) {
							if ((cc = mroute(0, 0, 0, tmout)) <= 0)
								goto done;
						} else
							goto done;
					} else {
						TVXSUBY(&tnow, tmout, &tnow);
						if ((cc = mroute(0, 0, 0, &tnow)) < 0)
							goto done;
					}

				} else {
					if ((cc = mroute(0, 0, 0, (struct timeval *)0)) < 0)
						goto done;
				}
				up = up->m_link;
			}

			if ((cc = recv_match(up->m_mid, tid, tag)) < 0)
				goto done;
			if (cc == 1) {
				bestup = up;
				break;
			}
			if (cc > bestcc) {
				bestcc = cc;
				bestup = up;
			}
		}

fnd:
		LISTDELETE(bestup, m_link, m_rlink);
		bestup->m_flag &= ~MM_UPACK;
		if (!(cc = pvm_setrbuf(bestup->m_mid)))
			cc = bestup->m_mid;
	}

done:
	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_TRECV,TEV_EVENT_EXIT)) {
			int nb, mc, src;
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &cc, 1, 1 );
			if (cc > 0)
				pvm_bufinfo(cc, &nb, &mc, &src);
			else
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
		lpvmerr("pvm_trecv", cc);
	return cc;
}


char *
pvm_version()
{
	static char *_pvm_version_str = PVM_VER;
	return(_pvm_version_str);
}


int
pvm_reg_rm(hip)
	struct pvmhostinfo **hip;
{
	int old_sched;
	int cc;
	int sbf;
	int rbf;
	/* char buf[256]; XXX static limit, argh (Not Any More! :-) JAK */
	static struct pvmhostinfo *hin = 0;
	static int mbindex = -1;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_REG_RM,TEV_EVENT_ENTRY)) {
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);

		if (pvmschedtid)
			cc = msendrecv(pvmschedtid, SM_SCHED, PvmBaseContext);
		else
			cc = msendrecv(TIDPVMD, TM_SCHED, SYSCTX_TM);
		if (cc > 0) {
			pvm_upkint(&cc, 1, 1);
			if (cc >= 0) {
				if (hin) {
					PVM_FREE(hin->hi_name);
					PVM_FREE(hin->hi_arch);

				} else
					hin = TALLOC(1, struct pvmhostinfo, "hi");
				pvm_upkint(&hin->hi_tid, 1, 1);
				pvmupkstralloc(&(hin->hi_name));
				pvmupkstralloc(&(hin->hi_arch));
				pvm_upkint(&hin->hi_speed, 1, 1);
				if (hip)
					*hip = hin;

				if ( mbindex >= 0 )
					pvm_delinfo( PVMRMCLASS, mbindex, PvmMboxDefault );
				pvm_setsbuf(pvm_mkbuf(PvmDataDefault));
				pvm_pkint(&pvmmytid,1,1);
				mbindex = pvm_putinfo( PVMRMCLASS, pvm_getsbuf(),
					PvmMboxMultiInstance );
			}
			pvm_freebuf(pvm_setrbuf(rbf));

		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_REG_RM,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_reg_rm", cc);
	return cc;
}


int
pvm_reg_tasker()
{
	static int imit = 0;		/* i'm the tasker */
	static int mbindex = -1;	/* mbox index */

	int cc;
	int sbf;
	int rbf;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_REG_TASKER,TEV_EVENT_ENTRY)) {
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);

		cc = imit ? 0 : 1;
		pvm_pkint(&cc, 1, 1);
		cc = msendrecv(TIDPVMD, TM_TASKER, SYSCTX_TM);
		if (cc > 0) {
			pvm_upkint(&cc, 1, 1);
			if (!cc) {
				imit = !imit;
				if ( imit ) {
					pvm_setsbuf(pvm_mkbuf(PvmDataDefault));
					pvm_pkint(&pvmmytid,1,1);
					mbindex = pvm_putinfo( PVMTASKERCLASS,
						pvm_getsbuf(), PvmMboxMultiInstance );
				} else if ( mbindex >= 0 ) {
					if ( pvm_delinfo( PVMTASKERCLASS, mbindex,
							PvmMboxDefault ) >= 0 ) {
						mbindex = -1;
					}
				}
			}
			pvm_freebuf(pvm_setrbuf(rbf));

		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_REG_TASKER,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_reg_tasker", cc);
	return cc;
}


int
pvm_reg_hoster()
{
	static int imit = 0;		/* i'm the hoster */
	static int mbindex = -1;	/* mbox index */

	int cc;
	int sbf;
	int rbf;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_REG_HOSTER,TEV_EVENT_ENTRY)) {
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);

		cc = imit ? 0 : 1;
		pvm_pkint(&cc, 1, 1);
		cc = msendrecv(TIDPVMD, TM_HOSTER, SYSCTX_TM);
		if (cc > 0) {
			pvm_upkint(&cc, 1, 1);
			if (!cc) {
				imit = !imit;
				if ( imit ) {
					pvm_setsbuf(pvm_mkbuf(PvmDataDefault));
					pvm_pkint(&pvmmytid,1,1);
					mbindex = pvm_putinfo( PVMHOSTERCLASS,
						pvm_getsbuf(), PvmMboxMultiInstance );
				} else if ( mbindex >= 0 ) {
					if ( pvm_delinfo( PVMHOSTERCLASS, mbindex,
							PvmMboxDefault ) >= 0 ) {
						mbindex = -1;
					}
				}
			}
			pvm_freebuf(pvm_setrbuf(rbf));

		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_REG_HOSTER,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_reg_hoster", cc);
	return cc;
}


int
pvm_reg_tracer( tctx, ttag, octx, otag, tmask, tbuf, topt )
	int tctx;
	int ttag;
	int octx;
	int otag;
	Pvmtmask tmask;
	int tbuf;
	int topt;
{
	static int imit = 0;		/* i'm the tracer */
	static int mbindex = -1;	/* mbox index */

	int cc;
	int sbf;
	int rbf;
	int x;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_REG_TRACER,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_TRC, TEV_DATA_SCALAR, &tctx, 1, 1 );
			TEV_PACK_INT( TEV_DID_TRT, TEV_DATA_SCALAR, &ttag, 1, 1 );
			TEV_PACK_INT( TEV_DID_TRX, TEV_DATA_SCALAR, &octx, 1, 1 );
			TEV_PACK_INT( TEV_DID_TRO, TEV_DATA_SCALAR, &otag, 1, 1 );
			TEV_PACK_STRING( TEV_DID_TRM, TEV_DATA_SCALAR,
				tmask, 1, 1 );
			TEV_PACK_INT( TEV_DID_TRB, TEV_DATA_SCALAR, &tbuf, 1, 1 );
			TEV_PACK_INT( TEV_DID_TRL, TEV_DATA_SCALAR, &topt, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);

		cc = imit ? 0 : 1;
		pvm_pkint(&cc, 1, 1);
		if ( cc ) {
			pvm_pkint(&tctx, 1, 1);
			pvm_pkint(&ttag, 1, 1);
			pvm_pkint(&octx, 1, 1);
			pvm_pkint(&otag, 1, 1);
			pvm_pkstr(tmask);
			pvm_pkint(&tbuf, 1, 1);
			pvm_pkint(&topt, 1, 1);
		}
		cc = msendrecv(TIDPVMD, TM_TRACER, SYSCTX_TM);
		if (cc > 0) {
			pvm_upkint(&cc, 1, 1);
			if (!cc) {
				imit = !imit;
				if ( imit ) {
					pvm_setsbuf(pvm_mkbuf(PvmDataDefault));
					pvm_pkint(&pvmmytid,1,1);
					pvm_pkint(&tctx, 1, 1);
					pvm_pkint(&ttag, 1, 1);
					pvm_pkint(&octx, 1, 1);
					pvm_pkint(&otag, 1, 1);
					pvm_pkstr(tmask);
					pvm_pkint(&tbuf, 1, 1);
					pvm_pkint(&topt, 1, 1);
					mbindex = pvm_putinfo( PVMTRACERCLASS,
						pvm_getsbuf(), PvmMboxDefault );
				} else if ( mbindex >= 0 ) {
					if ( pvm_delinfo( PVMTRACERCLASS, mbindex,
							PvmMboxDefault ) >= 0 ) {
						mbindex = -1;
					}
				}
			}
			pvm_freebuf(pvm_setrbuf(rbf));

		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_REG_TRACER,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_reg_tracer", cc);
	return cc;
}


/*	pvm_hostsync()
*
*	Get time of day clock from remote host.
*	Returns current time on remote host,
*	difference between local clock and remote host clock.
*
*	Note the delta time is a 2s-compl./1000000-compl. signed timeval.
*	Positive values are normal, negative ones are f.e.:
*		-1 uSec  = -1,999999
*		-1 Sec   = -1,000000
*		-1.1 Sec = -2,999000
*/

int
pvm_hostsync(host, clk, delta)
	int host;				/* pvmd tid of host */
	struct timeval *clk;	/* current time on host */
	struct timeval *delta;	/* time relative to local clock */
{
	int cc;
	int sbf, rbf;
	struct timeval myta, mytb, remt;
	int i[2];

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		pvm_pkint(&host, 1, 1);

		gettimeofday(&myta, (struct timezone *) NULL);
		if ((cc = msendrecv(TIDPVMD, TM_HOSTSYNC,SYSCTX_TM)) > 0) {
			gettimeofday(&mytb, (struct timezone *) NULL);

			pvm_upkint(&cc, 1, 1);
			if (cc >= 0) {
				cc = 0;
				pvm_upkint(i, 2, 1);
				remt.tv_sec = i[0];
				remt.tv_usec = i[1];

				if (clk)
					*clk = remt;

				if (delta) {
					TVDIVN(&myta, &myta, 2);
					TVDIVN(&mytb, &mytb, 2);
					TVXADDY(&myta, &myta, &mytb);
					TVXSUBY(&myta, &myta, &remt);
					*delta = myta;
				}
			}
			pvm_freebuf(pvm_setrbuf(rbf));
		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (cc < 0)
		lpvmerr("pvm_host_sync", cc);
	return cc;
}


/*	pvm_gettmask()
*
*	Get our (or child) trace mask.
*/

int
pvm_gettmask(who, tmask)
	int who;
	Pvmtmask tmask;
{
	int i;
	char *tm = 0;

	if (who == PvmTaskChild)
		tm = pvmctrc.tmask;
	else if (who == PvmTaskSelf)
		tm = pvmtrc.tmask;

	if (!tm)
		return lpvmerr("pvm_gettmask", PvmBadParam);
	BCOPY(tm, tmask, TEV_MASK_LENGTH);
	return PvmOk;
}


/*	pvm_settmask()
*
*	Set our (or child's) trace mask.
*/

int
pvm_settmask(who, tmask)
	int who;
	Pvmtmask tmask;
{
	int i;
	char *tm = 0;

	if (who == PvmTaskChild)
		tm = pvmctrc.tmask;
	else if (who == PvmTaskSelf)
		tm = pvmtrc.tmask;

	if (!tm)
		return lpvmerr("pvm_settmask", PvmBadParam);

	BCOPY(tmask, tm, TEV_MASK_LENGTH);
	return PvmOk;
}


int
pvm_archcode(arch)
	char *arch;
{
	struct pvmhostinfo *hlist;
	int sbf, rbf, cc;
	int nhost, narch;
	int i;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_ARCHCODE,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_AN, TEV_DATA_SCALAR,
				arch ? arch : "", 1, 1 );
			TEV_FIN;
		}
	}

	if (!arch)
		cc = PvmBadParam;

	else
	{
		/* Go get pvm_config() info...  :-Q :-Q :-Q */
		/* (can't use pvm_config() directly, as stomps */
		/* hostinfo structure...  Damn. */

		if (!(cc = BEATASK)) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			if (pvmschedtid)
				cc = msendrecv(pvmschedtid, SM_CONFIG, PvmBaseContext);
			else
				cc = msendrecv(TIDPVMD, TM_CONFIG, SYSCTX_TM);
			if (cc > 0) {
				pvm_upkint(&nhost, 1, 1);
				pvm_upkint(&narch, 1, 1);
				hlist = TALLOC(nhost, struct pvmhostinfo, "hi");
				for (i = 0; i < nhost; i++) {
					pvm_upkint(&hlist[i].hi_tid, 1, 1);
					pvmupkstralloc(&(hlist[i].hi_name));
					pvmupkstralloc(&(hlist[i].hi_arch));
					pvm_upkint(&hlist[i].hi_speed, 1, 1);
					pvm_upkint(&hlist[i].hi_dsig, 1, 1);
				}
				pvm_freebuf(pvm_setrbuf(rbf));
			}
			pvm_freebuf(pvm_setsbuf(sbf));
			pvm_setrbuf(rbf);

			cc = PvmNotFound;

			for ( i=0 ; i < nhost ; i++ ) {
				if ( !strcmp(hlist[i].hi_arch, arch) ) {
					cc = hlist[i].hi_dsig;
					break;
				}
			}

			while (nhost-- > 0) {
				PVM_FREE(hlist[nhost].hi_name);
				PVM_FREE(hlist[nhost].hi_arch);
			}
			PVM_FREE(hlist);
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_ARCHCODE,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_AC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_archcode", cc);
	
	return( cc );
}


int
pvm_newcontext()
{
	int sbf, rbf, cc;
	int mid = -1;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_NEWCONTEXT,TEV_EVENT_ENTRY)) {
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		cc = 0;
		pvm_pkint(&cc, 1, 1);
		if ((cc = msendrecv(TIDPVMD, TM_CONTEXT, SYSCTX_TM)) > 0) {
			pvm_upkint(&cc, 1, 1);
			pvm_freebuf(pvm_setrbuf(rbf));

		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_NEWCONTEXT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CXN, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_newcontext", cc);
	return cc;
}


int
pvm_freecontext(cid)
	int cid;
{
	int sbf, rbf, cc;
	int mid = -1;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_FREECONTEXT,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_CXF, TEV_DATA_SCALAR, &cid, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		pvm_pkint(&cid, 1, 1);
		if ((cc = msendrecv(TIDPVMD, TM_CONTEXT, SYSCTX_TM)) > 0) {
			pvm_upkint(&cc, 1, 1);
			pvm_freebuf(pvm_setrbuf(rbf));

		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_FREECONTEXT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_freecontext", cc);
	return cc;
}


int
pvm_getcontext()
{
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETCONTEXT,TEV_EVENT_ENTRY)) {
			TEV_FIN;
		}
	}

	/* Yep, overkill... */

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETCONTEXT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CXC, TEV_DATA_SCALAR,
				&pvmmyctx, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	return pvmmyctx;
}


int
pvm_setcontext(newctx)
	int newctx;
{
	int c;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SETCONTEXT,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_CXS, TEV_DATA_SCALAR, &newctx, 1, 1 );
			TEV_FIN;
		}
	}

	c = pvmmyctx;
	pvmmyctx = newctx;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SETCONTEXT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &c, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	return c;
}

/*
*	pvm_addmhf()
*
*  add message handler function
*
*  function returns:
*	index ( >= 0 ) message handler
*			- can be used to delete the handler via pvm_delmhf()
*	PvmExists - can't insert as handler already exists
*			- search by (tag, ctx, src) including "wild-cards" (-1)
*
*/
int
pvm_addmhf(src, tag, ctx, f)
	int src, tag, ctx;			/* message source, context, tag */
#ifdef  IMA_SCO
	int (*f)(int);				/* handler function */
#else
	int (*f)();				/* handler function */
#endif
{
	int i;		/* temp local */
	int hand;	/* index into handles[] array -->> internal use only */
				/* - not for user */
	int mhid;	/* message handler index -->> into dhandles[] */
				/* -->> give to user */
	int exists = 0;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_ADDMHF,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_RST, TEV_DATA_SCALAR, &src, 1, 1 );
			TEV_PACK_INT( TEV_DID_RMC, TEV_DATA_SCALAR, &tag, 1, 1 );
			TEV_PACK_INT( TEV_DID_RCX, TEV_DATA_SCALAR, &ctx, 1, 1 );
			TEV_FIN;
		}
	}

	/*
	*  confirm that does not already exist within "handles"
	*  this includes "wild-card" options in both directions too...
	*
	*  search from most recently added message handler to system
	*  added message handlers
	*/
	for ( i=nhandles-1 ; i >= 0 && !exists ; i-- ){
		if ((handles[i].header.tag == -1
				|| handles[i].header.tag == tag || tag == -1)
		&& (handles[i].header.ctx == -1
				|| handles[i].header.ctx == ctx || ctx == -1)
		&& (handles[i].header.src == -1
				|| handles[i].header.src == src || src == -1))
		{
			exists++;
		}
	}

	/*
	*  see if there are any already on free list
	*  -  use existing if on free list
	*  -  create new if none free - two scenarios:
	*       1. may be initial message handlers
	*       2. may be additional message handlers and lists just full
	*
	*  note: ndhandles never decreases - thus if less than nhandles,
	*  must be some free slots...
	*/
	if ( exists )
			mhid = PvmExists;
	else if (ndhandles > nhandles){
		/*
		*  there exist some free slots
		*/
		mhid = fl_dhandles;		/* take from head of free list */
		nhandles++;				/* incr actual # of active headers */

		/*
		*  move next one to head of free list
		*  if tail of list - will move "tail marker" (-1) to
		*  fl_dhandles to indicte empty list
		*/
		fl_dhandles = dhandles[mhid].mhid;

		/*
		*  fill the lists now...
		*/

		dhandles[mhid].mhid = mhid;		/* set to self */

		/* dhandles[mhid].handle	no change here as should be next */
		/* contiguous on handles[] */

		hand = dhandles[mhid].handle;	/* set to index -> handles[] */
		handles[hand].mhid = mhid;		/* set to corresponding mhid */
		handles[hand].header.tag = tag;
		handles[hand].header.ctx = ctx;
		handles[hand].header.src = src;
		handles[hand].f = f;	/* sonnofabeech! */
	}
	else{
		/* no free slots available - will have to allocate more slots...
		*  two scenarios for this:
		*    1. first time adding message handlers - none ever
		*           allocated.
		*    2. all presently allocated are filled and active - but
		*           been here before.
		*
		*  note: since no free slots, must have case of:
		*           ndhandles == nhandles
		*
		*  ndhandles : is the "high-water" mark as it is never decreased
		*  nhandles  : indicates the actual number of active handles
		*/
		if (ndhandles == 0){
			/* first time here - allocate new memory... */
			dhandles = TALLOC(++ndhandles, struct dhandler, "dhandles");
			handles = TALLOC(++nhandles, struct mhandler, "mhandle");
		}
		else{
			/* been here! - reallocate more space... */
			dhandles = TREALLOC(dhandles, ++ndhandles, struct dhandler);
			handles = TREALLOC(handles, ++nhandles, struct mhandler);
		}

		mhid = ndhandles - 1;		/* set index into dhandles */

		/*
		*  fill the lists now...
		*/

		dhandles[mhid].mhid = mhid;		/* set to self */
		dhandles[mhid].handle = mhid;	/* set same since lists full */

		handles[mhid].mhid = mhid;
		handles[mhid].header.tag = tag;
		handles[mhid].header.ctx = ctx;
		handles[mhid].header.src = src;
		handles[mhid].f = f;	/* sonnofabeech! */
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_ADDMHF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MHI, TEV_DATA_SCALAR, &mhid, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (mhid < 0)
		lpvmerr("pvm_addmhf", mhid);
	return mhid;	/* return message handler */
					/* -->> index into dhandles[] */
} /* pvm_addmhf */


/*
*	pvm_delmhf()
*
*  delete message handler function
*
*  function returns:
*	PvmBadParam - negative mhid value
*	PvmNotFound - mhid not found in list
*			- possibly deleted before...
*			- possibly never existed (access past end of list index)
*			- return PvmNotFound but no error message as is normal
*                 operation...
*	PvmOk
*/
int
pvm_delmhf(mhid)
	int mhid;			/* message handler index */
{
	char *errstr;
	int htoid;			/* handle - moved to   - id location */
	int hfrid;			/* handle - moved from - id location */
	int rc = 0;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_DELMHF,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MHI, TEV_DATA_SCALAR, &mhid, 1, 1 );
			TEV_FIN;
		}
	}

	/*
	*  check incomming parameter at lower and upper boundary
	*	- can't have negative mhid values
	*	- too high an index - mhid entry does not exist
	*/
	if (mhid < 0) { /* too low */
		errstr = "pvm_delmhf";
		rc = PvmBadParam;
	}
	else if (mhid >= ndhandles) { /* too high */
		errstr = "(mhid >= ndhandles) pvm_delmhf";
		rc = PvmNotFound;
	}

	/*
	*  make sure is not already on free list - can only delete active
	*  mhid entries
	*
	*  confirm active by:
	*    since handles is contiguous at low order indices, thus any
	*    entry in dhandles[x].handle less-than nhandles is an active
	*    entry and may be deleted
	*/
	if ( rc >= 0 ) {
		if ( dhandles[mhid].handle < nhandles ) {
			/*
			*  this one's active - delete it
			*/

			nhandles--;		/* reduce # of active handles[] entries */
			htoid = dhandles[mhid].handle;	/* handle - moved to */
											/* -id location */
			hfrid = nhandles;		/* handle - moved from - id loc */

			/* Don't Spank List if Last Entry...  D-Oh! */
			if ( htoid != hfrid )
			{
				/*
				*  move the last "active" handle[] entry up to "deleted"
				*  location
				*/

				handles[htoid] = handles[hfrid];	/* the move */

				/* unnecessary?! handles[hfrid].mhid = htoid; */
				/* ("back-link" ptr adjust?) */

				/*
				*  adjust dhandles[] to point to
				*    1. the moved "up" entry
				*    2. the "deleted" entry
				*/

				/* 1. the moved "up" entry */
				dhandles[handles[htoid].mhid].handle = htoid;

				/* 2. the "deleted" entry */
				dhandles[mhid].handle = hfrid;
			}

			/*
			*  put the "freed" dhandle entry (mhid value) on head of
			*  the free list
			*/

			/* link this to previous free list */
			dhandles[mhid].mhid = fl_dhandles;

			fl_dhandles = mhid;		/* put this one on head of list */
		}
		else {
			/* this one is already on free list - not active entry */
			errstr = "pvm_delmhf";
			rc = PvmNotFound;
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_DELMHF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &rc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (rc < 0)
		lpvmerr(errstr, rc);
	return( rc );

} /* pvm_delmhf */


int
pvm_putinfo(name, mid, flags)
	char *name;			/* class name */
	int mid;			/* message to store */
	int flags;			/* options */
{
	int index = -1;
	int sbf, rbf, cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PUTINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				name ? name : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_CI, TEV_DATA_SCALAR, &index, 1, 1 );
			TEV_PACK_INT( TEV_DID_CF, TEV_DATA_SCALAR, &flags, 1, 1);
			TEV_PACK_INT( TEV_DID_CD, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (!name || !*name || index < -1) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			cc = TMDB_PUT;
			pvm_pkint(&cc, 1, 1);
			pvm_pkint(&pvmmytid, 1, 1);
			pvm_pkstr(name);
			pvm_pkint(&index, 1, 1);
			pvm_pkint(&flags, 1, 1);
			pvm_pkmesg(mid);
			if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PUTINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		if (cc != PvmDenied && cc != PvmExists)
			lpvmerr("pvm_putinfo", cc);
		else
			pvm_errno = cc;
	return cc;
}


int
pvm_recvinfo(name, index, flags)
	char *name;		/* class name */
	int index;		/* req class index or -1 for any */
	int flags;		/* options */
{
	int sbf, cc;
	int mid = -1;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				name ? name : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_CI, TEV_DATA_SCALAR, &index, 1, 1 );
			TEV_PACK_INT( TEV_DID_CF, TEV_DATA_SCALAR, &flags, 1, 1 );
			TEV_FIN;
		}
	}

	if (!name || !*name || index < 0) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			if (pvmrbuf)
				umbuf_free(pvmrbuf);
			pvmrbuf = 0;

			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			cc = TMDB_GET;
			pvm_pkint(&cc, 1, 1);
			pvm_pkint(&pvmmytid, 1, 1);
			pvm_pkstr(name);
			pvm_pkint(&index, 1, 1);
			pvm_pkint(&flags, 1, 1);
			if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				if (cc >= 0) {
					mid = pvm_upkmesg();
					pvm_freebuf(pvm_setrbuf(mid));
				}
			}
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_PACK_INT( TEV_DID_CR, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if ( cc < 0 ) {
		if ( cc != PvmNotFound )
			lpvmerr("pvm_recvinfo", cc);
		else
			pvm_errno = cc;
		return cc;
	}
	else
		return mid;
}


int
pvm_delinfo(name, index, flags)
	char *name;		/* class name */
	int index;		/* class index or -1 for all */
	int flags;		/* options */
{
	int sbf, rbf, cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_DELINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				name ? name : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_CI, TEV_DATA_SCALAR, &index, 1, 1 );
			TEV_PACK_INT( TEV_DID_CF, TEV_DATA_SCALAR, &flags, 1, 1 );
			TEV_FIN;
		}
	}

	if (!name || !*name || index < 0) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			cc = TMDB_REMOVE;
			pvm_pkint(&cc, 1, 1);
			pvm_pkint(&pvmmytid, 1, 1);
			pvm_pkstr(name);
			pvm_pkint(&index, 1, 1);
			pvm_pkint(&flags, 1, 1);
			if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_DELINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		if (cc != PvmDenied && cc != PvmNotFound)
			lpvmerr("pvm_delinfo", cc);
		else
			pvm_errno = cc;
	return cc;
}


int
pvm_getmboxinfo(pattern, nclasses, classes)
	char *pattern;					/* class name */
	int *nclasses;					/* number of classes returned */
	struct pvmmboxinfo **classes;	/* name list returned */
{
	static struct pvmmboxinfo *clist = (struct pvmmboxinfo *) NULL;
	static int nclist = 0;

	int sbf, rbf, cc;
	int i, j;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETMBOXINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				pattern ? pattern : "", 1, 1 );
			TEV_FIN;
		}
	}

	if ( !pattern )
		pattern = "";

	if ( clist ) {
		for ( i=0 ; i < nclist ; i++ ) {
			if ( clist[i].mi_name )
				PVM_FREE( clist[i].mi_name );
			if ( clist[i].mi_indices )
				PVM_FREE( clist[i].mi_indices );
			if ( clist[i].mi_owners )
				PVM_FREE( clist[i].mi_owners );
			if ( clist[i].mi_flags )
				PVM_FREE( clist[i].mi_flags );
		}
		PVM_FREE( clist );
		clist = (struct pvmmboxinfo *) NULL;
		nclist = 0;
	}

	if ( !(cc = BEATASK) )
	{
		sbf = pvm_setsbuf( pvm_mkbuf( PvmDataFoo ) );
		rbf = pvm_setrbuf( 0 );
		cc = TMDB_NAMES;
		pvm_pkint( &cc, 1, 1 );
		pvm_pkint( &pvmmytid, 1, 1 );
		pvm_pkstr( pattern );
		cc = 0;
		pvm_pkint( &cc, 1, 1 );
		pvm_pkint( &cc, 1, 1 );
		if ( (cc = msendrecv( TIDPVMD, TM_DB, SYSCTX_TM )) > 0 ) {
			pvm_upkint( &cc, 1, 1 );
			if ( cc >= 0 ) {
				pvm_upkint( &nclist, 1, 1 );
				clist = TALLOC( nclist, struct pvmmboxinfo, "classes" );
				for ( i=0 ; i < nclist ; i++ ) {
					pvmupkstralloc( &(clist[i].mi_name) );
					pvm_upkint( &(clist[i].mi_nentries), 1, 1 );
					clist[i].mi_indices = TALLOC( clist[i].mi_nentries,
							int, "class_indices" );
					clist[i].mi_owners = TALLOC( clist[i].mi_nentries,
							int, "class_owners" );
					clist[i].mi_flags = TALLOC( clist[i].mi_nentries,
							int, "class_flags" );
					for ( j=0 ; j < clist[i].mi_nentries ; j++ ) {
						pvm_upkint( &(clist[i].mi_indices[j]), 1, 1 );
						pvm_upkint( &(clist[i].mi_owners[j]), 1, 1 );
						pvm_upkint( &(clist[i].mi_flags[j]), 1, 1 );
					}
				}
				if (classes)
					*classes = clist;
				if (nclasses)
					*nclasses = nclist;
			}
			pvm_freebuf(pvm_setrbuf(rbf));
		}
		
		else
			pvm_setrbuf(rbf);

		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETMBOXINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_getmboxinfo", cc);
	return cc;
}


int
pvm_siblings(tidsp)
	int **tidsp;
{
	static int pvmsiblings_me[1];
	int cc = PvmParentNotSet;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SIBLINGS,TEV_EVENT_ENTRY)) {
			TEV_FIN;
		}
	}

	if (pvmnsibs == -1)  /* only call pvm_parent if we haven't gotten sibs */
		cc = pvm_parent();

	if (cc > 0 || cc == PvmParentNotSet) 
	{
		/* wait for pvmnsibs to be set by spawning program */ 
		while (pvmnsibs == -1) {
			cc = mroute(0, 0, 0, (struct timeval *)0);
			if (cc < 0)
				break;
		}
		if (pvmnsibs != -1) {
			cc = pvmnsibs;
			*tidsp = pvmsibtids;
		}

	} else 
		if (cc == PvmNoParent)  /* I am my only sibling */
		{
			cc = 1;
			pvmsiblings_me[0] = pvmmytid;
			*tidsp = pvmsiblings_me; 
		}
			
			

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SIBLINGS,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_SIB, TEV_DATA_ARRAY,
				pvmsibtids, cc, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_siblings", cc);
	return cc;
}


int
pvm_export(name)
	char *name;
{
	char *vn = "PVM_EXPORT";
	char *e;
	char *p, *q;
	int l;

	if (!name[0])
		goto done;

	if (e = getenv(vn)) {
		p = e;
		while (*p) {
			while (*p == ':')
				p++;
			q = p;
			while (*q && *q != ':')
				q++;
			l = q - p;
			if (strlen(name) == l && !strncmp(name, p, l))
				goto done;
			p = q;
		}
		p = TALLOC(strlen(vn) + strlen(e) + strlen(name) + 3, char, "str");
		strcpy(p, vn);
		strcat(p, "=");
		strcat(p, e);
		if (e[0])
			strcat(p, ":");
		strcat(p, name);
		pvmputenv(p);

	} else {
		e = TALLOC(strlen(vn) + strlen(name) + 2, char, "str");
		strcpy(e, vn);
		strcat(e, "=");
		strcat(e, name);
		pvmputenv(e);
	}

done:
/*
	e = getenv(vn);
	pvmlogprintf("pvm_export() %s=%s\n", vn, e);
*/
	return 0;
}


int
pvm_unexport(name)
	char *name;
{
	char *vn = "PVM_EXPORT";
	char *e, *enew;
	char *p, *q;
	int l;

	if (!name[0])
		goto done;

	if (e = getenv(vn)) {
		p = e;
		while (*p) {
			while (*p == ':')
				p++;
			q = p;
			while (*q && *q != ':')
				q++;
			l = q - p;
			if (strlen(name) == l && !strncmp(name, p, l)) {
				if (*q == ':')
					q++;
				else if (p > e && *(p - 1) == ':')
					p--;
				enew = TALLOC(strlen(vn) + (p - e) + strlen(q) + 2, char, "str");
				strcpy(enew, vn);
				strcat(enew, "=");
				strncat(enew, e, p - e);
				strcat(enew, q);
				pvmputenv(enew);
				goto done;
			}
			p = q;
		}
	}

done:
/*
	e = getenv(vn);
	pvmlogprintf("pvm_unexport() %s=%s\n", vn, e);
*/
	return 0;
}


/*	pvmreset()
*
*	One handy routine for cleaning up the virtual machine.
*	Used by "pvm" console, XPVM and whoever else is in a
*	tidy mood.  Not necessarily for public consumption...
*/

int
pvmreset( mytid, killtasks, class, index )
int mytid;
int killtasks;
char *class;
int index;
{
	struct pvmtaskinfo *tip;
	int ntask;

	int *noresets = (int *) NULL;
	int nnr = 0;

	int sbf, rbf;
	int found;
	int i, j;
	int cc;

	if ( !pvm_tasks( 0, &ntask, &tip ) && ntask > 0 )
	{
		pvm_getnoresets( &noresets, &nnr );

		for ( i=0 ; i < ntask && killtasks ; i++ )
		{
			for ( j=0, found=0 ; j < nnr && !found ; j++ )
				if ( noresets[j] == tip[i].ti_tid )
					found++;

			if ( !found && tip[i].ti_tid && tip[i].ti_tid != mytid )
				pvm_kill(tip[i].ti_tid);
		}
	}

	/* XXX this is gnasty... */
	/* while ((i = pvm_lookup("pvmgs", -1, &j)) >= 0) */
		/* pvm_delete("pvmgs", i); */

	/* Tell Master PVMD to clean up mboxes... */

	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
	rbf = pvm_setrbuf(0);
	cc = TMDB_RESET;
	pvm_pkint(&cc, 1, 1);
	pvm_pkint(&pvmmytid, 1, 1);
	pvm_pkstr(class ? class : "");
	cc = 0;
	pvm_pkint(&index, 1, 1);
	pvm_pkint(&killtasks, 1, 1);
	pvm_pkint(&nnr, 1, 1);
	for ( i=0 ; i < nnr ; i++ )
		pvm_pkint(&(noresets[i]), 1, 1);
	if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
		pvm_upkint(&cc, 1, 1);
		pvm_freebuf(pvm_setrbuf(rbf));
	} else
		pvm_setrbuf(rbf);
	pvm_freebuf(pvm_setsbuf(sbf));

	return 0;
}


/***************************************************************
 **  backwards compat functions: built on new mbox interface  **
 **                                                           **
 ***************************************************************/

int
pvm_insert(name, req, data)
	char *name;		/* class name */
	int req;		/* requested class index or -1 for any */
	int data;
{
	int sbf, rbf, cc;
	int flags;
	int mid;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_INSERT,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				name ? name : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_CI, TEV_DATA_SCALAR, &req, 1, 1 );
			TEV_PACK_INT( TEV_DID_CD, TEV_DATA_SCALAR, &data, 1, 1 );
			TEV_FIN;
		}
	}

	if (!name || !*name || req < -1) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {

			flags = PvmMboxDefault
				| PvmMboxMultiInstance
				| PvmMboxPersistent;

			mid = pvm_mkbuf(PvmDataFoo);
			sbf = pvm_setsbuf(mid);
			pvm_pkint(&data, 1, 1);

			pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			cc = TMDB_PUT;
			pvm_pkint(&cc, 1, 1);
			pvm_pkint(&pvmmytid, 1, 1);
			pvm_pkstr(name);
			pvm_pkint(&req, 1, 1);
			pvm_pkint(&flags, 1, 1);
			pvm_pkmesg(mid);
			if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
			pvm_freebuf(mid);
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_INSERT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		if (cc != PvmDupEntry)
			lpvmerr("pvm_insert", cc);
		else
			pvm_errno = cc;
	return cc;
}


int
pvm_lookup(name, req, datap)
	char *name;		/* class name */
	int req;		/* req class index or -1 for any */
	int *datap;		/* data return */
{
	int sbf, rbf, cc;
	int flags;
	int mid;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_LOOKUP,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				name ? name : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_CI, TEV_DATA_SCALAR, &req, 1, 1);
			TEV_FIN;
		}
	}

	if (!name || !*name || req < -1) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {

			flags = PvmMboxDefault;

			if ( req < 0 ) {
				flags |= PvmMboxFirstAvail;
				req = 0;
			}

			rbf = pvm_setrbuf(0);

			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			cc = TMDB_GET;
			pvm_pkint(&cc, 1, 1);
			pvm_pkint(&pvmmytid, 1, 1);
			pvm_pkstr(name);
			pvm_pkint(&req, 1, 1);
			pvm_pkint(&flags, 1, 1);
			if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				if (cc >= 0) {
					mid = pvm_upkmesg();
					pvm_freebuf(pvm_setrbuf(mid));
				}
			}
			pvm_freebuf(pvm_setsbuf(sbf));

			if ( cc >= 0 && datap )
				pvm_upkint(datap, 1, 1);
			
			pvm_freebuf(pvm_setrbuf(rbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_LOOKUP,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		if (cc != PvmNoEntry)
			lpvmerr("pvm_lookup", cc);
		else
			pvm_errno = cc;
	return cc;
}


int
pvm_delete(name, req)
	char *name;		/* class name */
	int req;		/* class index or -1 for all */
{
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_DELETE,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				name ? name : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_CI, TEV_DATA_SCALAR, &req, 1, 1 );
			TEV_FIN;
		}
	}

	if (!name || !*name || req < 0) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			cc = pvm_delinfo( name, req, PvmMboxDefault );
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_DELETE,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		if (cc != PvmNoEntry)
			lpvmerr("pvm_delete", cc);
		else
			pvm_errno = cc;
	return cc;
}


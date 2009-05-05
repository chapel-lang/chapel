
static char rcsid[] =
	"$Id: lpvmshmem.c,v 1.50 2009/01/30 21:22:46 pvmsrc Exp $";

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
 *	lpvmshmem.c
 *
 *	Libpvm core for MPP environment.
 *
 * $Log: lpvmshmem.c,v $
 * Revision 1.50  2009/01/30 21:22:46  pvmsrc
 * Fixed up insecure usage of fopen() for creating/writing/appending files.
 * 	- replace with open(...O_EXCL...) and fdopen()...
 * (Spanker=kohl)
 *
 * Revision 1.49  2006/10/09 17:05:51  pvmsrc
 * Fixed long-standing typo - no one must ever use this stuff...!
 * 	- missing "||" before last "defined(IMA_AIX5MP)"...  :-Q
 * 	- submitted by "Richard A. Roberts " <roberts@asc.hpc.mil>
 * (Spanker=kohl)
 *
 * Revision 1.48  2005/05/18 17:56:29  pvmsrc
 * Added calls to pvmchkuid()...
 * 	- keep people from running PVM as root...!  :-o
 * (Spanker=kohl)
 *
 * Revision 1.47  2004/01/14 18:50:57  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.46  2002/02/18 19:18:35  pvmsrc
 * Added "|| defined(IMA_SUN4SOL2)" to "const void *i, *j" #if
 * 	for int_compare() / qsort()...
 * 	(needed for sure for 64 bit) Solaris)
 * (Spanker=kohl)
 *
 * Revision 1.45  2001/09/25 21:21:00  pvmsrc
 * Yanked "char *pvmgettmp();" decl - now in pvm3.h...
 * (Spanker=kohl)
 *
 * Revision 1.44  2001/05/11 17:32:29  pvmsrc
 * Eliminated references to sys_errlist & sys_nerr.
 * 	- unnecessary, and we're whacking that crap anyway.
 * (Spanker=kohl)
 *
 * Revision 1.43  2000/02/10 20:44:41  pvmsrc
 * Replaced hard-coded /tmp for PVMSHMFILE.
 * 	- use pvmgettmp() routine now to determine PVM temp dir.
 * (Spanker=kohl)
 *
 * Revision 1.42  1999/07/08 19:00:02  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.41  1999/03/15  19:06:03  pvmsrc
 * Replaced sleep() calls with pvmsleep().
 * 	- Unix / Win32, C / Fortran compat...
 * (Spanker=kohl)
 *
 * Revision 1.40  1999/02/20  14:17:57  pvmsrc
 * Another try at the SGI compiler warning...  :-]
 * (Spanker=kohl)
 *
 * Revision 1.39  1999/02/19  14:10:56  pvmsrc
 * SGI compiler warning...
 * (Spanker=kohl)
 *
 * Revision 1.38  1998/08/13  18:31:13  pvmsrc
 * Altered SUNMP to use test and set operations with semaphores
 * 		for page locking instead of MUTEX and cond vars.
 * 	Changes are mainly in pvmshmem.h, with lots of #ifdefs changes.
 * 	Makefile altered to use the PLOCKFILE to indicate the Page Locking
 * 		INLINE code used (from SUNMP.conf).
 * 	Some changes effect AIX MP versions which still use conditional
 * 		variables and may change to semaphores soon.
 * (Spanker=fagg)
 *
 * Revision 1.37  1998/07/24  17:11:46  pvmsrc
 * Cleaned up use of SOCKLENISUINT / oslen.
 * 	- use oslen for every socket-related call:
 * 		* bind(), recvfrom(), getsockname() and accept().
 * (Spanker=kohl)
 *
 * Revision 1.36  1998/02/24  15:36:25  pvmsrc
 * Oops...  leftover typo from umbuf -> pmsg renaming way back when.
 * 	- UB_DIFFNODE -> MM_DIFFNODE for IMA_CSPP.
 * (Spanker=kohl)
 *
 * Revision 1.35  1998/02/23  22:51:39  pvmsrc
 * Added AIX4SP2 stuff.
 * (Spanker=kohl)
 *
 * Revision 1.34  1997/12/31  20:50:09  pvmsrc
 * Cleaned Up System Message Handlers.
 * 	- current send / recv buffers now saved before invocation of
 * 		message handler functs.
 * 	- removed manual rbf = setrbuf(mid) saving & resetting in
 * 		handlers...
 * (Spanker=kohl)
 *
 * Revision 1.33  1997/12/01  19:20:44  pvmsrc
 * Replaced #ifdef IMA_OS2 fd_set declarations:
 * 	- new #ifdef FDSETNOTSTRUCT.
 * 	- choose between "fd_set foo" and "struct fd_set foo"...
 * (Spanker=kohl)
 *
 * Revision 1.32  1997/11/04  23:19:21  pvmsrc
 * Cleaned up fd_set stuff (hopefully).
 * (Spanker=kohl)
 *
 * Revision 1.31  1997/10/24  15:18:03  pvmsrc
 * Added TEV_DID_RCX to trace events for Receive Message Context.
 * 	- in pvm_recv(), pvm_trecv(), pvm_nrecv(), and pvm_precv().
 * (Spanker=kohl)
 *
 * Revision 1.30  1997/10/24  14:29:33  pvmsrc
 * Added TEV_DID_MCX / pvmmyctx trace event info to:
 * 	- pvm_send(), pvm_mcast(), pvm_recv(), pvm_trecv(), pvm_nrecv().
 * 	- pvm_psend(), pvm_precv().
 * (Spanker=kohl)
 *
 * Revision 1.29  1997/08/06  22:43:20  pvmsrc
 * Added new SGI6 and SGIMP6 arches.
 *
 * Revision 1.28  1997/07/08  21:25:50  pvmsrc
 * peer_send() now directs control TC messages via the daemon.
 * node_mcast() is no-longer called until its pmsg buffer error is found,
 * i.e. mcast messages are sent via the daemon, using a MCA instead of directly.
 * pvmmcast() sets the context to the current default.
 * msendrecv() now takes a context arg! It sets it as well (and the waitid).
 * beatask() uses SYS contexts on handshakes.
 *
 * Revision 1.27  1997/07/03  17:41:50  pvmsrc
 * Added new sleep_dammit() routine.
 * 	- usleep() on SUNMP, RS6KMP, AIX4MP.
 * 	- select() w/ timeout everywhere else.
 *
 * Revision 1.26  1997/07/02  20:34:58  pvmsrc
 * Commented out usleep() call.
 * 	- not implemented on every arch...
 *
 * Revision 1.25  1997/07/02  14:33:45  pvmsrc
 * beatask() connection attempt tries harder by kicking the daemon
 * 	out of select().
 *
 * Revision 1.24  1997/06/27  20:48:08  pvmsrc
 * Missed AIX arches on write test.
 *
 * Revision 1.23  1997/06/25  22:09:10  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.22  1997/06/16  13:42:44  pvmsrc
 * Oops...  My mistake.  D-Oh.
 * 	- can't use mid as struct pmsg * damn damn damn.
 * 	- wouldn't have been a problem if stupid shmem code didn't
 * 		call mroute() down inside system...  :-/~
 *
 * Revision 1.21  1997/06/12  20:10:46  pvmsrc
 * Made sure all communications for TC_* task control messages
 * 	use the SYSCTX_TC system context.
 * 	- some messages being sent in default context...  D-Oh...
 *
 * Revision 1.20  1997/06/02  13:50:02  pvmsrc
 * Added missing #include host.h for waitc.h.
 *
 * Revision 1.19  1997/06/02  13:24:13  pvmsrc
 * Added test to shmemuptod()
 * 	- so that it creates daemon socket if it doesn't already exist.
 *
 * Revision 1.18  1997/05/29  15:14:05  pvmsrc
 * Removed static decls:
 * 	- pvmtrcsbf, pvmrouteopt now in lpvmglob.c / lpvm.h.
 * 	- pvmtrcmid doesn't exist.
 * Fixed arg casting for SGI's:
 * 	- read_int() takes (int *), not (struct pidtidhdr *)...
 *
 * Revision 1.17  1997/05/27  15:05:30  pvmsrc
 * Added SHMCONN handshake code into pvmbeatask():
 * 	via new call send_my_pid().
 * 	- this removes strange floating tid 0 processes...
 * 	- send_my_pid() calls new function shmemuptod() which is a
 * 		simplified one-way mxfer().
 * 	- shmem node tasks still do not read from pvmd via sockets.
 * Text formatting (JAK).
 *
 * Revision 1.16  1997/05/21  16:01:50  pvmsrc
 * Updated ifdefs to include AIX4MP arch type.
 *
 * Revision 1.15  1997/05/21  15:45:41  pvmsrc
 * pvm_tc_taskexit handled correctly.
 * 	Reports error only when no peer entry exits.
 *
 * Revision 1.14  1997/05/19  15:00:56  pvmsrc
 * Number of small fixes:
 * 	midtobuf, umbuf_new are prototyped correctly now.
 * 	msg contexts set/read correctly in peer_send & peer_recv.
 * 	pvmbeatask():
 * 		shmem key id allocated/located without bizzare attachment error.
 * 		waitc struct is now set using wait_init() before first use... ;)
 * 	pvmmcast():
 * 		mroute is called with a valid mid
 * 		TT direct routes are not checked until ttpcb is defined
 *
 * Revision 1.13  1997/04/30  21:26:07  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.12  1997/04/25  19:19:40  pvmsrc
 * altered to allow integration into 3.4 messaging.
 * Changes:
 * 	-	includes
 * 	- 	added mhp struct def (same as pmsg)
 * 	- 	updated pvmmidh, pvmmidsiz and m_umbs
 * 	- 	added ttpcd lists
 * 	- 	own copy of pvmmcast() (same as lpvm.c)
 * 	- 	own copy of ttpcb_find() minus mpp_ttpcb_find routine XXX
 * 	- 	own copy of post_routedelete() (from lpvmgen.c)
 *
 * Revision 1.11  1997/04/07  21:09:14  pvmsrc
 * pvm_addmhf() - new paramter interface
 *
 * Revision 1.10  1997/04/01  21:28:18  pvmsrc
 * Damn Damn Damn.
 * 	- pvm_recvinfo() returns a bufid, not an index.  Damn.
 *
 * Revision 1.9  1997/04/01  20:48:21  pvmsrc
 * Fixed tracer mbox usage:
 * 	- pvm_getinfo() -> pvm_recvinfo(), new semantics handled (recvinfo
 * 		sets rbuf implicitly, a la pvm_recv, need to save rbuf).
 *
 * Revision 1.8  1997/03/27  19:55:31  pvmsrc
 * Fixed up pvmbeatask() to go get tracer info if spawned from shell:
 * 	- env var info including trace mask, trace buffer size, trace opts.
 * 	- use PVMTRACERCLASS mbox entry to fill in values, if matches
 * 		on trctid, trcctx, and trctag.
 *
 * Revision 1.7  1997/03/06  21:50:21  pvmsrc
 * Yanked out #includes for <netinet/in.h> and <netinet/tcp.h>.
 * 	- dups with lpvm.h #includes...
 *
 * Revision 1.6  1997/01/28  19:26:52  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.5  1996/12/19  19:57:58  pvmsrc
 * Eradicated remainder of old control message interface.
 * 	- replaced pvmmctl() routine with individual messages handlers:
 * 		* pvm_tc_shmat() (lpvmshmem.c only).
 * 		* pvm_tc_conreq().
 * 		* pvm_tc_conack().
 * 		* pvm_tc_taskexit().
 * 	- added appropriate calls to pvm_addmhf() in pvmbeatask().
 * 	- removed calls to pvmmctl() in mroute() (peer_recv() in
 * 		lpvmshmem.c), replaced with new mesg_input() call,
 * 		use new pmsg_setenc() routine to set message encoding.
 *
 * Revision 1.4  1996/12/18  22:27:50  pvmsrc
 * Extracted duplicate versions of routines from lpvm/mimd/shmem.c,
 * 	inserted into shared lpvmgen.c:
 * 	- pvmbailout().
 * 	- pvmlogerror().
 * 	- vpvmlogprintf(), pvmlogprintf().  (hope these work on MPP & shmem)
 * 	- pvmlogperror().
 *
 * Revision 1.3  1996/10/25  13:57:31  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.2  1996/10/24  22:44:35  pvmsrc
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
 * Revision 1.1  1996/09/23  23:44:20  pvmsrc
 * Initial revision
 *
 * Revision 1.39  1995/11/10  21:37:46  manchek
 * check for EINTR from semop in peer_wait
 *
 * Revision 1.38  1995/11/02  16:17:20  manchek
 * free replies to control messages in mxfer
 *
 * Revision 1.37  1995/09/08  17:26:02  manchek
 * aargh forgot semicolon
 *
 * Revision 1.36  1995/09/08  16:56:17  manchek
 * experimental changes to pvm_psend to improve performance (postpone freebuf)
 *
 * Revision 1.35  1995/09/06  17:37:24  manchek
 * aargh, forgot pvm_precv
 *
 * Revision 1.34  1995/09/06  17:31:41  manchek
 * pvm_psend returns not implemented instead of bad param for string type
 *
 * Revision 1.33  1995/09/05  19:20:30  manchek
 * changes from bigapple to make busywait work better
 *
 * Revision 1.32  1995/07/28  16:40:59  manchek
 * wrap HASERRORVARS around errno declarations
 *
 * Revision 1.31  1995/07/28  15:35:19  manchek
 * address of copy databuf wasn't set right in peer_send
 *
 * Revision 1.30  1995/07/28  15:10:37  manchek
 * only send message header on first fragment
 *
 * Revision 1.29  1995/07/28  15:07:30  manchek
 * peer_send wasn't generating message checksum
 *
 * Revision 1.28  1995/07/24  18:45:51  manchek
 * message headers passed in inbox shmpkhdr instead of databuf
 * requires changes to peer_recv, peer_send, msendrecv.
 * use pvmnametag function to print message tags symbolically
 *
 * Revision 1.27  1995/07/19  16:37:51  manchek
 * peer_send returns NotImpl if DataInPlace used
 *
 * Revision 1.26  1995/07/18  17:00:13  manchek
 * added code to generate and check crc on each message (MCHECKSUM).
 * get and put wait-id in message header
 *
 * Revision 1.25  1995/07/12  01:20:40  manchek
 * initialize lots of globals, reset in pvmendtask.
 * pvmendtask checks that globals are set before using them.
 * catch_kill reinstalls old sighldr, doesn't call if it's SIG_IGN,
 * sets pidtid state to ST_FINISH.
 * peer_detach now frees the peer struct
 *
 * Revision 1.24  1995/07/11  18:58:30  manchek
 * peer_wait returns int, -1 if error reading semaphore.
 * mroute and dynbuf check peer_wait return for error
 *
 * Revision 1.23  1995/07/05  16:18:10  manchek
 * exiting task sets pidtid entry state to ST_FINISH instead of ST_EXIT
 *
 * Revision 1.22  1995/07/05  15:36:40  manchek
 * free messages before sending TASKEXIT in pvmendtask
 *
 * Revision 1.21  1995/07/03  20:51:15  manchek
 * made convex poll-type code in peer_wait() the default case
 *
 * Revision 1.20  1995/07/03  20:18:26  manchek
 * hellish cleanup of comments and formatting.
 * removed POWER4 ifdefs.
 * added deadlock detection where task owns all pages in shared
 * segment and needs still another
 *
 * Revision 1.19  1995/06/30  16:24:45  manchek
 * aargh
 *
 * Revision 1.18  1995/06/28  18:17:56  manchek
 * moved check_for_exit from pvmumbuf.c
 *
 * Revision 1.17  1995/06/28  17:59:11  manchek
 * typo
 *
 * Revision 1.16  1995/06/28  15:47:18  manchek
 * added TC_SHMAT connect handshake
 *
 * Revision 1.15  1995/06/19  17:47:58  manchek
 * was packing random string in TC_CONACK message in pvmmctl
 *
 * Revision 1.14  1995/06/02  17:21:34  manchek
 * pvm_start_pvmd ignores INT, QUIT, TSTP signals
 *
 * Revision 1.13  1995/05/30  14:48:37  manchek
 * in pvmendtask() must call peer_cleanup() before erasing our tid
 *
 * Revision 1.12  1995/05/24  19:07:37  manchek
 * small fix in getdsock().
 * changed HPPA shared memory name to HPPAMP
 *
 * Revision 1.11  1995/05/22  19:45:45  manchek
 * added ifdefs for RS6KMP
 *
 * Revision 1.10  1995/05/22  19:09:02  manchek
 * took out ifdefs around read_int().
 * with new startup, can read pvmd sockaddr file only once
 *
 * Revision 1.9  1995/05/18  17:22:08  manchek
 * need to export pvminbox and myshmbufid
 *
 * Revision 1.8  1995/05/17  16:21:17  manchek
 * added support for CSPP shared memory.
 * lots of bug fixes from SGI and Convex.
 * added PVMTASKDEBUG envar.
 * pvm_start_pvmd reads sockaddr from pvmd instead of sleeping on addr file.
 * CSPP port (only) uses new TM_GETOPT message to get trace, output dest.
 *
 * Revision 1.7  1995/02/06  21:43:59  manchek
 * pvmmctl now replies to TC_CONREQ message.
 * better cleanup of message buffers in pvmendtask
 *
 * Revision 1.6  1995/02/01  21:24:47  manchek
 * error 4 is now PvmOverflow
 *
 * Revision 1.5  1994/12/20  16:38:44  manchek
 * added pvmshowtaskid variable
 *
 * Revision 1.4  1994/11/07  22:42:39  manchek
 * general damage control and cleanup:
 * initialize variables
 * send null packets to wake up pvmd instead of reconnecting
 * clean up on catching SIGTERM
 *
 * Revision 1.3  1994/06/30  21:35:40  manchek
 * typo in peer_recv()
 *
 * Revision 1.2  1994/06/04  21:44:31  manchek
 * updated header.
 * changed TM_SET to TM_SETOPT
 *
 * Revision 1.1  1994/06/03  20:38:18  manchek
 * Initial revision
 *
 */

#include <sys/param.h>
#include <stdio.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <unistd.h>
#ifdef IMA_SYMM
#include <parallel/parallel.h>
#endif
#ifdef IMA_KSR1
#include <pthread.h>
#endif
#include <sys/time.h>
#ifdef NEEDSSELECTH
#include <sys/select.h>
#endif

#include <pvm3.h>
#include <pvmproto.h>
#include "pvmalloc.h"
#include "pvmdabuf.h"
#include "pvmfrag.h"
#include "pmsg.h"
#include "listmac.h"
#include "pvmshmem.h"
#include "bfunc.h"
#include "lpvm.h"
#include "host.h"
#include "waitc.h" /* needed by post_routedelete */
#include <pvmtev.h>
#include "tevmac.h"
#include "global.h"

#ifndef max
#define max(a,b)	((a)>(b)?(a):(b))
#endif

#ifndef min
#define min(a,b)	((a)<(b)?(a):(b))
#endif

char *getenv();

void hex_inadport __ProtoGlarp__ (( char *, struct sockaddr_in * ));

/* These prototypes stop compiler casting structs to ints... */
struct pmsg *midtobuf(); /* Oops someone forgot this one KEV/GEF */
struct pmsg *umbuf_new(); /* Here is another :) KEV/GEF */


extern struct encvec *enctovec();
char *pvmgetpvmd();
char *pvmgethome();
char *pvmdsockfile();
char *pvmnametag();

#ifdef IMA_CSPP
int current_node();
#endif


/***************
 **  Globals  **
 **           **
 ***************/

#ifndef HASERRORVARS
extern int errno;					/* from libc */
#endif

/* XXX this should be in a header file not copied */
/* message heap element */

struct mhp {
	int m_free;                 /* free list (next or 0) */
	struct pmsg *m_umb;         /* message or null if on free list */
};


extern int bufpageused;				/* from pvmshmem.c */
extern struct mhp *pvmmidh;			/* from lpvmpack.c */
extern int pvmmidhsiz;				/* from lpvmpack.c */

int shmbufsiz = 0;					/* shared-memory buffer size */

int pvmtidhmask = TIDHOST;			/* mask - host field of tids */
int pvmtidlmask = TIDLOCAL;			/* mask - local field of tids */

int pgsz = 0;						/* system page size */
int pvmpgsz = 0;					/* PVM virtual page size */
char *outmsgbuf = 0;				/* my outgoing message buffer */
int outbufsz = 0;					/* # of frags in outgoing msg buf */
int nbufsowned = 0;					/* num shared frags held by us */
struct pidtid *pidtids = 0;			/* pid -> tid table */
char *infopage = 0;					/* proto, NDF, pid-tid table */
int maxpidtid = 0;					/* size of pid-tid table */
int myshmbufid = -1;				/* ID of shared-memory buffer */
char *pvminbox = 0;					/* incoming message buffer */

struct pmsg *m_umb;            /* message or null if on free list */



/***************
 **  Private  **
 **           **
 ***************/

static char pvmtxt[512];				/* scratch for error log */
static struct pmsg *rxfrag = 0;			/* not-assembled incm msgs */
static struct frag *rxbuf;				/* buffer for incoming packts */

#ifdef IMA_SYMM
static int cpuonline;					/* the number of CPUs available */
static struct shpage *pvmfraginfo;		/* frag locks and ref counts */
#endif
#ifdef USERECVSEMAPHORE
static int mysemid = -1;				/* ID of semaphore to sleep on */
#endif
static char *pvmdinbox = 0;				/* pvmd's incoming message buffer */
static char *pvmdoutbuf = 0;			/* pvmd's outgoing message buffer */
static int pvminboxsz = 0;				/* size of incoming message buffer */
static int mypidtid = -1;				/* my position in pid-tid table */
static int pvmdpid = 0;					/* pvmd's Unix proc ID */
static struct sockaddr_in pvmdsad;		/* address of pvmd socket */
static int pvmdsock = -1;				/* pvmd socket descriptor */

static void (*pvmoldtermhdlr)() = 0;
static int pvmsettermhdlr = 1;
static int outta_here = 0;

/* TT lists used for mcasting [GEF] */
static struct ttpcb *ttlist = 0;        /* dll of connected tasks */
static struct ttpcb *topvmd = 0;        /* default route (to pvmd) */


/**************************
 **  Internal Functions  **
 **                      **
 **************************/

/*	getdsock()
*
*	Get address of pvmd socket, set up pvmdsad.
*/

void
getdsock()
{
	char buf[128];
	int d;
	int n;
	char *p;

	if (!(p = getenv("PVMSOCK"))) {
		if (!(p = pvmdsockfile())) {
			pvmlogerror("getdsock() pvmdsockfile() failed\n");
			return;
		}
		if ((d = open(p, O_RDONLY, 0)) == -1) {
			pvmlogperror(p);
			return;
		}
		n = read(d, buf, sizeof(buf));
		(void)close(d);
		if (n == -1) {
			pvmlogperror("getdsock() read addr file");
			return;
		}
		if (n == 0) {
			pvmlogerror("getdsock() read addr file: wrong length read\n");
			return;
		}
		buf[n] = 0;
		p = buf;
	}

	hex_inadport(p, &pvmdsad);
	pvmdsad.sin_family = AF_INET;
}


/*	prodpvmd()
*
*	wake up the pvmd, which is sleeping on sockets and not shared memory.
*	XXX this sucks.
*/

void
prodpvmd()
{
	static char dummy[TDFRAGHDR];

	if (pvmdsock == -1) {
		if ((pvmdsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			pvmlogperror("prodpvmd() socket");
			return;
		}
		if (connect(pvmdsock, (struct sockaddr*)&pvmdsad, sizeof(pvmdsad))
		== -1) {
			pvmlogperror("prodpvmd() connect");
			close(pvmdsock);
			pvmdsock = -1;
			return;
		}
		BZERO(dummy, sizeof(dummy));

	} else {
		write(pvmdsock, dummy, TDFRAGHDR);
	}
}


/*	peer_wait()
*
*	Check inbox for messages, block on a semaphore if no message.
*/

int
peer_wait()
{
	struct msgboxhdr *inbp = (struct msgboxhdr *)pvminbox;

#ifndef USERECVSEMAPHORE

	PAGELOCK(&inbp->mb_lock);
	while (inbp->mb_read == inbp->mb_last)
#if defined(IMA_SUNMP) && defined(PVMUSEMUTEX)
		cond_wait(&inbp->mb_cond, &inbp->mb_lock);
#endif
#if defined(IMA_RS6KMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP)
		pthread_cond_wait(&inbp->mb_cond, &inbp->mb_lock);
#endif
	PAGEUNLOCK(&inbp->mb_lock);

#else	/*USERECVSEMAPHORE*/

	struct sembuf sops;

	sops.sem_num = 0;
	sops.sem_op = -1;
	sops.sem_flg = 0;
	while (inbp->mb_read == inbp->mb_last) {
		if (pvmdebmask & PDMSEM) {
			sprintf(pvmtxt, "peer_wait(): Waiting on semop id = %d\n", mysemid);
			pvmlogerror(pvmtxt);
		}
		if (semop(mysemid, &sops, 1) == -1) {
			if (errno != EINTR) {
				sprintf(pvmtxt, "peer_wait(): Error waiting for semop id = %d",
						mysemid);
				pvmlogperror(pvmtxt);
				return -1;
			}
		}
		else if (pvmdebmask & PDMSEM) {
			sprintf(pvmtxt, "peer_wait(): Processing Event on semop id = %d\n",
					mysemid);
			pvmlogerror(pvmtxt);
		}
	}

#endif	/*USERECVSEMAPHORE*/
	return 0;
}


/*	pvm_tc_shmat()
*
*	Some Shared Memory Attach Protocol Spanking.
*
*	TC_SHMAT()
*/

static int
pvm_tc_shmat(mid)
	int mid;
{
	int src;					/* sender of request */
	struct peer *pp;
	struct msgboxhdr *dboxp;
	int ackd;					/* allow connection (0) */

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);

	if (!(pp = peer_conn(src, (int *)0)) || pp == (struct peer *)-1L) {
		pvmlogprintf("pvm_tc_shmat() can't connect to src t%x\n", src);
		ackd = -1;
	} else
		ackd = pvmmytid;
	dboxp = (struct msgboxhdr *)pp->p_buf;
	dboxp->mb_attached = ackd;

	pvm_freebuf(mid);
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
	int ackd;					/* allow connection (0) */
	int i;
	int ictx;

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);

	pvmlogprintf("pvm_tc_conreq() TCP conn request from t%x!\n", src);

	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
	ttpro = TDPROTOCOL;
	ackd = 1;
	pvm_pkint(&ttpro, 1, 1);
	pvm_pkint(&ackd, 1, 1);
	pvm_pkstr("");
i = pvmrescode;
pvmrescode = 1;
	ictx = pvm_setcontext(SYSCTX_TC);
	pvm_send(src, TC_CONACK);
	pvm_setcontext(ictx);
pvmrescode = i;
	pvm_freebuf(pvm_setsbuf(sbf));

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

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);

	pvmlogprintf("pvm_tc_conack() unexpected TC msg from t%x!\n", src);

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
	int src;					/* sender of notify */
	struct peer *pp;
	int found_one = 0;			/* did we find a peer? */

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);


	if (!outta_here) {
		int tid;
		extern struct peer *peers;

		pvm_upkint(&tid, 1, 1);

		for (pp = peers->p_link; pp != peers; pp = pp->p_link)
			if (pp->p_tid == tid) {
				pp->p_exited = 1;		/* mark as deleted */
				found_one = 1;
				break;
			}

	}

	if (!found_one) /* then it was unexpected! */
		pvmlogprintf("pvm_tc_taskexit() unexpected TC msg from t%x!\n", src);

	pvm_freebuf(mid);
	return 0;
}


/*	mroute()
*
*   Route a message to a destination.
*   Returns when
*       outgoing message (if any) fully sent and
*       (timed out (tmout) or
*           at least one message fully received)
*   Returns >=0 the number of complete messages downloaded, or
*   negative on error.
*/

int
mroute(mid, dtid, code, tmout)
	int mid;				/* message */
	int dtid;				/* dest */
	int code;				/* type tag */
	struct timeval *tmout;	/* get at least one message */
{
	struct pmsg *txup;			/* tx message or null */
	struct frag *txfp = 0;		/* cur tx frag or null */
	int gotem = 0;				/* count complete msgs downloaded */
	int block;					/* get at least one message */
	int loopcount = 0;
	struct msgboxhdr *inbp;		/* incoming box */
	struct timeval tnow, tstop;
	int sbf;					/* reply to control message */
	int cc;
	int freethis = 0;			/* (control) message came from stack */
	int tstkp = 0;
	int tstk[100];				/* XXX shouldn't be a stack */

	/* XXX do we really have to do this? */
/* 	if ((dtid == TIDPVMD && code == TM_MCA) || dtid == TIDGID) */
/* 		return node_mcast(mid, dtid, code); */

	if (tmout) {
		if (tmout->tv_sec || tmout->tv_usec) {
			pvmgetclock(&tnow);
			tstop.tv_sec = tnow.tv_sec + tmout->tv_sec;
			tstop.tv_usec = tnow.tv_usec + tmout->tv_usec;
			block = 1;
		} else
			block = 0;
	} else {
		block = 1;
		tstop.tv_sec = -1;
		tstop.tv_usec = -1;
	}

	if (txup = midtobuf(mid)) {
		txfp = txup->m_frag->fr_link;
		txfp = txfp->fr_buf ? txfp : 0;
	}

	inbp = (struct msgboxhdr *)pvminbox;

	do {
		if (block && tstop.tv_sec != -1) {
			pvmgetclock(&tnow);
			if (tnow.tv_sec > tstop.tv_sec
			|| (tnow.tv_sec == tstop.tv_sec && tnow.tv_usec >= tstop.tv_usec))
				break;
		}

		if (pvmpolltype == PvmPollSleep
		&& loopcount++ > pvmpolltime && !txfp && tstop.tv_sec == -1) {
			PAGELOCK(&inbp->mb_lock);
			if (inbp->mb_read == inbp->mb_last) {
				inbp->mb_sleep = 1;
				PAGEUNLOCK(&inbp->mb_lock);
				if (peer_wait() == -1)
					return PvmSysErr;
				loopcount = 0;
			} else
				PAGEUNLOCK(&inbp->mb_lock);
		}

		if (inbp->mb_read != inbp->mb_last) {
			if ((sbf = peer_recv(&gotem)) == -1)
				return PvmSysErr;
			if (sbf > 0) {
				if (txfp)
					tstk[tstkp++] = sbf;
				else {
					txup = midtobuf(sbf);
					dtid = txup->m_dst;
					code = txup->m_tag;
					txfp = txup->m_frag->fr_link;
					txfp = txfp->fr_buf ? txfp : 0;
					freethis = 1;
				}
			}
		}

		if (txfp) {
			if ((cc = peer_send(txup, txfp, dtid, code)) < 0)
				return cc;
			if (cc) {
				txfp = txfp->fr_link;
				if (!txfp->fr_buf) {
					if (freethis)
						umbuf_free(txup);
					if (tstkp > 0) {
						txup = midtobuf(tstk[--tstkp]);
						dtid = txup->m_dst;
						code = txup->m_tag;
						txfp = txup->m_frag->fr_link;
						txfp = txfp->fr_buf ? txfp : 0;
						freethis = 1;
					} else
						txfp = 0;
				}
			}
		}

	} while (txfp || (block && !gotem));

	return gotem;
}



/*	peer_recv()
*
*	Receive a message fragment from another task.
*	Reassemble frags into messages and place on rxlist.
*	On completing a control message, call mesg_input(),
*		which consumes it.
*	Returns:
*		0 normally,
*		negative if error,
*		or message handle of a reply control message to be sent.
*/

int
peer_recv(gotem)
	int *gotem;
{
	struct pmsg *rxup;			/* rx message */
	struct pmsg *up;
	struct frag *fp;
	int sdr;
	int src;
	int ff;
	struct peer *pp = 0;
	int next;					/* frag being received */
	struct shmpkhdr *inmsgs;	/* incoming messages */
	struct msgboxhdr *inbp;		/* incoming box */
	int sbf = 0;				/* reply control message */

	inbp = (struct msgboxhdr *)pvminbox;
	inmsgs = (struct shmpkhdr *)(inbp + 1);

	next = (inbp->mb_read + 1) % pvminboxsz;
	sdr = inmsgs[next].ph_sdr;
	fp = fr_new(0);
	if ((sdr & ~pvmtidhmask) != TIDPVMD) {
		if (!(pp = peer_conn(sdr, (int *)0)) || pp == (struct peer *)-1L) {
			sprintf(pvmtxt, "peer_recv() can't connect to sender t%x\n", sdr);
			pvmlogerror(pvmtxt);
			return PvmSysErr;
		}
		fp->fr_dat = pp->p_buf + INBOXPAGE*pgsz + inmsgs[next].ph_dat;
	} else
		fp->fr_dat = pvmdoutbuf + inmsgs[next].ph_dat;
	fp->fr_buf = fp->fr_dat - (inmsgs[next].ph_dat & (pgsz-1))
				+ PVMPAGEHDR;
	fp->fr_max = pvmfrgsiz;
	ff = inmsgs[next].ph_flag;

	src = inmsgs[next].ph_src;
	fp->fr_len = inmsgs[next].ph_len;
	fp->fr_max = pvmfrgsiz;

	if (pvmdebmask & PDMPACKET) {
		sprintf(pvmtxt, "peer_recv() sdr t%x src t%x len %d dst t%x flag %d\n",
			sdr, src, fp->fr_len, inmsgs[next].ph_dst, ff);
		pvmlogerror(pvmtxt);
	}

	/*
	* if start of message, make new umbuf, add to frag pile
	*/
	if (ff & FFSOM) {
		rxup = umbuf_new();
		rxup->m_tag = inmsgs[next].ph_tag;
		rxup->m_ctx = inmsgs[next].ph_ctx;
		rxup->m_enc = inmsgs[next].ph_enc;
		rxup->m_wid = inmsgs[next].ph_wid;
		rxup->m_crc = inmsgs[next].ph_crc;
		rxup->m_src = src;
#ifdef IMA_CSPP
		if (pp && pp->p_node != current_node())
			rxup->m_flag |= MM_DIFFNODE;
#endif
		LISTPUTBEFORE(rxfrag, rxup, m_link, m_rlink);
	}

	/* locate frag's message */

	for (rxup = rxfrag->m_link; rxup != rxfrag; rxup = rxup->m_link)
		if (rxup->m_src == src)
			break;

	if (rxup == rxfrag) {	/* uh oh, no message for it */
		pvmlogerror("peer_recv() frag with no message\n");
		fr_unref(fp);

	} else {
		LISTPUTBEFORE(rxup->m_frag, fp, fr_link, fr_rlink);
		rxup->m_len += fp->fr_len;
	/*
	* if end of message, move to rxlist and count it
	*/
		if (ff & FFEOM) {
			LISTDELETE(rxup, m_link, m_rlink);
#ifdef	MCHECKSUM
			if (rxup->m_crc != umbuf_crc(rxup)) {
				sprintf(pvmtxt,
				"peer_recv() message src t%x tag %d bad checksum\n",
						rxup->m_src, rxup->m_tag);
				pvmlogerror(pvmtxt);
				umbuf_free(rxup);

			} else {
#endif
				pmsg_setenc(rxup, rxup->m_enc);
				mesg_input(rxup);
				(*gotem)++;
#ifdef	MCHECKSUM
			}
#endif
		}
	}
	inbp->mb_read = next;
	return sbf;
}


/*	peer_send()
*
*	Send fragment to another process.
*	Establish connection if necessary, exchange handshake message.
*
*	Returns 1 if sent, 0 otherwise, negative on error.
*/

int
peer_send(txup, txfp, dtid, code)
	struct pmsg *txup;		/* tx message or null */
	struct frag *txfp;		/* cur tx frag or null */
	int dtid;				/* dest */
	int code;				/* type code */
{
	char *cp = 0;				/* points to copy-databuf (if necessary) */
	int ff;
	int loc;					/* location of data in shared segment */
	int next;					/* frag being received */
	struct peer *pp;
	struct shmpkhdr *dmsgs;
	struct msgboxhdr *dboxp;	/* receiving box of peer */

	if (!txfp->fr_u.dab) {
		pvmlogerror("peer_send() PvmDataInPlace not implemented\n");
		return PvmNotImpl;
	}

	/* If its a control message always pass via the daemon */
	/* This avoids the I'cant send to you as you haven't handled my */
	/* tc_shmat handshake *message* yet problem... it was the chicken first */
	/* why? because the daemon is connected as beatask() has worked (maybe) */

	if (((dtid & pvmtidhmask) == (pvmmytid & pvmtidhmask)
	&& (dtid & ~pvmtidhmask) != TIDPVMD) /* to local task */
	&& !(code>=TC_FIRST && code<=TC_LAST) )	/* and not a CTRL TC message */
	{
		int new_connection;

		if (!(pp = peer_conn(dtid, &new_connection)))
			return 0;
		if (pp != (struct peer *)-1L) {
			dboxp = (struct msgboxhdr *)pp->p_buf;
			if (new_connection) {
				struct pmsg *mp;
				int sbf, l;
				struct msgboxhdr *inbp = (struct msgboxhdr *)pvminbox;
				static struct timeval ztv = { 0, 0 };

				sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
				l = TDPROTOCOL;
				pvm_pkint(&l, 1, 1);
				sbf = pvm_setsbuf(sbf);
				inbp->mb_attached = 0;
	/* XXX glggg, could we just return to mroute here instead of calling it? */
				mp = midtobuf(sbf);
				mp->m_ctx = SYSCTX_TC;
				mroute(sbf, dtid, TC_SHMAT, &ztv);
				pvm_freebuf(sbf);
 				while (!inbp->mb_attached)  
					mroute(0, 0, 0, &ztv);		/* avoid deadlock */
				if (inbp->mb_attached != dtid) {
					sprintf(pvmtxt,
						"peer_send: mb_attached: expected %x, got %x\n",
						dtid, inbp->mb_attached);
					pvmlogerror(pvmtxt);
				}
			}

		} else
			dboxp = (struct msgboxhdr *)pvmdinbox;
	} else
		dboxp = (struct msgboxhdr *)pvmdinbox;

	if ((dboxp->mb_last + 1) % pvminboxsz == dboxp->mb_read)
		return 0;		/* full */

	/*
	* if page is private, copy and replace it with one in shared buf
	*/

	if ((loc = txfp->fr_dat - outmsgbuf) > outbufsz * pvmpgsz || loc < 0) {
		if (nbufsowned == outbufsz) {
			int i;

			pvmlogerror("peer_send() Message(s) too long for shared buffer, deadlocked.\n");
			PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
			for (i = 0; i < maxpidtid; i++)
				if (pidtids[i].pt_tid == pvmmytid) {
					pidtids[i].pt_cond = 1;
					break;
				}
			PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
		}

		cp = 0;
		do {
			if (cp)
				da_unref(cp);
			cp = da_new(MAXHDR + txfp->fr_len);
		} while ((loc = cp - outmsgbuf + MAXHDR) > outbufsz*pvmpgsz || loc < 0);

		BCOPY(txfp->fr_dat, cp + MAXHDR, txfp->fr_len);
		txfp->fr_dat = cp + MAXHDR;
		da_unref(txfp->fr_buf);
		txfp->fr_buf = cp;
	}

	if (txfp->fr_rlink == txup->m_frag)
		ff = FFSOM;
	else
		ff = 0;
	if (txfp->fr_link == txup->m_frag)
		ff |= FFEOM;

	if (pvmdebmask & PDMPACKET) {
		sprintf(pvmtxt, "peer_send() dst t%x len %d page %d flag %d\n",
			dtid, txfp->fr_len, loc/pgsz + 1, ff);
		pvmlogerror(pvmtxt);
	}

	dmsgs = (struct shmpkhdr *)(dboxp + 1);
	PAGELOCK(&dboxp->mb_lock);
	next = (dboxp->mb_last + 1) % pvminboxsz;
	/* if receive buffer full, must fail here and try again - sgi:jpb */
	if ( next == dboxp->mb_read ) {		/* full */
		PAGEUNLOCK(&dboxp->mb_lock);
		return 0;
	}
	dmsgs[next].ph_src = pvmmytid;
	dmsgs[next].ph_dst = dtid;
	dmsgs[next].ph_sdr = pvmmytid;
	dmsgs[next].ph_dat = loc;
	dmsgs[next].ph_len = txfp->fr_len;
	dmsgs[next].ph_flag = ff;
	if (ff & FFSOM) {
		dmsgs[next].ph_tag = code;
		dmsgs[next].ph_ctx = txup->m_ctx;
		dmsgs[next].ph_enc = txup->m_enc;
		dmsgs[next].ph_wid = txup->m_wid;
#ifdef	MCHECKSUM
		dmsgs[next].ph_crc = umbuf_crc(txup);
#else
		dmsgs[next].ph_crc = 0;
#endif
		if (pvmdebmask & PDMMESSAGE) {
			sprintf(pvmtxt, "peer_send() dst t%x tag %s ctx %d\n",
				dtid, pvmnametag(dmsgs[next].ph_tag,(int*)0), 
				dmsgs[next].ph_ctx);
			pvmlogerror(pvmtxt);
		} /* logging */

	} else {
		dmsgs[next].ph_tag = 0;
		dmsgs[next].ph_ctx = 0;
		dmsgs[next].ph_enc = 0;
		dmsgs[next].ph_wid = 0;
		dmsgs[next].ph_crc = 0;
	}
	da_ref(txfp->fr_buf);
	dboxp->mb_last = next;

	if (dboxp != (struct msgboxhdr *)pvmdinbox && dboxp->mb_sleep) {

/* #if	defined(IMA_SUNMP) || defined(IMA_RS6KMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP) */
#ifdef  PVMUSEMUTEX
#ifdef	IMA_SUNMP
		cond_signal(&dboxp->mb_cond);
#endif
#if defined(IMA_RS6KMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP)
		pthread_cond_signal(&dboxp->mb_cond);
#endif
#else
		peer_wake(pp);
#endif /* PVMUSEMUTEX */
		dboxp->mb_sleep = 0;
	}

	/* wake up pvmd */

	if (dboxp == (struct msgboxhdr *)pvmdinbox
	&& (dboxp->mb_last + pvminboxsz - 1) % pvminboxsz == dboxp->mb_read) {
		PAGEUNLOCK(&dboxp->mb_lock);
		(void)prodpvmd();
	} else
		PAGEUNLOCK(&dboxp->mb_lock);

	return 1;
}


int
node_mcast(mid, dtid, code)
	int mid;	/* message id */
	int dtid;	/* destination */
	int code;	/* type */
{
	static int *tids = 0;	/* intended recipients of multicast message */
	static int ntids = -1;
	static int ntask;		/* number of tids */
	static struct timeval ztv = { 0, 0 };

	int i;
	long count = 0;
	int cc = 0;
	int dummy;

	/* intercept multicast info */

	if (dtid == TIDPVMD) {
		int sbf = mid;

		pvm_setrbuf(mid);
		pvm_upkint(&ntask, 1, 1);
		if (ntask > ntids) {
			if (tids)
				PVM_FREE(tids);
			tids = TALLOC(ntask, int, "tids");
			ntids = ntask;
		}
		pvm_upkint(tids, ntask, 1);
		/* sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo)); */
		pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		dummy = TIDGID;
		pvm_pkint(&dummy, 1, 1);
		pvm_setrbuf(pvm_setsbuf(sbf));
		return 0;
	}

	for (i = 0; i < ntask; i++)
		if (tids[i] != pvmmytid)
			cc = mroute(mid, tids[i], code, &ztv);

	ntask = 0;

	return cc;
}


/*	msendrecv()
*
*	Single op to send a system message (usually to our pvmd) and get
*	the reply.
*	Returns message handle or negative if error.
*/

int
msendrecv(other, code, context)
	int other;				/* dst, src tid */
	int code;				/* message tag */
	int context;            /* context to use for the message */
{
	static int nextwaitid = 1;	/* I hope */

	int cc;
	struct pmsg *up;

	if (!pvmsbuf)
		return PvmNoBuf;

	/* send code to other */
	if (pvmdebmask & PDMMESSAGE) {
		sprintf(pvmtxt, "msendrecv() to t%x tag %s\n", other,
				pvmnametag(code, (int *)0));
		pvmlogerror(pvmtxt);
	}

   pvmsbuf->m_wid = nextwaitid++;
   pvmsbuf->m_ctx = context;

   if ((cc = mroute(pvmsbuf->m_mid, other, code, (struct timeval *)0)) < 0)
		return cc;

	/* Oops now what? */
	/* 	if (code == TM_MCA)		 */
		/* for node_mcast() */
	/* 		return 1; */

	/* recv code from other */
	for (up = pvmrxlist->m_link; 1; up = up->m_link) {
		if (up == pvmrxlist) {
			up = up->m_rlink;
			if ((cc = mroute(0, 0, 0, (struct timeval *)0)) < 0)
				return cc;
			up = up->m_link;
		}

		if (pvmdebmask & PDMMESSAGE) {
			sprintf(pvmtxt, "msendrecv() from t%x tag %s\n",
					up->m_src, pvmnametag(up->m_tag, (int *)0));
			pvmlogerror(pvmtxt);
		}
		if (up->m_src == other && up->m_tag == code)
			break;
	}
	LISTDELETE(up, m_link, m_rlink);
	if (pvmrbuf)
		umbuf_free(pvmrbuf);
	pvmrbuf = 0;
	if (cc = pvm_setrbuf(up->m_mid))
		return cc;
	return up->m_mid;
}


static void
catch_kill(sig)
	int sig;
{
	signal(SIGTERM, pvmoldtermhdlr);
	if (pvmoldtermhdlr && pvmoldtermhdlr != SIG_IGN)
		pvmoldtermhdlr(sig);
	/* XXX yes, i know the table isn't locked.  we're in a sighandler */
	if (pidtids && pidtids[mypidtid].pt_stat != ST_EXIT)
		pidtids[mypidtid].pt_stat = ST_FINISH;
	pvmendtask();
	exit(sig);
}


static int
read_int(val)
	int *val;
{
	return *val;
}


/*	pvmbeatask()
*
*	Initialize libpvm, config process as a task.
*	This is called as the first step of each libpvm function so no
*	explicit initialization is required.
*
*	Returns 0 if okay, else error code.
*
*	XXX needs work.  lots of inconsistent state is left on error return.
*	I'm working on it
*/

int
pvmbeatask()
{
	struct shmpkhdr *inmsgs;
	struct pidtidhdr *pvminfo;
	struct msgboxhdr *dboxp;		/* receiving box of pvmd */
	int next;
	int altpid;						/* pid of ancestor forked by pvmd */
	int msgcnt;
	char *msgbuf;					/* my message buffer */
	int bufid;
	int i;
	int pid;
	int bid;
	int rc;
	char *p;
	struct pvmminfo minfo;
	int cc = 0;
	int sbf = 0, rbf = 0;			/* saved rx and tx message handles */
	int outtid, outctx, outtag;
	int trctid, trcctx, trctag;
	int need_trcinfo = 0;
	int new_tracer = 0;
	char tmask[ 2 * TEV_MASK_LENGTH ];
	int tbuf, topt;
	int mid;
/*
	union semun {
		int val;
		struct semid_ds *buf;
		ushort *array;
	} sunion;
*/
#ifdef LOG
	char fname[32];
#endif
	int key, firstkeytried;
	int mytid;
	char *pvmtmp;
	int fd;
	TEV_DECLS

	if (pvmmytid != -1)
		return 0;

	pvmmydsig = pvmgetdsig();

	TEV_EXCLUSIVE;

	if ((pvm_useruid = getuid()) == -1) {
		pvmlogerror("pvmbeatask() can't getuid()\n");
		return PvmSysErr;
	}

	pvmchkuid( pvm_useruid );

	if (p = getenv("PVMTASKDEBUG")) {
		pvmdebmask = pvmxtoi(p);
		if (pvmdebmask) {
			sprintf(pvmtxt,"task debug mask is 0x%x\n", pvmdebmask);
			pvmlogerror(pvmtxt);
		}
	}

	pvmmyupid = getpid();

#ifdef LOG
#ifdef IMA_CSPP
	int scid = get_scid();

	pvmtmp = pvmgettmp();

	if (scid > 1)
		sprintf(fname, "%s/pvmt.%d.%d", pvmtmp, pvm_useruid, scid);
	else
#endif
		sprintf(fname, "%s/pvmt.%d", pvmtmp, pvm_useruid);
		if ((fd = open( fname,
				O_RDWR|O_CREAT|O_APPEND|O_EXCL|O_TRUNC, 0600 )) < 0) {
			pvmlogerror("pvmbeatask() can't open log file\n");
		}
		else if ((logfp = fdopen( fd, "a" )) == NULL)
			pvmlogerror("pvmbeatask() can't fdopen log file\n");
#endif

	/*
	* get expected pid from environment in case we were started by
	* the pvmd and someone forked again
	*/

	if (p = getenv("PVMEPID"))
		altpid = atoi(p);
	else
		altpid = 0;

	pgsz = sysconf(_SC_PAGESIZE);
	pvmpgsz = FRAGPAGE*pgsz;
	pvmfrgsiz = pvmpgsz - PVMPAGEHDR;
	pvminboxsz =
		(INBOXPAGE*pgsz - sizeof(struct msgboxhdr))/sizeof(struct shmpkhdr);

	/*
	*	initialize received-message list and fragment reassembly list
	*/

	rxfrag = pmsg_new(1);
	BZERO((char*)rxfrag, sizeof(struct pmsg));
	rxfrag->m_link = rxfrag->m_rlink = rxfrag;

	pvmrxlist = pmsg_new(1);
	BZERO((char*)pvmrxlist, sizeof(struct pmsg));
	pvmrxlist->m_link = pvmrxlist->m_rlink = pvmrxlist;

	peer_init();

	/*
	* SIGTERM handler to clean up our shared memory
	*/

	if (pvmsettermhdlr) {
		pvmoldtermhdlr = signal(SIGTERM, catch_kill);
		pvmsettermhdlr = 0;
	}

	/*
	* get pvmd's message buffer, check protocol revision
	*/

	key = pvmshmkey(0);
	if ((bufid = shmget((key_t)key, 0, PERMS)) == -1) {
		pvmlogperror("pvmbeatask() shmget: can't connect to pvmd");
		return PvmSysErr;
	}
	if ((pvmdinbox = (char *)shmat(bufid, 0, 0)) == (char *)-1L) {
		pvmlogperror("pvmbeatask() shmat pvmd");
		return PvmSysErr;
	}
	infopage = pvmdinbox + INBOXPAGE*pgsz;
	pvmdoutbuf = infopage + pgsz;

	pvminfo = (struct pidtidhdr *)(infopage + PVMPAGEHDR);

	while ( read_int( (int *) &(pvminfo[0]) ) == 0 ) {
		pvmsleep(1);
		if (pvmdebmask & PDMMEM)
			pvmlogerror("Waiting for pvmd to set protocol\n");
	}
	if (pvminfo->i_proto != TDPROTOCOL) {
		sprintf(pvmtxt, "beatask() t-d protocol mismatch (%d/%d)\n",
			TDPROTOCOL, *((int *) &(pvminfo[0])));
		pvmlogerror(pvmtxt);
		return PvmSysErr;
	}

	/*
	* send it a request for connection/task assignment
	*/

	dboxp = (struct msgboxhdr *)pvmdinbox;
	inmsgs = (struct shmpkhdr *)(dboxp + 1);
	PAGELOCK(&dboxp->mb_lock);
	while ((next = (dboxp->mb_last + 1) % pvminboxsz) == dboxp->mb_read) ;
	/* XXX yuck, overloading these fields */
	inmsgs[next].ph_src = pvmmyupid;
	inmsgs[next].ph_dst = altpid;
	inmsgs[next].ph_dat = -1;
	inmsgs[next].ph_sdr = 0;
	inmsgs[next].ph_len = 0;
	inmsgs[next].ph_flag = 0;
	inmsgs[next].ph_tag = 0;
	inmsgs[next].ph_enc = 0;
	inmsgs[next].ph_wid = 0;
	inmsgs[next].ph_crc = 0;
	dboxp->mb_last = next;
	PAGEUNLOCK(&dboxp->mb_lock);

	pvmdpid = pvminfo->i_dpid;
	(void)getdsock();

	PAGELOCK(&dboxp->mb_lock);
	if ((next - 1) % pvminboxsz == dboxp->mb_read) {
		PAGEUNLOCK(&dboxp->mb_lock);
		(void)prodpvmd();
	} else
		PAGEUNLOCK(&dboxp->mb_lock);


	/*
	* get global parameters from pvmd buffer
	*/

/*
	pvmmydsig = pvminfo->i_dsig;
*/
	shmbufsiz = pvminfo->i_bufsiz;
	outbufsz = (shmbufsiz - INBOXPAGE*pgsz)/pvmpgsz;
	nbufsowned = 0;
	pidtids = (struct pidtid *)(pvminfo + 1);
	maxpidtid = (pgsz - sizeof(struct pidtidhdr) - PVMPAGEHDR)/sizeof(struct pidtid);

	/*
	* wait for pvmd to write us an entry in pidtid table
	*/

	pid = altpid ? altpid : pvmmyupid;
	mytid = -1;
	while (mytid == -1) {
		int ntids;			/* number of entries in pid-tid table */

		PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
		ntids = min(maxpidtid, ((struct shmpghdr *)infopage)->pg_ref);
		for (i = 0; i < ntids; i++)
			if (pidtids[i].pt_pid == pid) {
				mytid = pidtids[i].pt_tid;
				pvmmyptid = pidtids[i].pt_ptid;
				/* pidtids[i].pt_pid = pvmmyupid; */
				mypidtid = i;
				break;
			}
		PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
		if (mytid==-1) { /* not entered in pvmd info page yet */
			prodpvmd();	/* kick daemon out of select() in work() */
			sleep_dammit( 10000 );	/* prevent peer page lock race */
									/* on info page */
		}
	}


	/*
	* create shared memory segment (and semaphore)
	* if we can't get the first key, keep trying others
	* XXX this could be moved back to after the protocol num check.
	*/

	firstkeytried = key = pvmshmkey(getpid());
	while (1) {

/* moved here from down there */
myshmbufid = -1;
errno = ENOSPC;
/* static int once = 0; */
/* if (once > 3) */
		myshmbufid = shmget((key_t)key, shmbufsiz, IPC_CREAT|IPC_EXCL|PERMS);
/* else { */
/* myshmbufid = -1; */
/* errno = ENOSPC; */
/* once++; */
/* } */
		if (myshmbufid == -1) {
			if (errno != EACCES && errno != EEXIST) {
				pvmlogperror("pvmbeatask() shmget");
				return PvmSysErr;
			}

		} else {
#ifdef	USERECVSEMAPHORE
			mysemid = semget((key_t)key, 1, IPC_CREAT|IPC_EXCL|PERMS);
			if (mysemid == -1) {
				if (errno != EACCES && errno != EEXIST) {
					pvmlogperror("pvmbeatask() semget");
					shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0);
					return PvmSysErr;

				} else {
					shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0);
				}

			} else {
/*
				sunion.val = 0;
				if (semctl(mysemid, 0, SETVAL, sunion) == -1) {
					pvmlogperror("pvmbeatask() semctl SETVAL");
					return PvmSysErr;
				}
*/
				break;
			}
#else
			break;
#endif
		}

		key = nextpvmshmkey(key);
		if (key == firstkeytried) {
			pvmlogerror("pvmbeatask() can't find a free key!\n");
			return PvmSysErr;
		}
	}

#ifdef IMA_CSPP
	if ((pvminbox = (char *)shm_search(myshmbufid)) == (char *)-1L)
#else
	if ((pvminbox = (char *)shmat(myshmbufid, 0, 0)) == (char *)-1L)
#endif
	{
		pvmlogperror("pvmbeatask() shmat");
		shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0);
		return PvmSysErr;
	}

	outmsgbuf = pvminbox + INBOXPAGE*pgsz;
	msgbufinit(pvminbox);
	/* XXX PAGELOCK(pvminfo); */
	pidtids[mypidtid].pt_key = key;
	pidtids[mypidtid].pt_stat = ST_SHMEM;
#ifdef IMA_CSPP
	pidtids[mypidtid].pt_node = current_node();
#endif
	/* XXX PAGEUNLOCK(pvminfo); */

	pvmmytid = mytid;

	/* OK..
	 * Daemon has been prodded... so lets give it our Unix pid via
	 * the socket so it can match the socket to the process.
	 * That way, when the socket goes EOF pvmd knows which process
	 * it was....
	 * Note we call it this late so that pmsg buffers are available.
	 * Although the message is constructed in shmem its sent via the
	 * socket!
	 */

	send_my_pid();


	/*
	*	Request task trace/output paramters from daemon
	*/

	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
	rbf = pvm_setrbuf(0);
	if ((cc = msendrecv(TIDPVMD, TM_GETOPT, SYSCTX_TM)) > 0) {
		pvm_upkint(&rc, 1, 1);	/* throw out tid, ptid */
		pvm_upkint(&rc, 1, 1);

		pvm_upkint(&outtid, 1, 1);
		pvm_upkint(&outctx, 1, 1);
		pvm_upkint(&outtag, 1, 1);
		if (!pvmtrc.outtid) {
			pvmtrc.outtid = outtid;
			pvmtrc.outctx = outctx;
			pvmtrc.outtag = outtag;
			pvmctrc.outtid = pvmtrc.outtid;
			pvmctrc.outctx = pvmtrc.outctx;
			pvmctrc.outtag = pvmtrc.outtag;
		}

		pvm_upkint(&trctid, 1, 1);
		pvm_upkint(&trcctx, 1, 1);
		pvm_upkint(&trctag, 1, 1);
		if (!pvmtrc.trctid) {
			pvmtrc.trctid = trctid;
			pvmtrc.trcctx = trcctx;
			pvmtrc.trctag = trctag;
			pvmctrc.trctid = pvmtrc.trctid;
			pvmctrc.trcctx = pvmtrc.trcctx;
			pvmctrc.trctag = pvmtrc.trctag;
			new_tracer++;
		}

		cc = 0;
	}
	pvm_freebuf(pvm_setrbuf(rbf));
	pvm_freebuf(pvm_setsbuf(sbf));

	if (p = getenv("PVMCTX"))
		pvmmyctx = pvmstrtoi(p);

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

	/* Setup waitc structures */
	wait_init(pvmmytid, TIDLOCAL);


	BZERO(&minfo, sizeof(minfo));
	minfo.src = -1;
	minfo.ctx = SYSCTX_TC;
	minfo.tag = TC_SHMAT;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_shmat);
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

	return cc;
}


/* XXX shouldn't clean up unless pvmmytid set?  or at least should check. */

int
pvmendtask()
{
	int i;
	struct shmid_ds shmds;
	struct pmsg *up;
	int sbf;
	static struct timeval ztv = { 0, 0 };
	struct peer *pp;
	extern struct peer *peers;
	struct pmsg *mp;

	/*
	* free any left-over messages.
	*/

	pvmsbuf = 0;
	pvmrbuf = 0;
	for (i = 1; i < pvmmidhsiz; i++)
		if (up = pvmmidh[i].m_umb)
			umbuf_free(up);

	/*
	* notify all connected tasks that we are exiting
	*/

	outta_here = 1;

	if (peers) {
		sbf = pvm_initsend(PvmDataFoo);
		mp = midtobuf(sbf);
		mp->m_ctx = SYSCTX_TC;
		pvm_pkint(&pvmmytid, 1, 1);

		for (pp = peers->p_link; pp != peers; pp = pp->p_link)
			if (pp->p_tid && pp->p_tid != pvmmytid)
				mroute(sbf, pp->p_tid, TC_TASKEXIT, &ztv);

		pvm_freebuf(sbf);
	}

	if (pidtids) {
		PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
		pidtids[mypidtid].pt_stat = ST_EXIT;
		PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
	}

	shmdt(pvminbox);
	if (shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0) == -1)
		pvmlogperror("pvmendtask() shmctl RMID");
#ifdef USERECVSEMAPHORE
	if (semctl(mysemid, 0, IPC_RMID) == -1)
		pvmlogperror("pvmendtask() semctl RMID");
	mysemid = -1;
#endif
	pvminbox = 0;
	shmdt(pvmdinbox);
	pvmdinbox = 0;
	infopage = 0;
	pvmdoutbuf = 0;
	pidtids = 0;
	mypidtid = -1;

	peer_cleanup();

	pvmmytid = -1;
	if (pvmdsock != -1) {
		(void)close(pvmdsock);
		pvmdsock = -1;
	}

	/* XXX free rxfrag and rxlist */
#ifdef LOG
	fclose(logfp);
#endif

	return 0;
}


/*	check_for_exit()
*
*	If peer struct for task id marked as exited, check for messages
*	in heap from the task.  If none (borrowing no pages), detach.
*/

void
check_for_exit(src)
	int src;
{
	extern struct peer *peers;
	struct peer *pp;

	for (pp = peers->p_link; pp != peers; pp = pp->p_link)
		if (pp->p_tid == src) {
			if (pp->p_exited) {
				int i;
				int detach = 1;
				struct pmsg *up;

				for (i = 1; i < pvmmidhsiz; i++) {
					if ((up = pvmmidh[i].m_umb) && (up->m_src == src)) {
						detach = 0;
						break;
					}
				}

				if (detach) {
					peer_detach(pp);
				}
			}
			break;
		}
}


/************************
 **  Libpvm Functions  **
 **                    **
 ************************/


int
pvm_getfds(fds)		/* XXX this function kinda sucks */
	int **fds;			/* fd list return */
{
	int cc;

	cc = PvmNotImpl;
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
	int x;
	TEV_DECLS

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

	if ((pvm_useruid = getuid()) == -1) {
		pvmlogerror("can't getuid()\n");
		cc = PvmSysErr;
		goto bail;
	}

	pvmchkuid( pvm_useruid );

	if (!(sfn = pvmdsockfile())) {
		pvmlogerror("pvm_start_pvmd() pvmdsockfile() failed\n");
		cc = PvmSysErr;
		goto bail;
	}

	if (stat(sfn, &sb) != -1) {
		cc = PvmDupHost;
		goto bail;
	}

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
			(void)signal(SIGTSTP, SIG_IGN);
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
/*
	fprintf(stderr, "pvm_start_pvmd: %s\n", buf);
*/

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
	static int last_rbf = 0;

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
		if (last_rbf > 0) {
			pvm_freebuf(last_rbf);
			last_rbf = 0;
		}
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
			last_rbf = cc;
			cc = 0;
		}
		pvm_setrbuf(rbf);
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


/* find dynamic buffer */
char *
dynbuf(tid, len)
	int tid;
	int len;
{
	struct peer *pp;
	int fd;
	char fname[32];
	struct shmpghdr *ph;
	char *pvmtmp;

	while (!(pp = peer_conn(tid, (int *)0)))
		;
/* XXX this doesn't check for peer_conn returning -1 */
	if (len > SHMBUFSIZE && len > pp->p_dlen && pp->p_dbuf) {
		munmap((caddr_t)pp->p_dbuf, SHMBUFSIZE);
		pp->p_dbuf = 0;
	}

	if (!(ph = (struct shmpghdr *)pp->p_dbuf)) {
		pvmtmp = pvmgettmp();
		sprintf(fname, PVMSHMFILE, pvmtmp, tid);
		if ((fd = open(fname, O_CREAT|O_EXCL|O_RDWR, 0600)) == -1 ||
		(pp->p_dbuf = (char *)mmap(0, max(len,SHMBUFSIZE), PROT_READ|PROT_WRITE,
#if defined(IMA_SGIMP) || defined(IMA_SGIMP6) || defined(IMA_SGIMP64)
		MAP_SHARED|MAP_AUTOGROW, fd, 0)) == (char *)-1L)
#else
		MAP_SHARED, fd, 0)) == (char *)-1)
#endif
		{
			pvmlogperror(fname);
			return (char *)-1L;
		}
#ifdef IMA_SUNMP
		/* fill buffer with 0's */
		lseek(fd, len - 1, SEEK_SET);
		write(fd, fname, 1);
#endif
		close(fd);
		pp->p_dlen = len;
	} else if (ph->pg_ref) {	/* previous msg has not been recv'd */
		while (ph->pg_ref)
			if (peer_wait() == -1)
				return (char *)-1L;
#ifdef IMA_SUNMP
	} else if (pp->p_dlen < len) {
		if ((fd = open(fname, O_CREAT|O_EXCL|O_RDWR, 0600)) == -1) {
			pvmlogperror(fname);
			return (char *)-1L;
		}
		lseek(fd, len - 1, SEEK_SET);
		write(fd, fname, 1);
		close(fd);
		pp->p_dlen = len;
#endif
	}

	return pp->p_dbuf;
}

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

	if (!cc) {
#if 0
		if ((tid & pvmtidhmask) == (pvmmytid & pvmtidhmask)
		&& (tid & ~pvmtidhmask) != TIDPVMD) {		/* to local task */
			char *dbuf;

			len += sizeof(struct shmpghdr);
			if ((dbuf = dynbuf(tid, len)) != (char *)-1L) {
				BCOPY(cp, dbuf, len);

			} else
				cc = PvmSysErr;

		} else
#endif /*0*/
		{
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataRaw));
			pvm_pkbyte((char *)cp, len, 1);
			if ((cc = pvm_send(tid, tag)) > 0)
				cc = 0;
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

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


/* XXX copied from lpvm.c as needed by pvmmcast */
/* XXX duplicate code should be cleaned up in a support lpvm* file [GEF97] */

static int
int_compare(i, j)
#if defined(IMA_SGI) || defined(IMA_SGI5) || defined(IMA_SGI6) || defined(IMA_SGI64) || defined(IMA_SGIMP) || defined(IMA_SGIMP6) || defined(IMA_SGIMP64) || defined(IMA_SUN4SOL2)
	const void *i, *j;
#else
	void *i, *j;
#endif
{
	return *((int *)i) - *((int *)j);
}


/* XXX copied from lpvm.c as needed by pvmmcast again */
/* XXX duplicate code should be cleaned up in a support lpvm* file [GEF97] */
/* XXX, XXX!! note that although shmem is a MPP port I had to take out the */
/* XXX ISA_MPP section as MPP_TTPCB_FIND won't work for shmem machines yet! */

/*  ttpcb_find()
*
*   Find a task-task entry by tid in ttlist.
*/

struct ttpcb *
ttpcb_find(tid)
	int tid;        /* peer tid */
{
	struct ttpcb *pcbp;

/* GEF fire fighter (TM) */
/*     return (struct ttpcb*)0; */

#if defined(IMA_MPP)
	if ( pcbp = mpp_ttpcb_find(tid))
		return pcbp;
#endif
	for (pcbp = ttlist->tt_link; pcbp != ttlist; pcbp = pcbp->tt_link)
		if (pcbp->tt_tid >= tid)
			break;
	return (pcbp->tt_tid == tid) ? pcbp : (struct ttpcb*)0;
}



/* XXX copied from lpvm.c as needed by lpvmgen.c */

/*  pvmmcast()
*
*   Multicast a message to a list of destinations.
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
	struct pmsg *mp;

	/*
	* make sorted list of destinations
	*/
	dst = TALLOC(count, int, "mcal");
	BCOPY(tids, dst, count * sizeof(int));
	qsort((char*)dst, count, sizeof(int), int_compare);

	/* 	pvmsbuf->m_ctx = pvmmyctx; */
	/* we are sending mesg mid which may not the same as pvmsbuf->m_mid */
	mp = midtobuf(mid);
	mp->m_ctx = pvmmyctx;

	/*
	* remove duplicates
	*/
	j = 0;
	for (i = 1; i < count; i++)
		if (dst[i] != dst[j])
			dst[++j] = dst[i];
	count = j + 1;

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
		/* if ((pcbp = ttpcb_find(dst[i])) && pcbp->tt_state == TTOPEN) */
			/* mroute(pvmsbuf->m_mid, dst[i], tag, &ztv); */
		/* else */
			dst[j++] = dst[i];
	}
	count = j;      /* destinations still to go */

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
/*             if ((cc = mroute(pvmsbuf->m_mid, pvmmytid | TIDGID, tag, &ztv)) > 0) */
			if ((cc = mroute(mid, pvmmytid | TIDGID, tag, &ztv)) > 0)
				cc = 0;
	}

	PVM_FREE(dst);
	return cc;
}


/* XXX copied from lpvm.c as needed by lpvmgen.c */
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



/*	shmemuptod()
 *
 *	Move message frags between task and pvmd using SOCKETs only.
 *	Returns when
 *		Outgoing message fully sent or
 *		(Timed out (tmout) OR errored.
 *
 *	Returns 0 if OK, or negative on error.
 */

#define HEADER_SIZE (TDFRAGHDR + MSGHDRLEN)

static int
shmemuptod(txup)
	struct pmsg *txup;			/* outgoing message */
{
	struct frag *txfp = 0;		/* cur ogm frag or null */
	struct timeval tin;
	struct timeval tnow;
	struct timeval *tvp;
#ifdef FDSETNOTSTRUCT
	fd_set rfds, wfds;
#else
	struct fd_set rfds, wfds;
#endif
	int ff;
	int n;

	char *txcp = 0;				/* point to remainder of txfp */
	int txtogo = 0;				/* len of remainder of txfp */
	struct sockaddr_in sad;
	int s;
	struct msgid *sendmsg = (struct msgid *) NULL;
	char errtxt[64];
	int upfd;					/* one-way socket to the daemon */

	if (txup) {
	/* transmitting a umsg */
		txfp = txup->m_frag->fr_link;
		if (!txfp->fr_buf) {
			if (!(txfp = fr_new(MAXHDR)))
				return PvmNoMem;
			txfp->fr_dat += MAXHDR;
			LISTPUTBEFORE(txup->m_frag, txfp, fr_link, fr_rlink);
		}

		/* To the daemon ? */
		upfd = pvmdsock;

		/* txup->m_ref++; */
	}

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("shmemuptod() mid %d", (txup ? txup->m_mid : 0));
		if (txup) {
			pvmlogprintf(" ctx %d tag %s",
					txup->m_ctx,
					pvmnametag(txup->m_tag, (int *)0));
		}
	}

	/* must block sending to pvmd */
	tvp = (struct timeval *) NULL;

	/* We haven't sent anything, yet, Build Header */

	txcp = txfp->fr_dat; /* packed data */
	/* XXXX we assume thats its only packed data TMs */

	txtogo = txfp->fr_len;	/* actual data size */

	/*
	 * This is first and last frag. So prepend message header
	 */
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
	ff = FFSOM | FFEOM;	/* Alpha Omega: first and last frag man */

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

	/*
	 * Make sure we have a valid socket address 
	 * Prod will connect us to the daemon if we are not already
	 * connected.
	 */

	if ( pvmdsock < 0 ) {

		prodpvmd();			/* connect to the daemon setting pvmdsock */

		if ( pvmdsock < 0 ) {	/* nested yuck.. */
			pvmlogerror("shmemuptod() Can't get daemon socket open\n");
		    pmsg_unref(txup);   /* unref the message */
			return -1;
		}
	}

	if (pvmdebmask & PDMPACKET) {
		pvmlogprintf("shmemuptod() dst t%x n=%d\n",
				txup->m_dst, txtogo);
	}

	/* XXX yep... blocking loop forever!!!!! */
	/* If I can't get 100+ bytes up that socket we are */
	/* in trouble anyway! */
	while (txtogo) {

#if defined(IMA_RS6K) || defined(IMA_SP2MPI) || defined(IMA_AIX4) \
		|| defined (IMA_AIX4MP) || defined(IMA_AIX5MP) \
		|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
		n = write(pvmdsock, txcp, min(txtogo, 4096));
#else
		n = write(pvmdsock, txcp, txtogo);
#endif

		if (pvmdebmask & PDMPACKET)
			pvmlogprintf("shmemuptod() wrote %d\n", n);

		if (n == -1 && errno != EWOULDBLOCK && errno != EINTR)
		{
			pvmlogperror("shmemuptod() write pvmd sock");
			return PvmSysErr;
		}

		if (n > 0)
		{
			txcp += n;
			txtogo -= n;
		}

	} /* while we have data to send */


	/* we made it here so the message went! */

	pmsg_unref(txup);	/* unref the message */

	return 0;
}


int
send_my_pid()
{
	struct pmsg *up;
	int sbf;
	int cc;

	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
	pvm_pkint(&pvmmyupid, 1, 1);
	sbf = pvm_setsbuf(sbf);

	up = midtobuf(sbf);
	up->m_dst = TIDPVMD;
	up->m_tag = TM_SHMCONN;
	up->m_ctx = SYSCTX_TM;

	/* I would love a mxfer... but we have to use a custom */
	/* one-off routine! */
	/* cc = mxfer(up2, &ztv); */
	cc = shmemuptod (up);

	pvm_freebuf(sbf);
	return cc;
}


int
sleep_dammit( msecs )
int msecs;
{
#if defined(IMA_SUNMP) || defined(IMA_RS6KMP) || defined(IMA_AIX4MP) \
		|| defined(IMA_AIX5MP)
	return( usleep( msecs ) );
#else
	struct timeval biteme;

	biteme.tv_sec = 0;
	biteme.tv_usec = msecs;

	while ( biteme.tv_usec > 1000000 )
	{
		(biteme.tv_sec)++;
		biteme.tv_usec -= 1000000;
	}

	return( select( 0,
#ifdef	FDSETISINT
				(int *) NULL, (int *) NULL, (int *) NULL,
#else
				(fd_set *) NULL, (fd_set *) NULL, (fd_set *) NULL,
#endif
				&biteme ) );
#endif
}


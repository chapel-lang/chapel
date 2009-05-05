
static char rcsid[] =
	"$Id: lpvmmimd.c,v 1.26 2005/05/18 17:56:28 pvmsrc Exp $";

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
 *	lpvmmimd.c
 *
 *	Libpvm core for MPP environment.
 *
 * $Log: lpvmmimd.c,v $
 * Revision 1.26  2005/05/18 17:56:28  pvmsrc
 * Added calls to pvmchkuid()...
 * 	- keep people from running PVM as root...!  :-o
 * (Spanker=kohl)
 *
 * Revision 1.25  2004/01/14 18:50:57  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.24  2001/05/11 17:32:28  pvmsrc
 * Eliminated references to sys_errlist & sys_nerr.
 * 	- unnecessary, and we're whacking that crap anyway.
 * (Spanker=kohl)
 *
 * Revision 1.23  1999/07/08 18:59:59  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.22  1998/02/23  22:51:37  pvmsrc
 * Added AIX4SP2 stuff.
 * (Spanker=kohl)
 *
 * Revision 1.21  1997/12/31  20:50:06  pvmsrc
 * Cleaned Up System Message Handlers.
 * 	- current send / recv buffers now saved before invocation of
 * 		message handler functs.
 * 	- removed manual rbf = setrbuf(mid) saving & resetting in
 * 		handlers...
 * (Spanker=kohl)
 *
 * Revision 1.20  1997/12/01  19:20:41  pvmsrc
 * Replaced #ifdef IMA_OS2 fd_set declarations:
 * 	- new #ifdef FDSETNOTSTRUCT.
 * 	- choose between "fd_set foo" and "struct fd_set foo"...
 * (Spanker=kohl)
 *
 * Revision 1.19  1997/11/04  23:19:18  pvmsrc
 * Cleaned up fd_set stuff (hopefully).
 * (Spanker=kohl)
 *
 * Revision 1.18  1997/10/24  15:17:58  pvmsrc
 * Added TEV_DID_RCX to trace events for Receive Message Context.
 * 	- in pvm_recv(), pvm_trecv(), pvm_nrecv(), and pvm_precv().
 * (Spanker=kohl)
 *
 * Revision 1.17  1997/10/24  14:29:30  pvmsrc
 * Added TEV_DID_MCX / pvmmyctx trace event info to:
 * 	- pvm_send(), pvm_mcast(), pvm_recv(), pvm_trecv(), pvm_nrecv().
 * 	- pvm_psend(), pvm_precv().
 * (Spanker=kohl)
 *
 * Revision 1.16  1997/06/27  17:32:27  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.15  1997/06/12  20:10:44  pvmsrc
 * Made sure all communications for TC_* task control messages
 * 	use the SYSCTX_TC system context.
 * 	- some messages being sent in default context...  D-Oh...
 *
 * Revision 1.14  1997/05/29  15:13:35  pvmsrc
 * Removed static decls:
 * 	- pvmtrcsbf, pvmrouteopt now in lpvmglob.c / lpvm.h.
 * 	- pvmtrcmid doesn't exist.
 *
 * Revision 1.13  1997/04/30  21:26:05  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.12  1997/04/07  21:09:12  pvmsrc
 * pvm_addmhf() - new paramter interface
 *
 * Revision 1.11  1997/04/01  21:28:16  pvmsrc
 * Damn Damn Damn.
 * 	- pvm_recvinfo() returns a bufid, not an index.  Damn.
 *
 * Revision 1.10  1997/04/01  20:48:19  pvmsrc
 * Fixed tracer mbox usage:
 * 	- pvm_getinfo() -> pvm_recvinfo(), new semantics handled (recvinfo
 * 		sets rbuf implicitly, a la pvm_recv, need to save rbuf).
 *
 * Revision 1.9  1997/03/27  19:55:29  pvmsrc
 * Fixed up pvmbeatask() to go get tracer info if spawned from shell:
 * 	- env var info including trace mask, trace buffer size, trace opts.
 * 	- use PVMTRACERCLASS mbox entry to fill in values, if matches
 * 		on trctid, trcctx, and trctag.
 *
 * Revision 1.8  1997/03/06  21:50:19  pvmsrc
 * Yanked out #includes for <netinet/in.h> and <netinet/tcp.h>.
 * 	- dups with lpvm.h #includes...
 *
 * Revision 1.7  1997/01/28  19:26:29  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.6  1996/12/19  20:17:04  pvmsrc
 * Replaced old struct umbuf with new struct pmsg.
 *
 * Revision 1.5  1996/12/19  19:57:56  pvmsrc
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
 * Revision 1.4  1996/12/18  22:27:48  pvmsrc
 * Extracted duplicate versions of routines from lpvm/mimd/shmem.c,
 * 	inserted into shared lpvmgen.c:
 * 	- pvmbailout().
 * 	- pvmlogerror().
 * 	- vpvmlogprintf(), pvmlogprintf().  (hope these work on MPP & shmem)
 * 	- pvmlogperror().
 *
 * Revision 1.3  1996/10/25  13:57:27  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.2  1996/10/24  22:44:33  pvmsrc
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
 * Revision 1.1  1996/09/23  23:44:17  pvmsrc
 * Initial revision
 *
 * Revision 1.20  1995/11/02  16:12:23  manchek
 * free replies to control messages in mxfer
 *
 * Revision 1.19  1995/11/02  16:11:15  manchek
 * removed hdump()
 *
 * Revision 1.18  1995/09/06  17:37:25  manchek
 * aargh, forgot pvm_precv
 *
 * Revision 1.17  1995/09/06  17:32:39  manchek
 * pvm_psend returns not implemented instead of bad param for string type
 *
 * Revision 1.16  1995/09/05  19:16:11  manchek
 * changed some comments
 *
 * Revision 1.15  1995/07/28  16:40:58  manchek
 * wrap HASERRORVARS around errno declarations
 *
 * Revision 1.14  1995/07/18  19:03:35  manchek
 * added code to generate and check crc on each message (MCHECKSUM)
 *
 * Revision 1.13  1995/06/28  18:19:09  manchek
 * do-nothing check_for_exit so one can be in lpvmshmem.c
 *
 * Revision 1.12  1995/06/19  17:49:22  manchek
 * was packing random string in TC_CONACK message in pvmmctl
 *
 * Revision 1.11  1995/06/16  16:07:54  manchek
 * set debug mask and trace mask from environment vars.
 * hack to set trace and output sink and tag for PGON
 *
 * Revision 1.10  1995/06/12  15:58:52  manchek
 * added PGON partition size support
 *
 * Revision 1.9  1995/05/30  17:29:52  manchek
 * Added ifdefs for SP2MPI architecture.
 * Fixed bug in pvm_precv.
 * Use asyncsend and probe for incoming messages in pvm_psend.
 * Prefix my_node, etc. with "pvm" and make them static to avoid name clashes.
 * Fix in mroute() to handle null message
 *
 * Revision 1.8  1995/02/01  21:11:45  manchek
 * error 4 is now PvmOverflow
 *
 * Revision 1.7  1994/12/20  16:39:05  manchek
 * added pvmshowtaskid variable
 *
 * Revision 1.6  1994/11/07  22:39:59  manchek
 * Modify node_mcast() to handle DataInPlace correctly.
 * Modify pvm_precv() to deal with packets from service nodes.  Those
 * packets have PVM headers.
 * Change the limit on pvmfrgsiz in pvm_setopt() to MAXFRAGSIZE.
 * Respond to RouteDirect requests
 *
 * Revision 1.5  1994/09/02  15:25:03  manchek
 * fixed typos in pvm_precv - rtid should be rlen
 *
 * Revision 1.4  1994/06/03  20:38:17  manchek
 * version 3.3.0
 *
 * Revision 1.3  1993/12/20  15:39:15  manchek
 * patch 6 from wcj
 *
 * Revision 1.1  1993/08/30  23:26:48  manchek
 * Initial revision
 *
 */

#include <stdio.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
#include <sys/stat.h>
#ifdef IMA_CM5
#include <unistd.h>
#endif
#include <fcntl.h>
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <errno.h>
#ifdef IMA_PGON
#include <nx.h>
#endif
#ifdef IMA_I860
#include <cube.h>
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
#include <sys/socket.h>
#include <sys/select.h>
#include "mpi.h"
#endif
#include <pvm3.h>
#include <pvmproto.h>
#include "pvmalloc.h"
#include "pvmfrag.h"
#include "pmsg.h"
#include "listmac.h"
#include "pvmdmp.h"
#include "pvmmimd.h"
#include "bfunc.h"
#include "lpvm.h"
#include <pvmtev.h>
#include "tevmac.h"
#include "global.h"

#ifndef max
#define max(a,b)	((a)>(b)?(a):(b))
#endif

char *getenv();
void hex_inadport __ProtoGlarp__ (( char *, struct sockaddr_in * ));

extern struct encvec *enctovec();


/***************
 **  Globals  **
 **           **
 ***************/

#ifndef HASERRORVARS
extern int errno;					/* from libc */
#endif

int pvmtidhmask = TIDHOST;			/* mask - host field of tids */
int pvmtidnmask = TIDNODE;			/* mask - node field of tids */
int pvmtidtmask = TIDPTYPE;			/* mask - proc type field of tids */

int pvmpgonpartsize = 0;			/* pgon parent partition size */


/***************
 **  Private  **
 **           **
 ***************/

static int pvmdebmask = 0;				/* which debugging info */
static char pvmtxt[512];				/* scratch for error log */
static struct pmsg *rxfrag = 0;			/* not-assembled incm msgs */
static int mpierrcode = 0;				/* error code returned by MPI calls */
static struct tmpfrag outfrags[NUMSMHD];/* fragments queued by async send */
static int nextsmhd = 0;				/* index of current isend mhdl */
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		defined(IMA_AIX5SP2)
static struct pmsg *precvlist = 0;		/* not-processed incm msgs */
#endif
static long pvmmyptype = 0;				/* my process type */
static long pvmhostnode = -1;				/* host node number */
static int pvmmynode = -1;				/* my node number */
#ifdef IMA_I860
static int pvmmyvnode = -1;				/* my virtual node number */
#endif
static int mysetpart = 0;				/* host field & set field */

/* static int recvmask = 0x80000000 + (1<<PMTDN) + (1<<PMTNN) + (1<<PMTMCAST);*/


/**************************
 **  Internal Functions  **
 **                      **
 **************************/


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
	int src;				/* sender of request */
	int sbf = 0;			/* return message to send */
	int ttpro;				/* protocol revision */
	int ackd;				/* allow connection if 0 */
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
	int src;				/* sender of reply */

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
	int src;				/* sender of notify */

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);

	pvmlogprintf("pvm_tc_taskexit() unexpected TC msg from t%x!\n",
			src);

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
	int code;				/* type code */
	struct timeval *tmout;	/* get at least one message */
{
	struct pmsg *txup;			/* tx message or null */
	struct frag *txfp = 0;		/* cur tx frag or null */
	int gotem = 0;				/* count complete msgs downloaded */
	struct pmsg *rxup;			/* rx message */
	struct frag *fp;
	char *cp = 0;
	int src;
	int dst;
	int ff;
	int block;					/* get at least one message */
	struct timeval tnow, tstop;
	int len;
	long node;
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	MPI_Status info;
	int mpiflag = 0;
	int mpisiz;
#else
	long info[8];				/* info about pending message */
#endif

	/* XXX do we really have to do this? */
	if ((dtid == TIDPVMD && code == TM_MCA) || dtid == TIDGID)
		return node_mcast(mid, dtid, code);

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
		if (!txfp->fr_buf) {
			txfp = fr_new(MAXHDR);
			txfp->fr_dat += MAXHDR;
			LISTPUTBEFORE(txup->m_frag, txfp, fr_link, fr_rlink);
		}
		node_send(txup, txfp, dtid, code);
		if (!block)
			return 0;
	}

	do {
        if (block && tstop.tv_sec != -1) {
            pvmgetclock(&tnow);
            if (tnow.tv_sec > tstop.tv_sec 
            || (tnow.tv_sec == tstop.tv_sec && tnow.tv_usec >= tstop.tv_usec)) 
                break;
        }

		/* 
		 * Receive PMTPACK msg only; psent msg can have any (user-supplied)
		 * type not equal to PMTPACK. They're handled by precv.
		 */
		node = MPPANYNODE;
		if (MSGPROBE(node, PMTPACK, info)) {
			/* 
			 * 1. Null msg (except I860) from pvmd, followed by the real msg.
			 * 2. Header (inplace), followed by msg body.
			 * 3. Entire msg from another node.
			 */
			if (len = MSGSIZE(info)) {
				fp = fr_new(len);
				cp = fp->fr_dat;
			} 
			if ((mpierrcode = PVMCRECV(node, PMTPACK, cp, len, 0, info))
#if defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_I860)
			&& (mpierrcode < 0)
#endif
			) {
				pvmlogperror("mroute() PVMCRECV");
				return PvmSysErr;
			}
			if (!len) {
				/*
				 * Discard null msg. Now we must wait for the real (packed)
				 * msg, otherwise it could get picked up later by precv and
				 * cause problems.
				 */
        		block = 1;
				continue;
			}
			if ((len = pvmget32(cp + 8) + TDFRAGHDR) != fp->fr_max) {
				struct frag *hdr;		/* buffer for header */

				/* inplace data, head & body sent separately */
				hdr = fp;
				fp = fr_new(len);
				len = hdr->fr_max;
				BCOPY(hdr->fr_dat, fp->fr_dat, len);
				fr_unref(hdr);
				node = MSGSENDER(info);
#ifdef IMA_I860
				/* msg type is (virtual) node # */
				if (_crecv(node, fp->fr_dat + len, fp->fr_max - len) < 0) {
#else
				if ((mpierrcode = PVMCRECV(node, PMTPACK, fp->fr_dat + len, 
				fp->fr_max - len, 0, info))
#if defined(IMA_PGON) || defined(IMA_CM5)
				&& (mpierrcode < 0)
#endif
				) {
#endif
					pvmlogperror("mroute() PVMCRECV msg body");
					return PvmSysErr;
				}
				cp = fp->fr_dat;
			} 

			/* fp->fr_len = fp->fr_max; */
			dst = pvmget32(cp);
			src = pvmget32(cp + 4);
			fp->fr_len = pvmget32(cp + 8);
			ff = pvmget8(cp + 12);
			/* fp->fr_len -= TDFRAGHDR; */
			fp->fr_dat += TDFRAGHDR;
			if (pvmdebmask & PDMPACKET) {
				sprintf(pvmtxt, "mroute() src t%x len %d dst t%x\n",
					src, fp->fr_len, dst);
				pvmlogerror(pvmtxt);
			}
	/*
	* if start of message, make new umbuf, add to frag pile
	*/
			if (ff & FFSOM) {
				cp += TDFRAGHDR;
				fp->fr_len -= TTMSGHDR;
				fp->fr_dat += TTMSGHDR;
				rxup = umbuf_new();
				rxup->m_tag = pvmget32(cp);
				rxup->m_enc = pvmget32(cp + 4);
				rxup->m_wid = pvmget32(cp + 8);
				rxup->m_crc = pvmget32(cp + 12);
				rxup->m_src = src;
				LISTPUTBEFORE(rxfrag, rxup, m_link, m_rlink);
			}

	/* locate frag's message */

			for (rxup = rxfrag->m_link; rxup != rxfrag; rxup = rxup->m_link)
				if (rxup->m_src == src)
					break;

			if (rxup == rxfrag) {	/* uh oh, no message for it */
				pvmlogerror("mroute() frag with no message\n");
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
						"mxinput() message src t%x cod %d bad checksum\n",
								rxup->m_src, rxup->m_tag);
						pvmlogerror(pvmtxt);
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

	} while (block && !gotem);

	return gotem;
}


/* sends a frag to another process */
int
node_send(txup, txfp, dtid, code)
	struct pmsg *txup;		/* tx message or null */
	struct frag *txfp;		/* cur tx frag or null */
	int dtid;				/* dest */
	int code;				/* type code */
{
	int mask = pvmtidhmask;		/* host */
	char *txcp = 0;				/* point to remainder of txfp */
	int txtogo = 0;				/* len of txfp */
	long node;					/* destination node */
	long ptype;					/* destination process type */
	int i;
	int ff;
	char dummy[TDFRAGHDR+TTMSGHDR];	/* for inplace data */
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	MPI_Status mpista;
	int mpiflag = 0;
#endif

#if defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_SP2MPI) \
		|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	mask |= pvmtidtmask;				/* process type */
#endif

	if (TIDISNODE(dtid) && (dtid & mask) == (pvmmytid & mask)) {
		node = dtid & pvmtidnmask;
		ptype = pvmmyptype;			/* send to node directly */
	} else {
		node = pvmhostnode;
		ptype = PVMDPTYPE;			/* send to pvmd first */
	}

	do {

	/* check any pending sends; free data if send is completed */

		if (nextsmhd == NUMSMHD)
			nextsmhd = 0;
		i = nextsmhd;

		while (outfrags[i].tf_mhdl != PVMMHDNULL &&
		ASYNCDONE(outfrags[i].tf_mhdl) > 0) {
			fr_unref(outfrags[i].tf_fp);
#ifdef IMA_CM5
			CMMD_free_mcb(outfrags[i].tf_mhdl);
#endif
			outfrags[i++].tf_mhdl = PVMMHDNULL;
		}

		if (outfrags[nextsmhd].tf_mhdl != PVMMHDNULL) {
			if (pvmdebmask & PDMPACKET)
				pvmlogerror("out of mid's?");
			nextsmhd++;		/* don't get stuck here; check out next mhd */
			continue;
		}

		if (txfp->fr_u.dab) 		/* packed data */
			txcp = txfp->fr_dat;
		else 						/* inplace data */
			txcp = dummy + sizeof(dummy);
		txtogo = txfp->fr_len;

	/*
	* if this is first frag, prepend t-t header
	*/
		ff = 0;
		if (txfp->fr_rlink == txup->m_frag) {
			txcp -= TTMSGHDR;
			txtogo += TTMSGHDR;
			pvmput32(txcp, code);
			pvmput32(txcp + 4, (txup->m_enc == 2 ? pvmmydsig : txup->m_enc));
			pvmput32(txcp + 8, txup->m_wid);
#ifdef	MCHECKSUM
			pvmput32(txcp + 12, umbuf_crc(txup));
#else
			pvmput32(txcp + 12, 0);
#endif
			ff = FFSOM;
		}
		if (txfp->fr_link == txup->m_frag)
			ff |= FFEOM;
	/*
	* prepend t-d header
	*/
		txcp -= TDFRAGHDR;
		pvmput32(txcp, dtid);
		pvmput32(txcp + 4, pvmmytid);
		pvmput32(txcp + 8, txtogo);
		pvmput32(txcp + 12, 0);			/* to keep putrify happy */
		pvmput8(txcp + 12, ff);
		txtogo += TDFRAGHDR;
		if (pvmdebmask & PDMPACKET) {
			sprintf(pvmtxt, "node_send() dst t%x len %d ptype=%ld node=%ld\n",
				dtid, txfp->fr_len, ptype, node);
			pvmlogerror(pvmtxt);
		}

		if (!txfp->fr_u.dab) {           /* inplace data */
			if ((mpierrcode = PVMCSEND(PMTPACK, txcp, txtogo- txfp->fr_len, 
			node, ptype))
#if defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_I860)
			&& (mpierrcode < 0)
#endif
			) {
				pvmlogperror("node_send() csend header");
				return PvmSysErr;
			}
#ifdef IMA_I860
			/* crecv can't select msg with node #; it can only select type */
			if (_csend(pvmmyvnode, txfp->fr_dat, txfp->fr_len, node, 0) < 0) {
				pvmlogperror("node_send() csend inplace data");
				return PvmSysErr;
			}
#else
			/* here the type field is reserved for psend/precv */
			if ((mpierrcode = 
			PVMCSEND(PMTPACK, txfp->fr_dat, txfp->fr_len, node, ptype))
#if defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_I860)
			&& (mpierrcode < 0)
#endif
			) {
				pvmlogperror("node_send() csend inplace data");
				return PvmSysErr;
			}
#endif
		} else {

			if (node != pvmhostnode) {
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
				if (mpierrcode = MPI_Isend(txcp, txtogo, MPI_BYTE, node, 
				PMTPACK, MPI_COMM_WORLD, &outfrags[nextsmhd].tf_mhdl)) {
#else
				if ((outfrags[nextsmhd].tf_mhdl =
				ASYNCSEND(PMTPACK, txcp, txtogo, node, ptype)) < 0) {
#endif
					pvmlogperror("node_send() asyncsend");
					return PvmSysErr;
				}
				/* don't free data 'til frag's sent */
				txfp->fr_u.ref++;		
				outfrags[nextsmhd++].tf_fp = txfp;
			} else {
				/* 
			 	 * In multicast, the fragment header may get overwritten
			 	 * if we use ASYNCSEND. This is OK for node-node send,
			 	 * because the dst field is not used. But pvmd relies on
			 	 * this value to deliever the packet.
			 	 */
				if ((mpierrcode = PVMCSEND(PMTPACK, txcp, txtogo, node, ptype)) 
#if defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_I860)
				&& (mpierrcode < 0)
#endif
				) {
					pvmlogperror("node_send() csend");
					return PvmSysErr;
				}
			}
		}

		txfp = txfp->fr_link;
		if (!txfp->fr_buf)
			txfp = 0;

	} while (txfp);
}


int
node_mcast(mid, dtid, code)
	int mid;	/* message id */
	int dtid;	/* destination */
	int code;	/* type */
{
	int i;
	long count = 0;
	int cc = 0;
	static int *tids;		/* intended recipients of multicast message */
	static int ntask;		/* number of tids */
	int sbf;
	int tmp;
	static struct timeval ztv = { 0, 0 };
#if defined(IMA_PGON)
	long *nodes;
	int mask = pvmtidhmask;		/* host */
	struct pmsg *txup;			/* tx message or null */
	struct frag *txfp = 0;		/* cur tx frag or null */
	int txtogo = 0;				/* len of txfp */
	int ff;
	char *txcp = 0;				/* point to remainder of txfp */
	int len = 0;				/* len of txfp */
	char dummy[TDFRAGHDR+TTMSGHDR];	/* for inplace data */
#endif /*defined(IMA_PGON)*/

	/* intercept multicast info */

	if (dtid == TIDPVMD) {
		pvm_setrbuf(mid);
		pvm_upkint(&ntask, 1, 1);
		tids = TALLOC(ntask, int, "tids");
		pvm_upkint(tids, ntask, 1);
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		tmp = TIDGID;
		pvm_pkint(&tmp, 1, 1);
		pvm_setrbuf(pvm_setsbuf(sbf));
		return 0;
	}
	
#if defined(IMA_PGON)

/* #if defined(IMA_PGON) */
	mask |= pvmtidtmask;		/* process type */
/* #endif */
	nodes = TALLOC(ntask, long, "nodes");
	for (i = 0; i < ntask; i++) {
		if (TIDISNODE(tids[i]) && (tids[i] & mask) == (pvmmytid & mask))
			nodes[count++] = tids[i] & pvmtidnmask;
		else
			cc = mroute(mid, tids[i], code, &ztv);
	}
	if (count) {
		if (txup = midtobuf(mid)) {
			txfp = txup->m_frag->fr_link;
			txfp = txfp->fr_buf ? txfp : 0;
		}
		while (txfp) {
			if (txfp->fr_u.dab) 		/* packed data */
				txcp = txfp->fr_dat;
			else 						/* inplace data */
				txcp = dummy + sizeof(dummy);
			txtogo = txfp->fr_len;

			ff = 0;
			if (txfp->fr_rlink == txup->m_frag) {
				txcp -= TTMSGHDR;
				txtogo += TTMSGHDR;
				pvmput32(txcp, code);
				pvmput32(txcp + 4, 
					(txup->m_enc == 2 ? pvmmydsig : txup->m_enc));
				pvmput32(txcp + 8, txup->m_wid);
#ifdef	MCHECKSUM
				pvmput32(txcp + 12, umbuf_crc(txup));
#else
				pvmput32(txcp + 12, 0);
#endif
				ff = FFSOM;
			}
			if (txfp->fr_link == txup->m_frag)
				ff |= FFEOM;
			txcp -= TDFRAGHDR;
			pvmput32(txcp, dtid);
			pvmput32(txcp + 4, pvmmytid);
			pvmput32(txcp + 8, txtogo);
			pvmput32(txcp + 12, 0);			/* to keep putrify happy */
			pvmput8(txcp + 12, ff);
			txtogo += TDFRAGHDR;
			if (pvmdebmask & PDMPACKET) {
				sprintf(pvmtxt, "node_mcast() len %d\n", txfp->fr_len);
				pvmlogerror(pvmtxt);
			}
			if (count == numnodes() - 1) {
				if (!txfp->fr_u.dab) {           /* inplace data */
					if (_csend(PMTPACK, txcp, txtogo - txfp->fr_len, -1, 
					pvmmyptype) < 0) {
						pvmlogperror("node_mcast() csend header");
						return PvmSysErr;
					}
					if (_csend(PMTPACK, txfp->fr_dat, txfp->fr_len, -1, 
					pvmmyptype) < 0){
						pvmlogperror("node_mcast() csend inplace data");
						return PvmSysErr;
					}
				} else {						/* packed */
					if (_csend(PMTPACK, txcp, (long)txtogo, -1, pvmmyptype) < 0) {
					pvmlogperror("node_mcast() csend");
					return PvmSysErr;
				}
				}
			
			} else {
				if (!txfp->fr_u.dab) {           /* inplace data */
					if (_gsendx(PMTPACK, txcp, txtogo - txfp->fr_len, nodes, 
					count) < 0) {
						pvmlogperror("node_mcast() gsendx header");
						return PvmSysErr;
					}
					if (_gsendx(PMTPACK, txfp->fr_dat, txfp->fr_len, nodes, 
					count) < 0) {
						pvmlogperror("node_send() gsendx inplace data");
						return PvmSysErr;
					}
				} else {
					if (_gsendx(PMTPACK, txcp, (long)txtogo, nodes, count) < 0){
					pvmlogperror("node_mcast() gsendx");
					return PvmSysErr;
				}
			}
			}
			txfp = txfp->fr_link;
			if (!txfp->fr_buf)
				txfp = 0;
		}
	}
	PVM_FREE(nodes);
#endif /*defined(IMA_PGON)*/

#if defined(IMA_CM5) || defined(IMA_I860) || defined(IMA_SP2MPI) \
		|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	for (i = 0; i < ntask; i++)
		cc = mroute(mid, tids[i], code, &ztv);
#endif

	PVM_FREE(tids);
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
msendrecv(other, code)
	int other;				/* dst, src tid */
	int code;				/* message code */
{
	int cc;
	struct pmsg *up;

	if (!pvmsbufmid)
		return PvmNoBuf;

	/* send code to other */
	if (pvmdebmask & PDMMESSAGE) {
		sprintf(pvmtxt, "msendrecv() to t%x code %d\n", other, code);
		pvmlogerror(pvmtxt);
	}
	if ((cc = mroute(pvmsbuf->m_mid, other, code, (struct timeval *)0)) < 0)
		return cc;

	if (code == TM_MCA)		/* for node_mcast() */
		return 1;

	/* recv code from other */
	for (up = pvmrxlist->m_link; 1; up = up->m_link) {
		if (up == pvmrxlist) {
			up = up->m_rlink;
			if ((cc = mroute(0, 0, 0, (struct timeval *)0)) < 0)
				return cc;
			up = up->m_link;
		}

		if (pvmdebmask & PDMMESSAGE) {
			sprintf(pvmtxt, "msendrecv() cmp from t%x code %d\n",
					up->m_src, up->m_tag);
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


#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)

/* Relay messages between pvmd and node tasks. */
void relay(dsock)
	int dsock;					/* pvmd socket */
{
#ifdef FDSETNOTSTRUCT
	fd_set wrk_rfds, wrk_wfds, rfds, wfds;
#else
	struct fd_set wrk_rfds, wrk_wfds, rfds, wfds;
#endif
	int nfds;
	struct timeval tout;
	struct frag *frpvmd = 0;	/* (small) frag from pvmd */
	struct frag *topvmd = 0;	/* (big) frag to pvmd */
	struct frag *frtask = 0;	/* (big) frag from task */
	struct frag *totask;		/* (small) frag being sent to task */
	char *txcp = 0;             /* point to remainder of topvmd */
    int txtogo = 0;             /* len of remainder of topvmd */
	int toread;					/* number of bytes to be read from pvmd */
	int frtogo;					/* len of remainder of a fragment */
	int topvmd_dst;				/* dst of fragment being sent to pvmd */ 
	int topvmd_src;				/* src of fragment being sent to pvmd */
	int len;
	int topvmd_ff;				/* ff of fragment being sent to pvmd */
	int	dst;					/* dst of fragment being sent to node */
	int	node;					/* node number */
	MPI_Request rmhd;			/* msg IDs returned by async recv */
	int n;
	char *cp;
	int err;
	MPI_Status sta;				/* info on pending message */
	int dummy;
	int flag;					/* MPI_Test result */
	struct frag *hdr;

	nfds = dsock + 1;

	frtask = fr_new(MAXFRAGSIZE);
	if (err = MPI_Irecv(frtask->fr_dat, frtask->fr_max, MPI_BYTE, 
	MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &rmhd)) {
		MPI_Error_string(err, pvmtxt, &dummy);
		pvmlogerror(pvmtxt);
		pvmlogerror("relay() MPI_Irecv");
		pvm_exit();
		MPI_Finalize();
		exit(9);
	}

	FD_ZERO(&wrk_rfds);
	FD_ZERO(&wrk_wfds);
	FD_SET(dsock, &wrk_rfds);

	while (1) {			/* ferry messages between pvmd and tasks */
		
		tout.tv_sec = 0;
		tout.tv_usec = 0;
		if (!topvmd && !MPI_Test(&rmhd, &flag, &sta) && flag) {
			topvmd = frtask;
			MPI_Get_count(&sta, MPI_BYTE, &txtogo);		/* w/ header */
/*
sprintf(pvmtxt, "relay() recv %d", txtogo);
pvmlogerror(pvmtxt);
*/
			txcp = topvmd->fr_dat;
			if ((len = pvmget32(txcp + 8) + TDFRAGHDR) != txtogo) {
				/* inplace data, head & body sent separately */
				if (len > MAXFRAGSIZE) {
					hdr = topvmd;
					topvmd = fr_new(len);
					BCOPY(hdr->fr_dat, topvmd->fr_dat, txtogo);
					fr_unref(hdr);
					txcp = topvmd->fr_dat;
				}
				MPI_Recv(topvmd->fr_dat + txtogo, len - txtogo, MPI_BYTE,
				sta.MPI_SOURCE, PMTPACK, MPI_COMM_WORLD, &sta);
				txtogo = len;
			}
			frtask = fr_new(MAXFRAGSIZE);
			/* ready for the next message */
			if (err = MPI_Irecv(frtask->fr_dat, frtask->fr_max, MPI_BYTE, 
			MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &rmhd)) {
				MPI_Error_string(err, pvmtxt, &dummy);
				pvmlogerror(pvmtxt);
				pvmlogerror("relay() MPI_Irecv");
				pvm_exit();
				MPI_Finalize();
				exit(10);
			}
			FD_SET(dsock, &wrk_wfds);
		}
		rfds = wrk_rfds;
		wfds = wrk_wfds;

		if (select(nfds,
#ifdef  FDSETISINT
				(int *)&rfds, (int *)&wfds, (int *)0,
#else
				(fd_set *)&rfds, (fd_set *)&wfds, (fd_set *)0,
#endif
				&tout) == -1 && errno != EINTR) {
			pvmlogperror("relay() select");
			pvm_exit();
			MPI_Finalize();
			exit(11);
		}

		if (FD_ISSET(dsock, &rfds)) {

			if (!frpvmd) {
				frpvmd = fr_new(MAXFRAGSIZE);
				toread = TDFRAGHDR;
			}
			n = read(dsock, frpvmd->fr_dat + frpvmd->fr_len, toread);
/*
sprintf(pvmtxt, "relay() read %d", n);
pvmlogerror(pvmtxt);
*/
			if (n == -1 && errno != EWOULDBLOCK && errno != EINTR) {
				pvmlogperror("relay() read pvmd sock");
				MPI_Finalize();
				exit(12);
			}
			if (!n) {
				/* pvmlogerror("relay() read EOF on pvmd sock\n"); */
				MPI_Finalize();
				exit(13);
			}

			if ((frpvmd->fr_len += n) < TDFRAGHDR) {
				toread -= n;
				continue;
			}
			n = TDFRAGHDR + pvmget32(frpvmd->fr_dat + 8);   /* header + body */
			if (frpvmd->fr_len < n) {
				if (frpvmd->fr_max < n) {					/* n > MAXFRAGSIZ */
					hdr = frpvmd;
					frpvmd = fr_new(n);
					BCOPY(hdr->fr_dat, frpvmd->fr_dat, hdr->fr_len);
					frpvmd->fr_len = hdr->fr_len;
					fr_unref(hdr);
				}
				toread = n - frpvmd->fr_len;
				continue;
			}

			dst = pvmget32(frpvmd->fr_dat);
			node = dst & pvmtidnmask;
			/* inform precv/recv of a packed message */
			while (err = 
			MPI_Bsend(&node, 0, MPI_BYTE, node, PMTPACK, MPI_COMM_WORLD)) {
				MPI_Error_string(err, pvmtxt, &dummy);
				pvmlogerror(pvmtxt);
				sprintf(pvmtxt, "relay() can't send null msg to t%x", dst);
				pvmlogerror(pvmtxt);
			}
			while (err = MPI_Bsend(frpvmd->fr_dat, frpvmd->fr_len, MPI_BYTE, 
			node, PMTPACK, MPI_COMM_WORLD)) {
				MPI_Error_string(err, pvmtxt, &dummy);
				pvmlogerror(pvmtxt);
				sprintf(pvmtxt, "relay() can't send to t%x", dst);
				pvmlogerror(pvmtxt);
			}
			fr_unref(frpvmd);
			frpvmd = 0;
/*
sprintf(pvmtxt, "relay() sent %d to node %d\n", frpvmd->fr_len, (dst & pvmtidnmask));
pvmlogerror(pvmtxt);
*/
				
		}

		if (FD_ISSET(dsock, &wfds)) {
			n = write(dsock, txcp, txtogo);
/*
sprintf(pvmtxt, "relay() wrote %d to pvmd\n", n);
pvmlogerror(pvmtxt);
*/
			if (n == -1 && errno != EWOULDBLOCK && errno != EINTR) {
				pvmlogperror("relay() write pvmd sock");
				MPI_Finalize();
				exit(14);
			}
			if (n > 0 && (txtogo -= n) > 0) 
				txcp += n;
			if (!txtogo) {		/* entire message sent */
				FD_CLR(dsock, &wrk_wfds);
				fr_unref(topvmd);
				topvmd = 0;
			}
		}
	}
}


/* We're the "host" process. Connect to pvmd. */
void
pvmhost()
{
	char *p;
	int dsock;					/* pvmd socket */
	struct sockaddr_in dsadr;	/* address of pvmd socket */
	int n;
	int pvminfo[SIZEHINFO];		/* ntask, hostpart, ptid, MTU, NDF */
	char nullmsg[TDFRAGHDR+TTMSGHDR];

	if (!(p = getenv("PVMSOCK"))) {
		pvmlogerror("pvmhost() getenv() pvmd socket\n");
		MPI_Finalize();
		exit(2);
	}
	mpierrcode = 0;
	if ((dsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		pvmlogperror("pvmhost() socket");
		MPI_Finalize();
		exit(3);
	}
	BZERO((char*)&dsadr, sizeof(dsadr));
	hex_inadport(p, &dsadr);
	dsadr.sin_family = AF_INET;
	n = sizeof(dsadr);
	while (connect(dsock, (struct sockaddr*)&dsadr, n) == -1)
		if (errno != EINTR) {
			pvmlogperror("pvmhost() connect");
			MPI_Finalize();
			exit(4);
		}
#ifndef NOSOCKOPT
	n = 1;
	if (setsockopt(dsock, IPPROTO_TCP, TCP_NODELAY, (char*)&n, sizeof(int))
	== -1) {
		pvmlogperror("pvmhost() setsockopt");
		MPI_Finalize();
		exit(5);
	}
#endif
	if (!(p = getenv("PVMEPID"))) {
		pvmlogerror("pvmhost() getenv() pid\n");
		MPI_Finalize();
		exit(6);
	}
	pvmmyupid = atoi(p);
	BZERO(nullmsg, TDFRAGHDR+TTMSGHDR);
	pvmput32(nullmsg, TIDPVMD);
	pvmput32(nullmsg + 4, pvmmyupid);
	pvmput32(nullmsg + 8, TTMSGHDR);
	pvmput32(nullmsg + 12, 0);			/* to keep putrify happy */
	pvmput8(nullmsg + 12, FFSOM|FFEOM);
	if (write(dsock, nullmsg, TDFRAGHDR+TTMSGHDR) != TDFRAGHDR+TTMSGHDR
	|| read(dsock, pvminfo, SIZEHINFO*sizeof(int)) != SIZEHINFO*sizeof(int)) {
		pvmlogperror("pvmhost() write/read");
		MPI_Finalize();
		exit(8);
	}
	MPI_Bcast(pvminfo, SIZEHINFO, MPI_INT, pvmhostnode, MPI_COMM_WORLD);
	(void)relay(dsock);
}

#endif /*IMA_SP2MPI*/
	
	
/*	pvmbeatask()
*
*	Initialize libpvm, config process as a task.
*	This is called as the first step of each libpvm function so no
*	explicit initialization is required.
*
*	Returns 0 if okay, else error code.
*/

int
pvmbeatask()
{
	int pvminfo[SIZEHINFO];		/* proto, hostpart, ptid, MTU, NDF */
	long rmid = -1;				/* msg ID returned by irecv() */
	int i;
	int cc;
	int ac = 0;
	char *p;
	struct pvmminfo minfo;
	int n;
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	char *msgbuf;				/* buffer for Bsend */
	int msgbufsiz;
#endif
	int need_trcinfo = 0;
	int new_tracer = 0;
	char tmask[ 2 * TEV_MASK_LENGTH ];
	int tbuf, topt;
	int mid, rbf;
	TEVDECLS

	if (pvmmytid != -1)
		return 0;

	TEV_EXCLUSIVE;

	if (p = getenv("PVMTASKDEBUG")) {	/* read the debug mask */
		pvmdebmask = pvmxtoi(p);
		if (pvmdebmask) {
			sprintf(pvmtxt,"task debug mask is 0x%x\n", pvmdebmask);
			pvmlogerror(pvmtxt);
		}
	}

#if defined(IMA_PGON) || defined(IMA_I860)
	if ((rmid = _irecv(PMTHOST, pvminfo, sizeof(pvminfo))) < 0) {
		pvmlogperror("beatask() recv pvminfo");
		return PvmSysErr;
	}
	if ((pvmhostnode = _myhost()) < 0)
		pvmlogperror("beatask() no controlling process");
#endif

	if ((pvm_useruid = getuid()) == -1) {
		pvmlogerror("can't getuid()\n");
		return PvmSysErr;
	}

	pvmchkuid( pvm_useruid );

#ifdef IMA_CM5
	CMMD_enable_host();
	CMMD_receive_bc_from_host(pvminfo, sizeof(pvminfo));
	CMMD_reset_partition_size(pvminfo[0]);
	if ((pvmmynode = CMMD_self_address()) >= pvminfo[0])
		exit(0);		/* no task to spawn on this node  */

	CMMD_fset_io_mode(stdout, CMMD_independent);
	CMMD_fset_io_mode(stderr, CMMD_independent);
	pvmhostnode = CMMD_host_node();
#endif

#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	MPI_Init(&ac, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &pvmmynode);
	MPI_Comm_size(MPI_COMM_WORLD, &pvmhostnode);
	pvmhostnode--;			/* host is last process in group */
	if (!(p = getenv("PVMBUFSIZE")) || !(msgbufsiz = strtol(p, (char**)0, 0)))
		msgbufsiz = MPIBUFSIZ;
	if (!(msgbuf = malloc(msgbufsiz)))
		pvmlogerror("relay() out of memory");
	MPI_Buffer_attach(msgbuf, msgbufsiz);			/* used in psend or relay */
	if (pvmmynode == pvmhostnode)
		(void)pvmhost();
	MPI_Bcast(pvminfo, SIZEHINFO, MPI_INT, pvmhostnode, MPI_COMM_WORLD);
#endif

	pvmmyupid = getpid();

/*
sprintf(pvmtxt, "pvminfo: %d  %d  %d  %d  %d  node=%d\n", pvminfo[0], pvminfo[1],  pvminfo[2], pvminfo[3], pvminfo[4], pvmmynode);
pvmlogerror(pvmtxt);
fflush(stdout);
*/

#ifdef IMA_PGON
	if ((pvmmyptype = myptype()) == INVALID_PTYPE)
		pvmlogerror("beatask() no process type\n");
	pvmmynode = _mynode();
#endif
#ifdef IMA_I860
	pvmmynode = _mydirect();
	pvmmyvnode = _mynode();
	if (_csend(PMTPHYS, &pvmmynode, sizeof(int), pvmhostnode, PVMDPTYPE) < 0)
		pvmlogperror("beatask() can't send to host");
#endif

	/*
	*	initialize received-message list and fragment reassembly list
	*/

	rxfrag = TALLOC(1, struct pmsg, "umb");
	BZERO((char*)rxfrag, sizeof(struct pmsg));
	rxfrag->m_link = rxfrag->m_rlink = rxfrag;

	pvmrxlist = TALLOC(1, struct pmsg, "umb");
	BZERO((char*)pvmrxlist, sizeof(struct pmsg));
	pvmrxlist->m_link = pvmrxlist->m_rlink = pvmrxlist;

#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
	precvlist = TALLOC(1, struct pmsg, "umb");
	BZERO((char*)precvlist, sizeof(struct pmsg));
	precvlist->m_link = precvlist->m_rlink = precvlist;
#endif

	for (i = 0; i < NUMSMHD; i++)
		outfrags[i].tf_mhdl = PVMMHDNULL;

#if defined(IMA_PGON) || defined(IMA_I860)
	_msgwait(rmid);
#endif

#if defined(IMA_PGON) || defined(IMA_I860) || defined(IMA_SP2MPI) \
		|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	if (pvminfo[0] != TDPROTOCOL) {
		sprintf(pvmtxt, "beatask() t-d protocol mismatch (%d/%d)\n",
			TDPROTOCOL, pvminfo[0]);
		pvmlogerror(pvmtxt);
		return PvmSysErr;
	}
#endif

	n = 1;

	mysetpart = pvminfo[n++];
	pvmmyptid = pvminfo[n++];
	pvmudpmtu = pvminfo[n++];
	pvmmydsig = pvminfo[n++];

#if defined(IMA_PGON)
	pvmpgonpartsize = pvminfo[n++];
#endif

	if (!pvmtrc.outtid) {
		pvmtrc.outtid = pvminfo[n++];
		pvmtrc.outctx = pvminfo[n++];
		pvmtrc.outtag = pvminfo[n++];
		pvmctrc.outtid = pvmtrc.outtid;
		pvmctrc.outctx = pvmtrc.outctx;
		pvmctrc.outtag = pvmtrc.outtag;
	}
	if (!pvmtrc.trctid) {
		pvmtrc.trctid = pvminfo[n++];
		pvmtrc.trcctx = pvminfo[n++];
		pvmtrc.trctag = pvminfo[n++];
		pvmctrc.trctid = pvmtrc.trctid;
		pvmctrc.trcctx = pvmtrc.trcctx;
		pvmctrc.trctag = pvmtrc.trctag;
		new_tracer++;
	}

	pvmmytid = mysetpart + pvmmynode;
	pvmfrgsiz = pvmudpmtu;

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
		if ( new_tracer ) need_trcinfo++
	}
 
	BCOPY(pvmtrc.tmask, pvmctrc.tmask, TEV_MASK_LENGTH);

	/* get trace buffering from envar */

	if ((p = getenv("PVMTRCBUF")))
		pvmtrc.trcbuf = atoi( p );
	else {
		pvmtrc.trcbuf = 0;
		if ( new_tracer ) need_trcinfo++
	}
	
	pvmctrc.trcbuf = pvmtrc.trcbuf;

	/* get trace options from envar */

	if ((p = getenv("PVMTRCOPT")))
		pvmtrc.trcopt = atoi( p );
	else {
		pvmtrc.trcopt = 0;
		if ( new_tracer ) need_trcinfo++
	}
	
	pvmctrc.trcopt = pvmtrc.trcopt;

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
}


int
pvmendtask()
{
#if defined(IMA_PGON) || defined(IMA_I860) || defined(IMA_SP2MPI) \
		|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	int i;
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	MPI_Status mpista;
#endif

	for (i = 0; i < NUMSMHD; i++)
		if (outfrags[i].tf_mhdl != PVMMHDNULL)
			ASYNCWAIT(outfrags[i].tf_mhdl);
#endif

	if (pvmmytid != -1) {
		pvmmytid = -1;
	}

#ifdef IMA_CM5
	CMMD_all_msgs_wait();
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	MPI_Finalize();
#endif

	/* XXX free rxfrag and rxlist */

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
	int cc;

	cc = PvmNotImpl;
	return (cc < 0 ? lpvmerr("pvm_getfds", cc) : cc);
}


/*
 * receive a message for precv
 * returns 1 if message is received, 0 if not.
 * returns -1 on error
 */
int
mpprecv(tid, tag, cp, len, rtid, rtag, rlen)
	int tid;
	int tag;
	void *cp;
	int len;
	int *rtid;
	int *rtag;
	int *rlen;
{
#if defined(IMA_PGON) || defined(IMA_CM5)
	long info[8];
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	MPI_Status info;
	int mpiflag = 0;
	int mpisiz;
#endif
	long node;
	struct pmsg *up;
	int cc;

	node = (tid == -1) ? MPPANYNODE : tid & pvmtidnmask;
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
	for (up = precvlist->m_link; up != precvlist; up = up->m_link)
		/* message picked up by psend */
		if ((tag == -1 || tag == up->m_tag) 
		&& (tid == -1 || node == up->m_src)) {
			struct frag *fp;

			fp = up->m_frag->fr_rlink;
			if (fp->fr_max < len)
				len = fp->fr_max;
			BCOPY(fp->fr_dat, cp, len);
			if (rlen)
				*rlen = len;
			if (rtid)
				*rtid = up->m_src + (pvmmytid & ~pvmtidnmask);
			if (rtag)
				*rtag = up->m_tag;
			LISTDELETE(up, m_link, m_rlink);
			if (pvmdebmask & PDMPACKET) {
				sprintf(pvmtxt, "pvm_precv() task %x len %d tag %d\n",
					up->m_src + (pvmmytid & ~pvmtidnmask), 
					rlen ? *rlen : fp->fr_max, rtag ? *rtag : up->m_tag);
					pvmlogerror(pvmtxt);
			}
			umbuf_free(up);
			return 1;
		}
#endif
	
	/* 
	 * Msg routed from pvmd has type PMTPACK, so it may have already
	 * been picked up by the last pvm_recv. If so, we call pvm_recv
	 * to process it. Note we only need consider the case tid = -1,
	 * because msg psent by another node cannot have type PMTPACK.
	 */
	if (tid == -1) {
		for (up = pvmrxlist->m_link; up != pvmrxlist; up = up->m_link)
			if (tag == -1 || tag == up->m_tag)
				return 0;			/* go on to pvm_recv */
		if (tag != -1)
			/* wait for msg with tag or PMTPACK */
			while (!MSGPROBE(MPPANYNODE, tag, info))
				if (MSGPROBE(pvmhostnode, PMTPACK, info)) {
					if ((cc = mroute(0, 0, 0, (struct timeval *)0)) < 0)
						return cc;
					for (up = pvmrxlist->m_link; up != pvmrxlist; 
					up = up->m_link)
						if (tag == up->m_tag)
							return 0;			/* go on to pvm_recv */
				}
	}

	/* 1) from another node: 1a) tid = -1; 1b) tid given; 2) tag = -1 */
#ifndef IMA_PGON
	if (tag == -1)
		tag = MPPANYTAG;
#endif
	if ((mpierrcode = 
	PVMCRECV(node, tag, (char*)cp, len, pvmmyptype, info)) 
#if defined(IMA_PGON) || defined(IMA_CM5)
	&& (mpierrcode < 0)
#endif
	) {
		pvmlogperror("precv() PVMCRECV");
		return PvmSysErr;
	} 
	if ((node = MSGSENDER(info)) == pvmhostnode)
		/* got null msg; go on to pvm_recv */
		return 0;
			
	if (rlen)
		*rlen = MSGSIZE(info);
	if (rtid)
		*rtid = node + (pvmmytid & ~pvmtidnmask);
	if (rtag)
		*rtag = MSGTAG(info);
	if (pvmdebmask & PDMPACKET) {
		sprintf(pvmtxt, "pvm_precv() task %x len %d tag %d\n",
			node + (pvmmytid & ~pvmtidnmask), rlen ? *rlen : MSGSIZE(info), 
			rtag ? *rtag : MSGTAG(info));
		pvmlogerror(pvmtxt);
	}
	return 1;
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
#if defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_SP2MPI) \
		|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
		int mask = pvmtidhmask | pvmtidtmask;		/* same partition */

		if (tid == -1 || (TIDISNODE(tid) && (tid & mask) == (pvmmytid & mask)))
			/*
			 * 1) Don't know where msg is coming from.
			 * 2) Expect msg from a node in the same partition.
			 */
			if (cc = mpprecv(tid, tag, cp, len, &src, &mc, &nb)) {
				/* 1) Whole msg recv'd. 2) Error */
				if (rtid)
					*rtid = src;
				if (rtag)
					*rtag = mc;
				if (rlen)
					*rlen = nb;
				goto done;
			}
#endif 
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
	}

done:

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

#if defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_SP2MPI) \
		|| defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)

		int mask = pvmtidhmask;     		/* host */
		long node = tid & pvmtidnmask;
#ifdef IMA_CM5
		CMMD_mcb mhdl;
		int info;
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
		MPI_Request mhdl;
		MPI_Status info, mpista;
		int mpiflag = 0;
		int mpisiz;
#endif

		mask |= pvmtidtmask;                /* process type */
		if (TIDISNODE(tid) && (tid & mask) == (pvmmytid & mask)) {
			if (pvmdebmask & PDMPACKET) {
				sprintf(pvmtxt, 
					"pvm_psend() dst t%x len %d ptype=%ld node=%ld\n",
					tid, len, pvmmyptype, tid & pvmtidnmask);
				pvmlogerror(pvmtxt);
			}
#ifdef IMA_PGON
			if (PVMCSEND(tag, cp, len, node, pvmmyptype) < 0)
#endif
#ifdef IMA_CM5
			if ((mhdl = ASYNCSEND(tag, cp, len, node, pvmmyptype)) < 0)
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
			if (mpierrcode = MPI_Isend(cp, len, MPI_BYTE, node,
                tag, MPI_COMM_WORLD, &mhdl))
#endif
			{
				pvmlogperror("psend() PVMCSEND");
				cc = PvmSysErr;
				goto done;
			}
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2)
			while (!ASYNCDONE(mhdl)) {
				int rtag;

				/* 
				 * Psend is really a buffered send. We use async send to
				 * avoid the overhead of buffering (50%). The behavior of
				 * psend is now similar to RouteDirect: the sender blocks
				 * until the other party signals its intention to receive,
				 * but it can accept incoming messages in the meantime.
				 * This avoids deadlock when two tasks send messages to
				 * each other at the same time.
				 */
				if (MSGPROBE(MPPANYNODE, MPPANYTAG, info)) {
					if ((rtag = MSGTAG(info)) != PMTPACK) {
						struct pmsg *rxup;
						struct frag *fp;

						rxup = midtobuf(umbuf_new());
						rxup->m_src = MSGSENDER(info);
						rxup->m_tag = rtag;
						fp = fr_new(MSGSIZE(info));
						LISTPUTBEFORE(rxup->m_frag, fp, fr_link, fr_rlink);
						if ((mpierrcode = PVMCRECV(rxup->m_src, rtag, 
						fp->fr_dat, fp->fr_max, pvmmyptype, info)) 
#ifdef IMA_CM5
						&& (mpierrcode < 0)
#endif
						) {
							pvmlogperror("psend() PVMCRECV");
							cc = PvmSysErr;
							goto done;
						} 
						LISTPUTBEFORE(precvlist, rxup, m_link, m_rlink);
					} else {
						if ((cc = mroute(0, 0, 0, (struct timeval *)0)) < 0)
							goto done;
					}
				}
			}
#ifdef IMA_CM5
			CMMD_free_mcb(mhdl);
#endif
#endif /*defined(IMA_CM5) || defined(IMA_SP2MPI)*/
		} else

#endif /*defined(IMA_PGON) || defined(IMA_CM5) || defined(IMA_SP2MPI)*/

		{
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataInPlace));
			pvm_pkbyte((char *)cp, len, 1);
			if ((cc = pvm_send(tid, tag)) > 0)
				cc = 0;
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

done:

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




static char rcsid[] =
	"$Id: mppmsg.c,v 1.13 2005/08/22 15:13:19 pvmsrc Exp $";

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

/* Define low level message functions for the MPP port */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#if defined(NEEDSSELECTH)
#include <sys/select.h>
#endif
#include <errno.h>
#include <sys/stat.h>
#include <sys/socket.h>
#ifndef NOUNIXDOM
#include <sys/un.h>
#endif
#include <netinet/in.h>
#ifndef NO_NETINET_TCP_H
#include <netinet/tcp.h>
#endif
#include <pvm3.h>
#include <pvmproto.h>
#include "lpvm.h"
#include <pvmtev.h>
#include "global.h"
#include "pvmalloc.h"
#include "listmac.h"
#include "lmsg.h"
#include "mppmsg.h"
#include "mppchunk.h"
#include "pvmmimd.h"
#include "bfunc.h"


/* --- external declarations */

void hex_inadport __ProtoGlarp__ (( char *, struct sockaddr_in * ));

extern int errno;
extern int pvmdebmask;           /* from pvmd.c */

#if defined(IMA_NODE)
extern int pvmhostnode;
#endif

#if defined(IMA_PGON)
#include <nx.h>

#define ASYNCSEND(_app, _tag, _buf, _len, _dest, _partid, _mid)  \
		_isend((long)(_tag), _buf, (long)(_len), (long)(_dest), (long)(_partid))

#define ASYNCRECV(_app, _src, _tag, _buf, _len, _partid, _info, _mid) \
		_irecvx((long)(_tag), _buf, (long)(_len), (long)(_src), \
				(long)(_partid), (long *)_info)

#define MSGDONE(_app,_mid,_flag,_status) \
		_msgdone(*(_mid))

#define MSGMERGE(_mid1, _mid2) \
		_msgmerge(*(_mid1), *(_mid2))

#define MSGTAG(_info) \
		_info[0]

#define MSGLEN(_info, _len) \
		_info[1]

#define MSGSRC(_info) \
		_info[2]

/* Host (daemon) uses the same nx routines that nodes do */

#define HOSTASYNCSEND ASYNCSEND
#define HOSTASYNCRECV ASYNCRECV
#define HOSTMSGMERGE MSGMERGE
#define HOSTMSGDONE MSGDONE
#define HOSTMSGTAG MSGTAG
#define HOSTMSGLEN MSGLEN
#define HOSTMSGSRC MSGSRC


#endif /* IMA_PGON */

#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
#if defined(IMA_NODE)
#include <mpi.h>

#define ASYNCSEND(_app, _tag, _buf, _len, _dest, _partid, _mid) \
		MPI_Isend((void *) (_buf), len, MPI_BYTE, _dest,\
				 _tag, MPI_COMM_WORLD, _mid)

#define ASYNCRECV(_app, _src, _tag, _buf, _len, _partid, _info, _mid) \
		MPI_Irecv((void *) (_buf), _len, MPI_BYTE, _src, _tag,\
				MPI_COMM_WORLD, _mid)

#define MSGDONE(_app,_mid,_flag,_status) \
		MPI_Test(_mid, _flag, _status )

#define MSGTAG(_info) \
		(_info)->MPI_TAG

#define MSGMERGE(_mid1, _mid2) NULL 
		

#define MSGLEN(_info, _len) \
		MPI_Get_count(_info, MPI_BYTE, _len) 

#define MSGSRC(_info) \
		(_info)->MPI_SOURCE

#else /* IMA_NODE */

#define ASYNCSEND(_app, _tag, _buf, _len, _dest, _partid, _mid) 0

#define ASYNCRECV(_app, _src, _tag, _buf, _len, _partid, _info, _mid) 0

#define MSGDONE(_app,_mid,_flag,_status) 0 

#define MSGTAG(_info) 0 

#define MSGMERGE(_mid1, _mid2) NULL 
		
#define MSGLEN(_info, _len) 0 

#define MSGSRC(_info) 0 

#endif /*IMA_NODE*/

/* Relay process uses the same  routines that nodes do */

#define HOSTASYNCSEND ASYNCSEND
#define HOSTASYNCRECV ASYNCRECV
#define HOSTMSGMERGE MSGMERGE
#define HOSTMSGDONE MSGDONE
#define HOSTMSGTAG MSGTAG
#define HOSTMSGLEN MSGLEN
#define HOSTMSGSRC MSGSRC


#endif /* IMA_SP2MPI */

char *getenv();


/* ----------( Node -- Node ) Routines ----------- */
static int
pvm_inodesend(appid, tag, buffer, len, dest, partid, mid)
int appid;
int tag;
char *buffer;
int len;
int dest;
int partid;
msgmid_t *mid;
{
#if defined(IMA_PGON)
	return ((*mid) = ASYNCSEND(appid, tag, buffer, len, dest, partid, mid));
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	return  ASYNCSEND(appid, tag, buffer, len, dest, partid, mid);
#endif
}


static int
pvm_inoderecv(appid, src, tag, buffer, len, partid, info, mid)
int appid;
int src;
int tag;
char *buffer;
int len;
int partid;
int *info;
msgmid_t *mid;
{
#if defined(IMA_PGON)
	return ((*mid)=ASYNCRECV(appid, src, tag, buffer, len, partid, info, mid));
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	return ASYNCRECV(appid, src, tag, buffer, len, partid, info, mid);
#endif
}


static msgmid_t 
pvm_nodemsgmerge (mid1, mid2)
msgmid_t *mid1;
msgmid_t *mid2;
{
	return (msgmid_t) MSGMERGE(mid1, mid2);
}

static int
pvm_nodemsgdone (appid, mid, info)
int appid;
msgmid_t *mid;
info_t *info;
{
int flag;
#if defined(IMA_PGON)
	return MSGDONE(appid, mid, &flag, info);
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	MSGDONE(appid, mid, &flag, info);
	return flag;
#endif
}

static int
pvm_nodemsgsrc( info )
info_t *info;
{
	return MSGSRC(info);
}

static int
pvm_nodemsgtag( info )
info_t *info;
{
	return MSGTAG(info);
}

static int
pvm_nodemsglen( info )
info_t *info;
{
int len;
#if defined(IMA_PGON)
	return MSGLEN(info, &len);
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	MSGLEN(info,&len);
	return len;
#endif
}

/* ---------- ( Host <--> Node ) Routines ----------- */
static int
pvm_ihostsend(appid, tag, buffer, len, dest, partid, mid)
int appid;
int tag;
char *buffer;
int len;
int dest;
int partid;
msgmid_t *mid;
{
#if defined(IMA_PGON)
	return ((*mid) = HOSTASYNCSEND(appid, tag, buffer, len, dest, partid, mid));
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	return  HOSTASYNCSEND(appid, tag, buffer, len, dest, partid, mid);
#endif
}


static int
pvm_ihostrecv(appid, src, tag, buffer, len, partid, info, mid)
int appid;
int src;
int tag;
char *buffer;
int len;
int partid;
int *info;
msgmid_t *mid;
{
#if defined(IMA_PGON)
	return ((*mid)=HOSTASYNCRECV(appid, src, tag, buffer, len, partid, info, mid));
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	return HOSTASYNCRECV(appid, src, tag, buffer, len, partid, info, mid);
#endif
}

static msgmid_t 
pvm_hostmsgmerge (mid1, mid2)
msgmid_t *mid1;
msgmid_t *mid2;
{
	return HOSTMSGMERGE(mid1, mid2);
}

static int
pvm_hostmsgdone (appid, mid, info)
int appid;
msgmid_t *mid;
info_t *info;
{
int flag;
#if defined(IMA_PGON)
	return HOSTMSGDONE(appid, mid, &flag, info);
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	HOSTMSGDONE(appid, mid, &flag, info);
	return flag;
#endif
}

static int
pvm_hostmsgsrc( info )
info_t *info;
{
	return HOSTMSGSRC(info);
}

static int
pvm_hostmsgtag( info )
info_t *info;
{
	return HOSTMSGTAG(info);
}

static int
pvm_hostmsglen( info )
info_t *info;
{
int len;
#if defined(IMA_PGON)
	return HOSTMSGLEN(info, &len);
#endif
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	HOSTMSGLEN(info,&len);
	return len;
#endif
}


static MSGFUNC nodemsgfunc = { pvm_inodesend,
						pvm_inoderecv,
#if defined(IMA_PGON)
						pvm_nodemsgmerge,
#else
						NULL,
#endif
						pvm_nodemsgdone,
						pvm_nodemsglen,
						pvm_nodemsgsrc,
						pvm_nodemsgtag };

static MSGFUNC hostmsgfunc = { pvm_ihostsend,
						pvm_ihostrecv,
#if defined(IMA_PGON)
						pvm_hostmsgmerge,
#else
						NULL,
#endif
						pvm_hostmsgdone,
						pvm_hostmsglen,
						pvm_hostmsgsrc,
						pvm_hostmsgtag };


/* -------- External Routines --------- */
MSGFUNC_PTR 
pvm_nodemsgfunc()
{
	return &nodemsgfunc;
} 

MSGFUNC_PTR
pvm_hostmsgfunc()
{
	return &hostmsgfunc;
}


/* Define some intialization functions that are only used by node procs
*/
#if defined(IMA_NODE)

/* ------ messsage system initialization, destruction ----- */
int
pvm_mpp_message_init(node, partsize, host, partid)
int *node;
int *partsize;
int *host; 
int *partid;
{
	char errtxt[64];
	char *msgbuf;
	char *p;

	int ac = 0;
	int msgbufsiz;

	*node = -1;			/* initialize to bogus values */
	*partsize = -1;
	*host = -1;
	*partid = 0;

#if defined(IMA_PGON)

	if ( (*partid = _myptype()) == INVALID_PTYPE ) 
	{
        pvmlogerror("mpp_message_init() no process type\n");
		return PvmSysErr;
	}

	_setptype(0);		/* always set ptype to 0 */

	*node = _mynode();
	*partsize = _numnodes();
	*host = *partsize;
#endif

#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
    MPI_Init(&ac, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, node);
    MPI_Comm_size(MPI_COMM_WORLD, partsize);
	*host = *partsize - 1;		/* host is last process in the group */

/*	if (!(p = getenv("PVMBUFSIZE")) || !(msgbufsiz = strtol(p, (char**)0, 0)))
        msgbufsiz = MPIBUFSIZ;
    if (!(msgbuf = malloc(msgbufsiz)))
        pvmlogerror("relay() out of memory");
    MPI_Buffer_attach(msgbuf, msgbufsiz);           used in psend or relay */

    	if (*node == *host)	/* I'm the host, run the host proc */
	{
        	(void)pvmhost();
	}
	else
		

/*	MPI_Bcast(pvminfo, SIZEHINFO, MPI_INT, pvmhostnode, MPI_COMM_WORLD); */
#endif

	return 0;
}

int
pvm_mpp_message_stop() 
{
#if defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2)
	MPI_Finalize();
#endif
	return 0;
}
	
#endif /* IMA_NODE */
/* ===============  Host and Relay Processes for MPI =============== */
#if ( defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2) ) && defined(IMA_NODE)

#define NMPPSBUFMIDS 32			/* number of allowed outstanding send mids */
static msgmid_t mppsendmids[NMPPSBUFMIDS];
static CHUNK_PTR *mppoutchunks[NMPPSBUFMIDS];
static int lastindex = 0;

/* ------------ relay -------------- */
/* Relay messages between pvmd and node tasks. */
void relay(dsock, numnodes)
	int dsock;					/* pvmd socket */
	int numnodes;				/* number of nodes in the partition */
{
#ifdef FDSETNOTSTRUCT
	fd_set wrk_rfds, wrk_wfds, rfds, wfds;
#else
	struct fd_set wrk_rfds, wrk_wfds, rfds, wfds;
#endif
	int nfds;
	struct timeval tout;

	struct frag *topvmdq = 0;	/* frags to pvmd */
	struct frag *frtask = 0;	/* (big) frag from task */
	struct frag *totaskq;		/* frag being sent to task */
	struct frag *fp;
	struct frag *frpvmd = (struct frag *) NULL;
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
	int err,i;
	MPI_Status sta;				/* info on pending message */
	int dummy;
	int flag;					/* MPI_Test result */
	struct frag *hdr;
	int taskbuf = 0;			/* current buffer number to probe */ 




	MPP_DIRECTI_PTR taskdirect = (MPP_DIRECTI_PTR) NULL; /* ordering structs */

	MSG_INFO_PTR taskfrags = (MSG_INFO_PTR) NULL;

	CHUNK_PTR readyFrags = (CHUNK_PTR) NULL;

	MSGFUNC_PTR mfunc;
	
	struct frag *cftd = (struct frag *) NULL; /* cur.frag going to daemon */

	struct frag *cftt = (struct frag *) NULL; /* cur. frag to task */

	int cftd_togo;			/* len left to go to daemon */

	int frg_complete;		/* flag  if outgoing frag is complete */


/* This is the relay process for MPI ports. 
 *
 * It should be called in pvm_mpp_beatask 
 * after pvm_mpp_message_init, so none of the pre-allocated buffers have
 * been allocated.
 *
 * It is basically a fragment forwarder
 * pvmd -> task
   - each fragment is read from the pvmd socket and forwarded to the
     tasks.
   task -> pvmd
   - each fragment is read from a task and forwarded onto the pvmd socket.

*/

	/* Initialization */

	mfunc = pvm_hostmsgfunc();

	nfds = dsock + 1;

	/* set up pre-allocated receive buffers to/from tasks */

	taskdirect = new_vdirectstruct(numnodes, NSBUFS, NRBUFS);

	taskfrags = init_recv_list(NSBUFS, PMTDBASE, MAXFRAGSIZE, 0, MPPANY,
					pvm_hostmsgfunc());

	pvmlogprintf("relay() numnodes is %d\n", numnodes);
 	/* initialize the packet numbering for packets to/from peers */	
	for (i = 0; i < numnodes; i ++)
	{
		fill_directstruct (taskdirect + i, NRBUFS, i,
			0, PMTDBASE, 0, MPPANY);
		init_chunkostruct( (taskdirect+i)->ordering, NSBUFS);
	}

	/* initialize the fragment queues */

 	if( !(topvmdq = TALLOC(1, struct frag, "topvmdq")) 
		|| !(totaskq = TALLOC(1, struct frag, "totaskq")) )
	{
		pvmlogerror("relay() could not init frag queues\n");	
		pvm_mpp_message_stop();
		exit(PvmOutOfRes);
	}	
	BZERO((char *) topvmdq, sizeof (struct frag));
	BZERO((char *) totaskq, sizeof (struct frag));

	topvmdq->fr_link = topvmdq->fr_rlink = topvmdq;
	totaskq->fr_link = totaskq->fr_rlink = totaskq;


	/* initialize the asynchronous send structures */	

	pvm_init_asynch_list(mppsendmids, mppoutchunks, NMPPSBUFMIDS);

	FD_ZERO(&wrk_rfds);		/* zero the file set descriptor for select */ 
	FD_ZERO(&wrk_wfds);

	tout.tv_sec = tout.tv_usec = 0; /* don't block in select */
	while (1) 
	{			
		/* Step 1) get any frags from tasks */

		while ( fp = (struct frag *) pvm_chunkReady(taskfrags, NRBUFS,
				mfunc, taskdirect, numnodes, &taskbuf, &readyFrags) ) 	
		{
			LISTPUTBEFORE(topvmdq, fp, fr_link, fr_rlink);
			if (pvmdebmask & PDMNODE)
			pvmlogprintf("Frag from task %d len %d \n", fp->fr_src, fp->fr_len);
		}
			

		/* If we don't have a frag to go to pvmd, check if one has been read */
		if (!cftd && topvmdq->fr_link != topvmdq )
		{
			cftd = topvmdq->fr_link;
			LISTDELETE(cftd, fr_link, fr_rlink);
			cftd_togo = cftd->fr_len;
			txcp = cftd->fr_dat;
			if (pvmdebmask & PDMNODE)
			pvmlogprintf("Frag to daemoen %d len %d \n", cftd->fr_src, cftd->fr_len);

		}

		/* There is a frag that needs to go to the daemon ? */
		if (cftd)		
		{
			FD_SET(dsock, &wrk_wfds);
			if (pvmdebmask & PDMNODE)
			pvmlogprintf("Setting dsock in the write fds \n");
		}

		FD_SET(dsock, &wrk_rfds); /* need to probe the socket for reading */

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
			pvm_mpp_message_stop();
			exit(-1);
		}

		/* we've got something read  from pvmd, if the frag is 
			complete, then put it on the totaskq  */
		if (FD_ISSET(dsock, &rfds)) 
		{
			frg_complete = 1; /* initially mark the frag as completely read */

			if (!frpvmd) {			/* allocate a data frag */
				frpvmd = fr_new(MAXFRAGSIZE);
				toread = TDFRAGHDR;
			}

			n = read(dsock, frpvmd->fr_dat + frpvmd->fr_len, toread);

			if (n == -1 && errno != EWOULDBLOCK && errno != EINTR) {
				pvmlogperror("relay() read pvmd sock");
				pvm_mpp_message_stop();
				exit(12);
			}
			if (!n) {
				/* pvmlogerror("relay() read EOF on pvmd sock\n"); */
				pvm_mpp_message_stop();
				exit(13);
			}

			if ((frpvmd->fr_len += n) < TDFRAGHDR) {
				toread -= n;
			 	frg_complete = 0;	
			}

			
			n = TDFRAGHDR + pvmget32(frpvmd->fr_dat + 8);   /* get the length */

			if (frpvmd->fr_len < n) {
				if (frpvmd->fr_max < n) {					/* n > MAXFRAGSIZ */
					hdr = frpvmd;
					frpvmd = fr_new(n);
					BCOPY(hdr->fr_dat, frpvmd->fr_dat, hdr->fr_len);
					frpvmd->fr_len = hdr->fr_len;
					fr_unref(hdr);
				}
				toread = n - frpvmd->fr_len;
				frg_complete = 0;
			}

			if (pvmdebmask & PDMNODE)
			pvmlogprintf("reading from daemon len %d frg_complete %d \n", frpvmd->fr_len, frg_complete);
			if (frg_complete)
			{	
				dst = pvmget32(frpvmd->fr_dat);

				frpvmd -> fr_src = dst; /* overload the src field with the dst */
		
				LISTPUTAFTER(totaskq, frpvmd, fr_link, fr_rlink); /* queue it */

			if (pvmdebmask & PDMNODE)
			pvmlogprintf("frg_complete dest %x len %d\n", frpvmd->fr_src, frpvmd->fr_len);
				frpvmd = (struct frag *) NULL;
			}

		}


		if (!cftt && totaskq -> fr_link != totaskq )
		{
			cftt = totaskq->fr_link;
			LISTDELETE(cftt, fr_link, fr_rlink);
			if (pvmdebmask & PDMNODE)
			pvmlogprintf("New frag to task %x  len %d \n",
				cftt->fr_src, cftt->fr_len);
		}


		/* Let's see if we something to send to a task  */
		if (cftt)
		{
			if (send_to_node(cftt,  taskdirect, numnodes) >= cftt->fr_len)
				cftt = (struct frag *) NULL;
			else
			/* if for some reason send_to_node could not send this frag,
			   the next time through the loop, the frag will be resent.
	        */;
		}

		/* Now let's see if we have something to write to the pvmd */

		if ( cftd && FD_ISSET(dsock, &wfds))
		{
			if (pvmdebmask & PDMNODE)
			pvmlogprintf("Writing packet to daemon %d\n", cftd->fr_len);
			n = write(dsock, txcp, cftd_togo);

			if (n == -1 && errno != EWOULDBLOCK && errno != EINTR) {
				pvmlogperror("relay() write pvmd sock");
				pvm_mpp_message_stop();
				exit(14);
			}
			if (n > 0 && (cftd_togo -= n) > 0) 
				txcp += n;
			if (!cftd_togo) {		/* entire message sent */
				FD_CLR(dsock, &wrk_wfds);
				fr_unref(cftd);
				cftd = (struct frag *) NULL;
			}
		}
		else
		{
			if (cftd)
			if (pvmdebmask & PDMNODE)
				pvmlogprintf("could not write to daemon \n");
		}
	}
}

/* --------- send_to_node() ------------ */
/* return the number of bytes sent. This posts sends asynchronously.
   A fixed number of asynch sends are allowed to be outstanding.
   If the relay asks for more than this number in sends. send_to_node
   will return 0.
*/

send_to_node(fp,  taskdirect, numnodes )
struct frag *fp; 
MPP_DIRECTI_PTR taskdirect;
int numnodes;
{
	int appid = 0;
	int cc;
	int idx;
	int len;
	int node;
	int ptype;
	int tag;
    msgmid_t mid;


	MPP_DIRECTI_PTR tdirect;

	MSGFUNC_PTR mfunc;

	mfunc = pvm_hostmsgfunc();

	if ( (idx = pvm_mpp_find_midx(mppsendmids, mppoutchunks, &lastindex, 
				NMPPSBUFMIDS, mfunc)) >= 0)
	{
		node = fp->fr_src & TIDNODE;
		len = fp->fr_len;
		ptype = (fp->fr_src & TIDPTYPE) >> (ffs(TIDPTYPE) - 1);

		tdirect = taskdirect + node;

		tag = tdirect->tagbase + tdirect->sseq;

		if ( ++(tdirect->sseq) >= tdirect->nbufs)
			tdirect->sseq = 0;

			
        	if ( (cc  = (*mfunc->imsgsend)(appid, tag,
				fp->fr_dat, len, node, ptype, &mid)) < 0 )
 		{
			pvmlogprintf("Relay -- async send failed!  (%d) \n", cc);
		}
		else /* send was ok. */
		{
			pvm_assign_mid(mppsendmids, mid, idx);
			pvm_assign_chunk(mppoutchunks, (CHUNK_PTR) fp, idx);
		}

		return (len);

	}
	else
		return 0;	/* could not send, no free low-level message ids */
}
	

/* --------- pvmhost() ------------ */
/* We're the "host" process. Connect to pvmd. */
int 
pvmhost()
{
	char *p;
	int dsock;					/* pvmd socket */
	struct sockaddr_in dsadr;	/* address of pvmd socket */
	int n;
	int i;
	int cc;
	int err;
	
	int pvminfo[SIZEHINFO];		/* ntask, hostpart, ptid, MTU, NDF */
	char nullmsg[TDFRAGHDR+MSGHDRLEN];
	int appid = 0;				/* Would be used for PGONPUMA */

	MSGFUNC_PTR mfunc;

	msgmid_t confmid;

	info_t confinfo;

	if (!(p = getenv("PVMSOCK"))) {
		pvmlogerror("pvmhost() getenv() pvmd socket\n");
		pvm_mpp_message_stop();
		exit(2);
	}
	if ((dsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		pvmlogperror("pvmhost() socket");
		pvm_mpp_message_stop();
		exit(3);
	}
	BZERO((char*)&dsadr, sizeof(dsadr));
	hex_inadport(p, &dsadr);
	dsadr.sin_family = AF_INET;
	n = sizeof(dsadr);
	while (connect(dsock, (struct sockaddr*)&dsadr, n) == -1)
		if (errno != EINTR) {
			pvmlogperror("pvmhost() connect");
			pvm_mpp_message_stop();
			exit(4);
		}
#ifndef NOSOCKOPT
	n = 1;
	if (setsockopt(dsock, IPPROTO_TCP, TCP_NODELAY, (char*)&n, sizeof(int))
	== -1) {
		pvmlogperror("pvmhost() setsockopt");
		pvm_mpp_message_stop();
		exit(5);
	}
#endif
	if (!(p = getenv("PVMEPID"))) {
		pvmlogerror("pvmhost() getenv() pid\n");
		pvm_mpp_message_stop();
		exit(6);
	}
	pvmmyupid = atoi(p);
	BZERO(nullmsg, MAXHDR);
	pvmput32(nullmsg, TIDPVMD);
	pvmput32(nullmsg + 4, pvmmyupid);
	pvmput32(nullmsg + 8, MSGHDRLEN);
	pvmput32(nullmsg + 12, 0);			/* to keep putrify happy */
	pvmput8(nullmsg + 12, FFSOM|FFEOM);
	if (write(dsock, nullmsg, TDFRAGHDR+MSGHDRLEN) != TDFRAGHDR+MSGHDRLEN
	|| read(dsock, pvminfo, SIZEHINFO*sizeof(int)) != SIZEHINFO*sizeof(int)) {
		pvmlogperror("pvmhost() write/read");
		pvm_mpp_message_stop();
		exit(8);
	}
	/* Multicast the configuration message */  
	mfunc = pvm_hostmsgfunc();
	pvmlogprintf("pvmhost() --  Starting configMessage send Loop %d\n",
		pvmhostnode);
	for (i = 0; i < pvmhostnode; i ++ )
	{
		if ( (*mfunc->imsgsend)(appid, PMTCONF,  (char *) pvminfo, 
				sizeof(pvminfo), i, PVMDPTYPE, &confmid) < 0) 
		{
			pvmlogperror("pvmhost() configuration message \n");
			err = PvmDSysErr;
			pvm_mpp_message_stop();
			exit(9);
		}
		while (! ((*mfunc->msgdone)(appid, &confmid, &confinfo)));
	}
	pvmlogerror("pvmhost() -- Finished Message send Loop\n");
	(void)relay(dsock, pvmhostnode + 1);
}

#endif /*IMA_SP2MPI*/

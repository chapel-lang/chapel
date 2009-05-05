
static char rcsid[] =
	"$Id: lpvmmpp.c,v 1.12 2005/08/22 15:13:18 pvmsrc Exp $";

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
 *	lpvmmpp.c
 *
 *	support routines for the MPP environment
 *
 */

#include <stdio.h>
#include <rpc/types.h>
#include <rpc/xdr.h>
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <errno.h>

#include <pvm3.h>
#include <pvmtev.h>
#include "global.h"
#include "pvmalloc.h"
#include "pvmfrag.h"
#include "listmac.h"
#include "bfunc.h"
#include <pvmproto.h>
#include "tevmac.h"
#include "mppmsg.h"
#include "mppchunk.h"
#include "lmsg.h"
#include "pmsg.h"
#include "pvmmimd.h"
#include "lpvm.h"
#ifndef max
#define max(a,b)	((a)>(b)?(a):(b))
#endif
/***************
 **  Globals  **
 **           **
 ***************/

/* Indicate that we are able to receive directly into a users buffer */

extern struct msgid *pvm_inprecv;		/* from lpvm.c */
int pvmhostnode;

/***************
 **  Private  **
 **           **
 ***************/

static int pvmmynode;
static int pvmpartsize;
static int pvmmyndf;

static int pvmtidnmask = TIDNODE;
static int pvmtidpmask = TIDPTYPE;
static int pvmtidhmask = TIDHOST;
static int pvmmyptype;

static struct frag * pvm_frReady __ProtoGlarp__((
MSG_INFO_PTR, int, MSGFUNC_PTR, MPP_DIRECTI_PTR, int, int *, struct frag ** ));

static MPP_DIRECTI_PTR find_direct __ProtoGlarp__((MPP_DIRECTI_PTR, int, int ));
/* header of posted precvs */

static struct msgid *precvIds = (struct msgid *) NULL; 

static MSG_INFO_PTR nodefrags = (MSG_INFO_PTR) NULL; /* pre-posted recv bufs */
static MSG_INFO_PTR pvmdfrags = (MSG_INFO_PTR) NULL; /* pre-posted recv bufs */
static MPP_DIRECTI_PTR pvmddirect = (MPP_DIRECTI_PTR) NULL;
static MPP_DIRECTI_PTR peerdirect = (MPP_DIRECTI_PTR) NULL;

static struct ttpcb *peerpcbs = (struct ttpcb *) NULL;
static struct ttpcb *pvmdpcb = (struct ttpcb *) NULL;
static struct pmsg *rxpmsgs = (struct pmsg *) NULL;
static int inplaceDelay = 0;

struct pmsg *  midtobuf();
char * getenv();

/**************************
 **  Internal Functions  **
 **                      **
 **************************/


/* ------------ pvm_mppbeatask --------- */
/*	
*
*	Initialize libpvm, config process as a task.
*	This is called as the first step of each libpvm function so no
*	explicit initialization is required.
*
*	Returns 0 if okay, else error code.
*/

int
pvm_mpp_beatask(mytid, myptid, 
				outtid, outctx, outtag,
				trctid, trcctx, trctag,
				udpmtu, schedtid, topvmd)
int *mytid, *myptid; 
int *outtid, *outctx, *outtag;
int *trctid, *trcctx, *trctag;
int *udpmtu, *schedtid;
struct ttpcb **topvmd;
{
	char errtxt[64];
	char *p;
	char *s;

	int ac = 0;
	int cc;
	int hostid = 0;
	int i;
	int myhost;
	int mynode, mysetpart;
	int partsize, partid;
	int pvminfo[SIZEHINFO];		/* proto, hostpart, ptid, MTU, NDF */

	msgmid_t rmid;				/* msg ID returned by imsgrecv() */

	info_t minfo[MPPINFOSIZE];	/* info that might be returned by msgdone */
	
	MSGFUNC_PTR mfunc;

	if (pvmmytid != -1)			/* already configured */
		return 0;

	mfunc = pvm_hostmsgfunc();

	pvm_mpp_message_init(&mynode, &partsize, &pvmhostnode, &pvmmyptype);

	myhost = pvmhostnode;
	
	if (mynode < 0 || partsize < 0 || myhost < 0)
	{
		sprintf(errtxt, "mppbeatask(): bad init, node %d, part %d, host %d\n",
				mynode, partsize, myhost);
		pvmlogerror(errtxt);
		return PvmSysErr;
	} 	

	if (pvmdebmask & PDMPACKET) 
	{
		sprintf(errtxt," %d Posting receive for config message \n", mynode);
		pvmlogerror(errtxt);
	}

	/* Post receive for the configuration message */
	if ((*mfunc->imsgrecv)(hostid, MPPANY, PMTCONF, (char *) pvminfo,
			 sizeof(pvminfo), MPPANY, (int *) NULL, &rmid) < 0) 
	{
		pvmlogperror("beatask() recv pvminfo");
		return PvmSysErr;
	}

	if (pvmdebmask & PDMPACKET) {
		sprintf(errtxt," %d Receive posted for config message \n", mynode);
		pvmlogerror(errtxt);
	}

	if ((pvm_useruid = getuid()) == -1) {
		pvmlogerror("can't getuid()\n");
		return PvmSysErr;
	}

	pvmchkuid( pvm_useruid );

	pvmmyupid = getpid();

	/*
	*	initialize received-message list 
	*/

	rxpmsgs = TALLOC(1, struct pmsg, "pmsgs");
	BZERO((char*)rxpmsgs, sizeof(struct pmsg));
	rxpmsgs->m_link = rxpmsgs->m_rlink = rxpmsgs;

	if (pvmdebmask & PDMPACKET) {
		sprintf(errtxt," %d Waiting  for config message \n", mynode);
		pvmlogerror(errtxt);
	}
	while (!((*mfunc->msgdone)(0, &rmid, minfo)));

	if (pvmdebmask & PDMPACKET) {
		sprintf(errtxt," %d Got config message \n", mynode);
		pvmlogerror(errtxt);
	}

	if (pvminfo[0] != TDPROTOCOL) {
		sprintf(errtxt, "beatask() t-d protocol mismatch (%d/%d)\n",
			TDPROTOCOL, pvminfo[0]);
		pvmlogerror(errtxt);
		return PvmSysErr;
	}

	/* We now have the configuration message -- set up various parameters
		based on the message */

	pvmmynode = mynode;
	mysetpart = pvminfo[1];
	*myptid = pvminfo[2];
	*udpmtu = pvminfo[3];
	pvmmyndf = pvminfo[4];
	pvmpartsize = pvminfo[5];
	*outtid = pvminfo[6];
	*outtag = pvminfo[7];
	*outctx = pvminfo[8];
	*trctid = pvminfo[9];
	*trctag = pvminfo[10];
	*trcctx = pvminfo[11];
	*mytid = mysetpart + pvmmynode;
	pvmfrgsiz = pvmudpmtu;

	/* ---- set up the pre-allocated receive buffers ---- */

	pvmddirect = new_directstruct( NSBUFS, NRBUFS );
	peerdirect = new_vdirectstruct( pvmpartsize, NSBUFS, NRBUFS);

	pvmdfrags = init_recv_list(NSBUFS, PMTDBASE, MAXFRAGSIZE, 0, MPPANY, 
				pvm_hostmsgfunc());
	nodefrags = init_recv_list(NSBUFS, PMTPBASE, MAXFRAGSIZE, 0, MPPANY,
				pvm_nodemsgfunc()); 

	/* intialize the packet numbering for packets from daemon */
	fill_directstruct (pvmddirect, NRBUFS, pvm_tidtohost(pvmmytid), 
		0, PMTDBASE, 0, MPPANY);
	init_chunkostruct( pvmddirect->ordering, NSBUFS);

	/* intialize the packet numbering for packets from peers */
	for (i = 0; i < pvmpartsize; i ++)
	{
		fill_directstruct (peerdirect + i, NRBUFS, i, 
			0, PMTPBASE, 0, MPPANY);
		init_chunkostruct( (peerdirect+i)->ordering, NSBUFS);
	}

	/* ---- Create Task, pvmd  PCBs so that they can be found easily ----- */

	peerpcbs = TALLOC(pvmpartsize + 1, struct ttpcb, "pcbs");
	for (i = 0; i <= pvmpartsize; i ++)
	{
		BZERO((char *)(peerpcbs + i), sizeof(struct ttpcb));
		(peerpcbs + i) -> tt_tid = i;
		(peerpcbs + i) -> tt_state = TTOPEN;
		(peerpcbs + i) -> mpdirect = peerdirect + i;
	} 

	pvmdpcb = peerpcbs + pvmpartsize;	
	pvmdpcb -> mpdirect = pvmddirect;

	*topvmd = pvmdpcb;
	
	precvIds = msgid_new();
	precvIds->ms_link = precvIds->ms_rlink = precvIds;

	if (s = getenv("PVMINPLACEDELAY"))
	{
		inplaceDelay = atoi(s);
		pvmlogprintf("setting in place delay to %d \n", inplaceDelay);
	}
	else
#if !defined(IMA_PGON)
		inplaceDelay = 0;
#else
		inplaceDelay = 250;				/* uSec. Seems to work well */
#endif

	pvm_setopt(PvmRoute, PvmDontRoute); /* Deny direct routing */
	return 0;


	
}

/* ------ pvm_node_send ------- */
/* this routine sends a single array of data (a pvm fragment) to a process.
 * The semantics are such that the send is asynchronous with the send 
 * message id added to the sendmsg list.
 *
 * inputs:
 * 	cp 		- array of data to be sent
 *  len 	- length  of data
 *  ttpcbp	- pointer to the task control structure
 *  sendmsg - list of sendmsgs to add this one to (or merge with for PGONs)
 *	inPlaceHeader - if this is an inplace header, this is == cp
 *	inPlaceBodyLen - length of the inplace body that will follow this header
 *
 *	outputs:
 * 	  returns #bytes queued for sending 
 *	  sendmsg is initialized if *sendmsg == NULL, otherwise the message is 
 *				added to the list of outstanding send messages	
*/
int
pvm_node_send(cp, len, ttpcbp, smsglist, inPlaceHeader, inPlaceBodyLen)
	char *cp;				/* this is what we are supposed to be sending */
	int len;				/* this is the length of the frag */
	struct ttpcb * ttpcbp;	/* info about where this is going */
	struct msgid **smsglist; /* msgid for this send */
	char *inPlaceHeader;	/* This is an inplace header? */
	int inPlaceBodyLen;		/* Length of inplace fragment */
{
	int cc;
	int dtid;
	int mask;						/* used to mask different parts of tid */
	msgmid_t mid;
	int node;						/* destination node */
	int partid;						/* destination partition id */
	int tag;


	MSGFUNC_PTR	mfunc;	

#if defined(IMA_PGON)
	double dclock();
#endif
	static double lastClock;
	double delay;

	static int savelen = -1;/* save the length of inplace body for next call */
	
	MPP_DIRECTI_PTR tdirect;

	struct msgid *cmsgid = (struct msgid *) NULL;

	mask = TIDHOST;

	dtid = ttpcbp -> tt_tid;

	if (TIDISNODE(dtid) && (dtid & pvmtidhmask) == (pvmmytid & pvmtidhmask)
		&& (dtid & pvmtidpmask) == (pvmmytid & pvmtidpmask)) 
	{
		node = dtid & pvmtidnmask;
		partid = pvmmyptype;			/* send to node directly */
		mfunc = pvm_nodemsgfunc();		/* point to node-node routines */
	} 
	else 
	{
		node = pvmhostnode;
		partid = PVMDPTYPE;			/* send to pvmd first */
		mfunc = pvm_hostmsgfunc();		/* point to host-node routines */
	}

	
	tdirect = ttpcbp->mpdirect;

	if (inPlaceHeader)
	{
		savelen = inPlaceBodyLen;
#if defined(IMA_PGON)
		if (savelen > 4096 )
		{
			lastClock = dclock();	
			delay = (double) inplaceDelay / 1e6;
		}
#endif
	}

	if ( savelen >= 0  && inPlaceHeader == (char *) NULL) 
	{
		tag = pvmmynode;		/* send inplace body with my physical node # */
#if defined(IMA_PGON)
		if (savelen > 4096 )
			while ((dclock() - lastClock) < delay); 
#endif
		savelen = -1;		  	/* as the tag */
	}
	else
	{
		tag = tdirect->tagbase + tdirect->sseq;

		if (++(tdirect->sseq) >= tdirect->nbufs) 
				tdirect->sseq = 0;
	}

	{
		if ((cc = (*mfunc->imsgsend)(0,tag, cp, len, node, partid, &mid))
				&& (cc < 0)) 
		{
			pvmlogperror("node_send() IMSGSEND \n ");
			return PvmSysErr;
		}

		/* can't merge id's  add to the list */
		if (!(mfunc->msgmerge)  || !(*smsglist)) 
		{
			if ( (cmsgid = msgid_new()) == (struct msgid *) NULL)
			{
				pvmlogerror("node_send(): couldn't allocate send msgid\n");
				return PvmSysErr;
			}
			if (! (*smsglist))  	/* intialize the list */
			{
				*smsglist = cmsgid;
				cmsgid->ms_link = cmsgid->ms_rlink = cmsgid;
			}
			else				/* put at the end of the list of message id's */
				LISTPUTBEFORE(*smsglist, cmsgid, ms_link, ms_rlink);
		}
		else
		{
			cmsgid = *smsglist;
			cmsgid -> id = (*mfunc->msgmerge)(&(cmsgid->id), &mid);
		}

		if (cmsgid -> otid != node)
		{
			cmsgid -> id = mid;
			cmsgid -> otid = node;
			cmsgid -> complete = 0;
			cmsgid -> len += len;
			cmsgid -> mfunc = mfunc;
		}
	
	}
	return len;
}


/* ---------- pvm_node_mcast --------- */
/* This multicasts to tasks - it works takes an array of tids
 * and sends directly to tasks that are on the local partition using
 * pvm_send.
 * It returns a list of off-host tids and the number of off-host
 * tids. The off-host tids should then be sent to using pvm_mcast()
 * semantics.
*/
int
pvm_node_mcast(tids, count, code, offhtids, offhcnt)
int *tids; 	/* array of tids, on and off host */
int count;	/* #of tids */
int code;	/* msgtag tag */
int **offhtids; /* array of tids going off host, returns null if none */
int *offhcnt; /* # of tids going off host, returns 0 if none */
{

	int i;
	int mask = pvmtidhmask;		/* host */
	int nlocal = 0;
	int localidx = count;
	int nremote = 0;
	int *tmptids = (int *) NULL;
	*offhtids = (int *) NULL;
	*offhcnt = 0;	

	if (!tids || count < 0) 		/* no tids, or bogus count */
		return PvmOk;

	if (! (tmptids = TALLOC(count, int, "nmcast")))
	{
		pvmlogerror("node_mcast(): couldn't alloc memory\n");
		return PvmNoMem;
	}

	for (i = 0; i < count; i++)
	{
		if ( TIDISNODE(tids[i]) && (tids[i] & mask) == (pvmmytid & mask))
		{
			/* put the local tids starting at the back of the array */
			tmptids[--localidx] = tids[i];	/* tid local to us */
			nlocal ++;
		}
		else
		{
			/* put non-local tids at the front of the array */
			tmptids[nremote++] = tids[i];
		}

	}

	/* multicast to the local tasks */
	for (i = 0; i < nlocal; i++)
		pvm_send(tmptids[localidx++], code);      /* loop of sends for now */

	if (nlocal == count)
	{
		PVM_FREE(tmptids);
		tmptids = (int *) NULL;
	}
	else
	{
		*offhtids = tmptids;
		*offhcnt = nremote;	
	}
	
	return PvmOk;	
}


	
	


int
mpp_pvmendtask()
{

	if (pvmmytid != -1) 
	{
		pvmmytid = -1;

		pvm_mpp_message_stop();
	}
	
	return 0;
}


/* mpp-specific stuff on precv */
int
pvm_mppprecv(tid, tag, cp ,len, dt, rtid, rtag, rlen)
	int tid;
	int tag;
	void *cp;
	int len;
	int dt;
	int *rtid;
	int *rtag;
	int *rlen;
{
	char errtxt[64];

	int rbf;
	int cc = 0;
	int l;
	int x;
	long ad;

	static int first = 1;
	static struct msgid * thisprecv;

	/* allocate a new message id for this precv */

	if (first)
	{
		if ( !(thisprecv = msgid_new()))
		{
			pvmlogerror("mppprecv(): can't get memory \n");
			return PvmNoMem;
		}
		first = 0;
	}
 	

	thisprecv -> otid =  tid;
	thisprecv -> tag = tag;
	thisprecv -> ctxt = pvm_getcontext();
	thisprecv -> ubuf = cp;
	thisprecv -> len = len; 
	thisprecv -> complete = 0;

	pvm_inprecv = thisprecv;
	
	if (!cc) {
	
		cc = pvm_recv(tid, tag);

		if (cc >= 0) {

			if (!(thisprecv->complete)) /* receive went into pvm buffers */
			{		
				pvm_bufinfo(cc, &l, rtag, rtid);
				if 	(rlen)
					*rlen = l;
				if (l < len)
					len = l;
				if (pvmdebmask & PDMMESSAGE) {
					sprintf(errtxt, "precv() unpacking message len = %d\n", l);
					pvmlogerror(errtxt);
				}
				pvm_upkbyte((char *)cp, len, 1);
			}
			else /* receive went straight into user memory */
			{
				if(rlen)
					*rlen = thisprecv -> len;
				if (pvmdebmask & PDMMESSAGE) {
					sprintf(errtxt, "precv() short ckted into user buf len = %d\n", l);
					pvmlogerror(errtxt);
				}
			}
			cc = 0;
		}
	}

	pvm_inprecv = (struct msgid *) NULL;

	if (cc < 0)
		lpvmerr("pvm_mppprecv", cc);
	return cc;
}

/* ---------- pvm_mpppsend ------------- */
/* mpp_psend does short cuts on buffer management, 
	o This is a reasonable hack that greatly decreases (overhead)
	  latency.
	o It smacks the message structure directly. 
*/
int 
pvm_mpppsend(cp, len, tid, tag)
char *cp;
int len;
int tid;
int tag;
{

	static char nullusrmsg;
	static int first = 1;
	static int psbuf;
	static struct frag *dfrag;
	static struct pmsg *psmsg;
	static struct timeval ztv = { 0, 0 };
	int savebuf;
	int cc;

	/* First call to psend will create a regular pvm buffer for packing.
	 *	o	A new message is created as a by-product of pvm_mkbuf.
	 * 	o	A single (inplace) data buffer is created and put in the frag chain 
	 *	o	Later calls will simply twiddle the message buffer and the frag
	 */
	if (first)
	{
		psbuf = pvm_mkbuf(PvmDataInPlace); /* Make an inplace data buffer */
	    savebuf = pvm_setsbuf(psbuf);
		pvm_pkint(&cc, 1, 1);			/* create a single frag for msg */ 
		pvm_setsbuf(savebuf);
		psmsg = midtobuf(psbuf);		/* save a pointer to the msg struct */
		dfrag = psmsg->m_frag->fr_link;
		first = 0;
	}

	psmsg->m_ctx = pvmmyctx;			/* set the context */

	if (len)
	{
		dfrag->fr_buf = dfrag->fr_dat = cp;
		dfrag->fr_max = dfrag->fr_len = len;
	}
	else
	{
		dfrag->fr_buf = dfrag->fr_dat = &nullusrmsg;
		dfrag->fr_max = dfrag->fr_len = len;
	}

	cc = mroute(psbuf, tid, tag, &ztv); /* route the message */ 
	
	return cc;
}

	

	



/* ----------- pvm_readfrompvmd ----------- */
struct frag *
pvm_readfrompvmd()
{
	static int hostbuf = 0; 				/* buffer we are working on */

	static CHUNK_PTR readyFrags = (CHUNK_PTR) NULL;

	int mxbufs = NRBUFS;

	MSGFUNC_PTR mfunc;

	mfunc = pvm_hostmsgfunc();

	/* check to see if any fragments are ready to be processed */

	return  (struct frag *) pvm_chunkReady( pvmdfrags, mxbufs, mfunc, 
		pvmddirect, 1, &hostbuf, &readyFrags);   
}

/* ----------- pvm_readfrompeer ----------- */
struct frag *
pvm_readfrompeer()
{
	static int nodebuf = 0; 				/* buffer we are working on */

	static CHUNK_PTR readyFrags = (CHUNK_PTR) NULL;

	int mxbufs = NRBUFS;

	MSGFUNC_PTR mfunc;

	mfunc = pvm_nodemsgfunc();

	/* check to see if any fragments are ready to be processed */

	return  (struct frag *) pvm_chunkReady( nodefrags, mxbufs, mfunc, 
			peerdirect, pvmpartsize, &nodebuf, &readyFrags);   
}


/* ------- init_precvMsgs ----- */
struct msgid *
init_precvMsgs()
{
	return msgid_new();
}

/* ---------- mpp_ttpcb_find() ------- */
struct ttpcb *
mpp_ttpcb_find(dtid) 
int dtid;
{
	int i;

	if (TIDISNODE(dtid) && (dtid & pvmtidhmask) == (pvmmytid & pvmtidhmask)
		&& (dtid & pvmtidpmask) == (pvmmytid & pvmtidpmask)) 
	{
		i = dtid & pvmtidnmask;
		(peerpcbs + i)->tt_tid = dtid;
		return peerpcbs + i;
	}
	else
		return (struct ttpcb *) NULL;
}


/* ---------- pvm_mpp_pmsgs() ------- */
struct pmsg *
pvm_mpp_pmsgs()
{
	return rxpmsgs;
}

/* ----------- find_direct ---------- */
/* this is a hack to find the correct ordering structure for
	a node. 
*/
MPP_DIRECTI_PTR 
pvm_find_direct (dlist, nstruct, node)
MPP_DIRECTI_PTR dlist;
int nstruct;
int node;
{
	node = node & pvmtidnmask;  /* make sure this is a node */

	if (dlist == pvmddirect)
		return pvmddirect;
	else 
	{
		return dlist + node;
	}	
}
		 
struct msgid *
pvm_mpp_get_precvids()
{
    	return pvm_inprecv; 
}


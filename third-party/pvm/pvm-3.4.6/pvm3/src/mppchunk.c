
static char rcsid[] =
	"$Id: mppchunk.c,v 1.7 2005/08/22 15:13:19 pvmsrc Exp $";

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

/* File: mppchunk.c -- implement asynchronous, pre-posted receive 
*	buffers for mpp machines.  
*
* NOTE: This code is designed to handle pvm packets (struct pkt) and
* pvm frags (struct frag). The packet and frag structures are very similar.
* So similar, in fact, that a small number of macros can be used to define
* fwd,rev link ptrs, allocation routines, etc. In this code, these are
* generically called chunks.  Daemons use packets while user code use
* fragments. 
*
*
* This code implements virtual sequence numbers to maintain ordered
*	delivery of message packets to the rest of the messaging system.
*
*	for each connected task (O(1000) for PVM daemons) a packet sequence number
*	is maintained. Tasks send their packets in increasing
*	order to the pre-alloced buffers (with wrap when the buffer depth is
*	depleted).  The underlying messaging insures that packets sent in 
*	the same task on the same message tag will arrive at the receiver
*	in order.  However, difficulty arises when multiple tasks are 
*	sharing the buffers. A receive must be processed and then information 
*	about who sent the message is extracted. In this scenario, it is
*	likely that packets will be received out of order. An example follows
*	for a buffer depth of four and two tasks A, B. The sequence numbers indicate
*	the order in which the packets are sent. A has sent 4 packets, B has sent
*	7 packets 
*
*	BUF1: 	A(0)  B(0) B(4)   
*	BUF2:	B(1)  A(1) B(5)
*	BUF3:   A(2)  B(2) B(6)
*	BUF4:	A(3)  B(3)
*
*	If round robin message probing is used, then packets are received in the
*	following order:
*
*	A(0) B(1) A(2) A(3) B(0) A(1) B(2) B(3) B(4) B(5) B(6) 
*
*	In this case B(0) is received before B(1), A(2) and A(3) are both
*	received before A(1).  
*
*	By keeping track of what the current sequence number is for each task,
*	message delivery order can be maintained.
*/


/* NEEDED Support Routines:
 * 
 * struct msgid * pvm_mpp_get_precvids()
 *
 * 		gets a pointer to head of list for post precv(s).
 *		for daemons, this can return (struct msgid) NULL 
 *
 * MPP_DIRECTI_PTR pvm_find_direct(ostructs, nstructs, src)))
 * 		MPP_DIRECTI_PTR ostructs;
 *		int nstructs;
 *		int src;
 *
 *		ostructs - the start of an array of ordering structs. 
 *		nstructs - length of ostructs array
 *		src -	tid or node # that we are trying to find the ordering struct
*/



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <pvm3.h>
#include <pvmproto.h>
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
extern int errno;
extern int pvmdebmask;           /* from pvmd.c */

/* Global Variables that can be Changed */
int MAXSEQ = DFN_MAXSEQ;

/* Private variables */
static char mpperrtext[128];

/* Prototypes for externals */
MPP_DIRECTI_PTR pvm_find_direct __ProtoGlarp__((MPP_DIRECTI_PTR, int , int));
struct msgid * pvm_mpp_get_precvids __ProtoGlarp__( () ); 

/* ======== new_directstruct ======== */
/* Allocate and fill and a new MPP_DIRECTI struct */

MPP_DIRECTI_PTR 
new_directstruct( nrbufs, nsbufs )
int nrbufs;  /* number of receive buffers */
int nsbufs;	 /* number of send buffers */
{
	MPP_DIRECTI_PTR new;
	int i;
	
	if ( !( new = (MPP_DIRECTI_PTR) TALLOC(1, MPP_DIRECTI, "new_directstruct") )
			|| ! (new -> ordering = new_chunkostruct( nrbufs )))
	{
		pvmlogerror("new_directstruct: can't get memory\n");
		pvmbailout(PvmNoMem);	
		return  (MPP_DIRECTI_PTR) NULL;
	}

	init_directstruct( new,  nsbufs);
	
	return new;

}

/* ======== init_directstruct ======== */
int
init_directstruct( dstruct , nsbufs )
MPP_DIRECTI_PTR dstruct;
int nsbufs;
{
	/* Initialize the variables. Set unknown values to -1 */
	dstruct -> rtid = -1;
	dstruct -> rpid = -1;
	dstruct -> tagbase = -1;
	dstruct -> nbufs = nsbufs;
	dstruct -> sseq = -1;

	return PvmOk;
}
	

/* ======== fill_directstruct ======== */
int
fill_directstruct( dstruct , nsbufs, rtid, rpid, tagbase, seq , appid)
MPP_DIRECTI_PTR dstruct;
int nsbufs;
int rtid;
int rpid;
int tagbase;
int seq;
int appid;
{
	/* Initialize the variables. Set unknown values to -1 */

	dstruct -> rtid = rtid;
	dstruct -> rpid = rpid;
	dstruct -> tagbase = tagbase;
	dstruct -> nbufs = nsbufs;
	dstruct -> sseq = seq;
	dstruct -> appid = appid;

	return PvmOk;
}
	
/* ======== new_vdirectstruct ======== */
/* Allocate and fill n new MPP_DIRECTI structs, return an
	array of MPP_DIRECTI structs */

MPP_DIRECTI_PTR 
new_vdirectstruct( nstructs, nsbufs, nrbufs )
int nsbufs;
int nrbufs;
{
	MPP_DIRECTI_PTR vnew, current;
	CHUNK_ORDER_PTR ostructs;
	int i,j;
	
	if (  !( vnew = (MPP_DIRECTI_PTR) TALLOC(nstructs, MPP_DIRECTI, "new_vdirectstruct") ) 
		 	|| !( ostructs = new_vchunkostruct( nstructs, nrbufs ) ) )
	{
		pvmlogerror("new_vdirectstruct: can't get memory\n");
		pvmbailout(PvmNoMem);	
		return  (MPP_DIRECTI_PTR) NULL;
	}

	/* Initialize the variables. Set unknown values to -1 */

	for (j = 0; j < nstructs; j++)
	{
		current = vnew + j;

		init_directstruct( current, nsbufs);

		/* point to one of the ordering structs that we just malloc'ed */
		current -> ordering = ostructs + j;
	}


	return vnew;
}
/* ======== new_chunkostruct ======== */
/* Allocate and fill and a new CHUNK_ORDER struct */

CHUNK_ORDER_PTR 
new_chunkostruct( nbufs )
int nbufs;
{
	CHUNK_ORDER_PTR new;
	int i;
	
	if ( !( new = (CHUNK_ORDER_PTR) TALLOC(1, CHUNK_ORDER, "new_chunkostruct") ) 
		 	|| !( new->bufseq = (int *) TALLOC(nbufs, int, "new_chunkostruct") ) )
	{
		pvmlogerror("new_chunkostruct: can't get memory\n");
		pvmbailout(PvmNoMem);	
		return  (CHUNK_ORDER_PTR) NULL;
	}

	/* init the sequence #'s and buffer sequence #'s, nbufs */

	init_chunkostruct( new, nbufs );

	return new;
}


/* ======== new_vchunkostruct ======== */
/* Allocate and fill n new CHUNK_ORDER structs , do it in a couple of mallocs
	so that initializing a large number of structs is done efficiently */

CHUNK_ORDER_PTR 
new_vchunkostruct( nstructs, nbufs )
int nstructs;
int nbufs;
{
	CHUNK_ORDER_PTR vnew, current;
	int *bufs;
	int i,j;
	
	if (  !( vnew = (CHUNK_ORDER_PTR) TALLOC(nstructs, CHUNK_ORDER, "new_vchunkostruct") ) 
		 	|| !( bufs = (int *) TALLOC(nbufs * nstructs, int, "new_vchunkostruct") ) )
	{
		pvmlogerror("new_vchunkostruct: can't get memory\n");
		pvmbailout(PvmNoMem);	
		return  (CHUNK_ORDER_PTR) NULL;
	}

	/* init the sequence #'s and buffer sequence #'s, nbufs */

	for (j = 0; j < nstructs; j++)
	{
		current = vnew + j;
		current -> bufseq = bufs + ( nbufs * j ); 
		init_chunkostruct( current, nbufs );
	}

	return vnew;
}

/* ======== init_chunkostruct ======== */
int
init_chunkostruct( ostruct , nrbufs  )
CHUNK_ORDER_PTR ostruct;
int nrbufs;
{
	int i;

	/* Initialize the variables. Set unknown values to -1 */
	ostruct -> nbufs = nrbufs;
	ostruct -> seq = 0;
	for (i = 0; i < nrbufs; i ++)
		ostruct->bufseq[i] = i;	

	ostruct->oochunks = (CHUNK_PTR) NULL;	

	return PvmOk;
}

/* ======== order_chunk  ======== */
/* take a buffer number and pkt_order struct and a pkt, 
	place the pkt on the oopkts list, update sequence #'s.
*/
int
order_chunk( ostruct, ibuf, inchunk )
CHUNK_ORDER_PTR ostruct;
int ibuf;
CHUNK_PTR inchunk;  
{
	int *obseq;		/* order buffer sequence number */

	CHUNK_PTR firstchunk, tstchunk;	
	obseq = ostruct->bufseq + ibuf;

	inchunk->CHUNK_SEQ = *obseq; 


	/* store next packet sequence # to arrive in this buffer 
		from this sender*/
	*obseq = NEXTCHUNKSEQ(*obseq, ostruct->nbufs);

	if (ostruct -> oochunks == (CHUNK_PTR) NULL)
	{
		ostruct -> oochunks = inchunk;
		inchunk -> CHUNK_NEXT = inchunk -> CHUNK_PREV = inchunk;
	}
	else
	{
		firstchunk = tstchunk = ostruct->oochunks;
		while (SEQLESSTHAN(tstchunk->CHUNK_SEQ, inchunk -> CHUNK_SEQ)
				&& tstchunk -> CHUNK_NEXT != firstchunk)
			tstchunk = tstchunk -> CHUNK_NEXT;

		if (SEQLESSTHAN(tstchunk ->CHUNK_SEQ, inchunk->CHUNK_SEQ))
		{
			LISTPUTAFTER(tstchunk, inchunk, CHUNK_NEXT, CHUNK_PREV);
		}
		else
		{
			LISTPUTBEFORE(tstchunk, inchunk, CHUNK_NEXT, CHUNK_PREV);
			if (tstchunk == firstchunk) /* new first of the list ? */
				ostruct -> oochunks = inchunk;
		}
	}

	if (pvmdebmask & (PDMPACKET | PDMNODE))
	{
		sprintf(mpperrtext,"Ordering packet seq %d from tid %x\n", 
				inchunk -> CHUNK_SEQ, inchunk -> CHUNK_SRC); 
		pvmlogerror(mpperrtext);
	}
	return 0;
}	

/* ======== ochunk_delete  ======== */
/* delete a pkt from the oopkts struct if any. Adjust the
	seq number.
	returns NULL if no packet is ready.
*/
CHUNK_PTR
ochunk_delete( ostruct )
CHUNK_ORDER_PTR ostruct;
{
	CHUNK_PTR tstchunk;

	if ( !ostruct || ! (ostruct->oochunks))
		return (CHUNK_PTR) NULL;

	tstchunk = ostruct->oochunks;

	if (tstchunk -> CHUNK_SEQ == ostruct -> seq)  /* yep, we got one */
	{
		ostruct->seq = NEXTCHUNKSEQ(ostruct -> seq, 1);
		if (tstchunk -> CHUNK_NEXT == tstchunk)
			ostruct -> oochunks = (CHUNK_PTR) NULL;
		else
		{
			ostruct -> oochunks = tstchunk -> CHUNK_NEXT;
			LISTDELETE(tstchunk, CHUNK_NEXT, CHUNK_PREV);
		}

		if (pvmdebmask & (PDMPACKET | PDMNODE))
		{
			sprintf(mpperrtext,"ochunk_delete() packet seq %d from tid %x\n", 
					tstchunk -> CHUNK_SEQ, tstchunk -> CHUNK_SRC); 
			pvmlogerror(mpperrtext);
		}

		tstchunk -> CHUNK_NEXT = tstchunk -> CHUNK_PREV = (CHUNK_PTR) NULL;
		tstchunk -> CHUNK_SEQ = 0;
		
		return tstchunk;
	}	

	return (CHUNK_PTR) NULL;
}
/* ======== init_recv_list  ======== */
/* allocate a message receive array and post the actual receives */

MSG_INFO_PTR
init_recv_list( nbufs, tagbase, msize, hsize, appid, llmsg )
int nbufs, tagbase, msize, hsize;
int appid;
MSGFUNC_PTR llmsg;
{
	MSG_INFO_PTR new, tmp;
	CHUNK_PTR msgchunk;
	char * datap;
	int i;	
	int cc;
	int mrsize; /* message receive size */

	if (  !( new = (MSG_INFO_PTR) TALLOC(nbufs, MSG_INFO, "init_recv_list") ) ) 
	{
		pvmlogerror("init_recv_list: couldn't allocate memory \n");
		return (MSG_INFO_PTR) NULL;
	}

	tmp = new;
	
	for (i = 0; i < nbufs; i ++)
	{
		cc = post_receive(tmp, -1, tagbase + i, msize, hsize, MPPANY, appid,
			llmsg);
		tmp ++;
		if (cc < 0)
		{	
			sprintf(mpperrtext,"init_recv_list(): post_receive failed! src %d tag %d msize %d hsize %d pid %d appid %d : nbufs %d buffer %d \n", 
			-1, tagbase + i, msize, hsize, MPPANY, appid, nbufs, i);
		
			pvmlogerror(mpperrtext);
		}
	}

	return new;
}

/* ======= post_receive ======== */	
/* allocs a new packet and posts an asynchronous receive for the message */ 
int
post_receive(imsg, src, tag, mxsize, headroom, ptype, appid, llmsg)
MSG_INFO_PTR imsg;
int src, tag, mxsize, headroom;
int ptype;
int appid;
MSGFUNC_PTR llmsg;
{
	int cc;

	char *datap;
	char errtxt[128];

	CHUNK_PTR msgchunk;
	
 	if ( !(msgchunk = CHUNK_NEW(mxsize)) )
	{
		pvmlogerror("post_recv: couldn't allocate memory \n");
		return PvmNoMem;
	}	

	datap =  msgchunk->CHUNK_DAT + headroom; 

	imsg -> rchunk = msgchunk;		/* PVM packet */
	imsg -> src = src; 
	imsg -> tag = tag;	/* tag for this buffer */
	imsg -> mxsize = mxsize;	/* max msg size for this buffer */ 
	imsg -> hsize =  headroom; 	/* header size (data offset) */
	
	cc = (*llmsg->imsgrecv)(appid, src, tag, datap, mxsize - headroom, 
			ptype, imsg->info, &(imsg->mid));

	if ( cc < 0)
	{
		
		sprintf(errtxt, "post_recv: bad msg id (%d). Fatal! \n", cc);
		pvmlogerror(errtxt);
		return PvmSysErr;
	}	
}		

/* ======= post_send ======== */	
/* Given a packet,length, and a MPP_DIRECTI structure, send a message
to the recipient. Info used from the MPP_DIRECTI struct is
	rtid;
	tagbase;
	nbufs;
	sseq;  

Returns:
	a positive mid if the send was successful.
	a negative mid if the send was unsuccessful.

	sequence #s in the MPP_DIRECTI struct are only advanced if 
	the send was successful.
*/ 
msgmid_t
post_send(buffer, len, distruct, llmsg)
char *buffer;
int len;
MPP_DIRECTI_PTR distruct;
MSGFUNC_PTR llmsg;
{
	int appid;
	int cc;
	int dest;
	int ptype;
	int rtid;
	int tag; 

	msgmid_t mid;

	if (distruct == (MPP_DIRECTI_PTR) NULL)
		return (-1);

	rtid = distruct->rtid;

	tag = distruct -> sseq % distruct -> tagbase;

	dest = (distruct -> rtid) & TIDNODE;

	ptype = (distruct -> rpid); 
	
	appid = (distruct -> appid);

	/* Increment the send sequence number if the send is successful */
	if ( (cc =  (*llmsg->imsgsend)(appid, tag, buffer, len, dest, ptype, &mid)) >= 0 )
		distruct -> sseq = INCRSEQ( distruct->sseq, MAXSEQ); 

	return mid;
}	
	
/* ======= read_chunk ======== */	
/*  reads the chunk indicated by the msg_info structure and 
	then re-posts a receive to takes its place. Will not block
	if the message has completed, so can be used as a "probe with
	receive"

	if repost > 0; then repost a receive with the same parameters;
*/
CHUNK_PTR
read_chunk( imsg, src, tag, len , repost, appid, llmsg)
MSG_INFO_PTR imsg;
int *src;
int *tag;
int *len;
int repost;
int appid;
MSGFUNC_PTR llmsg;
{
	CHUNK_PTR rchunk= (CHUNK_PTR) NULL;

	if (imsg && imsg->rchunk && imsg->mid >= 0 
			&& (*llmsg->msgdone)(appid, &(imsg->mid), imsg->info))

	{
		*src = (*llmsg->msgsrc)(imsg->info);
		*len = (*llmsg->msglen)(imsg->info);
		*tag = (*llmsg->msgtag)(imsg->info);
		rchunk = imsg -> rchunk;

		if (pvmdebmask & PDMPACKET)
		{
			sprintf(mpperrtext,"read-chunk(): node %d, len %d, tag %d \n", 
					*src, *len, *tag);
			pvmlogerror(mpperrtext);
		}

		if (repost)
			post_receive(imsg, imsg->src, imsg->tag, imsg->mxsize, imsg->hsize, 					MPPANY, appid, llmsg);
	}
		
	return rchunk;		
}


/* -------- pvm_chunkReady() --------- */
/* probe to see if any chunks have arrived. Chunks are in pre-posted
 * buffers numbered 0, 1, ..., (nbufs-1). 
 * pvm_chunkReady() will return a chunk if it is completed.

 * chunkReady handles inplace reception of messages by examining the
 * header structure of the chunk. It allocates more memory if it needs
 *
 * Input:
 *	ppmsg - vector of pre-posted receive MSG_INFO pointers (length == nbufs)
 *  nbufs - length of ppmsg array
 *  mfunc - low level message passing primitive functions to use
 *  ostructs - ordering structures 
 *  nstructs - number of ordering structures
 *  *cbuf - current buffer to probe in the ppmsg array. Updated by pvm_frReady
 *  **hdReadyList - list of ready chunks, updated by pvm_chunkReady
 *
 * Return:
 *  rcp - pointer to ready chunk. NULL if none ready
*/
CHUNK_PTR
pvm_chunkReady(ppmsg, nbufs, mfunc, ostructs, nstructs, cbuf, hdReadyList )
MSG_INFO_PTR ppmsg;	/* vector of pre-posted message buffers */
int nbufs;				/* number of bufs in ppsmg */
MSGFUNC_PTR mfunc;		/* structure of message passing primitives */
MPP_DIRECTI_PTR	ostructs;
int nstructs;
int *cbuf;				/* buffer to probe in ppmsg vector */
CHUNK_PTR *hdReadyList;

{
	char *cp;
	char errtxt[64];

	MPP_DIRECTI_PTR tcon;

	int cc;
	int i, ipctxt, ipff, iplen, ipsrc, iptag;
	int flen;
	int len;
	int match;
	int src;
	int tag, tpff, tpsrc; 
	
	CHUNK *rcp, *rcp2; 

	struct msgid *precvMsg, *precvIds;

	if (*hdReadyList)
		if ((*hdReadyList)->CHUNK_NEXT != *hdReadyList)
		{
 			rcp = (*hdReadyList)->CHUNK_NEXT;
			LISTDELETE(rcp, CHUNK_NEXT, CHUNK_PREV);
			return rcp;
		}

	/*  check if there is a ready chunk in the preposted buffers.  
		repost the receive for the next time around */ 

	for (i = 0; i < nbufs; i ++)
	{
		if ( *cbuf >= nbufs || *cbuf < 0) 
			*cbuf = 0;

		if (rcp = read_chunk(ppmsg + (*cbuf), &src, &tag, &len, MPPREPOST, 
				MPPANY, mfunc)) 	
		{
			rcp->CHUNK_LEN = len; 	/* remember how much data was sent */

			rcp->CHUNK_SRC = src; 	/* this will be a node #, not a tid ! */

			if (pvmdebmask & (PDMNODE | PDMPACKET))
			{
				tpff = pvmget8(rcp->CHUNK_DAT + 12);

				tpsrc = pvmget32(rcp->CHUNK_DAT + 4);

				sprintf(errtxt, "readyChunk()read_chunk src %x node %d len %d ff %d buffer %d\n",
						tpsrc, src, len, tpff, *cbuf);
				pvmlogerror(errtxt);

			}


			/* find the ordering structure for this task, put this chunk
				on this tasks reorder queue */

			if ( !(tcon =  pvm_find_direct(ostructs, nstructs, src)))
			{
				pvmlogerror("frReady() could not find ordering struct \n");	
				return (CHUNK_PTR) NULL;
			}
			else
				order_chunk(tcon->ordering, *cbuf, rcp); /* put on order q */
		
			/*  remove in-order chunks on for this task, put them in 
				callers readyFrags queue */

			while ( rcp = ochunk_delete(tcon->ordering)) /* de-queue chunks */
			{
				if (!*hdReadyList)   /* readyList is empty, initialize */
					*hdReadyList = CHUNK_NEW(0);


				if (pvmdebmask & (PDMNODE | PDMPACKET))
					pvmlogprintf("dequeing chunk from %x len %d\n", rcp->CHUNK_SRC, rcp->CHUNK_LEN);	
				/* XXX - This next part is really ugly! It checks if this
			     *	chunk is an inplace header and if there is a 
			     *	precv outstanding for it. If there is, then the message
			 	 *	can go directly into the user memory. 
				 *
				 * for daemons, it is sufficient to set precvIds to NULL
				 * This has to be done *AFTER* the frag header has been
				 * dequeued from the readyList to preserve ordering
				*/
	
				if ( (flen = pvmget32(rcp->CHUNK_DAT + 8 ) + TDFRAGHDR)  
						!= rcp->CHUNK_LEN )
				{
					ipff = pvmget8(rcp->CHUNK_DAT + 12);
	
					match = 0;
	
					if (ipff  & FFSOM ) /* start of message, check for precv*/ 
					{
						ipsrc = pvmget32(rcp->CHUNK_DAT + 4);
	
						iptag = pvmget32(rcp->CHUNK_DAT + TDFRAGHDR + 4);
	
						ipctxt = pvmget32(rcp->CHUNK_DAT + TDFRAGHDR + 8);
	
				 		if 	(precvMsg = pvm_mpp_get_precvids()) 
						{
							if ( match = (((precvMsg->otid == -1 || ipsrc == precvMsg->otid)
									&& (precvMsg->tag  == -1 || iptag == precvMsg->tag)
									&& (ipctxt == precvMsg->ctxt)) ? 1 : 0) )
							{
								cc = inplaceRecv( src, src, precvMsg->ubuf, 
									precvMsg->len, &iplen, mfunc);	
								precvMsg->len = iplen;
								precvMsg->complete = 1; /* got it ! */
								rcp->CHUNK_RIP = 1; 	/* received in place */
							}
						}
					}
	
					if (!match)
					{
						if ( flen > rcp->CHUNK_MAX ) /* won't fit */
						{
							rcp2 = CHUNK_NEW(flen);
	
							BCOPY(rcp->CHUNK_DAT, rcp2->CHUNK_DAT, rcp->CHUNK_LEN);
	
							rcp2->CHUNK_SRC = rcp->CHUNK_SRC;
	
							rcp2->CHUNK_LEN = rcp->CHUNK_LEN;
	
							CHUNK_FREE(rcp);
	
							rcp = rcp2;
			
						}
	
						cc = inplaceRecv(src, src, rcp->CHUNK_DAT + rcp->CHUNK_LEN,
							rcp->CHUNK_MAX - rcp->CHUNK_LEN, &iplen, mfunc);
	
						rcp->CHUNK_LEN += iplen;
	
					}
						 
				}

				LISTPUTBEFORE(*hdReadyList, rcp, CHUNK_NEXT, CHUNK_PREV);
			}

			(*cbuf) ++;		/* increment to probe the next buffer */
			break;
		}

		(*cbuf) ++;		/* increment to probe the next buffer */
		
	}


	/* if ordered frags have been put into the global ready list, then
		dequeue one of them */

	if (*hdReadyList && (*hdReadyList)->CHUNK_NEXT != *hdReadyList)
	{
 		rcp = (*hdReadyList)->CHUNK_NEXT;
		LISTDELETE(rcp, CHUNK_NEXT, CHUNK_PREV);		
		if (pvmdebmask & (PDMNODE | PDMPACKET))
			pvmlogprintf(" chunk  from hdready from %x len %d\n", rcp->CHUNK_SRC, rcp->CHUNK_LEN);	
		return rcp;
	}
	else
		return (CHUNK_PTR) NULL;

}

/* -------  inplaceRecv() ------- */
int
inplaceRecv(src, tag, buf, len, rlen, llmsg)
int src;
int tag;
char *buf;
int len;
int *rlen;
MSGFUNC_PTR llmsg;
{
 	MSG_INFO imsg;		

	int appid = 0;		/* ignored by tasks */
	int cc;
	int ptype = 0;		/* ouch! XXX. Fix this ! */

	char logtxt[64];

 	cc = (*llmsg->imsgrecv)(0, src, tag, buf,len, ptype, imsg.info, &(imsg.mid));

	if (cc < 0)
	{
		sprintf(logtxt," inplaceRecv(): bad mid (%d)\n", cc);
		pvmlogerror(logtxt);
		return PvmSysErr;
	}

	while ( (cc = (*llmsg->msgdone)(appid, &(imsg.mid), imsg.info)) == 0 )
		/* wait for recv to finish */ ; 

	if (cc < 0)
 	{
		sprintf(logtxt,"inplaceRecv(): error on msgdone (%d)\n", cc);
		pvmlogerror(logtxt);
		return PvmSysErr;
	}

	*rlen = (*llmsg->msglen)(imsg.info);

	return 0;

}


/* ===========  Look for a send mid that has finished  ========= */
/* This routine checks to see if there is a free slot to describe
   a chunk (pkt or frag) that has been asychronously sent.  If
   the available slots (num) are taken, it checks to see if
   the message has completed. If it has, then it frees the chunk
  
   returns 
		>= 0		successful
		< 0			no free descriptors left, none of the previous ones
					have completed.   
*/
int
pvm_mpp_find_midx(mppsendmids, mppoutchunks, current, num, mfunc )
msgmid_t *mppsendmids;
CHUNK_PTR *mppoutchunks;
int *current;
int num;
MSGFUNC_PTR mfunc;
{
int foundidx;
int i, idx;
int appid = 0;			/** XXXX fix this for PGONPUMA */
info_t minfo[MPPINFOSIZE];

	foundidx = 0;

	idx = *current;
	

	for (i = 0; i < num; i++ )
	{

		/* Check if a previous send has finished */
		if (mppsendmids[idx] >= 0)
		{
			if ((*mfunc->msgdone)(appid, &(mppsendmids[idx]), minfo) )
			{
				mppsendmids[idx] = (msgmid_t) -1; 
				if (mppoutchunks[idx])
				{
					CHUNK_FREE(mppoutchunks[idx]);		
					mppoutchunks[idx] = (CHUNK_PTR) NULL;
				}
				foundidx = 1;
			}
		}
		else 
		{
			if (mppsendmids[idx] == (msgmid_t) MPPMIDFREE)
				foundidx = 1;
		}
		
		if (foundidx)
			break;

		if (++idx >= num)
			 idx = 0;
	}

	if (foundidx)
	{
		mppsendmids[idx] = (msgmid_t) MPPMIDALLOCED;

		if ( (*current = idx + 1) >= num)
			*current = 0;

		return (idx);
	}
	else
		return (-1);
}

/* ============ pvm_assign_mid =========== */
/* set the mid in the mppsendmids array */
int
pvm_assign_mid(mppsendmids, mid, idx)
msgmid_t *mppsendmids;
msgmid_t mid;
int idx;
{
	mppsendmids[idx] = mid;
	return 0;
};


/* ============ pvm_assign_chunk =========== */
/* set the chunk pointer in the in the mppoutchunks array */
int
pvm_assign_chunk(mppoutchunks, icp, idx)
CHUNK_PTR *mppoutchunks;
CHUNK_PTR icp;
int idx;
{
	mppoutchunks[idx] = icp;
	return 0;
}

/* ========== pvm_init_asynch_list ========== */
int 
pvm_init_asynch_list(outmids, outchunks, num)
msgmid_t *outmids;
CHUNK_PTR *outchunks;
int num;
{
int i;
	for (i = 0 ; i < num ; i ++ )
	{
		outmids[i] = (msgmid_t) MPPMIDFREE;
		outchunks[i] = (CHUNK_PTR) NULL;
	}
	return 0;
}

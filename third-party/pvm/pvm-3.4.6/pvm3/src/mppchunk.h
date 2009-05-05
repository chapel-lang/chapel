
/* $Id: mppchunk.h,v 1.5 1997/07/09 13:52:35 pvmsrc Exp $ */

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

#ifndef __mppchunk_h__
#define __mppchunk_h__

#include "mppmsg.h"
/* exportable parameters */
extern int MAXSEQ;		/* from mppchunk.c */

/* Some parameters that should be made runtime/startup changeable */

#define TAGBASE 
#define NRBUFS 4
#define NSBUFS 4
#define DFN_MAXSEQ 0xffff

#define INCRSEQ(_seq,_maxseq) (((_seq)++)&(_maxseq))
#define NEXTCHUNKSEQ(_seq, _delta) (((_seq) + (_delta)) & 0xffff )


/*  array of mids of outstanding messages to be sent to backend */
#define NMPPSBUFMIDS 32
#define MPPMIDFREE -1
#define MPPMIDALLOCED -2

#if !defined(IMA_NODE)
#define CHUNK struct pkt
#define CHUNK_NEW pk_new
#define CHUNK_NEXT pk_link
#define CHUNK_PREV pk_rlink
#define CHUNK_SEQ pk_seq
#define CHUNK_SRC pk_src
#define CHUNK_LEN pk_len
#define CHUNK_DAT pk_dat
#define CHUNK_MAX pk_max
#define CHUNK_FREE pk_free
#define CHUNK_RIP	pk_rip
#include "pkt.h"
#else
#define CHUNK struct frag
#define CHUNK_NEW fr_new
#define CHUNK_NEXT fr_link
#define CHUNK_PREV fr_rlink
#define CHUNK_SEQ fr_seq
#define CHUNK_SRC fr_src
#define CHUNK_LEN fr_len
#define CHUNK_DAT fr_dat
#define CHUNK_MAX fr_max
#define CHUNK_FREE fr_unref
#define CHUNK_RIP fr_rip
#include "pvmfrag.h"
#endif

/* ======== Type declarations ========= */
typedef CHUNK *CHUNK_PTR;

/* Information used for sending/receiving packets to/from tasks. 
Keeps track of the send sequence number and the tagbase used for '
sending data. Points to a pkt_order structure for receives so that
reordering happens in a sane manner.
*/

struct mpp_directi
{
	int rtid;			/* task id of the remote task sender/receiver */
	int rpid;			/* pid of the remote proc, used by puma */
	int appid;			/* used for by  PUMA for host to node send */
	int tagbase;		/* base for sending/receive messages */
	int nbufs;			/* nbuffers to use before wrap */
	int sseq;			/* sequence number to use for send */
	struct chunk_order *ordering; /* ordering structure for receive sequencing */ 
};

typedef struct mpp_directi MPP_DIRECTI;
typedef struct mpp_directi *MPP_DIRECTI_PTR;

struct chunk_order 
{
	int seq;			/* sequence number for  this task */
	int nbufs;			/* number of buffers that we have to sequence */	
	int *bufseq;		/* sequence number of the ith buffer for this task */
	CHUNK *oochunks;    /* list of out-of-order packets */	
};  	          

typedef struct chunk_order CHUNK_ORDER;
typedef struct chunk_order *CHUNK_ORDER_PTR;



struct msg_info
{
	int mid;			/* message id from message-passing substrate */
	CHUNK *rchunk;		/* posted chunk for receive */
	int src;			/* source for this message */
	int tag;			/* tag for this message */
	int mxsize;			/* max size of the message */
	int hsize;			/* size of header */
	info_t info[MPPINFOSIZE];		/* info array 8 for pgons, 1 for MPI */
	
};

typedef struct msg_info MSG_INFO;
typedef struct msg_info *MSG_INFO_PTR;

/* ----- Prototype Declarations ------- */

/* ====== Functions that return MPP_DIRECTI_PTR structs ======= */

MPP_DIRECTI_PTR new_directstruct __ProtoGlarp__( (int, int) );
			/* int nsbufs -- number of send buffers 
			*  int nrbufs -- number of receive buffers
			*/
MPP_DIRECTI_PTR new_vdirectstruct __ProtoGlarp__( (int nstructs, int nsbufs, int nrbufs) );
			/* int nstructs -- number of structures to alloc 
			*  int nsbufs -- number of send buffers 
			*  int nrbufs -- number of receive buffers
			*/ 

/* ====== Functions that return CHUNK_ORDER_PTR  ======= */

CHUNK_ORDER_PTR new_chunkostruct __ProtoGlarp__( (int nbufs) );
			/* nbufs -- number of receive buffers */
	
CHUNK_ORDER_PTR new_vchunkostruct __ProtoGlarp__( (int nstructs, int nbufs) );
			/* nstructs -- number of structures
			*  nbufs -- number of receive buffers 
			*/

/* ====== Functions that return struct CHUNK *  ======= */
CHUNK_PTR ochunk_delete __ProtoGlarp__( (CHUNK_ORDER_PTR) );
CHUNK_PTR read_chunk __ProtoGlarp__( (MSG_INFO_PTR, int *, int *, int *, int , int, MSGFUNC_PTR) );

/* ====== Functions that return MSG_INFO_PTR  ======= */

MSG_INFO_PTR init_recv_list __ProtoGlarp__( (int, int, int, int, int, MSGFUNC_PTR) );

/* ====== Functions that return int  ======= */

int init_directstruct __ProtoGlarp__( ( MPP_DIRECTI_PTR, int ));
int order_chunk __ProtoGlarp__( (CHUNK_ORDER_PTR, int, CHUNK_PTR ));
int ochunk_insert __ProtoGlarp__( (CHUNK_ORDER_PTR, CHUNK_PTR) );
int post_receive __ProtoGlarp__( (MSG_INFO_PTR , int, int, int, int, int, int, MSGFUNC_PTR) );
int post_send __ProtoGlarp__( (char * , int, MPP_DIRECTI_PTR, MSGFUNC_PTR) );
int update_seq_numbers __ProtoGlarp__( (CHUNK_ORDER_PTR) );
int pvm_init_asynch_list __ProtoGlarp__( (msgmid_t *, CHUNK_PTR *, int) );
int pvm_assign_chunk __ProtoGlarp__( (CHUNK_PTR *, CHUNK_PTR, int) );
int pvm_assign_mid __ProtoGlarp__( (msgmid_t *, int, int) );
int pvm_mpp_find_midx __ProtoGlarp__( (msgmid_t *, CHUNK_PTR *, int *, int, MSGFUNC_PTR) );

/* ====== Functions that return struct CHUNK *  ======= */
#endif


/* $Id: mppmsg.h,v 1.6 2004/01/14 18:50:58 pvmsrc Exp $ */

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

#ifndef __mppmsg_h__
#define __mppmsg_h__


/* This library uses primitives of a native message passing system. Because
 *	different routes may use different functions, the following documents 
 *	the call sequence expected.
 * 
 int
 imsgsend(int appid, int tag, char *buffer, int len, int dest, int partid,
	 msgmid_t *mid)
 	appid -	used for some host programs to identify application
 	dest -	destinations of message.
 	tag - 	tag to use for sending the message 
 	datap -	buffer to send 
 	len	-	length of buffer (in bytes)
 	partid -partition id
	mid -	message id of send message
 
 	returns integer >= 0 on success. < 0 on failure.
	message id is returned in mid.
 
 int
 imsgrecv(int appid, int src, int tag, char *datap, int len, int partid,
 			int *info, msgmid_t *mid)
 	appid -	used for some host programs to identify application
 	src	-	src of message. -1 means any src
 	tag - 	tag to match on message
 	datap -	buffer in which to receive message
 	len	-	max length of buffer (in bytes)
 	partid -partition id
 	info	- pointer to integer array of memory to store message information
	mid 	- pointer the recv message id
 
 	returns integer >= 0 on success.  < 0 on failure.
	message id returned in mid
 
 msgmid_t 
 msgmerge(msgmid_t *mid1, msgmid_t *mid2)
 	mid1 - message id of message 1 (-1 indicates no existing message )	
 	mid2 - message id of message 2 (-1 indicates no existing message ) 
 	
 	returns mid3, merged id of message 1 and 2, if possible

 int
 msgdone(int appid, msgmid_t mid, info_t *info)
 	appid -	used for some host programs to identify application
 	mid	 -	message id of an incoming message (imsgrecv) or outgoing (imsgsend)
 	
 	returns 0, message not complete; 1 - message complete, < 0 error
 	
 int
 msgsrc(info_t *info)
 	info -	pointer to low-level message passing information block
 	
 	returns node of the message src
 
 int
 msglen(info_t *info)
 	info -	pointer to low-level message passing information block
 	
 	returns length of the message given in info block
 
 int
 msgtag(info_t *info)
 	info -	pointer to low-level message passing information block
 	
 	returns tag of the message 
 
*/
#if ( defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2) ) && defined(IMA_NODE)
#include <mpi.h>
#define msgmid_t MPI_Request 
#define info_t MPI_Status
#define MPPINFOSIZE 1
#else
#define msgmid_t int
#define info_t int
#define MPPINFOSIZE 8
#endif
struct msgfunc {
	int (*imsgsend) __ProtoGlarp__( (int, int, char *, int, int, int, msgmid_t *) );
	int (*imsgrecv) __ProtoGlarp__( (int, int, int, char *, int, int, int *, msgmid_t *) );
	msgmid_t (*msgmerge) __ProtoGlarp__( (msgmid_t *, msgmid_t *) );
	int (*msgdone) __ProtoGlarp__( (int, msgmid_t *, info_t *) );
	int (*msglen) __ProtoGlarp__( (info_t *) );
	int (*msgsrc) __ProtoGlarp__( (info_t *) );
	int (*msgtag) __ProtoGlarp__( (info_t *) );
};

typedef struct msgfunc MSGFUNC;
typedef struct msgfunc *MSGFUNC_PTR;

/* ======= Function Protoypes ========= */
MSGFUNC_PTR pvm_nodemsgfunc __ProtoGlarp__(());
MSGFUNC_PTR pvm_hostmsgfunc __ProtoGlarp__(());

int pvm_mpp_message_init __ProtoGlarp__( (int *, int *, int *, int *) );
int pvm_mpp_message_stop __ProtoGlarp__( () );
#endif /* not defined __mppmsg_h */

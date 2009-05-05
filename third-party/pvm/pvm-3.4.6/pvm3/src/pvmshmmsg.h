
/* $Id: pvmshmmsg.h,v 1.1 1997/10/09 14:15:04 pvmsrc Exp $ */

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
 *	pvmshmmsg.h
 *
 * structures and prototypes to handle asynchronous message primitives
 * for shared memory machines.
 *
 */

#ifndef __PVMSHMMSG_H__
#define __PVMSHMMSG_H__

#define NSHMMIDS 1024
/* Already define in pvmmimd.h
 * #define MAXNODES 128
*/
#define NOBUFFER	0
#define ISRECVMSG	1
#define ISSENDMSG	2
#define BUFINFLUX	4
#define	COMPLETE	8
#define ISALLOCED  16

#define SHM_OWNER	1
#define SHM_NOTOWNER 0
#define LOCK_OWNER 1
#define LOCK_NOTOWNER 0


#ifdef IMA_CSPP
#define	SHMIDPIDPART	0xfff
#endif

#ifndef	SHMIDUIDPART
#define	SHMIDUIDPART	0xffff0000
#endif
#ifndef	SHMIDPIDPART
#define	SHMIDPIDPART	0xffff
#endif

#define PERMS		0600

#define CTRLPAGELEN	MAXFRAGSIZE
#define SHMSEGLEN	0x100000	/* 1 Meg Buffering */
#define MAXMSGS	((SHMSEGLEN/MAXFRAGSIZE)-1)

#define TAGENTRY 0
#define LENENTRY 1
#define SRCENTRY 2

typedef int PVM_LOCK;	

struct shm_midlist
{
	int ml_status;  /* status? send, receive, complete */
	int ml_src;		/* source of the message, if recv */
	int ml_dst;		/* destination if send */
	int ml_tag;		/* tag of message */
	char *ml_buf;	/* buffer to send/recv */
	int ml_len;		/* length of the buffer */
	int *ml_info;	/* vector for src, tag, len on recv for caller */
	struct segdesc *shmseg;  /* shared segment to use */
	int shm_msgnum;	/* message num in the shared structure */
	struct shm_midlist *ml_link; /* might put these in a list */
	struct shm_midlist *ml_rlink;
};
	
struct msginfo {
	int mi_status;
	int mi_src;
	int mi_tag;
	int mi_len;
	int mi_midlistId;
	int mi_offset;
};

struct msgctrl {
	int nattach;	/* number attached to this shared memory seg */
	int len;		/* total length of the this segment */
	int nmsgs;		/* how many msgs are posted for receive */
	int maxmsgs;	/* max number of receives that can be posted */
	struct msginfo msgs[MAXMSGS];
};

struct segdesc {
	PVM_LOCK lock;
	char *start;
	int len;
	int segid;
	int bound;
	int iAmOwner;
	int (*pagelock) __ProtoGlarp__( (PVM_LOCK) );
	int (*pageunlock) __ProtoGlarp__( (PVM_LOCK) );
	struct msgctrl *segControl;
	struct shm_midlist  *sendq;
	struct shm_midlist  *recvq;
};

struct keylist
{
	key_t shared_key;
	key_t lock_key;
};

/* -------- Prototypes --------- */

struct segdesc *pvm_shmget __ProtoGlarp__(( key_t, int, int ));

char	*pvm_shmat __ProtoGlarp__(( struct segdesc * ));

int		pvm_lockget __ProtoGlarp__(( key_t, struct segdesc *, int ));
int		pvm_lockdt __ProtoGlarp__(( struct segdesc * ));
int		pvm_msgctrlInit __ProtoGlarp__(( struct msgctrl *,
				int, int, int ));
int		pvm_shmdt __ProtoGlarp__(( struct segdesc * ));
int		pvm_shmlock __ProtoGlarp__(( PVM_LOCK ));
int		pvm_shmmsgdone __ProtoGlarp__(( int ));
int		pvm_shmmsginit __ProtoGlarp__(( key_t, struct segdesc * ));
int		pvm_shmunlock __ProtoGlarp__(( PVM_LOCK ));
int		pvm_irecvshm __ProtoGlarp__(( struct segdesc *, char *,
				int, int, int ));
int		pvm_isendshm __ProtoGlarp__(( struct segdesc *, char *,
				int, int, int, int * ));

/* --------- ProtoTypes that should be supplied by the system ------- */
#if defined(IMA_SUN4) || defined(IMA_SUNMP)
char * shmat __ProtoGlarp__(( int, char *, int));
#endif

#endif /* __PVMSHMMSG_H__ */


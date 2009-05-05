
/* $Id: pvmshmem.h,v 1.9 2004/01/14 18:51:03 pvmsrc Exp $ */

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
 * PVM 3.0 was funded in part by the U.S. Department of Energy, the
 * National Science Foundation and the State of Tennessee.
 */

/*
 *  pvmshmem.h
 *
 * $Log: pvmshmem.h,v $
 * Revision 1.9  2004/01/14 18:51:03  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.8  2000/02/10 20:45:49  pvmsrc
 * Replaced hard-coded /tmp usage with PVMSHMFILE.
 * 	- use pvmgettmp() routine now to determine PVM temp dir.
 * (Spanker=kohl)
 *
 * Revision 1.7  1999/07/08 19:00:24  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.6  1998/08/13  18:31:11  pvmsrc
 * Altered SUNMP to use test and set operations with semaphores
 * 		for page locking instead of MUTEX and cond vars.
 * 	Changes are mainly in pvmshmem.h, with lots of #ifdefs changes.
 * 	Makefile altered to use the PLOCKFILE to indicate the Page Locking
 * 		INLINE code used (from SUNMP.conf).
 * 	Some changes effect AIX MP versions which still use conditional
 * 		variables and may change to semaphores soon.
 * (Spanker=fagg)
 *
 * Revision 1.5  1997/08/06  22:43:23  pvmsrc
 * Added new SGI6 and SGIMP6 arches.
 *
 * Revision 1.4  1997/06/25  22:11:11  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.3  1997/05/21  16:01:56  pvmsrc
 * Updated ifdefs to include AIX4MP arch type.
 *
 * Revision 1.2  1997/01/28  19:28:23  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:43:33  pvmsrc
 * Initial revision
 *
 * Revision 1.13  1995/11/09  20:42:49  manchek
 * use add_then_test instead of test_then_add (SGI)
 *
 * Revision 1.12  1995/07/24  18:29:56  manchek
 * added message header fields to shmpkhdr
 *
 * Revision 1.11  1995/07/20  16:11:17  manchek
 * added ph_cod to packet header; isn't a shared value
 *
 * Revision 1.10  1995/07/05  16:17:49  manchek
 * added pidtid entry state ST_FINISH
 *
 * Revision 1.9  1995/07/03  19:14:24  manchek
 * added USERECVSEMAPHORE flag to simplify ifdefs.
 * added pt_key and pt_cond fields to pidtid table, p_key to struct peer
 *
 * Revision 1.8  1995/06/28  16:43:04  manchek
 * added mb_attached to struct msgboxhdr.
 * severe tidyup
 *
 * Revision 1.7  1995/06/12  16:26:28  manchek
 * changed SGI PAGEUNLOCK to test_and_set
 *
 * Revision 1.6  1995/05/24  19:08:52  manchek
 * changed HPPA shared memory name to HPPAMP
 *
 * Revision 1.5  1995/05/22  19:55:50  manchek
 * added ifdefs for RS6KMP
 *
 * Revision 1.4  1995/05/17  16:49:29  manchek
 * changes from Convex and SGI
 *
 * Revision 1.3  1994/11/08  15:38:05  manchek
 * shared memory damage control
 *
 */


/*
 *		    Page Layout
 *		 _________________
 *		|      lock       | \
 *		|_________________|  \  PVMPAGEHDR
 *		|    ref count    |  /
 *		|=================| /
 *		|                 |
 *		|      frag       |
 *		|       .         |
 *		|       .         |
 *		|       .         |
 */

#ifdef IMA_SYMM
#define	PVMPAGEHDR			16
#define PVM_INITLOCK(cp)	s_init_lock((slock_t)(cp))
#define PVM_LOCK(cp)		s_lock((slock_t)(cp))
#define PVM_UNLOCK(cp)		s_unlock((slock_t)(cp))
#endif /*IMA_SYMM*/

/* page header and incoming message box header */

#ifdef IMA_KSR1
struct shmpghdr {
	int pg_ref;				/* count */
#define pg_lock pg_ref
};

struct msgboxhdr {
	int mb_read;			/* message last read */
	int mb_last;			/* message last received */
#define mb_lock mb_read
	int mb_attached;		/* used for connection ack */
};

#define	PVMPAGEHDR			sizeof(struct shmpghdr)
#define PAGELOCK(cp)		_gspwt(cp)
#define PAGEUNLOCK(cp)		_rsp(cp)
#define _SC_PAGESIZE		_SC_PAGE_SIZE
#endif /*IMA_KSR1*/

#ifdef IMA_ALPHAMP
#include <sys/mman.h>

struct shmpghdr {
	msemaphore pg_lock;		/* mutex lock */
	int pg_priv;			/* TRUE if page is private */
	int pg_ref;				/* reference count */
};

struct msgboxhdr {
	msemaphore mb_lock;		/* mutex lock */
	int mb_read;			/* message last read */
	int mb_last;			/* message last received */
	int mb_sleep;			/* Is task blocked on a semaphore? */
	int mb_attached;		/* used for connection ack */
};

#define	USERECVSEMAPHORE	1
#define	PVMPAGEHDR			sizeof(struct shmpghdr)
#define PAGEINITLOCK(lp)	msem_init((lp), MSEM_UNLOCKED)
#define PAGELOCK(lp)		msem_lock((lp), 0)
#define PAGEUNLOCK(lp)		msem_unlock((lp), 0)
#endif /*IMA_ALPHAMP*/

#if defined(IMA_RS6KMP) || defined(IMA_AIX4MP) || defined(IMA_AIX5MP)

#include <pthread.h>

struct shmpghdr {
		pthread_mutex_t pg_lock;	/* mutex lock */
		int pg_priv;				/* TRUE if page is private */
		int pg_ref;					/* reference count */
};

struct msgboxhdr {
		pthread_mutex_t mb_lock;	/* mutex lock */
		int mb_read;				/* message last read */
		int mb_last;				/* message last received */
		int mb_sleep;				/* Is task blocked on a semaphore? */
		int mb_attached;			/* used for connection ack */
		pthread_cond_t mb_cond;		/* condition to block on */
};

#define PVMPAGEHDR			sizeof(struct shmpghdr)
#define PAGEINITLOCK(lp)	pthread_mutex_init((lp), NULL)
#define PAGELOCK(lp)		pthread_mutex_lock(lp)
#define PAGEUNLOCK(lp)		pthread_mutex_unlock(lp)

#endif /*IMA_RS6KMP and AIX4MP and AIX5MP */

#ifdef IMA_SUNMP
#include <synch.h>
#include <thread.h>
#include <stdio.h>

struct shmpghdr {
#ifdef PVMUSEMUTEX
	mutex_t pg_lock;		/* mutex lock */
#else
	volatile int pg_lock;	/* shema lock */
#endif /* PVMUSEMUTEX */
	int pg_priv;			/* TRUE if page is private */
	int pg_ref;				/* reference count */
};

struct msgboxhdr {
#ifdef PVMUSEMUTEX
	mutex_t mb_lock;		/* mutex lock */
#else
	volatile int mb_lock;	/* shema lock */
#endif /* PVMUSEMUTEX */
	int mb_read;			/* message last read */
	int mb_last;			/* message last received */
	int mb_sleep;			/* Is task blocked on a semaphore? */
	int mb_attached;		/* used for connection ack */
	cond_t mb_cond;			/* condition to block on */
};

#define	PVMPAGEHDR			sizeof(struct shmpghdr)

#ifdef PVMUSEMUTEX	/* If we use mutex and cond vars instead of semas */
	
#define PAGEINITLOCK(lp)	mutex_init((lp), USYNC_PROCESS, 0)
#define PAGELOCK(lp)		mutex_lock(lp)
#define PAGEUNLOCK(lp)		mutex_unlock(lp)

#endif /* PVMUSEMUTEX */


#ifndef PVMUSEMUTEX /* If no mutex then use Test And Set and semaphores */

#define USERECVSEMAPHORE        1

#define PAGEINITLOCK(lp)         (*(lp) = 0)
#define PAGELOCK(lp)             while (Test_and_Set(lp, 1L));
#define PAGEUNLOCK(lp)           Test_and_Set(lp,0L)

#endif /* No PVMUSEMUTEX*/

#endif /*IMA_SUNMP*/

#if defined(IMA_SGIMP) || defined(IMA_SGIMP6) || defined(IMA_SGIMP64)
#include <mutex.h>

struct shmpghdr {
	unsigned long pg_lock;
	int pg_priv;				/* TRUE if page is private */
	unsigned long pg_ref;		/* reference count */
};

struct msgboxhdr {
	unsigned long mb_lock;		/* mutex lock */
	int mb_read;				/* message last read */
	int mb_last;				/* message last received */
	int mb_sleep;				/* Is task blocked on a semaphore? */
	int mb_attached;			/* used for connection ack */
};

#define	USERECVSEMAPHORE	1
#define PAGEINITLOCK(lp)	(*(lp) = 0)
#define PAGELOCK(lp)		while (test_and_set(lp, 1L)) ;
#define PAGEUNLOCK(lp)		test_and_set(lp, 0L)
#define	TEST_ADD(addr,inc)	add_then_test(addr, (long)inc)
#define	PVMPAGEHDR			sizeof(struct shmpghdr)
#endif /*IMA_SGIMP || IMA_SGIMP6 || IMA_SGIMP64*/

#if defined(IMA_CSPP) || defined(IMA_HPPAMP)
#include <unistd.h>
#define _SC_PAGESIZE		_SC_PAGE_SIZE

/* shmat is guaranteed to return value that is page aligned, so
   lock is already aligned */

typedef struct pvmspplock_s { char lock [64]; } pvmspplock_t;

struct shmpghdr {
	pvmspplock_t pg_lock;
	int pg_priv;			/* TRUE if page is private */
	int pg_ref;				/* reference count */
};

struct msgboxhdr {
	pvmspplock_t mb_lock;
	int mb_read;			/* message last read */
	int mb_last;			/* message last received */
	int mb_sleep;			/* Is task blocked on a semaphore? */
	int mb_attached;		/* used for connection ack */
};

#ifdef IMA_CSPP
#include <sys/cnx_pattr.h>
int cnx_getpattr();
int get_scid();
#endif

key_t pvmkey __ProtoGlarp__((int));
void init_lock __ProtoGlarp__((pvmspplock_t *, char *, int));
int acquire_lock __ProtoGlarp__((pvmspplock_t *, char *, int));
int release_lock __ProtoGlarp__((pvmspplock_t *, char *, int));
int __ldcws32 __ProtoGlarp__((int *));

#define	USERECVSEMAPHORE	1
#define PAGEINITLOCK(lp)	init_lock(lp, __FILE__, __LINE__)
#define PAGELOCK(lp)		acquire_lock(lp, __FILE__, __LINE__)
#define PAGEUNLOCK(lp)		release_lock(lp, __FILE__, __LINE__)

#define	PVMPAGEHDR			sizeof(struct shmpghdr)
#endif /* IMA_CSPP || IMA_HPPAMP */

/* header of pid -> tid table */

struct pidtidhdr {
	int i_proto;				/* t-d protocol */
	int i_dsig;					/* native data format */
	int i_next;					/* pointer to next (maybe) free pidtid */
	int i_bufsiz;				/* shared-memory buffer size */
	int i_dpid;					/* pvmd's Unix proc ID */
};

/* pid -> tid table entry */

struct pidtid {
	int pt_pid;					/* Unix proc id */
	int pt_tid;					/* PVM task id */
	int pt_ptid;				/* PVM parent task id */
	int pt_stat;				/* Is it ready to recv msg? */
	int pt_key;					/* Shared memory segment key */
	int pt_cond;				/* Task status cue to pvmd */
#ifdef IMA_CSPP
	int pt_node;				/* node number */
#endif
};
#define ST_NOTREADY		0		/* not ready */
#define ST_SHMEM		1		/* has shared-memory buffer */
#define ST_SOCKET		2		/* has socket connection */
#define ST_FINISH		3		/* task exited */
#define ST_EXIT			-1		/* misnomer for "slot not in use" */

/* packet header */

struct shmpkhdr {
	int ph_src;					/* source tid */
	int ph_dst;					/* destination tid */
	int ph_sdr;					/* sender's tid */
	int ph_dat;					/* relative page address in sender's buf */
	int ph_len;					/* length of data (after assumed header pad) */
	int ph_flag;				/* fragment flags */
	int ph_tag;					/* message type tag */
	int ph_ctx;					/* message context */
	int ph_enc;					/* message encoding */
	int ph_wid;					/* message wait context id */
	int ph_crc;					/* message checksum */
};

/* tasks whose msg buf has been mapped into our address space */

struct peer {
	struct peer *p_link;		/* dll */
	struct peer *p_rlink;
	int p_tid;					/* task ID */
	int p_key;					/* shared-memory/semaphore key */
	int p_shmid;				/* shared-memory ID of msg buffer */
	int p_semid;				/* semaphore ID */
	char *p_buf;				/* peer's msg buffer */
	char *p_dbuf;				/* my dynamic buffer for this guy */
	int p_dlen;					/* size of dynamic buffer */
	int p_exited;				/* task exit flag */
#ifdef IMA_CSPP
	int p_node;					/* node number */
#endif
};

#ifdef IMA_CSPP
#define MAXFRAGSIZE	0x4000		/* max fragment size (16K) */
#define FRAGPAGE	4			/* number of pages in each fragment */
#define SHMBUFSIZE	0x180000	/* 1.5 MB of shared msg buffer for each task */
#else
#define MAXFRAGSIZE	0x100000		/* max fragment size (4K) */
/* now... 256K? */
#define FRAGPAGE	1			/* number of pages in each fragment */
#define SHMBUFSIZE	0x100000	/* 1 MB of shared msg buffer for each task */
#endif

#define INBOXPAGE	5			/* size of incoming box (number of pages) */
#define PERMS		0600		/* permissions for shared-memory msg buffers */
#define	BUSYWAIT	1000000		/* max wait cycles before backing off */
#define PVMSHMFILE	"%s/pvmshm.%d"	/* file to map to shared memory */

struct peer *peer_conn __ProtoGlarp__((int tid, int *new_connection));
void peer_init __ProtoGlarp__((void));
void peer_wake __ProtoGlarp__((struct peer *pp));
void peer_cleanup __ProtoGlarp__((void));


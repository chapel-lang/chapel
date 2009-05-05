
/* $Id: shmd.h,v 1.3 2001/07/30 16:02:47 pvmsrc Exp $ */

/* header file needed for silly stuff like fixed contexts blah */
/* (c) Graham E. Fagg, University of Tennessee, Knoxville, TN, USA */
/* Contact address fagg@cs.utk.edu   or  pvm@msr.csm.ornl.gov */

#include <sys/types.h>	/* needed for key_t */

/* Globals */

/* These may be changed by knowledgable users */
/* MAXPAGESALLOWED sets the maximum number of pages allowed per segment */
/* This is important as some machines allow very large segments and others */
/* i.e. Suns under Solaris default to 1Megabyte segments */
/* The daemon will attempt to get segments upto but not exceeding this size */

#define MAXPAGESALLOWED	(4096)

/* This controls the maximum number of segments the daemon is allowed to */
/* allocate. Again this is important as some machines may allow all VM to */
/* be allocated to shared memory while other have small limits i.e. Solaris */
/* The larger the number the better the interlacing of memory between procs */
/* and thus less sharing of memory (less contension). Too high a number and */
/* you will have no memory left for what yoru program is actually doing! */

#define MAXSEGS 8 



/* This is the data structure stored in the head shm segment */

typedef struct blockinfo {
	/* now for the block map itself */
	/* not that we statically allocate this upto the maximum */
	int pagemap [MAXPAGESALLOWED];

	/* we put the block map first as it is most used and will */
	/* hopefully be put on a page boundary by the nice block init routines */

	/* now for the useful stuff */

	key_t semkey;	/* semaphore for this infomation */
	key_t segkey;		/* key for this segment (infomation only) */
	int   semid;	/* id of actual semaphore */
	int   segid;    /* segment id */
	int	  psize;		/* page size, should be obvious */
	int   npages;		/* total number of pages in this segment */
	
	/* ptr to start of first page in segment */
	unsigned  segoffset;

	/* not an absolute value as each process may map a shared memory segment */
	/* into a different part of its memory map */
	/* expect this to be 0 or after HEADER in first segment only */

	int	pagesfree;		/* total number of pages free */
	int	largestblockfree;	/* largest continuous block of pages free */
	int largeststart;		/* where this block starts */
							/* used to optimse certain allocation schemes */

	/* some book keeping stuff */
	/* not enabled all the time */
	/* also, watch out for roll overs */
	unsigned int	totalallocated;		/* how many have been used */
	unsigned int 	totalfreed;			/* how many have been recycled  */	
	unsigned int 	jumpsaved;			/* search shortcut effectiviness */
	unsigned int 	firstfound;			/* how often the first free saved us */
	unsigned int	highestutilised;	/* how many were in use */
	unsigned int	largestused;		/* largest set of blocks used */
	
	/* links */
	/* Although this isn't a link'd list we have a relative offset */
	/* to the start of the next blockinfo struct from the start of this one */

	/* esp important as I force the blocks onto page boundaries... */
	/* so you cannot just inc by blockinfo_t size to get to the next one */

	int	nextoffset;				/* relative address in bytes */
	int previousoffset;			/* relative address in bytes */

} blockinfo_t;






typedef struct {
	int			tid;		/* attached TID */
	key_t		mqkey;		/* message queue key */
	int			mqid;		/* message queue id */
}	tidinfo_t;


typedef struct {
	/* First segment info */
	key_t	seg0key;
	int		seg0id;
	int		numsegs;
	int		maxsegsize;
	int		maxsegsizeallowed;
	/* note any other info is in the blockinfo structures */
	/* which are stored in their own headers to avoid contension */
	/* lock problems if they were all stored here */

	/* tid contact info */
	unsigned maxtids;		/* maximum of tids allowed */
	unsigned tidsinuse;		/* number of tids in table */
	unsigned freeslots;		/* should be obvious */
	unsigned firstfree;		/* for when we are not using hashing */
							/* and doing a linear search */

	/* tid table stats */
	unsigned maxtidsused;	/* the most we had at once */
	unsigned totalstids;	/* how many are/have been through here */
	unsigned tidsexited;	/* how many have said good bye */
	unsigned tidsmurdered;	/* how many have been 'murdered' */

	/* First TIDINFO entry would follow here! */


} shm_header_info_t;


typedef struct {
	long mtype;
	int	 from;
	int  seg;
	int  block;
	int  pages;
	int  bytes;
}	msg_info_t;



/*
Semaphore control header for Binary Semaphores only

GEF UTK/ORNL 
20 Jan 1997
*/

/* This operation creates the semaphore set */
/* Should only be called by one process as it destroys any pre-existing ones */

int init_sem (key_t k, int n);

/* This operation waits untill the above semaphore exits */
/* Yes it can be done by nesting semaphores, but hey its late */

int get_sem (key_t k, int n);


/* Finially, he who creates can bloody well destroy! */

void del_sem (int semid);

int sem_is (key_t k, int n);	/* gets the actual semaphore value */

/* set and reset or P,Q or lock unlock operations! */

void lock_sem (int sem, int n);

void unlock_sem (int sem, int n);



/* 
 Prototypes of functions that are provided 
*/

/* block functions */
int initblockstruct();
int get();
int back();
void map();

/* tidinfo functions */
int init_tidinfo ();
int add_tidinfo ();
int find_tidinfo_id ();
int remove_tidinfo ();


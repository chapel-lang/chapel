
static char rcsid[] =
	"$Id: tidinfo.c,v 1.2 1999/03/04 22:03:48 pvmsrc Exp $";

#include "shmd.h"
#include <stdio.h>

/* global */
/* this is used so we don't have to recheck the head ptr every time */

shm_header_info_t*		headptr=(shm_header_info_t *)0;
tidinfo_t*				tidinfoptr=(tidinfo_t *)0;

int init_tidinfo (ptr, headersize) 
char* ptr;	/* where we start to put the info */
unsigned int headersize; /* how much space we have to play with */
{
	int s, t, n, i, m;
	shm_header_info_t* hdrptr;
	tidinfo_t*		   tptr;
	tidinfo_t*		   tptr0;
	char *			   tmpptr;
	unsigned			totalsize;

	hdrptr = (shm_header_info_t*) ptr;	

	s = sizeof(shm_header_info_t);

	m = headersize;

	t = sizeof(tidinfo_t);

	n = (m - s - 4) / t; 
		/* -4 is used to force 32bit boundary */
		/* as this is the most a structure can be moved by to make aligment */

	printf("Sizeof shm_header_info_t [%d]\n", s);
	printf("Sizeof tidinfo_t [%d]\n", t);
	printf("Number of tidinfo blocks allowed [%d]\n", n);

	hdrptr->maxtids = n;
	hdrptr->tidsinuse = 0;
	hdrptr->freeslots = n;
	hdrptr->firstfree = 0;
	hdrptr->maxtidsused = 0;
	hdrptr->totalstids = 0;
	hdrptr->tidsexited = 0;
	hdrptr->tidsmurdered = 0;

	printf("Wrote header info at [0x%lx]\n", (long) hdrptr);
	fflush(stdout);

	/* now to find start of tidinfo table */
	tmpptr = ptr + s; 
	tptr = (tidinfo_t *) tmpptr;
	tptr0 = tptr;

	printf("Start of tidinfo at [0x%lx]\n", (long) tptr);
	fflush(stdout);

	for(i=0;i<n;i++) { /* init the space the old fashioned way */
		tptr->tid = -1;
		tptr->mqkey = -1;
		tptr->mqid = -1;
		tptr++;
	}
	
	/* paranoid check */
	printf("End   of tidinfo at [0x%lx]\n", (long) tptr);
	printf("End   of tidinfo[0] at [0x%lx]\n", (long) &tptr0[0]);
	printf("End   of tidinfo[%d] at [0x%lx]\n",
		n-1, (long) &tptr0[n-1]); 
	fflush(stdout);

	if (n>0) { /* i.e. we are ok, then set the global pointers */
		headptr = hdrptr;
		tidinfoptr = tptr0;
	}
		
	return (n);
}


int		add_tidinfo (tid, key, id) 
int tid;
key_t key;
int id;
{
	int slot, c, i, j;
	int max;


	if (!headptr) {
		fprintf(stderr,"Panic, headerinfo address not set???\n");
		fflush(stderr);
		exit(-9);
	}

	if (!tidinfoptr) {
		fprintf(stderr,"Panic, tidinfo address not set???\n");
		fflush(stderr);
		exit(-9);
	}

	if (!(headptr->freeslots)) {
		fprintf(stderr,"Warning: TID [0x%x] cannot be put in shm tid info table as there is no space left\n", tid);
		fflush(stderr);
		return (-1);
	}

	/* get basic info first */
	max = headptr->maxtids;

	/* ok, lets find one */
	slot = (tid * 5) % max;	/* a simple hash lookup */

	for(c=0;c<max;c++) { /* only search for the table length */
		if (tidinfoptr[slot].tid == -1) {	/* found a spare/free one */
			tidinfoptr[slot].tid = tid;
			tidinfoptr[slot].mqkey = key;
			tidinfoptr[slot].mqid = id;
			/* main info */
			headptr->tidsinuse++;
			headptr->freeslots--;

			/* Stats info */
			headptr->totalstids++;
			if (headptr->maxtidsused < headptr->tidsinuse)
				headptr->maxtidsused = headptr->tidsinuse;

			return (slot);	/* from 0 to maxtids-1 */
		}
		
		/* paranoid check */
		if (tidinfoptr[slot].tid == tid) { /* already in table... */
			fprintf(stderr,"TID [0x%x] is already in the shm tidinfo table at slot [%d]\n", tid, slot);
			fflush(stderr);
			/* do we exit or continue? */
			/* continue and return -1  which means no shm usage! */
			return (-1);
		}

		/* no space so inc counters with wrap around */
		slot++;
		if (slot==max) slot=0;
	} /* for linear search */

	/* default case is no space */
	fprintf(stderr,"TID [0x%x] cannot be put in tidinfo table for unknown reason...????", tid);
	fflush(stderr);

	return (-1);

}


int		remove_tidinfo (tid) 
/* returns slot or -1 if error */
int tid;
{
	int slot, c, i, j;
	int max;


	if (!headptr) {
		fprintf(stderr,"Panic, headerinfo address not set???\n");
		fflush(stderr);
		exit(-9);
	}

	if (!tidinfoptr) {
		fprintf(stderr,"Panic, tidinfo address not set???\n");
		fflush(stderr);
		exit(-9);
	}

	if (!(headptr->tidsinuse)) {
		fprintf(stderr,"Warning: TID [0x%x] cannot be in shm tid info table as there are no entries!\n", tid);
		fflush(stderr);
		return (-1);
	}

	/* get basic info first */
	max = headptr->maxtids;

	/* ok, lets find one */
	slot = (tid * 5) % max;	/* a simple hash lookup */

	for(c=0;c<max;c++) { /* only search for the table length */
		if (tidinfoptr[slot].tid == tid) {	/* found it */
			tidinfoptr[slot].tid = -1;
			tidinfoptr[slot].mqkey = -1;
			tidinfoptr[slot].mqid = -1;
			/* main info */
			headptr->tidsinuse--;
			headptr->freeslots++;

			/* Stats info */
			/* have to be updated by main caller as we don't know */
			/* if it exited or was killed */

			return (slot);	/* from 0 to maxtids-1 */
		}
		
		/* not found so inc counters with wrap around */
		slot++;
		if (slot==max) slot=0;
	} /* for linear search */

	/* whatthe case is */
	fprintf(stderr,"TID [0x%x] was not found in tidinfo table for unknown reason...????", tid);
	fflush(stderr);

	return (-1);

}

int		find_tidinfo_id (ptr, tid) 
/* returns id or -1 if error */
/* this is used by the clients (once only, I hope ) */
int tid;
char* ptr;	/* start of first mem segment i.e. system header location */
{
	int slot, c, i, j;
	int max;
	char* tmpptr;
	shm_header_info_t* sptr;
	tidinfo_t* tptr;


	if (!ptr) {
		fprintf(stderr,"Panic, SYSTEM header address not set???\n");
		fflush(stderr);
		exit(-9);
	}

	sptr = (shm_header_info_t *) ptr;				/* system pointer */
	tmpptr = ptr + (sizeof(shm_header_info_t));		
	tptr = (tidinfo_t *) tmpptr;					/* tidinfo pointer */


	/* get basic info first */
	max = sptr->maxtids;

	/* ok, lets find one */
	slot = (tid * 5) % max;	/* a simple hash lookup */

#ifdef TIDINFODEBUG
	printf("Searching for TID [0x%x] starting at mem location [0x%x] for [%d] tidinfo entries using hash of [%d] \n", tid, tptr, max, slot);
#endif

	for(c=0;c<max;c++) { /* only search for the table length */
		if (tptr[slot].tid == tid) {	/* found it */
				return (tptr[slot].mqid);
		}
		
		/* not found so inc counters with wrap around */
		slot++;
		if (slot>=max) slot=0;
	} /* for linear search */

	/* default case not found */
/* 	fprintf(stderr,"TID [0x%x] cannot be found in tidinfo table for unknown reason...????", tid); */
/* 	fflush(stderr); */

	return (-1);
}


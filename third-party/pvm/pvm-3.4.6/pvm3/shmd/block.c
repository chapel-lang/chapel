
static char rcsid[] =
	"$Id: block.c,v 1.1 1999/03/04 21:30:49 pvmsrc Exp $";

#include <stdio.h>
#include <stdlib.h>
#include "shmd.h"

/* int get(); */
/* int back(); */
/* void map(); */


int initblocks (blkptr, psize, npages, semkey, segkey, semid, segid) 
blockinfo_t* blkptr;
int psize, npages;
key_t semkey;
key_t segkey;	/* used for info only */
int semid, segid;
{
	int i,j;
	int r,s;

	blkptr->semkey = semkey;
	blkptr->segkey = segkey;
	blkptr->semid = semid;
	blkptr->segid = segid;
	blkptr->psize = psize;
	if (npages<MAXPAGESALLOWED) blkptr->npages = npages;
	else	blkptr->npages = MAXPAGESALLOWED;
			/* should log a warning here */
	
	blkptr->pagesfree = blkptr->npages;
	blkptr->largestblockfree = blkptr->npages;
	blkptr->largeststart = 0;	/* i.e. at the very very start */

	/* stats setup */
	blkptr->totalallocated = 0;
	blkptr->totalfreed = 0;
	blkptr->jumpsaved = 0;
	blkptr->firstfound = 0;
	blkptr->highestutilised = 0;
	blkptr->largestused = 0;

	/* now we can setup the block/pagemap */
	for(i=0;i<npages;i++) 
		blkptr->pagemap[i] = npages-i;

	return (blkptr->npages);	/* return the number of pages provided */

}



int	getblocks(blkptr, id, size) 
blockinfo_t* blkptr;
int id, size;
{
	/* here we find first place where the section will fit */
int i,j,k;

#if SEM
	lock_sem (blkptr->semid, 0);
#endif /* SEM */

	for (i=0;i<blkptr->npages;i++) {
		if (blkptr->pagemap[i] >= size) {	/* found a block! */
			/* first mark it as ours */
			/* could use a copy operation here todo[] */
			for(j=i;j<(i+size);j++) blkptr->pagemap[j] = -id;
			blkptr->pagesfree -= size;
			/* STATS */
			/* these will be ifdef'd later */
			blkptr->totalallocated+= size;
			if (blkptr->highestutilised < (blkptr->npages - blkptr->pagesfree))
				blkptr->highestutilised = (blkptr->npages - blkptr->pagesfree);
			if (blkptr->largestused < size) blkptr->largestused = size;
			/* to set largest block free and largest start will take a search */
			/* so we will worry about them later. */
			/* todo[] */
			/* unless we can make an assumption.. */
			/* which does fail BTW! */
			/* have just put it here to see how usefull it is.. */
			blkptr->largestblockfree -= size; /* bad */
			blkptr->largeststart = i+size; 	  /* possibly worse */

#if SEM
			unlock_sem (blkptr->semid, 0);
#endif /* SEM */

			return (i);	/* i.e. where we started the allocation */

		} /* end we found one */

	if (blkptr->pagemap[i]>0) /* i.e. we are in a block that is too small */
		/* jump ahead by the size of the block that is too small */
		{ 
			blkptr->jumpsaved += (blkptr->pagemap[i]) -1; 
			i += (blkptr->pagemap[i]) -1; 
		}
		
		
	} /* else just do a linear search (yuck) */

#if SEM
	unlock_sem (blkptr->semid, 0);
#endif /* SEM */

	return (-1); /* no single block big enough! */

}

int freeblock_by_id(blkptr, id) 
blockinfo_t* blkptr;
int id;
{
	int i, j; 
	int freed=0;

	/* NOTE */
	/* can only be called by owner of segments */
	/* i.e. pvm_shmd or sniped/snipe_shmd */

	/* Does not lock segment as it calls a routine which does and therefore */
	/* would lock itself out... */
	/* Is safe in that freeblock() calls lock and */
	/* this routine only changes pages that belond to a DEAD task */
	/* Weak assumptions, but true */

	/* short cuts */
	/* if none allocated in this segment, then just return */
	if (blkptr->npages == blkptr->pagesfree) return (freed);

	/* do a linear search of blocks and when you find one that matches */
	/* nuke it (i.e. call freeblock() on it */

	for(i=0;i<blkptr->npages;i++) {
		if ((blkptr->pagemap[i])==(-id)) { 
			/* we have the start of a matching block */ 
#ifdef BLOCKDEBUG
			printf("Block started at [%d]\n", i);
#endif /* BLOCKDEBUG */
			for (j=0;j<((blkptr->npages)-i);j++) 
				if ((blkptr->pagemap[i+j])!=(-id)) break;
			/* j = size of section owned by id in blocks */
#ifdef BLOCKDEBUG
			printf("block owned by [%d] found at [%d] of length [%d]\n",
					id, i, j);
#endif /* BLOCKDEBUG */
			freed += freeblock (blkptr, id, j, i);	/* free it */
					/* this does all the stats for us as well... */
			}	
		else if(blkptr->pagemap[i]>0) {
				blkptr->jumpsaved += (blkptr->pagemap[i])-1;
				i += (blkptr->pagemap[i])-1;
				}
	}	/* for linear search with hops(tm) */

#ifdef BLOCKDEBUG
	if(freed) {
		printf("Total pages previously owned by [%d] freed [%d]\n",
				id, freed);
		printf("Total pages free in segment [0x%x] is now [%d]\n", 
				blkptr->segkey, blkptr->pagesfree);
		}
	else 
		printf("No pages owned by [%d] found in this segment [0x%x]\n",
				id, blkptr->segkey);

#endif /* BLOCKDEBUG */
			
	return (freed);
}



int freeblock(blkptr, id, size, where)
blockinfo_t* blkptr;
int id, size, where;
/* where is in blocks from 0..npages-1 */
/* not an abs addr yet */
{
	int i,j,k,last;
	int after=0;	/* number free after last in our block... */

#if SEM
	lock_sem (blkptr->semid, 0);
#endif /* SEM */

	/* first 3 checks paranoid but safer */

	/* first check, right address ? */

	if (blkptr->pagemap[where]!=(-id)) {
		fprintf(stderr,"Help, block belongs to wrong task!\n");
#if SEM
		unlock_sem (blkptr->semid, 0);
#endif /* SEM */
		return(-1);
	}

	/* is block too big ? */
	if ((where+size)>blkptr->npages) {
		 fprintf(stderr,"Help, block is too big to fit in segment?!\n");
#if SEM
		 unlock_sem (blkptr->semid, 0);
#endif /* SEM */
		 return(-1);
 	}

	last = where+size -1;	/* last block in this set of pages */

	/* check last block? */
	/* instead of checking them all... */
	/* note, this check my introduce page faults, except we use it as */
	/* the start of our readdressing of pages loop 8) */
	/* so, we use that page again straight away.... */
	if ((blkptr->pagemap[last]) != -id) {
		 fprintf(stderr,"Help, block is not all ours?!\n");
#if SEM
		 unlock_sem (blkptr->semid, 0);
#endif /* SEM */
		 return(-1);
 	}

	/* ok, get after value... */
	if (last==(blkptr->npages-1)) /* i.e. on the edge */
		after = 0; /* boundary */
	else 
		after = blkptr->pagemap[last+1]; /* not on the edge so get the count! */

	if (after<0) after = 1; /* as after maybe an identifer value */
							/* instead of a count */
							/* note =1 instead of 1 so that we don't have */
							/* to increment later */
	else 
		/* increment after as we are one space to the left... */
		after++;

	for (i=last;i>=where;i--) 	/* fill 'em back in */
		blkptr->pagemap[i] = after++;
								/* incrementing as we go... */

	/* well now we need to check to see if we have run into any other */
	/* free blocks and if so, we can keep on adding them bad boys together */

#ifdef BLOCKDEBUG
	printf("where[%d], i[%d], after[%d]\n", where, i, after);
#endif /* BLOCKDEBUG */
	for(j=where-1;j>=0; j--) 
		if (blkptr->pagemap[j] > 0) blkptr->pagemap[j] = after++;
		else
			break; /* found a reserved block, so stop! */

#ifdef BLOCKDEBUG
	printf("later j+1[%d] new large block of [%d]\n", j, blkptr->pagemap[j+1]);
#endif /* BLOCKDEBUG */

	/* and yes.. we now have a few more pages free.. */

	blkptr->pagesfree += size;

	/* optional STATS */
	if (blkptr->largestblockfree < blkptr->pagemap[j+1]) {
			blkptr->largestblockfree = blkptr->pagemap[j+1];
			blkptr->largeststart = j+1;
#ifdef BLOCKDEBUG
			printf("New largest block at [%d] of size [%d]\n", 
				blkptr->largeststart, blkptr->largestblockfree);
#endif /* BLOCKDEBUG */
		}
	blkptr->totalfreed += size;
#if SEM
	unlock_sem (blkptr->semid, 0);
#endif /* SEM */

	return (size); /* how many did we free */

}

void blockmap(blkptr)
blockinfo_t* blkptr;
{

int i,j;

#if SEM
	lock_sem (blkptr->semid, 0);
#endif /* SEM */

printf("Info and Block map for segment [0x%x]\n", blkptr->segkey);
for(i=0;i<blkptr->npages;i++) printf("%3d\t", blkptr->pagemap[i]);
printf("\n\n");
printf("Pages free left=[%d]\n", blkptr->pagesfree);
printf("Total pages asked for [%d]\n", blkptr->totalallocated);
printf("Total pages freed [%d]\n", blkptr->totalfreed);
printf("Jumps saved [%d] steps\n", blkptr->jumpsaved);
printf("Found first time [%d]\n", blkptr->firstfound);
printf("Highest utilisation [%d]\n", blkptr->highestutilised);
printf("Largest block used [%d]\n", blkptr->largestused);

#if SEM
	unlock_sem (blkptr->semid, 0);
#endif /* SEM */
}


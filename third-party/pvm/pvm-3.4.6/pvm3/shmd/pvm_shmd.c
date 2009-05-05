
static char rcsid[] =
	"$Id: pvm_shmd.c,v 1.2 1999/03/12 20:22:39 pvmsrc Exp $";

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/param.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <pvm3.h>

/* GEF libs */
#include "shmd.h"

/* Prototypes */
unsigned int	get_max_seg_size();
unsigned int	create_segments();
void		clean_segs();
int		make_mqueue();
int		del_mqueue();
void 		bambismother();
void 		huphuphup();

/* Consts */
/* #define MAXSEGS 255		*/ 
/* in the header files now */
/* well its a limit and we do need one */
						/* can be controlled by ifdef or ENV vars */
						/* at run-time */

#ifndef SHM_FAILED
#define SHM_FAILED (void*)-1
#endif

#define FAILEDCOUNT (10)

/* Globals */
unsigned segsize;
unsigned numsegs;
key_t keys[MAXSEGS];
int shmids[MAXSEGS];
void* segptrs[MAXSEGS];
unsigned totalmem;



main(argc, argv) 
int argc;
char *argv[];
{
	int i,id;
	key_t	key;
	int		semid;
	struct shmid_ds shmsbuf;
	unsigned size;
	int pagesize;
	size_t bsize, bsizepb, o1, o2, o3;		/* for working out mem offsets */
	int pages[MAXSEGS];
	char* addr;
	unsigned  firstpagehdr;
	int tid;
	int bufid, from, bytes, msgtag;
	int died;
	int tidslots; /* not an array, but number of tids I can handle at once */
	int slot, left, join;
	key_t mqkey;
	int  mqid;
	int j, cc;
	int finished=0;

	/* first things first */

	/* The PVM stuff */
	tid = pvm_mytid();
	if (tid<=0) { 
		fprintf(stderr,"Cannot join the virtual machine.\n");
		exit (-3);
	}	

	if (register_shmd (tid)) {
		fprintf(stderr,"Cannot setup system mailbox entry.\n");
		exit (-4);
	}	




	o1 = (size_t) (16*1024); /* first offset is a 16K block */

	/* calculate offsets into shmpages */
	pagesize = getpagesize();
	bsize = sizeof (blockinfo_t);
	/* calculate blockinfo size in whole pages (on page boundary) */
	if (bsize%pagesize) bsizepb = ((bsize/pagesize)+1) * pagesize;
	else bsizepb = (bsize);
		
	key = (key_t)	 (0x2d000000+((int)getuid()*0x1000));	/* inital key */
	/* segsize = get_max_seg_size(); */
	segsize = get_max_seg_size(pagesize);
/* printf("seg size %d %x\n", segsize, segsize);
exit (-1); */
	/* numsegs = create_segments(key, keys, shmids, segptrs, MAXSEGS); */
	numsegs = create_segments(key, MAXSEGS);
	if (numsegs<2) {
		fprintf(stderr,"WARNING:\nCan not create 2 or more segments.\nExiting.\n");
		fflush(stderr);
		clean_segs();
		exit (-2);
	}

	for(i=0;i<numsegs;i++) 
		printf("Base addresses are [0x%x]\n", segptrs[i]);

	/* first segment is not all available for messages as it has a header */
	firstpagehdr = o1+(bsizepb*numsegs);
	semid = init_sem(0x4000, 1);	/* todo */
	addr = (char*)segptrs[0]+(o1);
	printf("Blockinfo addresses are [0x%x]\n", addr);
	pages[0] = initblocks ( addr, pagesize, (segsize-firstpagehdr)/pagesize, 
				0x4000+0, keys[0], semid, shmids[0]);

	((blockinfo_t*)addr)->segoffset = firstpagehdr;	
				/* first page starts after header */

	/* First page offsets */
	((blockinfo_t*)addr)->nextoffset = bsizepb;
		/* and the previous is wrapped around... */
	((blockinfo_t*)addr)->previousoffset = (numsegs-1) * bsizepb; 

	/* all other segments are just for message info */
	for(i=1;i<numsegs;i++) {
		semid = init_sem(0x4000+i, 1); /* todo */
		addr = (char*)segptrs[0]+(o1)+(bsizepb *i); 
		printf("Blockinfo addresses are [0x%x]\n", addr);
		pages[i] = initblocks ( addr, 
			pagesize, (segsize/pagesize), 0x4000+i, keys[i], semid, shmids[i]);
		((blockinfo_t*)addr)->segoffset = 0;	
			/* i.e. first page is at the head of the seg */

		/* offsets to previous / next blockinfo structs */
		if (i<(numsegs-1)) 
			((blockinfo_t*)addr)->nextoffset = bsizepb;
		else 	/* wrap around */
			((blockinfo_t*)addr)->nextoffset = -((numsegs-1) * bsizepb);

		((blockinfo_t*)addr)->previousoffset = -bsizepb;
	}	

	/* previous/next wrap around tests */
	/* first forwards */
	printf("Forwards loop\n");
	addr = o1 + (char*)segptrs[0];
	for(i=0;i<=numsegs;i++) {
		printf("segkey [0x%x]\t", ((blockinfo_t*)addr)->segkey );
		addr += ((blockinfo_t*)addr)->nextoffset;
	}
	printf("\n\n");

	/* backwards */
	printf("Backwards loop\n");
	addr = o1 + (char*)segptrs[0];
	for(i=0;i<=numsegs;i++) {
		printf("segkey [0x%x]\t", ((blockinfo_t*)addr)->segkey );
		addr += ((blockinfo_t*)addr)->previousoffset;
	}
	printf("\n\n");




	for(i=0;i<numsegs;i++) {
		addr = (char*)segptrs[0]+(o1)+(bsizepb *i); 
		if (pages[i]) blockmap (addr);
	}
		
/* 	totalmem = segsize * numsegs; totalmem /= (1024*1024);  */
	/* MB */

	printf("Pages available in each segment: ");
	totalmem = 0;
	for(i=0;i<numsegs;i++) {
		printf("seg[%d]:[%d]\t", i, pages[i]);
		totalmem += pages[i];
	}
	totalmem = ((totalmem * pagesize) / 1024);

	printf("Seg size in bytes [%d] and number of segments [%d]\nTotal message buffer memory [%d KBytes]\n",
			segsize, numsegs, totalmem);


	/* Now we can fill in that first section of memory */
	tidslots = init_tidinfo ((char *)segptrs[0], 
							(16*1024));  /* make tidinfo struct */

	/* missed out a few details.. */
	((shm_header_info_t *) segptrs[0])->numsegs = numsegs;

	/* should check tidslots here TODO */

	/* Safe to register myself now */

	if (register_shmd (tid, shmids[0])) {
		fprintf(stderr,"Cannot setup system mailbox entry.\n");
		exit (-4);
	}	

	/* now the pvm daemon knows we are here */
	/* when it exits it sends up a TERM so that we can exit neatly */
	/* we also catch a HUP for completeness */ 
	(void) signal (SIGTERM, bambismother);
#ifndef LINUX
	(void) sigset (SIGTERM, bambismother); /* yes I mean it, ok ? */
#endif
	(void) signal (SIGHUP, huphuphup);

	while (!finished) {
		/* ok, we really would do some work in here... honest */
		bufid = pvm_recv(-1,-1);
		pvm_bufinfo( bufid, &bytes, &msgtag, &from );
		switch (msgtag) {
			case 1: /* join */
				join = from;
				make_mqueue (join, &mqkey, &mqid);
				if (mqid<0) {
					pvm_initsend(PvmDataDefault);
					pvm_pkint (&mqid, 1, 1);  /* failed */
					pvm_send (join, join);
					fprintf(stderr,"TID [0x%x] failed on make_mqueue\n",
						join);
					break;
				}

				slot = add_tidinfo (join, mqkey, mqid);

				if (slot<0) {
					pvm_initsend(PvmDataDefault);
					pvm_pkint (&slot, 1, 1);  /* failed */
					pvm_send (join, join);
					fprintf(stderr,"TID [0x%x] failed on add_tidinfo\n",
						join);
					break;
				}

				/* else ok, so we setup notify and then signal task */

				printf("TID [0x%x] added into tidinfo slot [%d]\n", join, slot);
				pvm_notify (PvmTaskExit, 9, 1, &join);
				fprintf(stderr,"Adding [0x%x] to notify list\n", join);
				fflush(stderr);

				j = 0;	/* i.e. isitok is yes */
				pvm_initsend(PvmDataDefault);
				pvm_pkint (&j, 1, 1);  /* passed */
				fprintf(stderr,"Sending message [%d] to [0x%x] tag [0x%x]\n",
						j, join, join);
				cc = pvm_send (join, join);
				fprintf(stderr,"Sent message [%d] to [0x%x] tag [0x%x] = [%d]\n",
						j, join, join, cc);
				fflush(stderr);
				break;

			case 2: /* exit (polite) */

				left = from;	/* just adds to readability, maybe  */
				del_mqueue (left);
				slot = remove_tidinfo (left);
				printf("TID [0x%x] removed from tidinfo slot [%d]\n", left, slot);

				/* freeing space by this task */
				left = left & 0xFFFF; /* task part of address */
				for(i=0;i<numsegs;i++) {
					addr = (char*)segptrs[0]+(o1)+(bsizepb *i); 
					if (pages[i]) freeblock_by_id (addr, left);
				}

				((shm_header_info_t *)segptrs[0])->tidsexited++;
				break;

			case 3: /* blockmap */
				for(i=0;i<numsegs;i++) {
					addr = (char*)segptrs[0]+(o1)+(bsizepb *i); 
					if (pages[i]) blockmap (addr);
				}
				break;
		    case 9: /* TERM and caught by pvmd with notify */
				/* Find out who died */
				pvm_upkint(&died, 1, 1);
				
				del_mqueue (died);
				slot = remove_tidinfo (died);
				printf("TID [0x%x] removed from tidinfo slot [%d]\n", died, slot);

				printf("Task [0x%x] exited. Deleting any blocks allocated.\n",
						died);

				/* freeing space by this task */
				died = died & 0xFFFF; /* task part of address */

				for(i=0;i<numsegs;i++) {
					addr = (char*)segptrs[0]+(o1)+(bsizepb *i); 
					if (pages[i]) freeblock_by_id (addr, died);
				}
				
				((shm_header_info_t *)segptrs[0])->tidsmurdered++;

				break;


			default: /* ? */
				fprintf(stderr,"Unknown message from [0x%x] of tag [0x%x]\n",
						from, msgtag);
				pvm_freebuf (bufid);
				break;

			} /* end switch */

	}

	clean_segs();
	pvm_exit(); /* which takes the PVM reg with it! */
	exit (0);	/* Normally I hope */
}




/* unsigned int	get_max_seg_size() */
unsigned int	get_max_seg_size(psize)
unsigned psize;
{
	key_t key;
	unsigned int	size;
	int	id;

	key = (key_t)	 0x2d000000;	
/* 	size = 1024 * 1024 * 1024; */
/*	size = 1024 * 1024 * 32;		*/
/* start looking from 32MBytes */
	size = MAXPAGESALLOWED * psize;	/* start from our limit */

	for(;;) {
		id = shmget (key, size, IPC_CREAT | 0600);

		if (id>0) {
/* 		printf("id\t%d\n",id); */
/* 		printf("key\t%x\n",key); */
/* 		printf("size\t[0x%x or %u bytes] [%u KBytes]\n", size, size, size/1024); */
		break;
		}
/* 	key += 0x10; */

	   if (id<0) {
		   if (errno==EINVAL) perror("shmget:EINVAL");
		   if (errno==ENOSPC) perror("shmget:ENOSPC");
		   size /= 0x2;	
/* 		   break; */
		  }
	}
	shmctl (id, IPC_RMID, 0);	/* I know its a waste.. but.. */

	return (size);
}

unsigned int create_segments(basekey, maxsegs)
key_t	basekey;
unsigned int maxsegs;
{
	int c=0;	/* number of segments that we get a valid id for */
	int v=0;	/* number of segments that we get a valid ptr for */
	void *ptr;
	key_t key;
	struct shmid_ds shmsbuf;
	int suc, attached;
	int i;
	int f;	/* f is for failed */
	int num;	/* number of segments that we have a valid id and valid ptr for */


	key = (key_t) basekey;

	/* make sure we are all reset fully */
	for(i=0;i<maxsegs;i++) { keys[i] =-1; shmids[i] = -1; segptrs[i] = (void*)0; };

	/* loop until we get a valid key for each segment */
	f = 0;
	for(c=0;c<maxsegs;) {
		keys[c] = basekey; 	/* this one gets this key */
		basekey += 0x10;	/* the next one gets a different one */
	
			
		shmids[c] = shmget (keys[c], segsize, IPC_CREAT | 0600);
		if ((int)shmids[c]==-1) { 
			perror("shmget");
			f++;
		}
		else {	/* we got an id ok */
			f = 0;	/* reset failed count */
			c++;	/* inc key count */
		}
		if (f==FAILEDCOUNT) {
			fprintf(stderr,"[create_segments] failed shmget() count reached.\n");
			break;
		}

	}	
			
		
	/* Now to try and attach to these segments */		
	for (v=0;v<c;v++) {
/* 		segptrs[v] = shmat (shmids[v],0,SHM_RDONLY); */
		segptrs[v] = shmat (shmids[v],0,0);
/* 		if ((int)segptrs[v]==-1) {  */
		if (segptrs[v]==SHM_FAILED) { 
			perror("shmat");
			fprintf(stderr,"shmat error code [%d]\n", segptrs[v]);
			segptrs[v] = (void *) 0;
		}
	}	

	
/* sleep(1); */
#ifdef DEBUG
	for (i=0;i<c;i++) {
		printf("id\t%d\n",shmids[i]);
		printf("key\t%x\n",keys[i]);
		printf("ptr\t0x%x\n", segptrs[i]);
	}
#endif

	/* now to get rid of the ones that don't count! */
	num = 0; /* meaning none */
	for(i=0;i<(c);i++)
		if (!segptrs[i]) {
			shmctl (shmids[i], IPC_STAT, &shmsbuf);
			attached=shmsbuf.shm_nattch; 
			suc = shmctl (shmids[i], IPC_RMID, 0);
			shmids[i] = -1;
			keys[i] = -1;
		}
		else /* i.e. its ok */
			num = i+1;	/* i+1 as i is from 0..N-1 not 1..N ! */


	return (num);

}

void clean_segs () 
{
int i;

	for (i=0;i<MAXSEGS;i++) shmctl (shmids[i], IPC_RMID, 0);
	/* gone */
}
	

int	register_shmd (tid, shmid)
int tid;
int shmid;	/* id of first shm seg that has all syshrd info in it */
{
    int cc;
    int sbuf,ssbuf;
	char fullname[1024];
	char hname[MAXHOSTNAMELEN];


	bzero (fullname, 1024);
	gethostname(hname, MAXHOSTNAMELEN);
	strcat (fullname, "pvm_shmd:");
	strcat (fullname, hname);

	printf("Registering process [0x%x] as [%s]\n", tid, fullname);
	if (shmid>0) 
		printf("%s storing syshdr as shmid [0x%x]\n", fullname, shmid);
	else {
		printf("%s marking shmem as disabled\n", fullname, shmid);
		 shmid = -1;
		 }


    ssbuf = pvm_setsbuf(0);
    sbuf = pvm_initsend(PvmDataDefault);
    pvm_pkint(&tid,1,1);
    pvm_pkint(&shmid,1,1);

    if ( (cc = pvm_putinfo(fullname, sbuf, PvmMboxDefault)) != 0 )
    {
        pvm_perror("pvm_shmd already running?");
    }

    pvm_setsbuf(ssbuf);

    pvm_freebuf(sbuf);

    return(cc); /* Will be PvmOK if register succeeded, < 0 otherwise */
}

int	make_mqueue (tid, mqkey, mqid) 
int tid;
key_t* mqkey;
int* mqid;
{
key_t key;
int  id;
int i,j,k;

	key = (key_t)    (0x2c000000+((int)getuid()*0x1000)+(0xFFFF&tid));   
			/* inital key */

	for(i=0;i<1000;i++) {
		id = msgget(key, IPC_CREAT|0666);
		if (id>0) break;
		else
			key++;
	}

	if (id<0) return (-1);	/* can't get one ! */

	/* else we can and we did! */
	printf("TID [0x%x] has a new message queue of key [0x%x] and id [%d]\n",
			tid, key, id);

	*mqkey = key;
	*mqid  = id;

	return (0);
}


int	del_mqueue (tid) 
int tid;
{
key_t key;
int  id;
int i,j,k;

	/* first get the id from the tidinfo structure using the processes tid */
	
	id = find_tidinfo_id (((char *)segptrs[0]), tid);

	if (id<0) {
		fprintf (stderr, "No TID [0x%x] found when removing mqueue ??\n",
				tid);
		fflush(stderr);
		return (-1); /* i.e. no tid record... ? */
	}
	
	i = msgctl (id, IPC_RMID, (struct msqid_ds *) 0 );

	if (!i) {
		printf("TID [0x%x] had a message queue of id [%d] deleted.\n",
				tid, id);
		return (0);
	}
	else {
		printf("Failed to remove TID [0x%x] message queue [%d]\n",
			tid, id);
		return (-1);
	}

}

void huphuphup ()
{
fprintf(stderr,"pvm_shmd: Received a sig HUP.\nExiting.\n");
fflush(stderr);
clean_segs();
pvmendtask();
exit (SIGHUP); 
}

void bambismother ()
{
fprintf(stderr,"pvm_shmd: Received a sig TERM.\nExiting.\n");
fflush(stderr);
clean_segs();
pvmendtask();
/* pvm_exit(); */
/* this causes a PVM not implemented error to appear */
exit (SIGTERM); 
}



static char rcsid[] =
	"$Id: sendrecv.c,v 1.4 1999/03/12 20:22:19 pvmsrc Exp $";

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
/* #include <sys/processor.h> */
/* #include <sys/procset.h> */
#include <errno.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <string.h>
#include <unistd.h>

#include "shmd.h"

#include <pvm3.h>

#ifndef SHM_FAILED
#define SHM_FAILED (void*)-1
#endif

/* Globals */
/* used to make this library easier and faster to code as always */

int	 shm_enabled=0;
/* if 0 not attempted yet */
/* if 1 ok */
/* if -1 not ready or unavailable as of yet */

int  mytid=0;	/* so we don't get our own tid every single call */
int  localid=0; /* a shorten version used by the block routines */
int  mymqueue=0;/* my own message queue */

/* this lot is filled in by shm-attach() call. */
/* its mostly read-only info  that is only changed by the pvm_shmd. */

shm_header_info_t* syshdr;		/* pointer to the start of all this mess */
int	 psize;		/* page/block size */
int  nsegs;		/* number of segments to choose from */
int	 maxpages;	/* maximum number of pages in a segment.. */
blockinfo_t* 	blocks[MAXSEGS];	/* pointers to the block info stuff... yeah */
char* segs[MAXSEGS];				/* pointers to actual segments */
char* msgblocks[MAXSEGS];			/* pointers to the actual shm blocks */
									/* fully qualified addresses used */
									/* so you can access directly */



int shmd_pvm_psend(tid, tag, cp, len, dt)

int tid;
int tag;
void *cp;
int len;
int dt;
{

	unsigned pages;
	int i, j, k;
	int remoteid=0;
	int olen;
	msg_info_t msg_info;
	int cc=0;
	int mqid;	/* message queue of the destination */
	int seg;
	int block, c;

/* 	printf("shmd_pvm_psend()\n"); */
#ifdef NAMES
	printf("shmd_pvm_psend(0x%x,%d,%x,%d,%d)\n",
			tid, tag, cp, len, dt);
#endif
#ifdef DEBUG
	printf("shm enabled = %d\n", shm_enabled);
#endif /* DEBUG */

	/* if no shared memory allowed then use pvm_psend() */
	if (shm_enabled<0) return (pvm_psend(tid, tag, cp, len, dt));

#ifdef DEBUG
	printf("shm enabled = %d\n", shm_enabled);
#endif /* DEBUG */
	
	/* Else check to see if we have ever used shared memory */
	/* if not can we ? */

	if (!shm_enabled) {
		shm_attach();		/* get it while its hot or not */
		if (shm_enabled<0) return (pvm_psend(tid, tag, cp, len, dt));
	}	

	/* remote or local */
	if ((mytid&0xFFFF0000)!=(tid&0xFFFF0000)) /* remote then... */
				return (pvm_psend(tid, tag, cp, len, dt));


	/* ok, we are here  because we might be able to... */

	/* ok, is the destination a valid target yet? */
	/* i.e. does it have a valid message queue? */

	mqid = find_tidinfo_id (syshdr, tid);

	if (mqid<0) {
		fprintf(stderr,"TID [0x%x] cannot send to TID [0x%x] as have not found its mqueue yet.\n", mytid, tid); fflush(stderr);
		return (pvm_psend(tid, tag, cp, len, dt));	/* nope */ 
	}

	/* ok, we think we can.. you, me, we both shared memory enabled */
	/* from out of the woods into the wood burner (tm) */


	olen = len;	/* backup of length just in case ;) */

    switch (dt) {

    case PVM_BYTE:
        len *= sizeof(char);
        break;

    case PVM_SHORT:
    case PVM_USHORT:
        len *= sizeof(short);
        break;

    case PVM_INT:
    case PVM_UINT:
        len *= sizeof(int);
        break;

    case PVM_LONG:
    case PVM_ULONG:
        len *= sizeof(long);
        break;

    case PVM_FLOAT:
        len *= sizeof(float);
        break;

    case PVM_CPLX:
        len *= sizeof(float) * 2;
        break;

    case PVM_DOUBLE:
        len *= sizeof(double);
        break;

    case PVM_DCPLX:
        len *= sizeof(double) * 2;
        break;

    case PVM_STR:
        cc = PvmNotImpl;
        break;

    default:
        cc = PvmBadParam;
        break;
    }

	if (cc<0) return (cc); /* data type error */

	/* ok len is now the length of the message in bytes */

	/* short cut here for zero length messages... */

	if (len) {
	/* we now need to know how many pages that is */

	pages = (len / psize);
	if (len % psize) pages++;

#ifdef DEBUG
	fprintf(stderr,"send: len [%d] bytes or [%d] whole pages\n",
			len, pages);
	fflush(stderr);

		fprintf(stderr,"send: pages requested [%d] max in any segment [%d]\n",
			pages, maxpages);
		fflush(stderr);
#endif /* DEBUG */
		
	/* now find a block with that kind of memory available */
	if (pages>maxpages) return (pvm_psend(tid, tag, cp, olen, dt));  /* nope */

	/* ok now really look! */
	remoteid = (tid & 0x0000FFFF);	/* shorten TIDs */

	seg = (remoteid*5) % nsegs;	/* hash into a set segment */
	block = -1;

#ifdef DEBUG
	fprintf(stderr,"Hash localid&[0x%x]*5 %% nsegs [%d] -> [%d]\n",	
		remoteid, nsegs, seg);
	fflush(stderr);
#endif /* DEBUG */

	/* loop round until we find one! */
	for(c=0;c<nsegs;c++) {
#ifdef DEBUG
	fprintf(stderr,"seg[%d] largest block has [%d] pages. Need [%d]\n",
		seg, blocks[seg]->largestblockfree, pages);
	fflush(stderr);
#endif /* DEBUG */
		if (blocks[seg]->largestblockfree >= pages) { /* maybe in luck */
/* 			block = getblocks (blocks[seg], localid, pages); */
			/* try remote id for speed see writen notes */
			block = getblocks (blocks[seg], remoteid, pages);
#ifdef DEBUG
	fprintf(stderr,"Blocks found at [0x%x] [%d] when looking for [%d]\n",
		blocks[seg], block, pages);
	fflush(stderr);
#endif /* DEBUG */

			if (block>=0) break;	/* happy we have the memory */
		} /* there might be space */	
		seg = (seg+1) % nsegs;	/* wrap round if required */
	}
	
	if (block<0) /* no space in at the Inn... */	{	
		fprintf(stderr, "psend() from [0x%x] to [0x%x] of len [%d] bytes out of message buffers?\n", mytid, tid, olen);
		for(j=0;j<nsegs;j++) fprintf(stderr, "pages free [%d]\t", blocks[j]->pagesfree);
		fprintf(stderr, "\nUsing pvm_psend() sockets instead.\n");
		return (pvm_psend(tid, tag, cp, olen, dt));	
				/* glad we copied len into olen.... */
	} 		

	/* ok, we have a reserved block big enought in seg 'seg' at the */
	/* start of block 'block' */

	/* so lets copy data into it and then mqueue it out of here */
#ifdef DEBUG
	printf("Copy into [%x] from [%x] [%d] bytes\n",
		(char*)msgblocks[seg]+(psize*block),
		(char *)cp, len);
#endif

	memcpy ((char*)msgblocks[seg]+(psize*block), (char *)cp, len);


	} /* if ( len != 0 ) */
	else {
		seg = -1; block = -1; pages = 0; 
	}

	/* dirty dead has been done, now to kick the receiver */

	
	/* First make the message */
	msg_info.mtype = (long) tag;
	msg_info.from  = mytid;
    msg_info.seg   = seg;
    msg_info.block = block;
	msg_info.pages = pages;
    msg_info.bytes = len;

	/* now to shift the data out of here! */

	cc = msgsnd (mqid, (void *)&msg_info, (sizeof(msg_info)), 0);
#ifdef DEBUG
	printf("TID [0x%x] sent message via shared memory seg [%d] block [%d] that was [%d]bytes or [%d] pages long to tid [0x%x] signalling on mqueue [%d]=[%d]\n",
			mytid, seg, block, len, pages, tid, mqid, cc);
	fflush(stdout);
#endif /* DEBUG */

/* 	if (cc<0) {  */
	/* i.e. the message was not sent! */
	/* to be done, and its messy... */


	return (cc);		
}


int shmd_pvm_precv(tid, tag, cp, len, dt, rtid, rtag, rlen)

int tid;
int tag;
void *cp;
int len;
int dt;
int *rtid;
int *rtag;
int *rlen;
{

	unsigned pages;
	int i, j, k;
	int remoteid=0;
	int olen;
	msg_info_t msg_info;
	int cc=0;
	unsigned int stime = 1;	/* initial usleep time */
	int found=0;
	int alen;
	int c;

/* 	printf("shmd_pvm_precv()\n"); */
#ifdef NAMES
	printf("shmd_pvm_precv(%x, %d, %x, %d, %d, %x, %x, %x)\n",
			tid, tag, cp, len, dt, rtid, rtag, rlen);
#endif
	/* if no shared memory allowed then use pvm_precv() */
	if (shm_enabled<0) 
		return (pvm_precv(tid, tag, cp, len, dt, rtid, rtag, rlen));
	
	
	/* Else check to see if we have ever used shared memory */
	/* if not can we ? */

	if (!shm_enabled) {
		shm_attach();		/* get it while its hot or not */
		if (shm_enabled<0) 
	        return (pvm_precv(tid, tag, cp, len, dt, rtid, rtag, rlen));
	}	

	/* remote or local */
	if ((mytid&0xFFFF0000)!=(tid&0xFFFF0000)) { /* remote then... */
			fprintf(stderr,"Me [0x%x] and src [0x%x] not on same machine\n",
				mytid, tid); fflush(stderr);
			return (pvm_precv(tid, tag, cp, len, dt, rtid, rtag, rlen));
	}

	/* ok, we are here  because we might be able to... */

	/* does the sender have shared memory?  */
	/* actually it might have sent and then died why we were busy */
	/* so we do not check whether it has a message queue (left) itself */
	/* 	mqid = find_tidinfo_id (seg0, tid); */

	/* ok, we think we can.. you, me, we both shared memory enabled */
	/* from out of the woods into the wood burner (tm) */


	olen = len;	/* backup of length just in case ;) */

    switch (dt) {

    case PVM_BYTE:
        len *= sizeof(char);
        break;

    case PVM_SHORT:
    case PVM_USHORT:
        len *= sizeof(short);
        break;

    case PVM_INT:
    case PVM_UINT:
        len *= sizeof(int);
        break;

    case PVM_LONG:
    case PVM_ULONG:
        len *= sizeof(long);
        break;

    case PVM_FLOAT:
        len *= sizeof(float);
        break;

    case PVM_CPLX:
        len *= sizeof(float) * 2;
        break;

    case PVM_DOUBLE:
        len *= sizeof(double);
        break;

    case PVM_DCPLX:
        len *= sizeof(double) * 2;
        break;

    case PVM_STR:
        cc = PvmNotImpl;
        break;

    default:
        cc = PvmBadParam;
        break;
    }

	if (cc<0) return (cc);	/* i.e. invalid data type param */

	/* ok len is now the length of the message in bytes */
	/* we now need to know how many pages that is */

	pages = (len / psize);
	if (len % psize) pages++;

#ifdef DEBUG
	fprintf(stderr,"recv: len [%d] bytes or [%d] whole pages\n",
			len, pages);
	fflush(stderr);

		fprintf(stderr,"recv: pages requested [%d] max in any segment [%d]\n",
			pages, maxpages);
		fflush(stderr);
#endif /* DEBUG */
		
	/* paranoid and safe */
	if (pages>maxpages) {
			return (pvm_precv(tid, tag, cp, olen, dt, rtid, rtag, rlen));
	}


	/* ok, only now can we change the tag as mqueue and pvm use different */
	/* ones for a wildcard */
	if (tag==-1) tag = 0;

	found = 0;
	stime =1;
	c = 0;	/* tight loop counter */
	while (!found) {	
		cc = msgrcv (mymqueue, 
		(void *) &msg_info, 
		sizeof(msg_info), 
		(long) tag, 
		0);
/* 		IPC_NOWAIT); */
		if (cc>0) { /* success */
#ifdef DEBUG
			printf("got one from the message cue. Sleep was [%d] on count[%d].\nAsked for src[0x%x] tag[%d] size[%d]bytes\nGot from [0x%x] of tag[%d] size[%d]bytes, pages[%d]\n",
			 stime, c, 
			 tid, tag, len, 
			 msg_info.from, msg_info.mtype, msg_info.bytes, msg_info.pages);
#endif /* DEBUG */
			break; /* out to main routine */
		}

		if (cc==-1) { /* failure */
			if (errno == EIDRM) { /* id is now invalid */
				shm_enabled = -1; 
				mymqueue = -1;
				return (pvm_precv(tid, tag, cp, olen, dt, rtid, rtag, rlen));
			}
/* 			if (errno == ENOMSG) 	 */
		}

		/* tight loop counter c is checked */
		if (c>100) {
		   /* ok, we now probe for a real pvm message and the iterate */ 
		   cc = pvm_probe (tid, tag);
		   if (cc>0) 
				return (pvm_precv(tid, tag, cp, olen, dt, rtid, rtag, rlen)); 
		   }	
		
		/* ok, neither message avaliable under either */
		/* so, sleep and increase the sleep amount each time */

		if (c>200) {
		   usleep (stime);
		   if (stime<500) 
			   stime *= 2;		/* exp increase... */
		}
		c++;	/* tight loop counter */
	}	/* end while not found */

	/* so lets get a copy of the data out of here */
	/* first agree on the data size */
	/* i.e. the smallest of the two.. */

	alen = len;
	if (msg_info.bytes < len) alen = msg_info.bytes;

	if (alen) {

#ifdef DEBUG
	printf("Copy into [%x] from [%x] [%d] bytes\n",
		(char *)cp,
		(char*)(msgblocks[msg_info.seg]+(psize*msg_info.block)),
		alen);
#endif

		memcpy ((char *)cp, 
				(char*)(msgblocks[msg_info.seg]+(psize*msg_info.block)), 
				alen);

		/* dirty dead has been done, now to clean the block info up. */

#ifdef DEBUG
		printf("recv: [0x%x] cleaning up message block from [0x%x] in Seg[%d] start block [%d] of [%d] pages\n",
				mytid, msg_info.from, msg_info.seg, msg_info.block, msg_info.pages);
#endif /* DEBUG */
		freeblock (blocks[msg_info.seg], localid, msg_info.pages, msg_info.block);
	}

/* 	if (rlen) *rlen = (int) alen; */
/* 	if (rtid) *rtid = (int) msg_info.from; */
/* 	if (rtag) *rtag = (int) msg_info.mtype; */
					/* must cast or else long will over write other args */
	
	return (PvmOk);		
}



int shm_attach() {
	int i,id;
	key_t	key, k1, k2;
	int id1, id2;
	struct shmid_ds shmsbuf;
	unsigned size;
	void* 	ptr;
	int*	iptr;
	int 	toview;
	int		last;
	int		shmdtid;
	int 	srbuf, bufid;
	blockinfo_t*	baseptr;
	char*	cptr;
	shm_header_info_t* hptr;
	int		numsegs=0;
	char	hname[MAXHOSTNAMELEN];
	char	shmdname[1024];
	int isitok=0; /* ok or not ? */

	/* make sure we have our tid so we can decide if this local or remote */
	if (!mytid) {
		mytid = pvm_mytid(); 
		localid = (mytid & 0x0000FFFF);	/* shorten TIDs */
		if (mytid<0) /* ops no pvm... */
			return (-1);
	}

	bzero (shmdname, 1024);
	gethostname(hname, MAXHOSTNAMELEN);
	strcat (shmdname, "pvm_shmd:");
	strcat (shmdname, hname);


	srbuf = pvm_setrbuf(0);
	if ( pvm_recvinfo(shmdname, 0, PvmMboxDefault) == PvmNotFound ) {
		fprintf(stderr,"No pvm_shmd\n");	
		exit (-1);
	}	
	pvm_upkint(&shmdtid,1,1);
	pvm_upkint(&id,1,1);	
	pvm_setrbuf(srbuf);


	pvm_initsend(PvmDataDefault);
	pvm_pkint(&mytid,1,1);
	pvm_send (shmdtid, 1);			/* join the shared memory system */

#ifdef DEBUGSTART
	fprintf(stderr,"Joining the system\n"); fflush(stderr);

	fprintf(stderr,"Recv on [0x%x] and tag [0x%x]\n", shmdtid, mytid);
	fflush(stderr);
#endif /* DEBUGSTART */

	bufid = pvm_recv (shmdtid, mytid);	/* i.e. my message no questions */
#ifdef DEBUGSTART
	fprintf(stderr,"Got message back?\n"); fflush(stderr);
#endif /* DEBUGSTART */

	pvm_upkint (&isitok, 1, 1);			/* shm says yeah or nay ? */
#ifdef DEBUGSTART
	fprintf(stderr,"Value [%d]\n", isitok); fflush(stderr);
#endif /* DEBUGSTART */

	/* we do this handshake so that we don't have to spin */
	/* on our own info mqueue appearing in the shm header segment */
	/* once we get this message we know its there ok! */

	if (isitok<0) {
			fprintf(stderr,"pvm_shmd said no shm for [0x%x] ?", mytid);
			shm_enabled = -1;
			return (-1);
	}

      	
	ptr = shmat (id, 0, 0);			/* inital shmat attempt */
	
/* 	if (ptr>0) { */
	if (ptr != SHM_FAILED) {
#ifdef DEBUGSTART
		printf("attached at offset of 0x%lx\n", (long) ptr );
#endif /* DEBUGSTART */
		}
	else {
		perror("shmat:");
		shm_enabled = -1;
		return (-1);
	}

	segs[0] = (char *) ptr;				/* first segment pointer */
	syshdr = (shm_header_info_t *) ptr;	/* system header pointer */
	psize  = getpagesize();				/* ok a system call really */
	nsegs  = syshdr->numsegs;			/* number of segments */

#ifdef DEBUGSTART
	printf("Got syshdr [0x%lx] pagesize [%d] nsegs [%d]\n",
		(long) syshdr, psize, nsegs);
#endif /* DEBUGSTART */

	cptr = (char *) ptr;
/* 	cptr += sizeof (shm_header_info_t);	 */
		/* gets us to the blockinfo */
	/* actually this was a mistake, and a bug of my own making!!! */
	/* the sysheader only tasks around 200 bytes.. the rest is */
	/* the tidlist proper! */
	/* real start of blockinfo is 16KBytes lower.... */
	cptr += (16384);

	baseptr = (blockinfo_t*)cptr;		/* cast and assign */
	
	for(i=0;i<nsegs;i++) { 	/* for each segement (set of blocks) */	
		blocks[i] = baseptr;
#ifdef DEBUGSTART
		printf("Seg[%d] key [0x%x] id [%d] blockinfo [0x%lx]\n", i, 
				blocks[i]->segkey, blocks[i]->segid, (long) blocks[i]);
#endif /* DEBUGSTART */
		if (i) {	
		  /* we have already attached the first segment just the rest to go */
		   segs[i] = shmat (baseptr->segid, 0, 0);
/* 		   if (segs[i]<0) { */
		   if (segs[i]==SHM_FAILED) {
			   perror("shmat:");
			   shm_enabled = -1;
			   return (-1);
		   	}
#ifdef DEBUGSTART
		  printf("Seg[%d] attached at [0x%lx]\n", i, (long) segs[i]);
#endif /* DEBUGSTART */
		}
		msgblocks[i] = segs[i]+(baseptr->segoffset);
		cptr = (char *)blocks[i] + blocks[i]->nextoffset;
		baseptr = (blockinfo_t *) cptr; 
					/* point to the next blockinfo_t struct */ 	
	}

#ifdef DEBUGSTART
	printf("Number of segments is [%d]\n", i);
	fflush(stdout);
#endif /* DEBUGSTART */

	/* Now to find the maximum pages allowed for any single message.. */
	/* this should change when I allow messages across segments. */

#ifdef DEBUGSTART
	for(i=0;i<nsegs;i++)
		printf("[%d] has [%d] pages of memory starting at [0x%lx]. Whole pages starts at [0x%lx]\n",
			i, blocks[i]->npages, (long) msgblocks[i], (long) segs[i]);
#endif /* DEBUGSTART */

	maxpages = blocks[0]->npages;	
	for(i=1;i<nsegs;i++)
		if (blocks[i]->npages > maxpages) maxpages = blocks[i]->npages;

	
	/* now to find my own message queue and we are away */
	mymqueue = find_tidinfo_id (syshdr, mytid);		
	if (mymqueue < 0) {
		shm_enabled = -1;
		return (-1);
	}

#ifdef DEBUGSTART
	printf("mqueue: TID [0x%x] queue id [%d]\n", mytid, mymqueue);
#endif /* DEBUGSTART */


	/* if (processor_bind(P_PID, P_MYID, 0, NULL) == -1)
		perror("processor_bind"); */


	/* no further objections my honour */

	shm_enabled = 1;	/* i.e. all is ok and ready for SHM action */
	return (0);
}


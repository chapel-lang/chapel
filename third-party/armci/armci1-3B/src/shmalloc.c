/* $Id$ */
#include <stdio.h>
#include "shmalloc.h"

/* Storage allocator basically copied from ANSI K&R and corrupted */

extern char *armci_allocate(); /* Used to get memory from the system */
extern void armci_die();

#define DEFAULT_NALLOC 8192	/* No. of units of length ALIGNMENT to get in
				   every request to the system for memory */

#define DEFAULT_MAX_NALLOC 786432 /* Maximum  no. of units that can get */

static size_t nalloc = DEFAULT_NALLOC;
static size_t max_nalloc = DEFAULT_MAX_NALLOC;

/* mutual exclusion defs go here */
#define  LOCKIT 
#define  UNLOCKIT 

static int do_verify = 0;	/* Flag for automatic heap verification */

#define VALID1  0xaaaaaaaa	/* For validity check on headers */
#define VALID2  0x55555555

static struct shmalloc_struct {
  size_t total;			/* Amount request from system in units */
  long nchunk;			/* No. of chunks of system memory */
  long inuse;			/* Amount in use in units */
  long maxuse;			/* Maximum value of inuse */
  long nfrags;			/* No. of fragments divided into */
  long nmcalls;			/* No. of calls to shmalloc */
  long nfcalls;			/* No. of calls to shfree */
} usage;

#ifdef CRAY
#define LOG_ALIGN 6
#elif defined(KSR)
#define LOG_ALIGN 7
#else
#define LOG_ALIGN 6
#endif

#define ALIGNMENT (1 << LOG_ALIGN)

union header{
  struct {
    unsigned valid1;		/* Token to check if is not overwritten */
    union header *ptr;		/* next block if on free list */
    size_t size;		/* size of this block*/
    unsigned valid2;		/* Another token acting as a guard */
  } s;
  char align[ALIGNMENT];	/* Align to ALIGNMENT byte boundary */
};

typedef union header Header;

static Header base;		/* empty list to get started */
static Header *freep = NULL;	/* start of free list */
static Header *usedp = NULL;	/* start of used list */

static void sherror(s, i)
     char *s;
     unsigned long i;
{
  fflush(stdout);
  fprintf(stderr,"shmalloc error: %s %ld(0x%lx)\n", s, i, i);
  fflush(stderr);
  shmalloc_print_stats();
  armci_die("shmalloc: fatal error", i);
}

void shmalloc_request(size, maxsize)
     size_t size, maxsize;
{
#define UNIT 1024
int scale;
/*
  nalloc = (size+ALIGNMENT-1) >> LOG_ALIGN;
  max_nalloc = (maxsize+ALIGNMENT-1) >> LOG_ALIGN;
 */
 scale = UNIT>>LOG_ALIGN;
 if(scale<1)fprintf(stderr,"Error: shmalloc_request !!!\n");
 nalloc     = size * scale;
 max_nalloc = maxsize * scale;

}

void shmalloc_debug(code)
     int code;
{
  do_verify = code;
}


static Header *morecore(nu)
     size_t nu;
{
  char *cp;
  Header *up;
  void armci_addtofree();
 
  if (usage.total >= max_nalloc)
    return (Header *) NULL;   /* Enforce upper limit on core usage */

  nu = nalloc*((nu-1)/nalloc+1); /* nu must by a multiplicity of nalloc */
  /*if (nu < nalloc) nu = nalloc;*/ /* Minimum size for getting memory */

#if DEBUG
  (void) printf("morecore: Getting %ld more units of length %d\n",
                (long)nu, sizeof(Header));
  (void) fflush(stdout);
#endif

  if ((cp = armci_allocate((size_t)nu * sizeof(Header))) == (char *) NULL)
    return (Header *) NULL;
 
  usage.total += nu;   /* Have just got nu more units */
  usage.nchunk++;      /* One more chunk */
  usage.nfrags++;      /* Currently one more frag */
  usage.inuse += nu;   /* Inuse will be decremented by shfree */

  up = (Header *) cp;
  up->s.size = nu;
  up->s.valid1 = VALID1;
  up->s.valid2 = VALID2;

  /* Insert into linked list of blocks in use so that shfree works
     ... for debug only */
  up->s.ptr = usedp;
  usedp = up;

  armci_addtofree((char *)(up+1));  /* Try to join into the free list */
  return freep;
}

char *armci_shmalloc(nbytes)
     size_t nbytes;
{
  Header *p, *prevp;
  size_t nunits;
  char *return_ptr;

  LOCKIT;

  /* If first time in need to initialize the free list */ 

  if ((prevp = freep) == NULL) { 

    if (sizeof(Header) != ALIGNMENT)
      sherror("Alignment is not valid", (unsigned long) ALIGNMENT);
    
    usage.total  = 0;  /* Initialize statistics */
    usage.nchunk = 0;
    usage.inuse  = 0;
    usage.nfrags = 0;
    usage.maxuse = 0;
    usage.nmcalls= 0;
    usage.nfcalls= 0;

    base.s.ptr = freep = prevp = &base;  /* Initialize linked list */
    base.s.size = 0;
    base.s.valid1 = VALID1;
    base.s.valid2 = VALID2;
  }

  usage.nmcalls++;
  
  if (do_verify)
    shmalloc_verify();
  
  /* Rather than divide make the alignment a known power of 2 */
  
  nunits = ((nbytes + sizeof(Header) - 1)>>LOG_ALIGN) + 1;

  for (p=prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
    if (p->s.size >= nunits) {	/* Big enuf */
      if (p->s.size == nunits)	/* exact fit */
        prevp->s.ptr = p->s.ptr;
      else {			/* allocate tail end */
	p->s.size -= nunits;
	p += p->s.size;
	p->s.size = nunits;
	p->s.valid1 = VALID1;
	p->s.valid2 = VALID2;
	usage.nfrags++;  /* Have just increased the fragmentation */
      }

      /* Insert into linked list of blocks in use ... for debug only */
      p->s.ptr = usedp;
      usedp = p;

      usage.inuse += nunits;  /* Record usage */
      if (usage.inuse > usage.maxuse)
	usage.maxuse = usage.inuse;
      freep = prevp;
      return_ptr = (char *) (p+1);
      break;
    }
    
    if (p == freep)		/* wrapped around the free list */
      if ((p = morecore(nunits)) == (Header *) NULL) {
        return_ptr = (char *) NULL;
        break;
      }
  }

  UNLOCKIT;

  return return_ptr;

}


void shfree(ap)
     char *ap;
{
  Header *bp, *p, **up;

  LOCKIT;

  usage.nfcalls++;


  if (do_verify)
    shmalloc_verify();

 /* only do something if pointer is not NULL */

  if ( ap ) {

  bp = (Header *) ap - 1;  /* Point to block header */

  if (bp->s.valid1 != VALID1 || bp->s.valid2 != VALID2)
    sherror("shfree: pointer not from shmalloc", (unsigned long) ap);
  
  usage.inuse -= bp->s.size; /* Decrement memory usage */

  /* Extract the block from the used linked list
     ... for debug only */

  for (up=&usedp; ; up = &((*up)->s.ptr)) {
    if (!*up)
      sherror("shfree: block not found in used list\n", (unsigned long) ap);
    if (*up == bp) {
      *up = bp->s.ptr;
      break;
    }
  }

  /* Join the memory back into the free linked list */

  for (p=freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break; /* Freed block at start or end of arena */

  if (bp + bp->s.size == p->s.ptr) {/* join to upper neighbour */
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
    usage.nfrags--;                 /* Lost a fragment */
  } else
    bp->s.ptr = p->s.ptr;

  if (p + p->s.size == bp) { /* Join to lower neighbour */
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
    usage.nfrags--;          /* Lost a fragment */
  } else
    p->s.ptr = bp;

  freep = p;

} /* end if on ap */

  UNLOCKIT;
}

void shmalloc_stats(total, nchunk, inuse, maxuse, nfrags, 
		    nmcalls, nfcalls)
     size_t *total, *inuse, *maxuse;
     long *nchunk, *nfrags, *nmcalls, *nfcalls;
/*
  Return stats on shmalloc performance. Use arg list instead of
  returning structure so that FORTRAN can eventually use it
*/
{
  *total  = usage.total * sizeof(Header);
  *nchunk = usage.nchunk;
  *inuse  = (size_t)usage.inuse * sizeof(Header);
  *maxuse = (size_t)usage.maxuse* sizeof(Header);
  *nfrags = usage.nfrags;
  *nmcalls= usage.nmcalls;
  *nfcalls= usage.nfcalls;
}

void shmalloc_print_stats()
/*
  Print to standard output the usage statistics.
*/
{
  size_t total, inuse, maxuse;
  long nchunk, nfrags, nmcalls, nfcalls;

  shmalloc_stats(&total, &nchunk, &inuse, &maxuse, &nfrags,
		 &nmcalls, &nfcalls);

  fflush(stderr);
  printf("\nshmalloc statistics\n-------------------\n\n");

  printf("Total memory from system ... %ld bytes\n", (long)total);
  printf("Current memory usage ....... %ld bytes\n", (long)inuse);
  printf("Maximum memory usage ....... %ld bytes\n", (long)maxuse);
  printf("No. chunks from system ..... %ld\n", nchunk);
  printf("No. of fragments ........... %ld\n", nfrags);
  printf("No. of calls to shmalloc ... %ld\n", nmcalls);
  printf("No. of calls to shfree ..... %ld\n", nfcalls);
  printf("\n");

  fflush(stdout);
}

void shmalloc_verify()
/*
  Currently assumes that are working in a single region.
*/
{
  Header *p;

  LOCKIT;

  if ( freep ) {

  /* Check the used list */

  for (p=usedp; p; p=p->s.ptr) {
    if (p->s.valid1 != VALID1 || p->s.valid2 != VALID2)
      sherror("invalid header on usedlist", (unsigned long) p->s.valid1);

    if (p->s.size > usage.total)
      sherror("invalid size in header on usedlist", (unsigned long) p->s.size);
  }

  /* Check the free list */

  p = base.s.ptr;
  while (p != &base) {
    if (p->s.valid1 != VALID1 || p->s.valid2 != VALID2)
      sherror("invalid header on freelist", (unsigned long) p->s.valid1);

    if (p->s.size > usage.total)
      sherror("invalid size in header on freelist", (unsigned long) p->s.size);

    p = p->s.ptr;
  }
  } /* end if */

  UNLOCKIT;
}

void armci_addtofree(ap)
     char *ap;
{
  Header *bp, *p, **up;

  usage.nfcalls++;


  if (do_verify)
    shmalloc_verify();

 /* only do something if pointer is not NULL */

  if ( ap ) {

  bp = (Header *) ap - 1;  /* Point to block header */

  if (bp->s.valid1 != VALID1 || bp->s.valid2 != VALID2)
    sherror("shfree: pointer not from shmalloc", (unsigned long) ap);
  
  usage.inuse -= bp->s.size; /* Decrement memory usage */

  /* Extract the block from the used linked list
     ... for debug only */

  for (up=&usedp; ; up = &((*up)->s.ptr)) {
    if (!*up)
      sherror("shfree: block not found in used list\n", (unsigned long) ap);
    if (*up == bp) {
      *up = bp->s.ptr;
      break;
    }
  }

  /* Join the memory back into the free linked list */

  for (p=freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break; /* Freed block at start or end of arena */

  if (bp + bp->s.size == p->s.ptr) {/* join to upper neighbour */
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
    usage.nfrags--;                 /* Lost a fragment */
  } else
    bp->s.ptr = p->s.ptr;

  if (p + p->s.size == bp) { /* Join to lower neighbour */
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
    usage.nfrags--;          /* Lost a fragment */
  } else
    p->s.ptr = bp;

  freep = p;

} /* end if on ap */
}

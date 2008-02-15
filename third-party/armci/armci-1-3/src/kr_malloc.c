/* $Id: kr_malloc.c,v 1.22.2.2 2006/09/13 17:58:02 andriy Exp $ */
#include <stdio.h>
#include "kr_malloc.h"
#include "armcip.h" /* for DEBUG purpose only. remove later */
#include "locks.h"

#define DEBUG 0

/* Storage allocator basically copied from ANSI K&R and corrupted */

extern char *armci_allocate(); /* Used to get memory from the system */
extern void armci_die();
static char *kr_malloc_shmem(size_t nbytes, context_t *ctx);
static void kr_free_shmem(char *ap, context_t *ctx);

/**
 * DEFAULT_NALLOC: No. of units of length ALIGNMENT to get in every 
 * request to the system for memory (8MB/64 => 128*1024units)
 * DEFAULT_MAX_NALLOC: Maximum number of units that can get i.e.1GB 
 * (if unit size=64bytes, then max units=1024MB/64 = 16*1024*1024)
 */
#define DEFAULT_NALLOC       (128*1024)  
#define DEFAULT_NALLOC_ALIGN 1024  
#define DEFAULT_MAX_NALLOC   (1024*1024*16) 

/* mutual exclusion defs go here */
#define LOCKED   100
#define UNLOCKED 101
static int lock_mode=UNLOCKED;

/* enable locking only after armci is initailized as locks (and lock
   data structures) are initialized in ARMCI_Init */
#define  LOCKIT(p) \
    if(_armci_initialized && lock_mode==UNLOCKED) { \
       NAT_LOCK(0,p); lock_mode=LOCKED;             \
    }
#define  UNLOCKIT(p) \
    if(_armci_initialized && lock_mode==LOCKED) {   \
       NAT_UNLOCK(0,p); lock_mode=UNLOCKED;         \
    }

static int do_verify = 0;	/* Flag for automatic heap verification */

#define VALID1  0xaaaaaaaa	/* For validity check on headers */
#define VALID2  0x55555555

#define USEDP 0 /* CHECK. By default anable this. */

static void kr_error(char *s, unsigned long i, context_t *ctx) {
char string[256];
    sprintf(string,"kr_malloc: %s %ld(0x%lx)\n", s, i, i);
#if 0
    kr_malloc_print_stats(ctx);
#endif
    armci_die(string, i);
}

static Header *morecore(size_t nu, context_t *ctx) {
    char *cp;
    Header *up;

#if DEBUG
    (void) printf("%d: morecore 1: Getting %ld more units of length %d nalloc=%d\n", armci_me, (long)nu, sizeof(Header),ctx->nalloc);
    (void) fflush(stdout);
#endif

    if (ctx->total >= ctx->max_nalloc) {
#      if DEBUG
         armci_die("kr_malloc: morecore: maximum allocation reached",armci_me);
#      endif
       return (Header *) NULL;   /* Enforce upper limit on core usage */
    }

#if 1
    /* 07/03 ctx->nalloc is now the minimum # units we ask from OS */
    nu = DEFAULT_NALLOC_ALIGN*((nu-1)/DEFAULT_NALLOC_ALIGN+1);
    if(nu < ctx->nalloc) nu = ctx->nalloc; 
#else
    nu = ctx->nalloc*((nu-1)/ctx->nalloc+1); /* nu must by a multiplicity of nalloc */
#endif

#if DEBUG
    (void) printf("%d: morecore: Getting %ld more units of length %d\n",
		  armci_me, (long)nu, sizeof(Header));
    (void) fflush(stdout);
#endif
    
    if ((cp =(char *)(*ctx->alloc_fptr)((size_t)nu * sizeof(Header))) == (char *)NULL)
      return (Header *) NULL;

    ctx->total += nu;   /* Have just got nu more units */
    ctx->nchunk++;      /* One more chunk */
    ctx->nfrags++;      /* Currently one more frag */
    ctx->inuse += nu;   /* Inuse will be decremented by kr_free */
    
    up = (Header *) cp;
    up->s.size = nu;
    up->s.valid1 = VALID1;
    up->s.valid2 = VALID2;

    /* Insert into linked list of blocks in use so that kr_free works
       ... for debug only */
    up->s.ptr = ctx->usedp;
    ctx->usedp = up;

    kr_free((char *)(up+1), ctx);  /* Try to join into the free list */
    return ctx->freep;
}

void kr_malloc_init(size_t usize, /* unit size in bytes */
		    size_t nalloc,
		    size_t max_nalloc,
		    void * (*alloc_fptr)(), /* memory alloc routine */
		    int debug,
		    context_t *ctx) {
    int scale;

    if(usize <= 0) usize = sizeof(Header);
    
    scale = usize>>LOG_ALIGN;
    if(scale<1)fprintf(stderr,"Error: kr_malloc_init !!!\n");
    
    if(nalloc==0) nalloc = DEFAULT_NALLOC;
    if(max_nalloc==0) max_nalloc = DEFAULT_MAX_NALLOC;

    ctx->usize      = sizeof(Header);
    ctx->nalloc     = nalloc * scale;
    ctx->max_nalloc = max_nalloc * scale;
    ctx->alloc_fptr = alloc_fptr;
    ctx->freep = NULL;
    ctx->usedp = NULL;
    ctx->shmid = -1;
    ctx->shmoffset = 0;
    ctx->shmsize   = 0;
    ctx->ctx_type  = -1;
    do_verify = debug;
}


char *kr_malloc(size_t nbytes, context_t *ctx) {
    Header *p, *prevp;
    size_t nunits;
    char *return_ptr;

#if !((defined(SUN) || defined(SOLARIS)) && !defined(SHMMAX_SEARCH_NO_FORK))
    if(ctx->ctx_type == KR_CTX_SHMEM) return kr_malloc_shmem(nbytes,ctx);
#endif
    
    /* If first time in need to initialize the free list */
    if ((prevp = ctx->freep) == NULL) {

       if (sizeof(Header) != ALIGNMENT)
	  kr_error("Alignment is not valid", (unsigned long) ALIGNMENT, ctx);

       ctx->total  = 0;  /* Initialize statistics */
       ctx->nchunk = 0;
       ctx->inuse  = 0;
       ctx->nfrags = 0;
       ctx->maxuse = 0;
       ctx->nmcalls= 0;
       ctx->nfcalls= 0;
       /* Initialize linked list */
       ctx->base.s.ptr = ctx->freep = prevp = &(ctx->base);
       ctx->base.s.size = 0;
       ctx->base.s.valid1 = VALID1;
       ctx->base.s.valid2 = VALID2;
    }

    ctx->nmcalls++;

    if (do_verify)
       kr_malloc_verify(ctx);

    /* Rather than divide make the alignment a known power of 2 */

    nunits = ((nbytes + sizeof(Header) - 1)>>LOG_ALIGN) + 1;

    for (p=prevp->s.ptr; ; prevp = p, p = p->s.ptr) {

       if (p->s.size >= nunits) {        /* Big enuf */
	  if (p->s.size == nunits)        /* exact fit */
	     prevp->s.ptr = p->s.ptr;
	  else {                  /* allocate tail end */
	     p->s.size -= nunits;
	     p += p->s.size;
	     p->s.size = nunits;
	     p->s.valid1 = VALID1;
	     p->s.valid2 = VALID2;
	     ctx->nfrags++;  /* Have just increased the fragmentation */
	  }

	  /* Insert into linked list of blocks in use ... for debug only */
	  p->s.ptr = ctx->usedp;
	  ctx->usedp = p;

	  ctx->inuse += nunits;  /* Record usage */
	  if (ctx->inuse > ctx->maxuse)
	     ctx->maxuse = ctx->inuse;
	  ctx->freep = prevp;
	  return_ptr = (char *) (p+1);
	  break;
       }

       if (p == ctx->freep) {       /* wrapped around the free list */
	  if ((p = morecore(nunits, ctx)) == (Header *) NULL) {
	     return_ptr = (char *) NULL;
	     break;
	  }
       }
    }

    return return_ptr;

}


void kr_free(char *ap, context_t *ctx) {
    Header *bp, *p, **up;
    
#if !((defined(SUN) || defined(SOLARIS)) && !defined(SHMMAX_SEARCH_NO_FORK))
    if(ctx->ctx_type == KR_CTX_SHMEM) { kr_free_shmem(ap,ctx); return; }
#endif
    
    ctx->nfcalls++;


    if (do_verify)
       kr_malloc_verify(ctx);

    /* only do something if pointer is not NULL */

    if ( ap ) {

       bp = (Header *) ap - 1;  /* Point to block header */

       if (bp->s.valid1 != VALID1 || bp->s.valid2 != VALID2)
	  kr_error("kr_free: pointer not from kr_malloc",
		   (unsigned long) ap, ctx);

       ctx->inuse -= bp->s.size; /* Decrement memory ctx->usage */

       /* Extract the block from the used linked list
	  ... for debug only */

       for (up=&(ctx->usedp); ; up = &((*up)->s.ptr)) {
	  if (!*up)
	     kr_error("kr_free: block not found in used list\n",
		      (unsigned long) ap, ctx);
	  if (*up == bp) {
	     *up = bp->s.ptr;
	     break;
	  }
       }

       /* Join the memory back into the free linked list */

       for (p=ctx->freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
	  if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
	     break; /* Freed block at start or end of arena */

       if (bp + bp->s.size == p->s.ptr) {/* join to upper neighbour */
	  bp->s.size += p->s.ptr->s.size;
	  bp->s.ptr = p->s.ptr->s.ptr;
	  ctx->nfrags--;                 /* Lost a fragment */
       } else
	  bp->s.ptr = p->s.ptr;

       if (p + p->s.size == bp) { /* Join to lower neighbour */
	  p->s.size += bp->s.size;
	  p->s.ptr = bp->s.ptr;
	  ctx->nfrags--;          /* Lost a fragment */
       } else
	  p->s.ptr = bp;
       ctx->freep = p;

    } /* end if on ap */
}

/*
  Print to standard output the usage statistics.
*/
void kr_malloc_print_stats(context_t *ctx) {
    fflush(stderr);
    printf("\nkr_malloc statistics\n-------------------\n\n");
    
    printf("Total memory from system ... %ld bytes\n", 
	   (long)(ctx->total*ctx->usize));
    printf("Current memory usage ....... %ld bytes\n", 
	   (long)(ctx->inuse*ctx->usize));
    printf("Maximum memory usage ....... %ld bytes\n", 
	   (long)(ctx->maxuse*ctx->usize));
    printf("No. chunks from system ..... %ld\n", ctx->nchunk);
    printf("No. of fragments ........... %ld\n", ctx->nfrags);
    printf("No. of calls to kr_malloc ... %ld\n", ctx->nmcalls);
    printf("No. of calls to kr_free ..... %ld\n", ctx->nfcalls);
    printf("\n");
    
    fflush(stdout);
}

/*
  Currently assumes that are working in a single region.
*/
void kr_malloc_verify(context_t *ctx) {
    Header *p;
    
    if(_armci_initialized && lock_mode==UNLOCKED) {
       LOCKIT(armci_master); lock_mode=LOCKED;
    }

    if ( ctx->freep ) {
      
      /* Check the used list */
      
      for (p=ctx->usedp; p; p=p->s.ptr) {
	if (p->s.valid1 != VALID1 || p->s.valid2 != VALID2)
	  kr_error("invalid header on usedlist", 
		   (unsigned long) p->s.valid1, ctx);
	
	if (p->s.size > ctx->total)
	  kr_error("invalid size in header on usedlist", 
		   (unsigned long) p->s.size, ctx);
      }
      
      /* Check the free list */
      
      p = ctx->base.s.ptr;
      while (p != &(ctx->base)) {
	if (p->s.valid1 != VALID1 || p->s.valid2 != VALID2)
	  kr_error("invalid header on freelist", 
		   (unsigned long) p->s.valid1, ctx);
	
	if (p->s.size > ctx->total)
	  kr_error("invalid size in header on freelist", 
		   (unsigned long) p->s.size, ctx);
	
	p = p->s.ptr;
      }
    } /* end if */
    
    if(_armci_initialized && lock_mode==LOCKED) {
       UNLOCKIT(armci_master); lock_mode=UNLOCKED;
    }
}

/********************** BEGIN: kr_malloc for ctx_shmem *********************/
#if defined(SYSV) || defined(MMAP)

#include "shmem.h"

extern int armci_get_shmem_info(char *addrp,  int* shmid, long *shmoffset,
				size_t *shmsize);
extern Header *armci_get_shmem_ptr(int shmid, long shmoffset, size_t shmsize);

/* returns, address of the shared memory region based on shmid, offset.
 * (i.e. return_addr = stating address of shmid + offset) */
#define SHM_PTR(hdr) armci_get_shmem_ptr((hdr)->s.shmid, (hdr)->s.shmoffset, (hdr)->s.shmsize)

/*
 * kr_malloc_shmem: memory allocator for shmem context (i.e ctx_shmem)
 */
static char *kr_malloc_shmem(size_t nbytes, context_t *ctx) {
    Header *p, *prevp;
    size_t nunits, prev_shmsize=0;
    char *return_ptr;
    int prev_shmid=-1;
    long prev_shmoffset=0;

    LOCKIT(armci_master);

    /* Rather than divide make the alignment a known power of 2 */
    nunits = ((nbytes + sizeof(Header) - 1)>>LOG_ALIGN) + 1;
    
    /* If first time in need to initialize the free list */ 
    if ((prevp = ctx->freep) == NULL) { 
      
      if (sizeof(Header) != ALIGNMENT)
	kr_error("kr_malloc_shmem: Alignment is not valid",
                 (unsigned long) ALIGNMENT, ctx);
      
      ctx->total  = 0; /* Initialize statistics */
      ctx->nchunk = ctx->inuse   = ctx->maxuse  = 0;  
      ctx->nfrags = ctx->nmcalls = ctx->nfcalls = 0;
      
      /* Initialize linked list */
      ctx->base.s.size = 0;
      ctx->base.s.shmid     = -1;
      ctx->base.s.shmoffset = 0;
      ctx->base.s.shmsize   = 0;
      ctx->base.s.valid1 = VALID1;
      ctx->base.s.valid2 = VALID2;
      if ((p = morecore(nunits, ctx)) == (Header *) NULL) return NULL;
      ctx->base.s.ptr = prevp = ctx->freep; /* CHECK */
    }

    prev_shmid     = ctx->shmid;
    prev_shmoffset = ctx->shmoffset;
    prev_shmsize   = ctx->shmsize;
    prevp = ctx->freep = armci_get_shmem_ptr(ctx->shmid, ctx->shmoffset,
                                             ctx->shmsize);

    ctx->nmcalls++;
    
    if (do_verify)  kr_malloc_verify(ctx);
    
    for (p=SHM_PTR(prevp); ; prevp = p, p = SHM_PTR(p)) {

      if (p->s.size >= nunits) {	/* Big enuf */
	if (p->s.size == nunits) {	/* exact fit */
	  prevp->s.ptr = p->s.ptr;
	  prevp->s.shmid     = p->s.shmid;
	  prevp->s.shmoffset = p->s.shmoffset;
	  prevp->s.shmsize   = p->s.shmsize;
	}
	else {			/* allocate tail end */
	   p->s.size -= nunits;
	   p += p->s.size;
	   p->s.size = nunits;
	   p->s.valid1 = VALID1;
	   p->s.valid2 = VALID2;
	   ctx->nfrags++;  /* Have just increased the fragmentation */
	}
#if USEDP
	/* Insert into linked list of blocks in use ... for debug only */
	p->s.ptr = ctx->usedp;
	ctx->usedp = p;
#endif
	
	ctx->inuse += nunits;  /* Record usage */
	if (ctx->inuse > ctx->maxuse)
	  ctx->maxuse = ctx->inuse;
	ctx->freep = prevp;
	ctx->shmid     = prev_shmid;
	ctx->shmoffset = prev_shmoffset;
	ctx->shmsize   = prev_shmsize;
	return_ptr = (char *) (p+1);
	break;
      }

      prev_shmid     = prevp->s.shmid;
      prev_shmoffset = prevp->s.shmoffset;
      prev_shmsize   = prevp->s.shmsize;
      
      if (p == ctx->freep)	{	/* wrapped around the free list */
	if ((p = morecore(nunits, ctx)) == (Header *) NULL) {
	  return_ptr = (char *) NULL;
	  break;
	}
	prev_shmid     = ctx->shmid;
	prev_shmoffset = ctx->shmoffset;
	prev_shmsize   = ctx->shmsize;
      }
    }
    
    UNLOCKIT(armci_master);
    return return_ptr;    
}


static void kr_free_shmem(char *ap, context_t *ctx) {
    Header *bp, *p, **up, *nextp;
    int shmid=-1;
    long shmoffset=0;
    size_t shmsize=0;

    LOCKIT(armci_master);
    
    ctx->nfcalls++;
    
    if (do_verify)
      kr_malloc_verify(ctx);
    
    /* only do something if pointer is not NULL */
    
    if ( ap ) {
      
      bp = (Header *) ap - 1;  /* Point to block header */
      
      if (bp->s.valid1 != VALID1 || bp->s.valid2 != VALID2)
	kr_error("kr_free_shmem: pointer not from kr_malloc", 
		 (unsigned long) ap, ctx);
      
      ctx->inuse -= bp->s.size; /* Decrement memory ctx->usage */
      
#if USEDP
      /* Extract the block from the used linked list
	 ... for debug only */
      
      for (up=&(ctx->usedp); ; up = &((*up)->s.ptr)) {
	if (!*up)
	  kr_error("kr_free_shmem: block not found in used list\n", 
		   (unsigned long) ap, ctx);
	if (*up == bp) {
	  *up = bp->s.ptr;
	  break;
	}
      }
#endif

      if(ctx->shmid==-1) { 
	 armci_get_shmem_info((char*)bp, &ctx->shmid, &ctx->shmoffset,
			      &ctx->shmsize);

         ctx->base.s.shmid     = ctx->shmid;
	 ctx->base.s.shmsize   = ctx->shmsize;
	 ctx->base.s.shmoffset = ctx->shmoffset;

         p = ctx->freep = bp;
         p->s.ptr  = bp;
         p->s.size-=SHMEM_CTX_BYTES; /*memory to store shmem info in context*/
         p->s.shmid     = ctx->shmid;
	 p->s.shmsize   = ctx->shmsize;
	 p->s.shmoffset = ctx->shmoffset;
         
         UNLOCKIT(armci_master);
         return;
      }

      ctx->freep = armci_get_shmem_ptr(ctx->shmid, ctx->shmoffset,
                                       ctx->shmsize);

      shmid     = ctx->shmid;
      shmoffset = ctx->shmoffset;
      shmsize   = ctx->shmsize;

      /* Join the memory back into the free linked list */
      p = ctx->freep;
      nextp = SHM_PTR(p);

      for ( ; !(bp > p && bp < nextp); p=nextp, nextp=SHM_PTR(p)) {
	 if (p >= nextp && (bp > p || bp < nextp))
	    break; /* Freed block at start or end of arena */
	 nextp = SHM_PTR(p);
	 shmid     = p->s.shmid;
	 shmoffset = p->s.shmoffset;
	 shmsize   = p->s.shmsize;
      }

      if (bp + bp->s.size == nextp) {/* join to upper neighbour */
	bp->s.size += nextp->s.size;
	bp->s.ptr = nextp->s.ptr;
	ctx->nfrags--;                 /* Lost a fragment */
	bp->s.shmid     = nextp->s.shmid;
	bp->s.shmoffset = nextp->s.shmoffset;
	bp->s.shmsize   = nextp->s.shmsize;	   
      } else {
	 bp->s.ptr = nextp;
	 bp->s.shmid     = p->s.shmid;
	 bp->s.shmoffset = p->s.shmoffset;
	 bp->s.shmsize   = p->s.shmsize;
      }

      if (p + p->s.size == bp) { /* Join to lower neighbour */
	 p->s.size += bp->s.size;
	 p->s.ptr = bp->s.ptr;
	 ctx->nfrags--;          /* Lost a fragment */
	 p->s.shmid     = bp->s.shmid;
	 p->s.shmoffset = bp->s.shmoffset;
	 p->s.shmsize   = bp->s.shmsize;
      } else {
	 p->s.ptr = bp;
	 armci_get_shmem_info((char*)bp, &p->s.shmid, &p->s.shmoffset,
			      &p->s.shmsize);
      }
      
      ctx->freep = p;
      ctx->shmid     = shmid;
      ctx->shmoffset = shmoffset;
      ctx->shmsize   = shmsize;
    } /* end if on ap */
    
    UNLOCKIT(armci_master);
}
#else /* #ifdef SYSV */
/* What are doing here */
static char *kr_malloc_shmem(size_t nbytes, context_t *ctx) 
{
    armci_die("kr_malloc_shmem(): Invalid Function Call", 0L);
}
static void kr_free_shmem(char *ap, context_t *ctx) 
{
    armci_die("kr_free_shmem(): Invalid Function Call", 0L);
}
#endif /* #ifdef SYSV */
/********************** END: kr_malloc for ctx_shmem *********************/


/**
issues:
1. do usage statistics only if debug/DEBUG is enabled 
*/

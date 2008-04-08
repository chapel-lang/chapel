/* $Id: bufalloc.c,v 1.2 2001-06-07 23:23:23 d3h325 Exp $ 
 * storage manager for a chunk of memory passed by user in armci_init_buf_alloc
 * derived from K&R that manages a chunk of memory
 */

#include <stdio.h>

#define USAGE_ 

extern char *buf_allocate(); /* Used to get memory from the system */
extern void armci_die();

#define VALID1  0xaaaaaaaa	/* For validity check on headers */
#define VALID2  0x55555555
#define LOG_ALIGN 6
#define ALIGNMENT (1 << LOG_ALIGN)
#define DEFAULT_NALLOC (1024 - ALIGNMENT)

#ifdef USAGE
static struct shmalloc_struct {
  size_t total;			/* Amount request from system in units */
  long nchunk;			/* No. of chunks of system memory */
  long inuse;			/* Amount in use in units */
  long maxuse;			/* Maximum value of inuse */
  long nfrags;			/* No. of fragments divided into */
  long nmcalls;			/* No. of calls to shmalloc */
  long nfcalls;			/* No. of calls to buf_free */
} usage;
#endif

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
static size_t nalloc = DEFAULT_NALLOC;
static size_t max_nalloc = DEFAULT_NALLOC;
static int do_verify = 0;	/* Flag for automatic heap verification */
static int initialized=0;


static void buf_error(char* s, unsigned long i)
{
  void buf_alloc_print_stats();
  fflush(stdout);
  fprintf(stderr,"buf_alloc error: %s %ld(0x%lx)\n", s, i, i);
  fflush(stderr);
#ifdef USAGE
  buf_alloc_print_stats();
#endif
  armci_die("buf_alloc: fatal error", i);
}

void armci_buf_alloc_request(size_t size, size_t maxsize)
{
  nalloc = (size+ALIGNMENT-1) >> LOG_ALIGN;
  max_nalloc = (maxsize+ALIGNMENT-1) >> LOG_ALIGN;
}

void armci_buf_alloc_debug(int code)
{
  do_verify = code;
}


void armci_buf_alloc_verify()
{
  Header *p;

  if ( freep ) {

    /* Check the used list */
    for (p=usedp; p; p=p->s.ptr) {
      if (p->s.valid1 != VALID1 || p->s.valid2 != VALID2)
        buf_error("invalid header on usedlist", (unsigned long) p->s.valid1);

#ifdef USAGE
      if (p->s.size > usage.total)
        buf_error("invalid size in header usedlist",(unsigned long)p->s.size);
#endif
    }

    /* Check the free list */
    p = base.s.ptr;
    while (p != &base) {
      if (p->s.valid1 != VALID1 || p->s.valid2 != VALID2)
        buf_error("invalid header on freelist", (unsigned long) p->s.valid1);

#ifdef USAGE
      if (p->s.size > usage.total)
        buf_error("invalid size in header freelist",(unsigned long)p->s.size);
#endif

      p = p->s.ptr;
    }
  } /* end if */
}


static void addtofree(char* ap)
{
  Header *bp, *p, **up;

#ifdef USAGE
  usage.nfcalls++;
#endif
  if (do_verify) armci_buf_alloc_verify();

  /* only do something if pointer is not NULL */
  if ( ap ) {

     bp = (Header *) ap - 1;  /* Point to block header */

     if (bp->s.valid1 != VALID1 || bp->s.valid2 != VALID2)
       buf_error("buf_free: pointer not from buf_alloc", (unsigned long) ap);
 
#ifdef USAGE
     usage.inuse -= bp->s.size; /* Decrement memory usage */
#endif

     /* Extract the block from the used linked list ... for debug only */
     for (up=&usedp; ; up = &((*up)->s.ptr)) {
       if (!*up)
         buf_error("buf_free:block not found in used list\n",(unsigned long)ap);
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
#ifdef USAGE
       usage.nfrags--;                 /* Lost a fragment */
#endif
     } else
       bp->s.ptr = p->s.ptr;

     if (p + p->s.size == bp) { /* Join to lower neighbour */
       p->s.size += bp->s.size;
       p->s.ptr = bp->s.ptr;
#ifdef USAGE
       usage.nfrags--;          /* Lost a fragment */
#endif
     } else
       p->s.ptr = bp;

     freep = p;

   } /* end if on ap */
}


void armci_init_buf_alloc(size_t len, void* buffer)
{
  char *cp;
  Header *up, *prevp;
  size_t nu;
  /* need to initialize the free list */
    if (sizeof(Header) != ALIGNMENT)
      buf_error("Alignment is not valid", (unsigned long) ALIGNMENT);

  if(initialized)armci_die("armci_init_buf_alloc: already initialized",0);

#ifdef USAGE
    usage.total  = 0;  /* Initialize statistics */
    usage.nchunk = 0;
    usage.inuse  = 0;
    usage.nfrags = 0;
    usage.maxuse = 0;
    usage.nmcalls= 0;
    usage.nfcalls= 0;
#endif

    base.s.ptr = freep = prevp = &base;  /* Initialize linked list */
    base.s.size = 0;
    base.s.valid1 = VALID1;
    base.s.valid2 = VALID2;
 
  nu = len/sizeof(Header); /* nu must by a multiplicity of nalloc */
  max_nalloc = nu*nalloc;
  if(nu<1) armci_die("buffer less than nalloc",(int)len);
  cp = (char*)buffer;
 
#ifdef USAGE
  usage.total += nu;   /* Have just got nu more units */
  usage.nchunk++;      /* One more chunk */
  usage.nfrags++;      /* Currently one more frag */
  usage.inuse += nu;   /* Inuse will be decremented by buf_free */
#endif

  up = (Header *) cp;
  up->s.size = nu;
  up->s.valid1 = VALID1;
  up->s.valid2 = VALID2;

  /* Insert into linked list of blocks in use so that buf_free works 
     ...  for debug only */
  up->s.ptr = usedp;
  usedp = up;

  addtofree((char *)(up+1));  /* Try to join into the free list */
}


/*\ return a chunk memory of given size
\*/
char *armci_buf_alloc(size_t nbytes)
{
  Header *p, *prevp;
  size_t nunits;
  char *return_ptr;

  /* need to initialize the free list */ 
  if ((prevp = freep) == NULL)  armci_die("not initialized", 0);
    
#ifdef USAGE
  usage.nmcalls++;
#endif
  
  if (do_verify) armci_buf_alloc_verify();
  
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
#ifdef USAGE
	usage.nfrags++;  /* Have just increased the fragmentation */
#endif
      }

      /* Insert into linked list of blocks in use ... for debug only */
      p->s.ptr = usedp;
      usedp = p;

#ifdef USAGE
      usage.inuse += nunits;  /* Record usage */
      if (usage.inuse > usage.maxuse)
	usage.maxuse = usage.inuse;
#endif
      freep = prevp;
      return_ptr = (char *) (p+1);
      break;
    }
    
    if (p == freep){		/* wrapped around the free list */
        return_ptr = (char *) NULL;
        break;
    }
  }
  return return_ptr;
}


void armci_buf_free(char *ap)
{
  Header *bp, *p, **up;
#ifdef USAGE
  usage.nfcalls++;
#endif
  if (do_verify) armci_buf_alloc_verify();

 /* only do something if pointer is not NULL */

  if ( ap ) {

      bp = (Header *) ap - 1;  /* Point to block header */

      if (bp->s.valid1 != VALID1 || bp->s.valid2 != VALID2)
        buf_error("buf_free: pointer not from buf_alloc", (unsigned long) ap);
  
#ifdef USAGE
      usage.inuse -= bp->s.size; /* Decrement memory usage */
#endif

      /* Extract the block from the used linked list for debug only */
      for (up=&usedp; ; up = &((*up)->s.ptr)) {
        if (!*up)
          buf_error("buf_free:block not found in used list\n",(unsigned long)ap);
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
#ifdef USAGE
                    usage.nfrags--;                 /* Lost a fragment */
#endif
      } else
                    bp->s.ptr = p->s.ptr;

      if (p + p->s.size == bp) { /* Join to lower neighbour */
                    p->s.size += bp->s.size;
                    p->s.ptr = bp->s.ptr;
#ifdef USAGE
                    usage.nfrags--;          /* Lost a fragment */
#endif
      } else
                    p->s.ptr = bp;

      freep = p;

    } /* end if on ap */
}


#ifdef USAGE
/*
  Return stats on buf_alloc performance. Use arg list instead of
  returning structure so that FORTRAN can eventually use it
*/
void buf_alloc_stats(size_t *total, long* nchunk, size_t * inuse,
                     size_t * maxuse, long* nfrags, long* nmcalls,long* nfcalls)
{
  *total  = usage.total * sizeof(Header);
  *nchunk = usage.nchunk;
  *inuse  = (size_t)usage.inuse * sizeof(Header);
  *maxuse = (size_t)usage.maxuse* sizeof(Header);
  *nfrags = usage.nfrags;
  *nmcalls= usage.nmcalls;
  *nfcalls= usage.nfcalls;
}

/*
  Print to standard output the usage statistics.
*/
void buf_alloc_print_stats()
{
  size_t total, inuse, maxuse;
  long nchunk, nfrags, nmcalls, nfcalls;

  buf_alloc_stats(&total, &nchunk, &inuse, &maxuse, &nfrags,
                 &nmcalls, &nfcalls);

  fflush(stderr);
  printf("\nbuf_alloc statistics\n-------------------\n\n");
  printf("Total memory from system ... %ld bytes\n", (long)total);
  printf("Current memory usage ....... %ld bytes\n", (long)inuse);
  printf("Maximum memory usage ....... %ld bytes\n", (long)maxuse);
  printf("No. chunks from system ..... %ld\n", nchunk);
  printf("No. of fragments ........... %ld\n", nfrags);
  printf("No. of calls to buf_alloc ... %ld\n", nmcalls);
  printf("No. of calls to buf_free ..... %ld\n", nfcalls);
  printf("\n");
  fflush(stdout);
}
#endif


#if 0
void armci_die(char *str, int c)
{
fprintf(stderr,"%s %d\n",str,c);
_exit(1);
}


#define LEN (16*1024)
char buf[LEN];

main (int argc, char **argv)
{
int i,k,total=0,size=1024;
char *ar[100];
    armci_init_buf_alloc(LEN, buf);
    
    for(i=0; i<100; i++)ar[i]=(char*)0;

    for(i=0; i<100; i++){
     ar[i] =armci_buf_alloc(size);
     if(!ar[i]){
        printf("i =%d total=%d\n", i, total);
        buf_alloc_print_stats(); 
        k=i;
        break;
     }
     total+=size;
    }
    for(i=0; i<k/2; i++){
      armci_buf_free(ar[i]);
      ar[i]=(char*)0;
    } 

    printf("relased %d blocks\n",k/2);
    buf_alloc_print_stats(); 
    size *=2;
    for(i=k; i<100; i++){
     ar[i] =armci_buf_alloc(size);
     if(!ar[i]){
        printf("i =%d total=%d\n", i, total);
        buf_alloc_print_stats(); 
        break;
     }
    }
    for(i=0; i<100; i++) if(ar[i])armci_buf_free(ar[i]);
    printf("relased all blocks\n");
    buf_alloc_print_stats(); 
}
#endif


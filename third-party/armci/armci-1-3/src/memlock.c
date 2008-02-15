/* $Id: memlock.c,v 1.21.4.5 2007/08/03 19:38:22 manoj Exp $ */
#include "armcip.h"
#include "locks.h"
#include "copy.h"
#include "memlock.h"
#include <stdio.h>

#define DEBUG_ 0
#define INVALID_VAL -9999999

#ifdef DATA_SERVER
#  define CORRECT_PTR 
#endif
size_t armci_mem_offset=0;

/* We start by  using table: assign address of local variable set to 1
 * On shmem systems, this addres is overwritten by a shared memory location
 * when memlock array is allocated in armci_init 
 * Therefore, any process within shmem node can reset armci_use_memlock_table
 * to "not used" when offset changes. Since the variable is in shmem, everybody
 * on that SMP node will see the change and use the same locking functions
 */ 
int init_use_memlock_table=1;
int *armci_use_memlock_table=&init_use_memlock_table;

static int locked_slot=INVALID_VAL;

volatile double armci_dummy_work=0.;
void **memlock_table_array;

/* constants for cache line alignment */
#ifdef SOLARIS
#  define CALGN 32
#  define LOG_CALGN 5
#else
#  define CALGN 64
#  define LOG_CALGN 6
#endif

#define ALIGN_ADDRESS(x) (char*)((((unsigned long)x) >> LOG_CALGN) << LOG_CALGN) 
#ifdef CRAY_T3E
#pragma _CRI cache_align table
#endif
static memlock_t table[MAX_SLOTS];

#if defined(SGIALTIX) || (defined(CRAY_SHMEM) && defined(XT3))
#define MAX_SEGS 512
armci_memoffset_t armci_memoffset_table[MAX_SEGS];
static short int seg_count=0;
static short int new_seg=0;
#endif

/*\ simple locking scheme that ignores addresses
\*/
void armci_lockmem_(void *pstart, void *pend, int proc)
{
#ifdef BGML
    bgml_lockmem(pstart, pend, proc);
#else

#if defined(CLUSTER) && !defined(SGIALTIX)
    int lock = (proc-armci_clus_info[armci_clus_id(proc)].master)%NUM_LOCKS;
#else
    int lock = 0;
#endif

    if(DEBUG_){
      printf("%d: armci_lockmem_ proc=%d lock=%d\n",armci_me,proc,lock);
      fflush(stdout);
    }

    NATIVE_LOCK(lock,proc);
#   ifdef LAPI
    {
       extern int kevin_ok;
       kevin_ok=0;
    }
#   endif
    if(DEBUG_){
      printf("%d: armci_lockmem_ done\n",armci_me);
      fflush(stdout);
    }
#endif
}

void armci_unlockmem_(int proc)
{
#ifdef BGML
    bgml_unlockmem(proc);
#else

#if defined(CLUSTER) && !defined(SGIALTIX) 
    int lock = (proc-armci_clus_info[armci_clus_id(proc)].master)%NUM_LOCKS;
#else
    int lock = 0;
#endif
    NATIVE_UNLOCK(lock,proc);
#   ifdef LAPI
    {
       extern int kevin_ok;
       kevin_ok=1;
    }
#   endif
#endif
}



/*\ idle for a time proportional to factor 
\*/
void armci_waitsome(int factor)
{
int i=factor*100000;

   if(factor <= 1) armci_dummy_work =0.;
   if(factor < 1) return;
   while(--i){
      armci_dummy_work = armci_dummy_work + 1./(double)i;  
   }
}
   
#ifdef SGIALTIX
#include <mpp/shmem.h>
#endif

/*\ acquire exclusive LOCK to MEMORY area <pstart,pend> owned by process "proc"
 *   . only one area can be locked at a time by the calling process
 *   . must unlock it with armci_unlockmem
\*/
void armci_lockmem(void *start, void *end, int proc)
{
     register void* pstart, *pend;
     register  int slot, avail=0;
     int turn=0, conflict=0;
     memlock_t *memlock_table;
#if defined(CLUSTER) && !defined(SGIALTIX)
    int lock = (proc-armci_clus_info[armci_clus_id(proc)].master)%NUM_LOCKS;
#else
    int lock = 0;
#endif

#ifdef CORRECT_PTR
     if(! *armci_use_memlock_table){
       /* if offset invalid, use dumb locking scheme ignoring addresses */
       armci_lockmem_(start, end, proc); 
       return;
     }

#  ifndef SGIALTIX
     /* when processes are attached to a shmem region at different addresses,
      * addresses written to memlock table must be adjusted to the node master
      */
     if(armci_mem_offset){
        start = armci_mem_offset + (char*)start;
        end   = armci_mem_offset + (char*)end;
     }
#  endif
#endif

     if(DEBUG_){
       printf("%d: calling armci_lockmem for %d range %p -%p\n",
              armci_me, proc, start,end);
       fflush(stdout);
     }
     memlock_table = (memlock_t*)memlock_table_array[proc];


#ifdef ALIGN_ADDRESS
     /* align address range on cache line boundary to avoid false sharing */
     pstart = ALIGN_ADDRESS(start);
     pend = CALGN -1 + ALIGN_ADDRESS(end);
#else
     pstart=start;
     pend =end;
#endif

#ifdef CRAY_SHMEM
     { /* adjust according the remote process raw address */
        long bytes = (long) ((char*)pend-(char*)pstart);
        extern void* armci_shmalloc_remote_addr(void *ptr, int proc);
        pstart = armci_shmalloc_remote_addr(pstart, proc);
        pend   = (char*)pstart + bytes;
     }
#endif
    
#ifdef SGIALTIX
     if (proc == armci_me) {
    pstart = shmem_ptr(pstart,armci_me);
    pend = shmem_ptr(pend,armci_me);
     }
     /* In SGI Altix  processes are attached to a shmem region at different
    addresses. Addresses written to memlock table must be adjusted to 
    the node master
      */
     if(ARMCI_Uses_shm()){
    int i, seg_id=-1;
    size_t tile_size,offset;
    void *start_addr, *end_addr;
    for(i=0; i<seg_count; i++) {
       tile_size = armci_memoffset_table[i].tile_size;
       start_addr = (void*) ((char*)armci_memoffset_table[i].seg_addr +
                 proc*tile_size);
       end_addr = (void*) ((char*)start_addr +
                   armci_memoffset_table[i].seg_size);
       /* CHECK: because of too much "span" in armci_lockmem_patch in 
        * strided.c, it is not possible to have condition as (commented):*/
       /*if(pstart>=start_addr && pend<=end_addr) {seg_id=i; break;}*/
       if(pstart >= start_addr && pstart <= end_addr) {seg_id=i; break;}
    }
    if(seg_id==-1) armci_die("armci_lockmem: Invalid segment", seg_id);

    offset = armci_memoffset_table[seg_id].mem_offset;
    pstart = ((char*)pstart + offset);
    pend = ((char*)pend + offset);
     }
#endif

     while(1){
        NATIVE_LOCK(lock,proc);

        armci_get(memlock_table, table, sizeof(table), proc);
/*        armci_copy(memlock_table, table, sizeof(table));*/
        
        /* inspect the table */
        conflict = 0; avail =-1;
        for(slot = 0; slot < MAX_SLOTS; slot ++){

            /* nonzero starting address means the slot is occupied */ 
            if(table[slot].start == NULL){

              /* remember a free slot to store address range */
              avail = slot;  
          
            }else{
              /*check for conflict: overlap between stored and current range*/
              if(  (pstart >= table[slot].start && pstart <= table[slot].end)
                 || (pend >= table[slot].start && pend <= table[slot].end) ){

                  conflict = 1;
                  break;

              }
              
            }
       }
        
       if(avail != -1 && !conflict) break;

       NATIVE_UNLOCK(lock,proc);
       armci_waitsome( ++turn );

     }

     /* we got the memory lock: enter address into the table */
     table[avail].start = pstart;
     table[avail].end = pend;
     armci_put(table+avail,memlock_table+avail,sizeof(memlock_t),proc);

     FENCE_NODE(proc);

     NATIVE_UNLOCK(lock,proc);
     locked_slot = avail;

}
        

/*\ release lock to the memory area locked by previous call to armci_lockemem
\*/
void armci_unlockmem(int proc)
{
     void *null[2] = {NULL,NULL};
     memlock_t *memlock_table;

#ifdef CORRECT_PTR
     if(! *armci_use_memlock_table){
       /* if offset invalid, use dumb locking scheme ignoring addresses */
       armci_unlockmem_(proc);               
       return;
     }
#endif

#ifdef DEBUG
     if(locked_slot == INVALID_VAL) armci_die("armci_unlock: empty",0);
     if(locked_slot >= MAX_SLOTS || locked_slot <0) 
        armci_die("armci_unlock: corrupted slot?",locked_slot);
#endif

     memlock_table = (memlock_t*)memlock_table_array[proc];
     armci_put(null,&memlock_table[locked_slot].start,2*sizeof(void*),proc);

}



/*\ based on address for set by master, determine correction for
 *  memory addresses set in memlock table
 *  if the correction/offset ever changes stop using memlock table locking
\*/ 
void armci_set_mem_offset(void *ptr)
{
   extern size_t armci_mem_offset;
   size_t off;
   static int first_time=1;
   volatile void *ref_ptr;

   /* do not care if memlock not used */
   if(! *armci_use_memlock_table) return;

   if(!ptr) armci_die("armci_set_mem_offset : null ptr",0);
   ref_ptr = *(void**)ptr;
   off = (size_t)((char*)ref_ptr - (char*)ptr);

   if(first_time){

      armci_mem_offset =off;
      first_time =0;
      if(DEBUG_){
        printf("%d memlock offset=%ld ref=%p ptr=%p\n",armci_me,
                  (long)armci_mem_offset, ref_ptr, ptr); fflush(stdout);
      }

   }else{
      if(armci_mem_offset != off){
         *armci_use_memlock_table =0;
         printf("%d: WARNING:armci_set_mem_offset: offset changed %ld to %ld\n",
                 armci_me, (long)armci_mem_offset, (long)off); fflush(stdout);
      }
   }
}

#ifdef SGIALTIX
/* SGI Altix Stuff */ 
static void armci_altix_gettilesize(void *ptr, void **ptr_arr,
                                    size_t *tile_size) {
    int i;
    size_t diff=0;
    for(i=0; i<armci_nproc; i++) {
      ptr_arr[i]=shmem_ptr(ptr,i);
      if(i>0) diff = (size_t)((char*)ptr_arr[i]-(char*)ptr_arr[i-1]);
      if(i>1 && diff!=*tile_size)
    armci_die("armci_memoffset_table_newentry:Inconsistent tile size",
          armci_me);
      *tile_size = diff;
    }
}

void armci_memoffset_table_newentry(void *ptr, size_t seg_size) {
 
    void **ptr_arr;
    void *master_addr = NULL;
    size_t tile_size=0, offset=0;

    if(!ptr) armci_die("armci_memoffset_table_newentry : null ptr",0);
 
    if(seg_count >= MAX_SEGS) /* CHECK: make it dynamic */
       armci_die("armci_altix_allocate: Increase MAX_SEGS > 512", armci_me);
 
    if(armci_me == armci_master) master_addr = shmem_ptr(ptr, armci_me);
    armci_msg_brdcst(&master_addr, sizeof(void*), armci_master);
 
    ptr_arr = (void**)malloc(armci_nproc*sizeof(void*));
    armci_altix_gettilesize(ptr, ptr_arr, &tile_size);
    offset = (size_t)((char*)master_addr -  (char*)ptr_arr[armci_master]);
 
    /* enter in memoffset table */
    armci_memoffset_table[seg_count].seg_addr   = ptr_arr[armci_master];
    armci_memoffset_table[seg_count].seg_size   = seg_size;
    armci_memoffset_table[seg_count].tile_size  = tile_size;
    armci_memoffset_table[seg_count].mem_offset = offset;

#if DEBUG_
    printf("%d: addr=%p seg_size=%ld tile_size=%ld offset=%ld\n", armci_me,
       ptr_arr[armci_master], seg_size, tile_size, offset);
#endif
 
    ++seg_count;
    free(ptr_arr);
}
#endif

#if defined(CRAY_SHMEM) && defined(XT3)
/* CRAY-XT3 stuff */ 
static void armci_cray_gettilesize(void *ptr, void **ptr_arr,
                                    size_t *tile_size) {
    int i;
    size_t diff=0;
    for(i=0; i<armci_nproc; i++) {
      ptr_arr[i]=ptr;
      if(i>0) diff = (size_t)((char*)ptr_arr[i]-(char*)ptr_arr[i-1]);
      if(i>1 && diff!=*tile_size)
          armci_die("armci_memoffset_table_newentry:Inconsistent tile size",
                    armci_me);
      *tile_size = diff;
    }
}

void armci_memoffset_table_newentry(void *ptr, size_t seg_size) {
 
    void **ptr_arr;
    void *master_addr = NULL;
    size_t tile_size=0, offset=0;

    if(!ptr) armci_die("armci_memoffset_table_newentry : null ptr",0);
 
    if(seg_count >= MAX_SEGS) /* CHECK: make it dynamic */
       armci_die("armci_cary_allocate: Increase MAX_SEGS > 512", armci_me);
 
    if(armci_me == armci_master) master_addr = ptr;
    armci_msg_brdcst(&master_addr, sizeof(void*), armci_master);
 
    ptr_arr = (void**)malloc(armci_nproc*sizeof(void*));
    armci_cray_gettilesize(ptr, ptr_arr, &tile_size);
    offset = (size_t)((char*)master_addr -  (char*)ptr_arr[armci_master]);
 
    /* enter in memoffset table */
    armci_memoffset_table[seg_count].seg_addr   = ptr_arr[armci_master];
    armci_memoffset_table[seg_count].seg_size   = seg_size;
    armci_memoffset_table[seg_count].tile_size  = tile_size;
    armci_memoffset_table[seg_count].mem_offset = offset;

#if DEBUG_
    printf("%d: addr=%p seg_size=%ld tile_size=%ld offset=%ld\n", armci_me,
       ptr_arr[armci_master], seg_size, tile_size, offset);
#endif
 
    ++seg_count;
    free(ptr_arr);
}
#endif

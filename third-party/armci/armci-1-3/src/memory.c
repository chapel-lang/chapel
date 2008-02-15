/* $Id: memory.c,v 1.52.4.6 2007/08/03 19:38:22 manoj Exp $ */
#include <stdio.h>
#include <assert.h>
#include "armcip.h"
#include "message.h"
#include "kr_malloc.h"

#define DEBUG_ 0
#define USE_MALLOC
#define USE_SHMEM_
#define SHM_UNIT 1024

#if defined(CRAY_SHMEM)
extern void armci_shmalloc_exchange_address(void **ptr_arr);
extern void armci_shmalloc_exchange_offsets(context_t *);
#  if defined(XT3)
#  include <mpp/shmem.h>
#    ifdef CATAMOUNT
#      include <catamount/data.h>
#    endif
#  endif
#endif

static context_t ctx_localmem;

#if defined(SYSV) || defined(WIN32) || defined(MMAP) || defined(HITACHI)
#include "shmem.h"

#if !defined(USE_SHMEM) && (defined(HITACHI) || defined(MULTI_CTX))
#    define USE_SHMEM
#endif

#if !(defined(LAPI)||defined(QUADRICS)||defined(SERVER_THREAD)) ||\
    defined(USE_SHMEM)
#define RMA_NEEDS_SHMEM
#endif

#ifdef GA_USE_VAMPIR
#include "armci_vampir.h"
#endif

/****************************************************************************
 * Memory Allocator called by kr_malloc on SGI Altix to get more core from OS
 */
#ifdef SGIALTIX

#include <mpp/shmem.h>
#include <unistd.h>

#define DEF_UNITS (64)
#define MAX_SEGS  512

#define _SHMMAX_ALTIX     32*1024  /* 32 MB */
#define _SHMMAX_ALTIX_GRP 512*1024 /* 512 MB */

static  context_t altix_ctx_shmem;
static  context_t altix_ctx_shmem_grp;
static  size_t altix_pagesize;
extern void armci_memoffset_table_newentry(void *ptr, size_t seg_size);

void *armci_altix_allocate(size_t bytes)
{
    void *ptr, *sptr;

    sptr=ptr= shmalloc(bytes);
    if(sptr == NULL) armci_die("armci_altix_allocate: shmalloc failed\n",
                               armci_me);
    armci_memoffset_table_newentry(ptr, bytes);
#if 0
    if(ptr){  /* touch each page to establish ownership */
       int i;
       for(i=0; i< bytes/altix_pagesize; i++){
           *(double*)ptr=0.;
           ((char*)ptr) += altix_pagesize;
       }
    }
#endif
    return sptr;
}

void armci_altix_shm_init()
{
    altix_pagesize = getpagesize();
    kr_malloc_init(SHM_UNIT, _SHMMAX_ALTIX, 0,
                   armci_altix_allocate, 0, &altix_ctx_shmem);
    kr_malloc_init(SHM_UNIT, _SHMMAX_ALTIX_GRP, _SHMMAX_ALTIX_GRP,
                   armci_altix_allocate, 0, &altix_ctx_shmem_grp);
    /* allocate a huge segment for groups. When kr_malloc() is called for
     the first time for this altix_ctx_shmem_grp context with some minimal
     size of 8 bytes, a huge segment of size (SHM_UNIT*_SHMMAX_ALTIX_GRP)
     will be created */
    {
       void *ptr;
       ptr=kr_malloc((size_t)8, &altix_ctx_shmem_grp);
       if(ptr==NULL)
           armci_die("armci_altix_shm_init(): malloc failed", armci_me);
    }
}

void armci_altix_shm_malloc(void *ptr_arr[], armci_size_t bytes)
{
    long size=bytes;
    void *ptr;
    int i;
    armci_msg_lgop(&size,1,"max");
    ptr=kr_malloc((size_t)size, &altix_ctx_shmem);
    bzero(ptr_arr,(armci_nproc)*sizeof(void*));
    ptr_arr[armci_me] = ptr;
    if(size!=0 && ptr==NULL)
       armci_die("armci_altix_shm_malloc(): malloc failed", armci_me);
    for(i=0; i< armci_nproc; i++) if(i!=armci_me) ptr_arr[i]=shmem_ptr(ptr,i);
}

#ifdef MPI
void armci_altix_shm_malloc_group(void *ptr_arr[], armci_size_t bytes,
                                  ARMCI_Group *group) {
    long size=bytes;
    void *ptr;
    int i,grp_me, grp_nproc;
    armci_grp_attr_t *grp_attr=ARMCI_Group_getattr(group);

    ARMCI_Group_size(group, &grp_nproc);
    ARMCI_Group_rank(group, &grp_me);
    armci_msg_group_lgop(&size,1,"max",group);
    ptr=kr_malloc((size_t)size, &altix_ctx_shmem_grp);
    if(size!=0 && ptr==NULL)
       armci_die("armci_altix_shm_malloc_group(): malloc failed for groups. Increase _SHMMAX_ALTIX_GRP", armci_me);
    bzero(ptr_arr,(grp_nproc)*sizeof(void*));
    ptr_arr[grp_me] = ptr;
    for(i=0; i< grp_nproc; i++) if(i!=grp_me) ptr_arr[i]=shmem_ptr(ptr,ARMCI_Absolute_id(group, i));
}
#endif

#endif /* end ifdef SGIALTIX */
/* ------------------ End Altix memory allocator ----------------- */

void kr_check_local()
{
#if 0
kr_malloc_print_stats(&ctx_localmem);
#endif
kr_malloc_verify(&ctx_localmem);
}

void  armci_print_ptr(void **ptr_arr, int bytes, int size, void* myptr, int off)
{
int i;
int nproc = armci_clus_info[armci_clus_me].nslave;

    for(i=0; i< armci_nproc; i++){
      int j;
      if(armci_me ==i){
        printf("%d master =%d nproc=%d off=%d\n",armci_me, 
               armci_master,nproc, off);
        printf("%d:bytes=%d mptr=%p s=%d ",armci_me, bytes, myptr,size);
        for(j = 0; j< armci_nproc; j++)printf(" %p",ptr_arr[j]);
        printf("\n"); fflush(stdout);
      }
      armci_msg_barrier();
   }
}


/*\ master exports its address of shmem region at the beggining of that region
\*/
static void armci_master_exp_attached_ptr(void* ptr)
{
    if(!ptr) armci_die("armci_master_exp_att_ptr: null ptr",0);
    *(volatile void**)ptr = ptr;
}


/*\ Collective Memory Allocation on shared memory systems
\*/
void armci_shmem_malloc(void *ptr_arr[], armci_size_t bytes)
{
    void *myptr=NULL, *ptr=NULL;
    long idlist[SHMIDLEN];
    long size=0, offset=0;
    long *size_arr;
    void **ptr_ref_arr;
    int  i,cn, len;
    int  nproc = armci_clus_info[armci_clus_me].nslave;

    bzero((char*)ptr_arr,armci_nproc*sizeof(void*));

    /* allocate work arrays */
    size_arr = (long*)calloc(armci_nproc,sizeof(long));
    if(!size_arr)armci_die("armci_malloc:calloc failed",armci_nproc);

    /* allocate arrays for cluster address translations */
#   if defined(DATA_SERVER)
       len = armci_nclus;
#   else
       len = nproc;
#   endif

    ptr_ref_arr = calloc(len,sizeof(void*)); /* must be zero */
    if(!ptr_ref_arr)armci_die("armci_malloc:calloc 2 failed",len);

    /* combine all memory requests into size_arr  */
    size_arr[armci_me] = bytes;
    armci_msg_lgop(size_arr, armci_nproc, "+");

    /* determine aggregate request size on the cluster node */
    for(i=0, size=0; i< nproc; i++) size += size_arr[i+armci_master];

    /* master process creates shmem region and then others attach to it */
    if(armci_me == armci_master ){

       /* can malloc if there is no data server process and has 1 process/node*/
#      ifndef RMA_NEEDS_SHMEM
             if(nproc == 1)
                myptr = kr_malloc(size, &ctx_localmem);
             else
#      endif
                myptr = Create_Shared_Region(idlist+1,size,idlist);
       if(!myptr && size>0 )armci_die("armci_malloc: could not create", (int)(size>>10));

       /* place its address at begining of attached region for others to see */
       if(size)armci_master_exp_attached_ptr(myptr);

       if(DEBUG_){
         printf("%d:armci_malloc addr mptr=%p size=%ld\n",armci_me,myptr,size);
         fflush(stdout);
       }
    }

    /* broadcast shmem id to other processes on the same cluster node */
    armci_msg_clus_brdcst(idlist, SHMIDLEN*sizeof(long));

    if(armci_me != armci_master){
       myptr=(double*)Attach_Shared_Region(idlist+1,size,idlist[0]);
       if(!myptr)armci_die("armci_malloc: could not attach", (int)(size>>10));

       /* now every process in a SMP node needs to find out its offset
        * w.r.t. master - this offset is necessary to use memlock table
        */
       if(size) armci_set_mem_offset(myptr);
       if(DEBUG_){
          printf("%d:armci_malloc attached addr mptr=%p ref=%p size=%ld\n",
                 armci_me,myptr, *(void**)myptr,size); fflush(stdout);
       }
    }
#   ifdef HITACHI
        armci_register_shmem(myptr,size,idlist+1,idlist[0],ptr_ref_arr[armci_clus_me]);
#   endif
#   if defined(DATA_SERVER)

       /* get server reference address for every cluster node to perform
        * remote address translation for global address space */
       if(armci_nclus>1){
          if(armci_me == armci_master){

#            ifdef SERVER_THREAD

               /* data server thread runs on master process */
               ptr_ref_arr[armci_clus_me]=myptr;

#            else
               /* ask dataserver process to attach to the region and get
                * ptr*/
               {
                  extern int _armci_server_started;
                  if(_armci_server_started) {
                     armci_serv_attach_req(idlist, SHMIDLEN*sizeof(long), size,
                                           &ptr, sizeof(void*));
                     ptr_ref_arr[armci_clus_me]= ptr; /* from server*/
                  }
                  else /* server not yet started */
                     ptr_ref_arr[armci_clus_me]=myptr;
               }

               if(DEBUG_){
                 printf("%d:addresses server=%p myptr=%p\n",armci_me,ptr,myptr);
                 fflush(stdout);
               }
#            endif
          }
          /* exchange ref addr of shared memory region on every cluster node*/
          armci_exchange_address(ptr_ref_arr, armci_nclus);
       }else {

          ptr_ref_arr[armci_master] = myptr;

       }

       /* translate addresses for all cluster nodes */
       for(cn = 0; cn < armci_nclus; cn++){

         int master = armci_clus_info[cn].master;
         offset = 0;

         /* on local cluster node use myptr directly */
         ptr = (armci_clus_me == cn) ? myptr: ptr_ref_arr[cn];

         /* compute addresses pointing to the memory regions on cluster node*/
         for(i=0; i< armci_clus_info[cn].nslave; i++){

           /* NULL if request size is 0*/
           ptr_arr[i+master] = (size_arr[i+master])? ((char*)ptr)+offset : NULL;
           offset += size_arr[i+master];

         }
       }

#   else

      /* compute addresses for local cluster node */
      offset =0;
      for(i=0; i< nproc; i++) {

        ptr_ref_arr[i] = (size_arr[i+armci_master])? ((char*)myptr)+offset : 0L;
        offset += size_arr[i+armci_master];

      }
      
      /* exchange addreses with all other processes */
      ptr_arr[armci_me] = (char*)ptr_ref_arr[armci_me-armci_master]; 
      armci_exchange_address(ptr_arr, armci_nproc);

      /* overwrite entries for local cluster node with ptr_ref_arr */
      bcopy((char*)ptr_ref_arr, (char*)(ptr_arr+armci_master), nproc*sizeof(void*)); 

     /*  armci_print_ptr(ptr_arr, bytes, size, myptr, offset);*/

#   endif

#ifdef ALLOW_PIN
    if(armci_nclus>1)armci_global_region_exchange(myptr, (long) size_arr[armci_me]);
    else
#endif
    armci_msg_barrier();

    /* free work arrays */
    free(ptr_ref_arr);
    free(size_arr);

}

#ifdef MPI
/********************************************************************
 * Group Memory Allocation on shared memory systems for ARMCI Groups
\*/
void armci_shmem_malloc_group(void *ptr_arr[], armci_size_t bytes,
                              ARMCI_Group *group)
{
    void *myptr=NULL, *ptr=NULL;
    long idlist[SHMIDLEN];
    long size=0, offset=0;
    long *size_arr;
    void **ptr_ref_arr;
    int  i,cn, len;
    /* int  nproc = armci_clus_info[armci_clus_me].nslave; ? change ? */
    int grp_me, grp_nproc, grp_nclus, grp_master, grp_clus_nproc, grp_clus_me;
    armci_grp_attr_t *grp_attr=ARMCI_Group_getattr(group);

    /* Get the group info: group size & group rank */
    ARMCI_Group_size(group, &grp_nproc);
    ARMCI_Group_rank(group, &grp_me);
    if(grp_me == MPI_UNDEFINED) { /* check if the process is in this group */
       armci_die("armci_malloc_group: process is not a member in this group",
                 armci_me);
    }

    grp_nclus      = grp_attr->grp_nclus;
    grp_clus_me    = grp_attr->grp_clus_me;
    grp_master     = grp_attr->grp_clus_info[grp_clus_me].master;
    grp_clus_nproc = grp_attr->grp_clus_info[grp_clus_me].nslave;

    bzero((char*)ptr_arr,grp_nproc*sizeof(void*));

    /* allocate work arrays */
    size_arr = (long*)calloc(grp_nproc,sizeof(long));
    if(!size_arr)armci_die("armci_malloc_group:calloc failed",grp_nproc);

    /* allocate arrays for cluster address translations */
#   if defined(DATA_SERVER)
        len = grp_nclus;
#   else
        len = grp_clus_nproc;
#   endif

    ptr_ref_arr = calloc(len,sizeof(void*)); /* must be zero */
    if(!ptr_ref_arr)armci_die("armci_malloc_group:calloc 2 failed",len);

    /* combine all memory requests into size_arr  */
    size_arr[grp_me] = bytes;
    armci_msg_group_gop_scope(SCOPE_ALL, size_arr, grp_nproc, "+", ARMCI_LONG,
                              group);

    /* determine aggregate request size on the cluster node */
    for(i=0, size=0; i< grp_clus_nproc; i++) size += size_arr[i+grp_master];

    /* master process creates shmem region and then others attach to it */
    if(grp_me == grp_master ){


       /* can malloc if there is no data server process and has 1 process/node*/
#     ifndef RMA_NEEDS_SHMEM
       if( armci_clus_info[armci_clus_me].nslave == 1)
         myptr = kr_malloc(size, &ctx_localmem);
       else
#     endif
         myptr = Create_Shared_Region(idlist+1,size,idlist);
       if(!myptr && size>0 )armci_die("armci_malloc_group: could not create", (int)(size>>10));

       /* place its address at begining of attached region for others to see */
       if(size)armci_master_exp_attached_ptr(myptr);

       if(DEBUG_){
         printf("%d:armci_malloc_group addr mptr=%p ref=%p size=%ld %ld %ld \n",armci_me,myptr,*(void**)myptr, size,idlist[0],idlist[1]);
         fflush(stdout);
       }
    }

    /* broadcast shmem id to other processes (in the same group) on the 
       same cluster node */
    armci_grp_clus_brdcst(idlist, SHMIDLEN*sizeof(long), grp_master, 
                          grp_clus_nproc, group);

    if(grp_me != grp_master){
       myptr=(double*)Attach_Shared_Region(idlist+1,size,idlist[0]);
       if(!myptr)armci_die("armci_malloc_group: could not attach", (int)(size>>10));

       /* now every process in a SMP node needs to find out its offset
        * w.r.t. master - this offset is necessary to use memlock table
        */
       if(size) armci_set_mem_offset(myptr);
       if(DEBUG_){
          printf("%d:armci_malloc_group attached addr mptr=%p ref=%p size=%ld\n",
                 armci_me,myptr, *(void**)myptr,size); fflush(stdout);
       }
    }
#   ifdef HITACHI
    armci_register_shmem(myptr,size,idlist+1,idlist[0],ptr_ref_arr[armci_clus_me]);
#   endif
    
#   if defined(DATA_SERVER)
 
    /* get server reference address for every cluster node in the group 
     * to perform remote address translation for global address space */
    if(grp_nclus>1){
       if(grp_me == grp_master){
 
#            ifdef SERVER_THREAD
 
          /* data server thread runs on master process */
          if(ARMCI_Absolute_id(group,grp_master)!=armci_master){
            /*printf("\n%d: grp_master=%d %ld %ld \n",armci_me,ARMCI_Absolute_id(group,grp_master),idlist[0],idlist[1]);*/
            armci_serv_attach_req(idlist, SHMIDLEN*sizeof(long), size,
                                  &ptr, sizeof(void*));
            ptr_ref_arr[grp_clus_me]= ptr; /* from server*/
          }
          else
            ptr_ref_arr[grp_clus_me]=myptr;
          
#            else
          /* ask data server process to attach to the region and get ptr */
          {
             extern int _armci_server_started;
             if(_armci_server_started) {
                armci_serv_attach_req(idlist, SHMIDLEN*sizeof(long), size,
                                      &ptr, sizeof(void*));
                ptr_ref_arr[grp_clus_me]= ptr; /* from server*/
             }
             else /* server not yet started */
                ptr_ref_arr[grp_clus_me]=myptr;
          }
 
          if(DEBUG_){
             printf("%d:addresses server=%p myptr=%p\n",grp_me,ptr,myptr);
               fflush(stdout);
          }
#            endif
       }
       /* exchange ref addr of shared memory region on every cluster node*/
       {
          int ratio = sizeof(void*)/sizeof(int);
          if(DEBUG_)printf("%d: exchanging %ld ratio=%d\n",armci_me,
                           (long)ptr_arr[grp_me], ratio);
          armci_msg_group_gop_scope(SCOPE_ALL, ptr_ref_arr, grp_nclus*ratio,
                                    "+", ARMCI_INT, group);
       }
    }else {
       
       ptr_ref_arr[grp_master] = myptr;
       
    }
    
    /* translate addresses for all cluster nodes */
    for(cn = 0; cn < grp_nclus; cn++){
       
       int master = grp_attr->grp_clus_info[cn].master;
       offset = 0;
 
       /* on local cluster node use myptr directly */
       ptr = (grp_clus_me == cn) ? myptr: ptr_ref_arr[cn];

       /* compute addresses pointing to the memory regions on cluster node*/
       for(i=0; i< grp_attr->grp_clus_info[cn].nslave; i++){
 
          /* NULL if request size is 0*/
          ptr_arr[i+master] =(size_arr[i+master])? ((char*)ptr)+offset: NULL;
            offset += size_arr[i+master];
       }
    }
 
#   else

    /* compute addresses for local cluster node */
    offset =0;
    for(i=0; i< grp_clus_nproc; i++) {
       
       ptr_ref_arr[i] = (size_arr[i+grp_master])? ((char*)myptr)+offset : 0L;
       offset += size_arr[i+grp_master];
       
    }
    
    /* exchange addreses with all other processes */
    ptr_arr[grp_me] = (char*)ptr_ref_arr[grp_me-grp_master]; 
    armci_exchange_address_grp(ptr_arr, grp_nproc, group);

    /* overwrite entries for local cluster node with ptr_ref_arr */
    bcopy((char*)ptr_ref_arr, (char*)(ptr_arr+grp_master), grp_clus_nproc*sizeof(void*)); 
     
#   endif

    /*  armci_print_ptr(ptr_arr, bytes, size, myptr, offset);*/
       
#ifdef ALLOW_PIN
#if 0
    /* ????? check ??????  */
    armci_die("armci_malloc_group: Not yet implemented", 0);
    if(grp_nclus>1)armci_global_region_exchange(myptr, (long) size_arr[grp_me]);
    else
#endif
#endif
       armci_msg_group_barrier(group);

    /* free work arrays */
    free(ptr_ref_arr);
    free(size_arr);
}
#endif /* ifdef MPI */

#else

void armci_shmem_malloc(void* ptr_arr[], int bytes)
{
  armci_die("armci_shmem_malloc should never be called on this system",0);
}
# ifdef MPI
  void armci_shmem_malloc_group(void *ptr_arr[], armci_size_t bytes,
                                ARMCI_Group *group) {
      armci_die("armci_shmem_malloc_group should never be called on this system",0);
  }
# endif

#endif


#ifdef ALLOW_PIN
void *reg_malloc(size_t size)
{
     char *ptr = malloc(size);
     armci_region_register_loc(ptr,(long)size);
     return(ptr);
}
#endif

/* public constructor to initialize the kr_malloc context */
void armci_krmalloc_init_localmem() {
#if defined(ALLOW_PIN)
    
    kr_malloc_init(0, 0, 0, reg_malloc, 0, &ctx_localmem);
    
#elif defined(CRAY_SHMEM) && defined(XT3)

#   ifdef CATAMOUNT
    /* note, 1M is a hack, needs justification and/or better number */
    int units_avail = (cnos_shmem_size() - 1024 * 1024) / SHM_UNIT;
#   else
    extern size_t get_xt_heapsize();
    int units_avail = (get_xt_heapsize() - 1024 * 1024) / SHM_UNIT;
#   endif
    
    if(DEBUG_) 
    {
       fprintf(stderr,"%d:krmalloc_init_localmem: symheap=%llu,units(%d)=%d\n",
               armci_me, SHM_UNIT*units_avail, SHM_UNIT, units_avail);
    }
    kr_malloc_init(SHM_UNIT, units_avail, units_avail, shmalloc, 0,
                   &ctx_localmem);

    armci_shmalloc_exchange_offsets(&ctx_localmem);
#else
    
    kr_malloc_init(0, 0, 0, malloc, 0, &ctx_localmem);
    
#endif
    
    ctx_localmem.ctx_type = KR_CTX_LOCALMEM;
}

/**
 * Local Memory Allocation and Free
 */
void *ARMCI_Malloc_local(armci_size_t bytes) {
    return (void *)kr_malloc((size_t)bytes, &ctx_localmem);
}

int ARMCI_Free_local(void *ptr) {
    kr_free((char *)ptr, &ctx_localmem);
    return 0;
}

#ifdef REGION_ALLOC
static  context_t ctx_region_shmem;
static  long *reg_pids=NULL;

void armci_region_shm_malloc(void *ptr_arr[], size_t bytes)
{
    long size=bytes;
    void *ptr;
    int i, peers=armci_clus_last-armci_clus_first+1;
    extern void* armci_region_getcore(size_t);
    extern int armci_region_register(int p, void **pinout, long pid, size_t bytes);

    if(!reg_pids){
       kr_malloc_init(0,0,500*1024*1024, armci_region_getcore, 0,
                      &ctx_region_shmem);
       reg_pids = (long*)calloc(peers,sizeof(long));
       reg_pids[armci_me -armci_clus_first] = getpid();
       armci_msg_gop_scope(SCOPE_NODE,reg_pids, peers,"+",ARMCI_LONG);
    }

    ptr=kr_malloc((size_t)size, &ctx_region_shmem);
    if(bytes) if(!ptr) armci_die("armci_region_shm_malloc: failed",bytes);

    bzero((char*)ptr_arr,armci_nproc*sizeof(void*));
    ptr_arr[armci_me] = ptr;

    /* now combine individual addresses into a single array */
    armci_exchange_address(ptr_arr, armci_nproc);

    for(i=0; i<peers; i++)
       if(i+armci_clus_first == armci_me) continue;
       else if(ptr_arr[i+armci_clus_first])armci_region_register(i,ptr_arr+i+armci_clus_first,reg_pids[i], bytes);
}

#ifdef MPI
void armci_region_shm_malloc_grp(void *ptr_arr[], size_t bytes, ARMCI_Group *group)
{
    long size=bytes;
    void *ptr;
    armci_grp_attr_t *grp_attr=ARMCI_Group_getattr(group);
    int grp_me, grp_nproc, grp_clus_me=grp_attr->grp_clus_me;
    int i, peers=grp_attr->grp_clus_info[grp_clus_me].nslave;
    int grp_clus_first=grp_attr->grp_clus_info[grp_clus_me].master;
    extern void* armci_region_getcore(size_t);
    extern int armci_region_register(int p, void **pinout, long pid, size_t bytes);

    ARMCI_Group_rank(group, &grp_me);
    ARMCI_Group_size(group, &grp_nproc);

    ptr=kr_malloc((size_t)size, &ctx_region_shmem);
    if(bytes) if(!ptr) armci_die("armci_region_shm_malloc_grp: failed",bytes);

    bzero((char*)ptr_arr,grp_nproc*sizeof(void*));
    ptr_arr[grp_me] = ptr;

    /* now combine individual addresses into a single array */
    armci_exchange_address_grp(ptr_arr, grp_nproc, group);

    for(i=0; i<peers; i++)
       if(i+grp_clus_first == grp_me) continue;
       else if(ptr_arr[i+grp_clus_first]) {
          int p;
          /* get global peer id within SMP */
          p = (ARMCI_Absolute_id(group, grp_clus_first+i) - armci_clus_first);
          armci_region_register(p,ptr_arr+i+grp_clus_first,reg_pids[p], bytes);
       }
}
#endif /* ifdef MPI */
#endif


/*\ Collective Memory Allocation
 *  returns array of pointers to blocks of memory allocated by everybody
 *  Note: as the same shared memory region can be mapped at different locations
 *        in each process address space, the array might hold different values
 *        on every process. However, the addresses are legitimate
 *        and can be used in the ARMCI data transfer operations.
 *        ptr_arr[nproc]
\*/
int ARMCI_Malloc(void *ptr_arr[], armci_size_t bytes)
{
    void *ptr;
#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_MALLOC,__FILE__,__LINE__);
#endif

    if(DEBUG_) 
    {
       fprintf(stderr,"%d bytes in armci_malloc %d\n",armci_me, (int)bytes);
       fflush(stderr);
       armci_msg_barrier();
    }

#ifdef REGION_ALLOC
    armci_region_shm_malloc(ptr_arr, bytes);
#else
#ifdef USE_MALLOC
    if(armci_nproc == 1) {
      ptr = kr_malloc((size_t) bytes, &ctx_localmem);
      if(bytes) if(!ptr) armci_die("armci_malloc:malloc 1 failed",(int)bytes);
      ptr_arr[armci_me] = ptr;
#     ifdef GA_USE_VAMPIR
           vampir_end(ARMCI_MALLOC,__FILE__,__LINE__);
#     endif
      return (0);
    }
#endif

#ifdef SGIALTIX
    if( ARMCI_Uses_shm() ) armci_altix_shm_malloc(ptr_arr,bytes);
#else
    if( ARMCI_Uses_shm() ) armci_shmem_malloc(ptr_arr,bytes);
#endif
    else {
      /* on distributed-memory systems just malloc & collect all addresses */
      ptr = kr_malloc(bytes, &ctx_localmem);
      if(bytes) if(!ptr) armci_die("armci_malloc:malloc 2 failed",bytes);

      bzero((char*)ptr_arr,armci_nproc*sizeof(void*));
      ptr_arr[armci_me] = ptr;
      
#if defined(CRAY_SHMEM)
      armci_shmalloc_exchange_address(ptr_arr);
#else
      /* now combine individual addresses into a single array */
      armci_exchange_address(ptr_arr, armci_nproc);
#endif
#     ifdef ALLOW_PIN
         armci_global_region_exchange(ptr, (long) bytes);
#     endif
    }
#endif
#ifdef GA_USE_VAMPIR
      vampir_end(ARMCI_MALLOC,__FILE__,__LINE__);
#endif
    return(0);
}



/*\ shared memory is released to kr_malloc only on process 0
 *  with data server malloc cannot be used
\*/
int ARMCI_Free(void *ptr)
{
    if(!ptr)return 1;
#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_FREE,__FILE__,__LINE__);
#endif

#ifndef SGIALTIX
#  ifdef REGION_ALLOC
     kr_free(ptr, &ctx_region_shmem);
#  else

#    if (defined(SYSV) || defined(WIN32) || defined(MMAP)) && !defined(NO_SHM)
#       ifdef USE_MALLOC
          if(armci_nproc > 1)
#       endif
             if(ARMCI_Uses_shm()){
                if(armci_me==armci_master){
#               ifdef RMA_NEEDS_SHMEM
                   Free_Shmem_Ptr(0,0,ptr);
#               else
                   if(armci_clus_info[armci_clus_me].nslave>1)
                      Free_Shmem_Ptr(0,0,ptr);
                   else kr_free(ptr, &ctx_localmem);
#               endif
                }
                ptr = NULL;
#               ifdef GA_USE_VAMPIR
                vampir_end(ARMCI_FREE,__FILE__,__LINE__);
#               endif
                return 0;
             }
#    endif
     kr_free(ptr, &ctx_localmem);
#  endif /* REGION_ALLOC */
#else
     /* Altix */
     if( ARMCI_Uses_shm() ) kr_free(ptr, &altix_ctx_shmem);
     else kr_free(ptr, &ctx_localmem);
#endif

     ptr = NULL;
#ifdef GA_USE_VAMPIR
     vampir_end(ARMCI_FREE,__FILE__,__LINE__);
#endif
     return 0;
}


int ARMCI_Uses_shm()
{
    int uses=0;

#if (defined(SYSV) || defined(WIN32) || defined(MMAP) ||defined(HITACHI)) && !defined(NO_SHM)
#   ifdef RMA_NEEDS_SHMEM
      if(armci_nproc >1) uses= 1; /* always unless serial mode */
#   else
      if(armci_nproc != armci_nclus)uses= 1; /* only when > 1 node used */
#   endif
#endif
    if(DEBUG_) fprintf(stderr,"%d:uses shmem %d\n",armci_me, uses);
    return uses;
}
#ifdef MPI

int ARMCI_Uses_shm_grp(ARMCI_Group *group) 
{    
    int uses=0, grp_me, grp_nproc, grp_nclus;
    armci_grp_attr_t *grp_attr=ARMCI_Group_getattr(group);

    ARMCI_Group_size(group, &grp_nproc);
    ARMCI_Group_rank(group, &grp_me);
    grp_nclus = grp_attr->grp_nclus;
    
#if (defined(SYSV) || defined(WIN32) || defined(MMAP) ||defined(HITACHI)) && !defined(NO_SHM)
#   ifdef RMA_NEEDS_SHMEM
      if(grp_nproc >1) uses= 1; /* always unless serial mode */
#   else
      if(grp_nproc != grp_nclus)uses= 1; /* only when > 1 node used */
#   endif
#endif
    if(DEBUG_) fprintf(stderr,"%d (grp_id=%d):uses shmem %d\n",armci_me, grp_me, uses);
    return uses;
}

/*\ ************** Begin Group Collective Memory Allocation ******************
 *  returns array of pointers to blocks of memory allocated by everybody
 *  Note: as the same shared memory region can be mapped at different locations
 *        in each process address space, the array might hold different values
 *        on every process. However, the addresses are legitimate
 *        and can be used in the ARMCI data transfer operations.
 *        ptr_arr[nproc]
\*/
int ARMCI_Malloc_group(void *ptr_arr[], armci_size_t bytes,
                       ARMCI_Group *group)
{
    void *ptr;
    int grp_me, grp_nproc;
#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_MALLOC_GROUP,__FILE__,__LINE__);
#endif
    ARMCI_Group_size(group, &grp_nproc);
    ARMCI_Group_rank(group, &grp_me);
    if(DEBUG_)fprintf(stderr,"%d (grp_id=%d) bytes in armci_malloc_group %d\n",
                      armci_me, grp_me, (int)bytes);
#ifdef REGION_ALLOC
    armci_region_shm_malloc_grp(ptr_arr, bytes, group);
#else 
#ifdef USE_MALLOC
    if(grp_nproc == 1) {
       ptr = kr_malloc((size_t) bytes, &ctx_localmem);
       if(bytes) if(!ptr) armci_die("armci_malloc_group:malloc 1 failed",(int)bytes);
       ptr_arr[grp_me] = ptr;
#      ifdef GA_USE_VAMPIR
            vampir_end(ARMCI_MALLOC_GROUP,__FILE__,__LINE__);
#      endif
       return (0);
    }
#endif
    
    if( ARMCI_Uses_shm_grp(group) ) {
#      ifdef SGIALTIX
          armci_altix_shm_malloc_group(ptr_arr,bytes,group);
#      else   
          armci_shmem_malloc_group(ptr_arr,bytes,group);
#      endif
    }
    else {
       /* on distributed-memory systems just malloc & collect all addresses */
       ptr = kr_malloc(bytes, &ctx_localmem);
       if(bytes) if(!ptr) armci_die("armci_malloc:malloc 2 failed",bytes);
       
       bzero((char*)ptr_arr,grp_nproc*sizeof(void*));
       ptr_arr[grp_me] = ptr;
       
       /* now combine individual addresses into a single array */
#if defined(CRAY_SHMEM)
       armci_shmalloc_exchange_address_grp(ptr_arr, group);
#else
       armci_exchange_address_grp(ptr_arr, grp_nproc, group);
#endif
      
#      ifdef ALLOW_PIN
#         if 0
             /* ????? check ?????? */
             armci_die("armci_malloc_group: Not yet implemented", 0);
             armci_global_region_exchange(ptr, (long) bytes);
#         endif
#      endif
    }
#endif
#ifdef GA_USE_VAMPIR
    vampir_end(ARMCI_MALLOC_GROUP,__FILE__,__LINE__);
#endif
    return(0);
}


/*\ shared memory is released to kr_malloc only on process 0
 *  with data server malloc cannot be used
 \*/
int ARMCI_Free_group(void *ptr, ARMCI_Group *group)
{
    int grp_me, grp_nproc, grp_master, grp_clus_me;
    armci_grp_attr_t *grp_attr=ARMCI_Group_getattr(group);
    
    if(!ptr)return 1;
#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_FREE_GROUP,__FILE__,__LINE__);
#endif

    ARMCI_Group_size(group, &grp_nproc);
    ARMCI_Group_rank(group, &grp_me);
    if(grp_me == MPI_UNDEFINED) { /* check if the process is in this group */
       armci_die("armci_malloc_group: process is not a member in this group",
                 armci_me);
    }
    /* get the group cluster info */
    grp_clus_me    = grp_attr->grp_clus_me;
    grp_master     = grp_attr->grp_clus_info[grp_clus_me].master;

#ifndef SGIALTIX
#ifdef REGION_ALLOC
    kr_free(ptr, &ctx_region_shmem);
#else
#   if (defined(SYSV) || defined(WIN32) || defined(MMAP)) && !defined(NO_SHM)
#      ifdef USE_MALLOC
         if(grp_nproc > 1)
#      endif
       if(ARMCI_Uses_shm_grp(group)){
          if(grp_me == grp_master) {
#            ifdef RMA_NEEDS_SHMEM
             Free_Shmem_Ptr(0,0,ptr);
#            else
             if(armci_clus_info[armci_clus_me].nslave>1) Free_Shmem_Ptr(0,0,ptr);
             else kr_free(ptr, &ctx_localmem);
#            endif
          }
          ptr = NULL;
#         ifdef GA_USE_VAMPIR
          vampir_end(ARMCI_FREE_GROUP,__FILE__,__LINE__);
#         endif
          return 0;
       }
#   endif
    kr_free(ptr, &ctx_localmem);
#endif /* ifdef REGION_ALLOC */
#else /* SGI Altix */
    if(ARMCI_Uses_shm_grp(group)) 
       kr_free(ptr, &altix_ctx_shmem_grp);
    else kr_free(ptr, &ctx_localmem);
       
#endif /* SGIALTIX */

    ptr = NULL;
#ifdef GA_USE_VAMPIR
    vampir_end(ARMCI_FREE_GROUP,__FILE__,__LINE__);
#endif
    return 0;
}
/* ***************** End Group Collective Memory Allocation ******************/
#endif

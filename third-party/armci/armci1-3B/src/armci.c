/* $Id$ */

/* DISCLAIMER
 *
 * This material was prepared as an account of work sponsored by an
 * agency of the United States Government.  Neither the United States
 * Government nor the United States Department of Energy, nor Battelle,
 * nor any of their employees, MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR
 * ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY,
 * COMPLETENESS, OR USEFULNESS OF ANY INFORMATION, APPARATUS, PRODUCT,
 * SOFTWARE, OR PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT
 * INFRINGE PRIVATELY OWNED RIGHTS.
 *
 *
 * ACKNOWLEDGMENT
 *
 * This software and its documentation were produced with United States
 * Government support under Contract Number DE-AC06-76RLO-1830 awarded by
 * the United States Department of Energy.  The United States Government
 * retains a paid-up non-exclusive, irrevocable worldwide license to
 * reproduce, prepare derivative works, perform publicly and display
 * publicly by or for the US Government, including the right to
 * distribute to other US Government contractors.
 */

#define  EXTERN
#include <stdio.h>
#include <stdlib.h>
#if defined(CRAY) && !defined(__crayx1)
#  include <sys/category.h>
#  include <sys/resource.h>
#  include <unistd.h>
#endif
#ifdef LAPI
#  include "lapidefs.h"
#endif
#include <errno.h>
#include "armcip.h"
#include "copy.h"
#include "memlock.h"
#include "shmem.h"
#include "signaltrap.h"

#ifdef GA_USE_VAMPIR
#include "armci_vampir.h"
#endif
#ifdef ARMCI_PROFILE
#include "armci_profile.h"
#endif
#ifdef BGML
#include "bgml.h"
#include <assert.h>
#include "bgmldefs.h"
extern void armci_msg_barrier(void);
#endif

#ifdef CRAY_SHMEM
#  ifdef XT3
#    include <mpp/shmem.h>
#  else
#    include <shmem.h>
#  endif
#endif

/* global variables */
int armci_me, armci_nproc;
int armci_clus_me, armci_nclus, armci_master;
int armci_clus_first, armci_clus_last;
int _armci_initialized=0;
int _armci_terminating =0;
thread_id_t armci_usr_tid;
armci_ireq_t armci_inb_handle[ARMCI_MAX_IMPLICIT];/*implicit non-blocking handle*/
#ifndef HITACHI
double armci_internal_buffer[BUFSIZE_DBL];
#endif
#if defined(SYSV) || defined(WIN32) || defined(MMAP) || defined(HITACHI) || defined(CATAMOUNT) || defined(BGML)
#   include "locks.h"
    lockset_t lockid;
#endif

#ifdef ALLOW_PIN
int* armci_prot_switch_fence=NULL;
int armci_prot_switch_preproc = -1;
int armci_prot_switch_preop = -1;
#endif

#ifdef BGML
//   void armci_allocate_locks();
   void armci_init_memlock();
#endif

#ifdef LIBELAN_ATOMICS
ELAN_ATOMIC *a;
#warning "Enabling new atomics"
#endif

typedef struct{
  int sent;
  int received;
  int waited;
}armci_notify_t;

armci_notify_t **_armci_notify_arr;

void ARMCI_Cleanup()
{
#ifdef GA_USE_VAMPIR
  vampir_begin(ARMCI_CLEANUP,__FILE__,__LINE__);
#endif
#if (defined(SYSV) || defined(WIN32) || defined(MMAP))&& !defined(HITACHI) 
    Delete_All_Regions();
    if(armci_nproc>1)
#if !defined(LAPI) 
       DeleteLocks(lockid);
#endif

    /* in case of an error notify server that it is time to quit */
#if defined(DATA_SERVER)
    if(armci_nclus >1){
        /* send quit request to server unless it is already dead */
        armci_wait_for_server();
        armci_transport_cleanup();
    }
#endif
#ifndef WIN32
    ARMCI_RestoreSignals();
#endif
#endif
#ifdef GA_USE_VAMPIR
  vampir_end(ARMCI_CLEANUP,__FILE__,__LINE__);
#endif
}


void armci_notify_init()
{
  int rc,bytes=sizeof(armci_notify_t)*armci_nproc;

#ifdef DOELAN4
  armci_elan_notify_init();
  return;
#endif

  _armci_notify_arr=
        (armci_notify_t**)malloc(armci_nproc*sizeof(armci_notify_t*));
  if(!_armci_notify_arr)armci_die("armci_notify_ini:malloc failed",armci_nproc);

  if((rc=ARMCI_Malloc((void **)_armci_notify_arr, bytes))) 
        armci_die(" armci_notify_init: armci_malloc failed",bytes); 
  bzero(_armci_notify_arr[armci_me], bytes);
}


static void armci_perror_msg()
{
    char perr_str[80];
    if(!errno)return;
    sprintf(perr_str,"Last System Error Message from Task %d:",armci_me);
    perror(perr_str);
}


#if defined(IBM) || defined(IBM64)
int AR_caught_sigint;
int AR_caught_sigterm;
#endif

static void armci_abort(int code)
{
#if !defined(BGML)
    armci_perror_msg();
#endif
    ARMCI_Cleanup();
#if defined(CRAY) && !defined(__crayx1)
    limit(C_PROC,0,L_CORE,1L); /* MPI_Abort on Cray dumps core!!! - sqeeze it */
    chdir("/"); /* we should not be able to write core file here */
#endif

    /* data server process cannot use message-passing library to abort
     * it simply exits, parent will get SIGCHLD and abort the program
     */
#if defined(IBM) || defined(IBM64)
     /* hack for a problem in POE signal handlers in non-LAPI MPI  */
     if(AR_caught_sigint || AR_caught_sigterm) _exit(1);
#endif

#if defined(DATA_SERVER)
    if(armci_me<0)_exit(1);
    else
#endif
    armci_msg_abort(code);
}


void armci_die(char *msg, int code)
{
    if(_armci_terminating)return;
    else _armci_terminating=1;

    if(SERVER_CONTEXT){
       fprintf(stdout,"%d(s):%s: %d\n",armci_me, msg, code); fflush(stdout);
       fprintf(stderr,"%d(s):%s: %d\n",armci_me, msg, code);
    }else{
      fprintf(stdout,"%d:%s: %d\n",armci_me, msg, code); fflush(stdout);
      fprintf(stderr,"%d:%s: %d\n",armci_me, msg, code);
    }
    armci_abort(code);
}


void armci_die2(char *msg, int code1, int code2)
{
    if(_armci_terminating)return;
    else _armci_terminating=1;

    if(SERVER_CONTEXT){
      fprintf(stdout,"%d(s):%s: (%d,%d)\n",armci_me,msg,code1,code2);
      fflush(stdout);
      fprintf(stderr,"%d(s):%s: (%d,%d)\n",armci_me,msg,code1,code2);
    }else{
      fprintf(stdout,"%d:%s: (%d,%d)\n",armci_me,msg,code1,code2);
      fflush(stdout);
      fprintf(stderr,"%d:%s: (%d,%d)\n",armci_me,msg,code1,code2);
    }

    armci_abort(code1);
}


void ARMCI_Error(char *msg, int code)
{
    armci_die(msg,code);
}


void armci_allocate_locks()
{
    /* note that if ELAN_ACC is defined the scope of locks is limited to SMP */
#if !defined(CRAY_SHMEM) && (defined(HITACHI) || defined(CATAMOUNT) || \
    (defined(QUADRICS) && defined(_ELAN_LOCK_H) && !defined(ELAN_ACC)))
       armcill_allocate_locks(NUM_LOCKS);
#elif (defined(SYSV) || defined(WIN32) || defined(MMAP)) && !defined(HITACHI)
       if(armci_nproc == 1)return;
#  if defined(SPINLOCK) || defined(PMUTEXES)
       CreateInitLocks(NUM_LOCKS, &lockid);
#  else
       if(armci_master==armci_me)CreateInitLocks(NUM_LOCKS, &lockid);
       armci_msg_clus_brdcst(&lockid, sizeof(lockid));
       if(armci_master != armci_me)InitLocks(NUM_LOCKS, lockid);
#  endif
#endif
}


void ARMCI_Set_shm_limit(unsigned long shmemlimit)
{
#if (defined(SYSV) || defined(WIN32)  || defined(MMAP)) && !defined(HITACHI)
#define EXTRASHM  1024   /* extra shmem used internally in ARMCI */
unsigned long limit;
    limit = armci_clus_info[armci_clus_me].nslave * shmemlimit + EXTRASHM;
    armci_set_shmem_limit(limit);
#endif
}


 
/*\ allocate and initialize memory locking data structure
\*/
void armci_init_memlock()
{
    int bytes = MAX_SLOTS*sizeof(memlock_t);
    int rc, msize_per_proc=bytes;
    
#ifdef MEMLOCK_SHMEM_FLAG    
    /* last proc on node allocates memlock flag in shmem */
    if(armci_clus_last == armci_me) bytes += sizeof(int);
#endif

    memlock_table_array = malloc(armci_nproc*sizeof(void*));
    if(!memlock_table_array) armci_die("malloc failed for ARMCI lock array",0);

    rc = ARMCI_Malloc(memlock_table_array, bytes);
    if(rc) armci_die("failed to allocate ARMCI memlock array",rc);

    armci_msg_barrier();

    bzero(memlock_table_array[armci_me],bytes);

#ifdef BGML
    bgml_init_locks ((void *) memlock_table_array[armci_me]);
#endif


#ifdef MEMLOCK_SHMEM_FLAG    
    /* armci_use_memlock_table is a pointer to local memory variable=1
     * we overwrite the pointer with address of shared memory variable 
     * armci_use_memlock_table and initialize it >0
     */
    armci_use_memlock_table = (int*) (msize_per_proc + 
                              (char*) memlock_table_array[armci_clus_last]);  
                              
    /* printf("%d: last=%d bytes=%d ptr =(%d, %d)\n",
           armci_me,armci_clus_last,bytes,armci_use_memlock_table,  
           memlock_table_array[armci_clus_last]); fflush(stdout); */

    if(armci_clus_last == armci_me) *armci_use_memlock_table =1+armci_me;

#endif

    armci_msg_barrier();
}


#if defined(SYSV) || defined(WIN32)
static void armci_check_shmmax()
{
  long mylimit, limit;
  mylimit = limit = (long) armci_max_region();
  armci_msg_bcast_scope(SCOPE_MASTERS, &limit, sizeof(long), 0);
  if(mylimit != limit){
     printf("%d:Shared mem limit in ARMCI is %ld bytes on node %s vs %ld on %s\n",
            armci_me,mylimit<<10,armci_clus_info[armci_clus_me].hostname,
            limit<<10, armci_clus_info[0].hostname);
     fflush(stdout); sleep(1);
     armci_die("All nodes must have the same SHMMAX limit if NO_SHM is not defined",0);
  }
}
#endif

#ifdef MPI
ARMCI_Group armci_world_group;
void armci_create_world_group()
{
    int i, list[MAX_PROC];
    for(i=0;i<armci_nproc;i++)list[i] = i;
    ARMCI_Group_create(armci_nproc,list,&armci_world_group);
}
ARMCI_Group *ARMCI_Get_world_group()
{
    return(&armci_world_group);
}
#endif

void* test_ptr_arr[MAX_PROC];
extern void armci_region_shm_malloc(void *ptr_arr[], size_t bytes);

#ifdef DO_CKPT
int armci_ft_spare_procs;
void armci_set_spare_procs(int spare)
{
    armci_ft_spare_procs = spare;
}
ARMCI_Group armci_ft_group;
ARMCI_Group *ARMCI_Get_ft_group()
{
    return(&armci_ft_group);
}
void armci_create_ft_group()
{
    int i, list[MAX_PROC];
    for(i=0;i<armci_nproc-armci_ft_spare_procs;i++)list[i] = i;
    printf("\n%d:here ok\n",armci_me);fflush(stdout);
    ARMCI_Group_create((armci_nproc-armci_ft_spare_procs),list,&armci_ft_group);
    printf("\n%d:done with group create\n",armci_me);fflush(stdout);
}
#endif

int ARMCI_Init()
{
    if(_armci_initialized>0) return 0;
#ifdef GA_USE_VAMPIR
    vampir_init(NULL,NULL,__FILE__,__LINE__);
    armci_vampir_init(__FILE__,__LINE__);
    vampir_begin(ARMCI_INIT,__FILE__,__LINE__);
#endif
#ifdef BGML
    BGML_Messager_Init();
    BG1S_Configuration_t config;
    config=BG1S_Configure(NULL);
    config.consistency= BG1S_ConsistencyModel_Weak;
    BG1S_Configure(&config);

    unsigned long long available = BGML_Messager_available();
    if (available & BGML_MESSAGER_GI)
      bgml_barrier = (BGML_Barrier) BGGI_Barrier;
    else
      bgml_barrier = (BGML_Barrier) BGTr_Barrier;

#endif

    armci_nproc = armci_msg_nproc();
    armci_me = armci_msg_me();
    armci_usr_tid = THREAD_ID_SELF(); /*remember the main user thread id */

#ifdef _CRAYMPP
    cmpl_proc=-1;
#endif
#ifdef LAPI
#   ifdef AIX
    {
       char *tmp1 = getenv("RT_GRQ"), *tmp2 = getenv("AIXTHREAD_SCOPE");
       if(tmp1 == NULL || strcmp((const char *)tmp1,"ON")) 
	  armci_die("Armci_Init: environment variable RT_GRQ not set. It should be set as RT_GRQ=ON, to restore original thread scheduling LAPI relies upon",0);
       if(tmp2 == NULL || strcmp((const char *)tmp2,"S")) 
	  armci_die("Armci_Init: environment variable AIXTHREAD_SCOPE=S should be set to assure correct operation of LAPI", 0);
    }
#   endif
    armci_init_lapi();
#endif

#ifdef PORTALS
    armci_init_portals();
#endif

#ifdef QUADRICS
#   ifdef DECOSF
    {
       char *tmp = getenv("SHMEM_SMP_ENABLE");
       if(tmp == NULL || strcmp((const char *)tmp,"0"))
           printf("WARNING: On Tru64 (Compaq Alphaserver) it might be required to set the Quadrics environment variable SHMEM_SMP_ENABLE=0 as a work around for shmem_fadd problem.\n");
    }
#   endif

    /* Ensure we're linked with compatible software */
    if (!elan_checkVersion (ELAN_VERSION))
        fprintf(stderr,
                "libelan version '%s' incompatible with '%s' ('%s' expected)",
                ELAN_VERSION, elan_version(), ELAN_VERSION);

#ifdef QSNETLIBS_VERSION_CODE
#   if QSNETLIBS_VERSION_CODE < QSNETLIBS_VERSION(1,4,6)
       elan_baseInit();
#   else
       elan_baseInit(0);
#   endif
#endif

#   ifdef LIBELAN_ATOMICS
    {
        ELAN_QUEUE *q = elan_gallocQueue(elan_base, elan_base->allGroup);
        a = elan_atomicInit(elan_base->state, q, 16, 0);
    }
#   else
       shmem_init();
#   endif
#endif /* QUADRICS */

#ifdef CRAY_SHMEM
    shmem_init();
#endif

    armci_init_clusinfo();
#ifdef MPI
    armci_create_world_group();
#endif
    armci_krmalloc_init_localmem();

#ifndef BLRTS
    /* trap signals to cleanup ARMCI system resources in case of crash */
    if(armci_me==armci_master) ARMCI_ParentTrapSignals();
    ARMCI_ChildrenTrapSignals();
#endif

    /* init K&R memory for CRAY-XT3 */
#if defined(CRAY_SHMEM) && defined(XT3)
    armci_cray_shm_init();
#endif

#if defined(SYSV) || defined(WIN32) || defined(MMAP)
    /* init shared/K&R memory */
    if(ARMCI_Uses_shm() ) {
#      ifdef SGIALTIX
          armci_altix_shm_init();
#      else
          armci_shmem_init();
#      endif
    }

#   if defined(QUADRICS) && !defined(NO_SHM)
       if(armci_me == armci_master)armci_check_shmmax();
#   endif
#endif

#ifdef REGION_ALLOC
       ARMCI_Malloc(test_ptr_arr,256*1024*1024);
#if 0
       {
          int i;
          armci_region_shm_malloc(test_ptr_arr, 100000000);
          *(long*)test_ptr_arr[armci_me]=10000+armci_me;
          armci_msg_barrier();
          for(i = 0; i < armci_clus_last -armci_clus_first+1; i++){
             printf("for %d got ptr is %p\n", armci_clus_first+i,(long*)test_ptr_arr[i+armci_clus_first]);
             printf("for %d got %ld\n",armci_clus_first+i, *(long*)test_ptr_arr[i+armci_clus_first]);
          }
          armci_msg_barrier();
       }
#endif
       ARMCI_Free(test_ptr_arr[armci_me]);
#endif

#ifdef MULTI_CTX
    /* this is a hack for the Elan-3 multi-tiled memory (qsnetlibs v 1.4.10) 
     * we need to allocate and then free memory to satisfy libelan requirements
     * for symmetric memory addresses
     */ 
    if(armci_nclus >1){ 
       int segments, segsize, seg;
       void **addr;
       armci_nattach_preallocate_info(&segments, &segsize);

       segsize -= 1024*1024; /* leave some for the K&RM headers */
       if(armci_me!=armci_master)segsize=0; /* only one allocates mem on node*/

       addr = (void*) malloc(segments*armci_nproc*sizeof(void*));
       if(!addr)armci_die("armci_init:addr malloc failed",segments*armci_nproc);

       for(seg=0; seg< segments; seg++) /* allocate segments */
          if(ARMCI_Malloc(addr+armci_nproc*seg,segsize))
             armci_die("problem in Elan-3 mem preallocation",seg);
       
       for(seg=0; seg< segments; seg++) /* return to free pool */
         if(armci_me==armci_master)
           if(ARMCI_Free(*(addr+armci_nproc*seg+armci_me)))
              armci_die("problem in Elan-3 mem preallocation - free stage",seg);
       free(addr);

#if 0
       if(armci_me==armci_master){
          printf("%d:preallocated %d segments %d bytes each\n",armci_me,
                 segments, segsize); fflush(stdout);
       }
#endif

    }
#endif

    /* allocate locks: we need to do it before server is started */
    armci_allocate_locks();
    armci_init_fence();

#ifdef ARMCI_ENABLE_GPC_CALLS
    gpc_init_signals();
#endif

#ifdef ALLOW_PIN
    armci_prot_switch_fence = malloc(sizeof(int*)*armci_nproc);
    armci_prot_switch_preproc = -1;
    armci_prot_switch_preop = -1;
#endif

    /* NOTE: FOR PROCESS-BASED DATA SERVER WE CANNOT call ARMCI_Malloc yet */

#   if defined(DATA_SERVER) || defined(ELAN_ACC)
       if(armci_nclus >1) armci_start_server();
#   endif
#if defined(GM) || defined(VAPI) || defined (PORTALS)
    /* initialize registration of memory */
    armci_region_init();
#endif

    armci_msg_barrier();
    armci_init_memlock(); /* allocate data struct for locking memory areas */
#if !defined(GM) 
    armci_notify_init();
#endif
    armci_msg_barrier();
    armci_msg_gop_init();

#ifdef ARMCI_PROFILE
    armci_profile_init();
#endif
#ifdef GA_USE_VAMPIR
    vampir_end(ARMCI_INIT,__FILE__,__LINE__);
#endif    

    _armci_initialized++;
#ifdef DO_CKPT
    armci_init_checkpoint(armci_ft_spare_procs);
#endif
    return 0;
}


void ARMCI_Finalize()
{
    _armci_initialized--;
    if(_armci_initialized)return;
#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_FINALIZE,__FILE__,__LINE__);
#endif
#ifdef ARMCI_PROFILE
    armci_profile_terminate();
#endif

    _armci_terminating =1;;
    armci_msg_barrier();
    if(armci_me==armci_master) ARMCI_ParentRestoreSignals();

#if defined(DATA_SERVER)
    if(armci_nclus >1){
       armci_wait_for_server();
       armci_msg_barrier(); /* need to sync before closing sockets */
    }
#endif

#ifdef PORTALS
    armci_fini_portals();
#endif
#ifdef ALLOW_PIN
    free(armci_prot_switch_fence);
#endif
    ARMCI_Cleanup();
    armci_msg_barrier();
#ifdef GA_USE_VAMPIR
    vampir_end(ARMCI_FINALIZE,__FILE__,__LINE__);
    vampir_finalize(__FILE__,__LINE__);
#endif
}


#if !(defined(SYSV) || defined(WIN32))
void ARMCI_Set_shmem_limit(unsigned long shmemlimit)
{
   /* not applicable here
    * aborting would  make user's life harder
    */
}
#endif



void ARMCI_Copy(void *src, void *dst, int n)
{
#ifdef GA_USE_VAMPIR
 vampir_begin(ARMCI_COPY,__FILE__,__LINE__);
#endif
 armci_copy(src,dst,n);
#ifdef GA_USE_VAMPIR
 vampir_end(ARMCI_COPY,__FILE__,__LINE__);
#endif
}

extern void cpu_yield();
void armci_util_wait_int(volatile int *p, int val, int maxspin)
{
int count=0;
extern void cpu_yield();
       while(*p != val)
            if((++count)<maxspin) armci_util_spin(count,(int *)p);
            else{
               cpu_yield();
               count =0;
#if defined(MACX) && defined(__ppc__) && defined(__GNUC__)
               __asm__ __volatile__ ("sync" ::: "memory");
#endif
            }
}
  

/*\ returns 1 if specified process resides on the same smp node as calling task 
\*/
int ARMCI_Same_node(int proc)
{
   int direct=SAMECLUSNODE(proc);
   return direct;
}

/*\ blocks the calling process until a nonblocking operation represented
 *  by the user handle completes
\*/
int ARMCI_Wait(armci_hdl_t* usr_hdl){
armci_ihdl_t nb_handle = (armci_ihdl_t)usr_hdl;
int success=0;
int direct=SAMECLUSNODE(nb_handle->proc);
#ifdef ARMCI_PROFILE
    armci_profile_start(ARMCI_PROF_WAIT);
#endif

#ifdef BGML
    assert(nb_handle->cmpl_info);
    BGML_Wait(&(nb_handle->count));
#ifdef ARMCI_PROFILE
    armci_profile_stop(ARMCI_PROF_WAIT);
#endif
    return(success);
#else

    if(direct) {
#      ifdef ARMCI_PROFILE
       armci_profile_stop(ARMCI_PROF_WAIT);
#      endif
       return(success);
    }
    if(nb_handle) {
      if(nb_handle->agg_flag) {
        armci_agg_complete(nb_handle, UNSET);
#       ifdef ARMCI_PROFILE
        armci_profile_stop(ARMCI_PROF_WAIT);
#       endif
        return (success);
      }
    }

    if(nb_handle){

#     ifdef ARMCI_NB_WAIT

        if(nb_handle->tag==0){
              ARMCI_NB_WAIT(nb_handle->cmpl_info);
#             ifdef ARMCI_PROFILE
              armci_profile_stop(ARMCI_PROF_WAIT);
#             endif
              return(success);
        }
#       if defined(LAPI) || defined(ALLOW_PIN)
         if(nb_handle->tag!=0 && nb_handle->bufid==NB_NONE){
               ARMCI_NB_WAIT(nb_handle->cmpl_info);
#              ifdef ARMCI_PROFILE
               armci_profile_stop(ARMCI_PROF_WAIT);
#              endif
               return(success);
         }
#       endif

#ifdef PORTALS
         armci_portals_complete(nb_handle);
#endif

#     endif
#     ifdef COMPLETE_HANDLE
       COMPLETE_HANDLE(nb_handle->bufid,nb_handle->tag,(&success));
#     endif
    }
#endif

#ifdef ARMCI_PROFILE
    armci_profile_stop(ARMCI_PROF_WAIT);
#endif
    return(success);
}

/** 
 * implicit handle 
 */
static char hdl_flag[ARMCI_MAX_IMPLICIT];
static int impcount=0;
armci_ihdl_t armci_set_implicit_handle (int op, int proc) {
 
  int i=impcount%ARMCI_MAX_IMPLICIT;
  if(hdl_flag[i]=='1')
    ARMCI_Wait((armci_hdl_t*)&armci_inb_handle[i]);

#ifdef BGML
   armci_inb_handle[i].count=0;
#endif
  armci_inb_handle[i].tag   = GET_NEXT_NBTAG();
  armci_inb_handle[i].op    = op;
  armci_inb_handle[i].proc  = proc;
  armci_inb_handle[i].bufid = NB_NONE;
  armci_inb_handle[i].agg_flag = 0;
  hdl_flag[i]='1';
  ++impcount;
  return &armci_inb_handle[i];
}
 
 
/* wait for all non-blocking operations to finish */
int ARMCI_WaitAll (void) {
#ifdef BGML
  BGML_WaitAll();
#else
  int i;
  if(impcount) {
    for(i=0; i<ARMCI_MAX_IMPLICIT; i++) {
      if(hdl_flag[i] == '1') {
        ARMCI_Wait((armci_hdl_t*)&armci_inb_handle[i]);
        hdl_flag[i]='0';
      }
    }
  }
  impcount=0;
#endif
  return 0;
}
 
/* wait for all non-blocking operations to a particular process to finish */
int ARMCI_WaitProc (int proc) {
#ifdef BGML
  BGML_WaitProc(proc);
#else
  int i;
  if(impcount) {
    for(i=0; i<ARMCI_MAX_IMPLICIT; i++) {
      if(hdl_flag[i]=='1' && armci_inb_handle[i].proc==proc) {
        ARMCI_Wait((armci_hdl_t*)&armci_inb_handle[i]);
        hdl_flag[i]='0';
      }
    }
  }
#endif
  return 0;
}

static unsigned int _armci_nb_tag=0;
unsigned int _armci_get_next_tag(){
    return((++_armci_nb_tag));
}

void ARMCI_SET_AGGREGATE_HANDLE(armci_hdl_t* nb_handle) { 
      ((armci_ihdl_t)(nb_handle))->agg_flag = 1;
      ((armci_ihdl_t)(nb_handle))->proc = -1;
}
 
void ARMCI_UNSET_AGGREGATE_HANDLE(armci_hdl_t* nb_handle) {
      ((armci_ihdl_t)(nb_handle))->agg_flag = 0;
      ((armci_ihdl_t)(nb_handle))->proc = -1;
}

int armci_notify(int proc)
{
#ifdef DOELAN4
  if(proc==armci_me){
    return 0;
  }
#endif
#if defined(GM) || (defined(DOELAN4) && defined(ELAN_ACC))
  {
    extern int armci_inotify_proc(int);
    return(armci_inotify_proc(proc));
  }
#else
   armci_notify_t *pnotify = _armci_notify_arr[armci_me]+proc;
   pnotify->sent++;
# ifdef MEM_FENCE
   if(SAMECLUSNODE(proc)) MEM_FENCE;
# endif
   ARMCI_Put(&pnotify->sent,&(_armci_notify_arr[proc]+armci_me)->received, 
             sizeof(pnotify->sent),proc);
   return(pnotify->sent);
#endif
}


/*\ blocks until received count becomes >= waited count
 *  return received count and store waited count in *pval
\*/
int armci_notify_wait(int proc,int *pval)
{
  int retval;
#ifdef ARMCI_PROFILE
  armci_profile_start(ARMCI_PROF_NOTIFY);
#endif
#ifdef DOELAN4
  if(proc==armci_me){
#ifdef MEM_FENCE
       MEM_FENCE;
#endif
#ifdef ARMCI_PROFILE
    armci_profile_stop(ARMCI_PROF_NOTIFY);
#endif
    return 0;
  }
#endif

#if defined(GM) || (defined(DOELAN4) && defined(ELAN_ACC))
  {
     extern int armci_inotify_wait(int,int*);
     retval=armci_inotify_wait(proc,pval);
  }
#else
  {
     long loop=0;
     armci_notify_t *pnotify = _armci_notify_arr[armci_me]+proc;
     pnotify->waited++;
     while( pnotify->waited > pnotify->received) {
         if(++loop == 1000) { loop=0;cpu_yield(); }
         armci_util_spin(loop, pnotify);
     }
     *pval = pnotify->waited;
     retval=pnotify->received;
  }
#endif

#ifdef ARMCI_PROFILE
  armci_profile_stop(ARMCI_PROF_NOTIFY);
#endif
  return retval;
}

long armci_util_long_getval(long* p)
{
   return *p;
}

int armci_util_int_getval(int* p)
{
   return *p;
}


int ARMCI_Test(armci_hdl_t *usr_hdl)
{
armci_ihdl_t nb_handle = (armci_ihdl_t)usr_hdl;
int success=0;
#ifdef BGML
   success=(int)nb_handle->count;
#else
int direct=SAMECLUSNODE(nb_handle->proc);
   if(direct)return(success);
    if(nb_handle) {
      if(nb_handle->agg_flag) {
         armci_die("test for aggregate handle not yet implemented\n",0);
      }
    }
    if(nb_handle){
#     ifdef ARMCI_NB_TEST
        if(nb_handle->tag==0){
              ARMCI_NB_TEST(nb_handle->cmpl_info,&success);
              return(success);
        }
#       ifdef LAPI
         if(nb_handle->tag!=0 && nb_handle->bufid==NB_NONE){
               ARMCI_NB_TEST(nb_handle->cmpl_info,&success);
               return(success);
         }
#       endif
#     endif
#     ifdef TEST_HANDLE
       TEST_HANDLE(nb_handle->bufid,nb_handle->tag,(&success));
#     endif
    }
#endif
    return(success);
}

#ifdef DO_CKPT
void ARMCI_Ckpt_create_ds(armci_ckpt_ds_t *ckptds, int count)
{
    armci_create_ckptds(ckptds,count);
}

int ARMCI_Ckpt_init(char *filename, ARMCI_Group *grp, int savestack, int saveheap, armci_ckpt_ds_t *ckptds)
{
int rid;
    rid = armci_icheckpoint_init(filename,grp,savestack,saveheap,ckptds);
    return(rid);
}

int ARMCI_Ckpt(int rid)
{
    return(armci_icheckpoint(rid));
}

void ARMCI_Ckpt_Recover(int rid, int iamreplacement)
{
    armci_irecover(rid, iamreplacement);
}
void ARMCI_Ckpt_finalize(int rid)
{
    armci_icheckpoint_finalize(rid);
}
#endif
#ifdef ARMCI_ENABLE_GPC_CALLS
int armci_gpc(int hndl, int proc, void  *hdr, int hlen,  void *data,  int dlen,
              void *rhdr, int rhlen, void *rdata, int rdlen,
              armci_hdl_t* nbh) {
  armci_ihdl_t nb_handle = (armci_ihdl_t)nbh;
  armci_giov_t darr[2]; /* = {{&rhdr, &rhdr, 1, rhlen}, {&rdata, &rdata, 1, rdlen}};*/
  gpc_send_t send;
  char *ptr;

  /* initialize giov */
  darr[0].src_ptr_array = &rhdr;
  darr[0].dst_ptr_array = &rhdr;
  darr[0].ptr_array_len = 1;
  darr[0].bytes         = rhlen;

  darr[1].src_ptr_array = &rdata;
  darr[1].dst_ptr_array = &rdata;
  darr[1].ptr_array_len = 1;
  darr[1].bytes         = rdlen;

  
/*    if(hlen<0 || hlen>=ARMCI_Gpc_get_hlen()) */
/*      return FAIL2; */
/*    if(rhlen<0 || rhlen>=ARMCI_Gpc_get_hlen()) */
/*      return FAIL2; */
/*    if(dlen<0 || dlen>=ARMCI_Gpc_get_dlen())  */
/*      return FAIL2; */
/*    if(rdlen<0 || rdlen>=ARMCI_Gpc_get_dlen())  */
/*      return FAIL2; */

  if(hlen>0 && hdr==NULL) 
    return FAIL3;
  if(rhlen>0 && rhdr==NULL) 
    return FAIL3;
  if(dlen>0 && data==NULL) 
    return FAIL3;
  if(rdlen>0 && rdata==NULL) 
    return FAIL3;

  if(proc<0 || proc >= armci_nproc)
    return FAIL4;

  send.hndl = hndl;
  send.hlen = hlen;
  send.dlen = dlen;
  send.hdr = hdr;
  send.data = data;

  if(nb_handle){
    nb_handle->tag = GET_NEXT_NBTAG();
    nb_handle->op  = GET;
    nb_handle->proc= proc;
    nb_handle->bufid=NB_NONE;
  }
  else {
    ORDER(GET,proc); /*ensure ordering */      
    nb_handle = NULL;
  }  

#if defined(GM) || defined(VAPI) || defined(QUADRICS)
  if(armci_rem_gpc(GET, darr, 2, &send, proc, 1, nb_handle))
#endif
    return FAIL2;
  return 0;
}

int armci_sameclusnode(int proc) {
  return SAMECLUSNODE(proc);
}
#endif

void _armci_init_handle(armci_hdl_t *hdl)
{
    ((double *)((hdl)->data))[0]=0;
    ((double *)((hdl)->data))[1]=0;
}

#ifdef CHANGE_SERVER_AFFINITY
static inline int val_to_char(int v)
{
	if (v >= 0 && v < 10)
		return '0' + v;
	else if (v >= 10 && v < 16)
		return ('a' - 10) + v;
	else
		return -1;
}
static const char *nexttoken(const char *q, int sep)
{
	if (q)
		q = strchr(q, sep);
	if (q)
		q++;
	return q;
}

int cstr_to_cpuset(cpu_set_t * mask, const char *str)
{
	const char     *p, *q;
	q = str;
	CPU_ZERO(mask);

	while (p = q, q = nexttoken(q, ','), p) {
		unsigned int    a;	/* beginning of range */
		unsigned int    b;	/* end of range */
		unsigned int    s;	/* stride */
		const char     *c1, *c2;

		if (sscanf(p, "%u", &a) < 1)
			return 1;
		b = a;
		s = 1;

		c1 = nexttoken(p, '-');
		c2 = nexttoken(p, ',');
		if (c1 != NULL && (c2 == NULL || c1 < c2)) {
			if (sscanf(c1, "%u", &b) < 1)
				return 1;
			c1 = nexttoken(c1, ':');
			if (c1 != NULL && (c2 == NULL || c1 < c2))
				if (sscanf(c1, "%u", &s) < 1) {
					return 1;
				}
		}
		if (!(a <= b))
			return 1;
		while (a <= b) {
			CPU_SET(a, mask);
			a += s;
		}
	}

	return 0;
}

char *cpuset_to_cstr(cpu_set_t * mask, char *str)
{
	int             i;
	char           *ptr = str;
	int             entry_made = 0;

	for (i = 0; i < CPU_SETSIZE; i++) {
		if (CPU_ISSET(i, mask)) {
			int             j;
			int             run = 0;
			entry_made = 1;
			for (j = i + 1; j < CPU_SETSIZE; j++) {
				if (CPU_ISSET(j, mask))
					run++;
				else
					break;
			}
			if (!run)
				sprintf(ptr, "%d,", i);
			else if (run == 1) {
				sprintf(ptr, "%d,%d,", i, i + 1);
				i++;
			} else {
				sprintf(ptr, "%d-%d,", i, i + run);
				i += run;
			}
			while (*ptr != 0)
				ptr++;
		}
	}
	ptr -= entry_made;
	*ptr = 0;

	return str;
}

char *cpuset_to_str(cpu_set_t * mask, char *str)
{
	int             base;
	char           *ptr = str;
	char           *ret = 0;

	for (base = CPU_SETSIZE - 4; base >= 0; base -= 4) {
		char            val = 0;
		if (CPU_ISSET(base, mask))
			val |= 1;
		if (CPU_ISSET(base + 1, mask))
			val |= 2;
		if (CPU_ISSET(base + 2, mask))
			val |= 4;
		if (CPU_ISSET(base + 3, mask))
			val |= 8;
		if (!ret && val)
			ret = ptr;
		*ptr++ = val_to_char(val);
	}
	*ptr = 0;
	return ret ? ret : ptr - 1;
}
#endif


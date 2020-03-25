/*
 *  testcollperf2.c
 *  gasnet_tree_coll
 *
 *  Created by Rajesh Nishtala on 10/1/07.
 *  Copyright 2007 Berkeley UPC Group. All rights reserved.
 *
 */

/* The primary aim of this tester is to measure the performance and correctness of the various collectives
at larger message sizes. It is NOT intended as a tester to measure correctness of synchronization 
options that is covered testcoll
*/

#include <gasnetex.h>
#include "gasnet_coll.h"

#define DEFAULT_OUTER_VERIFICATION_ITERS 2
#define DEFAULT_INNER_VERIFICATION_ITERS 50
#define DEFAULT_PERFORMANCE_ITERS 0


#ifndef ALL_COLL_ENABLED
#define ALL_COLL_ENABLED 1


#define BROADCAST_ENABLED 0
#define SCATTER_ENABLED 0
#define GATHER_ENABLED 0
#define GATHER_ALL_ENABLED 0
#define EXCHANGE_ENABLED 0
#define REDUCE_ENABLED 0
#define REDUCE_ALL_ENABLED 0
#endif

#ifndef NB_TESTS_ENABLED 
#define NB_TESTS_ENABLED 1
#endif

#define ROOT_THREAD 0

/* max data size for the test in bytes*/
#define DEFAULT_MAX_DATA_SIZE 32768 

#define PRINT_TIMERS 1
#define VERBOSE_VERIFICATION_OUTPUT 0

/*max_dsize is a variable set in main*/
#define TOTAL_THREADS threads_per_node*gex_TM_QuerySize(myteam)

#if 1
#define ERROR_EXIT() gasnet_exit(1)
#else
#define ERROR_EXIT() do {} while(0)
#endif

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

gex_Rank_t mynode;
gex_Rank_t nodes;
gasnet_image_t threads_per_node;
gasnet_image_t THREADS;
int inner_verification_iters;
int outer_verification_iters;
int performance_iters;
size_t max_data_size;

static int src_insegment = 1;
static int dst_insegment = 1;

#define SEG_PER_THREAD (sizeof(int)*max_data_size*(inner_verification_iters)*TOTAL_THREADS)
#define TEST_SEGSZ_EXPR ((src_insegment+dst_insegment) * SEG_PER_THREAD*threads_per_node)

#define TEST_USE_PRIMORDIAL_THREAD 1
#include "test.h"

#define COLL_BARRIER() PTHREAD_BARRIER(threads_per_node)

typedef struct {
  int my_local_thread;
  int mythread;
  
  gasnet_coll_handle_t *hndl;
  char _pad[GASNETT_CACHE_LINE_BYTES];
  uint8_t *mysrc, *mydest;
  uint8_t *node_src, *node_dst;
} thread_data_t;

uint8_t **my_srcs;
uint8_t **my_dsts;

void fill_flag_str(int flags, char *outstr) {
  
  if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    strcpy(outstr, "no/no");
  } else if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    strcpy(outstr, "no/my");
  } else if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    strcpy(outstr, "no/all");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    strcpy(outstr, "my/no");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    strcpy(outstr, "my/my");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    strcpy(outstr, "my/all");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    strcpy(outstr, "all/no");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    strcpy(outstr, "all/my");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    strcpy(outstr, "all/all");
  }
}

void scale_ptrM(void * out_ptr[], void * const in_ptr[], size_t elem_count, size_t elem_size, gasnet_image_t total_images) {
  int i;
  for(i=0; i<total_images; i++) {
    out_ptr[i] = (void *)((uintptr_t)in_ptr[i] + (elem_count * elem_size));
  }
}

#if PRINT_TIMERS
#define print_timer(td, coll_str, addr_mode, num_addrs, sync_mode, nelem, total_ticks) \
if(td->my_local_thread==0 && performance_iters>0) MSG0("%c: %d> %s/%s %s sync_mode: (%s) tree: %s size: %"PRIuPTR" bytes time: %g us", \
                                TEST_SECTION_NAME(), td->mythread, addr_mode, num_addrs,\
                                coll_str, sync_mode, gasnett_getenv("GASNET_COLL_ROOTED_GEOM"), \
                                (uintptr_t) nelem*sizeof(int), (double)gasnett_ticks_to_us(total_ticks)/performance_iters)
#else
#define print_timer(td, coll_str, addr_mode, num_addrs, sync_mode, nelem, total_ticks)
#endif
void run_SINGLE_ADDR_test(thread_data_t *td, uint8_t **dst_arr, uint8_t **src_arr, size_t nelem, int root_thread, int in_flags) {
  /* all threads pass the same pointers for src and dest*/
  int i,j,t,k;
  int flags = in_flags
            | (src_insegment ? GASNET_COLL_SRC_IN_SEGMENT : 0)
            | (dst_insegment ? GASNET_COLL_DST_IN_SEGMENT : 0);
  int *src, *dst;
  char output_str[8];
  gasnett_tick_t begin, end;
  char flag_str[8];
  gex_Event_t *handles = performance_iters ? test_malloc(sizeof(gasnet_coll_handle_t)*performance_iters) : NULL;

  fill_flag_str(flags, flag_str);
  {
    src = ((int*) td->mysrc);
    dst = ((int*) td->mydest);
    strcpy(output_str, "LOCAL");
  }
  
#if BROADCAST_ENABLED || ALL_COLL_ENABLED  
  for(k=0; k<outer_verification_iters; k++) {
    COLL_BARRIER();
    /* BROADCAST*/  
    if(td->mythread == root_thread) {
      for(i=0; i<nelem*inner_verification_iters; i++) {
        src[i] = 42+i;
      } 
    }
    for(i=0; i<nelem*inner_verification_iters; i++) {
      dst[i] = -1;
    }
    
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
    for(i=0; i<inner_verification_iters; i++) { 
      gex_Event_Wait(gex_Coll_BroadcastNB(myteam, root_thread,
                                          dst+i*nelem, src+i*nelem,
                                          sizeof(int) * nelem, 0));
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
   
    for(i=0; i<nelem*inner_verification_iters; i++) {
      int expected = 42+i;
      if(dst[i] != expected) {
        MSG("%d> broadcast verification @ iteration: %d ... expected %d got %d", (int) td->mythread, (int) (i/nelem), expected, dst[i]);
        ERROR_EXIT();
      }
    }
  }

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    gex_Event_Wait(gex_Coll_BroadcastNB(myteam, root_thread, dst, src, sizeof(int) * nelem, 0));
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();
  print_timer(td, "broadcast", output_str,  "SINGLE-addr", flag_str, nelem, end);  

 #if NB_TESTS_ENABLED
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  {
    for(i=0; i<performance_iters; i++) { 
      handles[i] = gex_Coll_BroadcastNB(myteam, root_thread, dst, src, sizeof(int) * nelem, 0);
    }
    if (handles) gex_Event_WaitAll(handles, performance_iters, 0);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();
  print_timer(td, "broadcast_NB", output_str,  "SINGLE-addr", flag_str, nelem, end);  
 #endif
#endif
  
#if SCATTER_ENABLED || ALL_COLL_ENABLED  
  for(k=0; k<outer_verification_iters; k++) {
    COLL_BARRIER();
    /*SCATTER*/
    if(td->mythread == root_thread) {
      for(i=0; i<inner_verification_iters*THREADS*nelem; i++) {
        src[i] = 42+i;
      }
    }
    for(i=0; i<inner_verification_iters*nelem; i++) {
      dst[i] = -1;
    }
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();} 
    for(i=0; i<inner_verification_iters; i++) {
      gasnet_coll_scatter(GASNET_TEAM_ALL, dst+i*nelem, root_thread, src+i*nelem*THREADS, sizeof(int)*nelem, flags);
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    
    for(i=0; i<inner_verification_iters; i++) {
      for(j=0; j<nelem; j++) {
        int expected = (int) (42+i*THREADS*nelem+td->mythread*nelem+j);
        if(dst[i*nelem+j] != expected) {
          MSG("%d> scatter verification @ iteration: %d ... expected %d got %d", td->mythread, i, expected, dst[i*nelem+j]);
          ERROR_EXIT();
        }
      }
    }
  }
  
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    gasnet_coll_scatter(GASNET_TEAM_ALL, dst, root_thread, src, sizeof(int)*nelem, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "scatter", output_str,  "SINGLE-addr", flag_str, nelem, end);  
  
 #if NB_TESTS_ENABLED
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}

  for(i=0; i<performance_iters; i++) {  
    handles[i] = gasnet_coll_scatter_nb(GASNET_TEAM_ALL, dst, root_thread, src, sizeof(int)*nelem, flags);
  }
  for(i=0; i<performance_iters; i++) {
    gasnet_coll_wait_sync(handles[i]); 
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "scatter_NB", output_str,  "SINGLE-addr", flag_str, nelem, end);  
 #endif
#endif
  
#if GATHER_ENABLED || ALL_COLL_ENABLED  
  /*GATHER*/
  for(k=0; k<outer_verification_iters; k++) {
    COLL_BARRIER();
    for(i=0; i<inner_verification_iters; i++) {
      for(j=0; j<nelem; j++) {
        src[i*nelem+j] = 42+i*THREADS*nelem+td->mythread*nelem+j;
      }
    }
    if(root_thread == td->mythread) {
      for(i=0; i<nelem*inner_verification_iters*THREADS; i++) {
        dst[i] = -1;
      }
    }
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();} 
    for(i=0; i<inner_verification_iters; i++) {
      gasnet_coll_gather(GASNET_TEAM_ALL, root_thread, dst+i*nelem*THREADS, src+i*nelem, sizeof(int)*nelem, flags);
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    
    if(td->mythread == root_thread) {
      for(i=0; i<inner_verification_iters*THREADS*nelem; i++) {
        int expected = 42+i;
        if(dst[i] != 42+i) {
          MSG("%d> gather verification @ iteration: %d ... expected %d got %d", td->mythread, (int)(i/(THREADS*nelem)), expected, dst[i]);
        }
      }
    }
  }
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    gasnet_coll_gather(GASNET_TEAM_ALL, root_thread, dst, src, sizeof(int)*nelem, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "gather", output_str,  "SINGLE-addr", flag_str, nelem, end);  

 #if NB_TESTS_ENABLED
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    handles[i]  = gasnet_coll_gather_nb(GASNET_TEAM_ALL, root_thread, dst, src, sizeof(int)*nelem, flags);
  }
  for(i=0; i<performance_iters; i++) {
    gasnet_coll_wait_sync(handles[i]);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "gather_NB", output_str,  "SINGLE-addr", flag_str, nelem, end);  
 #endif
#endif
  
#if GATHER_ALL_ENABLED || ALL_COLL_ENABLED    
  /*GATHER_ALL*/
  for(k=0; k<outer_verification_iters; k++) {
    COLL_BARRIER();
    for(i=0; i<inner_verification_iters; i++) {
      for(j=0; j<nelem; j++) {
        src[i*nelem+j] = 42+i*THREADS*nelem+td->mythread*nelem+j;
      }
    }
    for(i=0; i<nelem*inner_verification_iters*THREADS; i++) {
      dst[i] = -1;
    }  
    
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();} 
    for(i=0; i<inner_verification_iters; i++) {
      gasnet_coll_gather_all(GASNET_TEAM_ALL, dst+i*nelem*THREADS, src+i*nelem, nelem*sizeof(int), flags);
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    
    for(i=0; i<inner_verification_iters*THREADS*nelem; i++) {
      int expected = 42+i;
      if(dst[i] != 42+i) {
        MSG("%d> gather_all verification @ iteration: %d ... expected %d got %d", td->mythread, (int)(i/(nelem*THREADS)), expected, dst[i]);
        ERROR_EXIT();
      }
    }
  }
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    gasnet_coll_gather_all(GASNET_TEAM_ALL, dst, src, sizeof(int)*nelem, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "gather_all", output_str,  "SINGLE-addr", flag_str, nelem, end);  

 #if NB_TESTS_ENABLED
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    handles[i] = gasnet_coll_gather_all_nb(GASNET_TEAM_ALL, dst, src, sizeof(int)*nelem, flags);
  }
  for(i=0; i<performance_iters; i++) {
    gasnet_coll_wait_sync(handles[i]);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "gather_all_NB", output_str,  "SINGLE-addr", flag_str, nelem, end);  
 #endif
#endif  
  
#if EXCHANGE_ENABLED || ALL_COLL_ENABLED  
  /*EXCHANGE*/
  for(k=0; k<outer_verification_iters; k++) {
    COLL_BARRIER();
    for(i=0; i<inner_verification_iters*nelem*THREADS; i++) {
      src[i] = 42+i+td->mythread*10000;
      dst[i] = -1;
    }
    
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();} 
    for(i=0; i<inner_verification_iters; i++) {
      gasnet_coll_exchange(GASNET_TEAM_ALL, dst+i*nelem*THREADS, src+i*nelem*THREADS, nelem*sizeof(int), flags);
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    
    for(i=0; i<inner_verification_iters; i++) {
      for(t=0; t<THREADS; t++) {
        for(j=0; j<nelem; j++) {
          int expected = (int)(42+t*10000+i*THREADS*nelem+td->mythread*nelem+j);
          if(dst[i*nelem*THREADS+t*nelem+j] != expected) {
            MSG("%d> exchange verification @ iteration: %d ... expected %d got %d", td->mythread, i, expected, dst[i*nelem*THREADS+t*nelem+j]);
            ERROR_EXIT();
          }
        }
      }
    }
  }
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    gasnet_coll_exchange(GASNET_TEAM_ALL, dst, src, sizeof(int)*nelem, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "exchange", output_str,  "SINGLE-addr", flag_str, nelem, end);  

 #if NB_TESTS_ENABLED
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    handles[i] = gasnet_coll_exchange_nb(GASNET_TEAM_ALL, dst, src, sizeof(int)*nelem, flags);
  }
  for(i=0; i<performance_iters; i++) { 
    gasnet_coll_wait_sync(handles[i]);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "exchange_NB", output_str,  "SINGLE-addr", flag_str, nelem, end);  
 #endif
#endif
  
#if REDUCE_ENABLED || ALL_COLL_ENABLED  
  /*REDUCE*/
  for(k=0; k<outer_verification_iters; k++) {
    COLL_BARRIER();
    for(i=0; i<inner_verification_iters; i++) {
      for(j=0; j<nelem; j++) {
        src[i*nelem+j] = (42*(i+1)+j);
      }
    }
    if(root_thread == td->mythread) {
      for(i=0; i<nelem*inner_verification_iters*THREADS; i++) {
        dst[i] = -1;
      }
    }
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();} 
    for(i=0; i<inner_verification_iters; i++) {
        gex_Event_Wait(gex_Coll_ReduceToOneNB(myteam, root_thread,
                                         dst+i*nelem, src+i*nelem,
                                         GEX_DT_I32, sizeof(int), nelem,
                                         GEX_OP_ADD, NULL, NULL, 0));
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    
    if(td->mythread == root_thread) {
      for(i=0; i<inner_verification_iters; i++) {
        for(j=0; j<nelem; j++) {
          int expected = (42*(i+1)+j)*THREADS;
          if(dst[i*nelem+j] != expected) {
            MSG("%d> reduce verification @ iteration: %d,%d ... expected %d got %d", td->mythread, i, j, expected, dst[i*nelem+j]);
            ERROR_EXIT();
          } else if(0) {
            MSG("%d> reduce passed @ iteration: %d,%d ... expected %d got %d", td->mythread, i, j, expected, dst[i*nelem+j]);
          
          }
        }
      }
    }
  }

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
      gex_Event_Wait(gex_Coll_ReduceToOneNB(myteam, root_thread, dst, src,
                                       GEX_DT_I32, sizeof(int), nelem,
                                       GEX_OP_ADD, NULL, NULL, 0));
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  

  print_timer(td,  "reduce", output_str,  "SINGLE-addr", flag_str, nelem, end);  

 #if NB_TESTS_ENABLED
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
      handles[i] = gex_Coll_ReduceToOneNB(myteam, root_thread, dst, src,
                                     GEX_DT_I32, sizeof(int), nelem,
                                     GEX_OP_ADD, NULL, NULL, 0);
  }
  if (handles) gex_Event_WaitAll(handles, performance_iters, 0);
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "reduce_NB", output_str,  "SINGLE-addr", flag_str, nelem, end);  
 #endif
#endif

#if REDUCE_ALL_ENABLED || ALL_COLL_ENABLED  
  /*REDUCE_ALL*/
  for(k=0; k<outer_verification_iters; k++) {
    COLL_BARRIER();
    for(i=0; i<inner_verification_iters; i++) {
      for(j=0; j<nelem; j++) {
        src[i*nelem+j] = (42*(i+1)+j);
      }
    }
    for(i=0; i<nelem*inner_verification_iters*THREADS; i++) {
      dst[i] = -1;
    }
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();} 
    for(i=0; i<inner_verification_iters; i++) {
        gex_Event_Wait(gex_Coll_ReduceToAllNB(myteam,
                                         dst+i*nelem, src+i*nelem,
                                         GEX_DT_I32, sizeof(int), nelem,
                                         GEX_OP_ADD, NULL, NULL, 0));
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    
    for(i=0; i<inner_verification_iters; i++) {
      for(j=0; j<nelem; j++) {
        int expected = (42*(i+1)+j)*THREADS;
        if(dst[i*nelem+j] != expected) {
          MSG("%d> reduce_all verification @ iteration: %d,%d ... expected %d got %d", td->mythread, i, j, expected, dst[i*nelem+j]);
          ERROR_EXIT();
        } else if(0) {
          MSG("%d> reduce_all passed @ iteration: %d,%d ... expected %d got %d", td->mythread, i, j, expected, dst[i*nelem+j]);
        }
      }
    }
  }

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
      gex_Event_Wait(gex_Coll_ReduceToAllNB(myteam, dst, src,
                                       GEX_DT_I32, sizeof(int), nelem,
                                       GEX_OP_ADD, NULL, NULL, 0));
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  

  print_timer(td,  "reduce_all", output_str,  "SINGLE-addr", flag_str, nelem, end);  

 #if NB_TESTS_ENABLED
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
      handles[i] = gex_Coll_ReduceToAllNB(myteam, dst, src,
                                     GEX_DT_I32, sizeof(int), nelem,
                                     GEX_OP_ADD, NULL, NULL, 0);
  }
  if (handles) gex_Event_WaitAll(handles, performance_iters, 0);
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "reduce_all_NB", output_str,  "SINGLE-addr", flag_str, nelem, end);  
 #endif
#endif

  if(td->my_local_thread==0 && VERBOSE_VERIFICATION_OUTPUT) MSG0("%c: %s/SINGLE-addr sync_mode: %s size: %"PRIuPTR" bytes root: %d.  PASS", 
                                                                 TEST_SECTION_NAME(), output_str, flag_str, (uintptr_t) (sizeof(int)*nelem), root_thread);
  
  COLL_BARRIER();
  test_free(handles);
}

static double szfactor = 2.;

void *thread_main(void *arg) {
  thread_data_t *td = (thread_data_t*) arg;
  double sz;
  int flag_iter;
  gex_Rank_t root_thread = ROOT_THREAD;
  int skip_msg_printed = 0;

  COLL_BARRIER();

  for(flag_iter=0; flag_iter<9; flag_iter++) {
    int flags;
    PTHREAD_BARRIER(threads_per_node);
    if(td->my_local_thread==0) TEST_SECTION_BEGIN();
    COLL_BARRIER();
    
    if(TEST_SECTION_ENABLED()) {
      
      switch(flag_iter) { 
      case 0: flags = GASNET_COLL_IN_NOSYNC  | GASNET_COLL_OUT_NOSYNC; break;
      case 1: flags = GASNET_COLL_IN_NOSYNC  | GASNET_COLL_OUT_MYSYNC; break;
      case 2: flags = GASNET_COLL_IN_NOSYNC  | GASNET_COLL_OUT_ALLSYNC; break;
      case 3: flags = GASNET_COLL_IN_MYSYNC  | GASNET_COLL_OUT_NOSYNC; break;
      case 4: flags = GASNET_COLL_IN_MYSYNC  | GASNET_COLL_OUT_MYSYNC; break;
      case 5: flags = GASNET_COLL_IN_MYSYNC  | GASNET_COLL_OUT_ALLSYNC; break;
      case 6: flags = GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_NOSYNC; break;
      case 7: flags = GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_MYSYNC; break;
      case 8: flags = GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_ALLSYNC; break;  
      default: continue;
      }
    
    COLL_BARRIER();

      for(sz = 1; sz<=max_data_size; sz*=szfactor) {
        run_SINGLE_ADDR_test(td, my_dsts, my_srcs, (size_t)sz, root_thread, flags|GASNET_COLL_LOCAL);   
      }

      if(td->my_local_thread==0  && !VERBOSE_VERIFICATION_OUTPUT) {
        size_t final_size = (size_t) (sz/szfactor);
        char flag_str[8];
        fill_flag_str(flags, flag_str);
        MSG0("%c: sync_mode: %s %d-%"PRIuPTR" bytes root: %d.  PASS",  
             TEST_SECTION_NAME(), flag_str, (int)(sizeof(int)*1), (uintptr_t) sizeof(int)*final_size, (int) root_thread);
      }

    }
  }
  gasnet_coll_dumpTuningState(NULL, GASNET_TEAM_ALL);
  gasnet_coll_dumpProfile(NULL, GASNET_TEAM_ALL);
  MSG("thread %d> done", td->mythread);
  return NULL;
}

int main(int argc, char **argv)
{
  static uint8_t *A, *B;
  int i,j;
  int help = 0;
  thread_data_t *td_arr;
  
  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testcollperf", &argc, &argv, 0));
  
  int arg = 1;
  while (argc > arg) {
    if (!strcmp(argv[arg], "-szfactor")) {
      ++arg;
      if (argc > arg) { szfactor = atof(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-src-in")) {
      src_insegment = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-src-out")) {
      src_insegment = 0;
      ++arg;
    } else if (!strcmp(argv[arg], "-src-in")) {
      dst_insegment = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-dst-out")) {
      dst_insegment = 0;
      ++arg;
    } else if (argv[arg][0] == '-') {
      help = 1;
      ++arg;
    } else break;
  }
  
  if(argc > arg) {
    max_data_size = MAX(atoi(argv[arg])/sizeof(int),1); 
    arg++;
  } else {
    max_data_size = DEFAULT_MAX_DATA_SIZE/sizeof(int);
  }
  
  if (argc > arg) {
    outer_verification_iters = MAX(1,atoi(argv[arg]));
    arg++;
  } else {
    outer_verification_iters = DEFAULT_OUTER_VERIFICATION_ITERS;
  }

  
  if (argc > arg) {
    inner_verification_iters = MAX(1,atoi(argv[arg]));
    arg++;
  } else {
    inner_verification_iters = DEFAULT_INNER_VERIFICATION_ITERS;
  }
  /* make sure that there is at least 1 inner verification iteration 
     since this is waht we use for the performance runs*/
  inner_verification_iters = MAX(1, inner_verification_iters);
  
  if(argc > arg) {
    performance_iters = atoi(argv[arg]);
    arg++;
  } else {
    performance_iters = DEFAULT_PERFORMANCE_ITERS;
  }
  
#if GASNET_PAR
  #define PAR_USAGE "(thread count per node) "
#else
  #define PAR_USAGE ""
#endif

  /* Need to test_init before gasnet_attach to use TEST_LOCALPROCS() */
  test_init_early("testcollperf",(performance_iters != 0),
                  "[options] (max data size) (outer_verification_iters) (inner_verification_iters) (performance_iters) " PAR_USAGE "(test sections)\n"
                  "  The '-{src,dst}-{in,out} options selects whether src and dst\n"
                  "   buffers are in the GASNet segment or not (defaults are 'in').\n"
                  "  -szfactor <f>   \n"
                  "            selects f as growth factor for data sizes."
                 );

#if GASNET_PAR
  if (argc > arg) {
    threads_per_node = atoi(argv[arg]);
    arg++;
  } else {
    if (gasnett_getenv_yesno_withdefault("GASNET_TEST_POLITE_SYNC",0)) {
      /* May overcommit only if somebody already expected it */
      threads_per_node = gasnett_cpu_count();
    } else {
      threads_per_node = gasnett_cpu_count() / TEST_LOCALPROCS(); 
    }
    threads_per_node = MIN(threads_per_node, 4);
    threads_per_node = test_thread_limit(threads_per_node);
    threads_per_node = MAX(threads_per_node, 1);
  }
  if (threads_per_node > TEST_MAXTHREADS || threads_per_node < 1) {
    printf("ERROR: Threads must be between 1 and %d\n", TEST_MAXTHREADS);
    exit(EXIT_FAILURE);
  }
  // NO MULTI-IMAGE SUPPORT IN CURRENT COLLECTIVES
  if (threads_per_node > 1) {
    MSG0("WARNING: thread count reduced to 1 (no multi-image support)");
    threads_per_node = 1;
  }
#else
  threads_per_node = 1;
#endif  

  if (argc > arg) {
    TEST_SECTION_PARSE(argv[arg]);
    arg++;
  }

  if (help || argc > arg) test_usage();

  /* get SPMD info */
  mynode = gex_TM_QueryRank(myteam);
  nodes = gex_TM_QuerySize(myteam);
  THREADS = nodes * threads_per_node;

  /* do some sanity checking of the input arguments*/
  /* the total memory that we will need to attach is inner_verification_iters*total_images*my_images*2*sizeof(int)*max_data_size*/
  /* make sure that this value is about less than or equal to half the maximum gasnet segment */
  /* if it is not scale down inner_verification_iters and scale up outer_verification_iters until we match*/
  {
    size_t curr_req = inner_verification_iters * THREADS * threads_per_node * sizeof(int) * max_data_size * 2;
    size_t max_mem_usage = gasnet_getMaxGlobalSegmentSize()/2;
    MSG0("command line args:\n"
         "    max_data_size = %"PRIuPTR"\n"
         "    bytes outer_verification_iters = %d\n"
         "    inner_verification_iters = %d\n"
         "    performance_iters = %d\n"
         "    threads_per_node = %d\n"
         "    szfactor = %.2f\n"
         "    src addresses %sside the segment\n"
         "    dst addresses %sside the segment\n",
         (uintptr_t)(max_data_size*sizeof(int)),
         outer_verification_iters, inner_verification_iters, performance_iters,
         (int) threads_per_node, szfactor,
         (src_insegment ? "in" : "out"),
         (dst_insegment ? "in" : "out"));
    if(curr_req > max_mem_usage) {
      MSG0("WARNING: inner iterations too large.\n");
      MSG0("Scaling down inner iterations and scaling up outer iterations to compensate\n");
      while(curr_req > max_mem_usage && inner_verification_iters > 1) {
        inner_verification_iters /=2; 
        outer_verification_iters *=2;
        curr_req = inner_verification_iters * THREADS * threads_per_node * sizeof(int) * max_data_size * 2;
      }
      /* we can't scale down inner verifications further than 1 so we have to scale down the data size until it fits*/
      if(inner_verification_iters ==1 && curr_req > max_mem_usage) {
        MSG0("WARNING: scaled inner iterations down to 1 but still not enough room in segment.");
        MSG0("WARNING: scaling down datasize");
      }
      while(inner_verification_iters == 1 && curr_req > max_mem_usage && max_data_size > 0) {
        max_data_size /=2;
        curr_req = inner_verification_iters * THREADS * threads_per_node * sizeof(int) * max_data_size * 2;
      }
      if(max_data_size ==0) {
        MSG0("ERROR: Segment too small ... can't run testcollperf");
        gasnet_exit(1);
      }
      MSG0("adjusted args: max_data_size=%"PRIuPTR" bytes outer_verification_iters=%d inner_verification_iters=%d performance_iters=%d threads_per_node=%d ", (uintptr_t)(max_data_size*sizeof(int)), 
           outer_verification_iters, inner_verification_iters, performance_iters, (int) threads_per_node);
    } 
  }

  TEST_SET_WAITMODE(threads_per_node);

  if (0 != TEST_SEGSZ_REQUEST) {
    GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  }
  {
    size_t sz = SEG_PER_THREAD * threads_per_node;
    A = src_insegment ? (uint8_t*)TEST_MYSEG()                    : test_malloc(sz);
    B = dst_insegment ? (uint8_t*)TEST_MYSEG() + src_insegment*sz : test_malloc(sz);
  }

  my_srcs =  (uint8_t**) test_malloc(sizeof(uint8_t*)*threads_per_node);
  my_dsts =  (uint8_t**) test_malloc(sizeof(uint8_t*)*threads_per_node);
  td_arr = (thread_data_t*) test_malloc(sizeof(thread_data_t)*threads_per_node);
  
  for(i=0; i<threads_per_node; i++) {
    my_srcs[i] = A + i*SEG_PER_THREAD;
    my_dsts[i] = B + i*SEG_PER_THREAD;
    td_arr[i].my_local_thread = i;
    td_arr[i].mythread = mynode*threads_per_node+i;
    td_arr[i].mysrc = my_srcs[i];
    td_arr[i].mydest = my_dsts[i];
  }
  
#if GASNET_PAR
  test_createandjoin_pthreads(threads_per_node, &thread_main, td_arr, sizeof(thread_data_t));
#else
  thread_main(&td_arr[0]);
#endif
  
  test_free(td_arr);
  BARRIER();
  MSG("done.");
  gasnet_exit(0);
  return 0;
}

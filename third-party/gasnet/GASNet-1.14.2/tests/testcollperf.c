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
#include "gasnet.h"
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
#endif

#ifndef ALL_ADDR_MODE_ENABLED 
#define ALL_ADDR_MODE_ENABLED 1


#define SINGLE_SINGLE_MODE_ENABLED 0
#define SINGLE_LOCAL_MODE_ENABLED 0
#define MULTI_SINGLE_MODE_ENABLED 1
#define MULTI_LOCAL_MODE_ENABLED 0
#endif


#define ROOT_THREAD 0

/* max data size for the test in bytes*/
#define DEFAULT_MAX_DATA_SIZE 32768 

#define PRINT_TIMERS 1
#define VERBOSE_VERIFICATION_OUTPUT 0

/*max_dsize is a variable set in main*/
#define TOTAL_THREADS threads_per_node*gasnet_nodes()

#if 1
#define ERROR_EXIT() gasnet_exit(1)
#else
#define ERROR_EXIT() do {} while(0)
#endif

gasnet_node_t mynode;
gasnet_node_t nodes;
gasnet_image_t threads_per_node;
gasnet_image_t THREADS;
int inner_verification_iters;
int outer_verification_iters;
int performance_iters;
size_t max_data_size;

#define TEST_SEGSZ_EXPR (sizeof(int)*(max_data_size*(inner_verification_iters)*TOTAL_THREADS*threads_per_node*2))
#define SEG_PER_THREAD (sizeof(int)*max_data_size*(inner_verification_iters)*TOTAL_THREADS)

#define TEST_USE_PRIMORDIAL_THREAD 1
#include "test.h"

#if 0
#define COLL_BARRIER() do{  gasnet_coll_barrier_notify(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS | GASNET_BARRIERFLAG_IMAGES); gasnet_coll_barrier_wait(GASNET_TEAM_ALL, 0, GASNET_BARRIERFLAG_ANONYMOUS | GASNET_BARRIERFLAG_IMAGES);} while(0)
#else
#define COLL_BARRIER() PTHREAD_BARRIER(threads_per_node)
#endif

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
uint8_t **all_srcs;
uint8_t **all_dsts;

void fill_flag_str(int flags, char *outstr) {
  
  if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    sprintf(outstr, "no/no");
  } else if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    sprintf(outstr, "no/my");
  } else if(flags & GASNET_COLL_IN_NOSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    sprintf(outstr, "no/all");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    sprintf(outstr, "my/no");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    sprintf(outstr, "my/my");
  } else if(flags & GASNET_COLL_IN_MYSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    sprintf(outstr, "my/all");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_NOSYNC) {
    sprintf(outstr, "all/no");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_MYSYNC) {
    sprintf(outstr, "all/my");
  } else if(flags & GASNET_COLL_IN_ALLSYNC && flags & GASNET_COLL_OUT_ALLSYNC) {
    sprintf(outstr, "all/all");
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
if(td->my_local_thread==0 && performance_iters>0) MSG0("%c: %d> %s/%s %s sync_mode: (%s) tree: %s size: %ld bytes time: %g us", TEST_SECTION_NAME(), td->mythread, addr_mode, num_addrs,\
                                coll_str, sync_mode, gasnett_getenv("GASNET_COLL_ROOTED_GEOM"), (long int) nelem*sizeof(int), (double)gasnett_ticks_to_us(total_ticks)/performance_iters)
#else
#define print_timer(td, coll_str, addr_mode, num_addrs, sync_mode, nelem, total_ticks)
#endif
void int_reduce_fn(void *results, size_t result_count,
                   const void *left_operands, size_t left_count,
                   const void *right_operands,
                   size_t elem_size, int flags, int arg) {
  int i;
  int *res = (int*) results;
  int *src1 = (int*) left_operands;
  int *src2 = (int*) right_operands;
  assert(elem_size == sizeof(int));
  assert(result_count==left_count);
  switch(arg) {
  case 0:
    for(i=0; i<result_count; i++) {
      res[i] = src1[i] + src2[i];
    } break;
  case 1:
    for(i=0; i<result_count; i++) {
      res[i] = MAX(src1[i],src2[i]);
    } break;
  case 2:
    for(i=0; i<result_count; i++) {
      res[i] = MIN(src1[i],src2[i]);
    } break;
  default:
    MSG("NOT SUPPORTED reduce op %d\n", arg); ERROR_EXIT();
  }
  
}
gasnet_coll_fn_entry_t fntable;
void run_SINGLE_ADDR_test(thread_data_t *td, uint8_t **dst_arr, uint8_t **src_arr, size_t nelem, int root_thread, int in_flags) {
  /* all threads pass the same pointers for src and dest*/
  int i,j,t,k;
  int flags = in_flags | GASNET_COLL_SRC_IN_SEGMENT|GASNET_COLL_DST_IN_SEGMENT;
  int *src, *dst;
  char output_str[8];
  gasnett_tick_t begin, end;
  char flag_str[8];
  gasnet_coll_handle_t *handles;
  handles = performance_iters ? test_malloc(sizeof(gasnet_coll_handle_t)*performance_iters) : NULL;

  fill_flag_str(flags, flag_str);
  if(flags & GASNET_COLL_SINGLE) {
    src = (int*) (src_arr[0]); /* all threads have the same address so just use slot 0*/
    dst = (int*) (dst_arr[0]); /* all threads have the same address so just use slot 0*/
    sprintf(output_str, "SINGLE");
  } else {
    src = ((int*) td->mysrc);
    dst = ((int*) td->mydest);
    sprintf(output_str, "LOCAL");
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
      gasnet_coll_broadcast(GASNET_TEAM_ALL, dst+i*nelem, root_thread, src+i*nelem, sizeof(int)*nelem, flags);
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
    gasnet_coll_broadcast(GASNET_TEAM_ALL, dst, root_thread, src, sizeof(int)*nelem, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();
  print_timer(td, "broadcast", output_str,  "SINGLE-addr", flag_str, nelem, end);  

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  {
    for(i=0; i<performance_iters; i++) { 
      handles[i] = gasnet_coll_broadcast_nb(GASNET_TEAM_ALL, dst, root_thread, src, sizeof(int)*nelem, flags);
    }
    for(i=0; i<performance_iters; i++) {
      gasnet_coll_wait_sync(handles[i]);
    }
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();
  print_timer(td, "broadcast_NB", output_str,  "SINGLE-addr", flag_str, nelem, end);  

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
      gasnet_coll_reduce(GASNET_TEAM_ALL, root_thread, dst+i*nelem, src+i*nelem, 0,0, sizeof(int), nelem, 0, 0, flags);
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
    gasnet_coll_reduce(GASNET_TEAM_ALL, root_thread, dst, src, 0,0, sizeof(int), nelem, 0, 0, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  

  print_timer(td,  "reduce", output_str,  "SINGLE-addr", flag_str, nelem, end);  

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    handles[i] = gasnet_coll_reduce_nb(GASNET_TEAM_ALL, root_thread, dst, src, 0,0, sizeof(int), nelem, 0, 0, flags);
  }
  for(i=0; i<performance_iters; i++) { 
    gasnet_coll_wait_sync(handles[i]);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  

  print_timer(td,  "reduce_NB", output_str,  "SINGLE-addr", flag_str, nelem, end);  
#endif
  if(td->my_local_thread==0 && VERBOSE_VERIFICATION_OUTPUT) MSG0("%c: %s/SINGLE-addr sync_mode: %s size: %ld bytes root: %d.  PASS", TEST_SECTION_NAME(), output_str, flag_str, (long int) (sizeof(int)*nelem), root_thread);
  
  COLL_BARRIER();
  test_free(handles);
}

void run_MULTI_ADDR_test(thread_data_t *td, uint8_t **dst_arr, uint8_t **src_arr, size_t nelem, gasnet_image_t root_thread, int in_flags) {  
  /* all threads pass the same pointers for src and dest*/
  int i,j,t,k;
  int flags = in_flags|  GASNET_COLL_SRC_IN_SEGMENT | GASNET_COLL_DST_IN_SEGMENT;
  gasnet_image_t num_addrs;
  gasnett_tick_t begin,end;
  int *src, *dst, *mysrc, *mydest;
  uint8_t **tmp_src, **tmp_dest, **curr_dst_arr, **curr_src_arr;
  char output_str[8];
  char flag_str[8];
  gasnet_coll_handle_t *handles;
  handles = performance_iters ? test_malloc(sizeof(gasnet_coll_handle_t)*performance_iters) : NULL;
  fill_flag_str(flags, flag_str);
  
  COLL_BARRIER();
  if(flags & GASNET_COLL_SINGLE) {
    src = (int*) src_arr[root_thread]; /* all threads have the same address so just use slot 0*/
    dst = (int*) dst_arr[root_thread]; /* all threads have the same address so just use slot 0*/
    num_addrs = THREADS;
    sprintf(output_str, "SINGLE");
  } else {
    src = ((int*)src_arr[root_thread % threads_per_node]); 
    dst = ((int*)dst_arr[root_thread % threads_per_node]); 
    num_addrs = threads_per_node;
    sprintf(output_str, "LOCAL");
  }
  
  mysrc = ((int*) td->mysrc);
  mydest = ((int*) td->mydest);
  tmp_src = (uint8_t**) test_malloc(sizeof(uint8_t*)*num_addrs*inner_verification_iters);
  tmp_dest = (uint8_t**) test_malloc(sizeof(uint8_t*)*num_addrs*inner_verification_iters);
  

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
      mydest[i] = -1;
    }
    curr_dst_arr = tmp_dest;
    
    gasnett_local_mb();
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
    for(i=0; i<inner_verification_iters; i++) { 
      scale_ptrM((void**) curr_dst_arr, (void**) dst_arr, nelem*i, sizeof(int), num_addrs);
      gasnet_coll_broadcastM(GASNET_TEAM_ALL, (void**) curr_dst_arr, root_thread, src+i*nelem, sizeof(int)*nelem, flags);
      curr_dst_arr+=num_addrs;
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    gasnett_local_mb();
    for(i=0; i<nelem*inner_verification_iters; i++) {
      int expected = 42+i;
      if(mydest[i] != 42+i) {
        MSG("%d> broadcastM verification @ iteration: %d ... expected %d got %d (%d bytes)", td->mythread, (int)(i/nelem), expected, mydest[i], (int)(nelem*sizeof(int)));
        ERROR_EXIT();
      }
    }
  }
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    gasnet_coll_broadcastM(GASNET_TEAM_ALL, (void**) dst_arr, root_thread, src, sizeof(int)*nelem, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();
  print_timer(td, "broadcastM", output_str,  "MULTI-addr", flag_str, nelem, end);  

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    handles[i] = gasnet_coll_broadcastM_nb(GASNET_TEAM_ALL, (void**) dst_arr, root_thread, src, sizeof(int)*nelem, flags);
  }
  for(i=0; i<performance_iters; i++) {
    gasnet_coll_wait_sync(handles[i]);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();
  print_timer(td, "broadcastM_NB", output_str,  "MULTI-addr", flag_str, nelem, end);  
  
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
      mydest[i] = -1;
    }
    
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();} 
    curr_dst_arr = tmp_dest;
    for(i=0; i<inner_verification_iters; i++) {
      scale_ptrM((void**) curr_dst_arr, (void**) dst_arr, nelem*i, sizeof(int), num_addrs);
      gasnet_coll_scatterM(GASNET_TEAM_ALL, (void**) curr_dst_arr, root_thread, src+i*nelem*THREADS, sizeof(int)*nelem, flags);
      curr_dst_arr+=num_addrs;
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    
    
    for(i=0; i<inner_verification_iters; i++) {
      for(j=0; j<nelem; j++) {
        int expected = (int)(42+i*THREADS*nelem+td->mythread*nelem+j);
        if(mydest[i*nelem+j] != expected) {
          MSG("%d> scatterM verification @ iteration: %d ... expected %d got %d", td->mythread, i, expected, mydest[i*nelem+j]);
          ERROR_EXIT();
        }
      }
    }
  }
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    gasnet_coll_scatterM(GASNET_TEAM_ALL, (void**) dst_arr, root_thread, src, sizeof(int)*nelem, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();
  
  print_timer(td, "scatterM", output_str,  "MULTI-addr", flag_str, nelem, end);  

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    handles[i] = gasnet_coll_scatterM_nb(GASNET_TEAM_ALL, (void**) dst_arr, root_thread, src, sizeof(int)*nelem, flags);
  }
  for(i=0; i<performance_iters; i++) {
    gasnet_coll_wait_sync(handles[i]);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();
  
  print_timer(td, "scatterM_NB", output_str,  "MULTI-addr", flag_str, nelem, end);  
#endif

#if 1  
#if GATHER_ENABLED || ALL_COLL_ENABLED
  /*GATHER*/
  for(k=0; k<outer_verification_iters; k++) {
    COLL_BARRIER();
    for(i=0; i<inner_verification_iters; i++) {
      for(j=0; j<nelem; j++) {
        mysrc[i*nelem+j] = 42+i*THREADS*nelem+td->mythread*nelem+j;
      }
    }
    if(root_thread == td->mythread) {
      for(i=0; i<nelem*inner_verification_iters*THREADS; i++) {
        dst[i] = -1;
      }
    }
    
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();} 
    curr_src_arr = tmp_src;
    for(i=0; i<inner_verification_iters; i++) {
      scale_ptrM((void**) curr_src_arr, (void**) src_arr, nelem*i, sizeof(int), num_addrs);
      gasnet_coll_gatherM(GASNET_TEAM_ALL, root_thread, dst+i*nelem*THREADS, (void**) curr_src_arr, sizeof(int)*nelem, flags);
      curr_src_arr+=num_addrs;
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    
    if(td->mythread == root_thread) {
      for(i=0; i<inner_verification_iters*THREADS*nelem; i++) {
        int expected = 42+i;
        if(mydest[i] != expected) {
          MSG("%d> gatherM verification @ iteration: %d ... expected %d got %d", td->mythread, (int)(i/(nelem*THREADS)), expected, mydest[i]);
          ERROR_EXIT();
        }
      }
    }
  }
  
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    gasnet_coll_gatherM(GASNET_TEAM_ALL, root_thread, dst, (void**)src_arr, sizeof(int)*nelem, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();
  
  print_timer(td, "gatherM", output_str,  "MULTI-addr", flag_str, nelem, end);  

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    handles[i] = gasnet_coll_gatherM_nb(GASNET_TEAM_ALL, root_thread, dst, (void**)src_arr, sizeof(int)*nelem, flags);
  }
  for(i=0; i<performance_iters; i++) {
    gasnet_coll_wait_sync(handles[i]);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();
  
  print_timer(td, "gatherM_NB", output_str,  "MULTI-addr", flag_str, nelem, end);  
#endif
#if GATHER_ALL_ENABLED || ALL_COLL_ENABLED
  /*GATHER_ALL*/
  for(k=0; k<outer_verification_iters; k++) {
    COLL_BARRIER();
    for(i=0; i<inner_verification_iters; i++) {
      for(j=0; j<nelem; j++) {
        mysrc[i*nelem+j] = 42+i*THREADS*nelem+td->mythread*nelem+j;
      }
    }
    for(i=0; i<nelem*inner_verification_iters*THREADS; i++) {
      mydest[i] = -1;
    }  
    
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();} 
    curr_dst_arr = tmp_dest; curr_src_arr = tmp_src;
    for(i=0; i<inner_verification_iters; i++) {
      scale_ptrM((void**) curr_dst_arr, (void**) dst_arr, nelem*i*THREADS, sizeof(int), num_addrs);
      scale_ptrM((void**) curr_src_arr, (void**) src_arr, nelem*i, sizeof(int), num_addrs);
      gasnet_coll_gather_allM(GASNET_TEAM_ALL, (void**) curr_dst_arr, (void**) curr_src_arr, nelem*sizeof(int), flags);
      curr_dst_arr+=num_addrs; curr_src_arr+=num_addrs;
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    
    for(i=0; i<inner_verification_iters*THREADS*nelem; i++) {
      int expected = 42+i;
      if(mydest[i] != 42+i) {
        MSG("%d> gather_allM verification @ iteration: %d ... expected %d got %d", td->mythread, (int)(i/(nelem*THREADS)), expected, mydest[i]);
        ERROR_EXIT();
      } 
    }
  }
  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    gasnet_coll_gather_allM(GASNET_TEAM_ALL, (void**) dst_arr, (void**)src_arr, sizeof(int)*nelem, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td, "gather_allM", output_str,  "MULTI-addr", flag_str, nelem, end);  

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    handles[i] = gasnet_coll_gather_allM_nb(GASNET_TEAM_ALL, (void**) dst_arr, (void**)src_arr, sizeof(int)*nelem, flags);
  }
  for(i=0; i<performance_iters; i++) {
    gasnet_coll_wait_sync(handles[i]);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td, "gather_allM_NB", output_str,  "MULTI-addr", flag_str, nelem, end);  
#endif
  
#if EXCHANGE_ENABLED || ALL_COLL_ENABLED
  /*EXCHANGE*/
  for(k=0; k<outer_verification_iters; k++) {
    COLL_BARRIER();
    for(i=0; i<inner_verification_iters*nelem*THREADS; i++) {
      mysrc[i] = 42+i+td->mythread*10000;
      mydest[i] = -1;
    }
    
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();} 
    curr_dst_arr = tmp_dest; curr_src_arr = tmp_src;
    for(i=0; i<inner_verification_iters; i++) {
      scale_ptrM((void**) curr_dst_arr, (void**) dst_arr, nelem*i*THREADS, sizeof(int), num_addrs);
      scale_ptrM((void**) curr_src_arr, (void**) src_arr, nelem*i*THREADS, sizeof(int), num_addrs);
      gasnet_coll_exchangeM(GASNET_TEAM_ALL, (void**) curr_dst_arr, (void**) curr_src_arr, nelem*sizeof(int), flags);
      curr_dst_arr+=num_addrs; curr_src_arr+=num_addrs;
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    
    for(i=0; i<inner_verification_iters; i++) {
      for(t=0; t<THREADS; t++) {
        for(j=0; j<nelem; j++) {
          int expected = (int)(42+t*10000+i*THREADS*nelem+td->mythread*nelem+j);
          if(mydest[i*nelem*THREADS+t*nelem+j] != expected) {
            MSG("%d> exchangeM verification @ iteration: %d ... expected %d got %d", td->mythread, i, expected, mydest[i*nelem*THREADS+t*nelem+j]);
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
    gasnet_coll_exchangeM(GASNET_TEAM_ALL, (void**) dst_arr, (void**)src_arr, sizeof(int)*nelem, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td, "exchangeM", output_str,  "MULTI-addr", flag_str, nelem, end);  

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    handles[i] = gasnet_coll_exchangeM_nb(GASNET_TEAM_ALL, (void**) dst_arr, (void**)src_arr, sizeof(int)*nelem, flags);
  }
  for(i=0; i<performance_iters; i++) {
    gasnet_coll_wait_sync(handles[i]);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td, "exchangeM_NB", output_str,  "MULTI-addr", flag_str, nelem, end);  
#endif
  
#if REDUCE_ENABLED || ALL_COLL_ENABLED  
  /*REDUCE*/
  for(k=0; k<outer_verification_iters; k++) {
    COLL_BARRIER();
    for(i=0; i<inner_verification_iters; i++) {
      for(j=0; j<nelem; j++) {
        mysrc[i*nelem+j] = (42*(i+1)+j);
      }
    }
    if(root_thread == td->mythread) {
      for(i=0; i<nelem*inner_verification_iters*THREADS; i++) {
        dst[i] = -1;
      }
    }
    if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();} 
    curr_src_arr = tmp_src;
    for(i=0; i<inner_verification_iters; i++) {
      scale_ptrM((void**) curr_src_arr, (void**) src_arr, nelem*i, sizeof(int), num_addrs);
      gasnet_coll_reduceM(GASNET_TEAM_ALL, root_thread, dst+i*nelem, (void**)curr_src_arr, 0,0, sizeof(int), nelem, 0, 0, flags);
      curr_src_arr +=num_addrs;
    }
    if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
    
    if(td->mythread == root_thread) {
      for(i=0; i<inner_verification_iters; i++) {
        for(j=0; j<nelem; j++) {
          int expected = (42*(i+1)+j)*THREADS;
          if(mydest[i*nelem+j] != expected) {
            MSG("%d> reduceM verification @ iteration: %d,%d,%d ... expected %d got %d", td->mythread, k, i, j, expected, mydest[i*nelem+j]);
            ERROR_EXIT();
          } else if(0) {
            MSG("%d> reduceM passed @ iteration: %d,%d ... expected %d got %d", td->mythread, i, j, expected, mydest[i*nelem+j]);
            
          }
        }
      }
    }
  }

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    gasnet_coll_reduceM(GASNET_TEAM_ALL, root_thread, dst, (void**)src_arr, 0,0, sizeof(int), nelem, 0, 0, flags);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "reduceM", output_str,  "MULTI-addr", flag_str, nelem, end);  

  COLL_BARRIER();
  begin = gasnett_ticks_now();
  if(flags & GASNET_COLL_IN_NOSYNC) {COLL_BARRIER();}
  for(i=0; i<performance_iters; i++) { 
    handles[i] = gasnet_coll_reduceM_nb(GASNET_TEAM_ALL, root_thread, dst, (void**)src_arr, 0,0, sizeof(int), nelem, 0, 0, flags);
  }
  for(i=0; i<performance_iters; i++) {
    gasnet_coll_wait_sync(handles[i]);
  }
  if(flags & GASNET_COLL_OUT_NOSYNC) {COLL_BARRIER();}
  end =  gasnett_ticks_now() - begin;
  COLL_BARRIER();  
  print_timer(td,  "reduceM_NB", output_str,  "MULTI-addr", flag_str, nelem, end);  
#endif
  
#endif
  if(td->my_local_thread==0  && VERBOSE_VERIFICATION_OUTPUT) MSG0("%c: %s/MULTI-addr sync_mode: %s size: %ld bytes root: %d.  PASS", TEST_SECTION_NAME(), output_str, flag_str, (long int) (sizeof(int)*nelem), (int) root_thread);

  COLL_BARRIER();
  test_free(tmp_src);
  test_free(tmp_dest);
  test_free(handles);
}



void *thread_main(void *arg) {
  thread_data_t *td = (thread_data_t*) arg;
  size_t size;
  int i,flag_iter;
  gasnet_node_t root_thread = ROOT_THREAD;
  int skip_msg_printed = 0;
  gasnet_coll_fn_entry_t fntable[1];
#if GASNET_PAR
  gasnet_image_t *imagearray = test_malloc(nodes * sizeof(gasnet_image_t));
  fntable[0].fnptr = int_reduce_fn;
  fntable[0].flags = 0;

  for (i=0; i<nodes; ++i) { imagearray[i] = threads_per_node; }
  gasnet_coll_init(imagearray, td->mythread, fntable, 1, 0);
  test_free(imagearray);
#else
  fntable[0].fnptr = int_reduce_fn;
  fntable[0].flags = 0;

  gasnet_coll_init(NULL, 0, fntable, 1, 0);
#endif

  COLL_BARRIER();

  for(flag_iter=0; flag_iter<9; flag_iter++) {
    int flags;
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
#if SINGLE_SINGLE_MODE_ENABLED || ALL_ADDR_MODE_ENABLED
      if (!TEST_ALIGNED_SEGMENTS()) {
        if(td->my_local_thread == 0 && !skip_msg_printed)
          MSG0("Skipping SINGLE/SINGLE test (unaligned segments)");
      } else if (threads_per_node != 1) { 
	if(td->my_local_thread == 0 && !skip_msg_printed)
          MSG0("skipping SINGLE/SINGLE test (multiple threads per node)");
      } else {
	for(size = 1; size<=max_data_size; size=size*2) {
	  run_SINGLE_ADDR_test(td, all_dsts, all_srcs, size, root_thread, flags|GASNET_COLL_SINGLE);
	}
      }
      skip_msg_printed =1;
#endif

#if SINGLE_LOCAL_MODE_ENABLED || ALL_ADDR_MODE_ENABLED
      for(size = 1; size<=max_data_size; size=size*2) {
        run_SINGLE_ADDR_test(td, my_dsts, my_srcs, size, root_thread, flags|GASNET_COLL_LOCAL);   
      }
#endif

#if MULTI_SINGLE_MODE_ENABLED || ALL_ADDR_MODE_ENABLED
      for(size = 1; size<=max_data_size; size=size*2) {
        run_MULTI_ADDR_test(td, all_dsts, all_srcs, size, root_thread, flags|GASNET_COLL_SINGLE);
      }
#endif      

#if MULTI_LOCAL_MODE_ENABLED || ALL_ADDR_MODE_ENABLED
      for(size = 1; size<=max_data_size; size=size*2) {
        run_MULTI_ADDR_test(td, my_dsts, my_srcs, size, root_thread, flags|GASNET_COLL_LOCAL);
      }
#endif
      if(td->my_local_thread==0  && !VERBOSE_VERIFICATION_OUTPUT) {
        char flag_str[8];
        fill_flag_str(flags, flag_str);
        MSG0("%c: sync_mode: %s %ld-%ld (powers of 2) bytes root: %d.  PASS",  TEST_SECTION_NAME(), flag_str, (long int) (sizeof(int)*1), (long int) sizeof(int)*max_data_size, (int) root_thread);
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
  thread_data_t *td_arr;
  
  GASNET_Safe(gasnet_init(&argc, &argv));
  
  
  if(argc > 1) {
    max_data_size = MAX(atoi(argv[1])/sizeof(int),1); 
  } else {
    max_data_size = DEFAULT_MAX_DATA_SIZE/sizeof(int);
  }
  
  if (argc > 2) {
    outer_verification_iters = MAX(1,atoi(argv[2]));
  } else {
    outer_verification_iters = DEFAULT_OUTER_VERIFICATION_ITERS;
  }

  
  if (argc > 3) {
    inner_verification_iters = MAX(1,atoi(argv[3]));
  } else {
    inner_verification_iters = DEFAULT_INNER_VERIFICATION_ITERS;
  }
  /* make sure that there is at least 1 inner verification iteration 
     since this is waht we use for the performance runs*/
  inner_verification_iters = MAX(1, inner_verification_iters);
  
  if(argc > 4) {
    performance_iters = atoi(argv[4]);
  } else {
    performance_iters = DEFAULT_PERFORMANCE_ITERS;
  }
  
#if GASNET_PAR
  if (argc > 5) {
    threads_per_node = atoi(argv[5]);
  } else {
    threads_per_node = gasnett_cpu_count(); 
    threads_per_node = MIN(threads_per_node, 8);
  }
  if (threads_per_node > TEST_MAXTHREADS || threads_per_node < 1) {
    printf("ERROR: Threads must be between 1 and %d\n", TEST_MAXTHREADS);
    exit(EXIT_FAILURE);
  }
  if (argc > 6) TEST_SECTION_PARSE(argv[6]);
#else
  threads_per_node = 1;
  if (argc > 5) TEST_SECTION_PARSE(argv[5]);
#endif  
  
  /* get SPMD info */
  mynode = gasnet_mynode();
  nodes = gasnet_nodes();
  THREADS = nodes * threads_per_node;
  
  /* do some sanity checking of the input arguments*/
  /* the total memory that we will need to attach is inner_verification_iters*total_images*my_images*2*sizeof(int)*max_data_size*/
  /* make sure that this value is about less than or equal to half the maximum gasnet segment */
  /* if it is not scale down inner_verification_iters and scale up outer_verification_iters until we match*/
  {
    size_t curr_req = inner_verification_iters * THREADS * threads_per_node * sizeof(int) * max_data_size * 2;
    size_t max_mem_usage = gasnet_getMaxGlobalSegmentSize()/2;
    MSG0("command line args: max_data_size=%ld bytes outer_verification_iters=%d inner_verification_iters=%d performance_iters=%d threads_per_node=%d ", (long int)(max_data_size*sizeof(int)), 
         outer_verification_iters, inner_verification_iters, performance_iters, (int) threads_per_node);
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
      MSG0("adjusted args: max_data_size=%ld bytes outer_verification_iters=%d inner_verification_iters=%d performance_iters=%d threads_per_node=%d ", (long int)(max_data_size*sizeof(int)), 
           outer_verification_iters, inner_verification_iters, performance_iters, (int) threads_per_node);
    } 
  }
  
 
  GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
  test_init("testcollperf",0,"(max data size) (outer_verification_iters) (inner_verification_iters) (performance_iters) (thread count per node) ");
  TEST_SET_WAITMODE(threads_per_node);
  A = TEST_MYSEG();
  B = A+(SEG_PER_THREAD*threads_per_node);
  my_srcs =  (uint8_t**) test_malloc(sizeof(uint8_t*)*threads_per_node);
  my_dsts =  (uint8_t**) test_malloc(sizeof(uint8_t*)*threads_per_node);
  all_srcs = (uint8_t**) test_malloc(sizeof(uint8_t*)*THREADS);
  all_dsts = (uint8_t**) test_malloc(sizeof(uint8_t*)*THREADS);
  td_arr = (thread_data_t*) test_malloc(sizeof(thread_data_t)*threads_per_node);
  
  for(i=0; i<threads_per_node; i++) {
    my_srcs[i] = A + i*SEG_PER_THREAD;
    my_dsts[i] = B + i*SEG_PER_THREAD;
    td_arr[i].my_local_thread = i;
    td_arr[i].mythread = mynode*threads_per_node+i;
    td_arr[i].mysrc = my_srcs[i];
    td_arr[i].mydest = my_dsts[i];
  }
  for(i=0; i<nodes; i++) {
    /*    assert_always(TEST_SEG(i).size >= SEG_PER_THREAD*threads_per_node); */
    for(j=0; j<threads_per_node; j++) {
      all_srcs[i*threads_per_node+j] = (uint8_t*) TEST_SEG(i) + j*SEG_PER_THREAD;
      all_dsts[i*threads_per_node+j] = (uint8_t*) TEST_SEG(i) + SEG_PER_THREAD*threads_per_node + j*SEG_PER_THREAD;
    }
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

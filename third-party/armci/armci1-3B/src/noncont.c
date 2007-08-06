/*
 * noncont.c
 *
 * Developed by Andriy Kot <andriy.kot@pnl.gov>
 * Copyright (c) 2006 Pacific Northwest National Laboratory
 *
 * Alternative version of non-contiguous calls using non-blocking ones
 *
 * Changelog:
 * 2006-09-08 - created
 *
 */

#include "armcip.h"
#include "copy.h"
#include "acc.h"
#include "memlock.h"
#include <stdio.h>

#ifdef  NB_NONCONT

#if   defined(QUADRICS)
typedef ELAN_EVENT *HTYPE;
#else
typedef void *HTYPE;
#endif

#define MAX_SLOTS_LL    64
#define MIN_OUTSTANDING 6
static int max_pending = 16; /* throttle number of outstanding nb calls */

/* might have to use MAX_SLOTS_LL < MAX_PENDING due to throttling problem */
#define MAX_PENDING 6
#define ZR (HTYPE)0

static HTYPE put_dscr[MAX_SLOTS_LL]= {
ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,
ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR};

static HTYPE get_dscr[MAX_SLOTS_LL] = {
ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,
ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR};

static int cur_get=0;
static int cur_put=0;
static int pending_get=0;
static int pending_put=0;

/* strided put, nonblocking */
void armcill_put2D(int proc, int bytes, int count, void* src_ptr,int src_stride,
                                                   void* dst_ptr,int dst_stride)
{
    int _j, i, batch, issued=0;
    char *ps=src_ptr, *pd=dst_ptr;

    for (_j = 0;  _j < count;  ){
      /* how big a batch of requests can we issue */
      batch = (count - _j )<max_pending ? count - _j : max_pending;
      _j += batch;
#ifdef SHMEM_HANDLE_SUPPORTED
      for(i=0; i< batch; i++){
        if (put_dscr[cur_put]) armcill_nb_wait(put_dscr[cur_put]);
        else pending_put++;
        put_dscr[cur_put] = armcill_nb_put(pd,ps,bytes,proc,put_dscr+cur_put);
#else
      shmem_quiet();
      for(i=0; i< batch; i++){
        armcill_nb_put(pd,ps,bytes,proc,ZR);
#endif
        issued++;
        ps += src_stride;
        pd += dst_stride;
        cur_put++;
        if(cur_put>=max_pending)cur_put=0;
      }
    }

    if(issued != count)
       armci_die2("armcill_put2D: mismatch %d %d \n", count,issued);
}


/* blocking vector put */
void armcill_putv(int proc, int bytes, int count, void* src[], void* dst[])
{
    int _j, i, batch, issued=0;
    void *ps, *pd;

    for (_j = 0;  _j < count;  ){
      /* how big a batch of requests can we issue */
      batch = (count - _j )<max_pending ? count - _j : max_pending;
      _j += batch;
#ifdef SHMEM_HANDLE_SUPPORTED
      for(i=0; i< batch; i++){
        if (put_dscr[cur_put]) armcill_nb_wait(put_dscr[cur_put]);
        else pending_put++;
        ps = src[issued];
        pd = dst[issued];
        put_dscr[cur_put] = armcill_nb_put(pd,ps,bytes,proc,put_dscr+cur_put);
#else
      shmem_quiet();
      for(i=0; i< batch; i++){
        armcill_nb_put(pd,ps,bytes,proc,ZR);
#endif
        issued++;
        cur_put++;
        if(cur_put>=max_pending)cur_put=0;
      }
    }
    if(issued != count)
       armci_die2("armcill_putv: mismatch\n", count,issued);

#ifdef SHMEM_HANDLE_SUPPORTED
    for(i=0; i<max_pending; i++) if(put_dscr[i]){
        armcill_nb_wait(put_dscr[i]);
        put_dscr[i]=ZR;
    }
#else
    shmem_quiet();
#endif
}




/* strided get, nonblocking */
void armcill_get2D(int proc, int bytes, int count, void* src_ptr,int src_stride,
                                                   void* dst_ptr,int dst_stride)
{
    int _j, i, batch, issued=0;
    char *ps=src_ptr, *pd=dst_ptr;

    for (_j = 0;  _j < count;  ){
      /* how big a batch of requests can we issue */
      batch = (count - _j )<max_pending ? count - _j : max_pending;
      _j += batch;
#ifdef SHMEM_HANDLE_SUPPORTED
      for(i=0; i< batch; i++){
        if (get_dscr[cur_get]) armcill_nb_wait(get_dscr[cur_get]);
        else pending_get++;
        get_dscr[cur_get] = armcill_nb_get(pd,ps,bytes,proc,get_dscr+cur_get);
#else
      shmem_quiet();
      for(i=0; i< batch; i++){
        armcill_nb_get(pd,ps,bytes,proc,ZR);
#endif
        issued++;
        ps += src_stride;
        pd += dst_stride;
        cur_get++;
        if(cur_get>=max_pending)cur_get=0;
      }
    }

    if(issued != count)
       armci_die2("armcill_get2D: mismatch %d %d \n", count,issued);
}


/* blocking vector get */
void armcill_getv(int proc, int bytes, int count, void* src[], void* dst[])
{
    int _j, i, batch, issued=0;
    void *ps, *pd;

    for (_j = 0;  _j < count;  ){
      /* how big a batch of requests can we issue */
      batch = (count - _j )<max_pending ? count - _j : max_pending;
      _j += batch;
#ifdef SHMEM_HANDLE_SUPPORTED
      for(i=0; i< batch; i++){
        if (get_dscr[cur_get]) armcill_nb_wait(get_dscr[cur_get]);
        else pending_get++;
        ps = src[issued];
        pd = dst[issued];
        get_dscr[cur_get] = armcill_nb_get(pd,ps,bytes,proc,get_dscr+cur_get);
#else
      shmem_quiet();
      for(i=0; i< batch; i++){
        armcill_nb_get(pd,ps,bytes,proc,ZR);
#endif
        issued++;
        cur_get++;
        if(cur_get>=max_pending)cur_get=0;
      }
    }
    if(issued != count)
       armci_die2("armcill_getv: mismatch %d %d \n", count,issued);

#ifdef SHMEM_HANDLE_SUPPORTED
    for(i=0; i<max_pending; i++) if(get_dscr[i]){
        armcill_nb_wait(get_dscr[i]);
        get_dscr[i]=ZR;
    }
#else
    shmem_quiet();
#endif
}


void armcill_wait_get()
{
#ifdef SHMEM_HANDLE_SUPPORTED
    int i;
    if(!pending_get)return;
    else pending_get=0;
    for(i=0; i<max_pending; i++) if(get_dscr[i]){
        armcill_nb_wait(get_dscr[i]);
        get_dscr[i]=ZR;
    }
#else
    shmem_quiet();
#endif
}


void armcill_wait_put()
{
#ifdef SHMEM_HANDLE_SUPPORTED
    int i;
    if(!pending_put)return;
    else pending_put=0;
    for(i=0; i<max_pending; i++) if(put_dscr[i]){
        armcill_nb_wait(put_dscr[i]);
        put_dscr[i]=ZR;
    }
#else
    shmem_quiet();
#endif
}


#endif/*NB_NONCONT*/

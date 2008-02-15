/* $Id: rmw.c,v 1.21.2.6 2007/09/25 18:44:16 manoj Exp $ */
#include "armcip.h"
#include "locks.h"
#include "copy.h"
#include <stdio.h>

#ifdef LIBELAN_ATOMICS 

ELAN_ATOMIC *a;

int elan_int_fadd(int *target, int inc, int vp)
{
    int result;

    elan_wait(elan_atomic32(a, ELAN_ATOMIC_ADD, target, inc, 0, vp, &result), elan_base->waitType);
    return(result);
}

int elan_long_fadd(long *target, long inc, int vp)
{
    long result;
    
#ifdef _LP64
    elan_wait(elan_atomic64(a, ELAN_ATOMIC_ADD, target, inc, 0, vp, &result), elan_base->waitType);
#else
    elan_wait(elan_atomic32(a, ELAN_ATOMIC_ADD, target, inc, 0, vp, &result), elan_base->waitType);
#endif

    return(result);
}

int elan_int_swap(int *target, int value, int vp)
{
    int result;

    elan_wait(elan_atomic32(a, ELAN_ATOMIC_SWAP, target, value, 0, vp, &result), elan_base->waitType);
    return(result);
}

int elan_long_swap(long *target, long value, int vp)
{
    long result;
    
#ifdef _LP64
    elan_wait(elan_atomic64(a, ELAN_ATOMIC_SWAP, target, value, 0, vp, &result), elan_base->waitType);
#else
    elan_wait(elan_atomic32(a, ELAN_ATOMIC_SWAP, target, value, 0, vp, &result), elan_base->waitType);
#endif

    return(result);
}
#endif /* LIBELAN_ATOMICS */

/* enable use of newer interfaces in SHMEM */
#ifndef CRAY
#ifndef LIBELAN_ATOMICS
/* manpages for shmem_fadd exist on the T3E but library code does not */
#define SHMEM_FADD 
#endif
#endif


#ifdef GA_USE_VAMPIR
#include "armci_vampir.h"
#endif

/* global scope to prevent compiler optimization of volatile code */
int  _a_temp;
long _a_ltemp;

void armci_generic_rmw(int op, void *ploc, void *prem, int extra, int proc)
{
#if defined(CLUSTER) && !defined(SGIALTIX)
    int lock = (proc-armci_clus_info[armci_clus_id(proc)].master)%NUM_LOCKS;
#else
    int lock = 0;
#endif
    NATIVE_LOCK(lock,proc);

    switch (op) {
      case ARMCI_FETCH_AND_ADD:
                armci_get(prem,ploc,sizeof(int),proc);
                _a_temp = *(int*)ploc + extra;
                armci_put(&_a_temp,prem,sizeof(int),proc);
           break;
      case ARMCI_FETCH_AND_ADD_LONG:
                armci_get(prem,ploc,sizeof(long),proc);
                _a_ltemp = *(long*)ploc + extra;
                armci_put(&_a_ltemp,prem,sizeof(long),proc);
           break;
      case ARMCI_SWAP:
                armci_get(prem,&_a_temp,sizeof(int),proc);
                armci_put(ploc,prem,sizeof(int),proc);
                *(int*)ploc = _a_temp; 
           break;
      case ARMCI_SWAP_LONG:
                armci_get(prem,&_a_ltemp,sizeof(long),proc);
                armci_put(ploc,prem,sizeof(long),proc);
                *(long*)ploc = _a_ltemp;
           break;
      default: armci_die("rmw: operation not supported",op);
    }
#ifdef VAPI
    if(!SERVER_CONTEXT)
#endif
      ARMCI_Fence(proc); 
    NATIVE_UNLOCK(lock,proc);
}


int ARMCI_Rmw(int op, int *ploc, int *prem, int extra, int proc)
{
#ifdef LAPI64
    extern int LAPI_Rmw64(lapi_handle_t hndl, RMW_ops_t op, uint tgt, 
             long long *tgt_var,
             long long *in_val, long long *prev_tgt_val, lapi_cntr_t *org_cntr);
    long long llval, *pllarg = (long long*)ploc, lltmp;
/* enable RMWBROKEN if RMW fails for long datatype */
#define RMWBROKEN_ 
#endif

#ifdef LAPI
    int  ival, rc, opcode=SWAP, *parg=ploc;
    lapi_cntr_t req_id;
#elif defined(_CRAYMPP) || defined(QUADRICS) || defined(XT3)
    int  ival;
    long lval;
#endif

#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_RMW,__FILE__,__LINE__);
#endif

#if defined(LAPI64) && defined(RMWBROKEN)
/* hack for rmw64 BROKEN: we operate on least significant part of long */
if(op==ARMCI_FETCH_AND_ADD_LONG || op==ARMCI_SWAP_LONG){
  ploc[0]=0;
  ploc[1]=0;
  ploc++;
  parg ++; prem++;
}
#endif

#if defined(CLUSTER) && !defined(LAPI) && !defined(QUADRICS) &&!defined(CYGWIN)\
    && !defined(HITACHI) && !defined(XT3)
     if(!SAMECLUSNODE(proc)){
       armci_rem_rmw(op, ploc, prem,  extra, proc);
       return 0;
     }
#endif

#ifdef REGION_ALLOC
     if(SAMECLUSNODE(proc)) (void)armci_region_fixup(proc,&prem);
#endif
#ifdef BGML     
   BGML_Op oper;
   BGML_Dt dt;  
   void *temp;
   long ltemp;
   switch(op)   
   {            
                
      case ARMCI_FETCH_AND_ADD:
      case ARMCI_FETCH_AND_ADD_LONG:
         dt=BGML_SIGNED_INT;
         temp=(int *)&extra;
         oper=BGML_SUM;
         break;
#if 0 
      case ARMCI_FETCH_AND_ADD_LONG:
         fprintf(stderr,"adding int to longs....\n");
         dt=BGML_SIGNED_LONG;
         ltemp=(long)extra;
         temp=&ltemp;
         oper=BGML_SUM;
         break;
#endif
      case ARMCI_SWAP:
      case ARMCI_SWAP_LONG:
         dt=BGML_SIGNED_INT;
         oper=BGML_NOOP;
         temp=(int *)ploc;
         break;
#if 0
      case ARMCI_SWAP_LONG:
         fprintf(stderr,"long armci_swap\n");
         dt=BGML_SIGNED_LONG;
         oper=BGML_NOOP;
         temp=(long *)ploc;
         break;
#endif
      default:
         ARMCI_Error("Invalid operation for RMW", op);
   }
    
// int ARMCI_Rmw(int op, int *ploc, int *prem, int extra, int proc)
// // assumes ploc will change
// // dstbuf=prem, input=temp(extra), output=ploc
// // val=ploc, arr[0]=prem, 1=extra
//
    int me=armci_msg_me();
    BG1S_t request; 
    unsigned done=1;
    BGML_Callback_t cb_wait={wait_callback, &done};
    BG1S_rmw(&request, proc, 0, prem, temp, ploc, oper, dt, &cb_wait, 1);
    BGML_Wait(&done);
#else
    switch (op) {
#   if defined(QUADRICS) || defined(_CRAYMPP) || defined(XT3)
      case ARMCI_FETCH_AND_ADD:
#ifdef SHMEM_FADD
         /* printf(" calling intfdd arg %x %ld \n", prem, *prem); */
          *(int*) ploc = shmem_int_fadd(prem, extra, proc);
#elif defined(LIBELAN_ATOMICS)
          *(int*) ploc = elan_int_fadd(prem, extra, proc);
#else
          while ( (ival = shmem_int_swap(prem, INT_MAX, proc) ) == INT_MAX);
          (void) shmem_int_swap(prem, ival +extra, proc);
          *(int*) ploc = ival;
#endif
        break;
      case ARMCI_FETCH_AND_ADD_LONG:
#ifdef SHMEM_FADD
          *(long*) ploc = shmem_long_fadd( (long*)prem, (long) extra, proc);
#elif defined(LIBELAN_ATOMICS)
          *(long*) ploc = elan_long_fadd( (long*)prem, (long) extra, proc);
#else
          while ((lval=shmem_long_swap((long*)prem,LONG_MAX,proc)) == LONG_MAX);
          (void) shmem_long_swap((long*)prem, (lval + extra), proc);
          *(long*)ploc   = lval;
#endif
        break;
      case ARMCI_SWAP:
#ifdef LIBELAN_ATOMICS
          *(int*)ploc = elan_int_swap((int*)prem, *(int*)ploc,  proc); 
#else
          *(int*)ploc = shmem_int_swap((int*)prem, *(int*)ploc,  proc); 
#endif
        break;
      case ARMCI_SWAP_LONG:
#ifdef LIBELAN_ATOMICS
          *(long*)ploc = elan_long_swap((long*)prem, *(long*)ploc,  proc); 
#else
          *(long*)ploc = shmem_swap((long*)prem, *(long*)ploc,  proc); 
#endif
        break;
#   elif defined(LAPI)
#     if defined(LAPI64) && !defined(RMWBROKEN)
        case ARMCI_FETCH_AND_ADD_LONG:
           opcode = FETCH_AND_ADD;
           lltmp  = (long long)extra;
           pllarg = &lltmp;
        case ARMCI_SWAP_LONG:
#if 0
          printf("before opcode=%d rem=%ld, loc=(%ld,%ld) extra=%ld\n",
                  opcode,*prem,*(long*)ploc,llval, lltmp);  
          rc= sizeof(long);
          ARMCI_Get(prem, &llval, rc, proc);
          printf("%d:rem val before %ld\n",armci_me, llval); fflush(stdout);
#endif
          if( rc = LAPI_Setcntr(lapi_handle,&req_id,0))
                        armci_die("rmw setcntr failed",rc);
          if( rc = LAPI_Rmw64(lapi_handle, opcode, proc, (long long*)prem,
                        pllarg, &llval, &req_id)) armci_die("rmw failed",rc);
          if( rc = LAPI_Waitcntr(lapi_handle, &req_id, 1, NULL))
                        armci_die("rmw wait failed",rc);

          *(long*)ploc  = (long)llval;
#if 0
          rc= sizeof(long);
          ARMCI_Get(prem, &lltmp, rc, proc);
          printf("%d:after rmw remote val from rmw=%ld and get=%ld extra=%d\n",
                  armci_me,llval, lltmp,extra);  
#endif
        break;
#     endif
      /************** here sizeof(long)= sizeof(int) **************/
      case ARMCI_FETCH_AND_ADD:
#     if !defined(LAPI64) || defined(RMWBROKEN)
        case ARMCI_FETCH_AND_ADD_LONG:
#     endif
           opcode = FETCH_AND_ADD;
           parg = &extra;
      case ARMCI_SWAP:
#     if !defined(LAPI64) || defined(RMWBROKEN)
        case ARMCI_SWAP_LONG:
#     endif
          /* Within SMPs LAPI_Rmw needs target's address. */
          if(SAMECLUSNODE(proc)) proc=armci_me;
           
          if( rc = LAPI_Setcntr(lapi_handle,&req_id,0))
                        armci_die("rmw setcntr failed",rc);
          if( rc = LAPI_Rmw(lapi_handle, opcode, proc, prem,
                        parg, &ival, &req_id)) armci_die("rmw failed",rc);
          if( rc = LAPI_Waitcntr(lapi_handle, &req_id, 1, NULL))
                        armci_die("rmw wait failed",rc);
          *ploc  = ival;
        break;
#   else
      case ARMCI_FETCH_AND_ADD:
      case ARMCI_FETCH_AND_ADD_LONG:
      case ARMCI_SWAP:
      case ARMCI_SWAP_LONG:
           armci_generic_rmw(op, ploc, prem,  extra, proc);
        break;
#   endif
      default: armci_die("rmw: operation not supported",op);
    }
#endif /*bgml*/
#ifdef GA_USE_VAMPIR
    vampir_end(ARMCI_RMW,__FILE__,__LINE__);
#endif

    return 0;
}


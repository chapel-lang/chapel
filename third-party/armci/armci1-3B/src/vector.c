/* $Id$ */
#include "armcip.h"
#include "copy.h"
#include "acc.h"
#include "memlock.h"
#include <stdio.h>
#include <assert.h>

#ifdef GA_USE_VAMPIR
#include "armci_vampir.h"
#endif
#ifdef ARMCI_PROFILE
#include "armci_profile.h"
#endif

typedef struct {
    float real;
    float imag;
} complex_t;

typedef struct {
    double real;
    double imag;
} dcomplex_t;

/*
void I_ACCUMULATE(void* scale, int elems, void*src, void* dst)
{
    int j;
    int *a=(int*)dst, *b=(int*)src;
    int alpha = *(int*)scale;

    for(j=0;j<elems;j++) a[j] += alpha*b[j];
}
*/


#define ACCUMULATE( DTYPE, scale, elems, src, dst) {\
    int j;\
    DTYPE *a =(DTYPE *)(dst);\
    DTYPE *b =(DTYPE *)(src);\
    DTYPE alpha = *(DTYPE *)(scale);\
    for(j=0;j<(elems);j++)a[j] += alpha*b[j];\
}
        
#define CPL_ACCUMULATE( DTYPE, scale, elems, src, dst) {\
    int j;\
    DTYPE *a =(DTYPE *)(dst);\
    DTYPE *b =(DTYPE *)(src);\
    DTYPE alpha = *(DTYPE *)(scale);\
    for(j=0;j<(elems);j++){\
        a[j].real += alpha.real*b[j].real - alpha.imag*b[j].imag;\
        a[j].imag += alpha.imag*b[j].real + alpha.real*b[j].imag;\
    }\
}



/*\ compute address range for memory to lock 
\*/
void armci_lockmem_scatter(void *ptr_array[], int len, int bytes, int proc)
{
     int i;
     void *pmin, *pmax;

     pmin=ptr_array[0];
     pmax=ptr_array[0];

     for(i = 0; i< len; i++){
              pmin = MIN(ptr_array[i],pmin);
              pmax = MAX(ptr_array[i],pmax);
     }
     pmax =  bytes-1 + (char*)pmax;
     ARMCI_LOCKMEM(pmin, pmax, proc);

/*    printf("%d: locked %ld-%ld bytes=%d\n",armci_me,pmin,pmax,
     1+(char*)pmax -(char*)pmin);fflush(stdout); */  
}



void armci_scatter_acc(int op, void *scale, armci_giov_t dsc, 
                                            int proc, int lockit)
{
#   define ITERATOR for(i = 0; i< dsc.ptr_array_len; i++)
    int i, elems, size;

      if(lockit)
         armci_lockmem_scatter(dsc.dst_ptr_array, dsc.ptr_array_len, 
                               dsc.bytes, proc); 

      switch (op){
      case ARMCI_ACC_INT:
          size  = sizeof(int);
          elems = dsc.bytes/size;
          if(dsc.bytes%size) armci_die("ARMCI vector accumulate: bytes not consistent with datatype",dsc.bytes);
          ITERATOR{
            ACCUMULATE(int, scale, elems, dsc.src_ptr_array[i], dsc.dst_ptr_array[i])
          }
          break;

      case ARMCI_ACC_LNG:
          size  = sizeof(int);
          elems = dsc.bytes/size;          
          if(dsc.bytes%size) armci_die("ARMCI vector accumulate: bytes not consistent with datatype",dsc.bytes);
          ITERATOR{
            ACCUMULATE(long, scale, elems, dsc.src_ptr_array[i], dsc.dst_ptr_array[i])
          }
          break;

      case ARMCI_ACC_DBL:
          size  = sizeof(double);      
          elems = dsc.bytes/size;
          if(dsc.bytes%size) armci_die("ARMCI vector accumulate: bytes not consistent with datatype",dsc.bytes);
          ITERATOR{
            ACCUMULATE(double, scale, elems, dsc.src_ptr_array[i], dsc.dst_ptr_array[i])
          }
          break;

      case ARMCI_ACC_DCP:
          size  = 2*sizeof(double);       
          elems = dsc.bytes/size;
          if(dsc.bytes%size) armci_die("ARMCI vector accumulate: bytes not consistent with datatype",dsc.bytes);
          ITERATOR{
            CPL_ACCUMULATE(dcomplex_t, scale, elems, dsc.src_ptr_array[i], dsc.dst_ptr_array[i])
          }
          break;

      case ARMCI_ACC_CPL:
          size  = 2*sizeof(float);      
          elems = dsc.bytes/size;
          if(dsc.bytes %size) armci_die("ARMCI vector accumulate: bytes not consistent with datatype",dsc.bytes);
          ITERATOR{
            CPL_ACCUMULATE(complex_t, scale, elems, dsc.src_ptr_array[i], dsc.dst_ptr_array[i])
          }
          break;

      case ARMCI_ACC_FLT:
          size  = sizeof(float);      
          elems = dsc.bytes/size;
          if(dsc.bytes%size) armci_die("ARMCI vector accumulate: bytes not consistent with datatype",dsc.bytes);
          ITERATOR{
            ACCUMULATE(float, scale, elems, dsc.src_ptr_array[i], dsc.dst_ptr_array[i])
          }
          break;
      default: armci_die("ARMCI vector accumulate: operation not supported",op);
      }

      if(lockit) ARMCI_UNLOCKMEM(proc);
}


#ifdef ACC_COPY
#  define PWORKLEN 2048
   static void *pwork[PWORKLEN];  /* work array of pointers */
#endif

int armci_acc_vector(int op,             /* operation code */
                    void *scale,         /* pointer to scale factor in accumulate */
                    armci_giov_t darr[], /* descriptor array */
                    int len,             /* length of descriptor array */
                    int proc             /* remote process(or) ID */
              )
{
    int i;

#if defined(ACC_COPY)
    if(proc == armci_me ){
#endif

       for(i = 0; i< len; i++) armci_scatter_acc(op, scale, darr[i], proc, 1);
#if defined(ACC_COPY)
    }else{

       for(i = 0; i< len; i++){
           armci_giov_t dr =  darr[i];
           int j, rc, nb;

           if(dr.bytes > BUFSIZE/2){
               /* for large segments use strided implementation */
               for(j=0; j< dr.ptr_array_len; j++){
                   rc = armci_acc_copy_strided(op, scale,proc, 
                           dr.src_ptr_array[j], NULL, dr.dst_ptr_array[j],NULL,
                           &dr.bytes, 0);
                   if(rc)return(rc);
               }

           }else{

               armci_giov_t dl;

               /*lock memory:should optimize it to lock only a chunk at a time*/
               armci_lockmem_scatter(dr.dst_ptr_array, dr.ptr_array_len, dr.bytes, proc);

               /* copy as many blocks as possible into the local buffer */
               dl.bytes = dr.bytes;
               nb = MIN(PWORKLEN,BUFSIZE/dr.bytes);

               for(j=0; j< dr.ptr_array_len; j+= nb){
                   int nblocks = MIN(nb, dr.ptr_array_len -j);
                   int k;

                   /* setup vector descriptor for remote memory copy 
                      to bring data into buffer*/

                   dl.ptr_array_len = nblocks;
                   dl.src_ptr_array = dr.dst_ptr_array + j; /* GET destination becomes source for copy */
                   for(k=0; k< nblocks; k++) pwork[k] = k*dl.bytes + (char*)armci_internal_buffer;
                   dl.dst_ptr_array = pwork;

                   /* get data to the local buffer */
                   rc = armci_copy_vector(GET, &dl, 1, proc);
                   if(rc){ ARMCI_UNLOCKMEM(proc); return(rc);}

                   /* update source array for accumulate */
                   dl.src_ptr_array = dr.src_ptr_array +j;

                   /* do scatter accumulate updating copy of data in buffer */
                   armci_scatter_acc(op, scale, dl, armci_me, 0);

                   /* modify descriptor-now source becomes destination for PUT*/
                   dl.dst_ptr_array = dr.dst_ptr_array + j;
                   dl.src_ptr_array = pwork;

                   /* put data back */
                   rc = armci_copy_vector(PUT, &dl, 1, proc);
                   FENCE_NODE(proc);

                   if(rc){ ARMCI_UNLOCKMEM(proc); return(rc);}
               }

               ARMCI_UNLOCKMEM(proc);
           }
       }/*endfor*/
    }
#endif

    return 0;
}




int armci_copy_vector(int op,            /* operation code */
                    armci_giov_t darr[], /* descriptor array */
                    int len,             /* length of descriptor array */
                    int proc             /* remote process(or) ID */
              )
{
    int i,s,shmem= SAMECLUSNODE(proc);

    if(shmem ){ 
      /* local/shared memory copy */

      for(i = 0; i< len; i++){
        for( s=0; s< darr[i].ptr_array_len; s++){
           armci_copy(darr[i].src_ptr_array[s],darr[i].dst_ptr_array[s],darr[i].bytes);
        }
      }

    }else {   

      /* access through global address space */

      /* March 19 - removed FENCE from here - it is in ORDER inside armci.c */

      switch(op){
      case PUT:

        for(i = 0; i< len; i++){

#ifdef QUADRICS
          armcill_putv(proc, darr[i].bytes, darr[i].ptr_array_len,
                       darr[i].src_ptr_array, darr[i].dst_ptr_array); 
#else
#         ifdef LAPI
                SET_COUNTER(ack_cntr,darr[i].ptr_array_len);
#         endif
          UPDATE_FENCE_STATE(proc, PUT, darr[i].ptr_array_len);
 
          for( s=0; s< darr[i].ptr_array_len; s++){   
              armci_put(darr[i].src_ptr_array[s],darr[i].dst_ptr_array[s],
                        darr[i].bytes, proc);
           }
#endif
        }
        break;

      case GET:

        for(i = 0; i< len; i++){

#ifdef QUADRICS
          armcill_getv(proc, darr[i].bytes, darr[i].ptr_array_len,
                       darr[i].src_ptr_array, darr[i].dst_ptr_array); 
#else
#         ifdef LAPI
                SET_COUNTER(get_cntr,darr[i].ptr_array_len);
#         endif

          for( s=0; s< darr[i].ptr_array_len; s++){   
              armci_get(darr[i].src_ptr_array[s],darr[i].dst_ptr_array[s],
                        darr[i].bytes,proc);
           }
#endif
        }
        break;

      default:
          armci_die("armci_copy_vector: wrong optype",op);
      }
   }

#ifdef LAPI
    if(!shmem){

       if(op == GET) CLEAR_COUNTER(get_cntr); /* wait for data arrival */
       if(op == PUT) CLEAR_COUNTER(ack_cntr); /* data must be copied out*/
    }
#endif

   return 0;
}


void armci_vector_to_buf(armci_giov_t darr[], int len, void* buf)
{
int i,s;
char *ptr = (char*)buf; 

      for(i = 0; i< len; i++){
        for( s=0; s< darr[i].ptr_array_len; s++){
          armci_copy(darr[i].src_ptr_array[s],ptr,darr[i].bytes);
          ptr += darr[i].bytes;
        }
      }
}


void armci_vector_from_buf(armci_giov_t darr[], int len, void* buf)
{
int i,s;
char *ptr = (char*)buf;

      for(i = 0; i< len; i++){
        for( s=0; s< darr[i].ptr_array_len; s++){
          armci_copy(ptr, darr[i].dst_ptr_array[s],darr[i].bytes);
          ptr += darr[i].bytes;
        }
      }
}

int ARMCI_PutV( armci_giov_t darr[], /* descriptor array */
                int len,  /* length of descriptor array */
                int proc  /* remote process(or) ID */
              )
{
    int rc=0, i,direct=1;
#if defined(USE_SOCKET_VECTOR_API)
    int totvec=0;
#endif

#ifdef GA_USE_VAMPIR
    int tot=0;
    for(i=0;i<len;i++) tot+=darr[i].bytes;
#endif

    if(len<1) return FAIL;
    for(i=0;i<len;i++){
        if(darr[i].src_ptr_array == NULL || darr[i].dst_ptr_array ==NULL) return FAIL2;
        if(darr[i].bytes<1)return FAIL3;
        if(darr[i].ptr_array_len <1) return FAIL4;
#if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API)  
        totvec+=darr[i].ptr_array_len;
#endif
    }

    if(proc<0 || proc >= armci_nproc)return FAIL5;

#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_PUTV,__FILE__,__LINE__);
    if (armci_me != proc)
       vampir_start_comm(armci_me,proc,tot,ARMCI_PUTV);
#endif
#ifdef ARMCI_PROFILE
    armci_profile_start_vector(darr, len, proc, ARMCI_PROF_PUTV);
#endif

    ORDER(PUT,proc); /* ensure ordering */
#ifndef QUADRICS
    direct=SAMECLUSNODE(proc);
#endif

#ifdef BGML
   armci_hdl_t nb_handle;
   ARMCI_INIT_HANDLE(&nb_handle);
   ARMCI_NbPutV(darr, len, proc, &nb_handle);
   ARMCI_Wait(&nb_handle);
#else

    /* use direct protocol for remote access when performance is better */
#   if defined(LAPI)
      if(!direct)
          if(len <5 || darr[0].ptr_array_len <5) direct=1;
#   endif


    if(direct)
         rc = armci_copy_vector(PUT, darr, len, proc);
    else{
#if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API)  
       /*500 is very conservative, the number here should be modified to be 
       based on the size of send/recv buffer*/
       if(totvec<500)
         rc = armci_rem_vector(PUT, NULL, darr, len, proc, 1,NULL);
       else 
#endif    
         rc = armci_pack_vector(PUT, NULL, darr, len, proc,NULL);
    }

#endif
#ifdef ARMCI_PROFILE
    armci_profile_stop_vector(ARMCI_PROF_PUTV);
#endif
#ifdef GA_USE_VAMPIR
    if (armci_me != proc)
       vampir_end_comm(armci_me,proc,tot,ARMCI_PUTV);
    vampir_end(ARMCI_PUTV,__FILE__,__LINE__);
#endif

    if(rc) return FAIL6;
    else return 0;

}


int ARMCI_GetV( armci_giov_t darr[], /* descriptor array */
                int len,  /* length of descriptor array */
                int proc  /* remote process(or) ID */
              )
{
    int rc=0, i,direct=1;
#if defined(USE_SOCKET_VECTOR_API)
    int totvec=0;
#endif

#ifdef GA_USE_VAMPIR
    int tot=0;
    for(i=0;i<len;i++) tot+=darr[i].bytes;
#endif

    if(len<1) return FAIL;
    for(i=0;i<len;i++){
      if(darr[i].src_ptr_array==NULL ||darr[i].dst_ptr_array==NULL)return FAIL2;
      if(darr[i].bytes<1)return FAIL3;
      if(darr[i].ptr_array_len <1) return FAIL4;
#if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API)  
      totvec+=darr[i].ptr_array_len;
#endif
    }

    if(proc<0 || proc >= armci_nproc)return FAIL5;

#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_GETV,__FILE__,__LINE__);
    if (armci_me != proc)
       vampir_start_comm(proc,armci_me,tot,ARMCI_GETV);
#endif
#ifdef ARMCI_PROFILE
    armci_profile_start_vector(darr, len, proc, ARMCI_PROF_GETV);
#endif

    ORDER(GET,proc); /* ensure ordering */
#ifndef QUADRICS
    direct=SAMECLUSNODE(proc);
#endif

#ifdef BGML
   armci_hdl_t nb_handle;
   ARMCI_INIT_HANDLE(&nb_handle);
   ARMCI_NbGetV(darr, len, proc, &nb_handle);
   ARMCI_Wait(&nb_handle);
#else

    /* use direct protocol for remote access when performance is better */
#   if defined(LAPI)
      if(!direct)
          if(len <5 || darr[0].ptr_array_len <8) direct=1;
#   endif


    if(direct)
       rc = armci_copy_vector(GET, darr, len, proc);
    else{
#if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API) 
       /*500 is very conservative, the number here should be modified to be 
       based on the size of send/recv buffer*/
       if(totvec<500)
          rc = armci_rem_vector(GET, NULL, darr, len, proc,1,NULL);
       else
#endif   
       rc = armci_pack_vector(GET, NULL, darr, len, proc,NULL);
    }

#endif
#ifdef ARMCI_PROFILE
    armci_profile_stop_vector(ARMCI_PROF_GETV);
#endif
#ifdef GA_USE_VAMPIR
    if (armci_me != proc)
       vampir_end_comm(proc,armci_me,tot,ARMCI_GETV);
    vampir_end(ARMCI_GETV,__FILE__,__LINE__);
#endif

    if(rc) return FAIL6;
    else return 0;
}




int ARMCI_AccV( int op,              /* oeration code */
                void *scale,         /*scaling factor for accumulate */
                armci_giov_t darr[], /* descriptor array */
                int len,             /* length of descriptor array */
                int proc             /* remote process(or) ID */
              )
{
    int rc=0, i,direct=1;

#ifdef GA_USE_VAMPIR
    int tot=0;
    for(i=0;i<len;i++) tot+=darr[i].bytes;
#endif

    if(len<1) return FAIL;
    for(i=0;i<len;i++){
      if(darr[i].src_ptr_array==NULL ||darr[i].dst_ptr_array==NULL)return FAIL2;
      if(darr[i].bytes<1)return FAIL3;
      if(darr[i].ptr_array_len <1) return FAIL4;
    }

    if(proc<0 || proc >= armci_nproc)return FAIL5;

#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_ACCV,__FILE__,__LINE__);
    if (armci_me != proc)
       vampir_start_comm(armci_me,proc,tot,ARMCI_ACCV);
#endif
#ifdef ARMCI_PROFILE
    armci_profile_start_vector(darr, len, proc, ARMCI_PROF_ACCV);
#endif

    ORDER(op,proc); /* ensure ordering */
    direct=SAMECLUSNODE(proc);
#ifdef BGML
   armci_hdl_t nb_handle;
   ARMCI_INIT_HANDLE(&nb_handle);
   ARMCI_NbAccV(op, scale, darr, len, proc, &nb_handle);
   ARMCI_Wait(&nb_handle);
#else

#   if defined(ACC_COPY) && !defined(ACC_SMP)
       if(armci_me != proc) direct=0;
#   endif

    if(direct)
         rc = armci_acc_vector( op, scale, darr, len, proc);
    else
         rc = armci_pack_vector(op, scale, darr, len, proc,NULL);

#endif
#ifdef ARMCI_PROFILE
    armci_profile_stop_vector(ARMCI_PROF_ACCV);
#endif
#ifdef GA_USE_VAMPIR
    if (armci_me != proc)
       vampir_end_comm(armci_me,proc,tot,ARMCI_ACCV);
    vampir_end(ARMCI_ACCV,__FILE__,__LINE__);
#endif

    if(rc) return FAIL6;
    else return 0;
}

/*****************************************************************************/

/*\ Non-blocking vector API
\*/
int ARMCI_NbPutV( armci_giov_t darr[], /* descriptor array */
                int len,  /* length of descriptor array */
                int proc, /* remote process(or) ID */
                armci_hdl_t* usr_hdl  /*non-blocking request handle*/
              )
{
    armci_ihdl_t nb_handle = (armci_ihdl_t)usr_hdl;
    int rc=0, i,direct=1;
#if defined(USE_SOCKET_VECTOR_API)
    int totvec=0;
#endif

    if(len<1) return FAIL;
    for(i=0;i<len;i++){
        if(darr[i].src_ptr_array == NULL || darr[i].dst_ptr_array ==NULL) return FAIL2;
        if(darr[i].bytes<1)return FAIL3;
        if(darr[i].ptr_array_len <1) return FAIL4;
#if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API)  
        totvec+=darr[i].ptr_array_len;
#endif
    }

    if(proc<0 || proc >= armci_nproc)return FAIL5;
    
#ifdef ARMCI_PROFILE
    armci_profile_start_vector(darr, len, proc, ARMCI_PROF_NBPUTV);
#endif
#ifndef QUADRICS
    direct=SAMECLUSNODE(proc);
#endif

    /* aggregate put */
    if(nb_handle && nb_handle->agg_flag == SET) {
       if(!direct) {
	  rc=armci_agg_save_giov_descriptor(darr, len, proc, PUT, nb_handle);
#         ifdef ARMCI_PROFILE
	  armci_profile_stop_vector(ARMCI_PROF_NBPUTV);
#         endif	  
	  return rc;
       }
    }
    else {
      
      /*ORDER(PUT,proc);  ensure ordering */
      UPDATE_FENCE_INFO(proc);
      
      /*set tag and op in the nb handle*/
      if(nb_handle){
	nb_handle->tag = GET_NEXT_NBTAG();
	nb_handle->op  = PUT;
	nb_handle->proc= proc;
	nb_handle->bufid=NB_NONE;
      }
      else
	nb_handle = armci_set_implicit_handle(PUT, proc);
    }

    if(direct){
#ifdef BGML
         nb_handle->count = 0;
#endif
         rc = armci_copy_vector(PUT, darr, len, proc);
    }
    else{
#ifdef BGML
         nb_handle->count = 1;
         BGML_Callback_t cb_wait={wait_callback, &nb_handle->count};
         BGML_giov_t *array=(BGML_giov_t *)darr;
         BG1S_MemputV(&nb_handle->cmpl_info, proc, len, 
                      (BGML_giov_t *)darr, 0, &cb_wait, 1);
#else
#if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API)  
       /*500 is very conservative, the number here should be modified to be 
       based on the size of send/recv buffer*/
       if(totvec<500)
         rc = armci_rem_vector(PUT, NULL, darr, len, proc, 1,nb_handle);
       else 
#endif    
         rc = armci_pack_vector(PUT, NULL, darr, len, proc,nb_handle);
#endif /* BGML */
    }

#ifdef ARMCI_PROFILE
    armci_profile_stop_vector(ARMCI_PROF_NBPUTV);
#endif
    if(rc) return FAIL6;
    else return 0;
}

int ARMCI_NbGetV( armci_giov_t darr[], /* descriptor array */
                int len,  /* length of descriptor array */
                int proc, /* remote process(or) ID */
                armci_hdl_t* usr_hdl  /*non-blocking request handle*/
              )
{
    armci_ihdl_t nb_handle = (armci_ihdl_t)usr_hdl;
    int rc=0, i,direct=1;
#if defined(USE_SOCKET_VECTOR_API)
    int totvec=0;
#endif

    if(len<1) return FAIL;
    for(i=0;i<len;i++){
      if(darr[i].src_ptr_array==NULL ||darr[i].dst_ptr_array==NULL)return FAIL2;
      if(darr[i].bytes<1)return FAIL3;
      if(darr[i].ptr_array_len <1) return FAIL4;
#if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API)  
      totvec+=darr[i].ptr_array_len;
#endif
    }

    if(proc<0 || proc >= armci_nproc)return FAIL5;

#ifdef ARMCI_PROFILE
    armci_profile_start_vector(darr, len, proc, ARMCI_PROF_NBGETV);
#endif
#ifndef QUADRICS
    direct=SAMECLUSNODE(proc);
#endif

    /* aggregate get */
    if(nb_handle && nb_handle->agg_flag == SET) {
       if(!direct) {
	  rc=armci_agg_save_giov_descriptor(darr, len, proc, GET, nb_handle);
#         ifdef ARMCI_PROFILE
	  armci_profile_stop_vector(ARMCI_PROF_NBGETV);
#         endif 	
	  return rc;
       }
    }
    else {
      /* ORDER(GET,proc); ensure ordering */      
      if(nb_handle){
	nb_handle->tag = GET_NEXT_NBTAG();
	nb_handle->op  = GET;
	nb_handle->proc= proc;
	nb_handle->bufid=NB_NONE;
      }
      else
	nb_handle = armci_set_implicit_handle(GET, proc);
    }

    if(direct){ 
#ifdef BGML
         nb_handle->count = 0;
#endif
         rc = armci_copy_vector(GET, darr, len, proc);
    }
    else{
#ifdef BGML
       nb_handle->count = 1;
       BGML_Callback_t cb_wait={wait_callback, &nb_handle->count};
       BG1S_MemgetV(&nb_handle->cmpl_info, proc, len, 
                (BGML_giov_t *)darr, 0, &cb_wait, 1);
#else
#if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API) 
       /*500 is very conservative, the number here should be modified to be 
       based on the size of send/recv buffer*/
       if(totvec<500)
          rc = armci_rem_vector(GET, NULL, darr, len, proc,1,nb_handle);
       else
#endif   
       rc = armci_pack_vector(GET, NULL, darr, len, proc,nb_handle);
#endif /* BGML */
    }

#ifdef ARMCI_PROFILE
    armci_profile_stop_vector(ARMCI_PROF_NBGETV);
#endif
    if(rc) return FAIL6;
    else return 0;
}


int ARMCI_NbAccV( int op,              /* oeration code */
                void *scale,         /*scaling factor for accumulate */
                armci_giov_t darr[], /* descriptor array */
                int len,             /* length of descriptor array */
                int proc,            /* remote process(or) ID */
                armci_hdl_t* usr_hdl  /*non-blocking request handle*/
              )
{
    armci_ihdl_t nb_handle = (armci_ihdl_t)usr_hdl;
    int rc=0, i,direct=1;

#ifdef GA_USE_VAMPIR
    int tot=0;
    for(i=0;i<len;i++) tot+=darr[i].bytes;
#endif

    if(len<1) return FAIL;
    for(i=0;i<len;i++)
    {
      if(darr[i].src_ptr_array==NULL ||darr[i].dst_ptr_array==NULL)return FAIL2;
      if(darr[i].bytes<1)return FAIL3;
      if(darr[i].ptr_array_len <1) return FAIL4;
    }

    if(proc<0 || proc >= armci_nproc)return FAIL5;

#ifdef ARMCI_PROFILE
    armci_profile_start_vector(darr, len, proc, ARMCI_PROF_NBACCV);
#endif

#ifdef BGML
    if(nb_handle){
      nb_handle->tag = GET_NEXT_NBTAG();
      nb_handle->op  = op;
      nb_handle->proc= proc;
      nb_handle->bufid=NB_NONE;
    }
    else
      nb_handle = armci_set_implicit_handle(op, proc);
    
    BGML_Dt dt;
    switch(op)
      {
        case ARMCI_ACC_INT:
          dt=BGML_SIGNED_INT;
          break;
        case ARMCI_ACC_LNG:
#if 1
          dt=BGML_SIGNED_LONG;
#else
          dt=BGML_SIGNED_INT;
#endif
          break;
        case ARMCI_ACC_DBL:
          dt=BGML_DOUBLE;
          break;
        case ARMCI_ACC_CPL:
          dt=BGML_SINGLE_COMPLEX;
          break;
        case ARMCI_ACC_FLT:
          dt=BGML_FLOAT;
          break;
        case ARMCI_ACC_DCP:
          dt=BGML_DOUBLE_COMPLEX;
          break;
        default:
          armci_die("Unsupported data operation\n",0);
      }

    nb_handle->count = 1;
    BGML_Callback_t cb_wait={wait_callback, &nb_handle->count};
    BG1S_AccumulateV(&nb_handle->cmpl_info, 
                     proc, 
                     len, 
                     (BGML_giov_t *)darr,
                     scale, 
                     0,
                     dt, 
                     BGML_PROD, 
                     BGML_SUM, 
                     &cb_wait, 
                     1);
#else    

    /* ORDER(op,proc);  ensure ordering */
    UPDATE_FENCE_INFO(proc);
    direct=SAMECLUSNODE(proc);
    
    if(nb_handle){
      nb_handle->tag = GET_NEXT_NBTAG();
      nb_handle->op  = op;
      nb_handle->proc= proc;
      nb_handle->bufid=NB_NONE;
    }
    else
      nb_handle = armci_set_implicit_handle(op, proc);

#   if defined(ACC_COPY) && !defined(ACC_SMP)
       if(armci_me != proc) direct=0;
#   endif

    if(direct){
         rc = armci_acc_vector( op, scale, darr, len, proc);
    }
    else{
         rc = armci_pack_vector(op, scale, darr, len, proc,nb_handle);
    }
#endif /* BGML */

#ifdef ARMCI_PROFILE
    armci_profile_stop_vector(ARMCI_PROF_NBACCV);
#endif
    if(rc) return FAIL6;
    else return 0;
}
/*****************************************************************************/

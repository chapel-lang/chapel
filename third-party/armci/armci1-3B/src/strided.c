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

#define ARMCI_OP_2D(op, scale, proc, src, dst, bytes, count, src_stride, dst_stride,lockit)\
if(op == GET || op ==PUT)\
      armci_copy_2D(op, proc, src, dst, bytes, count, src_stride,dst_stride);\
else if(count==1) armci_acc_1D(op, scale, proc, src, dst, bytes,lockit);\
else\
      armci_acc_2D(op, scale, proc, src, dst, bytes, count, src_stride,dst_stride,lockit) 

/* macro supports run-time selection of request sending scheme */
#if defined(CLIENT_BUF_BYPASS)
#define CAN_REQUEST_DIRECTLY _armci_bypass
#else
#  if defined(HITACHI)
#    define CAN_REQUEST_DIRECTLY 0
#  else
#    define CAN_REQUEST_DIRECTLY 1
#  endif
#endif

#ifdef BGML
#define PREPROCESS_STRIDED(tmp_count)
#define POSTPROCESS_STRIDED(tmp_count)
#else
#define PREPROCESS_STRIDED(tmp_count) {\
 tmp_count=0;\
 if(stride_levels) \
    for(;stride_levels;stride_levels--)if(count[stride_levels]>1)break;\
 if(stride_levels&&(count[0]==src_stride_arr[0]&&count[0]==dst_stride_arr[0])){\
      tmp_count=seg_count[1];\
      count = seg_count+1;\
      seg_count[1] = seg_count[0] * seg_count[1];\
      stride_levels --;\
      src_stride_arr ++;  dst_stride_arr++ ;\
 }\
}
#define POSTPROCESS_STRIDED(tmp_count) if(tmp_count)seg_count[1]=tmp_count
#endif

#define SERVER_GET 1
#define SERVER_NBGET 2
#define DIRECT_GET 3
#define DIRECT_NBGET 4
#define SERVER_PUT 5
#define SERVER_NBPUT 6
#define DIRECT_PUT 7
#define DIRECT_NBPUT 8


#ifdef ALLOW_PIN
#  define DO_FENCE(__proc,__prot) if(__prot==SERVER_GET);\
        else if(__prot==SERVER_PUT);\
        else if(__prot==DIRECT_GET || __prot==DIRECT_NBGET){\
          if(armci_prot_switch_fence[__proc]==SERVER_PUT)\
            ARMCI_Fence(__proc);\
        }\
        else if(__prot==DIRECT_PUT || __prot==DIRECT_NBPUT){\
          if(armci_prot_switch_fence[__proc]==SERVER_PUT)\
            ARMCI_Fence(__proc);\
        }\
        else;\
        armci_prot_switch_fence[__proc]==__prot
#else

#  define DO_FENCE(__proc,__prot)  
        
#endif


#ifndef REGIONS_REQUIRE_MEMHDL 
#  define ARMCI_MEMHDL_T void
#endif

ARMCI_MEMHDL_T *mhloc=NULL,*mhrem=NULL; 

#ifdef REGIONS_REQUIRE_MEMHDL 
   int armci_region_both_found_hndl(void *loc, void *rem, int size, int node,
                 ARMCI_MEMHDL_T **loc_memhdl,ARMCI_MEMHDL_T **rem_memhdl);
#  define ARMCI_REGION_BOTH_FOUND(_s,_d,_b,_p) \
    armci_region_both_found_hndl((_s),(_d),(_b),(_p),&mhloc,&mhrem)
#else
#  define ARMCI_REGION_BOTH_FOUND(_s,_d,_b,_p) \
    armci_region_both_found((_s),(_d),(_b),(_p))
#endif

#ifdef HAS_RDMA_GET
        
#  ifdef REGIONS_REQUIRE_MEMHDL 
   void armci_client_direct_get(int p, void *src_buf, void *dst_buf, int len,
         void** cptr,int nbtag,ARMCI_MEMHDL_T *lochdl,ARMCI_MEMHDL_T *remhdl);
#  else
   void armci_client_direct_get(int p, void *src_buf, void *dst_buf, int len,
                    void** contextptr,int nbtag,void *mhdl,void *mhdl1);
#  endif
#  define ARMCI_NBREM_GET(_p,_s,_sst,_d,_dst,_cou,_lev,_hdl) \
    armci_client_direct_get((_p),(_s),(_d),(_cou)[0],&((_hdl)->cmpl_info),(_hdl)->tag,(void *)mhloc,(void *)mhrem); \

#  define ARMCI_REM_GET(_p,_s,_sst,_d,_dst,_cou,_lev,_hdl) \
    armci_client_direct_get((_p),(_s),(_d),(_cou)[0],NULL,0,(void *)mhloc,(void *)mhrem); \

#else

#  define ARMCI_REM_GET(_p,_s,_sst,_d,_dst,_cou,_lev,_hdl) \
    armci_rem_get((_p),(_s),(_sst),(_d),(_dst),(_cou),(_lev),(_hdl),(void *)mhloc,(void *)mhrem)
#  define ARMCI_NBREM_GET ARMCI_REM_GET
        
#endif

#ifdef ALLOW_PIN
 extern int* armci_prot_switch_fence;
 extern int armci_prot_switch_preproc;
 extern int armci_prot_switch_preop;
#endif

        
int armci_iwork[MAX_STRIDE_LEVEL];

/*\ 2-dimensional array copy
\*/
static void armci_copy_2D(int op, int proc, void *src_ptr, void *dst_ptr, 
                          int bytes, int count, int src_stride, int dst_stride)
{
#ifdef LAPI2__
#  define COUNT 1
#else
#  define COUNT count
#endif

#ifdef __crayx1
    int shmem = 1;
#else
    int shmem = SAMECLUSNODE(proc);
#endif
    
    if(shmem) {
        
        /* data is in local/shared memory -- can use memcpy */

        if(count==1 && bytes <THRESH1D){
	  
            armci_copy(src_ptr, dst_ptr, bytes); 

        }else {
            
            if(bytes < THRESH){ /* low-latency copy for small data segments */        
#if defined(__crayx1)
	      if( !(bytes%sizeof(float)) ) {
                float *ps=(float*)src_ptr;
                float *pd=(float*)dst_ptr;
                long fsstride = src_stride/sizeof(float);
                long fdstride = dst_stride/sizeof(float);
                int j;
                
                for (j = 0;  j < count;  j++){
                    int i;
#pragma _CRI concurrent
                    for(i=0;i<bytes/sizeof(float);i++) pd[i] = ps[i];
                    ps += fsstride;
                    pd += fdstride;
                }
	      } else
#endif
		{
		  char *ps=(char*)src_ptr;
		  char *pd=(char*)dst_ptr;
		  int j;
		  
		  for (j = 0;  j < count;  j++){
                    int i;
                    for(i=0;i<bytes;i++) pd[i] = ps[i];
                    ps += src_stride;
                    pd += dst_stride;
		  }
		}
            } else if(bytes %ALIGN_SIZE
                      || dst_stride % ALIGN_SIZE
                      || src_stride % ALIGN_SIZE
#ifdef PTR_ALIGN
                      || (unsigned long)src_ptr%ALIGN_SIZE
                      || (unsigned long)dst_ptr%ALIGN_SIZE
#endif
                      ){ 

                /* size/address not alligned */
                ByteCopy2D(bytes, count, src_ptr, src_stride, dst_ptr, dst_stride);
                
            }else { /* size aligned -- should be the most efficient copy */
                
                DCopy2D(bytes/ALIGN_SIZE, count,src_ptr, src_stride/ALIGN_SIZE, 
                        dst_ptr, dst_stride/ALIGN_SIZE);
            }
        }
        
    } else {
        
        /* data not in local/shared memory-access through global address space*/
        
        if(op==PUT){ 
            
            UPDATE_FENCE_STATE(proc, PUT, COUNT);
#ifdef LAPI
            SET_COUNTER(ack_cntr,COUNT);
#endif
            if(count==1){
                armci_put(src_ptr, dst_ptr, bytes, proc);
            }else{
                armci_put2D(proc, bytes, count, src_ptr, src_stride,
                            dst_ptr, dst_stride);
            }
            
        }else{
            
#ifdef LAPI
            SET_COUNTER(get_cntr, COUNT);
#endif
            if(count==1){
                armci_get(src_ptr, dst_ptr, bytes, proc);
            }else{
                armci_get2D(proc, bytes, count, src_ptr, src_stride,
                            dst_ptr, dst_stride);
            }
        }
    }
}


#if (defined(CRAY) && !defined(__crayx1)) || defined(FUJITSU)
#ifdef CRAY
#  define DAXPY  SAXPY
#else
#  define DAXPY  daxpy_
#endif

static int ONE=1;
#define THRESH_ACC 32

static void daxpy_2d_(void* alpha, int *rows, int *cols, void *a, int *ald,
               void* b, int *bld)
{
   int c,r;   
   double *A = (double*)a;
   double *B = (double*)b;
   double Alpha = *(double*)alpha;

   if(*rows < THRESH_ACC)
      for(c=0;c<*cols;c++)
         for(r=0;r<*rows;r++)
           A[c* *ald+ r] += Alpha * B[c* *bld+r];
   else for(c=0;c<*cols;c++)
         DAXPY(rows, alpha, B + c* *bld, &ONE, A + c* *ald, &ONE);
}
#endif


void armci_acc_1D(int op, void *scale, int proc, void *src, void *dst, int bytes, int lockit)
{
int rows;
void (ATR *func)(void*, void*, void*, int*);
      switch (op){
      case ARMCI_ACC_INT:
          rows = bytes/sizeof(int);
          func = I_ACCUMULATE_1D;
          break;
      case ARMCI_ACC_LNG:
          rows = bytes/sizeof(long);
          func = L_ACCUMULATE_1D;
          break;
      case ARMCI_ACC_DBL:
          rows = bytes/sizeof(double);
          func = D_ACCUMULATE_1D;
          break;
      case ARMCI_ACC_DCP:
          rows = bytes/(2*sizeof(double));
          func = Z_ACCUMULATE_1D;
          break;
      case ARMCI_ACC_CPL:
          rows = bytes/(2*sizeof(float));
          func = C_ACCUMULATE_1D;
          break;
      case ARMCI_ACC_FLT:
          rows = bytes/sizeof(float);
          func = F_ACCUMULATE_1D;
          break;
      default: armci_die("ARMCI accumulate: operation not supported",op);
          func = F_ACCUMULATE_1D; /*avoid compiler whining */
      }


      if(lockit){
          ARMCI_LOCKMEM(dst, bytes + (char*)dst, proc);
      }
      func(scale, dst, src, &rows);
      if(lockit)ARMCI_UNLOCKMEM(proc);
}

/*\ 2-dimensional accumulate
\*/
void armci_acc_2D(int op, void* scale, int proc, void *src_ptr, void *dst_ptr,
                  int bytes, int cols, int src_stride, int dst_stride, int lockit)
{
int   rows, lds, ldd, span;
void (ATR *func)(void*, int*, int*, void*, int*, void*, int*);

/*
      if((long)src_ptr%ALIGN)armci_die("src not aligned",(long)src_ptr);
      if((long)dst_ptr%ALIGN)armci_die("src not aligned",(long)dst_ptr);
*/

      switch (op){
      case ARMCI_ACC_INT:
          rows = bytes/sizeof(int);
          ldd  = dst_stride/sizeof(int);
          lds  = src_stride/sizeof(int);
          func = I_ACCUMULATE_2D;
          break;
      case ARMCI_ACC_LNG:
          rows = bytes/sizeof(long);
          ldd  = dst_stride/sizeof(long);
          lds  = src_stride/sizeof(long);
          func = L_ACCUMULATE_2D;
          break;
      case ARMCI_ACC_DBL:
          rows = bytes/sizeof(double);
          ldd  = dst_stride/sizeof(double);
          lds  = src_stride/sizeof(double);
          func = D_ACCUMULATE_2D;
          break;
      case ARMCI_ACC_DCP:
          rows = bytes/(2*sizeof(double));
          ldd  = dst_stride/(2*sizeof(double));
          lds  = src_stride/(2*sizeof(double));
          func = Z_ACCUMULATE_2D;
          break;
      case ARMCI_ACC_CPL:
          rows = bytes/(2*sizeof(float));
          ldd  = dst_stride/(2*sizeof(float));
          lds  = src_stride/(2*sizeof(float));
          func = C_ACCUMULATE_2D;
          break;
      case ARMCI_ACC_FLT:
          rows = bytes/sizeof(float);
          ldd  = dst_stride/sizeof(float);
          lds  = src_stride/sizeof(float);
          func = F_ACCUMULATE_2D;
          break;
      default: armci_die("ARMCI accumulate: operation not supported",op);
          func = F_ACCUMULATE_2D; /*avoid compiler whining */
      }

             
      if(lockit){ 
          span = cols*dst_stride;
          ARMCI_LOCKMEM(dst_ptr, span + (char*)dst_ptr, proc);
      }
      func(scale, &rows, &cols, dst_ptr, &ldd, src_ptr, &lds);
      if(lockit)ARMCI_UNLOCKMEM(proc);

}


/*\ compute range of strided data AND lock it
\*/
static void 
armci_lockmem_patch(void* dst_ptr, int dst_stride_arr[], int count[], int stride_levels, int proc)
{
    long span = count[stride_levels];
    span *= dst_stride_arr[stride_levels-1];

    /* lock region of remote memory */
    ARMCI_LOCKMEM(dst_ptr, span + (char*)dst_ptr, proc);
}


/*\ strided accumulate on top of remote memory copy:
 *  copies remote data to local buffer, accumulates, puts it back 
 *  Note: if we are here then remote patch must fit in the ARMCI buffer
\*/
int armci_acc_copy_strided(int optype, void* scale, int proc,
                                  void* src_ptr, int src_stride_arr[],  
		                  void* dst_ptr, int dst_stride_arr[], 
                                  int count[], int stride_levels)
{
    void *buf_ptr = armci_internal_buffer;
    int  rc, i, *buf_stride_arr = armci_iwork;

    armci_lockmem_patch(dst_ptr,dst_stride_arr, count, stride_levels, proc);

    /* setup stride array for internal buffer */
    buf_stride_arr[0]=count[0];
    for(i=0; i< stride_levels; i++) {
         buf_stride_arr[i+1]= buf_stride_arr[i]*count[i+1];
    }

    /* get remote data to local buffer */
    rc = armci_op_strided(GET, scale, proc, dst_ptr, dst_stride_arr, buf_ptr, 
                          buf_stride_arr, count, stride_levels, 0,NULL);

    if(rc) { ARMCI_UNLOCKMEM(proc); return(rc); }

    /* call local accumulate with lockit=0 (we locked it already) and proc=me */
    rc = armci_op_strided(optype, scale, armci_me, src_ptr, src_stride_arr, 
                          buf_ptr,buf_stride_arr, count, stride_levels,0,NULL);
    if(rc) { ARMCI_UNLOCKMEM(proc); return(rc); }

    /* put data back from the buffer to remote location */
    rc = armci_op_strided(PUT, scale, proc, buf_ptr, buf_stride_arr, dst_ptr, 
                          dst_stride_arr, count, stride_levels,0,NULL);

    FENCE_NODE(proc); /* make sure put completes before unlocking */
    ARMCI_UNLOCKMEM(proc);    /* release memory lock */

    return(rc);
}



/*\ Strided  operation
\*/
int armci_op_strided(int op, void* scale, int proc,void *src_ptr, 
                     int src_stride_arr[], void* dst_ptr, int dst_stride_arr[], 
                     int count[], int stride_levels, int lockit,
                     armci_ihdl_t nb_handle)
{
    char *src = (char*)src_ptr, *dst=(char*)dst_ptr;
    int s2, s3, i,j, unlockit=0;
    int total_of_2D;
    int index[MAX_STRIDE_LEVEL], unit[MAX_STRIDE_LEVEL];
    
#   if defined(ACC_COPY)
      
#      ifdef ACC_SMP
         if(ACC(op) && !(SAMECLUSNODE(proc)) )
#      else
         if ( ACC(op) && proc!=armci_me)
#      endif
             /* copy remote data, accumulate, copy back*/
             return (armci_acc_copy_strided(op,scale, proc, src_ptr, src_stride_arr,
                                       dst_ptr, dst_stride_arr, count, stride_levels));

         else; /* do it directly through shared/local memory */
#   endif

      if(ACC(op) && (stride_levels>2) && lockit){
         /* we need one lock operation only - must be done outside 2d acc */
         armci_lockmem_patch(dst_ptr,dst_stride_arr, count, stride_levels, proc);
         unlockit=1;
         lockit =0;
      }

/*    if(proc!=armci_me) INTR_OFF;*/

#  if defined(LAPI2) /*|| defined(DOELAN4) && !defined(NB_NONCONT)*/
    /*even 1D armci_nbput has to use different origin counters for 1D */
#   if defined(LAPI2)
    if(!ACC(op) && !SAMECLUSNODE(proc) && (nb_handle || 
       (!nb_handle && stride_levels>=1 && count[0]<=LONG_PUT_THRESHOLD))) 
#   else
    /*if(!ACC(op) && !SAMECLUSNODE(proc) && nb_handle && stride_levels<2)*/
    if(!ACC(op) && !SAMECLUSNODE(proc) && stride_levels<2)
#   endif
       armci_network_strided(op,scale,proc,src_ptr,src_stride_arr,dst_ptr,
                         dst_stride_arr,count,stride_levels,nb_handle);
    else
#  endif
    switch (stride_levels) {
      case 0: /* 1D copy */ 

          ARMCI_OP_2D(op, scale, proc, src_ptr, dst_ptr, count[0], 1, 
                      count[0], count[0], lockit); 
          
          break;
          
      case 1: /* 2D op */
          ARMCI_OP_2D(op, scale, proc, src_ptr, dst_ptr, count[0], count[1], 
                      src_stride_arr[0], dst_stride_arr[0], lockit);
          break;

      case 2: /* 3D op */
          for (s2= 0; s2  < count[2]; s2++){ /* 2D copy */
              ARMCI_OP_2D(op, scale, proc, src+s2*src_stride_arr[1], 
                          dst+s2*dst_stride_arr[1], count[0], count[1], 
                          src_stride_arr[0], dst_stride_arr[0], lockit );
          }
          break;
          
      case 3: /* 4D op */
          for(s3=0; s3< count[3]; s3++){
              src = (char*)src_ptr + src_stride_arr[2]*s3;
              dst = (char*)dst_ptr + dst_stride_arr[2]*s3;
              for (s2= 0; s2  < count[2]; s2++){ /* 3D copy */
                  ARMCI_OP_2D(op, scale, proc, src+s2*src_stride_arr[1],
                              dst+s2*dst_stride_arr[1],
                              count[0], count[1],src_stride_arr[0],
                              dst_stride_arr[0],lockit);
              }
          }
          break;
          
      default: /* N-dimensional */ 
      {
	  /* stride_levels is not the same as ndim. it is ndim-1
	   * For example a 10x10x10... array, suppose the datatype is byte
	   * the stride_arr is 10, 10x10, 10x10x10 ....
	   */
          index[2] = 0; unit[2] = 1; total_of_2D = count[2];
          for(j=3; j<=stride_levels; j++) {
              index[j] = 0; unit[j] = unit[j-1] * count[j-1];
              total_of_2D *= count[j];
          }

          for(i=0; i<total_of_2D; i++) {
              src = (char *)src_ptr; dst = (char *)dst_ptr;
              for(j=2; j<=stride_levels; j++) {
                  src += index[j] * src_stride_arr[j-1];
                  dst += index[j] * dst_stride_arr[j-1];
                  
                  if(((i+1) % unit[j]) == 0) index[j]++;
                  if(index[j] >= count[j]) index[j] = 0;
              }
              
              ARMCI_OP_2D(op, scale, proc, src, dst, count[0], count[1], 
                          src_stride_arr[0], dst_stride_arr[0], lockit);
          }
          
      }
    }
    
    /* deal with non-blocking loads and stores */
#if defined(LAPI) || defined(_ELAN_PUTGET_H) || defined(NB_NONCONT)
#   if defined(LAPI)
     if(!nb_handle)
#   endif
    {
       if(!(SAMECLUSNODE(proc))){
          if(op == GET){
            WAIT_FOR_GETS; /* wait for data arrival */
          }else { 
            WAIT_FOR_PUTS; /* data must be copied out*/ 
          }
       }
    }
#endif

/*    if(proc!=armci_me) INTR_ON;*/

    if(unlockit){
#      if defined(ACC_COPY)
          FENCE_NODE(proc); 
#      endif
       ARMCI_UNLOCKMEM(proc);    /* release memory lock */
    }

    return 0;
}


int ARMCI_PutS( void *src_ptr,        /* pointer to 1st segment at source*/ 
		int src_stride_arr[], /* array of strides at source */
		void* dst_ptr,        /* pointer to 1st segment at destination*/
		int dst_stride_arr[], /* array of strides at destination */
		int seg_count[],      /* number of segments at each stride 
                                         levels: count[0]=bytes*/
		int stride_levels,    /* number of stride levels */
                int proc              /* remote process(or) ID */
                )
{
    int rc=0, direct=1;
    int *count=seg_count, tmp_count=0;

    if(src_ptr == NULL || dst_ptr == NULL) return FAIL;
    if(count[0]<0)return FAIL3;
    if(stride_levels <0 || stride_levels > MAX_STRIDE_LEVEL) return FAIL4;
    if(proc<0)return FAIL5;

#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_PUTS,__FILE__,__LINE__);
    if (armci_me != proc)
       vampir_start_comm(armci_me,proc,count[0],ARMCI_PUTS);
#endif
#ifdef ARMCI_PROFILE
    armci_profile_start_strided(seg_count, stride_levels, proc, 
				ARMCI_PROF_PUTS);
#endif

    ORDER(PUT,proc); /* ensure ordering */
    PREPROCESS_STRIDED(tmp_count);

#ifdef BGML
    armci_hdl_t nb_handle;
    ARMCI_INIT_HANDLE(&nb_handle);
    ARMCI_NbPutS(src_ptr, src_stride_arr, dst_ptr, dst_stride_arr, count,
                 stride_levels, proc, &nb_handle);
    ARMCI_Wait(&nb_handle);
#else


#if !defined(QUADRICS) || defined(PACKPUT)
    direct=SAMECLUSNODE(proc);
#endif

    /* use direct protocol for remote access when performance is better */
#   if defined(LAPI) || defined(DOELAN4)
       if(!direct) switch(stride_levels) {
          case 0:  direct =1; break;
          case 1:  if((count[1]<PACKPUT)||count[0]>LONG_PUT_THRESHOLD) direct =1; break;
          default: if(count[0]> LONG_PUT_THRESHOLD )direct=1; break;
       }
#   endif

#ifndef LAPI2
    if(!direct){
#    ifdef ALLOW_PIN /*if we can pin, we do*/
       if(!stride_levels && 
         ARMCI_REGION_BOTH_FOUND(src_ptr,dst_ptr,count[0],armci_clus_id(proc))){
         DO_FENCE(proc,DIRECT_PUT);
         armci_client_direct_send(proc, src_ptr, dst_ptr, count[0],NULL,0,mhloc,mhrem);
         POSTPROCESS_STRIDED(tmp_count);
#        ifdef ARMCI_PROFILE
	 armci_profile_stop_strided(ARMCI_PROF_PUTS);
#        endif
         return 0;
       }
#if   defined(VAPI)
       if(stride_levels==1 && count[0]>VAPI_SGPUT_MIN_COLUMN &&
         ARMCI_REGION_BOTH_FOUND(src_ptr,dst_ptr,count[0],armci_clus_id(proc))){
           DO_FENCE(proc,DIRECT_PUT);
           armci_two_phase_send(proc, src_ptr, src_stride_arr, dst_ptr,
                          dst_stride_arr,count,stride_levels,NULL,NULL,mhloc);
#          ifdef ARMCI_PROFILE
	   armci_profile_stop_strided(ARMCI_PROF_PUTS);
#          endif
           return 0;  
         
       }
#     endif
       
#    endif
       DO_FENCE(proc,SERVER_PUT);
#if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API) 
       if(count[0]> LONG_PUT_THRESHOLD && stride_levels>0){
           rc = armci_rem_strided(PUT, NULL, proc, src_ptr, src_stride_arr,
                     dst_ptr, dst_stride_arr, count, stride_levels,NULL,1,NULL);
       }
       else
#endif
       
       rc = armci_pack_strided(PUT, NULL, proc, src_ptr, src_stride_arr,dst_ptr,
                  dst_stride_arr, count, stride_levels, NULL, -1, -1, -1,NULL);

    }
    else
#endif
    {
      if(stride_levels == 0) {
	  armci_copy_2D(PUT, proc, src_ptr, dst_ptr, count[0], 1, count[0],
			count[0]);
#         if defined(LAPI) || defined(_ELAN_PUTGET_H)
	  if(proc != armci_me) { WAIT_FOR_PUTS; }
#         endif
      }
	else 
	  rc = armci_op_strided( PUT, NULL, proc, src_ptr, src_stride_arr, 
				 dst_ptr, dst_stride_arr,count,stride_levels, 
				 0,NULL);
    }
#endif
    POSTPROCESS_STRIDED(tmp_count);
#ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_PUTS);
#endif
#ifdef GA_USE_VAMPIR
    if (armci_me != proc)
       vampir_end_comm(armci_me,proc,count[0],ARMCI_PUTS);
    vampir_end(ARMCI_PUTS,__FILE__,__LINE__);
#endif
    if(rc) return FAIL6;
    else return 0;

}




/*\ function offers the same functionality as regular PutS and sets remote flag
\*/  
static int *valflagarr=NULL;
#ifdef ALLOW_PIN
static armci_hdl_t nbhdlarr1[1000];
#endif

int ARMCI_PutS_flag_dir( 
                void *src_ptr,        /* pointer to 1st segment at source*/ 
		int src_stride_arr[], /* array of strides at source */
		void* dst_ptr,        /* pointer to 1st segment at destination*/
		int dst_stride_arr[], /* array of strides at destination */
		int seg_count[],          /* number of segments at each stride 
                                         levels: count[0]=bytes*/
		int stride_levels,    /* number of stride levels */
                int *flag,            /* pointer to remote flag */
                int val,              /* value to set flag upon completion of
                                         data transfer */
                int proc              /* remote process(or) ID */
                )
{
    int rc, direct=1;
    int *count=seg_count, tmp_count;

    if(src_ptr == NULL || dst_ptr == NULL) return FAIL;
    if(count[0]<0)return FAIL3;
    if(stride_levels <0 || stride_levels > MAX_STRIDE_LEVEL) return FAIL4;
    if(proc<0)return FAIL5;

#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_PUTS,__FILE__,__LINE__);
    if (armci_me != proc)
       vampir_start_comm(armci_me,proc,count[0],ARMCI_PUTS);
#endif
#ifdef ARMCI_PROFILE
    armci_profile_start_strided(seg_count, stride_levels, proc, 
				ARMCI_PROF_PUTS);
#endif

    ORDER(PUT,proc); /* ensure ordering */
    PREPROCESS_STRIDED(tmp_count);

#ifndef QUADRICS
    direct=SAMECLUSNODE(proc);
#endif

    if(!valflagarr)
       valflagarr = (int *)ARMCI_Malloc_local(armci_nproc*sizeof(int));

    if(!direct){
       ext_header_t h;
       armci_flag_t remf;
       remf.val = val;
       remf.ptr = flag;
       h.exthdr = &remf;
       h.len = sizeof(remf);
#ifdef ALLOW_PIN
       {
	  int i=0;
	  if(stride_levels==1 &&
	     ARMCI_REGION_BOTH_FOUND(src_ptr,dst_ptr,count[0],
				     armci_clus_id(proc))){ 
	     for(i=0;i<count[1];i++){
                char *csp=(char *)src_ptr,*cdp=(char *)dst_ptr;
		ARMCI_INIT_HANDLE(&nbhdlarr1[i%1000]);
		ARMCI_NbPut(src_ptr,dst_ptr,count[0],proc,&nbhdlarr1[i%1000]);
		csp +=src_stride_arr[0];
		cdp +=dst_stride_arr[0];
	     }
	     valflagarr[proc]=val;
	     ARMCI_NbPut(valflagarr+proc,flag,4,proc,&nbhdlarr1[i%1000]);
#            if 1
	     for(i=0;i<count[1];i++){
		ARMCI_Wait(&nbhdlarr1[i%1000]);
	     }
#            endif
#            ifdef ARMCI_PROFILE  
	     armci_profile_stop_strided(ARMCI_PROF_PUTS);   
#            endif 
	     return 0;
	  }
	  if(stride_levels==0 &&
	     ARMCI_REGION_BOTH_FOUND(src_ptr,dst_ptr,count[0],
				     armci_clus_id(proc))){ 
	     ARMCI_INIT_HANDLE(&nbhdlarr1[i%1000]);
	     ARMCI_NbPut(src_ptr,dst_ptr,count[0],proc,&nbhdlarr1[i%1000]);
	     valflagarr[proc]=val;
	     ARMCI_NbPut(valflagarr+proc,flag,4,proc,&nbhdlarr1[i%1000]);
	  }
       }
#endif
        
#if 0
       printf("%d: ptr=%p to %d\n",armci_me,flag,proc); fflush(stdout);
#endif
#if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API) 
       if(count[0]> LONG_PUT_THRESHOLD && stride_levels>0){
           rc = armci_rem_strided(PUT, NULL, proc, src_ptr, src_stride_arr,
                     dst_ptr, dst_stride_arr, count, stride_levels, &h,1,NULL);
       }
       else
#endif

       rc = armci_pack_strided(PUT, NULL, proc, src_ptr, src_stride_arr,dst_ptr,
                       dst_stride_arr, count, stride_levels, &h,-1,-1,-1,NULL);
    }
    else {
       rc = armci_op_strided( PUT, NULL, proc, src_ptr, src_stride_arr, 
                       dst_ptr, dst_stride_arr, count, stride_levels, 0,NULL);
       armci_put(&val,flag,sizeof(int),proc); 
    }

    POSTPROCESS_STRIDED(tmp_count);
#ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_PUTS);
#endif
#ifdef GA_USE_VAMPIR
    if (armci_me != proc)
       vampir_end_comm(armci_me,proc,count[0],ARMCI_PUTS);
    vampir_end(ARMCI_PUTS,__FILE__,__LINE__);
#endif
    if(rc) return FAIL6;
    else return 0;

}
int ARMCI_PutS_flag( 
                void *src_ptr,        /* pointer to 1st segment at source*/ 
		int src_stride_arr[], /* array of strides at source */
		void* dst_ptr,        /* pointer to 1st segment at destination*/
		int dst_stride_arr[], /* array of strides at destination */
		int seg_count[],          /* number of segments at each stride 
                                         levels: count[0]=bytes*/
		int stride_levels,    /* number of stride levels */
                int *flag,            /* pointer to remote flag */
                int val,              /* value to set flag upon completion of
                                         data transfer */
                int proc              /* remote process(or) ID */
                )
{
    int rc, direct=1;
    int *count=seg_count, tmp_count;

    if(src_ptr == NULL || dst_ptr == NULL) return FAIL;
    if(count[0]<0)return FAIL3;
    if(stride_levels <0 || stride_levels > MAX_STRIDE_LEVEL) return FAIL4;
    if(proc<0)return FAIL5;

#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_PUTS,__FILE__,__LINE__);
    if (armci_me != proc)
       vampir_start_comm(armci_me,proc,count[0],ARMCI_PUTS);
#endif
#ifdef ARMCI_PROFILE
    armci_profile_start_strided(seg_count, stride_levels, proc, 
				ARMCI_PROF_PUTS);
#endif

    ORDER(PUT,proc); /* ensure ordering */
    PREPROCESS_STRIDED(tmp_count);

#ifndef QUADRICS
    direct=SAMECLUSNODE(proc);
#endif

    if(!valflagarr)
       valflagarr = (int *)ARMCI_Malloc_local(armci_nproc*sizeof(int));

    if(!direct){
       ext_header_t h;
       armci_flag_t remf;
       remf.val = val;
       remf.ptr = flag;
       h.exthdr = &remf;
       h.len = sizeof(remf);
       
#if 0
       printf("%d: ptr=%p to %d\n",armci_me,flag,proc); fflush(stdout);
#endif
#if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API) 
       if(count[0]> LONG_PUT_THRESHOLD && stride_levels>0){
           rc = armci_rem_strided(PUT, NULL, proc, src_ptr, src_stride_arr,
                     dst_ptr, dst_stride_arr, count, stride_levels, &h,1,NULL);
       }
       else
#endif

       rc = armci_pack_strided(PUT, NULL, proc, src_ptr, src_stride_arr,dst_ptr,
                       dst_stride_arr, count, stride_levels, &h,-1,-1,-1,NULL);
    }
    else {
       rc = armci_op_strided( PUT, NULL, proc, src_ptr, src_stride_arr, 
                       dst_ptr, dst_stride_arr, count, stride_levels, 0,NULL);
       armci_put(&val,flag,sizeof(int),proc); 
    }

    POSTPROCESS_STRIDED(tmp_count);
#ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_PUTS);
#endif
#ifdef GA_USE_VAMPIR
    if (armci_me != proc)
       vampir_end_comm(armci_me,proc,count[0],ARMCI_PUTS);
    vampir_end(ARMCI_PUTS,__FILE__,__LINE__);
#endif
    if(rc) return FAIL6;
    else return 0;

}



int ARMCI_GetS( void *src_ptr,  	/* pointer to 1st segment at source*/ 
		int src_stride_arr[],   /* array of strides at source */
		void* dst_ptr,          /* 1st segment at destination*/
		int dst_stride_arr[],   /* array of strides at destination */
		int seg_count[],       /* number of segments at each stride 
					   levels: count[0]=bytes*/
		int stride_levels,      /* number of stride levels */
                int proc                /* remote process(or) ID */
                )
{
    int rc,direct=1;
    int *count=seg_count, tmp_count=0;

    if(src_ptr == NULL || dst_ptr == NULL) return FAIL;
    if(seg_count[0]<0)return FAIL3;
    if(stride_levels <0 || stride_levels > MAX_STRIDE_LEVEL) return FAIL4;
    if(proc<0)return FAIL5;
    
#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_GETS,__FILE__,__LINE__);
    if (armci_me != proc)
       vampir_start_comm(proc,armci_me,count[0],ARMCI_GETS);
#endif
#ifdef ARMCI_PROFILE
    armci_profile_start_strided(seg_count, stride_levels, proc, 
				ARMCI_PROF_GETS);
#endif

    ORDER(GET,proc); /* ensure ordering */
    PREPROCESS_STRIDED(tmp_count);
#ifdef BGML
   armci_hdl_t nb_handle;
   ARMCI_INIT_HANDLE(&nb_handle);
   ARMCI_NbGetS(src_ptr, src_stride_arr, dst_ptr, dst_stride_arr, count,
                                stride_levels, proc, &nb_handle);
   ARMCI_Wait(&nb_handle);
#else

#ifndef QUADRICS
    direct=SAMECLUSNODE(proc);
#endif

    /* use direct protocol for remote access when performance is better */
#   if (defined(LAPI) && !defined(LAPI2))
      if(!direct)
        if( stride_levels==0 || count[0]> LONG_GET_THRESHOLD)direct=1;
        else{
          int i, chunks=1;
          for(i=1, direct=1; i<= stride_levels; i++)
              if((chunks *= count[i]) >MAX_CHUNKS_SHORT_GET){ direct=0; break;}
        }
#   endif

#ifndef LAPI2
    if(!direct){
#     ifdef ALLOW_PIN
       if(!stride_levels && 
         ARMCI_REGION_BOTH_FOUND(dst_ptr,src_ptr,count[0],armci_clus_id(proc))){
         DO_FENCE(proc,DIRECT_GET);
         ARMCI_REM_GET(proc, src_ptr,NULL,dst_ptr,NULL,count, 0, NULL);
         POSTPROCESS_STRIDED(tmp_count);
#        ifdef ARMCI_PROFILE
         armci_profile_stop_strided(ARMCI_PROF_GETS);
#        endif
         return 0;
       }
#if   defined(VAPI)
       if(stride_levels==1 && count[0]>VAPI_SGGET_MIN_COLUMN &&
         ARMCI_REGION_BOTH_FOUND(dst_ptr,src_ptr,count[0],armci_clus_id(proc))){
          DO_FENCE(proc,DIRECT_GET);
          armci_two_phase_get(proc, src_ptr, src_stride_arr, dst_ptr,
                          dst_stride_arr,count,stride_levels,NULL,NULL,mhloc);  
#         ifdef ARMCI_PROFILE
	  armci_profile_stop_strided(ARMCI_PROF_GETS);
#         endif
          return 0;
       }
#     endif
#     endif
       DO_FENCE(proc,SERVER_GET);
#if defined(DATA_SERVER) && (defined(SOCKETS) || defined(CLIENT_BUF_BYPASS))
       /* for larger strided or 1D reqests buffering can be avoided to send data
        * we can try to bypass the packetization step and send request directly
        */
        if(CAN_REQUEST_DIRECTLY && ((count[0]> LONG_GET_THRESHOLD) ||
            (stride_levels && count[0]>LONG_GET_THRESHOLD_STRIDED) ) ) {

          int nobuf =1; /* tells the sending routine not to buffer */
          rc = armci_rem_strided(GET, NULL, proc,src_ptr,src_stride_arr,dst_ptr,
                                dst_stride_arr, count, stride_levels,
                                (ext_header_t*)0,nobuf,NULL);
          if(rc) goto DefaultPath; /* attempt to avoid buffering failed */ 

       }else
               DefaultPath: /* standard buffered path */
#endif
               
          rc = armci_pack_strided(GET, NULL, proc, src_ptr, src_stride_arr,
                                 dst_ptr,dst_stride_arr,count,stride_levels,
                                 NULL,-1,-1,-1,NULL);
               
    }else
#else
       /* avoid LAPI_GetV  - note count[0]<0 means disabled*/
       if(stride_levels==1 && count[0]<0 && !direct) 
         ARMCI_REM_GET(proc,src_ptr,src_stride_arr,dst_ptr,
                                dst_stride_arr, count, stride_levels, NULL);
       else
#endif
       rc = armci_op_strided(GET, NULL, proc, src_ptr, src_stride_arr, dst_ptr,
                             dst_stride_arr,count, stride_levels,0,NULL);
#endif

    POSTPROCESS_STRIDED(tmp_count);
#ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_GETS);
#endif
#ifdef GA_USE_VAMPIR
    if (armci_me != proc)
       vampir_end_comm(proc,armci_me,count[0],ARMCI_GETS);
    vampir_end(ARMCI_GETS,__FILE__,__LINE__);
#endif
    if(rc) return FAIL6;
    else return 0;
}




int ARMCI_AccS( int  optype,            /* operation */
                void *scale,            /* scale factor x += scale*y */
                void *src_ptr,          /* pointer to 1st segment at source*/ 
		int src_stride_arr[],   /* array of strides at source */
		void* dst_ptr,          /* 1st segment at destination*/
		int dst_stride_arr[],   /* array of strides at destination */
		int seg_count[],        /* number of segments at each stride 
                                           levels: count[0]=bytes*/
		int stride_levels,      /* number of stride levels */
                int proc                /* remote process(or) ID */
                )
{
    int rc, direct=1;
    int *count=seg_count, tmp_count=0;

    if(src_ptr == NULL || dst_ptr == NULL) return FAIL;
    if(src_stride_arr == NULL || dst_stride_arr ==NULL) return FAIL2;
    if(count[0]<0)return FAIL3;
    if(stride_levels <0 || stride_levels > MAX_STRIDE_LEVEL) return FAIL4;
    if(proc<0)return FAIL5;

#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_ACCS,__FILE__,__LINE__);
    if (armci_me != proc)
        vampir_start_comm(armci_me,proc,count[0],ARMCI_ACCS);
#endif
#ifdef ARMCI_PROFILE
    armci_profile_start_strided(seg_count, stride_levels, proc, 
				ARMCI_PROF_ACCS);
#endif

    ORDER(optype,proc); /* ensure ordering */
    PREPROCESS_STRIDED(tmp_count);
#ifdef BGML
    armci_hdl_t nb_handle;
    ARMCI_INIT_HANDLE(&nb_handle);
    ARMCI_NbAccS(optype, scale, src_ptr, src_stride_arr, dst_ptr,
                 dst_stride_arr, count, stride_levels, proc, &nb_handle);
    ARMCI_Wait(&nb_handle);
#else

    direct=SAMECLUSNODE(proc);

#   if defined(ACC_COPY) && !defined(ACC_SMP)
       if(armci_me != proc) direct=0;
#   endif
 
    if(direct)
      rc = armci_op_strided(optype,scale, proc, src_ptr, src_stride_arr,dst_ptr,
                           dst_stride_arr, count, stride_levels,1,NULL);
    else{
      DO_FENCE(proc,SERVER_PUT);
      rc = armci_pack_strided(optype,scale,proc,src_ptr, src_stride_arr,dst_ptr,
                      dst_stride_arr,count,stride_levels,NULL,-1,-1,-1,NULL);
    }
#endif /*bgml*/
    POSTPROCESS_STRIDED(tmp_count);
#ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_ACCS);
#endif
#ifdef GA_USE_VAMPIR
    if (armci_me != proc)
       vampir_end_comm(armci_me,proc,count[0],ARMCI_ACCS);
    vampir_end(ARMCI_ACCS,__FILE__,__LINE__);
#endif
    if(rc) return FAIL6;
    else return 0;
}



int ARMCI_Put(void *src, void* dst, int bytes, int proc)
{
    int rc=0;
#ifdef ARMCI_PROFILE
    armci_profile_start_strided(&bytes, 0, proc, ARMCI_PROF_PUT);
#endif

#ifdef __crayx1
    memcpy(dst,src,bytes);
#else
#ifdef ALLOW_PIN
    if(ARMCI_REGION_BOTH_FOUND(src,dst,bytes,armci_clus_id(proc))){
       DO_FENCE(proc,DIRECT_PUT);
       armci_client_direct_send(proc, src, dst, bytes,NULL,0,mhloc,mhrem);
    }else
#endif
#ifdef BGML
   unsigned count=1;
   BGML_Callback_t cb_wait={wait_callback, &count};
   BG1S_t request;
   BGML_CriticalSection_enter();
   BG1S_Memput(&request, proc, src, 0, dst, bytes, &cb_wait, 1);
   //BGML_Wait(&count);
   while (count) BGML_Messager_advance();
   BGML_CriticalSection_exit();
#else

       rc = ARMCI_PutS(src, NULL, dst, NULL, &bytes, 0, proc);
#endif
#endif
    
#ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_PUT);
#endif
    return rc;
}

extern int ARMCI_Put_flag(void *src, void* dst,int bytes,int *f,int v,int proc)
{
    return  ARMCI_PutS_flag(src, NULL, dst, NULL, &bytes, 0, f, v, proc);
}

int ARMCI_Get(void *src, void* dst, int bytes, int proc)
{
    int rc=0;
#ifdef ARMCI_PROFILE
    armci_profile_start_strided(&bytes, 0, proc, ARMCI_PROF_GET);
#endif

#ifdef __crayx1
    memcpy(dst,src,bytes);   
#else
# ifdef ALLOW_PIN
    if(ARMCI_REGION_BOTH_FOUND(dst,src,bytes,armci_clus_id(proc))){
       DO_FENCE(proc,DIRECT_GET);
       ARMCI_REM_GET(proc, src,NULL,dst,NULL,&bytes, 0, NULL);
    }  else
# endif
#ifdef BGML
   BG1S_t request;
   unsigned count=1;
   BGML_Callback_t cb_wait={wait_callback, &count};
   BG1S_Memget(&request, proc, dst, 0, src, bytes, &cb_wait, 1);
   BGML_Wait(&count);
#else
       rc = ARMCI_GetS(src, NULL, dst, NULL, &bytes, 0, proc);
#endif
#endif
    
#ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_GET);
#endif
    return rc;
}

#define PACK1D 1

#if PACK1D 
#  define armci_read_strided1  armci_read_strided
#  define armci_write_strided1 armci_write_strided
#else
#  define armci_read_strided2  armci_read_strided
#  define armci_write_strided2 armci_write_strided
#endif

void armci_write_strided1(void *ptr, int stride_levels, int stride_arr[],
                   int count[], char *buf)
{
    int i, j;
    long idx;    /* index offset of current block position to ptr */
    int n1dim;  /* number of 1 dim block */
    int bvalue[MAX_STRIDE_LEVEL], bunit[MAX_STRIDE_LEVEL];
    int bytes = count[0];

    /* number of n-element of the first dimension */
    n1dim = 1;
    for(i=1; i<=stride_levels; i++)
        n1dim *= count[i];

    /* calculate the destination indices */
    bvalue[0] = 0; bvalue[1] = 0; bunit[0] = 1; bunit[1] = 1;
    for(i=2; i<=stride_levels; i++) {
        bvalue[i] = 0;
        bunit[i] = bunit[i-1] * count[i-1];
    }

    for(i=0; i<n1dim; i++) {
        idx = 0;
        for(j=1; j<=stride_levels; j++) {
            idx += bvalue[j] * stride_arr[j-1];
            if((i+1) % bunit[j] == 0) bvalue[j]++;
            if(bvalue[j] > (count[j]-1)) bvalue[j] = 0;
        }

        armci_copy( ((char*)ptr)+idx, buf, bytes);
        buf += count[0];
    }
}


void armci_write_strided2(void *ptr, int stride_levels, int stride_arr[],
                          int count[], char *buf)
{                  
    int i, j;
    int total;   /* number of 2 dim block */
    int index[MAX_STRIDE_LEVEL], unit[MAX_STRIDE_LEVEL];
    
    if(stride_levels == 0){
       armci_copy( ptr, buf, count[0]);
    }else if (count[0]%ALIGN_SIZE || (unsigned long)ptr%ALIGN_SIZE ) 
              armci_write_strided1(ptr,stride_levels, stride_arr,count,buf);
          else {
             int rows, ld, idx, ldd;
             char *src;
             rows = count[0]/8;
             ld   = stride_arr[0]/8;
             switch(stride_levels){
             case 1: 
                     DCOPY21(&rows, count+1, ptr, &ld, buf, &idx);
                     break;
             case 2: 
#if 0
                     for(i=0; i< count[2]; i++){ 
                         DCOPY21(&rows, count+1, ptr, &ld, buf, &idx);
                         ptr = ((char*)ptr)+stride_arr[1];
                         buf = (char*) ((double*)buf + idx);
                     }
#endif
                     ldd = stride_arr[1]/stride_arr[0];
                     DCOPY31(&rows, count+1, count+2, ptr, &ld, &ldd, buf,&idx);

                     break;
             default: 
                     index[2] = 0; unit[2] = 1; total = count[2];
                     for(j=3; j<=stride_levels; j++) {
                         index[j] = 0; unit[j] = unit[j-1] * count[j-1];
                         total *= count[j];
                     }
                     for(i=0; i<total; i++) {
                         src = (char *)ptr; 
                         for(j=2; j<=stride_levels; j++) {
                             src += index[j] * stride_arr[j-1];
                             if(((i+1) % unit[j]) == 0) index[j]++;
                             if(index[j] >= count[j]) index[j] = 0;
                         }
                         DCOPY21(&rows, count+1,src, &ld, buf, &idx); 
                         buf = (char*) ((double*)buf + idx);
                     }
            } /*switch */
         } /*else */
}


void armci_read_strided1(void *ptr, int stride_levels, int stride_arr[],
                        int count[], char *buf)
{
    int i, j;
    long idx;    /* index offset of current block position to ptr */
    int n1dim;  /* number of 1 dim block */
    int bvalue[MAX_STRIDE_LEVEL], bunit[MAX_STRIDE_LEVEL];
    int bytes = count[0];

    /* number of n-element of the first dimension */
    n1dim = 1;
    for(i=1; i<=stride_levels; i++)
        n1dim *= count[i];

    /* calculate the destination indices */
    bvalue[0] = 0; bvalue[1] = 0; bunit[0] = 1; bunit[1] = 1;
    for(i=2; i<=stride_levels; i++) {
        bvalue[i] = 0;
        bunit[i] = bunit[i-1] * count[i-1];
    }

    for(i=0; i<n1dim; i++) {
        idx = 0;
        for(j=1; j<=stride_levels; j++) {
            idx += bvalue[j] * stride_arr[j-1];
            if((i+1) % bunit[j] == 0) bvalue[j]++;
            if(bvalue[j] > (count[j]-1)) bvalue[j] = 0;
        }

        armci_copy(buf, ((char*)ptr)+idx,bytes);
        buf += count[0];
    }
}


void armci_read_strided2(void *ptr, int stride_levels, int stride_arr[],
                         int count[], char *buf)
{                  
    int i, j;
    int total;   /* number of 2 dim block */
    int index[MAX_STRIDE_LEVEL], unit[MAX_STRIDE_LEVEL];
   
    if(stride_levels == 0){
       armci_copy( buf, ptr, count[0]);
    }else if (count[0]%ALIGN_SIZE || (unsigned long)ptr%ALIGN_SIZE) 
              armci_read_strided1(ptr,stride_levels, stride_arr,count,buf);
          else {
             int rows, ld, idx, ldd;
             char *src;
             rows = count[0]/8;
             ld   = stride_arr[0]/8;
             switch(stride_levels){
             case 1: 
                     DCOPY12(&rows, count+1, ptr, &ld, buf, &idx);
                     break;
             case 2:
#if 0
                     for(i=0; i< count[2]; i++){
                         DCOPY12(&rows, count+1, ptr, &ld, buf, &idx);
                         ptr = ((char*)ptr)+stride_arr[1];
                         buf = (char*) ((double*)buf + idx);
                     }
#endif
                     ldd = stride_arr[1]/stride_arr[0];   
                     DCOPY13(&rows, count+1, count+2, ptr, &ld, &ldd, buf,&idx);
                     break;
             default:
                     index[2] = 0; unit[2] = 1; total = count[2];
                     for(j=3; j<=stride_levels; j++) {
                         index[j] = 0; unit[j] = unit[j-1] * count[j-1];
                         total *= count[j];
                     }
                     for(i=0; i<total; i++) {
                         src = (char *)ptr; 
                         for(j=2; j<=stride_levels; j++) {
                             src += index[j] * stride_arr[j-1];
                             if(((i+1) % unit[j]) == 0) index[j]++;
                             if(index[j] >= count[j]) index[j] = 0;
                         }
                         DCOPY12(&rows, count+1,src, &ld, buf, &idx);
                         buf = (char*) ((double*)buf + idx);
                     }
            } /*switch */
         } /*else */
}

/*\Non-Blocking API
\*/
int ARMCI_NbPutS( void *src_ptr,        /* pointer to 1st segment at source*/ 
		int src_stride_arr[], /* array of strides at source */
		void* dst_ptr,        /* pointer to 1st segment at destination*/
		int dst_stride_arr[], /* array of strides at destination */
		int seg_count[],      /* number of segments at each stride 
                                         levels: count[0]=bytes*/
		int stride_levels,    /* number of stride levels */
                int proc,             /* remote process(or) ID */
                armci_hdl_t* usr_hdl  /* armci non-blocking call handle*/
                )
{
    armci_ihdl_t nb_handle = (armci_ihdl_t)usr_hdl;
    int *count=seg_count, tmp_count=0;
    int rc=0, direct=1;

    if(src_ptr == NULL || dst_ptr == NULL) return FAIL;
    if(count[0]<0)return FAIL3;
    if(stride_levels <0 || stride_levels > MAX_STRIDE_LEVEL) return FAIL4;
    if(proc<0)return FAIL5;

#ifdef GA_USE_VAMPIR
    vampir_begin(ARMCI_PUTS,__FILE__,__LINE__);
    if (armci_me != proc)
       vampir_start_comm(armci_me,proc,count[0],ARMCI_PUTS);
#endif

    PREPROCESS_STRIDED(tmp_count);

#ifdef ARMCI_PROFILE
    armci_profile_start_strided(seg_count, stride_levels, proc,
				ARMCI_PROF_NBPUTS);
#endif


#if !defined(QUADRICS) || defined(PACKPUT)
    direct=SAMECLUSNODE(proc);
#endif

    /* aggregate put */
    if(nb_handle && nb_handle->agg_flag == SET) {
      if(!direct){ 
	rc= armci_agg_save_strided_descriptor(src_ptr, src_stride_arr, 
						 dst_ptr, dst_stride_arr, 
						 count, stride_levels, proc, 
						 PUT, nb_handle);
        POSTPROCESS_STRIDED(tmp_count);
#       ifdef ARMCI_PROFILE
	  armci_profile_stop_strided(ARMCI_PROF_NBPUTS);
#       endif
        return(rc);
      }
    } else {
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
#ifdef BGML
    nb_handle->count = 1;
    BGML_Callback_t cb_wait={wait_callback, &nb_handle->count};
    BG1S_MemputS (&nb_handle->cmpl_info, proc,
                  src_ptr, src_stride_arr,
                  dst_ptr, dst_stride_arr,
                  seg_count, stride_levels,
                  0, &cb_wait, 1);
#endif

#if defined(DOELAN4)
    if(!direct) switch(stride_levels) {
       case 0:  direct =1; break;
       case 1:  if((count[1]<PACKPUT)||count[0]>LONG_PUT_THRESHOLD) direct =1; break;
       case 2:  direct =1; break;
       default: break;
    }
#endif

#ifndef LAPI2
    if(!direct){
#     ifdef ALLOW_PIN
#if defined(VAPI)
      extern int armci_max_num_sg_ent;
#endif
       if(!stride_levels && 
         ARMCI_REGION_BOTH_FOUND(src_ptr,dst_ptr,count[0],armci_clus_id(proc))){
         DO_FENCE(proc,DIRECT_NBPUT);
         armci_client_direct_send(proc, src_ptr, dst_ptr, count[0],
                                  (void **)(&nb_handle->cmpl_info),
                                  nb_handle->tag,mhloc,mhrem);
         POSTPROCESS_STRIDED(tmp_count);
#        ifdef ARMCI_PROFILE
	 armci_profile_stop_strided(ARMCI_PROF_NBPUTS);
#        endif 
         return 0;
       }
#if   defined(VAPI)
       if(0&&stride_levels==1 && count[0]>VAPI_SGPUT_MIN_COLUMN &&
         /*(count[1] < armci_max_num_sg_ent || count[0] > VAPI_SGPUT_MIN_COLUMN)&&*/
         ARMCI_REGION_BOTH_FOUND(src_ptr,dst_ptr,count[0],armci_clus_id(proc))){
         DO_FENCE(proc,DIRECT_NBPUT);
         armci_two_phase_send(proc, src_ptr, src_stride_arr, dst_ptr,
                       dst_stride_arr,count,stride_levels,NULL,nb_handle,mhloc);
#        ifdef ARMCI_PROFILE
          armci_profile_stop_strided(ARMCI_PROF_NBPUTS);
#        endif 
         return 0;  
       }
#     endif
#     endif
      DO_FENCE(proc,SERVER_NBPUT);
#  if defined(DATA_SERVER) && defined(SOCKETS) && defined(USE_SOCKET_VECTOR_API)
       if(count[0]> LONG_PUT_THRESHOLD && stride_levels>0){
           rc = armci_rem_strided(PUT, NULL, proc, src_ptr, src_stride_arr,
                     dst_ptr, dst_stride_arr, count, stride_levels,NULL,1,nb_handle);
       }
       else
#  endif

       nb_handle->tag =0; /* packed request is completed locally */ 
       CLEAR_HNDL_FIELD(nb_handle->cmpl_info);
       rc = armci_pack_strided(PUT, NULL, proc, src_ptr, src_stride_arr,dst_ptr,
                  dst_stride_arr, count, stride_levels,NULL,-1,-1,-1,NULL);
    }
    else
#endif 
      {
	rc = armci_op_strided( PUT, NULL, proc, src_ptr, src_stride_arr,
		       dst_ptr,dst_stride_arr,count,stride_levels, 0,nb_handle);
      }
    
    POSTPROCESS_STRIDED(tmp_count);
#ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_NBPUTS);
#endif 
#ifdef GA_USE_VAMPIR
    if (armci_me != proc)
       vampir_end_comm(armci_me,proc,count[0],ARMCI_PUTS);
    vampir_end(ARMCI_PUTS,__FILE__,__LINE__);
#endif
    if(rc) return FAIL6;
    else return 0;
    
}

int ARMCI_NbGetS( void *src_ptr,  	/* pointer to 1st segment at source*/ 
		int src_stride_arr[],   /* array of strides at source */
		void* dst_ptr,          /* 1st segment at destination*/
		int dst_stride_arr[],   /* array of strides at destination */
		int seg_count[],        /* number of segments at each stride 
                                           levels: byte_count[0]=bytes*/
		int stride_levels,      /* number of stride levels */
                int proc,               /* remote process(or) ID */
                armci_hdl_t* usr_hdl  /* armci non-blocking call handle*/
                )
{
    armci_ihdl_t nb_handle = (armci_ihdl_t)usr_hdl;
    int rc=0,direct=1;
    int *count=seg_count, tmp_count=0;

    if(src_ptr == NULL || dst_ptr == NULL) return FAIL;
    if(seg_count[0]<0)return FAIL3;
    if(stride_levels <0 || stride_levels > MAX_STRIDE_LEVEL) return FAIL4;
    if(proc<0)return FAIL5;

#ifdef ARMCI_PROFILE
    armci_profile_start_strided(seg_count, stride_levels, proc,
				ARMCI_PROF_NBGETS);
#endif
#ifdef BGML
   armci_ihdl_t nbh;
   set_nbhandle(&nbh, usr_hdl, PUT, proc);
   nbh->count=1;
   BGML_Callback_t cb_wait={wait_callback, &nbh->count};

   BG1S_MemgetS (&nbh->cmpl_info, proc,
                 src_ptr, src_stride_arr,
                 dst_ptr, dst_stride_arr,
                 seg_count, stride_levels,
                 0, &cb_wait, 1);
#else

#if !defined(QUADRICS)
    direct=SAMECLUSNODE(proc);
#endif
    PREPROCESS_STRIDED(tmp_count);

    /* aggregate get */
    if(nb_handle && nb_handle->agg_flag == SET) {
      if(!direct){ 
	rc= armci_agg_save_strided_descriptor(src_ptr, src_stride_arr,
					 dst_ptr, dst_stride_arr, 
					 count, stride_levels, proc, 
					 GET, nb_handle);
        POSTPROCESS_STRIDED(tmp_count);
#       ifdef ARMCI_PROFILE
	  armci_profile_stop_strided(ARMCI_PROF_NBGETS);
#       endif	
        return(rc);
      }
    } 
    else {
      /* ORDER(GET,proc); ensure ordering */
      /*set tag and op in the nb handle*/
      if(nb_handle){
	nb_handle->tag = GET_NEXT_NBTAG();
	nb_handle->op  = GET;
	nb_handle->proc= proc;
	nb_handle->bufid=NB_NONE;
      }
      else
        nb_handle = armci_set_implicit_handle(GET, proc);
    }
#ifdef DOELAN4
    if(stride_levels==0){
      ARMCI_NbGet(src_ptr,dst_ptr,count[0],proc,usr_hdl);
      POSTPROCESS_STRIDED(tmp_count);
#     ifdef ARMCI_PROFILE
	 armci_profile_stop_strided(ARMCI_PROF_NBGETS);
#     endif
      return 0;

    }
#endif
    
#ifndef LAPI2
    if(!direct){
#     ifdef ALLOW_PIN
#if defined(VAPI)
      extern int armci_max_num_sg_ent;
#endif
       if(!stride_levels && 
         ARMCI_REGION_BOTH_FOUND(dst_ptr,src_ptr,count[0],armci_clus_id(proc))){
         DO_FENCE(proc,DIRECT_NBGET);
         ARMCI_NBREM_GET(proc, src_ptr,NULL,dst_ptr,NULL,count, 0, nb_handle);
         POSTPROCESS_STRIDED(tmp_count);
#        ifdef ARMCI_PROFILE
	 armci_profile_stop_strided(ARMCI_PROF_NBGETS);
#        endif
         return 0;
       }
#if   defined(VAPI)
       if(0 && stride_levels==1 && 
         (count[1] < armci_max_num_sg_ent || count[0] > VAPI_SGGET_MIN_COLUMN) && 
         ARMCI_REGION_BOTH_FOUND(dst_ptr,src_ptr,count[0],armci_clus_id(proc))){
         DO_FENCE(proc,DIRECT_NBGET);
          armci_two_phase_get(proc, src_ptr, src_stride_arr, dst_ptr,
                       dst_stride_arr,count,stride_levels,NULL,nb_handle,mhloc);  
#        ifdef ARMCI_PROFILE
	 armci_profile_stop_strided(ARMCI_PROF_NBGETS);
#        endif
         return 0;
       }
#     endif
#     endif
       DO_FENCE(proc,SERVER_NBGET);
#if defined(DATA_SERVER) && (defined(SOCKETS) || defined(CLIENT_BUF_BYPASS))
       /* for larger strided or 1D reqests buffering can be avoided to send data
        * we can try to bypass the packetization step and send request directly
        */
        if(CAN_REQUEST_DIRECTLY && ((count[0]> LONG_GET_THRESHOLD) ||
            (stride_levels && count[0]>LONG_GET_THRESHOLD_STRIDED) ) ) {

          int nobuf =1; /* tells the sending routine not to buffer */
          rc = armci_rem_strided(GET, NULL, proc,src_ptr,src_stride_arr,dst_ptr,
                                dst_stride_arr, count, stride_levels,
                                (ext_header_t*)0,nobuf,nb_handle);
          if(rc) goto DefaultPath; /* attempt to avoid buffering failed */ 

       }else
               DefaultPath: /* standard buffered path */
#endif
          rc = armci_pack_strided(GET, NULL, proc, src_ptr, src_stride_arr,
                                 dst_ptr,dst_stride_arr,count,stride_levels,
                                 NULL,-1,-1,-1,nb_handle);
    }else
#else
       /* avoid LAPI_GetV */
       if(stride_levels==1 && count[0]>320 && !direct) 
               ARMCI_REM_GET(proc,src_ptr,src_stride_arr,dst_ptr,
                               dst_stride_arr, count, stride_levels, nb_handle);
       else
#endif
       rc = armci_op_strided(GET, NULL, proc, src_ptr, src_stride_arr, dst_ptr,
                             dst_stride_arr,count, stride_levels,0,nb_handle);

    POSTPROCESS_STRIDED(tmp_count);
#endif /*bgml*/

#ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_NBGETS);
#endif
    if(rc) return FAIL6;
    else return 0;
}


int ARMCI_NbAccS( int  optype,            /* operation */
                void *scale,            /* scale factor x += scale*y */
                void *src_ptr,          /* pointer to 1st segment at source*/ 
		int src_stride_arr[],   /* array of strides at source */
		void* dst_ptr,          /* 1st segment at destination*/
		int dst_stride_arr[],   /* array of strides at destination */
		int seg_count[],        /* number of segments at each stride 
                                           levels: count[0]=bytes*/
		int stride_levels,      /* number of stride levels */
                int proc,               /* remote process(or) ID */
                armci_hdl_t* usr_hdl  /* armci non-blocking call handle*/
                )
{
    armci_ihdl_t nb_handle = (armci_ihdl_t)usr_hdl;
    int *count=seg_count, tmp_count=0;
    int rc, direct=1;

    if(src_ptr == NULL || dst_ptr == NULL) return FAIL;
    if(src_stride_arr == NULL || dst_stride_arr ==NULL) return FAIL2;
    if(count[0]<0)return FAIL3;
    if(stride_levels <0 || stride_levels > MAX_STRIDE_LEVEL) return FAIL4;
    if(proc<0)return FAIL5;

#ifdef ARMCI_PROFILE
    armci_profile_start_strided(seg_count, stride_levels, proc,
				ARMCI_PROF_NBACCS);
#endif
#ifdef BGML
   armci_ihdl_t nbh;
   set_nbhandle(&nbh, usr_hdl, PUT, proc);
   nbh->count=1;
   BGML_Callback_t cb_wait={wait_callback, &nbh->count};

   BGML_Op oper1=BGML_PROD;
   BGML_Op oper2=BGML_SUM;
   BGML_Dt dt;
   switch(optype)
     {
       case ARMCI_ACC_INT:
       case ARMCI_ACC_LNG:
         dt=BGML_SIGNED_INT;
         break;
#if 0
       case ARMCI_ACC_LNG:
         dt=BGML_SIGNED_LONG;
         break;
#endif
       case ARMCI_ACC_DBL:
         dt=BGML_DOUBLE;
         break;
       case ARMCI_ACC_CPL:
         dt=BGML_SINGLE_COMPLEX;
         break;
       case ARMCI_ACC_DCP:
         dt=BGML_DOUBLE_COMPLEX;
         break;
       case ARMCI_ACC_FLT:
         dt=BGML_FLOAT;
         break;
       default:
         assert(0);
     }

  BG1S_AccumulateS (&nbh->cmpl_info, proc,
                    src_ptr, src_stride_arr,
                    dst_ptr, dst_stride_arr,
                    seg_count, stride_levels,
                    scale, 0,
                    dt, oper1, oper2,
                    &cb_wait, 1);
#else

    UPDATE_FENCE_INFO(proc);
    PREPROCESS_STRIDED(tmp_count);
    direct=SAMECLUSNODE(proc);

#   if defined(ACC_COPY) && !defined(ACC_SMP)
       if(armci_me != proc) direct=0;
#   endif
 
    /*set tag and op in the nb handle*/
    if(nb_handle){
      nb_handle->tag = GET_NEXT_NBTAG();
      nb_handle->op  = optype;
      nb_handle->proc= proc;
      nb_handle->bufid=NB_NONE;
    }
    else
      nb_handle = armci_set_implicit_handle(optype, proc);


    ORDER(PUT,proc); /* ensure ordering */
    if(direct){
      rc = armci_op_strided(optype,scale, proc, src_ptr, src_stride_arr,dst_ptr,
			    dst_stride_arr, count, stride_levels,1,NULL);
    }
    else{
      DO_FENCE(proc,SERVER_NBPUT);
      rc = armci_pack_strided(optype,scale,proc,src_ptr, src_stride_arr,dst_ptr,
                    dst_stride_arr,count,stride_levels,NULL,-1,-1,-1,nb_handle);
    }

    POSTPROCESS_STRIDED(tmp_count);
#endif /*BGML*/

#   ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_NBACCS);
#   endif
    if(rc) return FAIL6;
    else return 0;
}


#if !defined(ACC_COPY)&&!defined(CRAY_YMP)&&!defined(CYGNUS)&&!defined(CYGWIN) &&!defined(BGML)
#   define REMOTE_OP
#endif

#define INIT_NB_HANDLE(nb,o,p) if(nb){\
             (nb)->tag = 0;\
             (nb)->op  = (o); (nb)->proc= (p);\
             (nb)->bufid=NB_NONE;}\
             else { (nb)=armci_set_implicit_handle(o, p); (nb)->tag=0; }

void set_nbhandle(armci_ihdl_t *nbh, armci_hdl_t *nb_handle, int op,
                         int proc)
{
   if(nb_handle)
   {
      *nbh=(armci_ihdl_t)nb_handle;
   }
   else
   {
      *nbh=armci_set_implicit_handle(op, proc);
   }
}


int ARMCI_NbPut(void *src, void* dst, int bytes, int proc,armci_hdl_t* uhandle)
{

    int rc=0, direct;
    armci_ihdl_t nb_handle = (armci_ihdl_t)uhandle;
    
    if(src == NULL || dst == NULL) return FAIL;
#ifdef ARMCI_PROFILE
    armci_profile_start_strided(&bytes, 0, proc, ARMCI_PROF_NBPUT);
#endif

    direct =SAMECLUSNODE(proc);

    /* aggregate put */
    if(nb_handle && nb_handle->agg_flag == SET) {
      if(direct) { armci_copy(src,dst,bytes); rc=0; }
      else
	rc=armci_agg_save_descriptor(src,dst,bytes,proc,PUT,0,nb_handle); 
#     ifdef ARMCI_PROFILE
        armci_profile_stop_strided(ARMCI_PROF_NBPUT);
#     endif
      return rc;
    }

    if(direct) {
      /*armci_wait needs proc to compute direct*/
      INIT_NB_HANDLE(nb_handle,PUT,proc);
#ifdef BGML
      nb_handle->count = 0;
#endif
      armci_copy(src,dst,bytes);
    }else{
#ifdef BGML
      INIT_NB_HANDLE(nb_handle,PUT,proc);
      nb_handle->count = 1;
      BGML_Callback_t cb_wait={wait_callback, &nb_handle->count};
      BG1S_Memput(&nb_handle->cmpl_info, proc, src, 0, dst, bytes, &cb_wait, 1);
#else
#     ifdef ARMCI_NB_PUT
      /*set tag and op in the nb handle*/
      INIT_NB_HANDLE(nb_handle,PUT,proc);

      
      UPDATE_FENCE_STATE(proc, PUT, 1);
      
      ARMCI_NB_PUT(src, dst, bytes, proc, &nb_handle->cmpl_info);
#     else
#       ifdef ALLOW_PIN
       if(ARMCI_REGION_BOTH_FOUND(src,dst,bytes,armci_clus_id(proc))){
         DO_FENCE(proc,DIRECT_NBPUT);
         INIT_NB_HANDLE(nb_handle,PUT,proc);
	 nb_handle->tag = GET_NEXT_NBTAG();
	 nb_handle->op  = PUT;
	 nb_handle->proc= proc;
	 nb_handle->bufid=NB_NONE;
         armci_client_direct_send(proc, src, dst, bytes,
                                  (void **)(&nb_handle->cmpl_info),
                                  nb_handle->tag,mhloc,mhrem);
         rc=0;
       }else
#       endif
         rc=ARMCI_NbPutS(src, NULL,dst,NULL, &bytes,0,proc,uhandle);
#     endif
#endif /*BGML*/
    }

#ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_NBPUT);
#endif
    return(rc);
}


int ARMCI_NbGet(void *src, void* dst, int bytes, int proc,armci_hdl_t* uhandle)
{

    int rc=0, direct;
    armci_ihdl_t nb_handle = (armci_ihdl_t)uhandle;
    
    if(src == NULL || dst == NULL) return FAIL;
#ifdef ARMCI_PROFILE
    armci_profile_start_strided(&bytes, 0, proc, ARMCI_PROF_NBGET);
#endif    

    direct =SAMECLUSNODE(proc);

    /* aggregate get */
    if(nb_handle && nb_handle->agg_flag == SET) {
      if(direct) { armci_copy(src,dst,bytes); rc=0; }
      else
	rc=armci_agg_save_descriptor(src,dst,bytes,proc,GET,0,nb_handle);
#     ifdef ARMCI_PROFILE
        armci_profile_stop_strided(ARMCI_PROF_NBGET); 
#     endif
      return rc;
    }

    if(direct) {
      /*armci_wait needs proc to compute direct*/
      INIT_NB_HANDLE(nb_handle,PUT,proc);
#ifdef BGML
      nb_handle->count = 0;
#endif
      armci_copy(src,dst,bytes);
    }else{
#ifdef BGML
      INIT_NB_HANDLE(nb_handle,GET,proc);
      nb_handle->count = 1;
      BGML_Callback_t cb_wait={wait_callback, &nb_handle->count};
      BG1S_Memget(&nb_handle->cmpl_info, proc, dst, 0, src, bytes, &cb_wait, 1);
#else

#     ifdef ARMCI_NB_GET
      /*set tag and op in the nb handle*/
      INIT_NB_HANDLE(nb_handle,GET,proc);
      
      ARMCI_NB_GET(src, dst, bytes, proc, &nb_handle->cmpl_info);
#     else
#       ifdef ALLOW_PIN
       if(ARMCI_REGION_BOTH_FOUND(dst,src,bytes,armci_clus_id(proc))){
         DO_FENCE(proc,DIRECT_NBGET);
         INIT_NB_HANDLE(nb_handle,PUT,proc);
	 nb_handle->tag = GET_NEXT_NBTAG();
	 nb_handle->op  = GET;
	 nb_handle->proc= proc;
	 nb_handle->bufid=NB_NONE;
         ARMCI_NBREM_GET(proc, src,NULL,dst,NULL,&bytes, 0, nb_handle);
         rc=0;
       }else
#       endif
      rc=ARMCI_NbGetS(src, NULL,dst,NULL, &bytes,0,proc,uhandle);
#     endif
#endif /*BGML*/
    }
#ifdef ARMCI_PROFILE
    armci_profile_stop_strided(ARMCI_PROF_NBGET);
#endif
    return(rc);
}


static void _armci_rem_value(int op, void *src, void *dst, int proc, 
			     int bytes) {  
  int rc=0;
    
    ORDER(op,proc); /* ensure ordering */
    
#if defined(REMOTE_OP) && !defined(QUADRICS)
    rc = armci_rem_strided(op, NULL, proc, src, NULL, dst, NULL,
			   &bytes, 0, NULL, 0, NULL);
    if(rc) armci_die("ARMCI_Value: armci_rem_strided incomplete", FAIL6);
#else
    
    if(op==PUT) {
      UPDATE_FENCE_STATE(proc, PUT, 1);
#     ifdef LAPI
      SET_COUNTER(ack_cntr, 1);
#     endif
#if defined(BGML)
//fprintf(stderr,"bytes: %d\n",bytes);
//this call is blocking, so local count is fine
      BG1S_t req;
      unsigned count=1;
      BGML_Callback_t cb_wait={wait_callback, &count};
      BG1S_Memput(&req, proc, src, 0, dst, bytes, &cb_wait, 1);
      BGML_Wait(&count);
#else

      armci_put(src, dst, bytes, proc);
#endif
    }
    else {
#     ifdef LAPI
      SET_COUNTER(get_cntr, 1);
#     endif
#if defined(BGML)
//fprintf(stderr,"before memget\n");
   BG1S_t req;
   unsigned count=1;
   BGML_Callback_t cb_wait={wait_callback, &count};
   BG1S_Memget(&req, proc, dst, 0, src, bytes, &cb_wait, 1);
   BGML_Wait(&count);

#else
      armci_get(src, dst, bytes, proc);
#endif
    }
    
    /* deal with non-blocking loads and stores */
# if defined(LAPI) || defined(_ELAN_PUTGET_H)
    if(proc != armci_me){
      if(op == GET){
	WAIT_FOR_GETS; /* wait for data arrival */
      }else {
	WAIT_FOR_PUTS; /* data must be copied out*/
      }
    }
#endif
#endif
}

/* non-blocking remote value put/get operation */
static void _armci_nb_rem_value(int op, void *src, void *dst, int proc, 
				int bytes, armci_ihdl_t nb_handle) {  
    int rc=0, pv=0;

    if(nb_handle && nb_handle->agg_flag == SET) {
      if(op==PUT) pv = 1;
      (void)armci_agg_save_descriptor(src,dst,bytes,proc,op,pv,nb_handle);
      return;
    }
    else {
      if(op==PUT) UPDATE_FENCE_INFO(proc); 
      
      /*set tag and op in the nb handle*/
      if(nb_handle){
	nb_handle->tag = GET_NEXT_NBTAG();
	nb_handle->op  = op;
	nb_handle->proc= proc;
	nb_handle->bufid=NB_NONE;
      }
      else 
	nb_handle = armci_set_implicit_handle(op, proc);
    }

#if defined(REMOTE_OP) && !defined(QUADRICS)
    rc = armci_rem_strided(op, NULL, proc, src, NULL, dst, NULL,
			   &bytes, 0, NULL, 0, nb_handle);
    if(rc) armci_die("ARMCI_Value: armci_rem_strided incomplete", FAIL6);
#else
    
    if(op==PUT) {
      UPDATE_FENCE_STATE(proc, PUT, 1);
#     ifdef LAPI
      SET_COUNTER(ack_cntr, 1);
#     endif
      armci_put(src, dst, bytes, proc);
    }
    else {
#     ifdef LAPI
      SET_COUNTER(get_cntr, 1);
#     endif
      armci_get(src, dst, bytes, proc);
    }
    
    /* deal with non-blocking loads and stores */
# if defined(LAPI) || defined(_ELAN_PUTGET_H)
#   ifdef LAPI
    if(!nb_handle)
#   endif
      {
	if(proc != armci_me){
          if(op == GET){
	    WAIT_FOR_GETS; /* wait for data arrival */
          }else {
            WAIT_FOR_PUTS; /* data must be copied out*/
          }
	}
      }
# endif
#endif
}


#define CHK_ERR(dst, proc)       \
    if(dst==NULL) armci_die("ARMCI_PutValue: NULL pointer passed",FAIL);  \
    if(proc<0) armci_die("ARMCI_PutValue: Invalid process rank", proc);

#define CHK_ERR_GET(src, dst, proc, bytes)       \
    if(src==NULL || dst==NULL) armci_die("ARMCI_GetValue: NULL pointer passed",FAIL);  \
    if(proc<0) armci_die("ARMCI_GetValue: Invalid process rank", proc); \
    if(bytes<0) armci_die("ARMCI_GetValue: Invalid size", bytes);

/** 
 * Register-Originated Put.
 */
int ARMCI_PutValueInt(int src, void *dst, int proc) 
{
    CHK_ERR(dst, proc);
    if( SAMECLUSNODE(proc) ) *(int *)dst = src;
    else _armci_rem_value(PUT, &src, dst, proc, sizeof(int));
    return 0;
}

int ARMCI_PutValueLong(long src, void *dst, int proc) 
{
    CHK_ERR(dst, proc);
    if( SAMECLUSNODE(proc) ) *(long *)dst = src;
    else _armci_rem_value(PUT, &src, dst, proc, sizeof(long));
    return 0;
}

int ARMCI_PutValueFloat(float src, void *dst, int proc) 
{
    CHK_ERR(dst, proc);
    if( SAMECLUSNODE(proc) ) *(float *)dst = src;
    else _armci_rem_value(PUT, &src, dst, proc, sizeof(float));
    return 0;
}

int ARMCI_PutValueDouble(double src, void *dst, int proc) 
{
    CHK_ERR(dst, proc);
    if( SAMECLUSNODE(proc) ) *(double *)dst = src;
    else _armci_rem_value(PUT, &src, dst, proc, sizeof(double));
    return 0;
}

/**
 * Non-Blocking register-originated put.
 */
int ARMCI_NbPutValueInt(int src, void *dst, int proc, armci_hdl_t* usr_hdl) 
{
    CHK_ERR(dst, proc);
    if( SAMECLUSNODE(proc) ) *(int *)dst = src;
    else _armci_nb_rem_value(PUT,&src,dst,proc,sizeof(int),(armci_ihdl_t)usr_hdl);
    return 0;
}

int ARMCI_NbPutValueLong(long src, void *dst, int proc, armci_hdl_t* usr_hdl) 
{
    CHK_ERR(dst, proc);
    if( SAMECLUSNODE(proc) ) *(long *)dst = src;
    else _armci_nb_rem_value(PUT,&src,dst,proc,sizeof(long),(armci_ihdl_t)usr_hdl);
    return 0;
}

int ARMCI_NbPutValueFloat(float src, void *dst, int proc, armci_hdl_t* usr_hdl)
{
    CHK_ERR(dst, proc);
    if( SAMECLUSNODE(proc) ) *(float *)dst = src;
    else  _armci_nb_rem_value(PUT,&src,dst,proc,sizeof(float),(armci_ihdl_t)usr_hdl);
    return 0;
}

int ARMCI_NbPutValueDouble(double src, void *dst, int proc, armci_hdl_t* usr_hdl)
{
    CHK_ERR(dst, proc);
    if( SAMECLUSNODE(proc) ) *(double *)dst = src;
    else  _armci_nb_rem_value(PUT,&src,dst,proc,sizeof(double),(armci_ihdl_t)usr_hdl);
     return 0;
 }

#if 1
/** 
 * Register-Originated Get.
 */
int ARMCI_GetValueInt(void *src, int proc) 
{
    int dst;
    if( SAMECLUSNODE(proc) ) return *(int *)src;
    else _armci_rem_value(GET, src, &dst, proc, sizeof(int));
    return dst;
}

long ARMCI_GetValueLong(void *src, int proc) 
{
    long dst;
    if( SAMECLUSNODE(proc) ) return *(long *)src;
    else _armci_rem_value(GET, src, &dst, proc, sizeof(long));
    return dst;
}

float ARMCI_GetValueFloat(void *src, int proc) 
{
    float dst;
    if( SAMECLUSNODE(proc) ) return *(float *)src;
    else _armci_rem_value(GET, src, &dst, proc, sizeof(float));
    return dst;
}

double ARMCI_GetValueDouble(void *src, int proc) 
{
    double dst;
    if( SAMECLUSNODE(proc) ) return *(double *)src;
    else _armci_rem_value(GET, src, &dst, proc, sizeof(double));
    return dst;
}

#endif

#if 0
/**
 * Register-Originated Get.
 */
int ARMCI_GetValue(void *src, void *dst, int proc, int bytes) 
{
    CHK_ERR_GET(src, dst, proc, bytes);
    if( SAMECLUSNODE(proc) ) { armci_copy(src, dst, bytes); }
    else _armci_rem_value(GET, src, dst, proc, bytes);
    return 0;
}

/**
 * Non-Blocking register-originated get.
 */
int ARMCI_NbGetValue(void *src, void *dst, int proc, int bytes, armci_hdl_t* usr_hdl) 
{
    CHK_ERR_GET(src, dst, proc, bytes);
    if( SAMECLUSNODE(proc) ) { armci_copy(src, dst, bytes); }
    else _armci_nb_rem_value(GET, src, dst, proc, bytes, (armci_ihdl_t)usr_hdl);
    return 0;
}
#endif


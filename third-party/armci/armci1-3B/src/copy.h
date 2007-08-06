/* $Id$ */
#ifndef _COPY_H_
#define _COPY_H_

#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#  include <string.h>
#endif
#ifdef DECOSF
#include <c_asm.h>
#endif

#if defined(NOFORT) || defined(HITACHI) || defined(CRAY_T3E)\
        || defined(CRAY_SHMEM) || defined(BGML)
#  define MEMCPY
#endif
#if defined(LINUX64) && defined(SGIALTIX) && defined(MPI)
/* fastbcopy from Wayne Vieira and Gerardo Cisneros */
#define MEMCPY
#define armci_copy(src, dst, len) _fastbcopy(src, dst, len)
#define memcpy(dst, src, len)  _fastbcopy(src, dst, len)
#define bcopy(src, dst, len) _fastbcopy(src, dst, len)
#endif

#ifndef EXTERN
#   define EXTERN extern
#endif
 
#ifdef NEC
#  define memcpy1 _VEC_memcpy
#  define armci_copy1(src,dst,n) _VEC_memcpy((dst),(src),(n))
   EXTERN long long _armci_vec_sync_flag;
#endif

#if defined(SGI) || defined(FUJITSU) || defined(HPUX) || defined(SOLARIS) || defined (DECOSF) || defined(__ia64__) || defined(__crayx1)
#   define PTR_ALIGN
#endif

#if defined(NB_NONCONT) && !defined(CRAY_SHMEM) && !defined(QUADRICS)
#error NB_NONCONT can only be defined for CRAY-SHMEM and ELAN
#endif

#if defined(SHMEM_HANDLE_SUPPORTED) && !defined(CRAY_SHMEM)
#warning SHMEM_HANDLE_SUPPORTED should not be defined on a non CRAY_SHMEM network
#endif

/* 08/30/06 moved up here from lines 252-397, MEM_FENCE before FENCE_NODE */
#ifdef COPY686 
     extern void *armci_asm_memcpy(void *dst, const void *src, size_t n, int tid);
     extern void *armci_asm_memcpy_nofence(void *d,const void *s,size_t n, int id);
#    ifdef SERVER_CONTEXT
#      define armci_copy(src,dst,n) {\
        int _id= (SERVER_CONTEXT)?1:0; armci_asm_memcpy((dst), (src), (n), _id);}
#    else
#      define armci_copy(src,dst,n)  armci_asm_memcpy((dst), (src), (n), 0)
#    endif
#    define armci_copy_nofence(_s,_d,_n) armci_asm_memcpy_nofence((_d),(_s),(_n),0)
#    ifndef MEMCPY
#       define MEMCPY
#    endif
#    define MEM_FENCE armci_asm_mem_fence()
     extern void armci_asm_mem_fence();
#endif
                                                 
#if  defined(MEMCPY)  && !defined(armci_copy)
#if defined(BGML)
#define armci_copy(src, dst, n) BGLML_memcpy((dst), (src), (n))
#else
#    define armci_copy(src,dst,n)  memcpy((dst), (src), (n)) 
#endif
#endif

#ifdef NEC
#    define MEM_FENCE {mpisx_clear_cache(); _armci_vec_sync_flag=1;mpisx_syncset0_long(&_armci_vec_sync_flag);}
#endif

#ifdef DECOSF
#    define MEM_FENCE asm ("mb")
#endif

#if defined(NEED_MEM_SYNC)
#  ifdef AIX
#    define MEM_FENCE {int _dummy=1; _clear_lock((int *)&_dummy,0); }
#  elif defined(__ia64)
#    if defined(__GNUC__) && !defined (__INTEL_COMPILER)
#       define MEM_FENCE __asm__ __volatile__ ("mf" ::: "memory");
#    else /* Intel Compiler */ 
        extern void _armci_ia64_mb();
#       define MEM_FENCE _armci_ia64_mb();
#    endif
#  elif defined(LINUX) && defined(__GNUC__) && defined(__ppc__)
#    define MEM_FENCE \
             __asm__ __volatile__ ("isync" : : : "memory");
#  endif
#endif

#ifndef armci_copy
# ifdef PTR_ALIGN
#   define armci_copy(src,dst,n)     \
     do if( ((n) < THRESH1D)   || ((n)%ALIGN_SIZE) || \
            ((unsigned long)(src)%ALIGN_SIZE) ||\
            ((unsigned long)(dst)%ALIGN_SIZE)) memcpy((dst),(src),(n));\
        else{ int _bytes=(n)/sizeof(double); DCOPY1D((src),(dst),&_bytes);}\
     while (0)
# else
#   define armci_copy(src,dst,n)     \
     do if( ((n) < THRESH1D) || ((n)%ALIGN_SIZE) ) memcpy((dst), (src), (n));\
          else{ int _bytes=(n)/sizeof(double); DCOPY1D((src),(dst),&_bytes);}\
     while (0)
# endif
#endif

/****************************** 2D Copy *******************/


#ifndef MEMCPY
#   define DCopy2D(rows, cols, src_ptr, src_ld, dst_ptr, dst_ld){\
      int rrows, ldd, lds, ccols;\
          rrows = (rows);\
          lds =   (src_ld);\
          ldd =   (dst_ld);\
          ccols = (cols);\
          DCOPY2D(&rrows, &ccols, src_ptr, &lds,dst_ptr,&ldd);\
      }

#else
#   define DCopy2D(rows, cols, src_ptr, src_ld, dst_ptr, dst_ld){\
    int j, nbytes = sizeof(double)* rows;\
    char *ps=src_ptr, *pd=dst_ptr;\
      for (j = 0;  j < cols;  j++){\
          armci_copy(ps, pd, nbytes);\
          ps += sizeof(double)* src_ld;\
          pd += sizeof(double)* dst_ld;\
      }\
    }
#endif


#   define ByteCopy2D(bytes, count, src_ptr, src_stride, dst_ptr,dst_stride){\
    int _j;\
    char *ps=src_ptr, *pd=dst_ptr;\
      for (_j = 0;  _j < count;  _j++){\
          armci_copy(ps, pd, bytes);\
          ps += src_stride;\
          pd += dst_stride;\
      }\
    }

#if defined(FUJITSU)

#   define armci_put2D(p, bytes,count,src_ptr,src_stride,dst_ptr,dst_stride)\
           CopyPatchTo(src_ptr, src_stride, dst_ptr, dst_stride, count,bytes, p)

#   define armci_get2D(p, bytes, count, src_ptr,src_stride,dst_ptr,dst_stride)\
           CopyPatchFrom(src_ptr, src_stride, dst_ptr, dst_stride,count,bytes,p)

#elif defined(HITACHI) || defined(_ELAN_PUTGET_H) && !defined(NB_NONCONT)

#if defined(QUADRICS)
#if 0
#   define WAIT_FOR_PUTS elan_putWaitAll(elan_base->state,200)
#   define WAIT_FOR_GETS elan_getWaitAll(elan_base->state,200)
#else
#   define WAIT_FOR_PUTS armcill_wait_put()
#   define WAIT_FOR_GETS armcill_wait_get()
    extern void armcill_wait_put();
    extern void armcill_wait_get();
#endif
#endif

    extern void armcill_put2D(int proc, int bytes, int count,
                void* src_ptr,int src_stride, void* dst_ptr,int dst_stride);
    extern void armcill_get2D(int proc, int bytes, int count,
                void* src_ptr,int src_stride, void* dst_ptr,int dst_stride);
#   define armci_put2D armcill_put2D
#   define armci_get2D armcill_get2D

#elif defined(NB_NONCONT)

    extern void armcill_wait_put();
    extern void armcill_wait_get();
#   define WAIT_FOR_PUTS armcill_wait_put()
#   define WAIT_FOR_GETS armcill_wait_get()
 
    extern void armcill_put2D(int proc, int bytes, int count,
                void* src_ptr,int src_stride, void* dst_ptr,int dst_stride);
    extern void armcill_get2D(int proc, int bytes, int count,
                void* src_ptr,int src_stride, void* dst_ptr,int dst_stride);
#   define armci_put2D armcill_put2D
#   define armci_get2D armcill_get2D

#   if   defined(QUADRICS)

#       define armcill_nb_put(_dst, _src, _sz, _proc, _hdl)\
               elan_put(elan_base->state,_src,_dst,(size_t)_sz,_proc)
#       define armcill_nb_get(_dst, _src, _sz, _proc, _hdl)\
               elan_get(elan_base->state,_src,_dst,(size_t)_sz,_proc)
#       define armcill_nb_wait(_hdl)\
               elan_wait(_hdl,100)

#   elif defined(CRAY_SHMEM)

#       define armcill_nb_put(_dst, _src, _sz, _proc, _hdl)\
               shmem_putmem_nb(_dst, _src, (size_t)_sz, _proc, _hdl)
#       define armcill_nb_wait(_hdl)\
               shmem_wait_nb(_hdl)

#       if defined (CATAMOUNT)
#           define armcill_nb_get(_dst, _src, _sz, _proc, _hdl)\
                   shmem_getmem(_dst, _src, (size_t)_sz, _proc)
#       else
#           define armcill_nb_get(_dst, _src, _sz, _proc, _hdl)\
                   shmem_getmem_nb(_dst, _src, (size_t)_sz, _proc, _hdl)
#       endif

#   endif

#else
#   define armci_put2D(proc,bytes,count,src_ptr,src_stride,dst_ptr,dst_stride){\
    int _j;\
    char *ps=src_ptr, *pd=dst_ptr;\
      for (_j = 0;  _j < count;  _j++){\
          armci_put(ps, pd, bytes, proc);\
          ps += src_stride;\
          pd += dst_stride;\
      }\
    }


#   define armci_get2D(proc,bytes,count,src_ptr,src_stride,dst_ptr,dst_stride){\
    int _j;\
    char *ps=src_ptr, *pd=dst_ptr;\
      for (_j = 0;  _j < count;  _j++){\
          armci_get(ps, pd, bytes, proc);\
          ps += src_stride;\
          pd += dst_stride;\
      }\
    }
#endif
   
/* macros to ensure ordering of consecutive puts or gets following puts */
#if defined(LAPI)

#   include "lapidefs.h"

#elif defined(_CRAYMPP) || defined(QUADRICS) || defined(__crayx1)\
   || defined(CRAY_SHMEM)
#if defined(CRAY) || defined(XT3)
#   include <mpp/shmem.h>
#else
#   include <unistd.h>
#ifndef ptrdiff_t
#   include <malloc.h>
#endif
#   include <shmem.h>
#endif
#   ifdef ELAN_ACC
#     define FENCE_NODE(p) {\
          if(((p)<armci_clus_first)||((p)>armci_clus_last))armci_elan_fence(p);}
#     define UPDATE_FENCE_STATE(p, op, nissued) 
#   else
      int cmpl_proc;
#     ifdef DECOSF
#       define FENCE_NODE(p) if(cmpl_proc == (p)){\
             if(((p)<armci_clus_first)||((p)>armci_clus_last))shmem_quiet();\
             else asm ("mb"); }
#     else
#       define FENCE_NODE(p) if(cmpl_proc == (p)){\
             if(((p)<armci_clus_first)||((p)>armci_clus_last))shmem_quiet(); }
#     endif
#     define UPDATE_FENCE_STATE(p, op, nissued) if((op)==PUT) cmpl_proc=(p);
#   endif
#else
#   if defined(GM) && defined(ACK_FENCE) 
     extern void armci_gm_fence(int p);
#    define FENCE_NODE(p) armci_gm_fence(p)
#   elif defined(BGML)
#   include "bgmldefs.h"
#   define FENCE_NODE(p) BGML_WaitProc(p)    
#   else
#    define FENCE_NODE(p)
#   endif   
#   define UPDATE_FENCE_STATE(p, op, nissued)

#endif


#ifdef NEC
#  define THRESH 1
#  define THRESH1D 1
#else
#  define THRESH 32
#  define THRESH1D 512
#endif
#define ALIGN_SIZE sizeof(double)

/********* interface to fortran 1D and 2D memory copy functions ***********/
/* dcopy2d_u_ uses explicit unrolled loops to depth 4 */
#if   defined(AIX) || defined(NOUNDERSCORE)
#     define DCOPY2D	dcopy2d_u
#     define DCOPY1D	dcopy1d_u
#elif defined(LINUX) || defined(__crayx1) || defined(HPUX64) || defined(DECOSF)
#     define DCOPY2D	dcopy2d_n_
#     define DCOPY1D	dcopy1d_n_
#elif defined(CRAY)  || defined(WIN32) || defined(HITACHI)
#     define DCOPY2D    DCOPY2D_N
#     define DCOPY1D    DCOPY1D_N
#elif defined(BGML)
#     define DCOPY2D dcopy2d_u__
#     define DCOPY1D dcopy1d_u__
#else
#     define DCOPY2D	dcopy2d_u_
#     define DCOPY1D	dcopy1d_u_
#endif
void FATR DCOPY2D(int*, int*, void*, int*, void*, int*); 
void FATR DCOPY1D(void*, void*, int*); 

#if   defined(AIX) || defined(NOUNDERSCORE)
#     define DCOPY21	dcopy21
#     define DCOPY12	dcopy12
#     define DCOPY31	dcopy31
#     define DCOPY13	dcopy13
#elif (defined(CRAY) &&!defined(__crayx1)) || defined(WIN32) || defined(HITACHI)
#else
#     define DCOPY21	dcopy21_
#     define DCOPY12	dcopy12_
#     define DCOPY31	dcopy31_
#     define DCOPY13	dcopy13_
#endif

void FATR DCOPY21(int*, int*, void*, int*, void*, int*); 
void FATR DCOPY12(int*, int*, void*, int*, void*, int*); 
void FATR DCOPY31(int*, int*, int*, void*, int*, int*, void*, int*); 
void FATR DCOPY13(int*, int*, int*, void*, int*, int*, void*, int*); 


/***************************** 1-Dimensional copy ************************/
#if defined(QUADRICS)
#   include <elan/elan.h>

#   if defined(_ELAN_PUTGET_H)
#      define qsw_put(src,dst,n,proc) \
        elan_wait(elan_put(elan_base->state,src,dst,n,proc),elan_base->waitType)
#      define qsw_get(src,dst,n,proc) \
        elan_wait(elan_get(elan_base->state,src,dst,n,proc),elan_base->waitType)
/*
#      define ARMCI_NB_PUT(src,dst,n,proc,phandle)\
              *(phandle)=elan_put(elan_base->state,src,dst,n,proc)
*/
#ifdef DOELAN4
extern void armci_elan_put_with_tracknotify(char *src,char *dst,int n,int proc, ELAN_EVENT **phandle);
#      define ARMCI_NB_PUT(src,dst,n,proc,phandle)\
              armci_elan_put_with_tracknotify(src,dst,n,proc,phandle)
#endif
			                      
#      define ARMCI_NB_GET(src,dst,n,proc,phandle)\
              *(phandle)=elan_get(elan_base->state,src,dst,n,proc)
#      define ARMCI_NB_WAIT(handle) if(handle)elan_wait(handle,elan_base->waitType) 
#      define ARMCI_NB_TEST(handle,_succ) (*(_succ))= (handle)? !elan_poll(handle,1L): 1 
#   else
#      define qsw_put(src,dst,n,proc) shmem_putmem((dst),(src),(int)(n),(proc))
#      define qsw_get(src,dst,n,proc) shmem_getmem((dst),(src),(int)(n),(proc))
#   endif

#   define armci_put(src,dst,n,proc)\
           if(((proc)<=armci_clus_last) && ((proc>= armci_clus_first))){\
              armci_copy(src,dst,n);\
           } else { qsw_put(src,dst,n,proc);}
#   define armci_get(src,dst,n,proc) \
           if(((proc)<=armci_clus_last) && ((proc>= armci_clus_first))){\
             armci_copy(src,dst,n);\
           } else { qsw_get((src),(dst),(int)(n),(proc));}

#elif defined(CRAY_T3E) || defined(CRAY_SHMEM)
#      define armci_copy_disabled(src,dst,n)\
        if((n)<256 || n%sizeof(long) ) memcpy((dst),(src),(n));\
        else {\
          shmem_put((long*)(dst),(long*)(src),(int)(n)/sizeof(long),armci_me);\
          shmem_quiet(); }

#      define armci_put(src,dst,n,proc) \
              shmem_put32((void *)(dst),(void *)(src),(int)(n)/4,(proc));\
              shmem_quiet()

#      define armci_get(src,dst,n,proc) \
              shmem_get32((void *)(dst),(void *)(src),(int)(n)/4,(proc));\
              shmem_quiet()

#elif  defined(HITACHI)

        extern void armcill_put(void *src, void *dst, int bytes, int proc);
        extern void armcill_get(void *src, void *dst, int bytes, int proc);

#      define armci_put(src,dst,n,proc) \
            if(((proc)<=armci_clus_last) && ((proc>= armci_clus_first))){\
               armci_copy(src,dst,n);\
            } else { armcill_put((src), (dst),(n),(proc));}

#      define armci_get(src,dst,n,proc)\
            if(((proc)<=armci_clus_last) && ((proc>= armci_clus_first))){\
               armci_copy(src,dst,n);\
            } else { armcill_get((src), (dst),(n),(proc));}

#elif  defined(FUJITSU)

#      include "fujitsu-vpp.h"
#      ifndef __sparc
#         define armci_copy(src,dst,n)  _MmCopy((char*)(dst), (char*)(src), (n))
#      endif
#      define armci_put  CopyTo
#      define armci_get  CopyFrom

#elif  defined(LAPI)

#      include <lapi.h>
       extern lapi_handle_t lapi_handle;

#      define armci_put(src,dst,n,proc)\
              if(proc==armci_me){\
                 armci_copy(src,dst,n);\
              } else {\
              if(LAPI_Put(lapi_handle, (uint)proc, (uint)n, (dst), (src),\
                 NULL, &ack_cntr.cntr, &cmpl_arr[proc].cntr))\
                  ARMCI_Error("LAPI_put failed",0); else; }

       /**** this copy is nonblocking and requires fence to complete!!! ****/
#      define armci_get(src,dst,n,proc) \
              if(proc==armci_me){\
                 armci_copy(src,dst,n);\
              } else {\
              if(LAPI_Get(lapi_handle, (uint)proc, (uint)n, (src), (dst), \
                 NULL, &get_cntr.cntr))\
                 ARMCI_Error("LAPI_Get failed",0);else;}

#      define ARMCI_NB_PUT(src,dst,n,proc,cmplt)\
              {if(LAPI_Setcntr(lapi_handle, &((cmplt)->cntr), 0))\
                  ARMCI_Error("LAPI_Setcntr in NB_PUT failed",0);\
              (cmplt)->val=1;\
              if(LAPI_Put(lapi_handle, (uint)proc, (uint)n, (dst), (src),\
                 NULL, &((cmplt)->cntr), &cmpl_arr[proc].cntr))\
                  ARMCI_Error("LAPI_put failed",0); else;}

#      define ARMCI_NB_GET(src,dst,n,proc,cmplt)\
              {if(LAPI_Setcntr(lapi_handle, &((cmplt)->cntr), 0))\
                  ARMCI_Error("LAPI_Setcntr in NB_GET failed",0);\
              (cmplt)->val=1;\
              if(LAPI_Get(lapi_handle, (uint)proc, (uint)n, (src), (dst), \
                 NULL, &((cmplt)->cntr)))\
                 ARMCI_Error("LAPI_Get NB_GET failed",0);else;}

#      define ARMCI_NB_WAIT(cmplt) CLEAR_COUNTER((cmplt))
#      define ARMCI_NB_TEST(cmplt,_succ) TEST_COUNTER((cmplt),(_succ))
       
#elif defined(PORTALS)
#define armci_put ARMCI_Put
#define armci_get ARMCI_Get

#elif defined(BGML)
#define armci_get(src, dst, n, p)   ARMCI_Get(src, dst, n, p)
#define armci_put(src, dst, n, p)   ARMCI_Put(src, dst, n, p)

#else

#      define armci_get(src,dst,n,p)    armci_copy((src),(dst),(n))
#      define armci_put(src,dst,n,p)    armci_copy((src),(dst),(n))

#endif

#ifndef MEM_FENCE
#   define MEM_FENCE
#endif
#ifndef armci_copy_fence
#   define armci_copy_fence armci_copy
#endif

#endif

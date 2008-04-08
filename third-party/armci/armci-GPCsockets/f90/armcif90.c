#include "CompilerCharacteristics.h"
#include <F90Compiler.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <armci.h>
#include <arraydesc.h>
#define HIDDEN_DESC
#if defined(F90_SYM_CASE_LOWER)
#  define ARMCI_Arr_init  F90_SYMBOL( armci_arr_init )
#  define ARMCI_Arr_finalize  F90_SYMBOL( armci_arr_finalize )
#  define ARMCI_Malloc_farray F90_SYMBOL( armci_malloc_farray )
#  define ARMCI_Free_farray   F90_SYMBOL( armci_free_farray )
#  define ARMCI_Put_farrays F90_SYMBOL( armci_put_farrays )
#  define ARMCI_NbPut_farrays F90_SYMBOL( armci_nbput_farrays )
#  define ARMCI_Get_farrays F90_SYMBOL( armci_get_farrays )
#  define ARMCI_NbGet_farrays F90_SYMBOL( armci_nbget_farrays )
#  define ARMCI_Sync   F90_SYMBOL( armci_sync )
#  define ARMCI_Waitall_fa   F90_SYMBOL( armci_waitall_fa )
#  define ARMCI_Notify_fa   F90_SYMBOL( armci_notify_fa )
#  define ARMCI_NotifyWait_fa   F90_SYMBOL( armci_notifywait_fa )
#  define ARMCI_Malloc_fa F90_SYMBOL( armci_malloc_fa )
#endif /* F90_SYM_CASE_LOWER */

#if defined(F90_SYM_CASE_MIXED)
#  define ARMCI_Arr_init  F90_SYMBOL( ARMCI_Arr_init )
#  define ARMCI_Arr_finalize  F90_SYMBOL( ARMCI_Arr_finalize )
#  define ARMCI_Malloc_farray F90_SYMBOL( ARMCI_Malloc_farray )
#  define ARMCI_Free_farray   F90_SYMBOL( ARMCI_Free_farray )
#  define ARMCI_Put_farrays F90_SYMBOL( ARMCI_Put_farrays )
#  define ARMCI_NbPut_farrays F90_SYMBOL( ARMCI_NbPut_farrays )
#  define ARMCI_Get_farrays F90_SYMBOL( ARMCI_Get_farrays )
#  define ARMCI_NbGet_farrays F90_SYMBOL( ARMCI_NbGet_farrays )
#  define ARMCI_Sync   F90_SYMBOL( ARMCI_Sync )
#  define ARMCI_Waitall_fa   F90_SYMBOL( ARMCI_Waitall_fa )
#  define ARMCI_Notify_fa   F90_SYMBOL( ARMCI_Notify_fa )
#  define ARMCI_NotifyWait_fa   F90_SYMBOL( ARMCI_NotifyWait_fa )
#endif /* F90_SYM_CASE_MIXED */

#ifdef F90_SYM_CASE_UPPER
#  define ARMCI_Arr_init  F90_SYMBOL( ARMCI_ARR_INIT )
#  define ARMCI_Arr_finalize  F90_SYMBOL( ARMCI_ARR_FINALIZE )
#  define ARMCI_Malloc_farray F90_SYMBOL( ARMCI_MALLOC_FARRAY )
#  define ARMCI_Free_farray   F90_SYMBOL( ARMCI_FREE_FARRAY )
#  define ARMCI_Put_farrays F90_SYMBOL( ARMCI_PUT_FARRAYS )
#  define ARMCI_NbPut_farrays F90_SYMBOL( ARMCI_NBPUT_FARRAYS )
#  define ARMCI_Get_farrays F90_SYMBOL( ARMCI_GET_FARRAYS )
#  define ARMCI_NbGet_farrays F90_SYMBOL( ARMCI_NBGET_FARRAYS )
#  define ARMCI_Sync   F90_SYMBOL( ARMCI_SYNC )
#  define ARMCI_Waitall_fa   F90_SYMBOL( ARMCI_WAITALL_FA )
#  define ARMCI_Notify_fa   F90_SYMBOL( ARMCI_NOTIFY_FA )
#  define ARMCI_NotifyWait_fa   F90_SYMBOL( ARMCI_NOTIFYWAIT_FA )
#endif /* F90_SYM_CASE_UPPER */

#ifdef F90_INTEL
#  define GET_ADDRESS_FROM_FDSC(dsc) (((void**)(dsc))[2])
#else
#  define GET_ADDRESS_FROM_FDSC(dsc) (((void**)(dsc))[0])
#endif

typedef struct {
  int lo[MAXDIM];
  int hi[MAXDIM];
  int stride[MAXDIM];
} array_slice_t;


/* allows simple test to assure that allocated and freed addresses the same */
void* g_save_addr;

/* structure containing function pointers to chasm lib, must be initialized */
F90_CompilerCharacteristics cc;

extern int armci_nproc, armci_me;


void ARMCI_Arr_init()
{
  F90_SetCompilerCharacteristics(&cc, FORTRAN_COMPILER);
  ARMCI_Init();
}

void ARMCI_Arr_finalize()
{
  ARMCI_Finalize();
}

void ARMCI_Waitall_fa()
{
          ARMCI_WaitAll();
}

void ARMCI_Notify_fa(int* proc)
{
        int tempvar;
           tempvar = armci_notify(*proc);
}

void ARMCI_NotifyWait_fa(int* proc)
{
        int rc,wc;
          rc = armci_notify_wait(*proc,&wc);
}

void armci_abort()
{
}
void armci_abort_()
{
}
        
void ARMCI_Sync()
{
  ARMCI_Barrier();
}

void ARMCI_Terminate()
{

}

void ARMCI_Malloc_farray(void* dv, int* rank, int* elemsize,
                     int lb_in[], int ub_in[], int* rc, void* dv_hidden)
{
long stride[MAXDIM];
long lo[MAXDIM];
unsigned long extent[MAXDIM];
void* dv_local;
armci_arr_dsc_t *ad;
int i;

  _array_create(&g_save_addr,*elemsize, *rank, lb_in, ub_in);
  if(!g_save_addr)ARMCI_Error("_array_create failed",*rank);

  /************************ chasm interface ***************************/
  /* memory for dope vector */

  dv_local = malloc( cc.getArrayDescSize(*rank) );
  if(!dv_local) {*rc = -2; return;}

  ad = GET_ARR_DSC_FROM_ARRAY(g_save_addr);
/*  *rc = cc.setArrayDesc(dv_local, g_save_addr, *rank, F90_ArrayPointer,
                   F90_Integer, elemsize, ad->lo, ad->extent, ad->stride);
*/
  for(i=0; i< *rank; i++) lo[i] = (long)ad->lo[i];
  for(i=0; i< *rank; i++) stride[i] = (long)ad->stride[i];
  for(i=0; i< *rank; i++) extent[i] = (unsigned long) ad->extent[i];
  *rc = cc.setArrayDesc(dv_local, g_save_addr, *rank, F90_ArrayPointer,
                   F90_Integer, *elemsize, lo, extent, stride);
  if (*rc) { fprintf(stderr, "ERROR in setting array descriptor\n");
    return;
  }

  /* copy dope vector to the stack memory */
  *rc = cc.copyToArrayDescAndHidden(dv_local, *rank,
                                    F90_ArrayPointer, dv, dv_hidden);
  if (*rc) { fprintf(stderr, "ERROR in copying array descriptor\n");
    return;
  }
  free(dv_local);
   
}


void ARMCI_Free_farray(void* dv, int* rank, int* rc, void* dv_hidden)
{
  F90_CompilerCharacteristics cc;
  void* dv_local;
  void* addr;
  armci_arr_dsc_t *ad;

  *rc = 0;

  F90_SetCompilerCharacteristics(&cc, FORTRAN_COMPILER);

  dv_local = cc.createArrayDesc(dv, dv_hidden, *rank, F90_ArrayPointer);
  if (dv_local == 0) {
    fprintf(stderr, "ERROR in createArrayDesc\n");
    *rc = -1;
    return;
  }

  addr = cc.getArrayBaseAddress(dv_local, *rank);
  if (addr == 0) {
    fprintf(stderr, "ERROR in getArrayBaseAddress rank=%d\n",*rank);
    *rc = -1;
    return;
  }
  _array_free(addr);
}

static long Index(int rank, int subscript[], int lo[], int extent[])
{
        long idx = 0, i, factor=1;
        for(i=0;i<rank;i++){
                idx += (subscript[i]-lo[i])*factor;
                factor *= extent[i];
        }
        return (long)idx;
}

void ARMCI_Put_farrays(void* dv_src, array_slice_t* src_slc,
                  void* dv_dst, array_slice_t* dst_slc,
                  int* proc, int* rank, int* rc,
                  void* dv_h_src, void* dv_h_dst)
{
  void *addr_src, *addr_dst, *dv_l_src, *dv_l_dst;
  armci_arr_dsc_t *ad_dst;
  void *ptr_src, *ptr_dst;
  int i;
  int src_count[MAXDIM+1];
  int dst_count[MAXDIM+1];
  int src_stride[MAXDIM];
  int dst_stride[MAXDIM];
  int src_unit_stride, src_first_stride_eq_1;
  int dst_unit_stride, dst_first_stride_eq_1;
  int mv_rank;
  int ad_src_rank;
  int ad_src_elemsize;
  int ad_src_lo[MAXDIM];
  int ad_src_extent[MAXDIM];
  int ad_src_stride[MAXDIM];
  *rc = 0;

#ifdef HIDDEN_DESC
  dv_l_src = cc.createArrayDesc(dv_src, dv_h_src, *rank, F90_ArrayPointer);
#else
  dv_l_src = dv_src;
#endif
  if (dv_l_src == 0) {
    fprintf(stderr, "ARMCI_Put_farrays: ERROR in createArrayDesc\n");
    *rc = -1;
    return;
  }
  addr_src = cc.getArrayBaseAddress(dv_l_src, *rank);
  if (addr_src == 0) {
    fprintf(stderr, "ARMCI_Put_farrays: ERROR in getArrayBaseAddress\n");
    *rc = -1;
    return;
  }

#ifdef HIDDEN_DESC
  dv_l_dst = cc.createArrayDesc(dv_dst, dv_h_dst, *rank, F90_ArrayPointer);
#else
  dv_l_dst = dv_dst;
#endif
  if (dv_l_dst == 0) {
    fprintf(stderr, "ARMCI_Put_farrays: ERROR in createArrayDesc\n");
    *rc = -1;
    return;
  }
  addr_dst = cc.getArrayBaseAddress(dv_l_dst, *rank);
  if (addr_dst == 0) {
    fprintf(stderr, "ARMCI_Put_farrays: ERROR in getArrayBaseAddress\n");
    *rc = -1;
    return;
  }
#if 0
  assert(addr_src = g_save_addr);
  assert(addr_dst = g_save_addr);
#endif
  /*
  ad_src = GET_ARR_DSC_FROM_ARRAY(addr_src);
  Need to replace ad_src_rank, ad_src_lo, ad_src_extent, ad_src_stride,
  and ad_src_elemsize with local stuff from the chasm interface.
  No way to get elemsize from chasm stuff, so for
  now take src_elemsize to be the same as dst_elemsize.
  */
  
  ad_dst = GET_ARR_DSC_FROM_ARRAY(addr_dst);
  ad_src_elemsize = ad_dst->elemsize;
  ad_src_rank = ad_dst->rank;

  for (i=0;i<ad_src_rank;i++) {
    ad_src_lo[i] = cc.getArrayLowerBound(dv_l_src, ad_src_rank, i+1);
    ad_src_extent[i] = cc.getArrayExtent(dv_l_src,ad_src_rank, i+1);
    ad_src_stride[i] = cc.getArrayStrideMult(dv_l_src,ad_src_rank,i+1);
  }

  ptr_src= Index(ad_src_rank, src_slc->lo, ad_src_lo, ad_src_extent)*ad_src_elemsize + (char*) addr_src; 
  ptr_dst= Index(ad_dst->rank, dst_slc->lo, ad_dst->lo, ad_dst->extent)*ad_dst->elemsize + (char*) ad_dst->aptr[*proc]; 

  /* The following attempts to deal with non-unit stride. */
  /* Seems like we assume that src_slc->hi[i] >= src_slc->lo[i] and 
     stride[i] > 0 for all i. One might wish to modify that 
     assumption at some point. Should we check that the patches
     are the same size, does ARMCI_Puts handle that?
  */
  if (ad_dst->rank != ad_src_rank) {
    fprintf(stderr, "ARMCI_Put_farrays: ERROR ranks of src and dst do not match\n");
    *rc = -1;
    return;
  }
  src_first_stride_eq_1 = 1;
  src_unit_stride = 1;
  for(i=0; i< ad_src_rank;  i++) {
    if (src_slc->stride[i] > 1) {
      if (i == 0) {
	src_first_stride_eq_1 = 0;
      }
      src_unit_stride = 0;
      src_count[i] =   (int)((src_slc->hi[i] -src_slc->lo[i]+src_slc->stride[i])/src_slc->stride[i]);
    } else {
      src_count[i] =   src_slc->hi[i] -src_slc->lo[i]+1;
    }
  }
  dst_first_stride_eq_1 = 1;
  dst_unit_stride = 1;
  for(i=0; i< ad_dst->rank;  i++) {
    if (dst_slc->stride[i] > 1) {
      if (i == 0) {
	dst_first_stride_eq_1 = 0;
      }
      dst_unit_stride = 0;
      dst_count[i] =   (int)((dst_slc->hi[i] -dst_slc->lo[i]+dst_slc->stride[i])/dst_slc->stride[i]);
    } else {
      dst_count[i] =   dst_slc->hi[i] -dst_slc->lo[i]+1;
    }
  }
  /* Might want to check that src_count and dst_count match here?,
     perhaps that happens in ARMC_PutS but I think not. 
  */
  for (i=0;i<ad_src_rank;i++) {
    if (dst_count[i] != src_count[i]) {
       fprintf(stderr, "ARMCI_Put_farrays: ERROR counts of src and dst slices do not match\n");
       *rc = -1;
       return;
    }
  }
      
  if ((src_unit_stride != 0) && (dst_unit_stride != 0)) {
    mv_rank = ad_src_rank-1;
    for (i=0; i<mv_rank; i++) {
      src_stride[i] = ad_src_stride[i+1];
    }
    for (i=0; i<mv_rank; i++) {
      dst_stride[i] = ad_dst->stride[i+1];
    }
    src_count[0] *= ad_src_elemsize;
  } else {
    /* If first dimension strides are both 1, then we
       can till use ad_src->rank-1 levels. Just 
       the strides need changing.
    */
    
    if ((src_first_stride_eq_1 != 0) && (dst_first_stride_eq_1 != 0)) {
      mv_rank = ad_src_rank-1;
      src_count[0] *= ad_src_elemsize;
      for (i=0; i< mv_rank; i++) {
	src_stride[i] = ad_src_stride[i+1]*src_slc->stride[i+1];
      }
      for (i=0; i< mv_rank; i++) {
	dst_stride[i] = ad_dst->stride[i+1]*dst_slc->stride[i+1];
      }
    } else {
      mv_rank = ad_src_rank;
      /* Shift counts, right, set count[0] to elemsize. */
      for (i=mv_rank;i>0;i--) {
	src_count[i] = src_count[i-1];
      }
      src_count[0] = ad_src_elemsize;
      /* Adjust strides. */
      for (i=0;i<mv_rank;i++) {
	src_stride[i] = ad_src_stride[i]*src_slc->stride[i];
      }
      for (i=0;i<mv_rank;i++) {
	dst_stride[i] = ad_dst->stride[i]*dst_slc->stride[i];
      }
    }
  }
#if 0
  printf(" ARMCI_Put_farrays(src): lo = %d hi = %d stride = %d\n",
         src_slc->lo[0], src_slc->hi[0], src_slc->stride[0]);
  printf(" ARMCI_Put_farrays(dst): lo = %d hi = %d stride = %d on proc = %d\n",
         dst_slc->lo[0], dst_slc->hi[0], dst_slc->stride[0], *proc);
#endif
  /*
  ARMCI_PutS(ptr_src, ad_src->stride+1, ptr_dst, ad_dst->stride+1, count, ad_src->rank-1, *proc); 
  */
  ARMCI_PutS(ptr_src, src_stride, ptr_dst, dst_stride, src_count, mv_rank, *proc); 
}

ARMCI_NbPut_farrays(void* dv_src, array_slice_t* src_slc,
                  void* dv_dst, array_slice_t* dst_slc,
                  int* proc, int* rank, int* rc,
                  void* dv_h_src, void* dv_h_dst)
{
  void *addr_src, *addr_dst, *dv_l_src, *dv_l_dst;
  armci_arr_dsc_t *ad_dst,*ad_src;
  void *ptr_src, *ptr_dst;
  int i;
  int src_count[MAXDIM+1];
  int dst_count[MAXDIM+1];
  int src_stride[MAXDIM];
  int dst_stride[MAXDIM];
  int src_unit_stride, src_first_stride_eq_1;
  int dst_unit_stride, dst_first_stride_eq_1;
  int mv_rank;
  int ad_src_rank;
  int ad_src_elemsize;
  int ad_src_lo[MAXDIM];
  int ad_src_extent[MAXDIM];
  int ad_src_stride[MAXDIM];
  *rc = 0;

  dv_l_src = cc.createArrayDesc(dv_src, dv_h_src, *rank, F90_ArrayPointer);
  if (dv_l_src == 0) {
    fprintf(stderr, "nb_ARMCI_Put_farrays: ERROR in createArrayDesc\n");
    *rc = -1;
    return;
  }
  addr_src = cc.getArrayBaseAddress(dv_l_src, *rank);
  if (addr_src == 0) {
    fprintf(stderr, "%d:nb_ARMCI_Put_farrays: ERROR in getArrayBaseAddress src %d\n",armci_me,*rank);
    *rc = -1;
    return;
  }

  dv_l_dst = cc.createArrayDesc(dv_dst, dv_h_dst, *rank, F90_ArrayPointer);
  if (dv_l_dst == 0) {
    fprintf(stderr, "nb_ARMCI_Put_farrays: ERROR in createArrayDesc\n");
    *rc = -1;
    return;
  }
  addr_dst = cc.getArrayBaseAddress(dv_l_dst, *rank);
  if (addr_dst == 0) {
    fprintf(stderr, "%d:nb_ARMCI_Put_farrays: ERROR in getArrayBaseAddress dst %d\n",armci_me,*rank);
    *rc = -1;
    return;
  }

#if 0
  assert(addr_src = g_save_addr);
  assert(addr_dst = g_save_addr);
#endif
  /*
   ad_src = GET_ARR_DSC_FROM_ARRAY(addr_src);
   Need to replace ad_src_rank, ad_src_lo, ad_src_extent, ad_src_stride,
   and ad_src_elemsize with local stuff from the chasm interface.
   No way to get elemsize from chasm stuff, so for
   now take src_elemsize to be the same as dst_elemsize.
   */

  ad_dst = GET_ARR_DSC_FROM_ARRAY(addr_dst);
  ad_src_elemsize = ad_dst->elemsize;
  ad_src_rank = ad_dst->rank;

  for (i=0;i<ad_src_rank;i++) {
    ad_src_lo[i] = cc.getArrayLowerBound(dv_l_src, ad_src_rank, i+1);
    ad_src_extent[i] = cc.getArrayExtent(dv_l_src,ad_src_rank, i+1);
    ad_src_stride[i] = cc.getArrayStrideMult(dv_l_src,ad_src_rank,i+1);
  }

  /*
  fprintf(stdout," ad_src_rank = %d %d\n",ad_src_rank,ad_src_elemsize);
  for (i=0;i<ad_src_rank;i++) {
  fprintf(stdout," ad_src_lo[%d] = %d\n",i,ad_src_lo[i]);
  }
  for (i=0;i<ad_src_rank;i++) {
  fprintf(stdout," ad_src_extent[%d] = %d\n",i,ad_src_extent[i]);
  }
  for (i=0;i<ad_src_rank;i++) {
  fprintf(stdout," ad_src_stride[%d] = %d\n",i,ad_src_stride[i]);
  }
  */

  ptr_src= Index(ad_src_rank, src_slc->lo, ad_src_lo, ad_src_extent)*ad_src_elemsize + (char*) addr_src;
  ptr_dst= Index(ad_dst->rank, dst_slc->lo, ad_dst->lo, ad_dst->extent)*ad_dst->elemsize + (char*) ad_dst->aptr[*proc];

  /* The following attempts to deal with non-unit stride. */
  /* Seems like we assume that src_slc->hi[i] >= src_slc->lo[i] and
     stride[i] > 0 for all i. One might wish to modify that
     assumption at some point. Should we check that the patches
     are the same size, does ARMCI_Puts handle that?
  */
  if (ad_dst->rank != ad_src_rank) {
    fprintf(stderr, "nb_ARMCI_Put_farrays: ERROR ranks of src and dst do not match\n");
    *rc = -1;
    return;
  }
  src_first_stride_eq_1 = 1;
  src_unit_stride = 1;
  for(i=0; i< ad_src_rank;  i++) {
    if (src_slc->stride[i] > 1) {
      if (i == 0) {
        src_first_stride_eq_1 = 0;
      }
      src_unit_stride = 0;
      src_count[i] =   (int)((src_slc->hi[i] -src_slc->lo[i]+src_slc->stride[i])/src_slc->stride[i]);
    } else {
      src_count[i] =   src_slc->hi[i] -src_slc->lo[i]+1;
    }
  }
  dst_first_stride_eq_1 = 1;
  dst_unit_stride = 1;
  for(i=0; i< ad_dst->rank;  i++) {
    if (dst_slc->stride[i] > 1) {
      if (i == 0) {
        dst_first_stride_eq_1 = 0;
      }
      dst_unit_stride = 0;
      dst_count[i] =   (int)((dst_slc->hi[i] -dst_slc->lo[i]+dst_slc->stride[i])/dst_slc->stride[i]);
    } else {
      dst_count[i] =   dst_slc->hi[i] -dst_slc->lo[i]+1;
    }
  }
  /* Might want to check that src_count and dst_count match here?,
   *      perhaps that happens in ARMC_PutS but I think not.
   *        */
  for (i=0;i<ad_src_rank;i++) {
    if (dst_count[i] != src_count[i]) {
       fprintf(stderr, "nb_ARMCI_Put_farrays: ERROR counts of src and dst slices do not match\n");
       *rc = -1;
       return;
    }
  }
  if ((src_unit_stride != 0) && (dst_unit_stride != 0)) {
    mv_rank = ad_src_rank-1;
    for (i=0; i<mv_rank; i++) {
      src_stride[i] = ad_src_stride[i+1];
    }
    for (i=0; i<mv_rank; i++) {
      dst_stride[i] = ad_dst->stride[i+1];
    }
    src_count[0] *= ad_src_elemsize;
  } else {
    /* If first dimension strides are both 1, then we
     *        can till use ad_src->rank-1 levels. Just
     *               the strides need changing.
     *                   */

    if ((src_first_stride_eq_1 != 0) && (dst_first_stride_eq_1 != 0)) {
      mv_rank = ad_src_rank-1;
      src_count[0] *= ad_src_elemsize;
      for (i=0; i< mv_rank; i++) {
        src_stride[i] = ad_src_stride[i+1]*src_slc->stride[i+1];
      }
      for (i=0; i< mv_rank; i++) {
        dst_stride[i] = ad_dst->stride[i+1]*dst_slc->stride[i+1];
      }
    } else {
      mv_rank = ad_src_rank;
      /* Shift counts, right, set count[0] to elemsize. */
      for (i=mv_rank;i>0;i--) {
        src_count[i] = src_count[i-1];
      }
      src_count[0] = ad_src_elemsize;
      /* Adjust strides. */
      for (i=0;i<mv_rank;i++) {
        src_stride[i] = ad_src_stride[i]*src_slc->stride[i];
      }
      for (i=0;i<mv_rank;i++) {
        dst_stride[i] = ad_dst->stride[i]*dst_slc->stride[i];
      }
    }
  }
#if 0
  printf(" nb_ARMCI_Put_farrays(src): lo = %d hi = %d stride = %d\n",
         src_slc->lo[0], src_slc->hi[0], src_slc->stride[0]);
  printf(" nb_ARMCI_Put_farrays(dst): lo = %d hi = %d stride = %d on proc = %d\n",
         dst_slc->lo[0], dst_slc->hi[0], dst_slc->stride[0], *proc);
#endif
  /*
   *   ARMCI_PutS(ptr_src, ad_src->stride+1, ptr_dst, ad_dst->stride+1, count, ad_src->rank-1, *proc);
   *     */
  ARMCI_NbPutS(ptr_src, src_stride, ptr_dst, dst_stride, src_count, mv_rank, *proc,NULL);

}



void ARMCI_Get_farrays(void* dv_src, array_slice_t* src_slc,
                  void* dv_dst, array_slice_t* dst_slc,
                  int* proc, int* rank, int* rc,
                  void* dv_h_src, void* dv_h_dst)
{
  void *addr_src, *addr_dst, *dv_l_src, *dv_l_dst;
  armci_arr_dsc_t *ad_src, *ad_dst;
  void *ptr_src, *ptr_dst;
  int i;
  int src_count[MAXDIM+1];
  int dst_count[MAXDIM+1];
  int src_stride[MAXDIM];
  int dst_stride[MAXDIM];
  int src_unit_stride, src_first_stride_eq_1;
  int dst_unit_stride, dst_first_stride_eq_1;
  int mv_rank;
  int ad_dst_rank;
  int ad_dst_elemsize;
  int ad_dst_lo[MAXDIM];
  int ad_dst_extent[MAXDIM];
  int ad_dst_stride[MAXDIM];
  *rc = 0;

#ifdef HIDDEN_DESC
  dv_l_src = cc.createArrayDesc(dv_src, dv_h_src, *rank, F90_ArrayPointer);
#else
  dv_l_src = dv_src;
#endif
  if (dv_l_src == 0) {
    fprintf(stderr, "ARMCI_Get_farrays: ERROR in createArrayDesc\n");
    *rc = -1;
    return;
  }
  addr_src = cc.getArrayBaseAddress(dv_l_src, *rank);
  if (addr_src == 0) {
    fprintf(stderr, "ARMCI_Get_farrays: ERROR in getArrayBaseAddress\n");
    *rc = -1;
    return;
  }

#ifdef HIDDEN_DESC
  dv_l_dst = cc.createArrayDesc(dv_dst, dv_h_dst, *rank, F90_ArrayPointer);
#else
  dv_l_dst = dv_dst;
#endif
  if (dv_l_dst == 0) {
    fprintf(stderr, "ARMCI_Get_farrays: ERROR in createArrayDesc\n");
    *rc = -1;
    return;
  }
  addr_dst = cc.getArrayBaseAddress(dv_l_dst, *rank);
  if (addr_dst == 0) {
    fprintf(stderr, "ARMCI_Get_farrays: ERROR in getArrayBaseAddress\n");
    *rc = -1;
    return;
  }
#if 0
  assert(addr_src = g_save_addr);
  assert(addr_dst = g_save_addr);
#endif

  ad_src = GET_ARR_DSC_FROM_ARRAY(addr_src);
  /*
  ad_dst = GET_ARR_DSC_FROM_ARRAY(addr_dst);
  */
  ad_dst_elemsize = ad_src->elemsize;
  ad_dst_rank     = ad_src->rank;
  for (i=0;i<ad_dst_rank;i++) {
    ad_dst_lo[i] = cc.getArrayLowerBound(dv_l_dst, ad_dst_rank, i+1);
    ad_dst_extent[i] = cc.getArrayExtent(dv_l_dst,ad_dst_rank, i+1);
    ad_dst_stride[i] = cc.getArrayStrideMult(dv_l_dst,ad_dst_rank,i+1);
  }
  /*
  fprintf(stdout," ad_dst_rank = %d\n",ad_dst_rank);
  for (i=0;i<ad_dst_rank;i++) {
    fprintf(stdout," ad_dst_lo[%d] = %d\n",i,ad_dst_lo[i]);
  }
  for (i=0;i<ad_dst_rank;i++) {
    fprintf(stdout," ad_dst_extent[%d] = %d\n",i,ad_dst_extent[i]);
  }
  for (i=0;i<ad_dst_rank;i++) {
    fprintf(stdout," ad_dst_stride[%d] = %d\n",i,ad_dst_stride[i]);
  }
  */
  /* The following two lines are the only ones that change besides the
     ARMCI_GetS line from the ARMCI_Put_farrays routine.
  */

  ptr_src= Index(ad_src->rank, src_slc->lo, ad_src->lo, ad_src->extent)*ad_src->elemsize + (char*) ad_src->aptr[*proc]; 
  ptr_dst= Index(ad_dst_rank, dst_slc->lo, ad_dst_lo, ad_dst_extent)*ad_dst_elemsize + (char*) addr_dst;
  /* The following attempts to deal with non-unit stride. */
  /* Seems like we assume that src_slc->hi[i] >= src_slc->lo[i] and 
     stride[i] > 0 for all i. One might wish to modify that 
     assumption at some point. Should we check that the patches
     are the same size, does ARMCI_Gets handle that?
  */
  if (ad_dst_rank != ad_src->rank) {
    fprintf(stderr, "ARMCI_Get_farrays: ERROR ranks of src and dst do not match\n");
    *rc = -1;
    return;
  }
  src_first_stride_eq_1 = 1;
  src_unit_stride = 1;
  for(i=0; i< ad_src->rank;  i++) {
    if (src_slc->stride[i] > 1) {
      if (i == 0) {
	src_first_stride_eq_1 = 0;
      }
      src_unit_stride = 0;
      src_count[i] =   (int)((src_slc->hi[i] -src_slc->lo[i]+src_slc->stride[i])/src_slc->stride[i]);
    } else {
      src_count[i] =   src_slc->hi[i] -src_slc->lo[i]+1;
    }
  }
  dst_first_stride_eq_1 = 1;
  dst_unit_stride = 1;
  for(i=0; i< ad_dst_rank;  i++) {
    if (dst_slc->stride[i] > 1) {
      if (i == 0) {
	dst_first_stride_eq_1 = 0;
      }
      dst_unit_stride = 0;
      dst_count[i] =   (int)((dst_slc->hi[i] -dst_slc->lo[i]+dst_slc->stride[i])/dst_slc->stride[i]);
    } else {
      dst_count[i] =   dst_slc->hi[i] -dst_slc->lo[i]+1;
    }
  }
  /* Might want to check that src_count and dst_count match here?,
     perhaps that happens in ARMC_PutS but I think not. 
  */
  for (i=0;i<ad_src->rank;i++) {
    if (dst_count[i] != src_count[i]) {
       fprintf(stderr, "ARMCI_Get_farrays: ERROR counts of src and dst slices do not match\n");
       *rc = -1;
       return;
    }
  }
      
  if ((src_unit_stride != 0) && (dst_unit_stride != 0)) {
    mv_rank = ad_src->rank-1;
    for (i=0; i<mv_rank; i++) {
      src_stride[i] = ad_src->stride[i+1];
    }
    for (i=0; i<mv_rank; i++) {
      dst_stride[i] = ad_dst_stride[i+1];
    }
    src_count[0] *= ad_src->elemsize;
  } else {
    /* If first dimension strides are both 1, then we
       can till use ad_src->rank-1 levels. Just 
       the strides need changing.
    */
    
    if ((src_first_stride_eq_1 != 0) && (dst_first_stride_eq_1 != 0)) {
      mv_rank = ad_src->rank-1;
      src_count[0] *= ad_src->elemsize;
      for (i=0; i< mv_rank; i++) {
	src_stride[i] = ad_src->stride[i+1]*src_slc->stride[i+1];
      }
      for (i=0; i< mv_rank; i++) {
	dst_stride[i] = ad_dst_stride[i+1]*dst_slc->stride[i+1];
      }
    } else {
      mv_rank = ad_src->rank;
      /* Shift counts, right, set count[0] to elemsize. */
      for (i=mv_rank;i>0;i--) {
	src_count[i] = src_count[i-1];
      }
      src_count[0] = ad_src->elemsize;
      /* Adjust strides. */
      for (i=0;i<mv_rank;i++) {
	src_stride[i] = ad_src->stride[i]*src_slc->stride[i];
      }
      for (i=0;i<mv_rank;i++) {
	dst_stride[i] = ad_dst_stride[i]*dst_slc->stride[i];
      }
    }
  }
#if 0
  printf(" ARMCI_Get_farrays(src): lo = %d hi = %d stride = %d\n",
         src_slc->lo[0], src_slc->hi[0], src_slc->stride[0]);
  printf(" ARMCI_Get_farrays(dst): lo = %d hi = %d stride = %d on proc = %d\n",
         dst_slc->lo[0], dst_slc->hi[0], dst_slc->stride[0], *proc);
#endif
  /*
  ARMCI_GetS(ptr_src, ad_src->stride+1, ptr_dst, ad_dst->stride+1, count, ad_src->rank-1, *proc); 
  */
  ARMCI_GetS(ptr_src, src_stride, ptr_dst, dst_stride, src_count, mv_rank, *proc); 
	     
}
void ARMCI_NbGet_farrays(void* dv_src, array_slice_t* src_slc,
                  void* dv_dst, array_slice_t* dst_slc,
                  int* proc, int* rank, int* rc,
                  void* dv_h_src, void* dv_h_dst)
{
  void *addr_src, *addr_dst, *dv_l_src, *dv_l_dst;
  armci_arr_dsc_t *ad_src, *ad_dst;
  void *ptr_src, *ptr_dst;
  int i;
  int src_count[MAXDIM+1];
  int dst_count[MAXDIM+1];
  int src_stride[MAXDIM];
  int dst_stride[MAXDIM];
  int src_unit_stride, src_first_stride_eq_1;
  int dst_unit_stride, dst_first_stride_eq_1;
  int mv_rank;
  int ad_dst_rank;
  int ad_dst_elemsize;
  int ad_dst_lo[MAXDIM];
  int ad_dst_extent[MAXDIM];
  int ad_dst_stride[MAXDIM];
  *rc = 0;
  dv_l_src = cc.createArrayDesc(dv_src, dv_h_src, *rank, F90_ArrayPointer);
  if (dv_l_src == 0) {
    fprintf(stderr, "ARMCI_Get_farrays: ERROR in createArrayDesc\n");
    *rc = -1;
    return;
  }
  addr_src = cc.getArrayBaseAddress(dv_l_src, *rank);
  if (addr_src == 0) {
    fprintf(stderr, "ARMCI_Get_farrays: ERROR in getArrayBaseAddress\n");
    *rc = -1;
    return;
  }
#if 0
  assert(addr_src = g_save_addr);
  assert(addr_dst = g_save_addr);
#endif

  ad_src = GET_ARR_DSC_FROM_ARRAY(addr_src);
  /*
   *   ad_dst = GET_ARR_DSC_FROM_ARRAY(addr_dst);
   *     */
  ad_dst_elemsize = ad_src->elemsize;
  ad_dst_rank     = ad_src->rank;
  for (i=0;i<ad_dst_rank;i++) {
    ad_dst_lo[i] = cc.getArrayLowerBound(dv_l_dst, ad_dst_rank, i+1);
    ad_dst_extent[i] = cc.getArrayExtent(dv_l_dst,ad_dst_rank, i+1);
    ad_dst_stride[i] = cc.getArrayStrideMult(dv_l_dst,ad_dst_rank,i+1);
  }
  /*
   fprintf(stdout," ad_dst_rank = %d\n",ad_dst_rank);
   for (i=0;i<ad_dst_rank;i++) {
   fprintf(stdout," ad_dst_lo[%d] = %d\n",i,ad_dst_lo[i]);
   }
   for (i=0;i<ad_dst_rank;i++) {
   fprintf(stdout," ad_dst_extent[%d] = %d\n",i,ad_dst_extent[i]);
   }
   for (i=0;i<ad_dst_rank;i++) {
   fprintf(stdout," ad_dst_stride[%d] = %d\n",i,ad_dst_stride[i]);
   }
   */
  /* The following two lines are the only ones that change besides the
     ARMCI_GetS line from the ARMCI_Put_farrays routine.
  */
  ptr_src= Index(ad_src->rank, src_slc->lo, ad_src->lo, ad_src->extent)*ad_src->elemsize + (char*) ad_src->aptr[*proc];
  ptr_dst= Index(ad_dst_rank, dst_slc->lo, ad_dst_lo, ad_dst_extent)*ad_dst_elemsize + (char*) addr_dst;
  /* The following attempts to deal with non-unit stride. */
  /* Seems like we assume that src_slc->hi[i] >= src_slc->lo[i] and
   *      stride[i] > 0 for all i. One might wish to modify that
   *           assumption at some point. Should we check that the patches
   *                are the same size, does ARMCI_Gets handle that?
   *                  */
  if (ad_dst_rank != ad_src->rank) {
    fprintf(stderr, "ARMCI_Get_farrays: ERROR ranks of src and dst do not match\n");
    *rc = -1;
    return;
  }
  src_first_stride_eq_1 = 1;
  src_unit_stride = 1;
  for(i=0; i< ad_src->rank;  i++) {
    if (src_slc->stride[i] > 1) {
      if (i == 0) {
        src_first_stride_eq_1 = 0;
      }
      src_unit_stride = 0;
      src_count[i] =   (int)((src_slc->hi[i] -src_slc->lo[i]+src_slc->stride[i])/src_slc->stride[i]);
    } else {
      src_count[i] =   src_slc->hi[i] -src_slc->lo[i]+1;
    }
  }
  dst_first_stride_eq_1 = 1;
  dst_unit_stride = 1;
  for(i=0; i< ad_dst_rank;  i++) {
    if (dst_slc->stride[i] > 1) {
      if (i == 0) {
        dst_first_stride_eq_1 = 0;
      }
      dst_unit_stride = 0;
      dst_count[i] =   (int)((dst_slc->hi[i] -dst_slc->lo[i]+dst_slc->stride[i])/dst_slc->stride[i]);
    } else {
      dst_count[i] =   dst_slc->hi[i] -dst_slc->lo[i]+1;
    }
  }
  /* Might want to check that src_count and dst_count match here?,
   *      perhaps that happens in ARMC_PutS but I think not.
   *        */
  for (i=0;i<ad_src->rank;i++) {
    if (dst_count[i] != src_count[i]) {
       fprintf(stderr, "ARMCI_Get_farrays: ERROR counts of src and dst slices do not match\n");
       *rc = -1;
       return;
    }
  }

  if ((src_unit_stride != 0) && (dst_unit_stride != 0)) {
    mv_rank = ad_src->rank-1;
    for (i=0; i<mv_rank; i++) {
      src_stride[i] = ad_src->stride[i+1];
    }
    for (i=0; i<mv_rank; i++) {
      dst_stride[i] = ad_dst_stride[i+1];
    }
    src_count[0] *= ad_src->elemsize;
  } else {
    /* If first dimension strides are both 1, then we
       can till use ad_src->rank-1 levels. Just
       the strides need changing.
    */
    if ((src_first_stride_eq_1 != 0) && (dst_first_stride_eq_1 != 0)) {
      mv_rank = ad_src->rank-1;
      src_count[0] *= ad_src->elemsize;
      for (i=0; i< mv_rank; i++) {
        src_stride[i] = ad_src->stride[i+1]*src_slc->stride[i+1];
      }
      for (i=0; i< mv_rank; i++) {
        dst_stride[i] = ad_dst_stride[i+1]*dst_slc->stride[i+1];
      }
    } else {
      mv_rank = ad_src->rank;
      /* Shift counts, right, set count[0] to elemsize. */
      for (i=mv_rank;i>0;i--) {
        src_count[i] = src_count[i-1];
      }
      src_count[0] = ad_src->elemsize;
      /* Adjust strides. */
      for (i=0;i<mv_rank;i++) {
        src_stride[i] = ad_src->stride[i]*src_slc->stride[i];
      }
      for (i=0;i<mv_rank;i++) {
        dst_stride[i] = ad_dst_stride[i]*dst_slc->stride[i];
      }
    }
  }
#if 0
  printf(" ARMCI_Get_farrays(src): lo = %d hi = %d stride = %d\n",
         src_slc->lo[0], src_slc->hi[0], src_slc->stride[0]);
  printf(" ARMCI_Get_farrays(dst): lo = %d hi = %d stride = %d on proc = %d\n",
         dst_slc->lo[0], dst_slc->hi[0], dst_slc->stride[0], *proc);
#endif
  /*
   *   ARMCI_GetS(ptr_src, ad_src->stride+1, ptr_dst, ad_dst->stride+1, count, ad_src->rank-1, *proc);
  */
  ARMCI_NbGetS(ptr_src, src_stride, ptr_dst, dst_stride, src_count, mv_rank, *proc,NULL);

}


/*******************************************************************************************************
 * This is a language (python,f90) independent layer of array management operations
 * Oerations defined here are called from language-specific layer to allocate & free memory
 *
 *******************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <armci.h>
#include <arraydesc.h>

extern int armci_nproc, armci_me;

/*\ allocates memory for array according to rank an lower/upper bounds
 *  pptr contains address to user visible array data
 *  lb can be specified as NULL - in that case ub contains shape
\*/
void _array_create(void **pptr, int elemsize, int rank,  index_size_t *lb, index_size_t *ub)
{
  void **tmp;
  char *hptr;
  armci_arr_dsc_t *ad;
  void *gtemp;
  int* ia, i;
  int bytes=0, elems=1, apsize;

  *pptr = (void*)0;

  if(rank <1 || rank > MAXDIM) {fprintf(stderr,"%d: bad rank %d\n",armci_me,rank); return;}

  if(lb)
     for(i=0; i< rank; i++) elems *=  1 + ub[i] - lb[i];
  else
     for(i=0; i< rank; i++) elems *=  ub[i];

  /* allocate pointer vector */
  apsize = armci_nproc *sizeof(void*);
  tmp  = (void*) malloc(apsize);
  if(!tmp) {fprintf(stderr,"%: malloc failed\n",armci_me); return;}
  bytes += apsize;

  /* allocate header */
  bytes += HEADER_SIZE;

  /* allocate array memory */
  bytes += elemsize * elems;
  
  if( ARMCI_Malloc(tmp,bytes))
    { fprintf(stderr,"%: armci_malloc failed\n",armci_me); return;}
  /* set up the header */
  hptr = ((char*)tmp[armci_me])+apsize;

  gtemp = ((char*)hptr)+HEADER_SIZE;
#if DEBUG_
  printf("%d: array memory %p tmp=%p\n", armci_me, gtemp,tmp[armci_me]); fflush(stdout);
#endif
  /* store array descriptor */
  ad = (armci_arr_dsc_t*) hptr;
  /*
  */
  ad->aptr = tmp[armci_me]; /* store ptr to array of ptrs */
  ad->elemsize = elemsize;
  ad->amem = gtemp;
  ad->rank = rank;

  if(lb){
    for(i=0; i< rank; i++) ad->lo[i] = lb[i];
    for(i=0; i< rank; i++) ad->hi[i] = ub[i];
    for(i=0; i< rank; i++) ad->extent[i] = 1 + ub[i] - lb[i];
  }else {
    for(i=0; i< rank; i++) ad->lo[i] = 0; /* C default for array lower bound */
    for(i=0; i< rank; i++) ad->hi[i] = ub[i]-1; /* C default for array upper bound */
    for(i=0; i< rank; i++) ad->extent[i] = ub[i];
  }

  ad->stride[0]=elemsize;
  for(i=1; i< rank; i++) ad->stride[i] = ad->stride[i-1]*ad->extent[i-1];

  /* adjust ptr array to point to the user f90 array rather than allocated memory */
  for(i = 0; i<armci_nproc; i++)
      tmp[i] = ((char*)tmp[i]) + HEADER_SIZE+ apsize;

  /* store descriptors in the allocated memory */
  bcopy(tmp,ad->aptr,apsize);
  *pptr = gtemp;
  free(tmp);
} 


/*\ deallocates array corresponding to ptr
\*/
void _array_free(void *ptr)
{
armci_arr_dsc_t *ad;
  ad = GET_ARR_DSC_FROM_ARRAY(ptr);
  if(ptr != ad->amem)ARMCI_Error("free - bad memory\n",0);
  ARMCI_Free(ad->aptr);
#if DEBUG_
  printf("%d: freed %p\n",armci_me, addr); fflush(stdout);
#endif
}

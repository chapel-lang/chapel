#ifndef __ARMCIARR__
#define __ARMCIARR__

#define MAXDIM 7
typedef struct {
  int rank;
  int elemsize;
  int extent[MAXDIM];
  int lo[MAXDIM];
  int hi[MAXDIM];
  int  stride[MAXDIM];
  void **aptr;
  void *amem;
}armci_arr_dsc_t;


#define HEADER_SIZE 256 

#define GET_PTR_ARR_FROM_ARRAY(ptr) (((char*)ptr)-HEADER_SIZE -armci_nproc*sizeof(void*))
#define GET_ARR_DSC_FROM_ARRAY(ptr) (armci_arr_dsc_t*)(((char*)ptr)-HEADER_SIZE)

typedef int index_size_t;

extern void _array_create(void **pptr, int elemsize, int rank,  index_size_t *lb, index_size_t *ub);
extern void _array_free(void *ptr);


#endif

/* $Id: shmalloc.c,v 1.10.12.1 2007/08/03 19:38:22 manoj Exp $ */
#include <stdio.h>
#include <string.h>
#include "armcip.h"
#include "message.h"
#include "kr_malloc.h"

static long *offset_arr;

void armci_shmalloc_exchange_offsets(context_t *ctx_local) 
{
    void **ptr_arr;
    void *ptr;
    armci_size_t bytes = 128;
    int i;    
    
    ptr_arr    = (void**)malloc(armci_nproc*sizeof(void*));
    offset_arr = (long*) malloc(armci_nproc*sizeof(long));
    if(!ptr_arr || !offset_arr) armci_die("armci_shmalloc_get_offsets: malloc failed", 0);

    /* get memory with same size on all procs */
    ptr = kr_malloc(bytes, ctx_local);
    if(!ptr) armci_die("armci_shmalloc_get_offsets: kr_malloc failed",bytes);
    
    bzero((char*)ptr_arr,armci_nproc*sizeof(void*));
    ptr_arr[armci_me] = ptr;

    /* now combine individual addresses into a single array */
    armci_exchange_address(ptr_arr, armci_nproc);
    
    /* identify offets */
    for (i=0; i<armci_nproc; i++) 
    {
       offset_arr[i] = (long) ((char*)ptr - (char*)ptr_arr[i]);
    }
       
    /* release memory */
    kr_free(ptr, ctx_local);
}

void armci_shmalloc_exchange_address(void **ptr_arr) 
{
    int i;

    /* now combine individual addresses into a single array */
    armci_exchange_address(ptr_arr, armci_nproc);

    /* since shmalloc may not give symmetric addresses (especially on Linux),
     * adjust addresses based on offset calculated during initialization */
    for (i=0; i<armci_nproc; i++) 
    {
       ptr_arr[i] = (char*)ptr_arr[i] + offset_arr[i];
    }
}

#ifdef MPI

extern int ARMCI_Absolute_id(ARMCI_Group *group,int group_rank);

/* group based exchange address */
void armci_shmalloc_exchange_address_grp(void **ptr_arr, ARMCI_Group *group) 
{
    int i, world_rank;
    int grp_nproc;

    ARMCI_Group_size(group, &grp_nproc);
    
    /* now combine individual addresses into a single array */
    armci_exchange_address_grp(ptr_arr, grp_nproc, group);

    /* since shmalloc may not give symmetric addresses (especially on Linux),
     * adjust addresses based on offset calculated during initialization */
    for (i=0; i<grp_nproc; i++) 
    {
       world_rank = ARMCI_Absolute_id(group,i);
       ptr_arr[i] = (char*)ptr_arr[i] + offset_arr[world_rank];
    }
}
#endif

/* get the remote process's pointer */
void* armci_shmalloc_remote_addr(void *ptr, int proc) 
{
    return (void*)((char*)ptr - offset_arr[proc]);
}

#if defined(XT3)

#define XT_SYMMETRIC_HEAP_SIZE ((size_t)1024)*1024*1024; /* 1 GB is default */
size_t get_xt_heapsize() 
{
    
    char *uval = getenv("XT_SYMMETRIC_HEAP_SIZE");
    char *token    = NULL;
    char *unit_str = NULL;
    size_t scale=1, size;

    if(uval != NULL) 
    {
       if((unit_str=strchr(uval, 'K')) != NULL) 
       {
          scale = 1024;
          token = strtok(uval, "K");
       }
       else if((unit_str=strchr(uval, 'M')) != NULL) 
       {
          scale = ((size_t)1024)*1024;
          token = strtok(uval, "M");
       }
       else if((unit_str=strchr(uval, 'G')) != NULL) 
       {
          scale = ((size_t)1024)*1024*1024;
          token = strtok(uval, "G");
       }
       else 
       {
          scale = 1;
          token = uval;
       }
       
       size = (size_t)atol(token);
       size *= scale; /* in bytes */

       if(size < 1024*1024)
          armci_die("get_xt_heapsize(): Symmetric heapsize should be > 1MB",0);
    }
    else 
    {
       size = XT_SYMMETRIC_HEAP_SIZE;
    }
    
    return size; 
}
#endif /* defined XT3 */


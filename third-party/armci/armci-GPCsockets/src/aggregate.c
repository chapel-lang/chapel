/** $Id: aggregate.c,v 1.6 2003-10-22 22:12:14 d3h325 Exp $
 * Aggregate Put/Get requests
 */

#include "armcip.h"
#include <string.h> /* memcpy */
#include <stdio.h>

#define _MAX_AGG_BUFFERS 32   /* Maximum # of aggregation buffers available*/
#define _MAX_AGG_BUFSIZE 2048 /* size of each buffer. should be < 2^15 */
#define _MAX_PTRS        256  /* < 2^15, as it is "short int" in agg_req_t */
#define _MAX_AGG_HANDLE  _MAX_AGG_BUFFERS /* Max # of aggregation handles */

/* aggregate request handle */
typedef struct {
  unsigned int tag;        /* non-blocking request tag */
  short int proc;          /* remote process id */
  short int request_len ;  /* number of requests */
  short int ptr_array_len; /* pointer length for this request */
  short int buf_pos_end;   /* position of buffer (from right end) */
  armci_giov_t *darr;      /* giov vectors */
}agg_req_t;
static agg_req_t *aggr[_MAX_AGG_HANDLE];  /* aggregate request handle */


/* data structure for dynamic buffer management */
typedef struct {
  int size; /* represents the size of the list (not linked list) */
  int index[_MAX_AGG_HANDLE];
} agg_list_t;
static agg_list_t ulist, alist;/*in-use & available aggr buffer index list*/


/* aggregation buffer */
static char agg_buf[_MAX_AGG_BUFFERS][_MAX_AGG_BUFSIZE];
/* aggregation buffer to store the pointers */
static void* agg_src_ptr[_MAX_AGG_BUFFERS][_MAX_PTRS]; 
static void* agg_dst_ptr[_MAX_AGG_BUFFERS][_MAX_PTRS]; 

/**
 * ---------------------------------------------------------------------
 * fill descriptor from this side (left to right)
 *            --->
 *             _______________________________________________
 *            |  |  |  |. .  .  .  .  .  .  .   .  . |  |  |  |
 *            |__|__|__|_____________________________|__|__|__|
 *	                                      
 *	                                             <---
 *	             fill src and dst pointer (arrays) in this direction
 *		     (right to left)
 *			  
 * Once they are about to cross each other (implies buffer is full), 
 * complete the data transfer.
 * ---------------------------------------------------------------------
 */

#define AGG_INIT_NB_HANDLE(op_type, p, nb_handle) \
    if(nb_handle->proc < 0) {                 \
      nb_handle->tag  = GET_NEXT_NBTAG();     \
      nb_handle->op   = op_type;              \
      nb_handle->proc = p;                    \
      nb_handle->bufid= NB_NONE;              \
    }                                         \
    else if(nb_handle->op != op_type)         \
      armci_die("ARMCI_NbXXX: AGG_INIT_NB_HANDLE(): Aggregate Failed, Invalid non-blocking handle", nb_handle->op);             \
    else if(nb_handle->proc != p)          \
      armci_die("ARMCI_NbXXX: AGG_INIT_NB_HANDLE(): Aggregate Failed, Invalid non-blocking handle", p)


/* initialize/set the fields in the buffer*/
#define _armci_agg_set_buffer(index, tag, proc, len) { \
    aggr[(index)]->tag = (tag);       \
    aggr[(index)]->proc = (proc);     \
    aggr[(index)]->request_len = (len); \
    ulist.index[ulist.size++] = (index);/* add the new index to the in-use list and increment it's size*/ \
}

/* get the index of the aggregation buffer to be used */
static int _armci_agg_get_bufferid(armci_ihdl_t nb_handle) {
    int i, index, tag = nb_handle->tag, proc = nb_handle->proc;
    
    /* check if there is an entry for this handle in the existing list*/
    for(i=ulist.size-1; i>=0; i--) {
      index = ulist.index[i];
      if(aggr[index]->tag == tag && aggr[index]->proc == proc) 
	return index;
    }
    
    /* else it is a new handle, so get a aggr buffer from either 
       of the lists. ???? don't throw exception here */
    if(ulist.size >= _MAX_AGG_BUFFERS && alist.size == 0)
      armci_die("_armci_agg_get_index: Too many outstanding aggregation requests\n", ulist.size);
    
    /*If there is a buffer in readily available list,use it*/
    if(alist.size > 0) index = alist.index[alist.size--];
    else { /* else use/get a buffer from the main list */
      index = ulist.size; 
      
      /* allocate memory for aggregate request handle */
      aggr[index] = (agg_req_t *)agg_buf[index];
      
      aggr[index]->request_len   = 0;
      aggr[index]->ptr_array_len = 0;
      aggr[index]->buf_pos_end   = _MAX_AGG_BUFSIZE;

      /* allocate memory for giov vector field in aggregate request handler */
      aggr[index]->darr = (armci_giov_t *)(agg_buf[index]+sizeof(agg_req_t));
    }
    
    _armci_agg_set_buffer(index, tag, proc, 0); 
    return index;
}

static void _armci_agg_update_lists(int index) {
    int i;
    /* remove that index from the in-use list and bring the last element 
       in the in-use list to the position of the removed one. */
    for(i=0; i<ulist.size; i++)
      if(ulist.index[i] == index) {
	ulist.index[i] = ulist.index[ulist.size-1];
	--(ulist.size);
	break;
      }
    
    /* and add the removed index to the available list and increment */
    alist.index[alist.size++] = index;
}


/* replace with macro later */
static armci_giov_t* 
_armci_agg_get_descriptor(int *ptr_array_len,int bytes,armci_ihdl_t nb_handle,
			  int is_registered_put, void **registered_put_data) {
  
    short unsigned int get_new_descr=0, bytes_needed=0, rid;
    int bytes_remaining;
    int index = _armci_agg_get_bufferid(nb_handle);
    
    rid   = aggr[index]->request_len; /* index of giov descriptor */
    bytes_remaining = aggr[index]->buf_pos_end - 
      (sizeof(agg_req_t) + aggr[index]->request_len*sizeof(armci_giov_t));
    
    /* extra bytes required to store registered put data */
    if(is_registered_put) bytes_needed = bytes; 
    
    /* if (byte-)sizes are equal, use previously created descriptor 
       else get a new descriptor */
    if( rid && bytes==aggr[index]->darr[rid-1].bytes) --rid;
    else { get_new_descr=1;  bytes_needed += sizeof(armci_giov_t); }
    
    /* If buffer is full, then complete data transfer. After completion, 
       if still ptr array_len is greater than maximum limit(_MAX_PTRS), 
       then do it by parts. Determine new ptr_array_len that fits buffer */
    if( (bytes_needed > bytes_remaining) ||
	(_MAX_PTRS - aggr[index]->ptr_array_len < *ptr_array_len)) {
      armci_agg_complete(nb_handle, SET);
      rid = 0; get_new_descr=1; 
      if(*ptr_array_len > _MAX_PTRS) *ptr_array_len = _MAX_PTRS;
    }
    
    /* if new descriptor, allocate memory for src_ptr & dst_ptr arrays */
    if(get_new_descr) { 
      int i = aggr[index]->ptr_array_len;
      aggr[index]->darr[rid].src_ptr_array = (void **)&agg_src_ptr[index][i];
      aggr[index]->darr[rid].dst_ptr_array = (void **)&agg_dst_ptr[index][i];
      aggr[index]->darr[rid].ptr_array_len = 0;
      aggr[index]->request_len++;    
    }

    /* store registered put data */
    if(is_registered_put) {
      aggr[index]->buf_pos_end -= bytes;
      memcpy(&((char *)aggr[index])[aggr[index]->buf_pos_end], 
	     *((char **)registered_put_data), bytes);
      *(char **)registered_put_data = (char *)&((char *)aggr[index])[aggr[index]->buf_pos_end];
    }

    aggr[index]->ptr_array_len += *ptr_array_len;
    return (&aggr[index]->darr[rid]);
}

int armci_agg_save_descriptor(void *src, void *dst, int bytes, int proc, int op,
			      int is_registered_put, armci_ihdl_t nb_handle) {

    int one=1, idx;
    armci_giov_t * darr;

    /* set up the handle if it is a new aggregation request */
    AGG_INIT_NB_HANDLE(op, proc, nb_handle);
    
    darr = _armci_agg_get_descriptor(&one, bytes, nb_handle, 
				     is_registered_put, &src);
    idx = darr->ptr_array_len;

    darr->src_ptr_array[idx] = src;
    darr->dst_ptr_array[idx] = dst;
    darr->bytes              = bytes;
    darr->ptr_array_len     += 1;

    fflush(stdout);
    return 0;
}


int armci_agg_save_giov_descriptor(armci_giov_t dscr[], int len, int proc, 
				   int op, armci_ihdl_t nb_handle) {  
    int i, j, k, idx, bytes, ptr_array_len;
    armci_giov_t * darr;

    /* set up the handle if it is a new aggregation request */
    AGG_INIT_NB_HANDLE(op, proc, nb_handle);

    for(i=0; i<len; i++) {
      k = 0;
      bytes         = dscr[i].bytes;
      ptr_array_len = dscr[i].ptr_array_len;
      do {
	darr=_armci_agg_get_descriptor(&ptr_array_len,bytes,nb_handle,0,0);
	idx = darr->ptr_array_len;
	
	for(j=idx; j<idx+ptr_array_len; j++, k++) {
	  darr->src_ptr_array[j] = dscr[i].src_ptr_array[k];
	  darr->dst_ptr_array[j] = dscr[i].dst_ptr_array[k];
	}
	darr->bytes          = dscr[i].bytes;
	darr->ptr_array_len += ptr_array_len;

	ptr_array_len = dscr[i].ptr_array_len - ptr_array_len;
	if(ptr_array_len <0) armci_die("agg_save_giov_descr failed", 0L);
      } while(k < darr[i].ptr_array_len);
    }
    return 0;
}

int armci_agg_save_strided_descriptor(void *src_ptr, int src_stride_arr[], 
				      void* dst_ptr, int dst_stride_arr[], 
				      int count[], int stride_levels, int proc,
				      int op, armci_ihdl_t nb_handle) {  

    int i, j, k, idx, ptr_array_len=1, total1D=1, num1D=0;
    int offset1, offset2, factor[MAX_STRIDE_LEVEL];
    armci_giov_t * darr;

    /* set up the handle if it is a new aggregation request */
    AGG_INIT_NB_HANDLE(op, proc, nb_handle);

    for(i=1; i<=stride_levels; i++) {
      total1D *= count[i]; 
      factor[i-1]=0;   
    }      
    ptr_array_len = total1D;

    do {
      darr=_armci_agg_get_descriptor(&ptr_array_len,count[0],nb_handle,0,0);
      idx = darr->ptr_array_len;
  
      /* converting stride into giov vector */
      for(i=idx; i<idx+ptr_array_len; i++) {
	for(j=0, offset1=0, offset2=0; j<stride_levels; j++) {
	  offset1 += src_stride_arr[j]*factor[j];
	  offset2 += dst_stride_arr[j]*factor[j];
	}
	darr->src_ptr_array[i] = (char *)src_ptr + offset1;
	darr->dst_ptr_array[i] = (char *)dst_ptr + offset2;
	++factor[0];
	++num1D;
	for(j=1; j<stride_levels; j++)
	  if(num1D%count[j]==0) { 
	    ++factor[j];
	    for(k=0; k<j;k++) factor[k]=0;
	  }
      }

      darr->bytes          = count[0];
      darr->ptr_array_len += ptr_array_len;      
      ptr_array_len = total1D - ptr_array_len;
      if(ptr_array_len <0) armci_die("agg_save_strided_descr failed", 0L);
    } while(num1D < total1D);

    return 0;
}


void armci_agg_complete(armci_ihdl_t nb_handle, int condition) {
    int i, index=0, rc;

    /* get the buffer index for this handle */
    for(i=ulist.size-1; i>=0; i--) {
      index = ulist.index[i];
      if(aggr[index]->tag == nb_handle->tag && 
	 aggr[index]->proc == nb_handle->proc)	
	break;
    }
    if(i<0) return; /* implies this handle has no requests at all */
    
#if 0
    printf("%d: Hey Buddy! Aggregation Complete to remote process %d (%d:%d requests)\n", 
	   armci_me, nb_handle->proc, index, aggr[index]->request_len);
#endif

    /* complete the data transfer. NOTE: in LAPI, Non-blocking calls 
       (followed by wait) performs better than blocking put/get */
    if(aggr[index]->request_len) {
      switch(nb_handle->op) {
#ifdef LAPI
	armci_hdl_t usr_hdl;
      case PUT:
	ARMCI_INIT_HANDLE(&usr_hdl);       
	if((rc=ARMCI_NbPutV(aggr[index]->darr, aggr[index]->request_len, 
			    nb_handle->proc, (armci_hdl_t*)&usr_hdl)))
	  ARMCI_Error("armci_agg_complete: nbputv failed",rc);
	ARMCI_Wait((armci_hdl_t*)&usr_hdl);
	break;
      case GET:
	ARMCI_INIT_HANDLE(&usr_hdl);       
	if((rc=ARMCI_NbGetV(aggr[index]->darr, aggr[index]->request_len, 
			    nb_handle->proc, (armci_hdl_t*)&usr_hdl)))
	  ARMCI_Error("armci_agg_complete: nbgetv failed",rc);  
	ARMCI_Wait((armci_hdl_t*)&usr_hdl);
	break;
#else
      case PUT:
	if((rc=ARMCI_PutV(aggr[index]->darr, aggr[index]->request_len, 
			  nb_handle->proc)))
	  ARMCI_Error("armci_agg_complete: putv failed",rc);
	break;
      case GET:
	if((rc=ARMCI_GetV(aggr[index]->darr, aggr[index]->request_len, 
			  nb_handle->proc)))
	  ARMCI_Error("armci_agg_complete: getv failed",rc);  
	break;
#endif
      }
    }
    
    /* setting request length to zero, as the requests are completed */
    aggr[index]->request_len   = 0;
    aggr[index]->ptr_array_len = 0;
    aggr[index]->buf_pos_end   = _MAX_AGG_BUFSIZE;
    
    /* If armci_agg_complete() is called ARMCI_Wait(), then unset nb_handle*/
    if(condition==UNSET) { 
      nb_handle->proc = -1;
      _armci_agg_update_lists(index);
    }
}


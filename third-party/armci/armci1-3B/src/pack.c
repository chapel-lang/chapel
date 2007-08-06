/* $Id$ */
#include "armcip.h"
#include <stdio.h>

#if !defined(ACC_COPY) &&!defined(CRAY_YMP) &&!defined(CYGNUS)&&!defined(CYGWIN) &&!defined(BGML)
#   define REMOTE_OP 
#endif

#if defined(REMOTE_OP) 
#  define OP_STRIDED armci_rem_strided
#else
#  define OP_STRIDED(_a,_b,_c,_d,_e,_f,_g,_h,_i,_delete1,_j,_hdl)\
                    armci_op_strided(_a,_b,_c,_d,_e,_f,_g,_h,_i,_j,_hdl)
#endif


/*\ determine if patch fits in the ARMCI buffer, and if not 
 *  at which stride level (patch dim) need to decompose it 
 *  *fit_level is the value of stride level to perform packing at 
 *  *nb means number of elements of count[*fit_level] that fit in buf 
\*/
static void armci_fit_buffer(int count[], int stride_levels, int* fit_level, 
                             int *nb, int bufsize)
{
   int bytes=1, sbytes=1;
   int level;

   /* find out at which stride level BUFFER becomes too small */
   for(level=0; level<= stride_levels; level++){
      sbytes = bytes; /* store #bytes at current level to save div cost later */
      bytes *= count[level];
      if(bufsize < bytes) break;
   }

   /* buffer big enough for entire patch */
   if(bufsize >= bytes){
       *fit_level = stride_levels;
       *nb = count[stride_levels];
       return;
   }

   /* buffer too small */
   switch (level){
   case 0: 
       /* smaller than a single column */
       *fit_level = 0;
       *nb = bufsize;
       break;
   case -1:   /* one column fits */
       *fit_level = 0;
       *nb = sbytes;
       break;
   default:
       /* it could keep nb instances of (level-1)-dimensional patch */
       *fit_level = level;
       *nb = bufsize/sbytes;
   }   
}


/*\ The function decomposes a multi-dimensional patch so that it fits in the
 *  internal ARMCI buffer.
 *  It works by recursively reducing patch dimension until some portion of the
 *  subpatch fits in the buffer.
 *  The recursive process is controlled by "fit_level" and "nb" arguments, 
 *  which have to be set to -1 at the top-level of the recursion tree.
 *
 *  Argument last and variable looplast are used to indicate to sending/packing
 *  routine that we are dealing with the last portion of the request.
 *  Due to the recursive nature of packing code, the algorithm is following:
 *      if last=1  then internal for loop passes 1 for the last chunk
 *      else it passes 0
 *  
\*/
int armci_pack_strided(int op, void* scale, int proc,
                       void *src_ptr, int src_stride_arr[],
                       void* dst_ptr, int dst_stride_arr[],
                       int count[], int stride_levels, ext_header_t *h,
                       int fit_level, int nb, int last,armci_ihdl_t nb_handle)
{
    int rc=0, sn, bufsize=BUFSIZE,noswap=0;
    void *src, *dst;
#ifdef REMOTE_OP
    int flag=0;
#else
    int flag=1;
#endif
    int b;
    static int call_count;

#ifdef STRIDED_GET_BUFLEN
    if(op==GET)bufsize=STRIDED_GET_BUFLEN;
#  ifdef HITACHI
    else 
	if(stride_levels || ACC(op))bufsize=MSG_BUFLEN_SMALL-PAGE_SIZE;
#  endif
#endif

#if (defined(GM_) || defined(VIA_) || defined(VAPI_))
    /*we cant assume that the entire available buffer will be used for data, 
      fact that the header and descriptor also go in the same buffer should be
      considered while packing.
    */
    bufsize-=(sizeof(request_header_t)+(MAX_STRIDE_LEVEL+4)*sizeof(int)+2*sizeof(void *));
#  if defined(PIPE_BUFSIZE) && defined(MAX_PIPELINE_CHUNKS)
    bufsize-=8*MAX_PIPELINE_CHUNKS;
#  endif
#endif

#ifdef BALANCE_FACTOR
    /* Added the following for balancing buffers */
    if(op==PUT){
        int bytes=1, i;
        for(i=0; i<= stride_levels; i++)
                bytes *= count[i];
        if(bytes > bufsize && bytes/bufsize < 3 && bytes%bufsize < BALANCE_BUFSIZE){
        /* bytes div bufsize - 1 is to increase the balence factor for 3 buffer case */
                bufsize = bytes/ (bytes/bufsize - 1 + BALANCE_FACTOR);
                noswap = 1; /*** yuck: if set to 1, error in buffers.c ***/
        }
        bytes = bufsize%8;
        bufsize -= bytes;
    }
#endif

    /* determine decomposition of the patch to fit in the buffer */
    if(fit_level<0){
       armci_fit_buffer(count, stride_levels, &fit_level, &nb, bufsize);
       last = 1;
    }

    if(fit_level == stride_levels){

        /* we can fit subpatch into the buffer */
        int chunk = count[fit_level];
        int dst_stride, src_stride;

        if(nb == chunk){ /* take shortcut when whole patch fits in the buffer */
           if(h) h->last = last?1:0;
           if(nb_handle  && call_count ){
             nb_handle->bufid=NB_MULTI;
             call_count++;
           }
           return(OP_STRIDED(op, scale, proc, src_ptr, src_stride_arr,
                  dst_ptr,dst_stride_arr,count,stride_levels,h,flag,nb_handle));
        }

        if(fit_level){
           dst_stride = dst_stride_arr[fit_level -1];
           src_stride = src_stride_arr[fit_level -1];
        }else{
           dst_stride = src_stride = 1;
        }
        if(op == GET || noswap == 1) b =nb; 
        else{ b = chunk%nb; if(b==0)b=nb; } /* put smallest piece first */

        for(sn = 0; sn < chunk; ){
           src = (char*)src_ptr + src_stride* sn;
           dst = (char*)dst_ptr + dst_stride* sn;
           count[fit_level] = MIN(b, chunk-sn); /*modify count for this level*/

           if(h) h->last = (last && ((sn+b)>=chunk))? 1: 0 ;
           if(nb_handle)call_count++;
           rc = OP_STRIDED( op, scale, proc, src, src_stride_arr,
                           dst,dst_stride_arr,count,fit_level,h,flag,nb_handle);
           if(rc) break;

           sn += b;
           b = nb;
        }
        count[fit_level] = chunk; /* restore original count */

    }
    else {
        for(sn = 0; sn < count[stride_levels]; sn++){
           int looplast =0;
           src = (char*)src_ptr + src_stride_arr[stride_levels -1]* sn;
           dst = (char*)dst_ptr + dst_stride_arr[stride_levels -1]* sn;

           if(last && (sn == count[stride_levels]-1)) looplast =1;
           rc = armci_pack_strided(op, scale, proc, src, src_stride_arr,
                                   dst, dst_stride_arr, count, stride_levels -1,
                                   h,fit_level, nb, looplast,nb_handle);
           if(rc) return rc;
        }
    }
    if(nb_handle && call_count )
       nb_handle->bufid=NB_MULTI;
    return rc;
}

/*\ decompose strided data into chunks and call func on each chunk
\*/
void armci_dispatch_strided(void *ptr, int stride_arr[], int count[],
                            int strides, int fit_level, int nb, int bufsize, 
                            void (*fun)(void*,int*,int*,int,void*), void *arg)
{
    int  sn,first_call=0;
    void *ptr_upd;

    /* determine decomposition of the patch to fit in the buffer */
    if(fit_level<0){
       first_call=1;
       armci_fit_buffer(count, strides, &fit_level, &nb, bufsize);
    }
 

    if(fit_level == strides){

        /* we can fit subpatch into the buffer */
        int chunk = count[fit_level];
        int stride_upd;
   
#       ifdef PIPE_MEDIUM_BUFSIZE_
          /* for first call we adjust nb for performance in medium request  */
          if(first_call && strides==0)
             if(chunk<2*bufsize && chunk>PIPE_MEDIUM_BUFSIZE) 
                                              nb = PIPE_MEDIUM_BUFSIZE;
#       endif

        if(nb == chunk){ /* take shortcut when whole patch fits in the buffer */
           fun(ptr, stride_arr, count, strides, arg);
        }

        if(fit_level)
           stride_upd = stride_arr[fit_level -1];
        else
           stride_upd = 1;

        for(sn = 0; sn < chunk; sn += nb){

           ptr_upd = (char*)ptr + stride_upd* sn;
           count[fit_level] = MIN(nb, chunk-sn); /*modify count for this level*/
           fun(ptr_upd, stride_arr, count, fit_level, arg);
        }
        count[fit_level] = chunk; /* restore original count */

    }else for(sn = 0; sn < count[strides]; sn++){
              ptr_upd = (char*)ptr + stride_arr[strides -1]* sn;
              armci_dispatch_strided(ptr_upd, stride_arr, count, strides -1,
                                     fit_level, nb, bufsize, fun, arg);
    }
}

/* how much space is needed to move data + reduced descriptor ? */
int armci_vector_bytes( armci_giov_t darr[], int len)
{
int i, bytes=0;
    for(i=0; i<len; i++){                                   
        /*       # elements            * (elem size     + dst address ) */
        bytes += darr[i].ptr_array_len * (darr[i].bytes + sizeof(void*));
        bytes += 2*sizeof(int); /* ptr_array_len + bytes */
    }
    return bytes;
}


#define BUFSIZE10 26000 
#define BUFSIZE1  BUFSIZE

void armci_split_dscr_array( armci_giov_t darr[], int len,
                             armci_giov_t* extra, int *nlen, armci_giov_t* save)
{
int s;
int bytes=0, split=0;

    extra->src_ptr_array=NULL;
    /* go through the sets looking for set to be split */
    for(s=0;s<len;s++){
        int csize;

        csize  = darr[s].ptr_array_len * (darr[s].bytes + sizeof(void*));
        csize += 2*sizeof(int); /* ptr_array_len + bytes */

        if(csize + bytes >BUFSIZE1){

          split =(BUFSIZE1 -bytes-2*sizeof(int))/(darr[s].bytes +sizeof(void*));
          if(split == 0) s--; /* no room available - do not split */
          break;

        }else bytes+=csize;

        if(BUFSIZE1 -bytes < 64) break; /* stop here if almost full */
    }

    if(s==len)s--; /* adjust loop counter should be < number of sets */ 
    *nlen = s+1;

    if(split){

       /* save the value to be overwritten only if "save" is not filled */ 
       if(!save->src_ptr_array)*save= darr[s];

       /* split the set: reduce # of elems, "extra" keeps info for rest of set*/
       *extra = darr[s];
       darr[s].ptr_array_len = split;
       extra->ptr_array_len -= split;
       extra->src_ptr_array  = &extra->src_ptr_array[split];
       extra->dst_ptr_array  = &extra->dst_ptr_array[split];
    }
} 
    
 

int armci_pack_vector(int op, void *scale, armci_giov_t darr[],int len,
                      int proc,armci_ihdl_t nb_handle)
{
armci_giov_t extra; /* keeps data remainder of set to be processed in chunks */
armci_giov_t save;  /* keeps original value of set to be processed in chunks */
armci_giov_t *ndarr; /* points to first array element to be processed now */
int rc=0, nlen, count=0;

    ndarr = darr;

    save.src_ptr_array=NULL; /* indicates that save slot is empty */
    while(len){

       armci_split_dscr_array(ndarr, len, &extra, &nlen, &save); 
#  if defined(REMOTE_OP) 
       rc = armci_rem_vector(op, scale, ndarr,nlen,proc,0,nb_handle);
#  else
       if(ACC(op))rc=armci_acc_vector(op,scale,ndarr,nlen,proc);
       else rc = armci_copy_vector(op,ndarr,nlen,proc);
#  endif
       if(rc) break;

       /* non-NULL pointer indicates that set was split */
       if(extra.src_ptr_array){

          ndarr[nlen-1]=extra; /* set the pointer to remainder of last set */
          nlen--; /* since last set not done in full need to process it again */

       }else{

          if(save.src_ptr_array){
             ndarr[0]=save;
             save.src_ptr_array=NULL; /* indicates that save slot is empty */
          }

          if(nlen==0)
            armci_die("vector packetization problem:buffer too small",BUFSIZE1);
       }

       len -=nlen;
       ndarr +=nlen;
       count ++;
    }

    return rc;
}

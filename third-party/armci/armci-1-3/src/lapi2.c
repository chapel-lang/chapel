/* $Id: lapi2.c,v 1.14.2.6 2007/06/01 16:28:37 vinod Exp $ */
#define DEBUG 0
#define DSCR_SIZE 4096*8  /*given that bufsize=30000*8,conservative,indeed*/

#define LAPI_CLEAR_CNTR(ocmpl_) if((ocmpl_)->val) {\
int _val_;\
    if(LAPI_Waitcntr(lapi_handle,&((ocmpl_)->cntr), ((ocmpl_)->val), &_val_))\
             armci_die("LAPI_Waitcntr failed",-1);\
    if(_val_ != 0) armci_die("CLEAR_COUNTER: nonzero in file ", _val_);\
    (ocmpl_)->val = 0;  \
} 
 

/*\ create lapi vector descriptors from the buffer
\*/
static void lapi2_create_vec_info(lapi_vec_t **srcv, lapi_vec_t **dstv, 
                          int iovnum,int iovlen,char *bufptr)
{
int dsize = iovnum*sizeof(void *);
int dlen  = iovlen*sizeof(int);
int offset=0;

    *srcv      = (lapi_vec_t *)(bufptr+offset);  offset+=sizeof(lapi_vec_t);
    *dstv      = (lapi_vec_t *)(bufptr+offset);  offset+=sizeof(lapi_vec_t);


    (*srcv)->info= (void **)(bufptr+offset);       offset+=dsize;
    (*dstv)->info= (void **)(bufptr+offset);       offset+=dsize;
    if(dlen!=0){
       (*srcv)->len = (unsigned long *)(bufptr+offset); offset+=dlen;
       (*dstv)->len = (unsigned long *)(bufptr+offset); offset+=dlen;
    }
    else {
       (*srcv)->len = (*dstv)->len = NULL;
    }
}


/*\ 2D strided get/put using lapi vector/strided transfer
\*/
void armcill_op2D(int op,void *src_ptr,int src_stride,void *dst_ptr,
                 int dst_stride,int count, int bytes, int p,
                 lapi_cntr_t *ocntr,char *bufptr)
{
lapi_vec_t *src, *dst;
int reqid,rc;
int offset=0;
    
    if(DEBUG){
       printf("\n%d:in put2d p=%d bytes=%d\n",armci_me,p,bytes);fflush(stdout);
    }

    /*lapi2_create_vec_info(&src,&dst,3,0,bufptr);*/ 
    src    = (lapi_vec_t *)(bufptr+offset);    offset+=sizeof(lapi_vec_t);
    dst    = (lapi_vec_t *)(bufptr+offset);    offset+=sizeof(lapi_vec_t);
    src->info  = (void **)(bufptr+offset);         offset+=3*sizeof(void *);
    dst->info  = (void **)(bufptr+offset);         offset+=3*sizeof(void *);


    src->vec_type = dst->vec_type                   = LAPI_GEN_STRIDED_XFER;
    src->num_vecs = (uint)count;       dst->num_vecs= (uint)count;

    src->len      = NULL;              dst->len     = NULL;
    src->info[0]  = src_ptr;           dst->info[0]  = dst_ptr;
    src->info[1]  = (void*)bytes;      dst->info[1]  = (void*)bytes;
    src->info[2]  = (void*)src_stride; dst->info[2]  = (void*)dst_stride;

    if(op==GET)
       rc = LAPI_Getv(lapi_handle, (uint)p,  src, dst,NULL,ocntr);
    else
       rc = LAPI_Putv(lapi_handle,(uint)p,dst,src,NULL,ocntr,&cmpl_arr[p].cntr);

    if(rc) armci_die2("LAPI_op2D failed",rc,op);

    if(DEBUG)printf("\n%d: put completed \n",armci_me);
}


/*\ ND strided get/put packed and sent as vectors
\*/
void armcill_opND(int op,void *src_ptr, int src_stride_arr[],void* dst_ptr,
                  int dst_stride_arr[],int count[], int stride_levels, 
                  int proc, lapi_cmpl_t *ocmpl,char *bufptr)
{
char *dst=(char*)dst_ptr;
char *src=(char*)src_ptr;
char *dst1;
char *src1;
int i,j,k,num_xmit=0,lastiovlength,iovlength,n=0,max_iovec,totalsize=0;
int total_of_2D=1;
int index[MAX_STRIDE_LEVEL], unit[MAX_STRIDE_LEVEL];
int rc,vecind;
lapi_vec_t *srcv, *dstv;
lapi_cntr_t *ocntr=&(ocmpl->cntr);
int offset=0;

    if(DEBUG){
       printf("\n%d:in getND count[0] is %d and strarr[0] is%d maxiov=%d\n",
              armci_me,count[0],dst_stride_arr[0],max_iovec);
       fflush(stdout);
    }

    index[2] = 0; unit[2] = 1;
    if(stride_levels>1){
       total_of_2D = count[2];
       for(j=3; j<=stride_levels; j++) {
         index[j] = 0; unit[j] = unit[j-1] * count[j-1];
         total_of_2D *= count[j];
       }
    }

    max_iovec=(DSCR_SIZE-2*sizeof(lapi_vec_t))/(2*(sizeof(int)+sizeof(void*)));

    /*compute number of loops and the size of last iovector based of buf size*/ 
    num_xmit = total_of_2D*count[1]/max_iovec;
    lastiovlength = (total_of_2D*count[1])%max_iovec;
    if(num_xmit == 0) num_xmit = 1;
    else if(lastiovlength!=0)num_xmit++;

    /*set the current iov length*/ 
    k=0;vecind=0;
    if(lastiovlength!=0 && k==(num_xmit-1))iovlength=lastiovlength;
    else iovlength=max_iovec;

    /*create the lapi_vec_t from the buffer*/
    /*lapi2_create_vec_info(&srcv,&dstv,iovlength,iovlength,bufptr);*/
    srcv      = (lapi_vec_t *)(bufptr+offset);  offset+=sizeof(lapi_vec_t);
    dstv      = (lapi_vec_t *)(bufptr+offset);  offset+=sizeof(lapi_vec_t);
    srcv->info= (void **)(bufptr+offset);       offset+=iovlength*sizeof(void*);
    dstv->info= (void **)(bufptr+offset);       offset+=iovlength*sizeof(void*);
    srcv->len = (unsigned long *)(bufptr+offset);offset+=iovlength*sizeof(unsigned long);
    dstv->len = (unsigned long *)(bufptr+offset);offset+=iovlength*sizeof(unsigned long);


    srcv->vec_type = dstv->vec_type             = LAPI_GEN_IOVECTOR;
    srcv->num_vecs = (uint)iovlength;  dstv->num_vecs= (uint)iovlength;

    for(i=0; i<total_of_2D; i++) {
       dst = (char *)dst_ptr;
       src = (char *)src_ptr;
       for(j=2; j<=stride_levels; j++) {
         dst += index[j] * dst_stride_arr[j-1];
         src += index[j] * src_stride_arr[j-1];
         if(((i+1) % unit[j]) == 0) index[j]++;
         if(index[j] >= count[j]) index[j] = 0;
       }
       dst1=dst;
       src1=src;
       for(j=0;j<count[1];j++,vecind++){
         if(vecind==iovlength){
           LAPI_CLEAR_CNTR((ocmpl));
           ocmpl->val+=1;
           if(op==GET){
             rc = LAPI_Getv(lapi_handle,(uint)proc,srcv,dstv,NULL,ocntr);
           }
           else {
             UPDATE_FENCE_STATE(proc,PUT,1);
             rc = LAPI_Putv(lapi_handle,(uint)proc,dstv,srcv,NULL,ocntr,
                            &cmpl_arr[proc].cntr);
           }
           if(rc) armci_die2("LAPI_opND failed",rc,op);
           vecind = 0; totalsize=0; k++;
           if(lastiovlength!=0 && k==(num_xmit-1))iovlength=lastiovlength;
           else iovlength=max_iovec;
           srcv->num_vecs = (uint)iovlength;  dstv->num_vecs= (uint)iovlength;
         }

         dstv->info[vecind] = dst1;
         dstv->len[vecind] = count[0];
         srcv->info[vecind] = src1;
         srcv->len[vecind] = count[0];
         totalsize+=count[0];
         dst1+=dst_stride_arr[0];
         src1+=src_stride_arr[0];
       }
       if(vecind==iovlength){
         LAPI_CLEAR_CNTR((ocmpl));
         ocmpl->val+=1;
         if(op==GET){
           rc = LAPI_Getv(lapi_handle,(uint)proc,srcv,dstv,NULL,ocntr);
         }
         else {
           UPDATE_FENCE_STATE(proc,PUT,1);
           rc = LAPI_Putv(lapi_handle,(uint)proc,dstv,srcv,NULL,ocntr,
                          &cmpl_arr[proc].cntr);
         }
         if(rc) armci_die2("LAPI_opND failed",rc,op);
         vecind = 0; totalsize=0; k++;
         if(lastiovlength!=0 && k==(num_xmit-1))iovlength=lastiovlength;
         else iovlength=max_iovec;
         srcv->num_vecs = (uint)iovlength;  dstv->num_vecs= (uint)iovlength;
       }
    }
    if(DEBUG)printf("\n%d: get completed \n",armci_me);
}



void lapi_op_2d(int op, uint proc, void *src_ptr, void *dst_ptr,uint bytes, 
                int count, int src_stride, int dst_stride,lapi_cmpl_t* o_cmpl)
{
int i,rc;
    if(op==PUT)UPDATE_FENCE_STATE(proc, PUT, count); 
    o_cmpl->val+=count; 
    for(i=0;i<count;i++){
       if(op==PUT)
         rc=LAPI_Put(lapi_handle,proc,bytes,(dst_ptr),(src_ptr),
                     NULL,&(o_cmpl->cntr),&cmpl_arr[proc].cntr);
       else
         rc=LAPI_Get(lapi_handle,proc,bytes,(src_ptr),(dst_ptr),NULL,
                     &(o_cmpl->cntr));
       if(rc)ARMCI_Error("LAPI_put failed",0);
       src_ptr = (void*) ((unsigned long)src_ptr+src_stride);
       dst_ptr = (void*) ((unsigned long)dst_ptr+dst_stride);
    }
}


/*\This function is designed as follows.
 *  CONTIG code breaks ND into 1D chunks a does Lapi_Put on each chunk.
 *  STRIDED code uses strided option in the LAPI_PutV call
 *  VECTOR code packs multi-strided/vector data as vectors as transmits.
 *   ____________________________________ 
 *  | type        small/medium    large |
 *  |------------------------------------ 
 *  | 1D          CONTIG          CONTIG|
 *  | 2D          STRIDED         CONTIG|
 *  | >2D         VECTOR          CONTIG|
 *  |-----------------------------------|
 *  this code uses orig counter from nb_handle for non-blk call
 *  completion counter should always be same for non-blk and blk code to be
 *  able to do ordering/fence.
\*/
void armci_network_strided(int op, void* scale, int proc,void *src_ptr,
                   int src_stride_arr[], void* dst_ptr, int dst_stride_arr[],
                   int count[], int stride_levels, armci_ihdl_t nb_handle)
{
int rc=0;
lapi_cmpl_t *o_cmpl;
int total_of_2D,i,j;    
char *src = (char*)src_ptr, *dst=(char*)dst_ptr;
char *bufptr;
int index[MAX_STRIDE_LEVEL], unit[MAX_STRIDE_LEVEL];
int dsize=3*sizeof(void*);
    /*pick a counter, default for blocking, from descriptor for non-blocking*/
    if(nb_handle){
       INIT_COUNTER((nb_handle->cmpl_info),0);
       o_cmpl = &(nb_handle->cmpl_info);
    }
    else{
       if(op==GET)
         o_cmpl = &get_cntr;
       else
         o_cmpl = &ack_cntr;
    }
    /*CONTIG protocol: used for 1D(contiguous) or if stride is very large in
      a multi strided case*/
    if(stride_levels==0 || count[0]>LONG_PUT_THRESHOLD){
       /*set bufid in nb_handle, in this case, no buffer used, hence NB_NONE*/
       if(nb_handle)
         armci_set_nbhandle_bufid(nb_handle,NULL,NB_NONE);
       switch (stride_levels) {
         case 0: /* 1D op */
           lapi_op_2d(op, (uint)proc, src_ptr, dst_ptr, count[0], 1,
                      0,0,o_cmpl);
           break;
         case 1: /* 2D op */
           lapi_op_2d(op, (uint)proc, src_ptr,dst_ptr, (uint)count[0], count[1],
                      src_stride_arr[0], dst_stride_arr[0], o_cmpl);
           break;
         default: /* N-dimensional */
         {
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
              lapi_op_2d(op, (uint)proc, src, dst,(uint)count[0], count[1],
                          src_stride_arr[0], dst_stride_arr[0],o_cmpl);
          }
         }
       }
    }              
    else{ /* greated than 1D small/med stride */

       if(stride_levels==1){             /*small/med 2D, use lapi STRIDED */
         bufptr = GET_SEND_BUFFER(2*(sizeof(lapi_vec_t)+dsize),op,proc);
         if(nb_handle){ 
           /*update info in the buf_info_t data-structure*/
           SET_BUF_TAG(bufptr,nb_handle->tag,0);
           /*set the buffer id in nb_handle*/
           armci_set_nbhandle_bufid(nb_handle,bufptr,0);
         }
         if(op==PUT)UPDATE_FENCE_STATE(proc, PUT, 1); 

         /*we use the counter in the buffer*/
         o_cmpl = (BUF_TO_EVBUF(bufptr));

         armcill_op2D(op,src_ptr,src_stride_arr[0],dst_ptr,dst_stride_arr[0],
                         count[1],count[0],proc,&(o_cmpl->cntr),bufptr);
       }
       else {                            /*small/med >2D, use lapi VECTOR*/
         bufptr = GET_SEND_BUFFER(DSCR_SIZE,op,proc);
         if(nb_handle){
           /*update info in the buf_info_t data-structure*/
           SET_BUF_TAG(bufptr,nb_handle->tag,0);
           /*set the buffer id in nb_handle*/
           armci_set_nbhandle_bufid(nb_handle,bufptr,0);
         }
         /*we use the counter in the buffer*/
         o_cmpl = (BUF_TO_EVBUF(bufptr));
           
         /*val set to 0 because of the way opND is writted, to be modified*/
         o_cmpl->val=0;

         armcill_opND(op,src_ptr,src_stride_arr,dst_ptr, dst_stride_arr,count,
                         stride_levels,proc,o_cmpl,bufptr);
       }

       /*
         for blocking cases, we can free cmpldescr buffer and wait for op 
         to complete. 
       */
       if(!nb_handle){
          /*for now, we manually clear the counter here for blocking calls.
            for later, this has to be done in FREE_SEND_BUFFER.*/
          LAPI_CLEAR_CNTR(o_cmpl);
          FREE_SEND_BUFFER(bufptr);
       }
       
    }
}


void armci_send_strided_data_bypass(int proc, request_header_t *msginfo,
                                    void *bufptr, int msg_buflen,
                                    void *src_ptr, int *loc_stride_arr,
                                    void *dst_ptr, int *rem_stride_arr,
                                    int *pcount, int stride_levels)
{
lapi_cntr_t c;
int count= pcount[1],bytes=pcount[0],rc;
int src_stride= loc_stride_arr[0];
int dst_stride= rem_stride_arr[0];
lapi_vec_t *src, *dst;
int offset=0;
int p=msginfo->from;


  if(stride_levels!=1)armci_die("armci_send_strided_data_bypass wrong stride",stride_levels);

  LAPI_Setcntr(lapi_handle,&c,0);
 
  src    = (lapi_vec_t *)((unsigned long)bufptr+offset);    offset+=sizeof(lapi_vec_t);
  dst    = (lapi_vec_t *)((unsigned long)bufptr+offset);    offset+=sizeof(lapi_vec_t);
  src->info  = (void **)((unsigned long)bufptr+offset);         offset+=3*sizeof(void *);
  dst->info  = (void **)((unsigned long)bufptr+offset);         offset+=3*sizeof(void *);


  src->vec_type = dst->vec_type                   = LAPI_GEN_STRIDED_XFER;
  src->num_vecs = (uint)count;       dst->num_vecs= (uint)count;

  src->len      = NULL;              dst->len     = NULL;
  src->info[0]  = src_ptr;           dst->info[0]  = dst_ptr;
  src->info[1]  = (void*)bytes;      dst->info[1]  = (void*)bytes;
  src->info[2]  = (void*)src_stride; dst->info[2]  = (void*)dst_stride;

  rc = LAPI_Putv(lapi_handle,(uint)p,dst,src,msginfo->tag.cntr,&c,NULL);
  if(rc)armci_die("armci_send_strided_data_bypass failed",rc);

  if(DEBUG_){
     printf("%dserv: did putv to %d cntr =%p\n",armci_me,p,msginfo->tag.cntr); fflush(stdout);
  }
  LAPI_Waitcntr(lapi_handle, &c,1,NULL);
}
 


/*\ client receives strided data from server
\*/
void armci_rcv_strided_data_bypass_both(int proc, request_header_t* msginfo,
                        void *ptr, int count[], int strides)
{
lapi_cmpl_t *pcntr=BUF_TO_EVBUF(msginfo);
     if(DEBUG_){
        printf("%d: expecting data from %d cntr =%p v=%d\n",armci_me,proc,&pcntr->cntr,pcntr->val);
        fflush(stdout);
     }
     CLEAR_COUNTER((*pcntr));
     if(DEBUG_){
        printf("%d: got data from %d\n",armci_me,proc); fflush(stdout);
     }
}
 

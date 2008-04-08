/* $Id: async.c,v 1.5 2002-12-18 18:25:33 vinod Exp $ */
/* data structures and interfaces for handling asynchronous requests 
 */

#include <stdio.h>
#include "armcip.h" 

#define DEBUG_ 0
/*
#define UBUF_LEN 496
*/
#define MAX_PENDING_REQ 4
#define MAX_STRIDE_LEVELS 8

typedef struct {
  void *ptr;
  int  stride_levels;
  int  stride_arr[MAX_STRIDE_LEVELS];
  int  count[MAX_STRIDE_LEVELS];
}strided_dscr_t;

typedef struct {
  int segments;
  int len;
  void *ptrs[60];
}vector_dscr_t;

typedef struct {
  int reqid;              /* request id */
  int bufid;              /* communication buffer id */
  union {                 /* 8 bytes for alignment reason */
        void *dscrbuf;
        double pad;
  }ptr;
  union {
      char buf[UBUF_LEN];
      strided_dscr_t strided;
      vector_dscr_t vector;
  }dscr;
}saved_dscr_t;


static int cur_id=191;

#define GET_REQ_ID cur_id++
#define REQ_TO_DSC_ID(reqid) ((reqid)>>8)

static saved_dscr_t armci_pending_dscr[MAX_PENDING_REQ];

      
/*\ save a part of strided descriptor needed to complete request
\*/
void _armci_asyn_save_dscr_strided(int id, void *ptr, int stride[],
                                   int count[], int levels)
{
strided_dscr_t *dscr;
int i, dsc_id = REQ_TO_DSC_ID(id);

   dscr = &armci_pending_dscr[dsc_id].dscr.strided;
   dscr->stride_levels = levels;
   dscr->ptr =ptr;
   for(i=0;i<levels;i++)dscr->stride_arr[i]=stride[i];
   for(i=0;i<levels+1;i++)dscr->count[i]=count[i];
}     
   

/*\ for request stored in buf, save local strided descriptor and return  req id
\*/
int _armci_asyn_init_strided_get(void *buf, void *ptr,int levels, int stride[],
                                 int count[])
{
int id, i;
int dsc_id=_armci_buf_to_index(buf);
strided_dscr_t *dscr;       
       
   dscr = &armci_pending_dscr[dsc_id].dscr.strided;
   dscr->stride_levels = levels;

   dscr->ptr =ptr;
   for(i=0;i<levels;i++)dscr->stride_arr[i]=stride[i];
   for(i=0;i<levels+1;i++)dscr->count[i]=count[i];

   /* convert bufid into req id returned to user */
   id = dsc_id;
   id <<= 8;  /* buffer id is in second byte */
   cur_id = (cur_id+1)%255 +1; /* counter in LSB */
   id += cur_id;
   armci_pending_dscr[dsc_id].reqid = id;

   if(DEBUG_){
      printf("%d: init strided get: ptr=%p reqid=%d bufid=%d cid=%d levels=%d count[0]=%d\n",
             armci_me,ptr,id,dsc_id, cur_id, levels,count[0]); fflush(stdout);
   }

   return id;
}    


void _armci_asyn_complete_strided_get(int dsc_id, void *buf)
{
request_header_t *msginfo = (request_header_t*) buf;
strided_dscr_t *dscr;       

   dscr = &armci_pending_dscr[dsc_id].dscr.strided;
   armci_pending_dscr[dsc_id].reqid = 0;

   if(DEBUG_){
      printf("%d:complete_strided_get: ptr=%p bufid=%d levels=%d count[0]=%d\n",
      armci_me,dscr->ptr,dsc_id,dscr->stride_levels,dscr->count[0]); 
      fflush(stdout);
   }

   armci_rcv_strided_data(msginfo->to, msginfo, msginfo->datalen, dscr->ptr, 
                          dscr->stride_levels, dscr->stride_arr,dscr->count);
}

#if 0 /*this function has been added in armci.c*/
int ARMCI_Wait(int req_id)
{
int dsc_id = REQ_TO_DSC_ID(req_id);
void *buf;

   if(DEBUG_){
       printf("%d: WAIT for req id=%d bufid=%d\n",armci_me,req_id,dsc_id); 
       fflush(stdout);
   }

   buf = _armci_buf_ptr_from_id(dsc_id);

   if(dsc_id >MAX_PENDING_REQ) 
       armci_die2("ARMCI_Wait: bad id",dsc_id,MAX_PENDING_REQ);

   /* when 0 it means the request was completed to get the buffer */
   if(armci_pending_dscr[dsc_id].reqid == 0) return 0;

   /* return 1 if request id looks bad */ 
   if(armci_pending_dscr[dsc_id].reqid < req_id) return 1;

   _armci_asyn_complete_strided_get(dsc_id,buf);

   FREE_SEND_BUFFER(buf);

   return 0;
}
#endif

/* $Id: buffers.c,v 1.29 2004-09-07 19:11:06 manoj Exp $    **/
#define SIXTYFOUR 64
#define DEBUG_  0
#define DEBUG2_ 0
#define EXTRA_ERR_CHECK     

/**********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "armcip.h"
#include "request.h"
#ifdef WIN32
#  include <windows.h>
   typedef unsigned long ssize_t;
#else
#  include <unistd.h>
#endif

#define ALIGN64ADD(buf) (SIXTYFOUR-(((ssize_t)(buf))%SIXTYFOUR))
/* the following symbols should be defined if needed in protocol specific
   header file:  BUF_EXTRA_FIELD, BUF_ALLOCATE 
*/

#ifndef BUF_ALLOCATE
#   define BUF_ALLOCATE malloc
#endif

#if defined(DATA_SERVER) && defined(SOCKETS)
#define MAX_BUFS  1
#define MAX_SMALL_BUFS 1
#else
#define MAX_BUFS  4
#define MAX_SMALL_BUFS 16
#endif
#define SMALL_BUF_LEN 8192
#ifndef MSG_BUFLEN_SMALL
#define MSG_BUFLEN_SMALL (MSG_BUFLEN >>0) 
#endif
#define LEFT_GUARD  11.11e11
#define RIGHT_GUARD 22.22e22
#define CLEAR_TABLE_SLOT(idx) *((int*)(_armci_buf_state->table+(idx))) =0


/* we allow multiple buffers (up to 15) per single request
 * adjacent buffers can be coalesced into a large one
 */
typedef struct {
  unsigned int op:8;     /* pending operation code */
  unsigned int snd:1;    /* if 1 then buffer is used for sending request */
  unsigned int rcv:1;    /* if 1 then buffer is used for receiving data */
  unsigned int async:1;  /* if 1 then request is nonblocking */
  unsigned int first:5;  /* id of the 1st buffer in the set in same request */
  unsigned int count:4;  /* how many buffers used for this request 8 possible*/
  unsigned int to:12;    /* serv/proc to which request was sent, 4096 possible*/
}buf_state_t;

#ifndef BUFID_PAD_T
#define BUFID_PAD_T BUF_INFO_T
#endif

/* message send buffer data structure */
typedef struct {
  BUF_INFO_T id;
# ifdef BUF_EXTRA_FIELD_T
        BUF_EXTRA_FIELD_T field;
# endif
  char buffer[MSG_BUFLEN_SMALL];
} buf_ext_t;

/* message send buffer data structure */
typedef struct {
  BUF_INFO_T id;
# ifdef BUF_EXTRA_FIELD_T
        BUF_EXTRA_FIELD_T field;
# endif
  char buffer[SMALL_BUF_LEN];
} buf_smext_t;

/* we keep table and buffer pointer together for better locality */
typedef struct {
  double left_guard;        /* stamp to verify if array was corrupted */
  buf_state_t table[MAX_BUFS+MAX_SMALL_BUFS]; /*array with state of buffer */
  buf_ext_t *buf;           /* address of buffer pool */
  buf_smext_t *smallbuf;      /* address of the large buffer pool */
  int avail;
  int smavail;
  int pad;
  double right_guard;       /* stamp to verify if array was corrupted */
} reqbuf_pool_t;            

#ifndef  BUF_EXTRA_FIELD_T
#  define        SIZE_BUF_EXTRA_FIELD 0 
#  define BUF_TO_EBUF(buf) (buf_ext_t*)(((char*)buf) - sizeof(BUFID_PAD_T) -\
                                      SIZE_BUF_EXTRA_FIELD)
#  define BUF_TO_SMEBUF(buf) (buf_smext_t*)(((char*)buf)- sizeof(BUFID_PAD_T) -\
                                      SIZE_BUF_EXTRA_FIELD)
#else
#  define BUF_TO_EBUF(buf) (buf_ext_t*)(((char*)buf) - sizeof(BUFID_PAD_T) -\
				      sizeof(BUF_EXTRA_FIELD_T))
#  define BUF_TO_SMEBUF(buf) (buf_smext_t*)(((char*)buf)- sizeof(BUFID_PAD_T) -\
				      sizeof(BUF_EXTRA_FIELD_T))
#endif

#define BUF_TO_BUFINDEX(buf) (BUF_TO_EBUF((buf)))->id.bufid
#define BUF_TO_SMBUFINDEX(buf) (BUF_TO_SMEBUF((buf)))->id.bufid




buf_ext_t *_armci_buffers;        /* these are the actual buffers */
buf_smext_t *_armci_smbuffers;    /* no, these are the actual buffers */
reqbuf_pool_t* _armci_buf_state;  /* array that describes state of each buf */ 


/*\ we allocate alligned buffer space
 *  this operation can be implemented in platform specific files
\*/ 
void _armci_buf_init()
{
char *tmp;
int  extra=0;
int smallbuf_size = sizeof(buf_smext_t)*(MAX_SMALL_BUFS);
     tmp = (char *)BUF_ALLOCATE((MAX_BUFS*sizeof(buf_ext_t) + 64 + smallbuf_size));
     extra= ALIGN64ADD(tmp);
     if(sizeof(buf_state_t) != sizeof(int)) 
        armci_die("armci_buf_init size buf_state_t!=int",sizeof(buf_state_t));
                   
     _armci_buffers = (buf_ext_t *) (tmp + extra); 

     tmp = (char *)(_armci_buffers + MAX_BUFS);
     extra = ALIGN64ADD(tmp);
     _armci_smbuffers = (buf_smext_t *) (tmp + extra); 
     

     if(DEBUG2_){
	printf("%d:armci_init_bufs: pointer %p, before align ptr=%p bufptr=%p end of region is %p  size=%d extra=%d\n",
               armci_me,_armci_buffers,tmp,_armci_buffers->buffer,(_armci_buffers+MAX_BUFS),
               MAX_BUFS*sizeof(buf_ext_t),extra);
	fflush(stdout);
     }

     /* now allocate state array */
     tmp  = calloc(1, sizeof(reqbuf_pool_t) + 64);
     if(!tmp)armci_die("_armci_buf_init calloc failed",0);
     extra= ALIGN64ADD(tmp);
     _armci_buf_state = (reqbuf_pool_t*)(tmp + extra); 

     /* initialize it */
     _armci_buf_state->left_guard  = LEFT_GUARD;
     _armci_buf_state->right_guard = RIGHT_GUARD;
     _armci_buf_state->avail =0;
     _armci_buf_state->smavail =MAX_BUFS;
     _armci_buf_state->buf = _armci_buffers;
     _armci_buf_state->smallbuf = _armci_smbuffers;



     if(BUF_TO_EBUF(_armci_buf_state->buf[0].buffer)!=_armci_buf_state->buf)
        armci_die("buffers.c, internal structure alignment problem",0);
}


/*\ convert buffer pointer to index (in state array)
\*/
int _armci_buf_to_index(void *buf)
{
int index;
char *ptr = (char*)buf;

   if(DEBUG2_){
     printf("%d: in _armci_buf_to_index %p\n",armci_me, buf);
     fflush(stdout);
   }
   if(buf > (void *)_armci_buffers && buf < (void *)(_armci_buffers+MAX_BUFS)){
      index = BUF_TO_BUFINDEX(ptr);
      if((index >= MAX_BUFS)|| (index<0)) 
        armci_die2("armci_buf_to_index: bad index:",index,MAX_BUFS);
      return(index);
   }
   else if(buf > (void *)_armci_smbuffers && buf < (void *)(_armci_smbuffers+MAX_SMALL_BUFS)){
      index = BUF_TO_SMBUFINDEX(ptr);
      if((index >= MAX_BUFS+MAX_SMALL_BUFS)|| (index<MAX_BUFS)) 
        armci_die2("armci_buf_to_ind:indexwrong",index,MAX_BUFS+MAX_SMALL_BUFS);
      return(index);
   } 
   else {
        armci_die("armci_buf_to_index: bad pointer",0);
        return(0);
   }
}



/*\  complete outstanding operation that uses the specified buffer
\*/
void _armci_buf_complete_index(int idx, int called)
{
int count;
buf_state_t *buf_state = _armci_buf_state->table +idx;

    count = buf_state->count;
    if(DEBUG_ ){
       printf("%d:buf_complete_index:%d op=%d first=%d count=%d called=%d\n",
              armci_me,idx,buf_state->op,buf_state->first,buf_state->count,
              called); 
       fflush(stdout);
    }

    if(buf_state->first != (unsigned int)idx){ 
      armci_die2("complete_buf_index:inconsistent index:",idx,buf_state->first);
    }

    if(buf_state->async){
      /* completion of strided get should release that buffer */
      if(buf_state->op == GET);
      else
         armci_die2("buf_complete_index: async mode not avail for this op",
                     buf_state->op,idx);
    }
#   ifdef BUF_EXTRA_FIELD_T
    else{
       /* need to call platform specific function */
       if(idx>=MAX_BUFS){
         int relidx;
         relidx = idx-MAX_BUFS; 
         
         CLEAR_SEND_BUF_FIELD(_armci_buf_state->smallbuf[relidx].field,buf_state->snd,buf_state->rcv,buf_state->to,buf_state->op);

       /*later, we might just need to do this for all operations, not just get*/
         if(_armci_buf_state->smallbuf[relidx].id.tag!=0 &&(buf_state->op == GET)){
          armci_complete_req_buf(&(_armci_buf_state->smallbuf[relidx].id),
                                _armci_buf_state->smallbuf[relidx].buffer);
         }
         _armci_buf_state->smallbuf[relidx].id.tag=0;
       }
       else {
         CLEAR_SEND_BUF_FIELD(_armci_buf_state->buf[idx].field,buf_state->snd,buf_state->rcv,buf_state->to,buf_state->op);

       /*later, we might just need to do this for all operations, not just get*/
         if(_armci_buf_state->buf[idx].id.tag!=0 &&(buf_state->op == GET)){
           armci_complete_req_buf(&(_armci_buf_state->buf[idx].id),
                                _armci_buf_state->buf[idx].buffer);
         }
         _armci_buf_state->buf[idx].id.tag=0;
       }
    }
#   endif

    /* clear table slots for all the buffers in the set for this request */
    for(; count; count--, buf_state++) *(int*)buf_state = 0;
}


/*\  test outstanding operation that uses the specified buffer for complete
 *   It is important not to change the state of the buffer, the buffer has
 *   to remain as it was, only completion has to be indicated
\*/
int _armci_buf_test_index(int idx, int called)
{
int count,retval=0;
buf_state_t *buf_state = _armci_buf_state->table +idx;
    count = buf_state->count;
    if(DEBUG_ ){
       printf("%d:buf_test_index:%d op=%d first=%d count=%d called=%d\n",
              armci_me,idx,buf_state->op,buf_state->first,buf_state->count,
              called); 
       fflush(stdout);
    }
    if(buf_state->first != (unsigned int)idx){ 
      armci_die2("_buf_test_index:inconsistent index:",idx,buf_state->first);
    }
#   ifdef BUF_EXTRA_FIELD_T
    /* need to call platform specific function */
    if(idx>=MAX_BUFS){
       int relidx;
       relidx = idx-MAX_BUFS; 
       /*printf("\n%d:relidx=%d \n",armci_me,relidx);fflush(stdout);*/
       TEST_SEND_BUF_FIELD(_armci_buf_state->smallbuf[relidx].field,buf_state->snd,buf_state->rcv,buf_state->to,buf_state->op,&retval);

    }
    else {
       TEST_SEND_BUF_FIELD(_armci_buf_state->buf[idx].field,buf_state->snd,buf_state->rcv,buf_state->to,buf_state->op,&retval);
    }
#   endif
    if(DEBUG_ ){
       printf("%d:buf_test_index:%d op=%d first=%d count=%d called=%d ret=%d\n",
              armci_me,idx,buf_state->op,buf_state->first,buf_state->count,
              called,retval); 
       fflush(stdout);
    }
    return(retval);
}


/*\ make sure that there are no other pending operations to that smp node
 *  this operation is called from platforms specific routine that sends
 *  request
 *  we could have accomplished the same in armci_buf_get but as Vinod
 *  is pointing out, it is better to delay completing outstanding
 *  calls to overlap memcpy for the current buffer with communication
\*/
void _armci_buf_ensure_one_outstanding_op_per_node(void *buf, int node)
{
    int i;
    int index =_armci_buf_to_index(buf); 
    int this = _armci_buf_state->table[index].first;
    int nfirst, nlast;

    nfirst=armci_clus_info[node].master;
    nlast = nfirst+armci_clus_info[node].nslave-1;

    if((_armci_buf_state->table[index].to<(unsigned int) nfirst) || 
       (_armci_buf_state->table[index].to>(unsigned int) nlast))
        armci_die2("_armci_buf_ensure_one_outstanding_op_per_node: bad to",node,
                (int)_armci_buf_state->table[index].to);

    for(i=0;i<MAX_BUFS+MAX_SMALL_BUFS;i++){
        buf_state_t *buf_state = _armci_buf_state->table +i;
        if((buf_state->to >= nfirst) && (buf_state->to<= (unsigned int) nlast))
          if((buf_state->first != (unsigned int) this)&&(buf_state->first==(unsigned int) i) && buf_state->op)
                _armci_buf_complete_index(i,0);
    }
}

/*\ same as above but for process
\*/
void _armci_buf_ensure_one_outstanding_op_per_proc(void *buf, int proc)
{
    int i;
    int index = _armci_buf_to_index(buf); 
    int this = _armci_buf_state->table[index].first;

    if(_armci_buf_state->table[index].to !=(unsigned int)  proc )
       armci_die2("_armci_buf_ensure_one_outstanding_op_per_proc: bad to", proc,
                (int)_armci_buf_state->table[index].to);

    for(i=0;i<MAX_BUFS+MAX_SMALL_BUFS;i++){
        buf_state_t *buf_state = _armci_buf_state->table +i;
        if(buf_state->to == (unsigned int) proc)
          if((buf_state->first != (unsigned int) this)&&(buf_state->first==(unsigned int) i) && buf_state->op)
                _armci_buf_complete_index(i,0);
    }
}


#define HISTORY__ 
#ifdef HISTORY
typedef struct{ int size; int op; int count; int id; } history_t;
history_t history[100];
int h=0;

void print_history()
{
int i;
    fflush(stdout);
    printf("%d records\n",h);
    for(i=0; i<h;i++) printf("size=%d id=%d ptr=%p count=%d op=%d\n",
        history[i].size, history[i].id,
       _armci_buf_state->buf[history[i].id].buffer, history[i].count,
        history[i].op);

    fflush(stdout);
}
#endif

/*\  call corresponding to GET_SEND_BUF
\*/
char *_armci_buf_get_small(int size, int operation, int to)
{
int avail=_armci_buf_state->smavail,i;
    if(_armci_buf_state->table[avail].op || 
       _armci_buf_state->table[avail].first) {
       
       for(i=MAX_BUFS;i<MAX_BUFS+MAX_SMALL_BUFS;i++){
          if(!_armci_buf_state->table[i].op &&
             !_armci_buf_state->table[i].first)
            break;
       }
       if(i<(MAX_SMALL_BUFS+MAX_BUFS))avail = i;
       else {
          _armci_buf_complete_index(avail,1);
       }
    }
    _armci_buf_state->table[avail].op = operation;
    _armci_buf_state->table[avail].to = to;
    _armci_buf_state->table[avail].count=  1;
    _armci_buf_state->table[avail].first = avail;
    _armci_buf_state->smallbuf[avail-MAX_BUFS].id.tag=0;
    _armci_buf_state->smallbuf[avail-MAX_BUFS].id.bufid= avail; 
    _armci_buf_state->smallbuf[avail-MAX_BUFS].id.protocol=0;
# ifdef BUF_EXTRA_FIELD_T
    INIT_SEND_BUF(_armci_buf_state->smallbuf[avail-MAX_BUFS].field,_armci_buf_state->table[avail].snd,_armci_buf_state->table[avail].rcv);
#endif
     
    _armci_buf_state->smavail = (avail+1-MAX_BUFS)%MAX_SMALL_BUFS + MAX_BUFS;

    if(DEBUG_ || 0){
      printf("%d:buf_get_sm:size=%d max=%d got %d ptr=%p op=%d to=%d\n",
             armci_me,size,SMALL_BUF_LEN,avail,
            _armci_buf_state->smallbuf[avail-MAX_BUFS].buffer,operation,to);
      fflush(stdout);
    }

    return(_armci_buf_state->smallbuf[avail-MAX_BUFS].buffer); 

}

/*\  call corresponding to GET_SEND_BUF
\*/
char *_armci_buf_get(int size, int operation, int to)
{
int avail=_armci_buf_state->avail;
int count=1, i;
    /*if small buffer, we go to another routine that gets smallbuf*/
    if(size<SMALL_BUF_LEN) return(_armci_buf_get_small(size,operation,to));
    /* compute number of buffers needed (count) to satisfy the request */
    if((size > MSG_BUFLEN_SMALL) ){ 
       double val = (double)size;  /* use double due to a bug in gcc */
       val /= MSG_BUFLEN_SMALL;
       count=(int)val;
       if(size%MSG_BUFLEN_SMALL) count++; 
    }
    /* start from 0 if there is not enough bufs available from here */
    if((avail+count) > MAX_BUFS)avail = 0;

    /* avail should never point to buffer in a middle of a set of used bufs */
    if(_armci_buf_state->table[avail].op && 
      (_armci_buf_state->table[avail].first != (unsigned int) avail)){ sleep(1); 
              armci_die2("armci_buf_get: inconsistent first", avail,
                         _armci_buf_state->table[avail].first);
      }
 
    /* we need complete "count" number of buffers */
    for(i=0;i<count;i++){
        int cur = i +avail;
        if(_armci_buf_state->table[cur].op &&
           _armci_buf_state->table[cur].first==(unsigned int) cur)
                                   _armci_buf_complete_index(cur,1);
    }

    for(i=0; i<count; i++){
       _armci_buf_state->table[avail+i].op = operation;
       _armci_buf_state->table[avail+i].to = to;
       _armci_buf_state->table[avail+i].count=  count;
       _armci_buf_state->table[avail+i].first = avail;
    }

    _armci_buf_state->buf[avail].id.tag=0;
    _armci_buf_state->buf[avail].id.bufid=avail; 
    _armci_buf_state->buf[avail].id.protocol=0;

# ifdef BUF_EXTRA_FIELD_T
    INIT_SEND_BUF(_armci_buf_state->buf[avail].field,_armci_buf_state->table[avail].snd,_armci_buf_state->table[avail].rcv);
#endif

#ifdef HISTORY
    history[h].size=size;
    history[h].op=operation;
    history[h].count=count;
    history[h].id = avail;
    h++;
#endif

    if(DEBUG_ || 0){
      printf("%d:buf_get:size=%d max=%d got %d ptr=%p count=%d op=%d to=%d\n",
             armci_me,size,MSG_BUFLEN_SMALL,avail,
            _armci_buf_state->buf[avail].buffer, count,operation,to);
      fflush(stdout);
    }

    /* select candidate buffer for next allocation request */
    _armci_buf_state->avail = avail+count;
    _armci_buf_state->avail %= MAX_BUFS;

    return(_armci_buf_state->buf[avail].buffer); 
}



/*\ release buffer when it becomes free
\*/
void _armci_buf_release(void *buf)
{
int  count, index = _armci_buf_to_index(buf);
buf_state_t *buf_state = _armci_buf_state->table +index;
   if((index >= MAX_BUFS+MAX_SMALL_BUFS)|| (index<0))
      armci_die2("armci_buf_release: bad index:",index,MAX_BUFS);

   count =  _armci_buf_state->table[index].count;

   if(DEBUG_){
     printf("%d:_armci_buf_release %d ptr=%p count=%d op=%d\n",
            armci_me,index,buf,count, _armci_buf_state->table[index].op);
     fflush(stdout);
   }

   /* clear table slots for all the buffers in the set for this request */
   for(; count; count--, buf_state++) *(int*)buf_state = 0;
   if(index >= MAX_BUFS){
      _armci_buf_state->smallbuf[index-MAX_BUFS].id.tag=0;
#ifndef VAPI
      _armci_buf_state->smavail = index;
#endif
   }
   else{
      _armci_buf_state->buf[index].id.tag=0;
#ifndef VAPI
      _armci_buf_state->avail = index;
#endif
   }
   

   /* the current buffer is prime candidate to satisfy next buffer request */
}


/*\ return pointer to buffer number id
\*/
char *_armci_buf_ptr_from_id(int id)
{
  if(id <0 || id >=(MAX_BUFS+MAX_SMALL_BUFS)) 
              armci_die2("armci_buf_ptr_from_id: bad id",id,MAX_BUFS);
  if(id >=MAX_BUFS)return(_armci_buf_state->smallbuf[id-MAX_BUFS].buffer);
  return(_armci_buf_state->buf[id].buffer);
}



/*\function called from ARMCI_Wait to wait for non-blocking ops
\*/
void _armci_buf_complete_nb_request(int bufid,unsigned int tag, int *retcode) 
{
int i=0;
#if 0
    printf("\n%d:wait called with bufid=%d tag=%d \n",armci_me,bufid,tag);
    fflush(stdout);
#endif
 
    if(bufid == NB_NONE) *retcode=0;
    else if(bufid == NB_MULTI) {
       for(i=0;i<MAX_BUFS;i++){ 
         if(tag && tag==_armci_buf_state->buf[i].id.tag)
           _armci_buf_complete_index(i,1); 
       }
       for(i=0;i<MAX_SMALL_BUFS;i++){ 
         if(tag && tag==_armci_buf_state->smallbuf[i].id.tag)
           _armci_buf_complete_index(i+MAX_BUFS,1); 
       }
       *retcode=0;
    }
    else {
       if(bufid<MAX_BUFS){
         if(tag && tag==_armci_buf_state->buf[bufid].id.tag)
           _armci_buf_complete_index(bufid,1);
       }
       else{
         if(tag && tag==_armci_buf_state->smallbuf[bufid-MAX_BUFS].id.tag)
           _armci_buf_complete_index(bufid,1);
       }
       *retcode=0;
    } 
}


/*\function called from ARMCI_Test to test completion of non-blocking ops
\*/
void _armci_buf_test_nb_request(int bufid,unsigned int tag, int *retcode) 
{
int i;
    if(bufid == NB_NONE) *retcode=0;
    else if(bufid == NB_MULTI) {
       for(i=0;i<MAX_BUFS;i++){ 
         if(tag && tag==_armci_buf_state->buf[i].id.tag){
           if(_armci_buf_test_index(i,1)){
             *retcode=1;
	     break;
	   }
	 }
       }
       for(i=0;i<MAX_SMALL_BUFS;i++){ 
         if(tag && tag==_armci_buf_state->smallbuf[i].id.tag)
           if(_armci_buf_test_index(i+MAX_BUFS,1)){
             *retcode=1;
	     break;
	   }
       }
    }
    else {
       if(bufid<MAX_BUFS){
         if(tag && tag==_armci_buf_state->buf[bufid].id.tag)
           *retcode = _armci_buf_test_index(bufid,1);
       }
       else{
         if(tag && tag==_armci_buf_state->smallbuf[bufid-MAX_BUFS].id.tag)
           *retcode = _armci_buf_test_index(bufid,1);
       }
    }
}

/*\function to set the buffer tag and the protocol
\*/
void _armci_buf_set_tag(void *bufptr,unsigned int tag,short int protocol)
{
int  index = _armci_buf_to_index(bufptr);
   /*_armci_buf_state->table[index].async=1;*/
   if(index<MAX_BUFS){
      _armci_buf_state->buf[index].id.tag=tag;
      _armci_buf_state->buf[index].id.protocol=protocol;
   }
   else{
      _armci_buf_state->smallbuf[index-MAX_BUFS].id.tag=tag;
      _armci_buf_state->smallbuf[index-MAX_BUFS].id.protocol=protocol;
   }
}

/*\function to return bufinfo, given buf ptr
\*/
BUF_INFO_T *_armci_buf_to_bufinfo(void *buf){
    if(buf > (void *)_armci_buffers && buf < (void *)(_armci_buffers+MAX_BUFS)){
       return(&((BUF_TO_EBUF(buf))->id));
    }
   else if(buf > (void *)_armci_smbuffers && buf < (void *)(_armci_smbuffers+MAX_SMALL_BUFS)){
       return(&((BUF_TO_SMEBUF(buf))->id));
   } 
   else {
        armci_die("armci_buf_to_index: bad pointer",0);
        return(0);
   }
}

/*\function to clear all buffers
\*/
void _armci_buf_clear_all()
{
int i; 
    for(i=0;i<MAX_BUFS;i++){
# ifdef BUF_EXTRA_FIELD_T
       if(_armci_buf_state->table[i].op || _armci_buf_state->table[i].first)
         CLEAR_SEND_BUF_FIELD(_armci_buf_state->buf[i].field,_armci_buf_state->table[i].snd,_armci_buf_state->table[i].rcv,_armci_buf_state->table[i].to,_armci_buf_state->table[i].op);
#endif
    }
    for(i=MAX_BUFS;i<MAX_BUFS+MAX_SMALL_BUFS;i++){
# ifdef BUF_EXTRA_FIELD_T
       if(_armci_buf_state->table[i].op || _armci_buf_state->table[i].first)
         CLEAR_SEND_BUF_FIELD(_armci_buf_state->smallbuf[i-MAX_BUFS].field,_armci_buf_state->table[i].snd,_armci_buf_state->table[i].rcv,_armci_buf_state->table[i].to,_armci_buf_state->table[i].op);
#endif
    }
}

#ifdef VAPI
/* this will work for vapi as there is no get pipeline enabled in vapi
 * with get pipeline, this will break very badly
 */
void _armci_buf_update_scatter_count(int id)
{
int i,num,last,first;
    for(i=0;i<MAX_BUFS;i++){
# ifdef BUF_EXTRA_FIELD_T
       if(_armci_buf_state->table[i].op==GET){
         request_header_t *msginfo;
	 msginfo = (request_header_t*)_armci_buf_state->buf[i].buffer; 
         if(msginfo->pinned && msginfo->bypass && msginfo->format == STRIDED){
           num = *(int *)((char *)msginfo+msginfo->bytes); 
           last = *(int *)((char *)msginfo+msginfo->bytes+sizeof(int));
           first = last - num+1;
           if(first < 0 )first+=DSCRID_SCATTERCLIENT_END-DSCRID_SCATTERCLIENT-1;
           if(id == first && num!=0){
             *(int *)((char *)msginfo+msginfo->bytes) = (--num);
             return;
           }
         }
       }
# endif
    }
    for(i=MAX_BUFS;i<MAX_BUFS+MAX_SMALL_BUFS;i++){
# ifdef BUF_EXTRA_FIELD_T
       if(_armci_buf_state->table[i].op==GET){
         request_header_t *msginfo = (request_header_t*)_armci_buf_state->smallbuf[i-MAX_BUFS].buffer;
         if(msginfo->pinned && msginfo->bypass && msginfo->format == STRIDED){
           num = *(int *)((char *)msginfo+msginfo->bytes); 
           last = *(int *)((char *)msginfo+msginfo->bytes+sizeof(int));
           first = last - num+1;
           if(first < 0 )first+=DSCRID_SCATTERCLIENT_END-DSCRID_SCATTERCLIENT-1;
           if(id == first && num!=0){
             *(int *)((char *)msginfo+msginfo->bytes) = (--num);
             return;
           }
         }
       }
# endif
    }

}
#endif


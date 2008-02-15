/* $Id: request.c,v 1.72.2.1 2007/03/07 01:11:58 vinod Exp $ */
#include "armcip.h"
#include "request.h"
#include "memlock.h"
#include "shmem.h"
#include "copy.h"
#include "gpc.h"
#include <stdio.h>
#include <signal.h>

#define DEBUG_ 0

#if !defined(GM) && !defined(VIA) && !defined(LAPI) &&!defined(VAPI)
  double _armci_rcv_buf[MSG_BUFLEN_DBL];
  double _armci_snd_buf[MSG_BUFLEN_DBL]; 
  char* MessageSndBuffer = (char*)_armci_snd_buf;
  char* MessageRcvBuffer = (char*)_armci_rcv_buf;
#endif


#define MAX_EHLEN 248
#define ADDBUF(buf,type,val) *(type*)(buf) = (val); (buf) += sizeof(type)
#define GETBUF(buf,type,var) (var) = *(type*)(buf); (buf) += sizeof(type)

#define ALLIGN8(buf){size_t _adr=(size_t)(buf); \
                    _adr>>=3; _adr<<=3; _adr+=8; (buf) = (char*)_adr; }

#ifndef CLN
#   define CLN 1
#endif                    
#ifndef SERV
#   define SERV 2
#endif

/*******************Routines to handle completion descriptor******************/
/*\
 *Following the the routines to fill a completion descriptor, if necessary
 *copy the data to destination based on completion descriptor
 *NOTE, THE FOLLOWING ROUTINES ARE FOR CLIENTS ONLY
\*/ 


/*\Routine to complete a vector request, data is in buf and descriptor in dscr
\*/
static void armci_complete_vector_get(armci_giov_t darr[],int len,void *buf)
{
int proc;
request_header_t *msginfo = (request_header_t*) buf;
    proc = msginfo->to;
    armci_rcv_vector_data(proc, msginfo, darr, len);
    FREE_SEND_BUFFER(buf);
}


/*\ Routine called from buffers.c to complete a request for which the buffer was
 *  used for, so that the buffer can be reused.
\*/
void armci_complete_req_buf(BUF_INFO_T *info, void *buffer)
{
request_header_t *msginfo = (request_header_t*) buffer;
    if(info->protocol==0)return;
    else if(info->protocol==SDSCR_IN_PLACE){
       char *dscr = info->dscr;
       void *loc_ptr;
       int stride_levels;
       int *loc_stride_arr,*count;

       loc_ptr = *(void**)dscr;           dscr += sizeof(void*);
       stride_levels = *(int*)dscr;       dscr += sizeof(int);
       loc_stride_arr = (int*)dscr;       dscr += stride_levels*sizeof(int);
       count = (int*)dscr;
       if(0 || DEBUG_){ 
         if(armci_me==0){
           printf("\n%d:extracted loc_ptr=%p, stridelevels=%d\n",armci_me,
                  loc_ptr,stride_levels);
           fflush(stdout);
         }
       }

#if defined(ALLOW_PIN) || defined(LAPI2)
       if(msginfo->pinned && msginfo->bypass){
         armci_rcv_strided_data_bypass_both(msginfo->to,msginfo,loc_ptr,count,
	                                    stride_levels);
       }
       else
#endif
         armci_rcv_strided_data(msginfo->to, msginfo, msginfo->datalen, loc_ptr,
                                stride_levels,loc_stride_arr,count);

       FREE_SEND_BUFFER(msginfo);
    }
    else if(info->protocol==VDSCR_IN_PLACE || info->protocol==VDSCR_IN_PTR){
       char *dscr;
       int len,i;
       if(info->protocol==VDSCR_IN_PLACE)dscr = info->dscr;
       else dscr = info->ptr.dscrbuf;
       GETBUF(dscr, long ,len);
       {
         armci_giov_t *darr;
         darr = (armci_giov_t *)malloc(sizeof(armci_giov_t)*len);
         if(!darr)armci_die("malloc in complete_req_buf failed",len);
         for(i = 0; i< len; i++){
           int parlen, bytes;
           GETBUF(dscr, int, parlen);
           GETBUF(dscr, int, bytes);
           darr[i].ptr_array_len = parlen;
           darr[i].bytes = bytes;
           if(msginfo->operation==GET)darr[i].dst_ptr_array=(void **)dscr;
           else darr[i].src_ptr_array=(void **)dscr;
           dscr+=sizeof(void *)*parlen;
         }
         if(msginfo->operation==GET)armci_complete_vector_get(darr,len,buffer);
       }
    }
    else 
       armci_die("armci_complete_req_buf,protocol val invalid",info->protocol);
}


/*\ save a part of strided descriptor needed to complete request
\*/
void armci_save_strided_dscr(char **bptr, void *rem_ptr,int rem_stride_arr[],
                             int count[], int stride_levels,int is_nb)
{
int i;
char *bufptr=*bptr;
BUF_INFO_T *info=NULL;
    if(is_nb){    
       info=BUF_TO_BUFINFO(*bptr);
       bufptr = (info->dscr);
    }
    *(void**)bufptr = rem_ptr;         bufptr += sizeof(void*);
    *(int*)bufptr = stride_levels;     bufptr += sizeof(int);
    for(i=0;i<stride_levels;i++)((int*)bufptr)[i] = rem_stride_arr[i];
    bufptr += stride_levels*sizeof(int);
    for(i=0;i< stride_levels+1;i++)((int*)bufptr)[i] = count[i];
    bufptr += (1+stride_levels)*sizeof(int);
    if((0 || DEBUG_) && is_nb){
      bufptr = (info->dscr);
      if(armci_me==0)
        printf("\n%d:rem_ptr %p=%p stride_levels %d=%d\n",armci_me,
                *(void**)bufptr,rem_ptr,
                *(int*)(bufptr + sizeof(void*)),stride_levels);
    } 
    /*remote_strided expects the pointer to point to the end of descr hence..*/
    if(is_nb)
       info->protocol=SDSCR_IN_PLACE;
    else
       *bptr=bufptr;
}


/*\ save a part of vector descriptor needed to complete request
\*/
void armci_save_vector_dscr(char **bptr,armci_giov_t darr[],int len,
                            int op,int is_nb)
{
int i,size=sizeof(int);
BUF_INFO_T *info;
char *buf,*bufptr=*bptr;
void *rem_ptr;
    if(is_nb){    
       for(i=0;i<len;i++){
         size+=(2*sizeof(int)+darr[i].ptr_array_len * sizeof(void*));
       }
       info=BUF_TO_BUFINFO(bufptr);
       /*if descr fits in available buffer, use it else do malloc */
       if(size<=UBUF_LEN){
         buf = info->dscr;
         info->protocol=VDSCR_IN_PLACE;
       }
       else {
         info->ptr.dscrbuf = (void *)malloc(size);
         buf = (char *)info->ptr.dscrbuf;
         info->protocol=VDSCR_IN_PTR;
       }
    }
    else
       buf=bufptr;
      
    ADDBUF(buf,long,len); /* number of sets */
    for(i=0;i<len;i++){
        ADDBUF(buf,int,darr[i].ptr_array_len); /* number of elements */
        ADDBUF(buf,int,darr[i].bytes);         /* sizeof element */
        if(op==GET){
          if(is_nb)
            rem_ptr = darr[i].dst_ptr_array;
          else
            rem_ptr = darr[i].src_ptr_array;
        }
        else rem_ptr = darr[i].dst_ptr_array;
        armci_copy(rem_ptr,buf, darr[i].ptr_array_len * sizeof(void*));
        buf += darr[i].ptr_array_len*sizeof(void*);
    }
    if(!is_nb)
       *bptr=buf;
}

/*\
 * If buf==null, set handle->bufid to val, else set it to the id of the buf
\*/
void armci_set_nbhandle_bufid(armci_ihdl_t nb_handle,char *buf,int val)
{
BUF_INFO_T *info;
    if(buf){
       info = BUF_TO_BUFINFO(buf);
       val = info->bufid;
    }
    nb_handle->bufid = val; 
} 

/**************End--Routines to handle completion descriptor******************/


/*\ send request to server to LOCK MUTEX
\*/
void armci_rem_lock(int mutex, int proc, int *ticket)      
{
request_header_t *msginfo;
int *ibuf;
int bufsize = sizeof(request_header_t)+sizeof(int);
 
    msginfo = (request_header_t*)GET_SEND_BUFFER(bufsize,LOCK,proc);
    bzero(msginfo,sizeof(request_header_t));

    msginfo->datalen = sizeof(int);
    msginfo->dscrlen = 0;
    msginfo->from  = armci_me;
    msginfo->to    = proc;
    msginfo->operation = LOCK;
    msginfo->format  = mutex;
    msginfo->bytes = msginfo->datalen + msginfo->dscrlen;

    ibuf = (int*)(msginfo+1);
    *ibuf = mutex;

    armci_send_req(proc, msginfo, bufsize);

    /* receive ticket from server */
    *ticket = *(int*)armci_rcv_data(proc,msginfo);
    FREE_SEND_BUFFER(msginfo);
    
    if(DEBUG_)fprintf(stderr,"%d receiving ticket %d\n",armci_me, *ticket);
}




void armci_server_lock(request_header_t *msginfo)
{
int *ibuf = (int*)(msginfo+1);
int proc  = msginfo->from;
int mutex;
int ticket;

    mutex = *(int*)ibuf;

    /* acquire lock on behalf of requesting process */
    ticket = armci_server_lock_mutex(mutex, proc, msginfo->tag);
    
    if(ticket >-1){
       /* got lock */
       msginfo->datalen = sizeof(int);
       armci_send_data(msginfo, &ticket);
    }
}
       
    


/*\ send request to server to UNLOCK MUTEX
\*/
void armci_rem_unlock(int mutex, int proc, int ticket)
{
request_header_t *msginfo;
int *ibuf;
int bufsize = sizeof(request_header_t)+sizeof(ticket);

    msginfo = (request_header_t*)GET_SEND_BUFFER(bufsize,UNLOCK,proc);
    bzero(msginfo,sizeof(request_header_t));

    msginfo->dscrlen = msginfo->bytes = sizeof(ticket); 
    msginfo->datalen = 0; 
    msginfo->from  = armci_me;
    msginfo->to    = proc;
    msginfo->operation = UNLOCK;
    msginfo->format  = mutex;

    ibuf = (int*)(msginfo+1);
    *ibuf = ticket;

    if(DEBUG_)fprintf(stderr,"%d sending unlock\n",armci_me);
    armci_send_req(proc, msginfo, bufsize);
}
    


/*\ server unlocks mutex and passes lock to the next waiting process
\*/
void armci_server_unlock(request_header_t *msginfo, char* dscr)
{
    int ticket = *(int*)dscr;
    int mutex  = msginfo->format;
    int proc   = msginfo->to;
    int waiting;
    
    waiting = armci_server_unlock_mutex(mutex,proc,ticket,&msginfo->tag);

    if(waiting >-1){ /* -1 means that nobody is waiting */

       ticket++;
       /* pass ticket to the waiting process */
       msginfo->from = waiting;
       msginfo->datalen = sizeof(ticket);
       armci_send_data(msginfo, &ticket);

    }
}


void armci_unlock_waiting_process(msg_tag_t tag, int proc, int ticket)
{
request_header_t header;
request_header_t *msginfo = &header;

       msginfo->datalen = sizeof(int);
       msginfo->tag     = tag;
       msginfo->from      = proc;
       msginfo->to    = armci_me;
       armci_send_data(msginfo, &ticket); 
}

#ifdef REGIONS_REQUIRE_MEMHDL
void armci_serv_register_req(void *ptr,long sz,ARMCI_MEMHDL_T *memhdl)
{
char *buf;
int bufsize = sizeof(request_header_t)+sizeof(long)+sizeof(void *)+sizeof(ARMCI_MEMHDL_T);
request_header_t *msginfo = (request_header_t*)GET_SEND_BUFFER(bufsize,ATTACH,armci_me);
    bzero(msginfo,sizeof(request_header_t));

    msginfo->from  = armci_me;
    msginfo->to    = SERVER_NODE(armci_clus_me);
    msginfo->dscrlen = sizeof(long)+sizeof(void *);
    msginfo->datalen = sizeof(ARMCI_MEMHDL_T);
    msginfo->operation =  REGISTER;
    msginfo->bytes = msginfo->dscrlen+ msginfo->datalen;
    msginfo->tag.ack = 0;
    buf = (char *)(msginfo+1);
    ADDBUF(buf,void*,ptr);
    ADDBUF(buf,long,sz);
    armci_send_req(armci_master, msginfo, bufsize);
    buf= armci_rcv_data(armci_master, msginfo);  /* receive response */
    armci_copy(buf,memhdl,sizeof(ARMCI_MEMHDL_T));
    FREE_SEND_BUFFER(msginfo);

    if(DEBUG_){
          printf("%d:client register req sent ptr=%p %d bytes\n",armci_me,
                  buf,bufsize);fflush(stdout);
    }
}
#endif

/*\ control message to the server, e.g.: ATTACH to shmem, return ptr etc.
\*/
void armci_serv_attach_req(void *info, int ilen, long size, void* resp,int rlen)
{
char *buf;
int bufsize = sizeof(request_header_t)+ilen + sizeof(long)+sizeof(rlen);
request_header_t *msginfo = (request_header_t*)GET_SEND_BUFFER(bufsize,ATTACH,armci_me);
    bzero(msginfo,sizeof(request_header_t));

    msginfo->from  = armci_me;
    msginfo->to    = SERVER_NODE(armci_clus_me);
    msginfo->dscrlen   = ilen;
    msginfo->datalen = sizeof(long)+sizeof(rlen);
    msginfo->operation =  ATTACH;
    msginfo->bytes = msginfo->dscrlen+ msginfo->datalen;

    armci_copy(info, msginfo +1, ilen);
    buf = ((char*)msginfo) + ilen + sizeof(request_header_t);
    *((long*)buf) =size;
    *(int*)(buf+ sizeof(long)) =rlen;
    armci_send_req(armci_master, msginfo, bufsize);
    if(rlen){
      msginfo->datalen = rlen;
      buf= armci_rcv_data(armci_master, msginfo);  /* receive response */
      armci_copy(buf, resp, rlen);
      FREE_SEND_BUFFER(msginfo);

      if(DEBUG_){printf("%d:client attaching got ptr=%p %d bytes\n",armci_me,buf,rlen);
         fflush(stdout);
      }
    }
}


/*\ server initializes its copy of the memory lock data structures
\*/
static void server_alloc_memlock(void *ptr_myclus)
{
int i;

    /* for protection, set pointers for processes outside local node NULL */
    memlock_table_array = calloc(armci_nproc,sizeof(void*));
    if(!memlock_table_array) armci_die("malloc failed for ARMCI lock array",0);

    /* set pointers for processes on local cluster node
     * ptr_myclus - corresponds to the master process
     */
    for(i=0; i< armci_clus_info[armci_clus_me].nslave; i++){
        memlock_table_array[armci_master +i] = ((char*)ptr_myclus)
                + MAX_SLOTS*sizeof(memlock_t)*i;
    }

    /* set pointer to the use flag */
#ifdef MEMLOCK_SHMEM_FLAG
    armci_use_memlock_table = (int*) (MAX_SLOTS*sizeof(memlock_t) +
                      (char*) memlock_table_array[armci_clus_last]);
    
    if(DEBUG_)
      fprintf(stderr,"server initialized memlock %p\n",armci_use_memlock_table);
#endif
}


static int allocate_memlock=1;

/*\ server actions triggered by client request to ATTACH
\*/
void armci_server_ipc(request_header_t* msginfo, void* descr,
                      void* buffer, int buflen)
{
   double *ptr;
   long *idlist = (long*)descr;
   long size = *(long*)buffer;
   int rlen = *(int*)(sizeof(long)+(char*)buffer);
   if(size<0) armci_die("armci_server_ipc: size<0",(int)size);
	ptr=(double*)Attach_Shared_Region(idlist+1,size,idlist[0]);
   if(!ptr)armci_die("armci_server_ipc: failed to attach",0);
   /* provide data server with access to the memory lock data structures */
   if(allocate_memlock){
      allocate_memlock = 0;
      server_alloc_memlock(ptr);
   }
   if(size>0)armci_set_mem_offset(ptr);
   if(msginfo->datalen != sizeof(long)+sizeof(int))
      armci_die("armci_server_ipc: bad msginfo->datalen ",msginfo->datalen);

   if(rlen==sizeof(ptr)){
     msginfo->datalen = rlen;
     armci_send_data(msginfo, &ptr);
   }else armci_die("armci_server_ipc: bad rlen",rlen);
}


/*\ send RMW request to server
\*/
void armci_rem_rmw(int op, int *ploc, int *prem, int extra, int proc)
{
request_header_t *msginfo;
char *buf;
void *buffer;
int bufsize = sizeof(request_header_t)+sizeof(long)+sizeof(void*);
 
    msginfo = (request_header_t*)GET_SEND_BUFFER(bufsize,op,proc);
    bzero(msginfo,sizeof(request_header_t));

    msginfo->dscrlen = sizeof(void*);
    msginfo->from  = armci_me;
    msginfo->to    = proc; 
    msginfo->operation = op;
    msginfo->datalen = sizeof(long);

    buf = (char*)(msginfo+1);
    ADDBUF(buf, void*, prem); /* pointer is shipped as descriptor */

    /* data field: extra argument in fetch&add and local value in swap */
    if(op==ARMCI_SWAP){
       ADDBUF(buf, int, *ploc); 
    }else if(op==ARMCI_SWAP_LONG) {
       ADDBUF(buf, long, *((long*)ploc) ); 
       msginfo->datalen = sizeof(long);
    }else {
       ADDBUF(buf, int, extra);
    }

    msginfo->bytes   = msginfo->datalen+msginfo->dscrlen ;

    if(DEBUG_){
        printf("%d sending RMW request %d to %d\n",armci_me,op,proc);
        fflush(stdout);
    }
    armci_send_req(proc, msginfo, bufsize);

    buffer = armci_rcv_data(proc,msginfo);  /* receive response */

    if(op==ARMCI_FETCH_AND_ADD || op== ARMCI_SWAP)
        *ploc = *(int*)buffer;
    else
        *(long*)ploc = *(long*)buffer;

    FREE_SEND_BUFFER(msginfo);
}


/*\ server response to RMW 
\*/
void armci_server_rmw(request_header_t* msginfo,void* ptr, void* pextra)
{
     long lold;
     int iold;
     void *pold=0;
     int op = msginfo->operation;

     if(DEBUG_){
        printf("%d server: executing RMW from %d\n",armci_me,msginfo->from);
        fflush(stdout);
     }
     if(msginfo->datalen != sizeof(long))
          armci_die2("armci_server_rmw: bad datalen=",msginfo->datalen,op);

     /* for swap operations *pextra has the  value to swap
      * for fetc&add it carries the increment argument
      */
     switch(op){
     case ARMCI_SWAP:
        iold = *(int*) pextra;
     case ARMCI_FETCH_AND_ADD:
        pold = &iold;
        break;

     case ARMCI_SWAP_LONG:
        lold = *(long*) pextra;
     case ARMCI_FETCH_AND_ADD_LONG:
        pold = &lold;
        break;

     default:
          armci_die("armci_server_rmw: bad operation code=",op);
     }

     armci_generic_rmw(op, pold, *(int**)ptr, *(int*) pextra, msginfo->to);

     armci_send_data(msginfo, pold);
}


extern int armci_direct_vector(request_header_t *msginfo , armci_giov_t darr[], int len, int proc);
int armci_rem_vector(int op, void *scale, armci_giov_t darr[],int len,int proc,int flag, armci_ihdl_t nb_handle)
{
    char *buf,*buf0;
    request_header_t *msginfo;
    int bytes =0, s, slen=0;
    size_t adr;
    int bufsize = sizeof(request_header_t),isnonblocking=0;
    
    if(nb_handle)isnonblocking=1;

    /* compute size of the buffer needed */
    for(s=0; s<len; s++){
        bytes   += darr[s].ptr_array_len * darr[s].bytes; /* data */
        bufsize += darr[s].ptr_array_len *sizeof(void*)+2*sizeof(int); /*descr*/
    }
            
    bufsize += bytes + sizeof(long) +2*sizeof(double) +8; /*+scale+allignment*/
#if defined(USE_SOCKET_VECTOR_API) 
    if(flag){
        int totaliovecs=MAX_IOVEC;
        /*if(op==PUT)*/bufsize-=bytes; 
        buf = buf0= GET_SEND_BUFFER((bufsize+sizeof(struct iovec)*totaliovecs),op,proc);
    }
    else
#endif
    {
        buf = buf0= GET_SEND_BUFFER(bufsize,op,proc);
    }
    msginfo = (request_header_t*)buf;
    bzero(msginfo,sizeof(request_header_t));

    if(nb_handle){
      INIT_SENDBUF_INFO(nb_handle,buf,op,proc);
      _armci_buf_set_tag(buf,nb_handle->tag,0);  
      if(nb_handle->bufid == NB_NONE)
        armci_set_nbhandle_bufid(nb_handle,buf,0);
    }

    buf += sizeof(request_header_t);

    /* fill vector descriptor */
    armci_save_vector_dscr(&buf,darr,len,op,0);
    
    /* align buf for doubles (8-bytes) before copying data */
    adr = (size_t)buf;
    adr >>=3;
    adr <<=3;
    adr +=8;
    buf = (char*)adr;
    
    msginfo->ehlen = 0;

    /* fill message header */
    msginfo->dscrlen = buf - buf0 - sizeof(request_header_t);
    msginfo->from  = armci_me;
    msginfo->to    = proc;
    msginfo->operation  = op;
    msginfo->format  = VECTOR;
    msginfo->datalen = bytes;

    /* put scale for accumulate */
    switch(op){
    case ARMCI_ACC_INT:
               *(int*)buf = *(int*)scale; slen= sizeof(int); break;
    case ARMCI_ACC_DCP:
               ((double*)buf)[0] = ((double*)scale)[0];
               ((double*)buf)[1] = ((double*)scale)[1];
               slen=2*sizeof(double);break;
    case ARMCI_ACC_DBL:
               *(double*)buf = *(double*)scale; slen = sizeof(double); break;
    case ARMCI_ACC_CPL:
               ((float*)buf)[0] = ((float*)scale)[0];
               ((float*)buf)[1] = ((float*)scale)[1];
               slen=2*sizeof(float);break; 
    case ARMCI_ACC_FLT:
               *(float*)buf = *(float*)scale; slen = sizeof(float); break;
    default: slen=0;
    }
    buf += slen;
    msginfo->datalen += slen;
    msginfo->bytes = msginfo->datalen+msginfo->dscrlen;

#if defined(USE_SOCKET_VECTOR_API) 
    if(flag&&(op==GET||op==PUT)){
    	armci_direct_vector(msginfo,darr,len,proc);
        return 0;
    }   
#endif 
    /* for put and accumulate copy data into buffer */
    if(op != GET){
/*       fprintf(stderr,"sending %lf\n",*(double*)darr[0].src_ptr_array[0]);*/
       
       armci_vector_to_buf(darr, len, buf);
    }
#ifdef VAPI
    else{
    if(msginfo->dscrlen < (bytes - sizeof(int)))
       *(int*)(((char*)(msginfo+1))+(bytes-sizeof(int))) = ARMCI_VAPI_COMPLETE;
    else
       *(int*)(((char*)(msginfo+1))+(msginfo->dscrlen+bytes-sizeof(int))) = ARMCI_VAPI_COMPLETE;
    }
#endif

    armci_send_req(proc, msginfo, bufsize);
    if(nb_handle && op==GET)armci_save_vector_dscr(&buf0,darr,len,op,1);
    if(op == GET 
#   if !defined(SOCKETS) 
       && !nb_handle
#   endif 
      ){
       armci_complete_vector_get(darr,len,msginfo);
    }
    return 0;
}

#define CHUN_ (8*8096)
#define CHUN 200000

/*\ client version of remote strided operation
\*/
int armci_rem_strided(int op, void* scale, int proc,
                       void *src_ptr, int src_stride_arr[],
                       void* dst_ptr, int dst_stride_arr[],
                       int count[], int stride_levels, 
                       ext_header_t *h, int flag,armci_ihdl_t nb_handle) 
{
    char *buf, *buf0;
    request_header_t *msginfo;
    int  i, slen=0, bytes;
    void *rem_ptr;
    int  *rem_stride_arr;
    int bufsize = sizeof(request_header_t);
    int ehlen =0;

    /* we send ext header only for last chunk */
#if 0
    if(h)  ehlen = h->len;
#else
    if(h) if(h->last)  ehlen = h->len;
#endif
    if(ehlen>MAX_EHLEN || ehlen <0) 
       armci_die2("armci_rem_strided ehlen out of range",MAX_EHLEN,ehlen);
    /* calculate size of the buffer needed */
    for(i=0, bytes=1;i<=stride_levels;i++)bytes*=count[i];
    bufsize += bytes+sizeof(void*)+2*sizeof(int)*(stride_levels+1) +ehlen
               +2*sizeof(double) + 16; /* +scale+alignment */

    if (flag){
#   ifdef CLIENT_BUF_BYPASS
      if(_armci_bypass) bufsize -=bytes; /* we are not sending data*/
#   elif defined(USE_SOCKET_VECTOR_API)
      bufsize -=bytes; /* we are not sending data*/
      bufsize += sizeof(struct iovec)*MAX_IOVEC;
#   else
      if(op==GET)bufsize -=bytes;
#   endif
    }
    buf = buf0= GET_SEND_BUFFER(bufsize,op,proc);
    if(nb_handle)
#ifdef ACC_SMP
	 if(!ACC(op))
#endif
    {
      INIT_SENDBUF_INFO(nb_handle,buf,op,proc);
      _armci_buf_set_tag(buf,nb_handle->tag,0);  
      if(nb_handle->bufid == NB_NONE)
        armci_set_nbhandle_bufid(nb_handle,buf,0);
    }
    msginfo = (request_header_t*)buf;
    bzero(msginfo,sizeof(request_header_t));

    if(op == GET){
       rem_ptr = src_ptr;
       rem_stride_arr = src_stride_arr;
    }else{
       rem_ptr = dst_ptr;
       rem_stride_arr = dst_stride_arr;
    }
     
    msginfo->datalen=bytes;  
#if defined(USE_SOCKET_VECTOR_API) 
    /*****for making put use readv/writev on sockets*****/
    if(op==PUT && flag)
       msginfo->datalen=0;
#endif

    /* fill strided descriptor */
    buf += sizeof(request_header_t);
    /*this function fills the dscr into buf and also moves the buf ptr to the 
      end of the dscr*/
    armci_save_strided_dscr(&buf,rem_ptr,rem_stride_arr,count,stride_levels,0);

#   ifdef CLIENT_BUF_BYPASS
      if(flag && _armci_bypass){
         /* to bypass the client MessageSnd buffer in get we need to add source
            pointer and stride info - server will put data directly there */
         ADDBUF(buf,void*,dst_ptr);
         for(i=0;i<stride_levels;i++)((int*)buf)[i] = dst_stride_arr[i];
                                       buf += stride_levels*sizeof(int);
         msginfo->bypass=1;
         msginfo->pinned=0; /* if set then pin is done before sending req*/
      }else{
         msginfo->bypass=0;
         msginfo->pinned=0;
      }
#   endif
#ifdef LAPI2
         msginfo->bypass=0;
         msginfo->pinned=0;
#endif
  


    /* align buf for doubles (8-bytes) before copying data */
    ALLIGN8(buf);
    
    /* fill message header */
    msginfo->from   = armci_me;
    msginfo->to     = proc;
    msginfo->format = STRIDED;
    msginfo->operation  = op;

    /* put scale for accumulate */
    switch(op){
    case ARMCI_ACC_INT: 
               *(int*)buf = *(int*)scale; slen= sizeof(int); break;
    case ARMCI_ACC_DCP:
               ((double*)buf)[0] = ((double*)scale)[0];
               ((double*)buf)[1] = ((double*)scale)[1];
               slen=2*sizeof(double);break; 
    case ARMCI_ACC_DBL: 
               *(double*)buf = *(double*)scale; slen = sizeof(double); break;
    case ARMCI_ACC_CPL:
               ((float*)buf)[0] = ((float*)scale)[0];
               ((float*)buf)[1] = ((float*)scale)[1];
               slen=2*sizeof(float);break; 
    case ARMCI_ACC_FLT:
               *(float*)buf = *(float*)scale; slen = sizeof(float); break;
    case ARMCI_ACC_LNG:
               *(long*)buf = *(long*)scale; slen = sizeof(long); break;
    default: slen=0;
    }
	
    /*	
	if(ACC(op))printf("%d client len=%d alpha=%lf data=%lf,%lf\n",
	     armci_me, buf-(char*)msginfo,((double*)buf)[0],*((double*)src_ptr),             ((double*)buf)[1]);
    */

    buf += slen;

    /**** add extended header *******/
    if(ehlen){
       bcopy(h->exthdr,buf,ehlen);
       i = ehlen%8; ehlen += (8-i); /* make sure buffer is still alligned */
       buf += ehlen;
    }

    msginfo->ehlen  = ehlen;
    msginfo->dscrlen = buf - buf0 - sizeof(request_header_t);
    msginfo->bytes = msginfo->datalen+msginfo->dscrlen;

    if(op == GET){
#ifdef VAPI
       if(msginfo->dscrlen < (bytes - sizeof(int)))
          *(int*)(((char*)(msginfo+1))+(bytes-sizeof(int)))=ARMCI_VAPI_COMPLETE;
       else
          *(int*)(((char*)(msginfo+1))+(msginfo->dscrlen+bytes-sizeof(int))) = 
                  ARMCI_VAPI_COMPLETE;
#endif            
#      if defined(CLIENT_BUF_BYPASS) 
         if(msginfo->bypass){

#ifdef MULTISTEP_PIN
          if(stride_levels==0 && !msginfo->pinned && count[0]>=400000){
             int seq=1;
             armci_send_req(proc,msginfo,bufsize);
             for(i=0; i< bytes; i+=CHUN){
                  int len= MIN(CHUN,(bytes-i));
                  char *p = i +(char*)dst_ptr;
  
                  armci_pin_contig(p, len);
                  armci_client_send_ack(proc, seq);
                  seq++;
             }
          }else
#endif
          {
             int armci_pin_memory(void *,int *,int *,int);
             void armci_client_send_ack(int, int);
             if(!msginfo->pinned) armci_send_req(proc,msginfo,bufsize);
             if(!armci_pin_memory(dst_ptr,dst_stride_arr,count, stride_levels)){
               armci_client_send_ack(proc, -1);
               armci_rcv_strided_data_bypass(proc,msginfo,dst_ptr,stride_levels);
               FREE_SEND_BUFFER(msginfo);
               return 1; /* failed:cannot do bypass */
             }

             if(msginfo->pinned) armci_send_req(proc,msginfo,bufsize);
             else armci_client_send_ack(proc, 1);
             /*if(nb_handle)
               armci_save_strided_dscr(&buf,dst_ptr,dst_stride_arr,count,
                                       stride_levels,1);*/
          }

         }else
#      endif             
       {
          armci_send_req(proc, msginfo, bufsize);
       }
#     if !defined(SOCKETS)
       if(nb_handle){
#ifdef ACC_SMP
	 if(!ACC(op))
#endif
         armci_save_strided_dscr(&buf0,dst_ptr,dst_stride_arr,count,
                                 stride_levels,1);
       }
       else
#     endif 
       {
         armci_rcv_strided_data(proc, msginfo, msginfo->datalen,
                              dst_ptr, stride_levels, dst_stride_arr, count);
         FREE_SEND_BUFFER(msginfo);
       }
    } else{
       /* for put and accumulate send data */
       armci_send_strided(proc,msginfo, buf, 
                          src_ptr, stride_levels, src_stride_arr, count); 
    }

    return 0;
}


#if defined(ALLOW_PIN) && defined(VAPI)
/*\
 * two phase send
\*/
int armci_two_phase_send(int proc,void *src_ptr,int src_stride_arr[],
                void *dst_ptr,int dst_stride_arr[],int count[],
                int stride_levels,void ** context_ptr,armci_ihdl_t nbhandle, 
                ARMCI_MEMHDL_T *mhloc)
{ 
char *buf, *buf0;
request_header_t *msginfo;
int bytes, i;
int ehlen = 0,nbtag=0;
int *rem_ptr;
int * rem_stride_arr;
int bufsize = sizeof(request_header_t);
void armci_post_gather(void *, int *, int *,int, armci_vapi_memhndl_t *,int,int,int,NB_CMPL_T *);
     
    bytes = 0;
    if(nbhandle)nbtag = nbhandle->tag;     

    /*calculate the size of buffer needed */
    bufsize += bytes+sizeof(void *) + 2*sizeof(int)*(stride_levels+1) + ehlen
				                  +2*sizeof(double) + 16;

    buf = buf0 = GET_SEND_BUFFER(bufsize,PUT,proc);
    msginfo = (request_header_t*)buf;
    buf += sizeof(request_header_t);
					  
    rem_ptr = dst_ptr;
    rem_stride_arr = dst_stride_arr;

    armci_save_strided_dscr(&buf,rem_ptr,rem_stride_arr,count,stride_levels,0);
          
    if(DEBUG_){
       printf(" CLIENT :the dest_ptr is %p src is %p\n", rem_ptr,src_ptr);
       for(i =0; i<stride_levels; i++)
	 printf("the value of stride_arr[i] is %d,value of count[i] is %d\n",
                               rem_stride_arr[i], count[i]);
       printf("the value of stride_levels is %d\n", stride_levels);
       fflush(stdout);	    
    }
    /* fill message header */
    msginfo->datalen = 0;
    msginfo->bypass = 1;
    msginfo->pinned = 1;
    msginfo->from = armci_me;
    msginfo->to = proc;
    msginfo->format = STRIDED;
    msginfo->operation = PUT;
    msginfo->ehlen = 0;
    msginfo->dscrlen = buf - buf0 - sizeof(request_header_t);
    msginfo->bytes = msginfo->datalen + msginfo->dscrlen;
    /* I have not set msginfo->tag */

    /* send the first phase request */
    armci_send_req(proc, msginfo, bufsize); 

    if(DEBUG_){
        printf("%d:CLIENT : finished sending first put request \n",armci_me);
        fflush(stdout);
    }

    armci_wait_ack(buf0);

    if(DEBUG_){
       printf("\n%d: client got ack about to post gather\n",armci_me);
       fflush(stdout);
    }        

    /* 
       the client is now in the second phase, in a loop 
       creates the gather descr one at a time and posts them 
    */

    armci_post_gather(src_ptr,src_stride_arr,count,stride_levels,
                     mhloc,proc,nbtag,CLN,&nbhandle->cmpl_info);

    if(DEBUG_){
       printf("%d(c) : returned from armci_post_gather\n",armci_me);
       fflush(stdout);
    }
    if(nbhandle){
       BUF_INFO_T *info=NULL;
       info=BUF_TO_BUFINFO(buf0);
       info->protocol=0;
    }

    FREE_SEND_BUFFER(msginfo);

    return 0;

}


int armci_two_phase_get(int proc, void*src_ptr, int src_stride_arr[], 
                void*dst_ptr,int dst_stride_arr[], int count[], 
                int stride_levels, void**context_ptr,
                armci_ihdl_t nb_handle, ARMCI_MEMHDL_T *mhloc)
{
char *buf, *buf0;
request_header_t *msginfo;
int bytes;
int ehlen = 0,nbtag=0;
int *rem_ptr;
int num; 
int *rem_stride_arr;
int bufsize = sizeof(request_header_t);
extern void armci_post_scatter(void *,int *,int *,int, armci_vapi_memhndl_t *,int,int,int,NB_CMPL_T *);
extern void armci_wait_for_blocking_scatter();

    if(DEBUG_){
       printf("%d(c):about to call armci_post_scatter, CLN value is %d\n",
                      armci_me,CLN);
       fflush(stdout);
    }    

    if(nb_handle)nbtag = nb_handle->tag;     
    
    armci_post_scatter(dst_ptr, dst_stride_arr, count, stride_levels,mhloc,
                    proc,nbtag,CLN,&nb_handle->cmpl_info);

    if(DEBUG_){
       printf("\n%d: returned from armci_post_scatter %d\n",armci_me,num);
       fflush(stdout);
    }   

    bytes = 0; 
    bufsize += bytes+sizeof(void *) + 2*sizeof(int)*(stride_levels+1) + ehlen
                           +2*sizeof(int) +2*sizeof(double) + 16;

    buf = buf0 = GET_SEND_BUFFER(bufsize,GET,proc);

    msginfo = (request_header_t *)buf;  
    buf += sizeof(request_header_t);

    rem_ptr = src_ptr;
    rem_stride_arr = src_stride_arr;

    /*this call is to put the remote descriptor into the buffer to send*/
    armci_save_strided_dscr(&buf,rem_ptr,rem_stride_arr,count,stride_levels,0);

    msginfo->datalen = 0;
    msginfo->bypass = 1;
    msginfo->pinned = 1;
    msginfo->from = armci_me;
    msginfo->to = proc;
    msginfo->format = STRIDED;
    msginfo->operation = GET;
    msginfo->ehlen = 0;
    msginfo->dscrlen = buf - buf0 - sizeof(request_header_t);
    msginfo->bytes = msginfo->datalen + msginfo->dscrlen; 

    /* send the request asking the server to post gather */
    armci_send_req(proc, msginfo, bufsize);
  
    if(DEBUG_){
       printf("%d(c) : finished sending get request to server\n",armci_me);     
       fflush(stdout);
    }

    if(nb_handle){
       BUF_INFO_T *info=NULL;
       info=BUF_TO_BUFINFO(buf0);
       info->protocol=0;
    }
    else{
       armci_wait_for_blocking_scatter();
    }

    FREE_SEND_BUFFER(msginfo);

    if(DEBUG_){
       printf("%d(c) : finished polling for scatter_recv\n",armci_me);
       fflush(stdout);
    }
  
    return 0;  
}
#endif


#if defined(ALLOW_PIN) || defined(LAPI2)
/*\ client version of remote strided get
\*/
int armci_rem_get(int proc,
                  void *src_ptr, int src_stride_arr[],
                  void* dst_ptr, int dst_stride_arr[],
                  int count[], int stride_levels,
                  armci_ihdl_t nb_handle,void *mhloc,void *mhrem)
{
    char *buf, *buf0;
    request_header_t *msginfo;
    int  i, bytes;
    void *rem_ptr;
    int  *rem_stride_arr;
    int bufsize = sizeof(request_header_t);

    /* calculate size of the buffer needed */
    for(i=0, bytes=1;i<=stride_levels;i++)bytes*=count[i];
    bufsize += sizeof(void*)+2*sizeof(int)*(stride_levels+1) 
               +2*sizeof(double) + 16; /* +scale+alignment */
#ifdef VAPI
    /*need to send the rkey and lkey*/
    /* lkey and rkey are unsigned its, but we cant trust it to stay like that*/
# ifdef OPENIB
    bufsize +=(sizeof(uint32_t)+sizeof(uint32_t));
# else
    bufsize +=(sizeof(VAPI_lkey_t)+sizeof(VAPI_rkey_t));
    /*bufsize +=(sizeof(uint32_t)+sizeof(uint32_t));*/
# endif
#endif

    buf = buf0= GET_SEND_BUFFER(bufsize,GET,proc);
    if(nb_handle){
      INIT_SENDBUF_INFO(nb_handle,buf,op,proc);
      _armci_buf_set_tag(buf,nb_handle->tag,0);
      if(nb_handle->bufid == NB_NONE)
        armci_set_nbhandle_bufid(nb_handle,buf,0);
    }

    msginfo = (request_header_t*)buf;
    buf += sizeof(request_header_t);

    rem_ptr = src_ptr;
    rem_stride_arr = src_stride_arr;
    /*this function fills the dscr into buf and also moves the buf ptr to the 
      end of the dscr*/
    armci_save_strided_dscr(&buf,rem_ptr,rem_stride_arr,count,stride_levels,0);


    /* to bypass the client MessageSnd buffer in get we need to add source
       pointer and stride info - server will put data directly there */
    ADDBUF(buf,void*,dst_ptr);
#ifdef VAPI
# ifdef OPENIB
    ADDBUF(buf,uint32_t,((ARMCI_MEMHDL_T *)mhloc)->rkey);
    ADDBUF(buf,uint32_t,((ARMCI_MEMHDL_T *)mhrem)->lkey);
#else
    ADDBUF(buf,VAPI_rkey_t,((ARMCI_MEMHDL_T *)mhloc)->rkey);
    ADDBUF(buf,VAPI_lkey_t,((ARMCI_MEMHDL_T *)mhrem)->lkey);
#endif
#endif
    for(i=0;i<stride_levels;i++)((int*)buf)[i] = dst_stride_arr[i];
                                       buf += stride_levels*sizeof(int);
    /* fill message header */
    msginfo->bypass=1;
    msginfo->pinned=1;
    msginfo->from   = armci_me;
    msginfo->to     = proc;
    msginfo->format = STRIDED;
    msginfo->operation  = GET;
    msginfo->ehlen  = 0;
    msginfo->datalen=0;
    msginfo->dscrlen = buf - buf0 - sizeof(request_header_t);
    msginfo->bytes = msginfo->dscrlen;


#if defined(GM) || defined(VAPI) || defined(QUADRICS)
    /* prepare for  set the stamp at the end of the user buffer */
    if(count[0]<sizeof(int))armci_die("armci_rem_get: wrong protocol",count[0]);
#  ifdef GM
    *(int*)(((char*)(dst_ptr)) + (count[0] -sizeof(int))) = ARMCI_GM_COMPLETE;
#  else
    *(int*)(((char*)(dst_ptr)) + (count[0] -sizeof(int))) = ARMCI_VAPI_COMPLETE;
#  endif
#endif

    armci_send_req(proc,msginfo,bufsize);

    if(nb_handle) {
         armci_save_strided_dscr(&buf0,dst_ptr,dst_stride_arr,count,
                                 stride_levels,1);
    }
    else {
       armci_rcv_strided_data_bypass_both(proc,msginfo,dst_ptr,count,
		                          stride_levels);
       FREE_SEND_BUFFER(msginfo);
    }
    return 0;
}
#endif


void armci_process_extheader(request_header_t *msginfo, char *dscr, char* buf, int buflen)
{
 armci_flag_t *h;
 int *flag;

   h = (armci_flag_t*)(dscr + msginfo->dscrlen - msginfo->ehlen);
#if 0
   if(msginfo->ehlen)printf("%d:server from=%d len=%d: ptr=%p val=%d\n",armci_me,msginfo->from, msginfo->ehlen,h->ptr,h->val);
   fflush(stdout);
#endif
   flag = (int*)(h->ptr);
   *flag = h->val;
}

void armci_server(request_header_t *msginfo, char *dscr, char* buf, int buflen)
{
    int  buf_stride_arr[MAX_STRIDE_LEVEL+1];
    int  *loc_stride_arr,slen; 
    int  *count, stride_levels;
    void *buf_ptr, *loc_ptr;
    void *scale;
    char *dscr_save = dscr;
    int  rc, i,proc;
#   if defined(CLIENT_BUF_BYPASS) || defined(LAPI2) 
      int  *client_stride_arr=0; 
      void *client_ptr=0;
#   endif

    /*return if using readv/socket for put*/
    if(msginfo->operation==PUT && msginfo->datalen==0){
      if(msginfo->ehlen) /* process extra header if available */
         armci_process_extheader(msginfo, dscr, buf, buflen);
      return;
    }

    /* unpack descriptor record */
    loc_ptr = *(void**)dscr;           dscr += sizeof(void*);
    stride_levels = *(int*)dscr;       dscr += sizeof(int);
    loc_stride_arr = (int*)dscr;       dscr += stride_levels*sizeof(int);
    count = (int*)dscr;                

    /* compute stride array for buffer */
    buf_stride_arr[0]=count[0];
    for(i=0; i< stride_levels; i++)
        buf_stride_arr[i+1]= buf_stride_arr[i]*count[i+1];

#   if defined(CLIENT_BUF_BYPASS) || defined(LAPI2) 
       if(msginfo->bypass){
          dscr += (1+stride_levels)*sizeof(int); /* move past count */
          GETBUF(dscr,void*,client_ptr);
          client_stride_arr = (int*)dscr; dscr += stride_levels*sizeof(int);
        }
#   endif


    /* get scale for accumulate, adjust buf to point to data */
    switch(msginfo->operation){
    case ARMCI_ACC_INT:     slen = sizeof(int); break;
    case ARMCI_ACC_DCP:     slen = 2*sizeof(double); break;
    case ARMCI_ACC_DBL:     slen = sizeof(double); break;
    case ARMCI_ACC_CPL:     slen = 2*sizeof(float); break;
    case ARMCI_ACC_FLT:     slen = sizeof(float); break;
    case ARMCI_ACC_LNG:     slen = sizeof(long); break;
	default:				slen=0;
    }

	scale = dscr_save+ (msginfo->dscrlen - slen -msginfo->ehlen);
/*
    if(ACC(msginfo->operation))
      fprintf(stderr,"%d in server len=%d slen=%d alpha=%lf data=%lf\n", 
               armci_me, msginfo->dscrlen, slen, *(double*)scale,*(double*)buf);
*/

    buf_ptr = buf; /*  data in buffer */

    proc = msginfo->to;

    if(msginfo->operation == GET){
    
#      if defined(CLIENT_BUF_BYPASS) || defined(LAPI2) || defined(VAPI)
         if(msginfo->bypass){
             armci_send_strided_data_bypass(proc, msginfo, buf, buflen, loc_ptr, loc_stride_arr, 
                       client_ptr, client_stride_arr, count, stride_levels);

         }else
#      endif

       armci_send_strided_data(proc, msginfo, buf,
                               loc_ptr, stride_levels, loc_stride_arr, count); 

    } else{

#ifdef PIPE_BUFSIZE
       if((msginfo->bytes==0) && (msginfo->operation==PUT)){
         armci_pipe_prep_receive_strided(msginfo,buf_ptr,stride_levels,
                    loc_stride_arr, count, buflen); 
         armci_pipe_receive_strided(msginfo,loc_ptr,loc_stride_arr, count,
                    stride_levels);
           
       } else
#endif
       if((rc = armci_op_strided(msginfo->operation, scale, proc,
               buf_ptr, buf_stride_arr, loc_ptr, loc_stride_arr,
               count, stride_levels, 1,NULL)))
               armci_die("server_strided: op from buf failed",rc);
    }

    if(msginfo->ehlen) /* process extra header if available */
         armci_process_extheader(msginfo, dscr_save, buf, buflen);
}



void armci_server_vector( request_header_t *msginfo, 
                          char *dscr, char* buf, int buflen)
{
    int  proc;
    long  len;
    void *scale;
    int  i,s;
    char *sbuf = buf;
    if(msginfo->operation==PUT && msginfo->datalen==0)return;/*return if using readv/socket for put*/
    /* unpack descriptor record */
    GETBUF(dscr, long ,len);
    
    /* get scale for accumulate, adjust buf to point to data */
    scale = buf;
    switch(msginfo->operation){
    case ARMCI_ACC_INT:     buf += sizeof(int); break;
    case ARMCI_ACC_DCP:     buf += 2*sizeof(double); break;
    case ARMCI_ACC_DBL:     buf += sizeof(double); break;
    case ARMCI_ACC_CPL:     buf += 2*sizeof(float); break;
    case ARMCI_ACC_FLT:     buf += sizeof(float); break;
    }

    proc = msginfo->to;

    /*fprintf(stderr,"scale=%lf\n",*(double*)scale);*/
    /* execute the operation */

    switch(msginfo->operation) {
    case GET:
/*        fprintf(stderr, "%d:: Got a vector message!!\n", armci_me); */
      if(msginfo->ehlen) {
#if defined(ARMCI_ENABLE_GPC_CALLS) && (defined(GM) || defined(VAPI) || defined(DOELAN4))
	gpc_call_process(msginfo, len, dscr, buf, buflen, sbuf);
#else
	armci_die("Unexpected vector message with non-zero ehlen. GPC call?",
		   msginfo->ehlen);
#endif
      }
      else {
	for(i = 0; i< len; i++){
	  int parlen, bytes;
	  void **ptr;
	  GETBUF(dscr, int, parlen);
	  GETBUF(dscr, int, bytes);
	  /*        fprintf(stderr,"len=%d bytes=%d parlen=%d\n",len,bytes,parlen);*/
	  ptr = (void**)dscr; dscr += parlen*sizeof(char*);
	  for(s=0; s< parlen; s++){
	    armci_copy(ptr[s], buf, bytes);
	    buf += bytes;
	  }
	}
    
	/*      fprintf(stderr,"server sending buffer %lf\n",*(double*)sbuf);*/
	armci_send_data(msginfo, sbuf);
      }
      break;

    case PUT:

/*    fprintf(stderr,"received in buffer %lf\n",*(double*)buf);*/
      for(i = 0; i< len; i++){
        int parlen, bytes;
        void **ptr;
        GETBUF(dscr, int, parlen);
        GETBUF(dscr, int, bytes);
        ptr = (void**)dscr; dscr += parlen*sizeof(char*);
        for(s=0; s< parlen; s++){
/*
          armci_copy(buf, ptr[s], bytes);
*/
          bcopy(buf, ptr[s], (size_t)bytes);
          buf += bytes;
        }
      }
      break;

     default:

      /* this should be accumulate */
      if(!ACC(msginfo->operation))
               armci_die("v server: wrong op code",msginfo->operation);

/*      fprintf(stderr,"received first=%lf last =%lf in buffer\n",*/
/*                     *((double*)buf),((double*)buf)[99]);*/

      for(i = 0; i< len; i++){
        int parlen, bytes;
        void **ptr;
        GETBUF(dscr, int, parlen);
        GETBUF(dscr, int, bytes);
        ptr = (void**)dscr; dscr += parlen*sizeof(char*);
        armci_lockmem_scatter(ptr, parlen, bytes, proc); 
        for(s=0; s< parlen; s++){
          armci_acc_2D(msginfo->operation, scale, proc, buf, ptr[s],
                       bytes, 1, bytes, bytes, 0);
          buf += bytes;
        }
        ARMCI_UNLOCKMEM(proc);
      }
    }
}

/**Routines for GPC calls**/

/**Server side routine to handle a GPC call request**/
/*===============Register this memory=====================*/
#ifdef ARMCI_ENABLE_GPC_CALLS
#if defined(GM) || defined(VAPI) || defined(QUADRICS)
gpc_buf_t *gpc_req;
/*VT: I made the change below because DATA_SERVER is not defined for elan4
 *VT: This will only be invoked in case of GPC call and should not intefere
 *VT: with any other call
 */
#if (defined(DOELAN4) || defined(DATA_SERVER)) && defined(SERVER_THREAD) 
#  ifdef PTHREADS
pthread_t data_server;
#  else
#    error Threading other than pthreads not yet implemented
#  endif
#endif

void block_thread_signal(int signo) {
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGUSR1);
  pthread_sigmask(SIG_BLOCK, &mask, NULL);
}

void unblock_thread_signal(int signo) {
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGUSR1);
  pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
}

void gpc_init() {
  int i;
  for(i=0; i<MAX_GPC_REQ; i++)
    gpc_req[i].active = 0;
}


void gpc_free_buf_handle(int idx) {
  gpc_req[idx].active = 0;
}

void gpc_completion_scan() {
  int i;

  /***run through gpc buffers and probe for completion**/
  for(i=0;i<MAX_GPC_REQ; i++) {
    gpc_call_t *gcall = &gpc_req[i].call;
    
    if(!gpc_req[i].active)
      continue;

    if(armci_gpc_local_exec(gcall->hndl, 
			    gpc_req[i].msginfo.to, gpc_req[i].msginfo.from, 
			    gcall->hdr, gcall->hlen, gcall->data, gcall->dlen, 
			    gcall->rhdr, gcall->rhlen, gcall->rdata, gcall->rdlen,
			    GPC_PROBE) == GPC_DONE) {
      armci_send_data(&gpc_req[i].msginfo, gpc_req[i].reply);
      gpc_free_buf_handle(i);
    }
  }
}

void gpc_completion_handler(int sig) {
  if(sig != SIGUSR1) 
    armci_die("gpc_completion_handler. Invoked with unexpected signal", sig);

  if(!pthread_equal(pthread_self(), data_server)) 
    armci_die("Signal in a thread other than the data server!!!", sig);

/*    fprintf(stderr, "%d::SIGNAL\n", armci_me); */
  gpc_completion_scan();
}

void gpc_init_signals() {
  struct sigaction action;

  /*register signal handler**/
  action.sa_handler = gpc_completion_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;
  sigaction(SIGUSR1, &action, NULL);

  /*Block the GPC completion signal. 
    data server unblocks it later for itself*/
/*    block_thread_signal(SIGUSR1); */

#if 0
  /*Mask this signal on main thread. Should only execute in the
    context of the data server.*/ 
  if(pthread_self() == armci_usr_tid) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
  }
#endif
}

int gpc_get_buf_handle() {
  int i;
  gpc_call_t *gcall;
  
  for(i=0; i<MAX_GPC_REQ && gpc_req[i].active==1; i++);

  if(i==MAX_GPC_REQ) { /**wait for a buffer to be free**/
    i = rand()%MAX_GPC_REQ;
    gcall = &gpc_req[i].call;

    if(armci_gpc_local_exec(gcall->hndl, 
			    gpc_req[i].msginfo.to, gpc_req[i].msginfo.from, 
			    gcall->hdr, gcall->hlen, gcall->data, gcall->dlen, 
			    gcall->rhdr, gcall->rhlen, gcall->rdata, gcall->rdlen,
			    GPC_WAIT) != GPC_DONE) 
      armci_die("Wait on GPC call completion failed", 0);
    armci_send_data(&gpc_req[i].msginfo, gpc_req[i].reply);
    gpc_free_buf_handle(i);
  }
  
  gpc_req[i].active = 1;
  return i;
}

int gpc_call_process( request_header_t *msginfo, int len,
                          char *dscr, char* buf, int buflen, char *sbuf) {
  int h, hlen, dlen, rhlen, rdlen;
  int parlen;
  int rbuf;
  void *hdr, *data, *rhdr, *rdata;
  gpc_buf_t *gbuf;
  gpc_call_t *gcall;

  GETBUF(dscr, int, parlen);
  if(parlen != 1)
    armci_die("gpc_call_process: Invalid parlen in dscr", parlen);
  GETBUF(dscr, int, rhlen);
  dscr += parlen*sizeof(char*);
  GETBUF(dscr, int, parlen); 
  if(parlen != 1)
    armci_die("gpc_call_process: Invalid parlen in dscr", parlen);
  GETBUF(dscr, int, rdlen);
  dscr += parlen*sizeof(char *);
	
  GETBUF(dscr, int, h);
  GETBUF(dscr, int, hlen);
  hdr = dscr;
  dscr += hlen;
  GETBUF(dscr, int, dlen);
  data = dscr;
  dscr += dlen;
  
  rhdr = buf;
  rdata = (char *)rhdr + rhlen;

  rbuf = gpc_get_buf_handle();
  gbuf = &gpc_req[rbuf];

  gbuf->msginfo = *msginfo;
  gbuf->call.hndl = h;
  gbuf->call.hlen = hlen;
  gbuf->call.dlen = dlen;
  gbuf->call.rhlen = rhlen;
  gbuf->call.rdlen = rdlen;

  gbuf->call.hdr = gbuf->send;
  gbuf->call.data = gbuf->send + hlen;
  bcopy(hdr, gbuf->call.hdr, gbuf->call.hlen);
  bcopy(data, gbuf->call.data, gbuf->call.dlen);

  gbuf->call.rhdr = gbuf->reply;
  gbuf->call.rdata = gbuf->reply + rhlen;

  gcall = &gbuf->call;

  if(armci_gpc_local_exec(gcall->hndl, gbuf->msginfo.to, gbuf->msginfo.from, 
			  gcall->hdr, gcall->hlen, gcall->data, gcall->dlen, 
			  gcall->rhdr, gcall->rhlen, gcall->rdata, gcall->rdlen,
			  GPC_INIT) == GPC_DONE) {
   armci_send_data(&gbuf->msginfo, gbuf->reply);
   gpc_free_buf_handle(rbuf);
 }
  return 0;
}

/*Based on armci_rem_vector. On the server side, a vector GET request
  with a non-zero ehlen is a GPC call*/
int armci_rem_gpc(int op, armci_giov_t darr[],int len, gpc_send_t *send, 
		  int proc, int flag, armci_ihdl_t nb_handle)
{
    char *buf, *buf0;
    request_header_t *msginfo;
    int bytes =0, s/*, slen=0*/;
    size_t adr;
    int bufsize = sizeof(request_header_t),isnonblocking=0;
    int send_len=0, i;
    void *ptr;
    
    if(nb_handle)isnonblocking=1;

    if(len != 2)
      armci_die("armci_rem_gpc: invalid len parameter", len);

    /* compute size of the buffer needed */
    for(s=0; s<len; s++){
        bytes   += darr[s].ptr_array_len * darr[s].bytes; /* data */
        bufsize += darr[s].ptr_array_len *sizeof(void*)+2*sizeof(int); /*descr*/
    }
    send_len = 3*sizeof(int) + send->hlen + send->dlen;
            
    bufsize += bytes + sizeof(long) + send_len + 2*sizeof(double) + sizeof(double) + 8; /*+scale+allignment*/
#if defined(USE_SOCKET_VECTOR_API) 
    if(flag){
        int totaliovecs=MAX_IOVEC;
        /*if(op==PUT)*/bufsize-=bytes; 
        buf = buf0= GET_SEND_BUFFER((bufsize+sizeof(struct iovec)*totaliovecs),op,proc);
    }
    else
#endif
    {
        buf = buf0= GET_SEND_BUFFER(bufsize,op,proc);
    }
    msginfo = (request_header_t*)buf;
    bzero(msginfo,sizeof(request_header_t));

    if(nb_handle){
      INIT_SENDBUF_INFO(nb_handle,buf,op,proc);
      _armci_buf_set_tag(buf,nb_handle->tag,0);  
      if(nb_handle->bufid == NB_NONE)
        armci_set_nbhandle_bufid(nb_handle,buf,0);
    }

    buf += sizeof(request_header_t);

    /* fill vector descriptor */
    armci_save_vector_dscr(&buf,darr,len,op,0);
    
    /* add data to send*/
    ptr = buf;
    ADDBUF(ptr, int, send->hndl);
    ADDBUF(ptr, int, send->hlen);

    bcopy(send->hdr, ptr, send->hlen);    /*     armci_copy? */
    ptr += send->hlen;

    ADDBUF(ptr, int, send->dlen);
    bcopy(send->data, ptr, send->dlen);    /*     armci_copy? */
  
    i = send_len%8; send_len += 8-i;
    buf += send_len;

    /*Dummy non-zero extended header to identify a GPC call*/
    buf += sizeof(double);
    msginfo->ehlen = sizeof(double);
    
    /* fill message header */
    msginfo->dscrlen = buf - buf0 - sizeof(request_header_t);
    msginfo->from  = armci_me;
    msginfo->to    = proc;
    msginfo->operation  = op;
    msginfo->format  = VECTOR;
    msginfo->datalen = bytes;

    msginfo->bytes = msginfo->datalen+msginfo->dscrlen;

#if defined(USE_SOCKET_VECTOR_API) 
    if(flag&&(op==GET||op==PUT)){
    	armci_direct_vector(msginfo,darr,len,proc);
        return 0;
    }   
#endif 

#ifdef VAPI
    if(op == GET) {
    if(msginfo->dscrlen < (bytes - sizeof(int)))
       *(int*)(((char*)(msginfo+1))+(bytes-sizeof(int))) = ARMCI_VAPI_COMPLETE;
    else
       *(int*)(((char*)(msginfo+1))+(msginfo->dscrlen+bytes-sizeof(int))) = ARMCI_VAPI_COMPLETE;
    }
#endif

    armci_send_req(proc, msginfo, bufsize);
    if(nb_handle && op==GET)armci_save_vector_dscr(&buf0,darr,len,op,1);
    if(op == GET 
#   if !defined(SOCKETS) 
       && !nb_handle
#   endif 
      ){
       armci_complete_vector_get(darr,len,msginfo);
    }
    return 0;
}

#else

/*Empty functions to let unsupported hardware smoothly pass through initialization*/

void gpc_init(void) {}

void gpc_init_signals(void) {}

#endif
#endif

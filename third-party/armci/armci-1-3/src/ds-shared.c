#include "armcip.h"
#include "request.h"
#include "message.h"
#include "memlock.h"
#include "copy.h"
#include "gpc.h"
#include <stdio.h>
#ifdef WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#define DEBUG_ 0
#define DEBUG1 0

#ifndef SERV
#     define SERV 2
#endif

int _armci_server_started=0;

/**************************** pipelining for medium size msg ***********/
#ifdef PIPE_BUFSIZE


static int pack_size(int len)
{
int oldlen = len;
#define PIPE_ROUNDUP 512 
#define PIPE_SHORT_ROUNDUP (1024) 
int n;
 if(len <4*PIPE_BUFSIZE){  
   len /=2;
    n = len%PIPE_SHORT_ROUNDUP;
   if(n)len += (PIPE_SHORT_ROUNDUP-n);
 } 
#if defined(VIA) || defined(VAPI)
 else if(len <25*PIPE_BUFSIZE){
   len /=4;
   n = len%PIPE_SHORT_ROUNDUP;
   if(n)len += (PIPE_SHORT_ROUNDUP-n);
 }
else if(len <41*PIPE_BUFSIZE){
   len /=8;
   n = len%PIPE_SHORT_ROUNDUP;
   if(n)len += (PIPE_SHORT_ROUNDUP-n);
 }
#else
 else if(len <32*PIPE_BUFSIZE){
   len /=8;
   n = len%PIPE_SHORT_ROUNDUP;
   if(n)len += (PIPE_SHORT_ROUNDUP-n); 
 }
#endif 
else 
#if defined(VIA) || defined(VAPI)
   len = 8*4096;
#elif defined(HITACHI)
   len = 128*1024-128;
#else
   len = 64*1024-128;
#endif
#ifdef MAX_PIPELINE_CHUNKS
 if(oldlen/len > MAX_PIPELINE_CHUNKS-1){
  len = oldlen/MAX_PIPELINE_CHUNKS;
  n = len%PIPE_SHORT_ROUNDUP;
  if(n)len += (PIPE_SHORT_ROUNDUP-n);
 }
#endif
 return len;
}
    
#define PACK_SIZE1(_len) ((_len)<PIPE_BUFSIZE)?PIPE_MIN_BUFSIZE:PIPE_BUFSIZE;
#define PACK_SIZE(_len) pack_size(_len) 

void armci_pipe_prep_receive_strided(request_header_t *msginfo, char *buf,
                        int strides, int stride_arr[], int count[], int bufsize)
{
buf_arg_t arg;
int  packsize = PACK_SIZE(msginfo->datalen);

     arg.buf_posted = arg.buf   = buf;
#ifdef HITACHI
     arg.count = 0;
#else
     arg.count = bufsize;
#endif
     arg.proc  = (msginfo->operation==GET)?msginfo->to:msginfo->from;
     arg.op    = msginfo->operation;
    
     armci_dispatch_strided(buf, stride_arr, count, strides, -1, -1,
                            packsize, armcill_pipe_post_bufs,&arg);
}

void armci_pipe_receive_strided(request_header_t* msginfo, void *ptr,
                                int stride_arr[], int count[], int strides)
{
buf_arg_t arg;
int  packsize = PACK_SIZE(msginfo->datalen);
#if defined(GM)  
     arg.buf_posted   = msginfo->tag.data_ptr;
#endif
#if (defined(VIA) && defined(VIA_USES_RDMA)) || defined(VAPI)
     arg.buf_posted   = msginfo->tag;
#endif

     arg.buf   = ptr;
     arg.count = 0;
     arg.proc  = (msginfo->operation==GET)?msginfo->to:msginfo->from;
     arg.op    = msginfo->operation;

     armci_dispatch_strided(ptr, stride_arr, count, strides, -1, -1,
                            packsize, armcill_pipe_extract_data, &arg);
}
    
void armci_pipe_send_strided(request_header_t *msginfo, void *buf, int buflen,
                             void *ptr, int *stride_arr,int count[],int strides)
{
buf_arg_t arg;
int  packsize = PACK_SIZE(msginfo->datalen);

#if defined(GM) || defined(HITACHI)
     arg.buf_posted   = msginfo->tag.data_ptr;
#endif
#if (defined(VIA) && defined(VIA_USES_RDMA)) || defined(VAPI)
     arg.buf_posted   = msginfo->tag;
#endif

     arg.buf   = buf;
     arg.count = 0;
     arg.proc  = (msginfo->operation==GET)?msginfo->from:msginfo->to;
     arg.op    = msginfo->operation;

     armci_dispatch_strided(ptr, stride_arr, count, strides, -1, -1,
                            packsize, armcill_pipe_send_chunk, &arg);
#ifdef GM
     armci_serv_send_nonblocking_complete(0);
#endif
}
#endif
/**************************** end of pipelining for medium size msg ***********/


#if defined(CLIENT_BUF_BYPASS) && !defined(GM)
/**************** NOTE: for now this code can only handle contiguous data *****/
void armci_send_strided_data_bypass(int proc, request_header_t *msginfo,
                                    void *loc_buf, int msg_buflen,
                                    void *loc_ptr, int *loc_stride_arr,
                                    void *rem_ptr, int *rem_stride_arr,
                                    int *count, int stride_levels)
{
    int armcill_server_wait_ack(int,int);
    if(DEBUG_){
      printf("%d(s): strided(%d) get bypass from %d\n",armci_me,stride_levels,
             msginfo->from);
      fflush(stdout);
    }

#ifdef VAPI
    if(stride_levels==0 && msginfo->pinned){
      armci_send_contig_bypass(proc,msginfo,loc_ptr,rem_ptr,count[0]);
      return;
    }
    else {
      armci_die("***Contact Developers with machine/network info at hpctools@emsl.pnl.gov: bypass path wrongly invoked***",0);
    }
#endif
    
    armci_pin_memory(loc_ptr, loc_stride_arr,count, stride_levels);
    /*wait until client ready*/
    if(!armcill_server_wait_ack(msginfo->from,1)){
       /*client was not able to pin memory, it will revert to default protocol
         hence, unpin the memory and leave. */
       armci_unpin_memory(loc_ptr, loc_stride_arr,count, stride_levels);
       return;
    }

    armcill_server_put(msginfo->from,loc_ptr,rem_ptr,count[0]);
    armci_unpin_memory(loc_ptr, loc_stride_arr,count, stride_levels);
    if(DEBUG_){
      printf("%d(s): strided(%d) get bypass done \n",armci_me,stride_levels);
      fflush(stdout);
    }
}
#endif

/*\ client initialization
\*/
void armci_client_code()
{
   if(DEBUG_){
      printf("in client after fork %d(%d)\n",armci_me,getpid()); fflush(stdout);
   }

   armci_client_connect_to_servers();
   armci_msg_barrier();

   if(DEBUG_){
      printf("%d client connected to all %d servers\n",armci_me, armci_nclus-1);
      fflush(stdout);
   }
}


/*\ client sends request to server
\*/
void armci_send_req(int proc, request_header_t* msginfo, int len)
{
int hdrlen = sizeof(request_header_t);
int bytes;

    if(msginfo->operation == GET) {
      if(msginfo->format==VECTOR && msginfo->ehlen > 0) 
	bytes = msginfo->dscrlen + hdrlen + msginfo->datalen;
      else
        bytes = msginfo->dscrlen + hdrlen;
    }
    else
        bytes = msginfo->bytes + hdrlen;

    if(DEBUG_){printf("%d: sending req %d (len=%d dscr=%d data=%d) to %d \n",
               armci_me, msginfo->operation, bytes,msginfo->dscrlen,
               msginfo->datalen,proc); fflush(stdout);
    }
    if(bytes > len)armci_die2("armci_send_req:buffer overflow",bytes,len);

#ifdef PIPE_BUFSIZE
    if(
#   ifdef CLIENT_BUF_BYPASS 
     (!msginfo->bypass) &&
#   endif
     (msginfo->datalen>2*PIPE_MIN_BUFSIZE) && (msginfo->operation == GET)
                                        && (msginfo->format == STRIDED)){
      char *buf = sizeof(void*) + (char*)(msginfo+1);
      int  *ibuf = (int*)buf;
      int  *strides =ibuf;
      int  *stride_arr= ibuf +1;
      int  *count = stride_arr + *strides;
      armci_pipe_prep_receive_strided(msginfo, buf, *strides, stride_arr, count,
                                      len-2**strides*sizeof(int)-sizeof(void*));
      armci_pipe_send_req(proc,msginfo, bytes);
    }else
#endif

       armci_send_req_msg(proc,msginfo, bytes);
}


/*\ client sends strided data + request to server
\*/
void armci_send_strided(int proc, request_header_t *msginfo, char *bdata,
                        void *ptr, int strides, int stride_arr[], int count[])
{
    int hdrlen = sizeof(request_header_t);
    int dscrlen = msginfo->dscrlen;
    int datalen = msginfo->datalen;
    int cluster = armci_clus_id(proc);
    int bytes;

    bytes = msginfo->bytes + hdrlen;

    if(DEBUG_){
       printf("%d:sending strided %d to(%d,%d,%d) bytes=%d dslen=%d dlen=%d,\n",
                armci_me, msginfo->operation, msginfo->to,
                cluster, proc, bytes, dscrlen, datalen); fflush(stdout);
    }

#ifdef SOCKETS
    /* zero-copy optimization for large requests */
    if(count[0] >  TCP_PAYLOAD){
       if(armci_send_req_msg_strided(proc, msginfo,ptr,strides,
          stride_arr, count))armci_die("armci_send_strided_req long: failed",0);
       return; /************** done **************/
    }
#elif defined(PIPE_BUFSIZE___)
    if((msginfo->datalen>2*PIPE_MIN_BUFSIZE) && (msginfo->operation == PUT)){
       msginfo->bytes =0; /*** this tells server that we use pipelined put ****/
       armci_send_req_msg(proc,msginfo, hdrlen+dscrlen);
       armci_pipe_send_strided(msginfo, bdata, datalen,
                               ptr, stride_arr, count, strides);
       return; /************** done **************/
    }
#endif
    /*  copy into a buffer before sending */
   
#  ifdef SERV_BUF_IDX_T
    msginfo->inbuf = armcill_getbidx((msginfo->datalen+msginfo->dscrlen), proc, &msginfo->tag.ack);
    msginfo->tag.ack_ptr = &msginfo->tag.ack;
#  endif
    armci_write_strided(ptr, strides, stride_arr, count, bdata);
    if(armci_send_req_msg(proc,msginfo, bytes))
       armci_die("armci_send_strided_req: failed",0);
}



/*\ client receives data from server
\*/
char *armci_rcv_data(int proc, request_header_t* msginfo )
{
    int datalen = msginfo->datalen;
    char *buf;

    if(DEBUG_) {
        printf("%d:armci_rcv_data:  bytes= %d \n", armci_me, datalen);
        fflush(stdout);
    }

    if(datalen == 0) armci_die("armci_rcv_data: no data to receive",datalen);
    if(datalen > (MSG_BUFLEN-sizeof(request_header_t)-sizeof(long)))
        armci_die("armci_rcv_data:data overflowing rcv buffer",datalen);

    buf = armci_ReadFromDirect(proc, msginfo, datalen);

    if(DEBUG_){
        printf("%d:armci_rcv_data: got %d bytes \n",armci_me,datalen);
        fflush(stdout);
    }

    return(buf);
}


/*\ client receives vector data from server and unpacks to the right loc
\*/
void armci_rcv_vector_data(int proc, request_header_t* msginfo, armci_giov_t darr[], int len)
{
    char *buf = armci_rcv_data(proc, msginfo);
    armci_vector_from_buf(darr, len, buf);
}


/*\ client receives strided data from server
\*/
void armci_rcv_strided_data(int proc, request_header_t* msginfo, int datalen, 
                            void *ptr, int strides,int stride_arr[],int count[])
{
    char *databuf;

    if(DEBUG_){
        printf("%d: armci_rcv_strided_data: expecting datalen %d from %d\n",
                armci_me, datalen, proc); fflush(stdout);
    }

#ifdef CLIENT_BUF_BYPASS
    if(msginfo->bypass){
       /* zero-copy protocol: get ACK and then unpin user buffer */
       armci_rcv_strided_data_bypass(proc, msginfo, ptr, strides);
       armci_unpin_memory(ptr, stride_arr, count, strides);
       return; /* we are done */
    }
#endif
      

#ifdef SOCKETS
    /* zero-copy optimization for large requests */
    if(count[0] >  TCP_PAYLOAD){
       armci_ReadStridedFromDirect(proc,msginfo,ptr,strides,stride_arr, count);
       return; /*********************** done ************************/
    }
#elif defined(PIPE_BUFSIZE) 
    if(msginfo->datalen>2*PIPE_MIN_BUFSIZE){
       armci_pipe_receive_strided(msginfo, ptr, stride_arr, count, strides);
       return; /*********************** done ************************/
    } 
#endif

    databuf = armci_ReadFromDirect(proc,msginfo,datalen);
    armci_read_strided(ptr, strides, stride_arr, count, databuf);
}


/*\ get ACK from server
\*/
void armci_rem_ack(int clus)
{
int bufsize = sizeof(request_header_t)+sizeof(int);
int destproc = 0;
request_header_t *msginfo;
destproc = SERVER_NODE(clus);
msginfo = (request_header_t *)GET_SEND_BUFFER(bufsize,ACK,destproc);

    msginfo->dscrlen = 0;
    msginfo->from  = armci_me;
    msginfo->to    = SERVER_NODE(clus);
    msginfo->operation = ACK;
    msginfo->bytes   =0;
    msginfo->datalen =sizeof(int);

    if(DEBUG_){
       printf("%d(c):sending ACKreq to %d clus=%d\n",armci_me,msginfo->to,clus);
        fflush(stdout);
    }

    armci_send_req(armci_clus_info[clus].master, msginfo, bufsize);
    armci_rcv_data(armci_clus_info[clus].master, msginfo);  /* receive ACK */
    FREE_SEND_BUFFER(msginfo);
}



/***************************** server side *********************************/

static void armci_check_req(request_header_t *msginfo, int buflen)
{

    if((msginfo->to != armci_me && msginfo->to < armci_master) ||
       msginfo->to >= armci_master + armci_clus_info[armci_clus_me].nslave)
        armci_die("armci_rcv_req: invalid to", msginfo->to);
#if 0   
    /* should be done in recv_req */
    if(msginfo->operation != GET && msginfo->bytes > buflen)
        armci_die2("armci_rcv_req: message overflowing rcv buffer",
                  msginfo->bytes,MSG_BUFLEN);
#endif
   
    if(msginfo->dscrlen < 0)
        armci_die("armci_rcv_req: dscrlen < 0", msginfo->dscrlen);
    if(msginfo->datalen < 0)
        armci_die("armci_rcv_req: datalen < 0", msginfo->datalen);
#ifndef PIPE_BUFSIZE
    if(msginfo->dscrlen > (int)msginfo->bytes)
        armci_die2("armci_rcv_req: dsclen > bytes", msginfo->dscrlen,
                   msginfo->bytes);
#endif
}


/*\ server response - send data to client
\*/
void armci_send_data(request_header_t* msginfo, void *data)
{
    int to = msginfo->from;

#if defined(VIA) || defined(GM) || defined(VAPI)
    /* if the data is in the pinned buffer: MessageRcvBuffer */
    if((data > (void *)MessageRcvBuffer) &&
       (data < (void *)(MessageRcvBuffer + MSG_BUFLEN)))
        /* write the message to the client */
        armci_WriteToDirect(to, msginfo, data);
    else {
        /* copy the data to the MessageRcvBuffer */
#ifdef GM
        /* leave space for header ack */
        char *buf = MessageRcvBuffer + sizeof(long);
#else
        char *buf = MessageRcvBuffer;
#endif
        armci_copy(data, buf, msginfo->datalen);
        armci_WriteToDirect(to, msginfo, buf);
    }
#else
#ifdef DOELAN4
        /*this is because WriteToDirect is a no-op in elan4.c so we have
         * to do a put. This will not cause problems anywhere else in the
         * code and this part on elan4 will only be invoked in a GPC
         */
        ARMCI_Put(data,msginfo->tag.data_ptr,msginfo->datalen,to);
#else
        armci_WriteToDirect(to, msginfo, data);
#endif
#endif
}


/*\ server sends strided data back to client
\*/
void armci_send_strided_data(int proc,  request_header_t *msginfo,
                             char *bdata, void *ptr, int strides,
                             int stride_arr[], int count[])
{

    int to = msginfo->from;

    if(DEBUG_){ printf("%d(server): sending datalen = %d to %d %p\n",
                armci_me, msginfo->datalen, to,ptr); fflush(stdout); }
 
#if defined(SOCKETS)
    /* zero-copy optimization for large requests */
    if(count[0] >  TCP_PAYLOAD){
       armci_WriteStridedToDirect(to,msginfo,ptr, strides, stride_arr, count);
       return; /*********************** done ************************/
    }
#elif defined(PIPE_BUFSIZE)
    if(msginfo->datalen>2*PIPE_MIN_BUFSIZE) {
        armci_pipe_send_strided(msginfo, bdata, msginfo->datalen,
                                ptr, stride_arr, count, strides);
        return;
    }
#endif

    /* for small contiguous blocks copy into a buffer before sending */
    armci_write_strided(ptr, strides, stride_arr, count, bdata);

    /* write the message to the client */
    armci_WriteToDirect(to, msginfo, bdata);

    if(DEBUG_){
        printf("%d(serv):sent len=%d to %d\n",armci_me,msginfo->datalen,to);
        fflush(stdout);
    }
}


/*\ server sends ACK to client
\*/
void armci_server_ack(request_header_t* msginfo)
{
     int ack=ACK;
     if(DEBUG_){
        printf("%d server: sending ACK to %d\n",armci_me,msginfo->from);
        fflush(stdout);
     }

     if(msginfo->datalen != sizeof(int))
        armci_die("armci_server_ack: bad datalen=",msginfo->datalen);
     armci_send_data(msginfo, &ack);
}


/*  main routine for data server process in a cluster environment
 *  the process is blocked until message arrives from
 *  the clients and services the requests
 */
void armci_data_server(void *mesg)
{
    /* message */
    request_header_t *msginfo;
    void *descr;
    void *buffer;
    int buflen;
    int from, i;
    static int mytag=1;

    /* read header, descriptor, data, and buffer length */
    armci_rcv_req(mesg, &msginfo, &descr, &buffer, &buflen );

    /* check what we got */
    armci_check_req(msginfo,buflen);
    from = msginfo->from;

    if(DEBUG_){ 
       printf("%d(serv):got %d request from %d\n",armci_me,msginfo->operation,
               from);
       fflush(stdout);
    }

    switch(msginfo->operation){
      case ACK:
          if(DEBUG_) {
              fprintf(stdout, "%d(server): got ACK request from %d\n",
                      armci_me, msginfo->from); fflush(stdout);
          }
          armci_server_ack(msginfo);
          break;

      case ATTACH: 
          if(DEBUG_){
             printf("%d(serv):got ATTACH request from%d\n",armci_me, from);
             fflush(stdout);
          }
          armci_server_ipc(msginfo, descr, buffer, buflen);
          break;
#if defined(SOCKETS) || defined(HITACHI)
      case QUIT:   
          if(DEBUG_){ 
             printf("%d(serv):got QUIT request from %d\n",armci_me, from);
             fflush(stdout);
          }
          armci_server_goodbye(msginfo);
          break;
#endif

      case ARMCI_SWAP:
      case ARMCI_SWAP_LONG:
      case ARMCI_FETCH_AND_ADD:
      case ARMCI_FETCH_AND_ADD_LONG:
          armci_server_rmw(msginfo,descr,buffer);
          break;

      case LOCK:
          armci_server_lock(msginfo);
          break;

      case UNLOCK:
          armci_server_unlock(msginfo, descr);
          break;

      default:
          if(msginfo->format ==VECTOR)
              armci_server_vector(msginfo, descr, buffer, buflen);
          else if(msginfo->format ==STRIDED){
#if defined(VAPI) && defined(MELLANOX) /* buffer bypass protocol */
              if(msginfo->pinned == 1){
                  int armci_post_gather(void *, int *, int *,int, 
                                  armci_vapi_memhndl_t *,int,int,int,void *);
                  void * src_ptr;
                  int stride_levels;
                  int count[MAX_STRIDE_LEVEL];
                  int src_stride_arr[MAX_STRIDE_LEVEL];    
                  int found;
                  ARMCI_MEMHDL_T *mhandle;
                  int i,num,id;
                  
                  if(DEBUG1){
                     printf("%d(s) : unpacking dscr\n",armci_me);
                     fflush(stdout);
                  }
                  
                  src_ptr = *(void**)descr;
                  descr = (char*)descr + sizeof(void*);
                  stride_levels = *(int*)descr;
                  descr = (char*)descr + sizeof(int);
                  for(i =0; i<stride_levels;i++){
                      src_stride_arr[i] = *(int *)descr;
                      descr = (int *)descr + 1;
                  }
                  for(i =0;i<stride_levels+1;i++){
                      count[i] = *(int*)descr;
                      descr = (int*)descr + 1;   
                  
                  }

                  found = get_armci_region_local_hndl(src_ptr, armci_me,
                                 &mhandle);
                  if(!found){
                     armci_die("SERVER : local region not found",id);
                  }
                   
                  num =  armci_post_gather(src_ptr,src_stride_arr,
                                  count,stride_levels, mhandle,
                                  msginfo->from,mytag,SERV,NULL );
                  mytag =  (mytag+1)%MAX_PENDING;
                  if(mytag==0)mytag=1;
                  if(DEBUG1){
                     printf("%d(s) : finished posting %d gather\n", 
                                     armci_me,num);
                     fflush(stdout);
                  }     
                 
              }
              else        
#endif
                armci_server(msginfo, descr, buffer, buflen);
          }
          else
              armci_die2("armci_data_serv: unknown format code",
                         msginfo->format, msginfo->from);
    }
}


/*\ initialize connection and start server thread/processes
\*/
void armci_start_server()
{
    armci_init_connections();

    if(armci_me == armci_master) {

#ifdef SERVER_THREAD

     armci_create_server_thread( armci_server_code );
#else

     armci_create_server_process( armci_server_code );

#endif

    }

    armci_client_code();
    _armci_server_started=1;
}




void *armci_server_code(void *data)
{
#ifdef SERVER_THREAD
#if (defined(GM) || defined(VAPI) || defined(QUADRICS)) && defined(ARMCI_ENABLE_GPC_CALLS)
#  ifdef PTHREADS
  extern pthread_t data_server;
  data_server = pthread_self();
#  else  
  armci_die("armci_server_code: threaded data servers not using pthreads not supported by gpc", 0);
#  endif
#endif
#endif

    if(DEBUG_)
        printf("%d: in server after creating thread.\n",armci_me);

    /* make initial contact with all the computing process */
    armci_server_initial_connection();

    if(DEBUG_) {
        printf("%d(server): connected to all computing processes\n",armci_me);
        fflush(stdout);
    }
#ifdef ARMCI_ENABLE_GPC_CALLS
    gpc_init();
#endif
    armci_call_data_server();

    armci_transport_cleanup();

    return(NULL);
}



/*\ request to QUIT sent by client
\*/
void armci_serv_quit()
{
int bufsize = sizeof(request_header_t)+sizeof(int);
int destproc;
request_header_t *msginfo;
destproc = SERVER_NODE(armci_clus_me);  
msginfo = (request_header_t*)GET_SEND_BUFFER(bufsize,QUIT,destproc);

    if(DEBUG_){ printf("%d master: sending quit request to server\n",armci_me);
        fflush(stdout);
    }

    msginfo->dscrlen = 0;
    msginfo->from  = armci_me;
    msginfo->to    = SERVER_NODE(armci_clus_me);
    msginfo->operation = QUIT;
    if(ACK_QUIT)
       msginfo->bytes   = msginfo->datalen = sizeof(int); /* ACK */
    else
       msginfo->bytes   = msginfo->datalen = 0; /* no ACK */

    armci_send_req(armci_master, msginfo, bufsize);

    if(ACK_QUIT){
       int stat;
       stat = *(int*)armci_rcv_data(armci_master,msginfo);  /* receive ACK */
       if(stat  != QUIT)
            armci_die("armci_serv_quit: wrong response from server", stat);
       FREE_SEND_BUFFER(msginfo);
    }
}


/*\ server action triggered by request to quit
\*/
void armci_server_goodbye(request_header_t* msginfo)
{
     int ack=QUIT;
     if(DEBUG_){
        printf("%d server: terminating request by %d\n",armci_me,msginfo->from);
        fflush(stdout);
     }

     if(msginfo->datalen){
       msginfo->datalen = -msginfo->datalen;
       if(msginfo->datalen != sizeof(int))
          armci_die("armci_server_goodbye: bad datalen=",msginfo->datalen);

       armci_send_data(msginfo, &ack);
     }

     armci_transport_cleanup();

     /* Finalizing data server process w.r.t. MPI is not portable
      */
     _exit(0);
}


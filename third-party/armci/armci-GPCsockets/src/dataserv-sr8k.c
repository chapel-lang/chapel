/* $Id: dataserv-sr8k.c,v 1.4 2003-10-22 22:12:20 d3h325 Exp $ */
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include "armcip.h"
#include "shmem.h"

#define DEBUG_ 0

int *sbufreqdescarray;
int *sbufrcvdescarray;
extern ndes_t _armci_group;

rcv_field_info_t *serv_field_info;

int *cpendcauth;
void __stest(){
int dst,mykey=CLIENT_COUNTERS_KEY;
Cb_node_rt remote;
int blen,rc,i;
dst = armci_clus_me;
    cpendcauth = (int *)malloc(sizeof(int)*armci_nclus);
    for(i=0;i<armci_nclus;i++){
       bzero( &remote, sizeof(remote));
       remote.type = CB_NODE_RELATIVE;
       remote.ndes = _armci_group;
       dst++;dst%=armci_nclus;
       remote.node = dst;
       blen=sizeof(remote);
            
       rc=combuf_target((Cb_node *)&remote,blen,mykey,0,-1,&(cpendcauth[dst]));
       if(rc != COMBUF_SUCCESS)armci_die("combuf_target:",rc);
    }
}


/* server buffers struct--
 * common is the common buffer used to exchange data between server and clients
 * direct is a ptr to the whole server buffer
 * sm_buf and la_buf are the pointers formed by breaking direct into buffers.
*/
typedef struct{
    char *sm_buf[SMALL_BUFS_PER_PROCESS];/*small buf ptrs*/
    int sm_dsc[SMALL_BUFS_PER_PROCESS];  /*small buf field dscs*/
    char *common;     /*common buffer for client and server, client creates*/
    char *direct;     /*ptr to the whole client large buf*/
    char **dirflg;    /*array of directbuf(above) flags*/
    int getdsc;       /*descriptor for the get buffer*/
    char *getbuf;     /*Buffer used for get pipeline*/
} _sr8k_server_bufs_struct;
static _sr8k_server_bufs_struct srv_buf;

/*structure containing pointers and authorizations for locking server buffers*/
typedef struct{
    int *getbuf;
    int **putbuf;
    int getauth;
    int putauth;
} serv_buf_lock_t;
static serv_buf_lock_t lockb;

/*stucture to hold pending counts from each client*/
typedef struct{
    int *pend_cnt;
    int *pend_auth;
    int *smallbufcnt; /*small buf count*/
} serv_pending_cnt_t;
static serv_pending_cnt_t spend;

/*server holds authorizations from clients to put data in response to get*/
int *clauth;

server_auth_t *server_auths;

extern void server_set_mutex_array(char *tmp);


extern void create_map(int key,int size,Cb_object_t *oid,char **ptr,char *fn);
extern void map_attach(int,int,int,Cb_object_t *,char **,char *); 



/*steps for server_initial_connection are
 * 0. do a get and map on the memory client created as common
 * 1. get physical memory for server buffers and map to it
 * 2. create arrays for calls to mwait
 * 3. create recv fields and update serv_rcv_info
 * 4. create a rcv field for control information 
 * 5. create space and tcw's for all client server_buf_avail arrays
*/
void armci_server_initial_connection(){
int size,bytes,extra,sbufdesc,dst,field_num,key,auth;
int snum,ssize,lnum,lsize,rc,i,k,j;
char *temp, **tmpptr;
Cb_node_rt remote;
Cb_object_t oid,s_oid,moid;
Cb_opt_region options;
int mykey = CLIENT_SERV_COMMON_CB_KEY;
long s_key=SERVER_GENBUF_KEY;

    size = sizeof(rcv_field_info_t)*(armci_nproc+armci_nclus);
    size = ROUND_UP_PAGE(size + (armci_nproc+1)*sizeof(int)); 
    tmpptr = &(srv_buf.common);
    map_attach(0,mykey,(Cb_size_t)size,&oid,tmpptr,"server_initial_conn");

    /*three create/attach and map calls in both client and server code.
     *three calls for common buffer, mutex array and send/recv buffers
     */

    server_ready_flag = (int *)(srv_buf.common);
    client_ready_flag = (int *)(srv_buf.common)+1;
    serv_field_info = (rcv_field_info_t *)((int*)srv_buf.common+armci_nproc+1);
    client_rcv_field_info = serv_field_info + armci_nclus;
    server_reset_memory_variables();

    server_set_mutex_array(temp);

    ssize = SMALL_MSG_SIZE;snum = SMALL_MSG_NUM;
    lnum = LARGE_MSG_NUM;lsize = LARGE_MSG_SIZE;
    bytes = ssize*snum+lsize*lnum+128+(lnum+snum*numofbuffers+1)*FLAGSIZE;
    bytes = ROUND_UP_PAGE(bytes+2*armci_nproc*sizeof(int));
    tmpptr = (char**)&(srv_buf.direct);  mykey = s_key;
    map_attach(1,mykey,(Cb_size_t)bytes,&s_oid,tmpptr,"server_initial_conn");

    serv_field_info[armci_clus_me].bufptr = (long)(srv_buf.direct);
    serv_field_info[armci_clus_me].cb_key = s_key;
    spend.pend_cnt=(int *)((srv_buf.direct)+size*snum+lsize*lnum+128+(lnum+snum*numofbuffers+1)*FLAGSIZE);
    spend.smallbufcnt=spend.pend_cnt+armci_nproc;
    for(i=0;i<armci_nproc;i++){spend.pend_cnt[i]=0;spend.smallbufcnt[i]=0;}
    /*step 2 */
    sbufreqdescarray = (int *)malloc(sizeof(int)*(snum+lnum*numofbuffers+1));
    sbufrcvdescarray = (int *)malloc(sizeof(int)*(snum+lnum*numofbuffers+1));
    if(!sbufreqdescarray || !sbufrcvdescarray)
       armci_die("sbufrcvdescarray maloc failed",0);
    /*end step-2*/
    for(i=0;i<snum;i++){
       temp = ((char*)(srv_buf.direct)+ssize*i);
       if(DEBUG_)printf("\n%d:for index %d servbufptr=%p\n",armci_me,i,temp);
       if((rc=combuf_create_field(s_oid,temp,(Cb_size_t)(ssize),
           SERV_FIELD_NUM+i,0,0 ,&sbufdesc)) != COMBUF_SUCCESS)
         armci_die("serv_init_connection-1:combuf_create_field failed",rc);
       sbufreqdescarray[i] = sbufdesc;
       if(DEBUG_){
         printf("\n%d:server:sbufdesc=%d for index %d\n",armci_me,sbufdesc,i);
         fflush(stdout);
       }
    }
    temp += ssize;
    lockb.putbuf=(int **)malloc(sizeof(int*)*numofbuffers);
    if(!lockb.putbuf)armci_die("lockb.putbuf malloc failed",0);
    remote.type = CB_NODE_RELATIVE;
    remote.ndes = _armci_group;
    remote.node = armci_clus_me;
    combuf_target((Cb_node *)&remote, sizeof(remote), s_key, 0, -1, &lockb.putauth);
    for(i=0;i<lnum;i++){
       temp = ((char*)(srv_buf.direct)+lsize*i+ssize*snum);
       /*temporary solution, has to be modified for more large buffers*/
       for(k=0;k<numofbuffers;k++){
         if((rc=combuf_create_field(s_oid,(temp+k*lsize/numofbuffers+4),
             (Cb_size_t)(lsize/numofbuffers-4),SERV_FIELD_NUM_FOR_LARGE_BUF+k,
             0,0,&sbufdesc)) != COMBUF_SUCCESS)
           armci_die("serv_init_connection-2:combuf_create_field failed",rc);
         sbufreqdescarray[k+i+snum] = sbufdesc;
         lockb.putbuf[k+i]=(int *)(temp+k*lsize/numofbuffers);
         *(lockb.putbuf[k+i])=0;
         if(DEBUG_){
           printf("\n%d:server:sbufdesc=%d for index %d fieldsize=%d\n",
               armci_me,sbufdesc,k+i+snum,(lsize/numofbuffers));
           fflush(stdout);
         }
       }
    }

    /*create one large buffer on server only for handeling client get requests
      useful only in get pipeline*/

    bytes = ROUND_UP_PAGE(lsize);
    mykey = SERVER_LARGE_GETBUF_KEY;
    tmpptr = (char**)&(srv_buf.getbuf);
    map_attach(1,mykey,(Cb_size_t)bytes,&s_oid,tmpptr,"server_initial_conn");

    lockb.getbuf = (int *)(srv_buf.getbuf);
    remote.type = CB_NODE_RELATIVE;
    remote.ndes = _armci_group;
    remote.node = armci_clus_me;
    combuf_target((Cb_node *)&remote,sizeof(remote),SERVER_LARGE_GETBUF_KEY,0, -1,&lockb.getauth);
    *lockb.getbuf = 0;

    if(DEBUG_){
       printf("\nsrvrbufend=%p",((srv_buf.direct)+bytes));
       fflush(stdout);
    }
    server_auths=(server_auth_t *)malloc(sizeof(server_auth_t)*armci_nproc);
    clauth = (int *)malloc(sizeof(int)*armci_nproc);
    spend.pend_auth = (int *)malloc(sizeof(int)*armci_nproc);
    if(!server_auths || !clauth || !spend.pend_auth)
       armci_die("server_init amlloc failed",0);
    for(i=0;i<armci_nproc;i++){
       dst=armci_clus_id(i);

       /*Get a authorization of the entire client buffer,used in get*/
       remote.type = CB_NODE_RELATIVE;
       remote.ndes = _armci_group;
       remote.node = dst;
       rc = combuf_get_sendright( (Cb_node *)&remote, sizeof(remote),
           CLIENT_DIRECTBUF_KEY+i, CLIENT_GET_DIRECTBUF_FIELDNUM, -1, &auth);
       if(rc != COMBUF_SUCCESS){
         printf("%d:failed\n",armci_me);fflush(stdout);sleep(1);
         armci_die("armci_client_connect_to_servers combuf_get_sendright:",rc);
       }
       clauth[i]=auth;

       /*get authorization to write client count to client*/
       rc = combuf_get_sendright( (Cb_node *)&remote, sizeof(remote),
            CLIENT_DIRECTBUF_KEY+i, CLIENT_PENDING_OP_FIELDNUM, -1, &auth);
       if(rc != COMBUF_SUCCESS){
         printf("%d:failed\n",armci_me);fflush(stdout);sleep(1);
         armci_die("server pending op combuf_get_sendright:",rc);
       }
       spend.pend_auth[i]=auth;
       for(k=0;k<numofbuffers;k++){
         rc = combuf_get_sendright( (Cb_node *)&remote, sizeof(remote),
             CLIENT_DIRECTBUF_KEY+i, CLIENT_DIRECTBUF_FIELDNUM+i+k, -1, &auth);
         if(rc != COMBUF_SUCCESS){
           printf("%d:failed\n",armci_me);fflush(stdout);sleep(1);
           armci_die("server pending op combuf_get_sendright:",rc);
         }
         server_auths[i].lbuf_put_auths[k]=auth;
       }
       field_num=CLIENT_SMALLBUF_FIELDNUM;
       key = CLIENT_SMALLBUF_KEY+i;
       for(k=0;k<SMALL_BUFS_PER_PROCESS;k++){
         rc = combuf_get_sendright( (Cb_node *)&remote, sizeof(remote),
             key,field_num+k, -1, &auth);
         if(rc != COMBUF_SUCCESS){
           printf("%d:failed\n",armci_me);fflush(stdout);sleep(1);
           armci_die("server combuf_get_sendright:",rc);
         }
         server_auths[i].put_auths[k]=auth;
       }
       rc = combuf_target( (Cb_node *)&remote, sizeof(remote),
            key, 0, -1, &auth);
       if(rc != COMBUF_SUCCESS) armci_die("combuf_target:",rc);
       server_auths[i].get_auth=auth;
       if(DEBUG_){
         printf("\n%d: getting authorization=%d from %d and bufkey =%d\n",
             armci_me,auth,dst,key);
         fflush(stdout);
       }
       rc = combuf_target((Cb_node *)&remote,sizeof(remote),
           (BUF_KEY+i-armci_clus_info[dst].master), 0, -1, &auth);
       if(rc != COMBUF_SUCCESS) 
         armci_die("combuf_target:armci_client_connect_to_servers",rc);
       server_auths[i].clientputbuf_auth=auth;
       if(DEBUG_){
         printf("\n%d:read/write from/to node=%d now\n",armci_me,dst);
         fflush(stdout);
       }
    }
    __stest();
    if(DEBUG_){
       printf("\n:%d:Server successfully initialized\n",armci_me);
       fflush(stdout);
    }
}

void armci_wait_for_server(){
    if(armci_me == armci_master){
#   ifndef SERVER_THREAD
       RestoreSigChldDfl();
       armci_serv_quit();
       armci_wait_server_process();
#   endif
  }
}

int find_rcvd_desc_index(int i){
int j;
    for(j=0;j<SMALL_MSG_NUM+LARGE_MSG_NUM*numofbuffers+1;j++)
       if(sbufreqdescarray[j]==i)return(j);
    return(-1);
}

char * getbuffer(int i){
char *retptr;
    return(((srv_buf.direct)+(i<SMALL_MSG_NUM?i*SMALL_MSG_SIZE:(SMALL_MSG_NUM*SMALL_MSG_SIZE+(i-SMALL_MSG_NUM)*(LARGE_MSG_SIZE/numofbuffers)+4))));
}

unsigned int _sr8k_data_server_ev;
unsigned int _sr8k_data_server_smallbuf_ev;
unsigned int _sr8k_data_server_complet=0;
unsigned int _sr8k_server_smallbuf_complete=0;
int _sr8k_server_buffer_index;

void armci_sr8k_data_server(char *buf,int bufindex){
static Cb_msg msg;
int rc;
request_header_t *msginfo = (request_header_t *)buf;
    if(DEBUG_){
       printf("\n%d:for index %d servbufptr=%p spend.smallbufcnt=%d\n",armci_me,
           bufindex,buf,spend.smallbufcnt[msginfo->from]);
       fflush(stdout);
    }
    /*first call the actual data server routine*/
    armci_data_server(buf);
#if defined(PIPE_BUFSIZE)
#   if 0
    if(msginfo->operation==GET && msginfo->format==STRIDED 
                               && msginfo->datalen>2*PIPE_MIN_BUFSIZE)
       combuf_swap(lockb.getauth,0,0);
#   endif
#endif
    if((msginfo->operation==PUT || ACC(msginfo->operation) ) 
         && msginfo->tag.ack!=0 &&bufindex>=SMALL_MSG_NUM){
       memset(&msg, 0, sizeof(msg));
       msg.data.addr =  (char *)(spend.pend_cnt+msginfo->from);
       msg.data.size = sizeof(int);
       if(DEBUG_){
          printf("\n%d:got ack=%d from %d for op=%d bufindex=%d\n",armci_me,
              msginfo->tag.ack,msginfo->from,msginfo->operation,bufindex);
          fflush(stdout);
       }
       spend.pend_cnt[msginfo->from]++;/*=msginfo->tag.ack;*/
       rc=combuf_send(&msg,spend.pend_auth[msginfo->from],
           sizeof(int)*armci_clus_me,0,&_sr8k_data_server_ev);
       if (rc != COMBUF_SUCCESS)
         armci_die("armci_sr8k_data_server:combuf_send failed",rc);
       _sr8k_data_server_complet=99;
       if(_sr8k_data_server_complet==99)
          combuf_send_complete(_sr8k_data_server_ev,-1,&_sr8k_data_server_complet);
    }
    if(bufindex<SMALL_MSG_NUM){
       spend.smallbufcnt[msginfo->from]++;
       /*printf("\n%d last from %d = %d i have %d\n",armci_me,msginfo->from,msginfo->tag.ack,spend.smallbufcnt[msginfo->from]);*/
       memset(&msg, 0, sizeof(msg));
       msg.data.addr =  (char *)(spend.smallbufcnt+msginfo->from);
       msg.data.size = sizeof(int);
       rc=combuf_send(&msg,spend.pend_auth[msginfo->from],
      sizeof(int)*(armci_clus_me+armci_nproc),0,&_sr8k_data_server_smallbuf_ev);
       if (rc != COMBUF_SUCCESS) 
         armci_die("armci_sr8k_data_server:combuf_send failed",rc);
       _sr8k_server_smallbuf_complete=99;
       if(_sr8k_server_smallbuf_complete==99)
         combuf_send_complete(_sr8k_data_server_smallbuf_ev,-1,
             &_sr8k_server_smallbuf_complete);
    }
    /*else{*/
       rc=combuf_swap(lockb.putauth,(SMALL_MSG_SIZE*SMALL_MSG_NUM)+(/*bufindex-SMALL_MSG_NUM*/0)*LARGE_MSG_SIZE/numofbuffers,armci_nproc+2);
{
register Cb_size_t len__=8;
register char *c_;
c_ = (char *)(lockb.putbuf[0]);
combuf_purge(c_,len__);
}
       rc=combuf_swap(lockb.putauth,(SMALL_MSG_SIZE*SMALL_MSG_NUM)+(/*bufindex-SMALL_MSG_NUM*/0)*LARGE_MSG_SIZE/numofbuffers,0);
    /*}*/

    /*printf("\n%d last from %d = %d rc %d now=%d\n",armci_me,msginfo->from,msginfo->tag.ack,rc,*(lockb.putbuf[0]));*/
    if(DEBUG_){
       printf("\n%d:done %d putbuflocked  \n",armci_me,bufindex);
       fflush(stdout);
    }
}


void armci_call_data_server(){
int rc,i;
int index;
int descarraylen;
int sbufrcvnum;
    descarraylen = SMALL_MSG_NUM+LARGE_MSG_NUM*numofbuffers;
    if(DEBUG_){printf("\nabout to enter server loop\n");fflush(stdout);}
    *server_ready_flag = 1;
    while(1){
       rc=combuf_block_mwait(sbufreqdescarray,descarraylen,COMBUF_WAIT_ONE,
           -1,sbufrcvdescarray,&sbufrcvnum);
       for(i=0;i<sbufrcvnum;i++){
         index=find_rcvd_desc_index(sbufrcvdescarray[i]);
         _sr8k_server_buffer_index=index;
         if(index<0 || index>descarraylen)
           armci_die("wrong index in armci_call_data_server",index);
         if(DEBUG_){
           printf("\n%d:rcvd data from %d sbufrcvnum=%d\n",armci_me
               ,index,sbufrcvnum);fflush(stdout);
         }
         armci_sr8k_data_server(getbuffer(index),index);	
       }
    }
}

extern long _sr8k_armci_getbuf_ofs;
void armci_rcv_req(void *mesg,void *phdr,void *pdescr,void *pdata,int *buflen)
{
request_header_t *msginfo = (request_header_t *)mesg;
int rc;
    /*printf("%d(server): got %d req (dscrlen=%d datalen=%d)from %d to=%d\n",
         armci_me, msginfo->operation, msginfo->dscrlen,
         msginfo->datalen, msginfo->from ,msginfo->to);
         fflush(stdout);*/
rc=combuf_swap(cpendcauth[armci_clus_id(msginfo->from)],armci_clus_me*sizeof(int),0);
/*printf("\n%d: rc=%d dst=%d ofs=%d\n",armci_me,rc,armci_clus_id(msginfo->from),armci_clus_me);*/
    if(DEBUG_){
       printf("%d(server): got %d req (dscrlen=%d datalen=%d)from %d to=%d\n",
         armci_me, msginfo->operation, msginfo->dscrlen,
         msginfo->datalen, msginfo->from ,msginfo->to);
         fflush(stdout);
    }

    *(void **)phdr = msginfo;
    *buflen = LARGE_MSG_SIZE/numofbuffers - sizeof(request_header_t);
    if(msginfo->bytes) {
       if(DEBUG_){
         printf("\n%d:in armic_rcv_req aboutto set pdata  bytes=%d\n",
             armci_me,msginfo->bytes);fflush(stdout);
       }
       *(void **)pdescr = msginfo+1;
       *(void **)pdata = msginfo->dscrlen + (char*)(msginfo+1);  
    }
    else{
       *(void**)pdescr = NULL;
    }
    if(msginfo->operation == ATTACH){
       *(void **)pdata = msginfo->dscrlen + (char*)(msginfo+1);
       *buflen = SMALL_MSG_SIZE - sizeof(request_header_t);
    }
    if(msginfo->operation == GET) {
       *(void **)pdata =  msginfo->dscrlen + (char*)(msginfo+1);
       if((msginfo->bytes+sizeof(request_header_t))<SMALL_MSG_SIZE)
          *buflen = SMALL_MSG_SIZE - sizeof(request_header_t);	
#if    defined(PIPE_BUFSIZE)
       else if(msginfo->format==STRIDED && msginfo->datalen>2*PIPE_MIN_BUFSIZE){
         _sr8k_armci_getbuf_ofs=msginfo->tag.ack;/*client buffer offset*/
         /*if(*lockb.getbuf-10==msginfo->from)*/ /*can be usedif client cswaps*/
         *(void **)pdata = (srv_buf.getbuf);
         /*else*/
         /*armci_die2("process that locked is not same as the one requested",
           *lockb.getbuf,msginfo->from);
         */
       }
#      endif
       else{
         *buflen = LARGE_MSG_SIZE/numofbuffers - sizeof(request_header_t);
       }
    }
}


void armci_transport_cleanup(){

}

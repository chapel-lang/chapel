/* $Id$ */
#include <elan/elan.h>
#include <stdio.h>
#include <stdlib.h>
#include "armcip.h"
#include "copy.h"
#include "elandefs.h"
#include <elan/devent.h>

#define DEBUG_ 0
#define DEBUG_NOTIFY 0
#ifndef DOELAN4
 what are we doing here
#endif

#define DBL_BUF_SIZE 50000
#define MAX_BUFS 16
static double elan4_serv_bufs[MAX_BUFS][DBL_BUF_SIZE];

static ELAN_PGCTRL *_pgctrl;
static void *_pgsstate;
static void *_qd;
static void *zero=(void*)0;
extern void *pgs_init (ELAN_STATE *state, void *qMem);
extern void * pgs_ds_init (ELAN_STATE *state, void *qMem, void *dsqMem, int max);

static int _ELAN_SLOTSIZE=700;
static int server_can_poll=0;

#ifndef NB_NONCONT
#define VCALLS 1
#endif

#if !VCALLS
#define MAX_SLOTS 64
#define MIN_OUTSTANDING 6
static int max_pending=16; /* throttle number of outstanding nb calls */
#endif

#ifdef ELAN_ACC
static int armci_server_terminating=0;
static ELAN_MAIN_QUEUE *mq;
static int armci_request_from=-1;
static int armci_request_to=-1;


typedef struct{
	int *verify_seq_ar;
	int *wait_seq_ar;
	int** recv_verify_arr;
	int** recv_verify_smp_arr;
}armci_verify_wait_t;
armci_verify_wait_t __armci_verify_wait_struct;
armci_verify_wait_t *verify_wait = &__armci_verify_wait_struct;

static ops_t** armci_elan_fence_arr;
static ops_t *ops_pending_ar;
ops_t *rdma_ops_pending_ar;
static ops_t *ops_done_ar;
int notify_tmp[3];
static ELAN_EVENT* prevnotifydscr = (ELAN_EVENT *)0;
static ELAN_EVENT* prevnotifywaitdscr = (ELAN_EVENT *)0;
int **mynotify_epochs;
static int **notify_epoch_arr;
static int *mycurrent_epoch;
static int **current_epoch_seq;
static int numepochs = 2;

#define MSG_DATA_LEN (_ELAN_SLOTSIZE - sizeof(request_header_t))

#define MY_PUT2(src,dst,bytes,p) \
        elan_wait(elan_doput(_pgctrl,(src),(dst),(ELAN_ADDR)elanev[0],(bytes),p,0), elan_base->waitType)

#define MY_PUT(src,dst,bytes,p) \
        elan_wait(elan_put(elan_base->state,(src),(dst),(bytes),p), elan_base->waitType)
#define MY_GET(src,dst,len,p)\
        elan_wait(elan_get(elan_base->state,src,dst,len,p),elan_base->waitType)


#define _RAIL 0
#define NEWQAPI 1

#if NEWQAPI
static ELAN_QUEUE_TX *qtx;
static ELAN_QUEUE_RX *qrx;
#else
static ELAN_MAIN_QUEUE *mq;
#endif

#define NEVENTS (armci_nproc*numepochs) 
#define SETEVAL 1 
static ELAN_EVENT_ELAN *evdelan;
static ELAN_EVENT **elanev;

extern int elan_devent_completed(int setval, ELAN_EVENT *e);
#define DOPUT(_src, _dst, _bidx, _len, _p) \
        elan_doput(_pgctrl,_src,_dst,elan_destEvent(elanev[_bidx]),_len,_p,_RAIL)
#define DOWAIT(_bidx) elan_wait(elanev[_bidx],elan_base->waitType)
#define REPRIME(_bidx) elan_setWaitCount(elanev[_bidx],1)

void test_put(void *src, void *dst, int bytes, int p)
{
elan_wait(elan_doput(_pgctrl,(src),(dst),elan_destEvent(elanev[0]),(bytes),p,0), elan_base->waitType);
sleep(5);
elan_wait(elan_doput(_pgctrl,(src),(dst),elan_destEvent(elanev[0]),(bytes),p,0), elan_base->waitType);
sleep(2);
elan_wait(elan_doput(_pgctrl,(src),(dst),elan_destEvent(elanev[0]),(bytes),p,0), elan_base->waitType);
sleep(2);
elan_wait(elan_doput(_pgctrl,(src),(dst),elan_destEvent(elanev[0]),(bytes),p,0), elan_base->waitType);
}

void test_wait()
{

while(!elan_poll(elanev[0],0)){ 
   _elan_deventDump ("wait",elanev[0]); 
   printf("%d:completed %d\n",armci_me, elan_devent_completed(SETEVAL, elanev[0]));
   sleep(1);
}
//elan_wait(elanev[0],0);
    _elan_deventDump ("wait",elanev[0]);
}

void test_dump()
{
    _elan_deventDump ("after wait",elanev[0]);
}

//extern ELAN_EVENT *elan_getbflag(void *pgs,u_int destvp, long *retval);
extern ELAN_EVENT *elan_getbflag(void *pgs,u_int destvp, int lo, int hi, int w, long *retval);
extern void elan_clearbflag(void *pgs, int which);
extern void elan_deventDump (const char *label, ELAN_EVENT *e);

int armci_getbflag(int p)
{
static long retval=-1;
     elan_wait(elan_getbflag(_pgsstate,p,0,MAX_BUFS,1,&retval), elan_base->waitType);
     return(int)retval;
}


void armci_clearbflag(int which)
{
     elan_clearbflag(_pgsstate, which);
}


/* NOTE: for thread-safety we need to make put this in user buffer */
static ELAN_EVENT *event_getbflag=NULL;
static long _bidx=0;

#define BFLAG_PATH_SIZE (_ELAN_SLOTSIZE-sizeof(request_header_t))
#define BFLAG_PATH_SIZE_ 4000

int armcill_getbidx(int size, int proc, SERV_BUF_IDX_T *bufidx)
{
#if 1
    if(size > BFLAG_PATH_SIZE){ 
    int cluster = armci_clus_id(proc);
    int proc_serv = armci_clus_info[cluster].master;
    event_getbflag = elan_getbflag(_pgsstate,proc_serv,0,MAX_BUFS,0,bufidx);
    return 1;
  } 
#endif
  *bufidx = -1;
  return 0;
}

#ifdef ARMCI_ENABLE_GPC_CALLS
extern gpc_buf_t *gpc_req;
#endif
void armci_init_connections()
{
ELAN_QUEUE *q, *qs;
int nslots=armci_nproc+256, slotsize;
int R=0;
int i;
char *enval;
  
    //_ELAN_SLOTSIZE = elan_queueMaxSlotSize(elan_base->state);
    slotsize=_ELAN_SLOTSIZE;
#ifdef ARMCI_ENABLE_GPC_CALLS
    gpc_req = (gpc_buf_t *)malloc(MAX_GPC_REQ*sizeof(gpc_buf_t)+SIXTYFOUR);
#endif
    if ((q = elan_gallocQueue(elan_base, elan_base->allGroup)) == NULL)
            armci_die( "elan_gallocElan",0 );
#if NEWQAPI
    _armci_buf_init();
    if(!(qrx = elan_queueRxInit(elan_base->state, q, nslots, slotsize, R, 0))) 
               armci_die("Failed to initialise elan receive Q",0);
    if(!(qtx = elan_queueTxInit(elan_base->state, q, R, 0)))
//  if(!(qtx = elan_queueTxInit(elan_base->state, q, R, LIBELAN_QUEUEREUSEBUF)))
#else
    if(!(mq  = elan_mainQueueInit( elan_base->state, q, nslots, slotsize, 0)))
#endif
               armci_die("Failed to initialise elan Q",0);

#if VCALLS
    _qd = elan_gallocElan(elan_base, elan_base->allGroup, ELAN_QUEUE_ALIGN,
                              elan_pgvGlobalMemSize(elan_base->state));

    if(!_qd) armci_die("failed elan_gallocElan 1",0);
    elan_gsync(elan_base->allGroup);
    _pgctrl = elan_putgetInit(elan_base->state, _qd, 16, 4096, 4096, 32, ELAN_PGVINIT);
    if(!_pgctrl) armci_die("failed elan_gallocElan 2",0);
    elan_gsync(elan_base->allGroup);
    

    evdelan = elan_gallocElan(elan_base,elan_base->allGroup,32,NEVENTS*sizeof(ELAN_EVENT_ELAN));
    if(!evdelan) armci_die("failed elan_gallocElan for dest events",0);
    elanev = (ELAN_EVENT **)malloc(sizeof(ELAN_EVENT *)*NEVENTS);
    for(i=0;  i<NEVENTS; i++){
        elanev[i]= elan_initEvent(elan_base->state,elan_base->state->rail[_RAIL],evdelan+i,0);
        if(!elanev[i]) armci_die("elan_initEvent failed",i);
	//print_event_info(elanev[i]);
    }
    elan_gsync(elan_base->allGroup);
    //_elan_deventDump ("init",elanev[0]);

#endif

    /* pgs calls */
     qs = elan_gallocElan(elan_base, elan_base->allGroup, ELAN_QUEUE_ALIGN,
			elan_pgsGlobalMemSize(elan_base->state));
     //_pgsstate = pgs_init(elan_base->state, q);
     _pgsstate = pgs_ds_init(elan_base->state, qs, q, MAX_BUFS);

    if(armci_me == armci_master) {
        if(!(ops_done_ar=(ops_t*)calloc(armci_nproc,sizeof(ops_t))))
             armci_die("malloc failed for ARMCI ops_done_ar",0);
    }

    armci_elan_fence_arr = (ops_t**)malloc(armci_nproc*sizeof(ops_t*));
    if(!armci_elan_fence_arr)armci_die("malloc failed for ARMCI fence array",0);
    if(ARMCI_Malloc((void**)armci_elan_fence_arr, armci_nclus*sizeof(ops_t)))
             armci_die("failed to allocate ARMCI fence array",0);
    bzero(armci_elan_fence_arr[armci_me],armci_nclus*sizeof(ops_t));

    if(!(rdma_ops_pending_ar=(ops_t*)calloc(armci_nproc,sizeof(ops_t))))
         armci_die("malloc failed for ARMCI rdma_ops_pending_ar",0);


    
#ifdef OLD_QSNETLIBS
    /* initialize control descriptor for put/get */
    armci_pgctrl = elan_putgetInit(elan_base->state, 32, 8);
    if(!armci_pgctrl) armci_die("armci_init_con: elan_putgetInit failed",0);
#endif

    /* check if we can poll in the server thread */
    enval = getenv("ARMCI_SERVER_CAN_POLL");
    if(enval != NULL){
       if((enval[0] != 'N') && (enval[0]!='n')) server_can_poll=1;
    } else {
      if(armci_clus_info[armci_clus_me].nslave < armci_getnumcpus()) server_can_poll=1;
    }

    if(MessageSndBuffer){
      ((request_header_t*)MessageSndBuffer)->tag.data_ptr = (void*)0;
    }else armci_die("armci_init_connections: buf not set",0);
}

void armci_elan_notify_init()
{
    int i;
    if(!(ops_pending_ar=(ops_t*)calloc(armci_nclus,sizeof(ops_t))))
         armci_die("malloc failed for ARMCI ops_pending_ar",0);

    notify_epoch_arr = (int **)malloc(sizeof(int *)*armci_nproc);
    if(!notify_epoch_arr)armci_die("malloc failed for notify_epoch_arr",0);
    if(ARMCI_Malloc((void**)notify_epoch_arr,armci_nproc*numepochs*sizeof(int)))
             armci_die("failed to allocate ARMCI fence array",0);
    bzero(notify_epoch_arr[armci_me],armci_nproc*numepochs*sizeof(int));
    mynotify_epochs = (int **)calloc(armci_nproc,sizeof(int *));
    current_epoch_seq = (int **)calloc(armci_nproc,sizeof(int*));

    for(i=0;i<armci_nproc;i++){
       mynotify_epochs[i] = notify_epoch_arr[armci_me]+i*numepochs;
       current_epoch_seq[i] = (int *)calloc(armci_nproc,sizeof(int));
    }


    if(!(verify_wait->verify_seq_ar=(int*)calloc(armci_nproc,sizeof(int))))
        armci_die("malloc failed for ARMCI verify_seq_ar",0);
    /*allocate an array for wait sequence array*/
    if(!(verify_wait->wait_seq_ar=(int*)calloc(armci_nproc,sizeof(int))))
        armci_die("malloc failed for ARMCI wait_seq_ar",0);
    for(i=0;i<armci_nproc;i++){
       verify_wait->verify_seq_ar[i]=1;
       verify_wait->wait_seq_ar[i]=1;
    }

    verify_wait->recv_verify_smp_arr = (int**)malloc(armci_nproc*sizeof(int*));
    if(!verify_wait->recv_verify_smp_arr)armci_die("malloc-recv_verify_smp",0);
    bzero(verify_wait->recv_verify_smp_arr ,armci_nproc*sizeof(ops_t*));

    verify_wait->recv_verify_arr = (int**)malloc(armci_nproc*sizeof(int*));
    if(!verify_wait->recv_verify_arr)armci_die("malloc fail-recv_verify_arr",0);
    bzero(verify_wait->recv_verify_arr ,armci_nproc*sizeof(int*));

    if(ARMCI_Malloc((void**)verify_wait->recv_verify_arr,
                    armci_nproc*3*sizeof(int)))
             armci_die("failed to allocate recv_verify_arr",0);

    if(ARMCI_Malloc((void**)verify_wait->recv_verify_smp_arr,
                    armci_nproc*sizeof(int)*2))
       armci_die("failed to allocate ARMCI fence array",0); 
}



/*\ server sends ACK to client when request is processed
\*/
static void armci_send_ack()
{
ops_t val=0;
ops_t *buf = armci_elan_fence_arr[armci_request_from] + armci_clus_me;

#if 0
    if(armci_me==0)
    printf("%d:server sends ack p=%d fence=%p slot %p got=%d\n", armci_me, 
           armci_request_from, armci_elan_fence_arr[armci_request_from],buf,
           ops_done_ar[armci_request_from]+1); fflush(stdout);
#endif

    val = ++ops_done_ar[armci_request_from];
    verify_wait->recv_verify_smp_arr[armci_request_to][armci_request_from]=val;

    MY_PUT(&val,buf,sizeof(ops_t),armci_request_from);
}


ops_t armci_check_int_val(ops_t *v)
{
  return (*v);
}


void armci_elan_fence(int p)
{
    long loop=0;
    int cluster = armci_clus_id(p);
    ops_t *buf = armci_elan_fence_arr[armci_me] + cluster;
    long res = ops_pending_ar[cluster] - armci_check_int_val(buf);

#if 0
    if(ops_pending_ar[cluster])
    printf("%d: client fencing proc=%d fence=%p slot %p pending=%d got=%d\n", 
           armci_me, p, armci_elan_fence_arr[armci_me], buf, 
           ops_pending_ar[cluster], armci_check_int_val(buf)); fflush(stdout);
#endif

    while(res){
       if(++loop == 1000) { loop=0; usleep(1);  }
       armci_util_spin(loop, buf);
       res = ops_pending_ar[cluster] - armci_check_int_val(buf);
    }
}

#if 0
void armci_send_q(int p, void *buf, int len)
{
extern ELAN_EVENT * armci_sendq(void *pgs,u_int destvp,void *buf, int len);
armci_sendq(_pgsstate, p, buf, len);

}
#endif

/*\ server sends data to client buffer
\*/
void armci_WriteToDirect(int dst, request_header_t *msginfo, void *buffer)
{
   armci_die("armci_WriteToDirect: should not be called in this case",0);
}

char *armci_ReadFromDirect(int proc, request_header_t * msginfo, int len)
{
    char *buf = (char*) msginfo;
    return(buf);
}


static long bidx=-1;
void armci_call_data_server()
{
long usec_to_poll;

    usec_to_poll = server_can_poll? ELAN_POLL_EVENT: 0;

    if(DEBUG_){
        printf("%d(server): waiting for request\n",armci_me); fflush(stdout);
    }
#ifdef ARMCI_ENABLE_GPC_CALLS
    unblock_thread_signal(GPC_COMPLETION_SIGNAL); 
#endif
    while(1){
#ifdef ARMCI_ENABLE_GPC_CALLS
        block_thread_signal(GPC_COMPLETION_SIGNAL);
#endif
#       if NEWQAPI
           void *buf;
           buf = elan_queueRxWait(qrx, NULL, usec_to_poll);  
#       else
           char buf[_ELAN_SLOTSIZE];
           elan_queueWait(mq, buf, usec_to_poll );
#       endif
        armci_data_server((char*)buf);
        armci_send_ack();

        /* free the buffer if used */
        if(bidx>=0) { armci_clearbflag(bidx); bidx =-1; }
#ifdef ARMCI_ENABLE_GPC_CALLS
        unblock_thread_signal(GPC_COMPLETION_SIGNAL);
#endif
    }

    if(DEBUG_) {printf("%d(server): done! closing\n",armci_me); fflush(stdout);}
}

/*\ server receives request 
\*/
void armci_rcv_req(void *mesg, void *phdr, void *pdescr, void *pdata, int *buflen)
{
int i;
char *MessageBuffer = MessageRcvBuffer;  
request_header_t *msginfo = (request_header_t *)mesg;

    *(void **)phdr = msginfo;
    armci_request_from = msginfo->from;
    armci_request_to = msginfo->to;

    if(DEBUG_ ) {
       printf("%d(server): got %d req (dscrlen=%d datalen=%d) from %d %p\n",
              armci_me, msginfo->operation, msginfo->dscrlen,
              msginfo->datalen, msginfo->from,msginfo->tag.data_ptr); fflush(stdout);
    }

    *buflen = MSG_BUFLEN - sizeof(request_header_t);
    *(void **)pdescr = msginfo+1;
    *(void **)pdata  = msginfo->dscrlen + (char*)(msginfo+1);

    if(msginfo->bytes){

       if(msginfo->operation != GET){
          int payload = msginfo->datalen;
          int off =0;
          char *rembuf = msginfo->tag.data_ptr;
   
          if(msginfo->inbuf){
                bidx = (long)msginfo->tag.ack;
                if(DEBUG_){
                  printf("%ds bidx=%ld\n",armci_me,bidx); fflush(stdout);
                }
                if(bidx>MAX_BUFS || bidx<0) 
                   armci_die2("got wrong buffer index",(int)bidx,MAX_BUFS);
                MessageBuffer= (char*) &elan4_serv_bufs[bidx][0];
          }else MessageBuffer = MessageRcvBuffer;

          if(msginfo->dscrlen > MSG_DATA_LEN){
             payload += msginfo->dscrlen;
             *(void **)pdescr = MessageBuffer;
             off = msginfo->dscrlen;
          }else rembuf += msginfo->dscrlen;

          if((msginfo->dscrlen+msginfo->datalen)> MSG_DATA_LEN){

             if(!msginfo->inbuf){
                void *flag_to_clear = ((void**)msginfo->tag.ack_ptr); 
                MY_GET(rembuf,MessageBuffer,payload, msginfo->from);

                /* mark sender buffer as free -- flag is before descriptor */
                MY_PUT(&zero,flag_to_clear,sizeof(void*),msginfo->from);
                if(DEBUG_){
                  printf("%d:serv &tag=%p tag=%p dscrlen=%d %d data to %p pdscr=%p pdata=%p\n",
                         armci_me, flag_to_clear, msginfo->tag.ack, msginfo->dscrlen, payload, 
                         MessageBuffer,*(void **)pdescr, *(void **)pdata); fflush(stdout);
                }

             }
             *(void **)pdata  = MessageBuffer + off; 
          }
        }
    }else
        *(void**)pdescr = NULL;
}



#if NEWQAPI
    ELAN_EVENT *qtxevent=(ELAN_EVENT*)0;
#endif

/*\ send request to server thread
\*/
int armci_send_req_msg(int proc, void *vbuf, int len)
{
    char *MessageBuffer;  
    char *buf = (char*)vbuf;
    request_header_t *msginfo = (request_header_t *)buf;
    int cluster = armci_clus_id(proc);
    int size=_ELAN_SLOTSIZE;
    int proc_serv = armci_clus_info[cluster].master;
    int off =sizeof(request_header_t);
    int payload=0;
    if(msginfo->operation==PUT || ACC(msginfo->operation))
       ops_pending_ar[cluster]++;
    msginfo->tag.ack_ptr = &msginfo->tag.ack;
    if(msginfo->inbuf){
         if(event_getbflag)elan_wait(event_getbflag,elan_base->waitType);
         else { 
           printf("%ds: slotsize=%d flag=%d size=%d\n",armci_me,_ELAN_SLOTSIZE,
                  event_getbflag,msginfo->dscrlen+msginfo->datalen);
           armci_die("protocol inconsitency",(int)(long)msginfo->tag.ack);
         }   
         event_getbflag=NULL;
    }

    if(msginfo->operation != GET){
      if((msginfo->dscrlen+msginfo->datalen)> MSG_DATA_LEN){
      /* choose remote buffer */
      long Bidx;
      extern ELAN_EVENT *armci_sendq(void *,u_int,void*,int,void*,void*, int);

        payload = msginfo->datalen;
        if(msginfo->dscrlen > MSG_DATA_LEN){
             payload += msginfo->dscrlen;
        }
        else off+= msginfo->dscrlen;

        if(msginfo->inbuf){

          Bidx = (long)msginfo->tag.ack;
          if(Bidx<0){ 
            msginfo->inbuf = 0;  /* no buf -> take the other path */
          }
          else {
            MessageBuffer= (char*) &elan4_serv_bufs[Bidx][0];
            if(DEBUG_){
              printf("%d:SQ %p len=%d tag=%ld\n",armci_me,vbuf,len-payload,Bidx); 
              fflush(stdout);
            }
            qtxevent =armci_sendq(_pgsstate, proc_serv, vbuf, len-payload, buf+off, 
                                  MessageBuffer,payload);
            buf -= sizeof(ELAN_EVENT*); *(ELAN_EVENT**)buf =  qtxevent;

            return 0;   /*********** DONE **********/
          }
        }

        /* set message tag -> has pointer to client buffer with descriptor+data */
        msginfo->tag.data_ptr = (void *)(buf + sizeof(request_header_t));
        /*
         printf("%d: SENDing for %d %p to %p %d bytes bidx=%d\n",armci_me,proc_serv,
             buf+off,MessageBuffer,payload,Bidx); fflush(stdout);
         MY_PUT(buf+off,MessageBuffer,payload, proc_serv);
        */
        if(DEBUG_){ printf("%d:in SEND &tag=%p %p tag=%p\n",armci_me,&msginfo->tag.ack,
                  msginfo->tag.ack_ptr,msginfo->tag.data_ptr); fflush(stdout); }

      } 
      else {
            msginfo->tag.data_ptr=NULL; /* null tag means sender buffer is free */
            msginfo->tag.ack=0L; /* tag=0 means sender buffer is free */
      }
    }

#   if NEWQAPI
#      ifdef BUF_EXTRA_FIELD_T

          qtxevent = elan_queueTx(qtx, proc_serv, vbuf, len-payload, 0);
          buf -= sizeof(ELAN_EVENT*); 
          *(ELAN_EVENT**)buf =  qtxevent;
#      else
          if(qtxevent)elan_wait(qtxevent,ELAN_POLL_EVENT);
          qtxevent = elan_queueTx(qtx, proc_serv, vbuf, len-payload, 0);
#      endif

#   else
       elan_queueReq(mq, proc_serv, vbuf, len-payload); /* vbuf is sent/copied out */
#   endif
    
#if 0
    if(armci_me==0){
      printf("%d sent request %d to (%d,%d)\n",armci_me,ops_pending_ar[proc], 
      proc,proc_serv); fflush(stdout);
    }
#endif

    return 0;
}


void armcill_clearbuf(ELAN_EVENT** handle)
{
request_header_t *msginfo = (request_header_t *)(handle+1);

     elan_wait(*handle, elan_base->waitType); 
     if(!msginfo->inbuf){
        while(msginfo->tag.ack){
          armci_util_spin(100,msginfo);
          msginfo->tag.data_ptr=NULL;
        }
     }
}

int armcill_testbuf(ELAN_EVENT** handle)
{
int ret;
request_header_t *msginfo = (request_header_t *)(handle+1);

     if(msginfo->inbuf)
        ret = !elan_poll(handle,1L);
     else
        ret = (msginfo->tag.ack)? 0: 1;

     return ret;
}

void armci_wait_for_server()
{
  armci_server_terminating=1;
}

void armci_transport_cleanup() {
  _elan_deventDump ("terminate",elanev[0]);
}
void armci_client_connect_to_servers(){}
void armci_server_initial_connection(){}

void armci_elan_put_with_tracknotify(char *src,char *dst,int n,int proc,
		ELAN_EVENT **phandle)
{
   int es;
   
   rdma_ops_pending_ar[proc]++;	
   es = current_epoch_seq[armci_me][proc]+1;
   es%=numepochs;
   *phandle = DOPUT(src,dst,numepochs*armci_me+es,n,proc); 
   if(DEBUG_){printf("\n%d:done put rdma=%d\n",armci_me,rdma_ops_pending_ar[proc]);fflush(stdout);}
}

int armci_inotify_proc(int proc)
{
int *remptr = verify_wait->recv_verify_arr[proc]+3*armci_me;
int *myptr = verify_wait->recv_verify_smp_arr[armci_me]+armci_nproc;
int loop=0;
		   
    if(SAMECLUSNODE(proc)){
       remptr = verify_wait->recv_verify_smp_arr[proc]+armci_me;
       *(remptr)=verify_wait->verify_seq_ar[proc]++;
       MEM_FENCE;
       return((*remptr));
    }
    else{

       if(prevnotifydscr)elan_wait(prevnotifydscr,elan_base->waitType); 

       ALIGN_PTR_LONG(int,myptr);
       myptr[0] = verify_wait->verify_seq_ar[proc]++;
       myptr[1] = ops_pending_ar[armci_clus_id(proc)];
       myptr[2] = rdma_ops_pending_ar[proc];
       rdma_ops_pending_ar[proc]=0;
        
       
       /*\
	* we wait before ensure that last epoch is complete, we do this because
	* we want to overlap the time it takes to recv message from server
       \*/

       mycurrent_epoch =mynotify_epochs[proc]+current_epoch_seq[armci_me][proc];

       if(DEBUG_NOTIFY){
         fprintf(stderr,"%d:waiting for %p from %d at ind %d to be 0",
               armci_me,mycurrent_epoch,proc,current_epoch_seq[armci_me][proc]);
       }

       while(armci_check_int_val(mycurrent_epoch)){
         if(++loop == 1000) { loop=0;usleep(1); }
         armci_util_spin(loop, mycurrent_epoch);
       }

       if(DEBUG_NOTIFY){
         fprintf(stderr,"%d:done waiting for %p from %d at ind %d to be 0",
               armci_me,mycurrent_epoch,proc,current_epoch_seq[armci_me][proc]);
       }

       current_epoch_seq[armci_me][proc]++;
       current_epoch_seq[armci_me][proc]%=numepochs;
       mycurrent_epoch =mynotify_epochs[proc]+current_epoch_seq[armci_me][proc];
       *mycurrent_epoch=1;

       /*prevnotifydscr = DOPUT(myptr,remptr,proc,sizeof(int)*3,proc);*/
       prevnotifydscr = elan_put(elan_base->state,myptr,remptr,sizeof(int)*3,
		       proc);

       if(DEBUG_NOTIFY){
         printf("\n%d: sending %d %d %d to %d at %p\n",armci_me,*(myptr),
                  *(myptr+1),*(myptr+2),proc,remptr);
         fflush(stdout);
       }
       fflush(stdout);
       return(myptr[0]);
    }
}


int armci_inotify_wait(int proc, int *pval)
{
int *buf_notify,serv_count,rdma_count,*myserv_count;
int wait_val,wait_fence=0,zer=0;
int res,eventcount,es;
int *myptr=verify_wait->recv_verify_smp_arr[armci_me]+armci_nproc;
long loop=0;

    wait_val = verify_wait->wait_seq_ar[proc]++;

    buf_notify = verify_wait->recv_verify_arr[armci_me]+3*proc;

    if(SAMECLUSNODE(proc)){
       buf_notify = verify_wait->recv_verify_smp_arr[armci_me]+proc;
       *pval = wait_val;
    }

    if(DEBUG_NOTIFY){
      printf("\n%d:expecting %d at %p from %d\n",armci_me,wait_val,buf_notify,
              proc);
      fflush(stdout);
    }
    /*first we wait for sequence to match*/
    if((wait_val - armci_check_int_val(buf_notify)) > 0) {
      if(DEBUG_NOTIFY){
        printf("\n%d:verifyseq expecting%d have %d",armci_me,
               wait_val,armci_check_int_val(buf_notify));fflush(stdout);
      }
      MEM_FENCE;      
      res = wait_val - armci_check_int_val(buf_notify);
      while(res>0){
        if(++loop == 1000) { loop=0;usleep(1); }
         armci_util_spin(loop, buf_notify);
        MEM_FENCE;
        res = wait_val - armci_check_int_val(buf_notify);
      }
      if(DEBUG_NOTIFY){
        printf("\n%d:arrived verifyseq expected %d have %d",armci_me,
               wait_val,armci_check_int_val(buf_notify));fflush(stdout);
      }
    }

    if(SAMECLUSNODE(proc))
       return wait_val;

    serv_count = verify_wait->recv_verify_arr[armci_me][3*proc+1];
    rdma_count = verify_wait->recv_verify_arr[armci_me][3*proc+2];
    myserv_count = verify_wait->recv_verify_smp_arr[armci_me]+proc;
    wait_fence = serv_count;


    MEM_FENCE;
    res = wait_fence - armci_check_int_val(myserv_count);

    if(DEBUG_NOTIFY){
      printf("\n%d:fence expecting%d have %d",
             armci_me,wait_fence,armci_check_int_val(myserv_count));
      fflush(stdout);
    }

    if(!SAMECLUSNODE(proc)){ 
      while(res>0){
        if(++loop == 1000) { loop=0;usleep(1); }
          armci_util_spin(loop, myserv_count);
        MEM_FENCE;
        wait_fence=serv_count =verify_wait->recv_verify_arr[armci_me][3*proc+1];
        res = wait_fence - armci_check_int_val(myserv_count);
      }
    }
    
    rdma_count = verify_wait->recv_verify_arr[armci_me][3*proc+2];
    current_epoch_seq[proc][armci_me]++;
    current_epoch_seq[proc][armci_me]%=numepochs;
    es = proc*numepochs+current_epoch_seq[proc][armci_me];
    if(rdma_count)
       armci_elan_wait_event(elanev[es],rdma_count);
	    
    *pval = armci_check_int_val(buf_notify);

    *myptr = 0;

    if(prevnotifywaitdscr)elan_wait(prevnotifywaitdscr,elan_base->waitType); 


    if(DEBUG_NOTIFY){
       fprintf(stderr,"\n%d:clearing %d on %d at %p\n",armci_me,
               current_epoch_seq[proc][armci_me],proc,
	       notify_epoch_arr[proc]+armci_me*numepochs+current_epoch_seq[proc][armci_me]);
    }
    prevnotifywaitdscr = elan_put(elan_base->state,myptr,
    notify_epoch_arr[proc]+armci_me*numepochs+current_epoch_seq[proc][armci_me],
    sizeof(int),proc);


    return(wait_val);
}
#endif


/************************************************************************/
#if defined(_ELAN_LOCK_H) 

#define MAX_LOCKS 4
static ELAN_LOCK *my_locks, *all_locks;
static int num_locks=0;

/* NOTE that if ELAN is defined the scope of locks is limited to SMP
   and we do not call the interfaces below */


/*\ allocate and initialize num locks on each processor (collective call)
\*/
void armcill_allocate_locks(int num)
{
   char *buf;
   int i,elems;
   long mod;

   if(MAX_LOCKS<num)armci_die2("too many locks",MAX_LOCKS,num);
   num_locks = num;

   /* allocate memory to hold lock info for all the processors */
   buf = malloc(armci_nproc*num *sizeof(ELAN_LOCK) + ELAN_LOCK_ALIGN);
   if(!buf) armci_die("armcill_init_locks: malloc failed",0);

   mod = ((long)buf) %ELAN_LOCK_ALIGN;
   all_locks = (ELAN_LOCK*)(buf +ELAN_LOCK_ALIGN-mod); 
   if(((long)all_locks) %ELAN_LOCK_ALIGN) 
        armci_die2("lock alligment failed",mod,ELAN_LOCK_ALIGN);
   bzero(all_locks,armci_nproc*num *sizeof(ELAN_LOCK));

   /* initialize local locks */
   my_locks = all_locks + armci_me * num;
   for(i=0; i<num; i++)
       elan_lockInit(elan_base->state, my_locks+i, ELAN_LOCK_NORMAL);

   /* now we use all-reduce to exchange locks info among everybody */
   elems = (num*armci_nproc*sizeof(ELAN_LOCK))/sizeof(long);
   if((num*sizeof(ELAN_LOCK))%sizeof(long)) 
       armci_die("armcill_init_locks: size mismatch",sizeof(ELAN_LOCK));
   armci_msg_lgop((long*)all_locks,elems,"+");
#if 0
   if(armci_me == 0){
     for(i=0; i<num*armci_nproc; i++) printf("%d:(%d) master=%d type=%d\n",i,elems,(all_locks+i)->lp_master, (all_locks+i)->lp_type);
   }
#endif
   armci_msg_barrier();
}


void armcill_lock(int m, int proc)
{
ELAN_LOCK *rem_locks = (ELAN_LOCK*)(all_locks + proc*num_locks);

   if(m<0 || m>= num_locks) armci_die2("armcill_lock: bad lock id",m,num_locks);
   if(proc<0 || proc>= armci_nproc) armci_die("armcill_lock: bad proc id",proc);

   elan_lockLock(elan_base->state, rem_locks + m, ELAN_LOCK_BUSY);
}

void armcill_unlock(int m, int proc)
{
ELAN_LOCK *rem_locks = (ELAN_LOCK*)(all_locks + proc*num_locks);

   if(m<0 || m>= num_locks) armci_die2("armcill_unlock:bad lockid",m,num_locks);
   if(proc<0 || proc>=armci_nproc)armci_die("armcill_unlock: bad proc id",proc);

   elan_lockUnLock(elan_base->state, rem_locks + m);
}
     
#endif

#if HAS_PUTS
extern ELAN_EVENT *elan_putss (void *pgs, void *src, void *dst, int *src_stride_arr, int *dst_stride_arr, u_int *count, u_int strides, u_int destvp);


void armcill_putS(int proc, void* src_ptr, int src_stride_arr[], void* dst_ptr,
                  int dst_stride_arr[], int count[], int stride_levels)
{
    elan_wait(elan_putss(_pgsstate,src_ptr,dst_ptr, src_stride_arr, 
              dst_stride_arr, count, stride_levels, proc),elan_base->waitType);
}

ELAN_EVENT * armcill_nbputS(int proc, void* src_ptr, int src_stride_arr[], 
             void* dst_ptr, int dst_stride_arr[], int count[], int stride_levels)
{
    return elan_putss(_pgsstate,src_ptr,dst_ptr, src_stride_arr,
              dst_stride_arr, count, stride_levels, proc);
}
#endif

#if HAS_GETS
extern ELAN_EVENT *elan_getss (void *pgs, void *src, void *dst, int *src_stride_arr, int *dst_stride_arr, u_int *count, u_int strides, u_int destvp);
void armcill_getS(int proc, void* src_ptr, int src_stride_arr[], void* dst_ptr,
                  int dst_stride_arr[], int count[], int stride_levels)
{
    elan_wait(elan_getss(_pgsstate,src_ptr,dst_ptr, src_stride_arr, 
              dst_stride_arr, count, stride_levels, proc),elan_base->waitType);
}

ELAN_EVENT* armcill_nbgetS(int proc, void* src_ptr, int src_stride_arr[],  
              void* dst_ptr, int dst_stride_arr[], int count[], int stride_levels)
{
    return elan_getss(_pgsstate,src_ptr,dst_ptr, src_stride_arr,
              dst_stride_arr, count, stride_levels, proc);
}
#endif

/************************************************************************/
#if VCALLS 

#define MAX_VECS 1024 
static void* _src[MAX_VECS], *_dst[MAX_VECS];


void armci_network_strided(int op, void* scale, int proc,void *src_ptr,
                   int src_stride_arr[], void* dst_ptr, int dst_stride_arr[],
                   int count[], int stride_levels, armci_ihdl_t nb_handle)
{
int rc=0;
int total_of_2D,i,j;    
char *src = (char*)src_ptr, *dst=(char*)dst_ptr;
char *bufptr;
int dsize=3*sizeof(void*);
ELAN_EVENT* o_cmpl;
extern ELAN_EVENT *elan_getss (void *pgs, void *src, void *dst, int *src_stride_arr, int *dst_stride_arr, u_int *count, u_int strides, u_int destvp);
extern ELAN_EVENT *elan_putss (void *pgs, void *src, void *dst, int *src_stride_arr, int *dst_stride_arr, u_int *count, u_int strides, u_int destvp);
    if(stride_levels==0){
       if(op==GET)
         o_cmpl = elan_get(elan_base->state,src,dst,count[0],proc);
       if(op==PUT){
         if(nb_handle) 
           armci_elan_put_with_tracknotify(src,dst,count[0],proc,&o_cmpl);
         else
           o_cmpl = elan_put(elan_base->state,src,dst,count[0],proc);
       }
    }
    else if(stride_levels==1){ 
      if(op==GET)
        o_cmpl = elan_getss(_pgsstate,src_ptr,dst_ptr, src_stride_arr,
                            dst_stride_arr, count, stride_levels, proc);
      else if(op==PUT)
        o_cmpl = elan_putss(_pgsstate,src_ptr,dst_ptr, src_stride_arr,
                           dst_stride_arr, count, stride_levels, proc);
      else
        armci_die("network strided called for accumulate",proc);
    }
    else if(stride_levels==2){
      if(op==GET)
        o_cmpl = elan_getss(_pgsstate,src_ptr,dst_ptr, src_stride_arr,
                           dst_stride_arr, count, stride_levels, proc);
      else if(op==PUT)
        o_cmpl = elan_putss(_pgsstate,src_ptr,dst_ptr, src_stride_arr,
                           dst_stride_arr, count, stride_levels, proc);
      else
        armci_die("network strided called for accumulate",proc);
    }
    else{
       armci_die("network strided called for stride_levels>=3",proc);
    }
    if(!nb_handle)
       elan_wait(o_cmpl,elan_base->waitType);
    else
       nb_handle->cmpl_info = o_cmpl;
}

void armcill_getv(int proc, int bytes, int count, void* src[], void* dst[])
{
int _j, issued=0;

#if 0
    printf("%d: getv %d\n", armci_me, count); fflush(stdout);
#endif
    for (_j = 0;  _j < count;  _j++ ){
        _src[issued] = src[_j];
        _dst[issued] = dst[_j]; 
        issued++;
        if(issued == MAX_VECS){
           elan_wait(elan_getv(_pgctrl,_src,_dst,bytes,issued,proc),elan_base->waitType);
           issued=0;
        }
    }
    if(issued)elan_wait(elan_getv(_pgctrl,_src,_dst,bytes,issued,proc),
                        elan_base->waitType);
}


void armcill_putv(int proc, int bytes, int count, void* src[], void* dst[])
{
int _j, issued=0;

#if 0
    printf("%d: putv %d\n", armci_me, count); fflush(stdout);
#endif

    for (_j = 0;  _j < count;  _j++ ){
        _src[issued] = src[_j];
        _dst[issued] = dst[_j];
        issued++;
        if(issued == MAX_VECS){
           elan_wait(elan_putv(_pgctrl,_src,_dst,bytes,issued,proc),
                               elan_base->waitType);
           issued=0;
        }
    }
    if(issued)elan_wait(elan_putv(_pgctrl,_src,_dst,bytes,issued,proc),
                        elan_base->waitType);
}

 




#if HAS_PUTS_

void armcill_put2D(int proc, int bytes, int count, void* src_ptr,int src_stride,
                                                   void* dst_ptr,int dst_stride)
{
u_int acount[2];
acount[0]=bytes; 
acount[1]=count; 
elan_wait(elan_putss(_pgsstate,src_ptr,dst_ptr, &src_stride, &dst_stride, acount,1,proc),elan_base->waitType); 
}

#else

void armcill_put2D(int proc, int bytes, int count, void* src_ptr,int src_stride,
                                                   void* dst_ptr,int dst_stride)
{
int _j, issued=0;
char *ps=src_ptr, *pd=dst_ptr;

#if 0
    printf("%d: putv %d\n", armci_me, count); fflush(stdout);
#endif

    for (_j = 0;  _j < count;  _j++ ){
        _src[issued] = ps;
        _dst[issued] = pd;
        ps += src_stride;
        pd += dst_stride;
        issued++;
        if(issued == MAX_VECS){
           elan_wait(elan_putv(_pgctrl,_src,_dst,bytes,issued,proc),elan_base->waitType);
           issued=0;
        }
    }
    if(issued)elan_wait(elan_putv(_pgctrl,_src,_dst,bytes,issued,proc),elan_base->waitType);
}

#endif

#if HAS_GETS_

void armcill_get2D(int proc, int bytes, int count, void* src_ptr,int src_stride,
                                                   void* dst_ptr,int dst_stride)
{
   u_int acount[2];
   acount[0]=bytes; 
   acount[1]=count; 
   elan_wait(elan_getss(_pgsstate,src_ptr,dst_ptr, &src_stride, &dst_stride, acount,1,proc),elan_base->waitType); 
}
#else

void armcill_get2D(int proc, int bytes, int count, void* src_ptr,int src_stride,
                                                   void* dst_ptr,int dst_stride)
{
int _j, issued=0;
char *ps=src_ptr, *pd=dst_ptr;
    
#if 0
    printf("%d: getv %d\n", armci_me, count); fflush(stdout);
#endif
    for (_j = 0;  _j < count;  _j++ ){
        _src[issued] = ps;
        _dst[issued] = pd;
        ps += src_stride;
        pd += dst_stride;
        issued++;
        if(issued == MAX_VECS){
           elan_wait(elan_getv(_pgctrl,_src,_dst,bytes,issued,proc),elan_base->waitType);
           issued=0;
        } 
    }
    if(issued)elan_wait(elan_getv(_pgctrl,_src,_dst,bytes,issued,proc),elan_base->waitType);
#if 0
    printf("%d: getv count=%d issued=%d\n", armci_me, count,issued); fflush(stdout);
#endif
}
#endif


void armcill_wait_get(){}
void armcill_wait_put(){}

#elif 0 /*#else*/

#ifdef _ELAN_PUTGET_H

/* might have to use MAX_SLOTS<MAX_PENDING due to throttling a problem in Elan*/
#define MAX_PENDING 6 
#define ZR  (ELAN_EVENT*)0

static ELAN_EVENT* put_dscr[MAX_SLOTS]= {
ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,
ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR};

static ELAN_EVENT* get_dscr[MAX_SLOTS] = {
ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,
ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR,ZR};

static int cur_get=0;
static int cur_put=0;
static int pending_get=0;
static int pending_put=0;

int kwach=0;
/*\ strided put, nonblocking
\*/
void armcill_put2D(int proc, int bytes, int count, void* src_ptr,int src_stride,
                                                   void* dst_ptr,int dst_stride)
{
int _j, i, batch, issued=0;
char *ps=src_ptr, *pd=dst_ptr;

#if 1
    for (_j = 0;  _j < count;  ){
      /* how big a batch of requests can we issue */
      batch = (count - _j )<max_pending ? count - _j : max_pending; 
      _j += batch;
      for(i=0; i< batch; i++){
        if(put_dscr[cur_put])elan_wait(put_dscr[cur_put],100); 
        else pending_put++;
#if 1
        put_dscr[cur_put]= elan_put(elan_base->state,ps, pd,(size_t)bytes,proc);
#else
        elan_wait(elan_put(elan_base->state, ps, pd, (size_t)bytes, proc),1000);
#endif
        issued++;
        ps += src_stride;
        pd += dst_stride;
        cur_put++;
        if(cur_put>=max_pending)cur_put=0;
      }
    }

    if(issued != count) 
       armci_die2("armci-elan put:mismatch %d %d \n", count,issued);
#else
     for (_j = 0;  _j < count;  _j++){
       elan_wait(elan_put(elan_base->state, ps, pd, (size_t)bytes, proc),1000);
       ps += src_stride;
       pd += dst_stride;
     }
#endif
}


/*\ blocking vector put
\*/
void armcill_putv(int proc, int bytes, int count, void* src[], void* dst[])
{
int _j, i, batch, issued=0;
void *ps, *pd;

#if 0
    printf("%d: putv %d\n", armci_me, count); fflush(stdout);
#endif

    for (_j = 0;  _j < count;  ){
      /* how big a batch of requests can we issue */
      batch = (count - _j )<max_pending ? count - _j : max_pending; 
      _j += batch;
      for(i=0; i< batch; i++){
        if(put_dscr[cur_put])elan_wait(put_dscr[cur_put],100); 
        else pending_put++;
        ps = src[issued];
        pd = dst[issued];
        put_dscr[cur_put]= elan_put(elan_base->state,ps, pd,(size_t)bytes,proc);
        issued++;
        cur_put++;
        if(cur_put>=max_pending)cur_put=0;
      }
    }
    if(issued != count) 
       armci_die2("armci-elan putv:mismatch\n", count,issued);

    for(i=0; i<max_pending; i++) if(put_dscr[i]){
        elan_wait(put_dscr[i],100);
        put_dscr[i]=(ELAN_EVENT*)0;
    }
}



/*\ strided get, nonblocking
\*/
void armcill_get2D(int proc, int bytes, int count, void* src_ptr,int src_stride,
                                                   void* dst_ptr,int dst_stride)
{
int _j, i, batch, issued=0;
char *ps=src_ptr, *pd=dst_ptr;

#if 1
    for (_j = 0;  _j < count;  ){
      /* how big a batch of requests can we issue */
      batch = (count - _j )<max_pending ? count - _j : max_pending;
      _j += batch;
      for(i=0; i< batch; i++){
#if 1
        if(get_dscr[cur_get])elan_wait(get_dscr[cur_get],100); 
        else pending_get++;
        get_dscr[cur_get]=elan_get(elan_base->state,ps,pd, (size_t)bytes, proc);
#else
        elan_wait(elan_get(elan_base->state, ps, pd, (size_t)bytes, proc),elan_base->waitType);
#endif
        issued++;
        ps += src_stride;
        pd += dst_stride;
        cur_get++;
        if(cur_get>=max_pending)cur_get=0;
      }
    }

    if(issued != count) 
       armci_die2("armci-elan get:mismatch %d %d \n", count,issued);
#else
      for (_j = 0;  _j < count;  _j++){
        elan_wait(elan_get(elan_base->state, ps, pd, (size_t)bytes, proc),elan_base->waitType);
        ps += src_stride;
        pd += dst_stride;
      }
#endif
}


/*\ blocking vector get 
\*/
void armcill_getv(int proc, int bytes, int count, void* src[], void* dst[])
{
int _j, i, batch, issued=0;
void *ps, *pd;

#if 0
    printf("%d: getv %d\n", armci_me, count); fflush(stdout);
#endif

    for (_j = 0;  _j < count;  ){
      /* how big a batch of requests can we issue */
      batch = (count - _j )<max_pending ? count - _j : max_pending;
      _j += batch;
      for(i=0; i< batch; i++){
        if(get_dscr[cur_get])elan_wait(get_dscr[cur_get],100);
        else pending_get++;
        ps = src[issued];
        pd = dst[issued];
        get_dscr[cur_get]= elan_get(elan_base->state,ps, pd,(size_t)bytes,proc);
        issued++;
        cur_get++;
        if(cur_get>=max_pending)cur_get=0;
      }
    }
    if(issued != count)
       armci_die2("armci-elan getv:mismatch %d %d \n", count,issued);

    for(i=0; i<max_pending; i++) if(get_dscr[i]){
        elan_wait(get_dscr[i],100);
        get_dscr[i]=(ELAN_EVENT*)0;
    }
}


void armcill_wait_get()
{
int i;
    
    if(!pending_get)return;
    else pending_get=0;
    for(i=0; i<max_pending; i++) if(get_dscr[i]){
        elan_wait(get_dscr[i],100); 
        get_dscr[i]=(ELAN_EVENT*)0;
    }
}


void armcill_wait_put()
{
int i;
    if(!pending_put)return;
    else pending_put=0;
    for(i=0; i<max_pending; i++) if(put_dscr[i]){
        elan_wait(put_dscr[i],100); 
        put_dscr[i]=(ELAN_EVENT*)0;
    }
}

#endif
#endif

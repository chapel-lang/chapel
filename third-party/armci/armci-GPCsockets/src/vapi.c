/* $Id: vapi.c,v 1.29 2006-06-05 21:13:13 vinod Exp $ */
/* 
   File organized as follows
*/

#include <stdio.h>
#include <strings.h>
#include <assert.h>
#include <unistd.h>

#include "armcip.h"
#include "copy.h"
#include "request.h"
#include "armci-vapi.h"
#define DEBUG_INIT 0
#define DEBUG_FINALIZE 0
#define DEBUG_SERVER 0
#define DEBUG_CLN 0
#define TIME_INIT 0
/* The device name is "InfiniHost0" */
#  define VAPIDEV_NAME "InfiniHost0"
#  define INVAL_HNDL 0xFFFFFFFF


u_int32_t armci_max_num_sg_ent;
u_int32_t armci_max_qp_ous_swr;
u_int32_t armci_max_qp_ous_rwr;

typedef struct {
   VAPI_qp_hndl_t qp;
   VAPI_qp_prop_t qp_prop;         /*mostly for getting scq num*/
   VAPI_qp_num_t sqpnum;           /*we need to exchng qp nums,arr for that*/
   VAPI_qp_num_t *rqpnum;          /*we need rqp nums,arr for that*/
   IB_lid_t lid;
} armci_connect_t;
armci_connect_t *CLN_con,*SRV_con;
VAPI_hca_id_t   hca_id= VAPIDEV_NAME;
/*\
 * datastrucure for infinihost NIC
\*/
typedef struct {
  VAPI_hca_hndl_t handle;           /*IB nic handle*/
  VAPI_hca_id_t   hca_id;
  VAPI_hca_vendor_t vendor;
  VAPI_hca_cap_t attr;              /*IB nic attributes*/
  VAPI_pd_hndl_t ptag;              /*protection tag*/
  VAPI_hca_port_t hca_port;         /*mostly for getting lid*/
  IB_port_t active_port;
  VAPI_cq_hndl_t scq;               /*send completion queue*/
  VAPI_cq_hndl_t rcq;               /*recv completion queue*/
  IB_lid_t *lid_arr;                /*we need to exchange lids, arr for that*/
  VAPI_qp_num_t rqpnum;            /*we need to exchng qp nums,arr for that*/
  EVAPI_compl_handler_hndl_t rcq_eventh;
  int maxtransfersize; 
} vapi_nic_t;

typedef struct {
  armci_vapi_memhndl_t *prem_handle; /*address server to store memory handle*/ 
  armci_vapi_memhndl_t handle;
}ack_t;

armci_vapi_memhndl_t *CLN_handle;
armci_vapi_memhndl_t serv_memhandle, client_memhandle;
armci_vapi_memhndl_t *handle_array;
armci_vapi_memhndl_t *pinned_handle;

static vapi_nic_t nic_arr[3];
static vapi_nic_t *SRV_nic= nic_arr;
static vapi_nic_t *CLN_nic= nic_arr+1;
static int armci_server_terminating;

#define NONE -1
static int armci_ack_proc=NONE;

static int armci_vapi_server_ready;
static int armci_vapi_server_stage1=0;
static int armci_vapi_client_stage1=0;
static int armci_vapi_server_stage2=0;
static int armci_vapi_client_ready;
int _s=-1,_c=-1;
static int server_can_poll=0;
static int armci_vapi_max_inline_size=-1;
#define CLIENT_STAMP 101
#define SERV_STAMP 99 

static char * client_tail;
static char * serv_tail;
static ack_t *SRV_ack;
typedef struct {
	  VAPI_rr_desc_t dscr;
	  VAPI_sg_lst_entry_t    sg_entry;
	  char buf[VBUF_DLEN];
}vapibuf_t;


typedef struct {
          VAPI_sr_desc_t         snd_dscr;
          VAPI_sg_lst_entry_t    ssg_entry;
          VAPI_rr_desc_t         rcv_dscr;
          VAPI_sg_lst_entry_t    rsg_entry;
          char buf[VBUF_DLEN];
}vapibuf_ext_t;


typedef struct {
        VAPI_sr_desc_t rmw_dscr;
        VAPI_sg_lst_entry_t rmw_entry;
}vapirmw_t;


static vapibuf_t **serv_buf_arr, *spare_serv_buf,*spare_serv_bufptr;
static vapibuf_ext_t *serv_buf;

static vapirmw_t rmw[64];

static int *flag_arr; /* flag indicates its receiving scatter data */
#define SERV 2
#define CLN 1
/*typedef enum TYPES{CLN=1,SERV}; */



#define MAX_DESCR 2
typedef struct { 
    int avail; VAPI_qp_hndl_t qp; VAPI_rr_desc_t *descr;
} descr_pool_t;

static int* _gtmparr;
static void* test_ptr;
static int test_stride_arr[1];
static int test_count[2];
static int test_stride_levels;

char *MessageRcvBuffer;

extern void armci_util_wait_int(volatile int *,int,int);
void armci_send_data_to_client(int proc, void *buf,int bytes,void *dbuf);
void armci_server_register_region(void *,long,ARMCI_MEMHDL_T *);
static descr_pool_t serv_descr_pool = {MAX_DESCR, 0, (VAPI_rr_desc_t *)0};
static descr_pool_t client_descr_pool = {MAX_DESCR,0,(VAPI_rr_desc_t *)0};

#define GET_DATA_PTR(buf) (sizeof(request_header_t) + (char*)buf)

#define BUF_TO_SDESCR(buf) ((VAPI_sr_desc_t *)(&((armci_vapi_field_t *)((char *)(buf) - sizeof(armci_vapi_field_t)))->sdscr))

#define BUF_TO_RDESCR(buf) ((VAPI_rr_desc_t *)(&((armci_vapi_field_t *)((char *)(buf) - sizeof(armci_vapi_field_t)))->rdscr))

#define BUF_TO_SSGLST(buf) ((VAPI_sg_lst_entry_t *)(&((armci_vapi_field_t *)((char *)(buf) - sizeof(armci_vapi_field_t)))->ssg_entry))

#define BUF_TO_RSGLST(buf) ((VAPI_sg_lst_entry_t *)(&((armci_vapi_field_t *)((char *)(buf) - sizeof(armci_vapi_field_t)))->rsg_entry))

#define BUF_TO_EVBUF(buf) (vapibuf_ext_t*)(((char*)buf) - (sizeof(VAPI_sr_desc_t)+sizeof(VAPI_rr_desc_t)+2*sizeof(VAPI_sg_lst_entry_t)))

#define SERVER_SEND_ACK(p) {*((long *)serv_buf->buf)=ARMCI_VAPI_COMPLETE;armci_send_data_to_client((p),serv_buf->buf,sizeof(long),msginfo->tag.ack_ptr);}


/*\ descriptors will have unique ID's for the wait on descriptor routine to 
 * complete a descriptor and know where it came from
\*/

#define NUMOFBUFFERS 20
#define DSCRID_FROMBUFS 1
#define DSCRID_FROMBUFS_END DSCRID_FROMBUFS+NUMOFBUFFERS

#define DSCRID_NBDSCR 10000
#define DSCRID_NBDSCR_END (10000+MAX_PENDING)

#define DSCRID_SCATGAT 20000
#define DSCRID_SCATGAT_END 20000+MAX_PENDING

#define DSCRID_RMW 30000
#define DSCRID_RMW_END 30000+9999

extern double MPI_Wtime();
static double inittime0=0,inittime1=0,inittime2=0,inittime3=0,inittime4=0;

static int mark_buf_send_complete[NUMOFBUFFERS+1];
static sr_descr_t armci_vapi_client_nbsdscr_array[MAX_PENDING];
static sr_descr_t armci_vapi_client_nbrdscr_array[MAX_PENDING];
static sr_descr_t armci_vapi_serv_nbsdscr_array[MAX_PENDING];
static sr_descr_t armci_vapi_serv_nbrdscr_array[MAX_PENDING];

void armci_server_transport_cleanup();


/*********************FUNCTIONS TO CHECK VAPI RETURN STATUS********************/
void armci_check_status(int debug,VAPI_ret_t rc,char *msg)
{
    if(rc != VAPI_OK){
       char buf[100];
       if(armci_server_terminating){
         armci_server_transport_cleanup();
         /* server got interrupted when clients terminate connections */
         sleep(1);
         _exit(0);
       }
       printf("%d in check FAILURE %s\n",armci_me,msg);fflush(stdout);
       assert(strlen(msg)<100-20);
       sprintf(buf,"ARMCI(vapi):failure:%d:%s code %d %d ",rc,msg,
               _s,_c);
#     ifdef  PAUSE_ON_ERROR
       printf("%d(%d): Error from VIPL: %s - pausing\n",
              armci_me, getpid(), msg);
       fflush(stdout);
       pause();
#     endif
       armci_die(buf,(int)rc);
    }else if(debug){
       printf("%d:ARMCI(vapi): %s successful\n",armci_me,msg);
       fflush(stdout);
    }
}
void armci_vapi_check_return(int debug,VAPI_ret_t ret, const char *ss)
{
    if(ret!=VAPI_OK){
       printf("\n%d:from %s ret=%d str=%s str_sym=%s\n",armci_me,ss,ret,
		       VAPI_strerror(ret),VAPI_strerror_sym(ret));
       fflush(stdout);
    }
    if(debug){
       printf("\n%d:from %s ret=%d str=%s str_sym=%s\n",armci_me,ss,ret,
		       VAPI_strerror(ret),VAPI_strerror_sym(ret));
    }
}
void armci_vapi_print_dscr_info(VAPI_sr_desc_t *sr,VAPI_rr_desc_t *rr)
{
int i;
    if(rr){
       printf("\n%d:print_dscr id=%d opcode=%d sg_lst_len=%d",armci_me,
                       rr->id,rr->opcode,rr->sg_lst_len);
       for(i=0;i<rr->sg_lst_len;i++){
         printf("\n\t:sg_entry=%d addr=%p len=%d",i,rr->sg_lst_p[i].addr,
                         rr->sg_lst_p[i].len);
       }
       fflush(stdout);
    }
    if(sr){
       printf("\n%d:print_dscr id=%d opcode=%d sg_lst_len=%d",armci_me,
                       sr->id,sr->opcode,sr->sg_lst_len);
       for(i=0;i<sr->sg_lst_len;i++){
         printf("\n\t:sg_entry=%d addr=%p len=%d",i,sr->sg_lst_p[i].addr,
                         sr->sg_lst_p[i].len);
       }
       fflush(stdout);
    }
          

}

/*****************END FUNCTIONS TO CHECK VAPI RETURN STATUS********************/


void armci_recv_complete(VAPI_rr_desc_t *rcv_dscr, char *from, int numofrecvs)
{
VAPI_ret_t rc=VAPI_CQ_EMPTY;
VAPI_wc_desc_t pdscr1;
VAPI_wc_desc_t *pdscr = &pdscr1;
sr_descr_t *rdscr_arr;
vapi_nic_t *nic;
int debug,i;

    if(SERVER_CONTEXT){
       rdscr_arr = armci_vapi_serv_nbrdscr_array;
       nic=CLN_nic;
       debug = DEBUG_SERVER;
    }
    else{
       rdscr_arr = armci_vapi_client_nbrdscr_array;
       nic=SRV_nic;
       debug = DEBUG_CLN;
    }
    if(debug){
       printf("\n%d%s:recv_complete called from %s id=%ld\n",armci_me,
               ((SERVER_CONTEXT)?"(s)":" "),from,rcv_dscr->id);fflush(stdout);
    }
    for(i=0;i<numofrecvs;i++){ 
    do{
       while(rc == VAPI_CQ_EMPTY)     
         rc = VAPI_poll_cq(nic->handle, nic->rcq, pdscr);
       armci_check_status(debug,rc,"client_scatter_rcv");
       if(debug){
         if(pdscr->id >= DSCRID_SCATGAT && pdscr->id < DSCRID_SCATGAT_END)
           printf("\n%d:recv from %s complete id=%d num=%d",armci_me,
             from,pdscr->id,rdscr_arr[pdscr->id-DSCRID_SCATGAT].numofrecvs);
       }
       if(pdscr->id >= DSCRID_SCATGAT && pdscr->id < DSCRID_SCATGAT_END){
         rdscr_arr[pdscr->id-DSCRID_SCATGAT].numofrecvs--;
         if(rdscr_arr[pdscr->id-DSCRID_SCATGAT].numofrecvs==0)
           rdscr_arr[pdscr->id-DSCRID_SCATGAT].tag=0;
       }
       else if(pdscr->id == (DSCRID_SCATGAT + MAX_PENDING)){
               /*this was from a blocking call, do nothing*/
         continue;
       }
       else {
         armci_die("\nclient should be posting only one kind of recv",armci_me);
       }
       rc = VAPI_CQ_EMPTY;
   }while(pdscr->id!=rcv_dscr->id);
       rc = VAPI_CQ_EMPTY;
   }
}

void armci_vapi_set_mark_buf_send_complete(int id)
{
    mark_buf_send_complete[id]=0;
}

void armci_send_complete(VAPI_sr_desc_t *snd_dscr, char *from,int numoftimes)
{
VAPI_ret_t rc=VAPI_CQ_EMPTY;
VAPI_wc_desc_t pdscr1;
VAPI_wc_desc_t *pdscr=&pdscr1;
sr_descr_t *sdscr_arr;
vapi_nic_t *nic;
int debug,i;

    if(SERVER_CONTEXT){
       sdscr_arr = armci_vapi_serv_nbsdscr_array;
       nic=CLN_nic;
       debug = DEBUG_SERVER;
    }
    else{
       sdscr_arr = armci_vapi_client_nbsdscr_array;
       nic=SRV_nic;
       debug = DEBUG_CLN;
    }

    if(debug){
       printf("\n%d%s:send_complete called from %s id=%ld nt=%d\n",armci_me,
               ((SERVER_CONTEXT)?"(s)":" "),from,snd_dscr->id,numoftimes);fflush(stdout);
    }
    for(i=0;i<numoftimes;i++){
    do{
       while(rc == VAPI_CQ_EMPTY){  
         rc = VAPI_poll_cq(nic->handle, nic->scq, pdscr);
       }  
       if(SERVER_CONTEXT){
         armci_check_status(DEBUG_SERVER,rc,"armci_send_complete wait fr send");
         if(debug)printf("%d:completed id %d i=%d\n",armci_me,pdscr->id,i);
         if(pdscr->id >=DSCRID_SCATGAT && pdscr->id < DSCRID_SCATGAT_END){
           sdscr_arr[pdscr->id-DSCRID_SCATGAT].numofsends--;
           if(sdscr_arr[pdscr->id-DSCRID_SCATGAT].numofsends==0)
             sdscr_arr[pdscr->id-DSCRID_SCATGAT].tag=0;
         }
         else if(pdscr->id >=armci_nproc && pdscr->id < 2*armci_nproc){
                 /*its coming from send_data_to_client just return*/
         }
         else armci_die("server send complete got weird id",pdscr->id);
       }
       else{
         armci_check_status(DEBUG_CLN,rc,"armci_send_complete wait for send");
         if(debug)printf("%d:completed id %d i=%d\n",armci_me,pdscr->id,i);
         if(pdscr->id >=DSCRID_FROMBUFS && pdscr->id < DSCRID_FROMBUFS_END)
           mark_buf_send_complete[pdscr->id]=1;
         else if(pdscr->id >=DSCRID_NBDSCR && pdscr->id < DSCRID_NBDSCR_END){
           sdscr_arr[pdscr->id-DSCRID_NBDSCR].tag=0;
           sdscr_arr[pdscr->id-DSCRID_NBDSCR].numofsends=0;
         }
         else if(pdscr->id >=DSCRID_SCATGAT && pdscr->id < DSCRID_SCATGAT_END){
           sdscr_arr[pdscr->id-DSCRID_SCATGAT].numofsends--;
           if(sdscr_arr[pdscr->id-DSCRID_SCATGAT].numofsends==0)
             sdscr_arr[pdscr->id-DSCRID_SCATGAT].tag=0;
         }
         else if(pdscr->id == (DSCRID_SCATGAT + MAX_PENDING)){
           /*this was from a blocking call, do nothing*/
           continue;
         }
         else armci_die("client send complete got weird id",pdscr->id);
       }
       rc = VAPI_CQ_EMPTY;
    }while(pdscr->id!=snd_dscr->id);
       rc = VAPI_CQ_EMPTY;
    }
}


void armci_dscrlist_recv_complete(int tag, char* from,sr_descr_t *dscr)
{
int i,nr,j;
sr_descr_t *retdscr,*rdscr_arr;
    if(dscr==NULL){
       if(SERVER_CONTEXT)
         rdscr_arr = armci_vapi_serv_nbrdscr_array;
       else
         rdscr_arr = armci_vapi_client_nbrdscr_array;

       for(i=0;i<MAX_PENDING;i++){
         if(rdscr_arr[i].tag==tag)
           break;
       }

       if(i==MAX_PENDING)return;
       retdscr = &rdscr_arr[i];
    }
    else
       retdscr=dscr;

    nr = retdscr->numofrecvs;
    armci_recv_complete(&(retdscr->rdescr),"(s)list_send_complete",nr);
}


void armci_dscrlist_send_complete(int tag,char *from, sr_descr_t *dscr)
{
int i,ns,j;
sr_descr_t *retdscr,*sdscr_arr;
    if(dscr==NULL){
       if(SERVER_CONTEXT)
         sdscr_arr = armci_vapi_serv_nbsdscr_array;
       else
         sdscr_arr = armci_vapi_client_nbsdscr_array;

       for(i=0;i<MAX_PENDING;i++){
         if(sdscr_arr[i].tag==tag)
           break;
       }
       if(i==MAX_PENDING)return;
       retdscr=&sdscr_arr[i];
    }
    else
       retdscr=dscr;

#if 0
    printf("\n%d:i=%d tag=%d sdscr tag=%d id=%d",armci_me,i,tag,sdscr_arr[i].tag
                    ,sdscr_arr[i].sdescr.id);
#endif

    ns = retdscr->numofsends;

    armci_send_complete(&(retdscr->sdescr),"dscrlist_send_complete",ns);

}

void armci_client_nbcall_complete(sr_descr_t *dscr, int tag, int op)
{
    if(tag != dscr->tag)return;
    if(op==GET){
       if(dscr->issg){
         if(dscr->numofrecvs>0)
           armci_dscrlist_recv_complete(tag,"armci_client_nbcall_complete recv",
                           dscr);
       }
       else{
         if(dscr->numofsends>0)
           armci_dscrlist_send_complete(tag,"armci_client_nbcall_complete send",
                           dscr);
       }
    }
    if(op==PUT){
       if(dscr->numofsends>0)
         armci_dscrlist_send_complete(tag,"armci_client_nbcall_complete send",
                         dscr);
    }
}


static int cur_serv_pend_descr;
static int cur_client_pend_descr;

sr_descr_t *armci_vapi_get_next_rdescr(int nbtag,int sg)
{
static int serverthreadavail=-1; /*client thread can't touch this*/
static int clientthreadavail=-1; /*server thread can't touch this*/
int avail,newavail;
sr_descr_t *retdscr,*rdscr_arr;

    if(SERVER_CONTEXT){
       rdscr_arr = armci_vapi_serv_nbrdscr_array;
       avail = serverthreadavail;
       /*printf("\n%d:serv thread avail=%d",armci_me,serverthreadavail);*/
    }
    else{
       rdscr_arr = armci_vapi_client_nbrdscr_array;
       avail = clientthreadavail;
    }
    if(avail==-1){
       int i;
       for(i=0;i<MAX_PENDING;i++){
         rdscr_arr[i].tag=0;
         bzero(&rdscr_arr[i].rdescr,sizeof(VAPI_rr_desc_t)); 
         if(sg)
           rdscr_arr[i].rdescr.id = DSCRID_SCATGAT + i;
         else
           rdscr_arr[i].rdescr.id = DSCRID_NBDSCR + i; 
       }
       avail=0;
    }

    if(rdscr_arr[avail].tag!=0){
       armci_dscrlist_recv_complete(rdscr_arr[avail].tag,
                         "armci_vapi_get_next_rdescr",&rdscr_arr[avail]);
    }

    rdscr_arr[avail].tag=nbtag;
    rdscr_arr[avail].issg=sg;
    retdscr= (rdscr_arr+avail);

    memset(&retdscr->rdescr,0,sizeof(VAPI_rr_desc_t));

    if(sg)
       retdscr->rdescr.id = DSCRID_SCATGAT + avail;
    else{
       retdscr->rdescr.id = DSCRID_NBDSCR + avail; 
       retdscr->numofrecvs=1;
    }

    newavail = (avail+1)%MAX_PENDING;

    if(SERVER_CONTEXT){
      cur_serv_pend_descr = avail;
      serverthreadavail=newavail;
    }
    else{
      cur_client_pend_descr = avail;
      clientthreadavail=newavail;
    }

    return(retdscr);

}

sr_descr_t *armci_vapi_get_next_sdescr(int nbtag,int sg)
{
static int serverthreadavail=-1; /*client thread can't touch this*/
static int clientthreadavail=-1; /*server thread can't touch this*/
int avail,newavail;
sr_descr_t *retdscr,*sdscr_arr;

    if(SERVER_CONTEXT){
       sdscr_arr = armci_vapi_serv_nbsdscr_array;
       avail = serverthreadavail;
    }
    else{
       sdscr_arr = armci_vapi_client_nbsdscr_array;
       avail = clientthreadavail;
    }

    if(avail==-1){ /*first call*/
       int i;
       for(i=0;i<MAX_PENDING;i++){
         sdscr_arr[i].tag=0;
         bzero(&sdscr_arr[i].sdescr,sizeof(VAPI_sr_desc_t)); 
	 if(sg)
           sdscr_arr[i].sdescr.id = DSCRID_SCATGAT+i;
	 else
           sdscr_arr[i].sdescr.id = DSCRID_NBDSCR + i; 
       }
       avail=0;
    }

    if(sdscr_arr[avail].tag!=0){
       armci_dscrlist_send_complete(sdscr_arr[avail].tag,
                       "armci_vapi_get_next_sdescr",&sdscr_arr[avail]);
    }

    sdscr_arr[avail].tag=nbtag;
    sdscr_arr[avail].issg=sg;
    retdscr= (sdscr_arr+avail);

    memset(&retdscr->sdescr,0,sizeof(VAPI_sr_desc_t));

    if(sg)
       retdscr->sdescr.id = DSCRID_SCATGAT + avail;
    else{
       retdscr->sdescr.id = DSCRID_NBDSCR + avail;
       retdscr->numofsends=1;
    }

    newavail = (avail+1)%MAX_PENDING;

    if(SERVER_CONTEXT){
      cur_serv_pend_descr = avail;
      serverthreadavail=newavail;
    }
    else{
      cur_client_pend_descr = avail;
      clientthreadavail=newavail;
    }
    /*
    printf("\n%d:avail=%d newavail=%d cln=%d serv=%d",armci_me,avail,
      newavail,clientthreadavail,serverthreadavail); 
    */
    return(retdscr);
}

void armci_wait_for_server()
{
    armci_server_terminating=1;
}


/*\
 * create QP == create VI in via
\*/
static void armci_create_qp(vapi_nic_t *nic,VAPI_qp_hndl_t *qp,VAPI_qp_prop_t *qp_prop)
{
call_result_t rc;
VAPI_qp_init_attr_t initattr;

    bzero(&initattr, sizeof(VAPI_qp_init_attr_t));
    *qp=INVAL_HNDL;

    if(DEBUG_INIT){
       printf("\n%d:max wqe=%d max sglist=%d maxcq=%d\n",armci_me,
               nic->attr.max_qp_ous_wr,nic->attr.max_num_sg_ent,
               nic->attr.max_num_ent_cq);
    }

    initattr.cap.max_oust_wr_rq = armci_max_qp_ous_rwr;
    initattr.cap.max_oust_wr_sq = armci_max_qp_ous_swr;

    initattr.cap.max_sg_size_rq = armci_max_num_sg_ent;
    initattr.cap.max_sg_size_sq = armci_max_num_sg_ent;
    initattr.pd_hndl            = nic->ptag;
    initattr.rdd_hndl           = VAPI_INVAL_HNDL;
    initattr.rq_cq_hndl         = nic->rcq;
    initattr.sq_cq_hndl         = nic->scq;
    initattr.rq_sig_type        = VAPI_SIGNAL_REQ_WR;
    initattr.sq_sig_type        = VAPI_SIGNAL_REQ_WR;
    initattr.ts_type            = IB_TS_RC;
   
    if(DEBUG_INIT){
       printf("\n%d:here in create_qp before call\n",armci_me);
       printf("\n%d:nic=%p,qp=%p,prop=%p\n",armci_me,nic,qp,qp_prop);
    }
    rc = VAPI_create_qp(nic->handle, &initattr, qp, qp_prop);
    if(!armci_vapi_max_inline_size){
       armci_vapi_max_inline_size = qp_prop->cap.max_inline_data_sq;
       if(DEBUG_CLN){
         printf("\n%d:maxinline size=%d\n",armci_me,armci_vapi_max_inline_size);
         printf("\n%d:swr=%d rwr=%d ssg=%d rsg=%d\n",armci_me,qp_prop->cap.max_oust_wr_sq,qp_prop->cap.max_oust_wr_rq,qp_prop->cap.max_sg_size_sq,qp_prop->cap.max_sg_size_rq);fflush(stdout);
       }
    }

    armci_check_status(DEBUG_INIT, rc,"create qp");

}

static void armci_init_nic(vapi_nic_t *nic, int scq_entries, int rcq_entries)
{
VAPI_ret_t rc;
VAPI_cqe_num_t num;
int i;

    bzero(nic,sizeof(vapi_nic_t));
    /*hca_id = VAPIDEV_NAME;*/
    nic->lid_arr    = (IB_lid_t *)calloc(armci_nproc,sizeof(IB_lid_t));
    if(!nic->lid_arr)
       armci_die("malloc for nic_t arrays in vapi.c failed",0);

    /*first open nic, this is not necessary, says document*/ 
    rc = VAPI_open_hca(hca_id, &nic->handle);
    /*armci_check_status(DEBUG_INIT, rc,"open nic");*/

    rc = EVAPI_get_hca_hndl(hca_id, &nic->handle);
    armci_check_status(DEBUG_INIT, rc,"get handle");

    nic->maxtransfersize = MAX_RDMA_SIZE;

    rc = VAPI_query_hca_cap(nic->handle, &nic->vendor, &nic->attr);
    armci_check_status(DEBUG_INIT, rc,"query nic");

    /*query nic port basically for lid, lid in IB is required*/
    for(i = 1; i <= 2; i++) {
       rc = VAPI_query_hca_port_prop(nic->handle,(IB_port_t)i,
                             &(nic->hca_port));
       armci_check_status(DEBUG_INIT, rc,"query for lid");
       if(PORT_ACTIVE == nic->hca_port.state) {
         nic->active_port = i;
         break;
       }
    }


    /*save the lid for doing a global exchange later */
    nic->lid_arr[armci_me] = nic->hca_port.lid;

    /*allocate tag (protection domain) */
    rc = VAPI_alloc_pd(nic->handle, &nic->ptag);
    armci_check_status(DEBUG_INIT, rc,"create protection domain");

    /*properties of scq and rcq required for the cq number, this also needs 
     * to be globally exchanged
     */
    nic->scq = INVAL_HNDL;
    nic->rcq = INVAL_HNDL;
    /*do the actual queue creation */
    if(scq_entries){
       rc = VAPI_create_cq(nic->handle,4000, 
                           &nic->scq,&num);
       armci_check_status(DEBUG_INIT, rc,"create send completion queue");
    }
    if(rcq_entries){
       /*rc = VAPI_create_cq(nic->handle,(VAPI_cqe_num_t)rcq_entries, 
                           &nic->rcq,&num);*/
       rc = VAPI_create_cq(nic->handle,4000, 
                           &nic->rcq,&num);
       armci_check_status(DEBUG_INIT, rc,"create recv completion queue");
    }
    /*VAPIErrorCallback(nic->handle, 0, armci_err_callback);*/
    /*set local variable values*/
    armci_max_num_sg_ent=30;
    armci_max_qp_ous_swr=800;
    armci_max_qp_ous_rwr=400*16;
    if(armci_nproc>200){
       armci_max_qp_ous_swr=armci_nproc;
       armci_max_qp_ous_rwr=armci_nproc*16;
       if(armci_me==0)printf("\nARMCI:%d:qp_swr changed to %d qp_rwr changed to %d",armci_me,armci_max_qp_ous_swr,armci_max_qp_ous_rwr);
    }
    if(armci_max_qp_ous_rwr>nic->attr.max_qp_ous_wr){
       armci_max_qp_ous_swr=nic->attr.max_qp_ous_wr/16;
       armci_max_qp_ous_rwr=nic->attr.max_qp_ous_wr;
       if(armci_me==0)printf("\nARMCI:%d:qp_swr changed to %d qp_rwr changed to %d",armci_me,armci_max_qp_ous_swr,armci_max_qp_ous_rwr);
    }
    if(armci_max_num_sg_ent>nic->attr.max_num_sg_ent)
       armci_max_num_sg_ent=nic->attr.max_num_sg_ent;
#if 0
    for(i=0;i<MAX_PENDING;i++){
    }
#endif

}

/****************MEMORY ALLOCATION REGISTRATION DEREGISTRATION****************/
static char * serv_malloc_buf_base;
#ifdef ARMCI_ENABLE_GPC_CALLS
extern gpc_buf_t *gpc_req;
#endif
void armci_server_alloc_bufs()
{
VAPI_ret_t rc;
VAPI_mrw_t mr_in,mr_out;
int mod, bytes, total, extra =sizeof(VAPI_rr_desc_t)*MAX_DESCR+SIXTYFOUR;
int mhsize = armci_nproc*sizeof(armci_vapi_memhndl_t); /* ack */
char *tmp, *tmp0; 
int clients = armci_nproc,i,j=0;
  
    /* allocate memory for the recv buffers-must be alligned on 64byte bnd */
    /* note we add extra one to repost it for the client we are received req */ 
    bytes =(clients+1)*sizeof(vapibuf_t)+sizeof(vapibuf_ext_t) + extra+ mhsize
#if ARMCI_ENABLE_GPC_CALLS
      + MAX_GPC_REQ * sizeof(gpc_buf_t) 
#endif
      + SIXTYFOUR;
    total = bytes + SIXTYFOUR;
    if(total%4096!=0) 
       total = total - (total%4096) + 4096;
    tmp0=tmp = VMALLOC(total);
    serv_malloc_buf_base = tmp0;

    if(!tmp) armci_die("failed to malloc server vapibufs",total);
    /* stamp the last byte */
    serv_tail= tmp + bytes+SIXTYFOUR-1;
    *serv_tail=SERV_STAMP;
    /* allocate memory for client memory handle to support put response 
     *         in dynamic memory registration protocols */
    CLN_handle = (armci_vapi_memhndl_t*)tmp;
    memset(CLN_handle,0,mhsize); /* set it to zero */
    tmp += mhsize;
    /* gpc_req memory*/
    tmp += SIXTYFOUR - ((ssize_t)tmp % SIXTYFOUR);
#ifdef ARMCI_ENABLE_GPC_CALLS
    gpc_req = (gpc_buf_t *)tmp;
    tmp += MAX_GPC_REQ * sizeof(gpc_buf_t);
#endif
    /* setup descriptor memory */
    mod = ((ssize_t)tmp)%SIXTYFOUR;
    serv_descr_pool.descr= (VAPI_rr_desc_t*)(tmp+SIXTYFOUR-mod);
    tmp += extra;
    /* setup buffer pointers */
    mod = ((ssize_t)tmp)%SIXTYFOUR;
    serv_buf_arr = (vapibuf_t **)malloc(sizeof(vapibuf_t*)*armci_nproc);
    for(i=0;i<armci_nproc;i++){
       serv_buf_arr[i] = (vapibuf_t*)(tmp+SIXTYFOUR-mod) + j++;
    }
    i=0;
    while(serv_buf_arr[i]==NULL)i++;
    spare_serv_buf = serv_buf_arr[i]+clients; /* spare buffer is at the end */
    spare_serv_bufptr = spare_serv_buf;    /* save the pointer for later */
    serv_buf =(vapibuf_ext_t*)(serv_buf_arr[i]+clients+1);
    MessageRcvBuffer = serv_buf->buf;

    /* set up server_scatter descriptor memory */
    /*serv_scatter_arr = (VAPI_rr_desc_t *)malloc(sizeof(VAPI_rr_desc_t)*armci_nproc);*/
   
   flag_arr = (int *)malloc(sizeof(int)*armci_nproc); 
   for (i =0; i<armci_nproc; i++) flag_arr[i] = 9999; 
   
   /* setup memory attributes for the region */
    /*mr_in.acl =  VAPI_EN_LOCAL_WRITE|VAPI_EN_REMOTE_ATOM | VAPI_EN_REMOTE_WRITE |VAPI_EN_REMOTE_READ;*/
    mr_in.acl =  VAPI_EN_LOCAL_WRITE | VAPI_EN_REMOTE_WRITE | VAPI_EN_REMOTE_READ;
    mr_in.l_key = 0;
    mr_in.pd_hndl = CLN_nic->ptag;
    mr_in.r_key = 0;
    mr_in.size = total;
    mr_in.start = (VAPI_virt_addr_t)(MT_virt_addr_t)tmp0;
    mr_in.type = VAPI_MR;

    if(DEBUG_SERVER){
      printf("\n%d(s):registering mem %p %dbytes ptag=%ld handle=%d\n",
             armci_me, tmp0,total,CLN_nic->ptag,CLN_nic->handle);fflush(stdout);
    }

    rc = VAPI_register_mr(CLN_nic->handle,&mr_in,&(serv_memhandle.memhndl),&mr_out);
    armci_check_status(DEBUG_INIT, rc,"server register recv vbuf");
    
    serv_memhandle.lkey = mr_out.l_key;
    serv_memhandle.rkey = mr_out.r_key;
     
    /* exchange address of ack/memhandle flag on servers */

/*  I disabled it on 3/17/05 on system x
    if(!serv_memhandle.memhndl)armci_die("server got null handle for vbuf",0);
*/
    if(DEBUG_SERVER){
       printf("%d(s):registered mem %p %dbytes mhandle=%d mharr starts%p\n",
              armci_me, tmp0, total, serv_memhandle.memhndl,CLN_handle);
       fflush(stdout); 
    }
}

static char * client_malloc_buf_base;
char * armci_vapi_client_mem_alloc(int size)
{
VAPI_ret_t rc;
VAPI_mrw_t mr_in,mr_out;
int mod,  total;
int extra = MAX_DESCR*sizeof(VAPI_rr_desc_t)+SIXTYFOUR;
char *tmp,*tmp0;

    /*we use the size passed by the armci_init_bufs routine instead of bytes*/
    
    total = size + extra + 2*SIXTYFOUR;
    
    if(total%4096!=0)  
       total = total - (total%4096) + 4096;
    tmp0  = tmp = VMALLOC(total);
    client_malloc_buf_base = tmp;
    if(ALIGN64ADD(tmp0))tmp0+=ALIGN64ADD(tmp0);
    if(!tmp) armci_die("failed to malloc client bufs",total);
    /* stamp the last byte */
    client_tail= tmp + extra+ size +2*SIXTYFOUR-1;
    *client_tail=CLIENT_STAMP;

    /* we also have a place to store memhandle for zero-copy get */
    pinned_handle =(armci_vapi_memhndl_t *) (tmp + extra+ size +SIXTYFOUR-16);

    mod = ((ssize_t)tmp)%SIXTYFOUR;
    client_descr_pool.descr= (VAPI_rr_desc_t*)(tmp+SIXTYFOUR-mod);
    tmp += extra;

    /*mr_in.acl =  VAPI_EN_LOCAL_WRITE|VAPI_EN_REMOTE_ATOM | VAPI_EN_REMOTE_WRITE |VAPI_EN_REMOTE_READ;*/
    mr_in.acl =  VAPI_EN_LOCAL_WRITE|VAPI_EN_REMOTE_WRITE | VAPI_EN_REMOTE_READ;
    mr_in.l_key = 0;
    mr_in.pd_hndl = SRV_nic->ptag;
    mr_in.r_key = 0;
    mr_in.size = total;
    mr_in.start = (VAPI_virt_addr_t)(MT_virt_addr_t)tmp0;
    mr_in.type = VAPI_MR;


    rc = VAPI_register_mr(SRV_nic->handle,&mr_in,&(client_memhandle.memhndl),
                          &mr_out);
    armci_check_status(DEBUG_INIT, rc,"client register snd vbuf");
    /*printf("\n%d(c):my lkey=%d",armci_me,mr_out.l_key);fflush(stdout);*/
/*  I disabled it on 3/17/05 on system x
    if(!client_memhandle.memhndl)armci_die("client got null handle for vbuf",total);
*/

    client_memhandle.lkey = mr_out.l_key;
    client_memhandle.rkey = mr_out.r_key;
    handle_array[armci_me].lkey = client_memhandle.lkey;
    handle_array[armci_me].rkey = client_memhandle.rkey;
    handle_array[armci_me].memhndl = client_memhandle.memhndl;

    /* lock allocated memory */
    /*mattr.EnableRdmaWrite = VIP_FALSE;
    rc = VipRegisterMem(SRV_nic->handle, tmp0, total, &mattr,&client_memhandle);
    armci_check_status(DEBUG_INIT, rc,"client register snd vbuf");
    if(!client_memhandle)armci_die("client got null handle for vbuf",0); */
    if(DEBUG_INIT){
       printf("%d: registered client memory %p %dsize tmp=%p \n",
               armci_me,tmp0, total, tmp);
       fflush(stdout);
    }
    /*now that we have the handle array, we get every body elses RDMA handle*/
    total = (sizeof(armci_vapi_memhndl_t)*armci_nproc)/sizeof(int);
    armci_msg_gop_scope(SCOPE_ALL,handle_array,total,"+",ARMCI_INT);

    return(tmp);

}


void armci_server_register_region(void *ptr,long bytes, ARMCI_MEMHDL_T *memhdl)
{
VAPI_ret_t rc;
VAPI_mrw_t mr_in,mr_out;
    bzero(memhdl,sizeof(ARMCI_MEMHDL_T));
    mr_in.acl =  VAPI_EN_LOCAL_WRITE | VAPI_EN_REMOTE_WRITE | VAPI_EN_REMOTE_READ;
    mr_in.l_key = 0;
    mr_in.pd_hndl = CLN_nic->ptag;
    mr_in.r_key = 0;
    mr_in.size = bytes;
    mr_in.start = (VAPI_virt_addr_t)(MT_virt_addr_t)ptr;
    mr_in.type = VAPI_MR;
    rc = VAPI_register_mr(CLN_nic->handle, &mr_in,&(serv_memhandle.memhndl), &mr_out);
    armci_check_status(DEBUG_INIT, rc,"server register region");
    memhdl->lkey = mr_out.l_key;
    memhdl->rkey = mr_out.r_key;
    if(DEBUG_SERVER){
       printf("\n%d(s):registered lkey=%d rkey=%d ptr=%p end=%p %p\n",armci_me,
               memhdl->lkey,memhdl->rkey,ptr,(char *)ptr+bytes,memhdl);
       fflush(stdout);
    }
}

int armci_pin_contig_hndl(void *ptr, int bytes, ARMCI_MEMHDL_T *memhdl)
{
VAPI_ret_t rc;
VAPI_mrw_t mr_in,mr_out;
    
    mr_in.acl =  VAPI_EN_LOCAL_WRITE | VAPI_EN_REMOTE_WRITE | VAPI_EN_REMOTE_READ;
    mr_in.l_key = 0;
    mr_in.pd_hndl = SRV_nic->ptag;
    mr_in.r_key = 0;
    mr_in.size = bytes;
    mr_in.start = (VAPI_virt_addr_t)(MT_virt_addr_t)ptr;
    mr_in.type = VAPI_MR;
    rc = VAPI_register_mr(SRV_nic->handle, &mr_in,&(memhdl->memhndl), &mr_out);
    armci_check_status(DEBUG_INIT, rc,"client register region");
    memhdl->lkey = mr_out.l_key;
    memhdl->rkey = mr_out.r_key;
    if(0){
       printf("\n%d:registered lkey=%d rkey=%d ptr=%p end=%p\n",armci_me,
               memhdl->lkey,memhdl->rkey,ptr,(char *)ptr+bytes);fflush(stdout);
    }
    return 1;
}

void armci_network_client_deregister_memory(ARMCI_MEMHDL_T *mh)
{
VAPI_ret_t rc;
    rc = VAPI_deregister_mr(SRV_nic->handle,mh->memhndl);
    armci_vapi_check_return(DEBUG_FINALIZE,rc,"armci_network_client_deregister_memory:deregister_mr");

}
void armci_network_server_deregister_memory(ARMCI_MEMHDL_T *mh)
{
VAPI_ret_t rc;
    return;
    printf("\n%d:deregister ptr=%p",armci_me,mh);fflush(stdout);
    rc = VAPI_deregister_mr(CLN_nic->handle,mh->memhndl);
    armci_vapi_check_return(DEBUG_FINALIZE,rc,"armci_network_server_deregister_memory:deregister_mr");
}


void armci_set_serv_mh()
{
int s, ratio = sizeof(ack_t)/sizeof(int);
    /* first collect addrresses on all masters */
    if(armci_me == armci_master){
       SRV_ack[armci_clus_me].prem_handle=CLN_handle;
       SRV_ack[armci_clus_me].handle =serv_memhandle;
       armci_msg_gop_scope(SCOPE_MASTERS,SRV_ack,ratio*armci_nclus,"+",
                           ARMCI_INT);
    }
    /* next master broadcasts the addresses within its node */
    armci_msg_bcast_scope(SCOPE_NODE,SRV_ack,armci_nclus*sizeof(ack_t),
                          armci_master);

    /* Finally save address corresponding to my id on each server */
    for(s=0; s< armci_nclus; s++){
       SRV_ack[s].prem_handle += armci_me;
       /*printf("%d: my addr on %d = %p\n",armci_me,s,SRV_ack[s].prem_handle);
         fflush(stdout); */
    }

}
/**********END MEMORY ALLOCATION REGISTRATION AND DEREGISTRATION**************/

/*\
 * init_connections, client_connect_to_servers -- client code
 * server_initial_connection, all_data_server -- server code 
\*/ 
void armci_init_connections()
{
int c,s;
int sz;
int *tmparr;
    if(TIME_INIT)inittime0 = MPI_Wtime(); 
    /* initialize nic connection for qp numbers and lid's */
    armci_init_nic(SRV_nic,1,1);
    bzero(mark_buf_send_complete,sizeof(int)*(NUMOFBUFFERS+1));
    _gtmparr = (int *)calloc(armci_nproc,sizeof(int)); 

    /*qp_numbers and lids need to be exchanged globally*/
    tmparr = (int *)calloc(armci_nproc,sizeof(int));
    tmparr[armci_me] = SRV_nic->lid_arr[armci_me];
    sz = armci_nproc;
    armci_msg_gop_scope(SCOPE_ALL,tmparr,sz,"+",ARMCI_INT);
    for(c=0;c<armci_nproc;c++){
       SRV_nic->lid_arr[c]=tmparr[c];
       tmparr[c]=0;
    }

    /*SRV_con is for client to connect to servers */
    SRV_con=(armci_connect_t *)malloc(sizeof(armci_connect_t)*armci_nclus);
    if(!SRV_con)armci_die("cannot allocate SRV_con",armci_nclus);
    bzero(SRV_con,sizeof(armci_connect_t)*armci_nclus);

    CLN_con=(armci_connect_t*)malloc(sizeof(armci_connect_t)*armci_nproc);
    if(!CLN_con)armci_die("cannot allocate SRV_con",armci_nproc);
    bzero(CLN_con,sizeof(armci_connect_t)*armci_nproc);

    /*every client creates a qp with every server other than the one on itself*/
    sz = armci_nproc*(sizeof(VAPI_qp_num_t)/sizeof(int));
    armci_vapi_max_inline_size = 0;
    for(s=0; s< armci_nclus; s++){
       armci_connect_t *con = SRV_con + s;
       con->rqpnum = (VAPI_qp_num_t *)malloc(sizeof(VAPI_qp_num_t)*armci_nproc);
       bzero(con->rqpnum,sizeof(VAPI_qp_num_t)*armci_nproc);
       /*if(armci_clus_me != s)*/
       {
         armci_create_qp(SRV_nic,&con->qp,&con->qp_prop);
         con->sqpnum  = con->qp_prop.qp_num;
         con->rqpnum[armci_me]  = con->qp_prop.qp_num;
         con->lid     = SRV_nic->lid_arr[s];
       }
       armci_msg_gop_scope(SCOPE_ALL,con->rqpnum,sz,"+",ARMCI_INT);
    }

    if(DEBUG_CLN) printf("%d: connections ready for client\n",armci_me);

    /* ............ masters also set up connections for clients ......... */
    SRV_ack = (ack_t*)calloc(armci_nclus,sizeof(ack_t));
    if(!SRV_ack)armci_die("buffer alloc failed",armci_nclus*sizeof(ack_t));

    handle_array = (armci_vapi_memhndl_t *)calloc(sizeof(armci_vapi_memhndl_t),armci_nproc);
    if(!handle_array)armci_die("handle_array malloc failed",0);
    if(TIME_INIT)printf("\n%d:time for init_conn is %f",armci_me,MPI_Wtime()-inittime2);


}

static void vapi_connect_client()
{
int i,start,sz=0,c;
call_result_t rc;
VAPI_qp_attr_t         qp_attr;
VAPI_qp_cap_t          qp_cap;
VAPI_qp_attr_mask_t    qp_attr_mask;
    if(TIME_INIT)inittime0=MPI_Wtime();
    if(armci_me==armci_master)
       armci_util_wait_int(&armci_vapi_server_stage1,1,10);
   if(TIME_INIT) printf("\n%d:wait for server to get to stage 1 time for vapi_connect_client is %f",armci_me,(inittime1=MPI_Wtime())-inittime0);
    sz = armci_nproc;
    if(armci_me==armci_master){
       armci_msg_gop_scope(SCOPE_MASTERS,_gtmparr,sz,"+",ARMCI_INT);
       for(c=0;c<armci_nproc;c++){
         CLN_nic->lid_arr[c]=_gtmparr[c];
         _gtmparr[c]=0;
       }
       if(DEBUG_CLN){
         printf("\n%d(svc): mylid = %d",armci_me,CLN_nic->lid_arr[armci_me]);
         fflush(stdout);
       }
    }


    armci_vapi_client_stage1 = 1;

    /* allocate and initialize connection structs */
    sz = armci_nproc*(sizeof(VAPI_qp_num_t)/sizeof(int));

    if(armci_me==armci_master)
       armci_util_wait_int(&armci_vapi_server_stage2,1,10);
    for(c=0; c< armci_nproc; c++){
       armci_connect_t *con = CLN_con + c;
       if(armci_me!=armci_master){
         char *ptrr;
         int extra;
         ptrr = malloc(8+sizeof(VAPI_qp_num_t)*armci_nproc);
         extra = ALIGNLONGADD(ptrr);
         ptrr = ptrr+extra;
         con->rqpnum=(VAPI_qp_num_t *)ptrr;
         bzero(con->rqpnum,sizeof(VAPI_qp_num_t)*armci_nproc);
       }
       armci_msg_gop_scope(SCOPE_ALL,con->rqpnum,sz,"+",ARMCI_INT);
    }

   if(TIME_INIT) printf("\n%d:wait for server tog et to stage 2 time for vapi_connect_client is %f",armci_me,(inittime2=MPI_Wtime())-inittime1);
    /*armci_set_serv_mh();*/

    if(DEBUG_CLN){printf("%d:all connections ready\n",armci_me);fflush(stdout);}

    /* Modifying  QP to INIT */
    QP_ATTR_MASK_CLR_ALL(qp_attr_mask);
    qp_attr.qp_state = VAPI_INIT;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_QP_STATE);
    qp_attr.pkey_ix  = DEFAULT_PKEY_IX;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_PKEY_IX);
    qp_attr.port     = SRV_nic->active_port;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_PORT);
    qp_attr.remote_atomic_flags = VAPI_EN_REM_WRITE | VAPI_EN_REM_READ;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_REMOTE_ATOMIC_FLAGS);

    /* start from from server on my_node -1 */
    start = (armci_clus_me==0)? armci_nclus-1 : armci_clus_me-1;
    for(i=0; i< armci_nclus; i++){
       armci_connect_t *con;
       con = SRV_con + i;
       rc = VAPI_modify_qp(SRV_nic->handle,(con->qp),&qp_attr, &qp_attr_mask, &qp_cap);
       armci_check_status(DEBUG_INIT, rc,"client connect requesti RST->INIT");
    }

    if(TIME_INIT)printf("\n%d:to init time for vapi_connect_client is %f",armci_me,(inittime1=MPI_Wtime())-inittime2);
    QP_ATTR_MASK_CLR_ALL(qp_attr_mask);
    qp_attr.qp_state = VAPI_RTR;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_QP_STATE);
    qp_attr.qp_ous_rd_atom = 4;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_QP_OUS_RD_ATOM);
    qp_attr.av.sl            = 0;
    qp_attr.av.grh_flag      = FALSE;
    qp_attr.av.static_rate   = 0; /* 1x */
    qp_attr.av.src_path_bits = 0;
    qp_attr.path_mtu         = MTU1024;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_PATH_MTU);
    qp_attr.rq_psn           = 0;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_RQ_PSN);
    qp_attr.pkey_ix = 0;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_PKEY_IX);
    qp_attr.min_rnr_timer = 0;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_MIN_RNR_TIMER);

    start = (armci_clus_me==0)? armci_nclus-1 : armci_clus_me-1;
    for(i=0; i< armci_nclus; i++){
       armci_connect_t *con;
       armci_connect_t *conS;
       con = SRV_con + i;
       conS = CLN_con + armci_me;
       qp_attr.dest_qp_num=conS->rqpnum[armci_clus_info[i].master];
       QP_ATTR_MASK_SET(qp_attr_mask, QP_ATTR_DEST_QP_NUM);
       qp_attr.av.dlid = SRV_nic->lid_arr[armci_clus_info[i].master];
       QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_AV);
       rc = VAPI_modify_qp(SRV_nic->handle,(con->qp),&qp_attr, &qp_attr_mask, 
                           &qp_cap);
       armci_check_status(DEBUG_INIT, rc,"client connect request INIT->RTR");
    }

    /*to to to RTS, other side must be in RTR*/
    armci_msg_barrier();
    if(TIME_INIT)printf("\n%d:init to rtr time for vapi_connect_client is %f",armci_me,(inittime2=MPI_Wtime())-inittime1);

    armci_vapi_client_ready=1; 

    QP_ATTR_MASK_CLR_ALL(qp_attr_mask);
    qp_attr.qp_state   = VAPI_RTS;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_QP_STATE);
    qp_attr.sq_psn   = 0;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_SQ_PSN);
    qp_attr.timeout   = 18;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_TIMEOUT);
    qp_attr.retry_count   = 1;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_RETRY_COUNT);
    qp_attr.rnr_retry     = 3;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_RNR_RETRY);
    qp_attr.ous_dst_rd_atom  = 128;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_OUS_DST_RD_ATOM);

    start = (armci_clus_me==0)? armci_nclus-1 : armci_clus_me-1;
    for(i=0; i< armci_nclus; i++){
       armci_connect_t *con;
       con = SRV_con + i;
       rc=VAPI_modify_qp(SRV_nic->handle,(con->qp),&qp_attr,&qp_attr_mask,
                         &qp_cap);
       armci_check_status(DEBUG_CLN, rc,"client connect request RTR->RTS");
    }
    if(TIME_INIT)printf("\n%d:rtr to rts time for vapi_connect_client is %f",armci_me,(inittime1=MPI_Wtime())-inittime2);

}

void armci_client_connect_to_servers()
{
    /* initialize buffer managment module */
    extern void armci_util_wait_int(volatile int *,int,int);
    if(TIME_INIT)inittime0=MPI_Wtime();
    _armci_buf_init();

    vapi_connect_client();
    if(armci_me==armci_master)
      armci_util_wait_int(&armci_vapi_server_ready,1,10);
    armci_msg_barrier();
    if(DEBUG_CLN && armci_me==armci_master){
       printf("\n%d:server_ready=%d\n",armci_me,armci_vapi_server_ready);
       fflush(stdout);
    }
    if(TIME_INIT)printf("\n%d:time for client_connect_to_s is %f",armci_me,MPI_Wtime()-inittime0);
}


void armci_init_vapibuf_recv(VAPI_rr_desc_t *rd,VAPI_sg_lst_entry_t *sg_entry, char* buf, int len, armci_vapi_memhndl_t *mhandle)
{
     memset(rd,0,sizeof(VAPI_rr_desc_t));
     rd->opcode = VAPI_RECEIVE;
     rd->comp_type = VAPI_SIGNALED;
     rd->sg_lst_len = 1;
     rd->sg_lst_p  = sg_entry;
     rd->id = 0;

     sg_entry->lkey = mhandle->lkey;
     sg_entry->addr = (VAPI_virt_addr_t)(MT_virt_addr_t)buf;
     sg_entry->len = len;
}



void armci_init_vapibuf_send(VAPI_sr_desc_t *sd,VAPI_sg_lst_entry_t *sg_entry, char* buf, int len, armci_vapi_memhndl_t *mhandle)
{
     sd->opcode = VAPI_SEND;
     sd->comp_type = VAPI_SIGNALED;
     sd->sg_lst_len = 1;
     sd->sg_lst_p  = sg_entry;
     /*sd->id = 0;*/
     sd->remote_qkey=0;
     sd->set_se = FALSE;
     sd->fence = FALSE;

     sg_entry->lkey = mhandle->lkey;
     sg_entry->addr = (VAPI_virt_addr_t)(MT_virt_addr_t)buf;
     sg_entry->len = len;
}


static void armci_init_vbuf_srdma(VAPI_sr_desc_t *sd, VAPI_sg_lst_entry_t *sg_entry, char* lbuf, char *rbuf, int len, armci_vapi_memhndl_t *lhandle,armci_vapi_memhndl_t *rhandle)
{
     sd->opcode = VAPI_RDMA_WRITE;
     sd->comp_type = VAPI_SIGNALED;
     sd->sg_lst_len = 1;
     sd->sg_lst_p  = sg_entry;
     /*sd->id = 0;*/
     sd->remote_qkey=0;
     if(rhandle)sd->r_key = rhandle->rkey;
     sd->remote_addr = (VAPI_virt_addr_t) (MT_virt_addr_t) rbuf;
     sd->set_se = FALSE;
     sd->fence = FALSE;

     if(lhandle)sg_entry->lkey = lhandle->lkey;
     sg_entry->addr = (VAPI_virt_addr_t)(MT_virt_addr_t)lbuf;
     sg_entry->len = len;
}



static void armci_init_vbuf_rrdma(VAPI_sr_desc_t *sd, VAPI_sg_lst_entry_t *sg_entry, char* lbuf, char *rbuf, int len, armci_vapi_memhndl_t *lhandle,armci_vapi_memhndl_t *rhandle)
{
     sd->opcode = VAPI_RDMA_READ;
     sd->comp_type = VAPI_SIGNALED;
     sd->sg_lst_len = 1;
     sd->sg_lst_p  = sg_entry;
     /*sd->id = 0;*/
     sd->remote_qkey=0;
     if(rhandle)sd->r_key = rhandle->rkey;
     sd->remote_addr = (VAPI_virt_addr_t) (MT_virt_addr_t) rbuf;
     sd->set_se = FALSE;
     sd->fence = FALSE;

     if(lhandle)sg_entry->lkey = lhandle->lkey;
     sg_entry->addr = (VAPI_virt_addr_t)(MT_virt_addr_t)lbuf;
     sg_entry->len = len;
}

#if 0
static void vapi_signal_comp_handler(VAPI_hca_hndl_t hca_hndl,
                                     VAPI_cq_hndl_t cq_hndl,void* sem_p)
{
  /*printf("%d:in comp handler",armci_me);fflush(stdout);
  MOSAL_sem_rel((MOSAL_semaphore_t*)sem_p);
  printf("%d:in comp handler - semaphore released",armci_me);fflush(stdout);
  */
}
#endif


void armci_server_initial_connection()
{
int c, ib;
VAPI_ret_t rc;
VAPI_qp_attr_t         qp_attr;
VAPI_qp_cap_t          qp_cap;
VAPI_qp_attr_mask_t    qp_attr_mask;
char *enval;
    if(TIME_INIT)inittime0=MPI_Wtime();
    if(DEBUG_SERVER){ 
       printf("in server after fork %d (%d)\n",armci_me,getpid());
       fflush(stdout);
    }

    armci_init_nic(CLN_nic,1,1);

    /*MOSAL_sem_init(&(res->rq_sem),0);*/

    _gtmparr[armci_me] = CLN_nic->lid_arr[armci_me];
    armci_vapi_server_stage1 = 1;
    armci_util_wait_int(&armci_vapi_client_stage1,1,10);
    if(TIME_INIT)printf("\n%d:wait for client time for server_initial_conn is %f",armci_me,(inittime4=MPI_Wtime())-inittime0);

    for(c=0; c< armci_nproc; c++){
       char *ptrr;
       int extra;
       armci_connect_t *con = CLN_con + c;
       if(DEBUG_SERVER){
         printf("\n%d:create qp before malloc c=%d\n",armci_me,c);
         fflush(stdout);
       }
       ptrr = malloc(8+sizeof(VAPI_qp_num_t)*armci_nproc);
       extra = ALIGNLONGADD(ptrr);
       ptrr = ptrr+extra;
       con->rqpnum = (VAPI_qp_num_t *)ptrr;
       bzero(con->rqpnum,sizeof(VAPI_qp_num_t)*armci_nproc);
       armci_create_qp(CLN_nic,&con->qp,&con->qp_prop);
       con->sqpnum  = con->qp_prop.qp_num;
       con->lid      = CLN_nic->lid_arr[c];
       con->rqpnum[armci_me]  = con->qp_prop.qp_num;
       if(DEBUG_SERVER){
         printf("\n%d:create qp success  for server c=%d\n",armci_me,c);fflush(stdout);
       }
    }
    if(DEBUG_SERVER){
       printf("\n%d:create qps success for server",armci_me);fflush(stdout);
    }
    if(TIME_INIT)printf("\n%d:create qp time for server_initial_conn is %f",armci_me,(inittime1=MPI_Wtime())-inittime4);

    armci_vapi_server_stage2 = 1;

    QP_ATTR_MASK_CLR_ALL(qp_attr_mask);
    qp_attr.qp_state = VAPI_INIT;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_QP_STATE);
    qp_attr.pkey_ix  = DEFAULT_PKEY_IX;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_PKEY_IX);
    qp_attr.port     = CLN_nic->active_port;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_PORT);
    qp_attr.remote_atomic_flags = VAPI_EN_REM_WRITE | VAPI_EN_REM_READ;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_REMOTE_ATOMIC_FLAGS);
    for(c=0; c< armci_nproc; c++){
       armci_connect_t *con = CLN_con + c;
       rc = VAPI_modify_qp(CLN_nic->handle,(con->qp),&qp_attr, &qp_attr_mask,
                           &qp_cap);
       armci_check_status(DEBUG_INIT, rc,"master connect request RST->INIT");
    }
    if(TIME_INIT)printf("\n%d:to init time for server_initial_conn is %f",armci_me,(inittime2=MPI_Wtime())-inittime1);
    QP_ATTR_MASK_CLR_ALL(qp_attr_mask);
    qp_attr.qp_state = VAPI_RTR;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_QP_STATE);
    qp_attr.qp_ous_rd_atom = 4;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_QP_OUS_RD_ATOM);
    qp_attr.av.sl            = 0;
    qp_attr.av.grh_flag      = FALSE;
    qp_attr.av.static_rate   = 0;                           /* 1x */
    qp_attr.av.src_path_bits = 0;
    qp_attr.path_mtu      = MTU1024;                        /*MTU*/
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_PATH_MTU);
    qp_attr.rq_psn           = 0;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_RQ_PSN);
    qp_attr.pkey_ix = 0;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_PKEY_IX);
    qp_attr.min_rnr_timer = 0;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_MIN_RNR_TIMER);

    for(c=0; c< armci_nproc; c++){
       armci_connect_t *con = CLN_con + c;
       armci_connect_t *conC = SRV_con + armci_clus_me;
       qp_attr.dest_qp_num=conC->rqpnum[c];
       QP_ATTR_MASK_SET(qp_attr_mask, QP_ATTR_DEST_QP_NUM);
       qp_attr.av.dlid          = SRV_nic->lid_arr[c];
       QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_AV); 
       if(DEBUG_SERVER){
         printf("\n%d(s):connecting to %d rqp = %d dlid=%d\n",armci_me,c,
                  conC->rqpnum[c],qp_attr.av.dlid);fflush(stdout);
       }
       rc = VAPI_modify_qp(CLN_nic->handle,(con->qp),&qp_attr, &qp_attr_mask,
                           &qp_cap);
       armci_check_status(DEBUG_SERVER, rc,"master connect request INIT->RTR");
    }
    if(TIME_INIT)printf("\n%d:init to rtr time for server_initial_conn is %f",armci_me,(inittime3=MPI_Wtime())-inittime2);

    armci_util_wait_int(&armci_vapi_client_ready,1,10);

    QP_ATTR_MASK_CLR_ALL(qp_attr_mask);
    qp_attr.qp_state   = VAPI_RTS;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_QP_STATE);
    qp_attr.sq_psn   = 0;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_SQ_PSN);
    qp_attr.timeout   = 18;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_TIMEOUT);
    qp_attr.retry_count   = 1;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_RETRY_COUNT);
    qp_attr.rnr_retry     = 3;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_RNR_RETRY);
    qp_attr.ous_dst_rd_atom  = 128;
    QP_ATTR_MASK_SET(qp_attr_mask,QP_ATTR_OUS_DST_RD_ATOM);
    for(c=0; c< armci_nproc; c++){
       armci_connect_t *con = CLN_con + c;
       rc = VAPI_modify_qp(CLN_nic->handle,(con->qp),&qp_attr,&qp_attr_mask,
                           &qp_cap);
       armci_check_status(DEBUG_SERVER, rc,"master connect request RTR->RTS");
    }
    if(TIME_INIT)printf("\n%d:rtr to rts time for server_initial_conn is %f",armci_me,(inittime4=MPI_Wtime())-inittime3);

    if(DEBUG_SERVER)
       printf("%d:server thread done with connections\n",armci_me);

    armci_server_alloc_bufs();/* create receive buffers for server thread */

 
    /* setup descriptors and post nonblocking receives */
    for(c = ib= 0; c < armci_nproc; c++) {
       vapibuf_t *vbuf = serv_buf_arr[c];
       armci_init_vapibuf_recv(&vbuf->dscr, &vbuf->sg_entry,vbuf->buf, 
                               VBUF_DLEN, &serv_memhandle);
       /* we use index of the buffer to identify the buffer, this index is
        * returned with a call to VAPI_poll_cq inside the VAPI_wc_desc_t */
       vbuf->dscr.id = c + armci_nproc;
       if(DEBUG_SERVER){
         printf("\n%d(s):posted rr with lkey=%d",armci_me,vbuf->sg_entry.lkey);
         fflush(stdout);
       }
       rc = VAPI_post_rr(CLN_nic->handle,(CLN_con+c)->qp,&(vbuf->dscr));
       armci_check_status(DEBUG_SERVER, rc,"server post recv vbuf");
       
    }

    rc = EVAPI_set_comp_eventh(CLN_nic->handle,CLN_nic->rcq,
                               EVAPI_POLL_CQ_UNBLOCK_HANDLER,NULL,
                               &(CLN_nic->rcq_eventh));
    armci_check_status(DEBUG_SERVER, rc,"EVAPI_set_comp_eventh"); 
    if(TIME_INIT)printf("\n%d:post time for server_initial_conn is %f",armci_me,MPI_Wtime()-inittime4);

    armci_vapi_server_ready=1;
    /* check if we can poll in the server thread */
    enval = getenv("ARMCI_SERVER_CAN_POLL");
    if(enval != NULL){
       if((enval[0] != 'N') && (enval[0]!='n')) server_can_poll=1;
    } 
    else{
      if(armci_clus_info[armci_clus_me].nslave < armci_getnumcpus()) 
        server_can_poll=1;
    }
    server_can_poll=0;

    /* establish connections with compute processes/clients */
    /*vapi_connect_server();*/

    if(DEBUG_SERVER){
       printf("%d: server connected to all clients\n",armci_me); fflush(stdout);
    }
    if(TIME_INIT)printf("\n%d:time for server_initial_conn is %f",armci_me,MPI_Wtime()-inittime0);
}

static void armci_finalize_nic(vapi_nic_t *nic)
{
VAPI_ret_t ret;
VAPI_cqe_num_t ns,nr;
VAPI_wc_desc_t com_desc_p;
int i;

    ret = VAPI_query_cq(nic->handle,nic->scq,&ns);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:query_scq");
    /*printf("\n%d:this many scq %d",armci_me,ns);*/
    ret = EVAPI_peek_cq(nic->handle,nic->scq,1);
    if(ret==VAPI_OK)printf("\n%d:finalize nic scq still has stuff in it",armci_me);

#if 0
    for(i=0;i<ns;i++){
      ret = VAPI_poll_cq( nic->handle,nic->scq,&com_desc_p);
      armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:poll_scq");
    }
    ret = VAPI_query_cq(nic->handle,nic->scq,&ns);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:query_scq");
    printf("\n%d:now this many scq %d",armci_me,ns);
#endif

    ret = VAPI_destroy_cq(nic->handle, nic->scq);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:destroy_scq");

    ret = VAPI_query_cq(nic->handle,nic->rcq,&nr);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:query_rcq");
    /*printf("\n%d:this many rcq %d",armci_me,nr);*/
    ret = EVAPI_peek_cq(nic->handle,nic->rcq,1);
    if(ret==VAPI_OK)printf("\n%d:finalize nic rcq still has stuff in it",armci_me);

#if 0
    for(i=0;i<nr;i++){
      ret = VAPI_poll_cq( nic->handle,nic->rcq,&com_desc_p);
      armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:poll_scq");
    }
    ret = VAPI_query_cq(nic->handle,nic->rcq,&nr);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:query_rcq");
    printf("\n%d:now this many rcq %d",armci_me,nr);
#endif

    ret = VAPI_destroy_cq(nic->handle, nic->rcq);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:destroy_rcq");
    ret = EVAPI_release_hca_hndl(nic->handle);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:release_hca");
}


void armci_server_transport_cleanup()
{
int s;
VAPI_ret_t rc;
    /*first we have empty send/recv queues TBD*/
    if(serv_malloc_buf_base){
       rc = VAPI_deregister_mr(CLN_nic->handle,serv_memhandle.memhndl);
       armci_vapi_check_return(DEBUG_FINALIZE,rc,
                       "armci_server_transport_cleanup:deregister_mr");
       /*now free it*/
       free(serv_malloc_buf_base);
    }
    /*now deregister all my regions from regionskk.c*/
    armci_server_region_destroy();
    if(CLN_con){
       for(s=0; s< armci_nproc; s++){
         armci_connect_t *con = CLN_con + s;
         if(con->qp){
           rc = VAPI_destroy_qp(CLN_nic->handle,con->qp);
           armci_vapi_check_return(DEBUG_FINALIZE,rc,
                           "armci_server_transport_cleanup:destroy_qp");
         }
         free(con->rqpnum);
       }
       free(CLN_con);
    }
    rc = EVAPI_clear_comp_eventh(CLN_nic->handle,(CLN_nic->rcq_eventh));
    armci_vapi_check_return(DEBUG_FINALIZE,rc,
                    "armci_server_transport_cleanup:clear_eventh");
    armci_finalize_nic(CLN_nic);

}

void armci_transport_cleanup()
{
int s;
VAPI_ret_t rc;
    /*first deregister buffers memory */
    if(client_malloc_buf_base){
       rc = VAPI_deregister_mr(SRV_nic->handle,client_memhandle.memhndl);
       armci_vapi_check_return(DEBUG_FINALIZE,rc,"armci_client_transport_cleanup:deregister_mr");
       /*now free it*/
       free(client_malloc_buf_base);
    }
    /*now deregister all my regions from regions.c*/
    armci_region_destroy();
    if(SRV_con){
       for(s=0; s< armci_nclus; s++){
         armci_connect_t *con = SRV_con + s;
         if(con->qp){
           rc = VAPI_destroy_qp(SRV_nic->handle,con->qp);
           armci_vapi_check_return(DEBUG_FINALIZE,rc,"armci_client_transport_cleanup:destroy_qp");
         }
         free(con->rqpnum);
       }
       free(SRV_con);
    }
    armci_finalize_nic(SRV_nic);
}


void armci_call_data_server()
{
VAPI_ret_t rc=VAPI_CQ_EMPTY;
vapibuf_t *vbuf,*vbufs;
request_header_t *msginfo,*msg;
int c,i,need_ack,pollcount;
static int mytag=1;
#ifdef ARMCI_ENABLE_GPC_CALLS
    unblock_thread_signal(GPC_COMPLETION_SIGNAL);
#endif
    for(;;){
       VAPI_wc_desc_t *pdscr=NULL;
       VAPI_wc_desc_t pdscr1;
       pdscr = &pdscr1;
       rc=VAPI_CQ_EMPTY;

#ifdef ARMCI_ENABLE_GPC_CALLS
       block_thread_signal(GPC_COMPLETION_SIGNAL);
#endif
       /*we just snoop to see if we have something */ 
	 rc = VAPI_poll_cq(CLN_nic->handle, CLN_nic->rcq, pdscr);
       if(server_can_poll){
         while(rc == VAPI_CQ_EMPTY){
           rc = VAPI_poll_cq(CLN_nic->handle, CLN_nic->rcq, pdscr);
           if(armci_server_terminating){
             /* server got interrupted when clients terminate connections */
             armci_server_transport_cleanup();
             sleep(1);
             _exit(0);
           }
         }
       }
       else {
         while(rc == VAPI_CQ_EMPTY){
           rc = EVAPI_poll_cq_block(CLN_nic->handle, CLN_nic->rcq, 0, pdscr);
           if(armci_server_terminating){
             /* server got interrupted when clients terminate connections */
             armci_server_transport_cleanup();
             sleep(1);
             _exit(0);
           }
           /*
           if(rc==VAPI_CQ_EMPTY){
             for(pollcount=0;pollcount<10,rc==VAPI_CQ_EMPTY;pollcount++)
	       rc = VAPI_poll_cq(CLN_nic->handle, CLN_nic->rcq, pdscr);
           }
           */
         }
       }
       
       armci_check_status(DEBUG_SERVER, rc,"server poll/block");
       /*we can figure out which buffer we got data info from the wc_desc_t id
        * this can tell us from which process we go the buffer, as well */

       if(DEBUG_SERVER){
          printf("%d(s) : NEW MESSAGE bytelen %d \n",armci_me,pdscr->byte_len);
          printf("%d(s) : NEW MESSAGE id is %ld \n",armci_me,pdscr->id);
          fflush(stdout);
       }    
               
       if(pdscr->id >= DSCRID_SCATGAT && pdscr->id < DSCRID_SCATGAT_END){
         sr_descr_t *rdscr_arr;
         if(DEBUG_SERVER){
	   printf("%d(s) : received DATA id = %ld, length = %d\n",
                    armci_me,pdscr->id, pdscr->byte_len);     
           fflush(stdout);
         }
         rdscr_arr = armci_vapi_serv_nbrdscr_array;
         rdscr_arr[pdscr->id-DSCRID_SCATGAT].numofrecvs--;
         if(rdscr_arr[pdscr->id-DSCRID_SCATGAT].numofrecvs==0)
           rdscr_arr[pdscr->id-DSCRID_SCATGAT].tag=0;
	 continue;
       }
       
       vbuf = serv_buf_arr[pdscr->id - armci_nproc];
       msginfo = (request_header_t*)vbuf->buf;
       armci_ack_proc = c = msginfo->from;

       if(DEBUG_SERVER){  	       
          printf("%d(s) : request id is %ld operation is %d, length is %d %d\n",
           armci_me,pdscr->id,msginfo->operation,pdscr->byte_len,msginfo->from);
          fflush(stdout);
       }
       
       if(msginfo->operation == PUT &&msginfo->pinned == 1){
          int found, num;
          int stride_arr[MAX_STRIDE_LEVEL]; /*should be MAX_STRIDE_LEVELS*/
          int count[MAX_STRIDE_LEVEL];
          void *dest_ptr;
          int stride_levels;
          ARMCI_MEMHDL_T *loc_memhandle;
          void armci_post_scatter(void *,int *,int *,int, armci_vapi_memhndl_t *,int,int,int,sr_descr_t **);

          /*unpack decsriptor_record : should call a function instead */
          msg = msginfo + 1;
          test_ptr = dest_ptr = *(void**)msg; 
	  msg = (request_header_t *) ((char*)msg + sizeof(void*));
          test_stride_levels=stride_levels = *(int*)msg; 
          msg = (request_header_t *) ((char*)msg + sizeof(int));
          for(i =0; i<stride_levels; i++){
              test_stride_arr[i] = stride_arr[i] = *(int*)msg;
              msg = (request_header_t*) ((int*)msg + 1);
          }
          for(i=0; i<stride_levels+1; i++){
              test_count[i] = count[i] = *(int*)msg;
              msg = (request_header_t*) ((int*)msg + 1);
          }
	
          if(DEBUG_SERVER){
            printf(" server:the dest_ptr is %p\n", dest_ptr);
            for(i =0; i<stride_levels; i++)
	      printf("stride_arr[i] is %d,value of count[i] is %d\n",
                               stride_arr[i], count[i]);
            printf("the value of stride_levels is %d\n", stride_levels);
            fflush(stdout);	    
          }

          found =get_armci_region_local_hndl(dest_ptr,armci_me, &loc_memhandle);

	  if(!found){
	     armci_die("SERVER : local region not found",pdscr->id);
	  }

          if(DEBUG_SERVER){
             printf("%d(s) : about to call armci_post_scatter\n",armci_me);
             fflush(stdout);
          }     

          armci_post_scatter(dest_ptr, stride_arr, count, stride_levels,
                        loc_memhandle,msginfo->from, mytag, SERV,NULL );

          mytag = (mytag+1)%(MAX_PENDING);
          if(mytag==0)mytag=1;
         
          if(DEBUG_SERVER){
             printf("%d(s) : finished posting %d scatter\n",armci_me,num);
             fflush(stdout);
          }     
       }
       
       vbufs = serv_buf_arr[pdscr->id - armci_nproc] = spare_serv_buf;
       armci_init_vapibuf_recv(&vbufs->dscr, &vbufs->sg_entry,vbufs->buf, 
                               VBUF_DLEN, &serv_memhandle);
       vbufs->dscr.id = c + armci_nproc;
      
       rc = VAPI_post_rr(CLN_nic->handle,(CLN_con+c)->qp,&(vbufs->dscr));
       armci_check_status(DEBUG_SERVER, rc,"server post recv vbuf");

       spare_serv_buf = vbuf; 

       if(DEBUG_SERVER){
          printf("%d(s):Came out of poll id=%ld\n",armci_me,pdscr->id);
          fflush(stdout);
       }
	   
       if(msginfo->operation == REGISTER){
          if(DEBUG_SERVER){   
	  printf("%d(s) : Register_op id is %d, comp_dscr_id is  %ld\n",
                   armci_me,msginfo->operation,pdscr->id);
          fflush(stdout);
          }
            
          armci_server_register_region(*((void **)(msginfo+1)),
                           *((long *)((char *)(msginfo+1)+sizeof(void *))),
                           (ARMCI_MEMHDL_T *)(msginfo->tag.data_ptr));
          *(long *)(msginfo->tag.ack_ptr) = ARMCI_VAPI_COMPLETE;
          continue;
       }

       if( msginfo->operation == PUT &&msginfo->pinned == 1);
       else{
         if(DEBUG_SERVER){
            printf("%d(s) : request is %ld about to call armci_data_server\n",
                    armci_me, pdscr->id);
            fflush(stdout);
         }      
         armci_data_server(vbuf);
       }  
       if((msginfo->operation == PUT) || ACC(msginfo->operation)){
           /* for operations that do not send data back we can send ACK now */
           SERVER_SEND_ACK(armci_ack_proc);
           need_ack=0;
           if(DEBUG_SERVER){
	      printf("%d(s) : posted ack\n\n",armci_me);
              fflush(stdout);
           }
       }else need_ack=1;

       if(0){
          printf("%d(s):Done processed request\n\n",armci_me);
          fflush(stdout);
       }

#ifdef ARMCI_ENABLE_GPC_CALLS
       unblock_thread_signal(GPC_COMPLETION_SIGNAL);
#endif
   }/* end of for */
}



void armci_vapi_complete_buf(armci_vapi_field_t *field,int snd,int rcv,int to,
                             int op)
{
VAPI_sr_desc_t *snd_dscr;

BUF_INFO_T *info;
    info = (BUF_INFO_T *)((char *)field-sizeof(BUF_INFO_T));

    if(info->tag && op==GET)return;

    if(snd){
       request_header_t *msginfo = (request_header_t *)(field+1);
       snd_dscr=&(field->sdscr);
       if(mark_buf_send_complete[snd_dscr->id]==0)
         armci_send_complete(snd_dscr,"armci_vapi_complete_buf",1);
    }
   
    if(rcv){
       int *last;
       long *flag;
       int loop = 0;
       request_header_t *msginfo = (request_header_t *)(field+1);
       flag = (long *)&msginfo->tag.ack;

       if(op==PUT || ACC(op)){
         if(msginfo->bypass && msginfo->pinned && msginfo->format == STRIDED &&
                                       op == PUT);
         else{
           while(armci_util_long_getval(flag) != ARMCI_VAPI_COMPLETE) {
             loop++;
             loop %=100000;
             if(loop==0){
             }
           }
         }
         *flag = 0L;
       }
       else{
         last = (int *)((char *)msginfo+msginfo->datalen-sizeof(int));
         while(armci_util_int_getval(last) == ARMCI_VAPI_COMPLETE &&
               armci_util_long_getval(flag)  != ARMCI_VAPI_COMPLETE){
           loop++;
           loop %=100000;
           if(loop==0){
             if(DEBUG_CLN){
               printf("%d: client last(%p)=%d flag(%p)=%ld off=%d\n",
                      armci_me,last,*last,flag,*flag,msginfo->datalen);
               fflush(stdout);
             }
           }
         }
       }
    }
}

static inline void armci_vapi_post_send(int isclient,int con_offset,
                                      VAPI_sr_desc_t *snd_dscr,char *from)
{
VAPI_ret_t rc=VAPI_CQ_EMPTY;
vapi_nic_t *nic;
armci_connect_t *con;
int total = 0;

    if(!isclient){
       nic = CLN_nic;
       con = CLN_con+con_offset;
    }
    else{
       nic = SRV_nic;
       con = SRV_con+con_offset;
    }

    if(DEBUG_CLN){
       printf("vapi_post_send: snd_dscr->sg_lst_p->len is %d, seg_sizeis %d\n"
                    ,snd_dscr->sg_lst_len,snd_dscr->sg_lst_p->len);
       fflush(stdout);
    }   


    /* find the total length of all the segments */
    total = snd_dscr->sg_lst_p->len * snd_dscr->sg_lst_len;         
    if(DEBUG_CLN){
       printf("%d(c) : total is %d\t, max_size is %d\n",armci_me,total,
                    armci_vapi_max_inline_size);
    }
    
    if(total > armci_vapi_max_inline_size){
       rc = VAPI_post_sr(nic->handle,con->qp,snd_dscr);
    }   
    else{
       rc = EVAPI_post_inline_sr(nic->handle,con->qp,snd_dscr);
    } 
    armci_check_status(DEBUG_INIT, rc, from);
}

int armci_send_req_msg(int proc, void *buf, int bytes)
{
int cluster = armci_clus_id(proc);
request_header_t *msginfo = (request_header_t *)buf;
VAPI_sr_desc_t *snd_dscr;
VAPI_sg_lst_entry_t *ssg_lst; 
//double t0,t1;

    snd_dscr = BUF_TO_SDESCR((char *)buf);
    ssg_lst  = BUF_TO_SSGLST((char *)buf);

    _armci_buf_ensure_one_outstanding_op_per_node(buf,cluster);  
    msginfo->tag.ack = 0;

    if(msginfo->operation == PUT || ACC(msginfo->operation))
       msginfo->tag.data_ptr = (void *)&msginfo->tag.ack;
    else {
       if(msginfo->operation == GET && !msginfo->bypass && msginfo->dscrlen 
                       >= (msginfo->datalen-sizeof(int)))
         msginfo->tag.data_ptr = (char *)(msginfo+1)+msginfo->dscrlen;
       else
         msginfo->tag.data_ptr = GET_DATA_PTR(buf);
    }
    msginfo->tag.ack_ptr = &(msginfo->tag.ack);
    
    if(DEBUG_CLN){
       printf("%d:the ack_ptr is initialised to %p, ack->value is %ld\n",
                 armci_me,msginfo->tag.ack_ptr,msginfo->tag.ack);fflush(stdout);
    }
    
    armci_init_vapibuf_send(snd_dscr, ssg_lst,buf, 
                            bytes, &client_memhandle);


    //t0 = MPI_Wtime();
    armci_vapi_post_send(1,cluster,snd_dscr,"send_req_msg:post_send");
    //t1 = MPI_Wtime();
    //printf("%d:posting took %lf\n",armci_me,1e6*(t1-t0));fflush(stdout);

    if(DEBUG_CLN){
       printf("%d:client sent REQ=%d %d bytes serv=%d qp=%ld id =%ld lkey=%d\n",
               armci_me,msginfo->operation,bytes,cluster,
               (SRV_con+cluster)->qp,snd_dscr->id,ssg_lst->lkey);
       fflush(stdout);
    }
    return(0);
}


/*\
 *  client waits for first phase ack before posting gather desr
\*/
void armci_wait_ack(char *buffer)
{
   long *flag;
   request_header_t *msginfo = (request_header_t *)(buffer);
   flag = (long*)&msginfo->tag.ack;

   while(armci_util_long_getval(flag) != ARMCI_VAPI_COMPLETE);
   flag = 0; 
}




void armci_client_direct_send(int p,void *src_buf, void *dst_buf, int len,void** contextptr,int nbtag,ARMCI_MEMHDL_T *lochdl,ARMCI_MEMHDL_T *remhdl)
{
sr_descr_t *dirdscr;
int clus = armci_clus_id(p);

    /*ID for the desr that comes from get_next_descr is already set*/
    dirdscr = armci_vapi_get_next_sdescr(nbtag,0);
    if(nbtag)*contextptr = dirdscr;

    armci_init_vbuf_srdma(&dirdscr->sdescr,dirdscr->sg_entry,src_buf,dst_buf,
                          len,lochdl,remhdl);

    armci_vapi_post_send(1,clus,&(dirdscr->sdescr),
                         "client_direct_send:post_send");

    if(nbtag==0)
       armci_send_complete(&(dirdscr->sdescr),"armci_client_direct_send",1);
}

/*\ RDMA get 
\*/ 
void armci_client_direct_get(int p, void *src_buf, void *dst_buf, int len,
                             void** cptr,int nbtag,ARMCI_MEMHDL_T *lochdl,
                             ARMCI_MEMHDL_T *remhdl)
{      
VAPI_ret_t rc=VAPI_CQ_EMPTY;
sr_descr_t *dirdscr;
int clus = armci_clus_id(p);
//double t0,t1;
    /*ID for the desr that comes from get_next_descr is already set*/
    dirdscr = armci_vapi_get_next_sdescr(nbtag,0);
    if(nbtag)*cptr = dirdscr;

    if(DEBUG_CLN){
      printf("\n%d: in direct get lkey=%d rkey=%d\n",armci_me,lochdl->lkey,
               remhdl->rkey);fflush(stdout);
    }

    armci_init_vbuf_rrdma(&dirdscr->sdescr,dirdscr->sg_entry,dst_buf,src_buf,
                          len,lochdl,remhdl);
    //t0 = MPI_Wtime();
    rc = VAPI_post_sr(SRV_nic->handle,(SRV_con+clus)->qp,&(dirdscr->sdescr));
    armci_check_status(DEBUG_CLN, rc,"armci_client_get_direct");
    //t1 = MPI_Wtime();
    //printf("%d:posting took %lf\n",armci_me,1e6*(t1-t0));fflush(stdout);
    if(!nbtag){
       armci_send_complete(&(dirdscr->sdescr),"armci_client_direct_get",1);
    }
}



char *armci_ReadFromDirect(int proc, request_header_t *msginfo, int len)
{
int cluster = armci_clus_id(proc);
vapibuf_ext_t* evbuf=BUF_TO_EVBUF(msginfo);
char *dataptr = GET_DATA_PTR(evbuf->buf);
extern void armci_util_wait_int(volatile int *,int,int);

    if(DEBUG_CLN){ printf("%d(c):read direct %d qp=%p\n",armci_me,
                len,&(SRV_con+cluster)->qp); fflush(stdout);
    }
   
    if(mark_buf_send_complete[evbuf->snd_dscr.id]==0)
       armci_send_complete(&(evbuf->snd_dscr),"armci_ReadFromDirect",1); 

    if(!msginfo->bypass){
       long *flag;
       int *last;
       int loop = 0;
       flag = &(msginfo->tag.ack);
       if(msginfo->operation==GET){
         last = (int *)(dataptr+len-sizeof(int));
         if(msginfo->dscrlen >= (len-sizeof(int))){
           last = (int *)(dataptr+len+msginfo->dscrlen-sizeof(int));
           dataptr+=msginfo->dscrlen;
         }

         if(DEBUG_CLN){
           printf("\n%d:flagval=%d at ptr=%p ack=%ld dist=%d\n",armci_me,*last,
                   last,*flag,len);fflush(stdout);
         }

         while(armci_util_int_getval(last) == ARMCI_VAPI_COMPLETE &&
               armci_util_long_getval(flag)  != ARMCI_VAPI_COMPLETE){
           loop++;
           loop %=100000;
           if(loop==0){
             if(DEBUG_CLN){
               printf("%d: client last(%p)=%d flag(%p)=%ld off=%d\n",
                      armci_me,last,*last,flag,*flag,msginfo->datalen);
               fflush(stdout);
             }
           }
         }
         *flag = 0L;
       }
       else if(msginfo->operation == REGISTER){
         while(armci_util_long_getval(flag)  != ARMCI_VAPI_COMPLETE){
           loop++;
           loop %=100000;
           if(loop==0){
             if(DEBUG_CLN){
               printf("%d: client last(%p)=%d flag(%p)=%ld off=%d\n",
                      armci_me,last,*last,flag,*flag,msginfo->datalen);
               fflush(stdout);
             }
           }
         }
       }
       else{
         int *flg = (int *)(dataptr+len);
         while(armci_util_int_getval(flg) != ARMCI_VAPI_COMPLETE){
           loop++;
           loop %=100000;
           if(loop==0){
             if(DEBUG_CLN){
               printf("%d: client waiting (%p)=%d off=%d\n",
                      armci_me,flg,*flg,len);
               fflush(stdout);
             }
           }
         }
       }
    }
    return dataptr;
}

void armci_send_data_to_client(int proc, void *buf, int bytes,void *dbuf)
{
VAPI_ret_t rc=VAPI_CQ_EMPTY;
VAPI_sr_desc_t *sdscr;
VAPI_wc_desc_t *pdscr=NULL; 
VAPI_wc_desc_t pdscr1;
  
    pdscr = &pdscr1;
    sdscr=&serv_buf->snd_dscr;

    if(DEBUG_SERVER){
       printf("\n%d(s):sending data to client %d at %p flag = %p bytes=%d\n",
               armci_me,
               proc,dbuf,(char *)dbuf+bytes-sizeof(int),bytes);fflush(stdout);
    }

    memset(sdscr,0,sizeof(VAPI_sr_desc_t));
    armci_init_vbuf_srdma(sdscr,&serv_buf->ssg_entry,buf,dbuf,bytes,
                          &serv_memhandle,(handle_array+proc));

    if(DEBUG_SERVER){
       printf("\n%d(s):handle_array[%d]=%p dbuf=%p flag=%p bytes=%d\n",armci_me,
              proc,&handle_array[proc],(char *)dbuf,
              (char *)dbuf+bytes-sizeof(int),bytes);
       fflush(stdout);
    }

    serv_buf->snd_dscr.id = proc+armci_nproc;
    rc = VAPI_post_sr(CLN_nic->handle,(CLN_con+proc)->qp,&serv_buf->snd_dscr);
    armci_check_status(DEBUG_SERVER, rc,"server post send to client");

#if 0
repeat_poll: 
    rc=VAPI_CQ_EMPTY;
    while(rc == VAPI_CQ_EMPTY)
       rc = VAPI_poll_cq(CLN_nic->handle,CLN_nic->scq,pdscr);
    armci_check_status(DEBUG_SERVER, rc,"server wait post send to client");
    if(pdscr->id != proc+armci_nproc)goto repeat_poll;
#endif

    armci_send_complete(&serv_buf->snd_dscr,"armci_send_data_to_client",1);
}

void armci_WriteToDirect(int proc, request_header_t* msginfo, void *buf)
{
int bytes;
int *last;
    bytes = (int)msginfo->datalen;
    if(DEBUG_SERVER){
      printf("%d(s):write to direct sent %d to %d at %p\n",armci_me,
             bytes,proc,(char *)msginfo->tag.data_ptr);
      fflush(stdout);
    }
    if(msginfo->operation!=GET){
       *(int *)((char *)buf+bytes)=ARMCI_VAPI_COMPLETE;
       bytes+=sizeof(int);
    }
    armci_send_data_to_client(proc,buf,bytes,msginfo->tag.data_ptr);
    /*if(msginfo->dscrlen >= (bytes-sizeof(int)))
       last = (int*)(((char*)(buf)) + (msginfo->dscrlen+bytes - sizeof(int)));
    else*/
       last = (int*)(((char*)(buf)) + (bytes - sizeof(int)));

    if(msginfo->operation==GET && *last == ARMCI_VAPI_COMPLETE){
       SERVER_SEND_ACK(msginfo->from);
    }
    armci_ack_proc=NONE;
}


void armci_rcv_req(void *mesg,void *phdr,void *pdescr,void *pdata,int *buflen)
{
vapibuf_t *vbuf = (vapibuf_t*)mesg;
request_header_t *msginfo = (request_header_t *)vbuf->buf;
*(void **)phdr = msginfo;

    if(DEBUG_SERVER){
        printf("%d(server): got %d req (dscrlen=%d datalen=%d) from %d\n",
               armci_me, msginfo->operation, msginfo->dscrlen,
               msginfo->datalen, msginfo->from); fflush(stdout);
    }
 
    /* we leave room for msginfo on the client side */
    *buflen = MSG_BUFLEN - sizeof(request_header_t);
  
    if(msginfo->bytes) {
        *(void **)pdescr = msginfo+1;
        if(msginfo->operation == GET)
            *(void **)pdata = MessageRcvBuffer;
        else
            *(void **)pdata = msginfo->dscrlen + (char*)(msginfo+1);
    }else {
          *(void**)pdescr = NULL;
          *(void**)pdata = MessageRcvBuffer;
    }  
}


/**********************SCATTER GATHER STUFF***********************************/
static void posts_scatter_desc(sr_descr_t *pend_dscr,int proc,int type)
{
VAPI_ret_t rc;
int cluster = armci_clus_id(proc);
VAPI_rr_desc_t *scat_dscr;
   
    scat_dscr = &pend_dscr->rdescr;
    
    /*armci_vapi_print_dscr_info(NULL,scat_dscr);*/
    if((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN)){
       printf("%d(%d) : inside posts scatter dscr, id is %d\n",
	      armci_me,type,scat_dscr->id);
       fflush(stdout);
    }     

    
    if(type == SERV)   
       rc = VAPI_post_rr(CLN_nic->handle,(CLN_con + proc)->qp,scat_dscr);
    else
       rc = VAPI_post_rr(SRV_nic->handle,(SRV_con+cluster)->qp,scat_dscr);
    
    armci_check_status(DEBUG_SERVER,rc,"posts 1 of several rcv");
   
    if((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN) ){
       printf("\n%d: list_length is %d, id is %ld\n",
	      armci_me,scat_dscr->sg_lst_len,scat_dscr->id);
       fflush(stdout);
    }  
}


/*\
 *  client calls from request.c
 *  server calls from ds-shared.c
\*/ 
static sr_descr_t serv_blocking_scatter_dscr;
static sr_descr_t client_blocking_scatter_dscr;
void armci_post_scatter(void *dest_ptr, int dest_stride_arr[], int count[],
     int stride_levels, armci_vapi_memhndl_t *mhandle,
     int proc, int nbtag, int type, sr_descr_t **srd)
{
int i;
int total_size = 0;
int total_of_2D = 1;
int index[MAX_STRIDE_LEVEL], unit[MAX_STRIDE_LEVEL];
int j,k,y,z;
int num_dscr = 0; 
int num_xmit = 0, num_seg, max_seg, rem_seg,vecind;
char* src, *src1;
sr_descr_t *pend_dscr;
VAPI_sg_lst_entry_t *scat_sglist;
VAPI_rr_desc_t *scat_dscr;

    if((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN) ){
       printf("%d(%d)  : inside post_scatter %d\n",armci_me,type,nbtag);     
       fflush(stdout);
    }

    max_seg =  armci_max_num_sg_ent;
    
    if(nbtag){
       pend_dscr = armci_vapi_get_next_rdescr(nbtag,1);
       if(srd!=NULL)*srd=pend_dscr;
    }
    else{
       pend_dscr = &client_blocking_scatter_dscr;
       pend_dscr->rdescr.id=DSCRID_SCATGAT + MAX_PENDING;
    }

    /*pend_dscr->proc = proc;*/
    pend_dscr->numofrecvs=0; 
    
    scat_dscr = &pend_dscr->rdescr;
    scat_sglist = pend_dscr->sg_entry;
    scat_dscr->opcode = VAPI_RECEIVE;
    scat_dscr->comp_type = VAPI_SIGNALED;
    scat_dscr->sg_lst_p = scat_sglist;
    scat_dscr->sg_lst_len = 0;

    index[2] = 0; unit[2] = 1;
    if(stride_levels > 1){
       total_of_2D = count[2];
       for(j=3; j<=stride_levels; j++){
         index[j] = 0; unit[j] = unit[j-1]*count[j-1];
         total_of_2D*=count[j];
       }
    }

    num_xmit = total_of_2D*count[1]/max_seg;
    rem_seg = (total_of_2D*count[1])%max_seg;
    if(num_xmit == 0) num_xmit = 1;
    else if(rem_seg!= 0)num_xmit++;


    if((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN) ){ 
       printf("%d(%d):armci_post_scatter num_xmit = %d\t, rem_seg = %d\n", 
               armci_me,type,num_xmit,rem_seg);
       fflush(stdout);
    }
    
    k=0; vecind = 0;
    if(rem_seg!=0 && k==(num_xmit-1))num_seg = rem_seg;
    else num_seg = max_seg;

    y=0,z=0;
    for(i=0;i<total_of_2D;i++){
       src = (char *)dest_ptr;
       for(j=2;j<=stride_levels;j++){
         src+= index[j]*dest_stride_arr[j-1];
         if(((i+1)%unit[j]) == 0) index[j]++;
         if(index[j] >= count[j]) index[j] =0;
       }
       src1 = src;

       for(j=0; j<count[1]; j++, vecind++){
         if(vecind == num_seg){
           posts_scatter_desc(pend_dscr,proc,type);
           pend_dscr->numofrecvs++; 

           /* the previous one has been posted, start off new*/ 
           scat_dscr->sg_lst_len=0;
           y = 0; /* reuse the same scatter descriptor */
           vecind=0;total_size=0;k++;
           if(rem_seg!=0 && k==(num_xmit-1))num_seg = rem_seg;
         }
         /* fill the scatter descriptor */     
         scat_sglist[y].addr = (VAPI_virt_addr_t)(MT_virt_addr_t)src1;
         scat_sglist[y].lkey = mhandle->lkey;
         scat_sglist[y].len = count[0];
         scat_dscr->sg_lst_len++;
         src1 += dest_stride_arr[0];
         y++;

       }

       if(vecind == num_seg){
         posts_scatter_desc(pend_dscr,proc,type);
         pend_dscr->numofrecvs++; 

         /* the previous one has been posted, start off new*/ 
         scat_dscr->sg_lst_len=0;
         y =0 ;
         vecind = 0; total_size=0; k++;
         if(rem_seg!=0 && k==(num_xmit-1))num_seg=rem_seg;
         else num_seg = max_seg;
       }

    }
    if(!nbtag){
       /*if blocking call wait_for_blocking_scatter to complete*/
    }
    return;
}

void armci_wait_for_blocking_scatter()
{
sr_descr_t *pend_dscr=&client_blocking_scatter_dscr;
int i;
    armci_recv_complete(&pend_dscr->rdescr,"armci_post_scatter",pend_dscr->numofrecvs);
}


/*\
 *  function used by armci_post_gather to actually post the sctter list 
\*/ 
static void posts_gather_desc(sr_descr_t *pend_dscr,int proc,int type)
{
VAPI_ret_t rc;
int cluster = armci_clus_id(proc);
VAPI_sr_desc_t *gat_dscr;

    gat_dscr = &pend_dscr->sdescr;
    /*armci_vapi_print_dscr_info(gat_dscr,NULL);*/
    if((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN)){
       printf("%d: type(client=1)=%d inside posts gather dscr, id is %d\n",
	      armci_me,type,gat_dscr->id);
       fflush(stdout);
    }     
           
    rc=VAPI_CQ_EMPTY;

    if(type == CLN){
       rc = VAPI_post_sr(SRV_nic->handle,(SRV_con+cluster)->qp,gat_dscr); 
       armci_check_status(DEBUG_CLN,rc,"client posts a gather sends");
    }
    else{
       rc = VAPI_post_sr(CLN_nic->handle, (CLN_con + proc)->qp, gat_dscr);
       armci_check_status(DEBUG_SERVER,rc,"client posts a gather sends");
    }

}

/*\
 *  posts a bunch of gather descriptors
\*/ 
static sr_descr_t serv_blocking_gather_dscr;
static sr_descr_t client_blocking_gather_dscr;
void armci_post_gather(void *src_ptr, int src_stride_arr[], int count[],
      int stride_levels, armci_vapi_memhndl_t *mhandle,
      int proc,int nbtag, int type, sr_descr_t **srd)
{
int i;
int total_of_2D = 1;
int total_size = 0;
int index[MAX_STRIDE_LEVEL], unit[MAX_STRIDE_LEVEL];
int j,k,y,z;
int num_posted = 0;
char *src, *src1;
int num_xmit = 0, num_seg, max_seg, rem_seg,vecind;
sr_descr_t *pend_dscr;
VAPI_sg_lst_entry_t *gat_sglist;
VAPI_sr_desc_t *gat_dscr;

    if((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN)){
      printf("%d(%d)  : inside post_gather\n",armci_me,type);     
      fflush(stdout);
    }

    max_seg =  armci_max_num_sg_ent;
    if(nbtag){
       pend_dscr = armci_vapi_get_next_sdescr(nbtag,1);
       if(srd!=NULL)*srd=pend_dscr;
    }
    else{
       pend_dscr = &client_blocking_gather_dscr;
       pend_dscr->sdescr.id=DSCRID_SCATGAT + MAX_PENDING;
    }
    pend_dscr->numofsends=0;
    
    gat_dscr = &pend_dscr->sdescr;
    gat_sglist = pend_dscr->sg_entry;
    gat_dscr->opcode = VAPI_SEND;
    gat_dscr->comp_type = VAPI_SIGNALED;
    gat_dscr->sg_lst_p = gat_sglist;   
    gat_dscr->sg_lst_len = 0;
    gat_dscr->set_se = FALSE;
    gat_dscr->fence = FALSE;

    index[2] = 0; unit[2] = 1;
    if(stride_levels > 1){
      total_of_2D = count[2];
      for(j=3; j<=stride_levels; j++){
        index[j] = 0; unit[j] = unit[j-1]*count[j-1];
        total_of_2D*=count[j];
      }
    }
    
    num_xmit = total_of_2D*count[1]/max_seg;
    rem_seg = (total_of_2D*count[1])%max_seg;
    if(num_xmit == 0) num_xmit = 1;
    else if(rem_seg!= 0)num_xmit++;
    
    if((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN) ){ 
       printf("%d(%d):armci_post_gather total_2D=%d, num_xmit=%d, rem_seg =%d, count[1] = %d\n",armci_me,type,total_of_2D, num_xmit,rem_seg,count[1]);
      fflush(stdout);
    }
    
    k=0; vecind = 0;
    if(rem_seg!=0 && k==(num_xmit-1))num_seg = rem_seg;
    else num_seg = max_seg;

    y=0,z=0;
    for(i=0;i<total_of_2D;i++){
       src = (char *)src_ptr;
       for(j=2;j<=stride_levels;j++){
         src+= index[j]*src_stride_arr[j-1];
         if(((i+1)%unit[j]) == 0) index[j]++;
         if(index[j] >= count[j]) index[j] =0;
       }
       src1 = src;

       for(j=0; j<count[1]; j++, vecind++){
         if(vecind == num_seg){
           posts_gather_desc(pend_dscr,proc,type);
           pend_dscr->numofsends++;

           /* the previous one has been posted, start off new*/ 
           gat_dscr->sg_lst_len = 0;
           y = 0; 
           vecind=0;total_size=0;k++;
           if(rem_seg!=0 && k==(num_xmit-1))num_seg = rem_seg;
         }
            
         /* fill the gather descriptor */ 
         gat_sglist[y].addr = (VAPI_virt_addr_t)(MT_virt_addr_t)src1;
         gat_sglist[y].lkey = mhandle->lkey;
         gat_sglist[y].len = count[0];
         gat_dscr->sg_lst_len++;
         src1 += src_stride_arr[0];
         y++;
                       
       }

       if(vecind == num_seg){
         posts_gather_desc(pend_dscr,proc,type);
         pend_dscr->numofsends++;
         if((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN) ){
           printf("%d(%d)posts_gather_desc done\n",armci_me,type);
           fflush(stdout);
         }

         /* the previous one has been posted, start off new*/ 
         gat_dscr->sg_lst_len = 0;
         y = 0;        
         vecind = 0; total_size=0; k++;
         if(rem_seg!=0 && k==(num_xmit-1))num_seg=rem_seg;
         else num_seg = max_seg;
       }
    }
    if(!nbtag){
       /*complete here*/
       armci_send_complete(&pend_dscr->sdescr,"armci_post_gather",pend_dscr->numofsends);
    }
    return;
}
/***********************END SCATTER GATHER STUFF******************************/



/***********************SPECIAL SEND/RECV*************************************/
void armci_server_direct_send(int dst, char *src_buf, char *dst_buf, int len,
                              VAPI_lkey_t *lkey,VAPI_rkey_t *rkey)
{
VAPI_ret_t rc=VAPI_CQ_EMPTY;
VAPI_sr_desc_t *sdscr;
VAPI_wc_desc_t *pdscr=NULL;
VAPI_wc_desc_t pdscr1;

    pdscr = &pdscr1;
    sdscr=&serv_buf->snd_dscr;

    if(DEBUG_SERVER){
       printf("\n%d(s):sending dir data to client %d at %p bytes=%d last=%p\n",
                armci_me,dst,dst_buf,len,(dst_buf+len-4));fflush(stdout);
               
    }

    memset(sdscr,0,sizeof(VAPI_sr_desc_t));
    armci_init_vbuf_srdma(sdscr,&serv_buf->ssg_entry,src_buf,dst_buf,len,NULL,NULL);
    sdscr->r_key = *rkey;
    serv_buf->ssg_entry.lkey = *lkey;
                          
    serv_buf->snd_dscr.id = dst+armci_nproc;
    rc = VAPI_post_sr(CLN_nic->handle,(CLN_con+dst)->qp,&serv_buf->snd_dscr);
    armci_check_status(DEBUG_SERVER, rc,"server post sent dir data to client");

    while(rc == VAPI_CQ_EMPTY)
       rc = VAPI_poll_cq(CLN_nic->handle,CLN_nic->scq,pdscr);
    armci_check_status(DEBUG_SERVER, rc,"server poll sent dir data to client");

}



void armci_send_contig_bypass(int proc, request_header_t *msginfo,
                              void *src_ptr, void *rem_ptr, int bytes)
{
int *last;
VAPI_lkey_t *lkey=NULL;
VAPI_rkey_t *rkey;    
int dscrlen = msginfo->dscrlen;

    last = (int*)(((char*)(src_ptr)) + (bytes - sizeof(int)));
    if(!msginfo->pinned)armci_die("armci_send_contig_bypass: not pinned",proc);

    rkey = (VAPI_rkey_t *)((char *)(msginfo+1)+dscrlen-(sizeof(VAPI_rkey_t)+sizeof(VAPI_lkey_t)));

    if(DEBUG_SERVER){
       printf("%d(server): sending data bypass to %d (%p,%p) %d %d\n", armci_me,
               msginfo->from,src_ptr, rem_ptr,*lkey,*rkey);
       fflush(stdout);
    }
    armci_server_direct_send(msginfo->from,src_ptr,rem_ptr,bytes,lkey,rkey);

    if(*last == ARMCI_VAPI_COMPLETE){
       SERVER_SEND_ACK(msginfo->from);
    }
}

void armci_rcv_strided_data_bypass_both(int proc, request_header_t *msginfo,
                                       void *ptr, int *count, int stride_levels)
{
int datalen = msginfo->datalen;
int *last;
long *ack;
int loop=0;

    if(DEBUG_CLN){ printf("%d:rcv_strided_data_both bypass from %d\n",
                armci_me,  proc); fflush(stdout);
    }
    if(!stride_levels){
      last = (int*)(((char*)(ptr)) + (count[0] -sizeof(int)));
      ack  = (long *)&msginfo->tag;
      while(armci_util_int_getval(last) == ARMCI_VAPI_COMPLETE &&
            armci_util_long_getval(ack)  != ARMCI_VAPI_COMPLETE){
        loop++;
        loop %=1000000;
        if(loop==0){
          if(DEBUG_CLN){
            printf("%d: client last(%p)=%d ack(%p)=%ld off=%d\n",
                  armci_me,last,*last,ack,*ack,(int)((char*)last - (char*)ptr));
            fflush(stdout);
          }
        }
      }
    }
    else {
      printf("\n%d:rcv_strided_data called, it should never be called\n",armci_me);
      armci_dscrlist_recv_complete(0,"armci_rcv_strided_data_bypass_both",NULL); 
    }

    if(DEBUG_CLN){printf("%d:rcv_strided_data bypass both: %d bytes from %d\n",
                          armci_me, datalen, proc); fflush(stdout);
    }
}

/*************************END OF FILE UNUSED CODE BELOW********************/

int armci_pin_memory(void *ptr, int stride_arr[], int count[], int strides)
{
    printf("\n%d:armci_pin_memory not implemented",armci_me);fflush(stdout);
    return 0;
}    


void armci_client_send_ack(int proc, int n)
{
    printf("\n%d:client_send_ack not implemented",armci_me);fflush(stdout);
}


void armci_rcv_strided_data_bypass(int proc, request_header_t* msginfo,
                                   void *ptr, int stride_levels)
{
    printf("\n%d:armci_rcv_strided_data_bypass not implemented",armci_me);
    fflush(stdout);
}


void armci_unpin_memory(void *ptr, int stride_arr[], int count[], int strides)
{
    printf("\n%d:armci_unpin_memory not implemented",armci_me);fflush(stdout);
}


int armcill_server_wait_ack(int proc, int n)
{ 
    printf("\n%d:armcill_server_wait_ack not implemented",armci_me);
    fflush(stdout);
    return(0);
}


void armcill_server_put(int proc, void* s, void *d, int len)
{
    printf("\n%d:armcill_server_put not implemented",armci_me);fflush(stdout);
}


/*\
 *  initialising the atomic send descriptor
\*/
void armci_init_vapibuf_atomic(VAPI_sr_desc_t *sd, VAPI_sg_lst_entry_t * sg,
                   int op, int*ploc,int *prem, int extra,
                   int id,ARMCI_MEMHDL_T *lhandle,
                   ARMCI_MEMHDL_T *rhandle)
{
    
    if(1){
       printf("%d(c) : entered armci_init_vapibuf_atomic\n",armci_me);   
       fflush(stdout);
    }   
    memset(sd,0,sizeof(VAPI_sr_desc_t));
    if(op == ARMCI_FETCH_AND_ADD_LONG ){
       printf("%d(c) :setting opcode for snd dscr to FETCH_AND_ADD\n",armci_me);     
       sd->opcode = VAPI_ATOMIC_FETCH_AND_ADD;
       sd->compare_add = (long) extra;
    }   
    else if(op == ARMCI_SWAP_LONG){
       sd->opcode = VAPI_ATOMIC_CMP_AND_SWP; 
       sd->swap = extra ;
    }
    sd->comp_type = VAPI_SIGNALED;
    sg->len = 8; /* 64 bit atomic*/
    printf("--------\n");
    sg->addr= (VAPI_virt_addr_t)(MT_virt_addr_t)(void *)ploc;
    if(lhandle)
    sg->lkey = lhandle->lkey;
    sd->sg_lst_p = sg;
    sd->sg_lst_len = 1;
    sd->remote_addr = (VAPI_virt_addr_t)(MT_virt_addr_t)(void *)prem;
    if(rhandle)
       sd->r_key = rhandle->rkey; /* how do we get the remote key  */
    sd->id = DSCRID_RMW + armci_me;
    sd->fence = FALSE;
    sd->set_se = FALSE; 

    if(1){
       printf("%d(c) : finished initialising atomic send desc id is %ld,armci_ime = %d\n",armci_me,sd->id,armci_me);
       fflush(stdout);
    }   
}
/*\
 *   using vapi remote atomic operations
\*/
void client_rmw_complete(VAPI_sr_desc_t *snd_dscr, char *from)
{
  VAPI_ret_t rc = VAPI_CQ_EMPTY;
  VAPI_wc_desc_t pdscr1;
  VAPI_wc_desc_t *pdscr=&pdscr1;

  printf("%d(c) : inside client_rmw_complete\n",armci_me);
  do{
      while(rc == VAPI_CQ_EMPTY){
            rc = VAPI_poll_cq(SRV_nic->handle, CLN_nic->scq, pdscr);
      }         
      armci_check_status(DEBUG_CLN,rc,"rmw complete");
      rc = VAPI_CQ_EMPTY;
    }while(pdscr->id != snd_dscr->id);
}        


void armci_direct_rmw(int op, int*ploc, int *prem, int extra, int proc,
                      ARMCI_MEMHDL_T *lhandle, ARMCI_MEMHDL_T *rhandle)
{

  VAPI_ret_t rc = VAPI_CQ_EMPTY;
  VAPI_sr_desc_t *sd;
  VAPI_sg_lst_entry_t *sg;
  vapi_nic_t *nic;
  armci_connect_t *con;
  /*int clus = armci_clus_id(proc);*/

  nic = SRV_nic;
  con = CLN_con+proc;
  /*con = SRV_con+proc; */

  sd = &(rmw[armci_me].rmw_dscr);
  sg = &(rmw[armci_me].rmw_entry);
  
  if(1){
     printf("%d(c) : about to call armci_init_vapibuf_atomic\n",armci_me);
     fflush(stdout);
  }
  
  armci_init_vapibuf_atomic(sd, sg, op,ploc,prem,extra,proc,lhandle,rhandle);
 
  if(1){
     printf("%d(c) : finished armci_init_vapibuf_atomic\n",armci_me);
     fflush(stdout);
  }        
  
  rc = VAPI_post_sr(nic->handle,con->qp, sd);  
  /*rc = EVAPI_post_inline_sr(nic->handle,con->qp,sd); *//*since bytes = 8*/
  armci_check_status(DEBUG_CLN,rc,"client direct atomic");

  if(1){
     printf("%d(c) : finished posting desc\n",armci_me);
     fflush(stdout);
  }
  
  /*armci_send_complete(sd,"send_remote_atomic");*/
  client_rmw_complete(sd,"send_remote_atomic");
  
  return; 

}


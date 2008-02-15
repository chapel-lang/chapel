/*$id$ */
#define _GNU_SOURCE
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
#  define VAPIDEV_NAME "InfiniHost0"
#  define INVAL_HNDL 0xFFFFFFFF
#define RNR_TIMER 12

u_int32_t armci_max_num_sg_ent;
u_int32_t armci_max_qp_ous_swr;
u_int32_t armci_max_qp_ous_rwr;

typedef struct {
   struct ibv_qp *qp;
   uint32_t sqpnum;                /*we need to exchng qp nums,arr for that*/
   uint32_t *rqpnum;               /*we need rqp nums,arr for that*/
   uint16_t lid;
} armci_connect_t;
armci_connect_t *CLN_con, *SRV_con;
/*\
 * datastrucure for infinihost NIC
\*/
typedef struct {
  uint16_t *lid_arr;                /*we need to exchange lids, arr for that*/
  struct ibv_context *handle;       /*device context/handle*/
  int maxtransfersize;
  struct ibv_device_attr attr;      /*device properties*/
  struct ibv_port_attr hca_port;    /*mostly for getting lid*/
  uint8_t active_port;
  struct ibv_pd *ptag;              /*protection tag*/
  const char *vendor;
  struct ibv_cq *scq;               /*send completion queue*/
  struct ibv_cq *rcq;               /*recv completion queue*/
  struct ibv_comp_channel *sch;     /*send completion channel*/
  struct ibv_comp_channel *rch;     /*recv completion channel*/
  void *scq_cntx;                   /*send context for completion queue*/
  void *rcq_cntx;                   /*recv context for completion queue*/
  int scv;                          /*send completion vector*/
  int rcv;                          /*recv completion vector*/
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
    struct ibv_recv_wr  dscr;
    struct ibv_sge      sg_entry;
    char buf[VBUF_DLEN];
} vapibuf_t;

typedef struct {
    struct ibv_send_wr  snd_dscr;
    struct ibv_sge      ssg_entry;
    struct ibv_recv_wr  rcv_dscr;
    struct ibv_sge      rsg_entry;
          char buf[VBUF_DLEN];
} vapibuf_ext_t;

typedef struct {
    struct ibv_send_wr  rmw_dscr;
    struct ibv_sge      rmw_entry;
} vapirmw_t;


static vapibuf_t **serv_buf_arr, *spare_serv_buf,*spare_serv_bufptr;
static vapibuf_ext_t *serv_buf;

static vapirmw_t rmw[64];

static int *flag_arr; /* flag indicates its receiving scatter data */
#define SERV 2
#define CLN 1

#define MAX_DESCR 2
typedef struct {
    int avail;
    struct ibv_qp *qp;
    struct ibv_recv_wr *descr;
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
static descr_pool_t serv_descr_pool = {MAX_DESCR,NULL,NULL};
static descr_pool_t client_descr_pool = {MAX_DESCR,NULL,NULL};

#define GET_DATA_PTR(buf) (sizeof(request_header_t) + (char*)buf)

#define BUF_TO_SDESCR(buf) ((struct ibv_send_wr *)(&((armci_vapi_field_t *)((char *)(buf) - sizeof(armci_vapi_field_t)))->sdscr))

#define BUF_TO_RDESCR(buf) ((struct ibv_recv_wr *)(&((armci_vapi_field_t *)((char *)(buf) - sizeof(armci_vapi_field_t)))->rdscr))

#define BUF_TO_SSGLST(buf) ((struct ibv_sge *)(&((armci_vapi_field_t *)((char *)(buf) - sizeof(armci_vapi_field_t)))->ssg_entry))

#define BUF_TO_RSGLST(buf) ((struct ibv_sge *)(&((armci_vapi_field_t *)((char *)(buf) - sizeof(armci_vapi_field_t)))->rsg_entry))

#define BUF_TO_EVBUF(buf) (vapibuf_ext_t*)(((char*)buf) - (sizeof(struct ibv_send_wr)+sizeof(struct ibv_recv_wr)+2*sizeof(struct ibv_sge)))

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
/********************FUNCTIONS TO CHECK OPENIB RETURN STATUS*******************/
void armci_check_status(int debug, int rc,char *msg)
{
    if(debug)printf("%d:%s, rc = %d\n", armci_me,msg, rc);
    if(rc!=0)armci_die(msg,rc);
}

void armci_vapi_check_return(int debug, int ret, const char *ss)
{
#if 0
    if(ret!=VAPI_OK){
       printf("\n%d:from %s ret=%d str=%s str_sym=%s\n",armci_me,ss,ret,
		       VAPI_strerror(ret),VAPI_strerror_sym(ret));
       fflush(stdout);
    }
    if(debug){
       printf("\n%d:from %s ret=%d str=%s str_sym=%s\n",armci_me,ss,ret,
		       VAPI_strerror(ret),VAPI_strerror_sym(ret));
    }
#endif
}

void armci_vapi_print_dscr_info(struct ibv_send_wr *sr, struct ibv_recv_wr *rr)
{
int i;
    if(rr){
       printf("\n%d:print_dscr rr id=%ld sg_lst_len=%d",
              armci_me, rr->wr_id, rr->num_sge);
       for (i = 0; i < rr->num_sge; i++) {
         printf("\n\t:sg_entry=%d addr=%p len=%d",
                i, rr->sg_list[i].addr, rr->sg_list[i].length);
       }
       fflush(stdout);
    }
    if(sr){
       printf("\n%d:print_dscr sr id=%d opcode=%d sg_lst_len=%d",
              armci_me, sr->wr_id, sr->opcode, sr->num_sge);
       for (i = 0; i < sr->num_sge; i++) {
         printf("\n\t:sg_entry=%d addr=%p len=%d",
                i, sr->sg_list[i].addr, sr->sg_list[i].length);
       }
       fflush(stdout);
    }
}

/*****************END FUNCTIONS TO CHECK VAPI RETURN STATUS********************/

void armci_recv_complete(struct ibv_recv_wr *rcv_dscr, char *from, int numofrecvs) /*needs work*/
{
int rc=0;
struct ibv_wc pdscr1;
struct ibv_wc *pdscr = &pdscr1;
sr_descr_t *rdscr_arr;
vapi_nic_t *nic;
int debug,i,done=0;

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
               ((SERVER_CONTEXT)?"(s)":" "),from,rcv_dscr->wr_id);fflush(stdout);
    }
    for(i=0;i<numofrecvs;i++){
    do{
       while(rc == 0)
         rc = ibv_poll_cq(nic->rcq, 1, pdscr);
       if(rc<0)armci_check_status(debug,rc,"client_scatter_rcv");
       if(debug){
         if(pdscr->wr_id >= DSCRID_SCATGAT && pdscr->wr_id < DSCRID_SCATGAT_END)
           printf("\n%d:recv from %s complete id=%d num=%d",armci_me,
             from,pdscr->wr_id,rdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].numofrecvs);
       }
       if(pdscr->wr_id >= DSCRID_SCATGAT && pdscr->wr_id < DSCRID_SCATGAT_END){
         rdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].numofrecvs--;
         if(rdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].numofrecvs==0)
           rdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].tag=0;
       }
       else if(pdscr->wr_id == (DSCRID_SCATGAT + MAX_PENDING)){
               /*this was from a blocking call, do nothing*/
         continue;
       }
       else {
         armci_die("\nclient should be posting only one kind of recv",armci_me);
       }
       rc = 0;
   }while(pdscr->wr_id!=rcv_dscr->wr_id);
   rc = 0;
   }

}

void armci_vapi_set_mark_buf_send_complete(int id)
{
    mark_buf_send_complete[id]=0;
}

void armci_send_complete(struct ibv_send_wr *snd_dscr, char *from,int numoftimes)
{
int rc=0;
struct ibv_wc pdscr1;
struct ibv_wc *pdscr = &pdscr1;
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
               ((SERVER_CONTEXT)?"(s)":" "),from,snd_dscr->wr_id,numoftimes);
       fflush(stdout);
    }
    for(i=0;i<numoftimes;i++){
    do{
       while(rc == 0){  
         rc = ibv_poll_cq(nic->scq,1, pdscr);
       }  
       if(rc<0)armci_die("got negative return code for ib_poll_cq",rc);
       if(SERVER_CONTEXT){
         if(rc<0)armci_check_status(DEBUG_SERVER,rc,"armci_send_complete wait fr send");
         if(debug)printf("%d:completed id %d i=%d\n",armci_me,pdscr->wr_id,i);
         if(pdscr->wr_id >=DSCRID_SCATGAT && pdscr->wr_id < DSCRID_SCATGAT_END){
           sdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].numofsends--;
           if(sdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].numofsends==0)
             sdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].tag=0;
         }
         else if(pdscr->wr_id >=armci_nproc && pdscr->wr_id < 2*armci_nproc){
                 /*its coming from send_data_to_client just return*/
         }
         else armci_die("server send complete got weird id",pdscr->wr_id);
       }
       else{
         if(rc<0)armci_check_status(DEBUG_CLN,rc,"armci_send_complete wait for send");
         if(debug)printf("%d:completed id %d i=%d\n",armci_me,pdscr->wr_id,i);
         if(pdscr->wr_id >=DSCRID_FROMBUFS && pdscr->wr_id < DSCRID_FROMBUFS_END)
           mark_buf_send_complete[pdscr->wr_id]=1;
         else if(pdscr->wr_id >=DSCRID_NBDSCR && pdscr->wr_id < DSCRID_NBDSCR_END){
           sdscr_arr[pdscr->wr_id-DSCRID_NBDSCR].tag=0;
           sdscr_arr[pdscr->wr_id-DSCRID_NBDSCR].numofsends=0;
         }
         else if(pdscr->wr_id >=DSCRID_SCATGAT && pdscr->wr_id < DSCRID_SCATGAT_END){
           sdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].numofsends--;
           if(sdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].numofsends==0)
             sdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].tag=0;
         }
         else if(pdscr->wr_id == (DSCRID_SCATGAT + MAX_PENDING)){
           /*this was from a blocking call, do nothing*/
           continue;
         }
         else armci_die("client send complete got weird id",pdscr->wr_id);
       }
       rc = 0;
    }while(pdscr->wr_id!=snd_dscr->wr_id);
       rc = 0;
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
         bzero(&rdscr_arr[i].rdescr,sizeof(struct ibv_recv_wr)); 
         if(sg)
           rdscr_arr[i].rdescr.wr_id = DSCRID_SCATGAT + i;
         else
           rdscr_arr[i].rdescr.wr_id = DSCRID_NBDSCR + i; 
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

    memset(&retdscr->rdescr,0,sizeof(struct ibv_recv_wr));

    if(sg)
       retdscr->rdescr.wr_id = DSCRID_SCATGAT + avail;
    else{
       retdscr->rdescr.wr_id = DSCRID_NBDSCR + avail; 
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
         bzero(&sdscr_arr[i].sdescr,sizeof(struct ibv_recv_wr));
	 if(sg)
           sdscr_arr[i].sdescr.wr_id = DSCRID_SCATGAT+i;
	 else
           sdscr_arr[i].sdescr.wr_id = DSCRID_NBDSCR + i;
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

    memset(&retdscr->sdescr,0,sizeof(struct ibv_recv_wr));

    if(sg)
       retdscr->sdescr.wr_id = DSCRID_SCATGAT + avail;
    else{
       retdscr->sdescr.wr_id = DSCRID_NBDSCR + avail;
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


/* ibv_create_qp does not use separate structure to return properties,
   seems it is all inside ibv_qp */
static void armci_create_qp(vapi_nic_t *nic, struct ibv_qp **qp)
{
    struct ibv_qp_init_attr initattr;

    bzero(&initattr, sizeof(struct ibv_qp_init_attr));
    *qp=NULL;

    initattr.cap.max_send_wr = armci_max_qp_ous_rwr;
    initattr.cap.max_recv_wr = armci_max_qp_ous_swr;
    initattr.cap.max_recv_sge = armci_max_num_sg_ent;
    initattr.cap.max_send_sge = armci_max_num_sg_ent;
    initattr.send_cq = nic->scq;
    initattr.recv_cq = nic->rcq;
    initattr.qp_type = IBV_QPT_RC;

    if(DEBUG_INIT){
       printf("\n%d:here in create_qp before call\n",armci_me);
    }

    *qp = ibv_create_qp(nic->ptag, &initattr);

    if(!armci_vapi_max_inline_size){
       armci_vapi_max_inline_size = initattr.cap.max_inline_data;
       if(DEBUG_CLN){
         printf("\n%d:maxinline size=%d\n",armci_me,armci_vapi_max_inline_size);
#if 0
         printf("\n%d:swr=%d rwr=%d ssg=%d rsg=%d\n",armci_me,qp_prop->cap.max_oust_wr_sq,qp_prop->cap.max_oust_wr_rq,qp_prop->cap.max_sg_size_sq,qp_prop->cap.max_sg_size_rq);fflush(stdout);
#endif
       }
    }

    if (!*qp)
        armci_die("create qp returned NULL", 0);
}

static void armci_init_nic(vapi_nic_t *nic, int scq_entries, int rcq_entries)
{
    int rc, ndevs, i;
    struct ibv_device **devs=NULL;
    struct ibv_context *cxt;

    bzero(nic,sizeof(vapi_nic_t));
    nic->lid_arr = (uint16_t *)calloc(armci_nproc,sizeof(uint16_t));
    if(!nic->lid_arr)
        armci_die("malloc for nic_t arrays in vapi.c failed",0);

    devs = ibv_get_device_list(&ndevs);
    if (!devs)
        armci_die("ibv_get_device_list returned NULL",0);

    nic->handle = ibv_open_device(*devs); /*open first device*/
    if (!nic->handle)
        armci_die("ibv_open_device_list returned NULL",0);

    nic->maxtransfersize = MAX_RDMA_SIZE;

    nic->vendor = ibv_get_device_name(*devs);
    if (!nic->vendor)
        armci_die("ibv_get_device_name returned NULL",0);

    rc = ibv_query_device(nic->handle, &nic->attr);
    armci_check_status(DEBUG_INIT, rc, "query device");

    for (i = 1; i <= 2; i++) {
        rc = ibv_query_port(nic->handle, (uint8_t)i, &nic->hca_port);
        armci_check_status(DEBUG_INIT, rc, "query ports");
        if (IBV_PORT_ACTIVE == nic->hca_port.state) {
            nic->active_port = i;
            break;
        }
    }

    /*save the lid for doing a global exchange later */
    nic->lid_arr[armci_me] = nic->hca_port.lid;

    /*allocate tag (protection domain) */
    nic->ptag = ibv_alloc_pd(nic->handle);
    if (!nic->ptag)
        armci_die("ibv_alloc_pd returned NULL",0);

    /* properties of scq and rcq required for the cq number, this also needs
     * to be globally exchanged
     */
    nic->scv = 1;
    nic->rcv = 2;
    nic->scq = nic->rcq = NULL; /*INVAL_HNDL;*/
    /*do the actual queue creation */
    if(scq_entries) {
        nic->sch = ibv_create_comp_channel(nic->handle);
        if (!nic->sch)
            armci_die("ibv_create_comp_channel (send) returned NULL",0);
        nic->scq=ibv_create_cq(nic->handle,4000,nic->scq_cntx,nic->sch,0);
        if (!nic->scq)
            armci_die("ibv_create_cq (send) returned NULL",0);
    }
    if(rcq_entries) {
        nic->rch = ibv_create_comp_channel(nic->handle);
        if (!nic->rch)
             armci_die("ibv_create_comp_channel (recv) returned NULL",0);
        nic->rcq=ibv_create_cq(nic->handle,4000,nic->rcq_cntx,nic->rch,0);
        if (!nic->rcq)
            armci_die("ibv_create_cq (recv) returned NULL",0);
    }
    ibv_free_device_list(devs);

    /*set local variable values*/
    armci_max_num_sg_ent=30;
    armci_max_qp_ous_swr=800;
    armci_max_qp_ous_rwr=400*16;
    if(armci_nproc>200){
       armci_max_qp_ous_swr=armci_nproc;
       armci_max_qp_ous_rwr=armci_nproc*16;
       if(armci_me==0)printf("\nARMCI:%d:qp_swr changed to %d qp_rwr changed to %d",armci_me,armci_max_qp_ous_swr,armci_max_qp_ous_rwr);
    }
    if(armci_max_qp_ous_rwr>nic->attr.max_qp_wr){
       armci_max_qp_ous_swr=nic->attr.max_qp_wr/16;
       armci_max_qp_ous_rwr=nic->attr.max_qp_wr;
       if(armci_me==0)printf("\nARMCI:%d:qp_swr changed to %d qp_rwr changed to %d",armci_me,armci_max_qp_ous_swr,armci_max_qp_ous_rwr);
    }
    if(armci_max_num_sg_ent>nic->attr.max_sge){
       armci_max_num_sg_ent=nic->attr.max_sge-1;
       if(armci_me==0)
       printf("\n%d:Based on attributes from NIC, max SG list has changed to %d"
                       ,armci_me,armci_max_num_sg_ent);
    }
}

/****************MEMORY ALLOCATION REGISTRATION DEREGISTRATION****************/
static char * serv_malloc_buf_base;
#ifdef ARMCI_ENABLE_GPC_CALLS
extern gpc_buf_t *gpc_req;
#endif
void armci_server_alloc_bufs()
{
    int rc;
    int mod, bytes, total, extra =sizeof(struct ibv_recv_wr)*MAX_DESCR+SIXTYFOUR;
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
    tmp0=tmp = malloc(total);
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
    serv_descr_pool.descr= (struct ibv_recv_wr *)(tmp+SIXTYFOUR-mod);
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

   flag_arr = (int *)malloc(sizeof(int)*armci_nproc);
   for (i =0; i<armci_nproc; i++) flag_arr[i] = 9999;

    if(DEBUG_SERVER){
      printf("\n%d(s):registering mem %p %dbytes ptag=%ld handle=%d\n",
             armci_me, tmp0,total,CLN_nic->ptag,CLN_nic->handle);fflush(stdout);
    }

    serv_memhandle.memhndl = ibv_reg_mr(CLN_nic->ptag, tmp0, total,
                                        IBV_ACCESS_LOCAL_WRITE |
                                        IBV_ACCESS_REMOTE_WRITE |
                                        IBV_ACCESS_REMOTE_READ);
    if (!serv_memhandle.memhndl)
        armci_die("server register recv vbuf returned NULL", 0);
    serv_memhandle.lkey=serv_memhandle.memhndl->lkey;
    serv_memhandle.rkey=serv_memhandle.memhndl->rkey;

    /* exchange address of ack/memhandle flag on servers */
    if(DEBUG_SERVER){
       printf("%d(s):registered mem %p %dbytes mhandle=%d mharr starts%p\n",
              armci_me, tmp0, total, serv_memhandle.memhndl,CLN_handle);
       fflush(stdout);
    }
}

static char * client_malloc_buf_base;
char * armci_vapi_client_mem_alloc(int size)
{
    int rc;
    int mod, total;
    int extra = MAX_DESCR*sizeof(struct ibv_recv_wr)+SIXTYFOUR;
    char *tmp,*tmp0;

    /*we use the size passed by the armci_init_bufs routine instead of bytes*/

    total = size + extra + 2*SIXTYFOUR;

    if(total%4096!=0)
       total = total - (total%4096) + 4096;
    tmp0  = tmp = malloc(total);
    client_malloc_buf_base = tmp;
    if(ALIGN64ADD(tmp0))tmp0+=ALIGN64ADD(tmp0);
    if(!tmp) armci_die("failed to malloc client bufs",total);
    /* stamp the last byte */
    client_tail= tmp + extra+ size +2*SIXTYFOUR-1;
    *client_tail=CLIENT_STAMP;

    /* we also have a place to store memhandle for zero-copy get */
    pinned_handle =(armci_vapi_memhndl_t *) (tmp + extra+ size +SIXTYFOUR-16);

    mod = ((ssize_t)tmp)%SIXTYFOUR;
    client_descr_pool.descr= (struct ibv_recv_wr*)(tmp+SIXTYFOUR-mod);
    tmp += extra;

    client_memhandle.memhndl = ibv_reg_mr(SRV_nic->ptag, tmp0, total,
                                          IBV_ACCESS_LOCAL_WRITE |
                                          IBV_ACCESS_REMOTE_WRITE |
                                          IBV_ACCESS_REMOTE_READ);
    if (!client_memhandle.memhndl)
        armci_die("client register send vbuf returned NULL", 0);
    
    client_memhandle.lkey = client_memhandle.memhndl->lkey;
    client_memhandle.rkey = client_memhandle.memhndl->rkey;
    handle_array[armci_me].lkey = client_memhandle.lkey;
    handle_array[armci_me].rkey = client_memhandle.rkey;
  
    handle_array[armci_me].memhndl = client_memhandle.memhndl;

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
    bzero(memhdl,sizeof(ARMCI_MEMHDL_T));

    memhdl->memhndl = ibv_reg_mr(CLN_nic->ptag, ptr, bytes,
               IBV_ACCESS_LOCAL_WRITE |
               IBV_ACCESS_REMOTE_WRITE |
               IBV_ACCESS_REMOTE_READ);
    if (!memhdl)
        armci_die("server register region returned NULL", 0);

    memhdl->lkey=memhdl->memhndl->lkey;
    memhdl->rkey=memhdl->memhndl->rkey;

    if(DEBUG_SERVER){
       printf("\n%d(s):registered lkey=%d rkey=%d ptr=%p end=%p %p\n",armci_me,
               memhdl->lkey,memhdl->rkey,ptr,(char *)ptr+bytes,memhdl);
       fflush(stdout);
    }
}

int armci_pin_contig_hndl(void *ptr, int bytes, ARMCI_MEMHDL_T *memhdl)
{
    memhdl->memhndl = ibv_reg_mr(SRV_nic->ptag, ptr, bytes,
               IBV_ACCESS_LOCAL_WRITE |
               IBV_ACCESS_REMOTE_WRITE |
               IBV_ACCESS_REMOTE_READ);
    if (!memhdl->memhndl)
        armci_die("client register regioni returned NULL", 0);
    memhdl->lkey=memhdl->memhndl->lkey;
    memhdl->rkey=memhdl->memhndl->rkey;
    if(0){
       printf("\n%d:registered lkey=%d rkey=%d ptr=%p end=%p\n",armci_me,
               memhdl->lkey,memhdl->rkey,ptr,(char *)ptr+bytes);fflush(stdout);
    }
    return 1;
}

#if 1
void armci_network_client_deregister_memory(ARMCI_MEMHDL_T *mh)
{
    int rc;
    rc = ibv_dereg_mr(mh->memhndl);
    armci_vapi_check_return(DEBUG_FINALIZE,rc,
                        "armci_network_client_deregister_memory:deregister_mr");
}
void armci_network_server_deregister_memory(ARMCI_MEMHDL_T *mh)
{
    int rc;
return; /* ??? why ??? */
    printf("\n%d:deregister ptr=%p",armci_me,mh);fflush(stdout);
    rc = ibv_dereg_mr(mh->memhndl);
    armci_vapi_check_return(DEBUG_FINALIZE,rc,
                        "armci_network_server_deregister_memory:deregister_mr");
}
#else
#   define armci_network_client_deregister_memory(mh)           \
           armci_vapi_check_return(DEBUG_FINALIZE,              \
                                   ibv_dereg_mr(mh->memhndl),   \
                                   "armci_network_client_deregister_memory:deregister_mr")
#   define armci_network_server_deregister_memory(mh)           \
           armci_vapi_check_return(DEBUG_FINALIZE,              \
                                   ibv_dereg_mr(mh->memhndl),   \
                                   "armci_network_server_deregister_memory:deregister_mr")
#endif

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
    sz = armci_nproc*(sizeof(uint32_t)/sizeof(int));
    armci_vapi_max_inline_size = 0;
    for(s=0; s< armci_nclus; s++){
       armci_connect_t *con = SRV_con + s;
       con->rqpnum = (uint32_t *)malloc(sizeof(uint32_t)*armci_nproc);
       bzero(con->rqpnum,sizeof(uint32_t)*armci_nproc);
       /*if(armci_clus_me != s)*/
       {
         armci_create_qp(SRV_nic,&con->qp);
         con->sqpnum  = con->qp->qp_num;
         con->rqpnum[armci_me] = con->qp->qp_num;
         con->lid = SRV_nic->lid_arr[s];
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
    int i, start, sz=0, c, rc;
    struct ibv_qp_attr qp_attr;
    struct ibv_qp_cap qp_cap;
    enum ibv_qp_attr_mask qp_attr_mask;

    if (TIME_INIT) inittime0 = MPI_Wtime();
    if (armci_me == armci_master)
        armci_util_wait_int(&armci_vapi_server_stage1, 1, 10);
    if (TIME_INIT) printf("\n%d:wait for server to get to stage 1 time for "
                          "vapi_connect_client is %f",
                          armci_me, (inittime1 = MPI_Wtime()) - inittime0);
    sz = armci_nproc;
    if (armci_me == armci_master) {
       armci_msg_gop_scope(SCOPE_MASTERS, _gtmparr, sz, "+", ARMCI_INT);
       for (c=0; c<armci_nproc; c++) {
         CLN_nic->lid_arr[c] = _gtmparr[c];
         _gtmparr[c] = 0;
       }
       if (DEBUG_CLN) {
         printf("\n%d(svc): mylid = %d",armci_me,CLN_nic->lid_arr[armci_me]);
         fflush(stdout);
       }
    }

    armci_vapi_client_stage1 = 1;

    /* allocate and initialize connection structs */
    sz = armci_nproc*(sizeof(uint32_t)/sizeof(int));

    if (armci_me == armci_master)
       armci_util_wait_int(&armci_vapi_server_stage2, 1, 10);
    for (c = 0; c < armci_nproc; c++){
       armci_connect_t *con = CLN_con + c;
       if (armci_me != armci_master) {
         char *ptrr;
         int extra;
         ptrr = malloc(8 + sizeof(uint32_t) * armci_nproc);
         extra = ALIGNLONGADD(ptrr);
         ptrr = ptrr + extra;
         con->rqpnum = (uint32_t *)ptrr;
         bzero(con->rqpnum, sizeof(uint32_t) * armci_nproc);
       }
       armci_msg_gop_scope(SCOPE_ALL, con->rqpnum, sz, "+", ARMCI_INT);
    }

   if (TIME_INIT) printf("\n%d:wait for server tog et to stage 2 time for "
                         "vapi_connect_client is %f",
                         armci_me, (inittime2 = MPI_Wtime()) - inittime1);
    /*armci_set_serv_mh();*/

    if (DEBUG_CLN) {
        printf("%d:all connections ready\n", armci_me);
        fflush(stdout);
    }

    /* Modifying  QP to INIT */
    qp_attr_mask = IBV_QP_STATE
                 | IBV_QP_PKEY_INDEX
                 | IBV_QP_PORT
                 | IBV_QP_ACCESS_FLAGS;

    qp_attr.qp_state = IBV_QPS_INIT;
    qp_attr.pkey_index = DEFAULT_PKEY_IX;
    qp_attr.port_num = SRV_nic->active_port;
    qp_attr.qp_access_flags = IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_READ;

    /* start from from server on my_node -1 */
    start = (armci_clus_me == 0) ? armci_nclus - 1 : armci_clus_me - 1;
    for (i = 0; i < armci_nclus; i++) {
       armci_connect_t *con;
       con = SRV_con + i;
       rc = ibv_modify_qp(con->qp, &qp_attr, qp_attr_mask);
       armci_check_status(DEBUG_INIT, rc,"client connect requesti RST->INIT");
    }

    if (TIME_INIT) printf("\n%d:to init time for vapi_connect_client is %f",
                          armci_me, (inittime1 = MPI_Wtime()) - inittime2);
    qp_attr_mask = IBV_QP_STATE
                 | IBV_QP_MAX_DEST_RD_ATOMIC
                 | IBV_QP_PATH_MTU
                 | IBV_QP_RQ_PSN
                 | IBV_QP_MIN_RNR_TIMER;

    qp_attr.qp_state        = IBV_QPS_RTR;
    qp_attr.max_dest_rd_atomic   = 4;
    qp_attr.path_mtu        = IBV_MTU_1024;
    qp_attr.rq_psn          = 0;
    qp_attr.min_rnr_timer   = RNR_TIMER;

    start = (armci_clus_me == 0) ? armci_nclus - 1 : armci_clus_me - 1;
    for (i = 0; i < armci_nclus; i++) {
        armci_connect_t *con;
        armci_connect_t *conS;
        con = SRV_con + i;
        conS = CLN_con + armci_me;

        qp_attr_mask |= IBV_QP_AV | IBV_QP_DEST_QPN;
        qp_attr.dest_qp_num = conS->rqpnum[armci_clus_info[i].master];
        qp_attr.ah_attr.dlid = SRV_nic->lid_arr[armci_clus_info[i].master];
        qp_attr.ah_attr.port_num = SRV_nic->active_port;

        rc = ibv_modify_qp(con->qp, &qp_attr, qp_attr_mask);
        armci_check_status(DEBUG_INIT, rc,"client connect request INIT->RTR");
    }

    /*to to to RTS, other side must be in RTR*/
    armci_msg_barrier();
    if (TIME_INIT) printf("\n%d:init to rtr time for vapi_connect_client is %f",
                          armci_me, (inittime2 = MPI_Wtime()) - inittime1);
    armci_vapi_client_ready=1;

    qp_attr_mask = IBV_QP_STATE
                 | IBV_QP_SQ_PSN
                 | IBV_QP_TIMEOUT
                 | IBV_QP_RETRY_CNT
                 | IBV_QP_RNR_RETRY
                 | IBV_QP_MAX_QP_RD_ATOMIC;

    qp_attr.qp_state            = IBV_QPS_RTS;
    qp_attr.sq_psn              = 0;
    qp_attr.timeout             = 18;
    qp_attr.retry_cnt           = 7;
    qp_attr.rnr_retry           = 7;
    qp_attr.max_rd_atomic  = 4;

    start = (armci_clus_me == 0) ? armci_nclus - 1 : armci_clus_me - 1;
    for (i = 0; i < armci_nclus; i++){
       armci_connect_t *con;
       con = SRV_con + i;
       rc = ibv_modify_qp(con->qp, &qp_attr, qp_attr_mask);
       armci_check_status(DEBUG_CLN, rc,"client connect request RTR->RTS");
       if(rc!=0)armci_die("client RTS fail",rc);
    }
    if (TIME_INIT) printf("\n%d:rtr to rts time for vapi_connect_client is %f",
                          armci_me, (inittime1 = MPI_Wtime()) - inittime2);
}


void armci_client_connect_to_servers()
{
    extern void armci_util_wait_int(volatile int *,int,int);
    if (TIME_INIT) inittime0 = MPI_Wtime();
    _armci_buf_init();

    vapi_connect_client();
    if (armci_me == armci_master) 
       armci_util_wait_int(&armci_vapi_server_ready,1,10);
    armci_msg_barrier();
    if (DEBUG_CLN && armci_me == armci_master) {
       printf("\n%d:server_ready=%d\n",armci_me,armci_vapi_server_ready);
       fflush(stdout);
    }
    if (TIME_INIT) printf("\n%d:time for client_connect_to_s is %f",
                          armci_me,MPI_Wtime()-inittime0);
}


void armci_init_vapibuf_recv(struct ibv_recv_wr *rd, struct ibv_sge *sg_entry,
                             char *buf, int len, armci_vapi_memhndl_t *mhandle)
{
     memset(rd,0,sizeof(struct ibv_recv_wr));
     rd->num_sge    = 1;
     rd->sg_list    = sg_entry;
     rd->wr_id      = 0;

     sg_entry->lkey     = mhandle->lkey;
     sg_entry->addr     = (uint64_t)buf;
     sg_entry->length   = len;
}


void armci_init_vapibuf_send(struct ibv_send_wr *sd, struct ibv_sge *sg_entry,
                             char *buf, int len, armci_vapi_memhndl_t *mhandle)
{
     sd->opcode = IBV_WR_SEND;
     sd->send_flags = IBV_SEND_SIGNALED;
     sd->num_sge            = 1;
     sd->sg_list            = sg_entry;
     sd->wr.ud.remote_qkey  = 0;

     sg_entry->lkey     = mhandle->lkey;
     sg_entry->addr     = (uint64_t)buf;
     sg_entry->length   = len;
}


static void armci_init_vbuf_srdma(struct ibv_send_wr *sd, struct ibv_sge *sg_entry,
                                  char *lbuf, char *rbuf, int len,
                                  armci_vapi_memhndl_t *lhandle,
                                  armci_vapi_memhndl_t *rhandle)
{
     /* NOTE: sd->wr is a union, sr->wr.ud might conflict with sr->wr.rdma */
     sd->opcode = IBV_WR_RDMA_WRITE;
     sd->send_flags = IBV_SEND_SIGNALED;
     sd->num_sge                    = 1;
     sd->sg_list                    = sg_entry;
     sd->wr.ud.remote_qkey          = 0;
     if (rhandle) sd->wr.rdma.rkey  = rhandle->rkey;
     sd->wr.rdma.remote_addr        = (uint64_t)rbuf;

     if (lhandle) sg_entry->lkey    = lhandle->lkey;
     sg_entry->addr                 = (uint64_t)lbuf;
     sg_entry->length               = len;
}


static void armci_init_vbuf_rrdma(struct ibv_send_wr *sd, struct ibv_sge *sg_entry,
                                  char *lbuf, char *rbuf, int len,
                                  armci_vapi_memhndl_t *lhandle,
                                  armci_vapi_memhndl_t *rhandle)
{
     sd->opcode = IBV_WR_RDMA_READ;
     sd->send_flags = IBV_SEND_SIGNALED;
     sd->num_sge                    = 1;
     sd->sg_list                    = sg_entry;
     sd->wr.ud.remote_qkey          = 0;
     if (rhandle) sd->wr.rdma.rkey  = rhandle->rkey;
     sd->wr.rdma.remote_addr        = (uint64_t)rbuf;

     if (lhandle) sg_entry->lkey    = lhandle->lkey;
     sg_entry->addr                 = (uint64_t)lbuf;
     sg_entry->length               = len;
     /* sd->wr is a union, sr->wr.ud might conflict with sr->wr.rdma */
}


void armci_server_initial_connection()
{
    int c, ib, rc;
    struct ibv_qp_attr qp_attr;
    struct ibv_qp_init_attr qp_init_attr;
    struct ibv_qp_cap qp_cap;
    enum ibv_qp_attr_mask qp_attr_mask;
    char *enval;
    struct ibv_recv_wr *bad_wr;

    if (TIME_INIT) inittime0 = MPI_Wtime();
    if (DEBUG_SERVER) {
       printf("in server after fork %d (%d)\n",armci_me,getpid());
       fflush(stdout);
    }

    armci_init_nic(CLN_nic,1,1);

    _gtmparr[armci_me] = CLN_nic->lid_arr[armci_me];
    armci_vapi_server_stage1 = 1;
    armci_util_wait_int(&armci_vapi_client_stage1, 1, 10);
    if (TIME_INIT) printf("\n%d:wait for client time for server_initial_conn is %f",
                          armci_me, (inittime4 = MPI_Wtime()) - inittime0);

    for (c = 0; c < armci_nproc; c++) {
       char *ptrr;
       int extra;
       armci_connect_t *con = CLN_con + c;
       if (DEBUG_SERVER) {
         printf("\n%d:create qp before malloc c=%d\n",armci_me,c);
         fflush(stdout);
       }
       ptrr = malloc(8 + sizeof(uint32_t) * armci_nproc);
       extra = ALIGNLONGADD(ptrr);
       ptrr = ptrr + extra;
       con->rqpnum = (uint32_t *)ptrr;
       bzero(con->rqpnum, sizeof(uint32_t) * armci_nproc);
       armci_create_qp(CLN_nic, &con->qp);
       con->sqpnum = con->qp->qp_num;
       con->lid    = CLN_nic->lid_arr[c];
       con->rqpnum[armci_me]  = con->qp->qp_num;
       if (DEBUG_SERVER) {
         printf("\n%d:create qp success  for server c=%d\n",armci_me,c);fflush(stdout);
       }
    }
    if (DEBUG_SERVER) {
       printf("\n%d:create qps success for server",armci_me);fflush(stdout);
    }
    if (TIME_INIT) printf("\n%d:create qp time for server_initial_conn is %f",
                          armci_me, (inittime1 = MPI_Wtime()) - inittime4);

    armci_vapi_server_stage2 = 1;

    qp_attr_mask = IBV_QP_STATE
                 | IBV_QP_PKEY_INDEX
                 | IBV_QP_PORT
                 | IBV_QP_ACCESS_FLAGS;

    qp_attr.qp_state        = IBV_QPS_INIT;
    qp_attr.pkey_index      = DEFAULT_PKEY_IX;
    qp_attr.port_num        = CLN_nic->active_port;
    qp_attr.qp_access_flags = IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_READ;

    for (c = 0; c < armci_nproc; c++) {
       armci_connect_t *con = CLN_con + c;
       rc = ibv_modify_qp(con->qp, &qp_attr, qp_attr_mask);
       armci_check_status(DEBUG_INIT, rc,"master connect request RST->INIT");
    }
    if (TIME_INIT) printf("\n%d:to init time for server_initial_conn is %f",
                          armci_me, (inittime2 = MPI_Wtime()) - inittime1);
    qp_attr_mask = IBV_QP_STATE
                 | IBV_QP_MAX_DEST_RD_ATOMIC
                 | IBV_QP_PATH_MTU
                 | IBV_QP_RQ_PSN
                 | IBV_QP_MIN_RNR_TIMER;
    qp_attr.qp_state           = IBV_QPS_RTR;
    qp_attr.path_mtu           = IBV_MTU_1024;          /*MTU*/
    qp_attr.max_dest_rd_atomic = 4;
    qp_attr.min_rnr_timer      = RNR_TIMER;
    qp_attr.rq_psn             = 0;

    for(c = 0; c < armci_nproc; c++) {
       armci_connect_t *con = CLN_con + c;
       armci_connect_t *conC = SRV_con + armci_clus_me;
       qp_attr_mask |= IBV_QP_DEST_QPN | IBV_QP_AV;
       qp_attr.dest_qp_num  = conC->rqpnum[c];
       qp_attr.ah_attr.dlid = SRV_nic->lid_arr[c];
       qp_attr.ah_attr.port_num = CLN_nic->active_port;

       if (DEBUG_SERVER) {
         printf("\n%d(s):connecting to %d rqp = %d dlid=%d\n",armci_me,c,
                  conC->rqpnum[c],qp_attr.ah_attr.dlid);fflush(stdout);
       }

       rc = ibv_modify_qp(con->qp, &qp_attr, qp_attr_mask); 
       armci_check_status(DEBUG_SERVER, rc,"master connect request INIT->RTR");
       if(rc!=0)armci_die("ibv_modify_qp to RTR failed",rc);

    }
    if (TIME_INIT) printf("\n%d:init to rtr time for server_initial_conn is %f",
                          armci_me, (inittime3 = MPI_Wtime()) - inittime2);

    armci_util_wait_int(&armci_vapi_client_ready,1,10);

    qp_attr_mask = IBV_QP_STATE
                 | IBV_QP_SQ_PSN
                 | IBV_QP_TIMEOUT
                 | IBV_QP_RETRY_CNT
                 | IBV_QP_RNR_RETRY
                 | IBV_QP_MAX_QP_RD_ATOMIC;

    qp_attr.qp_state            = IBV_QPS_RTS;
    qp_attr.sq_psn              = 0;
    qp_attr.timeout             = 18;
    qp_attr.retry_cnt           = 7;
    qp_attr.rnr_retry           = 7;
    qp_attr.max_rd_atomic  = 4;

    for (c = 0; c < armci_nproc; c++) {
       armci_connect_t *con = CLN_con + c;
       rc = ibv_modify_qp(con->qp, &qp_attr,qp_attr_mask);
       armci_check_status(DEBUG_SERVER, rc,"master connect request RTR->RTS");
       if(rc!=0)armci_die("master RTS fail",rc);
    }
    if (TIME_INIT) printf("\n%d:rtr to rts time for server_initial_conn is %f",
                          armci_me, (inittime4 = MPI_Wtime()) - inittime3);

    if(DEBUG_SERVER)
       printf("%d:server thread done with connections\n",armci_me);

    armci_server_alloc_bufs();/* create receive buffers for server thread */

    /* setup descriptors and post nonblocking receives */
    for(c = ib = 0; c < armci_nproc; c++) {
       vapibuf_t *vbuf = serv_buf_arr[c];
       armci_init_vapibuf_recv(&vbuf->dscr, &vbuf->sg_entry, vbuf->buf,
                               VBUF_DLEN, &serv_memhandle);
       /* we use index of the buffer to identify the buffer, this index is
        * returned with a call toibv_poll_cq inside the ibv_wr */
       vbuf->dscr.wr_id = c + armci_nproc;
       if (DEBUG_SERVER) {
         printf("\n%d(s):posted rr with lkey=%d",armci_me,vbuf->sg_entry.lkey);
         fflush(stdout);
       }

       rc = ibv_post_recv((CLN_con+c)->qp, &vbuf->dscr, &bad_wr);
       armci_check_status(DEBUG_SERVER, rc,"server post recv vbuf");
    }

    if (TIME_INIT) printf("\n%d:post time for server_initial_conn is %f",
                          armci_me, MPI_Wtime() - inittime4);

    armci_vapi_server_ready=1;
    /* check if we can poll in the server thread */
    enval = getenv("ARMCI_SERVER_CAN_POLL");
    if (enval != NULL){
       if((enval[0] != 'N') && (enval[0]!='n')) server_can_poll=1;
    } else{
      if(armci_clus_info[armci_clus_me].nslave < armci_getnumcpus())
        server_can_poll=1;
    }
    server_can_poll=0;

    if (DEBUG_SERVER) {
       printf("%d: server connected to all clients\n",armci_me); fflush(stdout);
    }
    if (TIME_INIT) printf("\n%d:time for server_initial_conn is %f",
                          armci_me, MPI_Wtime() - inittime0);
}

static void armci_finalize_nic(vapi_nic_t *nic)
{
    int ret;

    ret = ibv_destroy_cq(nic->scq);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:destroy_scq");

    ret = ibv_destroy_comp_channel(nic->sch);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:destroy_sch");

    ret = ibv_destroy_cq(nic->rcq);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:destroy_rcq");

    ret = ibv_destroy_comp_channel(nic->rch);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:destroy_rch");

    ret = ibv_close_device(nic->handle);
    armci_vapi_check_return(DEBUG_FINALIZE,ret,"armci_finalize_nic:release_hca");

}


void armci_server_transport_cleanup()
{
    int s;
    int rc;

    /*first we have empty send/recv queues TBD*/
    if(serv_malloc_buf_base){
        rc = ibv_dereg_mr(serv_memhandle.memhndl);
        armci_vapi_check_return(DEBUG_FINALIZE,rc,
                                "armci_server_transport_cleanup:deregister_mr");
       /*now free it*/
       free(serv_malloc_buf_base);
    }
    /*now deregister all my regions from regionskk.c*/
    armci_server_region_destroy();
    if (CLN_con) {
        for (s = 0; s < armci_nproc; s++) {
            armci_connect_t *con = CLN_con + s;
            if (con->qp) {
                rc = ibv_destroy_qp(con->qp);
                armci_vapi_check_return(DEBUG_FINALIZE,rc,
                                        "armci_server_transport_cleanup:destroy_qp");
            }
            free(con->rqpnum);
        }
        free(CLN_con);
    }
    armci_finalize_nic(CLN_nic);
}

void armci_transport_cleanup()
{
    int s;
    int rc;

    /*first deregister buffers memory */
    if (client_malloc_buf_base) {
        rc = ibv_dereg_mr(client_memhandle.memhndl);
        armci_vapi_check_return(DEBUG_FINALIZE,rc,"armci_client_transport_cleanup:deregister_mr");
        /*now free it*/
        free(client_malloc_buf_base);
    }
    /*now deregister all my regions from regions.c*/
    armci_region_destroy();
    if (SRV_con) {
        for (s = 0; s < armci_nclus; s++) {
            armci_connect_t *con = SRV_con + s;
            if (con->qp) {
                rc = ibv_destroy_qp(con->qp);
                armci_vapi_check_return(DEBUG_FINALIZE,rc,"armci_client_transport_cleanup:destroy_qp");
            }
            free(con->rqpnum);
        }
        free(SRV_con);
    }
    armci_finalize_nic(SRV_nic);
}

/*#define CHANGE_SERVER_AFFINITY*/
#define DATA_SERVER_YIELD_CPU
void armci_call_data_server()
{
int rc = 0;
int rc1 = 0;
vapibuf_t *vbuf,*vbufs;
request_header_t *msginfo,*msg;
int c,i,need_ack,pollcount;
static int mytag=1;
static int doineednotify=1;
int rrr,serverwcount=0;

#ifdef CHANGE_SERVER_AFFINITY
cpu_set_t mycpuid,new_mask;
char str[CPU_SETSIZE];
char cid[8];
extern char * cpuset_to_cstr(cpu_set_t *mask, char *str);
int nslave=armci_clus_info[armci_clus_me].nslave;
    rrr=sched_getaffinity(0, sizeof(mycpuid), &mycpuid);
#endif

#ifdef ARMCI_ENABLE_GPC_CALLS
    unblock_thread_signal(GPC_COMPLETION_SIGNAL);
#endif
    for (;;) {
      struct ibv_wc *pdscr=NULL;
      struct ibv_wc pdscr1;
      pdscr = &pdscr1;
      rc = 0;
#ifdef CHANGE_SERVER_AFFINITY
      static int ccc;
      serverwcount++;
      if(serverwcount==100){
        serverwcount=0;
        ccc=(ccc+1)%nslave;
        sprintf (cid, "%d", ccc);
        rrr = cstr_to_cpuset(&new_mask,cid);
        if (sched_setaffinity(0, sizeof (new_mask), &new_mask)) {
          perror("sched_setaffinity");
          printf("failed to set pid %d's affinity.\n", getpid());
        }
        rrr=sched_getaffinity(0, sizeof(mycpuid), &mycpuid);
        if(rrr)perror("sched_getaffinity");
      }
#else
#ifdef DATA_SERVER_YIELD_CPU_
      serverwcount++;
      if(serverwcount==50){
        serverwcount=0;usleep(1);
      }
#endif
#endif

#ifdef ARMCI_ENABLE_GPC_CALLS
      block_thread_signal(GPC_COMPLETION_SIGNAL);
#endif
      if (server_can_poll) {
        rc = ibv_poll_cq(CLN_nic->rcq, 1, pdscr);
        while (rc == 0) {
          rc = ibv_poll_cq(CLN_nic->rcq, 1, pdscr);
          if (armci_server_terminating) {
            /* server got interrupted when clients terminate connections */
            armci_server_transport_cleanup();
            sleep(1);
            _exit(0);
          }
        }
      } else {
          rc = ibv_poll_cq(CLN_nic->rcq, 1, pdscr);
          if(rc==0){doineednotify=1;/*continue;*/}
          if(doineednotify){
            rc1 = ibv_req_notify_cq(CLN_nic->rcq, 0);
            if(rc1!=0){
              armci_check_status(DEBUG_SERVER, rc1,"CQ notify req fail");
            }
            if (ibv_get_cq_event(CLN_nic->rch, &CLN_nic->rcq, &CLN_nic->rcq_cntx)){
              armci_die("Failed to get cq_event\n",0);
            }
            doineednotify=0;
            rc = ibv_poll_cq(CLN_nic->rcq, 1, pdscr);
          }

        if (armci_server_terminating) {
          /* server got interrupted when clients terminate connections */
          armci_server_transport_cleanup();
          sleep(1);
          _exit(0);
        }
      }

      if(DEBUG_SERVER){
        printf("\n%d:pdscr=%p %p %d %d %d %d\n",armci_me,pdscr,&pdscr1,
                           pdscr->status,pdscr->opcode,pdscr->vendor_err,
                           pdscr->src_qp);
        fflush(stdout);
      }
      if(rc<0)armci_check_status(DEBUG_SERVER, rc,"server poll/block");
      /*we can figure out which buffer we got data info from the wc_desc_t id
        * this can tell us from which process we go the buffer, as well */
                      
       if (DEBUG_SERVER) {
         printf("%d(s) : NEW MESSAGE bytelen %d \n",armci_me,pdscr->byte_len);
         printf("%d(s) : NEW MESSAGE id is %ld \n",armci_me,pdscr->wr_id);
         fflush(stdout);
       }

       if (pdscr->wr_id >= DSCRID_SCATGAT && pdscr->wr_id < DSCRID_SCATGAT_END) {
         sr_descr_t *rdscr_arr;
         if (DEBUG_SERVER) {
           printf("%d(s) : received DATA id = %ld, length = %d\n",
                  armci_me,pdscr->wr_id, pdscr->byte_len);
           fflush(stdout);
         }
         rdscr_arr = armci_vapi_serv_nbrdscr_array;
         rdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].numofrecvs--;
         if(rdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].numofrecvs==0)
           rdscr_arr[pdscr->wr_id-DSCRID_SCATGAT].tag=0;
         continue;
       }

       vbuf = serv_buf_arr[pdscr->wr_id - armci_nproc];
       msginfo = (request_header_t*)vbuf->buf;
       armci_ack_proc = c = msginfo->from;

       if (DEBUG_SERVER) {
         printf("%d(s) : request id is %ld operation is %d, length is %d %d\n",
armci_me,pdscr->wr_id,msginfo->operation,pdscr->byte_len,msginfo->from);
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

          if (DEBUG_SERVER) {
            printf(" server:the dest_ptr is %p\n", dest_ptr);
            for(i =0; i<stride_levels; i++)
	      printf("stride_arr[i] is %d,value of count[i] is %d\n",
                               stride_arr[i], count[i]);
            printf("the value of stride_levels is %d\n", stride_levels);
            fflush(stdout);
          }

          found =get_armci_region_local_hndl(dest_ptr,armci_me, &loc_memhandle);

	  if(!found){
	    armci_die("SERVER : local region not found",pdscr->wr_id);
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

        vbufs = serv_buf_arr[pdscr->wr_id - armci_nproc] = spare_serv_buf;
        armci_init_vapibuf_recv(&vbufs->dscr, &vbufs->sg_entry,vbufs->buf,
                                VBUF_DLEN, &serv_memhandle);
        vbufs->dscr.wr_id = c + armci_nproc;

        struct ibv_recv_wr *bad_wr;
        rc = ibv_post_recv((CLN_con+c)->qp, &(vbufs->dscr), &bad_wr);
        armci_check_status(DEBUG_SERVER, rc,"server post recv vbuf");

        spare_serv_buf = vbuf;

        if(DEBUG_SERVER){
          printf("%d(s):Came out of poll id=%ld\n",armci_me,pdscr->wr_id);
          fflush(stdout);
        }

        if(msginfo->operation == REGISTER){
          if (DEBUG_SERVER) {
            printf("%d(s) : Register_op id is %d, comp_dscr_id is  %ld\n",
                     armci_me,msginfo->operation,pdscr->wr_id);
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
                    armci_me, pdscr->wr_id);
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
       } else need_ack=1;

       if (0) {
          printf("%d(s):Done processed request\n\n",armci_me);
          fflush(stdout);
       }

#ifdef ARMCI_ENABLE_GPC_CALLS
       unblock_thread_signal(GPC_COMPLETION_SIGNAL);
#endif
   }/* end of for */
}


void armci_vapi_complete_buf(armci_vapi_field_t *field,int snd,int rcv,int to,int op)
{
    struct ibv_send_wr *snd_dscr;

    BUF_INFO_T *info;
    info = (BUF_INFO_T *)((char *)field-sizeof(BUF_INFO_T));

    if(info->tag && op==GET)return;

    if(snd){
       request_header_t *msginfo = (request_header_t *)(field+1);
       snd_dscr=&(field->sdscr);
       if(mark_buf_send_complete[snd_dscr->wr_id]==0)
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
                                        struct ibv_send_wr *snd_dscr,char *from)
{
    int rc = 0;
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
       printf("vapi_post_send: snd_dscr->num_sge=%d, snd_dscr->sg_list->length=%d\n",
              snd_dscr->num_sge, snd_dscr->sg_list->length);
       fflush(stdout);
    }


    /* find the total length of all the segments */
    total = snd_dscr->sg_list->length * snd_dscr->num_sge;
    if(DEBUG_CLN){
       printf("%d(c) : total is %d\t, max_size is %d\n",armci_me,total,
                    armci_vapi_max_inline_size);
    }

    struct ibv_send_wr *bad_wr;
    if (total > armci_vapi_max_inline_size) {
        rc = ibv_post_send(con->qp, snd_dscr, &bad_wr);
    } else {
        rc = ibv_post_send(con->qp, snd_dscr, &bad_wr);
        /* no corresponding call, using ibv_post_send
       rc = EVAPI_post_inline_sr(nic->handle,con->qp,snd_dscr);*/
    }
    armci_check_status(DEBUG_INIT, rc, from);
}

int armci_send_req_msg(int proc, void *buf, int bytes)
{
    int cluster = armci_clus_id(proc);
    request_header_t *msginfo = (request_header_t *)buf;
    struct ibv_send_wr *snd_dscr;
    struct ibv_sge *ssg_lst;

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


    armci_vapi_post_send(1,cluster,snd_dscr,"send_req_msg:post_send");

    if(DEBUG_CLN){
       printf("%d:client sent REQ=%d %d bytes serv=%d qp=%ld id =%ld lkey=%d\n",
               armci_me,msginfo->operation,bytes,cluster,
               (SRV_con+cluster)->qp,snd_dscr->wr_id,ssg_lst->lkey);
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
int rc = 0;
sr_descr_t *dirdscr;
int clus = armci_clus_id(p);
struct ibv_send_wr *bad_wr;
    /*ID for the desr that comes from get_next_descr is already set*/
    dirdscr = armci_vapi_get_next_sdescr(nbtag,0);
    if(nbtag)*cptr = dirdscr;

    if(DEBUG_CLN){
      printf("\n%d: in direct get lkey=%d rkey=%d\n",armci_me,lochdl->lkey,
               remhdl->rkey);fflush(stdout);
    }

    armci_init_vbuf_rrdma(&dirdscr->sdescr,dirdscr->sg_entry,dst_buf,src_buf,
                          len,lochdl,remhdl);
    rc = ibv_post_send((SRV_con+clus)->qp, &(dirdscr->sdescr), &bad_wr);
    armci_check_status(DEBUG_CLN, rc,"armci_client_get_direct");
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

    if(mark_buf_send_complete[evbuf->snd_dscr.wr_id]==0)
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
    int rc = 0;
    struct ibv_send_wr *sdscr;

    sdscr=&serv_buf->snd_dscr;

    if(DEBUG_SERVER){
       printf("\n%d(s):sending data to client %d at %p flag = %p bytes=%d\n",
               armci_me,
               proc,dbuf,(char *)dbuf+bytes-sizeof(int),bytes);fflush(stdout);
    }

    memset(sdscr,0,sizeof(struct ibv_send_wr));
    armci_init_vbuf_srdma(sdscr,&serv_buf->ssg_entry,buf,dbuf,bytes,
                          &serv_memhandle,(handle_array+proc));

    if(DEBUG_SERVER){
       printf("\n%d(s):handle_array[%d]=%p dbuf=%p flag=%p bytes=%d\n",armci_me,
              proc,&handle_array[proc],(char *)dbuf,
              (char *)dbuf+bytes-sizeof(int),bytes);
       fflush(stdout);
    }

    serv_buf->snd_dscr.wr_id = proc+armci_nproc;
    struct ibv_send_wr *bad_wr;
    rc = ibv_post_send((CLN_con+proc)->qp, &serv_buf->snd_dscr, &bad_wr);
    armci_check_status(DEBUG_SERVER, rc,"server post send to client");

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
int rc;
int cluster = armci_clus_id(proc);
struct ibv_recv_wr *scat_dscr;
struct ibv_recv_wr *bad_wr;

    scat_dscr = &pend_dscr->rdescr;

    /*armci_vapi_print_dscr_info(NULL,scat_dscr);*/
    if((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN)){
       printf("%d(%d) : inside posts scatter dscr, id is %d\n",
	      armci_me,type,scat_dscr->wr_id);
       fflush(stdout);
    }

    if(type == SERV)
        rc = ibv_post_recv((CLN_con + proc)->qp, scat_dscr, &bad_wr);
    else
        rc = ibv_post_recv((SRV_con+cluster)->qp, scat_dscr, &bad_wr);

    armci_check_status(DEBUG_SERVER,rc,"posts 1 of several rcv");

    if((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN) ){
       printf("\n%d: list_length is %d, id is %ld\n",
	      armci_me,scat_dscr->num_sge,scat_dscr->wr_id);
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
    struct ibv_sge *scat_sglist;
    struct ibv_recv_wr *scat_dscr;

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
       pend_dscr->rdescr.wr_id=DSCRID_SCATGAT + MAX_PENDING;
    }

    /*pend_dscr->proc = proc;*/
    pend_dscr->numofrecvs=0;

    scat_dscr = &pend_dscr->rdescr;
    scat_sglist = pend_dscr->sg_entry;
    /* scat_dscr->opcode = VAPI_RECEIVE; no ->opcode in ibv_recv_wr */
    /* scat_dscr->comp_type = VAPI_SIGNALED; no ->comp_type in ibv_recv_wr */
    scat_dscr->sg_list = scat_sglist;
    scat_dscr->num_sge = 0;

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


    if ((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN) ) {
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
         if(vecind == num_seg) {
           posts_scatter_desc(pend_dscr,proc,type);
           pend_dscr->numofrecvs++;

           /* the previous one has been posted, start off new*/
           scat_dscr->num_sge = 0;
           y = 0; /* reuse the same scatter descriptor */
           vecind=0;total_size=0;k++;
           if(rem_seg!=0 && k==(num_xmit-1))num_seg = rem_seg;
         }
         /* fill the scatter descriptor */
         scat_sglist[y].addr = (uint64_t)src1;
         scat_sglist[y].lkey = mhandle->lkey;
         scat_sglist[y].length = count[0];
         scat_dscr->num_sge++;
         src1 += dest_stride_arr[0];
         y++;

       }

       if(vecind == num_seg){
         posts_scatter_desc(pend_dscr,proc,type);
         pend_dscr->numofrecvs++;

         /* the previous one has been posted, start off new*/
         scat_dscr->num_sge = 0;
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
    int rc;
    int cluster = armci_clus_id(proc);
    struct ibv_send_wr *gat_dscr;
    struct ibv_send_wr *bad_wr;

    gat_dscr = &pend_dscr->sdescr;
    /*armci_vapi_print_dscr_info(gat_dscr,NULL);*/
    if((type==SERV && DEBUG_SERVER) || (type==CLN && DEBUG_CLN)){
       printf("%d: type(client=1)=%d inside posts gather dscr, id is %d\n",
	      armci_me,type,gat_dscr->wr_id);
       fflush(stdout);
    }

    rc = 0;

    if(type == CLN){
       rc = ibv_post_send((SRV_con+cluster)->qp, gat_dscr, &bad_wr);
       armci_check_status(DEBUG_CLN,rc,"client posts a gather sends");
    }
    else{
        rc = ibv_post_send((CLN_con + proc)->qp, gat_dscr, &bad_wr);
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

    struct ibv_sge *gat_sglist;
    struct ibv_send_wr *gat_dscr;

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
       pend_dscr->sdescr.wr_id=DSCRID_SCATGAT + MAX_PENDING;
    }
    pend_dscr->numofsends=0;

    gat_dscr = &pend_dscr->sdescr;
    gat_sglist = pend_dscr->sg_entry;
    gat_dscr->opcode = IBV_WR_SEND;
    gat_dscr->send_flags = IBV_SEND_SIGNALED;
    gat_dscr->sg_list = gat_sglist;
    gat_dscr->num_sge = 0;
    gat_dscr->send_flags = 0;

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
           gat_dscr->num_sge = 0;
           y = 0;
           vecind=0;total_size=0;k++;
           if(rem_seg!=0 && k==(num_xmit-1))num_seg = rem_seg;
         }

         /* fill the gather descriptor */
         gat_sglist[y].addr = (uint64_t)src1;
         gat_sglist[y].lkey = mhandle->lkey;
         gat_sglist[y].length = count[0];
         gat_dscr->num_sge++;
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
         gat_dscr->num_sge = 0;
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
                              uint32_t *lkey, uint32_t *rkey)
{
    int rc = 0;
    struct ibv_send_wr *sdscr;
    struct ibv_wc *pdscr=NULL;
    struct ibv_wc pdscr1;

    pdscr = &pdscr1;
    sdscr=&serv_buf->snd_dscr;

    if(DEBUG_SERVER){
       printf("\n%d(s):sending dir data to client %d at %p bytes=%d last=%p\n",
                armci_me,dst,dst_buf,len,(dst_buf+len-4));fflush(stdout);
    }

    memset(sdscr,0,sizeof(struct ibv_send_wr));
    armci_init_vbuf_srdma(sdscr,&serv_buf->ssg_entry,src_buf,dst_buf,len,NULL,NULL);
    sdscr->wr.rdma.rkey = *rkey;
    serv_buf->ssg_entry.lkey = *lkey;

    serv_buf->snd_dscr.wr_id = dst+armci_nproc;
    struct ibv_send_wr *bad_wr;
    rc = ibv_post_send((CLN_con+dst)->qp, &serv_buf->snd_dscr, &bad_wr);
    armci_check_status(DEBUG_SERVER, rc,"server post sent dir data to client");

    while (rc == 0)
       rc = ibv_poll_cq(CLN_nic->scq, 1, pdscr);
    armci_check_status(DEBUG_SERVER, rc,"server poll sent dir data to client");

}



void armci_send_contig_bypass(int proc, request_header_t *msginfo,
                              void *src_ptr, void *rem_ptr, int bytes)
{
    int *last;
    uint32_t *lkey=NULL;
    uint32_t *rkey;    
    int dscrlen = msginfo->dscrlen;

    last = (int*)(((char*)(src_ptr)) + (bytes - sizeof(int)));
    if(!msginfo->pinned)armci_die("armci_send_contig_bypass: not pinned",proc);

    rkey = (uint32_t *)((char *)(msginfo+1)+dscrlen-(sizeof(uint32_t)+sizeof(uint32_t)));

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
void armci_init_vapibuf_atomic(struct ibv_send_wr *sd, struct ibv_sge *sg,
                   int op, int*ploc,int *prem, int extra,
                   int id,ARMCI_MEMHDL_T *lhandle,
                   ARMCI_MEMHDL_T *rhandle)
{
    if (1) {
       printf("%d(c) : entered armci_init_vapibuf_atomic\n",armci_me);
       fflush(stdout);
    }
    memset(sd,0,sizeof(struct ibv_send_wr));
    if (op == ARMCI_FETCH_AND_ADD_LONG ) {
       printf("%d(c) :setting opcode for snd dscr to FETCH_AND_ADD\n",armci_me);
       sd->opcode = IBV_WR_ATOMIC_FETCH_AND_ADD;
       sd->wr.atomic.compare_add = (uint64_t)extra;
    } else if(op == ARMCI_SWAP_LONG){
       sd->opcode = IBV_WR_ATOMIC_CMP_AND_SWP;
       sd->wr.atomic.swap = (uint64_t)extra;
    }
    sd->send_flags = IBV_SEND_SIGNALED;
    sg->length = 8; /* 64 bit atomic*/
    printf("--------\n");
    sg->addr= (uint64_t)(void *)ploc;
    if(lhandle)
    sg->lkey = lhandle->lkey;
    sd->sg_list = sg;
    sd->num_sge = 1;
    sd->wr.atomic.remote_addr = (uint64_t)(void *)prem;
    if(rhandle)
       sd->wr.atomic.rkey = rhandle->rkey; /* how do we get the remote key  */
    sd->wr_id = DSCRID_RMW + armci_me;

    if(1){
       printf("%d(c) : finished initialising atomic send desc id is %ld,armci_ime = %d\n",armci_me,sd->wr_id,armci_me);
       fflush(stdout);
    }   
}
/*\
 *   using vapi remote atomic operations
\*/
void client_rmw_complete(struct ibv_send_wr *snd_dscr, char *from)
{
    int rc = 0;
    struct ibv_wc pdscr1;
    struct ibv_wc *pdscr=&pdscr1;

  printf("%d(c) : inside client_rmw_complete\n",armci_me);
  do {
      while (rc == 0) {
            rc =  ibv_poll_cq(CLN_nic->scq, 1, pdscr);
      }
      armci_check_status(DEBUG_CLN,rc,"rmw complete");
      rc = 0;
    } while(pdscr->wr_id != snd_dscr->wr_id);
}


void armci_direct_rmw(int op, int*ploc, int *prem, int extra, int proc,
                      ARMCI_MEMHDL_T *lhandle, ARMCI_MEMHDL_T *rhandle)
{
    int rc = 0;
    struct ibv_send_wr *sd;
    struct ibv_sge *sg;
    vapi_nic_t *nic;
    armci_connect_t *con;

    nic = SRV_nic;
    con = CLN_con+proc;

    sd = &(rmw[armci_me].rmw_dscr);
    sg = &(rmw[armci_me].rmw_entry);

    if (1) {
        printf("%d(c) : about to call armci_init_vapibuf_atomic\n",armci_me);
        fflush(stdout);
    }

  armci_init_vapibuf_atomic(sd, sg, op,ploc,prem,extra,proc,lhandle,rhandle);

  if (1) {
     printf("%d(c) : finished armci_init_vapibuf_atomic\n",armci_me);
     fflush(stdout);
  }

  struct ibv_send_wr * bad_wr;
  rc = ibv_post_send(con->qp, sd, &bad_wr);
  armci_check_status(DEBUG_CLN,rc,"client direct atomic");

  if (1) {
     printf("%d(c) : finished posting desc\n",armci_me);
     fflush(stdout);
  }

  /*armci_send_complete(sd,"send_remote_atomic");*/
  client_rmw_complete(sd,"send_remote_atomic");

  return;
}



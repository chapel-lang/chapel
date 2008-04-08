/* $Id: sr8k.c,v 1.6 2002-06-20 23:10:30 vinod Exp $
 *
 * Hitachi SR8000 specific code 
 *
 * *** WE NEED TO OPTIMIZE armcill_put/get AND armcill_put2D/get2D ******* 
 * *** latency by using TCW and combuf_kick_tcw_fast()
 * *** bandwidth by overlapping memory copy with RDMA nonblocking communication
 *
 * Optimisations performed:
 * [BPE, Hitachi, 01/11/01]
 * (0) Increase the value of MSG_BUFLEN_DBL in request.h
 * (1) Reuse tcws for the put operation
 * (2) Pipeline contiguous put with memory copy
 * (3) Pipeline contiguous get with memory copy
 * (4) Use combuf_stride_send() for strided put
 * LGET and LPUT are tunable parameters for the pipelining thresholds.
 */

/*un aligned code is debug code*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include "armcip.h"
#include "copy.h"
#include "locks.h"
#include "shmem.h"

#include <hmpp/nalloc.h>
#include <hxb/combuf.h>
#include <hxb/combuf_node.h>
#include <hxb/combuf_returns.h>
#define BUF_KEY 2020L
#define PAGE_SIZE       0x1000
#define ROUND_UP_PAGE(size)  ((size + (PAGE_SIZE-1)) & ~(PAGE_SIZE-1))
#define NOOPTION        0
#define NOFLAG          0
#define NOATOMIC        0
#define LPUT 180000
#define LGET 450000
#define FLAGSIZE 8
#define CLIENT_GETBUF_FIELDNUM 2

/* data structure used to store ptr/size/authorization info for
   every shared memory (combuf) area on every other smp node
   -- we only need it for one process on each smp node - "master"
*/
typedef struct{
    int pauth;   /* segment descriptor for send/put operation*/
    int gauth;   /* segment descriptor for get operation*/
    long size;   /* segment size */
    char* ptr;   /* address of the segment */
    int tcwd;    /* transmission control word for put operation */
}reg_auth_t;

#if defined(DATA_SERVER)
#if 0
typedef struct{
    int *cln_sm;  /*count of unacked pending client small bufs(local count)*/
    int *cln_la;  /*count of unacked pending client large bufs(local count)*/
    int *srv_la;  /*count of unacked pending server large bufs(updated by srv)*/
    int *srv_sm;  /*count of unacked pending server small bufs(updated by srv)*/
    int *syn_sm;  /*flag to see if ack already obtained for a small buffer*/
    int *syn_la;  /*flag to see if ack already obtained for a large buffer*/
    int *syn;
    int cln_dsc;
    int syn_dsc;
    int *sauth;
} pending_op_struct_t;

static pending_op_struct_t cpend;


rcv_field_info_t *client_rcv_field_info;
client_auth_t *client_auths;

int *client_ready_flag;
int *server_ready_flag; 
int sr8k_server_ready=0;

typedef struct{
    char *sm_buf[SMALL_BUFS_PER_PROCESS];/*small buf ptrs*/
    char *sm_flg[SMALL_BUFS_PER_PROCESS];/*small buf flags*/
    int sm_dsc[SMALL_BUFS_PER_PROCESS];  /*small buf field dscs*/
    char *common;     /*common client buffer(temporary)*/
    char *direct;     /*ptr to the whole client large buf*/
    char **dirflg;    /*array of directbuf(above) flags*/ 
    char **la_buf;    /*ptrs to client large bufs,(constitute direct)*/
    int *la_dsc;      /*array of descriptors to above bufs*/
    int getdsc;       /*not used*/
    char *getflg;     /*notused*/
}_sr8k_client_bufs_struct;

static _sr8k_client_bufs_struct cln_buf;
#endif
#endif
int current_clientbuf_tcwd;

static long reg_idlist[MAX_REGIONS]; /* keys for shmem segments on this node */
static int  reg_num=0;          /* number of rdma/shmem segments on this node */
static reg_auth_t *rem_reg_list;/* info about rdma segments on every node */
static reg_auth_t **rem_reg;    /* info about rdma segments on every node */
static long *reg_id;            /* buffer to exchange info about new segments */
static int  *proc_reg;          /* count of shmem/rdma segments on every node */
extern ndes_t _armci_group;     /* processor group -- set in clusterinfo */
double *armci_internal_buffer;  /* work buffer for accumulate -- in RDMA area */
static char *bufstart, *bufend; /* address range for local rdma area */
static int bufdesc,cbufdesc;    /* descriptor for local rdma buffer */

static int descs[2];            /* descriptors for get pipeline buffers */
static char *getbuf[2];         /* the get pipeline buffers themselves */

#define DEBUG0 0
#define DEBUG2 0
#define DEBUG1 0

#define ALIGNUP(x) (4*((3+x)/4))

#define BUF_TO_TCWD(bu) ((int *)((int *)bu-1))


/**TCW manipulation routines****************************************/

      /**** Routines for PUT ****/

static char *tcw_flag;

int armci_rdma_make_tcw(void *src, Cb_size_t off, int bytes, int desc, char *flag)
{
int rc;
Cb_msg msg;
int tcwd;
     memset(&msg, 0, sizeof(msg));
     msg.data.addr = src;
     msg.data.size = (Cb_size_t)bytes;
     if ( (rc=combuf_make_tcw(&msg, desc, off, flag, &tcwd)) != COMBUF_SUCCESS)
         armci_die("combuf_make_tcw failed",rc);
     if(DEBUG0){printf("%d:put dsc=%d off=%d giving tcw=%d\n",armci_me,desc,off,tcwd); fflush(stdout);}
     return tcwd;
}

void armci_rdma_modify_tcw(int tcwd, char *src, Cb_size_t off, int bytes)
{
int rc;
int modfunc = COMBUF_MOD_ADDRESS + COMBUF_MOD_SIZE + COMBUF_MOD_OFFSET;
Cb_mod_info modinfo;
     modinfo.send_addr  = src;
     modinfo.send_size  = (Cb_size_t)bytes;
     modinfo.rcv_offset = off;
     if ( (rc=combuf_modify_tcw(tcwd, modfunc, &modinfo)) != COMBUF_SUCCESS)
         armci_die("combuf_modify_tcw failed",rc);
}

void armci_rdma_modify_tcw_partial(int tcwd,int bytes){
int rc;
int modfunc =COMBUF_MOD_SIZE;
Cb_mod_info modinfo;
     modinfo.send_size  = (Cb_size_t)bytes;

     if ( (rc=combuf_modify_tcw(tcwd, modfunc, &modinfo)) != COMBUF_SUCCESS)
         armci_die("combuf_modify_tcw partial failed",rc);
}

void armci_rdma_kick_tcw_put(int tcwd)
{
    int rc ;
    if ( (rc=combuf_kick_tcw(tcwd, COMBUF_SEND_NOBLOCK)) != COMBUF_SUCCESS)
        armci_die("combuf_kick_tcw failed",rc);
}


void armci_rdma_kick_tcw_block(int tcwd)
{
    int rc ;
    if ( (rc=combuf_kick_tcw(tcwd,0)) != COMBUF_SUCCESS)
        armci_die("combuf_kick_tcw failed",rc);
}


void armci_rdma_put_wait(int tcwd, char *flag)
{
    while(!combuf_check_sendflag(flag)); /* Wait */
}


static void armci_rdma_get_wait(int desc)
{
    int rc;
    unsigned int recd;
    if ((rc=combuf_spin_wait(desc,-1,&recd)) != COMBUF_SUCCESS)
       armci_die("combuf_spin_wait failed",rc);
}


/*make these #defines*/
void create_map(int key,int size,Cb_object_t *oid,char **ptr,char *fn){
int rc;
    rc=combuf_object_get(key,(Cb_size_t)size, COMBUF_OBJECT_CREATE, oid);
    if(rc!= COMBUF_SUCCESS){
       printf("\n%d:%s:create_map get failed rc=%d\n",armci_me,fn,rc);
       fflush(stdout);armci_die("combufget buf failed",rc);
    }
    rc=combuf_map(*oid,0,(Cb_size_t)size,0,ptr);
    if(rc!= COMBUF_SUCCESS){
       printf("\n%d:%s:create_map map failed rc=%d\n",armci_me,fn,rc);
       fflush(stdout);armci_die("combuf map for buf failed",0);
    }
}


void map_attach(int create,int key,int size,Cb_object_t *oid,char **ptr,
       char *fn){
int rc;
    if(create){
       rc=(combuf_object_get(key, (Cb_size_t)size,COMBUF_OBJECT_CREATE,oid));
       if(rc!= COMBUF_SUCCESS){
         printf("\n%d:%s:map_attach get failed rc=%d\n",armci_me,fn,rc);
         fflush(stdout);armci_die("combufget buf failed",rc);
       }
       rc=combuf_map(*oid,0,(Cb_size_t)size,COMBUF_COMMON_USE,ptr);
       if(rc!= COMBUF_SUCCESS){
         printf("\n%d:%s:map_attach map failed rc=%d\n",armci_me,fn,rc);
         fflush(stdout);armci_die("combuf map for buf failed",0);
       }
    }
    else {
       rc=(combuf_object_get(key,(Cb_size_t)size,0,oid));
       if(rc!= COMBUF_SUCCESS){
         printf("\n%d:%s:map_attach get failed rc=%d\n",armci_me,fn,rc);
         fflush(stdout);armci_die("combufget buf failed",rc);
       }
       rc=combuf_map(*oid,0,(Cb_size_t)size,COMBUF_COMMON_USE,ptr);
       if(rc!= COMBUF_SUCCESS){
         printf("\n%d:%s:map_attach map failed rc=%d\n",armci_me,fn,rc);
         fflush(stdout);armci_die("combuf map for buf failed",0);
       }
    }
}
/*******************************************************************/

/*\ intialization of data structures 
 *  called by armci_register_shmem in 1st ARMCI_Malloc call in  ARMCI_Init
\*/ 
/*new temp array for client_rcv_fields should not be created here*/
/*code from client_init has to be moved here instead*/

void armci_init_sr8k()
{
    int rc;
    Cb_object_t oid,coid;
    int i, bytes = armci_nclus*MAX_REGIONS*sizeof(reg_auth_t);
    int client_bytes = armci_nproc*MAX_REGIONS*sizeof(reg_auth_t);
    long key;int size;
    Cb_opt_region options;
    double *client_buffers;
    char **ptr;
    rem_reg = (reg_auth_t**)malloc(armci_nclus*sizeof(void*));
    if(!rem_reg)armci_die("rem_reg malloc failed",0);

    rem_reg_list = (reg_auth_t*)malloc(bytes);
    if(!rem_reg_list)armci_die("rem_reg_list malloc failed",bytes);
    bzero(rem_reg_list,bytes);

    reg_id = (long*) malloc(3*armci_nclus*sizeof(long));
    if(!reg_id)armci_die("rem_id: malloc failed",0);

    proc_reg = (int*) malloc(armci_nclus*sizeof(int));
    if(!proc_reg)armci_die("proc_reg: malloc failed",0);
    bzero(proc_reg,armci_nclus*sizeof(int));
     
    /* setup pointers for each smp cluster node */
    for(i = 0; i<armci_nclus; i++)rem_reg[i] = rem_reg_list + MAX_REGIONS*i;

    /* allocate internal RDMA work buffer */
    /*printf("BUFSIZE = %d, LPUT = %d, LGET = %d\n",BUFSIZE,LPUT,LGET);*/
    if (2*LPUT > BUFSIZE) armci_die("LPUT must be <= BUFSIZE/2",0);

    /* We need space for the work buffer (BUFSIZE) and two receive
    * fields (LGET) along with a flag for each of them (FLAGSIZE) */

    bytes = ROUND_UP_PAGE(BUFSIZE+FLAGSIZE+2*(LGET+FLAGSIZE));
    key   = BUF_KEY + armci_me-armci_master;
    ptr = (char **)&armci_internal_buffer;

    create_map(key,bytes,&oid,ptr,"armci_init_sr8k"); 

    /* store the range of addresses taken by local buffer in rdma memory */
    bufstart = (char*) armci_internal_buffer;
    bufend   = (char*) armci_internal_buffer + BUFSIZE;
    tcw_flag = bufend; /* We allocated extra bytes here for the flag */

     /* Create a field for the work buffer */
    if((rc=combuf_create_field(oid, bufstart, BUFSIZE, FIELD_NUM,
         NOFLAG, NOOPTION, &bufdesc)) != COMBUF_SUCCESS)
        armci_die("combufget field failed",rc);
    /* Create two further fields for the get pipeline */
     
    getbuf[0] = bufend+FLAGSIZE;
    getbuf[1] = bufend+FLAGSIZE+LGET+FLAGSIZE;

    memset(&options,0, sizeof(options));
    options.flag.addr = getbuf[0]+LGET;   /* set up the spin_wait flag */
    options.flag.size = FLAGSIZE;
    if((rc=combuf_create_field(oid, getbuf[0], LGET, CLIENT_GETBUF_FIELDNUM,
          &options, COMBUF_CHECK_FLAG, &descs[0])) != COMBUF_SUCCESS)
        armci_die("combufget field 2 failed",rc);

    options.flag.addr = getbuf[1]+LGET;   /* set up the spin_wait flag */
    options.flag.size = FLAGSIZE;
    if( (rc=combuf_create_field(oid, getbuf[1], LGET, CLIENT_GETBUF_FIELDNUM+1,
          &options, COMBUF_CHECK_FLAG, &descs[1])) != COMBUF_SUCCESS)
        armci_die("combufget field 3 failed",rc);

    /*we now create client smallbuffers and fields for them*/
#   if defined(DATA_SERVER)
#   if 0  
    bytes = ROUND_UP_PAGE((SMALL_MSG_SIZE+FLAGSIZE)*SMALL_BUFS_PER_PROCESS);
    key = CLIENT_SMALLBUF_KEY+armci_me;	
    ptr = (char **)&client_buffers;

    create_map(key,bytes,&coid,ptr,"armci_init_sr8k"); 

    for(i=0;i<SMALL_BUFS_PER_PROCESS;i++){
       int fn=CLIENT_SMALLBUF_FIELDNUM;
       cln_buf.sm_buf[i] = (char*)client_buffers + i*SMALL_MSG_SIZE + 
                             i*FLAGSIZE;
       cln_buf.sm_flg[i] = (char*)client_buffers + (i+1)*SMALL_MSG_SIZE 
                             + i*FLAGSIZE;
       options.flag.addr = cln_buf.sm_buf[i]+SMALL_MSG_SIZE;   
       options.flag.size = FLAGSIZE;
       if((rc=combuf_create_field(coid, cln_buf.sm_buf[i], SMALL_MSG_SIZE, 
              fn+i,0,0, &cln_buf.sm_dsc[i])) != COMBUF_SUCCESS)
          armci_die("combuf create field for buf failed",rc);
    }
#   endif
#   endif
    if(DEBUG0){
       printf("%d:armci_init_sr8k initialization done\n",armci_me);
       fflush(stdout);
    }
}


/*\ registers new rdma area - called in every call to ARMCI_Malloc
\*/
void armci_register_shmem(void *my_ptr, long size, long *idlist, long off,
       void *sptr)
{
     int i=0,dst,found=0,cfound=0;
     long id = idlist[2];
     long reg_size=0;

     if(DEBUG0){
        printf("%d: registering id=%ld size=%ld\n",armci_me,id,size);
        fflush(stdout);
     }
     if(!reg_num /*&& armci_nclus<2*/) armci_init_sr8k();
     /* find if we allocated a new region, and if yes how much memory */
     if(size){
        if(reg_num>MAX_REGIONS)armci_die("error: reg_num corrupted",reg_num);
        for(i=0; i<reg_num; i++) if(reg_idlist[i]==id){
            found = 1;
            break;
        }
        if(!found){ 
          /* record new region id */
          reg_idlist[reg_num] = id; reg_num++; 
          reg_size = armci_shmem_reg_size(i,id);
          fflush(stdout);
        }
     }
     if(DEBUG0){
        printf("%d: regist id=%ld found=%d size=%ld reg_num=%d\n",
           armci_me,id,found,reg_size,reg_num);
        fflush(stdout);
     }

if(!reg_id)armci_die("problems with regid allocationg",0);
     bzero(reg_id,3*armci_nclus*sizeof(long));

     /* store id and ptr into array of longs: 
       sizeof(long) must be >=sizeof(void*)
     */
     if(armci_me==armci_master){
        if(!found && size){
           reg_id[3*armci_clus_me]=id;
#if 1
           reg_id[3*armci_clus_me+1]=(long)armci_shmem_reg_ptr(i);
#else
           reg_id[3*armci_clus_me+1]=(long)((char *)sptr-off);
#endif
           reg_id[3*armci_clus_me+2]=reg_size;
        }
        /* master processes exchange region data */
        armci_msg_gop_scope(SCOPE_MASTERS,reg_id,3*armci_nclus,"+",ARMCI_LONG); 
     }

     /* make the data available within each cluster smp node */
     armci_msg_clus_brdcst(reg_id,3*armci_nclus*sizeof(long));

	dst = armci_clus_me; 
     for(i = 0; i < armci_nclus; i++){
         void *ptr;
         long len;
         Cb_node_rt     remote;
         int rc, auth, tcwd;

         dst ++; dst %= armci_nclus;  /* select smp node */

         /* unpack region info for that smp node */
         id = reg_id[3*dst];
         ptr= (void*)reg_id[3*dst+1];
         len= reg_id[3*dst+2];
         if(!id) continue; /* nothing to register - likely we did it before */

         /* aquire authorization to send/put */
         bzero( &remote, sizeof(remote) );
         remote.type = CB_NODE_RELATIVE;
         remote.ndes = _armci_group;
         remote.node = dst;

         rc = combuf_get_sendright( (Cb_node *)&remote, sizeof(remote),
                                    id, FIELD_NUM, -1, &auth);
         if(rc != COMBUF_SUCCESS){ 
            printf("%d:failed\n",armci_me);fflush(stdout);sleep(1);
            armci_die("combuf_get_sendright:",rc);
         }

	     /* Make a generic tcw for put communication to this region.*/
	     tcwd = armci_rdma_make_tcw( bufstart, 0, LPUT, auth, tcw_flag);

         rem_reg[dst][proc_reg[dst]].pauth = auth;      
         rem_reg[dst][proc_reg[dst]].size  = len;      
         rem_reg[dst][proc_reg[dst]].ptr   = ptr;      
         rem_reg[dst][proc_reg[dst]].tcwd  = tcwd;

         /* aquire authorization to do get */
         bzero( &remote, sizeof(remote) );
         remote.type = CB_NODE_RELATIVE;
         remote.ndes = _armci_group;
         remote.node = dst;

         if(DEBUG0){
            printf("%d:register target %d\n",armci_me,dst); fflush(stdout);
         }
         rc = combuf_target((Cb_node *)&remote, sizeof(remote),id,0,-1,&auth);
         if(rc != COMBUF_SUCCESS) armci_die("combuf_target:",rc);
         rem_reg[dst][proc_reg[dst]].gauth = auth;      

         proc_reg[dst] ++; 
     }
     if(DEBUG0){printf("%d:registered id=%ld\n",armci_me,id); fflush(stdout);}
}

/*\ find shmem region corresponding to dst address
\*/

int armci_find_shmem( int node, char *ptr, Cb_size_t *off)
{
    char *ps,*pe;
    int found, i;

    found = 0;
    for(i=0; i< proc_reg[node]; i++){
       ps = rem_reg[node][i].ptr;
       pe = rem_reg[node][i].size + ps;
       if((ptr>=ps) && (ptr<pe)){
	 found=1;
	 *off = (Cb_size_t)(ptr-ps);
	 break;
       }
    }
    return(found ? i : -1);
}

/*\ basic put operation to combuf desc field at specified offset
\*/ 
static unsigned int armci_rdma_put(void *src, Cb_size_t off, int bytes, int desc)
{
int rc;
static Cb_msg msg;
unsigned int ev;
     memset(&msg, 0, sizeof(msg));
     msg.data.addr = src;
     msg.data.size = (Cb_size_t)bytes;
     rc = combuf_send(&msg, desc, off, 0, &ev);
     if (rc != COMBUF_SUCCESS) armci_die("combuf_send failed",rc);
     if(DEBUG0){
        printf("%d:put dsc=%d off=%d\n",armci_me,desc,off); 
        fflush(stdout);
     }
     return ev;
}

/*\ basic get operation from combuf desc field at specified offset
\*/
static unsigned int armci_rdma_get(Cb_size_t soff, int src_dsc, 
                                   Cb_size_t doff, int dst_dsc, int bytes)
{
int rc;
static Cb_msg msg;
unsigned int ev;

    if(DEBUG0){
       printf("%d:get s=%d d=%d bytes=%d\n",armci_me,soff,doff,bytes);
       fflush(stdout);
    }

    memset(&msg, 0, sizeof(msg));
    msg.data.addr = (void*)soff;  /* RDMA doc says to stuff offset here */
    msg.data.size = (Cb_size_t)bytes;
    rc = combuf_get(src_dsc, &msg, dst_dsc, doff, NOOPTION, NOATOMIC, &ev);
    if(rc != COMBUF_SUCCESS) armci_die("combuf_get failed",rc);
    return ev;
}

#if 0
/*\ loop to wait for ack's
\*/
#define DEBUGACK 0
int ackcount;
void _sr8k_wait_for_ack(int node){
int rc;
unsigned int recvd;
    if(!cpend.syn_la[node])
       while(cpend.srv_la[node]!=cpend.cln_la[node]){
         rc=combuf_spin_wait(cpend.cln_dsc,10,&recvd);
    }
    cpend.syn_la[node]=1;
    if(!cpend.syn_sm[node])
       while(cpend.srv_sm[node]!=cpend.cln_sm[node]){
         rc=combuf_spin_wait(cpend.cln_dsc,10,&recvd); 
       }
    cpend.syn_sm[node]=1;
}

void _sr8k_wait_smallbuf_ack(int node){
int rc;
unsigned int recvd;
    if(cpend.syn_sm[node])return;
    while(cpend.srv_sm[node]!=cpend.cln_sm[node]){
       rc=combuf_spin_wait(cpend.cln_dsc,10,&recvd);
    }
    cpend.syn_sm[node]=1;
}

void _sr8k_wait_largebuf_ack(int node){
int rc;
unsigned int recvd;
    if(cpend.syn_la[node])return;
    if(DEBUGACK)printf("\n%d:IN largebuf ack\n",armci_me);
    while(cpend.srv_la[node]!=cpend.cln_la[node]){
       rc=combuf_spin_wait(cpend.cln_dsc,10,&recvd);
    }
    cpend.syn_la[node]=1;
    if(DEBUGACK)printf("\n%d:OUT of largebuf ack.....\n",armci_me);
}
#endif

/*\ basic get operation from combuf desc field at specified offset
 *  (non-blocking)
\*/
static unsigned int armci_rdma_get_nbl(Cb_size_t soff, int src_dsc, 
                                       Cb_size_t doff, int dst_dsc, int bytes)
{
int rc;
static Cb_msg msg;
unsigned int ev;

    if(DEBUG0){
     printf("%d:get s=%d d=%d bytes=%d\n",armci_me,soff,doff,bytes);
     fflush(stdout);
    }

    memset(&msg, 0, sizeof(msg));
    msg.data.addr = (void*)soff;  /* RDMA doc says to stuff offset here */
    msg.data.size = (Cb_size_t)bytes;
    rc = combuf_get(src_dsc, &msg, dst_dsc, doff, COMBUF_GET_NOBLOCK, NOATOMIC, &ev);
    if(rc != COMBUF_SUCCESS) armci_die("combuf_get failed",rc);
    return ev;
}

/*\ Strided put operation to combuf desc field at specified offset
\*/ 
static unsigned int armci_rdma_stride_put(void *src, int bytes, int count,
                    int src_stride, Cb_size_t off, int dst_stride, int desc)
{
int rc;
static Cb_stride_msg msg;
unsigned int ev;
      memset(&msg, 0, sizeof(msg));
      msg.data.addr = src;
      msg.data.elem_size = bytes;
      msg.data.elem_num = count;
      msg.data.stridesize = src_stride;
      
      rc = combuf_stride_send(&msg, desc, off, dst_stride, 0, &ev);
      if (rc != COMBUF_SUCCESS) armci_die("combuf_stride_send failed",rc);
      return(ev);
}

/*\  contiguous put  dst(proc) = src
\*/
void armcill_put(void *src, void *dst, int bytes, int proc)
{
Cb_size_t off;
int found =0, i, node = armci_clus_id(proc);
int master = armci_clus_info[node].master;
char *ptr=(char*)dst;
int desc, tcwd;
	/*_sr8k_wait_for_ack(node);*/
    if(DEBUG0){
       printf("%d:put s=%p d=%p p=%d\n",armci_me,src,dst,proc); fflush(stdout);
    }

    /* Find shmem region corresponding to dst address */
    if ( (i = armci_find_shmem(node, ptr, &off)) >= 0 ) {
        desc = rem_reg[node][i].pauth;
	    tcwd = rem_reg[node][i].tcwd;
    }
    else 
        armci_die("armcill_put: bad dst address for p=",proc);

    if((src >= (void*)bufstart) && (src <(void*)bufend)){
       /* no need to copy or pipeline - data is in the rdma buffer */
        (void)armci_rdma_put(src, off, bytes, desc);

    } else {

      /* If bytes > LPUT/3 then pipeline sends with memory copies
       * else this just reverts to a single copy and send      */

      char *intbuf=(char*)armci_internal_buffer; 
      int len, bufpos=0;
      int dlen=LPUT;

      /* Intermediate size messages benefit from pipelining in
       * smaller chunks */
      if (bytes<3*LPUT/2 && bytes>=LPUT/2) dlen=ALIGNUP(LPUT/2);
      if (bytes<LPUT/2   && bytes>=LPUT/3) dlen=ALIGNUP(LPUT/3);

      /* Do the smallest copy first; we can hide the others */
      len = 1+(bytes-1)%dlen;

      for(i = 0; i< bytes;){

	/* send (mostly) in dlen size chunks.  dlen <= BUFSIZE/2
         * so that data are not overwritten before they are sent */

	armci_copy((char*)src, intbuf+bufpos, len);
	if (i>0) {
	  armci_rdma_put_wait(tcwd, tcw_flag);
	}
	armci_rdma_modify_tcw(tcwd, intbuf+bufpos, off, len);
	armci_rdma_kick_tcw_put(tcwd);

        i   +=len;
        off +=len;
        src = len +(char*)src;
	bufpos = (bufpos <= BUFSIZE-2*LPUT) ? bufpos+LPUT : 0;
	len = dlen; /* We know that the rest of the message
                     * is a multiple of this size */
      }
      armci_rdma_put_wait(tcwd,tcw_flag); /* wait for the last send */
    }
}


/*\  contiguous get  src = dst(proc)
\*/
void armcill_get(void *src, void *dst, int bytes, int proc)
{
Cb_size_t off, buf_off;
int found =0, i, node = armci_clus_id(proc);
int master = armci_clus_info[node].master;
char *ptr=(char*)src;
int desc;
    /*_sr8k_wait_for_ack(node);*/
    if(DEBUG0){
       printf("%d:get s=%p d=%p p=%d\n",armci_me,src,dst,proc); fflush(stdout);}

    /* find shmem region corresponding to dst address */
    if ( (i = armci_find_shmem(node, ptr, &off)) >= 0 )
        desc = rem_reg[node][i].gauth;
    else 
        armci_die("armcill_get: bad dst address for p=",proc);

    if((dst >= (void*)bufstart) && (dst <(void*)bufend)){

       /* no need to copy - this is our rdma buffer */
        buf_off = (Cb_size_t)(((char*)dst) - bufstart);
        (void)armci_rdma_get(off, desc, buf_off, bufdesc, bytes);

    } else {

        /* If bytes > LGET then pipeline gets with memory copies
         * else this just reverts to a single get and copy */

        /*
         * There doesn't seem to be an equivalent for get of
         * combuf_modify_tcw(), so we can't make the tcw reuse
         * optimisation for latency.  This has an adverse impact on
         * the efficiency of the pipelining.
         *
         * By using separate combuf fields for the get buffers we can
         * use spin_wait which is by far the fastest receive
         * confirmation method.  We can also initiate more than one
         * get at a time which helps to hide a little of the latency
         * for the pipelined transfers.
         */

	char *dstold;
	int len, lenold;
	int new=0, old=1;

        for(i = 0; i< bytes;){ 

          len = ((bytes -i)<LGET)? (bytes -i): LGET;

	  /* Get to current buffer: combuf_get() seems to be fastest way */
          armci_rdma_get_nbl(off, desc, 0, descs[new], len);

	  /* Wait, then copy from the old buffer */
	  if (i>0) {
	    armci_rdma_get_wait(descs[old]);
	    armci_copy(getbuf[old], dstold, lenold);
	  }

	  /* Save current values for later copy operation */
	  lenold = len;
	  dstold = (char*)dst;

          /* Update to new values */
          i   +=len;
          off +=len;
          dst = len +(char*)dst;

	  /* Switch to alternate descriptors and buffers */
	  old = new; new = 1-new;
	}
	armci_rdma_get_wait(descs[old]);
	armci_copy(getbuf[old], dstold, lenold);
    }
}


/*\ strided put
\*/
void armcill_put2D(int proc, int bytes, int count, void* src_ptr,int src_stride,
                                                   void* dst_ptr,int dst_stride)
{
int _j,node = armci_clus_id(proc);
char *ps=src_ptr, *pd=dst_ptr;
  /*_sr8k_wait_for_ack(node);*/
  if (2*bytes > BUFSIZE) {

    /* 
     * No more than one block will fit into the buffer, so it is
     * convenient to send each individually making use of armcill_put()
     * for pipelining.  Alternatively we could just do a number of
     * shorter strided sends, and we could even pipeline them with the
     * packing, I guess.
     * Exercise for the reader: implement this. [BPE]
     */

    for (_j = 0;  _j < count;  _j++){
      armcill_put(ps, pd, bytes, proc);
      ps += src_stride;
      pd += dst_stride;
    }    

  } else {

    /* We choose to send blocks in buffer-loads */

    Cb_size_t off;
    int node = armci_clus_id(proc);
    int desc, stride, nocopy;
    char *source;
    int i, ict, ct, dct=BUFSIZE/bytes;  /* dct is the number of elements
                                         * we can fit into the buffer */

    /* Find shmem region corresponding to dst address */
    if ( (i = armci_find_shmem(node, pd, &off)) >= 0 )
        desc = rem_reg[node][i].pauth;
    else 
        armci_die("armcill_put_2D: bad dst address for p=",proc);

    nocopy = ((ps >= bufstart) && (ps <bufend));

    ict = 0;
    while(ict<count) {
      ct = (ict+dct <= count) ? dct : count-ict;
      
      /* Copy data to send buffer if necessary */
      if(nocopy) {
	/* Data are already in rdma buffer */
	stride = src_stride;
	source = ps;
      } else {
	/* Pack into rdma buffer */
	char *ps_tmp = ps;
	char *pd_tmp = (char*)armci_internal_buffer; 
	for (_j = 0;  _j < ct;  _j++){
	  armci_copy(ps_tmp, pd_tmp, bytes);
	  ps_tmp += src_stride;
	  pd_tmp += bytes;
	}
	stride = bytes; /* Since it's now packed */
	source = (char*)armci_internal_buffer;
      }
      
      /* Send the data */
      (void)armci_rdma_stride_put(source, bytes, ct, stride, off, dst_stride, desc);

      ps  += ct*src_stride;
      off += ct*dst_stride;
      ict += ct;
    }
  }
}



/*\ strided get: source is at proc, destination on calling process 
\*/
void armcill_get2D(int proc, int bytes, int count, void* src_ptr,int src_stride,
                                                   void* dst_ptr,int dst_stride)
{
int _j;
char *ps=src_ptr, *pd=dst_ptr;
    if(DEBUG0){
       printf("%d:get s=%p d=%p p=%d\n",armci_me,src_ptr,dst_ptr,proc); fflush(stdout);}
      for (_j = 0;  _j < count;  _j++){
          armcill_get(ps, pd, bytes, proc);
          ps += src_stride;
          pd += dst_stride;
      }
    if(DEBUG0){
       printf("%d:get s=%p d=%p p=%d\n",armci_me,src_ptr,dst_ptr,proc); fflush(stdout);}
}


typedef struct {
       Cb_size_t off;
       int desc;
} sr8k_mutex_t;

static sr8k_mutex_t *_mutex_array;

#if defined(DATA_SERVER) 
#if 0
sr8k_mutex_t *_server_mutex_array;

void server_set_mutex_array(char *tmp){
int rc,size,auth,i,dst,nproc=armci_clus_info[armci_clus_me].nslave;
Cb_object_t oid;
Cb_node_rt remote;
char *temp;
    size = ROUND_UP_PAGE(sizeof(sr8k_mutex_t)*armci_nclus);
    map_attach(0,MUTEX_ARRAY_KEY,size,&oid,&temp,"server_set_mutex_array"); 

    _server_mutex_array = (sr8k_mutex_t*)temp;
    dst = armci_clus_me;
    for(i=0;i<armci_nclus;i++){	
       bzero( &remote, sizeof(remote));
       remote.type = CB_NODE_RELATIVE;
       remote.ndes = _armci_group;
       dst++;dst%=armci_nclus;
       remote.node = dst;
       rc = combuf_target((Cb_node *)&remote,sizeof(remote),1961,0,-1,&auth);
       if(rc != COMBUF_SUCCESS)armci_die("combuf_target:",rc);
       _mutex_array[dst].off  = _server_mutex_array[armci_clus_me].off; 
       _mutex_array[dst].desc = auth;
    }
}
#endif
#endif

/*\ allocate the specified number of mutexes on the current SMP node
\*/
void armcill_allocate_locks(int num)
{
int bytes = num*sizeof(int);
char *ptr;
void *myptr;
Cb_object_t oid;
int nproc = armci_clus_info[armci_clus_me].nslave;
int rc,i,size,auth,dst;
long idlist[SHMIDLEN];
Cb_node_rt remote;
    if(armci_me==armci_master){
       myptr=Create_Shared_Region(idlist+1, bytes, idlist);	
    }
    armci_msg_clus_brdcst(idlist, SHMIDLEN*sizeof(long));
    if(armci_me != armci_master){
       myptr=Attach_Shared_Region(idlist+1, bytes, idlist[0]);
    }
    #if defined(DATA_SERVER)
    if(armci_me==armci_master){			
       size = ROUND_UP_PAGE(sizeof(sr8k_mutex_t)*armci_nclus);
       map_attach(1,MUTEX_ARRAY_KEY,size,&oid,&ptr,"armcill_allocate_locks"); 
       _server_mutex_array = (sr8k_mutex_t*)ptr;
    }
    #endif
    _mutex_array = (sr8k_mutex_t *)malloc(sizeof(sr8k_mutex_t)*armci_nclus);
    if(!_mutex_array)
       armci_die("armcill_allocate_locks: malloc failed",armci_nclus);

    /*get authorization to do local swap*/
    dst = armci_clus_me;
    for(i=0;i<armci_nclus;i++){	
       bzero( &remote, sizeof(remote));
       remote.type = CB_NODE_RELATIVE;
       remote.ndes = _armci_group;
       dst++;dst%=armci_nclus;
       remote.node = dst;
       rc = combuf_target((Cb_node *)&remote,sizeof(remote),1961,0, -1, &auth);
       if(rc != COMBUF_SUCCESS)armci_die("combuf_target:",rc);
       _mutex_array[dst].off  = idlist[0];
       _mutex_array[dst].desc = auth;
       #if defined(DATA_SERVER)
       if(armci_me==armci_master){
         _server_mutex_array[dst].off  = idlist[0];
         _server_mutex_array[dst].desc = 0;
       }
       #endif

       if(_mutex_array[dst].off >armci_shmem_reg_size(0,idlist[3])) 
       armci_die2("armcill_allocate_locks:offset error",_mutex_array[i].off,armci_shmem_reg_size(0,idlist[3]));
    }
    /*replace the following with a proper barrier*/
    armci_msg_clus_brdcst(idlist, SHMIDLEN*sizeof(long));
}


/*\ lock specified mutex on node where process proc is running
\*/
void armcill_lock(int mutex, int proc)
{
int desc,node = armci_clus_id(proc);
Cb_size_t off;
    off = _mutex_array[node].off + mutex*sizeof(int);
    desc = _mutex_array[node].desc;
    while(combuf_swap(desc,off,1));
}


/*\ unlock specified mutex on node where process proc is running
\*/
void armcill_unlock(int mutex, int proc)
{
int desc,node = armci_clus_id(proc);
Cb_size_t off;
    off = _mutex_array[node].off + mutex*sizeof(int);
    desc = _mutex_array[node].desc;
    combuf_swap(desc,off,0);
}

/*code to be used for dataserver */
#if defined(DATA_SERVER) 
#if 0
void armci_init_connections(){
int size,rc;
Cb_object_t moid;
Cb_opt_region options;
int mykey = CLIENT_SERV_COMMON_CB_KEY;
char **gptr;

    armci_init_sr8k();
    size = ROUND_UP_PAGE(sizeof(rcv_field_info_t)*(armci_nproc+armci_nclus)
         +(armci_nproc+1)*sizeof(int));
    gptr = (char **)&cln_buf.common;
    if(armci_me==armci_master)
       map_attach(1,mykey,size,&moid,gptr,"armci_init_connection"); 
    armci_msg_barrier();
    if(armci_me!=armci_master)
       map_attach(0,mykey,size,&moid,gptr,"armci_init_connection"); 

    server_ready_flag = (int *)cln_buf.common;
    client_ready_flag = (int *)cln_buf.common+1;

    serv_field_info = (rcv_field_info_t *)
         ((int *)cln_buf.common+armci_nproc+1);
    client_rcv_field_info = serv_field_info + armci_nclus;	

    _armci_buf_init(); /*initialize the buffers.c buffers*/
}

static void armci_sr8k_init_counters(Cb_object_t *oid,char *cntrptr,char *flg){
Cb_opt_region options;
char *bptr; int blen; int bfieldnum,bkey, *bdsc;
int rc,i,size;
int dst;
Cb_node_rt remote;
Cb_object_t moid;
int mykey = CLIENT_COUNTERS_KEY;

    cpend.srv_la=(int *)cntrptr;
    cpend.srv_sm=(int *)(cntrptr)+armci_nproc;

    size = ROUND_UP_PAGE(sizeof(int)*(armci_nproc)*2);
    if(armci_me==armci_master)
       map_attach(1,mykey,size,&moid,&cntrptr,"sr8k_init_counters"); 
    armci_msg_barrier();
    if(armci_me!=armci_master)
       map_attach(0,mykey,size,&moid,&cntrptr,"sr8k_init_counters"); 

    cpend.syn=(int *)cntrptr;
    cpend.syn_sm=(int *)malloc(sizeof(int)*armci_nproc);
    cpend.syn_la=(int *)malloc(sizeof(int)*armci_nproc);
    cpend.sauth = (int *)calloc(sizeof(int),armci_nclus);
    cpend.cln_sm=(int *)malloc(sizeof(int)*armci_nproc);
    cpend.cln_la=(int *)malloc(sizeof(int)*armci_nproc);
    if(!cpend.cln_sm || !cpend.cln_la || !cpend.sauth)
       armci_die("malloc for one of cpend fields cln_sm cln_la sauth failed",0);

    /*create field for the server to write its counter information*/
    memset(&options, 0, sizeof(options));
    options.flag.addr = flg;
    options.flag.size = FLAGSIZE;
    bptr = (char *)cpend.srv_la;
    blen = (2*armci_nproc*sizeof(int));bfieldnum = CLIENT_PENDING_OP_FIELDNUM;
    bdsc = &cpend.cln_dsc;
    rc=combuf_create_field(*oid,bptr,(Cb_size_t)blen,bfieldnum,&options,
         COMBUF_CHECK_FLAG,bdsc);
    if(rc!= COMBUF_SUCCESS)
       armci_die("_sr8k_init_counters combuf_create_field failed",rc);
    
    /*create a field for syn_sm to ensure one pending op per node*/
    if(armci_me==armci_master){
       bptr = (char *)cpend.syn;
       blen = (2*armci_nproc*sizeof(int));
       bdsc = &cpend.syn_dsc;
       bfieldnum = CLIENT_PENDING_SYN_FIELDNUM;
       rc=combuf_create_field(moid,bptr,(Cb_size_t)blen,bfieldnum,0,0,bdsc);
       if(rc!= COMBUF_SUCCESS)
         armci_die("_sr8k_init_counters combuf_create_field failed",rc);
    }
    armci_msg_barrier();

    bzero( &remote, sizeof(remote));
    remote.type = CB_NODE_RELATIVE;
    remote.ndes = _armci_group;
    dst = armci_clus_me;
    remote.node = dst;
    blen=sizeof(remote);
    rc=combuf_target((Cb_node *)&remote,blen,mykey,0,-1,&(cpend.sauth[dst]));
    if(rc != COMBUF_SUCCESS)armci_die("combuf_target:",rc);

    for(i=0;i<armci_nproc;i++){
       cpend.cln_la[i]=0;cpend.srv_la[i]=0;
       cpend.cln_sm[i]=0;cpend.srv_sm[i]=0;
       cpend.syn_la[i]=0;cpend.syn_sm[i]=0;
                cpend.syn[i]=0;
    }
}

char *_sr8k_armci_buf_init(int size){
int rc,i,bufextsize,origsize,key;
char *bptr; int blen; int bfieldnum, *bdsc;
char **tmpptr;
Cb_object_t moid;
char *returnbuf;
Cb_opt_region options;
int counters_size=0;

    /*allocate arrays for small and large bufptrs*/
    cln_buf.dirflg = (char **)malloc(sizeof(char *)*numofbuffers);
    cln_buf.la_buf = (char **)malloc(sizeof(char *)*numofbuffers);
    cln_buf.la_dsc = (int *)malloc(sizeof(int)*numofbuffers);
    if(!cln_buf.dirflg || !cln_buf.la_buf || ! cln_buf.la_dsc)
       armci_die("_sr8k_armci_buf_init: malloc failed",0);

    origsize=size;
    counters_size = armci_nproc*2*sizeof(int);
    size=size+(numofbuffers+1)*FLAGSIZE+counters_size;
    size=ROUND_UP_PAGE(size);
    key = CLIENT_DIRECTBUF_KEY+armci_me;
    tmpptr=(char**)&cln_buf.direct;
    create_map(key,size,&moid,tmpptr,"sr8k_armci_buf_init"); 


    /*create counters and initialize them*/
    armci_sr8k_init_counters(&moid,(cln_buf.direct+origsize+(numofbuffers+1)*FLAGSIZE),(cln_buf.direct+origsize+numofbuffers*FLAGSIZE));


    /*create one large field out of client buffer to be used when doing a get*/
    memset(&options, 0, sizeof(options));
    options.flag.addr = cln_buf.direct+origsize;
    cln_buf.getflg = options.flag.addr;
    options.flag.size = FLAGSIZE;
    bptr = cln_buf.direct;blen = (MSG_BUFLEN_SMALL*numofbuffers);
    bfieldnum = CLIENT_GET_DIRECTBUF_FIELDNUM; bdsc = &cln_buf.getdsc;
    rc=combuf_create_field(moid,bptr,(Cb_size_t)blen,bfieldnum,&options,
         COMBUF_CHECK_FLAG,bdsc);
    if(rc!= COMBUF_SUCCESS)
       armci_die("_sr8k_armci_buf_init combuf_create_field-3 failed",rc);

    /*create fields for buffers used in buffers.c*/
    returnbuf = cln_buf.direct + ALIGN64ADD(cln_buf.direct);
    bufextsize = (origsize-64)/numofbuffers;

    for(i=0;i<numofbuffers;i++){
       memset(&options, 0, sizeof(options));
       options.flag.addr =  returnbuf+origsize-64+i*FLAGSIZE; 
       /*NOTusing buf_pad_t as flag for allignment issues*/
       cln_buf.dirflg[i]= options.flag.addr;
       options.flag.size = FLAGSIZE;
       bptr = (returnbuf+i*bufextsize);
       bptr+= (sizeof(BUFID_PAD_T)+sizeof(BUF_EXTRA_FIELD_T)); 
       blen = (MSG_BUFLEN_SMALL);bfieldnum=CLIENT_DIRECTBUF_FIELDNUM+armci_me+i;
       bdsc=&cln_buf.la_dsc[i];
       rc=combuf_create_field(moid,bptr,(Cb_size_t)blen,bfieldnum,0,0,bdsc);
       if(rc!= COMBUF_SUCCESS)
         armci_die("_sr8k_armci_buf_init combuf_create_fieldfailed",rc);
       *((int *)(returnbuf+i*bufextsize+sizeof(BUFID_PAD_T)))=-1;
       cln_buf.la_buf[i] = bptr;
    }	
    return(cln_buf.direct);
}



void _sr8k_armci_buf_release(void *buf){
int i;
int count;
    /*we need to reset buffers after every get*/
    if((char *)buf == cln_buf.sm_buf[0]){ 
       if(((request_header_t *)buf)->operation==GET && 
            ((request_header_t *)buf)->format==STRIDED){
         count= ((request_header_t *)buf)->bytes+sizeof(request_header_t);
         count=count/MSG_BUFLEN_SMALL+(count%MSG_BUFLEN_SMALL)?1:0;
         for(i=0;i<count;i++){
           *((int *)(cln_buf.la_buf[i]-sizeof(BUF_EXTRA_FIELD_T))) = -1;
           *((BUFID_PAD_T *)(cln_buf.la_buf[i]-sizeof(BUF_EXTRA_FIELD_T)-
               sizeof(BUFID_PAD_T))) = 0;
         }
         _armci_buf_release(cln_buf.la_buf[0]);
       } 
       else {

       }
    }
    else 
       _armci_buf_release(buf);
}

char *_sr8k_get_client_small_buf(int *tcwd,int dst){
    _sr8k_wait_smallbuf_ack(dst);
    *tcwd=client_auths[dst].small_buf_tcwd[0];
    cpend.cln_sm[dst]++;
    cpend.syn_sm[dst]=0;	
    return(cln_buf.sm_buf[0]);
}


int _sr8k_using_smallbuf;
char *_sr8k_armci_getbuf_ptr;
long _sr8k_armci_getbuf_ofs;

char *_sr8k_armci_buf_get(int size, int op,int proc){
char *tmp;	
int b_ind,dst,rc;
unsigned int recvd;
    if(size<SMALL_MSG_SIZE){
       if(proc<0)proc=-proc+SOFFSET;  
       _sr8k_using_smallbuf=1;
       tmp=_sr8k_get_client_small_buf(&current_clientbuf_tcwd,
           armci_clus_id(proc));  
    }
    else{
       tmp = _armci_buf_get(size,op,proc);
       b_ind = _sr8k_buffer_index;
       _sr8k_using_smallbuf=0;	
       if(proc<0)proc=-proc+SOFFSET;
       current_clientbuf_tcwd=client_auths[armci_clus_id(proc)].large_buf_tcwd[b_ind];
       *((int *)(tmp-sizeof(BUF_EXTRA_FIELD_T))) = current_clientbuf_tcwd;
    }	
    _sr8k_armci_getbuf_ptr=tmp;
    _sr8k_armci_getbuf_ofs=(tmp-cln_buf.direct);

    return(tmp);
}


int _sr8k_buffer_index,_sr8k_pipeget_req=0;
int buf_index=0;
int previousbuf=-1;

int armci_send_req_msg(int proc, void *buf, int bytes){
int dst,tcwd,i,rc;
unsigned int recvd;
request_header_t *msginfo=(request_header_t *)buf;

    dst = armci_clus_id(proc);
    if(msginfo->operation==ACK){
       cpend.cln_sm[dst]--;
       for(i=0;i<armci_nclus;i++){
         _sr8k_wait_for_ack(i);
       }
       return(0);
    }

    tcwd=current_clientbuf_tcwd;
    buf_index = _sr8k_buffer_index;	
    /*make sure only one outstanding process per node is sending a req*/
/* this is a local swap that makes sure there is no more than one outstanding send from all the processes in the node.*/ 
    {
    int coun=0;
    while((rc=combuf_swap(cpend.sauth[armci_clus_me],dst*sizeof(int),1+armci_me))){
      coun++;
      if(coun==1000000){
        printf("\n%d:hanging in swap %d dst=%d\n",armci_me,rc,dst);
        coun=0;
      }
    }
    }
/*    while((rc=combuf_swap(cpend.sauth[armci_clus_me],dst*sizeof(int),1+armci_me)));*/

    if(!_sr8k_pipeget_req && msginfo->operation==GET){
       _sr8k_wait_largebuf_ack(dst);
       if(!_sr8k_using_smallbuf)
         _sr8k_wait_smallbuf_ack(dst);
    }
   if((msginfo->operation==PUT||ACC(msginfo->operation))&&bytes>SMALL_MSG_SIZE){
       if(previousbuf!=-1){
         _armci_buf_ensure_one_outstanding_op_per_proc(buf,proc);
       }
       previousbuf=_sr8k_buffer_index;
       cpend.cln_la[dst]++;
       cpend.syn_la[dst]=0;
       msginfo->tag.ack=cpend.cln_la[dst];
    }
#if 1
/*This is a swap for server buffer that makes sure that no one is using the server buffer*/
ackcount=0; i=-1;
while((rc=combuf_swap(client_auths[armci_clus_id(proc)].get_auth,
((SMALL_MSG_SIZE*SMALL_MSG_NUM)+/*buf_index*/ 0*(LARGE_MSG_SIZE/numofbuffers)),armci_me+1))){
if(i==-1 || rc==0)i=rc;
ackcount++;
if(ackcount==1000000){

printf("\n%d:hanging swap1 %d value from first swap=%d dst=%d\n",armci_me,rc,i,dst);ackcount=0;
}
}
#endif
/*    while((rc=combuf_swap(client_auths[armci_clus_id(proc)].get_auth,
       ((SMALL_MSG_SIZE*SMALL_MSG_NUM)),armci_me+1)));
*/
    armci_rdma_modify_tcw_partial(tcwd,bytes);
    armci_rdma_kick_tcw_block(tcwd);
    return(0);
}


/**********poll wait functions**********/
int _sr8k_armci_wait_some =20;
double _sr8k_armci_fake_work=99.0;
long check_flag(long*buf){
    return(*buf);
}
void armci_wait_long_flag_updated(long *buf, int val)
{
long res;
long spin =0;
    res = check_flag(buf);
    while(res != (long)val){
      for(spin=0;spin<_sr8k_armci_wait_some;spin++)_sr8k_armci_fake_work+=0.001;
      res = check_flag(buf);
    }
    _sr8k_armci_fake_work =99.0;
}
void armci_wait_long_flag_updated_clear(long *buf, int val)
{
    armci_wait_long_flag_updated(buf,val);
    *buf = 0L;
}
/***************************************/

char * armci_ReadFromDirect(int proc,request_header_t *msginfo, int datalen){
long *ack;
int rc;unsigned int recvd;
    if(msginfo->operation==ACK)return((char*)msginfo);
    if((msginfo->bytes+sizeof(request_header_t))<SMALL_MSG_SIZE){
       ack=(long*)((char *)(msginfo+1)+msginfo->dscrlen+msginfo->datalen);
       rc=combuf_block_wait(cln_buf.sm_dsc[0],-1,&recvd);
       if(rc!=COMBUF_SUCCESS)armci_die("wait failed",rc);
       armci_wait_long_flag_updated_clear(ack,1);
       msginfo->datalen=datalen;
       return(cln_buf.sm_buf[0]+(sizeof(request_header_t)+msginfo->dscrlen));
    }
    else{
       rc=combuf_block_wait(cln_buf.la_dsc[_sr8k_buffer_index],-1,&recvd);
       if(rc!=COMBUF_SUCCESS)armci_die("armci_ReadFromDirect:wait fail",rc);
       ack=(long*)((char *)(msginfo+1)+msginfo->dscrlen+msginfo->datalen);
       armci_wait_long_flag_updated_clear(ack,1);	
       return((char *)(msginfo+1)+msginfo->dscrlen);
    }
}


extern char* serverbuffer;
extern int *cpendcauth;
extern int _sr8k_server_buffer_index;
void  armci_WriteToDirect(int proc,request_header_t *msginfo, void* data){
char *buf;
int auth,rc;
long *ack;
Cb_node_rt remote;
int dst=armci_clus_id(proc);
static Cb_msg msg;unsigned int ev;
    remote.type = CB_NODE_RELATIVE;
    remote.ndes = _armci_group;
    remote.node = dst;
    if(msginfo->bytes+sizeof(request_header_t)<SMALL_MSG_SIZE){
       if(msginfo->operation==ATTACH || msginfo->operation==ARMCI_SWAP
           ||msginfo->operation==ARMCI_SWAP_LONG 
           || msginfo->operation==ARMCI_FETCH_AND_ADD
           || msginfo->operation==ARMCI_FETCH_AND_ADD_LONG){
         armci_copy(data,(msginfo+1),msginfo->datalen);
         data=(msginfo+1);
       }
       ack=(long*)((char *)(data)+msginfo->datalen);
       *ack=1L;
       auth = server_auths[proc].put_auths[0];
       memset(&msg, 0, sizeof(msg));
       msg.data.addr = (char *)data;
       msg.data.size = msginfo->datalen+sizeof(long);
       rc = combuf_send(&msg,auth,(sizeof(request_header_t)+msginfo->dscrlen),
            0, &ev);
       if (rc != COMBUF_SUCCESS) 
         armci_die("armci_WriteToDirect combuf_send failed",rc);
    }
    else{
       auth = server_auths[proc].lbuf_put_auths[_sr8k_server_buffer_index-SMALL_MSG_NUM];
       ack=(long*)((char *)(data)+msginfo->datalen);
       *ack=1L;
       memset(&msg, 0, sizeof(msg));
       msg.data.addr = (char *)data;
       msg.data.size = msginfo->datalen+sizeof(long);
       rc = combuf_send(&msg,auth,(sizeof(request_header_t)+msginfo->dscrlen),
            0, &ev);
       if (rc != COMBUF_SUCCESS) 
         armci_die("armci_WriteToDirect combuf_send failed",rc);
    }
}


/*Sequence of steps
*/
void armci_client_connect_to_servers(){
int i,j,dst,field_num,auth,rc;
    client_auths = (client_auth_t *)malloc(sizeof(client_auth_t)*armci_nclus);
    if(!client_auths)
       armci_die("armci_client_connect_to_servers malloc failed",0);
    if(armci_me==armci_master){
       while(*server_ready_flag!=1)*client_ready_flag=124*432;	
       armci_msg_gop_scope(SCOPE_MASTERS,(long *)serv_field_info,
         3*armci_nclus,"+",ARMCI_LONG);
    }
    armci_msg_clus_brdcst(serv_field_info,3*armci_nclus*sizeof(long));
    sr8k_server_ready = 1;
    /*step 3a*/ /*yet to be implemented*/
    *client_ready_flag = 1;
    dst = armci_clus_me;
    for(i=0;i<armci_nclus;i++){
       Cb_node_rt remote; 
       dst++;dst%=armci_nclus;
       remote.type = CB_NODE_RELATIVE;
       remote.ndes = _armci_group;
       remote.node = dst;
       field_num=(SERV_FIELD_NUM+armci_me*SMALL_BUFS_PER_PROCESS);
       for(j=0;j<SMALL_BUFS_PER_PROCESS;j++,field_num++){
         unsigned int ev;
         static Cb_msg msg;
         rc = combuf_get_sendright( (Cb_node *)&remote, sizeof(remote),
         serv_field_info[dst].cb_key, field_num+j, -1, &auth);
         if(rc != COMBUF_SUCCESS){
           printf("%d:failed\n",armci_me);fflush(stdout);sleep(1);
           armci_die("armci_client_connect_to_server combuf_get_sendright:",rc);
         }
         client_auths[dst].small_buf_auth[j] = auth;
         /*step 5*/
         client_auths[dst].small_buf_tcwd[j] = armci_rdma_make_tcw(cln_buf.sm_buf[j],0,SMALL_MSG_SIZE,auth,cln_buf.sm_flg[j]);
       }
       field_num = SERV_FIELD_NUM_FOR_LARGE_BUF;	
       for(j=0;j<numofbuffers;j++){
         rc = combuf_get_sendright( (Cb_node *)&remote, sizeof(remote),
              serv_field_info[dst].cb_key, field_num/*+j*/, -1, &auth);
         if(rc != COMBUF_SUCCESS){
           printf("%d:failed\n",armci_me);fflush(stdout);sleep(1);
           armci_die("armci_client_connect_to_s combuf_get_sendright:",rc);
         }
         client_auths[dst].large_buf_auth[j] = auth;
         client_auths[dst].large_buf_tcwd[j] = armci_rdma_make_tcw(cln_buf.la_buf[j],0,MSG_BUFLEN_SMALL,auth,cln_buf.dirflg[j]);
       }
       /*Get authorization for doing a get from all the servers*/
       rc = combuf_target( (Cb_node *)&remote, sizeof(remote), 
            SERVER_GENBUF_KEY, 0, -1, &auth);
       if(rc != COMBUF_SUCCESS) armci_die("combuf_target:",rc);
       client_auths[dst].get_auth = auth;	
       rc = combuf_target( (Cb_node *)&remote, sizeof(remote), 
            SERVER_LARGE_GETBUF_KEY, 0,-1,&auth);
       if(rc != COMBUF_SUCCESS) armci_die("combuf_target:",rc);
       client_auths[dst].server_large_getbuf_auth=auth;
    }
}


/*code for get pipeline*/

void armci_server_direct_send(int proc, char *srcbuf,int dstoffset, 
    int bytes_ack){
char *buf;
int auth,rc;
unsigned int complet;
Cb_node_rt remote;
int dst=armci_clus_id(proc);
static Cb_msg msg;unsigned int ev;

    remote.type = CB_NODE_RELATIVE;
    remote.ndes = _armci_group;
    remote.node = dst;
    memset(&msg, 0, sizeof(msg));
    msg.data.addr = srcbuf;
    msg.data.size = bytes_ack;
    rc = combuf_send(&msg,clauth[proc], dstoffset, 0, &ev); 
    combuf_send_complete(ev,-1,&complet);
    if (rc != COMBUF_SUCCESS)
       armci_die("armci_server_direct_send combuf_send failed",rc);	
}



static void armci_pipe_advance_buf(int strides, int count[],
                                   char **buf, long **ack, int *bytes )
{
int i, extra;
    for(i=0, *bytes=1; i<=strides; i++)*bytes*=count[i]; 
    extra = ALIGN64ADD((*buf));
    (*buf) +=extra;
    *ack = (long*)((*buf) + *bytes); 
}


/*\ prepost buffers for receiving data from server (pipeline)
\*/
void armcill_pipe_post_bufs(void *ptr, int stride_arr[], int count[],
                            int strides, void* argvoid)
{
int bytes;
buf_arg_t *arg = (buf_arg_t*)argvoid;
long *ack;
    /*we want to use pipe get buf */
    if(arg->count==0){
       int dscrlen=((request_header_t*)_sr8k_armci_getbuf_ptr)->dscrlen;
       arg->buf_posted =_sr8k_armci_getbuf_ptr+sizeof(request_header_t)+dscrlen;
       ((request_header_t*)_sr8k_armci_getbuf_ptr)->tag.data_ptr=dscrlen+
         _sr8k_armci_getbuf_ptr+sizeof(request_header_t);
    }
    armci_pipe_advance_buf(strides, count, &arg->buf_posted, &ack, &bytes);

    *ack = 0L;                      /*** clear ACK flag ***/

    arg->buf_posted += bytes+sizeof(long);/* advance pointer for next chunk */
    arg->count++;
}


void armcill_pipe_extract_data(void *ptr, int stride_arr[], int count[],
                               int strides, void* argvoid)
{
int bytes,rc; unsigned int rcvd;
long *ack;
buf_arg_t *arg = (buf_arg_t*)argvoid;

    if(!arg->count) {
       int dscrlen=((request_header_t*)_sr8k_armci_getbuf_ptr)->dscrlen;
       arg->buf_posted =_sr8k_armci_getbuf_ptr+sizeof(request_header_t)+dscrlen;
    }

    armci_pipe_advance_buf(strides, count, &arg->buf_posted, &ack, &bytes);
    armci_wait_long_flag_updated(ack, 1); 

    /* copy data to the user buffer identified by ptr */
    armci_read_strided(ptr, strides, stride_arr, count, arg->buf_posted);
 
    arg->buf_posted += bytes+sizeof(long);/* advance pointer for next chunk */
    arg->count++;
}
 
static int dst_offset; 

void armcill_pipe_send_chunk(void *data, int stride_arr[], int count[],
                             int strides, void* argvoid){
int bytes, bytes_ack;
buf_arg_t *arg = (buf_arg_t*)argvoid;
long *ack;
    if(!arg->count){
       /*for the first send, reset the destination offset*/
       dst_offset = _sr8k_armci_getbuf_ofs;
    }
    dst_offset+=ALIGN64ADD(arg->buf_posted);
    armci_pipe_advance_buf(strides, count, &arg->buf_posted, &ack, &bytes);
    armci_pipe_advance_buf(strides, count, &arg->buf, &ack, &bytes);
    bytes_ack = bytes+sizeof(long);

     /* copy data to buffer */
    armci_write_strided(data, strides, stride_arr, count, arg->buf);
    /*set the ack to one, destination polls on this*/
    *ack=1;

    armci_server_direct_send(arg->proc, arg->buf, dst_offset, bytes_ack);

    arg->buf += bytes+sizeof(long);        /* advance pointer for next chunk */
    arg->buf_posted += bytes+sizeof(long); /* advance pointer for next chunk */
    dst_offset += bytes+sizeof(long);      /* advance destination offser*/
    arg->count++;
}

void armci_pipe_send_req(int proc, void *vbuf, int len)
{
int rc,k=0;
unsigned int recvd;
int dst=armci_clus_id(proc);
int dscrlen;
    dscrlen = ((request_header_t*)vbuf)->dscrlen;
    dscrlen += sizeof(request_header_t);
    ((request_header_t*)vbuf)->tag.data_ptr = _sr8k_armci_getbuf_ptr+dscrlen;
    ((request_header_t*)vbuf)->tag.ack=_sr8k_armci_getbuf_ofs+dscrlen;
    if(_sr8k_using_smallbuf)k=-1;

    _sr8k_wait_largebuf_ack(dst);
    while(cpend.srv_sm[dst]!=(cpend.cln_sm[dst]+k)){
       rc=combuf_spin_wait(cpend.cln_dsc,10,&recvd);
    }
    rc=-1;

    if(!_sr8k_using_smallbuf)
       _sr8k_pipeget_req=1;
    armci_send_req_msg(proc, vbuf, len);
    _sr8k_pipeget_req=0;
}

#endif 

#endif 
/*to be used for dataserver*/

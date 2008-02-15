/* $Id: myrinet.c,v 1.78.2.3 2006/09/13 17:58:03 andriy Exp $
 * DISCLAIMER
 *
 * This material was prepared as an account of work sponsored by an
 * agency of the United States Government.  Neither the United States
 * Government nor the United States Department of Energy, nor Battelle,
 * nor any of their employees, MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR
 * ASSUMES ANY LEGAL LIABILITY OR RESPONSIBILITY FOR THE ACCURACY,
 * COMPLETENESS, OR USEFULNESS OF ANY INFORMATION, APPARATUS, PRODUCT,
 * SOFTWARE, OR PROCESS DISCLOSED, OR REPRESENTS THAT ITS USE WOULD NOT
 * INFRINGE PRIVATELY OWNED RIGHTS.
 *
 *
 * ACKNOWLEDGMENT
 *
 * This software and its documentation were produced with United States
 * Government support under Contract Number DE-AC06-76RLO-1830 awarded by
 * the United States Department of Energy.  The United States Government
 * retains a paid-up non-exclusive, irrevocable worldwide license to
 * reproduce, prepare derivative works, perform publicly and display
 * publicly by or for the US Government, including the right to
 * distribute to other US Government contractors.
 *
 * History: 
 * 03/00, Jialin: initial version
 * 9/8/00, Jarek: added armci_gm_server_ready to fix timing problems at startup
 * 10/12/00 jarek: fixed unititialized context variable -hangs
 *                 changed context allocation to static
 * 10/00    jarek: support for overlaping client pinning with memcopy at server
 *
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/mman.h>

#include "myrinet.h"
/*
#define GM_STRONG_TYPES 0
#include "gm.h"
*/
#include "armcip.h"
#include "request.h"
#include "copy.h"

#define DEBUG_ 0
#define DEBUG2 0
#define DEBUG_INIT_ 0
#define DEBUG_NOTIFY 0
#define STATIC_PORTS__

#define FALSE  0
#define TRUE   1
#define TAG_DFLT  0
#define ARMCI_GM_SENT    1
#define ARMCI_GM_READY     1

#define ARMCI_GM_MIN_MESG_SIZE 5
#define SHORT_MSGLEN 400000
#define CLIENT_STAMP 101
#define SERV_STAMP 99
/*static char * client_tail;*/
static char * serv_tail;

static int server_can_poll=0;

#if 0
extern void armci_buf_free(char *ap);
extern void armci_init_buf_alloc(size_t len, void* buffer);
#endif

/*the structures below are for fence and should go to a common place later */
typedef int ops_t;
ops_t** armci_gm_fence_arr;

/*the structures below are for verify-wait and should go to a common place*/
typedef struct{
    int *verify_seq_ar;
    int *wait_seq_ar;
    int** recv_verify_arr;
    int** recv_verify_smp_arr;
}armci_verify_wait_t;
armci_verify_wait_t __armci_verify_wait_struct;
armci_verify_wait_t *verify_wait = &__armci_verify_wait_struct;
armci_gm_context_t **notify_gmc_arr;
/***************/

/* data structure of computing process */
typedef struct {
    unsigned int node_id;    /* my node id */
    int *node_map;           /* other's node id */
    int *port_map;           /* servers port to send request to */
    struct gm_port *port;    /* my port */
    long *ack;               /* acknowledgment for server */
    long *tmp;
    int *itmp;               /*sending long as int on 64 bit pltfrms not safe*/
    long **serv_ack_ptr;     /* keep the pointers of server ack buffer */
    ops_t *ops_pending_ar;   /*for fence*/
    ops_t *rdmaop_pending_ar;/*for fence*/
} armci_gm_proc_t;

/* data structure of server thread */
typedef struct {
    int node_id;            /* my node id */
    int port_id;            /* my port id */
    int *node_map;          /* other's node id */
    int *port_map;          /* other's port id. server only */
    struct gm_port *rcv_port;   /* server receive port */
    struct gm_port *snd_port;   /* server receive port */
    void **dma_buf;         /* dma memory for receive */
    long *ack;              /* ack for each computing process */
    long *direct_ack;
    long *proc_buf_ptr;     /* keep the pointers of client MessageSndBuffer */
    long *proc_ack_ptr;
    ops_t *ops_done_ar;     /*for fence*/
    ops_t *ops_done_ack;    /*for fence*/
    unsigned long pending_msg_ct;
    unsigned long complete_msg_ct;
} armci_gm_serv_t;

#define BUF_TO_EVBUF(buf) ((armci_gm_context_t *)(((char*)buf) - sizeof(armci_gm_context_t))) 
/***************/
typedef struct {
    int port_id;
    long *ack;
} armci_gm_client_init_t;
 
armci_gm_client_init_t *client_init_struct;
armci_gm_client_init_t *server_init_struct;

extern struct gm_port *gmpi_gm_port; /* the port that mpi currently using */

armci_gm_proc_t __armci_proc_gm_struct;
armci_gm_proc_t *proc_gm = &__armci_proc_gm_struct;

armci_gm_context_t __armci_gm_client_context_struct = {0, ARMCI_GM_CLEAR};
armci_gm_context_t *armci_gm_client_context = &__armci_gm_client_context_struct;

armci_gm_context_t __armci_gm_serv_context_struct = {0, ARMCI_GM_CLEAR};
armci_gm_context_t *armci_gm_serv_context = &__armci_gm_serv_context_struct;

armci_gm_context_t __armci_gm_serv_ack_context_struct = {0, ARMCI_GM_CLEAR};
armci_gm_context_t *armci_gm_serv_ack_context = &__armci_gm_serv_ack_context_struct;

armci_gm_context_t __armci_gm_serv_pendingop_context_struct = {0, ARMCI_GM_CLEAR};
armci_gm_context_t *armci_gm_serv_pendingop_context = &__armci_gm_serv_pendingop_context_struct;

armci_gm_serv_t __armci_serv_gm_struct;
armci_gm_serv_t *serv_gm = &__armci_serv_gm_struct;

char *MessageSndBuffer;
char *MessageRcvBuffer;

int _armci_bypass = 0;
static int armci_gm_server_ready = 0;
static int armci_gm_num_send_tokens=0;
static int armci_gm_num_receive_tokens=0;

GM_ENTRY_POINT char * _gm_get_kernel_build_id(struct gm_port *p);
/*\
 * function to get the next available context for non-blocking RDMA. We limit
 * the number of uncompleted non-blocking RDMA sends to 8.
\*/
#define MAX_PENDING 8
static armci_gm_context_t armci_gm_nbcontext_array[MAX_PENDING];
armci_gm_context_t *armci_gm_get_next_context(int nbtag)
{
static int avail=-1;
armci_gm_context_t *retcontext;
    if(avail==-1){
       int i;
       for(i=0;i<MAX_PENDING;i++){
         armci_gm_nbcontext_array[i].tag=0;
         armci_gm_nbcontext_array[i].done = ARMCI_GM_CLEAR;
       }
       avail=0;
    }
    if(armci_gm_nbcontext_array[avail].tag!=0){
       armci_client_send_complete(armci_gm_nbcontext_array+avail,"armci_gm_get_next_context");
    }
    armci_gm_nbcontext_array[avail].tag=nbtag;
    retcontext= (armci_gm_nbcontext_array+avail);
    avail = (avail+1)%MAX_PENDING;
    return(retcontext);
}

/*global variables set from rcv_req to be used by server_ack (....fence) */
int armci_gm_req_to=0,armci_gm_req_from=0,armci_gm_req_op=0;

void armci_gm_set_tofromop(int to,int from,int op)
{
    armci_gm_req_to=to;
    armci_gm_req_from=from;
    armci_gm_req_op=op;
}

/*********************************************************************
                        UTILITY FUNCTIONS                            
 *********************************************************************/

int __armci_wait_some =20;
double __armci_fake_work=99.0;
extern long check_flag(long*);

/* check memory */
void armci_wait_long_flag_updated(long *buf, int val)
{
    long res;
    long spin =0;

    res = check_flag(buf);
    while(res != (long)val){
       for(spin=0; spin<__armci_wait_some; spin++)__armci_fake_work+=0.001;
       res = check_flag(buf);
    }
    __armci_fake_work =99.0;
}

void armci_wait_long_flag_updated_clear(long *buf, int val)
{
    armci_wait_long_flag_updated(buf,val);
    *buf = ARMCI_GM_CLEAR;
}

/*\ wait until flag is not ARMCI_GM_CLEAR and return its value
\*/
long armci_wait_long_flag_not_clear(long *buf)
{
    long res;
    long spin =0;

    res = check_flag(buf);
    while(res == ARMCI_GM_CLEAR){
       for(spin=0; spin<__armci_wait_some; spin++)__armci_fake_work+=0.001;
       res = check_flag(buf);
    }
/*    *buf = ARMCI_GM_CLEAR; */
    __armci_fake_work =99.0;
    return res;
}

static void armci_ensure_req_complete(request_header_t* msginfo){
# if defined(BUF_EXTRA_FIELD_T)
    BUF_EXTRA_FIELD_T *tmp;
    tmp = ((BUF_EXTRA_FIELD_T *)((BUF_EXTRA_FIELD_T *)msginfo-1));
    CLEAR_SEND_BUF_FIELD(*tmp,0,0,0,0);
# endif
}

static int pin_in_block;   /* indicate pin memory in one large block or not */
static int pin_in_segment; /* when pining segment by segment, serves as
                            * counter how many segments have been pinned so far
                            */
int armci_pin_contigs(void *ptr, int bytes)
{
    gm_status_t status;
    struct gm_port *port;
    port = serv_gm->rcv_port;
    status = gm_register_memory(port, (char *)ptr, bytes);
    if(status == GM_SUCCESS) return TRUE;
    printf("%d(s):  pinning failed %d\n",armci_me, bytes);
    fflush(stdout);

    return FALSE;
}
int armci_pin_contig(void *ptr, int bytes)
{
    gm_status_t status;
    struct gm_port *port;
    if(SERVER_CONTEXT) port = serv_gm->snd_port;
    else port = proc_gm->port;
    status = gm_register_memory(port, (char *)ptr, bytes);
    if(status == GM_SUCCESS) return TRUE;
    printf("%d:  pinning failed %d status=%d\n",armci_me, bytes,status);
    fflush(stdout);

    return FALSE;
}

static int pin_error(char *port, int bytes, void *ptr,  gm_status_t status)
{
#if 0
    printf("%d: pinning (%p,%d) on port %s failed status=%d\n",armci_me, ptr, bytes,
           port, status);
    fflush(stdout);
#endif
    return FALSE;
}

int armci_pin_contig1(void *ptr, int bytes)
{
    gm_status_t status;

    status = gm_register_memory(proc_gm->port, (char *)ptr, bytes);
    if(DEBUG_){
       printf("%d:  pinning %p %d status=%d\n",armci_me, ptr, bytes,status);
       fflush(stdout);
    }
    if(status != GM_SUCCESS) return pin_error("CLN REQ",bytes,ptr,status);

    if(armci_me == armci_master){
       /* prepare for direct put by client */
       status = gm_register_memory(serv_gm->rcv_port, (char *)ptr, bytes);
       if(DEBUG_){
          printf("%d: pinning sr %p %d status=%d\n",armci_me, ptr,bytes,status);
          fflush(stdout);
       }
       if(status != GM_SUCCESS) return pin_error("SRV RCV",bytes,ptr,status);

       /* prepare for get reply by server */
       status = gm_register_memory(serv_gm->snd_port, (char *)ptr, bytes);
       if(DEBUG_){
          printf("%d: pinning ss %p %d status=%d\n",armci_me, ptr,bytes,status);
          fflush(stdout);
       }
       if(status != GM_SUCCESS) return pin_error("SRV SND",bytes,ptr,status);
    }
    return TRUE;
}


void armci_unpin_contig(void *ptr, int bytes)
{
    gm_status_t status;
    struct gm_port *port;

    if(SERVER_CONTEXT) port = serv_gm->snd_port;
    else port = proc_gm->port;

    status = gm_deregister_memory(port, (char *)ptr, bytes);
    if(status != GM_SUCCESS)
       armci_die(" unpinning cont memory failed", armci_me);
}


int armci_pin_memory(void *ptr, int stride_arr[], int count[], int strides)
{
    int i, j;
    long idx;
    int n1dim;  /* number of 1 dim block */
    int bvalue[MAX_STRIDE_LEVEL], bunit[MAX_STRIDE_LEVEL];
    gm_status_t status;
    struct gm_port *port;

    if(SERVER_CONTEXT) port = serv_gm->snd_port;
    else port = proc_gm->port;
    
    if(strides ==0){
       if(gm_register_memory(port, (char *)ptr, count[0])==GM_SUCCESS)
          return TRUE;
       else return FALSE;
    }

    if(count[0] == stride_arr[0]){
       int sizes = 1;
       for(i=0; i<strides; i++) sizes *= stride_arr[i];
       sizes *= count[strides];
        
       status = gm_register_memory(port, (char *)ptr, sizes);
       if(status == GM_SUCCESS) { pin_in_block = TRUE; return TRUE; }
    }

    pin_in_block = FALSE;
    pin_in_segment = 0;  /* set counter to zero */
    
    /* if can pin memory in one piece, pin it segment by segment */
    n1dim = 1;
    for(i=1; i<=strides; i++) n1dim *= count[i];

    /* calculate the destination indices */
    bvalue[0] = 0; bvalue[1] = 0; bunit[0] = 1; bunit[1] = 1;
    for(i=2; i<=strides; i++) {
        bvalue[i] = 0; bunit[i] = bunit[i-1] * count[i-1];
    }

    for(i=0; i<n1dim; i++) {
        idx = 0;
        for(j=1; j<=strides; j++) {
            idx += bvalue[j] * stride_arr[j-1];
            if((i+1) % bunit[j] == 0) bvalue[j]++;
            if(bvalue[j] > (count[j]-1)) bvalue[j] = 0;
        }

        status = gm_register_memory(port, (char *)ptr+idx, count[0]);
        if(status != GM_SUCCESS) {
            armci_unpin_memory(ptr, stride_arr, count, strides);
            printf("%d: strided pinning 2 failed %d\n",armci_me, count[0]);
            fflush(stdout);
            return FALSE;
        }
        pin_in_segment++;
    }

    return TRUE;
}


void armci_unpin_memory(void *ptr, int stride_arr[], int count[], int strides)
{
    int i, j, sizes;
    long idx;
    int n1dim;  /* number of 1 dim block */
    int bvalue[MAX_STRIDE_LEVEL], bunit[MAX_STRIDE_LEVEL]; 
    gm_status_t status;
    struct gm_port *port;

    if(SERVER_CONTEXT) port = serv_gm->snd_port;
    else port = proc_gm->port;

    if(strides ==0){
       if(gm_deregister_memory(port, (char *)ptr, count[0])!=GM_SUCCESS)
            armci_die(" unpinning memory failed", armci_me);
    }
   
    if(pin_in_block) {
        sizes = 1;
        for(i=0; i<strides; i++) sizes *= stride_arr[i];
        sizes *= count[strides];
        
#if 1
        status = gm_deregister_memory(port, (char *)ptr, sizes);
        if(status != GM_SUCCESS)
            armci_die(" unpinning memory failed", armci_me);
#else
        status = munlock((char *)ptr, sizes);
        if(!status)  return; 
        else armci_die("unpin failed",sizes);
#endif

    } else {
        
        /* if can unpin memory in one piece, unpin it segment by segment */
        n1dim = 1;
        for(i=1; i<=strides; i++) n1dim *= count[i];
        
        /* calculate the destination indices */
        bvalue[0] = 0; bvalue[1] = 0; bunit[0] = 1; bunit[1] = 1;
        for(i=2; i<=strides; i++) {
            bvalue[i] = 0; bunit[i] = bunit[i-1] * count[i-1];
        }
        
        for(i=0; i<n1dim; i++) {
            idx = 0;
            for(j=1; j<=strides; j++) {
                idx += bvalue[j] * stride_arr[j-1];
                if((i+1) % bunit[j] == 0) bvalue[j]++;
                if(bvalue[j] > (count[j]-1)) bvalue[j] = 0;
            }

            if(pin_in_segment > 0) {
                status = gm_deregister_memory(port, (char *)ptr+idx, count[0]);
                if(status != GM_SUCCESS)
                    armci_die(" unpinning memory failed", armci_me);
                pin_in_segment--;
            }
        }
    }
}

/*********************************************************************
                           COMPUTING PROCESS                            
 *********************************************************************/

/* pre-allocate required memory at start up*/
int armci_gm_client_mem_alloc()
{
char *tmp;
/*int extra=256;*/

    /* allocate buf keeping the pointers of server ack buf */
    proc_gm->serv_ack_ptr = (long **)calloc(armci_nclus, sizeof(long*));
    if(!proc_gm->serv_ack_ptr) return FALSE;

    /* initialize buffer managment module */
    _armci_buf_init();

    tmp = gm_dma_malloc(proc_gm->port, (2*sizeof(long)+3*sizeof(int)));
    if(!tmp)return FALSE;
    
    proc_gm->ack = (long*)tmp;
    proc_gm->tmp = (long*)(tmp + sizeof(long)); 
    proc_gm->itmp = (int*)(tmp + 2*sizeof(long)); 
    
    return TRUE;
}

/* deallocate the preallocated memory used by gm */
int armci_gm_proc_mem_free()
{
    free(proc_gm->serv_ack_ptr);

    gm_dma_free(proc_gm->port, proc_gm->ack);
        
    return TRUE;
}

/*seperate function required to init fence and verify arrays */
/*should later be made a common function*/
void armci_gm_fence_init()
{
    int i;
    armci_gm_fence_arr = (ops_t**)malloc(armci_nproc*sizeof(ops_t*));
    bzero(armci_gm_fence_arr,armci_nproc*sizeof(ops_t*));
    armci_gm_fence_arr[armci_me]=(ops_t *)malloc(armci_nproc*sizeof(ops_t)); 
    armci_exchange_address((void **)armci_gm_fence_arr,armci_nproc);
    /*the fence array, has to be pinned for server to be able to put*/
    /*armci_gm_fence_arr = (ops_t**)malloc(armci_nproc*sizeof(ops_t*));
    if(!armci_gm_fence_arr)armci_die("malloc failed for ARMCI fence array",0);
    if(ARMCI_Malloc((void**)armci_gm_fence_arr, armci_nclus*sizeof(ops_t)))
             armci_die("failed to allocate ARMCI fence array",0);
    printf("\n%d:successfully malloc'd shared memory\n",armci_me);*/
    

/****************************verify-wait code*******************************/
    /*allocate an array for verify sequence array */
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
    bzero(verify_wait->recv_verify_arr ,armci_nproc*sizeof(ops_t*));
#if 0
    verify_wait->recv_verify_arr[armci_me]=
	    (int *)malloc(armci_nproc*2*sizeof(int));
    armci_exchange_address((void **)verify_wait->recv_verify_arr ,armci_nproc);

#else
    if(ARMCI_Malloc((void**)verify_wait->recv_verify_arr, armci_nproc*2*sizeof(int)))
             armci_die("failed to allocate ARMCI fence array",0);
#endif
    if(ARMCI_Malloc((void**)verify_wait->recv_verify_smp_arr, 
                    armci_nproc*sizeof(int)))
       armci_die("failed to allocate ARMCI fence array",0);
    /*create and initialize some nonblocking descriptors for doing notify*/
#if 0
    notify_nbdscr_arr = malloc(sizeof(armci_hdl_t)*armci_nproc);
    for(i=0;i<armci_nproc;i++){
      ARMCI_INIT_HANDLE(notify_nbdscr_arr[i]);
      INIT_NB_HANDLE((notify_nbdscr_arr+i),PUT,i);
      (notify_nbdscr_arr+i)->op  = PUT;
      (notify_nbdscr_arr+i)->proc= proc;
      (notify_nbdscr_arr+i)->bufid=NB_NONE;
    }
#endif
    notify_gmc_arr = ARMCI_Malloc_local(sizeof(armci_gm_context_t *)*armci_nproc);
    bzero(notify_gmc_arr,sizeof(armci_gm_context_t *)*armci_nproc);

/************************End verify-wait code*******************************/
    /*printf("\n%d:in fence init",armci_me);fflush(stdout);*/

}

/* initialization of client process/thread */
int armci_gm_client_init()
{
    int status,i,intcount;
    client_init_struct = (armci_gm_client_init_t *)(calloc(armci_nclus, sizeof(armci_gm_client_init_t))); 
    intcount = armci_nclus *sizeof(armci_gm_client_init_t)/sizeof(int);
    /* allocate gm data structure for computing process */
    proc_gm->node_map = (int *)calloc(armci_nproc, sizeof(int));
    if(!proc_gm->node_map) armci_die("Error allocating proc data structure",0);
 
    proc_gm->port_map = (int*)calloc(armci_nclus, sizeof(int));
    if(!proc_gm->port_map)armci_die("error allocating serv port map",0);
 
    /* use existing MPI port to save ports */
    proc_gm->port = gmpi_gm_port;
 
    /* get my node id */
    status = gm_get_node_id(proc_gm->port, &(proc_gm->node_id));
    if(status != GM_SUCCESS)armci_die("Could not get GM node id",0);
    if(DEBUG_INIT_) printf("%d: node id is %d\n", armci_me, proc_gm->node_id);
    /* broadcast my node id to other processes */

#ifdef GM2
    /* has to convert GM local node id to the global one */
    status = gm_node_id_to_global_id (proc_gm->port,proc_gm->node_id,
                                      proc_gm->node_map+armci_me);
    if(status != GM_SUCCESS)armci_die("Could not convert GM local node id",0);  
#else
    proc_gm->node_map[armci_me] = proc_gm->node_id;
#endif

    armci_msg_igop(proc_gm->node_map, armci_nproc, "+");

#ifdef GM2
    /* has to convert received global node ids to local ids */
    for(i=0; i<armci_nproc; i++){
        status = gm_global_id_to_node_id (proc_gm->port,proc_gm->node_map[i],
                                                        proc_gm->node_map+i);
        if(status != GM_SUCCESS)armci_die("Could not convert GM node id",i);
    }
#endif
 
    if(armci_me==armci_master){
       for(i=0; i<armci_nproc; i++) serv_gm->node_map[i] = proc_gm->node_map[i];
       client_init_struct[armci_clus_me].ack = (long *)serv_gm->ack;
       /* publish port id of local server thread to other smp nodes */
       client_init_struct[armci_clus_me].port_id =  serv_gm->port_id;
       armci_msg_gop_scope(SCOPE_MASTERS,(int *)(client_init_struct), intcount,
                           "+", ARMCI_INT);
    }
 
    /* master makes port ids of server processes available to other tasks */
    if(armci_nclus*sizeof(armci_gm_client_init_t)%sizeof(int)!=0)
       armci_die("allignment problem in client init",
                  sizeof(armci_gm_client_init_t));
    armci_msg_bcast_scope(SCOPE_NODE,client_init_struct,intcount*sizeof(int),
                          armci_master);
    for(i=0;i<armci_nclus;i++){
        proc_gm->port_map[i] = client_init_struct[i].port_id;
        if(DEBUG_){
          printf("%d: portmap for me \n%d = %d   %d ",armci_me,
                  client_init_struct[i].port_id, proc_gm->port_map[i],i);
          fflush(stdout);
        }
    }
 

    /* allow direct send */
    status = gm_allow_remote_memory_access(proc_gm->port);
    if(status != GM_SUCCESS) armci_die("could not enable direct sends",0);
 
    /* memory preallocation for computing process */
    if(!armci_gm_client_mem_alloc()) armci_die(" client mem alloc failed ",0);
 
    server_init_struct = (armci_gm_client_init_t *)(calloc(armci_nproc, sizeof(armci_gm_client_init_t)));    
    server_init_struct[armci_me].port_id = gm_get_port_id(proc_gm->port);
    server_init_struct[armci_me].ack = proc_gm->ack;
    intcount = armci_nproc*sizeof(armci_gm_client_init_t)/sizeof(int);
    armci_msg_igop((int*)(server_init_struct), intcount, "+");
    /* query GM for number of tokens available */
    armci_gm_num_receive_tokens = gm_num_receive_tokens(proc_gm->port);
    armci_gm_num_send_tokens = gm_num_send_tokens(proc_gm->port);
    if(DEBUG_ && armci_me==0){
        printf("has %d send %d receive tokens\n",armci_gm_num_send_tokens,
           armci_gm_num_receive_tokens); fflush(stdout);
    }
 
#ifdef CLIENT_BUF_BYPASS
    /* get the gm version number and set bypass flag: need GM >1.1 */
    if(armci_me == 0) {
        char gm_version[8];
        strncpy(gm_version, _gm_get_kernel_build_id(proc_gm->port), 3);
        /*printf("GM version is %s\n",gm_version);*/
        gm_version[3] = '\0';
        if(strcmp(gm_version, "1.0") == 0) _armci_bypass = FALSE;
        else if(strcmp(gm_version, "1.1") == 0) _armci_bypass = FALSE;
        else _armci_bypass = TRUE;
    }
    armci_msg_brdcst(&_armci_bypass, sizeof(int), 0);
#endif

    /*allocate an array for pending operation count on client side*/
    /*the fence array, has to be pinned for server to be able to put*/
    if(!(proc_gm->ops_pending_ar=(ops_t*)calloc(armci_nclus,sizeof(ops_t))))
        armci_die("malloc failed for ARMCI ops_pending_ar",0);
    if(!(proc_gm->rdmaop_pending_ar=(ops_t*)calloc(armci_nclus,sizeof(ops_t))))
        armci_die("malloc failed for ARMCI rdmaop_pending_ar",0);

    armci_pin_contig(armci_gm_fence_arr[armci_me],armci_nclus*sizeof(ops_t));
    bzero(armci_gm_fence_arr[armci_me],armci_nclus*sizeof(ops_t));

    armci_pin_contig1(verify_wait->recv_verify_arr[armci_me],
		    armci_nproc*2*sizeof(int));
    bzero(verify_wait->recv_verify_arr[armci_me],armci_nproc*2*sizeof(int));
    return TRUE;
}


/* callback func of gm_send_with_callback */
void armci_client_send_callback(struct gm_port *port, void *context,gm_status_t status)
{
    if(status==GM_SUCCESS){
         ((armci_gm_context_t*)context)->done = ARMCI_GM_CLEAR;
    }
    else ((armci_gm_context_t *)context)->done = ARMCI_GM_FAILED;
}


/* callback func of gm_send_directed */
void armci_client_send_callback_direct(struct gm_port *port, void *context,gm_status_t status)
{
    if(status==GM_SUCCESS)((armci_gm_context_t*)context)->done = ARMCI_GM_CLEAR;
    else ((armci_gm_context_t *)context)->done = ARMCI_GM_FAILED;
}

/* client trigers gm_unknown, so that callback func can be executed */
void armci_client_send_complete(armci_gm_context_t* context,char *st)
{
    MPI_Status status;
    int flag;
    
    /* blocking: wait til the send is done by calling the callback */
    while(context->done == ARMCI_GM_SENDING) 
        MPI_Iprobe(armci_me, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    if(context->done == ARMCI_GM_FAILED){
       printf("\n%d:armci_client_send_complete called from %s\n",armci_me,st);
       fflush(stdout);
       armci_die("armci_client_send_complete: failed code=",context->done);
    }
    MEM_FENCE;
}

void armci_client_to_client_direct_send(int p, void *src_buf, void *dst_buf, int len)
{
    int s           = armci_clus_id(p);
    int serv_mpi_id = armci_clus_info[s].master;

    armci_client_send_complete(armci_gm_client_context,"armci_client_to_client_direct_send");
    armci_gm_client_context->done = ARMCI_GM_SENDING;
    gm_directed_send_with_callback(proc_gm->port, src_buf,
               (gm_remote_ptr_t)(gm_up_t)dst_buf, len, GM_LOW_PRIORITY,
                proc_gm->node_map[serv_mpi_id], server_init_struct[p].port_id, 
                armci_client_send_callback_direct, armci_gm_client_context);
}

/*\ direct send to server 
\*/
void armci_client_direct_send(int p, void *src_buf, void *dst_buf, int len,void** contextptr,int nbtag,void *mhloc,void *mhrem)
{
    int s           = armci_clus_id(p);
    int serv_mpi_id = armci_clus_info[s].master;
    armci_gm_context_t *context;

    if(nbtag)
       *contextptr = context = armci_gm_get_next_context(nbtag);
    else{
       context = armci_gm_client_context;
       proc_gm->rdmaop_pending_ar[s]++;
    }

	    
    context->done = ARMCI_GM_SENDING;
    gm_directed_send_with_callback(proc_gm->port, src_buf,
               (gm_remote_ptr_t)(gm_up_t)dst_buf, len, GM_LOW_PRIORITY,
                proc_gm->node_map[serv_mpi_id], proc_gm->port_map[s], 
                armci_client_send_callback_direct, context);
    if(!nbtag)
       armci_client_send_complete(context,"armci_client_direct_send ");

    /* blocking: wait until send is done by calling the callback */
}

/*\ RDMA get 
\*/
void armci_client_direct_get(int p, void *src_buf, void *dst_buf, int len,
                             void** contextptr,int nbtag,void *mhdl,void *mhdl1)
{

#ifdef GM2
    int s           = armci_clus_id(p);
    int serv_mpi_id = armci_clus_info[s].master;
    armci_gm_context_t *context;

    if(nbtag)
       *contextptr = context = armci_gm_get_next_context(nbtag);
    else{
       context = armci_gm_client_context;
       proc_gm->rdmaop_pending_ar[s]++;
    }


    context->done = ARMCI_GM_SENDING;
    gm_get(proc_gm->port, (gm_remote_ptr_t)(gm_up_t)src_buf, dst_buf, len, 
                GM_LOW_PRIORITY,
                proc_gm->node_map[serv_mpi_id], proc_gm->port_map[s],
                armci_client_send_callback_direct, context);

    /* blocking: wait until get is done by calling the callback */
    if(!nbtag)
       armci_client_send_complete(context,"armci_client_direct_get");
#else
    armci_die("armci_client_direct_get:gm_get not available",0);
#endif

}


/* computing process start initial communication with all the servers */
void armci_client_connect_to_servers()
{
    int server_mpi_id, i;
    /* make sure that server thread is ready */
    if(armci_me == armci_master) while(!armci_gm_server_ready) usleep(10);
    armci_msg_barrier();
 
    /* make initial conection to the server, not the server in this node */
    for(i=0; i<armci_nclus; i++) {
        if(armci_clus_me != i) {
            server_mpi_id = armci_clus_info[i].master;
            proc_gm->serv_ack_ptr[i] = (client_init_struct[i].ack+armci_me);
            if(DEBUG_INIT_) fprintf(stderr, "%d: connected to server %d\n",
                                    armci_me, server_mpi_id);
 
        }
    }
}

/*\ wait until flag is updated indicated that bypass transfer is done
\*/
void armci_wait_for_data_bypass(request_header_t* msginfo)
{
    /* check the header ack */
    armci_wait_long_flag_updated_clear(&(msginfo->tag.ack), ARMCI_GM_COMPLETE);

    /* reset header ack */
    msginfo->tag.ack = ARMCI_GM_CLEAR;
}

void armci_clear_ack(int proc)
{
 serv_gm->ack[proc]=ARMCI_GM_CLEAR;
}

/*\ wait until ack is set by client to certain value: on failure return 1  
\*/
int armci_wait_client_seq(int proc,int val)
{
long *buf;
long spin =0,res;

    if(proc <0 || proc >= armci_nproc)armci_die("armci_wait_pin_client:p",proc);
    buf = serv_gm->ack +proc;

    res = check_flag(buf);
    while(res < (long)val){
       for(spin=0; spin<__armci_wait_some; spin++)__armci_fake_work+=0.001;
       res = check_flag(buf);
    }
    __armci_fake_work =99.0;
    return 1;
}

/*\ wait until ack is set by client: on failure return 1  
\*/
int armci_wait_pin_client(int proc)
{
long status;

    if(proc <0 || proc >= armci_nproc)armci_die("armci_wait_pin_client:p",proc);

    status = armci_wait_long_flag_not_clear(serv_gm->ack +proc);
    serv_gm->ack[proc]=ARMCI_GM_CLEAR;
    if(status == ARMCI_GM_FAILED) return 1;
    return 0;
}

void armci_client_send_ack(int p, int success)
{
     int cluster = armci_clus_id(p);
     long *pflag = proc_gm->tmp;
     *pflag= (success)? success : ARMCI_GM_FAILED;
     armci_client_direct_send(p, pflag, proc_gm->serv_ack_ptr[cluster], 
                              sizeof(long),NULL,0,NULL,NULL);
}

static int get_corrected_size(size){
    if(size<=11)return 11;
    else if(size<=15)return 15;
    else if(size<=18)return 18;
    else if(size<=19)return 19;
    else armci_die("wrong size",size);
    return(-1);
}

/*\ send request message to server and wait for completion
 *  assumption: the buffer is pinned and most probably is MessageSndBuffer
\*/
int armci_send_req_msg(int proc, void *vbuf, int len)
{
    char *buf     = (char*)vbuf;
    int size      = get_corrected_size(gm_min_size_for_length(len));
    int s         = armci_clus_id(proc);
    int serv_mpi_id = armci_clus_info[s].master;
    request_header_t *msginfo = (request_header_t *)vbuf;
    armci_gm_context_t *context;
    MPI_Status status;
    int flag;
    context= BUF_TO_EVBUF(buf);
    
    /* set the message tag */
    msginfo->tag.data_ptr = buf + sizeof(request_header_t) - sizeof(long);
    msginfo->tag.ack = ARMCI_GM_CLEAR;
    context->done = ARMCI_GM_SENDING;

    /*for fence operation*/
    if(msginfo->operation==PUT || ACC(msginfo->operation))
       proc_gm->ops_pending_ar[s]++;

    /* flow control */
    _armci_buf_ensure_one_outstanding_op_per_node(buf, s );

    if(DEBUG_){
       printf("%d: armci_send_req_msg op is %d sending ack=%d to %d\n",armci_me,
               msginfo->operation,proc_gm->ops_pending_ar[s],proc);
       fflush(stdout);
    }
    MPI_Iprobe(armci_me, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status); 
    gm_send_with_callback(proc_gm->port, buf, size, len, GM_LOW_PRIORITY,
                          proc_gm->node_map[serv_mpi_id], proc_gm->port_map[s], 
                          armci_client_send_callback, context);
    return 0;
}


/* check if data is available in the buffer
 * assume the buf is pinned and is inside MessageSndBuffer
 * format buf = hdr ack + data + tail ack
 */
char *armci_ReadFromDirect(int proc, request_header_t * msginfo, int len)
{
    char *buf = (char*) msginfo;
    long *tail;

/*
     printf("%d: reading direct ptr=%x\n", armci_me,&(msginfo->tag.ack));fflush(stdout);
*/

    /* make sure request message was delivered */
    armci_ensure_req_complete(msginfo);

    /* check the header ack */
    armci_wait_long_flag_updated_clear(&(msginfo->tag.ack), ARMCI_GM_COMPLETE);

    /* reset header ack */
    msginfo->tag.ack = ARMCI_GM_CLEAR;

    buf += sizeof(request_header_t);
    
    /* check the tail ack */
    tail = (long*)(buf+len);
    ALIGN_PTR_LONG(long, tail);

    armci_wait_long_flag_updated_clear(tail, ARMCI_GM_COMPLETE);

    /* reset tail ack */
    *tail = ARMCI_GM_CLEAR;
    MEM_FENCE;
    return(buf);
}


/*********************************************************************
                           SERVER SIDE                            
 *********************************************************************/

static int get_size_for_index(int index)
{

    /*when changing any of the below, dont forget to change the total returned
     * in the line below! Also, modify get_corrected_size function accordingly*/

    if(index==-1)return 94;         /*to get the total number of buffers */
    else if(index<64)return(11);    /*64 buffers of size 11 and length 2040*/
    else if(index<84)return(15);    /*20 buffers of size 15 and length 32760*/
    else if(index<92)return(18);    /* 8 buffers of size 18 and length 262136*/
    else if(index<94)return(19);    /* 2 buffers of size 19 and length 400000*/
    else return(0);                 /*total 94 buffers total length 3682848 */
}


/* preallocate required memory at the startup */
int armci_gm_serv_mem_alloc()
{
    int i,idx=0;
    /********************** get local unregistered memory *******************/
    /* allocate dma buffer for low priority */
      
    serv_gm->dma_buf = (void **)malloc((get_size_for_index(-1))*sizeof(void*));
    if(!serv_gm->dma_buf)return FALSE;

    /* allocate buf for keeping the pointers of client MessageSndbuffer */
    serv_gm->proc_buf_ptr = (long *)calloc(armci_nproc, sizeof(long));
    if(!serv_gm->proc_buf_ptr) return FALSE;
    /********************** get registered memory **************************/
    i = get_size_for_index(idx);
    while(i) {
        gm_size_t len;
        if(i==19)len = (gm_size_t)MSG_BUFLEN;
        else len = gm_max_length_for_size(i);
        if((armci_me==0) && DEBUG_){
           printf("%d:size %d len=%ld\n",armci_me,i,len);
        }
        serv_gm->dma_buf[idx] = (char *)gm_dma_malloc(serv_gm->rcv_port,
                                        len);
        if(!serv_gm->dma_buf[idx]) return FALSE;
        i = get_size_for_index(++idx);
    }

    if(DEBUG_INIT_){
       printf("\n%d:armci_serv_mem_alloc- allocated mem properly\n",armci_me);
       fflush(stdout);
    }

    /* allocate ack buffer for each client process */
    serv_gm->ack = (long *)gm_dma_malloc(serv_gm->rcv_port,
                                             armci_nproc*sizeof(long));
    if(serv_gm->ack == 0) return FALSE;
    for(i=0; i<armci_nproc; i++)serv_gm->ack[i]=ARMCI_GM_CLEAR;

    serv_gm->direct_ack = (long*)gm_dma_malloc(serv_gm->snd_port, sizeof(long));
    if(serv_gm->direct_ack == 0) return FALSE;
    *serv_gm->ack = ARMCI_GM_CLEAR;
    
    /*place to copy the done operation count before transmission (for fence) */
    serv_gm->ops_done_ack=(ops_t*)gm_dma_malloc(serv_gm->snd_port,sizeof(ops_t));
    if(serv_gm->ops_done_ack == 0) return FALSE;
    *serv_gm->ops_done_ack = ARMCI_GM_CLEAR;
 
    /* allocate recv buffer */
    MessageRcvBuffer = (char *)gm_dma_malloc(serv_gm->snd_port, MSG_BUFLEN+64);
    if(MessageRcvBuffer == 0) return FALSE;
    /* stamp the last byte */
    serv_tail= MessageRcvBuffer + MSG_BUFLEN;
    *serv_tail=SERV_STAMP;

    serv_gm->proc_ack_ptr = (long *)gm_dma_malloc(serv_gm->snd_port,
                                                  armci_nproc*sizeof(long));

    /*Registered memory for GPC data structures*/
#ifdef ARMCI_ENABLE_GPC_CALLS
    gpc_req = (gpc_buf_t *)gm_dma_malloc(serv_gm->snd_port, 
					 MAX_GPC_REQ*sizeof(gpc_buf_t));
#endif

    if(serv_gm->proc_ack_ptr == 0) return FALSE;

    return TRUE;
}


/* deallocate the preallocated memory used by gm */
int armci_gm_serv_mem_free()
{
    int i,idx;

    free(serv_gm->proc_buf_ptr);

    gm_dma_free(serv_gm->snd_port, serv_gm->proc_ack_ptr);
    gm_dma_free(serv_gm->rcv_port, serv_gm->ack);
    gm_dma_free(serv_gm->snd_port, serv_gm->direct_ack);
    
    idx = 0;
    i = get_size_for_index(idx);
    while(i) {
        gm_dma_free(serv_gm->rcv_port, serv_gm->dma_buf[idx]);
        i = get_size_for_index(++idx);
    }
    free(serv_gm->dma_buf);
    
    gm_dma_free(serv_gm->snd_port, MessageRcvBuffer);
#ifdef ARMCI_ENABLE_GPC_CALLS
    gm_dma_free(serv_gm->snd_port, gpc_req);
#endif

    return TRUE;
}

/* server side call back func */
void armci_serv_callback(struct gm_port *port, void *context, 
			 gm_status_t status)
{
    if(status==GM_SUCCESS) ((armci_gm_context_t*)context)->done = ARMCI_GM_SENT;
    else ((armci_gm_context_t *)context)->done = ARMCI_GM_FAILED;
}

/* server side call back func */
void armci_serv_callback_nonblocking(struct gm_port *port, void *context, 
                                     gm_status_t status)
{
    if(status == GM_SUCCESS)
        serv_gm->complete_msg_ct++;
    else
        armci_die(" armci_serv_callback_nonblocking: send failed", 0);
}

/* server trigers gm_unknown, so that callback func can be executed */
int armci_serv_send_complete()
{
    gm_recv_event_t *event;

    while(armci_gm_serv_context->done == ARMCI_GM_SENDING) {
        event = gm_blocking_receive_no_spin(serv_gm->snd_port);
        gm_unknown(serv_gm->snd_port, event);
    }
    
    return(armci_gm_serv_context->done);
}


/*\ block until the number of outstanding nonblocking messages <= specified val
\*/
void armci_serv_send_nonblocking_complete(int max_outstanding)
{
    gm_recv_event_t *event;

    while((serv_gm->pending_msg_ct - serv_gm->complete_msg_ct) >
          max_outstanding) {
        event = gm_blocking_receive_no_spin(serv_gm->snd_port);
        gm_unknown(serv_gm->snd_port, event);
    }
}


int armci_serv_ack_complete()
{
    gm_recv_event_t *event;

    while(armci_gm_serv_ack_context->done == ARMCI_GM_SENDING) {
        event = gm_blocking_receive_no_spin(serv_gm->snd_port);
        gm_unknown(serv_gm->snd_port, event);
    }
    
    return(armci_gm_serv_ack_context->done);
}

static int armci_serv_pendingop_complete()
{
    gm_recv_event_t *event;

    while(armci_gm_serv_pendingop_context->done == ARMCI_GM_SENDING) {
        event = gm_blocking_receive_no_spin(serv_gm->snd_port);
        gm_unknown(serv_gm->snd_port, event);
    }

    return(armci_gm_serv_pendingop_context->done);
}

static int port_id=1,board_id=0;
static int armci_get_free_port(int ports, int boards, struct gm_port **p)
{

   for(; board_id<boards; board_id++){
      for(; port_id<ports; port_id++){
         if(GM_SUCCESS == gm_open(p,board_id,port_id,"try",GM_API_VERSION_1_1))
                          return port_id;
      }
      port_id=0; /*start from 0 on next board */
   }
   return(-1);     
}

/* initialization of server thread */
int armci_gm_server_init() 
{
    int i,idx;
    int status;
    int buf_tag = TAG_DFLT,prev_i;
    
    unsigned long size_mask;
    unsigned int min_mesg_size, min_mesg_length;
    unsigned int max_mesg_size, max_mesg_length;
    char *enval;
 
    /* allocate gm data structure for server */
    serv_gm->node_map = (int *)malloc(armci_nproc * sizeof(int));
    serv_gm->port_map = (int *)calloc(armci_nproc, sizeof(int));
    if((serv_gm->node_map == NULL) || (serv_gm->port_map == NULL)) {
        fprintf(stderr,"%d: Error allocate server data structure.\n",armci_me);
        return FALSE;
    }

    if(DEBUG_) {
       fprintf(stdout,
               "%d(server):opening gm port %d(rcv)dev=%d and %d(snd)dev=%d\n",
               armci_me, ARMCI_GM_SERVER_RCV_PORT,ARMCI_GM_SERVER_RCV_DEV,
               ARMCI_GM_SERVER_SND_PORT, ARMCI_GM_SERVER_SND_DEV);
    }

    serv_gm->rcv_port = NULL; serv_gm->snd_port = NULL;

#ifdef STATIC_PORTS
    /* opening gm port for sending data back to clients */
    status = gm_open(&(serv_gm->snd_port), ARMCI_GM_SERVER_SND_DEV,
                     ARMCI_GM_SERVER_SND_PORT, "gm_pt", GM_API_VERSION_1_1);
    if(status != GM_SUCCESS) 
                 armci_die("did not get rcv port",ARMCI_GM_SERVER_SND_DEV);
    /* opening gm rcv port for requests */
    serv_gm->port_id  = ARMCI_GM_SERVER_RCV_PORT; 
    status = gm_open(&(serv_gm->rcv_port), ARMCI_GM_SERVER_RCV_DEV,
                     ARMCI_GM_SERVER_RCV_PORT, "gm_pt", GM_API_VERSION_1_1);
    if(status != GM_SUCCESS) 
                 armci_die("did not get rcv port",ARMCI_GM_SERVER_RCV_DEV);
#else
    status= armci_get_free_port(12, 2, &(serv_gm->snd_port));  
    if(DEBUG_INIT_)printf("%d server got snd port %d \n",armci_me,status);
    if(status!=-1) status= armci_get_free_port(12, 2, &(serv_gm->rcv_port));  
    if(DEBUG_INIT_)printf("%d server got rcv port %d \n",armci_me,status);
    serv_gm->port_id = status;
#endif

    /* get my node id */
    status = gm_get_node_id(serv_gm->rcv_port,(unsigned int *) (&(serv_gm->node_id)));
    if(status != GM_SUCCESS)armci_die("Could not get GM node id",0);
    if(DEBUG_)printf("%d(server): node id is %d\n", armci_me, serv_gm->node_id);

    /* allow direct send */
    status = gm_allow_remote_memory_access(serv_gm->rcv_port);
    if(status != GM_SUCCESS) {
        fprintf(stderr,"%d(server): could not enable direct sends\n", armci_me);
        return FALSE;
    }

    /* enable put i.e., dma send */
    status = gm_allow_remote_memory_access(serv_gm->snd_port);
    if(status != GM_SUCCESS) {
        fprintf(stderr,"%d(server): could not enable direct sends\n", armci_me);
        return FALSE;
    }
    
    /* memory preallocation for server */
    if(!armci_gm_serv_mem_alloc()) {
        fprintf(stderr,"%d(server): failed allocating memory\n", armci_me);
        return FALSE;
    }

    /* set message size on server */
    min_mesg_size = ARMCI_GM_MIN_MESG_SIZE;
    max_mesg_size = gm_min_size_for_length(MSG_BUFLEN);
    min_mesg_length = gm_max_length_for_size(min_mesg_size);
    max_mesg_length = MSG_BUFLEN;
    if(DEBUG_INIT_) {
        printf("%d: SERVER min_mesg_size = %d, max_mesg_size = %d\n",
               armci_me, min_mesg_size, max_mesg_size);
        printf("%d: SERVER min_mesg_length = %d, max_mesg_length = %d\n",
               armci_me, min_mesg_length, max_mesg_length);
         fflush(stdout);
    }
    
    /* accept only the smallest size messages */
    size_mask = (2 << max_mesg_size) - 1;
    status = gm_set_acceptable_sizes(serv_gm->rcv_port, GM_LOW_PRIORITY,
                                     size_mask);
    if (status != GM_SUCCESS) {
        fprintf(stderr, "%d(server): error setting acceptable sizes", armci_me);
        return FALSE;
    }

    /* provide the buffers initially create a size mask and set */
    idx = 0;
    i = get_size_for_index(idx);
    while(i) {
        prev_i=i;
        if((armci_me==0) && DEBUG_){
           printf("%d:size %d len=%ld tag=%d\n",armci_me,i,gm_max_length_for_size(i),buf_tag);
        }
        gm_provide_receive_buffer_with_tag(serv_gm->rcv_port,
               serv_gm->dma_buf[idx], i, GM_LOW_PRIORITY, buf_tag);
        i = get_size_for_index(++idx);
        if(prev_i != i)buf_tag++;
        prev_i = i;
    }

    /* provide the extra set of buffers for short messages */
    if(DEBUG_){
       printf("provided (%d,%d) buffers, rcv tokens=%d\n",
               idx,gm_min_size_for_length(get_size_for_index(idx-1)),
               gm_num_receive_tokens(serv_gm->rcv_port));
    }

    serv_gm->pending_msg_ct = 0; serv_gm->complete_msg_ct = 0; 

    if(!(serv_gm->ops_done_ar = (ops_t*)calloc(armci_nproc,sizeof(ops_t))))
        armci_die("malloc failed for ARMCI ops_done_ar",0);

    /* check if we can poll in the server thread */
    enval = getenv("ARMCI_SERVER_CAN_POLL");
    if(enval != NULL){
       if((enval[0] != 'N') && (enval[0]!='n')) server_can_poll=1;
    } else {
      if(armci_clus_info[armci_clus_me].nslave < armci_getnumcpus()) server_can_poll=1;
    }
    
    return TRUE;
}


/* server start communication with all the compute processes */
void armci_server_initial_connection()
{
    int i;
    /* notify client thread that we are ready to take requests */
    armci_gm_server_ready = 1;
    /* receive the initial connection from all computing processes,
     * except those from the same node
    */
 
    for(i=0;i<armci_nproc;i++){
        serv_gm->port_map[i] = server_init_struct[i].port_id ;
        serv_gm->proc_ack_ptr[i] = (long)server_init_struct[i].ack;
    }
    /*for(i=0;i<armci_nproc;i++){
        fprintf(stderr,"\nin here%d is s_i_stuct ack ",server_init_struct[i].ack);
    }*/
 
}

/* direct send from server to client */
void armci_server_direct_send(int dst, char *src_buf, char *dst_buf, int len,
                              int type)
{
    if(type == ARMCI_GM_BLOCKING) {
        armci_gm_serv_context->done = ARMCI_GM_SENDING;

        gm_directed_send_with_callback(serv_gm->snd_port, src_buf,
               (gm_remote_ptr_t)(gm_up_t)(dst_buf),
               len, GM_LOW_PRIORITY, serv_gm->node_map[dst],
               serv_gm->port_map[dst], armci_serv_callback,
               armci_gm_serv_context);
    }
    else if(type == ARMCI_GM_NONBLOCKING) {
        gm_directed_send_with_callback(serv_gm->snd_port, src_buf,
               (gm_remote_ptr_t)(gm_up_t)(dst_buf),
               len, GM_LOW_PRIORITY, serv_gm->node_map[dst],
               serv_gm->port_map[dst], armci_serv_callback_nonblocking,
               armci_gm_serv_context);
        serv_gm->pending_msg_ct++;
    }
    else {
        gm_directed_send(serv_gm->snd_port, src_buf,
               (gm_remote_ptr_t)(gm_up_t)(dst_buf), len,
               GM_LOW_PRIORITY, serv_gm->node_map[dst], serv_gm->port_map[dst]);
    }
}


/* server direct send to the client
 * assume buf is pinned and using MessageRcvBuffer
 * MessageRcvBuffer: .... + hdr ack + data + tail ack
 *                                         ^
 *                                         buf (= len)
 */
void armci_WriteToDirect(int dst, request_header_t *msginfo, void *buffer)
{
    char *buf = (char*)buffer; 
    char *ptr = buf - sizeof(long);
    long *tail, *utail;
    int bytes;

    /* adjust the dst pointer */
    void *dst_addr = msginfo->tag.data_ptr;
    
    /* set head ack */
    *(long *)ptr = ARMCI_GM_COMPLETE;
   
    /* set tail ack, make sure it is alligned */
    utail= tail = (long*)(buf + msginfo->datalen);
    ALIGN_PTR_LONG(long, tail);
    *tail = ARMCI_GM_COMPLETE;
    bytes = (char*)tail - (char*)utail; /* add allignment */
    bytes+= 2*sizeof(long);

    if(armci_serv_send_complete() == ARMCI_GM_FAILED)
        armci_die(" server last send failed", dst);
    armci_gm_serv_context->done = ARMCI_GM_SENDING;
    

/*    printf("%d: writing direct ptr=%x\n", armci_me,dst_addr);fflush(stdout);
*/
    gm_directed_send_with_callback(serv_gm->snd_port, ptr,
                     (gm_remote_ptr_t)(gm_up_t)(dst_addr),
                     msginfo->datalen+bytes, GM_LOW_PRIORITY,
                     serv_gm->node_map[dst], serv_gm->port_map[dst],
                     armci_serv_callback, armci_gm_serv_context);
}


/* NOT USED server inform the client the send is complete */
void armci_InformClient(int dst, void *buf, long flag)
{
    *(long *)buf = flag;

    armci_gm_serv_ack_context->done = ARMCI_GM_SENDING;
    gm_directed_send_with_callback(serv_gm->snd_port, buf,
       (gm_remote_ptr_t)(gm_up_t)(serv_gm->proc_ack_ptr[dst]),
        sizeof(long), GM_LOW_PRIORITY, serv_gm->node_map[dst],
        serv_gm->port_map[dst], armci_serv_callback, armci_gm_serv_ack_context);
    
    /* blocking: wait til the send is done by calling the callback */
    if(armci_serv_ack_complete() == ARMCI_GM_FAILED)
        armci_die(" failed sending data to client", dst);
}


void armci_server_send_ack(request_header_t* msginfo)
{
    void *dst_addr = msginfo->tag.data_ptr;
    long *p_ack = serv_gm->direct_ack;
    int client = msginfo->from;

    armci_gm_serv_ack_context->done = ARMCI_GM_SENDING;
    *p_ack = ARMCI_GM_COMPLETE;

    gm_directed_send_with_callback(serv_gm->snd_port, p_ack,
        (gm_remote_ptr_t)(gm_up_t)dst_addr, sizeof(long),
        GM_LOW_PRIORITY, serv_gm->node_map[client], serv_gm->port_map[client],
        armci_serv_callback, armci_gm_serv_ack_context);

    if(armci_serv_ack_complete() == ARMCI_GM_FAILED)
        armci_die(" failed sending data to client", client);
}


/*\ to be used to clear all outstanding sends that this client might have
 * for now, even the non-blocking sends are completed
\*/
void armci_client_clear_outstanding_sends()
{
int i;
    if(armci_nclus==1) return;
    /*three sends that need to be cleared viz.. */
    /* 1)sends using client_send_context, */
    armci_client_send_complete(armci_gm_client_context,"armci_client_clear_outstanding_sends");

    /* 2)sends using context in buffers and */
    _armci_buf_clear_all();

    /* 3)the sends using non-blocking context array */
    for(i=0;i<MAX_PENDING;i++)
       armci_client_send_complete(armci_gm_nbcontext_array+i,"armci_client_clear_outstanding_sends");
}

/*\ sends notification to client that data in direct send was transfered/put
 *  into the client buffer
\*/
void armci_server_send_ack_old(int client)
{
    long *p_ack = serv_gm->direct_ack;

    armci_gm_serv_ack_context->done = ARMCI_GM_SENDING;
    *p_ack = ARMCI_GM_COMPLETE;

    gm_directed_send_with_callback(serv_gm->snd_port, p_ack,
        (gm_remote_ptr_t)(gm_up_t)(serv_gm->proc_ack_ptr[client]), sizeof(long),
        GM_LOW_PRIORITY, serv_gm->node_map[client], serv_gm->port_map[client], 
        armci_serv_callback, armci_gm_serv_ack_context);

    if(armci_serv_ack_complete() == ARMCI_GM_FAILED)
        armci_die(" failed sending data to client", client);
}

/*\ send the count of pending ops every time we get a put request
 *  to be used for fence
\*/
void armci_send_pendingop_ack(int donothing)
{
ops_t *p_ack=serv_gm->ops_done_ack;
ops_t *remptr = armci_gm_fence_arr[armci_gm_req_from] + armci_clus_me;
int client=armci_gm_req_from;

    if(armci_gm_req_op!=PUT && !ACC(armci_gm_req_op))return;

    if(donothing){
       *p_ack=++(serv_gm->ops_done_ar[client]);
       verify_wait->recv_verify_smp_arr[armci_gm_req_to][client]=*p_ack;
       return;
    }
    if(DEBUG_){
      printf("%d: sening ack %d to %p on %d op=%d\n",armci_me,*p_ack,remptr,
             client,armci_gm_req_op);
      fflush(stdout);
    }

    if(armci_serv_pendingop_complete() == ARMCI_GM_FAILED)
          armci_die(" failed sending data to client", client);
    armci_gm_serv_pendingop_context->done = ARMCI_GM_SENDING;

    *p_ack=++(serv_gm->ops_done_ar[client]);
    verify_wait->recv_verify_smp_arr[armci_gm_req_to][client]=*p_ack;

    gm_directed_send_with_callback(serv_gm->snd_port, p_ack,
        (gm_remote_ptr_t)(gm_up_t)(remptr), sizeof(ops_t),
        GM_LOW_PRIORITY, serv_gm->node_map[client], serv_gm->port_map[client],
        armci_serv_callback, armci_gm_serv_pendingop_context);

}


/* the main data server loop: accepting events and pass it to data server
 * code to be processed -- this is handler for GM specific requests
 */
void armci_call_data_server()
{
    int iexit = FALSE;
    unsigned int size, length, tag;
    char *buf;
    gm_recv_event_t *event; /* gm event */
    
    if(DEBUG_){
        fprintf(stdout, "%d(server): waiting for request\n",armci_me);
        fflush(stdout);
    }
#ifdef ARMCI_ENABLE_GPC_CALLS
    unblock_thread_signal(GPC_COMPLETION_SIGNAL);
#endif
    /* server main loop; wait for and service requests until QUIT requested */
    while(!iexit) {        
#ifdef ARMCI_ENABLE_GPC_CALLS
      block_thread_signal(GPC_COMPLETION_SIGNAL);
#endif
        if(server_can_poll)
            event = gm_receive(serv_gm->rcv_port);
        else
            event = gm_blocking_receive_no_spin(serv_gm->rcv_port);

        if(DEBUG_INIT_){fprintf(stdout,"%d(server:%d):receive event type %d\n",
                    armci_me,server_can_poll,event->recv.type); fflush(stdout);  
        }
        
        switch(event->recv.type) {
          case GM_RECV_EVENT:
          case GM_PEER_RECV_EVENT:
              size = gm_ntohc(event->recv.size);
              tag = gm_ntohc(event->recv.tag);
              length = gm_ntohl(event->recv.length);
              buf = (char *)gm_ntohp(event->recv.buffer);
              MEM_FENCE;
              armci_data_server(buf);
#           ifdef ACK_FENCE
              armci_send_pendingop_ack(0);
#           else
              armci_send_pendingop_ack(1);
#           endif
              gm_provide_receive_buffer_with_tag(serv_gm->rcv_port, buf,
                                                 size, GM_LOW_PRIORITY, tag);
    
              break;
          case GM_NO_RECV_EVENT:
              if(server_can_poll) break;

          default:
              gm_unknown(serv_gm->rcv_port, event);
              break;
        }
#ifdef ARMCI_ENABLE_GPC_CALLS
	unblock_thread_signal(GPC_COMPLETION_SIGNAL);
#endif
    }
    
    if(DEBUG_) {printf("%d(server): done! closing\n",armci_me); fflush(stdout);}
}


/* cleanup of GM: applies to either server or client */
void armci_transport_cleanup()
{    
    /* deallocate the gm data structure */
    if(SERVER_CONTEXT) {
#if 0        
        if(!armci_gm_serv_mem_free()) 
            armci_die("server memory deallocate memory failed", armci_me);
#endif            
        gm_close(serv_gm->rcv_port);
        gm_close(serv_gm->snd_port);
        free(serv_gm->node_map); free(serv_gm->port_map);
        if(*serv_tail != SERV_STAMP){
          printf("%d: server_stamp %d %d\n",armci_me,*serv_tail, SERV_STAMP);
          armci_die("ARMCI Internal Error: end-of-buffer overwritten",0);
        }
    }
    else {
#if 0
        if(!armci_gm_proc_mem_free()) 
            armci_die("computing process  memory deallocate memory failed",0);
#endif   
        free(proc_gm->node_map);
        /*if(*client_tail != CLIENT_STAMP){
          printf("%d: client_stamp %d %d\n",armci_me,*client_tail,CLIENT_STAMP);
          armci_die("ARMCI Internal Error: end-of-buffer overwritten",0);
        }*/
    }
}
 

void armci_init_connections()
{
    if(armci_me == armci_master) {
        if(!armci_gm_server_init())
            armci_die("GM:server connection initialization failed", 0L);
    }
    if(!armci_gm_client_init())
        armci_die("GM:client connection initialization failed", 0L);

}

/***************************** dead code *******************************/
#if 0
#define NBUFS 4
#define NBUFS_MAX 8
static void *bufarr[NBUFS_MAX]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
static int nbuf_cur=0;

void armci_gm_freebuf_tag(int tag)
{
armci_gm_context_t *context;

  if( tag<0 || tag>=NBUFS) armci_die("armci_gm_freebuf_tag: bad tag",tag);
  if(bufarr[tag]){
     context = (armci_gm_context_t *)bufarr[tag];
     if(tag != context->tag)armci_die("armci_gm_freebuf_tag: tag mismatch",tag);
     armci_client_send_complete(context);
     armci_buf_free(bufarr[tag]);
     bufarr[tag]=(void*)0;
  }else{
      armci_die(" armci_gm_freebuf_tag: NULL ptr",tag );
  }

  if(DEBUG_)    {int i;
      printf("%d:freed tag=%d op=%d bufarr[",armci_me,tag,
             ((request_header_t*)(context+1))->operation);
      for(i=0;i<NBUFS;i++)printf("%p ",bufarr[i]);
      printf("]\n"); fflush(stdout);
  }
}

void armci_wait_for_server_ack(){
   armci_wait_long_flag_updated((long *)proc_gm->ack,ARMCI_GM_COMPLETE);
}	

char* armci_gm_getbuf(size_t size)
{
extern char *armci_buf_alloc(size_t bytes);
armci_gm_context_t *context;
char *ptr;
int i, bytes = size + sizeof(armci_gm_context_t);
  
   if(DEBUG_){
      printf("%d: getting buffer size %d\n",armci_me,bytes); fflush(stdout);}

   /* get buffer of specified size but also add context */     
   ptr = armci_buf_alloc(bytes);

   if(!ptr){ /* no memory available wait until all pending requests complete */
      nbuf_cur = (nbuf_cur+1)%NBUFS;
      for(i=0;i<NBUFS;i++){
         if(bufarr[nbuf_cur])
            armci_gm_freebuf_tag(((armci_gm_context_t*)bufarr[nbuf_cur])->tag);
         nbuf_cur = (nbuf_cur+1)%NBUFS;
      }
      nbuf_cur =0; /* start from beginning next time*/

      ptr = armci_buf_alloc(bytes );
      if(!ptr)armci_die("armci_gm_getbuf: did not get memory ",(int)size);

      if(DEBUG_){
         printf("%d: armci_gm_getbuf: completed all reqs\n",armci_me);
         fflush(stdout); }

   }else {

      /* search for first free slot */
      for(i=0;i<NBUFS;i++)
         if(bufarr[nbuf_cur]) {nbuf_cur++; nbuf_cur %=NBUFS; }
         else break;

      if(bufarr[nbuf_cur]){ /* no empty found -> free current one */
            armci_gm_freebuf_tag(((armci_gm_context_t*)bufarr[nbuf_cur])->tag);
      }
   }

   bufarr[nbuf_cur]=ptr;

   /* initialize context properly */
   context = (armci_gm_context_t*)bufarr[nbuf_cur];
   context->tag= nbuf_cur;
   context->done = ARMCI_GM_CLEAR;

   nbuf_cur = (nbuf_cur+1)%NBUFS;
   
   return (char*)(context+1);
}

void armci_gm_freebuf(void *ptr)
{
armci_gm_context_t *context = ((armci_gm_context_t *)ptr)-1;
      armci_gm_freebuf_tag(context->tag);
}
#endif

/***************************** end dead code *******************************/
static void armci_pipe_advance_buf(int strides, int count[], 
                                   char **buf, long **ack, int *bytes )
{
int i, extra;

     for(i=0, *bytes=1; i<=strides; i++)*bytes*=count[i]; /*compute chunk size*/
     
     /* allign receive buffer on 64-byte boundary */
     extra = ALIGNLONGADD((*buf));
     (*buf) +=extra;                  /*** this where the data is *******/
     if(DEBUG2){ printf("%d: pipe advancing %d %d\n",armci_me, *bytes,extra); fflush(stdout);
     }
     *ack = (long*)((*buf) + *bytes); /*** this is where ACK should be ***/
}


/*\ prepost buffers for receiving data from server (pipeline)
\*/
void armcill_pipe_post_bufs(void *ptr, int stride_arr[], int count[],
                            int strides, void* argvoid)
{
int bytes;
buf_arg_t *arg = (buf_arg_t*)argvoid;
long *ack;

     armci_pipe_advance_buf(strides, count, &arg->buf_posted, &ack, &bytes);

     if(DEBUG2){ printf("%d: posting %d pipe receive %d b=%d (%p,%p) ack=%p\n",
          armci_me,arg->count,arg->proc,bytes,arg->buf, arg->buf_posted,ack);
          fflush(stdout);
     }
     *ack = 0L;                      /*** clear ACK flag ***/

     arg->buf_posted += bytes+sizeof(long);/* advance pointer for next chunk */
     arg->count++;
}


void armcill_pipe_extract_data(void *ptr, int stride_arr[], int count[],
                               int strides, void* argvoid)
{
int bytes;
long *ack;
buf_arg_t *arg = (buf_arg_t*)argvoid;

     /* for first chunk, wait for the request message to complete */
     if(!arg->count) {
       /* adjust for ack in msg_tag_t */
       request_header_t* msginfo=(request_header_t*)
                                 (arg->buf_posted+sizeof(long));
       msginfo--;
       armci_ensure_req_complete(msginfo);
     }

     armci_pipe_advance_buf(strides, count, &arg->buf_posted, &ack, &bytes);

     if(DEBUG2){ printf("%d:extracting pipe  data from %d %d b=%d %p ack=%p\n",
            armci_me,arg->proc,arg->count,bytes,arg->buf,ack); fflush(stdout);
     }

     armci_wait_long_flag_updated(ack, 1); /********* wait for data ********/

     *ack = 0L;                      /*** touching memory ***/
     /* copy data to the user buffer identified by ptr */
     armci_read_strided(ptr, strides, stride_arr, count, arg->buf_posted);
     if(DEBUG2 ){printf("%d(c):extracting: data %p first=%f\n",armci_me,
                arg->buf_posted,((double*)arg->buf_posted)[0]); 
                fflush(stdout);
     }

     arg->buf_posted += bytes+sizeof(long);/* advance pointer for next chunk */
     arg->count++;
}


void armcill_pipe_send_chunk(void *data, int stride_arr[], int count[],
                             int strides, void* argvoid)
{
int bytes, bytes_ack;
buf_arg_t *arg = (buf_arg_t*)argvoid;
long *ack;

     armci_pipe_advance_buf(strides, count, &arg->buf_posted, &ack, &bytes);
     armci_pipe_advance_buf(strides, count, &arg->buf, &ack, &bytes);
     bytes_ack = bytes+sizeof(long);

     if(DEBUG2){ printf("%d:SENDING pipe data %d to %d %p b=%d %p)\n",armci_me,
                 arg->count, arg->proc, arg->buf, bytes, ack); fflush(stdout);
     }

     /* copy data to buffer */
     armci_write_strided(data, strides, stride_arr, count, arg->buf);
     *ack=1;

     armci_server_direct_send(arg->proc, arg->buf, arg->buf_posted, bytes_ack,
                              ARMCI_GM_NONBLOCKING);

     if(DEBUG2){ printf("%d:  out of send %d bytes=%d first=%f\n",armci_me,
               arg->count,bytes,((double*)arg->buf)[0]); fflush(stdout);
     }

#if 0
     /* at any time, we will allow fixed numer of outstanding sends */
     armci_serv_send_nonblocking_complete(8);
#endif

     arg->buf += bytes+sizeof(long);        /* advance pointer for next chunk */
     arg->buf_posted += bytes+sizeof(long); /* advance pointer for next chunk */
     arg->count++;
}

void armci_pipe_send_req(int proc, void *vbuf, int len)
{
 armci_send_req_msg(proc, vbuf, len);
}


/*****************************New ARMCI Fence for GM**********************/
ops_t armci_check_int_val(ops_t *v)
{
  return (*v);
}

void armci_gm_fence(int p)
{
    long loop=0;
    int cluster = armci_clus_id(p);
    ops_t *buf;
    ops_t res;
    if(SAMECLUSNODE(p))return;
    buf=armci_gm_fence_arr[armci_me] + cluster;
    res = proc_gm->ops_pending_ar[cluster] - armci_check_int_val(buf);
#if 0
    if(proc_gm->ops_pending_ar[cluster])
    printf("%d: client fencing proc=%d fence=%p slot %p pending=%d got=%d\n", 
           armci_me, p, armci_gm_fence_arr[armci_me], buf, 
           proc_gm->ops_pending_ar[cluster], armci_check_int_val(buf)); 
    fflush(stdout);
#endif

    while(res){
       if(++loop == 1000) { loop=0; usleep(1);  }
       armci_util_spin(loop, buf);
       res = proc_gm->ops_pending_ar[cluster] - armci_check_int_val(buf);
    }
}


/**************************Functions for notify-wait**********************/
int armci_inotify_proc(int proc)
{
int *remptr = verify_wait->recv_verify_arr[proc]+2*armci_me;
int tag;
    if(SAMECLUSNODE(proc)){
#ifdef MEM_FENCE
       MEM_FENCE;
#endif
       remptr = verify_wait->recv_verify_smp_arr[proc]+armci_me;
       *(remptr)=verify_wait->verify_seq_ar[proc]++;
       return((*remptr));
    }
    else {
       *(proc_gm->itmp) = verify_wait->verify_seq_ar[proc]++;
       *(proc_gm->itmp+1) = proc_gm->ops_pending_ar[armci_clus_id(proc)];
       *(proc_gm->itmp+2) = proc_gm->rdmaop_pending_ar[armci_clus_id(proc)];
#if 0
       armci_client_to_client_direct_send(proc,proc_gm->itmp,remptr,
		                          2*sizeof(int));
#else
       tag = GET_NEXT_NBTAG();
       if (notify_gmc_arr[proc])
         if(notify_gmc_arr[proc]->done == ARMCI_GM_SENDING)
           armci_client_send_complete(notify_gmc_arr[proc],"armci_inotify_proc");
       armci_client_direct_send(proc,proc_gm->itmp,remptr,2*sizeof(int),&notify_gmc_arr[proc],tag,
                                NULL,NULL);
#endif
       if(DEBUG_NOTIFY){
         printf("\n%d: sending %d %d to %d at %p\n",armci_me,*(proc_gm->itmp),
                  *(proc_gm->itmp+1),proc,remptr);
         fflush(stdout);
       }
       return(*(proc_gm->itmp));
    }
}


int armci_inotify_wait(int proc, int *pval)
{
int *buf_notify,*buf_fence;
int wait_val,wait_fence=0;
int res;
long loop=0;
    wait_val = verify_wait->wait_seq_ar[proc]++;
    buf_notify = verify_wait->recv_verify_arr[armci_me]+2*proc;
    buf_fence = verify_wait->recv_verify_smp_arr[armci_me]+proc;
    if(SAMECLUSNODE(proc)){
       buf_notify = verify_wait->recv_verify_smp_arr[armci_me]+proc;
       wait_fence = wait_val; 
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
      res = wait_val - armci_check_int_val(buf_notify);
      while(res>0){
        if(++loop == 1000) { loop=0;usleep(1); }
        armci_util_spin(loop, buf_notify);
        res = wait_val - armci_check_int_val(buf_notify);
      }
      if(DEBUG_NOTIFY){
        printf("\n%d:arrived verifyseq expected%d have %d",armci_me,
               wait_val,armci_check_int_val(buf_notify));fflush(stdout);
      }
    }

    if(!SAMECLUSNODE(proc))
       wait_fence = *(verify_wait->recv_verify_arr[armci_me]+2*proc+1);
    res = wait_fence - armci_check_int_val(buf_fence);
    if(DEBUG_NOTIFY){
      printf("\n%d:fence expecting%d have %d",
             armci_me,wait_fence,armci_check_int_val(buf_fence));
    }
    while(res>0){
       if(++loop == 1000) { loop=0;usleep(1); }
       armci_util_spin(loop, buf_fence);
       res = wait_fence - armci_check_int_val(buf_fence);
    }
    *pval = armci_check_int_val(buf_notify);

    return(wait_val);
}

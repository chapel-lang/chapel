/* $id$ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "armcip.h"
#include "message.h"
#include <stdint.h>

#define DEBUG_COMM 0
#define DEBUG_INIT 0

#ifdef XT3
#include "locks.h"
typedef struct {
       int off;
       int desc;
} cnos_mutex_t;

static cnos_mutex_t *_mutex_array;
#endif


/*global variables and data structures */
armci_portals_proc_t _armci_portals_proc_struct;
armci_portals_proc_t *portals = &_armci_portals_proc_struct;
comp_desc *_region_compdesc_array[PORTALS_MEM_REGIONS+1];
int ptl_initialized = 0;
int free_desc_index[PORTALS_MEM_REGIONS+1];
FILE *utcp_lib_out;
FILE* utcp_api_out;
ptl_ni_limits_t armci_ptl_nilimits;

int armci_init_portals(void)
{
int num_interface;
int rc;
int npes,i;
comp_desc *armci_comp_desc;
    ARMCI_PR_DBG("enter",0);
   
    if (PtlInit(&num_interface) != PTL_OK) {
       fprintf(stderr, "PtlInit() failed\n");
       exit(1);
    }
    portals->ptl = 37; /* our own ptl number */
    for(i=0;i<=PORTALS_MEM_REGIONS;i++){
      free_desc_index[i]=0;
    }

    rc=PtlNIInit(IFACE_FROM_BRIDGE_AND_NALID(PTL_BRIDGE_UK,PTL_IFACE_SS), PTL_PID_ANY, NULL, &armci_ptl_nilimits, &(portals->ni_h)); 
    switch(rc) {
       case PTL_OK:
       case PTL_IFACE_DUP:
         break;
       default:
         printf( "PtlNIInit() failed %d error=%s\n",rc,ptl_err_str[rc]);
         armci_die("NIInit Failed",0);
    }
    if(DEBUG_INIT || DEBUG_COMM)
      PtlNIDebug(portals->ni_h,PTL_DEBUG_ALL);

    PtlGetId(portals->ni_h,&portals->ptl_my_procid);
    if(DEBUG_INIT){
      printf("%d:the rank is %d, size is %d\n",armci_me,
                      portals->ptl_my_procid,portals->size);
    }

    if ((npes = cnos_get_nidpid_map(&portals->ptl_pe_procid_map)) == -1) {
      printf(" CNOS ERROR:Getting proc id/PE map failed (npes=%d)\n", npes);
    }

    /* Allocate one shared event queue for all operations 
     * TODO tune size.
     */
    rc = PtlEQAlloc(portals->ni_h,1024,NULL, &(portals->eq_h));
    if (rc != PTL_OK) {
       printf("%d:PtlEQAlloc() failed: %s (%d)\n",
                            portals->ptl_my_procid, ptl_err_str[rc], rc);
       armci_die("EQ Alloc failed",rc);                            
    }

    ptl_initialized = 1;
    portals->num_match_entries = 0;

#ifndef XT3
    utcp_lib_out = stdout;
    utcp_api_out = stdout;
#endif

    /*now prepare for use of local memory*/ 
    armci_comp_desc = (comp_desc *)malloc(sizeof(comp_desc)*MAX_OUT); 
    for(i=0; i< MAX_OUT;i++){
      ptl_md_t *md_ptr;
      ptl_handle_md_t *md_h;
      armci_comp_desc[i].active=0;
      md_ptr = &armci_comp_desc[i].mem_dsc;
      md_h = &armci_comp_desc[i].mem_dsc_hndl;
      md_ptr->eq_handle = portals->eq_h;
      md_ptr->max_size =0;
      md_ptr->threshold = 2;/*PTL_MD_THRESH_INF;*/
      md_ptr->options =  PTL_MD_OP_GET | PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE;
    }
    _region_compdesc_array[PORTALS_MEM_REGIONS]=armci_comp_desc;
    ARMCI_PR_DBG("exit",0);
    return 0;   
}



void armci_fini_portals()
{
    ARMCI_PR_DBG("enter",0);
    if(DEBUG_INIT){
      printf("ENTERING ARMCI_FINI_PORTALS\n");fflush(stdout);
    }    
    PtlNIFini(portals->ni_h);
    /*PtlFini();*/
    if(DEBUG_INIT){
      printf("LEAVING ARMCI_FINI_PORTALS\n");fflush(stdout);    
    }
    ARMCI_PR_DBG("exit",0);
}



void armci_serv_register_req(void *start,int bytes, ARMCI_MEMHDL_T *reg_mem)
{
int rc;
void * context;
ptl_md_t *md_ptr;
ptl_match_bits_t *mb;
ptl_process_id_t match_id;
ptl_handle_md_t *md_h;
ptl_match_bits_t ignbits = 0xFFFFFFFFFFFFFFF0;

    ARMCI_PR_DBG("enter",reg_mem->regid);
    if(DEBUG_COMM){
      printf("%d:armci_serv_register_req:size of mem_hndl is %d\n", 
                      armci_me,sizeof(region_memhdl_t));
      printf("\n%d:armci_serv_register_req start=%p bytes=%d\n",
                      armci_me,start,bytes);fflush(stdout);
    }

    md_ptr = &reg_mem->cdesc.mem_dsc;
    mb = &reg_mem->match_bits; 
    md_h = &reg_mem->cdesc.mem_dsc_hndl;
    context = NULL;

    md_ptr->start = start;
    md_ptr->length = bytes;
    md_ptr->threshold = PTL_MD_THRESH_INF;
    md_ptr->options =  PTL_MD_OP_PUT | PTL_MD_OP_GET | PTL_MD_MANAGE_REMOTE;
    md_ptr->user_ptr = context;
    /*eq_hdl is null for the attaches done for a remote proc*/
    /*md_ptr->eq_handle = portals->eq_h;*/
    md_ptr->eq_handle = PTL_EQ_NONE;
    md_ptr->max_size =0;
    *mb = 0x0000000000000000;
    *mb = (*mb+reg_mem->regid);
 
    match_id.nid = PTL_NID_ANY;
    match_id.pid = PTL_PID_ANY; 

    rc = PtlMEAttach(portals->ni_h,portals->ptl,match_id,*mb,ignbits,
		     PTL_RETAIN,PTL_INS_AFTER,
		     &(portals->me_h[portals->num_match_entries])); 
  
    if (rc != PTL_OK) {
      printf("%d:PtlMEAttach: %s\n", portals->ptl_my_procid, ptl_err_str[rc]);
      armci_die("portals attach error2",rc);
    }

    rc = PtlMDAttach(portals->me_h[portals->num_match_entries],*md_ptr,PTL_RETAIN,md_h);
                    
    if (rc != PTL_OK) {
      printf("%d:PtlMDAttach: %s\n", portals->ptl_my_procid, ptl_err_str[rc]);
      armci_die("portals attach error1",rc);
    }
     
    portals->num_match_entries++;
    ARMCI_PR_DBG("exit",reg_mem->regid);
 
}

int armci_pin_contig_hndl(void *start,int bytes, ARMCI_MEMHDL_T *reg_mem)
{
int rc,i;
void * context;
ptl_md_t *md_ptr;
ptl_process_id_t match_id;
ptl_handle_md_t *md_h;
comp_desc *armci_comp_desc;

    ARMCI_PR_DBG("enter",reg_mem->regid);
   /*first create comp_desc arr for this region if it is not local*/
    if(!reg_mem->islocal){
      armci_comp_desc = (comp_desc *)malloc(sizeof(comp_desc)*MAX_OUT); 
      for(i=0; i< MAX_OUT;i++){
        armci_comp_desc[i].active=0;
        md_ptr = &armci_comp_desc[i].mem_dsc;
        md_h = &armci_comp_desc[i].mem_dsc_hndl;
        context = NULL;
        md_ptr->start = start;
        md_ptr->length = bytes;
        md_ptr->threshold = 2;/*PTL_MD_THRESH_INF;*/
        md_ptr->options =  PTL_MD_OP_GET | PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE;
        /*md_ptr->options =  PTL_MD_EVENT_START_DISABLE;*/
                      
        md_ptr->user_ptr = context;
        md_ptr->eq_handle = portals->eq_h;
        md_ptr->max_size =0;
#ifdef DO_MD_UPDATE 
        rc = PtlMDBind(portals->ni_h,*md_ptr, PTL_RETAIN, md_h);
        if (rc != PTL_OK){
          printf("%d:PtlMDBind: %s\n", portals->ptl_my_procid, ptl_err_str[rc]);
          armci_die("ptlmdbind failed",0);
        }
#endif
      }
      _region_compdesc_array[reg_mem->regid]=armci_comp_desc;
      ARMCI_PR_DBG("exit",0);
      return 1;
    }
    else {
      md_ptr = &reg_mem->cdesc.mem_dsc;
      md_h = &reg_mem->cdesc.mem_dsc_hndl;
      context = NULL;
      md_ptr->start = start;
      md_ptr->length = bytes;
      md_ptr->threshold = 2;/*PTL_MD_THRESH_INF;*/
      md_ptr->options =  PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE;
                      
      md_ptr->user_ptr = context;
      md_ptr->eq_handle = portals->eq_h;
      md_ptr->max_size =0;
#if DO_MD_UPDATE
      rc = PtlMDBind(portals->ni_h,*md_ptr, PTL_RETAIN, md_h);
      if (rc != PTL_OK){
         printf("%d:PtlMDBind: %s\n", portals->ptl_my_procid, ptl_err_str[rc]);
         armci_die("ptlmdbind failed",0);
      }
#endif
      ARMCI_PR_DBG("exit",1);
      return 1;
    }
}


int armci_client_complete(ptl_event_kind_t *evt,int proc_id, int nb_tag,
                comp_desc *cdesc)
{
int rc;  
ptl_event_t ev_t;
ptl_event_t *ev=&ev_t;
comp_desc *temp_comp = NULL;
int loop=1;
int temp_proc;
    ARMCI_PR_DBG("enter",0);
    if(DEBUG_COMM){
      printf("\n%d:enter:client_complete active=%d tag=%d %d\n",armci_me,
                      cdesc->active,cdesc->tag,nb_tag);fflush(stdout);
    }
    while(cdesc->active!=0){
      ev->type=0;
      if((rc = PtlEQWait(portals->eq_h, ev)) != PTL_OK){
        printf("%d:PtlEQWait(): %d %s\n", portals->ptl_my_procid,rc,
                        ptl_err_str[rc]); 
        armci_die("EQWait problem",rc);
      }
      if (ev->ni_fail_type != PTL_NI_OK) {
        printf("%d:NI sent %d in event.\n",
                        portals->ptl_my_procid,  ev->ni_fail_type); 
        armci_die("event failure problem",0);
      }
      if(DEBUG_COMM){
        printf("\n%d:armci_client_complete:done waiting type=%d\n",armci_me,
                        ev->type);
        fflush(stdout);
      }
      if (ev->type == PTL_EVENT_SEND_END){
        if(DEBUG_COMM){
          printf("\n%d:armci_client_complete:event send end\n",armci_me);
          fflush(stdout);
        }
        temp_comp = (comp_desc *)ev->md.user_ptr;
#ifdef PUT_LOCAL_ONLY_COMPLETION
        if(temp_comp->type==ARMCI_PORTALS_PUT || temp_comp->type=ARMCI_PORTALS_NBPUT){
          temp_comp->active=0;
          temp_comp->tag=-1;
        }
        else
#else
        temp_comp->active++;
#endif
        continue;
      }

      if (ev->type == PTL_EVENT_REPLY_END){
        if(DEBUG_COMM){
          printf("\n%d:client_send_complete:reply end\n",armci_me);
          fflush(stdout);
        }
        temp_comp = (comp_desc *)ev->md.user_ptr;
        temp_comp->active = 0; /*this was a get request, so we are done*/
        temp_comp->tag=-1;
        continue;
      }
      if (ev->type == PTL_EVENT_ACK){
        if(DEBUG_COMM){
          printf("\n%d:client_send_complete:event ack\n",armci_me);
          fflush(stdout);
        }
        temp_comp = (comp_desc *)ev->md.user_ptr;
        temp_comp->active=0;
        temp_comp->tag=-1;
        armci_update_fence_array(temp_comp->dest_id,0);              
        portals->outstanding_puts--; 
      }
    }
    if(DEBUG_COMM){
      printf("\n%d:exit:client_complete active=%d tag=%d %d\n",armci_me,
                      cdesc->active,cdesc->tag,nb_tag);fflush(stdout);
    }
    ARMCI_PR_DBG("exit",0);
    return rc; 
}


comp_desc * get_free_comp_desc(int region_id, int * comp_id)
{
comp_desc * c;     
int rc = PTL_OK;
    ARMCI_PR_DBG("enter",region_id);
    c = &(_region_compdesc_array[region_id][free_desc_index[region_id]]);
    if(c->active!=0 && c->tag>0)armci_client_complete(NULL,c->dest_id,c->tag,c);
#ifdef PUT_LOCAL_ONLY_COMPLETION
    if(region_id<PORTALS_MEM_REGIONS){
      do {
         rc = PtlMDUnlink(*md_hdl_local);
      }while(rc==PTL_MD_IN_USE);
    }
#endif
    *comp_id = (region_id*PORTALS_MEM_REGIONS+free_desc_index[region_id]);
    if(DEBUG_COMM){
      printf("the value of comp_desc_id is %d\n",*comp_id);
      fflush(stdout);
    }
    free_desc_index[region_id] = ((free_desc_index[region_id] + 1) % MAX_OUT);
    ARMCI_PR_DBG("exit",0);
    return c;
}


void print_mem_desc(ptl_md_t * md)
{
  printf("%d:md : start %p : length %d\n",armci_me,md->start, md->length);
  fflush(stdout);
        
}

void armci_client_direct_get(int proc, void *src_buf, void *dst_buf, int bytes,
                             void** cptr,int tag,ARMCI_MEMHDL_T *lochdl,
                             ARMCI_MEMHDL_T *remhdl)
{
int clus = armci_clus_id(proc);
int rc, i;
ptl_size_t offset_local = 0, offset_remote=0;
ptl_match_bits_t mb = remhdl->regid;
ptl_md_t *md_remote,md, *md_local;
ptl_md_t * md_ptr;
ptl_handle_md_t *md_hdl_local;
comp_desc *cdesc;
ptl_process_id_t dest_proc;
int c_info;
int lproc,rproc,user_memory=0;

    ARMCI_PR_DBG("enter",remhdl->regid);

    if(DEBUG_COMM){
      printf("%d:armci_client_direct_get:src_buf %p dstbuf %p loc_hd is %p\n"
                      "rem_hndl is %p, BYTES = %d\n",armci_me,src_buf,dst_buf,
                      lochdl,remhdl,bytes);
      fflush(stdout);
    }

    /*first process information*/
    dest_proc.nid = portals->ptl_pe_procid_map[proc].nid;
    dest_proc.pid = portals->ptl_pe_procid_map[proc].pid;
    md_remote =&remhdl->cdesc.mem_dsc;

    /*updating md to send*/
    if(lochdl == NULL){ /*this is user memory (stack/heap/whatever) */
      user_memory=1;
      cdesc = get_free_comp_desc(PORTALS_MEM_REGIONS,&c_info);
      md_local = &cdesc->mem_dsc;
      md_hdl_local = &cdesc->mem_dsc_hndl; 
      md_local->length=bytes;
      md_local->start=dst_buf;
    }
    else {
      if(lochdl->islocal){ /*ARMCI_Malloc_local memory*/
        user_memory=1;
#if    1
        cdesc = get_free_comp_desc(PORTALS_MEM_REGIONS,&c_info);
        md_local = &cdesc->mem_dsc;
        md_hdl_local = &cdesc->mem_dsc_hndl; 
        md_local->length=bytes;
        md_local->start=dst_buf;
#else
        cdesc=&lochdl->cdesc;
        md_local = &lochdl->cdesc.mem_dsc; 
        md_hdl_local = &lochdl->cdesc.mem_dsc_hndl; 
#endif
      }
      else{
        /*we need to pass region id to get corresponding md*/
        cdesc = get_free_comp_desc(lochdl->regid,&c_info);
        md_local = &cdesc->mem_dsc;
        md_hdl_local = &cdesc->mem_dsc_hndl; 
      }
    }

    /*compute the local and remote offsets*/ 
    offset_local = (char*)dst_buf - (char *)md_local->start;
    offset_remote = (char*)src_buf - (char *)md_remote->start;
    if(DEBUG_COMM){
      printf("\n%d:offr=%d offl=%d %p %p\n",armci_me,offset_remote,offset_local,md_local->start,md_remote->start);
    }
    /*printf("\n%d:get offr=%d ptrr=%p offl=%d ptrl=%p\n",armci_me,offset_remote,md_remote->start,offset_local,md_local->start);fflush(stdout);*/
    
    if(tag) *((comp_desc **)cptr) = cdesc;
    /*if(tag){printf("\n%d:get tag=%d c_info=%d %p",armci_me,tag,c_info,cdesc);fflush(stdout);}*/
    if (tag){
      cdesc->tag = tag;
      cdesc->dest_id = proc;
      cdesc->type = ARMCI_PORTALS_NBGET;
    }
    else{
       cdesc->tag = 0;
       cdesc->dest_id = proc;
       cdesc->type = ARMCI_PORTALS_GET; 
    }
    cdesc->active = 1;
    md_local->user_ptr = (void *)cdesc;
    md_local->options =  PTL_MD_OP_GET | PTL_MD_EVENT_START_DISABLE;
#if DO_MD_UPDATE
    if(user_memory==0){
      do{
        rc = PtlMDUpdate(*md_hdl_local,NULL,md_local,portals->eq_h);
        printf("\n%d:trying to update\n",armci_me);fflush(stdout);
      } while (rc == PTL_MD_NO_UPDATE);
      if (rc != PTL_OK){
         printf("%d:PtlMDUpdate: %s\n", portals->rank, ptl_err_str[rc]);
         armci_die("ptlmdbind failed",0);
      }
    }
    else{
#endif
      rc = PtlMDBind(portals->ni_h,*md_local, PTL_UNLINK, md_hdl_local);
      if (rc != PTL_OK){
         fprintf(stderr, "%d:PtlMDBind: %s\n", portals->rank, ptl_err_str[rc]);
         armci_die("ptlmdbind failed",0);
      }
#if DO_MD_UPDATE
    }
#endif

    rc = PtlGetRegion(*md_hdl_local,offset_local,bytes,dest_proc,
                   portals->ptl,
                   0, 
                   mb,
                   offset_remote);
    if (rc != PTL_OK){
      printf("%d:PtlGetRegion: %s\n", portals->rank,ptl_err_str[rc]);
      armci_die("PtlGetRegion failed",0); 
    }

    if(DEBUG_COMM){
      printf("\n%d:issued get to %d %d\n",armci_me,proc,c_info);fflush(stdout);
    }

    if(!tag){ 
       armci_client_complete(NULL,proc,0,cdesc); /* check this later */
    }
    /*printf("\n%d:issued get to %d %d\n",armci_me,proc,c_info);fflush(stdout);*/
    ARMCI_PR_DBG("exit",remhdl->regid);
}


void armci_client_nb_get(int proc, void *src_buf, int *src_stride_arr, 
                             void *dst_buf, int *dst_stride_arr, int bytes,
                             void** cptr,int tag,ARMCI_MEMHDL_T *lochdl,
                             ARMCI_MEMHDL_T *remhdl)
{
}


int armci_client_direct_send(int proc,void *src, void* dst, int bytes,  
                void **cptr, int tag, ARMCI_MEMHDL_T *lochdl, 
                ARMCI_MEMHDL_T *remhdl )
{
int clus = armci_clus_id(proc);    
int rc, i;
ptl_size_t offset_local = 0, offset_remote = 0;
ptl_match_bits_t mb = remhdl->regid;
ptl_md_t *md_remote,md, *md_local;
ptl_md_t * md_ptr;
ptl_match_bits_t * mb_ptr;
ptl_handle_md_t *md_hdl_local;
comp_desc *cdesc;
ptl_process_id_t dest_proc;
int c_info;
int lproc,rproc,user_memory=0;

    ARMCI_PR_DBG("enter",remhdl->regid);
    dest_proc.nid = portals->ptl_pe_procid_map[proc].nid;
    dest_proc.pid = portals->ptl_pe_procid_map[proc].pid;
    md_remote =&remhdl->cdesc.mem_dsc;

    if(lochdl == NULL){ /*this is user memory*/
      user_memory=1;
      cdesc = get_free_comp_desc(PORTALS_MEM_REGIONS,&c_info);
      md_local = &cdesc->mem_dsc;
      md_hdl_local = &cdesc->mem_dsc_hndl; 
      md_local->length=bytes;
      md_local->start=src;
    }
    else {
      if(lochdl->islocal){ /*ARMCI_Malloc_local memory*/
        user_memory=1;
#if    1
        cdesc = get_free_comp_desc(PORTALS_MEM_REGIONS,&c_info);
        md_local = &cdesc->mem_dsc;
        md_hdl_local = &cdesc->mem_dsc_hndl; 
        md_local->length=bytes;
        md_local->start=src;
#else
        cdesc=&lochdl->cdesc;
        md_local = &lochdl->cdesc.mem_dsc; 
        md_hdl_local = &lochdl->cdesc.mem_dsc_hndl;
#endif
      }
      else{
        /*we need to pass region id to get corresponding md*/
        cdesc = get_free_comp_desc(lochdl->regid,&c_info);
        md_local = &cdesc->mem_dsc;
        md_hdl_local = &cdesc->mem_dsc_hndl; 

      }
    }
    
    offset_local = (char *)src - (char *)md_local->start;
    offset_remote =(char *)dst - (char *)md_remote->start;
    if(DEBUG_COMM){
      printf("\n%d:offr=%d offl=%d\n",armci_me,offset_remote,offset_local);
    }
    /*printf("\n%d:offr=%d ptrr=%p offl=%d ptrl=%p\n",armci_me,offset_remote,md_remote->start,offset_local,md_local->start);fflush(stdout);*/

    if(tag) *((comp_desc **)cptr) = cdesc; /*TOED*/
    /*
    if(tag){printf("\n%d:put tag=%d c_info=%d %p",armci_me,tag,c_info,cdesc);fflush(stdout);}
    */
    if (tag){
      cdesc->tag = tag;
      cdesc->dest_id = proc;
      cdesc->type = ARMCI_PORTALS_NBPUT;
    }
    else{
      cdesc->tag = 0;
      cdesc->dest_id = proc;
      cdesc->type = ARMCI_PORTALS_PUT; 
    }
#ifdef PUT_LOCAL_COMPLETION_ONLY
    cdesc->active = 2;
#else
    cdesc->active = 1;
#endif

    md_local->user_ptr = (void *)cdesc;
    md_local->options =  PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE;
#if DO_MD_UPDATE
    if(user_memory==0){
      do{
        rc = PtlMDUpdate(*md_hdl_local,NULL,md_local,portals->eq_h);
      } while (rc == PTL_MD_NO_UPDATE);
      if (rc != PTL_OK){
         printf("%d:PtlMDUpdate: %s\n", portals->rank, ptl_err_str[rc]);
         armci_die("ptlmdupdate failed",0);
      }
    }
    else{
#endif
      rc = PtlMDBind(portals->ni_h,*md_local, PTL_UNLINK, md_hdl_local);
      if (rc != PTL_OK){
         fprintf(stderr, "%d:PtlMDBind: %s\n", portals->rank, ptl_err_str[rc]);
         armci_die("ptlmdbind failed",0);
      }
#if DO_MD_UPDATE
    }
#endif
    
    rc = PtlPutRegion(*md_hdl_local,offset_local,bytes,
#ifdef PUT_LOCAL_COMPLETION_ONLY
                    PTL_NOACK_REQ,
#else
                    PTL_ACK_REQ,
#endif
                    dest_proc,
                   portals->ptl,
                   0, mb,offset_remote, 0);
    if (rc != PTL_OK){
      fprintf(stderr, "%d:PtlPutRegion: %s\n", portals->rank,ptl_err_str[rc]);
      armci_die("PtlPutRegion failed",0);
    }
    if(DEBUG_COMM){
      printf("\n%d:issued put to %d\n",armci_me,proc);fflush(stdout);
    }

    armci_update_fence_array(proc, 1);
    if(!tag){
       armci_client_complete(NULL,proc,0,cdesc); /* check this later */
    }
    else
       portals->outstanding_puts++;   
    ARMCI_PR_DBG("exit",remhdl->regid);
    return rc;
}

void armci_client_nb_send(int proc, void *src_buf, int *src_stride_arr, 
                             void *dst_buf, int *dst_stride_arr, int bytes,
                             void** cptr,int tag,ARMCI_MEMHDL_T *lochdl,
                             ARMCI_MEMHDL_T *remhdl)
{
}
#if 1 
/*using non-blocking for multiple 1ds inside a 2d*/
void armci_network_strided(int op, void* scale, int proc,void *src_ptr,
                   int src_stride_arr[], void* dst_ptr, int dst_stride_arr[],
                   int count[], int stride_levels, armci_ihdl_t nb_handle)
{
int i, j,tag=0;
long idxs,idxd;    /* index offset of current block position to ptr */
int n1dim;  /* number of 1 dim block */
int bvalues[MAX_STRIDE_LEVEL], bunit[MAX_STRIDE_LEVEL];
int bvalued[MAX_STRIDE_LEVEL];
int bytes = count[0];
void *loc, *rem;
void *sptr,*dptr;
ARMCI_MEMHDL_T *loc_memhdl=NULL,*rem_memhdl=NULL; 
NB_CMPL_T cptr;
int armci_region_both_found_hndl(void *loc, void *rem, int size, int node,
                ARMCI_MEMHDL_T **loc_memhdl,ARMCI_MEMHDL_T **rem_memhdl);
    if(nb_handle)tag=nb_handle->tag;
    if(op==GET){
      loc = dst_ptr;
      rem = src_ptr;  
    }
    else {
      loc = src_ptr;
      rem = dst_ptr;  
    }
    armci_region_both_found_hndl(loc,rem,bytes,armci_clus_id(proc),
                 &loc_memhdl,&rem_memhdl);
    /* number of n-element of the first dimension */
    n1dim = 1;
    for(i=1; i<=stride_levels; i++)
        n1dim *= count[i];
    /* calculate the destination indices */
    bvalues[0] = 0; bvalues[1] = 0; bunit[0] = 1; 
    bvalued[0] = 0; bvalued[1] = 0; bunit[1] = 1;
    for(i=2; i<=stride_levels; i++) {
        bvalues[i] = bvalued[i] = 0;
        bunit[i] = bunit[i-1] * count[i-1];
    }
    if(ACC(op)){
      /*lock here*/
            printf("\nSHOULD NOT DO NETWORK_STRIDED FOR ACCS \n",armci_me);
            fflush(stdout);
            armci_die("network_strided called for acc",proc);
    }
    for(i=0; i<n1dim; i++) {
      tag = GET_NEXT_NBTAG();      
      idxs = 0;
      idxd = 0;
      for(j=1; j<=stride_levels; j++) {
        idxs += bvalues[j] * src_stride_arr[j-1];
        idxd += bvalued[j] * dst_stride_arr[j-1];
        if((i+1) % bunit[j] == 0) {bvalues[j]++;bvalued[j]++;}
        if(bvalues[j] > (count[j]-1)) bvalues[j] = 0;
        if(bvalued[j] > (count[j]-1)) bvalued[j] = 0;
      }

      sptr = ((char *)src_ptr)+idxs;
      dptr = ((char *)dst_ptr)+idxd;
      if(op==GET){
        armci_client_direct_get(proc,sptr,dptr,bytes,&cptr,tag,loc_memhdl,
                             rem_memhdl);
      }
      else if(op==PUT){
          armci_client_direct_send(proc,sptr,dptr,bytes,&cptr,tag,loc_memhdl,
                             rem_memhdl);
      }
      else if(ACC(op)){
          armci_client_direct_get(proc,sptr,dptr,bytes,&cptr,tag,loc_memhdl,
                             rem_memhdl);
          /*DO ACC*/
          armci_client_direct_send(proc,sptr,dptr,bytes,&cptr,tag,loc_memhdl,
                             rem_memhdl);
      }
      else
          armci_die("in network_strided unknown opcode",op);
    }
    if(ACC(op)){
    /*unlock here*/
    }
    if(nb_handle){
      nb_handle->tag=tag;
      nb_handle->cmpl_info=cptr;
    }
    else{
      armci_client_complete(NULL,proc,tag,cptr); /* check this later */
    }
}                  
#else /*using blocking for multiple 1ds inside a 2d*/
void armci_network_strided(int op, void* scale, int proc,void *src_ptr,
                   int src_stride_arr[], void* dst_ptr, int dst_stride_arr[],
                   int count[], int stride_levels, armci_ihdl_t nb_handle)
{
    int i, j;
    long idxs,idxd;    /* index offset of current block position to ptr */
    int n1dim;  /* number of 1 dim block */
    int bvalues[MAX_STRIDE_LEVEL], bunit[MAX_STRIDE_LEVEL];
    int bvalued[MAX_STRIDE_LEVEL];
    int bytes = count[0];
    void *loc, *rem;
    void *sptr,*dptr;
#if 0
    ARMCI_MEMHDL_T *loc_memhdl=NULL,*rem_memhdl=NULL; 
    int armci_region_both_found_hndl(void *loc, void *rem, int size, int node,
                 ARMCI_MEMHDL_T **loc_memhdl,ARMCI_MEMHDL_T **rem_memhdl);
#endif
    if(op==GET){
      loc = dst_ptr;
      rem = src_ptr;  
    }
    else {
      loc = src_ptr;
      rem = dst_ptr;  
    }
#if 0
    armci_region_both_found_hndl(loc,rem,count[0],armci_clus_id(proc),
                 &loc_memhdl,&rem_memhdl);
#endif
    /* number of n-element of the first dimension */
    n1dim = 1;
    for(i=1; i<=stride_levels; i++)
        n1dim *= count[i];
    
    /* calculate the destination indices */
    bvalues[0] = 0; bvalues[1] = 0; bunit[0] = 1; 
    bvalued[0] = 0; bvalued[1] = 0; bunit[1] = 1;
    for(i=2; i<=stride_levels; i++) {
        bvalues[i] = bvalued[i] = 0;
        bunit[i] = bunit[i-1] * count[i-1];
    }

    for(i=0; i<n1dim; i++) {
        idxs = 0;
        idxd = 0;
        for(j=1; j<=stride_levels; j++) {
            idxs += bvalues[j] * src_stride_arr[j-1];
            idxd += bvalued[j] * dst_stride_arr[j-1];
            if((i+1) % bunit[j] == 0) {bvalues[j]++;bvalued[j]++;}
            if(bvalues[j] > (count[j]-1)) bvalues[j] = 0;
            if(bvalued[j] > (count[j]-1)) bvalued[j] = 0;
        }

	sptr = ((char *)src_ptr)+idxs;
	dptr = ((char *)dst_ptr)+idxd;
	if((i<(n1dim-1)) || nb_handle==NULL){
	  if(op==GET)
            ARMCI_Get(sptr,dptr,bytes,proc);
	  else if(op==PUT)
            ARMCI_Put(sptr,dptr,bytes,proc);
	  else if(ACC(op))
            ARMCI_AccS(op,scale,sptr,NULL,dptr,NULL,count,1,proc);
	  else
            armci_die("in network_strided unknown opcode",op);
	}
    }
    if(nb_handle!=NULL){
      if(op==GET)
        ARMCI_NbGet(sptr,dptr,bytes,proc,(armci_hdl_t *)nb_handle);
      else if(op==PUT)
        ARMCI_NbPut(sptr,dptr,bytes,proc,(armci_hdl_t *)nb_handle);
      else if(ACC(op))
        ARMCI_NbAccS(op,scale,sptr,NULL,dptr,NULL,count,1,proc,(armci_hdl_t *)nb_handle);
      else
        armci_die("in network_strided unknown opcode",op);
    }
}
#endif

int armci_client_direct_getput(int proc,void *getinto, void *putfrom, void* dst,
                int bytes, void **cptr, int tag, ARMCI_MEMHDL_T *lochdl, 
                ARMCI_MEMHDL_T *remhdl )
                
{
int clus = armci_clus_id(proc);    
int rc, i;
ptl_size_t offset_get = 0, offset_put=0, offset_remote = 0;
ptl_match_bits_t mb = 100;
ptl_md_t *md_remote,md, *md_local_put, *md_local_get;
ptl_md_t * md_ptr;
ptl_match_bits_t * mb_ptr;
ptl_handle_md_t *md_hdl_local_put,*md_hdl_local_get;
comp_desc *cdesc;
ptl_process_id_t dest_proc;
int c_info;
int lproc,rproc;
    printf("\n%d:****************getput*********\n",armci_me);
    dest_proc.nid = portals->ptl_pe_procid_map[proc].nid;
    dest_proc.pid = portals->ptl_pe_procid_map[proc].pid;
    md_remote =&remhdl->cdesc.mem_dsc;

    cdesc = get_free_comp_desc(PORTALS_MEM_REGIONS,&c_info);
    md_local_get = &cdesc->mem_dsc;
    md_hdl_local_get = &cdesc->mem_dsc_hndl; 
    md_local_get->length=bytes;
    md_local_get->start=getinto;
    offset_get = (char *)getinto - (char *)md_local_get->start;
    offset_remote =(char *)dst - (char *)md_remote->start;
    cdesc->tag = 999999;
    cdesc->dest_id = proc;
    cdesc->type = ARMCI_PORTALS_PUT;
    cdesc->active = 0;
    md_local_get->user_ptr = (void *)cdesc;
    md_local_get->options =  PTL_MD_OP_GET | PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE | PTL_MD_EVENT_END_DISABLE;
    rc = PtlMDBind(portals->ni_h,*md_local_get, PTL_UNLINK, md_hdl_local_get);
    if (rc != PTL_OK){
      fprintf(stderr, "%d:PtlMDBind: %s\n", portals->rank, ptl_err_str[rc]);
      armci_die("ptlmdbind failed",0);
    }

    cdesc = get_free_comp_desc(PORTALS_MEM_REGIONS,&c_info);
    md_local_put = &cdesc->mem_dsc;
    md_hdl_local_put = &cdesc->mem_dsc_hndl; 
    md_local_put->length=bytes;
    md_local_put->start=putfrom;
    offset_put = (char *)putfrom - (char *)md_local_put->start;
    cdesc->tag = 999999;
    cdesc->dest_id = proc;
    cdesc->type = ARMCI_PORTALS_GET;
    cdesc->active = 0;
    md_local_put->user_ptr = (void *)cdesc;
    md_local_put->options =  PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE;
    rc = PtlMDBind(portals->ni_h,*md_local_put, PTL_UNLINK, md_hdl_local_put);
    if (rc != PTL_OK){
      fprintf(stderr, "%d:PtlMDBind: %s\n", portals->rank, ptl_err_str[rc]);
      armci_die("ptlmdbind failed",0);
    }
    
    rc = PtlGetPutRegion(*md_hdl_local_get,offset_get,*md_hdl_local_put,
                    offset_put,bytes,dest_proc, portals->ptl,0,mb,offset_remote,
                    0);
    if (rc != PTL_OK){
      printf("%d:PtlPutRegion: %s\n", portals->rank,ptl_err_str[rc]);
      armci_die("PtlPutRegion failed",0);
    }
    if(DEBUG_COMM){
      printf("\n%d:issued getput to %d\n",armci_me,proc);fflush(stdout);
    }

    armci_client_complete(NULL,proc,0,cdesc); /* check this later */
    return rc;
}

void armci_network_client_deregister_memory(ARMCI_MEMHDL_T *mh)
{
}


void armci_network_server_deregister_memory(ARMCI_MEMHDL_T *mh)
{
}

#ifdef XT3_
static int num_locks=0;
static long **all_locks;
#define ARMCI_PORTALS_MAX_LOCKS 16
typedef struct {
  ptl_handle_md_t mem_dsc_h;
  ptl_handle_me_t me_lock_h;
  region_memhdl_t armci_portal_lock_memhdl;
} armci_lock_struct;
armci_lock_struct armci_portals_lock_st;
void armcill_allocate_locks(int num)
{
ptl_md_t *md_ptr;
ptl_match_bits_t *mb;
ptl_process_id_t match_id;
ptl_handle_md_t *md_h;
int ace_any=1;
int rc;
long *my_locks;
int elems;
armci_lock_struct *armci_portals_lock=&armci_portals_lock_st;

    num_locks = num;
    if(DEBUG_COMM){
      printf("%d:armci_allocate_locks num=%d\n",
                      armci_me,num_locks);
    }
    if(MAX_LOCKS<num)armci_die2("too many locks",ARMCI_PORTALS_MAX_LOCKS,num);

   /* allocate memory to hold lock info for all the processors */
   all_locks = malloc(armci_nproc*sizeof(long *));
   if(!all_locks) armci_die("armcill_init_locks: malloc failed",0);
   bzero(all_locks,armci_nproc*sizeof(long));
   /* initialize local locks */
   my_locks = malloc(num*sizeof(long));
   if(!my_locks) armci_die("armcill_init_locks: malloc failed",0);
   bzero(all_locks,armci_nproc*sizeof(long));

   all_locks[armci_me]=my_locks;
   /* now we use all-reduce to exchange locks info among everybody */
   elems = armci_nproc;
   armci_exchange_address((void **)all_locks,elems);
#if 0
    rc = PtlACEntry(portals->ni_h, ace_any,
                    (ptl_process_id_t){PTL_NID_ANY, PTL_PID_ANY},
                    PTL_PID_ANY, 128);
    if (rc != PTL_OK) {
      printf("%d: PtlACEntry() failed: %s\n",
           armci_me, ptl_err_str[rc]);
      armci_die("PtlACEntry failed",0);
    }
#endif
    armci_portals_lock->armci_portal_lock_memhdl.cdesc.mem_dsc.start =&my_locks;
    armci_portals_lock->armci_portal_lock_memhdl.cdesc.mem_dsc.length = 
            sizeof(my_locks);
    armci_portals_lock->armci_portal_lock_memhdl.cdesc.mem_dsc.threshold = 
            PTL_MD_THRESH_INF;
    armci_portals_lock->armci_portal_lock_memhdl.cdesc.mem_dsc.options =
                PTL_MD_OP_PUT | PTL_MD_OP_GET |
                PTL_MD_MANAGE_REMOTE | PTL_MD_TRUNCATE |
                PTL_MD_EVENT_START_DISABLE;
    armci_portals_lock->armci_portal_lock_memhdl.cdesc.mem_dsc.max_size = 0;
    armci_portals_lock->armci_portal_lock_memhdl.cdesc.mem_dsc.user_ptr = NULL;
    armci_portals_lock->armci_portal_lock_memhdl.cdesc.mem_dsc.eq_handle = 
            PTL_EQ_NONE;
    match_id.nid = PTL_NID_ANY;
    match_id.pid = PTL_PID_ANY;

    /* Lockmaster needs a match entry for clients to access lock value. 
    */
    rc = PtlMEAttach(portals->ni_h, portals->ptl,
                         match_id,      /* source address */
                         100,           /* expected match bits */
                         0,             /* ignore bits to mask */
                         PTL_RETAIN,    /* unlink when md is unlinked */
                         PTL_INS_AFTER,
                         &armci_portals_lock->me_lock_h);
    if (rc != PTL_OK){
      printf("%d: PtlMEAttach(): %s\n",
                        armci_me, ptl_err_str[rc]);
      armci_die("PtlMEAttach in init_locks failed",0);
    }
    rc = PtlMDAttach(armci_portals_lock->me_lock_h, 
                 armci_portals_lock->armci_portal_lock_memhdl.cdesc.mem_dsc,
                 PTL_RETAIN, 
                 &armci_portals_lock->mem_dsc_h);
    if (rc != PTL_OK) {
      printf("%d: PtlMDAttach(): %s\n",
                        armci_me, ptl_err_str[rc]);
      armci_die("PtlMDAttach in init_locks failed",0);
    }
}

void armcill_lock(int mutex, int proc)
{
long getinto=0,putfrom=1;
armci_lock_struct *armci_portal_lock=&armci_portals_lock_st;
region_memhdl_t *rem_lock_hdl=&armci_portal_lock->armci_portal_lock_memhdl;
printf("\n%d:in lock before\n",armci_me);fflush(stdout);
    do{
      armci_client_direct_getput(proc,&getinto,&putfrom,(all_locks[proc]+mutex),
                sizeof(long), NULL, 0, NULL,rem_lock_hdl);
    }while(getinto!=0);
printf("\n%d:in lock after\n",armci_me);fflush(stdout);
}


/*\ unlock specified mutex on node where process proc is running
\*/
void armcill_unlock(int mutex, int proc)
{
long getinto=0,putfrom=0;
armci_lock_struct *armci_portal_lock=&armci_portals_lock_st;
region_memhdl_t *rem_lock_hdl=&armci_portal_lock->armci_portal_lock_memhdl;
    armci_client_direct_getput(proc,&getinto,&putfrom,(all_locks[proc]+mutex),
              sizeof(long), NULL, 0, NULL,rem_lock_hdl);
}

int armci_portals_rmw_(int op, int *ploc, int *prem, int extra, int proc)
{
    return(0);
}
#endif

void armci_portals_shmalloc_allocate_mem(int num_lks)
{
    void **ptr_arr;
    void *ptr;
    armci_size_t bytes = 128;
    int i;    
    
    ptr_arr    = (void**)malloc(armci_nproc*sizeof(void*));
    if(!ptr_arr) armci_die("armci_shmalloc_get_offsets: malloc failed", 0);
    bzero((char*)ptr_arr,armci_nproc*sizeof(void*));

    ARMCI_Malloc(ptr_arr,bytes);
    
    return;
}

/* preliminary implementation on top of portals */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "armcip.h"
#include <stdint.h>

//#define DEBUG 

/*global variables and data structures */
armci_portals_proc_t _armci_portals_proc_struct;
armci_portals_proc_t *portals = &_armci_portals_proc_struct;
md_table_entry_t _armci_md_table[MAX_ENT];
comp_desc _armci_portals_comp[MAX_OUT];
int ptl_initialized = 0;
int free_desc_index = 0;
FILE *utcp_lib_out;
FILE* utcp_api_out;


void print_mem_desc_table()
{
int i;
    for (i = 0; i<portals->num_match_entries;i++){
       printf ("%d: i=%d print_mem match_ent=%d,start:%p, end:%p, bytes:%d,
                       %p-md.length=%llu, %p-md.start=%p\n",  
		       portals->rank,i,portals->num_match_entries,
                       _armci_md_table[i].start,
		       _armci_md_table[i].end, 
                       _armci_md_table[i].bytes,
                       &(_armci_md_table[i].md.length),
		       _armci_md_table[i].md.length,  
                       &(_armci_md_table[i].md.start),
                       _armci_md_table[i].md.start);
       fflush(stdout);
  }       
}


void comp_desc_init()
{
  int i;
  comp_desc *c;
  for(i=0; i< MAX_OUT;i++){
      c = &(_armci_portals_comp[i]);
      c->active = 0;
  }
}



int armci_init_portals(void)
{
    int num_interface;
    int rc;
   
    if (PtlInit(&num_interface) != PTL_OK) {
       fprintf(stderr, "PtlInit() failed\n");
       exit(1);
    }
    portals->ptl = 4;

    /* should this be PTL_PID_ANY */ 
    if(PtlNIInit(PTL_IFACE_DEFAULT, PTL_PID_ANY, NULL, NULL, &(portals->ni_h)) != PTL_OK){
       fprintf(stderr, "PtlNIInit() failed\n");
       exit(EXIT_FAILURE);
    }

    if(PtlGetRank(&(portals->rank), &(portals->size)) != PTL_OK) {
       fprintf(stderr, "PtlGetRank() failed\n");
       exit(EXIT_FAILURE);
    }

#ifdef DEBUG
    printf("the rank is %d, size is %d\n",portals->rank,portals->size);
    fflush(stdout);
#endif
    
    rc = PtlEQAlloc(portals->ni_h,64, PTL_EQ_HANDLER_NONE, &(portals->eq_h));
    if (rc != PTL_OK) {
       fprintf(stderr, "%d:PtlEQAlloc() failed: %s (%d)\n",
                            portals->rank, PtlErrorStr(rc), rc);
       exit(EXIT_FAILURE);                            
    }
    ptl_initialized = 1;
    portals->num_match_entries = 0;
    comp_desc_init();
    utcp_lib_out = stdout;
    utcp_api_out = stdout;
    return 0;   
}



void armci_fini_portals()
{
    printf("ENTERING ARMCI_FINI_PORTALS\n");fflush(stdout);    
    PtlNIFini(portals->ni_h);
    PtlFini();
    printf("LEAVING ARMCI_FINI_PORTALS\n");fflush(stdout);    
    
}



int armci_pin_contig_hndl(void *start,int bytes, ARMCI_MEMHDL_T *reg_mem)
{
    int rc;
    void * context;
    ptl_md_t *md_ptr;
    ptl_match_bits_t *mb;
    ptl_process_id_t match_id;

#ifdef DEBUG
    printf("inside portals.c : size of mem_hndl is %d\n", sizeof(region_memhdl_t));
#endif
    
    md_ptr = &reg_mem->mem_dsc;
    mb = &reg_mem->match_bits; 
    context = NULL;
    md_ptr->start = start;
    md_ptr->length = bytes;
    md_ptr->threshold = PTL_MD_THRESH_INF;
    md_ptr->options =  PTL_MD_OP_PUT | PTL_MD_OP_GET | PTL_MD_EVENT_START_DISABLE | PTL_MD_MANAGE_REMOTE;
    md_ptr->user_ptr = context;
    md_ptr->eq_handle = portals->eq_h;
    md_ptr->max_size =0;
    *mb = 100;//+portals->num_match_entries; (let us make it 100)

#ifdef DEBUG
    printf("dont with pincontig\n");
    fflush(stdout);
#endif

    return(1);
}

void armci_serv_register_req(void *start,int bytes, ARMCI_MEMHDL_T *reg_mem)
{
   int rc;
   void * context;
   ptl_md_t *md_ptr;
   ptl_match_bits_t *mb;
   ptl_process_id_t match_id;

#ifdef DEBUG
    printf("inside portals.c : size of mem_hndl is %d\n", sizeof(region_memhdl_t));
    printf("\n%d:armci_serv_register_req start=%p bytes=%d",armci_me,start,bytes);fflush(stdout);
#endif
    
   md_ptr = &reg_mem->mem_dsc;
   mb = &reg_mem->match_bits; 
   context = NULL;
   md_ptr->start = start;
   md_ptr->length = bytes;
   md_ptr->threshold = PTL_MD_THRESH_INF;
   md_ptr->options =  PTL_MD_OP_PUT | PTL_MD_OP_GET | PTL_MD_EVENT_START_DISABLE 
                      | PTL_MD_MANAGE_REMOTE;
   md_ptr->user_ptr = context;
   md_ptr->eq_handle = portals->eq_h;
   md_ptr->max_size =0;
   *mb = 100; //+portals->num_match_entries;
 
   match_id.nid = PTL_NID_ANY;
   match_id.pid = PTL_PID_ANY; 

#ifdef DEBUG
    printf("about to call attach\n");
    fflush(stdout);
#endif

    if(portals->num_match_entries == 0){
       rc = PtlMEAttach(portals->ni_h,portals->ptl,match_id,*mb,0,
		       PTL_RETAIN,PTL_INS_AFTER,
		       &(portals->me_h[portals->num_match_entries])); 
                    
    }
    else{
       rc = PtlMEInsert(portals->me_h[(portals->num_match_entries - 1)],
		       match_id,*mb,0, PTL_RETAIN,PTL_INS_AFTER,
		       &portals->me_h[(portals->num_match_entries)]);
                     
   }
  
   if (rc != PTL_OK) {
      fprintf(stderr, "%d:PtlMEAttach: %s\n", portals->rank, PtlErrorStr(rc));
      exit(EXIT_FAILURE); 
   }

#ifdef DEBUG
   printf("about to call md attach: the md_h is %p\n",&(portals->md_h[portals->num_match_entries]));
   fflush(stdout);
#endif
   if(portals->num_match_entries == 0){
      rc = PtlMDAttach(portals->me_h[portals->num_match_entries],*md_ptr,PTL_RETAIN,
                    &(portals->md_h[portals->num_match_entries]));
     if (rc != PTL_OK) {
         fprintf(stderr, "%d:PtlMDAttach: %s\n", portals->rank, PtlErrorStr(rc)); 
         exit(EXIT_FAILURE);
     }
     
#ifdef DEBUG
     printf("%d: ,finished attach\n",portals->rank);
     fflush(stdout);
#endif
  }  

  else{ 
     rc = PtlMDAttach(portals->me_h[(portals->num_match_entries)],*md_ptr,
                     PTL_RETAIN,&(portals->md_h[(portals->num_match_entries)]));
     if (rc != PTL_OK) {
         fprintf(stderr, "%d:PtlMDAttach: %s\n", portals->rank, PtlErrorStr(rc)); 
         exit(EXIT_FAILURE);
     } 
  }  

  
  portals->num_match_entries++;
  //print_mem_desc_table(); 
#ifdef DEBUG
  printf("%d:in prepost i=%d %p-start=%p,%p-length=%llu table.eq_handle = %u\n",
                portals->rank,(portals->num_match_entries - 1),
                &(_armci_md_table[portals->num_match_entries - 1].md.start),
                 _armci_md_table[portals->num_match_entries - 1].md.start,
                &(_armci_md_table[portals->num_match_entries - 1].md.length),
                 (_armci_md_table[portals->num_match_entries - 1].md.length),
                _armci_md_table[portals->num_match_entries - 1].md.eq_handle);
  fflush(stdout); 
#endif
}



int armci_client_complete(ptl_event_kind_t *evt,int proc_id, int nb_tag,comp_desc *cdesc,int b_tag )
{
  int rc;  
  ptl_event_t *ev; // = NULL;
  /*armci_ihdl_t nb_handle;*/
  comp_desc *temp_comp = NULL;
  int temp_tag;
  int loop = 1;
  int temp_proc;

#ifdef DEBUG
  printf("entering armci_client_complete\n");
  fflush(stdout);
#endif

    while(loop){
        
        if(cdesc)
        {        
                if(cdesc->active == 2 || cdesc->active == 3)
                {
                        cdesc->active = 0;
                        loop = 0;
                        break;        
                }
        }   
            
       if((rc = PtlEQWait(portals->eq_h, ev)) != PTL_OK){
         fprintf(stderr, "%d:PtlEQWait(): %s\n", portals->rank, PtlErrorStr(rc));
         exit(EXIT_FAILURE);
       }
    
       if (ev->ni_fail_type != PTL_NI_OK) {
         fprintf(stderr, "%d:NI sent %s in event.\n",
                         portals->rank, PtlNIFailStr(portals->ni_h, 
                                 ev->ni_fail_type));
         exit(EXIT_FAILURE);
       }

       /* handle the corresponding event */
       if (ev->type == PTL_EVENT_SEND_END){
#ifdef DEBUG
         printf ("INSIDED PTL_EVENT_SEND_END\n");
         fflush(stdout);      
#endif               
         temp_comp = (comp_desc *)ev->md.user_ptr;
         temp_tag = temp_comp->tag;
         temp_proc = temp_comp->dest_id;
         temp_comp->active = 1;
         if ((nb_tag != 0) && (nb_tag == temp_tag))
           break;
         else if ((b_tag == 1) && !(cdesc))// && (cdesc == temp_comp) ) 
         {
               loop = 0;
#ifdef DEBUG
               printf("finished receiving event poll\n");
               fflush(stdout);
#endif
               break;
                              
         }      
       }

       if (ev->type == PTL_EVENT_REPLY_END){
#ifdef DEBUG
         printf ("INSIDED PTL_EVENT_REPLY_END\n");
         fflush(stdout);  
#endif    
         temp_comp = (comp_desc *)ev->md.user_ptr;
         temp_tag = temp_comp->tag;
         temp_proc = temp_comp->dest_id;
         temp_comp->active = 3;
         if ((nb_tag != 0) && (nb_tag == temp_tag))
           break;
         else if ((b_tag == 1) && !(cdesc))// && (cdesc == temp_comp) )
          {
                  temp_comp->active = 0;
                  loop = 0; 
                  
#ifdef DEBUG
                  printf("breaking from event poll\n");
                  fflush(stdout); 
#endif
                  
                  break;
          }
       } 
         
        if (ev->type == PTL_EVENT_ACK){
#ifdef DEBUG
                printf ("INSIDED PTL_EVENT_ACK\n");
                fflush(stdout);      
#endif
                temp_comp = (comp_desc *)ev->md.user_ptr;
                temp_proc = temp_comp->dest_id;
                temp_comp->active = 2;
                armci_update_fence_array(temp_proc,0);              
                portals->outstanding_puts--; 
                if((evt != NULL) && (evt == PTL_EVENT_ACK))   
                {
                   if (cdesc == NULL)            
                        break;
                }
        }

        if ( cdesc && (temp_comp == cdesc)){
             if(cdesc->active == 2 || cdesc->active == 3)
                cdesc->active = 0;
                break;        
        }
        
        
   }
  return rc; 
  
}


#if 0

/* XXX: need to add code in regions to exchange base ptr and size of each allocated region */
ptl_size_t armci_get_offset(ptl_md_t md, void *ptr, int proc)
{
  void * start_address;
  ptl_size_t offset;
  start_address = md.start;
  offset =  (char *)ptr - (char *)start_address;

#ifdef DEBUG
  printf("%d: start is %p, md.start is %p , offset is %llu\n", portals->rank,
                  ptr, start_address,offset);
  fflush(stdout);
#endif 
  return offset;
}




int armci_portals_put(ptl_handle_md_t md_h,ptl_process_id_t dest_id,
                int bytes,int mb,int local_offset, int remote_offset,int ack )
{
     int rc;
     rc = PtlPutRegion(md_h, local_offset, bytes,ack,dest_id, portals->ptl, 0, mb,remote_offset, 0);
     if (rc != PTL_OK) {
             fprintf(stderr, "%d:PtlPutRegion: %s\n", portals->rank, PtlErrorStr(rc));
             exit(EXIT_FAILURE); 
     }
     return rc;
     
}

int armci_portals_get(ptl_handle_md_t md_h,ptl_process_id_t dest_id,int bytes,
                int mb,int local_offset, int remote_offset)
{
     int rc;
     printf("%d: about to call ptl_get, dest_id is %d, md_handle is %p\n",portals->rank,dest_id, md_h);
     fflush(stdout);
     rc = PtlGetRegion(md_h, local_offset, bytes,dest_id, portals->ptl, 0, mb,remote_offset);
     if (rc != PTL_OK) {
             fprintf(stderr, "%d:PtlGetRegion: %s\n", portals->rank, PtlErrorStr(rc));
             exit(EXIT_FAILURE); 
     }
     return rc;
     
}





int armci_get_md(void * start, int bytes , ptl_md_t * md_ptr, ptl_match_bits_t * mb_ptr)
{
    int i;
    int found = 0;
    
    printf("inside armci_get_md, the value of portals->num_match_ent is %d\n", portals->num_match_entries);
    fflush(stdout);
    
    for (i=0; i<portals->num_match_entries; i++){
         md_ptr = &(_armci_md_table[i].md);
         /*md_ptr = _armci_md_table[i].md;*/
         
         printf("the value of start is %p,  bytes is %d, the value of table-start is %p,
                         the value of table-end is %p,md_ptr->start is %p\n",start, 
                         bytes,_armci_md_table[i].start, _armci_md_table[i].end, md_ptr->start);
         fflush(stdout);
         
         printf("%d: start: %p, tab.start: %p, start+bytes is %p, tab.end is %p\n",
                         portals->rank,start, _armci_md_table[i].start,
                         ((char *)start + bytes), (char *)_armci_md_table[i].end );
         fflush(stdout);
         if ( (start >= _armci_md_table[i].start)  && 
                         ( ((char *)start + bytes) <= (char *)_armci_md_table[i].end)  )
         {
                 mb_ptr = &(_armci_md_table[i].mb);        
                 found = 1;
                 break;
         }        
    }
    printf("%d: returning from get_md found is %d , entry is %d , md_ptr->start is %p, 
                    md_ptr->eq_handle %u\n",portals->rank, found, i, md_ptr->start, 
                   md_ptr->eq_handle);
    fflush(stdout); 
         
    return found;
}
#endif


comp_desc * get_free_comp_desc(int * comp_id)
{
    comp_desc * c;     
    c = &(_armci_portals_comp[free_desc_index]);
    while (c->active != 0){
       armci_client_complete(NULL,c->dest_id,0,c,1); // changed blocking tag to 1 
                                  
    }
    *comp_id = free_desc_index;
#ifdef DEBUG
    printf("the value of comp_desc_id is %d\n",*comp_id);
    fflush(stdout);
#endif
    free_desc_index = ((free_desc_index + 1) % MAX_OUT);
    return c;
}


print_mem_desc(ptl_md_t * md)
{
  printf("md : start %p : length %d\n",md->start, md->length);
  fflush(stdout);
        
}

ARMCI_MEMHDL_T armci_ptl_local_mhdl;
ARMCI_MEMHDL_T *armci_portals_fill_local_mhdl(void *ptr,int bytes)
{
    armci_ptl_local_mhdl.mem_dsc.start = ptr;
    armci_ptl_local_mhdl.mem_dsc.length = bytes;
    armci_ptl_local_mhdl.mem_dsc.max_size =0;
    return(&armci_ptl_local_mhdl);
}

void armci_client_direct_get(int proc, void *src_buf, void *dst_buf, int bytes,
                             void** cptr,int tag,ARMCI_MEMHDL_T *lochdl,
                             ARMCI_MEMHDL_T *remhdl)
{
   int clus = armci_clus_id(proc);
   int rc, i;
   ptl_size_t offset_local = 0, offset_remote=0;
   ptl_match_bits_t mb = 100;
   ptl_md_t *md_remote,md, *md_local;
   ptl_md_t * md_ptr;
   ptl_match_bits_t * mb_ptr;
   ptl_handle_md_t *md_hdl_local;
   comp_desc *cdesc;
   ptl_process_id_t dest_proc;
   int c_info;
   int lproc,rproc;


#ifdef DEBUG
       printf("ENTERING CLIENT GET: src_buf is %p\n, loc_hd is %p ,
                     rem_hndl is %p, BYTES = %d\n",src_buf,lochdl,remhdl,bytes);
       fflush(stdout);
#endif

  if (PtlGetRankId(proc, &dest_proc) != PTL_OK) {
      fprintf(stderr, "%d:PtlGetRankId() failed\n", portals->rank);
      exit(EXIT_FAILURE);
  }
 
   
  if(lochdl == NULL)lochdl=armci_portals_fill_local_mhdl(dst_buf,bytes);
  md_local = &lochdl->mem_dsc; 
  md_hdl_local = &lochdl->mem_dsc_hndl; 
  md_remote =&remhdl->mem_dsc;

#ifdef DEBUG
    printf("%d ,the value of local desc is %p and remote desc is %p\n"
                    ,portals->rank,md_local,md_remote);
    print_mem_desc(md_local);
    print_mem_desc(md_remote);
#endif

    //offset_local = lochdl->offset;
    offset_local = (char*)dst_buf - (char *)md_local->start;
    offset_remote = (char*)src_buf - (char *)md_remote->start;
   
#ifdef DEBUG
    printf (" src_buf %p, local_offset is %lu , local_len is %d\n", 
                    src_buf, offset_local,md_local->length );
    fflush(stdout);
    printf("rem_start  is %p, len : %d, rem_offset is %lu\n",
                    md_remote->start, md_remote->length, offset_remote);
    fflush(stdout);
#endif
    
    cdesc = get_free_comp_desc(&c_info);
    if(tag)*cptr = c_info; /*TOED*/
    if (!tag){
       cdesc->tag = tag;
       cdesc->dest_id = proc;
       cdesc->type = ARMCI_PORTALS_GET;
       cdesc->active = 0;
    }
    else{
       cdesc->tag = 999999;
       cdesc->dest_id = proc;
       cdesc->type = ARMCI_PORTALS_NBGET; 
       cdesc->active = 0;
    }

    md_local->threshold = PTL_MD_THRESH_INF;
    md_local->options =  PTL_MD_OP_GET | PTL_MD_EVENT_START_DISABLE
                         | PTL_MD_MANAGE_REMOTE;
    md_local->user_ptr = (void *)cdesc;
    md_local->eq_handle = portals->eq_h;

    rc = PtlMDBind(portals->ni_h,*md_local, PTL_RETAIN, md_hdl_local);
    if (rc != PTL_OK){
       fprintf(stderr, "%d:PtlMDBind: %s\n", portals->rank, PtlErrorStr(rc));
       armci_die("ptlmdbind failed",0);
    }
    
    rc = PtlGetRegion(*md_hdl_local,offset_local,bytes,dest_proc,
                   portals->ptl,
                   0, mb,offset_remote);
    if (rc != PTL_OK){
       fprintf(stderr, "%d:PtlGetRegion: %s\n", portals->rank,PtlErrorStr(rc));
       armci_die("PtlGetRegion failed",0); 
    }
#ifdef DEBUG
    printf("returning from ptlgetregion call\n");
    fflush(stdout);
#endif
    
    if(!tag){ // this is a blocking call
       armci_client_complete(NULL,proc,0,NULL,1); /* check this later */
    }
}


int armci_client_direct_send(int proc,void *src, void* dst, int bytes,  NB_CMPL_T *cmpl_info, int tag, ARMCI_MEMHDL_T *lochdl, ARMCI_MEMHDL_T *remhdl )
{
    int clus = armci_clus_id(proc);    
    int rc, i;
    ptl_size_t offset_local = 0, offset_remote = 0;
    ptl_match_bits_t mb = 100;
    ptl_md_t *md_remote,md, *md_local;
    ptl_md_t * md_ptr;
    ptl_match_bits_t * mb_ptr;
    ptl_handle_md_t *md_hdl_local;
    comp_desc *cdesc;
    ptl_process_id_t dest_proc;
    int c_info;
    int lproc,rproc;

    if (PtlGetRankId(proc, &dest_proc) != PTL_OK) {
        fprintf(stderr, "%d:PtlGetRankId() failed\n", portals->rank);
        exit(EXIT_FAILURE);
    }

    if(lochdl == NULL)lochdl=armci_portals_fill_local_mhdl(src,bytes);
    md_local = &lochdl->mem_dsc; 
    md_hdl_local = &lochdl->mem_dsc_hndl; 
    md_remote =&remhdl->mem_dsc;
    
    offset_local = (char *)src - (char *)md_local->start;
    offset_remote =(char *)dst - (char *)md_remote->start;

    cdesc = get_free_comp_desc(&c_info);
    //*cmpl_info = c_info; /*TOED*/
    if (!tag){
       cdesc->tag = tag;
       cdesc->dest_id = proc;
       cdesc->type = ARMCI_PORTALS_PUT;
       cdesc->active = 0;
    }
    else{
       cdesc->tag = 999999;
       cdesc->dest_id = proc;
       cdesc->type = ARMCI_PORTALS_NBPUT; 
       cdesc->active = 0;
    }

    md_local->threshold = PTL_MD_THRESH_INF;
    md_local->options =  PTL_MD_OP_PUT | PTL_MD_EVENT_START_DISABLE
                         | PTL_MD_MANAGE_REMOTE;
    md_local->user_ptr = (void *)cdesc;
    md_local->eq_handle = portals->eq_h;

    rc = PtlMDBind(portals->ni_h,*md_local, PTL_RETAIN, md_hdl_local);
    if (rc != PTL_OK){
       fprintf(stderr, "%d:PtlMDBind: %s\n", portals->rank, PtlErrorStr(rc));
       armci_die("ptlmdbind failed",0);
    }
    
    rc = PtlPutRegion(*md_hdl_local,offset_local,bytes,PTL_ACK_REQ,dest_proc,
                   portals->ptl,
                   0, mb,offset_remote, 0);
    if (rc != PTL_OK){
       fprintf(stderr, "%d:PtlPutRegion: %s\n", portals->rank,PtlErrorStr(rc));
       exit(EXIT_FAILURE); 
    }

#ifdef DEBUG
    printf("returning from ptlputregion call\n");
    fflush(stdout);
#endif
    
    armci_update_fence_array(proc, 1);
    if(!tag){
       armci_client_complete(NULL,proc,0,NULL,1); /* check this later */
    }
    else
       portals->outstanding_puts++;   
    return rc;
}


int armci_portals_complete(int tag, NB_CMPL_T *cmpl_info)
{
   int rc;
   int proc;
   /*TOED*/
   rc = armci_client_complete(NULL,proc,tag,NULL,0);
   return rc;
}


/* $id$ */
#ifndef PORTALS_H
#define PORTALS_H

/* portals header file */

#ifdef XT3

#include <portals/portals3.h>
#include <catamount/cnos_mpi_os.h>

#else

#include <portals3.h>
#include P3_NAL
#include <p3rt/p3rt.h>
#include <p3api/debug.h>

#endif

/*this should match regions.c*/
#define PORTALS_MEM_REGIONS 24

#define MAX_OUT 16
#define HAS_RDMA_GET

typedef enum op {
        ARMCI_PORTALS_PUT,
        ARMCI_PORTALS_NBPUT,
        ARMCI_PORTALS_GET, 
        ARMCI_PORTALS_NBGET, 
        ARMCI_PORTALS_ACC
} armci_portals_optype;

/* array of memory segments and corresponding memory descriptors */
typedef struct md_table{
        ptl_md_t md; /* make this a ptr instead of struct */
        void * start;
        void * end;
        int id;
        int bytes;
        ptl_match_bits_t mb;
} md_table_entry_t;


typedef struct desc{
       int active;
       int tag;      
       int dest_id;
       armci_portals_optype type;
       ptl_md_t mem_dsc;
       ptl_md_t mem_dsc_save;
       ptl_handle_md_t mem_dsc_hndl;
}comp_desc; 

typedef struct region_memhdl{
       ptl_match_bits_t match_bits;
       int regid;
       int islocal;
       ptl_size_t offset;
       comp_desc cdesc;
} region_memhdl_t;

#define NB_CMPL_T comp_desc*
#define REGIONS_REQUIRE_MEMHDL
#define ARMCI_MEMHDL_T region_memhdl_t 

#define ARMCI_NB_WAIT(_cntr) if(_cntr){\
        int rc;\
        if(nb_handle->tag==_cntr->tag)\
          rc = armci_client_complete(NULL,nb_handle->proc,nb_handle->tag,_cntr);\
}



/* structure of computing process */
typedef struct {
  int armci_rank;  /* if different from portals_rank */      
  int rank;       /* my rank*/
  int size;       /* size of the group */
  ptl_handle_me_t me_h[64];

  ptl_handle_eq_t eq_h;
  ptl_handle_ni_t ni_h; 
  ptl_pt_index_t ptl;
  int outstanding_puts;
  int outstanding_gets;
  int outstanding_accs;
  void * buffers; /* ptr to head of buffer */
  int num_match_entries;
  cnos_nidpid_map_t       *ptl_pe_procid_map;  
  ptl_process_id_t        ptl_my_procid;
} armci_portals_proc_t;


extern void print_mem_desc_table(void);
extern int armci_init_portals(void);
extern void armci_fini_portals(void);
extern int armci_post_descriptor(ptl_md_t *md); 
extern int armci_prepost_descriptor(void* start, long bytes);
extern ptl_size_t armci_get_offset(ptl_md_t md, void *ptr,int proc);
extern int armci_get_md(void * start, int bytes , ptl_md_t * md, ptl_match_bits_t * mb);
extern int armci_portals_put(ptl_handle_md_t md_h,ptl_process_id_t dest_id,int bytes,int mb,int local_offset, int remote_offset,int ack );
extern int armci_portals_get(ptl_handle_md_t md_h,ptl_process_id_t dest_id,int bytes,int mb,int local_offset, int remote_offset);
extern comp_desc * get_free_comp_desc(int,int *);
extern int armci_client_direct_send(int proc,void *src, void* dst, int bytes,  void **cmpl_info, int tag, ARMCI_MEMHDL_T *lochdl, ARMCI_MEMHDL_T *remhdl);
extern int armci_portals_direct_get(void *src, void *dst, int bytes, int proc, int nbtag, NB_CMPL_T *cmpl_info);
extern void comp_desc_init();
extern int armci_client_complete(ptl_event_kind_t *evt,int proc_id, int nb_tag ,comp_desc * cdesc);

#endif /* PORTALS_H */

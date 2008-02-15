/* $Id: regions.c,v 1.12.2.2 2006/09/13 17:58:03 andriy Exp $ interface to keep track of memory regions accross the cluster */
/* 
 * armci_region_init - allocates list of regions, initialization
 * armci_region_register_shm - registers shared memory on the current node
 * armci_region_register_loc - registers local memory
 * armci_region_clus_record  - stores info on pinned region on a given cluster node
 * armci_region_clus_found   - returns 1/0 if specified memory is registered
 * armci_region_loc_found    - same for local memory
 * armci_region_loc_both_found - returns 1 if local and remote are found, otherwise 0
 *
 */

#include "armcip.h"
#include <stdlib.h>
#include <stdio.h>
#include "copy.h"

#define MAX_REGIONS 8

typedef struct {
  void *start;
  void *end;
#ifdef REGIONS_REQUIRE_MEMHDL
  ARMCI_MEMHDL_T memhdl;
#endif
} armci_region_t;

typedef struct {
        long n;
        armci_region_t list[MAX_REGIONS];
} armci_reglist_t;

static int allow_pin=0;
static armci_reglist_t  *clus_regions;        /* cluster shared/remote memory */
#ifdef REGIONS_REQUIRE_MEMHDL
static armci_reglist_t  *serv_regions;        /* server regions */
#endif
static armci_reglist_t loc_regions_arr;       /* local memory */
static void *needs_pin_shmptr=NULL, *needs_pin_ptr=NULL;
static int  needs_pin_shmsize=0, needs_pin_size=0;

#ifdef REGIONS_REQUIRE_MEMHDL
extern int armci_pin_contig_hndl(void *ptr,int bytes, ARMCI_MEMHDL_T *memhdl);
extern int armci_server_pin_contig(void *ptr,int bytes, ARMCI_MEMHDL_T *memhdl);
#else
extern int armci_pin_contig1(void *ptr, int bytes);
#endif
static void **exch_list=(void**)0;
static char exch_loc[MAX_REGIONS];
static char exch_rem[MAX_REGIONS];


static int armci_region_record(void *start, void *end, armci_reglist_t *reg)
{
     int cur=reg->n;
#ifdef DEBUG_
     int i;
     for(i=0; i<reg->n; i++)
        if(reg->start >= start && reg->end < start) 
           armci_die("armci_region_record: already recorded",i);
#endif
     if(reg->n >= MAX_REGIONS) return 0;
     (reg->list+cur)->start = start; 
     (reg->list+cur)->end   = end;
     reg->n++;
     return 1;
}

static void armci_region_register(void *start, long size, armci_reglist_t *reg)
{
     if(reg->n >= MAX_REGIONS) return;
     if(armci_nclus<=1)return;

#ifdef REGIONS_REQUIRE_MEMHDL
     if(!armci_pin_contig_hndl(start,(int)size, &((reg->list+reg->n)->memhdl))){
        printf("%d pin failed %p bytes=%ld\n",armci_me,start,size);
        fflush(stdout); return; 
     }
#else
     if(!armci_pin_contig1(start, (int) size)){
        printf("%d pin failed %p bytes=%ld\n",armci_me,start,size);
        fflush(stdout); return; 
     }
#endif

     (void)armci_region_record(start,((char*)start)+size,reg);
}


void armci_region_register_shm(void *start, long size)
{
     if(allow_pin)
         armci_region_register(start, size, clus_regions+armci_clus_me);     
     else{
         needs_pin_shmptr = start;
         needs_pin_shmsize= size;
     }

#if 0
     if(allow_pin){
        printf("\n%d:%d registering shm %p bytes=%ld\n",armci_me,allow_pin,start,size);
        fflush(stdout);
     }
#endif
}


void armci_region_register_loc(void *start, long size)
{
     if(allow_pin)armci_region_register(start, size, &loc_regions_arr);
     else{
         needs_pin_ptr = start;
         needs_pin_size= size;
     }
#if 0
     if(allow_pin){
        printf("\n%d:%d registered local %p bytes=%ld\n",armci_me,allow_pin,start,size);
        fflush(stdout);
     }
#endif
}


void armci_region_clus_record(int node, void *start, long size)
{
     if(node > armci_nclus || node <0 ) 
               armci_die("armci_region_remote: bad node ",node);

     (void)armci_region_record(start,((char*)start)+size,clus_regions+node);
}


void armci_region_init()
{ 
     allow_pin =1; 
     clus_regions=(armci_reglist_t*)calloc(armci_nclus,sizeof(armci_reglist_t));
     if(!clus_regions)armci_die("armci_region_init: calloc failed",armci_nclus);
#ifdef REGIONS_REQUIRE_MEMHDL
     serv_regions=(armci_reglist_t*)calloc(armci_nclus,sizeof(armci_reglist_t));
     if(!serv_regions)armci_die("armci_region_init: calloc failed",armci_nclus);
#endif
     exch_list = (void**)calloc(2*armci_nclus, sizeof(void*));
     if(!exch_list) armci_die("armci_region_init: calloc 2 failed",armci_nclus);
     bzero(exch_loc,sizeof(exch_loc));
     bzero(exch_rem,sizeof(exch_rem));

#if 0
     printf("%d: initialized regions\n",armci_me); fflush(stdout);
#endif
     if(needs_pin_ptr) armci_region_register_loc(needs_pin_ptr,needs_pin_size); 
     if(needs_pin_shmptr) armci_region_register_shm(needs_pin_shmptr,needs_pin_shmsize); 
} 

void armci_region_destroy()
{
armci_reglist_t *reg = &loc_regions_arr;
int i; 
#ifdef REGIONS_REQUIRE_MEMHDL
ARMCI_MEMHDL_T *loc_memhdl;
#endif
    if(!allow_pin) return;
    for(i=0; i<reg->n; i++){
#ifdef REGIONS_REQUIRE_MEMHDL
       loc_memhdl=&((reg->list+i)->memhdl);
       armci_network_client_deregister_memory(loc_memhdl);
#endif
    }

    reg=clus_regions+armci_clus_me;
    for(i=0; i<reg->n; i++){
#ifdef REGIONS_REQUIRE_MEMHDL
       loc_memhdl=&((reg->list+i)->memhdl);
       armci_network_client_deregister_memory(loc_memhdl);
#endif
    }
}

void armci_server_region_destroy()
{
armci_reglist_t *reg;
int i;
#ifdef REGIONS_REQUIRE_MEMHDL
ARMCI_MEMHDL_T *loc_memhdl;
    reg=serv_regions+armci_clus_me;
#endif
    for(i=0; i<reg->n; i++){
#ifdef REGIONS_REQUIRE_MEMHDL
       loc_memhdl=&((reg->list+i)->memhdl);
       armci_network_server_deregister_memory(loc_memhdl);
#endif
    }
}
 

int armci_region_clus_found(int node, void *start, int size)
{
    armci_reglist_t *reg=clus_regions+node;
    int i,found=-1;
    if(!allow_pin) return 0;
    if(node > armci_nclus || node <0 ) 
               armci_die("armci_region_clus_found: bad node ",node);
    for(i=0; i<reg->n; i++)
        if((reg->list+i)->start <= start && (reg->list+i)->end > start){found=i; break;}
    
    return(found);
}

int armci_region_loc_found(void *start, int size)
{
     armci_reglist_t *reg = &loc_regions_arr;
     int i,found=-1;
     if(!allow_pin) return 0;
     for(i=0; i<reg->n; i++)
        if((reg->list+i)->start <= start && (reg->list+i)->end > start){found=i; break;}
#if 0
     if(found){
        printf("%d: found loc %d n=%ld (%p,%p) %p\n",armci_me,found,reg->n,
               (reg->list)->start,(reg->list)->end, start); fflush(stdout);
     }
#endif

     return(found);
}

#ifdef REGIONS_REQUIRE_MEMHDL
int armci_region_both_found_hndl(void *loc, void *rem, int size, int node,
                        ARMCI_MEMHDL_T **loc_memhdl,ARMCI_MEMHDL_T **rem_memhdl)
{
 armci_reglist_t *reg = &loc_regions_arr;
     int i,found=0; 
     if(!allow_pin) return 0;
     /* first scan for local */
     for(i=0; i<reg->n; i++){
        if((reg->list+i)->start <= loc && (reg->list+i)->end > loc){
	   /* printf("\n%d: loc found \n",armci_me); */
	  found=1; break;
	}
#if 0
	else {
	  printf("\n%d: loc ptr=%p st=%p end=%p size=%d\n",armci_me,loc,
			  (reg->list+i)->start,(reg->list+i)->end,size);
	  fflush(stdout);
	}
#endif
     }

     if(!found){ /* might be local shared */
         reg=clus_regions+armci_clus_me;
         for(i=0; i<reg->n; i++){
           if((reg->list+i)->start <= loc && (reg->list+i)->end > loc){
	      /* printf("\n%d: clus found \n",armci_me); */
	     found=1; break;
	   }
#if 0
	   else {
	     printf("\n%d:clus ptr=%p st=%p end=%p size=%d\n",armci_me,loc,
			  (reg->list+i)->start,(reg->list+i)->end,size);
	     fflush(stdout);
	   }
#endif
	 }
     }
#ifdef PORTALS
     if(found!=1){
        *loc_memhdl=NULL;
        found=1;
     }
#else
     if(!found) return 0;
#endif
     else {*loc_memhdl=&((reg->list+i)->memhdl);}
      

     /* now check remote shared */
     reg=serv_regions+node;
     for(i=0; i<reg->n; i++){
         if((reg->list+i)->start <= rem && (reg->list+i)->end > rem){
	    /* printf("\n%d: serv found \n",armci_me); */
		 found=2;break;
	 }
#if 0
	 else {
	   printf("\n%d: serv ptr=%p st=%p end=%p size=%d nd=%d\n",armci_me,rem,
	          (reg->list+i)->start,(reg->list+i)->end,size,node);
	   fflush(stdout);
	 }
#endif
     }

     if(0){ 
            if(found==2){printf("%d: found both %d %p\n",
                      armci_me,node,*loc_memhdl); 
            fflush(stdout); 
            }
     }
     if(found==2){*rem_memhdl=&((reg->list+i)->memhdl); return 1;}
     else return 0; 
}


int armci_region_remote_found_hndl(void *rem,int size, int node, ARMCI_MEMHDL_T **remhdl)
{
armci_reglist_t *reg = serv_regions+node;
int i,found=0;

    for(i=0; i<reg->n; i++)
      if((reg->list+i)->start <= rem && (reg->list+i)->end > rem){
              found=1;break;
      }

    if(found==1){*remhdl=&((reg->list+i)->memhdl); return 1;}
    else return 0;

}

int get_armci_region_local_hndl(void *loc, int node,ARMCI_MEMHDL_T **loc_memhdl)
{

  armci_reglist_t *reg = &loc_regions_arr;
  int i, found = 0;

   if(!allow_pin) {printf("inside get_armci_region_local_hndl : case allow_pin = 0\n"); return 0; }
   if(!found){
      reg = serv_regions+armci_clus_me;
      for(i=0; i<reg->n; i++){
         if((reg->list+i)->start <= loc && (reg->list+i)->end >loc){
           found =1;
           break; 
         }
#if 0
         else {
	   printf("\n%d: serv ptr=%p st=%p end=%p nd=%d nreg=%d\n",armci_me,loc,
	          (reg->list+i)->start,(reg->list+i)->end,node,reg->n);
	   fflush(stdout);
         }
#endif
      }
   }
   if(found == 1){
      *loc_memhdl = &((reg->list+i)->memhdl); 
      if(0){
         printf("%d(s) : found local %p\n",armci_me,*loc_memhdl);
         fflush(stdout);
      }     
      return 1;
   }
   else return 0;
}


int armci_region_serv_found(int node,void *start,int size)
{
    armci_reglist_t *reg=serv_regions+node;
    int i,found=-1;
    if(!allow_pin) return 0;
    if(node > armci_nclus || node <0 ) 
      armci_die("armci_region_serv_found: bad node ",node);
    for(i=0; i<reg->n; i++)
        if((reg->list+i)->start <= start && (reg->list+i)->end > start){found=i; break;}
    
    return(found);
}

#endif


int armci_region_both_found(void *loc, void *rem, int size, int node)
{
     armci_reglist_t *reg = &loc_regions_arr;
     int i,found=0;
     if(!allow_pin) return 0;

     /* first scan for local */
     for(i=0; i<reg->n; i++)
        if((reg->list+i)->start <= loc && (reg->list+i)->end > loc){found=1; break;}

     if(!found){ /* might be local shared */
         reg=clus_regions+armci_clus_me;
         for(i=0; i<reg->n; i++)
           if((reg->list+i)->start <= loc && (reg->list+i)->end > loc){found=1; break;}
     }
     if(!found) return 0;

     /* now check remote shared */
     reg=clus_regions+node;
     for(i=0; i<reg->n; i++)
         if((reg->list+i)->start <= rem && (reg->list+i)->end > rem){found=2;break;}

#if 0
     if(found==2){printf("%d: found both %d\n",armci_me,node); fflush(stdout); }
#endif
     if(found==2) return 1;
     else return 0;
}
     


void armci_region_exchange(void *start, long size)
{
     int found=0, i;
     armci_region_t *reg=0;
#ifdef REGIONS_REQUIRE_MEMHDL
     ARMCI_MEMHDL_T *hdlarr;
     
     hdlarr = calloc(armci_nclus,sizeof(ARMCI_MEMHDL_T));
#endif
     if(!allow_pin)return;
     if(armci_nclus<=1)return;

     found=armci_region_clus_found(armci_clus_me, start,size);
     if(found>-1){
        if(!exch_rem[found]){
           reg = (clus_regions+armci_clus_me)->list+found; 
           exch_rem[found]=1;
        }
     }else{
        found= armci_region_loc_found(start,size);
        if(found>-1){
             if(!exch_loc[found]){
               reg =  (&loc_regions_arr)->list+found;
               exch_loc[found]=1;
             }
        }
     }

     bzero(exch_list,2*armci_nclus*sizeof(void *));
     if( reg && (armci_me == armci_master)){  
        exch_list[2*armci_clus_me] = reg->start;
        exch_list[2*armci_clus_me+1] = reg->end;
#ifdef REGIONS_REQUIRE_MEMHDL
        armci_copy(&reg->memhdl,&hdlarr[armci_clus_me],sizeof(ARMCI_MEMHDL_T));
#endif
     }

     /* exchange info on new regions with other nodes */
     armci_exchange_address(exch_list,2*armci_nclus);
#ifdef REGIONS_REQUIRE_MEMHDL
     i = armci_nclus*sizeof(ARMCI_MEMHDL_T)/sizeof(int);
     armci_msg_gop_scope(SCOPE_ALL,hdlarr,i,"+",ARMCI_INT);
#endif
     for(i=0; i<armci_nclus; i++){
         armci_reglist_t *r=clus_regions+i;
         if(i==armci_clus_me) continue;
         if(exch_list[2*i]){
#if 0
           printf("%d recording clus=%d mem %p-%p n=%d\n",armci_me,i,exch_list[2*i],
                  exch_list[2*i+1],r->n);
           fflush(stdout);
#endif

#ifdef REGIONS_REQUIRE_MEMHDL
          armci_copy(&hdlarr[i],&(r->list+r->n)->memhdl,sizeof(ARMCI_MEMHDL_T));
#endif
           armci_region_record(exch_list[2*i],exch_list[2*i+1], r);
         }
     }
}

/*\
 * for server thread to know which region is what in cases where there is one 
 * process per node
\*/
void armci_global_region_exchange(void *start, long size)
{
#ifdef REGIONS_REQUIRE_MEMHDL
    ARMCI_MEMHDL_T *hdlarr;
     
    hdlarr = calloc(armci_nclus,sizeof(ARMCI_MEMHDL_T));
#endif
    if(!allow_pin)return;
    if(armci_nclus<=1)return;

    armci_region_exchange(start,size);

#ifdef REGIONS_REQUIRE_MEMHDL
    {
      int foundclus=0, foundserv=0, i,loc=0;
      armci_reglist_t *reglist=NULL,*clreglist=NULL;      
      armci_region_t *reg=NULL;
      
      foundclus=armci_region_clus_found(armci_clus_me, start,size);
      foundserv=armci_region_serv_found(armci_clus_me, start,size);
      if(foundclus==-1){
	foundclus = armci_region_loc_found(start,size);
	loc=1;
      }
      
      if(foundclus!=-1 && foundserv==-1){
	reglist = (serv_regions+armci_clus_me); 
	if(loc)
	  clreglist = &(loc_regions_arr); 
	else
	  clreglist = (clus_regions+armci_clus_me); 
#ifdef DATA_SERVER
	armci_serv_register_req((clreglist->list+foundclus)->start,((char *)(clreglist->list+foundclus)->end-(char *)((clreglist->list+foundclus)->start)),&((reglist->list+reglist->n)->memhdl));
#endif
	(void)armci_region_record((clreglist->list+foundclus)->start,(clreglist->list+foundclus)->end,reglist);
#if 0
	printf("\n%d:serv recording %p from %d n=%d \n",armci_me,(clreglist->list+foundclus)->start,armci_clus_me,reglist->n);fflush(stdout);
#endif
	foundserv=armci_region_serv_found(armci_clus_me, start,size);
	reg = (serv_regions+armci_clus_me)->list+foundserv; 
      }
      if(reg)
	armci_copy(&reg->memhdl,&hdlarr[armci_clus_me],sizeof(ARMCI_MEMHDL_T));
      
      i = armci_nclus*sizeof(ARMCI_MEMHDL_T)/sizeof(int);
      armci_msg_gop_scope(SCOPE_ALL,hdlarr,i,"+",ARMCI_INT);
      for(i=0; i<armci_nclus; i++){
	armci_reglist_t *r=serv_regions+i;
	armci_reglist_t *rc=clus_regions+i;
	if(i==armci_clus_me) continue;
	if((rc->list+r->n)->start){
#if 0
	  printf("\n%d:serv recording %p from %d n=%d \n",armci_me,(rc->list+r->n)->start,i,r->n);fflush(stdout);
#endif
	  armci_copy(&hdlarr[i],&(r->list+r->n)->memhdl,sizeof(ARMCI_MEMHDL_T));
	  armci_region_record((rc->list+r->n)->start,(rc->list+r->n)->end,r);
	}
      }
    }
#endif
}

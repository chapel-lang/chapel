/*interfaces for checkpointing */

/* TODO
 * work on the case if pagenum==firstpage or lastpage when writing pages
 */
#include <stdio.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <stdarg.h>
#include "armcip.h"
#include "message.h"
#include "armci_chkpt.h"

#define DEBUG 1

/*\
 * ----------CORE FUNCTIONS -----------
 * armci_init_checkpoint() - the checkpointing code is initialized 
 * armci_icheckpoint_init() - called when with first checkpoint
 * armci_icheckpoint_finalize() - called when done with chkpt
 * armci_icheckpoint() - called every time we checkpoint
 * armci_recover() - called to recoved
 * ----------SUPPORT FUNCTIONS -----------
 * armci_ckpt_pgfh() - pagefault handler, to set the list of dirty pages
 * armci_monitor_addr() - monitors address to look for and pages to set readonly
 * armci_create_record() - create the record for local part of data being stored
 * armci_protect_pages() - to protect pages in the dirty page list
 * armci_storage_read() - reads record from storage 
 * armci_storage_write() - writes into storage
 * armci_storage_fopen() - opens a file in storage
 *
\*/

/*\ global variables
\*/
static armci_storage_record_t armci_storage_record[1001];
static int isheap;
static int number_of_records=1;
static int next_available_rid=0;
static int mypagesize; 
int **armci_rec_ind;
static armci_page_info_t armci_dpage_info;
static int checkpointing_initialized=0;
static int armci_recovering=0;

/* ----------SUPPORT FUNCTIONS ----------- */

/* This function is called from ARMCI sigv and sigbus handler */
int armci_ckpt_pgfh(void *addr, int errno, int fd)
{
    char *paddr;
    unsigned long pagenum;
    /*find the page number and the corresponding page aligned address*/
    pagenum = (unsigned long)((long)addr/mypagesize);
    (long)paddr = pagenum*mypagesize; 

    if(DEBUG)printf("\n%d:paddr=%p addr=%p %lu\n",armci_me,paddr,addr,pagenum);

    /*page is being touched change page permission to READ/WRITE*/
    mprotect(paddr, mypagesize, PROT_READ | PROT_WRITE);
    
    /*mark pagenumber dirty in dirty page array*/
    armci_dpage_info.touched_page_arr[armci_dpage_info.num_touched_pages++] =
            pagenum;
    if(pagenum<armci_dpage_info.firstpage)
            armci_dpage_info.firstpage = pagenum;
    if(pagenum>armci_dpage_info.lastpage)
            armci_dpage_info.lastpage = pagenum;
    
    printf("\n%d: came here success %ld\n",armci_me,armci_dpage_info.num_touched_pages);fflush(stdout);

    return(0);
}

#if 0
printf("\n%d:pagenum=%d first=%d last=%d\n",armci_me,pagenum,armci_storage_record[i].firstpage,(armci_storage_record[i].firstpage+armci_storage_record[i].totalpages));fflush(stdout);
#endif

static int armci_create_record(ARMCI_Group *group, int count)
{
    int recind;
    int relprocid,relmaster;
    int subs[1]={0};
    int rc;

    rc = ARMCI_Group_rank(group,&relprocid);
    /*create and broadcast new index in the records data structure*/
    next_available_rid = 0;
    if(next_available_rid==0){
       if(relprocid==0)
         ARMCI_Rmw(ARMCI_FETCH_AND_ADD,&recind,armci_rec_ind[0],1,0);
    }
    else recind=next_available_rid;
    armci_msg_group_bcast_scope(SCOPE_ALL,&recind,sizeof(int),0,group);

    if(recind>1001) armci_die("create_record, failure",recind);
    armci_storage_record[recind].pid = armci_me;
    armci_storage_record[recind].rid = recind;
    armci_storage_record[recind].rel_pid = relprocid;
    memcpy(&armci_storage_record[recind].group,group,sizeof(ARMCI_Group));
    if(count!=0)
       armci_storage_record[recind].user_addr = (armci_monitor_address_t *)malloc(sizeof(armci_monitor_address_t)*count);
    armci_storage_record[recind].user_addr_count=count;
    
    if(next_available_rid!=0)
       next_available_rid = 0;
    else
       number_of_records++;

    return(recind);
}


static void armci_protect_pages(unsigned long startpagenum,unsigned long numpages)
{
    unsigned long i=0;
    char *addr;
    addr =(char *)((unsigned long)(startpagenum*mypagesize));
    mprotect(addr, mypagesize*numpages,PROT_READ);
    if(DEBUG)printf("\n%d:protecting address %p %ld",armci_me,addr,mypagesize*numpages);
}

/*\ ----------CORE FUNCTIONS -----------
\*/
/*called in armci init*/
int armci_init_checkpoint(int spare)
{
    int val=1,rc;
    armci_size_t rec_size=0;
    
    mypagesize = getpagesize();
    if(checkpointing_initialized)return(0);

    /* malloc for record index */
    armci_rec_ind = (int **)malloc(sizeof(int *)*armci_nproc);
    if(armci_me==0){
       rc = ARMCI_Malloc((void **)armci_rec_ind, 2*sizeof(int));
       armci_rec_ind[armci_me][0]=armci_rec_ind[armci_me][1]=1;
    }
    else
       rc = ARMCI_Malloc((void **)armci_rec_ind, 0);
    assert(rc==0);

    ARMCI_Register_Signal_Handler(SIGSEGV,(void *)armci_ckpt_pgfh);
    armci_dpage_info.touched_page_arr = (unsigned long *)malloc(sizeof(unsigned long)*100000);
    armci_dpage_info.num_touched_pages=armci_dpage_info.lastpage=0;
    armci_dpage_info.firstpage = 99999999;
    armci_create_ft_group(spare);

    checkpointing_initialized = 1;
    return(0);
}

void armci_create_ckptds(armci_ckpt_ds_t *ckptds, int count)
{
    printf("\n%d:in armci_create_ckptds with count=%d",armci_me,count);fflush(stdout);
    ckptds->count=count;
    ckptds->ptr_arr=(void **)malloc(sizeof(void *)*(count+1));
    ckptds->sz=(size_t *)malloc(sizeof(size_t)*(count+1));
    ckptds->saveonce=(int *)calloc((count+1),sizeof(int));
    if( ckptds->saveonce==NULL || ckptds->ptr_arr==NULL || ckptds->sz == NULL )
      armci_die("malloc failed in armci_create_ckptds",count);
}

void armci_free_chkptds(armci_ckpt_ds_t *ckptds)
{
    free(ckptds->ptr_arr);
    free(ckptds->sz);
}

static void armci_create_protect_pages(armci_monitor_address_t *addrds, void *ptr, unsigned long bytes,int callprotect)
{
    unsigned long firstpage,laddr;
    unsigned long totalpages;
    unsigned long j;
    addrds->ptr=(void *)(ptr);
    addrds->bytes = bytes;
    laddr = (unsigned long)(addrds->ptr);
    addrds->firstpage = (unsigned long)((unsigned long)laddr/mypagesize);

    if(laddr%mypagesize ==0){
       totalpages = (int)(bytes/mypagesize);
       if(bytes%mypagesize)totalpages++;
    }
    else {
       int shift;
       shift = mypagesize - laddr%mypagesize;
       if(DEBUG){
         printf("\n%d:shift=%d bytes=%ld",armci_me,shift,bytes);
         fflush(stdout);
       }
       if(bytes<shift)totalpages=1;
       else{
         totalpages = 1+(bytes-shift)/mypagesize;
         if((bytes-shift)%mypagesize)totalpages++;
       }
    }
    addrds->totalpages = totalpages;
    addrds->num_touched_pages = totalpages;
    addrds->touched_page_arr = malloc(totalpages*sizeof(unsigned long));
    if(addrds->touched_page_arr==NULL)
       armci_die("malloc failed in armci_icheckpoint_init",totalpages);
    addrds->touched_page_arr[0]=addrds->firstpage;
    for(j=1;j<totalpages;j++){
       addrds->touched_page_arr[j]=addrds->touched_page_arr[j-1]+1;
    }
    if(DEBUG){
       printf("\n%d: first=%lu total=%lu laddr=%lu (%p)",armci_me, addrds->firstpage, addrds->totalpages, laddr, laddr);
       fflush(stdout);
    }
    if(callprotect){
       if(isheap)
         armci_protect_pages(addrds->firstpage+16,addrds->totalpages);
       else
         armci_protect_pages(addrds->firstpage,addrds->totalpages);
    }
}

/*called everytime a new checkpoint record is created*/
int armci_icheckpoint_init(char *filename,ARMCI_Group *grp, int savestack, 
                int saveheap, armci_ckpt_ds_t *ckptds)
{
    int rid;
    long bytes;
    char *startaddr;
    unsigned long laddr;
    int totalpages=0,i=0,j=0;
    char line[256],tmpfilename[100];
    unsigned long databottom;
    FILE *fp;
    off_t carriedoff;
    printf("\n%d:in armci ckpt init\n",armci_me);fflush(stdout);
    if(DEBUG && ckptds!=NULL)
            printf("\n%d:ckptdscount=%d",armci_me,ckptds->count);

    /*create the record*/
    if(ckptds!=NULL) rid = armci_create_record(grp,ckptds->count);
    else rid = armci_create_record(grp,0);
    if(DEBUG) printf("\n%d:got rid = %d",armci_me,rid);fflush(stdout);

    armci_storage_record[rid].ckpt_heap = saveheap;
    armci_storage_record[rid].ckpt_stack = savestack;

    /*open the file for reading and writing*/
    if(filename == NULL){
      filename = (char *)malloc(sizeof(char)*(11+1+6+1+4));
      if(filename==NULL)armci_die("alloc for filename failed",11+1+6+1+4);
      sprintf(filename,"%s","armci_chkpt_");
      sprintf((filename+strlen(filename)),"%d",armci_me);
      sprintf((filename+strlen(filename)),"%s","_");
      sprintf(filename+strlen(filename),"%d",rid);
    }
    armci_storage_record[rid].fileinfo.filename = malloc(sizeof(char)*strlen(filename));
    if(NULL==armci_storage_record[rid].fileinfo.filename)
      armci_die("malloc failed for filename in ga_icheckpoint_init",0);
    strcpy(armci_storage_record[rid].fileinfo.filename,filename);
    armci_storage_record[rid].fileinfo.fd = armci_storage_fopen(filename);
    if(DEBUG){printf("\nfilename=%s\n",filename);fflush(stdout);}

    sprintf(tmpfilename, "/proc/%d/maps",getpid());
    fp=fopen(tmpfilename, "r");
    if (fp == NULL){
      armci_die("couldnt find dseg base",getpid());
    }
    /*startaddr = (char *)(&startaddr); */ /* now points to address of itself*/
    /*base offset*/
    carriedoff = sizeof(jmp_buf)+4*sizeof(int);

    if(armci_storage_record[rid].ckpt_stack){
       unsigned long stackbottom;
       char *start,*end;
       armci_monitor_address_t *addrds =&armci_storage_record[rid].stack_mon;
       do {
         fgets(line, 255, fp);
       } while ( line[0] != '6' );
       sscanf(line, "%p", &databottom);
       printf("\n%p databot",(char *)databottom);
       do {
         start = fgets(line, 255, fp);
       } while(start != NULL);
       start = strstr(line, "-") + 1;
       if(DEBUG){printf("\nstack top=%s",start);fflush(stdout);}
       end   = strstr(line, " ");
       *end = 0;
       sscanf(start, "%p", &stackbottom);
       addrds->ptr = (void *)(stackbottom);
       addrds->fileoffset = carriedoff;
    }
    if(armci_storage_record[rid].ckpt_heap){
       char *datatop;
       /*for backing store
        * char *mybass;
        * asm("mov %0=ar.bsp": "=r"(mybss));
        */
       armci_monitor_address_t *addrds =&armci_storage_record[rid].heap_mon;
       datatop=(char *)sbrk(0);
       if(!armci_storage_record[rid].ckpt_stack){
         do {
           fgets(line, 255, fp);
         } while ( line[0] != '6' );
         sscanf(line, "%p", &databottom);
       }
       printf("%d:databot=%p datatop=%p %ld %p\n",armci_me,(void*)databottom,datatop,(unsigned long)((char *)datatop-(char *)databottom),&armci_storage_record[rid].jmp);
       isheap = 1;
       armci_create_protect_pages(addrds,(void *)databottom,(unsigned long)((char *)datatop-(char *)databottom),1);
       isheap = 0;
       mprotect(armci_storage_record,sizeof(armci_storage_record_t)*1000, PROT_READ | PROT_WRITE);
       mprotect(&armci_recovering,sizeof(int), PROT_READ | PROT_WRITE);
       mprotect(&armci_dpage_info,sizeof(armci_page_info_t), PROT_READ | PROT_WRITE);
       mprotect(armci_dpage_info.touched_page_arr,sizeof(unsigned long)*99999999, PROT_READ | PROT_WRITE);
    }
    else {
       if(ckptds!=NULL)
          for(i=0;i<ckptds->count;i++){
             armci_monitor_address_t *addrds =&armci_storage_record[rid].user_addr[i];
             addrds->saveonce = ckptds->saveonce[i];
             if(addrds->saveonce)
                armci_create_protect_pages(addrds,ckptds->ptr_arr[i],ckptds->sz[i],0);
             else
                armci_create_protect_pages(addrds,ckptds->ptr_arr[i],ckptds->sz[i],1);
          }
    }
    if(DEBUG){printf("\n%d:completed init\n",armci_me);fflush(stdout);}
    return(rid);
}

int armci_create_touchedpagearray(unsigned long *tpa,unsigned long firstpage, unsigned long totalpages){
unsigned long i,j=0;
    tpa = (unsigned long *)malloc(sizeof(unsigned long)*armci_dpage_info.num_touched_pages);
    for(i=0;i<armci_dpage_info.num_touched_pages;i++){
       if(DEBUG){
         printf("\n%d: armci_create_tpa %lu %lu %lu",armci_me,
                         armci_dpage_info.touched_page_arr[i],firstpage,
                         (firstpage+totalpages));
         fflush(stdout);
       }
       if(armci_dpage_info.touched_page_arr[i]>=firstpage || armci_dpage_info.touched_page_arr[i] < (firstpage+totalpages)){
         tpa[j]=armci_dpage_info.touched_page_arr[i];
         j++;
       }
    }

    armci_dpage_info.num_touched_pages=0;
    return(j);

}
#define EST_OFFSET 100
static void armci_ckpt_write_stack(int rid)
{
    int dummy1;
    int tmp;
    int dummy2;
    char *top;
    int rc;
    top = ((char *)((unsigned long)(&dummy1) - EST_OFFSET));
    armci_storage_record[rid].stack_mon.bytes=(unsigned long)(armci_storage_record[rid].stack_mon.ptr)-((unsigned long)(top));
    armci_storage_record[rid].stack_mon.fileoffset +=armci_storage_record[rid].stack_mon.bytes;
    rc = armci_storage_write_ptr(armci_storage_record[rid].fileinfo.fd,top,
                    armci_storage_record[rid].stack_mon.bytes,
                    armci_storage_record[rid].stack_mon.fileoffset);
    printf("\n%d:stack %p to %p \n",armci_me,top, top+armci_storage_record[rid].stack_mon.bytes);fflush(stdout);
}

/*get the list of changed pages from touched_page_array and rewrite the 
 * changed pages*/
int armci_icheckpoint(int rid)
{
    int i,j,rc=0;
    off_t ofs;
    char *addr;
    if(DEBUG){
       printf("\n%d: in checkpoint rid=%d %p",armci_me,rid,&armci_recovering);fflush(stdout);
    }
    if(armci_storage_record[rid].ckpt_stack || armci_storage_record[rid].ckpt_heap){
       if((armci_recovering=setjmp(armci_storage_record[rid].jmp))==0){
         rc = armci_storage_write_ptr(armci_storage_record[rid].fileinfo.fd,
                       &armci_storage_record[rid].jmp,sizeof(jmp_buf),
                       4*sizeof(int));
         if(armci_storage_record[rid].ckpt_stack){
           armci_ckpt_write_stack(rid);
         }
         if(armci_storage_record[rid].ckpt_heap){
           armci_monitor_address_t *addrds =&armci_storage_record[rid].heap_mon;
           /*write the heap information*/
           addr = sbrk(0);
           if(armci_storage_record[rid].ckpt_stack)
             armci_storage_record[rid].heap_mon.fileoffset = armci_storage_record[rid].stack_mon.fileoffset+armci_storage_record[rid].stack_mon.bytes;
           else
             armci_storage_record[rid].heap_mon.fileoffset = 4*sizeof(int)+sizeof(jmp_buf);
           if(addr > (char *)armci_storage_record[rid].heap_mon.ptr){ 
             /*this means change in data segment - save what ever is new and reset size*/
             void *tmpaddr = addrds->ptr;
             void *tmpaddr1 = (void *)((char *)addrds->ptr+addrds->bytes);
             unsigned long firstpage = addrds->firstpage;
             unsigned long totalpages = addrds->totalpages;
             /*first save new pages*/
             ofs=(off_t)(armci_storage_record[rid].heap_mon.fileoffset+totalpages*mypagesize);
             /*problem here - remove mallocs*/
             isheap = 1;
             armci_create_protect_pages(addrds,tmpaddr1,(addr-(char *)tmpaddr1),1);
             isheap = 0;
             rc = armci_storage_write_pages(armci_storage_record[rid].fileinfo.fd,addrds->firstpage,addrds->touched_page_arr,addrds->num_touched_pages,mypagesize,ofs);
             /*now write the touched pages*/
             addrds->ptr = tmpaddr;
             addrds->bytes = addr-(char *)tmpaddr;
             addrds->firstpage = firstpage;
             /*problem here if last data seg addr is not a page boundary*/
             addrds->totalpages+=totalpages;
           }
           /*write touched pages since*/
           ofs=(off_t)(armci_storage_record[rid].heap_mon.fileoffset);
           addrds->num_touched_pages = armci_create_touchedpagearray(addrds->touched_page_arr,addrds->firstpage,addrds->totalpages);
           /*problem here - remove mallocs*/
           if(addrds->num_touched_pages!=0)
             addrds->num_touched_pages = armci_create_touchedpagearray(addrds->touched_page_arr,addrds->firstpage,addrds->totalpages);
           rc = armci_storage_write_pages(armci_storage_record[rid].fileinfo.fd,addrds->firstpage,addrds->touched_page_arr,addrds->num_touched_pages,mypagesize,ofs);
           for(j=0;j<addrds->num_touched_pages;j++){
             addr =(char *)(addrds->touched_page_arr[j]*mypagesize);
             mprotect(addr, mypagesize,PROT_READ);
           }
         }
         else{
           for(i=0;i<armci_storage_record[rid].user_addr_count;i++){
             armci_monitor_address_t *addrds = &armci_storage_record[rid].user_addr[i];
             ofs=(off_t)(addrds->fileoffset);
             if(armci_storage_record[rid].ckpt_stack && i==0)
               addrds->fileoffset = armci_storage_record[rid].stack_mon.fileoffset+armci_storage_record[rid].stack_mon.bytes;
             if(i!=0)
               addrds->fileoffset = armci_storage_record[rid].user_addr[i-1].fileoffset+armci_storage_record[rid].user_addr[i-1].bytes;
             ofs = addrds->fileoffset;
             if(addrds->num_touched_pages!=0)
               addrds->num_touched_pages = armci_create_touchedpagearray(addrds->touched_page_arr,addrds->firstpage,addrds->totalpages);
             rc = armci_storage_write_pages(armci_storage_record[rid].fileinfo.fd,addrds->firstpage,addrds->touched_page_arr,addrds->num_touched_pages,mypagesize,ofs);
             for(j=0;j<addrds->num_touched_pages;j++){
               addr =(char *)(addrds->touched_page_arr[j]*mypagesize);
               mprotect(addr, mypagesize,PROT_READ);
             }
             bzero(addrds->touched_page_arr,sizeof(unsigned long)*addrds->num_touched_pages);
             addrds->num_touched_pages = 0;
           }
         }
      }
      else { /*long jump brings us here */
         /*open the ckpt files*/
      }
    }
    else{
       for(i=0;i<armci_storage_record[rid].user_addr_count;i++){
         armci_monitor_address_t *addrds = &armci_storage_record[rid].user_addr[i];
         ofs=(off_t)(addrds->fileoffset);
         if(armci_storage_record[rid].ckpt_stack && i==0)
           addrds->fileoffset = armci_storage_record[rid].stack_mon.fileoffset+armci_storage_record[rid].stack_mon.bytes;
         if(i!=0)
           addrds->fileoffset = armci_storage_record[rid].user_addr[i-1].fileoffset+armci_storage_record[rid].user_addr[i-1].bytes;
         ofs = addrds->fileoffset;
         if(addrds->num_touched_pages!=0)
           addrds->num_touched_pages = armci_create_touchedpagearray(addrds->touched_page_arr,addrds->firstpage,addrds->totalpages);
         rc = armci_storage_write_pages(armci_storage_record[rid].fileinfo.fd,addrds->firstpage,addrds->touched_page_arr,addrds->num_touched_pages,mypagesize,ofs);
         for(j=0;j<addrds->num_touched_pages;j++){
           addr =(char *)(addrds->touched_page_arr[j]*mypagesize);
           mprotect(addr, mypagesize,PROT_READ);
         }
         bzero(addrds->touched_page_arr,sizeof(unsigned long)*addrds->num_touched_pages);
         addrds->num_touched_pages = 0;
       }
    }
         
    armci_msg_group_barrier(&armci_storage_record[rid].group);
    return(rc); /* 0 is good*/
}


int armci_irecover(int rid,int iamreplacement)
{
    int rc;
    off_t ofs;
    /*restore jmpbuf and pid and longjmp*/
    if(iamreplacement){
      /*first create a new armci group and make it the armci ft group*/
      rc=armci_storage_read_ptr(armci_storage_record[rid].fileinfo.fd,&armci_storage_record[rid].jmp,sizeof(jmp_buf),4*sizeof(int));
    }
    armci_msg_group_barrier(&armci_storage_record[rid].group);
    longjmp(armci_storage_record[rid].jmp,1);

    /*we should never come here things are hosed */
    armci_die2("recovery hosed",rid,iamreplacement);
    return(1);
}


void armci_icheckpoint_finalize(int rid)
{
    int i;
    //armci_msg_group_barrier(&armci_storage_record[rid].group);
    for(i=0;i<armci_storage_record[rid].user_addr_count;i++){
       armci_monitor_address_t *addrds=&armci_storage_record[rid].user_addr[i];
       free(addrds->touched_page_arr);
    }
    free(armci_storage_record[rid].user_addr);
    free(armci_storage_record[rid].fileinfo.filename);
    armci_storage_fclose(armci_storage_record[rid].fileinfo.fd);
    next_available_rid = rid;
}

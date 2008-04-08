/* $Id: winshmem.c,v 1.20 2005-09-29 21:30:10 d3h325 Exp $ */
/* WIN32 & Posix SysV-like shared memory allocation and management
 * 
 *
 * Interface:
 * ~~~~~~~~~
 *  char *Create_Shared_Region(long *idlist, long *size, long *offset)
 *       . to be called by just one process. 
 *       . calls kr_malloc,  malloc-like
 *         memory allocator from K&R.kr_malloc inturn calls armci_allocate() that
 *         does shmget() and shmat(). 
 *       . idlist might be just a pointer to integer or a true array in the
 *         MULTIPLE_REGIONS versions (calling routine has to take cere of it) 
 *  char *Attach_Shared_Region(long *idlist, long *size, long *offset)
 *       . called by any other process to attach to existing shmem region or
 *         if attached just calculate the address based on the offset
 *       . has to be called after shmem region was created
 *  void  Free_Shmem_Ptr(long id, long size, char* addr)
 *       . called ONLY by the process that created shmem region (id) to return
 *         pointer to kr_malloc (shmem is not destroyed)
 *  long  Delete_All_Regions()
 *       . destroys all shared memory regions
 *       . can be called by any process assuming that all processes attached
 *         to alllocated shmem regions 
 *       . needs to by called by cleanup procedure(s)
 */


#define DEBUG 0
#define DEBUG0 0

#include <stdio.h>

#ifdef WIN32
#  include <windows.h>
#  include <process.h>
#  define  GETPID _getpid
#elif defined(NEC)
#  include <unistd.h>
#  include <sys/mppg.h>
   typedef void* HANDLE;
   typedef void* LPVOID;
#  define  GETPID getpid
#elif defined(HITACHI)
#  include <unistd.h>
#  define PAGE_SIZE       0x1000
#  define ROUND_UP_PAGE(size)  ((size + (PAGE_SIZE-1)) & ~(PAGE_SIZE-1))
#  include <strings.h>
#  include <stdlib.h>
#  include <hxb/combuf.h>
#  include <hxb/combuf_returns.h>
   typedef long HANDLE;
   typedef char* LPVOID;
#  define  GETPID getpid
   static long cb_key=1961;
   static long _hitachi_reg_size;
#elif defined(MMAP)
#  include <fcntl.h>
#  include <unistd.h>
#  include <sys/stat.h>
#  include <sys/mman.h>
   typedef int HANDLE;
   typedef void* LPVOID;
#  define  GETPID getpid
#else
#  ifndef _POSIX_C_SOURCE
#    define  _POSIX_C_SOURCE 199309L
#  endif
#  include <fcntl.h>
#  include <unistd.h>
#  include <sys/stat.h>
#  include <sys/mman.h>
   typedef int HANDLE; 
   typedef void* LPVOID; 
#  define  GETPID getpid
#endif

#include <assert.h>
#include "kr_malloc.h"
#include "shmem.h"
#include "armcip.h"

#define SHM_UNIT (1024)

/* default unit for shared memory allocation in KB! */
#if defined(WIN32)
#  define _SHMMAX  32678      
#elif defined(MACX)
#  define _SHMMAX  64*1024
#else
#  define _SHMMAX  2*32678      
#endif

#define SET_MAPNAME(id) sprintf(map_fname,"/tmp/ARMCIshmem.%d.%d",parent_pid,(id))

/*********************** global data structures ****************************/

/* Terminology
 *   region - actual piece of shared memory allocated from OS
 *   block  - a part of allocated shmem that is given to the requesting process
 */


/* array holds handles and addreses for each shmem region*/ 
static struct shm_region_list{
   char     *addr;
   HANDLE   id;
   long      size;
}region_list[MAX_REGIONS];

static char map_fname[64];
static  int alloc_regions=0;   /* counter to identify mapping handle */
static  int last_allocated=0; /* counter trailing alloc_regions by 0/1 */

/* Min and Max amount of aggregate memory that can be allocated */
static  unsigned long MinShmem = _SHMMAX;  
static  unsigned long MaxShmem = MAX_REGIONS*_SHMMAX;
static  context_t ctx_winshmem;    /* kr_malloc context */
static  context_t *ctx_winshmem_global;/*for processor groups,kr_malloc context
					is stored in shared memory */
static  int parent_pid=-1;  /* process id of process 0 "parent" */

extern void armci_die(char*,int);
extern int armci_me;

 
/* Create shared region to store kr_malloc context in shared memory */
void armci_krmalloc_init_ctxwinshmem() {
    void *myptr=NULL;
    long idlist[SHMIDLEN];
    long size;
    int offset = sizeof(void*)/sizeof(int);

    /* to store shared memory context and  myptr */
    size = SHMEM_CTX_MEM;
    
    if(armci_me == armci_master ){
       myptr = Create_Shared_Region(idlist+1,size,idlist);
       if(!myptr && size>0 ) armci_die("armci_krmalloc_init_ctxwinshmem: could not create", (int)(size>>10));
       if(size) *(volatile void**)myptr = myptr;
       if(DEBUG){
          printf("%d:armci_krmalloc_init_ctxwinshmem addr mptr=%p ref=%p size=%ld\n", armci_me, myptr, *(void**)myptr, size);
          fflush(stdout);
       }
       
       /* Bootstrapping: allocate storage for ctx_winshmem_global. NOTE:there
          is offset,as master places its addr at begining for others to see */
       ctx_winshmem_global = (context_t*) ( ((int*)myptr)+offset );
       *ctx_winshmem_global=ctx_winshmem;/*master copies ctx into shared rgn*/
    }
 
    /* broadcast shmem id to other processes on the same cluster node */
    armci_msg_clus_brdcst(idlist, SHMIDLEN*sizeof(long));
    
    if(armci_me != armci_master){
       myptr=(double*)Attach_Shared_Region(idlist+1,size,idlist[0]);
       if(!myptr)armci_die("armci_krmalloc_init_ctxwinshmem: could not attach", (int)(size>>10));
       
       /* now every process in a SMP node needs to find out its offset
        * w.r.t. master - this offset is necessary to use memlock table
        */
       if(size) armci_set_mem_offset(myptr);
       if(DEBUG){
          printf("%d:armci_krmalloc_init_ctxwinshmem attached addr mptr=%p ref=%p size=%ld\n", armci_me,myptr, *(void**)myptr,size); fflush(stdout);
       }
       /* store context info */
       ctx_winshmem_global = (context_t*) ( ((int*)myptr)+offset );
       if(DEBUG){
          printf("%d:armci_krmalloc_init_ctxwinshmem:shmid=%d off=%ld size=%ld\n", armci_me, ctx_winshmem_global->shmid,ctx_winshmem_global->shmoffset,
                 (long)ctx_winshmem_global->shmsize);
          fflush(stdout);
       }
    }
}

/* not done here yet */
void armci_shmem_init() {
    armci_krmalloc_init_ctxwinshmem();
}

unsigned long armci_max_region()
{
  return MinShmem;
}

/*\ application can reset the upper limit for memory allocation
\*/
void armci_set_shmem_limit(unsigned long shmemlimit) /* comes in bytes */ 
{
     unsigned long kbytes;
     kbytes = (shmemlimit + SHM_UNIT -1)/SHM_UNIT;
     if(MaxShmem > kbytes) MaxShmem = kbytes;
     if(MinShmem > kbytes) MinShmem = kbytes;
}


void Delete_All_Regions()
{
int reg;

  for(reg = 0; reg < alloc_regions; reg++){
    if(region_list[reg].addr != (char*)0){
#       if defined(WIN32)
          UnmapViewOfFile(region_list[reg].addr);
          CloseHandle(region_list[reg].id);
#       elif defined(NEC)
          (int)dp_xmfree(region_list[reg].addr);
#       else
          munmap(region_list[reg].addr, region_list[reg].size);
          SET_MAPNAME(reg);
          (void*)unlink(map_fname);
#       endif
        region_list[reg].addr = (char*)0;
    }
  }
}


/*\ only process that created shared region returns the pointer to kr_malloc 
\*/
void Free_Shmem_Ptr(long id, long size, char* addr)
{  
  kr_free(addr, ctx_winshmem_global);
}


char *armci_get_core_from_map_file(int exists, long size)
{
    LPVOID  ptr;

#if defined(HITACHI)

    Cb_object_t oid;
    int desc;

    region_list[alloc_regions].addr = (char*)0;
    if(exists){ 
      int rc,nsize=_hitachi_reg_size;
      if(size < MinShmem*SHM_UNIT) size = MinShmem*SHM_UNIT;
      nsize = ROUND_UP_PAGE(nsize);
 
      if((rc=combuf_object_get(region_list[alloc_regions].id,(Cb_size_t)nsize,0, &oid))
                       != COMBUF_SUCCESS) armci_die("attaching combufget fail",0);
      if((rc=combuf_map(oid, 0, (Cb_size_t)nsize, COMBUF_COMMON_USE, &ptr)) 
                       != COMBUF_SUCCESS) armci_die("combuf map failed",0);
       
    }else{
      int rc;
      size = ROUND_UP_PAGE(size);

      if((rc=combuf_object_get(cb_key,(Cb_size_t)size,COMBUF_OBJECT_CREATE,&oid))
                       != COMBUF_SUCCESS) armci_die("creat combufget fail",0);
      if((rc=combuf_map(oid, 0, (Cb_size_t)size, COMBUF_COMMON_USE, &ptr)) 
                       != COMBUF_SUCCESS) armci_die("combuf map failed",0);

      /* make the region suitable for communication */
      if(combuf_create_field(oid, ptr, (Cb_size_t)size, FIELD_NUM, 0, 0, &desc)
                       != COMBUF_SUCCESS) armci_die("create field failed",0);
    
      region_list[alloc_regions].id = cb_key;
      _hitachi_reg_size=size;
      cb_key++; /* increment for next combuf create call */
     
    }

#elif defined(NEC)

    region_list[alloc_regions].addr = (char*)0;
    if(exists)
       ptr = dp_xmatt(parent_pid, region_list[alloc_regions].id, (void*)0);  
    else {
       ptr = dp_xmalloc((void*)0, (long long) size);
       region_list[alloc_regions].id = ptr;
    }

    if(ptr == (void*)-1) return ((char*)0); 
       
#else
    HANDLE  h_shm_map;
    SET_MAPNAME(alloc_regions);
    region_list[alloc_regions].addr = (char*)0;

#if defined(WIN32)
    h_shm_map = CreateFileMapping(INVALID_HANDLE_VALUE,
                NULL, PAGE_READWRITE, 0, (int)size, map_fname);
    if(h_shm_map == NULL) return NULL;
    
    if(exists){
       /* get an error code when mapping should exist */
       if (GetLastError() != ERROR_ALREADY_EXISTS){
          CloseHandle(h_shm_map);
          fprintf(stderr,"map handle does not exist (attach)\n");
          return NULL;
       }else { /* OK */ }
    } else {
        /* problem if mapping it should not be there */
        if (GetLastError() == ERROR_ALREADY_EXISTS){
          CloseHandle(h_shm_map);
          fprintf(stderr,"map handle already exists (create)\n");
          return NULL;
        }
    }
     /* now map file into process address space */
    ptr = MapViewOfFile(h_shm_map, 
                        FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
    if((char*)ptr == NULL){
       CloseHandle(h_shm_map);
       h_shm_map = INVALID_HANDLE_VALUE;
    }
#elif defined(MMAP)&&!defined(HITACHI) && !defined(MACX)

    if(exists){
       if(size < MinShmem*SHM_UNIT) size = MinShmem*SHM_UNIT;
       h_shm_map = open(map_fname, O_RDWR, S_IRWXU);
       if(h_shm_map <0) return NULL;
    }else{
       (void*)unlink(map_fname); /* sanity cleanup */
       h_shm_map = open(map_fname, O_CREAT|O_RDWR, S_IRWXU);
       if(h_shm_map <0) return NULL;
       if(ftruncate(h_shm_map,size) < 0) return NULL;
    }

    ptr = mmap((caddr_t)0, (size_t)size, PROT_READ|PROT_WRITE,
                                         MAP_SHARED, h_shm_map, 0);

    close(h_shm_map);
    h_shm_map = -1;
#elif defined(MACX)

    if(exists){
       if(size < MinShmem*SHM_UNIT) size = MinShmem*SHM_UNIT;
       h_shm_map = shm_open(map_fname, O_RDWR, S_IRWXU);
       if(h_shm_map == -1) return NULL;
    }else{
       (void*)shm_unlink(map_fname); /* sanity cleanup */
       h_shm_map = shm_open(map_fname, O_CREAT|O_RDWR, S_IRWXU);
       if(h_shm_map<0) perror("open");
       if(h_shm_map == -1) return NULL;
       if(ftruncate(h_shm_map,size) < 0){
            perror("ftruncate");
            return NULL;
       }
    }

    ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, h_shm_map, 0L);
    if((long)ptr <0){ perror("mmap"); return NULL; }

    close(h_shm_map);
    h_shm_map = -1;
    
#else

    if(exists){
       h_shm_map = shm_open(map_fname, O_RDWR, S_IRWXU);
       if(h_shm_map == -1) return NULL;
    }else{
       (void*)shm_unlink(map_fname); /* sanity cleanup */
       h_shm_map = shm_open(map_fname, O_CREAT|O_RDWR, S_IRWXU);
       if(h_shm_map) perror("shm_open");
       if(h_shm_map == -1) return NULL;
       if(ftruncate(h_shm_map,size) < 0) return NULL;
    }

    ptr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, h_shm_map, 0L);

    close(h_shm_map);
    h_shm_map = -1;

#endif

    /*     save file handle in the array to close it in the future */
    region_list[alloc_regions].id   = h_shm_map;

#endif

    if(DEBUG0){printf("%d: got ptr=%p bytes=%ld mmap\n",armci_me,ptr,size); fflush(stdout); }
    region_list[alloc_regions].addr = (char*)ptr;
    region_list[alloc_regions].size = size;

    return((char*)ptr);
}


int find_regions(char *addrp,  long* id, int *region)
{
int nreg, reg;

    if(last_allocated!=-1){
       reg=last_allocated;
       last_allocated = -1;
    } else{
       for(reg=0,nreg=0;nreg<alloc_regions; nreg++){
          if(region_list[nreg].addr > addrp )break;
          reg = nreg;
       }
    }

    *region = reg;
    *id = reg;

    return 1;
}


/*\ function called by shared memory allocator (kr_malloc)
\*/
char *armci_allocate(size_t size)
{
    char *ptr;

    if(alloc_regions>= MAX_REGIONS)
        armci_die("max alloc regions exceeded", alloc_regions);

    ptr = armci_get_core_from_map_file( 0, (long)size);
    if(ptr !=NULL) alloc_regions++;
    if(DEBUG)printf("%d:got more core %lx %ld this was %ld segment allocated\n",armci_me,(unsigned long)ptr, (long)size, alloc_regions);

    return ptr;
}


char* Create_Shared_Region(long idlist[], long size, long *offset)
{
    char *temp;
    int reg;

    /*initialization */
    if(!alloc_regions){
          for(reg=0;reg<MAX_REGIONS;reg++){
            region_list[reg].addr=(char*)0;
            region_list[reg].id=0;
            parent_pid = GETPID();
          }
          kr_malloc_init(SHM_UNIT, (unsigned)MinShmem, (unsigned)MaxShmem, 
			 (void *)armci_allocate, 0, &ctx_winshmem);
	  ctx_winshmem.ctx_type = KR_CTX_SHMEM;
    }

    if(!alloc_regions) temp = kr_malloc((unsigned)size, &ctx_winshmem);
    else temp = kr_malloc((unsigned)size, ctx_winshmem_global);

     if(temp == (char*)0 )
           armci_die("Create_Shared_Region: kr_malloc failed ",0);
    
     /* find if kr_malloc allocated a new shmem region */
     if(!find_regions(temp,idlist,&reg))
        armci_die("CreateSharedRegion: allocation inconsitent",0);

     *offset = (long) (temp - region_list[reg].addr);

     if(DEBUG)printf("Create_Shared_Region(): Found region reg=%d (id=%d) offset=%d temp=%p\n", reg, idlist[0], *offset, temp); 

     /* idlist[0] = alloc_regions; This is set in find_regions() */
     idlist[1] = parent_pid;
#if  defined(HITACHI) || defined(NEC)
     idlist[2] = (long) region_list[reg].id;
#if  defined(HITACHI)
     idlist[SHMIDLEN-2]=_hitachi_reg_size;
#endif
#endif
     if(DEBUG)printf("%d:created %p %ld id=%ld id[0]=%d\n",armci_me,temp, size,idlist[2],idlist[0]);
     return (temp);
}

#ifdef HITACHI
void server_reset_memory_variables()
{
	alloc_regions=0;
	parent_pid=-1;
}
#endif

char *Attach_Shared_Region(long id[], long size, long offset)
{
    int found=0;
    int reg;
    char *temp=NULL;

    /*initialization */
    if(!alloc_regions){
          for(reg=0;reg<MAX_REGIONS;reg++){
            region_list[reg].addr=(char*)0;
            region_list[reg].id=0;
            parent_pid= (int) id[1];
          }
     }

     if(DEBUG)printf("%d:alloc_regions=%d size=%ld attachto(id)=%d\n",armci_me,alloc_regions,size, id[0]);

     /* find out if a new shmem region was allocated */
     if(alloc_regions < id[0]+1){
#if      defined(HITACHI) || defined(NEC)
#if        defined(HITACHI)
               _hitachi_reg_size=id[SHMIDLEN-2];
#          endif

               region_list[alloc_regions].id = (HANDLE) id[2];
#        endif
         if(DEBUG)printf("alloc_regions=%d size=%ld\n",alloc_regions,size);
         temp = armci_get_core_from_map_file(1,size);
         if(temp != NULL)alloc_regions++;
         else return NULL;
     }

     /* search region_list for the current shmem id */
     /* for(found = 0, reg=0; reg < MAX_REGIONS;reg++)
         if((found=(region_list[reg].id == *id)))break;*/
       
     /*if( found){*/
     if(id[0] <= alloc_regions) {
         temp = region_list[*id].addr + offset;
     } else armci_die2("Attach_Shared_Region:inconsistency in counters",
                      alloc_regions, (int) id[0]);

      if(DEBUG)fprintf(stderr,"\n%d:attach succesful off=%ld ptr=%p id[0]=%ld\n",armci_me,offset,temp,id[0]);
     return(temp);
}



long armci_shmem_reg_size(int i, long id)
{
     if(i<0 || i>= MAX_REGIONS)armci_die("armci_shmem_reg_size: bad i",i); 
     if(region_list[i].id !=(HANDLE)id)armci_die("armci_shmem_reg_size id",(int)id);
     return region_list[i].size;
}

char* armci_shmem_reg_ptr(int i)
{
     if(i<0 || i>= MAX_REGIONS)armci_die("armci_shmem_reg_ptr: bad i",i); 
     return region_list[i].addr;
}

int armci_get_shmem_info(char *addrp,  int* shmid, long *shmoffset,
                         size_t *shmsize)
{
    int region;
    long idlist[SHMIDLEN];

    if(!find_regions(addrp,idlist,&region))
        armci_die("CreateSharedRegion: allocation inconsitent",0);
 
    *shmid     = (int) (*idlist);
    *shmoffset = (long)(addrp - region_list[region].addr);
    *shmsize   = region_list[region].size;

#if DEBUG
    printf("%d: armci_get_shmem_info: shmid=%d offset=%ld size=%ld %p\n",
           armci_me, *shmid, *shmoffset, *shmsize, addrp);
    fflush(stdout);
#endif    
    return 1;
}

Header *armci_get_shmem_ptr(int shmid, long shmoffset, size_t shmsize)
{
    /* returns, address of the shared memory region based on shmid, offset.
     * (i.e. return_addr = stating address of shmid + offset)*/
    long idlist[SHMIDLEN];
    Header *p = NULL;

    idlist[1] = (long)shmid;
    idlist[0] = shmoffset;
    idlist[IDLOC+1] = shmsize;

    if(!(p=(Header*)Attach_Shared_Region(idlist+1, shmsize, shmoffset)))
       armci_die("armci_get_shmem_ptr: could not attach",
                 (int)(shmsize>>10));
#if DEBUG
    printf("%d: armci_get_shmem_ptr: shmid=%d offset=%ld size=%ld %p\n",
           armci_me, shmid, shmoffset, shmsize, p);
    fflush(stdout);
#endif
    return p;
}

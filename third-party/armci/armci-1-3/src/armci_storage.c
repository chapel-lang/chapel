#include <stdio.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include "armci_storage.h"
extern int armci_me;
extern void armci_die(char *msg, int code);
#define DEBUG 0
FILE_DS armci_storage_fopen(char *filename)
{
    FILE_DS file_d;
    file_d = fopen(filename,"w+");
   if(DEBUG) printf("\n%d:filed=%p %s",armci_me,file_d,filename);
    return(file_d);
}
void armci_storage_fclose(FILE_DS filed)
{
    int rc = fclose(filed);
    if(rc !=0)printf("file could not be close");
}

int armci_storage_read_ptr(FILE_DS file_d,void *ptr,int size,off_t ofs)
{
    int rc=0,orc=0,isize=size;
    
    rc = fseek(file_d,ofs,SEEK_SET);
    if(rc)armci_die("fseek failed in armci_storage_read_ptr",rc);

    while(orc!=isize){
       rc = fread(ptr,1,size,file_d);
       orc+=rc;
       if(DEBUG)printf("\n%d:read %d so far of %d\n",armci_me,orc,isize);
       if(orc!=isize){
         ptr+=rc;
         size-=rc;
       }
    }
    rc = fseek(file_d,0,SEEK_SET);
    if(rc)armci_die("fseek failed in armci_storage_read_ptr",rc);
    return 0;
}

int armci_storage_read_pages(FILE_DS file_d, unsigned long first_page,
                unsigned long *page_arr, unsigned long page_arr_sz,int pagesize,
                off_t ofs)
{
    int i,rc=0;
    /*this can be heavily optimized*/ 
    for(i=0;i<page_arr_sz;i++){
       off_t ofs1 = ofs+(page_arr[i]-first_page)*pagesize;
       void *ptr = (void *)(page_arr[i]*pagesize);
       rc=armci_storage_read_ptr(file_d,ptr,pagesize,ofs1);
    }
    return rc;
}

int armci_storage_write_ptr(FILE_DS file_d,void *ptr,int size,off_t ofs)
{
    int rc=0,orc=0,isize=size;
    if(DEBUG)printf("in storage_write_ptr %ld is ofs\n",ofs); 
    rc = fseek(file_d,ofs,SEEK_SET);
    if(rc)armci_die("fseek failed in armci_storage_write_ptr",rc);

    while(orc!=isize){
       rc = fwrite(ptr,1,size,file_d);
       orc+=rc;
       if(DEBUG)printf("\n%d:wrote %d so far of %d\n",armci_me,orc,isize);
       if(orc!=isize){
         ptr+=rc;
         size-=rc;
       }
    }
    rc = fseek(file_d,0,SEEK_SET);
    if(rc)armci_die("fseek failed in armci_storage_write_ptr",rc);
    return 0;
}

int armci_storage_write_pages(FILE_DS file_d, unsigned long first_page,
                unsigned long *page_arr, unsigned long page_arr_sz,int pagesize,
                off_t ofs)
{
    int i,rc=0;
    /*this can be heavily optimized*/ 
    for(i=0;i<page_arr_sz;i++){
       off_t ofs1 = ofs+(page_arr[i]-first_page)*pagesize;
       void *ptr = (void *)(page_arr[i]*pagesize);
       rc = armci_storage_write_ptr(file_d,ptr,pagesize,ofs1);
    }
    return 0;
}

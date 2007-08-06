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
#define FILE_DS FILE *
FILE_DS armci_storage_fopen(char *filename);
int armci_storage_read_ptr(FILE_DS file_d,void *ptr,int size,off_t ofs);
int armci_storage_read_pages(FILE_DS file_d, unsigned long first_page,
                unsigned long *page_arr, unsigned long page_arr_sz,int pagesize,
                off_t ofs);
int armci_storage_write_ptr(FILE_DS file_d,void *ptr,int size,off_t ofs);
int armci_storage_write_pages(FILE_DS file_d, unsigned long first_page,
                unsigned long *page_arr, unsigned long page_arr_sz,int pagesize,
                off_t ofs);

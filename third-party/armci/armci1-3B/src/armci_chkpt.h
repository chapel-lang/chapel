#include "armci_storage.h"
typedef struct{
    void *ptr;
    size_t bytes;
    int saveonce;
    unsigned long *touched_page_arr;
    unsigned long num_touched_pages;
    unsigned long firstpage;
    unsigned long totalpages;
    unsigned long fileoffset;
} armci_monitor_address_t;

typedef struct{
    FILE_DS fd;
    long filestartindex;
    int filestatus;
    char *filename;
} armci_file_info_t;

typedef struct{
    unsigned long num_touched_pages;
    unsigned long *touched_page_arr;
    unsigned long firstpage;
    unsigned long lastpage;
}armci_page_info_t;

typedef struct{
    int rid;                         /*unique record id*/ 
    int pid;                         /*id of the process*/
    int rel_pid;                     /*group pid for the process*/
    int tmp;                         /*for jmp_buf alignment*/
    jmp_buf jmp;                     /*the jmp buffer for setjmp and longjmp*/
    int ckpt_heap,ckpt_stack;
    armci_monitor_address_t stack_mon,heap_mon;
    armci_monitor_address_t *user_addr;
    int user_addr_count;
    armci_file_info_t fileinfo;
    ARMCI_Group group;
} armci_storage_record_t;

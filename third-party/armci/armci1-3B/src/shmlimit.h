#ifndef _SHMLIMIT_H_
#define _SHMLIMIT_H_

extern int armci_me, armci_master;
extern void armci_die(char *, int);
extern int armci_shmem_test();
extern int armci_child_shmem_init();
#endif

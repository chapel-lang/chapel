#ifndef _ARMCI_VAMPIR_H_
#define _ARMCI_VAMPIR_H_

/* This file defines the codes that are used to label
   ARMCI routines in VAMPIR
*/
#include <VT.h>
#include <ga_vt.h>

#define ARMCI_INIT            32500
#define ARMCI_FINALIZE        32499
#define ARMCI_PUTV            32498
#define ARMCI_NBPUTV          32497
#define ARMCI_GETV            32496
#define ARMCI_NBGETV          32495
#define ARMCI_PUTS            32494
#define ARMCI_NBPUTS          32493
#define ARMCI_GETS            32492
#define ARMCI_NBGETS          32491
#define ARMCI_WAIT            32490
#define ARMCI_FENCE           32489
#define ARMCI_ALLFENCE        32488
#define ARMCI_POLL            32487
#define ARMCI_MALLOC          32486
#define ARMCI_FREE            32485
#define ARMCI_CLEANUP         32484
#define ARMCI_ERROR           32483
#define ARMCI_RMW             32482
#define ARMCI_LOCK            32481
#define ARMCI_UNLOCK          32480
#define ARMCI_ACCV            32479
#define ARMCI_ACCS            32478
#define ARMCI_COPY            32477
#define ARMCI_SET_SHMEM_LIMIT 32476
#define ARMCI_USES_SHM        32475
#define ARMCI_CREATE_MUTEXES  32474
#define ARMCI_DESTROY_MUTEXES 32473

extern void armci_vampir_init();

#endif


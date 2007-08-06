/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ---------------------------------------------------------------- */
/* Product(s):                                                      */
/* 5733-BG1                                                         */
/*                                                                  */
/* (C)Copyright IBM Corp. 2004, 2006                                */
/* All rights reserved.                                             */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* Licensed Materials-Property of IBM                               */
/* --------------------------------------------------------------   */
/* end_generated_IBM_copyright_prolog                               */
/********************************************************************/

#ifndef _bgmldefs_h
#define _bgmldefs_h

void wait_callback(void *clientdata);
void BGML_Wait(unsigned *clientdata);
void BGML_WaitProc(int proc);
void BGML_WaitAll();

void bgml_init_locks (void * local_memlock_table);
void bgml_lockmem (void *start, void *end, int proc);
void bgml_unlockmem (int proc);

typedef void (*BGML_Barrier)  (unsigned);
extern BGML_Barrier bgml_barrier;

#endif

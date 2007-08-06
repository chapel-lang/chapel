/* begin_generated_IBM_copyright_prolog                             */
/*                                                                  */
/* This is an automatically generated copyright prolog.             */
/* After initializing,  DO NOT MODIFY OR MOVE                       */
/* ---------------------------------------------------------------- */
/* Product(s):                                                      */
/* 5733-BG1                                                         */
/*                                                                  */
/* (C)Copyright IBM Corp. 2004, 2004                                */
/* All rights reserved.                                             */
/* US Government Users Restricted Rights -                          */
/* Use, duplication or disclosure restricted                        */
/* by GSA ADP Schedule Contract with IBM Corp.                      */
/*                                                                  */
/* Licensed Materials-Property of IBM                               */
/* --------------------------------------------------------------   */
/* end_generated_IBM_copyright_prolog                               */
/********************************************************************/



#include <assert.h>
#include "bgml.h"
#include "bgmldefs.h"
#include <unistd.h>
#include "memlock.h"
#include<stdio.h>

void wait_callback(void *clientdata)
{
   unsigned *data = (unsigned *)clientdata;
   data[0]--;
}


void BGML_Wait(unsigned *clientdata)
{
   BGML_CriticalSection_enter();
   while(*clientdata)
      BGML_Messager_advance();
   BGML_CriticalSection_exit();
}

   
void BGML_WaitProc(int proc)
{
   unsigned wait_proc_not_done=1;
   BGML_Callback_t cb_wait_proc = {wait_callback, &wait_proc_not_done};

   if(BG1S_WaitProc(proc, &cb_wait_proc))
   {
      BGML_CriticalSection_enter();
      while (wait_proc_not_done)
         BGML_Messager_advance();
      BGML_CriticalSection_exit();
   }
}

void BGML_WaitAll()
{
   unsigned wait_all_not_done = 1;
   BGML_Callback_t cb_wait_all = {wait_callback, &wait_all_not_done};

   if(BG1S_WaitAll (&cb_wait_all))
   {
      BGML_CriticalSection_enter();
      while(wait_all_not_done) 
         BGML_Messager_advance();
      BGML_CriticalSection_exit();
   }
}

typedef struct bgml_lockinfo_t
{
  void     * start;
  void     * end;
  unsigned   unused[2];
}
bgml_lockinfo_t;

typedef struct bgml_keyinfo_t
{
  unsigned   slot;
  unsigned   unused[3];
}
bgml_keyinfo_t;

unsigned bgml_lock (BGQuad * lockinfo, void * clientdata, BGQuad * out)
{
  unsigned acquired_lock = 0;
  
  memlock_t * local_memlock_table = (memlock_t *) clientdata;
  bgml_lockinfo_t * info = (bgml_lockinfo_t *) lockinfo;
  void * pstart = info->start;
  void * pend = info->end;
  
  /* inspect the table */
  unsigned conflict = 0; 
  unsigned avail = -1;
  unsigned slot = 0;
  for(; slot < MAX_SLOTS; slot++)
    {
      /* nonzero starting address means the slot is occupied */ 
      if(local_memlock_table[slot].start == NULL)
        {
          /* remember a free slot to store address range */
          avail = slot;  
        }
      else
        {
          /* check for conflict: overlap between stored and current range */
          if ( (pstart >= local_memlock_table[slot].start && 
                pstart <= local_memlock_table[slot].end) || 
               (pend >= local_memlock_table[slot].start && 
                pend <= local_memlock_table[slot].end) )
            {
              conflict = 1;
              break;
            }
        }
    }
    
  if (avail != -1 && !conflict)
    {
      /* acquired the memory lock: enter address into the table */
      local_memlock_table[avail].start = pstart;
      local_memlock_table[avail].end = pend;
      
      acquired_lock = 1;
      bgml_keyinfo_t * key = (bgml_keyinfo_t *) out;
      key->slot = avail;
    }
  else
    {
      /* There is only one "lock resource" for ARMCI - 
       * the local memlock table - so set the lock identifier to 0. */
      unsigned long long * tmp = (unsigned long long *) out;
      tmp[0] = 0;
      tmp[1] = 0;
    }
    
  return acquired_lock;
}

BGQuad bgml_unlock (BGQuad * keyinfo, void * clientdata)
{
  bgml_keyinfo_t * key = (bgml_keyinfo_t *) keyinfo;
  memlock_t * local_memlock_table = (memlock_t *) clientdata;
  
  local_memlock_table[key->slot].start = NULL;
  local_memlock_table[key->slot].end = NULL;

   /* There is only one "lock resource" for ARMCI - 
   * the local memlock table - so set the lock identifier to 0. */
  BGQuad out; 
  unsigned long long * tmp = (unsigned long long *) &out;
  tmp[0] = 0; tmp[1] = 0;
  
  return out; 
}

void bgml_init_locks (void * local_memlock_table)
{
  BG1S_Configuration_t configuration = BG1S_Configure (NULL);
  configuration.cb_lock = bgml_lock;
  configuration.cb_lock_clientdata = local_memlock_table;
  configuration.cb_unlock = bgml_unlock;
  configuration.cb_unlock_clientdata = local_memlock_table;
  BG1S_Configure (&configuration);
}

static bgml_keyinfo_t key;

void bgml_lockmem (void *start, void *end, int proc)
{
  static unsigned bgml_lockmem_count = 0;
  
  bgml_lockinfo_t lock;
  lock.start     = start;
  lock.end       = end;
  lock.unused[0] = 0;
  lock.unused[1] = 0;
  
  unsigned waiting = 1;
  BGML_Callback_t cb_done = {wait_callback, &waiting};
  
  BG1S_t request;
  
  BGML_CriticalSection_enter();
  BG1S_Lock (&request,
             proc,
             (const BGQuad *) &lock,
             (const BGQuad *) &key,
             &cb_done,
             1);
  while (waiting) BGML_Messager_advance();
  BGML_CriticalSection_exit();
}

void bgml_unlockmem (int proc)
{
  static unsigned bgml_unlockmem_count = 0;
  
  unsigned waiting = 1;
  BGML_Callback_t cb_done = {wait_callback, &waiting};
  
  BG1S_t request;
  
  BGML_CriticalSection_enter();
  BG1S_Unlock (&request,
               proc,
               (const BGQuad *) &key,
               &cb_done,
               1);
  while (waiting) BGML_Messager_advance();
  BGML_CriticalSection_exit();
}

BGML_Barrier bgml_barrier;






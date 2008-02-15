/* $Id: mutex.c,v 1.24.8.1 2006/12/28 01:23:30 manoj Exp $ */
#include "armcip.h"
#include "copy.h"
#include "request.h"
#include <stdio.h>

#define DEBUG 0
#define MAX_LOCKS 32768
#define SPINMAX 1000

#if defined(LAPI) || defined(GM)
#  define SERVER_LOCK 
#endif

double _dummy_work_=0.;
#ifdef LAPI /*fix to if cmpl handler for a pending unlock runs after destroy*/
int mymutexcount;
double _dummy_server_work_=0.;
#endif
static int num_mutexes=0, *tickets; 

typedef struct {
       int mutex;
       int turn;
       msg_tag_t tag;
} waiting_list_t;


/* data structure to store info about blocked (waiting) process for mutex */
static waiting_list_t* blocked=(waiting_list_t*)0; 


typedef struct {
int* token;
int* turn;
int* tickets;
int count;
} mutex_entry_t;

void** mutex_mem_ar;
mutex_entry_t *glob_mutex;


int ARMCI_Create_mutexes(int num)
{
int rc,p, totcount;
int *mutex_count = (int*)armci_internal_buffer;

	if (num < 0 || num > MAX_LOCKS) return(FAIL);
        if(num_mutexes) armci_die("mutexes already created",num_mutexes);

        if(armci_nproc == 1){  num_mutexes=1; return(0); }

        /* local memory allocation for mutex arrays*/
        mutex_mem_ar = (void*) malloc(armci_nproc*sizeof(void*));
        if(!mutex_mem_ar) armci_die("ARMCI_Create_mutexes: malloc failed",0);
        glob_mutex = (void*)malloc(armci_nproc*sizeof(mutex_entry_t));
        if(!glob_mutex){
           free(mutex_mem_ar);
           armci_die("ARMCI_Create_mutexes: malloc 2 failed",0);
        }

           
/*        bzero(mutex_count,armci_nproc*sizeof(int));*/
        bzero((char*)mutex_count,sizeof(int)*armci_nproc);

        /* find out how many mutexes everybody allocated */
        mutex_count[armci_me]=num;
        armci_msg_igop(mutex_count, armci_nproc, "+");
	for(p=totcount=0; p< armci_nproc; p++)totcount+=mutex_count[p];

        tickets = calloc(totcount,sizeof(int));
        if(!tickets) {
           free(glob_mutex);
           free(mutex_mem_ar);
           return(FAIL2);
        }

        /* we need memory for token and turn - 2 ints */
	rc = ARMCI_Malloc(mutex_mem_ar,2*num*sizeof(int));
        if(rc){
           free(glob_mutex);
           free(mutex_mem_ar);
           free(tickets);
           return(FAIL3);
        }

        if(num)bzero((char*)mutex_mem_ar[armci_me],2*num*sizeof(int));

        /* setup global mutex array */
	for(p=totcount=0; p< armci_nproc; p++){
           glob_mutex[p].token   = mutex_mem_ar[p];
           glob_mutex[p].turn    = glob_mutex[p].token + mutex_count[p];
           glob_mutex[p].count   = mutex_count[p];
           glob_mutex[p].tickets = tickets + totcount;
           totcount += mutex_count[p];
        }

        num_mutexes= totcount;
#ifdef LAPI
        mymutexcount = num;
#endif
        ARMCI_Barrier();

        if(DEBUG)
           fprintf(stderr,"%d created (%d,%d) mutexes\n",armci_me,num,totcount);

        return(0);
}


void armci_serv_mutex_create()
{
     int mem = armci_nproc*sizeof(waiting_list_t);
     blocked = (waiting_list_t*)malloc(mem);
     if(!blocked) armci_die("armci server:error allocating mutex memory ",0);
}


void armci_serv_mutex_close()
{
     if(blocked) free(blocked );
     blocked = (waiting_list_t*)0;
}
        

int ARMCI_Destroy_mutexes()
{
#ifdef LAPI /*fix to if cmpl handler for a pending unlock runs after destroy*/
     int proc, mutex, i,factor=0;
#endif
     if(num_mutexes==0)armci_die("armci_destroy_mutexes: not created",0);
     if(armci_nproc == 1) return(0);

     armci_msg_barrier();

#ifdef LAPI /*fix to if cmpl handler for a pending unlock runs after destroy*/
     for(proc=0;proc<armci_nproc;proc++){
          for(mutex=0;mutex<glob_mutex[proc].count;mutex++){
            _dummy_server_work_ = 0.; /* must be global to fool the compiler */
            while(!armci_mutex_free(mutex,proc)){
              for(i=0; i<  SPINMAX *factor; i++) _dummy_server_work_ += 1.;
	      factor+=1;
            }
	  } 
     }
#endif
     num_mutexes=0;

#    if defined(SERVER_LOCK)
        armci_serv_mutex_close();
#    endif

     if(glob_mutex[armci_me].count)ARMCI_Free(glob_mutex[armci_me].token);

     free(tickets);
     free(glob_mutex);
     free(mutex_mem_ar);

     return(0);
}


static int register_in_mutex_queue(int id, int proc)
{
int *mutex_entry, ticket;

    if(glob_mutex[proc].count < id)
       armci_die2("armci:invalid mutex id",id, glob_mutex[proc].count);
    mutex_entry = glob_mutex[proc].token  + id;
    ARMCI_Rmw(ARMCI_FETCH_AND_ADD, &ticket, mutex_entry, 1, proc);

    return ticket;
}


/*\ check if mutex is available by comparing turn and token
 *  can only be called by a process on the same SMP node as proc
\*/
static int armci_mutex_free(int mutex, int proc)
{
volatile int *mutex_ticket=glob_mutex[proc].turn + mutex;
volatile int *turn = glob_mutex[proc].token  +mutex;

      /* here we will put code to check if other processes on the node
       * are waiting for this mutex
       * lockinfo_node[me].ticket = mutex_ticket;
       * lockinfo_node[me].mutex  = mutex;
       */

       if(*mutex_ticket == *turn) return 1;
       else return 0;
}



static void armci_generic_lock(int mutex, int proc)
{
int i, myturn, factor=0, len=sizeof(int);
int  *mutex_ticket, next_in_line;
        
      mutex_ticket= glob_mutex[proc].turn + mutex;
      myturn = register_in_mutex_queue(mutex, proc);

      /* code to reduce cost of unlocking mutex on the same SMP node goes here
       * lockinfo_node[me].ticket = mutex_ticket;
       * lockinfo_node[me].mutex  = mutex;
       */

      _dummy_work_ = 0.; /* must be global to fool the compiler */
      do {

           ARMCI_Get(mutex_ticket, &next_in_line, len, proc);
           if(next_in_line > myturn)
              armci_die2("armci: problem with tickets",myturn,next_in_line); 
          
           /* apply a linear backoff delay before retrying  */
           for(i=0; i<  SPINMAX * factor; i++) _dummy_work_ += 1.;

           factor += 1;

      }while (myturn != next_in_line);

      glob_mutex[proc].tickets[mutex] = myturn; /* save ticket value */
}


static void armci_generic_unlock(int mutex, int proc)
{
int *mutex_ticket= glob_mutex[proc].turn + mutex;
int *newval = glob_mutex[proc].tickets +mutex;
int len=sizeof(int);

       /* update ticket for next process requesting this mutex */
       (*newval) ++; 

       /* write new ticket value stored previously in tickets  */
       ARMCI_Put(newval, mutex_ticket, len, proc);
       MEM_FENCE;
}


/*\  Acquire mutex  for "proc"
 *   -must be executed in hrecv/AM handler thread
 *   -application thread must use generic_lock routine
\*/
int armci_server_lock_mutex(int mutex, int proc, msg_tag_t tag)
{
int myturn;
int *mutex_ticket, next_in_line, len=sizeof(int);
int owner = armci_me;
        

      if(DEBUG)fprintf(stderr,"SLOCK=%d owner=%d p=%d m=%d\n",
                       armci_me,owner, proc,mutex);

      mutex_ticket= glob_mutex[owner].turn + mutex;
      myturn = register_in_mutex_queue(mutex, owner);

      armci_copy(mutex_ticket, &next_in_line, len);

      if(next_in_line > myturn)
         armci_die2("armci-s: problem with tickets",myturn,next_in_line); 

      if(next_in_line != myturn){
           if(!blocked)armci_serv_mutex_create();
           blocked[proc].mutex = mutex;
           blocked[proc].turn = myturn;
           blocked[proc].tag  = tag;
           if(DEBUG) fprintf(stderr,"SLOCK=%d proc=%d blocked (%d,%d)\n",
                                     armci_me, proc, next_in_line,myturn);
           return -1;

      } else {

           if(DEBUG) fprintf(stderr,"SLOCK=%d proc=%d sending ticket (%d)\n",
                                                       armci_me, proc, myturn);

           /* send ticket to requesting node */
           /* GA_SEND_REPLY(tag, &myturn, sizeof(int), proc); */
           return (myturn);
      } 
}
           

        
/*\  Release mutex "id" held by proc 
 *   called from hrecv/AM handler AND application thread
\*/
int armci_server_unlock_mutex(int mutex, int proc, int Ticket, msg_tag_t* ptag)
{
#define NOBODY -1
int owner = armci_me;
int i, p=NOBODY, *mutex_ticket= glob_mutex[owner].turn + mutex;
int len=sizeof(int);

     if(DEBUG) fprintf(stderr,"SUNLOCK=%d node=%d mutex=%d ticket=%d\n",
                       armci_me,proc,mutex,Ticket);

     Ticket++; 
     armci_copy(&Ticket, mutex_ticket, len);

     /* if mutex is free then nobody is reqistered in queue */
     if(armci_mutex_free(mutex, proc))  return -1;

     /* search for the next process in queue waiting for this mutex */
     for(i=0; i< armci_nproc; i++){
        if(!blocked)break; /* not allocated yet - nobody is waiting */
        if(DEBUG)fprintf(stderr,"SUNLOCK=%d node=%d list=(%d,%d)\n",
                          armci_me, i, blocked[i].mutex, blocked[i].turn);
        if((blocked[i].mutex == mutex) && (blocked[i].turn == Ticket)){
           p = i;
           break;
        }
     }

     /* send Ticket to a process waiting for mutex */
     if(p != NOBODY){
        if(p == armci_me)armci_die("server_unlock: cannot unlock self",0);
        else {

          if(DEBUG)fprintf(stderr,"SUNLOCK=%d node=%d unlock ticket=%d go=%d\n",
                                         armci_me, proc, Ticket, p);

          /*   GA_SEND_REPLY(blocked[p].tag, &Ticket, sizeof(int), p); */
          *ptag = blocked[p].tag;
          return p;

        }
     }

     return -1; /* nobody is waiting */
}



void ARMCI_Lock(int mutex, int proc)        
{
#if defined(SERVER_LOCK)
int direct;
#endif

        if(DEBUG)fprintf(stderr,"%d enter lock\n",armci_me);

        if(!num_mutexes) armci_die("armci_lock: create mutexes first",0);

        if(mutex > glob_mutex[proc].count)
           armci_die2("armci_lock: mutex not allocated", mutex,
                      glob_mutex[proc].count); 

        if(armci_nproc == 1) return;

#       if defined(SERVER_LOCK)
           direct=SAMECLUSNODE(proc); 
           if(!direct) 
              armci_rem_lock(mutex,proc, glob_mutex[proc].tickets + mutex );
           else
#       endif
              armci_generic_lock(mutex,proc);

        if(DEBUG)fprintf(stderr,"%d leave lock\n",armci_me);
}



void ARMCI_Unlock(int mutex, int proc)
{
        if(DEBUG)fprintf(stderr,"%d enter unlock\n",armci_me);

        if(!num_mutexes) armci_die("armci_lock: create mutexes first",0);

        if(mutex > glob_mutex[proc].count)
           armci_die2("armci_lock: mutex not allocated", mutex,
                      glob_mutex[proc].count); 

        if(armci_nproc == 1) return;

#       if defined(SERVER_LOCK)
           if(armci_nclus >1) { 
	     if(proc != armci_me)
               armci_rem_unlock(mutex, proc, glob_mutex[proc].tickets[mutex]);
	     else {
	       int ticket = glob_mutex[proc].tickets[mutex];
	       msg_tag_t tag;
	       int waiting;
	       
	       waiting = armci_server_unlock_mutex(mutex, proc, ticket, &tag);
	       if(waiting >-1)
		 armci_unlock_waiting_process(tag, waiting, ++ticket);
	     }
	   }
	   else
#       endif
	     armci_generic_unlock(mutex, proc);

        if(DEBUG)fprintf(stderr,"%d leave unlock\n",armci_me);
}

/* $Id: armci_profile.c,v 1.8 2005-11-30 10:20:53 vinod Exp $ */

/**
 * Set an environment variable as follows to enable ARMCI profiling
 *    export ARMCI_PROFILE=YES (bash)
 *    setenv ARMCI_PROFILE YES (csh/tcsh)
 * 
 * Profiler can profile the following ARMCI Calls:
 *    ARMCI_Get,ARMCI_Put,ARMCI_Acc,ARMCI_NbGet,ARMCI_NbPut,ARMCI_NbAcc,
 *    ARMCI_GetS,ARMCI_PutS,ARMCI_AccS,ARMCI_NbGetS,ARMCI_NbPutS,ARMCI_NbAccS,
 *    ARMCI_GetV,ARMCI_PutV,ARMCI_AccV,ARMCI_NbGetV,ARMCI_NbPutV,ARMCI_NbAccV,
 *    ARMCI_Wait, armci_wait_notify
 *      (NOTE: As armci_notify is same as ARMCI_Put, it is not profiled.)
 *   
 * 
 * Note #1: Right now, only process 0's profile is printed.
 * Each and every process saves its profile in the correspoding data struture.
 * Each process prints its profile to an output file armci_profile.<myrank> 
 * when armci_profile_terminate() is called (called in ARMCI_Finalize()).
 *
 * Note #2: By default profiler prints msg ranges 0 to 21. Example: range 10
 * corresponds to message ranges from 1024 bytes to 2047 bytes.
 * Message ranges are in the power of 2. for ex:
 * ------------------------------------
 *  MSG_RANGE (r)        BYTES (2^r to 2^(r+1)-1)
 * ------------------------------------
 *      0                    0-1 
 *      1                    2-3
 *      2                    4-7
 *     ...                   ...
 *      10                1024-2047 bytes
 *     ...                   ...
 *      20                1MB - (2MB-1)
 *      21                  >= 2MB
 * -------------------------------------
 * To increase the message range, set ARMCI_MAX_MSG_RANGE accordingly.
 *
 * Note #3: If Stride information needs to be printed, set ARMCI_PRINT_STRIDE.
 * Stride information is printed in armci_profile_terminate() for a various 
 * selective message ranges and event types.Modify it according to your needs.
 *
 * Note #4: There is no profiling support for non-blocking operations yet!!
 */


#ifdef ARMCI_PROFILE
#define DEBUG_ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "armci.h"
#include "armcip.h"
#include "armci_profile.h" 

#ifndef MPI
#  include "sndrcv.h"
#   define MP_TIMER TCGTIME_
#else
#  include "mpi.h"
#   define MP_TIMER MPI_Wtime
#endif


#define ARMCI_PRINT_STRIDE 1
#define ARMCI_MAX_MSG_RANGE 22 /* 0 to 21 */

#if ARMCI_PRINT_STRIDE

# define STRIDE_COUNT 1000
# define ARMCI_MAX_DIM 7

  typedef struct armci_stride {
    int stride_levels;
    int proc;
    int count[ARMCI_MAX_DIM];
    double time;
  }armci_stride_t;

  typedef struct giov {
    int ptr_array_len;
    int bytes;
  }giov_t;

  typedef struct armci_vector {
    int vec_len;
    int proc;
    giov_t *giov;
    double time;
  }armci_vector_t;

#endif

#define ARMCI_EVENTS 24

char *gEventName[ARMCI_EVENTS]={
  "GET", "PUT", "ACC", 
  "STRIDED GET", "STRIDED PUT", "STRIDED ACC",
  "VECTOR GET", "VECTOR PUT", "VECTOR ACC",
  "NBGET", "NBPUT", "NBACC",
  "STRIDED NBGET", "STRIDED NBPUT", "STRIDED NBACC",
  "VECTOR NBGET", "VECTOR NBPUT", "VECTOR NBACC",
  "BARRIER","ARMCI_WAIT","NOTIFY_WAIT",
  "FENCE", "ALLFENCE", "RMW"
};

typedef struct armci_profile {
  int count;          /* number of times called */
  double time;  /* total execution time for "count" calls */
#if ARMCI_PRINT_STRIDE
  armci_stride_t *stride;
  armci_vector_t *vector;
#endif
}armci_profile_t;

/* profile get/put/acc for various message ranges (i.e ARMCI_MAX_MSG_RANGE) */
static armci_profile_t ARMCI_PROF[ARMCI_EVENTS][ARMCI_MAX_MSG_RANGE];

/* Current event */
struct event_info {
  int is_set;
  int event_type;
  int range;
  double start_time;
} gCURRENT_EVNT; 

static int strided_event(int e) {
    if (e==ARMCI_PROF_GETS || e==ARMCI_PROF_PUTS || e==ARMCI_PROF_ACCS || 
	e==ARMCI_PROF_NBGETS || e==ARMCI_PROF_NBPUTS || e==ARMCI_PROF_NBACCS)
       return 1;
    return 0;
}

void armci_profile_init() {
    int i,j;
    if(armci_me==0) {printf("\nProfiling ARMCI - ON\n");fflush(stdout);}

    gCURRENT_EVNT.is_set = 0;

    for(i=0; i<ARMCI_EVENTS; i++)
       for(j=0; j<ARMCI_MAX_MSG_RANGE; j++) {
	  ARMCI_PROF[i][j].count = 0; ARMCI_PROF[i][j].time = 0.0; 
       }

#if ARMCI_PRINT_STRIDE
    for(i=0; i<ARMCI_EVENTS; i++) {
       if(strided_event(i))
	  for(j=0; j<ARMCI_MAX_MSG_RANGE; j++) {
	     ARMCI_PROF[i][j].stride = (armci_stride_t*)malloc(STRIDE_COUNT*sizeof(armci_stride_t));
	     ARMCI_PROF[i][j].vector = NULL;
	     if( ARMCI_PROF[i][j].stride == NULL)
		armci_die("armci_profile_init(): malloc failed", armci_me);
	  }
       if(i==ARMCI_PROF_GETV || i==ARMCI_PROF_PUTV || i==ARMCI_PROF_ACCV ||
	  i==ARMCI_PROF_NBGETV || i==ARMCI_PROF_NBPUTV || i==ARMCI_PROF_NBACCV)
	  for(j=0; j<ARMCI_MAX_MSG_RANGE; j++) {
	     ARMCI_PROF[i][j].vector = (armci_vector_t*)malloc(STRIDE_COUNT*sizeof(armci_vector_t));
	     ARMCI_PROF[i][j].stride = NULL;
	     if( ARMCI_PROF[i][j].vector == NULL)
		armci_die("armci_profile_init(): malloc failed", armci_me);
	  }
    }
#endif
}

#define ARMCI_EVENT_CLOSED     0
#define ARMCI_EVENT_NOTCLOSED -1
#define ARMCI_EVENT_SET        0
#define ARMCI_EVENT_NOTSET    -1

static int armci_profile_set_event(int event_type, int range) {
#ifdef DEBUG
    if(SERVER_CONTEXT)
       printf("\n%d(s):call profile set for %s isset is %d",armci_me,
                                   gEventName[event_type],gCURRENT_EVNT.is_set);
    else
       printf("\n%d:call profile set for %s isset is %d",armci_me,
                                   gEventName[event_type],gCURRENT_EVNT.is_set);
    fflush(stdout);
#endif
    if(gCURRENT_EVNT.is_set == 0) { /* set an event */
       gCURRENT_EVNT.is_set     = 1;
       gCURRENT_EVNT.event_type = event_type;
       gCURRENT_EVNT.range      = range;
       gCURRENT_EVNT.start_time = MP_TIMER();
       return ARMCI_EVENT_SET;
    }
    else gCURRENT_EVNT.is_set++; /* event overlap */
    return ARMCI_EVENT_NOTSET;
}

static int armci_profile_close_event(int event_type, int range, double *time, 
				     char *name) {
    
    int curr_event = gCURRENT_EVNT.event_type;
#ifdef DEBUG
    if(SERVER_CONTEXT)
       printf("\n%d(s):call profile close for %s isset is %d",armci_me,
                                   gEventName[event_type],gCURRENT_EVNT.is_set);
    else
       printf("\n%d:call profile close for %s isset is %d",armci_me,
                                   gEventName[event_type],gCURRENT_EVNT.is_set);
    fflush(stdout);
#endif 


    if(gCURRENT_EVNT.is_set==1) { /* Yep, there is an event set. So close it.*/
       /*Check if "profile stop" is called for corresponding "profile start"*/
       if(event_type != curr_event) {
	  printf( 
		  "%d: %s: ERROR:Profile started for %s, but stopped for %s\n",
		  armci_me,name,gEventName[curr_event],gEventName[event_type]);
          fflush(stdout);
	  armci_die("Profile_stop is called a different event", armci_me); 
       }
       
       *time = MP_TIMER() - gCURRENT_EVNT.start_time;
       ARMCI_PROF[curr_event][range].time += *time;
       gCURRENT_EVNT.is_set = 0; /* close the event */
       return ARMCI_EVENT_CLOSED;
    }
    else { /* event overlapping */
       gCURRENT_EVNT.is_set--;
       if(gCURRENT_EVNT.is_set<=0) {
	  char *msg="Profile_stop is called before profile_start";
	    printf("%d: %s: ERROR: %s. Event Name = %s\n", armci_me, 
		    name, msg, gEventName[curr_event]);
            fflush(stdout);
	    armci_die(" profile_stop is called before profile_start", armci_me);
       }
    }
    return ARMCI_EVENT_NOTCLOSED;
}

void armci_profile_start_strided(int count[], int stride_levels, int proc, 
				 int event_type) {
    int i, status, bytes=1, range;

    if(stride_levels >= ARMCI_MAX_DIM) 
       armci_die("ARMCI_PROFILE: stride_levels >= ARMCI_MAX_DIM. Increase ARMCI_MAX_DIM.", armci_me);

    /* find the message range */
    for(i=0; i<= stride_levels; i++)  bytes *= count[i];
    if(bytes<=0) range=0;
    else range = (int) (log((double)bytes)/log(2.0));
    if(range>=ARMCI_MAX_MSG_RANGE-1) range = ARMCI_MAX_MSG_RANGE-1;
 
    /* set the curent event for timer */
    status = armci_profile_set_event(event_type, range);
    
    if(status == ARMCI_EVENT_SET) { /* new event set */
       /* profile update: i.e. update event count */
       ARMCI_PROF[event_type][range].count++;
       
#      if ARMCI_PRINT_STRIDE 
          if(strided_event(event_type)) {
	     int idx = ARMCI_PROF[event_type][range].count-1;
	     if(idx<STRIDE_COUNT) {
		ARMCI_PROF[event_type][range].stride[idx].stride_levels = stride_levels;
		ARMCI_PROF[event_type][range].stride[idx].proc = proc;
		for(i=0;i<=stride_levels;i++) {
		   ARMCI_PROF[event_type][range].stride[idx].count[i]=count[i];
		}
	     }
	  }
#      endif
    }
    else { /* Do nothing. It is just an event overlap */ }
}

void armci_profile_stop_strided(int event_type) {
    double time;
    int status, range = gCURRENT_EVNT.range;

    status = armci_profile_close_event(event_type, range, &time,
				       "armci_profile_stop_strided");
    
#if ARMCI_PRINT_STRIDE
    if(status == ARMCI_EVENT_CLOSED) {
       /* record the time of each strided data transfer */
       if(strided_event(event_type)) {
	  int idx = ARMCI_PROF[event_type][range].count-1;
	  if(idx<STRIDE_COUNT) 
	     ARMCI_PROF[event_type][range].stride[idx].time = time;
       }
    }
#endif
}

void armci_profile_start_vector(armci_giov_t darr[], int len, int proc, 
				int event_type) {

    int i, bytes=0, range, status;

    /* find the message range */
    for(i=0; i<len; i++) bytes += darr[i].bytes;
    if(bytes<=0) range=0;
    else range = (int) (log((double)bytes)/log(2.0));
    if(range>=ARMCI_MAX_MSG_RANGE-1) range = ARMCI_MAX_MSG_RANGE-1;
       
    /* set the curent event for timer */
    status = armci_profile_set_event(event_type, range);

    if(status == ARMCI_EVENT_SET) { /* new event set */
       /* profile update: i.e. update event count */
       ARMCI_PROF[event_type][range].count++;
       
#      if ARMCI_PRINT_STRIDE 
       {
	  int idx = ARMCI_PROF[event_type][range].count-1;
	  if(idx<STRIDE_COUNT) {
	     ARMCI_PROF[event_type][range].vector[idx].vec_len = len;
	     ARMCI_PROF[event_type][range].vector[idx].proc = proc;
	     ARMCI_PROF[event_type][range].vector[idx].giov = 
	       (giov_t*)malloc(len*sizeof(giov_t));
	     for(i=0;i<len;i++) {
		ARMCI_PROF[event_type][range].vector[idx].giov[i].ptr_array_len = darr[i].ptr_array_len;
		ARMCI_PROF[event_type][range].vector[idx].giov[i].bytes = 
		  darr[i].bytes;
	     }
	  }
       }
#      endif
    }
}

void armci_profile_stop_vector(int event_type) {
    double time;
    int status, range = gCURRENT_EVNT.range;
    
    status = armci_profile_close_event(event_type, range, &time, 
				       "armci_profile_stop_vector"); 

#if ARMCI_PRINT_STRIDE
    if(status == ARMCI_EVENT_CLOSED) {/*record time of each data transfer*/
       int idx = ARMCI_PROF[event_type][range].count-1;
       if(idx<STRIDE_COUNT)  
	  ARMCI_PROF[event_type][range].vector[idx].time = time;
    }
#endif
}

void armci_profile_start(int event_type) {
    int range, status;

    /* message range is zero for events registered using this call */
    range=0;
    
    /* set the curent event for timer */
    status = armci_profile_set_event(event_type, range);
    if(status == ARMCI_EVENT_SET) { /* new event set */  
       /* profile update: i.e. update event count */
       ARMCI_PROF[event_type][range].count++;
    }
}

void armci_profile_stop(int event_type) {
    double time;
    int status,range = gCURRENT_EVNT.range;
    status = armci_profile_close_event(event_type, range, &time, 
				       "armci_profile_stop");
}

#define ARMCI_HDR0(fp) fprintf(fp, "\n\n************** TOTAL DATA TRANSFERS **************\n\n");
#define ARMCI_HDR1(fp) fprintf(fp, "\n\n************ CONTIGUOUS DATA TRANSFER ************\n\n");
#define ARMCI_HDR2(fp) fprintf(fp, "\n\n********** NON-CONTIGUOUS DATA TRANSFER **********\n\n"); 
#define ARMCI_HDR3(fp) fprintf(fp, "#gets\t #puts\t #accs\t get_time   put_time   acc_time   RANGE(bytes)\n\n");
#define ARMCI_HDR4(fp) fprintf(fp, "SL#\tndim\t proc\t time      stride_info\n\n");
#define ARMCI_HDR5(fp) fprintf(fp, "SL#\tnvec\t proc\t time\t    [ #arrays\t bytes\t]\n");
#define ARMCI_HDR6(fp) fprintf(fp, "\n\n****** NON-BLOCKING CONTIGUOUS DATA TRANSFER *****\n\n");
#define ARMCI_HDR7(fp) fprintf(fp, "\n\n*** NON-BLOCKING NON-CONTIGUOUS DATA TRANSFER ****\n\n"); 
#define ARMCI_HDR8(fp) fprintf(fp, "#gets\t #puts\t #accs\t get_time   put_time   acc_time   RANGE(bytes)\n\n");
#define ARMCI_HDR9(fp) fprintf(fp, "\n\n******************* ARMCI MISC *******************\n\n");

/* print profile of all get/put/acc calls for every message range */
static void armci_print_all(FILE *fp) {
    int i, nget, nput, nacc, nrange=ARMCI_MAX_MSG_RANGE;
    double gtime, ptime, atime;
 
    ARMCI_HDR0(fp); ARMCI_HDR3(fp);
    for(i=0; i< nrange; i++) {

       nget =(ARMCI_PROF[ARMCI_PROF_GET][i].count + 
	      ARMCI_PROF[ARMCI_PROF_GETS][i].count + 
	      ARMCI_PROF[ARMCI_PROF_GETV][i].count +
	      ARMCI_PROF[ARMCI_PROF_NBGET][i].count + 
	      ARMCI_PROF[ARMCI_PROF_NBGETS][i].count + 
	      ARMCI_PROF[ARMCI_PROF_NBGETV][i].count);
       nput =(ARMCI_PROF[ARMCI_PROF_PUT][i].count + 
	      ARMCI_PROF[ARMCI_PROF_PUTS][i].count + 
	      ARMCI_PROF[ARMCI_PROF_PUTV][i].count +
	      ARMCI_PROF[ARMCI_PROF_NBPUT][i].count +
              ARMCI_PROF[ARMCI_PROF_NBPUTS][i].count + 
	      ARMCI_PROF[ARMCI_PROF_NBPUTV][i].count);
       nacc =(ARMCI_PROF[ARMCI_PROF_ACC][i].count + 
	      ARMCI_PROF[ARMCI_PROF_ACCS][i].count + 
	      ARMCI_PROF[ARMCI_PROF_ACCV][i].count +
	      ARMCI_PROF[ARMCI_PROF_NBACC][i].count +
              ARMCI_PROF[ARMCI_PROF_NBACCS][i].count + 
	      ARMCI_PROF[ARMCI_PROF_NBACCV][i].count);

       gtime = (ARMCI_PROF[ARMCI_PROF_GET][i].time + 
		ARMCI_PROF[ARMCI_PROF_GETS][i].time + 
		ARMCI_PROF[ARMCI_PROF_GETV][i].time +
		ARMCI_PROF[ARMCI_PROF_NBGET][i].time +
		ARMCI_PROF[ARMCI_PROF_NBGETS][i].time + 
		ARMCI_PROF[ARMCI_PROF_NBGETV][i].time);
       ptime = (ARMCI_PROF[ARMCI_PROF_PUT][i].time + 
		ARMCI_PROF[ARMCI_PROF_PUTS][i].time + 
		ARMCI_PROF[ARMCI_PROF_PUTV][i].time +
		ARMCI_PROF[ARMCI_PROF_NBPUT][i].time +
		ARMCI_PROF[ARMCI_PROF_NBPUTS][i].time + 
		ARMCI_PROF[ARMCI_PROF_NBPUTV][i].time);
       atime = (ARMCI_PROF[ARMCI_PROF_ACC][i].time + 
		ARMCI_PROF[ARMCI_PROF_ACCS][i].time + 
		ARMCI_PROF[ARMCI_PROF_ACCV][i].time +
		ARMCI_PROF[ARMCI_PROF_NBACC][i].time +
                ARMCI_PROF[ARMCI_PROF_NBACCS][i].time + 
		ARMCI_PROF[ARMCI_PROF_NBACCV][i].time);
       
       fprintf(fp, "%d\t %d\t %d\t %.2e   %.2e   %.2e  ",
               nget, nput, nacc,  gtime, ptime, atime);
       if (i< nrange-1) fprintf(fp, "(%d-%d)\n", 1<<i, (1<<(i+1))-1);
       else fprintf(fp, "(>=%d)\n", 1<<(ARMCI_MAX_MSG_RANGE-1));
    }
}

/* print profile of contiguous get/put/acc calls for every message range */
static void armci_print_contig(FILE *fp) {
    int i, nrange=ARMCI_MAX_MSG_RANGE; 
    ARMCI_HDR1(fp); ARMCI_HDR3(fp);
    for(i=0; i< nrange; i++) {
       fprintf(fp, "%d\t %d\t %d\t %.2e   %.2e   %.2e  ",
	       ARMCI_PROF[ARMCI_PROF_GET][i].count,
	       ARMCI_PROF[ARMCI_PROF_PUT][i].count,
	       ARMCI_PROF[ARMCI_PROF_ACC][i].count, 
	       ARMCI_PROF[ARMCI_PROF_GET][i].time,
	       ARMCI_PROF[ARMCI_PROF_PUT][i].time,
	       ARMCI_PROF[ARMCI_PROF_ACC][i].time);
       if(i< nrange-1) fprintf(fp, "(%d-%d)\n", 1<<i, (1<<(i+1))-1);
       else fprintf(fp, "(>=%d)\n", 1<<(ARMCI_MAX_MSG_RANGE-1));
    }
}

/* This prints the number of non-contiguous get/put/acc/ calls for every 
   message range */
static void armci_print_noncontig(FILE *fp) {
    int i, nget, nput, nacc, nrange=ARMCI_MAX_MSG_RANGE;
    double gtime, ptime, atime;

    ARMCI_HDR2(fp); ARMCI_HDR3(fp);
    for(i=0; i< nrange; i++) {
       nget = (ARMCI_PROF[ARMCI_PROF_GETS][i].count + 
	       ARMCI_PROF[ARMCI_PROF_GETV][i].count);
       nput = (ARMCI_PROF[ARMCI_PROF_PUTS][i].count + 
	       ARMCI_PROF[ARMCI_PROF_PUTV][i].count);
       nacc = (ARMCI_PROF[ARMCI_PROF_ACCS][i].count + 
	       ARMCI_PROF[ARMCI_PROF_ACCV][i].count);
       gtime = (ARMCI_PROF[ARMCI_PROF_GETS][i].time + 
		ARMCI_PROF[ARMCI_PROF_GETV][i].time);
       ptime = (ARMCI_PROF[ARMCI_PROF_PUTS][i].time + 
		ARMCI_PROF[ARMCI_PROF_PUTV][i].time);
       atime = (ARMCI_PROF[ARMCI_PROF_ACCS][i].time + 
		ARMCI_PROF[ARMCI_PROF_ACCV][i].time);
       
       fprintf(fp, "%d\t %d\t %d\t %.2e   %.2e   %.2e  ",
	       nget, nput, nacc,  gtime, ptime, atime);
       if (i< nrange-1) fprintf(fp, "(%d-%d)\n", 1<<i, (1<<(i+1))-1);
       else fprintf(fp, "(>=%d)\n", 1<<(ARMCI_MAX_MSG_RANGE-1));
    }
}

/* print profile of non-blocking contiguous get/put/acc calls for every 
   message range */
static void armci_print_nbcontig(FILE *fp) {
    int i, nrange=ARMCI_MAX_MSG_RANGE; 
    ARMCI_HDR6(fp); ARMCI_HDR8(fp);
    for(i=0; i< nrange; i++) {
       fprintf(fp, "%d\t %d\t %d\t %.2e   %.2e   %.2e  ",
	       ARMCI_PROF[ARMCI_PROF_NBGET][i].count,
	       ARMCI_PROF[ARMCI_PROF_NBPUT][i].count,
	       ARMCI_PROF[ARMCI_PROF_NBACC][i].count, 
	       ARMCI_PROF[ARMCI_PROF_NBGET][i].time,
	       ARMCI_PROF[ARMCI_PROF_NBPUT][i].time,
	       ARMCI_PROF[ARMCI_PROF_NBACC][i].time);
       if(i< nrange-1) fprintf(fp, "(%d-%d)\n", 1<<i, (1<<(i+1))-1);
       else fprintf(fp, "(>=%d)\n", 1<<(ARMCI_MAX_MSG_RANGE-1));
    }
}

/* This prints the number of non-blocking non-contiguous get/put/acc/ calls 
   for every message range */
static void armci_print_nbnoncontig(FILE *fp) {
    int i, nget, nput, nacc, nrange=ARMCI_MAX_MSG_RANGE;
    double gtime, ptime, atime;

    ARMCI_HDR7(fp); ARMCI_HDR8(fp);
    for(i=0; i< nrange; i++) {
       nget = (ARMCI_PROF[ARMCI_PROF_NBGETS][i].count + 
	       ARMCI_PROF[ARMCI_PROF_NBGETV][i].count);
       nput = (ARMCI_PROF[ARMCI_PROF_NBPUTS][i].count + 
	       ARMCI_PROF[ARMCI_PROF_NBPUTV][i].count);
       nacc = (ARMCI_PROF[ARMCI_PROF_NBACCS][i].count + 
	       ARMCI_PROF[ARMCI_PROF_NBACCV][i].count);
       gtime = (ARMCI_PROF[ARMCI_PROF_NBGETS][i].time + 
		ARMCI_PROF[ARMCI_PROF_NBGETV][i].time);
       ptime = (ARMCI_PROF[ARMCI_PROF_NBPUTS][i].time + 
		ARMCI_PROF[ARMCI_PROF_NBPUTV][i].time);
       atime = (ARMCI_PROF[ARMCI_PROF_NBACCS][i].time + 
		ARMCI_PROF[ARMCI_PROF_NBACCV][i].time);

       fprintf(fp, "%d\t %d\t %d\t %.2e   %.2e   %.2e  ",
	       nget, nput, nacc,  gtime, ptime, atime);
       if (i< nrange-1) fprintf(fp, "(%d-%d)\n", 1<<i, (1<<(i+1))-1);
       else fprintf(fp, "(>=%d)\n", 1<<(ARMCI_MAX_MSG_RANGE-1));
    }
}

/* Profile of armci_notify_wait(), ARMCI_Wait() and ARMCI_Barrier() */
static void armci_print_misc(FILE *fp) {
    ARMCI_HDR9(fp);
    fprintf(fp, "#calls\t time\t   EVENT\n\n");
    fprintf(fp, "%d\t %.2e  ARMCI_Wait()\n", 
	    ARMCI_PROF[ARMCI_PROF_WAIT][0].count, 
	    ARMCI_PROF[ARMCI_PROF_WAIT][0].time);
    fprintf(fp, "%d\t %.2e  armci_notify_wait()\n", 
	    ARMCI_PROF[ARMCI_PROF_NOTIFY][0].count, 
	    ARMCI_PROF[ARMCI_PROF_NOTIFY][0].time);
    fprintf(fp, "%d\t %.2e  ARMCI_Barrier()\n", 
	    ARMCI_PROF[ARMCI_PROF_BARRIER][0].count, 
	    ARMCI_PROF[ARMCI_PROF_BARRIER][0].time);
    fprintf(fp, "%d\t %.2e  ARMCI_Fence()\n", 
	    ARMCI_PROF[ARMCI_PROF_FENCE][0].count, 
	    ARMCI_PROF[ARMCI_PROF_FENCE][0].time);
    fprintf(fp, "%d\t %.2e  ARMCI_Allfence()\n", 
	    ARMCI_PROF[ARMCI_PROF_ALLFENCE][0].count, 
	    ARMCI_PROF[ARMCI_PROF_ALLFENCE][0].time);
    fprintf(fp, "%d\t %.2e  ARMCI_Rmw()\n", 
	    ARMCI_PROF[ARMCI_PROF_RMW][0].count, 
	    ARMCI_PROF[ARMCI_PROF_RMW][0].time);
}

#if ARMCI_PRINT_STRIDE 
static void armci_print_warning_msg(FILE *fp, int range, int str_count) {
    fprintf(fp, "WARNING: In your program, total number of data transfers\n");
    fprintf(fp, "for message range[%d - %d] is %d. This exceeds\n", 
	    1<<range, 1<<(range+1), str_count);
    fprintf(fp,"the maximum # of data transfers [%d] that can be profiled.\n",
	    STRIDE_COUNT); 
    fprintf(fp, "Therefore profile of only first %d data \n", STRIDE_COUNT);
    fprintf(fp, "transfers are shown below. To increase the count, set\n");
    fprintf(fp, "STRIDE_COUNT > %d (in armci_profile.c)\n", str_count);
}

static void armci_print_stridedinfo(FILE *fp, int event, int range) {
    int i, j, stride_levels, str_count;
    double time=0.0;
    
    str_count = ARMCI_PROF[event][range].count;
    if(str_count <=0) return;    
    if(str_count > STRIDE_COUNT) { 
       armci_print_warning_msg(fp, range, str_count);
       str_count = STRIDE_COUNT;
    }

    fprintf(fp, "\n\nSTRIDE INFORMATION FOR MSG_RANGE %d-%d for EVENT: %s\n", 
	    1<<range, (1<<(range+1))-1, gEventName[event]);
    ARMCI_HDR4(fp);

    for(i=0; i< str_count; i++) {
       time += ARMCI_PROF[event][range].stride[i].time;
       stride_levels  = ARMCI_PROF[event][range].stride[i].stride_levels;
       fprintf(fp, "%d\t%d\t %d\t %.2e  (",i, stride_levels,
	       ARMCI_PROF[event][range].stride[i].proc,
	       ARMCI_PROF[event][range].stride[i].time);
       for(j=0;j<=stride_levels;j++) {
	  fprintf(fp, "%d", ARMCI_PROF[event][range].stride[i].count[j]);
	  if(j!=stride_levels) fprintf(fp, "x");
       }
       fprintf(fp, ")\n");
    }
    /*This o/p is just for verification*/
    fprintf(fp, "**** STRIDE_COUNT = %d ; TOTAL TIME = %.2e\n",
	    str_count, time);
}

static void armci_print_vectorinfo(FILE *fp, int event, int range) {
    int i, j, vec_len, str_count;
    double time=0.0;
    
    str_count = ARMCI_PROF[event][range].count;
    if(str_count <=0) return; 
    if(str_count > STRIDE_COUNT) { 
       armci_print_warning_msg(fp, range, str_count);
       str_count = STRIDE_COUNT;
    }
    
    fprintf(fp, "\n\nVECTOR INFORMATION FOR MSG_RANGE %d-%d for EVENT: %s\n", 
	    1<<range, (1<<(range+1))-1, gEventName[event]);
    ARMCI_HDR5(fp);

    for(i=0; i< str_count; i++) {
       time += ARMCI_PROF[event][range].vector[i].time;
       vec_len  = ARMCI_PROF[event][range].vector[i].vec_len;
       fprintf(fp, "%d\t%d\t %d\t %.2e   [  ",i, vec_len,
	       ARMCI_PROF[event][range].vector[i].proc,
	       ARMCI_PROF[event][range].vector[i].time);
       for(j=0;j<vec_len;j++) {
	  fprintf(fp, "%-9d %d\t]\n", 
		  ARMCI_PROF[event][range].vector[i].giov[j].ptr_array_len,
		  ARMCI_PROF[event][range].vector[i].giov[j].bytes);
	  if(j!=vec_len-1) fprintf(fp, "\t\t\t\t    [  ");
       }
    }
    /*This o/p is just for verification*/
    fprintf(fp, "**** STRIDE_COUNT = %d ; TOTAL TIME = %.2e\n",
	    str_count, time);
}
#endif /* end of ARMCI_PRINT_STRIDE */

void armci_profile_terminate() {
    FILE *fp = stdout;
    char file_name[50];
    sprintf(file_name, "armci_profile.%d", armci_me);
    fp = fopen(file_name, "w");

    armci_print_all(fp);         /* total get/put/acc calls */
    armci_print_contig(fp);      /* contiguous calls */
    armci_print_noncontig(fp);   /* non-contiguous calls */
    armci_print_nbcontig(fp);    /* non-blocking contiguous calls */
    armci_print_nbnoncontig(fp); /* non-blocking non-contiguous calls */
    
    /* miscellaneous (barrier, armci_wait, notify_wait) */
    armci_print_misc(fp);

#if ARMCI_PRINT_STRIDE
    {
       /**
	* printing stride info for non-contiguous get (ARMCI_PROF_GETS) for message
	* range #6. 2^6 - 2^(6+1) bytes (i.e. 64-128 bytes)
	*    Ex: armci_print_stridedinfo(ARMCI_PROF_GETS,6);
 	*/
#define ARMCI_PRINT_EVENTS 6
       int i,j;
       int str_event[ARMCI_PRINT_EVENTS]={ARMCI_PROF_GETS, ARMCI_PROF_PUTS,
					  ARMCI_PROF_ACCS, ARMCI_PROF_NBGETS,
					  ARMCI_PROF_NBPUTS,ARMCI_PROF_NBACCS};
       int vec_event[ARMCI_PRINT_EVENTS]={ARMCI_PROF_GETV, ARMCI_PROF_PUTV,
					  ARMCI_PROF_ACCV, ARMCI_PROF_NBGETV,
					  ARMCI_PROF_NBPUTV,ARMCI_PROF_NBACCV};
       
       fprintf(fp,"\n\n***************************************************\n");
       fprintf(fp,    " STRIDE INFORMATION for all strided data transfers\n");
       fprintf(fp,    "***************************************************\n");
       for(i=0; i<ARMCI_MAX_MSG_RANGE; i++)
	  for(j=0; j<ARMCI_PRINT_EVENTS; j++)
	     armci_print_stridedinfo(fp,str_event[j], i);
       
       fprintf(fp,"\n\n**************************************************\n");
       fprintf(fp,    " VECTOR INFORMATION for all vector data transfers\n");
       fprintf(fp,    "**************************************************\n");
       for(i=0; i<ARMCI_MAX_MSG_RANGE; i++)
	  for(j=0; j<ARMCI_PRINT_EVENTS; j++)
	     armci_print_vectorinfo(fp,vec_event[j], i);
    }
#endif
    fclose(fp);
}

#endif /* end of ARMCI_PROFILE */


/* Copyright (c) 1995 by PDCL Corporation.  All Rights Reserved */

/***
   NAME
     timing.c
   PURPOSE
     Timing routines for calculating the execution time:
       void start_timer(void);  Set the timer.
       double elapsed_time(void);  Return the timing elapsed since
                                   the timer has been set.
   NOTES
     Jialin Ju - Oct 16, 1995 Created.
***/

#include <sys/types.h>
#include <sys/errno.h>
#include <sys/time.h>

/* Timing routines that use standard Unix gettingofday() */
static struct timezone tz;
static struct timeval start_time, finish_time;

/* Start measuring a time delay */
void start_timer(void)
{
    gettimeofday( &start_time, &tz);
}

/* Retunrn elapsed time in milliseconds */
double elapsed_time(void)
{
    gettimeofday( &finish_time, &tz);
    return(1000.0*(finish_time.tv_sec - start_time.tv_sec) +
           (finish_time.tv_usec - start_time.tv_usec)/1000.0 );
}

/* Return the stopping time in milliseconds */
double stop_time(void)
{
    gettimeofday( &finish_time, &tz);
    return(1000.0*finish_time.tv_sec + finish_time.tv_usec/1000.0);
}


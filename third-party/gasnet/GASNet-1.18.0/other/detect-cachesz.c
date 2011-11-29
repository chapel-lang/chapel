/*   $Source: /var/local/cvs/gasnet/other/detect-cachesz.c,v $
 *     $Date: 2009/03/30 02:40:40 $
 * $Revision: 1.4 $
 * Description: stand-alone tool to empirically detect SMP shared data cache line size
 * Copyright 2005, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_TRIALS 10
#define MAX_SIZE  1024
#define MAX_ALIGN (65*1024)

char buf[2*MAX_ALIGN+MAX_SIZE+1];
volatile int done = 1;
typedef unsigned long long ustime_t;
static ustime_t timeStamp(void) {
    struct timeval tv;
    if (gettimeofday(&tv, NULL)) { perror("gettimeofday"); abort(); }
    return ((ustime_t)tv.tv_sec) * 1000000 + tv.tv_usec;
}

void * thread_spin(void *arg) {
  volatile unsigned char *p = arg;
  done = 0;
  while (!done) { 
    (*p)++;
    if (*p % 0xFFFF == 0) pthread_testcancel(); /* fix hangs on SunCC optimizer */
  }
  return NULL;
}
int vote[32];
int detect_cachesz(double confidence_threshold, int conservative_default) {
  pthread_attr_t attr;   
  pthread_t threadid;
  int trial, size, iters, i, winner, max=0;
  char *base = (char *)((((unsigned long long)&buf[0])+(MAX_ALIGN-1))&~((unsigned long long)MAX_ALIGN-1));
  #ifdef HAVE_PTHREAD_SETCONCURRENCY
    pthread_setconcurrency(2);
  #endif
  pthread_attr_init(&attr);   
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); 
  if (pthread_create(&threadid, &attr, &thread_spin, (void *)base)) 
    { perror("pthread_create"); abort(); }
  for (trial=0; trial < NUM_TRIALS; trial++) {
    int pos = 2;
    double maxdrop=0, res[64]; 
    for (size=4; size <= MAX_SIZE; size*=2) {
      volatile unsigned char *p = (volatile unsigned char *)(base+size);
      for (iters = 100; iters < 1LLU<<31; iters*=10) {
        ustime_t end, start = timeStamp();
        for (i = 0; i < iters; i++) (*p)++;
        end = timeStamp();
        if (end - start >= 10000) {
          res[pos] = (end - start)/(double)iters;
          break;
        }
      }
      pos++;
    }
    winner = 0;
    #ifndef CONFIG_TEST
      printf("  4:\t%f\n", res[2]);
    #endif
    for (i=3; i < pos; i++) {
      double drop = res[i-1]-res[i];
      #ifndef CONFIG_TEST
        printf("  %i:\t%f\n", (int)(1<<i), res[i]);
      #endif
      if (drop > maxdrop) { maxdrop = drop; winner = i; }
    }
      if (winner) { 
        #ifndef CONFIG_TEST
          printf("%i\n", (1<<winner)); 
        #endif
        vote[winner]++; 
      }
  }
  done = 1;
  if (pthread_join(threadid, NULL))
    { perror("pthread_join"); abort(); }
  winner = 0;
  for (i=0; i < 32; i++) {
    if (vote[i] > max) { max = vote[i]; winner = i; }
  }
  { double confidence = (100.0*max)/NUM_TRIALS;
    #ifdef CONFIG_TEST
      if (confidence >= confidence_threshold) return (1<<winner);
      else return conservative_default;
    #else
      printf("RESULT= %i bytes (%3.0f%% confidence)\n", (1<<winner), confidence);
      return (1<<winner);
    #endif
  }
}

#ifndef CONFIG_TEST
int main(void) {
  detect_cachesz(0.0, 0);
  return 0;
}
#endif

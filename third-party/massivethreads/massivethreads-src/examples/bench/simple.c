/* 
 * simple.c 
 * Simple multi-threaded program for debugging 
 */

#include <stdlib.h>
#include <stdio.h>
//#include <pthread.h>
#include "../../myth.h"

void * func1(void *arg)
{
  printf("Hello from func1\n");
}

void * func2(void *arg)
{
  printf("Hello from func2\n");
}

int main(int argc, char *argv)
{
	/* MYTH_NATIVE test 
  myth_thread_t th1;
  printf("init\n");
  myth_init();
  printf("create\n");
  th1 = myth_create(func1, 0x0);
  printf("join\n");
  myth_join(th1, NULL);
  printf("fini\n");
	myth_fini();
  printf("exit\n");
   */
  pthread_t th1, th2;
  pthread_create(&th1, NULL, func1, NULL);
  pthread_create(&th2, NULL, func2, NULL);
  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  return 0;
}

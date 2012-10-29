/* 
 * simple.c 
 * Simple multi-threaded program for debugging 
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

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
  printf("start\n");
	/* MYTH_NATIVE test 
  myth_init();
  printf("run\n");
	myth_fini();
   */
  pthread_t th1, th2;
  pthread_create(&th1, NULL, func1, NULL);
  pthread_create(&th2, NULL, func2, NULL);
  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  printf("start\n");
  return 0;
}

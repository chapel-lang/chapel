/*
 * mm.c
 * Memory management/migration 
 *
 * by Nan Dun <dun@logos.ic.i.u-tokyo.ac.jp>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>

#define ARR_SIZE  1
#define BLK_SIZE  1048576

#define MEM_TRACE

#ifdef MEM_TRACE
#include <mcheck.h>
#endif

struct data {
  char **arr;
  int mem_allocated;
  double t_alloc, t_free, t_realloc, t_refree, t_total;
};

inline double curr_time(void)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1e6 + (double) tv.tv_usec;
}

void * malloc_local(void *args)
{
  struct data *p = (struct data *) args;
  unsigned long tid;
  int i;
  
  tid = pthread_self();
  
  // malloc
  printf("  Thread %lu: malloc\n", tid);
  p->t_alloc = curr_time();
  p->arr = malloc(sizeof(char *) * ARR_SIZE);
  if (p->arr == NULL) {
    perror(NULL);
    abort();
  }
  for (i = 0; i < ARR_SIZE; i++) {
    p->arr[i] = (char *) malloc(sizeof(char) * BLK_SIZE);
    if (p->arr[i] == NULL) {
      perror(NULL);
      abort();
    }
  }
  p->t_alloc = curr_time() - p->t_alloc;

  // free
  printf("  Thread %lu: free\n", tid);
  p->t_free = curr_time();
  for (i = 0; i < ARR_SIZE; i++) 
    free(p->arr[i]);
  free(p->arr);
  p->t_free = curr_time() - p->t_free;

  // re-malloc
  printf("  Thread %lu: re-malloc\n", tid);
  p->t_realloc = curr_time();
  p->arr = malloc(sizeof(char *) * ARR_SIZE);
  if (p->arr == NULL) {
    perror(NULL);
    abort();
  }
  for (i = 0; i < ARR_SIZE; i++) {
    p->arr[i] = (char *) malloc(sizeof(char) * BLK_SIZE);
    if (p->arr[i] == NULL) {
      perror(NULL);
      abort();
    }
  }
  p->t_realloc = curr_time() - p->t_realloc;
  
  // re-free
  printf("  Thread %lu: re-free\n", tid);
  p->t_refree = curr_time();
  for (i = 0; i < ARR_SIZE; i++)
    free(p->arr[i]);
  free(p->arr);
  p->t_refree = curr_time() - p->t_refree;

  p->t_total = p->t_alloc + p->t_free + p->t_realloc + p->t_refree;

  printf("  Thread %lu: exit\n", tid);
  pthread_exit((void *) 0);
}

pthread_mutex_t mutex;
pthread_cond_t cond;

void * malloc_remote_from(void *args)
{
  struct data *p = (void *) args;
  unsigned long tid;
  int i;
 
  tid = pthread_self();
  
  pthread_mutex_lock(&mutex);

  // malloc
  printf("  Thread %lu: malloc\n", tid);
  p->t_alloc = curr_time();
  p->arr = (char **) malloc(sizeof(char *) * ARR_SIZE);
  if (p->arr == NULL) {
    perror(NULL);
    abort();
  }
  for (i = 0; i < ARR_SIZE; i++) {
    (p->arr)[i] = (char *) malloc(sizeof(char) * BLK_SIZE);
    if ((p->arr)[i] == NULL) {
      perror(NULL);
      abort();
    }
  }
  p->t_alloc = curr_time() - p->t_alloc;

  p->mem_allocated = 1;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);

  pthread_mutex_lock(&mutex);
  while (p->mem_allocated == 1)
    pthread_cond_wait(&cond, &mutex);
  pthread_mutex_unlock(&mutex);
  
  p->t_total = p->t_alloc + p->t_free + p->t_realloc + p->t_refree;

  printf("  Thread %lu: exit\n", tid);
  pthread_exit((void *) 0);
}

void * malloc_remote_to(void *args)
{
  struct data *p = (void *) args;
  unsigned long tid;
  int i;
  
  tid = pthread_self();
  pthread_mutex_lock(&mutex);
  while (p->mem_allocated == 0)
    pthread_cond_wait(&cond, &mutex);
  pthread_mutex_unlock(&mutex);

  // free
  printf("  Thread %lu: free\n", tid);
  p->t_free = curr_time();
  for (i = 0; i < ARR_SIZE; i++) 
    free(p->arr[i]);
  free(p->arr);
  p->t_free = curr_time() - p->t_free;
  
  // re-malloc
  printf("  Thread %lu: re-malloc\n", tid);
  p->t_realloc = curr_time();
  p->arr = malloc(sizeof(char *) * ARR_SIZE);
  if (p->arr == NULL) {
    perror(NULL);
    abort();
  }
  for (i = 0; i < ARR_SIZE; i++) {
    p->arr[i] = (char *) malloc(sizeof(char) * BLK_SIZE);
    if (p->arr[i] == NULL) {
      perror(NULL);
      abort();
    }
  }
  p->t_realloc = curr_time() - p->t_realloc;

  // re-free
  printf("  Thread %lu: re-free\n", tid);
  p->t_refree = curr_time();
  for (i = 0; i < ARR_SIZE; i++)
    free(p->arr[i]);
  free(p->arr);
  p->t_refree = curr_time() - p->t_refree;

#ifdef MEM_TRACE
  muntrace();
#endif
  
  pthread_mutex_lock(&mutex);
  p->mem_allocated = 0;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
  
  printf("  Thread %lu: exit\n", tid);
  pthread_exit((void *) 0);
}

int main(int argc, char **argv)
{
  pthread_t tha, thb;
  struct data dat;
  int nthreads;
  int ret;

  nthreads = argc == 1 ? 1 : atoi(argv[1]);
  
  printf("Memory management/migration test (usec)\n");

#ifdef MEM_TRACE
  mtrace();
#endif
  
  if (nthreads == 1) {
      printf("One thread...\n");
      pthread_create(&tha, NULL, malloc_local, (void *) &dat);
      pthread_join(tha, (void **) &ret);
      
      printf("     malloc: %.2f\n"
             "       free: %.2f\n"
             "  re-malloc: %.2f\n"
             "    re-free: %.2f\n"
             "      Total: %.2f\n", dat.t_alloc, dat.t_free, 
             dat.t_realloc, dat.t_refree, dat.t_total);
  }

  if (nthreads >= 2) {
      printf("Two threads...\n");
      dat.mem_allocated = 0;
      pthread_mutex_init(&mutex, NULL);
      pthread_cond_init(&cond, NULL);
      pthread_create(&tha, NULL, malloc_remote_from, (void *) &dat);
      pthread_create(&thb, NULL, malloc_remote_to, (void *) &dat);
      pthread_join(tha, (void **) &ret);
      pthread_join(thb, (void **) &ret);
      pthread_mutex_destroy(&mutex);
      pthread_cond_destroy(&cond);
      
      printf("     malloc: %.2f\n"
             "       free: %.2f\n"
             "  re-malloc: %.2f\n"
             "    re-free: %.2f\n"
             "      Total: %.2f\n", dat.t_alloc, dat.t_free, 
             dat.t_realloc, dat.t_refree, dat.t_total);
  }

  return 0;
}

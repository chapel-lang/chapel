#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <myth/myth.h>

#define nthreads 100

myth_key_t key;
int vals[nthreads];

void destroy_key(void * arg) {
  long i = (long)arg;
  vals[i] = (i + 1) * (i + 1);
}

void * f(void * arg) {
  int r = myth_setspecific(key, arg);
  assert(r == 0);
  return 0;
}

int create_and_delete_keys() {
  long i;
  myth_thread_t th[nthreads];
  int r = myth_key_create(&key, destroy_key);
  assert(r == 0);

  for (i = 0; i < nthreads; i++) {
    vals[i] = 0;
    th[i] = myth_create(f, (void *)i);
  }
  for (i = 0; i < nthreads; i++) {
    myth_join(th[i], 0);
  }
  for (i = 0; i < nthreads; i++) {
    assert(vals[i] == (i + 1) * (i + 1));
  }
  r = myth_key_delete(key);
  assert(r == 0);
  return 0;
}

int main() {
  int i;
  for (i = 0; i < 10; i++) {
    create_and_delete_keys();
  }
  printf("OK\n");
  return 0;
}

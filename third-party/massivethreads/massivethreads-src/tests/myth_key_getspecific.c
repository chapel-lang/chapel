#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <myth/myth.h>

#define n_keys 100

myth_key_t keys[n_keys];

void * f(void * x) {
  long a = (long)x;
  unsigned short rg[3] = { a, a + 1, a + 2 };
  void * vals[n_keys];
  int i, t;
  for (i = 0; i < n_keys; i++) {
    void * v = (void *)jrand48(rg);
    vals[i] = v;
    int r = myth_setspecific(keys[i], v);
    assert(r == 0);
  }
  for (t = 0; t < 10; t++) {
    for (i = 0; i < n_keys; i++) {
      void * v = myth_getspecific(keys[i]);
      assert(v == vals[i]);
      v = (void *)jrand48(rg);
      vals[i] = v;
      myth_setspecific(keys[i], v);
      myth_yield_ex(myth_yield_option_steal_first);
    }
  }
  return 0;
}

int create_and_delete_keys() {
  long i;
  myth_thread_t th[n_keys];
  for (i = 0; i < n_keys; i++) {
    myth_key_create(&keys[i], 0);
  }
  for (i = 0; i < n_keys; i++) {
    th[i] = myth_create(f, (void *)i);
  }
  for (i = 0; i < n_keys; i++) {
    myth_join(th[i], 0);
  }
  for (i = 0; i < n_keys; i++) {
    int r = myth_key_delete(keys[i]);
    assert(r == 0);
  }
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <myth/myth.h>

#define n_keys 100

char keys[n_keys];

void alloc_key() {
  myth_key_t x = -1;
  int r = myth_key_create(&x, 0);
  assert(r == 0);
  assert(x >= 0);
  assert(x < n_keys);
  assert(keys[x] == 0);
  keys[x] = 1;
}

void * f(void * x) {
  alloc_key();
  return 0;
}

int create_and_delete_keys() {
  myth_thread_t th[n_keys];
  long i;
  for (i = 0; i < n_keys; i++) {
    keys[i] = 0;
  }
  for (i = 0; i < n_keys; i++) {
    th[i] = myth_create(f, (void *)i);
  }
  for (i = 0; i < n_keys; i++) {
    myth_join(th[i], 0);
  }
  for (i = 0; i < n_keys; i++) {
    assert(keys[i] == 1);
  }
  for (i = 0; i < n_keys; i++) {
    int r = myth_key_delete((myth_key_t)i);
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

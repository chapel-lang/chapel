/* 
 * test myth_create and join
 */

#include <assert.h>
#include "myth.h"

void * f(void * a) {
  return a + 456;
}

int main() {
  void * res = 0;
  myth_thread_t th = myth_create(f, (void *)123);
  myth_join(th, &res);
  assert(res == (void *)579);
  printf("OK\n");
  return 0;
}


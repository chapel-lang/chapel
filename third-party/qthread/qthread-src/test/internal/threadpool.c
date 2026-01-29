#include "argparsing.h"
#include "qt_threadpool.h"

static int on_thread_test(void *arg) {
  printf("hello from thread\n");
  return 0;
}

int main() {
  hw_pool_init(2);
  hw_pool_destroy();
  hw_pool_init(2);
  hw_pool_run_on_all(&on_thread_test, NULL);
  hw_pool_destroy();
  printf("exited successfully\n");
  fflush(stdout);
  return 0;
}

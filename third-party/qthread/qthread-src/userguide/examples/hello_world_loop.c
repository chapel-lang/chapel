#include <qthread/qloop.h>
#include <qthread/qthread.h>
#include <stdio.h>

static void greeter(size_t const startat, size_t const stopat, void *arg) {
  size_t i;

  for (i = startat; i < stopat; i++) printf("#%lu: Hello, world!\n", i);
}

int main(int argc, char *argv[]) {
  qthread_initialize();

  qt_loop(0, 10, greeter, NULL);

  return 0;
}

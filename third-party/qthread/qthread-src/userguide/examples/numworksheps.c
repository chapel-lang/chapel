#include <qthread/qthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  qthread_initialize();

  printf("Number of workers: %d\n", qthread_num_workers());
  printf("Number of shepherds: %d\n", qthread_num_shepherds());

  return 0;
}

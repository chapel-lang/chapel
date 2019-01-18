/*
 * test program for the task_group class (identical to
 * the one found in TBB) on top of 
 * MassiveThreads/Qthreads/NANOX/TBB
 */

#include <stdio.h>

#define DAG_RECORDER 2
#include <mtbb/task_group.h>

#include <unistd.h>

long bin(int n) {
  if (n == 0) {
    //usleep(1000);
    return 1;
  } else {
    mtbb::task_group tg;
    long x, y;
    tg.run([=,&x] { x = bin(n - 1); });
    y = bin(n - 1);
    tg.wait();
    return x + y;
  }
}

int main(int argc, char ** argv) {
  int n = (argc > 1 ? atoi(argv[1]) : 10);
  dr_start(0);
  long x = bin(n);
  printf("bin(%d) = %ld\n", n, x);
  dr_stop();
  return 0;
}


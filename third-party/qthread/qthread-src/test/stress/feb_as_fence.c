#include <stdint.h>

#include "qthread/qthread.h"

#include "argparsing.h"

#define NUM_CACHE_LINES 2048
#define CACHE_LINE_SIZE 64
#define BUFFER_SIZE (NUM_CACHE_LINES * CACHE_LINE_SIZE)
// Large number coprime to the buffer size
#define JUMP 1232121

static aligned_t flag1 = 0ull;
static aligned_t flag2 = 0ull;
char buffer[BUFFER_SIZE];

static aligned_t worker(void *unused) {
  qthread_readFE(NULL, &flag1);
  size_t idx = 0;
  for (size_t i = 0ull; i < BUFFER_SIZE; i++) {
    idx += JUMP;
    idx %= BUFFER_SIZE;
    test_check(buffer[idx] == 1);
    buffer[idx] = 2;
  }
  aligned_t out = 1ull;
  qthread_writeEF(&flag2, &out);
  return 0ull;
}

int main(int argc, char *argv[]) {
  int status = qthread_initialize();
  test_check(!status);
  status = qthread_empty(&flag1);
  test_check(!status);
  qthread_empty(&flag2);
  status = qthread_empty(&flag2);
  test_check(!status);
  qthread_fork(worker, NULL, NULL);
  for (size_t i = 0ull; i < BUFFER_SIZE; i++) {
    test_check(buffer[i] == 0);
    buffer[i] = 1;
  }
  aligned_t out = 1ull;
  qthread_writeEF(&flag1, &out);
  qthread_readFE(NULL, &flag2);
  for (size_t i = 0ull; i < BUFFER_SIZE; i++) { test_check(buffer[i] == 2); }
  return 0;
}


#include "argparsing.h"
#include <qthread/qthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char const cfilename[] = "test_qthread_read.XXXXXXXXXXXXX";
#define BUFFER_SIZE 32u // strlen(cfilename) + 1

static aligned_t reader(void *arg) {
  int ret;
  char buf[BUFFER_SIZE];

  memset(buf, 0, BUFFER_SIZE);
  iprintf("in reader function\n");
  ret = read((int)(intptr_t)arg, buf, 5);
  iprintf("read '%s'\n", buf);
  iprintf("ret = %i\n", ret);
  test_check(ret == 5);
  if (strncmp(buf, cfilename, 5)) {
    buf[5] = 0;
    fprintf(stderr, "Corrupt Read! '%s' != '%s'!\n", buf, cfilename);
    abort();
  }

  return 0;
}

int main(int argc, char *argv[]) {
  aligned_t t;
  int fd;
  char filename[BUFFER_SIZE];
  ssize_t ret;

  CHECK_VERBOSE();

  snprintf(filename, BUFFER_SIZE, "%s", cfilename);
  iprintf("filename = '%s'\n", filename);

  /* First, set up a temporary file */
  fd = mkstemp(filename);
  iprintf("filename = '%s'\n", filename);
  iprintf("fd = %i\n", fd);
  if (fd < 0) { perror("mkstemp failed"); }
  test_check(fd >= 0);

  ret = write(fd, cfilename, BUFFER_SIZE - 1u);
  test_check(ret == BUFFER_SIZE - 1u);

  ret = (ssize_t)lseek(fd, 0, SEEK_SET);
  test_check(ret == 0);

  reader((void *)(intptr_t)fd);

  ret = (ssize_t)lseek(fd, 0, SEEK_SET);
  test_check(ret == 0);

  test_check(qthread_initialize() == 0);

  CHECK_VERBOSE();

  qthread_fork(reader, (void *)(intptr_t)fd, &t);
  qthread_readFF(NULL, &t);

  qthread_finalize();

  close(fd);
  unlink(filename);

  return 0;
}

/* vim:set expandtab */

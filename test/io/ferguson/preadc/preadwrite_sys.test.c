#include "sys_basic.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "sys.h"

int main() {
  char* buf;
  size_t buflen = 4096;
  char* data = "aaaaaaaaaaaaaaaaaaaa";
  int datalen = strlen(data);
  int fd;
  ssize_t got;
  err_t err;

  buf = malloc(buflen);
  assert(buf);

  // open file
  fd = open("tmp.data", O_RDWR | O_CREAT, S_IRWXU);
  assert(fd>=0);
  
  // copy data to buf
  memset(buf, 0, buflen);
  memcpy(buf, data, datalen);
  // write data
  err = sys_pwrite(fd, buf, datalen, 0, &got);
  assert(!err);
  assert(got == datalen);
 
  // clear buf
  memset(buf, 0, buflen);
  // read data
  err = sys_pread(fd, buf, buflen, 0, &got);
  assert(!err);
  assert(got == datalen);
  // check data
  assert(0 == memcmp(data, buf, datalen));
 
  close(fd);
  unlink("tmp.data");

  printf("OK\n");

  return 0;
}


#include "sys.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char** argv)
{

  FILE* f = fopen("test.bin", "w+");
  int fd = fileno(f);
  int i;
  unsigned char byte;
  err_t err;
  ssize_t got_num;

  for( i = 0; i < 256; i++ ) {
    fputc(i, f);
  }
  fflush(f);

  for( i = 0; i < 256; i++ ) {
    byte = 0;
    got_num = 0;
    err = sys_pread(fd, &byte, 1, i, &got_num);
    assert( got_num == 1 );
    assert( err == 0 );
    assert( byte == (unsigned char) i );
  }

  // Make sure EOF works.
  err = sys_pread(fd, &byte, 1, 256, &got_num);
  assert( got_num == 0 );
  assert( err == EEOF );

  // Now try overwriting the original data with
  // descending bytes using sys_pwrite.
  
  for( i = 0; i < 256; i++ ) {
    byte = 255 - i;
    got_num = 0;
    err = sys_pwrite(fd, &byte, 1, i, &got_num);
    assert( got_num == 1 );
    assert( err == 0 );
  }

  // Now try reading the new data using sys_pread.
  for( i = 0; i < 256; i++ ) {
    byte = 0;
    got_num = 0;
    err = sys_pread(fd, &byte, 1, i, &got_num);
    assert( got_num == 1 );
    assert( byte == (unsigned char) (255 - i) );
  }

  return 0;
}

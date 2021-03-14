#include "qio.h"
#include <stdio.h>
#include <assert.h>


int main(int argc, char** argv)
{
  qbuffer_t* buf;
  qio_file_t* f;
  qio_channel_t* ch;
  qioerr err;
  char* testone = "the first test ";
  char* testtwo = "the second test";
  char* expect = "the first test the second test";
  qbytes_t* b;
  int64_t len;

  err = qbuffer_create(&buf);
  assert(!err);

  err = qio_file_open_mem(&f, buf, NULL);
  assert(!err);

  err = qio_file_length(f, &len);
  assert(!err);

  err = qio_channel_create(&ch, f, 0, 1, 1, len, INT64_MAX, NULL);
  assert(!err);

  err = qio_channel_write_amt(1, ch, testone, strlen(testone));
  assert(!err);

  err = qio_channel_close(1, ch);
  assert(!err);

  qio_channel_release(ch);

  ch = NULL;
  err = qio_file_length(f, &len);
  assert(!err);

  err = qio_channel_create(&ch, f, 0, 1, 1, len, INT64_MAX, NULL);
  assert(!err);

  err = qio_channel_write_amt(1, ch, testtwo, strlen(testtwo));
  assert(!err);

  err = qio_channel_close(1, ch);
  assert(!err);

  qio_channel_release(ch);

  qio_file_release(f);

  // now we should only have one part in our buffer!
  assert(qbuffer_num_parts(buf) == 1);
  // and it should be equal to expect
  err = qbuffer_flatten(buf, qbuffer_begin(buf), qbuffer_end(buf), &b);
  assert(!err);

  assert( b->len == strlen(expect) );
  assert( 0 == memcmp(b->data, expect, b->len) );

  qbytes_release(b);
  qbuffer_release(buf);

  printf("qio_memfile_test PASS\n");

  return 0;
}


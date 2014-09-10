#include "qio.h"
#include <assert.h>

void check_mark_easy(void)
{
  qio_file_t* f;
  qio_channel_t* writing;
  qio_channel_t* reading;
  int64_t offset;

  qioerr err;

  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  err = qio_channel_create(&writing, f, 0, 0, 1, 0, INT64_MAX, NULL);
  assert(!err);

  err = qio_channel_offset(true, writing, &offset);
  assert(!err);
  assert(offset == 0);

  err = qio_channel_write_byte(true, writing, 'a');
  assert(!err);
  err = qio_channel_offset(true, writing, &offset);
  assert(!err);
  assert(offset == 1);

  err = qio_channel_mark(true, writing);
  assert(!err);

  err = qio_channel_write_byte(true, writing, 'b');
  assert(!err);
  err = qio_channel_offset(true, writing, &offset);
  assert(!err);
  assert(offset == 2);

  err = qio_channel_revert(true, writing);
  assert(!err);
  err = qio_channel_offset(true, writing, &offset);
  assert(!err);
  assert(offset == 1);

  err = qio_channel_mark(true, writing);
  assert(!err);

  err = qio_channel_write_byte(true, writing, 'c');
  assert(!err);
  err = qio_channel_offset(true, writing, &offset);
  assert(!err);
  assert(offset == 2);

  err = qio_channel_commit(true, writing);
  assert(!err);
  assert(offset == 2);

  qio_channel_release(writing);

  // Read the data a byte at a time.
  err = qio_channel_create(&reading, f, 0, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  assert('a' == qio_channel_read_byte(true, reading));
  assert('c' == qio_channel_read_byte(true, reading));
  assert(-EEOF == qio_channel_read_byte(true, reading));

  qio_channel_release(reading);

  // Close the file.
  qio_file_release(f);
}

void check_mark(char* writepattern, char* readpattern)
{
  qio_file_t* f;
  qio_channel_t* writing;
  qio_channel_t* reading;
  int64_t offset;
  int32_t got;

  qioerr err;
  int i;

  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  err = qio_channel_create(&writing, f, 0, 0, 1, 0, INT64_MAX, NULL);
  assert(!err);

  err = qio_channel_offset(true, writing, &offset);
  assert(!err);
  assert(offset == 0);

  for( i = 0; writepattern[i]; i++ ) {
    switch (writepattern[i]) {
      case 'M':
        err = qio_channel_mark(true, writing);
        break;
      case 'R':
        err = qio_channel_revert(true, writing);
        break;
      case 'C':
        err = qio_channel_commit(true, writing);
        break;
      case 'A':
        err = qio_channel_advance(true, writing, 1);
        break;
      default:
        err = qio_channel_write_byte(true, writing, writepattern[i]);
    }
    assert(!err);
  }

  qio_channel_release(writing);

  // Read the data a byte at a time.
  err = qio_channel_create(&reading, f, 0, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  for( i = 0; readpattern[i]; i++ ) {
    switch (readpattern[i]) {
      case 'M':
        err = qio_channel_mark(true, reading);
        break;
      case 'R':
        err = qio_channel_revert(true, reading);
        break;
      case 'C':
        err = qio_channel_commit(true, reading);
        break;
      case 'A':
        err = qio_channel_advance(true, reading, 1);
        break;
      default:
        got = qio_channel_read_byte(true, reading);
        if( got >= 0 ) assert(got == readpattern[i]);
        else err = qio_int_to_err(-got);
    }
    assert(!err);
  }

  got = qio_channel_read_byte(true, reading);
  assert( got == - EEOF );

  qio_channel_release(reading);

  // Close the file.
  qio_file_release(f);
}

int main(int argc, char** argv)
{
  int sizes[] = {qbytes_iobuf_size, 1, 2, 0};

  qbytes_iobuf_size = 2;
  check_mark("aaaa", "MaaaaRaaaa");

  for( int i = 0; sizes[i] != 0; i++ ) {
    qbytes_iobuf_size = sizes[i];

    check_mark_easy();
    check_mark("aMbRc", "ac");
    check_mark("aMbCc", "abc");
    check_mark("MMaaRRb", "b");
    check_mark("MMMaaRRRb", "b");
    check_mark("MMMMMaaRRRRRb", "b");
    check_mark("MMMMaaRbbRccRddRx", "x");
    check_mark("MMMMMMaaRRRbbRccRddRx", "x");
    check_mark("abc", "aMbRMbCc");
    check_mark("abc", "MabcRabc");
    check_mark("abc", "MabcC");
    check_mark("abc", "MMabRabCc");
    check_mark("abc", "MabMcRcC");

    check_mark("a", "MaRa");
    check_mark("aa", "MaaRaa");
    check_mark("aaaa", "MaaaaRaaaa");
    check_mark("aaaaaaaa", "MaaaaaaaaRaaaaaaaa");
    check_mark("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "MaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaRaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    check_mark("abcdefjhijklmnopqrstuvwxyz", "MabCMMcdCCefMjhijCklmnoMpqrstuCvwxyz");
  }

  printf("qio_mark_test PASS\n");

}


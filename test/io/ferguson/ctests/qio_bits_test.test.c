#include "qio.h"
#include <assert.h>

#ifdef CHPL_VALGRIND_TEST
int valgrind = 1;
#else
int valgrind = 0;
#endif

int verbose = 0;

void check_bits(int offset, int padding)
{
  qio_file_t* f;
  qio_channel_t* writing;
  qio_channel_t* reading;
  // bitpats is in pairs of data, num bits
  uint64_t bitpats[] = {1,1, 0,1, 1,1, 1,1, // 1011 -> b
                        0,1, 0,1, 1,1, 0,1, // 0010 -> 2
                        2,2, 3,2,           // 1011 -> b
                        2,2, 0,2,           // 1000 -> 8
                        0xffffffffffffffffull,64, // ffff ffff ffff ffff
                        0,6, 0x3f,6,        // 0000 0011 1111 -> 03f
                        0,1, 0xff,8, 0,1, 0x7f,7, 3,3,// 0111 1111 1011 1111 1011 ->7fbfb
                        0,3, 1,2, 0,3,      // 0000 1000 -> 08
                        1,1, 0x8100ff00ff20ff81ull,64, 0,7, // 1100 0000 1000 0000 0111 1111 1000 0000 0111 1111 1001 0000 0111 1111 1100 0000 1000 0000 -> c0807f807f907fc080
                        1,2
                       };

  uint8_t expect[] = {0xb2, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xf7, 0xfb, 0xfb, 0x08, 0xc0, 0x80, 0x7f, 0x80, 0x7f, 0x90, 0x7f, 0xc0, 0x80, 0x40, 0xff, 0x40, 0xff, 0x40};
  qioerr err;
  int i;


  if( verbose ) printf("\nBIT IO TEST AT OFFSET %i PADDING %i\n\n", offset, padding);

  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  err = qio_channel_create(&writing, f, 0, 0, 1, 0, INT64_MAX, NULL);
  assert(!err);

  for( i = 0; i < offset; i++ ) {
    err = qio_channel_write_byte(true, writing, 0xc1);
    assert(!err);
  }

  // Write stuff to the file.
  for( i = 0; i < sizeof(bitpats)/(2*sizeof(uint64_t)); i++ ) {
    if( verbose ) printf("Writing %i bits of %llx\n", (int) bitpats[2*i + 1], (long long int) bitpats[2*i]);
    err = qio_channel_write_bits(true, writing, bitpats[2*i], bitpats[2*i + 1]);
    assert(!err);
  }
  if( verbose ) printf("Writing byte 0xff at the end\n");
  err = qio_channel_write_byte(true, writing, 0xff);
  assert(!err);
  if( verbose ) printf("Writing bits 010 at the end\n");
  err = qio_channel_write_bits(true, writing, 2, 3);
  assert(!err);
  if( verbose ) printf("Writing byte 0xff at the end\n");
  err = qio_channel_write_byte(true, writing, 0xff);
  assert(!err);
  if( verbose ) printf("Writing bits 010 at the end\n");
  err = qio_channel_write_bits(true, writing, 2, 3);
  assert(!err);

  for( i = 0; i < padding; i++ ) {
    err = qio_channel_write_byte(true, writing, 0xc1);
    assert(!err);
  }


  qio_channel_release(writing);

  // Read the data a byte at a time.
  err = qio_channel_create(&reading, f, 0, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  for( i = 0; i < offset; i++ ) {
    uint8_t got = 0;
    err = qio_channel_read_amt(true, reading, &got, 1);
    assert(!err);
    assert(got == 0xc1);
  }

  // Read stuff from the file.
  for( i = 0; i < sizeof(expect); i++ ) {
    uint8_t got = 0;
    err = qio_channel_read_amt(true, reading, &got, 1);
    assert(!err);
    if( verbose ) printf("Reading byte expecting %x got %x\n", (int) expect[i], (int) got);
    assert( got == expect[i] );
  }

  for( i = 0; i < padding; i++ ) {
    uint8_t got = 0;
    err = qio_channel_read_amt(true, reading, &got, 1);
    assert(!err);
    assert(got == 0xc1);
  }


  qio_channel_release(reading);

  // Read the data with the binary reader.
  err = qio_channel_create(&reading, f, 0, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  for( i = 0; i < offset; i++ ) {
    uint8_t got = 0;
    err = qio_channel_read_amt(true, reading, &got, 1);
    assert(!err);
    assert(got == 0xc1);
  }

  // Read stuff from the file.
  for( i = 0; i < sizeof(bitpats)/(2*sizeof(uint64_t)); i++ ) {
    uint64_t got = 0;
    err = qio_channel_read_bits(true, reading, &got, bitpats[2*i + 1]);
    assert(!err);
    if( verbose ) printf("Reading %i bits expecting %llx got %llx\n", (int) bitpats[2*i + 1], (long long int) bitpats[2*i], (long long int) got);
    assert( got == bitpats[2*i] );
  }

  {
    uint8_t got = 0;
    uint64_t b = 0;

    got = 0;
    err = qio_channel_read_amt(true, reading, &got, 1);
    assert(!err);
    assert( got == 0xff );
  
    b = 0;
    err = qio_channel_read_bits(true, reading, &b, 3);
    assert(!err);
    assert( b == 2 );
 
    got = 0;
    err = qio_channel_read_amt(true, reading, &got, 1);
    assert(!err);
    assert( got == 0xff );
 
    b = 0;
    err = qio_channel_read_bits(true, reading, &b, 3);
    assert(!err);
    assert( b == 2 );
  }

  for( i = 0; i < padding; i++ ) {
    uint8_t got = 0;
    err = qio_channel_read_amt(true, reading, &got, 1);
    assert(!err);
    assert(got == 0xc1);
  }

  qio_channel_release(reading);

  // Close the file.
  qio_file_release(f);
}

void check_write_read_pat(int width, int num, int pat, qio_chtype_t type, qio_hint_t file_hints, qio_hint_t ch_hints, char reopen)
{
  qio_file_t* f;
  qio_channel_t* writing;
  qio_channel_t* reading;
  qioerr err;
  int memory;
  char* chhints;
  char* fhints;
  char filename[128];
  int fd = -1;
  uint64_t one = 1;
  uint64_t mask = (one << width) - 1;
  if( width == 64 ) mask = -1;

  strcpy(filename,"/tmp/qio_bits_testXXXXXX");

  ch_hints = (ch_hints & ~ QIO_CHTYPEMASK) | type;
  memory = 0;

  if( (file_hints & QIO_METHODMASK) == QIO_METHOD_MEMORY ||
      (ch_hints & QIO_METHODMASK) == QIO_METHOD_MEMORY ) {
    memory = 1;
  }
  if( memory ) {
    file_hints = (file_hints & ~ QIO_METHODMASK ) | QIO_METHOD_MEMORY;
    ch_hints = (ch_hints & ~ QIO_METHODMASK ) | QIO_METHOD_MEMORY;
  }
  if( memory && type == QIO_CH_ALWAYS_UNBUFFERED ) return;
  if( memory && reopen ) return;
  if( (ch_hints & QIO_METHODMASK) == QIO_METHOD_FREADFWRITE ) {
    if( (file_hints & QIO_METHODMASK) != QIO_METHOD_FREADFWRITE ) return;
  }
  if( (ch_hints & QIO_METHODMASK) == QIO_METHOD_MMAP ) {
    if( (file_hints & QIO_METHODMASK) != QIO_METHOD_MMAP ) return;
  }

  fhints = qio_hints_to_string(file_hints);
  chhints = qio_hints_to_string(ch_hints);
  if( verbose )
    printf("check_write_read_pat(width=%i, num=%i, pat=%i, type=%i, file_hints=%s %i, ch_hints=%s %i, reopen=%i)\n",
         width, num, pat, type,
         fhints, (int) file_hints, chhints, (int) ch_hints,  (int) reopen);

  free(fhints);
  free(chhints);

  if( memory ) {
    err = qio_file_open_mem_ext(&f, NULL, QIO_FDFLAG_READABLE|QIO_FDFLAG_WRITEABLE|QIO_FDFLAG_SEEKABLE, file_hints, NULL);
    assert(!err);
  } else {
    if( reopen ) {
      fd = mkstemp(filename);
      close(fd);
      err = qio_file_open_access(&f, filename, "w", file_hints, NULL);
      assert(!err);

    } else {
      err = qio_file_open_tmp(&f, 0, NULL);
      assert(!err);
    }
  }

  err = qio_channel_create(&writing, f, ch_hints, 0, 1, 0, INT64_MAX, NULL);
  assert(!err);

  for( int i = 0; i < num; i++ ) {
    uint64_t x;
    if( pat == 0 ) {
      if( i & 1 ) x = mask;
      else x = 0;
    } else {
      x = ((uint64_t) i) & mask;
    }
    { // check offset
      int64_t off = qio_channel_offset_unlocked(writing);
      assert( off == (width * i + 7) / 8 );
    }
    err = qio_channel_write_bits(false, writing, x, width);
    assert(!err);
  }
  qio_channel_release(writing);

  // Reopen the file if we're doing reopen
  if( reopen ) {
    // Close the file.
    qio_file_release(f);
    err = qio_file_open_access(&f, filename, "r", file_hints, NULL);
    assert(!err);
  }
  // Rewind the file 
  if( !memory ) {
    off_t off;

    sys_lseek(f->fd, 0, SEEK_SET, &off);
    assert(!err);
  }



  err = qio_channel_create(&reading, f, ch_hints, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  for( int i = 0; i < num; i++ ) {
    uint64_t got = 0;
    uint64_t x;
    if( pat == 0 ) {
      if( i & 1 ) x = mask;
      else x = 0;
    } else {
      x = ((uint64_t) i) & mask;
    }
    { // check offset
      int64_t off = qio_channel_offset_unlocked(reading);
      assert( off == (width * i + 7) / 8 );
    }
    //printf("Reading at %lli\n", (long long int) qio_channel_offset_unlocked(reading));
    err = qio_channel_read_bits(false, reading, &got, width);
    assert(!err);
    //printf("Got  %lli\n", (long long int) got);
    if( got != x ) {
      printf("Fails (%i %i %i) at %i got=%llx expect=%llx\n", width, num, pat, i, (unsigned long long int) got, (unsigned long long int) x);
      assert(got == x);
      break;
    }
  }

  qio_channel_release(reading);

  // Close the file.
  qio_file_release(f);

  if( reopen ) {
    unlink(filename);
  }
}

int main(int argc, char** argv)
{
  int offset, padding;
  int width, logn, maxlogn;
  qio_chtype_t type;
  qio_hint_t hints[] = {QIO_METHOD_DEFAULT, QIO_METHOD_READWRITE, QIO_METHOD_PREADPWRITE, QIO_METHOD_FREADFWRITE, QIO_METHOD_MEMORY, QIO_METHOD_MMAP, QIO_METHOD_MMAP|QIO_HINT_PARALLEL, QIO_METHOD_PREADPWRITE | QIO_HINT_NOFAST};
  int nhints = sizeof(hints)/sizeof(qio_hint_t);
  int file_hint, ch_hint;

  {
    //for( file_hint = 0; file_hint < nhints; file_hint++ ) {
    //  check_write_read_pat(1, 262144, 0, 0, file_hint, file_hint, 0);
    //}

    //check_write_read_pat(32,4096,0,3,QIO_METHOD_PREADPWRITE,0,1);
    //check_write_read_pat(32,4096,0,3,0,0,1);
    //exit(0);
//check_write_read_pat(width=32, num=4096, pat=0, type=3, file_hints=default_type default, ch_hints=buffered default, reopen=1)

    // Run unit tests.
    for( offset = 0; offset < 63; offset++ ) {
      for( padding = 0; padding < 63; padding++ ) {
        check_bits(offset, padding);
      }
    }

    maxlogn = 19;
    if( valgrind ) maxlogn = 10;

    for( logn = 0; logn < maxlogn; logn+=9 ) {
      for( width = 1; width <= 64; width++ ) {
        for( file_hint = 0; file_hint < nhints; file_hint++ ) {
          ch_hint = file_hint;
          type = 0;
          check_write_read_pat(width,1 << logn,0,type,file_hint,ch_hint,0);
          check_write_read_pat(width,1 << logn,1,type,file_hint,ch_hint,1);
        }
      }
    }
  }

  // Do extra testing that showed a bug at one point...
  if( !valgrind ) {

    int i;
    int n = 256*1024*1024;

    qio_file_t* f;
    qio_channel_t* writing;
    qio_channel_t* reading;
    uint64_t got;
    uint64_t expect;
    qioerr err;
    

    //qbytes_iobuf_size = 256;

    err = qio_file_open_mem(&f, NULL, NULL);
    assert(!err);
    //err = qio_file_open_access(&f, "test.bin", "w+", 0, NULL);
    //assert(!err);

    err = qio_channel_create(&writing, f, 0, 0, 1, 0, INT64_MAX, NULL);
    assert(!err);

    for( i = 0; i < n; i++ ) {
      err = qio_channel_write_bits(false, writing, i & 0x1f, 5);
      //err = qio_channel_write_bits(false, writing, 0x1f, 5);
      assert(!err);
    }

    qio_channel_release(writing);

    if( argc > 2 ) {
      err = qio_channel_create(&reading, f, 0, 1, 0, 0, INT64_MAX, NULL);
      assert(!err);

      for( i = 0; i < n; i++ ) {
        got = 0;
        err = qio_channel_read_bits(false, reading, &got, 5);
        assert(!err);
        expect = i & 0x1f;
        //expect = 0x1f;
        if( got != expect ) {
          printf("Failed at i=%i got %llx expect %llx\n", i, (long long int) got, (long long int) expect);
          assert( got == expect );
        }
      }

      qio_channel_release(reading);
    }

    qio_file_release(f);
  }

  printf("qio_bits_test PASS\n");

  return 0;
}


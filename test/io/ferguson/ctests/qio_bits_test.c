#include "qio.h"
#include <assert.h>

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
  err_t err;
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

void check_write_read_pat(int width, int num, int pat)
{
  qio_file_t* f;
  qio_channel_t* writing;
  qio_channel_t* reading;
  err_t err;
  uint64_t one = 1;
  uint64_t mask = (one << width) - 1;
  if( width == 64 ) mask = -1;
 
  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  err = qio_channel_create(&writing, f, 0, 0, 1, 0, INT64_MAX, NULL);
  assert(!err);

  for( int i = 0; i < num; i++ ) {
    uint64_t x;
    if( pat == 0 ) {
      if( i & 1 ) x = mask;
      else x = 0;
    } else {
      x = ((uint64_t) i) & mask;
    }
    err = qio_channel_write_bits(false, writing, x, width);
    assert(!err);
  }
  qio_channel_release(writing);

  err = qio_channel_create(&reading, f, 0, 1, 0, 0, INT64_MAX, NULL);
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
    err = qio_channel_read_bits(true, reading, &got, width);
    assert(!err);
    if( got != x ) {
      printf("Fails (%i %i %i) at %i got=%llx expect=%llx\n", width, num, pat, i, (unsigned long long int) got, (unsigned long long int) x);
      assert(got == x);
      break;
    }
  }

  qio_channel_release(reading);

  // Close the file.
  qio_file_release(f);
}

void check_write_read(int width, int num)
{
  check_write_read_pat(width, num, 0);
  check_write_read_pat(width, num, 1);
}

int main(int argc, char** argv)
{
  int offset, padding;
  int width, logn;


  if( argc == 1 ) {
    // Run unit tests.
    for( offset = 0; offset < 63; offset++ ) {
      for( padding = 0; padding < 63; padding++ ) {
        check_bits(offset, padding);
      }
    }
    for( width = 1; width <= 64; width++ ) {
      for( logn = 0; logn < 18; logn++ ) {
        check_write_read(width, 1 << logn);
      }
    }
  }

  if( argc > 1 ) {

    int i;
    int n = 256*1024*1024;

    qio_file_t* f;
    qio_channel_t* writing;
    qio_channel_t* reading;
    uint64_t got;
    uint64_t expect;
    err_t err;
    

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

  return 0;
}


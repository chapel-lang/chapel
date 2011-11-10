#include "qio.h"
#include <assert.h>

void check_bits(void)
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
                        1,1, 0x8100ff00ff20ff81ull,64, 0,7 // 1100 0000 1000 0000 0111 1111 1000 0000 0111 1111 1001 0000 0111 1111 1100 0000 1000 0000 -> c0807f807f907fc080
                       };

  uint8_t expect[] = {0xb2, 0xb8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0xf7, 0xfb, 0xfb, 0x08, 0xc0, 0x80, 0x7f, 0x80, 0x7f, 0x90, 0x7f, 0xc0, 0x80};
  err_t err;
  int i;


  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  err = qio_channel_create(&writing, f, 0, 0, 1, 0, INT64_MAX, NULL);
  assert(!err);

  // Write stuff to the file.
  for( i = 0; i < sizeof(bitpats)/(2*sizeof(uint64_t)); i++ ) {
    printf("Writing %i bits of %llx\n", (int) bitpats[2*i + 1], (long long int) bitpats[2*i]);
    err = qio_channel_write_bits(true, writing, bitpats[2*i], bitpats[2*i + 1]);
    assert(!err);
  }

  qio_channel_release(writing);

  // Read the data a byte at a time.
  err = qio_channel_create(&reading, f, 0, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  // Read stuff from the file.
  for( i = 0; i < sizeof(expect); i++ ) {
    uint8_t got = 0;
    err = qio_channel_read_amt(true, reading, &got, 1);
    assert(!err);
    printf("Reading byte expecting %x got %x\n", (int) expect[i], (int) got);
    assert( got == expect[i] );
  }

  qio_channel_release(reading);

  // Read the data with the binary reader.
  err = qio_channel_create(&reading, f, 0, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  // Read stuff from the file.
  for( i = 0; i < sizeof(bitpats)/(2*sizeof(uint64_t)); i++ ) {
    uint64_t got = 0;
    err = qio_channel_read_bits(true, reading, &got, bitpats[2*i + 1]);
    assert(!err);
    printf("Reading %i bits expecting %llx got %llx\n", (int) bitpats[2*i + 1], (long long int) bitpats[2*i], (long long int) got);
    assert( got == bitpats[2*i] );
  }

  qio_channel_release(reading);

  // Close the file.
  qio_file_release(f);
}

int main(int argc, char** argv)
{
  check_bits();


  return 0;
}


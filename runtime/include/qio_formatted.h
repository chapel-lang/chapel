#ifndef _QIO_FORMATTED_H_
#define _QIO_FORMATTED_H_

#include "qio.h"

#include "bswap.h"

// true 1 false 0   __bool_true_false_are_defined
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h> // HUGE_VAL
#include <float.h> // DBL_MANT_DIG
#include <wchar.h>
#include <assert.h>


#include "qio_style.h"

extern int glocale_utf8; // for testing use.

// Read/Write methods for Binary I/O

static always_inline
err_t qio_channel_read_int8(const int threadsafe, qio_channel_t* restrict ch, int8_t* restrict ptr) {
  return qio_channel_read_amt(threadsafe, ch, ptr, 1);
}

static always_inline
err_t qio_channel_write_int8(const int threadsafe, qio_channel_t* restrict ch, int8_t x) {
  return qio_channel_write_amt(threadsafe, ch, &x, 1);
}

static always_inline
err_t qio_channel_read_uint8(const int threadsafe, qio_channel_t* restrict ch, uint8_t* restrict ptr) {
  return qio_channel_read_amt(threadsafe, ch, ptr, 1);
}

static always_inline
err_t qio_channel_write_uint8(const int threadsafe, qio_channel_t* restrict ch, uint8_t x) {
  return qio_channel_write_amt(threadsafe, ch, &x, 1);
}



static always_inline
err_t qio_channel_read_int16(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int16_t* restrict ptr) {
  err_t err;
  int16_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 2);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be16toh(x);
  if( byteorder == QIO_LITTLE ) x = le16toh(x);
  *ptr = x;
  return 0;
}

static always_inline
err_t qio_channel_write_int16(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int16_t x) {
  if( byteorder == QIO_BIG ) x = htobe16(x);
  if( byteorder == QIO_LITTLE ) x = htole16(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 2);
}

static always_inline
err_t qio_channel_read_uint16(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint16_t* restrict ptr) {
  err_t err;
  uint16_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 2);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be16toh(x);
  if( byteorder == QIO_LITTLE ) x = le16toh(x);
  *ptr = x;
  return 0;
}

static always_inline
err_t qio_channel_write_uint16(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint16_t x) {
  if( byteorder == QIO_BIG ) x = htobe16(x);
  if( byteorder == QIO_LITTLE ) x = htole16(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 2);
}


static always_inline
err_t qio_channel_read_int32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int32_t* restrict ptr) {
  err_t err;
  int32_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 4);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be32toh(x);
  if( byteorder == QIO_LITTLE ) x = le32toh(x);
  *ptr = x;
  return 0;
}

static always_inline
err_t qio_channel_write_int32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int32_t x) {
  if( byteorder == QIO_BIG ) x = htobe32(x);
  if( byteorder == QIO_LITTLE ) x = htole32(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 4);
}

static always_inline
err_t qio_channel_read_uint32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint32_t* restrict ptr) {
  err_t err;
  uint32_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 4);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be32toh(x);
  if( byteorder == QIO_LITTLE ) x = le32toh(x);
  *ptr = x;
  return 0;
}

static always_inline
err_t qio_channel_write_uint32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint32_t x) {
  if( byteorder == QIO_BIG ) x = htobe32(x);
  if( byteorder == QIO_LITTLE ) x = htole32(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 4);
}

static always_inline
err_t qio_channel_read_int64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int64_t* restrict ptr) {
  int64_t x;
  err_t err;
  err = qio_channel_read_amt(threadsafe, ch, &x, 8);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be64toh(x);
  if( byteorder == QIO_LITTLE ) x = le64toh(x);
  *ptr = x;
  return 0;
}

static always_inline
err_t qio_channel_write_int64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int64_t x) {
  if( byteorder == QIO_BIG ) x = htobe64(x);
  if( byteorder == QIO_LITTLE ) x = htole64(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 8);
}

static always_inline
err_t qio_channel_read_uint64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint64_t* restrict ptr) {
  uint64_t x;
  err_t err;
  err = qio_channel_read_amt(threadsafe, ch, &x, 8);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be64toh(x);
  if( byteorder == QIO_LITTLE ) x = le64toh(x);
  *ptr = x;
  return 0;
}

static always_inline
err_t qio_channel_write_uint64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint64_t x) {
  if( byteorder == QIO_BIG ) x = htobe64(x);
  if( byteorder == QIO_LITTLE ) x = htole64(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 8);
}

// Reading/writing varints in the same format as Google Protocol Buffers.
err_t qio_channel_read_uvarint(const int threadsafe, qio_channel_t* restrict ch, uint64_t* restrict ptr);
err_t qio_channel_read_svarint(const int threadsafe, qio_channel_t* restrict ch, int64_t* restrict ptr);
err_t qio_channel_write_uvarint(const int threadsafe, qio_channel_t* restrict ch, uint64_t num);
err_t qio_channel_write_svarint(const int threadsafe, qio_channel_t* restrict ch, int64_t num);


static always_inline
err_t qio_channel_read_int(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, void* restrict ptr, size_t len, int issigned) {

  ssize_t signed_len = len;
  if( issigned ) signed_len = - signed_len;

  switch ( signed_len ) {
    case 1:
      return qio_channel_read_uint8(threadsafe, ch, ptr);
    case -1:
      return qio_channel_read_int8(threadsafe, ch, ptr);
    case 2:
      return qio_channel_read_uint16(threadsafe, byteorder, ch, ptr);
    case -2:
      return qio_channel_read_int16(threadsafe, byteorder, ch, ptr);
    case 4:
      return qio_channel_read_uint32(threadsafe, byteorder, ch, ptr);
    case -4:
      return qio_channel_read_int32(threadsafe, byteorder, ch, ptr);
    case 8:
      return qio_channel_read_int64(threadsafe, byteorder, ch, ptr);
    case -8:
      return qio_channel_read_uint64(threadsafe, byteorder, ch, ptr);
    default:
      return EINVAL;
  }
}

static always_inline
err_t qio_channel_write_int(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, const void* restrict ptr, size_t len, int issigned) {

  ssize_t signed_len = len;
  if( issigned ) signed_len = - signed_len;

  switch ( signed_len ) {
    case 1:
      return qio_channel_write_uint8(threadsafe, ch, *(const uint8_t*) ptr);
    case -1:
      return qio_channel_write_int8(threadsafe, ch, *(const int8_t*) ptr);
    case 2:
      return qio_channel_write_uint16(threadsafe, byteorder, ch, *(const uint16_t*) ptr);
    case -2:
      return qio_channel_write_int16(threadsafe, byteorder, ch, *(const int16_t*) ptr);
    case 4:
      return qio_channel_write_uint32(threadsafe, byteorder, ch, *(const uint32_t*) ptr);
    case -4:
      return qio_channel_write_int32(threadsafe, byteorder, ch, *(const int32_t*) ptr);
    case 8:
      return qio_channel_write_uint64(threadsafe, byteorder, ch, *(const uint64_t*) ptr);
    case -8:
      return qio_channel_write_int64(threadsafe, byteorder, ch, *(const int64_t*) ptr);
    default:
      return EINVAL;
  }
}

static always_inline
err_t qio_channel_read_float32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, float* restrict ptr) {
  union {
   uint32_t i;
   float f;
  } x;
  err_t err;

  err = qio_channel_read_amt(threadsafe, ch, &x, 4);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x.i = be32toh(x.i);
  if( byteorder == QIO_LITTLE ) x.i = le32toh(x.i);
  *ptr = x.f;
  return 0;
}

static always_inline
err_t qio_channel_write_float32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, float x) {
  union {
   uint32_t i;
   float f;
  } u;

  u.f = x;

  if( byteorder == QIO_BIG ) u.i = htobe32(u.i);
  if( byteorder == QIO_LITTLE ) u.i = htole32(u.i);
  return qio_channel_write_amt(threadsafe, ch, &u, 4);
}


static always_inline
err_t qio_channel_read_float64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, double* restrict ptr) {
  union {
   uint64_t i;
   double f;
  } x;
  err_t err;

  err = qio_channel_read_amt(threadsafe, ch, &x, 8);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x.i = be64toh(x.i);
  if( byteorder == QIO_LITTLE ) x.i = le64toh(x.i);
  *ptr = x.f;
  return 0;
}


static always_inline
err_t qio_channel_write_float64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, double x) {
  union {
   uint64_t i;
   double f;
  } u;

  u.f = x;

  if( byteorder == QIO_BIG ) u.i = htobe64(u.i);
  if( byteorder == QIO_LITTLE ) u.i = htole64(u.i);
  return qio_channel_write_amt(threadsafe, ch, &u, 8);
}

static always_inline
err_t qio_channel_read_float(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, void* restrict ptr, size_t len) {
  switch ( len ) {
    case 4:
      return qio_channel_read_float32(threadsafe, byteorder, ch, ptr);
    case 8:
      return qio_channel_read_float64(threadsafe, byteorder, ch, ptr);
    default:
      return EINVAL;
  }
}

static always_inline
err_t qio_channel_write_float(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, const void* restrict ptr, size_t len) {
  switch ( len ) {
    case 4:
      return qio_channel_write_float32(threadsafe, byteorder, ch, *(const float*) ptr);
    case 8:
      return qio_channel_write_float64(threadsafe, byteorder, ch, *(const double*) ptr);
    default:
      return EINVAL;
  }
}

static always_inline
err_t qio_channel_read_complex(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, void* restrict re_ptr, void* restrict im_ptr, size_t len) {
  err_t err;
  switch ( len ) {
    case 4:
      err = qio_channel_read_float32(threadsafe, byteorder, ch, re_ptr);
      if( ! err ) {
        err = qio_channel_read_float32(threadsafe, byteorder, ch, im_ptr);
      }
      break;
    case 8:
      err = qio_channel_read_float64(threadsafe, byteorder, ch, re_ptr);
      if( ! err ) {
        err = qio_channel_read_float64(threadsafe, byteorder, ch, im_ptr);
      }
      break;
    default:
      err = EINVAL;
  }
  return err;
}

static always_inline
err_t qio_channel_write_complex(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, const void* restrict re_ptr, const void* restrict im_ptr, size_t len) {
  err_t err;
  switch ( len ) {
    case 4:
      err = qio_channel_write_float32(threadsafe, byteorder, ch, *(const float*) re_ptr);
      if( ! err ) {
        err = qio_channel_write_float32(threadsafe, byteorder, ch, *(const float*) im_ptr);
      }
      break;
    case 8:
      err = qio_channel_write_float64(threadsafe, byteorder, ch, *(const double*) re_ptr);
      if( ! err ) {
        err = qio_channel_write_float64(threadsafe, byteorder, ch, *(const double*) im_ptr);
      }
      break;
    default:
      err = EINVAL;
  }
  return err;
}

// string binary style:
// -1 -- 1 byte of length before
// -2 -- 2 bytes of length before
// -4 -- 4 bytes of length before
// -8 -- 8 bytes of length before
// -10 -- variable byte length before (hi-bit 1 means more, little endian)
// -0x01XX -- read until terminator XX is read
//  + -- nonzero positive -- read exactly this length.
err_t qio_channel_read_string(const int threadsafe, const int byteorder, const int64_t str_style, qio_channel_t* restrict ch, const char* restrict * restrict out, ssize_t* restrict len_out, ssize_t maxlen);

// string binary style:
// -1 -- 1 byte of length before
// -2 -- 2 bytes of length before
// -4 -- 4 bytes of length before
// -8 -- 8 bytes of length before
// -10 -- variable byte length before (hi-bit 1 means more, little endian)
// -0x01XX -- read until terminator XX is read
//  + -- nonzero positive -- read exactly this length.
err_t qio_channel_write_string(const int threadsafe, const int byteorder, const int64_t str_style, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len);


err_t qio_channel_scan_bool(const int threadsafe, qio_channel_t* restrict ch, uint8_t* restrict out);
err_t qio_channel_print_bool(const int threadsafe, qio_channel_t* restrict ch, uint8_t num);

err_t qio_channel_scan_int(const int threadsafe, qio_channel_t* restrict ch, void* restrict out, size_t len, int issigned);
err_t qio_channel_scan_float(const int threadsafe, qio_channel_t* restrict ch, void* restrict out, size_t len);
err_t qio_channel_print_int(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, size_t len, int issigned);
err_t qio_channel_print_float(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, size_t len);

err_t qio_channel_scan_complex(const int threadsafe, qio_channel_t* restrict ch, void* restrict re_out, void* restrict im_out, size_t len);
err_t qio_channel_print_complex(const int threadsafe, qio_channel_t* restrict ch, const void* restrict re_ptr, const void* im_ptr, size_t len);

// These methods read or write UTF-8 characters (code points).
err_t qio_channel_read_char(const int threadsafe, qio_channel_t* restrict ch, int32_t* restrict chr);
err_t qio_channel_write_char(const int threadsafe, qio_channel_t* restrict ch, int32_t chr);

err_t qio_channel_skip_past_newline(const int threadsafe, qio_channel_t* restrict ch);

err_t qio_channel_write_newline(const int threadsafe, qio_channel_t* restrict ch);

err_t qio_channel_scan_string(const int threadsafe, qio_channel_t* restrict ch, const char* restrict * restrict out, ssize_t* restrict len_out, ssize_t maxlen);

// returns 0 if it matched, or EFORMAT if it did not.
err_t qio_channel_scan_literal(const int threadsafe, qio_channel_t* restrict ch, const char* restrict match, ssize_t len, int skipws);

// Prints a string according to the style.
err_t qio_channel_print_string(const int threadsafe, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len);

// Prints a string as-is (this really just calls qio_channel_write_amt).
err_t qio_channel_print_literal(const int threadsafe, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len);

#endif


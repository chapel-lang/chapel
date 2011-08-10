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


// Read/Write methods for Binary I/O

static inline
err_t qio_channel_read_int8(const int threadsafe, qio_channel_t* ch, int8_t* ptr) {
  return qio_channel_read_amt(threadsafe, ch, ptr, 1);
}

static inline
err_t qio_channel_write_int8(const int threadsafe, qio_channel_t* ch, int8_t x) {
  return qio_channel_write_amt(threadsafe, ch, &x, 1);
}

static inline
err_t qio_channel_read_uint8(const int threadsafe, qio_channel_t* ch, uint8_t* ptr) {
  return qio_channel_read_amt(threadsafe, ch, ptr, 1);
}

static inline
err_t qio_channel_write_uint8(const int threadsafe, qio_channel_t* ch, uint8_t x) {
  return qio_channel_write_amt(threadsafe, ch, &x, 1);
}



static inline
err_t qio_channel_read_int16(const int threadsafe, const int byteorder, qio_channel_t* ch, int16_t* ptr) {
  err_t err;
  int16_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 2);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be16toh(x);
  if( byteorder == QIO_LITTLE ) x = le16toh(x);
  *ptr = x;
  return 0;
}

static inline
err_t qio_channel_write_int16(const int threadsafe, const int byteorder, qio_channel_t* ch, int16_t x) {
  if( byteorder == QIO_BIG ) x = htobe16(x);
  if( byteorder == QIO_LITTLE ) x = htole16(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 2);
}

static inline
err_t qio_channel_read_uint16(const int threadsafe, const int byteorder, qio_channel_t* ch, uint16_t* ptr) {
  err_t err;
  uint16_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 2);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be16toh(x);
  if( byteorder == QIO_LITTLE ) x = le16toh(x);
  *ptr = x;
  return 0;
}

static inline
err_t qio_channel_write_uint16(const int threadsafe, const int byteorder, qio_channel_t* ch, uint16_t x) {
  if( byteorder == QIO_BIG ) x = htobe16(x);
  if( byteorder == QIO_LITTLE ) x = htole16(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 2);
}


static inline
err_t qio_channel_read_int32(const int threadsafe, const int byteorder, qio_channel_t* ch, int32_t* ptr) {
  err_t err;
  int32_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 4);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be32toh(x);
  if( byteorder == QIO_LITTLE ) x = le32toh(x);
  *ptr = x;
  return 0;
}

static inline
err_t qio_channel_write_int32(const int threadsafe, const int byteorder, qio_channel_t* ch, int32_t x) {
  if( byteorder == QIO_BIG ) x = htobe32(x);
  if( byteorder == QIO_LITTLE ) x = htole32(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 4);
}

static inline
err_t qio_channel_read_uint32(const int threadsafe, const int byteorder, qio_channel_t* ch, uint32_t* ptr) {
  err_t err;
  uint32_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 4);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be32toh(x);
  if( byteorder == QIO_LITTLE ) x = le32toh(x);
  *ptr = x;
  return 0;
}

static inline
err_t qio_channel_write_uint32(const int threadsafe, const int byteorder, qio_channel_t* ch, uint32_t x) {
  if( byteorder == QIO_BIG ) x = htobe32(x);
  if( byteorder == QIO_LITTLE ) x = htole32(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 4);
}

static inline
err_t qio_channel_read_int64(const int threadsafe, const int byteorder, qio_channel_t* ch, int64_t* ptr) {
  int64_t x;
  err_t err;
  err = qio_channel_read_amt(threadsafe, ch, &x, 8);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be64toh(x);
  if( byteorder == QIO_LITTLE ) x = le64toh(x);
  *ptr = x;
  return 0;
}

static inline
err_t qio_channel_write_int64(const int threadsafe, const int byteorder, qio_channel_t* ch, int64_t x) {
  if( byteorder == QIO_BIG ) x = htobe64(x);
  if( byteorder == QIO_LITTLE ) x = htole64(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 8);
}

static inline
err_t qio_channel_read_uint64(const int threadsafe, const int byteorder, qio_channel_t* ch, uint64_t* ptr) {
  uint64_t x;
  err_t err;
  err = qio_channel_read_amt(threadsafe, ch, &x, 8);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be64toh(x);
  if( byteorder == QIO_LITTLE ) x = le64toh(x);
  *ptr = x;
  return 0;
}

static inline
err_t qio_channel_write_uint64(const int threadsafe, const int byteorder, qio_channel_t* ch, uint64_t x) {
  if( byteorder == QIO_BIG ) x = htobe64(x);
  if( byteorder == QIO_LITTLE ) x = htole64(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 8);
}

static inline
err_t qio_channel_read_uvarint(const int threadsafe, qio_channel_t* ch, uint64_t* ptr) {
  err_t err;
  uint8_t byte;
  long part;
  uint64_t num;
  int i;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      *ptr = 0;
      return err;
    }
  }

  num = 0;

  for( i = 0; i < 10; i++ ) {
    err = qio_channel_read_uint8(false, ch, &byte);
    if( err ) goto error;
    part = byte & 0x7f;
    num |= part << (7*i);
    if( byte & 0x80 ) { // continue.
    } else {
      break; // all done.
    }
  }

  if( !err && (byte & 0x80) ) err = EOVERFLOW;

error:
  *ptr = num;

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

// Reading/writing varints in the same format as Google Protocol Buffers.
static inline
err_t qio_channel_read_svarint(const int threadsafe, qio_channel_t* ch, int64_t* ptr) {
  err_t err;
  uint64_t u_num;
  err = qio_channel_read_uvarint(threadsafe, ch, &u_num);
  *ptr = (u_num >> 1) ^ -((int64_t)(u_num & 1));
  return err;
}

static inline
err_t qio_channel_write_uvarint(const int threadsafe, qio_channel_t* ch, uint64_t num) {
  err_t err;
  uint8_t byte;
  int i;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  for( i = 0; i < 10; i++ ) {
    byte = num & 0x7f;
    num >>= 7;
    if( num != 0 ) byte |= 0x80;
    err = qio_channel_write_uint8(false, ch, byte);
    if( err ) goto error;
    if( byte & 0x80 ) {
      // OK, continue;
    } else {
      break;
    }
  }

error:
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

static inline
err_t qio_channel_write_svarint(const int threadsafe, qio_channel_t* ch, int64_t num) {
  uint64_t u_num = (num << 1) ^ (num >> 63);
  return qio_channel_write_uvarint(threadsafe, ch, u_num);
}


static inline
err_t qio_channel_read_int(const int threadsafe, const int byteorder, qio_channel_t* ch, void* ptr, size_t len, int issigned) {

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

static inline
err_t qio_channel_write_int(const int threadsafe, const int byteorder, qio_channel_t* ch, const void* ptr, size_t len, int issigned) {

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

static inline
err_t qio_channel_read_float32(const int threadsafe, const int byteorder, qio_channel_t* ch, float* ptr) {
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

static inline
err_t qio_channel_write_float32(const int threadsafe, const int byteorder, qio_channel_t* ch, float x) {
  union {
   uint32_t i;
   float f;
  } u;

  u.f = x;

  if( byteorder == QIO_BIG ) u.i = htobe32(u.i);
  if( byteorder == QIO_LITTLE ) u.i = htole32(u.i);
  return qio_channel_write_amt(threadsafe, ch, &u, 4);
}


static inline
err_t qio_channel_read_float64(const int threadsafe, const int byteorder, qio_channel_t* ch, double* ptr) {
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


static inline
err_t qio_channel_write_float64(const int threadsafe, const int byteorder, qio_channel_t* ch, double x) {
  union {
   uint64_t i;
   double f;
  } u;

  u.f = x;

  if( byteorder == QIO_BIG ) u.i = htobe64(u.i);
  if( byteorder == QIO_LITTLE ) u.i = htole64(u.i);
  return qio_channel_write_amt(threadsafe, ch, &u, 8);
}

static inline
err_t qio_channel_read_float(const int threadsafe, const int byteorder, qio_channel_t* ch, void* ptr, size_t len) {
  switch ( len ) {
    case 4:
      return qio_channel_read_float32(threadsafe, byteorder, ch, ptr);
    case 8:
      return qio_channel_read_float64(threadsafe, byteorder, ch, ptr);
    default:
      return EINVAL;
  }
}

static inline
err_t qio_channel_write_float(const int threadsafe, const int byteorder, qio_channel_t* ch, const void* ptr, size_t len) {
  switch ( len ) {
    case 4:
      return qio_channel_write_float32(threadsafe, byteorder, ch, *(const float*) ptr);
    case 8:
      return qio_channel_write_float64(threadsafe, byteorder, ch, *(const double*) ptr);
    default:
      return EINVAL;
  }
}

static inline
err_t qio_channel_read_complex(const int threadsafe, const int byteorder, qio_channel_t* ch, void* re_ptr, void* im_ptr, size_t len) {
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

static inline
err_t qio_channel_write_complex(const int threadsafe, const int byteorder, qio_channel_t* ch, const void* re_ptr, const void* im_ptr, size_t len) {
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
err_t qio_channel_read_string(const int threadsafe, const int byteorder, const int64_t str_style, qio_channel_t* ch, const char** out, ssize_t* len_out, ssize_t maxlen);

// string binary style:
// -1 -- 1 byte of length before
// -2 -- 2 bytes of length before
// -4 -- 4 bytes of length before
// -8 -- 8 bytes of length before
// -10 -- variable byte length before (hi-bit 1 means more, little endian)
// -0x01XX -- read until terminator XX is read
//  + -- nonzero positive -- read exactly this length.
err_t qio_channel_write_string(const int threadsafe, const int byteorder, const int64_t str_style, qio_channel_t* ch, const char* ptr, ssize_t len);


err_t qio_channel_scan_bool(const int threadsafe, qio_channel_t* ch, uint8_t* out);
err_t qio_channel_print_bool(const int threadsafe, qio_channel_t* ch, uint8_t num);

err_t qio_channel_scan_int(const int threadsafe, qio_channel_t* ch, void* out, size_t len, int issigned);
err_t qio_channel_scan_float(const int threadsafe, qio_channel_t* ch, void* out, size_t len);
err_t qio_channel_print_int(const int threadsafe, qio_channel_t* ch, const void* ptr, size_t len, int issigned);
err_t qio_channel_print_float(const int threadsafe, qio_channel_t* ch, const void* ptr, size_t len);

err_t qio_channel_scan_complex(const int threadsafe, qio_channel_t* ch, void* re_out, void* im_out, size_t len);
err_t qio_channel_print_complex(const int threadsafe, qio_channel_t* ch, const void* re_ptr, const void* im_ptr, size_t len);

// These methods read or write UTF-8 characters (code points).
static inline
err_t qio_channel_read_char(const int threadsafe, qio_channel_t* ch, wchar_t* chr) {
  char mbs[MB_LEN_MAX];
  mbstate_t ps;
  size_t n;
  size_t got;
  wchar_t tmp_chr;
  err_t err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  // We always read 1 character at least.
  n = 0;
  err = qio_channel_read_amt(threadsafe, ch, &mbs[n], 1);
  if( err ) {
    if( err == EEOF ) *chr = -1;
    goto unlock;
  }
  n++;

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  while( 1 ) {
    memset(&ps, 0, sizeof(mbstate_t));
    got = mbrtowc(&tmp_chr, mbs, n, &ps);
    if( got == 0 ) {
      *chr = 0;
      n = 1;
      break;
    } else if( got == (size_t) -1 ) {
      *chr = 0xfffd;
      err = errno;
      break;
    } else if( got == (size_t) -2 ) {
      // continue the loop.
      err = qio_channel_read_amt(threadsafe, ch, &mbs[n], 1);
      if( err == EEOF ) err = ESHORT;
      if( err ) break;
      n++;
    } else {
      *chr = tmp_chr;
      n = got;
      break;
    }
  }

  // Now adjust the buffer to pass the number of characters we used.
  qio_channel_revert_unlocked(ch);
  qio_channel_advance_unlocked(ch, n-1);

unlock:
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

static inline
err_t qio_channel_write_char(const int threadsafe, qio_channel_t* ch, wchar_t chr) {
  char mbs[MB_LEN_MAX];
  mbstate_t ps;
  size_t got;

  memset(&ps, 0, sizeof(mbstate_t));
  got = wcrtomb(mbs, chr, &ps);
  if( got == (size_t) -1 ) return errno; // EILSEQ.

  return qio_channel_write_amt(threadsafe, ch, mbs, got);
}

static inline
err_t qio_channel_skip_past_newline(const int threadsafe, qio_channel_t* ch)
{
  wchar_t c;
  err_t err;
  while( 1 ) {
    err = qio_channel_read_char(threadsafe, ch, &c);
    if( err ) return err;
    if( c == '\n' ) break;
  }
  return 0;
}

static inline
err_t qio_channel_write_newline(const int threadsafe, qio_channel_t* ch)
{
  char c = '\n';
  return qio_channel_write_amt(threadsafe, ch, &c, 1);
}

err_t qio_channel_scan_string(const int threadsafe, qio_channel_t* ch, const char** out, ssize_t* len_out, ssize_t maxlen);

// returns 0 if it matched, or EFORMAT if it did not.
err_t qio_channel_scan_literal(const int threadsafe, qio_channel_t* ch, const char* match, ssize_t len, int skipws);

// Prints a string according to the style.
err_t qio_channel_print_string(const int threadsafe, qio_channel_t* ch, const char* ptr, ssize_t len);

// Prints a string as-is (this really just calls qio_channel_write_amt).
err_t qio_channel_print_literal(const int threadsafe, qio_channel_t* ch, const char* ptr, ssize_t len);

#endif


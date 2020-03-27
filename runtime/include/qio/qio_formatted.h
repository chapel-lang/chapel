/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _QIO_FORMATTED_H_
#define _QIO_FORMATTED_H_

#include "sys_basic.h"
#include "qio.h"

#include "bswap.h"
#include "error.h"

// true 1 false 0   __bool_true_false_are_defined
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h> // HUGE_VAL
#include <float.h> // DBL_MANT_DIG
#include <assert.h>

#include <wchar.h>
#define HAS_WCTYPE_H

#include "qio_style.h"
#include "encoding/encoding-support.h"

extern int qio_glocale_utf8; // for testing use.
#define QIO_GLOCALE_UTF8 1
#define QIO_GLOCALE_ASCII 2
#define QIO_GLOCALE_OTHER -1

void qio_set_glocale(void);

// Read/Write methods for Binary I/O

static inline
qioerr qio_channel_read_int8(const int threadsafe, qio_channel_t* restrict ch, int8_t* restrict ptr) {
  return qio_channel_read_amt(threadsafe, ch, ptr, 1);
}

static inline
qioerr qio_channel_write_int8(const int threadsafe, qio_channel_t* restrict ch, int8_t x) {
  return qio_channel_write_amt(threadsafe, ch, &x, 1);
}

static inline
qioerr qio_channel_read_uint8(const int threadsafe, qio_channel_t* restrict ch, uint8_t* restrict ptr) {
  return qio_channel_read_amt(threadsafe, ch, ptr, 1);
}

static inline
qioerr qio_channel_write_uint8(const int threadsafe, qio_channel_t* restrict ch, uint8_t x) {
  return qio_channel_write_amt(threadsafe, ch, &x, 1);
}



static inline
qioerr qio_channel_read_int16(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int16_t* restrict ptr) {
  qioerr err;
  int16_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 2);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be16toh(x);
  if( byteorder == QIO_LITTLE ) x = le16toh(x);
  *ptr = x;
  return 0;
}

static inline
qioerr qio_channel_write_int16(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int16_t x) {
  if( byteorder == QIO_BIG ) x = htobe16(x);
  if( byteorder == QIO_LITTLE ) x = htole16(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 2);
}

static inline
qioerr qio_channel_read_uint16(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint16_t* restrict ptr) {
  qioerr err;
  uint16_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 2);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be16toh(x);
  if( byteorder == QIO_LITTLE ) x = le16toh(x);
  *ptr = x;
  return 0;
}

static inline
qioerr qio_channel_write_uint16(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint16_t x) {
  if( byteorder == QIO_BIG ) x = htobe16(x);
  if( byteorder == QIO_LITTLE ) x = htole16(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 2);
}


static inline
qioerr qio_channel_read_int32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int32_t* restrict ptr) {
  qioerr err;
  int32_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 4);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be32toh(x);
  if( byteorder == QIO_LITTLE ) x = le32toh(x);
  *ptr = x;
  return 0;
}

static inline
qioerr qio_channel_write_int32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int32_t x) {
  if( byteorder == QIO_BIG ) x = htobe32(x);
  if( byteorder == QIO_LITTLE ) x = htole32(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 4);
}

static inline
qioerr qio_channel_read_uint32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint32_t* restrict ptr) {
  qioerr err;
  uint32_t x;
  err = qio_channel_read_amt(threadsafe, ch, &x, 4);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be32toh(x);
  if( byteorder == QIO_LITTLE ) x = le32toh(x);
  *ptr = x;
  return 0;
}

static inline
qioerr qio_channel_write_uint32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint32_t x) {
  if( byteorder == QIO_BIG ) x = htobe32(x);
  if( byteorder == QIO_LITTLE ) x = htole32(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 4);
}

static inline
qioerr qio_channel_read_int64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int64_t* restrict ptr) {
  int64_t x;
  qioerr err;
  err = qio_channel_read_amt(threadsafe, ch, &x, 8);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be64toh(x);
  if( byteorder == QIO_LITTLE ) x = le64toh(x);
  *ptr = x;
  return 0;
}

static inline
qioerr qio_channel_write_int64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, int64_t x) {
  if( byteorder == QIO_BIG ) x = htobe64(x);
  if( byteorder == QIO_LITTLE ) x = htole64(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 8);
}

static inline
qioerr qio_channel_read_uint64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint64_t* restrict ptr) {
  uint64_t x;
  qioerr err;
  err = qio_channel_read_amt(threadsafe, ch, &x, 8);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x = be64toh(x);
  if( byteorder == QIO_LITTLE ) x = le64toh(x);
  *ptr = x;
  return 0;
}

static inline
qioerr qio_channel_write_uint64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, uint64_t x) {
  if( byteorder == QIO_BIG ) x = htobe64(x);
  if( byteorder == QIO_LITTLE ) x = htole64(x);
  return qio_channel_write_amt(threadsafe, ch, &x, 8);
}

// Reading/writing varints in the same format as Google Protocol Buffers.
qioerr qio_channel_read_uvarint(const int threadsafe, qio_channel_t* restrict ch, uint64_t* restrict ptr);
qioerr qio_channel_read_svarint(const int threadsafe, qio_channel_t* restrict ch, int64_t* restrict ptr);
qioerr qio_channel_write_uvarint(const int threadsafe, qio_channel_t* restrict ch, uint64_t num);
qioerr qio_channel_write_svarint(const int threadsafe, qio_channel_t* restrict ch, int64_t num);


static inline
qioerr qio_channel_read_int(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, void* restrict ptr, size_t len, int issigned) {

  ssize_t signed_len = len;
  if( issigned ) signed_len = - signed_len;

  switch ( signed_len ) {
    case 1:
      return qio_channel_read_uint8(threadsafe, ch, (uint8_t*) ptr);
    case -1:
      return qio_channel_read_int8(threadsafe, ch, (int8_t*) ptr);
    case 2:
      return qio_channel_read_uint16(threadsafe, byteorder, ch,(uint16_t*)ptr);
    case -2:
      return qio_channel_read_int16(threadsafe, byteorder, ch, (int16_t*) ptr);
    case 4:
      return qio_channel_read_uint32(threadsafe, byteorder, ch,(uint32_t*)ptr);
    case -4:
      return qio_channel_read_int32(threadsafe, byteorder, ch, (int32_t*) ptr);
    case 8:
      return qio_channel_read_int64(threadsafe, byteorder, ch, (int64_t*) ptr);
    case -8:
      return qio_channel_read_uint64(threadsafe, byteorder, ch,(uint64_t*)ptr);
    default:
      QIO_RETURN_CONSTANT_ERROR(EINVAL, "bad integer size");
  }
}

static inline
qioerr qio_channel_write_int(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, const void* restrict ptr, size_t len, int issigned) {

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
      QIO_RETURN_CONSTANT_ERROR(EINVAL, "bad integer size");
  }
}

static inline
qioerr qio_channel_read_float32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, float* restrict ptr) {
  union {
   uint32_t i;
   float f;
  } x;
  qioerr err;

  err = qio_channel_read_amt(threadsafe, ch, &x, 4);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x.i = be32toh(x.i);
  if( byteorder == QIO_LITTLE ) x.i = le32toh(x.i);
  *ptr = x.f;
  return 0;
}

static inline
qioerr qio_channel_write_float32(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, float x) {
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
qioerr qio_channel_read_float64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, double* restrict ptr) {
  union {
   uint64_t i;
   double f;
  } x;
  qioerr err;

  err = qio_channel_read_amt(threadsafe, ch, &x, 8);
  if( err ) return err;
  if( byteorder == QIO_BIG ) x.i = be64toh(x.i);
  if( byteorder == QIO_LITTLE ) x.i = le64toh(x.i);
  *ptr = x.f;
  return 0;
}


static inline
qioerr qio_channel_write_float64(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, double x) {
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
qioerr qio_channel_read_float(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, void* restrict ptr, size_t len) {
  switch ( len ) {
    case 4:
      return qio_channel_read_float32(threadsafe, byteorder, ch, (float*) ptr);
    case 8:
      return qio_channel_read_float64(threadsafe, byteorder, ch, (double*) ptr);
    default:
      QIO_RETURN_CONSTANT_ERROR(EINVAL, "bad float size");
  }
}

static inline
qioerr qio_channel_write_float(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, const void* restrict ptr, size_t len) {
  switch ( len ) {
    case 4:
      return qio_channel_write_float32(threadsafe, byteorder, ch, *(const float*) ptr);
    case 8:
      return qio_channel_write_float64(threadsafe, byteorder, ch, *(const double*) ptr);
    default:
      QIO_RETURN_CONSTANT_ERROR(EINVAL, "bad float size");
  }
}

static inline
qioerr qio_channel_read_complex(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, void* restrict re_ptr, void* restrict im_ptr, size_t len) {
  qioerr err;
  switch ( len ) {
    case 4:
      err = qio_channel_read_float32(threadsafe, byteorder, ch, (float*) re_ptr);
      if( ! err ) {
        err = qio_channel_read_float32(threadsafe,byteorder,ch,(float*)im_ptr);
      }
      break;
    case 8:
      err = qio_channel_read_float64(threadsafe, byteorder, ch,(double*)re_ptr);
      if( ! err ) {
        err = qio_channel_read_float64(threadsafe,byteorder,ch,(double*)im_ptr);
      }
      break;
    default:
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "bad complex size");
  }
  return err;
}

static inline
qioerr qio_channel_write_complex(const int threadsafe, const int byteorder, qio_channel_t* restrict ch, const void* restrict re_ptr, const void* restrict im_ptr, size_t len) {
  qioerr err;
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
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "bad complex size");
  }
  return err;
}

#define QIO_BINARY_STRING_STYLE_LEN1B_DATA -1
#define QIO_BINARY_STRING_STYLE_LEN2B_DATA -2
#define QIO_BINARY_STRING_STYLE_LEN4B_DATA -4
#define QIO_BINARY_STRING_STYLE_LEN8B_DATA -8
#define QIO_BINARY_STRING_STYLE_LENvB_DATA -10
#define QIO_BINARY_STRING_STYLE_DATA_NULL -0x0100
#define QIO_BINARY_STRING_STYLE_TOEOF -0xff00

// string binary style:
// QIO_BINARY_STRING_STYLE_LEN1B_DATA -1 -- 1 byte of length before
// QIO_BINARY_STRING_STYLE_LEN2B_DATA -2 -- 2 bytes of length before
// QIO_BINARY_STRING_STYLE_LEN4B_DATA -4 -- 4 bytes of length before
// QIO_BINARY_STRING_STYLE_LEN8B_DATA -8 -- 8 bytes of length before
// QIO_BINARY_STRING_STYLE_LENvB_DATA -10 -- variable byte length before
//                                        (hi-bit 1 means more, little endian)
// QIO_BINARY_STRING_STYLE_TOEOF -0xff00 -- read until end or up to maxlen
// BINARY_STRING_STYLE_DATA_NULL|0xXX -0x01XX -- read until terminator XX
//  + -- nonzero positive -- read exactly this length.
qioerr qio_channel_read_string(const int threadsafe, const int byteorder, const int64_t str_style, qio_channel_t* restrict ch, const char* restrict * restrict out, int64_t* restrict len_out, ssize_t maxlen_bytes);

// string binary style:
// QIO_BINARY_STRING_STYLE_LEN1B_DATA -1 -- 1 byte of length before
// QIO_BINARY_STRING_STYLE_LEN2B_DATA -2 -- 2 bytes of length before
// QIO_BINARY_STRING_STYLE_LEN4B_DATA -4 -- 4 bytes of length before
// QIO_BINARY_STRING_STYLE_LEN8B_DATA -8 -- 8 bytes of length before
// QIO_BINARY_STRING_STYLE_LENvB_DATA -10 -- variable byte length before
//                                        (hi-bit 1 means more, little endian)
// QIO_BINARY_STRING_STYLE_TOEOF -0xff00 -- read until end or up to maxlen
// BINARY_STRING_STYLE_DATA_NULL|0xXX -0x01XX -- read until terminator XX
//  + -- nonzero positive -- read exactly this length.
qioerr qio_channel_write_string(const int threadsafe, const int byteorder, const int64_t str_style, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len);


qioerr qio_channel_scan_bool(const int threadsafe, qio_channel_t* restrict ch, uint8_t* restrict out);
qioerr qio_channel_print_bool(const int threadsafe, qio_channel_t* restrict ch, uint8_t num);

qioerr qio_channel_scan_int(const int threadsafe, qio_channel_t* restrict ch, void* restrict out, size_t len, int issigned);
qioerr qio_channel_scan_float(const int threadsafe, qio_channel_t* restrict ch, void* restrict out, size_t len);
qioerr qio_channel_scan_imag(const int threadsafe, qio_channel_t* restrict ch, void* restrict out, size_t len);
qioerr qio_channel_print_int(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, size_t len, int issigned);
qioerr qio_channel_print_float(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, size_t len);
qioerr qio_channel_print_imag(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, size_t len);

qioerr qio_channel_scan_complex(const int threadsafe, qio_channel_t* restrict ch, void* restrict re_out, void* restrict im_out, size_t len);
qioerr qio_channel_print_complex(const int threadsafe, qio_channel_t* restrict ch, const void* restrict re_ptr, const void* im_ptr, size_t len);

// These methods read or write UTF-8 characters (codepoints).


qioerr _qio_channel_read_char_slow_unlocked(qio_channel_t* restrict ch, int32_t* restrict chr);

static inline
qioerr qio_channel_read_char(const int threadsafe, qio_channel_t* restrict ch, int32_t* restrict chr) {
  qioerr err;
  uint32_t codepoint=0, state;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = 0;

  // TODO: This part can be refactored to use some of the functions in
  // encoding/encoding-support.h

  // Fast path: an entire multi-byte sequence
  // is stored in the buffers.
  if( qio_glocale_utf8 > 0 &&
      qio_space_in_ptr_diff(4, ch->cached_end, ch->cached_cur) ) {
    if( qio_glocale_utf8 == QIO_GLOCALE_UTF8 ) {
      state = 0;
      while( 1 ) {
        chpl_enc_utf8_decode(&state,
                             &codepoint,
                             *(unsigned char*)ch->cached_cur);
        ch->cached_cur = qio_ptr_add(ch->cached_cur,1);
        if (state <= 1) {
          break;
        }
      }
      if( state == UTF8_ACCEPT ) {
        *chr = codepoint;
      } else {
        *chr = 0xfffd; // replacement character
        QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
      }
    } else if( qio_glocale_utf8 == QIO_GLOCALE_ASCII ) {
      // character == byte.
      *chr = *(unsigned char*)ch->cached_cur;
      ch->cached_cur = qio_ptr_add(ch->cached_cur,1);
      err = 0;
    }
  } else {
    err = _qio_channel_read_char_slow_unlocked(ch, chr);
  }

 //unlock:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

// Return the number of bytes used in encoding chr,
// or 0 if it's an invalid character.
static inline
int qio_nbytes_char(int32_t chr)
{
  if( qio_glocale_utf8 > 0 ) {
    if( qio_glocale_utf8 == QIO_GLOCALE_UTF8 ) {
      if( chr < 0 ) {
        return 0;
      } else if( chr < 0x80 ) {
        // OK, we got a 1-byte character; case #1
        return 1;
      } else if( chr < 0x800 ) {
        // OK, we got a fits-in-2-bytes character; case #2
        return 2;
      } else if( chr < 0x10000 ) {
        // OK, we got a fits-in-3-bytes character; case #3
        return 3;
      } else {
        // OK, we got a fits-in-4-bytes character; case #4
        return 4;
      }
    } else if( qio_glocale_utf8 == QIO_GLOCALE_ASCII ) {
      return 1;
    }
  } else {
#ifdef HAS_WCTYPE_H
    char buf[MB_CUR_MAX];
    mbstate_t ps;
    size_t got;
    memset(&ps, 0, sizeof(mbstate_t));
    // The buf argument is never used, but if we put NULL there,
    // wcrtomb ignores chr and assumes L'\0' per the C standard.
    got = wcrtomb(buf, chr, &ps);
    if( got == (size_t) -1 ) {
      return 0;
    } else {
      return got;
    }
#else
    return 0;
#endif
  }
  return 0;
}
// dst is a pointer to a buffer containing room for the encoded characters
// (use qio_nbytes_char to find out the size).
static inline
qioerr qio_encode_char_buf(char* dst, int32_t chr)
{
  qioerr err = 0;
  if( qio_glocale_utf8 > 0 ) {
    if( qio_glocale_utf8 == QIO_GLOCALE_UTF8 ) {
      if( chr < 0 ) {
        QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
      } else if( chr < 0x80 ) {
        // OK, we got a 1-byte character; case #1
        *(unsigned char*)dst = (unsigned char) chr;
      } else if( chr < 0x800 ) {
        // OK, we got a fits-in-2-bytes character; case #2
        *(unsigned char*)dst = (0xc0 | (chr >> 6));
        *(((unsigned char*)dst)+1) = (0x80 | (chr & 0x3f));
      } else if( chr < 0x10000 ) {
        // OK, we got a fits-in-3-bytes character; case #3
        *(unsigned char*)dst = (0xe0 | (chr >> 12));
        *(((unsigned char*)dst)+1) = (0x80 | ((chr >> 6) & 0x3f));
        *(((unsigned char*)dst)+2) = (0x80 | (chr & 0x3f));
      } else {
        // OK, we got a fits-in-4-bytes character; case #4
        *(unsigned char*)dst = (0xf0 | (chr >> 18));
        *(((unsigned char*)dst)+1) = (0x80 | ((chr >> 12) & 0x3f));
        *(((unsigned char*)dst)+2) = (0x80 | ((chr >> 6) & 0x3f));
        *(((unsigned char*)dst)+3) = (0x80 | (chr & 0x3f));
      }
    } else if( qio_glocale_utf8 == QIO_GLOCALE_ASCII ) {
      *(unsigned char*)dst = (unsigned char) chr;
    }
  } else {
#ifdef HAS_WCTYPE_H
    mbstate_t ps;
    size_t got;
    memset(&ps, 0, sizeof(mbstate_t));
    got = wcrtomb(dst, chr, &ps);
    if( got == (size_t) -1 ) {
      QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
    }
#else
    QIO_GET_CONSTANT_ERROR(err, ENOSYS, "missing wctype.h");
#endif
  }
  return err;
}

// Returns NULL if it's an illegal character OR we're out of memory; otherwise,
// a newly-allocated ASCIIZ string containing the result.
c_string qio_encode_to_string(int32_t chr);

static inline
qioerr do_qio_decode_char_buf(int32_t* restrict chr, int* restrict nbytes,
                              const char* buf, ssize_t buflen, bool allow_escape)
{
  // Fast path: an entire multi-byte sequence
  // is stored in the buffers.
  if( qio_glocale_utf8 > 0 ) {
    if( qio_glocale_utf8 == QIO_GLOCALE_UTF8 ) {
      const int ret = chpl_enc_decode_char_buf_utf8(chr, nbytes, buf, buflen,
                                                    allow_escape);
      if (ret == 0) {
        return 0;
      }
      else {
        QIO_RETURN_CONSTANT_ERROR(EILSEQ, "");
      }
    } else if( qio_glocale_utf8 == QIO_GLOCALE_ASCII ) {
      const int ret = chpl_enc_decode_char_buf_ascii(chr, nbytes, buf, buflen);
      if (ret == 0) {
        return 0;
      }
      else {
        QIO_RETURN_CONSTANT_ERROR(EILSEQ, "");
      }
    }
  } else {
    const int ret = chpl_enc_decode_char_buf_wctype(chr, nbytes, buf, buflen);
    if (ret == 0) {
      return 0;
    }
    else if (ret == -1) {
      QIO_RETURN_CONSTANT_ERROR(EILSEQ, "");
    }
    else {  // -2
      QIO_RETURN_CONSTANT_ERROR(ENOSYS, "missing wctype.h");
    }
  }
  *chr = 0; // Makes GCC stop complaining about *chr not being set in other locations
  chpl_internal_error("qio_decode_char_buf");
  QIO_RETURN_CONSTANT_ERROR(EILSEQ, ""); // this should never be reached.
}

static inline
qioerr qio_decode_char_buf(int32_t* restrict chr, int* restrict nbytes,
                           const char* buf, ssize_t buflen)
{
  return do_qio_decode_char_buf(chr, nbytes, buf, buflen, false);
}

// this version allows escaped nonUTF8 data
static inline
qioerr qio_decode_char_buf_esc(int32_t* restrict chr, int* restrict nbytes,
                               const char* buf, ssize_t buflen)
{
  return do_qio_decode_char_buf(chr, nbytes, buf, buflen, true);

}

qioerr _qio_channel_write_char_slow_unlocked(qio_channel_t* restrict ch, int32_t chr);

static inline
qioerr qio_channel_write_char(const int threadsafe, qio_channel_t* restrict ch, int32_t chr)
{
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = 0;

  if( qio_glocale_utf8 > 0 &&
      qio_space_in_ptr_diff(4, ch->cached_end, ch->cached_cur) ) {
    if( qio_glocale_utf8 == QIO_GLOCALE_UTF8 ) {
      if( chr < 0 ) {
        QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
      } else if( chr < 0x80 ) {
        // OK, we got a 1-byte character; case #1
        *(unsigned char*)ch->cached_cur = (unsigned char) chr;
        ch->cached_cur = qio_ptr_add(ch->cached_cur,1);
      } else if( chr < 0x800 ) {
        // OK, we got a fits-in-2-bytes character; case #2
        *(unsigned char*)ch->cached_cur = (0xc0 | (chr >> 6));
        *(((unsigned char*)ch->cached_cur)+1) = (0x80 | (chr & 0x3f));
        ch->cached_cur = qio_ptr_add(ch->cached_cur,2);
      } else if( chr < 0x10000 ) {
        // OK, we got a fits-in-3-bytes character; case #3
        *(unsigned char*)ch->cached_cur = (0xe0 | (chr >> 12));
        *(((unsigned char*)ch->cached_cur)+1) = (0x80 | ((chr >> 6) & 0x3f));
        *(((unsigned char*)ch->cached_cur)+2) = (0x80 | (chr & 0x3f));
        ch->cached_cur = qio_ptr_add(ch->cached_cur,3);
      } else {
        // OK, we got a fits-in-4-bytes character; case #4
        *(unsigned char*)ch->cached_cur = (0xf0 | (chr >> 18));
        *(((unsigned char*)ch->cached_cur)+1) = (0x80 | ((chr >> 12) & 0x3f));
        *(((unsigned char*)ch->cached_cur)+2) = (0x80 | ((chr >> 6) & 0x3f));
        *(((unsigned char*)ch->cached_cur)+3) = (0x80 | (chr & 0x3f));
        ch->cached_cur = qio_ptr_add(ch->cached_cur,4);
      }
    } else if( qio_glocale_utf8 == QIO_GLOCALE_ASCII ) {
      *(unsigned char*)ch->cached_cur = (unsigned char) chr;
      ch->cached_cur = qio_ptr_add(ch->cached_cur,1);
    }
  } else {
    err = _qio_channel_write_char_slow_unlocked(ch, chr);
  }

//unlock:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

static inline
int qio_unicode_supported(void) {
  return qio_glocale_utf8 == QIO_GLOCALE_UTF8;
}

qioerr qio_channel_skip_past_newline(const int threadsafe, qio_channel_t* restrict ch, int skipOnlyWs);

qioerr qio_channel_write_newline(const int threadsafe, qio_channel_t* restrict ch);

qioerr qio_channel_scan_string(const int threadsafe, qio_channel_t* restrict ch, const char* restrict * restrict out, int64_t* restrict len_out, ssize_t maxlen_bytes);

qioerr qio_channel_scan_bytes(const int threadsafe, qio_channel_t* restrict ch, const char* restrict * restrict out, int64_t* restrict len_out, ssize_t maxlen_bytes);

// reads match exactly - skipping whitespace before it if skipwsbefore is set.
// returns 0 if it matched, or EFORMAT if it did not.
qioerr qio_channel_scan_literal(const int threadsafe, qio_channel_t* restrict ch, const char* restrict match, ssize_t len, int skipwsbefore);
// Chapel needs another name for the same routine.
qioerr qio_channel_scan_literal_2(const int threadsafe, qio_channel_t* ch, /* const char* */ void* match, ssize_t len, int skipwsbefore);

typedef struct qio_truncate_info_ {
  ssize_t max_columns;
  ssize_t max_chars;
  ssize_t max_bytes;
  ssize_t ret_columns; 
  ssize_t ret_chars; 
  ssize_t ret_bytes; 
  ssize_t ret_truncated_at_byte;
  ssize_t ret_truncated;
} qio_truncate_info_t;

// Quote a string according to a style (we have this one for some error
// situations in which it's undesirable to use the stdout channel
// because of e.g. Chapel module initialization order)
qioerr qio_quote_string(uint8_t string_start, uint8_t string_end, uint8_t string_format, const char* restrict ptr, ssize_t len, const char** out, qio_truncate_info_t* ti);
// like qio_quote_string, but only get length information.
qioerr qio_quote_string_length(uint8_t string_start, uint8_t string_end, uint8_t string_format, const char* restrict ptr, ssize_t len, qio_truncate_info_t* ti);

qioerr qio_quote_bytes_length(uint8_t string_start, uint8_t string_end, uint8_t string_format, const char* restrict ptr, ssize_t len, qio_truncate_info_t* ti);

// Prints a bytes according to the style.
qioerr qio_channel_print_bytes(const int threadsafe, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len);
// Prints a string according to the style.
qioerr qio_channel_print_string(const int threadsafe, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len);

// Prints a string as-is (this really just calls qio_channel_write_amt).
qioerr qio_channel_print_literal(const int threadsafe, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len);
qioerr qio_channel_print_literal_2(const int threadsafe, qio_channel_t* ch, /*const char* */ void* ptr, ssize_t len);


int32_t qio_skip_json_object_unlocked(qio_channel_t* restrict ch);
int32_t qio_skip_json_array_unlocked(qio_channel_t* restrict ch);
int32_t qio_skip_json_value_unlocked(qio_channel_t* restrict ch);
int32_t qio_skip_json_string_unlocked(qio_channel_t* restrict ch);
int32_t qio_skip_json_field_unlocked(qio_channel_t* restrict ch);

qioerr qio_channel_skip_json_field(const int threadsafe, qio_channel_t* ch);

enum {
  QIO_CONV_UNK = 0,
  QIO_CONV_ARG_TYPE_NUMERIC,
  QIO_CONV_ARG_TYPE_SIGNED,
  QIO_CONV_ARG_TYPE_BINARY_SIGNED,
  QIO_CONV_ARG_TYPE_UNSIGNED,
  QIO_CONV_ARG_TYPE_BINARY_UNSIGNED,
  QIO_CONV_ARG_TYPE_REAL,
  QIO_CONV_ARG_TYPE_BINARY_REAL,
  QIO_CONV_ARG_TYPE_IMAG,
  QIO_CONV_ARG_TYPE_BINARY_IMAG,
  QIO_CONV_ARG_TYPE_COMPLEX,
  QIO_CONV_ARG_TYPE_BINARY_COMPLEX,
  QIO_CONV_ARG_TYPE_CHAR,
  QIO_CONV_ARG_TYPE_STRING,
  QIO_CONV_ARG_TYPE_BINARY_STRING,
  QIO_CONV_ARG_TYPE_REPR,
  QIO_CONV_ARG_TYPE_REGEXP, // argument contains a regexp
  QIO_CONV_ARG_TYPE_NONE_REGEXP_LITERAL, // literal regexp in string
  QIO_CONV_ARG_TYPE_NONE_LITERAL,
  QIO_CONV_SET_MIN_WIDTH_COLS,
  QIO_CONV_SET_MAX_WIDTH_COLS,
  QIO_CONV_SET_MAX_WIDTH_CHARS,
  QIO_CONV_SET_MAX_WIDTH_BYTES,
  QIO_CONV_SET_STRINGLEN,
  QIO_CONV_SET_PRECISION,
  QIO_CONV_SET_TERMINATOR,
  QIO_CONV_SET_STRINGSTART,
  QIO_CONV_SET_STRINGSTARTEND,
  QIO_CONV_SET_STRINGEND,
  QIO_CONV_SET_IGNORE,
  QIO_CONV_SET_CAPTURE, // These are used internally by the code calling qio_conv_parse
  QIO_CONV_SET_DONE,
};

// Conversion specifier for printf/scanf
typedef struct qio_conv_s {
  uint8_t preArg1; // could be min width
  uint8_t preArg2; // could be precision or max str length
  uint8_t preArg3; // could be string start/end character or terminator byte.
  uint8_t argType;

  uint8_t literal_is_whitespace; // = 1 for ' ', = 2 for \n
  uint32_t literal_length;
  /*const char*/ void* literal; // pointer into format string...
  uint32_t regexp_length;
  /*const char*/ void* regexp;
  uint32_t regexp_flags_length;
  /*const char*/ void* regexp_flags;
} qio_conv_t;


void qio_conv_destroy(qio_conv_t* spec);
void qio_conv_init(qio_conv_t* spec_out);
qioerr qio_conv_parse(c_string fmt, size_t start, uint64_t* end_out, int scanning, qio_conv_t* spec_out, qio_style_t* style_out);

// These error codes can be used by callers to qio_conv_parse
qioerr qio_format_error_too_many_args(void);
qioerr qio_format_error_too_few_args(void);
qioerr qio_format_error_arg_mismatch(int64_t arg);
qioerr qio_format_error_bad_regexp(void);
qioerr qio_format_error_write_regexp(void);

#endif


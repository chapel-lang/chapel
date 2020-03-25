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


#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include "qio_formatted.h"

#include <limits.h>
#include <ctype.h>

#ifdef HAS_WCTYPE_H
#include <wctype.h>
#include <langinfo.h>
#endif

// 0 means not set
// 1 means use faster, hard-coded UTF-8 decode/encoder
// -1 means use C multibyte functions (e.g. mbtowc)
// -2 means use C locale (ie 1 byte per character)
int qio_glocale_utf8 = 0;

void qio_set_glocale(void) {
#ifdef HAS_WCTYPE_H
  char* codeset = nl_langinfo(CODESET);

  if( 0 == strcmp(codeset, "UTF-8") ) {
    qio_glocale_utf8 = QIO_GLOCALE_UTF8;
  } else if( 0 == strcmp(codeset, "ANSI_X3.4-1968") || // what Linux calls it
             0 == strcmp(codeset, "US-ASCII") ) { // what Mac OS X calls it
    qio_glocale_utf8 = QIO_GLOCALE_ASCII;
  } else {
    qio_glocale_utf8 = QIO_GLOCALE_OTHER;
  }
#else
  qio_glocale_utf8 = QIO_GLOCALE_ASCII;
#endif
}

#ifndef HAS_WCTYPE_H
static int towlower(int wc) { return tolower(wc); }
static int iswprint(int wc) { return isprint(wc); }
static int iswspace(int wc) { return isspace(wc); }
static int wcwidth(int wc) {
  if( isprint(wc) ) return 1;
  else return -1;
}
#endif

qioerr qio_channel_read_uvarint(const int threadsafe, qio_channel_t* restrict ch, uint64_t* restrict ptr) {
  qioerr err = 0;
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

  if( !err && (byte & 0x80) )
    QIO_GET_CONSTANT_ERROR(err, EFORMAT, "overflow in varint");

error:
  *ptr = num;
  _qio_channel_set_error_unlocked(ch, err);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

qioerr qio_channel_read_svarint(const int threadsafe, qio_channel_t* restrict ch, int64_t* restrict ptr) {
  qioerr err;
  uint64_t u_num;
  err = qio_channel_read_uvarint(threadsafe, ch, &u_num);
  *ptr = (u_num >> 1) ^ -((int64_t)(u_num & 1));
  return err;
}
qioerr qio_channel_write_uvarint(const int threadsafe, qio_channel_t* restrict ch, uint64_t num) {
  qioerr err = 0;
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
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

qioerr qio_channel_write_svarint(const int threadsafe, qio_channel_t* restrict ch, int64_t num) {
  uint64_t u_num = (num << 1) ^ (num >> 63);
  return qio_channel_write_uvarint(threadsafe, ch, u_num);
}



static
qioerr _peek_until_byte(qio_channel_t* restrict ch, uint8_t term_byte, int64_t* restrict amt_read_out, int* restrict found_term_out)
{
  qioerr err;
  int64_t mark_offset = 0;
  int64_t end_offset = 0;
  uint64_t num = 0;
  uint8_t byte = 0;
  int found_term;

  mark_offset = qio_channel_offset_unlocked(ch);

  err = qio_channel_mark(false, ch);
  if( err ) return err;

  while( 1 ) {
    err = qio_channel_read_uint8(false, ch, &byte);
    if( err ) break;
    if( byte == term_byte ) break;
  }

  if( byte == term_byte ) found_term = 1;
  else found_term = 0;

  end_offset = qio_channel_offset_unlocked(ch);

  qio_channel_revert_unlocked(ch);

  num = end_offset - mark_offset - found_term;

  *amt_read_out = num;
  *found_term_out = found_term;
  return err;
}

static
qioerr _peek_until_len(qio_channel_t* restrict ch, ssize_t len, int64_t* restrict amt_read_out)
{
  qioerr err;
  int64_t mark_offset = 0;
  int64_t end_offset = 0;
  uint64_t num = 0;
  uint8_t byte = 0;
  ssize_t count;

  mark_offset = qio_channel_offset_unlocked(ch);

  err = qio_channel_mark(false, ch);
  if( err ) return err;

  for( count = 0; count < len; count++ ) {
    err = qio_channel_read_uint8(false, ch, &byte);
    if( err ) break;
  }

  end_offset = qio_channel_offset_unlocked(ch);

  qio_channel_revert_unlocked(ch);

  num = end_offset - mark_offset;

  *amt_read_out = num;
  return err;
}


static
qioerr _getc_after_whitespace(qio_channel_t* restrict ch, int32_t* restrict got_chr)
{
  qioerr err = 0;
  int32_t chr = 0;

  while( 1 ) {
    err = qio_channel_read_char(false, ch, &chr);
    if( ! iswspace(chr) ) break;
    if( err ) break;
  }

  *got_chr = chr;
  return err;
}

/*
static inline
qioerr _peek_until_char(qio_channel_t* ch, int32_t term_chr, int64_t* amt_read_out, int* found_term_out)
{
  qioerr err, newerr;
  int64_t mark_offset = 0;
  int64_t end_offset = 0;
  uint64_t num = 0;
  int32_t chr = 0;
  int found_term;

  err = qio_channel_mark(false, ch);
  if( err ) goto error;

  err = qio_channel_offset(false, ch, &mark_offset);
  if( err ) goto error;

  while( 1 ) {
    err = qio_channel_read_char(false, ch, &chr);
    if( err ) break;
    if( chr == term_chr ) break;
  }

  if( chr == term_chr ) found_term = 1;
  else found_term = 0;

  newerr = qio_channel_offset(false, ch, &end_offset);
  if( ! err ) err = newerr;

  newerr = qio_channel_revert(false, ch);
  if( ! err ) err = newerr;

  num = end_offset - start_offset - found_term;

  *amt_read_out = num;
  *found_term_out = found_term;
  return err;
}*/

// always appends room for a NULL byte at the end.
static
qioerr _append_byte(char* restrict * restrict buf, size_t* restrict buf_len, size_t* restrict buf_max, int8_t byte)
{
  char* buf_in = *buf;
  size_t len_in = *buf_len;
  size_t max_in = *buf_max;
  char* newbuf;
  size_t newsz;
  size_t need;

  need = len_in + 2; // one for the byte the other for the null character
  if( need < len_in || need > (SSIZE_MAX-1) ) {
    // Too big.
    QIO_RETURN_CONSTANT_ERROR(EOVERFLOW, "");
  }
  // First, make sure that there is room.
  if( need >= max_in ) {
    // Reallocate buffer.
    newsz = 2 * max_in;
    if( newsz < 16  ) newsz = 16;
    if( newsz < need  ) newsz = need;
    newbuf = qio_realloc(buf_in, newsz);
    if( ! newbuf ) return QIO_ENOMEM;
    buf_in = newbuf;
    max_in = newsz;
  }

  // Now store it in the buffer.
  buf_in[len_in] = byte;
  len_in += 1;

  *buf = buf_in;
  *buf_len = len_in;
  *buf_max = max_in;

  return 0;
}

// always appends room for a NULL byte at the end.
static
qioerr _append_char(char* restrict * restrict buf, size_t* restrict buf_len, size_t* restrict buf_max, int32_t chr)
{
  char* buf_in = *buf;
  size_t len_in = *buf_len;
  size_t max_in = *buf_max;
  char* newbuf;
  size_t newsz;
  size_t need;
  size_t chbytes;

  // Encode the character in UTF-8...
  // (or whatever the locale requires)
  // How many bytes is our character?
  chbytes = qio_nbytes_char(chr);

  need = len_in + chbytes + 1;
  if( need < len_in || need > (SSIZE_MAX-1) ) {
    // Too big.
    QIO_RETURN_CONSTANT_ERROR(EOVERFLOW, "");
  }
  // First, make sure that there is room.
  if( need >= max_in ) {
    // Reallocate buffer.
    newsz = 2 * max_in;
    if( newsz < 16  ) newsz = 16;
    if( newsz < need  ) newsz = need;
    newbuf = qio_realloc(buf_in, newsz);
    if( ! newbuf ) return QIO_ENOMEM;
    buf_in = newbuf;
    max_in = newsz;
  }

  // Now store it in the buffer.
  qio_encode_char_buf(&buf_in[len_in], chr);
  len_in += chbytes;

  *buf = buf_in;
  *buf_len = len_in;
  *buf_max = max_in;

  return 0;
}

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
qioerr qio_channel_read_string(const int threadsafe, const int byteorder, const int64_t str_style, qio_channel_t* restrict ch, const char* restrict* restrict out, int64_t* restrict len_out, ssize_t maxlen)
{
  qioerr err;
  uint8_t term = 0;
  uint8_t num8 = 0;
  uint16_t num16 = 0;
  uint32_t num32 = 0;
  uint64_t num = 0;
  int64_t peek_amt = 0;
  char* restrict ret = NULL;
  int found_term=0;
  ssize_t len=0;
  ssize_t amt = 0;
  err_t errcode;

  if( maxlen <= 0 ) maxlen = SSIZE_MAX - 1;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;


  // read a string length.
  switch (str_style) {
    case QIO_BINARY_STRING_STYLE_LEN1B_DATA:
      err = qio_channel_read_uint8(false, ch, &num8);
      num = num8;
      break;
    case QIO_BINARY_STRING_STYLE_LEN2B_DATA:
      err = qio_channel_read_uint16(false, byteorder, ch, &num16);
      num = num16;
      break;
    case QIO_BINARY_STRING_STYLE_LEN4B_DATA:
      err = qio_channel_read_uint32(false, byteorder, ch, &num32);
      num = num32;
      break;
    case QIO_BINARY_STRING_STYLE_LEN8B_DATA:
      err = qio_channel_read_uint64(false, byteorder, ch, &num);
      break;
    case QIO_BINARY_STRING_STYLE_LENvB_DATA:
      err = qio_channel_read_uvarint(false, ch, &num);
      break;
    case QIO_BINARY_STRING_STYLE_TOEOF:
      // read until the end of the file.
      // Figure out how many bytes are available.
      err = _peek_until_len(ch, maxlen, &peek_amt);
      num = peek_amt;
      // Ignore EOF errors as long as we read something.
      if( err && qio_err_to_int(err) == EEOF && num > 0 ) err = 0;
      break;
    default:
      if( str_style >= 0 ) {
        // just read the suggested length
        num = str_style;
      } else {
        // read a terminated amount.
        term = (-str_style) + QIO_STRSTYLE_NULL_TERMINATED;
        // What is the position we're marking?
        err = _peek_until_byte(ch, term, &peek_amt, &found_term);
        num = peek_amt;
        if( !err && !found_term )
          QIO_GET_CONSTANT_ERROR(err, EFORMAT, "missing string terminator");
      }
  }
  if( err ) goto rewind;

  if( num > (SSIZE_MAX-1) ) {
    QIO_GET_CONSTANT_ERROR(err, EOVERFLOW, "");
    goto rewind;
  }
  if( num > maxlen ) {
    QIO_GET_CONSTANT_ERROR(err, EOVERFLOW, "");
    goto rewind;
  }
  len = num;

 
  // Now read that many bytes into an allocated area.
  ret = qio_malloc(len + 1); // room for \0.
  if( ! ret ) {
    err = QIO_ENOMEM;
    goto rewind;
  }

  ret[0] = '\0'; // start with terminator in case we don't read anything.
  err = qio_channel_read(false, ch, ret, num, &amt);
  ret[len] = '\0'; // always add terminator at the end
  if( err ) goto rewind;
  if( amt != len ) {
    err = QIO_ESHORT;
    // zero out the rest of it...
    memset(ret + amt, 0, len - amt);
    goto rewind;
  }

  if( found_term ) {
    // Read the end-of-string character.
    err = qio_channel_read_amt(false, ch, &num8, 1);
    if( err ) goto rewind;
  }

  err = 0;
rewind:
  if( err ) {
    qio_channel_revert_unlocked(ch);
  } else {
    qio_channel_commit_unlocked(ch);
  }
unlock:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  errcode = qio_err_to_int(err);
  if( errcode && errcode != EEOF && errcode != ESHORT ) qio_free(ret);
  else {
    // don't modify out if we didn't read anything.
    if( ret ) {
      *out = ret;
      *len_out = amt;
    }
  }

  return err;
}

// allocates and returns a string.
qioerr qio_channel_scan_string(const int threadsafe, qio_channel_t* restrict ch, const char* restrict * restrict out, int64_t* restrict len_out, ssize_t maxlen_bytes)
{
  qioerr err;
  char* restrict ret = NULL;
  size_t ret_len = 0;
  size_t ret_max = 0;
  int32_t term_chr;
  int32_t chr;
  char tmp[4*MB_LEN_MAX + 1]; // room for XXXX in \uXXXX
  int32_t tmpchr;
  int tmpi;
  int handle_back;
  int handle_0x;
  int handle_u;
  int stop_space;
  unsigned long conv, conv2;
  qio_style_t* style;
  ssize_t nread = 0;
  int64_t mark_offset;
  int64_t end_offset;
  ssize_t maxlen_chars = SSIZE_MAX - 1;
  int found_term = 0;

  if( maxlen_bytes <= 0 ) maxlen_bytes = SSIZE_MAX - 1;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  style = &ch->style;

  if( style->max_width_characters < UINT32_MAX &&
      style->max_width_characters < maxlen_chars ) {
    maxlen_chars = style->max_width_characters;
  }
  if( style->max_width_bytes < UINT32_MAX &&
      style->max_width_bytes < maxlen_bytes ) {
    maxlen_bytes = style->max_width_bytes;
  }

  // Allocate room for our buffer...
  err = _append_char(&ret, &ret_len, &ret_max, 0);
  if( err ) goto unlock;
  ret_len = 0;

  mark_offset = qio_channel_offset_unlocked(ch);

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  term_chr = style->string_end;
  if( style->string_format == QIO_STRING_FORMAT_WORD ) {
    handle_back = 0;
    handle_0x = 0;
    handle_u = 0;
    stop_space = 1;
  } else if( style->string_format == QIO_STRING_FORMAT_BASIC ) {
    handle_back = 1;
    handle_0x = 0;
    handle_u = 0;
    stop_space = 0;
  } else if( style->string_format == QIO_STRING_FORMAT_CHPL ) {
    handle_back = 1;
    handle_0x = 1;
    handle_u = 0;
    stop_space = 0;
  } else if( style->string_format == QIO_STRING_FORMAT_JSON ) {
    handle_back = 1;
    handle_0x = 0;
    handle_u = 1;
    stop_space = 0;
  } else if( style->string_format == QIO_STRING_FORMAT_TOEND ) {
    handle_back = 0;
    handle_0x = 0;
    handle_u = 0;
    stop_space = 0;
  } else if( style->string_format == QIO_STRING_FORMAT_TOEOF ) {
    handle_back = 0;
    handle_0x = 0;
    handle_u = 0;
    stop_space = 0;
    term_chr = -1;
  } else {
    handle_back = 1;
    handle_0x = 1;
    handle_u = 0;
    stop_space = 0;
  }

  err = 0;
  for( nread = 0;
      // limit # characters
      nread < maxlen_chars &&
      // stop on error
      !err &&
      // limit # bytes
      qio_channel_offset_unlocked(ch) - mark_offset < maxlen_bytes;
      nread++ ) {
    err = qio_channel_read_char(false, ch, &chr);
    if( err ) break;

    // If we're using FORMAT_WORD, skip any whitespace at the beginning
    if( nread == 0 ) {
      while( !(style->string_format == QIO_STRING_FORMAT_TOEND ||
               style->string_format == QIO_STRING_FORMAT_TOEOF) &&
             iswspace(chr) ) {
        // Read the next character!
        err = qio_channel_read_char(false, ch, &chr);
        if( err ) break;
      }
      if( err ) break;

      if( style->string_format == QIO_STRING_FORMAT_WORD ||
          style->string_format == QIO_STRING_FORMAT_TOEND ||
          style->string_format == QIO_STRING_FORMAT_TOEOF ) {
        // OK, use the character we have
      } else if( chr == style->string_start ) {
        // Read the next character!
        err = qio_channel_read_char(false, ch, &chr);
        if( err ) break;
      } else {
        // Format error.
        QIO_GET_CONSTANT_ERROR(err, EFORMAT, "missing string start");
        break;
      }
    }

    // if it's a \ we'll probably do something special.
    if( handle_back && chr == '\\' ) {
      // Get the next character.
      err = qio_channel_read_char(false, ch, &chr);
      if( err ) break;

      if( handle_0x && chr == 'x' ) {
        // handle \x20
        tmpi = 0;
        err = qio_channel_read_char(false, ch, &tmpchr);
        if( err ) break;
        err = qio_encode_char_buf(&tmp[tmpi], tmpchr);
        if( err ) break;
        tmpi += qio_nbytes_char(tmpchr);

        err = qio_channel_read_char(false, ch, &tmpchr);
        if( err ) break;
        err = qio_encode_char_buf(&tmp[tmpi], tmpchr);
        if( err ) break;
        tmpi += qio_nbytes_char(tmpchr);

        tmp[tmpi] = '\0';

        errno = 0;
        conv = strtol(tmp, NULL, 16);
        if( (conv == ULONG_MAX || conv == 0) && errno ) {
          err = qio_mkerror_errno();
          break;
        }

        err = _append_char(&ret, &ret_len, &ret_max, conv);
      } else if( handle_u && chr == 'u' ) {
        int z;
        // handle \uABCD
        tmpi = 0;

        for( z = 0; z < 4; z++ ) { 
          err = qio_channel_read_char(false, ch, &tmpchr);
          if( err ) break;
          err = qio_encode_char_buf(&tmp[tmpi], tmpchr);
          if( err ) break;
          tmpi += qio_nbytes_char(tmpchr);
        }
        if( err ) break;

        tmp[tmpi] = '\0';

        errno = 0;
        conv = strtol(tmp, NULL, 16);
        if( (conv == ULONG_MAX || conv == 0) && errno ) {
          err = qio_mkerror_errno();
          break;
        }

        // It might be a surrogate pair....
        if( 0xD800 <= conv && conv <= 0xDBFF ) {
          // We *must* read another \uXXXX, 
          //  and it *must* be a trail surrogate (ie in 0xDC00..0xDFFF)
          // Read \ character
          err = qio_channel_read_char(false, ch, &chr);
          if( err ) break;
          if( chr != '\\' ) {
            QIO_GET_CONSTANT_ERROR(err, EILSEQ, "expected surrogate pair");
            break;
          }
          // Read u character
          err = qio_channel_read_char(false, ch, &chr);
          if( err ) break;
          if( chr != 'u' ) {
            QIO_GET_CONSTANT_ERROR(err, EILSEQ, "expected surrogate pair");
            break;
          }
          tmpi = 0;
          // Now read XXXX
          for( z = 0; z < 4; z++ ) { 
            err = qio_channel_read_char(false, ch, &tmpchr);
            if( err ) break;
            err = qio_encode_char_buf(&tmp[tmpi], tmpchr);
            if( err ) break;
            tmpi += qio_nbytes_char(tmpchr);
          }
          if( err ) break;

          tmp[tmpi] = '\0';
          errno = 0;
          conv2 = strtol(tmp, NULL, 16);
          if( (conv2 == ULONG_MAX || conv2 == 0) && errno ) {
            err = qio_mkerror_errno();
            break;
          }

          // conv2 *must* be in the range 0xDC00..0xDFFF
          if( 0xDC00 <= conv2 && conv2 <= 0xDFFF ) {
            // a surrogate pair, in conv and conv2.
            // Update conv to be the right decoded character.
            conv = ((conv - 0xD800) << 10) | (conv2 - 0xDC00);
          } else {
            // Format error.
            QIO_GET_CONSTANT_ERROR(err, EILSEQ, "bad surrogate pair");
            break;
          }
        }
        err = _append_char(&ret, &ret_len, &ret_max, conv);
      } else {
        // a backslash'd character.
        if( chr == 'a' ) chr = '\a';
        else if( chr == 'b' ) chr = '\b';
        else if( chr == 't' ) chr = '\t';
        else if( chr == 'n' ) chr = '\n';
        else if( chr == 'v' ) chr = '\v';
        else if( chr == 'f' ) chr = '\f';
        else if( chr == 'r' ) chr = '\r';
        err = _append_char(&ret, &ret_len, &ret_max, chr);
      }
    } else if((stop_space && iswspace(chr)) || ((!stop_space) && (chr == term_chr )) ) {
      if(style->string_format == QIO_STRING_FORMAT_TOEND) {
        err = _append_char(&ret, &ret_len, &ret_max, chr);
      }
      found_term = 1;
      break;
    } else {
      err = _append_char(&ret, &ret_len, &ret_max, chr);
    }
  }

  // Add the NULL... space for this is allocated in _append_char.
  ret[ret_len] = '\0';

  end_offset = qio_channel_offset_unlocked(ch);

  if(style->string_format == QIO_STRING_FORMAT_WORD) {

    // Unget the terminating character - there must
    // be one (or else err!=0)
    if( err == 0 && found_term ) {
      // Unget the terminating character.
      qio_channel_revert_unlocked(ch);
      qio_channel_advance_unlocked(ch, end_offset - mark_offset - 1);
      goto unlock;
    }

  }

  // Now we'll ignore EOF for some styles.
  if(style->string_format == QIO_STRING_FORMAT_WORD ||
     style->string_format == QIO_STRING_FORMAT_TOEND ||
     style->string_format == QIO_STRING_FORMAT_TOEOF) {
    // Not an error to reach EOF with these ones.
    if( ret_len > 0 && qio_err_to_int(err) == EEOF ) err = 0;
  }

  if( err ) {
    qio_channel_revert_unlocked(ch);
  } else {
    qio_channel_commit_unlocked(ch);
  }
unlock:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  if( err ) qio_free(ret);
  else {
    if( ret ) {
      *out = ret;
      *len_out = ret_len;
    }
  }

  return err;
}

// allocates and returns a string without checking for valid encoding
qioerr qio_channel_scan_bytes(const int threadsafe, qio_channel_t* restrict ch, const char* restrict * restrict out, int64_t* restrict len_out, ssize_t maxlen_bytes)
{
  qioerr err;
  char* restrict ret = NULL;
  size_t ret_len = 0;
  size_t ret_max = 0;
  int32_t term_chr;
  int32_t chr;
  char tmp[4*MB_LEN_MAX + 1]; // room for XXXX in \uXXXX
  int32_t tmpchr;
  int handle_back;
  int handle_0x;
  int stop_space;
  unsigned long conv;
  qio_style_t* style;
  ssize_t nread = 0;
  int64_t mark_offset;
  int64_t end_offset;
  ssize_t maxlen_chars = SSIZE_MAX - 1;
  int found_term = 0;

  if( maxlen_bytes <= 0 ) maxlen_bytes = SSIZE_MAX - 1;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  style = &ch->style;

  if( style->max_width_characters < UINT32_MAX &&
      style->max_width_characters < maxlen_chars ) {
    maxlen_chars = style->max_width_characters;
  }
  if( style->max_width_bytes < UINT32_MAX &&
      style->max_width_bytes < maxlen_bytes ) {
    maxlen_bytes = style->max_width_bytes;
  }

  // Allocate room for our buffer...
  err = _append_byte(&ret, &ret_len, &ret_max, 0);
  if( err ) goto unlock;
  ret_len = 0;

  mark_offset = qio_channel_offset_unlocked(ch);

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  term_chr = style->string_end;
  if( style->string_format == QIO_STRING_FORMAT_WORD ) {
    handle_back = 0;
    handle_0x = 0;
    stop_space = 1;
  } else if( style->string_format == QIO_STRING_FORMAT_BASIC ) {
    handle_back = 1;
    handle_0x = 0;
    stop_space = 0;
  } else if( style->string_format == QIO_STRING_FORMAT_CHPL ) {
    handle_back = 1;
    handle_0x = 1;
    stop_space = 0;
  } else if( style->string_format == QIO_STRING_FORMAT_JSON ) {
    handle_back = 1;
    handle_0x = 0;
    stop_space = 0;
  } else if( style->string_format == QIO_STRING_FORMAT_TOEND ) {
    handle_back = 0;
    handle_0x = 0;
    stop_space = 0;
  } else if( style->string_format == QIO_STRING_FORMAT_TOEOF ) {
    handle_back = 0;
    handle_0x = 0;
    stop_space = 0;
    term_chr = -1;
  } else {
    handle_back = 1;
    handle_0x = 1;
    stop_space = 0;
  }

  err = 0;
  for( nread = 0;
      // limit # characters
      nread < maxlen_chars &&
      // stop on error
      !err &&
      // limit # bytes
      qio_channel_offset_unlocked(ch) - mark_offset < maxlen_bytes;
      nread++ ) {
    chr = qio_channel_read_byte(false, ch);
    if(chr < 0) {
      err = qio_int_to_err(-chr);
      break;
    }

    // If we're using FORMAT_WORD, skip any whitespace at the beginning
    if( nread == 0 ) {
      while( !(style->string_format == QIO_STRING_FORMAT_TOEND ||
               style->string_format == QIO_STRING_FORMAT_TOEOF) &&
             iswspace(chr) ) {
        // Read the next byte!
        chr = qio_channel_read_byte(false, ch);
        if(chr < 0) {
          err = qio_int_to_err(-chr);
          break;
        }
      }
      if( err ) break;

      if( style->string_format == QIO_STRING_FORMAT_WORD ||
          style->string_format == QIO_STRING_FORMAT_TOEND ||
          style->string_format == QIO_STRING_FORMAT_TOEOF ) {
        // OK, use the byte we have
      } else if( style->bytes_prefix != 0 &&
                 chr == style->bytes_prefix ) {
        // style->bytes_prefix is set and the character is a match:
        // Read the next byte hoping that it is string_start
        chr = qio_channel_read_byte(false, ch);
        if(chr < 0) {
          err = qio_int_to_err(-chr);
          break;
        }
        if (chr == style->string_start) {
          // Read the next byte
          chr = qio_channel_read_byte(false, ch);
          if(chr < 0) {
            err = qio_int_to_err(-chr);
            break;
          }
        } else {
          QIO_GET_CONSTANT_ERROR(err, EFORMAT, "missing bytes start");
          break;
        }
      } else if ( style->bytes_prefix == 0 &&
                  chr == style->string_start) {
        // we are OK with reading "some bytes" into a bytes object as long as
        // the bytes_prefix is set to 0.
        // so, go ahead and read the next byte
        chr = qio_channel_read_byte(false, ch);
        if(chr < 0) {
          err = qio_int_to_err(-chr);
          break;
        }
      } else {
        // Format error.
        QIO_GET_CONSTANT_ERROR(err, EFORMAT, "missing bytes start");
        break;
      }
    }

    // if it's a \ we'll probably do something special.
    if( handle_back && chr == '\\' ) {
      // Get the next byte.
      chr = qio_channel_read_byte(false, ch);
      if(chr < 0) {
        err = qio_int_to_err(-chr);
        break;
      }

      if( handle_0x && chr == 'x' ) {
        tmpchr = qio_channel_read_byte(false, ch);
        if(tmpchr < 0) {
          err = qio_int_to_err(-tmpchr);
          break;
        }
        tmp[0] = tmpchr;
        tmpchr = qio_channel_read_byte(false, ch);
        if(tmpchr < 0) {
          err = qio_int_to_err(-tmpchr);
          break;
        }
        tmp[1] = tmpchr;
        tmp[2] = '\0';

        errno = 0;
        conv = strtol(tmp, NULL, 16);
        if( (conv == ULONG_MAX || conv == 0) && errno ) {
          err = qio_mkerror_errno();
          break;
        }

        err = _append_byte(&ret, &ret_len, &ret_max, conv);
      } else {
        // a backslash'd character.
        if( chr == 'a' ) chr = '\a';
        else if( chr == 'b' ) chr = '\b';
        else if( chr == 't' ) chr = '\t';
        else if( chr == 'n' ) chr = '\n';
        else if( chr == 'v' ) chr = '\v';
        else if( chr == 'f' ) chr = '\f';
        else if( chr == 'r' ) chr = '\r';
        err = _append_byte(&ret, &ret_len, &ret_max, chr);
      }
    } else if((stop_space && iswspace(chr)) || ((!stop_space) && (chr == term_chr )) ) {
      if(style->string_format == QIO_STRING_FORMAT_TOEND) {
        err = _append_byte(&ret, &ret_len, &ret_max, chr);
      }
      found_term = 1;
      break;
    } else {
      err = _append_byte(&ret, &ret_len, &ret_max, chr);
    }
  }

  // Add the NULL... space for this is allocated in _append_byte.
  ret[ret_len] = '\0';

  end_offset = qio_channel_offset_unlocked(ch);

  if(style->string_format == QIO_STRING_FORMAT_WORD) {

    // Unget the terminating character - there must
    // be one (or else err!=0)
    if( err == 0 && found_term ) {
      // Unget the terminating character.
      qio_channel_revert_unlocked(ch);
      qio_channel_advance_unlocked(ch, end_offset - mark_offset - 1);
      goto unlock;
    }

  }

  // Now we'll ignore EOF for some styles.
  if(style->string_format == QIO_STRING_FORMAT_WORD ||
     style->string_format == QIO_STRING_FORMAT_TOEND ||
     style->string_format == QIO_STRING_FORMAT_TOEOF) {
    // Not an error to reach EOF with these ones.
    if( ret_len > 0 && qio_err_to_int(err) == EEOF ) err = 0;
  }

  if( err ) {
    qio_channel_revert_unlocked(ch);
  } else {
    qio_channel_commit_unlocked(ch);
  }
unlock:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  if( err ) qio_free(ret);
  else {
    if( ret ) {
      *out = ret;
      *len_out = ret_len;
    }
  }

  return err;
}

qioerr qio_channel_scan_literal(const int threadsafe, qio_channel_t* restrict ch, const char* restrict match, ssize_t len, int skipwsbefore)
{
  qioerr err;
  int32_t wchr = -1;
  char chr = -1;
  ssize_t nread = 0;
  int64_t lastwspos = 0;

  if( skipwsbefore && len > 0 ) {
    int nbytes = 0;
    int32_t wchr;
    size_t min_nonspace = len;
    size_t max_nonspace = 0;
    // If we're skipping whitespace,
    // ignore leading or trailing whitespace
    // in the pattern.
    for( ; nread < len; nread += nbytes ) {
      err = qio_decode_char_buf(&wchr, &nbytes, &match[nread], len - nread);
      if( err ) return err;
      if( ! iswspace(wchr) ) {
        if( nread < min_nonspace ) min_nonspace = nread;
        if( nread > max_nonspace ) max_nonspace = nread;
      }
    }

    if( min_nonspace > max_nonspace ) {
      nread = 0;
      len = 0;
    } else {
      nread = 0;
      if( skipwsbefore ) nread = min_nonspace;
      //if( skipwsafter ) len = max_nonspace + 1;
    }
  }

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  if( skipwsbefore ) {
    err = qio_channel_mark(false, ch);
    if( err ) goto revert;

    while( 1 ) {
      lastwspos = qio_channel_offset_unlocked(ch);
      err = qio_channel_read_char(false, ch, &wchr);
      if( err ) break;
      if( ! iswspace(wchr) ) break;
    }

    // ignore EOF when looking for whitespace.
    // ignore EILSEQ (illegal unicode sequence) when
    // looking for whitespace (that just means it wasn't whitespace)
    if( qio_err_to_int(err) == EEOF ||
        qio_err_to_int(err) == EILSEQ ) {
      err = 0;
      qio_channel_clear_error(ch);
    }

    qio_channel_revert_unlocked(ch);

    if( ! err ) {
      // We've exited the loop because the last
      // one we read wasn't whitespace, so seek
      // back to lastwspos. 
      qio_channel_advance_unlocked(ch, lastwspos - qio_channel_offset_unlocked(ch));
    }
  }

  if( err == 0 ) {
    for( ; nread < len; nread++ ) {
      err = qio_channel_read_amt(false, ch, &chr, 1);
      if( err ) break;
      if( chr != match[nread]) break;
    }
  }

  if( err == 0 ) {
    if( nread == len ) {
      // we matched the whole thing!
      err = 0;
    } else {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "missing expected literal");
    }
  }

  /*
  if( skipwsafter && !err && len > 0 ) {
    // skip whitespace after the pattern.
    // but only bother if there was a pattern at all...
    err = qio_channel_mark(false, ch);
    if( err ) goto revert;

    while( 1 ) {
      lastwspos = qio_channel_offset_unlocked(ch);
      err = qio_channel_read_char(false, ch, &wchr);
      if( err ) break;
      if( ! iswspace(wchr) ) break;
    }

    // ignore EOF when looking for whitespace.
    if( qio_err_to_int(err) == EEOF ) err = 0;
    // ignore EILSEQ (illegal unicode sequence) when
    // looking for whitespace (that just means it wasn't whitespace)
    if( qio_err_to_int(err) == EILSEQ ) err = 0;

    qio_channel_revert_unlocked(ch);

    if( ! err ) {
      // We've exited the loop because the last
      // one we read wasn't whitespace, so seek
      // back to lastwspos. 
      qio_channel_advance_unlocked(ch, lastwspos - qio_channel_offset_unlocked(ch));
    }
  }*/

revert:
  if( err ) {
    qio_channel_revert_unlocked(ch);
  } else {
    qio_channel_commit_unlocked(ch);
  }
  // Don't set error indicator on EFORMAT because
  // that's probably a temporary error.
  if( qio_err_to_int(err) != EFORMAT ) _qio_channel_set_error_unlocked(ch, err);
unlock:
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

qioerr qio_channel_scan_literal_2(const int threadsafe, qio_channel_t* ch, void* match, ssize_t len, int skipwsbefore)
{
  return qio_channel_scan_literal(threadsafe, ch, (const char*) match, len, skipwsbefore);
}


qioerr qio_channel_print_literal(const int threadsafe, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len)
{
  return qio_channel_write_amt(threadsafe, ch, ptr, len);
}

qioerr qio_channel_print_literal_2(const int threadsafe, qio_channel_t* ch, void* ptr, ssize_t len)
{
  return qio_channel_write_amt(threadsafe, ch, ptr, len);
}


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
qioerr qio_channel_write_string(const int threadsafe, const int byteorder, const int64_t str_style, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len)
{
  qioerr err;
  uint8_t num8 = 0;
  uint16_t num16 = 0;
  uint32_t num32 = 0;
  int64_t num64 = 0;
  int use_term = 0;
  uint8_t term = 0;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  // write a string length if necessary.
  switch (str_style) {
    case QIO_BINARY_STRING_STYLE_LEN1B_DATA:
      num8 = len;
      if( (ssize_t) num8 != len ) {
        QIO_GET_CONSTANT_ERROR(err, EOVERFLOW, "overflow in string length");
      } else err = qio_channel_write_uint8(false, ch, num8);
      break;
    case QIO_BINARY_STRING_STYLE_LEN2B_DATA:
      num16 = len;
      if( (ssize_t) num16 != len ) {
        QIO_GET_CONSTANT_ERROR(err, EOVERFLOW, "overflow in string length");
      } else err = qio_channel_write_uint16(false, byteorder, ch, num16);
      break;
    case QIO_BINARY_STRING_STYLE_LEN4B_DATA:
      num32 = len;
      if( (ssize_t) num32 != len ) {
        QIO_GET_CONSTANT_ERROR(err, EOVERFLOW, "overflow in string length");
      } else err = qio_channel_write_uint32(false, byteorder, ch, num32);
      break;
    case QIO_BINARY_STRING_STYLE_LEN8B_DATA:
      num64 = len;
      if( (ssize_t) num64 != len ) {
        QIO_GET_CONSTANT_ERROR(err, EOVERFLOW, "overflow in string length");
      } else err = qio_channel_write_uint64(false, byteorder, ch, num64);
      break;
    case QIO_BINARY_STRING_STYLE_LENvB_DATA:
      num64 = len;
      if( (ssize_t) num64 != len ) {
        QIO_GET_CONSTANT_ERROR(err, EOVERFLOW, "overflow in string length");
      } else err = qio_channel_write_uvarint(false, ch, num64);
      break;
    case QIO_BINARY_STRING_STYLE_TOEOF:
      // Just don't worry about the length - write len bytes.
      break;
    default:
      if( str_style >= 0 ) {
        // MPF - perhaps we should allow writing a string
        // of a different length; we could zero-pad after,
        // and truncate it if it's too long?

        // Verify that the length matches.
        if( len != str_style ) {
          QIO_GET_CONSTANT_ERROR(err, EOVERFLOW, "unexpected string length");
        } else err = 0;
      } else {
        // We're going to write a terminating character.
        use_term = 1;
        term = (-str_style) + QIO_STRSTYLE_NULL_TERMINATED;
        err = 0;
      }
  }

  if( err ) goto rewind;

  // write the string itself
  if (len > 0) {
    err = qio_channel_write_amt(false, ch, ptr, len);
  }
  if( err ) goto rewind;

  // write the terminator if necessary.
  if( use_term ) {
    err = qio_channel_write_amt(false, ch, &term, 1);
    if( err ) goto rewind;
  }

  err = 0;
rewind:
  if( err ) {
    qio_channel_revert_unlocked(ch);
  } else {
    qio_channel_commit_unlocked(ch);
  }
unlock:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

#if MB_LEN_MAX < 6
#define MB_LEN_MAX_OR_6 6
#else
#define MB_LEN_MAX_OR_6 MB_LEN_MAX
#endif

#define JSON_ESC_MAX (2*MB_LEN_MAX_OR_6+1)

static inline
char _qio_tohex(unsigned char i)
{
  if( i < 10 ) return '0' + i;
  else return 'A' + i - 10;
}

static
int _qio_byte_escape(uint8_t b, int32_t string_end, int string_format, char* tmp, int *width_chars_out, int *width_cols_out)
{
  char tmpchr;
  int cwidth;
  if( b == string_end || b == '\\' ||
      b == '\'' || b == '"' || b == '\n' ) {
    if(tmp) {
      tmp[0] = '\\';
      if( b == '\n' ) {
        tmpchr = 'n';
      } else {
        tmpchr = b;
      }
      tmp[1] = tmpchr;
    }
    cwidth = 2;
  } else if( b >= 128 || !isprint(b) ) {
    if(tmp) {
      tmp[0] = '\\';
      tmp[1] = 'x';
      tmp[2] = _qio_tohex((b >> 4) & 0xf);
      tmp[3] = _qio_tohex((b >> 0) & 0xf);
    }
    cwidth = 4;
  } else {
    if(tmp) {
      tmp[0] = b;
    }
    cwidth = 1;
  }

  if( width_chars_out ) *width_chars_out = cwidth;
  if( width_cols_out ) *width_cols_out = cwidth;
  return cwidth;
}
// Returns \" or \x00 or \uXXXX etc depending on string style
// returns negative or 0 on error.
// Or returns number of bytes of UTF-8 characters printed in tmp
//  and can return number of characters/columns printed
// tmp must have room for JSON_ESC_MAX
static
int _qio_chr_escape(int32_t chr, int32_t string_end, int string_format, char* tmp, int *width_chars_out, int *width_cols_out)
{
  int32_t tmpchr;
  int i = 0;
  int clen = 0;
  qioerr err;
  int width_chars = 0;
  int width_cols = 0;
  int cwidth;
 
#define WRITEC(c) { \
    width_chars++; \
    if( width_cols_out ) { \
      cwidth = wcwidth(c); \
      if( cwidth < 0 ) { \
        cwidth = 0; \
      } \
      width_cols += cwidth; \
    } \
    clen = qio_nbytes_char(c); \
    if( tmp ) { \
      err = qio_encode_char_buf(&tmp[i], c); \
      if( err ) goto error; \
    } \
    i += clen; \
  }

  switch(string_format) {
    case QIO_STRING_FORMAT_BASIC:
      if( chr == string_end || chr == '\\' ) {
        WRITEC('\\');
        WRITEC(chr);
      } else {
        WRITEC(chr);
      }
      break;
    case QIO_STRING_FORMAT_CHPL:
      if( chr == string_end || chr == '\\' ||
          chr == '\'' || chr == '"' || chr == '\n' ) {
        WRITEC('\\');
        if( chr == '\n' ) {
          tmpchr = 'n';
        } else {
          tmpchr = chr;
        }
        WRITEC(tmpchr);
      } else if( !iswprint(chr) ) {
        char buf[JSON_ESC_MAX];
        int buflen;
        int j;
        int x;
        // convert each of the bytes into \x00 escaped things.
        buflen = qio_nbytes_char(chr);
        err = qio_encode_char_buf(buf, chr);
        if( err ) goto error;

        // Now, print out each byte escaped.
        for( j = 0; j < buflen; j++ ) {
          x = buf[j];
          WRITEC('\\');
          WRITEC('x');
          WRITEC(_qio_tohex((x >> 4) & 0xf));
          WRITEC(_qio_tohex((x >> 0) & 0xf));
        }
      } else {
        WRITEC(chr);
      }
      break;
    case QIO_STRING_FORMAT_JSON:
      if( chr == string_end || chr == '\\' ||
          chr == '"' || chr == '\n' ) {
        WRITEC('\\');
        if( chr == '\n' ) {
          tmpchr = 'n';
        } else {
          tmpchr = chr;
        }
        WRITEC(tmpchr);
      } else if( !iswprint(chr) ) {
        // write it as \uXXXX 4 hex digits.
        // If it is a codepoint >= 0x10000, it needs to be
        // encoded as a surrogate pair in \u....
        if( chr < 0x10000 ) {
          WRITEC('\\');
          WRITEC('u');
          WRITEC(_qio_tohex((chr >> 12) & 0xf));
          WRITEC(_qio_tohex((chr >>  8) & 0xf));
          WRITEC(_qio_tohex((chr >>  4) & 0xf));
          WRITEC(_qio_tohex((chr >>  0) & 0xf));
        } else {
          unsigned int code = (chr - 0x10000) & 0xFFFFF;
          unsigned int lead = (0xD800 + (code >> 10)) & 0xFFFF; 
          unsigned int trail = (0xDC00 + (code & 0x3FF)) & 0xFFFF;
          WRITEC('\\');
          WRITEC('u');
          WRITEC(_qio_tohex((lead >> 12) & 0xf));
          WRITEC(_qio_tohex((lead >>  8) & 0xf));
          WRITEC(_qio_tohex((lead >>  4) & 0xf));
          WRITEC(_qio_tohex((lead >>  0) & 0xf));
          WRITEC('\\');
          WRITEC('u');
          WRITEC(_qio_tohex((trail >> 12) & 0xf));
          WRITEC(_qio_tohex((trail >>  8) & 0xf));
          WRITEC(_qio_tohex((trail >>  4) & 0xf));
          WRITEC(_qio_tohex((trail >>  0) & 0xf));
        }
      } else {
        WRITEC(chr);
      }
      break;
    default:
      WRITEC(chr);
  }

  if( width_chars_out ) *width_chars_out = width_chars;
  if( width_cols_out ) *width_cols_out = width_cols;
  return i;

error:
  if( width_chars_out ) *width_chars_out = -1;
  if( width_cols_out ) *width_cols_out = -1;
  return -1;

#undef WRITEC
}

inline
qioerr qio_channel_print_string(const int threadsafe, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len)
{
  qioerr err;
  ssize_t i;
  int clen = 1;
  int32_t chr;
  qio_style_t* style;
  char tmp[JSON_ESC_MAX];
  int tmplen;
  ssize_t width = 0;
  qio_truncate_info_t ti;
  ssize_t use_len;
  int overfull = 0;
  ti.max_columns = SSIZE_MAX;
  ti.max_chars = SSIZE_MAX;
  ti.max_bytes = SSIZE_MAX;
  ti.ret_bytes = -1;

  if( !ptr || len == 0 ) {
    // hilde sez: Having a distinguished value for empty strings is
    // undesirable.
    ptr = "";
    len = 0;
  }

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  style = &ch->style;

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  if( style->max_width_columns != UINT32_MAX )
    ti.max_columns = style->max_width_columns;
  if( style->max_width_characters != UINT32_MAX )
    ti.max_chars = style->max_width_characters;

  // Compute the size of the string, in case we have to
  // do padding.
  if( style->min_width_columns > 0 ||
      style->max_width_columns != UINT32_MAX ||
      style->max_width_characters != UINT32_MAX ) {

    err = qio_quote_string_length(style->string_start, style->string_end, style->string_format, ptr, len, &ti);
    if( err ) goto rewind;

    use_len = ti.ret_truncated_at_byte;
    overfull = ti.ret_truncated;
    len = use_len;
    if( ti.ret_columns >= 0 ) width = ti.ret_columns;
    else if( ti.ret_chars >= 0 ) width = ti.ret_chars;
    else width = ti.ret_bytes;
    if( width < 0 ) width = 0;

    if( style->min_width_columns > 0 ) {
      if( !style->leftjustify && width < style->min_width_columns ) {
        // Put what we need to for getting to the min_width.
        for( i = 0; i < style->min_width_columns - width; i++ ) {
          err = qio_channel_write_char(false, ch, style->pad_char);
          if( err ) goto rewind;
        }
      }
    }
  }


  // write the string itself, possible with some escape-handling.
  if( style->string_format == QIO_STRING_FORMAT_WORD ||
      style->string_format == QIO_STRING_FORMAT_TOEND ) {
    // Do not interpret the string in any way... just write it.
    if( ti.ret_bytes != -1 ) len = ti.ret_bytes;
    err = qio_channel_write_amt(false, ch, ptr, len);
    if( err ) goto rewind;
  } else {
    // Write string_start.
    err = qio_channel_write_char(false, ch, style->string_start);
    if( err ) goto rewind;

    // Write the string while translating it.
    for( i = 0; i < len; i+=clen ) {
      err = qio_decode_char_buf(&chr, &clen, ptr + i, len - i);
      if( err ) goto rewind;

      tmplen = _qio_chr_escape(chr, style->string_end, style->string_format, tmp, NULL, NULL);
      if( tmplen < 0 ) {
        QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
        goto rewind;
      }

      err = qio_channel_write_amt(false, ch, tmp, tmplen);
      if( err ) goto rewind;
    }

    // Write string_end.
    err = qio_channel_write_char(false, ch, style->string_end);
    if( err ) goto rewind;
    if( overfull ) {
      err = qio_channel_write_char(false, ch, '.');
      if( err ) goto rewind;
      err = qio_channel_write_char(false, ch, '.');
      if( err ) goto rewind;
      err = qio_channel_write_char(false, ch, '.');
      if( err ) goto rewind;
    }
  }

  if( style->min_width_columns > 0 ) {
    if( style->leftjustify && width < style->min_width_columns ) {
      // Put what we need to for getting to the min_width.
      for( i = 0; i < style->min_width_columns - width; i++ ) {
        err = qio_channel_write_char(false, ch, style->pad_char);
        if( err ) goto rewind;
      }
    }
  }

  err = 0;
rewind:
  if( err ) {
    qio_channel_revert_unlocked(ch);
  } else {
    qio_channel_commit_unlocked(ch);
  }

unlock:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

inline
qioerr qio_channel_print_bytes(const int threadsafe, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len)
{
  qioerr err;
  ssize_t i;
  int clen = 1;
  qio_style_t* style;
  char tmp[JSON_ESC_MAX];
  int tmplen;
  ssize_t width = 0;
  qio_truncate_info_t ti;
  ssize_t use_len;
  int overfull = 0;
  ti.max_columns = SSIZE_MAX;
  ti.max_chars = SSIZE_MAX;
  ti.max_bytes = SSIZE_MAX;
  ti.ret_bytes = -1;

  if( !ptr || len == 0 ) {
    // hilde sez: Having a distinguished value for empty strings is
    // undesirable.
    ptr = "";
    len = 0;
  }

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  style = &ch->style;

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  if( style->max_width_columns != UINT32_MAX )
    ti.max_columns = style->max_width_columns;
  if( style->max_width_characters != UINT32_MAX )
    ti.max_chars = style->max_width_characters;

  // Compute the size of the string, in case we have to
  // do padding.
  if( style->min_width_columns > 0 ||
      style->max_width_columns != UINT32_MAX ||
      style->max_width_characters != UINT32_MAX ) {

    err = qio_quote_bytes_length(style->string_start, style->string_end, style->string_format, ptr, len, &ti);
    if( err ) goto rewind;

    use_len = ti.ret_truncated_at_byte;
    overfull = ti.ret_truncated;
    len = use_len;
    if( ti.ret_columns >= 0 ) width = ti.ret_columns;
    else if( ti.ret_chars >= 0 ) width = ti.ret_chars;
    else width = ti.ret_bytes;
    if( width < 0 ) width = 0;

    if( style->min_width_columns > 0 ) {
      if( !style->leftjustify && width < style->min_width_columns ) {
        // Put what we need to for getting to the min_width.
        for( i = 0; i < style->min_width_columns - width; i++ ) {
          err = qio_channel_write_byte(false, ch, style->pad_char);
          if( err ) goto rewind;
        }
      }
    }
  }


  // write the string itself, possible with some escape-handling.
  if( style->string_format == QIO_STRING_FORMAT_WORD ||
      style->string_format == QIO_STRING_FORMAT_TOEND ) {
    // Do not interpret the string in any way... just write it.
    if( ti.ret_bytes != -1 ) len = ti.ret_bytes;
    err = qio_channel_write_amt(false, ch, ptr, len);
    if( err ) goto rewind;
  } else {
    // Write b.
    err = qio_channel_write_byte(false, ch, style->bytes_prefix);
    if( err ) goto rewind;
    // Write string_start.
    err = qio_channel_write_byte(false, ch, style->string_start);
    if( err ) goto rewind;
    for( i = 0; i < len; i+=clen ) {
      tmplen = _qio_byte_escape(ptr[i], style->string_end, style->string_format, tmp, NULL, NULL);
      if( tmplen < 0 ) {
        QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
        goto rewind;
      }

      err = qio_channel_write_amt(false, ch, tmp, tmplen);
      if( err ) goto rewind;
    }
    // Write string_end.
    err = qio_channel_write_byte(false, ch, style->string_end);
    if( err ) goto rewind;
    if( overfull ) {
      err = qio_channel_write_byte(false, ch, '.');
      if( err ) goto rewind;
      err = qio_channel_write_byte(false, ch, '.');
      if( err ) goto rewind;
      err = qio_channel_write_byte(false, ch, '.');
      if( err ) goto rewind;
    }
  }

  if( style->min_width_columns > 0 ) {
    if( style->leftjustify && width < style->min_width_columns ) {
      // Put what we need to for getting to the min_width.
      for( i = 0; i < style->min_width_columns - width; i++ ) {
        err = qio_channel_write_byte(false, ch, style->pad_char);
        if( err ) goto rewind;
      }
    }
  }

  err = 0;
rewind:
  if( err ) {
    qio_channel_revert_unlocked(ch);
  } else {
    qio_channel_commit_unlocked(ch);
  }

unlock:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

// Returns length information for how we would quote ptr
// without actually saving it anywhere. 
qioerr qio_quote_bytes_length(uint8_t string_start, uint8_t string_end, uint8_t string_format, const char* restrict ptr, ssize_t len, qio_truncate_info_t* ti)
{
  ssize_t i; // how far along the input are we (ie ptr[i])
  ssize_t quoted_bytes = 0; // how many bytes of output?
  ssize_t quoted_chars = 0; // how many chars of output?
  ssize_t quoted_cols = 0; // how many columns of output?
  ssize_t safe_i = 0;
  ssize_t safe_quoted_bytes = 0;
  ssize_t safe_quoted_chars = 0;
  ssize_t safe_quoted_cols = 0;
  int ellipses_size;
  int end_quote_size;
  int start_quote_size;
  int clen = 1;
  ssize_t max_cols = (ti)?(ti->max_columns):(SSIZE_MAX);
  ssize_t max_chars = (ti)?(ti->max_chars):(SSIZE_MAX);
  ssize_t max_bytes = (ti)?(ti->max_bytes):(SSIZE_MAX);
  int overfull = 0;
  int tmplen, tmpchars, tmpcols;

  // write the string itself, possible with some escape-handling.
  if( string_format == QIO_STRING_FORMAT_WORD ||
      string_format == QIO_STRING_FORMAT_TOEND ) {
    ellipses_size = 0;
    end_quote_size = 0;
    start_quote_size = 0;
  } else {
    ellipses_size = 3; // ie ...
    end_quote_size = 1; // ie a double quote
    start_quote_size = 2; // ie b"
    // Smallest pattern is b""...
    if( max_cols < 6 ) max_cols = 6;
    if( max_chars < 6 ) max_chars = 6;
    if( max_bytes < 6 ) max_bytes = 6;
  }

  // Account for the starting quote.
  quoted_bytes += start_quote_size;
  quoted_chars += start_quote_size;
  quoted_cols += start_quote_size;

  // we need to compute the number of characters
  // and the total number of columns.
  for( i = 0; i < len; i+=clen ) {
    tmplen = _qio_byte_escape(ptr[i], string_end, string_format, NULL,
                              &tmpchars, &tmpcols);
    if( quoted_bytes + ellipses_size + end_quote_size <= max_bytes &&
        quoted_chars + ellipses_size + end_quote_size <= max_chars &&
        quoted_cols + ellipses_size + end_quote_size <= max_cols) {
      safe_i = i;
      safe_quoted_bytes = quoted_bytes;
      safe_quoted_chars = quoted_chars;
      safe_quoted_cols = quoted_cols;
    }
    quoted_bytes += tmplen;
    quoted_chars += tmpchars;
    quoted_cols += tmpcols;
    if( quoted_bytes + end_quote_size > max_bytes ||
        quoted_chars + end_quote_size > max_chars ||
        quoted_cols + end_quote_size > max_cols ) {
      // We have over-filled.
      overfull = 1;
      break;
    }
  }

  if( overfull ) {
    i = safe_i;
    // and add end quote and ellipses.
    quoted_bytes = safe_quoted_bytes;
    quoted_chars = safe_quoted_chars;
    quoted_cols = safe_quoted_cols;
  }

  // Account for the end quote
  quoted_bytes += end_quote_size;
  quoted_chars += end_quote_size;
  quoted_cols += end_quote_size;

  if( overfull ) {
    // Account for the ellipses
    quoted_bytes += ellipses_size;
    quoted_chars += ellipses_size;
    quoted_cols += ellipses_size;
  }

  if( ti ) {
    ti->ret_columns = quoted_cols;
    ti->ret_chars = quoted_chars;
    ti->ret_bytes = quoted_bytes;
    ti->ret_truncated_at_byte = i;
    ti->ret_truncated = overfull;
  }
  return 0;
}

// Returns length information for how we would quote ptr
// without actually saving it anywhere. 
qioerr qio_quote_string_length(uint8_t string_start, uint8_t string_end, uint8_t string_format, const char* restrict ptr, ssize_t len, qio_truncate_info_t* ti)
{
  qioerr err;
  ssize_t i; // how far along the input are we (ie ptr[i])
  ssize_t quoted_bytes = 0; // how many bytes of output?
  ssize_t quoted_chars = 0; // how many chars of output?
  ssize_t quoted_cols = 0; // how many columns of output?
  ssize_t safe_i = 0;
  ssize_t safe_quoted_bytes = 0;
  ssize_t safe_quoted_chars = 0;
  ssize_t safe_quoted_cols = 0;
  int ellipses_size;
  int end_quote_size;
  int start_quote_size;
  int clen = 1;
  int32_t chr;
  ssize_t max_cols = (ti)?(ti->max_columns):(SSIZE_MAX);
  ssize_t max_chars = (ti)?(ti->max_chars):(SSIZE_MAX);
  ssize_t max_bytes = (ti)?(ti->max_bytes):(SSIZE_MAX);
  int overfull = 0;
  int tmplen, tmpchars, tmpcols;

  // write the string itself, possible with some escape-handling.
  if( string_format == QIO_STRING_FORMAT_WORD ||
      string_format == QIO_STRING_FORMAT_TOEND ) {
    ellipses_size = 0;
    end_quote_size = 0;
    start_quote_size = 0;
  } else {
    ellipses_size = 3; // ie ...
    end_quote_size = 1; // ie a double quote
    start_quote_size = 1;
    // Smallest pattern is ""...
    if( max_cols < 5 ) max_cols = 5;
    if( max_chars < 5 ) max_chars = 5;
    if( max_bytes < 5 ) max_bytes = 5;
  }

  // Account for the starting quote.
  quoted_bytes += start_quote_size;
  quoted_chars += start_quote_size;
  quoted_cols += start_quote_size;

  // we need to compute the number of characters
  // and the total number of columns.
  for( i = 0; i < len; i+=clen ) {
    err = qio_decode_char_buf(&chr, &clen, ptr + i, len - i);
    if( err ) {
      err = 0;
    }
    tmplen = _qio_chr_escape(chr, string_end, string_format, NULL,
                             &tmpchars, &tmpcols);
    if( tmplen < 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
      goto error;
    }
    if( quoted_bytes + ellipses_size + end_quote_size <= max_bytes &&
        quoted_chars + ellipses_size + end_quote_size <= max_chars &&
        quoted_cols + ellipses_size + end_quote_size <= max_cols) {
      safe_i = i;
      safe_quoted_bytes = quoted_bytes;
      safe_quoted_chars = quoted_chars;
      safe_quoted_cols = quoted_cols;
    }
    quoted_bytes += tmplen;
    quoted_chars += tmpchars;
    quoted_cols += tmpcols;
    if( quoted_bytes + end_quote_size > max_bytes ||
        quoted_chars + end_quote_size > max_chars ||
        quoted_cols + end_quote_size > max_cols ) {
      // We have over-filled.
      overfull = 1;
      break;
    }
  }

  if( overfull ) {
    i = safe_i;
    // and add end quote and ellipses.
    quoted_bytes = safe_quoted_bytes;
    quoted_chars = safe_quoted_chars;
    quoted_cols = safe_quoted_cols;
  }

  // Account for the end quote
  quoted_bytes += end_quote_size;
  quoted_chars += end_quote_size;
  quoted_cols += end_quote_size;

  if( overfull ) {
    // Account for the ellipses
    quoted_bytes += ellipses_size;
    quoted_chars += ellipses_size;
    quoted_cols += ellipses_size;
  }

  if( ti ) {
    ti->ret_columns = quoted_cols;
    ti->ret_chars = quoted_chars;
    ti->ret_bytes = quoted_bytes;
    ti->ret_truncated_at_byte = i;
    ti->ret_truncated = overfull;
  }
  return 0;
error:
  if( ti ) {
    ti->ret_columns = -1;
    ti->ret_chars = -1;
    ti->ret_bytes = -1;
    ti->ret_truncated_at_byte = -1;
    ti->ret_truncated = -1;
  }
  return err;
}


qioerr qio_quote_string(uint8_t string_start, uint8_t string_end, uint8_t string_format, const char* restrict ptr, ssize_t len, const char** out, qio_truncate_info_t* ti_arg)
{
  qioerr err;
  ssize_t i;
  ssize_t ilen;
  ssize_t q;
  int32_t chr;
  int clen = 0;
  char* ret;
  int tmplen;
  qio_truncate_info_t ti;

  ti.max_columns = (ti_arg)?(ti_arg->max_columns):(SSIZE_MAX);
  ti.max_chars = (ti_arg)?(ti_arg->max_chars):(SSIZE_MAX);
  ti.max_bytes = (ti_arg)?(ti_arg->max_bytes):(SSIZE_MAX);
 
  // Figure out the how big the string will be
  err = qio_quote_string_length(string_start, string_end, string_format, ptr, len, &ti);
  if( err ) return err;

  // If we are returning ti info, copy it out.
  if( ti_arg ) {
    *ti_arg = ti;
  }

  // If we don't have to return the string... stop now.
  if( ! out ) return 0;

  // Now, allocate a string of the right size
  ret = qio_malloc(ti.ret_bytes + 1); // room for \0.
  if( !ret ) {
    return QIO_ENOMEM;
  }

  // Now, copy while escaping.
  ret[ti.ret_bytes] = '\0';

  q = 0;
  ilen = ti.ret_truncated_at_byte;

#define WRITEC(c) { \
    clen = qio_nbytes_char(c); \
    qio_encode_char_buf(&ret[q], c); \
    q += clen; \
  }

  if( string_format == QIO_STRING_FORMAT_WORD ||
      string_format == QIO_STRING_FORMAT_TOEND ) {
    // No string start
  } else {
    // Write string_start.
    WRITEC(string_start);
  }

  for( i = 0; i < ilen; i+=clen ) {
    // Write the string while translating it.
    err = qio_decode_char_buf(&chr, &clen, ptr + i, len - i);
    if( err ) goto error;

    // handle escaping for the different formats.
    tmplen = _qio_chr_escape(chr,string_end,string_format,&ret[q], NULL, NULL);
    if( tmplen < 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
      goto error;
    }
    q += tmplen;
  }

  if( string_format == QIO_STRING_FORMAT_WORD ||
      string_format == QIO_STRING_FORMAT_TOEND ) {
    // No string end
  } else {
    // Write string_end.
    WRITEC(string_end);
    if( ti.ret_truncated ) {
      WRITEC('.');
      WRITEC('.');
      WRITEC('.');
    }
  }

  if( out ) *out = ret;
  return 0;
error:
  qio_free(ret);
  if( out ) *out = NULL;
  return err;
#undef WRITEC
}

static inline bool is_json_whitespace(int32_t c)
{
  return ( c == ' ' || c == '\b' ||
           c == '\f' || c == '\n' || c == '\r' || c == '\t' );
}

// Read and skip an arbitrary JSON object, assuming the leading '{'
// has already been read. Returns 0 on success or a negative error code.
int32_t qio_skip_json_object_unlocked(qio_channel_t* restrict ch)
{
  int32_t c;

  while( true ) {
    // Read a field.
    c = qio_skip_json_field_unlocked(ch);
    if( c < 0 ) return c;

    // Read whitespace followed by , or '}'
    if( c == 0 || is_json_whitespace(c) ) {
      while( true ) {
        c = qio_channel_read_byte(false, ch);
        if( c < 0 ) return c;
        if( is_json_whitespace(c) ) {
          // continue reading whitespace.
        } else {
          break;
        }
      }
    }

    if( c == ',' ) {
      // OK, move on to the next value.
    } else if( c == '}' ) {
      // we've reached the end.
      return 0;
    } else {
      return -EFORMAT;
    }
  }
}

// Read and skip an arbitrary JSON array, assuming the leading '['
// has already been read. Returns 0 on success or a negative error code.
int32_t qio_skip_json_array_unlocked(qio_channel_t* restrict ch)
{
  int32_t c;

  while( true ) {
    // Read a value.
    c = qio_skip_json_value_unlocked(ch);
    if( c < 0 ) return c;

    // Read a whitespace followed by , or ']'
    if( c == 0 || is_json_whitespace(c) ) {
      while( true ) {
        c = qio_channel_read_byte(false, ch);
        if( c < 0 ) return c;
        if( is_json_whitespace(c) ) {
          // continue reading whitespace.
        } else {
          break;
        }
      }
    }

    if( c == ',' ) {
      // OK, move on to the next value.
    } else if( c == ']' ) {
      // we've reached the end.
      return 0;
    } else {
      return -EFORMAT;
    }
  }
}

// Read and skip an arbitrary JSON value.
// Returns any unhandled character, 0 if all characters
// were handled, or a negative error code.
// unhandled characters could easily be , ] }
int32_t qio_skip_json_value_unlocked(qio_channel_t* restrict ch)
{
  int32_t c;

  // Read whitespace and then a value.
  while( true ) {
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( is_json_whitespace(c) ) {
      // continue reading whitespace.
    } else {
      break;
    }
  }

  if( c == '"' ) {
    // read string until matching '"'
    return qio_skip_json_string_unlocked(ch);
  } else if( c == '-' || ('0' <= c && c <= '9') ) {
    // read digits before .
    while( true ) {
      c = qio_channel_read_byte(false, ch);
      if( c < 0 ) return c;
      if( '0' <= c && c <= '9' ) {
        // continue reading digits.
      } else {
        break;
      }
    }

    // now c is the first non-digit.

    if( c == '.' ) {
      // read some more digits after .
      while( true ) {
        c = qio_channel_read_byte(false, ch);
        if( c < 0 ) return c;
        if( '0' <= c && c <= '9' ) {
          // continue reading digits.
        } else {
          break;
        }
      }
    }

    // now read e or E followed by + - and exponent digits
    if( c == 'e' || c == 'E' ) {
      // read +, -, or a digit
      c = qio_channel_read_byte(false, ch);
      if( c < 0 ) return c;
      if( c == '+' || c == '-' || ('0' <= c && c <= '9') ) {
        // OK
      } else {
        return -EFORMAT;
      }
      // read some more digits
      while( true ) {
        c = qio_channel_read_byte(false, ch);
        if( c < 0 ) return c;
        if( '0' <= c && c <= '9' ) {
          // continue reading digits.
        } else {
          break;
        }
      }
    }

    // Returns the next non-digit (could be , for example)
    return c;
  } else if( c == '{' ) {
    // read object until matching '}'
    return qio_skip_json_object_unlocked(ch);
  } else if( c == '[' ) {
    // read array until matching ']'
    return qio_skip_json_array_unlocked(ch);
  } else if( c == 't' ) {
    // read true
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( c != 'r' ) return -EFORMAT;
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( c != 'u' ) return -EFORMAT;
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( c != 'e' ) return -EFORMAT;
    return 0;
  } else if( c == 'f' ) {
    // read false
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( c != 'a' ) return -EFORMAT;
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( c != 'l' ) return -EFORMAT;
    c = qio_channel_read_byte(false, ch);
    if( c < 0  ) return c;
    if( c != 's' ) return -EFORMAT;
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( c != 'e' ) return -EFORMAT;
    return 0;
  } else if( c == 'n' ) {
    // read null
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( c != 'u' ) return -EFORMAT;
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( c != 'l' ) return -EFORMAT;
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( c != 'l' ) return -EFORMAT;
    return 0;
  } else {
    // some other character - it could be ] } , after a value.
    return c;
  }
}

// Read and skip an arbitrary JSON string, assuming the leading "
// has already been read. Returns 0 on success, or a negative
// error code.
int32_t qio_skip_json_string_unlocked(qio_channel_t* restrict ch)
{
  int32_t c;

  while( true ) {
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;

    // quote: end of string.
    if( c == '\"' ) return 0;

    // backslash: handle quoting.
    if( c == '\\' ) {
      c = qio_channel_read_byte(false, ch);
      if( c < 0 ) return c;

      // As long as we handle \" correctly, there is
      // no need to validate the JSON in the string.
      // Furthermore, since we are just skipping over it,
      // there is no need to take special action for
      // particular backslash-escapes.
    }
  }
}

// Read and skip an arbitrary JSON field, not counting a following ,
// Returns the last character read, or
// negative for a negative error code.
int32_t qio_skip_json_field_unlocked(qio_channel_t* restrict ch)
{
  int32_t c;

  // Read a whitespace followed by " or '}'
  while( true ) {
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( is_json_whitespace(c) ) {
      // continue reading whitespace.
    } else {
      break;
    }
  }

  if( c == '"' ) {
    // OK, move on to reading the value.
  } else if( c == '}' ) {
    return c;
  } else {
    return -EFORMAT;
  }

  c = qio_skip_json_string_unlocked(ch);
  if( c < 0 ) return c;

  // Read a whitespace followed by :
  while( true ) {
    c = qio_channel_read_byte(false, ch);
    if( c < 0 ) return c;
    if( is_json_whitespace(c) ) {
      // continue reading whitespace.
    } else {
      break;
    }
  }

  if( c == ':' ) {
    // OK, move on to reading the value.
  } else {
    return -EFORMAT;
  }

  c = qio_skip_json_value_unlocked(ch);
  return c;
}

qioerr qio_channel_skip_json_field(const int threadsafe, qio_channel_t* ch)
{
  qioerr err;
  int32_t got;
  int64_t start_offset;
  int64_t offset;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  start_offset = qio_channel_offset_unlocked(ch);

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  got = qio_skip_json_field_unlocked(ch);
  if( got < 0 ) {
    err = qio_int_to_err(-got);
    qio_channel_revert_unlocked(ch);
  } else {
    offset = qio_channel_offset_unlocked(ch);
    // if there was a last character, we need to un-get it.
    if( got > 0 ) {
      if( offset > start_offset ) {
        offset--;
      }
    }

    // now actually commit the skip.
    qio_channel_revert_unlocked(ch);

    qio_channel_advance_unlocked(ch, offset - start_offset);
  }


unlock:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }
  return err;

}

// only support floating point numbers in
// base 10, or base 16 (with decimal exponent).
//
// For other numbers, we can support more arbitrary bases...
//
// Always do case insensitive reading; the characters here should be
// lower case.
typedef struct number_reading_state_s {
  int base; // 0 means 0b 0x supported; otherwise particular base 2,10,16

  char allow_base; // allow 0b or 0x when base == 2 or == 16 respectively
                   // (these are always allowed when base == 0)
  char allow_pos_sign;
  char allow_neg_sign;
  char positive_char; // normally '+'
  char negative_char; // normally '-'

  char allow_real; // allow any real thing, infinity, etc.
  char allow_point; // just allow e.g. 123.254
  char point_char; // normally '.'; set to -1 if no radix point allowed
  char exponent_char; // use if base <= 10; normally 'e'
  char other_exponent_char; // use if base > 10; normally 'p' or '@'

  char allow_i_after; // allow an 'i' after the number.
  char i_char;

  int usebase;

  signed char sign; // resulting sign; positive or negative
  char is_nan; // did we read nan()? 
  char is_inf; // did we read inf or infinity?
  int gotbase;
  int64_t digits_start; // where do the digits start?
  int64_t point; // where is the decimal point?
  int64_t exponent; // where is the exponent character (e.g. e)?
  int64_t i_after; // where is the i after the number, or -1 if not present
  int64_t end; // where is the end?
               // set to -1 if we shouldn't yet be at the end.
} number_reading_state_t;

static
qioerr _peek_number_unlocked(qio_channel_t* restrict ch, number_reading_state_t* restrict s, int64_t* restrict amount)
{
#define NEXT_CHR \
  { \
    err = qio_channel_read_char(false, ch, &chr); \
    if( err ) goto error; \
    chr = towlower(chr); \
  }
#define NEXT_CHR_OR_EOF \
  { \
    err = qio_channel_read_char(false, ch, &chr); \
    if( qio_err_to_int(err) == EEOF ) chr = -1; \
    else if( err ) goto error; \
    chr = towlower(chr); \
  }
#define ACCEPT \
  { \
    s->end = qio_channel_offset_unlocked(ch); \
    goto done; \
  }
#define UNGET_ACCEPT \
  { \
    s->end = qio_channel_offset_unlocked(ch) - 1; \
    goto done; \
  }

#define START_DIGITS \
  { \
    s->digits_start = qio_channel_offset_unlocked(ch) - 1; \
  }

  int64_t mark_offset;
  int32_t chr;
  qioerr err;

  s->sign = 0;
  s->is_nan = 0;
  s->is_inf = 0;
  s->digits_start = -1;
  s->point = -1;
  s->exponent = -1;
  s->i_after = -1;
  s->end = -1;
  if( s->base == 0 ) {
    s->gotbase = s->usebase = 10;
  } else {
    s->gotbase = s->usebase = s->base;
  }
  if( s->allow_real ) s->allow_point = 1;

  mark_offset = qio_channel_offset_unlocked(ch);

  err = qio_channel_mark(false, ch);
  if( err ) return err;

  // First, skip any whitespace.
  do {
    NEXT_CHR;
  } while( iswspace(chr) );
  // At end of loop, chr is non-whitespace.

  // Next, read + or -.
  if( s->allow_pos_sign && chr == s->positive_char ) {
    s->sign = 1;
  } else if( s->allow_neg_sign && chr == s->negative_char ) {
    s->sign = -1;
  } else {
    s->sign = 0;
  }

  if( s->sign ) {
    // chr is + or - so read next character.
    NEXT_CHR;
  }

  // Next, read 0 or base specifier or infinity or nan
  if( s->allow_real && chr == 'i' ) {
    START_DIGITS;
    // Read infinity.
    NEXT_CHR;
    if( chr != 'n' ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "expected 'n' in inf");
      goto error;
    }
    NEXT_CHR;
    if( chr != 'f' ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "expected 'f' in inf");
      goto error;
    }
    s->is_inf = 1;
    NEXT_CHR_OR_EOF;
    if( chr != 'i' ) UNGET_ACCEPT;
    NEXT_CHR;
    if( chr != 'n' ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "expected 'n' in infinity");
      goto error;
    }
    NEXT_CHR;
    if( chr != 'i' ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "expected 'i' in infinity");
      goto error;
    }
    NEXT_CHR;
    if( chr != 't' ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "expected 't' in infinity");
      goto error;
    }
    NEXT_CHR;
    if( chr != 'y' ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "expected 'y' in infinity");
      goto error;
    }
    ACCEPT;
  } else if( s->allow_real && chr == 'n' ) {
    START_DIGITS;
    // Read not a number.
    NEXT_CHR;
    if( chr != 'a' ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "expected 'a' in nan");
      goto error;
    }
    NEXT_CHR;
    if( chr != 'n' ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "expected 'n' in nan");
      goto error;
    }
    s->is_nan = 1;
    NEXT_CHR_OR_EOF;
    if( chr != '(' ) UNGET_ACCEPT;
    while( 1 ) {
      NEXT_CHR;
      if( chr == ')' ) ACCEPT;
    }
  } else if( chr == '0' ) {
    // Read x or b
    NEXT_CHR_OR_EOF;
    if( qio_err_to_int(err) == EEOF ) {
      s->digits_start = qio_channel_offset_unlocked(ch) - 1;
    } else if( s->allow_base && chr == 'x' ) {
      s->gotbase = s->usebase = 16;
      NEXT_CHR;
      START_DIGITS;
    } else if( s->allow_base && chr == 'o' ) {
      s->gotbase = s->usebase = 8;
      NEXT_CHR;
      START_DIGITS;
    } else if( s->allow_base && chr == 'b' ) {
      s->gotbase = s->usebase = 2;
      NEXT_CHR;
      START_DIGITS;
    } else {
      // It could just be a number starting with 0.
      // We've already read an extra digit.
      s->digits_start = qio_channel_offset_unlocked(ch) - 2;
    }
  } else {
    // We (hopefully) read a digit.
    START_DIGITS;
  }

  // Read some digits.
  while( 1 ) {
    //printf("In read digit, chr is %c\n", chr);
    if( qio_err_to_int(err) == EEOF ) ACCEPT;
    if( s->allow_point && chr == s->point_char && s->point == -1 ) {
      s->end = s->point = qio_channel_offset_unlocked(ch);
      NEXT_CHR_OR_EOF;
      // Continue to read digits.
    } else if( s->allow_real && 
               ( (s->usebase <= 10 && chr == s->exponent_char) ||
                 (s->usebase > 10 && chr == s->other_exponent_char) ) &&
               s->exponent == -1 ) {
      // Exponent char
      s->end = s->exponent = qio_channel_offset_unlocked(ch);
      s->usebase = 10; // exponents are always base 10.
      // See if we have a +.
      NEXT_CHR_OR_EOF;
      // Next, read + or -.
      if( chr == s->positive_char || chr == s->negative_char ) {
        NEXT_CHR;
      }
      // Continue to read digits.
    } else if( s->usebase <= 10 && ('0' <= chr && chr < '0' + s->usebase) ) {
      // have digit.
      NEXT_CHR_OR_EOF;
    } else if( s->usebase > 10 &&
               ( ('0' <= chr && chr <= '9') ||
                 ('a' <= chr && chr < 'a' + s->usebase-10) ||
                 ('A' <= chr && chr < 'A' + s->usebase-10) ) ) {
      NEXT_CHR_OR_EOF;
    } else if( s->allow_i_after && chr == s->i_char ) {
      s->end = s->i_after = qio_channel_offset_unlocked(ch);
      NEXT_CHR_OR_EOF;
    } else {
      // Not a digit.
      UNGET_ACCEPT;
    }
  }

done:
  err = 0;
  *amount = s->end - mark_offset;
  if( *amount == 0 ) {
    QIO_GET_CONSTANT_ERROR(err, EFORMAT, "malformed number");
  }
error:
  qio_channel_revert_unlocked(ch);

  return err;
}


qioerr qio_channel_scan_int(const int threadsafe, qio_channel_t* restrict ch, void* restrict out, size_t len, int issigned)
{
  unsigned long long int num = 0;
  long long int signed_num;
  int sign = 1;
  ssize_t signed_len;
  number_reading_state_t st;
  int64_t amount;
  int64_t start;
  char* end;
  char* buf = NULL;
  char buf_onstack[MAX_ON_STACK];
  qioerr err;
  qio_style_t* style;


  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  style = &ch->style;

  memset(&st, 0, sizeof(number_reading_state_t));

  st.base = style->base;
  st.allow_base = style->prefix_base;
  st.allow_pos_sign = 1;
  st.allow_neg_sign = issigned;
  if(style->showpoint || style->precision > 0) {
    st.allow_point = 1;
    st.point_char = tolower(style->point_char);
  }
  st.positive_char = tolower(style->positive_char);
  st.negative_char = tolower(style->negative_char);

  err = _peek_number_unlocked(ch, &st, &amount);
  if( qio_err_to_int(err) == EEOF && st.end > 0 ) err = 0; // we tolerate EOF if there's data.
  if( err ) goto error;

  MAYBE_STACK_ALLOC(char, amount + 1, buf, buf_onstack);
  if( ! buf ) {
    err = QIO_ENOMEM;
    goto error;
  }
  buf[amount] = '\0';

  start = qio_channel_offset_unlocked(ch);

  err = qio_channel_read_amt(false, ch, buf, amount);
  if( err ) goto error;

  // Now we have the number we're converting in buf.

  // Read a sign, if necessary.
  sign = 1; // positive!

  if( issigned ) {
    sign = st.sign;
  }

  //printf("Scanning int %s in base %i\n", buf, st.gotbase);

  // Now read the number.
  num = strtoull( buf + st.digits_start - start, &end, st.gotbase );
  if( num == ULLONG_MAX ) {
    // overflow.
    err = qio_mkerror_errno();
    goto error;
  }
  if( st.allow_point ) {
    // pass . or .00000
    if( *end == '.' ) end++;
    while( *end == '0' ) end++;
  }
  if( end - buf != st.end - start ) {
    // some kind of format error.
    if( st.allow_point ) {
      QIO_GET_CONSTANT_ERROR(err, ERANGE, "malformed integer with fraction");
    } else {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "malformed integer");
    }
    goto error;
  }

  // success!
  err = 0;

error:
  if( err != 0 ) num = 0;

  // now return the number.
  signed_len = len;
  if( issigned ) signed_len = - signed_len;

  signed_num = num;
  if( sign < 0 ) signed_num = - num;

  switch( signed_len ) {
    case -1:
      *(int8_t*) out = signed_num;
      if( signed_num > INT8_MAX || signed_num < INT8_MIN )
        QIO_GET_CONSTANT_ERROR(err, ERANGE, "read out of bounds integer");
      break;
    case 1:
      *(uint8_t*) out = num;
      if( num > UINT8_MAX )
        QIO_GET_CONSTANT_ERROR(err, ERANGE, "read out of bounds integer");
      break;
    case -2:
      *(int16_t*) out = signed_num;
      if( signed_num > INT16_MAX || signed_num < INT16_MIN )
        QIO_GET_CONSTANT_ERROR(err, ERANGE, "read out of bounds integer");
      break;
    case 2:
      *(uint16_t*) out = num;
      if( num > UINT16_MAX )
        QIO_GET_CONSTANT_ERROR(err, ERANGE, "read out of bounds integer");
      break;
    case -4:
      *(int32_t*) out = signed_num;
      if( signed_num > INT32_MAX || signed_num < INT32_MIN )
        QIO_GET_CONSTANT_ERROR(err, ERANGE, "read out of bounds integer");
      break;
    case 4:
      *(uint32_t*) out = num;
      if( num > UINT32_MAX )
        QIO_GET_CONSTANT_ERROR(err, ERANGE, "read out of bounds integer");
      break;
    case -8:
      *(int64_t*) out = signed_num;
      //if( signed_num > INT64_MAX || signed_num < INT64_MIN )
      //  QIO_GET_CONSTANT_ERROR(err, ERANGE, "read out of bounds integer");
      break;
    case 8:
      *(uint64_t*) out = num;
      //if( num > UINT64_MAX )
      //  QIO_GET_CONSTANT_ERROR(err, ERANGE, "read out of bounds integer");
      break;
    default:
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "bad integer type");
  }

  MAYBE_STACK_FREE(buf, buf_onstack);

  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

static
qioerr qio_channel_scan_float_or_imag(const int threadsafe, qio_channel_t* restrict ch, void* restrict out, size_t len, bool imag)
{
  double num = 0.0;
  number_reading_state_t st;
  int64_t amount;
  int64_t start;
  char* end_conv;
  char* buf = NULL;
  char buf_onstack[MAX_ON_STACK];
  ssize_t i;
  ssize_t digits_start;
  ssize_t point;
  ssize_t exponent;
  qioerr err;
  qio_style_t* style;
  bool needs_i = 0;


  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  style = &ch->style;

  needs_i = imag && style->complex_style == QIO_COMPLEX_FORMAT_ABI;

  memset(&st, 0, sizeof(number_reading_state_t));

  st.base = style->base;
  st.allow_base = style->prefix_base;
  st.allow_pos_sign = 1; // could check style->showplus == 1 also
  st.allow_neg_sign = 1;
  st.positive_char = tolower(style->positive_char);
  st.negative_char = tolower(style->negative_char);
  st.allow_real = 1;
  st.point_char = tolower(style->point_char);
  st.exponent_char = tolower(style->exponent_char);
  st.other_exponent_char = tolower(style->other_exponent_char);
  st.allow_i_after = needs_i;
  st.i_char = style->i_char;

  err = _peek_number_unlocked(ch, &st, &amount);
  if( qio_err_to_int(err) == EEOF && st.end > 0 ) err = 0; // we tolerate EOF if there's data.
  if( err ) goto error;


  if( needs_i ) {
    if( st.i_after != -1 ) {
      // OK! we found an i after.
    } else {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "missing i after imaginary number");
      goto error;
    }
  }
  //printf("got amount %lli\n", (long long int) amount);

  MAYBE_STACK_ALLOC(char, amount + 4, buf, buf_onstack);
  if( ! buf ) {
    err = QIO_ENOMEM;
    goto error;
  }
  buf[0] = ' ';
  buf[1] = ' ';
  buf[2] = ' ';
  buf[amount+3] = '\0';

  start = qio_channel_offset_unlocked(ch);

  err = qio_channel_read_amt(false, ch, buf + 3, amount);
  if( err ) goto error;

  digits_start = 3 + st.digits_start - start;
  point = 3 + st.point - start;
  exponent = 3 + st.exponent - start;;
  //end = 3 + st.end - start;

  //printf("read '%s', d %i p %i e %i e %i\n", buf, digits_start, point, exponent, end);
  // Where in the string do we start?
  i = digits_start - 3;

  // Store a base and sign at the front of the buffer, or 00 for decimal.
  // Convert a binary number to hexadecimal.
  if( st.gotbase == 10 ) {
    buf[i++] = ' ';
    buf[i++] = ' ';
    buf[i++] = (st.sign>=0)?'+':'-';
  } else if ( st.gotbase == 16 ) {
    if( st.is_nan || st.is_inf ) {
      // do nothing. 
    } else {
      buf[i++] = (st.sign>=0)?'+':'-';
      buf[i++] = '0';
      buf[i++] = 'x';
    }
  } else {
    QIO_GET_CONSTANT_ERROR(err, EFORMAT, "unknown floating point base");
    goto error;
  }
 
  // set the radix character to '.'
  if( point >= i && tolower(buf[point]) == st.point_char ) {
    buf[point] = '.';
  }

  // set the exponent character to 'p' for hex-floating point conversions.
  if( exponent >= i &&
      st.base == 16 &&
      tolower(buf[exponent]) == st.other_exponent_char &&
      st.other_exponent_char != 'p') {
    buf[exponent] = 'p';
  }

  // set the exponent character to 'e' for decimal floating point conversions.
  if( exponent >= i &&
      st.base == 10 &&
      tolower(buf[exponent]) == st.exponent_char &&
      st.exponent_char != 'e' ) {
    buf[exponent] = 'e';
  }
 
  // fix the sign characters in the exponent.
  if( exponent >= i &&
      tolower(buf[exponent]) == st.exponent_char ) {
    if( tolower(buf[exponent+1]) == st.negative_char ) {
      buf[exponent+1] = '-';
    } else if( tolower(buf[exponent+1]) == st.positive_char ) {
      buf[exponent+1] = '+';
    }
  }

  //printf("strtod on %s\n", buf);

  errno = 0;
  // Now we have the number we're converting in buf. Read it.
  num = strtod( buf, &end_conv);
  if( num == 0 && end_conv == buf ) {
    // no conversion is performed.
    QIO_GET_CONSTANT_ERROR(err, EFORMAT, "not a floating point number");
    goto error;
  }
  if((num == HUGE_VAL||num == -HUGE_VAL||num == 0.0) && errno == ERANGE){
    // overflow or underflow
    QIO_GET_CONSTANT_ERROR(err, ERANGE, "floating point number out of bounds");
    goto error;
  }

  err = 0;

error:
  if( err ) num = 0;

  switch ( len ) {
    case 8:
      *(double*) out = num;
      break;
    case 4:
      *(float*) out = num;
      break;
    default:
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "bad floating point type");
  }

  MAYBE_STACK_FREE(buf, buf_onstack);

  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

qioerr qio_channel_scan_float(const int threadsafe, qio_channel_t* restrict ch, void* restrict out, size_t len)
{
  return qio_channel_scan_float_or_imag(false, ch, out, len, false);
}
qioerr qio_channel_scan_imag(const int threadsafe, qio_channel_t* restrict ch, void* restrict out, size_t len)
{
  return qio_channel_scan_float_or_imag(false, ch, out, len, true);
}

// core of ltoa for arbitrary base.
// Fills in tmp from right to left
// Returns the number of positions in tmp to skip to get to number.
// Returns -1 on buffer overflow.
// supports up to base 36.
static inline int _ltoa_convert(char *tmp, int tmplen, uint64_t num, int base, int uppercase)
{
  int at;
  int digit;
  char ch;
  tmp[tmplen-1] = '\0';
  for( at = tmplen-2; at >= 0; at-- ) {
    // Get the remainder mod base
    digit = num % base;
    // Divide by base
    num /= base;
    // Compute the character to use
    if( base <= 10 || digit < 10 ) ch = '0' + digit;
    else ch = ((uppercase)?('A'):('a')) + digit - 10;
    // Set the character.
    tmp[at] = ch;
    if( num == 0 ) break;
  }
  return at;
}

// dst must have room (at most 65 bytes for binary + '\0')
// Returns the number of characters written (not including '\0')
// or >= size if there wasn't room in the buffer (returns amt needed)
// or a negative value if there was an error.
// We have this function to support binary numbers.
static
int _ltoa(char* restrict dst, size_t size, uint64_t num, int isnegative,
          int base, const qio_style_t* restrict style)
{
  char tmp[65]; // enough room for largest 64-bit number base 2.
  int tmp_len=0;
  int tmp_skip=0;
  char b=0;
  int i,j;
  int width;
  int ret_width;
  int show_point = 0; // 1 to show a . afterwards
  int num_zeros_after = 0; // how many 0s to print afterwards?

  // Optimize conversions for supported bases
  if( base == 2 )
    tmp_skip = _ltoa_convert(tmp, sizeof(tmp), num, 2, 0);
  else if( base == 8 )
    tmp_skip = _ltoa_convert(tmp, sizeof(tmp), num, 8, 0);
  else if( base == 10 )
    tmp_skip = _ltoa_convert(tmp, sizeof(tmp), num, 10, 0);
  else if( base == 16 )
    tmp_skip = _ltoa_convert(tmp, sizeof(tmp), num, 16, style->uppercase);
  else
    tmp_skip = _ltoa_convert(tmp, sizeof(tmp), num, base, style->uppercase);

  if (tmp_skip < 0) {
    // Internal buffer too small.  Increase size of tmp[].
    return -1;
  }

  tmp_len = sizeof(tmp) - 1 - tmp_skip;
  width = tmp_len;

  if( style->showplus || isnegative ) width++;
  if( style->prefix_base && base != 10 ) width += 2;
  if( style->showpoint || style->precision > 0 ) {
    // intentionally doesn't handle showpointzero since showpointzero is
    // used to distinguish between floating point and integer values.

    show_point = 1;
    // and any number of requested digits
    if( style->precision > 0 ) num_zeros_after += style->precision;

    width += show_point + num_zeros_after;
  }

  // We might not have room...
  ret_width = width;
  if( ret_width < style->min_width_columns ) ret_width = style->min_width_columns;
  if( ret_width >= size ) return ret_width;

  i = 0;

  if( !style->leftjustify && width < style->min_width_columns && style->pad_char != '0' ) {
    // Put what we need to for getting to the min_width.
    for( ; i < style->min_width_columns - width; i++ ) {
      dst[i] = style->pad_char;
    }
  }

  if( style->showplus || isnegative ) {
    char pluschar = (style->showplus==2)?style->pad_char:style->positive_char;
    // put the '+' or '-' sign.
    dst[i++] = isnegative?style->negative_char:pluschar;
    width--;
  }
  if( style->prefix_base && base != 10 ) {
    dst[i++] = '0';
    width--;
    if( base == 2 ) b = style->uppercase?'B':'b';
    if( base == 8 ) b = style->uppercase?'O':'o';
    else if( base == 16 ) b = style->uppercase?'X':'x';
    dst[i++] = b;
    width--;
  }

  if( !style->leftjustify && width < style->min_width_columns && style->pad_char == '0' ) {
    // Put what we need to for getting to the min_width.
    for( ; i < style->min_width_columns - width; i++ ) {
      dst[i] = '0';
    }
  }

  // now output the digits.
  qio_memcpy(dst + i, tmp+tmp_skip, tmp_len);
  i += tmp_len;

  // Now output a period if we're doing showpoint.
  if( show_point ) {
    dst[i] = '.';
    i++;
  }

  for( j = 0; j < num_zeros_after; j++ ) {
    dst[i] = '0';
    i++;
  }

  // Now if we're left justified we might need padding.
  if( style->leftjustify && width < style->min_width_columns) {
    // Put what we need to for getting to the min_width.
    for( ; i < style->min_width_columns; i++ ) {
      dst[i] = style->pad_char;
    }
  }

  assert( i == ret_width );

  dst[i] = '\0';
  return i;
}

//This function finds where the last non-zero digit
//is in the decimal part of an exponential number.
//
//This will be used in _ftoa_core for resolve a
//a problem about trailing zeroes.
static int _find_prec(char *buf, int len){
  int dp_index = 0;
  int last_dig = 0;
  int index = 0;

  //Find where the decimal point is
  while( dp_index < len && buf[dp_index++] != '.');

  //Explore the decimal part for find where the
  //last non-zero digit is.
  for( index = dp_index; index < len && buf[index] != 'e'; index++ ){
    if( buf[index] != '0' )
      last_dig = index - dp_index + 1;
  }
  return last_dig;
}

// Converts num to a string in buf, returns the number
// of bytes that would be used if space permits (not including null)
// or -1 on error
// in *skip, returns the number of bytes at the front of buf that
// should be skipped in the final output. Such bytes are counted
// in the return value. (this happens with 0x in %a conversions)
//
// The output buffer will always contain a null byte at the end,
// but that byte is not counted in the return value. For that reason,
// a return = buf_sz means that we ran out of space (by 1 byte).
//
// num is the number to be converted
// buf and buf_sz are the output buffer
// base is the numeric base (10 or 16 only)
// realfmt is style->realfmt; 0->%g, 1->%f, 2->%e
// precision is the number of digits after . for %f or %e or
//   the number of significant digits
// uppercase indicates hex digits or exponent character should be uppercase
// prefix_base means add 0x for base=16 conversions
// skip is described above and indicates some buffer space used that does
//  not encode output.
static
int _ftoa_core(char* buf, size_t buf_sz, double num,
               int base, int realfmt,
               int precision, int uppercase,
               int prefix_base,
               int *skip)
{
  int got = 0;

  // Do the numeric conversion and figure out how big the output
  // is. This conversion must concern itself with precision but
  // need not handle padding or various trailing . or .0 rules.
  // That is handled in the caller.

  // snprintf always returns the number of characters that would be
  // output if the buffer were unlimited. It truncates the output -
  // and includes a trailing '\0' byte - if there is not enough space.
  //
  // For example:
  // If snprintf wants to output 12 characters, but buf_sz is 12,
  // it will return 12 and replace the last byte with a '\0'.
  // Therefore, if snprintf returns buf_sz, the output was truncated
  // and we need to try again.

  *skip = 0;

  if( base == 16 ) {
    if( precision < 0 ) {
      if( uppercase ) {
        got = snprintf(buf, buf_sz, "%A", num);
      } else {
        got = snprintf(buf, buf_sz, "%a", num);
      }
    } else {
      if( uppercase ) {
        got = snprintf(buf, buf_sz, "%.*A", precision, num);
      } else {
        got = snprintf(buf, buf_sz, "%.*a", precision, num);
      }
    }
    // The 0x isn't part of the conversion.
    // We'll add 0x later if we want it.
    if( !isnan(num) && !isinf(num) ) *skip = 2;
  } else if( realfmt == 0 ) {
    if( precision < 0 ) {
      if( uppercase ) {
        // This if is necessary because if the number has
        // 6 digits in the integer part, %g will not print
        // the decimal part because the integer part have
        // a number of digits equals to the standard precision.
        if(num >= 100000.0 && num < 1000000.0){
          got = snprintf(buf, buf_sz, "%.5E", num);
          //Since we force the %.5e for maintain a precision of
          //6 digits, the output could include some trailing zeroes.
          //With _find_prec, we find how much digits we need.
          //
          //It can also be done starting from the number itself
          //but this way avoids to deal with the loss of precision
          //caused by floating point representation
          if(buf_sz > 0)
            got = snprintf(buf, buf_sz, "%.*E",_find_prec(buf, got), num);
        }
        else
          got = snprintf(buf, buf_sz, "%G", num);
      } else {
        if(num >= 100000.0 && num < 1000000.0){
          got = snprintf(buf, buf_sz, "%.5e", num);
          if(buf_sz > 0)
            got = snprintf(buf, buf_sz, "%.*e",_find_prec(buf, got), num);
        }
        else
          got = snprintf(buf, buf_sz, "%g", num);
      }
    } else {
      if( uppercase ) {
        got = snprintf(buf, buf_sz, "%.*G", precision, num);
      } else {
        got = snprintf(buf, buf_sz, "%.*g", precision, num);
      }
    }
  } else if( realfmt == 1 ) {
    if( precision < 0 ) {
      if( uppercase ) {
        got = snprintf(buf, buf_sz, "%F", num);
      } else {
        got = snprintf(buf, buf_sz, "%f", num);
      }
    } else {
      if( uppercase ) {
        got = snprintf(buf, buf_sz, "%.*F", precision, num);
      } else {
        got = snprintf(buf, buf_sz, "%.*f", precision, num);
      }
    }
  } else if( realfmt == 2 ) {
    if( precision < 0 ) {
      if( uppercase ) {
        got = snprintf(buf, buf_sz, "%E", num);
      } else {
        got = snprintf(buf, buf_sz, "%e", num);
      }
    } else {
      if( uppercase ) {
        got = snprintf(buf, buf_sz, "%.*E", precision, num);
      } else {
        got = snprintf(buf, buf_sz, "%.*e", precision, num);
      }
    }
  }

  return got;
}

// Converts a double value into a string
// returns:
//   n a number of bytes that will be output by the conversion
//  -1 for out of memory
//  -2 for error in conversion
// *extra_space_needed is, on output, an amount of extra temporary
// space in the passed buffer which is necessary to achieve the conversion,
// beyond the returned n. In the current implementation, this is always
// at least 1 (to account for the trailing NULL byte) and might be more
// (if we needed to print a longer number and then shrink it).
//
// if conversion was not totally successful because we ran out of space,
// the returned n value might overestimate the space required. It should
// not underestimate it though.
//
// The conversion was successfull if
//   n + *extra_space_needed <= size.
static
int _ftoa(char* restrict dst, size_t size, double num, int base, bool needs_i, const qio_style_t* restrict style, int * extra_space_needed )
{
  int got=0;
  int width;
  int isnegative;
  int shownegative;
  int showbase;
  int i;
  int precision;
  int sign_base_width;
  int left_pad_width;
  int right_pad_width;
  int number_width;
  int skip;
  char pad_char;

  if( signbit(num) ) {
    //num = - num;
    num = copysign(num, 1.0);
    isnegative = 1;
  } else {
    isnegative = 0;
  }

  if( isnan(num) ) {
    shownegative = 0;
  } else {
    shownegative = isnegative;
  }

  showbase = 0;
  if( style->prefix_base && base == 16 && !isnan(num) && !isinf(num) ) {
    showbase = 1;
  }
  // NOTE: only base 10 or 16 are currently supported.

  precision = style->precision;

  // How many bytes are we adding for the sign?
  sign_base_width = 0;
  if( style->showplus || shownegative ) sign_base_width++;
  if( showbase ) sign_base_width+=2; // for 0x.

  // Fill sign_base_width with spaces
  // (useful for debugging and clarity).
  if( sign_base_width < size ) {
    for( i = 0; i < sign_base_width; i++ ) {
      dst[i] = ' '; // put spaces to overwrite later
    }
  }

  skip = 0;
  got = _ftoa_core(&dst[sign_base_width],
                   (size>sign_base_width)?(size-sign_base_width):0,
                   num, base, style->realfmt,
                   precision, style->uppercase, style->prefix_base,
                   &skip);

  // got should include space for sign and base
  got += sign_base_width;
  // but skip space for sign and base
  skip += sign_base_width;

  // We might use this many extra bytes that aren't represented
  // in the output number of bytes, but are temporary space we
  // need to achieve the conversion.
  // Always include 1 in extra space for the NULL byte at the end.
  *extra_space_needed = 1 + skip;

  if( got < 0 ) return -1;

  // Handle adding . or .0 at the end of integers.
  if( style->showpoint || style->showpointzero ) {
    if( base == 16 ) {
      // base 16 always generates with signed exponent,
      // so there's no need for . or .0 at the end of integers.
      // This could change ...
    } else {
      // maybe it was an integer that needs a .0 ?
      int needspoint;
      int i;

      needspoint = 1;
      if( got < size ) {
        // If we have succeeded at putting the number in dst,
        // go about figuring out if we need to add .0
        // If not - we will assume we need it for the purpose
        // of reserving temporary space.
        for( i = skip; i < got; i++ ) {
          if( '0' <= dst[i] && dst[i] <= '9' ) {
           // maybe still need decimal point!
          } else {
           // . e E inf etc. don't need decimal point.
           needspoint = 0;
          }
        }
      }

      if( needspoint ) {
        if( got < size ) dst[got] = '.';
        got++;

        if( style->showpointzero ) {
          if( got < size ) dst[got] = '0';
          got++;
        }
      }
    }
  }

  // Handle adding an i after an imaginary number.
  if( needs_i ) {
    if( got < size ) dst[got] = style->i_char;
    got++;
  }

  // How many bytes are we writing from the number?
  number_width = got - skip;

  width = number_width + sign_base_width;

  // How much left padding will we add?
  left_pad_width = 0;
  if( !style->leftjustify && width < style->min_width_columns ) {
    left_pad_width = style->min_width_columns - width;
  }

  // How much right padding will we add?
  right_pad_width = 0;
  if( style->leftjustify && width < style->min_width_columns) {
    right_pad_width = style->min_width_columns - width;
  }

  width = left_pad_width + sign_base_width + number_width + right_pad_width;

  if( width + skip < size ) {
    // do the rest of the conversion.

    // Move the number to its destination
    // if it's not in the right place already.
    if( sign_base_width + left_pad_width != skip ) {
      memmove(&dst[sign_base_width + left_pad_width],
              &dst[skip],
              number_width);
    }

    pad_char = style->pad_char;

    got = 0;
    // space padding goes before the sign
    if( left_pad_width > 0 && pad_char != '0' ) {
      // Put what we need to for getting to the min_width.
      for( i = 0; i < left_pad_width; i++ ) {
        dst[got++] = pad_char;
      }
    }

    // and then we put the sign
    if( style->showplus || shownegative ) {
      char pluschar = (style->showplus==2)?pad_char:style->positive_char;
      // put the '+' or '-' sign.
      dst[got++] = isnegative?style->negative_char:pluschar;
    }

    // Now we put e.g. 0x if we're going to.
    if( showbase ) {
      dst[got++] = '0';
      if( base == 16 ) dst[got++] = style->uppercase?'X':'x';
      else return -2; //unsupported floating point base.
    }

    // 0 padding goes after the sign
    if( left_pad_width > 0 && pad_char == '0' ) {
      // Put what we need to for getting to the min_width.
      for( i = 0; i < left_pad_width; i++ ) {
        dst[got++] = pad_char;
      }
    }

    // the number was already put here with the memmove call earlier.
    got += number_width;

    // Now put right padding.
    // Now if we're left justified we might need padding.
    if( right_pad_width > 0 ) {
      // Put what we need to for getting to the min_width.
      for( i = 0; i < right_pad_width; i++ ) {
        dst[got++] = pad_char;
      }
    }

    if( got < size ) dst[got] = '\0';
    assert( got == width );
  }

  return width;
}

// TODO -- support max_width
qioerr qio_channel_print_int(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, size_t len, int issigned)
{
  uint64_t num=0;
  int64_t num_s=0;
  int isneg;
  int max = 70; // room enough for binary output. (1 '\0', 2 0b, 1 +-, 64 bits)
  int signed_len;
  int base;
  int got;
  qioerr err;
  qio_style_t* style;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  style = &ch->style;

  if( style->min_width_columns + 1 > max ) max = style->min_width_columns + 1;

  base = style->base;
  if( base == 0 ) base = 10;

  signed_len = len;
  if( issigned ) signed_len = -signed_len;

  // Read the number we're printing.
  err = 0;
  switch( signed_len ) {
    case -1:
      num_s = *(int8_t*) ptr;
      break;
    case 1:
      num = *(uint8_t*) ptr;
      break;
    case -2:
      num_s = *(int16_t*) ptr;
      break;
    case 2:
      num = *(uint16_t*) ptr;
      break;
    case 4:
      num = *(uint32_t*) ptr;
      break;
    case -4:
      num_s = *(int32_t*) ptr;
      break;
    case 8:
      num = *(uint64_t*) ptr;
      break;
    case -8:
      num_s = *(int64_t*) ptr;
      break;
    default:
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "bad integer type");
  }
  if( err ) goto error;

  isneg = 0;
  if( issigned ) {
    if (num_s < 0 ) {
      isneg = 1;
      num = - num_s;
    } else {
      num = num_s;
    }
  }

  // Try printing it directly into the buffer.
  if( qio_space_in_ptr_diff(max, ch->cached_end,ch->cached_cur) ) {
    // Print it all directly into the buffer.
    got = _ltoa(ch->cached_cur, qio_ptr_diff(ch->cached_end,ch->cached_cur), 
                num, isneg, base, style);
    if( got < 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "unknown base or bad width");
      goto error;
    } else if( got < qio_ptr_diff(ch->cached_end,ch->cached_cur) ) {
      ch->cached_cur = qio_ptr_add(ch->cached_cur, got);
      err = _qio_channel_post_cached_write(ch);
      // OK!
      goto error;
    } else {
      // There was not enough room, but we can set the
      // amount of room needed.
      max = got + 1;
    }
  }
 
  // We get here if there wasn't enough room in cached for _ltoa
  {
    char* tmp = NULL;
    char tmp_onstack[MAX_ON_STACK];

    // Store it in a temporary variable and then
    // copy that in to the buffer.
    MAYBE_STACK_ALLOC(char, max, tmp, tmp_onstack);
    if( ! tmp ) {
      err = QIO_ENOMEM;
      goto error_free;
    }

    // Print it all to tmp
    got = _ltoa(tmp, max,
                num, isneg, base, style);
    if( got < 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "unknown base or bad width");
      goto error_free;
    } else if( got < max ) {
      err = qio_channel_write_amt(false, ch, tmp, got);
      goto error_free;
    } else {
      // the 2nd call to ltoa wants more space than the first.
      assert(got < max);
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "internal error");
      goto error_free;
    }
error_free:
    MAYBE_STACK_FREE(tmp, tmp_onstack);
  }

error:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

// +10 = a little extra room.
#define MAX_DOUBLE_DIGITS (3 + DBL_MANT_DIG - DBL_MIN_EXP + 1 + 10)

// TODO -- support max_width.
static
qioerr qio_channel_print_float_or_imag(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, size_t len, bool imag)
{
  int max = 0;
  int got;
  int base;
  qioerr err;
  double num = 0;
  qio_style_t* style;
  bool needs_i;
  int extra = 0;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  style = &ch->style;

  needs_i = imag && style->complex_style == QIO_COMPLEX_FORMAT_ABI;
  base = style->base;
  if( base == 0 ) base = 10;

  err = 0;
  switch (len) {
    case 4:
      num = *(float*) ptr;
      break;
    case 8:
      num = *(double*) ptr;
      break;
    default:
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "bad floating point type");
  }
  if( err ) goto error;

  // Try printing it directly into the buffer.
  got = _ftoa(ch->cached_cur, qio_ptr_diff(ch->cached_end, ch->cached_cur),
              num, base, needs_i, style, &extra);

  if( got < 0 ) {
    if( got == -1 ) err = QIO_ENOMEM;
    else QIO_GET_CONSTANT_ERROR(err, EINVAL, "converting floating point number to string");
    goto error;
  } else if( qio_space_in_ptr_diff(got + extra,
                                   ch->cached_end, ch->cached_cur) ) {

    ch->cached_cur = qio_ptr_add(ch->cached_cur, got);
    err = _qio_channel_post_cached_write(ch);
    // OK!
    goto error;
  } else {
    // There was not enough room, but we can set the
    // amount of room needed.
    max = got + extra + 1;
  }
 

  // We get here if there wasn't enough room in cached for _ftoa
  // In that case, write to a temporary buffer. Note that
  // 'max' should be at this point the number of digits needed.
  {
    char* buf = NULL;
    char buf_onstack[MAX_ON_STACK];

    // Store it in a temporary variable and then
    // copy that in to the buffer.
    MAYBE_STACK_ALLOC(char, max, buf, buf_onstack);
    if( ! buf ) {
      err = QIO_ENOMEM;
      goto error_free;
    }

    // Print it all to buf
    got = _ftoa(buf, max, num, base, needs_i, style, &extra);
    if( got < 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "converting floating point number to string");
      goto error_free;
    } else if( got + extra < max ) {

      err = qio_channel_write_amt(false, ch, buf, got);
      goto error_free;
    } else {
      // the 2nd call to ftoa wants more space than the first.
      assert(got + extra < max);
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "internal error");
      goto error_free;
    }

error_free:
    MAYBE_STACK_FREE(buf, buf_onstack);
  }

error:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

qioerr qio_channel_print_float(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, size_t len)
{
  return qio_channel_print_float_or_imag(threadsafe, ch, ptr, len, false);
}
qioerr qio_channel_print_imag(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, size_t len)
{
  return qio_channel_print_float_or_imag(threadsafe, ch, ptr, len, true);
}


qioerr qio_channel_scan_complex(const int threadsafe, qio_channel_t* restrict ch, void* restrict re_out, void* restrict im_out, size_t len)
{
  int32_t chr;
  qioerr err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  //mark_offset = qio_channel_offset_unlocked(ch);

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  // ignore any initial whitespace.
  err = _getc_after_whitespace(ch, &chr);
  if( err ) goto rewind;

  if( chr == '(' && ch->style.complex_style == (QIO_COMPLEX_FORMAT_READ_STRICT | QIO_COMPLEX_FORMAT_ABI) ) {
    QIO_GET_CONSTANT_ERROR(err, EFORMAT, "missing ( when reading (#.#,#.#) complex");
    goto rewind;
  }

  if( chr == '(' ) {
    // read in the parens-style.
    err = qio_channel_scan_float(false, ch, re_out, len);
    if( err ) goto rewind;

    err = _getc_after_whitespace(ch, &chr);
    if( err ) goto rewind;

    if( chr != ',' ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "missing , when reading (#.#,#.#) complex");
      goto rewind;
    }

    err = qio_channel_scan_float(false, ch, im_out, len);
    if( err ) goto rewind;

    err = _getc_after_whitespace(ch, &chr);
    if( err ) goto rewind;

    if( chr != ')' ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "missing ) when reading (#.#,#.#) complex");
      goto rewind;
    }
  } else {
    int64_t start;
    int64_t after_real;
    int64_t after_nextc;

    // We'll skip the whitespace again...
    qio_channel_revert_unlocked(ch);

    err = qio_channel_mark(false, ch);
    if( err ) goto unlock;

    start = qio_channel_offset_unlocked(ch);

    // read the real part
    err = qio_channel_scan_float(false, ch, re_out, len);
    if( err ) goto rewind;

    after_real = qio_channel_offset_unlocked(ch);

    err = _getc_after_whitespace(ch, &chr);
    if( err ) goto rewind;

    after_nextc = qio_channel_offset_unlocked(ch);

    // is the character after the real number an i?
    if( chr == 'i' && after_real + 1 == after_nextc ) {
      switch ( len ) {
        case 8:
          *(double*) im_out = *(double*)re_out;
          *(double*) re_out = 0.0;
          break;
        case 4:
          *(float*) im_out = *(float*)re_out;
          *(float*) re_out = 0.0;
          break;
      }
    } else if( chr == '+' || chr == '-' ) {
      err = qio_channel_scan_float(false, ch, im_out, len);
      if( chr == '-' ) {
        switch ( len ) {
          case 8:
            *(double*) im_out = - *(double*) im_out;
            break;
          case 4:
            *(float*) im_out = - *(float*) im_out;
            break;
          default:
            QIO_GET_CONSTANT_ERROR(err, EINVAL, "bad floating point type");
        }
      }
      if( err ) goto rewind;

      // Read the 'i'
      //err = _getc_after_whitespace(ch, &chr); -- this would allow space before the i
      err = qio_channel_read_char(false, ch, &chr);
      if( err ) goto rewind;
      if( chr != 'i' ) {
        QIO_GET_CONSTANT_ERROR(err, EFORMAT, "missing i when reading #.# + #.#i complex");
        goto rewind;
      }
    } else {
      // One element complex. Could just be real, or could be nan.
      // If the real part is nan, copy it to the imaginary part.
      // Otherwise, set the imaginary part to 0.
      switch ( len ) {
        case 8:
          if( isnan(*(double*) re_out) ) *(double*) im_out = *(double*) re_out;
          else *(double*) im_out = 0.0;
          err = 0;
          break;
        case 4:
          if( isnan(*(float*) re_out) ) *(float*) im_out = *(float*) re_out;
          else *(float*) im_out = 0.0;
          err = 0;
          break;
        default:
          QIO_GET_CONSTANT_ERROR(err, EINVAL, "bad floating point type");
      }

      // unget that character.
      qio_channel_revert_unlocked(ch);
      qio_channel_advance_unlocked(ch, after_real - start);
      goto unlock;
    }
  }


rewind:
  if( err ) {
    qio_channel_revert_unlocked(ch);
    // Clear the number.
    switch ( len ) {
      case 8:
        *(double*) re_out = 0;
        *(double*) im_out = 0;
        break;
      case 4:
        *(float*) re_out = 0;
        *(float*) im_out = 0;
        break;
    }
  } else {
    qio_channel_commit_unlocked(ch);
  }

unlock:
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }
  return err;
}

qioerr qio_channel_skip_past_newline(const int threadsafe, qio_channel_t* restrict ch, int skipOnlyWs)
{
  int32_t c = 0;
  qioerr err;
  int needs_backup = 0;
  int64_t lastpos;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  if( skipOnlyWs ) {
    err = qio_channel_mark(false, ch);
    if( err ) goto unlock;
  }

  while( 1 ) {
    lastpos = qio_channel_offset_unlocked(ch);
    err = qio_channel_read_char(threadsafe, ch, &c);
    if( err  || c == '\n' ) break;
    if( skipOnlyWs && ! iswspace(c) ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "encountered non-whitespace");
      needs_backup = 1; // we have to unread this character.
      break;
    }
  }

  if( skipOnlyWs ) {
    if( needs_backup ) {
      // unread the not-newline not-whitespace.
      qio_channel_revert_unlocked(ch);
      qio_channel_advance_unlocked(ch, lastpos-qio_channel_offset_unlocked(ch));
    } else {
      // No need to unread anything.
      qio_channel_commit_unlocked(ch);
    }
  }

  _qio_channel_set_error_unlocked(ch, err);
unlock:
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }
  return err;
}

qioerr qio_channel_write_newline(const int threadsafe, qio_channel_t* restrict ch)
{
  char c = '\n';
  return qio_channel_write_amt(threadsafe, ch, &c, 1);
}

static qioerr maybe_left_pad(qio_channel_t* restrict ch, int gotsize)
{
  qioerr err = 0;
  // left justify == pad on the right
  // right justify == pad on the left!
  if( ! ch->style.leftjustify ) {
    while( gotsize < ch->style.min_width_columns && !err ) {
      err = qio_channel_write_char(false, ch, ch->style.pad_char);
      gotsize++;
    }
  }
  return err;
}
static qioerr maybe_right_pad(qio_channel_t* restrict ch, int gotsize)
{
  qioerr err = 0;
  if( ch->style.leftjustify ) {
    while( gotsize < ch->style.min_width_columns && !err ) {
      err = qio_channel_write_char(false, ch, ch->style.pad_char);
      gotsize++;
    }
  }
  return err;
}


qioerr qio_channel_print_complex(const int threadsafe,
                                 qio_channel_t* restrict ch,
                                 const void* restrict re_ptr,
                                 const void* restrict im_ptr,
                                 size_t len)
{
  char* re_buf = NULL;
  char* im_buf = NULL;
  char re_buf_onstack[MAX_ON_STACK];
  char im_buf_onstack[MAX_ON_STACK];
  int re_max = sizeof(re_buf_onstack); // would be nice if > MAX_DOUBLE_DIGITS;
  int im_max = sizeof(im_buf_onstack);
  double re_num = 0.0;
  double im_num = 0.0;
  int re_got, im_got;
  qioerr err;
  int im_neg = 0;
  int re_isnan = 0;
  int im_isnan = 0;
  style_char_t pos_char;
  style_char_t neg_char;
  qio_style_t* style;
  int save_show_plus;
  int save_min_columns;
  style_char_t save_pad_char;
  int width;
  int base;
  bool imag_needs_i = false;
  int extra = 0;

  // Read our numbers.
  err = 0;
  switch (len) {
    case 4:
      re_num = *(float*)re_ptr;
      im_num = *(float*)im_ptr;
      break;
    case 8:
      re_num = *(double*)re_ptr;
      im_num = *(double*)im_ptr;
      break;
    default:
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "bad floating point type");
  }

  re_isnan = isnan(re_num);
  im_isnan = isnan(im_num);

  // Lock before reading any style information from the
  // channel.
  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  style = &ch->style;
  base = style->base;
  if( base == 0 ) base = 10;
  save_show_plus = style->showplus;
  save_min_columns = style->min_width_columns;
  save_pad_char = ch->style.pad_char;
  // clear min_width_columns so ftoa doesn't include the padding.
  style->min_width_columns = 0;

  // force pad char to not be 0 since e.g. 0000(1,3) is a bad idea.
  // We could emit e.g. 00001+2i for format ABI but it would complicate
  // this routine to handle -0001+2i. And arguably for zero padding
  // you might want to split the zeros among the two components. For now
  // we just force it not to happen.
  if( ch->style.pad_char == '0' ) ch->style.pad_char = ' ';

  pos_char = ch->style.positive_char;
  neg_char = ch->style.negative_char;

  if( (style->complex_style & QIO_COMPLEX_FORMAT_PART) == QIO_COMPLEX_FORMAT_ABI ) {
    // For the a+bi format, instead of emitting a+-bi, always
    // compute the string for |b| and then fold replace the original
    // + with a - if necessary (ie so we get a-bi).
    if( signbit(im_num) ) {
      // num = - num;
      im_num = copysign(im_num, 1.0);
      im_neg = 1;
    } else {
      im_neg = 0;
    }
  }

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  // convert the number - written as two local loops

  // convert the real part
  while( 1 ) {
    // This loop should only ever run twice, since ftoa should
    // return the size needed the first time.
    MAYBE_STACK_ALLOC(char, re_max, re_buf, re_buf_onstack);
    if( ! re_buf ) {
      err = QIO_ENOMEM;
      goto rewind;
    }

    // Convert re_num8 into digits in re_buf
    re_got = _ftoa(re_buf, re_max, re_num, base, false, style, &extra);
    if( re_got < 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "error in floating point to string conversion");
      goto rewind;
    } else if( re_got + extra  < re_max ) {
      break;
    } else {
      // Not enough room... try again. 
      MAYBE_STACK_FREE(re_buf, re_buf_onstack);
      re_buf = NULL;
      re_max = re_got + extra + 1;
    }
  }

  // convert the imaginary part
  if( (style->complex_style & QIO_COMPLEX_FORMAT_PART) == QIO_COMPLEX_FORMAT_ABI ) {
    style->showplus = 0; // never put a + or - before the imag part.
    imag_needs_i = true;
  }
  while( 1 ) {
    // This loop should only ever run twice, since ftoa should
    // return the size needed the first time.
    MAYBE_STACK_ALLOC(char, im_max, im_buf, im_buf_onstack);
    if( ! im_buf ) {
      err = QIO_ENOMEM;
      goto rewind;
    }

    // Convert im_num8 into digits in im_buf
    im_got = _ftoa(im_buf, im_max, im_num, base, imag_needs_i, style, &extra);
    if( im_got < 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EFORMAT, "error in floating point to string conversion");
      goto rewind;
    } else if( im_got + extra < im_max ) {
      break;
    } else {
      // Not enough room... try again. 
      MAYBE_STACK_FREE(im_buf, im_buf_onstack);
      im_buf = NULL;
      im_max = im_got + extra + 1;
    }
  }

  // Put the minimum width columns back so maybe_left_pad etc use it.
  style->min_width_columns = save_min_columns;

  if( (style->complex_style & QIO_COMPLEX_FORMAT_PART) == QIO_COMPLEX_FORMAT_ABI ) {
    // If real or imag part is NAN, just write one NAN number.
    if( re_isnan ) {
      err = qio_channel_print_float(false, ch, re_ptr, len);
      if( err ) goto rewind;
    } else if( im_isnan ) {
      err = qio_channel_print_float(false, ch, im_ptr, len);
      if( err ) goto rewind;
    } else {
      // write a + bi
      width = re_got + im_got + 3;
      err = maybe_left_pad(ch, width);
      if( err ) goto rewind;
      err = qio_channel_write_amt(false, ch, re_buf, re_got);
      if( err ) goto rewind;
      err = qio_channel_write_char(false, ch, ' ');
      if( err ) goto rewind;
      err = qio_channel_write_char(false, ch, im_neg?neg_char:pos_char);
      if( err ) goto rewind;
      err = qio_channel_write_char(false, ch, ' ');
      if( err ) goto rewind;
      err = qio_channel_write_amt(false, ch, im_buf, im_got);
      if( err ) goto rewind;
      err = maybe_right_pad(ch, width);
      if( err ) goto rewind;
    }
  } else if( (style->complex_style & QIO_COMPLEX_FORMAT_PART) == QIO_COMPLEX_FORMAT_PARENS ) {
    // write (a,b)
    width = re_got + im_got + 3;
    err = maybe_left_pad(ch, width);
    if( err ) goto rewind;
    err = qio_channel_write_char(false, ch, '(');
    if( err ) goto rewind;
    err = qio_channel_write_amt(false, ch, re_buf, re_got);
    if( err ) goto rewind;
    err = qio_channel_write_char(false, ch, ',');
    if( err ) goto rewind;
    err = qio_channel_write_amt(false, ch, im_buf, im_got);
    if( err ) goto rewind;
    err = qio_channel_write_char(false, ch, ')');
    if( err ) goto rewind;
    err = maybe_right_pad(ch, width);
    if( err ) goto rewind;
  } else {
    QIO_GET_CONSTANT_ERROR(err, EINVAL, "unknown complex format");
    goto rewind;
  }

rewind:
  // Release the buffers used to format the real and imaginary parts.
  MAYBE_STACK_FREE(re_buf, re_buf_onstack);
  MAYBE_STACK_FREE(im_buf, im_buf_onstack);

  // Revert the channel on error; otherwise, commit the change.
  if( err ) {
    qio_channel_revert_unlocked(ch);
  } else {
    qio_channel_commit_unlocked(ch);
  }

unlock:
  style->showplus = save_show_plus;
  style->min_width_columns = save_min_columns;
  style->pad_char = save_pad_char;
  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}


qioerr _qio_channel_read_char_slow_unlocked(qio_channel_t* restrict ch, int32_t* restrict chr) {
  qioerr err=0;
  int32_t gotch;
  uint32_t codepoint=0, state;

  if( qio_glocale_utf8 == QIO_GLOCALE_UTF8 ) {
    /* This decoder was written and tested... but it doesn't
     * check for all the UTF-8 miscodings... so using a DFA version.
     *
    // #1           00000000 0xxxxxxx <-> 0xxxxxxx
    // #2           00000yyy yyxxxxxx <-> 110yyyyy 10xxxxxx
    // #3           zzzzyyyy yyxxxxxx <-> 1110zzzz 10yyyyyy 10xxxxxx
    // #4  000uuuzz zzzzyyyy yyxxxxxx <-> 11110uuu 10zzzzzz 10yyyyyy 10xxxxxx

    // We always read 1 character at least.
    gotch = qio_channel_read_byte(false, ch);

    if( gotch >= 0 ) {
      if( gotch < 0x80 ) { // starts 0
        // OK, we got a 1-byte character; case #1
      } else if( gotch < 0xc2 ) { // starts 10
        // It's a misplaced continuation character.
        gotch = - EILSEQ;
      } else if( gotch < 0xe0 ) { // starts 110
        // We read this and one byte; case #2
        tmp1 = qio_channel_read_byte(false, ch);
        if( tmp1 > 0 && (tmp1 & 0xc0) == 0x80 ) {
          gotch = ((((unsigned int) gotch) & 0x1f) << 6) |
                  (((unsigned int)tmp1) & 0x3f);
        } else {
          if( tmp1 == - EEOF ) gotch = - ESHORT; // error code
          else if( tmp1 < 0 ) gotch = tmp1; // error code
          else gotch = - EILSEQ; // must have 10 after 110
        }
      } else if( gotch < 0xf0 ) {
        // Read this and two bytes; case #3
        tmp1 = qio_channel_read_amt(false, ch, tmp, 2);
        if (tmp1 == 0 &&
            (tmp[0] & 0xc0) == 0x80 &&
            (tmp[1] & 0xc0) == 0x80 ) {
          gotch = ((((unsigned int) gotch) & 0x0f) << 12) |
                  ((((unsigned int)tmp[0]) & 0x3f) << 6 ) |
                  (((unsigned int)tmp[1]) & 0x3f);

        } else {
          if( tmp1 == EEOF ) gotch = - ESHORT; // error code
          else if( tmp1 ) gotch = - tmp1; // error code.
          else gotch = -EILSEQ;
        }
      } else {
        // Read this and three bytes; case #4
        tmp1 = qio_channel_read_amt(false, ch, tmp, 3);
        if( tmp1 != 0 ) gotch = - tmp1; // error code.
        if( tmp1 == 0 &&
            (tmp[0] & 0xc0) == 0x80 && 
            (tmp[1] & 0xc0) == 0x80 && 
            (tmp[2] & 0xc0) == 0x80 ) {
          gotch = ((((unsigned int) gotch) & 0x07) << 18) |
                  ((((unsigned int)tmp[0]) & 0x3f) << 12 ) |
                  ((((unsigned int)tmp[1]) & 0x3f) << 6 ) |
                  (((unsigned int)tmp[2]) & 0x3f);
        } else {
          if( tmp1 == EEOF ) gotch = -ESHORT; // error code
          else if( tmp1 != 0 ) gotch = - tmp1; // error code
          else gotch = -EILSEQ;
        }
      }
    }

    if( gotch < 0 ) {
      *chr = 0xfffd;
      err = -gotch;
    } else {
      *chr = gotch;
      err = 0;
    }
    */

    state = 0;

    // Slow path.
    while( 1 ) {
      gotch = qio_channel_read_byte(false, ch);
      if(gotch < 0 ||
         chpl_enc_utf8_decode(&state,
                              &codepoint,
                              gotch) <= 1){
        break;
      }
    }
    if( gotch >= 0 && state == UTF8_ACCEPT ) {
      *chr = codepoint;
      err = 0;
    } else if( gotch < 0 ) {
      *chr = -1; // ie like EOF.
      err = qio_int_to_err(-gotch);
    } else {
      *chr = 0xfffd; // replacement character
      QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
    }
  } else if( qio_glocale_utf8 == QIO_GLOCALE_ASCII ) {
    // character == byte.
    gotch = qio_channel_read_byte(false, ch);
    if( gotch < 0 ) {
      err = qio_int_to_err(-gotch);
      *chr = -1;
    } else {
      *chr = gotch;
      err = 0;
    }
  } else {
#ifdef HAS_WCTYPE_H
    mbstate_t ps;
    size_t got=0;
    char mb;
    wchar_t tmp_chr;

    // Use C functions, probably not UTF-8.
    memset(&ps, 0, sizeof(mbstate_t));

    // Fast path: an entire multi-byte sequence
    // is stored in the buffers.
    if( qio_space_in_ptr_diff(MB_LEN_MAX, ch->cached_end, ch->cached_cur) ) {
      got = mbrtowc(&tmp_chr, ch->cached_cur, MB_LEN_MAX, &ps);
      if( got == 0 ) {
        *chr = 0;
      } else if( got == (size_t) -1 || got == (size_t) -2 ) {
        // it contains an invalid multibyte sequence.
        // or it claims we don't have a complete character
        // (even though we had MB_LEN_MAX!).
        // errno should be EILSEQ.
        *chr = -3; // invalid character... think 0xfffd for unicode
        QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
      } else {
        *chr = tmp_chr;
        err = 0;
        ch->cached_cur = qio_ptr_add(ch->cached_cur,got);
      }
    } else {
      // Slow path: we might need to read 1 byte at a time.

      while( 1 ) {
        // We always read 1 character at least.
        gotch = qio_channel_read_byte(false, ch);
        if( gotch < 0 ) {
          err = qio_int_to_err(-got);
          *chr = -1;
          break;
        }
        mb = gotch;

        got = mbrtowc(&tmp_chr, &mb, 1, &ps);
        if( got == 0 ) {
          // We read a NUL.
          *chr = 0;
          err = 0;
          break;
        } else if( got == 1 ) {
          // OK!
          *chr = tmp_chr;
          err = 0;
          break;
        } else if( got == (size_t) -1 ) {
          // it contains an invalid multibyte sequence.
          // errno should be EILSEQ.
          *chr = -3; // invalid character... think 0xfffd for unicode
          QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
          break;
        } else if( got == (size_t) -2 ) {
          // continue as long as we have an incomplete char.
        }
      }
    }
#else
    err = ENOSYS;
#endif
  }

  return err;
}

c_string qio_encode_to_string(int32_t chr)
{
  int nbytes;
  char* buf;
  qioerr err;

  nbytes = qio_nbytes_char(chr);
  if( nbytes == 0 ) {
    return NULL;
  }
  
  buf = qio_malloc(nbytes + 1);
  if( ! buf ) return NULL;

  err = qio_encode_char_buf(buf, chr);
  if( err ) {
    qio_free(buf);
    return NULL;
  }

  buf[nbytes] = '\0';
  return buf;
}

qioerr _qio_channel_write_char_slow_unlocked(qio_channel_t* restrict ch, int32_t chr) {
  char mbs[MB_LEN_MAX];
  qioerr err;

  if( qio_glocale_utf8 == QIO_GLOCALE_UTF8 ) {
    // #1           00000000 0xxxxxxx <-> 0xxxxxxx
    // #2           00000yyy yyxxxxxx <-> 110yyyyy 10xxxxxx
    // #3           zzzzyyyy yyxxxxxx <-> 1110zzzz 10yyyyyy 10xxxxxx
    // #4  000uuuzz zzzzyyyy yyxxxxxx <-> 11110uuu 10zzzzzz 10yyyyyy 10xxxxxx

    if( chr < 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
    } else if( chr < 0x80 ) {
      // OK, we got a 1-byte character; case #1
      err = qio_channel_write_byte(false, ch, chr);
    } else if( chr < 0x800 ) {
      // OK, we got a fits-in-2-bytes character; case #2
      mbs[0] = (0xc0 | (chr >> 6));
      mbs[1] = (0x80 | (chr & 0x3f));

      err = qio_channel_write_amt(false, ch, mbs, 2);
    } else if( chr < 0x10000 ) {
      // OK, we got a fits-in-3-bytes character; case #3
      mbs[0] = (0xe0 | (chr >> 12));
      mbs[1] = (0x80 | ((chr >> 6) & 0x3f));
      mbs[2] = (0x80 | (chr & 0x3f));

      err = qio_channel_write_amt(false, ch, mbs, 3);
    } else {
      // OK, we got a fits-in-4-bytes character; case #4
      mbs[0] = (0xf0 | (chr >> 18));
      mbs[1] = (0x80 | ((chr >> 12) & 0x3f));
      mbs[2] = (0x80 | ((chr >> 6) & 0x3f));
      mbs[3] = (0x80 | (chr & 0x3f));

      err = qio_channel_write_amt(false, ch, mbs, 4);
    }
  } else if( qio_glocale_utf8 == QIO_GLOCALE_ASCII ) {
    err = qio_channel_write_byte(false, ch, chr);
  } else {
#ifdef HAS_WCTYPE_H
    mbstate_t ps;
    size_t got;
    memset(&ps, 0, sizeof(mbstate_t));
    got = wcrtomb(mbs, chr, &ps);
    if( got == (size_t) -1 ) {
      QIO_GET_CONSTANT_ERROR(err, EILSEQ, "");
    } else {
      err = qio_channel_write_amt(false, ch, mbs, got);
    }
#else
    QIO_GET_CONSTANT_ERROR(err, ENOSYS, "missing wctype.h");
#endif
  }

  return err;
}





// This could be changed to use tables, but the current version
// favors simplicity&flexibility of implementation over speed.
static const char* space = " \f\n\r\t\v";
/*static const char* bin_num = "01";
static const char* oct_num = "01234567";*/
static const char* dec_num = "0123456789";
/*
static const char* lower_hex_num = "0123456789abcdef";
static const char* upper_hex_num = "0123456789ABCDEF";
static const char* hex_num = "0123456789ABCDEFabcdef";
static const char* pound_conv = "#.";*/
static const char* lit_chars = "% \f\n\r\t\v";
static const char* regexp_flag_chars = "imsU";

static inline int istype(char x, const char* allow)
{
  while( *allow ) {
    if( *allow == x ) return 1;
    allow++;
  }
  return 0;
}

void qio_conv_destroy(qio_conv_t* spec)
{
  // Don't need to free literal because
  // it points into format string
  spec->literal = NULL;
}

void qio_conv_init(qio_conv_t* spec_out)
{
  memset(spec_out, 0, sizeof(qio_conv_t));
}

int _qio_regexp_flags_then_rcurly(const char* ptr, int * len);

int _qio_regexp_flags_then_rcurly(const char* ptr, int * len)
{
  int i = 0;
  while( ptr[i] ) {
    // '{' don't confuse vi
    if( ptr[i] == '}' ) {
      *len = i;
      return 1;
    }
    if( ! istype(ptr[i], regexp_flag_chars) ) break;
    i++;
  }
  *len = i;
  return 0;
}


qioerr qio_conv_parse(c_string fmt,
                      size_t start,
                      uint64_t* end,
                      int scanning,
                      qio_conv_t* spec_out,
                      qio_style_t* style_out)
{
  size_t i;
  int in_group = 0;
  char* endptr;
  long int val;
  // For these, we set them to -2 if they are in an argument
  // and -1 if they have not been set.
#define WIDTH_IN_ARG -2
#define WIDTH_NOT_SET -1
  long int width = WIDTH_NOT_SET;
  long int precision = WIDTH_NOT_SET;
  int at_flag = 0;
  int zero_flag = 0;
  int minus_flag = 0;
  int space_flag = 0;
  int plus_flag = 0;
  int sloppy_flag = 0;
  char base_flag = 0;
  char specifier = 0;
  char binary = 0;
  char exponential = 0;
  char S_encoding = 0;
  qioerr err = 0;
  int after_percent = 0;

  qio_conv_init(spec_out);
  qio_style_init_default(style_out);
  style_out->prefix_base = 0;
  style_out->byteorder = QIO_NATIVE;
  style_out->showpointzero = 0;

  i = start;

  // do we have a ####.#### conversion to match?
  if( fmt[i] == '%' && fmt[i+1] == '{' && 
      (fmt[i+2] == '#' || (fmt[i+2] == '.' && fmt[i+3] == '#') ) ) {
    // handle %{####} conversions
    size_t num_before, num_after, period;

    in_group = 1;
    i++; // pass %
    i++; // pass {

    num_before = 0;
    num_after = 0;
    period = 0;
    
    // how many ### do we have before a . ?
    for( ; fmt[i] == '#'; i++ ) num_before++;

    if( fmt[i] == '.' ) {
      i++; // pass '.'
      period = 1;
      for( ; fmt[i] == '#'; i++ ) num_after++;
    }

    if( in_group ) {
      if( fmt[i] != '}' ) {
        QIO_GET_CONSTANT_ERROR(err, EINVAL, "Bad %{###.###} format group");
      } else {
        i++; // pass }
      }
    }

    spec_out->argType = QIO_CONV_ARG_TYPE_NUMERIC;

    style_out->base = 10;
    style_out->pad_char = ' ';
    style_out->precision = num_after;
    style_out->min_width_columns = num_before + period + num_after;
    style_out->realfmt = 1; // like Chapel %dr or C %f
    if( period && num_after == 0 ) {
      // ie ##.
      style_out->showpoint = 1;
    }
    goto done;
  }

  // do we have a % conversion to match?
  if( fmt[i] == '%' ) {
    i++; // pass %

    if( fmt[i] == '{' ) {
      i++;
      in_group = 1;
    }

    after_percent = i;

    if( fmt[i] == '%' ) {
      i++; // pass %
      spec_out->argType = QIO_CONV_ARG_TYPE_NONE_LITERAL;
      spec_out->literal_is_whitespace = 0;
      spec_out->literal_length = 1;
      spec_out->literal = (int8_t*) "%";
      goto done;  
    }

    // Are we working with binary?
    if( fmt[i] == '<' || fmt[i] == '|' || fmt[i] == '>' ) {
      binary = fmt[i];
      i++;
    } 

    // Read some flags.
    for( ; fmt[i]; i++ ) {
      /* Should we have something like these?
         *  # For a, A, e, E, f, F, g, and  G
              conversions,  the  result  will  always contain a decimal point,
              even if no digits follow it (normally, a decimal  point  appears
              in  the  results  of those conversions only if a digit follows).
              For g and G conversions, trailing zeros are not removed from the
              result  as  they would otherwise be.  For other conversions, the
              result is undefined.
         * ' means group with thousands grouping character
         */
      if( fmt[i] == '@' ) {
        at_flag = 1;
      } else if( fmt[i] == '0' ) {
        zero_flag = 1;
      } else if( fmt[i] == '-' ) {
        minus_flag = 1;
      } else if( fmt[i] == ' ' ) {
        space_flag = 1;
      } else if( fmt[i] == '+' ) {
        plus_flag = 1;
      } else if( fmt[i] == '~' ) {
        // ~ might one day mean allow non-quoted JSON field names
        // but it also means to skip JSON fields not in use.
        sloppy_flag = 1;
      } else {
        break;
      }
    }
    // a - overrides a 0 if both are given
    if( minus_flag ) zero_flag = 0;

    // Read the width. *S has different meaning.
    if( fmt[i] == '*' && fmt[i+1] != 'S') {
      i++;
      // next argument contains field width
      width = WIDTH_IN_ARG;
      spec_out->preArg1 = QIO_CONV_SET_IGNORE;
    } else if( istype(fmt[i], dec_num) ) {
      // OK, now fmt[i] is the start of the integer-width area.
      val = strtol(&fmt[i], &endptr, 10);
      if( endptr != &fmt[i] ) {
        // advance past whatever number we got
        i += endptr - &fmt[i];
        if( val < 0 || val == LONG_MIN || val == LONG_MAX ) {
          val = 0;
          // could do err = EINVAL; goto done;
        }
        width = val;
      }
    }
    // read the precision. *S has different meaning.
    if( fmt[i] == '.' && fmt[i+1] != 'S') {
      i++; // pass '.'
      if( fmt[i] == '*' ) {
        i++;
        // next argument contains precision
        precision = WIDTH_IN_ARG;
        spec_out->preArg2 = QIO_CONV_SET_IGNORE;
      } else if( istype(fmt[i], dec_num) ) {
        // OK, now fmt[i] is the start of the precision area.
        val = strtol(&fmt[i], &endptr, 10);
        if( endptr != &fmt[i] ) {
          // advance past whatever number we got
          i += endptr - &fmt[i];
          if( val < 0 || val == LONG_MIN || val == LONG_MAX ) {
            val = 0;
          }
          precision = val;
        }
      }
    }


    // Read a base flag
    if( istype(fmt[i], "bdxXjho'\"") ) {
      base_flag = fmt[i];
      i++;
    }

    // Read an exponent flag
    if( fmt[i] == 'e' || fmt[i] == 'E') {
      exponential = fmt[i];
      i++;
    }

    // Read a specifier character
    if( istype(fmt[i], "ntiurmzs/cS") ) {
      specifier = fmt[i];
      if( fmt[i] == 'S' ) {
        // handle numbers parsed as width for e.g. %|0S
        if( i > after_percent ) S_encoding = fmt[i-1];
        else S_encoding = '"'; // default to double-quotes.
      }
      i++;
    } else if( fmt[i] && fmt[i+1] == 'S' ) {
      specifier = 'S';
      S_encoding = fmt[i];
      i++; // pass e.g. v
      i++; // pass S
    }

    if( specifier == 0 ) {
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "Could not find the end of a conversion specifier");
      goto done;
    }


    // Consume the width, precision, flags and base arguments,
    // updating the style appropriately.
    if( binary ) {
      // Binary conversions silently consume space characters after the
      // conversion.
      while( fmt[i] == ' ' ) i++;
      
      // Do Binary.
      style_out->binary = 1;

      // Handle endianness flags
      if( binary == '|' ) style_out->byteorder = QIO_NATIVE;
      else if( binary == '<' ) style_out->byteorder = QIO_LITTLE;
      else if( binary == '>' ) style_out->byteorder = QIO_BIG;

      if( specifier == 't' ) {
        // Does nothing at all with width.
        spec_out->argType = QIO_CONV_ARG_TYPE_REPR;
      } else if( specifier == 'n' ) {
        spec_out->argType = QIO_CONV_ARG_TYPE_NUMERIC;
      } else if( specifier == 'i' || specifier == 'u' ||
                 specifier == 'r' || specifier == 'm' || specifier == 'z') {
        // Handle width. Precision doesn't do anything for these.
        if( width != WIDTH_NOT_SET ) {
          if( width == WIDTH_IN_ARG ) {
            spec_out->preArg1 = QIO_CONV_SET_MAX_WIDTH_BYTES;
          } else {
            style_out->max_width_bytes = width;
          }
        } else {
          // Width is required for binary conversions of these types.
          QIO_GET_CONSTANT_ERROR(err, EINVAL, "Width is required for binary numeric conversions");
          goto done;
        }

        // Set the conversion type.
        if( specifier == 'i' ) {
          spec_out->argType = QIO_CONV_ARG_TYPE_BINARY_SIGNED;
        } else if( specifier == 'u' ) {
          spec_out->argType = QIO_CONV_ARG_TYPE_BINARY_UNSIGNED;
        } else if( specifier == 'r' ) {
          spec_out->argType = QIO_CONV_ARG_TYPE_BINARY_REAL;
        } else if( specifier == 'm' ) {
          spec_out->argType = QIO_CONV_ARG_TYPE_BINARY_IMAG;
        } else if( specifier == 'z' ) {
          spec_out->argType = QIO_CONV_ARG_TYPE_BINARY_COMPLEX;
        }
      } else if( specifier == 's' || specifier == 'S') {
        char type = 's';
        if( S_encoding ) type = S_encoding;

        // Handle width. Precision doesn't do anything for %s
        // If there's no other encoding info, width is exact str len 
        if( width != WIDTH_NOT_SET ) {
          if( width == WIDTH_IN_ARG ) {
            if( type == 's' ) spec_out->preArg1 = QIO_CONV_SET_STRINGLEN;
            else spec_out->preArg1 = QIO_CONV_SET_MAX_WIDTH_BYTES;
          } else {
            if( type == 's' ) style_out->str_style = width;
            else style_out->max_width_bytes = width;
          }
        }

        // s conversions without following encoding type must have a width.
        if( type == 's' && width == WIDTH_NOT_SET ) {
          QIO_GET_CONSTANT_ERROR(err, EINVAL, "Binary s conversion must have a width");
          goto done;
        }

        if( type == 's' ) ; // OK
        else if( type == '0' ) style_out->str_style = QIO_STRSTYLE_NULL_TERMINATED;
        else if( type == 'v' ) style_out->str_style = QIO_STRSTYLE_VLEN; // variable length
        else if( type == '1' ) style_out->str_style = -1; // 1b length before
        else if( type == '2' ) style_out->str_style = -2; // 2b length before
        else if( type == '4' ) style_out->str_style = -4; // 4b length before
        else if( type == '8' ) style_out->str_style = -8; // 8b length before
        else if( type == '*' ) {
          style_out->str_style = QIO_STRSTYLE_NULL_TERMINATED;
          // need to overwrite str_style
          spec_out->preArg3 = QIO_CONV_SET_TERMINATOR;
        } else {
          QIO_GET_CONSTANT_ERROR(err, EINVAL, "Unknown binary %S conversion");
        }

        spec_out->argType = QIO_CONV_ARG_TYPE_BINARY_STRING;
      } else {
        QIO_GET_CONSTANT_ERROR(err, EINVAL, "Unknown binary conversion");
      }
    } else if( istype(specifier, "niurmz" ) ) {
      // For numeric conversions
      
      // Handle width and precision
      if( width != WIDTH_NOT_SET ) {
        // These settings have no effect when scanning
        if( width == WIDTH_IN_ARG ) {
          spec_out->preArg1 = QIO_CONV_SET_MIN_WIDTH_COLS;
        } else {
          style_out->min_width_columns = width;
        }
      }
      if( precision != WIDTH_NOT_SET ) {
        // These settings have no effect when scanning
        if( precision == WIDTH_IN_ARG ) {
          spec_out->preArg2 = QIO_CONV_SET_PRECISION;
        } else {
          style_out->precision = precision;
        }
      }

      // Handle flags
      if( at_flag ) {
        if( specifier != 'z' ) style_out->prefix_base = 1;
        else style_out->complex_style = QIO_COMPLEX_FORMAT_PARENS;
      }
      if( zero_flag ) style_out->pad_char = '0';
      if( minus_flag ) style_out->leftjustify = 1;
      if( space_flag ) style_out->showplus = 2;
      if( plus_flag ) style_out->showplus = 1;

      if( base_flag == 'b' ) style_out->base = 2;
      else if( base_flag == 'o' ) style_out->base = 8;
      else if( base_flag == 'd' ) style_out->base = 10;
      else if( base_flag == 'x' ) {
        style_out->base = 16;
        style_out->uppercase = 0;
      } else if( base_flag == 'X' ) {
        style_out->base = 16;
        style_out->uppercase = 1;
      } else if( base_flag == 'j' || base_flag == 'h') {
        style_out->base = 10;
        style_out->realfmt = 2;
        style_out->pad_char = ' ';
      }
  
      // Handle real formats.
      if( specifier == 'r' || specifier == 'm' || specifier == 'z' ) {
        if( exponential == 'e' ) {
          //style_out->uppercase = 0; %Xe would be uppercase
          style_out->realfmt = 2;
        } else if( exponential == 'E' ) {
          style_out->uppercase = 1;
          style_out->realfmt = 2;
        } else if( base_flag == 'd' ) {
          style_out->realfmt = 1;
        } else {
          style_out->realfmt = 0;
        }
      }

      // Set the conversion type.
      if( specifier == 'n' ) {
        style_out->prefix_base = 1;
        spec_out->argType = QIO_CONV_ARG_TYPE_NUMERIC;
      }
      if( specifier == 'i' ) spec_out->argType = QIO_CONV_ARG_TYPE_SIGNED;
      if( specifier == 'u' ) spec_out->argType = QIO_CONV_ARG_TYPE_UNSIGNED;
      if( specifier == 'r' ) spec_out->argType = QIO_CONV_ARG_TYPE_REAL;
      if( specifier == 'm' ) spec_out->argType = QIO_CONV_ARG_TYPE_IMAG;
      if( specifier == 'z' ) spec_out->argType = QIO_CONV_ARG_TYPE_COMPLEX;
    } else if( specifier == 'c' ) {
      // width, precision, flags have no effect on %c,
      spec_out->argType = QIO_CONV_ARG_TYPE_CHAR;
    } else if( specifier == 's' || specifier == 'S' || specifier == '/' ) {
      // Either one could limit the number of bytes read (width)
      // or the number of characters read (precision)
      // (although the / conversion will ignore # chars read/precision)
     
      // Handle width and precision
      if( width != WIDTH_NOT_SET ) {
        if( width == WIDTH_IN_ARG ) {
          if( scanning ) spec_out->preArg1 = QIO_CONV_SET_MAX_WIDTH_BYTES;
          else spec_out->preArg1 = QIO_CONV_SET_MIN_WIDTH_COLS;
        } else {
          if( scanning ) style_out->max_width_bytes = width;
          else style_out->min_width_columns = width;
        }
      }
      if( precision != WIDTH_NOT_SET ) {
        if( precision == WIDTH_IN_ARG ) {
          if( scanning ) spec_out->preArg2 = QIO_CONV_SET_MAX_WIDTH_CHARS;
          else spec_out->preArg2 = QIO_CONV_SET_MAX_WIDTH_COLS;
        } else {
          if( scanning ) style_out->max_width_characters = precision;
          else style_out->max_width_columns = precision;
        }
      }

      if( specifier == 's' || specifier == 'S') {
        // Handle base flags modifying string format

        // Note -- when scanning, a precision should adjust the string
        // format to not end at a whitespace.
        // This mode scans a particular number of codepoints.
        if( scanning && precision != WIDTH_NOT_SET ) {
          style_out->string_format = QIO_STRING_FORMAT_TOEOF;
        }

        if( base_flag == 'j' ) {
          style_out->string_format = QIO_STRING_FORMAT_JSON;
        } else if( base_flag == 'h' ) {
          style_out->string_format = QIO_STRING_FORMAT_CHPL;
        }

        if( specifier == 'S' ) {
          style_out->string_format = QIO_STRING_FORMAT_BASIC;
          style_out->string_start = '"';
          style_out->string_end = '"';
        }

        if( minus_flag ) style_out->leftjustify = 1;

        // Handle %cS or %{*S*} or %{(S)} which sets quote characters
        if( specifier == 'S' ) {
          char pre = S_encoding;
          char post = 0;
          if( in_group && fmt[i] != '}') {
            post = fmt[i];
            i++;
          }

          if( pre && post == 0 ) {
            if( pre == '*' ) spec_out->preArg3 = QIO_CONV_SET_STRINGSTARTEND;
            else {
              style_out->string_start = pre;
              style_out->string_end = pre;
            }
          } else if( pre ) {
            if( pre == '*' ) spec_out->preArg3 = QIO_CONV_SET_STRINGSTARTEND;
            else style_out->string_start = pre;
          }

          if( post ) {
            if( post == '*' ) {
              QIO_GET_CONSTANT_ERROR(err, EINVAL, "Cannot specify end quote in argument after string");
              goto done;
            } else style_out->string_end = post;
          }
        }
        spec_out->argType = QIO_CONV_ARG_TYPE_STRING;
      } else if( specifier == '/' ) {
        // No base flags have any effect on RE search.
        if( fmt[i] == '*' && fmt[i+1] == '/') {
          // Next argument contains the RE to match
          i++; // pass *
          i++; // pass '/'
          spec_out->argType = QIO_CONV_ARG_TYPE_REGEXP;
        } else {
          // RE right here
          int start = i;
          int end;
          int flagslen = 0;
          int flagsstart = 0;
          spec_out->argType = QIO_CONV_ARG_TYPE_NONE_REGEXP_LITERAL;
          // read until '/' or, if we are in a group,
          // from the '%{/' to the '/}'
          if( in_group ) {
            int curlydepth = 0;
            while( true ) {
              if( fmt[i] == 0 ) {
                QIO_GET_CONSTANT_ERROR(err, EINVAL, "Found { but missing } in conversion");
                goto done;
              }
              if( fmt[i-1] != '\\' && fmt[i] == '{' ) curlydepth++;
              if( fmt[i-1] != '\\' && fmt[i] == '}' ) curlydepth--;
              if( curlydepth < 0 ) {
                QIO_GET_CONSTANT_ERROR(err, EINVAL, "Missing { for this } in conversion");
                goto done;
              }
              if( curlydepth == 0 &&
                  fmt[i-1] != '\\' &&
                  fmt[i] == '/' &&
                  _qio_regexp_flags_then_rcurly(&fmt[i+1], &flagslen) ) {
                // OK!
                break;
              }
              i++;
            }
            end = i;
            flagsstart = i + 1; // past / ending re
            i = flagsstart + flagslen; // at closing { } 
          } else {
            while( fmt[i-1] == '\\' || fmt[i] != '/' ) {
              if( fmt[i] == 0 ) {
                QIO_GET_CONSTANT_ERROR(err, EINVAL, "Unterminated regular expression pattern - missing closing /");
                goto done;
              }
              i++;
            }
            end = i;
            i++; // pass the final '/'
          }
          // Now, from start...i is the literal regexp.
          // and from i+1..flagslen is the flags
          //  (if flagslen > 0)
          spec_out->regexp_length = end - start;
          spec_out->regexp = (int8_t*) &fmt[start];
          spec_out->regexp_flags_length = flagslen;
          spec_out->regexp_flags = (int8_t*) &fmt[flagsstart];
        }
      }
    } else if( specifier == 't' ) {
      style_out->base = 10;
      style_out->pad_char = ' ';
      style_out->realfmt = 2;
      style_out->string_format = QIO_STRING_FORMAT_CHPL;

      // Handle precision
      if( precision != WIDTH_NOT_SET ) {
        // These settings have no effect when scanning
        if( precision == WIDTH_IN_ARG ) {
          spec_out->preArg2 = QIO_CONV_SET_PRECISION;
        } else {
          style_out->precision = precision;
        }
      }

      if( sloppy_flag ) {
        style_out->skip_unknown_fields = 1;
      }

      if( base_flag == 'j' ) {
        style_out->realfmt = 2;
        style_out->string_format = QIO_STRING_FORMAT_JSON;
        style_out->array_style = QIO_ARRAY_FORMAT_JSON;
        style_out->aggregate_style = QIO_AGGREGATE_FORMAT_JSON;
        style_out->tuple_style = QIO_TUPLE_FORMAT_JSON;
      } else if( base_flag == 'h' ) {
        style_out->realfmt = 2;
        style_out->string_format = QIO_STRING_FORMAT_CHPL;
        style_out->array_style = QIO_ARRAY_FORMAT_CHPL;
        style_out->aggregate_style = QIO_AGGREGATE_FORMAT_CHPL;
        style_out->tuple_style = QIO_TUPLE_FORMAT_CHPL;
        style_out->pad_char = ' ';
      } else if( base_flag == 'x' ) {
        style_out->prefix_base = 1;
        style_out->base = 16;
      } else if( base_flag == 'X' ) {
        style_out->prefix_base = 1;
        style_out->base = 16;
        style_out->uppercase = 1;
      } else if( base_flag == 'b' ) {
        style_out->prefix_base = 1;
        style_out->base = 2;
      } else if( base_flag == 'd' ) {
        style_out->prefix_base = 0;
        style_out->base = 10;
      } else if( base_flag == '\'' ) {
        style_out->string_format = QIO_STRING_FORMAT_BASIC;
        style_out->string_start = '\'';
        style_out->string_end = '\'';
      } else if( base_flag == '"' ) {
        style_out->string_format = QIO_STRING_FORMAT_BASIC;
        style_out->string_start = '"';
        style_out->string_end = '"';
      }
      spec_out->argType = QIO_CONV_ARG_TYPE_REPR;
    } else {
      QIO_GET_CONSTANT_ERROR(err, EINVAL, "Unknown text conversion");
    }

    if( in_group ) {
      // we must match '}' or there is an error.
      if( fmt[i] == '}' ) {
        i++;
      } else {
        QIO_GET_CONSTANT_ERROR(err, EINVAL, "Found { but missing } in conversion");
      }
    }
    goto done;
  }

  // do we have any whitespace to match?
  if( fmt[i] == '\n' ) {
    size_t len = 0;
    const char* lit = &fmt[i];
    // pass any whitespace characters.
    for( ; fmt[i] && istype(fmt[i], space); i++ ) len++;

    spec_out->argType = QIO_CONV_ARG_TYPE_NONE_LITERAL;
    spec_out->literal_is_whitespace = 2;
    spec_out->literal_length = len;
    spec_out->literal = (int8_t*) lit;
    goto done;
  } else if( istype(fmt[i], space) ) {
    size_t len = 0;
    const char* lit = &fmt[i];
    // pass any whitespace characters.
    for( ; fmt[i] && fmt[i] != '\n' && istype(fmt[i], space); i++ ) len++;

    spec_out->argType = QIO_CONV_ARG_TYPE_NONE_LITERAL;
    spec_out->literal_is_whitespace = 1;
    spec_out->literal_length = len;
    spec_out->literal = (int8_t*) lit;
    goto done;
  } else {
    // No other ideas -- it is a literal
    size_t len = 0;
    const char* lit = &fmt[i];
    // pass any whitespace characters.
    for( ; fmt[i] && !istype(fmt[i], lit_chars); i++ ) len++;

    spec_out->argType = QIO_CONV_ARG_TYPE_NONE_LITERAL;
    spec_out->literal_is_whitespace = 0;
    spec_out->literal_length = len;
    spec_out->literal = (int8_t*) lit;
    goto done;
  }

done:
  *end = i;
  return err;
}

qioerr qio_format_error_too_many_args(void)
{
  qioerr err;
  QIO_GET_CONSTANT_ERROR(err, EINVAL, "Too many arguments for format string");
  return err;
}

qioerr qio_format_error_too_few_args(void)
{
   qioerr err;
   QIO_GET_CONSTANT_ERROR(err, EINVAL, "Too few arguments for format string");
   return err;
}

qioerr qio_format_error_arg_mismatch(int64_t arg)
{
   qioerr err;
   if( arg == 0 ) {
     QIO_GET_CONSTANT_ERROR(err, EINVAL, "Argument type mismatch in argument 0");
   } else if( arg == 1 ) {
     QIO_GET_CONSTANT_ERROR(err, EINVAL, "Argument type mismatch in argument 1");
   } else if( arg == 2 ) {
     QIO_GET_CONSTANT_ERROR(err, EINVAL, "Argument type mismatch in argument 2");
   } else if( arg == 3 ) {
     QIO_GET_CONSTANT_ERROR(err, EINVAL, "Argument type mismatch in argument 3");
   } else if( arg == 4 ) {
     QIO_GET_CONSTANT_ERROR(err, EINVAL, "Argument type mismatch in argument 4");
   } else if( arg == 5 ) {
     QIO_GET_CONSTANT_ERROR(err, EINVAL, "Argument type mismatch in argument 5");
   } else if( arg == 6 ) {
     QIO_GET_CONSTANT_ERROR(err, EINVAL, "Argument type mismatch in argument 6");
   } else {
     QIO_GET_CONSTANT_ERROR(err, EINVAL, "Argument type mismatch in argument >6");
   }
   return err;
}

qioerr qio_format_error_bad_regexp(void)
{
   qioerr err;
   QIO_GET_CONSTANT_ERROR(err, EINVAL, "Bad regular expression in format string");
   return err;
}

qioerr qio_format_error_write_regexp(void)
{
   qioerr err;
   QIO_GET_CONSTANT_ERROR(err, EINVAL, "Regular expressions are not supported in writef format strings");
   return err;
}




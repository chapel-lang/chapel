
#ifndef SIMPLE_TEST
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
#endif


err_t qio_channel_read_uvarint(const int threadsafe, qio_channel_t* restrict ch, uint64_t* restrict ptr) {
  err_t err = 0;
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
  _qio_channel_set_error_unlocked(ch, err);

  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

err_t qio_channel_read_svarint(const int threadsafe, qio_channel_t* restrict ch, int64_t* restrict ptr) {
  err_t err;
  uint64_t u_num;
  err = qio_channel_read_uvarint(threadsafe, ch, &u_num);
  *ptr = (u_num >> 1) ^ -((int64_t)(u_num & 1));
  return err;
}
err_t qio_channel_write_uvarint(const int threadsafe, qio_channel_t* restrict ch, uint64_t num) {
  err_t err = 0;
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

err_t qio_channel_write_svarint(const int threadsafe, qio_channel_t* restrict ch, int64_t num) {
  uint64_t u_num = (num << 1) ^ (num >> 63);
  return qio_channel_write_uvarint(threadsafe, ch, u_num);
}



static
err_t _peek_until_byte(qio_channel_t* restrict ch, uint8_t term_byte, int64_t* restrict amt_read_out, int* restrict found_term_out)
{
  err_t err;
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
err_t _getc_after_whitespace(qio_channel_t* restrict ch, int32_t* restrict got_chr)
{
  err_t err = 0;
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
err_t _peek_until_char(qio_channel_t* ch, int32_t term_chr, int64_t* amt_read_out, int* found_term_out)
{
  err_t err, newerr;
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
err_t _append_char(char* restrict * restrict buf, size_t* restrict buf_len, size_t* restrict buf_max, int32_t chr)
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
    return EOVERFLOW;
  }
  // First, make sure that there is room.
  if( need >= max_in ) {
    // Reallocate buffer.
    newsz = 2 * max_in;
    if( newsz < 16  ) newsz = 16;
    if( newsz < need  ) newsz = need;
    newbuf = qio_realloc(buf_in, newsz);
    if( ! newbuf ) return ENOMEM;
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
// -1 -- 1 byte of length before
// -2 -- 2 bytes of length before
// -4 -- 4 bytes of length before
// -8 -- 8 bytes of length before
// -10 -- variable byte length before (hi-bit 1 means more, little endian)
// -0x01XX -- read until terminator XX is read
//  + -- nonzero positive -- read exactly this length.
err_t qio_channel_read_string(const int threadsafe, const int byteorder, const int64_t str_style, qio_channel_t* restrict ch, const char* restrict* restrict out, ssize_t* restrict len_out, ssize_t maxlen)
{
  err_t err;
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

  if( maxlen <= 0 ) maxlen = SSIZE_MAX - 1;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;


  // read a string length.
  switch (str_style) {
    case -1:
      err = qio_channel_read_uint8(false, ch, &num8);
      num = num8;
      break;
    case -2:
      err = qio_channel_read_uint16(false, byteorder, ch, &num16);
      num = num16;
      break;
    case -4:
      err = qio_channel_read_uint32(false, byteorder, ch, &num32);
      num = num32;
      break;
    case -8:
      err = qio_channel_read_uint64(false, byteorder, ch, &num);
      break;
    case -10:
      err = qio_channel_read_uvarint(false, ch, &num);
      break;
    default:
      if( str_style >= 0 ) {
        // just read the suggested length
        num = str_style;
      } else {
        // read a terminated amount.
        term = (-str_style) - 0x0100;
        // What is the position we're marking?
        err = _peek_until_byte(ch, term, &peek_amt, &found_term);
        num = peek_amt;
        if( !err && !found_term ) err = EFORMAT;
      }
  }
  if( err ) goto rewind;

  if( num > (SSIZE_MAX-1) ) {
    err = EOVERFLOW;
    goto rewind;
  }
  if( num > maxlen ) {
    err = EOVERFLOW;
    goto rewind;
  }
  len = num;

  // Now read that many bytes into an allocated area.
  ret = qio_malloc(len + 1); // room for \0.
  if( ! ret ) {
    err = ENOMEM;
    goto rewind;
  }
  err = qio_channel_read(false, ch, ret, num, &amt);
  if( err ) goto rewind;
  if( amt != len ) {
    err = ESHORT;
    // zero out the rest of it...
    memset(ret + amt, 0, len - amt);
    goto rewind;
  }
  ret[len] = '\0'; // add terminator..

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

  if( err && err != EEOF && err != ESHORT ) qio_free(ret);
  else {
    // don't modify out if we didn't read anything.
    if( ret ) {
      *out = ret;
      *len_out = len;
    }
  }

  return err;
}

// allocates and returns a string.
err_t qio_channel_scan_string(const int threadsafe, qio_channel_t* restrict ch, const char* restrict * restrict out, ssize_t* restrict len_out, ssize_t maxlen)
{
  err_t err;
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
  unsigned long conv;
  qio_style_t* style;
  ssize_t nread = 0;
  int64_t mark_offset;
  int64_t end_offset;

  if( qio_glocale_utf8 == 0 ) {
    qio_set_glocale();
  }

  if( maxlen <= 0 ) maxlen = SSIZE_MAX - 1;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  style = &ch->style;

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
  } else {
    handle_back = 1;
    handle_0x = 1;
    handle_u = 0;
    stop_space = 0;
  }

  err = 0;
  for( nread = 0; nread < maxlen && !err; nread++ ) {
    err = qio_channel_read_char(false, ch, &chr);
    if( err ) break;

    // If we're using FORMAT_WORD, skip any whitespace at the beginning
    if( nread == 0 ) {
      while( style->string_format != QIO_STRING_FORMAT_TOEND &&
             iswspace(chr) ) {
        // Read the next character!
        err = qio_channel_read_char(false, ch, &chr);
        if( err ) break;
      }
      if( style->string_format == QIO_STRING_FORMAT_WORD ||
          style->string_format == QIO_STRING_FORMAT_TOEND ) {
        // OK, use the character we have
      } else if( chr == style->string_start ) {
        // Read the next character!
        err = qio_channel_read_char(false, ch, &chr);
        if( err ) break;
      } else {
        // Format error.
        err = EFORMAT;
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
          err = errno;
          break;
        }

        err = _append_char(&ret, &ret_len, &ret_max, conv);
      } else if( handle_u && chr == 'u' ) {
        // handle \uABCD
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
          err = errno;
          break;
        }

        err = _append_char(&ret, &ret_len, &ret_max, conv);
      } else {
        // a backslash'd character.
        err = _append_char(&ret, &ret_len, &ret_max, chr);
      }
    } else if((stop_space && iswspace(chr)) || ((!stop_space) && (chr == term_chr )) ) {
      if(style->string_format == QIO_STRING_FORMAT_TOEND) {
        err = _append_char(&ret, &ret_len, &ret_max, chr);
      }
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
    if( err == 0 ) {
      // Unget the terminating character.
      qio_channel_revert_unlocked(ch);
      qio_channel_advance_unlocked(ch, end_offset - mark_offset - 1);
      goto unlock;
    }

    // Not an error to reach EOF with these ones.
    if( ret_len > 0 && err == EEOF ) err = 0;
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

err_t qio_channel_scan_literal(const int threadsafe, qio_channel_t* restrict ch, const char* restrict match, ssize_t len, int skipws)
{
  err_t err;
  int32_t wchr = -1;
  char chr = -1;
  ssize_t nread = 0;
  int64_t lastwspos = 0;


  if( len == 0 ) return EINVAL;

  if( skipws ) {
    int nbytes;
    int32_t wchr;
    size_t min_nonspace = len;
    size_t max_nonspace = 0;
    // If we're skipping whitespace,
    // ignore leading or trailing whitespace
    // in the pattern.
    for( nread = 0; nread < len; nread += nbytes ) {
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
      nread = min_nonspace;
      len = max_nonspace + 1; 
    }
  }

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  if( skipws ) {
    err = qio_channel_mark(false, ch);
    if( err ) goto revert;

    while( 1 ) {
      lastwspos = qio_channel_offset_unlocked(ch);
      err = qio_channel_read_char(false, ch, &wchr);
      if( err ) break;
      if( ! iswspace(wchr) ) break;
    }

    // ignore EOF when looking for whitespace.
    if( err == EEOF ) err = 0;

    qio_channel_revert_unlocked(ch);

    if( ! err ) {
      // We've exited the loop because the last
      // one we read wasn't whitespace, so seek
      // back to lastwspos. 
      qio_channel_advance_unlocked(ch, lastwspos - qio_channel_offset_unlocked(ch));
    }
  }

  if( err == 0 ) {
    for( nread = 0; nread < len; nread++ ) {
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
      err = EFORMAT;
    }
  }

  if( skipws && !err ) {
    // skip whitespace after the pattern.
    err = qio_channel_mark(false, ch);
    if( err ) goto revert;

    while( 1 ) {
      lastwspos = qio_channel_offset_unlocked(ch);
      err = qio_channel_read_char(false, ch, &wchr);
      if( err ) break;
      if( ! iswspace(wchr) ) break;
    }

    // ignore EOF when looking for whitespace.
    if( err == EEOF ) err = 0;

    qio_channel_revert_unlocked(ch);

    if( ! err ) {
      // We've exited the loop because the last
      // one we read wasn't whitespace, so seek
      // back to lastwspos. 
      qio_channel_advance_unlocked(ch, lastwspos - qio_channel_offset_unlocked(ch));
    }
  }

revert:
  if( err ) {
    qio_channel_revert_unlocked(ch);
  } else {
    qio_channel_commit_unlocked(ch);
  }
  // Don't set error indicator on EFORMAT because
  // that's probably a temporary error.
  if( err != EFORMAT ) _qio_channel_set_error_unlocked(ch, err);
unlock:
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

err_t qio_channel_print_literal(const int threadsafe, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len)
{
  return qio_channel_write_amt(threadsafe, ch, ptr, len);
}

// string binary style:
// -1 -- 1 byte of length before
// -2 -- 2 bytes of length before
// -4 -- 4 bytes of length before
// -8 -- 8 bytes of length before
// -10 -- variable byte length before (hi-bit 1 means more, little endian)
// -0x01XX -- read until terminator XX is read
//  + -- nonzero positive -- read exactly this length.
err_t qio_channel_write_string(const int threadsafe, const int byteorder, const int64_t str_style, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len)
{
  err_t err;
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
    case -1:
      num8 = len;
      if( (ssize_t) num8 != len ) err = EOVERFLOW;
      else err = qio_channel_write_uint8(false, ch, num8);
      break;
    case -2:
      num16 = len;
      if( (ssize_t) num16 != len ) err = EOVERFLOW;
      else err = qio_channel_write_uint16(false, byteorder, ch, num16);
      break;
    case -4:
      num32 = len;
      if( (ssize_t) num32 != len ) err = EOVERFLOW;
      else err = qio_channel_write_uint32(false, byteorder, ch, num32);
      break;
    case -8:
      num64 = len;
      if( (ssize_t) num64 != len ) err = EOVERFLOW;
      else err = qio_channel_write_uint64(false, byteorder, ch, num64);
      break;
    case -10:
      num64 = len;
      if( (ssize_t) num64 != len ) err = EOVERFLOW;
      else err = qio_channel_write_uvarint(false, ch, num64);
      break;
    default:
      if( str_style >= 0 ) {
        // Verify that the length matches.
        if( len != str_style ) err = EOVERFLOW;
        else err = 0;
      } else {
        // We're going to write a padding character.
        use_term = 1;
        term = (-str_style) - 0x0100;
        err = 0;
      }
  }

  if( err ) goto rewind;

  // write the string itself
  err = qio_channel_write_amt(false, ch, ptr, len);
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

err_t qio_channel_print_string(const int threadsafe, qio_channel_t* restrict ch, const char* restrict ptr, ssize_t len)
{
  err_t err;
  ssize_t i, j;
  int clen = 1;
  int32_t chr;
  qio_style_t* style;
  char tmp[MB_LEN_MAX_OR_6+1];
  int tmplen;
  char tmpch_hi, tmpch_lo;
  int32_t tmpchr;

  if( qio_glocale_utf8 == 0 ) {
    qio_set_glocale();
  }

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  style = &ch->style;

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  // write the string itself, possible with some escape-handling.
  if( style->string_format == QIO_STRING_FORMAT_WORD ||
      style->string_format == QIO_STRING_FORMAT_TOEND ) {
    // Do not interpret the string in any way... just write it.
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

      // handle escaping for the different formats.
      switch (style->string_format) {
        case QIO_STRING_FORMAT_BASIC:
          if( chr == style->string_end || chr == '\\' ) {
            err = qio_channel_write_char(false, ch, '\\');
            if( err ) goto rewind;
            err = qio_channel_write_char(false, ch, chr);
            if( err ) goto rewind;
          } else {
            err = qio_channel_write_char(false, ch, chr);
            if( err ) goto rewind;
          }
          break;
        case QIO_STRING_FORMAT_CHPL:
          if( chr == style->string_end || chr == '\\' || chr == '\'' || chr == '"' || chr == '\n' ) {
            err = qio_channel_write_char(false, ch, '\\');
            if( err ) goto rewind;
            if( chr == '\n' ) {
              tmpchr = 'n';
            } else {
              tmpchr = chr;
            }

            err = qio_channel_write_char(false, ch, tmpchr);
            if( err ) goto rewind;

          } else if( !iswprint(chr) ) {
            // convert each of the bytes into \x00 escaped things.
            tmplen = qio_nbytes_char(chr);
            err = qio_encode_char_buf(tmp, chr);
            if( err ) goto rewind;

            // Now, print out each byte escaped.
            for( j = 0; j < tmplen; j++ ) {
              tmpch_hi = (tmp[j] >> 4) & 0xf;
              tmpch_lo = tmp[j] & 0xf;
              if( tmpch_hi < 10 ) tmpch_hi = '0' + tmpch_hi;
              else tmpch_hi = 'a' + tmpch_hi - 10;
              if( tmpch_lo < 10 ) tmpch_lo = '0' + tmpch_lo;
              else tmpch_lo = 'a' + tmpch_lo - 10;

              err = qio_channel_write_char(false, ch, '\\');
              if( err ) goto rewind;
              err = qio_channel_write_char(false, ch, 'x');
              if( err ) goto rewind;
              err = qio_channel_write_char(false, ch, tmpch_hi);
              if( err ) goto rewind;
              err = qio_channel_write_char(false, ch, tmpch_lo);
              if( err ) goto rewind;
            }
          } else {
            err = qio_channel_write_char(false, ch, chr);
            if( err ) goto rewind;
          }
          break;
        case QIO_STRING_FORMAT_JSON:
          if( chr == style->string_end || chr == '\\' || chr == '"' ||
              chr == '\b' || chr == '\f' || chr == '\n' || chr == '\r' || chr == '\t' ) {

            err = qio_channel_write_char(false, ch, '\\');
            if( err ) goto rewind;

            if( chr == '\b' ) {
              tmpchr = 'b';
            } else if( chr == '\f' ) {
              tmpchr = 'f';
            } else if( chr == '\n' ) {
              tmpchr = 'n';
            } else if( chr == '\r' ) {
              tmpchr = 'r';
            } else if( chr == '\t' ) {
              tmpchr = 't';
            } else {
              tmpchr = chr;
            }

            err = qio_channel_write_char(false, ch, tmpchr);
            if( err ) goto rewind;
          } else if( !iswprint(chr) ) {
            // write it as \uXXXX 4 hex digits.
            tmplen = sprintf(tmp, "\\u%04x", (unsigned int) chr);
            if( tmplen < 0 ) {
              err = errno;
              goto rewind;
            }
            err = qio_channel_write_amt(false, ch, tmp, tmplen);
            if( err ) goto rewind;
          } else {
            err = qio_channel_write_char(false, ch, chr);
            if( err ) goto rewind;
          }
          break;
      }
    }

    // Write string_end.
    err = qio_channel_write_char(false, ch, style->string_end);
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

err_t qio_quote_string(uint8_t string_start, uint8_t string_end, uint8_t string_format, const char* restrict ptr, ssize_t len, const char** restrict out)
{
  err_t err;
  ssize_t i;
  int j;
  int clen = 1;
  int32_t chr;
  char tmp[MB_LEN_MAX_OR_6+1];
  int tmplen;
  char tmpch_hi, tmpch_lo;
  int32_t tmpchr;
  ssize_t quoted_len = 0;
  ssize_t q = 0;
  int clenout = 0;
  int pass = 0;
  char* ret = NULL;

#define WRITEC(ch) { \
    clenout = qio_nbytes_char(ch); \
    if( pass ) qio_encode_char_buf(&ret[q], ch); \
    q += clenout; \
  }

  for( pass = 0; pass < 2; pass++ ) {
    if( pass ) {
      quoted_len = q;
      q = 0;
      // allocate quoted_len memory at the start of pass 1.
      ret = qio_malloc(quoted_len + 1); // room for \0.
      if( !ret ) {
        err = ENOMEM;
        goto error;
      }
      ret[quoted_len] = '\0';
    }
    // write the string itself, possible with some escape-handling.
    if( string_format == QIO_STRING_FORMAT_WORD ||
        string_format == QIO_STRING_FORMAT_TOEND ) {
      if( pass ) memcpy(ret,ptr,len);
      else quoted_len = len;
    } else {
      // Write string_start.
      WRITEC(string_start);

      // Write the string while translating it.
      for( i = 0; i < len; i+=clen ) {
        err = qio_decode_char_buf(&chr, &clen, ptr + i, len - i);
        if( err ) goto error;

        // handle escaping for the different formats.
        switch (string_format) {
          case QIO_STRING_FORMAT_BASIC:
            if( chr == string_end || chr == '\\' ) {
              WRITEC('\\');
              WRITEC(chr);
            } else {
              WRITEC(chr);
            }
            break;
          case QIO_STRING_FORMAT_CHPL:
            if( chr == string_end || chr == '\\' || chr == '\'' || chr == '"' || chr == '\n' ) {
              WRITEC('\\');
              if( chr == '\n' ) {
                tmpchr = 'n';
              } else {
                tmpchr = chr;
              }

              WRITEC(tmpchr);
            } else if( !iswprint(chr) ) {
              // convert each of the bytes into \x00 escaped things.
              tmplen = qio_nbytes_char(chr);
              err = qio_encode_char_buf(tmp, chr);
              if( err ) goto error;

              // Now, print out each byte escaped.
              for( j = 0; j < tmplen; j++ ) {
                tmpch_hi = (tmp[j] >> 4) & 0xf;
                tmpch_lo = tmp[j] & 0xf;
                if( tmpch_hi < 10 ) tmpch_hi = '0' + tmpch_hi;
                else tmpch_hi = 'a' + tmpch_hi - 10;
                if( tmpch_lo < 10 ) tmpch_lo = '0' + tmpch_lo;
                else tmpch_lo = 'a' + tmpch_lo - 10;

                WRITEC('\\');
                WRITEC('x');
                WRITEC(tmpch_hi);
                WRITEC(tmpch_lo);
              }
            } else {
              WRITEC(chr);
            }
            break;
          case QIO_STRING_FORMAT_JSON:
            if( chr == string_end || chr == '\\' || chr == '"' ||
                chr == '\b' || chr == '\f' || chr == '\n' || chr == '\r' || chr == '\t' ) {

              WRITEC('\\');

              if( chr == '\b' ) {
                tmpchr = 'b';
              } else if( chr == '\f' ) {
                tmpchr = 'f';
              } else if( chr == '\n' ) {
                tmpchr = 'n';
              } else if( chr == '\r' ) {
                tmpchr = 'r';
              } else if( chr == '\t' ) {
                tmpchr = 't';
              } else {
                tmpchr = chr;
              }

              WRITEC(tmpchr);
            } else if( !iswprint(chr) ) {
              // write it as \uXXXX 4 hex digits.
              tmplen = sprintf(tmp, "\\u%04x", (unsigned int) chr);
              if( tmplen < 0 ) {
                err = errno;
                goto error;
              }
              for( int k = 0; k < tmplen; k++ ) {
                WRITEC(tmp[k]);
              }
            } else {
              WRITEC(chr);
            }
            break;
        }
      }

      WRITEC(string_end);
    }
  }
  *out = ret;
  return 0;
error:
  qio_free(ret);
  *out = NULL;
  return err;
#undef WRITEC
}

const char* qio_quote_string_chpl(const char* ptr, ssize_t len)
{
  const char* ret = NULL;
  err_t err;
  err = qio_quote_string('"', '"', QIO_STRING_FORMAT_CHPL, ptr, len, &ret);
  if( err || !ret ) return qio_strdup("<error>");
  return ret;
}

// only support floating point numbers in
// base 10, or base 16 (with decimal exponent).
//
// For other numbers, we can support more arbitrary bases...
//
// Always do case insensitive reading; the characters here should be
// lower case.
typedef struct number_reading_state_s {
  int base; // 0 means 0b 0x supported; othewise particular base 2,10,16

  char allow_base; // allow 0b or 0x when base == 2 or == 16 respectively
                   // (these are always allowed when base == 0)
  char allow_sign;
  char positive_char; // normally '+'
  char negative_char; // normally '-'

  char allow_real;
  char point_char; // normally '.'; set to -1 if no radix point allowed
  char exponent_char; // use if base <= 10; normally 'e'
  char other_exponent_char; // use if base > 10; normally 'p' or '@'

  int usebase;

  signed char sign; // resulting sign; positive or negative
  char is_nan; // did we read nan()? 
  char is_inf; // did we read inf or infinity?
  int gotbase;
  int64_t digits_start; // where do the digits start?
  int64_t point; // where is the decimal point?
  int64_t exponent; // where is the exponent character (e.g. e)?
  int64_t end; // where is the end?
               // set to -1 if we shouldn't yet be at the end.
} number_reading_state_t;

static
err_t _peek_number_unlocked(qio_channel_t* restrict ch, number_reading_state_t* restrict s, int64_t* restrict amount)
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
    if( err == EEOF ) chr = -1; \
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
  err_t err;

  s->sign = 0;
  s->is_nan = 0;
  s->is_inf = 0;
  s->digits_start = -1;
  s->point = -1;
  s->exponent = -1;
  s->end = -1;
  if( s->base == 0 ) {
    s->gotbase = s->usebase = 10;
  } else {
    s->gotbase = s->usebase = s->base;
  }

  mark_offset = qio_channel_offset_unlocked(ch);

  err = qio_channel_mark(false, ch);
  if( err ) return err;

  // First, skip any whitespace.
  do {
    NEXT_CHR;
  } while( iswspace(chr) );
  // At end of loop, chr is non-whitespace.

  // Next, read + or -.
  if( s->allow_sign && chr == s->positive_char ) {
    s->sign = 1;
  } else if( s->allow_sign && chr == s->negative_char ) {
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
    if( chr != 'n' ) { err = EFORMAT; goto error; }
    NEXT_CHR;
    if( chr != 'f' ) { err = EFORMAT; goto error; }
    s->is_inf = 1;
    NEXT_CHR_OR_EOF;
    if( chr != 'i' ) UNGET_ACCEPT;
    NEXT_CHR;
    if( chr != 'n' ) { err = EFORMAT; goto error; }
    NEXT_CHR;
    if( chr != 'i' ) { err = EFORMAT; goto error; }
    NEXT_CHR;
    if( chr != 't' ) { err = EFORMAT; goto error; }
    NEXT_CHR;
    if( chr != 'y' ) { err = EFORMAT; goto error; }
    ACCEPT;
  } else if( s->allow_real && chr == 'n' ) {
    START_DIGITS;
    // Read not a number.
    NEXT_CHR;
    if( chr != 'a' ) { err = EFORMAT; goto error; }
    NEXT_CHR;
    if( chr != 'n' ) { err = EFORMAT; goto error; }
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
    if( err == EEOF ) {
      s->digits_start = qio_channel_offset_unlocked(ch) - 1;
    } else if( s->allow_base && chr == 'x' ) {
      s->gotbase = s->usebase = 16;
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
    if( err == EEOF ) ACCEPT;
    if( s->allow_real && chr == s->point_char && s->point == -1 ) {
      NEXT_CHR_OR_EOF;
      s->end = s->point = qio_channel_offset_unlocked(ch);
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
    } else {
      // Not a digit.
      UNGET_ACCEPT;
    }
  }

done:
  err = 0;
  *amount = s->end - mark_offset;
  if( *amount == 0 ) {
    err = EFORMAT;
  }
error:
  qio_channel_revert_unlocked(ch);
  return err;
}


err_t qio_channel_scan_int(const int threadsafe, qio_channel_t* restrict ch, void* restrict out, size_t len, int issigned)
{
  unsigned long long int num;
  long long int signed_num;
  int sign;
  ssize_t signed_len;
  number_reading_state_t st;
  int64_t amount;
  int64_t start;
  char* end;
  char* buf = NULL;
  MAYBE_STACK_SPACE(buf_onstack);
  err_t err;
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
  st.allow_sign = issigned;
  st.positive_char = tolower(style->positive_char);
  st.negative_char = tolower(style->negative_char);

  err = _peek_number_unlocked(ch, &st, &amount);
  if( err == EEOF && st.end > 0 ) err = 0; // we tolerate EOF if there's data.
  if( err ) goto error;

  MAYBE_STACK_ALLOC(amount + 1, buf, buf_onstack);
  if( ! buf ) {
    err = ENOMEM;
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
    err = errno;
    goto error;
  }
  if( end - buf != st.end - start ) {
    // some kind of format error.
    err = EFORMAT;
    goto error;
  }

  // success!
  err = 0;

  // now return the number.
  signed_len = len;
  if( issigned ) signed_len = - signed_len;

  signed_num = num;
  if( sign < 0 ) signed_num = - num;

  err = 0;
  switch( signed_len ) {
    case -1:
      *(int8_t*) out = signed_num;
      if( signed_num > INT8_MAX || signed_num < INT8_MIN ) err = ERANGE;
      break;
    case 1:
      *(uint8_t*) out = num;
      if( num > UINT8_MAX ) err = ERANGE;
      break;
    case -2:
      *(int16_t*) out = signed_num;
      if( signed_num > INT16_MAX || signed_num < INT16_MIN ) err = ERANGE;
      break;
    case 2:
      *(uint16_t*) out = num;
      if( num > UINT16_MAX ) err = ERANGE;
      break;
    case -4:
      *(int32_t*) out = signed_num;
      if( signed_num > INT32_MAX || signed_num < INT32_MIN ) err = ERANGE;
      break;
    case 4:
      *(uint32_t*) out = num;
      if( num > UINT32_MAX ) err = ERANGE;
      break;
    case -8:
      *(int64_t*) out = signed_num;
      //if( signed_num > INT64_MAX || signed_num < INT64_MIN ) err = ERANGE;
      break;
    case 8:
      *(uint64_t*) out = num;
      //if( num > UINT64_MAX ) err = ERANGE;
      break;
    default:
      err = EINVAL;
  }

error:
  MAYBE_STACK_FREE(buf, buf_onstack);

  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

err_t qio_channel_scan_float(const int threadsafe, qio_channel_t* restrict ch, void* restrict out, size_t len)
{
  double num;
  number_reading_state_t st;
  int64_t amount;
  int64_t start;
  char* end_conv;
  char* buf = NULL;
  MAYBE_STACK_SPACE(buf_onstack);
  ssize_t i;
  ssize_t digits_start;
  ssize_t point;
  ssize_t exponent;
  err_t err;
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
  st.allow_sign = 1;
  st.positive_char = tolower(style->positive_char);
  st.negative_char = tolower(style->negative_char);
  st.allow_real = 1;
  st.point_char = tolower(style->point_char);
  st.exponent_char = tolower(style->exponent_char);
  st.other_exponent_char = tolower(style->other_exponent_char);

  err = _peek_number_unlocked(ch, &st, &amount);
  if( err == EEOF && st.end > 0 ) err = 0; // we tolerate EOF if there's data.
  if( err ) goto error;

  //printf("got amount %lli\n", (long long int) amount);

  MAYBE_STACK_ALLOC(amount + 4, buf, buf_onstack);
  if( ! buf ) {
    err = ENOMEM;
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
    err = EFORMAT;
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
    err = EFORMAT;
    goto error;
  }
  if((num == HUGE_VAL||num == -HUGE_VAL||num == 0.0) && errno == ERANGE){
    // overflow or underflow
    err = ERANGE;
    goto error;
  }

  err = 0;
  switch ( len ) {
    case 8:
      *(double*) out = num;
      break;
    case 4:
      *(float*) out = num;
      break;
    default:
      err = EINVAL;
  }

error:
  MAYBE_STACK_FREE(buf, buf_onstack);

  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
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
  char tmp[22]; // enough room for largest 64-bit number base 10.
  int tmp_len=0;
  char b=0;
  int i,shift=0;
  int width;
  int ret_width;

  if( base == 2 ) {

    // Pass any zero digits.
    for( shift = 0; (num >> 63) == 0 && shift < 63; shift++, num <<= 1 ) ;

    // Our number will be 64-shift digits.
    width = 64 - shift;
  } else {
    if( base == 16 ) {
      if( style->uppercase ) 
        width = snprintf(tmp, 22, "%" PRIX64, num);
      else 
        width = snprintf(tmp, 22, "%" PRIx64, num);
    } else if ( base == 10 ) {
      width = snprintf(tmp, 22, "%" PRIu64, num);
    } else return -1;
    tmp_len = width;
    if( width < 0 ) return -1;
  }

  if( style->showplus || isnegative ) width++;
  if( style->prefix_base && base != 10 ) width += 2;

  // We might not have room...
  ret_width = width;
  if( ret_width < style->min_width ) ret_width = style->min_width;
  if( ret_width >= size ) return ret_width;

  i = 0;

  if( !style->leftjustify && width < style->min_width && style->pad_char != '0' ) {
    // Put what we need to for getting to the min_width.
    for( ; i < style->min_width - width; i++ ) {
      dst[i] = style->pad_char;
    }
  }

  if( style->showplus || isnegative ) {
    char pluschar = (style->showplus==2)?style->pad_char:style->positive_char;
    // put the '+' or '-' sign.
    dst[i++] = isnegative?style->negative_char:pluschar;
  }
  if( style->prefix_base && base != 10 ) {
    dst[i++] = '0';
    if( base == 2 ) b = style->uppercase?'B':'b';
    else if( base == 16 ) b = style->uppercase?'X':'x';
    dst[i++] = b;
  }

  if( !style->leftjustify && width < style->min_width && style->pad_char == '0' ) {
    // Put what we need to for getting to the min_width.
    for( ; i < style->min_width - width; i++ ) {
      dst[i] = '0';
    }
  }

  // now output the digits.
  if( base == 2 ) {
    for( ; shift < 64; shift++, num <<= 1 ) {
      dst[i++] = '0' + (num >> 63);
    }
  } else {
    memcpy(dst + i, tmp, tmp_len);
    i += tmp_len;
  }

  // Now if we're left justified we might need padding.
  if( style->leftjustify && width < style->min_width) {
    // Put what we need to for getting to the min_width.
    for( ; i < style->min_width; i++ ) {
      dst[i] = style->pad_char;
    }
  }

  assert( i == ret_width );

  dst[i] = '\0';
  return i;
}

static
int _ftoa(char* restrict dst, size_t size, double num, int base, const qio_style_t* restrict style )
{
  int buf_sz = 32;
  char* buf = NULL;
  MAYBE_STACK_SPACE(buf_onstack);
  int buf_len;
  int got=0;
  int width;
  int isnegative;
  int shownegative;
  int ret_width;
  int i;
  int precision, sigdigits;
  int adjust_width;

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

  precision = style->precision;
  sigdigits = style->significant_digits;

  adjust_width = 0;
  if( style->showplus || shownegative ) adjust_width++;
  if( !style->prefix_base && base != 10 ) {
    if( !isnan(num) && !isinf(num) ) adjust_width -= 2; // remove 0x
  }

  while( 1 ) {
    MAYBE_STACK_ALLOC(buf_sz, buf, buf_onstack);
    if( ! buf ) {
      return -1;
    }

    //printf("Printing with base %i type %i precision %i sig %i upper %i showp %i\n",
    //       style->base, style->realfmt, precision, sigdigits, style->uppercase, style->showpoint);

    // Figure out how big our output is...
    if( style->base == 16 ) {
      if( precision < 0 ) {
        if( style->uppercase ) {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#A", num);
          else
            got = snprintf(buf, buf_sz, "%A", num);
        } else {
          if( style->showpoint ) 
            got = snprintf(buf, buf_sz, "%#a", num);
          else
            got = snprintf(buf, buf_sz, "%a", num);
        }
      } else {
        if( style->uppercase ) {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#.*A", precision, num);
          else
            got = snprintf(buf, buf_sz, "%.*A", precision, num);
        } else {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#.*a", precision, num);
          else
            got = snprintf(buf, buf_sz, "%.*a", precision, num);
        }
      }
    } else if( style->realfmt == 0 ) {
      if( sigdigits < 0 ) {
        if( style->uppercase ) {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#G", num);
          else
            got = snprintf(buf, buf_sz, "%G", num);
        } else {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#g", num);
          else
            got = snprintf(buf, buf_sz, "%g", num);
        }
      } else {
        if( style->uppercase ) {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#.*G", sigdigits, num);
          else
            got = snprintf(buf, buf_sz, "%.*G", sigdigits, num);
        } else {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#.*g", sigdigits, num);
          else
            got = snprintf(buf, buf_sz, "%.*g", sigdigits, num);
        }
      }
    } else if( style->realfmt == 1 ) {
      if( precision < 0 ) {
        if( style->uppercase ) {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#F", num);
          else
            got = snprintf(buf, buf_sz, "%F", num);
        } else {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#f", num);
          else
            got = snprintf(buf, buf_sz, "%f", num);
        }
      } else {
        if( style->uppercase ) {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#.*F", precision, num);
          else
            got = snprintf(buf, buf_sz, "%.*F", precision, num);
        } else {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#.*f", precision, num);
          else
            got = snprintf(buf, buf_sz, "%.*f", precision, num);
        }
      }
    } else if( style->realfmt == 2 ) {
      if( precision < 0 ) {
        if( style->uppercase ) {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#E", num);
          else
            got = snprintf(buf, buf_sz, "%E", num);
        } else {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#e", num);
          else
            got = snprintf(buf, buf_sz, "%e", num);
        }
      } else {
        if( style->uppercase ) {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#.*E", precision, num);
          else
            got = snprintf(buf, buf_sz, "%.*E", precision, num);
        } else {
          if( style->showpoint )
            got = snprintf(buf, buf_sz, "%#.*e", precision, num);
          else
            got = snprintf(buf, buf_sz, "%.*e", precision, num);
        }
      }
    }

    if( style->showpointzero && style->base != 16 ) {
      // .. but if it ended in just . we add a .0
      if( got > 0 && buf[got-1] == '.' ) {
        int period = got - 1;
        got += 1;
        if( got < buf_sz ) {
          buf[period] = '.'; // (redundant)
          buf[period+1] = '0';
        }
        // otherwise buf will be reallocated and we try again.
      } else { // maybe it was an integer than needs a .0 ?
        int needspoint;
        int i;

        needspoint = 1;
        for( i = 0; i < got; i++ ) {
          if( isdigit(buf[i]) ) {
           // maybe still need decimal point!
          } else {
           // . e E inf etc. don't need decimal point.
           needspoint = 0;
          }
        }

        if( needspoint ) {
          int period = got; // '\0' at end of string.
          got += 2;
          if( got < buf_sz ) {
            buf[period] = '.';
            buf[period+1] = '0';
          }
          // otherwise, buf will be reallocated and we try again.
        }
      }
    }

    if( got < 0 ) {
      MAYBE_STACK_FREE(buf, buf_onstack);
      return -1;
    }
    if( got < buf_sz ) {
      buf_len = got;
      // OK! we printed it.
      break;
    } else {
      // Get a bigger buffer.
      MAYBE_STACK_FREE(buf, buf_onstack);
      buf = NULL;
      // We might not have room...
      ret_width = got + adjust_width;
      if( ret_width < style->min_width ) ret_width = style->min_width;
      if( ret_width >= size ) return ret_width;
      buf_sz = got + 1;
    }
  }

  // OK; now we have our number printed in buf.
  width = got;

  // We might not have room...
  ret_width = got + adjust_width;
  if( ret_width < style->min_width ) ret_width = style->min_width;
  if( ret_width >= size ) {
    MAYBE_STACK_FREE(buf, buf_onstack);
    return ret_width;
  }

  //printf("Got '%s' from sprintf\n", buf);

  i = 0;

  width = got + adjust_width;

  if( !style->prefix_base && base != 10 ) i += 2;

  if( !style->leftjustify && width < style->min_width && style->pad_char != '0' ) {
    // Put what we need to for getting to the min_width.
    for( ; i < style->min_width - width; i++ ) {
      dst[i] = style->pad_char;
    }
  }

  if( style->showplus || shownegative ) {
    char pluschar = (style->showplus==2)?style->pad_char:style->positive_char;
    // put the '+' or '-' sign.
    dst[i++] = isnegative?style->negative_char:pluschar;
  }
  // 0x is already there if we're putting it.

  if( !style->leftjustify && width < style->min_width && style->pad_char == '0' ) {
    // Put what we need to for getting to the min_width.
    for( ; i < style->min_width - width; i++ ) {
      dst[i] = '0';
    }
  }

  // now output the digits
  memcpy(dst + i, buf, buf_len);
  i += buf_len;

  // Now if we're left justified we might need padding.
  if( style->leftjustify && width < style->min_width) {
    // Put what we need to for getting to the min_width.
    for( ; i < style->min_width; i++ ) {
      dst[i] = style->pad_char;
    }
  }

  assert( i == ret_width );
  dst[i] = '\0';

  MAYBE_STACK_FREE(buf, buf_onstack);

  return i;
}

// TODO -- support max_width
err_t qio_channel_print_int(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, size_t len, int issigned)
{
  uint64_t num=0;
  int64_t num_s=0;
  int isneg;
  int max = 70; // room enough for binary output. (1 '\0', 2 0b, 1 +-, 64 bits)
  int signed_len;
  int base;
  char* tmp = NULL;
  MAYBE_STACK_SPACE(tmp_onstack);
  int got;
  err_t err;
  qio_style_t* style;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  style = &ch->style;

  if( style->min_width + 1 > max ) max = style->min_width + 1;

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
      err = EINVAL;
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
  if( VOID_PTR_DIFF(ch->cached_end,ch->cached_cur) > max ) {
    // Print it all directly into the buffer.
    got = _ltoa(ch->cached_cur, VOID_PTR_DIFF(ch->cached_end,ch->cached_cur), 
                num, isneg, base, style);
    if( got < 0 ) {
      err = EFORMAT;
      goto error;
    } else if( got < VOID_PTR_DIFF(ch->cached_end,ch->cached_cur) ) {
      ch->cached_cur = VOID_PTR_ADD(ch->cached_cur, got);
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
  while( 1 ) {
    // Store it in a tempory variable and then
    // copy that in to the buffer.
    MAYBE_STACK_ALLOC(max, tmp, tmp_onstack);
    if( ! tmp ) {
      err = ENOMEM;
      goto error;
    }

    // Print it all to tmp
    got = _ltoa(tmp, max,
                num, isneg, base, style);
    if( got < 0 ) {
      err = EFORMAT;
      goto error;
    } else if( got < max ) {
      err = qio_channel_write_amt(false, ch, tmp, got);
      goto error;
    } else {
      // Not enough room... try again. 
      MAYBE_STACK_FREE(tmp, tmp_onstack);
      tmp = NULL;
      max = got + 1;
    }
  }

error:
  MAYBE_STACK_FREE(tmp, tmp_onstack);

  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

// TODO -- support max_width.
err_t qio_channel_print_float(const int threadsafe, qio_channel_t* restrict ch, const void* restrict ptr, size_t len)
{
  int max = 3 + DBL_MANT_DIG - DBL_MIN_EXP + 1 + 10; // +10=a little extra room
  char* buf = NULL;
  MAYBE_STACK_SPACE(buf_onstack);
  int got;
  int base;
  err_t err;
  double num;
  qio_style_t* style;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  style = &ch->style;

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
      err = EINVAL;
  }
  if( err ) goto error;

  // Try printing it directly into the buffer.
  if( VOID_PTR_DIFF(ch->cached_end, ch->cached_cur) > max ) {
    // Print it all directly into the buffer.
    got = _ftoa(ch->cached_cur, VOID_PTR_DIFF(ch->cached_end, ch->cached_cur), 
                num, base, style);
    if( got < 0 ) {
      err = EFORMAT;
      goto error;
    } else if( got < VOID_PTR_DIFF(ch->cached_end, ch->cached_cur) ) {
      ch->cached_cur = VOID_PTR_ADD(ch->cached_cur, got);
      err = _qio_channel_post_cached_write(ch);
      // OK!
      goto error;
    } else {
      // There was not enough room, but we can set the
      // amount of room needed.
      max = got + 1;
    }
  }
 
  // We get here if there wasn't enough room in cached for _ftoa
  while( 1 ) {
    // Store it in a tempory variable and then
    // copy that in to the buffer.
    MAYBE_STACK_ALLOC(max, buf, buf_onstack);
    if( ! buf ) {
      err = ENOMEM;
      goto error;
    }

    // Print it all to buf
    got = _ftoa(buf, max, num, base, style);
    if( got < 0 ) {
      err = EFORMAT;
      goto error;
    } else if( got < max ) {
      err = qio_channel_write_amt(false, ch, buf, got);
      goto error;
    } else {
      // Not enough room... try again. 
      MAYBE_STACK_FREE(buf, buf_onstack);
      buf = NULL;
      max = got + 1;
    }
  }


error:
  MAYBE_STACK_FREE(buf, buf_onstack);

  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;

}

err_t qio_channel_scan_complex(const int threadsafe, qio_channel_t* restrict ch, void* restrict re_out, void* restrict im_out, size_t len)
{
  int32_t chr;
  err_t err;

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
    err = EFORMAT;
    goto rewind;
  }

  if( chr == '(' ) {
    // read in the parens-style.
    err = qio_channel_scan_float(false, ch, re_out, len);
    if( err ) goto rewind;

    err = _getc_after_whitespace(ch, &chr);
    if( err ) goto rewind;

    if( chr != ',' ) {
      err = EFORMAT;
      goto rewind;
    }

    err = qio_channel_scan_float(false, ch, im_out, len);
    if( err ) goto rewind;

    err = _getc_after_whitespace(ch, &chr);
    if( err ) goto rewind;

    if( chr != ')' ) {
      err = EFORMAT;
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
            err = EINVAL;
        }
      }
      if( err ) goto rewind;

      // Read the 'i'
      err = qio_channel_read_char(false, ch, &chr);
      if( err ) goto rewind;
      if( chr != 'i' ) {
        err = EFORMAT;
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
          err = EINVAL;
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

err_t qio_channel_skip_past_newline(const int threadsafe, qio_channel_t* restrict ch)
{
  int32_t c;
  err_t err;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  while( 1 ) {
    err = qio_channel_read_char(threadsafe, ch, &c);
    if( err  || c == '\n' ) break;
  }

  _qio_channel_set_error_unlocked(ch, err);
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }
  return err;
}

err_t qio_channel_write_newline(const int threadsafe, qio_channel_t* restrict ch)
{
  char c = '\n';
  return qio_channel_write_amt(threadsafe, ch, &c, 1);
}

err_t qio_channel_print_complex(const int threadsafe, qio_channel_t* restrict ch, const void* restrict re_ptr, const void* restrict im_ptr, size_t len)
{
  double num8;
  float num4;
  err_t err;
  int re_isnan = 0;
  int im_isnan = 0;
  int isnegative = 0;
  style_char_t pos_char = ch->style.positive_char;
  style_char_t neg_char = ch->style.negative_char;

  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) return err;
  }

  err = qio_channel_mark(false, ch);
  if( err ) goto unlock;

  if( (ch->style.complex_style & QIO_COMPLEX_FORMAT_PART) == QIO_COMPLEX_FORMAT_ABI ) {
    { // is im_ptr positive or negative? are either nan?
      err = 0;
      switch (len) {
        case 4:
          num4 = *(float*)im_ptr;
          num8 = num4;
          re_isnan = isnan(*(float*)re_ptr);
          im_isnan = isnan(num4);
          break;
        case 8:
          num8 = *(double*)im_ptr;
          num4 = num8;
          re_isnan = isnan(*(double*)re_ptr);
          im_isnan = isnan(num8);
          break;
        default:
          err = EINVAL;
      }

      if( signbit(num8) ) {
        isnegative = 1;
        num4 = -num4;
        num8 = -num8;
      }
      else isnegative = 0;
    }

    if( re_isnan ) {
      err = qio_channel_print_float(false, ch, re_ptr, len);
      if( err ) goto rewind;
    } else if( im_isnan ) {
      err = qio_channel_print_float(false, ch, im_ptr, len);
      if( err ) goto rewind;
    } else {
      // write a + bi
      err = qio_channel_print_float(false, ch, re_ptr, len);
      if( err ) goto rewind;

      err = qio_channel_write_char(false, ch, ' ');
      if( err ) goto rewind;

      err = qio_channel_write_char(false, ch, isnegative?neg_char:pos_char);
      if( err ) goto rewind;

      err = qio_channel_write_char(false, ch, ' ');
      if( err ) goto rewind;
       
      switch (len) {
        case 4:
          err = qio_channel_print_float(false, ch, &num4, len);
          break;
        case 8:
          err = qio_channel_print_float(false, ch, &num8, len);
          break;
        default:
          err = EINVAL;
      }
      if( err ) goto rewind;
      

      // write the i...
      err = qio_channel_write_char(false, ch, 'i');
      if( err ) goto rewind;
    }

  } else if( (ch->style.complex_style & QIO_COMPLEX_FORMAT_PART) == QIO_COMPLEX_FORMAT_PARENS ) {
    // write (a,b)
    err = qio_channel_write_char(false, ch, '(');
    if( err ) goto rewind;

    err = qio_channel_print_float(false, ch, re_ptr, len);
    if( err ) goto rewind;

    err = qio_channel_write_char(false, ch, ',');
    if( err ) goto rewind;

    //err = qio_channel_write_char(false, ch, ' ');
    //if( err ) goto rewind;

    err = qio_channel_print_float(false, ch, im_ptr, len);
    if( err ) goto rewind;

    err = qio_channel_write_char(false, ch, ')');
    if( err ) goto rewind;
  } else {
    err = EINVAL;
    goto rewind;
  }

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


err_t _qio_channel_read_char_slow_unlocked(qio_channel_t* restrict ch, int32_t* restrict chr) {
  err_t err=0;
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
         qio_utf8_decode(&state,
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
      err = -gotch;
    } else {
      *chr = 0xfffd; // replacement character
      err = EILSEQ;
    }
  } else if( qio_glocale_utf8 == QIO_GLOCALE_ASCII ) {
    // character == byte.
    gotch = qio_channel_read_byte(false, ch);
    if( gotch < 0 ) {
      err = -gotch;
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
    if( MB_LEN_MAX <= VOID_PTR_DIFF(ch->cached_end, ch->cached_cur) ) {
      got = mbrtowc(&tmp_chr, ch->cached_cur, MB_LEN_MAX, &ps);
      if( got == 0 ) {
        *chr = 0;
      } else if( got == (size_t) -1 || got == (size_t) -2 ) {
        // it contains an invalid multibyte sequence.
        // or it claims we don't have a complete character
        // (even though we had MB_LEN_MAX!).
        // errno should be EILSEQ.
        *chr = -3; // invalid character... think 0xfffd for unicode
        err = EILSEQ;
      } else {
        *chr = tmp_chr;
        err = 0;
        ch->cached_cur = VOID_PTR_ADD(ch->cached_cur,got);
      }
    } else {
      // Slow path: we might need to read 1 byte at a time.

      while( 1 ) {
        // We always read 1 character at least.
        gotch = qio_channel_read_byte(false, ch);
        if( gotch < 0 ) {
          err = -got;
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
          err = EILSEQ;
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

const char* qio_encode_to_string(int32_t chr)
{
  int nbytes;
  char* buf;
  err_t err;

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

err_t _qio_channel_write_char_slow_unlocked(qio_channel_t* restrict ch, int32_t chr) {
  char mbs[MB_LEN_MAX];
  err_t err;

  if( qio_glocale_utf8 == QIO_GLOCALE_UTF8 ) {
    // #1           00000000 0xxxxxxx <-> 0xxxxxxx
    // #2           00000yyy yyxxxxxx <-> 110yyyyy 10xxxxxx
    // #3           zzzzyyyy yyxxxxxx <-> 1110zzzz 10yyyyyy 10xxxxxx
    // #4  000uuuzz zzzzyyyy yyxxxxxx <-> 11110uuu 10zzzzzz 10yyyyyy 10xxxxxx

    if( chr < 0 ) {
      err = EILSEQ;
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
      err = EILSEQ;
    } else {
      err = qio_channel_write_amt(false, ch, mbs, got);
    }
#else
    err = ENOSYS;
#endif
  }

  return err;
}

/*
void free_conv_spec(conv_spec_t* spec)
{
  qio_free(spec->allow_chars);
  qio_free(spec->literal);
  spec->allow_chars = NULL;
  spec->literal = NULL;
}


// This could be changed to use tables, but the current version
// favors simplicity&flexibility of implementation over speed.
static const char* space = " \f\n\r\t\v";
static const char* bin_num = "01";
static const char* oct_num = "01234567";
static const char* dec_num = "0123456789";
static const char* lower_hex_num = "0123456789abcdef";
static const char* upper_hex_num = "0123456789ABCDEF";
static const char* hex_num = "0123456789ABCDEFabcdef";
static const char* pound_conv = "#.";

static inline int istype(char x, const char* allow)
{
  while( *allow ) {
    if( *allow == x ) return 1;
    allow++;
  }
  return 0;
}

static inline void init_conv(conv_spec_t* spec_out)
{
  memset(spec_out, 0, sizeof(conv_spec_t));

  spec->min_width = 0;
  spec->max_width = 0x7fffffffffffffffLL;
  spec->pad_character = ' ';
  spec->pad_on_right = 0;
  spec->base = 10;
  spec->radix_character = '.';
  spec->uppercase_base = 0;
  spec->uppercase_real = 0;
  spec->print_int_precision = -1;
  spec->print_radix_precision = -1;
  spec->print_radix_precision = -1;
  spec->print_exponent_precision = -1;
  spec->store_base = 0;
  spec->specifier = SP_SKIP;
  spec->sign_type = 0;
  spec->uppercase = 0;
  spec->no_argument = 0;
  spec->allow_chars = NULL;
  spec->literal = NULL;
}

err_t build_conv(const char* fmt, ptrdiff_t* parsed_out, conv_spec_t* spec_out)
{
  ptrdiff_t i;
  ptrdiff_t num_before, num_after;
  char* endptr;
  long int val;

  memset(spec_out, 0, sizeof(conv_spec_t));

  i = 0;

  // do we have a ####.#### conversion to match?
  if( fmt[i] == '#' ) {
    // how many ### do we have before a . ?
    for( ; fmt[i] == '#'; i++ ) ;
    num_before = i;
    num_after = 0;

    if( fmt[i] == '.' ) {
      i++; // pass '.'
      for( ; fmt[i] == '#'; i++ ) ;
      num_after = i - num_before - 1;
    }

    spec_out->specifier = SP_REAL;
    spec_out->allow_chars = pound_conv;
    spec_out->base = 10;
    spec_out->print_int_precision = num_before;
    spec_out->print_radix_precision = num_after;
    *parsed_out = i;
    return 0;
  }

  // do we have a % conversion to match?
  if( fmt[i] == '%' ) {
    i++; // pass %

    if( fmt[i] == '%' ) {
      spec_out->specifier = SP_LITERAL;
      spec_out->literal_char = '%';
      *parsed_out = i;
      return 0;
    }

    if( fmt[i] == '#' && fmt[i+1] == '#' ) {
      spec_out->specifier = SP_LITERAL;
      spec_out->literal_char = '#';
      *parsed_out = i;
      return 0;
    }

    // Read some flags.
    for( ; fmt[i]; i++ ) {
      if( fmt[i] == '#' ) spec_out->store_base = 1;
      else if( fmt[i] == '0' ) {
        spec_out->pad_character = '0';
      } else if( fmt[i] == '-' ) {
        spec_out->pad_on_right = 1;
      } else if( fmt[i] == ' ' ) {
        spec_out->sign_type = 2;
      } else if( fmt[i] == '+' ) {
        spec_out->sign_type = 1;
      } else {
        break;
      }
    }
    // 'a - overrides a 0 if both are given'
    if( spec_out->pad_on_right ) spec_out->pad_character = ' ';

    // OK, now fmt[i] is the start of the integer-width area.
    val = strtol(&fmt[i], &endptr, 10);
    if( endptr != &fmt[i] ) {
      // advance past whatever number we got
      i += endptr - &fmt[i];
      if( val < 0 || val == LONG_MIN || val == LONG_MAX ) {
        *parsed_out = i;
        return EINVAL; // bad precision!
      }
      spec_out->print_int_precision = val;
    }

    if( fmt[i] == '.' ) {
      i++; // pass '.'
      // OK, now fmt[i] is the start of the radix-width area.
      val = strtol(&fmt[i], &endptr, 10);
      if( endptr != &fmt[i] ) {
        // advance past whatever number we got
        i += endptr - &fmt[i];
        if( val < 0 || val == LONG_MIN || val == LONG_MAX ) {
          *parsed_out = i;
          return EINVAL; // bad precision!
        }
        spec_out->print_radix_precision = val;
      }
    }

    // Now read a base modifier
    spec_out->base = 10;
    if( fmt[i] == 'b' ) {
      i++;
      spec_out->base = 2;
    } else if( fmt[i] == 'o' ) {
      i++;
      spec_out->base = 8;
    } else if( fmt[i] == 'd' ) {
      i++;
      spec_out->base = 10;
    } else if( fmt[i] == 'x' ) {
      i++;
      spec_out->base = 16;
    } else if( fmt[i] == 'X' ) {
      i++;
      spec_out->base = 16;
      spec_out->uppercase_base = 1;
    }

    // now read a conversion specifier
    if( fmt[i] == 'i' ) {
      i++;
      spec_out->store_base = 1;
      spec_out->specifier = SP_INTEGER;
    } else if( fmt[i] == 'g' ) {
      i++;
      spec_out->specifier = SP_REAL_G;
    } else if( fmt[i] == 'G' ) {
      i++;
      spec_out->specifier = SP_REAL_G;
      spec_out->uppercase_real = 1;
    } else if( fmt[i] == 'e' ) {
      i++;
      spec_out->specifier = SP_REAL_E;
    } else if( fmt[i] == 'E' ) {
      i++;
      spec_out->specifier = SP_REAL_E;
      spec_out->uppercase_real = 1;
    } else if( fmt[i] == 'f' ) {
      i++;
      spec_out->specifier = SP_REAL_F;
    } else if( fmt[i] == 'F' ) {
      i++;
      spec_out->specifier = SP_REAL_F;
      spec_out->uppercase_real = 1;
    } else if( fmt[i] == 'c' ) {
      spec_out->specifier = SP_CHAR;
    } else if( fmt[i] == 's' ) {
      if( spec_out->print_int_precision >= 0 ) {
        spec_out->max_width = spec_out->print_int_precision;
      }
      spec_out->specifier = SP_STRING;
    } else if( fmt[i] == '[' ) {
      
      // read the character set. TODO
      return ENOSYS;

      if( spec_out->print_int_precision >= 0 ) {
        spec_out->max_width = spec_out->print_int_precision;
      }
      spec_out->specifier = SP_STRING;
    }

  }

  // do we have any whitespace to match?
  if( istype(fmt[i], space) ) {
    // pass any whitespace characters.
    for( ; fmt[i] && istype(space, fmt[i]); i++ ) ;

    spec_out->specifier = SP_SKIP;
    spec_out->allow_chars = space;
    spec_out->no_argument = 1;
    *parsed_out = i;
    return 0;
  }
}


err_t qio_vparse_int32(qio_channel_t* ch,
                       conv_spec_t spec*,
                       ptrdiff_t* amt_read, int32_t* ptr)
{
  // Figure out the length of the matching string.
  ptrdiff_t m = match_length(spec, 
}

err_t qio_vparse_uint32(qio_channel_t* ch,
                        conv_spec_t spec*,
                        ptrdiff_t* amt_read, int32_t* ptr)
{
}


err_t qio_vparse(const char threadsafe, qio_channel_t* ch,
                 conv_spec_t spec*, ptrdiff_t nspec,
                 ptrdiff_t* amt_read, va_list ap)
{
  err_t err;
  err_t newerr;
  void* start;
  void* end;
  int got;
  char success = 0;


  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  if( ch->type == QIO_CH_BUFFERED ) {
    err = qio_channel_begin_peek_cached(0, ch, &start, &end);
    if( err ) goto error;

    if( start != end ) {
      // Try to use this..
      //
      err = sys_vsnprintf(start, end - start, fmt, ap, &got);
      if( err ) success = 0;
      if( got > end - start ) success = 0;
      else {
        success = 1;
        start += got;
      }
    }

    newerr = qio_channel_end_peek_cached(0, ch, start);
    if( ! err ) err = newerr;

    if( success ) goto error;
  }

  // If the fast buffered way failed, or we're not using a buffered
  // channel, fall back on a slow, but reliable method.
  {
    char* tmp;
    err_t err;

    tmp = NULL;
    err = sys_vasprintf(&tmp, fmt, ap);
    if( err ) return err;

    err = qio_channel_write(0, ch, tmp, strlen(tmp), amt_written);
    qio_free(tmp);
  }

error:
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

err_t qio_channel_vprintf(const char threadsafe, qio_channel_t* ch,
                          const char* fmt, ptrdiff_t* amt_written, va_list ap);
{
  err_t err;
  err_t newerr;
  void* start;
  void* end;
  int got;
  char success = 0;


  if( threadsafe ) {
    err = qio_lock(&ch->lock);
    if( err ) {
      return err;
    }
  }

  if( ch->type == QIO_CH_BUFFERED ) {
    err = qio_channel_begin_peek_cached(0, ch, &start, &end);
    if( err ) goto error;

    if( start != end ) {
      // Try to use this..
      err = sys_vsnprintf(start, end - start, fmt, ap, &got);
      if( err ) success = 0;
      if( got > end - start ) success = 0;
      else {
        success = 1;
        start += got;
      }
    }

    newerr = qio_channel_end_peek_cached(0, ch, start);
    if( ! err ) err = newerr;

    if( success ) goto error;
  }

  // If the fast buffered way failed, or we're not using a buffered
  // channel, fall back on a slow, but reliable method.
  {
    char* tmp;
    err_t err;

    tmp = NULL;
    err = sys_vasprintf(&tmp, fmt, ap);
    if( err ) return err;

    err = qio_channel_write(0, ch, tmp, strlen(tmp), amt_written);
    qio_free(tmp);
  }

error:
  if( threadsafe ) {
    qio_unlock(&ch->lock);
  }

  return err;
}

*/


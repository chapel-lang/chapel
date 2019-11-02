#ifndef _encoding_support_H_
#define _encoding_support_H_

#include <stdlib.h>
#include <inttypes.h>
#include <wchar.h>

/* BEGIN UTF-8 decoder from http://bjoern.hoehrmann.de/utf-8/decoder/dfa */
// Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.
#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

// FIXME: this check is probably too naive
#if defined(__cplusplus)
  #if __cplusplus >= 201103L
    #define RESTRICT __restrict
  #endif
#else
    #define RESTRICT restrict
#endif

static const uint8_t utf8d[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
  8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
  0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
  0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
  0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
  1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
  1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
  1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
};

static inline
uint32_t qio_utf8_decode(uint32_t* RESTRICT state,
                         uint32_t* RESTRICT codep,
                         uint32_t byte) {
  uint32_t type = utf8d[byte & 0xff];

  *codep = (*state != UTF8_ACCEPT) ?
    (byte & 0x3fu) | (*codep << 6) :
    (0xff >> type) & (byte);

  *state = utf8d[256 + *state*16 + type];
  return *state;
}
/* END UTF-8 decoder from http://bjoern.hoehrmann.de/utf-8/decoder/dfa */


/* Returns the difference between two pointers,
   but returns 0 if either pointer is NULL.
 */
static inline intptr_t ptr_diff(void* a, void* b)
{
  if( a == NULL || b == NULL ) return 0;
  return ((intptr_t)a) - ((intptr_t)b);
}

static inline
int decode_char_buf_utf8(int32_t* RESTRICT chr, int* RESTRICT nbytes,
                         const char* buf, ssize_t buflen)
{
  const char* start = buf;
  const char* end = start + buflen;
  uint32_t codepoint=0, state;
  state = 0;
  while( buf != end ) {
    qio_utf8_decode(&state, &codepoint, *(const unsigned char*)buf);
    buf++;
    if (state <= 1) {
      break;
    }
  }
  if( state == UTF8_ACCEPT ) {
    *chr = codepoint;
    *nbytes = ptr_diff((void*) buf, (void*) start);
    return 0;
  } else {
    *chr = 0xfffd; // replacement character
    *nbytes = ptr_diff((void*) buf, (void*) start);
    return -1; // -1: EILSEQ
  }
}

static inline
int decode_char_buf_ascii(int32_t* RESTRICT chr, int* RESTRICT nbytes,
                          const char* buf, ssize_t buflen)
{
  const char* start = buf;
  const char* end = start + buflen;
  if( buf != end ) {
    *chr = (unsigned char) *buf;
    *nbytes = 1;
    return 0;
  } else {
    *chr = -1;
    *nbytes = 0;
    return -1;  // -1:EILSEQ
  }
}

static inline
int decode_char_buf_wctype(int32_t* RESTRICT chr, int* RESTRICT nbytes,
                           const char* buf, ssize_t buflen)
{
#ifdef HAS_WCTYPE_H
  mbstate_t ps;
  size_t got;
  wchar_t wch = 0;
  memset(&ps, 0, sizeof(mbstate_t));
  got = mbrtowc(&wch, buf, buflen, &ps);
  if( got == 0 ) {
    // We read a NUL.
    *chr = 0;
    *nbytes = 1;
    return 0;
  } else if( got == (size_t) -1 ) {
    // it contains an invalid multibyte sequence.
    // errno should be EILSEQ.
    *chr = -3; // invalid character... think 0xfffd for unicode
    *nbytes = 1;
    return -1;  // -1:EILSEQ
  } else if( got == (size_t) -2 ) {
    // continue as long as we have an incomplete char.
    *chr = -3; // invalid character... think 0xfffd for unicode
    *nbytes = 1;
    return -1;  // -1:EILSEQ
  } else {
    // OK!
    // mbrtowc already set the character.
    *chr = wch;
    *nbytes = got;
    return 0;
  }
#else
  return -2; // -2: ENOSYS
#endif
}

static inline
int validate_buf(const char *buf, ssize_t buflen) {
  int32_t cp;
  int nbytes;

  int offset = 0;

  while (offset<buflen) {
    if (decode_char_buf_utf8(&cp, &nbytes, buf+offset, buflen-offset) != 0) {
      return 1;  // invalid : probably return EILSEQ
    }
    offset += nbytes;
  }
  return 0;  // valid
}


#endif

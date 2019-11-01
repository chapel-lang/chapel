#include "utf8-decoder.h"

#include <stdlib.h>
#include <inttypes.h>

#include <wchar.h>

/* Returns the difference between two pointers,
   but returns 0 if either pointer is NULL.
 */
static inline intptr_t ptr_diff(void* a, void* b)
{
  if( a == NULL || b == NULL ) return 0;
  return ((intptr_t)a) - ((intptr_t)b);
}

static inline
int decode_char_buf_utf8(int32_t* restrict chr, int* restrict nbytes,
                         const char* buf, ssize_t buflen)
{
  const char* start = buf;
  const char* end = start + buflen;
  uint32_t codepoint=0, state;
  state = 0;
  while( buf != end ) {
    qio_utf8_decode(&state,
        &codepoint,
        *(const unsigned char*)buf);
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
    //QIO_RETURN_CONSTANT_ERROR(EILSEQ, "");
    return -1; // -1: EILSEQ
  }
}

static inline
int decode_char_buf_ascii(int32_t* restrict chr, int* restrict nbytes,
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
    //QIO_RETURN_CONSTANT_ERROR(EILSEQ, "");
    return -1;  // -1:EILSEQ
  }
}

static inline
int decode_char_buf_wctype(int32_t* restrict chr, int* restrict nbytes,
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
    //QIO_RETURN_CONSTANT_ERROR(EILSEQ, "");
    //return EILSEQ;
    return -1;
  } else if( got == (size_t) -2 ) {
    // continue as long as we have an incomplete char.
    *chr = -3; // invalid character... think 0xfffd for unicode
    *nbytes = 1;
    //QIO_RETURN_CONSTANT_ERROR(EILSEQ, "");
    //return EILSEQ;
    return -1;
  } else {
    // OK!
    // mbrtowc already set the character.
    *chr = wch;
    *nbytes = got;
    return 0;
  }
#else
  //QIO_GET_CONSTANT_ERROR(err, ENOSYS, "missing wctype.h");
  return -2; // -2: ENOSYS
#endif
}

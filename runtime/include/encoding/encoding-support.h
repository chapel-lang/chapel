/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _ENCODING_SUPPORT_H_
#define _ENCODING_SUPPORT_H_

// Note: This header is used by both the compiler and the runtime.

#include <sys/types.h>
#include <stdlib.h>
#include <inttypes.h>
#include <wchar.h>

#include "utf8-decoder.h"


/* Returns the difference between two pointers,
   but returns 0 if either pointer is NULL.
 */
static inline intptr_t chpl_enc_ptr_diff(void* a, void* b)
{
  if( a == NULL || b == NULL ) return 0;
  return ((intptr_t)a) - ((intptr_t)b);
}

/*
 * Decodes the char buffer `buf` using UTF8 encoding and writes the codepoint in
 * `chr`
 *
 * :arg nbytes: An out argument that stores number of bytes read when function
 *              returns
 * :arg buflen: Upper limit for number of bytes to read
 *
 * :returns: 0 if successful, -1 if illegal byte sequence
 */
static inline
int chpl_enc_decode_char_buf_utf8(int32_t* CHPL_ENC_RESTRICT chr,
                                  int* CHPL_ENC_RESTRICT nbytes,
                                  const char* buf, ssize_t buflen)
{
  const char* start = buf;
  const char* end = start + buflen;
  uint32_t codepoint=0, state;
  state = 0;
  while( buf != end ) {
    chpl_enc_utf8_decode(&state, &codepoint, *(const unsigned char*)buf);
    buf++;
    if (state <= 1) {
      break;
    }
  }
  if( state == UTF8_ACCEPT ) {
    *chr = codepoint;
    *nbytes = chpl_enc_ptr_diff((void*) buf, (void*) start);
    return 0;
  } else {
    *chr = 0xfffd; // replacement character
    *nbytes = chpl_enc_ptr_diff((void*) buf, (void*) start);
    return -1; // -1: EILSEQ
  }
}

/*
 * Decodes the char buffer `buf` using ASCII encoding and writes the codepoint
 * in `chr`
 *
 * :arg nbytes: An out argument that stores number of bytes read when function
 *              returns
 * :arg buflen: Upper limit for number of bytes to read
 *
 * :returns: 0 if successful, -1 if illegal byte sequence
 */
static inline
int chpl_enc_decode_char_buf_ascii(int32_t* CHPL_ENC_RESTRICT chr,
                                   int* CHPL_ENC_RESTRICT nbytes,
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

/*
 * Decodes the char buffer `buf` using the system's wctype header and writes the
 * codepoint * in `chr`
 *
 * :arg nbytes: An out argument that stores number of bytes read when function
 *              returns
 * :arg buflen: Upper limit for number of bytes to read
 *
 * :returns: 0 if successful, -1 if illegal byte sequence, -2 if the
 * system does not have wctype.h
 */
static inline
int chpl_enc_decode_char_buf_wctype(int32_t* CHPL_ENC_RESTRICT chr,
                                    int* CHPL_ENC_RESTRICT nbytes,
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

/*
 * Check if the bytes in the char buffer form a valid UTF8 sequence
 *
 * :arg buflen: Upper limit for number of bytes to read
 *
 * :returns: 0 if valid, -1 if illegal byte sequence
 */
static inline
int chpl_enc_validate_buf(const char *buf, ssize_t buflen) {
  int32_t cp;
  int nbytes;

  int offset = 0;

  while (offset<buflen) {
    if (chpl_enc_decode_char_buf_utf8(&cp, &nbytes, buf+offset, buflen-offset) != 0) {
      return -1;  // invalid : return EILSEQ
    }
    offset += nbytes;
  }
  return 0;  // valid
}

#endif


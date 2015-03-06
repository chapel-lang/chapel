/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef _QIO_STYLE_H_
#define _QIO_STYLE_H_

#include "sys_basic.h"
#include "qbuffer.h"

#include <limits.h>
#include <stdint.h>

#define QIO_NATIVE 1
#define QIO_BIG 2
#define QIO_LITTLE 3

typedef uint8_t style_char_t;


#define QIO_STYLE_ELEMENT_STRING 1
#define QIO_STYLE_ELEMENT_COMPLEX 2
#define QIO_STYLE_ELEMENT_ARRAY 3
#define QIO_STYLE_ELEMENT_AGGREGATE 4
#define QIO_STYLE_ELEMENT_TUPLE 5
#define QIO_STYLE_ELEMENT_BYTE_ORDER 6
#define QIO_STYLE_ELEMENT_IS_NATIVE_BYTE_ORDER 7


#define QIO_STRING_FORMAT_WORD 0
#define QIO_STRING_FORMAT_BASIC 1
#define QIO_STRING_FORMAT_CHPL 2
#define QIO_STRING_FORMAT_JSON 3
#define QIO_STRING_FORMAT_TOEND 4
#define QIO_STRING_FORMAT_TOEOF 5

#define QIO_COMPLEX_FORMAT_READ_ANY 0
#define QIO_COMPLEX_FORMAT_READ_STRICT 0x10

#define QIO_COMPLEX_FORMAT_ABI 0x0
#define QIO_COMPLEX_FORMAT_PARENS 0x1
#define QIO_COMPLEX_FORMAT_PART 0xf

#define QIO_ARRAY_FORMAT_SPACE 0
#define QIO_ARRAY_FORMAT_CHPL 1
#define QIO_ARRAY_FORMAT_JSON 2

#define QIO_AGGREGATE_FORMAT_BRACES 0
#define QIO_AGGREGATE_FORMAT_CHPL 1
#define QIO_AGGREGATE_FORMAT_JSON 2

#define QIO_TUPLE_FORMAT_CHPL 0
#define QIO_TUPLE_FORMAT_SPACE 1
#define QIO_TUPLE_FORMAT_JSON 2


#define QIO_STRSTYLE_VLEN (-10)
#define QIO_STRSTYLE_NULL_TERMINATED (-0x0100)

typedef struct qio_style_s {
  uint8_t binary;
  // binary style choices:
  uint8_t byteorder; // QIO_NATIVE, QIO_BIG, or QIO_LITTLE.
  // string binary style:
  // -1 -- 1 byte of length before
  // -2 -- 2 bytes of length before
  // -4 -- 4 bytes of length before
  // -8 -- 8 bytes of length before
  // -10 -- variable byte length before (hi-bit 1 means more, little endian)
  // -0x01XX -- read until terminator XX is read
  //  + -- nonzero positive -- read exactly this length.
  //  0 means write as is, read path cannot work
  int64_t str_style;

  // text style choices
  uint32_t min_width_columns; // minimum field width; default is 0.
                      // For floating point and integer printing,
                      //   min_width_columns == min # of characters == min # of bytes
                      //    (since those only print printable ASCII)
                      // For string printing, min_width is in
                      //   *screen positions*. If you want to print
                      //   a specific number of bytes, use binary I/O.
  uint32_t max_width_columns; // maxiumum field width in screen positions; default is UINT32_MAX.
                              // (used when printing)
  uint32_t max_width_characters; // maxiumum field width in characters; default is UINT32_MAX.
                                 // (used when scanning)
  uint32_t max_width_bytes; // maxiumum field width in bytes; default is UINT32_MAX.
                            // (used when scanning)


  // string_start, string_end, string_format ignored for binary
  // string_start/string_end only used with format>=1 when printing
  // string_end is used when scanning format==0.
  style_char_t string_start;
  style_char_t string_end;
      /* QIO_STRING_FORMAT_WORD  string is as-is; reading reads until whitespace.
       QIO_STRING_FORMAT_BASIC only escape string_end and \ with \
       QIO_STRING_FORMAT_CHPL  escape string_end \ ' " \n with \
                               and nonprinting characters c = 0xXY with \xXY
       QIO_STRING_FORMAT_JSON  escape string_end " and \ with \,
                               and nonprinting characters c = \uABCD
       QIO_STRING_FORMAT_TOEND string is as-is; reading reads until string_end;
                               returned string includes string_end.
       QIO_STRING_FORMAT_TOEOF string is as-is; reading reads until EOF;
                               returned string includes string_end.
     */
  uint8_t string_format;

  // numeric scanning/printing choices
  uint8_t base; // 0 is %i; other valid values are 2 8 10 16
  style_char_t point_char; // normally '.'
  style_char_t exponent_char; // normally 'e'; used for base <= 10
  style_char_t other_exponent_char; // normally 'p' or '@'; used for base > 10
  style_char_t positive_char; // normally '+'
  style_char_t negative_char; // normally '-'
  style_char_t i_char; // normally 'i', suffix for imaginary numbers
  uint8_t prefix_base; // read/write integral values preceeded by base prefix 0x 0b
                    // (if not base 10). When scanning, the base prefix is
                    // always allowed if base==0 (ie determine base from #).
                    // prefix_base does not apply to floating-point values
                    // when printing (ie base 16 values always start 0x).
                    // When scanning, prefix_base == 0 will prohibit
                    // hexadecimal floating point values.

  // numeric printing choices
  style_char_t pad_char; // pad with this character.; should be ' ' or '0'
                 // (but don't use '0' when leftjustify=1)
  uint8_t showplus; // 0 == nothing before positive numbers
                 // 1 == write positive numbers preceeded by positive_char
                 // 2 == write positive numbers preceeded by pad_char
  uint8_t uppercase; // numeric stuff is uppercase
  uint8_t leftjustify; // 1 == left, 0 == right

  // floating point options
  uint8_t showpoint; // integer floating point values include a decimal point
                     // with some level of precision (maybe just . or maybe .00000 for %g)
  uint8_t showpointzero;  // integer floating point values get a .0
                          // if they would otherwise have be printed without a .0

  int32_t precision; // for floating point, number after decimal point.
                     // or number of significant digits in realfmt 2.
  uint8_t realfmt; //0 -> print with %g; 1 -> print with %f; 2 -> print with %e

  // Other data type choices
  //
  // complex numbers: one of
  // QIO_COMPLEX_FORMAT_ABI    -- a + bi like Chapel
  // QIO_COMPLEX_FORMAT_PARENS -- (a,b) like C++
  // and optionally | in:
  // QIO_COMPLEX_FORMAT_READ_STRICT -- do not accept the other format when reading
  uint8_t complex_style;

  // arrays (not directly supported by QIO but used in Chapel)
  // QIO_ARRAY_FORMAT_SPACE space in 1st dimensions, \n in later dims
  // QIO_ARRAY_FORMAT_CHPL make it look like an anonymous array [1,3,4]
  // QIO_ARRAY_FORMAT_JSON make it look like a JSON array [1,2]
  uint8_t array_style;

  // aggregates (not directly supported by QIO but used in Chapel)
  // (includes records, classes, unions)
  // QIO_AGGREGATE_FORMAT_BRACES record/union:(a=1) class:{a=1}
  // QIO_AGGREGATE_FORMAT_CHPL call chpl constructor: new Something(a=1)
  // QIO_AGGREGATE_FORMAT_JSON show JSON object: {a:1, b:2}
  uint8_t aggregate_style;

  // tuples (not directly supported by QIO but used in Chapel)
  // QIO_TUPLE_FORMAT_SPACE space separates all values
  // QIO_TUPLE_FORMAT_CHPL make it look like (a,b,c)
  // QIO_TUPLE_FORMAT_JSON make it look like a JSON array [1,2]
  uint8_t tuple_style;
} qio_style_t;

typedef qio_style_t _qio_style_ptr_t;
typedef qio_style_t* qio_style_ptr_t;

// Chapel compiler does not allow type alias to rename type
// and have constructor, new, methods, etc;
// so we redefine it here to include...
#ifdef _chplrt_H_
typedef qio_style_t iostyle;
#endif


static inline
void qio_style_init_default(qio_style_t* s)
{
  memset(s, 0, sizeof(qio_style_t));

  s->binary = 0;
  s->byteorder = QIO_NATIVE;
  s->str_style = -10;

  s->min_width_columns = 0;
  s->max_width_columns = UINT32_MAX;
  s->max_width_characters = UINT32_MAX;
  s->max_width_bytes = UINT32_MAX;

  s->string_start = '\"';
  s->string_end = '\"';
  s->string_format = 0;

  s->base = 0;
  s->point_char = '.';
  s->exponent_char = 'e';
  s->other_exponent_char = 'p';

  s->positive_char = '+';
  s->negative_char = '-';
  s->i_char = 'i';

  s->prefix_base = 1;
  s->pad_char = ' ';
  s->showplus = 0;
  s->uppercase = 0;
  s->leftjustify = 0;

  s->showpoint = 0;
  s->showpointzero = 1;
  s->precision = -1; // use default printf values; usually 6 but more for base 16

  s->realfmt = 0;

  s->complex_style = 0;

  s->array_style = 0;
  s->aggregate_style = 0;
  s->tuple_style = 0;
}

static inline
qio_style_t qio_style_default(void)
{
  qio_style_t s;
  qio_style_init_default(&s);
  return s;
}

static inline
void qio_style_copy(qio_style_t* dst, const qio_style_t* src)
{
  qio_memcpy(dst, src, sizeof(qio_style_t));
}

static inline
qio_style_t* qio_style_dup(const qio_style_t* src)
{
  qio_style_t* ret = (qio_style_t*) qio_malloc(sizeof(qio_style_t));
  if( src ) qio_style_copy(ret, src);
  else qio_style_init_default(ret);
  return ret;
}

static inline
void qio_style_free(qio_style_t* style)
{
  qio_free(style);
}

#endif

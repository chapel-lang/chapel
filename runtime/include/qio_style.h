#ifndef _QIO_STYLE_H_
#define _QIO_STYLE_H_

#include "qbuffer.h"

#include <limits.h>
#include <stdint.h>

#define QIO_NATIVE 1
#define QIO_BIG 2
#define QIO_LITTLE 3

typedef uint8_t style_char_t;

#define QIO_STRING_FORMAT_WORD 0
#define QIO_STRING_FORMAT_BASIC 1
#define QIO_STRING_FORMAT_CHPL 2
#define QIO_STRING_FORMAT_JSON 3
#define QIO_STRING_FORMAT_TOEND 4

#define QIO_COMPLEX_FORMAT_READ_ANY 0
#define QIO_COMPLEX_FORMAT_READ_STRICT 0x10

#define QIO_COMPLEX_FORMAT_ABI 0x0
#define QIO_COMPLEX_FORMAT_PARENS 0x1
#define QIO_COMPLEX_FORMAT_PART 0xf

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
  int64_t str_style;

  // text style choices
  uint32_t min_width; // minimum field width; default is 1.
  uint32_t max_width; // maxiumum field width; default is UINT_MAX.
                    
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
     */
  uint8_t string_format;

  // numeric scanning/printing choices
  uint8_t base; // 0 is %i; other valid values are 2 8 10 16
  style_char_t point_char; // normally '.'
  style_char_t exponent_char; // normally 'e'; used for base <= 10
  style_char_t other_exponent_char; // normally 'p' or '@'; used for base > 10
  style_char_t positive_char; // normally '+'
  style_char_t negative_char; // normally '-'
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
  int32_t significant_digits; // maximum display precision for floating point.
  uint8_t realfmt; //0 -> print with %g; 1 -> print with %f; 2 -> print with %e

  // Other data type choices
  //
  // complex numbers: one of
  // QIO_COMPLEX_FORMAT_ABI    -- a + bi like Chapel
  // QIO_COMPLEX_FORMAT_PARENS -- (a,b) like C++
  // and optionally | in:
  // QIO_COMPLEX_FORMAT_READ_STRICT -- do not accept the other format when reading
  uint8_t complex_style;

  /*
  uint8_t spaces_after_sep; // automatically add/consume spaces after a
                            // delimiter other than '\n' and around
                            // record_after_field_name
                            // normally 1.

  style_char_t array_start_char; // normally '[' or '\0' for blank
  style_char_t array_end_char; // normally ']' or '\0' for blank
  style_char_t array_delim_char1; // sep 1-d array or elements in a row (normally ' ')
  style_char_t array_delim_char2; // sep 2-d array or row from row (normally '\n')
  style_char_t array_delim_char3; // sep 3-d array block from block (normally '\n)

  uint8_t array_include_index; // should we print array index?
  uint8_t array_include_domain; // should we print array domain?

  style_char_t tuple_start_char; // normally '('
  style_char_t tuple_end_char; // normally ')'
  style_char_t tuple_delim_char; // normally ', '

  style_char_t record_start_char; // normally '('
  style_char_t record_end_char; // normally ')'
  style_char_t record_delim_char; // normally ', '
  style_char_t record_after_field_name_char; // normally ' = '
  uint8_t record_print_field_names; // print record field names? normally 1
  uint8_t record_print_name; // print record name? normally 0

  style_char_t class_start_char; // normally '{'
  style_char_t class_end_char; // normally '}'
  style_char_t class_delim_char; // normally ', '
  style_char_t class_after_field_name_char; // normally ' = '
  uint8_t class_print_field_names; // print class fields names? normally 1
  uint8_t class_print_name; // print class name? normally 0
  */
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

  s->min_width = 1;
  s->max_width = UINT32_MAX;

  s->string_start = '\"';
  s->string_end = '\"';
  s->string_format = 0;

  s->base = 0;
  s->point_char = '.';
  s->exponent_char = 'e';
  s->other_exponent_char = 'p';

  s->positive_char = '+';
  s->negative_char = '-';
  s->pad_char = ' ';

  s->prefix_base = 1;
  s->showplus = 0;
  s->uppercase = 0;
  s->leftjustify = 0;

  s->showpoint = 0;
  s->showpointzero = 1;
  s->precision = -1; // use default printf values; usually 6 but more for base 16
  s->significant_digits = -1;

  s->realfmt = 0;

  s->complex_style = 0;

  /*
  s->spaces_after_sep = 1;

  s->array_start_char = '\0';
  s->array_end_char = '\0';
  s->array_delim_char1 = ' ';
  s->array_delim_char2 = '\n';
  s->array_delim_char3 = '\n';

  s->array_include_index = 0;
  s->array_include_domain = 0;

  s->tuple_start_char = '(';
  s->tuple_end_char = ')';
  s->tuple_delim_char = ',';

  s->record_delim_char = ',';
  s->record_start_char = '(';
  s->record_end_char = ')';
  s->record_after_field_name_char = '=';
  s->record_print_field_names = 1;
  s->record_print_name = 0;

  s->class_delim_char = ',';
  s->class_start_char = '(';
  s->class_end_char = ')';
  s->class_after_field_name_char = '=';
  s->class_print_field_names = 1;
  s->class_print_name = 0;
  */
}

static inline
qio_style_t qio_style_default(void)
{
  qio_style_t s;
  qio_style_init_default(&s);
  return s;
}

static inline
void qio_style_copy(qio_style_t* dst, qio_style_t* src)
{
  memcpy(dst, src, sizeof(qio_style_t));
}

static inline
qio_style_t* qio_style_dup(qio_style_t* src)
{
  qio_style_t* ret = qio_malloc(sizeof(qio_style_t));
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

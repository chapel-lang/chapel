#include <stdlib.h>
#include <ctype.h>
#include "chplrt.h"
#include "chplcast.h"

static _string _get_string_imag_part(_string s) {
  char* ps = s;
  while (*ps == ' ') // eat space
    ps++;
  if (*ps == '-' || *ps == '+') // eat sign
    ps++;
  if (!isdigit(*ps)) // test for digit
    return NULL;
  while (isdigit(*ps)) // eat integral part
    ps++;
  if (*ps == '.') { // test for decimal
    ps++;
    if (!isdigit(*ps)) // test for digit
      return NULL;
    while (isdigit(*ps)) // eat fractional part
      ps++;
  }
  if (*ps == 'e' || *ps == 'E') { // test for exponent
    ps++;
    if (*ps == '+' || *ps == '-') // test for exponent sign
      ps++;
    if (!isdigit(*ps)) // test for digit
      return NULL;
    while (isdigit(*ps)) // eat exponent part
      ps++;
  }
  while (*ps == ' ') // eat space
    ps++;
  if (*ps == '+' || *ps == '-') // check for imaginary part
    return ++ps;
  return NULL;
}

_complex64 _string_to_complex64(_string s) {
  _string im;
  _complex64 c = {(_real32)0, (_real32)0};
  c.re = (_real32)(atof(s));
  im = _get_string_imag_part(s);
  if (im)
    c.im = (_real32)(atof(im));
  return c;
}

_complex128 _string_to_complex128(_string s) {
  _string im;
  _complex128 c = {(_real64)0, (_real64)0};
  c.re = (_real64)(atof(s));
  im = _get_string_imag_part(s);
  if (im)
    c.im = (_real64)(atof(im));
  return c;
}

_complex256 _string_to_complex256(_string s) {
  _string im;
  _complex256 c = {(_real128)0, (_real128)0};
  c.re = (_real128)(atof(s));
  im = _get_string_imag_part(s);
  if (im)
    c.im = (_real128)(atof(im));
  return c;
}

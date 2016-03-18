/* A module to provide a "long double" type to Chapel that matches the
   underlying C compiler's "long double" type.
 */

require "longdouble.h";

extern type longdouble;

proc _cast(type t, ld: longdouble) where t == real(64) || t == real(32) ||
                                         t == int(64)  || t == uint(64) ||
                                         t == int(32)  || t == uint(32) ||
                                         t == int(16)  || t == uint(16) ||
                                         t == int(8)   || t == uint(8) {
  return __primitive("cast", t, ld);
}

proc +(ld: longdouble, d: real): longdouble
  return __primitive("+", ld, d);
proc -(ld: longdouble, d: real): longdouble
  return __primitive("-", ld, d);
proc *(ld: longdouble, d: real): longdouble
  return __primitive("*", ld, d);
proc /(ld: longdouble, d: real): longdouble
  return __primitive("/", ld, d);

proc +=(ref ld: longdouble, d: real) {
  ld = ld + d;
}
proc -=(ref ld: longdouble, d: real) {
  ld = ld - d;
}
proc *=(ref ld: longdouble, d: real) {
  ld = ld * d;
}
proc -=(ref ld: longdouble, d: real) {
  ld = ld / d;
}

proc =(ref ld: longdouble, d: real) {
  __primitive("=", ld, d);
}

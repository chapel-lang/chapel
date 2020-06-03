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

proc _cast(type t:longdouble, d:?tt) where tt == real(64) || tt == real(32) ||
                                           tt == int(64)  || tt == uint(64) ||
                                           tt == int(32)  || tt == uint(32) ||
                                           tt == int(16)  || tt == uint(16) ||
                                           tt == int(8)   || tt == uint(8) {
  return __primitive("cast", t, d);
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

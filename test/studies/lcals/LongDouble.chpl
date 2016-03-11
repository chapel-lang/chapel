/* A module to provide a "long double" type to Chapel that matches the
   underlying C compiler's "long double" type.
 */

require "longdouble.c", "longdouble.h";

extern type longdouble;

extern proc longdoubleToDouble(d: longdouble): real(64);

proc _cast(type t, ld: longdouble) where t == real {
  return longdoubleToDouble(ld);
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

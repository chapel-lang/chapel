proc acceptsUint(arg: uint) { }
proc checkPositiveIntUint() {
  param p = 11;
  acceptsUint(p);
}
checkPositiveIntUint();

proc acceptsReal(arg: real) { }
proc checkRealReal() {
  param p = 2.25:real(32);
  acceptsReal(p);
}
checkRealReal();

proc acceptsInt(arg: int) { }
proc checkIntInt() {
  param p = 33: int(16);
  acceptsInt(p);
}
checkIntInt();

proc checkUintUint() {
  param p = 44: uint(16);
  acceptsUint(p);
}
checkUintUint();

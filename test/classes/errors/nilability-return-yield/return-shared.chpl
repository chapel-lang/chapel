class C { }
var global = new shared C() : shared C?;
proc P(): shared C  return global;
var dum = P();

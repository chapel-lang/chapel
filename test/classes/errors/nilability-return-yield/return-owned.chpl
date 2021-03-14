class C { }
var global = new owned C() : owned C?;
proc P(): owned C  return global;
var dum = P();

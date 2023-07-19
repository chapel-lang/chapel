class C { }
var global = new unmanaged C() : unmanaged C?;
proc P(): unmanaged C do  return global;
var dum = P();

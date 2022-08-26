class C { }
var global = (new owned C() : borrowed C?).borrow();
proc P(): borrowed C  return global;
var dum = P();

class C { }
var global = (new owned C() : borrowed C?).borrow();
proc P(): borrowed C do  return global;
var dum = P();

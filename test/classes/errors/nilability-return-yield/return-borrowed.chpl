class C { }
var global = new borrowed C() : borrowed C?;
proc P(): borrowed C  return global;
var dum = P();

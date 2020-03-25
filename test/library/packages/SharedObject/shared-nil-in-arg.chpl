class C { }

proc foo(in sc: shared C?) { }

var sc = nil: shared C?;

foo(sc);

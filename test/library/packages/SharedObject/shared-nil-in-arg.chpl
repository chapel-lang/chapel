class C { }

proc foo(in sc: shared C?) { }

var sc = new shared(nil:unmanaged C?);

foo(sc);

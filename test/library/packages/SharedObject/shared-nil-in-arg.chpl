class C { }

proc foo(in sc: shared C?) { }

var sc = shared.create(nil:unmanaged C?);

foo(sc);

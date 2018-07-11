use SharedObject;

class C { }

proc foo(in sc: Shared(C)) { }

var sc = new Shared(nil:unmanaged C);

foo(sc);

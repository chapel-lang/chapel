use SharedObject;

class C { }

proc foo(in sc: Shared(C)) { }

var sc = new Shared(nil:C);

foo(sc);

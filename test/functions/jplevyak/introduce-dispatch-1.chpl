class C { var c; }
def C.f() { writeln(c); }
var a = new C(1);
a.f();
a = new C(2.0);
a.f();

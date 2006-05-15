class C { var c; }
fun C.f() { writeln(c); }
var a = C(1);
a.f();
a = C(2.0);
a.f();

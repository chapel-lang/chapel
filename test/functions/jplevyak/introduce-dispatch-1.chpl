class C { var c; }
function C.f() { writeln(c); }
var a;
a = C(1);
a.f();
a = C(2.0);
a.f();

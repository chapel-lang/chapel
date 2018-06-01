class C { var c; }
proc C.f() { writeln(c); }
var a = new borrowed C(1);
a.f();
a = new borrowed C(2.0);
a.f();

class C { var c; }
proc C.f() { writeln(c); }
var a = (new owned C(1)).borrow();
a.f();
a = (new owned C(2.0)).borrow();
a.f();

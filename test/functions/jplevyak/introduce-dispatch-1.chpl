class C { var c; }
proc C.f() { writeln(c); }
var aObj = new C(1); var a = aObj.borrow();
a.f();
var bObj = new C(2.0); a = bObj.borrow();
a.f();

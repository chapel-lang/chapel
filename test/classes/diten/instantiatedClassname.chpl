class C {
  param a: int = 1;
  param b: int = 2;
  param c: int = 3;
}

proc foo(c: borrowed C(?)) {
  writeln(c.b);
}

var own = new C(b = 1);
var c: borrowed C(b = 1) = own;
foo(c);

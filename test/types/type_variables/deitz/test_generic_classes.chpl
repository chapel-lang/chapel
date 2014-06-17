class C {
  param a: int;
  param b: int;
  param c: int = 1;
  param d: int = 2;
  param e: int = 3;
}

class D {
  param a: int;
  param b: int;
  var c: C(a, b);
}

var c = new C(5, 4, 1, 2, 3);
var d = new D(5, 4, c);

writeln(c);
writeln(d);

class C {
  param a: int = 1;
  param b: int = 2;
  param c: int = 3;
}

proc foo(c: C) {
  writeln(c.b);
}

var c: C(b = 1);
foo(c);

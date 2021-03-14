class C {
  type t;
  var x: t;
}

proc foo(c: borrowed C) {
  return c.x;
}

var A = [i in 1..4] new owned C(int, i);

writeln(A);

writeln(foo(A));

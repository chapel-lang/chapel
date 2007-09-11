class C {
  type t;
  var x: t;
}

def foo(c: C) {
  return c.x;
}

var A: [i in 1..4] C(int) = C(int, i);

writeln(A);

writeln(foo(A));

class C {
  type t;
  var x: t;
}

proc foo(c: C) {
  return c.x;
}

var A: [1..4] C(int) = [i in 1..4] new C(int, i);

writeln(A);

writeln(foo(A));

for a in A do delete a;

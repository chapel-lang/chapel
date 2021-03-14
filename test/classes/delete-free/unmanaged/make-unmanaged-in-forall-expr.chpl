class C {
  type t;
  var x: t;
}

proc foo(c: unmanaged C) {
  return c.x;
}

var A = [i in 1..4] new unmanaged C(int, i);

writeln(A);

writeln(foo(A));

for a in A do delete a;


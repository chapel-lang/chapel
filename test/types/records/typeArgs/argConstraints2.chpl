record R {
  type t;
}

proc foo(type t: R(?)) {
  writeln("generic R");
}

proc foo(type t: R(real)) {
  writeln("real R");
}

proc foo(type t: R) {
  writeln("int R");
}

writeln("foo");
//foo(R);
foo(R(int));
foo(R(real));
foo(R(complex));

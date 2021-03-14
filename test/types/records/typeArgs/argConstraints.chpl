record R {
  type t = int;
}

proc foo(type t: R(?)) {
  writeln("generic R");
}

proc foo(type t: R(real)) {
  writeln("real R");
}

proc foo(type t: R(int)) {
  writeln("int R");
}

writeln("foo");
foo(R);
foo(R(int));
foo(R(real));
foo(R(complex));

proc bar(type t: R(?)) {
  writeln("generic R");
}

proc bar(type t: R(real)) {
  writeln("real R");
}

proc bar(type t: R) {
  writeln("default R");
}

writeln("bar");
bar(R);
bar(R(int));
bar(R(real));
bar(R(complex));

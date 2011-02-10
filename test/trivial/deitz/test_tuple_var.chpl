proc foo(i) {
  writeln("foo");
  return i;
}

var (a,) = (foo(1),foo(2));

writeln(a);

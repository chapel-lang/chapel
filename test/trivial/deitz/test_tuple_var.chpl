proc foo(i) {
  writeln("foo");
  return i;
}

var (a,_) = (foo(1),foo(2));

writeln(a);

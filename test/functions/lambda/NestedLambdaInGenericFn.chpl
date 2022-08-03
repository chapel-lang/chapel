
proc test() {
  proc foo(x) {
    writeln(x);
    proc bar(n : int) { return n; }
    return bar;
  }

  const a = foo(8);
  writeln(a);
  const b = foo(3.1415);
  writeln(b);
  const c = foo('Hello!');
  writeln(c);
}
test();


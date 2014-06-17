proc foo(i : int) {
  writeln("foo of ", i);
  proc bar(i : int) {
    writeln("bar of ", i);
  }
  proc car(i : int) {
    writeln("car of ", i);
  }
  bar(i);
  car(i);
}

foo(5);

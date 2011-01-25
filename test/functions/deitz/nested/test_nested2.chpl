proc foo(i : int) {
  writeln("foo of ", i);
  proc bar(i : int) {
    writeln("bar of ", i);
    if (i > 1) then
      car(i - 1);
  }
  proc car(i : int) {
    writeln("car of ", i);
    if (i > 1) then
      bar(i - 1);
  }
  bar(i);
}

foo(5);

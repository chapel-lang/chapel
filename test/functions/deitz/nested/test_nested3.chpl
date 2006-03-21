function foo(i : int) {
  writeln("foo of ", i);
  function bar(i : int) {
    writeln("bar of ", i);
  }
  function car(i : int) {
    writeln("car of ", i);
  }
  bar(i);
  car(i);
}

foo(5);

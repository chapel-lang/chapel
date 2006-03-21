function foo(i : int) {
  writeln("foo of ", i);
  function bar(i : int) {
    writeln("bar of ", i);
    car(i);
  }
  function car(i : int) {
    writeln("car of ", i);
  }
  bar(i);
}

foo(5);

function foo(i : integer) {
  writeln("foo of ", i);
  function bar(i : integer) {
    writeln("bar of ", i);
  }
  function car(i : integer) {
    writeln("car of ", i);
  }
  bar(i);
  car(i);
}

foo(5);

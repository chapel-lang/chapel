function foo(i : integer) {
  writeln("foo of ", i);
  function bar(i : integer) {
    writeln("bar of ", i);
    if (i > 1) then
      car(i - 1);
  }
  function car(i : integer) {
    writeln("car of ", i);
    if (i > 1) then
      bar(i - 1);
  }
  bar(i);
}

foo(5);

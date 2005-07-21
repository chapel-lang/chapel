function bar() {
  function foo(i : integer = 1, j : integer = 2, k : integer = 3) {
    writeln("foo of ", i, ", ", j, ", ", k);
  }

  foo(4, 5, 6);
  foo(4, 5);
  foo(4);
  foo();
}

bar();

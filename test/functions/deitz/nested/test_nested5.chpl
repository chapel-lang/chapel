proc bar() {
  proc foo(i : int = 1, j : int = 2, k : int = 3) {
    writeln("foo of ", i, ", ", j, ", ", k);
  }

  foo(4, 5, 6);
  foo(4, 5);
  foo(4);
  foo();
}

bar();

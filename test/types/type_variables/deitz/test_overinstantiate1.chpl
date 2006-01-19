function foo(param i : integer) {
  writeln("in foo ", i);
  foo(i + 1);
}

foo(1);

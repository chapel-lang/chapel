proc foo(param i : int) {
  writeln("in foo ", i);
  foo(i + 1);
}

foo(1);

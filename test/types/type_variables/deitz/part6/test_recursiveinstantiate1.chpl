inline proc foo(param i : int) {
  writeln("in foo ", i);
  if i < 10 then
    foo(i + 1);
}

foo(1);

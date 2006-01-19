pragma "inline" function foo(param i : integer) {
  writeln("in foo ", i);
  if i < 10 then
    foo(i + 1);
}

foo(1);

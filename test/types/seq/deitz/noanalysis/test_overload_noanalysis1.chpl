function foo(i : integer, s : string) {
  writeln(i);
  writeln(s);
}

function foo(i : integer, j : integer) {
  writeln(i);
  writeln(j);
}

foo(1, 2);
foo(3, "four");

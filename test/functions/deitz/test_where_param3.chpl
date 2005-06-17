function foo(j : integer, parameter i : integer) where i == 1 {
  writeln("Alpha ", j);
}

function foo(j : integer, parameter i : integer) where i == 2 {
  writeln("Beta ", j);
}

foo(3, 1);
foo(4, 2);

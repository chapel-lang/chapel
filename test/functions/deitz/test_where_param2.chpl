function foo(parameter i : integer) {
  where i == 1;
  writeln("Alpha");
}

function foo(parameter i : integer) {
  where i == 2;
  writeln("Beta");
}

foo(1);
foo(2);

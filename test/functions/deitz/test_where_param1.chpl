proc foo(param i : int) where i == 1 {
  writeln("Alpha");
}

proc foo(param i : int) where i == 2 {
  writeln("Beta");
}

foo(1);
foo(2);

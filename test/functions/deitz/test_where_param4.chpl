proc foo(param i : int, param j : int) where i < j {
  writeln(i, " is less than ", j);
}

proc foo(param i : int, param j : int) where i == j {
  writeln(i, " is equal to ", j);
}

proc foo(param i : int, param j : int) where i > j {
  writeln(i, " is greater than ", j);
}

foo(2, 5);
foo(4, 4);
foo(6, 3);
foo(7, 3);

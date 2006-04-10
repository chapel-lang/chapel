fun foo(param i : int, param j : int) where i < j {
  writeln(i, " is less than ", j);
}

fun foo(param i : int, param j : int) where i == j {
  writeln(i, " is equal to ", j);
}

fun foo(param i : int, param j : int) where i > j {
  writeln(i, " is greater than ", j);
}

foo(2, 5);
foo(4, 4);
foo(6, 3);
foo(7, 3);

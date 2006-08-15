def foo(j : int, param i : int) where i == 1 {
  writeln("Alpha ", j);
}

def foo(j : int, param i : int) where i == 2 {
  writeln("Beta ", j);
}

foo(3, 1);
foo(4, 2);

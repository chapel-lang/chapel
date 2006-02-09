function foo(param rank : integer, x : (rank*integer)) {
  writeln(x);
}

var x : 2*integer;
foo(2, x);
foo(3, x);

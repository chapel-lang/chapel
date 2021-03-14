proc foo(param rank : int, x : rank*int) {
  writeln(x);
}

var x : 2*int;
foo(2, x);
foo(3, x);

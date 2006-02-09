function foo(param rank : integer, x : (rank*integer)) {
  writeln(x);
}

var x : 2*integer;
foo(2, x);
var y : 3*integer;
foo(3, y);

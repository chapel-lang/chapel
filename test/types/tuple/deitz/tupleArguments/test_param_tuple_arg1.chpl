proc foo(param rank : int, x : rank*int) {
  writeln(x);
}

var x : 2*int;
foo(2, x);
var y : 3*int;
foo(3, y);

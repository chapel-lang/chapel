proc g(x: [1..3] int) {
  writeln("in g([1..3] int)");
}
proc g(x: []) {
  writeln("in g([])");
}

var A: [1..3] int;

g(A);

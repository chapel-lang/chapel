proc foo(d: domain(int)) {
  writeln("indefinite domain of int = ", d);
}

proc foo(d: domain(2)) {
  writeln("2D arithmetic domain = ", d);
}

var d1: domain(2) = {1..5, 1..5};
var d2: domain(int);
d2 += 4;
d2 += 7;
foo(d1);
foo(d2);

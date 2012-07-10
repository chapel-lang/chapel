proc f(a: int): int {
  return 1;
}

var D = {1..5};
var A: [D] int;

A += f(A(D));

writeln(A);

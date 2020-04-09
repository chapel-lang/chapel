proc foo(n: int): [1..n] int {
  var A: [1..n] int = 1..n;
  return A;
}

var A = foo(5);
writeln(A);

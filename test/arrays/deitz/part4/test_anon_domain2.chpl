config var n : int = 8;

var A : [1..n, 1..n] int;

forall (i,j) in {1..n, 1..n} {
  A(i,j) = (i-1)*n + j;
}

writeln(A);

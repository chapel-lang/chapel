config var n : integer = 8;

var A : [1..n, 1..n] integer;

forall (i,j) in (1..n, 1..n) {
  A(i,j) = (i-1)*n + j;
}

writeln(A);

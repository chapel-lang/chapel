config var n : integer = 12;

var A : [1..n] integer;

forall i in (1..n) {
  A(i) = i;
}

writeln(A);

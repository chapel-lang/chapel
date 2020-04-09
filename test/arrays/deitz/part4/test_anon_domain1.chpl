config var n : int = 12;

var A : [1..n] int;

forall i in 1..n {
  A(i) = i;
}

writeln(A);

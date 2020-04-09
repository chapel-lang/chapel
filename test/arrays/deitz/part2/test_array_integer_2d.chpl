config var n : int = 4;

var D2 : domain(2) = {1..n, 1..n};

var A2 : [D2] int;

forall (i,j) in D2 {
  A2(i,j) = (i-1)*n+j;
}

writeln("A2:\n", A2);


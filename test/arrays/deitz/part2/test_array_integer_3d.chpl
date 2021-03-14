config var n : int = 4;

var D1 : domain(1) = {1..n};
var D2 : domain(2) = {1..n, 1..n};
var D3 : domain(3) = {1..n, 1..n, 1..n};

var A1 : [D1] int;
var A2 : [D2] int;
var A3 : [D3] int;

forall i in D1 {
  A1(i) = 0;
}

forall (i,j) in D2 {
  A2(i,j) = 0;
}

forall (i,j,k) in D3 {
  A3(i,j,k) = 0;
}

writeln("A1:\n", A1);
writeln("A2:\n", A2);
writeln("A3:\n", A3);

config var n = 8;

var D: domain(1) = {1..n};
var DInner: subdomain(D) = {2..n-1};

var A: [DInner] int;

forall i in DInner {
  A(i) = i;
}

writeln("A is: ", A);

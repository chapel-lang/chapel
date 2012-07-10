config const n = 5000000;

const D = {1..n*n};

var D1: sparse subdomain(D);
forall i in 0..n do
  D1 += max(int)-i;

forall i in 0..n by -1 do
  D1 -= max(int)-i;

writeln(D1);

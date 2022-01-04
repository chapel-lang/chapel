var A: [1..1] int;
ref refA = A[1];
var s$: sync bool;

begin { A[1] = 1; s$.writeEF(true); }

s$.readFE();

while refA == 0 { /* spin wait */ }

writeln("Finished spinning. refA = ", refA);

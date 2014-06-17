config const n = 9;

const D = {1..n, 1..n};

var S1: sparse subdomain(D);

var A1: [S1] real;

for i in 1..n do
  S1 += (i,i);

writeln("S1 is:\n", S1);

removeItAndProveIt(5,5);

removeItAndProveIt(3,3);

removeItAndProveIt(7,7);

removeItAndProveIt(1,1);

removeItAndProveIt(9,9);

removeItAndProveIt(2,2);

removeItAndProveIt(8,8);

removeItAndProveIt(6,6);

removeItAndProveIt(4,4);

proc removeItAndProveIt(i,j) {
  writeln("Removing (",i,",",j,")");
  S1 -= (i,j);
  writeln("S1 is:\n", S1);
}

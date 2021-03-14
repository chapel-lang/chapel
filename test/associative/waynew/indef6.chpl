use printHelp;

param SIZE = 10;

var idomain: domain(int);
var A: [idomain] real;

proc jam() {
  writelnSorted(idomain);
  writeln( "numIndices = ", idomain.size);
  writelnSortedByDom(A);
  writeln( "numElements = ", A.size);
}

for d in 1..SIZE do idomain.add( d);
for d in 1..SIZE do A[d] = d + d:real/10.0;
jam();

for d in 1..SIZE do 
  if (d%2 == 0) then idomain.remove( d);
jam();

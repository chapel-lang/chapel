param SIZE = 10;

var idomain: domain(int);
var A: [idomain] float;

def jam() {
  writeln( idomain);
  writeln( "numIndices = ", idomain.numIndices);
  writeln( A);
  writeln( "numElements = ", A.numElements);
}

for d in 1..SIZE do idomain.add( d);
for d in 1..SIZE do A[d] = d + d:float/10.0;
jam();

for d in 1..SIZE do 
  if (d%2 == 0) then idomain.remove( d);
jam();

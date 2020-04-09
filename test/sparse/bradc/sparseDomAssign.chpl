config var n: int = 5;

const D = {1..n, 1..n};
var S: sparse subdomain(D);

for i in 1..n do
  S += (i,i);

var S2: sparse subdomain(D);

S2 = S;

writeln("S is: ", S);
writeln("S2 is: ", S2);

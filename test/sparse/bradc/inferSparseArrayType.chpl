config var n: int = 5;

const D = {1..n, 1..n};
var S: sparse subdomain(D);

for i in 1..n do
  S += (i,i);

var A: [S] real;

for i in 1..n do
  A(i,i) = i;


writeln("A is:\n", A);
writeln("A.domain is:\n", A.domain);


var B = [(i,j) in S] -A(i,j);

writeln("B is:\n", B);
writeln("B.domain is:\n", B.domain);



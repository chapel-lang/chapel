

var D = {1..10, 1..10};
var SD: sparse subdomain(D);
var A: [D] real;

for i in 1..10 do
  SD += (i,i);

A[SD] = 1.0;

writeln(A);

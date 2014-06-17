config var n = 10;

const D = {1..n, 1..n};

var DSps: sparse subdomain(D) = for i in 1..n do (i,i);

writeln("DSps is: ", DSps);

for i in 1..n do
  DSps += (i,1);

writeln("DSps is: ", DSps);

for i in 1..n do
  DSps += (1,i);

writeln("DSps is: ", DSps);


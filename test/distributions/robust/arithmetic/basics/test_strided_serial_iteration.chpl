use driver;

const D: domain(2,int,true) dmapped Dist2D = {1..24 by 2, 1..24 by 2};

var A: [D] int;

for i in D do
  A(i) = 2;

writeln(A);

for i in D by 2 do
  A(i) = 4;

writeln(A);

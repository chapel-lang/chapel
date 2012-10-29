var D: domain(1, uint) = {2147483646..2147483650:uint};
writeln(D);
var A: [D] uint;

for i in D do
  A(i) = i;

for i in D do
  writeln(i);

writeln(A);

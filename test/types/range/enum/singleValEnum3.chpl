enum color { red };

var r = color.red..color.red;

writeln(r);
writeln(r.size);

for i in r do
  writeln(i);

forall i in r do
  writeln(i);

const D = {color.red..color.red};
const D2 = {r, r};

writeln(D);
for i in D do
  writeln(i);

writeln(D2);
for ij in D2 do
  writeln(ij);

var A: [D] int;
var B: [D2] int;
var C: [color.red..color.red] int;

A[color.red] = 1;
B[color.red, color.red] = 2;
C[color.red] = 3;

writeln(A);
writeln(B);
writeln(C);

forall a in A do
  writeln(a);

forall b in B do
  writeln(b);

forall c in C do
  writeln(c);


var r2: range(color);
writeln(r2);

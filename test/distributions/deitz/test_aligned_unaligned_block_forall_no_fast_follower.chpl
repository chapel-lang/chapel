use BlockDist;

config var n: int = 32;

var BD1 = new dmap(new Block(boundingBox={1..n}, dataParTasksPerLocale=1));
var BD2 = new dmap(new Block(boundingBox={1..n/2}, dataParTasksPerLocale=1));

var A: [{1..n} dmapped BD1] real;
var B: [{1..n} dmapped BD1] real;
var C: [{1..n} dmapped BD2] real;

for i in 1..n do
  A(i) = i;


writeln("aligned forall:");
writeln(for a in A do a.locale.id);
writeln(for b in B do b.locale.id);

forall (b,a) in zip(B,A) do
  b = a;

writeln(B);

writeln("unaligned forall:");
writeln(for a in A do a.locale.id);
writeln(for c in C do c.locale.id);

forall (c,a) in zip(C,A) do
  c = a;

writeln(C);

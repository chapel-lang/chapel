use BlockDist, Time;

const D = {1..10} dmapped Block(rank=1);
writeln("D is: ", D);
sleep(10);
var A: [D] real;

forall a in A do
  a = here.id;

writeln(A);

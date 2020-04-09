var D1: domain(1, int(32)) = {1..4:int(32)};
var D2: domain(1, int(64)) = {1..4:int};

var A: [D1] real;
var B: [D2] real = [i in D2] i;

writeln("B is: ", B);

forall (a,b) in zip(A,B) do
  a = b;

writeln("A is: ", A);

B = 0;

forall (b,a) in zip(B,A) do
  b = -a;

writeln("B is: ", B);

use BlockDist;

var D = Block.createDomain({1..10});

var A: [D] int;
var B: [D] int;

B = 1;

forall (a,b) in zip(A,B) {
  a = b;
}

writeln(A);

A = B;

writeln(A);

use BlockDist;

const D1 = blockDist.createDomain({1..10, 1..10});
const D2 = {1..10, 1..10};
var A: [D1] int = 2;
var B: [D2] int = 3;

proc foo(x: [], m = (min reduce x)) {
  writeln("min ", m);
}
foo(A);
foo(B);
foo(A, + reduce A);
foo(B, + reduce B);


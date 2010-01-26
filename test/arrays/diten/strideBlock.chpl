use BlockDist;
var Dist = new dist(new Block(rank=1, bbox=[1..10]));
var dom: domain(1, stridable=true) distributed Dist = [1..10 by 2];
var A: [dom] int = [i in dom] i;
writeln(A);
for i in dom {
  writeln("A(", i, ") is ", A(i), " on ", A(i).locale);
}

def foo(X, i) {
  writeln(X);
  X(i) = 0;
}

foo(A[1..10 by 4], 5);
writeln(A);

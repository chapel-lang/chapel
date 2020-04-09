use BlockDist;
var Dist = new dmap(new Block(rank=1, boundingBox={1..10}));
var dom: domain(1, stridable=true) dmapped Dist = {1..10 by 2};
var A: [dom] int = [i in dom] i;
writeln(A);
for i in dom {
  writeln("A(", i, ") is ", A(i), " on ", A(i).locale);
}

proc foo(X, i) {
  writeln(X);
  X(i) = 0;
}

foo(A[1..10 by 4], 5);
writeln(A);

use BlockDist;

var Dist = new dmap(new Block(rank=2, boundingBox={1..3, 1..3}));
var dom: domain(2) dmapped Dist = {1..3, 1..3};

writeln("numIndices: ", dom.size);

for i in {0..4, 0..4} {
  writeln("contains(", i, ") ", dom.contains(i));
}

for i in {0..4, 0..4} {
  writeln("order(", i, ") ", dom.indexOrder(i));
}

for i in {0..4, 0..4} {
  writeln("position(", i, ") ", dom.position(i));
}

var A: [dom] int = [(i,j) in dom] i*10+j;
var B: [1..9] int;

B = reshape(A, {1..9});
writeln(B);

var dist2 = new dmap(new Block(rank=1, boundingBox={1..9}));
var dom2: domain(1) dmapped dist2 = {1..9};
var C: [dom2] int;

C = reshape(A, {1..9});
writeln(C);

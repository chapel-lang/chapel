use BlockDist;

var dist = distributionValue(new Block(rank=2, bbox=[1..3, 1..3], idxType=int));
var dom: domain(2) distributed dist = [1..3, 1..3];

writeln("numIndices: ", dom.numIndices);

for i in [0..4, 0..4] {
  writeln("member(", i, ") ", dom.member(i));
}

for i in [0..4, 0..4] {
  writeln("order(", i, ") ", dom.order(i));
}

for i in [0..4, 0..4] {
  writeln("position(", i, ") ", dom.position(i));
}

var A: [(i,j) in dom] int = i*10+j;
var B: [1..9] int;

B = reshape(A, [1..9]);
writeln(B);

var dist2 = distributionValue(new Block(rank=1, bbox=[1..9], idxType=int));
var dom2: domain(1) distributed dist2 = [1..9];
var C: [dom2] int;

C = reshape(A, [1..9]);
writeln(C);

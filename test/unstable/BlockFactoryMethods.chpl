use BlockDist;

const dom = {1..6, 1..6},
      rng = (1..6, 1..6);

const a: [dom] int = 5,
      b: [2..7, 0..10 by 2] int(16) = 6;

testArray("Block value:", blockDist.createArray(dom, int, -1));
testArray("Block value:", blockDist.createArray((...rng), int, -1));
testArray("Block iter:", blockDist.createArray(dom, int, [(i, j) in dom] i + j));
testArray("Block iter:", blockDist.createArray((...rng), int, [(i, j) in dom] i + j));
testArray("Block array:", blockDist.createArray(dom, int, a));
testArray("Block array:", blockDist.createArray(dom, int, b));
testArray("Block array:", blockDist.createArray((...rng), int, a));
testArray("Block array:", blockDist.createArray((...rng), int, b));

proc testArray(test: string, in A: [] int) {
  writeln("\n", test);
  forall a in A do a += here.id;
  writeln(A);
  writeln(A.domain);
  writeln(A.type:string);
}

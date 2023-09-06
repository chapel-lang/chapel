use BlockDist;

const dom = {1..6, 1..6},
      rng = (1..6, 1..6);

const a: [dom] int = 5,
      b: [2..7, 0..10 by 2] int(16) = 6;

testArray("Block value:", Block.createArray(dom, int, -1));
testArray("Block value:", Block.createArray((...rng), int, -1));
testArray("Block iter:", Block.createArray(dom, int, [(i, j) in dom] i + j));
testArray("Block iter:", Block.createArray((...rng), int, [(i, j) in dom] i + j));
testArray("Block array:", Block.createArray(dom, int, a));
testArray("Block array:", Block.createArray(dom, int, b));
testArray("Block array:", Block.createArray((...rng), int, a));
testArray("Block array:", Block.createArray((...rng), int, b));

proc testArray(test: string, in A: [] int) {
  writeln("\n", test);
  forall a in A do a += here.id;
  writeln(A);
  writeln(A.domain);
  writeln(A.type:string);
}

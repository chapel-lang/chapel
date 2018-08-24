use CyclicDist;
use BlockDist;

proc testCyclic() {
  var D1: domain(1) dmapped Cyclic;

  var D2: domain(1) dmapped Cyclic(startIdx=1);

  type t = domain(1);
  var D3: t dmapped Cyclic(startIdx=1);

  var A:[1..10] int;
  var D4: A.domain.type dmapped Cyclic(startIdx=1);
}
testCyclic();

proc testBlock() {
  // This one doesn't work because the domain type won't have a boundingBox
  //var D1: domain(1) dmapped Block;

  var D2: domain(1) dmapped Block(boundingBox={1..10});

  type t = domain(1);
  var D3: t dmapped Block(boundingBox={1..10});

  var A:[1..10] int;
  var D4: A.domain.type dmapped Block(boundingBox={1..10});
}
testBlock();

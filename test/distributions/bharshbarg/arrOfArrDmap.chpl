use BlockDist, CyclicDist, BlockCycDist;

proc test(Space) {
  type Elem = [1..5] int;
  const WorkArr : [Space] Elem;

  forall b in WorkArr {
    for el in b {
      el = 1;
    }
    assert(b == 1);
  }
}

const Dom = {1..10};

var BlockSpace = Dom dmapped Block(boundingBox=Dom);
test(BlockSpace);
writeln("Success: Block");

var CyclicSpace = Dom dmapped Cyclic(startIdx=Dom.low);
test(CyclicSpace);
writeln("Success: Cyclic");

var BCSpace = Dom dmapped BlockCyclic(startIdx=Dom.low, blocksize=2);
test(BCSpace);
writeln("Success: BlockCyclic");

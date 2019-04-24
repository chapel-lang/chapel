
use BlockDist;
use CyclicDist;

proc main() {
  var D = {1..10, 1..10};
  var S = D dmapped Block(D);
  var A, B : [S] int;

  writeln("Block=Block: ", chpl__bulkTransferArray(A, B));

  {
    var C : [D] int;
    writeln("Block=DefaultRectangular: ", chpl__bulkTransferArray(A, C));
    writeln("DefaultRectangular=Block: ", chpl__bulkTransferArray(C, A));
  }

  {
    var CD = S dmapped Cyclic(S.low);
    var C : [CD] int;
    writeln("Block=Cyclic: ", chpl__bulkTransferArray(A, C));
  }
}


use BlockDist;
use StencilDist;
use CyclicDist;

config param distType = DistType.block;

enum DistType {
  block,
  stencil
}

proc main() {
  var D = {1..10, 1..10};
  var S = if distType == DistType.block then D dmapped Block(D)
          else D dmapped Stencil(D, fluff=(1,1));
  var A, B : [S] int;

  writeln("Dist=Dist: ", chpl__bulkTransferArray(A, B));

  {
    var C : [D] int;
    writeln("Dist=DefaultRectangular: ", chpl__bulkTransferArray(A, C));
    writeln("DefaultRectangular=Dist: ", chpl__bulkTransferArray(C, A));
  }

  {
    var CD = S dmapped Cyclic(S.low);
    var C : [CD] int;
    writeln("Dist=Cyclic: ", chpl__bulkTransferArray(A, C));
  }
}

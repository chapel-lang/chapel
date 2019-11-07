
use Random;

use driver;
use driver_domains;
use driver_arrays;

config const seed = 1234;

config const debug = false;

proc DEBUG() { if debug then writeln(); }
proc DEBUG(const args ...?n) {
  if debug then writeln((...args));
}

test(DistType.default);
test(DistType.block);
test(DistType.cyclic);

proc test(param DT : DistType) {
  DEBUG();
  DEBUG("==================================================");
  DEBUG(DT:string, " VS ", distType:string);
  DEBUG();
  const (oneD, _, threeD, _, _) = setupDistributions(DT);

  const localOne   : domain(1) dmapped oneD   = Space1;
  const localThree : domain(3) dmapped threeD = Space3;

  var arrOne   : [localOne]   int;
  var arrThree : [localThree] int;

  // DT.1D = distType.2D[rankchange];
  assign(arrOne, A2D, 1);

  // distType.2D[rankchange] = DT.1D
  assign(A2D, arrOne, 1);

  // DT.3D[rankchange] = distType.4D[rankchange]
  assign(arrThree, A4D, 2);

  // distType.4D[rankchange] = DT.3D[rankchange]
  assign(A4D, arrThree, 2);
  DEBUG("==================================================");
}

proc assign(left, right, param rank:int) {
  const leftDom  = {(...left.domain.dims())};
  const rightDom = {(...right.domain.dims())};

  proc buildSlice(dom, param targetRank:int) {
    param numCollapsed = dom.rank - targetRank;
    param numFull      = dom.rank - numCollapsed;

    if numCollapsed == 0 {
      var ret : numFull * dom.dim(0).type;
      return ret;
    } else {
      var collapsed : numCollapsed * dom.dim(0).idxType;
      var full      : numFull * dom.dim(0).type;
      var ret = ((...collapsed), (...full));
      return ret;
    }
  }

  proc fillSlice(ref slice, A, B) {
    for param i in 0..A.rank-1 {
      slice(i) = if isRange(slice(i)) then A.dim(0)[B.dim(0)]
                 else A.dim(0).first;
    }
  }

  var lslice = buildSlice(leftDom, rank);
  var rslice = buildSlice(rightDom, rank);

  fillSlice(lslice, leftDom, rightDom);
  fillSlice(rslice, rightDom, leftDom);

  DEBUG();
  DEBUG("--------------------------------------------------");
  DEBUG("Target rank = ", rank);
  DEBUG();
  DEBUG("LHS type  : ", left.type:string);
  DEBUG("LHS dom   : ", leftDom);
  DEBUG("LHS slice : ", lslice);
  DEBUG();
  DEBUG("RHS type  : ", right.type:string);
  DEBUG("RHS dom   : ", rightDom);
  DEBUG("RHS slice : ", rslice);

  assign(left, lslice, right, rslice);
  DEBUG();
  DEBUG("SUCCESS");

  DEBUG("--------------------------------------------------");
}

proc assign(left, lslice, right, rslice) {
  // make copies
  var LHS = left;
  var RHS = right;

  fillRandom(RHS, seed);

  ref leftView = LHS[(...lslice)];
  ref rightView = RHS[(...rslice)];

  leftView = rightView;

  forall (L, R) in zip(leftView, rightView) {
    assert(L == R);
  }
}

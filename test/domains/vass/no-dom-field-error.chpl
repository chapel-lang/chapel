config const n=2, m=3;
var D_orig: domain(2) = {1..n, 1..m};
const dist = new dmap(new GlobalDistribution());
var D: domain(2) dmapped dist = D_orig;
var a: [D] real;


class GlobalDistribution : BaseDist {}

class GlobalDomain : BaseDom {
  param rank : int;
  type idxType;
  param strides: strideKind;
  var ranges: rank * range(idxType);
}

class GlobalArray : BaseArr {}

proc GlobalDistribution.init() {}

proc GlobalDistribution.dsiClone(): unmanaged GlobalDistribution {
  return new unmanaged GlobalDistribution();
}

override proc GlobalDistribution.dsiNewRectangularDom(param rank: int,
                                            type idxType,
                                            param strides, inds)
                                      : GlobalDomain(rank, idxType, strides)
{
  return new unmanaged GlobalDomain(rank, idxType, strides, inds);
}

proc GlobalDomain.dsiSetIndices(arg_ranges: rank * range(idxType)): void {
  ranges = arg_ranges;
}

proc GlobalDomain.dsiGetIndices(): rank * range(idxType) {
  return ranges;
}

proc GlobalDomain.dsiBuildArray(type eltType, param initElts): GlobalArray {
  return new unmanaged GlobalArray();
}

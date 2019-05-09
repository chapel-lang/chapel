config const n=2, m=3;
var D_orig: domain(2) = {1..n, 1..m};
const dist = new dmap(new GlobalDistribution());
var D: domain(2) dmapped dist = D_orig;
var a: [D] real;


class GlobalDistribution : BaseDist {}

class GlobalDomain : BaseDom {
  param rank : int;
  type idxType;
  param stridable: bool;
  var ranges: rank * range(idxType);
}

class GlobalArray : BaseArr {}

proc GlobalDistribution.init() {}

proc GlobalDistribution.dsiClone(): unmanaged GlobalDistribution {
  return new unmanaged GlobalDistribution();
}

proc GlobalDistribution.dsiNewArithmeticDom(param rank: int,
                                            type idxType,
                                            param stridable: bool)
                                      : GlobalDomain(rank, idxType, stridable)
{
  return new GlobalDomain(rank, idxType, stridable);
}

proc GlobalDomain.init(param rank: int,
                               type idxType,
                               param stridable: bool) {
  this.rank = rank;
  this.idxType = idxType;
  this.stridable = stridable;
}

proc GlobalDomain.dsiSetIndices(arg_ranges: rank * range(idxType)): void {
  ranges = arg_ranges;
}

proc GlobalDomain.dsiGetIndices(): rank * range(idxType) {
  return ranges;
}

proc GlobalDomain.dsiBuildArray(type eltType): GlobalArray {
  return new GlobalArray();
}

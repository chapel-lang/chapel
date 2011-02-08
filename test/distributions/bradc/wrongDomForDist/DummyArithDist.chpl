class MyDist : BaseDist {
  proc dsiNewArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    return new MyDom(rank=rank, idxType=idxType);
  }

  proc dsiClone() return new MyDist();
}

class MyDom : BaseArithmeticDom {
  param rank: int;
  type idxType = int(32);

  proc dsiGetIndices() {
    var r: range;
    return r;
  }

  proc dsiSetIndices(ind) {
  }
}

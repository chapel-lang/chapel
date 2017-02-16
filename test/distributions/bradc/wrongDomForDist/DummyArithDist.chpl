class MyDist : BaseDist {
  proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool) {
    return new MyDom(rank=rank, idxType=idxType);
  }

  proc dsiClone() return new MyDist();
}

class MyDom : BaseRectangularDom {
  param rank: int;
  type idxType = int(32);
  const dist: MyDist;

  proc dsiGetIndices() {
    var r: range;
    return r;
  }

  proc dsiSetIndices(ind) {
  }
}

class MyDist : BaseDist {
  proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool) {
    return new MyDom(rank=rank, idxType=idxType, stridable=stridable);
  }

  proc dsiClone() return new MyDist();
}

class MyDom : BaseRectangularDom {
  const dist: MyDist;

  proc dsiGetIndices() {
    var r: range;
    return r;
  }

  proc dsiSetIndices(ind) {
  }
}

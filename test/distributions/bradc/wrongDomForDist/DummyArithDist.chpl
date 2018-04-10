class MyDist : BaseDist {

  proc init() { }

  proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool, inds) {
    const dom = new MyDom(rank=rank, idxType=idxType, stridable=stridable);
    dom.dsiSetIndices(inds);
    return dom;
  }

  proc dsiClone() return new MyDist();
}

pragma "use default init"
class MyDom : BaseRectangularDom {
  const dist: MyDist;

  proc dsiGetIndices() {
    var r: range;
    return r;
  }

  proc dsiSetIndices(ind) {
  }
}

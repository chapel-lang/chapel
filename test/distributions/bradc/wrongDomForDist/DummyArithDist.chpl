class MyDist : BaseDist {

  proc init() { }

  proc dsiNewRectangularDom(param rank: int, type idxType, param stridable:
      bool, inds) {
    const dom = new unmanaged MyDom(rank=rank, idxType=idxType, stridable=stridable);
    dom.dsiSetIndices(inds);
    return dom;
  }

  proc dsiClone() return new unmanaged MyDist();
}

class MyDom : BaseRectangularDom {
  const dist: unmanaged MyDist?;

  proc dsiGetIndices() {
    var r: range;
    return r;
  }

  proc dsiSetIndices(ind) {
  }
}

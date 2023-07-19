class MyDist : BaseDist {

  proc init() { }

  proc dsiNewRectangularDom(param rank: int, type idxType,
                            param strides: strideKind, inds) {
    const dom = new unmanaged MyDom(rank=rank, idxType=idxType, strides=strides);
    dom.dsiSetIndices(inds);
    return dom;
  }

  proc dsiClone() do return new unmanaged MyDist();
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

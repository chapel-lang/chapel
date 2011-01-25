class MyDist : BaseDist {
  proc dsiNewAssociativeDom(type idxType) {
    return new MyDom();
  }

  proc dsiClone() return new MyDist();
}

class MyDom : BaseAssociativeDom {
  type idxType = int(32);

  proc dsiGetIndices() {
    var r: range;
    return r;
  }

  proc dsiSetIndices(ind) {
  }
}

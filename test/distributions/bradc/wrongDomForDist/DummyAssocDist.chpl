class MyDist : BaseDist {
  proc init() { }

  proc dsiNewAssociativeDom(type idxType, param parSafe:bool) {
    return new unmanaged MyDom();
  }

  proc dsiClone() return new unmanaged MyDist();
}

class MyDom : BaseAssociativeDom {
  type idxType = int(32);
  const dist: unmanaged MyDist?;

  proc dsiGetIndices() {
    var r: range;
    return r;
  }

  proc dsiSetIndices(ind) {
  }
}

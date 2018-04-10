class MyDist : BaseDist {
  proc init() { }

  proc dsiNewAssociativeDom(type idxType, param parSafe:bool) {
    return new MyDom();
  }

  proc dsiClone() return new MyDist();
}

pragma "use default init"
class MyDom : BaseAssociativeDom {
  type idxType = int(32);
  const dist: MyDist;

  proc dsiGetIndices() {
    var r: range;
    return r;
  }

  proc dsiSetIndices(ind) {
  }
}

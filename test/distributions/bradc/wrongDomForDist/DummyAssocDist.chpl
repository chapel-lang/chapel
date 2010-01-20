class MyDist : BaseDist {
  def dsiNewAssociativeDom(type idxType) {
    return new MyDom();
  }

  def dsiClone() return new MyDist();
}

class MyDom : BaseAssociativeDom {
  type idxType = int(32);

  def dsiGetIndices() {
    var r: range;
    return r;
  }

  def dsiSetIndices(ind) {
  }
}

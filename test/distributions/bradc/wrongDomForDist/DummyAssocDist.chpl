class MyDist : BaseDist {
  def newAssociativeDom(type idxType) {
    return new MyDom();
  }

  def clone() return new MyDist();
}

class MyDom : BaseAssociativeDom {
  type idxType = int(32);

  def getIndices() {
    var r: range;
    return r;
  }

  def setIndices(ind) {
  }
}

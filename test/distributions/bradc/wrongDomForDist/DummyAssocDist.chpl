class MyDist : BaseDist {
  def newAssociativeDom(type idxType) {
    return new MyDom();
  }
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

class MyDist : Distribution {
  def newAssociativeDomain(type idxType) {
    return new MyDom();
  }
}

class MyDom : BaseAssociativeDomain {
  type idxType = int(32);
  def buildEmptyDomain() {
    return new MyDom();
  }

  def getIndices() {
    var r: range;
    return r;
  }

  def setIndices(ind) {
  }
}

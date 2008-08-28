class MyDist : Distribution {
  def newDomain(param rank: int, type idxType = int(32), param stridable: bool = false) {
    return new MyDom();
  }
}

class MyDom : BaseDenseArithmeticDomain {
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

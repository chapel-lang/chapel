class MyDist : Distribution {
  def newArithmeticDomain(param rank: int, type idxType, param stridable: bool) {
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

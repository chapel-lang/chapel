class MyDist : BaseDist {
  def dsiNewArithmeticDom(param rank: int, type idxType, param stridable: bool) {
    return new MyDom(rank=rank, idxType=idxType);
  }

  def dsiClone() return new MyDist();
}

class MyDom : BaseArithmeticDom {
  param rank: int;
  type idxType = int(32);

  def dsiGetIndices() {
    var r: range;
    return r;
  }

  def dsiSetIndices(ind) {
  }
}

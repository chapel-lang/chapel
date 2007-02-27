class SingleLocaleSparseDomain: BaseDomain {
  param rank : int;
  type dim_type;
  var adomain : SingleLocaleArithmeticDomain(rank=rank, dim_type=dim_type);

  def getIndices() return 0;
  def setIndices(x);

  def buildArray(type eltType)
    return SingleLocaleSparseArray(eltType, rank, dim_type, dom=this);

  def buildEmptyDomain()
    return SingleLocaleSparseDomain(rank=rank, dim_type=dim_type, adomain=adomain);
}

class SingleLocaleSparseArray: BaseArray {
  type eltType;
  param rank : int;
  type dim_type;

  var dom : SingleLocaleSparseDomain(rank=rank, dim_type=dim_type);
}

def SingleLocaleSparseArray.writeThis(f: Writer) {
  halt("Sparse arrays are not implemented");
}

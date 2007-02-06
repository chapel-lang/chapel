class SingleLocaleSparseDomain: BaseDomain {
  param rank : int;
  type dim_type;
  var adomain : SingleLocaleArithmeticDomain(rank, dim_type);

  def _build_array(type eltType)
    return SingleLocaleSparseArray(eltType, rank, dim_type, dom=this);
}

class SingleLocaleSparseArray: BaseArray {
  type eltType;
  param rank : int;
  type dim_type;

  var dom : SingleLocaleSparseDomain(rank, dim_type);
}

def SingleLocaleSparseArray.writeThis(f: Writer) {
  halt("Sparse arrays are not implemented");
}

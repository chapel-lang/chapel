class _sdomain {
  param rank : int;
  type dim_type;
  var adomain : _adomain(rank, dim_type);

  def _build_array(type eltType)
    return _sarray(eltType, rank, dim_type, dom=this);
}

class _sarray: _abase {
  type eltType;
  param rank : int;
  type dim_type;

  var dom : _sdomain(rank, dim_type);
}

def _sarray.writeThis(f: Writer) {
  halt("Sparse arrays are not implemented");
}

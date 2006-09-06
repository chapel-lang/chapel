class _sdomain {
  param rank : int;
  var adomain : _adomain(rank);

  def _build_array(type elt_type)
    return _sarray(elt_type, rank, dom=this);
}

class _sarray: _abase {
  type elt_type;
  param rank : int;

  var dom : _sdomain(rank);
}

def fwrite(f : file, x : _sarray) {
  halt("Sparse arrays are not implemented");
}

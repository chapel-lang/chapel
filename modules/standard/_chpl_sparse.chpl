class _sdomain : _domain {
  param rank : int;
  var adomain : _adomain(rank);

  fun _build_array(type elt_type)
    return _sarray(elt_type, rank, dom=this);
}

record _sarray {
  type elt_type;
  param rank : int;

  var dom : _sdomain(rank);
}

fun fwrite(f : file, x : _sarray) {
  write("Sparse arrays are not implemented");
}

class _adomain : _domain {
  param rank : integer;
  var ranges : (rank * _aseq);

  function _forall() {
    // eventually this should be an iterator with inferred result
    if rank == 1 {
      var s : seq of integer;
      for i in ranges(1) do
        s._append_in_place(i);
      return s;
    } else {
      var s : seq of (rank*integer);
      for i in _cross(rank, ranges) do
        s._append_in_place(i);
      return s;
    }
  }

  function this(dim : integer)
    return ranges(dim);

  function _build_array(type elt_type)
    return _aarray(elt_type, rank, dom=this);

  function _build_sparse_domain()
    return _sdomain(rank, adomain=this);
}

class _aarray : value {
  type elt_type;
  param rank : integer;

  var dom : _adomain(rank);

  var info : (rank*2*integer);
  var size : integer;
  var data : _ddata(elt_type);

  function off(dim : integer) var
    return info(dim)(1);

  function blk(dim : integer) var
    return info(dim)(2);

  function initialize() {
    if dom == nil then return;
    for dim in 1..rank do
      off(dim) = dom(dim)._low;
    blk(rank) = 1;
    for dim in 1..rank-1 by -1 do
      blk(dim) = blk(dim+1) * dom(dim+1).length;
    size = blk(1) * dom(1).length;
    data = _ddata(elt_type, size);
    data.init();
  }

  function this(ind : (rank*integer)) var : elt_type {
    for i in 1..rank do
      if not _in(dom(i), ind(i)) {
        writeln("out of bounds error ", ind);
        exit(0);
      }
    var sum : integer;
    for i in 1..rank do
      sum = sum + (ind(i) - off(i)) * blk(i);
    return data(sum);
  }

  function this(ind : integer ...?rank) var : elt_type
    return this(ind);
}

function _aseq._translate(i : integer) : _aseq {
  return _low+i.._high+i by _stride;
}

function _adomain.translate(dim : integer ...?numDims) {
  var x = _adomain(rank);
  if (numDims != rank) {
    halt("***Error: Rank mismatch between domain and translate() arguments (", rank, " != ", numDims, ")***");
  }
  for i in 1..rank do
    x.ranges(i) = ranges(i)._translate(dim(i));
  return x;
}

function _aseq._interior(i : integer) : _aseq {
  var x : _aseq;
  if (i < 0) {
    x = _low.._low-1-i by _stride;
  } else if (i > 0) {
    x = _high+1-i.._high by _stride;
  }
  return x;
}

function _adomain.interior(dim : integer ...?numDims) {
  var x = _adomain(rank);
  if (numDims != rank) {
    halt("***Error: Rank mismatch between domain and interior() arguments (", rank, " != ", numDims, ")***");
  }
  for i in 1..rank do {
    if ((dim(i) > 0) and (ranges(i)._high+1-dim(i) < ranges(i)._low) or
        (dim(i) < 0) and (ranges(i)._low-1-dim(i) > ranges(i)._high)) {
      halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
    } 
    if (dim(i) == 0) {
      x.ranges(i) = ranges(i);
    } else {
      x.ranges(i) = ranges(i)._interior(dim(i));
    }
  }
  return x;
}

function _aseq._exterior(i : integer) : _aseq {
  var x : _aseq;
  if (i < 0) {
    x = _low+i.._low-1 by _stride;
  } else if (i > 0) {
    x = _high+1.._high+i by _stride;
  }
  return x;
}

function _adomain.exterior(dim : integer ...?numDims) {
  var x = _adomain(rank);
  if (numDims != rank) {
    halt("***Error: Rank mismatch between domain and exterior() arguments (", rank, " != ", numDims, ")***");
  }
  for i in 1..rank do {
    if (dim(i) == 0) {
      x.ranges(i) = ranges(i);
    } else {
      x.ranges(i) = ranges(i)._exterior(dim(i));
    }
  }
  return x;
}

function _aseq._expand(i : integer) : _aseq {
  return _low-i.._high+i by _stride;
}

function _adomain.expand(dim : integer ...?numDims) {
  var x = _adomain(rank);
  for i in 1..numDims do {
    if (numDims == rank) {
      if (dim(i) == 0) {
        x.ranges(i) = ranges(i);
      } else {
        x.ranges(i) = ranges(i)._expand(dim(i));
        if (x.ranges(i)._low > x.ranges(i)._high) {
          halt("***Error: Degenerate dimension created in dimension ", i, "***");
        }
      }
    } else if (numDims == 1) {
      for i in 1..rank do {
        if (dim(1) == 0) {
          x.ranges(i) = ranges(i);
        } else {
          x.ranges(i) = ranges(i)._low-dim(1)..ranges(i)._high+dim(1) by ranges(i)._stride;
        }
      }
    } else {
      halt("***Error: Rank mismatch between domain and expand() arguments (", rank, " != ", numDims, ")***");
    } 
  }
  return x;
}

function fwrite(f : file, x : _adomain) {
  fwrite(f, "[", x(1));
  for i in 2..x.rank do
    fwrite(f, ", ", x(i));
  fwrite(f, "]");
}

function fwrite(f : file, x : _aarray) {
  if x.rank == 1 {
    var first : bool = true;
    for i in x.dom(1) {
      if not first then
        fwrite(f, " ");
      else
        first = false;
      fwrite(f, x(i));
    }
  } else if x.rank == 2 {
    for i in x.dom(1) {
      var first : bool = true;
      for j in x.dom(2) {
        if not first then
          fwrite(f, " ");
        else
          first = false;
        fwrite(f, x(i,j));
      }
      fwriteln(f);
    }
  } else
    halt("Cannot write out arrays of more than two dimensions");
}

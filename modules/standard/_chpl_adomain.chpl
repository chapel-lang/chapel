//
// Arrays and domains
//

class _domain { }

function _build_domain(x : _domain)
  return x;

function _build_domain(ranges : _aseq ...?rank)
  return _adomain(rank, ranges);

function _build_domain_type(param rank : integer)
  return _adomain(rank);

function _build_domain_type(type ind_type)
  return _idomain(ind_type);

function _build_sparse_domain_type(dom)
  return dom._build_sparse_domain();

class _array { }

function _build_array_type(dom, type elt_type)
  return dom._build_array(elt_type);

///////////////

function _aseq._translate(i : integer) : _aseq {
  return _low+i.._high+i by _stride;
}

function _aseq._interior(i : integer) : _aseq {
  var x : _aseq = _low.._high by _stride;
  if (i < 0) {
    x = _low.._low-1-i by _stride;
  } else if (i > 0) {
    x = _high+1-i.._high by _stride;
  }
  return x;
}

function _aseq._exterior(i : integer) : _aseq {
  var x : _aseq = _low.._high by _stride;
  if (i < 0) {
    x = _low+i.._low-1 by _stride;
  } else if (i > 0) {
    x = _high+1.._high+i by _stride;
  }
  return x;
}

function _aseq._expand(i : integer) : _aseq {
  return _low-i.._high+i by _stride;
}


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

  function translate(dim : (rank*integer)) {
    var x = _adomain(rank);
    for i in 1..rank do
      x.ranges(i) = this(i)._translate(dim(i));
    return x;
  }

  function translate(dim : integer ...?numDims) {
    return translate(dim);
  }

  function interior(dim : (rank*integer)) {
    var x = _adomain(rank);
    for i in 1..rank do {
      if ((dim(i) > 0) and (this(i)._high+1-dim(i) < this(i)._low) or
          (dim(i) < 0) and (this(i)._low-1-dim(i) > this(i)._high)) {
        halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
      } 
      x.ranges(i) = this(i)._interior(dim(i));
    }
    return x;
  }

  function interior(dim : integer ...?numDims) {
    return interior(dim);
  }

  function exterior(dim : (rank*integer)) {
    var x = _adomain(rank);
    for i in 1..rank do {
      x.ranges(i) = this(i)._exterior(dim(i));
    }
    return x;
  }
  
  function exterior(dim : integer ...?numDims) {
    return exterior(dim);
  }

  function expand(dim : (rank*integer)) {
    var x = _adomain(rank);
    for i in 1..rank do {
      x.ranges(i) = ranges(i)._expand(dim(i));
      if (x.ranges(i)._low > x.ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
    return x;
  }  
  
  function expand(dim : integer ...?numDims) {
    var x = _adomain(rank);
    if (rank == numDims) {
      -- NOTE: would probably like to get rid of this assignment
      -- since domain assignment is/will eventually be nontrivial
      -- in cost;  yet returning expand(dim) currently causes
      -- problems as captured in trivial/shannon/condReturn3.chpl
      x = expand(dim);
    } else if (numDims == 1) {
      for i in 1..rank do {
        x.ranges(i) = ranges(i)._expand(dim(1));
      }
    } else {
      halt("***Error: Rank mismatch between domain and expand() arguments (", rank, " != ", numDims, ")***");
    }
    return x;
  }
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

function fwrite(f : file, x : _adomain) {
  fwrite(f, "[", x(1));
  for i in 2..x.rank do
    fwrite(f, ", ", x(i));
  fwrite(f, "]");
}

function fwrite(f : file, x : _aarray) {
  var i : (x.rank*integer);
  for dim in 1..x.rank do
    i(dim) = x.dom(dim)._low;
  label next while true {
    fwrite(f, x(i));
    if i(x.rank) < x.dom(x.rank)._high {
      fwrite(f, " ");
      i(x.rank) += x.dom(x.rank)._stride;
    } else {
      for dim in 1..x.rank-1 by -1 {
        if i(dim) < x.dom(dim)._high {
          i(dim) += x.dom(dim)._stride;
          for dim2 in dim+1..x.rank {
            fwrite(f, "\n");
            i(dim2) = x.dom(dim2)._low;
          }
          continue next;
        }
      }
      break;
    }
  }
}

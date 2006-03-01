function _build_array(dom, type elt_type)
  return dom._build_array(elt_type);

function _build_domain(x : _domain)
  return x;

function _build_domain(ranges : _aseq ...?rank)
  return _adomain(rank, ranges);

class _domain { }

class _adomain : _domain {
  param rank : integer;
  var ranges : (rank * _aseq);

  iterator _forall(dim : integer) : integer
    forall i in range(dim) do
      yield i;

  iterator _for(dim : integer) : integer
    for i in range(dim) do
      yield i;

  iterator _forall_help(param rank : integer) : (rank*integer) {
    if rank > 1 {
      for i in _forall(rank) do
        for x in _forall_help(rank-1) {
          var result : (rank*integer);
          for j in 1..rank-1 do
            result(j) = x(j);
          result(rank) = i;
          yield result;
        }
    } else if rank == 1 {
      var result : 1*integer;
      for i in _forall(1) {
        result(1) = i;
        yield result;
      }
    }
  }

  // eventually this should be an iterator with inferred result
  function _forall() {
    if rank == 1 {
      var s : seq of integer;
      for i in _forall_help(rank) do
        s._append_in_place(i(1));
      return s;
    } else {
      var s : seq of (rank*integer);
      for i in _forall_help(rank) do
        s._append_in_place(i);
      return s;
    }
  }

  function range(dim : integer)
    return ranges(dim);

  function _build_array(type elt_type)
    return _aarray(elt_type, rank, dom=this);
}

function _adomain.translate(dim : integer ...?rank) {
  var x = _adomain(rank);
  for i in 1..rank do
    x.ranges(i) = range(i).translate(dim(i));
  return x;
}

function _adomain.interior(dim : integer ...?rank) {
  var x = _adomain(rank);
  for i in 1..rank do {
    if ((-dim(i) > range(i)._high) or (dim(i) > range(i)._high)) {
      halt("***Error: Argument to 'interior' function out of range in dimension ", i, "***");
    } 
    if (dim(i) < 0) {
      x.ranges(i) = range(i)._low..range(i)._low-1-dim(i) by range(i)._stride;
    } else if (dim(i) == 0) {
      x.ranges(i) = ranges(i)._low..ranges(i)._high by ranges(i)._stride;
    } else if (dim(i) > 0) {
      x.ranges(i) = range(i)._high+1-dim(i)..range(i)._high by range(i)._stride;
    }
  }
  return x;
}

function _adomain.exterior(dim : integer ...?rank) {
  var x = _adomain(rank);
  for i in 1..rank do {
    if (dim(i) < 0) {
      x.ranges(i) = ranges(i)._low+dim(i)..ranges(i)._low-1 by ranges(i)._stride;
    } else if (dim(i) == 0) {
      x.ranges(i) = ranges(i)._low..ranges(i)._high by ranges(i)._stride;
    } else if (dim(i) > 0) {
      x.ranges(i) = ranges(i)._high+1..ranges(i)._high+dim(i) by ranges(i)._stride;
    }
  }
  return x;
}

function _adomain.expand(dim : integer ...?rank) {
  var x = _adomain(rank);
  for i in 1..rank do {
    if (dim(i) == 0) {
      x.ranges(i) = ranges(i)._low..ranges(i)._high by ranges(i)._stride;
    } else {
      x.ranges(i) = ranges(i)._low-dim(i)..ranges(i)._high+dim(i) by ranges(i)._stride;
      if (x.ranges(i)._low > x.ranges(i)._high) {
        halt("***Error: Degenerate dimension created in dimension ", i, "***");
      }
    }
  }
  return x;
}

function fwrite(f : file, x : _adomain) {
  fwrite(f, "[", x.range(1));
  for i in 2..x.rank do
    fwrite(f, ", ", x.range(i));
  fwrite(f, "]");
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
      off(dim) = dom.range(dim)._low;
    blk(rank) = 1;
    for dim in 1..rank-1 by -1 do
      blk(dim) = blk(dim+1) * dom.range(dim+1).length;
    size = blk(1) * dom.range(1).length;
    data = _ddata(elt_type, size);
    data.init();
  }

  function this(ind : (rank*integer)) var : elt_type {
    var sum : integer;
    for i in 1..rank do
      sum += (ind(i) - off(i)) * blk(i);
    return data(sum);
  }

  function this(ind : integer ...?rank) var : elt_type
    return this(ind);
}

function fwrite(f : file, x : _aarray) {
  if x.rank == 1 {
    var first : boolean = true;
    for i in x.dom._for(1) {
      if not first then
        fwrite(f, " ");
      else
        first = false;
      fwrite(f, x(i));
    }
  } else if x.rank == 2 {
    for i in x.dom._for(1) {
      var first : boolean = true;
      for j in x.dom._for(2) {
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

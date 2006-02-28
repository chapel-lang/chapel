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

record _aarray_info {
  var _off : integer;
  var _blk : integer;
}

class _aarray : value {
  type elt_type;
  param rank : integer;

  var dom : _adomain(rank);

  var info : _ddata(_aarray_info) = _ddata(_aarray_info, 16);
  var size : integer;
  var data : _ddata(elt_type) = _ddata(elt_type, 128);

  function initialize() {
    info.init();
    data.init();
    if dom == nil then return;
    var tmp : _aarray_info;
    tmp._off = dom.range(rank)._low;
    tmp._blk = 1;
    info(rank-1) = tmp;
    var i : integer = rank-2;
    while i >= 0 {
      var tmp4 : _aarray_info;
      tmp4._off = dom.range(i+1)._low;
      tmp4._blk = info(i+1)._blk *
                    ((dom.range(i+2)._high - dom.range(i+2)._low + 1)
                      / dom.range(i+2)._stride);
      info(i) = tmp4;
      i -= 1;
    }
    size = info(0)._blk *
             ((dom.range(1)._high - dom.range(1)._low + 1)
               / dom.range(1)._stride);
  }

  function this(ij : 2*integer) var : elt_type
    return data((ij(1) - info(0)._off) * info(0)._blk +
                (ij(2) - info(1)._off) * info(1)._blk);

  function this(i : integer, j : integer) var : elt_type
    return data((i - info(0)._off) * info(0)._blk +
                (j - info(1)._off) * info(1)._blk);

  function this(i : integer) var : elt_type
    return data((i - info(0)._off) * info(0)._blk);
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

var _hack_x : (integer, integer) = (1, 2);

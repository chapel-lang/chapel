function _array(dom, type elt_type) {
  return dom._build_array(elt_type);
}

class _adomain {
  param rank : integer;

  var info : _ddata(_aseq) = _ddata(_aseq, rank);

  function initialize() {
    info.init();
  }

  iterator _forall(dim : integer) : integer {
    forall i in info(dim-1)._low..info(dim-1)._high by info(dim-1)._stride do
      yield i;
  }

  iterator _for(dim : integer) : integer {
    for i in info(dim-1)._low..info(dim-1)._high by info(dim-1)._stride do
      yield i;
  }

  iterator _forall_help(param rank : integer) : (rank*integer) {
    if rank > 2 {
      for i in _forall(rank) do
        for x in _forall_help(rank-1) {
          var result : (rank*integer);
          for j in 1..rank-1 do
            result(j) = x(j);
          result(rank) = i;
          yield result;
        }
    } else if rank == 2 {
      for i in _forall(rank) do
        for x in _forall_help(1) {
          var result : (rank*integer);
          result(1) = x;
          result(2) = i;
          yield result;
        }
    } else if rank == 1 {
      for i in _forall(1) do
        yield i;
    }
  }

  iterator _forall() {
    var x : (rank*integer);
    for i in _forall_help(rank) {
      x = i;
      yield x;
    }
  }

  function range(dim : integer)
    return info(dim-1)._low..info(dim-1)._high by info(dim-1)._stride;

  function _build_array(type elt_type) {
    return _aarray(elt_type, rank, dom=this);
  }
}

function _build_domain(x : _adomain)
  return x;

function _build_domain(d1 : _aseq) {
  var x = _adomain(1);
  x.info(0) = d1;
  return x;
}

function _build_domain(d1 : _aseq, d2 : _aseq) {
  var x = _adomain(2);
  x.info(0) = d1;
  x.info(1) = d2;
  return x;
}

function fwrite(f : file, x : _adomain) {
  fwrite(f, "[");
  for i in 0..x.rank-1 {
    if i > 0 then
      fwrite(f, ", ");
    fwrite(f, x.info(i)._low, "..", x.info(i)._high);
    if x.info(i)._stride > 1 then
      fwrite(f, " by ", x.info(i)._stride);
  }
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
    tmp._off = dom.info(rank-1)._low;
    tmp._blk = 1;
    info(rank-1) = tmp;
    var i : integer = rank-2;
    while i >= 0 {
      var tmp4 : _aarray_info;
      tmp4._off = dom.info(i)._low;
      tmp4._blk = info(i+1)._blk *
                    ((dom.info(i+1)._high - dom.info(i+1)._low + 1)
                      / dom.info(i+1)._stride);
      info(i) = tmp4;
      i -= 1;
    }
    size = info(0)._blk *
             ((dom.info(0)._high - dom.info(0)._low + 1)
               / dom.info(0)._stride);
//    for i in 1..size do
//      _move(data(i-1), _init(elt_type));
  }


  function this(ij : 2*integer) var : elt_type {
    var ind : integer = (ij(1) - info(0)._off) * info(0)._blk +
                        (ij(2) - info(1)._off) * info(1)._blk;
    return data(ind);
  }

  function this(i : integer, j : integer) var : elt_type {
    var ind : integer = (i - info(0)._off) * info(0)._blk +
                        (j - info(1)._off) * info(1)._blk;
    return data(ind);
  }

  function this(i : integer) var : elt_type {
    var ind : integer = (i - info(0)._off) * info(0)._blk;
    return data(ind);
  }

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

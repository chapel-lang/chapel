record _adomain_info {
  var _low : integer;
  var _high : integer;
  var _stride : integer;
  var _alignment : integer;
}

pragma "instantiate multidimensional iterator"
class _adomain : value {
  param rank : integer;

  var info : _ddata(_adomain_info) = _ddata(_adomain_info, 16);

  function _set(dim : integer, x : _aseq) {
    var tmp : _adomain_info;
    tmp._low = x._low;
    tmp._high = x._high;
    tmp._stride = x._stride;
    tmp._alignment = x._low;
    info(dim-1) = tmp;
  }

  iterator _forall(dim : integer) : integer {
    var tmp : _adomain_info = info(dim-1);
    forall i in tmp._low..tmp._high by tmp._stride do
      yield i;
  }

  iterator _for(dim : integer) : integer {
    var tmp : _adomain_info = info(dim-1);
    for i in tmp._low..tmp._high by tmp._stride do
      yield i;
  }
}

class _adomain_lit : value {
  param rank : integer;

  var info : _ddata(_aseq) = _ddata(_aseq, 16);

  var dummy : integer;

  function _set(dim : integer, x : _aseq) {
    info(dim-1) = x;
  }
}

function =(x : _adomain, y : _adomain_lit) {
  if x.rank != y.rank then
    halt("Domain ranks do not match");
  for dim in 1..x.rank do
    x._set(dim, y.info(dim-1));
  return x;
}

function =(x : _adomain, y : _adomain) {
  if x.rank != y.rank then
    halt("Domain ranks do not match");
  for dim in 1..x.rank do
    x.info(dim-1) = y.info(dim-1);
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

  function myinit() {
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

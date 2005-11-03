use _chpl_seq;
use _chpl_htuple;
use _chpl_file;

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

  var dom : _adomain(2);

  var info : _ddata(_aarray_info) = _ddata(_aarray_info, 16);
  var size : integer;
  var data : _ddata(elt_type) = _ddata(elt_type, 16);

  function myinit() {
    var dtmp : _adomain_info = dom.info(rank-1);
    var tmp : _aarray_info;
    tmp._off = dtmp._low;
    tmp._blk = 1;
    info(rank-1) = tmp;
    var i : integer = rank-2;
    while i >= 0 {
      var dtmp2 : _adomain_info = dom.info(i);
      tmp._off = dtmp2._low;
      var dtmp3 : _adomain_info = dom.info(i+1);
      var tmp2 = info(i+1);
      var tmp4 : _aarray_info;
      tmp4._blk = tmp2._blk * ((dtmp3._high - dtmp3._low + 1) / dtmp3._stride);
      info(i) = tmp4;
      i -= 1;
    }
    var dtmp4 : _adomain_info = dom.info(1);
    var tmp3 = info(0);
    size = tmp3._blk * ((dtmp4._high - dtmp4._low + 1) / dtmp4._stride);
  }

  function this(i : integer, j : integer) var : elt_type {
    var ind : integer = (i - info(0)._off) * info(0)._blk +
                        (j - info(1)._off) * info(1)._blk;
    return data(ind);
  }

}

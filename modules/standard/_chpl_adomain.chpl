use _chpl_seq;
use _chpl_data;
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

  var info : _fdata(_adomain_info);

  function _set(dim : integer, x : _aseq) {
    info(dim-1)._low = x._low;
    info(dim-1)._high = x._high;
    info(dim-1)._stride = x._stride;
    info(dim-1)._alignment = x._low;
  }

  iterator _forall(dim : integer) : integer
    forall i in info(dim-1)._low..info(dim-1)._high by info(dim-1)._stride do
      yield i;

  iterator _for(dim : integer) : integer
    for i in info(dim-1)._low..info(dim-1)._high by info(dim-1)._stride do
      yield i;
}

class _adomain_lit : value {
  param rank : integer;

  var info : _fdata(_aseq);

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

  var info : _fdata(_aarray_info);
  var size : integer;
  var data : _fdata(elt_type);

  function myinit() {
    info(rank-1)._off = dom.info(rank-1)._low;
    info(rank-1)._blk = 1;
    var i : integer = rank-2;
    while i >= 0 {
      info(i)._off = dom.info(i)._low;
      info(i)._blk = info(i+1)._blk *
        ((dom.info(i+1)._high - dom.info(i+1)._low + 1) / dom.info(i+1)._stride);
      i -= 1;
    }
    size = info(0)._blk *
      ((dom.info(1)._high - dom.info(1)._low + 1) / dom.info(1)._stride);
  }

  function this(i : integer, j : integer) var : elt_type
    return data((i - info(0)._off) * info(0)._blk +
                (j - info(1)._off) * info(1)._blk);

}

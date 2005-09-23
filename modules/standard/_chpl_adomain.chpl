use _chpl_seq;
use _chpl_data;
use _chpl_htuple;

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

  iterator _forall() : (integer, integer) {
    forall i in _forall(1) {
      forall j in _forall(2) {
        yield (i, j);
      }
    }
  }
}

function write(x : _adomain) {
  write("[");
  var first : boolean = true;
  for i in 0..x.rank-1 {
    if not first then
      write(", ");
    write(x.info(i)._low);
    write("..");
    write(x.info(i)._high);
    if (x.info(i)._stride > 1) then
      write(" by ", x.info(i)._stride);
    first = false;
  }
  write("]");
}

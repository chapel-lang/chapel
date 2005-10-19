class adomain : value {
  param rank : integer;

  var low : _data(integer) = _data(integer, 2);
  var high : _data(integer) = _data(integer, 2);
  var stride : _data(integer) = _data(integer, 2);
  var alignment : _data(integer) = _data(integer, 2);

  function set(dim : integer, x : _aseq) {
    low(dim) = x._low;
    high(dim) = x._high;
    stride(dim) = x._stride;
    alignment(dim) = x._low;
  }

  iterator _forall(dim : integer) : integer {
    var alow : integer = low(dim);
    var ahigh : integer = high(dim);
    var astride : integer = stride(dim);
    forall i:integer in alow..ahigh by astride do
      yield i;
  }

  iterator _forall() : (integer, integer) {
    for i:integer in _forall(0) {
      for j:integer in _forall(1) {
        yield (i, j);
      }
    }
  }
}

function fwrite(f : file, x : adomain) {
  fwrite(f, "[");
  var first : boolean = true;
  var hi : integer = x.rank-1;
  for i:integer in 0..hi {
    if not first then
      fwrite(f, ", ");
    fwrite(f, x.low(i));
    fwrite(f, "..");
    fwrite(f, x.high(i));
    var str = x.stride(i);
    if (str != 1) then
      fwrite(f, " by ", str);
    first = false;
  }
  fwrite(f, "]");
}

var d : adomain(2);
d.set(0, 1..5);
d.set(1, 2..8 by 2);
writeln(d);

writeln(d._forall(0));
writeln(d._forall(1));
writeln(d._forall());

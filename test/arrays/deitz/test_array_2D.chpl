record myarray_info {
  var off : integer;
  var blk : integer;
}

class myarray : value {
  type elt_type;
  param rank : integer;

  var dom : _adomain(2);

  var info : _fdata(myarray_info);
  var size : integer;
  var data : _fdata(elt_type);

  function myinit() {
    info(rank-1).off = dom.info(rank-1)._low;
    info(rank-1).blk = 1;
    var i : integer = rank-2;
    while i >= 0 {
      info(i).off = dom.info(i)._low;
      info(i).blk = info(i+1).blk *
        ((dom.info(i+1)._high - dom.info(i+1)._low + 1) / dom.info(i+1)._stride);
      i -= 1;
    }
    size = info(0).blk *
      ((dom.info(1)._high - dom.info(1)._low + 1) / dom.info(1)._stride);
  }

  function this(i : integer, j : integer) var : elt_type
    return data((i - info(0).off) * info(0).blk +
                (j - info(1).off) * info(1).blk);

}

var d : domain(2) = [1..3, 1..3];

var a : myarray(integer,2);
a.dom = d;
a.myinit();

a(1,1) = 1;
a(1,2) = 2;
writeln(a(1,1));
writeln(a(1,2));
writeln(a(2,1));

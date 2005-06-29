record mydomain_info {
  var low : integer;
  var high : integer;
  var stride : integer;
  var alignment : integer;
}

class mydomain {
  parameter rank : integer;
  var info0 : mydomain_info;
  var info1 : mydomain_info;
  var info2 : mydomain_info;
--  var info : rank*mydomain_info;
  function dimInit(d : integer, low, high, stride) {
    select d {
      when 1 {
        info0.low = low;
        info0.high = high;
        info0.stride = stride;
        info0.alignment = stride;
      }
      when 2 {
        info1.low = low;
        info1.high = high;
        info1.stride = stride;
        info1.alignment = stride;
      }
      when 3 {
        info2.low = low;
        info2.high = high;
        info2.stride = stride;
        info2.alignment = stride;
      }
      otherwise {
        halt("domains are only supported up to rank 3");
      }
    }
--    info(d-1).low = low;
--    info(d-1).high = high;
--    info(d-1).stride = stride;
--    info(d-1).alignment = stride;
  }
  iterator dforall(d : integer) {
    var low = (info0.low, info1.low, info2.low);
    var high = (info0.high, info1.high, info2.high);
    var alow = low(d);
    var ahigh = high(d);
    forall i in alow..ahigh {
      yield i;
    }
  }
  iterator dfor(d : integer) {
    var low = (info0.low, info1.low, info2.low);
    var high = (info0.high, info1.high, info2.high);
    for i in low(d)..high(d) {
      yield i;
    }
  }
}

var D : mydomain = mydomain(rank=2);
D.dimInit(1, 1, 8, 1);
D.dimInit(2, 1, 16, 1);
writeln(D);

writeln(D.dforall(1));
writeln(D.dforall(2));

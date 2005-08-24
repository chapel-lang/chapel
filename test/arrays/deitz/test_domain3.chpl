record mydomain_info {
  var low : integer;
  var high : integer;
  var stride : integer;
  var alignment : integer;
}

class mydomain {
  param rank : integer;
  var info0 : mydomain_info;
  var info1 : mydomain_info;
  var info2 : mydomain_info;
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
  }
  iterator dforall(d : integer) {
    var low : integer;
    select d {
      when 1 do low = info0.low;
      when 2 do low = info1.low;
      when 3 do low = info2.low;
    }
    var high : integer;
    select d {
      when 1 do high = info0.high;
      when 2 do high = info1.high;
      when 3 do high = info2.high;
    }
    forall i in low..high {
      yield i;
    }
  }
  iterator dfor(d : integer) {
    var low : integer;
    select d {
      when 1 do low = info0.low;
      when 2 do low = info1.low;
      when 3 do low = info2.low;
    }
    var high : integer;
    select d {
      when 1 do high = info0.high;
      when 2 do high = info1.high;
      when 3 do high = info2.high;
    }
    for i in low..high {
      yield i;
    }
  }
}

var D : mydomain = mydomain(rank=2);
D.dimInit(1, 1, 3, 1);
D.dimInit(2, 1, 4, 1);
writeln(D);

writeln(D.dforall(1));
writeln(D.dforall(2));

writeln(D.dfor(1));
writeln(D.dfor(2));


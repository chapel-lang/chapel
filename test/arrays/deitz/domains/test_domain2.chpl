record mydomain_info {
  var low : int;
  var high : int;
  var stride : int;
  var alignment : int;
}

class mydomain {
  param rank : int;
  var info0 : mydomain_info;
  var info1 : mydomain_info;
  var info2 : mydomain_info;
  proc dimInit(d : int, low, high, stride) {
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
  iter dforall(d : int) : int {
    var low : int;
    select d {
      when 1 do low = info0.low;
      when 2 do low = info1.low;
      when 3 do low = info2.low;
    }
    var high : int;
    select d {
      when 1 do high = info0.high;
      when 2 do high = info1.high;
      when 3 do high = info2.high;
    }
    for i in low..high { // changed from forall to avoid error
      yield i;
    }
  }
  iter dfor(d : int) : int {
    var low : int;
    select d {
      when 1 do low = info0.low;
      when 2 do low = info1.low;
      when 3 do low = info2.low;
    }
    var high : int;
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

var D : borrowed mydomain(rank=2) = new borrowed mydomain(rank=2);
D.dimInit(1, 1, 3, 1);
D.dimInit(2, 1, 4, 1);
writeln(D);

writeln( D.dforall(1));
writeln( D.dforall(2));

writeln( D.dfor(1));
writeln( D.dfor(2));

for i in D.dfor(1) {
  for j in D.dfor(2) {
    var t = (i,j);
    writeln(t);
  }
}

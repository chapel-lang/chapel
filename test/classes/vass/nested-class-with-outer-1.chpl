// class nested in a record
record R {
  var x = 5;

  class Q {
    var x = 7;
    proc pq { writeln((x,outer.x)); }
  }

  proc pr {
    var q:owned Q = new owned Q();

    q.pq;
  }
}

var r:R;
r.pr;

// class nested in a class
class C {
  var x = 3;

  class D {
    var x = 4;
    proc pd { writeln((x,outer.x)); }
  }

  proc pc {
    var d:borrowed D = new borrowed D();

    d.pd;
  }
}

var c:unmanaged C = new unmanaged C();

c.pc;

delete c;


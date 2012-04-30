// record nested in a record
record R {
  var x = 5;
  record Q {
    var x = 7;
    proc pq { writeln((x,outer.x)); }
  }
  proc pr {
    var q:Q;
    q.pq;
  }
}

var r:R;
r.pr;

// record nested in a class
class C {
  var x = 3;
  record D {
    var x = 4;
    proc pd { writeln((x,outer.x)); }
  }
  proc pc {
    var d:D;
    d.pd;
  }
}

var c:C = new C();
c.pc;

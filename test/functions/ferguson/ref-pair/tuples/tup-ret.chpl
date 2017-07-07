record R {
  var x:int;
  proc f() : 3*R {
    return (this, this, this);
  }
}

const r = new R(1);

writeln(r);

var tup = r.f();

writeln(tup);


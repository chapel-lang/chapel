class C {
  proc a(param rank): rank * range(int(64), stridable=true) {
    var dummy: rank * range(int(64), stridable=true);
    return dummy;
  }
}

class D: C {
  override proc a(param rank): rank * range(int(64), stridable=true) {
    var dummy: rank * range(int(64), stridable=true);
    return dummy;
  }
}

var c: owned C = new owned D();

writeln(c.a(3));

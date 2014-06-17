class C {
  proc a(param rank): rank * range(int(64), stridable=true) {
    var dummy: rank * range(int(64), stridable=true);
    return dummy;
  }
}

class D: C {
  proc a(param rank): rank * range(int(64), stridable=true) {
    var dummy: rank * range(int(64), stridable=true);
    return dummy;
  }
}

var c: C = new D();
writeln(c.a(3));

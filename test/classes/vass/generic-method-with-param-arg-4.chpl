class C {
  proc a(param rank): rank * range(int(64), boundKind.both, stridable=true) {  // could be generic if we fix implementation
    var dummy: rank * range(int(64), boundKind.both, stridable=true);
    return dummy;
  }
}

class D: C {
  override proc a(param rank): rank * range(int(64), boundKind.both, stridable=true) {  // could be generic if we fix implementation
    var dummy: rank * range(int(64), boundKind.both, stridable=true);
    return dummy;
  }
}

var c: owned C = new owned D();

writeln(c.a(3));

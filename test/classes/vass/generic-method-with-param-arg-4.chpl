class C {
  proc a(param rank): rank * range(int(64), strides=strideKind.any) {
    var dummy: rank * range(int(64), strides=strideKind.any);
    return dummy;
  }
}

class D: C {
  override proc a(param rank): rank * range(int(64), strides=strideKind.any) {
    var dummy: rank * range(int(64), strides=strideKind.any);
    return dummy;
  }
}

var c: owned C = new owned D();

writeln(c.a(3));

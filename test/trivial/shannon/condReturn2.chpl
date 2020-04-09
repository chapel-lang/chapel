class C {
  var rank: int = 2;

  proc foo(dim: rank*int) {
    writeln("In other foo()");
    var x = new unmanaged C();
    return x;
  }

  proc foo(dim: int ...?numdims) {
    if (rank == numdims) {
      return foo(dim);
    } else {
      halt("rank/numdims mismatch");
    }
  }
}

var myC = new unmanaged C(rank=2);


myC.foo(1);

class C {
  var rank: int = 2;

  fun foo(dim: (rank*int)) {
    writeln("In other foo()");
    var x = C();
    return x;
  }

  fun foo(dim: int ...?numdims) {
    if (rank == numdims) {
      return foo(dim);
    } else {
      halt("rank/numdims mismatch");
    }
  }
}

var myC = C(rank=2);


myC.foo(1);

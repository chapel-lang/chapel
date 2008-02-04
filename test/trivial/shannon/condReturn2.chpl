class C {
  var rank: int = 2;

  def foo(dim: rank*int) {
    writeln("In other foo()");
    var x = new C();
    return x;
  }

  def foo(dim: int ...?numdims) {
    if (rank == numdims) {
      return foo(dim);
    } else {
      halt("rank/numdims mismatch");
    }
  }
}

var myC = new C(rank=2);


myC.foo(1);

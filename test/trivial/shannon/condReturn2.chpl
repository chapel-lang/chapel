class C {
  var rank: integer = 2;

  function foo(dim: (rank*integer)) {
    writeln("In other foo()");
    var x = C();
    return x;
  }

  function foo(dim: integer ...?numdims) {
    if (rank == numdims) {
      return foo(dim);
    } else {
      halt("rank/numdims mismatch");
    }
  }
}

var myC = C(rank=2);


myC.foo(1);

class C {
  var x: int;
  proc foo(param rank: int) {
    writeln("foo<", rank, ">: (x = ", x, ")");
  }
}

class D: C {
  var y: int;
  override proc foo(param rank: int) where rank == 1 {
    writeln("foo<", rank, ">: (x = ", x, ", y = ", y, ")");
  }
}

var c = new unmanaged C();
c.foo(1);
c.foo(2);
delete c;

c = new unmanaged D();
c.foo(1);
c.foo(2);
delete c;

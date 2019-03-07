class C {
  var x: int;
  proc foo() {
    writeln("foo: (x = ", x, ")");
  }
}

class D: C {
  param rank: int;
  var y: int;
  override proc foo() where rank == 1 {
    writeln("foo<", rank, ">: (x = ", x, ", y = ", y, ")");
  }
}

var c = new unmanaged C();
c.foo();
c.foo();
delete c;

c = new unmanaged D(rank=1);
c.foo();
c.foo();
delete c;

c = new unmanaged D(rank=2);
c.foo();
c.foo();
delete c;

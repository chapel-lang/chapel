class C {
  var x: int;
  proc foo() {
    writeln("foo: (x = ", x, ")");
  }
}

class D: C {
  param rank: int;
  var y: int;
  proc foo() where rank == 1 {
    writeln("foo<", rank, ">: (x = ", x, ", y = ", y, ")");
  }
}

var c = new C();
c.foo();
c.foo();
delete c;

c = new D(rank=1);
c.foo();
c.foo();
delete c;

c = new D(rank=2);
c.foo();
c.foo();
delete c;

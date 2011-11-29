class C {
  var x: int;
  proc foo(param rank: int) {
    writeln("foo<", rank, ">: (x = ", x, ")");
  }
}

class D: C {
  var y: int;
  proc foo(param rank: int) where rank == 1 {
    writeln("foo<", rank, ">: (x = ", x, ", y = ", y, ")");
  }
}

var c = new C();
c.foo(1);
c.foo(2);
delete c;

c = new D();
c.foo(1);
c.foo(2);
delete c;

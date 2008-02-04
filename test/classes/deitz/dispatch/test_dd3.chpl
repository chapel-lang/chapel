class C {
  var x: int;
  def foo(i: int, j: int) {
    writeln((x, i, j));
  }
}

class D : C {
  var y: int;
  def foo(i: int, j: int) {
    writeln((x, y, i, j));
  }
}

class E : D {
  var z: int;
  def foo(i: int, j: int) {
    writeln((x, y, z, i, j));
  }
}

var c = new C(x=10);
c.foo(1, 2);
c = new D(x=20,y=30);
c.foo(3, 4);
c = new E(x=40,y=50,z=60);
c.foo(5, 6);

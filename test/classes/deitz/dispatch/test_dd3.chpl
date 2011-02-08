class C {
  var x: int;
  proc foo(i: int, j: int) {
    writeln((x, i, j));
  }
}

class D : C {
  var y: int;
  proc foo(i: int, j: int) {
    writeln((x, y, i, j));
  }
}

class E : D {
  var z: int;
  proc foo(i: int, j: int) {
    writeln((x, y, z, i, j));
  }
}

var c = new C(x=10);
c.foo(1, 2);
delete c;
c = new D(x=20,y=30);
c.foo(3, 4);
delete c;
c = new E(x=40,y=50,z=60);
c.foo(5, 6);
delete c;

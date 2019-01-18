class C {
  var x: int;
  proc foo(i: int, j: int) {
    writeln((x, i, j));
  }
}

class D : C {
  var y: int;
  override proc foo(i: int, j: int) {
    writeln((x, y, i, j));
  }
}

class E : D {
  var z: int;
  override proc foo(i: int, j: int) {
    writeln((x, y, z, i, j));
  }
}

var c = new unmanaged C(x=10);
c.foo(1, 2);
delete c;
c = new unmanaged D(x=20,y=30);
c.foo(3, 4);
delete c;
c = new unmanaged E(x=40,y=50,z=60);
c.foo(5, 6);
delete c;

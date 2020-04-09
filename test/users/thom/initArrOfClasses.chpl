class C {
}

class D : C {
  var x: int;

  proc writeThis() throws {
    writeln("x is: ", x);
  }
}

class E : C {
  var y: real;

  proc writeThis() throws {
    writeln("y is: ", y);
  }
}

var arr: [1..3] unmanaged C = (new unmanaged D(1), new unmanaged D(2), new unmanaged E(3));

writeln("arr is: ", arr);

for a in arr do delete a;

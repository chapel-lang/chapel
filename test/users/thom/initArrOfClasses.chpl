class C {
}

class D : C {
  var x: int;

  proc writeThis() {
    writeln("x is: ", x);
  }
}

class E : C {
  var y: real;

  proc writeThis() {
    writeln("y is: ", y);
  }
}

var arr: [1..3] C = (new D(1), new D(2), new E(3));

writeln("arr is: ", arr);

for a in arr do delete a;

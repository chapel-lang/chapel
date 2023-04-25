class C {
}

class D : C {
  var x: int;
}

class E : C {
  var y: real;
}

var arr: [1..] C = (new D(1), new D(2), new E(3));

writeln("arr is: ", arr);

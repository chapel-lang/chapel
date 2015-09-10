class C {
}

class D: C {
  var x: int;
}

class E: C {
  var x: real;
}

var d = new D();
var e = new E();

var A = [d, e];
// WORKAROUND:
// var A = [d:C, e:C];

writeln("A is: ", A);

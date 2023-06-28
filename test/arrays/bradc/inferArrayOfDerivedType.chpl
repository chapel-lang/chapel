class C {
}

class D: C {
  var x: int;
}

class E: C {
  var x: real;
}

var dd = new D(); var d = dd.borrow();
var ee = new E(); var e = ee.borrow();

var A = [d, e];
// WORKAROUND:
// var A = [d:C, e:C];

writeln("A is: ", A);

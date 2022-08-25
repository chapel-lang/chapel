class C {
}

class D: C {
  var x: int;
}

class E: C {
  var x: real;
}

var d = (new owned D()).borrow();
var e = (new owned E()).borrow();

var A = [d, e];
// WORKAROUND:
// var A = [d:C, e:C];

writeln("A is: ", A);

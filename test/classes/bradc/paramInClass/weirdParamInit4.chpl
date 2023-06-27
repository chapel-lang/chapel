class C {
  param x: int;
}

class D {
  var c: borrowed C;
  param y: int = c.x;
}

var ownC = new owned C(2);
var c = ownC.borrow();

var ownD = new owned D(c);
var d = ownD.borrow();

writeln("c is: ", c);
writeln("d is: ", d);

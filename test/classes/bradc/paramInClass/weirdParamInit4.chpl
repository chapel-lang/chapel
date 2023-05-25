class C {
  param x: int;
}

class D {
  var c: borrowed C;
  param y: int = c.x;
}

var c = (new owned C(2)).borrow();

var d = (new owned D(c)).borrow();

writeln("c is: ", c);
writeln("d is: ", d);

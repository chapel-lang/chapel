class C {
  param x: int;
}

class D {
  var c: borrowed C = (new owned C(2)).borrow();
  param y: int = c.x;
}

var c = new C(2);

var d = new D(c);

writeln("c is: ", c);
writeln("d is: ", d);

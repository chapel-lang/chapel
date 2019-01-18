class C {
  param x: int;
}

class D {
  var c: borrowed C;
  param y: int = c.x;
}

var c = new borrowed C(2);

var d = new borrowed D(c);

writeln("c is: ", c);
writeln("d is: ", d);

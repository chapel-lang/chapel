class C {
  param x: int;
}

class D {
  var c: C = new C();
  param y: int = c.x;
}

var c = new C(2);

var d = new D(c);

writeln("c is: ", c);
writeln("d is: ", d);

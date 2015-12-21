class C {
  param x: int;
}

class D {
  param y: int;
}

var c = new C(2);

var d = new D(c.x);

writeln("c is: {x = ", c.x, "}");
writeln("d is: {x = ", d.x, "}");

delete c;
delete d;

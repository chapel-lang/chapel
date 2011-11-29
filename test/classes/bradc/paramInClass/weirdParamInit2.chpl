class C {
  param x: int;
}

class D {
  param y: int;
}

var c = new C(2);

var d = new D(c.x);

writeln("c is: ", c);
writeln("d is: ", d);

delete c;
delete d;

class C {
  param x: int;
}

class D {
  param y: int;
}

var c = C(2);

var d = D(c.x);

writeln("c is: ", c);
writeln("d is: ", d);

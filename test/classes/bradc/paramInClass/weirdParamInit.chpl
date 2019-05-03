class C {
  param x: int;
}

class D {
  param y: int;
}

var c = new C(2);

param x = c.x;

writeln("c is: {x = ", c.x, "}");
writeln("x is: ", x);

class C {
  param x: int;
}

class D {
  param y: int;
}

var c = C(2);

param x = c.x;

writeln("c is: ", c);
writeln("x is: ", x);

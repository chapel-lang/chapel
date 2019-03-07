

class C {
  const x: int;

  proc init(y, z) {
    x = y + z;
    x = 1;

  }
}

var c = new owned C(2, 3);

writeln(c.x);

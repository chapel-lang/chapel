class C {
  const x: int;

  def C(y, z) {
    x = y+z;
    x -= 1;
  }
}

var c = C(2, 3);

writeln(c.x);

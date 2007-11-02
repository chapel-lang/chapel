class C {
  var x: int;

  def C(y, z) {
    x = y+z;
  }
}

var c = C(2, 3);

writeln(c.x);

class C {
  var x: int = y;
  var y: int = 1;
}

var c: borrowed C = new borrowed C();

writeln(c.x, " ", c.y);

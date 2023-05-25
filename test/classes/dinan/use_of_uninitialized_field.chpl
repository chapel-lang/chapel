class C {
  var x: int = y;
  var y: int = 1;
}

var c: borrowed C = (new owned C()).borrow();

writeln(c.x, " ", c.y);

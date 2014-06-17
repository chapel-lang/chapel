var l: int = 0;
var r: int = 1;

proc left() var {
  writeln("left");
  return l;
}

proc right() var {
  writeln("right");
  return r;
}

left() = right();

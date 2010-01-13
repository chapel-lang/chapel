var l: int = 0;
var r: int = 1;

def left() var {
  writeln("left");
  return l;
}

def right() var {
  writeln("right");
  return r;
}

left() = right();

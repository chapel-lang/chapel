var l: int = 0;
var r: int = 1;

proc left() ref {
  writeln("left");
  return l;
}

proc right() ref {
  writeln("right");
  return r;
}

left() = right();

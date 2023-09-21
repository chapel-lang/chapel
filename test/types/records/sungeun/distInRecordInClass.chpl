use BlockDist;
record myR {
  var d = new blockDist(boundingBox={1..2});
}

class myC {
  var r = new myR();
}

proc main {
  var c = new myC();
  writeln(c);
  c.r.d = new blockDist(boundingBox={2..3});
  writeln(c);
}

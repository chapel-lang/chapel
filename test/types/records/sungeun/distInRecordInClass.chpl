use BlockDist;
record myR {
  var d = new dmap(new Block(boundingBox={1..2}));
}

class myC {
  var r = new myR();
}

proc main {
  var c = new myC();
  writeln(c);
  c.r.d = new dmap(new Block(boundingBox={2..3}));
  writeln(c);
}

use BlockDist;
record myR {
  var d = new dmap(new Block(boundingBox={1..2}));
}

proc main() {
  var r: myR;
  writeln(r);
  r.d = new dmap(new Block(boundingBox={2..3}));
  writeln(r);
}


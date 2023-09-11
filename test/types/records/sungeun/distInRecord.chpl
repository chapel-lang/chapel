use BlockDist;
record myR {
  var d = new blockDist(boundingBox={1..2});
}

proc main() {
  var r: myR;
  writeln(r);
  r.d = new blockDist(boundingBox={2..3});
  writeln(r);
}


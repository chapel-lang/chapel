
record Numbers {
  var totVertices : int;
  var maxParallelEdge :int;
  var numIntEdges : int;
  var numStrEdges : int;
  var maxIntWeight : int;
}

proc f(x) {
  return x + 1;
}

class Edges {
  with Numbers;
  var y = 5;
  var z = f(y);
}

var x = new Edges();

writeln("expecting 6 :", x.z);

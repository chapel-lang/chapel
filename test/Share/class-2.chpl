
record Numbers {
  var totVertices : int;
  var maxParallelEdge :int;
  var numIntEdges : int;
  var numStrEdges : int;
  var maxIntWeight : int;
}

fun f(x) {
  return x + 1;
}

class Edges {
  with Numbers;
  var y = 5;
  var z = f(y);
}

var x = Edges();

writeln("expecting 6 :", x.z);


record Numbers {
  var totVertices : integer;
  var maxParallelEdge :integer;
  var numIntEdges : integer;
  var numStrEdges : integer;
  var maxIntWeight : integer;
}

function f(x) {
  return x + 1;
}

class Edges {
  with Numbers;
  var y = 5;
  var z = f(y);
}

var x = Edges();

writeln("expecting 6 :", x.z);


record Numbers {
  var totVertices : int;
  var maxParallelEdge :int;
  var numIntEdges : int;
  var numStrEdges : int;
  var maxIntWeight : int;
}

class Edges {
  with Numbers;
}

var x = Edges();

x.totVertices = 5;

writeln(x.totVertices);

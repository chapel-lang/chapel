
record Numbers {
  var totVertices : integer;
  var maxParallelEdge :integer;
  var numIntEdges : integer;
  var numStrEdges : integer;
  var maxIntWeight : integer;
}

class Edges {
  with Numbers;
}

var x = Edges();

x.totVertices = 5;

writeln(x.totVertices);

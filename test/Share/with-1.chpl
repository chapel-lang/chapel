record Numbers {
  var totVertices : integer;
  var maxParallelEdge :integer;
  var numIntEdges : integer;
  var numStrEdges : integer;
  var maxIntWeight : integer;
}

var x : Numbers;

with x;

totVertices = 5;

writeln("expect 5 ", x.totVertices);

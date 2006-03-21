record Numbers {
  var totVertices : int;
  var maxParallelEdge :int;
  var numIntEdges : int;
  var numStrEdges : int;
  var maxIntWeight : int;
}

var x : Numbers;

with x;

totVertices = 5;

writeln("expect 5 ", x.totVertices);

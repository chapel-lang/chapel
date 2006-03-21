
record EndPoints {
  var start : int;
  var end : int;
}

record Numbers {
  var totVertices : int;
  var maxParallelEdge :int;
  var numIntEdges : int;
  var numStrEdges : int;
  var maxIntWeight : int;
}

class Subgraph {
  type wtype;
  var VertexD : domain(1);
  var ParEdgeD : domain(1);
  var AdjD : domain sparse (VertexD * VertexD) = nil;
  var weights [AdjD] : seq of wtype;
  constructor EndPoints((s,e) : AdjD) {
    start = s;
    end = e;
  }
  function adjMatrix [i:AdjD] { return weights(i).length; }
}

class Graph {
  with Numbers;
  var VertexD : domain(1);
  var ParEdgeD : domain(1);
  var intg = Subgraph(wtype = int, VertexD => VertexD, ParEdgeD => ParEdgeD);
  var strg = Subgraph(wtype = string, VertexD => VertexD, ParEdgeD => ParEdgeD);
  function copy(s : Graph) {
    return Graph(VertexD = s.VertexD, ParEdgeD = s.ParEdgeD);
  }
}

var x = Graph();

writeln("expecting 6 :", x.z);

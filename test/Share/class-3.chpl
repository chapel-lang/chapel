
record EndPoints {
  var start : integer;
  var end : integer;
}

record Numbers {
  var totVertices : integer;
  var maxParallelEdge :integer;
  var numIntEdges : integer;
  var numStrEdges : integer;
  var maxIntWeight : integer;
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
  var intg = Subgraph(wtype = integer, VertexD => VertexD, ParEdgeD => ParEdgeD);
  var strg = Subgraph(wtype = string, VertexD => VertexD, ParEdgeD => ParEdgeD);
  function copy(s : Graph) {
    return Graph(VertexD = s.VertexD, ParEdgeD = s.ParEdgeD);
  }
}

var x = Graph();

writeln("expecting 6 :", x.z);

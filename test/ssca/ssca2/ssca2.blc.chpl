-- heuristic value taken from the Matlab implementation to set default
-- values if the user doesn't override any of them
config const scale = 9;


-- input values to describe the graph.  Override these on the
-- executable's command-line using the -s flag
config const totVertices: int = 2**scale;
config const maxCliqueSize : int = 10;
config const probUnidirectional: real = 0.3;
config const maxParallelEdges: int = 3;
config const probIntercliqueEdges: real = 0.6;
config const percentIntWeights: real = 0.6;
config const maxIntWeight: int = 8**scale;
config const maxStrLen: int = ceil(scale/6.0);

config const cliqueGeneratorsPerLocale: int = 1;
config const numCliqueGenerators: int = numLocales * cliqueGeneratorsPerLocale;

var Vertices: domain(1) = 1..totVertices;
type vertex: index(Vertices);

--var EdgeSpace: domain(Vertices*Vertices);
--var EdgeSpace: domain(2) = {1..totVertices, 1..totVertices};
var EdgeSpace: domain(2) = (1..totVertices, 1..totVertices);
--var AdjSpace: sparse domain(EdgeSpace) = nil;
var AdjSpace: domain(EdgeSpace) = EdgeSpace;
var edge: index(Edges);

record multiedge {
  var intWeights: list(int);
  var strWeights: list(string);
};

var Edges: [AdjSpace] multiedge;

union weightType {
  var intWeight: int;
  var stringWeight: string;
}

record edgeTuple {
  var source: vertex;
  var sink: vertex;
  var weight: weightType;
}



-- random number stuff: currently unimplemented, and doing so in
-- parallel will require some care

-- return a random real in [0.0, 1.0)
proc randomFloat() {
  halt("Need to implement randomFloat()");
  return 0.0;
}

-- flip a coin, returning heads (true) with probability headsProb
proc coinFlip(headsProb: real = 0.5) {
  return (randomFloat() < headsProb);
}

-- return a random int from 1 to maxval, inclusive
proc randomInt(maxint: int) {
  halt("Need to implement randomInt()");
  return 0;
}

-- return a random string of length strlen
proc randomString(strlen: int) {
  halt("Need to implement randomString()");
  return "<empty>";
}


proc generateDirectedEdges(v1, v2) {
  var edges: list(edgeTuple);
  var numParallelEdges: int = randomInt(maxParallelEdges);
  forall parEdge in 1..numParallelEdges { // TODO: could drop "parEdge"
    var weight: weightType;
    if (coinFlip(percentIntWeights)) {
      weight.intWeight = randomInt(maxIntWeight);
    } else {
      weight.stringWeight = randomString(maxStrLen);
    }
    edges #= new edgeTuple(source = v1, sink = v2, weight = weight);
  }
  return edges;
}


proc generateEdges(v1, v2) {
  var edges: list(edgeTuple);
  var forward = true,   // generate forward edges?
      backward = true;  // generate backward edges?

  if (coinFlip(probUnidirectional)) {
    if (coinFlip()) {
      backward = false;
    } else {
      forward = false;
    }
  }
  if (forward) {
    edges #= generateDirectedEdges(v1, v2);
  }
  if (backward) {
    edges #= generateDirectedEdges(v2, v1);
  }
}


proc generateEdgeList() {
  var edgeTuples: list(edgeTuple);
  var numVerticesTaken = 0;
  var clique: [Vertices] int;
  var numCliques = 0;

  forall g in 1..numCliqueGenerators { // TODO: could drop "g"
    while (numVerticesTaken != totVertices) {
      var cliqueSize: int;
      var vertexOffset: int;
      var myCliqueNum: int;
      atomic {
        vertexOffset = numVerticesTaken;
        var numVerticesRemaining = totVertices - numVerticesTaken;
        cliqueSize = min(randomInt(maxCliqueSize), numVerticesRemaining);
        numVerticesTaken += cliqueSize;
        numCliques += 1;
        myCliqueNum = numCliques;
      }
      // generate intraclique edges
      forall v1 in 1..cliqueSize {
        clique(v1) = myCliqueNum;
        forall v2 in v1+1..cliqueSize {
          edgeTuples #= generateEdges(v1 + vertexOffset, v2 + vertexOffset);
        }
      }
    }
  }

  // generate interclique edges
  // the following uses the basic exponential distribution from
  // the pseudocode in the written spec rather than the
  // distribution of the executable specification
  forall v1 in Vertices {
    var d = 2;
    var p = probIntercliqueEdges;
    while (d < totVertices/2) {
      var v2: vertex = (v1+d) % totVertices;
      var r = randomFloat();
      if (clique(v1) != clique(v2) && r <= p) {
        edgeTuples #= generateEdges(v1, v2);
      }

      d *= 2;
      p /= 2;
    }
  }

  // TODO: at this point, I think we're required to randomly shuffle
  // the vertex numbers and edge tuples

  return edgeTuples;
}


proc kernel1(edgeTuples: list(edgeTuple)) {
  forall t in edgeTuples {
    var newEdge: edge = (t.source, t.sink);
    AdjSpace += newEdge;
    typeselect t.weight {
      when i:int do
        Edges(newEdge).intWeights #= i;
      when s:string do
        Edges(newEdge).strWeights #= s;
    }
  }
}


proc kernel2(soughtString: string) {
  var maxIntWeight = sum reduce Edges.intWeights;
  var maxEdges: sparse domain(EdgeSpace) = nil;

  forall e in Edges {
    cobegin {
      forall i in e.intWeights {
        if (i == maxIntWeight) {
          maxEdges += e;
        }
      }
      forall s in e.strWeights {
        if (s == soughtString) {
          maxEdges += e;
        }
      }
    }
  }

  return maxEdges;
}


proc kernel3(startEdges, subGraphEdgeLength: int) {
  forall startEdge in startEdges {
    results #= findSubgraph(startEdge);
  }

  proc findSubgraph(startEdge) {
    var frontier: sparse domain(Vertices);
    frontier += sv1;
    frontier += sv2;
    var subgraph = SubGraph(V = frontier, E = startEdge);
    for dist in 1..startEdges {
      var newEdges: list(edge);
      forall v1 in frontier {
        forall v2 in AdjDom(v1, *) {
          newEdges #= (v1, v2);
        }
      }
--      subgraph.E += newEdges;
      forall e in newEdges {
        subgraph.E += e;
      }
--      frontier += [(v1, v2) in newEdges] v2;
      forall (v1, v2) in newEdges {
        frontier += v2;
      }
--      frontier -= subgraph.V;
      forall v in subgraph.V {
        frontier -= v;
      }
      subgraph.V += frontier;
      forall v in frontier {
        subgraph.V += v;
      }
      return subgraph;
    }
  }
}


proc kernel4(maxClusterSize: int, k4alpha) {
  halt("kernel4 not implemented yet");
}


proc writeConfigs() {
  writeln("scale = ", scale);
  writeln("totVertices = ", totVertices);
  writeln("maxCliqueSize = ", maxCliqueSize);
  writeln("probUnidirectional = ", probUnidirectional);
  writeln("maxParallelEdges = ", maxParallelEdges);
  writeln("probIntercliqueEdges = ", probIntercliqueEdges);
  writeln("percentIntWeights = ", percentIntWeights);
  writeln("maxIntWeight = ", maxIntWeight);
  writeln("maxStrLen = ", maxStrLen);
}

proc main() {
  writeConfigs();
  generateEdgeList();
}

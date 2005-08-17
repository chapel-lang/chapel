-- heuristic value taken from the Matlab implementation to set default
-- values if the user doesn't override any of them
config const scale = 9;


-- input values to describe the graph.  Override these on the
-- executable's command-line using the -s flag
config const totVertices: integer = 2**scale;
config const maxCliqueSize : integer = 10;
config const probUnidirectional: float = 0.3;
config const maxParallelEdges: integer = 3;
config const probIntercliqueEdges: float = 0.6;
config const percentIntegerWeights: float = 0.6;
config const maxIntWeight: integer = 8**scale;
config const maxStrLen: integer = ceil(scale/6.0);

config const cliqueGeneratorsPerLocale: integer = 1;
config const numCliqueGenerators: integer = numLocales * cliqueGeneratorsPerLocale;

var Vertices: domain(1) = 1..totVertices;
type vertex: index(Vertices);

--var EdgeSpace: domain(Vertices*Vertices);
--var EdgeSpace: domain(2) = [1..totVertices, 1..totVertices];
var EdgeSpace: domain(2) = (1..totVertices, 1..totVertices);
--var AdjSpace: sparse domain(EdgeSpace) = nil;
var AdjSpace: domain(EdgeSpace) = EdgeSpace;
var edge: index(Edges);

record multiedge {
  var intWeights: seq(integer);
  var strWeights: seq(string);
};

var Edges: [AdjSpace] multiedge;

union weightType {
  var intWeight: integer;
  var stringWeight: string;
}

record edgeTuple {
  var source: vertex;
  var sink: vertex;
  var weight: weightType;
}



-- random number stuff: currently unimplemented, and doing so in
-- parallel will require some care

-- return a random float in [0.0, 1.0)
function randomFloat() {
  halt("Need to implement randomFloat()");
  return 0.0;
}

-- flip a coin, returning heads (true) with probability headsProb
function coinFlip(headsProb: float = 0.5) {
  return (randomFloat() < headsProb);
}

-- return a random integer from 1 to maxval, inclusive
function randomInt(maxint: integer) {
  halt("Need to implement randomInt()");
  return 0;
}

-- return a random string of length strlen
function randomString(strlen: integer) {
  halt("Need to implement randomString()");
  return "<empty>";
}


function generateDirectedEdges(v1, v2) {
  var edges: seq(edgeTuple);
  var numParallelEdges: integer = randomInt(maxParallelEdges);
  forall parEdge in 1..numParallelEdges { -- TODO: could drop "parEdge"
    var weight: weightType;
    if (coinFlip(percentIntegerWeights)) {
      weight.intWeight = randomInt(maxIntWeight);
    } else {
      weight.stringWeight = randomString(maxStrLen);
    }
    edges #= edgeTuple(source = v1, sink = v2, weight = weight);
  }
  return edges;
}


function generateEdges(v1, v2) {
  var edges: seq(edgeTuple);
  var forward = true,   -- generate forward edges?
      backward = true;  -- generate backward edges?

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


function generateEdgeList() {
  var edgeTuples: seq(edgeTuple);
  var numVerticesTaken = 0;
  var clique: [Vertices] integer;
  var numCliques = 0;

  forall g in 1..numCliqueGenerators { -- TODO: could drop "g"
    while (numVerticesTaken != totVertices) {
      var cliqueSize: integer;
      var vertexOffset: integer;
      var myCliqueNum: integer;
      atomic {
        vertexOffset = numVerticesTaken;
        var numVerticesRemaining = totVertices - numVerticesTaken;
        cliqueSize = min(randomInt(maxCliqueSize), numVerticesRemaining);
        numVerticesTaken += cliqueSize;
        numCliques += 1;
        myCliqueNum = numCliques;
      }
      -- generate intraclique edges
      forall v1 in 1..cliqueSize {
        clique(v1) = myCliqueNum;
        forall v2 in v1+1..cliqueSize {
          edgeTuples #= generateEdges(v1 + vertexOffset, v2 + vertexOffset);
        }
      }
    }
  }

  -- generate interclique edges
  -- the following uses the basic exponential distribution from
  -- the pseudocode in the written spec rather than the
  -- distribution of the executable specification
  forall v1 in Vertices {
    var d = 2;
    var p = probIntercliqueEdges;
    while (d < totVertices/2) {
      var v2: vertex = (v1+d) mod totVertices;
      var r = randomFloat();
      if (clique(v1) != clique(v2) and r <= p) {
        edgeTuples #= generateEdges(v1, v2);
      }

      d *= 2;
      p /= 2;
    }
  }

  -- TODO: at this point, I think we're required to randomly shuffle
  -- the vertex numbers and edge tuples

  return edgeTuples;
}


function kernel1(edgeTuples: seq(edgeTuple)) {
  forall t in edgeTuples {
    var newEdge: edge = (t.source, t.sink);
    AdjSpace += newEdge;
    typeselect t.weight {
      when i:integer do
        Edges(newEdge).intWeights #= i;
      when s:string do
        Edges(newEdge).strWeights #= s;
    }
  }
}


function kernel2(soughtString: string) {
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


function kernel3(startEdges, subGraphEdgeLength: integer) {
  forall startEdge in startEdges {
    results #= findSubgraph(startEdge);
  }

  function findSubgraph(startEdge) {
    var frontier: sparse domain(Vertices);
    frontier += sv1;
    frontier += sv2;
    var subgraph = SubGraph(V = frontier, E = startEdge);
    for dist in 1..startEdges {
      var newEdges: seq(edge);
      forall v1 in frontier {
        forall v2 in AdjDom(v1, *) {
          newEdges #= (v1, v2);
        }
      }
--      subgraph.E += newEdges;
      forall e in newEdges {
        subgraph.E += newEdges;
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


function kernel4(maxClusterSize: integer, k4alpha) {
  halt("kernel4 not implemented yet");
}


function writeConfigs() {
  writeln("scale = ", scale);
  writeln("totVertices = ", totVertices);
  writeln("maxCliqueSize = ", maxCliqueSize);
  writeln("probUnidirectional = ", probUnidirectional);
  writeln("maxParallelEdges = ", maxParallelEdges);
  writeln("probIntercliqueEdges = ", probIntercliqueEdges);
  writeln("percentIntegerWeights = ", percentIntegerWeights);
  writeln("maxIntWeight = ", maxIntWeight);
  writeln("maxStrLen = ", maxStrLen);
}

function main() {
  writeConfigs();
  generateEdgeList();
}

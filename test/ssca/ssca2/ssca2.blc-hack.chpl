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


union weightType {
  var intWeight: int;
  var stringWeight: string;
}

record edgeTuple {
  var start: int;
  var stop: int;
  var weight: weightType;
}


proc randomInt(maxint: int) {
  halt("Need to implement randomInt()");
  return 0;
}

proc randomString(strlen: int) {
  halt("Need to implement randomString()");
  return "<empty>";
}

proc randomFloat(maxval: real = 1.0) {
  halt("Need to implement randomFloat()");
  return 0.0;
}


proc generateDirectedEdges(start, stop) {
  var edges: list(edgeTuple);
  var numParallelEdges: int = randomInt(maxParallelEdges);
  forall parEdge in 1..numParallelEdges { // could drop "parEdge"
    var weight: weightType;
    if (randomFloat() < percentIntWeights) {
      weight.intWeight = randomInt(maxIntWeight);
    } else {
      weight.stringWeight = randomString(maxStrLen);
    }
--    edges #= new edgeTuple(start, stop, weight);
-- rewritten:
--    edges = edges # _seq( edgeTuple(start, stop, weight) );
-- rewritten:
    var newEdge: edgeTuple = new edgeTuple();
    newEdge.start = start;
    newEdge.stop = stop;
    newEdge.weight = weight;
--    edges = edges # _seq( newEdge );
-- rewritten:
--    edges = edges # newEdge;
-- rewritten:
--    edges._concat(newEdge);
  }
  return edges;
}


proc generateEdges(start, stop) {
  var edges: list(edgeTuple);
  var forward = true,   // generate forward edges?
      backward = true;  // generate backward edges?

  if (randomFloat() < probUnidirectional) {
    if (randomFloat() < 0.5) {
      backward = false;
    } else {
      forward = false;
    }
  }
  if (forward) {
--    edges #= generateDirectedEdges(start, stop);
--    edges = edges # generateDirectedEdges(start, stop);
  }
  if (backward) {
--    edges #= generateDirectedEdges(stop, start);
--    edges = edges # generateDirectedEdges(stop, start);
  }
}


proc generateEdgeList() {
  var edgeTuples: list(edgeTuple);
  var numVerticesRemaining = totVertices;

  forall g in 1..numCliqueGenerators { // could drop "g"
    while (numVerticesRemaining != totVertices) {
      var cliqueSize: int;
      atomic {
        cliqueSize = min(randomInt(maxCliqueSize), numVerticesRemaining);
        numVerticesRemaining -= cliqueSize;
      }
      forall start in 1..cliqueSize {
        forall stop in start+1..cliqueSize {
--          edgeTuples #= generateEdges(start, stop);
--          edgeTuples = new edgeTuples # generateEdges(start, stop);
        }
      }
    }
  }
}


proc kernel2(soughtString: string) {
}


proc kernel3(subGraphEdgeLength: int) {
}


proc kernel4(maxClusterSize: int, k4alpha) {
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

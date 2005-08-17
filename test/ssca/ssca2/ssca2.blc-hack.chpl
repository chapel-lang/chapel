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


union weightType {
  var intWeight: integer;
  var stringWeight: string;
}

record edgeTuple {
  var start: integer;
  var stop: integer;
  var weight: weightType;
}


function randomInt(maxint: integer) {
  halt("Need to implement randomInt()");
  return 0;
}

function randomString(strlen: integer) {
  halt("Need to implement randomString()");
  return "<empty>";
}

function randomFloat(maxval: float = 1.0) {
  halt("Need to implement randomFloat()");
  return 0.0;
}


function generateDirectedEdges(start, stop) {
  var edges: seq(edgeTuple);
  var numParallelEdges: integer = randomInt(maxParallelEdges);
  forall parEdge in 1..numParallelEdges { -- could drop "parEdge"
    var weight: weightType;
    if (randomFloat() < percentIntegerWeights) {
      weight.intWeight = randomInt(maxIntWeight);
    } else {
      weight.stringWeight = randomString(maxStrLen);
    }
--    edges #= edgeTuple(start, stop, weight);
-- rewritten:
--    edges = edges # (/ edgeTuple(start, stop, weight) /);
-- rewritten:
    var newEdge: edgeTuple = edgeTuple();
    newEdge.start = start;
    newEdge.stop = stop;
    newEdge.weight = weight;
--    edges = edges # (/ newEdge /);
-- rewritten:
--    edges = edges # newEdge;
-- rewritten:
--    edges._concat(newEdge);
  }
  return edges;
}


function generateEdges(start, stop) {
  var edges: seq(edgeTuple);
  var forward = true,   -- generate forward edges?
      backward = true;  -- generate backward edges?

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


function generateEdgeList() {
  var edgeTuples: seq(edgeTuple);
  var numVerticesRemaining = totVertices;

  forall g in 1..numCliqueGenerators { -- could drop "g"
    while (numVerticesRemaining != totVertices) {
      var cliqueSize: integer;
      atomic {
        cliqueSize = min(randomInt(maxCliqueSize), numVerticesRemaining);
        numVerticesRemaining -= cliqueSize;
      }
      forall start in 1..cliqueSize {
        forall stop in start+1..cliqueSize {
--          edgeTuples #= generateEdges(start, stop);
--          edgeTuples = edgeTuples # generateEdges(start, stop);
        }
      }
    }
  }
}


function kernel2(soughtString: string) {
}


function kernel3(subGraphEdgeLength: integer) {
}


function kernel4(maxClusterSize: integer, k4alpha) {
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

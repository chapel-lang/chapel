use IO;

config const printTime = false;

record Node {
  var x,y: int(32);
}

record Edge {
  var x,y: int(32);
}

enum Color {
  white, gray, black
}

proc bfs(nodes: [] Node, edges: [] Edge, colors: [] Color, costs: [] int(32), source: int(32)) {
  use Deque;
  var wavefront = new deque(int(32));
  wavefront.pushBack(source);
  colors[source] = Color.gray;
  while !wavefront.empty() {
    var idx = wavefront.popFront();
    for i in nodes[idx].x..nodes[idx].x+nodes[idx].y-1 {
      var id = edges[i].x;
      if colors[id] == Color.white {
        costs[id] = costs[idx] + 1;
        wavefront.pushBack(id);
        colors[id] = Color.gray;
      }
    }
    colors[idx] = Color.black;
  }
}

config const infileName = "graph_input.dat";
config const outfileName = "output.dat";

param INF = 2**31 - 1;

proc main {
  use Time;
  var numNodes: int(32);
  var f = open(infileName, iomode.r);
  var r = f.reader();
  var inputTimer, outputTimer, computeTimer, totalTimer: Timer;

  totalTimer.start();
  inputTimer.start();
  r.read(numNodes);
  var graphNodes: [0..#numNodes] Node;
  var color: [0..#numNodes] Color;
  writeln("Initializing nodes and colors: ", numNodes);
  for i in 0..#numNodes {
    var start, edgeNum: int(32);
    r.read(start, edgeNum);
    graphNodes[i].x = start;
    graphNodes[i].y = edgeNum;
    color[i] = Color.white;
  }

  var source, edgeListSize: int(32);

  r.read(source, edgeListSize);
  var graphEdges: [0..#edgeListSize] Edge;
  writeln("Initializing edges: ", edgeListSize);
  for i in 0..#edgeListSize {
    var id, cost: int(32);
    r.read(id, cost);
    graphEdges[i].x = id;
    graphEdges[i].y = cost;
  }
  r.close();
  f.close();
  inputTimer.stop();

  computeTimer.start();
  var cost:[0..#numNodes] int(32);
  cost = INF;
  cost[source] = 0;
  writeln("Starting bfs");
  bfs(graphNodes, graphEdges, color, cost, source);
  computeTimer.stop();

  outputTimer.start();
  var outf = open(outfileName, iomode.cw);
  var w = outf.writer();
  w.writeln(numNodes);
  writeln("writing nodes");
  for i in 0..#numNodes {
    w.writeln(i, " ", cost[i]);
  }

  w.close();
  outf.close();
  outputTimer.stop();
  totalTimer.stop();
  if printTime {
    writeln("Input:   ", inputTimer.elapsed());
    writeln("Compute: ", computeTimer.elapsed());
    writeln("Output:  ", outputTimer.elapsed());
    writeln("Total:   ", totalTimer.elapsed());
  }
}

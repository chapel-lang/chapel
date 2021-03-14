module Nodes {

  use NodesEdges;

  /*If isDirected = 1, then it is a directed graph, else it is not */
  proc InitializeNodes(nEdges: int, nNodes: int, Edges, isDirected: int) {

    // Count number of neighbors
    var D1 = {0..(nNodes-1)};

    var counts: [D1] int;

    // Compute Degree (Outdegree for directed)
    if (isDirected == 1) {
      // Directed graph: an edge is stored only once
      for i in 0..(nEdges-1) do counts[Edges[i]!.n1] += 1;
    } else {
      // Undirected graph: every edge stored twice
      for i in 0..(nEdges-1) {
        counts[Edges[i]!.n1] += 1;
        counts[Edges[i]!.n2] += 1;
      }
    }

    var Nodes: [D1] unmanaged Node = for i in D1 do createNode(i);

    proc createNode(i) {
      const node = new unmanaged Node(id = i, NeighborD = {0..(counts[i] - 1)}, EdgeIndexD = {0..(counts[i] - 1)}, nodeType = 2);
      node.vb$.writeEF(0.0);
      return node;
    }

    for i in 0..(nEdges-1) {
      if (Edges[i]!.dupl != 0) {  // if i is duplicate, continue

        var u: int = Edges[i]!.n1;
        var v: int = Edges[i]!.n2;

        var k1: int = Nodes[u].nNeighbors;
        Nodes[u].nNeighbors += 1;
        Nodes[u].Neighbors[k1] = v;
        Nodes[u].EdgeIndex[k1] = i;

        // Undirected graph: every edge stored twice
        if (isDirected != 1) {
          var k2:int = Nodes[v].nNeighbors;
          Nodes[v].nNeighbors += 1;
          Nodes[v].Neighbors[k2] = u;
          Nodes[v].EdgeIndex[k2] = i;
        }
      }
    }

    return Nodes;
  }

  /// Display the graph:
  proc displayGraph(nodes, nNodes: int) {
    writeln("*************************");
    writeln("Graph in Metis format: ");
    writeln("*************************");
    for i in 0..(nNodes-1) {
      if (nodes[i].nNeighbors > 0) {
        writeln("ID= ", nodes[i].id+1, " -  degree= ", nodes[i].nNeighbors);
        writeln(nodes[i].Neighbors+1);
      }
    }
    writeln("*************************");
  }

  /// Display sources and sinks
  proc displaySourceSink(Sources, nSource: int, Sinks, nSink: int) {
    writeln("Sources (", nSource, "): ");
    for i in 0..(nSource-1) do writeln(Sources[i]+1, ", ");
    writeln();
    writeln("*************************");
    writeln("Sinks (", nSink, "): ");
    for i in 0..(nSink-1) do writeln(Sinks[i]+1, ", ");
    writeln();
    writeln("*************************");
  }

}

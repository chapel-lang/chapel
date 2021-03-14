module Edges {

  use NodesEdges;

  proc SortEdges(nEdges: int, nNodes: int, Edges) {
    var D1 = {0..nNodes};
    var D2 = {0..(nEdges-1)};

    var Counts: [D1] int;
    var OutEdges: [D2] unmanaged Edge?;

    Counts[nNodes] = nEdges;
    for i in D2 do Counts[Edges[i]!.n1] += 1;
    for i in 1..(nNodes-1) do Counts[i] += Counts[i-1];

    for i in D2 {
      var ndx = Edges[i]!.n1;
      var tmp = Counts[ndx];
      Counts[ndx] -= 1;
      ndx = tmp;
      OutEdges[ndx-1] = Edges[i];
    }

    for i in 0..(nNodes-1) {
      for end in Counts[i+1]..Counts[i] by -1 {
        for start in Counts[i]..(end-1) {
          if (OutEdges[start]!.n2 > OutEdges[start+1]!.n2) {
            OutEdges[start] <=> OutEdges[start+1];
          }
        }
      }
    }

    return OutEdges;
  }

  proc InitializeEdges(nEdges: int, out nNodes: int, U, V, W) {
    var nodes: int;

    var D1 = {0..(nEdges-1)};
    var Edges: [D1] unmanaged Edge?;

    for i in D1 {
      var u: int = U[i];
      var v: int = V[i];
      var w: real = W[i];
      if (w < 0) {
        var temp: int = u;
        u = v;
        v = temp;
      }

      Edges[i] = new unmanaged Edge(id = i, dupl = 1, n1 = u - 1, n2 = v - 1, distance = if (w > 0.0) then w else -w);
      Edges[i]!.vb$.writeEF(0.0);

      var lm: int;
      lm = if (u > v) then u else v;
      if (lm > nodes) then nodes = lm;
    }

    writeln("Number of nodes, as counted from number of edges: ", nodes);

    Edges = SortEdges(nEdges, nodes, Edges);

    // mark duplicate edges
    for i in 1..(nEdges-1) {
      if (Edges[i]!.n1 == Edges[i-1]!.n1 && 
          Edges[i]!.n2 == Edges[i-1]!.n2) then
        Edges[i]!.dupl = 0;
    }

    for i in D1 {
      if (Edges[i]!.dupl != 0) {  // not the first of duplicates
        var sum: real = 1.0/Edges[i]!.distance;
        for j in (i+1)..(nEdges-1) {
          if (Edges[j]!.dupl == 1) then break; else sum += 1/Edges[j]!.distance;
        }
        Edges[i]!.distance = 1.0/sum;
      }
    }

    nNodes = nodes;
    return Edges;
  }

  proc displayGroups(Groups, nGroups: int) {
    writeln("Group Information:");
    for g in 0..(nGroups-1) {
      writeln("*ID: ", Groups[g].id+1, ", Size: ", Groups[g].size, ", GBC: ", Groups[g].vb);
      writeln("Edges: ");
      for i in 0..(Groups[g].size-1) {
        writeln("(", Groups[g].edgeList[i].n1+1, ",", Groups[g].edgeList[i].n2+1, ")");
      }
    }
  }

  proc displayGroupBC(Groups, nGroups: int) {
    writeln("Group Betweenness Centrality Scores:");
    for g in 0..(nGroups-1) {
      writeln("*ID: ", Groups[g].id+1, ", Size: ", Groups[g].size, ", GBC: ", Groups[g].vb);
    }
  }

  proc displayNonBoundaryEdges(nEdges: int, Edges, Nodes) {
    var numNewEdges: int;

    for i in 0..(nEdges-1) {
      var v: int = Edges[i]!.n1;
      var w: int = Edges[i]!.n2;

      if ((Nodes[v].nNeighbors > 1) &&
          (Nodes[w].nNeighbors > 1)) then numNewEdges += 1;
    }

    writeln("***********************************************");
    writeln();
    writeln(numNewEdges);
    for i in 0..(nEdges-1) {
      var v: int = Edges[i]!.n1;
      var w: int = Edges[i]!.n2;
      if ((Nodes[v].nNeighbors > 1) &&
          (Nodes[w].nNeighbors > 1)) {
        writeln();
        writeln(v+1, w+1, Edges[i]!.distance);
      }
    }
    writeln("***********************************************");
    writeln();
  }
}

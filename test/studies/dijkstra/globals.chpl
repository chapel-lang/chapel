module NodesEdges {

  class PreEdge {
    var edge: int;
    var next: unmanaged PreEdge?;
  }

  class Edge {
    var id: int;              // edge id
    var n1: int;              // node 1 id, edge between node 1 and node 2
    var n2: int;              // node 2 id, edge between node 1 and node 2
    var dupl: int;            // edge is/is not a duplicate
    var vb: sync real;       // edge betweenness
    var distance: real;       // edge distance

    proc init(id: int = 0,
              n1: int = 0,
              n2: int = 0,
              dupl: int = 0,
              distance: real = 0.0) {
      this.id = id;
      this.n1 = n1;
      this.n2 = n2;
      this.dupl = dupl;
      this.distance = distance;
    }
  }

  class Node {
    var id: int;              // node id
    var nNeighbors: int;      // number of neighbors (Out-degree)
    var vb: sync real;       // node betweeneess
    var NeighborD: domain(1);
    var EdgeIndexD: domain(1);
    var Neighbors: [NeighborD] int;      // list of neighbor ids
    var EdgeIndex: [EdgeIndexD] int;     // list of edge ids

    //New variables:
    var nodeType: int;        //0=Source; 1=Sink; 2=Regular node

    proc init(id: int = 0,
              nNeighbors: int = 0,
              NeighborD: domain(1) = {1..0},
              EdgeIndexD: domain(1) = {1..0},
              nodeType: int = 0) {
      this.id = id;
      this.nNeighbors = nNeighbors;
      this.NeighborD = NeighborD;
      this.EdgeIndexD = EdgeIndexD;
      this.nodeType = nodeType;
    }
  }

  class SimpleEdge {
    var n1: int;              // node 1 id, edge between node 1 and node 2
    var n2: int;              // node 2 id, edge between node 1 and node 2
  }

  class GroupOfEdges {
    var id: int;              // group id
    var size: int;            // size of group
    var vb: real;
    var EdgeList: unmanaged SimpleEdge?; // Vector of edges
  }

  class Record {
    var distance: real;       // shortest distance from S
    var delta: real;
    var sigma: real;
    var onStack: int;         // next node on stack; -1, EOS; -2, not on stack
    var inHeap: int;          // position in heap; -1, not in heap
    var preEdge: unmanaged PreEdge;     // list of edges to predecessor nodes
  }

  class Heap {
    var size: int;            // size of heap
    var leafLevel: int;       // 1/2 maximum size (= 1/2 number of nodes)
    var IdsD: domain(1);
    var Ids: [IdsD] int;      // labels of nodes in heap
  }

}

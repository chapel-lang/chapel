module BC_dijkstra {

  use Heap, NodesEdges;

  proc dijkstra(S: int, nEdges: int, nNodes: int, Edges, Nodes) {
    // Initialize stack
    var stack: int = -1;

    // Initialize records
    var D1 = {0..(nNodes-1)};
    var Records: [D1] unmanaged Record =
     for i in D1 do 
      // onStack = next record on stack; -1 EOS; -2 not on stack
      // inHeap = location of this record in heap
      new unmanaged Record(distance = INFINITY, onStack = -2, inHeap = -1,
                      preEdge = new unmanaged PreEdge(edge = -1, next = nil));

    // Initialize heap
    var heap = new unmanaged Heap(leafLevel = nNodes/2, IdsD = {0..(nNodes-1)});

    for i in D1 do heap.Ids[i] = -1;

    // Put S onto heap
    Records[S].distance = 0.0;
    Records[S].sigma    = 1.0;
    Records[S].inHeap   = 0;

    heap.Ids[0] = S;
    heap.size   = 1;

    while (heap.size != 0) {

      // remove root from heap on push onto stack
      var node: int = heap.Ids[0];
      RemoveRoot(heap, Records);

      Records[node].onStack = stack;
      stack = node;

      var sigma: real = Records[node].sigma;
      var distance: real = Records[node].distance;
      var numberNeighbors: int = Nodes[node].nNeighbors;

      // for each neighbor of node
      label examineneighbors for i in 0..(numberNeighbors-1) {
        var neighbor: int = Nodes[node].Neighbors[i];
        // is neighbor already on stack?
        if (Records[neighbor].onStack != -2) then continue examineneighbors;

        var edgeIndex: int = Nodes[node].EdgeIndex[i];
        // distance from S to neighbor through node
        var newDistance: real = distance + Edges[edgeIndex]!.distance;

        // current shortest distance from S to neighbor
        var neighborDistance: real = Records[neighbor].distance;

        // First shortest path from S to neighbor is found
        //     set neighbor's distance and sigma and insert record in heap
        //     node is the pre node of neighbor
        if (isinf(neighborDistance)) {

          Records[neighbor].distance = newDistance;
          Records[neighbor].sigma = sigma;
          InsertNode(neighbor, heap, Records);

          Records[neighbor].preEdge.edge = edgeIndex;
          Records[neighbor].preEdge.next = nil;

        // A shorter path from S to neighbor is found
        //     reset neighbor's distance and sigma and adjust record's
        //     position in heap node is the pre node of neighbor
        } else if (neighborDistance > newDistance) {

          var ptr: unmanaged PreEdge? = Records[neighbor].preEdge.next;

          Records[neighbor].distance = newDistance;
          Records[neighbor].sigma = sigma;
          HeapUp(Records[neighbor].inHeap, heap, Records);

          Records[neighbor].preEdge.edge = edgeIndex;
          Records[neighbor].preEdge.next = nil;

        // Another shortest path from S to neighbor is found
        //     increment neighbor's sigma
        //     add node as a pre node of neighbor
        } else if (neighborDistance == newDistance) {

          var ptr: unmanaged PreEdge = new unmanaged PreEdge(edge = -1, next = nil);
          Records[neighbor].sigma += sigma;

          ptr.edge = edgeIndex;
          ptr.next = Records[neighbor].preEdge.next;
          Records[neighbor].preEdge.next = ptr;
        }
      }
    }

    // pop node off stack and increment node and edge scores
    while (stack != S) {
      var node: int = stack;
      stack = Records[node].onStack;

//      Nodes[node].vb += Records[node].delta;
      Nodes[node].vb$ += Records[node].delta;

      var ptr: unmanaged PreEdge? = Records[node].preEdge;

      while (ptr != nil) {
        var edge: int = ptr!.edge;
        ptr = ptr!.next;

        var predecessor: int = Edges[edge]!.n1;
        if (predecessor == node) then predecessor = Edges[edge]!.n2;

        var factor: real = Records[predecessor].sigma / Records[node].sigma;

        factor *= (1.0 + Records[node].delta);
        Records[predecessor].delta += factor;

//        Edges[edge].vb += factor;
        Edges[edge]!.vb$ += factor;
      }
    }

    delete heap;

    for r in Records do delete r.preEdge;
    for r in Records do delete r;
  }

}

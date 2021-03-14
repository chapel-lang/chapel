
module Heap {

  use NodesEdges;

  proc Parent(node: int) {
    return ((node - 1)/2);
  }

  proc LeftChild(node: int) {
    return ((2 * node) + 1);
  }

  proc RightChild(node: int) {
    return ((2 * node) + 2);
  }

  proc Swap(n: int, nId: int, m: int, mId: int, heap, records) {
    heap.Ids[n] = mId;
    heap.Ids[m] = nId;
    records[nId].inHeap = m;     // Update records' inHeap fields
    records[mId].inHeap = n;
  }

  proc HeapUp(in node: int, heap, records) {
    var nodeId: int = heap.Ids[node];
    var distance: real = records[nodeId].distance;

    while (node > 0) {
      var parent: int = Parent(node);
      var parentId: int = heap.Ids[parent];

      if (distance >= records[parentId].distance) then break;
      else {
        Swap(node, nodeId, parent, parentId, heap, records);
        node = parent;
      }
    } 
  }

  proc HeapDown(in node: int, heap, records) {
    var nodeId: int = heap.Ids[node];
    var distance: real = records[nodeId].distance;

    while (node < heap.leafLevel) {
      var leftChild: int = LeftChild(node);
      var rightChild: int = RightChild(node);
      var leftChildId: int = heap.Ids[leftChild];
      var rightChildId: int = heap.Ids[rightChild];

      var distanceLeftChild: real;
      if (leftChildId == -1) then distanceLeftChild = INFINITY;
      else distanceLeftChild = records[leftChildId].distance;

      var distanceRightChild: real;
      if (rightChildId == -1) then distanceRightChild = INFINITY;
      else distanceRightChild = records[rightChildId].distance;

      // Swap node with smallest of its two children
      if (distanceLeftChild > distanceRightChild) {
        if (distance <= distanceRightChild) then break;
        else {
          Swap(node, nodeId, rightChild, rightChildId, heap, records);
          node = rightChild;
        }
      } else {
        if (distance <= distanceLeftChild) then break;
        else {
          Swap(node, nodeId, leftChild, leftChildId, heap, records);
          node = leftChild;
        }
      }
    }
  }

  proc RemoveRoot(heap, records) {
    heap.size -= 1;
    var size: int = heap.size;
    var nodeToMove: int = heap.Ids[size];

    heap.Ids[size] = -1;
    heap.Ids[0] = nodeToMove;

    records[nodeToMove].inHeap = 0;
    HeapDown(0, heap, records);
  }

  proc InsertNode(node: int, heap, records) {
    var size: int = heap.size;
    heap.Ids[size] = node;
    heap.size +=1;

    records[node].inHeap = size;
    HeapUp(size, heap, records);
  }

}
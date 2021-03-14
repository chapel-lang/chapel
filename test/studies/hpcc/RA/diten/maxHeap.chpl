class MaxHeap {

  const maxSize: int;
  var size: int = 0;
  var heap: [0..#maxSize] unmanaged HeapNode?;
  var IndexToHeapNode: [0..#maxSize] int;

  class HeapNode {
    var ind: int;
    var key: int;
  }

  proc parent(i: int) {
    return (i-1) / 2;
  }

  proc left(i: int) {
    return 2*i + 1;
  }

  proc right(i: int) {
    return 2*i + 2;
  }

  proc verify(node: int = 0): bool {
    if left(node) < size then
      if heap(left(node)).key > heap(node).key || !verify(left(node)) then
        return false;
    if right(node) < size then
      if heap(right(node)).key > heap(node).key || !verify(right(node)) then
        return false;
    return true;
  }

  proc heapify(node: int) {
    var par, child: int;
    var done = false;
    
    child = node;
    while (!done && child > 0) {
      par = parent(child);
      if par >= 0 && heap(par)!.key < heap(child)!.key {
        var tmp = heap(child);
        heap(child) = heap(par);
        IndexToHeapNode(heap(child)!.ind) = child;
        heap(par) = tmp;
        IndexToHeapNode(heap(par)!.ind) = par;
        child = par;
      } else {
        done = true;
      }
    }
  }

  proc extractMax() {
    var node = heap(0)!;
    var parent, child, ind, key: int;
    ind = node.ind;
    key = node.key;
    size -= 1;
    node = heap(size)!;
    parent = 0;
    child = left(parent);
    while child <= size {
      if child < size && heap(child)!.key < heap(child+1)!.key then
        child += 1;
      if node.key >= heap(child)!.key then
        break;
      heap(parent) = heap(child);
      IndexToHeapNode(heap(parent)!.ind) = parent;
      parent = child;
      child = left(child);
    }
    heap(parent) = node;
    IndexToHeapNode(heap(parent)!.ind) = parent;
    return (ind, key);
  }

  proc increaseKey(ind:int) {
    var node = IndexToHeapNode(ind);
    heap(node)!.key += 1;
    heapify(node);
  }

  proc insert(ind: int, key: int) {
    var node: int = size;
    var par: int = parent(node);
    var newNode = new unmanaged HeapNode(ind, key);
    size += 1;
    while node != 0 && key > heap(par)!.key {
      heap(node) = heap(par);
      IndexToHeapNode(heap(node)!.ind) = node;
      node = par;
      par = parent(node);
    }
    heap(node) = newNode;
    IndexToHeapNode(ind) = node;
  }
  proc printHeap() {
    writeln("Heap:");
    for i in 0..#size {
      var node = heap(i);
      writeln(here.id, ": (", node.ind, ", ", node.key, ")");
    }
  }
}

/*
proc main {
  var heap = new MaxHeap(1024);
  heap.insert(1,1);
  heap.printHeap();
  heap.insert(2,1);
  heap.printHeap();
  heap.insert(3,1);
  heap.printHeap();
  heap.increaseKey(1);
  heap.printHeap();
  heap.increaseKey(1);
  heap.printHeap();
  heap.increaseKey(2);
  heap.printHeap();
  heap.increaseKey(2);
  heap.printHeap();
  heap.increaseKey(2);
  heap.printHeap();
  writeln(heap.verify());

  writeln("Max: ", heap.extractMax());
  writeln("Verify: ", heap.verify());
  heap.printHeap();

  heap.increaseKey(3);
  heap.increaseKey(3);
  heap.increaseKey(3);
  heap.printHeap();
  writeln("Verify: ", heap.verify());
  writeln("Max: ", heap.extractMax());
  heap.printHeap();
  writeln("Verify: ", heap.verify());
}
*/


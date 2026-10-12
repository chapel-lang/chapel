use Heap, Sort;

record minComparator : relativeComparator {
  proc compare(a: int, b: int) {
    return b - a;
  }
}

var h: heap(int, false, minComparator);
h.push(3);
h.push(1);
h.push(5);

writeln(h.top());

while !h.isEmpty() {
  writeln(h.pop());
}

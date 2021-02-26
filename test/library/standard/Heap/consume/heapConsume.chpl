use Heap;
var h = new heap(int);
h.push(5);
h.push(3);
h.push(1);
h.push(2);
for x in h.consume() {
  writeln(x);
}
writeln(h.isEmpty());

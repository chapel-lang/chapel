use Heap;
var h = new heap(int);
for i in 1 .. 3 {
  h.push(i);
  writeln(h.size);
}
for i in 1 .. 3 {
  h.pop();
  writeln(h.size);
}

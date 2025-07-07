use Heap;

var h = new heap(int, parSafe=true);

forall i in 1..1000 with (ref h) {
  h.push(i);
  var t = h.top();
  h.pop();
  h.push(t);
}
writeln(h.size);

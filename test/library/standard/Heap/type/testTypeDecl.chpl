use Heap;

var h: heap(int);
h.push(3);
h.push(1);
h.push(5);
writeln(h.top());

var h2: heap(int, true);
h2.push(10);
writeln(h2.top());

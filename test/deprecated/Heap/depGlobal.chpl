use Heap;

var h = new heap(string, false, defaultComparator);

h.push("foo");
h.push("bar");
writeln(h.top());

var h2 = new heap(string, false, reverseComparator);
h2.push("foo");
h2.push("bar");
writeln(h2.top());

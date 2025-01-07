use Heap;

var h = new heap(int);
h.push(6);

writeln("Before: isEmpty = ", h.isEmpty());
writeln("Before: size = ", h.size);
h.clear();
writeln("After: isEmpty = ", h.isEmpty());
writeln("After: size = ", h.size);

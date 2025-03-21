use Heap;
use Random;

var h = new heap(int);
var rs = new randomStream(int);

for 0..25 {
  h.push(rs.next());
}

writeln("Before: isEmpty = ", h.isEmpty(), ", size = ", h.size);
h.clear();
writeln("After: ", h);

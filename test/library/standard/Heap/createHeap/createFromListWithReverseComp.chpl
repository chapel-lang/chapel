use List;
use Heap;

var l:list(int) = [2, 1, 3];
var h = createHeap(l, comparator = new ReverseComparator());

writeln(h);

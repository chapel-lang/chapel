use Heap;
import HeapTest;

proc <(const ref left: HeapTest.T?, const ref right: HeapTest.T?) {
  if (left == nil || right == nil) then return false;
  return left!.value < right!.value;
}

var l = new heap(borrowed HeapTest.T?);

var a = new HeapTest.T();
var b: borrowed HeapTest.T? = a.borrow();
l.push(b);

assert(l.size == 1);

var value = l.top();
l.pop();
assert(l.size == 0);

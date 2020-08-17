use Heap;

class T {
  var value = 0;
}

proc <(const ref left: T, const ref right: T) {
  return left.value < right.value;
}
proc <(const ref left: T?, const ref right: T?) {
  if (left == nil || right == nil) then return false;
  return left!.value < right!.value;
}

// It's needed for comparing (T, T)
proc >(const ref left: T, const ref right: T) {
  return left.value > right.value;
}
proc >(const ref left: T?, const ref right: T?) {
  if (left == nil || right == nil) then return false;
  return left!.value > right!.value;
}

proc testHeap(type t) where isTuple(t) {
  var l = new heap(t);
  var x = (new t[0](1), new t[1](2));

  l.push(x);
  assert(l.size == 1);

  var value = l.top();
  l.pop();
  assert(l.size == 0);
}

proc testHeap(type t) {
  var l = new heap(t);

  var x: t = new t(1);

  l.push(x);
  assert(l.size == 1);

  var value = l.top();
  l.pop();
  assert(l.size == 0);

  if isUnmanagedClass(t) {
    delete x;
  }

}

proc testHeapOwned(type t) {
  var l = new heap(t);

  var x: t = new t(1);

  l.push(x);
  assert(l.size == 1);

  var value = l.pop();
  assert(l.size == 0);

  if isUnmanagedClass(t) {
    delete x;
  }

}

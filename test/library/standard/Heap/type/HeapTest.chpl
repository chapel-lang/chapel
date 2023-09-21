use Heap;

class T {
  var value = 0;
}

operator T.<(const ref left: T, const ref right: T) {
  return left.value < right.value;
}
operator T.<(const ref left: T?, const ref right: T?) {
  if (left == nil || right == nil) then return false;
  return left!.value < right!.value;
}

// It's needed for comparing (T, T)
operator T.>(const ref left: T, const ref right: T) {
  return left.value > right.value;
}
operator T.>(const ref left: T?, const ref right: T?) {
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

proc testHeap(type t) where isBorrowedClass(t) {
  var l = new heap(t);

  // create values with 'owned' if t is a borrowed class type
  // (the set will still store borrowed)
  type useT = (t:owned);

  var x: useT = new useT(1);

  l.push(x.borrow());
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

  if !isOwnedClass(t) {
    var value = l.top();
  }
  l.pop();
  assert(l.size == 0);

  if isUnmanagedClass(t) {
    delete x;
  }
}

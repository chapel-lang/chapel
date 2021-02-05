import UnrolledLinkedList.unrolledLinkedList;

class T {
  var value = 0;
}

var l = new unrolledLinkedList(borrowed T?);

var a = new T();
var b: borrowed T? = a.borrow();
l.append(b);

assert(l.size == 1);

var value = l.pop();
assert(l.size == 0);

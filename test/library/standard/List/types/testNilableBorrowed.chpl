import List.list;

class T {
  var value = 0;
}

var l = new list(borrowed T?);

var a = new T();
var b: borrowed T? = a.borrow();
l.append(b);

assert(l.size == 1);

var value = l.pop();
assert(l.size == 0);

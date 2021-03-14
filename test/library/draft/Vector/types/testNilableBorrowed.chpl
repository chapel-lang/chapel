import Vector.vector;

class T {
  var value = 0;
}

var l = new vector(borrowed T?);

var a = new T();
var b: borrowed T? = a.borrow();
l.append(b);

assert(l.size == 1);

var value = l.pop();
assert(l.size == 0);

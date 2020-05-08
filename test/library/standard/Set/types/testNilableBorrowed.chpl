import Set.set;

class T {
  var value = 0;
}

var s = new set(borrowed T?);

var a = new T();
var b: borrowed T? = a.borrow();
s.add(b);
assert(s.size == 1);

s.remove(b);
assert(s.size == 0);

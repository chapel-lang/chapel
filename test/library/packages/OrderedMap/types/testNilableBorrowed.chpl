use OrderedMap;

config param impl: mapImpl;

class T {
  var value = 0;
}

var m = new orderedMap(int, borrowed T?, false, defaultComparator, impl);

var a = new T(1);
var b: borrowed T? = a.borrow();

var ret = m.add(1, b);
assert(ret);
assert(m.contains(1));

var x = new T(2);
var y: borrowed T? = x.borrow();

ret = m.add(1, y);
assert(!ret);

ret = m.remove(1);
assert(ret);
assert(!m.contains(2));

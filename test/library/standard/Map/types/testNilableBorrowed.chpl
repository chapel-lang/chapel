import Map.map;

class T {
  var value = 0;
}

var m = new map(int, borrowed T?);

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

var z = new T(3);
var zz: borrowed T? = z.borrow();

m[4] = zz;
assert(m.contains(4));
var zzz: borrowed T? = m[4];
assert(zzz!.value == 3);

ret = m.add(0, nil);
assert(ret);
assert(m.contains(0));
var zzzz = m.getValue(0);
assert(zzzz == nil);
ret = m.remove(0);
assert(ret);

ret = m.remove(4);
assert(ret);

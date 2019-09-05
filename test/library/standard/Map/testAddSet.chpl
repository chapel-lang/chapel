use Map;

var m = new map(int, int);

var ret:bool = m.add(1, -1);
assert(ret);
assert(m.contains(1));

ret = m.add(1, -1);
assert(!ret);

ret = m.add(2, -2);
assert(ret);
assert(m.contains(2));

ret = m.set(2, -3);
assert(ret);
assert(m[2] == -3);

ret = m.set(3, -3);
assert(!ret);
assert(!m.contains(3));

ret = m.remove(2);
assert(ret);
assert(!m.contains(2));

ret = m.remove(3);
assert(!ret);

writeln(m);

private use UnrolledLinkedList;

config const testIters = 16;
config const testElem = 42;
config const testCap = 6;

var lst = new unrolledLinkedList(int, false, testCap);

for i in 0..<testIters do
  lst.append(i);

for i in 0..<testIters {
  assert(lst.insert(i*2, i+testIters));
}
writeln(lst);

var oldSize = lst.size;

// Insert at -1 should fail.
assert(!lst.insert(-1, testElem));
assert(lst.size == oldSize);
writeln(lst);

// Insert at size should always work.
assert(lst.insert(lst.size, testElem));
writeln(lst);

lst.clear();
assert(lst.size == 0);

assert(lst.insert(0, testElem));
writeln(lst);

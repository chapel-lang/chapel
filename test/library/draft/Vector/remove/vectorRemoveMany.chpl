use Vector;

config const testElem = 42;
config const testIters = 1024;

type listType = int;
param listLock = true;

var vec: vector(listType, listLock);
var elemCount = 0;

for i in 1..testIters {
  var elem = if i % 2 == 0 then i else testElem;
  if i == testElem then
    elem *= -1;
  if elem == testElem then
    elemCount += 1;
  vec.append(elem);
}

assert(vec.size == testIters);

var oldSize = vec.size;

var removed = vec.remove(testElem, elemCount / 2);

assert(removed == elemCount / 2);
assert(vec.size == oldSize - removed);

//
// Now remove all remaining elements from the vector.
//

removed = vec.remove(testElem, 0);

assert(removed == elemCount / 2);
assert(vec.size == oldSize - elemCount);
assert(vec.count(testElem) == 0);

vec.clear();

// Check that remove() handles consecutive runs of elements to remove
for i in 1..10 {
  vec.append(testElem);
}
for i in 1..10 {
  vec.append(testElem * 2);
}

removed = vec.remove(testElem, 0);
writeln(removed);
writeln(vec);

var slist: vector(string, listLock);
for w in [ "do", "do", "de", "do" ] do
  slist.append(w);

removed = slist.remove("do", 0);
writeln(removed);
writeln(slist);

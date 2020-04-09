use List;

config const testElem = 42;
config const testIters = 1024;

type listType = int;
param listLock = true;

var lst: list(listType, listLock);
var elemCount = 0;

for i in 1..testIters {
  var elem = if i % 2 == 0 then i else testElem;
  if i == testElem then
    elem *= -1;
  if elem == testElem then
    elemCount += 1;
  lst.append(elem);
}

assert(lst.size == testIters);

var oldSize = lst.size;

var removed = lst.remove(testElem, elemCount / 2);

assert(removed == elemCount / 2);
assert(lst.size == oldSize - removed);

//
// Now remove all remaining elements from the list.
//

removed = lst.remove(testElem, 0);

assert(removed == elemCount / 2);
assert(lst.size == oldSize - elemCount);
assert(lst.count(testElem) == 0);



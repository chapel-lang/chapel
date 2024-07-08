use List;

config const testNum = 42;

//
// Use a higher iteration count to try and trigger a resize.
//
config const testIters = 32;

type listType = int;
param listLock = true;

var lst1 : list(listType, listLock);

writeln(lst1.count(testNum));

for 1..testIters do {
  lst1.pushBack(testNum);
  const count = lst1.count(testNum);
  writeln(count);
}

var lst2 = lst1;

lst2.pushBack(lst1);

writeln(lst2.count(testNum));

lst1.clear();
lst2.clear();

writeln(lst1.count(testNum));
writeln(lst2.count(testNum));

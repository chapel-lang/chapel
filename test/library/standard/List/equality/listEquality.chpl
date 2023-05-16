use List;

config type listType = int;
config param listLock = true;

config const testIters = 1024;

var lst1 = new list(listType, listLock);

for i in 1..testIters do
  lst1.pushBack(i);

var lst2 = new list(listType, listLock);

writeln(lst1 != lst2);

lst2.pushBack(lst1);

writeln(lst1 == lst2);

lst1.clear();

writeln(lst1 != lst2);

lst2.clear();

writeln(lst1 == lst2);

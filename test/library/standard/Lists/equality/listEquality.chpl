use Lists;

config type listType = int;
config param listLock = false;

config const testIters = 1024;

var lst1 = new List(listType, listLock);

for i in 1..testIters do
  lst1.append(i);

var lst2 = new List(listType, listLock);

writeln(lst1 != lst2);

lst2.extend(lst1);

writeln(lst1 == lst2);

lst1.clear();

writeln(lst1 != lst2);

lst2.clear();

writeln(lst1 == lst2);

use Lists;

config const testNum = 42;
config const testIters = 8;

type listType = int;
param listLock = true;

var lst1 : List(listType, listLock);

writeln(lst1.count(testNum));

for 1..testIters do {
  lst1.append(testNum);
  writeln(lst1.count(testNum));
}

var lst2 = lst1;

lst2.extend(lst1);

writeln(lst2.count(testNum));

lst1.clear();
lst2.clear();

writeln(lst1.count(testNum));
writeln(lst2.count(testNum));

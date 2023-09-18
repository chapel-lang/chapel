use List;


config type listType = int;
config param listLock = true;

config const testIters = 2 * 1024 * 1024;

var lst1 = new list(listType, listLock);

writeln(lst1.size);

for i in 1..testIters do {
  lst1.pushBack(i);
}

var lst2 = lst1;

writeln(lst1.size);
writeln(lst2.size);

lst1.clear();

writeln(lst1.size);
writeln(lst2.size);

lst2.clear();

writeln(lst2.size);

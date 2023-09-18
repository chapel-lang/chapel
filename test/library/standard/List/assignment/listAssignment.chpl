use List;

type listType = int;
param listLock = true;

var lst1: list(listType, listLock);

for i in 1..8 do
  lst1.pushBack(i);

var lst2: list(listType, listLock);

writeln(lst1);
writeln(lst2);

lst2 = lst1;

writeln(lst1);
writeln(lst2);

lst1.clear();

writeln(lst1);
writeln(lst2);

lst2 = lst1;

writeln(lst1);
writeln(lst2);

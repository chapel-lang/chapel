use List;

type listType = int;
param listLock = true;


var lst: list(listType, listLock);

writeln(lst.isEmpty());

for i in 1..5 do
  lst.pushBack(i:listType);

writeln(lst.isEmpty());

lst.clear();

writeln(lst.isEmpty());

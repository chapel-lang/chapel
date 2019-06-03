use Lists;

config type listType = int;
config param listLock = true;

var lst = new List(listType, listLock);

for i in 1..8 by -1 do
  lst.append(i);

writeln(lst);

lst.sort();

writeln(lst);

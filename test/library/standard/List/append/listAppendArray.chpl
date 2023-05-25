use List;

// Control these via COMPOPTS to test class types and no parallel safety.
config type listType = int;
config param listLock = true;

var lst1 = new list(listType, listLock);

for i in 1..5 do
  lst1.pushBack(i);

writeln(lst1);

var lst2 = new list(listType, listLock);
var arr1: [1..5] listType = [6, 7, 8, 9, 10];

lst2.pushBack(arr1);

writeln(lst2);

lst1.pushBack(lst2);

writeln(lst1);

lst2.clear();

writeln(lst1);
writeln(lst2);

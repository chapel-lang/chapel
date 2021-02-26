use UnrolledLinkedList;

// Control these via COMPOPTS to test class types and no parallel safety.
config type listType = int;
config param listLock = true;

var lst1 = new unrolledLinkedList(listType, listLock);

for i in 1..5 do
  lst1.append(i);

writeln(lst1);

var lst2 = new unrolledLinkedList(listType, listLock);
var arr1: [1..5] listType = [6, 7, 8, 9, 10];

lst2.extend(arr1);

writeln(lst2);

lst1.extend(lst2);

writeln(lst1);

lst2.clear();

writeln(lst1);
writeln(lst2);

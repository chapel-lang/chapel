use UnrolledLinkedList;

config type listType = int;
config param listLock = true;

config const testIters = 8;


var lst = new unrolledLinkedList(listType, listLock);

for i in 1..testIters do
  lst.append(i);

var arr = lst.toArray();

writeln(lst);
writeln(arr);


use UnrolledLinkedList;

config type listType = int;
config param listLock = true;

config const stop = 31;

var lst = new unrolledLinkedList(listType, listLock, 6);

for i in 1..stop do {
  var x = i:listType;
  lst.append(x);
}

writeln(lst);

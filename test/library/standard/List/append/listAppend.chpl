use List;

config type listType = int;
config param listLock = true;

config const stop = 16;

var lst = new list(listType, listLock);

for i in 1..stop do {
  var x = i:listType;
  lst.pushBack(x);
}

writeln(lst);

use Lists;

config type listType = int;
config param listLock = true;

var lst : List(listType, listLock);
writeln(lst);

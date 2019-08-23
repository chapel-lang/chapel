use List;

config type listType = int;
config param listLock = true;

var lst : list(listType, listLock);
writeln(lst);

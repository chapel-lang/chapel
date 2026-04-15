private use List;

var lst = new list(int);
lst.pushBack(1);
lst.pushBack(2);
lst.pushBack(3);

writeln(lst.contains(proc(x: int): bool { return x == 2; }));
writeln(lst.find(proc(x: int): bool { return x == 2; }));
